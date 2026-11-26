#include "vtable.h"
#include "common.h"
#include "ui/uithing.h"
#include "ui/uiscreen.h"
#include "be/beinterface.h"
#include "main/gamestate.h"
#include "ui/uiengine.h"
#include "movie/mcoverlaymanager.h"
#include "fe/ovstate.h"
#include "sound/ssxaudio.h"
#include "bx/appman.h"
#include "ui/uistate.h"
#include "be/beintnewrace.h"
#include "fe/ovstateheat.h"
#include "fe/ovstaterewards.h"
#include "fe/ovstatepause.h"
#include "fe/ovstatesavereplay.h"
#include "fe/ovstatesaveoptions.h"
#include "online/lobbyman.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ui/uitext.h"
#include "util/bxmemcard.h"
#include "util/locale.h"
#include "ui/uislider.h"
#include "ui/uilistbox.h"
#include "ui/uimenu.h"
#include "main/gameviewman.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int idx);
extern "C" int cUIListBox_getEntryByData(void* box, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* box, unsigned char sel);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_00266F90(void* self, int* a, int* b);
extern "C" void func_00266E88(void* self);
extern "C" void func_00266E30(void* self);
extern void* VoipMan;
extern char str_37beoptions[];
extern char str_kT_CMNHUDFull[];
extern char str_kT_CMNHUDMinimal[];
extern char str_kT_CMNHUDNone_b[];
extern char str_kT_OVRCMNNear[];
extern char str_kT_OVRCMNMid[];
extern char str_kT_OVRCMNFar[];
extern char str_camera_select[];
extern char str_kT_CMNCameraVar[];
extern char str_camera_2_select[];
extern char str_kT_CMNOff_b[];
extern char str_kT_CMNOn_b[];
extern char str_EATalkPair[];
extern char str_EATalksl[];
extern char str_0lb[];
extern char str_1lb_b[];
extern char str_2lb_b[];
extern char str_4sl_b[];
extern char str_5sl[];
extern char str_6sl[];
extern char str_7lb_b[];
extern char str_8lb[];
extern char str_EATalk[];

// The pause-options settings word (same layout as cOVState_PAUSE_OPTIONS_onWidgetEvent's view).
struct sOptions_PO {
    unsigned int f0 : 4;
    unsigned int f4 : 4;
    unsigned int f8 : 4;
    unsigned int pad12 : 4;
    int iUseArcadeSFX : 1;
    int iUseDJSpeech : 1;
    unsigned int pad18 : 12;
    unsigned int f30 : 2;
    int data[0x288 / 4 - 1];
};

// The options interface's view of the same word (unsigned flag bits).
struct sOptionsU_PO {
    unsigned int f0 : 4;
    unsigned int f4 : 4;
    unsigned int f8 : 4;
    unsigned int pad12 : 4;
    unsigned int iUseArcadeSFX : 1;
    unsigned int iUseDJSpeech : 1;
    unsigned int pad18 : 12;
    unsigned int f30 : 2;
    int data[0x288 / 4 - 1];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sOptions_PO cBEOptionInterface_mOption_PO __asm__("cBEOptionInterface_mOption");

struct cBEOptionInterface_PO {
    // PORT: reinterprets the settings global with unsigned flag bits
    sOptionsU_PO getOptions() { return *(sOptionsU_PO*)&cBEOptionInterface_mOption_PO; }
};

static inline sOptions_PO Cfg_PO()
{
    return cBEOptionInterface_mOption_PO;
}

extern tARGB gStaticColour_PO __asm__("gStaticColour");

class cUIObj_PO {
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

struct sVE_PO {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

// Looks up a localized string by the hash of its name.
static inline unsigned short* getString_PO(char* name)
{
    cFELocale* loc = SSXApp->FELocale;
    sVE_PO* vt = (sVE_PO*)VPTR_OF(cUILocaleInterface, loc);
    void* thisp = vthis(loc, vt[4]);
    return vt[4].fn(thisp, GetHashValue32(name));
}

static inline void* find_PO(cUIState* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

struct sText_PO {
    unsigned short buf[0x140];
    operator unsigned short*() { return buf; }
};

static inline void setFormatted_PO(void* obj, char* name, int n)
{
    sText_PO text;
    unsigned short* s = getString_PO(name);
    if (s != 0) {
        sprintf1(text, s, n);
        cUIText_setUnicodeString(obj, text);
    }
}

static inline cGameViewMan* views_PO()
{
    return SSXApp->Game->ViewMan;
}

/**
 * Builds the in-game options screen ("37beoptions") and loads every widget from the current settings.
 *
 * List boxes: HUD (0lb: Full / Minimal / None, settings word cBEOptionInterface_mOption bits 30-31), player 1 and 2 camera
 * (1lb / 2lb: Near / Mid / Far = camera types 0x3C-0x3E, preselected from each game view's chase camera;
 * "camera select" / "camera 2 select" get kT_CMNCameraVar with 1 / 2), DJ and arcade audio (7lb / 8lb: Off /
 * On, bits 17 / 16). The 12-step sliders 4sl / 5sl / 6sl take the music, effects and speech volumes (bits
 * 0-3, 8-11, 4-7), and all five audio settings are reapplied to the audio object.
 * In a network game the EA Talk widgets (EATalkPair, EATalk, EATalksl) are shown and made selectable, and the
 * voice slider (0..10) is loaded from func_00266F90's level / 10, calling func_00266E88 below level 10, else
 * func_00266E30 (probably voice off / on); otherwise EATalk is hidden and made static in its static colour.
 * setStatic / setUseStaticColour are vtable slots 8 / 25 (cUIThing_setStatic / cUIText_setUseStaticColour).
 */
extern "C" void cOVState_PAUSE_OPTIONS_onCreateScreen(cOVState_PAUSE_OPTIONS* self)
{
    unsigned char sel;
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_37beoptions), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cBEOptionInterface_PO* iface = (cBEOptionInterface_PO*)cBE_getInterface_Fv(cBE_getBE(), 4);
    {
        void* w = find_PO(self, str_0lb);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNHUDFull), 0);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNHUDMinimal), 1);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNHUDNone_b), 2);
            if (cUIListBox_getEntryByData(w, iface->getOptions().f30, &sel))
                cUIListBox_setCurrentEntryByIndex(w, sel);
        }
    }
    {
        void* w = find_PO(self, str_1lb_b);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNNear), 0x3C);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNMid), 61);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNFar), 0x3E);
            cCamera* camera = views_PO()->PlayerCamera[0];
            if (cUIListBox_getEntryByData(w, camera->ChaseController->LatestAlgoType, &sel))
                cUIListBox_setCurrentEntryByIndex(w, sel);
            void* cameraLabel = find_PO(self, str_camera_select);
            if (cameraLabel != 0) {
                setFormatted_PO(cameraLabel, str_kT_CMNCameraVar, 1);
            }
        }
    }
    {
        void* w = find_PO(self, str_2lb_b);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNNear), 0x3C);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNMid), 61);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_OVRCMNFar), 0x3E);
            cGameViewMan* views = views_PO();
            if (views->m_uiNumView >= 2) {
                cCamera* camera = views->PlayerCamera[1];
                if (cUIListBox_getEntryByData(w, camera->ChaseController->LatestAlgoType, &sel))
                    cUIListBox_setCurrentEntryByIndex(w, sel);
            }
            void* cameraLabel = find_PO(self, str_camera_2_select);
            if (cameraLabel != 0) {
                setFormatted_PO(cameraLabel, str_kT_CMNCameraVar, 2);
            }
        }
    }
    {
        cUISlider* w = (cUISlider*)find_PO(self, str_4sl_b);
        if (w != 0) {
            w->mNumValues = 12;
            w->mValue = iface->getOptions().f0;
            func_00287410(cAudioInterface_bxAudio(), Cfg_PO().f0);
        }
    }
    {
        cUISlider* w = (cUISlider*)find_PO(self, str_5sl);
        if (w != 0) {
            w->mNumValues = 12;
            w->mValue = iface->getOptions().f8;
            cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_PO().f8);
        }
    }
    {
        cUISlider* w = (cUISlider*)find_PO(self, str_6sl);
        if (w != 0) {
            w->mNumValues = 12;
            w->mValue = iface->getOptions().f4;
            func_00287520(cAudioInterface_bxAudio(), Cfg_PO().f4);
        }
    }
    {
        void* w = find_PO(self, str_7lb_b);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOff_b), 0);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOn_b), 1);
            unsigned char sel2;
            if (cUIListBox_getEntryByData(w, iface->getOptions().iUseDJSpeech, &sel2))
                cUIListBox_setCurrentEntryByIndex(w, sel2);
            cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_PO().iUseDJSpeech != 0);
        }
    }
    {
        void* w = find_PO(self, str_8lb);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOff_b), 0);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOn_b), 1);
            unsigned char sel3;
            if (cUIListBox_getEntryByData(w, iface->getOptions().iUseArcadeSFX, &sel3))
                cUIListBox_setCurrentEntryByIndex(w, sel3);
            cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_PO().iUseArcadeSFX != 0);
        }
    }
    cUIObj_PO* eaTalkPair = (cUIObj_PO*)find_PO(self, str_EATalkPair);
    if (eaTalkPair != 0) {
        eaTalkPair->setVisible(0);
        eaTalkPair->setStatic(1);
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cUIObj_PO* talkPair = (cUIObj_PO*)find_PO(self, str_EATalkPair);
        cUIObj_PO* talkItem = (cUIObj_PO*)find_PO(self, str_EATalk);
        cUIObj_PO* talkSlider = (cUIObj_PO*)find_PO(self, str_EATalksl);
        if (talkPair != 0 && talkItem != 0 && talkSlider != 0) {
            talkPair->setVisible(1);
            talkPair->setStatic(0);
            talkItem->setVisible(1);
            talkItem->setStatic(0);
            talkSlider->setVisible(1);
            talkSlider->setStatic(0);
            ((cUISlider*)talkSlider)->mNumValues = 10;
            int level;
            int max;
            func_00266F90(VoipMan, &level, &max);
            ((cUISlider*)talkSlider)->mValue = level / 10;
            if (level < 10) {
                func_00266E88(VoipMan);
            } else {
                func_00266E30(VoipMan);
            }
        }
    } else {
        cUIObj_PO* eaTalk = (cUIObj_PO*)find_PO(self, str_EATalk);
        if (eaTalk != 0) {
            eaTalk->setUseStaticColour(1);
            ((cUIText*)eaTalk)->mStaticColour = gStaticColour_PO;
            eaTalk->setStatic(1);
            eaTalk->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onPreRender2D);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern char D_004A24E0[];
extern char str_camera_2_select[];
extern char str_2lb_b[];
extern char str_save[];

static inline bool isField_FA100(signed char* g, int off, int v)
{
    return g[off] == v;
}

/**
 * Greys out the in-game options that do not apply to the current game, every frame before the 2D render.
 *
 * Outside a local two-player game (a network game, or newRace cGamePath != 2) object "2" is made static (slot
 * 8, setStatic) and "camera 2 select" / "2lb" are greyed out (DisableTextWidget: static, alpha at most
 * 0.5). "save" is greyed out unless cGameMode is 4 (free ride in the game-flow notes).
 */
void cOVState_PAUSE_OPTIONS::onPreRender2D()
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 || !isField_FA100((signed char*)&cBENewRaceInterface_newRace, 73, 2)) {
        cUIThing* pair2 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(D_004A24E0));
        if (pair2 != 0) {
            pair2->setStatic(1);
        }
        void* camSelect2 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_camera_2_select));
        if (camSelect2 != 0) {
            DisableTextWidget(camSelect2);
        }
        void* label2 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_2lb_b));
        if (label2 != 0) {
            DisableTextWidget(label2);
        }
    }
    if (!isField_FA100((signed char*)&cBENewRaceInterface_newRace, 72, 4)) {
        void* saveItem = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_save));
        if (saveItem != 0) {
            DisableTextWidget(saveItem);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cOVState_PROFILE_cOVState_PROFILE(void* self, void* engine, unsigned char a2, int a3);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern "C" int cUIListBox_getCurrentEntryData(void* obj);
extern "C" void cChaseCameraController_switchCamera_UserChange(void* a0, int a1, void* a2);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_00266F90(void* self, int* a, int* b);
extern "C" void func_00266F58(void* self, int a, int b);
extern "C" void func_00266E88(void* self);
extern "C" void func_00266E30(void* self);
extern void* VoipMan;
extern char str_EATalksl[];
extern char str_Save_Profile[];
extern char str_0lb[];
extern char str_1lb_b[];
extern char str_2lb_b[];
extern char str_4sl_b[];
extern char str_5sl[];
extern char str_6sl[];
extern char str_7lb_b[];
extern char str_8lb[];
extern char str_save[];

struct sOptions_A238 {
    unsigned int f0 : 4;
    unsigned int f4 : 4;
    unsigned int f8 : 4;
    unsigned int pad12 : 4;
    int iUseArcadeSFX : 1;
    int iUseDJSpeech : 1;
    unsigned int pad18 : 12;
    unsigned int f30 : 2;
    int data[0x288 / 4 - 1];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sOptions_A238 cBEOptionInterface_mOption_A238 __asm__("cBEOptionInterface_mOption");

struct sVE_A238 {
    short delta;
    short index;
    void (*fn)(void*);
};

// The options interface (kind 4).
struct cBEOptionInterface_A238 {
    char pad[0xC];
    sVE_A238* vtbl;
    sOptions_A238 getOptions() { return cBEOptionInterface_mOption_A238; }
};

static inline void SetF0_A238(unsigned char v) { cBEOptionInterface_mOption_A238.f0 = v; }
static inline void SetF4_A238(unsigned char v) { cBEOptionInterface_mOption_A238.f4 = v; }
static inline void SetF8_A238(unsigned char v) { cBEOptionInterface_mOption_A238.f8 = v; }

static inline bool IsId_A238(cUIThing* obj, char* key)
{
    int id = obj->mHashName;
    return id == GetHashValue32(key);
}

static inline cCamera* Camera_A238(int view)
{
    return SSXApp->Game->ViewMan->PlayerCamera[view];
}

/**
 * Handles the in-game options widgets: Save opens the profile save, Triangle closes the screen, and a changed
 * value is stored and applied at once.
 *
 * Cross (kONNEXT) on "save" pushes a new 0x27C-byte "Save Profile" state (cOVState_PROFILE_cOVState_PROFILE, kind 0x2A)
 * as a special state; Triangle (kONPREV) closes this state (cUIStateStack_pop).
 * A value change (kONCHANGE) writes the widget's value into the settings word cBEOptionInterface_mOption and applies
 * it: HUD mode, player 1 / 2 camera (cChaseCameraController_switchCamera_UserChange on game view 1 / 2), music /
 * effects / speech volume, DJ and arcade audio; in a network game EATalksl sets the voice level (value x 10). The
 * options interface's virtual slot 1 then runs, probably to commit the settings.
 *
 * @param obj  the widget (On Tour pWidget); nothing happens when it is null
 * @param msg  the widget event: kONNEXT Cross, kONPREV Triangle, kONCHANGE value changed
 */
// Pause-options widget handler: kONNEXT opens the sub-state, kONPREV pops, kONCHANGE applies a changed setting.
void cOVState_PAUSE_OPTIONS::onWidgetEvent(cUIThing* obj, unsigned int msg)
{
    if (obj == 0) {
        return;
    }
    cBEOptionInterface_A238* iface = (cBEOptionInterface_A238*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (msg) {
    case cUIState::kONNEXT:
        if (IsId_A238(obj, str_save)) {
            cUIEngine* stack = SSXApp->Game->UI;
            cUIState* st = (cUIState*)cOVState_PROFILE_cOVState_PROFILE(cMemMan_alloc(636, str_Save_Profile, 0x100, 0),
                                                                        mpEngine,
                                                                        *(unsigned char*)&mCurrentCtrl, 42);
            st->mAttrState = (st->mAttrState & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(&stack->mStateStack, st, 0, 0);
        }
        break;
    case cUIState::kONPREV:
        cUIStateStack_pop(&mpEngine->mStateStack, 1);
        break;
    case cUIState::kONCHANGE:
        if (IsId_A238(obj, str_0lb)) {
            cBEOptionInterface_mOption_A238.f30 = ((cUIListBox*)obj)->mCurrentEntry;
        } else if (IsId_A238(obj, str_1lb_b)) {
            int on = cUIListBox_getCurrentEntryData(obj);
            cCamera* camera = Camera_A238(0);
            cChaseCameraController_switchCamera_UserChange(camera->ChaseController, on, camera);
        } else if (IsId_A238(obj, str_2lb_b)) {
            int on = cUIListBox_getCurrentEntryData(obj);
            cCamera* camera = Camera_A238(1);
            cChaseCameraController_switchCamera_UserChange(camera->ChaseController, on, camera);
        } else if (IsId_A238(obj, str_4sl_b)) {
            SetF0_A238(((cUISlider*)obj)->mValue);
            func_00287410(cAudioInterface_bxAudio(), iface->getOptions().f0);
        } else if (IsId_A238(obj, str_5sl)) {
            SetF8_A238(((cUISlider*)obj)->mValue);
            cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), iface->getOptions().f8);
        } else if (IsId_A238(obj, str_6sl)) {
            SetF4_A238(((cUISlider*)obj)->mValue);
            func_00287520(cAudioInterface_bxAudio(), iface->getOptions().f4);
        } else if (IsId_A238(obj, str_7lb_b)) {
            cBEOptionInterface_mOption_A238.iUseDJSpeech = cUIListBox_getCurrentEntryData(obj) != 0;
            cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), iface->getOptions().iUseDJSpeech != 0);
        } else if (IsId_A238(obj, str_8lb)) {
            cBEOptionInterface_mOption_A238.iUseArcadeSFX = cUIListBox_getCurrentEntryData(obj) != 0;
            cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), iface->getOptions().iUseArcadeSFX != 0);
        }
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && IsId_A238(obj, str_EATalksl)) {
            int level;
            int max;
            func_00266F90(VoipMan, &level, &max);
            level = ((cUISlider*)obj)->mValue * 10;
            func_00266F58(VoipMan, level, max);
            if (level < 10) {
                func_00266E88(VoipMan);
            } else {
                func_00266E30(VoipMan);
            }
        }
        iface->vtbl[1].fn(vthis(iface, iface->vtbl[1]));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onUpdate);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cOVState_onUpdate(void* self);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern char str_TransitionOut_b[];

