#include "common.h"
#include "replay/replay.h"
#include "ui/uiengine.h"
#include "ui/uimenu.h"
#include "ui/uithing.h"
#include "fe/ovstatereplay.h"
#include "fe/ovstate.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ui/uiscreen.h"

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_option_savereplay[];
extern char str_option_exitreplay[];
extern char str_option_continue[];

/**
 * Tags the replay menu's items with their choice ids and hides Save Replay for a loaded replay.
 *
 * option_savereplay 0 (for a cReplay with Loaded set: setEnabled(0) at slot 6, setStatic(1) at slot 8, hidden),
 * option_exitreplay 1, option_continue 2.
 *
 * @param w  the widget being created
 */
void cOVState_REPLAY::onWidgetCreate(cUIThing* w)
{
    int id = *(int*)((char*)w + 0x38);
    if (id == GetHashValue32(str_option_savereplay)) {
        *(int*)((char*)w + 0x18) = 0;
        if (SSXApp->Game->Replay->Loaded != 0) {
            w->setEnabled(0);
            w->setStatic(1);
            w->setVisible(0);
        }
    } else {
        int id2 = *(int*)((char*)w + 0x38);
        if (id2 == GetHashValue32(str_option_exitreplay)) {
            *(int*)((char*)w + 0x18) = 1;
        } else {
            int id3 = *(int*)((char*)w + 0x38);
            if (id3 == GetHashValue32(str_option_continue)) {
                *(int*)((char*)w + 0x18) = 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onWidgetMenu);
#ifdef SKIP_ASM
#include "fakematch.h"

/**
 * Gives the replay menu's notifications 2-5 the 0x101 feedback while the menu is open; buttons get none.
 *
 * Cross and Triangle are handled in cOVState_REPLAY_onInputBegin instead.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event
 * @return 0x101 for events 2-5 while the menu is open (+0x9C), else 0
 */
// FAKEMATCH: register pin `register int r FM_REG("$2")` keeps the result in v0 (local-alloc gives v0 to the block-local temps first, so the plain r lands in a0 and is copied to v0 at the end); plain form 88.50%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int cOVState_REPLAY_onWidgetMenu(cOVState_REPLAY* self, int pMenu, unsigned int key)
{
    register int result FM_REG("$2") = 0;
    if (self->mShowPopupMenu != 0) {
        if (key - 2 <= 3) {
            result = 257;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onInputBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
class cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
int cInput_getState(void* self, int button);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern "C" void cReplay_stopReplay(void* replay);
extern "C" void cReplay_startAutoReplay(void* replay);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern "C" void cGame_exit(void* game, int a1);
extern "C" bool cReplay_updateInput(void* replay, int debug);
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern int D_004428F8[];
extern char str_top[];
extern char str_hidetimeline[];
extern char str_showtimeline[];
extern char str_ReplayMenu[];
extern char str_TransitionOut_b[];

class cSnd_20DF38 {
public:
    virtual void v01();
    virtual void play(int id);
};
extern cSnd_20DF38 gUISoundNull;

class cPad_20DF38 {
public:
    int f0;
    unsigned char port;     // 0x4
    // vptr at 0x8
    virtual void v01();
    virtual void v02();
    virtual int isStart();
    virtual void v04();
    virtual int isNext();
    virtual int isPrevious();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual int isUp();
    virtual int isDown();
};

struct sMgr_20DF38 {
    char pad0[0x84];
    char* game;             // 0x84
    char pad88[0x28];
    void* pads[8];          // 0xB0
};
// Typed view of the game manager (the unit declares SSXApp as void*).
extern sMgr_20DF38* SSXApp_20DF38 __asm__("SSXApp");


static inline char* replay_20DF38() { return *(char**)(SSXApp_20DF38->game + 0x28); }

/**
 * Handles replay input before the menu does: slides the help panel, runs the replay menu, toggles the
 * timeline, and passes the rest to the replay transport.
 *
 * Input from a pad other than the replay's PadNum is swallowed. With the menu closed: ReplayMenuUp / Down
 * (202 / 203) set the panel speed to -4 / +4 (sound 2 when starting from an end or reversing), ReplayTimeLine
 * (204) toggles "top" and the timeline hints, and Start opens the menu (pause mode 2 pushed, +0x9C = 1).
 * With it open, Cross runs the selected item (0 save: overlay 0x12; 1 exit: close, "TransitionOut", and for a
 * loaded replay cGame_exit; 2 continue) and Triangle closes it; both pop the pause-mode stack. For a loaded
 * replay (+0xB4, copied from cReplay Loaded at create) Up on row 1 and Down on row 2 are swallowed, so the
 * hidden Save item is never reached.
 *
 * @param basePInput  the UI input (pad port at +0x4)
 * @return true when handled here, false while the menu is open (the menu handles it), else cReplay_updateInput()'s result
 */
bool cOVState_REPLAY::onInputBegin(cUIInput* basePInput)
{
    cPad_20DF38* pInput = (cPad_20DF38*)basePInput;
    cSnd_20DF38* snd = &gUISoundNull;
    if ((cSnd_20DF38*)mpEngine->mpSound != 0)
        snd = (cSnd_20DF38*)mpEngine->mpSound;
    if (pInput->port != *(int*)(replay_20DF38() + 0x28)) {
        return true;
    }
    if (mShowPopupMenu == 0 && cInput_getState(SSXApp_20DF38->pads[pInput->port], 202)) {
        if (mBottom_y == mMaxBottom_y || mBottom_ydiff == 4) {
            snd->play(2);
        }
        mBottom_ydiff = -4;
        return true;
    }
    if (mShowPopupMenu == 0 && cInput_getState(SSXApp_20DF38->pads[pInput->port], 203)) {
        if (mBottom_y == mMinBottom_y || mBottom_ydiff == -4) {
            snd->play(2);
        }
        mBottom_ydiff = 4;
        return true;
    }
    if (pInput->isStart() && mShowPopupMenu == 0) {
        snd->play(6);
        int n = cPauseState_StackIdx + 1;
        cPauseState_Stack[n] = 2;
        cPauseState_Flags = D_004428F8[0];
        cPauseState_StackIdx = n;
        mShowPopupMenu = 1;
        cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
        *(int*)&obj->mAttrMenu &= ~8;
        return true;
    }
    if (mShowPopupMenu != 0 && pInput->isNext()) {
        snd->play(6);
        cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
        switch (obj->mpSelected->mMiscData) {
        case 0:
            cReplay_stopReplay(replay_20DF38());
            cOVStateManager_triggerOverlay(18);
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            break;
        case 1: {
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
            unsigned short frame = cUIScreen_getFrameByLabel((cUIScreen*)mpScreen, GetHashValue32(str_TransitionOut_b));
            if (frame != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, frame, 1);
            }
            cReplay_stopReplay(replay_20DF38());
            if (mLoaded == 0) {
                cReplay_startAutoReplay(replay_20DF38());
            }
            if (*(int*)(replay_20DF38() + 0x608) != 0) {
                cGame_exit(SSXApp_20DF38->game, 0);
            }
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            break;
        }
        case 2:
            mShowPopupMenu = 0;
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            break;
        }
        return true;
    }
    if (mShowPopupMenu != 0 && pInput->isPrevious()) {
        snd->play(3);
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
        mShowPopupMenu = 0;
        cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
        *(int*)&obj->mAttrMenu |= 8;
        return true;
    }
    if (mShowPopupMenu == 0 && cInput_getState(SSXApp_20DF38->pads[pInput->port], 204)) {
        snd->play(6);
        mTimeLineHidden ^= 1;
        if (mTimeLineHidden != 0) {
            cUIState_hideObjSafe((this), str_top);
            cUIState_hideObjSafe((this), str_hidetimeline);
            cUIState_showObjSafe((this), str_showtimeline);
        } else {
            cUIState_showObjSafe((this), str_top);
            cUIState_showObjSafe((this), str_hidetimeline);
            cUIState_hideObjSafe((this), str_showtimeline);
        }
        return true;
    }
    if (pInput->isUp()) {
        cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
        if (((char*)obj) != 0 && mLoaded != 0 && obj->mScrollItemCurrent == 1) {
            return true;
        }
    } else if (pInput->isDown()) {
        cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
        if (((char*)obj) != 0 && mLoaded != 0 && obj->mScrollItemCurrent == 2) {
            return true;
        }
    }
    if (mShowPopupMenu != 0) {
        return false;
    }
    return cReplay_updateInput(replay_20DF38(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cOVState_onUpdate(cOVState* self);
extern "C" void cReplay_updateCameraSelect(void* p);
extern "C" void cOVState_REPLAY_setupCameraName(cOVState_REPLAY* self);
extern "C" void cOVState_REPLAY_setupTicker(cOVState_REPLAY* self);
extern char str_hidehelp[];
extern char str_showhelp[];
extern char str_ReplayMenuPopup[];
extern char str_bottom[];



/**
 * Updates the replay overlay: slides the help panel, shows the matching hints, and refreshes the camera name
 * and the progress ticker.
 *
 * After cOVState_onUpdate() the panel position (+0xA4) moves by the speed and stops at its ends (+0xAC /
 * +0xB0); "hidehelp" shows at the start, "showhelp" at the end, and "bottom" takes the position as its y.
 * "ReplayMenuPopup" shows while the menu is open, else the replay advances (cReplay_updateCameraSelect).
 *
 */
void cOVState_REPLAY::onUpdate()
{
    cOVState_REPLAY* self = this;
    self->cOVState::onUpdate();
    self->mBottom_y += self->mBottom_ydiff;
    if (self->mBottom_y <= self->mMinBottom_y) {
        self->mBottom_y = self->mMinBottom_y;
        self->mBottom_ydiff = 0;
    } else if (self->mBottom_y >= self->mMaxBottom_y) {
        self->mBottom_y = self->mMaxBottom_y;
        self->mBottom_ydiff = 0;
    }
    if (self->mBottom_y == self->mMinBottom_y) {
        cUIState_showObjSafe(self, str_hidehelp);
        cUIState_hideObjSafe(self, str_showhelp);
    } else if (self->mBottom_y == self->mMaxBottom_y) {
        cUIState_hideObjSafe(self, str_hidehelp);
        cUIState_showObjSafe(self, str_showhelp);
    } else {
        cUIState_hideObjSafe(self, str_hidehelp);
        cUIState_hideObjSafe(self, str_showhelp);
    }
    cUIThing* obj = (cUIThing*)((char*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_bottom)));
    t3Vector bottomPos = obj->mPosition;
    bottomPos.y = (float)self->mBottom_y;
    obj->mPosition = bottomPos;
    if (self->mShowPopupMenu != 0) {
        cUIState_showObjSafe(self, str_ReplayMenuPopup);
    } else {
        cUIState_hideObjSafe(self, str_ReplayMenuPopup);
    }
    if (self->mShowPopupMenu == 0) {
        cReplay_updateCameraSelect(SSXApp->Game->Replay);
    }
    self->setupCameraName();
    self->setupTicker();
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupCameraName);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cOVState_onUpdate(cOVState* self);
extern "C" void cReplay_updateCameraSelect(void* p);
extern "C" void cOVState_REPLAY_setupCameraName(cOVState_REPLAY* self);
extern "C" void cOVState_REPLAY_setupTicker(cOVState_REPLAY* self);
extern char str_hidehelp[];
extern char str_showhelp[];
extern char str_ReplayMenuPopup[];
extern char str_bottom[];

extern char str_camera[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern long D_00471C40[];
extern long D_00471C50[];
extern long D_00471C60[];
extern long D_00471C70[];
extern long D_00471C80[];
extern long D_004A2738[];
extern long D_00471C90[];
extern long D_004A2740[];
extern long D_004A2748[];
extern char D_004A2138[];

/**
 * Shows the name of the current replay camera in the "camera" text.
 *
 * By cReplay CurrentReplayCamIndex: Web-cam, Manual-cam, Near-cam, Mid-cam, Far-cam, then the fixed angles
 * 2:00, 10:00, 4:00 and 8:00 (each after "-  "); other indices print index + 1, which the % 9 cycling never
 * reaches.
 */
// Shows the name of the current replay camera.
void cOVState_REPLAY::setupCameraName()
{
    long buf[2];
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_camera));
    if (text == 0)
        return;
    unsigned int cam = (unsigned int)SSXApp->Game->Replay->CurrentReplayCamIndex;
    switch (cam) {
    case 0: __builtin_memcpy(buf, D_00471C40, 11); break;
    case 1: __builtin_memcpy(buf, D_00471C50, 14); break;
    case 2: __builtin_memcpy(buf, D_00471C60, 12); break;
    case 3: __builtin_memcpy(buf, D_00471C70, 11); break;
    case 4: __builtin_memcpy(buf, D_00471C80, 11); break;
    case 5: __builtin_memcpy(buf, D_004A2738, 8); break;
    case 6: __builtin_memcpy(buf, D_00471C90, 9); break;
    case 7: __builtin_memcpy(buf, D_004A2740, 8); break;
    case 8: __builtin_memcpy(buf, D_004A2748, 8); break;
    default:
        sprintf((char*)buf, D_004A2138, cam + 1);
        break;
    }
    cUIText_setAsciiString(text, (char*)buf);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupTicker);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_percentcomplete[];

/**
 * Moves the "percentcomplete" marker along the timeline to the playback position.
 *
 * x = 170 + 315 * PlaybackTick / EndRecordTick (0 when nothing was recorded).
 */
void cOVState_REPLAY::setupTicker()
{
    char* obj = (char*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_percentcomplete));
    if (obj != 0) {
        t3Vector pos = *(t3Vector*)(obj + 0x44);
        cReplay* replay = SSXApp->Game->Replay;
        int total = replay->EndRecordTick;
        float frac;
        if (total > 0) {
            frac = (float)replay->PlaybackTick / (float)total;
        } else {
            frac = 0.0f;
        }
        pos.x = frac * 315.0f + 170.0f;
        *(t3Vector*)(obj + 0x44) = pos;
    }
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the overlay state's 2D layer with the shared UI-screen render routine cUIState_onRender2D().
 */
void cOVState::onRender2D()
{
    cUIState_onRender2D(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_onUpdate);
#ifdef SKIP_ASM
extern "C" void cGame_activatePauseMenu(void* game, int a);
extern "C" void cUIState_onUpdate(void* self);

/**
 * Updates the overlay state: calls cGame_activatePauseMenu(SSXApp->Game, 1), then the UI-screen update cUIState_onUpdate().
 */
void cOVState::onUpdate()
{
    cGame_activatePauseMenu(SSXApp->Game, 1);
    cUIState_onUpdate(this);
}
#endif

