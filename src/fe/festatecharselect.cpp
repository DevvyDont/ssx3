#include "common.h"
#include "be/beintplayer.h"
#include "ui/uimenu.h"
#include "ui/uiicon.h"
#include "vtable.h"
#include "bx/math3d.h"
#include "fe/ferider.h"
#include "be/berewarddb.h"
#include "fe/fereal.h"
#include "fe/festatecharsetup.h"
#include "ui/uistate.h"
#include "ui/uiinput.h"
#include "ui/uithing.h"
#include "be/beintnewrace.h"
#include "ui/uiengine.h"
#include "fe/festatecharselect.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_08sel_char[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cSimpleCamera_setupCamWithPosAndTarget(void* cam, t4Vector* a, t4Vector* b);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(void* self, int a1);

/**
 * Creates the character-select screen "08sel_char" and sets up the front-end camera and rider view.
 *
 * The camera (cFE::mCamera) gets (0, 200, 0) and the origin, probably position and target (cSimpleCamera_setupCamWithPosAndTarget),
 * and a field of view of 0.436 (25 degrees). Calls cFERider_setVisible with 0 on the user's front-end rider, sets
 * +0x80 to 1, +0x5C to -1 and +0x58 and +0x60 to 0.
 */
extern "C" void cFEStateCharSelect_onCreateScreen(cFEStateCharSelect* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_08sel_char), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFE* cam = SSXApp->RealFE;
    t4Vector pos;
    pos.x = 0.0f;
    pos.y = 200.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
    t4Vector lookAt;
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 0.0f;
    lookAt.w = 1.0f;
    cSimpleCamera_setupCamWithPosAndTarget(&cam->mCamera, &pos, &lookAt);
    cam->mCamera.View = 0.4363323450088501f;
    cFERider_setVisible(cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum), 0);
    self->mbDraw3D = 1;
    self->mPan = -1.0f;
    *(int*)&self->mZoom = 0;
    *(int*)&self->mPlayerRotation = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(void* self, int a1);
void* cUIState_onDestroyScreen(void* self);

/**
 * Destroys the character-select screen; off game path 0 it first resets the user's front-end rider.
 *
 * The rider (cFE::mRiderManager slot for mUserNum) gets cFERider_setVisible with 0, then cUIState_onDestroyScreen()
 * runs.
 */
extern "C" void cFEStateCharSelect_onDestroyScreen(cFEStateCharSelect* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* mgr = (char*)SSXApp->RealFE;
    if (mgr != 0 && cBENewRaceInterface_newRace.cGamePath != 0) {
        cFERider_setVisible(cFERiderManager_getHuman1(mgr + 0xB0, self->mUserNum), 0);
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onGainTransition);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void cFEStateCharSelect_updateWidget(void* self, int index, signed char charID);

/**
 * Shows the stats of the player's current character when the character-select gain transition starts.
 *
 * When @p on is nonzero, reads the character id for mUserNum from the new-player interface and passes both to
 * cFEStateCharSelect_updateWidget(), which fills the seven stat texts and bars and the overall average.
 *
 * @param on  the state's first-time flag as passed by the state stack; nonzero on the transition's first call
 * @return always 0
 */
extern "C" int cFEStateCharSelect_onGainTransition(cFEStateCharSelect* self, int on)
{
    if (on != 0) {
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
        cFEStateCharSelect_updateWidget(self, self->mUserNum, id);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onGainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int idx, int a2, int a3);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFEStateCharSelect_updateCharacter3DPos(cFEStateCharSelect* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);
extern signed char D_00440F68[];
extern char str_Menu[];

/**
 * Gives the character-select screen the focus: selects the player's character in "Menu" and shows the rider.
 *
 * Finds the player's character among the 10 selectable ones (D_00440F68) and selects its entry. The front-end
 * rider is then set to D_00440F68[0] (not the player's character), its +0xCCC cleared, and cFEStateCharSelect_updateCharacter3DPos
 * places it before cUIState_onGainFocus() runs.
 *
 * @param pPrevState  the state that had focus before, passed on
 */
extern "C" void cFEStateCharSelect_onGainFocus(cFEStateCharSelect* self, int pPrevState)
{
    int i = 0;
    int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
    for (; i < 10; i++) {
        if (D_00440F68[i] == id) {
            void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu));
            if (menu != 0)
                cUIMenu_setSelectedByIndex(menu, i);
            break;
        }
    }
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, self->mUserNum, D_00440F68[0], 0);
    cFERider* feRider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum);
    feRider->mAnimateBoard = 0;
    cFEStateCharSelect_updateCharacter3DPos(self);
    cUIState_onGainFocus(self, pPrevState);
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the character-select state's 2D layer with the shared UI-state render routine cUIState_onRender2D().
 *
 * @return no meaningful value (cUIState_onRender2D() returns nothing)
 */
