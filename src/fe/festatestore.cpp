#include "common.h"
#include "fe/festateubertrick.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_costVisible);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList* list);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_cost_num[];
extern char str_have_num[];
extern char str_cost_text[];
extern char str_have_text[];

class cWidget_00184668 {
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
    virtual void setVisible(int a);
};

/**
 * Shows or hides the uber-trick store's cost display: "cost_num", "cost_text", "have_num" and "have_text".
 *
 * Looks the objects up on the state's first screen (cUIState::mScreens), skips missing ones and calls each
 * one's setVisible (vtable slot 9).
 *
 * @param on  nonzero to show, zero to hide (On Tour v)
 */
extern "C" void cFEStateUberTrick_costVisible(cFEStateUberTrick* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184668* costNum = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_num));
    if (costNum) {
        costNum->setVisible(on);
    }
    cWidget_00184668* costText = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_cost_text));
    if (costText) {
        costText->setVisible(on);
    }
    cWidget_00184668* haveNum = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_num));
    if (haveNum) {
        haveNum->setVisible(on);
    }
    cWidget_00184668* haveText = (cWidget_00184668*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_have_text));
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

class cWidget_00184780 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09(int a);
};

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

/**
 * Shows or hides the uber-trick screen's "btext5" and "ps2sq" objects.
 *
 * Looks both up on the state's first screen (cList_first on the screen list +0x24) and calls setVisible (vtable slot
 * 9). cFEStateUberTrick_onWidgetEvent shows them with the cost display when the trick menu gets focus and hides them when it is left;
 * cFEStateUberTrick_onCreateScreen hides them.
 *
 * @param on  1 to show, 0 to hide
 */
extern "C" void cFEStateUberTrick_previewVisible(cFEStateUberTrick* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184780* w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_btext5));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_ps2sq));
    if (w) {
        w->v09(on);
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
extern "C" void cFEStateUberTrick_trickVisible(cFEStateUberTrick* self, int on)
{
    void* screen = cList_first((cList*)((char*)self + 0x24));
    cWidget_00184780* w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_To_Execute_info));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_Checkboxes));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_MenuTrick));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn0));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn1));
    if (w) {
        w->v09(on);
    }
    w = (cWidget_00184780*)cUIScreen_getObjectByHashName(screen, GetHashValue32(str_InstBtn2));
    if (w) {
        w->v09(on);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onCreateScreen);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int a1, int a2, int a3);
extern "C" void cFERiderManager_setHumanVisible(void* self, int a1, int a2);
extern "C" void cSimpleCamera_setupCamWithPosAndTarget(void* cam, void* a, void* b);
extern "C" void* cUITextureBank_getElementByHashname(void* list, int hash);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* p, int a);
extern "C" void cFEStateUberTrick_costVisible(cFEStateUberTrick* self, int on);
extern "C" void cFEStateUberTrick_previewVisible(cFEStateUberTrick* self, int on);
extern char str_66ut_btnmap[];
extern char str_empty_box[];
extern char str_checkbox[];
extern char str_yell_dollar_sign[];

struct sVec4_1849B0 {
    float x, y, z, w;
};

struct sVE_1849B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Builds the uber-trick store screen ("66ut_btnmap") and sets up the rider preview, the camera and the icons.
 *
 * Adds the screen through mpEngine into mpScreen (+0x40) and plays frame 0. Calls the player interface's (cBE interface
 * 1) vtable slot 2, has the FE rider manager (RealFE + 0xB0) set the user's rider to the user's character
 * (cFERiderManager_setHumanCharacter) and hides that rider (cFERiderManager_setHumanVisible with 0). Points the FE camera (RealFE + 0x10, cSimpleCamera_setupCamWithPosAndTarget) from
 * (0, 660, 80) at (0, 0, 80) and sets its first float to 0.4363 (25 degrees). Looks up the "empty_box", "checkbox" and
 * "yell_dollar_sign" texture elements (mpEngine + 0x58) into +0x58, +0x54 and +0x5C (On Tour mpCheckbox, mpCheckmark,
 * mpDollarSign), sets the sound bank manager's mode 5 (func_0028F140 on cAudioInterface_bxAudio()) and hides the cost display
 * (cFEStateUberTrick_costVisible) and cFEStateUberTrick_previewVisible's objects. onCreateScreen slot (+0x24) of the vtable D_0046CCC0.
 */