/**
 * Closes the in-game options when an online pause or overlay timeout runs out.
 *
 * After cOVState_onUpdate(), if a cGameComm exists and its mPauseTimeout or mOverlayTimeout is 0, plays the
 * screen's "TransitionOut" frame and closes the state (cUIStateStack_pop).
 */
void cOVState_PAUSE_OPTIONS::onUpdate()
{
    cOVState_onUpdate(this);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        int ok = gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_TransitionOut_b));
            if (frame != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, (unsigned short)frame, 1);
            }
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onWidgetMenu);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cList_getByIndex(void* list, int index);
extern char str_save[];

/**
 * Tells the options menu how to react to a button: the press animation alone, or with the item's sound.
 *
 * Cross plays the sound only on the "save" item; Square and Circle never play it.
 *
 * @param menu  the options menu (On Tour pMenu)
 * @param key  the menu event: kONNEXT Cross, kONPREVIOUS Triangle, kONOPTION Square, kONMISC Circle
 *             (kISHIGHLIGHTABLE asks whether row @p index can be selected)
 * @param index  the visible row the event is about
 * @return 0x100 (animation only) for Square, Circle and Cross on any item but "save", else 0x101 (animation
 *         and sound)
 */
// Pause-menu key filter: returns 0x100 for kONOPTION / kONMISC and for kONNEXT unless the item is str_save, else 0x101.
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetMenu(cOVState_PAUSE_OPTIONS* self, cUIMenu* menu, unsigned int key,
                                                   int index)
{
    switch (key) {
    case cUIMenu::kONNEXT: {
        int id = ((cUIThing*)cList_getByIndex(&menu->mList, index))->mHashName;
        if (id == GetHashValue32(str_save)) {
            break;
        }
    }
    case cUIMenu::kONOPTION:
    case cUIMenu::kONMISC:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onWidgetListBox);
#ifdef SKIP_ASM
/**
 * Tells a list box how to react to input: Triangle animates silently, moves also play a sound.
 *
 * @param pListBox  the list box (unused)
 * @param Event  the list-box event (tUILBEvent): kONLEFT .. kONDOWN moves, kONPREVIOUS Triangle
 * @return 0x100 (animation only) for kONPREVIOUS, else 0x101 (animation and sound)
 */
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetListBox(cOVState_PAUSE_OPTIONS* self, int pListBox, int Event)
{
    return Event != cUIListBox::kONPREVIOUS ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onWidgetPair);
#ifdef SKIP_ASM
/**
 * Tells a pair widget how to react to input: Cross animates silently, Triangle also plays a sound.
 *
 * @param pPair  the pair widget (unused)
 * @param Event  the pair event: 0 Cross, 1 Triangle
 * @return 0x100 (animation only) for event 0, else 0x101 (animation and sound)
 */
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetPair(cOVState_PAUSE_OPTIONS* self, int pPair, int Event)
{
    return Event != 0 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_enableCancelButton(cOVState_PAUSE_ONLINE_ERROR* self, bool on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(cOVState_PAUSE_ONLINE_ERROR* self, int visible);
extern "C" void func_002583A8(void* self, int a1, int a2);
extern char str_265InGameError[];
extern char str_SubTitle[];
extern char str_kT_OVROnlineError[];
extern char str_TextError[];
extern char str_kT_POPUPCMNExcessiveDelay[];
extern char str_kT_POPUPCMNCheatDetected[];
extern char str_kT_POPUPCMNConnectionLost_b[];
extern char str_kT_POPUPCMNOpponentQuit[];
extern char str_Text_CancelGame[];
extern char str_kT_BTNOK_b[];
extern char str_kT_OVRCancelEvent[];
extern char str_Cancelps2X[];
extern char str_Continueps2X[];
extern char str_CancelButtonBack[];
extern char str_ContinueButtonBack[];
extern char str_TextInfo[];
extern char str_kT_PauseErrorCheckingServer[];

/**
 * Builds the online-error pause screen ("265InGameError"), picks the error text and starts checking the
 * server.
 *
 * TextError (word-wrapped) explains the cGameComm error: this machine's mCheating[0] kCheatDiscTrayOpened ..
 * kCheatTooManyUnplugs gives ExcessiveDelay and other values CheatDetected; the peer's mCheating[1], or no flag at all,
 * ConnectionLost; mRemoteEndGame or mRemoteDisconnected OpponentQuit. Every case but the first clears
 * mbShouldDisplayAuxInfo, the flag that gates the ping texts. TextError's attribute bit 0x80 (mAttr) is set.
 * Text_CancelGame reads kT_BTNOK after the local finish, else kT_OVRCancelEvent (choice 0); it, the four button
 * graphics and the continue option start hidden. With a cGameComm it pings the lobby server for 3 s (func_002583A8),
 * stores the start time in miStartPingTick, hides Cancel (cOVState_PAUSE_ONLINE_ERROR_enableCancelButton) and shows
 * kT_PauseErrorCheckingServer; without one it shows Cancel. The TextError branch reads cGameComm_mGameComm before that
 * null check.
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onCreateScreen(cOVState_PAUSE_ONLINE_ERROR* self)
{
    void* eng = self->mpEngine;
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_265InGameError), 0);
    self->mpScreen = (cUIScreen*)scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cUIText* subTitle = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_SubTitle));
    if (subTitle != 0) {
        cUIText_setUnicodeStringByID(subTitle, GetHashValue32(str_kT_OVROnlineError));
    }
    cUIText* errorText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextError));
    if (errorText != 0) {
        errorText->setVisible(1);
        cGameComm* gameComm = cGameComm_mGameComm;
        errorText->mAttr |= 0x80;
        int localCheating = gameComm->mCheating[0];
        int one = 1;
        if (localCheating != 0) {
            if (localCheating == one) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else if (localCheating == cGameComm::kCheatTooManyPauses) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else if (localCheating == cGameComm::kCheatTooManyUnplugs) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNCheatDetected));
            }
        } else if (gameComm->mCheating[1] != 0) {
            self->mbShouldDisplayAuxInfo = 0;
            int peerCheating = gameComm->mCheating[1];
            if (peerCheating == one) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else if (peerCheating == cGameComm::kCheatTooManyPauses) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else if (peerCheating == cGameComm::kCheatTooManyUnplugs) {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else {
                cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            }
        } else if (gameComm->mRemoteEndGame != 0) {
            cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNOpponentQuit));
            self->mbShouldDisplayAuxInfo = 0;
        } else if (gameComm->mRemoteDisconnected != 0) {
            self->mbShouldDisplayAuxInfo = 0;
            cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNOpponentQuit));
        } else {
            self->mbShouldDisplayAuxInfo = 0;
            cUIText_setUnicodeStringByID(errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
        }
        cUIText* widget = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Text_CancelGame));
        if (widget != 0) {
            if (cGameComm_mGameComm->mLocalFinishedRace != 0) {
                cUIText_setUnicodeStringByID(widget, GetHashValue32(str_kT_BTNOK_b));
            } else {
                cUIText_setUnicodeStringByID(widget, GetHashValue32(str_kT_OVRCancelEvent));
            }
            widget->setVisible(0);
            widget->mMiscData = 0;
        }
        widget = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Cancelps2X));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Continueps2X));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_CancelButtonBack));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ContinueButtonBack));
        if (widget != 0) {
            widget->setVisible(0);
        }
        self->setContinueOptionVisible(0);
    }
    if (cGameComm_mGameComm != 0) {
        func_002583A8(cGameComm_mGameComm, LobbyMan_mLobbyMan->mServerAddr, 3);
        self->miStartPingTick = (unsigned int)(cAppMan_TheAppMan->TimerTick);
        self->enableCancelButton(0);
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextInfo));
        if (infoText != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorCheckingServer));
        }
    } else {
        self->enableCancelButton(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the online-error pause overlay's gain transition at once.
 *
 * The state stack update cUIStateStack_updateLoop() ends a state's gain transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cOVState::PAUSE_ONLINE_ERROR_onGainTransition()
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cOVState_onUpdate(void* self);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_enableCancelButton(cOVState_PAUSE_ONLINE_ERROR* self, bool on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(cOVState_PAUSE_ONLINE_ERROR* self);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(cOVState_PAUSE_ONLINE_ERROR* self);
extern char str_TextInfo[];

/**
 * Per-frame update of the online error overlay: reports the result of the connection ping once it is known.
 *
 * Calls cOVState_onUpdate(). While a ping is pending (+0x9C) and cGameComm_mGameComm exists: if its timer (mPingTimer)
 * has run out with no reply (mPingReceived == 0), it clears the comm's mPingResult and shows the timed-out message
 * (cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut()). If a reply came, it shows the received message
 * (cOVState_PAUSE_ONLINE_ERROR_displayPingReceived()) when a race is in progress, the local rider has not finished and
 * mCheating[0] is clear, else it hides the "TextInfo" object. In both cases it then calls cOVState_PAUSE_ONLINE_ERROR_enableCancelButton(self, 1)
 * and clears +0x9C; with no reply and time left it waits.
 */
void cOVState_PAUSE_ONLINE_ERROR::onUpdate()
{
    cOVState_onUpdate(this);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && (int)miStartPingTick != 0) {
        int timedOut = gameComm->mPingTimer <= 0 && gameComm->mPingReceived == 0;
        if (timedOut) {
            gameComm->mPingResult = 0;
            displayPingTimedOut();
        } else {
            if (gameComm->mPingReceived == 0) {
                return;
            }
            int received = gameComm->mRaceInProgress != 0 && gameComm->mLocalFinishedRace == 0 && gameComm->mCheating[0] == 0;
            if (received) {
                displayPingReceived();
            } else {
                cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_TextInfo))->setVisible(0);
            }
        }
        enableCancelButton(1);
        miStartPingTick = (unsigned int)0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_enableCancelButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Text_CancelGame[];
extern char str_Menu_b[];
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int index);

class cUIObj_1FAFF8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void setFocus(int v);
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Shows or hides the online-error overlay's "Cancel game" option.
 *
 * Sets the "Text_CancelGame" text visible and live (setVisible(@p on), setStatic(!@p on)). When showing it, also gives
 * the "Menu_b" menu the focus (setFocus(1)) and selects its entry 1 (cUIMenu_setSelectedByIndex). The online-error
 * overlay's cOVState_PAUSE_ONLINE_ERROR_onCreateScreen and cOVState_PAUSE_ONLINE_ERROR_onUpdate call it.
 *
 * @param on  true to show and select the option, false to hide it and make it static
 */
void cOVState_PAUSE_ONLINE_ERROR::enableCancelButton(bool on)
{
    cUIObj_1FAFF8* cancelText = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(mpScreen,
            GetHashValue32(str_Text_CancelGame));
    cancelText->setVisible(on);
    cancelText->setStatic(!on);
    if (on) {
        cUIObj_1FAFF8* menuWidget = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Menu_b));
        menuWidget->setFocus(1);
        cUIMenu_setSelectedByIndex(menuWidget, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_doContinueCancel(cOVState_PAUSE_ONLINE_ERROR* self, cUIThing* item,
                                                             int msg);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight(cOVState_PAUSE_ONLINE_ERROR* self, int on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight(cOVState_PAUSE_ONLINE_ERROR* self, int on);
extern char str_Text_CancelGame[];
extern char str_Text_ContinueGame[];

/**
 * Online pause error overlay: handles its Continue/Cancel widgets.
 *
 * Ignores a null widget. kONNEXT (select) runs cOVState_PAUSE_ONLINE_ERROR_doContinueCancel; kONGAINFOCUS (focus)
 * highlights the widget gaining focus (Text_CancelGame or Text_ContinueGame) and unhighlights the other with
 * cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight/cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight.
 *
 * @param item  the widget
 * @param msg   the event (tUIStateEvent)
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onWidgetEvent(cOVState_PAUSE_ONLINE_ERROR* self, cUIThing* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case cUIState::kONNEXT:
        self->doContinueCancel(item, msg);
        break;
    case cUIState::kONGAINFOCUS: {
        int id = item->mHashName;
        if (id == GetHashValue32(str_Text_CancelGame)) {
            self->setCancelHighLight(1);
            self->setContinueHighLight(0);
        } else {
            int id2 = item->mHashName;
            if (id2 == GetHashValue32(str_Text_ContinueGame)) {
                self->setContinueHighLight(1);
                self->setCancelHighLight(0);
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_TextInfo[];
extern char str_kT_PauseErrorServerCheckTimedOut[];

/**
 * Reports that the server check timed out: TextInfo gets kT_PauseErrorServerCheckTimedOut.
 *
 * Called by the overlay update (cOVState_PAUSE_ONLINE_ERROR_onUpdate) when the ping runs out with no reply. The text is set only
 * while +0xA0 is set, and TextInfo's visibility is set to +0xA0 without a null check on the object.
 */
void cOVState_PAUSE_ONLINE_ERROR::displayPingTimedOut()
{
    if (mpScreen != 0) {
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_TextInfo));
        if (infoText != 0 && mbShouldDisplayAuxInfo != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorServerCheckTimedOut));
        }
        infoText->setVisible(mbShouldDisplayAuxInfo);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingReceived);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(cOVState_PAUSE_ONLINE_ERROR* self, int visible);
extern char str_TextInfo[];
extern char str_kT_PauseErrorOpponentDisconnected[];

/**
 * Reports that the server answered, so the opponent dropped: shows kT_PauseErrorOpponentDisconnected and the
 * continue option.
 *
 * Called by cOVState_PAUSE_ONLINE_ERROR_onUpdate when a reply came while the race runs, the local rider has not finished and this
 * machine is not flagged. The text is set only while +0xA0 is set; TextInfo's visibility becomes +0xA0 (no
 * null check), then setContinueOptionVisible(self, 1).
 */
void cOVState_PAUSE_ONLINE_ERROR::displayPingReceived()
{
    if (mpScreen != 0) {
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_TextInfo));
        if (infoText != 0 && mbShouldDisplayAuxInfo != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorOpponentDisconnected));
        }
        infoText->setVisible(mbShouldDisplayAuxInfo);
        setContinueOptionVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_doContinueCancel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGame_exit(void* game, int a);
