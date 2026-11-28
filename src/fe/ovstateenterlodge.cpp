#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beintscore.h"
#include "ui/uiengine.h"
#include "main/gamestate.h"
#include "wscript/wscriptman.h"
#include "main/game.h"
#include "sound/ssxaudio.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "fe/ovstateenterlodge.h"
#include "fe/ovstatetoptimes.h"
#include "fe/ovstatebigchallenge.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"
#include "util/locale.h"
#include "ui/uimenu.h"
#include "ui/uiscreen.h"
#include "fe/ovstatemanager.h"

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_98enterlodge[];

/**
 * Builds the Enter Lodge prompt ("98enterlodge"), adds the PDA template and runs the base overlay set-up.
 */
void cOVState_ENTERLODGE::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_98enterlodge), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    cOVState_onCreateScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes_b[];
extern char str_no_b[];
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_onGainFocus(void* self, void* a1);

/**
 * Tags the Yes / No items with their choice ids, then runs the base focus handling.
 *
 * "yes" gets miscData 0 and "no" 1, then cUIState_onGainFocus(self, pPrevState).
 *
 * @param pPrevState  the state that had the focus before (passed on)
 */
void cOVState_ENTERLODGE::onGainFocus(cUIState* pPrevState)
{
    cUIThing* obj = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_yes_b));
    if (((void*)obj) != 0) {
        obj->mMiscData = 0;
    }
    obj = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_no_b));
    if (((void*)obj) != 0) {
        obj->mMiscData = 1;
    }
    cUIState_onGainFocus(this, pPrevState);
}
#endif

extern "C" void* cOVState_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the enter-lodge overlay's screens: clears the word at +0x50, then runs cOVState_onDestroyScreen().
 */
void cOVState_ENTERLODGE::onDestroyScreen()
{
    mRestoreCutSceneMode = 0;
    cOVState_onDestroyScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Picks the Enter Lodge menu's button feedback: Square and Circle animate silently.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param key  the menu event (8 Square, 9 Circle)
 * @return 0x100 (animation only) for events 8 and 9, else 0x101 (animation and sound)
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
int cOVState_ENTERLODGE::onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onWidgetEvent);
#ifdef SKIP_ASM
extern int cRealFE_mStartState;
extern "C" void cGame_exit(void* game, int a);
extern "C" void cUIStateStack_pop(void*, int);
struct sSelf001F72E0 {
    char pad[0x10];
    char* mgr;
    char pad14[0x40];
    int done;
};

/**
 * Handles the Enter Lodge prompt: Yes leaves the race for the lodge, No or Triangle closes the prompt.
 *
 * Cross and Triangle first set word +0x8 of Game->mCurrentState (flag->active here) to 1. Yes (choice 0) sets
 * the front-end start state cRealFE_mStartState to 0x27, which cRealFE_loadStartState turns into the lodge state, and
 * exits the game (cGame_exit). No (1) and Triangle set mRemovePDA (+0x54, called done here) and close the
 * state.
 *
 * @param widget  the item (On Tour pWidget); ignored when null
 * @param msg  the widget event: kONNEXT Cross, kONPREV Triangle
 */