extern "C" void cFEStateUberTrick_onCreateScreen(cFEStateUberTrick* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_66ut_btnmap), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    signed char charID = cBENewPlayerInterface_getPlayerCharID(player, self->mUserNum);
    {
        sVE_1849B0* vt = *(sVE_1849B0**)(player + 0xC);
        vt[2].fn(player + vt[2].delta);
    }
    cFERiderManager_setHumanCharacter((char*)SSXApp->RealFE + 0xB0, self->mUserNum, charID, 0);
    cFERiderManager_setHumanVisible((char*)SSXApp->RealFE + 0xB0, self->mUserNum, 0);
    char* realFe = (char*)SSXApp->RealFE;
    sVec4_1849B0 camPos;
    sVec4_1849B0 lookAt;
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
    cSimpleCamera_setupCamWithPosAndTarget(realFe + 0x10, &camPos, &lookAt);
    *(float*)(realFe + 0x10) = 0.4363323450088501f;
    {
        char* uiEngine = (char*)self->mpEngine;
        int iconHash = GetHashValue32(str_empty_box);
        self->mpCheckbox = (cTextureElement*)cUITextureBank_getElementByHashname(uiEngine + 0x58, iconHash);
    }
    {
        char* uiEngine = (char*)self->mpEngine;
        int iconHash = GetHashValue32(str_checkbox);
        self->mpCheckmark = (cTextureElement*)cUITextureBank_getElementByHashname(uiEngine + 0x58, iconHash);
    }
    {
        char* uiEngine = (char*)self->mpEngine;
        int iconHash = GetHashValue32(str_yell_dollar_sign);
        self->mpDollarSign = (cTextureElement*)cUITextureBank_getElementByHashname(uiEngine + 0x58, iconHash);
    }
    func_0028F140(cAudioInterface_bxAudio(), 5);
    cFEStateUberTrick_costVisible(self, 0);
    cFEStateUberTrick_previewVisible(self, 0);
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
 * onDestroyScreen slot (+0x2C) of the vtable D_0046CCC0.
 */
