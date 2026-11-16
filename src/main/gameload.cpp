#include "common.h"
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
void func_00317530(void* bar, float v);
void func_00317550(void* bar, float v);
extern "C" void func_00195DE0(void* p);
extern "C" void cSSXScriptEngine_Load(void* eng);
extern "C" void* cFont_cFont(void* p, const char* name, int a);
extern "C" void cGameComm_syncGame(void* comm);
void func_0014BD98(void* iface);
extern "C" void cBucketMan_init(void* bm, int n);
extern "C" void func_00355260(void* bm, int a);
extern "C" void func_0016C918(void* p);
void* cStreamPause_construct();
void* cOcclusionMan_cOcclusionMan(void* p);
extern "C" void* cWorld_cWorld(void* p);
extern "C" void* cWorldLightMan_cWorldLightMan(void* p, void* a);
extern "C" void* func_003087D0(void* p);
extern "C" void* func_00229180(void* p);
extern "C" void* func_00343A68(void* p);
extern "C" void* func_003579C8(void* p);
extern "C" void* cWScriptCompile_parseKeywords(void* p, int a, int b);
extern "C" void cIrradianceDataBase_Load(void* p);
void* cBXLoadIndicator_cBXLoadIndicator(void* p);
struct cBigFile;
void* cBigFile_cBigFile(cBigFile* p);
extern "C" void cGame_loadTrack(cGame* self);
extern "C" void* func_0026EFB8(void* p);
extern "C" void* func_002DBD10(void* p, int a);
extern "C" void* func_00237060(void* p, int n, int m);
int BXrand();
extern "C" void func_00237CB0(int seed);
extern "C" void* cAI_cAI(void* p);
extern "C" void* cPlaybackMan_cPlaybackMan(void* p, int n);
extern "C" void cReplay_init(void* p);
extern "C" void cAI_initAIRiders(void* ai);
extern "C" void cGameModeMan_initGameMode(void* p, int mode);
extern "C" char* func_001526D0(void* iface);
extern "C" void func_0012BAC0(void* ai, void* p);
extern "C" void cAI_initAIActors(void* ai, int a);
extern "C" void cAI_loadRiders(void* ai);
extern "C" void THREAD_yieldticks(int n);
extern "C" int func_0012A180(void* ai);
void func_00270EF8(void* p);
extern "C" void func_00271758(void* p, void* a);
void func_00270F18(void* p);
extern "C" void func_00152728(void* iface);
extern "C" void* cGameViewMan_cGameViewMan(void* p, int a);
extern "C" void func_0015EDC8(void* p, void* a);
extern "C" void* func_002CC018(void* p);
extern "C" char* FILE_load(const char* name, int flags);
extern "C" void* func_002E4228(void* p, int a, int i);
extern "C" void* cSplashManager_cSplashManager(void* p, int a, int i);
extern "C" void* cCutSceneTransitionMan_cCutSceneTransitionMan(void* p, int a);
extern "C" void* cFlagManager_cFlagManager(void* p, int a);
extern "C" void* cSnowFall_cSnowFall(void* p, int a);
extern "C" void cSSXAudio_FrontEndLoad(void* p);
extern "C" void* func_00397B08(void* p);
extern "C" void cUIEngine_loadFile(void* ui, char* name);
extern "C" void* func_001A2F38(void* p);
extern "C" void* func_001E9A30(void* p, void* ui, int a);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
void func_0026F898(void* p, int a);
void func_00231278(void* self, int val);
extern "C" void func_00230F40(void* self);
extern "C" void func_003E5398(int a);
extern "C" int func_002312D8(void* self);
extern "C" void func_002586B0(void* comm);
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern "C" void* cObjNode_cObjNode1(void* p, int a);
extern "C" void* func_002F0390();
extern "C" void* cVisualEffectsMan_construct();
extern "C" void* func_002D2100();
extern char* D_004A5B64;
extern void* D_004A289C;
extern void* cGameComm_mGameComm;
extern void* gSSXAudio;
extern int D_004A1228;
extern int D_004A3484;
extern void* cGameState_mpGame;
extern char D_004A5988_gl __asm__("gBucketMan");
extern int cBENetworkInterface_networkOpt[];
extern char cBENewRaceInterface_newRace[];
extern int D_00535BFC[];
extern char D_004C5830[];
extern char D_005047F8[];
extern char D_004A2A58[];
extern char D_0047B548[];
extern char D_0047B558[];
extern char D_0047B570[];
extern char D_004A2A60[];
extern char D_0047B580[];
extern char D_0047B590[];
extern char D_0047B5A0[];
extern char D_0047B5B0[];
extern char D_0047B5C0[];
extern char D_0047B5D8[];
extern char D_00479A80[];
extern char D_004A2A68[];
extern char D_004A2A70[];
extern char D_0047B5E8[];
extern char D_0047B5F8[];
extern char D_004A2A78[];
extern char D_0047B608[];
extern char D_0047B618[];
extern char D_004A2A80[];
extern char D_0047B628[];
extern char D_004A2A88[];
extern char D_0047B640[];
extern char D_0047B650[];
extern char D_0047B660[];
extern char D_0047B678[];
extern char D_0047B688[];
extern char D_0047B698[];
extern char D_0047B6B8[];
extern char D_0047B6C8[];
extern char D_0047B6D8[];
extern char D_0047B6E8[];
extern char D_0047B700[];
extern char D_0047B718[];
extern void* cFEInput_vtbl[];
extern void* D_0047CB60[];
extern void* D_0047D070[];
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

struct cGameGL {
    char pad_000[0xC];
    char* ai; // 0xC
    void* world; // 0x10
    char pad_014[0xC];
    sGridGL* grid; // 0x20
    void* m24; // 0x24
    void* m28; // 0x28
    void* m2C; // 0x2C
    void* m30; // 0x30
    void* m34; // 0x34
    void* m38; // 0x38
    void* m3C; // 0x3C
    void* m40; // 0x40
    char pad_044[0x4];
    sUIGL* ui; // 0x48
    char pad_04C[0x4];
    void* m50; // 0x50
    void* m54; // 0x54
    void* m58; // 0x58
    void* m5C[2]; // 0x5C
    void* m64; // 0x64
    void* m68[2]; // 0x68
    void* m70; // 0x70
    void* m74; // 0x74
    char pad_078[0x4];
    void* m7C; // 0x7C
    char pad_080[0x4];
    void* m84; // 0x84
    void* m88; // 0x88
    void* m8C; // 0x8C
    void* m90; // 0x90
    void* m94; // 0x94
    void* m98; // 0x98
    int m9C; // 0x9C
    int mA0; // 0xA0
    char* hud; // 0xA4
    void* mA8; // 0xA8
    int mAC; // 0xAC
    char pad_0B0[0x14C];
    int m1FC; // 0x1FC
    int m200; // 0x200
    int m204; // 0x204
    int m208; // 0x208
    int m20C; // 0x20C
    int m210; // 0x210
    int m214; // 0x214
    int m218; // 0x218
    int m21C; // 0x21C
    int m220; // 0x220
};

