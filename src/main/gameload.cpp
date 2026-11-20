#include "common.h"
#include "main/loadscreens.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("main/gameload", cGame_load);
#ifdef SKIP_ASM
// FAKEMATCH: an unused 11-byte copy `name` of D_0047B6A8 (the call passes D_0047B6A8.c directly); plain form 99.21%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
void cMemMan_free(void* p);
void cLoadTimer_startload(void* bar, float v);
void cLoadTimer_setnextpercent(void* bar, float v);
extern "C" void cFELocale_loadOV(void* p);
extern "C" void cSSXScriptEngine_Load(void* eng);
extern "C" void* cFont_cFont(void* p, const char* name, int a);
extern "C" void cGameComm_syncGame(void* comm);
void func_0014BD98(void* iface);
extern "C" void cBucketMan_init(void* bm, int n);
extern "C" void cBucketMan_setsortflag(void* bm, int a);
extern "C" void cCameraTriggerMan_init(void* p);
void* cStreamPause_construct();
void* cOcclusionMan_cOcclusionMan(void* p);
extern "C" void* cWorld_cWorld(void* p);
extern "C" void* cWorldLightMan_cWorldLightMan(void* p, void* a);
extern "C" void* cWScriptMan_cWScriptMan(void* p);
extern "C" void* func_00229180(void* p);
extern "C" void* func_00343A68(void* p);
extern "C" void* cMultiParticleMan_cMultiParticleMan(void* p);
extern "C" void* cWScriptCompile_parseKeywords(void* p, int a, int b);
extern "C" void cIrradianceDataBase_Load(void* p);
void* cBXLoadIndicator_cBXLoadIndicator(void* p);
struct cBigFile;
void* cBigFile_cBigFile(cBigFile* p);
extern "C" void cGame_loadTrack(cGame* self);
extern "C" void* cReplay_cReplay(void* p);
extern "C" void* cFXLoader_cFXLoader(void* p, int a);
extern "C" void* cInputSync_cInputSync(void* p, int n, int m);
int BXrand();
extern "C" void GMsrand(int seed);
extern "C" void* cAI_cAI(void* p);
extern "C" void* cPlaybackMan_cPlaybackMan(void* p, int n);
extern "C" void cReplay_init(void* p);
extern "C" void cAI_initAIRiders(void* ai);
extern "C" void cGameModeMan_initGameMode(void* p, int mode);
extern "C" char* cBESaveInterface_getGameReplayBuffer(void* iface);
extern "C" void cAI_setPodiumActorList(void* ai, void* p);
extern "C" void cAI_initAIActors(void* ai, int a);
extern "C" void cAI_loadRiders(void* ai);
extern "C" void THREAD_yieldticks(int n);
extern "C" int cAI_isAILoadedAndReady(void* ai);
void cReplay_initLoad(void* p);
extern "C" void cReplay_load(void* p, void* a);
void cReplay_purgeLoad(void* p);
extern "C" void cBESaveInterface_releaseReplayBuffer(void* iface);
extern "C" void* cGameViewMan_cGameViewMan(void* p, int a);
extern "C" void cCamera_switchChaseControllerTargetRider(void* p, void* a);
extern "C" void* cMenuSys_cMenuSys(void* p);
extern "C" char* FILE_load(const char* name, int flags);
extern "C" void* cTransitionMan_cTransitionMan(void* p, int a, int i);
extern "C" void* cSplashManager_cSplashManager(void* p, int a, int i);
extern "C" void* cCutSceneTransitionMan_cCutSceneTransitionMan(void* p, int a);
extern "C" void* cFlagManager_cFlagManager(void* p, int a);
extern "C" void* cSnowFall_cSnowFall(void* p, int a);
extern "C" void cSSXAudio_InGameLoad(void* p);
extern "C" void* cUIEngine_cUIEngine(void* p);
extern "C" void cUIEngine_loadFile(void* ui, char* name);
extern "C" void* cOVSound_cOVSound(void* p);
extern "C" void* cOVStateHUD1P_cOVStateHUD1P(void* p, void* ui, int a);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
void cReplay_setReplayPadNum(void* p, int a);
void cGame_setPreloadState(void* self, int val);
extern "C" void cGame_updateState(cGame* self);
extern "C" void THREAD_yield(int a);
extern "C" int cGame_isPreloadedStateReady(cGame* self);
extern "C" void cGameComm_syncWait(void* comm);
extern "C" void cGame_setState(cGame* self, int a, int b, int refresh);
extern "C" void* cObjNode_cObjNode1(void* p, int a);
extern "C" void* cAIVisualEffectsMan_construct();
extern "C" void* cVisualEffectsMan_construct();
extern "C" void* cSSXStatsMan_construct();
extern char* cAppMan_TheAppMan;
extern int cBESaveInterface_bDoReplay;
extern int D_004A3484;
extern void* cGameState_mpGame;
extern char D_004A5988_gl __asm__("gBucketMan");
extern char cBENewRaceInterface_newRace[];
extern int D_00535BFC[];
extern char gCamTriggerMan[];
extern char D_005047F8[];
extern char str_Nash[];
extern char str_DebugFont_b[];
extern char str_data_fonts_debugfnt_sfn_b[];
extern char str_OcclusionMan[];
extern char str_World[];
extern char str_WorldLightMan[];
extern char str_WScriptMan[];
extern char str_CrowdMan2d[];
extern char str_CrowdMan3d[];
extern char str_MultiParticleMan[];
extern char str_InstancesMan[];
extern char str_LoadIndicator[];
extern char str_BigFile[];
extern char str_Replay[];
extern char str_FXLoader[];
extern char str_InputSync[];
extern char str_AI[];
extern char str_PlaybackMan[];
extern char str_GameViewMan[];
extern char str_MenuSys[];
extern char str_data_fonts_menu_ssh_b[];
extern char str_menu_b[];
extern char str_cTransitionMan[];
extern char str_cSplashManager[];
extern char str_cCutSceneTransitionMan[];
extern char str_cFlagManager[];
extern char str_SnowFall[];
extern char str_UIEngine_b[];
extern char str_OVUIInput[];
extern char str_OVUISound[];
extern char str_cOVStateHUD1P[];
extern char str_AIVisualEffectsMan[];
extern char str_VisualEffectsMan[];
extern char str_SSXStatsMan[];
extern void* cFEInput_vtbl[];
extern void* cAIVisualEffectsObjNode_vtbl[];
extern void* cVisualEffectsObjNode_vtbl[];
extern void* cSSXStatsObjNode_vtbl[];

struct sVEntryGL {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sVEntryVoidGL {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sName11GL { char c[11]; };
extern sName11GL D_0047B6A8;

inline void* operator new(unsigned int, void* p) { return p; }

struct sCellGL {
    char pad[0x10];
    int value;
    sCellGL() { value = 0; }
};

struct sGridGL {
    sCellGL cells[2][2][2];
    int a0;
    int a4;
};

struct sUIDataGL {
    int pad0;
    char flag;
    void** vtbl;
    int c;
    int d;
    int e;
    int f;
};

struct sNamedGL {
    char pad[0xC];
    void** vtbl;
    void* obj;
};

struct sStateGL {
    char pad[0x1C];
    unsigned int pad0 : 8;
    unsigned int mode : 6;
    unsigned int rest : 18;
};

struct sUIGL {
    char pad[0x8];
    void* p8;
    sUIDataGL* data;
    void* p10;
    void* p14;
    char stack[1];
};


/**
 * Loads a race: creates the game's managers, loads the track and enters the first game state.
 *
 * Progress (probably the load bar, cLoadTimer_setnextpercent) goes from 13.2 % to 99 %. In order it creates the Nash
 * grid, the debug font, the world and its managers and loads the track (cGame_loadTrack). Then come the
 * replay, the network input sync (network games only) and the AI. Last are the views, the debug menu, the
 * transition and splash managers, the in-game audio, the UI engine, and the AI visual-effects, visual-effects
 * and stats nodes. When cBESaveInterface_bDoReplay is set (the 512 KB save buffer exists; probably a saved replay) the riders
 * are loaded here, the replay is restored from that buffer and the game enters state 16. Otherwise game state
 * 10 is preloaded and then entered.
 */
extern "C" void cGame_load(cGame* self)
{
    sGridGL** pGrid = (sGridGL**)&self->BXNash;
    void** worlds = ((void**)&self->TransitionMan[0]);
    void* bm = &D_004A5988_gl;
    void** lights = ((void**)&self->SplashManager[0]);
    cLoadTimer_startload(cAppMan_TheAppMan + 0x38, 13.199999809265137f);
    cLoadTimer_setnextpercent(cAppMan_TheAppMan + 0x38, 43.5f);
    cFELocale_loadOV(SSXApp->FELocale);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryVoidGL* vt = (sVEntryVoidGL*)ps2GraphicsMan->mVtbl;
        vt[44].fn((char*)ps2GraphicsMan + vt[44].delta, 0);
    }
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cSSXScriptEngine_Load(ScriptEngine);
    sGridGL* grid = new (cMemMan_alloc(168, str_Nash, 0, 0)) sGridGL;
    *pGrid = grid;
    grid->a4 = 0;
    grid->a0 = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    self->DebugFont = (cFont*)cFont_cFont(operator new(132, str_DebugFont_b, 0, 0), str_data_fonts_debugfnt_sfn_b, 0);
    if (cGameComm_mGameComm != 0)
        cGameComm_syncGame(cGameComm_mGameComm);
    func_0014BD98(cBE_getInterface_Fv(cBE_getBE(), 9));
    cBucketMan_init(bm, 9);
    cBucketMan_setsortflag(bm, 1);
    cCameraTriggerMan_init(gCamTriggerMan);
    self->StreamPause = (cStreamPause*)cStreamPause_construct();
    self->OcclusionMan = (cOcclusionMan*)cOcclusionMan_cOcclusionMan(operator new(276, str_OcclusionMan, 0x20000000, 0));
    self->World = (cWorld*)cWorld_cWorld(operator new(0x18, str_World, 0, 0));
    self->WorldLightMan = (cWorldLightMan*)cWorldLightMan_cWorldLightMan(operator new(12960, str_WorldLightMan, 0x20000000, 0), (sGridGL*)self->BXNash);
    self->WScriptMan = (cWScriptMan*)cWScriptMan_cWScriptMan(operator new(1176, str_WScriptMan, 0x20000000, 0));
    self->CrowdMan2d = (cCrowdMan2d*)func_00229180(operator new(10016, str_CrowdMan2d, 0x20000000, 0));
    self->CrowdMan3d = (cCrowdMan3d*)func_00343A68(operator new(2500, str_CrowdMan3d, 0x20000000, 0));
    self->MultiParticleMan = (cMultiParticleMan*)cMultiParticleMan_cMultiParticleMan(operator new(0x10, str_MultiParticleMan, 0x20000000, 0));
    self->InstanceMan = (cInstanceMan*)cWScriptCompile_parseKeywords(operator new(0x10, str_InstancesMan, 0x20000000, 0), 0x80, 0x40);
    cIrradianceDataBase_Load(D_005047F8);
    self->LoadIndicator = (cBXLoadIndicator*)cBXLoadIndicator_cBXLoadIndicator(operator new(0x10, str_LoadIndicator, 0x20000000, 0));
    self->mpTrackBigFile = (cBigFile*)cBigFile_cBigFile((cBigFile*)operator new(8, str_BigFile, 0, 0));
    cGame_loadTrack(self);
    self->Replay = (cReplay*)cReplay_cReplay(operator new(1672, str_Replay, 0, 0));
    self->FXLoader = (cFXLoader*)cFXLoader_cFXLoader(operator new(1, str_FXLoader, 0, 0), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        float dt = *(float*)((char*)SSXApp + 0x14);
        int n = (int)((cBENetworkInterface_networkOpt.fLatency + (float)cGameComm_mGameComm->mNumSamples * dt) / dt) + 2;
        self->InputSync = (cInputSync*)cInputSync_cInputSync(operator new(68, str_InputSync, 0, 0), n, cGameComm_mGameComm->mNumSamples);
    } else {
        GMsrand(BXrand());
    }
    {
        char* ai = (char*)cAI_cAI(operator new(208, str_AI, 0, 0));
        self->AI = (cAI*)ai;
        sVEntryVoidGL* vt = *(sVEntryVoidGL**)(ai + 0xCC);
        vt[2].fn(ai + vt[2].delta);
    }
    cLoadTimer_setnextpercent(cAppMan_TheAppMan + 0x38, 55.0f);
    self->PlaybackMan = (cPlaybackMan*)cPlaybackMan_cPlaybackMan(operator new(0x7C, str_PlaybackMan, 0x20000000, 0), 40);
    cReplay_init(self->Replay);
    void* race = cBE_getInterface_Fv(SSXApp->BE, 5);
    int replay = cBESaveInterface_bDoReplay;
    if (replay != 0) {
        cAI_initAIRiders((char*)self->AI);
        signed char* gs = (signed char*)cBENewRaceInterface_newRace;
        cGameModeMan_initGameMode(SSXApp->GameModeMan, gs[0x4A]);
        cAI_setPodiumActorList((char*)self->AI, cBESaveInterface_getGameReplayBuffer(race) + 0x20);
        int mode = gs[0x48];
        int sub = gs[0x4A];
        if (mode == 4) {
        } else if (mode == 5 && sub != 4) {
        } else if (mode == 6 && sub != 5) {
        } else {
            char* ai = (char*)self->AI;
            if (*(int*)(ai + 0x80) == 0) {
                if (mode == 0)
                    cAI_initAIActors(ai, 0);
                else
                    cAI_initAIActors(ai, 2);
                *(int*)((char*)self->AI + 0x8C) = 0;
            }
        }
        while (!cAI_isAILoadedAndReady((char*)self->AI)) {
            cAI_loadRiders((char*)self->AI);
            THREAD_yieldticks(1);
        }
        cReplay_initLoad(self->Replay);
        cReplay_load(self->Replay, cBESaveInterface_getGameReplayBuffer(race));
        cReplay_purgeLoad(self->Replay);
        cBESaveInterface_releaseReplayBuffer(race);
    }
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0)
        self->ViewMan = (cGameViewMan*)cGameViewMan_cGameViewMan(operator new(0x38, str_GameViewMan, 0x20000000, 0), 1);
    else
        self->ViewMan = (cGameViewMan*)cGameViewMan_cGameViewMan(operator new(0x38, str_GameViewMan, 0x20000000, 0), D_00535BFC[0]);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cBENetworkInterface_networkOpt.bIsHost == 0) {
        char* secondPlayer = *(char**)((char*)self->AI + 0x44);
        void* cam = secondPlayer != 0 ? *(void**)(secondPlayer + 0x18) : 0;
        cCamera_switchChaseControllerTargetRider(*(void**)((char*)self->ViewMan + 0x4), cam);
    }
    char* hud = (char*)cMenuSys_cMenuSys(operator new(104, str_MenuSys, 0, 0));
    self->MenuSys = (cMenuSys*)hud;
    *(void**)(hud + 0x58) = (void*)self->DebugFont;
    *(void**)((char*)self->MenuSys + 0x5C) = (void*)SSXApp->Inputs[0];
    char* file = FILE_load(str_data_fonts_menu_ssh_b, 0x100);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryGL* vt = (sVEntryGL*)ps2GraphicsMan->mVtbl;
        int result = vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, file + *(int*)(file + 0x14), str_menu_b, 0, 1, -1);
        self->mBoxTextureID = result;
        *(int*)((char*)self->MenuSys + 0x60) = result;
    }
    if (file != 0)
        cMemMan_free(file);
    cLoadTimer_setnextpercent(cAppMan_TheAppMan + 0x38, 93.0f);
    for (unsigned int i = 0; i < 2; i++) {
        worlds[i] = cTransitionMan_cTransitionMan(operator new(132, str_cTransitionMan, 0, 0), 3, i);
        lights[i] = cSplashManager_cSplashManager(operator new(4144, str_cSplashManager, 0, 0), 2, i);
    }
    self->CutSceneTransitionMan = (cCutSceneTransitionMan*)cCutSceneTransitionMan_cCutSceneTransitionMan(operator new(72, str_cCutSceneTransitionMan, 0, 0), 3);
    self->FlagManager = (cFlagManager*)cFlagManager_cFlagManager(operator new(5912, str_cFlagManager, 0, 0), 2);
    self->SnowFall = (cSnowFall*)cSnowFall_cSnowFall(operator new(0x60, str_SnowFall, 0, 0), 2);
    self->mUpdateTick = 0;
    cSSXAudio_InGameLoad(gSSXAudio);
    cLoadTimer_setnextpercent(cAppMan_TheAppMan + 0x38, 99.0f);
    sUIGL* ui = (sUIGL*)cUIEngine_cUIEngine(operator new(0x70, str_UIEngine_b, 0, 0));
    self->UI = (cUIEngine*)ui;
    ui->p10 = (void*)SSXApp->FELocale;
    sName11GL name = D_0047B6A8;
    cUIEngine_loadFile((sUIGL*)self->UI, D_0047B6A8.c);
    sUIDataGL* data = (sUIDataGL*)operator new(0x1C, str_OVUIInput, 0, 0);
    data->flag = 0;
    data->vtbl = cFEInput_vtbl;
    data->f = 0;
    data->c = 0;
    data->d = 0;
    data->e = 0;
    ((sUIGL*)self->UI)->data = data;
    ((sUIGL*)self->UI)->p14 = cOVSound_cOVSound(operator new(8, str_OVUISound, 0, 0));
    ((sUIGL*)self->UI)->p8 = (void*)SSXApp->Fonts;
    sStateGL* st = (sStateGL*)cOVStateHUD1P_cOVStateHUD1P(operator new(3224, str_cOVStateHUD1P, 0, 0), (sUIGL*)self->UI, 0);
    st->mode = 5;
    cUIStateStack_pushExplicit(((sUIGL*)self->UI)->stack, st);
    self->mPreloadingState = 0;
    self->mPrevState = 0;
    self->mCurrentState = 0;
    self->mPreloadStateID = 0;
    self->mPendingPreloadStateID = 0;
    self->mPrevStateID = 0;
    self->mCurrentStateID = 0;
    self->mPendingStateID = 0;
    self->mPendingStateStartupFlag = 0;
    cGameState_mpGame = (self);
    if (replay != 0) {
        cReplay_setReplayPadNum(self->Replay, D_004A3484);
        cGame_setState((self), 0x10, 0, 0);
    } else {
        cGame_setPreloadState((self), 10);
        do {
            cGame_updateState((self));
            THREAD_yield(0);
        } while (!cGame_isPreloadedStateReady((self)));
        if (cGameComm_mGameComm != 0)
            cGameComm_syncWait(cGameComm_mGameComm);
        if (cBENewRaceInterface_newRace[0x49] == 0)
            cGame_setState((self), 10, 0, 0);
        else
            cGame_setState((self), 10, 0, 0);
    }
    sNamedGL* objNode = (sNamedGL*)operator new(20, str_AIVisualEffectsMan, 0, 0);
    cObjNode_cObjNode1(objNode, 6);
    objNode->vtbl = cAIVisualEffectsObjNode_vtbl;
    void* result = cAIVisualEffectsMan_construct();
    self->AIVisualEffectsMan = (cAIVisualEffectsObjNode*)objNode;
    objNode->obj = result;
    objNode = (sNamedGL*)operator new(20, str_VisualEffectsMan, 0, 0);
    cObjNode_cObjNode1(objNode, 3);
    objNode->vtbl = cVisualEffectsObjNode_vtbl;
    result = cVisualEffectsMan_construct();
    self->VisualEffectsMan = (cVisualEffectsObjNode*)objNode;
    objNode->obj = result;
    objNode = (sNamedGL*)operator new(20, str_SSXStatsMan, 0, 0);
    cObjNode_cObjNode1(objNode, 4);
    objNode->vtbl = cSSXStatsObjNode_vtbl;
    result = cSSXStatsMan_construct();
    objNode->obj = result;
    self->SSXStatsMan = (cSSXStatsObjNode*)objNode;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_loadTrack);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sBXStr_F6B0 {
    char* p;
    sBXStr_F6B0() {}
    sBXStr_F6B0(const sBXStr_F6B0&);
};

