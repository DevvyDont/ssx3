#include "common.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

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
    virtual void setStatic(int);
    virtual void setVisible(int);
};

// Declarations shared by the functions of this unit.
struct cUIText;

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

void cUIText_setUnicodeStringByID(cUIText* self, int id);

extern "C" int func_00231AB8(void* game);

extern "C" void* func_0028B180();


extern char str_Menu_b[];

extern unsigned char D_004A2468;

extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* func_00416210(void* dst, int c, int n);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern "C" void cSSXAudio_PauseAudio(void* audio);
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_cOVTemplate_PauseMenu[];
extern char str_31paus_freeride[];
extern char* str_H_J[];

struct sPauseMenu_PM {
    char pad0[0xC];
    int f0C;                    // 0xC
    void* engine;               // 0x10
    char pad14[0x88];
    int a9C[8];                 // 0x9C
    int fBC;                    // 0xBC
    void* screen;               // 0xC0
    char padC4[4];
    int fC8;
    float fCC;
    int fD0;
    int fD4;
    int aD8[14];                // 0xD8
};

/**
 * Creates the pause menu ("31paus_freeride"): labels its up-to-8 option rows, decides which options are
 * enabled, pauses audio.
 *
 * Options come from the state's list (+0x9C, +0xBC entries): return, restart event, options, audio, quit,
 * give up,
 * challenge and competition quits, session, transport, messages. In game state 0xB give up is disabled;
 * offline,
 * when func_00231AB8() fails or a controller is unplugged, only return stays enabled. Restores the last
 * selection.
 *
 * @param state  the pause menu state
 */
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* state)
{
    sPauseMenu_PM* self = (sPauseMenu_PM*)state;
    char name[2];
    self->f0C = GetHashValue32(str_cOVTemplate_PauseMenu);
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_31paus_freeride), 0);
    self->screen = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    cOVState_onCreateScreen(self);
    func_00416210(name, 0, 2);
    for (int i = 0; i < 8; i++) {
        name[0] = '0' + i;
        char* textObj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (textObj != 0) {
            if (i < self->fBC) {
                int nameHash = GetHashValue32(str_H_J[self->a9C[i]]);
                char* mgr = *(char**)((char*)self->engine + 0x10);
                sVEntry<void*(void*, int)>* vt = *(sVEntry<void*(void*, int)>**)(mgr + 4);
                if (vt[4].fn(mgr + vt[4].delta, nameHash) != 0) {
                    cUIText_setUnicodeStringByID((cUIText*)textObj, nameHash);
                } else {
                    cUIText_setAsciiString((cUIText*)textObj, str_H_J[self->a9C[i]]);
                }
                *(int*)(textObj + 0x18) = self->a9C[i];
            } else {
                ((cUIThing*)textObj)->setVisible(0);
                ((cUIThing*)textObj)->setStatic(1);
            }
        }
    }
    self->fD4 = 1;
    self->fCC = 0.05f;
    self->fC8 = 0;
    self->fD0 = 0;
    int i;
    for (i = 0; i < 14; i++) {
        self->aD8[i] = 1;
    }
    if (SSXApp->Game->mCurrentStateID == 0xB) {
        self->aD8[6] = 0;
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0) {
        void* game = SSXApp->Game;
        if (func_00231AB8(game) == 0 || SSXApp->Game->ControllerUnplugged != 0) {
            for (i = 0; i < 14; i++) {
                self->aD8[i] = 0;
                if (i == 1) {
                    self->aD8[i] = 1;
                }
            }
        }
    }
    void* menu = cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Menu_b));
    if (menu != 0) {
        cUIMenu_setSelectedByIndex(menu, D_004A2468);
    }
    cSSXAudio_PauseAudio(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_00441C30[];
extern char str_pct_s_group[];

struct sVec3_001F8448 {
    float x, y, z;
};
struct sVEnt_001F8448 { short delta; short index; void (*fn)(void*, int); };

/**
 * On its first transition, shows only the PDA icon groups of the menu's options and stacks them 40 units
 * apart.
 *
 * @return 0
 */
extern "C" int cOVTemplate_PauseMenu_onGainTransition(void* self)
{
    char buf[64];
    if (*(int*)((char*)self + 0xD4) != 0) {
        *(int*)((char*)self + 0xD4) = 0;
        for (int i = 0; i < 14; i++) {
            sprintf(buf, str_pct_s_group, D_00441C30[i]);
            char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0xC0), GetHashValue32(buf));
            if (obj != 0) {
                sVEnt_001F8448* vt = *(sVEnt_001F8448**)(obj + 8);
                vt[9].fn(obj + vt[9].delta, 0);
            }
        }
        for (int i = 0; i < 8; i++) {
            if (i < *(int*)((char*)self + 0xBC)) {
                sprintf(buf, str_pct_s_group, D_00441C30[((int*)((char*)self + 0x9C))[i]]);
                char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0xC0), GetHashValue32(buf));
                if (obj != 0) {
                    sVEnt_001F8448* vt = *(sVEnt_001F8448**)(obj + 8);
                    vt[9].fn(obj + vt[9].delta, 1);
                    sVec3_001F8448 itemPos;
                    itemPos.x = 0.0f;
                    itemPos.z = 0.0f;
                    itemPos.y = (float)i * 40.0f;
                    *(sVec3_001F8448*)(obj + 0x44) = itemPos;
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIState_onGainFocus(void* self, int a1);
extern unsigned char D_004A2468;
extern char D_004A2138[];
extern char D_004C8AE8[];
extern char D_004C8AF8[];
extern char D_004C8B08[];
extern char D_004C8B18[];

struct sVEnt_001F85C0 { short delta; short index; void (*fn)(void*, void*); };

static inline void setColor_001F85C0(char* obj, void* c)
{
    sVEnt_001F85C0* vt = *(sVEnt_001F85C0**)(obj + 8);
    vt[11].fn(obj + vt[11].delta, c);
}

/**
 * Recolours the pause menu rows for focus (selected or not, enabled or disabled), then gives the state focus.
 *
 * @param pPrevState  the state that had focus before, passed on to cUIState_onGainFocus()
 */
extern "C" void cOVTemplate_PauseMenu_onGainFocus(void* self, int pPrevState)
{
    char buf[32];
    char* obj;
    int* sel = (int*)((char*)self + 0x9C);
    char* flags = (char*)self + 0xD8;
    int i = 0;
    while (i < 8 && (sprintf(buf, D_004A2138, i),
                     obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0xC0), GetHashValue32(buf)),
                     i < *(int*)((char*)self + 0xBC))) {
        if (obj != 0) {
            if (i == D_004A2468) {
                if (*(int*)(flags + (*sel << 2)) != 0) {
                    setColor_001F85C0(obj, D_004C8AF8);
                } else {
                    setColor_001F85C0(obj, D_004C8AE8);
                }
            } else {
                if (*(int*)(flags + (*sel << 2)) != 0) {
                    setColor_001F85C0(obj, D_004C8B18);
                } else {
                    setColor_001F85C0(obj, D_004C8B08);
                }
            }
        }
        sel++;
        i++;
    }
    cUIState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when the pause menu loses focus; the body is empty.
 */
void cOVTemplate_PauseMenu_onLoseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
class cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" int func_001E3408(void);
extern "C" void cOVState_onUpdate(void* self);
extern "C" void cGameComm_sendPauseCmd(void* self, int a1);
extern "C" void cOVStateManager_removePDATemplate(void);
// func_0039F190 returns its last list node in $v0; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");
extern int D_004A2A50;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];
extern char str_NewMail[];
extern char str_trans_out[];