void* cFEStateCharSelect_onRender2D(void* self)
{
    return cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onUpdate);
#ifdef SKIP_ASM
extern "C" void cFEStateCharSelect_updateCharacter3DPos(cFEStateCharSelect* self);
extern "C" void* cUIState_onUpdate(void* self);

/**
 * Updates the character-select state: poses the front-end rider model, then runs the base UI-state update.
 *
 * cFEStateCharSelect_updateCharacter3DPos() places and turns the rider model once it is loaded and restarts its animation when the state
 * asks for it. cUIState_onUpdate() is the base UI-state update, which dispatches controller input to the state and
 * its screens.
 */
extern "C" void cFEStateCharSelect_onUpdate(cFEStateCharSelect* self)
{
    cFEStateCharSelect_updateCharacter3DPos(self);
    cUIState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onInputBegin);
#ifdef SKIP_ASM
struct sPadVtI_1450 { short delta; short index; int (*fn)(void*); };
struct sPadVtF_1450 { short delta; short index; float (*fn)(void*); };

static inline int padBtn_1450(char* pad, int slot)
{
    sPadVtI_1450* vt = *(sPadVtI_1450**)(pad + 8);
    return vt[slot].fn(vthis(pad, vt[slot]));
}

static inline float padAxis_1450(char* pad, int slot)
{
    sPadVtF_1450* vt = *(sPadVtF_1450**)(pad + 8);
    return vt[slot].fn(vthis(pad, vt[slot]));
}

static inline float wrapHi_1450(float a)
{
    while (a >= 360.0f)
        a -= 360.0f;
    return a;
}

static inline float wrapLo_1450(float a)
{
    while (a < 0.0f)
        a += 360.0f;
    return a;
}