extern char* cBXString_EmptyString;
extern int D_004A11B8;
extern char str_data_worlds_c[];
extern char str_big_b[];
extern char str_SkyBox[];
extern char str_SectionMan[];
extern char str_SurfPropCache[];
extern char str_cWorldSound[];
extern char str_cPS2FogParticleMan[];
extern char str_bar_b[];
extern char str_cam[];
extern char gCamTriggerMan[];
extern void* D_004882E0[];

extern "C" void* cBXString_cBXString2(sBXStr_F6B0* self, const char* s);
extern "C" sBXStr_F6B0* func_00318630(sBXStr_F6B0* self, sBXStr_F6B0* a, const char* str);
extern "C" sBXStr_F6B0* func_003186D0(sBXStr_F6B0* self, const char* str, sBXStr_F6B0* b);
extern "C" void cBXString__cBXString(sBXStr_F6B0* self, int flags);
extern "C" sBXStr_F6B0* cBXString_operatorE(sBXStr_F6B0* self, sBXStr_F6B0* o);
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cBigFile_open(void* bf, const char* name, int flags);
extern "C" void cWorld_load(void* a, const char* name, void* b);
void* cSkyBox_cSkyBox(void* p);
extern "C" void* cSectionMan_cSectionMan(void* p, void* game);
extern "C" void func_002C2F18(void* p);
extern "C" void* cWorldSound_cWorldSound(void* p, int a);
extern "C" void cBXNash_initTheAIFwdDiffCache(void* p);
extern "C" void cSectionMan_activate(void* p, int id, int a);
extern "C" int cSectionMan_isLoaded(void* p, int id);
extern "C" void cSectionMan_update(void* p);
void cWorld_update(void* p);
extern "C" void THREAD_yield(int a);
// PORT: cCameraTriggerMan_loadTriggersIfNotStreaming__FPv also receives the path string (passed through in $5)
void cCameraTriggerMan_loadTriggersIfNotStreaming_F6B0(void* p, const char* s) __asm__("cCameraTriggerMan_loadTriggersIfNotStreaming__FPv");
extern "C" void cFogParticleMan_cFogParticleMan(void* p);
void* cLensFxMan_construct();
extern "C" void cRenderStateMan_init();

struct sElem4_F6B0 {
    int v;
    sElem4_F6B0() {}
};

struct sRow_F6B0 {
    sElem4_F6B0 e[4];
    char pad[0xB0 - 0x10];
    sRow_F6B0() {}
};

struct sTbl_F6B0 {
    sRow_F6B0 rows[19];
    static void* operator new(unsigned int size, const char* tag, unsigned int flags, int d)
    {
        return cMemMan_alloc(size, tag, flags, d);
    }
    sTbl_F6B0() {}
};

struct sElemFF_F6B0 {
    char b[0x30];
    sElemFF_F6B0() {}
    static void* operator new[](unsigned int, void* p) { return p; }
};

/**
 * Loads the race's track: opens its big file, loads the world and streams in the start section.
 *
 * Opens "data/worlds/" + name + ".big", where the name is the location's +0x44 string when D_004A11B8 is set,
 * otherwise its +0x34 string. It creates the sky box, the section manager, the surface property cache and the
 * world sound. It then activates the location's start section and updates the section manager until that
 * section is in. It also passes the track's ".cam" file to cCameraTriggerMan_loadTriggersIfNotStreaming, and creates the fog particle
 * manager ("cPS2FogParticleMan") and the lens effects manager.
 */
extern "C" void cGame_loadTrack(cGame* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    sBXStr_F6B0 name;
    name.p = cBXString_EmptyString;
    if (D_004A11B8 != 0) {
        sBXStr_F6B0 worldPath;
        sBXStr_F6B0 locName;
        func_003186D0(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x44), &locName));
        cBXString_operatorE(&name, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    } else {
        sBXStr_F6B0 worldPath;
        sBXStr_F6B0 locName;
        func_003186D0(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x34), &locName));
        cBXString_operatorE(&name, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    }
    {
        sBXStr_F6B0 path;
        func_00318630(&path, &name, str_big_b);
        cBigFile_open(self->mpTrackBigFile, path.p, 0);
        cBXString__cBXString(&path, 2);
    }
    sTbl_F6B0*& tbl = (sTbl_F6B0*)self->SurfPropCache;
    char*& ent = (char*)self->FogMan;
    cWorld_load(self->World, name.p, self->BXNash);
    *(void**)&self->SkyBox = cSkyBox_cSkyBox(cMemMan_alloc(0x18, str_SkyBox, 0, 0));
    *(void**)&self->SectionMan = cSectionMan_cSectionMan(cMemMan_alloc(468, str_SectionMan, 0, 0), ((char*)self));
    tbl = new (str_SurfPropCache, 0, 0) sTbl_F6B0;
    func_002C2F18(self->SurfPropCache);
    *(void**)&self->WorldSound = cWorldSound_cWorldSound(cMemMan_alloc(592, str_cWorldSound, 0, 0), 0);
    cBXNash_initTheAIFwdDiffCache(self->BXNash);
    sBXStr_F6B0 dir;
    dir.p = cBXString_EmptyString;
    *(int*)(*(char**)self->World + 0x3D4) = 0;
    {
        sBXStr_F6B0 worldPath;
        sBXStr_F6B0 locName;
        func_003186D0(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x34), &locName));
        cBXString_operatorE(&dir, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    }
    int id = *(int*)cBENewRaceInterface_getLocation(iface);
    cSectionMan_activate(self->SectionMan, id, 0);
    *(int*)((char*)self->SectionMan + 0x1A4) = 1;
    while (!cSectionMan_isLoaded(self->SectionMan, id)) {
        cSectionMan_update(self->SectionMan);
        cWorld_update(self->World);
        THREAD_yield(0);
    }
    *(int*)(*(char**)self->World + 0x3D4) = 1;
    {
        sBXStr_F6B0 worldPath;
        sBXStr_F6B0 camBasePath;
        cCameraTriggerMan_loadTriggersIfNotStreaming_F6B0(gCamTriggerMan, (func_003186D0(&camBasePath, str_bar_b, &dir), func_00318630(&worldPath, &camBasePath, str_cam), worldPath.p));
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&camBasePath, 2);
    }
    char* obj = (char*)cMemMan_alloc(12304, str_cPS2FogParticleMan, 0, 0);
    cFogParticleMan_cFogParticleMan(obj);
    *(void***)(obj + 4) = D_004882E0;
    new (obj + 0x10) sElemFF_F6B0[256];
    ent = obj;
    *(void**)&self->LensFxMan = cLensFxMan_construct();
    cRenderStateMan_init();
    cBXString__cBXString(&dir, 2);
    cBXString__cBXString(&name, 2);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBE_setState(int state);
