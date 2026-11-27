#include "common.h"
#include "ui/uithing.h"
#include "be/beinterface.h"
#include "ui/uiengine.h"
#include "sound/ssxaudio.h"
#include "fe/ovstatepause.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "util/locale.h"
#include "ui/uiscreen.h"
#include "ui/uimenu.h"

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// Declarations shared by the functions of this unit.
struct cUIText;

int GetHashValue32(char* str);

extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

void cUIText_setUnicodeStringByID(cUIText* self, int id);

extern "C" int cGame_canUserActivatePauseMenu(void* game);

extern "C" cSSXAudio* cAudioInterface_bxAudio();


extern char str_Menu_b[];

extern unsigned char cOVTemplate_PauseMenu_mSelectedMenuIndex;

extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* memset(void* dst, int c, int n);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern "C" void cSSXAudio_PauseAudio(void* audio);
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_cOVTemplate_PauseMenu[];
extern char str_31paus_freeride[];
extern char* str_H_J[];


/**
 * Creates the pause menu ("31paus_freeride"): labels its up-to-8 option rows, decides which options are
 * enabled, pauses audio.
 *
 * Options come from the state's list (mPauseMenuOptions, mPauseMenuNumOptions entries): return, restart event, options,
 * audio, quit, give up, challenge and competition quits, session, transport, messages. In game state 0xB give up is
 * disabled; offline, when cGame_canUserActivatePauseMenu() fails or a controller is unplugged, only return stays
 * enabled. Restores the last selection.
 *
 * @param state  the pause menu state
 */
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* state)
{
    cOVTemplate_PauseMenu* self = state;
    char name[2];
    self->mHashName = GetHashValue32(str_cOVTemplate_PauseMenu);
    void* engine = ((void*)self->mpEngine);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_31paus_freeride), 0);
    self->mpPauseScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    cOVState_onCreateScreen(self);
    memset(name, 0, 2);
    for (int i = 0; i < 8; i++) {
        name[0] = '0' + i;
        cUIThing* textObj = cUIScreen_getObjectByHashName(((void*)self->mpPauseScreen), GetHashValue32(name));
        if (textObj != 0) {
            if (i < self->mPauseMenuNumOptions) {
                int nameHash = GetHashValue32(str_H_J[self->mPauseMenuOptions[i]]);
                cUILocaleInterface* mgr = self->mpEngine->mpLocaleInterface;
                if (mgr->getStringByHash(nameHash) != 0) {
                    cUIText_setUnicodeStringByID((cUIText*)textObj, nameHash);
                } else {
                    cUIText_setAsciiString((cUIText*)textObj, str_H_J[self->mPauseMenuOptions[i]]);
                }
                textObj->mMiscData = self->mPauseMenuOptions[i];
            } else {
                textObj->setVisible(0);
                textObj->setStatic(1);
            }
        }
    }
    self->mMoveIcons = 1;
    self->mNewMailAlphaDiff = 0.05f;
    self->mNewMailAlpha = 0.0f;
    self->mScreenExiting = 0;
    int i;
    for (i = 0; i < 14; i++) {
        self->mPauseOptionUnlocked[i] = 1;
    }
    if (SSXApp->Game->mCurrentStateID == kGFSM_SectionLoad) {
        self->mPauseOptionUnlocked[6] = 0;
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0) {
        void* game = SSXApp->Game;
        if (cGame_canUserActivatePauseMenu(game) == 0 || SSXApp->Game->ControllerUnplugged != 0) {
            for (i = 0; i < 14; i++) {
                self->mPauseOptionUnlocked[i] = 0;
                if (i == 1) {
                    self->mPauseOptionUnlocked[i] = 1;
                }
            }
        }
    }
    void* menu = cUIScreen_getObjectByHashName(((void*)self->mpPauseScreen), GetHashValue32(str_Menu_b));
    if (menu != 0) {
        cUIMenu_setSelectedByIndex(menu, cOVTemplate_PauseMenu_mSelectedMenuIndex);
    }
    cSSXAudio_PauseAudio(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* gMenuItemIcons[];
extern char str_pct_s_group[];

/**
 * On its first transition, shows only the PDA icon groups of the menu's options and stacks them 40 units
 * apart.
 *
 * @return 0
 */
bool cOVTemplate_PauseMenu::onGainTransition(int)
{
    char buf[64];
    if (mMoveIcons != 0) {
        mMoveIcons = 0;
        for (int i = 0; i < 14; i++) {
            sprintf(buf, str_pct_s_group, gMenuItemIcons[i]);
            cUIThing* obj = cUIScreen_getObjectByHashName(mpPauseScreen, GetHashValue32(buf));
            if (obj != 0) {
                obj->setVisible(0);
            }
        }
        for (int i = 0; i < 8; i++) {
            if (i < mPauseMenuNumOptions) {
                sprintf(buf, str_pct_s_group, gMenuItemIcons[mPauseMenuOptions[i]]);
                cUIThing* obj = cUIScreen_getObjectByHashName(mpPauseScreen, GetHashValue32(buf));
                if (obj != 0) {
                    obj->setVisible(1);
                    t3Vector itemPos;
                    itemPos.x = 0.0f;
                    itemPos.z = 0.0f;
                    itemPos.y = (float)i * 40.0f;
                    obj->mPosition = itemPos;
                }
            }
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cUIState_onGainFocus(void* self, int a1);
extern unsigned char cOVTemplate_PauseMenu_mSelectedMenuIndex;
extern char D_004A2138[];
extern char D_004C8AE8[];
extern char D_004C8AF8[];
extern char D_004C8B08[];
extern char D_004C8B18[];

struct sVEnt_001F85C0 { short delta; short index; void (*fn)(void*, void*); };

static inline void setColor_001F85C0(cUIThing* obj, void* c)
{
    obj->setColour((const tARGB*)c);
}

/**
 * Recolours the pause menu rows for focus (selected or not, enabled or disabled), then gives the state focus.
 *
 * @param pPrevState  the state that had focus before, passed on to cUIState_onGainFocus()
 */
extern "C" void cOVTemplate_PauseMenu_onGainFocus(cOVTemplate_PauseMenu* self, int pPrevState)
{
    char buf[32];
    cUIThing* obj;
    int* sel = (int*)&self->mPauseMenuOptions;
    char* flags = (char*)&self->mPauseOptionUnlocked;
    int i = 0;
    while (i < 8 && (sprintf(buf, D_004A2138, i),
                     obj = cUIScreen_getObjectByHashName(self->mpPauseScreen, GetHashValue32(buf)),
                     i < self->mPauseMenuNumOptions)) {
        if (obj != 0) {
            if (i == cOVTemplate_PauseMenu_mSelectedMenuIndex) {
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
void cOVTemplate_PauseMenu::onLoseFocus(cUIState*)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
class cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" int func_001E3408(void);
extern "C" void cOVState_onUpdate(void* self);
extern "C" void cGameComm_sendPauseCmd(void* self, int a1);
extern "C" void cOVStateManager_removePDATemplate(void);
// cUIStateStack_pop returns its last list node in $v0; the unit declares it void.
void* cUIStateStack_pop_r(void* self, int a1) __asm__("cUIStateStack_pop");
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern char str_NewMail[];
extern char str_trans_out[];

/**
 * Pulses the "NewMail" icon while messages are waiting and closes the pause menu when an online pause times
 * out.
 *
 * The pulse runs in kOVSTATE_PAUSE_COMPETE, _BIGCHAL and _SPOKE when func_001E3408() reports mail. When the online
 * pause or overlay timeout reaches 0 it sends an unpause, plays "trans_out", removes the PDA and pops the pause state.
 */
extern "C" void cOVTemplate_PauseMenu_onUpdate(cOVTemplate_PauseMenu* self)
{
    int st = self->mPauseMenuType;
    if ((st == kOVSTATE_PAUSE_COMPETE || st == kOVSTATE_PAUSE_BIGCHAL || st == kOVSTATE_PAUSE_SPOKE)
        && func_001E3408() > 0)
    {
        float phase = self->mNewMailAlpha + self->mNewMailAlphaDiff;
        self->mNewMailAlpha = phase;
        if (phase >= 1.0f)
        {
            self->mNewMailAlpha = 1.0f;
            self->mNewMailAlphaDiff = -self->mNewMailAlphaDiff;
        }
        else if (phase < 0.0f)
        {
            self->mNewMailAlpha = 0.0f;
            self->mNewMailAlphaDiff = -self->mNewMailAlphaDiff;
        }
        cUIThing* uiObj = cUIScreen_getObjectByHashName(self->mpPauseScreen, GetHashValue32(str_NewMail));
        if (uiObj != 0)
        {
            tARGB color = uiObj->mColour;
            color.A = self->mNewMailAlpha;
            uiObj->setColour(&color);
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
    int frame = cUIScreen_getFrameByLabel(self->mpPauseScreen, GetHashValue32(str_trans_out));
    cUIScreen_playFrame(self->mpPauseScreen, frame, 1);
    self->mScreenExiting = 1;
    cOVStateManager_removePDATemplate();
    cUIStateStack_pop_r(&self->mpEngine->mStateStack, 1);
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
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
void* cOVTemplate_PauseMenu_onDestroyScreen(void* vself)
{
    cOVTemplate_PauseMenu* self = (cOVTemplate_PauseMenu*)vself;
    self->mpThread = 0;
    return cOVState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu kONOPTION and kONMISC and refuses kONNEXT in the pause menu.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param event  menu event number (On Tour Event)
 * @return 0x100 for kONOPTION and kONMISC, 0 for kONNEXT, otherwise 0x101
 */
int cOVTemplate_PauseMenu::onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
{
    switch (event) {
    case cUIMenu::kONOPTION:
    case cUIMenu::kONMISC:
        return 0x100;
    case cUIMenu::kONNEXT:
        return 0;
    }
    return 257;
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
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void cGameComm_sendPauseCmd(void* self, int a1);
extern "C" void cOVStateManager_removePDATemplate(void);
// cUIStateStack_pop returns its last list node in $v0; the unit declares it void.
void* cUIStateStack_pop_r(void* self, int a1) __asm__("cUIStateStack_pop");
extern "C" void cOVStateManager_triggerOverlay(int cmd);
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* mem, void* owner, int a2, int a3);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_ResumeAudio(void* audio);
extern "C" void cOVTemplate_PauseMenu_setPDAIconColor(cOVTemplate_PauseMenu* self, int idx, int on);
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern unsigned char cOVTemplate_PauseMenu_mSelectedMenuIndex;
extern int cUITemplate_MAP_mMapStyle;
extern int cOVStateManager_mIsPauseDialog;
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
extern sSnd_PMWE gUISoundNull;

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
    w->vt[11].fn(vthis(w, w->vt[11]), c);
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
 * @param pWidget  the option row (its option at +0x18), or 0
 * @param event  widget event number: kONNEXT select, kONPREV back, kONGAINFOCUS highlight, kONLOSEFOCUS unhighlight
 */
void cOVTemplate_PauseMenu::onWidgetEvent(cUIThing* pWidget, unsigned int event)
{
    sWidget_PMWE* w = (sWidget_PMWE*)pWidget;
    if (w == 0)
        return;
    switch (event) {
    case cUIState::kONNEXT: {
        sSnd_PMWE* snd = &gUISoundNull;
        if ((sSnd_PMWE*)mpEngine->mpSound != 0)
            snd = (sSnd_PMWE*)mpEngine->mpSound;
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpPauseScreen, GetHashValue32(str_Menu_b));
        if (mPauseOptionUnlocked[w->idx] == 0) {
            snd->vt[2].fn(vthis(snd, snd->vt[2]), 4);
            return;
        }
        snd->vt[2].fn(vthis(snd, snd->vt[2]), 6);
        cOVTemplate_PauseMenu_mSelectedMenuIndex = *((unsigned char*)text + 0x95);
        int frame = cUIScreen_getFrameByLabel((cUIScreen*)mpPauseScreen, GetHashValue32(str_trans_out));
        cUIScreen_playFrame(mpPauseScreen, frame, 1);
        mScreenExiting = 1;
        switch (w->idx) {
        case 1:
            stopReplay_PMWE();
            cOVStateManager_removePDATemplate();
            cUIStateStack_pop_r(&mpEngine->mStateStack, 1);
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            cSSXAudio_ResumeAudio(cAudioInterface_bxAudio());
            break;
        case 2:
            cOVStateManager_mIsPauseDialog = 1;
            cOVStateManager_triggerOverlay(23);
            break;
        case 3:
            cOVStateManager_triggerOverlay(7);
            break;
        case 4: {
            cUIEngine* owner = SSXApp->Game->UI;
            char* popup = (char*)cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, str_cFEStateAudioOptions_b, 0, 0), owner, 0, 0);
            *(int*)(popup + 0x1C) = (*(int*)(popup + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(&owner->mStateStack, popup, 0, 0);
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
            cOVStateManager_triggerOverlay(25);
            break;
        case 8:
            cOVStateManager_triggerOverlay(26);
            break;
        case 9:
            cOVStateManager_triggerOverlay(27);
            break;
        case 10:
            cOVStateManager_triggerOverlay(0x20);
            break;
        case 11:
            cUITemplate_MAP_mMapStyle = 2;
            cOVStateManager_triggerOverlay(33);
            break;
        case 12:
            cOVStateManager_triggerOverlay(34);
            break;
        }
        break;
    }
    case cUIState::kONPREV:
        stopReplay_PMWE();
        cOVStateManager_removePDATemplate();
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
        cUIStateStack_pop_r(&mpEngine->mStateStack, 1);
        cSSXAudio_ResumeAudio(cAudioInterface_bxAudio());
        break;
    case cUIState::kONGAINFOCUS: {
        if (mScreenExiting == 0) {
            if (mPauseOptionUnlocked[w->idx] != 0)
                setColor_PMWE(w, D_004C8AF8);
            else
                setColor_PMWE(w, D_004C8AE8);
            if (mScreenExiting == 0)
                setPDAIconColor(w->idx, 1);
        }
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpPauseScreen, GetHashValue32(str_help0));
        if (text != 0)
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_H_J_b[w->idx]));
        break;
    }
    case cUIState::kONLOSEFOCUS:
        if (mScreenExiting != 0)
            return;
        if (mPauseOptionUnlocked[w->idx] != 0)
            setColor_PMWE(w, D_004C8B18);
        else
            setColor_PMWE(w, D_004C8B08);
        if (mScreenExiting == 0)
            setPDAIconColor(w->idx, 0);
        break;
    case cUIState::kONSTART:
    case cUIState::kONSELECT:
    case cUIState::kONOPTION:
    case cUIState::kONMISC:
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
bool cOVTemplate_PauseMenu::onInputBegin(cUIInput*)
{
    return false;
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
extern char* gMenuItemIcons[];
extern char str_pct_s_S[];
extern char str_pct_s_R[];

/**
 * Plays an option's PDA icon animation: "<icon>_S" when it gains focus, "<icon>_R" (release) when it loses
 * it.
 *
 * @param idx  the option (On Tour type), indexing the icon names
 * @param on  nonzero for focus (On Tour focus)
 */
void cOVTemplate_PauseMenu::setPDAIconColor(int idx, int on)
{
    char buf[64];
    if (on) {
        sprintf(buf, str_pct_s_S, gMenuItemIcons[idx]);
        unsigned short frame = cUIScreen_getFrameByLabel(mpPauseScreen, GetHashValue32(buf));
        if (frame != cUIScreen::kNoFrame && mpThread == 0) {
            mpThread = (cUIThread*)cUIScreen_playFrame(mpPauseScreen, frame, 1);
        }
    } else {
        sprintf(buf, str_pct_s_R, gMenuItemIcons[idx]);
        unsigned short frame = cUIScreen_getFrameByLabel(mpPauseScreen, GetHashValue32(buf));
        if (frame != cUIScreen::kNoFrame) {
            mpThread->mCurrentFrameNumber = frame;
            mpThread->mLastFrameNumber = 0xFFFF;
            mpThread = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_COMPETE_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern int gPauseCompeteMenu[];

/**
 * Builds pause overlay 1's screen: sets its 6 menu options from gPauseCompeteMenu, then runs the shared pause-menu screen
 * setup.
 *
 * Sets +0xBC (On Tour mPauseMenuNumOptions) to 6, copies that many option ids from gPauseCompeteMenu into the array at +0x9C
 * (On Tour mPauseMenuOptions), sets the menu type +0x110 (On Tour mPauseMenuType) to 1 and calls
 * cOVTemplate_PauseMenu_onCreateScreen. It is the onCreateScreen slot (+0x24) of cOVState_PAUSE_COMPETE_vtbl, the vtable
 * cOVStateManager_triggerOverlay gives overlay 1 (the career (game path 0) event pause that cOVStateManager_triggerPauseMenu opens).
 */
extern "C" void cOVState_PAUSE_COMPETE_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int i;
    self->mPauseMenuNumOptions = 6;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = gPauseCompeteMenu[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_COMPETE;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_BIGCHAL_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern int gPauseBigChallengeMenu[];

/**
 * Builds pause overlay 2's screen: sets its 6 menu options from gPauseBigChallengeMenu, then runs the shared pause-menu screen
 * setup.
 *
 * Sets +0xBC (On Tour mPauseMenuNumOptions) to 6, copies that many option ids from gPauseBigChallengeMenu into the array at +0x9C
 * (On Tour mPauseMenuOptions), sets the menu type +0x110 (On Tour mPauseMenuType) to 2 and calls
 * cOVTemplate_PauseMenu_onCreateScreen. It is the onCreateScreen slot (+0x24) of cOVState_PAUSE_BIGCHAL_vtbl, the vtable
 * cOVStateManager_triggerOverlay gives overlay 2 (the local-mission pause that cOVStateManager_triggerPauseMenu opens).
 */
extern "C" void cOVState_PAUSE_BIGCHAL_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int i;
    self->mPauseMenuNumOptions = 6;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = gPauseBigChallengeMenu[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_BIGCHAL;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_FREERIDE_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern int gPauseFreerideMenu[];

/**
 * Builds pause overlay 3's screen: sets its 7 menu options from gPauseFreerideMenu, then runs the shared pause-menu screen
 * setup.
 *
 * Sets +0xBC (On Tour mPauseMenuNumOptions) to 7, copies that many option ids from gPauseFreerideMenu into the array at +0x9C
 * (On Tour mPauseMenuOptions), sets the menu type +0x110 (On Tour mPauseMenuType) to 3 and calls
 * cOVTemplate_PauseMenu_onCreateScreen. It is the onCreateScreen slot (+0x24) of cOVState_PAUSE_FREERIDE_vtbl, the vtable
 * cOVStateManager_triggerOverlay gives overlay 3 (the freeride (cGameMode 4) pause that cOVStateManager_triggerPauseMenu opens).
 */
extern "C" void cOVState_PAUSE_FREERIDE_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int i;
    self->mPauseMenuNumOptions = 7;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = gPauseFreerideMenu[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_FREERIDE;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_SPOKE_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern int gPauseSpokeMenu[];

/**
 * Builds pause overlay 4's screen: sets its 5 menu options from gPauseSpokeMenu, then runs the shared pause-menu screen
 * setup.
 *
 * Sets +0xBC (On Tour mPauseMenuNumOptions) to 5, copies that many option ids from gPauseSpokeMenu into the array at +0x9C
 * (On Tour mPauseMenuOptions), sets the menu type +0x110 (On Tour mPauseMenuType) to 4 and calls
 * cOVTemplate_PauseMenu_onCreateScreen. It is the onCreateScreen slot (+0x24) of cOVState_PAUSE_SPOKE_vtbl, the vtable
 * cOVStateManager_triggerOverlay gives overlay 4 (the game-state-11 pause that cOVStateManager_triggerPauseMenu opens).
 */
extern "C" void cOVState_PAUSE_SPOKE_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int i;
    self->mPauseMenuNumOptions = 5;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = gPauseSpokeMenu[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_SPOKE;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_QUICKPLAY_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern int gQuickPlayMenu[];

/**
 * Builds pause overlay 5's screen: sets its 5 menu options from gQuickPlayMenu, then runs the shared pause-menu screen
 * setup.
 *
 * Sets +0xBC (On Tour mPauseMenuNumOptions) to 5, copies that many option ids from gQuickPlayMenu into the array at +0x9C
 * (On Tour mPauseMenuOptions), sets the menu type +0x110 (On Tour mPauseMenuType) to 5 and calls
 * cOVTemplate_PauseMenu_onCreateScreen. It is the onCreateScreen slot (+0x24) of cOVState_PAUSE_QUICKPLAY_vtbl, the vtable
 * cOVStateManager_triggerOverlay gives overlay 5 (the fallback pause that cOVStateManager_triggerPauseMenu opens).
 */
extern "C" void cOVState_PAUSE_QUICKPLAY_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int i;
    self->mPauseMenuNumOptions = 5;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = gQuickPlayMenu[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_QUICKPLAY;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", cOVState_PAUSE_ONLINE_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(cOVTemplate_PauseMenu* self);
extern "C" int cGame_canUserActivatePauseMenu(void* game);
extern int gOnlinePauseMenu[];
extern int gSystemInitiatedPauseMenu[2];

/**
 * Online pause overlay: fills in its menu items before the pause menu screen is built.
 *
 * Uses the three items of gOnlinePauseMenu (On Tour gOnlinePauseMenu), or the two of gSystemInitiatedPauseMenu (gSystemInitiatedPauseMenu)
 * when cGame_canUserActivatePauseMenu (On Tour cGame::canUserActivatePauseMenu) returns 0; copies them to +0x9C with the count at
 * +0xBC, sets +0x110 to 6 and runs cOVTemplate_PauseMenu_onCreateScreen.
 */
extern "C" void cOVState_PAUSE_ONLINE_onCreateScreen(cOVTemplate_PauseMenu* self)
{
    int* items = gOnlinePauseMenu;
    self->mPauseMenuNumOptions = 3;
    if (cGame_canUserActivatePauseMenu(SSXApp->Game) == 0) {
        items = gSystemInitiatedPauseMenu;
        self->mPauseMenuNumOptions = 2;
    }
    int i;
    for (i = 0; i < self->mPauseMenuNumOptions; i++) {
        self->mPauseMenuOptions[i] = items[i];
    }
    self->mPauseMenuType = kOVSTATE_PAUSE_ONLINE;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

