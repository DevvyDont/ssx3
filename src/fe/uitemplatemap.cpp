#include "common.h"
#include "fe/uitemplatemap.h"
#include "ui/uistate.h"
#include "fe/ovstatetransport.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cUITemplate_MAP_setShowInfo(cUITemplate_MAP* self, int show);
extern "C" int cFEAsyncManager_GetFreeAsyncFile(void* mgr);
extern "C" void cUITemplate_MAP_setupPeakUnlock(cUITemplate_MAP* self);
struct cUITemplate_MAP;
void cUITemplate_MAP_setupEventUnlock(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupLayout(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupMapPic(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupPopup(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupPeakInfo(cUITemplate_MAP* self);
extern unsigned int cUITemplate_MAP_mMapStyle;
extern int D_004A2598;
extern int cUITemplate_MAP_mMapState;
extern char str_MapPic[];
extern char str_maptab[];
extern char str_pdaart[];
extern char str_feart[];
extern char str_pdaxbb[];
extern char str_pdangcb[];
extern char str_PEAKno[];
extern char str_femaptab_back[];
extern char str_pdatitle[];
extern char str_pdabuttons[];
extern char str_locktab1[];
extern char str_locktab2[];
extern char str_title_title[];
extern char str_pdabtext2[];
extern char str_pdaps2tri[];
extern char str_pdaPEAKno_back[];
extern char str_fePEAKno_back[];
extern char str_SP_GameInfo[];
extern char str_HelpText[];
extern char str_peak1outline[];
extern char str_peak2outline[];
extern char str_peak3outline[];
extern char str_P1you_are_here[];
extern char str_P2you_are_here[];
extern char str_P3you_are_here[];
extern char str_SP_InfoDataTitle[];

class cUIObj_200700 {
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
    virtual void setVisible(int on);
};

struct sMapTmpl_200700 {
    char pad0[0x2CC];
    int popup;                  // 0x2CC
    int mode;                   // 0x2D0
    int pic;                    // 0x2D4
    int file;                   // 0x2D8
    int f2DC;
    int f2E0;
    cUIObj_200700* map;         // 0x2E4
    void* screen;               // 0x2E8
    void* owner;                // 0x2EC
    int f2F0;                   // 0x2F0
};

/**
 * Sets up the mountain map template on its screen: unlocks, layout, map picture, popup and the widgets per
 * map style.
 *
 * The map style (cUITemplate_MAP_mMapStyle; On Tour eMapStyle: 0/1 select peak event in the front end or back end, 2 pause
 * map, 3
 * transport, 4 end-of-event transport) decides which tabs and titles show; styles 1, 3 and 4 add the PDA
 * template.
 * Starts in the select-peak state unless returning to a front-end map, and requests the map file from the
 * async
 * manager.
 *
 * @param tmpl  the map template
 * @param screen  the map screen (On Tour cUIScreen* screen)
 * @param owner  the UI state the template belongs to (On Tour cUIState* pState)
 */
extern "C" void cUITemplate_MAP_onCreateScreen(cUITemplate_MAP* tmpl, void* screen, cUIState* owner)
{
    sMapTmpl_200700* self = (sMapTmpl_200700*)tmpl;
    self->owner = ((void*)owner);
    self->screen = screen;
    if (cUITemplate_MAP_mMapStyle == 1 || cUITemplate_MAP_mMapStyle == 3 || cUITemplate_MAP_mMapStyle == 4)
        cOVStateManager_addPDATemplate();
    if (cUITemplate_MAP_mMapStyle != 0 || D_004A2598 == 0)
        cUITemplate_MAP_mMapState = 0;
    cUITemplate_MAP_setShowInfo((cUITemplate_MAP*)self, 0);
    self->popup = 0;
    self->mode = -1;
    self->file = cFEAsyncManager_GetFreeAsyncFile(SSXApp->mFEAsyncManager);
    self->pic = 0;
    cUIObj_200700* mapObj = (cUIObj_200700*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_MapPic));
    self->map = mapObj;
    if (mapObj != 0)
        mapObj->setVisible(0);
    self->f2F0 = 0;
    cUITemplate_MAP_setupPeakUnlock((cUITemplate_MAP*)self);
    cUITemplate_MAP_setupEventUnlock((cUITemplate_MAP*)self);
    cUITemplate_MAP_setupLayout((cUITemplate_MAP*)self);
    cUITemplate_MAP_setupMapPic((cUITemplate_MAP*)self);
    cUITemplate_MAP_setupPopup((cUITemplate_MAP*)((char*)self));
    if (cUITemplate_MAP_mMapStyle >= 2)
        cUITemplate_MAP_setupPeakInfo((cUITemplate_MAP*)self);
    if (cUITemplate_MAP_mMapStyle < 2) {
        cUIState_hideObjSafe(self->owner, str_femaptab_back);
        cUIState_hideObjSafe(self->owner, str_maptab);
        if (cUITemplate_MAP_mMapStyle == 0) {
            cUIState_hideObjSafe(self->owner, str_pdaart);
        } else {
            cUIState_hideObjSafe(self->owner, str_pdatitle);
            cUIState_hideObjSafe(self->owner, str_pdabuttons);
            cUIState_hideObjSafe(self->owner, str_locktab1);
            cUIState_hideObjSafe(self->owner, str_locktab2);
        }
    } else {
        cUIState_hideObjSafe(self->owner, str_feart);
        cUIState_showObjSafe(self->owner, str_title_title);
    }
    if (cUITemplate_MAP_mMapStyle == 4) {
        cUIState_hideObjSafe(self->owner, str_pdabtext2);
        cUIState_hideObjSafe(self->owner, str_pdaps2tri);
        cUIState_hideObjSafe(self->owner, str_pdaxbb);
        cUIState_hideObjSafe(self->owner, str_pdangcb);
    }
    cUIState_hideObjSafe(self->owner, str_PEAKno);
    cUIState_hideObjSafe(self->owner, str_pdaPEAKno_back);
    cUIState_hideObjSafe(self->owner, str_fePEAKno_back);
    cUIState_hideObjSafe(self->owner, str_SP_GameInfo);
    cUIState_hideObjSafe(self->owner, str_HelpText);
    cUIState_hideObjSafe(self->owner, str_peak1outline);
    cUIState_hideObjSafe(self->owner, str_peak2outline);
    cUIState_hideObjSafe(self->owner, str_peak3outline);
    cUIState_hideObjSafe(self->owner, str_P1you_are_here);
    cUIState_hideObjSafe(self->owner, str_P2you_are_here);
    cUIState_hideObjSafe(self->owner, str_P3you_are_here);
    cUIState_hideObjSafe(self->owner, str_SP_InfoDataTitle);
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
int GetHashValue32(char* str);
extern char str_SP_PeakList[];
extern unsigned int cUITemplate_MAP_mMapStyle;
static inline int isLabel2009D0(int id, char* s)
{
    return id == GetHashValue32(s);
}
// PORT: the unit declares cUITemplate_MAP_onWidgetCreate(void*); the body also takes the menu in $5.
extern "C" void cUITemplate_MAP_onWidgetCreate_r(void* self, void* menu) __asm__("cUITemplate_MAP_onWidgetCreate");

/**
 * Preselects the map's peak list when it is created: entry 2, or for map styles 2 and up the entry of the current race
 * location's peak.
 *
 * Only for the widget whose hash name (+0x38) is "SP_PeakList": with cUITemplate_MAP_mMapStyle 0 or 1 it selects index
 * 2; otherwise 2 minus the peak byte at +0x54 of cBENewRaceInterface_getLocation (race interface 0), so the list runs
 * from the last peak to the first.
 *
 * @param menu  the widget being created (On Tour cUIThing* pWidget)
 */
extern "C" void cUITemplate_MAP_onWidgetCreate_r(void* self, void* menu)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (isLabel2009D0(*(int*)((char*)menu + 0x38), str_SP_PeakList)) {
        if (cUITemplate_MAP_mMapStyle < 2) {
            cUIMenu_setSelectedByIndex(menu, 2);
        } else {
            cUIMenu_setSelectedByIndex(menu, 2 - *(unsigned char*)((char*)cBENewRaceInterface_getLocation(iface) + 0x54));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(cUITemplate_MAP* self, int focus);
extern "C" void cUITemplate_MAP_setupMenus(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupLocationInfo(void* self);
extern "C" void cUITemplate_MAP_setShowInfo(cUITemplate_MAP* self, int show);
extern int cUITemplate_MAP_mMapState;
// PORT: the unit declares cUITemplate_MAP_onGainTransition(void*); the body also takes a flag in $5.
extern "C" int cUITemplate_MAP_onGainTransition_r(void* self, int on) __asm__("cUITemplate_MAP_onGainTransition");

/**
 * Sets up the map screen's menus when its gain transition starts.
 *
 * When @p on is nonzero: sets the menu focus for cUITemplate_MAP_mMapState, builds the menus, calls
 * cUITemplate_MAP_setupLocationInfo() and passes the object's first word to cUITemplate_MAP_setShowInfo(). Does nothing otherwise.
 *
 * @param on  nonzero to run the setup
 * @return always 0
 */
extern "C" int cUITemplate_MAP_onGainTransition_r(void* self, int on)
{
    if (on != 0) {
        cUITemplate_MAP_setupMenuFocus((cUITemplate_MAP*)self, cUITemplate_MAP_mMapState);
        cUITemplate_MAP_setupMenus((cUITemplate_MAP*)self);
        cUITemplate_MAP_setupLocationInfo(self);
        cUITemplate_MAP_setShowInfo((cUITemplate_MAP*)self, *(int*)self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupMenuFocus(cUITemplate_MAP* self, int focus);
extern "C" void* cAudioInterface_bxAudio(void);
extern "C" void func_0028F5B8(void* mgr);
extern int cUITemplate_MAP_mMapState;

/**
 * Restores the map screen's menu focus for the current map state when it gains focus.
 *
 * Calls cUITemplate_MAP_setupMenuFocus() for cUITemplate_MAP_mMapState, then func_0028F5B8() on the game audio
 * object returned by cAudioInterface_bxAudio().
 */
extern "C" void cUITemplate_MAP_onGainFocus(cUITemplate_MAP* self)
{
    cUITemplate_MAP_setupMenuFocus(self, cUITemplate_MAP_mMapState);
    func_0028F5B8(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void* cAudioInterface_bxAudio(void);
extern "C" void func_0028F678(void* mgr, int a);
extern unsigned int cUITemplate_MAP_mMapStyle;

/**
 * Tears down the map template's screen: unloads its map picture and, for map styles 1, 3 and 4, removes the PDA
 * template.
 *
 * Unloads the FE async file whose index is at +0x2D8 (On Tour mFEAsyncMapFileIndex) through SSXApp->mFEAsyncManager
 * (cFEAsyncManager_UnloadFEAsyncFile), calls cOVStateManager_removePDATemplate for cUITemplate_MAP_mMapStyle 1, 3 or 4,
 * and finally calls func_0028F678(cAudioInterface_bxAudio(), 0) on the game audio. cOVState_TRANSPORT_onDestroyScreen and
 * cFEStateEventSelect_onDestroyScreen call it for their embedded template.
 */
extern "C" void cUITemplate_MAP_onDestroyScreen(cUITemplate_MAP* self)
{
    cFEAsyncManager_UnloadFEAsyncFile(SSXApp->mFEAsyncManager, self->mFEAsyncMapFileIndex);
    if (cUITemplate_MAP_mMapStyle == 1 || cUITemplate_MAP_mMapStyle == 3 || cUITemplate_MAP_mMapStyle == 4) {
        cOVStateManager_removePDATemplate();
    }
    func_0028F678(cAudioInterface_bxAudio(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_isMapEventCompleted);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
int cBERewardInterface_getTrackMedal(void* self, int player, int charID, int mode, int track);
// PORT: the unit declares cBENewPlayerInterface_isEventLocked as returning int; its result is negated as a bool here.
bool cBENewPlayerInterface_isEventLocked_b(void* iface, int player, int charID, int x) __asm__("cBENewPlayerInterface_isEventLocked");

// The unit's sPeakGoal (0x6C bytes), defined further down; only the leading fields are read here.
struct sMapEvent_0B50 { int track; int f4; int kind; int rest[0x6C / 4 - 3]; };

// PORT: the unit declares cUITemplate_MAP_isMapEventCompleted(void*, sPeakGoal*, int) further down; the goal is really taken by value
// (the callee copies it), so this definition binds the by-value form to the symbol. Each kind returns through its
// own call; g++ merges the two identical tails.
extern "C" int cUITemplate_MAP_isMapEventCompleted_byval(void* self, sMapEvent_0B50 ev, int peak) __asm__("cUITemplate_MAP_isMapEventCompleted");
/**
 * Tells whether the map location's event has been completed by the current character.
 *
 * Track 23 never is. Kind 4 maps @p peak (0, 1, 2) to game event 6, 7 or 8 and kind 5 to 9, 10 or 11, and counts as
 * done when cBENewPlayerInterface_isEventLocked reports that event unlocked for character cBELibrary_getCharacterID(0);
 * any other kind is done when cBERewardInterface_getTrackMedal returns a medal (not -1) for its kind and track. The
 * location is taken by value (a 0x6C-byte sPeakGoal).
 *
 * @param ev    the map location: track (+0x0) and event kind (+0x8) (On Tour tMapLocation)
 * @param peak  the peak (On Tour eBEPeakID)
 * @return 1 when completed, otherwise 0
 */
extern "C" int cUITemplate_MAP_isMapEventCompleted_byval(void* self, sMapEvent_0B50 ev, int peak)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* rw = cBE_getInterface_Fv(cBE_getBE(), 13);
    int cid = cBELibrary_getCharacterID(0);
    if (ev.track == 23)
        return 0;
    if (ev.kind == 4) {
        int x = 6;
        if (peak == 1)
            x = 7;
        if (peak == 2)
            x = 8;
        return !cBENewPlayerInterface_isEventLocked_b(np, 0, cBELibrary_getCharacterID(0), x);
    } else if (ev.kind == 5) {
        int x = 9;
        if (peak == 1)
            x = 10;
        if (peak == 2)
            x = 11;
        return !cBENewPlayerInterface_isEventLocked_b(np, 0, cBELibrary_getCharacterID(0), x);
    }
    return cBERewardInterface_getTrackMedal(rw, 0, cid, ev.kind, ev.track) != -1;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_isMapEventLocked);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int cBENewPlayerInterface_isRivalEventLocked(void* iface, int player, int charID, int x, int flag);
extern "C" int cBENewPlayerInterface_isEventLocked(void* iface, int player, int charID, int x);

/**
 * Tells whether a map event is still locked for the current character.
 *
 * Nothing is locked while cUITemplate_MAP_mMapStyle is below 2. Kinds 4 and 5 ask cBENewPlayerInterface_isRivalEventLocked (On Tour
 * isRivalEventLocked) for the peak, with its time-challenge flag 1 for kind 4 and 0 for kind 5; kinds 6 to 11 ask
 * cBENewPlayerInterface_isEventLocked; every other kind is unlocked. The character is cBELibrary_getCharacterID(0).
 *
 * @param kind  the game event kind
 * @param x     the peak (On Tour eBEPeakID)
 * @return nonzero when the event is locked
 */
extern "C" int cUITemplate_MAP_isMapEventLocked(cUITemplate_MAP* self, int kind, int x)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (cUITemplate_MAP_mMapStyle < 2) {
        return 0;
    }
    if (kind == 4) {
        return cBENewPlayerInterface_isRivalEventLocked(iface, 0, cBELibrary_getCharacterID(0), x, 1);
    }
    if (kind == 5) {
        return cBENewPlayerInterface_isRivalEventLocked(iface, 0, cBELibrary_getCharacterID(0), x, 0);
    }
    if (kind >= 6 && kind <= 11) {
        return cBENewPlayerInterface_isEventLocked(iface, 0, cBELibrary_getCharacterID(0), kind);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenus);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.
class cUIThing {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void setEnabled(int on);
    virtual void setFocus(int on);
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

struct cUIText;

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int cUITemplate_MAP_mMapStyle;

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern char str_ConfirmMenu_No[];

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).
struct sPeakGoal {
    int track;          // 0x00
    int f04;
    int kind;           // 0x08
    int f0C;
    int useTrackName;   // 0x10
    char name[0x1E];    // 0x14
    char help[0x23];    // 0x32 help-text key (cUITemplate_MAP_setupLocationInfo)
    char path[0x17];    // 0x55
};

extern sPeakGoal D_004781D0_enus[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_enus[3][5] __asm__("D_004786E0");

extern sPeakGoal D_00478D38_enus[3][8] __asm__("D_00478D38");

extern "C" void cUITemplate_MAP_setWidgetUnHiliteColor_enus(void* self, void* obj, int on) __asm__("cUITemplate_MAP_setWidgetUnHiliteColor");
extern "C" int cUITemplate_MAP_isMapEventCompleted(void* self, sPeakGoal* loc, int peak);
extern "C" int cUITemplate_MAP_isMapEventLocked_enus(void* self, int kind, int x) __asm__("cUITemplate_MAP_isMapEventLocked");
extern "C" int func_004165A8(const char*, const char*);
extern "C" const char* cBENewRaceInterface_getLocationName_enus(void* iface, int id) __asm__("cBENewRaceInterface_getLocationName");
extern "C" unsigned short* func_002C2540_enus(unsigned short* dst, const char* s) __asm__("func_002C2540");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" unsigned short* func_001FD190(int kind);
extern "C" void cUIText_setUnicodeString_enus(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" char* strcpy(char* d, const char* s);
extern char str_Peak3[];
extern char str_Peak2[];
extern char str_Peak1[];
extern char str_Mountain[];
extern char str_Peakpct_dRaceGoal[];
extern char str_Peakpct_dFreestyleGoal[];
extern char str_Peakpct_dFreerideGoal[];
extern char str_Peakpct_dEarningsGoal[];
extern char D_004A2560[];
extern char D_004A2590[];
extern char str_ConfirmMenu_Yes[];
extern char kMAPCOLOR_REGULAR[];

// A menu entry widget: cUIThing plus its menu index at 0x18; slot 11 sets the text colour.
class cUIObj_Menus : public cUIThing {
public:
    int f0C;
    int f10;
    int f14;
    int idx;            // 0x18
    void setIndex(int i) { idx = i; }
    virtual void v10();
    virtual void setColor(char* col);
};


// Location entries with track 0x17 have no event: no availability colour or text, the widget is made static.
static inline int hasEvent(int track) { return track != 0x17; }

/**
 * Fills the map's menus: the peak entries, each peak's goal entries and its race, freestyle and freeride
 * locations.
 *
 * Locked entries get the locked colour; in the select-peak-event styles (0, 1) the freeride and earnings
 * goals are
 * hidden and so are locations flagged in the tables. Location names come from the race setup, the table or
 * the
 * event kind. Also sets up the confirm menu's Yes and No.
 *
 * @param tmpl  the map template
 */
extern "C" void cUITemplate_MAP_setupMenus(cUITemplate_MAP* tmpl)
{
    cUITemplate_MAP* self = tmpl;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cUIObj_Menus* menuObj;
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_Peak3));
    if (menuObj != 0) {
        menuObj->setIndex(2);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbPeakUnlocked[2]);
    }
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_Peak2));
    if (menuObj != 0) {
        menuObj->setIndex(1);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbPeakUnlocked[1]);
    }
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_Peak1));
    if (menuObj != 0) {
        menuObj->setIndex(0);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbPeakUnlocked[0]);
    }
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_Mountain));
    if (menuObj != 0) {
        menuObj->setIndex(3);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, 0);
    }
    char buf[32];
    for (int i = 0; i < 3; i++) {
        sprintf(buf, str_Peakpct_dRaceGoal, i + 1);
        menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(buf));
        menuObj->setIndex(0);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbEventUnlocked[0]);
        sprintf(buf, str_Peakpct_dFreestyleGoal, i + 1);
        menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(buf));
        menuObj->setIndex(1);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbEventUnlocked[1]);
        sprintf(buf, str_Peakpct_dFreerideGoal, i + 1);
        menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(buf));
        menuObj->setIndex(2);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbEventUnlocked[2]);
        if (cUITemplate_MAP_mMapStyle < 2) {
            menuObj->setStatic(1);
            menuObj->setVisible(0);
        }
        sprintf(buf, str_Peakpct_dEarningsGoal, i + 1);
        menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(buf));
        menuObj->setIndex(3);
        cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbEventUnlocked[3]);
        if (cUITemplate_MAP_mMapStyle < 2) {
            menuObj->setStatic(1);
            menuObj->setVisible(0);
        }
    }
    for (int peak = 0; peak < 3; peak++) {
        for (int mode = 0; mode < 3; mode++) {
            int count;
            char fmt[24];
            if (mode == 0) {
                count = 4;
                strcpy(fmt, "Peak%dRaceLoc%d");
            } else if (mode == 1) {
                count = 5;
                strcpy(fmt, "Peak%dFreestyleLoc%d");
            } else {
                count = 8;
                strcpy(fmt, "Peak%dFreerideLoc%d");
            }
            for (int j = 0; j < count; j++) {
                sPeakGoal goal = D_004781D0_enus[peak][j];
                if (mode == 1)
                    goal = D_004786E0_enus[peak][j];
                if (mode == 2)
                    goal = D_00478D38_enus[peak][j];
                if (mode == 0 || mode == 1)
                    self->mbLocationEventCompleted[peak][mode][j] = cUITemplate_MAP_isMapEventCompleted(self, &goal, peak);
                sprintf(buf, fmt, peak + 1, j);
                menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(buf));
                if (cUITemplate_MAP_mMapStyle < 2 && goal.f0C != 0) {
                    self->mbLocationUnlocked[peak][mode][j] = 0;
                    if (menuObj != 0) {
                        menuObj->setVisible(0);
                        menuObj->setStatic(1);
                    }
                } else {
                    self->mbLocationUnlocked[peak][mode][j] = cUITemplate_MAP_isMapEventLocked_enus(self, goal.kind, peak) ^ 1;
                }
                if (hasEvent(goal.track))
                    cUITemplate_MAP_setWidgetUnHiliteColor_enus(self, menuObj, self->mbLocationUnlocked[peak][mode][j]);
                if (hasEvent(goal.track)) {
                    if (goal.useTrackName == 1) {
                        cUIText_setAsciiString((cUIText*)menuObj, cBENewRaceInterface_getLocationName_enus(iface, goal.track));
                    } else if (func_004165A8(goal.name, D_004A2560)) {
                        cUIText_setAsciiString((cUIText*)menuObj, goal.name);
                    } else if (goal.kind == 4) {
                        unsigned short text[200];
                        unsigned short wfmt[24];
                        func_002C2540_enus(wfmt, D_004A2590);
                        sprintf1(text, wfmt, cBENewRaceInterface_getLocationName_enus(iface, goal.track));
                        cUIText_setUnicodeString_enus(menuObj, text);
                    } else {
                        cUIText_setUnicodeString_enus(menuObj, func_001FD190(goal.kind));
                    }
                    menuObj->setIndex(j);
                } else if (menuObj != 0) {
                    menuObj->setStatic(1);
                }
            }
        }
    }
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_ConfirmMenu_Yes));
    if (menuObj != 0) {
        menuObj->setIndex(0);
        menuObj->setColor(kMAPCOLOR_REGULAR);
    }
    menuObj = (cUIObj_Menus*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_ConfirmMenu_No));
    if (menuObj != 0) {
        menuObj->setIndex(1);
        menuObj->setColor(kMAPCOLOR_REGULAR);
    }
}
#endif

