#include "common.h"
#include "be/beintnewrace.h"
#include "main/gamemode.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void func_0039F840(void* list);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void cReplay_reset(void* p);
extern "C" void* func_0028B180(void);
extern "C" void func_00258968(void* p);
extern "C" void cGame_resetWorld(void* self);
extern "C" void cReplay_startRace(void* p);
void func_0029C418(void* p);
extern "C" void func_0029C420(void* p, int a1);
extern "C" void func_00234BE8(void* self, int a1);
int GetHashValue32(char* s);
extern char* cGameState_mpGame;
extern signed char D_00535C10[];
extern char str_StartlightBegin[];

struct sRace_234AD0 {
    char pad[0x8C];
    int f8C;
};

/**
 * Starts the start-gate countdown: resets the replay for the race, puts the AI in state 4 and fires
 * "StartlightBegin".
 *
 * An online game first resets the world. Game modes 4-6 skip the gate and go straight to the next state.
 */
extern "C" void cGFGateState_gainFocus(void* self)
{
    (*(sRace_234AD0**)(cGameState_mpGame + 0xC))->f8C = 0;
    if (cGameComm_mGameComm != 0) {
        func_00258968((char*)cGameComm_mGameComm);
    }
    cBE_setState(0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        cGame_resetWorld(cGameState_mpGame);
    }
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cReplay_startRace(*(void**)(cGameState_mpGame + 0x28));
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = D_00535C10[0];
    if (mode >= 4 && mode <= 6) {
        cGameState_gotoNextState(self);
        return;
    }
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 4);
    func_0029C418(func_0028B180());
    func_0029C420(func_0028B180(), 3);
    func_00234BE8(self, GetHashValue32(str_StartlightBegin));
    func_0039F840(*(char**)(cGameState_mpGame + 0x48) + 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234BE8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" int cSectionMan_getWorldSectionIDFromBELocationID(void* world, int id);
extern "C" int func_00309DD0(void* mgr, int id, int a);
extern "C" void func_00309E50(void* mgr, int id, int a);
extern void* D_004A3DD8;

extern "C" void func_00234BE8(void* self, int a1)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* world = SSXApp->Game->SectionMan;
    int id = cSectionMan_getWorldSectionIDFromBELocationID(world, *cBENewRaceInterface_getLocation(iface));
    if (func_00309DD0(D_004A3DD8, id, a1) != 0) {
        func_00309E50(D_004A3DD8, id, a1);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFGateState_update);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" int cReplay_isPauseAI(void* p);
extern "C" void func_00234BE8(void* self, int a1);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" void* func_0028B180(void);
extern "C" void func_0029C7B0(void* p);
void func_0028C8C0(void* self, int val);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void func_0029C420(void* p, int secs);
extern char* cGameState_mpGame;
extern signed char D_00535C10[];
extern char str_StartgateOpen[];
extern char str_hud_c[];

struct sVEntry00234C68 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

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
extern "C" void cGFGateState_update(void* self)
{
    if (cReplay_isPauseAI(*(void**)(cGameState_mpGame + 0x28)) != 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C10[0] == 4) {
        return;
    }
    if (*(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x1C) <= 0) {
        func_00234BE8(self, GetHashValue32(str_StartgateOpen));
        char* obj = (char*)cUIStateStack_findState(*(char**)(cGameState_mpGame + 0x48) + 0x18, GetHashValue32(str_hud_c));
        if (obj != 0) {
            sVEntry00234C68* vtEntry = &(*(sVEntry00234C68**)(obj + 8))[24];
            vtEntry->fn(obj + vtEntry->delta, 6, 0);
        }
        func_0029C7B0(func_0028B180());
        func_0028C8C0(func_0028B180(), 0x78);
        cGameState_gotoNextState(self);
    }
    int frames = *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x1C);
    float secs = frames / 60;
    if (frames % 60 == 0) {
        func_0029C420(func_0028B180(), (int)ffloor_00234C68(secs));
    }
}
#endif

extern "C" void* func_00233AF0(void* self);

//100%
INCLUDE_ASM("main/gamestate", func_00234DE0__FPv);
#ifdef SKIP_ASM
void* func_00234DE0(void* self)
{
    return func_00233AF0(self);
}
#endif

extern "C" void* func_00233B28(void* self);

//100%
INCLUDE_ASM("main/gamestate", func_00234E00__FPv);
#ifdef SKIP_ASM
void* func_00234E00(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234E20);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void func_0039F840(void* list);
extern char* cGameState_mpGame;
extern signed char D_00535C10[];

extern "C" void func_00234E20(void)
{
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 5);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C10[0] != 4) {
        char* g = cGameState_mpGame;
        *(int*)(*(char**)(g + 0xC) + 0x14) = 1;
        *(int*)(*(char**)(g + 0xC) + 0x8C) = 0;
    }
    cBE_setState(1);
    char* gs = cGameState_mpGame;
    int n = **(int**)(gs + 0x28);
    int ok = n != 0 && n < 10;
    if (!ok) {
        func_0039F840(*(char**)(gs + 0x48) + 0x18);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234EB8__FPv);
#ifdef SKIP_ASM
void* func_00234EB8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234ED8__FPv);
#ifdef SKIP_ASM
void* func_00234ED8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00234EF8);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char cGFTrackStartState_vtbl[];
extern char* cGameState_mpGame;

extern "C" void* func_00234EF8(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    cGameState_mpGame = 0;
    *(void**)((char*)self + 0xC) = cGFTrackStartState_vtbl;
    *(int*)((char*)self + 0x0) = 4;
    *(int*)((char*)self + 0x10) = 5;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_preloadInit);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cReplay_reset(void* p);
extern "C" void cSSXScriptEngine_PurgeTrack(void* a);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void cSSXScriptEngine_AsyncLoadTrack(void* a);
extern "C" void func_0012AB20(void* self);
extern "C" void func_0012AC48(void* self);
extern "C" int func_00145D38(void* self, int index, int bit);
extern "C" void cSSXScriptEngine_AppendStartSequence(void* a, int b);
extern "C" void* func_0028B180(void);
extern char* cGameState_mpGame;
extern signed char D_00535C11[];

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
extern "C" void cGFTrackStartState_preloadInit(sState_234F40* self)
{
    self->f18 = 0;
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cSSXScriptEngine_PurgeTrack(ScriptEngine);
    cSSXScriptEngine_AsyncLoadTrack(ScriptEngine);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    self->f14 = 0;
    int gamePath = D_00535C11[0];
    func_0012AB20(*(void**)(cGameState_mpGame + 0xC));
    func_0012AC48(*(void**)(cGameState_mpGame + 0xC));
    if (gamePath == 0) {
        int locationId = *(int*)((char*)SSXApp->Game->SectionMan + 0x1BC);
        if (locationId >= 14 && locationId <= 16) {
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
            char* sub = *(char**)(*(char**)(*(char**)(cGameState_mpGame + 0xC) + 0x40) + 0x18) + 0x6C0;
            sVEntry_234F40* vt = *(sVEntry_234F40**)sub;
            if (func_00145D38(iface, vt[7].fn(sub + vt[7].delta), locationId) != 0) {
                char* audio = (char*)func_0028B180();
                *(int*)(audio + 0x5790) = 0;
                *(int*)(audio + 0x578C) = 1;
                *(int*)(audio + 0x6254) = 1;
                self->f14 = 1;
            }
        }
    } else {
        cSSXScriptEngine_AppendStartSequence(ScriptEngine, 0);
    }
    self->f10 = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_preloadUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the range bounds 6 / 11 as locals `first` / `last` (li + slt instead of slti); plain literals 98.25%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).
