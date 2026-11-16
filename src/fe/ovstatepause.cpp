#include "common.h"
#include "online/lobbyman.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_OPTIONS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" int cUIListBox_addEntryByStringID(void* box, int id, int idx);
extern "C" int cUIListBox_getEntryByData(void* box, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* box, unsigned char sel);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" void* func_0028B180();
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
extern "C" void func_00266F90(void* self, int* a, int* b);
extern "C" void func_00266E88(void* self);
extern "C" void func_00266E30(void* self);
extern void* D_004A33F4;
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
    int f16 : 1;
    int f17 : 1;
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
    unsigned int f16 : 1;
    unsigned int f17 : 1;
    unsigned int pad18 : 12;
    unsigned int f30 : 2;
    int data[0x288 / 4 - 1];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sOptions_PO D_00535610_PO __asm__("cBEOptionInterface_mOption");

struct cBEOptionInterface_PO {
    // PORT: reinterprets the settings global with unsigned flag bits
    sOptionsU_PO getOptions() { return *(sOptionsU_PO*)&D_00535610_PO; }
};

static inline sOptions_PO Cfg_PO()
{
    return D_00535610_PO;
}

struct sSlider_PO {
    char pad[0x74];
    int value;
    int max;
};

struct sColor_PO {
    float r, g, b, a;
};
extern sColor_PO D_004C66C8_PO __asm__("D_004C66C8");

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
    virtual void setEnabled(int v);
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
    virtual void setHighlight(int v);
};

struct sVE_PO {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

// Looks up a localized string by the hash of its name.
static inline unsigned short* getString_PO(char* name)
{
    char* loc = (char*)SSXApp->FELocale;
    sVE_PO* vt = *(sVE_PO**)(loc + 4);
    char* thisp = loc + vt[4].delta;
    return vt[4].fn(thisp, GetHashValue32(name));
}

static inline void* find_PO(void* self, char* name)
{
    return cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
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

static inline char* riders_PO()
{
    return (char*)SSXApp->Game->ViewMan;
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
 * The calls declared setEnabled / setHighlight are vtable slots 8 / 25 (cUIThing_setStatic /
 * cUIText_setUseStaticColour).
 */
extern "C" void cOVState_PAUSE_OPTIONS_onCreateScreen(void* self)
{
    unsigned char sel;
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_37beoptions), 0);
    *(void**)((char*)self + 0x40) = screen;
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
            char* camera = *(char**)(riders_PO() + 4);
            if (cUIListBox_getEntryByData(w, *(int*)(*(char**)(camera + 0xA8) + 0x18), &sel))
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
            char* rs = riders_PO();
            if (*(unsigned int*)(rs + 0x10) >= 2) {
                char* camera = *(char**)(rs + 8);
                if (cUIListBox_getEntryByData(w, *(int*)(*(char**)(camera + 0xA8) + 0x18), &sel))
                    cUIListBox_setCurrentEntryByIndex(w, sel);
            }
            void* cameraLabel = find_PO(self, str_camera_2_select);
            if (cameraLabel != 0) {
                setFormatted_PO(cameraLabel, str_kT_CMNCameraVar, 2);
            }
        }
    }
    {
        sSlider_PO* w = (sSlider_PO*)find_PO(self, str_4sl_b);
        if (w != 0) {
            w->max = 12;
            w->value = iface->getOptions().f0;
            func_00287410(func_0028B180(), Cfg_PO().f0);
        }
    }
    {
        sSlider_PO* w = (sSlider_PO*)find_PO(self, str_5sl);
        if (w != 0) {
            w->max = 12;
            w->value = iface->getOptions().f8;
            func_00287488(func_0028B180(), Cfg_PO().f8);
        }
    }
    {
        sSlider_PO* w = (sSlider_PO*)find_PO(self, str_6sl);
        if (w != 0) {
            w->max = 12;
            w->value = iface->getOptions().f4;
            func_00287520(func_0028B180(), Cfg_PO().f4);
        }
    }
    {
        void* w = find_PO(self, str_7lb_b);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOff_b), 0);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOn_b), 1);
            unsigned char sel2;
            if (cUIListBox_getEntryByData(w, iface->getOptions().f17, &sel2))
                cUIListBox_setCurrentEntryByIndex(w, sel2);
            cSSXAudio_SetArcadeSFX(func_0028B180(), Cfg_PO().f17 != 0);
        }
    }
    {
        void* w = find_PO(self, str_8lb);
        if (w != 0) {
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOff_b), 0);
            cUIListBox_addEntryByStringID(w, GetHashValue32(str_kT_CMNOn_b), 1);
            unsigned char sel3;
            if (cUIListBox_getEntryByData(w, iface->getOptions().f16, &sel3))
                cUIListBox_setCurrentEntryByIndex(w, sel3);
            func_002875D0(func_0028B180(), Cfg_PO().f16 != 0);
        }
    }
    cUIObj_PO* eaTalkPair = (cUIObj_PO*)find_PO(self, str_EATalkPair);
    if (eaTalkPair != 0) {
        eaTalkPair->setVisible(0);
        eaTalkPair->setEnabled(1);
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cUIObj_PO* talkPair = (cUIObj_PO*)find_PO(self, str_EATalkPair);
        cUIObj_PO* talkItem = (cUIObj_PO*)find_PO(self, str_EATalk);
        cUIObj_PO* talkSlider = (cUIObj_PO*)find_PO(self, str_EATalksl);
        if (talkPair != 0 && talkItem != 0 && talkSlider != 0) {
            talkPair->setVisible(1);
            talkPair->setEnabled(0);
            talkItem->setVisible(1);
            talkItem->setEnabled(0);
            talkSlider->setVisible(1);
            talkSlider->setEnabled(0);
            ((sSlider_PO*)talkSlider)->max = 10;
            int level;
            int max;
            func_00266F90(D_004A33F4, &level, &max);
            ((sSlider_PO*)talkSlider)->value = level / 10;
            if (level < 10) {
                func_00266E88(D_004A33F4);
            } else {
                func_00266E30(D_004A33F4);
            }
        }
    } else {
        cUIObj_PO* eaTalk = (cUIObj_PO*)find_PO(self, str_EATalk);
        if (eaTalk != 0) {
            eaTalk->setHighlight(1);
            *(sColor_PO*)((char*)eaTalk + 0x88) = D_004C66C8_PO;
            eaTalk->setEnabled(1);
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern char cBENewRaceInterface_newRace[];
extern char D_004A24E0[];
extern char str_camera_2_select[];
extern char str_2lb_b[];
extern char str_save[];

class cUIObj_FA100 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

static inline bool isField_FA100(signed char* g, int off, int v)
{
    return g[off] == v;
}

/**
 * Greys out the in-game options that do not apply to the current game, every frame before the 2D render.
 *
 * Outside a local two-player game (a network game, or newRace cGamePath != 2) object "2" is made static (slot
 * 8, declared setEnabled) and "camera 2 select" / "2lb" are greyed out (DisableTextWidget: static, alpha at most
 * 0.5). "save" is greyed out unless cGameMode is 4 (free ride in the game-flow notes).
 */
extern "C" void cOVState_PAUSE_OPTIONS_onPreRender2D(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 || !isField_FA100((signed char*)cBENewRaceInterface_newRace, 73, 2)) {
        cUIObj_FA100* pair2 = (cUIObj_FA100*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A24E0));
        if (pair2 != 0) {
            pair2->setEnabled(1);
        }
        void* camSelect2 = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_camera_2_select));
        if (camSelect2 != 0) {
            DisableTextWidget(camSelect2);
        }
        void* label2 = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_2lb_b));
        if (label2 != 0) {
            DisableTextWidget(label2);
        }
    }
    if (!isField_FA100((signed char*)cBENewRaceInterface_newRace, 72, 4)) {
        void* saveItem = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_save));
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cOVState_PROFILE_cOVState_PROFILE(void* self, void* engine, unsigned char a2, int a3);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void func_0039F190(void* self, int a1);
extern "C" int cUIListBox_getCurrentEntryData(void* obj);
extern "C" void cChaseCameraController_switchCamera_UserChange(void* a0, int a1, void* a2);
extern "C" void* func_0028B180();
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
extern "C" void func_00266F90(void* self, int* a, int* b);
extern "C" void func_00266F58(void* self, int a, int b);
extern "C" void func_00266E88(void* self);
extern "C" void func_00266E30(void* self);
extern void* D_004A33F4;
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
    int f16 : 1;
    int f17 : 1;
    unsigned int pad18 : 12;
    unsigned int f30 : 2;
    int data[0x288 / 4 - 1];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sOptions_A238 D_00535610_A238 __asm__("cBEOptionInterface_mOption");

struct sVE_A238 {
    short delta;
    short index;
    void (*fn)(void*);
};

// The options interface (kind 4).
struct cBEOptionInterface_A238 {
    char pad[0xC];
    sVE_A238* vtbl;
    sOptions_A238 getOptions() { return D_00535610_A238; }
};

static inline void SetF0_A238(unsigned char v) { D_00535610_A238.f0 = v; }
static inline void SetF4_A238(unsigned char v) { D_00535610_A238.f4 = v; }
static inline void SetF8_A238(unsigned char v) { D_00535610_A238.f8 = v; }

static inline bool IsId_A238(char* obj, char* key)
{
    int id = *(int*)(obj + 0x38);
    return id == GetHashValue32(key);
}

static inline void* Sound_A238(int slot)
{
    char* p = (char*)SSXApp->Game->ViewMan;
    return *(void**)(p + slot);
}

/**
 * Handles the in-game options widgets: Save opens the profile save, Triangle closes the screen, and a changed
 * value is stored and applied at once.
 *
 * Cross (5) on "save" pushes a new 0x27C-byte "Save Profile" state (cOVState_PROFILE_cOVState_PROFILE, kind 0x2A) as a special
 * state; Triangle (6) closes this state (func_0039F190).
 * A value change (9) writes the widget's value into the settings word cBEOptionInterface_mOption and applies it: HUD mode,
 * player 1 / 2 camera (cChaseCameraController_switchCamera_UserChange on game view 1 / 2), music / effects /
 * speech volume, DJ and arcade audio; in a network game EATalksl sets the voice level (value x 10). The
 * options interface's virtual slot 1 then runs, probably to commit the settings.
 *
 * @param obj  the widget (On Tour pWidget); nothing happens when it is null
 * @param msg  the widget event: 5 Cross, 6 Triangle, 9 value changed
 */