extern unsigned int cUITemplate_MAP_mMapStyle;

//99.9%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupEventUnlock__FP15cUITemplate_MAP);
#ifdef SKIP_ASM
/**
 * Sets which map events are unlocked, probably On Tour's mbEventUnlocked[4] (+0x10 to +0x1C).
 *
 * The first two are unlocked and the fourth locked; the third is unlocked only in map state (cUITemplate_MAP_mMapStyle) 2 or
 * above.
 */
void cUITemplate_MAP_setupEventUnlock(cUITemplate_MAP* self)
{
    self->mbEventUnlocked[3] = 0;
    self->mbEventUnlocked[0] = 1;
    self->mbEventUnlocked[1] = 1;
    if (cUITemplate_MAP_mMapStyle < 2) {
        self->mbEventUnlocked[2] = 0;
    } else {
        self->mbEventUnlocked[2] = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakUnlock);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" int cBENewPlayerInterface_isPeakLocked(void* self, int a, int b, int peak);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern char cBENewRaceInterface_newRace[];

struct sPeakUnlock_MAP {
    int pad0;
    int unlocked[3];    // 0x4, indexed by peak
};

/**
 * Marks peaks 2 and 3 locked when the rider's character (either player's in a two-player game) has not
 * unlocked them.
 *
 * Peak 1 is always unlocked.
 *
 * @param tmpl  the map template
 */
// Marks each peak as unlocked unless it is still locked for the active rider(s).
extern "C" void cUITemplate_MAP_setupPeakUnlock(cUITemplate_MAP* tmpl)
{
    sPeakUnlock_MAP* self = (sPeakUnlock_MAP*)tmpl;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    self->unlocked[0] = 1;
    self->unlocked[1] = 1;
    self->unlocked[2] = 1;
    for (int peak = 1; peak < 3; peak++) {
        int locked;
        if (cBENewRaceInterface_newRace[0x49] == 2) {
            locked = cBENewPlayerInterface_isPeakLocked(player, 0, cBENewPlayerInterface_getPlayerCharID(player, 0), peak);
            if (locked) {
                locked = cBENewPlayerInterface_isPeakLocked(player, 1, cBENewPlayerInterface_getPlayerCharID(player, 1), peak);
            }
        } else {
            locked = cBENewPlayerInterface_isPeakLocked1(player, 0, peak);
        }
        // The store sits in its own do { } while (0) block (macro-style); the extra loop level is what
        // gives the slot pointer priority over the peak counter in register allocation.
        do {
            if (locked) {
                self->unlocked[peak] = 0;
            }
        } while (0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupScreenExit);
#ifdef SKIP_ASM
struct sVEntry00201868 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Prepares the map template for leaving the screen: marks it exiting, forgets the map texture and hides the map
 * picture.
 *
 * Sets +0x2F0 (On Tour mScreenExiting) to 1 and +0x2DC (On Tour mMapTextureSet) to 0, and calls setVisible(0) (slot 9)
 * on the map icon at +0x2E4 (On Tour mpMap) when there is one.
 */
extern "C" void cUITemplate_MAP_setupScreenExit(cUITemplate_MAP* self)
{
    self->mScreenExiting = 1;
    self->mMapTextureSet = 0;
    void* obj = self->mpMap;
    if (obj != 0) {
        sVEntry00201868* vt = *(sVEntry00201868**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onWidgetEvent);

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Map template's menu-event filter: events 6, 8 and 9 get 0, every other event 0x101.
 *
 * cOVState_TRANSPORT_onWidgetMenu and cFEStateEventSelect_onWidgetMenu forward their menu events to it.
 *
 * @param menu  the menu (unused)
 * @param event  the menu event
 * @return 0 for events 6, 8 and 9, else 0x101
 */
extern "C" int cUITemplate_MAP_onWidgetMenu(cUITemplate_MAP* self, int menu, int event)
{
    if (event == 6 || event == 9 || event == 8) {
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Map template's input hook: lets the input through unhandled.
 *
 * @return always 0
 */
int cUITemplate_MAP_onInputBegin(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onInputAlways);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupPeakInfo(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupPeakGoals(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupLayout(cUITemplate_MAP* self);
extern "C" void cUITemplate_MAP_setupLocationInfo(void* self);
extern int cUITemplate_MAP_mMapState;

struct sVEntry00202770 {
    short delta;
    short index;
    int (*fn)(void*);
};

// PORT: the unit declares cUITemplate_MAP_onInputAlways(void*); the body also takes the menu object in $5.
extern "C" void cUITemplate_MAP_onInputAlways_r(void* self, void* menu) __asm__("cUITemplate_MAP_onInputAlways");

/**
 * Refreshes the map's info for the current map state when the input's vtable slot 17 or 18 test passes.
 *
 * Select peak refreshes the peak info, select goal the peak goals (both with the layout), select location
 * cUITemplate_MAP_setupLocationInfo().
 *
 * @param menu  the input (On Tour cUIInput* pInput)
 */
extern "C" void cUITemplate_MAP_onInputAlways_r(void* self, void* menu)
{
    sVEntry00202770* isUpSlot = &(*(sVEntry00202770**)((char*)menu + 8))[17];
    if (isUpSlot->fn((char*)menu + isUpSlot->delta) == 0) {
        sVEntry00202770* isDownSlot = &(*(sVEntry00202770**)((char*)menu + 8))[18];
        if (isDownSlot->fn((char*)menu + isDownSlot->delta) == 0) {
            return;
        }
    }
    if (cUITemplate_MAP_mMapState == 0) {
        cUITemplate_MAP_setupPeakInfo((cUITemplate_MAP*)self);
        cUITemplate_MAP_setupLayout((cUITemplate_MAP*)self);
    }
    if (cUITemplate_MAP_mMapState == 1) {
        cUITemplate_MAP_setupPeakGoals((cUITemplate_MAP*)self);
        cUITemplate_MAP_setupLayout((cUITemplate_MAP*)self);
    }
    if (cUITemplate_MAP_mMapState == 2) {
        cUITemplate_MAP_setupLocationInfo(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cFEAsyncManager;
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
extern char str_peak1outline[];
extern char str_peak2outline[];
extern char str_peak3outline[];
extern char str_Indicators[];
extern char str_Ppct_dyou_are_here[];
extern int cUITemplate_MAP_mMapState;
extern unsigned int cUITemplate_MAP_mMapStyle;

struct sColor_202828 {
    float a, r, g, b;
};

class cUIPic_202828 {
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
    virtual void setVisible(int on);
    virtual void v10();
    virtual void setColour(sColor_202828* c);
};

/**
 * Fades in the map picture once its file has loaded, with the peak outlines or indicators, and the "you are
 * here" markers.
 *
 * The "P<n>you are here" marker shows only on the peak list, for the current location's peak, with info
 * hidden and
 * in the pause or transport styles.
 *
 * @param vself  the map template
 */
extern "C" void cUITemplate_MAP_onUpdate(cUITemplate_MAP* vself)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (vself->mMapTextureSet != 0) {
        if (vself->mMapPicAlpha < 1.0f)
            vself->mMapPicAlpha += 0.1f;
        cUIPic_202828* pic = (cUIPic_202828*)vself->mpMap;
        sColor_202828 color = *(sColor_202828*)((char*)pic + 0x1C);
        color.a = vself->mMapPicAlpha;
        pic->setColour(&color);
        ((cUIPic_202828*)vself->mpMap)->setVisible(1);
        if (cUITemplate_MAP_mMapState == 0) {
            cUIState_showObjSafe(vself->mpUIState, str_peak1outline);
            cUIState_showObjSafe(vself->mpUIState, str_peak2outline);
            cUIState_showObjSafe(vself->mpUIState, str_peak3outline);
        } else {
            cUIState_showObjSafe(vself->mpUIState, str_Indicators);
        }
    } else {
        vself->mMapPicAlpha = 0.0f;
        ((cUIPic_202828*)vself->mpMap)->setVisible(0);
        if (cUITemplate_MAP_mMapState == 0) {
            cUIState_hideObjSafe(vself->mpUIState, str_peak1outline);
            cUIState_hideObjSafe(vself->mpUIState, str_peak2outline);
            cUIState_hideObjSafe(vself->mpUIState, str_peak3outline);
        } else {
            cUIState_hideObjSafe(vself->mpUIState, str_Indicators);
        }
    }
    if (vself->mScreenExiting != 0)
        return;
    for (int i = 0; i < 3; i++) {
        char buf[0x20];
        sprintf(buf, str_Ppct_dyou_are_here, i + 1);
        if (vself->mMapTextureSet != 0 && cUITemplate_MAP_mMapState == 0 && i == *(int*)((char*)cBENewRaceInterface_getLocation(iface) + 0x54) &&
            *(int*)vself == 0 && cUITemplate_MAP_mMapStyle >= 2)
            cUIState_showObjSafe(vself->mpUIState, buf);
        else
            cUIState_hideObjSafe(vself->mpUIState, buf);
    }
    char* mgr = (char*)SSXApp->mFEAsyncManager;
    if (cFEAsyncManager_GetFileStatus((cFEAsyncManager*)mgr, vself->mFEAsyncMapFileIndex) == 3 && vself->mMapTextureSet == 0) {
        char* pic = (char*)vself->mpMap;
        if (pic != 0) {
            int tex = *(int*)(mgr + vself->mFEAsyncMapFileIndex * 0x11C + 0x110);
            *(int*)(pic + 0x7C) = 0;
            *(int*)(pic + 0x78) = tex;
            vself->mMapTextureSet = 1;
            vself->mMapPicAlpha = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupLayout);
#ifdef SKIP_ASM
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUITemplate_MAP_setupLocationInfo(void* self);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern int cUITemplate_MAP_mMapState;
extern int cUITemplate_MAP_mSelectedEvent;
extern int cUITemplate_MAP_mPeak;
extern unsigned int cUITemplate_MAP_mMapStyle;
extern char str_SubTitle[];
extern char str_SP_PeakList[];
extern char str_SPG_Peakpct_dGoals[];
extern char str_Indicators[];
extern char str_kT_240SelectPeak[];
extern char str_SelectPeak[];
extern char str_SelectPeakGoal[];
extern char str_SelectEvent[];
extern char str_pdamaptab_back[];
extern char str_pdabtext1[];
extern char str_kT_FESelPeakGoal[];
extern char str_peak0indicators[];
extern char str_peak1indicators[];
extern char str_peak2indicators[];
extern char str_peakpct_dindicators[];
extern char str_SPG_RaceFSEvents[];
extern char str_SPG_FreerideGoals[];
extern char str_SPG_EarningsGoals[];
extern char str_SPG_InfoBack[];
extern char str_kT_FESelRaceEvent[];
extern char str_kT_FESelFSEvent[];
extern char str_kT_FESelFREvent[];
extern char str_maptab[];
extern char str_SP_Info[];
extern char str_Locks[];
extern char str_pdaps2x[];
extern char str_pdaxba[];
extern char str_pdangca[];
extern char str_SE_Info[];

/**
 * Lays out the map for the current map state: title, which groups are shown, and the per-peak indicators.
 *
 * Select peak, select goal (the peak's goal list and its completion icon) and select location (the event
 * type's
 * title and cUITemplate_MAP_setupLocationInfo()); the bottom tabs are hidden while the list's fourth entry (the mountain) is
 * selected.
 *
 * @param vself  the map template
 */
extern "C" void cUITemplate_MAP_setupLayout(cUITemplate_MAP* vself)
{
    char name[32];
    char next[32];
    char again[32];
    cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_SubTitle));
    switch (cUITemplate_MAP_mMapState) {
    case 0: {
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_240SelectPeak));
        cUIState_showObjSafe(vself->mpUIState, str_SelectPeak);
        cUIState_hideObjSafe(vself->mpUIState, str_SP_Info);
        cUIState_hideObjSafe(vself->mpUIState, str_SelectPeakGoal);
        cUIState_hideObjSafe(vself->mpUIState, str_SelectEvent);
        if (vself->mTransportPopupActive == 0)
            cUIState_showObjSafe(vself->mpUIState, str_Locks);
        int state = 0;
        cUIState_hideObjSafe(vself->mpUIState, str_Indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_SP_Info);
        char* listMenu = (char*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_SP_PeakList));
        if (listMenu != 0)
            state = *(unsigned char*)(listMenu + 0x95);
        if (*(int*)vself != 0 && state != 3)
            cUIState_showObjSafe(vself->mpUIState, str_SP_Info);
        if (state == 3) {
            cUIState_hideObjSafe(vself->mpUIState, str_maptab);
            cUIState_hideObjSafe(vself->mpUIState, str_pdamaptab_back);
            cUIState_hideObjSafe(vself->mpUIState, str_pdabtext1);
            cUIState_hideObjSafe(vself->mpUIState, str_pdaps2x);
            cUIState_hideObjSafe(vself->mpUIState, str_pdaxba);
            cUIState_hideObjSafe(vself->mpUIState, str_pdangca);
        } else if (cUITemplate_MAP_mMapStyle >= 2) {
            cUIState_showObjSafe(vself->mpUIState, str_maptab);
            cUIState_showObjSafe(vself->mpUIState, str_pdamaptab_back);
            cUIState_showObjSafe(vself->mpUIState, str_pdabtext1);
            cUIState_showObjSafe(vself->mpUIState, str_pdaps2x);
            cUIState_showObjSafe(vself->mpUIState, str_pdaxba);
            cUIState_showObjSafe(vself->mpUIState, str_pdangca);
        }
        break;
    }
    case 1: {
        int state = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_FESelPeakGoal));
        cUIState_hideObjSafe(vself->mpUIState, str_SelectPeak);
        cUIState_showObjSafe(vself->mpUIState, str_SelectPeakGoal);
        cUIState_hideObjSafe(vself->mpUIState, str_SelectEvent);
        cUIState_hideObjSafe(vself->mpUIState, str_Locks);
        cUIState_showObjSafe(vself->mpUIState, str_Indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak0indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak1indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak2indicators);
        sprintf(name, str_peakpct_dindicators, cUITemplate_MAP_mPeak);
        cUIState_showObjSafe(vself->mpUIState, name);
        cUIState_hideObjSafe(vself->mpUIState, str_SPG_RaceFSEvents);
        cUIState_hideObjSafe(vself->mpUIState, str_SPG_FreerideGoals);
        cUIState_hideObjSafe(vself->mpUIState, str_SPG_EarningsGoals);
        cUIState_hideObjSafe(vself->mpUIState, str_SPG_InfoBack);
        sprintf(next, str_SPG_Peakpct_dGoals, cUITemplate_MAP_mPeak + 1);
        char* listMenu = (char*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(next));
        if (listMenu != 0)
            state = *(unsigned char*)(listMenu + 0x95);
        if (*(int*)vself != 0 || state == 3) {
            cUIState_showObjSafe(vself->mpUIState, str_SPG_InfoBack);
            switch (state) {
            case 0:
            case 1:
                cUIState_showObjSafe(vself->mpUIState, str_SPG_RaceFSEvents);
                break;
            case 2:
                cUIState_showObjSafe(vself->mpUIState, str_SPG_FreerideGoals);
                break;
            case 3:
                cUIState_showObjSafe(vself->mpUIState, str_SPG_EarningsGoals);
                break;
            }
        }
        if (state == 3) {
            cUIState_hideObjSafe(vself->mpUIState, str_maptab);
            cUIState_hideObjSafe(vself->mpUIState, str_pdamaptab_back);
            cUIState_hideObjSafe(vself->mpUIState, str_pdabtext1);
            cUIState_hideObjSafe(vself->mpUIState, str_pdaps2x);
            cUIState_hideObjSafe(vself->mpUIState, str_pdaxba);
            cUIState_hideObjSafe(vself->mpUIState, str_pdangca);
        } else if (cUITemplate_MAP_mMapStyle >= 2) {
            cUIState_showObjSafe(vself->mpUIState, str_maptab);
            cUIState_showObjSafe(vself->mpUIState, str_pdamaptab_back);
            cUIState_showObjSafe(vself->mpUIState, str_pdabtext1);
            cUIState_showObjSafe(vself->mpUIState, str_pdaps2x);
            cUIState_showObjSafe(vself->mpUIState, str_pdaxba);
            cUIState_showObjSafe(vself->mpUIState, str_pdangca);
        }
        break;
    }
    case 2:
        if (cUITemplate_MAP_mSelectedEvent == 0) {
            cUIState_hideObjSafe(vself->mpUIState, str_Locks);
            cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_FESelRaceEvent));
        } else if (cUITemplate_MAP_mSelectedEvent == 1) {
            cUIState_hideObjSafe(vself->mpUIState, str_Locks);
            cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_FESelFSEvent));
        } else {
            cUIState_hideObjSafe(vself->mpUIState, str_Locks);
            cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_FESelFREvent));
        }
        cUIState_hideObjSafe(vself->mpUIState, str_SelectPeak);
        cUIState_hideObjSafe(vself->mpUIState, str_SelectPeakGoal);
        cUIState_showObjSafe(vself->mpUIState, str_SelectEvent);
        cUIState_showObjSafe(vself->mpUIState, str_Indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak0indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak1indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak2indicators);
        sprintf(name, str_peakpct_dindicators, cUITemplate_MAP_mPeak);
        cUIState_showObjSafe(vself->mpUIState, name);
        cUIState_hideObjSafe(vself->mpUIState, str_SE_Info);
        if (*(int*)vself != 0)
            cUIState_showObjSafe(vself->mpUIState, str_SE_Info);
        cUITemplate_MAP_setupLocationInfo(((char*)vself));
        cUIState_hideObjSafe(vself->mpUIState, str_peak0indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak1indicators);
        cUIState_hideObjSafe(vself->mpUIState, str_peak2indicators);
        sprintf(again, str_peakpct_dindicators, cUITemplate_MAP_mPeak);
        cUIState_showObjSafe(vself->mpUIState, again);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMenuFocus);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_setupPeakGoals(cUITemplate_MAP* self);
extern char str_title_title[];
extern char str_ConfirmMenu_No[];
extern char str_kT_240SelectPeak[];
extern char str_SPG_Peak1Goals[];
extern char str_SPG_Peak2Goals[];
extern char str_SPG_Peak3Goals[];
extern char str_Peak1RaceLocations[];
extern char str_Peak1FreestyleLocations[];
extern char str_Peak1FreerideLocations[];
extern char str_Peak2RaceLocations[];
extern char str_Peak2FreestyleLocations[];
extern char str_Peak2FreerideLocations[];
extern char str_Peak3RaceLocations[];
extern char str_Peak3FreestyleLocations[];
extern char str_Peak3FreerideLocations[];
extern char str_ConfirmMenu[];
extern char str_kT_FEHelpSelectMode[];
extern char str_kT_240SelectEvent[];

class cUIObj_2030A0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void setFocus(int on);
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

static inline cUIObj_2030A0* getObj_2030A0(char* self, char* name)
{
    return (cUIObj_2030A0*)cUIScreen_getObjectByHashName(*(void**)(self + 0x2E8), GetHashValue32(name));
}

static inline cUIObj_2030A0* hideObj_2030A0(char* self, char* name)
{
    cUIObj_2030A0* o = getObj_2030A0(self, name);
    if (o != 0) {
        o->setFocus(0);
        o->setVisible(0);
    }
    return o;
}

/**
 * Switches the map to a map state: hides every menu, then shows and focuses the peak list, goal list or
 * location list.
 *
 * While the confirm popup is up only its menu is shown. In the select-peak-event styles it also sets the
 * title.
 *
 * @param vself  the map template
 * @param focus  the new map state (On Tour eMapState: 0 select peak, 1 select goal, 2 select location)
 */
extern "C" void cUITemplate_MAP_setupMenuFocus(cUITemplate_MAP* vself, int focus)
{
    cUITemplate_MAP_setShowInfo(vself, 0);
    cUITemplate_MAP_mMapState = focus;
    cUIObj_2030A0* peak = hideObj_2030A0(((char*)vself), str_SP_PeakList);
    cUIObj_2030A0* goal0 = hideObj_2030A0(((char*)vself), str_SPG_Peak1Goals);
    cUIObj_2030A0* goal1 = hideObj_2030A0(((char*)vself), str_SPG_Peak2Goals);
    cUIObj_2030A0* goal2 = hideObj_2030A0(((char*)vself), str_SPG_Peak3Goals);
    cUIObj_2030A0* ev00 = hideObj_2030A0(((char*)vself), str_Peak1RaceLocations);
    cUIObj_2030A0* ev01 = hideObj_2030A0(((char*)vself), str_Peak1FreestyleLocations);
    cUIObj_2030A0* ev02 = hideObj_2030A0(((char*)vself), str_Peak1FreerideLocations);
    cUIObj_2030A0* ev10 = hideObj_2030A0(((char*)vself), str_Peak2RaceLocations);
    cUIObj_2030A0* ev11 = hideObj_2030A0(((char*)vself), str_Peak2FreestyleLocations);
    cUIObj_2030A0* ev12 = hideObj_2030A0(((char*)vself), str_Peak2FreerideLocations);
    cUIObj_2030A0* ev20 = hideObj_2030A0(((char*)vself), str_Peak3RaceLocations);
    cUIObj_2030A0* ev21 = hideObj_2030A0(((char*)vself), str_Peak3FreestyleLocations);
    cUIObj_2030A0* ev22 = hideObj_2030A0(((char*)vself), str_Peak3FreerideLocations);
    cUIObj_2030A0* popup = hideObj_2030A0(((char*)vself), str_ConfirmMenu);
    if (vself->mTransportPopupActive != 0) {
        cUIState_hideObjSafe(vself->mpUIState, str_Locks);
        popup->setFocus(1);
        popup->setVisible(1);
        if (vself->mPopupType == 0) {
            cUIObj_2030A0* widget = getObj_2030A0(((char*)vself), str_ConfirmMenu_No);
            widget->setVisible(1);
            widget->setStatic(0);
        } else {
            cUIObj_2030A0* widget = getObj_2030A0(((char*)vself), str_ConfirmMenu_No);
            widget->setVisible(0);
            widget->setStatic(1);
        }
        return;
    }
    switch (cUITemplate_MAP_mMapState) {
    case 0:
        cUITemplate_MAP_setupPeakInfo(vself);
        if (peak != 0) {
            *(int*)((char*)peak + 0x14) |= 1;
            peak->setFocus(1);
            peak->setVisible(1);
        }
        if (cUITemplate_MAP_mMapStyle < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(((char*)vself), str_title_title), GetHashValue32(str_kT_240SelectPeak));
        }
        break;
    case 1:
        cUITemplate_MAP_setupPeakGoals(vself);
        if (cUITemplate_MAP_mPeak == 0 && goal0 != 0) {
            *(int*)((char*)goal0 + 0x14) |= 1;
            goal0->setVisible(1);
            goal0->setFocus(1);
        } else if (cUITemplate_MAP_mPeak == 1 && goal1 != 0) {
            *(int*)((char*)goal1 + 0x14) |= 1;
            goal1->setVisible(1);
            goal1->setFocus(1);
        } else if (goal2 != 0) {
            *(int*)((char*)goal2 + 0x14) |= 1;
            goal2->setVisible(1);
            goal2->setFocus(1);
        }
        if (cUITemplate_MAP_mMapStyle < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(((char*)vself), str_title_title), GetHashValue32(str_kT_FEHelpSelectMode));
        }
        break;
    case 2: {
        cUIObj_2030A0* widget = 0;
        if (cUITemplate_MAP_mPeak == 0) {
            if (cUITemplate_MAP_mSelectedEvent == 0)
                widget = ev00;
            else if (cUITemplate_MAP_mSelectedEvent == 1)
                widget = ev01;
            else if (cUITemplate_MAP_mSelectedEvent == 2)
                widget = ev02;
        } else if (cUITemplate_MAP_mPeak == 1) {
            if (cUITemplate_MAP_mSelectedEvent == 0)
                widget = ev10;
            else if (cUITemplate_MAP_mSelectedEvent == 1)
                widget = ev11;
            else if (cUITemplate_MAP_mSelectedEvent == 2)
                widget = ev12;
        } else if (cUITemplate_MAP_mPeak == 2) {
            if (cUITemplate_MAP_mSelectedEvent == 0)
                widget = ev20;
            else if (cUITemplate_MAP_mSelectedEvent == 1)
                widget = ev21;
            else if (cUITemplate_MAP_mSelectedEvent == 2)
                widget = ev22;
        }
        if (widget != 0) {
            *(int*)((char*)widget + 0x14) |= 1;
            widget->setVisible(1);
            widget->setFocus(1);
        }
        if (cUITemplate_MAP_mMapStyle < 2) {
            cUIText_setUnicodeStringByID((cUIText*)getObj_2030A0(((char*)vself), str_title_title), GetHashValue32(str_kT_240SelectEvent));
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupMapPic);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_Load3PeakPic(void* mgr, int file, int a);
extern "C" void cFEAsyncManager_Load1PeakPic(void* mgr, int peak, int file, int a);
extern int cUITemplate_MAP_mPeak;

/**
 * Requests the map picture for the current state: the three-peak picture on the peak list, else the current
 * peak's.
 *
 * Does nothing when that picture is already loaded; a new request restarts the fade-in.
 */
extern "C" void cUITemplate_MAP_setupMapPic(cUITemplate_MAP* self)
{
    void* mgr = SSXApp->mFEAsyncManager;
    if (cUITemplate_MAP_mMapState == 0) {
        if (self->mMapPicType != 1) {
            cFEAsyncManager_UnloadFEAsyncFile(mgr, self->mFEAsyncMapFileIndex);
            cFEAsyncManager_Load3PeakPic(mgr, self->mFEAsyncMapFileIndex, 1);
            self->mMapPicType = 1;
            self->mMapTextureSet = 0;
        }
    } else if (cUITemplate_MAP_mMapState >= 0) {
        if (cUITemplate_MAP_mMapState < 3) {
            if (self->mMapPicType != cUITemplate_MAP_mPeak + 2) {
                cFEAsyncManager_UnloadFEAsyncFile(mgr, self->mFEAsyncMapFileIndex);
                cFEAsyncManager_Load1PeakPic(mgr, cUITemplate_MAP_mPeak, self->mFEAsyncMapFileIndex, 1);
                int picIndex = cUITemplate_MAP_mPeak + 2;
                self->mMapTextureSet = 0;
                self->mMapPicType = picIndex;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPopup);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_kT_BTNOK_b[];
extern char str_ConfirmMenu_Yes[];
extern char str_ConfirmMenu_No[];
extern char str_ConfirmMenu_No_x[];
extern char str_ConfirmMenu[];
extern char str_ConfirmPopup[];
extern char str_P2_TransportNow[];
extern char str_kT_OVRCMNGoPeakNow[];
extern char str_kT_MAPTransArea[];
extern char str_kT_CMNYes_b[];
extern char str_maptab[];
extern char str_Popup1[];
extern char str_Popup2[];
extern int cUITemplate_MAP_mMapState;

class cUIObj_203A58 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

/**
 * Shows or hides the transport confirm popup and sets its text and buttons.
 *
 * The popup's text is "go to peak now" or "transport to area"; a one-button popup (+0x2D0 is 1) shows OK and
 * hides No, otherwise Yes and No.
 */
extern "C" void cUITemplate_MAP_setupPopup(cUITemplate_MAP* self)
{
    if (self->mTransportPopupActive != 0) {
        cUIState_showObjSafe(self->mpUIState, str_ConfirmPopup);
        void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ConfirmMenu));
        if (menu != 0)
            cUIMenu_setSelectedByIndex(menu, 0);
        if (self->mTransportPopupActive != 0) {
            cUIState_hideObjSafe(self->mpUIState, str_Popup1);
            cUIState_showObjSafe(self->mpUIState, str_Popup2);
            cUIState_hideObjSafe(self->mpUIState, str_maptab);
            cUIText* popupText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_P2_TransportNow));
            if (cUITemplate_MAP_mMapState == 0) {
                if (popupText != 0)
                    cUIText_setUnicodeStringByID(popupText, GetHashValue32(str_kT_OVRCMNGoPeakNow));
            } else {
                if (popupText != 0)
                    cUIText_setUnicodeStringByID(popupText, GetHashValue32(str_kT_MAPTransArea));
            }
        }
        cUIState_hideObjSafe(self->mpUIState, str_ConfirmMenu_No_x);
        cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ConfirmMenu_Yes));
        char* confirmNo = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ConfirmMenu_No));
        if (self->mPopupType == 1) {
            if (title != 0)
                cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_BTNOK_b));
            if (confirmNo != 0) {
                ((cUIObj_203A58*)confirmNo)->setVisible(0);
                ((cUIObj_203A58*)confirmNo)->setStatic(1);
            }
            cUIState_hideObjSafe(self->mpUIState, str_ConfirmMenu_No);
        } else {
            if (title != 0)
                cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_CMNYes_b));
            if (confirmNo != 0) {
                ((cUIObj_203A58*)confirmNo)->setVisible(1);
                ((cUIObj_203A58*)confirmNo)->setStatic(0);
            }
            cUIState_showObjSafe(self->mpUIState, str_ConfirmMenu_No);
        }
    } else {
        cUIState_hideObjSafe(self->mpUIState, str_ConfirmPopup);
        cUIState_showObjSafe(self->mpUIState, str_maptab);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setShowInfo);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_maptab_text[];