// Virtual calls in this unit go through these by hand.
#include "vtable.h"



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


// The game (D_004A2C68_date, also D_004A28A8_date->game).
struct sGame {
    char pad_0x0[0xC];
    cRace* race;                // 0xC
    void* cache;                // 0x10
    char pad_0x14[0x14];
    int* f28;                   // 0x28
    char pad_0x2C[0x1C];
    char* f48;                  // 0x48 (+0x18: the list for func_0039F840 / func_0039F9D8)
    char pad_0x4C[0x18];
    void* f64;                  // 0x64
    char pad_0x68[0x10];
    char* f78;                  // 0x78
    char pad_0x7C[0x8];
    void* cameras;              // 0x84
    char pad_0x88[0xC];
    void* f94;                  // 0x94 (the HUD for func_001F36D8 / func_001F36C0)
};

extern sGame* D_004A2C68_date __asm__("cGameState_mpGame");

// The game singleton (D_004A28A8_date).
struct sApp {
    char pad_0x0[0x78];
    void* f78;                  // 0x78
    char pad_0x7C[0x8];
    sGame* game;                // 0x84
    char pad_0x88[0x38];
    cGameModeMan* gm;           // 0xC0
};

extern sApp* D_004A28A8_date __asm__("SSXApp");

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void* func_0028B180(void);


// PORT: cSSXScriptEngine_ReadyNISList returns func_00276270's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);

extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);

extern "C" void cBENewPlayerInterface_setRiderCharID(void* player, int i, int id);

extern "C" void func_001473D0(void* player, int i, int v);

extern "C" int cAI_isReadyToPurgeComputerRiders(void* p);

extern "C" int cAI_isAILoadedAndReady(void* p);

