#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "camera/camera.h"
#include "visualfx/transitionman.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "sound/ssxaudio.h"
#include "main/gameviewman.h"
#include "main/sectionman.h"
#include "main/inputsync.h"
#include "main/gamestate.h"
#include "replay/frameptr.h"
#include "be/beintnewrace.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void cReplay_reset(void* p);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void cGameComm_gameStartGate(void* p);
extern "C" void cGame_resetWorld(void* self);
extern "C" void cReplay_startRace(void* p);
void cSSXAudio_CountdownStart(void* p);
extern "C" void cSSXAudio_CountdownBeep(void* p, int a1);
extern "C" void cGFGateState_runScript(cGFGateState* self, int a1);
int GetHashValue32(char* s);
extern char* cGameState_mpGame;
extern char str_StartlightBegin[];


/**
 * Starts the start-gate countdown: resets the replay for the race, puts the AI in state 4 and fires
 * "StartlightBegin".
 *
 * An online game first resets the world. Game modes 4-6 skip the gate and go straight to the next state.
 */
extern "C" void cGFGateState_gainFocus(cGFGateState* self)
{
    (*(cAI**)(cGameState_mpGame + 0xC))->ComputerActorsActive = 0;
    if (cGameComm_mGameComm != 0) {
        cGameComm_gameStartGate((char*)cGameComm_mGameComm);
    }
    cBE_setState(0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cGame_resetWorld(cGameState_mpGame);
    }
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cReplay_startRace(*(void**)(cGameState_mpGame + 0x28));
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGameMode;
    if (mode >= 4 && mode <= 6) {
        cGameState_gotoNextState(self);
        return;
    }
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 4);
    cSSXAudio_CountdownStart(cAudioInterface_bxAudio());
    cSSXAudio_CountdownBeep(cAudioInterface_bxAudio(), 3);
    cGFGateState_runScript(self, GetHashValue32(str_StartlightBegin));
    cUIStateStack_purgeNormalStates(*(char**)(cGameState_mpGame + 0x48) + 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_runScript);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" int cSectionMan_getWorldSectionIDFromBELocationID(void* world, int id);
extern "C" int cWScriptMan_isFunc(void* mgr, int id, int a);
extern "C" void cWScriptMan_runFunc(void* mgr, int id, int a);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs a world script of the race's location by hash, if the location has it.
 *
 * Gets the world section of the current race location, asks cWScriptMan_TheWScriptMan whether that section has the
 * script (cWScriptMan_isFunc) and runs it (cWScriptMan_runFunc).
 *
 * @param hashID  the script's hash (On Tour HashID)
 */
extern "C" void cGFGateState_runScript(cGFGateState* self, int hashID)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* world = SSXApp->Game->SectionMan;
    int id = cSectionMan_getWorldSectionIDFromBELocationID(world, *cBENewRaceInterface_getLocation(iface));
    if (cWScriptMan_isFunc(cWScriptMan_TheWScriptMan, id, hashID) != 0) {
        cWScriptMan_runFunc(cWScriptMan_TheWScriptMan, id, hashID);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_update);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" int cReplay_isPauseAI(void* p);
extern "C" void cGFGateState_runScript(cGFGateState* self, int a1);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_CountdownGateDrop(void* p);
void func_0028C8C0(void* self, int val);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void cSSXAudio_CountdownBeep(void* p, int secs);
extern char* cGameState_mpGame;
extern char str_StartgateOpen[];
extern char str_hud_c[];


// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_00234C68(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

/**
 * Runs the start-gate countdown: announces each second, and at zero opens the gate ("StartgateOpen") and
 * moves on.
 *
 * At zero it also tells the HUD (event 6) and the audio system, then goes to the next state. Nothing happens
 * while
 * a replay plays or in game mode 4.
 */
extern "C" void cGFGateState_update(cGFGateState* self)
{
    if (cReplay_isPauseAI(*(void**)(cGameState_mpGame + 0x28)) != 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode == 4) {
        return;
    }
    if (*(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x1C) <= 0) {
        cGFGateState_runScript(self, GetHashValue32(str_StartgateOpen));
        char* obj = (char*)cUIStateStack_findState(*(char**)(cGameState_mpGame + 0x48) + 0x18, GetHashValue32(str_hud_c));
        if (obj != 0) {
            sVEntry<int(void*, int, int)>* vtEntry = &(*(sVEntry<int(void*, int, int)>**)(obj + 8))[24];
            vtEntry->fn(vthis(obj, *vtEntry), 6, 0);
        }
        cSSXAudio_CountdownGateDrop(cAudioInterface_bxAudio());
        func_0028C8C0(cAudioInterface_bxAudio(), 0x78);
        cGameState_gotoNextState(self);
    }
    int frames = *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x1C);
    float secs = frames / 60;
    if (frames % 60 == 0) {
        cSSXAudio_CountdownBeep(cAudioInterface_bxAudio(), (int)ffloor_00234C68(secs));
    }
}
#endif

extern "C" void* cGameState_writeToReplayFrame(void* self);

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the gate game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFGateState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

extern "C" void* cGameState_readFromReplayFrame(void* self);

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the gate game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFGateState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFRidingState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern char* cGameState_mpGame;

/**
 * Starts the riding game state: lets the AI riders go and hands the game to the back end's racing state.
 *
 * Calls cAI_setAIState on the game's AI (mpGame + 0xC) with 5; unless the race's game mode (newRace.cGameMode) is
 * 4, sets the AI's +0x14 to 1 and +0x8C to 0. Then cBE_setState(1) and, when no replay is playing (Replay state outside
 * 1-9), purges the UI's normal states (cUIStateStack_purgeNormalStates). Slot +0x2C (gainFocus) of cGFRidingState_vtbl.
 */