extern "C" void cCameraTriggerMan_cleanupOnExit(void* self);
extern "C" void cCameraTriggerMan_purge(void* self);
extern "C" void cFEAsyncManager_FlushFEAsyncTextures(void* p);
extern "C" void cSSXScriptEngine_ForceEndScriptsAll(void* p);
extern "C" void cUIEngine__cUIEngine(void* p, int flags);
extern "C" void cInputSync__cInputSync(void* p, int flags);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
void cSurfPropCache_purge(void* p);
void operator_delete(int* p);
extern "C" void cPathSys_purgePaths(void* p);
extern "C" void cBXNash_purgeTheAIFwdDiffCache(void* p);
extern "C" void cMenuSys__cMenuSys(void* p, int flags);
extern "C" void cBucketMan_mergeBucket(void* mgr, int a);
extern "C" void cBucketMan_purge(void* mgr);
extern "C" void func_00229278(void* p, int flags);
extern "C" void func_00343B28(void* p, int flags);
extern "C" void cMultiParticleMan__cMultiParticleMan(void* p, int flags);
extern "C" void cPlaybackMan__cPlaybackMan(void* p, int flags);
extern "C" void cInstanceMan__cInstanceMan(void* p, int flags);
extern "C" void cRenderStateMan_purge(void);
extern "C" void cGameViewMan__cGameViewMan(void* p, int flags);
extern "C" void cReplay__cReplay(void* p, int flags);
extern "C" void cSectionMan__cSectionMan(void* p, int flags);
extern "C" void cFXLoader__cFXLoader(void* p, int flags);
extern "C" void cWorldLightMan__cWorldLightMan(void* p, int flags);
extern "C" void cWorld__cWorld(void* p, int flags);
extern "C" void cOcclusionMan__cOcclusionMan(void* p, int flags);
struct cBigFile;
void cBigFile__cBigFile(cBigFile* self, int flags);
extern "C" void cFont__cFont(void* p, int flags);
extern "C" void cGraphicsMan_PurgeTextures(void* p);
extern "C" void cIrradianceDataBase_Purge(void* p);
extern "C" void cWorldSound__cWorldSound(void* p, int flags);
extern "C" void cWorldTriggerManager_Reset(void* a);
extern "C" void cSSXAudio_InGameDeInit(void* p);
extern "C" void cSSXAudio_InGamePurge(void* p);
extern "C" void cSSXScriptEngine_Purge(void* p);
extern "C" void cGarbageMan_Purge(void);

extern void* gWorldTriggerManager;
extern int D_004A2A54;
extern char* cAppMan_TheAppMan;
extern char gPaths[];
extern char D_005047F8[];
// The unit declares gBucketMan later as sGp2898 (8 bytes); bind a small-data alias here.
extern char D_004A5988_gl __asm__("gBucketMan");

// g++ 2.95 vtable entry: this-adjust, index, function.
struct sVEnt_FA98 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVEnt0_FA98 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVEnt1_FA98 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

// Deletes an object through its virtual destructor (vtable pointer at vptr, slot entry at ent).
static inline void VDelete_FA98(char* p, int vptr, int ent)
{
    if (p != 0) {
        sVEnt_FA98* e = (sVEnt_FA98*)(*(char**)(p + vptr) + ent);
        e->fn(p + e->delta, 3);
    }
}

// 0xAC-byte record copied out of each rider before the game is torn down.
struct sRec_FA98 {
    int w[43];
};

// Results block saved across the game teardown (the unit declares it later as int[]).
struct sGameRes_FA98 {
    int active;
    int ready;
    int which;
    char pad[0x330 - 0xC];
    sRec_FA98 rec[2];
};
extern sRec_FA98 D_00534F0C_FA98 __asm__("D_00534F0C");

// Pointer array member with an inline indexer.
struct sPtrArr_FA98 {
    char* e[1];
    char* at(int i) { return e[i]; }
};

static inline char* GetRider_FA98(int i)
{
    return ((sPtrArr_FA98*)((char*)SSXApp->Game->AI + 0x28))->at(i);
}

struct sElem14_FA98 {
    int v[5];
    ~sElem14_FA98() {}
};

struct sObj20_FA98 {
    sElem14_FA98 e[8];
};

/**
 * Tears down a running event: saves the ranked-game records, deletes every game manager and resets the graphics,
 * audio and scripts.
 *
 * Sets the BE state to 0. For a ranked network game it copies each rider's 0xAC-byte record (rider +0x790 -> +0xFC;
 * host order by bIsHost) into the network options (+0x330) and D_00534F0C and calls the network interface's virtual
 * slot 1. Then: camera-trigger cleanup, GrMan slot 19, the FE async manager (cFEAsyncManager_FlushFEAsyncTextures), every script
 * force-ended; deletes UI and InputSync, returns the cut-scene transitions to regular mode, purges and frees
 * SurfPropCache, deletes SkyBox, FogMan and LensFxMan, purges the paths (cPathSys_purgePaths) and BXNash's AI cache,
 * deletes MenuSys, merges bucket 1 of the bucket manager D_004A5988, then deletes WScriptMan, CrowdMan2d/3d,
 * MultiParticleMan, PlaybackMan and InstanceMan, purges the camera triggers and render states, and deletes ViewMan,
 * AI, Replay, StreamPause, SectionMan, FXLoader, WorldLightMan, World, OcclusionMan, the track BigFile,
 * LoadIndicator (freed only), DebugFont and BXNash. Finally GrMan slots 113 and 50 (the box texture mBoxTextureID),
 * cGraphicsMan_PurgeTextures and cIrradianceDataBase_Purge, SSXApp->Game = 0, WorldSound deleted, the world triggers reset, the in-game
 * audio de-initialised and purged, the script engine purged, and D_004A2A54 and the word at cAppMan_TheAppMan + 0x34
 * cleared.
 */
extern "C" void cGame_purge(char* self)
{
    cBE_setState(0);
    char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cBENetworkInterface_networkOpt.bIsRankedGame != 0) {
        int which = cBENetworkInterface_networkOpt.bIsHost;
        int other = which != 0;
        *(sRec_FA98*)((char*)&cBENetworkInterface_networkOpt + 0x330) = *(sRec_FA98*)(*(char**)(GetRider_FA98(which == 0) + 0x790) + 0xFC);
        D_00534F0C_FA98 = *(sRec_FA98*)(*(char**)(GetRider_FA98(other) + 0x790) + 0xFC);
        sVEnt0_FA98* vtEntry = (sVEnt0_FA98*)(*(char**)(iface + 0xC) + 8);
        vtEntry->fn(iface + vtEntry->delta);
    }
    cCameraTriggerMan_cleanupOnExit(gCamTriggerMan);
    {
        cPS2GraphicsMan* be = GrMan;
        sVEnt0_FA98* vtEntry = (sVEnt0_FA98*)((char*)be->mVtbl + 0x98);
        vtEntry->fn((char*)be + vtEntry->delta);
    }
    cFEAsyncManager_FlushFEAsyncTextures(SSXApp->mFEAsyncManager);
    cSSXScriptEngine_ForceEndScriptsAll(ScriptEngine);
    if (*(void**)(self + 0x48) != 0) cUIEngine__cUIEngine(*(void**)(self + 0x48), 3);
    if (*(void**)(self + 0x7C) != 0) cInputSync__cInputSync(*(void**)(self + 0x7C), 3);
    cCutSceneTransitionMan_ForceToRegularMode(*(void**)(self + 0x64));
    cSurfPropCache_purge(*(void**)(self + 0x44));
    operator_delete(*(int**)(self + 0x44));
    VDelete_FA98(*(char**)(self + 0x14), 0xC, 8);
    VDelete_FA98(*(char**)(self + 0x18), 0x4, 8);
    VDelete_FA98(*(char**)(self + 0x1C), 25756, 8);
    cPathSys_purgePaths(gPaths);
    cBXNash_purgeTheAIFwdDiffCache(*(void**)(self + 0x20));
    if (*(void**)(self + 0xA4) != 0) cMenuSys__cMenuSys(*(void**)(self + 0xA4), 3);
    void* bm = &D_004A5988_gl;
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purge(bm);
    VDelete_FA98(*(char**)(self + 0x30), 1172, 8);
    if (*(void**)(self + 0x38) != 0) func_00229278(*(void**)(self + 0x38), 3);
    if (*(void**)(self + 0x3C) != 0) func_00343B28(*(void**)(self + 0x3C), 3);
    if (*(void**)(self + 0x40) != 0) cMultiParticleMan__cMultiParticleMan(*(void**)(self + 0x40), 3);
    if (*(void**)(self + 0x34) != 0) cPlaybackMan__cPlaybackMan(*(void**)(self + 0x34), 3);
    if (*(void**)(self + 0x88) != 0) cInstanceMan__cInstanceMan(*(void**)(self + 0x88), 3);
    cCameraTriggerMan_purge(gCamTriggerMan);
    cRenderStateMan_purge();
    if (*(void**)(self + 0x84) != 0) cGameViewMan__cGameViewMan(*(void**)(self + 0x84), 3);
    VDelete_FA98(*(char**)(self + 0xC), 204, 8);
    if (*(void**)(self + 0x28) != 0) cReplay__cReplay(*(void**)(self + 0x28), 3);
    VDelete_FA98(*(char**)(self + 0x98), 0x0, 8);
    if (*(void**)(self + 0x78) != 0) cSectionMan__cSectionMan(*(void**)(self + 0x78), 3);
    if (*(void**)(self + 0x2C) != 0) cFXLoader__cFXLoader(*(void**)(self + 0x2C), 3);
    if (*(void**)(self + 0x90) != 0) cWorldLightMan__cWorldLightMan(*(void**)(self + 0x90), 3);
    if (*(void**)(self + 0x10) != 0) cWorld__cWorld(*(void**)(self + 0x10), 3);
    if (*(void**)(self + 0x8C) != 0) cOcclusionMan__cOcclusionMan(*(void**)(self + 0x8C), 3);
    if (*(cBigFile**)(self + 0xA8) != 0) cBigFile__cBigFile(*(cBigFile**)(self + 0xA8), 3);
    operator_delete(*(int**)(self + 0x94));
    if (*(void**)(self + 0x24) != 0) cFont__cFont(*(void**)(self + 0x24), 3);
    {
        sObj20_FA98* nash = *(sObj20_FA98**)(self + 0x20);
        if (nash != 0) {
            nash->~sObj20_FA98();
            operator_delete((int*)nash);
        }
    }
    {
        cPS2GraphicsMan* be = GrMan;
        sVEnt0_FA98* vtEntry = (sVEnt0_FA98*)((char*)be->mVtbl + 0x388);
        vtEntry->fn((char*)be + vtEntry->delta);
    }
    {
        cPS2GraphicsMan* be = GrMan;
        sVEnt1_FA98* vtEntry = (sVEnt1_FA98*)((char*)be->mVtbl + 0x190);
        vtEntry->fn((char*)be + vtEntry->delta, *(void**)(self + 0x220));
    }
    cGraphicsMan_PurgeTextures(GrMan);
    cIrradianceDataBase_Purge(D_005047F8);
    *(void**)&SSXApp->Game = 0;
    if (*(void**)(self + 0x4C) != 0) cWorldSound__cWorldSound(*(void**)(self + 0x4C), 3);
    cWorldTriggerManager_Reset(gWorldTriggerManager);
    cSSXAudio_InGameDeInit(gSSXAudio);
    cSSXAudio_InGamePurge(gSSXAudio);
    cSSXScriptEngine_Purge(ScriptEngine);
    cGarbageMan_Purge();
    D_004A2A54 = 0;
    *(int*)(cAppMan_TheAppMan + 0x34) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_gainFocus);
#ifdef SKIP_ASM
class cMgr230050 {
public:
    int pad;
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
};
extern "C" void cFELocale_unloadFE(void* a);
extern "C" void cBXLoadIndicator_Init(void* a);
void cSSXScriptEngine_Init(void* a);
extern "C" void cGraphicsMan_UploadTextures(void* a);
extern "C" void cWorld_init(void* a);
extern "C" void cWorldTriggerManager_Reset(void* a);
extern "C" void cSSXAudio_InGameInit(void* a);
extern void* gWorldTriggerManager;
extern void* cOVStateManager_mpPDATemplate;
extern cMgr230050* cBezierMan_TheBezierMan;
extern int cLightGlow_m_Enable;
extern int cDepthFog_m_Enable;
extern int cScreenTint_m_Enable;

/**
 * Makes the game the active module again: refreshes the locale, the load indicator, the script engine and the
 * world.
 *
 * It also clears the PDA overlay template, calls helpers on the graphics manager, the world trigger manager
 * and the audio system, and turns the screen effects back on (cDepthFog_m_Enable, the screen tint, cLightGlow_m_Enable).
 */
extern "C" void cGame_gainFocus(cGame* self)
{
    cFELocale_unloadFE(SSXApp->FELocale);
    void* loadIndicator = self->LoadIndicator;
    cOVStateManager_mpPDATemplate = 0;
    cBXLoadIndicator_Init(loadIndicator);
    cSSXScriptEngine_Init(ScriptEngine);
    cBezierMan_TheBezierMan->v17();
    cBezierMan_TheBezierMan->v04();
    cGraphicsMan_UploadTextures(GrMan);
    cWorld_init(self->World);
    cWorldTriggerManager_Reset(gWorldTriggerManager);
    cSSXAudio_InGameInit(gSSXAudio);
    cDepthFog_m_Enable = 1;
    cScreenTint_m_Enable = 1;
    cLightGlow_m_Enable = 1;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_resetWorld);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPlaybackMan_restart(void* a);