extern char str_kT_BTNShowMap[];
extern char str_kT_BTNShowInfo[];

/**
 * Sets whether the map shows info instead of the map, and labels the tab button the other way (show map or
 * show info).
 *
 * @param show  nonzero to show info
 */
extern "C" void cUITemplate_MAP_setShowInfo(cUITemplate_MAP* self, int show)
{
    *(int*)self = show;
    cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_maptab_text));
    if (*(int*)self != 0) {
        cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_BTNShowMap));
    } else {
        cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_BTNShowInfo));
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakInfo);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.
struct cUIText;

int GetHashValue32(char* str);

extern "C" void cUIState_showObjSafe(void* self, char* name);

extern "C" void cUIState_hideObjSafe(void* self, char* name);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int cUITemplate_MAP_mMapStyle;

extern int cUITemplate_MAP_mMapState;


extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).
extern sPeakGoal D_004781D0_Info[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_Info[3][5] __asm__("D_004786E0");

// The text database (SSXApp + 0x8C): localised strings by hash through vtable slot 4 (as in cUITemplate_MAP_getMedalStr).
struct cTextDB_5098 {
    int pad;
    sVEntry<unsigned short*(void*, int)>* vt;
};

#define TextDB_5098 (*(cTextDB_5098**)((char*)SSXApp + 0x8C))

// A macro, not an inline helper: an inline's key parameter is a pseudo that loop.c hoists out of case 0's loop.
#define Loc_5098(key) (TextDB_5098->vt[4].fn((char*)TextDB_5098 + TextDB_5098->vt[4].delta, GetHashValue32(key)))

extern char str_PEAKno[];

extern char str_pdaPEAKno_back[];

extern char str_fePEAKno_back[];

extern char str_SP_GameInfo[];

extern char str_HelpText[];

int cBELibrary_getCharacterID(int);

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern char D_0046F9E0_Info[] __asm__("str_SubTitle");
extern char D_00470600_Info[] __asm__("str_SP_PeakList");
extern char D_004706D8_Info[] __asm__("str_SPG_Peakpct_dGoals");
extern char D_004707A0_Info[] __asm__("str_Indicators");
extern char D_004707C0_Info[] __asm__("str_kT_240SelectPeak");
extern char D_004707D8_Info[] __asm__("str_SelectPeak");
extern char D_004707E8_Info[] __asm__("str_SelectPeakGoal");
extern char D_004707F8_Info[] __asm__("str_SelectEvent");

extern char D_004A25B0_Info[] __asm__("str_maptab");
extern char D_004A2608_Info[] __asm__("str_SP_Info");
extern char D_004A2610_Info[] __asm__("str_Locks");
extern char D_004A2618_Info[] __asm__("str_pdaps2x");
extern char D_004A2620_Info[] __asm__("str_pdaxba");
extern char D_004A2628_Info[] __asm__("str_pdangca");
extern char D_004A2630_Info[] __asm__("str_SE_Info");

extern char D_004A2610_Info[] __asm__("str_Locks");

extern "C" int cBERewardInterface_isTrophyOwned(void* self, int a, int b, int c, int i);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int kind, int track);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString_Info(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int peak);
extern "C" int cBERewardInterface_getChallengesMedalTarget(void* self, int peak, int tier);
extern "C" int cBERewardInterface_getCollectibleMedalTarget(void* self, int peak, int tier);
int cBERewardInterface_getEarningsTarget(void* self, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
int cBEScoreInterface_getCurrentHighlightLevel(void* self, int i);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern char D_00470AE8[], D_004A2648[], D_00470AF8[], D_00470B08[], D_004A2650[], D_00470B20[], str_Mountain[];
extern char D_004A2658[], D_00470B38[], D_00470B50[], D_00470B68[], D_00470B80[], D_00470B98[], str_kT_10HELPUnlockPeak3[];
extern char D_00470BC8[], D_00470BE0[], D_00470BF8[], D_00470C10[], D_00470C20[], D_00470C30[], str_SP_Freestylecheck[];
extern char D_00470C60[], D_00470C70[], D_00470C88[], D_00470C98[], D_00470CB0[], D_00470CC8[], str_SP_GameStat1[];
extern char D_00470CE8[], D_00470D00[], D_00470D10[], D_00470D28[], D_00470D40[], D_00470D58[], str_SP_GameStat3check[];
extern char D_00470D80[], D_00470D90[], D_00470DA8[], D_00470DC0[], D_00470DD8[], D_00470DE8[], str_SP_GameComplete[];
extern char D_004A2398_Info[] __asm__("str_pct_d_pct_d_d");
extern char str_pct_d_5[];
extern char str_pct_d_2[];
extern char str_pct_d_1[];
extern char str_pct_d_14[];
extern char str_pct_d_12[];
extern char str_pct_d_24[];
extern char str_pct_dpct_pct[];

// The peak list entry selected in the map menu (at widget + 0xA0).
struct sPeakSel_MAP {
    char pad[0x18];
    int peak;           // 0x18
};

struct sPeakInfo_MAP {
    int pad0;
    int unlocked[3];            // 0x4, indexed by peak
    char pad10[0x2CC - 0x10];
    int popup;                  // 0x2CC
    char pad2D0[0x2E8 - 0x2D0];
    void* screen;               // 0x2E8
    void* owner;                // 0x2EC
};



// Goal kind test; the kind constant is taken by const reference.
static inline int isKind(int kind, const int& k) { return kind == k; }
/**
 * Fills the peak list's info panel: completed-peak checks, lock boxes and the help text for the highlighted
 * peak.
 *
 * A peak is checked when its four trophies are owned. In the pause and transport styles the panel also lists
 * the
 * peak's race medals, freestyle results, missions, collectibles, earnings and highlight level against their
 * medal
 * targets.
 *
 * @param tmpl  the map template
 */
extern "C" void cUITemplate_MAP_setupPeakInfo(cUITemplate_MAP* tmpl)
{
    sPeakInfo_MAP* self = (sPeakInfo_MAP*)tmpl;
    void* rew = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* mis = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    int ch = cBELibrary_getCharacterID(0);
    if (self->popup != 0) {
        cUIState_hideObjSafe(self->owner, D_00470AE8);
        cUIState_hideObjSafe(self->owner, D_004A2648);
        cUIState_hideObjSafe(self->owner, D_004A2610_Info);
    } else {
        cUIState_showObjSafe(self->owner, D_00470AE8);
        cUIState_showObjSafe(self->owner, D_004A2648);
        cUIState_showObjSafe(self->owner, D_004A2610_Info);
    }
    cUIState_hideObjSafe(self->owner, D_00470AF8);
    cUIState_hideObjSafe(self->owner, D_00470B08);
    cUIState_hideObjSafe(self->owner, D_004A2650);
    char name1[32];
    char name2[32];
    for (int i = 0; i < 3; i++) {
        sprintf(name1, D_00470B20, i + 1);
        if (cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 0) && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 1)
            && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 2) && cBERewardInterface_isTrophyOwned(rew, 0, ch, i, 3)) {
            cUIState_showObjSafe(self->owner, name1);
        } else {
            cUIState_hideObjSafe(self->owner, name1);
        }
    }
    if (cUITemplate_MAP_mMapStyle < 2) {
        cUIThing* mountain = (cUIThing*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Mountain));
        mountain->setStatic(1);
        mountain->setVisible(0);
    }
    cUIState_hideObjSafe(self->owner, str_PEAKno);
    cUIState_hideObjSafe(self->owner, str_pdaPEAKno_back);
    cUIState_hideObjSafe(self->owner, str_fePEAKno_back);
    for (int i = 0; i < 3; i++) {
        sprintf(name1, D_004A2658, i + 1);
        sprintf(name2, D_00470B38, i + 1);
        if (cUITemplate_MAP_mMapState == 0 && self->unlocked[i] == 0) {
            cUIState_hideObjSafe(self->owner, name2);
        } else {
            cUIState_hideObjSafe(self->owner, name1);
            if (cUITemplate_MAP_mMapStyle < 2)
                cUIState_hideObjSafe(self->owner, name2);
        }
    }
    cUIThing* title = (cUIThing*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_HelpText));
    title->setVisible(0);
    char* list = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470600_Info));
    if (list == 0)
        return;
    sPeakSel_MAP* sel = *(sPeakSel_MAP**)(list + 0xA0);
    if (sel == 0)
        return;
    if (sel->peak < 3) {
        cUIState_hideObjSafe(self->owner, str_SP_GameInfo);
        int peak = sel->peak;
        title->setVisible(1);
        if (self->unlocked[peak] != 0) {
            if (cUITemplate_MAP_mMapStyle < 2)
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B50));
            else
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B68));
        } else if (peak == 1) {
            if (cUITemplate_MAP_mMapStyle < 2)
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B80));
            else
                cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470B98));
        } else if (cUITemplate_MAP_mMapStyle < 2) {
            cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(str_kT_10HELPUnlockPeak3));
        } else {
            cUIText_setUnicodeStringByID((cUIText*)title, GetHashValue32(D_00470BC8));
        }
        if (cUITemplate_MAP_mMapStyle < 2)
            return;
        unsigned short wbuf[200];
        int cnt = 4;
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470BE0));
        int found = 0;
        unsigned short* fmt = Loc_5098(D_00470BF8);
        sprintf1(wbuf, fmt, peak + 1);
        cUIText_setUnicodeString_Info(textObj, wbuf);
        if (peak == 2)
            cnt = 3;
        for (int k = 0; k < cnt; k++) {
            sPeakGoal goal = D_004781D0_Info[peak][k];
            if (cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track) != -1)
                found++;
        }
        char buf[32];
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C10));
        sprintf(buf, D_004A2398_Info, found, cnt);
        cUIText_setAsciiString(textObj, buf);
        if (found >= cnt)
            cUIState_showObjSafe(self->owner, D_00470C20);
        else
            cUIState_hideObjSafe(self->owner, D_00470C20);
        int found2 = 0;
        for (int k = 0; k < 5; k++) {
            sPeakGoal goal = D_004786E0_Info[peak][k];
            if (cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track) != -1)
                found2++;
        }
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C30));
        sprintf(buf, str_pct_d_5, found2);
        cUIText_setAsciiString(textObj, buf);
        if (found2 >= 5)
            cUIState_showObjSafe(self->owner, str_SP_Freestylecheck);
        else
            cUIState_hideObjSafe(self->owner, str_SP_Freestylecheck);
        int medals = 0;
        if (cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, peak) >= cBERewardInterface_getChallengesMedalTarget(rew, peak, 3))
            medals++;
        if (cBEMissionInterface_getCurrentCollectForPeak(mis, 0, peak) >= cBERewardInterface_getCollectibleMedalTarget(rew, peak, 3))
            medals++;
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C60));
        sprintf(buf, str_pct_d_2, medals);
        cUIText_setAsciiString(textObj, buf);
        if (medals >= 2)
            cUIState_showObjSafe(self->owner, D_00470C70);
        else
            cUIState_hideObjSafe(self->owner, D_00470C70);
        unsigned int money = cBEEconInterface_getTotalMoneyEarned1(econ, 0);
        int ok = cBERewardInterface_getEarningsTarget(rew, peak) < money;
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470C88));
        sprintf(buf, str_pct_d_1, ok);
        cUIText_setAsciiString(textObj, buf);
        if (ok > 0)
            cUIState_showObjSafe(self->owner, D_00470C98);
        else
            cUIState_hideObjSafe(self->owner, D_00470C98);
    } else {
        int done = 0;
        cUIState_showObjSafe(self->owner, str_SP_GameInfo);
        cUIState_hideObjSafe(self->owner, D_004A2608_Info);
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470CB0));
        cUIText_setUnicodeStringByID(textObj, GetHashValue32(D_00470CC8));
        for (int p = 0; p < 3; p++) {
            int cnt = 4;
            if (p == 2)
                cnt = 3;
            for (int k = 0; k < cnt; k++) {
                sPeakGoal goal = D_004781D0_Info[p][k];
                if (!isKind(goal.kind, 4) && !isKind(goal.kind, 6) && !isKind(goal.kind, 7) && !isKind(goal.kind, 8)) {
                    if ((unsigned)cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track) < 2)
                        done++;
                }
            }
            for (int k = 0; k < 5; k++) {
                sPeakGoal goal = D_004786E0_Info[p][k];
                if (!isKind(goal.kind, 5) && !isKind(goal.kind, 9) && !isKind(goal.kind, 10) && !isKind(goal.kind, 11)) {
                    if ((unsigned)cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track) < 2)
                        done++;
                }
            }
        }
        char buf[32];
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_SP_GameStat1));
        sprintf(buf, str_pct_d_14, done);
        cUIText_setAsciiString(textObj, buf);
        if (done >= 14)
            cUIState_showObjSafe(self->owner, D_00470CE8);
        else
            cUIState_hideObjSafe(self->owner, D_00470CE8);
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D00));
        int gold = 0;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 6, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 9, 0xE) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 7, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 10, 0xF) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 4, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 8, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 5, 0x10) != -1)
            gold++;
        if (cBERewardInterface_getTrackMedal(rew, 0, ch, 11, 0x10) != -1)
            gold++;
        sprintf(buf, str_pct_d_12, gold);
        cUIText_setAsciiString(textObj, buf);
        if (gold >= 12)
            cUIState_showObjSafe(self->owner, D_00470D10);
        else
            cUIState_hideObjSafe(self->owner, D_00470D10);
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D28));
        cUIText_setUnicodeStringByID(textObj, GetHashValue32(D_00470D40));
        int cur = 0;
        int tot = 0;
        for (int p = 0; p < 3; p++) {
            cur += cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, p);
            tot += cBEMissionInterface_getNumberMissionsOnPeak(mis, p);
        }
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D58));
        sprintf(buf, D_004A2398_Info, cur, tot);
        cUIText_setAsciiString(textObj, buf);
        if (cur >= tot)
            cUIState_showObjSafe(self->owner, str_SP_GameStat3check);
        else
            cUIState_hideObjSafe(self->owner, str_SP_GameStat3check);
        int col = 0;
        int colTot = 0;
        for (int p = 0; p < 3; p++) {
            col += cBEMissionInterface_getCurrentCollectForPeak(mis, 0, p);
            colTot += cBEMissionInterface_getTotalCollectForPeak(mis, p);
        }
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470D80));
        sprintf(buf, D_004A2398_Info, col, colTot);
        cUIText_setAsciiString(textObj, buf);
        if (col >= colTot)
            cUIState_showObjSafe(self->owner, D_00470D90);
        else
            cUIState_hideObjSafe(self->owner, D_00470D90);
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470DA8));
        cUIText_setUnicodeStringByID(textObj, GetHashValue32(D_00470DC0));
        int hl = 0;
        for (int p = 0; p < 8; p++)
            hl += cBEScoreInterface_getCurrentHighlightLevel(score, p);
        sprintf(buf, str_pct_d_24, hl);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00470DD8)), buf);
        if (hl >= 24)
            cUIState_showObjSafe(self->owner, D_00470DE8);
        else
            cUIState_hideObjSafe(self->owner, D_00470DE8);
        float pct = ((float)done * (1.0f / 14.0f) + (float)gold * (1.0f / 12.0f) + (float)cur / (float)tot + (float)col / (float)colTot + (float)hl * (1.0f / 24.0f)) * 20.0f;
        int pctInt = 100;
        if (pct != 100.0f) {
            pctInt = (int)pct;
            if (pct >= 99.0f)
                pctInt = 99;
        }
        textObj = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_SP_GameComplete));
        sprintf(buf, str_pct_dpct_pct, pctInt);
        cUIText_setAsciiString(textObj, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_getMedalStr);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_CMNPlatinum[];
