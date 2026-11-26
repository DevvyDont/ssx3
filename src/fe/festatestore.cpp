#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beintbagt.h"
#include "ui/uimenu.h"
#include "bx/math3d.h"
#include "fe/ferider.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "fe/uistatebuypopup.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "be/beintnewrace.h"
#include "fe/festatecredits.h"
#include "ui/uiengine.h"
#include "fe/festateubertrick.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_costVisible);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_cost_num[];
extern char str_have_num[];
extern char str_cost_text[];
extern char str_have_text[];

/**
 * Shows or hides the uber-trick store's cost display: "cost_num", "cost_text", "have_num" and "have_text".
 *
 * Looks the objects up on the state's first screen (cUIState::mScreens), skips missing ones and calls each
 * one's setVisible (vtable slot 9).
 *
 * @param on  nonzero to show, zero to hide (On Tour v)
 */
void cFEStateUberTrick::costVisible(int on)
{
    void* screen = cList_first((cList*)&mScreens);
    cUIThing* costNum = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_num));
    if (costNum) {
        costNum->setVisible(on);
    }
    cUIThing* costText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_text));
    if (costText) {
        costText->setVisible(on);
    }
    cUIThing* haveNum = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_num));
    if (haveNum) {
        haveNum->setVisible(on);
    }
    cUIThing* haveText = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_text));
    if (haveText) {
        haveText->setVisible(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_previewVisible);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern char str_btext5[];
extern char str_ps2sq[];

extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

/**
 * Shows or hides the uber-trick screen's "btext5" and "ps2sq" objects.
 *
 * Looks both up on the state's first screen (cList_first on mScreens) and calls setVisible (vtable slot 9).
 * cFEStateUberTrick_onWidgetEvent shows them with the cost display when the trick menu gets focus and hides them when
 * it is left; cFEStateUberTrick_onCreateScreen hides them.
 *
 * @param on  1 to show, 0 to hide
 */
void cFEStateUberTrick::previewVisible(int on)
{
    void* screen = cList_first((cList*)&mScreens);
    cUIThing* w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_btext5));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_ps2sq));
    if (w) {
        w->setVisible(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_trickVisible);
#ifdef SKIP_ASM
extern char str_To_Execute_info[];
extern char str_Checkboxes[];
extern char str_MenuTrick[];
extern char str_InstBtn0[];
extern char str_InstBtn1[];
extern char str_InstBtn2[];

/**
 * Shows or hides the uber-trick store's trick menu: its info text, checkboxes, menu and the three "InstBtn"
 * objects.
 *
 * The objects are "To Execute info", "Checkboxes", "MenuTrick" and "InstBtn0" to "InstBtn2". Looks them up on
 * the state's first screen (cUIState::mScreens), skips missing ones and calls each one's setVisible (vtable
 * slot 9).
 *
 * @param on  nonzero to show, zero to hide (On Tour v)
 */
void cFEStateUberTrick::trickVisible(int on)
{
    void* screen = cList_first((cList*)&mScreens);
    cUIThing* w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_To_Execute_info));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_Checkboxes));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_MenuTrick));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn0));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn1));
    if (w) {
        w->setVisible(on);
    }
    w = cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn2));
    if (w) {
        w->setVisible(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onCreateScreen);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int a1, int a2, int a3);
extern "C" void cFERiderManager_setHumanVisible(void* self, int a1, int a2);
extern "C" void cSimpleCamera_setupCamWithPosAndTarget(void* cam, void* a, void* b);
extern "C" void* cUITextureBank_getElementByHashname(void* list, int hash);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* p, int a);
extern "C" void cFEStateUberTrick_costVisible(cFEStateUberTrick* self, int on);
extern "C" void cFEStateUberTrick_previewVisible(cFEStateUberTrick* self, int on);
extern char str_66ut_btnmap[];
extern char str_empty_box[];
extern char str_checkbox[];
extern char str_yell_dollar_sign[];