extern "C" void cWScriptMan_initMissions(void* a);
extern "C" void cBucketMan_mergeBucket(void* mgr, int a);
extern "C" void cBucketMan_purgeBucket(void* self, int index);
extern "C" void cFocusMan_reset(void* a);
extern "C" void cAvalanche_reset(void);
extern "C" void cWorld_resetMap(void* world);
extern "C" void cWScriptMan_runGlobalScripts(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern void* cWScriptMan_TheWScriptMan;
// The unit declares gBucketMan later as sGp2898 (8 bytes); bind a small-data alias here.
extern char D_004A5988_bm __asm__("gBucketMan");

/**
 * Resets the world for a new run: playback, missions, update buckets 1 and 8, the AI focus, avalanches and
 * the map.
 *
 * It then reruns the global world scripts and sets the missions up again.
 */
extern "C" void cGame_resetWorld(cGame* self)
{
    void* bm = &D_004A5988_bm;
    cPlaybackMan_restart(self->PlaybackMan);
    cWScriptMan_initMissions(self->WScriptMan);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    cFocusMan_reset(*(void**)((char*)self->AI + 0xA4));
    cAvalanche_reset();
    cWorld_resetMap(self->World);
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(self->WScriptMan);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_partialRestart);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_snapAll(void* self);
extern "C" void cReplay_reset(void* self);
extern "C" void cSSXScriptEngine_Restart(void* self);
extern "C" void cCamera_onRestart(void);
extern "C" void func_00229498(void* self);
extern "C" void func_00343BC0(void* self);
extern "C" void cMultiParticleMan_reset(void* self);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" void cSSXAudio_PartialRestart(void* self);
extern "C" void* cAudioInterface_bxAudio(void);
int GetHashValue32(char* s);
extern char str_hud_c[];

struct sVEntry_230180a {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEntry_230180b {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

/**
 * Restarts the race without reloading: resets playback, missions, buckets, riders (AI slot 5 with 3) and
 * avalanches.
 *
 * It also snaps the views and resets the map, the replay, the script engine, the crowds and the particles. It
 * tells the UI state found by str_hud_c's hash about it (slot 24 with 7), and reruns the global world
 * scripts and the mission setup. Last, it calls cSSXAudio_PartialRestart on the audio system (On Tour PartialRestart).
 */
extern "C" void cGame_partialRestart(cGame* self)
{
    void* bm = &D_004A5988_bm;
    cPlaybackMan_restart(self->PlaybackMan);
    cWScriptMan_initMissions(self->WScriptMan);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    char* gameAI = (char*)self->AI;
    sVEntry_230180a* vt = *(sVEntry_230180a**)(gameAI + 0xCC);
    vt[5].fn(gameAI + vt[5].delta, 3);
    cAvalanche_reset();
    cGameViewMan_snapAll(self->ViewMan);
    cWorld_resetMap(self->World);
    cReplay_reset(self->Replay);
    cSSXScriptEngine_Restart(ScriptEngine);
    cCamera_onRestart();
    func_00229498(self->CrowdMan2d);
    func_00343BC0(self->CrowdMan3d);
    cMultiParticleMan_reset(self->MultiParticleMan);
    void* list = (char*)self->UI + 0x18;
    char* hudState = (char*)cUIStateStack_findState(list, GetHashValue32(str_hud_c));
    if (hudState != 0) {
        sVEntry_230180b* vt2 = *(sVEntry_230180b**)(hudState + 8);
        vt2[24].fn(hudState + vt2[24].delta, 7, 0);
    }
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(self->WScriptMan);
    cSSXAudio_PartialRestart(cAudioInterface_bxAudio());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/gameload", cGame_restart);
#ifdef SKIP_ASM
extern "C" void cGame_partialRestart(cGame* self);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" void cGame_setState(cGame* self, int a, int b, int refresh);
extern "C" void* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_RestartEvent(void* mgr);

static inline int numRiders_2302A8(void* game)
{
    return *(int*)(*(char**)((char*)game + 0xC) + 0x78);
}

static inline char* rider_2302A8(void* game, int i)
{
    return *(char**)(*(char**)((char*)game + 0xC) + (i << 2) + 0x28);
}

/**
 * Restarts the current event from the start.
 *
 * Runs cGame_partialRestart() and cGameModeMan_restartHeat(), clears every allocated rider's unk_2F8 (which the
 * game-mode code sets to 0.7, 0.35 or 0 by the player's place), switches the game to state 1 at once
 * (cGame_setState(self, 1, 2, 1)), and calls cSSXAudio_RestartEvent() on the audio system.
 */
extern "C" void cGame_restart(cGame* self)
{
    cGame_partialRestart(self);
    cGameModeMan_restartHeat(SSXApp->GameModeMan);
    int i;
    for (i = 0; i < numRiders_2302A8(self); i++) {
        *(int*)(rider_2302A8(self, i) + 0x2F8) = 0;
    }
    cGame_setState(self, 1, 2, 1);
    cSSXAudio_RestartEvent(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_sectionLoaded);
#ifdef SKIP_ASM
/**
 * Called when a world section finishes loading: invalidates a cached value of the AI's focus manager.
 *
 * When the AI (+0xC) and its focus manager (+0xA4, On Tour FocusMan) exist, sets the manager's +0xD0 to -1. The
 * loaded section's id, passed by cSectionMan_update, is not used.
 */
extern "C" void cGame_sectionLoaded(cGame* self)
{
    void* gameAI = self->AI;
    if (gameAI != 0) {
        void* focusMan = *(void**)((char*)gameAI + 0xA4);
        if (focusMan != 0) {
            *(int*)((char*)focusMan + 0xD0) = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_sectionUnloading);
#ifdef SKIP_ASM
extern "C" void func_00358700(int kind);
extern "C" void func_00229408(void* self, int id);
extern "C" void func_00343C08(void* self, int arg);
extern "C" void cMultiParticleMan_unloadSection(void* self, int arg);
extern "C" void cBucketMan_purgeBucket1(void* self, int index, int key);
extern "C" void cFocusMan_purgesection(void* p, int arg);
extern "C" void cWScriptMan_unloadSection(void* self, int i);
extern void* D_004A2A00;
extern void* D_004A3FF0;
extern void* cMultiParticleMan_TheMultiParticleMan;

/**
 * Removes everything that belongs to a world section that is about to unload.
 *
 * The section's crowds, particles and update-bucket entries (buckets 1 and 8) go first. It then clears the AI
 * focus manager for the section and for 0x80 (cFocusMan_purgesection), and unloads the section's world scripts.
 *
 * @param id  the section id (On Tour sectionId)
 */
extern "C" void cGame_sectionUnloading(cGame* self, int id)
{
    void* bm = &D_004A5988_bm;
    func_00358700(id);
    func_00229408(D_004A2A00, id);
    func_00343C08(D_004A3FF0, id);
    cMultiParticleMan_unloadSection(cMultiParticleMan_TheMultiParticleMan, id);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket1(bm, 1, id);
    cBucketMan_purgeBucket1(bm, 8, id);
    void* gameAI = self->AI;
    if (gameAI != 0) {
        void* focusMan = *(void**)((char*)gameAI + 0xA4);
        if (focusMan != 0) {
            cFocusMan_purgesection(focusMan, id);
            cFocusMan_purgesection(*(void**)((char*)self->AI + 0xA4), 0x80);
        }
    }
    cWScriptMan_unloadSection(self->WScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_loadWScriptManSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: cWScriptMan_loadSection is declared with one argument; it forwards the section id in $a1 to cWScriptMan_allocateGlobalTable
void* cWScriptMan_loadSection_2(void* self, int sectionId) __asm__("cWScriptMan_loadSection__FPv");

struct sScriptSection_0430 {
    short state;
    short pad;
    int f4;
    void* data;
};

struct sScriptCache_0430 {
    int f0;
    sScriptSection_0430* sections;
};

struct sWorldCache_0430 {
    int f0;
    sScriptCache_0430* scripts;
};


static inline bool isSectionLoaded_0430(sScriptCache_0430* c, int sectionId)
{
    return c->sections[sectionId].state >= 2 && c->sections[sectionId].data != 0;
}

/**
 * Runs a world section's scripts once they are loaded: calls cWScriptMan_loadSection on cWScriptMan_TheWScriptMan with the
 * section id.
 *
 * Does nothing while the world's script cache (cWorld_m_pTheWorld->m_pWScriptCache) holds the section in a state below
 * 2 or without data. cPS2ChunkResolve_resolve calls it when a section's chunk resolves.
 *
 * @param sectionId  the world section
 */
extern "C" void cGame_loadWScriptManSection(cGame* self, int sectionId)
{
    if (isSectionLoaded_0430((sScriptCache_0430*)cWorld_m_pTheWorld->m_pWScriptCache, sectionId)) {
        cWScriptMan_loadSection_2(cWScriptMan_TheWScriptMan, sectionId);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_exit);
#ifdef SKIP_ASM
struct cAppMan;
void cReplay_stopAutoReplay(void* self);
extern "C" void cGameComm_gameExit(void* p);
extern "C" void func_00266DF8(void* p);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void* cPreFELoadScreen_cPreFELoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern void* D_004A33F4;
extern char str_PreGameLoadScreen[];
extern char str_PreFELoad[];

/**
 * Leaves the in-game module and queues the next app module.
 *
 * Stops the auto replay, sets game state 6, shuts down the online game-comm object (cGameComm_gameExit) and the
 * DirtySock object in D_004A33F4 when they exist. Despite the parameter name, a nonzero @p toFrontEnd queues a
 * cPreGameLoadScreen (used after choosing a new event in-game), and 0 queues the "PreFELoad" module, which
 * probably loads the front end.
 *
 * @param toFrontEnd  nonzero for the pre-game load screen, 0 for the "PreFELoad" module
 */
extern "C" void cGame_exit(cGame* self, int toFrontEnd)
{
    cReplay_stopAutoReplay(self->Replay);
    cGame_setState(self, 6, 0, 0);
    if (cGameComm_mGameComm != 0) {
        cGameComm_gameExit(cGameComm_mGameComm);
    }
    if (D_004A33F4 != 0) {
        func_00266DF8(D_004A33F4);
    }
    if (toFrontEnd != 0) {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_PreGameLoadScreen, 0x100, 0)));
    } else {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreFELoadScreen_cPreFELoadScreen(cMemMan_alloc(8, str_PreFELoad, 0x100, 0)));
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230558);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cReplay_startReplay(void* a, int b, int c, int d);
void cMenuSys_activate(void* a, int b);
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_00442910[];

struct sGameLoad_0558 {
    char pad_0x00[0x28];
    void* field_0x28;       // 0x28
    char pad_0x2C[0xA4 - 0x2C];
    void* field_0xA4;       // 0xA4
};

/**
 * Starts replay playback, the mirror of cGame_stopReplay.
 *
 * Pushes pause state 8 (D_005366E8[++D_004A2A54] = 8) and sets cPauseState_Flags to that state's flags (D_00442910,
 * entry 8 of the table D_004428F0), starts the replay (+0x28) with cReplay_startReplay(replay, 0, 0, 0) and calls
 * cMenuSys_activate (On Tour cMenuSys::activate) with 0 on the menu system (+0xA4). The pause stack index is not checked.
 */
extern "C" void func_00230558(sGameLoad_0558* self)
{
    D_005366E8[++D_004A2A54] = 8;
    cPauseState_Flags = D_00442910[0];
    cReplay_startReplay(self->field_0x28, 0, 0, 0);
    cMenuSys_activate(self->field_0xA4, 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_stopReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_stopReplay(void* a);
void cMenuSys_activate(void* a, int b);
extern "C" void cMenuSys_popMenu(void* a);
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_004428F0[];

/**
 * Ends replay playback and returns to the pause menu state that was active before it.
 *
 * Stops the replay (+0x28), calls cMenuSys_activate (On Tour cMenuSys::activate) with 1 and cMenuSys_popMenu
 * (cMenuSys::popMenu) on the menu system (+0xA4), then pops the pause stack (D_004A2A54 index into D_005366E8)
 * and restores cPauseState_Flags from D_004428F0. The stack index is not checked (On Tour asserts it is > 0).
 */
extern "C" void cGame_stopReplay(cGame* self)
{
    cReplay_stopReplay(self->Replay);
    cMenuSys_activate(self->MenuSys, 1);
    cMenuSys_popMenu(self->MenuSys);
    cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230640);
#ifdef SKIP_ASM
extern "C" void cBucketMan_renderBucket(void* mgr, int id);
struct sGp2898 { int a, b; };
extern sGp2898 gBucketMan; // target: raw $gp+0x2898 (splat doesn't symbolize it)
extern int D_004428D8[];

/**
 * Renders the six game buckets listed in D_004428D8, in order, with cBucketMan_renderBucket on gBucketMan.
 *
 * cGame_render calls it in each of its three render paths.
 */
extern "C" void func_00230640(void)
{
    int i;
    sGp2898* mgr = &gBucketMan;
    for (i = 0; i < 6; i++) {
        cBucketMan_renderBucket(mgr, D_004428D8[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_getTransitionMan__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the game's transition manager for one view: TransitionMan[@p i] (+0x5C).
 *
 * @param i  the view (player) index, 0 or 1
 * @return the cTransitionMan
 */
int cGame_getTransitionMan(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x5C);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_getSplashManager__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the game's splash manager for one view: SplashManager[@p i] (+0x68).
 *
 * @param i  the view (player) index, 0 or 1
 * @return the cSplashManager
 */
int cGame_getSplashManager(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x68);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_update);
#ifdef SKIP_ASM
extern "C" float cRenderStateMan_SkyBoxRotation(int id);
extern "C" void cGameComm_update(void* p);
extern "C" void cSSXScriptEngine_UpdateLoad(void* p);
extern "C" int cGameViewMan_isStreamReady(void* p);
extern "C" void cGameViewMan_updateAll(void* p);
extern "C" void cGame_updateState(cGame* self);
extern "C" void cGameViewMan_updateStreamFocus(void* p);
extern "C" void cWorldCache_flushInActivePages(void* p);
extern "C" void cInputSync_preupdate(void* p);
extern "C" void cOVStateManager_update(void);
extern "C" int cMenuSys_isActive(void* p);
extern "C" void cMenuSys_input(void* p);
extern "C" int* tMissionMenuQueue_getEntry(void* p);
extern "C" void cOVStateManager_triggerOverlay(int id);
extern "C" void cSSXAudio_PlayIconSound(void* p, int a, int b, float c);
extern "C" void cWScriptMan_doMissionCmd(void* p, int a, int b, int c);
extern "C" void cUIEngine_update(void* p);
extern "C" void tMissionMenuQueue_getNextEntry(void* p);
extern "C" void func_0029ED18(void* p);
// PORT: the unit defines cGame_activatePauseMenu(char* self); this caller passes a second argument (a1 = 0).
extern "C" int cGame_activatePauseMenu_pi(void* self, int a) __asm__("cGame_activatePauseMenu");
// cFadeScreen_update returns nothing (its body has no return value); the unit declares it void*.
extern "C" void cFadeScreen_update_v(void* self) __asm__("cFadeScreen_update");
extern "C" int cOVStateManager_allowPauseMenu(void);
extern "C" void cPadCache_clearAllVibrations(void* p);
extern "C" void func_0020CA10(void);
extern "C" void cSSXAudio_PauseAudio(void* p);
extern "C" void cSSXAudio_PlayPauseSound(void* p);
extern "C" int cOVStateManager_pauseOverlayActive(void);
extern "C" void cPlaybackMan_update(void* p);
extern "C" int cInputSync_aiinputready(void* p);
extern "C" int cInputSync_aiupdate(void* p);
extern "C" int cPlaybackMan_isPauseAI(void* p);
extern "C" void cReplay_update(void* p);
extern "C" int cReplay_isPauseAI(void* p);
extern "C" void cBucketMan_updateBucket(void* mgr, int a);
extern "C" void cBucketMan_applyBucket(void* mgr, int a);
extern "C" void cWScriptMan_update(void* p);
extern "C" void func_002294C8(void* p);
extern "C" void func_003440C8(void* p);
extern "C" void cMultiParticleMan_update(void* p);
extern "C" void cWorldSound_TriggerWorldAudio(void* p, void* q, int i);
void cInputSync_postupdate(void* p);
extern "C" void NetConnIdle(void);
extern "C" void cFEAsyncManager_Update(void* p);
extern "C" void func_001E1458(void);
extern "C" void cBXLoadIndicator_Update(void* p);

extern char* cAppMan_TheAppMan;
extern void* PadCache;
extern unsigned int D_004A20CC;
extern char str_90bc_fail_b[];

struct sVEntry_2306B8 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVEntryI_2306B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObjC_2306B8 {
    char pad0[0x8];
    sVEntryI_2306B8* vtbl;
};

struct sObj48_2306B8 {
    char pad0[0xC];
    sObjC_2306B8* fC;
};


static inline void pushState_2306B8(int state)
{
    D_005366E8[++D_004A2A54] = state;
    cPauseState_Flags = D_004428F0[state];
}

static inline int inRace_2306B8(int s)
{
    if (s != 0 && s < 10) {
        return 1;
    }
    return 0;
}

static inline int hasEvents_2306B8(char* list)
{
    return *(int*)(list + 0x0) != *(int*)(list + 0x4);
}

static inline int paused_2306B8(void)
{
    return cPauseState_Flags & 1;
}

static inline int notPlaying_2306B8(char* pb)
{
    return *(int*)(pb + 0x70) != 1;
}

static inline int notMode3_2306B8(int s)
{
    return s != 3;
}

// Object at game + 0x84: views at +0x4, count at +0x10.
struct sViewList_2306B8 {
    int f0;
    char* views[3];
    unsigned int count;
    unsigned int getCount() { return count; }
    char* getView(unsigned int i) { return views[i]; }
};

// Object at game + 0xC: riders at +0x28, count at +0x78.
struct sRiderList_2306B8 {
    char pad0[0x28];
    char* riders[20];
    int count;
    int getCount() { return count; }
    char* getRider(int i) { return riders[i]; }
};

static inline int canUpdate_2306B8(cGame* self)
{
    if (*(int*)((char*)SSXApp + 0x4) != 0) {
        self->unk_08 = 1;
        return 0;
    }
    return 1;
}

static inline int updatePause_2306B8(cGame* self)
{
    if (cGameViewMan_isStreamReady(((char*)self->ViewMan)) == 0 || *(int*)(((char*)self->SectionMan) + 0x1D0) == 0) {
        *(int*)(*((char**)((char*)self->World)) + 0x3D4) = 0;
        cWorld_update(((char*)self->World));
        cGameViewMan_updateAll(((char*)self->ViewMan));
        if (*(int*)(((char*)self->SectionMan) + 0x1D0) == 0) {
            cGame_updateState(self);
            cSectionMan_update(((char*)self->SectionMan));
        }
        if (self->IsStreamPaused == 0 && cGameViewMan_isStreamReady(((char*)self->ViewMan)) == 0) {
            self->m_bFlushStreamer = 1;
        }
        self->IsStreamPaused = 1;
        if (self->m_bFlushStreamer != 0) {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sVEntry_2306B8* vt = (sVEntry_2306B8*)ps2GraphicsMan->mVtbl;
            vt[19].fn((char*)ps2GraphicsMan + vt[19].delta);
            cGameViewMan_updateStreamFocus(((char*)self->ViewMan));
            cWorldCache_flushInActivePages(*((void**)((char*)self->World)));
            self->m_bFlushStreamer = 0;
        }
        return 0;
    }
    *(int*)(*((char**)((char*)self->World)) + 0x3D4) = 1;
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cWorld_update(((char*)self->World));
    cSectionMan_update(((char*)self->SectionMan));
    return 1;
}

static inline int updateState_2306B8(cGame* self)
{
    if (cMenuSys_isActive(((char*)self->MenuSys)) == 0) {
        char* q = ((char*)self->WScriptMan);
        char* list = q + 0x1C4;
        if (*(int*)(q + 0x1C4) != *(int*)(q + 0x1C8)) {
            int* e = tMissionMenuQueue_getEntry(list);
            if (e[1] == 8) {
                cOVStateManager_triggerOverlay(0x1D);
                pushState_2306B8(3);
                cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 4, 0, 1.0f);
                return 0;
            }
            if (e[1] == 9) {
                cWScriptMan_doMissionCmd(((char*)self->WScriptMan), e[0], 5, e[2]);
                if (cUIStateStack_findState(((char*)self->UI) + 0x18, GetHashValue32(str_90bc_fail_b)) == 0) {
                    cOVStateManager_triggerOverlay(0x1E);
                    pushState_2306B8(3);
                    cUIEngine_update(((char*)self->UI));
                } else {
                    tMissionMenuQueue_getNextEntry((char*)cWScriptMan_TheWScriptMan + 0x1C4);
                }
                return 0;
            }
        }
    }
    if (~D_004A20CC != 0) {
        cOVStateManager_triggerOverlay(0x22);
        pushState_2306B8(12);
        D_004A20CC = 0xFFFFFFFF;
        func_0029ED18(cAudioInterface_bxAudio());
        return 0;
    }
    if (cMenuSys_isActive(((char*)self->MenuSys)) != 0) {
        cUIEngine_update(((char*)self->UI));
        cMenuSys_input(((char*)self->MenuSys));
        return 0;
    }
    if (cGame_activatePauseMenu_pi(self, 0) != 0 && !inRace_2306B8(*((int*)((char*)self->Replay)))
        && notPlaying_2306B8(((char*)self->PlaybackMan)) && cOVStateManager_allowPauseMenu() != 0) {
        cPadCache_clearAllVibrations(PadCache);
        func_0020CA10();
        pushState_2306B8(2);
        {
            sObjC_2306B8* o = ((sObj48_2306B8*)((char*)self->UI))->fC;
            sVEntryI_2306B8* vt = o->vtbl;
            vt[44].fn((char*)o + vt[44].delta, (*(unsigned char*)&self->PauseOwner));
        }
        cSSXAudio_PauseAudio(cAudioInterface_bxAudio());
        cSSXAudio_PlayPauseSound(cAudioInterface_bxAudio());
        return 0;
    }
    {
        int wasPaused = paused_2306B8();
        cUIEngine_update(((char*)self->UI));
        if (cOVStateManager_pauseOverlayActive() == 0) {
            cPlaybackMan_update(((char*)self->PlaybackMan));
            if (cMenuSys_isActive(((char*)self->MenuSys)) != 0) {
                return 0;
            }
            if (wasPaused && !paused_2306B8()) {
                return 0;
            }
            return 1;
        }
        return 0;
    }
}

/**
 * Runs one game frame: streaming, the current game state, the update buckets, the AI, the views and the world
 * sound.
 *
 * While the views or the section manager are not ready, it only keeps streaming and the views going. Mission
 * overlays, the debug menu and the pause menu can take the frame first (they push a pause state). The pause
 * flags (cPauseState_Flags), the replay and the playback can skip the simulation buckets; then only buckets 5, 3 and
 * 4 run. Every frame also updates the network, the front-end async manager and the load indicator.
 */
extern "C" void cGame_update(cGame* self)
{
    if (*(unsigned int*)((char*)self->ViewMan + 0x10) >= 2) {
        *(int*)(cAppMan_TheAppMan + 0x34) = 1;
    } else {
        *(int*)(cAppMan_TheAppMan + 0x34) = cRenderStateMan_SkyBoxRotation(6) > 0.9f;
    }
    if (cGameComm_mGameComm != 0) {
        cGameComm_update(cGameComm_mGameComm);
    }
    cSSXScriptEngine_UpdateLoad(ScriptEngine);
    cFadeScreen_update_v((self));
    if (canUpdate_2306B8((self)) && updatePause_2306B8((self))) {
        if ((char*)self->InputSync != 0) {
            cInputSync_preupdate((char*)self->InputSync);
        }
        cOVStateManager_update();
        if (updateState_2306B8((self))) {
            int skip;
            if ((char*)self->InputSync == 0 || cInputSync_aiinputready((char*)self->InputSync) != 0) {
                if (self->mCurrentStateID == 3) {
                    cGame_updateState((self));
                    cBucketMan_mergeBucket(&gBucketMan, 1);
                } else {
                    cGame_updateState((self));
                }
                if (!paused_2306B8()) {
                    cReplay_update((char*)self->Replay);
                }
                if (cReplay_isPauseAI((char*)self->Replay) == 0 && !(cPauseState_Flags & 8)) {
                    char* w = (char*)ScriptEngine;
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(w + 0x2A8);
                    vt[5].fn(w + vt[5].delta);
                }
            }
            skip = 0;
            if ((cPauseState_Flags & 1) || cReplay_isPauseAI((char*)self->Replay) != 0) {
                skip = 1;
            }
            if (!skip) {
                if (((char*)self->InputSync != 0 && cInputSync_aiupdate((char*)self->InputSync) == 0)
                    || cPlaybackMan_isPauseAI((char*)self->PlaybackMan) != 0) {
                    skip = 1;
                }
            }
            if (!skip) {
                if (!(cPauseState_Flags & 2)) {
                    cBucketMan_updateBucket(&gBucketMan, 1);
                }
                cBucketMan_updateBucket(&gBucketMan, 5);
                cBucketMan_updateBucket(&gBucketMan, 6);
                if (!(cPauseState_Flags & 2)) {
                    cBucketMan_applyBucket(&gBucketMan, 1);
                }
                {
                    char* gameAI = (char*)self->AI;
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(gameAI + 0xCC);
                    vt[3].fn(gameAI + vt[3].delta);
                }
                if (!(cPauseState_Flags & 0x10)) {
                    cBucketMan_updateBucket(&gBucketMan, 2);
                }
                if (!(cPauseState_Flags & 4)) {
                    cBucketMan_updateBucket(&gBucketMan, 3);
                }
                cWScriptMan_update((char*)self->WScriptMan);
                func_002294C8((char*)self->CrowdMan2d);
                func_003440C8((char*)self->CrowdMan3d);
                cMultiParticleMan_update((char*)self->MultiParticleMan);
                cBucketMan_mergeBucket(&gBucketMan, 1);
            } else {
                cBucketMan_updateBucket(&gBucketMan, 5);
                if (!(cPauseState_Flags & 4)) {
                    cBucketMan_updateBucket(&gBucketMan, 3);
                }
            }
            cBucketMan_updateBucket(&gBucketMan, 4);
            {
                int replayState = *((int*)(char*)self->Replay);
                if (!inRace_2306B8(replayState) || notMode3_2306B8(replayState)) {
                    cGameViewMan_updateAll((char*)self->ViewMan);
                }
            }
            (self->mUpdateTick)++;
            {
                unsigned int i;
                for (i = 0; i < (((sViewList_2306B8*)(char*)self->ViewMan))->getCount(); i++) {
                    cWorldSound_TriggerWorldAudio((char*)self->WorldSound, (((sViewList_2306B8*)(char*)self->ViewMan))->getView(i) + 0x20, i);
                }
            }
            {
                int i;
                for (i = 0; i < (((sRiderList_2306B8*)(char*)self->AI))->getCount(); i++) {
                    char* rider = (((sRiderList_2306B8*)(char*)self->AI))->getRider(i);
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(rider + 0x6C0);
                    vt[19].fn(rider + vt[19].delta);
                }
            }
        }
        if ((char*)self->InputSync != 0) {
            cInputSync_postupdate((char*)self->InputSync);
        }
    }
    if (cCommSystem_mCommSystem != 0) {
        NetConnIdle();
    }
    if ((void*)SSXApp->mFEAsyncManager != 0) {
        cFEAsyncManager_Update(SSXApp->mFEAsyncManager);
    }
    func_001E1458();
    cBXLoadIndicator_Update((char*)self->LoadIndicator);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230E90);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty function that returns at once. Nothing in the build references it.
 */
extern "C" void func_00230E90(void) {

}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_loseFocus);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_loseFocus(void* self);
void cSSXScriptEngine_DeInit(void* self);
void cBXLoadIndicator_DeInit(void* self);

struct sVEntry00230E98 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVec3_00230E98 {
    float x, y, z;
    sVec3_00230E98(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
struct cMgr00230E98 {
    char pad0[0x270];
    int f270;
    char pad274[0x10D8 - 0x274];
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
    virtual void v15(const sVec3_00230E98& v);
};

/**
 * Lets the game give up focus, once the stream pause (its virtual slot 3) and the screen fade allow it.
 *
 * On success it calls the graphics manager's v15 with (0, 0, 0), sets its +0x270 flag, turns the screen
 * effects off, and calls helpers on the script engine and the load indicator.
 *
 * @return 1 when focus was given up, 0 while the stream pause or the fade is still busy
 */
extern "C" int cGame_loseFocus(cGame* self)
{
    void* obj = self->StreamPause;
    sVEntry00230E98* vt = *(sVEntry00230E98**)obj;
    if (vt[3].fn((char*)obj + vt[3].delta) == 0) {
        return 0;
    }
    if (cFadeScreen_loseFocus(self) == 0) {
        return 0;
    }
    cPS2GraphicsMan* mgr = GrMan;
    ((cMgr00230E98*)mgr)->v15(sVec3_00230E98(0.0f, 0.0f, 0.0f));
    ((cMgr00230E98*)GrMan)->f270 = 1;
    cDepthFog_m_Enable = 0;
    cScreenTint_m_Enable = 0;
    cLightGlow_m_Enable = 0;
    cSSXScriptEngine_DeInit(ScriptEngine);
    cBXLoadIndicator_DeInit(self->LoadIndicator);
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_updateState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGameViewMan_updateViewState(void* self);
extern int cPauseState_Flags;

class cGameState_0F40 {
public:
    int pad[3];
    virtual void v01();
    virtual void preloadInit();
    virtual void preloadUpdate();
    virtual void preloadCancel();
    virtual void gainFocus(int arg);
    virtual void update();
    virtual void loseFocus();
};


/**
 * Runs the game-flow state machine: updates the current state, drives a preload and switches to a requested state.
 *
 * Returns at once in an offline game (cBENetworkInterface_networkOpt.bIsNetworkGame 0) while ControllerUnplugged
 * (+0x224) is set, or while cPauseState_Flags has bit 0x20. Calls the current state's update() (virtual slot 6) unless
 * it is the gate state (3) with a switch pending. A pending preload id (mPendingPreloadStateID, +0x208) picks the
 * member state for that id (ids 1-16: +0xB0 ... +0x1EC) as mPreloadingState (+0x204), moves the id to mPreloadStateID
 * (+0x20C) and calls its preloadInit() (slot 2); a preloading state gets preloadUpdate() (slot 3) on every call.
 *
 * When mPendingStateID (+0x210) is set, the current state gets loseFocus() (slot 7) and becomes the previous one
 * (mPrevState, mPrevStateID), the member state for the id becomes mCurrentState / mCurrentStateID, the pending id and
 * mPendingStateStartupFlag (+0x21C) are cleared, a preload of that same state is forgotten, the split-screen layout is
 * refreshed (cGameViewMan_updateViewState on ViewMan, +0x84) and the new state gets gainFocus(startup flag) (slot 5). The per-function
 * view names these slots enter/update/start/tick/exit. It also calls cBE_getInterface(SSXApp->BE, 7) and ignores the
 * result.
 *
 * @param self_  the game (cGame, this)
 */
// Game load state machine: runs the current state and switches to the requested one.
extern "C" void cGame_updateState(cGame* self_)
{
    cGame* self = self_;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0 && self->ControllerUnplugged != 0)
        return;
    if (cPauseState_Flags & 0x20)
        return;
    if (((cGameState_0F40*)self->mCurrentState) != 0 && (self->mCurrentStateID != 3 || self->mPendingStateID == 0))
        ((cGameState_0F40*)self->mCurrentState)->update();
    if (self->mPendingPreloadStateID != 0) {
        switch (self->mPendingPreloadStateID) {
    case 1:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0xB0);
        break;
    case 2:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0xD0);
        break;
    case 3:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0xE0);
        break;
    case 4:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0xF0);
        break;
    case 5:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x100);
        break;
    case 6:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x118);
        break;
    case 7:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x128);
        break;
    case 8:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x138);
        break;
    case 9:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x148);
        break;
    case 10:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x158);
        break;
    case 11:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x178);
        break;
    case 12:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x18C);
        break;
    case 13:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x19C);
        break;
    case 14:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x1B8);
        break;
    case 15:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x1DC);
        break;
    case 16:
        (*(cGameState_0F40**)&self->mPreloadingState) = (cGameState_0F40*)((char*)self + 0x1EC);
        break;
        }
        self->mPreloadStateID = self->mPendingPreloadStateID;
        self->mPendingPreloadStateID = 0;
        ((cGameState_0F40*)self->mPreloadingState)->preloadInit();
    }
    if (((cGameState_0F40*)self->mPreloadingState) != 0)
        ((cGameState_0F40*)self->mPreloadingState)->preloadUpdate();
    int next = self->mPendingStateID;
    if (next == 0)
        return;
    int startupFlag = self->mPendingStateStartupFlag;
    if (((cGameState_0F40*)self->mCurrentState) != 0)
        ((cGameState_0F40*)self->mCurrentState)->loseFocus();
    self->mPrevStateID = self->mCurrentStateID;
    (*(cGameState_0F40**)&self->mPrevState) = ((cGameState_0F40*)self->mCurrentState);
    switch (next) {
    case 1:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0xB0);
        self->mCurrentStateID = 1;
        break;
    case 2:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0xD0);
        self->mCurrentStateID = 2;
        break;
    case 3:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0xE0);
        self->mCurrentStateID = 3;
        break;
    case 4:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0xF0);
        self->mCurrentStateID = 4;
        break;
    case 5:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x100);
        self->mCurrentStateID = 5;
        break;
    case 6:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x118);
        self->mCurrentStateID = 6;
        break;
    case 7:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x128);
        self->mCurrentStateID = 7;
        break;
    case 8:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x138);
        self->mCurrentStateID = 8;
        break;
    case 9:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x148);
        self->mCurrentStateID = 9;
        break;
    case 10:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x158);
        self->mCurrentStateID = 10;
        break;
    case 11:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x178);
        self->mCurrentStateID = 11;
        break;
    case 12:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x18C);
        self->mCurrentStateID = 12;
        break;
    case 13:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x19C);
        self->mCurrentStateID = 13;
        break;
    case 14:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x1B8);
        self->mCurrentStateID = 14;
        break;
    case 15:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x1DC);
        self->mCurrentStateID = 15;
        break;
    case 16:
        (*(cGameState_0F40**)&self->mCurrentState) = (cGameState_0F40*)((char*)self + 0x1EC);
        self->mCurrentStateID = 16;
        break;
    }
    self->mPendingStateID = 0;
    self->mPendingStateStartupFlag = 0;
    if (self->mPreloadStateID == self->mCurrentStateID) {
        self->mPreloadStateID = 0;
        (*(cGameState_0F40**)&self->mPreloadingState) = 0;
    }
    if (((cGameState_0F40*)self->mCurrentState) != 0) {
        cGameViewMan_updateViewState(((void*)self->ViewMan));
        ((cGameState_0F40*)self->mCurrentState)->gainFocus(startupFlag);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_setState);
