#include "common.h"
#include "ui/uithing.h"
#include "be/beintnewrace.h"
#include "fe/ovtemplatedialog.h"
#include "fe/ovstatereplay.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cPDATemplate_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...)
extern "C" void sprintf1_v(void* dst, int fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern char str_PDATemplate[];
extern char str_bganim1[];
extern char str_temperature[];

struct sVEntry_PDA {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_PDA(int hash)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_PDA* vt = *(sVEntry_PDA**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, hash);
}

static inline int div_PDA(int a, int b)
{
    return a / b;
}

/**
 * Creates the in-game PDA's screens ("PDATemplate" and the looping "bganim1") and fills in its temperature
 * readout.
 *
 * The temperature is -10 C minus (app update ticks / 3600) mod 15, so it drops by one degree a minute (at 60
 * updates a second) and wraps from -24 back to -10.
 */
extern "C" void cPDATemplate_onCreateScreen(cPDATemplate* self)
{
    self->mHashName = 41;
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_PDATemplate), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)self->mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_bganim1), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 1);
    }
    char* text = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_temperature));
    if (text != 0) {
        int fmt = Loc_PDA(*(int*)(text + 0xB0));
        if (fmt != 0) {
            char buf[48];
            sprintf1_v(buf, fmt, -10 - div_PDA(SSXApp->Tick, 3600) % 15, 67);
            cUIText_setUnicodeString(text, buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_getResultsOverlay);
#ifdef SKIP_ASM
/**
 * Picks which results overlay to show for a game event and heat.
 *
 * Events 6 to 11 give 0x16; events 0, 4 and 5, or heat 3, give 0xB; anything else gives 0xA.
 *
 * @param currentGameEvent  the current game event
 * @param currentHeat  the current heat
 * @return the overlay id (On Tour tOVStateEnum)
 */
extern "C" int cOVStateManager_getResultsOverlay(int currentGameEvent, int currentHeat)
{
    if ((unsigned int)(currentGameEvent - 6) < 6) {
        return 22;
    }
    if ((unsigned int)(currentGameEvent - 4) < 2 || currentGameEvent == 0 || currentHeat == 3) {
        return 11;
    }
    return 10;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_gotoOverlay);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVStateManager_triggerOverlay(int state);
int cBERewardInterface_getTotalNumUserAwards(void* self);
extern "C" void cReplay_startAutoReplay(void* p);
extern int cOVStateManager_mStatus;
extern int D_004A2704;
extern int cOVStateManager_mMiscData;
extern int cBESaveInterface_bDoReplay;

static inline bool isKind_A8F8(int k, int v)
{
    return k == v;
}

/**
 * Opens the overlay that belongs to a game-flow state: the pre-event screen or the post-race results.
 *
 * Does nothing while a replay (other than state 10+) is playing. For state 2 (pre-event) picks the replay (0x11),
 * event intro (8), team (0x15), rival (0x14) or normal (9) overlay; for state 7 (post-event) picks a network,
 * results (cOVStateManager_getResultsOverlay), reward (0x10) or 0xF overlay, starts the auto replay for non-rival events and, online,
 * sets cGameComm_mGameComm's overlay timeout to 0x8CA0. With no overlay chosen, sets
 * cOVStateManager_mStatus to 2 instead of calling cOVStateManager_triggerOverlay.
 *
 * @param mode  the game-flow state (On Tour eGFState)
 */
extern "C" void cOVStateManager_gotoOverlay(int mode)
{
    int replayMode = *(int*)SSXApp->Game->Replay;
    int busy = replayMode != 0 && replayMode < 10;
    if (busy) return;
    int state = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* newRace = (signed char*)&cBENewRaceInterface_newRace;
    int type = *(int*)SSXApp->GameModeMan;
    int sub = newRace[0x48];
    int team = newRace[0x49];
    int kind = newRace[0x4A];
    switch (mode) {
    case 2:
        D_004A2704 = 0;
        if (cBESaveInterface_bDoReplay != 0) {
            state = 17;
        } else if (sub == 4) {
            cOVStateManager_mStatus = mode;
        } else {
            cOVStateManager_mStatus = 0;
            if (kind == 0) {
                state = 8;
            } else if (team == mode) {
                state = 21;
            } else {
                if (kind >= 4 && kind <= 11) state = 20;
                else state = 9;
            }
        }
        break;
    case 7:
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (sub == 4) {
            cOVStateManager_mStatus = sub;
            return;
        }
        cOVStateManager_mStatus = 0;
        char* scoreIface = (char*)cBE_getInterface_Fv(cBE_getBE(), 8);
        char* rewardIface = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
        int any = 0;
        if (*(int*)(scoreIface + 0x18) >= 0 || *(int*)(scoreIface + 0x1C) >= 0) any = 1;
        if (any) {
            if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                state = cOVStateManager_getResultsOverlay(kind, type);
            } else {
                cOVStateManager_mMiscData = 0;
                state = 0xF;
            }
        } else {
            if (cBERewardInterface_getTotalNumUserAwards(rewardIface) > 0 || *(int*)(rewardIface + 0x10) != 0) {
                state = 0x10;
            } else {
                state = cOVStateManager_getResultsOverlay(kind, type);
            }
        }
        int z = !isKind_A8F8(kind, 6) && !isKind_A8F8(kind, 7) && !isKind_A8F8(kind, 8) && !isKind_A8F8(kind, 9) &&
                !isKind_A8F8(kind, 10) && !isKind_A8F8(kind, 11);
        if (z) cReplay_startAutoReplay(SSXApp->Game->Replay);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) cGameComm_mGameComm->mOverlayTimeout = 36000;
        break;
    }
    if (state == 0) {
        cOVStateManager_mStatus = 2;
    } else {
        cOVStateManager_triggerOverlay(state);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_triggerOverlay);
#ifdef SKIP_ASM
// Dialog/state factory: builds the UI state for `type` (the cases inline the state constructors over
// the cUIState base ctor cUIState_cUIState) and pushes it on the front-end state stack.
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
int GetHashValue32(char* str);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern "C" void* cUIState_cUIState(void* self, void* mgr);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void* cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(void* self, void* mgr, int type);
extern "C" void* cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(void* self, void* mgr, int type);
extern "C" void* cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(void* self, void* mgr, int type);
extern "C" void* cOVState_PROFILE_cOVState_PROFILE(void* self, void* mgr, int type, int a3);
extern "C" void* cOVState_AUTOSAVE_cOVState_AUTOSAVE(void* self, void* mgr, int type);
extern "C" void cOVState_BIGCHALLENGE_START_getChallengeData(void* self);
extern "C" void cOVState_BIGCHALLENGE_FAIL_getChallengeData(void* self);
extern "C" void cUITemplate_MAP_init(void* self);
struct sDlgSlot_AB50 {
    int v[2];
    sDlgSlot_AB50() {}
};

inline void* operator new[](unsigned int, void* p) { return p; }
extern unsigned short str_sp_b[];
extern char str_cOVStateOverlay[];
extern char str_finishov[];
extern char cOVState_REWARDS_vtbl[];
extern char D_004725C8[];
extern char D_004726A0[];
extern char D_00472778[];
extern char cOVState_HEAT_PREEVENT_vtbl[];
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_PAUSE_OPTIONS_vtbl[];
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char D_00473248[];
extern char cOVState_MAP_vtbl[];
extern char D_004733F8[];
extern char cOVState_TOPTIMES_vtbl[];
extern char D_004735B0[];
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_ENTERLODGE_vtbl[];
extern char D_00473C50[];
extern char cOVState_BASSSP_QUAL_FINISHOV_vtbl[];
extern char cOVState_RACE_FINISHOV_vtbl[];
extern char D_00473ED8[];
extern char cOVState_FINAL_STANDINGS_vtbl[];
extern char cOVState_HEAT_STANDINGS_vtbl[];
extern char D_004744C0[];
extern char D_00474598[];
extern char D_00474670[];
extern char D_00474748[];
extern char D_00474820[];
extern char D_004748F8[];
extern char D_004749D0[];
extern char cOVState_REPLAY_vtbl[];
extern char D_00474B80[];
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);

