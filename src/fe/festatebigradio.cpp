#include "common.h"
#include "be/beinterface.h"
#include "fe/festatenethelppopup.h"
#include "ui/uimenu.h"
#include "ui/uiengine.h"
#include "fe/screenpopup.h"
#include "ui/uiinput.h"
#include "ui/uithing.h"
#include "fe/festateruleselect.h"
#include "ui/uistate.h"
#include "be/beintnewrace.h"
#include "online/lobbyman.h"
#include "fe/festatenet.h"
#include "ui/uiscreen.h"

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onCreateScreen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: several callees are declared with other parameter types elsewhere; bind local views by asm label
extern "C" void cBraggingRightsMenu_setConfirmed_2DB0(void* panel, int on) __asm__("cBraggingRightsMenu_setConfirmed");
void cBraggingRightsMenu_setEnableFlag(void* panel, int mask);
extern "C" void cBraggingRightsMenu_setEnableItem_2DB0(void* panel, void* screen) __asm__("cBraggingRightsMenu_setEnableItem");
extern "C" int cFEStateBraggingRights_getBEData(void* self, int player, int which);
extern "C" unsigned char cUIListBox_addEntryByStringID_2DB0(void* box, int id, int idx) __asm__("cUIListBox_addEntryByStringID");
extern "C" unsigned char cUIListBox_addEntryByAsciiString_2DB0(void* box, const char* str, int data) __asm__("cUIListBox_addEntryByAsciiString");
extern "C" int cUIListBox_getEntryByData_2DB0(void* item, int key, unsigned char* out) __asm__("cUIListBox_getEntryByData");
extern "C" void cUIListBox_setCurrentEntryByIndex_2DB0(void* item, int v) __asm__("cUIListBox_setCurrentEntryByIndex");
extern int D_004A1A70;
extern signed char D_004A1710[6];
extern signed char D_004A1718[7];
extern signed char D_004A1720[7];
extern signed char D_004A1728[7];
extern signed char D_004A1730[5];
extern signed char D_004A1738[7];
extern char* D_00441108[];
extern char str_65bragrights[];
extern char D_004A14F8[];
extern char str_kT_CMNNo[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNOff[];
extern char str_5s[];
extern char str_10s[];
extern char str_15s[];
extern char str_20s[];
extern char str_3000[];
extern char str_5000[];
extern char str_10000[];
extern char str_15000[];
extern char str_ongroup[];
extern char str_fegroup[];
extern char str_x_continue2[];
extern char str_nml_player2[];
extern char str_x_nml_player2[];

struct sStr2_2DB0 { char c[2]; };

class cUIObj_2DB0 {
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



static inline void* find_2DB0(cFEStateBraggingRights* self, char* name)
{
    return cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
}

static inline bool isNet_2DB0()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}

/**
 * Builds the bragging-rights screen: the rule rows of the current event and both players' choice lists.
 *
 * Creates "65bragrights", unlocks both panels when bit 4 of +0x1C is set, and picks the rule table and row
 * count (5-7) from the game event. Each row's label gets the rule's text id (D_00441108), and each panel's
 * list gets No/Yes for kBRAGRIGHTS_GIVEFULLBOOST .. kBRAGRIGHTS_TAKEHALFPOINTS, Off/5s-20s for
 * kBRAGRIGHTS_GIVEHEADSTART and Off/3000-15000 for kBRAGRIGHTS_STARTINHOLE, with the current value
 * (cFEStateBraggingRights_getBEData) selected. Online (D_004A1A70 == cFEStateNet::kFEPathOnline) shows "ongroup" and
 * hides the second player's widgets.
 */
// Bragging rights screen: builds the rule rows of both player panels for the current mode.
void cFEStateBraggingRights::onCreateScreen()
{
    void* engine = ((void*)mpEngine);
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_65bragrights), 0);
    (*(void**)&mpScreen) = screen;
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    cBraggingRightsMenu* firstPanel = &mMenuCtrl[0];
    cBraggingRightsMenu* secondPanel = &mMenuCtrl[1];
    if ((mAttrState >> 4) & 1) {
        cBraggingRightsMenu_setConfirmed_2DB0(firstPanel, 0);
        cBraggingRightsMenu_setConfirmed_2DB0(secondPanel, 0);
    }
    int n = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    unsigned int mask = 0;
    switch (cBENewRaceInterface_newRace.cGameEvent) {
    case kGAMEEVENT_RACE:
        (*(signed char**)&mpGameEventMapping) = D_004A1710;
        n = 6;
        break;
    case kGAMEEVENT_SLOPESTYLE:
        (*(signed char**)&mpGameEventMapping) = D_004A1718;
        n = 7;
        break;
    case kGAMEEVENT_BIGAIR:
        (*(signed char**)&mpGameEventMapping) = D_004A1720;
        n = 7;
        break;
    case kGAMEEVENT_SUPERPIPE:
        (*(signed char**)&mpGameEventMapping) = D_004A1728;
        n = 7;
        break;
    case kGAMEEVENT_RIVALTIME:
        (*(signed char**)&mpGameEventMapping) = D_004A1730;
        n = 5;
        break;
    case kGAMEEVENT_RIVALPOINTS:
        (*(signed char**)&mpGameEventMapping) = D_004A1738;
        n = 7;
        break;
    }
    for (int i = 0; i < n; i++)
        mask |= 1 << i;
    cBraggingRightsMenu_setEnableFlag(firstPanel, mask);
    cBraggingRightsMenu_setEnableFlag(secondPanel, mask);
    cBraggingRightsMenu_setEnableItem_2DB0(firstPanel, ((void*)mpScreen));
    cBraggingRightsMenu_setEnableItem_2DB0(secondPanel, ((void*)mpScreen));
    sStr2_2DB0 name = *(sStr2_2DB0*)D_004A14F8;
    for (int row = 0; row < n; row++) {
        name.c[0] = row + '1';
        cUIText* txt = (cUIText*)find_2DB0(this, name.c);
        if (txt != 0)
            cUIText_setUnicodeStringByID(txt, GetHashValue32(D_00441108[((signed char*)mpGameEventMapping)[row]]));
        for (int playerIndex = 0; playerIndex < 2; playerIndex++) {
            unsigned char out = 0;
            void* lb = (&((cBraggingRightsMenu*)(&mMenuCtrl[0]))[playerIndex].mpListBox[0])[row];
            switch (((signed char*)mpGameEventMapping)[row]) {
            case kBRAGRIGHTS_GIVEFULLBOOST:
            case kBRAGRIGHTS_GIVEFASTRECOVERY:
            case kBRAGRIGHTS_TAKENOUBER:
            case kBRAGRIGHTS_TAKELEAKYAD:
            case kBRAGRIGHTS_TAKELV1CHAR:
            case kBRAGRIGHTS_TAKEHALFPOINTS:
                cUIListBox_addEntryByStringID_2DB0(lb, GetHashValue32(str_kT_CMNNo), 0);
                cUIListBox_addEntryByStringID_2DB0(lb, GetHashValue32(str_kT_CMNYes), 1);
                cUIListBox_getEntryByData_2DB0(lb, cFEStateBraggingRights_getBEData(this, playerIndex,
                        ((signed char*)mpGameEventMapping)[row]), &out);
                cUIListBox_setCurrentEntryByIndex_2DB0(lb, out);
                break;
            case kBRAGRIGHTS_GIVEHEADSTART:
                cUIListBox_addEntryByStringID_2DB0(lb, GetHashValue32(str_kT_CMNOff), 0);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_5s, 5);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_10s, 10);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_15s, 15);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_20s, 20);
                cUIListBox_getEntryByData_2DB0(lb, cFEStateBraggingRights_getBEData(this, playerIndex,
                        ((signed char*)mpGameEventMapping)[row]), &out);
                cUIListBox_setCurrentEntryByIndex_2DB0(lb, out);
                break;
            case kBRAGRIGHTS_STARTINHOLE:
                cUIListBox_addEntryByStringID_2DB0(lb, GetHashValue32(str_kT_CMNOff), 0);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_3000, 3000);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_5000, 5000);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_10000, 10000);
                cUIListBox_addEntryByAsciiString_2DB0(lb, str_15000, 15000);
                cUIListBox_getEntryByData_2DB0(lb, cFEStateBraggingRights_getBEData(this, playerIndex,
                        ((signed char*)mpGameEventMapping)[row]), &out);
                cUIListBox_setCurrentEntryByIndex_2DB0(lb, out);
                break;
            }
        }
    }
    cUIObj_2DB0* onGroupWidget = (cUIObj_2DB0*)find_2DB0(this, str_ongroup);
    if (onGroupWidget != 0) {
        if (isNet_2DB0()) {
            onGroupWidget->setVisible(1);
            cUIObj_2DB0* hiddenWidget = (cUIObj_2DB0*)find_2DB0(this, str_x_continue2);
            if (hiddenWidget != 0)
                hiddenWidget->setVisible(0);
            cUIObj_2DB0* player2Widget = (cUIObj_2DB0*)find_2DB0(this, str_nml_player2);
            if (player2Widget != 0)
                player2Widget->setVisible(0);
        } else {
            onGroupWidget->setVisible(0);
        }
    }
    if (isNet_2DB0()) {
        cUIObj_2DB0* hiddenWidget = (cUIObj_2DB0*)find_2DB0(this, str_fegroup);
        if (hiddenWidget != 0)
            hiddenWidget->setVisible(0);
        cUIObj_2DB0* player2Widget = (cUIObj_2DB0*)find_2DB0(this, str_x_nml_player2);
        if (player2Widget != 0)
            player2Widget->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Does no gain-transition work for the bragging-rights state.
 *
 * Returns 0, as the base cUIState_onGainTransition() does.
 *
 * @return always 0
 */
int cFEStateBraggingRights_onGainTransition(void* self)
{
    return 0;
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the bragging-rights state's 2D layer with the shared UI-state render routine cUIState_onRender2D().
 *
 * @return no meaningful value (cUIState_onRender2D() returns nothing)
 */
void* cFEStateBraggingRights_onRender2D(void* self)
{
    return cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onUpdate);
#ifdef SKIP_ASM
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

struct cUIScreen_3338;

int GetHashValue32(char* str);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern int D_004A1A70;


extern void* cSetupMan_mSetupMan;

// PORT: several callees are declared with other parameter types elsewhere; bind local views by asm label
extern "C" void cBraggingRightsMenu_setConfirmed(void* panel, int on);

extern "C" void cBraggingRightsMenu_setEnableItem_2DB0(void* panel, void* screen) __asm__("cBraggingRightsMenu_setEnableItem");

extern "C" int cBENewPlayerInterface_getPlayerCtrlID(void* iface, int idx);
extern "C" void cUIMenu_setSelectedByIndex_3338(void* menu, int idx) __asm__("cUIMenu_setSelectedByIndex");
extern "C" void memset_3338(void* dst, int c, int n) __asm__("memset");
extern "C" void func_00265700(void* snd);
extern char str_11[];

struct sStr3_3338 { char c[3]; };
typedef sVEntry<void(void*, unsigned char)> sVESet_3338;
typedef sVEntry<int(void*)> sVEGet_3338;
typedef sVEntry<int(void*, void*)> sVEPtr_3338;

static inline int get6F8_3338(void* p) { return *(int*)((char*)p + 0x6F8); }

/**
 * Feeds each player's controller to their bragging-rights panel and handles unlocking a locked panel.
 *
 * One panel online, else two. The UI input is pointed at the player's controller (input slot 44); when input
 * slot 40 is set and the panel is unlocked, the input goes to the panel's menu (slot 15). Back (input slot 6)
 * on a locked panel unlocks it, rebuilds it, selects its first row and plays the label "11" or "21"; online
 * that is also sent to the peer (func_00265700).
 */
void cFEStateBraggingRights::onUpdate()
{
    int n = isNet_2DB0() ? 1 : 2;
    int i;
    cUIInput* obj = (cUIInput*)(mpEngine->mpInput);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    for (i = 0; i < n; i++) {
        int panelIdx;
        char id;
        if (isNet_2DB0()) {
            id = cBENewPlayerInterface_getPlayerCtrlID(iface, 0);
            panelIdx = 0;
        } else {
            id = cBENewPlayerInterface_getPlayerCtrlID(iface, i);
            panelIdx = i;
        }
        sVESet_3338* vt = (sVESet_3338*)VPTR_OF(cUIInput, obj);
        vt[44].fn(vthis(obj, vt[44]), id);
        sVEGet_3338* vt2 = (sVEGet_3338*)VPTR_OF(cUIInput, obj);
        if (vt2[40].fn(vthis(obj, vt2[40])) != 0 && get6F8_3338((char*)this + panelIdx * 0x34) == 0) {
            void* menu = *(void**)((char*)this + panelIdx * 0x34 + 0x6D0);
            sVEPtr_3338* vt3 = *(sVEPtr_3338**)((char*)menu + 8);
            vt3[15].fn(vthis(menu, vt3[15]), obj);
        } else {
            sVEGet_3338* vt4 = (sVEGet_3338*)VPTR_OF(cUIInput, obj);
            if (vt4[6].fn(vthis(obj, vt4[6])) != 0 && get6F8_3338((char*)this + panelIdx * 0x34) != 0) {
                char label[5];  // "11", "21" for the second panel
                *(struct sStr3_3338*)label = *(struct sStr3_3338*)str_11;
                memset_3338(label + 3, 0, 2);
                cBraggingRightsMenu_setConfirmed(&((&mMenuCtrl[0]))[panelIdx], 0);
                cBraggingRightsMenu_setEnableItem_2DB0(&((&mMenuCtrl[0]))[panelIdx], mpScreen);
                if (panelIdx != 0)
                    label[0] = '2';
                cUIMenu_setSelectedByIndex_3338(*(void**)((char*)this + panelIdx * 0x34 + 0x6D0), 0);
                int frame = cUIScreen_getFrameByLabel((cUIScreen*)mpScreen, GetHashValue32(label));
                if (frame != cUIScreen::kNoFrame)
                    cUIScreen_playFrame(mpScreen, frame, 1);
                if (isNet_2DB0())
                    func_00265700(cSetupMan_mSetupMan);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onGainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char str_wait_1[];
extern char str_wait_2[];
extern int D_004A1A70;
extern "C" void cFEStateNet_onGainFocus(void* self, int a1);
extern "C" void cBraggingRightsMenu_setMenuItemColor(void* p);
// cUIState_onGainFocus returns nothing (ends in a void vcall).
void cUIState_onGainFocus_v(void* self) __asm__("cUIState_onGainFocus");

static inline int isOn00193568()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}

static inline int get6F8_3568(void* p) { return *(int*)((char*)p + 0x6F8); }

/**
 * Gives the bragging-rights screen the focus: plays a "wait" label and recolours the panels.
 *
 * Online it runs the online states' handler (cFEStateNet_onGainFocus) instead. Otherwise cUIState's handler runs, "wait
 * 1" or "wait 2" plays when exactly one panel is locked (+0x6F8, +0x72C), and both panels are recoloured
 * (cBraggingRightsMenu_setMenuItemColor).
 *
 * @param pPrevState  the state that had focus before (passed on online only)
 */
extern "C" void cFEStateBraggingRights_onGainFocus(cFEStateBraggingRights* self, int pPrevState)
{
    if (isOn00193568()) {
        cFEStateNet_onGainFocus(self, pPrevState);
        return;
    }
    cUIState_onGainFocus_v(self);
    if (self->mMenuCtrl[0].mbConfirmed != 0 && self->mMenuCtrl[1].mbConfirmed == 0) {
        int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_wait_1));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
    } else if (get6F8_3568(self) == 0 && self->mMenuCtrl[1].mbConfirmed != 0) {
        int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_wait_2));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
    }
    cBraggingRightsMenu_setMenuItemColor(&self->mMenuCtrl[0]);
    cBraggingRightsMenu_setMenuItemColor(&self->mMenuCtrl[1]);
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" const char* cBECharacterInterface_getShortName(void* iface, int a1);
extern char str_1Menu[];
extern char str_2Menu[];
extern char str_Player1_title[];
extern char str_Player1_name[];
extern char str_Player2_title[];
extern char str_Player2_name[];
extern char str_player1_info[];
extern char str_player2_info[];

class cUIObj_193650 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void setFocus(int on);
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
    virtual void v24(int on);
};

static inline int Is_193650(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Binds the bragging-rights widgets as they are created: the two menus, the rider names and the rule rows.
 *
 * "1Menu" and "2Menu" become the panels' menus; "Player1/2 title" and "Player1/2 name" show the human riders'
 * short character names; "player1 info" and "player2 info" go to the panels' +0x4. Widgets named "ij" (panel
 * i 1-2, row j 1-8) become row j of panel i and are tagged with the panel index.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateBraggingRights_onWidgetCreate(cFEStateBraggingRights* self, cUIObj_193650* w)
{
    cUIThing* wp = (cUIThing*)w;
    if (Is_193650(wp->mHashName, str_1Menu)) {
        *(void**)&self->mMenuCtrl[0] = 0;
        w->setFocus(1);
        *(void**)&self->mMenuCtrl[0] = w;
        wp->mAttr |= 1;
        *(int*)(((char*)wp) + 0x90) |= 1;
    } else if (Is_193650(wp->mHashName, str_2Menu)) {
        *(void**)&self->mMenuCtrl[1] = 0;
        w->setFocus(1);
        *(void**)&self->mMenuCtrl[1] = w;
        wp->mAttr |= 1;
        *(int*)(((char*)wp) + 0x90) |= 1;
    } else if (Is_193650(wp->mHashName, str_Player1_title) || Is_193650(wp->mHashName, str_Player1_name)) {
        void* newPlayer = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* character = cBE_getInterface_Fv(cBE_getBE(), 2);
        cUIText_setAsciiString((cUIText*)w, cBECharacterInterface_getShortName(character, cBENewPlayerInterface_getHumanRider(newPlayer, 0)));
    } else if (Is_193650(wp->mHashName, str_Player2_title) || Is_193650(wp->mHashName, str_Player2_name)) {
        void* newPlayer = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* character = cBE_getInterface_Fv(cBE_getBE(), 2);
        cUIText_setAsciiString((cUIText*)w, cBECharacterInterface_getShortName(character, cBENewPlayerInterface_getHumanRider(newPlayer, 1)));
    } else if (Is_193650(wp->mHashName, str_player1_info)) {
        self->mMenuCtrl[0].mpGroup = (cUIGroup*)w;
    } else if (Is_193650(wp->mHashName, str_player2_info)) {
        self->mMenuCtrl[1].mpGroup = (cUIGroup*)w;
    } else {
        char name[3];
        name[2] = 0;
        for (int i = 0; i < 2; i++) {
            void** slots = (void**)&self->mMenuCtrl[i].mpListBox[0];
            for (int j = 0; j < 8; j++) {
                name[0] = '1' + i;
                name[1] = '1' + j;
                if (wp->mHashName == GetHashValue32(name)) {
                    slots[j] = w;
                    wp->mMiscData = i;
                    wp->mAttr &= ~1;
                    w->v24(1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern int D_004A1A70;
extern void* cSetupMan_mSetupMan;
extern "C" void cBraggingRightsMenu_setMenuItemColor(void* p);
extern "C" void cFEStateBraggingRights_updateConfirmedState(cFEStateBraggingRights* self);
extern "C" void cFEStateBraggingRights_updateBE(cFEStateBraggingRights* self, int player, int which, int value);
extern "C" int cBraggingRightsMenu_findListBoxP_38F8(void* panel, void* item) __asm__("cBraggingRightsMenu_findListBoxP");
extern "C" int cUIListBox_getCurrentEntryData_38F8(void* item) __asm__("cUIListBox_getCurrentEntryData");
extern "C" void cBraggingRightsMenu_setConfirmed_38F8(void* panel, int on) __asm__("cBraggingRightsMenu_setConfirmed");
extern "C" void func_00265688(void* snd);
extern "C" void func_00265568(void* snd, int slot, int value);
extern "C" void cUIStateStack_push(void* list, void* obj);
extern "C" void* func_001A9C50(void* self);
extern "C" void func_001A9BC0(void* self, void* popup);
void func_001C57C0(void* self, int a1, int a2);
extern "C" void cFEStateNet_onWidgetEvent(void* self, void* widget);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char str_kT_POPUPCMNCancelChallenge[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char cBERules_mRules[];

struct sVE_38F8 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sVObj_38F8 {
    int f0;
    sVE_38F8* vt;                   // 0x4
};


static inline int isOn_38F8()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}

static inline void* vcall_38F8(char* self, int slot, cUIThing* w)
{
    sVObj_38F8* o = **(sVObj_38F8***)(self + 0x10);
    return o->vt[slot].fn(vthis(o, o->vt[slot]), self, w->mMiscData);
}

/**
 * Handles the bragging-rights widgets: locking a panel, leaving, highlighting and changing a rule's value.
 *
 * kONNEXT on a row locks its panel (cBraggingRightsMenu_setConfirmed) and plays the wait labels; offline, once both
 * panels are locked, the custom interface's next state is pushed. kONPREV online asks "kT_POPUPCMNCancelChallenge"
 * (popup id kCancelChallengePopup), offline pushes the previous state and clears the rule records cBERules_mRules.
 * kONGAINFOCUS and kONLOSEFOCUS recolour the panel; kONCHANGE stores the new value with cFEStateBraggingRights_updateBE
 * (online it is sent to the peer instead).
 *
 * @param w  the widget
 * @param event  the widget event
 */
// Widget event handler of the big radio-button screen (two radio panels at self+0x6D0).
extern "C" void cFEStateBraggingRights_onWidgetEvent(cFEStateBraggingRights* self, cUIThing* w, int event)
{
    int panel;
    switch (event) {
    case cUIState::kONNEXT: {
        int i = 0;
        do {
            if (cBraggingRightsMenu_findListBoxP_38F8(&self->mMenuCtrl[i], w) != cBraggingRightsMenu::kNotFound) {
                if (isOn_38F8() && i == 0)
                    func_00265688(cSetupMan_mSetupMan);
                cBraggingRightsMenu_setConfirmed_38F8(&self->mMenuCtrl[i], 1);
                self->updateConfirmedState();
                break;
            }
            i++;
        } while (i < 2);
        if (isOn_38F8())
            break;
        if (self->mMenuCtrl[0].mbConfirmed == 0)
            break;
        if (self->mMenuCtrl[1].mbConfirmed == 0)
            break;
        void* nextState = vcall_38F8(((char*)self), 4, w);
        if (nextState != 0)
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        break;
    }
    case cUIState::kONPREV:
        if (isOn_38F8()) {
            cScreenPopup* popup = (cScreenPopup*)((char*)func_001A9C50(((char*)self)));
            char* sub = (char*)&popup->info;
            popup->info.meType = 2;
            popup->info.mtMessage.ui = GetHashValue32(str_kT_POPUPCMNCancelChallenge);
            popup->info.mtMessage.eType = 3;
            func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
            func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
            popup->info.miMiscData = cFEStateBraggingRights::kCancelChallengePopup;
            popup->info.miDefaultOption = 1;
            func_001A9BC0(((char*)self), popup);
        } else {
            void* nextState = vcall_38F8(((char*)self), 5, w);
            if (nextState != 0)
                cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
            MEM_fill(cBERules_mRules, 0, 0x20);
        }
        break;
    case cUIState::kONGAINFOCUS:
    case cUIState::kONLOSEFOCUS:
        panel = w->mMiscData;
        cBraggingRightsMenu_setMenuItemColor(&self->mMenuCtrl[panel]);
        break;
    case cUIState::kONOPTION:
        if (isOn_38F8())
            cFEStateNet_onWidgetEvent(((char*)self), w);
        break;
    case cUIState::kONCHANGE:
        if (isOn_38F8()) {
            void* snd = cSetupMan_mSetupMan;
            int slot = cBraggingRightsMenu_findListBoxP_38F8(&self->mMenuCtrl[0], w);
            func_00265568(snd, slot, cUIListBox_getCurrentEntryData_38F8(w));
        } else {
            panel = w->mMiscData;
            int slot = cBraggingRightsMenu_findListBoxP_38F8(&self->mMenuCtrl[panel], w);
            int which = ((signed char*)self->mpGameEventMapping)[slot];
            if (which != cFEStateBraggingRights::kNoRule)
                self->updateBE(panel, which, cUIListBox_getCurrentEntryData_38F8(w));
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_00193C00);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void cFEStateNetHelpPopup_setOption(void* self, int i, int a, int b);
void cFEStateNetHelpPopup_setNumHelpOptions(void* self, int val);
void cFEStateNetHelpPopup_setDescription(void* self, int i, int value);
extern char str_kT_HELP65BraggingRights[];
extern char str_kT_HELP65BraggingRightsMessage[];
extern char str_kT_HELP65BraggingRightsMessage2[];
extern char str_kT_HELPCmnToggleHighlight[];
extern char str_kT_HELPCmnChangeHighlight[];
extern char str_kT_HELP65Confirm[];
extern char str_kT_HELP65Cancel[];

/**
 * Fills the bragging-rights help popup (vtable word 51, the cFEStateNet help-popup entry).
 *
 * Sets four options, the title kT_HELP65BraggingRights (+0x54), the two description lines
 * kT_HELP65BraggingRightsMessage and Message2, and the options kT_HELPCmnToggleHighlight (button 4),
 * kT_HELPCmnChangeHighlight (3), kT_HELP65Confirm (0) and kT_HELP65Cancel (1).
 *
 * @param popup  the help popup (cFEStateNetHelpPopup)
 */
extern "C" void func_00193C00(cFEStateBraggingRights* self, cFEStateNetHelpPopup* popup)
{
    cFEStateNetHelpPopup_setNumHelpOptions(popup, 4);
    popup->muiScreenName = GetHashValue32(str_kT_HELP65BraggingRights);
    cFEStateNetHelpPopup_setDescription(popup, 0, GetHashValue32(str_kT_HELP65BraggingRightsMessage));
    cFEStateNetHelpPopup_setDescription(popup, 1, GetHashValue32(str_kT_HELP65BraggingRightsMessage2));
    cFEStateNetHelpPopup_setOption(popup, 0, GetHashValue32(str_kT_HELPCmnToggleHighlight), 4);
    cFEStateNetHelpPopup_setOption(popup, 1, GetHashValue32(str_kT_HELPCmnChangeHighlight), 3);
    cFEStateNetHelpPopup_setOption(popup, 2, GetHashValue32(str_kT_HELP65Confirm), 0);
    cFEStateNetHelpPopup_setOption(popup, 3, GetHashValue32(str_kT_HELP65Cancel), 1);
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onWidgetMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A1A70;
extern void* cSetupMan_mSetupMan;
extern "C" void cBraggingRightsMenu_setMenuItemColor(void* p);
extern "C" void func_00265458(void* snd, int id);
static inline int isOn00193CF8()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}
/**
 * Handles menu events of the two bragging-rights menus: recolouring and which events pass on.
 *
 * kONOPTION and kONMISC return 0x100. kSCROLLCHANGE recolours the menu's panel (online, panel 0's selection is also
 * sent to the peer). kONNEXT returns 1 while either panel is unlocked. kONPREVIOUS returns 0 online.
 *
 * @param ctrl  the menu (On Tour: cUIMenu *pMenu)
 * @param msg  the menu event
 * @return 0x100, 1 or 0 as above, else 0x101
 */
// Handles a menu event for one of the two radio list controls.
extern "C" int cFEStateBraggingRights_onWidgetMenu(cFEStateBraggingRights* self, cUIMenu* ctrl, int msg)
{
    int found = 0;
    void** item;
    void** menuSlot = (void**)(&self->mMenuCtrl[0]);
    if (*menuSlot == ((void*)ctrl)) {
        item = menuSlot;
        found = 1;
    } else {
        item = (void**)(&self->mMenuCtrl[1]);
        if (*item != ((void*)ctrl)) item = 0;
    }
    switch (msg) {
    case cUIMenu::kONOPTION: case cUIMenu::kONMISC:
        return 0x100;
    case cUIMenu::kSCROLLCHANGE:
        if (isOn00193CF8() && found) {
            func_00265458(cSetupMan_mSetupMan, ctrl->mScrollItemCurrent);
        }
        cBraggingRightsMenu_setMenuItemColor(item);
        return 257;
    case cUIMenu::kONNEXT:
        if (self->mMenuCtrl[0].mbConfirmed == 0 || self->mMenuCtrl[1].mbConfirmed == 0) return 1;
        return 257;
    case cUIMenu::kONPREVIOUS:
        if (isOn00193CF8()) return 0;
        break;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_updateBE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sPlayerOpts_193DE0 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int pad : 26;
    int f4;     // 0x4
    int f8;     // 0x8
};
extern sPlayerOpts_193DE0 cBERules_mRules_193DE0[] __asm__("cBERules_mRules");

static inline int other_193DE0(int p)
{
    return p == 0;
}

/**
 * Stores the value of one bragging-rights rule for a player in the rule records cBERules_mRules (0xC bytes per player).
 *
 * kBRAGRIGHTS_GIVEHEADSTART (word +0x8), kBRAGRIGHTS_GIVEFULLBOOST (bit 3), kBRAGRIGHTS_GIVEFASTRECOVERY (bit 0) and
 * kBRAGRIGHTS_STARTINHOLE (word +0x4) go into the other player's record; kBRAGRIGHTS_TAKENOUBER (bit 4),
 * kBRAGRIGHTS_TAKELEAKYAD (bit 5), kBRAGRIGHTS_TAKELV1CHAR (bit 1) and kBRAGRIGHTS_TAKEHALFPOINTS (bit 2) into the
 * player's own. Other rules are ignored. The bragging-rights widget handler
 * calls it when a choice changes, and func_001941B0 when the online peer reports one.
 *
 * @param player  the player whose panel set the rule (0 or 1)
 * @param which   the rule (eBRAGRIGHTs)
 * @param value   the chosen value
 */
void cFEStateBraggingRights::updateBE(int player, int which, int value)
{
    switch (which) {
    case kBRAGRIGHTS_GIVEHEADSTART: cBERules_mRules_193DE0[other_193DE0(player)].f8 = value; break;
    case kBRAGRIGHTS_GIVEFULLBOOST: cBERules_mRules_193DE0[other_193DE0(player)].b3 = value; break;
    case kBRAGRIGHTS_GIVEFASTRECOVERY: cBERules_mRules_193DE0[other_193DE0(player)].b0 = value; break;
    case kBRAGRIGHTS_TAKENOUBER: cBERules_mRules_193DE0[player].b4 = value; break;
    case kBRAGRIGHTS_TAKELEAKYAD: cBERules_mRules_193DE0[player].b5 = value; break;
    case kBRAGRIGHTS_TAKELV1CHAR: cBERules_mRules_193DE0[player].b1 = value; break;
    case kBRAGRIGHTS_TAKEHALFPOINTS: cBERules_mRules_193DE0[player].b2 = value; break;
    case kBRAGRIGHTS_STARTINHOLE: cBERules_mRules_193DE0[other_193DE0(player)].f4 = value; break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_getBEData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline int other_3F38(int c) { return c == 0; }
struct sRadioChan_3F38 {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned rest : 26;
    int f4;
    int f8;
};
extern sRadioChan_3F38 cBERules_mRules_a[] __asm__("cBERules_mRules");

/**
 * Returns the value of one bragging-rights rule for a player from the rule records cBERules_mRules.
 *
 * Reads the same fields cFEStateBraggingRights_updateBE writes: the GIVE rules and kBRAGRIGHTS_STARTINHOLE from the
 * other player's record, the TAKE rules from the player's own.
 *
 * @param chan  the player (0 or 1)
 * @param kind  the rule (eBRAGRIGHTs)
 * @return the rule's value, or -1 for other rules
 */
extern "C" int cFEStateBraggingRights_getBEData(void *self, int chan, int kind) {
    switch (kind) {
    case kBRAGRIGHTS_GIVEHEADSTART:
        return cBERules_mRules_a[other_3F38(chan)].f8;
    case kBRAGRIGHTS_GIVEFULLBOOST:
        return cBERules_mRules_a[other_3F38(chan)].b3;
    case kBRAGRIGHTS_GIVEFASTRECOVERY:
        return cBERules_mRules_a[other_3F38(chan)].b0;
    case kBRAGRIGHTS_TAKENOUBER:
        return cBERules_mRules_a[chan].b4;
    case kBRAGRIGHTS_TAKELEAKYAD:
        return cBERules_mRules_a[chan].b5;
    case kBRAGRIGHTS_TAKELV1CHAR:
        return cBERules_mRules_a[chan].b1;
    case kBRAGRIGHTS_TAKEHALFPOINTS:
        return cBERules_mRules_a[chan].b2;
    case kBRAGRIGHTS_STARTINHOLE:
        return cBERules_mRules_a[other_3F38(chan)].f4;
    default:
        return -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_updateConfirmedState);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern char str_wait_1[];
extern char str_wait_2[];

static inline int get6F8_4080(void* p) { return *(int*)((char*)p + 0x6F8); }
/**
 * Plays the bragging-rights screen's "wait 1" or "wait 2" label when exactly one of the two panels is locked.
 *
 * "wait 1" when the first panel's flag (+0x6F8) is set and the second's (+0x72C) clear, "wait 2" the other way round;
 * nothing when both or neither are locked.
 */
void cFEStateBraggingRights::updateConfirmedState()
{
    if (mMenuCtrl[0].mbConfirmed != 0 && mMenuCtrl[1].mbConfirmed == 0) {
        int frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_wait_1));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(mpScreen, frame, 1);
        }
    } else if (get6F8_4080(this) == 0 && mMenuCtrl[1].mbConfirmed != 0) {
        int frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_wait_2));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(mpScreen, frame, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_handleEvent);
#ifdef SKIP_ASM
extern int D_004A1A70;
extern "C" void* cFEStateNet_handleEvent(void* self, int size, int a2);

static inline int isOn00194138()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}

/**
 * Passes an event on to the online states' shared handler while the game is online.
 *
 * When D_004A1A70 is 1 it returns cFEStateNet_handleEvent(@p self, @p msgId, @p data), the slot-24 handler that every
 * cFEStateNet-derived state uses; otherwise it ignores the event and returns 0.
 *
 * @param msgId  the event id (On Tour: int msg)
 * @param data   the event data (On Tour: void *pParam)
 * @return the handler's result, or 0
 */
extern "C" void* cFEStateBraggingRights_handleEvent(cFEStateBraggingRights* self, int msgId, int data)
{
    if (isOn00194138()) {
        return cFEStateNet_handleEvent(self, msgId, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", cFEStateBraggingRights_onPopupEvent);
#ifdef SKIP_ASM
extern int D_004A1A70;
extern "C" void func_00194440(cFEStateBraggingRights* self, cUIState* a1, int a2);
extern "C" int cFEStateNet_onPopupEvent(void* self, void* a1, int a2);

static inline int isOn00194168()
{
    return D_004A1A70 == cFEStateNet::kFEPathOnline;
}

/**
 * Handles popup events online: the cancel-challenge popup (id 0x838) separately, others with cFEStateNet_onPopupEvent.
 *
 * Does nothing offline.
 *
 * @param pPopup  the popup that sent the event
 * @param Event  the popup event
 */
extern "C" void cFEStateBraggingRights_onPopupEvent(cFEStateBraggingRights* self, cUIState* pPopup, int Event)
{
    if (isOn00194168()) {
        if (pPopup->miMiscData == cFEStateBraggingRights::kCancelChallengePopup) {
            func_00194440(self, pPopup, Event);
        } else {
            cFEStateNet_onPopupEvent(self, pPopup, Event);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_001941B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: several callees are declared with other parameter types elsewhere; bind local views by asm label
extern "C" void cUIMenu_setSelectedByIndex_41B0(void* menu, int idx) __asm__("cUIMenu_setSelectedByIndex");
extern "C" int cUIListBox_getEntryByData_41B0(void* item, int key, unsigned char* out) __asm__("cUIListBox_getEntryByData");
extern "C" void cUIListBox_setCurrentEntryByIndex_41B0(void* item, int v) __asm__("cUIListBox_setCurrentEntryByIndex");
extern "C" int cBraggingRightsMenu_findListBoxP_41B0(void* panel, void* item) __asm__("cBraggingRightsMenu_findListBoxP");
extern "C" int cUIListBox_getCurrentEntryData_41B0(void* item) __asm__("cUIListBox_getCurrentEntryData");
extern "C" void cBraggingRightsMenu_setConfirmed_41B0(void* panel, int on) __asm__("cBraggingRightsMenu_setConfirmed");
extern "C" void cBraggingRightsMenu_setEnableItem_41B0(void* panel, void* screen) __asm__("cBraggingRightsMenu_setEnableItem");
extern "C" void func_001AB288_41B0(void* self) __asm__("func_001AB288");
extern "C" int func_0025DA90_41B0(void* self) __asm__("func_0025DA90");
extern "C" void* cFEStateChatRoom_cFEStateChatRoom_41B0(void* mem, int a1) __asm__("cFEStateChatRoom_cFEStateChatRoom");
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_41B0(void* mem, int a1) __asm__("cFEStateOnlineMainMenu_cFEStateOnlineMainMenu");
extern "C" void func_001A8A40_41B0(void* self, void* obj) __asm__("func_001A8A40");
extern "C" int func_001A8E40_41B0(void* self, int msg, void* data) __asm__("func_001A8E40");
extern "C" void* cMemMan_alloc_41B0(int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern char str_cFEStateChatRoom[];
extern char str_cFEStateOnlineMainMenu[];
extern char str_11[];
extern char str_21[];


struct sRadioMsg_41B0 {
    char pad0[8];
    int panel;           // 0x8
    int idx;             // 0xC
    int key;             // 0x10
};

/**
 * Handles the online messages of the bragging-rights screen (vtable word 63, cFEStateNet entry 31).
 *
 * 251 selects row @p data's idx in the given panel's menu. 252 finds the list entry whose data is the message's key in
 * the given panel's list idx, selects it and stores the value for the row's rule (+0x738 table; cFEStateBraggingRights_updateBE) unless
 * the rule is kNoRule. 253 locks the panel (cBraggingRightsMenu_setConfirmed with 1) and refreshes the wait labels
 * (cFEStateBraggingRights_updateConfirmedState). 254 unlocks it, recolours it (cBraggingRightsMenu_setEnableItem),
 * selects its row 0 and plays label "11" (panel 0) or "21". 238, 241, 248, 250, 274 and 275 call func_001AB288. 268
 * hands func_001A8A40 a new chat room (cFEStateChatRoom, 1852 bytes) when func_0025DA90(LobbyMan_mLobbyMan) is true,
 * else a new online main menu (1744 bytes). Others go to func_001A8E40.
 *
 * @param msg   the message
 * @param data  the message data (+0x8 panel, +0xC row or list index, +0x10 key)
 * @return 0, or func_001A8E40's result for other messages
 */
extern "C" int func_001941B0(cFEStateBraggingRights* self, int msg, sRadioMsg_41B0* data)
{
    switch (msg) {
    case 251:
        cUIMenu_setSelectedByIndex_41B0(((void*)(((&self->mMenuCtrl[0])))[data->panel].mpPlayerMenu), *(unsigned char*)&data->idx);
        break;
    case 252: {
        unsigned char out;
        void* item = (&(((&self->mMenuCtrl[0])))[data->panel].mpListBox[0])[data->idx];
        if (!cUIListBox_getEntryByData_41B0(item, data->key, &out)) {
            break;
        }
        cUIListBox_setCurrentEntryByIndex_41B0(item, out);
        int slot = cBraggingRightsMenu_findListBoxP_41B0(&(((&self->mMenuCtrl[0])))[data->panel], item);
        int which = ((signed char*)self->mpGameEventMapping)[slot];
        if (which == cFEStateBraggingRights::kNoRule) {
            break;
        }
        int panel = data->panel;
        self->updateBE(panel, which, cUIListBox_getCurrentEntryData_41B0(item));
        break;
    }
    case 253:
        cBraggingRightsMenu_setConfirmed_41B0(&(((&self->mMenuCtrl[0])))[data->panel], 1);
        self->updateConfirmedState();
        break;
    case 238: case 241: case 248: case 250: case 274: case 275:
        func_001AB288_41B0(((char*)self));
        break;
    case 268:
        if (func_0025DA90_41B0(LobbyMan_mLobbyMan)) {
            func_001A8A40_41B0(((char*)self), cFEStateChatRoom_cFEStateChatRoom_41B0(cMemMan_alloc_41B0(1852, str_cFEStateChatRoom, 0, 0), (int)self->mpEngine));
        } else {
            func_001A8A40_41B0(((char*)self), cFEStateOnlineMainMenu_cFEStateOnlineMainMenu_41B0(cMemMan_alloc_41B0(1744, str_cFEStateOnlineMainMenu, 0, 0), (int)self->mpEngine));
        }
        break;
    case 254: {
        cBraggingRightsMenu_setConfirmed_41B0(&(((&self->mMenuCtrl[0])))[data->panel], 0);
        cBraggingRightsMenu_setEnableItem_41B0(&(((&self->mMenuCtrl[0])))[data->panel], self->mpScreen);
        cUIMenu_setSelectedByIndex_41B0(((void*)(((&self->mMenuCtrl[0])))[data->panel].mpPlayerMenu), 0);
        int frame = data->panel == 0 ? cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_11)) : cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_21));
        if (frame == cUIScreen::kNoFrame) {
            break;
        }
        cUIScreen_playFrame(self->mpScreen, frame, 1);
        break;
    }
    default:
        return func_001A8E40_41B0(((char*)self), msg, data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", func_00194440);
#ifdef SKIP_ASM
extern void* cSetupMan_mSetupMan;
extern "C" void func_00265768(void* snd, int id);
void func_001A94D8(void* self);
extern "C" int cFEStateNet_onPopupEvent(void* self, void* a1, int a2);

/**
 * Handles an event of the bragging-rights screen's cancel-challenge popup (id kCancelChallengePopup).
 *
 * kONSCREENPOPUPOPTION0 (confirmed) cancels the online race setup (func_00265768(cSetupMan_mSetupMan, 249) when
 * cSetupMan_mSetupMan is set) and calls the empty func_001A94D8; other events go to cFEStateNet_onPopupEvent.
 * cFEStateBraggingRights_onPopupEvent calls it online for that popup.
 *
 * @param popup  the popup
 * @param popupEvent  the popup event
 */
extern "C" void func_00194440(cFEStateBraggingRights* self, cUIState* popup, int popupEvent)
{
    if (popupEvent == cUIState::kONSCREENPOPUPOPTION0) {
        if (cSetupMan_mSetupMan != 0) {
            func_00265768(cSetupMan_mSetupMan, 249);
        }
        func_001A94D8(self);
    } else {
        cFEStateNet_onPopupEvent(self, popup, popupEvent);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebigradio", DisableTextWidget);
#ifdef SKIP_ASM



/**
 * Greys out a text widget.
 *
 * Calls the widget's virtual slot 8 with 1, then caps the first component of its colour (+0x1C) at 0.5 and
 * applies it with virtual slot 11 when it was brighter.
 *
 * @param self  the widget (not a method: a free function)
 */
extern "C" void DisableTextWidget(cUIThing* self)
{
    self->setStatic(1);
    tARGB colour = self->mColour;
    if (colour.A > 0.5f) {
        colour.A = 0.5f;
        self->setColour(&colour);
    }
}
#endif