#ifdef SKIP_ASM
extern "C" void cGame_updateState(cGame* self);

/**
 * Requests a change of game-flow state, optionally switching at once.
 *
 * Stores @p iNextState at +0x210 and @p startupFlag at +0x21C, and runs cGame_updateState straight away when @p refresh is nonzero.
 *
 * @param iNextState        the next state id
 * @param startupFlag        the state's startup flag
 * @param refresh  nonzero to switch now (On Tour bForce)
 */
extern "C" void cGame_setState(cGame* self, int iNextState, int startupFlag, int refresh)
{
    self->mPendingStateID = iNextState;
    self->mPendingStateStartupFlag = startupFlag;
    if (refresh != 0) {
        cGame_updateState(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_setPreloadState__FPvi);
#ifdef SKIP_ASM
/**
 * Asks the game to preload a state next: stores @p val in mPendingPreloadStateID (+0x208).
 *
 * cGame_load and cSectionMan_preLoadMode call it.
 *
 * @param val  the state id to preload
 */
void cGame_setPreloadState(void* self, int val)
{
    *(int*)((char*)self + 0x208) = val;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231280);
#ifdef SKIP_ASM
/**
 * Cancels the game's state preload.
 *
 * Clears mPendingPreloadStateID (+0x208); when a preload is running (mPreloadStateID, +0x20C), calls the preloading
 * state's preloadCancel (vtable slot +0x24 of mPreloadingState, +0x204) and clears mPreloadStateID and
 * mPreloadingState.
 */
extern "C" void func_00231280(void *self) {
    void *obj;
    void *vtbl;

    if (*(int *)((char*)self + 0x208) != 0) {
        *(int *)((char*)self + 0x208) = 0;
    }
    if (*(int *)((char*)self + 0x20C) != 0) {
        obj = *(void **)((char*)self + 0x204);
        vtbl = *(void **)((char*)obj + 0xC);
        (*(void (**)(void *))((char*)vtbl + 0x24))((char*)obj + *(short *)((char*)vtbl + 0x20));
        *(int *)((char*)self + 0x20C) = 0;
        *(void **)((char*)self + 0x204) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_isPreloadedStateReady);
#ifdef SKIP_ASM
struct sVEntry_002312D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Tells whether the state being preloaded is ready to run.
 *
 * Returns 0 while a preload request is still pending (mPendingPreloadStateID, +0x208), 1 when nothing is being
 * preloaded (mPreloadingState, +0x204), else that state's isPreloadComplete() (virtual slot 8).
 *
 * @return nonzero when ready
 */
extern "C" int cGame_isPreloadedStateReady(cGame* self)
{
    if (self->mPendingPreloadStateID != 0) {
        return 0;
    }
    void* preloadingState = self->mPreloadingState;
    if (preloadingState == 0) {
        return 1;
    }
    sVEntry_002312D8* vt = *(sVEntry_002312D8**)((char*)preloadingState + 0xC);
    return vt[8].fn((char*)preloadingState + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_doNextState);
#ifdef SKIP_ASM
extern "C" void cGameState_gotoNextState(void* p);

/**
 * Moves the game to its next state: calls cGameState_gotoNextState on the current state (mCurrentState, +0x200) when
 * there is one.
 */
extern "C" void cGame_doNextState(cGame* self)
{
    void* gameState = self->mCurrentState;
    if (gameState != 0) {
        cGameState_gotoNextState(gameState);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_writeToReplayFrame);
#ifdef SKIP_ASM
struct cSerObj_00231348 {
    int f0, f4, f8;
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
    virtual void serialize(void* stream);
};
struct cStream_00231348 {
    virtual void write(void* p, int n);
};

// PORT: these serializers take (self, stream); their mangled names say (void*).
void cGFEndState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFEndState_writeToReplayFrame__FPv");
void cGFEndWaitState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFEndWaitState_writeToReplayFrame__FPv");
void cGFGateState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFGateState_writeToReplayFrame__FPv");
void cGFRidingState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFRidingState_writeToReplayFrame__FPv");
void cGFSessionTeleportState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFSessionTeleportState_writeToReplayFrame__FPv");
void cGFSectionLoadState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFSectionLoadState_writeToReplayFrame__FPv");
void cGFTransportWaitState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFTransportWaitState_writeToReplayFrame__FPv");
void cGFPreEventState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFPreEventState_writeToReplayFrame__FPv");
void cGFPostEventState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFPostEventState_writeToReplayFrame__FPv");
void cGFPodiumState_writeToReplayFrame_s(void* p, void* s) __asm__("cGFPodiumState_writeToReplayFrame__FPv");
extern "C" void cGFFinishlineState_writeToReplayFrame(void* p, void* s);
extern "C" void cGFEventStartState_writeToReplayFrame(void* p, void* s);
extern "C" void cGFTrackStartState_writeToReplayFrame(void* p, void* s);
extern "C" void cGFNextHeatState_writeToReplayFrame(void* p, void* s);
extern "C" void cGFUserSelectState_writeToReplayFrame(void* p, void* s);

/**
 * Writes the game's state to a replay frame: flags, snowfall, the game-state ids and each game-flow state
 * object.
 *
 * The ids are the current (+0x214), previous (+0x218), pending (+0x210) and pending-startup (+0x21C) ones.
 *
 * @param stream  the replay frame (On Tour cReplayFramePtr*)
 */
extern "C" void cGame_writeToReplayFrame(cGame* self, cStream_00231348* stream)
{
    ((cSerObj_00231348*)self->FlagManager)->serialize(stream);
    ((cSerObj_00231348*)self->SnowFall)->serialize(stream);
    stream->write((char*)&self->mCurrentStateID, 4);
    stream->write((char*)&self->mPrevStateID, 4);
    stream->write((char*)&self->mPendingStateID, 4);
    stream->write((char*)&self->mPendingStateStartupFlag, 4);
    cGFEventStartState_writeToReplayFrame((char*)&self->mStates, stream);
    cGFPreEventState_writeToReplayFrame_s((char*)self + 0xD0, stream);
    cGFGateState_writeToReplayFrame_s((char*)self + 0xE0, stream);
    cGFRidingState_writeToReplayFrame_s((char*)self + 0xF0, stream);
    cGFFinishlineState_writeToReplayFrame((char*)self + 0x100, stream);
    cGFEndState_writeToReplayFrame_s((char*)self + 0x118, stream);
    cGFPostEventState_writeToReplayFrame_s((char*)self + 0x128, stream);
    cGFEndWaitState_writeToReplayFrame_s((char*)self + 0x138, stream);
    cGFTransportWaitState_writeToReplayFrame_s((char*)self + 0x148, stream);
    cGFTrackStartState_writeToReplayFrame((char*)self + 0x158, stream);
    cGFSectionLoadState_writeToReplayFrame_s((char*)self + 0x178, stream);
    cGFPodiumState_writeToReplayFrame_s((char*)self + 0x18C, stream);
    cGFNextHeatState_writeToReplayFrame((char*)self + 0x19C, stream);
    cGFUserSelectState_writeToReplayFrame((char*)self + 0x1B8, stream);
    cGFSessionTeleportState_writeToReplayFrame_s((char*)self + 0x1DC, stream);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_readFromReplayFrame);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: these state loaders are declared with one parameter in their units; this caller also passes the stream.
extern "C" void cGFEventStartState_readFromReplayFrame(void* state, void* stream);
void cGFPreEventState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFPreEventState_readFromReplayFrame__FPv");
void cGFGateState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFGateState_readFromReplayFrame__FPv");
void cGFRidingState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFRidingState_readFromReplayFrame__FPv");
extern "C" void cGFFinishlineState_readFromReplayFrame(void* state, void* stream);
void cGFEndState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFEndState_readFromReplayFrame__FPv");
void cGFPostEventState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFPostEventState_readFromReplayFrame__FPv");
void cGFEndWaitState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFEndWaitState_readFromReplayFrame__FPv");
void cGFTransportWaitState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFTransportWaitState_readFromReplayFrame__FPv");
extern "C" void cGFTrackStartState_readFromReplayFrame(void* state, void* stream);
void cGFSectionLoadState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFSectionLoadState_readFromReplayFrame__FPv");
void cGFPodiumState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFPodiumState_readFromReplayFrame__FPv");
extern "C" void cGFNextHeatState_readFromReplayFrame(void* state, void* stream);
extern "C" void cGFUserSelectState_readFromReplayFrame(void* state, void* stream);
void cGFSessionTeleportState_readFromReplayFrame_14D0(void* state, void* stream) __asm__("cGFSessionTeleportState_readFromReplayFrame__FPv");

class cGameState_14D0;

class cStream_14D0 {
public:
    virtual void v01();
    virtual void read(void* dst, int size);
};

class cLoadable_14D0 {
public:
    int pad[3];
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
    virtual void load(cStream_14D0* stream);
};


/**
 * Restores the game's flow state from a replay frame: the flag manager, snowfall, state ids and every game-flow
 * state.
 *
 * Loads FlagManager and SnowFall (their load virtuals), reads mCurrentStateID, mPrevStateID, mPendingStateID and
 * mPendingStateStartupFlag (4 bytes each), then each cGF*State in mStates (event start, pre-event, gate, riding,
 * finish line, end, post-event, end wait, transport wait, track start, section load, podium, next heat, user
 * select, session teleport; the state at +0x1EC is not read). mCurrentState and mPrevState are pointed at the state
 * object for their id (1-16; 0 is none, an unknown id becomes 0). The comment above the definition calls it a save
 * stream; it is a replay frame.
 *
 * @param self_  the game (this)
 * @param stream  the replay frame to read from (On Tour cReplayFramePtr*)
 */
// Game load state machine: restores the state ids and every state from a save stream.
extern "C" void cGame_readFromReplayFrame(cGame* self_, cStream_14D0* stream)
{
    cGame* self = self_;
    ((cLoadable_14D0*)self->FlagManager)->load(stream);
    ((cLoadable_14D0*)self->SnowFall)->load(stream);
    stream->read(&self->mCurrentStateID, 4);
    stream->read(&self->mPrevStateID, 4);
    stream->read(&self->mPendingStateID, 4);
    stream->read(&self->mPendingStateStartupFlag, 4);
    cGFEventStartState_readFromReplayFrame((char*)self + 0xB0, stream);
    cGFPreEventState_readFromReplayFrame_14D0((char*)self + 0xD0, stream);
    cGFGateState_readFromReplayFrame_14D0((char*)self + 0xE0, stream);
    cGFRidingState_readFromReplayFrame_14D0((char*)self + 0xF0, stream);
    cGFFinishlineState_readFromReplayFrame((char*)self + 0x100, stream);
    cGFEndState_readFromReplayFrame_14D0((char*)self + 0x118, stream);
    cGFPostEventState_readFromReplayFrame_14D0((char*)self + 0x128, stream);
    cGFEndWaitState_readFromReplayFrame_14D0((char*)self + 0x138, stream);
    cGFTransportWaitState_readFromReplayFrame_14D0((char*)self + 0x148, stream);
    cGFTrackStartState_readFromReplayFrame((char*)self + 0x158, stream);
    cGFSectionLoadState_readFromReplayFrame_14D0((char*)self + 0x178, stream);
    cGFPodiumState_readFromReplayFrame_14D0((char*)self + 0x18C, stream);
    cGFNextHeatState_readFromReplayFrame((char*)self + 0x19C, stream);
    cGFUserSelectState_readFromReplayFrame((char*)self + 0x1B8, stream);
    cGFSessionTeleportState_readFromReplayFrame_14D0((char*)self + 0x1DC, stream);
    switch (self->mCurrentStateID) {
    case 0:
        (*(cGameState_14D0**)&self->mCurrentState) = 0;
        break;
    case 1:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0xB0);
        break;
    case 2:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0xD0);
        break;
    case 3:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0xE0);
        break;
    case 4:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0xF0);
        break;
    case 5:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x100);
        break;
    case 6:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x118);
        break;
    case 7:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x128);
        break;
    case 8:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x138);
        break;
    case 9:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x148);
        break;
    case 10:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x158);
        break;
    case 11:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x178);
        break;
    case 12:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x18C);
        break;
    case 13:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x19C);
        break;
    case 14:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x1B8);
        break;
    case 15:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x1DC);
        break;
    case 16:
        (*(cGameState_14D0**)&self->mCurrentState) = (cGameState_14D0*)((char*)self + 0x1EC);
        break;
    default:
        (*(cGameState_14D0**)&self->mCurrentState) = 0;
        self->mCurrentStateID = 0;
        break;
    }
    switch (self->mPrevStateID) {
    case 1:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0xB0);
        break;
    case 2:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0xD0);
        break;
    case 3:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0xE0);
        break;
    case 4:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0xF0);
        break;
    case 5:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x100);
        break;
    case 6:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x118);
        break;
    case 7:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x128);
        break;
    case 8:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x138);
        break;
    case 9:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x148);
        break;
    case 10:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x158);
        break;
    case 11:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x178);
        break;
    case 12:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x18C);
        break;
    case 13:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x19C);
        break;
    case 14:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x1B8);
        break;
    case 15:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x1DC);
        break;
    case 16:
        (*(cGameState_14D0**)&self->mPrevState) = (cGameState_14D0*)((char*)self + 0x1EC);
        break;
    default:
        self->mPrevStateID = 0;
    case 0:
        (*(cGameState_14D0**)&self->mPrevState) = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_activatePauseMenu);