struct sColor_1F8728 { float r, g, b, a; };
struct sVEnt_1F8728 { short delta; short index; void (*fn)(void*, void*); };

/**
 * Pulses the "NewMail" icon while messages are waiting and closes the pause menu when an online pause times
 * out.
 *
 * The pulse runs in states 1, 2 and 4 when func_001E3408() reports mail. When the online pause or overlay
 * timeout
 * reaches 0 it sends an unpause, plays "trans_out", removes the PDA and pops the pause state.
 */
extern "C" void cOVTemplate_PauseMenu_onUpdate(void* self)
{
    int st = *(int*)((char*)self + 0x110);
    if ((st == 1 || st == 2 || st == 4) && func_001E3408() > 0)
    {
        float phase = *(float*)((char*)self + 0xC8) + *(float*)((char*)self + 0xCC);
        *(float*)((char*)self + 0xC8) = phase;
        if (phase >= 1.0f)
        {
            *(float*)((char*)self + 0xC8) = 1.0f;
            *(float*)((char*)self + 0xCC) = -*(float*)((char*)self + 0xCC);
        }
        else if (phase < 0.0f)
        {
            *(float*)((char*)self + 0xC8) = 0.0f;
            *(float*)((char*)self + 0xCC) = -*(float*)((char*)self + 0xCC);
        }
        char* uiObj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0xC0), GetHashValue32(str_NewMail));
        if (uiObj != 0)
        {
            sColor_1F8728 color = *(sColor_1F8728*)(uiObj + 0x1C);
            color.r = *(float*)((char*)self + 0xC8);
            sVEnt_1F8728* vt = *(sVEnt_1F8728**)(uiObj + 8);
            vt[11].fn(uiObj + vt[11].delta, &color);
        }
    }
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm == 0)
        return;
    int go = 0;
    if (gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0)
        go = 1;
    if (!go)
        return;
    cGameComm_sendPauseCmd((char*)cGameComm_mGameComm, 0);
    cGameComm* comm = cGameComm_mGameComm;
    comm->mPauseTimeout = -1;
    comm->mOverlayTimeout = -1;
    int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0xC0), GetHashValue32(str_trans_out));
    cUIScreen_playFrame(*(void**)((char*)self + 0xC0), frame, 1);
    *(int*)((char*)self + 0xD0) = 1;
    cOVStateManager_removePDATemplate();
    func_0039F190_r(*(char**)((char*)self + 0x10) + 0x18, 1);
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
}
#endif

