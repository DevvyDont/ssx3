#include "vtable.h"
#include "common.h"
#include "main/loadindicator.h"
#include "bx/bxstring.h"
#include "be/beinterface.h"
#include "ui/uistate.h"
#include "ai/focusbox.h"
#include "object/bucketman.h"
#include "world/worldcache.h"
#include "ai/player.h"
#include "replay/replay.h"
#include "visualfx/fog.h"
#include "wscript/wscriptman.h"
#include "wscript/wscriptcache.h"
#include "main/gameviewman.h"
#include "camera/camera.h"
#include "object/flagset.h"
#include "visualfx/snowfall.h"
#include "ui/uiengine.h"
#include "bx/appman.h"
#include "main/gamestate.h"
#include "sound/icepick/worldtriggermanager.h"
#include "camera/trigger/cameratriggerman.h"
#include "main/fadescreen.h"
#include "util/menusys.h"
#include "main/sectionman.h"
#include "ai/ai.h"
#include "be/beintnewrace.h"
#include "main/loadscreens.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "render/playerlights.h"
#include "render/2drender.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "main/crowdman2d.h"
#include "main/crowdman3d.h"
#include "main/inputsync.h"
#include "object/instanceman.h"
#include "object/multiparticle.h"
#include "render/font.h"
#include "render/occlusionman.h"
#include "sound/icepick/worldsound.h"
#include "visualfx/worldlightman.h"

extern "C" void cGame_load(cGame* self);