extern char str_kT_CMNGold[];
extern char str_kT_CMNSilver[];
extern char str_kT_CMNBronze[];

struct sVEntry00204FF0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int setPic00204FF0(char* name)
{
    void* obj = SSXApp->FELocale;
    sVEntry00204FF0* vt = *(sVEntry00204FF0**)((char*)obj + 4);
    return vt[4].fn((char*)obj + vt[4].delta, GetHashValue32(name));
}

/**
 * Returns the localised name of a medal.
 *
 * Looks up "kT_CMNPlatinum", "kT_CMNGold", "kT_CMNSilver" or "kT_CMNBronze" by hash through the front-end
 * locale's virtual slot 4.
 *
 * @param peak  the medal: 0 platinum, 1 gold, 2 silver, 3 bronze (On Tour names it medal); others give platinum
 * @return the localised string (On Tour: unsigned short*)
 */
extern "C" int cUITemplate_MAP_getMedalStr(cUITemplate_MAP* self, int peak)
{
    switch (peak) {
    case 0:
    default:
        return setPic00204FF0(str_kT_CMNPlatinum);
    case 1:
        return setPic00204FF0(str_kT_CMNGold);
    case 2:
        return setPic00204FF0(str_kT_CMNSilver);
    case 3:
        return setPic00204FF0(str_kT_CMNBronze);
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupPeakGoals);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.

// A UI widget as cUIScreen_getObjectByHashName returns it: vptr at 0x8. Slot names from the prototype's
// _vt$8cUIThing (as in fe/feasyncfile); the other slots are known only by position.

struct cUIText;

int GetHashValue32(char* str);

extern "C" void cUIState_showObjSafe(void* self, char* name);

extern "C" void cUIState_hideObjSafe(void* self, char* name);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern unsigned int cUITemplate_MAP_mMapStyle;

extern int cUITemplate_MAP_mMapState;


extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern int cUITemplate_MAP_mPeak;

// One 0x6C-byte goal entry of the per-peak goal tables (race: 4 per peak, freestyle: 5 per peak,
// freeride: 8 per peak).

extern sPeakGoal D_004781D0_oals[3][4] __asm__("D_004781D0");

extern sPeakGoal D_004786E0_oals[3][5] __asm__("D_004786E0");

// The text database (SSXApp + 0x8C): localised strings by hash through vtable slot 4 (as in cUITemplate_MAP_getMedalStr_oals).

#define TextDB_5098 (*(cTextDB_5098**)((char*)SSXApp + 0x8C))

// A macro, not an inline helper: an inline's key parameter is a pseudo that loop.c hoists out of case 0's loop.
#define Loc_5098(key) (TextDB_5098->vt[4].fn((char*)TextDB_5098 + TextDB_5098->vt[4].delta, GetHashValue32(key)))

extern char str_PEAKno[];

extern char str_pdaPEAKno_back[];

extern char str_fePEAKno_back[];

extern char str_HelpText[];

int cBELibrary_getCharacterID(int);

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern char D_0046F9E0_oals[] __asm__("str_SubTitle");
extern char D_00470600_oals[] __asm__("str_SP_PeakList");
extern char D_004706D8_oals[] __asm__("str_SPG_Peakpct_dGoals");
extern char D_004707A0_oals[] __asm__("str_Indicators");
extern char D_004707C0_oals[] __asm__("str_kT_240SelectPeak");
extern char D_004707D8_oals[] __asm__("str_SelectPeak");
extern char D_004707E8_oals[] __asm__("str_SelectPeakGoal");
extern char D_004707F8_oals[] __asm__("str_SelectEvent");

// (prototype from their definition at line 1392)
extern "C" int cUITemplate_MAP_getMedalStr_oals(void* self, int peak) __asm__("cUITemplate_MAP_getMedalStr");

extern "C" void cUITemplate_MAP_showEventPath_oals(void* tmpl, char* name) __asm__("cUITemplate_MAP_showEventPath");
extern "C" int func_004165A8(const char*, const char*);
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int a, int b, int c, int i);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int kind, int track);
extern "C" const char* cBENewRaceInterface_getLocationName_oals(void* iface, int id) __asm__("cBENewRaceInterface_getLocationName");
extern "C" unsigned short* func_002C2540_oals(unsigned short* dst, const char* s) __asm__("func_002C2540");
extern "C" unsigned short* strcpy1_oals(unsigned short* dst, const unsigned short* src) __asm__("strcpy1");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void sprintf2(unsigned short* dst, const char* fmt, ...);
extern "C" unsigned short* func_001FD190(int kind);
extern "C" void cUIText_setUnicodeString_oals(void* text, unsigned short* s) __asm__("cUIText_setUnicodeString");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* self, int peak);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int peak);
extern "C" int cBERewardInterface_getChallengesMedalTarget(void* self, int peak, int tier);
extern "C" int cBERewardInterface_getCollectibleMedalTarget(void* self, int peak, int tier);
int cBERewardInterface_getEarningsTarget(void* self, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
extern "C" const char* getDollarString(int value);
extern char D_004A2550[];
extern char D_004A2560_oals[] __asm__("D_004A2560");
extern char D_004A23C0[];
extern char str_pct_S_pct_s_b[];
extern char D_004A26A0_oals[] __asm__("str_pct_S_Jam_b");
extern char D_0046E548_oals[] __asm__("str_kT_cmnRaceCaps_b");
extern char D_00470E50[], D_00470E68[], D_00470E80[], D_00470E98[], D_00470EB0[], D_00470EC0[], str_kT_Peak3name_b[];
extern char D_00470EE0[], D_00470EF0[], D_00470F00[], D_00470F18[], D_00470F28[], D_00470F38[], str_peakgoal_check0[];
extern char D_00470F58[], D_00470F70[], D_00470F90[], D_00470FA8[], D_00470FC0[], D_00470FD0[], str_SPG_Event4[];
extern char D_00470FF0[], D_00471000[], D_00471010[], D_00471020[], D_00471030[], D_00471040[], str_SPG_Eventpct_dbox[];
extern char D_00471068[], D_00471078[], D_00471088[], D_004710A0[], D_004710B8[], D_004710D0[], str_SPG_Eventpct_dCheckbox[];
extern char D_00471108[], D_00471128[], D_00471140[], D_00471158[], D_00471170[], D_00471188[], str_kT_CMNPlatinumAt[];
extern char D_004711B8[], D_004711C8[], D_004711D8[], D_004711E8[], D_00471200[], D_00471218[], str_SPG_CollectablesCurrent[];
extern char D_00471248[], D_00471260[], D_00471278[], D_00471298[], D_004712B0[], D_004712C8[], str_kT_CMNEarnNum[];
extern char D_004712F0[], D_00471300[], D_00471318[], str_SPG_Earningscheck[];

extern "C" char* strcpy(char* d, const char* s);

/**
 * Fills the goal panel of the current peak: the race, freestyle and freeride goal groups with checks and
 * progress.
 *
 * Shows each event's medal state, the completed counts, and the challenge and collectible counts against the
 * platinum to bronze targets, and highlights the event path (showEventPath()).
 *
 * @param tmpl  the map template
 */
extern "C" void cUITemplate_MAP_setupPeakGoals(cUITemplate_MAP* tmpl)
{
    unsigned short wbuf[200];
    char buf[32];
    cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(str_HelpText));
    if (cUITemplate_MAP_mMapStyle < 2) {
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470E50);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470E68);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470E80);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470E98);
    }
    cUIState_showObjSafe(tmpl->mpUIState, str_PEAKno);
    if (cUITemplate_MAP_mMapStyle == 0) {
        cUIState_showObjSafe(tmpl->mpUIState, str_fePEAKno_back);
    } else {
        cUIState_showObjSafe(tmpl->mpUIState, str_pdaPEAKno_back);
    }
    cUIText* name = (cUIText*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(str_PEAKno));
    if (cUITemplate_MAP_mPeak == 0) {
        cUIText_setUnicodeStringByID(name, GetHashValue32(D_00470EB0));
    } else if (cUITemplate_MAP_mPeak == 1) {
        cUIText_setUnicodeStringByID(name, GetHashValue32(D_00470EC0));
    } else {
        cUIText_setUnicodeStringByID(name, GetHashValue32(str_kT_Peak3name_b));
    }
    cUIThing* widget;
    unsigned char goalGroup = 0;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* rew = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* mis = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    int ch = cBELibrary_getCharacterID(0);
    sprintf(buf, D_004706D8_oals, cUITemplate_MAP_mPeak + 1);
    char* obj = (char*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(buf));
    if (obj) goalGroup = obj[0x95];
    if (cUITemplate_MAP_mMapState == 1) {
        char path[24];
        if (goalGroup == 0) {
            sprintf(path, D_00470EE0, cUITemplate_MAP_mPeak + 1);
            cUITemplate_MAP_showEventPath_oals(((char*)tmpl), path);
        } else if (goalGroup == 1) {
            sprintf(path, D_00470EF0, cUITemplate_MAP_mPeak + 1);
            cUITemplate_MAP_showEventPath_oals(((char*)tmpl), path);
        } else {
            sprintf(path, D_00470F00, cUITemplate_MAP_mPeak + 1);
            cUITemplate_MAP_showEventPath_oals(((char*)tmpl), path);
        }
    }
    cUIState_hideObjSafe(tmpl->mpUIState, D_00470F18);
    cUIState_hideObjSafe(tmpl->mpUIState, D_00470F28);
    cUIState_hideObjSafe(tmpl->mpUIState, D_00470F38);
    cUIState_hideObjSafe(tmpl->mpUIState, str_peakgoal_check0);
    if (cUITemplate_MAP_mMapStyle < 2) {
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470F58));
        return;
    }
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, cUITemplate_MAP_mPeak, 0))
        cUIState_showObjSafe(tmpl->mpUIState, D_00470F18);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, cUITemplate_MAP_mPeak, 1))
        cUIState_showObjSafe(tmpl->mpUIState, D_00470F28);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, cUITemplate_MAP_mPeak, 2))
        cUIState_showObjSafe(tmpl->mpUIState, D_00470F38);
    if (cBERewardInterface_isTrophyOwned(rew, 0, ch, cUITemplate_MAP_mPeak, 3))
        cUIState_showObjSafe(tmpl->mpUIState, str_peakgoal_check0);
    switch (goalGroup) {
    case 0: {
        int cnt = 4;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00470F70));
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00470F90));
        sprintf1(wbuf, Loc_5098(D_00470FA8), cUITemplate_MAP_mPeak + 1);
        cUIText_setUnicodeString_oals(widget, wbuf);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470FC0);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470FD0);
        cUIState_hideObjSafe(tmpl->mpUIState, str_SPG_Event4);
        cUIState_hideObjSafe(tmpl->mpUIState, D_00470FF0);
        if (cUITemplate_MAP_mPeak == 2) {
            cnt = 3;
            cUIState_hideObjSafe(tmpl->mpUIState, D_00471000);
            cUIState_hideObjSafe(tmpl->mpUIState, D_00471010);
            cUIState_hideObjSafe(tmpl->mpUIState, D_00471020);
            cUIState_hideObjSafe(tmpl->mpUIState, D_00471030);
        }
        int done = 0;
        for (int k = 0; k < cnt; k++) {
            char name1[24];
            unsigned short ev[50];
            unsigned short medal[50];
            sPeakGoal goal = D_004781D0_oals[cUITemplate_MAP_mPeak][k];
            if (goal.useTrackName) {
                sprintf2(ev, cBENewRaceInterface_getLocationName_oals(race, goal.track));
            } else if (func_004165A8(goal.name, D_004A2560_oals)) {
                func_002C2540_oals(ev, goal.name);
            } else if (goal.kind == 4) {
                unsigned short fmt[20];
                func_002C2540_oals(fmt, str_pct_S_pct_s_b);
                sprintf1(ev, fmt, cBENewRaceInterface_getLocationName_oals(race, goal.track), Loc_5098(D_0046E548_oals));
            } else {
                strcpy1_oals(ev, func_001FD190(goal.kind));
            }
            sprintf(name1, D_00471040, k);
            char name2[24];
            sprintf(name2, str_SPG_Eventpct_dbox, k);
            cUIState_showObjSafe(tmpl->mpUIState, name2);
            char name3[24];
            sprintf(name3, D_00471068, k);
            cUIState_showObjSafe(tmpl->mpUIState, name3);
            func_002C2540_oals(medal, D_004A2560_oals);
            int medalLevel = cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track);
            if (medalLevel != -1) {
                unsigned short fmt[8];
                done++;
                func_002C2540_oals(fmt, D_004A23C0);
                sprintf1(medal, fmt, cUITemplate_MAP_getMedalStr_oals(((char*)tmpl), medalLevel));
                cUIState_showObjSafe(tmpl->mpUIState, name1);
            } else {
                cUIState_hideObjSafe(tmpl->mpUIState, name1);
            }
            char name4[24];
            sprintf(name4, D_00471078, k);
            widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(name4));
            unsigned short fmt[8];
            func_002C2540_oals(fmt, D_004A2550);
            sprintf1(wbuf, fmt, ev, medal);
            cUIText_setUnicodeString_oals(widget, wbuf);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471088));
        sprintf1(wbuf, Loc_5098(D_004710A0), done, cnt);
        cUIText_setUnicodeString_oals(widget, wbuf);
        break;
    }
    case 1: {
        int done = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_004710B8));
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00470F90));
        sprintf1(wbuf, Loc_5098(D_004710D0), cUITemplate_MAP_mPeak + 1);
        cUIText_setUnicodeString_oals(widget, wbuf);
        for (int k = 0; k < 5; k++) {
            unsigned short ev[50];
            unsigned short medal[50];
            sPeakGoal goal = D_004786E0_oals[cUITemplate_MAP_mPeak][k];
            if (goal.useTrackName) {
                sprintf2(ev, cBENewRaceInterface_getLocationName_oals(race, goal.track));
            } else if (func_004165A8(goal.name, D_004A2560_oals)) {
                func_002C2540_oals(ev, goal.name);
            } else if (goal.kind == 5) {
                unsigned short fmt[20];
                func_002C2540_oals(fmt, D_004A26A0_oals);
                sprintf1(ev, fmt, cBENewRaceInterface_getLocationName_oals(race, goal.track));
            } else {
                strcpy1_oals(ev, func_001FD190(goal.kind));
            }
            char name1[24];
            sprintf(name1, D_00471040, k);
            char name2[24];
            sprintf(name2, str_SPG_Eventpct_dbox, k);
            cUIState_showObjSafe(tmpl->mpUIState, name2);
            char name3[24];
            sprintf(name3, D_00471068, k);
            cUIState_showObjSafe(tmpl->mpUIState, name3);
            func_002C2540_oals(medal, D_004A2560_oals);
            int medalLevel = cBERewardInterface_getTrackMedal(rew, 0, ch, goal.kind, goal.track);
            char medalFmt[16];
            if (medalLevel != -1) {
                strcpy(medalFmt, "[%s]");
                done++;
                sprintf2(medal, medalFmt, cUITemplate_MAP_getMedalStr_oals(((char*)tmpl), medalLevel));
                cUIState_showObjSafe(tmpl->mpUIState, name1);
            } else {
                cUIState_hideObjSafe(tmpl->mpUIState, name1);
            }
            char name4[24];
            sprintf(name4, D_00471078, k);
            widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(name4));
            char fmt[16];
            strcpy(fmt, "%s %s");
            sprintf2(wbuf, fmt, ev, medal);
            cUIText_setUnicodeString_oals(widget, wbuf);
            widget->setVisible(1);
            char name5[24];
            sprintf(name5, str_SPG_Eventpct_dCheckbox, k);
            cUIState_showObjSafe(tmpl->mpUIState, name5);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471088));
        sprintf1(wbuf, Loc_5098(D_004710A0), done, 5);
        cUIText_setUnicodeString_oals(widget, wbuf);
        break;
    }
    case 2: {
        int count = 0;
        int ok = 0;
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00471108));
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471128));
        sprintf1(wbuf, Loc_5098(D_00471140), cUITemplate_MAP_mPeak + 1);
        cUIText_setUnicodeString_oals(widget, wbuf);
        int cur = cBEMissionInterface_getNumberCompletedMissionsOnPeak(mis, cUITemplate_MAP_mPeak);
        int tot = cBEMissionInterface_getNumberMissionsOnPeak(mis, cUITemplate_MAP_mPeak);
        sprintf1(wbuf, Loc_5098(D_00471158), cur, tot);
        cUIText_setUnicodeString_oals(cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471170)), wbuf);
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471188));
        if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 0)) {
            widget->setVisible(0);
            ok = 1;
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 1)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(str_kT_CMNPlatinumAt), cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 0));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 2)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711B8), cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 1));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else if (cur >= cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 3)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711C8), cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 2));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else {
            sprintf1(wbuf, Loc_5098(D_004711D8), cBERewardInterface_getChallengesMedalTarget(rew, cUITemplate_MAP_mPeak, 3));
            cUIText_setUnicodeString_oals(widget, wbuf);
        }
        if (ok) {
            cUIState_showObjSafe(tmpl->mpUIState, D_004711E8);
            count++;
        } else {
            cUIState_hideObjSafe(tmpl->mpUIState, D_004711E8);
        }
        ok = 0;
        cur = cBEMissionInterface_getCurrentCollectForPeak(mis, 0, cUITemplate_MAP_mPeak);
        tot = cBEMissionInterface_getTotalCollectForPeak(mis, cUITemplate_MAP_mPeak);
        sprintf1(wbuf, Loc_5098(D_00471200), cur, tot);
        cUIText_setUnicodeString_oals(cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471218)), wbuf);
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(str_SPG_CollectablesCurrent));
        if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 0)) {
            widget->setVisible(0);
            ok = 1;
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 1)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(str_kT_CMNPlatinumAt), cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 0));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 2)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711B8), cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 1));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else if (cur >= cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 3)) {
            ok = 1;
            sprintf1(wbuf, Loc_5098(D_004711C8), cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 2));
            cUIText_setUnicodeString_oals(widget, wbuf);
        } else {
            sprintf1(wbuf, Loc_5098(D_004711D8), cBERewardInterface_getCollectibleMedalTarget(rew, cUITemplate_MAP_mPeak, 3));
            cUIText_setUnicodeString_oals(widget, wbuf);
        }
        if (ok) {
            cUIState_showObjSafe(tmpl->mpUIState, D_00471248);
            count++;
        } else {
            cUIState_hideObjSafe(tmpl->mpUIState, D_00471248);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471260));
        sprintf1(wbuf, Loc_5098(D_004710A0), count, 2);
        cUIText_setUnicodeString_oals(widget, wbuf);
        break;
    }
    case 3: {
        unsigned short num[16];
        cUIText_setUnicodeStringByID(title, GetHashValue32(D_00471278));
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471298));
        sprintf1(wbuf, Loc_5098(D_004712B0), cUITemplate_MAP_mPeak + 1);
        cUIText_setUnicodeString_oals(widget, wbuf);
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_004712C8));
        int cost = cBERewardInterface_getEarningsTarget(rew, cUITemplate_MAP_mPeak);
        unsigned short* fmt = Loc_5098(str_kT_CMNEarnNum);
        func_002C2540_oals(num, getDollarString(cost));
        sprintf1(wbuf, fmt, num);
        cUIText_setUnicodeString_oals(widget, wbuf);
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_004712F0));
        int money = cBEEconInterface_getTotalMoneyEarned1(econ, 0);
        fmt = Loc_5098(D_00471300);
        func_002C2540_oals(num, getDollarString(money));
        sprintf1(wbuf, fmt, num);
        cUIText_setUnicodeString_oals(widget, wbuf);
        int ok;
        if (cost > money)
            ok = 0;
        else
            ok = 1;
        widget = (cUIThing*)cUIScreen_getObjectByHashName(tmpl->mpScreen, GetHashValue32(D_00471318));
        sprintf1(wbuf, Loc_5098(D_004710A0), ok, 1);
        cUIText_setUnicodeString_oals(widget, wbuf);
        if (ok > 0)
            cUIState_showObjSafe(tmpl->mpUIState, str_SPG_Earningscheck);
        else
            cUIState_hideObjSafe(tmpl->mpUIState, str_SPG_Earningscheck);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_showEventPath);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const char* a, const char* b);