extern "C" void* cOVState_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the pause menu's screens: forgets its playing screen animation (+0xC4), then runs
 * cOVState_onDestroyScreen().
 *
 * The pointer at +0xC4 holds the result of cUIScreen_playFrame() while a frame animation plays.
 *
 * @return cOVState_onDestroyScreen()'s return value (On Tour's returns nothing)
 */
void* cOVTemplate_PauseMenu_onDestroyScreen(void* self)
{
    *(int*)((char*)self + 0xc4) = 0;
    return cOVState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu events 8 and 9 and refuses event 6 in the pause menu.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param event  menu event number (On Tour Event)
 * @return 0x100 for events 8 and 9, 0 for event 6, otherwise 0x101
 */
extern "C" int cOVTemplate_PauseMenu_onWidgetMenu(void* self, int menu, unsigned int event)
{
    switch (event) {
    case 8:
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIText;
class cUIScreen;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void cGameComm_sendPauseCmd(void* self, int a1);
extern "C" void cOVStateManager_removePDATemplate(void);
// func_0039F190 returns its last list node in $v0; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");
extern "C" void cOVStateManager_triggerOverlay(int cmd);
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* mem, void* owner, int a2, int a3);
extern "C" void* func_0028B180();
extern "C" void func_00289BB8(void* audio);
extern "C" void cOVTemplate_PauseMenu_setPDAIconColor(void* self, int idx, int on);
extern int D_004A2A50;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];
extern unsigned char D_004A2468;
extern int D_004A2594;
extern int D_004A2708;
extern char str_Menu_b[];
extern char str_help0[];
extern char str_trans_out[];
extern char str_cFEStateAudioOptions_b[];
extern char* str_H_J_b[];
extern char D_004C8AE8[];
extern char D_004C8AF8[];
extern char D_004C8B08[];
extern char D_004C8B18[];

struct sVE_PMWE {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sSnd_PMWE {
    sVE_PMWE* vt;
};
extern sSnd_PMWE D_004A5A58;

struct sWidget_PMWE {
    char pad_0x0[0x8];
    sVE_PMWE* vt;                   // 0x8
    char pad_0xC[0xC];
    int idx;                        // 0x18
};

struct sPauseMenu_PMWE {
    char pad_0x0[0x10];
    char* owner;                    // 0x10
    char pad_0x14[0xAC];
    void* screen;                   // 0xC0
    char pad_0xC4[0xC];
    int busy;                       // 0xD0
    char pad_0xD4[0x4];
    int enabled[1];                 // 0xD8
};

static inline void stopReplay_PMWE()
{
    if (cGameComm_mGameComm != 0) {
        cGameComm_sendPauseCmd((char*)cGameComm_mGameComm, 0);
        cGameComm* gameComm = cGameComm_mGameComm;
        gameComm->mPauseTimeout = -1;
        gameComm->mOverlayTimeout = -1;
    }
}

static inline void setColor_PMWE(sWidget_PMWE* w, void* c)
{
    w->vt[11].fn((char*)w + w->vt[11].delta, c);
}

/**
 * Handles the pause menu: runs the chosen option, resumes on back, and updates colours, icons and help on
 * highlight.
 *
 * Selecting a disabled option only plays sound 4. Return (1) and back resume the game, popping the pause
 * state;
 * restart, options, quit, give up, session, transport (map style 2) and messages open their overlays, audio
 * opens
 * cFEStateAudioOptions.
 *
 * @param w  the option row (its option at +0x18), or 0
 * @param event  widget event number: 5 select, 6 back, 1 highlight, 2 unhighlight
 */
extern "C" void cOVTemplate_PauseMenu_onWidgetEvent(sPauseMenu_PMWE* self, sWidget_PMWE* w, int event)
{
    if (w == 0)
        return;
    switch (event) {
    case 5: {
        sSnd_PMWE* snd = &D_004A5A58;
        if (*(sSnd_PMWE**)(self->owner + 0x14) != 0)
            snd = *(sSnd_PMWE**)(self->owner + 0x14);
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Menu_b));
        if (self->enabled[w->idx] == 0) {
            snd->vt[2].fn((char*)snd + snd->vt[2].delta, 4);
            return;
        }
        snd->vt[2].fn((char*)snd + snd->vt[2].delta, 6);
        D_004A2468 = *((unsigned char*)text + 0x95);
        int frame = cUIScreen_getFrameByLabel((cUIScreen*)self->screen, GetHashValue32(str_trans_out));
        cUIScreen_playFrame(self->screen, frame, 1);
        self->busy = 1;
        switch (w->idx) {
        case 1:
            stopReplay_PMWE();
            cOVStateManager_removePDATemplate();
            func_0039F190_r(self->owner + 0x18, 1);
            D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
            func_00289BB8(func_0028B180());
            break;
        case 2:
            D_004A2708 = 1;
            cOVStateManager_triggerOverlay(0x17);
            break;
        case 3:
            cOVStateManager_triggerOverlay(7);
            break;
        case 4: {
            char* owner = (char*)SSXApp->Game->UI;
            char* popup = (char*)cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, str_cFEStateAudioOptions_b, 0, 0), owner, 0, 0);
            *(int*)(popup + 0x1C) = (*(int*)(popup + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(owner + 0x18, popup, 0, 0);
            break;
        }
        case 5:
        case 13:
            cOVStateManager_triggerOverlay(0x18);
            break;
        case 6:
            cOVStateManager_triggerOverlay(0x1C);
            break;
        case 7:
            cOVStateManager_triggerOverlay(0x19);
            break;
        case 8:
            cOVStateManager_triggerOverlay(0x1A);
            break;
        case 9:
            cOVStateManager_triggerOverlay(0x1B);
            break;
        case 10:
            cOVStateManager_triggerOverlay(0x20);
            break;
        case 11:
            D_004A2594 = 2;
            cOVStateManager_triggerOverlay(0x21);
            break;
        case 12:
            cOVStateManager_triggerOverlay(0x22);
            break;
        }
        break;
    }
    case 6:
        stopReplay_PMWE();
        cOVStateManager_removePDATemplate();
        D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
        func_0039F190_r(self->owner + 0x18, 1);
        func_00289BB8(func_0028B180());
        break;
    case 1: {
        if (self->busy == 0) {
            if (self->enabled[w->idx] != 0)
                setColor_PMWE(w, D_004C8AF8);
            else
                setColor_PMWE(w, D_004C8AE8);
            if (self->busy == 0)
                cOVTemplate_PauseMenu_setPDAIconColor(self, w->idx, 1);
        }
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_help0));
        if (text != 0)
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_H_J_b[w->idx]));
        break;
    }
    case 2:
        if (self->busy != 0)
            return;
        if (self->enabled[w->idx] != 0)
            setColor_PMWE(w, D_004C8B18);
        else
            setColor_PMWE(w, D_004C8B08);
        if (self->busy == 0)
            cOVTemplate_PauseMenu_setPDAIconColor(self, w->idx, 0);
        break;
    case 3:
    case 4:
    case 7:
    case 8:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Does no input handling for the pause menu and reports the input as not handled.
 *
 * @return always 0
 */