void cOVState_ENTERLODGE::onWidgetEvent(cUIThing* widget, unsigned int msg)
{
    if (((void*)widget) != 0) {
        switch (msg) {
        case cUIState::kONNEXT:
            ((SSXApp->Game)->mCurrentState)->mOverlayDone = 1;
            switch (widget->mMiscData) {
            case 0:
                {
                    void* game = SSXApp->Game;
                    cRealFE_mStartState = 39;
                    cGame_exit(game, 0);
                }
                break;
            case 1:
                mRemovePDA = 1;
                cUIStateStack_pop(&mpEngine->mStateStack, 1);
                break;
            }
            break;
        case cUIState::kONPREV:
            mRemovePDA = 1;
            ((SSXApp->Game)->mCurrentState)->mOverlayDone = 1;
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_63bc_start[];

/**
 * Builds the Big Challenge offer screen ("63bc_start") and runs the base overlay set-up.
 */
void cOVState_BIGCHALLENGE_START::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_63bc_start), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBEMissionInterface_getMissionName(void* iface, int id);
extern "C" void* cBEMissionInterface_getMissionLongDesc(void* iface, int id);
extern "C" void cUIText_setUnicodeString(void* text, void* str);
extern "C" void cUIText_setUnicodeStringWrap(void* text, void* str);
extern char str_chalname[];
extern char str_ObjText[];

/**
 * On the first call, fills in the offered Big Challenge's name and long description.
 *
 * For the mission id at +0x9C the mission interface (0xA) supplies "chalname" (getMissionName) and the
 * word-wrapped "ObjText" (getMissionLongDesc).
 *
 * @param gained  nonzero on the first call (On Tour bFirstTime)
 * @return always 0
 */
bool cOVState_BIGCHALLENGE_START::onGainTransition(int gained)
{
    if (gained) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
        void* obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_chalname));
        if (obj) {
            cUIText_setUnicodeString(obj, cBEMissionInterface_getMissionName(iface, mMissionId));
        }
        obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ObjText));
        if (obj) {
            cUIText_setUnicodeStringWrap(obj, cBEMissionInterface_getMissionLongDesc(iface, mMissionId));
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes_b[];
extern char str_no_b[];

/**
 * Tags the offer's Yes / No items with their choice ids ("yes" 0, "no" 1).
 *
 * @param item  the widget being created
 */
void cOVState_BIGCHALLENGE_START::onWidgetCreate(cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(str_yes_b)) {
        item->mMiscData = 0;
    } else {
        int id2 = item->mHashName;
        if (id2 == GetHashValue32(str_no_b)) {
            item->mMiscData = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_getChallengeData);
#ifdef SKIP_ASM
extern void* cWScriptMan_TheWScriptMan;
extern "C" int* tMissionMenuQueue_getNextEntry(void* list);

/**
 * Picks the mission the Big Challenge start overlay offers: the next entry of the mission menu queue, or the local
 * mission when the queue is empty.
 *
 * Reads the queue at cWScriptMan_TheWScriptMan + 0x1C4 with tMissionMenuQueue_getNextEntry; its entry's first word, or
 * else the local mission id at +0x2A0, is stored as the overlay's mission id (+0x9C).
 */
void cOVState_BIGCHALLENGE_START::getChallengeData()
{
    int* queueEntry = tMissionMenuQueue_getNextEntry((char*)cWScriptMan_TheWScriptMan + 0x1C4);
    if (queueEntry != 0) {
        mMissionId = *queueEntry;
    } else {
        mMissionId = *(int*)((char*)cWScriptMan_TheWScriptMan + 0x2A0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Picks the offer menu's button feedback: only Cross animates and plays a sound.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (6 Cross)
 * @return 0x101 for kONNEXT, else 0
 */
int cOVState_BIGCHALLENGE_START::onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
{
    return Event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
void cSSXAudio_SetBigChallengeStarting(void* self);
extern "C" void cWScriptMan_startLocalMission(void* self);
extern "C" void cWScriptMan_cancelLocalMission(void* self);
extern "C" void cWScriptMan_restartLocalMission(void* self);
extern "C" int cWScriptMan_isLocalMissionActive(void* self);
extern void* cWScriptMan_TheWScriptMan;
extern int cOVStateManager_mStatus;
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];


/**
 * Answers the Big Challenge offer: Yes starts the challenge, No or Triangle declines it, and the overlay
 * closes.
 *
 * Yes (Cross on choice 0) queues WScriptMan operation 2 (cWScriptMan_restartLocalMission, probably a restart) when a challenge
 * already runs (cWScriptMan_isLocalMissionActive), else stores the offer's mission id (+0x9C) in cWScriptMan_TheWScriptMan +0x2A0 and queues
 * operation 1 (start, cWScriptMan_startLocalMission); it also sets the audio's mBigChallengeStarting (cSSXAudio_SetBigChallengeStarting). No
 * falls through to Triangle: store the id, queue operation 3 (decline, cWScriptMan_cancelLocalMission).
 * Both set the pending overlay action cOVStateManager_mStatus to 3 (close) and pop the pause-mode stack cPauseState_Stack,
 * restoring the pause mask cPauseState_Flags.
 *
 * @param item  the selected item; ignored when null
 * @param msg  the widget event: kONNEXT Cross, kONPREV Triangle
 */
void cOVState_BIGCHALLENGE_START::onWidgetEvent(cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case cUIState::kONNEXT:
        if (item->mMiscData == 0) {
            if (cWScriptMan_isLocalMissionActive(cWScriptMan_TheWScriptMan) != 0) {
                cWScriptMan_restartLocalMission(cWScriptMan_TheWScriptMan);
            } else {
                ((cWScriptMan*)cWScriptMan_TheWScriptMan)->LocalMissionId = mMissionId;
                cWScriptMan_startLocalMission(cWScriptMan_TheWScriptMan);
            }
            cOVStateManager_mStatus = 3;
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            cSSXAudio_SetBigChallengeStarting(cAudioInterface_bxAudio());
            break;
        }
    case cUIState::kONPREV:
        ((cWScriptMan*)cWScriptMan_TheWScriptMan)->LocalMissionId = mMissionId;
        cWScriptMan_cancelLocalMission(cWScriptMan_TheWScriptMan);
        cOVStateManager_mStatus = 3;
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_FAIL_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_90bc_fail[];

/**
 * Creates the Big Challenge failed overlay's screen ("90bc_fail") and starts its animation.
 *
 * Sets mHashName to the hash of "90bc_fail", adds that screen to mpEngine for this state
 * (cUIEngine_addScreenByHashName) into mpScreen, plays it from frame 0 when it exists, then runs
 * cOVState_onCreateScreen. It is the onCreateScreen slot of this overlay's vtable cOVState_BIGCHALLENGE_FAIL_vtbl.
 */
void cOVState_BIGCHALLENGE_FAIL::onCreateScreen()
{
    void* engine;
    void* screen;
    mHashName = GetHashValue32(str_90bc_fail);
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_90bc_fail), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_FAIL_getChallengeData);
#ifdef SKIP_ASM
extern void* cWScriptMan_TheWScriptMan;
extern "C" int* tMissionMenuQueue_getNextEntry(void* list);

/**
 * Takes the Big Challenge failed overlay's mission: the next entry of the mission menu queue.
 *
 * Stores the first word of tMissionMenuQueue_getNextEntry(cWScriptMan_TheWScriptMan + 0x1C4) as the mission id (+0x9C).
 * Unlike cOVState_BIGCHALLENGE_START_getChallengeData it does not check for an empty queue (likely bug if the queue can be empty: a null dereference).
 */
void cOVState_BIGCHALLENGE_FAIL::getChallengeData()
{
    mMissionId = *tMissionMenuQueue_getNextEntry((char*)cWScriptMan_TheWScriptMan + 0x1C4);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_FAIL_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes_b[];
extern char str_no_b[];
extern char str_info[];

/**
 * Tags the Big Challenge failed overlay's buttons as they are created: "yes_b" gets misc data 0, "no_b" 1 and "info" 2.
 *
 * Compares the widget's hash name (+0x38) with the hashes of the three names and stores the index in its misc data
 * (+0x18), which cOVState_BIGCHALLENGE_FAIL_onWidgetEvent switches on.
 *
 * @param item  the widget being created
 */
void cOVState_BIGCHALLENGE_FAIL::onWidgetCreate(cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(str_yes_b)) {
        item->mMiscData = 0;
    } else {
        int id2 = item->mHashName;
        if (id2 == GetHashValue32(str_no_b)) {
            item->mMiscData = 1;
        } else {
            int id3 = item->mHashName;
            if (id3 == GetHashValue32(str_info)) {
                item->mMiscData = 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_FAIL_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Menu-event filter of the Big Challenge failed overlay: kONNEXT gets 0x101 (animation and sound), every other
 * event 0.
 *
 * @param menu  unused
 * @param event  the menu event
 * @return 0x101 for kONNEXT, else 0
 */
int cOVState_BIGCHALLENGE_FAIL::onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_FAIL_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_triggerOverlay(int id);
// PORT: cUIStateStack_pop returns the last node; the unit declares it void.
void* cUIStateStack_pop_r(void* self, int a1) __asm__("cUIStateStack_pop");
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern char str_TransitionOut_b[];

/**
 * Handles the Big Challenge failed overlay's buttons: retry restarts the challenge, info opens overlay 29, and anything
 * else (no, back) gives the challenge up.
 *
 * Ignored for a null @p item. kONNEXT (select) on "yes_b" (misc data 0) puts the mission id (+0x9C) into
 * cWScriptMan_TheWScriptMan + 0x2A0, restarts it (cWScriptMan_restartLocalMission), pops this state from mpEngine's
 * state stack (cUIStateStack_pop), pops the pause-mode stack (cPauseState_Stack / cPauseState_StackIdx) into cPauseState_Flags and calls
 * cSSXAudio_SetBigChallengeStarting on the audio object. On "info" (misc data 2) it opens overlay 29 (cOVStateManager_triggerOverlay) and
 * stays. kONPREV (back) first plays mpScreen's "TransitionOut_b" frame when it has one. kONPREV, and kONNEXT on any
 * other widget, then cancel the challenge (cWScriptMan_cancelLocalMission), pop the state and restore cPauseState_Flags
 * the same way. Other events do nothing.
 *
 * @param item  the widget; its misc data (+0x18) is set by cOVState_BIGCHALLENGE_FAIL_onWidgetCreate
 * @param msg  the widget event: 5 select, 6 back
 */
void cOVState_BIGCHALLENGE_FAIL::onWidgetEvent(cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case cUIState::kONNEXT: {
        int choice = item->mMiscData;
        if (choice == 0) {
            ((cWScriptMan*)cWScriptMan_TheWScriptMan)->LocalMissionId = mMissionId;
            cWScriptMan_restartLocalMission(cWScriptMan_TheWScriptMan);
            cUIStateStack_pop_r(&mpEngine->mStateStack, 1);
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            cSSXAudio_SetBigChallengeStarting(cAudioInterface_bxAudio());
            return;
        }
        if (choice == 2) {
            cOVStateManager_triggerOverlay(29);
            return;
        }
        break;
    }
    case cUIState::kONPREV: {
        int frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_TransitionOut_b));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(mpScreen, frame & 0xFFFF, 1);
        }
        break;
    }
    default:
        return;
    }
    cWScriptMan_cancelLocalMission(cWScriptMan_TheWScriptMan);
    cUIStateStack_pop_r(&mpEngine->mStateStack, 1);
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
// PORT: bound with an int return by asm label; the unit declares it void
int cUIText_setAsciiString_i(cUIText* self, const char* str) __asm__("cUIText_setAsciiString__FP7cUITextPCc");
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int cBENewRaceInterface_getLocationName1(void* iface);
extern "C" char* cBEScoreInterface_getCurrentHighScore(void* iface, int i);
extern "C" char* cBEScoreInterface_getCurrentHighScoreCharacter(void* iface, int i);
extern "C" char* cBEScoreInterface_getCurrentHighScoreName(void* iface, int i);
extern "C" int getPeakChallengeEventName();
extern "C" char* formatMinutesSeconds(char* s);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void* p, int id);
extern "C" void strcpy1(void* str, int a1);
extern "C" void strcpy3(void* str, const char* s);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...)
extern "C" void sprintf1_1F7958(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeString(void* text, void* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int cOVStateManager_mMiscData;
extern char str_61toptimes[];
extern char str_OV_darkblue[];
extern char str_pct_S_Jam[];
extern char str_track_xxx[];
extern char str_kT_cmnRaceCaps_b[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_pct_S_pct_s[];
extern char str_time[];
extern char str_points_b[];
extern char str_top5record[];
extern char str_top5scores[];
extern char str_helptext01[];
extern char str_helptext02[];
extern char str_ngca_b[];
extern char str_xba_b[];
extern char str_ps2x_b[];
extern char str_name0pct_d[];
extern char str_with0pct_d[];
extern char str_time0pct_d[];
extern char D_004A2138[];

struct sVEntry_1F7958 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc1F7958(char* name)
{
    cFELocale* obj = SSXApp->FELocale;
    sVEntry_1F7958* vt = (sVEntry_1F7958*)VPTR_OF(cUILocaleInterface, obj);
    return vt[4].fn(vthis(obj, vt[4]), GetHashValue32(name));
}

/**
 * Builds the top-times panel ("61toptimes" over "OV_darkblue") for the current event and fills its five rows.
 *
 * "track_xxx" names the event as the rewards overlay does. Race modes (cGameMode 0 / 5) show time, top5record
 * and helptext01, other modes points, top5scores and helptext02; ngca, xba and ps2x are shown, and both help
 * texts hidden while cOVStateManager_mMiscData is set. Rows 1-5 get name0N / with0N / time0N from the score interface (8),
 * the time formatted by formatMinutesSeconds or the points as "%d". Plays UI event 0xE.
 * getPeakChallengeEventName() is declared without parameters here; retail relies on $a0 still holding the event kind.
 */
void cOVState_TOPTIMES::onCreateScreen()
{
    char buf[0x40];
    char line[0x40];
    unsigned short wtext[104];
    unsigned short wfmt[0x10];
    int full;
    mIsNextScreenPending = 0;
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)this), GetHashValue32(str_61toptimes), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, ((char*)this), GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    full = 1;
    cOVState_onCreateScreen(((char*)this));
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* stats = cBE_getInterface_Fv(cBE_getBE(), 8);
    int ev = cBENewRaceInterface_newRace.cGameEvent;
    int mode = cBENewRaceInterface_newRace.cGameMode;
    switch (ev) {
    case kGAMEEVENT_TIME_PEAK1 ... kGAMEEVENT_POINTS_PEAK3:
        strcpy1(wtext, getPeakChallengeEventName());
        break;
    case kGAMEEVENT_RIVALPOINTS:
        strcpy3(wfmt, str_pct_S_Jam);
        sprintf1_1F7958(wtext, wfmt, cBENewRaceInterface_getLocationName1(race));
        break;
    default: {
        int name = 0;
        switch (mode) {
        case kGAMEMODE_RACE:
        case kGAMEMODE_TIMECHALLENGE:
            name = Loc1F7958(str_kT_cmnRaceCaps_b);
            break;
        case kGAMEMODE_SLOPESTYLE:
            name = Loc1F7958(str_kT_cmnSlopeCaps);
            break;
        case kGAMEMODE_BIGAIR:
            name = Loc1F7958(str_kT_cmnBigAirCaps);
            break;
        case kGAMEMODE_HALFPIPE:
            name = Loc1F7958(str_kT_cmnPipeCaps);
            break;
        }
        strcpy3(wfmt, str_pct_S_pct_s);
        sprintf1_1F7958(wtext, wfmt, cBENewRaceInterface_getLocationName1(race), name);
        break;
    }
    }
    cUIText_setUnicodeString(cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_track_xxx)), wtext);
    int gameMode = cBENewRaceInterface_newRace.cGameMode;
    if (gameMode == kGAMEMODE_RACE || gameMode == kGAMEMODE_TIMECHALLENGE) {
        cUIState_showObjSafe(((char*)this), str_time);
        cUIState_hideObjSafe(((char*)this), str_points_b);
        cUIState_showObjSafe(((char*)this), str_top5record);
        cUIState_hideObjSafe(((char*)this), str_top5scores);
        cUIState_showObjSafe(((char*)this), str_helptext01);
        cUIState_hideObjSafe(((char*)this), str_helptext02);
    } else {
        cUIState_hideObjSafe(((char*)this), str_time);
        cUIState_showObjSafe(((char*)this), str_points_b);
        cUIState_hideObjSafe(((char*)this), str_top5record);
        cUIState_showObjSafe(((char*)this), str_top5scores);
        cUIState_hideObjSafe(((char*)this), str_helptext01);
        cUIState_showObjSafe(((char*)this), str_helptext02);
        full = 0;
    }
    cUIState_showObjSafe(((char*)this), str_ngca_b);
    cUIState_showObjSafe(((char*)this), str_xba_b);
    cUIState_showObjSafe(((char*)this), str_ps2x_b);
    if (cOVStateManager_mMiscData != 0) {
        cUIState_hideObjSafe(((char*)this), str_helptext01);
        cUIState_hideObjSafe(((char*)this), str_helptext02);
    }
    for (int i = 1; i < 6; i++) {
        sprintf(buf, str_name0pct_d, i);
        cUIText* nameText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        cUIText_setAsciiString_i(nameText, cBEScoreInterface_getCurrentHighScoreName(stats, i - 1));
        sprintf(buf, str_with0pct_d, i);
        cUIText* withText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        cUIText_setAsciiString_i(withText, cBEScoreInterface_getCurrentHighScoreCharacter(stats, i - 1));
        sprintf(buf, str_time0pct_d, i);
        cUIText* timeText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        if (full == true) {
            sprintf(line, formatMinutesSeconds(cBEScoreInterface_getCurrentHighScore(stats, i - 1)));
        } else {
            sprintf(line, D_004A2138, cBEScoreInterface_getCurrentHighScore(stats, i - 1));
        }
        cUIText_setAsciiString_i(timeText, line);
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* self, int id);
extern char str_continue[];
extern char str_kT_CMNContinue_b[];
extern char str_kT_OVRCMNReturn[];
extern char str_saverecords[];
extern int cOVStateManager_mMiscData;

static inline int isHashTOPTIMES(int id, char* s)
{
    return id == GetHashValue32(s);
}

/**
 * Sets up the top-times buttons: Continue (choice 0) or Return, and Save Records (choice 1).
 *
 * "continue" reads kT_CMNContinue, or kT_OVRCMNReturn while cOVStateManager_mMiscData is set; "saverecords" gets miscData 1.
 *
 * @param pWidget  the widget being created
 */
void cOVState_TOPTIMES::onWidgetCreate(cUIThing* pWidget)
{
    cUIText* w = (cUIText*)pWidget;
    int id = w->mHashName;
    if (isHashTOPTIMES(id, str_continue)) {
        if (cOVStateManager_mMiscData == 0) {
            cUIText_setUnicodeStringByID(w, GetHashValue32(str_kT_CMNContinue_b));
        } else {
            cUIText_setUnicodeStringByID(w, GetHashValue32(str_kT_OVRCMNReturn));
        }
        w->mMiscData = 0;
    } else if (isHashTOPTIMES(id, str_saverecords)) {
        w->mMiscData = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onGainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIState_onGainFocus(void* self, void* a1);
extern "C" void cOVState_triggerTextFade(void* self, const char* name, const char* ext);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_name0pct_d[];
extern char str_with0pct_d[];
extern char str_time0pct_d[];
extern char D_004A2440[];
extern char str_at_TextPulseWhiteOrange[];

static inline void LoadRider1F7F08(void* self, int n)
{
    char name[32];
    sprintf(name, D_004A2440, n);
    cOVState_triggerTextFade(self, name, str_at_TextPulseWhiteOrange);
    sprintf(name, str_name0pct_d, n);
    cOVState_triggerTextFade(self, name, str_at_TextPulseWhiteOrange);
    sprintf(name, str_with0pct_d, n);
    cOVState_triggerTextFade(self, name, str_at_TextPulseWhiteOrange);
    sprintf(name, str_time0pct_d, n);
    cOVState_triggerTextFade(self, name, str_at_TextPulseWhiteOrange);
}

/**
 * Runs the base focus handling, then pulses the rows of the players' new top-five entries.
 *
 * For the new record rank at score interface (8) +0x18, and at +0x1C when cGamePath is 2, plays
 * "@TextPulseWhiteOrange" (cOVState_triggerTextFade) on that row's 0N, name0N, with0N and time0N texts; a
 * negative rank means no new record.
 *
 * @param pPrevState  the state that had the focus before (passed on)
 */
void cOVState_TOPTIMES::onGainFocus(cUIState* pPrevState)
{
    cUIState_onGainFocus(this, pPrevState);
    cBEScoreInterface* prof = (cBEScoreInterface*)((char*)cBE_getInterface_Fv(cBE_getBE(), 8));
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int id = prof->mNewTrackHighScoreIndex[0];
    if (id >= 0) {
        LoadRider1F7F08(this, id + 1);
    }
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        int id2 = prof->mNewTrackHighScoreIndex[1];
        if (id2 >= 0) {
            LoadRider1F7F08(this, id2 + 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Picks the top-times menu's button feedback: only Cross animates and plays a sound.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (6 Cross)
 * @return 0x101 for kONNEXT, else 0
 */
int cOVState_TOPTIMES::onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
{
    return Event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onWidgetEvent);
#ifdef SKIP_ASM

/**
 * On Cross, passes the chosen button to the panel's virtual slot 26 (cOVState_TOPTIMES_handleNext: continue or save
 * records).
 *
 * The call passes only this object, while cOVState_TOPTIMES_handleNext takes the item as its second argument: it gets
 * @p pWidget only because the argument register still holds it, so a port must pass it explicitly.
 *
 * @param pWidget  the chosen item; ignored when null
 * @param Event  the widget event; only 5 (Cross) acts
 */
void cOVState_TOPTIMES::onWidgetEvent(cUIThing* pWidget, unsigned int Event)
{
    if (((void*)pWidget) != 0) {
        if (Event == cUIState::kONNEXT) {
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cListNode, this);
            vt[26].fn(vthis(this, vt[26]));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_handleNext);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void cOVStateManager_triggerOverlay(int id);

/**
 * Acts on the button chosen on the top-times panel: continue, or save the records.
 *
 * Only acts once (+0xA0 still 0). The chosen item's index (@p pWidget +0x18) 0 sets +0xA0 and cOVStateManager_mStatus
 * to kOVSTATUS_GOTONEXT (continue); 1 sets +0xA0 and opens kOVSTATE_SAVEOPTIONS with cOVStateManager_triggerOverlay()
 * (save). It is virtual slot 26, called by cOVState_TOPTIMES_onWidgetEvent() on Cross.
 *
 * @param pWidget  the chosen item (On Tour: cUIThing *pWidget)
 */
void cOVState_TOPTIMES::handleNext(cUIThing* pWidget)
{
    if (mIsNextScreenPending == 0) {
        int choice = pWidget->mMiscData;
        switch (choice) {
        case 0:
            mIsNextScreenPending = 1;
            cOVStateManager_mStatus = kOVSTATUS_GOTONEXT;
            break;
        case 1:
            mIsNextScreenPending = choice;
            cOVStateManager_triggerOverlay(kOVSTATE_SAVEOPTIONS);
            break;
        }
    }
}
#endif