extern "C" void cFEStateUberTrick_onDestroyScreen(cFEStateUberTrick* self)
{
    char* mgr = (char*)SSXApp->RealFE;
    if (mgr != 0) {
        cFERiderManager_setHumanVisible(mgr + 0xB0, self->mUserNum, 0);
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cFEStateUberTrick_updateTrickMenu(void* self);

/**
 * Refills the uber-trick screen's trick list (cFEStateUberTrick_updateTrickMenu) when @p on is set.
 *
 * onGainTransition slot (+0x3C) of the vtable D_0046CCC0.
 *
 * @param on  non-zero to refill the trick list
 * @return 0
 */
extern "C" int cFEStateUberTrick_onGainTransition(cFEStateUberTrick* self, int on)
{
    if (on != 0) {
        cFEStateUberTrick_updateTrickMenu(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onGainFocus);
#ifdef SKIP_ASM
struct sVEntry00184BE0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00184BE0 {
    int pad[2];
    sVEntry00184BE0* vt;
};

extern "C" void cFEStateUberTrick_activateMainMenu(void* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);

/**
 * Gives the uber-trick screen's category menu the focus when the state gains focus.
 *
 * Runs cFEStateUberTrick_activateMainMenu, calls setFocus(1) on the category menu +0x4C and setFocus(0) on the trick menu +0x50 (vtable slot
 * 7; skipped when missing), then cUIState_onGainFocus. onGainFocus slot (+0x34) of the vtable D_0046CCC0.
 *
 * @param prevState  the state that had focus before, passed on to cUIState_onGainFocus
 */
extern "C" void cFEStateUberTrick_onGainFocus(cFEStateUberTrick* self, int prevState)
{
    sObj00184BE0* menu;
    cFEStateUberTrick_activateMainMenu(self);
    menu = (sObj00184BE0*)self->mpMainMenu;
    if (menu != 0) {
        menu->vt[7].fn((char*)menu + menu->vt[7].delta, 1);
    }
    menu = (sObj00184BE0*)self->mpTrickMenu;
    if (menu != 0) {
        menu->vt[7].fn((char*)menu + menu->vt[7].delta, 0);
    }
    cUIState_onGainFocus(self, prevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onUpdate);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void* cFERiderManager_getHuman1(void* self, int idx);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);
extern "C" void cUIState_onUpdate(void* self);

struct sQuat_184C60
{
    float x, y, z, w;
    sQuat_184C60() {}
    sQuat_184C60(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sXform_184C60
{
    sQuat_184C60 pos;
    sQuat_184C60 rot;
};

extern sQuat_184C60 t4vect_XAxis;
extern sQuat_184C60 t4vect_YAxis;
extern sQuat_184C60 t4vect_ZAxis;
extern "C" void cFERider_setPose(void* rider, sXform_184C60* xf);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_184C60 QuatMul_184C60(const sQuat_184C60& a, const sQuat_184C60& b)
{
    sQuat_184C60 r;
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

static inline sQuat_184C60 AxisAngle_184C60(const sQuat_184C60& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_184C60(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Poses the user's FE rider on the uber-trick screen once it has loaded, then runs cUIState_onUpdate.
 *
 * Until +0x48 (On Tour mbModelVisible) is set it takes the user's FE rider (cFERiderManager_getHuman1 on RealFE + 0xB0). When the
 * rider's character (+0x0) is below 10 and +0xCB8 and +0xCB4 are both set, it sets the rider's +0xCCC, resets its
 * animation (+0xC, cRiderAnimBase_reset) and plays animation 436 (cRiderAnimBase_play, blend -1), places it at (-228,
 * -205, 63) turned 1.0821 rad (62 degrees) about Z (the X and Y turns are 0; fSinCos gives the sines) with
 * cFERider_setPose, shows it (cFERiderManager_setHumanVisible with 1) and sets +0x48. onUpdate slot (+0x64) of the vtable D_0046CCC0.
 */
extern "C" void cFEStateUberTrick_onUpdate(cFEStateUberTrick* self)
{
    if (self->mbModelVisible == 0) {
        char* rider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, self->mUserNum);
        int ok = 0;
        if (*(unsigned int*)rider < 10 && *(int*)(rider + 0xCB8) != 0) {
            ok = *(int*)(rider + 0xCB4) != 0;
        }
        if (ok) {
            *(int*)(rider + 0xCCC) = 1;
            void* anim = *(void**)(rider + 0xC);
            if (anim != 0) {
                cRiderAnimBase_reset(anim);
                cRiderAnimBase_play(anim, 436, 0, -1.0f);
            }
            sQuat_184C60 pos;
            sXform_184C60 xf;
            {
                sQuat_184C60 position;
                position.x = -228.0f;
                position.y = -205.0f;
                position.z = 63.0f;
                position.w = 1.0f;
                pos = position;
                xf.pos = position;
                self->mbModelVisible = 1;
                xf.rot.x = 0.0f;
                xf.rot.y = 0.0f;
                xf.rot.z = 0.0f;
                xf.rot.w = 1.0f;
                xf.rot = QuatMul_184C60(xf.rot, AxisAngle_184C60(t4vect_XAxis, 0.0f));
            }
            xf.rot = QuatMul_184C60(xf.rot, AxisAngle_184C60(t4vect_YAxis, 0.0f));
            xf.rot = QuatMul_184C60(xf.rot, AxisAngle_184C60(t4vect_ZAxis, 1.082104206085205f));
            cFERider_setPose(rider, &xf);
            cFERiderManager_setHumanVisible((char*)SSXApp->RealFE + 0xB0, self->mUserNum, 1);
        }
    }
    cUIState_onUpdate(((char*)self));
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int func_0014FE08(void* self, int rider, int idx, int bit);
extern "C" void cFEStateUberTrick_updateTrickMenuEntry(void* self);
extern "C" void cFEStateUberTrick_updateTrickMenu(void* self);
extern char str_helptext_custom[];
extern char str_kT_66HELPCustomUber[];

struct sVE_4F40 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Pays for and unlocks the selected uber trick when the buy popup closes accepted.
 *
 * On event 22, when the popup's mbAccept (+0x6C) is set: subtracts its mInfo.mCost (+0x48) from the character's money
 * (cBEEconInterface_reduceMoney on cBE interface 11), clears flag bit "trick" (the trick menu's selected mMiscData) of the category's
 * entry (the category menu's selected mMiscData) with func_0014FE08 on interface 6, calls that interface's vtable slot
 * 1, sets "helptext_custom" to "kT_66HELPCustomUber" and refreshes the cost numbers (cFEStateUberTrick_updateTrickMenuEntry). On event 22 the
 * trick list is refilled either way (cFEStateUberTrick_updateTrickMenu). Other events do nothing. onPopupEvent slot (+0x8C) of the
 * vtable D_0046CCC0.
 *
 * @param item  the popup (the cUIStateBuyPopup)
 * @param msg  the popup event
 */
extern "C" void cFEStateUberTrick_onPopupEvent(cFEStateUberTrick* self, void* item, int msg)
{
    switch (msg) {
    case 21:
        break;
    case 22:
        if (*(int*)((char*)item + 0x6C) != 0) {
            int charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
            cBEEconInterface_reduceMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID, *(int*)((char*)item + 0x48));
            int entryIndex = *(signed char*)(*(char**)((char*)self->mpMainMenu + 0xA0) + 0x18);
            int flagBit = *(signed char*)(*(char**)((char*)self->mpTrickMenu + 0xA0) + 0x18);
            void* pi = cBE_getInterface_Fv(cBE_getBE(), 6);
            func_0014FE08(pi, self->mUserNum, entryIndex, flagBit);
            sVE_4F40* vt = *(sVE_4F40**)((char*)pi + 0xC);
            vt[1].fn((char*)pi + vt[1].delta);
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
extern "C" void cFEStateUberTrick_onWidgetCreate(cFEStateUberTrick* self, void* widget)
{
    cUIText* text = (cUIText*)widget;
    for (int i = 0; i < 6; i++) {
        if (Is_185080(*(int*)((char*)widget + 0x38), str_mute)) {
            cUIText_setAsciiString(text, str_Mute);
            *(int*)((char*)widget + 0x18) = 1;
            return;
        }
        if (Is_185080(*(int*)((char*)widget + 0x38), str_indy)) {
            cUIText_setAsciiString(text, str_Indy);
            *(int*)((char*)widget + 0x18) = 3;
            return;
        }
        if (Is_185080(*(int*)((char*)widget + 0x38), str_stalefish)) {
            cUIText_setAsciiString(text, str_Stalefish);
            *(int*)((char*)widget + 0x18) = 2;
            return;
        }
        if (Is_185080(*(int*)((char*)widget + 0x38), str_method)) {
            cUIText_setAsciiString(text, str_Method);
            *(int*)((char*)widget + 0x18) = 0;
            return;
        }
        if (Is_185080(*(int*)((char*)widget + 0x38), str_nose)) {
            cUIText_setAsciiString(text, str_Nose_Grab);
            *(int*)((char*)widget + 0x18) = 4;
            return;
        }
        if (Is_185080(*(int*)((char*)widget + 0x38), str_tail)) {
            cUIText_setAsciiString(text, str_Tail_Grab);
            *(int*)((char*)widget + 0x18) = 9;
            return;
        }
    }
    if (Is_185080(*(int*)((char*)widget + 0x38), str_done)) {
        *(int*)((char*)widget + 0x18) = -1;
    } else if (Is_185080(*(int*)((char*)widget + 0x38), str_MenuMain)) {
        self->mpMainMenu = (cUIMenu*)widget;
    } else if (Is_185080(*(int*)((char*)widget + 0x38), str_MenuTrick)) {
        self->mpTrickMenu = (cUIMenu*)widget;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" int func_00116938(int id);
extern "C" void func_0014FD80(void* iface, int rider, int a, int b);
struct sTrick_185268;
extern "C" sTrick_185268* func_0014FF90(void* iface, int rider, int a, int* out);
extern "C" int func_0014FFB8(void* iface, int rider, int a, int b);
extern "C" int func_00150048(void* iface, int rider, int a, int b);
extern "C" int cBEEconInterface_getMoney(void* iface, int a1, int charID);
extern "C" void cFEStateUberTrick_activateMainMenu(void* self);
extern "C" void cFEStateUberTrick_activateTrickMenu(void* self);
extern "C" void cFEStateUberTrick_updateTrickMenuEntry(void* self);
extern "C" void cFEStateUberTrick_previewVisible(cFEStateUberTrick* self, int on);
extern "C" void* cFERiderManager_getHuman1(void* self, int idx);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* mem, void* owner, void* state, int a3);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern signed char D_00535C11[];
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

struct sVE_185268 {
    short delta;
    short index;
    void* fn;
};
struct sSnd_185268 {
    sVE_185268* vt;
};
extern sSnd_185268 gUISoundNull;

struct sColor_185268 {
    float a, r, g, b;
    sColor_185268(float a_, float r_, float g_, float b_) : a(a_), r(r_), g(g_), b(b_) {}
};
class cUIObj_185268 {
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
    virtual void v09();
    virtual void v10();
    virtual void SetColor(const sColor_185268& c);
};

struct sTrick_185268 {
    unsigned short anim;
    unsigned short f2;
    unsigned short id;
    unsigned short cost;
};

static inline int Is_185268(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline void SetActive_185268(char* menu, int on)
{
    sVE_185268* vt = *(sVE_185268**)(menu + 8);
    ((void (*)(void*, int))vt[7].fn)(menu + vt[7].delta, on);
}

static inline void PlaySound_185268(char* self, int id)
{
    sSnd_185268* snd = *(sSnd_185268**)(*(char**)(self + 0x10) + 0x14);
    if (snd == 0)
        snd = &gUISoundNull;
    if (snd != 0)
        ((void (*)(void*, int))snd->vt[2].fn)((char*)snd + snd->vt[2].delta, id);
}

static inline void Back_185268(char* self)
{
    char* mgr = **(char***)(self + 0x10);
    sVE_185268* vt = *(sVE_185268**)(mgr + 4);
    void* r = ((void* (*)(void*, void*, int))vt[5].fn)(mgr + vt[5].delta, self, 0);
    if (r != 0)
        cUIStateStack_push(*(char**)(self + 0x10) + 0x18, r);
}

/**
 * Handles the uber-trick screen's widget events: moving between the menus, equipping or buying a trick, previewing it,
 * and the help texts.
 *
 * A widget's menu is its mpParent (+0x40): the category menu +0x4C or the trick menu +0x50 (On Tour mpMainMenu,
 * mpTrickMenu). The category and trick are the menus' selected items' (mpSelected, +0xA0) mMiscData (+0x18); the trick
 * table comes from func_0014FF90 on cBE interface 6. "Back" pushes the state that the custom front end (*mpEngine)
 * returns from its vtable slot 5 (cFECustom_getPrevState). onWidgetEvent slot (+0x9C) of the vtable D_0046CCC0.
 * - 5 (accept) on the category menu: "done" goes back; another item gives the trick menu focus at row 0 (setFocus,
 *   vtable slot 7), runs cFEStateUberTrick_activateTrickMenu and shows the cost display (cFEStateUberTrick_costVisible) and cFEStateUberTrick_previewVisible's
 *   objects. On the trick menu: an unlocked trick (func_00150048) that is not for sale (func_0014FFB8) becomes the
 *   category's trick (func_0014FD80), interface 6's vtable slot 1 runs and the list is refilled
 *   (cFEStateUberTrick_updateTrickMenu). Otherwise, outside game path 0 (D_00535C11) UI sound 4 plays; a trick for sale opens a
 *   "UITRICKBUY" cUIStateBuyPopup (cUIStateBuyPopup_initBuyTrick with the trick's name, cost and the character's money)
 *   when the money covers the cost, else sound 4 plays.
 * - 6 (back) on the trick menu: gives the category menu focus, colours the trick menu's selected item opaque black
 *   (setColour, slot 11), deselects it (index 0xFF), runs cFEStateUberTrick_activateMainMenu and hides the cost display and cFEStateUberTrick_previewVisible's
 *   objects. On the category menu it goes back.
 * - 7 on the trick menu: once the rider is shown (+0x48) plays the trick's animation (its table entry's first
 *   halfword).
 * - 1 (highlight) sets the FE rider's +0xCD0. On the category menu an item with a category (mMiscData not -1) shows the
 *   trick widgets (cFEStateUberTrick_trickVisible), refills the list and sets "helptext_choose" to
 *   "kT_66HELPChooseCat"; other items hide them and set "kT_FEHELPAcceptTrick". On the trick menu "helptext_custom"
 *   says "kT_66HELPCustomUber" for an equippable trick, "kT_FEHELPTrickAvailBuy" or "kT_OVRCMNSaveCashUber" for a trick
 *   for sale by whether the money covers it, "kT_FEHELPTrickBuyCTM" outside game path 0; then the cost numbers are
 *   refreshed (cFEStateUberTrick_updateTrickMenuEntry).
 * - 2 clears the FE rider's +0xCD0.
 *
 * @param item  the widget that sent the event
 * @param msg  the widget event
 */
extern "C" void cFEStateUberTrick_onWidgetEvent(cFEStateUberTrick* self, char* item, int msg)
{
    switch (msg) {
    case 5:
        if (*(char**)(item + 0x40) == (char*)self->mpMainMenu) {
            if (Is_185268(*(int*)(item + 0x38), str_done)) {
                Back_185268(((char*)self));
            } else {
                SetActive_185268((char*)self->mpMainMenu, 0);
                cUIMenu_setSelectedByIndex(self->mpTrickMenu, 0);
                SetActive_185268((char*)self->mpTrickMenu, 1);
                cFEStateUberTrick_activateTrickMenu(((char*)self));
                cFEStateUberTrick_costVisible(self, 1);
                cFEStateUberTrick_previewVisible(self, 1);
            }
        } else if (*(char**)(item + 0x40) == (char*)self->mpTrickMenu) {
            char* categoryItem = *(char**)((char*)self->mpMainMenu + 0xA0);
            char* trickItem = *(char**)((char*)self->mpTrickMenu + 0xA0);
            if (categoryItem != 0 && trickItem != 0) {
                int ai = *(signed char*)(categoryItem + 0x18);
                int bi = *(signed char*)(trickItem + 0x18);
                char* pi = (char*)cBE_getInterface_Fv(cBE_getBE(), 6);
                cBE_getInterface_Fv(cBE_getBE(), 0);
                if (func_00150048(pi, self->mUserNum, ai, bi) && !func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    func_0014FD80(pi, self->mUserNum, ai, bi);
                    sVE_185268* vt = *(sVE_185268**)(pi + 0xC);
                    ((int (*)(void*))vt[1].fn)(pi + vt[1].delta);
                    cFEStateUberTrick_updateTrickMenu(((char*)self));
                } else if (D_00535C11[0] != 0) {
                    PlaySound_185268(((char*)self), 4);
                } else if (func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
                    signed char charID = cBENewPlayerInterface_getPlayerCharID(player, self->mUserNum);
                    {
                        sVE_185268* vt = *(sVE_185268**)(player + 0xC);
                        ((void (*)(void*))vt[2].fn)(player + vt[2].delta);
                    }
                    int points = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID);
                    int n;
                    sTrick_185268* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
                    int trick = func_00116938(tbl[bi].id);
                    if (points >= tbl[bi].cost) {
                        void* popup = cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_UITRICKBUY, 0x100, 0), self->mpEngine, ((char*)self), self->mUserNum);
                        cUIStateBuyPopup_initBuyTrick(popup, trick, tbl[bi].cost, points);
                        cUIStateStack_pushPopup((char*)self->mpEngine + 0x18, popup);
                    } else {
                        PlaySound_185268(((char*)self), 4);
                    }
                }
            }
        }
        break;
    case 6:
        if (*(char**)(item + 0x40) == (char*)self->mpTrickMenu) {
            SetActive_185268((char*)self->mpMainMenu, 1);
            SetActive_185268((char*)self->mpTrickMenu, 0);
            {
                cUIObj_185268* trickSelection = *(cUIObj_185268**)((char*)self->mpTrickMenu + 0xA0);
                if (trickSelection != 0)
                    trickSelection->SetColor(sColor_185268(1.0f, 0.0f, 0.0f, 0.0f));
            }
            cUIMenu_setSelectedByIndex(self->mpTrickMenu, 0xFF);
            cFEStateUberTrick_activateMainMenu(((char*)self));
            cFEStateUberTrick_costVisible(self, 0);
            cFEStateUberTrick_previewVisible(self, 0);
        } else {
            Back_185268(((char*)self));
        }
        break;
    case 7:
        if (*(char**)(item + 0x40) == (char*)self->mpTrickMenu) {
            char* categoryItem = *(char**)((char*)self->mpMainMenu + 0xA0);
            char* trickItem = *(char**)((char*)self->mpTrickMenu + 0xA0);
            if (categoryItem != 0 && trickItem != 0) {
                char* pi = (char*)cBE_getInterface_Fv(cBE_getBE(), 6);
                int bi = *(signed char*)(trickItem + 0x18);
                int ai = *(signed char*)(categoryItem + 0x18);
                int n = 0;
                sTrick_185268* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
                if (self->mbModelVisible != 0) {
                    char* feRider = (char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, self->mUserNum);
                    cRiderAnimBase_play(*(void**)(feRider + 0xC), tbl[bi].anim, 0, -1.0f);
                }
            }
        }
        break;
    case 1:
        *(int*)((char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, self->mUserNum) + 0xCD0) = 1;
        if (*(char**)(item + 0x40) == (char*)self->mpMainMenu) {
            if (self->mpTrickMenu != 0 && *(int*)(item + 0x18) != -1) {
                cFEStateUberTrick_trickVisible(self, 1);
                cFEStateUberTrick_updateTrickMenu(((char*)self));
                cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
                if (helpText != 0)
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_66HELPChooseCat));
            } else {
                cFEStateUberTrick_trickVisible(self, 0);
                cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
                if (helpText != 0)
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_FEHELPAcceptTrick));
            }
        } else if ((char*)self->mpTrickMenu != 0 && *(char**)(item + 0x40) == (char*)self->mpTrickMenu) {
            cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_custom));
            char* categoryItem = *(char**)((char*)self->mpMainMenu + 0xA0);
            char* trickItem = *(char**)((char*)self->mpTrickMenu + 0xA0);
            if (categoryItem != 0 && trickItem != 0 && helpText != 0) {
                int ai = *(signed char*)(categoryItem + 0x18);
                int bi = *(signed char*)(trickItem + 0x18);
                char* pi = (char*)cBE_getInterface_Fv(cBE_getBE(), 6);
                cBE_getInterface_Fv(cBE_getBE(), 0);
                if (func_00150048(pi, self->mUserNum, ai, bi) && !func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_66HELPCustomUber));
                } else if (D_00535C11[0] == 0) {
                    if (func_0014FFB8(pi, self->mUserNum, ai, bi)) {
                        char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
                        signed char charID = cBENewPlayerInterface_getPlayerCharID(player, self->mUserNum);
                        {
                            sVE_185268* vt = *(sVE_185268**)(player + 0xC);
                            ((void (*)(void*))vt[2].fn)(player + vt[2].delta);
                        }
                        int points = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), self->mUserNum, charID);
                        int n;
                        sTrick_185268* tbl = func_0014FF90(pi, self->mUserNum, ai, &n);
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
    case 2:
        *(int*)((char*)cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, self->mUserNum) + 0xCD0) = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the uber-trick screen's menu events: 0x100 for event 9 on the trick menu and for events 8 and 9 on the
 * category menu, otherwise 0x101.
 *
 * The trick menu is +0x50. onWidgetMenu slot (+0xA4) of the vtable D_0046CCC0.
 *
 * @param menu  the menu that asks
 * @param event  the menu event
 * @return 0x100 or 0x101 as above
 */
extern "C" int cFEStateUberTrick_onWidgetMenu(cFEStateUberTrick* self, int menu, unsigned int event)
{
    if (menu == (int)self->mpTrickMenu) {
        switch (event) {
        case 9:
            return 0x100;
        }
    } else {
        switch (event) {
        case 8:
        case 9:
            return 0x100;
        }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", cFEStateCredits_cFEStateCredits1);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateCredits_vtbl[];

/**
 * Constructs the credits state (allocated as "cFEOptionsCredits", 104 bytes).
 *
 * Runs cUIState's constructor (func_0039E2A0; the engine argument reaches it untouched in the second argument
 * register), stores cFEStateCredits_vtbl and mHashName 36, sets +0x60 to -100 and +0x64 to 1 (On Tour mPixelOffset,
 * mPixelDelta) and clears +0x48, +0x58 and +0x5C (On Tour mpWidget, mpDolbyIcon, mpTextScroll).
 *
 * @return @p self
 */
extern "C" void* cFEStateCredits_cFEStateCredits1(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateCredits_vtbl;
    *(int*)((char*)self + 0x60) = -100;
    *(int*)((char*)self + 0x64) = 1;
    *(int*)((char*)self + 0xC) = 36;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/festatestore", cFEStateCredits__cFEStateCredits__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the credits front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (D_0046D0D0), then calls the cUIState destructor func_0039E390(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach func_0039E390() untouched in the second argument register.
 *
 * @return no meaningful value (func_0039E390() returns nothing)
 */
void* cFEStateCredits__cFEStateCredits(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

