#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beintmission.h"
#include "be/beintscore.h"
#include "be/beintreward.h"
#include "main/gamestate.h"
#include "ui/uitext.h"
#include "fe/ovstate.h"
#include "fe/ovstatemanager.h"
#include "main/gamemode.h"
#include "ui/uiscreen.h"
#include "ui/uiengine.h"
#include "ui/uithing.h"
#include "ui/uimenu.h"
#include "replay/replay.h"
#include "be/beintnewrace.h"
#include "fe/ovtemplatedialog.h"
#include "fe/ovstatereplay.h"
#include "fe/ovstatepause.h"
#include "fe/ovstateeventresults.h"
#include "fe/ovstatetransport.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"
#include "util/locale.h"

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cPDATemplate_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...)
extern "C" void sprintf1_v(void* dst, int fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern char str_PDATemplate[];
extern char str_bganim1[];
extern char str_temperature[];

static inline int Loc_PDA(int hash)
{
    cFELocale* obj = SSXApp->FELocale;
    return (int)obj->getStringByHash(hash);
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
void cPDATemplate::onCreateScreen()
{
    mHashName = 41;
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_PDATemplate), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_bganim1), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 1);
    }
    cUIText* text = (cUIText*)((char*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_temperature)));
    if (((char*)text) != 0) {
        int fmt = Loc_PDA(text->mStringID);
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
 * The peak challenges (kGAMEEVENT_TIME_PEAK1 .. kGAMEEVENT_POINTS_PEAK3) give kOVSTATE_PEAKCHAL_RESULTS; races and
 * the rival challenges (kGAMEEVENT_RACE, kGAMEEVENT_RIVALTIME, kGAMEEVENT_RIVALPOINTS) or the final heat
 * (cGameModeMan::kFinalHeat) give kOVSTATE_FINAL_STANDINGS; anything else gives kOVSTATE_HEAT_STANDINGS.
 *
 * @param currentGameEvent  the current game event
 * @param currentHeat  the current heat
 * @return the overlay id (On Tour tOVStateEnum)
 */
extern "C" int cOVStateManager_getResultsOverlay(int currentGameEvent, int currentHeat)
{
    if ((unsigned int)(currentGameEvent - kGAMEEVENT_TIME_PEAK1) < 6) {
        return kOVSTATE_PEAKCHAL_RESULTS;
    }
    if ((unsigned int)(currentGameEvent - kGAMEEVENT_RIVALTIME) < 2 || currentGameEvent == kGAMEEVENT_RACE
        || currentHeat == cGameModeMan::kFinalHeat) {
        return kOVSTATE_FINAL_STANDINGS;
    }
    return kOVSTATE_HEAT_STANDINGS;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_gotoOverlay);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVStateManager_triggerOverlay(int state);
int cBERewardInterface_getTotalNumUserAwards(void* self);
extern "C" void cReplay_startAutoReplay(void* p);
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mShowPeakPassOverlay;
extern int cOVStateManager_mMiscData;
extern int cBESaveInterface_bDoReplay;

static inline bool isKind_A8F8(int k, int v)
{
    return k == v;
}

/**
 * Opens the overlay that belongs to a game-flow state: the pre-event screen or the post-race results.
 *
 * Does nothing while a replay (other than state 10+) is playing. For kGFSM_PreEvent picks the replay
 * (kOVSTATE_REPLAY), race intro (kOVSTATE_HEAT_PREEVENT), multiplayer (kOVSTATE_MULTI_PREEVENT), challenge
 * (kOVSTATE_CHALLENGE_PREEVENT) or freestyle (kOVSTATE_FREESTYLE_PREEVENT) overlay; for kGFSM_PostEvent picks a
 * network, results (cOVStateManager_getResultsOverlay), reward (kOVSTATE_REWARDS) or kOVSTATE_TOPTIMES overlay, starts
 * the auto replay for non-rival events and, online, sets cGameComm_mGameComm's overlay timeout to 0x8CA0. With no
 * overlay chosen, sets cOVStateManager_mStatus to kOVSTATUS_EXIT instead of calling cOVStateManager_triggerOverlay.
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
    int type = *(int*)SSXApp->GameModeMan;
    int sub = cBENewRaceInterface_newRace.cGameMode;
    int team = cBENewRaceInterface_newRace.cGamePath;
    int kind = cBENewRaceInterface_newRace.cGameEvent;
    switch (mode) {
    case kGFSM_PreEvent:
        cOVStateManager_mShowPeakPassOverlay = 0;
        if (cBESaveInterface_bDoReplay != 0) {
            state = kOVSTATE_REPLAY;
        } else if (sub == kGAMEMODE_FREERIDE) {
            cOVStateManager_mStatus = mode;
        } else {
            cOVStateManager_mStatus = kOVSTATUS_IDLE;
            if (kind == kGAMEEVENT_RACE) {
                state = kOVSTATE_HEAT_PREEVENT;
            } else if (team == mode) {
                state = kOVSTATE_MULTI_PREEVENT;
            } else {
                if (kind >= kGAMEEVENT_RIVALTIME && kind <= kGAMEEVENT_POINTS_PEAK3)
                    state = kOVSTATE_CHALLENGE_PREEVENT;
                else
                    state = kOVSTATE_FREESTYLE_PREEVENT;
            }
        }
        break;
    case kGFSM_PostEvent:
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (sub == kGAMEMODE_FREERIDE) {
            cOVStateManager_mStatus = sub;
            return;
        }
        cOVStateManager_mStatus = 0;
        cBEScoreInterface* scoreIface = (cBEScoreInterface*)cBE_getInterface_Fv(cBE_getBE(), 8);
        cBERewardInterface* rewardIface = (cBERewardInterface*)cBE_getInterface_Fv(cBE_getBE(), 13);
        int any = 0;
        if (scoreIface->mNewTrackHighScoreIndex[0] >= 0 || scoreIface->mNewTrackHighScoreIndex[1] >= 0) any = 1;
        if (any) {
            if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                state = cOVStateManager_getResultsOverlay(kind, type);
            } else {
                cOVStateManager_mMiscData = 0;
                state = kOVSTATE_TOPTIMES;
            }
        } else {
            if (cBERewardInterface_getTotalNumUserAwards(rewardIface) > 0 || rewardIface->mLastEventCashReward != 0) {
                state = kOVSTATE_REWARDS;
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
    if (state == kOVSTATE_NULL) {
        cOVStateManager_mStatus = kOVSTATUS_EXIT;
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
extern char cOVState_CHALLENGE_PREEVENT_vtbl[];
extern char cOVState_MULTI_PREEVENT_vtbl[];
extern char cOVState_FREESTYLE_PREEVENT_vtbl[];
extern char cOVState_HEAT_PREEVENT_vtbl[];
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_PAUSE_OPTIONS_vtbl[];
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char cOVState_PRIVATEMESSAGE_vtbl[];
extern char cOVState_MAP_vtbl[];
extern char cOVState_TRANSPORT_vtbl[];
extern char cOVState_TOPTIMES_vtbl[];
extern char cOVState_BIGCHALLENGE_FAIL_vtbl[];
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_ENTERLODGE_vtbl[];
extern char cOVState_TOURNAMENT_STANDINGS_vtbl[];
extern char cOVState_BASSSP_QUAL_FINISHOV_vtbl[];
extern char cOVState_RACE_FINISHOV_vtbl[];
extern char cOVState_PEAKCHAL_RESULTS_vtbl[];
extern char cOVState_FINAL_STANDINGS_vtbl[];
extern char cOVState_HEAT_STANDINGS_vtbl[];
extern char cOVState_SAVEPROMPT_vtbl[];
extern char cOVState_GIVEUP_vtbl[];
extern char cOVState_QUITCOMPETITION_vtbl[];
extern char cOVState_QUITCHALLENGE_vtbl[];
extern char cOVState_RESTARTCHALLENGE_vtbl[];
extern char cOVState_QUIT_vtbl[];
extern char cOVState_RESTART_vtbl[];
extern char cOVState_REPLAY_vtbl[];
extern char cOVState_EMPTY_vtbl[];
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
    cUIState* result = 0;
    cUIEngine* mgr = SSXApp->Game->UI;
    if (type == kOVSTATE_PAUSE_COMPETE) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_COMPETE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_BIGCHAL) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_BIGCHAL_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_FREERIDE) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_FREERIDE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_SPOKE) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_SPOKE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_QUICKPLAY) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_QUICKPLAY_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_ONLINE) {
        cOVTemplate_PauseMenu* overlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_ONLINE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mpThread = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_OPTIONS) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_OPTIONS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_HEAT_PREEVENT) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_HEAT_PREEVENT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_FREESTYLE_PREEVENT) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_FREESTYLE_PREEVENT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_HEAT_STANDINGS) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_HEAT_STANDINGS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_FINAL_STANDINGS) {
        cOVState* overlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_FINAL_STANDINGS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_RACE_FINISHOV) {
        cOVState_RACE_FINISHOV* overlay = (cOVState_RACE_FINISHOV*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, overlay) = (void**)cOVState_RACE_FINISHOV_vtbl;
        overlay->mCanContinue = 0x1;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mHashName = GetHashValue32(str_finishov);
        result = overlay;
    }
    if (type == kOVSTATE_BASSSP_QUAL_FINISHOV) {
        cOVState_BASSSP_QUAL_FINISHOV* overlay = (cOVState_BASSSP_QUAL_FINISHOV*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, overlay) = (void**)cOVState_BASSSP_QUAL_FINISHOV_vtbl;
        overlay->mCanContinue = 0x1;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mHashName = GetHashValue32(str_finishov);
        result = overlay;
    }
    if (type == kOVSTATE_EMPTY) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_EMPTY_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_TOPTIMES) {
        cOVState* overlay = (cOVState*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_TOPTIMES_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_REWARDS) {
        cOVState* overlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_REWARDS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_REPLAY) {
        cOVState* overlay = (cOVState*)((char*)operator new(184, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_REPLAY_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_SAVEREPLAY) {
        result = (cUIState*)cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 18);
    }
    if (type == kOVSTATE_SAVEOPTIONS) {
        result = (cUIState*)cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 19);
    }
    if (type == kOVSTATE_CHALLENGE_PREEVENT) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_CHALLENGE_PREEVENT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_MULTI_PREEVENT) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_MULTI_PREEVENT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PEAKCHAL_RESULTS) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PEAKCHAL_RESULTS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_RESTART) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_RESTART_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->mDialogType = cOVTemplate_Dialog::kDialogRestart;
        result = overlay;
    }
    if (type == kOVSTATE_QUIT) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_QUIT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogQuit;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_RESTARTCHALLENGE) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_RESTARTCHALLENGE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogRestartChallenge;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_QUITCHALLENGE) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_QUITCHALLENGE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogQuitChallenge;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_QUITCOMPETITION) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_QUITCOMPETITION_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogQuitCompetition;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_GIVEUP) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_GIVEUP_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogGiveUp;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_BIGCHALLENGE_START) {
        cOVState* overlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_BIGCHALLENGE_START_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        cOVState_BIGCHALLENGE_START_getChallengeData(overlay);
        result = overlay;
    }
    if (type == kOVSTATE_BIGCHALLENGE_FAIL) {
        cOVState* overlay = (cOVState*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_BIGCHALLENGE_FAIL_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        cOVState_BIGCHALLENGE_FAIL_getChallengeData(overlay);
        result = overlay;
    }
    if (type == kOVSTATE_ENTERLODGE) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_ENTERLODGE_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_MAP) {
        cOVState* overlay = (cOVState*)((char*)operator new(468, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, overlay) = (void**)cOVState_MAP_vtbl;
        new (((char*)overlay) + 0x9C) sDlgSlot_AB50[8];
        result = overlay;
    }
    if (type == kOVSTATE_TRANSPORT) {
        cOVState_TRANSPORT* overlay = (cOVState_TRANSPORT*)((char*)operator new(912, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_TRANSPORT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        cUITemplate_MAP_init(&overlay->mMap);
        result = overlay;
    }
    if (type == kOVSTATE_MESSAGECENTER) {
        result = (cUIState*)cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(operator new(0x180, str_cOVStateOverlay, 0, 0), mgr, 34);
    }
    if (type == kOVSTATE_TOURNAMENT_STANDINGS) {
        cOVState* overlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_TOURNAMENT_STANDINGS_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PROFILE) {
        result = (cUIState*)cOVState_PROFILE_cOVState_PROFILE(operator new(636, str_cOVStateOverlay, 0, 0), mgr, 36, 42);
    }
    if (type == kOVSTATE_AUTOSAVE) {
        result = (cUIState*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(operator new(564, str_cOVStateOverlay, 0, 0), mgr, 37);
    }
    if (type == kOVSTATE_SAVEPROMPT) {
        cOVTemplate_Dialog* overlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_SAVEPROMPT_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mDialogType = cOVTemplate_Dialog::kDialogSavePrompt;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PAUSE_ONLINE_ERROR) {
        cOVState_PAUSE_ONLINE_ERROR* overlay = (cOVState_PAUSE_ONLINE_ERROR*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PAUSE_ONLINE_ERROR_vtbl;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        overlay->mbShouldDisplayAuxInfo = 0x1;
        overlay->mOVID = type;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        overlay->miStartPingTick = 0;
        result = overlay;
    }
    if (type == kOVSTATE_PRIVATEMESSAGE) {
        cOVState* overlay = (cOVState*)((char*)operator new(228, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(overlay, mgr);
        overlay->mOVID = type;
        *(unsigned short*)overlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, overlay) = (void**)cOVState_PRIVATEMESSAGE_vtbl;
        overlay->mDoneTransition = 0;
        overlay->mAllowPauseMenu = 0;
        overlay->mRemovePDA = 0;
        result = overlay;
    }

    result->mAttrState = (result->mAttrState & ~0x3F00) | 0x100;
    cUIStateStack_pushSpecial(&mgr->mStateStack, result, 0, 0);
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
extern char cOVState_CHALLENGE_PREEVENT_vtbl[];
extern char cOVState_MULTI_PREEVENT_vtbl[];
extern char cOVState_FREESTYLE_PREEVENT_vtbl[];
extern char cOVState_HEAT_PREEVENT_vtbl[];
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_PAUSE_OPTIONS_vtbl[];
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char cOVState_PRIVATEMESSAGE_vtbl[];
extern char cOVState_MAP_vtbl[];
extern char cOVState_TRANSPORT_vtbl[];
extern char cOVState_TOPTIMES_vtbl[];
extern char cOVState_BIGCHALLENGE_FAIL_vtbl[];
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_ENTERLODGE_vtbl[];
extern char cOVState_TOURNAMENT_STANDINGS_vtbl[];
extern char cOVState_BASSSP_QUAL_FINISHOV_vtbl[];
extern char cOVState_RACE_FINISHOV_vtbl[];
extern char cOVState_PEAKCHAL_RESULTS_vtbl[];
extern char cOVState_FINAL_STANDINGS_vtbl[];
extern char cOVState_HEAT_STANDINGS_vtbl[];
extern char cOVState_SAVEPROMPT_vtbl[];
extern char cOVState_GIVEUP_vtbl[];
extern char cOVState_QUITCOMPETITION_vtbl[];
extern char cOVState_QUITCHALLENGE_vtbl[];
extern char cOVState_RESTARTCHALLENGE_vtbl[];
extern char cOVState_QUIT_vtbl[];
extern char cOVState_RESTART_vtbl[];
extern char cOVState_REPLAY_vtbl[];
extern char cOVState_EMPTY_vtbl[];

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
    cUIState* result = 0;
    cUIEngine* mgr = SSXApp->Game->UI;
    int cur = (int)cUIStateStack_getCurrentState(&mgr->mStateStack);
    if (type == kOVSTATE_PAUSE_COMPETE) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_COMPETE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_BIGCHAL) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_BIGCHAL_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_FREERIDE) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_FREERIDE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_SPOKE) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_SPOKE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_QUICKPLAY) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_QUICKPLAY_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_ONLINE) {
        cOVTemplate_PauseMenu* newOverlay = (cOVTemplate_PauseMenu*)((char*)operator new(276, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_ONLINE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mpThread = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_OPTIONS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_OPTIONS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_HEAT_PREEVENT) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_HEAT_PREEVENT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_FREESTYLE_PREEVENT) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_FREESTYLE_PREEVENT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_HEAT_STANDINGS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_HEAT_STANDINGS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_FINAL_STANDINGS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_FINAL_STANDINGS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_RACE_FINISHOV) {
        cOVState_RACE_FINISHOV* newOverlay = (cOVState_RACE_FINISHOV*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_RACE_FINISHOV_vtbl;
        newOverlay->mCanContinue = 0x1;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mHashName = GetHashValue32(str_finishov);
        result = newOverlay;
    }
    if (type == kOVSTATE_BASSSP_QUAL_FINISHOV) {
        cOVState_BASSSP_QUAL_FINISHOV* newOverlay = (cOVState_BASSSP_QUAL_FINISHOV*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_BASSSP_QUAL_FINISHOV_vtbl;
        newOverlay->mCanContinue = 0x1;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mHashName = GetHashValue32(str_finishov);
        result = newOverlay;
    }
    if (type == kOVSTATE_EMPTY) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_EMPTY_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_TOPTIMES) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_TOPTIMES_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_REWARDS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_REWARDS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_REPLAY) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(184, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_REPLAY_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_SAVEREPLAY) {
        result = (cUIState*)cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 18);
    }
    if (type == kOVSTATE_SAVEOPTIONS) {
        result = (cUIState*)cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(operator new(628, str_cOVStateOverlay, 0, 0), mgr, 19);
    }
    if (type == kOVSTATE_CHALLENGE_PREEVENT) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_CHALLENGE_PREEVENT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_MULTI_PREEVENT) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_MULTI_PREEVENT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PEAKCHAL_RESULTS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PEAKCHAL_RESULTS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_RESTART) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_RESTART_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogRestart;
        result = newOverlay;
    }
    if (type == kOVSTATE_QUIT) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_QUIT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogQuit;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_RESTARTCHALLENGE) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_RESTARTCHALLENGE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogRestartChallenge;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_QUITCHALLENGE) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_QUITCHALLENGE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogQuitChallenge;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_QUITCOMPETITION) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_QUITCOMPETITION_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogQuitCompetition;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_GIVEUP) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_GIVEUP_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogGiveUp;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_BIGCHALLENGE_START) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_BIGCHALLENGE_START_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        cOVState_BIGCHALLENGE_START_getChallengeData(newOverlay);
        result = newOverlay;
    }
    if (type == kOVSTATE_BIGCHALLENGE_FAIL) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_BIGCHALLENGE_FAIL_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        cOVState_BIGCHALLENGE_FAIL_getChallengeData(newOverlay);
        result = newOverlay;
    }
    if (type == kOVSTATE_ENTERLODGE) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_ENTERLODGE_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_MAP) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(468, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_MAP_vtbl;
        new (((char*)newOverlay) + 0x9C) sDlgSlot_BAB8[8];
        result = newOverlay;
    }
    if (type == kOVSTATE_TRANSPORT) {
        cOVState_TRANSPORT* newOverlay = (cOVState_TRANSPORT*)((char*)operator new(912, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_TRANSPORT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        cUITemplate_MAP_init(&newOverlay->mMap);
        result = newOverlay;
    }
    if (type == kOVSTATE_MESSAGECENTER) {
        result = (cUIState*)cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(operator new(0x180, str_cOVStateOverlay, 0, 0), mgr, 34);
    }
    if (type == kOVSTATE_TOURNAMENT_STANDINGS) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(156, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_TOURNAMENT_STANDINGS_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PROFILE) {
        result = (cUIState*)cOVState_PROFILE_cOVState_PROFILE(operator new(636, str_cOVStateOverlay, 0, 0), mgr, 36, 42);
    }
    if (type == kOVSTATE_AUTOSAVE) {
        result = (cUIState*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(operator new(564, str_cOVStateOverlay, 0, 0), mgr, 37);
    }
    if (type == kOVSTATE_SAVEPROMPT) {
        cOVTemplate_Dialog* newOverlay = (cOVTemplate_Dialog*)((char*)operator new(160, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_SAVEPROMPT_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mDialogType = cOVTemplate_Dialog::kDialogSavePrompt;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PAUSE_ONLINE_ERROR) {
        cOVState_PAUSE_ONLINE_ERROR* newOverlay = (cOVState_PAUSE_ONLINE_ERROR*)((char*)operator new(164, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PAUSE_ONLINE_ERROR_vtbl;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        newOverlay->mbShouldDisplayAuxInfo = 0x1;
        newOverlay->mOVID = type;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        newOverlay->miStartPingTick = 0;
        result = newOverlay;
    }
    if (type == kOVSTATE_PRIVATEMESSAGE) {
        cOVState* newOverlay = (cOVState*)((char*)operator new(228, str_cOVStateOverlay, 0, 0));
        cUIState_cUIState(newOverlay, mgr);
        newOverlay->mOVID = type;
        *(unsigned short*)newOverlay->mszChatBuff = str_sp_b[0];
        VPTR_OF(cListNode, newOverlay) = (void**)cOVState_PRIVATEMESSAGE_vtbl;
        newOverlay->mDoneTransition = 0;
        newOverlay->mAllowPauseMenu = 0;
        newOverlay->mRemovePDA = 0;
        result = newOverlay;
    }

    result->mpParentState = (cUIState*)cur;
    cUIStateStack_pushPopup(&mgr->mStateStack, result);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_triggerPauseMenu);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
 * (cGameComm_sendPauseCmd with 1) and starts mPauseTimeout = 3600 and mOverlayTimeout = 1800; then triggers
 * kOVSTATE_PAUSE_ONLINE. Offline: kOVSTATE_PAUSE_BIGCHAL while a local mission is active
 * (cWScriptMan_isLocalMissionActive), else kOVSTATE_PAUSE_COMPETE on kGAMEPATH_WORLDCIRCUIT outside
 * kGAMEMODE_FREERIDE, kOVSTATE_PAUSE_SPOKE in game state kGFSM_SectionLoad (mCurrentStateID), kOVSTATE_PAUSE_FREERIDE
 * in kGAMEMODE_FREERIDE and kOVSTATE_PAUSE_QUICKPLAY otherwise
 * (cGameMode and cGamePath of cBENewRaceInterface_newRace; the overlays through cOVStateManager_triggerOverlay).
 * Finally clears cOVTemplate_PauseMenu_mSelectedMenuIndex, the pause menu's remembered selection. The results of the two cBE_getInterface__Fv calls
 * (interfaces 0 and 7) are not used.
 *
 * cGame_update and cGame_updateUI call it after cOVStateManager_allowPauseMenu allows the pause menu.
 */
extern "C" void cOVStateManager_triggerPauseMenu(void)
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
        cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_ONLINE);
    } else if (cWScriptMan_isLocalMissionActive(cWScriptMan_TheWScriptMan) != 0) {
        cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_BIGCHAL);
    } else {
        signed char* raceDb = (signed char*)&cBENewRaceInterface_newRace;
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_WORLDCIRCUIT && notMode4_0020CA10(raceDb)) {
            cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_COMPETE);
        } else if (SSXApp->Game->mCurrentStateID == kGFSM_SectionLoad) {
            cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_SPOKE);
        } else {
            if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE) {
                cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_FREERIDE);
            } else {
                cOVStateManager_triggerOverlay(kOVSTATE_PAUSE_QUICKPLAY);
            }
        }
    }
    cOVTemplate_PauseMenu_mSelectedMenuIndex = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_replayOverlayActive);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int cUIStateStack_findState(void* stack, int hash);