void func_001289F0(void* p);

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
extern "C" void cGFTrackStartState_preloadUpdate(void* self)
{
    *(int*)((char*)self + 0x18) += 1;
    switch (*(int*)((char*)self + 0x10)) {
    case 0: {
        if (cSSXScriptEngine_IsLoadTrackComplete(ScriptEngine) == 0)
            break;
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
        char* sectionMan = D_004A28A8_date->game->f78;
        int locationId = *(int*)(sectionMan + 0x1BC);
        int range = locationId >= 14 && locationId <= 16;
        if (*(int*)((char*)self + 0x14) != 0) {
            if (locationId == 14) {
                cSSXScriptEngine_AddToNISListByVideoCategory(ScriptEngine, 1, 0x1D, 3);
                func_0028CD48(func_0028B180());
                func_002B3A70((char*)func_0028B180() + 0x118);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0, 3, 0, -1, -1, -1);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 1, 3, 0, -1, -1, -1);
            } else {
                cSSXScriptEngine_AddToNISListByVideoCategory(ScriptEngine, 1, locationId == 15 ? 0x1E : 0x1F, 3);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x10, 3, 0, -1, -1, -1);
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x11, 3, 0, -1, -1, -1);
            }
            cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
        } else if (*(int*)(sectionMan + 0x1C8) != 0 && range) {
            cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x10, 3, 0, -1, -1, -1);
            cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x11, 3, 0, -1, -1, -1);
        }
        *(int*)((char*)self + 0x10) = 1;
        break;
    }
    case 1: {
        long done = 0;
        if (!isOne_235080(cSSXScriptEngine_GetNISListState(ScriptEngine, 0))) {
            if (!isOne_235080(cSSXScriptEngine_GetNISListState(ScriptEngine, 1)))
                done = 1;
        }
        if (done)
            *(int*)((char*)self + 0x10) = 2;
        break;
    }
    case 2: {
        // Modes first..last are already set up; anything else needs initGameMode.
        int first = 6, last = 11;
        if (cAI_isReadyToPurgeComputerRiders(D_004A2C68_date->race) == 0)
            break;
        if (cAI_isReadyToPurgeComputerActors(D_004A2C68_date->race) == 0)
            break;
        char* newRace = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
        char* newPlayer = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        cGameModeMan* gm = D_004A28A8_date->gm;
        int mode = beRaceDB->cGameEvent;
        if (mode < first || mode > last)
            cGameModeMan_initGameMode(gm, mode);
        int numAI = gm->iNumberAI - gm->iSimAI;
        int base = D_004A2C68_date->race->numPlayers;
        int i = 0;
        cBENewRaceInterface_setNumberAI(newRace, numAI);
        {
            sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)(newRace + 0xC);
            vt[1].fn(newRace + vt[1].delta);
        }
        for (; i < numAI; i++) {
            cBENewPlayerInterface_setRiderCharID(newPlayer, i + base, gm->iCharacters[base + i]);
            func_001473D0(newPlayer, i + base, gm->iCheatCharacters[base + i]);
        }
        {
            sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)(newPlayer + 0xC);
            vt[1].fn(newPlayer + vt[1].delta);
        }
        cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
        int quick = 0;
        cAI_cleanupAfterRace(D_004A2C68_date->race);
        cAI_cleanupActors(D_004A2C68_date->race);
        if (raceDB->cGameMode == 0)
            quick = raceDB->cGamePath == 0;
        cAI_initAIRiders(D_004A2C68_date->race);
        if (numAI == 0 && quick) {
            if (raceDB->cGameMode == 0)
                cAI_initAIActors(D_004A2C68_date->race, 0);
            else
                cAI_initAIActors(D_004A2C68_date->race, 2);
        }
        *(int*)((char*)self + 0x10) = 3;
        break;
    }
    case 3: {
        if (cAI_isAILoadedAndReady(D_004A2C68_date->race)) {
            cBE_getInterface_Fv(cBE_getBE(), 0);
            cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
            int gameMode = beRaceDB->cGameMode;
            if (gameMode == 0) {
                func_001289F0(D_004A2C68_date->race);
                if (D_004A2C68_date->race->f80 > 0)
                    D_004A2C68_date->race->f14 = 1;
                if (D_004A2C68_date->race->f84 > 0)
                    D_004A2C68_date->race->f8C = 1;
            }
            if (gameMode == 5) {
                cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
                if (raceDB->cGamePath == 0)
                    cAI_initFreeride(D_004A2C68_date->race);
            }
            *(int*)((char*)self + 0x10) = 4;
        } else {
            cAI_loadRiders(D_004A2C68_date->race);
        }
        break;
    }
    case 4:
        if (*(int*)((char*)self + 0x14) != 0 && *(int*)((char*)self + 0x18) < 180)
            break;
        *(int*)((char*)self + 0x10) = 5;
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
extern "C" void cGFTrackStartState_preloadCancel(void* self)
{
    cSSXScriptEngine_PurgeTrack(ScriptEngine);
    cSSXScriptEngine_ResetNISList(ScriptEngine, 1);
    *(int*)((char*)self + 0x10) = 5;
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
extern "C" int cGFTrackStartState_isPreloadComplete(void* self)
{
    return *(int*)((char*)self + 0x10) == 5;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFTrackStartState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void cAI_setAIState(void*, int);
extern "C" int cSSXScriptEngine_IsInTransportSequence(void* a);
extern "C" void cBERewardInterface_clearUserAwards(void* iface);
extern "C" void cWScriptMan_initMissions(void* a);
extern "C" void cWScriptMan_runGlobalScripts(void* a);
extern "C" int cBENewRaceInterface_getLocationID(void* race);
extern "C" int func_00144CC0(void* race, int a);
extern "C" void func_00146E10(void* player, int a, int b);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);
extern "C" void* func_0028B180(void);
extern "C" void func_002B3A98(void* p);
extern "C" void func_002A4B68(void* p);
extern char* cGameState_mpGame;
extern void* D_004A3DD8;


struct sRaceMan_2355C0 {
    char pad0[0x14];
    int f14;
};

struct sVE_2355C0 {
    short delta;
    short index;
    void (*fn)(void*);
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
extern "C" void cGFTrackStartState_gainFocus(void* self)
{
    *(int*)((char*)self + 0x1C) = 0;
    if (cSSXScriptEngine_IsInTransportSequence(ScriptEngine))
        *(int*)((char*)self + 0x1C) = 1;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBERewardInterface_clearUserAwards(cBE_getInterface_Fv(cBE_getBE(), 0xD));
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int flag = beRaceDB->cGamePath;
    int mode = beRaceDB->cGameMode;
    if (flag == 0 && !inRange_2355C0(*(int*)((char*)SSXApp->Game->SectionMan + 0x1BC))) {
        *(int*)((char*)self + 0x4) = 0;
        *(int*)((char*)self + 0x0) = 4;
    } else {
        *(int*)((char*)self + 0x0) = 1;
        *(int*)((char*)self + 0x4) = flag == 0;
    }
    if (mode == 4)
        cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 3);
    cBE_setState(0);
    cWScriptMan_initMissions(D_004A3DD8);
    cWScriptMan_runGlobalScripts(D_004A3DD8);
    if (func_00144CC0(race, cBENewRaceInterface_getLocationID(race)) == 1) {
        char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_00146E10(player, 0, cBENewRaceInterface_getLocationID(race));
        sVE_2355C0* vt = *(sVE_2355C0**)(player + 0xC);
        vt[1].fn(player + vt[1].delta);
    }
    (*(sRaceMan_2355C0**)(cGameState_mpGame + 0xC))->f14 = mode != 4;
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 0);
    if (flag) {
        *(int*)((char*)self + 0x0) = 1;
        *(int*)((char*)self + 0x4) = 0;
        if (isMode2_2355C0(ScriptEngine)) {
            cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 0, 1);
            if (isMode2_2355C0(ScriptEngine))
                cSSXScriptEngine_PlayNISList(ScriptEngine, 0, 0, 1);
        }
    } else if (*(int*)((char*)self + 0x14)) {
        cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 1, 0);
    }
    func_002B3A98((char*)func_0028B180() + 0x118);
    func_002A4B68(func_0028B180());
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
extern "C" void cGFTrackStartState_update(void* self)
{
    if (*(int*)((char*)self + 0x1C) != 0 && cSSXScriptEngine_IsInTransportSequence(ScriptEngine) == 0) {
        *(int*)((char*)SSXApp->Game->SectionMan + 0x1A4) = 1;
        *(int*)((char*)self + 0x1C) = 0;
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
extern "C" void func_00308F38(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern "C" void cReplay_reset(void* p);
extern "C" void cReplay_startRace(void* p);
extern "C" void func_00145DD0(void* iface, int a1, int a2, int a3);
extern "C" void func_00238510(void* gm, int a1, int a2);


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
extern "C" void cGFTrackStartState_loseFocus(void* self)
{
    func_00308F38(D_004A3DD8);
    cWScriptMan_setupMissions(D_004A3DD8);
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cReplay_startRace(*(void**)(cGameState_mpGame + 0x28));
    *(int*)((char*)self + 0x14) = 0;
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    if (beRaceDB->cGamePath == 0) {
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        func_00145DD0(player, 0, 0, *cBENewRaceInterface_getLocation(race));
    }
    int gameMode = beRaceDB->cGameMode;
    if (gameMode == 5) {
        goto restart;
    }
    if (gameMode == 6) {
    restart:
        func_00238510(SSXApp->GameModeMan, 0, 0);
    }
    char* game = (char*)SSXApp->Game;
    char* x = *(char**)(game + 0x84);
    if (*(int*)(x + 0x10) == 1) {
        sTbl_235868* viewTbl = **(sTbl_235868***)(game + 0x10);
        viewTbl->slot[otherSide_235868(*(char**)(x + 4))].f0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235990);
#ifdef SKIP_ASM
struct sVEntry_func_00235990 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00235990(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00235990* e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235A18);
#ifdef SKIP_ASM
struct sVEntry_func_00235A18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_00235A18(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00235A18* e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235A18**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void func_0030B7F8(void* self);
extern "C" void cGame_partialRestart(void* self);
extern "C" void func_0012AB20(void* self);
extern "C" void func_0012AC48(void* self);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void cSSXScriptEngine_AppendStartSequence(void* a, int b);
// PORT: cSSXScriptEngine_ReadyNISList returns func_00276270's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern "C" void cSSXScriptEngine_PlayTransportSequence(void* a, int b, int c);
extern "C" void func_001F36C0(void* a, int b);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* player, int i, int id);
extern "C" void func_001473D0(void* player, int i, int v);
extern char* cGameState_mpGame;
extern void* D_004A3DD8;


struct sVE_235AA0 {
    short delta;
    short index;
    void (*fn)(void*);
};


/**
 * Starts the next heat: restarts the heat and the game, re-picks the computer riders and queues the start
 * sequence.
 *
 * Game modes 5 and 6, and online games, skip straight to step 3. Race mode also plays the transport sequence.
 */
extern "C" void cGFNextHeatState_gainFocus(void* self)
{
    cBE_setState(0);
    func_0030B7F8(D_004A3DD8);
    char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
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
        *(int*)((char*)self + 0x10) = 3;
        return;
    }
    int n = gm->iNumberAI - gm->iSimAI;
    int base = *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x7C);
    int i = 0;
    cBENewRaceInterface_setNumberAI(race, n);
    {
        sVE_235AA0* vt = *(sVE_235AA0**)(race + 0xC);
        vt[1].fn(race + vt[1].delta);
    }
    for (; i < n; i++) {
        cBENewPlayerInterface_setRiderCharID(player, i + base, gm->iCharacters[base + i]);
        func_001473D0(player, i + base, gm->iCheatCharacters[base + i]);
    }
    {
        sVE_235AA0* vt = *(sVE_235AA0**)(player + 0xC);
        vt[1].fn(player + vt[1].delta);
    }
    func_0012AB20(SSXApp->Game->AI);
    if (n >= 2)
        func_0012AC48(SSXApp->Game->AI);
    *(int*)((char*)self + 0x18) = 0;
    cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
    if (raceDB->cGameMode == 0) {
        void* sc = ScriptEngine;
        *(int*)((char*)self + 0x18) = 1;
        cSSXScriptEngine_PlayTransportSequence(sc, 1, 0);
    }
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cSSXScriptEngine_AppendStartSequence(ScriptEngine, *(int*)((char*)gm + 0x98) ? 3 : 2);
    cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
    func_001F36C0(SSXApp->Game->LoadIndicator, 3);
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFNextHeatState_update);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
struct sVec4_236960;
struct sVec4_235CC8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRaceMan_235CC8 {
    char pad0[0x14];
    int f14;
};

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
void func_001289F0(void* p);
extern "C" void cAI_loadRiders(void* p);
extern "C" sVec4_235CC8 func_00122C28(void* rider);
extern "C" void func_003A9658(void* cache, int idx, const sVec4_236960* pos, float radius);
extern "C" int cWorldView_getTestFocus(void* cache, int idx, sVec4_236960* pos, float* dist);
extern "C" int cSSXScriptEngine_GetNISListState(void* p, int a);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* p);
extern "C" void cSSXScriptEngine_EndTransportSequence(void* p);
extern "C" void cGameState_gotoNextState(void* self);
void* func_00230698(void* self, int id);
extern char* cGameState_mpGame;
extern int D_00535C04[];

/**
 * Steps the next-heat setup: rebuild the AI riders, place the world-cache focus at the first rider, end the
 * transport, continue.
 *
 * Step 2 waits until the focus is 20000 units away, 120 frames have passed and the transport NIS and rider
 * are idle.
 */
extern "C" void cGFNextHeatState_update(void* self)
{
    *(int*)((char*)self + 0x14) += 1;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* game = (char*)SSXApp->Game;
    int state = *(int*)((char*)self + 0x10);
    char** cache = *(char***)(game + 0x10);
    int idx = *(int*)(*(char**)(*(char**)(game + 0x84) + 4) + 0xB0) == 0;
    switch (state) {
    case 0:
        if (cAI_isReadyToPurgeComputerRiders(*(void**)(game + 0xC))) {
            cAI_cleanupAfterRace(SSXApp->Game->AI);
            if (D_00535C04[0] >= 2)
                cAI_cleanupActors(SSXApp->Game->AI);
            cAI_initAIRiders(SSXApp->Game->AI);
            *(int*)((char*)self + 0x10) = 1;
        }
        break;
    case 1:
        if (cAI_isAILoadedAndReady(*(void**)(game + 0xC))) {
            func_001289F0(SSXApp->Game->AI);
            (*(sRaceMan_235CC8**)(cGameState_mpGame + 0xC))->f14 = 1;
            ((sCacheTbl_235CC8*)*cache)->slot[idx].f0 = 1;
            char* player = *(char**)((char*)SSXApp->Game->AI + 0x40);
            sVec4_235CC8 pos = func_00122C28(*(void**)(player + 0x18));
            func_003A9658(*cache + 0x10, idx, (const sVec4_236960*)&pos, 45000.0f);
            *(int*)((char*)self + 0x10) = 2;
        } else {
            cAI_loadRiders(SSXApp->Game->AI);
        }
        break;
    case 2:
        if (*(int*)((char*)self + 0x18) != 0) {
            sVec4_235CC8 focusPos;
            float dist;
            cWorldView_getTestFocus(*cache + 0x10, idx, (sVec4_236960*)&focusPos, &dist);
            if (dist < 20000.0f)
                break;
            if (*(int*)((char*)self + 0x14) < 120)
                break;
            if (cSSXScriptEngine_GetNISListState(ScriptEngine, 0) == 1)
                break;
            char* transitionMan = (char*)func_00230698(SSXApp->Game, 0);
            if (cSSXScriptEngine_IsActiveNISInTransition(ScriptEngine) != 0)
                break;
            if (((cRiderObj_235CC8*)(transitionMan + 0x10))->isBusy() != 0)
                break;
            if (*(int*)(transitionMan + 0x44) != 0)
                break;
            cSSXScriptEngine_EndTransportSequence(ScriptEngine);
        }
        *(int*)((char*)self + 0x10) = 3;
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
INCLUDE_ASM("main/gamestate", func_00235F48);
#ifdef SKIP_ASM
struct sVEntry_func_00235F48 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00235F48(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00235F48* e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235F48**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00235FD0);
#ifdef SKIP_ASM
struct sVEntry_func_00235FD0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_00235FD0(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00235FD0* e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00235FD0**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_gainFocus);
#ifdef SKIP_ASM
extern "C" void func_0030B7F8(void* self);
extern "C" void cGameModeMan_initGameMode(void* gm, int mode);
extern "C" void cGame_partialRestart(void* self);
extern "C" void cRider_putAtStartpoint(void* self, int a1, int a2);
extern "C" void cRider_cleanupAfterPutAt(void* self, int notify);
void* func_00230698(void* self, int id);
// PORT: func_002E4CE8 is the wake pool's allocator; it ignores the size the caller passes
extern "C" void* func_002E4CE8_sz(int size) __asm__("func_002E4CE8");
void* func_002E4D70(void* self);
extern "C" void func_002E4370(void* self, int a1, int a2, void* a3, float f0, float f1, int a4, int a5, float f2);
extern void* cFade_TObj_vtbl[];

struct sColor_236058 {
    float r, g, b, a;
};

/**
 * Teleports the rider for a new session: free-ride event 12, a partial restart at the mission start point, a
 * white fade.
 */
extern "C" void cGFSessionTeleportState_gainFocus(void* self)
{
    void* gm = SSXApp->GameModeMan;
    func_0030B7F8(D_004A3DD8);
    float one = 1.0f;
    cGameModeMan_initGameMode(gm, 0xC);
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 0xA);
    cGame_partialRestart(SSXApp->Game);
    cRider_putAtStartpoint(*(void**)(*(char**)((char*)SSXApp->Game->AI + 0x40) + 0x18), *(int*)(iface + 0x10), 2);
    cRider_cleanupAfterPutAt(*(void**)(*(char**)((char*)SSXApp->Game->AI + 0x40) + 0x18), 1);
    void* transitionMan = func_00230698(SSXApp->Game, 0);
    char* fade = (char*)func_002E4CE8_sz(0x14);
    sColor_236058 colour;
    colour.r = one;
    colour.g = one;
    colour.b = one;
    colour.a = one;
    func_002E4D70(fade);
    *(void***)fade = cFade_TObj_vtbl;
    *(sColor_236058*)(fade + 4) = colour;
    func_002E4370(transitionMan, 1, 0, fade, 0.0f, 0.0f, 0, 0, one);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFSessionTeleportState_update);
#ifdef SKIP_ASM
extern "C" void func_00231250(void* self, int a, int b, int refresh);

/**
 * Updates the session-teleport state with one fixed call on the game object.
 *
 * Calls func_00231250(SSXApp->Game, 1, 0, 0), which stores 1 and 0 in two game fields (offsets 0x210 and
 * 0x21C) and skips its refresh step.
 */
extern "C" void cGFSessionTeleportState_update(void)
{
    func_00231250(SSXApp->Game, 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002361C8__FPv);
#ifdef SKIP_ASM
void* func_002361C8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002361E8__FPv);
#ifdef SKIP_ASM
void* func_002361E8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236208);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char cGFUserSelectState_vtbl[];
extern char* cGameState_mpGame;

extern "C" void* func_00236208(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    cGameState_mpGame = 0;
    *(void**)((char*)self + 0xC) = cGFUserSelectState_vtbl;
    *(int*)((char*)self + 0x0) = 4;
    *(int*)((char*)self + 0x18) = 0x17;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cAI_setAIState(void*, int);
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cGame_partialRestart(void* self);
extern "C" void func_0030B7F8(void* self);
extern "C" void func_0011D390(void* rider);
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(void* self, int i, int a, int b, int c, int d, int f, int g);
// PORT: cSSXScriptEngine_ReadyNISList returns func_00276270's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);
extern char* cGameState_mpGame;
extern void* D_004A3DD8;
extern int D_004A11BC;

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
extern "C" void cGFUserSelectState_gainFocus(void* self, int mode)
{
    func_0030B7F8(D_004A3DD8);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cAI_setAIState(*(void**)(cGameState_mpGame + 0xC), 3);
    *(int*)(*(char**)(*(char**)(*(char**)(cGameState_mpGame + 0xC) + 0x40) + 0x18) + 0x2F8) = 0;
    *(int*)((char*)self + 0x18) = 0x17;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = mode;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    if (mode == 1) {
        *(int*)((char*)self + 0x8) = mode;
        cBE_getInterface_Fv(cBE_getBE(), 0);
        *(int*)((char*)self + 0x18) = D_004A11BC;
    }
    if (*(int*)((char*)self + 0x18) == *cBENewRaceInterface_getLocation(iface))
        cGame_partialRestart(SSXApp->Game);
    int curMode = *(int*)((char*)self + 0x14);
    if (curMode == 0) {
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x16, 3, 0, -1, -1, -1);
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0x17, 1, 0, -1, -1, -1);
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
        cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 1, 0);
    } else if (curMode >= 0) {
        if (curMode < 4) {
            if (!isOne_6250(curMode))
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 1, 0xB, 3, 0, -1, -1, -1);
            cSSXScriptEngine_ReadyNISList(ScriptEngine, 1);
            func_0011D390(*(void**)(*(char**)((char*)SSXApp->Game->AI + 0x40) + 0x18));
            cSSXScriptEngine_PlayNISList(ScriptEngine, 1, *(int*)((char*)self + 0x14) == 2, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFUserSelectState_update);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void func_001F36C0(void* a, int b);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern "C" void func_0022CEA8(void* self, int a, int b);
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern "C" void cGameState_gotoNextState(void* self);
void func_00278DE8(void* self, int a);
extern "C" int cSSXScriptEngine_GetNISListScriptCategory(void* self, int a, int b);
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* a, int b);
extern "C" void cSSXScriptEngine_PlayTransportSequence(void* a, int b, int c);
extern int D_004A2594;
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];
extern int D_004428F8[];
extern signed char D_00535C10[];
extern signed char D_00535C11[];