/**
 * Creates the in-game overlay state of the given type and pushes it onto the game UI's state stack.
 *
 * Each type 1-0x28 builds its own cOVState subclass ("cOVStateOverlay"). There is no default case: any other
 * type
 * leaves the result null and the flag write after the chain dereferences it.
 *
 * @param type  the overlay (On Tour tOVStateEnum overlay)
 */
extern "C" void cOVStateManager_triggerOverlay(int type)
{
    char* result = 0;
    char* mgr = (char*)SSXApp->Game->UI;
    if (type == 0x1) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_COMPETE_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x2) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_BIGCHAL_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x3) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_FREERIDE_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x4) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_SPOKE_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x5) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_QUICKPLAY_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x6) {
        char* overlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_ONLINE_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC4) = 0;
        result = overlay;
    }
    if (type == 0x7) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_OPTIONS_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0x8) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_HEAT_PREEVENT_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0x9) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00472778;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 10) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_HEAT_STANDINGS_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 11) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_FINAL_STANDINGS_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0xC) {
        char* overlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(void**)(overlay + 0x8) = cOVState_RACE_FINISHOV_vtbl;
        *(int*)(overlay + 0xA0) = 0x1;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC) = GetHashValue32(str_finishov);
        result = overlay;
    }
    if (type == 13) {
        char* overlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(void**)(overlay + 0x8) = cOVState_BASSSP_QUAL_FINISHOV_vtbl;
        *(int*)(overlay + 0xA0) = 0x1;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0xC) = GetHashValue32(str_finishov);
        result = overlay;
    }
    if (type == 0xE) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00474B80;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0xF) {
        char* overlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_TOPTIMES_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0x10) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_REWARDS_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 17) {
        char* overlay = (char*)operator new(184, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_REPLAY_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 18) {
        result = (char*)cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 18);
    }
    if (type == 19) {
        result = (char*)cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 19);
    }
    if (type == 20) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004725C8;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 21) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004726A0;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 22) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00473ED8;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 23) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004749D0;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0x9C) = 0;
        result = overlay;
    }
    if (type == 0x18) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004748F8;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x1;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 25) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00474820;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x2;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 26) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00474748;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x3;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 27) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00474670;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x4;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0x1C) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00474598;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x5;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 29) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_BIGCHALLENGE_START_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        cOVState_BIGCHALLENGE_START_getChallengeData(overlay);
        result = overlay;
    }
    if (type == 0x1E) {
        char* overlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004735B0;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        cOVState_BIGCHALLENGE_FAIL_getChallengeData(overlay);
        result = overlay;
    }
    if (type == 0x1F) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_ENTERLODGE_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 0x20) {
        char* overlay = (char*)operator new(468, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(void**)(overlay + 0x8) = cOVState_MAP_vtbl;
        new (overlay + 0x9C) sDlgSlot_AB50[8];
        result = overlay;
    }
    if (type == 33) {
        char* overlay = (char*)operator new(912, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004733F8;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        cUITemplate_MAP_init(overlay + 0x9C);
        result = overlay;
    }
    if (type == 34) {
        result = (char*)cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(operator new(0x180, str_cOVStateOverlay, 0, 0), mgr, 34);
    }
    if (type == 35) {
        char* overlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_00473C50;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 36) {
        result = (char*)cOVState_PROFILE_cOVState_PROFILE(operator new(636, str_cOVStateOverlay, 0, 0), mgr, 36, 42);
    }
    if (type == 37) {
        result = (char*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(operator new(564, str_cOVStateOverlay, 0, 0), mgr, 37);
    }
    if (type == 38) {
        char* overlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = D_004744C0;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0x9C) = 0x6;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }
    if (type == 39) {
        char* overlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(void**)(overlay + 0x8) = cOVState_PAUSE_ONLINE_ERROR_vtbl;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(int*)(overlay + 0xA0) = 0x1;
        *(int*)(overlay + 0x98) = type;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        *(int*)(overlay + 0x9C) = 0;
        result = overlay;
    }
    if (type == 40) {
        char* overlay = (char*)operator new(228, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(overlay, mgr);
        *(int*)(overlay + 0x98) = type;
        *(unsigned short*)(overlay + 0x58) = str_sp_b[0];
        *(void**)(overlay + 0x8) = D_00473248;
        *(int*)(overlay + 0x48) = 0;
        *(int*)(overlay + 0x4C) = 0;
        *(int*)(overlay + 0x54) = 0;
        result = overlay;
    }

    *(int*)(result + 0x1C) = (*(int*)(result + 0x1C) & ~0x3F00) | 0x100;
    cUIStateStack_pushSpecial(mgr + 0x18, result, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_triggerPopup);
#ifdef SKIP_ASM
// Dialog/state factory: builds the UI state for `type` (the cases inline the state constructors over
// the cUIState base ctor cUIState_cUIState) and pushes it on the front-end state stack.
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
int GetHashValue32(char* str);
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern "C" void* cUIState_cUIState(void* self, void* mgr);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void* cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(void* self, void* mgr, int type);
extern "C" void* cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(void* self, void* mgr, int type);
extern "C" void* cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(void* self, void* mgr, int type);
extern "C" void* cOVState_PROFILE_cOVState_PROFILE(void* self, void* mgr, int type, int a3);
extern "C" void* cOVState_AUTOSAVE_cOVState_AUTOSAVE(void* self, void* mgr, int type);
extern "C" void cOVState_BIGCHALLENGE_START_getChallengeData(void* self);
extern "C" void cOVState_BIGCHALLENGE_FAIL_getChallengeData(void* self);
extern "C" void cUITemplate_MAP_init(void* self);
struct sDlgSlot_BAB8 {
    int v[2];
    sDlgSlot_BAB8() {}
};

extern unsigned short str_sp_b[];
extern char str_cOVStateOverlay[];
extern char str_finishov[];
extern char cOVState_REWARDS_vtbl[];
extern char D_004725C8[];
extern char D_004726A0[];
extern char D_00472778[];
extern char cOVState_HEAT_PREEVENT_vtbl[];
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_PAUSE_OPTIONS_vtbl[];
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char D_00473248[];
extern char cOVState_MAP_vtbl[];
extern char D_004733F8[];
extern char cOVState_TOPTIMES_vtbl[];
extern char D_004735B0[];
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_ENTERLODGE_vtbl[];
extern char D_00473C50[];
extern char cOVState_BASSSP_QUAL_FINISHOV_vtbl[];
extern char cOVState_RACE_FINISHOV_vtbl[];
extern char D_00473ED8[];
extern char cOVState_FINAL_STANDINGS_vtbl[];
extern char cOVState_HEAT_STANDINGS_vtbl[];
extern char D_004744C0[];
extern char D_00474598[];
extern char D_00474670[];
extern char D_00474748[];
extern char D_00474820[];
extern char D_004748F8[];
extern char D_004749D0[];
extern char cOVState_REPLAY_vtbl[];
extern char D_00474B80[];

/**
 * Creates the in-game overlay state of the given type and pushes it as a popup over the current UI state.
 *
 * Builds the same overlays as cOVStateManager_triggerOverlay, case by case for types 1-40: each is allocated as
 * "cOVStateOverlay" with its own size, gets the cUIState base constructor (cUIState_cUIState), its vtable, mOVID = @p type
 * and the overlay defaults. Types 18, 19, 34, 36 and 37 use their own constructors (cOVState_SAVEREPLAY_cOVState_SAVEREPLAY, cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS,
 * cOVState_MESSAGECENTER_cOVState_MESSAGECENTER, cOVState_PROFILE_cOVState_PROFILE with 42,
 * cOVState_AUTOSAVE_cOVState_AUTOSAVE), 29 and 30 load their challenge data (cOVState_BIGCHALLENGE_START_getChallengeData, cOVState_BIGCHALLENGE_FAIL_getChallengeData) and 33
 * initialises its map template at +0x9C (cUITemplate_MAP_init). Unlike cOVStateManager_triggerOverlay it records the state
 * that was current (cUIStateStack_getCurrentState) as the new state's mpParentState and pushes it with
 * cUIStateStack_pushPopup on the engine's state stack.
 *
 * There is no default case: any other type leaves the result null and the mpParentState write dereferences it (likely
 * bug, shared with cOVStateManager_triggerOverlay). Nothing in the program calls it.
 *
 * @param type  the overlay (On Tour tOVStateEnum overlay)
 */
extern "C" void cOVStateManager_triggerPopup(int type)
{
    char* result = 0;
    char* mgr = (char*)SSXApp->Game->UI;
    int cur = (int)cUIStateStack_getCurrentState(mgr + 0x18);
    if (type == 0x1) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_COMPETE_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x2) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_BIGCHAL_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x3) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_FREERIDE_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x4) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_SPOKE_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x5) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_QUICKPLAY_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x6) {
        char* newOverlay = (char*)operator new(276, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_ONLINE_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC4) = 0;
        result = newOverlay;
    }
    if (type == 0x7) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_OPTIONS_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0x8) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_HEAT_PREEVENT_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0x9) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00472778;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 10) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_HEAT_STANDINGS_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 11) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_FINAL_STANDINGS_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0xC) {
        char* newOverlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(void**)(newOverlay + 0x8) = cOVState_RACE_FINISHOV_vtbl;
        *(int*)(newOverlay + 0xA0) = 0x1;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC) = GetHashValue32(str_finishov);
        result = newOverlay;
    }
    if (type == 13) {
        char* newOverlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(void**)(newOverlay + 0x8) = cOVState_BASSSP_QUAL_FINISHOV_vtbl;
        *(int*)(newOverlay + 0xA0) = 0x1;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0xC) = GetHashValue32(str_finishov);
        result = newOverlay;
    }
    if (type == 0xE) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00474B80;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0xF) {
        char* newOverlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_TOPTIMES_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0x10) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_REWARDS_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 17) {
        char* newOverlay = (char*)operator new(184, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_REPLAY_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 18) {
        result = (char*)cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 18);
    }
    if (type == 19) {
        result = (char*)cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 19);
    }
    if (type == 20) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004725C8;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 21) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004726A0;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 22) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00473ED8;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 23) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004749D0;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0x9C) = 0;
        result = newOverlay;
    }
    if (type == 0x18) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004748F8;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x1;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 25) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00474820;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x2;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 26) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00474748;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x3;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 27) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00474670;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x4;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0x1C) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00474598;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x5;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 29) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_BIGCHALLENGE_START_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        cOVState_BIGCHALLENGE_START_getChallengeData(newOverlay);
        result = newOverlay;
    }
    if (type == 0x1E) {
        char* newOverlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004735B0;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        cOVState_BIGCHALLENGE_FAIL_getChallengeData(newOverlay);
        result = newOverlay;
    }
    if (type == 0x1F) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_ENTERLODGE_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 0x20) {
        char* newOverlay = (char*)operator new(468, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(void**)(newOverlay + 0x8) = cOVState_MAP_vtbl;
        new (newOverlay + 0x9C) sDlgSlot_BAB8[8];
        result = newOverlay;
    }
    if (type == 33) {
        char* newOverlay = (char*)operator new(912, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004733F8;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        cUITemplate_MAP_init(newOverlay + 0x9C);
        result = newOverlay;
    }
    if (type == 34) {
        result = (char*)cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(operator new(0x180, str_cOVStateOverlay, 0, 0), mgr, 34);
    }
    if (type == 35) {
        char* newOverlay = (char*)operator new(156, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_00473C50;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 36) {
        result = (char*)cOVState_PROFILE_cOVState_PROFILE(operator new(636, str_cOVStateOverlay, 0, 0), mgr, 36, 42);
    }
    if (type == 37) {
        result = (char*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(operator new(564, str_cOVStateOverlay, 0, 0), mgr, 37);
    }
    if (type == 38) {
        char* newOverlay = (char*)operator new(160, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = D_004744C0;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0x9C) = 0x6;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }
    if (type == 39) {
        char* newOverlay = (char*)operator new(164, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(void**)(newOverlay + 0x8) = cOVState_PAUSE_ONLINE_ERROR_vtbl;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(int*)(newOverlay + 0xA0) = 0x1;
        *(int*)(newOverlay + 0x98) = type;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        *(int*)(newOverlay + 0x9C) = 0;
        result = newOverlay;
    }
    if (type == 40) {
        char* newOverlay = (char*)operator new(228, str_cOVStateOverlay, 0, 0);
        cUIState_cUIState(newOverlay, mgr);
        *(int*)(newOverlay + 0x98) = type;
        *(unsigned short*)(newOverlay + 0x58) = str_sp_b[0];
        *(void**)(newOverlay + 0x8) = D_00473248;
        *(int*)(newOverlay + 0x48) = 0;
        *(int*)(newOverlay + 0x4C) = 0;
        *(int*)(newOverlay + 0x54) = 0;
        result = newOverlay;
    }

    *(int*)(result + 0x20) = cur;
    cUIStateStack_pushPopup(mgr + 0x18, result);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020CA10);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVStateManager_triggerOverlay(int state);
extern "C" void cGameComm_sendUnplugCmd(void* net);
extern "C" void cGameComm_sendPauseCmd(void* net, int a1);
extern "C" int cWScriptMan_isLocalMissionActive(void* self);
extern void* cWScriptMan_TheWScriptMan;
extern char cOVTemplate_PauseMenu_mSelectedMenuIndex;

static inline bool notMode4_0020CA10(signed char* g)
{
    return g[0x48] != 4;
}

/**
 * Pauses the game by opening the pause overlay that fits the current game.
 *
 * Online (cBENetworkInterface_networkOpt.bIsNetworkGame) it first, when cGameComm_mGameComm exists, sends the
 * controller-unplugged command (cGameComm_sendUnplugCmd) if SSXApp->Game's ControllerUnplugged is set, sends the pause command
 * (cGameComm_sendPauseCmd with 1) and starts mPauseTimeout = 3600 and mOverlayTimeout = 1800; then triggers overlay 6.
 * Offline: overlay 2 while a local mission is active (cWScriptMan_isLocalMissionActive), else overlay 1 on game path 0
 * outside cGameMode 4, overlay 4 in game state 11 (mCurrentStateID), overlay 3 in cGameMode 4 and overlay 5 otherwise
 * (cGameMode and cGamePath of cBENewRaceInterface_newRace; the overlays through cOVStateManager_triggerOverlay).
 * Finally clears cOVTemplate_PauseMenu_mSelectedMenuIndex, the pause menu's remembered selection. The results of the two cBE_getInterface__Fv calls
 * (interfaces 0 and 7) are not used.
 *
 * cGame_update and cGame_updateUI call it after cOVStateManager_allowPauseMenu allows the pause menu.
 */
extern "C" void func_0020CA10(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cGameComm* net = cGameComm_mGameComm;
        if (net != 0) {
            cGame* game = SSXApp->Game;
            if (((char*)game) != 0 && game->ControllerUnplugged != 0) {
                cGameComm_sendUnplugCmd((char*)net);
            }
            cGameComm_sendPauseCmd((char*)cGameComm_mGameComm, 1);
            cGameComm* n = cGameComm_mGameComm;
            n->mPauseTimeout = 3600;
            n->mOverlayTimeout = 1800;
        }
        cOVStateManager_triggerOverlay(6);
    } else if (cWScriptMan_isLocalMissionActive(cWScriptMan_TheWScriptMan) != 0) {
        cOVStateManager_triggerOverlay(2);
    } else {
        signed char* raceDb = (signed char*)&cBENewRaceInterface_newRace;
        if (raceDb[0x49] == 0 && notMode4_0020CA10(raceDb)) {
            cOVStateManager_triggerOverlay(1);
        } else if (SSXApp->Game->mCurrentStateID == 11) {
            cOVStateManager_triggerOverlay(4);
        } else {
            signed char* modeRaceDb = (signed char*)&cBENewRaceInterface_newRace;
            if (modeRaceDb[0x48] == 4) {
                cOVStateManager_triggerOverlay(3);
            } else {
                cOVStateManager_triggerOverlay(5);
            }
        }
    }
    cOVTemplate_PauseMenu_mSelectedMenuIndex = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020CB58);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int cUIStateStack_findState(void* stack, int hash);
extern char str_64replay[];

/**
 * Tells whether a "64replay" state is on the game UI's state stack.
 *
 * @return 1 when cUIStateStack_findState finds the hash of "64replay" on SSXApp->Game->UI's stack, else 0
 */
extern "C" int func_0020CB58(void)
{
    char* stack = (char*)SSXApp->Game->UI + 0x18;
    return cUIStateStack_findState(stack, GetHashValue32(str_64replay)) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_pauseOverlayActive);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int cUIStateStack_findState(void* stack, int hash);
extern char str_cOVTemplate_PauseMenu[];

/**
 * Tells whether the pause menu is open: whether a "cOVTemplate_PauseMenu" state is on the game UI's state stack.
 *
 * cGame_update checks it before updating the playback manager, and cOVStateManager_allowPauseMenu refuses the pause menu while it is
 * open.
 *
 * @return 1 when cUIStateStack_findState finds the hash of "cOVTemplate_PauseMenu", else 0
 */
extern "C" int cOVStateManager_pauseOverlayActive(void)
{
    char* stack = (char*)SSXApp->Game->UI + 0x18;
    return cUIStateStack_findState(stack, GetHashValue32(str_cOVTemplate_PauseMenu)) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_allowPauseMenu);
#ifdef SKIP_ASM
class cUIState20CBE8 {
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
    virtual void v09();
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
    virtual int allowPauseMenu();
};
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern "C" int cOVStateManager_pauseOverlayActive(void);

/**
 * Tells whether the pause menu may be opened now.
 *
 * No while the pause menu is already open (cOVStateManager_pauseOverlayActive). Otherwise asks the game UI's current state
 * (cUIStateStack_getCurrentState): with no state the answer is yes; a popup state (mAttrState bit 0, On Tour mIsPopup)
 * says no; any other state answers through its virtual slot 25 (cOVState_allowPauseMenu in the overlay vtables).
 *
 * @return nonzero when the pause menu may open
 */
extern "C" int cOVStateManager_allowPauseMenu(void)
{
    char* mgr = (char*)SSXApp->Game->UI;
    if (cOVStateManager_pauseOverlayActive() == 0) {
        cUIState20CBE8* topState = (cUIState20CBE8*)cUIStateStack_getCurrentState(mgr + 0x18);
        if (topState != 0) {
            if (*(int*)((char*)topState + 0x1C) & 1) {
                return 0;
            }
            return topState->allowPauseMenu();
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_triggerFinishOverlay);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVStateManager_triggerOverlay(int state);
extern int cOVStateManager_mStatus;

/**
 * Opens the overlay that follows a finished event, or lets play resume when there is none.
 *
 * By cBENewRaceInterface_newRace's cGameMode and cGamePath: cGameMode 0 or game path 2 (online) trigger overlay 0xC;
 * cGameMode 1-3 and 6 overlay 13 (cOVStateManager_triggerOverlay; both are the "finishov" overlays); cGameMode 4 sets
 * cOVStateManager_mStatus to 2 so the next cOVStateManager_update resumes play; any other mode overlay 0xC. The result
 * of cBE_getInterface__Fv (interface 0) is not used. cGFFinishlineState_update (load screen) calls it.
 */
extern "C" void cOVStateManager_triggerFinishOverlay(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* newRace = (signed char*)&cBENewRaceInterface_newRace;
    int mode = newRace[0x48];
    int players = newRace[0x49];
    if (mode == 0 || players == 2) {
        cOVStateManager_triggerOverlay(0xC);
    } else if ((mode >= 1 && mode <= 3) || mode == 6) {
        cOVStateManager_triggerOverlay(13);
    } else if (mode == 4) {
        cOVStateManager_mStatus = 2;
    } else {
        cOVStateManager_triggerOverlay(0xC);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_gotoNextOverlay);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void cReplay_stopAutoReplay(void* self);
extern "C" void cGame_setState(void* self, int a, int b, int refresh);
extern "C" void cGameComm_localDisconnected(void* self, int a1);
extern "C" void cGame_exit(void* game, int a);
extern "C" void cReplay_startReplay(void* self, void* a1, int a2, int a3);
extern unsigned int cUITemplate_MAP_mMapStyle;
extern int cOVStateManager_mIsPauseDialog;



/**
 * Moves on from the overlay that just closed to the next one (or back to the game).
 *
 * Clears cOVStateManager_mStatus and switches on the current UI state's overlay id (+0x98): after results
 * (10, 11, 22) acts on cOVStateManager_mMiscData (open the map with style 1, restart via cGame_setState, show a
 * replay, leave an online game with cGameComm_localDisconnected and cGame_exit...); after 15 and 16 shows rewards or the
 * results overlay (cOVStateManager_getResultsOverlay); after 8, 9, 20 and 21 sets mStatus to 2 so the next update resumes play.
 */
// Continues after a result dialog closes, depending on the current overlay state and the pending dialog.
extern "C" void cOVStateManager_gotoNextOverlay(void)
{
    cOVStateManager_mStatus = 0;
    char* st = (char*)cUIStateStack_getCurrentState(((char*)((SSXApp)->Game)->UI) + 0x18);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int kind = cBENewRaceInterface_newRace.cGameEvent;
    int type = *(int*)SSXApp->GameModeMan;
    switch (*(int*)(st + 0x98)) {
    case 10:
    case 11:
        switch (cOVStateManager_mMiscData) {
        case 3:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cUITemplate_MAP_mMapStyle = 1;
            cOVStateManager_triggerOverlay(33);
            break;
        case 1:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cGame_setState(SSXApp->Game, 13, 0, 1);
            break;
        case 2:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_mStatus = 4;
            break;
        case 4:
            cOVStateManager_triggerOverlay(35);
            break;
        case 5:
            cOVStateManager_mIsPauseDialog = 0;
            cOVStateManager_triggerOverlay(23);
            break;
        case 6:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_triggerOverlay(17);
            cReplay_startReplay(SSXApp->Game->Replay, 0, 0, 0);
            break;
        case 7:
            cOVStateManager_mMiscData = 1;
            cOVStateManager_triggerOverlay(0xF);
            break;
        case 8:
            cBE_getInterface_Fv(SSXApp->BE, 7);
            if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                if (cGameComm_mGameComm != 0) {
                    cGameComm_localDisconnected((char*)cGameComm_mGameComm, 0);
                }
                cGame_exit(SSXApp->Game, 0);
            } else {
                cOVStateManager_triggerOverlay(0x18);
            }
            break;
        }
        break;
    case 15:
        if (cOVStateManager_mMiscData == 19) {
            cOVStateManager_triggerOverlay(19);
        } else {
            char* rewardIface = (char*)cBE_getInterface_Fv(cBE_getBE(), 13);
            if (cBERewardInterface_getTotalNumUserAwards(rewardIface) > 0 || *(int*)(rewardIface + 0x10) != 0) {
                cOVStateManager_triggerOverlay(0x10);
            } else {
                cOVStateManager_triggerOverlay(cOVStateManager_getResultsOverlay(kind, type));
            }
        }
        break;
    case 16:
        cOVStateManager_triggerOverlay(cOVStateManager_getResultsOverlay(kind, type));
        break;
    case 8:
    case 9:
    case 20:
    case 21:
        cOVStateManager_mStatus = 2;
        break;
    case 22:
        switch (cOVStateManager_mMiscData) {
        case 2:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_mStatus = 4;
            break;
        case 5:
            cOVStateManager_mIsPauseDialog = 0;
            cOVStateManager_triggerOverlay(23);
            break;
        case 8:
            cOVStateManager_triggerOverlay(0x18);
            break;
        }
        break;
    case 35:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_update);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void cUIStateStack_purgeNormalStates(void *);

struct sFlag_CFD0 {
    int f0;
    int f4;
    int f8;
};



/**
 * Per-frame overlay manager step: carries out the pending overlay action in cOVStateManager_mStatus.
 *
 * 1 goes to the next overlay (cOVStateManager_gotoNextOverlay); 2 stops the auto replay, flags the current game state to continue
 * and, in an online race's pre-event state, purges the UI states (cUIStateStack_purgeNormalStates); 3 removes the PDA template and
 * purges the UI states; 4 sets game state 0xE via cGame_setState. Each case clears mStatus first.
 */
extern "C" void cOVStateManager_update(void)
{
    char *mgr = ((char*)((SSXApp)->Game)->UI);
    char *st = 0;
    if (mgr + 0x18) {
        st = (char *)cUIStateStack_getCurrentState(mgr + 0x18);
    }
    switch (cOVStateManager_mStatus) {
    case 0:
        break;
    case 1:
        cOVStateManager_gotoNextOverlay();
        break;
    case 2:
        cOVStateManager_mStatus = 0;
        cReplay_stopAutoReplay(((void*)((SSXApp)->Game)->Replay));
        ((sFlag_CFD0*)((SSXApp)->Game)->mCurrentState)->f8 = 1;
        if (st != 0 && ((SSXApp)->Game)->mCurrentStateID == 2) {
            if (*(int *)(st + 0x98) == 8) {
                cBE_getInterface_Fv(((void*)(SSXApp)->BE), 7);
                if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                    cUIStateStack_purgeNormalStates(mgr + 0x18);
                }
            }
        }
        break;
    case 3:
        cOVStateManager_mStatus = 0;
        cOVStateManager_removePDATemplate();
        cUIStateStack_purgeNormalStates(((char*)((SSXApp)->Game)->UI) + 0x18);
        break;
    case 4:
        cOVStateManager_mStatus = 0;
        cGame_setState(((SSXApp)->Game), 0xE, 2, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_addPDATemplate);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIState_cUIState(void* self, void* mgr);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern void* cOVStateManager_mpPDATemplate;
extern char str_PDATemplate[];
extern char cPDATemplate_vtbl[];

/**
 * Pushes the PDA template state ("PDATemplate") onto the game UI's stack, once; the instance is kept in
 * cOVStateManager_mpPDATemplate.
 */
extern "C" void cOVStateManager_addPDATemplate(void)
{
    if (cOVStateManager_mpPDATemplate == 0) {
        char* mgr = (char*)SSXApp->Game->UI;
        void* pdaTemplate = cMemMan_alloc(72, str_PDATemplate, 0x100, 0);
        cUIState_cUIState(pdaTemplate, mgr);
        *(void**)((char*)pdaTemplate + 8) = cPDATemplate_vtbl;
        cOVStateManager_mpPDATemplate = pdaTemplate;
        cUIStateStack_pushExplicit(mgr + 0x18, pdaTemplate);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_removePDATemplate);
#ifdef SKIP_ASM
class cOVTemplate20D190 {
public:
    int pad[2];
    virtual ~cOVTemplate20D190();
};
extern "C" void cListNode_removeFromList(void* node);
extern void* cOVStateManager_mpPDATemplate;

/**
 * Removes and deletes the overlay manager's PDA template, if there is one.
 *
 * Unlinks cOVStateManager_mpPDATemplate from its list and deletes it through its virtual destructor; the pointer
 * is cleared either way.
 */
extern "C" void cOVStateManager_removePDATemplate(void)
{
    if (cOVStateManager_mpPDATemplate != 0) {
        cListNode_removeFromList(cOVStateManager_mpPDATemplate);
        delete (cOVTemplate20D190*)cOVStateManager_mpPDATemplate;
    }
    cOVStateManager_mpPDATemplate = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Simple_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A2860[2];
extern char* D_00441D10[];
extern char str_overlayMenu[];
extern char str_overlayOptpct_d[];

class cUIObj_20D1D8 {
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
};

/**
 * Builds the screen of a simple overlay: the base overlay screen plus a menu with one visible option.
 *
 * Runs cOVState_onCreateScreen, sets flag 8 in the "overlayMenu" widget's word at +0x90, then walks "overlayOpt0" ...
 * "overlayOpt4" of mpScreen: option 0 gets the localised string named D_00441D10[D_004A2860[0]] and that index as its
 * misc data (+0x18); options 1-4 are hidden (setVisible(0), slot 9) and made static (setStatic(1), slot 8). It fills the
 * onCreateScreen slot (+0x24) of D_00474D30, the vtable that also holds cOVTemplate_Simple_onWidgetMenu and cOVTemplate_Simple_onWidgetEvent; func_001FBD20
 * and cOVState_HEAT_PREEVENT_onCreateScreen call it as their base.
 */
extern "C" void cOVTemplate_Simple_onCreateScreen(cOVTemplate_Simple* self)
{
    cOVState_onCreateScreen(self);
    char* list = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_overlayMenu));
    *(int*)(list + 0x90) |= 8;
    for (int i = 0; i < 5; i++) {
        char buf[32];
        sprintf(buf, str_overlayOptpct_d, i);
        cUIObj_20D1D8* optionWidget = (cUIObj_20D1D8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf));
        if (optionWidget != 0) {
            if (i < 1) {
                int labelIndex = D_004A2860[i];
                cUIText_setUnicodeStringByID((cUIText*)optionWidget, GetHashValue32(D_00441D10[labelIndex]));
                *(int*)((char*)optionWidget + 0x18) = labelIndex;
            } else {
                optionWidget->setVisible(0);
                optionWidget->setStatic(1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Simple_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Simple overlay's menu-event filter: event 6 gets 0x101, every other event 0.
 *
 * It fills the onWidgetMenu slot (+0xA4) of D_00474D30 and of the overlay vtables built on it (D_004726A0, D_00472778,
 * cOVState_HEAT_PREEVENT_vtbl).
 *
 * @param menu  the menu (unused)
 * @param event  the menu event
 * @return 0x101 for event 6, else 0
 */
extern "C" int cOVTemplate_Simple_onWidgetMenu(cOVTemplate_Simple* self, int menu, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Simple_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;

/**
 * Simple overlay's widget handler: selecting a widget (event 5) sets cOVStateManager_mStatus to 1, so the next
 * cOVStateManager_update moves on to the next overlay.
 *
 * It fills the onWidgetEvent slot (+0x9C) of D_00474D30 and of the overlay vtables built on it (D_004726A0, D_00472778,
 * cOVState_HEAT_PREEVENT_vtbl).
 *
 * @param widget  the widget; nothing happens when it is null
 * @param key  the widget event
 */
extern "C" void cOVTemplate_Simple_onWidgetEvent(cOVTemplate_Simple* self, int widget, int key)
{
    if (widget != 0 && key == 5) {
        cOVStateManager_mStatus = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onCreateScreen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_87yndialog[];
extern char str_yes_b[];
extern char str_no_b[];
extern char str_title_quit[];
extern char str_kT_OVRCMNQuitGame[];
extern char str_kT_OVRCMNRestartEvent[];
extern char str_kT_OVRCMNQuitChal[];
extern char str_kT_OVRCMNRestartChal[];
extern char str_kT_CMNQuitEventQues[];
extern char str_kT_CMNGiveUp[];
extern char str_kT_CMNSaveBeforeQuit[];
extern char str_title_quitchallenge[];
extern char str_kT_OVRCMNQuitComp[];

struct sVE_DlgCreate {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj_DlgCreate {
    int pad[2];
    sVE_DlgCreate* vt;
};

/**
 * Creates the yes/no dialog overlay ("87yndialog") and sets its question for the dialog kind (+0x9C).
 *
 * Kinds: 0 restart event, 1 quit game, 2 restart challenge, 3 quit challenge, 4 quit event, 5 give up, 6 save
 * before quitting, others quit competition. Kinds 1 and 6 hide the "title_quitchallenge" object.
 */
// Builds the generic dialog overlay: title/body texts depend on the dialog kind at 0x9C.
extern "C" void cOVTemplate_Dialog_onCreateScreen(cOVTemplate_Dialog* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)self), GetHashValue32(str_87yndialog), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(((char*)self));
    *(int*)((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_yes_b)) + 0x18) = 0;
    *(int*)((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_no_b)) + 0x18) = 1;
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_quit));
    switch ((unsigned int)self->mDialogType) {
    case 1: {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNQuitGame));
        sObj_DlgCreate* quitChallengeTitle = (sObj_DlgCreate*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_quitchallenge));
        quitChallengeTitle->vt[9].fn((char*)quitChallengeTitle + quitChallengeTitle->vt[9].delta, 0);
        break;
    }
    case 0:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNRestartEvent));
        break;
    case 3:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNQuitChal));
        break;
    case 2:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNRestartChal));
        break;
    case 4:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNQuitEventQues));
        break;
    case 5:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNGiveUp));
        break;
    case 6: {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNSaveBeforeQuit));
        sObj_DlgCreate* quitChallengeTitle = (sObj_DlgCreate*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_title_quitchallenge));
        quitChallengeTitle->vt[9].fn((char*)quitChallengeTitle + quitChallengeTitle->vt[9].delta, 0);
        break;
    }
    default:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNQuitComp));
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern char str_Menu0000_b[];