extern char str_64replay[];

/**
 * Tells whether a "64replay" state is on the game UI's state stack.
 *
 * @return 1 when cUIStateStack_findState finds the hash of "64replay" on SSXApp->Game->UI's stack, else 0
 */
extern "C" bool cOVStateManager_replayOverlayActive(void)
{
    char* stack = (char*)&SSXApp->Game->UI->mStateStack;
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
extern "C" bool cOVStateManager_pauseOverlayActive(void)
{
    char* stack = (char*)&SSXApp->Game->UI->mStateStack;
    return cUIStateStack_findState(stack, GetHashValue32(str_cOVTemplate_PauseMenu)) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_allowPauseMenu);
#ifdef SKIP_ASM
extern "C" void* cUIStateStack_getCurrentState(void* stack);
extern "C" bool cOVStateManager_pauseOverlayActive(void);

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
    cUIEngine* mgr = SSXApp->Game->UI;
    if (cOVStateManager_pauseOverlayActive() == 0) {
        cOVState* topState = (cOVState*)cUIStateStack_getCurrentState(&mgr->mStateStack);
        if (topState != 0) {
            if (topState->mAttrState & 1) {
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVStateManager_triggerOverlay(int state);
extern int cOVStateManager_mStatus;

/**
 * Opens the overlay that follows a finished event, or lets play resume when there is none.
 *
 * By cBENewRaceInterface_newRace's cGameMode and cGamePath: kGAMEMODE_RACE or kGAMEPATH_MULTIPLAYER trigger
 * kOVSTATE_RACE_FINISHOV; kGAMEMODE_SLOPESTYLE .. kGAMEMODE_HALFPIPE and kGAMEMODE_PNTSCHALLENGE
 * kOVSTATE_BASSSP_QUAL_FINISHOV (cOVStateManager_triggerOverlay; both are the "finishov" overlays); kGAMEMODE_FREERIDE
 * sets cOVStateManager_mStatus to kOVSTATUS_EXIT so the next cOVStateManager_update resumes play; any other mode
 * kOVSTATE_RACE_FINISHOV. The result
 * of cBE_getInterface__Fv (interface 0) is not used. cGFFinishlineState_update (load screen) calls it.
 */
extern "C" void cOVStateManager_triggerFinishOverlay(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGameMode;
    int players = cBENewRaceInterface_newRace.cGamePath;
    if (mode == kGAMEMODE_RACE || players == kGAMEPATH_MULTIPLAYER) {
        cOVStateManager_triggerOverlay(kOVSTATE_RACE_FINISHOV);
    } else if ((mode >= kGAMEMODE_SLOPESTYLE && mode <= kGAMEMODE_HALFPIPE) || mode == kGAMEMODE_PNTSCHALLENGE) {
        cOVStateManager_triggerOverlay(kOVSTATE_BASSSP_QUAL_FINISHOV);
    } else if (mode == kGAMEMODE_FREERIDE) {
        cOVStateManager_mStatus = kOVSTATUS_EXIT;
    } else {
        cOVStateManager_triggerOverlay(kOVSTATE_RACE_FINISHOV);
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
 * (kOVSTATE_HEAT_STANDINGS, kOVSTATE_FINAL_STANDINGS, kOVSTATE_PEAKCHAL_RESULTS) acts on cOVStateManager_mMiscData
 * (open the map with style 1, restart via cGame_setState, show a replay, leave an online game with
 * cGameComm_localDisconnected and cGame_exit...); after kOVSTATE_TOPTIMES and kOVSTATE_REWARDS shows rewards or the
 * results overlay (cOVStateManager_getResultsOverlay); after the pre-event overlays (kOVSTATE_HEAT_PREEVENT,
 * kOVSTATE_FREESTYLE_PREEVENT, kOVSTATE_CHALLENGE_PREEVENT, kOVSTATE_MULTI_PREEVENT) sets mStatus to kOVSTATUS_EXIT so
 * the next update resumes play.
 */
// Continues after a result dialog closes, depending on the current overlay state and the pending dialog.
extern "C" void cOVStateManager_gotoNextOverlay(void)
{
    cOVStateManager_mStatus = kOVSTATUS_IDLE;
    cOVState* st = (cOVState*)cUIStateStack_getCurrentState(&((SSXApp)->Game)->UI->mStateStack);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int kind = cBENewRaceInterface_newRace.cGameEvent;
    int type = *(int*)SSXApp->GameModeMan;
    switch (st->mOVID) {
    case kOVSTATE_HEAT_STANDINGS:
    case kOVSTATE_FINAL_STANDINGS:
        switch (cOVStateManager_mMiscData) {
        case 3:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cUITemplate_MAP_mMapStyle = 1;
            cOVStateManager_triggerOverlay(kOVSTATE_TRANSPORT);
            break;
        case 1:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cGame_setState(SSXApp->Game, kGFSM_NextHeat, 0, 1);
            break;
        case 2:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_mStatus = kOVSTATUS_GOTOMAP;
            break;
        case 4:
            cOVStateManager_triggerOverlay(kOVSTATE_TOURNAMENT_STANDINGS);
            break;
        case 5:
            cOVStateManager_mIsPauseDialog = 0;
            cOVStateManager_triggerOverlay(kOVSTATE_RESTART);
            break;
        case 6:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_triggerOverlay(kOVSTATE_REPLAY);
            cReplay_startReplay(SSXApp->Game->Replay, 0, 0, 0);
            break;
        case 7:
            cOVStateManager_mMiscData = 1;
            cOVStateManager_triggerOverlay(kOVSTATE_TOPTIMES);
            break;
        case 8:
            cBE_getInterface_Fv(SSXApp->BE, 7);
            if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                if (cGameComm_mGameComm != 0) {
                    cGameComm_localDisconnected((char*)cGameComm_mGameComm, 0);
                }
                cGame_exit(SSXApp->Game, 0);
            } else {
                cOVStateManager_triggerOverlay(kOVSTATE_QUIT);
            }
            break;
        }
        break;
    case kOVSTATE_TOPTIMES:
        if (cOVStateManager_mMiscData == kOVSTATE_SAVEOPTIONS) {
            cOVStateManager_triggerOverlay(kOVSTATE_SAVEOPTIONS);
        } else {
            cBERewardInterface* rewardIface = (cBERewardInterface*)cBE_getInterface_Fv(cBE_getBE(), 13);
            if (cBERewardInterface_getTotalNumUserAwards(rewardIface) > 0 || rewardIface->mLastEventCashReward != 0) {
                cOVStateManager_triggerOverlay(kOVSTATE_REWARDS);
            } else {
                cOVStateManager_triggerOverlay(cOVStateManager_getResultsOverlay(kind, type));
            }
        }
        break;
    case kOVSTATE_REWARDS:
        cOVStateManager_triggerOverlay(cOVStateManager_getResultsOverlay(kind, type));
        break;
    case kOVSTATE_HEAT_PREEVENT:
    case kOVSTATE_FREESTYLE_PREEVENT:
    case kOVSTATE_CHALLENGE_PREEVENT:
    case kOVSTATE_MULTI_PREEVENT:
        cOVStateManager_mStatus = kOVSTATUS_EXIT;
        break;
    case kOVSTATE_PEAKCHAL_RESULTS:
        switch (cOVStateManager_mMiscData) {
        case 2:
            cReplay_stopAutoReplay(SSXApp->Game->Replay);
            cOVStateManager_mStatus = kOVSTATUS_GOTOMAP;
            break;
        case 5:
            cOVStateManager_mIsPauseDialog = 0;
            cOVStateManager_triggerOverlay(kOVSTATE_RESTART);
            break;
        case 8:
            cOVStateManager_triggerOverlay(kOVSTATE_QUIT);
            break;
        }
        break;
    case kOVSTATE_TOURNAMENT_STANDINGS:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_update);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void cUIStateStack_purgeNormalStates(void *);




/**
 * Per-frame overlay manager step: carries out the pending overlay action in cOVStateManager_mStatus.
 *
 * kOVSTATUS_GOTONEXT goes to the next overlay (cOVStateManager_gotoNextOverlay); kOVSTATUS_EXIT stops the auto
 * replay, flags the current game state to continue and, in an online race's pre-event state (kOVSTATE_HEAT_PREEVENT),
 * purges the UI states (cUIStateStack_purgeNormalStates); kOVSTATUS_PURGENORMAL removes the PDA template and purges the
 * UI states; kOVSTATUS_GOTOMAP sets game state kGFSM_UserSelect via cGame_setState. Each case clears mStatus first.
 */
extern "C" void cOVStateManager_update(void)
{
    cUIEngine* mgr = ((SSXApp)->Game)->UI;
    cOVState* st = 0;
    if ((char*)&mgr->mStateStack) {
        st = (cOVState*)((char *)cUIStateStack_getCurrentState(&mgr->mStateStack));
    }
    switch (cOVStateManager_mStatus) {
    case kOVSTATUS_IDLE:
        break;
    case kOVSTATUS_GOTONEXT:
        cOVStateManager_gotoNextOverlay();
        break;
    case kOVSTATUS_EXIT:
        cOVStateManager_mStatus = kOVSTATUS_IDLE;
        cReplay_stopAutoReplay(((void*)((SSXApp)->Game)->Replay));
        (((SSXApp)->Game)->mCurrentState)->mOverlayDone = 1;
        if (((char*)st) != 0 && ((SSXApp)->Game)->mCurrentStateID == kGFSM_PreEvent) {
            if (st->mOVID == kOVSTATE_HEAT_PREEVENT) {
                cBE_getInterface_Fv(((void*)(SSXApp)->BE), 7);
                if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                    cUIStateStack_purgeNormalStates(&mgr->mStateStack);
                }
            }
        }
        break;
    case kOVSTATUS_PURGENORMAL:
        cOVStateManager_mStatus = kOVSTATUS_IDLE;
        cOVStateManager_removePDATemplate();
        cUIStateStack_purgeNormalStates(&((SSXApp)->Game)->UI->mStateStack);
        break;
    case kOVSTATUS_GOTOMAP:
        cOVStateManager_mStatus = kOVSTATUS_IDLE;
        cGame_setState(((SSXApp)->Game), kGFSM_UserSelect, 2, 0);
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
        cUIEngine* mgr = SSXApp->Game->UI;
        cPDATemplate* pdaTemplate = (cPDATemplate*)cMemMan_alloc(72, str_PDATemplate, 0x100, 0);
        cUIState_cUIState(pdaTemplate, mgr);
        VPTR_OF(cListNode, pdaTemplate) = (void**)cPDATemplate_vtbl;
        cOVStateManager_mpPDATemplate = pdaTemplate;
        cUIStateStack_pushExplicit(&mgr->mStateStack, pdaTemplate);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_removePDATemplate);
#ifdef SKIP_ASM
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
        delete (cPDATemplate*)cOVStateManager_mpPDATemplate;
    }
    cOVStateManager_mpPDATemplate = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Simple_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int cOVTemplate_Simple_optionLabelTable[2];
extern char* cOVState_overlayOptionTextIDs[];
extern char str_overlayMenu[];
extern char str_overlayOptpct_d[];

/**
 * Builds the screen of a simple overlay: the base overlay screen plus a menu with one visible option.
 *
 * Runs cOVState_onCreateScreen, sets flag 8 in the "overlayMenu" widget's word at +0x90, then walks "overlayOpt0" ...
 * "overlayOpt4" of mpScreen: option 0 gets the localised string named cOVState_overlayOptionTextIDs[cOVTemplate_Simple_optionLabelTable[0]] and that index as its
 * misc data (+0x18); options 1-4 are hidden (setVisible(0), slot 9) and made static (setStatic(1), slot 8). It fills the
 * onCreateScreen slot (+0x24) of cOVTemplate_Simple_vtbl, the vtable that also holds cOVTemplate_Simple_onWidgetMenu and cOVTemplate_Simple_onWidgetEvent; cOVState_FREESTYLE_PREEVENT_onCreateScreen
 * and cOVState_HEAT_PREEVENT_onCreateScreen call it as their base.
 */
void cOVTemplate_Simple::onCreateScreen()
{
    cOVState_onCreateScreen(this);
    char* list = (char*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_overlayMenu));
    *(int*)(list + 0x90) |= 8;
    for (int i = 0; i < 5; i++) {
        char buf[32];
        sprintf(buf, str_overlayOptpct_d, i);
        cUIThing* optionWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        if (optionWidget != 0) {
            if (i < 1) {
                int labelIndex = cOVTemplate_Simple_optionLabelTable[i];
                cUIText_setUnicodeStringByID((cUIText*)optionWidget, GetHashValue32(cOVState_overlayOptionTextIDs[labelIndex]));
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
 * Simple overlay's menu-event filter: kONNEXT gets 0x101, every other event 0.
 *
 * It fills the onWidgetMenu slot (+0xA4) of cOVTemplate_Simple_vtbl and of the overlay vtables built on it (cOVState_MULTI_PREEVENT_vtbl, cOVState_FREESTYLE_PREEVENT_vtbl,
 * cOVState_HEAT_PREEVENT_vtbl).
 *
 * @param menu  the menu (unused)
 * @param event  the menu event
 * @return 0x101 for kONNEXT, else 0
 */
int cOVTemplate_Simple::onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Simple_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;

/**
 * Simple overlay's widget handler: selecting a widget (kONNEXT) sets cOVStateManager_mStatus to kOVSTATUS_GOTONEXT, so
 * the next cOVStateManager_update moves on to the next overlay.
 *
 * It fills the onWidgetEvent slot (+0x9C) of cOVTemplate_Simple_vtbl and of the overlay vtables built on it (cOVState_MULTI_PREEVENT_vtbl, cOVState_FREESTYLE_PREEVENT_vtbl,
 * cOVState_HEAT_PREEVENT_vtbl).
 *
 * @param widget  the widget; nothing happens when it is null
 * @param key  the widget event
 */
void cOVTemplate_Simple::onWidgetEvent(cUIThing* widget, unsigned int key)
{
    if (widget != 0 && key == cUIState::kONNEXT) {
        cOVStateManager_mStatus = kOVSTATUS_GOTONEXT;
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
void cOVTemplate_Dialog::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)this), GetHashValue32(str_87yndialog), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(((char*)this));
    ((cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_yes_b)))->mMiscData = 0;
    ((cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_no_b)))->mMiscData = 1;
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title_quit));
    switch ((unsigned int)mDialogType) {
    case cOVTemplate_Dialog::kDialogQuit: {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNQuitGame));
        cUIThing* quitChallengeTitle = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title_quitchallenge));
        quitChallengeTitle->setVisible(0);
        break;
    }
    case cOVTemplate_Dialog::kDialogRestart:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNRestartEvent));
        break;
    case cOVTemplate_Dialog::kDialogQuitChallenge:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNQuitChal));
        break;
    case cOVTemplate_Dialog::kDialogRestartChallenge:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNRestartChal));
        break;
    case cOVTemplate_Dialog::kDialogQuitCompetition:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNQuitEventQues));
        break;
    case cOVTemplate_Dialog::kDialogGiveUp:
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNGiveUp));
        break;
    case cOVTemplate_Dialog::kDialogSavePrompt: {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_CMNSaveBeforeQuit));
        cUIThing* quitChallengeTitle = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title_quitchallenge));
        quitChallengeTitle->setVisible(0);
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
 * Tour mDialogType). It fills the onWidgetCreate slot (+0x94) of the dialog vtables (cOVState_SAVEPROMPT_vtbl ... cOVTemplate_Dialog_vtbl) whose
 * onCreateScreen and onWidgetEvent are cOVTemplate_Dialog_onCreateScreen and cOVTemplate_Dialog_onWidgetEvent.
 *
 * @param item  the widget being created; only the one whose hash name (+0x38) is "Menu0000" is touched
 */