extern "C" void cAI_finishRemotePlayer(void* p);
// cUIStateStack_pop returns its last list node in $v0; the unit declares it void.
void* cUIStateStack_pop_r(void* self, int a1) __asm__("cUIStateStack_pop");
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];

/**
 * Online pause error overlay: continues the race without the lost player, or quits it.
 *
 * Pops the overlay off the UI stack and sets this state's transition bits; widget id eOptionContinue (its mMiscData)
 * finishes the remote player with cAI_finishRemotePlayer and restores the previous pause flags from the pause stack,
 * any other widget exits the game with cGame_exit(0).
 *
 * @param item  the widget that was selected
 * @param msg   the event (not read)
 */
void cOVState_PAUSE_ONLINE_ERROR::doContinueCancel(cUIThing* item, int msg)
{
    cUIStateStack_pop_r(&mpEngine->mStateStack, 1);
    mAttrState = (mAttrState & ~0x3F00) | 0x780;
    switch (item->mMiscData) {
    case eOptionContinue:
        cAI_finishRemotePlayer(SSXApp->Game->AI);
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
        break;
    case eOptionCancel:
    default:
        cGame_exit(SSXApp->Game, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible);
#ifdef SKIP_ASM
extern char str_Text_ContinueGame[];
extern char str_kT_OVRContinueRacing[];
extern char str_Menu_b[];
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int index);

/**
 * Shows or hides the "continue racing" choice of the online-error screen.
 *
 * Text_ContinueGame gets kT_OVRContinueRacing, is static while hidden (slot 8, setStatic), takes
 * visibility @p visible and choice 4, which cOVState_PAUSE_ONLINE_ERROR_doContinueCancel answers with cAI_finishRemotePlayer. When shown,
 * "Menu" selects row 0.
 *
 * @param visible  nonzero to show the option (On Tour bVisible)
 */
void cOVState_PAUSE_ONLINE_ERROR::setContinueOptionVisible(int visible)
{
    cUIThing* continueText = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text_ContinueGame));
    if (continueText != 0) {
        cUIText_setUnicodeStringByID((cUIText*)continueText, GetHashValue32(str_kT_OVRContinueRacing));
        continueText->setStatic(visible ^ 1);
        continueText->setVisible(visible);
        continueText->mMiscData = eOptionContinue;
    }
    continueText = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Menu_b));
    if (continueText != 0 && visible) {
        cUIMenu_setSelectedByIndex(continueText, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Text_ContinueGame[];
extern char str_Continueps2X[];
extern char str_ContinueButtonBack[];

// A tARGB with EA's (A, R, G, B) constructor (ours has none).
struct tARGBInit : tARGB {
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};

// cUIThing_vtbl slot 11 (setColour). On Tour's takes the colour by reference (const tARGB&) and the call passes a
// temporary; the header's pointer form makes the caller build it first, so the call is by hand.
typedef sVEntry<void(void*, const tARGB&)> tSetColourSlot;

/**
 * Highlights or unhighlights the Continue option of the online pause error overlay.
 *
 * Colours Text_ContinueGame white when @p on, else (1, 0, 0, 0), and shows the Continueps2X button and
 * ContinueButtonBack only when highlighted.
 *
 * @param on  nonzero to highlight
 */
void cOVState_PAUSE_ONLINE_ERROR::setContinueHighLight(int on)
{
    cUIThing* continueText = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text_ContinueGame));
    cUIThing* buttonIcon = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Continueps2X));
    cUIThing* buttonBack = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ContinueButtonBack));
    if (on) {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, continueText);
        vt[11].fn(vthis(continueText, vt[11]), tARGBInit(1.0f, 1.0f, 1.0f, 1.0f));
    } else {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, continueText);
        vt[11].fn(vthis(continueText, vt[11]), tARGBInit(1.0f, 0.0f, 0.0f, 0.0f));
    }
    buttonIcon->setVisible(on);
    buttonBack->setVisible(on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Text_CancelGame[];
extern char str_Cancelps2X[];
extern char str_CancelButtonBack[];

/**
 * Highlights or unhighlights the Cancel option of the online pause error overlay.
 *
 * Colours Text_CancelGame white when @p on, else (1, 0, 0, 0), and shows the Cancelps2X button and
 * CancelButtonBack only when highlighted.
 *
 * @param on  nonzero to highlight
 */
void cOVState_PAUSE_ONLINE_ERROR::setCancelHighLight(int on)
{
    cUIThing* cancelText = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text_CancelGame));
    cUIThing* buttonIcon = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Cancelps2X));
    cUIThing* buttonBack = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_CancelButtonBack));
    if (on) {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, cancelText);
        vt[11].fn(vthis(cancelText, vt[11]), tARGBInit(1.0f, 1.0f, 1.0f, 1.0f));
    } else {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, cancelText);
        vt[11].fn(vthis(cancelText, vt[11]), tARGBInit(1.0f, 0.0f, 0.0f, 0.0f));
    }
    buttonIcon->setVisible(on);
    buttonBack->setVisible(on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", cOVState_HEAT_PREEVENT_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_HEAT_PREEVENT_onGainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* label);
extern "C" void cUIState_onGainFocus(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_riderpct_d[];
extern char str_at_TextPulseWhiteOrange[];

/**
 * Gains focus on the race pre-event overlay: pulses the riders' labels and starts the online overlay timeout.
 *
 * Runs cUIState_onGainFocus, then plays "@TextPulseWhiteOrange" (cOVState_triggerTextFade) on "rider<n>" for the first
 * human rider (cBENewPlayerInterface_getHumanRider(.., 0) + 1) and, in an online race (cBENewRaceInterface_newRace's
 * cGamePath == kGAMEPATH_MULTIPLAYER), for the second. With a cGameComm_mGameComm link it sets mOverlayTimeout to 1200
 * frames, after which cOVState_HEAT_PREEVENT_onUpdate closes the overlay. The overlay factory builds this state (vtable
 * cOVState_HEAT_PREEVENT_vtbl, screen "40race_pre") for overlay type 8. BE interface 0 is fetched and unused.
 */
extern "C" void cOVState_HEAT_PREEVENT_onGainFocus(cOVState_HEAT_PREEVENT* self)
{
    char buf[32];
    cUIState_onGainFocus(self);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sprintf(buf, str_riderpct_d, cBENewPlayerInterface_getHumanRider(iface, 0) + 1);
    cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        sprintf(buf, str_riderpct_d, cBENewPlayerInterface_getHumanRider(iface, 1) + 1);
        cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    }
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = 1200;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_HEAT_PREEVENT_onLoseFocus);
#ifdef SKIP_ASM

/**
 * Loses focus on the race pre-event overlay: stops the online overlay timeout.
 *
 * Sets cGameComm_mGameComm's mOverlayTimeout to -1 when there is a link.
 */
extern "C" void cOVState_HEAT_PREEVENT_onLoseFocus(void)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_HEAT_PREEVENT_onUpdate);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cOVState_onUpdate(void* self);

/**
 * Updates the race pre-event overlay and closes it when the online overlay timeout runs out.
 *
 * Runs cOVState_onUpdate; when cGameComm_mGameComm exists and its mOverlayTimeout has counted down to 0, sets it to -1
 * and cOVStateManager_mStatus to 1 (go to the next overlay).
 */
void cOVState_HEAT_PREEVENT::onUpdate()
{
    cOVState_onUpdate(this);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && gameComm->mOverlayTimeout == 0) {
        gameComm->mOverlayTimeout = -1;
        cOVStateManager_mStatus = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBD20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

struct sColour {
    float r, g, b, a;
    sColour(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

int GetHashValue32(char* str);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);



extern "C" void* cUIScreen_playFrame_BD20(void* screen, int a1, int a2) __asm__("cUIScreen_playFrame");

void cUIText_setUnicodeStringByID(cUIText* text, int id);

extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);

extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);

extern "C" int sprintf(char* buf, const char* fmt, ...);

void cUIText_setAsciiString(cUIText* text, const char* s);

extern "C" void cUIState_hideObjSafe(void* self, char* name);

extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);

extern char str_OV_darkblue[];

extern char D_004A2138[];

extern "C" void cOVTemplate_Simple_onCreateScreen(void* self);
extern "C" char* cBENewRaceInterface_getLocationName1(void* iface);
extern "C" unsigned short* func_002C2540_BD20(unsigned short* dst, char* src) __asm__("func_002C2540");
extern "C" int sprintf2(unsigned short* buf, unsigned short* fmt, ...);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" int cGameModeMan_getHeatScore(void* stats, int id, int kind);
extern "C" char* cBECharacterInterface_getShortNameByCharacter(void* iface, int id, int a2);
extern "C" void cUIState_showObjSafe(void* self, char* name);
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" signed char cBENewPlayerInterface_getRiderCheatCharID(void* self, int index);
extern "C" void* func_00155130(void* iface, int idx);
extern int cGameModeMan_iCharRank[];
// The "%d" format D_004A2138, reached gp-relative here: a sized declaration of the same string.
extern char D_004A2138_n[3] __asm__("D_004A2138");
extern unsigned short D_004A2140[];
extern char str_41freestyle_pre[];
extern char str_track_event[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_objText1[];
extern char str_objText2[];
extern char str_kT_FSOBJ2HeatsQF[];
extern char str_kT_FSOBJHeatsGrandTtl[];
extern char str_kT_FSOBJEarnXpts[];
extern char str_kT_CMNHELPEarnedEnoughPoints[];
extern char str_bullet1[];
extern char str_kT_FSOBJFinalSolo[];
extern char str_kT_OVRCMNPlaceTop3Medal[];
extern char str_kT_OVRCMNCollPointAndMulti[];
extern char str_kT_OVRCMNCollPointIcons[];
extern char str_heat1_tab[];
extern char str_heat2_tab[];
extern char str_finalheat_tab[];
extern char str_title_b[];
extern char str_kT_CMNStateMultiPlay[];
extern char str_kT_CMNStateQuickPlay[];
extern char str_heat1[];
extern char str_kT_OVRCMNHeat1[];
extern char str_kT_OVRCMNScoreColon[];
extern char str_column_secondrun[];
extern char str_column_total[];
extern char str_rider_pct_d[];
extern char str_run_1_pct_d[];
extern char str_run_2_pct_d[];
extern char str_total_pct_d[];
extern char str_dash_sp_dash_sp_dash[];
extern char str_p0rider[];
extern char str_p0run1[];
extern char str_p1rider[];
extern char str_p1dashes[];
extern char str_topScore[];


static inline unsigned short* lookupString_1FBD20(char* name)
{
    cFELocale* g = SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, g);
    void* t = vthis(g, vt[4]);
    return vt[4].fn(t, GetHashValue32(name));
}