int cOVTemplate_PauseMenu_onInputBegin(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_setPDAIconColor);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_00441C30[];
extern char str_pct_s_S[];
extern char str_pct_s_R[];

/**
 * Plays an option's PDA icon animation: "<icon>_S" when it gains focus, "<icon>_R" (release) when it loses
 * it.
 *
 * @param idx  the option (On Tour type), indexing the icon names
 * @param on  nonzero for focus (On Tour focus)
 */
extern "C" void cOVTemplate_PauseMenu_setPDAIconColor(void* self, int idx, int on)
{
    char buf[64];
    if (on) {
        sprintf(buf, str_pct_s_S, D_00441C30[idx]);
        unsigned short frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0xC0), GetHashValue32(buf));
        if (frame != 0xFFFF && *(void**)((char*)self + 0xC4) == 0) {
            *(void**)((char*)self + 0xC4) = cUIScreen_playFrame(*(void**)((char*)self + 0xC0), frame, 1);
        }
    } else {
        sprintf(buf, str_pct_s_R, D_00441C30[idx]);
        unsigned short frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0xC0), GetHashValue32(buf));
        if (frame != 0xFFFF) {
            *(unsigned short*)((char*)*(void**)((char*)self + 0xC4) + 0xC) = frame;
            *(unsigned short*)((char*)*(void**)((char*)self + 0xC4) + 0xE) = 0xFFFF;
            *(void**)((char*)self + 0xC4) = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8EE0);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478160[];

extern "C" void func_001F8EE0(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 6;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478160[i];
    }
    *(int*)((char*)self + 0x110) = 1;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8F48);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478178[];

extern "C" void func_001F8F48(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 6;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478178[i];
    }
    *(int*)((char*)self + 0x110) = 2;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8FB0);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478140[];

extern "C" void func_001F8FB0(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 7;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478140[i];
    }
    *(int*)((char*)self + 0x110) = 3;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F9018);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478190[];

extern "C" void func_001F9018(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 5;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478190[i];
    }
    *(int*)((char*)self + 0x110) = 4;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F9080);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_004781A8[];

extern "C" void func_001F9080(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 5;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_004781A8[i];
    }
    *(int*)((char*)self + 0x110) = 5;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F90E8);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern "C" int func_00231AB8(void* game);
extern int D_004781C0[];
extern int D_004A2868[2];

extern "C" void func_001F90E8(void* self)
{
    int* items = D_004781C0;
    *(int*)((char*)self + 0xBC) = 3;
    if (func_00231AB8(SSXApp->Game) == 0) {
        items = D_004A2868;
        *(int*)((char*)self + 0xBC) = 2;
    }
    int i;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = items[i];
    }
    *(int*)((char*)self + 0x110) = 6;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