/**
 * Preselects the dialog's "Menu0000" menu when it is created: entry 0 for dialog type 6, entry 1 for every other type.
 *
 * The dialog type is the word at +0x9C, the value cOVStateManager_triggerOverlay stores per dialog overlay (0-6; On
 * Tour mDialogType). It fills the onWidgetCreate slot (+0x94) of the dialog vtables (D_004744C0 ... D_00474C58) whose
 * onCreateScreen and onWidgetEvent are cOVTemplate_Dialog_onCreateScreen and cOVTemplate_Dialog_onWidgetEvent.
 *
 * @param item  the widget being created; only the one whose hash name (+0x38) is "Menu0000" is touched
 */
extern "C" void cOVTemplate_Dialog_onWidgetCreate(cOVTemplate_Dialog* self, cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(str_Menu0000_b)) {
        if (self->mDialogType == 6) {
            cUIMenu_setSelectedByIndex(item, 0);
        } else {
            cUIMenu_setSelectedByIndex(item, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Dialog's menu-event filter: events 8 and 9 get 0x100, every other event 0x101.
 *
 * It fills the onWidgetMenu slot (+0xA4) of the dialog vtables (D_004744C0 ... D_00474C58).
 *
 * @param menu  the menu (unused)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, else 0x101
 */
extern "C" int cOVTemplate_Dialog_onWidgetMenu(cOVTemplate_Dialog* self, int menu, unsigned int key)
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
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onWidgetEvent);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern "C" void cGame_exit(void* game, int toFrontEnd);
extern "C" void cGame_restart(void* self);
void cReplay_stopAutoReplay(void* self);
extern "C" void cRider_quitEvent(char* self);
extern "C" void cGame_setState(void* self, int a, int b, int refresh);
extern "C" void cGameModeMan_initGameMode(void* self, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int ev);
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode);
extern "C" void cBE_setState(int state);
extern "C" void* cOVState_AUTOSAVE_cOVState_AUTOSAVE(void* self, void* a1, int a2);
extern "C" void* cOVState_PROFILE_cOVState_PROFILE(void* self, void* a1, int a2, int a3);
extern "C" void cWScriptMan_quitLocalMission(void* self);
extern "C" void cAI_initFreeride(void* self);
extern "C" void cRider_putAtStartpoint(void* self, int a1, int a2);
extern "C" void cRider_cleanupAfterPutAt(void* self, int notify);
extern "C" void cGame_partialRestart(void* self);
void cGameModeMan_restorePreviousHeat(void* self);
extern "C" void cGameComm_localDisconnected(void* self, int a1);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* st, int a2, int a3);
extern int cOVStateManager_mIsPauseDialog;
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern int cBENewRaceInterface_mNextLocation;
extern int cBENewRaceInterface_bIsBamLocation;
extern char str_TransitionOut_b[];
extern char str_Save_Profile[];