static inline void* getObj_1FBD20(cUIState* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Builds the freestyle pre-event overlay: the event title, the heat's objectives and tab, and the standings table.
 *
 * Adds the "41freestyle_pre" screen (kept in mpScreen) and the "OV_darkblue" backdrop, plays frame 0 of each and runs
 * cOVTemplate_Simple_onCreateScreen (cOVState_onCreateScreen plus the "overlayMenu" option). Reads cBENewRaceInterface_newRace's cGameMode
 * and cGamePath and SSXApp->GameModeMan's iHeatNum.
 *
 * "track_event" gets "<location> - <event>" ("%s - %s" through sprintf2; the location name from cBENewRaceInterface_getLocationName1, the
 * event kT_cmnSlopeCaps / kT_cmnBigAirCaps / kT_cmnPipeCaps for cGameMode 1 / 2 / 3). On game path 0 (career) the
 * objectives depend on the heat: heat 1 shows kT_FSOBJ2HeatsQF and kT_FSOBJHeatsGrandTtl; heat 2 shows how many points
 * the player still needs (kT_FSOBJEarnXpts with the third-placed rider's two-heat total minus the player's heat-1
 * score, or kT_CMNHELPEarnedEnoughPoints) and hides "objText2" and "bullet1"; otherwise kT_FSOBJFinalSolo and
 * kT_OVRCMNPlaceTop3Medal. Other paths show kT_OVRCMNPlaceTop3Medal and, offline, kT_OVRCMNCollPointAndMulti
 * (slopestyle) or kT_OVRCMNCollPointIcons; online "objText2" is hidden. Career shows the tab of the current heat
 * ("heat1_tab", "heat2_tab", "finalheat_tab") and hides "title_b"; other paths hide the tabs and title it
 * kT_CMNStateMultiPlay (online) or kT_CMNStateQuickPlay.
 *
 * The table: column "heat1" reads kT_OVRCMNHeat1, or kT_OVRCMNScoreColon in the final heat and outside career, where
 * "column_secondrun" and "column_total" are hidden too (also in heat 1). Rows "rider1".."rider3" list the first three
 * riders of the standings order cGameModeMan_iCharRank other than the human player: name (cBECharacterInterface_getShortNameByCharacter with GameModeMan's
 * iCharacters / iCheatCharacters), heat score (cGameModeMan_getHeatScore heat 0, or 2 in the final), and in career heat
 * 2 the second-run and total scores; "- - -" otherwise. "p0rider" gets the player's name and "p0run1" the player's
 * heat-1 score in career heat 2 ("- - -" otherwise); "p1rider" gets the second player online, else it and "p1dashes"
 * are hidden. "topScore" shows the event's record (func_00155130 on BE interface 8) with "%d".
 */
extern "C" void func_001FBD20(void* self)
{
    unsigned short title[0x38];
    unsigned short line[104];
    cOVState* overlay = (cOVState*)self;
    void* eng = overlay->mpEngine;
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_41freestyle_pre), 0);
    overlay->mpScreen = (cUIScreen*)scr;
    if (scr != 0)
        cUIScreen_playFrame_BD20(scr, 0, 0);
    eng = (void*)overlay->mpEngine;
    scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_OV_darkblue), 0);
    if (scr != 0)
        cUIScreen_playFrame_BD20(scr, 0, 0);
    cOVTemplate_Simple_onCreateScreen(self);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* stats = SSXApp->GameModeMan;
    int mode = stats->iHeatNum;
    void* textObj = getObj_1FBD20(overlay, str_track_event);
    func_002C2540_BD20(title, cBENewRaceInterface_getLocationName1(iface0));
    unsigned short* str = 0;
    switch (cBENewRaceInterface_newRace.cGameMode) {
    case kGAMEMODE_SLOPESTYLE:
        str = lookupString_1FBD20(str_kT_cmnSlopeCaps);
        break;
    case kGAMEMODE_BIGAIR:
        str = lookupString_1FBD20(str_kT_cmnBigAirCaps);
        break;
    case kGAMEMODE_HALFPIPE:
        str = lookupString_1FBD20(str_kT_cmnPipeCaps);
        break;
    }
    sprintf2(line, D_004A2140, title, str);
    cUIText_setUnicodeString(textObj, line);
    void* text = getObj_1FBD20(overlay, str_objText1);
    cUIThing* sub = (cUIThing*)getObj_1FBD20(overlay, str_objText2);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        if (mode == cGameModeMan::kHeat1) {
            cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(str_kT_FSOBJ2HeatsQF));
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_FSOBJHeatsGrandTtl));
        } else if (mode == cGameModeMan::kHeat2) {
            int total = cGameModeMan_getHeatScore(stats, cGameModeMan_iCharRank[2], 0) + cGameModeMan_getHeatScore(stats, cGameModeMan_iCharRank[2], 1);
            int n = total - cGameModeMan_getHeatScore(stats, cBENewPlayerInterface_getHumanRider(iface1, 0), 0);
            if (n > 0) {
                unsigned short buf[800];
                unsigned short* fmt = lookupString_1FBD20(str_kT_FSOBJEarnXpts);
                sprintf1(buf, fmt, n);
                cUIText_setUnicodeString(text, buf);
            } else {
                cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(str_kT_CMNHELPEarnedEnoughPoints));
            }
            sub->setVisible(0);
            cUIState_hideObjSafe(self, str_bullet1);
        } else {
            cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(str_kT_FSOBJFinalSolo));
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_OVRCMNPlaceTop3Medal));
        }
    } else {
        cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(str_kT_OVRCMNPlaceTop3Medal));
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER)
            sub->setVisible(0);
        else if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_SLOPESTYLE)
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_OVRCMNCollPointAndMulti));
        else
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_OVRCMNCollPointIcons));
    }
    cUIState_hideObjSafe(self, str_heat1_tab);
    cUIState_hideObjSafe(self, str_heat2_tab);
    cUIState_hideObjSafe(self, str_finalheat_tab);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        switch (mode) {
        case 0:
        case cGameModeMan::kHeat1:
            cUIState_showObjSafe(self, str_heat1_tab);
            break;
        case cGameModeMan::kHeat2:
            cUIState_showObjSafe(self, str_heat2_tab);
            break;
        case cGameModeMan::kFinalHeat:
            cUIState_showObjSafe(self, str_finalheat_tab);
            break;
        }
        cUIState_hideObjSafe(self, str_title_b);
    } else {
        textObj = getObj_1FBD20(overlay, str_title_b);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER)
            cUIText_setUnicodeStringByID((cUIText*)textObj, GetHashValue32(str_kT_CMNStateMultiPlay));
        else
            cUIText_setUnicodeStringByID((cUIText*)textObj, GetHashValue32(str_kT_CMNStateQuickPlay));
    }
    textObj = getObj_1FBD20(overlay, str_heat1);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        if (mode >= 3)
            cUIText_setUnicodeStringByID((cUIText*)textObj, GetHashValue32(str_kT_OVRCMNScoreColon));
        else
            cUIText_setUnicodeStringByID((cUIText*)textObj, GetHashValue32(str_kT_OVRCMNHeat1));
        if (mode == cGameModeMan::kHeat1 || mode == cGameModeMan::kFinalHeat) {
            cUIState_hideObjSafe(self, str_column_secondrun);
            cUIState_hideObjSafe(self, str_column_total);
        }
    } else {
        cUIText_setUnicodeStringByID((cUIText*)textObj, GetHashValue32(str_kT_OVRCMNScoreColon));
        cUIState_hideObjSafe(self, str_column_secondrun);
        cUIState_hideObjSafe(self, str_column_total);
    }
    {
        int k = 0;
        for (int i = 1; i < 4; i++, k++) {
            char name[0x20];
            char num[0x10];
            int id = cGameModeMan_iCharRank[k];
            if (id == cBENewPlayerInterface_getHumanRider(iface1, 0)) {
                k++;
                id = cGameModeMan_iCharRank[k];
            }
            sprintf(name, str_rider_pct_d, i);
            void* cell = getObj_1FBD20(overlay, name);
            if (cell != 0)
                cUIText_setAsciiString((cUIText*)cell, cBECharacterInterface_getShortNameByCharacter(iface2, stats->iCharacters[id], stats->iCheatCharacters[id]));
            sprintf(name, str_run_1_pct_d, i);
            cell = getObj_1FBD20(overlay, name);
            if (cell != 0) {
                sprintf(num, D_004A2138,
                        cGameModeMan_getHeatScore(stats, id, mode == cGameModeMan::kFinalHeat ? 2 : 0));
                cUIText_setAsciiString((cUIText*)cell, num);
            }
            sprintf(name, str_run_2_pct_d, i);
            cell = getObj_1FBD20(overlay, name);
            if (cBENewRaceInterface_newRace.cGamePath == 0 && mode == cGameModeMan::kHeat2) {
                char name2[0x20];
                char total[0x10];
                sprintf(num, D_004A2138_n, cGameModeMan_getHeatScore(stats, id, 1));
                cUIText_setAsciiString((cUIText*)cell, num);
                sprintf(name2, str_total_pct_d, i);
                cell = getObj_1FBD20(overlay, name2);
                int run1Score = cGameModeMan_getHeatScore(stats, id, 0);
                int run2Score = cGameModeMan_getHeatScore(stats, id, 1);
                sprintf(total, D_004A2138_n, run1Score + run2Score);
                cUIText_setAsciiString((cUIText*)cell, total);
            } else {
                cUIText_setAsciiString((cUIText*)cell, str_dash_sp_dash_sp_dash);
            }
        }
    }
    int idx = cBENewPlayerInterface_getHumanRider(iface1, 0);
    textObj = getObj_1FBD20(overlay, str_p0rider);
    cUIText_setAsciiString((cUIText*)textObj, cBECharacterInterface_getShortNameByCharacter(iface2, cBENewPlayerInterface_getRiderCharID(iface1, idx), cBENewPlayerInterface_getRiderCheatCharID(iface1, idx)));
    textObj = getObj_1FBD20(overlay, str_p0run1);
    if (cBENewRaceInterface_newRace.cGamePath == 0 && mode == cGameModeMan::kHeat2) {
        char buf[0x10];
        int score = cGameModeMan_getHeatScore(stats, cBENewPlayerInterface_getHumanRider(iface1, 0), 0);
        sprintf(buf, D_004A2138, score);
        cUIText_setAsciiString((cUIText*)textObj, buf);
    } else {
        cUIText_setAsciiString((cUIText*)textObj, str_dash_sp_dash_sp_dash);
    }
    textObj = getObj_1FBD20(overlay, str_p1rider);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        idx = cBENewPlayerInterface_getHumanRider(iface1, 1);
        cUIText_setAsciiString((cUIText*)textObj, cBECharacterInterface_getShortNameByCharacter(iface2, cBENewPlayerInterface_getRiderCharID(iface1, idx), cBENewPlayerInterface_getRiderCheatCharID(iface1, idx)));
    } else {
        ((cUIThing*)textObj)->setVisible(0);
        cUIState_hideObjSafe(self, str_p1dashes);
    }
    textObj = getObj_1FBD20(overlay, str_topScore);
    {
        char buf[0x10];
        void* recordScore = func_00155130(iface8, 0);
        sprintf(buf, D_004A2138, recordScore);
        cUIText_setAsciiString((cUIText*)textObj, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FC768);
#ifdef SKIP_ASM
extern "C" void cUIState_onGainFocus(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* label);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void func_002A31C0(void* self);
extern char str_at_TextPulseWhiteOrange[];
extern char str_p0rider[];
extern char str_p0run1[];
extern char str_p1rider[];
extern char str_p0dashes[];
extern char str_p1dashes[];

/**
 * Gains focus on the freestyle pre-event overlay: pulses the players' rows and plays its menu sound.
 *
 * Runs cUIState_onGainFocus and plays "@TextPulseWhiteOrange" (cOVState_triggerTextFade) on "p0rider" and on "p0dashes"
 * in heat 2 (SSXApp->GameModeMan's iHeatNum, kHeat2) or "p0run1" otherwise; in an online race
 * (cBENewRaceInterface_newRace's cGamePath == kGAMEPATH_MULTIPLAYER) also on "p1rider" and "p1dashes". Then plays menu
 * sound 0xE (cSSXAudio_MenuSound) and calls func_002A31C0 on the sound system (cAudioInterface_bxAudio()). BE interface
 * 0 is fetched and unused.
 */
extern "C" void func_001FC768(void* self)
{
    cUIState_onGainFocus(self);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int* mode = (int*)SSXApp->GameModeMan;
    cOVState_triggerTextFade(self, str_p0rider, str_at_TextPulseWhiteOrange);
    if (*mode == cGameModeMan::kHeat2) {
        cOVState_triggerTextFade(self, str_p0dashes, str_at_TextPulseWhiteOrange);
    } else {
        cOVState_triggerTextFade(self, str_p0run1, str_at_TextPulseWhiteOrange);
    }
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        cOVState_triggerTextFade(self, str_p1rider, str_at_TextPulseWhiteOrange);
        cOVState_triggerTextFade(self, str_p1dashes, str_at_TextPulseWhiteOrange);
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
    func_002A31C0(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FC878);
#ifdef SKIP_ASM
// self is a pause-menu UI state (no header yet): +0x10 its UI engine, +0x40 its screen.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" char* cBENewRaceInterface_getLocationName1(void* iface);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);
extern "C" char* func_00146F88(void* iface, int idx);
extern "C" char* cBECharacterInterface_getShortName(void* iface, int id);
extern "C" void* func_00155130(void* iface, int idx);
extern "C" char* func_0020A1A8(void* p);
extern "C" void func_002C2540(unsigned short* dst, char* src);
extern "C" int sprintf2(unsigned short* buf, unsigned short* fmt, ...);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_50multifs_pre[];
extern char str_OV_darkblue[];
extern char str_title1[];
extern char str_kT_CMNStateMultiPlay[];
extern char str_title2[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_kT_cmnRaceCaps_b[];
extern unsigned short D_0046E558[];
extern unsigned short D_004A2140[];
extern char str_name[];
extern char str_obj1[];
extern char str_obj2[];
extern char str_bullet2[];
extern char str_kT_OVRCMNCollPointAndMulti[];
extern char str_kT_OVRCMNCollPointIcons[];
extern char str_kT_OVRCMNVs[];
extern char str_vs[];
extern char str_screendata1[];
extern char str_screendata[];
extern char str_kT_HELPFirstToBottom[];
extern char str_kT_OVRCMNBeatOppMaxTrick[];
extern char str_topScoreLabel[];
extern char str_kT_OVRCMNRecordTime[];
extern char str_kT_OVRCMNRecordScore[];
extern char str_topScore[];
extern char D_004A2138[];

struct sVE_1FC878 { short delta; short index; unsigned short* (*fn)(void*, int); };

static inline unsigned short* lookupString_1FC878(char* name)
{
    cFELocale* g = SSXApp->FELocale;
    sVE_1FC878* vt = (sVE_1FC878*)VPTR_OF(cUILocaleInterface, g);
    void* t = vthis(g, vt[4]);
    return vt[4].fn(t, GetHashValue32(name));
}

static inline int getField_1FC878(signed char* g, int off)
{
    return g[off];
}

static inline bool isField_1FC878(signed char* g, int off, int v)
{
    return g[off] == v;
}

static inline void* getObj_1FC878(cUIState* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Builds the two-player pre-event overlay: titles, both players' names, the objectives and the event record.
 *
 * Adds the "50multifs_pre" screen (kept in mpScreen) and the "OV_darkblue" backdrop, plays frame 0 of each and runs
 * cOVState_onCreateScreen. Reads cBENewRaceInterface_newRace's cGameMode, cGamePath and cGameEvent.
 *
 * "title1" reads kT_CMNStateMultiPlay (set twice). "title2" gets "<location> - <event>" ("%s - %s" through sprintf2;
 * the location from cBENewRaceInterface_getLocationName1, the event kT_cmnRaceCaps_b / kT_cmnSlopeCaps / kT_cmnBigAirCaps / kT_cmnPipeCaps for
 * cGameMode 5 / 1 / 2 / 3, kGAMEMODE_TIMECHALLENGE / _SLOPESTYLE / _BIGAIR / _HALFPIPE), or "<location> - Jam" for
 * cGameMode 6 (kGAMEMODE_PNTSCHALLENGE). Quirk: the "%s - %s" call passes only the location;
 * the event string reaches it because the compiler keeps it in $a3 (fine on the PS2, undefined elsewhere). "name" gets
 * player 1 (the online player name func_00146F88 on game path 2, else cBECharacterInterface_getShortName of the first
 * human rider); "vs" gets kT_OVRCMNVs formatted with player 2's name, found the same way.
 *
 * For cGameMode 1-3 "obj2" and "bullet2" are shown with kT_OVRCMNCollPointAndMulti (slopestyle) or
 * kT_OVRCMNCollPointIcons, otherwise hidden; modes 2-3 hide "screendata1", modes 5-6 hide "screendata" and
 * "screendata1". "obj1" reads kT_HELPFirstToBottom for cGameEvent 4, else kT_OVRCMNBeatOppMaxTrick. For cGameEvent 4 or
 * 0 "topScoreLabel" reads kT_OVRCMNRecordTime and "topScore" the record as a time (func_0020A1A8); otherwise
 * kT_OVRCMNRecordScore and the record with "%d". The record is func_00155130 on BE interface 8.
 */
extern "C" void func_001FC878(void* self)
{
    unsigned short title[0x38];
    unsigned short line[104];
    char rider[0x20];
    unsigned short label[304];
    char num[0x20];
    cOVState* overlay = (cOVState*)self;
    void* eng = overlay->mpEngine;
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_50multifs_pre), 0);
    overlay->mpScreen = (cUIScreen*)scr;
    if (scr != 0)
        cUIScreen_playFrame(scr, 0, 0);
    eng = (void*)overlay->mpEngine;
    scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_OV_darkblue), 0);
    if (scr != 0)
        cUIScreen_playFrame(scr, 0, 0);
    cOVState_onCreateScreen(self);
    signed char* gs = (signed char*)&cBENewRaceInterface_newRace;
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    int mode4A = getField_1FC878(gs, 74);
    int mode48 = getField_1FC878(gs, 72);
    void* widget = getObj_1FC878(overlay, str_title1);
    cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_CMNStateMultiPlay));
    widget = getObj_1FC878(overlay, str_title2);
    func_002C2540(title, cBENewRaceInterface_getLocationName1(iface0));
    unsigned short* str = 0;
    switch (getField_1FC878(gs, 72))
    {
    case kGAMEMODE_TIMECHALLENGE:
        str = lookupString_1FC878(str_kT_cmnRaceCaps_b);
        break;
    case kGAMEMODE_SLOPESTYLE:
        str = lookupString_1FC878(str_kT_cmnSlopeCaps);
        break;
    case kGAMEMODE_BIGAIR:
        str = lookupString_1FC878(str_kT_cmnBigAirCaps);
        break;
    case kGAMEMODE_HALFPIPE:
        str = lookupString_1FC878(str_kT_cmnPipeCaps);
        break;
    }
    unsigned short* lineFmt = D_004A2140;
    if (mode48 == kGAMEMODE_PNTSCHALLENGE)
        sprintf2(line, D_0046E558, title, str);
    else
        sprintf2(line, lineFmt, title);
    cUIText_setUnicodeString(widget, line);
    widget = getObj_1FC878(overlay, str_name);
    if (getField_1FC878((signed char*)&cBENewRaceInterface_newRace, 73) == kGAMEPATH_MULTIPLAYER)
        cUIText_setAsciiString((cUIText*)widget, func_00146F88(iface1, 0));
    else
        cUIText_setAsciiString((cUIText*)widget, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
    cUIText* status = (cUIText*)getObj_1FC878(overlay, str_obj1);
    if (mode48 >= 1 && mode48 <= 3)
    {
        cUIState_showObjSafe(self, str_obj2);
        cUIState_showObjSafe(self, str_bullet2);
        widget = getObj_1FC878(overlay, str_obj2);
        if (mode48 == kGAMEMODE_SLOPESTYLE)
            cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCMNCollPointAndMulti));
        else
            cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCMNCollPointIcons));
    }
    else
    {
        cUIState_hideObjSafe(self, str_obj2);
        cUIState_hideObjSafe(self, str_bullet2);
    }
    widget = getObj_1FC878(overlay, str_title1);
    cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_CMNStateMultiPlay));
    if (getField_1FC878((signed char*)&cBENewRaceInterface_newRace, 73) == kGAMEPATH_MULTIPLAYER)
        strcpy(rider, func_00146F88(iface1, 1));
    else
        strcpy(rider, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 1)));
    unsigned short* fmt = lookupString_1FC878(str_kT_OVRCMNVs);
    sprintf1(label, fmt, rider);
    cUIText_setUnicodeString(getObj_1FC878(overlay, str_vs), label);
    if (mode48 >= 2 && mode48 <= 3)
    {
        cUIState_hideObjSafe(self, str_screendata1);
    }
    else if (mode48 >= 5 && mode48 <= 6)
    {
        cUIState_hideObjSafe(self, str_screendata);
        cUIState_hideObjSafe(self, str_screendata1);
    }
    if (mode4A == kGAMEEVENT_RIVALTIME)
        cUIText_setUnicodeStringByID(status, GetHashValue32(str_kT_HELPFirstToBottom));
    else
        cUIText_setUnicodeStringByID(status, GetHashValue32(str_kT_OVRCMNBeatOppMaxTrick));
    widget = getObj_1FC878(overlay, str_topScoreLabel);
    if (mode4A == kGAMEEVENT_RIVALTIME || mode4A == kGAMEEVENT_RACE)
    {
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCMNRecordTime));
        void* record = func_00155130(iface8, 0);
        widget = getObj_1FC878(overlay, str_topScore);
        cUIText_setAsciiString((cUIText*)widget, func_0020A1A8(record));
    }
    else
    {
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCMNRecordScore));
        void* record = func_00155130(iface8, 0);
        widget = getObj_1FC878(overlay, str_topScore);
        sprintf(num, D_004A2138, record);
        cUIText_setAsciiString((cUIText*)widget, num);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FCEC0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_screendata[];