extern int cUITemplate_MAP_mPeak;
extern char D_004A2560[];
extern char str_peak1racepath[];
extern char str_peak1fspath[];
extern char str_peak2racepath[];
extern char str_peak2fspath[];
extern char str_peak3racepath[];
extern char str_peak3fspath[];
extern char str_peak1racegroup[];
extern char str_peak1fsgroup[];
extern char str_peak1freeridegroup[];
extern char str_peak2racegroup[];
extern char str_peak2fsgroup[];
extern char str_peak2freeridegroup[];
extern char str_peak3racegroup[];
extern char str_peak3fsgroup[];
extern char str_peak3freeridegroup[];
extern char* D_004713A8[];
extern char* D_00471420[];
extern char* D_00471488[];
extern char* D_004714A0[];
extern char* D_004A26A8[];
extern char* D_004A26B0[];
extern char* D_004A26B8[];
extern char* D_004A26C0[];
extern char* D_004A26C8[];
extern char D_00478225[][0x6C];
extern char D_00478735[][0x6C];
extern char D_004783D5[][0x6C];
extern char D_00478951[][0x6C];
extern char D_00478585[][0x6C];
extern char D_00478B6D[][0x6C];

struct sNameList2_206690 {
    char* s[2];
};
struct sNameList3_206690 {
    char* s[3];
};
struct sNameList5_206690 {
    char* s[5];
};
struct sNameList6_206690 {
    char* s[6];
};