/**
 * Builds the uber-trick store screen ("66ut_btnmap") and sets up the rider preview, the camera and the icons.
 *
 * Adds the screen through mpEngine into mpScreen (+0x40) and plays frame 0. Calls the player interface's (cBE interface
 * 1) vtable slot 2, has the FE rider manager (RealFE + 0xB0) set the user's rider to the user's character
 * (cFERiderManager_setHumanCharacter) and hides that rider (cFERiderManager_setHumanVisible with 0). Points the FE camera (RealFE + 0x10, cSimpleCamera_setupCamWithPosAndTarget) from
 * (0, 660, 80) at (0, 0, 80) and sets its first float to 0.4363 (25 degrees). Looks up the "empty_box", "checkbox" and
 * "yell_dollar_sign" texture elements (mpEngine + 0x58) into +0x58, +0x54 and +0x5C (On Tour mpCheckbox, mpCheckmark,
 * mpDollarSign), sets the sound bank manager's mode 5 (func_0028F140 on cAudioInterface_bxAudio()) and hides the cost display
 * (cFEStateUberTrick_costVisible) and cFEStateUberTrick_previewVisible's objects. onCreateScreen slot (+0x24) of the vtable cFEStateUberTrick_vtbl.
 */
void cFEStateUberTrick::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_66ut_btnmap), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    signed char charID = cBENewPlayerInterface_getPlayerCharID(player, mUserNum);
    {
        player->discard();
    }
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, mUserNum, charID, 0);
    cFERiderManager_setHumanVisible(&SSXApp->RealFE->mRiderManager, mUserNum, 0);
    cFE* realFe = SSXApp->RealFE;
    t4Vector camPos;
    t4Vector lookAt;
    float z = 80.0f;
    float w = 1.0f;
    float y = 660.0f;
    camPos.x = 0.0f;
    camPos.y = y;
    camPos.z = z;
    camPos.w = w;
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = z;
    lookAt.w = w;
    cSimpleCamera_setupCamWithPosAndTarget(&realFe->mCamera, &camPos, &lookAt);
    realFe->mCamera.View = 0.4363323450088501f;
    {
        cUIEngine* uiEngine = mpEngine;
        int iconHash = GetHashValue32(str_empty_box);
        mpCheckbox = (cTextureElement*)cUITextureBank_getElementByHashname(&uiEngine->mTextureBank, iconHash);
    }
    {
        cUIEngine* uiEngine = mpEngine;
        int iconHash = GetHashValue32(str_checkbox);
        mpCheckmark = (cTextureElement*)cUITextureBank_getElementByHashname(&uiEngine->mTextureBank, iconHash);
    }
    {
        cUIEngine* uiEngine = mpEngine;
        int iconHash = GetHashValue32(str_yell_dollar_sign);
        mpDollarSign = (cTextureElement*)cUITextureBank_getElementByHashname(&uiEngine->mTextureBank, iconHash);
    }
    func_0028F140(cAudioInterface_bxAudio(), 5);
    costVisible(0);
    previewVisible(0);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cFERiderManager_setHumanVisible(void* self, int a1, int a2);
void* cUIState_onDestroyScreen(void* self);

/**
 * Hides the user's FE rider when the uber-trick screen goes away, then runs cUIState_onDestroyScreen.
 *
 * The rider is hidden with cFERiderManager_setHumanVisible (user mUserNum, 0) on RealFE + 0xB0, skipped when there is no front end.
 * onDestroyScreen slot (+0x2C) of the vtable cFEStateUberTrick_vtbl.
 */