//100%
INCLUDE_ASM("main/gameload", cGame_load);
#ifdef SKIP_ASM
// FAKEMATCH: an unused 11-byte copy `name` of D_0047B6A8 (the call passes D_0047B6A8.c directly); plain form 99.21%.
// Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
void cMemMan_free(void* p);
void cLoadTimer_startload(void* bar, float v);
void cLoadTimer_setnextpercent(void* bar, float v);
extern "C" void cFELocale_loadOV(void* p);
extern "C" void cSSXScriptEngine_Load(void* eng);
extern "C" void* __5cFontPCci(void* p, const char* name, int a);
extern "C" void cGameComm_syncGame(void* comm);
void cBEBoltInterface_refreshVisibleBolts(void* iface);
extern "C" void cBucketMan_init(void* bm, int n);
extern "C" void cBucketMan_setsortflag(void* bm, int a);
extern "C" void cCameraTriggerMan_init(void* p);
void* cStreamPause_construct();
void* cOcclusionMan_cOcclusionMan(void* p);
extern "C" void* cWorld_cWorld(void* p);
extern "C" void* cWorldLightMan_cWorldLightMan(void* p, void* a);
extern "C" void* cWScriptMan_cWScriptMan(void* p);
extern "C" void* __11cCrowdMan2d(void* p);
extern "C" void* __11cCrowdMan3d(void* p);
extern "C" void* __17cMultiParticleMan(void* p);
extern "C" void* __12cInstanceManii(void* p, int a, int b);
extern "C" void cIrradianceDataBase_Load(void* p);
void* cBXLoadIndicator_cBXLoadIndicator(void* p);
struct cBigFile;
void* cBigFile_cBigFile(cBigFile* p);
extern "C" void cGame_loadTrack(cGame* self);
extern "C" void* __7cReplay(void* p);
extern "C" void* cFXLoader_cFXLoader(void* p, int a);
extern "C" void* __10cInputSyncii(void* p, int n, int m);
int BXrand();
extern "C" void GMsrand(int seed);
extern "C" void* cAI_cAI(void* p);
extern "C" void* __12cPlaybackMani(void* p, int n);
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
extern "C" void* __8cMenuSys(void* p);
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
extern int cBESaveInterface_bDoReplay;
extern int gReplayPadNum;
extern void* cGameState_mpGame;
extern char gBucketMan_gl __asm__("gBucketMan");
extern char cIrradianceDataBase_TheDataBase[];
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
    void* bm = &gBucketMan_gl;
    void** lights = ((void**)&self->SplashManager[0]);
    cLoadTimer_startload(&cAppMan_TheAppMan->LoadTimer, 13.199999809265137f);
    cLoadTimer_setnextpercent(&cAppMan_TheAppMan->LoadTimer, 43.5f);
    cFELocale_loadOV(SSXApp->FELocale);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryVoidGL* vt = (sVEntryVoidGL*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
        vt[44].fn(vthis(ps2GraphicsMan, vt[44]), 0);
    }
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cSSXScriptEngine_Load(ScriptEngine);
    sGridGL* grid = new (cMemMan_alloc(168, str_Nash, 0, 0)) sGridGL;
    *pGrid = grid;
    grid->a4 = 0;
    grid->a0 = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    self->DebugFont = (cFont*)__5cFontPCci(operator new(132, str_DebugFont_b, 0, 0), str_data_fonts_debugfnt_sfn_b, 0);
    if (cGameComm_mGameComm != 0)
        cGameComm_syncGame(cGameComm_mGameComm);
    cBEBoltInterface_refreshVisibleBolts(cBE_getInterface_Fv(cBE_getBE(), 9));
    cBucketMan_init(bm, 9);
    cBucketMan_setsortflag(bm, 1);
    cCameraTriggerMan_init(((char*)&gCamTriggerMan));
    self->StreamPause = (cStreamPause*)cStreamPause_construct();
    self->OcclusionMan = (cOcclusionMan*)cOcclusionMan_cOcclusionMan(operator new(276, str_OcclusionMan, 0x20000000, 0));
    self->World = (cWorld*)cWorld_cWorld(operator new(0x18, str_World, 0, 0));
    self->WorldLightMan = (cWorldLightMan*)cWorldLightMan_cWorldLightMan(operator new(12960, str_WorldLightMan, 0x20000000, 0), (sGridGL*)self->BXNash);
    self->WScriptMan = (cWScriptMan*)cWScriptMan_cWScriptMan(operator new(1176, str_WScriptMan, 0x20000000, 0));
    self->CrowdMan2d = (cCrowdMan2d*)__11cCrowdMan2d(operator new(10016, str_CrowdMan2d, 0x20000000, 0));
    self->CrowdMan3d = (cCrowdMan3d*)__11cCrowdMan3d(operator new(2500, str_CrowdMan3d, 0x20000000, 0));
    self->MultiParticleMan = (cMultiParticleMan*)__17cMultiParticleMan(operator new(0x10, str_MultiParticleMan, 0x20000000, 0));
    self->InstanceMan = (cInstanceMan*)__12cInstanceManii(operator new(0x10, str_InstancesMan, 0x20000000, 0), 0x80, 0x40);
    cIrradianceDataBase_Load(cIrradianceDataBase_TheDataBase);
    self->LoadIndicator = (cBXLoadIndicator*)cBXLoadIndicator_cBXLoadIndicator(operator new(0x10, str_LoadIndicator, 0x20000000, 0));
    self->mpTrackBigFile = (cBigFile*)cBigFile_cBigFile((cBigFile*)operator new(8, str_BigFile, 0, 0));
    cGame_loadTrack(self);
    self->Replay = (cReplay*)__7cReplay(operator new(1672, str_Replay, 0, 0));
    self->FXLoader = (cFXLoader*)cFXLoader_cFXLoader(operator new(1, str_FXLoader, 0, 0), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        float dt = SSXApp->TimerPeriod;
        int n = (int)((cBENetworkInterface_networkOpt.fLatency + (float)cGameComm_mGameComm->mNumSamples * dt) / dt) + 2;
        self->InputSync = (cInputSync*)__10cInputSyncii(operator new(68, str_InputSync, 0, 0), n, cGameComm_mGameComm->mNumSamples);
    } else {
        GMsrand(BXrand());
    }
    {
        cAI* ai = (cAI*)cAI_cAI(operator new(208, str_AI, 0, 0));
        self->AI = (cAI*)ai;
        sVEntryVoidGL* vt = (sVEntryVoidGL*)ai->mVtbl;
        vt[2].fn(vthis(ai, vt[2]));
    }
    cLoadTimer_setnextpercent(&cAppMan_TheAppMan->LoadTimer, 55.0f);
    self->PlaybackMan = (cPlaybackMan*)__12cPlaybackMani(operator new(0x7C, str_PlaybackMan, 0x20000000, 0), 40);
    cReplay_init(self->Replay);
    void* race = cBE_getInterface_Fv(SSXApp->BE, 5);
    int replay = cBESaveInterface_bDoReplay;
    if (replay != 0) {
        cAI_initAIRiders((char*)self->AI);
        signed char* gs = (signed char*)&cBENewRaceInterface_newRace;
        cGameModeMan_initGameMode(SSXApp->GameModeMan, gs[0x4A]);
        cAI_setPodiumActorList((char*)self->AI, cBESaveInterface_getGameReplayBuffer(race) + 0x20);
        int mode = gs[0x48];
        int sub = gs[0x4A];
        if (mode == 4) {
        } else if (mode == 5 && sub != 4) {
        } else if (mode == 6 && sub != 5) {
        } else {
            cAI* ai = self->AI;
            if (ai->NumberOfComputersAllocated == 0) {
                if (mode == 0)
                    cAI_initAIActors(ai, 0);
                else
                    cAI_initAIActors(ai, 2);
                self->AI->ComputerActorsActive = 0;
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
        self->ViewMan = (cGameViewMan*)cGameViewMan_cGameViewMan(operator new(0x38, str_GameViewMan, 0x20000000, 0), cBENewRaceInterface_newRace.iNumberHumans);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cBENetworkInterface_networkOpt.bIsHost == 0) {
        cPlayer* secondPlayer = self->AI->Players[1];
        void* cam = ((char*)secondPlayer) != 0 ? (void*)secondPlayer->mRider : 0;
        cCamera_switchChaseControllerTargetRider(self->ViewMan->PlayerCamera[0], cam);
    }
    cMenuSys* hud = (cMenuSys*)__8cMenuSys(operator new(104, str_MenuSys, 0, 0));
    self->MenuSys = (cMenuSys*)hud;
    hud->Font = (cFont*)((void*)self->DebugFont);
    self->MenuSys->Input = (cInput*)((void*)SSXApp->Inputs[0]);
    char* file = FILE_load(str_data_fonts_menu_ssh_b, 0x100);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVEntryGL* vt = (sVEntryGL*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
        int result = vt[46].fn(vthis(ps2GraphicsMan, vt[46]), file + *(int*)(file + 0x14), str_menu_b, 0, 1, -1);
        self->mBoxTextureID = result;
        self->MenuSys->BoxTexture = result;
    }
    if (file != 0)
        cMemMan_free(file);
    cLoadTimer_setnextpercent(&cAppMan_TheAppMan->LoadTimer, 93.0f);
    for (unsigned int i = 0; i < 2; i++) {
        worlds[i] = cTransitionMan_cTransitionMan(operator new(132, str_cTransitionMan, 0, 0), 3, i);
        lights[i] = cSplashManager_cSplashManager(operator new(4144, str_cSplashManager, 0, 0), 2, i);
    }
    self->CutSceneTransitionMan = (cCutSceneTransitionMan*)cCutSceneTransitionMan_cCutSceneTransitionMan(operator new(72, str_cCutSceneTransitionMan, 0, 0), 3);
    self->FlagManager = (cFlagManager*)cFlagManager_cFlagManager(operator new(5912, str_cFlagManager, 0, 0), 2);
    self->SnowFall = (cSnowFall*)cSnowFall_cSnowFall(operator new(0x60, str_SnowFall, 0, 0), 2);
    self->mUpdateTick = 0;
    cSSXAudio_InGameLoad(gSSXAudio);
    cLoadTimer_setnextpercent(&cAppMan_TheAppMan->LoadTimer, 99.0f);
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
        cReplay_setReplayPadNum(self->Replay, gReplayPadNum);
        cGame_setState((self), 0x10, 0, 0);
    } else {
        cGame_setPreloadState((self), 10);
        do {
            cGame_updateState((self));
            THREAD_yield(0);
        } while (!cGame_isPreloadedStateReady((self)));
        if (cGameComm_mGameComm != 0)
            cGameComm_syncWait(cGameComm_mGameComm);
        if (cBENewRaceInterface_newRace.cGamePath == 0)
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);


extern char* cBXString_EmptyString;
extern int cBENewRaceInterface_bIsBamLocation;
extern char str_data_worlds_c[];
extern char str_big_b[];
extern char str_SkyBox[];
extern char str_SectionMan[];
extern char str_SurfPropCache[];
extern char str_cWorldSound[];
extern char str_cPS2FogParticleMan[];
extern char str_bar_b[];
extern char str_cam[];
extern void* cPS2FogParticleMan_vtbl[];

extern "C" void* cBXString_cBXString2(cBXString* self, const char* s);
extern "C" cBXString* cBXString_operatorPlus1(cBXString* self, cBXString* a, const char* str);
extern "C" cBXString* cBXString_operatorPlus2(cBXString* self, const char* str, cBXString* b);
extern "C" void cBXString__cBXString(cBXString* self, int flags);
extern "C" cBXString* cBXString_operatorE(cBXString* self, cBXString* o);
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cBigFile_open(void* bf, const char* name, int flags);
extern "C" void cWorld_load(void* a, const char* name, void* b);
cSkyBox* cSkyBox_cSkyBox(void* p);
extern "C" cSectionMan* __11cSectionMani(void* p, void* game);
extern "C" void func_002C2F18(void* p);
extern "C" cWorldSound* __11cWorldSound(void* p, int a);
extern "C" void cBXNash_initTheAIFwdDiffCache(void* p);
extern "C" void cSectionMan_activate(void* p, int id, int a);
extern "C" int cSectionMan_isLoaded(void* p, int id);
extern "C" void cSectionMan_update(void* p);
void cWorld_update(void* p);
extern "C" void THREAD_yield(int a);
// PORT: cCameraTriggerMan_loadTriggersIfNotStreaming__FPv also receives the path string (passed through in $5)
void cCameraTriggerMan_loadTriggersIfNotStreaming_F6B0(void* p, const char* s) __asm__("cCameraTriggerMan_loadTriggersIfNotStreaming__FPv");
extern "C" void cFogParticleMan_cFogParticleMan(void* p);
cLensFxMan* cLensFxMan_construct();
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
 * Opens "data/worlds/" + name + ".big", where the name is the location's +0x44 string when cBENewRaceInterface_bIsBamLocation is set,
 * otherwise its +0x34 string. It creates the sky box, the section manager, the surface property cache and the
 * world sound. It then activates the location's start section and updates the section manager until that
 * section is in. It also passes the track's ".cam" file to cCameraTriggerMan_loadTriggersIfNotStreaming, and creates the fog particle
 * manager ("cPS2FogParticleMan") and the lens effects manager.
 */
void cGame::loadTrack()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBXString name;
    name.Str = cBXString_EmptyString;
    if (cBENewRaceInterface_bIsBamLocation != 0) {
        cBXString worldPath;
        cBXString locName;
        cBXString_operatorPlus2(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x44), &locName));
        cBXString_operatorE(&name, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    } else {
        cBXString worldPath;
        cBXString locName;
        cBXString_operatorPlus2(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x34), &locName));
        cBXString_operatorE(&name, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    }
    {
        cBXString path;
        cBXString_operatorPlus1(&path, &name, str_big_b);
        cBigFile_open(mpTrackBigFile, path.Str, 0);
        cBXString__cBXString(&path, 2);
    }
    sTbl_F6B0*& tbl = (sTbl_F6B0*)SurfPropCache;
    char*& ent = (char*)FogMan;
    cWorld_load(World, name.Str, BXNash);
    SkyBox = cSkyBox_cSkyBox(cMemMan_alloc(0x18, str_SkyBox, 0, 0));
    SectionMan = __11cSectionMani(cMemMan_alloc(468, str_SectionMan, 0, 0), ((char*)this));
    tbl = new (str_SurfPropCache, 0, 0) sTbl_F6B0;
    func_002C2F18(SurfPropCache);
    WorldSound = __11cWorldSound(cMemMan_alloc(592, str_cWorldSound, 0, 0), 0);
    cBXNash_initTheAIFwdDiffCache(BXNash);
    cBXString dir;
    dir.Str = cBXString_EmptyString;
    World->m_pWorldCache->m_StreamMan.m_bLimitBandWidth = 0;
    {
        cBXString worldPath;
        cBXString locName;
        cBXString_operatorPlus2(&worldPath, str_data_worlds_c, (cBXString_cBXString2(&locName, cBENewRaceInterface_getLocation(iface) + 0x34), &locName));
        cBXString_operatorE(&dir, &worldPath);
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&locName, 2);
    }
    int id = *(int*)cBENewRaceInterface_getLocation(iface);
    cSectionMan_activate(SectionMan, id, 0);
    SectionMan->m_bTriggerSkySwap = 1;
    while (!cSectionMan_isLoaded(SectionMan, id)) {
        cSectionMan_update(SectionMan);
        cWorld_update(World);
        THREAD_yield(0);
    }
    World->m_pWorldCache->m_StreamMan.m_bLimitBandWidth = 1;
    {
        cBXString worldPath;
        cBXString camBasePath;
        cCameraTriggerMan_loadTriggersIfNotStreaming_F6B0(((char*)&gCamTriggerMan), (cBXString_operatorPlus2(&camBasePath, str_bar_b, &dir), cBXString_operatorPlus1(&worldPath, &camBasePath, str_cam), worldPath.Str));
        cBXString__cBXString(&worldPath, 2);
        cBXString__cBXString(&camBasePath, 2);
    }
    cPS2FogParticleMan* obj = (cPS2FogParticleMan*)cMemMan_alloc(12304, str_cPS2FogParticleMan, 0, 0);
    cFogParticleMan_cFogParticleMan(obj);
    obj->mVtbl = cPS2FogParticleMan_vtbl;
    new ((char*)&obj->mSprites[0]) sElemFF_F6B0[256];
    ent = ((char*)obj);
    LensFxMan = cLensFxMan_construct();
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
extern "C" void _$_10cInputSync(void* p, int flags);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
void cSurfPropCache_purge(void* p);
void operator_delete(int* p);
extern "C" void cPathSys_purgePaths(void* p);
extern "C" void cBXNash_purgeTheAIFwdDiffCache(void* p);
extern "C" void _$_8cMenuSys(void* p, int flags);
extern "C" void cBucketMan_mergeBucket(void* mgr, int a);
extern "C" void cBucketMan_purge(void* mgr);
extern "C" void _$_11cCrowdMan2d(void* p, int flags);
extern "C" void _$_11cCrowdMan3d(void* p, int flags);
extern "C" void _$_17cMultiParticleMan(void* p, int flags);
extern "C" void cPlaybackMan__cPlaybackMan(void* p, int flags);
extern "C" void _$_12cInstanceMan(void* p, int flags);
extern "C" void cRenderStateMan_purge(void);
extern "C" void cGameViewMan__cGameViewMan(void* p, int flags);
extern "C" void cReplay__cReplay(void* p, int flags);
extern "C" void _$_11cSectionMan(void* p, int flags);
extern "C" void cFXLoader__cFXLoader(void* p, int flags);
extern "C" void _$_14cWorldLightMan(void* p, int flags);
extern "C" void cWorld__cWorld(void* p, int flags);
extern "C" void _$_13cOcclusionMan(void* p, int flags);
struct cBigFile;
void cBigFile__cBigFile(cBigFile* self, int flags);
extern "C" void _$_5cFont(void* p, int flags);
extern "C" void cGraphicsMan_PurgeTextures(void* p);
extern "C" void cIrradianceDataBase_Purge(void* p);
extern "C" void _$_11cWorldSound(void* p, int flags);
extern "C" void cWorldTriggerManager_Reset(void* a);
extern "C" void cSSXAudio_InGameDeInit(void* p);
extern "C" void cSSXAudio_InGamePurge(void* p);
extern "C" void cSSXScriptEngine_Purge(void* p);
extern "C" void cGarbageMan_Purge(void);

