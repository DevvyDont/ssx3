#include "common.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"

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
extern "C" void cOVState_ENTERLODGE_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_98enterlodge), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes_b[];
extern char str_no_b[];
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_onGainFocus(void* self, void* a1);

/**
 * Tags the Yes / No items with their choice ids, then runs the base focus handling.
 *
 * "yes" gets miscData 0 and "no" 1, then cUIState_onGainFocus(self, pPrevState).
 *
 * @param pPrevState  the state that had the focus before (passed on)
 */
extern "C" void cOVState_ENTERLODGE_onGainFocus(void* self, void* pPrevState)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_yes_b));
    if (obj != 0) {
        *(int*)((char*)obj + 0x18) = 0;
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_no_b));
    if (obj != 0) {
        *(int*)((char*)obj + 0x18) = 1;
    }
    cUIState_onGainFocus(self, pPrevState);
}
#endif

extern "C" void* cOVState_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the enter-lodge overlay's screens: clears the word at +0x50, then runs cOVState_onDestroyScreen().
 *
 * @return cOVState_onDestroyScreen()'s return value (On Tour's returns nothing)
 */
void* cOVState_ENTERLODGE_onDestroyScreen(void* self)
{
    *(int*)((char*)self + 0x50) = 0;
    return cOVState_onDestroyScreen(self);
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
extern "C" int cOVState_ENTERLODGE_onWidgetMenu(void* self, int pMenu, unsigned int key)
{
    int result = 0x101;
    if (key < 10) {
        if (key < 8) {
            result = 0x101;
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
extern "C" void func_0039F190(void*, int);
struct sFlag001F72E0 {
    int f0;
    int f4;
    int active;
};
struct sGame001F72E0 {
    char pad[0x200];
    sFlag001F72E0* flag;
};
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
 * @param msg  the widget event: 5 Cross, 6 Triangle
 */
extern "C" void cOVState_ENTERLODGE_onWidgetEvent(sSelf001F72E0* self, void* widget, int msg)
{
    if (widget != 0) {
        switch (msg) {
        case 5:
            ((sGame001F72E0*)SSXApp->Game)->flag->active = 1;
            switch (*(int*)((char*)widget + 0x18)) {
            case 0:
                {
                    void* game = SSXApp->Game;
                    cRealFE_mStartState = 0x27;
                    cGame_exit(game, 0);
                }
                break;
            case 1:
                self->done = 1;
                func_0039F190(self->mgr + 0x18, 1);
                break;
            }
            break;
        case 6:
            self->done = 1;
            ((sGame001F72E0*)SSXApp->Game)->flag->active = 1;
            func_0039F190(self->mgr + 0x18, 1);
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
extern "C" void cOVState_BIGCHALLENGE_START_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_63bc_start), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
extern "C" int cOVState_BIGCHALLENGE_START_onGainTransition(void* self, int gained)
{
    if (gained) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xA);
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_chalname));
        if (obj) {
            cUIText_setUnicodeString(obj, cBEMissionInterface_getMissionName(iface, *(int*)((char*)self + 0x9C)));
        }
        obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ObjText));
        if (obj) {
            cUIText_setUnicodeStringWrap(obj, cBEMissionInterface_getMissionLongDesc(iface, *(int*)((char*)self + 0x9C)));
        }
    }
    return 0;
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
extern "C" void cOVState_BIGCHALLENGE_START_onWidgetCreate(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(str_yes_b)) {
        *(int*)((char*)item + 0x18) = 0;
    } else {
        int id2 = *(int*)((char*)item + 0x38);
        if (id2 == GetHashValue32(str_no_b)) {
            *(int*)((char*)item + 0x18) = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7548);
#ifdef SKIP_ASM
extern void* cWScriptMan_TheWScriptMan;
extern "C" int* func_0030C820(void* list);

extern "C" void func_001F7548(void* self)
{
    int* p = func_0030C820((char*)cWScriptMan_TheWScriptMan + 0x1C4);
    if (p != 0) {
        *(int*)((char*)self + 0x9C) = *p;
    } else {
        *(int*)((char*)self + 0x9C) = *(int*)((char*)cWScriptMan_TheWScriptMan + 0x2A0);
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
 * @return 0x101 for event 6, else 0
 */
extern "C" int cOVState_BIGCHALLENGE_START_onWidgetMenu(void* self, int pMenu, int Event)
{
    return Event == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
void func_0029D6D0(void* self);
extern "C" void func_0030B540(void* self);
extern "C" void func_0030B658(void* self);
extern "C" void func_0030B6F8(void* self);
extern "C" int func_0030B8C0(void* self);
extern void* cWScriptMan_TheWScriptMan;
extern int cOVStateManager_mStatus;
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_004428F0[];

struct sMgr_75A0 {
    char pad[0x2A0];
    int f2A0;
};

/**
 * Answers the Big Challenge offer: Yes starts the challenge, No or Triangle declines it, and the overlay
 * closes.
 *
 * Yes (Cross on choice 0) queues WScriptMan operation 2 (func_0030B6F8, probably a restart) when a challenge
 * already runs (func_0030B8C0), else stores the offer's mission id (+0x9C) in cWScriptMan_TheWScriptMan +0x2A0 and queues
 * operation 1 (start, func_0030B540); it also sets the audio's mBigChallengeStarting (func_0029D6D0). No
 * falls through to Triangle: store the id, queue operation 3 (decline, func_0030B658).
 * Both set the pending overlay action cOVStateManager_mStatus to 3 (close) and pop the pause-mode stack D_005366E8,
 * restoring the pause mask cPauseState_Flags.
 *
 * @param item  the selected item; ignored when null
 * @param msg  the widget event: 5 Cross, 6 Triangle
 */
extern "C" void cOVState_BIGCHALLENGE_START_onWidgetEvent(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5:
        if (*(int*)((char*)item + 0x18) == 0) {
            if (func_0030B8C0(cWScriptMan_TheWScriptMan) != 0) {
                func_0030B6F8(cWScriptMan_TheWScriptMan);
            } else {
                ((sMgr_75A0*)cWScriptMan_TheWScriptMan)->f2A0 = *(int*)((char*)self + 0x9C);
                func_0030B540(cWScriptMan_TheWScriptMan);
            }
            cOVStateManager_mStatus = 3;
            cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
            func_0029D6D0(func_0028B180());
            break;
        }
    case 6:
        ((sMgr_75A0*)cWScriptMan_TheWScriptMan)->f2A0 = *(int*)((char*)self + 0x9C);
        func_0030B658(cWScriptMan_TheWScriptMan);
        cOVStateManager_mStatus = 3;
        cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F76B8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_90bc_fail[];

extern "C" void func_001F76B8(void* self)
{
    void* engine;
    void* screen;
    *(int*)((char*)self + 0xC) = GetHashValue32(str_90bc_fail);
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_90bc_fail), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7738);
#ifdef SKIP_ASM
extern void* cWScriptMan_TheWScriptMan;
extern "C" int* func_0030C820(void* list);

extern "C" void func_001F7738(void* self)
{
    *(int*)((char*)self + 0x9C) = *func_0030C820((char*)cWScriptMan_TheWScriptMan + 0x1C4);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7770);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_yes_b[];
extern char str_no_b[];
extern char str_info[];

extern "C" void func_001F7770(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(str_yes_b)) {
        *(int*)((char*)item + 0x18) = 0;
    } else {
        int id2 = *(int*)((char*)item + 0x38);
        if (id2 == GetHashValue32(str_no_b)) {
            *(int*)((char*)item + 0x18) = 1;
        } else {
            int id3 = *(int*)((char*)item + 0x38);
            if (id3 == GetHashValue32(str_info)) {
                *(int*)((char*)item + 0x18) = 2;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F77F0);
#ifdef SKIP_ASM
extern "C" int func_001F77F0(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7800);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_triggerOverlay(int id);
// PORT: func_0039F190 returns the last node; the unit declares it void.
void* func_0039F190_r(void* self, int a1) __asm__("func_0039F190");
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern char str_TransitionOut_b[];

extern "C" void func_001F7800(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        int v = *(int*)((char*)item + 0x18);
        if (v == 0) {
            ((sMgr_75A0*)cWScriptMan_TheWScriptMan)->f2A0 = *(int*)((char*)self + 0x9C);
            func_0030B6F8(cWScriptMan_TheWScriptMan);
            func_0039F190_r((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
            cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
            func_0029D6D0(func_0028B180());
            return;
        }
        if (v == 2) {
            cOVStateManager_triggerOverlay(0x1D);
            return;
        }
        break;
    }
    case 6: {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(str_TransitionOut_b));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame & 0xFFFF, 1);
        }
        break;
    }
    default:
        return;
    }
    func_0030B658(cWScriptMan_TheWScriptMan);
    func_0039F190_r((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int func_00144BF0(void* iface);
extern "C" char* func_00155130(void* iface, int i);
extern "C" char* func_00155190(void* iface, int i);
extern "C" char* func_00155208(void* iface, int i);
extern "C" int func_001FD190();
extern "C" char* func_0020A1A8(char* s);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* func_0028B180();
extern "C" void func_00294F78(void* p, int id);
extern "C" void strcpy1(void* str, int a1);
extern "C" void func_002C2540(void* str, const char* s);
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
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_1F7958* vt = *(sVEntry_1F7958**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(name));
}

/**
 * Builds the top-times panel ("61toptimes" over "OV_darkblue") for the current event and fills its five rows.
 *
 * "track_xxx" names the event as the rewards overlay does. Race modes (cGameMode 0 / 5) show time, top5record
 * and helptext01, other modes points, top5scores and helptext02; ngca, xba and ps2x are shown, and both help
 * texts hidden while cOVStateManager_mMiscData is set. Rows 1-5 get name0N / with0N / time0N from the score interface (8),
 * the time formatted by func_0020A1A8 or the points as "%d". Plays UI event 0xE.
 * func_001FD190() is declared without parameters here; retail relies on $a0 still holding the event kind.
 */
extern "C" void cOVState_TOPTIMES_onCreateScreen(char* self)
{
    char buf[0x40];
    char line[0x40];
    unsigned short wtext[0x68];
    unsigned short wfmt[0x10];
    int full;
    *(int*)(self + 0xA0) = 0;
    void* engine = *(void**)(self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_61toptimes), 0);
    *(void**)(self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)(self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    full = 1;
    cOVState_onCreateScreen(self);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* stats = cBE_getInterface_Fv(cBE_getBE(), 8);
    signed char* raceDB = (signed char*)((char*)&cBENewRaceInterface_newRace);
    int ev = raceDB[0x4A];
    int mode = raceDB[0x48];
    switch (ev) {
    case 6 ... 11:
        strcpy1(wtext, func_001FD190());
        break;
    case 5:
        func_002C2540(wfmt, str_pct_S_Jam);
        sprintf1_1F7958(wtext, wfmt, func_00144BF0(race));
        break;
    default: {
        int name = 0;
        switch (mode) {
        case 0:
        case 5:
            name = Loc1F7958(str_kT_cmnRaceCaps_b);
            break;
        case 1:
            name = Loc1F7958(str_kT_cmnSlopeCaps);
            break;
        case 2:
            name = Loc1F7958(str_kT_cmnBigAirCaps);
            break;
        case 3:
            name = Loc1F7958(str_kT_cmnPipeCaps);
            break;
        }
        func_002C2540(wfmt, str_pct_S_pct_s);
        sprintf1_1F7958(wtext, wfmt, func_00144BF0(race), name);
        break;
    }
    }
    cUIText_setUnicodeString(cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_track_xxx)), wtext);
    signed char* newRace = (signed char*)((char*)&cBENewRaceInterface_newRace);
    int gameMode = newRace[0x48];
    if (gameMode == 0 || gameMode == 5) {
        cUIState_showObjSafe(self, str_time);
        cUIState_hideObjSafe(self, str_points_b);
        cUIState_showObjSafe(self, str_top5record);
        cUIState_hideObjSafe(self, str_top5scores);
        cUIState_showObjSafe(self, str_helptext01);
        cUIState_hideObjSafe(self, str_helptext02);
    } else {
        cUIState_hideObjSafe(self, str_time);
        cUIState_showObjSafe(self, str_points_b);
        cUIState_hideObjSafe(self, str_top5record);
        cUIState_showObjSafe(self, str_top5scores);
        cUIState_hideObjSafe(self, str_helptext01);
        cUIState_showObjSafe(self, str_helptext02);
        full = 0;
    }
    cUIState_showObjSafe(self, str_ngca_b);
    cUIState_showObjSafe(self, str_xba_b);
    cUIState_showObjSafe(self, str_ps2x_b);
    if (cOVStateManager_mMiscData != 0) {
        cUIState_hideObjSafe(self, str_helptext01);
        cUIState_hideObjSafe(self, str_helptext02);
    }
    for (int i = 1; i < 6; i++) {
        sprintf(buf, str_name0pct_d, i);
        cUIText* nameText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(buf));
        cUIText_setAsciiString_i(nameText, func_00155208(stats, i - 1));
        sprintf(buf, str_with0pct_d, i);
        cUIText* withText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(buf));
        cUIText_setAsciiString_i(withText, func_00155190(stats, i - 1));
        sprintf(buf, str_time0pct_d, i);
        cUIText* timeText = (cUIText*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(buf));
        if (full == 1) {
            sprintf(line, func_0020A1A8(func_00155130(stats, i - 1)));
        } else {
            sprintf(line, D_004A2138, func_00155130(stats, i - 1));
        }
        cUIText_setAsciiString_i(timeText, line);
    }
    func_00294F78(func_0028B180(), 0xE);
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
 * @param w  the widget being created
 */
extern "C" void cOVState_TOPTIMES_onWidgetCreate(void* self, cUIText* w)
{
    int id = *(int*)((char*)w + 0x38);
    if (isHashTOPTIMES(id, str_continue)) {
        if (cOVStateManager_mMiscData == 0) {
            cUIText_setUnicodeStringByID(w, GetHashValue32(str_kT_CMNContinue_b));
        } else {
            cUIText_setUnicodeStringByID(w, GetHashValue32(str_kT_OVRCMNReturn));
        }
        *(int*)((char*)w + 0x18) = 0;
    } else if (isHashTOPTIMES(id, str_saverecords)) {
        *(int*)((char*)w + 0x18) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onGainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIState_onGainFocus(void* self, void* a1);
extern "C" void cOVState_triggerTextFade(void* self, const char* name, const char* ext);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern signed char D_00535C11[];
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
extern "C" void cOVState_TOPTIMES_onGainFocus(void* self, void* pPrevState)
{
    cUIState_onGainFocus(self, pPrevState);
    char* prof = (char*)cBE_getInterface_Fv(cBE_getBE(), 8);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int id = *(int*)(prof + 0x18);
    if (id >= 0) {
        LoadRider1F7F08(self, id + 1);
    }
    if (D_00535C11[0] == 2) {
        int id2 = *(int*)(prof + 0x1C);
        if (id2 >= 0) {
            LoadRider1F7F08(self, id2 + 1);
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
 * @return 0x101 for event 6, else 0
 */
extern "C" int cOVState_TOPTIMES_onWidgetMenu(void* self, int pMenu, int Event)
{
    return Event == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry_001F80D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * On Cross, passes the chosen button to the panel's virtual slot 26 (func_001F8118: continue or save
 * records).
 *
 * The C call passes only self, while func_001F8118 takes the item as its second argument: it gets @p pWidget
 * only because the argument register still holds it, so a port must pass it explicitly.
 *
 * @param pWidget  the chosen item; ignored when null
 * @param Event  the widget event; only 5 (Cross) acts
 */
extern "C" void cOVState_TOPTIMES_onWidgetEvent(void* self, void* pWidget, int Event)
{
    if (pWidget != 0) {
        if (Event == 5) {
            sVEntry_001F80D8* vt = *(sVEntry_001F80D8**)((char*)self + 0x8);
            vt[26].fn((char*)self + vt[26].delta);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F8118);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern "C" void cOVStateManager_triggerOverlay(int id);

extern "C" void func_001F8118(void* self, void* a1)
{
    if (*(int*)((char*)self + 0xA0) == 0) {
        int v = *(int*)((char*)a1 + 0x18);
        switch (v) {
        case 0:
            *(int*)((char*)self + 0xA0) = 1;
            cOVStateManager_mStatus = 1;
            break;
        case 1:
            *(int*)((char*)self + 0xA0) = v;
            cOVStateManager_triggerOverlay(0x13);
            break;
        }
    }
}
#endif