/**
 * Highlights one event's path on the current peak's map: that path in red, the others in orange.
 *
 * Does nothing unless @p name is an object on the screen or the empty name.
 *
 * @param tmpl  the map template
 * @param name  the path object to highlight (On Tour pathName)
 */
// Recolours the map's event-path objects for the event or peak called `name`: objects on the
// path get col0, the rest col1. The name lists are local copies of constant tables.
extern "C" void cUITemplate_MAP_showEventPath(cUITemplate_MAP* tmpl, char* name)
{
    sMapTmpl_200700* self = (sMapTmpl_200700*)tmpl;
    sColor_202828 col0;
    col0.a = 1.0f;
    col0.r = 0.8901961445808411f;
    col0.g = 0.16470588743686676f;
    col0.b = 0.03529411926865578f;
    sColor_202828 col1;
    col1.a = 1.0f;
    col1.r = 0.8705883026123047f;
    col1.g = 0.5176470875740051f;
    col1.b = 0.13333334028720856f;
    sNameList6_206690 namesA = *(sNameList6_206690*)D_004713A8;
    sNameList6_206690 namesB = *(sNameList6_206690*)D_00471420;
    sNameList5_206690 namesC = *(sNameList5_206690*)D_00471488;
    sNameList3_206690 peak1RacePath = *(sNameList3_206690*)D_004714A0;
    sNameList2_206690 peak1FreestylePath = *(sNameList2_206690*)D_004A26A8;
    sNameList2_206690 peak2RacePath = *(sNameList2_206690*)D_004A26B0;
    sNameList2_206690 peak2FreestylePath = *(sNameList2_206690*)D_004A26B8;
    sNameList2_206690 peak3RacePath = *(sNameList2_206690*)D_004A26C0;
    sNameList2_206690 peak3FreestylePath = *(sNameList2_206690*)D_004A26C8;
    cUIPic_202828* obj;


    if (cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name)) != 0
        || func_004165A8(name, D_004A2560) == 0) {
        switch (cUITemplate_MAP_mPeak) {
        case 0:
            for (int i = 0; i < 6; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
                if (obj != 0) {
                    if (func_004165A8(namesA.s[i], name) == 0) {
                        obj->setColour(&col0);
                    } else {
                        obj->setColour(&col1);
                    }
                }
            }
            break;
        case 1:
            for (int i = 0; i < 6; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
                if (obj != 0) {
                    if (func_004165A8(namesB.s[i], name) == 0) {
                        obj->setColour(&col0);
                    } else {
                        obj->setColour(&col1);
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < 5; i++) {
                obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
                if (obj != 0) {
                    if (func_004165A8(namesC.s[i], name) == 0) {
                        obj->setColour(&col0);
                    } else {
                        obj->setColour(&col1);
                    }
                }
            }
            break;
        }
    } else if (func_004165A8(name, str_peak1racepath) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 3; j++) {
                    if (func_004165A8(namesA.s[i], peak1RacePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak1fspath) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(namesA.s[i], peak1FreestylePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak2racepath) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(namesB.s[i], peak2RacePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak2fspath) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(namesB.s[i], peak2FreestylePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak3racepath) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(namesC.s[i], peak3RacePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak3fspath) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
                for (int j = 0; j < 2; j++) {
                    if (func_004165A8(namesC.s[i], peak3FreestylePath.s[j]) == 0) {
                        obj->setColour(&col0);
                    }
                }
            }
        }
    } else if (func_004165A8(name, str_peak1racegroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 3; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478225[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak1fsgroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478735[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak1freeridegroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesA.s[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak2racegroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 3; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004783D5[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak2fsgroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478951[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak2freeridegroup) == 0) {
        for (int i = 0; i < 6; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesB.s[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak3racegroup) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 2; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478585[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak3fsgroup) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
            if (obj != 0) {
                obj->setColour(&col1);
            }
        }
        for (int i = 0; i < 4; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00478B6D[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    } else if (func_004165A8(name, str_peak3freeridegroup) == 0) {
        for (int i = 0; i < 5; i++) {
            obj = (cUIPic_202828*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(namesC.s[i]));
            if (obj != 0) {
                obj->setColour(&col0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setupLocationInfo);
#ifdef SKIP_ASM
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cUIText_setAsciiString(cUIText* text, const char* str);
int cBERewardInterface_getTrackMedal(void* iface, int player, int charID, int type, int event);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* cBENewRaceInterface_getLocationName(void* iface, int event);
extern "C" int cBENewRaceInterface_getLocationDefaultGameEvent(void* iface, int event);
extern "C" int cBEMissionInterface_getTotalCollectForSection(void* iface, int event);
extern "C" int cBEMissionInterface_getCurrentCollectForSection(void* iface, int a, int event);
extern "C" int cBEMissionInterface_getNumberMissionsOnTrack(void* iface, int event);
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnTrack(void* iface, int event);
extern "C" int func_001550E8(void* iface, int a, int event, int type);
extern "C" char* func_0020A1A8(int t);
extern "C" void strcpy1(void* dst, void* src);
extern "C" void func_002C2540(void* dst, char* src);
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeStringWrap(void* text, void* wstr);
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern "C" void cUITemplate_MAP_showEventPath(cUITemplate_MAP* tmpl, char* name);

struct sRewardDesc_0388 {
    int w[0x6C / 4];
};
extern sRewardDesc_0388 D_004781D0[][4];
extern sRewardDesc_0388 D_004786E0[][5];
extern sRewardDesc_0388 D_00478D38[][8];

struct sMapTmpl_207430 {
    int showInfo;                 // 0x0
    int peakUnlocked[3];          // 0x4
    int goalUnlocked[4];          // 0x10
    int eventUnlocked[3][3][8];   // 0x20 [peak][goal][event]
    int eventDone[3][3][8];       // 0x140
    sRewardDesc_0388 desc;        // 0x260
    int popup;                    // 0x2CC
    int mode;                     // 0x2D0
    int pic;                      // 0x2D4
    int file;                     // 0x2D8
    int f2DC;
    int f2E0;
    void* map;                    // 0x2E4
    void* screen;                 // 0x2E8
    void* owner;                  // 0x2EC
};

struct sLocVEntry_207430 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

extern int cUITemplate_MAP_mSelectedEvent;
extern int cUITemplate_MAP_mPeak;
extern char str_Peak_Locations_Art[];
extern char str_ehl[];
extern char str_Peakpct_dRaceLocations[];
extern char str_SE_boxpct_d[];
extern char str_SE_checkpct_d[];
extern char str_Peakpct_dFreestyleLocations[];
extern char str_Peakpct_dFreerideLocations[];
extern char str_SE_Run[];
extern char str_SE_Event[];
extern char str_kT_CMNHELPStations[];
extern char str_kT_HELPChalAvail[];
extern char str_kT_HELPLockCompRace[];
extern char str_kT_HELPLockCompFree[];
extern char D_004A2590[];
extern char str_kT_HELPLockCompEvent[];
extern char str_pct_S_Jam_b[];
extern char str_kT_cmnRaceCaps_b[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_CMNSuperpipe[];
extern char str_kT_TITLERivalChallenge[];
extern char str_kT_MAPChal10[];
extern char str_kT_MAPChal9[];
extern char str_Rival[];
extern char str_kT_CMNStateFreeRide[];
extern char str_SE_Data1Label[];
extern char str_kT_MAPCollectibles[];
extern char str_SE_Data1[];
extern char str_pct_d_pct_d_d[];
extern char str_SE_Data2Label[];
extern char str_kT_MAPBigChallenges[];
extern char str_SE_Data2[];
extern char str_N_A[];
extern char str_kT_MAPMedal[];
extern char str_kT_CMNNoMedal[];
extern char str_kT_CMNTopTime[];
extern char str_kT_CMNTopScore[];
extern char str_dash[];
extern char D_004A2138[];

static inline void* getObj_207430(sMapTmpl_207430* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
}

// PORT: the unit declares cUITemplate_MAP_setupLocationInfo(void*).
extern "C" void cUITemplate_MAP_setupLocationInfo_r(sMapTmpl_207430* self) __asm__("cUITemplate_MAP_setupLocationInfo");

/**
 * Fills the map's location panel for the location selected in the current event list: name, event type, help text, and
 * medal and best result or the freeride collectibles.
 *
 * Shows or hides "Peak_Locations_Art" and "ehl" by the popup flag at +0x2CC, and in map state 2
 * (cUITemplate_MAP_mMapState) shows "PEAKno" with its front-end or PDA backdrop (cUITemplate_MAP_mMapStyle 0 or not).
 * The selected entry of the "Peak%dRaceLocations", "Peak%dFreestyleLocations" or "Peak%dFreerideLocations" list
 * (cUITemplate_MAP_mSelectedEvent 0, 1 or 2; the list's selection byte +0x95) picks a 0x6C-byte goal entry from
 * D_004781D0, D_004786E0 or D_00478D38; for races (up to 4 entries, 3 on peak 3) and freestyle (5) the "SE_box%d" boxes
 * are shown with map styles 2 and up and the "SE_check%d" ticks mirror the completion flags, all other boxes are
 * hidden.
 *
 * "SE_Run" gets the location name (cBENewRaceInterface_getLocationName); locations 17-21 are stations (event type 12,
 * help kT_CMNHELPStations). Otherwise the help text is the entry's own key when it has one, else depends on the event
 * type: 4 and 5 (rival races) show kT_HELPChalAvail or kT_HELPLockCompRace / kT_HELPLockCompFree by the unlock flag;
 * 6-8 and 9-11 build "kT_HELPLockCompEvent" around the location name (the second with the "%S Jam" format) through the
 * FE locale. Event type 12 is resolved with cBENewRaceInterface_getLocationDefaultGameEvent. "SE_Event" and "PEAKno" then name the event type (race,
 * slopestyle, superpipe, big air, rival challenge, kT_MAPChal10 / kT_MAPChal9 with "Rival", freeride). In map state 2
 * the path named at +0x55 of the entry is shown (cUITemplate_MAP_showEventPath).
 *
 * For freeride the two data rows show collectibles found / total (cBEMissionInterface_getCurrentCollectForSection, cBEMissionInterface_getTotalCollectForSection) and big challenges done
 * / total (cBEMissionInterface_getNumberCompletedMissionsOnTrack, cBEMissionInterface_getNumberMissionsOnTrack, "N/A" when there are none); otherwise the medal
 * (cBERewardInterface_getTrackMedal through cUITemplate_MAP_getMedalStr, or kT_CMNNoMedal) and the top time (races,
 * formatted by func_0020A1A8) or top score (func_001550E8), with str_dash for no result.
 */
extern "C" void cUITemplate_MAP_setupLocationInfo_r(sMapTmpl_207430* self)
{
    char buf[200];
    sRewardDesc_0388 desc;
    char name[32];
    cUIObj_200700* mapObj = (cUIObj_200700*)getObj_207430(self, str_HelpText);
    if (cUITemplate_MAP_mMapState == 2)
        mapObj->setVisible(1);
    void* mission = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    int charID = cBELibrary_getCharacterID(0);
    if (self->popup != 0) {
        cUIState_hideObjSafe(self->owner, str_Peak_Locations_Art);
        cUIState_hideObjSafe(self->owner, str_ehl);
    } else {
        cUIState_showObjSafe(self->owner, str_Peak_Locations_Art);
        cUIState_showObjSafe(self->owner, str_ehl);
    }
    if (cUITemplate_MAP_mMapState == 2) {
        cUIState_showObjSafe(self->owner, str_PEAKno);
        if (cUITemplate_MAP_mMapStyle == 0)
            cUIState_showObjSafe(self->owner, str_fePEAKno_back);
        else
            cUIState_showObjSafe(self->owner, str_pdaPEAKno_back);
    }
    cUIText* title = (cUIText*)getObj_207430(self, str_PEAKno);
    desc = D_004781D0[0][0];
    int sel = 0;
    switch (cUITemplate_MAP_mSelectedEvent) {
    case 0: {
        sprintf(name, str_Peakpct_dRaceLocations, cUITemplate_MAP_mPeak + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_004781D0[cUITemplate_MAP_mPeak][sel];
        int n = 4;
        if (cUITemplate_MAP_mPeak == 2)
            n = 3;
        for (int i = 0; i < 8; i++) {
            if (i < n && cUITemplate_MAP_mMapStyle >= 2) {
                sprintf(buf, str_SE_boxpct_d, i);
                cUIState_showObjSafe(self->owner, buf);
                sprintf(buf, str_SE_checkpct_d, i);
                if (self->eventDone[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][i] != 0)
                    cUIState_showObjSafe(self->owner, buf);
                else
                    cUIState_hideObjSafe(self->owner, buf);
            } else {
                sprintf(buf, str_SE_boxpct_d, i);
                cUIState_hideObjSafe(self->owner, buf);
                sprintf(buf, str_SE_checkpct_d, i);
                cUIState_hideObjSafe(self->owner, buf);
            }
        }
        break;
    }
    case 1: {
        sprintf(name, str_Peakpct_dFreestyleLocations, cUITemplate_MAP_mPeak + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_004786E0[cUITemplate_MAP_mPeak][sel];
        for (int i = 0; i < 8; i++) {
            if (i < 5 && cUITemplate_MAP_mMapStyle >= 2) {
                sprintf(buf, str_SE_boxpct_d, i);
                cUIState_showObjSafe(self->owner, buf);
                sprintf(buf, str_SE_checkpct_d, i);
                if (self->eventDone[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][i] != 0)
                    cUIState_showObjSafe(self->owner, buf);
                else
                    cUIState_hideObjSafe(self->owner, buf);
            } else {
                sprintf(buf, str_SE_boxpct_d, i);
                cUIState_hideObjSafe(self->owner, buf);
                sprintf(buf, str_SE_checkpct_d, i);
                cUIState_hideObjSafe(self->owner, buf);
            }
        }
        break;
    }
    case 2: {
        char buf2[32];
        sprintf(name, str_Peakpct_dFreerideLocations, cUITemplate_MAP_mPeak + 1);
        sel = *(unsigned char*)((char*)getObj_207430(self, name) + 0x95);
        desc = D_00478D38[cUITemplate_MAP_mPeak][sel];
        for (int i = 0; i < 8; i++) {
            sprintf(buf2, str_SE_checkpct_d, i);
            cUIState_hideObjSafe(self->owner, buf2);
            sprintf(buf2, str_SE_boxpct_d, i);
            cUIState_hideObjSafe(self->owner, buf2);
        }
        break;
    }
    }
    cUIText* text = (cUIText*)getObj_207430(self, str_SE_Run);
    cUIText_setAsciiString(text, cBENewRaceInterface_getLocationName(race, desc.w[0]));
    text = (cUIText*)getObj_207430(self, str_SE_Event);
    int type = desc.w[2];
    if ((unsigned int)(desc.w[0] - 17) < 5) {
        type = 0xC;
        if (cUITemplate_MAP_mMapState == 2)
            cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_CMNHELPStations));
    } else {
        if (func_004165A8((char*)&desc.w[0] + 0x32, D_004A2560) != 0) {
            if (cUITemplate_MAP_mMapState == 2)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32((char*)&desc.w[0] + 0x32));
        } else if (desc.w[2] == 4) {
            if (self->eventUnlocked[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][sel] != 0)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPChalAvail));
            else
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPLockCompRace));
        } else if (desc.w[2] == 5) {
            if (self->eventUnlocked[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][sel] != 0)
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPChalAvail));
            else
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPLockCompFree));
        } else if (desc.w[2] >= 6 && desc.w[2] <= 8) {
            if (self->eventUnlocked[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][sel] != 0) {
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPChalAvail));
            } else {
                short fmt[16];
                short part[304];
                short pre[304];
                short line[304];
                func_002C2540(fmt, D_004A2590);
                sprintf1_v(part, fmt, cBENewRaceInterface_getLocationName(race, desc.w[0]));
                char* loc = (char*)SSXApp->FELocale;
                sLocVEntry_207430* vt = *(sLocVEntry_207430**)(loc + 4);
                char* thisp = loc + vt[4].delta;
                strcpy1(pre, vt[4].fn(thisp, GetHashValue32(str_kT_HELPLockCompEvent)));
                sprintf1_v(line, pre, part);
                cUIText_setUnicodeStringWrap(mapObj, line);
            }
        } else if (desc.w[2] >= 9 && desc.w[2] <= 11) {
            if (self->eventUnlocked[cUITemplate_MAP_mPeak][cUITemplate_MAP_mSelectedEvent][sel] != 0) {
                cUIText_setUnicodeStringByID((cUIText*)mapObj, GetHashValue32(str_kT_HELPChalAvail));
            } else {
                short part[304];
                short fmt[16];
                short pre[312];
                short line[304];
                func_002C2540(fmt, str_pct_S_Jam_b);
                sprintf1_v(part, fmt, cBENewRaceInterface_getLocationName(race, desc.w[0]));
                char* loc = (char*)SSXApp->FELocale;
                sLocVEntry_207430* vt = *(sLocVEntry_207430**)(loc + 4);
                char* thisp = loc + vt[4].delta;
                strcpy1(pre, vt[4].fn(thisp, GetHashValue32(str_kT_HELPLockCompEvent)));
                sprintf1_v(line, pre, part);
                cUIText_setUnicodeStringWrap(mapObj, line);
            }
        }
        if (type == 0xC)
            type = cBENewRaceInterface_getLocationDefaultGameEvent(race, desc.w[0]);
    }
    switch (type) {
    case 0:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_cmnRaceCaps_b));
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_cmnRaceCaps_b));
        break;
    case 1:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_cmnSlopeCaps));
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_cmnSlopeCaps));
        break;
    case 3:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_cmnBigAirCaps));
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_cmnBigAirCaps));
        break;
    case 2:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNSuperpipe));
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_CMNSuperpipe));
        break;
    case 4:
    case 5:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_TITLERivalChallenge));
        cUIText_setAsciiString(title, str_Rival);
        break;
    case 6:
    case 7:
    case 8:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_MAPChal10));
        cUIText_setAsciiString(title, str_Rival);
        break;
    case 9:
    case 10:
    case 11:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_MAPChal9));
        cUIText_setAsciiString(title, str_Rival);
        break;
    case 12:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNStateFreeRide));
        cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_CMNStateFreeRide));
        break;
    }
    if (cUITemplate_MAP_mMapState == 2)
        cUITemplate_MAP_showEventPath((cUITemplate_MAP*)self, (char*)&desc.w[0] + 0x55);
    if (cUITemplate_MAP_mSelectedEvent == 2) {
        text = (cUIText*)getObj_207430(self, str_SE_Data1Label);
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_MAPCollectibles));
        text = (cUIText*)getObj_207430(self, str_SE_Data1);
        sprintf(buf, str_pct_d_pct_d_d, cBEMissionInterface_getCurrentCollectForSection(mission, 0, desc.w[0]), cBEMissionInterface_getTotalCollectForSection(mission, desc.w[0]));
        cUIText_setAsciiString(text, buf);
        text = (cUIText*)getObj_207430(self, str_SE_Data2Label);
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_MAPBigChallenges));
        text = (cUIText*)getObj_207430(self, str_SE_Data2);
        sprintf(buf, str_pct_d_pct_d_d, cBEMissionInterface_getNumberCompletedMissionsOnTrack(mission, desc.w[0]), cBEMissionInterface_getNumberMissionsOnTrack(mission, desc.w[0]));
        cUIText_setAsciiString(text, buf);
        if (cBEMissionInterface_getNumberMissionsOnTrack(mission, desc.w[0]) == 0)
            cUIText_setAsciiString(text, str_N_A);
    } else {
        cUIState_showObjSafe(self->owner, str_SE_Data1Label);
        cUIState_showObjSafe(self->owner, str_SE_Data1);
        text = (cUIText*)getObj_207430(self, str_SE_Data1Label);
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_MAPMedal));
        int medal = cBERewardInterface_getTrackMedal(reward, 0, charID, desc.w[2], desc.w[0]);
        text = (cUIText*)getObj_207430(self, str_SE_Data1);
        if (medal != -1)
            cUIText_setUnicodeString(text, (char*)cUITemplate_MAP_getMedalStr((cUITemplate_MAP*)self, medal));
        else
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNNoMedal));
        cUIText* label = (cUIText*)getObj_207430(self, str_SE_Data2Label);
        cUIText* value = (cUIText*)getObj_207430(self, str_SE_Data2);
        if (cUITemplate_MAP_mSelectedEvent == 0) {
            cUIText_setUnicodeStringByID(label, GetHashValue32(str_kT_CMNTopTime));
            int best = func_001550E8(score, 0, desc.w[0], desc.w[2]);
            cUIText_setAsciiString(value, func_0020A1A8(best));
            if (best == 0)
                cUIText_setAsciiString(value, str_dash);
        } else {
            cUIText_setUnicodeStringByID(label, GetHashValue32(str_kT_CMNTopScore));
            int best = func_001550E8(score, 0, desc.w[0], desc.w[2]);
            char num[32];
            sprintf(num, D_004A2138, best);
            cUIText_setAsciiString(value, num);
            if (best == 0)
                cUIText_setAsciiString(value, str_dash);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setWidgetHiliteColor);
