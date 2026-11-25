#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "bx/math3d.h"
#include "fe/ferider.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "fe/festateriderdetail.h"
#include "fe/festatecharsetup.h"
#include "ui/uiengine.h"
#include "ui/uithing.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onCreateScreen);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void cFEStateCharSetup_updateWidget(cFEStateCharSetup* self);
extern "C" void cFERiderManager_setHumanVisible(void* self, int i, int v);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int idx, int a2, int a3);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* snd, int a);
extern char str_09set_char[];
extern char str_title[];
extern char str_kT_TITLEPlayer1SetupCharacter[];
extern char str_kT_TITLEPlayer2SetupCharacter[];
extern char str_Option_Continue[];
extern char str_Option_Equip[];
extern char str_Option_Rider[];
extern char str_Option_Radio[];
extern char str_Option_GBA[];


static inline void showIdx_182928(cFEStateCharSetup* self, char* name, int idx)
{
    cUIThing* o = cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
    if (o != 0) {
        o->setVisible(1);
        o->mMiscData = idx;
    }
}

/**
 * Builds the character setup screen ("09set_char"): rider name and model, title and option buttons.
 *
 * Fills "rider name" and sets up the camera and rider pose (cFEStateCharSetup_updateWidget), hides player mUserNum's front-end
 * rider and probably gives it the player's character (rider manager, RealFE + 0xB0), and sets the audio bank
 * mode to 1. In multiplayer (game path newRace.cGamePath == 2) "title" says "kT_TITLEPlayer1SetupCharacter" or
 * "kT_TITLEPlayer2SetupCharacter".
 *
 * "Option_Continue", "Option_Equip", "Option_Rider" and "Option_Radio" are shown with misc data 0 to 3;
 * "Option_GBA" is hidden and gets vtable slot 8 (cUIThing_setStatic) with 1.
 */