// PORT: g++ `<?` (min) operator.
static inline float clamp_1450(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

/**
 * Lets the player turn and adjust the character model on the select screen with the analogue inputs.
 *
 * While input 35 is held, axis 38 times 3 turns the model (+0x60, degrees, wrapped to 0-360). Inputs 33 and
 * 34 change +0x58 (clamped to 0-1) and +0x5C (clamped to -1-1) by their axes (36, 37) times -0.1.
 *
 * @param pInput  the input object
 * @return 1 when any of the values changed, else 0
 */
extern "C" int cFEStateCharSelect_onInputBegin(cFEStateCharSelect* self, cUIInput* pInput)
{
    int changed = 0;
    if (padBtn_1450(((char*)pInput), 35)) {
        self->mPlayerRotation += padAxis_1450(((char*)pInput), 38) * 3.0f;
        if (self->mPlayerRotation >= 360.0f) {
            float angle = self->mPlayerRotation;
            do
                angle -= 360.0f;
            while (angle >= 360.0f);
            self->mPlayerRotation = angle;
        }
        float angle = self->mPlayerRotation;
        float minAngle = 0.0f;
        if (angle < minAngle) {
            float wrapped;
            do {
                wrapped = angle + 360.0f;
                angle = wrapped;
            } while (wrapped < minAngle);
            self->mPlayerRotation = wrapped;
        }
        changed = 1;
    }
    if (padBtn_1450(((char*)pInput), 33)) {
        self->mZoom = clamp_1450(self->mZoom + padAxis_1450(((char*)pInput), 36) * -0.10000000149011612f, 0.0f, 1.0f);
        changed = 1;
    }
    if (padBtn_1450(((char*)pInput), 34)) {
        self->mPan = clamp_1450(self->mPan + padAxis_1450(((char*)pInput), 37) * -0.10000000149011612f, -1.0f, 1.0f);
        changed = 1;
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* cBECharacterInterface_getShortNameByCharacter(void* self, int player, int index);
extern signed char D_00440F68[];
extern char str_names_menu[];
extern char str_titlesel_character[];
extern char str_titlesel_character_multi[];
extern char str_kT_TITLEPlayer1SelectCharacter[];
extern char str_kT_TITLEPlayer2SelectCharacter[];
struct sCharKey_1620 { char c[2]; };
extern sCharKey_1620 D_004A13A0[];

struct sWVt_1620 { short delta; short index; void (*fn)(void*, int); };
struct sWidget_1620 {
    int f0;
    int f4;
    sWVt_1620* vt;              // 0x8
    int fC;
    int f10;
    unsigned b0 : 1;            // 0x14
    unsigned b1 : 6;
    unsigned b7 : 1;
    unsigned b8 : 24;
    int id18;                   // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;                   // 0x38
};

static inline void refresh_1620(sWidget_1620* w)
{
    w->vt[9].fn(vthis(w, w->vt[9]), 0);
}

/**
 * Sets up the character-select widgets: the name menu, the titles and the ten character names.
 *
 * "names menu" gets flag bit 0 cleared and bit 7 set. On game path 2 "titlesel_character" is hidden and
 * "titlesel_character_multi" shows the player 1 or 2 title (by mUserNum); on other paths that one is hidden.
 * Texts "0" to "9" get the selectable characters' names, their index as id, and start hidden.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateCharSelect_onWidgetCreate(cFEStateCharSelect* self, sWidget_1620* w)
{
    int hashName = w->hash;
    if (hashName == GetHashValue32(str_names_menu)) {
        w->b0 = 0;
        w->b7 = 1;
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (hashName == GetHashValue32(str_titlesel_character) && cBENewRaceInterface_newRace.cGamePath == 2) {
        refresh_1620(w);
        return;
    }
    if (hashName == GetHashValue32(str_titlesel_character_multi)) {
        if (cBENewRaceInterface_newRace.cGamePath == 2) {
            switch (self->mUserNum) {
            case 0:
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLEPlayer1SelectCharacter));
                break;
            case 1:
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLEPlayer2SelectCharacter));
                break;
            }
            return;
        }
        refresh_1620(w);
        return;
    }
    sCharKey_1620 key = D_004A13A0[0];
    for (int i = 0; i < 10; i++) {
        key.c[0] = '0' + i;
        if (hashName == GetHashValue32(key.c)) {
            cUIText_setAsciiString((cUIText*)w, (const char*)cBECharacterInterface_getShortNameByCharacter(cBE_getInterface_Fv(cBE_getBE(), 2), D_00440F68[i], 0));
            w->id18 = i;
            refresh_1620(w);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void* func_001887A0(void* mem, void* owner, int a2);
extern "C" void* cFEStateProfileLoad_cFEStateProfileLoad(void* mem, void* owner, int a2, unsigned char a3);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(void* self, int a1);
extern "C" void cFECustom_setPlayerCtrlMask(void* self, int rider, unsigned char mask);
extern "C" void cFEStateCharSelect_updateWidget(void* self, int index, signed char charID);
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(void* iface, int rider, int idx);
extern "C" void cBENewPlayerInterface_setPlayerCharID(void* iface, int rider, int id);
extern "C" void cBENewPlayerInterface_setPlayerCheatCharID(void* iface, int rider, int id);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern signed char D_00440F68[];
extern char str_FEOptions[];
extern char str_cFEStateProfileLoad[];

class cWidgetBase_17E8 {
public:
    int pad0[2];
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

class cWidget_17E8 : public cWidgetBase_17E8 {
public:
    char padC[0xC];
    int id;                         // 0x18
};
typedef cWidget_17E8 sObj_17E8;

class cIface_17E8 {
public:
    int pad0[3];
    virtual void commit();
};
typedef cIface_17E8 sIface_17E8;

class cStateFactory_17E8 {
public:
    int pad0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* onSelect(void* owner, int id);
    virtual void* onBack(void* owner, int id);
};


static inline char* riderSlot_17E8(cFEStateCharSelect* self)
{
    return (char*)cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum);
}

static inline void refresh_17E8(sIface_17E8* iface)
{
    iface->commit();
}

/**
 * Handles character-select events: picking or highlighting a character, back, options and profile load.
 *
 * Event 5 sets +0xC20 (clearing +0xC1C) on the user's front-end rider, stores the controller, the character
 * and +0x4C in the player table, commits it, records the controller mask (cFECustom_setPlayerCtrlMask) and pushes the
 * custom interface's next state. Event 1 shows the highlighted name and its stats (cFEStateCharSelect_updateWidget) and stores
 * the choice; 2 hides the name. 6 pushes the previous state; 7 opens "FEOptions" (state bits 0x100); 8 opens
 * "cFEStateProfileLoad" for this controller.
 *
 * @param widget  the widget (ignored when 0); its id (+0x18) is the character index
 * @param event  the widget event
 */
// Character select: widget event handler.
extern "C" void cFEStateCharSelect_onWidgetEvent(cFEStateCharSelect* self, sObj_17E8* widget, int event)
{
    if (widget == 0)
        return;
    switch (event) {
    case 5: {
        int on = 1;
        cFERider* feRider = (cFERider*)riderSlot_17E8((self));
        feRider->mSelected = on;
        feRider->mCustomized = 0;
        int rider = self->mUserNum;
        int player = self->mCurrentCtrl;
        int ch = D_00440F68[widget->id];
        int board = self->mCheatCharID;
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(iface, rider, player);
        cBENewPlayerInterface_setPlayerCharID(iface, rider, ch);
        cBENewPlayerInterface_setPlayerCheatCharID(iface, rider, board);
        refresh_17E8(iface);
        unsigned char mask = on << self->mCurrentCtrl;
        cFECustom_setPlayerCtrlMask(*(void**)(char*)self->mpEngine, self->mUserNum, mask);
        cUIState* st = (cUIState*)(*(cStateFactory_17E8**)(char*)self->mpEngine)->onSelect((self), widget->id);
        if (((void*)st) != 0)
            cUIStateStack_push(&self->mpEngine->mStateStack, st);
        cFERider_setVisible(riderSlot_17E8((self)), 0);
        self->mbDraw3D = 0;
        break;
    }
    case 6: {
        cUIState* st = (cUIState*)(*(cStateFactory_17E8**)(char*)self->mpEngine)->onBack((self), widget->id);
        if (((void*)st) != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, st);
            cFERider_setVisible(riderSlot_17E8((self)), 0);
            self->mbDraw3D = 0;
        }
        break;
    }
    case 1: {
        widget->setVisible(1);
        cFEStateCharSelect_updateWidget((self), self->mUserNum, D_00440F68[widget->id]);
        int rider = self->mUserNum;
        int player = self->mCurrentCtrl;
        int ch = D_00440F68[widget->id];
        int board = self->mCheatCharID;
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(iface, rider, player);
        cBENewPlayerInterface_setPlayerCharID(iface, rider, ch);
        cBENewPlayerInterface_setPlayerCheatCharID(iface, rider, board);
        refresh_17E8(iface);
        break;
    }
    case 2:
        widget->setVisible(0);
        break;
    case 3:
    case 4:
        break;
    case 7: {
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(iface, self->mUserNum, self->mCurrentCtrl);
        refresh_17E8(iface);
        cUIState* st = (cUIState*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), (char*)self->mpEngine, 1);
        st->mAttrState = (st->mAttrState & ~0x3F00) | 0x100;
        cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, st, 0, 0);
        cFERider_setVisible(riderSlot_17E8((self)), 0);
        self->mbDraw3D = 0;
        break;
    }
    case 8: {
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerCtrlID(iface, self->mUserNum, self->mCurrentCtrl);
        refresh_17E8(iface);
        cUIState* st = (cUIState*)cFEStateProfileLoad_cFEStateProfileLoad(cMemMan_alloc(640, str_cFEStateProfileLoad, 0x100, 0), (char*)self->mpEngine, 2, 1 << self->mCurrentCtrl);
        cUIStateStack_push(&self->mpEngine->mStateStack, st);
        char* top = *(char**)(char*)self->mpEngine;
        if (top != 0)
            *(int*)(top + 0xC) = 9;
        cFERider_setVisible(riderSlot_17E8((self)), 0);
        self->mbDraw3D = 0;
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_updateWidget);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int cBEStatInterface_getHumanSpeed(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanAccel(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanTrick(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanEdging(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanSpin(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanToughness(void* iface, int a, int b);
extern "C" int cBEStatInterface_getHumanStability(void* iface, int a, int b);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int idx, int a2, int a3);
extern "C" void cUIProgress_setValue(void* bar, int v);
extern "C" double fptodp(float f);
extern "C" void* memset(void* dst, int c, int n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_1f;
extern char str_NumAttrOver[];
extern char str_overall[];
struct sStr9_181BD0 { char c[9]; };
struct sStr4_181BD0 { char c[4]; };
extern sStr9_181BD0 D_0045D6E0;
extern char str_0pb[];

class cFEVObj_181BD0 {
public:
    char pad[0x8];
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

// PORT: the unit declares cFEStateCharSelect_updateWidget with an int index; the body sign-extends both id arguments.
extern "C" void cFEStateCharSelect_updateWidget_impl(void* self, signed char rider, signed char charID) __asm__("cFEStateCharSelect_updateWidget");

/**
 * Fills the character-select stats widgets for a character and puts it on the user's front-end rider.
 *
 * Reads the 7 attribute ratings from the player interface (cBE_getInterface kind 3: cBEStatInterface_getHumanAccel, cBEStatInterface_getHumanEdging,
 * cBEStatInterface_getHumanSpeed, cBEStatInterface_getHumanSpin, cBEStatInterface_getHumanStability, cBEStatInterface_getHumanToughness, cBEStatInterface_getHumanTrick). For attribute i it finds the text
 * "NumAttr<i>" and the bar "<i>pb" on the screen (+0x40); when both exist it shows the text with rating * 0.2 ("%.1f"),
 * sets the bar's style (+0x80) to 55 and its value to the rating (at least 0). The total, rounded down to a multiple of
 * 5, gives "NumAttrOver" (total / 35, "%.1f") and the "overall" bar (total / 7); neither of these two lookups is
 * null-checked. Finally sets the user's rider to the character (cFERiderManager_setHumanCharacter on the front end's rider manager at
 * +0xB0, flag 0).
 *
 * @param rider  the user (controller) index
 * @param charID  the character id
 */
extern "C" void cFEStateCharSelect_updateWidget_impl(void* vself, signed char rider, signed char charID)
{
    cFEStateCharSelect* self = (cFEStateCharSelect*)vself;
    sStr9_181BD0 name = D_0045D6E0;
    char bar[8];
    *(sStr4_181BD0*)bar = *(sStr4_181BD0*)str_0pb;
    memset(bar + 4, 0, 4);
    char buf[100];
    int stats[7];
    float k = 0.2f;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    stats[0] = cBEStatInterface_getHumanAccel(iface, rider, charID);
    stats[1] = cBEStatInterface_getHumanEdging(iface, rider, charID);
    stats[2] = cBEStatInterface_getHumanSpeed(iface, rider, charID);
    stats[3] = cBEStatInterface_getHumanSpin(iface, rider, charID);
    stats[4] = cBEStatInterface_getHumanStability(iface, rider, charID);
    stats[5] = cBEStatInterface_getHumanToughness(iface, rider, charID);
    stats[6] = cBEStatInterface_getHumanTrick(iface, rider, charID);
    int total = 0;
    for (int i = 0; i < 7; i++) {
        name.c[7] = '0' + i;
        cFEVObj_181BD0* statText = (cFEVObj_181BD0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name.c));
        bar[0] = '0' + i;
        char* statBar = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(bar));
        if (statText != 0 && statBar != 0) {
            int statValue = stats[i];
            // PORT: g++ minimum/maximum operator
            int barValue = statValue >? 0;
            total += statValue;
            sprintf(buf, &str_pct_1f, fptodp((float)statValue * k));
            statText->setVisible(1);
            cUIText_setAsciiString((cUIText*)statText, buf);
            *(int*)(statBar + 0x80) = 55;
            cUIProgress_setValue(statBar, barValue);
        }
    }
    total -= total % 5;
    char* ovr = str_overall;
    cUIText* avg = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_NumAttrOver));
    sprintf(buf, &str_pct_1f, fptodp((float)total * 0.02857142873108387f));
    cUIText_setAsciiString(avg, buf);
    char* ob = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(ovr));
    *(int*)(ob + 0x80) = 55;
    cUIProgress_setValue(ob, total / 7);
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, rider, charID, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_updateCharacter3DPos);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);

struct sQuat_181EF0
{
    float x, y, z, w;
    sQuat_181EF0() {}
    // Assigns w first, like the w-first quaternion ctors elsewhere (CHEATSHEET "Quaternions").
    sQuat_181EF0(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));


extern sQuat_181EF0 t4vect_XAxis;
extern sQuat_181EF0 t4vect_YAxis;
extern sQuat_181EF0 t4vect_ZAxis;
extern "C" void cFERider_setPose(void* rider, tPose* xf);
extern "C" void cFERider_setBoardPose(void* rider, tPose* xf);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_181EF0 QuatMul_181EF0(const sQuat_181EF0& a, const sQuat_181EF0& b)
{
    sQuat_181EF0 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sQuat_181EF0 AxisAngle_181EF0(const sQuat_181EF0& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_181EF0(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Places the user's front-end rider on the character-select screen and shows it once its model is loaded.
 *
 * Gets the rider for the user (+0x44) from the rider manager (cFERiderManager_getHuman1). When it has a valid character loaded (id
 * below 10, +0xCB8 and +0xCB4 set) and the state's +0x80 flag is set: sets the rider pose (cFERider_setPose) to position
 * (136, -250, -82) turned about Z by the model angle (+0x60, degrees) + 100; when +0x54 is set, resets the rider
 * animation (+0xC) and plays animation 434, clears the rider's +0xCCC and +0x54; moves the board out of view to (0,
 * 10000, 0) (cFERider_setBoardPose) and makes the rider visible (cFERider_setVisible).
 *
 * @param vself  the character-select state
 */
extern "C" void cFEStateCharSelect_updateCharacter3DPos(cFEStateCharSelect* vself)
{
    cFERider* rider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, vself->mUserNum);
    int ok = 0;
    if (*(unsigned int*)rider < 10 && rider->mDoneLoadingTextures != 0) {
        ok = rider->mCompiledModel != 0;
    }
    if (ok && vself->mbDraw3D != 0) {
        sQuat_181EF0 pos;
        tPose xf;
        float ang = vself->mPlayerRotation;
        {
            sQuat_181EF0 position;
            position.x = 136.0f;
            position.y = -250.0f;
            position.z = -82.0f;
            position.w = 1.0f;
            pos = position;
            (*(sQuat_181EF0*)&xf.P) = position;
            (*(sQuat_181EF0*)&xf.Q).x = 0.0f;
            (*(sQuat_181EF0*)&xf.Q).y = 0.0f;
            (*(sQuat_181EF0*)&xf.Q).z = 0.0f;
            (*(sQuat_181EF0*)&xf.Q).w = 1.0f;
            (*(sQuat_181EF0*)&xf.Q) = QuatMul_181EF0((*(sQuat_181EF0*)&xf.Q), AxisAngle_181EF0(t4vect_XAxis, 0.0f));
        }
        (*(sQuat_181EF0*)&xf.Q) = QuatMul_181EF0((*(sQuat_181EF0*)&xf.Q), AxisAngle_181EF0(t4vect_YAxis, 0.0f));
        ang += 100.0f;
        ang *= 0.01745329424738884f;
        (*(sQuat_181EF0*)&xf.Q) = QuatMul_181EF0((*(sQuat_181EF0*)&xf.Q), AxisAngle_181EF0(t4vect_ZAxis, ang));
        cFERider_setPose(rider, &xf);
        if (vself->mbIsFirst != 0) {
            void* anim = rider->mpRiderAnim;
            if (anim != 0) {
                cRiderAnimBase_reset(anim);
                cRiderAnimBase_play(anim, 434, 0, -1.0f);
            }
            rider->mAnimateBoard = 0;
            vself->mbIsFirst = 0;
        }
        {
            sQuat_181EF0 position;
            position.x = 0.0f;
            position.y = 10000.0f;
            position.z = 0.0f;
            position.w = 1.0f;
            (*(sQuat_181EF0*)&xf.P) = position;
        }
        cFERider_setBoardPose(rider, &xf);
        cFERider_setVisible(rider, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_cFEStateCheatCharSelect);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBECharacterInterface_getShortNameByCharacter(void* self, int player, int index);
extern "C" int cBERewardInterface_isCheatCharOwned(void* iface, int a1, int a2, int a3);
extern "C" cBERewardDB* cBERewardInterface_getDB();
extern "C" int cUITextureBank_getElementByHashname(void* self, int id);
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern void* cFEStateCheatCharSelect_vtbl[];
extern char* D_00440F78[];

struct sCharSel_2220 {
    char pad0[0x8];
    void** vt;                  // 0x8
    int fC;                     // 0xC
    char* owner;                // 0x10
    char f14;
    unsigned char f15;          // 0x15
    char pad16[0x44 - 0x16];
    signed char player;         // 0x44
    char pad45[0x54 - 0x45];
    int sel[6];                 // 0x54
    int f6C;
    int f70;                    // 0x70
    int f74;                    // 0x74
    int count;                  // 0x78
    int ids[30];                // 0x7C
    int hashes[30];             // 0xF4
    char* names[30];            // 0x16C
};

/**
 * Constructs the cheat-character select state for a user and a base character.
 *
 * Builds the UI state (cUIState_cUIState1), sets state id 10, the cFEStateCheatCharSelect_vtbl vtable and the user. Entry 0
 * is the base character itself: id 0, its face picture (D_00440F78[@p slot], looked up in the owner's texture bank at
 * +0x58) and its name (cBECharacterInterface_getShortNameByCharacter on interface 2). Then every reward-database character (cBERewardInterface_getDB)
 * that cBERewardInterface_isCheatCharOwned reports as owned by the user for @p slot is appended with its id, picture and name (count +0x78;
 * ids +0x7C, pictures +0xF4, names +0x16C, 30 entries). Clears +0x70, the six picture slots (+0x54) and +0x74, and
 * stores cFECustom_getPlayerCtrlMask's value for the user at +0x15.
 *
 * @param engine  the UI engine
 * @param owner  the parent state
 * @param player  the user (controller) index
 * @param slot  the base character id
 * @return @p self
 */
extern "C" sCharSel_2220* cFEStateCheatCharSelect_cFEStateCheatCharSelect(sCharSel_2220* self, cUIEngine* engine, cUIState* owner, signed char player, int slot)
{
    cUIState_cUIState1(self, engine, owner);
    self->fC = 10;
    self->vt = cFEStateCheatCharSelect_vtbl;
    self->player = player;
    self->count = 1;
    self->f70 = 0;
    self->ids[0] = 0;
    char* ownerState = self->owner;
    int faceHash = GetHashValue32(D_00440F78[slot]);
    self->hashes[0] = cUITextureBank_getElementByHashname(ownerState + 0x58, faceHash);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    cBERewardDB* list = cBERewardInterface_getDB();
    int n = list->mNumCheatChars;
    for (int i = 0; i < n; i++) {
        tCheatCharInfo* charEntry = &(list->mpCheatChars)[i];
        if (cBERewardInterface_isCheatCharOwned(iface, self->player, slot, charEntry->mCheatCharID)) {
            char* lbl = charEntry->mpIconName;
            self->ids[self->count] = charEntry->mCheatCharID;
            char* entryOwner = self->owner;
            int labelHash = GetHashValue32(lbl);
            self->hashes[self->count] = cUITextureBank_getElementByHashname(entryOwner + 0x58, labelHash);
            self->names[self->count] = charEntry->mNameStringID;
            self->count++;
        }
    }
    self->names[0] = (char*)cBECharacterInterface_getShortNameByCharacter(cBE_getInterface_Fv(cBE_getBE(), 2), slot, 0);
    for (int j = 5; j >= 0; j--)
        self->sel[j] = 0;
    self->f74 = 0;
    self->f15 = cFECustom_getPlayerCtrlMask(*(void**)self->owner, self->player);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_131cheat_char[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the cheat-character select screen "131cheat_char" and plays its first frame.
 */
extern "C" void cFEStateCheatCharSelect_onCreateScreen(cFEStateCheatCharSelect* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_131cheat_char), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetCreate);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char str_6pics[];
extern char str_arrowleft[];
extern char str_arrowright[];
extern char str_nametext[];
extern char D_004A1408[];
extern char* D_00440FA0[];
extern char gBigSize[];
extern "C" void cUIMenu_setScrollableMenu(void* w, unsigned char n);

struct sCWVt_2460 { short delta; short index; void (*fn)(void*, int); };
struct sCW_2460 {
    int f0;
    int f4;
    sCWVt_2460* vt;             // 0x8
    int fC;
    int f10;
    unsigned b0 : 1;            // 0x14
    unsigned b1 : 31;
    int id18;                   // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;                   // 0x38
    char pad3C[0x78 - 0x3C];
    int f78;                    // 0x78
    int f7C;                    // 0x7C
};

static inline void vcall_2460(sCW_2460* w, int k, int v)
{
    w->vt[k].fn(vthis(w, w->vt[k]), v);
}

/**
 * Binds the cheat-character select widgets: picture menu, arrows, name text and picture slots.
 *
 * "6pics" is scrollable when there are 6 or more entries; "arrowleft" and "arrowright" show from 7 entries;
 * "nametext" starts empty. The six picture slots ("pic1" ...) take the first entries' values; slots past the
 * entry count are hidden and disabled, and the first slot gets the highlight colour.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateCheatCharSelect_onWidgetCreate(cFEStateCheatCharSelect* self, sCW_2460* w)
{
    int hashName = w->hash;
    if (hashName == GetHashValue32(str_6pics)) {
        *(sCW_2460**)&self->mpMenu = w;
        w->id18 = 0;
        ((sCW_2460*)self->mpMenu)->b0 = 0;
        if (self->mNumCharacters >= 6) cUIMenu_setScrollableMenu(((sCW_2460*)self->mpMenu), self->mNumCharacters);
        return;
    }
    if (hashName == GetHashValue32(str_arrowleft)) {
        *(sCW_2460**)&self->mpArrowPrev = w;
        vcall_2460(w, 9, self->mNumCharacters >= 7);
        return;
    }
    if (hashName == GetHashValue32(str_arrowright)) {
        *(sCW_2460**)&self->mpArrowNext = w;
        vcall_2460(w, 9, self->mNumCharacters >= 7);
        return;
    }
    if (hashName == GetHashValue32(str_nametext)) {
        *(sCW_2460**)&self->mpNameWidget = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
        return;
    }
    int i = 0;
    char** tb = D_00440FA0;
    sCW_2460** sp = ((sCW_2460**)&self->mpCharWidget[0]);
    for (; i < 6; sp++, tb++, i++) {
        if (hashName == GetHashValue32(*tb)) {
            *sp = w;
            w->id18 = i;
            if (i == 0) {
                sVEntry<void(void*, void*)>* vtEntry = &((sVEntry<void(void*, void*)>*)((sCW_2460*)self->mpCharWidget[0])->vt)[21];
                vtEntry->fn((char*)self->mpCharWidget[0] + vtEntry->delta, gBigSize);
            }
            if (i >= self->mNumCharacters) {
                vcall_2460(*sp, 9, 0);
                vcall_2460(*sp, 8, 1);
            } else {
                sCW_2460* sl = *sp;
                sl->f7C = (&((int*)&self->mpCharElem[0])[0])[i];
                sl->f78 = -1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetEvent);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setPlayerCheatCharID(void* iface, int rider, int id);
extern "C" void cUIStateStack_pop(void*, int);
extern char gSmallSize[];
extern char gBigSize[];


/**
 * Handles the cheat-character pictures: highlight, the shown name, picking one and backing out.
 *
 * Event 1 highlights the picture and shows its entry's name, 2 restores its colour. Event 5 stores the
 * entry's value (0 for the first entry) in +0x70 and in the player table (cBENewPlayerInterface_setPlayerCheatCharID), commits it, sets
 * +0x74 and closes the popup; 6 closes it with +0x70 = 0.
 *
 * @param menu  the picture widget (ignored when 0); its id plus the menu's scroll offset is the entry
 * @param msg  the widget event
 */
extern "C" void cFEStateCheatCharSelect_onWidgetEvent(cFEStateCheatCharSelect* self, cUIThing* menu, unsigned int msg)
{
    if (((void*)menu) == 0)
        return;
    switch (msg) {
    case 2: {
        sVEntry<void(void*, void*)>* vt = (sVEntry<void(void*, void*)>*)menu->mVtbl;
        vt[21].fn(vthis(menu, vt[21]), gSmallSize);
        break;
    }
    case 1: {
        int sel = menu->mMiscData + self->mpMenu->mScrollItemOffset;
        sVEntry<void(void*, void*)>* vt = (sVEntry<void(void*, void*)>*)menu->mVtbl;
        vt[21].fn(vthis(menu, vt[21]), gBigSize);
        cUIText_setAsciiString(self->mpNameWidget, self->mpCharNames[sel]);
        break;
    }
    case 6:
        self->mCheatCharID = 0;
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
        break;
    case 5: {
        int sel = menu->mMiscData + self->mpMenu->mScrollItemOffset;
        if (sel == 0)
            self->mCheatCharID = 0;
        else
            self->mCheatCharID = self->mCharIDs[sel];
        cBENewPlayerInterface* pl = (cBENewPlayerInterface*)((char*)cBE_getInterface_Fv(cBE_getBE(), 1));
        cBENewPlayerInterface_setPlayerCheatCharID(pl, self->mUserNum, self->mCheatCharID);
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)pl->mVtbl;
        vt[1].fn(vthis(pl, vt[1]));
        self->mDone = 1;
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void cFEStateCheatCharSelect_updateCharWidgets(cFEStateCheatCharSelect* self);

/**
 * Handles the cheat-character menu's events: entry checks, picture refresh and which events pass on.
 *
 * @param pMenu  the menu (unused)
 * @param msg  the menu event
 * @param value  the entry index for event 1
 * @return 0x100 for events 7-9, for event 1 whether @p value is below the entry count, else 0x101 (event 5
 *         refreshes the pictures first)
 */
extern "C" int cFEStateCheatCharSelect_onWidgetMenu(cFEStateCheatCharSelect* self, int pMenu, unsigned int msg, int value)
{
    switch (msg) {
    case 7:
    case 8:
    case 9:
        return 0x100;
    case 1:
        return value < self->mNumCharacters;
    case 5:
        cFEStateCheatCharSelect_updateCharWidgets(self);
        break;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_updateCharWidgets);
#ifdef SKIP_ASM
/**
 * Refreshes the cheat-character select's six picture slots from the menu's scroll position.
 *
 * Reads the first visible entry from the menu (+0x6C, byte +0x98) and gives each existing picture widget (+0x54) the
 * texture element of entry first + i (+0xF4) at +0x7C, with +0x78 set to -1. cFEStateCheatCharSelect_onWidgetMenu calls
 * it for menu event 5.
 */
extern "C" void cFEStateCheatCharSelect_updateCharWidgets(cFEStateCheatCharSelect* self)
{
    int i;
    int idx = self->mpMenu->mScrollItemOffset;
    for (i = 0; i < 6; i++) {
        cUIIcon* it = (cUIIcon*)self->mpCharWidget[i];
        if (((void*)it) != 0) {
            int textureElem = ((int*)(&self->mpCharElem[0]))[idx + i];
            it->mTextureID = -1;
            it->mpTextureElem = (const cTextureElement*)textureElem;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSetup_cFEStateCharSetup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern void* cFEStateCharSetup_vtbl[];

/**
 * Constructs the character-setup state for a user.
 *
 * Builds the UI state (cUIState_cUIState), sets state id 0xC, the cFEStateCharSetup_vtbl vtable and the user (+0x44),
 * stores cFECustom_getPlayerCtrlMask's value for the user at +0x15 and clears the show-rider flag (+0x4C).
 *
 * @param engine  the UI engine
 * @param idx  the user (controller) index
 * @return @p self
 */
extern "C" void* cFEStateCharSetup_cFEStateCharSetup(cFEStateCharSetup* self, cUIEngine* engine, signed char idx)
{
    cUIState_cUIState(self, engine);
    self->mHashName = 0xC;
    self->mVtbl = cFEStateCharSetup_vtbl;
    self->mUserNum = idx;
    self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, idx);
    self->mbDraw3D = 0;
    return ((void*)self);
}
#endif