extern char str_vsGroup[];
extern char str_objectives[];
extern char str_topScoreGroup[];

// mPosition is copied through this constructed vector: a plain t3Vector temporary grows the frame against retail.
struct sVec3_1FCEC0
{
    float x, y, z;
    sVec3_1FCEC0() {}
    sVec3_1FCEC0(float a, float b, float c) : x(a), y(b), z(c) {}
};

/**
 * Shifts the two-player pre-event overlay's groups for the event kind when its transition starts.
 *
 * Only when @p msg is nonzero. For cGameMode (cBENewRaceInterface_newRace, newRace.cGameMode) kGAMEMODE_BIGAIR or
 * kGAMEMODE_HALFPIPE it moves "screendata" 130 units right; for kGAMEMODE_TIMECHALLENGE or kGAMEMODE_PNTSCHALLENGE it
 * moves "vsGroup" by +40 and "objectives" by +67 on y and "topScoreGroup" by -48 on y (the
 * object's mPosition). BE interface 0 is fetched and unused.
 *
 * @param msg  nonzero on the transition the layout is applied for (the first-time flag)
 * @return always 0
 */
extern "C" int func_001FCEC0(cOVState* self, void* msg)
{
    if (msg != 0)
    {
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGameMode;
    if (mode == kGAMEMODE_BIGAIR || mode == kGAMEMODE_HALFPIPE)
    {
        cUIThing* group = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_screendata));
        sVec3_1FCEC0 position = *(sVec3_1FCEC0*)&group->mPosition;
        *(sVec3_1FCEC0*)&group->mPosition = sVec3_1FCEC0(position.x + 130.0f, position.y, position.z);
    }
    else if (mode == kGAMEMODE_TIMECHALLENGE || mode == kGAMEMODE_PNTSCHALLENGE)
    {
        cUIThing* group = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_vsGroup));
        sVec3_1FCEC0 position = *(sVec3_1FCEC0*)&group->mPosition;
        *(sVec3_1FCEC0*)&group->mPosition = sVec3_1FCEC0(position.x, position.y + 40.0f, position.z);
        group = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_objectives));
        position = *(sVec3_1FCEC0*)&group->mPosition;
        *(sVec3_1FCEC0*)&group->mPosition = sVec3_1FCEC0(position.x, position.y + 67.0f, position.z);
        group = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_topScoreGroup));
        position = *(sVec3_1FCEC0*)&group->mPosition;
        *(sVec3_1FCEC0*)&group->mPosition = sVec3_1FCEC0(position.x, position.y - 48.0f, position.z);
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD0D8);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void cUIState_onGainFocus(void* self);

/**
 * Gains focus on the two-player pre-event overlay: starts the online overlay timeout and plays its menu sound.
 *
 * Runs cUIState_onGainFocus, sets cGameComm_mGameComm's mOverlayTimeout to 1200 frames when there is a link
 * (func_001FD150 closes the overlay when it runs out) and plays menu sound 0xE (cSSXAudio_MenuSound).
 */
extern "C" void func_001FD0D8(void* self)
{
    cUIState_onGainFocus(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = 1200;
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD118);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_StartEventWithNoStartGate(void* self);

/**
 * Loses focus on the two-player pre-event overlay: stops the online overlay timeout and calls cSSXAudio_StartEventWithNoStartGate.
 *
 * Sets cGameComm_mGameComm's mOverlayTimeout to -1 when there is a link, then calls cSSXAudio_StartEventWithNoStartGate on the sound system
 * (cAudioInterface_bxAudio()).
 */
extern "C" void func_001FD118(void)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = -1;
    }
    cSSXAudio_StartEventWithNoStartGate(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD150);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cOVState_onUpdate(void* self);

/**
 * Updates the two-player pre-event overlay and closes it when the online overlay timeout runs out.
 *
 * Runs cOVState_onUpdate; when cGameComm_mGameComm exists and its mOverlayTimeout has counted down to 0, sets it to -1
 * and cOVStateManager_mStatus to 1 (go to the next overlay).
 */
extern "C" void func_001FD150(void* self)
{
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && gameComm->mOverlayTimeout == 0) {
        gameComm->mOverlayTimeout = -1;
        cOVStateManager_mStatus = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD190);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_kT_EventPk1Race_b[];
extern char str_kT_EventPk2Race_b[];
extern char str_kT_EventAllPeakRace_b[];
extern char str_kT_EventPk1Jam_b[];
extern char str_kT_EventPk2Jam_b[];
extern char str_kT_EventAllPeakJam_b[];
struct sVEnt_1FD190 { short delta; short index; int (*fn)(void*, int); };

// Looks up a localized string by the hash of its key (SSXApp->FELocale's virtual slot 4).
static inline int sendHash_1FD190(char* name)
{
    cFELocale* obj = SSXApp->FELocale;
    sVEnt_1FD190* vt = (sVEnt_1FD190*)VPTR_OF(cUILocaleInterface, obj);
    return vt[4].fn(vthis(obj, vt[4]), GetHashValue32(name));
}
/**
 * Returns the localized name of a peak challenge event.
 *
 * Looks up the string through SSXApp->FELocale's virtual slot 4 by the hash of its key: 6 kT_EventPk1Race_b, 7
 * kT_EventPk2Race_b, 8 kT_EventAllPeakRace_b, 9 kT_EventPk1Jam_b, 10 kT_EventPk2Jam_b, 11 kT_EventAllPeakJam_b. The map
 * template (cUITemplate_MAP_setupMenus, cUITemplate_MAP_setupPeakGoals), cOVState_REWARDS_onGainTransition,
 * cOVState_TOPTIMES_onCreateScreen and func_001FD320 use it.
 *
 * @param id  the event id, kGAMEEVENT_TIME_PEAK1 to kGAMEEVENT_POINTS_PEAK3 (cBENewRaceInterface_newRace's cGameEvent)
 * @return the 16-bit string (as an int), or 0 for any other id
 */
extern "C" int func_001FD190(int id)
{
    switch (id) {
    case kGAMEEVENT_TIME_PEAK1: return sendHash_1FD190(str_kT_EventPk1Race_b);
    case kGAMEEVENT_TIME_PEAK2: return sendHash_1FD190(str_kT_EventPk2Race_b);
    case kGAMEEVENT_TIME_PEAK3: return sendHash_1FD190(str_kT_EventAllPeakRace_b);
    case kGAMEEVENT_POINTS_PEAK1: return sendHash_1FD190(str_kT_EventPk1Jam_b);
    case kGAMEEVENT_POINTS_PEAK2: return sendHash_1FD190(str_kT_EventPk2Jam_b);
    case kGAMEEVENT_POINTS_PEAK3: return sendHash_1FD190(str_kT_EventAllPeakJam_b);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD268);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern char str_68rival_pre[];
extern char str_OV_darkblue[];

/**
 * Builds the rival-challenge pre-event overlay's screens.
 *
 * Adds the "68rival_pre" screen (kept in mpScreen) and the "OV_darkblue" backdrop, plays frame 0 of each, runs
 * cOVState_onCreateScreen and plays menu sound 0xE (cSSXAudio_MenuSound). The texts are filled in by func_001FD320.
 */
extern "C" void func_001FD268(cOVState* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_68rival_pre), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)self->mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD320);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getCharacterID(int idx);
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int cBENewRaceInterface_getRival(void* iface);
extern "C" char* cBECharacterInterface_getShortNameByCharacter(void* iface, int id, int a2);
extern "C" int cBENewPlayerInterface_isEventLocked(void* iface, int a1, int charId, int item);
extern "C" int cBERewardInterface_isPeakLocked(void* iface, int a1, int a2);
extern "C" void* fptoui(float x);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
// func_002C2540 returns dst (the unit declares it void)
unsigned short* func_002C2540_r(unsigned short* dst, char* src) __asm__("func_002C2540");
extern char str_kT_TITLERivalChallenge[];
extern char str_kT_OVRCMNChallengeBy[];
extern char str_kT_HELPChalPeakRace[];
extern char str_kT_HELPChalPeakJam[];
extern char str_kT_HELPBeatChallengePeakRace[];
extern char str_timescoretobeat[];
extern char str_kT_HELPGetToBottomPoints[];
extern char str_kT_HELPBeatChallPeakJam[];
extern char str_kT_OVRCMNTimeToBeatColon[];
extern char D_004A2550[];
extern char str_kT_HELPRaceToBottom[];
extern char str_kT_HELPBeatPostedTime[];
extern char str_kT_HELPBeatPostedTimePass[];
extern char str_kT_OVRCMNScoreToBeatColon[];
extern char str_pct_s_pct_d_c[];
extern char str_kT_HELPMakeWayDnSlope[];
extern char str_kT_HELPBeatPostedScore[];
extern char str_kT_HELPBeatPostedScorePass[];

struct sVE_1FD320 { short delta; short index; unsigned short* (*fn)(void*, int); };

static inline unsigned short* lookupString_1FD320(char* name)
{
    cFELocale* g = SSXApp->FELocale;
    sVE_1FD320* vt = (sVE_1FD320*)VPTR_OF(cUILocaleInterface, g);
    void* t = vthis(g, vt[4]);
    return vt[4].fn(t, GetHashValue32(name));
}

static inline int getField_1FD320(signed char* g, int off)
{
    return g[off];
}

static inline void* getObj_1FD320(cUIState* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Fills the rival-challenge pre-event overlay's texts when its transition starts: title, challenger, objectives and
 * target.
 *
 * Only when @p msg is nonzero. Works from cBENewRaceInterface_newRace's cGameEvent (4 kGAMEEVENT_RIVALTIME, 5
 * kGAMEEVENT_RIVALPOINTS, 6-8 kGAMEEVENT_TIME_PEAK1-3, 9-11 kGAMEEVENT_POINTS_PEAK1-3). "name" first gets the player's
 * short character name, then (wrapped) the challenge line: kT_OVRCMNChallengeBy (events 4-5), kT_HELPChalPeakRace (6-8)
 * or kT_HELPChalPeakJam (9-11) formatted with the rival's name (cBECharacterInterface_getShortNameByCharacter of cBENewRaceInterface_getRival), which replaces it.
 * "title1" reads kT_TITLERivalChallenge; "title2" gets "<location> - <kT_cmnRaceCaps_b>" (event 4), "<location> - Jam"
 * (5) or the event name func_001FD190 (6-11).
 *
 * Objectives ("obj1", "obj2"): event 4 kT_HELPFirstToBottom and kT_HELPBeatChallengePeakRace, event 5
 * kT_HELPGetToBottomPoints and kT_HELPBeatChallPeakJam; both hide "obj2" and "bullet2" on kGAMEPATH_SINGLEEVENT or when
 * the peak event (6-8 for race, 9-11 for jam) of the current location (kLOCATION_ABC1, kLOCATION_DBC2, kLOCATION_EBC3)
 * is already unlocked
 * (cBENewPlayerInterface_isEventLocked), and hide "timescoretobeat". Events 6-8 show "<Time to beat:> <time>"
 * (kT_OVRCMNTimeToBeatColon and SSXApp->GameModeMan's uiTimeLimit / 60 as a time string, func_0020A1A8), with
 * kT_HELPRaceToBottom and kT_HELPBeatPostedTime, or kT_HELPBeatPostedTimePass while that peak (1 for event 6, 2 for
 * event 7) is still locked for the player (cBERewardInterface_isPeakLocked on BE interface 13; event 8 always uses the plain text).
 * Events 9-11 do the same with kT_OVRCMNScoreToBeatColon and uiPointTarget, kT_HELPMakeWayDnSlope and
 * kT_HELPBeatPostedScore(Pass).
 *
 * @param msg  nonzero on the transition the texts are filled for (the first-time flag)
 * @return always 0
 */
extern "C" int func_001FD320(cUIState* self, void* msg)
{
    unsigned short out[304];
    unsigned short fmt[0x18];
    unsigned short title[0x18];
    unsigned short name[0x20];
    unsigned short line[400];
    unsigned short num[0x20];
    unsigned short text[304];
    if (msg != 0) {
        void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
        void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
        void* iface13 = cBE_getInterface_Fv(cBE_getBE(), 13);
        cGameModeMan* stats = SSXApp->GameModeMan;
        int mode = getField_1FD320((signed char*)&cBENewRaceInterface_newRace, 74);
        void* widget = getObj_1FD320(self, str_name);
        cUIText_setAsciiString((cUIText*)widget, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
        widget = getObj_1FD320(self, str_title2);
        switch (mode) {
        case kGAMEEVENT_RIVALTIME:
            func_002C2540_r(title, cBENewRaceInterface_getLocationName1(iface0));
            func_002C2540_r(fmt, (char*)D_004A2140);
            sprintf1(out, fmt, title, lookupString_1FD320(str_kT_cmnRaceCaps_b));
            cUIText_setUnicodeString(widget, out);
            break;
        case kGAMEEVENT_RIVALPOINTS:
            func_002C2540_r(title, cBENewRaceInterface_getLocationName1(iface0));
            func_002C2540_r(fmt, (char*)D_0046E558);
            sprintf1(out, fmt, title);
            cUIText_setUnicodeString(widget, out);
            break;
        case kGAMEEVENT_TIME_PEAK1:
        case kGAMEEVENT_TIME_PEAK2:
        case kGAMEEVENT_TIME_PEAK3:
        case kGAMEEVENT_POINTS_PEAK1:
        case kGAMEEVENT_POINTS_PEAK2:
        case kGAMEEVENT_POINTS_PEAK3:
            cUIText_setUnicodeString(widget, (unsigned short*)func_001FD190(mode));
            break;
        }
        cUIText* statusA = (cUIText*)getObj_1FD320(self, str_obj1);
        cUIText* statusB = (cUIText*)getObj_1FD320(self, str_obj2);
        widget = getObj_1FD320(self, str_title1);
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_TITLERivalChallenge));
        func_002C2540_r(name, cBECharacterInterface_getShortNameByCharacter(iface2, cBENewRaceInterface_getRival(iface0), 0));
        unsigned short* lineFmt;
        if (mode >= 4 && mode <= 5)
            lineFmt = lookupString_1FD320(str_kT_OVRCMNChallengeBy);
        else if (mode >= 6 && mode <= 8)
            lineFmt = lookupString_1FD320(str_kT_HELPChalPeakRace);
        else
            lineFmt = lookupString_1FD320(str_kT_HELPChalPeakJam);
        sprintf1(line, lineFmt, name);
        cUIText_setUnicodeStringWrap(getObj_1FD320(self, str_name), line);
        switch (mode) {
        case kGAMEEVENT_RIVALTIME:
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPFirstToBottom));
            cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatChallengePeakRace));
            if (getField_1FD320((signed char*)&cBENewRaceInterface_newRace, 73) == kGAMEPATH_SINGLEEVENT
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_ABC1
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 6))
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_DBC2
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 7))
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_EBC3
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 8))) {
                cUIState_hideObjSafe(self, str_obj2);
                cUIState_hideObjSafe(self, str_bullet2);
            }
            cUIState_hideObjSafe(self, str_timescoretobeat);
            break;
        case kGAMEEVENT_RIVALPOINTS:
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPGetToBottomPoints));
            cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatChallPeakJam));
            if (getField_1FD320((signed char*)&cBENewRaceInterface_newRace, 73) == kGAMEPATH_SINGLEEVENT
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_ABC1
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 9))
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_DBC2
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 10))
                || (cBENewRaceInterface_getLocationID(iface0) == kLOCATION_EBC3
                    && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 11))) {
                cUIState_hideObjSafe(self, str_obj2);
                cUIState_hideObjSafe(self, str_bullet2);
            }
            cUIState_hideObjSafe(self, str_timescoretobeat);
            break;
        case kGAMEEVENT_TIME_PEAK1:
        case kGAMEEVENT_TIME_PEAK2:
        case kGAMEEVENT_TIME_PEAK3: {
            widget = getObj_1FD320(self, str_timescoretobeat);
            float mins = (float)stats->uiTimeLimit * 0.01666666753590107f;
            void* wholeSeconds = fptoui(mins);
            func_002C2540_r(num, func_0020A1A8(wholeSeconds));
            unsigned short* labelText = lookupString_1FD320(str_kT_OVRCMNTimeToBeatColon);
            func_002C2540_r(title, D_004A2550);
            sprintf1(text, title, labelText, num);
            cUIText_setUnicodeStringWrap(widget, text);
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPRaceToBottom));
            if (mode == kGAMEEVENT_TIME_PEAK3
                || (mode == kGAMEEVENT_TIME_PEAK2 && !cBERewardInterface_isPeakLocked(iface13, 0, 2))
                || (mode == kGAMEEVENT_TIME_PEAK1 && !cBERewardInterface_isPeakLocked(iface13, 0, 1)))
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedTime));
            else
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedTimePass));
            break;
        }
        case kGAMEEVENT_POINTS_PEAK1:
        case kGAMEEVENT_POINTS_PEAK2:
        case kGAMEEVENT_POINTS_PEAK3: {
            int n = (int)stats->uiPointTarget;
            unsigned short* labelText = lookupString_1FD320(str_kT_OVRCMNScoreToBeatColon);
            func_002C2540_r(title, str_pct_s_pct_d_c);
            sprintf1(num, title, labelText, n);
            cUIText_setUnicodeStringWrap(getObj_1FD320(self, str_timescoretobeat), num);
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPMakeWayDnSlope));
            if (mode == kGAMEEVENT_POINTS_PEAK3
                || (mode == kGAMEEVENT_POINTS_PEAK2 && !cBERewardInterface_isPeakLocked(iface13, 0, 2))
                || (mode == kGAMEEVENT_POINTS_PEAK1 && !cBERewardInterface_isPeakLocked(iface13, 0, 1)))
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedScore));
            else
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedScorePass));
            break;
        }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDB78);