#ifdef SKIP_ASM
struct sVEntry_cUITemplate_MAP_setWidgetHiliteColor {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char kMAPCOLOR_HIGHLIGHTED[];
extern char kMAPCOLOR_HIGHLIGHTED_LOCKED[];

/**
 * Colours a highlighted map entry: the normal highlight when unlocked, the locked highlight otherwise.
 *
 * Does nothing for a null widget; otherwise calls its virtual slot 11 (set colour) with kMAPCOLOR_HIGHLIGHTED
 * when @p on is nonzero, else kMAPCOLOR_HIGHLIGHTED_LOCKED.
 *
 * @param obj  the text widget
 * @param on   nonzero when the entry is unlocked
 */
extern "C" void cUITemplate_MAP_setWidgetHiliteColor(cUITemplate_MAP* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_cUITemplate_MAP_setWidgetHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, kMAPCOLOR_HIGHLIGHTED);
        } else {
            sVEntry_cUITemplate_MAP_setWidgetHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, kMAPCOLOR_HIGHLIGHTED_LOCKED);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/uitemplatemap", cUITemplate_MAP_setWidgetUnHiliteColor);
#ifdef SKIP_ASM
struct sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
extern char kMAPCOLOR_REGULAR[];
extern char kMAPCOLOR_REGULAR_LOCKED[];