#ifdef SKIP_ASM
extern int D_004A2E9C;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getRiderCtrlID(void* iface, int i);
extern "C" int cGame_canUserActivatePauseMenu(cGame* self);
int cInput_getState(void* obj, int a1);

static inline void** PlayerObj_1840(int p)
{
    return *(void***)((char*)SSXApp + (p << 2) + 0xB0);
}

static inline int IsReady_1840(void** o)
{
    return *o && *(int*)*o;
}

static inline int Count_1840(char* self)
{
    return *(int*)(*(char**)(self + 0xC) + 0x7C);
}

static inline char* At_1840(char* self, int i)
{
    return *(char**)(*(char**)(self + 0xC) + (i << 2) + 0x40);
}

/**
 * Decides whether the pause menu should open this frame, and for which player.
 *
 * Returns 0 while a network error is shown. Looks at the replay's player in replay mode, else at each human
 * rider's controller (cBENewPlayerInterface_getRiderCtrlID): unplugged controllers set bits in a mask, and the first with pause pressed
 * (input 0x3C) becomes the owner. The mask is stored at +0x224 (On Tour ControllerUnplugged); an unplugged
 * controller always pauses, owned by the presser or the first ready player. Otherwise a press pauses when online
 * or when cGame_canUserActivatePauseMenu allows it. The owner goes to +0x228 (PauseOwner).
 *
 * @return 1 when the pause menu should open
 */