#ifdef SKIP_ASM
/**
 * Answers the rival-challenge pre-event overlay's menu events: feedback 0x101 (animation and sound) for kONNEXT,
 * else 0.
 *
 * @param menu  the menu (unused)
 * @param event  the menu event (tUIMenuEvent)
 * @return 0x101 for kONNEXT, 0 otherwise
 */
extern "C" int func_001FDB78(void* self, int menu, int event)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDB88);
#ifdef SKIP_ASM

extern "C" void cSSXAudio_StartEventWithNoStartGate(void* snd);

/**
 * Starts the rival challenge when the pre-event overlay's button is selected.
 *
 * On kONNEXT (select) with a widget: sets cOVStateManager_mStatus to 3 (close the overlay), sets mOverlayDone of
 * the current game state (SSXApp->Game->mCurrentState) to 1 as the overlay manager does to let the game state continue,
 * calls cSSXAudio_StartEventWithNoStartGate on the sound system (cAudioInterface_bxAudio()) and pops the top state of
 * the engine's state stack (cUIStateStack_pop(&mpEngine->mStateStack, 1)).
 *
 * @param widget  the widget the event is for; nothing happens when it is null
 * @param event  the widget event
 */
extern "C" void func_001FDB88(cUIState* self, int widget, int event)
{
    if (widget != 0 && event == cUIState::kONNEXT) {
        cOVStateManager_mStatus = 3;
        (((SSXApp)->Game)->mCurrentState)->mOverlayDone = 1;
        cSSXAudio_StartEventWithNoStartGate(cAudioInterface_bxAudio());
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_cOVState_SAVEREPLAY);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard(void* self, int a1, int a2, int a3);
extern void* cOVState_SAVEREPLAY_vtbl[];

/**
 * Constructs the save-replay memory-card overlay.
 *
 * Runs the 4-argument cFEMemCard constructor cFEMemCard_cFEMemCard with iType 1 (replay), installs this class's vtable
 * cOVState_SAVEREPLAY_vtbl and clears +0x22C (mIsHidden in On Tour, the hideActionButtons state). The overlay factory
 * (cOVStateManager_triggerOverlay, cOVStateManager_triggerPopup) builds it in 628 bytes for overlay type 18.
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param ovStateId  the overlay id, 18 (On Tour tOVStateEnum eOVState)
 * @return @p self
 */
extern "C" void* cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(cOVState_SAVEREPLAY* self, int engine, int ovStateId)
{
    cFEMemCard_cFEMemCard(self, engine, 1, ovStateId);
    VPTR_OF(cListNode, self) = cOVState_SAVEREPLAY_vtbl;
    self->mIsHidden = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cUIEngine_addScreenByHashName_DC30(void* engine, void* owner, int hash, int a3) __asm__("cUIEngine_addScreenByHashName");
extern "C" void* cUIScreen_playFrame_DC30(void* self, unsigned short frame, int flag) __asm__("cUIScreen_playFrame");
extern "C" cUIThing* cUIScreen_getObjectByHashName_DC30(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern "C" void cOVState_onCreateScreen_DC30(void* self) __asm__("cOVState_onCreateScreen");
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern char str_empty_e[];
extern char str_119Save_Replay[];
extern char str_1percentage_b[];
extern char str_title_Save_Replay[];
extern char str_title_Save_Records[];
extern char str_helptext01[];
extern char str_Memory_card_info[];
extern char str_pct_dmemory_card_b[];
extern char str_OV_darkblue[];

static inline cUIThing* getObj_DC30(cUIState* self, char* name)
{
    return cUIScreen_getObjectByHashName_DC30(self->mpScreen, GetHashValue32(name));
}

/**
 * Builds the save-replay overlay's screen, pushes pause state 2 and runs the base overlay setup.
 *
 * Adds the "119Save_Replay" screen (kept in mpScreen); when it exists, plays frame 0, clears "1percentage_b" and
 * "Memory_card_info", shows "title_Save_Replay", hides "title_Save_Records" and "helptext01", and stores 0..3 in the
 * mMiscData of "1memory_card_b".."4memory_card_b". Adds the "OV_darkblue" backdrop, pushes pause state 2
 * (cPauseState_Stack[++cPauseState_StackIdx] = 2, cPauseState_Flags = cPauseState_FlagsTable[2]) and runs cOVState_onCreateScreen. The `//` note
 * above calls this a device-select overlay that pushes its menu state; it pushes the pause state.
 */
extern "C" void cOVState_SAVEREPLAY_onCreateScreen(cOVState_SAVEREPLAY* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName_DC30(engine, ((char*)self), GetHashValue32(str_119Save_Replay), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIThing* widget;
        cUIScreen_playFrame_DC30(screen, 0, 0);
        widget = getObj_DC30(self, str_1percentage_b);
        cUIText_setAsciiString((cUIText*)widget, str_empty_e);
        widget = getObj_DC30(self, str_title_Save_Replay);
        widget->setVisible(1);
        widget = getObj_DC30(self, str_title_Save_Records);
        widget->setVisible(0);
        widget = getObj_DC30(self, str_helptext01);
        widget->setVisible(0);
        widget = getObj_DC30(self, str_Memory_card_info);
        cUIText_setAsciiString((cUIText*)widget, str_empty_e);
        char name[0x20];
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            widget = getObj_DC30(self, name);
            widget->mMiscData = i - 1;
        }
    }
    engine = (void*)self->mpEngine;
    screen = cUIEngine_addScreenByHashName_DC30(engine, ((char*)self), GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame_DC30(screen, 0, 0);
    }
    cPauseState_Stack[++cPauseState_StackIdx] = 2;
    cPauseState_Flags = cPauseState_FlagsTable[2];
    cOVState_onCreateScreen_DC30(((char*)self));
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_onDestroyScreen);
#ifdef SKIP_ASM
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
void cReplay_purgeSave(void* self);
extern "C" void* cFEMemCard_onDestroyScreen(void* self);
extern "C" void cReplay_startReplay(void* self, void* a1, int a2, int a3);

/**
 * Tears down the save-replay overlay and restarts the replay.
 *
 * When a replay save buffer is still held (+0x1C8, On Tour mIsReplayAlloc) it releases it (cReplay_purgeSave on
 * SSXApp->Game->Replay, On Tour cReplay::purgeSave) and clears the flag. Pops the pause state (cPauseState_Flags =
 * cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]]), runs cFEMemCard_onDestroyScreen and restarts playback with
 * cReplay_startReplay(Replay, 0, 0, 0).
 */
extern "C" void cOVState_SAVEREPLAY_onDestroyScreen(cOVState_SAVEREPLAY* self)
{
    if (self->mIsReplayAlloc == true) {
        cReplay_purgeSave(SSXApp->Game->Replay);
        self->mIsReplayAlloc = 0;
    }
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
    cFEMemCard_onDestroyScreen(self);
    cReplay_startReplay(SSXApp->Game->Replay, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_updateDone);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_popToStateInclusive(void* self);

/**
 * Finishes the save-replay overlay once: pops the UI state stack down to it.
 *
 * When +0x1AC (On Tour mIsDone) is still 0 it calls cUIStateStack_popToStateInclusive(mpEngine + 0x18) (On Tour
 * cUIStateStack::popToStateInclusive); then sets +0x1AC to 1.
 */
void cOVState_SAVEREPLAY::updateDone()
{
    if (mIsDone == 0) {
        cUIStateStack_popToStateInclusive(&mpEngine->mStateStack);
    }
    mIsDone = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_displayStatus);
#ifdef SKIP_ASM
class cState_DF40 {
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

struct sVE_DF40 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cG_DF40 {
public:
    int pad;
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual void v04(int);
};

extern "C" void cFEMemCard_callbackDisplayNoCards();
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* mp, int dev, int a3);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mp, int dev, unsigned short* buf, int len);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" void* cSSXApp_getMCManager(void* self);
extern char str_1percentage_b[];
extern char str_Memory_card_info[];
extern char str_pct_dmemory_card_b[];
extern char str_kT_MEMEmpty_b[];
extern char str_arrw_left_b[];
extern char str_arrw_right_b[];

static inline cUIText* getObj_DF40(cUIState* self, char* name)
{
    return (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Refreshes the save-replay overlay's device panel: card name, free space, the four save slots and the device arrows.
 *
 * Works on the memory-card manager (cSSXApp_getMCManager). When cFEMemCard_displayFileOK (vtable slot 42) reports the card can be
 * shown, a missing card (mpMemCard slot 53, cPS2MemCard_IsAvailable on the current device) shows the no-cards message
 * (cFEMemCard_callbackDisplayNoCards), else the screen moves to display state 3 (cFEMemCard_triggerDisplayState) unless already there. When
 * slot 42 still holds, it re-highlights the menu (cFEMemCard_setMenuHighlight), writes the selected device's label
 * (cMCOverlayManager_GetDeviceDisplayString on mSelectedDevice) into "Memory_card_info", puts the free-space text
 * (mpMemCard slot 64) into "1percentage_b" and hides it. It looks up kT_MEMEmpty_b and drops the result.
 *
 * Each of "1memory_card_b".."4memory_card_b" gets its file slot's title (mpMemCard slot 14, cPS2MemCard_GetTitleString,
 * through the buffer at +0x230). An empty slot (no cPS2MemCard_HasFileName, not cPS2MemCard_IsFileSlotDisabled) on a
 * formatted card (slot 55) is live only if it is the first such one; later empty ones are static. A slot with a save is
 * live when the device is usable (slot 54) and the slot is not disabled; anything else is static. Static means
 * setStatic(1) and vtable slot 25 (cUIText_setUseStaticColour) 1. "arrw_left_b" / "arrw_right_b" are shown when
 * cFEMemCard_checkNextAvailableDevice finds a device that way. When the display state (+0x214) is not 3, all four slots
 * end up static with the normal colour.
 */
void cOVState_SAVEREPLAY::displayStatus()
{
    unsigned short buf[800];
    char name[0x40];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (((cState_DF40*)this)->displayFileOK()) {
        if (!mp->mpMemCard->IsAvailable1()) {
            cFEMemCard_callbackDisplayNoCards();
        } else if (mScreenStatus != cFEMemCard::kDISPLAY_ON) {
            ((cState_DF40*)this)->triggerDisplayState(cFEMemCard::kDISPLAY_ON);
        }
    }
    if (((cState_DF40*)this)->displayFileOK()) {
        int sel = 0;
        ((cState_DF40*)this)->setMenuHighlight();
        cMCOverlayManager_GetDeviceDisplayString(mp, mp->mSelectedDevice, buf, 800);
        cUIText_setUnicodeStringWrap(getObj_DF40(this, str_Memory_card_info), buf);
        cUIText* widget = getObj_DF40(this, str_1percentage_b);
        mp->mpMemCard->GetStatus(buf);
        cUIText_setUnicodeString(widget, buf);
        widget->setVisible(0);
        cFELocale* localeObj = SSXApp->FELocale;
        sVE_DF40* vt = (sVE_DF40*)VPTR_OF(cUILocaleInterface, localeObj);
        void* localeThis = vthis(localeObj, vt[4]);
        vt[4].fn(localeThis, GetHashValue32(str_kT_MEMEmpty_b));
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            widget = getObj_DF40(this, name);
            mp->mpMemCard->GetTitleString(i - 1, (unsigned short*)&mTitle);
            cUIText_setUnicodeString(widget, (unsigned short*)((char*)&mTitle));
            if (!mp->mpMemCard->HasFileName(i - 1) && !mp->mpMemCard->IsFileSlotDisabled(i - 1)
                && mp->mpMemCard->IsFormatted()) {
                if (sel == 0) {
                    sel = 1;
                    widget->setUseStaticColour(0);
                    widget->setStatic(0);
                } else {
                    widget->setUseStaticColour(1);
                    widget->setStatic(1);
                }
            } else if (mp->mpMemCard->IsActionable(mp->mSelectedDevice)) {
                if (mp->mpMemCard->IsFileSlotDisabled(i - 1)) {
                    widget->setStatic(1);
                    widget->setUseStaticColour(1);
                } else {
                    widget->setStatic(0);
                    widget->setUseStaticColour(0);
                }
            } else {
                widget->setUseStaticColour(1);
                widget->setStatic(1);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(((cState_DF40*)this), mp, mp->mSelectedDevice, 1)
            == cFEMemCard::kNoNextDevice)
            getObj_DF40(this, str_arrw_left_b)->setVisible(0);
        else
            getObj_DF40(this, str_arrw_left_b)->setVisible(1);
        if (cFEMemCard_checkNextAvailableDevice(((cState_DF40*)this), mp, mp->mSelectedDevice, 0)
            == cFEMemCard::kNoNextDevice)
            getObj_DF40(this, str_arrw_right_b)->setVisible(0);
        else
            getObj_DF40(this, str_arrw_right_b)->setVisible(1);
    }
    if (mScreenStatus != cFEMemCard::kDISPLAY_ON) {
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            cUIText* widget = getObj_DF40(this, name);
            widget->setUseStaticColour(0);
            widget->setStatic(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_hideActionButtons);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];
extern char str_o_delete[];
extern char str_ps2sq_b[];

/**
 * Hides or shows all four action buttons of the save-replay overlay.
 *
 * Only acts when @p on differs from +0x22C (On Tour mIsHidden): sets "ps2x_b", "x_save", "o_delete" and "ps2sq_b"
 * visible when @p on is false and hidden when true, then stores @p on at +0x22C.
 *
 * @param on  true to hide the buttons
 */
extern "C" void cOVState_SAVEREPLAY_hideActionButtons(cOVState_SAVEREPLAY* self, bool on)
{
    if (self->mIsHidden != on) {
        int off = !on;
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_o_delete))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b))->setVisible(off);
        self->mIsHidden = on;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_hideDeleteButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_o_delete[];
extern char str_ps2sq_b[];

/**
 * Hides or shows the save-replay overlay's delete button.
 *
 * Sets "o_delete" and "ps2sq_b" visible when @p on is false and hidden when true, and stores !@p on at +0x220 (On Tour
 * mDeleteButtonActive).
 *
 * @param on  true to hide the button
 */
extern "C" void cOVState_SAVEREPLAY_hideDeleteButton(cOVState_SAVEREPLAY* self, bool on)
{
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_o_delete))->setVisible(!on);
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b))->setVisible(!on);
    self->mDeleteButtonActive = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_hideMainButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];