// Pause-options widget handler: msg 5 opens the sub-state, 6 pops, 9 applies a changed setting.
extern "C" void cOVState_PAUSE_OPTIONS_onWidgetEvent(void* self, char* obj, unsigned int msg)
{
    if (obj == 0) {
        return;
    }
    cBEOptionInterface_A238* iface = (cBEOptionInterface_A238*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (msg) {
    case 5:
        if (IsId_A238(obj, str_save)) {
            char* stack = (char*)SSXApp->Game->UI;
            char* st = (char*)cOVState_PROFILE_cOVState_PROFILE(cMemMan_alloc(636, str_Save_Profile, 0x100, 0), *(void**)((char*)self + 0x10),
                                            *(unsigned char*)((char*)self + 0x14), 42);
            *(int*)(st + 0x1C) = (*(int*)(st + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(stack + 0x18, st, 0, 0);
        }
        break;
    case 6:
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    case 9:
        if (IsId_A238(obj, str_0lb)) {
            D_00535610_A238.f30 = *(unsigned char*)(obj + 0x319);
        } else if (IsId_A238(obj, str_1lb_b)) {
            int on = cUIListBox_getCurrentEntryData(obj);
            char* camera = (char*)Sound_A238(4);
            cChaseCameraController_switchCamera_UserChange(*(void**)(camera + 0xA8), on, camera);
        } else if (IsId_A238(obj, str_2lb_b)) {
            int on = cUIListBox_getCurrentEntryData(obj);
            char* camera = (char*)Sound_A238(8);
            cChaseCameraController_switchCamera_UserChange(*(void**)(camera + 0xA8), on, camera);
        } else if (IsId_A238(obj, str_4sl_b)) {
            SetF0_A238(*(int*)(obj + 0x74));
            func_00287410(func_0028B180(), iface->getOptions().f0);
        } else if (IsId_A238(obj, str_5sl)) {
            SetF8_A238(*(int*)(obj + 0x74));
            func_00287488(func_0028B180(), iface->getOptions().f8);
        } else if (IsId_A238(obj, str_6sl)) {
            SetF4_A238(*(int*)(obj + 0x74));
            func_00287520(func_0028B180(), iface->getOptions().f4);
        } else if (IsId_A238(obj, str_7lb_b)) {
            D_00535610_A238.f17 = cUIListBox_getCurrentEntryData(obj) != 0;
            cSSXAudio_SetArcadeSFX(func_0028B180(), iface->getOptions().f17 != 0);
        } else if (IsId_A238(obj, str_8lb)) {
            D_00535610_A238.f16 = cUIListBox_getCurrentEntryData(obj) != 0;
            func_002875D0(func_0028B180(), iface->getOptions().f16 != 0);
        }
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && IsId_A238(obj, str_EATalksl)) {
            int level;
            int max;
            func_00266F90(D_004A33F4, &level, &max);
            level = *(int*)(obj + 0x74) * 10;
            func_00266F58(D_004A33F4, level, max);
            if (level < 10) {
                func_00266E88(D_004A33F4);
            } else {
                func_00266E30(D_004A33F4);
            }
        }
        iface->vtbl[1].fn((char*)iface + iface->vtbl[1].delta);
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
extern "C" void func_0039F190(void* self, int a1);
extern char str_TransitionOut_b[];

/**
 * Closes the in-game options when an online pause or overlay timeout runs out.
 *
 * After cOVState_onUpdate(), if a cGameComm exists and its mPauseTimeout or mOverlayTimeout is 0, plays the
 * screen's "TransitionOut" frame and closes the state (func_0039F190).
 */
extern "C" void cOVState_PAUSE_OPTIONS_onUpdate(void* self)
{
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        int ok = gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(str_TransitionOut_b));
            if (frame != 0xFFFF) {
                cUIScreen_playFrame(*(void**)((char*)self + 0x40), (unsigned short)frame, 1);
            }
            func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
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
 * @param key  the menu event: 6 Cross, 7 Triangle, 8 Square, 9 Circle (1 asks whether row @p index can be
 *             selected)
 * @param index  the visible row the event is about
 * @return 0x100 (animation only) for Square, Circle and Cross on any item but "save", else 0x101 (animation
 *         and sound)
 */
// Pause-menu key filter: returns 0x100 for keys 8/9 and for key 6 unless the item is str_save, else 0x101.
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetMenu(void* self, void* menu, unsigned int key, int index)
{
    switch (key) {
    case 6: {
        int id = *(int*)((char*)cList_getByIndex((char*)menu + 0x74, index) + 0x38);
        if (id == GetHashValue32(str_save)) {
            break;
        }
    }
    case 8:
    case 9:
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
 * @param Event  the list-box event: 0-3 moves, 4 Triangle
 * @return 0x100 (animation only) for event 4, else 0x101 (animation and sound)
 */
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetListBox(void* self, int pListBox, int Event)
{
    return Event != 4 ? 257 : 0x100;
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
extern "C" int cOVState_PAUSE_OPTIONS_onWidgetPair(void* self, int pPair, int Event)
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void func_001FAFF8(void* self, bool on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible);
extern "C" void func_002583A8(void* self, int a1, int a2);
extern void* cAppMan_TheAppMan;
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

class cUIObj_1FAB18 {
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
    virtual void setVisible(int v);
};

/**
 * Builds the online-error pause screen ("265InGameError"), picks the error text and starts checking the
 * server.
 *
 * TextError (word-wrapped) explains the cGameComm error: this machine's mCheating[0] 1-3 gives ExcessiveDelay
 * and other values CheatDetected; the peer's mCheating[1], or no flag at all, ConnectionLost; mRemoteEndGame
 * or mRemoteDisconnected OpponentQuit. Every case but the first clears the +0xA0 flag that gates the ping
 * texts.
 * Text_CancelGame reads kT_BTNOK after the local finish, else kT_OVRCancelEvent (choice 0); it, the four
 * button graphics and the continue option start hidden. With a cGameComm it pings the lobby server for 3 s
 * (func_002583A8), stores the start time in +0x9C, hides Cancel (func_001FAFF8) and shows
 * kT_PauseErrorCheckingServer; without one it shows Cancel. The TextError branch reads cGameComm_mGameComm
 * before that null check.
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onCreateScreen(void* self)
{
    char* state = (char*)self;
    void* eng = *(void**)(state + 0x10);
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_265InGameError), 0);
    *(void**)(state + 0x40) = scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cUIText* subTitle = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_SubTitle));
    if (subTitle != 0) {
        cUIText_setUnicodeStringByID(subTitle, GetHashValue32(str_kT_OVROnlineError));
    }
    cUIObj_1FAB18* errorText = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_TextError));
    if (errorText != 0) {
        errorText->setVisible(1);
        cGameComm* gameComm = cGameComm_mGameComm;
        *(int*)((char*)errorText + 0x14) |= 0x80;
        int localCheating = gameComm->mCheating[0];
        int one = 1;
        if (localCheating != 0) {
            if (localCheating == one) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else if (localCheating == 2) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else if (localCheating == 3) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNExcessiveDelay));
            } else {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNCheatDetected));
            }
        } else if (gameComm->mCheating[1] != 0) {
            *(int*)(state + 0xA0) = 0;
            int peerCheating = gameComm->mCheating[1];
            if (peerCheating == one) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else if (peerCheating == 2) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else if (peerCheating == 3) {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            } else {
                cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
            }
        } else if (gameComm->mRemoteEndGame != 0) {
            cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNOpponentQuit));
            *(int*)(state + 0xA0) = 0;
        } else if (gameComm->mRemoteDisconnected != 0) {
            *(int*)(state + 0xA0) = 0;
            cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNOpponentQuit));
        } else {
            *(int*)(state + 0xA0) = 0;
            cUIText_setUnicodeStringByID((cUIText*)errorText, GetHashValue32(str_kT_POPUPCMNConnectionLost_b));
        }
        cUIObj_1FAB18* widget = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_Text_CancelGame));
        if (widget != 0) {
            if (cGameComm_mGameComm->mLocalFinishedRace != 0) {
                cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_BTNOK_b));
            } else {
                cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCancelEvent));
            }
            widget->setVisible(0);
            *(int*)((char*)widget + 0x18) = 0;
        }
        widget = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_Cancelps2X));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_Continueps2X));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_CancelButtonBack));
        if (widget != 0) {
            widget->setVisible(0);
        }
        widget = (cUIObj_1FAB18*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_ContinueButtonBack));
        if (widget != 0) {
            widget->setVisible(0);
        }
        cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(self, 0);
    }
    if (cGameComm_mGameComm != 0) {
        func_002583A8(cGameComm_mGameComm, LobbyMan_mLobbyMan->mServerAddr, 3);
        *(int*)(state + 0x9C) = *(int*)((char*)cAppMan_TheAppMan + 0x18);
        func_001FAFF8(self, 0);
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(state + 0x40), GetHashValue32(str_TextInfo));
        if (infoText != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorCheckingServer));
        }
    } else {
        func_001FAFF8(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the online-error pause overlay's gain transition at once.
 *
 * The state stack update func_0039ECB0() ends a state's gain transition once this returns nonzero.
 *
 * @return always 1 (transition finished)
 */
int cOVState_PAUSE_ONLINE_ERROR_onGainTransition(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cOVState_onUpdate(void* self);
extern "C" void func_001FAFF8(void* self, bool on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(void* self);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(void* self);
extern char str_TextInfo[];

class cUIObj_1FAF10 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07(int v);
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

/**
 * Per-frame update of the online error overlay: reports the result of the connection ping once it is known.
 *
 * Calls cOVState_onUpdate(). While a ping is pending (+0x9C) and cGameComm_mGameComm exists: if its timer (unk_138)
 * has run out with no reply (unk_13C == 0), it clears the comm's unk_09C and shows the timed-out message
 * (cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut()). If a reply came, it shows the received message
 * (cOVState_PAUSE_ONLINE_ERROR_displayPingReceived()) when a race is in progress, the local rider has not finished and
 * mCheating[0] is clear, else it hides the "TextInfo" object. In both cases it then calls func_001FAFF8(self, 1)
 * and clears +0x9C; with no reply and time left it waits.
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onUpdate(void* self)
{
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0 && *(int*)((char*)self + 0x9C) != 0) {
        int timedOut = gameComm->unk_138 <= 0 && gameComm->unk_13C == 0;
        if (timedOut) {
            gameComm->unk_09C = 0;
            cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(self);
        } else {
            if (gameComm->unk_13C == 0) {
                return;
            }
            int received = gameComm->mRaceInProgress != 0 && gameComm->mLocalFinishedRace == 0 && gameComm->mCheating[0] == 0;
            if (received) {
                cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(self);
            } else {
                ((cUIObj_1FAF10*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_TextInfo)))->setVisible(0);
            }
        }
        func_001FAFF8(self, 1);
        *(int*)((char*)self + 0x9C) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FAFF8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
    virtual void v07(int v);
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void func_001FAFF8(void* self, bool on)
{
    cUIObj_1FAFF8* o = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Text_CancelGame));
    o->setVisible(on);
    o->setEnabled(!on);
    if (on) {
        cUIObj_1FAFF8* m = (cUIObj_1FAFF8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu_b));
        m->v07(1);
        cUIMenu_setSelectedByIndex(m, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_doContinueCancel(void* self, void* item, int msg);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight(void* self, int on);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight(void* self, int on);
extern char str_Text_CancelGame[];
extern char str_Text_ContinueGame[];

/**
 * Online pause error overlay: handles its Continue/Cancel widgets.
 *
 * Ignores a null widget. Event 5 (select) runs cOVState_PAUSE_ONLINE_ERROR_doContinueCancel; event 1 (focus) highlights the widget gaining focus
 * (Text_CancelGame or Text_ContinueGame) and unhighlights the other with cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight/cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight.
 *
 * @param item  the widget
 * @param msg   the event
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onWidgetEvent(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5:
        cOVState_PAUSE_ONLINE_ERROR_doContinueCancel(self, item, msg);
        break;
    case 1: {
        int id = *(int*)((char*)item + 0x38);
        if (id == GetHashValue32(str_Text_CancelGame)) {
            cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight(self, 1);
            cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight(self, 0);
        } else {
            int id2 = *(int*)((char*)item + 0x38);
            if (id2 == GetHashValue32(str_Text_ContinueGame)) {
                cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight(self, 1);
                cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight(self, 0);
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
struct sVE_FB180 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_TextInfo[];
extern char str_kT_PauseErrorServerCheckTimedOut[];

/**
 * Reports that the server check timed out: TextInfo gets kT_PauseErrorServerCheckTimedOut.
 *
 * Called by the overlay update (cOVState_PAUSE_ONLINE_ERROR_onUpdate) when the ping runs out with no reply. The text is set only
 * while +0xA0 is set, and TextInfo's visibility is set to +0xA0 without a null check on the object.
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingTimedOut(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_TextInfo));
        if (infoText != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorServerCheckTimedOut));
        }
        sVE_FB180* vt = *(sVE_FB180**)((char*)infoText + 0x8);
        vt[9].fn((char*)infoText + vt[9].delta, *(int*)((char*)self + 0xA0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_displayPingReceived);
#ifdef SKIP_ASM
struct cUIText;
struct sVE_FB218 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible);
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
extern "C" void cOVState_PAUSE_ONLINE_ERROR_displayPingReceived(void* self)
{
    if (*(void**)((char*)self + 0x40) != 0) {
        cUIText* infoText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_TextInfo));
        if (infoText != 0 && *(int*)((char*)self + 0xA0) != 0) {
            cUIText_setUnicodeStringByID(infoText, GetHashValue32(str_kT_PauseErrorOpponentDisconnected));
        }
        sVE_FB218* vt = *(sVE_FB218**)((char*)infoText + 0x8);
        vt[9].fn((char*)infoText + vt[9].delta, *(int*)((char*)self + 0xA0));
        cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_doContinueCancel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cGame_exit(void* game, int a);
extern "C" void cAI_finishRemotePlayer(void* p);
// func_0039F190 returns its last list node in $v0; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");
extern int cPauseState_Flags;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];

/**
 * Online pause error overlay: continues the race without the lost player, or quits it.
 *
 * Pops the overlay off the UI stack and sets this state's transition bits; widget id 4 (Continue) finishes the
 * remote player with cAI_finishRemotePlayer and restores the previous pause flags from the pause stack, any
 * other widget exits the game with cGame_exit(0).
 *
 * @param item  the widget that was selected
 * @param msg   the event (not read)
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_doContinueCancel(void* self, void* item, int msg)
{
    func_0039F190_r(*(char**)((char*)self + 0x10) + 0x18, 1);
    *(int*)((char*)self + 0x1C) = (*(int*)((char*)self + 0x1C) & ~0x3F00) | 0x780;
    switch (*(int*)((char*)item + 0x18)) {
    case 4:
        cAI_finishRemotePlayer(SSXApp->Game->AI);
        cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
        break;
    case 0:
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

class cUIObj_1FB378 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

/**
 * Shows or hides the "continue racing" choice of the online-error screen.
 *
 * Text_ContinueGame gets kT_OVRContinueRacing, is static while hidden (slot 8, declared setEnabled), takes
 * visibility @p visible and choice 4, which cOVState_PAUSE_ONLINE_ERROR_doContinueCancel answers with cAI_finishRemotePlayer. When shown,
 * "Menu" selects row 0.
 *
 * @param visible  nonzero to show the option (On Tour bVisible)
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueOptionVisible(void* self, int visible)
{
    cUIObj_1FB378* continueText = (cUIObj_1FB378*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Text_ContinueGame));
    if (continueText != 0) {
        cUIText_setUnicodeStringByID((cUIText*)continueText, GetHashValue32(str_kT_OVRContinueRacing));
        continueText->setEnabled(visible ^ 1);
        continueText->setVisible(visible);
        *(int*)((char*)continueText + 0x18) = 4;
    }
    continueText = (cUIObj_1FB378*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu_b));
    if (continueText != 0 && visible) {
        cUIMenu_setSelectedByIndex(continueText, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Text_ContinueGame[];
extern char str_Continueps2X[];
extern char str_ContinueButtonBack[];

struct sColor_FB458 {
    float r, g, b, a;
    sColor_FB458(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cUIObj_FB458 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
    virtual void v10();
    virtual int setColor(const sColor_FB458& c);
};

/**
 * Highlights or unhighlights the Continue option of the online pause error overlay.
 *
 * Colours Text_ContinueGame white when @p on, else (1, 0, 0, 0), and shows the Continueps2X button and
 * ContinueButtonBack only when highlighted.
 *
 * @param on  nonzero to highlight
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setContinueHighLight(void* self, int on)
{
    cUIObj_FB458* t = (cUIObj_FB458*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Text_ContinueGame));
    cUIObj_FB458* a = (cUIObj_FB458*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Continueps2X));
    cUIObj_FB458* b = (cUIObj_FB458*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ContinueButtonBack));
    if (on) {
        t->setColor(sColor_FB458(1.0f, 1.0f, 1.0f, 1.0f));
    } else {
        t->setColor(sColor_FB458(1.0f, 0.0f, 0.0f, 0.0f));
    }
    a->setVisible(on);
    b->setVisible(on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Text_CancelGame[];
extern char str_Cancelps2X[];
extern char str_CancelButtonBack[];

struct sColor_FB588 {
    float r, g, b, a;
    sColor_FB588(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cUIObj_FB588 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
    virtual void v10();
    virtual int setColor(const sColor_FB588& c);
};

/**
 * Highlights or unhighlights the Cancel option of the online pause error overlay.
 *
 * Colours Text_CancelGame white when @p on, else (1, 0, 0, 0), and shows the Cancelps2X button and
 * CancelButtonBack only when highlighted.
 *
 * @param on  nonzero to highlight
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_setCancelHighLight(void* self, int on)
{
    cUIObj_FB588* t = (cUIObj_FB588*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Text_CancelGame));
    cUIObj_FB588* a = (cUIObj_FB588*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Cancelps2X));
    cUIObj_FB588* b = (cUIObj_FB588*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_CancelButtonBack));
    if (on) {
        t->setColor(sColor_FB588(1.0f, 1.0f, 1.0f, 1.0f));
    } else {
        t->setColor(sColor_FB588(1.0f, 0.0f, 0.0f, 0.0f));
    }
    a->setVisible(on);
    b->setVisible(on);
}
#endif

INCLUDE_ASM("fe/ovstatepause", func_001FB6B8);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBBD8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* label);
extern "C" void cUIState_onGainFocus(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_riderpct_d[];
extern char str_at_TextPulseWhiteOrange[];
extern signed char D_00535C11[];

extern "C" void func_001FBBD8(void* self)
{
    char buf[32];
    cUIState_onGainFocus(self);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    sprintf(buf, str_riderpct_d, cBENewPlayerInterface_getHumanRider(iface, 0) + 1);
    cOVState_triggerTextFade(self, buf, str_at_TextPulseWhiteOrange);
    if (D_00535C11[0] == 2) {
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
INCLUDE_ASM("fe/ovstatepause", func_001FBCC8);
#ifdef SKIP_ASM

extern "C" void func_001FBCC8(void)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FBCE0);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cOVState_onUpdate(void* self);

extern "C" void func_001FBCE0(void* self)
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
INCLUDE_ASM("fe/ovstatepause", func_001FBD20);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

struct sColour {
    float r, g, b, a;
    sColour(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

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
    virtual void setFocus(int v);
    virtual void setStatic(int v);
    virtual void setVisible(int v);
    virtual void v10();
    virtual int v11(const sColour& c);
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
    virtual void v25(int);
};

int GetHashValue32(char* str);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);


extern char cBENewRaceInterface_newRace[];

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

extern "C" void func_0020D1D8(void* self);
extern "C" char* func_00144BF0(void* iface);
extern "C" unsigned short* func_002C2540_BD20(unsigned short* dst, char* src) __asm__("func_002C2540");
extern "C" int sprintf2(unsigned short* buf, unsigned short* fmt, ...);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, ...);
extern "C" int cGameModeMan_getHeatScore(void* stats, int id, int kind);
extern "C" char* func_0014EEC8(void* iface, int id, int a2);
extern "C" void cUIState_showObjSafe(void* self, char* name);
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" signed char func_00147410(void* self, int index);
extern "C" void* func_00155130(void* iface, int idx);
extern int D_00536708[];
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
extern char D_004A2168[];
extern char str_p0rider[];
extern char str_p0run1[];
extern char str_p1rider[];
extern char str_p1dashes[];
extern char str_topScore[];


static inline unsigned short* lookupString_1FBD20(char* name)
{
    char* g = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = *(sVEntry<unsigned short*(void*, int)>**)(g + 4);
    char* t = g + vt[4].delta;
    return vt[4].fn(t, GetHashValue32(name));
}

static inline void* getObj_1FBD20(void* self, char* name)
{
    return cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001FBD20(void* self)
{
    unsigned short title[0x38];
    unsigned short line[104];
    char* s = (char*)self;
    void* eng = *(void**)(s + 0x10);
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_41freestyle_pre), 0);
    *(void**)(s + 0x40) = scr;
    if (scr != 0)
        cUIScreen_playFrame_BD20(scr, 0, 0);
    eng = *(void**)(s + 0x10);
    scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_OV_darkblue), 0);
    if (scr != 0)
        cUIScreen_playFrame_BD20(scr, 0, 0);
    func_0020D1D8(self);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    cGameModeMan* stats = SSXApp->GameModeMan;
    int mode = stats->iHeatNum;
    void* o = getObj_1FBD20(self, str_track_event);
    func_002C2540_BD20(title, func_00144BF0(iface0));
    unsigned short* str = 0;
    switch (cBENewRaceInterface_newRace[0x48]) {
    case 1:
        str = lookupString_1FBD20(str_kT_cmnSlopeCaps);
        break;
    case 2:
        str = lookupString_1FBD20(str_kT_cmnBigAirCaps);
        break;
    case 3:
        str = lookupString_1FBD20(str_kT_cmnPipeCaps);
        break;
    }
    sprintf2(line, D_004A2140, title, str);
    cUIText_setUnicodeString(o, line);
    void* text = getObj_1FBD20(self, str_objText1);
    cUIThing* sub = (cUIThing*)getObj_1FBD20(self, str_objText2);
    if (cBENewRaceInterface_newRace[0x49] == 0) {
        if (mode == 1) {
            cUIText_setUnicodeStringByID((cUIText*)text, GetHashValue32(str_kT_FSOBJ2HeatsQF));
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_FSOBJHeatsGrandTtl));
        } else if (mode == 2) {
            int total = cGameModeMan_getHeatScore(stats, D_00536708[2], 0) + cGameModeMan_getHeatScore(stats, D_00536708[2], 1);
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
        if (cBENewRaceInterface_newRace[0x49] == 2)
            sub->setVisible(0);
        else if (cBENewRaceInterface_newRace[0x48] == 1)
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_OVRCMNCollPointAndMulti));
        else
            cUIText_setUnicodeStringByID((cUIText*)sub, GetHashValue32(str_kT_OVRCMNCollPointIcons));
    }
    cUIState_hideObjSafe(self, str_heat1_tab);
    cUIState_hideObjSafe(self, str_heat2_tab);
    cUIState_hideObjSafe(self, str_finalheat_tab);
    if (cBENewRaceInterface_newRace[0x49] == 0) {
        switch (mode) {
        case 0:
        case 1:
            cUIState_showObjSafe(self, str_heat1_tab);
            break;
        case 2:
            cUIState_showObjSafe(self, str_heat2_tab);
            break;
        case 3:
            cUIState_showObjSafe(self, str_finalheat_tab);
            break;
        }
        cUIState_hideObjSafe(self, str_title_b);
    } else {
        o = getObj_1FBD20(self, str_title_b);
        if (cBENewRaceInterface_newRace[0x49] == 2)
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_CMNStateMultiPlay));
        else
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_CMNStateQuickPlay));
    }
    o = getObj_1FBD20(self, str_heat1);
    if (cBENewRaceInterface_newRace[0x49] == 0) {
        if (mode >= 3)
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNScoreColon));
        else
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNHeat1));
        if (mode == 1 || mode == 3) {
            cUIState_hideObjSafe(self, str_column_secondrun);
            cUIState_hideObjSafe(self, str_column_total);
        }
    } else {
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNScoreColon));
        cUIState_hideObjSafe(self, str_column_secondrun);
        cUIState_hideObjSafe(self, str_column_total);
    }
    {
        int k = 0;
        for (int i = 1; i < 4; i++, k++) {
            char name[0x20];
            char num[0x10];
            int id = D_00536708[k];
            if (id == cBENewPlayerInterface_getHumanRider(iface1, 0)) {
                k++;
                id = D_00536708[k];
            }
            sprintf(name, str_rider_pct_d, i);
            void* t = getObj_1FBD20(self, name);
            if (t != 0)
                cUIText_setAsciiString((cUIText*)t, func_0014EEC8(iface2, stats->iCharacters[id], stats->iCheatCharacters[id]));
            sprintf(name, str_run_1_pct_d, i);
            t = getObj_1FBD20(self, name);
            if (t != 0) {
                sprintf(num, D_004A2138, cGameModeMan_getHeatScore(stats, id, mode == 3 ? 2 : 0));
                cUIText_setAsciiString((cUIText*)t, num);
            }
            sprintf(name, str_run_2_pct_d, i);
            t = getObj_1FBD20(self, name);
            if (cBENewRaceInterface_newRace[0x49] == 0 && mode == 2) {
                char name2[0x20];
                char total[0x10];
                sprintf(num, D_004A2138_n, cGameModeMan_getHeatScore(stats, id, 1));
                cUIText_setAsciiString((cUIText*)t, num);
                sprintf(name2, str_total_pct_d, i);
                t = getObj_1FBD20(self, name2);
                int a = cGameModeMan_getHeatScore(stats, id, 0);
                int b = cGameModeMan_getHeatScore(stats, id, 1);
                sprintf(total, D_004A2138_n, a + b);
                cUIText_setAsciiString((cUIText*)t, total);
            } else {
                cUIText_setAsciiString((cUIText*)t, D_004A2168);
            }
        }
    }
    int idx = cBENewPlayerInterface_getHumanRider(iface1, 0);
    o = getObj_1FBD20(self, str_p0rider);
    cUIText_setAsciiString((cUIText*)o, func_0014EEC8(iface2, cBENewPlayerInterface_getRiderCharID(iface1, idx), func_00147410(iface1, idx)));
    o = getObj_1FBD20(self, str_p0run1);
    if (cBENewRaceInterface_newRace[0x49] == 0 && mode == 2) {
        char buf[0x10];
        int score = cGameModeMan_getHeatScore(stats, cBENewPlayerInterface_getHumanRider(iface1, 0), 0);
        sprintf(buf, D_004A2138, score);
        cUIText_setAsciiString((cUIText*)o, buf);
    } else {
        cUIText_setAsciiString((cUIText*)o, D_004A2168);
    }
    o = getObj_1FBD20(self, str_p1rider);
    if (cBENewRaceInterface_newRace[0x49] == 2) {
        idx = cBENewPlayerInterface_getHumanRider(iface1, 1);
        cUIText_setAsciiString((cUIText*)o, func_0014EEC8(iface2, cBENewPlayerInterface_getRiderCharID(iface1, idx), func_00147410(iface1, idx)));
    } else {
        ((cUIThing*)o)->setVisible(0);
        cUIState_hideObjSafe(self, str_p1dashes);
    }
    o = getObj_1FBD20(self, str_topScore);
    {
        char buf[0x10];
        void* p = func_00155130(iface8, 0);
        sprintf(buf, D_004A2138, p);
        cUIText_setAsciiString((cUIText*)o, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FC768);
#ifdef SKIP_ASM
extern "C" void cUIState_onGainFocus(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* label);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void func_002A31C0(void* self);
extern signed char D_00535C11[];
extern char str_at_TextPulseWhiteOrange[];
extern char str_p0rider[];
extern char str_p0run1[];
extern char str_p1rider[];
extern char str_p0dashes[];
extern char str_p1dashes[];

extern "C" void func_001FC768(void* self)
{
    cUIState_onGainFocus(self);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int* mode = (int*)SSXApp->GameModeMan;
    cOVState_triggerTextFade(self, str_p0rider, str_at_TextPulseWhiteOrange);
    if (*mode == 2) {
        cOVState_triggerTextFade(self, str_p0dashes, str_at_TextPulseWhiteOrange);
    } else {
        cOVState_triggerTextFade(self, str_p0run1, str_at_TextPulseWhiteOrange);
    }
    if (D_00535C11[0] == 2) {
        cOVState_triggerTextFade(self, str_p1rider, str_at_TextPulseWhiteOrange);
        cOVState_triggerTextFade(self, str_p1dashes, str_at_TextPulseWhiteOrange);
    }
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
    func_002A31C0(func_0028B180());
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" char* func_00144BF0(void* iface);
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
extern char cBENewRaceInterface_newRace[];
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
    char* g = (char*)SSXApp->FELocale;
    sVE_1FC878* vt = *(sVE_1FC878**)(g + 4);
    char* t = g + vt[4].delta;
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

static inline void* getObj_1FC878(void* self, char* name)
{
    return cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001FC878(void* self)
{
    unsigned short title[0x38];
    unsigned short line[104];
    char rider[0x20];
    unsigned short label[304];
    char num[0x20];
    char* s = (char*)self;
    void* eng = *(void**)(s + 0x10);
    void* scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_50multifs_pre), 0);
    *(void**)(s + 0x40) = scr;
    if (scr != 0)
        cUIScreen_playFrame(scr, 0, 0);
    eng = *(void**)(s + 0x10);
    scr = cUIEngine_addScreenByHashName(eng, self, GetHashValue32(str_OV_darkblue), 0);
    if (scr != 0)
        cUIScreen_playFrame(scr, 0, 0);
    cOVState_onCreateScreen(self);
    signed char* gs = (signed char*)cBENewRaceInterface_newRace;
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    int mode4A = getField_1FC878(gs, 74);
    int mode48 = getField_1FC878(gs, 72);
    void* o = getObj_1FC878(self, str_title1);
    cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_CMNStateMultiPlay));
    o = getObj_1FC878(self, str_title2);
    func_002C2540(title, func_00144BF0(iface0));
    unsigned short* str = 0;
    switch (getField_1FC878(gs, 72))
    {
    case 5:
        str = lookupString_1FC878(str_kT_cmnRaceCaps_b);
        break;
    case 1:
        str = lookupString_1FC878(str_kT_cmnSlopeCaps);
        break;
    case 2:
        str = lookupString_1FC878(str_kT_cmnBigAirCaps);
        break;
    case 3:
        str = lookupString_1FC878(str_kT_cmnPipeCaps);
        break;
    }
    unsigned short* lineFmt = D_004A2140;
    if (mode48 == 6)
        sprintf2(line, D_0046E558, title, str);
    else
        sprintf2(line, lineFmt, title);
    cUIText_setUnicodeString(o, line);
    o = getObj_1FC878(self, str_name);
    if (getField_1FC878((signed char*)cBENewRaceInterface_newRace, 73) == 2)
        cUIText_setAsciiString((cUIText*)o, func_00146F88(iface1, 0));
    else
        cUIText_setAsciiString((cUIText*)o, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
    cUIText* status = (cUIText*)getObj_1FC878(self, str_obj1);
    if (mode48 >= 1 && mode48 <= 3)
    {
        cUIState_showObjSafe(self, str_obj2);
        cUIState_showObjSafe(self, str_bullet2);
        o = getObj_1FC878(self, str_obj2);
        if (mode48 == 1)
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNCollPointAndMulti));
        else
            cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNCollPointIcons));
    }
    else
    {
        cUIState_hideObjSafe(self, str_obj2);
        cUIState_hideObjSafe(self, str_bullet2);
    }
    o = getObj_1FC878(self, str_title1);
    cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_CMNStateMultiPlay));
    if (getField_1FC878((signed char*)cBENewRaceInterface_newRace, 73) == 2)
        strcpy(rider, func_00146F88(iface1, 1));
    else
        strcpy(rider, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 1)));
    unsigned short* fmt = lookupString_1FC878(str_kT_OVRCMNVs);
    sprintf1(label, fmt, rider);
    cUIText_setUnicodeString(getObj_1FC878(self, str_vs), label);
    if (mode48 >= 2 && mode48 <= 3)
    {
        cUIState_hideObjSafe(self, str_screendata1);
    }
    else if (mode48 >= 5 && mode48 <= 6)
    {
        cUIState_hideObjSafe(self, str_screendata);
        cUIState_hideObjSafe(self, str_screendata1);
    }
    if (mode4A == 4)
        cUIText_setUnicodeStringByID(status, GetHashValue32(str_kT_HELPFirstToBottom));
    else
        cUIText_setUnicodeStringByID(status, GetHashValue32(str_kT_OVRCMNBeatOppMaxTrick));
    o = getObj_1FC878(self, str_topScoreLabel);
    if (mode4A == 4 || mode4A == 0)
    {
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNRecordTime));
        void* p = func_00155130(iface8, 0);
        o = getObj_1FC878(self, str_topScore);
        cUIText_setAsciiString((cUIText*)o, func_0020A1A8(p));
    }
    else
    {
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_OVRCMNRecordScore));
        void* p = func_00155130(iface8, 0);
        o = getObj_1FC878(self, str_topScore);
        sprintf(num, D_004A2138, p);
        cUIText_setAsciiString((cUIText*)o, num);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FCEC0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern signed char D_00535C10[];
extern char str_screendata[];
extern char str_vsGroup[];
extern char str_objectives[];
extern char str_topScoreGroup[];

struct sVec3_1FCEC0
{
    float x, y, z;
    sVec3_1FCEC0() {}
    sVec3_1FCEC0(float a, float b, float c) : x(a), y(b), z(c) {}
};

extern "C" int func_001FCEC0(void* self, void* msg)
{
    if (msg != 0)
    {
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = D_00535C10[0];
    if (mode == 2 || mode == 3)
    {
        char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_screendata));
        sVec3_1FCEC0 p = *(sVec3_1FCEC0*)(o + 0x44);
        *(sVec3_1FCEC0*)(o + 0x44) = sVec3_1FCEC0(p.x + 130.0f, p.y, p.z);
    }
    else if (mode == 5 || mode == 6)
    {
        char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_vsGroup));
        sVec3_1FCEC0 p = *(sVec3_1FCEC0*)(o + 0x44);
        *(sVec3_1FCEC0*)(o + 0x44) = sVec3_1FCEC0(p.x, p.y + 40.0f, p.z);
        o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_objectives));
        p = *(sVec3_1FCEC0*)(o + 0x44);
        *(sVec3_1FCEC0*)(o + 0x44) = sVec3_1FCEC0(p.x, p.y + 67.0f, p.z);
        o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_topScoreGroup));
        p = *(sVec3_1FCEC0*)(o + 0x44);
        *(sVec3_1FCEC0*)(o + 0x44) = sVec3_1FCEC0(p.x, p.y - 48.0f, p.z);
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD0D8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void cUIState_onGainFocus(void* self);

extern "C" void func_001FD0D8(void* self)
{
    cUIState_onGainFocus(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = 1200;
    }
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD118);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_002872A8(void* self);

extern "C" void func_001FD118(void)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        gameComm->mOverlayTimeout = -1;
    }
    func_002872A8(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD150);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cOVState_onUpdate(void* self);

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

// Sends the hash of the named pause-menu item to the front end's screen object.
static inline int sendHash_1FD190(char* name)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEnt_1FD190* vt = *(sVEnt_1FD190**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}
extern "C" int func_001FD190(int id)
{
    switch (id) {
    case 6: return sendHash_1FD190(str_kT_EventPk1Race_b);
    case 7: return sendHash_1FD190(str_kT_EventPk2Race_b);
    case 8: return sendHash_1FD190(str_kT_EventAllPeakRace_b);
    case 9: return sendHash_1FD190(str_kT_EventPk1Jam_b);
    case 10: return sendHash_1FD190(str_kT_EventPk2Jam_b);
    case 11: return sendHash_1FD190(str_kT_EventAllPeakJam_b);
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
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern char str_68rival_pre[];
extern char str_OV_darkblue[];

extern "C" void func_001FD268(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_68rival_pre), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cSSXAudio_MenuSound(func_0028B180(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FD320);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getCharacterID(int idx);
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int func_00145750(void* iface);
extern "C" char* func_0014EEC8(void* iface, int id, int a2);
extern "C" int cBENewPlayerInterface_isEventLocked(void* iface, int a1, int charId, int item);
extern "C" int func_00157B08(void* iface, int a1, int a2);
extern "C" void* func_004139F8(float x);
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
    char* g = (char*)SSXApp->FELocale;
    sVE_1FD320* vt = *(sVE_1FD320**)(g + 4);
    char* t = g + vt[4].delta;
    return vt[4].fn(t, GetHashValue32(name));
}

static inline int getField_1FD320(signed char* g, int off)
{
    return g[off];
}

static inline void* getObj_1FD320(void* self, char* name)
{
    return cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" int func_001FD320(void* self, void* msg)
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
        char* stats = (char*)SSXApp->GameModeMan;
        int mode = getField_1FD320((signed char*)cBENewRaceInterface_newRace, 74);
        void* o = getObj_1FD320(self, str_name);
        cUIText_setAsciiString((cUIText*)o, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
        o = getObj_1FD320(self, str_title2);
        switch (mode) {
        case 4:
            func_002C2540_r(title, func_00144BF0(iface0));
            func_002C2540_r(fmt, (char*)D_004A2140);
            sprintf1(out, fmt, title, lookupString_1FD320(str_kT_cmnRaceCaps_b));
            cUIText_setUnicodeString(o, out);
            break;
        case 5:
            func_002C2540_r(title, func_00144BF0(iface0));
            func_002C2540_r(fmt, (char*)D_0046E558);
            sprintf1(out, fmt, title);
            cUIText_setUnicodeString(o, out);
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            cUIText_setUnicodeString(o, (unsigned short*)func_001FD190(mode));
            break;
        }
        cUIText* statusA = (cUIText*)getObj_1FD320(self, str_obj1);
        cUIText* statusB = (cUIText*)getObj_1FD320(self, str_obj2);
        o = getObj_1FD320(self, str_title1);
        cUIText_setUnicodeStringByID((cUIText*)o, GetHashValue32(str_kT_TITLERivalChallenge));
        func_002C2540_r(name, func_0014EEC8(iface2, func_00145750(iface0), 0));
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
        case 4:
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPFirstToBottom));
            cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatChallengePeakRace));
            if (getField_1FD320((signed char*)cBENewRaceInterface_newRace, 73) == 1
                || (cBENewRaceInterface_getLocationID(iface0) == 0xE && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 6))
                || (cBENewRaceInterface_getLocationID(iface0) == 0xF && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 7))
                || (cBENewRaceInterface_getLocationID(iface0) == 0x10 && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 8))) {
                cUIState_hideObjSafe(self, str_obj2);
                cUIState_hideObjSafe(self, str_bullet2);
            }
            cUIState_hideObjSafe(self, str_timescoretobeat);
            break;
        case 5:
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPGetToBottomPoints));
            cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatChallPeakJam));
            if (getField_1FD320((signed char*)cBENewRaceInterface_newRace, 73) == 1
                || (cBENewRaceInterface_getLocationID(iface0) == 0xE && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 9))
                || (cBENewRaceInterface_getLocationID(iface0) == 0xF && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 10))
                || (cBENewRaceInterface_getLocationID(iface0) == 0x10 && !cBENewPlayerInterface_isEventLocked(iface1, 0, cBELibrary_getCharacterID(0), 11))) {
                cUIState_hideObjSafe(self, str_obj2);
                cUIState_hideObjSafe(self, str_bullet2);
            }
            cUIState_hideObjSafe(self, str_timescoretobeat);
            break;
        case 6:
        case 7:
        case 8: {
            o = getObj_1FD320(self, str_timescoretobeat);
            float mins = (float)*(unsigned int*)(stats + 0x78) * 0.01666666753590107f;
            void* r = func_004139F8(mins);
            func_002C2540_r(num, func_0020A1A8(r));
            unsigned short* f = lookupString_1FD320(str_kT_OVRCMNTimeToBeatColon);
            func_002C2540_r(title, D_004A2550);
            sprintf1(text, title, f, num);
            cUIText_setUnicodeStringWrap(o, text);
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPRaceToBottom));
            if (mode == 8 || (mode == 7 && !func_00157B08(iface13, 0, 2)) || (mode == 6 && !func_00157B08(iface13, 0, 1)))
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedTime));
            else
                cUIText_setUnicodeStringByID(statusB, GetHashValue32(str_kT_HELPBeatPostedTimePass));
            break;
        }
        case 9:
        case 10:
        case 11: {
            int n = *(int*)(stats + 0x7C);
            unsigned short* f = lookupString_1FD320(str_kT_OVRCMNScoreToBeatColon);
            func_002C2540_r(title, str_pct_s_pct_d_c);
            sprintf1(num, title, f, n);
            cUIText_setUnicodeStringWrap(getObj_1FD320(self, str_timescoretobeat), num);
            cUIText_setUnicodeStringByID(statusA, GetHashValue32(str_kT_HELPMakeWayDnSlope));
            if (mode == 11 || (mode == 10 && !func_00157B08(iface13, 0, 2)) || (mode == 9 && !func_00157B08(iface13, 0, 1)))
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
extern "C" int func_001FDB78(void* self, int menu, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDB88);
#ifdef SKIP_ASM
struct sPauseMgr_DB88 {
    char pad[0x8];
    int f8;
};

extern "C" void func_002872A8(void* snd);

extern "C" void func_001FDB88(void* self, int a1, int a2)
{
    if (a1 != 0 && a2 == 5) {
        cOVStateManager_mStatus = 3;
        ((sPauseMgr_DB88*)((SSXApp)->Game)->mCurrentState)->f8 = 1;
        func_002872A8(func_0028B180());
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDBF0);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_00472440[];

extern "C" void* func_001FDBF0(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 1, a2);
    *(void***)((char*)self + 0x8) = D_00472440;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDC30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cUIEngine_addScreenByHashName_DC30(void* engine, void* owner, int hash, int a3) __asm__("cUIEngine_addScreenByHashName");
extern "C" void* cUIScreen_playFrame_DC30(void* self, unsigned short frame, int flag) __asm__("cUIScreen_playFrame");
extern "C" void* cUIScreen_getObjectByHashName_DC30(void* self, int hash) __asm__("cUIScreen_getObjectByHashName");
extern "C" void cOVState_onCreateScreen_DC30(void* self) __asm__("cOVState_onCreateScreen");
extern int cPauseState_Flags;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];
extern char D_004A2560[];
extern char str_119Save_Replay[];
extern char str_1percentage_b[];
extern char str_title_Save_Replay[];
extern char str_title_Save_Records[];
extern char str_helptext01[];
extern char str_Memory_card_info[];
extern char str_pct_dmemory_card_b[];
extern char str_OV_darkblue[];

struct cUIObj_DC30 {
    int pad[2];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void setVisible(int on);
};

static inline cUIObj_DC30* getObj_DC30(char* self, char* name)
{
    return (cUIObj_DC30*)cUIScreen_getObjectByHashName_DC30(*(void**)(self + 0x40), GetHashValue32(name));
}

// Builds the memory-card device-select overlay and pushes its menu state.
extern "C" void func_001FDC30(char* self)
{
    void* engine = *(void**)(self + 0x10);
    void* screen = cUIEngine_addScreenByHashName_DC30(engine, self, GetHashValue32(str_119Save_Replay), 0);
    *(void**)(self + 0x40) = screen;
    if (screen != 0) {
        cUIObj_DC30* o;
        cUIScreen_playFrame_DC30(screen, 0, 0);
        o = getObj_DC30(self, str_1percentage_b);
        cUIText_setAsciiString((cUIText*)o, D_004A2560);
        o = getObj_DC30(self, str_title_Save_Replay);
        o->setVisible(1);
        o = getObj_DC30(self, str_title_Save_Records);
        o->setVisible(0);
        o = getObj_DC30(self, str_helptext01);
        o->setVisible(0);
        o = getObj_DC30(self, str_Memory_card_info);
        cUIText_setAsciiString((cUIText*)o, D_004A2560);
        char name[0x20];
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            o = getObj_DC30(self, name);
            *(int*)((char*)o + 0x18) = i - 1;
        }
    }
    engine = *(void**)(self + 0x10);
    screen = cUIEngine_addScreenByHashName_DC30(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame_DC30(screen, 0, 0);
    }
    D_005366E8[++D_004A2A54] = 2;
    cPauseState_Flags = D_004428F0[2];
    cOVState_onCreateScreen_DC30(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDE60);
#ifdef SKIP_ASM
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_004428F0[];
void func_00270ED8(void* self);
extern "C" void* cFEMemCard_onDestroyScreen(void* self);
extern "C" void cReplay_startReplay(void* self, void* a1, int a2, int a3);

extern "C" void func_001FDE60(void* self)
{
    if (*(int*)((char*)self + 0x1C8) == 1) {
        func_00270ED8(SSXApp->Game->Replay);
        *(int*)((char*)self + 0x1C8) = 0;
    }
    cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
    cFEMemCard_onDestroyScreen(self);
    cReplay_startReplay(SSXApp->Game->Replay, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDF00);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FDF00(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FDF40);
#ifdef SKIP_ASM
class cMCSub_DF40 {
public:
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
    virtual void v14(int, void*);
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
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54(int);
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual void v64(void*);
    virtual int v65();
    virtual int v66(int);
    virtual int v67(int);
};

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
    virtual void v35(int);
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual void v43();
};

class cUIObj_DF40 {
public:
    int pad[2];
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual void v08(int);
    virtual void v09(int);
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
    virtual void v25(int);
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

extern "C" void func_001D8240();
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

static inline cUIObj_DF40* getObj_DF40(cState_DF40* self, char* name)
{
    return (cUIObj_DF40*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001FDF40(cState_DF40* self)
{
    unsigned short buf[800];
    char name[0x40];
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    if (self->v42()) {
        if (!(*(cMCSub_DF40**)(mp + 0x434))->v53()) {
            func_001D8240();
        } else if (*(int*)((char*)self + 0x214) != 3) {
            self->v35(3);
        }
    }
    if (self->v42()) {
        int sel = 0;
        self->v43();
        cMCOverlayManager_GetDeviceDisplayString(mp, *(int*)(mp + 0x428), buf, 800);
        cUIText_setUnicodeStringWrap(getObj_DF40(self, str_Memory_card_info), buf);
        cUIObj_DF40* o = getObj_DF40(self, str_1percentage_b);
        (*(cMCSub_DF40**)(mp + 0x434))->v64(buf);
        cUIText_setUnicodeString(o, buf);
        o->v09(0);
        char* g = (char*)SSXApp->FELocale;
        sVE_DF40* vt = *(sVE_DF40**)(g + 4);
        char* t = g + vt[4].delta;
        vt[4].fn(t, GetHashValue32(str_kT_MEMEmpty_b));
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            o = getObj_DF40(self, name);
            (*(cMCSub_DF40**)(mp + 0x434))->v14(i - 1, (char*)self + 0x230);
            cUIText_setUnicodeString(o, (unsigned short*)((char*)self + 0x230));
            if (!(*(cMCSub_DF40**)(mp + 0x434))->v67(i - 1) && !(*(cMCSub_DF40**)(mp + 0x434))->v66(i - 1)
                && (*(cMCSub_DF40**)(mp + 0x434))->v55()) {
                if (sel == 0) {
                    sel = 1;
                    o->v25(0);
                    o->v08(0);
                } else {
                    o->v25(1);
                    o->v08(1);
                }
            } else if ((*(cMCSub_DF40**)(mp + 0x434))->v54(*(int*)(mp + 0x428))) {
                if ((*(cMCSub_DF40**)(mp + 0x434))->v66(i - 1)) {
                    o->v08(1);
                    o->v25(1);
                } else {
                    o->v08(0);
                    o->v25(0);
                }
            } else {
                o->v25(1);
                o->v08(1);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 1) == -1)
            getObj_DF40(self, str_arrw_left_b)->v09(0);
        else
            getObj_DF40(self, str_arrw_left_b)->v09(1);
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 0) == -1)
            getObj_DF40(self, str_arrw_right_b)->v09(0);
        else
            getObj_DF40(self, str_arrw_right_b)->v09(1);
    }
    if (*(int*)((char*)self + 0x214) != 3) {
        for (int i = 1; i < 5; i++) {
            sprintf(name, str_pct_dmemory_card_b, i);
            cUIObj_DF40* o = getObj_DF40(self, name);
            o->v25(0);
            o->v08(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE4A8);
#ifdef SKIP_ASM
class cUIObj_1FE4A8 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];
extern char str_o_delete[];
extern char str_ps2sq_b[];

extern "C" void func_001FE4A8(void* self, bool on)
{
    if (*(int*)((char*)self + 0x22C) != on) {
        int off = !on;
        ((cUIObj_1FE4A8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x_b)))->setVisible(off);
        ((cUIObj_1FE4A8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_x_save)))->setVisible(off);
        ((cUIObj_1FE4A8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_o_delete)))->setVisible(off);
        ((cUIObj_1FE4A8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq_b)))->setVisible(off);
        *(int*)((char*)self + 0x22C) = on;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE5B0);
#ifdef SKIP_ASM
class cUIObj_1FE5B0 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_o_delete[];
extern char str_ps2sq_b[];

extern "C" void func_001FE5B0(void* self, bool on)
{
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_o_delete)))->setVisible(!on);
    ((cUIObj_1FE5B0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq_b)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE648);
#ifdef SKIP_ASM
class cUIObj_1FE648 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];

extern "C" void func_001FE648(void* self, int loadBlocked, bool on)
{
    cUIObj_1FE648* xButton = (cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x_b));
    ((cUIObj_1FE648*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_x_save)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    xButton->setVisible(!on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE6E8);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* self);
extern "C" void func_0023CAE8(void* self, int a, int b);
void func_00270ED8(void* self);
// PORT: the unit declares cMCOverlayManager_SetAction as returning void*; this caller ignores the result (void view by asm label).
extern "C" void cMCOverlayManager_SetAction_v(void* self, int mode) __asm__("cMCOverlayManager_SetAction");

class cMovieSub434_1FE6E8 {
public:
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
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54(int);
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67(int);
};

class cMoviePlayer_1FE6E8 {
public:
    char pad[0x748];
    virtual void v01(int);
};

extern "C" void func_001FE6E8(void* self, void* msg, unsigned int type)
{
    if (msg == 0)
        return;
    if (*(int*)((char*)self + 0x1A8) != 0)
        return;
    switch (type)
    {
    case 5:
    {
        int one = 1;
        if (*(int*)((char*)self + 0x1C0) == 6)
            return;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        int st = *(int*)((char*)self + 0x214);
        if (st != 3)
            return;
        int v = *(int*)((char*)msg + 0x18);
        *(int*)((char*)self + 0x1C4) = v;
        *(int*)(mp + 0xF8) = v;
        *(int*)((char*)self + 0x1A8) = one;
        if (*(int*)((char*)self + 0x1C8) == one)
        {
            func_00270ED8(SSXApp->Game->Replay);
            *(int*)((char*)self + 0x1C8) = 0;
        }
        func_0023CAE8(mp, 0, -1);
        *(int*)((char*)self + 0x1DC) = one;
        *(int*)((char*)self + 0x1A8) = one;
        *(int*)((char*)self + 0x1C0) = st;
        break;
    }
    case 6:
    {
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (*(int*)((char*)self + 0x1A8) == 1)
            return;
        int s = *(int*)((char*)self + 0x1C0);
        if (s == 6)
            return;
        *(int*)((char*)self + 0x1E4) = 1;
        if (s == 0 || s == 2 || s == 1)
            ((cMoviePlayer_1FE6E8*)mp)->v01(0);
        *(int*)((char*)self + 0x1C0) = 6;
        break;
    }
    case 7:
    {
        int one = 1;
        if (*(int*)((char*)self + 0x1C0) == 6)
            return;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (!(*(cMovieSub434_1FE6E8**)(mp + 0x434))->v54(*(int*)(mp + 0x428)))
            return;
        int v = *(int*)((char*)msg + 0x18);
        *(int*)((char*)self + 0x1C4) = v;
        *(int*)(mp + 0xF8) = v;
        int s = *(int*)((char*)self + 0x1C0);
        if (s != one)
            return;
        if (!(*(cMovieSub434_1FE6E8**)(mp + 0x434))->v67(*(int*)((char*)self + 0x1C4)))
            return;
        *(int*)((char*)self + 0x1A8) = s;
        *(int*)((char*)self + 0x1C0) = 4;
        cMCOverlayManager_SetAction_v(mp, 0);
        ((cMoviePlayer_1FE6E8*)mp)->v01(0x18);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE8F0);
#ifdef SKIP_ASM
void func_00270ED8(void* self);
extern "C" void func_001D8700(void* self, void* a1, int key);

extern "C" void func_001FE8F0(void* self, void* popup, int key)
{
    if (key == 22) {
        if (*(int*)((char*)self + 0x19C) == 5) {
            func_00270ED8(SSXApp->Game->Replay);
            *(int*)((char*)self + 0x1C8) = 0;
        }
    }
    func_001D8700(self, popup, key);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE968);
#ifdef SKIP_ASM
extern "C" void* func_001D5330(void* self, int a1, int a2, int a3);
extern void* D_004722B8[];

extern "C" void* func_001FE968(void* self, int a1, int a2)
{
    func_001D5330(self, a1, 2, a2);
    *(void***)((char*)self + 0x8) = D_004722B8;
    *(int*)((char*)self + 0x22C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FE9A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
extern char D_004A2560[];

class cUIObj_1FE9A8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
};

static inline void* engine_1FE9A8(char* self)
{
    return *(void**)(self + 0x10);
}

extern "C" void func_001FE9A8(char* self)
{
    void* scr = cUIEngine_addScreenByHashName(engine_1FE9A8(self), self, GetHashValue32(str_119Save_Replay), 0);
    *(void**)(self + 0x40) = scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_1percentage_b)), D_004A2560);
        cUIObj_1FE9A8* o = (cUIObj_1FE9A8*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_title_Save_Replay));
        o->v09(0);
        o = (cUIObj_1FE9A8*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_title_Save_Records));
        o->v09(1);
        o = (cUIObj_1FE9A8*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_helptext01));
        o->v09(1);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_Memory_card_info)), D_004A2560);
        for (int i = 1; i < 5; i++) {
            char buf[0x20];
            sprintf(buf, str_pct_dmemory_card_b, i);
            o = (cUIObj_1FE9A8*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(buf));
            if (i == 1) {
                *(int*)((char*)o + 0x18) = 0;
            } else {
                o->v08(1);
                o->v09(0);
            }
            sprintf(buf, D_004A2138, i);
            o = (cUIObj_1FE9A8*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(buf));
            if (i >= 2)
                o->v09(0);
        }
    }
    void* s2 = cUIEngine_addScreenByHashName(engine_1FE9A8(self), self, GetHashValue32(str_OV_darkblue), 0);
    if (s2 != 0)
        cUIScreen_playFrame(s2, 0, 0);
    cOVState_onCreateScreen(self);
}
#endif

extern "C" void* cFEMemCard_onDestroyScreen(void* self);

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC40__FPv);
#ifdef SKIP_ASM
void* func_001FEC40(void* self)
{
    return cFEMemCard_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FEC60);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_001FEC60(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FECA0);
#ifdef SKIP_ASM
class cMCSub_ECA0 {
public:
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
    virtual void v14(int, void*);
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
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54(int);
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual void v64(void*);
    virtual int v65();
    virtual int v66(int);
    virtual int v67(int);
};

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
    virtual void v35(int);
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual void v43();
};

class cUIObj_ECA0 {
public:
    int pad[2];
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual void v08(int);
    virtual void v09(int);
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
    virtual void v25(int);
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

extern "C" void func_001D8240();
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

static inline cUIObj_ECA0* getObj_ECA0(cState_ECA0* self, char* name)
{
    return (cUIObj_ECA0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

extern "C" void func_001FECA0(cState_ECA0* self)
{
    unsigned short buf[800];
    char name[0x40];
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    if (self->v42()) {
        if (!(*(cMCSub_ECA0**)(mp + 0x434))->v53()) {
            func_001D8240();
        } else if (*(int*)((char*)self + 0x214) != 3) {
            self->v35(3);
        }
    }
    if (self->v42()) {
        self->v43();
        cMCOverlayManager_GetDeviceDisplayString(mp, *(int*)(mp + 0x428), buf, 800);
        cUIText_setUnicodeStringWrap(getObj_ECA0(self, str_Memory_card_info), buf);
        cUIObj_ECA0* o = getObj_ECA0(self, str_1percentage_b);
        (*(cMCSub_ECA0**)(mp + 0x434))->v64(buf);
        cUIText_setUnicodeString(o, buf);
        o->v09(0);
        char* g = (char*)SSXApp->FELocale;
        sVE_ECA0* vt = *(sVE_ECA0**)(g + 4);
        char* t = g + vt[4].delta;
        vt[4].fn(t, GetHashValue32(str_kT_MEMEmpty_b));
        sprintf(name, str_pct_dmemory_card_b, 1);
        o = getObj_ECA0(self, name);
        (*(cMCSub_ECA0**)(mp + 0x434))->v14(0, (char*)self + 0x230);
        cUIText_setUnicodeString(o, (unsigned short*)((char*)self + 0x230));
        if (!(*(cMCSub_ECA0**)(mp + 0x434))->v67(0) && !(*(cMCSub_ECA0**)(mp + 0x434))->v66(0)
            && (*(cMCSub_ECA0**)(mp + 0x434))->v55()) {
            o->v25(0);
            o->v08(0);
        } else if ((*(cMCSub_ECA0**)(mp + 0x434))->v54(*(int*)(mp + 0x428))) {
            if ((*(cMCSub_ECA0**)(mp + 0x434))->v66(0)) {
                o->v08(1);
                o->v25(1);
            } else {
                o->v08(0);
                o->v25(0);
            }
        } else {
            o->v25(1);
            o->v08(1);
        }
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 1) == -1)
            getObj_ECA0(self, str_arrw_left_b)->v09(0);
        else
            getObj_ECA0(self, str_arrw_left_b)->v09(1);
        if (cFEMemCard_checkNextAvailableDevice(self, mp, *(int*)(mp + 0x428), 0) == -1)
            getObj_ECA0(self, str_arrw_right_b)->v09(0);
        else
            getObj_ECA0(self, str_arrw_right_b)->v09(1);
    }
    if (*(int*)((char*)self + 0x214) != 3) {
        sprintf(name, str_pct_dmemory_card_b, 1);
        cUIObj_ECA0* o = getObj_ECA0(self, name);
        o->v25(0);
        o->v08(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF170);
#ifdef SKIP_ASM
class cUIObj_1FF170 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];
extern char str_o_delete[];
extern char str_ps2sq_b[];

extern "C" void func_001FF170(void* self, bool on)
{
    if (*(int*)((char*)self + 0x22C) != on) {
        int off = !on;
        ((cUIObj_1FF170*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x_b)))->setVisible(off);
        ((cUIObj_1FF170*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_x_save)))->setVisible(off);
        ((cUIObj_1FF170*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_o_delete)))->setVisible(off);
        ((cUIObj_1FF170*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq_b)))->setVisible(off);
        *(int*)((char*)self + 0x22C) = on;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF278);
#ifdef SKIP_ASM
class cUIObj_1FF278 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_o_delete[];
extern char str_ps2sq_b[];

extern "C" void func_001FF278(void* self, bool on)
{
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_o_delete)))->setVisible(!on);
    ((cUIObj_1FF278*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq_b)))->setVisible(!on);
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF310);
#ifdef SKIP_ASM
class cUIObj_1FF310 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];

extern "C" void func_001FF310(void* self, int loadBlocked, bool on)
{
    cUIObj_1FF310* xButton = (cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x_b));
    ((cUIObj_1FF310*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_x_save)))->setVisible(!on);
    *(int*)((char*)self + 0x224) = !on;
    xButton->setVisible(!on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", func_001FF3B0);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* self);
extern "C" void func_0023CAE8(void* self, int a, int b);
// PORT: the unit declares cMCOverlayManager_SetAction as returning void*; this caller ignores the result (void view by asm label).
extern "C" void cMCOverlayManager_SetAction_v(void* self, int mode) __asm__("cMCOverlayManager_SetAction");

class cMovieSub434_1FF3B0 {
public:
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
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int v53();
    virtual int v54(int);
    virtual int v55();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual int v64();
    virtual int v65();
    virtual int v66();
    virtual int v67(int);
};

class cMoviePlayer_1FF3B0 {
public:
    char pad[0x748];
    virtual void v01(int);
};

extern "C" void func_001FF3B0(void* self, void* msg, unsigned int type)
{
    if (msg == 0)
        return;
    if (*(int*)((char*)self + 0x1A8) != 0)
        return;
    switch (type)
    {
    case 5:
    {
        int one = 1;
        if (*(int*)((char*)self + 0x1C0) == 6)
            return;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        int st = *(int*)((char*)self + 0x214);
        if (st != 3)
            return;
        int v = *(int*)((char*)msg + 0x18);
        *(int*)((char*)self + 0x1C4) = v;
        *(int*)(mp + 0xF8) = v;
        *(int*)((char*)self + 0x1A8) = one;
        func_0023CAE8(mp, 2, -1);
        *(int*)((char*)self + 0x1DC) = one;
        *(int*)((char*)self + 0x1A8) = one;
        *(int*)((char*)self + 0x1C0) = st;
        break;
    }
    case 6:
    {
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (*(int*)((char*)self + 0x1A8) == 1)
            return;
        int s = *(int*)((char*)self + 0x1C0);
        if (s == 6)
            return;
        *(int*)((char*)self + 0x1E4) = 1;
        if (s == 0 || s == 2 || s == 1)
            ((cMoviePlayer_1FF3B0*)mp)->v01(0);
        *(int*)((char*)self + 0x1C0) = 6;
        break;
    }
    case 7:
    {
        int one = 1;
        if (*(int*)((char*)self + 0x1C0) == 6)
            return;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (!(*(cMovieSub434_1FF3B0**)(mp + 0x434))->v54(*(int*)(mp + 0x428)))
            return;
        int v = *(int*)((char*)msg + 0x18);
        *(int*)((char*)self + 0x1C4) = v;
        *(int*)(mp + 0xF8) = v;
        int s = *(int*)((char*)self + 0x1C0);
        if (s != one)
            return;
        if (!(*(cMovieSub434_1FF3B0**)(mp + 0x434))->v67(*(int*)((char*)self + 0x1C4)))
            return;
        *(int*)((char*)self + 0x1A8) = s;
        *(int*)((char*)self + 0x1C0) = 4;
        cMCOverlayManager_SetAction_v(mp, 0);
        ((cMoviePlayer_1FF3B0*)mp)->v01(0x18);
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
 * Zeroes the reward count (+0x9C) and runs cOVState_REWARDS_checkRewardList(self, 0, -1), which probably
 * counts them into it. "Menu0000" loses its wrap flag and becomes scrollable when it has fewer rows than
 * rewards, else gets mDisableDirSound; the "up" / "down" arrows start hidden.
 */
extern "C" void cOVState_REWARDS_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_62reward_list), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* engine2 = *(void**)((char*)self + 0x10);
    void* bg = cUIEngine_addScreenByHashName(engine2, self, GetHashValue32(str_OV_darkblue), 0);
    if (bg != 0) {
        cUIScreen_playFrame(bg, 0, 0);
    }
    cOVState_onCreateScreen(self);
    *(int*)((char*)self + 0x9C) = 0;
    cOVState_REWARDS_checkRewardList(self, 0, -1);
    char* menu = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu0000_b));
    if (menu != 0) {
        *(int*)(menu + 0x14) &= ~0x80;
        if (*(unsigned char*)(menu + 0x96) < *(int*)((char*)self + 0x9C)) {
            cUIMenu_setScrollableMenu(menu, *(int*)((char*)self + 0x9C));
        } else {
            *(int*)(menu + 0x90) |= 8;
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
extern "C" void cOVState_REWARDS_updateMenu(void* self, int a1);

/**
 * Runs the base UI-state focus handling, then refreshes the rewards menu.
 *
 * Calls cUIState_onGainFocus() and then cOVState_REWARDS_updateMenu(self, 0).
 */
extern "C" void cOVState_REWARDS_onGainFocus(void* self)
{
    cUIState_onGainFocus(self);
    cOVState_REWARDS_updateMenu(self, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatepause", cOVState_REWARDS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_clearUserAwards(void* iface);

/**
 * On Cross, clears the player's new-award list and lets the overlay manager move on.
 *
 * Sets the pending overlay action cOVStateManager_mStatus to 1 (cOVStateManager_update then calls cOVStateManager_gotoNextOverlay, which probably
 * triggers the next end-of-event overlay) and calls cBERewardInterface_clearUserAwards() on the reward
 * interface (0xD).
 *
 * @param pWidget  the item; ignored when null
 * @param key  the widget event; only 5 (Cross) acts
 */
extern "C" void cOVState_REWARDS_onWidgetEvent(void* self, void* pWidget, int key)
{
    if (pWidget != 0) {
        if (key == 5) {
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
extern "C" void cOVState_REWARDS_updateMenu(void* self, int a);

/**
 * Handles the rewards menu's notifications: refills the rows on a scroll and picks the button feedback.
 *
 * Event 4 (scroll offset changed) refills the rows from @p index (cOVState_REWARDS_updateMenu). Triangle,
 * Square and Circle (7-9) give 0; scroll up / down (2 / 3) give 0 with fewer than 10 rewards (+0x9C).
 *
 * @param id  the menu (On Tour pMenu); unused
 * @param event  the menu event (2 / 3 scrolled, 4 offset changed, 5 selection changed, 6-9 buttons)
 * @param index  the new scroll offset for events 2-5
 * @return 1 for event 4, 0 as above, else 0x101 (animation and sound)
 */
extern "C" int cOVState_REWARDS_onWidgetMenu(void* self, int id, int event, int index)
{
    switch (event) {
    case 4:
        cOVState_REWARDS_updateMenu(self, index);
        return 1;
    case 7: case 8: case 9:
        return 0;
    case 2: case 3:
        if (*(int*)((char*)self + 0x9C) < 10) {
            return 0;
        }
        /* fall through */
    case 5: case 6:
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

struct sCol_1FFD08 {
    float r, g, b, a;
};

class cUIObj_1FFD08 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int on);
    virtual void v09(int on);
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
    virtual void v25(int on);
};

/**
 * Fills the nine visible reward rows starting at reward @p base and shows the scroll arrows that apply.
 *
 * Rows "0".."8" get the text of reward base + i (cOVState_REWARDS_checkRewardList). With fewer than 10
 * rewards (+0x9C) every row is drawn in a fixed static colour (+0x88, slot 25) and made static (slot 8),
 * without a null check on the row. "up" is shown when @p base is not 0, "down" while rewards remain past base
 * + 9.
 *
 * @param vself  the rewards overlay state (this)
 * @param base  index of the reward in the top row (On Tour offset)
 */
extern "C" void cOVState_REWARDS_updateMenu(void* vself, int base)
{
    char* self = (char*)vself;
    sName_1FFD08 name = *(sName_1FFD08*)D_004A2580;
    unsigned short text[200];
    int i;
    for (i = 0; i < 9; i++) {
        name.c[0] = '0' + i;
        cOVState_REWARDS_checkRewardList_1FFD08(self, text, i + base);
        cUIObj_1FFD08* textObj = (cUIObj_1FFD08*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(name.c));
        if (textObj != 0) {
            cUIText_setUnicodeString(textObj, text);
        }
        if (*(int*)(self + 0x9C) < 10) {
            sCol_1FFD08 color;
            color.r = 1.0f;
            color.g = 0.3960784673690796f;
            color.b = 0.7215686440467834f;
            color.a = 0.7882353663444519f;
            *(sCol_1FFD08*)((char*)textObj + 0x88) = color;
            textObj->v25(1);
            textObj->v08(1);
        }
    }
    if (base == 0) {
        cUIObj_1FFD08* textObj = (cUIObj_1FFD08*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_up));
        if (textObj != 0) {
            textObj->v09(0);
        }
    } else {
        cUIObj_1FFD08* textObj = (cUIObj_1FFD08*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_up));
        if (textObj != 0) {
            textObj->v09(1);
        }
    }
    if (*(int*)(self + 0x9C) <= base + 9) {
        cUIObj_1FFD08* textObj = (cUIObj_1FFD08*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_down));
        if (textObj != 0) {
            textObj->v09(0);
        }
    } else {
        cUIObj_1FFD08* textObj = (cUIObj_1FFD08*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_down));
        if (textObj != 0) {
            textObj->v09(1);
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" char* func_00144BF0(void* iface);
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
extern char cBENewRaceInterface_newRace[];
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
    char* g = (char*)SSXApp->FELocale;
    sVE_1FFF78* vt = *(sVE_1FFF78**)(g + 4);
    char* t = g + vt[4].delta;
    return vt[4].fn(t, GetHashValue32(name));
}

/**
 * Fills in the rewards overlay's event title and congratulation text the first time it is shown.
 *
 * Runs while mDoneTransition (+0x48) is 0, then sets it. "title_rewardlisting" names the event: the peak
 * event title (func_001FD190) for cGameEvent 6-11, "%S - Jam" for 5, else "%S - %s" with the location name
 * and the mode label (cGameMode 0 / 5 race, 1 slopestyle, 2 big air, 3 pipe). "1text" gets
 * kT_OVRCMNCongratUser with the human rider's short name.
 *
 * @return always 0
 */
// Rewards overlay: fills in the event-result and reward texts the first time it is shown.
extern "C" int cOVState_REWARDS_onGainTransition(void* self)
{
    if (*(int*)((char*)self + 0x48) == 0) {
        unsigned short result[104];
        unsigned short fmt[0x10];
        unsigned short reward[104];
        unsigned short name[0x20];
        void* board = cBE_getInterface_Fv(cBE_getBE(), 2);
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
        signed char* newRace = (signed char*)cBENewRaceInterface_newRace;
        int mode = newRace[0x48];
        int kind = newRace[0x4A];
        switch (kind) {
        case 6 ... 11:
            strcpy1(result, (unsigned short*)func_001FD190(kind));
            break;
        case 5:
            func_002C2540(fmt, str_pct_S_Jam);
            sprintf1(result, fmt, func_00144BF0(race));
            break;
        default: {
            unsigned short* label = 0;
            switch (mode) {
            case 0:
            case 5:
                label = lookupString_1FFF78(str_kT_cmnRaceCaps_b);
                break;
            case 1:
                label = lookupString_1FFF78(str_kT_cmnSlopeCaps);
                break;
            case 2:
                label = lookupString_1FFF78(str_kT_cmnBigAirCaps);
                break;
            case 3:
                label = lookupString_1FFF78(str_kT_cmnPipeCaps);
                break;
            }
            func_002C2540(fmt, str_pct_S_pct_s);
            sprintf1(result, fmt, func_00144BF0(race), label);
            break;
        }
        }
        cUIText_setUnicodeString(cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_title_rewardlisting)), result);
        int id = cBENewPlayerInterface_getHumanRider(player, 0);
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_1text));
        sprintf2_void(name, D_004A2590, cBECharacterInterface_getShortName(board, id));
        unsigned short* text = lookupString_1FFF78(str_kT_OVRCMNCongratUser);
        sprintf1(reward, text, name);
        cUIText_setUnicodeString(obj, reward);
    }
    *(int*)((char*)self + 0x48) = 1;
    return 0;
}
#endif