void cOVTemplate_Dialog::onWidgetCreate(cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(str_Menu0000_b)) {
        if (mDialogType == cOVTemplate_Dialog::kDialogSavePrompt) {
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
 * It fills the onWidgetMenu slot (+0xA4) of the dialog vtables (cOVState_SAVEPROMPT_vtbl ... cOVTemplate_Dialog_vtbl).
 *
 * @param menu  the menu (unused)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, else 0x101
 */
int cOVTemplate_Dialog::onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int)
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

// Same vtable entry; the void/int return only changes v0/v1 choice around the call.
struct sVV_D5A8 { short delta; short index; void (*fn)(void*); };
struct sGame_D5A8 { char pad[0xC]; char* f0C; };
struct sP40_D5A8 { char pad[0x40]; char* f40; };
struct sP18_D5A8 { char pad[0x18]; char* f18; };
static inline char* Game_D5A8() { return ((char*)(SSXApp)->Game); }
static inline char* Rider_D5A8() { return ((sP18_D5A8*)((sP40_D5A8*)((sGame_D5A8*)Game_D5A8())->f0C)->f40)->f18; }

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
void cOVTemplate_Dialog::onWidgetEvent(cUIThing* w, unsigned int ev)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (((char*)w) == 0) {
        return;
    }
    switch (ev) {
    case 5:
        Play_20D5A8(((char*)this));
        switch (w->mMiscData) {
        case 0:
            mRemovePDA = 1;
            switch (mDialogType) {
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
                    cBEInterface* iface = cBE_getInterface_Fv(SSXApp->BE, 0);
                    void* gm = SSXApp->GameModeMan;
                    cBE_setState(0);
                    cBENewRaceInterface_setGameMode(iface, cBENewRaceInterface_newRace.cGameMode);
                    cBENewRaceInterface_setGameEvent(iface, kind);
                    cBENewRaceInterface_mNextLocation = mode;
                    cBENewRaceInterface_newRace.iLocationIdx = mode;
                    cBENewRaceInterface_bIsBamLocation = 1;
                    iface->submit();
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
                mRemovePDA = 0;
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
                    cUIEngine* ui = SSXApp->Game->UI;
                    cUIState* st = (cUIState*)cOVState_AUTOSAVE_cOVState_AUTOSAVE(
                        cMemMan_alloc(564, str_Save_Profile, 0x100, 0), mpEngine, 1);
                    st->mAttrState = (st->mAttrState & ~0x3F00) | 0x100;
                    cUIStateStack_pushSpecial(&ui->mStateStack, st, 0, 0);
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
                cBEInterface* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
                char* missionIface = (char*)cBE_getInterface_Fv(cBE_getBE(), 10);
                cBE_setState(0);
                cBENewRaceInterface_setGameEvent(iface, 0xC);
                cBENewRaceInterface_setGameMode(iface, 4);
                iface->submit();
                cGameModeMan_initGameMode(SSXApp->GameModeMan, 0xC);
                cAI_initFreeride(SSXApp->Game->AI);
                ((cBEMissionInterface*)missionIface)->nextSessionID = 1;
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
                mRemovePDA = 0;
                cUIEngine* ui = SSXApp->Game->UI;
                cUIState* st =
                    (cUIState*)cOVState_PROFILE_cOVState_PROFILE(cMemMan_alloc(636, str_Save_Profile, 0x100, 0),
                                                                 mpEngine, (unsigned char)mCurrentCtrl,
                                                                 0x18);
                st->mAttrState = (st->mAttrState & ~0x3F00) | 0x100;
                cUIStateStack_pushSpecial(&ui->mStateStack, st, 0, 0);
                break;
            }
            }
            break;
        case 1:
            if (mDialogType != 6) {
                cUIStateStack_pop(&mpEngine->mStateStack, 1);
            } else {
                mRemovePDA = 1;
                cGame_exit(SSXApp->Game, 0);
            }
            break;
        }
        break;
    case 6:
        if (mDialogType != ev) {
            Play_20D5A8(((char*)this));
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
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
 * cOVState_QUIT_vtbl, the dialog type 1 overlay (cOVStateManager_triggerOverlay type 0x18).
 */
void cOVState_QUIT::onUpdate()
{
    cOVState_onUpdate(this);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        int ok = gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0;
        if (ok) {
            int frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_TransitionOut_b));
            if (frame != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, frame, 1);
            }
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
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
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern char str_64replay[];
extern char str_ReplayMenuPopup[];
extern char str_showhelp[];
extern char str_hidetimeline[];
extern char str_showtimeline[];
extern char str_ReplayMenu[];
extern char str_option_savereplay[];


/**
 * Creates the replay overlay ("64replay"): hides the popup and help, shows the timeline toggle and sets the
 * camera name.
 *
 * When the replay was loaded from a save (cReplay Loaded), selects menu item 2 and hides and disables the
 * save
 * replay option.
 */
void cOVState_REPLAY::onCreateScreen()
{
    mHashName = GetHashValue32(str_64replay);
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((char*)this), GetHashValue32(str_64replay), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(((char*)this));
    mTimeLineHidden = 0;
    mShowPopupMenu = 0;
    cUIState_hideObjSafe(((char*)this), str_ReplayMenuPopup);
    cUIState_hideObjSafe(((char*)this), str_showhelp);
    if (mTimeLineHidden != 0) {
        cUIState_hideObjSafe(((char*)this), str_hidetimeline);
        cUIState_showObjSafe(((char*)this), str_showtimeline);
    } else {
        cUIState_showObjSafe(((char*)this), str_hidetimeline);
        cUIState_hideObjSafe(((char*)this), str_showtimeline);
    }
    mBottom_y = 245;
    mMinBottom_y = 245;
    mMaxBottom_y = 0x1E0;
    mBottom_ydiff = 0;
    cOVState_REPLAY_setupCameraName(((char*)this));
    if (SSXApp->Game->Replay->Loaded != 0) {
        mLoaded = 1;
    } else {
        mLoaded = 0;
    }
    cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ReplayMenu));
    *(int*)&obj->mAttrMenu |= 8;
    if (mLoaded != 0) {
        if (obj != 0) cUIMenu_setSelectedByIndex(obj, 2);
        cUIThing* saveReplayOption = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_option_savereplay));
        if (saveReplayOption != 0) {
            saveReplayOption->setVisible(0);
            saveReplayOption->setStatic(0);
        }
    }
}
#endif