extern "C" void cFEStateCharSetup_onCreateScreen(cFEStateCharSetup* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, (self), GetHashValue32(str_09set_char), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->updateWidget();
    cBEInterface* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int charId = cBENewPlayerInterface_getPlayerCharID(iface, self->mUserNum);
    iface->discard();
    cFERiderManager_setHumanVisible(&SSXApp->RealFE->mRiderManager, self->mUserNum, 0);
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, self->mUserNum, charId, 0);
    func_0028F140(cAudioInterface_bxAudio(), 1);
    cUIText* txt = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title));
    if (txt != 0) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == 2) {
            switch (self->mUserNum) {
            case 0:
                cUIText_setUnicodeStringByID(txt, GetHashValue32(str_kT_TITLEPlayer1SetupCharacter));
                break;
            case 1:
                cUIText_setUnicodeStringByID(txt, GetHashValue32(str_kT_TITLEPlayer2SetupCharacter));
                break;
            }
        }
    }
    showIdx_182928((self), str_Option_Continue, 0);
    showIdx_182928((self), str_Option_Equip, 1);
    showIdx_182928((self), str_Option_Rider, 2);
    showIdx_182928((self), str_Option_Radio, 3);
    cUIThing* gbaOption = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Option_GBA));
    if (gbaOption != 0) {
        gbaOption->setVisible(0);
        gbaOption->setStatic(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cFEStateCharSetup_show3DCharacter(cFEStateCharSetup* self, int a1);
void* cUIState_onDestroyScreen(void* self);

/**
 * Hides the user's 3D rider (cFEStateCharSetup_show3DCharacter with 0), then destroys the screens (cUIState_onDestroyScreen).
 *
 * It fills the onDestroyScreen slot (+0x2C) of cFEStateCharSetup_vtbl and cFEStateRiderDetail_vtbl.
 */
extern "C" void cFEStateCharSetup_onDestroyScreen(cFEStateCharSetup* self)
{
    self->show3DCharacter(0);
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cUIState_onUpdate(void* self);
extern "C" void cFEStateCharSetup_updateCharacter3DPos(cFEStateCharSetup* self);

/**
 * Updates the character-setup state: runs the base UI-state update, then poses the front-end rider model.
 *
 * cUIState_onUpdate() is the base UI-state update, which dispatches controller input to the state and its screens.
 * cFEStateCharSetup_updateCharacter3DPos() places the loaded rider model and, when the state asks for it, restarts its animation.
 */
extern "C" void cFEStateCharSetup_onUpdate(cFEStateCharSetup* self)
{
    cUIState_onUpdate(self);
    self->updateCharacter3DPos();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the character-setup menu's events: 0x100 for event 9, otherwise 0x101.
 *
 * It fills the onWidgetMenu slot (+0xA4) of cFEStateCharSetup_vtbl and cFEStateRiderDetail_vtbl.
 *
 * @param menu  the menu (unused)
 * @param event  the menu event
 * @return 0x100 for event 9, else 0x101
 */
extern "C" int cFEStateCharSetup_onWidgetMenu(cFEStateCharSetup* self, int menu, int event)
{
    return event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001887A0(void* self, void* engine, int reset);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* st, int a2, int a3);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void cFEStateCharSetup_show3DCharacter(cFEStateCharSetup* self, int a1);
extern char str_FEOptions[];


/**
 * Handles the character setup screen's widget events: next state, previous state, or the options screen.
 *
 * Event 5 pushes the custom interface's getNextState result, if any, and always hides the rider
 * (cFEStateCharSetup_show3DCharacter(self, 0)); event 6 does the same with slot 5 (probably getPrevState) but hides the rider
 * only when a state came back. Event 7 builds a cFEStateOptions (0x54 bytes, "FEOptions"), sets its mode
 * (mAttrState bits 8-13) to 1 and pushes it with cUIStateStack_pushSpecial(.., 0, 0).
 *
 * @param item  the widget (On Tour pWidget)
 * @param event  the event (On Tour Event)
 */
extern "C" void cFEStateCharSetup_onWidgetEvent(cFEStateCharSetup* self, cUIThing* item, unsigned int event)
{
    if (((void*)item) == 0)
        return;
    switch (event) {
    case 5: {
        cUICustomInterface* factory = (cUICustomInterface*)((char*)self->mpEngine->mpCustom);
        void* nextState = factory->getNextState((cUIState*)self, item->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        self->show3DCharacter(0);
        break;
    }
    case 6: {
        cUICustomInterface* factory = (cUICustomInterface*)((char*)self->mpEngine->mpCustom);
        void* nextState = factory->getPrevState((cUIState*)self, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
            self->show3DCharacter(0);
        }
        break;
    }
    case 7: {
        cUIState* st = (cUIState*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), self->mpEngine, 1);
        st->mAttrState = (st->mAttrState & ~0x3F00) | 0x100; // tAttrState mMode (bits 8-13) = 1
        cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, st, 0, 0);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_updateWidget);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void* cBECharacterInterface_getShortNameByCharacter(void* self, int player, int index);
extern "C" void cSimpleCamera_setupCamWithPosAndTarget(void* cam, t4Vector* a, t4Vector* b);
extern "C" void cFEStateCharSetup_show3DCharacter(cFEStateCharSetup* self, int a1);
extern "C" void cFEStateCharSetup_updateCharacter3DPos(cFEStateCharSetup* self);
extern char str_rider_name[];

/**
 * Shows the user's character name and sets up the front-end camera and rider for the character-setup screen.
 *
 * Sets the "rider_name" text to the name of the user's character (cBENewPlayerInterface_getPlayerCharID, then
 * cBECharacterInterface_getShortNameByCharacter on interface 2). Points the camera (cFE +0x10) from (0, 200, 0) at the origin (cSimpleCamera_setupCamWithPosAndTarget) with a
 * field of view of 0.436 (25 degrees), shows the rider (cFEStateCharSetup_show3DCharacter with 1), sets the restart-animation flag (+0x48)
 * and places the rider (cFEStateCharSetup_updateCharacter3DPos). Called by cFEStateCharSetup_onCreateScreen and
 * cFEStateRiderDetail_onCreateScreen.
 */
void cFEStateCharSetup::updateWidget()
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_rider_name));
    signed char cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), mUserNum);
    cUIText_setAsciiString(text, (const char*)cBECharacterInterface_getShortNameByCharacter(cBE_getInterface_Fv(cBE_getBE(), 2), cid, 0));
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
    show3DCharacter(1);
    mbIsFirst = 1;
    updateCharacter3DPos();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_updateCharacter3DPos);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int idx);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);
extern "C" void cFERider_speak(void* rider);
extern "C" void cFERider_setVisible(void* self, int a1);

// A t4Vector with EA's constructors (ours has none): the pose position and the rotation quaternions.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern t4Vector t4vect_XAxis;
extern t4Vector t4vect_YAxis;
extern t4Vector t4vect_ZAxis;
extern "C" void cFERider_setPose(void* rider, tPose* xf);
extern "C" void cFERider_setBoardPose(void* rider, tPose* xf);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4VectorXYZW QuatMul_182EC0(const t4Vector& a, const t4Vector& b)
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

static inline t4VectorXYZW AxisAngle_182EC0(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return t4VectorXYZW(s * axis.x, s * axis.y, s * axis.z, c);
}