extern "C" int cGame_activatePauseMenu(cGame* self)
{
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mDisplayError > 0) {
        return 0;
    }
    int mask = 0;
    void* bi = cBE_getInterface_Fv(cBE_getBE(), 1);
    char* replay = (char*)self->Replay;
    int sel = -1;
    if (*(int*)(replay + 0x608)) {
        int ctrlId = *(int*)(replay + 0x28);
        void** controller = PlayerObj_1840(ctrlId);
        if (!IsReady_1840(controller)) {
            mask = 1 << ctrlId;
        } else if (D_004A2E9C == 0) {
            if (cInput_getState(controller, 0x3C)) {
                sel = ctrlId;
            }
        }
    } else {
        int i;
        for (i = 0; i < Count_1840(((char*)self)); i++) {
            char* rider = At_1840(((char*)self), i);
            if (*(int*)(*(char**)(rider + 0x18) + 0x87C)) {
                int ctrlId = cBENewPlayerInterface_getRiderCtrlID(bi, i);
                void** controller = PlayerObj_1840(ctrlId);
                if (!IsReady_1840(controller)) {
                    mask |= 1 << ctrlId;
                } else if (sel < 0 && D_004A2E9C == 0) {
                    if (cInput_getState(controller, 0x3C)) {
                        sel = ctrlId;
                    }
                }
            }
        }
    }
    self->ControllerUnplugged = mask;
    if (mask) {
        if (sel < 0) {
            int i;
            sel = 0;
            for (i = 0; i < Count_1840(((char*)self)); i++) {
                char* rider = At_1840(((char*)self), i);
                if (*(int*)(*(char**)(rider + 0x18) + 0x87C)) {
                    int ctrlId = cBENewPlayerInterface_getRiderCtrlID(bi, i);
                    if (IsReady_1840(PlayerObj_1840(ctrlId))) {
                        sel = ctrlId;
                        break;
                    }
                }
            }
        }
        self->PauseOwner = sel;
        return 1;
    }
    if (sel >= 0) {
        cBE_getInterface_Fv(cBE_getBE(), 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 || cGame_canUserActivatePauseMenu(self)) {
            self->PauseOwner = sel;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_canUserActivatePauseMenu);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_IsPauseMenuAllowed(void* a);

/**
 * Tells whether the player may open the pause menu now.
 *
 * Not while mPendingStateID (+0x210) or mCurrentStateID (+0x214) is 1, 5, 10 or 14 (On Tour eGFState EventStart,
 * Finishline, TrackStart, UserSelect), and only when the script engine allows it (cSSXScriptEngine_IsPauseMenuAllowed).
 *
 * @return 1 when the pause menu may open, otherwise 0
 */
extern "C" int cGame_canUserActivatePauseMenu(cGame* self)
{
    int mode = self->mPendingStateID;
    if (mode != 0) {
        if (mode == 5 || mode == 14 || mode == 1 || mode == 10) {
            return 0;
        }
    }
    int allowed = 0;
    if (cSSXScriptEngine_IsPauseMenuAllowed(ScriptEngine) != 0) {
        int currentStateId = self->mCurrentStateID;
        allowed = currentStateId != 5 && currentStateId != 14 && currentStateId != 1 && currentStateId != 10;
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_drawSplash);
#ifdef SKIP_ASM
#include "bx/math3d.h"

struct sQuadVtx {
    sQuadVtx() {}
    float u, v, q, pad;
    int r, g, b, a;
    t4Vector pos;
};

/**
 * Draws the current texture over a unit quad: sets the draw state's texture (+0x10) to @p value and draws four
 * vertices.
 *
 * The vertices span (0, 0)-(1, 1) with matching texture coordinates, colour 0x80 (neutral) in all channels and q = 1;
 * they go to GrMan's vertex-drawing slot (vtable +0x238) with count 4.
 *
 * @param value  the texture id
 */
extern "C" void cLoadScreen_drawSplash(cLoadScreen* self, int value) {
    sQuadVtx verts[4];
    t4Vector cornerPos;
    int i;
    char* obj;

    for (i = 0; i < 4; i++) {
        verts[i].a = 0x80;
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
        verts[i].q = 1.0f;
    }
    cornerPos.x = 0.0f; cornerPos.y = 0.0f; cornerPos.z = 0.0f; cornerPos.w = 1.0f;
    verts[0].pos = cornerPos;
    cornerPos.x = 1.0f; cornerPos.y = 0.0f; cornerPos.z = 0.0f;
    verts[1].pos = cornerPos;
    cornerPos.x = 0.0f; cornerPos.y = 1.0f; cornerPos.z = 0.0f;
    verts[2].pos = cornerPos;
    cornerPos.x = 1.0f; cornerPos.y = 1.0f; cornerPos.z = 0.0f;
    verts[3].pos = cornerPos;
    verts[0].u = 0.0f; verts[0].v = 0.0f;
    verts[1].u = 1.0f; verts[1].v = 0.0f;
    verts[2].u = 0.0f; verts[2].v = 1.0f;
    verts[3].u = 1.0f; verts[3].v = 1.0f;
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = value;
    obj = (char*)GrMan->mVtbl;
    (*(void (**)(void*, int, void*, int))(obj + 0x23C))((char*)GrMan + *(short*)(obj + 0x238), 4, verts, 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_loseFocus);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_loseFocus(void* self);
extern char* cAppMan_TheAppMan;

/**
 * Lets a load screen lose focus only when the app's progress value (cAppMan_TheAppMan + 0x38) is exactly 100.
 *
 * Then it returns cFadeScreen_loseFocus; otherwise 0. It fills the loseFocus slot (+0x24) of cFELoadScreen_vtbl and
 * cLoadScreen_vtbl. The tree now calls this function cLoadScreen_loseFocus.
 *
 * @return cFadeScreen_loseFocus's result, or 0
 */
extern "C" int cLoadScreen_loseFocus(cLoadScreen* self)
{
    if (*(float*)(cAppMan_TheAppMan + 0x38) == 100.0f) {
        return cFadeScreen_loseFocus(self);
    }
    return 0;
}
#endif

extern "C" void* cFadeScreen_update(void* self);

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_update__FPv);
#ifdef SKIP_ASM
/**
 * Updates a load screen: calls cFadeScreen_update.
 *
 * It fills the update slot (+0x34) of the load-screen vtable cLoadScreen_vtbl; cFELoadScreen_update and the pre-start
 * screens' updates call it too.
 *
 * @return cFadeScreen_update's result
 */
void* cLoadScreen_update(void* self)
{
    return cFadeScreen_update(self);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_cFadeScreen);
#ifdef SKIP_ASM
extern char cFadeScreen_vtbl[];

/**
 * Constructs a screen fade: vtable cFadeScreen_vtbl, fade timer (+0x4) 0.2, ending flag (+0x8) 0.
 *
 * cGame_cGame, cFE_cFE, the debug front end and the load-screen constructors call it for their fade base.
 *
 * @return @p self
 */
extern "C" void* cFadeScreen_cFadeScreen(void* self)
{
    *(void**)((char*)self + 0) = cFadeScreen_vtbl;
    *(float*)((char*)self + 4) = 0.20000000298023224f;
    *(int*)((char*)self + 8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_loseFocus);
#ifdef SKIP_ASM
/**
 * Starts the screen fade-out and reports whether it has gone far enough.
 *
 * Sets the fade-out flag (offset 8), which makes the per-frame update raise the fade level by 1/60; the level
 * starts at 0.2 when constructed.
 *
 * @return 1 once the fade level (offset 4) is at least 0.2, otherwise 0
 */
extern "C" int cFadeScreen_loseFocus(void* self)
{
    *(int*)((char*)self + 8) = 1;
    return *(float*)((char*)self + 4) >= 0.20000000298023224f;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_update);
#ifdef SKIP_ASM
/**
 * Advances a screen fade by one frame.
 *
 * While IsEnding (+0x8) is set adds 1/60 to FadeTimer (+0x4); otherwise subtracts 1/60 and clamps at 0. The growing
 * side is not clamped here (also so in On Tour).
 *
 * @return no meaningful value (the body returns nothing)
 */
// PORT: the unit declares this void*-returning; the body returns nothing meaningful.
extern "C" void* cFadeScreen_update(void* self)
{
    if (*(int*)((char*)self + 8) != 0) {
        *(float*)((char*)self + 4) += 0.01666666753590107f;
    } else {
        *(float*)((char*)self + 4) -= 0.01666666753590107f;
        if (*(float*)((char*)self + 4) < 0.0f) {
            *(float*)((char*)self + 4) = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_drawFade);
#ifdef SKIP_ASM
extern "C" void c2DRender_drawRect4(void* self, float* p0, float* p1, float* col, float z);
extern int D_004A28AC;
extern char t4mat_Identity[];

struct sRS231D60 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 22;
    int fC;
    int f10;
};

extern sRS231D60 sDrawState_s_defaultDrawState;

class cCtx231D60 {
public:
    char pad0[0xE84];
    sRS231D60* top;
    char pad1[0x10D8 - 0xE88];
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
    virtual void v25();
    virtual void v26(int a, float x, float y, float w, float h, float zn, float zf, int b);
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34(void* p);
};

// Typed view of the render context (the unit declares GrMan as void*).

// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp231D60(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

/**
 * Draws a screen fade: a full-screen quad whose alpha is the fade timer times 5, clamped to [0, 1].
 *
 * Does nothing and returns 0 while the timer (+0x4) is 0 or less. Otherwise pushes a copy of the draw state, resets it
 * to sDrawState_s_defaultDrawState, sets the full-screen viewport and the identity matrix and the blend bits, draws a
 * quad from (-1, -1) to (1, 1) with colour {alpha, 0, 0, 0} through c2DRender_drawRect4 (D_004A28AC), and pops the draw
 * state. cGame_render, cRealFE_render, cDebugFE_render and cFELoadScreen_render call it.
 *
 * @return 1 once the fade is fully opaque (alpha >= 1), otherwise 0
 */
extern "C" int cFadeScreen_drawFade(void* self)
{
    if (*(float*)((char*)self + 4) > 0.0f) {
    cPS2GraphicsMan* ctx = GrMan;
    ((cCtx231D60*)ctx)->top[1] = ((cCtx231D60*)ctx)->top[0];
    ((cCtx231D60*)ctx)->top++;
    *((cCtx231D60*)GrMan)->top = sDrawState_s_defaultDrawState;
    ((cCtx231D60*)GrMan)->v26(0, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0);
    ((cCtx231D60*)GrMan)->v34(t4mat_Identity);
    ((cCtx231D60*)GrMan)->top->f8_5 = 31;
    ((cCtx231D60*)GrMan)->top->f4_23 = 2;
    ((cCtx231D60*)GrMan)->top->f4_20 = 0;
    ((cCtx231D60*)GrMan)->top->f4_12 = 0;
    float alpha = Clamp231D60(*(float*)((char*)self + 4) * 5.0f, 0.0f, 1.0f);
    float minCorner[2];
    float maxCorner[2];
    float col[4];
    minCorner[0] = -1.0f;
    minCorner[1] = -1.0f;
    maxCorner[0] = 1.0f;
    maxCorner[1] = 1.0f;
    col[0] = alpha;
    col[1] = 0.0f;
    col[2] = 0.0f;
    col[3] = 0.0f;
    c2DRender_drawRect4((void*)D_004A28AC, minCorner, maxCorner, col, 0.0f);
    *(int*)&GrMan->m_pCurrentDrawState -= 20;
    return alpha >= 1.0f;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadIndicator_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float floorf_231F80(float x)
{
    float f;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(f) : "f"(x));
    if (x < f) {
        f -= 1.0f;
    }
    return f;
}

/**
 * Advances a load indicator's animation phase (its first field) by one frame, wrapping it to [0, 1).
 */
extern "C" void cLoadIndicator_update(float* self)
{
    *self += 0.01666666753590107f;
    *self -= floorf_231F80(*self);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadIndicator_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float fCos(float);
extern char t4mat_Identity[];

struct sV2_1FC0 {
    float x, y;
    sV2_1FC0() {}
    sV2_1FC0(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sV4_1FC0 {
    float x, y, z, w;
    sV4_1FC0(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};

struct sCol_1FC0 {
    float r, g, b, a;
};

struct sFade_1FC0 {
    float t;            // 0x0
    float f4;           // 0x4
    float f8;           // 0x8
    sCol_1FC0 col;      // 0xC
};

struct sGs_1FC0 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 19;
    unsigned int f8_29 : 3;
    unsigned int wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF10(int v) { f10 = v; }
};

extern sGs_1FC0 D_00501420_1FC0 __asm__("sDrawState_s_defaultDrawState");

class cGfx_1FC0 {
public:
    char pad[0xE84];
    sGs_1FC0* stateTop;
    char padE88[0x10D8 - 0xE88];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetScreenWidth();
    virtual unsigned int GetScreenHeight();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual void PushViewport();
    virtual void PopViewport();
    virtual void v22(float, float, float, float, int);
    virtual void v23(); virtual void v24();
    virtual void SetViewportOrthographic(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void PushView();
    virtual void PopView();
    virtual void v32();
    virtual void SetView(void*);
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76();
    virtual void DrawSprite(const sV4_1FC0&, const sV2_1FC0&, const sV2_1FC0&, const sV2_1FC0&, const sCol_1FC0&);
};

struct sVE_1FC0 {
    short delta;
    short index;
    void (*fn)(void*, float, float, float, float, int);
};

extern char* D_004A28A8_1FC0 __asm__("SSXApp");

/**
 * Draws the load indicator: a pulsing sprite at (599, 445) in a full-screen 2D pass.
 *
 * Pushes a copy of the draw state, begins a scene and 2D mode, resets the state to sDrawState_s_defaultDrawState with
 * its blend bits, loads t4mat_Identity, sets the full-screen viewport (640x480) and the texture LoadIconTextureID
 * (SSXApp + 0xA0). The sprite is 18 high and (sin(2 * pi * phase) + 1) * 9 wide, phase being the first field (advanced
 * by cLoadIndicator_update); its colour (+0xC) takes the field at +0x4 as its first component. Then ends 2D mode and
 * the scene and pops the draw state. The pre-start load screens call it.
 */
extern "C" void cLoadIndicator_render(sFade_1FC0* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    ((cGfx_1FC0*)ps2GraphicsMan)->stateTop[1] = ((cGfx_1FC0*)ps2GraphicsMan)->stateTop[0];
    ((cGfx_1FC0*)ps2GraphicsMan)->stateTop++;
    ((cGfx_1FC0*)GrMan)->PushViewport();
    ((cGfx_1FC0*)GrMan)->PushView();
    *((cGfx_1FC0*)GrMan)->stateTop = D_00501420_1FC0;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_23 = 2;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_12 = 20;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_2 = 5;
    ((cGfx_1FC0*)GrMan)->stateTop->f8_5 = 0x1F;
    ((cGfx_1FC0*)GrMan)->SetView(t4mat_Identity);
    {
        sVE_1FC0* vt = (sVE_1FC0*)GrMan->mVtbl;
        char* thisp = (char*)GrMan + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_1FC0*)GrMan)->GetScreenWidth(), (float)((cGfx_1FC0*)GrMan)->GetScreenHeight(), 1);
    }
    ((cGfx_1FC0*)GrMan)->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 1, 0, 0.0f, 1.0f);
    float pulse = fCos(self->t * 6.2831854820251465f);
    sV2_1FC0 size((pulse + 1.0f) * 9.0f, 18.0f);
    sV4_1FC0 pos(599.0f, 445.0f, 0.0f, 1.0f);
    int f10v = *(int*)(D_004A28A8_1FC0 + 0xA0);
    *(short*)((char*)((cGfx_1FC0*)GrMan)->stateTop + 0x10) = f10v;
    self->col.r = self->f4;
    sV2_1FC0 uv0(0.0f, 0.0f);
    sV2_1FC0 uv1(1.0f, 1.0f);
    ((cGfx_1FC0*)GrMan)->DrawSprite(pos, size, uv0, uv1, self->col);
    ((cGfx_1FC0*)GrMan)->PopView();
    ((cGfx_1FC0*)GrMan)->PopViewport();
    ((cGfx_1FC0*)GrMan)->stateTop--;
}
#endif

extern void* cPreStartScreen_vtbl[];

//100%
INCLUDE_ASM("main/gameload", cPreStartScreen_cPreStartScreen__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the pre-start screen app module: installs its vtable (cPreStartScreen_vtbl) and clears +0x4 and +0x8.
 *
 * @return @p self
 */
void* cPreStartScreen_cPreStartScreen(void* self)
{
    int zero = 0;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)self = (int)(void*)cPreStartScreen_vtbl;
    *(int*)((char*)self + 0x8) = zero;
    return self;
}
#endif