extern "C" void cGFRidingState_gainFocus(void)
{
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 5);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode != 4) {
        cGame* game = (cGame*)cGameState_mpGame;
        game->AI->ComputerRidersActive = 1;
        game->AI->ComputerActorsActive = 0;
    }
    cBE_setState(1);
    cGame* gs = (cGame*)cGameState_mpGame;
    int n = *(int*)gs->Replay;
    int ok = n != 0 && n < 10;
    if (!ok) {
        cUIStateStack_purgeNormalStates((char*)gs->UI + 0x18);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFRidingState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the riding game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFRidingState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFRidingState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the riding game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFRidingState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_cGFTrackStartState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFTrackStartState_vtbl[];
extern char* cGameState_mpGame;

/**
 * Constructs the track-start game state.
 *
 * The cGameState part sets mNextState (+0x0) to 0, its vtable cGameState_vtbl at +0xC and clears the static
 * cGameState_mpGame; then the class stores cGFTrackStartState_vtbl, mNextState 4, the preload state (+0x10) 5 and
 * clears mStartupFlag (+0x4), +0x14, +0x18 and +0x1C. cGame_cGame builds it at +0x158 of the game.
 *
 * @return @p self
 */
extern "C" void* cGFTrackStartState_cGFTrackStartState(cGFTrackStartState* self)
{
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mVtbl = (void**)cGFTrackStartState_vtbl;
    self->mNextState = 4;
    self->mPreLoadState = 5;
    self->mStartupFlag = 0;
    self->mPlayFirstTimeBCVideo = 0;
    self->mPreLoadTicks = 0;
    self->mCheckTransportEnd = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_preloadInit);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cReplay_reset(void* p);
extern "C" void cSSXScriptEngine_PurgeTrack(void* a);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void cSSXScriptEngine_AsyncLoadTrack(void* a);
extern "C" void cAI_prepareToPurgeComputerRiders(void* self);
extern "C" void cAI_prepareToPurgeComputerActors(void* self);
extern "C" int cBENewPlayerInterface_isFirstTimeOnTrack(void* self, int index, int bit);
extern "C" void cSSXScriptEngine_AppendStartSequence(void* a, int b);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern char* cGameState_mpGame;

struct sVEntry_234F40 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sState_234F40 {
    int pad0[4];
    int f10;
    int f14;
    int f18;
};

/**
 * Prepares the track-start preload: resets the replay and NIS list, reloads the track scripts and sets up the
 * AI.
 *
 * Offline at locations 14-16 a first visit plays the location's intro (sets the audio flags and +0x14);
 * otherwise
 * the start sequence is appended to the NIS list.
 */
extern "C" void cGFTrackStartState_preloadInit(cGFTrackStartState* self)
{
    self->mPreLoadTicks = 0;
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cSSXScriptEngine_PurgeTrack(ScriptEngine);
    cSSXScriptEngine_AsyncLoadTrack(ScriptEngine);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    self->mPlayFirstTimeBCVideo = 0;
    int gamePath = cBENewRaceInterface_newRace.cGamePath;
    cAI_prepareToPurgeComputerRiders(*(void**)(cGameState_mpGame + 0xC));
    cAI_prepareToPurgeComputerActors(*(void**)(cGameState_mpGame + 0xC));
    if (gamePath == 0) {
        int locationId = SSXApp->Game->SectionMan->mBELocationID;
        if (locationId >= 14 && locationId <= 16) {
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
            char* sub = *(char**)(*(char**)(*(char**)(cGameState_mpGame + 0xC) + 0x40) + 0x18) + 0x6C0;
            sVEntry_234F40* vt = *(sVEntry_234F40**)sub;
            if (cBENewPlayerInterface_isFirstTimeOnTrack(iface, vt[7].fn(vthis(sub, vt[7])), locationId) != 0) {
                char* audio = (char*)cAudioInterface_bxAudio();
                *(int*)(audio + 0x5790) = 0;
                *(int*)(audio + 0x578C) = 1;
                *(int*)(audio + 0x6254) = 1;
                self->mPlayFirstTimeBCVideo = 1;
            }
        }
    } else {
        cSSXScriptEngine_AppendStartSequence(ScriptEngine, 0);
    }
    self->mPreLoadState = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_preloadUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the range bounds 6 / 11 as locals `first` / `last` (li + slt instead of slti); plain literals 98.25%.
// Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).
// Virtual calls in this unit go through these by hand.



// The race (seg/seg_1FBE38 / main/gamemode layout). It is a class with its vtable pointer at 0xCC.
class cRace {
public:
    char pad_0x0[0x14];
    int f14;                    // 0x14
    char pad_0x18[0x4];
    int f1C;                    // 0x1C frame counter
    char pad_0x20[0x20];
    void* players[12];          // 0x40
    char pad_0x70[0xC];
    int numPlayers;             // 0x7C
    int f80;                    // 0x80
    int f84;                    // 0x84
    char pad_0x88[0x4];
    int f8C;                    // 0x8C
    char pad_0x90[0x3C];
    virtual void v01();         // vtable pointer at 0xCC
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};


// The game (cGameState_mpGame_date, also SSXApp_date->game).
struct sGame {
    char pad_0x0[0xC];
    cRace* race;                // 0xC
    void* cache;                // 0x10
    char pad_0x14[0x14];
    int* f28;                   // 0x28
    char pad_0x2C[0x1C];
    char* f48;                  // 0x48 (+0x18: the list for cUIStateStack_purgeNormalStates / func_0039F9D8)
    char pad_0x4C[0x18];
    void* f64;                  // 0x64
    char pad_0x68[0x10];
    char* f78;                  // 0x78
    char pad_0x7C[0x8];
    void* cameras;              // 0x84
    char pad_0x88[0xC];
    void* f94;                  // 0x94 (the HUD for func_001F36D8 / cBXLoadIndicator_Show)
};

extern sGame* cGameState_mpGame_date __asm__("cGameState_mpGame");

// The game singleton (SSXApp_date).
struct sApp {
    char pad_0x0[0x78];
    void* f78;                  // 0x78
    char pad_0x7C[0x8];
    sGame* game;                // 0x84
    char pad_0x88[0x38];
    cGameModeMan* gm;           // 0xC0
};

extern sApp* SSXApp_date __asm__("SSXApp");

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" cSSXAudio* cAudioInterface_bxAudio(void);


// PORT: cSSXScriptEngine_ReadyNISList returns tNISList_ReadyList's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);

extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);

extern "C" void cBENewPlayerInterface_setRiderCharID(void* player, int i, int id);

extern "C" void cBENewPlayerInterface_setRiderCheatCharID(void* player, int i, int v);

extern "C" int cAI_isReadyToPurgeComputerRiders(void* p);

extern "C" int cAI_isAILoadedAndReady(void* p);

void cAI_resolveRace(void* p);

extern "C" int cSSXScriptEngine_GetNISListState(void* p, int a);

extern "C" void cGameModeMan_initGameMode(void* gm, int mode);

extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(void* self, int i, int a, int b, int c, int d, int f, int g);

extern "C" int cSSXScriptEngine_IsLoadTrackComplete(void* self);
extern "C" void cSSXScriptEngine_AddToNISListByVideoCategory(void* self, int i, int a, int b);
extern "C" void func_0028CD48(void* self);
extern "C" void func_002B3A70(void* self);
extern "C" int cAI_isReadyToPurgeComputerActors(void* p);
extern "C" void cAI_cleanupAfterRace(void* p);
extern "C" void cAI_cleanupActors(void* p);
extern "C" void cAI_initAIRiders(void* p);
extern "C" void cAI_loadRiders(void* p);
extern "C" void cAI_initAIActors(void* self, int n);
extern "C" void cAI_initFreeride(void* self);

static inline int isOne_235080(int v)
{
    return v == 1;
}

/**
 * Steps the track-start preload: track scripts and intro NIS, wait for the NIS lists, AI riders, loaded
 * check, intro time.
 *
 * Step 2 reinitialises the game mode (except events 6-11), sets the computer riders' characters from the game
 * mode
 * and rebuilds the AI riders and actors; step 3 loads riders until ready. An intro holds the last step for
 * 180
 * frames.
 */
extern "C" void cGFTrackStartState_preloadUpdate(cGFTrackStartState* self)
{
    self->mPreLoadTicks += 1;
    switch (self->mPreLoadState) {
    case 0: {
        if (cSSXScriptEngine_IsLoadTrackComplete(ScriptEngine) == 0)
            break;
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
        cSectionMan* sectionMan = (cSectionMan*)SSXApp_date->game->f78;
        int locationId = sectionMan->mBELocationID;
        int range = locationId >= 14 && locationId <= 16;
        if (self->mPlayFirstTimeBCVideo != 0) {
            if (locationId == 14) {
                cSSXScriptEngine_AddToNISListByVideoCategory(ScriptEngine, 1, 29, 3);
                func_0028CD48(cAudioInterface_bxAudio());
                func_002B3A70(&cAudioInterface_bxAudio()->MusicSys);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0, 3, 0, -1, -1, -1);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 1, 3, 0, -1, -1, -1);
            } else {
                cSSXScriptEngine_AddToNISListByVideoCategory(ScriptEngine, 1, locationId == 15 ? 0x1E : 0x1F, 3);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x10, 3, 0, -1, -1, -1);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 17, 3, 0, -1, -1, -1);
            }
            cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
        } else if (sectionMan->mWaitTransport != 0 && range) {
            cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x10, 3, 0, -1, -1, -1);
            cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 17, 3, 0, -1, -1, -1);
        }
        self->mPreLoadState = 1;
        break;
    }
    case 1: {
        long done = 0;
        if (!isOne_235080(cSSXScriptEngine_GetNISListState(ScriptEngine, 0))) {
            if (!isOne_235080(cSSXScriptEngine_GetNISListState(ScriptEngine, 1)))
                done = 1;
        }
        if (done)
            self->mPreLoadState = 2;
        break;
    }
    case 2: {
        // Modes first..last are already set up; anything else needs initGameMode.
        int first = 6, last = 11;
        if (cAI_isReadyToPurgeComputerRiders(cGameState_mpGame_date->race) == 0)
            break;
        if (cAI_isReadyToPurgeComputerActors(cGameState_mpGame_date->race) == 0)
            break;
        cBEInterface* newRace = cBE_getInterface_Fv(cBE_getBE(), 0);
        cBEInterface* newPlayer = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        cGameModeMan* gm = SSXApp_date->gm;
        int mode = beRaceDB->cGameEvent;
        if (mode < first || mode > last)
            cGameModeMan_initGameMode(gm, mode);
        int numAI = gm->iNumberAI - gm->iSimAI;
        int base = cGameState_mpGame_date->race->numPlayers;
        int i = 0;
        cBENewRaceInterface_setNumberAI(newRace, numAI);
        {
            newRace->submit();
        }
        for (; i < numAI; i++) {
            cBENewPlayerInterface_setRiderCharID(newPlayer, i + base, gm->iCharacters[base + i]);
            cBENewPlayerInterface_setRiderCheatCharID(newPlayer, i + base, gm->iCheatCharacters[base + i]);
        }
        {
            newPlayer->submit();
        }
        cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
        int quick = 0;
        cAI_cleanupAfterRace(cGameState_mpGame_date->race);
        cAI_cleanupActors(cGameState_mpGame_date->race);
        if (raceDB->cGameMode == 0)
            quick = raceDB->cGamePath == 0;
        cAI_initAIRiders(cGameState_mpGame_date->race);
        if (numAI == 0 && quick) {
            if (raceDB->cGameMode == 0)
                cAI_initAIActors(cGameState_mpGame_date->race, 0);
            else
                cAI_initAIActors(cGameState_mpGame_date->race, 2);
        }
        self->mPreLoadState = 3;
        break;
    }
    case 3: {
        if (cAI_isAILoadedAndReady(cGameState_mpGame_date->race)) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
            int gameMode = beRaceDB->cGameMode;
            if (gameMode == 0) {
                cAI_resolveRace(cGameState_mpGame_date->race);
                if (cGameState_mpGame_date->race->f80 > 0)
                    cGameState_mpGame_date->race->f14 = 1;
                if (cGameState_mpGame_date->race->f84 > 0)
                    cGameState_mpGame_date->race->f8C = 1;
            }
            if (gameMode == 5) {
                cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
                if (raceDB->cGamePath == 0)
                    cAI_initFreeride(cGameState_mpGame_date->race);
            }
            self->mPreLoadState = 4;
        } else {
            cAI_loadRiders(cGameState_mpGame_date->race);
        }
        break;
    }
    case 4:
        if (self->mPlayFirstTimeBCVideo != 0 && self->mPreLoadTicks < 180)
            break;
        self->mPreLoadState = 5;
        break;
    case 5:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_preloadCancel);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_PurgeTrack(void* a);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);

/**
 * Cancels the track-start preload: purges the script engine's track and marks the preload as finished.
 *
 * Calls cSSXScriptEngine_PurgeTrack() and cSSXScriptEngine_ResetNISList(ScriptEngine, 1), then sets the preload
 * state (+0x10, On Tour mPreLoadState) to 5, the value cGFTrackStartState_isPreloadComplete() tests.
 */
extern "C" void cGFTrackStartState_preloadCancel(cGFTrackStartState* self)
{
    cSSXScriptEngine_PurgeTrack(ScriptEngine);
    cSSXScriptEngine_ResetNISList(ScriptEngine, 1);
    self->mPreLoadState = 5;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_isPreloadComplete);
#ifdef SKIP_ASM
/**
 * Tells whether the track-start preload has finished: its preload state (+0x10) is 5.
 *
 * @return 1 when the preload is complete, otherwise 0
 */