void cFEStateUberTrick::onDestroyScreen()
{
    cFE* mgr = SSXApp->RealFE;
    if (((char*)mgr) != 0) {
        cFERiderManager_setHumanVisible(&mgr->mRiderManager, mUserNum, 0);
    }
    cUIState_onDestroyScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cFEStateUberTrick_updateTrickMenu(void* self);

/**
 * Refills the uber-trick screen's trick list (cFEStateUberTrick_updateTrickMenu) when @p on is set.
 *
 * onGainTransition slot (+0x3C) of the vtable cFEStateUberTrick_vtbl.
 *
 * @param on  non-zero to refill the trick list
 * @return 0
 */
bool cFEStateUberTrick::onGainTransition(int on)
{
    if (on != 0) {
        cFEStateUberTrick_updateTrickMenu(this);
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onGainFocus);
#ifdef SKIP_ASM


extern "C" void cFEStateUberTrick_activateMainMenu(void* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);

/**
 * Gives the uber-trick screen's category menu the focus when the state gains focus.
 *
 * Runs cFEStateUberTrick_activateMainMenu, calls setFocus(1) on the category menu +0x4C and setFocus(0) on the trick menu +0x50 (vtable slot
 * 7; skipped when missing), then cUIState_onGainFocus. onGainFocus slot (+0x34) of the vtable cFEStateUberTrick_vtbl.
 *
 * @param prevState  the state that had focus before, passed on to cUIState_onGainFocus
 */
extern "C" void cFEStateUberTrick_onGainFocus(cFEStateUberTrick* self, int prevState)
{
    cUIThing* menu;
    cFEStateUberTrick_activateMainMenu(self);
    menu = (cUIThing*)self->mpMainMenu;
    if (menu != 0) {
        menu->setFocus(1);
    }
    menu = (cUIThing*)self->mpTrickMenu;
    if (menu != 0) {
        menu->setFocus(0);
    }
    cUIState_onGainFocus(self, prevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onUpdate);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int idx);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);
extern "C" void cUIState_onUpdate(void* self);

// A t4Vector built from its four components (the pose's position and rotation quaternions below).
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;
extern t4Vector t4vect_ZAxis;
extern "C" void cFERider_setPose(void* rider, tPose* xf);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4VectorXYZW QuatMul_184C60(const t4Vector& a, const t4Vector& b)
{
    t4VectorXYZW r;
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

static inline t4VectorXYZW AxisAngle_184C60(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return t4VectorXYZW(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Poses the user's FE rider on the uber-trick screen once it has loaded, then runs cUIState_onUpdate.
 *
 * Until mbModelVisible is set it takes the user's FE rider (cFERiderManager_getHuman1 on RealFE's mRiderManager).
 * When the rider's mCharID is below 10 and mDoneLoadingTextures and mCompiledModel are both set, it sets the rider's
 * mAnimateBoard, resets its animation (mpRiderAnim, cRiderAnimBase_reset) and plays animation 436
 * (cRiderAnimBase_play, blend -1), places it at (-228, -205, 63) turned 1.0821 rad (62 degrees) about Z (the X and Y
 * turns are 0; fSinCos gives the sines) with cFERider_setPose, shows it (cFERiderManager_setHumanVisible with 1) and
 * sets mbModelVisible. onUpdate slot (+0x64) of the vtable cFEStateUberTrick_vtbl.
 */
void cFEStateUberTrick::onUpdate()
{
    if (mbModelVisible == 0) {
        cFERider* rider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
        int ok = 0;
        if ((unsigned int)rider->mCharID < 10 && rider->mDoneLoadingTextures != 0) {
            ok = rider->mCompiledModel != 0;
        }
        if (ok) {
            rider->mAnimateBoard = 1;
            void* anim = rider->mpRiderAnim;
            if (anim != 0) {
                cRiderAnimBase_reset(anim);
                cRiderAnimBase_play(anim, 436, 0, -1.0f);
            }
            t4VectorXYZW pos;
            tPose xf;
            {
                t4VectorXYZW position;
                position.x = -228.0f;
                position.y = -205.0f;
                position.z = 63.0f;
                position.w = 1.0f;
                pos = position;
                xf.P = position;
                mbModelVisible = 1;
                xf.Q.x = 0.0f;
                xf.Q.y = 0.0f;
                xf.Q.z = 0.0f;
                xf.Q.w = 1.0f;
                xf.Q = QuatMul_184C60(xf.Q, AxisAngle_184C60(t4vect_XAxis, 0.0f));
            }
            xf.Q = QuatMul_184C60(xf.Q, AxisAngle_184C60(t4vect_YAxis, 0.0f));
            xf.Q = QuatMul_184C60(xf.Q, AxisAngle_184C60(t4vect_ZAxis, 1.082104206085205f));
            cFERider_setPose(rider, &xf);
            cFERiderManager_setHumanVisible(&SSXApp->RealFE->mRiderManager, mUserNum, 1);
        }
    }
    cUIState_onUpdate(((char*)this));
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int func_0014FE08(void* self, int rider, int idx, int bit);
extern "C" void cFEStateUberTrick_updateTrickMenuEntry(void* self);
extern "C" void cFEStateUberTrick_updateTrickMenu(void* self);
extern char str_helptext_custom[];
extern char str_kT_66HELPCustomUber[];


/**
 * Pays for and unlocks the selected uber trick when the buy popup closes accepted.
 *
 * On kONPOPUPDESTROY, when the popup's mbAccept (+0x6C) is set: subtracts its mInfo.mCost (+0x48) from the character's
 * money (cBEEconInterface_reduceMoney on cBE interface 11), clears flag bit "trick" (the trick menu's selected
 * mMiscData) of the category's entry (the category menu's selected mMiscData) with func_0014FE08 on interface 6, calls
 * that interface's vtable slot 1, sets "helptext_custom" to "kT_66HELPCustomUber" and refreshes the cost numbers
 * (cFEStateUberTrick_updateTrickMenuEntry). On kONPOPUPDESTROY the trick list is refilled either way
 * (cFEStateUberTrick_updateTrickMenu). Other events do nothing. onPopupEvent slot (+0x8C) of the vtable
 * cFEStateUberTrick_vtbl.
 *
 * @param item  the popup (the cUIStateBuyPopup)
 * @param msg  the popup event
 */
extern "C" void cFEStateUberTrick_onPopupEvent(cFEStateUberTrick* self, cUIState* item, int msg)
{
    switch (msg) {
    case cUIState::kONPOPUPCREATE:
        break;
    case cUIState::kONPOPUPDESTROY:
        if (((cUIStateBuyPopup*)item)->mbAccept != 0) {
            int charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
            cBEEconInterface_reduceMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID, ((cUIStateBuyPopup*)item)->mInfo.mCost);
            int entryIndex = *(signed char*)&self->mpMainMenu->mpSelected->mMiscData;
            int flagBit = *(signed char*)&self->mpTrickMenu->mpSelected->mMiscData;
            cBEBAGTInterface* pi = (cBEBAGTInterface*)cBE_getInterface_Fv(cBE_getBE(), 6);
            func_0014FE08(pi, self->mUserNum, entryIndex, flagBit);
            pi->submit();
            void* helpText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_custom));
            if (helpText != 0) {
                cUIText_setUnicodeStringByID((cUIText*)helpText, GetHashValue32(str_kT_66HELPCustomUber));
            }
            cFEStateUberTrick_updateTrickMenuEntry(self);
        }
        cFEStateUberTrick_updateTrickMenu(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern char str_mute[];
extern char str_Mute[];
extern char str_indy[];
extern char str_Indy[];
extern char str_stalefish[];
extern char str_Stalefish[];
extern char str_method[];
extern char str_Method[];
extern char str_nose[];
extern char str_Nose_Grab[];
extern char str_tail[];
extern char str_Tail_Grab[];
extern char str_done[];
extern char str_MenuMain[];
extern char str_MenuTrick[];

static inline int Is_185080(int id, char* s)
{
    return id == GetHashValue32(s);
}

/**
 * Sets up the uber-trick store's widgets as they are created: labels the trick buttons, remembers the menus.
 *
 * By mHashName: "mute", "indy", "stalefish", "method", "nose" and "tail" get the texts "Mute", "Indy",
 * "Stalefish", "Method", "Nose Grab" and "Tail Grab" and mMiscData 1, 3, 2, 0, 4 and 9; "done" gets -1.
 * "MenuMain" is kept at +0x4C and "MenuTrick" at +0x50.
 *
 * The six trick tests sit in a loop that runs six times without using its counter.
 *
 * @param widget  the widget just created (On Tour pWidget)
 */
void cFEStateUberTrick::onWidgetCreate(cUIThing* widget)
{
    cUIText* text = (cUIText*)widget;
    for (int i = 0; i < 6; i++) {
        if (Is_185080(widget->mHashName, str_mute)) {
            cUIText_setAsciiString(text, str_Mute);
            widget->mMiscData = 1;
            return;
        }
        if (Is_185080(widget->mHashName, str_indy)) {
            cUIText_setAsciiString(text, str_Indy);
            widget->mMiscData = 3;
            return;
        }
        if (Is_185080(widget->mHashName, str_stalefish)) {
            cUIText_setAsciiString(text, str_Stalefish);
            widget->mMiscData = 2;
            return;
        }
        if (Is_185080(widget->mHashName, str_method)) {
            cUIText_setAsciiString(text, str_Method);
            widget->mMiscData = 0;
            return;
        }
        if (Is_185080(widget->mHashName, str_nose)) {
            cUIText_setAsciiString(text, str_Nose_Grab);
            widget->mMiscData = 4;
            return;
        }
        if (Is_185080(widget->mHashName, str_tail)) {
            cUIText_setAsciiString(text, str_Tail_Grab);
            widget->mMiscData = 9;
            return;
        }
    }
    if (Is_185080(widget->mHashName, str_done)) {
        widget->mMiscData = -1;
    } else if (Is_185080(widget->mHashName, str_MenuMain)) {
        mpMainMenu = (cUIMenu*)widget;
    } else if (Is_185080(widget->mHashName, str_MenuTrick)) {
        mpTrickMenu = (cUIMenu*)widget;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void cFEStateUberTrick_costVisible(cFEStateUberTrick* self, int on);
extern "C" void cFEStateUberTrick_trickVisible(cFEStateUberTrick* self, int on);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void cUIStateBuyPopup_initBuyTrick(void* popup, int trick, int cost, int points);
extern "C" void cFEStateUberTrick_updateTrickMenu(void* self);
extern "C" int gGetGrabName(int id);
extern "C" void func_0014FD80(void* iface, int rider, int a, int b);
extern "C" tBEUber* func_0014FF90(void* iface, int rider, int a, int* out);
extern "C" int func_0014FFB8(void* iface, int rider, int a, int b);
extern "C" int func_00150048(void* iface, int rider, int a, int b);
extern "C" int cBEEconInterface_getMoney(void* iface, int a1, int charID);
extern "C" void cFEStateUberTrick_activateMainMenu(void* self);
extern "C" void cFEStateUberTrick_activateTrickMenu(void* self);
extern "C" void cFEStateUberTrick_updateTrickMenuEntry(void* self);
extern "C" void cFEStateUberTrick_previewVisible(cFEStateUberTrick* self, int on);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int idx);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* mem, void* owner, void* state, int a3);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_done[];
extern char str_helptext_choose[];
extern char str_helptext_custom[];
extern char str_kT_66HELPCustomUber[];
extern char str_UITRICKBUY[];
extern char str_kT_66HELPChooseCat[];
extern char str_kT_FEHELPAcceptTrick[];
extern char str_kT_FEHELPTrickAvailBuy[];
extern char str_kT_OVRCMNSaveCashUber[];
extern char str_kT_FEHELPTrickBuyCTM[];

extern cUISoundInterface gUISoundNull;

// A tARGB built from its four components (A, R, G, B), as setColour takes it.
struct tARGBInit : tARGB {
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};
// cUIThing's setColour slot (11), called by hand with the colour as a reference (the slot takes const tARGB*)
typedef sVEntry<void(void*, const tARGB&)> tSetColourEntry;

static inline int Is_185268(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline void SetActive_185268(cUIThing* menu, int on)
{
    menu->setFocus(on);
}

static inline void PlaySound_185268(cFEStateUberTrick* self, int id)
{
    cUISoundInterface* snd = self->mpEngine->mpSound;
    if (snd == 0)
        snd = &gUISoundNull;
    if (snd != 0)
        snd->playSound(id);
}

static inline void Back_185268(cFEStateUberTrick* self)
{
    cUICustomInterface* custom = self->mpEngine->mpCustom;
    void* r = custom->getPrevState(self, 0);
    if (r != 0)
        cUIStateStack_push(&self->mpEngine->mStateStack, r);
}

/**
 * Handles the uber-trick screen's widget events: moving between the menus, equipping or buying a trick, previewing it,
 * and the help texts.
 *
 * A widget's menu is its mpParent: the category menu mpMainMenu or the trick menu mpTrickMenu. The category and
 * trick are the menus' selected items' (mpSelected) mMiscData; the trick table (tBEUber) comes from func_0014FF90 on
 * cBE interface 6. "Back" pushes the state that the engine's mpCustom returns from getPrevState (slot 5,
 * cFECustom_getPrevState). onWidgetEvent slot (+0x9C) of the vtable cFEStateUberTrick_vtbl.
 * - 5 (accept) on the category menu: "done" goes back; another item gives the trick menu focus at row 0 (setFocus,
 *   vtable slot 7), runs cFEStateUberTrick_activateTrickMenu and shows the cost display
 *   (cFEStateUberTrick_costVisible) and cFEStateUberTrick_previewVisible's objects. On the trick menu: an unlocked
 *   trick (func_00150048) that is not for sale (func_0014FFB8) becomes the category's trick (func_0014FD80),
 *   interface 6 is submitted (slot 1) and the list is refilled (cFEStateUberTrick_updateTrickMenu). Otherwise,
 *   outside game path 0 (newRace.cGamePath) UI sound 4 plays; a trick for sale opens a
 *   "UITRICKBUY" cUIStateBuyPopup (cUIStateBuyPopup_initBuyTrick with the trick's name, cost and the character's money)
 *   when the money covers the cost, else sound 4 plays.
 * - 6 (back) on the trick menu: gives the category menu focus, colours the trick menu's selected item opaque black
 *   (setColour, slot 11), deselects it (index 0xFF), runs cFEStateUberTrick_activateMainMenu and hides the cost
 *   display and cFEStateUberTrick_previewVisible's objects. On the category menu it goes back.
 * - 7 on the trick menu: once the rider is shown (mbModelVisible) plays the trick's animation (its table entry's
 *   anim).
 * - 1 (highlight) sets the FE rider's mUberIrradiance. On the category menu an item with a category (mMiscData not
 *   -1) shows the trick widgets (cFEStateUberTrick_trickVisible), refills the list and sets "helptext_choose" to
 *   "kT_66HELPChooseCat"; other items hide them and set "kT_FEHELPAcceptTrick". On the trick menu "helptext_custom"
 *   says "kT_66HELPCustomUber" for an equippable trick, "kT_FEHELPTrickAvailBuy" or "kT_OVRCMNSaveCashUber" for a trick
 *   for sale by whether the money covers it, "kT_FEHELPTrickBuyCTM" outside game path 0; then the cost numbers are
 *   refreshed (cFEStateUberTrick_updateTrickMenuEntry).
 * - 2 clears the FE rider's mUberIrradiance.
 *
 * @param item  the widget that sent the event
 * @param msg  the widget event
 */
extern "C" void cFEStateUberTrick_onWidgetEvent(cFEStateUberTrick* self, cUIThing* item, int msg)
{
    switch (msg) {
    case cUIState::kONNEXT:
        if ((char*)item->mpParent == (char*)self->mpMainMenu) {
            if (Is_185268(item->mHashName, str_done)) {
                Back_185268(self);
            } else {
                SetActive_185268(self->mpMainMenu, 0);
                cUIMenu_setSelectedByIndex(self->mpTrickMenu, 0);
                SetActive_185268(self->mpTrickMenu, 1);
                cFEStateUberTrick_activateTrickMenu(((char*)self));
                self->costVisible(1);
                self->previewVisible(1);
            }
        } else if ((char*)item->mpParent == (char*)self->mpTrickMenu) {
            cUIThing* categoryItem = (cUIThing*)self->mpMainMenu->mpSelected;
            cUIThing* trickItem = (cUIThing*)self->mpTrickMenu->mpSelected;
            if (((char*)categoryItem) != 0 && ((char*)trickItem) != 0) {
                int ai = *(signed char*)&categoryItem->mMiscData;
                int bi = *(signed char*)&trickItem->mMiscData;
                cBEInterface* pi = cBE_getInterface_Fv(cBE_getBE(), 6);
                cBE_getInterface_Fv(cBE_getBE(), 0);
                if (func_00150048(pi, self->mUserNum, ai, bi) && !func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    func_0014FD80(pi, self->mUserNum, ai, bi);
                    // submit (slot 1) by hand, through an int-returning slot: the void virtual call allocates
                    // other registers
                    sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)VPTR_OF(cBEInterface, pi);
                    vt[1].fn(vthis(pi, vt[1]));
                    cFEStateUberTrick_updateTrickMenu(((char*)self));
                } else if (cBENewRaceInterface_newRace.cGamePath != 0) {
                    PlaySound_185268(self, 4);
                } else if (func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
                    signed char charID = cBENewPlayerInterface_getPlayerCharID(player, self->mUserNum);
                    player->discard();
                    int points = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID);
                    int n;
                    tBEUber* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
                    int trick = gGetGrabName(tbl[bi].id);
                    if (points >= tbl[bi].cost) {
                        void* popup = cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_UITRICKBUY, 0x100, 0), self->mpEngine, ((char*)self), self->mUserNum);
                        cUIStateBuyPopup_initBuyTrick(popup, trick, tbl[bi].cost, points);
                        cUIStateStack_pushPopup(&self->mpEngine->mStateStack, popup);
                    } else {
                        PlaySound_185268(self, 4);
                    }
                }
            }
        }
        break;
    case cUIState::kONPREV:
        if ((char*)item->mpParent == (char*)self->mpTrickMenu) {
            SetActive_185268(self->mpMainMenu, 1);
            SetActive_185268(self->mpTrickMenu, 0);
            {
                cUIThing* trickSelection = self->mpTrickMenu->mpSelected;
                if (trickSelection != 0) {
                    tSetColourEntry* vt = (tSetColourEntry*)VPTR_OF(cListNode, trickSelection);
                    tSetColourEntry* colorEntry = &vt[11];
                    colorEntry->fn(vthis(trickSelection, *colorEntry), tARGBInit(1.0f, 0.0f, 0.0f, 0.0f));
                }
            }
            cUIMenu_setSelectedByIndex(self->mpTrickMenu, 0xFF);
            cFEStateUberTrick_activateMainMenu(((char*)self));
            self->costVisible(0);
            self->previewVisible(0);
        } else {
            Back_185268(self);
        }
        break;
    case cUIState::kONOPTION:
        if ((char*)item->mpParent == (char*)self->mpTrickMenu) {
            cUIThing* categoryItem = (cUIThing*)self->mpMainMenu->mpSelected;
            cUIThing* trickItem = (cUIThing*)self->mpTrickMenu->mpSelected;
            if (((char*)categoryItem) != 0 && ((char*)trickItem) != 0) {
                cBEInterface* pi = cBE_getInterface_Fv(cBE_getBE(), 6);
                int bi = *(signed char*)&trickItem->mMiscData;
                int ai = *(signed char*)&categoryItem->mMiscData;
                int n = 0;
                tBEUber* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
                if (self->mbModelVisible != 0) {
                    cFERider* feRider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum);
                    cRiderAnimBase_play(feRider->mpRiderAnim, tbl[bi].anim, 0, -1.0f);
                }
            }
        }
        break;
    case cUIState::kONGAINFOCUS:
        cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum)->mUberIrradiance = 1;
        if ((char*)item->mpParent == (char*)self->mpMainMenu) {
            if (self->mpTrickMenu != 0 && item->mMiscData != -1) {
                self->trickVisible(1);
                cFEStateUberTrick_updateTrickMenu(((char*)self));
                cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
                if (helpText != 0)
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_66HELPChooseCat));
            } else {
                self->trickVisible(0);
                cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
                if (helpText != 0)
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_FEHELPAcceptTrick));
            }
        } else if ((char*)self->mpTrickMenu != 0 && (char*)item->mpParent == (char*)self->mpTrickMenu) {
            cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_custom));
            cUIThing* categoryItem = (cUIThing*)self->mpMainMenu->mpSelected;
            cUIThing* trickItem = (cUIThing*)self->mpTrickMenu->mpSelected;
            if (((char*)categoryItem) != 0 && ((char*)trickItem) != 0 && helpText != 0) {
                int ai = *(signed char*)&categoryItem->mMiscData;
                int bi = *(signed char*)&trickItem->mMiscData;
                cBEInterface* pi = cBE_getInterface_Fv(cBE_getBE(), 6);
                cBE_getInterface_Fv(cBE_getBE(), 0);
                if (func_00150048(pi, self->mUserNum, ai, bi) && !func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_66HELPCustomUber));
                } else if (cBENewRaceInterface_newRace.cGamePath == 0) {
                    if (func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                        cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
                        signed char charID = cBENewPlayerInterface_getPlayerCharID(player, self->mUserNum);
                        player->discard();
                        int points = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID);
                        int n;
                        tBEUber* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
                        if (points >= tbl[bi].cost)
                            cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_FEHELPTrickAvailBuy));
                        else
                            cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_OVRCMNSaveCashUber));
                    }
                } else {
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_FEHELPTrickBuyCTM));
                }
                cFEStateUberTrick_updateTrickMenuEntry(((char*)self));
            }
        }
        break;
    case cUIState::kONLOSEFOCUS:
        cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, self->mUserNum)->mUberIrradiance = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the uber-trick screen's menu events: 0x100 for kONMISC on the trick menu and for kONOPTION and kONMISC on the
 * category menu, otherwise 0x101.
 *
 * The trick menu is +0x50. onWidgetMenu slot (+0xA4) of the vtable cFEStateUberTrick_vtbl.
 *
 * @param menu  the menu that asks
 * @param event  the menu event
 * @return 0x100 or 0x101 as above
 */