/**
 * Places the user's front-end rider on the character-setup screen and restarts its animation when asked.
 *
 * Gets the user's rider (cFERiderManager_getHuman1). When it has a valid character loaded (id below 10, +0xCB8 and +0xCB4 set) and
 * the show-rider flag (+0x4C) is set: sets the rider pose (cFERider_setPose) to position (136, -250, -82) turned 100
 * degrees about Z, moves the board out of view to (0, 10000, 0) (cFERider_setBoardPose), and when +0x48 is set resets the rider
 * animation and plays animation 435, clears the rider's +0xCCC and +0x48 and calls cFERider_speak on the rider; finally
 * makes the rider visible (cFERider_setVisible).
 *
 */
void cFEStateCharSetup::updateCharacter3DPos()
{
    cFERider* rider = cFERiderManager_getHuman1(&SSXApp->RealFE->mRiderManager, mUserNum);
    int ok = 0;
    if (*(unsigned int*)rider < 10 && rider->mDoneLoadingTextures != 0) {
        ok = rider->mCompiledModel != 0;
    }
    if (ok && mbDraw3D != 0) {
        t4VectorXYZW pos;
        tPose xf;
        {
            t4VectorXYZW position;
            position.x = 136.0f;
            position.y = -250.0f;
            position.z = -82.0f;
            position.w = 1.0f;
            pos = position;
            xf.P = position;
            xf.Q.x = 0.0f;
            xf.Q.w = 1.0f;
            xf.Q.y = 0.0f;
            xf.Q.z = 0.0f;
            xf.Q = QuatMul_182EC0(xf.Q, AxisAngle_182EC0(t4vect_XAxis, 0.0f));
        }
        xf.Q = QuatMul_182EC0(xf.Q, AxisAngle_182EC0(t4vect_YAxis, 0.0f));
        xf.Q = QuatMul_182EC0(xf.Q, AxisAngle_182EC0(t4vect_ZAxis, 1.7453293800354004f));
        cFERider_setPose(rider, &xf);
        {
            t4VectorXYZW position;
            position.x = 0.0f;
            position.y = 10000.0f;
            position.z = 0.0f;
            position.w = 1.0f;
            xf.P = position;
        }
        cFERider_setBoardPose(rider, &xf);
        if (mbIsFirst != 0) {
            void* anim = rider->mpRiderAnim;
            if (anim != 0) {
                cRiderAnimBase_reset(anim);
                cRiderAnimBase_play(anim, 435, 0, -1.0f);
            }
            rider->mAnimateBoard = 0;
            mbIsFirst = 0;
            cFERider_speak(rider);
        }
        cFERider_setVisible(rider, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_show3DCharacter);
#ifdef SKIP_ASM
extern "C" cFERider* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_setVisible(void* self, int a1);

/**
 * Turns the character-setup screen's 3D rider on or off.
 *
 * Does nothing without a front end. Off hides the user's rider (cFERider_setVisible with 0) and sets the restart-animation
 * flag (+0x48); either way the show-rider flag (+0x4C) becomes @p on. Turning it on does not show the rider directly;
 * cFEStateCharSetup_updateCharacter3DPos does once the model is loaded.
 *
 * @param on  nonzero to show the rider
 */
void cFEStateCharSetup::show3DCharacter(int on)
{
    cFE* mgr = SSXApp->RealFE;
    if (((char*)mgr) != 0) {
        void* rider = cFERiderManager_getHuman1(&mgr->mRiderManager, mUserNum);
        if (on == 0) {
            cFERider_setVisible(rider, 0);
        }
        mbDraw3D = on;
        if (on == 0) {
            mbIsFirst = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", cFEStateRiderDetail_cFEStateRiderDetail);
#ifdef SKIP_ASM
extern "C" void* cFEStateCharSetup_cFEStateCharSetup(void* self, void* engine, signed char idx);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern void* cFEStateRiderDetail_vtbl[];

/**
 * Constructs the rider-detail state for a user on top of the character-setup state.
 *
 * Runs the character-setup constructor (cFEStateCharSetup_cFEStateCharSetup), then sets state id 13, the cFEStateRiderDetail_vtbl vtable, the
 * user (+0x44), cFECustom_getPlayerCtrlMask's value for the user at +0x15 and clears the show-rider flag (+0x4C).
 *
 * @param engine  the UI engine
 * @param idx  the user (controller) index
 * @return @p self
 */
extern "C" void* cFEStateRiderDetail_cFEStateRiderDetail(cFEStateRiderDetail* self, cUIEngine* engine, signed char idx)
{
    cFEStateCharSetup_cFEStateCharSetup(self, engine, idx);
    self->mHashName = 13;
    VPTR_OF(cListNode, self) = cFEStateRiderDetail_vtbl;
    self->mUserNum = idx;
    self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, idx);
    self->mbDraw3D = 0;
    return ((void*)self);
}
#endif