/**
 * Sets a map text widget's unhighlighted colour: regular when unlocked, the locked colour otherwise.
 *
 * Calls the widget's virtual slot 11 with kMAPCOLOR_REGULAR or kMAPCOLOR_REGULAR_LOCKED. Does nothing for a
 * null widget.
 *
 * @param obj  the text widget (On Tour cUIText* pText)
 * @param on   nonzero when the item is unlocked (On Tour unlocked)
 */
extern "C" void cUITemplate_MAP_setWidgetUnHiliteColor(cUITemplate_MAP* self, void* obj, int on)
{
    if (obj != 0) {
        if (on != 0) {
            sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, kMAPCOLOR_REGULAR);
        } else {
            sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor* vt = *(sVEntry_cUITemplate_MAP_setWidgetUnHiliteColor**)((char*)obj + 0x8);
            vt[11].fn((char*)obj + vt[11].delta, kMAPCOLOR_REGULAR_LOCKED);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cUITemplate_MAP_onCreateScreen(cUITemplate_MAP* tmpl, void* screen, cUIState* owner);
extern char str_Map_b[];

/**
 * Builds the transport overlay's screen: adds the "Map" screen, plays its frame 0, then runs the overlay and
 * map-template screen setup.
 *
 * Adds the screen with cUIEngine_addScreenByHashName on mpEngine and keeps it in mpScreen, plays frame 0 when it
 * exists, calls cOVState_onCreateScreen and then cUITemplate_MAP_onCreateScreen for the template embedded at +0x9C with
 * mpScreen and this state. It is the onCreateScreen slot (+0x24) of D_004733F8, the vtable of overlay 33
 * (cOVStateManager_triggerOverlay), whose template cUITemplate_MAP_init initialises.
 */
extern "C" void cOVState_TRANSPORT_onCreateScreen(cOVState_TRANSPORT* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_Map_b), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cUITemplate_MAP_onCreateScreen(&self->mMap, self->mpScreen, (cUIState*)self);
}
#endif

extern "C" void* cUITemplate_MAP_onWidgetCreate(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Transport overlay's widget-create hook: forwards to the embedded map template (cUITemplate_MAP_onWidgetCreate on +0x9C).
 *
 * As declared it takes only the state; the widget reaches cUITemplate_MAP_onWidgetCreate untouched in the second argument register.
 *
 * @return what cUITemplate_MAP_onWidgetCreate leaves in the return register (that function returns nothing)
 */
void* cOVState_TRANSPORT_onWidgetCreate(void* vself)
{
    cOVState_TRANSPORT* self = (cOVState_TRANSPORT*)vself;
    return cUITemplate_MAP_onWidgetCreate((char*)self + 0x9c);
}
#endif

extern "C" void* cUITemplate_MAP_onGainTransition(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Transport overlay's gain-transition hook: forwards to the embedded map template (cUITemplate_MAP_onGainTransition on
 * +0x9C).
 *
 * As declared it takes only the state; the first-time flag reaches the template untouched in the second argument
 * register.
 *
 * @return the template's result
 */
void* cOVState_TRANSPORT_onGainTransition(void* vself)
{
    cOVState_TRANSPORT* self = (cOVState_TRANSPORT*)vself;
    return cUITemplate_MAP_onGainTransition((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUIState_onGainFocus(void* self);
extern "C" void cUITemplate_MAP_onGainFocus(cUITemplate_MAP* self);

/**
 * Transport overlay's gain-focus hook: runs cUIState_onGainFocus, then cUITemplate_MAP_onGainFocus for the template at
 * +0x9C.
 */
extern "C" void cOVState_TRANSPORT_onGainFocus(cOVState_TRANSPORT* self)
{
    cUIState_onGainFocus(self);
    cUITemplate_MAP_onGainFocus(&self->mMap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_onDestroyScreen(cUITemplate_MAP* self);
extern "C" void cOVState_onDestroyScreen(void* self);

/**
 * Transport overlay's screen teardown: tears down the map template at +0x9C (cUITemplate_MAP_onDestroyScreen), then runs
 * cOVState_onDestroyScreen.
 */
extern "C" void cOVState_TRANSPORT_onDestroyScreen(cOVState_TRANSPORT* self)
{
    cUITemplate_MAP_onDestroyScreen(&self->mMap);
    cOVState_onDestroyScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" int cUITemplate_MAP_onWidgetEvent(void* p);
// PORT: returns int (value unused here); the int return type keeps the target's register choice after the call.
extern "C" int cUIStateStack_pop_85B8(void* self, int a1) __asm__("cUIStateStack_pop");

/**
 * Transport overlay's widget handler: when the map template reports a pick (cUITemplate_MAP_onWidgetEvent returns 1),
 * closes the overlay.
 *
 * Then pops one state from mpEngine's state stack (cUIStateStack_pop, On Tour cUIStateStack::pop) and sets mode 7 plus the
 * pending-delete bit in mAttrState (bits 8-13 = 7, bit 7 set). As declared the template call only gets the template
 * (+0x9C); the widget and event reach it in the argument registers.
 */
extern "C" void cOVState_TRANSPORT_onWidgetEvent(cOVState_TRANSPORT* self)
{
    if (cUITemplate_MAP_onWidgetEvent(&self->mMap) == 1) {
        cUIStateStack_pop_85B8((char*)self->mpEngine + 0x18, 1);
        self->mAttrState = (self->mAttrState & ~0x3F00) | 0x780;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Transport overlay's menu-event filter: forwards to the map template's filter (cUITemplate_MAP_onWidgetMenu) and returns 0x101 when
 * that one returns nonzero, else 0.
 *
 * @param menu  the menu, passed on
 * @param event  the menu event, passed on
 * @return 0x101 or 0
 */
extern "C" int cOVState_TRANSPORT_onWidgetMenu(cOVState_TRANSPORT* self, int menu, int event)
{
    if (cUITemplate_MAP_onWidgetMenu(&self->mMap, menu, event) != 0) {
        return 257;
    }
    return 0;
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onInputBegin);
#ifdef SKIP_ASM
/**
 * Transport overlay's input hook: calls the map template's (cUITemplate_MAP_onInputBegin on +0x9C) and drops its result.
 */
extern "C" void cOVState_TRANSPORT_onInputBegin(cOVState_TRANSPORT* self)
{
    cUITemplate_MAP_onInputBegin((char*)&self->mMap);
}
#endif

extern "C" void* cUITemplate_MAP_onInputAlways(void*);

//99.29%
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onInputAlways__FPv);
#ifdef SKIP_ASM
/**
 * Transport overlay's every-frame input hook: forwards to cUITemplate_MAP_onInputAlways for the template at +0x9C.
 *
 * As declared it takes only the state; the input reaches the template untouched in the second argument register.
 *
 * @return what cUITemplate_MAP_onInputAlways leaves in the return register (it returns nothing)
 */
void* cOVState_TRANSPORT_onInputAlways(void* vself)
{
    cOVState_TRANSPORT* self = (cOVState_TRANSPORT*)vself;
    return cUITemplate_MAP_onInputAlways((char*)self + 0x9c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/uitemplatemap", cOVState_TRANSPORT_onUpdate);
#ifdef SKIP_ASM
extern "C" void cOVState_onUpdate(void* self);
extern "C" void cUITemplate_MAP_onUpdate(cUITemplate_MAP* self);

/**
 * Transport overlay's update: runs cOVState_onUpdate, then cUITemplate_MAP_onUpdate for the template at +0x9C.
 */
extern "C" void cOVState_TRANSPORT_onUpdate(cOVState_TRANSPORT* self)
{
    cOVState_onUpdate(self);
    cUITemplate_MAP_onUpdate(&self->mMap);
}
#endif