extern int cPauseState_StackIdx;
extern char gPaths[];
extern char cIrradianceDataBase_TheDataBase[];
// The unit declares gBucketMan later as sGp2898 (8 bytes); bind a small-data alias here.
extern char gBucketMan_gl __asm__("gBucketMan");

// g++ 2.95 vtable entry: this-adjust, index, function.
struct sVEnt_FA98 {
    short delta;
    short index;
    void (*fn)(void*, int);
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
        e->fn(vthis(p, *e), 3);
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

// Pointer array member with an inline indexer.
struct sPtrArr_FA98 {
    cRider* e[1];
    cRider* at(int i) { return e[i]; }
};

static inline cRider* GetRider_FA98(int i)
{
    return ((sPtrArr_FA98*)(&SSXApp->Game->AI->Riders[0]))->at(i);
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
 * Sets the BE state to 0. For a ranked network game it copies each rider's stats (its metrix's Totals; host order
 * by bIsHost) into cBENetworkInterface_networkOpt's MyStats and OpponentStats and submits the network interface. Then:
 * camera-trigger cleanup, GrMan slot 19, the FE async manager (cFEAsyncManager_FlushFEAsyncTextures), every script
 * force-ended; deletes UI and InputSync, returns the cut-scene transitions to regular mode, purges and frees
 * SurfPropCache, deletes SkyBox, FogMan and LensFxMan, purges the paths (cPathSys_purgePaths) and BXNash's AI cache,
 * deletes MenuSys, merges bucket 1 of the bucket manager D_004A5988, then deletes WScriptMan, CrowdMan2d/3d,
 * MultiParticleMan, PlaybackMan and InstanceMan, purges the camera triggers and render states, and deletes ViewMan,
 * AI, Replay, StreamPause, SectionMan, FXLoader, WorldLightMan, World, OcclusionMan, the track BigFile,
 * LoadIndicator (freed only), DebugFont and BXNash. Finally GrMan slots 113 and 50 (the box texture mBoxTextureID),
 * cGraphicsMan_PurgeTextures and cIrradianceDataBase_Purge, SSXApp->Game = 0, WorldSound deleted, the world triggers reset, the in-game
 * audio de-initialised and purged, the script engine purged, and cPauseState_StackIdx and the word at cAppMan_TheAppMan + 0x34
 * cleared.
 */
void cGame::purge()
{
    cBE_setState(0);
    cBEInterface* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cBENetworkInterface_networkOpt.bIsRankedGame != 0) {
        int which = cBENetworkInterface_networkOpt.bIsHost;
        int other = which != 0;
        cBENetworkInterface_networkOpt.MyStats = GetRider_FA98(which == 0)->mMetrix->Totals;
        cBENetworkInterface_networkOpt.OpponentStats = GetRider_FA98(other)->mMetrix->Totals;
        iface->submit();
    }
    cCameraTriggerMan_cleanupOnExit(((char*)&gCamTriggerMan));
    {
        cPS2GraphicsMan* be = GrMan;
        be->FlushRenderer();
    }
    cFEAsyncManager_FlushFEAsyncTextures(SSXApp->mFEAsyncManager);
    cSSXScriptEngine_ForceEndScriptsAll(ScriptEngine);
    if (UI != 0) cUIEngine__cUIEngine(UI, 3);
    delete InputSync;
    cCutSceneTransitionMan_ForceToRegularMode(CutSceneTransitionMan);
    cSurfPropCache_purge(SurfPropCache);
    operator_delete((int*)SurfPropCache);
    VDelete_FA98((char*)SkyBox, 0xC, 8);
    VDelete_FA98((char*)FogMan, 0x4, 8);
    VDelete_FA98((char*)LensFxMan, 25756, 8);
    cPathSys_purgePaths(gPaths);
    cBXNash_purgeTheAIFwdDiffCache(BXNash);
    delete MenuSys;
    void* bm = &gBucketMan_gl;
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purge(bm);
    VDelete_FA98((char*)WScriptMan, 1172, 8);
    delete CrowdMan2d;
    delete CrowdMan3d;
    delete MultiParticleMan;
    if (PlaybackMan != 0) cPlaybackMan__cPlaybackMan(PlaybackMan, 3);
    delete InstanceMan;
    cCameraTriggerMan_purge(((char*)&gCamTriggerMan));
    cRenderStateMan_purge();
    if (ViewMan != 0) cGameViewMan__cGameViewMan(ViewMan, 3);
    VDelete_FA98((char*)AI, 204, 8);
    if (Replay != 0) cReplay__cReplay(Replay, 3);
    VDelete_FA98((char*)StreamPause, 0x0, 8);
    delete SectionMan;
    if (FXLoader != 0) cFXLoader__cFXLoader(FXLoader, 3);
    delete WorldLightMan;
    if (World != 0) cWorld__cWorld(World, 3);
    delete OcclusionMan;
    if (mpTrackBigFile != 0) cBigFile__cBigFile(mpTrackBigFile, 3);
    operator_delete((int*)LoadIndicator);
    delete DebugFont;
    {
        sObj20_FA98* nash = (sObj20_FA98*)BXNash;
        if (nash != 0) {
            nash->~sObj20_FA98();
            operator_delete((int*)nash);
        }
    }
    {
        cPS2GraphicsMan* be = GrMan;
        be->PurgeCharModels();
    }
    {
        cPS2GraphicsMan* be = GrMan;
        be->RemoveTexture(mBoxTextureID);
    }
    cGraphicsMan_PurgeTextures(GrMan);
    cIrradianceDataBase_Purge(cIrradianceDataBase_TheDataBase);
    SSXApp->Game = 0;
    delete WorldSound;
    cWorldTriggerManager_Reset(gWorldTriggerManager);
    cSSXAudio_InGameDeInit(gSSXAudio);
    cSSXAudio_InGamePurge(gSSXAudio);
    cSSXScriptEngine_Purge(ScriptEngine);
    cGarbageMan_Purge();
    cPauseState_StackIdx = 0;
    cAppMan_TheAppMan->mLock30Fps = 0;
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
    virtual void Reset();
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
    virtual void ClearPatches();
};
extern "C" void cFELocale_unloadFE(void* a);
extern "C" void cBXLoadIndicator_Init(void* a);
void cSSXScriptEngine_Init(void* a);
extern "C" void cGraphicsMan_UploadTextures(void* a);
extern "C" void cWorld_init(void* a);
extern "C" void cWorldTriggerManager_Reset(void* a);
extern "C" void cSSXAudio_InGameInit(void* a);
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
void cGame::gainFocus()
{
    cFELocale_unloadFE(SSXApp->FELocale);
    void* loadIndicator = LoadIndicator;
    cOVStateManager_mpPDATemplate = 0;
    cBXLoadIndicator_Init(loadIndicator);
    cSSXScriptEngine_Init(ScriptEngine);
    cBezierMan_TheBezierMan->ClearPatches();
    cBezierMan_TheBezierMan->Reset();
    cGraphicsMan_UploadTextures(GrMan);
    cWorld_init(World);
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
extern cWScriptMan* cWScriptMan_TheWScriptMan;
// The unit declares gBucketMan later as sGp2898 (8 bytes); bind a small-data alias here.
extern char gBucketMan_bm __asm__("gBucketMan");

/**
 * Resets the world for a new run: playback, missions, update buckets 1 and 8, the AI focus, avalanches and
 * the map.
 *
 * It then reruns the global world scripts and sets the missions up again.
 */
void cGame::resetWorld()
{
    void* bm = &gBucketMan_bm;
    cPlaybackMan_restart(PlaybackMan);
    cWScriptMan_initMissions(WScriptMan);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    cFocusMan_reset(AI->FocusMan);
    cAvalanche_reset();
    cWorld_resetMap(World);
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(WScriptMan);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_partialRestart);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_snapAll(void* self);
extern "C" void cReplay_reset(void* self);
extern "C" void cSSXScriptEngine_Restart(void* self);
extern "C" void cCamera_onRestart(void);
extern "C" void cCrowdMan2d_restart(void* self);
extern "C" void cCrowdMan3d_clearAllBlocks(void* self);
extern "C" void cMultiParticleMan_reset(void* self);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" void cSSXAudio_PartialRestart(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
int GetHashValue32(char* s);
extern char str_hud_c[];


/**
 * Restarts the race without reloading: resets playback, missions, buckets, riders (AI slot 5 with 3) and
 * avalanches.
 *
 * It also snaps the views and resets the map, the replay, the script engine, the crowds and the particles. It
 * tells the UI state found by str_hud_c's hash about it (slot 24 with 7), and reruns the global world
 * scripts and the mission setup. Last, it calls cSSXAudio_PartialRestart on the audio system (On Tour PartialRestart).
 */
void cGame::partialRestart()
{
    void* bm = &gBucketMan_bm;
    cPlaybackMan_restart(PlaybackMan);
    cWScriptMan_initMissions(WScriptMan);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    cAI* gameAI = AI;
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)gameAI->mVtbl;
    vt[5].fn(vthis(gameAI, vt[5]), 3);
    cAvalanche_reset();
    cGameViewMan_snapAll(ViewMan);
    cWorld_resetMap(World);
    cReplay_reset(Replay);
    cSSXScriptEngine_Restart(ScriptEngine);
    cCamera_onRestart();
    cCrowdMan2d_restart(CrowdMan2d);
    cCrowdMan3d_clearAllBlocks(CrowdMan3d);
    cMultiParticleMan_reset(MultiParticleMan);
    void* list = (char*)&UI->mStateStack;
    cUIState* hudState = (cUIState*)((char*)cUIStateStack_findState(list, GetHashValue32(str_hud_c)));
    if (((char*)hudState) != 0) {
        hudState->handleEvent(7, 0);
    }
    cWScriptMan_runGlobalScripts(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(WScriptMan);
    cSSXAudio_PartialRestart(cAudioInterface_bxAudio());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/gameload", cGame_restart);
#ifdef SKIP_ASM
extern "C" void cGame_partialRestart(cGame* self);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" void cGame_setState(cGame* self, int a, int b, int refresh);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_RestartEvent(void* mgr);

// Inline copies of cAI::getNumRiders() / cAI::getRider(int) (EA's header has them inline: On Tour inlines both into
// several all_in_one units; retail's out-of-line copies are cAI_getNumRiders / cAI_getRider).
static inline int aiNumRiders(cAI* ai)
{
    return ai->NumberOfRidersAllocated;
}

static inline cRider* aiRider(cAI* ai, int i)
{
    return ai->Riders[i];
}

/**
 * Restarts the current event from the start.
 *
 * Runs cGame_partialRestart() and cGameModeMan_restartHeat(), clears every allocated rider's BoostPowerBar (which the
 * game-mode code sets to 0.7, 0.35 or 0 by the player's place), switches the game to state 1 at once
 * (cGame_setState(self, 1, 2, 1)), and calls cSSXAudio_RestartEvent() on the audio system.
 */
void cGame::restart()
{
    partialRestart();
    cGameModeMan_restartHeat(SSXApp->GameModeMan);
    int i;
    for (i = 0; i < aiNumRiders(AI); i++) {
        aiRider(AI, i)->BoostPowerBar = 0.0f;
    }
    setState(1, 2, 1);
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
void cGame::sectionLoaded()
{
    cAI* gameAI = AI;
    if (gameAI != 0) {
        cFocusMan* focusMan = gameAI->FocusMan;
        if (((void*)focusMan) != 0) {
            focusMan->LastUpdateTick = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_sectionUnloading);
#ifdef SKIP_ASM
extern "C" void cRailManNode_unloadSection(int kind);
extern "C" void cCrowdMan2d_releaseSectionQuads(void* self, int id);
extern "C" void cCrowdMan3d_removeSectionMembers(void* self, int arg);
extern "C" void cMultiParticleMan_unloadSection(void* self, int arg);
extern "C" void cBucketMan_purgeBucket1(void* self, int index, int key);
extern "C" void cFocusMan_purgesection(void* p, int arg);
extern "C" void cWScriptMan_unloadSection(void* self, int i);
extern void* TheCrowdMan2d;
extern void* TheCrowdMan3d;
extern void* cMultiParticleMan_TheMultiParticleMan;

/**
 * Removes everything that belongs to a world section that is about to unload.
 *
 * The section's crowds, particles and update-bucket entries (buckets 1 and 8) go first. It then clears the AI
 * focus manager for the section and for 0x80 (cFocusMan_purgesection), and unloads the section's world scripts.
 *
 * @param id  the section id (On Tour sectionId)
 */
void cGame::sectionUnloading(int id)
{
    void* bm = &gBucketMan_bm;
    cRailManNode_unloadSection(id);
    cCrowdMan2d_releaseSectionQuads(TheCrowdMan2d, id);
    cCrowdMan3d_removeSectionMembers(TheCrowdMan3d, id);
    cMultiParticleMan_unloadSection(cMultiParticleMan_TheMultiParticleMan, id);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket1(bm, 1, id);
    cBucketMan_purgeBucket1(bm, 8, id);
    cAI* gameAI = AI;
    if (gameAI != 0) {
        void* focusMan = gameAI->FocusMan;
        if (focusMan != 0) {
            cFocusMan_purgesection(focusMan, id);
            cFocusMan_purgesection(AI->FocusMan, 0x80);
        }
    }
    cWScriptMan_unloadSection(WScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_loadWScriptManSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: cWScriptMan_loadSection is declared with one argument; it forwards the section id in $a1 to
// cWScriptMan_allocateGlobalTable
void* cWScriptMan_loadSection_2(void* self, int sectionId) __asm__("cWScriptMan_loadSection__FPv");



struct sWorldCache_0430 {
    int f0;
    cWScriptCache* scripts;
};


static inline bool isSectionLoaded_0430(cWScriptCache* c, int sectionId)
{
    return (c->mpCache)[sectionId].mState >= 2 && ((void*)(c->mpCache)[sectionId].mpScriptHeader) != 0;
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
void cGame::loadWScriptManSection(int sectionId)
{
    if (isSectionLoaded_0430(cWorld_m_pTheWorld->m_pWScriptCache, sectionId)) {
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
extern "C" void cVoipMan_disconnect(void* p);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void* cPreFELoadScreen_cPreFELoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern void* VoipMan;
extern char str_PreGameLoadScreen[];
extern char str_PreFELoad[];

/**
 * Leaves the in-game module and queues the next app module.
 *
 * Stops the auto replay, sets game state 6, shuts down the online game-comm object (cGameComm_gameExit) and the
 * DirtySock object in VoipMan when they exist. Despite the parameter name, a nonzero @p toFrontEnd queues a
 * cPreGameLoadScreen (used after choosing a new event in-game), and 0 queues the "PreFELoad" module, which
 * probably loads the front end.
 *
 * @param toFrontEnd  nonzero for the pre-game load screen, 0 for the "PreFELoad" module
 */
void cGame::exit(int toFrontEnd)
{
    cReplay_stopAutoReplay(Replay);
    setState(6, 0, 0);
    if (cGameComm_mGameComm != 0) {
        cGameComm_gameExit(cGameComm_mGameComm);
    }
    if (VoipMan != 0) {
        cVoipMan_disconnect(VoipMan);
    }
    // ORIGINAL-PTR: the retail symbol cAppMan_setNextModule__FP7cAppManUi mangles the module parameter as unsigned
    // int, so the original converted the new module's pointer to it too
    if (toFrontEnd != 0) {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_PreGameLoadScreen, 0x100, 0)));
    } else {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreFELoadScreen_cPreFELoadScreen(cMemMan_alloc(8, str_PreFELoad, 0x100, 0)));
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_startReplay);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cReplay_startReplay(void* a, int b, int c, int d);
void cMenuSys_activate(void* a, int b);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];


/**
 * Starts replay playback, the mirror of cGame_stopReplay.
 *
 * Pushes pause state 8 (cPauseState_Stack[++cPauseState_StackIdx] = 8) and sets cPauseState_Flags to that state's flags
 * (cPauseState_FlagsTable[8]), starts the replay (+0x28) with cReplay_startReplay(replay, 0, 0, 0) and calls
 * cMenuSys_activate (On Tour cMenuSys::activate) with 0 on the menu system (+0xA4). The pause stack index is not checked.
 */
void cGame::startReplay()
{
    cPauseState_Stack[++cPauseState_StackIdx] = 8;
    cPauseState_Flags = cPauseState_FlagsTable[8];
    cReplay_startReplay(((void*)Replay), 0, 0, 0);
    cMenuSys_activate(((void*)MenuSys), 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_stopReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_stopReplay(void* a);
void cMenuSys_activate(void* a, int b);
extern "C" void cMenuSys_popMenu(void* a);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];

/**
 * Ends replay playback and returns to the pause menu state that was active before it.
 *
 * Stops the replay (+0x28), calls cMenuSys_activate (On Tour cMenuSys::activate) with 1 and cMenuSys_popMenu
 * (cMenuSys::popMenu) on the menu system (+0xA4), then pops the pause stack (cPauseState_StackIdx index into cPauseState_Stack)
 * and restores cPauseState_Flags from cPauseState_FlagsTable. The stack index is not checked (On Tour asserts it is > 0).
 */
void cGame::stopReplay()
{
    cReplay_stopReplay(Replay);
    cMenuSys_activate(MenuSys, 1);
    cMenuSys_popMenu(MenuSys);
    cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_renderBuckets);
#ifdef SKIP_ASM
extern "C" void cBucketMan_renderBucket(void* mgr, int id);
extern cBucketMan gBucketMan; // target: raw $gp+0x2898 (splat doesn't symbolize it)
extern int gRenderBuckets[];

/**
 * Renders the six game buckets listed in gRenderBuckets, in order, with cBucketMan_renderBucket on gBucketMan.
 *
 * cGame_render calls it in each of its three render paths.
 */
extern "C" void cGame_renderBuckets(void)
{
    int i;
    cBucketMan* mgr = &gBucketMan;
    for (i = 0; i < 6; i++) {
        cBucketMan_renderBucket(mgr, gRenderBuckets[i]);
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
cTransitionMan* cGame::getTransitionMan(int i)
{
    return TransitionMan[i];
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
cSplashManager* cGame::getSplashManager(int i)
{
    return SplashManager[i];
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
extern "C" void cSSXAudio_StopRiderLoops(void* p);
// PORT: the unit defines cGame_activatePauseMenu(char* self); this caller passes a second argument (a1 = 0).
extern "C" int cGame_activatePauseMenu_pi(void* self, int a) __asm__("cGame_activatePauseMenu");
// cFadeScreen_update returns nothing (its body has no return value); the unit declares it void*.
extern "C" void cFadeScreen_update_v(void* self) __asm__("cFadeScreen_update");
extern "C" int cOVStateManager_allowPauseMenu(void);
extern "C" void cPadCache_clearAllVibrations(void* p);
extern "C" void cOVStateManager_triggerPauseMenu(void);
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
extern "C" void cCrowdMan2d_update(void* p);
extern "C" void cCrowdMan3d_update(void* p);
extern "C" void cMultiParticleMan_update(void* p);
extern "C" void cWorldSound_TriggerWorldAudio(void* p, void* q, int i);
void cInputSync_postupdate(void* p);
extern "C" void NetConnIdle(void);
extern "C" void cFEAsyncManager_Update(void* p);
extern "C" void updateTimedMessages(void);
extern "C" void cBXLoadIndicator_Update(void* p);

extern void* PadCache;
extern unsigned int cOVState_MESSAGECENTER_openRequest;
extern char str_90bc_fail_b[];


struct sVEntryI_2306B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObjC_2306B8 {
    char pad0[0x8];
    sVEntryI_2306B8* vtbl;
};



static inline void pushState_2306B8(int state)
{
    cPauseState_Stack[++cPauseState_StackIdx] = state;
    cPauseState_Flags = cPauseState_FlagsTable[state];
}

static inline int inRace_2306B8(int s)
{
    if (s != 0 && s < 10) {
        return 1;
    }
    return 0;
}

static inline int hasEvents_2306B8(char* vlist)
{
    tMissionMenuQueue* list = (tMissionMenuQueue*)vlist;
    return list->Current != list->Next;
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

// EA's inline accessors (the preview's cGameViewMan::getNumGameViews / getGameView, cAI::getNumRiders / getRider):
// reading through them reloads the manager pointer in the loops below.
static inline unsigned int NumViews_2306B8(cGameViewMan* viewMan)
{
    return viewMan->m_uiNumView;
}

static inline cCamera* View_2306B8(cGameViewMan* viewMan, unsigned int i)
{
    return viewMan->PlayerCamera[i];
}

static inline int NumRiders_2306B8(cAI* ai)
{
    return ai->NumberOfRidersAllocated;
}

static inline cRider* Rider_2306B8(cAI* ai, int i)
{
    return ai->Riders[i];
}

static inline int canUpdate_2306B8(cGame* self)
{
    if ((int)SSXApp->LoadingModule != 0) {
        self->unk_08 = 1;
        return 0;
    }
    return 1;
}

static inline int updatePause_2306B8(cGame* self)
{
    if (cGameViewMan_isStreamReady(((char*)self->ViewMan)) == 0 || self->SectionMan->m_bStreamReady == 0) {
        self->World->m_pWorldCache->m_StreamMan.m_bLimitBandWidth = 0;
        cWorld_update(((char*)self->World));
        cGameViewMan_updateAll(((char*)self->ViewMan));
        if (self->SectionMan->m_bStreamReady == 0) {
            self->updateState();
            cSectionMan_update(((char*)self->SectionMan));
        }
        if (self->IsStreamPaused == 0 && cGameViewMan_isStreamReady(((char*)self->ViewMan)) == 0) {
            self->m_bFlushStreamer = 1;
        }
        self->IsStreamPaused = 1;
        if (self->m_bFlushStreamer != 0) {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            ps2GraphicsMan->FlushRenderer();
            cGameViewMan_updateStreamFocus(((char*)self->ViewMan));
            cWorldCache_flushInActivePages(self->World->m_pWorldCache);
            self->m_bFlushStreamer = 0;
        }
        return 0;
    }
    self->World->m_pWorldCache->m_StreamMan.m_bLimitBandWidth = 1;
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cWorld_update(((char*)self->World));
    cSectionMan_update(((char*)self->SectionMan));
    return 1;
}

static inline int updateState_2306B8(cGame* self)
{
    if (cMenuSys_isActive(((char*)self->MenuSys)) == 0) {
        cWScriptMan* q = self->WScriptMan;
        char* list = (char*)&q->MissionMenuQueue;
        if (q->MissionMenuQueue.Current != q->MissionMenuQueue.Next) {
            int* e = tMissionMenuQueue_getEntry(list);
            if (e[1] == 8) {
                cOVStateManager_triggerOverlay(0x1D);
                pushState_2306B8(3);
                cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 4, 0, 1.0f);
                return 0;
            }
            if (e[1] == 9) {
                cWScriptMan_doMissionCmd(((char*)self->WScriptMan), e[0], 5, e[2]);
                if (cUIStateStack_findState(&self->UI->mStateStack, GetHashValue32(str_90bc_fail_b)) == 0) {
                    cOVStateManager_triggerOverlay(0x1E);
                    pushState_2306B8(3);
                    cUIEngine_update(((char*)self->UI));
                } else {
                    tMissionMenuQueue_getNextEntry(&cWScriptMan_TheWScriptMan->MissionMenuQueue);
                }
                return 0;
            }
        }
    }
    if (~cOVState_MESSAGECENTER_openRequest != 0) {
        cOVStateManager_triggerOverlay(0x22);
        pushState_2306B8(12);
        cOVState_MESSAGECENTER_openRequest = 0xFFFFFFFF;
        cSSXAudio_StopRiderLoops(cAudioInterface_bxAudio());
        return 0;
    }
    if (cMenuSys_isActive(((char*)self->MenuSys)) != 0) {
        cUIEngine_update(((char*)self->UI));
        cMenuSys_input(((char*)self->MenuSys));
        return 0;
    }
    if (cGame_activatePauseMenu_pi(self, 0) != 0 && !inRace_2306B8(*((int*)self->Replay))
        && notPlaying_2306B8(((char*)self->PlaybackMan)) && cOVStateManager_allowPauseMenu() != 0) {
        cPadCache_clearAllVibrations(PadCache);
        cOVStateManager_triggerPauseMenu();
        pushState_2306B8(2);
        {
            sObjC_2306B8* o = ((sObjC_2306B8*)((cUIEngine*)self->UI)->mpInput);
            sVEntryI_2306B8* vt = o->vtbl;
            vt[44].fn(vthis(o, vt[44]), (*(unsigned char*)&self->PauseOwner));
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
void cGame::update()
{
    if (ViewMan->m_uiNumView >= 2) {
        cAppMan_TheAppMan->mLock30Fps = 1;
    } else {
        cAppMan_TheAppMan->mLock30Fps = cRenderStateMan_SkyBoxRotation(6) > 0.9f;
    }
    if (cGameComm_mGameComm != 0) {
        cGameComm_update(cGameComm_mGameComm);
    }
    cSSXScriptEngine_UpdateLoad(ScriptEngine);
    cFadeScreen_update_v((this));
    if (canUpdate_2306B8((this)) && updatePause_2306B8((this))) {
        if ((char*)InputSync != 0) {
            cInputSync_preupdate((char*)InputSync);
        }
        cOVStateManager_update();
        if (updateState_2306B8((this))) {
            int skip;
            if ((char*)InputSync == 0 || cInputSync_aiinputready((char*)InputSync) != 0) {
                if (mCurrentStateID == kGFSM_Gate) {
                    updateState();
                    cBucketMan_mergeBucket(&gBucketMan, 1);
                } else {
                    updateState();
                }
                if (!paused_2306B8()) {
                    cReplay_update((char*)Replay);
                }
                if (cReplay_isPauseAI((char*)Replay) == 0 && !(cPauseState_Flags & 8)) {
                    cSSXScriptEngine* w = ScriptEngine;
                    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)w->mVtbl;
                    vt[5].fn(vthis(w, vt[5]));
                }
            }
            skip = 0;
            if ((cPauseState_Flags & 1) || cReplay_isPauseAI((char*)Replay) != 0) {
                skip = 1;
            }
            if (!skip) {
                if (((char*)InputSync != 0 && cInputSync_aiupdate((char*)InputSync) == 0)
                    || cPlaybackMan_isPauseAI((char*)PlaybackMan) != 0) {
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
                    cAI* gameAI = AI;
                    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)gameAI->mVtbl;
                    vt[3].fn(vthis(gameAI, vt[3]));
                }
                if (!(cPauseState_Flags & 0x10)) {
                    cBucketMan_updateBucket(&gBucketMan, 2);
                }
                if (!(cPauseState_Flags & 4)) {
                    cBucketMan_updateBucket(&gBucketMan, 3);
                }
                cWScriptMan_update((char*)WScriptMan);
                cCrowdMan2d_update((char*)CrowdMan2d);
                cCrowdMan3d_update((char*)CrowdMan3d);
                cMultiParticleMan_update((char*)MultiParticleMan);
                cBucketMan_mergeBucket(&gBucketMan, 1);
            } else {
                cBucketMan_updateBucket(&gBucketMan, 5);
                if (!(cPauseState_Flags & 4)) {
                    cBucketMan_updateBucket(&gBucketMan, 3);
                }
            }
            cBucketMan_updateBucket(&gBucketMan, 4);
            {
                int replayState = *((int*)(char*)Replay);
                if (!inRace_2306B8(replayState) || notMode3_2306B8(replayState)) {
                    cGameViewMan_updateAll((char*)ViewMan);
                }
            }
            (mUpdateTick)++;
            {
                unsigned int i;
                for (i = 0; i < NumViews_2306B8(ViewMan); i++) {
                    cWorldSound_TriggerWorldAudio((char*)WorldSound, &View_2306B8(ViewMan, i)->Pose, i);
                }
            }
            {
                int i;
                for (i = 0; i < NumRiders_2306B8(AI); i++) {
                    cRider* rider = Rider_2306B8(AI, i);
                    // cBeing slot 19: vibrationUpdate
                    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_AT(rider, sizeof(cRiderState));
                    vt[19].fn(vthis(rider, vt[19]));
                }
            }
        }
        if ((char*)InputSync != 0) {
            cInputSync_postupdate((char*)InputSync);
        }
    }
    if (cCommSystem_mCommSystem != 0) {
        NetConnIdle();
    }
    if ((void*)SSXApp->mFEAsyncManager != 0) {
        cFEAsyncManager_Update(SSXApp->mFEAsyncManager);
    }
    updateTimedMessages();
    cBXLoadIndicator_Update((char*)LoadIndicator);
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
extern "C" int cFadeScreen_loseFocus(cFadeScreen* self);
void cSSXScriptEngine_DeInit(void* self);
void cBXLoadIndicator_DeInit(void* self);

// tRGB with On Tour's tRGB(float red, float green, float blue) constructor, for the temporary colour.
struct tRGBInit : tRGB {
    tRGBInit(float r, float g, float b) { R = r; G = g; B = b; }
};


/**
 * Lets the game give up focus, once the stream pause (its virtual slot 3) and the screen fade allow it.
 *
 * On success it sets a black screen clear colour, sets the graphics manager's view count (cGraphicsMan::mNumViews)
 * to 1, turns the screen effects off, and calls helpers on the script engine and the load indicator.
 *
 * @return true when focus was given up, false while the stream pause or the fade is still busy
 */
bool cGame::loseFocus()
{
    void* obj = StreamPause;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (vt[3].fn(vthis(obj, vt[3])) == 0) {
        return false;
    }
    if (((cFadeScreen*)this)->cFadeScreen::loseFocus() == 0) {
        return false;
    }
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    GrMan->cGraphicsMan::mNumViews = 1;
    cDepthFog_m_Enable = 0;
    cScreenTint_m_Enable = 0;
    cLightGlow_m_Enable = 0;
    cSSXScriptEngine_DeInit(ScriptEngine);
    cBXLoadIndicator_DeInit(LoadIndicator);
    return true;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_updateState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGameViewMan_updateViewState(void* self);
extern int cPauseState_Flags;

/**
 * Runs the game-flow state machine: updates the current state, drives a preload and switches to a requested state.
 *
 * Returns at once in an offline game (cBENetworkInterface_networkOpt.bIsNetworkGame 0) while ControllerUnplugged
 * (+0x224) is set, or while cPauseState_Flags has bit 0x20. Calls the current state's update() (virtual slot 6) unless
 * it is the gate state (kGFSM_Gate) with a switch pending. A pending preload id (mPendingPreloadStateID, +0x208) picks
 * the member state for that id (eGFState kGFSM_EventStart .. kGFSM_ReplayStart: +0xB0 ... +0x1EC) as mPreloadingState
 * (+0x204), moves the id to mPreloadStateID (+0x20C) and calls its preloadInit() (slot 2); a preloading state gets
 * preloadUpdate() (slot 3) on every call.
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
void cGame::updateState()
{
    cGame* self = this;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0 && self->ControllerUnplugged != 0)
        return;
    if (cPauseState_Flags & 0x20)
        return;
    if (self->mCurrentState != 0 && (self->mCurrentStateID != kGFSM_Gate || self->mPendingStateID == 0))
        self->mCurrentState->update();
    if (self->mPendingPreloadStateID != 0) {
        switch (self->mPendingPreloadStateID) {
    case kGFSM_EventStart:
        self->mPreloadingState = (cGameState*)&self->mEventStartState;
        break;
    case kGFSM_PreEvent:
        self->mPreloadingState = (cGameState*)&self->mPreEventState;
        break;
    case kGFSM_Gate:
        self->mPreloadingState = (cGameState*)&self->mGateState;
        break;
    case kGFSM_Riding:
        self->mPreloadingState = (cGameState*)&self->mRidingState;
        break;
    case kGFSM_Finishline:
        self->mPreloadingState = (cGameState*)&self->mFinishlineState;
        break;
    case kGFSM_End:
        self->mPreloadingState = (cGameState*)&self->mEndState;
        break;
    case kGFSM_PostEvent:
        self->mPreloadingState = (cGameState*)&self->mPostEventState;
        break;
    case kGFSM_EndWait:
        self->mPreloadingState = (cGameState*)&self->mEndWaitState;
        break;
    case kGFSM_TransportWait:
        self->mPreloadingState = (cGameState*)&self->mTransportWaitState;
        break;
    case kGFSM_TrackStart:
        self->mPreloadingState = (cGameState*)&self->mTrackStartState;
        break;
    case kGFSM_SectionLoad:
        self->mPreloadingState = (cGameState*)&self->mSectionLoadState;
        break;
    case kGFSM_Podium:
        self->mPreloadingState = (cGameState*)&self->mPodiumState;
        break;
    case kGFSM_NextHeat:
        self->mPreloadingState = (cGameState*)&self->mNextHeatState;
        break;
    case kGFSM_UserSelect:
        self->mPreloadingState = (cGameState*)&self->mUserSelectState;
        break;
    case kGFSM_SessionTeleport:
        self->mPreloadingState = (cGameState*)&self->mSessionTeleportState;
        break;
    case kGFSM_ReplayStart:
        self->mPreloadingState = (cGameState*)&self->mReplayStartState;
        break;
        }
        self->mPreloadStateID = self->mPendingPreloadStateID;
        self->mPendingPreloadStateID = 0;
        self->mPreloadingState->preloadInit();
    }
    if (self->mPreloadingState != 0)
        self->mPreloadingState->preloadUpdate();
    int next = self->mPendingStateID;
    if (next == 0)
        return;
    int startupFlag = self->mPendingStateStartupFlag;
    if (self->mCurrentState != 0)
        self->mCurrentState->loseFocus();
    self->mPrevStateID = self->mCurrentStateID;
    self->mPrevState = self->mCurrentState;
    switch (next) {
    case kGFSM_EventStart:
        self->mCurrentState = (cGameState*)&self->mEventStartState;
        self->mCurrentStateID = 1;
        break;
    case kGFSM_PreEvent:
        self->mCurrentState = (cGameState*)&self->mPreEventState;
        self->mCurrentStateID = 2;
        break;
    case kGFSM_Gate:
        self->mCurrentState = (cGameState*)&self->mGateState;
        self->mCurrentStateID = 3;
        break;
    case kGFSM_Riding:
        self->mCurrentState = (cGameState*)&self->mRidingState;
        self->mCurrentStateID = 4;
        break;
    case kGFSM_Finishline:
        self->mCurrentState = (cGameState*)&self->mFinishlineState;
        self->mCurrentStateID = 5;
        break;
    case kGFSM_End:
        self->mCurrentState = (cGameState*)&self->mEndState;
        self->mCurrentStateID = 6;
        break;
    case kGFSM_PostEvent:
        self->mCurrentState = (cGameState*)&self->mPostEventState;
        self->mCurrentStateID = 7;
        break;
    case kGFSM_EndWait:
        self->mCurrentState = (cGameState*)&self->mEndWaitState;
        self->mCurrentStateID = 8;
        break;
    case kGFSM_TransportWait:
        self->mCurrentState = (cGameState*)&self->mTransportWaitState;
        self->mCurrentStateID = 9;
        break;
    case kGFSM_TrackStart:
        self->mCurrentState = (cGameState*)&self->mTrackStartState;
        self->mCurrentStateID = 10;
        break;
    case kGFSM_SectionLoad:
        self->mCurrentState = (cGameState*)&self->mSectionLoadState;
        self->mCurrentStateID = 11;
        break;
    case kGFSM_Podium:
        self->mCurrentState = (cGameState*)&self->mPodiumState;
        self->mCurrentStateID = 12;
        break;
    case kGFSM_NextHeat:
        self->mCurrentState = (cGameState*)&self->mNextHeatState;
        self->mCurrentStateID = 13;
        break;
    case kGFSM_UserSelect:
        self->mCurrentState = (cGameState*)&self->mUserSelectState;
        self->mCurrentStateID = 14;
        break;
    case kGFSM_SessionTeleport:
        self->mCurrentState = (cGameState*)&self->mSessionTeleportState;
        self->mCurrentStateID = 15;
        break;
    case kGFSM_ReplayStart:
        self->mCurrentState = (cGameState*)&self->mReplayStartState;
        self->mCurrentStateID = 16;
        break;
    }
    self->mPendingStateID = 0;
    self->mPendingStateStartupFlag = 0;
    if (self->mPreloadStateID == self->mCurrentStateID) {
        self->mPreloadStateID = 0;
        self->mPreloadingState = 0;
    }
    if (self->mCurrentState != 0) {
        cGameViewMan_updateViewState(((void*)self->ViewMan));
        self->mCurrentState->gainFocus(startupFlag);
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
void cGame::setState(int iNextState, int startupFlag, int refresh)
{
    mPendingStateID = iNextState;
    mPendingStateStartupFlag = startupFlag;
    if (refresh != 0) {
        updateState();
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
void cGame::setPreloadState(int val)
{
    mPendingPreloadStateID = val;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_cancelPreloadingState);
#ifdef SKIP_ASM
/**
 * Cancels the game's state preload.
 *
 * Clears mPendingPreloadStateID (+0x208); when a preload is running (mPreloadStateID, +0x20C), calls the preloading
 * state's preloadCancel (vtable slot 4 of mPreloadingState, +0x204) and clears mPreloadStateID and
 * mPreloadingState.
 */
void cGame::cancelPreloadingState()
{
    if (mPendingPreloadStateID != 0) {
        mPendingPreloadStateID = 0;
    }
    if (mPreloadStateID != 0) {
        mPreloadingState->preloadCancel();
        mPreloadStateID = 0;
        mPreloadingState = 0;
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_isPreloadedStateReady);
#ifdef SKIP_ASM

/**
 * Tells whether the state being preloaded is ready to run.
 *
 * Returns 0 while a preload request is still pending (mPendingPreloadStateID, +0x208), 1 when nothing is being
 * preloaded (mPreloadingState, +0x204), else that state's isPreloadComplete() (virtual slot 8).
 *
 * @return nonzero when ready
 */
int cGame::isPreloadedStateReady()
{
    if (mPendingPreloadStateID != 0) {
        return 0;
    }
    cGameState* preloadingState = mPreloadingState;
    if (((void*)preloadingState) == 0) {
        return 1;
    }
    return preloadingState->isPreloadComplete();
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
void cGame::doNextState()
{
    void* gameState = mCurrentState;
    if (gameState != 0) {
        cGameState_gotoNextState(gameState);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_writeToReplayFrame);
#ifdef SKIP_ASM
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
void cGame::writeToReplayFrame(cBxReplayFrame* stream)
{
    FlagManager->writeToReplayFrame(stream);
    SnowFall->writeToReplayFrame(stream);
    stream->write((const char*)&mCurrentStateID, 4);
    stream->write((const char*)&mPrevStateID, 4);
    stream->write((const char*)&mPendingStateID, 4);
    stream->write((const char*)&mPendingStateStartupFlag, 4);
    cGFEventStartState_writeToReplayFrame((char*)&mEventStartState, stream);
    cGFPreEventState_writeToReplayFrame_s((char*)&mPreEventState, stream);
    cGFGateState_writeToReplayFrame_s((char*)&mGateState, stream);
    cGFRidingState_writeToReplayFrame_s((char*)&mRidingState, stream);
    cGFFinishlineState_writeToReplayFrame((char*)&mFinishlineState, stream);
    cGFEndState_writeToReplayFrame_s((char*)&mEndState, stream);
    cGFPostEventState_writeToReplayFrame_s((char*)&mPostEventState, stream);
    cGFEndWaitState_writeToReplayFrame_s((char*)&mEndWaitState, stream);
    cGFTransportWaitState_writeToReplayFrame_s((char*)&mTransportWaitState, stream);
    cGFTrackStartState_writeToReplayFrame((char*)&mTrackStartState, stream);
    cGFSectionLoadState_writeToReplayFrame_s((char*)&mSectionLoadState, stream);
    cGFPodiumState_writeToReplayFrame_s((char*)&mPodiumState, stream);
    cGFNextHeatState_writeToReplayFrame((char*)&mNextHeatState, stream);
    cGFUserSelectState_writeToReplayFrame((char*)&mUserSelectState, stream);
    cGFSessionTeleportState_writeToReplayFrame_s((char*)&mSessionTeleportState, stream);
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

/**
 * Restores the game's flow state from a replay frame: the flag manager, snowfall, state ids and every game-flow
 * state.
 *
 * Loads FlagManager and SnowFall (their load virtuals), reads mCurrentStateID, mPrevStateID, mPendingStateID and
 * mPendingStateStartupFlag (4 bytes each), then each cGF*State in mStates (event start, pre-event, gate, riding, finish
 * line, end, post-event, end wait, transport wait, track start, section load, podium, next heat, user select, session
 * teleport; the state at +0x1EC is not read). mCurrentState and mPrevState are pointed at the state object for their id
 * (eGFState kGFSM_EventStart .. kGFSM_ReplayStart; kGFSM_Null is none, an unknown id becomes kGFSM_Null). The comment
 * above the definition calls it a save stream; it is a replay frame.
 *
 * @param self_  the game (this)
 * @param stream  the replay frame to read from (On Tour cReplayFramePtr*)
 */
// Game load state machine: restores the state ids and every state from a save stream.
void cGame::readFromReplayFrame(cBxReplayFrame* stream)
{
    cGame* self = this;
    self->FlagManager->readFromReplayFrame(stream);
    self->SnowFall->readFromReplayFrame(stream);
    stream->read((char*)&self->mCurrentStateID, 4);
    stream->read((char*)&self->mPrevStateID, 4);
    stream->read((char*)&self->mPendingStateID, 4);
    stream->read((char*)&self->mPendingStateStartupFlag, 4);
    cGFEventStartState_readFromReplayFrame((char*)&self->mEventStartState, stream);
    cGFPreEventState_readFromReplayFrame_14D0((char*)&self->mPreEventState, stream);
    cGFGateState_readFromReplayFrame_14D0((char*)&self->mGateState, stream);
    cGFRidingState_readFromReplayFrame_14D0((char*)&self->mRidingState, stream);
    cGFFinishlineState_readFromReplayFrame((char*)&self->mFinishlineState, stream);
    cGFEndState_readFromReplayFrame_14D0((char*)&self->mEndState, stream);
    cGFPostEventState_readFromReplayFrame_14D0((char*)&self->mPostEventState, stream);
    cGFEndWaitState_readFromReplayFrame_14D0((char*)&self->mEndWaitState, stream);
    cGFTransportWaitState_readFromReplayFrame_14D0((char*)&self->mTransportWaitState, stream);
    cGFTrackStartState_readFromReplayFrame((char*)&self->mTrackStartState, stream);
    cGFSectionLoadState_readFromReplayFrame_14D0((char*)&self->mSectionLoadState, stream);
    cGFPodiumState_readFromReplayFrame_14D0((char*)&self->mPodiumState, stream);
    cGFNextHeatState_readFromReplayFrame((char*)&self->mNextHeatState, stream);
    cGFUserSelectState_readFromReplayFrame((char*)&self->mUserSelectState, stream);
    cGFSessionTeleportState_readFromReplayFrame_14D0((char*)&self->mSessionTeleportState, stream);
    // Each game-flow state is a flat layout that begins with cGameState's fields: the casts view it as its base.
    switch (self->mCurrentStateID) {
    case 0:
        self->mCurrentState = 0;
        break;
    case kGFSM_EventStart:
        self->mCurrentState = (cGameState*)&self->mEventStartState;
        break;
    case kGFSM_PreEvent:
        self->mCurrentState = (cGameState*)&self->mPreEventState;
        break;
    case kGFSM_Gate:
        self->mCurrentState = (cGameState*)&self->mGateState;
        break;
    case kGFSM_Riding:
        self->mCurrentState = (cGameState*)&self->mRidingState;
        break;
    case kGFSM_Finishline:
        self->mCurrentState = (cGameState*)&self->mFinishlineState;
        break;
    case kGFSM_End:
        self->mCurrentState = (cGameState*)&self->mEndState;
        break;
    case kGFSM_PostEvent:
        self->mCurrentState = (cGameState*)&self->mPostEventState;
        break;
    case kGFSM_EndWait:
        self->mCurrentState = (cGameState*)&self->mEndWaitState;
        break;
    case kGFSM_TransportWait:
        self->mCurrentState = (cGameState*)&self->mTransportWaitState;
        break;
    case kGFSM_TrackStart:
        self->mCurrentState = (cGameState*)&self->mTrackStartState;
        break;
    case kGFSM_SectionLoad:
        self->mCurrentState = (cGameState*)&self->mSectionLoadState;
        break;
    case kGFSM_Podium:
        self->mCurrentState = (cGameState*)&self->mPodiumState;
        break;
    case kGFSM_NextHeat:
        self->mCurrentState = (cGameState*)&self->mNextHeatState;
        break;
    case kGFSM_UserSelect:
        self->mCurrentState = (cGameState*)&self->mUserSelectState;
        break;
    case kGFSM_SessionTeleport:
        self->mCurrentState = (cGameState*)&self->mSessionTeleportState;
        break;
    case kGFSM_ReplayStart:
        self->mCurrentState = (cGameState*)&self->mReplayStartState;
        break;
    default:
        self->mCurrentState = 0;
        self->mCurrentStateID = 0;
        break;
    }
    switch (self->mPrevStateID) {
    case kGFSM_EventStart:
        self->mPrevState = (cGameState*)&self->mEventStartState;
        break;
    case kGFSM_PreEvent:
        self->mPrevState = (cGameState*)&self->mPreEventState;
        break;
    case kGFSM_Gate:
        self->mPrevState = (cGameState*)&self->mGateState;
        break;
    case kGFSM_Riding:
        self->mPrevState = (cGameState*)&self->mRidingState;
        break;
    case kGFSM_Finishline:
        self->mPrevState = (cGameState*)&self->mFinishlineState;
        break;
    case kGFSM_End:
        self->mPrevState = (cGameState*)&self->mEndState;
        break;
    case kGFSM_PostEvent:
        self->mPrevState = (cGameState*)&self->mPostEventState;
        break;
    case kGFSM_EndWait:
        self->mPrevState = (cGameState*)&self->mEndWaitState;
        break;
    case kGFSM_TransportWait:
        self->mPrevState = (cGameState*)&self->mTransportWaitState;
        break;
    case kGFSM_TrackStart:
        self->mPrevState = (cGameState*)&self->mTrackStartState;
        break;
    case kGFSM_SectionLoad:
        self->mPrevState = (cGameState*)&self->mSectionLoadState;
        break;
    case kGFSM_Podium:
        self->mPrevState = (cGameState*)&self->mPodiumState;
        break;
    case kGFSM_NextHeat:
        self->mPrevState = (cGameState*)&self->mNextHeatState;
        break;
    case kGFSM_UserSelect:
        self->mPrevState = (cGameState*)&self->mUserSelectState;
        break;
    case kGFSM_SessionTeleport:
        self->mPrevState = (cGameState*)&self->mSessionTeleportState;
        break;
    case kGFSM_ReplayStart:
        self->mPrevState = (cGameState*)&self->mReplayStartState;
        break;
    default:
        self->mPrevStateID = 0;
    case 0:
        self->mPrevState = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_activatePauseMenu);
#ifdef SKIP_ASM
extern int cMoviePlayer_m_isPlaying;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getRiderCtrlID(void* iface, int i);
extern "C" int cGame_canUserActivatePauseMenu(cGame* self);
int cInput_getState(void* obj, int a1);

static inline void** PlayerObj_1840(int p)
{
    return (void**)SSXApp->Inputs[p];
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
 * Returns false while a network error is shown. Looks at the replay's player in replay mode, else at each human
 * rider's controller (cBENewPlayerInterface_getRiderCtrlID): unplugged controllers set bits in a mask, and the first with pause pressed
 * (input 0x3C) becomes the owner. The mask is stored at +0x224 (On Tour ControllerUnplugged); an unplugged
 * controller always pauses, owned by the presser or the first ready player. Otherwise a press pauses when online
 * or when cGame_canUserActivatePauseMenu allows it. The owner goes to +0x228 (PauseOwner).
 *
 * @return true when the pause menu should open
 */
bool cGame::activatePauseMenu()
{
    if (cGameComm_mGameComm != 0 && cGameComm_mGameComm->mDisplayError > 0) {
        return false;
    }
    int mask = 0;
    void* bi = cBE_getInterface_Fv(cBE_getBE(), 1);
    cReplay* replay = Replay;
    int sel = -1;
    if (replay->Loaded) {
        int ctrlId = replay->PadNum;
        void** controller = PlayerObj_1840(ctrlId);
        if (!IsReady_1840(controller)) {
            mask = 1 << ctrlId;
        } else if (cMoviePlayer_m_isPlaying == 0) {
            if (cInput_getState(controller, 0x3C)) {
                sel = ctrlId;
            }
        }
    } else {
        int i;
        for (i = 0; i < Count_1840(((char*)this)); i++) {
            cPlayer* rider = (cPlayer*)At_1840(((char*)this), i);
            if (rider->mRider->mIsLocal) {
                int ctrlId = cBENewPlayerInterface_getRiderCtrlID(bi, i);
                void** controller = PlayerObj_1840(ctrlId);
                if (!IsReady_1840(controller)) {
                    mask |= 1 << ctrlId;
                } else if (sel < 0 && cMoviePlayer_m_isPlaying == 0) {
                    if (cInput_getState(controller, 0x3C)) {
                        sel = ctrlId;
                    }
                }
            }
        }
    }
    ControllerUnplugged = mask;
    if (mask) {
        if (sel < 0) {
            int i;
            sel = 0;
            for (i = 0; i < Count_1840(((char*)this)); i++) {
                cPlayer* rider = (cPlayer*)At_1840(((char*)this), i);
                if (rider->mRider->mIsLocal) {
                    int ctrlId = cBENewPlayerInterface_getRiderCtrlID(bi, i);
                    if (IsReady_1840(PlayerObj_1840(ctrlId))) {
                        sel = ctrlId;
                        break;
                    }
                }
            }
        }
        PauseOwner = sel;
        return true;
    }
    if (sel >= 0) {
        cBE_getInterface_Fv(cBE_getBE(), 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 || canUserActivatePauseMenu()) {
            PauseOwner = sel;
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_canUserActivatePauseMenu);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_IsPauseMenuAllowed(void* a);

/**
 * Tells whether the player may open the pause menu now.
 *
 * Not while mPendingStateID (+0x210) or mCurrentStateID (+0x214) is kGFSM_EventStart, kGFSM_Finishline,
 * kGFSM_TrackStart or kGFSM_UserSelect (On Tour eGFState EventStart, Finishline, TrackStart, UserSelect), and only when
 * the script engine allows it (cSSXScriptEngine_IsPauseMenuAllowed).
 *
 * @return 1 when the pause menu may open, otherwise 0
 */
int cGame::canUserActivatePauseMenu()
{
    int mode = mPendingStateID;
    if (mode != 0) {
        if (mode == kGFSM_Finishline || mode == kGFSM_UserSelect || mode == kGFSM_EventStart
            || mode == kGFSM_TrackStart) {
            return 0;
        }
    }
    int allowed = 0;
    if (cSSXScriptEngine_IsPauseMenuAllowed(ScriptEngine) != 0) {
        int currentStateId = mCurrentStateID;
        allowed = currentStateId != kGFSM_Finishline && currentStateId != kGFSM_UserSelect
                  && currentStateId != kGFSM_EventStart && currentStateId != kGFSM_TrackStart;
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_drawSplash);
#ifdef SKIP_ASM
#include "bx/math3d.h"
#include "replay/bxreplayframe.h"
#include "render/graphicsman.h"

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
 * they go to GrMan's RenderTriStrip (vtable slot 71) with count 4.
 *
 * @param value  the texture id
 */
void cLoadScreen::drawSplash(int value)
{
    sQuadVtx verts[4];
    t4Vector cornerPos;
    int i;

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
    *GrMan->m_pCurrentDrawState->m_textureState.m_texid = value;
    GrMan->RenderTriStrip(4, (tBxVertex*)verts, 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_loseFocus);
#ifdef SKIP_ASM
/**
 * Lets a load screen lose focus only when the app's progress value (cAppMan_TheAppMan + 0x38) is exactly 100.
 *
 * Then it returns its fade's loseFocus (cFadeScreen::loseFocus); otherwise false. It fills the loseFocus slot (+0x24)
 * of cFELoadScreen_vtbl and cLoadScreen_vtbl.
 *
 * @return cFadeScreen::loseFocus's result, or false
 */
bool cLoadScreen::loseFocus()
{
    if (cAppMan_TheAppMan->LoadTimer.loadpercent == 100.0f) {
        return cFadeScreen::loseFocus();
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cLoadScreen_update__FPv);
#ifdef SKIP_ASM
/**
 * Updates a load screen: runs its fade's update (cFadeScreen::update).
 *
 * It fills the update slot (+0x34) of the load-screen vtable cLoadScreen_vtbl; cFELoadScreen_update and the pre-start
 * screens' updates call it too.
 */
void cLoadScreen::update()
{
    cFadeScreen::update();
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_cFadeScreen);
#ifdef SKIP_ASM
/**
 * Constructs a screen fade: vtable cFadeScreen_vtbl, fade timer (+0x4) 0.2, ending flag (+0x8) 0.
 *
 * cGame_cGame, cFE_cFE, the debug front end and the load-screen constructors call it for their fade base.
 */
cFadeScreen::cFadeScreen()
{
    FadeTimer = 0.20000000298023224f;
    IsEnding = 0;
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
 * @return true once the fade level (offset 4) is at least 0.2
 */
bool cFadeScreen::loseFocus()
{
    IsEnding = 1;
    return FadeTimer >= 0.20000000298023224f;
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
 */
void cFadeScreen::update()
{
    if (IsEnding != 0) {
        FadeTimer += 0.01666666753590107f;
    } else {
        FadeTimer -= 0.01666666753590107f;
        if (FadeTimer < 0.0f) {
            FadeTimer = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_drawFade);
#ifdef SKIP_ASM
extern c2DRender* G2D;
extern t4Matrix t4mat_Identity;

extern sDrawState sDrawState_s_defaultDrawState;


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
 * Does nothing and returns false while the timer (+0x4) is 0 or less. Otherwise pushes a copy of the draw state, resets
 * it to sDrawState_s_defaultDrawState, sets the full-screen viewport and the identity matrix and the blend bits, draws
 * a quad from (-1, -1) to (1, 1) with colour {alpha, 0, 0, 0} through c2DRender_drawRect4 (G2D), and pops the draw
 * state. cGame_render, cRealFE_render, cDebugFE_render and cFELoadScreen_render call it.
 *
 * @return true once the fade is fully opaque (alpha >= 1)
 */
bool cFadeScreen::drawFade()
{
    if (FadeTimer > 0.0f) {
    cPS2GraphicsMan* ctx = GrMan;
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
    *GrMan->m_pCurrentDrawState = sDrawState_s_defaultDrawState;
    GrMan->SetViewportOrthographic(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0, 0);
    GrMan->SetView1(t4mat_Identity);
    GrMan->m_pCurrentDrawState->m_bitGroups[2] |= 0x3E0;
    GrMan->m_pCurrentDrawState->m_bitGroups[1] = (GrMan->m_pCurrentDrawState->m_bitGroups[1] & ~0x1800000) | 0x1000000;
    GrMan->m_pCurrentDrawState->m_bitGroups[1] &= ~0x300000;
    GrMan->m_pCurrentDrawState->m_bitGroups[1] &= ~0xFF000;
    float alpha = Clamp231D60(FadeTimer * 5.0f, 0.0f, 1.0f);
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
    G2D->drawRect4(minCorner, maxCorner, col, 0.0f);
    GrMan->m_pCurrentDrawState--;
    return alpha >= 1.0f;
    }
    return false;
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
extern t4Matrix t4mat_Identity;

struct sV2_1FC0 {
    float x, y;
    sV2_1FC0() {}
    sV2_1FC0(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sV4_1FC0 {
    float x, y, z, w;
    sV4_1FC0(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
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

extern sGs_1FC0 sDrawState_s_defaultDrawState_1FC0 __asm__("sDrawState_s_defaultDrawState");

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
    virtual void DrawSprite(const sV4_1FC0&, const sV2_1FC0&, const sV2_1FC0&, const sV2_1FC0&, const tARGB&);
};

struct sVE_1FC0 {
    short delta;
    short index;
    void (*fn)(void*, float, float, float, float, int);
};

extern cSSXApp* SSXApp_1FC0 __asm__("SSXApp");

/**
 * Draws the load indicator: a pulsing sprite at (599, 445) in a full-screen 2D pass.
 *
 * Pushes a copy of the draw state, begins a scene and 2D mode, resets the state to sDrawState_s_defaultDrawState with
 * its blend bits, loads t4mat_Identity, sets the full-screen viewport (640x480) and the texture LoadIconTextureID
 * (SSXApp + 0xA0). The sprite is 18 high and (sin(2 * pi * phase) + 1) * 9 wide, phase being the first field (advanced
 * by cLoadIndicator_update); its colour (+0xC) takes the field at +0x4 as its first component. Then ends 2D mode and
 * the scene and pops the draw state. The pre-start load screens call it.
 */
void cLoadIndicator::render()
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    ((cGfx_1FC0*)ps2GraphicsMan)->stateTop[1] = ((cGfx_1FC0*)ps2GraphicsMan)->stateTop[0];
    ((cGfx_1FC0*)ps2GraphicsMan)->stateTop++;
    ((cGfx_1FC0*)GrMan)->PushViewport();
    ((cGfx_1FC0*)GrMan)->PushView();
    *((cGfx_1FC0*)GrMan)->stateTop = sDrawState_s_defaultDrawState_1FC0;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_23 = 2;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_12 = 20;
    ((cGfx_1FC0*)GrMan)->stateTop->f4_2 = 5;
    ((cGfx_1FC0*)GrMan)->stateTop->f8_5 = 0x1F;
    ((cGfx_1FC0*)GrMan)->SetView(&t4mat_Identity);
    {
        sVE_1FC0* vt = (sVE_1FC0*)VPTR_OF(cGraphicsMan, GrMan);
        void* thisp = vthis(GrMan, vt[23]); // slot 23: SetViewportWindow
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_1FC0*)GrMan)->GetScreenWidth(), (float)((cGfx_1FC0*)GrMan)->GetScreenHeight(), 1);
    }
    ((cGfx_1FC0*)GrMan)->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 1, 0, 0.0f, 1.0f);
    float pulse = fCos(phase * 6.2831854820251465f);
    sV2_1FC0 size((pulse + 1.0f) * 9.0f, 18.0f);
    sV4_1FC0 pos(599.0f, 445.0f, 0.0f, 1.0f);
    int f10v = SSXApp_1FC0->LoadIconTextureID;
    // stored through a plain pointer: an in-struct store would let GrMan's later reload be dropped
    short* textureId = &GrMan->m_pCurrentDrawState->m_textureState.m_texid[0];
    *textureId = f10v;
    colour.A = alpha;
    sV2_1FC0 uv0(0.0f, 0.0f);
    sV2_1FC0 uv1(1.0f, 1.0f);
    ((cGfx_1FC0*)GrMan)->DrawSprite(pos, size, uv0, uv1, colour);
    ((cGfx_1FC0*)GrMan)->PopView();
    ((cGfx_1FC0*)GrMan)->PopViewport();
    ((cGfx_1FC0*)GrMan)->stateTop--;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cPreStartScreen_cPreStartScreen__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the pre-start screen app module: installs its vtable (cPreStartScreen_vtbl) and clears its timer
 * (progressivetimer) and the init load screen it creates (nextscreen).
 */
cPreStartScreen::cPreStartScreen()
{
    progressivetimer = 0.0f;
    nextscreen = 0;
}
#endif