extern "C" void cGame_load(cGame* self)
{
    sGridGL** pGrid = (sGridGL**)&self->BXNash;
    void** worlds = ((void**)&self->TransitionMan[0]);
    void* bm = &D_004A5988_gl;
    void** lights = ((void**)&self->SplashManager[0]);
    func_00317530(D_004A5B64 + 0x38, 13.199999809265137f);
    func_00317550(D_004A5B64 + 0x38, 43.5f);
    func_00195DE0(SSXApp->FELocale);
    {
        char* g = (char*)D_004A289C;
        sVEntryVoidGL* vt = *(sVEntryVoidGL**)(g + 0x10D8);
        vt[44].fn(g + vt[44].delta, 0);
    }
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cSSXScriptEngine_Load(ScriptEngine);
    sGridGL* grid = new (cMemMan_alloc(0xA8, D_004A2A58, 0, 0)) sGridGL;
    *pGrid = grid;
    grid->a4 = 0;
    grid->a0 = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    self->DebugFont = (cFont*)cFont_cFont(operator new(0x84, D_0047B548, 0, 0), D_0047B558, 0);
    if (cGameComm_mGameComm != 0)
        cGameComm_syncGame(cGameComm_mGameComm);
    func_0014BD98(cBE_getInterface_Fv(cBE_getBE(), 9));
    cBucketMan_init(bm, 9);
    func_00355260(bm, 1);
    func_0016C918(D_004C5830);
    self->StreamPause = (cStreamPause*)cStreamPause_construct();
    self->OcclusionMan = (cOcclusionMan*)cOcclusionMan_cOcclusionMan(operator new(0x114, D_0047B570, 0x20000000, 0));
    self->World = (cWorld*)cWorld_cWorld(operator new(0x18, D_004A2A60, 0, 0));
    self->WorldLightMan = (cWorldLightMan*)cWorldLightMan_cWorldLightMan(operator new(0x32A0, D_0047B580, 0x20000000, 0), (sGridGL*)self->BXNash);
    self->WScriptMan = (cWScriptMan*)func_003087D0(operator new(0x498, D_0047B590, 0x20000000, 0));
    self->CrowdMan2d = (cCrowdMan2d*)func_00229180(operator new(0x2720, D_0047B5A0, 0x20000000, 0));
    self->CrowdMan3d = (cCrowdMan3d*)func_00343A68(operator new(0x9C4, D_0047B5B0, 0x20000000, 0));
    self->MultiParticleMan = (cMultiParticleMan*)func_003579C8(operator new(0x10, D_0047B5C0, 0x20000000, 0));
    self->InstanceMan = (cInstanceMan*)cWScriptCompile_parseKeywords(operator new(0x10, D_0047B5D8, 0x20000000, 0), 0x80, 0x40);
    cIrradianceDataBase_Load(D_005047F8);
    self->LoadIndicator = (cBXLoadIndicator*)cBXLoadIndicator_cBXLoadIndicator(operator new(0x10, D_00479A80, 0x20000000, 0));
    self->mpTrackBigFile = (cBigFile*)cBigFile_cBigFile((cBigFile*)operator new(8, D_004A2A68, 0, 0));
    cGame_loadTrack(self);
    self->Replay = (cReplay*)func_0026EFB8(operator new(0x688, D_004A2A70, 0, 0));
    self->FXLoader = (cFXLoader*)func_002DBD10(operator new(1, D_0047B5E8, 0, 0), 0);
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt[0] != 0) {
        float dt = *(float*)((char*)SSXApp + 0x14);
        int n = (int)((*(float*)&cBENetworkInterface_networkOpt[4] + (float)*(int*)((char*)cGameComm_mGameComm + 0x120) * dt) / dt) + 2;
        self->InputSync = (cInputSync*)func_00237060(operator new(0x44, D_0047B5F8, 0, 0), n, *(int*)((char*)cGameComm_mGameComm + 0x120));
    } else {
        func_00237CB0(BXrand());
    }
    {
        char* ai = (char*)cAI_cAI(operator new(0xD0, D_004A2A78, 0, 0));
        self->AI = (cAI*)ai;
        sVEntryVoidGL* vt = *(sVEntryVoidGL**)(ai + 0xCC);
        vt[2].fn(ai + vt[2].delta);
    }
    func_00317550(D_004A5B64 + 0x38, 55.0f);
    self->PlaybackMan = (cPlaybackMan*)cPlaybackMan_cPlaybackMan(operator new(0x7C, D_0047B608, 0x20000000, 0), 0x28);
    cReplay_init(self->Replay);
    void* race = cBE_getInterface_Fv(SSXApp->BE, 5);
    int replay = D_004A1228;
    if (replay != 0) {
        cAI_initAIRiders((char*)self->AI);
        signed char* gs = (signed char*)cBENewRaceInterface_newRace;
        cGameModeMan_initGameMode(SSXApp->GameModeMan, gs[0x4A]);
        func_0012BAC0((char*)self->AI, func_001526D0(race) + 0x20);
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
        while (!func_0012A180((char*)self->AI)) {
            cAI_loadRiders((char*)self->AI);
            THREAD_yieldticks(1);
        }
        func_00270EF8(self->Replay);
        func_00271758(self->Replay, func_001526D0(race));
        func_00270F18(self->Replay);
        func_00152728(race);
    }
    if (cBENetworkInterface_networkOpt[0] != 0)
        self->ViewMan = (cGameViewMan*)cGameViewMan_cGameViewMan(operator new(0x38, D_0047B618, 0x20000000, 0), 1);
    else
        self->ViewMan = (cGameViewMan*)cGameViewMan_cGameViewMan(operator new(0x38, D_0047B618, 0x20000000, 0), D_00535BFC[0]);
    if (cBENetworkInterface_networkOpt[0] != 0 && cBENetworkInterface_networkOpt[2] == 0) {
        char* a = *(char**)((char*)self->AI + 0x44);
        void* cam = a != 0 ? *(void**)(a + 0x18) : 0;
        func_0015EDC8(*(void**)((char*)self->ViewMan + 0x4), cam);
    }
    char* hud = (char*)func_002CC018(operator new(0x68, D_004A2A80, 0, 0));
    self->MenuSys = (cMenuSys*)hud;
    *(void**)(hud + 0x58) = (void*)self->DebugFont;
    *(void**)((char*)self->MenuSys + 0x5C) = (void*)SSXApp->Inputs[0];
    char* file = FILE_load(D_0047B628, 0x100);
    {
        char* g = (char*)D_004A289C;
        sVEntryGL* vt = *(sVEntryGL**)(g + 0x10D8);
        int r = vt[46].fn(g + vt[46].delta, file + *(int*)(file + 0x14), D_004A2A88, 0, 1, -1);
        self->mBoxTextureID = r;
        *(int*)((char*)self->MenuSys + 0x60) = r;
    }
    if (file != 0)
        cMemMan_free(file);
    func_00317550(D_004A5B64 + 0x38, 93.0f);
    for (unsigned int i = 0; i < 2; i++) {
        worlds[i] = func_002E4228(operator new(0x84, D_0047B640, 0, 0), 3, i);
        lights[i] = cSplashManager_cSplashManager(operator new(0x1030, D_0047B650, 0, 0), 2, i);
    }
    self->CutSceneTransitionMan = (cCutSceneTransitionMan*)cCutSceneTransitionMan_cCutSceneTransitionMan(operator new(0x48, D_0047B660, 0, 0), 3);
    self->FlagManager = (cFlagManager*)cFlagManager_cFlagManager(operator new(0x1718, D_0047B678, 0, 0), 2);
    self->SnowFall = (cSnowFall*)cSnowFall_cSnowFall(operator new(0x60, D_0047B688, 0, 0), 2);
    self->mUpdateTick = 0;
    cSSXAudio_FrontEndLoad(gSSXAudio);
    func_00317550(D_004A5B64 + 0x38, 99.0f);
    sUIGL* ui = (sUIGL*)func_00397B08(operator new(0x70, D_0047B698, 0, 0));
    self->UI = (cUIEngine*)ui;
    ui->p10 = (void*)SSXApp->FELocale;
    sName11GL name = D_0047B6A8;
    cUIEngine_loadFile((sUIGL*)self->UI, D_0047B6A8.c);
    sUIDataGL* data = (sUIDataGL*)operator new(0x1C, D_0047B6B8, 0, 0);
    data->flag = 0;
    data->vtbl = cFEInput_vtbl;
    data->f = 0;
    data->c = 0;
    data->d = 0;
    data->e = 0;
    ((sUIGL*)self->UI)->data = data;
    ((sUIGL*)self->UI)->p14 = func_001A2F38(operator new(8, D_0047B6C8, 0, 0));
    ((sUIGL*)self->UI)->p8 = (void*)SSXApp->Fonts;
    sStateGL* st = (sStateGL*)func_001E9A30(operator new(0xC98, D_0047B6D8, 0, 0), (sUIGL*)self->UI, 0);
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
    cGameState_mpGame = ((cGameGL*)self);
    if (replay != 0) {
        func_0026F898(self->Replay, D_004A3484);
        func_00231250(((cGameGL*)self), 0x10, 0, 0);
    } else {
        func_00231278(((cGameGL*)self), 0xA);
        do {
            func_00230F40(((cGameGL*)self));
            func_003E5398(0);
        } while (!func_002312D8(((cGameGL*)self)));
        if (cGameComm_mGameComm != 0)
            func_002586B0(cGameComm_mGameComm);
        if (cBENewRaceInterface_newRace[0x49] == 0)
            func_00231250(((cGameGL*)self), 0xA, 0, 0);
        else
            func_00231250(((cGameGL*)self), 0xA, 0, 0);
    }
    sNamedGL* o = (sNamedGL*)operator new(0x14, D_0047B6E8, 0, 0);
    cObjNode_cObjNode1(o, 6);
    o->vtbl = D_0047CB60;
    void* r = func_002F0390();
    self->AIVisualEffectsMan = (cAIVisualEffectsObjNode*)o;
    o->obj = r;
    o = (sNamedGL*)operator new(0x14, D_0047B700, 0, 0);
    cObjNode_cObjNode1(o, 3);
    o->vtbl = D_0047D070;
    r = cVisualEffectsMan_construct();
    self->VisualEffectsMan = (cVisualEffectsObjNode*)o;
    o->obj = r;
    o = (sNamedGL*)operator new(0x14, D_0047B718, 0, 0);
    cObjNode_cObjNode1(o, 4);
    o->vtbl = cSSXStatsObjNode_vtbl;
    r = func_002D2100();
    o->obj = r;
    self->SSXStatsMan = (cSSXStatsObjNode*)o;
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

extern char* D_004A3E90;
extern int D_004A11B8;
extern char D_0047B480[];
extern char D_004A2A90[];
extern char D_004A2A98[];
extern char D_0047B728[];
extern char D_0047B738[];
extern char D_0047B748[];
extern char D_0047B758[];
extern char D_004A2AA0[];
extern char D_004A2AA8[];
extern char D_004C5830[];
extern void* D_004882E0[];

extern "C" void* cBXString_cBXString2(sBXStr_F6B0* self, const char* s);
extern "C" sBXStr_F6B0* func_00318630(sBXStr_F6B0* self, sBXStr_F6B0* a, const char* str);
extern "C" sBXStr_F6B0* func_003186D0(sBXStr_F6B0* self, const char* str, sBXStr_F6B0* b);
extern "C" void cBXString__cBXString(sBXStr_F6B0* self, int flags);
extern "C" sBXStr_F6B0* cBXString_operatorE(sBXStr_F6B0* self, sBXStr_F6B0* o);
extern "C" char* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cBigFile_open(void* bf, const char* name, int flags);
extern "C" void cWorld_load(void* a, const char* name, void* b);
void* func_00353AC0(void* p);
extern "C" void* func_0022CCE8(void* p, void* game);
extern "C" void func_002C2F18(void* p);
extern "C" void* func_002B6AA0(void* p, int a);
extern "C" void cBXNash_initTheAIFwdDiffCache(void* p);
extern "C" void func_0022D088(void* p, int id, int a);
extern "C" int func_0022D278(void* p, int id);
extern "C" void cSectionMan_update(void* p);
void func_003A6928(void* p);
extern "C" void func_003E5398(int a);
// PORT: func_0016CA10__FPv also receives the path string (passed through in $5)
void func_0016CA10_F6B0(void* p, const char* s) __asm__("func_0016CA10__FPv");
extern "C" void func_002DBF80(void* p);
void* cLensFxMan_construct();
extern "C" void func_002ED048();

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