/**
 * Hides or shows the save-replay overlay's save button.
 *
 * Sets "ps2x_b" and "x_save" visible when @p on is false and hidden when true, and stores !@p on at +0x224 (On Tour
 * mMainButtonActive). @p loadBlocked is ignored.
 *
 * @param loadBlocked  whether the load button should be hidden (unused: the screen has no load button)
 * @param on  true to hide the save button
 */
extern "C" void cOVState_SAVEREPLAY_hideMainButton(cOVState_SAVEREPLAY* self, int loadBlocked, bool on)
{
    cUIThing* xButton = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b));
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save))->setVisible(!on);
    self->mMainButtonActive = !on;
    xButton->setVisible(!on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* self);
extern "C" void cMCOverlayManager_writeSetFileType(void* self, int a, int b);
void cReplay_purgeSave(void* self);
// PORT: the unit declares cMCOverlayManager_SetAction as returning void*; this caller ignores the result (void view by
// asm label).
extern "C" void cMCOverlayManager_SetAction_v(void* self, int mode) __asm__("cMCOverlayManager_SetAction");

/**
 * Handles the save-replay overlay's file-slot buttons: save to a slot, back out, or ask to delete a save.
 *
 * Ignores events without a widget or while busy (+0x1A8, mIsProcessing). kONNEXT (select), when the screen is not
 * leaving (mState +0x1C0 != kDONE) and the display state (+0x214) is kDISPLAY_ON: stores the widget's slot (miMiscData)
 * in +0x1C4 (mSlot) and the manager's mFileCursor, releases a held replay buffer (+0x1C8, cReplay_purgeSave on the
 * replay), starts the replay save with cMCOverlayManager_writeSetFileType(manager, 0, -1) (On Tour writeSetFileType,
 * file type 0 = replay), and sets mIsProcessing, mIsSavePressed (+0x1DC) and mState 3 (kSAVING). kONPREV (back) sets
 * mIsCancelPressed (+0x1E4), resets the manager to state 0 (vtable slot 1, cMCOverlayManager_SetState) when mState is
 * kSTART, kCHECKING or kSKIP, and sets mState kDONE. kONOPTION, on a usable device (mpMemCard slot 54) in mState
 * kCHECKING for a slot holding a save (cPS2MemCard_HasFileName): mIsProcessing, mState kDELETING,
 * cMCOverlayManager_SetAction(0) and manager state 0x18 (On Tour kMCOverlay_DeleteConfirm).
 *
 * @param msg  the widget the event is for (its mMiscData is the file slot)
 * @param type  the widget event: kONNEXT select, kONPREV back, kONOPTION delete
 */
void cOVState_SAVEREPLAY::onWidgetEvent(cUIThing* msg, unsigned int type)
{
    if (msg == 0)
        return;
    if (mIsProcessing != 0)
        return;
    switch (type)
    {
    case cUIState::kONNEXT:
    {
        int one = 1;
        if (mState == cFEMemCard::kDONE)
            return;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        int st = mScreenStatus;
        if (st != cFEMemCard::kDISPLAY_ON)
            return;
        int fileSlot = msg->mMiscData;
        mSlot = fileSlot;
        mp->mFileCursor = fileSlot;
        mIsProcessing = one;
        if (mIsReplayAlloc == one)
        {
            cReplay_purgeSave(SSXApp->Game->Replay);
            mIsReplayAlloc = 0;
        }
        cMCOverlayManager_writeSetFileType(mp, 0, -1);
        mIsSavePressed = one;
        mIsProcessing = one;
        mState = st;
        break;
    }
    case cUIState::kONPREV:
    {
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (mIsProcessing == true)
            return;
        int cardState = mState;
        if (cardState == cFEMemCard::kDONE)
            return;
        mIsCancelPressed = 1;
        if (cardState == cFEMemCard::kSTART || cardState == cFEMemCard::kSKIP || cardState == cFEMemCard::kCHECKING)
            mp->SetState(0);
        mState = cFEMemCard::kDONE;
        break;
    }
    case cUIState::kONOPTION:
    {
        int one = 1;
        if (mState == cFEMemCard::kDONE)
            return;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (!mp->mpMemCard->IsActionable(mp->mSelectedDevice))
            return;
        int fileSlot = msg->mMiscData;
        mSlot = fileSlot;
        mp->mFileCursor = fileSlot;
        int cardState = mState;
        if (cardState != one)
            return;
        if (!mp->mpMemCard->HasFileName(mSlot))
            return;
        mIsProcessing = cardState;
        mState = cFEMemCard::kDELETING;
        cMCOverlayManager_SetAction_v(mp, 0);
        mp->SetState(0x18);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEREPLAY_onPopupEvent);
#ifdef SKIP_ASM
void cReplay_purgeSave(void* self);
extern "C" void cFEMemCard_onPopupEvent(void* self, void* a1, int key);

/**
 * Handles the save-replay overlay's popup events: frees the replay buffer once the save-done popup closes.
 *
 * On kONPOPUPDESTROY while the popup type (+0x19C, mPopupType) is kPOP_SAVEDONE, releases the replay save buffer
 * (cReplay_purgeSave on SSXApp->Game->Replay) and clears +0x1C8 (mIsReplayAlloc); then hands every event to the memory-card
 * base handler cFEMemCard_onPopupEvent.
 *
 * @param popup  the popup state that sent the event
 * @param key  the popup event
 */
extern "C" void cOVState_SAVEREPLAY_onPopupEvent(cOVState_SAVEREPLAY* self, cUIState* popup, int key)
{
    if (key == cUIState::kONPOPUPDESTROY) {
        if (self->mPopupType == cFEMemCard::kPOP_SAVEDONE) {
            cReplay_purgeSave(SSXApp->Game->Replay);
            self->mIsReplayAlloc = 0;
        }
    }
    cFEMemCard_onPopupEvent(self, popup, key);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard(void* self, int a1, int a2, int a3);
extern void* cOVState_SAVEOPTIONS_vtbl[];

/**
 * Constructs the save-records memory-card overlay.
 *
 * Runs the 4-argument cFEMemCard constructor cFEMemCard_cFEMemCard with iType 2 (settings), installs this class's vtable
 * cOVState_SAVEOPTIONS_vtbl and clears +0x22C (mIsHidden in On Tour). The overlay factory (cOVStateManager_triggerOverlay,
 * cOVStateManager_triggerPopup) builds it in 628 bytes for overlay type 19.
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param ovStateId  the overlay id, 19 (On Tour tOVStateEnum eOVState)
 * @return @p self
 */
extern "C" void* cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(cOVState_SAVEOPTIONS* self, int engine, int ovStateId)
{
    cFEMemCard_cFEMemCard(self, engine, 2, ovStateId);
    VPTR_OF(cListNode, self) = cOVState_SAVEOPTIONS_vtbl;
    self->mIsHidden = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_OV_darkblue[];
extern char str_helptext01[];
extern char str_119Save_Replay[];
extern char str_1percentage_b[];
extern char str_title_Save_Replay[];
extern char str_title_Save_Records[];
extern char str_Memory_card_info[];
extern char str_pct_dmemory_card_b[];
extern char D_004A2138[];
extern char str_empty_e[];

// The engine is read through this inline: reading self->mpEngine in the call reorders the prologue against retail.
static inline cUIEngine* engine_1FE9A8(cUIState* self)
{
    return self->mpEngine;
}

/**
 * Builds the save-records overlay's screen: one save slot under the "Save Records" title.
 *
 * Adds the shared "119Save_Replay" screen (kept in mpScreen); when it exists, plays frame 0, clears "1percentage_b" and
 * "Memory_card_info", hides "title_Save_Replay" and shows "title_Save_Records" and "helptext01". Slot "1memory_card_b"
 * gets mMiscData 0; "2memory_card_b".."4memory_card_b" are made static and hidden, as are their number labels
 * "2".."4". Adds the "OV_darkblue" backdrop and runs cOVState_onCreateScreen.
 */
extern "C" void cOVState_SAVEOPTIONS_onCreateScreen(cOVState_SAVEOPTIONS* self)
{
    void* scr = cUIEngine_addScreenByHashName(engine_1FE9A8(self), self, GetHashValue32(str_119Save_Replay), 0);
    self->mpScreen = (cUIScreen*)scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
        cUIText_setAsciiString(
            (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1percentage_b)), str_empty_e);
        cUIThing* widget = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_Save_Replay));
        widget->setVisible(0);
        widget = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_Save_Records));
        widget->setVisible(1);
        widget = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext01));
        widget->setVisible(1);
        cUIText_setAsciiString(
            (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Memory_card_info)), str_empty_e);
        for (int i = 1; i < 5; i++) {
            char buf[0x20];
            sprintf(buf, str_pct_dmemory_card_b, i);
            widget = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
            if (i == 1) {
                widget->mMiscData = 0;
            } else {
                widget->setStatic(1);
                widget->setVisible(0);
            }
            sprintf(buf, D_004A2138, i);
            widget = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
            if (i >= 2)
                widget->setVisible(0);
        }
    }
    void* backdrop = cUIEngine_addScreenByHashName(engine_1FE9A8(self), self, GetHashValue32(str_OV_darkblue), 0);
    if (backdrop != 0)
        cUIScreen_playFrame(backdrop, 0, 0);
    cOVState_onCreateScreen(((char*)self));
}
#endif

extern "C" void* cFEMemCard_onDestroyScreen(void* self);

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Tears down the save-records overlay: runs cFEMemCard_onDestroyScreen.
 *
 * @return what cFEMemCard_onDestroyScreen returns (declared void* here)
 */
void* cOVState::SAVEOPTIONS_onDestroyScreen()
{
    return cFEMemCard_onDestroyScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_updateDone);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_popToStateInclusive(void* self);

/**
 * Finishes the save-records overlay once: pops the UI state stack down to it.
 *
 * When +0x1AC (On Tour mIsDone) is still 0 it calls cUIStateStack_popToStateInclusive(mpEngine + 0x18) (On Tour
 * cUIStateStack::popToStateInclusive); then sets +0x1AC to 1.
 */
void cOVState_SAVEOPTIONS::updateDone()
{
    if (mIsDone == 0) {
        cUIStateStack_popToStateInclusive(&mpEngine->mStateStack);
    }
    mIsDone = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_displayStatus);
#ifdef SKIP_ASM
class cState_ECA0 {
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

struct sVE_ECA0 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cG_ECA0 {
public:
    int pad;
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual void v04(int);
};

extern "C" void cFEMemCard_callbackDisplayNoCards();
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* mp, int dev, int a3);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mp, int dev, unsigned short* buf, int len);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" void* cSSXApp_getMCManager(void* self);
extern char str_1percentage_b[];
extern char str_Memory_card_info[];
extern char str_pct_dmemory_card_b[];
extern char str_kT_MEMEmpty_b[];
extern char str_arrw_left_b[];
extern char str_arrw_right_b[];

static inline cUIText* getObj_ECA0(cUIState* self, char* name)
{
    return (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Refreshes the save-records overlay's device panel: card name, free space, its one save slot and the device arrows.
 *
 * The single-slot form of cOVState_SAVEREPLAY_displayStatus. When cFEMemCard_displayFileOK (vtable slot 42) reports the card can be shown, a missing
 * card (mpMemCard slot 53) shows the no-cards message (cFEMemCard_callbackDisplayNoCards), else the screen moves to display state 3
 * (cFEMemCard_triggerDisplayState) unless already there. When slot 42 still holds, it re-highlights the menu
 * (cFEMemCard_setMenuHighlight), writes the selected device's label into "Memory_card_info" and the free-space text
 * into "1percentage_b" (then hidden), and looks up kT_MEMEmpty_b without using it. "1memory_card_b" gets file slot 0's
 * title (cPS2MemCard_GetTitleString, through +0x230) and is live when empty on a formatted card, or when it holds a
 * save on a usable device and is not disabled; otherwise static (setStatic(1), slot 25 cUIText_setUseStaticColour 1).
 * The device arrows follow cFEMemCard_checkNextAvailableDevice. Outside display state 3 (+0x214) the slot ends up
 * static with the normal colour.
 */
void cOVState_SAVEOPTIONS::displayStatus()
{
    unsigned short buf[800];
    char name[0x40];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (((cState_ECA0*)this)->displayFileOK()) {
        if (!mp->mpMemCard->IsAvailable1()) {
            cFEMemCard_callbackDisplayNoCards();
        } else if (mScreenStatus != cFEMemCard::kDISPLAY_ON) {
            ((cState_ECA0*)this)->triggerDisplayState(cFEMemCard::kDISPLAY_ON);
        }
    }
    if (((cState_ECA0*)this)->displayFileOK()) {
        ((cState_ECA0*)this)->setMenuHighlight();
        cMCOverlayManager_GetDeviceDisplayString(mp, mp->mSelectedDevice, buf, 800);
        cUIText_setUnicodeStringWrap(getObj_ECA0(this, str_Memory_card_info), buf);
        cUIText* widget = getObj_ECA0(this, str_1percentage_b);
        mp->mpMemCard->GetStatus(buf);
        cUIText_setUnicodeString(widget, buf);
        widget->setVisible(0);
        cFELocale* localeObj = SSXApp->FELocale;
        sVE_ECA0* vt = (sVE_ECA0*)VPTR_OF(cUILocaleInterface, localeObj);
        void* localeThis = vthis(localeObj, vt[4]);
        vt[4].fn(localeThis, GetHashValue32(str_kT_MEMEmpty_b));
        sprintf(name, str_pct_dmemory_card_b, 1);
        widget = getObj_ECA0(this, name);
        mp->mpMemCard->GetTitleString(0, (unsigned short*)&mTitle);
        cUIText_setUnicodeString(widget, (unsigned short*)((char*)&mTitle));
        if (!mp->mpMemCard->HasFileName(0) && !mp->mpMemCard->IsFileSlotDisabled(0)
            && mp->mpMemCard->IsFormatted()) {
            widget->setUseStaticColour(0);
            widget->setStatic(0);
        } else if (mp->mpMemCard->IsActionable(mp->mSelectedDevice)) {
            if (mp->mpMemCard->IsFileSlotDisabled(0)) {
                widget->setStatic(1);
                widget->setUseStaticColour(1);
            } else {
                widget->setStatic(0);
                widget->setUseStaticColour(0);
            }
        } else {
            widget->setUseStaticColour(1);
            widget->setStatic(1);
        }
        if (cFEMemCard_checkNextAvailableDevice(((cState_ECA0*)this), mp, mp->mSelectedDevice, 1)
            == cFEMemCard::kNoNextDevice)
            getObj_ECA0(this, str_arrw_left_b)->setVisible(0);
        else
            getObj_ECA0(this, str_arrw_left_b)->setVisible(1);
        if (cFEMemCard_checkNextAvailableDevice(((cState_ECA0*)this), mp, mp->mSelectedDevice, 0)
            == cFEMemCard::kNoNextDevice)
            getObj_ECA0(this, str_arrw_right_b)->setVisible(0);
        else
            getObj_ECA0(this, str_arrw_right_b)->setVisible(1);
    }
    if (mScreenStatus != cFEMemCard::kDISPLAY_ON) {
        sprintf(name, str_pct_dmemory_card_b, 1);
        cUIText* widget = getObj_ECA0(this, name);
        widget->setUseStaticColour(0);
        widget->setStatic(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_hideActionButtons);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];
extern char str_o_delete[];
extern char str_ps2sq_b[];

/**
 * Hides or shows all four action buttons of the save-records overlay.
 *
 * Only acts when @p on differs from +0x22C (On Tour mIsHidden): sets "ps2x_b", "x_save", "o_delete" and "ps2sq_b"
 * visible when @p on is false and hidden when true, then stores @p on at +0x22C.
 *
 * @param on  true to hide the buttons
 */
extern "C" void cOVState_SAVEOPTIONS_hideActionButtons(cOVState_SAVEOPTIONS* self, bool on)
{
    if (self->mIsHidden != on) {
        int off = !on;
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_o_delete))->setVisible(off);
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b))->setVisible(off);
        self->mIsHidden = on;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_hideDeleteButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_o_delete[];
extern char str_ps2sq_b[];

/**
 * Hides or shows the save-records overlay's delete button.
 *
 * Sets "o_delete" and "ps2sq_b" visible when @p on is false and hidden when true, and stores !@p on at +0x220 (On Tour
 * mDeleteButtonActive).
 *
 * @param on  true to hide the button
 */
extern "C" void cOVState_SAVEOPTIONS_hideDeleteButton(cOVState_SAVEOPTIONS* self, bool on)
{
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_o_delete))->setVisible(!on);
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b))->setVisible(!on);
    self->mDeleteButtonActive = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_hideMainButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];