struct sIface_6418 {
    char pad0[0x10];
    int f10;
};

struct sRace_6418 {
    char pad0[0x1C8];
    int f1C8;
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
    D_005366E8[++D_004A2A54] = 2;
    D_004A2A50 = D_004428F8[0];
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
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
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
    // Retail builds `flag` as a copy of a register holding 1, then `movn flag, zero, (D != 4)`: a condition the compiler
    // folded to the constant 1 (a local flag, e.g. a feature toggle). A literal 1 gives a run-time sltiu instead (96.81).
    int one = 1;
    int ns;
    switch (*(int*)((char*)self + 0x10)) {
    case 0: {
        int r = cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 1);
        if (!(r == 4 || r == 5 || r == 0))
            return;
        int curMode = *(int*)((char*)self + 0x14);
        if (curMode == 0) {
            cOVStateManager_triggerOverlay(0x1F);
            push2_6418();
            *(int*)((char*)self + 0x8) = 0;
        } else if (curMode == 2) {
            int flag = 0;
            if (atLeast_6418(*cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))))
                flag = one && D_00535C10[0] == 4;
            if (flag)
                D_004A2594 = 3;
            else
                D_004A2594 = 4;
            cOVStateManager_triggerOverlay(0x21);
            push2_6418();
            *(int*)((char*)self + 0x8) = 0;
        }
        ns = 1;
        break;
    }
    case 1:
        if (*(int*)((char*)self + 0x8) == 0)
            return;
        func_00278DE8(ScriptEngine, 0x1E);
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
                ((sIface_6418*)cBE_getInterface_Fv(cBE_getBE(), 0xA))->f10 = one;
                func_00231250(SSXApp->Game, 0xF, 0, 0);
                return;
            }
            if (destInBackcountry && D_00535C11[0] == 0)
                *(int*)((char*)self + 0x8) = 0;
            cSSXScriptEngine_PlayTransportSequence(ScriptEngine, !destInBackcountry && !curInBackcountry, 1);
            ns = 2;
        }
        break;
    case 2:
        if (*(int*)((char*)self + 0x14) != 0) {
            int r = cSSXScriptEngine_GetNISListScriptCategory(ScriptEngine, 1, 0);
            if (r == 0xE || r == 0x14) {
                if (*(int*)((char*)self + 0x8) == 0) {
                    if (*(int*)((char*)self + 0x1C) != 0)
                        return;
                    *(int*)((char*)self + 0x1C) = one;
                    *(int*)((char*)self + 0x8) = one;
                    return;
                }
                *(int*)((char*)self + 0x1C) = 0;
                ((sRace_6418*)(void*)SSXApp->Game->SectionMan)->f1C8 = one;
                func_0022CEA8(SSXApp->Game->SectionMan, *(int*)((char*)self + 0x18), 7);
                ns = 3;
                break;
            }
            if (r == 0xD || r == 0x13)
                func_001F36C0(SSXApp->Game->LoadIndicator, 0);
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
INCLUDE_ASM("main/gamestate", func_00236728);
#ifdef SKIP_ASM
struct sVEntry_func_00236728 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233AF0 also reads $a1 (the stream); bound with its real arity.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

extern "C" void func_00236728(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00236728* e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
    e = &(*(sVEntry_func_00236728**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002367C8);
#ifdef SKIP_ASM
struct sVEntry_func_002367C8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: func_00233B28 also reads $a1 (the stream); bound with its real arity.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

extern "C" void func_002367C8(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_002367C8* e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
    e = &(*(sVEntry_func_002367C8**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x18, 4);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236868);
#ifdef SKIP_ASM
extern char D_0047D6E8[];
extern char D_0047D2C8[];
extern char* cGameState_mpGame;

extern "C" void* func_00236868(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(void**)((char*)self + 0xC) = D_0047D6E8;
    cGameState_mpGame = 0;
    *(void**)((char*)self + 0xC) = D_0047D2C8;
    *(int*)((char*)self + 0x0) = 0xA;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002368A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBE_setState(int state);
extern "C" void cReplay_reset(void* p);
extern "C" void func_00145CB0(void* iface, int i, int a);
extern char* cGameState_mpGame;

struct sRace002368A0 {
    char pad0[0x30];
    int count;
    char pad34[0x15];
    signed char flag49;
};

extern "C" void func_002368A0(void* self)
{
    cBE_setState(0);
    cReplay_reset(*(void**)(cGameState_mpGame + 0x28));
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        int i = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        for (; i < cBENewRaceInterface_newRace.iNumberRiders; i++) {
            func_00145CB0(iface, i, 0);
        }
    }
    *(int*)((char*)self + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236960);
#ifdef SKIP_ASM
struct sVec4_236960 {
    float x, y, z, w;
} __attribute__((aligned(16)));

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

extern "C" int func_0022D278(void* self, int idx);
extern "C" void cSectionMan_activate(void* self, int idx, int mode);
extern "C" sVec4_236960 cRider_GetNISEndPosition(void* rider);
extern "C" void func_003A9658(void* cache, int idx, const sVec4_236960* pos, float radius);
extern "C" int cWorldView_getTestFocus(void* cache, int idx, sVec4_236960* pos, float* dist);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* p);
extern "C" void cSSXScriptEngine_EndTransportSequence(void* p);

extern "C" void func_00236960(void* self)
{
    char* g84 = (char*)SSXApp->Game;
    char* game = *(char**)(g84 + 0x78);
    if (*(int*)(game + 0x1C8) == 0) {
        return;
    }
    int state = *(int*)((char*)self + 0x10);
    char** cache = *(char***)(g84 + 0x10);
    int idx = *(int*)(*(char**)(*(char**)(g84 + 0x84) + 4) + 0xB0) == 0;
    switch (state) {
    case 0:
        if (func_0022D278(game, *(int*)(game + 0x1BC)) != 0) {
            char* r = *(char**)((char*)SSXApp->Game->AI + 0x40);
            sVec4_236960 pos = cRider_GetNISEndPosition(*(void**)(r + 0x18));
            char* e = *cache + idx * 0x50;
            *(int*)(e + 0x250) = 1;
            func_003A9658(*cache + 0x10, idx, &pos, 45000.0f);
            *(int*)((char*)self + 0x10) = 1;
        }
        break;
    case 1: {
        sVec4_236960 v;
        float dist;
        cWorldView_getTestFocus(*cache + 0x10, idx, &v, &dist);
        if (dist < 20000.0f) {
            break;
        }
        char* o = (char*)func_00230698(SSXApp->Game, 0);
        if (((cRiderObj_236960*)(o + 0x10))->isBusy() != 0) {
            break;
        }
        if (*(int*)(o + 0x44) != 0) {
            break;
        }
        if (cSSXScriptEngine_IsActiveNISInTransition(ScriptEngine) != 0) {
            break;
        }
        cSSXScriptEngine_EndTransportSequence(ScriptEngine);
        *(int*)(game + 0x1C8) = 0;
        cSectionMan_activate(game, *(int*)(game + 0x1BC), 7);
        *(int*)((char*)self + 0x10) = 2;
        break;
    }
    case 2:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236AF0__FPv);
#ifdef SKIP_ASM
void func_00236AF0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236AF8__FPv);
#ifdef SKIP_ASM
void* func_00236AF8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B18__FPv);
#ifdef SKIP_ASM
void* func_00236B18(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B38);
#ifdef SKIP_ASM
class cRace236B38 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};
extern char* cGameState_mpGame;

extern "C" void func_00236B38(void)
{
    (*(cRace236B38**)(cGameState_mpGame + 0xC))->v05(3);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B70__FPv);
#ifdef SKIP_ASM
void* func_00236B70(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236B90__FPv);
#ifdef SKIP_ASM
void* func_00236B90(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236BB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBE_setState(int state);
extern "C" void func_002EA820(void* p);
extern "C" void func_0020A8F8(int mode);
struct sMgr_6BB0 {
    char pad0[0x64];
    void* world;    // 0x64
};
struct sApp_6BB0 {
    char pad0[0x84];
    sMgr_6BB0* mgr; // 0x84
};
extern sApp_6BB0* D_004A28A8_s __asm__("SSXApp");
extern int D_004A2A50;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F4[];

struct sGameState_6BB0 {
    char pad0[8];
    int done;   // 0x8
};

// Ends the game state: pushes front-end state 1 unless cGameComm_mGameComm is active, then hands control back.
extern "C" void func_00236BB0(sGameState_6BB0* self)
{
    self->done = 0;
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mError != 0) {
        func_002EA820(D_004A28A8_s->mgr->world);
        cBE_setState(0);
        return;
    }
    func_0020A8F8(2);
    D_005366E8[++D_004A2A54] = 1;
    D_004A2A50 = D_004428F4[0];
    func_002EA820(D_004A28A8_s->mgr->world);
    cBE_setState(0);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236C48);
#ifdef SKIP_ASM
extern "C" void cGameState_gotoNextState(void* self);

extern "C" void func_00236C48(void* self)
{
    if (cGameComm_mGameComm == 0 || cGameComm_mGameComm->mError == 0) {
        if (*(int*)((char*)self + 8) == 1) {
            cGameState_gotoNextState(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236C88);
#ifdef SKIP_ASM
extern "C" void func_00236CD8(void* self);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_00236C88(void* self)
{
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
    func_00236CD8(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236CD8);
#ifdef SKIP_ASM
class cRace236CD8 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void* func_0028B180(void);
extern "C" void func_002871B0(void* mgr);
extern char* cGameState_mpGame;
extern signed char D_00535C10[];

static inline int isMode4_236CD8()
{
    return D_00535C10[0] == 4;
}

extern "C" void func_00236CD8(void* self)
{
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (!isMode4_236CD8()) {
        (*(cRace236CD8**)(cGameState_mpGame + 0xC))->v05(3);
        *(int*)(*(char**)(cGameState_mpGame + 0xC) + 0x14) = 1;
    }
    func_002871B0(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236D60__FPv);
#ifdef SKIP_ASM
void* func_00236D60(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236D80__FPv);
#ifdef SKIP_ASM
void* func_00236D80(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236DA0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00145CB0(void* iface, int i, int a);
extern "C" void cSSXScriptEngine_ResetNISList(void* a, int b);
extern "C" void func_0020A8F8(int a);

struct sRace00236DA0 {
    char pad0[0x30];
    int count;
    char pad34[0x15];
    signed char flag49;
};

extern "C" void func_00236DA0(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        int i = 0;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        for (; i < cBENewRaceInterface_newRace.iNumberRiders; i++) {
            func_00145CB0(iface, i, 0);
        }
    }
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    *(int*)((char*)self + 0x8) = 0;
    func_0020A8F8(7);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236E60);
#ifdef SKIP_ASM
extern "C" void func_00231250(void* self, int a, int b, int refresh);

extern "C" void func_00236E60(void* self)
{
    if (*(int*)((char*)self + 8) == 1) {
        func_00231250(SSXApp->Game, 0xE, 2, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EA0);
#ifdef SKIP_ASM
extern "C" void func_0039F840(void* list);
extern char* cGameState_mpGame;

extern "C" void func_00236EA0(void)
{
    func_0039F840(*(char**)(cGameState_mpGame + 0x48) + 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EC8__FPv);
#ifdef SKIP_ASM
void* func_00236EC8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236EE8__FPv);
#ifdef SKIP_ASM
void* func_00236EE8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFPodiumState_gainFocus);
#ifdef SKIP_ASM
extern "C" void* func_0028B180(void);
extern "C" void func_0028CDF8(void* mgr);

/**
 * Enters the podium state: calls func_0028CDF8() on the audio object and turns the AI's computer actors on.
 *
 * func_0028B180() returns gSSXAudio; the AI's ComputerActorsActive (+0x8C) is set to 1. On Tour's version takes
 * this and a startup flag, which the body does not use.
 */
extern "C" void cGFPodiumState_gainFocus(void)
{
    func_0028CDF8(func_0028B180());
    *(int*)((char*)SSXApp->Game->AI + 0x8C) = 1;
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
extern "C" void cGFPodiumState_update(void* self)
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
    *(int*)((char*)SSXApp->Game->AI + 0x8C) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236FA8__FPv);
#ifdef SKIP_ASM
void* func_00236FA8(void* self)
{
    return func_00233AF0(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00236FC8__FPv);
#ifdef SKIP_ASM
void* func_00236FC8(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", cGFReplayStartState_gainFocus);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_initMissions(void* a);
extern "C" void cWScriptMan_runGlobalScripts(void* a);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern void* D_004A3DD8;

/**
 * Starts a replay: initialises the missions and global scripts, hides the load indicator and opens the replay
 * overlay (0x11).
 */
extern "C" void cGFReplayStartState_gainFocus(void)
{
    cWScriptMan_initMissions(D_004A3DD8);
    cWScriptMan_runGlobalScripts(D_004A3DD8);
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 0);
    cOVStateManager_triggerOverlay(0x11);
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
extern "C" void func_00308F38(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern void* D_004A3DD8;

/**
 * Leaves the replay-start state by purging the world script missions and setting them up again.
 *
 * Calls func_00308F38 (On Tour cWScriptMan::purgeMissions) and cWScriptMan_setupMissions() on TheWScriptMan.
 */
extern "C" void cGFReplayStartState_loseFocus(void)
{
    func_00308F38(D_004A3DD8);
    cWScriptMan_setupMissions(D_004A3DD8);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237060);
#ifdef SKIP_ASM
extern "C" void* func_00237060(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x4) = a1;
    *(int*)((char*)self + 0x8) = a2;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x0) = a1 * a2;
    *(int*)((char*)self + 0x4) = (60 / a2) * a2;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002370A0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

struct sEntry_func_002370A0 {
    void* buf;
    int pad[3];
    int* a;
    int* b;
};

struct sObj_func_002370A0 {
    char pad[0x10];
    int count;
    sEntry_func_002370A0 entries[1];
};

extern "C" void func_002370A0(sObj_func_002370A0* self, int flags)
{
    int i;
    for (i = 0; i < self->count; i++) {
        sEntry_func_002370A0* e = &self->entries[i];
        if (e->buf)
            cMemMan_free(e->buf);
        operator_delete(e->a);
        operator_delete(e->b);
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237140);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: func_00244400__FPvi takes the 0x2A4-byte block as an int (pointer in int)
void func_00244400(void* self, int a1);
void func_00244408(void* self, int a1);
extern char str_InputSamples[];
extern char str_SyncedPad[];
extern char str_SyncedInput[];

struct sRec_237140 {
    int type;
    char pad[0x60];
};

struct sEntry_237140 {
    sRec_237140* recs;
    int f4;
    int f8;
    int fC;
    int* f10;
    void* f14;
};

struct sOwner_237140 {
    int f0;
    int count;
    int pad8[2];
    int n;
    sEntry_237140 entries[1];
};

static inline void* initObj_237140(void* o, int a, int b)
{
    func_00244400(o, a);
    func_00244408(o, b);
    return o;
}

extern "C" void* func_00237140(sOwner_237140* self, int* a1)
{
    sEntry_237140* e = &self->entries[self->n++];
    e->recs = (sRec_237140*)operator_new_tag(self->count * 100, str_InputSamples, 0, 0);
    MEM_fill(e->recs, 0, self->count * 100);
    for (int i = 0; i < self->count; i++) {
        e->recs[i].type = 0x18;
    }
    e->f4 = 0;
    e->f8 = self->f0;
    e->fC = a1[0];
    e->f10 = (int*)cMemMan_alloc(0x2A4, str_SyncedPad, 0, 0);
    *e->f10 = 0;
    void* o = initObj_237140(cMemMan_alloc(8, str_SyncedInput, 0, 0), (int)e->f10, a1[1]);
    e->f14 = o;
    return o;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237280);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: func_00244400__FPvi takes the 0x2A4-byte block as an int (pointer in int)
void func_00244400(void* self, int a1);
void func_00244408(void* self, int a1);
extern char str_InputSamples[];
extern char str_SyncedPad[];
extern char str_SyncedInput[];

struct sRec_237280 {
    int type;
    char pad[0x60];
};

struct sEntry_237280 {
    sRec_237280* recs;
    int f4;
    int f8;
    int fC;
    int* f10;
    void* f14;
};

struct sOwner_237280 {
    int f0;
    int count;
    int pad8[2];
    int n;
    sEntry_237280 entries[1];
};

static inline void* initObj_237280(void* o, int a, int b)
{
    func_00244400(o, a);
    func_00244408(o, b);
    return o;
}

extern "C" void* func_00237280(sOwner_237280* self, int a1)
{
    sEntry_237280* e = &self->entries[self->n++];
    e->recs = (sRec_237280*)operator_new_tag(self->count * 100, str_InputSamples, 0, 0);
    MEM_fill(e->recs, 0, self->count * 100);
    for (int i = 0; i < self->count; i++) {
        e->recs[i].type = 0x18;
    }
    e->f4 = 0;
    e->f8 = self->f0;
    e->fC = 0;
    e->f10 = (int*)cMemMan_alloc(0x2A4, str_SyncedPad, 0, 0);
    *e->f10 = 0;
    void* o = initObj_237280(cMemMan_alloc(8, str_SyncedInput, 0, 0), (int)e->f10, a1);
    e->f14 = o;
    return o;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002373B8);
#ifdef SKIP_ASM
// FAKEMATCH: redundant pointer local `psp` (`SyncedPad* psp = &syncedpads[i]; SyncedPad& sp = *psp;`) adds the one RTL insn
// that puts the outer loop over loop.c's 257-insn hoist threshold, so the constant 24 is not hoisted into s5; plain form 97.12%
// (99.69% with a ternary clamp). Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sPackSrc_7B28;
extern "C" void func_00237BE8(void* self, sPackSrc_7B28* src, char* buf);
extern "C" void func_00237A98(void* self, sPackSrc_7B28* src);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" int cGameComm_readPacket(void* self, int ch, void* dst, int size);
extern "C" float func_00258360(void* self);

// cInputSync::preupdate (On Tour name and member names; the SSX 3 version differs in structure).
// cGameComm::mGameComm through On Tour's GameComm() accessor.
static inline char* GameComm() { return (char*)cGameComm_mGameComm; }

// cInputSync::PadSample (On Tour, size 0x64).
struct PadSample_2373B8 {
    int NumButtons;
    float Buttons[24];
};

// cInputSync::CompressedSample (SSX 3 layout: no PlayerID byte, size 0x1C).
struct CompressedSample_2373B8 {
    unsigned char NumButtons;
    unsigned char NonZeroBitBytes[3];
    unsigned char NonZeroBytes[24];
};

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

// cInputSync::SyncedPad (SSX 3 layout: no sendpos, size 0x18).
struct SyncedPad_2373B8 {
    PadSample_2373B8* samples;          // 0x00: ring of samples
    int readpos;                        // 0x04
    int writepos;                       // 0x08
    cInputPad_2373B8* sourcepad;        // 0x0C: local pad (0 = remote)
    void* destinationpad;               // 0x10
    void* input;                        // 0x14
};

struct cInputSync_2373B8 {
    int sendlimit;                      // 0x00
    int latency;                        // 0x04: ring size
    int numsamples;                     // 0x08: copies per sample
    int tick;                           // 0x0C
    int numsyncedpads;                  // 0x10
    SyncedPad_2373B8 syncedpads[1];     // 0x14
};

extern "C" void func_002373B8(cInputSync_2373B8* self)
{
    PadSample_2373B8 sample;
    CompressedSample_2373B8 compsample;
    if (*(int*)(GameComm() + 0x70) < 14) {
        return;
    }
    for (int i = 0; i < self->numsyncedpads; i++) {
        SyncedPad_2373B8* psp = &self->syncedpads[i];
        SyncedPad_2373B8& sp = *psp;
        if (sp.sourcepad == 0) {
            char* gc = GameComm();
            int net = *(int*)(gc + 0x40) != 0 && *(int*)(gc + 4) == 0 && *(int*)(gc + 0) != 0;
            if (net) {
                while (sp.writepos - sp.readpos < self->latency - (self->numsamples - 1) &&
                       cGameComm_readPacket(GameComm(), 0, &compsample, sizeof(compsample))) {
                    func_00237BE8(self, (sPackSrc_7B28*)&sample, (char*)&compsample);
                    for (int j = 0; j < self->numsamples; j++) {
                        sp.samples[sp.writepos % self->latency] = sample;
                        sp.writepos++;
                    }
                }
            } else {
                MEM_fill(&compsample, 0, sizeof(compsample));
                compsample.NumButtons = 24;
                func_00237BE8(self, (sPackSrc_7B28*)&sample, (char*)&compsample);
                while (sp.writepos - sp.readpos < self->latency - (self->numsamples - 1)) {
                    for (int j = 0; j < self->numsamples; j++) {
                        sp.samples[sp.writepos % self->latency] = sample;
                        sp.writepos++;
                    }
                }
            }
        } else if (sp.writepos - sp.readpos < self->sendlimit - (self->numsamples - 1)) {
            self->tick++;
            if (self->tick % self->numsamples == 0) {
                PadSample_2373B8 localsample;
                localsample.NumButtons = sp.sourcepad->getNumButtons();
                for (int j = 0; j < localsample.NumButtons; j++) {
                    localsample.Buttons[j] = sp.sourcepad->getButtonValue(j);
                }
                func_00237A98(self, (sPackSrc_7B28*)&localsample);
                for (int j = 0; j < self->numsamples; j++) {
                    sp.samples[sp.writepos % self->latency] = localsample;
                    sp.writepos++;
                }
                int n = (int)(func_00258360(GameComm()) / *(float*)((char*)SSXApp + 0x14));
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
INCLUDE_ASM("main/gamestate", func_00237948__FPv);
#ifdef SKIP_ASM
void func_00237948(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237950);
#ifdef SKIP_ASM
struct sSlot237950 {
    int f0;
    int cur;
    int max;
    int pad[3];
};
struct sOwner237950 {
    char pad[0x10];
    int count;
    sSlot237950 slots[1];
};

extern "C" int func_00237950(sOwner237950* self)
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
    for (i = 0; i < self->count; i++) {
        sSlot237950* s = &self->slots[i];
        if (s->cur == s->max) {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_002379C8);
#ifdef SKIP_ASM
extern "C" void func_00321298(void* target, int a, void* data);

struct sFrame002379C8 {
    int first;
    char data[0x60];
};
struct sTrack002379C8 {
    sFrame002379C8* frames;
    int cur;
    int f8;
    int fC;
    void* target;
    int f14;
};
struct sPlayer002379C8 {
    int f0;
    int nFrames;
    int f8;
    int fC;
    int count;
    sTrack002379C8 tracks[1];
};

extern "C" int func_002379C8(sPlayer002379C8* self)
{
    if (cGameComm_mGameComm->mState < 14) {
        return 0;
    }
    if (func_00237950((sOwner237950*)self) == 0) {
        return 0;
    }
    int i;
    for (i = 0; i < self->count; i++) {
        sTrack002379C8* t = &self->tracks[i];
        sFrame002379C8* f = &t->frames[t->cur % self->nFrames];
        func_00321298(t->target, f->first, f->data);
        t->cur++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237A98);
#ifdef SKIP_ASM
struct sPackSrc_7B28;
struct sPackDst_7B28;
extern "C" int func_00237B28(void* self, sPackSrc_7B28* src, sPackDst_7B28* dst);
extern "C" void func_00237BE8(void* self, sPackSrc_7B28* src, char* buf);
extern "C" void func_00257E98(void* mgr, int a, char* buf, int len);

extern "C" void func_00237A98(void* self, sPackSrc_7B28* src)
{
    char buf[32];
    int len = func_00237B28(self, src, (sPackDst_7B28*)buf);
    func_00237BE8(self, src, buf);
    cGameComm* gameComm = cGameComm_mGameComm;
    int ok = gameComm->mSyncInput != 0 && gameComm->mError == 0 && gameComm->mConnected != 0;
    if (ok) {
        func_00257E98((char*)cGameComm_mGameComm, 0, buf, len);
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237B28);
#ifdef SKIP_ASM
struct sPackSrc_7B28 {
    int count;
    float vals[1];
};

struct sPackDst_7B28 {
    unsigned char count;
    unsigned char mask[3];
    unsigned char vals[1];
};

extern "C" int func_00237B28(void* self, sPackSrc_7B28* src, sPackDst_7B28* dst)
{
    int i;
    int j;
    int n;
    dst->count = src->count;
    for (j = 0; j < 3; j++) {
        dst->mask[j] = 0;
    }
    n = 0;
    for (i = 0; i < src->count; i++) {
        unsigned char v = (int)(src->vals[i] * 255.0f);
        if (v != 0) {
            dst->mask[i >> 3] |= 1 << (i & 7);
            dst->vals[n++] = v;
        }
    }
    return n + 4;
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237BE8);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

extern "C" void func_00237BE8(void* self, sPackSrc_7B28* out, char* buf)
{
    sPackDst_7B28* in = (sPackDst_7B28*)buf;
    out->count = in->count;
    MEM_fill(out->vals, 0, 0x60);
    int k = 0;
    int i;
    for (i = 0; i < out->count; i++) {
        if ((in->mask[i >> 3] >> (i & 7)) & 1) {
            out->vals[i] = in->vals[k++] * 0.003921568859368563f;
        } else {
            out->vals[i] = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237CB0);
#ifdef SKIP_ASM
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed);
extern uint D_004C9548[];

extern "C" void func_00237CB0(uint seed)
{
    cBxPseudoRng_Seed(D_004C9548, seed);
}
#endif

//100%
INCLUDE_ASM("main/gamestate", func_00237CD8);
#ifdef SKIP_ASM
extern "C" uint cBxPseudoRng_NextInt(uint* state);
extern uint D_004C9548[];

extern "C" uint func_00237CD8(void)
{
    return cBxPseudoRng_NextInt(D_004C9548);
}
#endif