extern "C" void cGame_loadTrack(cGame* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    sBXStr_F6B0 name;
    name.p = D_004A3E90;
    if (D_004A11B8 != 0) {
        sBXStr_F6B0 a;
        sBXStr_F6B0 b;
        func_003186D0(&a, D_0047B480, (cBXString_cBXString2(&b, cBENewRaceInterface_getLocation(iface) + 0x44), &b));
        cBXString_operatorE(&name, &a);
        cBXString__cBXString(&a, 2);
        cBXString__cBXString(&b, 2);
    } else {
        sBXStr_F6B0 a;
        sBXStr_F6B0 b;
        func_003186D0(&a, D_0047B480, (cBXString_cBXString2(&b, cBENewRaceInterface_getLocation(iface) + 0x34), &b));
        cBXString_operatorE(&name, &a);
        cBXString__cBXString(&a, 2);
        cBXString__cBXString(&b, 2);
    }
    {
        sBXStr_F6B0 path;
        func_00318630(&path, &name, D_004A2A90);
        cBigFile_open(self->mpTrackBigFile, path.p, 0);
        cBXString__cBXString(&path, 2);
    }
    sTbl_F6B0*& tbl = (sTbl_F6B0*)self->SurfPropCache;
    char*& ent = (char*)self->FogMan;
    cWorld_load(self->World, name.p, self->BXNash);
    *(void**)&self->SkyBox = func_00353AC0(cMemMan_alloc(0x18, D_004A2A98, 0, 0));
    *(void**)&self->SectionMan = func_0022CCE8(cMemMan_alloc(0x1D4, D_0047B728, 0, 0), ((char*)self));
    tbl = new (D_0047B738, 0, 0) sTbl_F6B0;
    func_002C2F18(self->SurfPropCache);
    *(void**)&self->WorldSound = func_002B6AA0(cMemMan_alloc(0x250, D_0047B748, 0, 0), 0);
    cBXNash_initTheAIFwdDiffCache(self->BXNash);
    sBXStr_F6B0 dir;
    dir.p = D_004A3E90;
    *(int*)(*(char**)self->World + 0x3D4) = 0;
    {
        sBXStr_F6B0 a;
        sBXStr_F6B0 b;
        func_003186D0(&a, D_0047B480, (cBXString_cBXString2(&b, cBENewRaceInterface_getLocation(iface) + 0x34), &b));
        cBXString_operatorE(&dir, &a);
        cBXString__cBXString(&a, 2);
        cBXString__cBXString(&b, 2);
    }
    int id = *(int*)cBENewRaceInterface_getLocation(iface);
    func_0022D088(self->SectionMan, id, 0);
    *(int*)((char*)self->SectionMan + 0x1A4) = 1;
    while (!func_0022D278(self->SectionMan, id)) {
        cSectionMan_update(self->SectionMan);
        func_003A6928(self->World);
        func_003E5398(0);
    }
    *(int*)(*(char**)self->World + 0x3D4) = 1;
    {
        sBXStr_F6B0 a;
        sBXStr_F6B0 c;
        func_0016CA10_F6B0(D_004C5830, (func_003186D0(&c, D_004A2AA0, &dir), func_00318630(&a, &c, D_004A2AA8), a.p));
        cBXString__cBXString(&a, 2);
        cBXString__cBXString(&c, 2);
    }
    char* obj = (char*)cMemMan_alloc(0x3010, D_0047B758, 0, 0);
    func_002DBF80(obj);
    *(void***)(obj + 4) = D_004882E0;
    new (obj + 0x10) sElemFF_F6B0[256];
    ent = obj;
    *(void**)&self->LensFxMan = cLensFxMan_construct();
    func_002ED048();
    cBXString__cBXString(&dir, 2);
    cBXString__cBXString(&name, 2);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_0022FA98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cBE_setState(int state);
extern "C" void cCameraTriggerMan_cleanupOnExit(void* self);
extern "C" void cCameraTriggerMan_purge(void* self);
extern "C" void func_001A3C50(void* p);
extern "C" void cSSXScriptEngine_ForceEndScriptsAll(void* p);
extern "C" void func_00397B70(void* p, int flags);
extern "C" void func_002370A0(void* p, int flags);
extern "C" void func_002EA820(void* p);
void func_002C3FA0(void* p);
void operator_delete(int* p);
extern "C" void func_0026AF00(void* p);
extern "C" void func_00334850(void* p);
extern "C" void func_002CC048(void* p, int flags);
extern "C" void cBucketMan_mergeBucket(void* mgr, int a);
extern "C" void func_00354B38(void* mgr);
extern "C" void func_00229278(void* p, int flags);
extern "C" void func_00343B28(void* p, int flags);
extern "C" void func_00357A00(void* p, int flags);
extern "C" void func_0026C588(void* p, int flags);
extern "C" void func_00351120(void* p, int flags);
extern "C" void func_002ECFA0(void);
extern "C" void func_0022E730(void* p, int flags);
extern "C" void func_0026F088(void* p, int flags);
extern "C" void func_0022CD18(void* p, int flags);
extern "C" void func_002DBEE0(void* p, int flags);
extern "C" void func_002F5230(void* p, int flags);
extern "C" void cWorld__cWorld(void* p, int flags);
extern "C" void func_00242518(void* p, int flags);
struct cBigFile;
void cBigFile__cBigFile(cBigFile* self, int flags);
extern "C" void func_003916C0(void* p, int flags);
extern "C" void func_0036A020(void* p);
extern "C" void func_0038ADB0(void* p);
extern "C" void func_002B6AE0(void* p, int flags);
extern "C" void func_002B68D0(void* a);
extern "C" void func_00286A80(void* p);
extern "C" void func_00286C00(void* p);
extern "C" void cSSXScriptEngine_Purge(void* p);
extern "C" void func_0036A290(void);

extern void* D_004A289C;
extern void* gWorldTriggerManager;
extern void* gSSXAudio;
extern int D_004A2A54;
extern char* D_004A5B64;
extern char D_004D33A0[];
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
extern sGameRes_FA98 D_00534B30_s __asm__("cBENetworkInterface_networkOpt");
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

extern "C" void func_0022FA98(char* self)
{
    cBE_setState(0);
    char* iface = (char*)cBE_getInterface_Fv(SSXApp->BE, 7);
    if (D_00534B30_s.active != 0 && D_00534B30_s.ready != 0) {
        int which = D_00534B30_s.which;
        int other = which != 0;
        D_00534B30_s.rec[0] = *(sRec_FA98*)(*(char**)(GetRider_FA98(which == 0) + 0x790) + 0xFC);
        D_00534F0C_FA98 = *(sRec_FA98*)(*(char**)(GetRider_FA98(other) + 0x790) + 0xFC);
        sVEnt0_FA98* e = (sVEnt0_FA98*)(*(char**)(iface + 0xC) + 8);
        e->fn(iface + e->delta);
    }
    cCameraTriggerMan_cleanupOnExit(D_004C5830);
    {
        char* be = (char*)D_004A289C;
        sVEnt0_FA98* e = (sVEnt0_FA98*)(*(char**)(be + 0x10D8) + 0x98);
        e->fn(be + e->delta);
    }
    func_001A3C50(SSXApp->mFEAsyncManager);
    cSSXScriptEngine_ForceEndScriptsAll(ScriptEngine);
    if (*(void**)(self + 0x48) != 0) func_00397B70(*(void**)(self + 0x48), 3);
    if (*(void**)(self + 0x7C) != 0) func_002370A0(*(void**)(self + 0x7C), 3);
    func_002EA820(*(void**)(self + 0x64));
    func_002C3FA0(*(void**)(self + 0x44));
    operator_delete(*(int**)(self + 0x44));
    VDelete_FA98(*(char**)(self + 0x14), 0xC, 8);
    VDelete_FA98(*(char**)(self + 0x18), 0x4, 8);
    VDelete_FA98(*(char**)(self + 0x1C), 0x649C, 8);
    func_0026AF00(D_004D33A0);
    func_00334850(*(void**)(self + 0x20));
    if (*(void**)(self + 0xA4) != 0) func_002CC048(*(void**)(self + 0xA4), 3);
    void* bm = &D_004A5988_gl;
    cBucketMan_mergeBucket(bm, 1);
    func_00354B38(bm);
    VDelete_FA98(*(char**)(self + 0x30), 0x494, 8);
    if (*(void**)(self + 0x38) != 0) func_00229278(*(void**)(self + 0x38), 3);
    if (*(void**)(self + 0x3C) != 0) func_00343B28(*(void**)(self + 0x3C), 3);
    if (*(void**)(self + 0x40) != 0) func_00357A00(*(void**)(self + 0x40), 3);
    if (*(void**)(self + 0x34) != 0) func_0026C588(*(void**)(self + 0x34), 3);
    if (*(void**)(self + 0x88) != 0) func_00351120(*(void**)(self + 0x88), 3);
    cCameraTriggerMan_purge(D_004C5830);
    func_002ECFA0();
    if (*(void**)(self + 0x84) != 0) func_0022E730(*(void**)(self + 0x84), 3);
    VDelete_FA98(*(char**)(self + 0xC), 0xCC, 8);
    if (*(void**)(self + 0x28) != 0) func_0026F088(*(void**)(self + 0x28), 3);
    VDelete_FA98(*(char**)(self + 0x98), 0x0, 8);
    if (*(void**)(self + 0x78) != 0) func_0022CD18(*(void**)(self + 0x78), 3);
    if (*(void**)(self + 0x2C) != 0) func_002DBEE0(*(void**)(self + 0x2C), 3);
    if (*(void**)(self + 0x90) != 0) func_002F5230(*(void**)(self + 0x90), 3);
    if (*(void**)(self + 0x10) != 0) cWorld__cWorld(*(void**)(self + 0x10), 3);
    if (*(void**)(self + 0x8C) != 0) func_00242518(*(void**)(self + 0x8C), 3);
    if (*(cBigFile**)(self + 0xA8) != 0) cBigFile__cBigFile(*(cBigFile**)(self + 0xA8), 3);
    operator_delete(*(int**)(self + 0x94));
    if (*(void**)(self + 0x24) != 0) func_003916C0(*(void**)(self + 0x24), 3);
    {
        sObj20_FA98* o = *(sObj20_FA98**)(self + 0x20);
        if (o != 0) {
            o->~sObj20_FA98();
            operator_delete((int*)o);
        }
    }
    {
        char* be = (char*)D_004A289C;
        sVEnt0_FA98* e = (sVEnt0_FA98*)(*(char**)(be + 0x10D8) + 0x388);
        e->fn(be + e->delta);
    }
    {
        char* be = (char*)D_004A289C;
        sVEnt1_FA98* e = (sVEnt1_FA98*)(*(char**)(be + 0x10D8) + 0x190);
        e->fn(be + e->delta, *(void**)(self + 0x220));
    }
    func_0036A020(D_004A289C);
    func_0038ADB0(D_005047F8);
    *(void**)&SSXApp->Game = 0;
    if (*(void**)(self + 0x4C) != 0) func_002B6AE0(*(void**)(self + 0x4C), 3);
    func_002B68D0(gWorldTriggerManager);
    func_00286A80(gSSXAudio);
    func_00286C00(gSSXAudio);
    cSSXScriptEngine_Purge(ScriptEngine);
    func_0036A290();
    D_004A2A54 = 0;
    *(int*)(D_004A5B64 + 0x34) = 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230050);
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
extern "C" void func_00195D50(void* a);
extern "C" void func_001F30D8(void* a);
void func_00278710(void* a);
extern "C" void func_00369FF8(void* a);
extern "C" void cWorld_init(void* a);
extern "C" void func_002B68D0(void* a);
extern "C" void func_002867E8(void* a);
extern void* D_004A289C;
extern void* gWorldTriggerManager;
extern void* gSSXAudio;
extern void* cOVStateManager_mpPDATemplate;
extern cMgr230050* D_004A4248;
extern int D_004A43C8;
extern int D_004A4324;
extern int cScreenTint_m_Enable;

extern "C" void func_00230050(cGame* self)
{
    func_00195D50(SSXApp->FELocale);
    void* p94 = self->LoadIndicator;
    cOVStateManager_mpPDATemplate = 0;
    func_001F30D8(p94);
    func_00278710(ScriptEngine);
    D_004A4248->v17();
    D_004A4248->v04();
    func_00369FF8(D_004A289C);
    cWorld_init(self->World);
    func_002B68D0(gWorldTriggerManager);
    func_002867E8(gSSXAudio);
    D_004A4324 = 1;
    cScreenTint_m_Enable = 1;
    D_004A43C8 = 1;
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
extern void* D_004A3DD8;
// The unit declares gBucketMan later as sGp2898 (8 bytes); bind a small-data alias here.
extern char D_004A5988_bm __asm__("gBucketMan");

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
    cWScriptMan_runGlobalScripts(D_004A3DD8);
    cWScriptMan_setupMissions(self->WScriptMan);
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_partialRestart);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_snapAll(void* self);
extern "C" void cReplay_reset(void* self);
extern "C" void cSSXScriptEngine_Restart(void* self);
extern "C" void func_0015DB58(void);
extern "C" void func_00229498(void* self);
extern "C" void func_00343BC0(void* self);
extern "C" void func_00357B38(void* self);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" void func_00287108(void* self);
extern "C" void* func_0028B180(void);
int GetHashValue32(char* s);
extern char D_004A2AB0[];

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

extern "C" void cGame_partialRestart(cGame* self)
{
    void* bm = &D_004A5988_bm;
    cPlaybackMan_restart(self->PlaybackMan);
    cWScriptMan_initMissions(self->WScriptMan);
    cBucketMan_mergeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 1);
    cBucketMan_purgeBucket(bm, 8);
    char* o = (char*)self->AI;
    sVEntry_230180a* vt = *(sVEntry_230180a**)(o + 0xCC);
    vt[5].fn(o + vt[5].delta, 3);
    cAvalanche_reset();
    cGameViewMan_snapAll(self->ViewMan);
    cWorld_resetMap(self->World);
    cReplay_reset(self->Replay);
    cSSXScriptEngine_Restart(ScriptEngine);
    func_0015DB58();
    func_00229498(self->CrowdMan2d);
    func_00343BC0(self->CrowdMan3d);
    func_00357B38(self->MultiParticleMan);
    void* list = (char*)self->UI + 0x18;
    char* r = (char*)cUIStateStack_findState(list, GetHashValue32(D_004A2AB0));
    if (r != 0) {
        sVEntry_230180b* vt2 = *(sVEntry_230180b**)(r + 8);
        vt2[24].fn(r + vt2[24].delta, 7, 0);
    }
    cWScriptMan_runGlobalScripts(D_004A3DD8);
    cWScriptMan_setupMissions(self->WScriptMan);
    func_00287108(func_0028B180());
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/gameload", cGame_restart);
#ifdef SKIP_ASM
extern "C" void cGame_partialRestart(cGame* self);
extern "C" void cGameModeMan_restartHeat(void* gm);
extern "C" void func_00231250(void* self, int a, int b, int refresh);
extern "C" void* func_0028B180(void);
extern "C" void func_002870A0(void* mgr);

static inline int numRiders_2302A8(void* game)
{
    return *(int*)(*(char**)((char*)game + 0xC) + 0x78);
}

static inline char* rider_2302A8(void* game, int i)
{
    return *(char**)(*(char**)((char*)game + 0xC) + (i << 2) + 0x28);
}

extern "C" void cGame_restart(cGame* self)
{
    cGame_partialRestart(self);
    cGameModeMan_restartHeat(SSXApp->GameModeMan);
    int i;
    for (i = 0; i < numRiders_2302A8(self); i++) {
        *(int*)(rider_2302A8(self, i) + 0x2F8) = 0;
    }
    func_00231250(self, 1, 2, 1);
    func_002870A0(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230338);
#ifdef SKIP_ASM
extern "C" void func_00230338(void* self)
{
    void* a = *(void**)((char*)self + 0xC);
    if (a != 0) {
        void* b = *(void**)((char*)a + 0xA4);
        if (b != 0) {
            *(int*)((char*)b + 0xD0) = -1;
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
extern "C" void func_00357B90(void* self, int arg);
extern "C" void func_003551A8(void* self, int index, int key);
extern "C" void func_00103308(void* p, int arg);
extern "C" void cWScriptMan_unloadSection(void* self, int i);
extern void* D_004A2A00;
extern void* D_004A3FF0;
extern void* D_004A4028;

extern "C" void cGame_sectionUnloading(cGame* self, int id)
{
    void* bm = &D_004A5988_bm;
    func_00358700(id);
    func_00229408(D_004A2A00, id);
    func_00343C08(D_004A3FF0, id);
    func_00357B90(D_004A4028, id);
    cBucketMan_mergeBucket(bm, 1);
    func_003551A8(bm, 1, id);
    func_003551A8(bm, 8, id);
    void* p = self->AI;
    if (p != 0) {
        void* q = *(void**)((char*)p + 0xA4);
        if (q != 0) {
            func_00103308(q, id);
            func_00103308(*(void**)((char*)self->AI + 0xA4), 0x80);
        }
    }
    cWScriptMan_unloadSection(self->WScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230430);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: func_00308B58 is declared with one argument; it forwards the section id in $a1 to func_00308B78
void* func_00308B58_2(void* self, int sectionId) __asm__("func_00308B58__FPv");

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

extern sWorldCache_0430* D_004A47B8_0430 __asm__("cWorld_m_pTheWorld");

static inline bool isSectionLoaded_0430(sScriptCache_0430* c, int sectionId)
{
    return c->sections[sectionId].state >= 2 && c->sections[sectionId].data != 0;
}

extern "C" void func_00230430(void* self, int sectionId)
{
    if (isSectionLoaded_0430(D_004A47B8_0430->scripts, sectionId)) {
        func_00308B58_2(D_004A3DD8, sectionId);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", cGame_exit);
#ifdef SKIP_ASM
struct cAppMan;
void cReplay_stopAutoReplay(void* self);
extern "C" void func_00258AE0(void* p);
extern "C" void func_00266DF8(void* p);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void* func_00232738(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern void* cGameComm_mGameComm;
extern void* D_004A33F4;
extern char D_0047B770[];
extern char D_0047B788[];

extern "C" void cGame_exit(cGame* self, int toFrontEnd)
{
    cReplay_stopAutoReplay(self->Replay);
    func_00231250(self, 6, 0, 0);
    if (cGameComm_mGameComm != 0) {
        func_00258AE0(cGameComm_mGameComm);
    }
    if (D_004A33F4 != 0) {
        func_00266DF8(D_004A33F4);
    }
    if (toFrontEnd != 0) {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, D_0047B770, 0x100, 0)));
    } else {
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)func_00232738(cMemMan_alloc(8, D_0047B788, 0x100, 0)));
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230558);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0026F8A0(void* a, int b, int c, int d);
void func_002CC0B8(void* a, int b);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_00442910[];

struct sGameLoad_0558 {
    char pad_0x00[0x28];
    void* field_0x28;       // 0x28
    char pad_0x2C[0xA4 - 0x2C];
    void* field_0xA4;       // 0xA4
};

extern "C" void func_00230558(sGameLoad_0558* self)
{
    D_005366E8[++D_004A2A54] = 8;
    D_004A2A50 = D_00442910[0];
    func_0026F8A0(self->field_0x28, 0, 0, 0);
    func_002CC0B8(self->field_0xA4, 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002305C8);
#ifdef SKIP_ASM
extern "C" void func_0026F980(void* a);
void func_002CC0B8(void* a, int b);
extern "C" void func_002CC460(void* a);
extern int D_004A2A54;
extern int D_004A2A50;
extern int D_005366E8[];
extern int D_004428F0[];

extern "C" void func_002305C8(void* self)
{
    func_0026F980(*(void**)((char*)self + 0x28));
    func_002CC0B8(*(void**)((char*)self + 0xA4), 1);
    func_002CC460(*(void**)((char*)self + 0xA4));
    D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230640);
#ifdef SKIP_ASM
extern "C" void func_003550A0(void* mgr, int id);
struct sGp2898 { int a, b; };
extern sGp2898 gBucketMan; // target: raw $gp+0x2898 (splat doesn't symbolize it)
extern int D_004428D8[];

extern "C" void func_00230640(void)
{
    int i;
    sGp2898* mgr = &gBucketMan;
    for (i = 0; i < 6; i++) {
        func_003550A0(mgr, D_004428D8[i]);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230698__FPvi);
#ifdef SKIP_ASM
int func_00230698(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x5C);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002306A8__FPvi);
#ifdef SKIP_ASM
int func_002306A8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x68);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002306B8);
#ifdef SKIP_ASM
extern "C" float func_002EE400(int id);
extern "C" void func_002570D8(void* p);
extern "C" void func_00278918(void* p);
extern "C" int func_0022E920(void* p);
extern "C" void cGameViewMan_updateAll(void* p);
extern "C" void func_00230F40(void* self);
extern "C" void func_0022E8B8(void* p);
extern "C" void func_003A88A8(void* p);
extern "C" void func_002373B8(void* p);
extern "C" void func_0020CFD0(void);
extern "C" int func_002CC098(void* p);
extern "C" void func_002CC0C0(void* p);
extern "C" int* func_0030C870(void* p);
extern "C" void cOVStateManager_triggerOverlay(int id);
extern "C" void func_0029CED8(void* p, int a, int b, float c);
extern "C" void cWScriptMan_doMissionCmd(void* p, int a, int b, int c);
extern "C" void func_00398038(void* p);
extern "C" void func_0030C820(void* p);
extern "C" void func_0029ED18(void* p);
// PORT: the unit defines func_00231840(char* self); this caller passes a second argument (a1 = 0).
extern "C" int func_00231840_pi(void* self, int a) __asm__("func_00231840");
// func_00231D18 returns nothing (its body has no return value); the unit declares it void*.
extern "C" void func_00231D18_v(void* self) __asm__("func_00231D18");
extern "C" int func_0020CBE8(void);
extern "C" void func_00326AE0(void* p);
extern "C" void func_0020CA10(void);
extern "C" void func_00289B70(void* p);
extern "C" void func_00294F48(void* p);
extern "C" int func_0020CBA0(void);
extern "C" void cPlaybackMan_update(void* p);
extern "C" int func_00237950(void* p);
extern "C" int func_002379C8(void* p);
extern "C" int func_0026CE50(void* p);
extern "C" void func_0026F4A8(void* p);
extern "C" int func_00270280(void* p);
extern "C" void func_00354F98(void* mgr, int a);
extern "C" void func_00355028(void* mgr, int a);
extern "C" void func_00309270(void* p);
extern "C" void func_002294C8(void* p);
extern "C" void func_003440C8(void* p);
extern "C" void func_00357BF8(void* p);
extern "C" void func_002B7848(void* p, void* q, int i);
void func_00237948(void* p);
extern "C" void func_003F42A0(void);
extern "C" void cFEAsyncManager_Update(void* p);
extern "C" void func_001E1458(void);
extern "C" void func_001F3188(void* p);

extern char* D_004A5B64;
extern void* cCommSystem_mCommSystem;
extern void* PadCache;
extern unsigned int D_004A20CC;
extern char D_0047B798[];

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

// cGame (this unit: cGame_loadTrack/cGame_restart/cGame_exit). Field accesses through this struct
// (not raw casts) matter: gcc 2.95 lets struct-field loads skip the dependence on fixed-scalar stores.
struct cGame_2306B8 {
    char pad0[0x8];
    int f8;
    char* fC;
    char* f10;
    char pad14[0x28 - 0x14];
    char* f28;
    char pad2C[0x30 - 0x2C];
    char* f30;
    char* mPlaybackMan;
    char* f38;
    char* f3C;
    char* f40;
    char pad44[0x48 - 0x44];
    char* f48;
    char* f4C;
    char pad50[0x78 - 0x50];
    char* f78;
    char* f7C;
    char pad80[0x84 - 0x80];
    char* mGameViewMan;
    char pad88[0x94 - 0x88];
    char* f94;
    char pad98[0x9C - 0x98];
    int f9C;
    int fA0;
    char* fA4;
    char padA8[0xAC - 0xA8];
    int fAC;
    char padB0[0x214 - 0xB0];
    int f214;
    char pad218[0x228 - 0x218];
    unsigned char f228;
};

static inline void pushState_2306B8(int state)
{
    D_005366E8[++D_004A2A54] = state;
    D_004A2A50 = D_004428F0[state];
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
    return D_004A2A50 & 1;
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

static inline int canUpdate_2306B8(cGame_2306B8* self)
{
    if (*(int*)((char*)SSXApp + 0x4) != 0) {
        self->f8 = 1;
        return 0;
    }
    return 1;
}

static inline int updatePause_2306B8(cGame_2306B8* self)
{
    if (func_0022E920(self->mGameViewMan) == 0 || *(int*)(self->f78 + 0x1D0) == 0) {
        *(int*)(*((char**)self->f10) + 0x3D4) = 0;
        func_003A6928(self->f10);
        cGameViewMan_updateAll(self->mGameViewMan);
        if (*(int*)(self->f78 + 0x1D0) == 0) {
            func_00230F40(self);
            cSectionMan_update(self->f78);
        }
        if (self->f9C == 0 && func_0022E920(self->mGameViewMan) == 0) {
            self->fA0 = 1;
        }
        self->f9C = 1;
        if (self->fA0 != 0) {
            char* r = (char*)D_004A289C;
            sVEntry_2306B8* vt = *(sVEntry_2306B8**)(r + 0x10D8);
            vt[19].fn(r + vt[19].delta);
            func_0022E8B8(self->mGameViewMan);
            func_003A88A8(*((void**)self->f10));
            self->fA0 = 0;
        }
        return 0;
    }
    *(int*)(*((char**)self->f10) + 0x3D4) = 1;
    self->f9C = 0;
    self->fA0 = 0;
    func_003A6928(self->f10);
    cSectionMan_update(self->f78);
    return 1;
}

static inline int updateState_2306B8(cGame_2306B8* self)
{
    if (func_002CC098(self->fA4) == 0) {
        char* q = self->f30;
        char* list = q + 0x1C4;
        if (*(int*)(q + 0x1C4) != *(int*)(q + 0x1C8)) {
            int* e = func_0030C870(list);
            if (e[1] == 8) {
                cOVStateManager_triggerOverlay(0x1D);
                pushState_2306B8(3);
                func_0029CED8(func_0028B180(), 4, 0, 1.0f);
                return 0;
            }
            if (e[1] == 9) {
                cWScriptMan_doMissionCmd(self->f30, e[0], 5, e[2]);
                if (cUIStateStack_findState(self->f48 + 0x18, GetHashValue32(D_0047B798)) == 0) {
                    cOVStateManager_triggerOverlay(0x1E);
                    pushState_2306B8(3);
                    func_00398038(self->f48);
                } else {
                    func_0030C820((char*)D_004A3DD8 + 0x1C4);
                }
                return 0;
            }
        }
    }
    if (~D_004A20CC != 0) {
        cOVStateManager_triggerOverlay(0x22);
        pushState_2306B8(12);
        D_004A20CC = 0xFFFFFFFF;
        func_0029ED18(func_0028B180());
        return 0;
    }
    if (func_002CC098(self->fA4) != 0) {
        func_00398038(self->f48);
        func_002CC0C0(self->fA4);
        return 0;
    }
    if (func_00231840_pi(self, 0) != 0 && !inRace_2306B8(*((int*)self->f28))
        && notPlaying_2306B8(self->mPlaybackMan) && func_0020CBE8() != 0) {
        func_00326AE0(PadCache);
        func_0020CA10();
        pushState_2306B8(2);
        {
            sObjC_2306B8* o = ((sObj48_2306B8*)self->f48)->fC;
            sVEntryI_2306B8* vt = o->vtbl;
            vt[44].fn((char*)o + vt[44].delta, self->f228);
        }
        func_00289B70(func_0028B180());
        func_00294F48(func_0028B180());
        return 0;
    }
    {
        int wasPaused = paused_2306B8();
        func_00398038(self->f48);
        if (func_0020CBA0() == 0) {
            cPlaybackMan_update(self->mPlaybackMan);
            if (func_002CC098(self->fA4) != 0) {
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

extern "C" void func_002306B8(cGame* self)
{
    if (*(unsigned int*)((char*)self->ViewMan + 0x10) >= 2) {
        *(int*)(D_004A5B64 + 0x34) = 1;
    } else {
        *(int*)(D_004A5B64 + 0x34) = func_002EE400(6) > 0.9f;
    }
    if (cGameComm_mGameComm != 0) {
        func_002570D8(cGameComm_mGameComm);
    }
    func_00278918(ScriptEngine);
    func_00231D18_v(((cGame_2306B8*)self));
    if (canUpdate_2306B8(((cGame_2306B8*)self)) && updatePause_2306B8(((cGame_2306B8*)self))) {
        if ((char*)self->InputSync != 0) {
            func_002373B8((char*)self->InputSync);
        }
        func_0020CFD0();
        if (updateState_2306B8(((cGame_2306B8*)self))) {
            int skip;
            if ((char*)self->InputSync == 0 || func_00237950((char*)self->InputSync) != 0) {
                if (self->mCurrentStateID == 3) {
                    func_00230F40(((cGame_2306B8*)self));
                    cBucketMan_mergeBucket(&gBucketMan, 1);
                } else {
                    func_00230F40(((cGame_2306B8*)self));
                }
                if (!paused_2306B8()) {
                    func_0026F4A8((char*)self->Replay);
                }
                if (func_00270280((char*)self->Replay) == 0 && !(D_004A2A50 & 8)) {
                    char* w = (char*)ScriptEngine;
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(w + 0x2A8);
                    vt[5].fn(w + vt[5].delta);
                }
            }
            skip = 0;
            if ((D_004A2A50 & 1) || func_00270280((char*)self->Replay) != 0) {
                skip = 1;
            }
            if (!skip) {
                if (((char*)self->InputSync != 0 && func_002379C8((char*)self->InputSync) == 0)
                    || func_0026CE50((char*)self->PlaybackMan) != 0) {
                    skip = 1;
                }
            }
            if (!skip) {
                if (!(D_004A2A50 & 2)) {
                    func_00354F98(&gBucketMan, 1);
                }
                func_00354F98(&gBucketMan, 5);
                func_00354F98(&gBucketMan, 6);
                if (!(D_004A2A50 & 2)) {
                    func_00355028(&gBucketMan, 1);
                }
                {
                    char* c = (char*)self->AI;
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(c + 0xCC);
                    vt[3].fn(c + vt[3].delta);
                }
                if (!(D_004A2A50 & 0x10)) {
                    func_00354F98(&gBucketMan, 2);
                }
                if (!(D_004A2A50 & 4)) {
                    func_00354F98(&gBucketMan, 3);
                }
                func_00309270((char*)self->WScriptMan);
                func_002294C8((char*)self->CrowdMan2d);
                func_003440C8((char*)self->CrowdMan3d);
                func_00357BF8((char*)self->MultiParticleMan);
                cBucketMan_mergeBucket(&gBucketMan, 1);
            } else {
                func_00354F98(&gBucketMan, 5);
                if (!(D_004A2A50 & 4)) {
                    func_00354F98(&gBucketMan, 3);
                }
            }
            func_00354F98(&gBucketMan, 4);
            {
                int s = *((int*)(char*)self->Replay);
                if (!inRace_2306B8(s) || notMode3_2306B8(s)) {
                    cGameViewMan_updateAll((char*)self->ViewMan);
                }
            }
            (self->mUpdateTick)++;
            {
                unsigned int i;
                for (i = 0; i < (((sViewList_2306B8*)(char*)self->ViewMan))->getCount(); i++) {
                    func_002B7848((char*)self->WorldSound, (((sViewList_2306B8*)(char*)self->ViewMan))->getView(i) + 0x20, i);
                }
            }
            {
                int i;
                for (i = 0; i < (((sRiderList_2306B8*)(char*)self->AI))->getCount(); i++) {
                    char* r = (((sRiderList_2306B8*)(char*)self->AI))->getRider(i);
                    sVEntry_2306B8* vt = *(sVEntry_2306B8**)(r + 0x6C0);
                    vt[19].fn(r + vt[19].delta);
                }
            }
        }
        if ((char*)self->InputSync != 0) {
            func_00237948((char*)self->InputSync);
        }
    }
    if (cCommSystem_mCommSystem != 0) {
        func_003F42A0();
    }
    if ((void*)SSXApp->mFEAsyncManager != 0) {
        cFEAsyncManager_Update(SSXApp->mFEAsyncManager);
    }
    func_001E1458();
    func_001F3188((char*)self->LoadIndicator);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230E90);
#ifdef SKIP_ASM
extern "C" void func_00230E90(void) {

}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230E98);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_loseFocus(void* self);
void func_00278718(void* self);
void func_001F3170(void* self);

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

extern "C" int func_00230E98(cGame* self)
{
    void* obj = self->StreamPause;
    sVEntry00230E98* vt = *(sVEntry00230E98**)obj;
    if (vt[3].fn((char*)obj + vt[3].delta) == 0) {
        return 0;
    }
    if (cFadeScreen_loseFocus(self) == 0) {
        return 0;
    }
    cMgr00230E98* mgr = (cMgr00230E98*)D_004A289C;
    mgr->v15(sVec3_00230E98(0.0f, 0.0f, 0.0f));
    ((cMgr00230E98*)D_004A289C)->f270 = 1;
    D_004A4324 = 0;
    cScreenTint_m_Enable = 0;
    D_004A43C8 = 0;
    func_00278718(ScriptEngine);
    func_001F3170(self->LoadIndicator);
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00230F40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0022E550(void* self);
extern int D_004A2A50;
extern int cBENetworkInterface_networkOpt[];

class cGameState_0F40 {
public:
    int pad[3];
    virtual void v01();
    virtual void enter();
    virtual void update();
    virtual void v04();
    virtual void start(int arg);
    virtual void tick();
    virtual void exit();
};

struct sGameLoad_0F40 {
    char pad0[0x84];
    void* loader;                   // 0x84
    char pad88[0x174];
    cGameState_0F40* prev;          // 0x1FC
    cGameState_0F40* cur;           // 0x200
    cGameState_0F40* pending;       // 0x204
    int pendingId;                  // 0x208
    int enteredId;                  // 0x20C
    int nextId;                     // 0x210
    int curId;                      // 0x214
    int prevId;                     // 0x218
    int nextArg;                    // 0x21C
    char pad220[0x4];
    int hold;                       // 0x224
};

// Game load state machine: runs the current state and switches to the requested one.
extern "C" void func_00230F40(void* self_)
{
    sGameLoad_0F40* self = (sGameLoad_0F40*)self_;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt[0] == 0 && self->hold != 0)
        return;
    if (D_004A2A50 & 0x20)
        return;
    if (self->cur != 0 && (self->curId != 3 || self->nextId == 0))
        self->cur->tick();
    if (self->pendingId != 0) {
        switch (self->pendingId) {
    case 1:
        self->pending = (cGameState_0F40*)((char*)self + 0xB0);
        break;
    case 2:
        self->pending = (cGameState_0F40*)((char*)self + 0xD0);
        break;
    case 3:
        self->pending = (cGameState_0F40*)((char*)self + 0xE0);
        break;
    case 4:
        self->pending = (cGameState_0F40*)((char*)self + 0xF0);
        break;
    case 5:
        self->pending = (cGameState_0F40*)((char*)self + 0x100);
        break;
    case 6:
        self->pending = (cGameState_0F40*)((char*)self + 0x118);
        break;
    case 7:
        self->pending = (cGameState_0F40*)((char*)self + 0x128);
        break;
    case 8:
        self->pending = (cGameState_0F40*)((char*)self + 0x138);
        break;
    case 9:
        self->pending = (cGameState_0F40*)((char*)self + 0x148);
        break;
    case 10:
        self->pending = (cGameState_0F40*)((char*)self + 0x158);
        break;
    case 11:
        self->pending = (cGameState_0F40*)((char*)self + 0x178);
        break;
    case 12:
        self->pending = (cGameState_0F40*)((char*)self + 0x18C);
        break;
    case 13:
        self->pending = (cGameState_0F40*)((char*)self + 0x19C);
        break;
    case 14:
        self->pending = (cGameState_0F40*)((char*)self + 0x1B8);
        break;
    case 15:
        self->pending = (cGameState_0F40*)((char*)self + 0x1DC);
        break;
    case 16:
        self->pending = (cGameState_0F40*)((char*)self + 0x1EC);
        break;
        }
        self->enteredId = self->pendingId;
        self->pendingId = 0;
        self->pending->enter();
    }
    if (self->pending != 0)
        self->pending->update();
    int next = self->nextId;
    if (next == 0)
        return;
    int arg = self->nextArg;
    if (self->cur != 0)
        self->cur->exit();
    self->prevId = self->curId;
    self->prev = self->cur;
    switch (next) {
    case 1:
        self->cur = (cGameState_0F40*)((char*)self + 0xB0);
        self->curId = 1;
        break;
    case 2:
        self->cur = (cGameState_0F40*)((char*)self + 0xD0);
        self->curId = 2;
        break;
    case 3:
        self->cur = (cGameState_0F40*)((char*)self + 0xE0);
        self->curId = 3;
        break;
    case 4:
        self->cur = (cGameState_0F40*)((char*)self + 0xF0);
        self->curId = 4;
        break;
    case 5:
        self->cur = (cGameState_0F40*)((char*)self + 0x100);
        self->curId = 5;
        break;
    case 6:
        self->cur = (cGameState_0F40*)((char*)self + 0x118);
        self->curId = 6;
        break;
    case 7:
        self->cur = (cGameState_0F40*)((char*)self + 0x128);
        self->curId = 7;
        break;
    case 8:
        self->cur = (cGameState_0F40*)((char*)self + 0x138);
        self->curId = 8;
        break;
    case 9:
        self->cur = (cGameState_0F40*)((char*)self + 0x148);
        self->curId = 9;
        break;
    case 10:
        self->cur = (cGameState_0F40*)((char*)self + 0x158);
        self->curId = 10;
        break;
    case 11:
        self->cur = (cGameState_0F40*)((char*)self + 0x178);
        self->curId = 11;
        break;
    case 12:
        self->cur = (cGameState_0F40*)((char*)self + 0x18C);
        self->curId = 12;
        break;
    case 13:
        self->cur = (cGameState_0F40*)((char*)self + 0x19C);
        self->curId = 13;
        break;
    case 14:
        self->cur = (cGameState_0F40*)((char*)self + 0x1B8);
        self->curId = 14;
        break;
    case 15:
        self->cur = (cGameState_0F40*)((char*)self + 0x1DC);
        self->curId = 15;
        break;
    case 16:
        self->cur = (cGameState_0F40*)((char*)self + 0x1EC);
        self->curId = 16;
        break;
    }
    self->nextId = 0;
    self->nextArg = 0;
    if (self->enteredId == self->curId) {
        self->enteredId = 0;
        self->pending = 0;
    }
    if (self->cur != 0) {
        func_0022E550(self->loader);
        self->cur->start(arg);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231250);
#ifdef SKIP_ASM
extern "C" void func_00230F40(void* self);

extern "C" void func_00231250(void* self, int a, int b, int refresh)
{
    *(int*)((char*)self + 0x210) = a;
    *(int*)((char*)self + 0x21C) = b;
    if (refresh != 0) {
        func_00230F40(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231278__FPvi);
#ifdef SKIP_ASM
void func_00231278(void* self, int val)
{
    *(int*)((char*)self + 0x208) = val;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231280);
#ifdef SKIP_ASM
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
INCLUDE_ASM("main/gameload", func_002312D8);
#ifdef SKIP_ASM
struct sVEntry_002312D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_002312D8(void* self)
{
    if (*(int*)((char*)self + 0x208) != 0) {
        return 0;
    }
    void* p = *(void**)((char*)self + 0x204);
    if (p == 0) {
        return 1;
    }
    sVEntry_002312D8* vt = *(sVEntry_002312D8**)((char*)p + 0xC);
    return vt[8].fn((char*)p + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231320);
#ifdef SKIP_ASM
extern "C" void cGameState_gotoNextState(void* p);

extern "C" void func_00231320(void* self)
{
    void* p = *(void**)((char*)self + 0x200);
    if (p != 0) {
        cGameState_gotoNextState(p);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231348);
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
void func_00233B90_s(void* p, void* s) __asm__("func_00233B90__FPv");
void func_00233BD0_s(void* p, void* s) __asm__("func_00233BD0__FPv");
void func_00234DE0_s(void* p, void* s) __asm__("func_00234DE0__FPv");
void func_00234EB8_s(void* p, void* s) __asm__("func_00234EB8__FPv");
void func_002361C8_s(void* p, void* s) __asm__("func_002361C8__FPv");
void func_00236AF8_s(void* p, void* s) __asm__("func_00236AF8__FPv");
void func_00236B70_s(void* p, void* s) __asm__("func_00236B70__FPv");
void func_00236D60_s(void* p, void* s) __asm__("func_00236D60__FPv");
void func_00236EC8_s(void* p, void* s) __asm__("func_00236EC8__FPv");
void func_00236FA8_s(void* p, void* s) __asm__("func_00236FA8__FPv");
extern "C" void func_002340B8(void* p, void* s);
extern "C" void func_00234990(void* p, void* s);
extern "C" void func_00235990(void* p, void* s);
extern "C" void func_00235F48(void* p, void* s);
extern "C" void func_00236728(void* p, void* s);

extern "C" void func_00231348(cGame* self, cStream_00231348* stream)
{
    ((cSerObj_00231348*)self->FlagManager)->serialize(stream);
    ((cSerObj_00231348*)self->SnowFall)->serialize(stream);
    stream->write((char*)self + 0x214, 4);
    stream->write((char*)self + 0x218, 4);
    stream->write((char*)self + 0x210, 4);
    stream->write((char*)self + 0x21C, 4);
    func_00234990((char*)self + 0xB0, stream);
    func_00236D60_s((char*)self + 0xD0, stream);
    func_00234DE0_s((char*)self + 0xE0, stream);
    func_00234EB8_s((char*)self + 0xF0, stream);
    func_002340B8((char*)self + 0x100, stream);
    func_00233B90_s((char*)self + 0x118, stream);
    func_00236EC8_s((char*)self + 0x128, stream);
    func_00233BD0_s((char*)self + 0x138, stream);
    func_00236B70_s((char*)self + 0x148, stream);
    func_00235990((char*)self + 0x158, stream);
    func_00236AF8_s((char*)self + 0x178, stream);
    func_00236FA8_s((char*)self + 0x18C, stream);
    func_00235F48((char*)self + 0x19C, stream);
    func_00236728((char*)self + 0x1B8, stream);
    func_002361C8_s((char*)self + 0x1DC, stream);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002314D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: these state loaders are declared with one parameter in their units; this caller also passes the stream.
extern "C" void func_00234A30(void* state, void* stream);
void func_00236D80_14D0(void* state, void* stream) __asm__("func_00236D80__FPv");
void func_00234E00_14D0(void* state, void* stream) __asm__("func_00234E00__FPv");
void func_00234ED8_14D0(void* state, void* stream) __asm__("func_00234ED8__FPv");
extern "C" void func_00234120(void* state, void* stream);
void func_00233BB0_14D0(void* state, void* stream) __asm__("func_00233BB0__FPv");
void func_00236EE8_14D0(void* state, void* stream) __asm__("func_00236EE8__FPv");
void func_00233BF0_14D0(void* state, void* stream) __asm__("func_00233BF0__FPv");
void func_00236B90_14D0(void* state, void* stream) __asm__("func_00236B90__FPv");
extern "C" void func_00235A18(void* state, void* stream);
void func_00236B18_14D0(void* state, void* stream) __asm__("func_00236B18__FPv");
void func_00236FC8_14D0(void* state, void* stream) __asm__("func_00236FC8__FPv");
extern "C" void func_00235FD0(void* state, void* stream);
extern "C" void func_002367C8(void* state, void* stream);
void func_002361E8_14D0(void* state, void* stream) __asm__("func_002361E8__FPv");

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

struct sGameLoad_14D0 {
    char pad0[0x70];
    cLoadable_14D0* a;              // 0x70
    cLoadable_14D0* b;              // 0x74
    char pad78[0x184];
    cGameState_14D0* prev;          // 0x1FC
    cGameState_14D0* cur;           // 0x200
    char pad204[0xC];
    int nextId;                     // 0x210
    int curId;                      // 0x214
    int prevId;                     // 0x218
    int nextArg;                    // 0x21C
};

// Game load state machine: restores the state ids and every state from a save stream.
extern "C" void func_002314D0(cGame* self_, cStream_14D0* stream)
{
    sGameLoad_14D0* self = (sGameLoad_14D0*)self_;
    self->a->load(stream);
    self->b->load(stream);
    stream->read(&self->curId, 4);
    stream->read(&self->prevId, 4);
    stream->read(&self->nextId, 4);
    stream->read(&self->nextArg, 4);
    func_00234A30((char*)self + 0xB0, stream);
    func_00236D80_14D0((char*)self + 0xD0, stream);
    func_00234E00_14D0((char*)self + 0xE0, stream);
    func_00234ED8_14D0((char*)self + 0xF0, stream);
    func_00234120((char*)self + 0x100, stream);
    func_00233BB0_14D0((char*)self + 0x118, stream);
    func_00236EE8_14D0((char*)self + 0x128, stream);
    func_00233BF0_14D0((char*)self + 0x138, stream);
    func_00236B90_14D0((char*)self + 0x148, stream);
    func_00235A18((char*)self + 0x158, stream);
    func_00236B18_14D0((char*)self + 0x178, stream);
    func_00236FC8_14D0((char*)self + 0x18C, stream);
    func_00235FD0((char*)self + 0x19C, stream);
    func_002367C8((char*)self + 0x1B8, stream);
    func_002361E8_14D0((char*)self + 0x1DC, stream);
    switch (self->curId) {
    case 0:
        self->cur = 0;
        break;
    case 1:
        self->cur = (cGameState_14D0*)((char*)self + 0xB0);
        break;
    case 2:
        self->cur = (cGameState_14D0*)((char*)self + 0xD0);
        break;
    case 3:
        self->cur = (cGameState_14D0*)((char*)self + 0xE0);
        break;
    case 4:
        self->cur = (cGameState_14D0*)((char*)self + 0xF0);
        break;
    case 5:
        self->cur = (cGameState_14D0*)((char*)self + 0x100);
        break;
    case 6:
        self->cur = (cGameState_14D0*)((char*)self + 0x118);
        break;
    case 7:
        self->cur = (cGameState_14D0*)((char*)self + 0x128);
        break;
    case 8:
        self->cur = (cGameState_14D0*)((char*)self + 0x138);
        break;
    case 9:
        self->cur = (cGameState_14D0*)((char*)self + 0x148);
        break;
    case 10:
        self->cur = (cGameState_14D0*)((char*)self + 0x158);
        break;
    case 11:
        self->cur = (cGameState_14D0*)((char*)self + 0x178);
        break;
    case 12:
        self->cur = (cGameState_14D0*)((char*)self + 0x18C);
        break;
    case 13:
        self->cur = (cGameState_14D0*)((char*)self + 0x19C);
        break;
    case 14:
        self->cur = (cGameState_14D0*)((char*)self + 0x1B8);
        break;
    case 15:
        self->cur = (cGameState_14D0*)((char*)self + 0x1DC);
        break;
    case 16:
        self->cur = (cGameState_14D0*)((char*)self + 0x1EC);
        break;
    default:
        self->cur = 0;
        self->curId = 0;
        break;
    }
    switch (self->prevId) {
    case 1:
        self->prev = (cGameState_14D0*)((char*)self + 0xB0);
        break;
    case 2:
        self->prev = (cGameState_14D0*)((char*)self + 0xD0);
        break;
    case 3:
        self->prev = (cGameState_14D0*)((char*)self + 0xE0);
        break;
    case 4:
        self->prev = (cGameState_14D0*)((char*)self + 0xF0);
        break;
    case 5:
        self->prev = (cGameState_14D0*)((char*)self + 0x100);
        break;
    case 6:
        self->prev = (cGameState_14D0*)((char*)self + 0x118);
        break;
    case 7:
        self->prev = (cGameState_14D0*)((char*)self + 0x128);
        break;
    case 8:
        self->prev = (cGameState_14D0*)((char*)self + 0x138);
        break;
    case 9:
        self->prev = (cGameState_14D0*)((char*)self + 0x148);
        break;
    case 10:
        self->prev = (cGameState_14D0*)((char*)self + 0x158);
        break;
    case 11:
        self->prev = (cGameState_14D0*)((char*)self + 0x178);
        break;
    case 12:
        self->prev = (cGameState_14D0*)((char*)self + 0x18C);
        break;
    case 13:
        self->prev = (cGameState_14D0*)((char*)self + 0x19C);
        break;
    case 14:
        self->prev = (cGameState_14D0*)((char*)self + 0x1B8);
        break;
    case 15:
        self->prev = (cGameState_14D0*)((char*)self + 0x1DC);
        break;
    case 16:
        self->prev = (cGameState_14D0*)((char*)self + 0x1EC);
        break;
    default:
        self->prevId = 0;
    case 0:
        self->prev = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231840);
#ifdef SKIP_ASM
extern void* cGameComm_mGameComm;
extern int D_004A2E9C;
extern int cBENetworkInterface_networkOpt[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00147290(void* iface, int i);
extern "C" int func_00231AB8(void* self);
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

extern "C" int func_00231840(char* self)
{
    if (cGameComm_mGameComm != 0 && *(int*)((char*)cGameComm_mGameComm + 0x8) > 0) {
        return 0;
    }
    int mask = 0;
    void* bi = cBE_getInterface_Fv(cBE_getBE(), 1);
    char* g = *(char**)(self + 0x28);
    int sel = -1;
    if (*(int*)(g + 0x608)) {
        int p = *(int*)(g + 0x28);
        void** o = PlayerObj_1840(p);
        if (!IsReady_1840(o)) {
            mask = 1 << p;
        } else if (D_004A2E9C == 0) {
            if (cInput_getState(o, 0x3C)) {
                sel = p;
            }
        }
    } else {
        int i;
        for (i = 0; i < Count_1840(self); i++) {
            char* r = At_1840(self, i);
            if (*(int*)(*(char**)(r + 0x18) + 0x87C)) {
                int p = func_00147290(bi, i);
                void** o = PlayerObj_1840(p);
                if (!IsReady_1840(o)) {
                    mask |= 1 << p;
                } else if (sel < 0 && D_004A2E9C == 0) {
                    if (cInput_getState(o, 0x3C)) {
                        sel = p;
                    }
                }
            }
        }
    }
    *(int*)(self + 0x224) = mask;
    if (mask) {
        if (sel < 0) {
            int i;
            sel = 0;
            for (i = 0; i < Count_1840(self); i++) {
                char* r = At_1840(self, i);
                if (*(int*)(*(char**)(r + 0x18) + 0x87C)) {
                    int p = func_00147290(bi, i);
                    if (IsReady_1840(PlayerObj_1840(p))) {
                        sel = p;
                        break;
                    }
                }
            }
        }
        *(int*)(self + 0x228) = sel;
        return 1;
    }
    if (sel >= 0) {
        cBE_getInterface_Fv(cBE_getBE(), 7);
        if (cBENetworkInterface_networkOpt[0] != 0 || func_00231AB8(self)) {
            *(int*)(self + 0x228) = sel;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231AB8);
#ifdef SKIP_ASM
extern "C" int func_00278DA0(void* a);

extern "C" int func_00231AB8(void* self)
{
    int mode = *(int*)((char*)self + 0x210);
    if (mode != 0) {
        if (mode == 5 || mode == 14 || mode == 1 || mode == 10) {
            return 0;
        }
    }
    int r = 0;
    if (func_00278DA0(ScriptEngine) != 0) {
        int m = *(int*)((char*)self + 0x214);
        r = m != 5 && m != 14 && m != 1 && m != 10;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231B50);
#ifdef SKIP_ASM
#include "bx/math3d.h"

struct sQuadVtx {
    sQuadVtx() {}
    float u, v, q, pad;
    int r, g, b, a;
    t4Vector pos;
};

extern "C" void func_00231B50(void* self, int value) {
    sQuadVtx verts[4];
    t4Vector p;
    int i;
    char* obj;

    for (i = 0; i < 4; i++) {
        verts[i].a = 0x80;
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
        verts[i].q = 1.0f;
    }
    p.x = 0.0f; p.y = 0.0f; p.z = 0.0f; p.w = 1.0f;
    verts[0].pos = p;
    p.x = 1.0f; p.y = 0.0f; p.z = 0.0f;
    verts[1].pos = p;
    p.x = 0.0f; p.y = 1.0f; p.z = 0.0f;
    verts[2].pos = p;
    p.x = 1.0f; p.y = 1.0f; p.z = 0.0f;
    verts[3].pos = p;
    verts[0].u = 0.0f; verts[0].v = 0.0f;
    verts[1].u = 1.0f; verts[1].v = 0.0f;
    verts[2].u = 0.0f; verts[2].v = 1.0f;
    verts[3].u = 1.0f; verts[3].v = 1.0f;
    *(short*)(*(char**)((char*)D_004A289C + 0xE84) + 0x10) = value;
    obj = *(char**)((char*)D_004A289C + 0x10D8);
    (*(void (**)(void*, int, void*, int))(obj + 0x23C))((char*)D_004A289C + *(short*)(obj + 0x238), 4, verts, 0);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231C70);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_loseFocus(void* self);
extern char* D_004A5B64;

extern "C" int func_00231C70(void* self)
{
    if (*(float*)(D_004A5B64 + 0x38) == 100.0f) {
        return cFadeScreen_loseFocus(self);
    }
    return 0;
}
#endif

extern "C" void* func_00231D18(void* self);

//100%
INCLUDE_ASM("main/gameload", func_00231CB0__FPv);
#ifdef SKIP_ASM
void* func_00231CB0(void* self)
{
    return func_00231D18(self);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231CD0);
#ifdef SKIP_ASM
extern char D_0047D980[];

extern "C" void* func_00231CD0(void* self)
{
    *(void**)((char*)self + 0) = D_0047D980;
    *(float*)((char*)self + 4) = 0.20000000298023224f;
    *(int*)((char*)self + 8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/gameload", cFadeScreen_loseFocus);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_loseFocus(void* self)
{
    *(int*)((char*)self + 8) = 1;
    return *(float*)((char*)self + 4) >= 0.20000000298023224f;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231D18);
#ifdef SKIP_ASM
// PORT: the unit declares this void*-returning; the body returns nothing meaningful.
extern "C" void* func_00231D18(void* self)
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
INCLUDE_ASM("main/gameload", func_00231D60);
#ifdef SKIP_ASM
extern "C" void func_0038EC40(void* self, float* p0, float* p1, float* col, float z);
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

extern sRS231D60 D_00501420;

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

// Typed view of the render context (the unit declares D_004A289C as void*).
extern cCtx231D60* D_004A289C_231D60 __asm__("D_004A289C");

// PORT: `<?` (GNU min operator) gives the target's min.s
static inline float Clamp231D60(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

extern "C" int func_00231D60(void* self)
{
    if (*(float*)((char*)self + 4) > 0.0f) {
    cCtx231D60* ctx = D_004A289C_231D60;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *D_004A289C_231D60->top = D_00501420;
    D_004A289C_231D60->v26(0, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0);
    D_004A289C_231D60->v34(t4mat_Identity);
    D_004A289C_231D60->top->f8_5 = 31;
    D_004A289C_231D60->top->f4_23 = 2;
    D_004A289C_231D60->top->f4_20 = 0;
    D_004A289C_231D60->top->f4_12 = 0;
    float a = Clamp231D60(*(float*)((char*)self + 4) * 5.0f, 0.0f, 1.0f);
    float p0[2];
    float p1[2];
    float col[4];
    p0[0] = -1.0f;
    p0[1] = -1.0f;
    p1[0] = 1.0f;
    p1[1] = 1.0f;
    col[0] = a;
    col[1] = 0.0f;
    col[2] = 0.0f;
    col[3] = 0.0f;
    func_0038EC40((void*)D_004A28AC, p0, p1, col, 0.0f);
    *(int*)((char*)D_004A289C_231D60 + 0xE84) -= 0x14;
    return a >= 1.0f;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231F80);
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

extern "C" void func_00231F80(float* t)
{
    *t += 0.01666666753590107f;
    *t -= floorf_231F80(*t);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231FC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float func_0031C040(float);
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

extern sGs_1FC0 D_00501420_1FC0 __asm__("D_00501420");

class cGfx_1FC0 {
public:
    char pad[0xE84];
    sGs_1FC0* stateTop;
    char padE88[0x10D8 - 0xE88];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual void BeginScene();
    virtual void EndScene();
    virtual void v22(float, float, float, float, int);
    virtual void v23(); virtual void v24();
    virtual void SetViewport(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void Begin2D();
    virtual void End2D();
    virtual void v32();
    virtual void SetTexture(void*);
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

extern cGfx_1FC0* D_004A289C_1FC0 __asm__("D_004A289C");
extern char* D_004A28A8_1FC0 __asm__("SSXApp");

extern "C" void func_00231FC0(sFade_1FC0* self)
{
    cGfx_1FC0* g = D_004A289C_1FC0;
    g->stateTop[1] = g->stateTop[0];
    g->stateTop++;
    D_004A289C_1FC0->BeginScene();
    D_004A289C_1FC0->Begin2D();
    *D_004A289C_1FC0->stateTop = D_00501420_1FC0;
    D_004A289C_1FC0->stateTop->f4_23 = 2;
    D_004A289C_1FC0->stateTop->f4_20 = 0;
    D_004A289C_1FC0->stateTop->f4_12 = 0x14;
    D_004A289C_1FC0->stateTop->f4_2 = 5;
    D_004A289C_1FC0->stateTop->f8_5 = 0x1F;
    D_004A289C_1FC0->SetTexture(t4mat_Identity);
    {
        sVE_1FC0* vt = *(sVE_1FC0**)((char*)D_004A289C_1FC0 + 0x10D8);
        char* thisp = (char*)D_004A289C_1FC0 + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)D_004A289C_1FC0->GetWidth(), (float)D_004A289C_1FC0->GetHeight(), 1);
    }
    D_004A289C_1FC0->SetViewport(0.0f, 0.0f, 640.0f, 480.0f, 1, 0, 0.0f, 1.0f);
    float s = func_0031C040(self->t * 6.2831854820251465f);
    sV2_1FC0 size((s + 1.0f) * 9.0f, 18.0f);
    sV4_1FC0 pos(599.0f, 445.0f, 0.0f, 1.0f);
    int f10v = *(int*)(D_004A28A8_1FC0 + 0xA0);
    *(short*)((char*)D_004A289C_1FC0->stateTop + 0x10) = f10v;
    self->col.r = self->f4;
    sV2_1FC0 uv0(0.0f, 0.0f);
    sV2_1FC0 uv1(1.0f, 1.0f);
    D_004A289C_1FC0->DrawSprite(pos, size, uv0, uv1, self->col);
    D_004A289C_1FC0->End2D();
    D_004A289C_1FC0->EndScene();
    D_004A289C_1FC0->stateTop--;
}
#endif

extern void* D_0047D938[];

//100%
INCLUDE_ASM("main/gameload", func_00232328__FPv);
#ifdef SKIP_ASM
void* func_00232328(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = (int)(void*)D_0047D938;
    *(int*)((char*)self + 0x8) = t0;
    return self;
}
#endif