extern "C" int cGFTrackStartState_isPreloadComplete(cGFTrackStartState* self)
{
    return self->mPreLoadState == 5;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void cAI_setAIState(void*, int);
extern "C" int cSSXScriptEngine_IsInTransportSequence(void* a);
extern "C" void cBERewardInterface_clearUserAwards(void* iface);
extern "C" void cWScriptMan_initMissions(void* a);
extern "C" void cWScriptMan_runGlobalScripts(void* a);
extern "C" int cBENewRaceInterface_getLocationID(void* race);
extern "C" int cBENewRaceInterface_getLocationType(void* race, int a);
extern "C" void cBENewPlayerInterface_setLastHub(void* player, int a, int b);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void func_002B3A98(void* p);
extern "C" void func_002A4B68(void* p);
extern char* cGameState_mpGame;
extern void* cWScriptMan_TheWScriptMan;


struct sRaceMan_2355C0 {
    char pad0[0x14];
    int f14;
};


static inline int inRange_2355C0(int t)
{
    return t >= 14 && t <= 16;
}

static inline int isMode2_2355C0(void* s)
{
    return *(int*)((char*)s + 0x550) == 2;
}

/**
 * Starts a track: picks the start flow, initialises the missions and global scripts, and plays the start NIS
 * lists.
 *
 * Offline outside locations 14-16 the state goes to step 4; otherwise to step 1. Game mode 4 puts the AI in
 * state 3
 * and disables its countdown flag. A location visited for the first time is recorded for player 1.
 */
extern "C" void cGFTrackStartState_gainFocus(cGFTrackStartState* self)
{
    self->mCheckTransportEnd = 0;
    if (cSSXScriptEngine_IsInTransportSequence(ScriptEngine))
        self->mCheckTransportEnd = 1;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBERewardInterface_clearUserAwards(cBE_getInterface_Fv(cBE_getBE(), 13));
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int flag = beRaceDB->cGamePath;
    int mode = beRaceDB->cGameMode;
    if (flag == 0 && !inRange_2355C0(SSXApp->Game->SectionMan->mBELocationID)) {
        self->mStartupFlag = 0;
        self->mNextState = 4;
    } else {
        self->mNextState = 1;
        self->mStartupFlag = flag == 0;
    }
    if (mode == 4)
        cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 3);
    cBE_setState(0);
    cWScriptMan_initMissions(cWScriptMan_TheWScriptMan);
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    if (cBENewRaceInterface_getLocationType(race, cBENewRaceInterface_getLocationID(race)) == 1) {
        cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setLastHub(player, 0, cBENewRaceInterface_getLocationID(race));
        player->submit();
    }
    (*(sRaceMan_2355C0**)(cGameState_mpGame + 0xC))->f14 = mode != 4;
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 0);
    if (flag) {
        self->mNextState = 1;
        self->mStartupFlag = 0;
        if (isMode2_2355C0(ScriptEngine)) {
            cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 0, 1);
            if (isMode2_2355C0(ScriptEngine))
                cSSXScriptEngine_PlayNISList(ScriptEngine, 0, 0, 1);
        }
    } else if (self->mPlayFirstTimeBCVideo) {
        cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 1, 0);
    }
    func_002B3A98(&cAudioInterface_bxAudio()->MusicSys);
    func_002A4B68(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_update);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_IsInTransportSequence(void* a);
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* a, int b);
extern "C" void cGameState_gotoNextState(void* self);

/**
 * Waits for the track-start NIS to finish, then moves the game to its next state.
 *
 * While the transport-end check flag (+0x1C, On Tour mCheckTransportEnd) is set and the script engine has left
 * its transport sequence, sets the word at SectionMan + 0x1A4 and clears the flag. Calls
 * cGameState_gotoNextState() once NIS list 1 reports play state 0.
 */