struct sI10_D5A8 { char pad[0x10]; int f10; };
struct sSelf_D5A8 { char pad[0x54]; int f54; };
// Same vtable entry; the void/int return only changes v0/v1 choice around the call.
struct sVV_D5A8 { short delta; short index; void (*fn)(void*); };
struct sGame_D5A8 { char pad[0xC]; char* f0C; };
struct sP40_D5A8 { char pad[0x40]; char* f40; };
struct sP18_D5A8 { char pad[0x18]; char* f18; };
static inline char* Game_D5A8() { return ((char*)(SSXApp)->Game); }
static inline char* Rider_D5A8() { return ((sP18_D5A8*)((sP40_D5A8*)((sGame_D5A8*)Game_D5A8())->f0C)->f40)->f18; }
struct sVE_20D5A8 { short delta; short index; int (*fn)(void*); };
struct sStFlags_20D5A8 { char pad[0x1C]; unsigned int lo : 8; unsigned int mode : 6; };

static inline void Play_20D5A8(char* self)
{
    int f = cUIScreen_getFrameByLabel(*(cUIScreen**)(self + 0x40), GetHashValue32(str_TransitionOut_b));
    if (f != 0xFFFF) {
        cUIScreen_playFrame(*(void**)(self + 0x40), f, 1);
    }
}