extern "C" int cFEStateUberTrick_onWidgetMenu(cFEStateUberTrick* self, int menu, unsigned int event)
{
    if (menu == (int)self->mpTrickMenu) {
        switch (event) {
        case cUIMenu::kONMISC:
            return 0x100;
        }
    } else {
        switch (event) {
        case cUIMenu::kONOPTION:
        case cUIMenu::kONMISC:
            return 0x100;
        }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateCredits_cFEStateCredits1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateCredits_vtbl[];

/**
 * Constructs the credits state (allocated as "cFEOptionsCredits", 104 bytes).
 *
 * Runs cUIState's constructor (cUIState_cUIState; the engine argument reaches it untouched in the second argument
 * register), stores cFEStateCredits_vtbl and mHashName 36, sets +0x60 to -100 and +0x64 to 1 (On Tour mPixelOffset,
 * mPixelDelta) and clears +0x48, +0x58 and +0x5C (On Tour mpWidget, mpDolbyIcon, mpTextScroll).
 *
 * @return @p self
 */
void* cFEStateCredits::cFEStateCredits1()
{
    cUIState_cUIState(this);
    VPTR_OF(cListNode, this) = cFEStateCredits_vtbl;
    mPixelOffset = -100;
    mPixelDelta = 1;
    mHashName = 36;
    mpDolbyIcon = 0;
    mpTextScroll = 0;
    mpWidget = 0;
    return ((void*)this);
}
#endif

extern void* cFEState_vtbl[];
extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/festatestore", cFEStateCredits__cFEStateCredits__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the credits front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
void* cFEStateCredits__cFEStateCredits(void* vself)
{
    cFEStateCredits* self = (cFEStateCredits*)vself;
    VPTR_OF(cListNode, self) = (void**)((int)(void*)cFEState_vtbl);
    return cUIState__cUIState(self);
}
#endif