extern "C" void cGFTrackStartState_update(cGFTrackStartState* self)
{
    if (self->mCheckTransportEnd != 0 && cSSXScriptEngine_IsInTransportSequence(ScriptEngine) == 0) {
        SSXApp->Game->SectionMan->m_bTriggerSkySwap = 1;
        self->mCheckTransportEnd = 0;
    }
    if (cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 1) == 0) {
        cGameState_gotoNextState(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_loseFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWScriptMan_purgeMissions(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern "C" void cReplay_reset(void* p);
extern "C" void cReplay_startRace(void* p);
extern "C" void cBENewPlayerInterface_setFirstTimeOnTrack(void* iface, int a1, int a2, int a3);
extern "C" void cGameModeMan_riderCheckpointHit(void* gm, int a1, int a2);


struct sSlot_235868 {
    char pad[0x50];
};
struct sTbl_235868 {
    char pad[0x250];
    struct { int f0; char pad[0x4C]; } slot[2];
};

static inline int otherSide_235868(char* p)
{
    return *(int*)(p + 0xB0) == 0;
}

/**
 * Finishes the track start: sets up the missions, restarts the replay recording and resets heat flags.
 *
 * Offline it records the location as visited for player 1; game modes 5 and 6 reset the heat in the game mode
 * manager; a split-screen game clears the other view's cache flag.
 */
extern "C" void cGFTrackStartState_loseFocus(cGFTrackStartState* self)
{
    cWScriptMan_purgeMissions(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(cWScriptMan_TheWScriptMan);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cReplay_startRace(*(void**)(cGameState_mpGame + 0x28));
    self->mPlayFirstTimeBCVideo = 0;
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    if (beRaceDB->cGamePath == 0) {
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setFirstTimeOnTrack(player, 0, 0, *cBENewRaceInterface_getLocation(race));
    }
    int gameMode = beRaceDB->cGameMode;
    if (gameMode == 5) {
        goto restart;
    }
    if (gameMode == 6) {
    restart:
        cGameModeMan_riderCheckpointHit(SSXApp->GameModeMan, 0, 0);
    }
    cGame* game = SSXApp->Game;
    cGameViewMan* x = game->ViewMan;
    if ((int)x->m_uiNumView == 1) {
        sTbl_235868* viewTbl = *(sTbl_235868**)game->World;
        viewTbl->slot[otherSide_235868((char*)x->PlayerCamera[0])].f0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_writeToReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_writeToReplayFrame_2(void* self, void* obj) __asm__("cGameState_writeToReplayFrame");

/**
 * Track-start state: saves its replay data.
 *
 * Writes the base game-state part (cGameState_writeToReplayFrame), then the words at +0x10, +0x14 and +0x18 through the stream's
 * virtual slot 1 (write).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFTrackStartState_writeToReplayFrame(cGFTrackStartState* self, cBxReplayFrame* stream)
{
    cGameState_writeToReplayFrame_2(self, stream);
    stream->write((const char*)&self->mPreLoadState, 4);
    stream->write((const char*)&self->mPlayFirstTimeBCVideo, 4);
    stream->write((const char*)&self->mPreLoadTicks, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_readFromReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_readFromReplayFrame_2(void* self, void* obj) __asm__("cGameState_readFromReplayFrame");

/**
 * Track-start state: restores its replay data.
 *
 * Reads the base game-state part (cGameState_readFromReplayFrame), then the words at +0x10, +0x14 and +0x18 through the stream's
 * virtual slot 2 (read).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFTrackStartState_readFromReplayFrame(cGFTrackStartState* self, cBxReplayFrame* stream)
{
    cGameState_readFromReplayFrame_2(self, stream);
    stream->read((char*)&self->mPreLoadState, 4);
    stream->read((char*)&self->mPlayFirstTimeBCVideo, 4);
    stream->read((char*)&self->mPreLoadTicks, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void cWScriptMan_failActiveLocalMission(void* self);
extern "C" void cGame_partialRestart(void* self);
extern "C" void cAI_prepareToPurgeComputerRiders(void* self);
extern "C" void cAI_prepareToPurgeComputerActors(void* self);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void cSSXScriptEngine_AppendStartSequence(void* a, int b);
// PORT: cSSXScriptEngine_ReadyNISList returns tNISList_ReadyList's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern "C" void cSSXScriptEngine_PlayTransportSequence(void* a, int b, int c);
extern "C" void cBXLoadIndicator_Show(void* a, int b);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* player, int i, int id);
extern "C" void cBENewPlayerInterface_setRiderCheatCharID(void* player, int i, int v);
extern char* cGameState_mpGame;
extern void* cWScriptMan_TheWScriptMan;




/**
 * Starts the next heat: restarts the heat and the game, re-picks the computer riders and queues the start
 * sequence.
 *
 * Game modes 5 and 6, and online games, skip straight to step 3. Race mode also plays the transport sequence.
 */
extern "C" void cGFNextHeatState_gainFocus(cGFNextHeatState* self)
{
    cBE_setState(0);
    cWScriptMan_failActiveLocalMission(cWScriptMan_TheWScriptMan);
    cBEInterface* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* gm = SSXApp->GameModeMan;
    cGameModeMan_restartHeat(gm);
    cGame_partialRestart(SSXApp->Game);
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int gameMode = beRaceDB->cGameMode;
    if (gameMode == 5)
        goto bail;
    if (gameMode == 6)
        goto bail;
    if (beRaceDB->cGamePath != 0) {
    bail:
        self->CurrentNextHeatState = 3;
        return;
    }
    int n = gm->iNumberAI - gm->iSimAI;
    int base = *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x7C);
    int i = 0;
    cBENewRaceInterface_setNumberAI(race, n);
    {
        race->submit();
    }
    for (; i < n; i++) {
        cBENewPlayerInterface_setRiderCharID(player, i + base, gm->iCharacters[base + i]);
        cBENewPlayerInterface_setRiderCheatCharID(player, i + base, gm->iCheatCharacters[base + i]);
    }
    {
        player->submit();
    }
    cAI_prepareToPurgeComputerRiders(SSXApp->Game->AI);
    if (n >= 2)
        cAI_prepareToPurgeComputerActors(SSXApp->Game->AI);
    self->PlayTransport = 0;
    cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
    if (raceDB->cGameMode == 0) {
        void* sc = ScriptEngine;
        self->PlayTransport = 1;
        cSSXScriptEngine_PlayTransportSequence(sc, 1, 0);
    }
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cSSXScriptEngine_AppendStartSequence(ScriptEngine, gm->bIsLastHeat ? 3 : 2);
    cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
    cBXLoadIndicator_Show(SSXApp->Game->LoadIndicator, 3);
    self->WaitTicks = 0;
    self->CurrentNextHeatState = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_update);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
struct sVec4_236960;


struct sCacheTbl_235CC8 {
    char pad[0x250];
    struct { int f0; char pad[0x4C]; } slot[2];
};

struct cRiderObj_235CC8 {
    int f0, f4, f8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual int isBusy();
};

extern "C" int cAI_isReadyToPurgeComputerRiders(void* p);
extern "C" void cAI_cleanupAfterRace(void* p);
extern "C" void cAI_cleanupActors(void* p);
extern "C" void cAI_initAIRiders(void* p);
extern "C" int cAI_isAILoadedAndReady(void* p);
void cAI_resolveRace(void* p);
extern "C" void cAI_loadRiders(void* p);
extern "C" t4Vector cRider_getEventStartPos(void* rider);
extern "C" void cWorldView_setFocus(void* cache, int idx, const t4Vector* pos, float radius);
extern "C" int cWorldView_getTestFocus(void* cache, int idx, t4Vector* pos, float* dist);
extern "C" int cSSXScriptEngine_GetNISListState(void* p, int a);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* p);
extern "C" void cSSXScriptEngine_EndTransportSequence(void* p);
extern "C" void cGameState_gotoNextState(void* self);
void* cGame_getTransitionMan(void* self, int id);
extern char* cGameState_mpGame;

/**
 * Steps the next-heat setup: rebuild the AI riders, place the world-cache focus at the first rider, end the
 * transport, continue.
 *
 * Step 2 waits until the focus is 20000 units away, 120 frames have passed and the transport NIS and rider
 * are idle.
 */
extern "C" void cGFNextHeatState_update(cGFNextHeatState* self)
{
    self->WaitTicks += 1;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cGame* game = SSXApp->Game;
    int state = self->CurrentNextHeatState;
    char** cache = (char**)game->World;
    int idx = game->ViewMan->PlayerCamera[0]->StreamFocusID == 0;
    switch (state) {
    case 0:
        if (cAI_isReadyToPurgeComputerRiders(game->AI)) {
            cAI_cleanupAfterRace(SSXApp->Game->AI);
            if (cBENewRaceInterface_newRace.iNumberAI >= 2)
                cAI_cleanupActors(SSXApp->Game->AI);
            cAI_initAIRiders(SSXApp->Game->AI);
            self->CurrentNextHeatState = 1;
        }
        break;
    case 1:
        if (cAI_isAILoadedAndReady(game->AI)) {
            cAI_resolveRace(SSXApp->Game->AI);
            (*(cAI**)(cGameState_mpGame + 0xC))->ComputerRidersActive = 1;
            ((sCacheTbl_235CC8*)*cache)->slot[idx].f0 = 1;
            cPlayer* player = SSXApp->Game->AI->Players[0];
            t4Vector pos = cRider_getEventStartPos(player->mRider);
            cWorldView_setFocus(*cache + 0x10, idx, (const t4Vector*)&pos, 45000.0f);
            self->CurrentNextHeatState = 2;
        } else {
            cAI_loadRiders(SSXApp->Game->AI);
        }
        break;
    case 2:
        if (self->PlayTransport != 0) {
            t4Vector focusPos;
            float dist;
            cWorldView_getTestFocus(*cache + 0x10, idx, (t4Vector*)&focusPos, &dist);
            if (dist < 20000.0f)
                break;
            if (self->WaitTicks < 120)
                break;
            if (cSSXScriptEngine_GetNISListState(ScriptEngine, 0) == 1)
                break;
            cTransitionMan* transitionMan = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (cSSXScriptEngine_IsActiveNISInTransition(ScriptEngine) != 0)
                break;
            if (((cRiderObj_235CC8*)(&transitionMan->mObjType))->isBusy() != 0)
                break;
            if (transitionMan->mbPlayingTransition != 0)
                break;
            cSSXScriptEngine_EndTransportSequence(ScriptEngine);
        }
        self->CurrentNextHeatState = 3;
        break;
    case 3:
        cGameState_gotoNextState(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_loseFocus);
#ifdef SKIP_ASM
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);

/**
 * Hides the game's load indicator when the next-heat state loses focus.
 *
 * Calls cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 3).
 */
extern "C" void cGFNextHeatState_loseFocus(void)
{
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 3);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_writeToReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_writeToReplayFrame_2(void* self, void* obj) __asm__("cGameState_writeToReplayFrame");

/**
 * Next-heat state: saves its replay data.
 *
 * Writes the base game-state part (cGameState_writeToReplayFrame), then the words at +0x10, +0x14 and +0x18 through the stream's
 * virtual slot 1 (write).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFNextHeatState_writeToReplayFrame(cGFNextHeatState* self, cBxReplayFrame* stream)
{
    cGameState_writeToReplayFrame_2(self, stream);
    stream->write((const char*)&self->CurrentNextHeatState, 4);
    stream->write((const char*)&self->WaitTicks, 4);
    stream->write((const char*)&self->PlayTransport, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_readFromReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_readFromReplayFrame_2(void* self, void* obj) __asm__("cGameState_readFromReplayFrame");

/**
 * Next-heat state: restores its replay data.
 *
 * Reads the base game-state part (cGameState_readFromReplayFrame), then the words at +0x10, +0x14 and +0x18 through the stream's
 * virtual slot 2 (read).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFNextHeatState_readFromReplayFrame(cGFNextHeatState* self, cBxReplayFrame* stream)
{
    cGameState_readFromReplayFrame_2(self, stream);
    stream->read((char*)&self->CurrentNextHeatState, 4);
    stream->read((char*)&self->WaitTicks, 4);
    stream->read((char*)&self->PlayTransport, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_failActiveLocalMission(void* self);
extern "C" void cGameModeMan_initGameMode(void* gm, int mode);
extern "C" void cGame_partialRestart(void* self);
extern "C" void cRider_putAtStartpoint(void* self, int a1, int a2);
extern "C" void cRider_cleanupAfterPutAt(void* self, int notify);
void* cGame_getTransitionMan(void* self, int id);
// PORT: cTransitionObj_operator_new is the wake pool's allocator; it ignores the size the caller passes
extern "C" void* cTransitionObj_operator_new_sz(int size) __asm__("cTransitionObj_operator_new");
void* cTransitionObj_cTransitionObj(void* self);
extern "C" void cTransitionMan_StartTransition(void* self, int a1, int a2, void* a3, float f0, float f1, int a4, int a5, float f2);
extern void* cFade_TObj_vtbl[];


/**
 * Teleports the rider for a new session: free-ride event 12, a partial restart at the mission start point, a
 * white fade.
 */
extern "C" void cGFSessionTeleportState_gainFocus(cGFSessionTeleportState* self)
{
    void* gm = SSXApp->GameModeMan;
    cWScriptMan_failActiveLocalMission(cWScriptMan_TheWScriptMan);
    float one = 1.0f;
    cGameModeMan_initGameMode(gm, 0xC);
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 10);
    cGame_partialRestart(SSXApp->Game);
    cRider_putAtStartpoint(SSXApp->Game->AI->Players[0]->mRider, *(int*)(iface + 0x10), 2);
    cRider_cleanupAfterPutAt(SSXApp->Game->AI->Players[0]->mRider, 1);
    void* transitionMan = cGame_getTransitionMan(SSXApp->Game, 0);
    char* fade = (char*)cTransitionObj_operator_new_sz(20);
    tARGB colour;
    colour.A = one;
    colour.R = one;
    colour.G = one;
    colour.B = one;
    cTransitionObj_cTransitionObj(fade);
    *(void***)fade = cFade_TObj_vtbl;
    *(tARGB*)(fade + 4) = colour;
    cTransitionMan_StartTransition(transitionMan, 1, 0, fade, 0.0f, 0.0f, 0, 0, one);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_update);
#ifdef SKIP_ASM
extern "C" void cGame_setState(void* self, int a, int b, int refresh);

/**
 * Updates the session-teleport state with one fixed call on the game object.
 *
 * Calls cGame_setState(SSXApp->Game, 1, 0, 0), which stores 1 and 0 in two game fields (offsets 0x210 and
 * 0x21C) and skips its refresh step.
 */
extern "C" void cGFSessionTeleportState_update(void)
{
    cGame_setState(SSXApp->Game, 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the session-teleport game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFSessionTeleportState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the session-teleport game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFSessionTeleportState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_cGFUserSelectState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFUserSelectState_vtbl[];
extern char* cGameState_mpGame;

/**
 * Constructs the user-select game state.
 *
 * The cGameState part sets mNextState to 0, its vtable cGameState_vtbl and clears cGameState_mpGame; then the class stores
 * cGFUserSelectState_vtbl, mNextState 4, the next location (+0x18) 23 and clears mStartupFlag (+0x4), the overlay-done
 * flag (+0x8) and +0x14. cGame_cGame builds it at +0x1B8 of the game.
 *
 * @return @p self
 */
extern "C" void* cGFUserSelectState_cGFUserSelectState(cGFUserSelectState* self)
{
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mVtbl = (void**)cGFUserSelectState_vtbl;
    self->mNextState = 4;
    self->mNextLocation = 23;
    self->mStartupFlag = 0;
    self->mOverlayDone = 0;
    self->mMode = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cAI_setAIState(void*, int);
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cGame_partialRestart(void* self);
extern "C" void cWScriptMan_failActiveLocalMission(void* self);
extern "C" void cRider_initAlways(void* rider);
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(void* self, int i, int a, int b, int c, int d, int f, int g);
// PORT: cSSXScriptEngine_ReadyNISList returns tNISList_ReadyList's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);
extern char* cGameState_mpGame;
extern void* cWScriptMan_TheWScriptMan;
extern int cBENewRaceInterface_mNextLocation;

static inline int isOne_6250(int m)
{
    return m == 1;
}

/**
 * Starts the user-select flow (choosing where to go next) and plays its NIS.
 *
 * @param mode  how it was entered (On Tour startupFlag): 0 the session menu, 1 a saved target location, 2 or
 *              3 the
 *              transport map
 */
extern "C" void cGFUserSelectState_gainFocus(cGFUserSelectState* self, int mode)
{
    cWScriptMan_failActiveLocalMission(cWScriptMan_TheWScriptMan);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 3);
    *(int*)(*(char**)(*(char**)(*(char**)(cGameState_mpGame + 0xC) + 0x40) + 0x18) + 0x2F8) = 0;
    self->mNextLocation = 23;
    self->mCurrState = 0;
    self->mMode = mode;
    self->mOverlayDone = 0;
    self->mWaitingForOverlay = 0;
    if (mode == 1) {
        self->mOverlayDone = mode;
        cBE_getInterface_Fv(cBE_getBE(), 0);
        self->mNextLocation = cBENewRaceInterface_mNextLocation;
    }
    if (self->mNextLocation == *cBENewRaceInterface_getLocation(iface))
        cGame_partialRestart(SSXApp->Game);
    int curMode = self->mMode;
    if (curMode == 0) {
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 22, 3, 0, -1, -1, -1);
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 23, 1, 0, -1, -1, -1);
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
        cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 1, 0);
    } else if (curMode >= 0) {
        if (curMode < 4) {
            if (!isOne_6250(curMode))
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 11, 3, 0, -1, -1, -1);
            cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
            cRider_initAlways(SSXApp->Game->AI->Players[0]->mRider);
            cSSXScriptEngine_PlayNISList(ScriptEngine, 1, self->mMode == 2, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_update);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cBXLoadIndicator_Show(void* a, int b);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern "C" void cSectionMan_load(void* self, int a, int b);
extern "C" void cGame_setState(void* self, int a, int b, int refresh);
extern "C" void cGameState_gotoNextState(void* self);
void cSSXScriptEngine_DisallowNISListSkip(void* self, int a);
extern "C" int cSSXScriptEngine_GetNISListScriptCategory(void* self, int a, int b);
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* a, int b);
extern "C" void cSSXScriptEngine_PlayTransportSequence(void* a, int b, int c);
extern int cUITemplate_MAP_mMapStyle;
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern int D_004428F8[];

struct sIface_6418 {
    char pad0[0x10];
    int f10;
};


struct sGS_6418 {
    int f0;
    int f4;
    int f8;             // 0x08
    int fC;
    int state;          // 0x10
    int mode;           // 0x14
    int f18;            // 0x18
    int f1C;            // 0x1C
};

static inline void push2_6418()
{
    cPauseState_Stack[++cPauseState_StackIdx] = 2;
    cPauseState_Flags = D_004428F8[0];
}

static inline int atLeast_6418(int n)
{
    return n >= 0x11;
}

static inline int isOne_6418(int m)
{
    return m == 1;
}

static inline void pop_6418()
{
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
}

/**
 * Steps the user-select flow: open the session overlay or transport map, then transport or teleport to the
 * choice.
 *
 * Choosing the current location (outside 14-16) asks the game flow to teleport (0xF); otherwise it plays the
 * transport sequence, then loads the target location (cSectionMan load, mode 7) during it.
 */
extern "C" void cGFUserSelectState_update(void* self)
{
    // Retail builds `flag` as a copy of a register holding 1, then `movn flag, zero, (D != 4)`: a condition the
    //       compiler
    // folded to the constant 1 (a local flag, e.g. a feature toggle). A literal 1 gives a run-time sltiu instead
    //       (96.81).
    int one = 1;
    int ns;
    switch (*(int*)((char*)self + 0x10)) {
    case 0: {
        int nisListInfo = cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 1);
        if (!(nisListInfo == 4 || nisListInfo == 5 || nisListInfo == 0))
            return;
        int curMode = *(int*)((char*)self + 0x14);
        if (curMode == 0) {
            cOVStateManager_triggerOverlay(0x1F);
            push2_6418();
            *(int*)((char*)self + 0x8) = 0;
        } else if (curMode == 2) {
            int flag = 0;
            if (atLeast_6418(*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))))
                flag = one && cBENewRaceInterface_newRace.cGameMode == 4;
            if (flag)
                cUITemplate_MAP_mMapStyle = 3;
            else
                cUITemplate_MAP_mMapStyle = 4;
            cOVStateManager_triggerOverlay(33);
            push2_6418();
            *(int*)((char*)self + 0x8) = 0;
        }
        ns = 1;
        break;
    }
    case 1:
        if (*(int*)((char*)self + 0x8) == 0)
            return;
        cSSXScriptEngine_DisallowNISListSkip(ScriptEngine, 0x1E);
        if (!isOne_6418(*(int*)((char*)self + 0x14)))
            pop_6418();
        if (*(int*)((char*)self + 0x14) == 0) {
            ns = 2;
            break;
        }
        {
            int curLocation = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
            int destLocation = *(int*)((char*)self + 0x18);
            int curInBackcountry = (unsigned)(curLocation - 0xE) < 3;
            int destInBackcountry = (unsigned)(destLocation - 0xE) < 3;
            if (destLocation == curLocation && !destInBackcountry) {
                *(int*)((char*)self + 0x10) = 3;
                ((sIface_6418*)cBE_getInterface_Fv(cBE_getBE(), 10))->f10 = one;
                cGame_setState(SSXApp->Game, 0xF, 0, 0);
                return;
            }
            if (destInBackcountry && cBENewRaceInterface_newRace.cGamePath == 0)
                *(int*)((char*)self + 0x8) = 0;
            cSSXScriptEngine_PlayTransportSequence(ScriptEngine, !destInBackcountry && !curInBackcountry, 1);
            ns = 2;
        }
        break;
    case 2:
        if (*(int*)((char*)self + 0x14) != 0) {
            int nisListInfo = cSSXScriptEngine_GetNISListScriptCategory(ScriptEngine, 1, 0);
            if (nisListInfo == 0xE || nisListInfo == 20) {
                if (*(int*)((char*)self + 0x8) == 0) {
                    if (*(int*)((char*)self + 0x1C) != 0)
                        return;
                    *(int*)((char*)self + 0x1C) = one;
                    *(int*)((char*)self + 0x8) = one;
                    return;
                }
                *(int*)((char*)self + 0x1C) = 0;
                ((cSectionMan*)(void*)SSXApp->Game->SectionMan)->mWaitTransport = one;
                cSectionMan_load(SSXApp->Game->SectionMan, *(int*)((char*)self + 0x18), 7);
                ns = 3;
                break;
            }
            if (nisListInfo == 13 || nisListInfo == 19)
                cBXLoadIndicator_Show(SSXApp->Game->LoadIndicator, 0);
            return;
        }
        cGameState_gotoNextState(self);
        ns = 3;
        break;
    default:
        return;
    }
    *(int*)((char*)self + 0x10) = ns;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_writeToReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_writeToReplayFrame_2(void* self, void* obj) __asm__("cGameState_writeToReplayFrame");

/**
 * User-select state: saves its replay data.
 *
 * Writes the base game-state part (cGameState_writeToReplayFrame), then the words at +0x10, +0x14, +0x8 and +0x18 through the stream's
 * virtual slot 1 (write).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFUserSelectState_writeToReplayFrame(cGFUserSelectState* self, cBxReplayFrame* stream)
{
    cGameState_writeToReplayFrame_2(self, stream);
    stream->write((const char*)&self->mCurrState, 4);
    stream->write((const char*)&self->mMode, 4);
    stream->write((const char*)&self->mOverlayDone, 4);
    stream->write((const char*)&self->mNextLocation, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: cGameState_readFromReplayFrame also reads $a1 (the stream); bound with its real arity.
void* cGameState_readFromReplayFrame_2(void* self, void* obj) __asm__("cGameState_readFromReplayFrame");

/**
 * User-select state: restores its replay data.
 *
 * Reads the base game-state part (cGameState_readFromReplayFrame), then the words at +0x10, +0x14, +0x8 and +0x18 through the stream's
 * virtual slot 2 (read).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFUserSelectState_readFromReplayFrame(cGFUserSelectState* self, cBxReplayFrame* stream)
{
    cGameState_readFromReplayFrame_2(self, stream);
    stream->read((char*)&self->mCurrState, 4);
    stream->read((char*)&self->mMode, 4);
    stream->read((char*)&self->mOverlayDone, 4);
    stream->read((char*)&self->mNextLocation, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_cGFSectionLoadState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFSectionLoadState_vtbl[];
extern char* cGameState_mpGame;

/**
 * Constructs the section-load game state (vtable cGFSectionLoadState_vtbl).
 *
 * The cGameState part sets mNextState to 0, its vtable cGameState_vtbl and clears cGameState_mpGame; then stores cGFSectionLoadState_vtbl,
 * mNextState 10 and clears mStartupFlag (+0x4). cGame_cGame builds it at +0x178 of the game.
 *
 * @return @p self
 */
extern "C" void* cGFSectionLoadState_cGFSectionLoadState(cGFSectionLoadState* self)
{
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mVtbl = (void**)cGFSectionLoadState_vtbl;
    self->mNextState = 10;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void cReplay_reset(void* p);
extern "C" void cBENewPlayerInterface_setIsFirstTimeWC(void* iface, int i, int a);
extern char* cGameState_mpGame;

struct sRace002368A0 {
    char pad0[0x30];
    int count;
    char pad34[0x15];
    signed char flag49;
};

/**
 * Starts the section-load game state: back end state 0, replay reset, and on the career path resets each rider's entry.
 *
 * Calls cBE_setState(0) and cReplay_reset on the game's replay; when the race's cGamePath is 0, calls
 * cBENewPlayerInterface_setIsFirstTimeWC(player interface, i, 0) for each of the race's iNumberRiders. Clears the sub-state (+0x10). Slot +0x2C
 * (gainFocus) of cGFSectionLoadState_vtbl.
 */
extern "C" void cGFSectionLoadState_gainFocus(cGFSectionLoadState* self)
{
    cBE_setState(0);
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        int i = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        for (; i < cBENewRaceInterface_newRace.iNumberRiders; i++) {
            cBENewPlayerInterface_setIsFirstTimeWC(iface, i, 0);
        }
    }
    self->mSubState = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_update);
#ifdef SKIP_ASM

struct cRiderObj_236960 {
    int f0, f4, f8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual int isBusy();
};

extern "C" int cSectionMan_isLoaded(void* self, int idx);
extern "C" void cSectionMan_activate(void* self, int idx, int mode);
extern "C" t4Vector cRider_GetNISEndPosition(void* rider);
extern "C" void cWorldView_setFocus(void* cache, int idx, const t4Vector* pos, float radius);
extern "C" int cWorldView_getTestFocus(void* cache, int idx, t4Vector* pos, float* dist);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* p);
extern "C" void cSSXScriptEngine_EndTransportSequence(void* p);

/**
 * Steps the section-load game state while the section manager waits for a transport.
 *
 * Does nothing unless the section manager's mWaitTransport (+0x1C8) is set. Sub-state 0: once the location's section
 * (mBELocationID) is loaded, puts the world view's focus for the inactive view at the first rider's NIS end position
 * (cRider_GetNISEndPosition) with radius 45000 and goes to 1. Sub-state 1: waits until the test focus is at least 20000
 * away, the first transition manager (cGame_getTransitionMan) is idle and no NIS is in transition, then ends the transport
 * sequence (cSSXScriptEngine_EndTransportSequence), clears mWaitTransport, activates the location in mode 7
 * (cSectionMan_activate) and goes to 2. Slot +0x34 (update) of cGFSectionLoadState_vtbl.
 */
extern "C" void cGFSectionLoadState_update(cGFSectionLoadState* self)
{
    cGame* appGame = SSXApp->Game;
    cSectionMan* sectionMan = appGame->SectionMan;
    if (sectionMan->mWaitTransport == 0) {
        return;
    }
    int state = self->mSubState;
    char** cache = (char**)appGame->World;
    int idx = appGame->ViewMan->PlayerCamera[0]->StreamFocusID == 0;
    switch (state) {
    case 0:
        if (cSectionMan_isLoaded(sectionMan, sectionMan->mBELocationID) != 0) {
            cPlayer* aiPlayer = SSXApp->Game->AI->Players[0];
            t4Vector pos = cRider_GetNISEndPosition(aiPlayer->mRider);
            char* viewEntry = *cache + idx * 80;
            *(int*)(viewEntry + 0x250) = 1;
            cWorldView_setFocus(*cache + 0x10, idx, &pos, 45000.0f);
            self->mSubState = 1;
        }
        break;
    case 1: {
        t4Vector focusPos;
        float dist;
        cWorldView_getTestFocus(*cache + 0x10, idx, &focusPos, &dist);
        if (dist < 20000.0f) {
            break;
        }
        cTransitionMan* transitionMan = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
        if (((cRiderObj_236960*)(&transitionMan->mObjType))->isBusy() != 0) {
            break;
        }
        if (transitionMan->mbPlayingTransition != 0) {
            break;
        }
        if (cSSXScriptEngine_IsActiveNISInTransition(ScriptEngine) != 0) {
            break;
        }
        cSSXScriptEngine_EndTransportSequence(ScriptEngine);
        sectionMan->mWaitTransport = 0;
        cSectionMan_activate(sectionMan, sectionMan->mBELocationID, 7);
        self->mSubState = 2;
        break;
    }
    case 2:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the section-load game state's loseFocus (slot +0x3C of cGFSectionLoadState_vtbl).
 */
void cGFSectionLoadState_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the section-load game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFSectionLoadState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSectionLoadState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the section-load game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFSectionLoadState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTransportWaitState_loseFocus);
#ifdef SKIP_ASM
class cRace236B38 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void restart(int);
};
extern char* cGameState_mpGame;

/**
 * Ends the transport-wait game state: calls virtual slot 5 of the game's AI (mpGame + 0xC, vtable at +0xCC) with 3.
 *
 * Slot +0x3C (loseFocus) of cGFTransportWaitState_vtbl.
 */
extern "C" void cGFTransportWaitState_loseFocus(void)
{
    (*(cRace236B38**)(cGameState_mpGame + 0xC))->restart(3);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTransportWaitState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the transport-wait game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFTransportWaitState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTransportWaitState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the transport-wait game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFTransportWaitState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_gainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBE_setState(int state);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
extern "C" void cOVStateManager_gotoOverlay(int mode);
struct sMgr_6BB0 {
    char pad0[0x64];
    void* world;    // 0x64
};
struct sApp_6BB0 {
    char pad0[0x84];
    sMgr_6BB0* mgr; // 0x84
};
extern sApp_6BB0* SSXApp_s __asm__("SSXApp");
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;
extern int cPauseState_Stack[];
extern int D_004428F4[];

struct sGameState_6BB0 {
    char pad0[8];
    int done;   // 0x8
};

/**
 * Starts the pre-event game state: opens overlay 2 under pause state 1, unless the online session has failed.
 *
 * Clears the overlay-done flag (+0x8). When cGameComm_mGameComm reports an error it only forces the cut-scene
 * transition to regular mode and calls cBE_setState(0); otherwise it also calls cOVStateManager_gotoOverlay(2) and
 * pushes pause state 1 (cPauseState_Stack[++cPauseState_StackIdx] = 1, cPauseState_Flags = D_004428F4). Slot +0x2C (gainFocus) of
 * cGFPreEventState_vtbl.
 */
extern "C" void cGFPreEventState_gainFocus(cGFPreEventState* self)
{
    self->mOverlayDone = 0;
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mError != 0) {
        cCutSceneTransitionMan_ForceToRegularMode(SSXApp_s->mgr->world);
        cBE_setState(0);
        return;
    }
    cOVStateManager_gotoOverlay(2);
    cPauseState_Stack[++cPauseState_StackIdx] = 1;
    cPauseState_Flags = D_004428F4[0];
    cCutSceneTransitionMan_ForceToRegularMode(SSXApp_s->mgr->world);
    cBE_setState(0);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_update);
#ifdef SKIP_ASM
extern "C" void cGameState_gotoNextState(void* self);

/**
 * Steps the pre-event game state: moves to the next state once the overlay is done (+0x8 is 1), unless the online
 * session has failed.
 *
 * Calls cGameState_gotoNextState. Slot +0x34 (update) of cGFPreEventState_vtbl.
 */
extern "C" void cGFPreEventState_update(cGFPreEventState* self)
{
    if (cGameComm_mGameComm == 0 || cGameComm_mGameComm->mError == 0) {
        if (self->mOverlayDone == 1) {
            cGameState_gotoNextState(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_loseFocus);
#ifdef SKIP_ASM
extern "C" void cGFPreEventState_ResetGameState(cGFPreEventState* self);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];

/**
 * Ends the pre-event game state: pops the pause stack (restoring cPauseState_Flags) and resets the game state with
 * cGFPreEventState_ResetGameState.
 *
 * Slot +0x3C (loseFocus) of cGFPreEventState_vtbl. The pause stack index is not checked.
 */
extern "C" void cGFPreEventState_loseFocus(cGFPreEventState* self)
{
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
    cGFPreEventState_ResetGameState(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_ResetGameState);
#ifdef SKIP_ASM
class cRace236CD8 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void restart(int);
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_ResetAudioVariables(void* mgr);
extern char* cGameState_mpGame;

static inline int isMode4_236CD8()
{
    return cBENewRaceInterface_newRace.cGameMode == 4;
}

/**
 * Resets the race for the event: clears the NIS list, sets the AI back, and resets the audio.
 *
 * Calls cSSXScriptEngine_ResetNISList(ScriptEngine, 0); unless the race's game mode (newRace.cGameMode) is 4, calls
 * virtual slot 5 of the game's AI with 3 and sets the AI's +0x14 to 1; then cSSXAudio_ResetAudioVariables on the audio (cAudioInterface_bxAudio).
 */
extern "C" void cGFPreEventState_ResetGameState(cGFPreEventState* self)
{
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (!isMode4_236CD8()) {
        (*(cRace236CD8**)(cGameState_mpGame + 0xC))->restart(3);
        *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x14) = 1;
    }
    cSSXAudio_ResetAudioVariables(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the pre-event game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFPreEventState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPreEventState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the pre-event game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFPreEventState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPostEventState_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setIsFirstTimeWC(void* iface, int i, int a);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void cOVStateManager_gotoOverlay(int a);

struct sRace00236DA0 {
    char pad0[0x30];
    int count;
    char pad34[0x15];
    signed char flag49;
};

/**
 * Starts the post-event game state: on the career path resets each rider's entry, clears the NIS list and opens overlay
 * 7.
 *
 * When the race's cGamePath is 0, calls cBENewPlayerInterface_setIsFirstTimeWC(player interface, i, 0) for each rider; then
 * cSSXScriptEngine_ResetNISList(ScriptEngine, 0), clears the overlay-done flag (+0x8) and calls
 * cOVStateManager_gotoOverlay(7). Slot +0x2C (gainFocus) of cGFPostEventState_vtbl.
 */
extern "C" void cGFPostEventState_gainFocus(cGFPostEventState* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        int i = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        for (; i < cBENewRaceInterface_newRace.iNumberRiders; i++) {
            cBENewPlayerInterface_setIsFirstTimeWC(iface, i, 0);
        }
    }
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    self->mOverlayDone = 0;
    cOVStateManager_gotoOverlay(7);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPostEventState_update);
#ifdef SKIP_ASM
extern "C" void cGame_setState(void* self, int a, int b, int refresh);

/**
 * Steps the post-event game state: once the overlay is done (+0x8 is 1) sets the game's state to 14 with
 * cGame_setState(game, 0xE, 2, 0).
 *
 * Slot +0x34 (update) of cGFPostEventState_vtbl.
 */
extern "C" void cGFPostEventState_update(cGFPostEventState* self)
{
    if (self->mOverlayDone == 1) {
        cGame_setState(SSXApp->Game, 0xE, 2, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPostEventState_loseFocus);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern char* cGameState_mpGame;

/**
 * Ends the post-event game state: purges the UI's normal states (cUIStateStack_purgeNormalStates on the UI engine's
 * stack, mpGame + 0x48).
 *
 * Slot +0x3C (loseFocus) of cGFPostEventState_vtbl.
 */
extern "C" void cGFPostEventState_loseFocus(void)
{
    cUIStateStack_purgeNormalStates(*(char**)(cGameState_mpGame + 0x48) + 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPostEventState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the post-event game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFPostEventState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPostEventState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the post-event game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFPostEventState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_gainFocus);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void func_0028CDF8(void* mgr);

/**
 * Enters the podium state: calls func_0028CDF8() on the audio object and turns the AI's computer actors on.
 *
 * cAudioInterface_bxAudio() returns gSSXAudio; the AI's ComputerActorsActive (+0x8C) is set to 1. On Tour's version takes
 * this and a startup flag, which the body does not use.
 */
extern "C" void cGFPodiumState_gainFocus(void)
{
    func_0028CDF8(cAudioInterface_bxAudio());
    SSXApp->Game->AI->ComputerActorsActive = 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_update);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* a, int b);
extern "C" void cGameState_gotoNextState(void* self);

/**
 * Waits for the podium NIS to end, then moves the game to its next state.
 *
 * Calls cGameState_gotoNextState() once NIS list 0 reports play state 0 or 5.
 */
extern "C" void cGFPodiumState_update(cGFPodiumState* self)
{
    int playState = cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 0);
    int busy = playState != 0 && playState != 5;
    if (!busy) {
        cGameState_gotoNextState(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_loseFocus);
#ifdef SKIP_ASM

/**
 * Leaves the podium state by turning the AI's computer actors off (ComputerActorsActive = 0).
 */
extern "C" void cGFPodiumState_loseFocus(void)
{
    SSXApp->Game->AI->ComputerActorsActive = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the podium game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFPodiumState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the podium game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFPodiumState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFReplayStartState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_initMissions(void* a);
extern "C" void cWScriptMan_runGlobalScripts(void* a);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Starts a replay: initialises the missions and global scripts, hides the load indicator and opens the replay
 * overlay (0x11).
 */
extern "C" void cGFReplayStartState_gainFocus(void)
{
    cWScriptMan_initMissions(cWScriptMan_TheWScriptMan);
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 0);
    cOVStateManager_triggerOverlay(17);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFReplayStartState_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the replay-start state's update has an empty body.
 */
void cGFReplayStartState_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFReplayStartState_loseFocus);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_purgeMissions(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Leaves the replay-start state by purging the world script missions and setting them up again.
 *
 * Calls cWScriptMan_purgeMissions (On Tour cWScriptMan::purgeMissions) and cWScriptMan_setupMissions() on TheWScriptMan.
 */
extern "C" void cGFReplayStartState_loseFocus(void)
{
    cWScriptMan_purgeMissions(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(cWScriptMan_TheWScriptMan);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_cInputSync);
#ifdef SKIP_ASM
/**
 * Constructs a networked input synchroniser for a latency and a sample count.
 *
 * Stores sendlimit (+0x0) = @p latencyFrames * @p numSamples, latency (+0x4) = (60 / @p numSamples) * @p numSamples,
 * numsamples (+0x8), and clears tick (+0xC) and the synced-pad count (+0x10). cGame_load builds it.
 *
 * @param latencyFrames  the latency in frames
 * @param numSamples  copies per sample
 * @return @p self
 */
extern "C" void* cInputSync_cInputSync(cInputSync* self, int latencyFrames, int numSamples)
{
    self->latency = latencyFrames;
    self->numsamples = numSamples;
    self->tick = 0;
    self->numsyncedpads = 0;
    self->sendlimit = latencyFrames * numSamples;
    self->latency = (60 / numSamples) * numSamples;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync__cInputSync);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);


struct sObj_cInputSync__cInputSync {
    char pad[0x10];
    int count;
    cInputSync_SyncedPad entries[1];
};

/**
 * Destroys a networked input synchroniser: for each synced pad frees its sample ring (cMemMan_free) and deletes its two
 * input objects (+0x10, +0x14 of the pad), then frees the object when bit 0 of @p flags is set.
 *
 * cGame_purge calls it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cInputSync__cInputSync(cInputSync* self, int flags)
{
    int i;
    for (i = 0; i < self->numsyncedpads; i++) {
        cInputSync_SyncedPad* entry = &((&self->syncedpads[0]))[i];
        if (((void*)entry->samples))
            cMemMan_free(((void*)entry->samples));
        operator_delete(((int*)entry->destinationpad));
        operator_delete(((int*)entry->input));
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_createlocalinput);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: cInput_setPad__FPvi takes the 0x2A4-byte block as an int (pointer in int)
void cInput_setPad(void* self, int a1);
void cInput_setMap(void* self, int a1);
extern char str_InputSamples[];
extern char str_SyncedPad[];
extern char str_SyncedInput[];



struct sOwner_237140 {
    int f0;
    int count;
    int pad8[2];
    int n;
    cInputSync_SyncedPad entries[1];
};

static inline void* initObj_237140(void* o, int a, int b)
{
    cInput_setPad(o, a);
    cInput_setMap(o, b);
    return o;
}

/**
 * Wraps a local player's input in a network-synchronised input and returns the wrapper.
 *
 * Takes the next synced pad entry, allocates and clears its sample ring ("InputSamples", 100 bytes per sample,
 * each marked 0x18), records the frame (+0x0) and the real input's pad (@p realinput[0]), allocates the "SyncedPad"
 * buffer and builds a "SyncedInput" on it with the real input's map (@p realinput[1]).
 *
 * @param realinput  the real input (On Tour realinput)
 * @return the synced input that replaces it
 */
extern "C" void* cInputSync_createlocalinput(cInputSync* self, int* realinput)
{
    cInputSync_SyncedPad* syncedPad = &((&self->syncedpads[0]))[self->numsyncedpads++];
    *(cInputSync_PadSample**)&syncedPad->samples = (cInputSync_PadSample*)operator_new_tag(self->latency * 100, str_InputSamples, 0, 0);
    MEM_fill((syncedPad->samples), 0, self->latency * 100);
    for (int i = 0; i < self->latency; i++) {
        (syncedPad->samples)[i].NumButtons = 0x18;
    }
    syncedPad->readpos = 0;
    syncedPad->writepos = self->sendlimit;
    *(int*)&syncedPad->sourcepad = realinput[0];
    *(int**)&syncedPad->destinationpad = (int*)cMemMan_alloc(676, str_SyncedPad, 0, 0);
    *((int*)syncedPad->destinationpad) = 0;
    void* syncedInput = initObj_237140(cMemMan_alloc(8, str_SyncedInput, 0, 0), (int)((int*)syncedPad->destinationpad), realinput[1]);
    *(void**)&syncedPad->input = syncedInput;
    return syncedInput;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_createremoteinput);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: cInput_setPad__FPvi takes the 0x2A4-byte block as an int (pointer in int)
void cInput_setPad(void* self, int a1);
void cInput_setMap(void* self, int a1);
extern char str_InputSamples[];
extern char str_SyncedPad[];
extern char str_SyncedInput[];



struct sOwner_237280 {
    int f0;
    int count;
    int pad8[2];
    int n;
    cInputSync_SyncedPad entries[1];
};

static inline void* initObj_237280(void* o, int a, int b)
{
    cInput_setPad(o, a);
    cInput_setMap(o, b);
    return o;
}

/**
 * Creates a network-synchronised input for a remote player, fed only from received samples.
 *
 * Like cInputSync_createlocalinput, but the entry has no local pad (+0xC = 0) and the "SyncedInput" is built on @p realinputmap, the input
 * map to use.
 *
 * @param realinputmap  the input map (On Tour realinputmap)
 * @return the synced input
 */
extern "C" void* cInputSync_createremoteinput(cInputSync* self, int realinputmap)
{
    cInputSync_SyncedPad* syncedPad = &((&self->syncedpads[0]))[self->numsyncedpads++];
    *(cInputSync_PadSample**)&syncedPad->samples = (cInputSync_PadSample*)operator_new_tag(self->latency * 100, str_InputSamples, 0, 0);
    MEM_fill((syncedPad->samples), 0, self->latency * 100);
    for (int i = 0; i < self->latency; i++) {
        (syncedPad->samples)[i].NumButtons = 0x18;
    }
    syncedPad->readpos = 0;
    syncedPad->writepos = self->sendlimit;
    *(int*)&syncedPad->sourcepad = 0;
    *(int**)&syncedPad->destinationpad = (int*)cMemMan_alloc(676, str_SyncedPad, 0, 0);
    *((int*)syncedPad->destinationpad) = 0;
    void* syncedInput = initObj_237280(cMemMan_alloc(8, str_SyncedInput, 0, 0), (int)((int*)syncedPad->destinationpad), realinputmap);
    *(void**)&syncedPad->input = syncedInput;
    return syncedInput;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_preupdate);
#ifdef SKIP_ASM
// FAKEMATCH: redundant pointer local `psp` (`SyncedPad* psp = &syncedpads[i]; SyncedPad& sp = *psp;`) adds the one RTL
// insn
// that puts the outer loop over loop.c's 257-insn hoist threshold, so the constant 24 is not hoisted into s5; plain
// form 97.12%
// (99.69% with a ternary clamp). Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sPackSrc_7B28;
extern "C" void cInputSync_decompressinput(cInputSync* self, cInputSync_PadSample* src, char* buf);
extern "C" void cInputSync_sendClientInput(cInputSync* self, cInputSync_PadSample* src);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" int cGameComm_readPacket(void* self, int ch, void* dst, int size);
extern "C" float cGameComm_getLatency(void* self);

// cInputSync::preupdate (On Tour name and member names; the SSX 3 version differs in structure).
// cGameComm::mGameComm through On Tour's GameComm() accessor.
static inline char* GameComm() { return (char*)cGameComm_mGameComm; }



struct sInputPadButton_2373B8 {
    float value;
    char pad[0x18];
};

// cInputPad: button count and values (getNumButtons / getButtonValue in On Tour).
struct cInputPad_2373B8 {
    int numbuttons;
    sInputPadButton_2373B8 buttons[1];
    int getNumButtons() { return numbuttons; }
    float getButtonValue(int k) { return k < numbuttons ? buttons[k].value : 0.0f; }
};


struct cInputSync_2373B8 {
    int sendlimit;                      // 0x00
    int latency;                        // 0x04: ring size
    int numsamples;                     // 0x08: copies per sample
    int tick;                           // 0x0C
    int numsyncedpads;                  // 0x10
    cInputSync_SyncedPad syncedpads[1];     // 0x14
};

/**
 * Fills the synced pads' sample rings for this frame: reads remote samples from the network and samples the local pads.
 *
 * Does nothing until cGameComm_mGameComm's state is 14 or more. A remote pad (no source pad) takes compressed samples
 * from cGameComm_readPacket while its ring has room, or, offline, neutral samples; each is decompressed (cInputSync_decompressinput)
 * and written numsamples times. A local pad, while under the send limit, samples its buttons every numsamples ticks,
 * sends them (cInputSync_sendClientInput), queues them, and recomputes the send limit from the connection's latency (cGameComm_getLatency)
 * divided by the frame time (SSXApp + 0x14), at least 2 samples, capped at the ring size. cGame_update calls it.
 */
extern "C" void cInputSync_preupdate(cInputSync* self)
{
    cInputSync_PadSample sample;
    cInputSync_CompressedSample compsample;
    if (*(int*)(GameComm() + 0x70) < 14) {
        return;
    }
    for (int i = 0; i < self->numsyncedpads; i++) {
        cInputSync_SyncedPad* psp = &((&self->syncedpads[0]))[i];
        cInputSync_SyncedPad& sp = *psp;
        if (((cInputPad_2373B8*)sp.sourcepad) == 0) {
            cGameComm* gc = (cGameComm*)GameComm();
            int net = gc->mSyncInput != 0 && gc->mError == 0 && gc->mConnected != 0;
            if (net) {
                while (sp.writepos - sp.readpos < self->latency - (self->numsamples - 1) &&
                       cGameComm_readPacket(GameComm(), 0, &compsample, sizeof(compsample))) {
                    cInputSync_decompressinput(self, (&sample), (char*)&compsample);
                    for (int j = 0; j < self->numsamples; j++) {
                        (sp.samples)[sp.writepos % self->latency] = sample;
                        sp.writepos++;
                    }
                }
            } else {
                MEM_fill(&compsample, 0, sizeof(compsample));
                compsample.NumButtons = 24;
                cInputSync_decompressinput(self, (&sample), (char*)&compsample);
                while (sp.writepos - sp.readpos < self->latency - (self->numsamples - 1)) {
                    for (int j = 0; j < self->numsamples; j++) {
                        (sp.samples)[sp.writepos % self->latency] = sample;
                        sp.writepos++;
                    }
                }
            }
        } else if (sp.writepos - sp.readpos < self->sendlimit - (self->numsamples - 1)) {
            self->tick++;
            if (self->tick % self->numsamples == 0) {
                cInputSync_PadSample localsample;
                localsample.NumButtons = ((cInputPad_2373B8*)sp.sourcepad)->getNumButtons();
                for (int j = 0; j < localsample.NumButtons; j++) {
                    localsample.Buttons[j] = ((cInputPad_2373B8*)sp.sourcepad)->getButtonValue(j);
                }
                cInputSync_sendClientInput(self, (&localsample));
                for (int j = 0; j < self->numsamples; j++) {
                    (sp.samples)[sp.writepos % self->latency] = localsample;
                    sp.writepos++;
                }
                int n = (int)(cGameComm_getLatency(GameComm()) / SSXApp->TimerPeriod);
                if (!(1 < n)) {
                    n = 2;
                }
                self->sendlimit = n * self->numsamples;
                if (self->latency < self->sendlimit) {
                    self->sendlimit = self->latency;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_postupdate__FPv);
#ifdef SKIP_ASM
/**
 * Input sync step after the game update: does nothing in this build.
 */
void cInputSync_postupdate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_aiinputready);
#ifdef SKIP_ASM
struct sOwner237950 {
    char pad[0x10];
    int count;
    cInputSync_SyncedPad slots[1];
};

/**
 * Tells whether every synced pad has a sample ready for the AI update.
 *
 * Returns 0 while cGameComm_mGameComm's state is below 14, while it is unpausing (flag or timer), or when any pad's
 * read position equals its write position.
 *
 * @return 1 when input is ready, otherwise 0
 */
extern "C" int cInputSync_aiinputready(cInputSync* self)
{
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm->mState < 14) {
        return 0;
    }
    int busy = gameComm->mUnPauseFlag != 0 || gameComm->mUnPauseTimer > 0;
    if (busy) {
        return 0;
    }
    int i;
    for (i = 0; i < self->numsyncedpads; i++) {
        cInputSync_SyncedPad* slot = &((&self->syncedpads[0]))[i];
        if (slot->readpos == slot->writepos) {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_aiupdate);
#ifdef SKIP_ASM
extern "C" void cInputPad_update(void* target, int a, void* data);

struct sPlayer002379C8 {
    int f0;
    int nFrames;
    int f8;
    int fC;
    int count;
    cInputSync_SyncedPad tracks[1];
};

/**
 * Feeds the synced pads their next received input sample, once every player's input for the frame is there.
 *
 * Returns 0 while cGameComm_mGameComm's state is below 14 or cInputSync_aiinputready says inputs are not ready; otherwise,
 * for each synced pad, passes the current sample (cursor modulo the ring size) to cInputPad_update and advances it.
 *
 * @return 1 when the pads were updated
 */
extern "C" int cInputSync_aiupdate(cInputSync* self)
{
    if (cGameComm_mGameComm->mState < 14) {
        return 0;
    }
    if (cInputSync_aiinputready(self) == 0) {
        return 0;
    }
    int i;
    for (i = 0; i < self->numsyncedpads; i++) {
        cInputSync_SyncedPad* syncedPad = &((&self->syncedpads[0]))[i];
        cInputSync_PadSample* inputSample = &(syncedPad->samples)[syncedPad->readpos % self->latency];
        cInputPad_update(((void*)syncedPad->destinationpad), inputSample->NumButtons, ((char*)&inputSample->Buttons[0]));
        syncedPad->readpos++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_sendClientInput);
#ifdef SKIP_ASM
struct sPackSrc_7B28;
struct sPackDst_7B28;
extern "C" int cInputSync_compressinput(cInputSync* self, cInputSync_PadSample* src, cInputSync_CompressedSample* dst);
extern "C" void cInputSync_decompressinput(cInputSync* self, cInputSync_PadSample* src, char* buf);
extern "C" void cGameComm_writePacket(void* mgr, int a, char* buf, int len);

/**
 * Sends one local pad sample: compresses it, decompresses it back into @p src, and posts it on the network when the
 * session is synced, error-free and connected.
 *
 * Uses cInputSync_compressinput and cInputSync_decompressinput, then cGameComm_writePacket(cGameComm_mGameComm, 0, buffer, length). Decompressing back
 * makes the local copy match what the peer receives.
 *
 * @param src  the pad sample (button count and values)
 */
extern "C" void cInputSync_sendClientInput(cInputSync* self, cInputSync_PadSample* src)
{
    char buf[32];
    int len = cInputSync_compressinput(self, src, (cInputSync_CompressedSample*)buf);
    cInputSync_decompressinput(self, src, buf);
    cGameComm* gameComm = cGameComm_mGameComm;
    int ok = gameComm->mSyncInput != 0 && gameComm->mError == 0 && gameComm->mConnected != 0;
    if (ok) {
        cGameComm_writePacket((char*)cGameComm_mGameComm, 0, buf, len);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_compressinput);
#ifdef SKIP_ASM


/**
 * Compresses a pad sample: each button value times 255 becomes a byte, and only the non-zero bytes are kept, with a bit
 * mask.
 *
 * Writes the button count, three mask bytes (bit i set for a non-zero button i) and the kept bytes to @p dst.
 *
 * @param src  the pad sample (count, float values)
 * @param dst  the compressed sample
 * @return the compressed size in bytes (4 + the kept bytes)
 */
extern "C" int cInputSync_compressinput(cInputSync* self, cInputSync_PadSample* src, cInputSync_CompressedSample* dst)
{
    int i;
    int j;
    int n;
    dst->NumButtons = src->NumButtons;
    for (j = 0; j < 3; j++) {
        dst->NonZeroBitBytes[j] = 0;
    }
    n = 0;
    for (i = 0; i < src->NumButtons; i++) {
        unsigned char quantized = (int)(((float*)&src->Buttons[0])[i] * 255.0f);
        if (quantized != 0) {
            dst->NonZeroBitBytes[i >> 3] |= 1 << (i & 7);
            dst->NonZeroBytes[n++] = quantized;
        }
    }
    return n + 4;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cInputSync_decompressinput);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Decompresses a pad sample: buttons whose mask bit is set take the next byte / 255, the others 0.
 *
 * Clears the 24 button values first.
 *
 * @param out  the pad sample written
 * @param buf  the compressed sample
 */
extern "C" void cInputSync_decompressinput(cInputSync* self, cInputSync_PadSample* out, char* buf)
{
    cInputSync_CompressedSample* in = (cInputSync_CompressedSample*)buf;
    out->NumButtons = in->NumButtons;
    MEM_fill(((float*)&out->Buttons[0]), 0, 0x60);
    int k = 0;
    int i;
    for (i = 0; i < out->NumButtons; i++) {
        if ((in->NonZeroBitBytes[i >> 3] >> (i & 7)) & 1) {
            ((float*)&out->Buttons[0])[i] = in->NonZeroBytes[k++] * 0.003921568859368563f;
        } else {
            ((float*)&out->Buttons[0])[i] = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", GMsrand);
#ifdef SKIP_ASM
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed);
extern uint GMrandom[];

/**
 * Seeds the shared game random generator GMrandom (kept in step between networked players).
 *
 * @param seed  the seed
 */
extern "C" void GMsrand(uint seed)
{
    cBxPseudoRng_Seed(GMrandom, seed);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", GMrand);
#ifdef SKIP_ASM
extern "C" uint cBxPseudoRng_NextInt(uint* state);
extern uint GMrandom[];

/**
 * Returns the next number from the shared game random generator GMrandom (cBxPseudoRng_NextInt).
 *
 * @return the next pseudo-random number
 */
extern "C" uint GMrand(void)
{
    return cBxPseudoRng_NextInt(GMrandom);
}
#endif