static inline void Pop_20D5A8()
{
    cOVStateManager_mStatus = 3;
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
}

/**
 * Carries out the dialog's answer: Yes restarts, quits or leaves the event by dialog kind; No or back closes
 * it.
 *
 * Yes on kind 0 restarts the event (re-entering it for game events 6-11), 1 quits to the front end, 2 opens
 * overlay 0x1D, 4 switches to free ride at the start point, 5 makes the rider quit the event, 6 opens the
 * save
 * state. No on kind 6 quits without saving. In kind 1 the "Save Profile" branch is dead: its flag is always
 * 0.
 *
 * @param w  the button (its index at +0x18: 0 yes, 1 no), or 0
 * @param ev  widget event number: 5 select, 6 back
 */
extern "C" void cOVTemplate_Dialog_onWidgetEvent(cOVTemplate_Dialog* self, cUIThing* w, int ev)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (((char*)w) == 0) {
        return;
    }
    switch (ev) {
    case 5:
        Play_20D5A8(((char*)self));
        switch (w->mMiscData) {
        case 0:
            self->mRemovePDA = 1;
            switch (self->mDialogType) {
            case 0: {
                int kind = cBENewRaceInterface_newRace.cGameEvent;
                if (kind >= 6 && kind <= 11) {
                    if (cOVStateManager_mIsPauseDialog) {
                        Pop_20D5A8();
                    } else {
                        cReplay_stopAutoReplay(SSXApp->Game->Replay);
                        cOVStateManager_mStatus = 3;
                    }
                    int mode = 0xE;
                    if (kind == 7 || kind == 10) {
                        mode = 0xF;
                    } else if (kind == 8 || kind == 11) {
                        mode = 0x10;
                    }
                    char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 0);
                    void* gm = SSXApp->GameModeMan;
                    cBE_setState(0);
                    cBENewRaceInterface_setGameMode(iface, cBENewRaceInterface_newRace.cGameMode);
                    cBENewRaceInterface_setGameEvent(iface, kind);
                    cBENewRaceInterface_mNextLocation = mode;
                    cBENewRaceInterface_newRace.iLocationIdx = mode;
                    sVV_D5A8* vt = *(sVV_D5A8**)(iface + 0xC);
                    cBENewRaceInterface_bIsBamLocation = 1;
                    vt[1].fn(iface + vt[1].delta);
                    cGameModeMan_initGameMode(gm, cBENewRaceInterface_newRace.cGameEvent);
                    cGame_exit(SSXApp->Game, 1);
                } else if (cOVStateManager_mIsPauseDialog) {
                    Pop_20D5A8();
                    cGame_restart(Game_D5A8());
                } else {
                    cReplay_stopAutoReplay(SSXApp->Game->Replay);
                    cGameModeMan_restorePreviousHeat(SSXApp->GameModeMan);
                    cGame_setState(SSXApp->Game, 13, 0, 1);
                    cOVStateManager_mStatus = 3;
                }
                break;
            }
            case 1: {
                cReplay_stopAutoReplay(SSXApp->Game->Replay);
                self->mRemovePDA = 0;
                cBE_getInterface_Fv(cBE_getBE(), 4);
                if (cGameComm_mGameComm) {
                    cGameComm_localDisconnected((char*)cGameComm_mGameComm, 0);
                }
                int any = 0;
                int z = 0;
                cBE_getInterface_Fv(SSXApp->BE, 7);
                cBE_getInterface_Fv(cBE_getBE(), 0);
                if (cBENewRaceInterface_newRace.cGamePath != 0) {
                    cGame_exit(SSXApp->Game, 0);
                } else
                chk:
                if (any) {
                    char* ui = (char*)SSXApp->Game->UI;
                    sStFlags_20D5A8* st = (sStFlags_20D5A8*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(cMemMan_alloc(564, str_Save_Profile, 0x100, 0), self->mpEngine, 1);
                    st->mode = 1;
                    cUIStateStack_pushSpecial(ui + 0x18, st, 0, 0);
                } else {
                    cOVStateManager_triggerOverlay(38);
                    cOVStateManager_addPDATemplate();
                }
                // The dead `goto chk` (folded only at combine) keeps `any = 0` from
                // being propagated by gcse, so the zero flag and its test survive
                // as in the original (same trick as gamestate cGFTrackStartState_preloadUpdate).
                { int alwaysOne = z + 1; if (alwaysOne != 1) { any = 1; goto chk; } }
                break;
            }
            case 2:
                cOVStateManager_triggerOverlay(29);
                break;
            case 3:
                Pop_20D5A8();
                cWScriptMan_quitLocalMission(cWScriptMan_TheWScriptMan);
                break;
            case 4: {
                cReplay_stopAutoReplay(SSXApp->Game->Replay);
                cOVStateManager_mStatus = 3;
                char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
                char* missionIface = (char*)cBE_getInterface_Fv(cBE_getBE(), 10);
                cBE_setState(0);
                cBENewRaceInterface_setGameEvent(iface, 0xC);
                cBENewRaceInterface_setGameMode(iface, 4);
                sVE_20D5A8* vt = *(sVE_20D5A8**)(iface + 0xC);
                vt[1].fn(iface + vt[1].delta);
                cGameModeMan_initGameMode(SSXApp->GameModeMan, 0xC);
                cAI_initFreeride(SSXApp->Game->AI);
                ((sI10_D5A8*)missionIface)->f10 = 1;
                cGame_setState(SSXApp->Game, 1, 1, 0);
                cGame_partialRestart(SSXApp->Game);
                cRider_putAtStartpoint(SSXApp->Game->AI->Players[0]->mRider, 1, 2);
                cRider_cleanupAfterPutAt(SSXApp->Game->AI->Players[0]->mRider, 1);
                break;
            }
            case 5:
                Pop_20D5A8();
                cRider_quitEvent(Rider_D5A8());
                break;
            case 6: {
                ((sSelf_D5A8*)self)->f54 = 0;
                char* ui = (char*)SSXApp->Game->UI;
                sStFlags_20D5A8* st = (sStFlags_20D5A8*)cOVState_PROFILE_cOVState_PROFILE(cMemMan_alloc(636, str_Save_Profile, 0x100, 0), self->mpEngine, *(unsigned char*)&self->mCurrentCtrl, 0x18);
                st->mode = 1;
                cUIStateStack_pushSpecial(ui + 0x18, st, 0, 0);
                break;
            }
            }
            break;
        case 1:
            if (self->mDialogType != 6) {
                cUIStateStack_pop((char*)self->mpEngine + 0x18, 1);
            } else {
                ((sSelf_D5A8*)self)->f54 = 1;
                cGame_exit(SSXApp->Game, 0);
            }
            break;
        }
        break;
    case 6:
        if (self->mDialogType != ev) {
            Play_20D5A8(((char*)self));
            cUIStateStack_pop((char*)self->mpEngine + 0x18, 1);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVState_QUIT_onUpdate);
#ifdef SKIP_ASM
struct cUIScreen;
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onUpdate(void* self);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern char str_TransitionOut_b[];

/**
 * Updates the online quit dialog: once the network pause or overlay timer has run out, plays the screen's
 * "TransitionOut" frame and pops the state.
 *
 * After cOVState_onUpdate, when cGameComm_mGameComm exists and its mPauseTimeout or mOverlayTimeout is 0: plays the
 * "TransitionOut" label of mpScreen (cUIScreen_getFrameByLabel, cUIScreen_playFrame) when it exists, and pops one state
 * from mpEngine's state stack (cUIStateStack_pop, On Tour cUIStateStack::pop). It is the onUpdate slot (+0x64) of
 * D_004748F8, the dialog type 1 overlay (cOVStateManager_triggerOverlay type 0x18).
 */
extern "C" void cOVState_QUIT_onUpdate(cOVTemplate_Dialog* self)
{
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        int ok = gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut_b));
            if (frame != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, frame, 1);
            }
            cUIStateStack_pop((char*)self->mpEngine + 0x18, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVState_REPLAY_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cOVState_REPLAY_setupCameraName(void* self);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern char str_64replay[];
extern char str_ReplayMenuPopup[];
extern char str_showhelp[];
extern char str_hidetimeline[];
extern char str_showtimeline[];
extern char str_ReplayMenu[];
extern char str_option_savereplay[];

struct sVE_20DC68 { short delta; short index; void (*fn)(void*, int); };

/**
 * Creates the replay overlay ("64replay"): hides the popup and help, shows the timeline toggle and sets the
 * camera name.
 *
 * When the replay was loaded from a save (cReplay Loaded), selects menu item 2 and hides and disables the
 * save
 * replay option.
 */
extern "C" void cOVState_REPLAY_onCreateScreen(cOVState_REPLAY* self)
{
    self->mHashName = GetHashValue32(str_64replay);
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)self), GetHashValue32(str_64replay), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(((char*)self));
    self->mTimeLineHidden = 0;
    self->mShowPopupMenu = 0;
    cUIState_hideObjSafe(((char*)self), str_ReplayMenuPopup);
    cUIState_hideObjSafe(((char*)self), str_showhelp);
    if (self->mTimeLineHidden != 0) {
        cUIState_hideObjSafe(((char*)self), str_hidetimeline);
        cUIState_showObjSafe(((char*)self), str_showtimeline);
    } else {
        cUIState_showObjSafe(((char*)self), str_hidetimeline);
        cUIState_hideObjSafe(((char*)self), str_showtimeline);
    }
    self->mBottom_y = 245;
    self->mMinBottom_y = 245;
    self->mMaxBottom_y = 0x1E0;
    self->mBottom_ydiff = 0;
    cOVState_REPLAY_setupCameraName(((char*)self));
    if (*(int*)((char*)SSXApp->Game->Replay + 0x608) != 0) {
        self->mLoaded = 1;
    } else {
        self->mLoaded = 0;
    }
    char* obj = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ReplayMenu));
    *(int*)(obj + 0x90) |= 8;
    if (self->mLoaded != 0) {
        if (obj != 0) cUIMenu_setSelectedByIndex(obj, 2);
        char* saveReplayOption = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_option_savereplay));
        if (saveReplayOption != 0) {
            sVE_20DC68* vtEntry = &(*(sVE_20DC68**)(saveReplayOption + 8))[9];
            vtEntry->fn(saveReplayOption + vtEntry->delta, 0);
            vtEntry = &(*(sVE_20DC68**)(saveReplayOption + 8))[8];
            vtEntry->fn(saveReplayOption + vtEntry->delta, 0);
        }
    }
}
#endif