/**
 * Hides or shows the save-records overlay's save button.
 *
 * Sets "ps2x_b" and "x_save" visible when @p on is false and hidden when true, and stores !@p on at +0x224 (On Tour
 * mMainButtonActive). @p loadBlocked is ignored.
 *
 * @param loadBlocked  whether the load button should be hidden (unused: the screen has no load button)
 * @param on  true to hide the save button
 */
extern "C" void cOVState_SAVEOPTIONS_hideMainButton(cOVState_SAVEOPTIONS* self, int loadBlocked, bool on)
{
    cUIThing* xButton = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b));
    cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save))->setVisible(!on);
    self->mMainButtonActive = !on;
    xButton->setVisible(!on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_SAVEOPTIONS_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* self);
extern "C" void cMCOverlayManager_writeSetFileType(void* self, int a, int b);
// PORT: the unit declares cMCOverlayManager_SetAction as returning void*; this caller ignores the result (void view by
// asm label).
extern "C" void cMCOverlayManager_SetAction_v(void* self, int mode) __asm__("cMCOverlayManager_SetAction");

/**
 * Handles the save-records overlay's file-slot buttons: save the settings, back out, or ask to delete a save.
 *
 * Same as cOVState_SAVEREPLAY_onWidgetEvent without the replay buffer. Ignores events without a widget or while busy (+0x1A8,
 * mIsProcessing). kONNEXT (select), when mState (+0x1C0) is not kDONE and the display state (+0x214) is kDISPLAY_ON:
 * stores the
 * widget's slot (miMiscData) in +0x1C4 (mSlot) and the manager's mFileCursor, starts the save with
 * cMCOverlayManager_writeSetFileType(manager, 2, -1) (On Tour writeSetFileType, file type 2 = settings) and sets mIsProcessing,
 * mIsSavePressed (+0x1DC) and mState 3 (kSAVING). kONPREV (back) sets mIsCancelPressed (+0x1E4), resets the manager
 * to state 0 (vtable slot 1, cMCOverlayManager_SetState) when mState is kSTART, kCHECKING or kSKIP, and sets mState
 * kDONE. kONOPTION, on a usable device (mpMemCard slot 54) in mState kCHECKING for a slot holding a save
 * (cPS2MemCard_HasFileName): mIsProcessing, mState kDELETING,
 * cMCOverlayManager_SetAction(0) and manager state 0x18 (On Tour kMCOverlay_DeleteConfirm).
 *
 * @param msg  the widget the event is for (its mMiscData is the file slot)
 * @param type  the widget event: kONNEXT select, kONPREV back, kONOPTION delete
 */
void cOVState_SAVEOPTIONS::onWidgetEvent(cUIThing* msg, unsigned int type)
{
    if (msg == 0)
        return;
    if (mIsProcessing != 0)
        return;
    switch (type)
    {
    case cUIState::kONNEXT:
    {
        int one = 1;
        if (mState == cFEMemCard::kDONE)
            return;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        int st = mScreenStatus;
        if (st != cFEMemCard::kDISPLAY_ON)
            return;
        int fileSlot = msg->mMiscData;
        mSlot = fileSlot;
        mp->mFileCursor = fileSlot;
        mIsProcessing = one;
        cMCOverlayManager_writeSetFileType(mp, 2, -1);
        mIsSavePressed = one;
        mIsProcessing = one;
        mState = st;
        break;
    }
    case cUIState::kONPREV:
    {
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (mIsProcessing == true)
            return;
        int cardState = mState;
        if (cardState == cFEMemCard::kDONE)
            return;
        mIsCancelPressed = 1;
        if (cardState == cFEMemCard::kSTART || cardState == cFEMemCard::kSKIP || cardState == cFEMemCard::kCHECKING)
            mp->SetState(0);
        mState = cFEMemCard::kDONE;
        break;
    }
    case cUIState::kONOPTION:
    {
        int one = 1;
        if (mState == cFEMemCard::kDONE)
            return;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (!mp->mpMemCard->IsActionable(mp->mSelectedDevice))
            return;
        int fileSlot = msg->mMiscData;
        mSlot = fileSlot;
        mp->mFileCursor = fileSlot;
        int cardState = mState;
        if (cardState != one)
            return;
        if (!mp->mpMemCard->HasFileName(mSlot))
            return;
        mIsProcessing = cardState;
        mState = cFEMemCard::kDELETING;
        cMCOverlayManager_SetAction_v(mp, 0);
        mp->SetState(0x18);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cOVState_REWARDS_checkRewardList(void* self, int a1, int a2);
extern "C" void cUIMenu_setScrollableMenu(void* self, unsigned char a1);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern char str_62reward_list[];
extern char str_OV_darkblue[];
extern char str_Menu0000_b[];
extern char str_up[];
extern char str_down[];

/**
 * Builds the rewards list overlay ("62reward_list" over "OV_darkblue") and sizes its menu to the rewards.
 *
 * Zeroes the reward count (mItemSize) and runs cOVState_REWARDS_checkRewardList(self, 0, -1), which probably counts
 * them into it. "Menu0000" loses its wrap flag (mAttr bit 0x80) and becomes scrollable when it has fewer rows than
 * rewards, else gets mDisableDirSound; the "up" / "down" arrows start hidden.
 */
extern "C" void cOVState_REWARDS_onCreateScreen(cOVState_REWARDS* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_62reward_list), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* engine2 = self->mpEngine;
    void* bg = cUIEngine_addScreenByHashName(engine2, self, GetHashValue32(str_OV_darkblue), 0);
    if (bg != 0) {
        cUIScreen_playFrame(bg, 0, 0);
    }
    cOVState_onCreateScreen(self);
    self->mItemSize = 0;
    cOVState_REWARDS_checkRewardList(self, 0, -1);
    cUIMenu* menu = (cUIMenu*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu0000_b));
    if (menu != 0) {
        menu->mAttr &= ~0x80;
        if (menu->mScrollItemNum < self->mItemSize) {
            cUIMenu_setScrollableMenu(menu, self->mItemSize);
        } else {
            // mDisableDirSound (bit 3), set with a word read-modify-write as retail does
            *(int*)&menu->mAttrMenu |= 8;
        }
    }
    cUIState_hideObjSafe(self, str_up);
    cUIState_hideObjSafe(self, str_down);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUIState_onGainFocus(void* self);
extern "C" void cOVState_REWARDS_updateMenu(cOVState_REWARDS* self, int a1);

/**
 * Runs the base UI-state focus handling, then refreshes the rewards menu.
 *
 * Calls cUIState_onGainFocus() and then cOVState_REWARDS_updateMenu(self, 0).
 */
extern "C" void cOVState_REWARDS_onGainFocus(cOVState_REWARDS* self)
{
    cUIState_onGainFocus(self);
    self->updateMenu(0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_clearUserAwards(void* iface);

/**
 * On Cross, clears the player's new-award list and lets the overlay manager move on.
 *
 * Sets the pending overlay action cOVStateManager_mStatus to 1 (cOVStateManager_update then calls cOVStateManager_gotoNextOverlay, which probably
 * triggers the next end-of-event overlay) and calls cBERewardInterface_clearUserAwards() on the reward
 * interface (0xD).
 *
 * @param pWidget  the item; ignored when null
 * @param key  the widget event; only kONNEXT (Cross) acts
 */
extern "C" void cOVState_REWARDS_onWidgetEvent(cOVState_REWARDS* self, void* pWidget, int key)
{
    if (pWidget != 0) {
        if (key == cUIState::kONNEXT) {
            cOVStateManager_mStatus = 1;
            cBERewardInterface_clearUserAwards(cBE_getInterface_Fv(cBE_getBE(), 13));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onWidgetMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cOVState_REWARDS_updateMenu(cOVState_REWARDS* self, int a);

/**
 * Handles the rewards menu's notifications: refills the rows on a scroll and picks the button feedback.
 *
 * kSCROLLUPDATE (scroll offset changed) refills the rows from @p index (cOVState_REWARDS_updateMenu). Triangle,
 * Square and Circle (kONPREVIOUS, kONOPTION, kONMISC) give 0; kSCROLLUP / kSCROLLDOWN give 0 with fewer than 10
 * rewards (+0x9C).
 *
 * @param id  the menu (On Tour pMenu); unused
 * @param event  the menu event (tUIMenuEvent: kSCROLLUP / kSCROLLDOWN scrolled, kSCROLLUPDATE offset changed,
 *               kSCROLLCHANGE selection changed, kONNEXT .. kONMISC buttons)
 * @param index  the new scroll offset for kSCROLLUP .. kSCROLLCHANGE
 * @return 1 for kSCROLLUPDATE, 0 as above, else 0x101 (animation and sound)
 */
extern "C" int cOVState_REWARDS_onWidgetMenu(cOVState_REWARDS* self, int id, int event, int index)
{
    switch (event) {
    case cUIMenu::kSCROLLUPDATE:
        self->updateMenu(index);
        return 1;
    case cUIMenu::kONPREVIOUS: case cUIMenu::kONOPTION: case cUIMenu::kONMISC:
        return 0;
    case cUIMenu::kSCROLLUP: case cUIMenu::kSCROLLDOWN:
        if (self->mItemSize < 10) {
            return 0;
        }
        /* fall through */
    case cUIMenu::kSCROLLCHANGE: case cUIMenu::kONNEXT:
        break;
    }
    return 257;
}
#endif

INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_checkRewardList);

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_updateMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cOVState_REWARDS_checkRewardList_1FFD08(char* self, unsigned short* out, int idx) __asm__("cOVState_REWARDS_checkRewardList");

struct sName_1FFD08 {
    char c[2];
};
extern char D_004A2580[];

/**
 * Fills the nine visible reward rows starting at reward @p base and shows the scroll arrows that apply.
 *
 * Rows "0".."8" get the text of reward base + i (cOVState_REWARDS_checkRewardList). With fewer than 10
 * rewards (mItemSize) every row is drawn in a fixed static colour (mStaticColour, slot 25) and made static (slot 8),
 * without a null check on the row. "up" is shown when @p base is not 0, "down" while rewards remain past base
 * + 9.
 *
 * @param base  index of the reward in the top row (On Tour offset)
 */
void cOVState_REWARDS::updateMenu(int base)
{
    sName_1FFD08 name = *(sName_1FFD08*)D_004A2580;
    unsigned short text[200];
    int i;
    for (i = 0; i < 9; i++) {
        name.c[0] = '0' + i;
        cOVState_REWARDS_checkRewardList_1FFD08(((char*)this), text, i + base);
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name.c));
        if (textObj != 0) {
            cUIText_setUnicodeString(textObj, text);
        }
        if (mItemSize < 10) {
            tARGB color;
            color.A = 1.0f;
            color.R = 0.3960784673690796f;
            color.G = 0.7215686440467834f;
            color.B = 0.7882353663444519f;
            textObj->mStaticColour = color;
            textObj->setUseStaticColour(1);
            textObj->setStatic(1);
        }
    }
    if (base == 0) {
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_up));
        if (textObj != 0) {
            textObj->setVisible(0);
        }
    } else {
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_up));
        if (textObj != 0) {
            textObj->setVisible(1);
        }
    }
    if (mItemSize <= base + 9) {
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_down));
        if (textObj != 0) {
            textObj->setVisible(0);
        }
    } else {
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_down));
        if (textObj != 0) {
            textObj->setVisible(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" char* cBENewRaceInterface_getLocationName1(void* iface);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);
extern "C" char* cBECharacterInterface_getShortName(void* iface, int id);
extern "C" int func_001FD190(int id);
extern "C" void strcpy1(unsigned short* dst, unsigned short* src);
extern "C" void func_002C2540(unsigned short* dst, char* src);
extern "C" int sprintf2(unsigned short* buf, unsigned short* fmt, ...);
// Called for its side effect here; the unit declares it int-returning.
void sprintf2_void(unsigned short* buf, unsigned short* fmt, ...) __asm__("sprintf2");
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern char str_pct_S_Jam[];
extern char str_pct_S_pct_s[];
extern char str_title_rewardlisting[];
extern char str_1text[];
extern unsigned short D_004A2590[];
extern char str_kT_OVRCMNCongratUser[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_kT_cmnRaceCaps_b[];

struct sVE_1FFF78 { short delta; short index; unsigned short* (*fn)(void*, int); };

static inline unsigned short* lookupString_1FFF78(char* name)
{
    cFELocale* g = SSXApp->FELocale;
    sVE_1FFF78* vt = (sVE_1FFF78*)VPTR_OF(cUILocaleInterface, g);
    void* t = vthis(g, vt[4]);
    return vt[4].fn(t, GetHashValue32(name));
}

/**
 * Fills in the rewards overlay's event title and congratulation text the first time it is shown.
 *
 * Runs while mDoneTransition (+0x48) is 0, then sets it. "title_rewardlisting" names the event: the peak
 * event title (func_001FD190) for the peak events (cGameEvent kGAMEEVENT_TIME_PEAK1 .. kGAMEEVENT_POINTS_PEAK3), "%S -
 * Jam" for kGAMEEVENT_RIVALPOINTS, else "%S - %s" with the location name and the mode label (cGameMode
 * kGAMEMODE_RACE / kGAMEMODE_TIMECHALLENGE race, then slopestyle, big air, pipe). "1text" gets
 * kT_OVRCMNCongratUser with the human rider's short name.
 *
 * @return always 0
 */
// Rewards overlay: fills in the event-result and reward texts the first time it is shown.
extern "C" int cOVState_REWARDS_onGainTransition(cOVState_REWARDS* self)
{
    if (self->mDoneTransition == 0) {
        unsigned short result[104];
        unsigned short fmt[0x10];
        unsigned short reward[104];
        unsigned short name[0x20];
        void* board = cBE_getInterface_Fv(cBE_getBE(), 2);
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
        int mode = cBENewRaceInterface_newRace.cGameMode;
        int kind = cBENewRaceInterface_newRace.cGameEvent;
        switch (kind) {
        case kGAMEEVENT_TIME_PEAK1 ... kGAMEEVENT_POINTS_PEAK3:
            strcpy1(result, (unsigned short*)func_001FD190(kind));
            break;
        case kGAMEEVENT_RIVALPOINTS:
            func_002C2540(fmt, str_pct_S_Jam);
            sprintf1(result, fmt, cBENewRaceInterface_getLocationName1(race));
            break;
        default: {
            unsigned short* label = 0;
            switch (mode) {
            case kGAMEMODE_RACE:
            case kGAMEMODE_TIMECHALLENGE:
                label = lookupString_1FFF78(str_kT_cmnRaceCaps_b);
                break;
            case kGAMEMODE_SLOPESTYLE:
                label = lookupString_1FFF78(str_kT_cmnSlopeCaps);
                break;
            case kGAMEMODE_BIGAIR:
                label = lookupString_1FFF78(str_kT_cmnBigAirCaps);
                break;
            case kGAMEMODE_HALFPIPE:
                label = lookupString_1FFF78(str_kT_cmnPipeCaps);
                break;
            }
            func_002C2540(fmt, str_pct_S_pct_s);
            sprintf1(result, fmt, cBENewRaceInterface_getLocationName1(race), label);
            break;
        }
        }
        cUIText_setUnicodeString(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_rewardlisting)), result);
        int id = cBENewPlayerInterface_getHumanRider(player, 0);
        void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1text));
        sprintf2_void(name, D_004A2590, cBECharacterInterface_getShortName(board, id));
        unsigned short* text = lookupString_1FFF78(str_kT_OVRCMNCongratUser);
        sprintf1(reward, text, name);
        cUIText_setUnicodeString(obj, reward);
    }
    self->mDoneTransition = 1;
    return 0;
}
#endif

