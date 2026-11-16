#include "common.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_load);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_3438(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00397B08(void* mem);
extern "C" void cUIEngine_loadFile(void* engine, const char* name);
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" void func_00233640(void* self, int type);
extern "C" void cUIStateStack_push(void* list, void* screen);
extern int D_004A19C4;
extern signed char D_00535C11[];
extern char str_UIEngine_b[];
extern char str_data_ui_fl[];
extern char str_FLUIInput[];
extern char D_0047BFD0[];
extern char D_0047BFE8[];
extern char D_0047C000[];
extern void* cFEInput_vtbl[];
extern void* cFELoadStateInLodge_vtbl[];
extern void* cFELoadHintState_vtbl[];
extern void* cFELoadState_vtbl[];

struct sVE_233438 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sEngine_233438 {
    char pad0[0x8];
    int f8;
    void* fC;
    int f10;
};

struct sObj_233438 {
    int f0;
    char f4;
    void** vt;
    int fC;
    int f10;
    int f14;
    int f18;
};

static inline char* newScreenA_233438(void* self)
{
    char* p = (char*)cMemMan_alloc(0x48, D_0047BFD0, 0, 0);
    func_0039E2A0(p, *(void**)((char*)self + 0xC));
    *(void***)(p + 8) = cFELoadStateInLodge_vtbl;
    return p;
}

static inline char* newScreenB_233438(void* self)
{
    char* p = (char*)cMemMan_alloc(0x4C, D_0047BFE8, 0, 0);
    func_0039E2A0(p, *(void**)((char*)self + 0xC));
    *(int*)(p + 0x48) = 0;
    *(void***)(p + 8) = cFELoadHintState_vtbl;
    return p;
}

static inline char* newScreenC_233438(void* self)
{
    char* p = (char*)cMemMan_alloc(0x4C, D_0047C000, 0, 0);
    func_0039E2A0(p, *(void**)((char*)self + 0xC));
    *(int*)(p + 0x48) = 0;
    *(void***)(p + 8) = cFELoadState_vtbl;
    return p;
}

void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

/**
 * Loads the loading screen's UI and pushes the right loading state.
 *
 * Calls the graphics manager's slot 44 with 0x100, creates a UI engine loaded from "data/ui/fl" with the
 * app's fonts and locale and a cFEInput, then pushes cFELoadStateInLodge when D_004A19C4 is 0x27 (going to
 * the lodge), cFELoadHintState when results are not locked or the game path is 0, else cFELoadState; the last
 * two get this screen as parent and a texture (func_00233640 with 1 or 2). SplashTexID (+0x18) becomes -1.
 */
extern "C" void cFELoadScreen_load(void* self)
{
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVE_233438* vt = (sVE_233438*)ps2GraphicsMan->mVtbl;
        vt[44].fn((char*)ps2GraphicsMan + vt[44].delta, 0x100);
    }
    void* eng = func_00397B08(cMemMan_alloc(0x70, str_UIEngine_b, 0x100, 0));
    *(void**)((char*)self + 0xC) = eng;
    cUIEngine_loadFile(eng, str_data_ui_fl);
    (*(sEngine_233438**)((char*)self + 0xC))->f8 = (int)SSXApp->Fonts;
    (*(sEngine_233438**)((char*)self + 0xC))->f10 = (int)SSXApp->FELocale;
    {
        sObj_233438* feInput = new (str_FLUIInput, 0x100, 0) sObj_233438;
        feInput->f4 = 0;
        feInput->vt = cFEInput_vtbl;
        feInput->f18 = 0;
        feInput->fC = 0;
        feInput->f10 = 0;
        feInput->f14 = 0;
        (*(sEngine_233438**)((char*)self + 0xC))->fC = feInput;
    }
    char* gm = (char*)SSXApp->GameModeMan;
    cBE_getInterface_3438(cBE_getBE(), 0);
    char* screen;
    if (D_004A19C4 == 0x27) {
        screen = newScreenA_233438(self);
    } else if (*(int*)(gm + 0x9C) == 0 || D_00535C11[0] == 0) {
        screen = newScreenB_233438(self);
        *(void**)(screen + 0x48) = self;
        func_00233640(self, 1);
    } else {
        screen = newScreenC_233438(self);
        *(void**)(screen + 0x48) = self;
        func_00233640(self, 2);
    }
    cUIStateStack_push(*(char**)((char*)self + 0xC) + 0x18, screen);
    *(int*)((char*)self + 0x18) = -1;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233640);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerCharID(void* self, int idx);
int cBENewPlayerInterface_getRiderCharID(void* self, int idx);
extern "C" int cBENewPlayerInterface_getHumanRider(void* player, int idx);
struct cBigFile {
    int field_0x0;
    int field_0x4;
};
// PORT: cBigFile_cBigFile1 really takes (self, path, flags); bound by asm label.
cBigFile* cBigFile_cBigFile1_3640(cBigFile* self, const char* path, int flags) __asm__("cBigFile_cBigFile1__FP8cBigFile");
void cBigFile__cBigFile(cBigFile* self, int flags);
void cMemMan_free(void*);
extern "C" char* func_003E2190(const char* name, int flags);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_data_ui_charpic_big[];
extern char str_pct_spct_s_ssh[];
extern char str_CharaPics[];
extern char* D_00442928[];
extern int D_004A2B38;
extern int D_004A2B3C;


struct sVE_3640 {
    short delta;
    short index;
    int (*fn)(void*, void*, const char*, int, int, int);
};

static inline int loadTex_3640(char* data, const char* name)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_3640* vt = (sVE_3640*)ps2GraphicsMan->mVtbl;
    return vt[46].fn((char*)ps2GraphicsMan + vt[46].delta, data + *(int*)(data + 0x14), name, 0, 1, -1);
}

extern "C" void func_00233640(void* self, int type)
{
    char name[0x70];
    cBigFile bf;
    void* plr = cBE_getInterface_3438(cBE_getBE(), 1);
    cBE_getInterface_3438(cBE_getBE(), 0);
    cBigFile_cBigFile1_3640(&bf, str_data_ui_charpic_big, 0x100);
    if (type == 1) {
        const char* fmt = str_pct_spct_s_ssh;
        cBE_getInterface_3438(SSXApp->BE, 7);
        tBENetworkDB* opt = ((&cBENetworkInterface_networkOpt));
        if (opt->bIsNetworkGame != 0) {
            if (opt->bIsHost != 0) {
                sprintf(name, fmt, D_00442928[cBENewPlayerInterface_getRiderCharID(plr, 0)], D_004A2B38);
            } else {
                sprintf(name, fmt, D_00442928[cBENewPlayerInterface_getRiderCharID(plr, 1)], D_004A2B38);
            }
        } else {
            sprintf(name, fmt, D_00442928[cBENewPlayerInterface_getRiderCharID(plr, cBENewPlayerInterface_getHumanRider(plr, 0))], D_004A2B38);
        }
        char* data = func_003E2190(name, 0x3000100);
        *(int*)((char*)self + 0x10) = loadTex_3640(data, str_CharaPics);
        if (data != 0)
            cMemMan_free(data);
    } else if (type == 2 && D_00535C11[0] == 2) {
        int* slot = (int*)((char*)self + 0x10);
        for (int i = 0; i < 2; i++) {
            sprintf(name, str_pct_spct_s_ssh, D_00442928[cBENewPlayerInterface_getPlayerCharID(plr, i)], D_004A2B3C);
            char* data = func_003E2190(name, 0x3000100);
            *slot = loadTex_3640(data, str_CharaPics);
            if (data != 0)
                cMemMan_free(data);
            slot++;
        }
    }
    cBigFile__cBigFile(&bf, 2);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_002338C8);
#ifdef SKIP_ASM
extern int D_004A4324;
extern "C" void cGraphicsMan_UploadTextures(void* g);
extern "C" void cSSXApp_startFELoad(void* app);
extern "C" void* func_0028B180();
extern "C" void func_0028F768(void* snd);

struct sVec3i_38C8 {
    int x, y, z;
    sVec3i_38C8(int a, int b, int c) { x = a; y = b; z = c; }
};

struct sVE_38C8 {
    short delta;
    short index;
    void (*fn)(void*, sVec3i_38C8);
};

extern "C" void func_002338C8(void)
{
    cGraphicsMan_UploadTextures(GrMan);
    cSSXApp_startFELoad(SSXApp);
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* p = &D_004A4324;
    *p = 0;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_38C8* vt = (sVE_38C8*)ps2GraphicsMan->mVtbl;
    vt[15].fn((char*)ps2GraphicsMan + vt[15].delta, sVec3i_38C8(0, 0, 0));
    func_0028F768(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233930);
#ifdef SKIP_ASM
struct cGame00233930 {
    char pad[0x10D8];
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
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50(int id);
};

extern "C" void func_00397B70(void* self, int a1);
extern "C" void cGraphicsMan_PurgeTextures(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028FA98(void* self, float f);

extern "C" void func_00233930(void* self)
{
    ((cGame00233930*)GrMan)->v19();
    int id = *(int*)((char*)self + 0x18);
    if (id >= 0) {
        ((cGame00233930*)GrMan)->v50(id);
    }
    void* o = *(void**)((char*)self + 0xC);
    if (o != 0) {
        func_00397B70(o, 3);
    }
    int i;
    for (i = 0; i < 2; i++) {
        int h = *(int*)((char*)self + 0x10 + i * 4);
        if (h != 0) {
            ((cGame00233930*)GrMan)->v50(h);
        }
    }
    cGraphicsMan_PurgeTextures(GrMan);
    func_0028FA98(func_0028B180(), 1.0f);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_002339F8);
#ifdef SKIP_ASM
struct sVEntryI002339F8 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sVEntryV002339F8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_00231D60(void* self);
extern "C" void func_00397DF8(void* engine);

extern "C" int func_002339F8(void* self)
{
    char* g = (char*)GrMan;
    sVEntryI002339F8* vt = *(sVEntryI002339F8**)(g + 0x10D8);
    if (vt[17].fn(g + vt[17].delta) == 0) {
        return 0;
    }
    if (func_00231D60(self) == 0) {
        func_00397DF8(*(void**)((char*)self + 0xC));
    }
    g = (char*)GrMan;
    sVEntryV002339F8* vt2 = *(sVEntryV002339F8**)(g + 0x10D8);
    vt2[20].fn(g + vt2[20].delta);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233A70);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void func_00231CB0(void*);

extern "C" void func_00233A70(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    func_00231CB0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGameState_gotoNextState);
#ifdef SKIP_ASM
extern void* cGameState_mpGame;
extern "C" void func_00231250(void* self, int a, int b, int refresh);

/**
 * Switches the game to the state's next state (mNextState) with its startup flag, when one is set.
 */
extern "C" void cGameState_gotoNextState(void* self)
{
    int nextState = *(int*)self;
    if (nextState != 0) {
        func_00231250(cGameState_mpGame, nextState, *(int*)((char*)self + 4), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AD0);
#ifdef SKIP_ASM
extern "C" void func_00231278__FPvi(int);

extern "C" void func_00233AD0(void) {
    func_00231278__FPvi((*(int*)&cGameState_mpGame));
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AF0);
#ifdef SKIP_ASM
struct sVEntry_func_00233AF0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233AF0 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

void* func_00233AF0_2(void* self, void* obj)
{
    sVEntry_func_00233AF0* vt = *(sVEntry_func_00233AF0**)obj;
    return vt[1].fn((char*)obj + vt[1].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B28);
#ifdef SKIP_ASM
struct sVEntry_func_00233B28 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233B28 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

void* func_00233B28_2(void* self, void* obj)
{
    sVEntry_func_00233B28* vt = *(sVEntry_func_00233B28**)obj;
    return vt[2].fn((char*)obj + vt[2].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B60);
#ifdef SKIP_ASM
extern void* cGameState_mpGame;
extern "C" void func_00231250(void* self, int a, int b, int refresh);

extern "C" void func_00233B60(void)
{
    func_00231250(cGameState_mpGame, 0xA, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B88__FPv);
#ifdef SKIP_ASM
void func_00233B88(void* self)
{
}
#endif

extern "C" void* func_00233AF0(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233B90__FPv);
#ifdef SKIP_ASM
void* func_00233B90(void* self)
{
    return func_00233AF0(self);
}
#endif

extern "C" void* func_00233B28(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BB0__FPv);
#ifdef SKIP_ASM
void* func_00233BB0(void* self)
{
    return func_00233B28(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BD0__FPv);
#ifdef SKIP_ASM
void* func_00233BD0(void* self)
{
    return func_00233AF0(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BF0__FPv);
#ifdef SKIP_ASM
void* func_00233BF0(void* self)
{
    return func_00233B28(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233C10);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* D_0047D5E0[];
extern void* cGameState_mpGame;

extern "C" void* func_00233C10(void* self)
{
    *(int*)self = 0;
    *(void***)((char*)self + 0xC) = D_0047D6E8;
    cGameState_mpGame = 0;
    *(void***)((char*)self + 0xC) = D_0047D5E0;
    *(int*)self = 7;
    *(int*)((char*)self + 0x10) = 2;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233C50);
#ifdef SKIP_ASM
extern void* cGameState_mpGame;
extern "C" void cReplay_endReplay(void* self);
extern "C" void func_0039F840(void* list);
extern "C" void cSSXScriptEngine_ResetNISList(void* self, int i);
extern "C" void func_001F36C0(void* self, int bit);
void func_00244880(void* self);

extern "C" void func_00233C50(void* self)
{
    cReplay_endReplay(*(void**)((char*)cGameState_mpGame + 0x28));
    func_0039F840(*(char**)((char*)cGameState_mpGame + 0x48) + 0x18);
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0xB4;
    func_001F36C0(SSXApp->Game->LoadIndicator, 4);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        func_00244880(gameComm);
        gameComm->unk_090 = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233CD8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void* cBEAggressionInterface_getThis();
extern "C" void func_00155E58(void* p);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" void func_0020CC60();
extern "C" void cGameState_gotoNextState(void* self);
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern "C" void cSSXScriptEngine_ResetNISList(void* self, int i);
extern "C" int cSSXScriptEngine_GetNISListState(void* self, int i);
extern "C" int cSSXScriptEngine_GetNISListLength(void* self, int i);
extern "C" void cSSXScriptEngine_AppendEndEventSequence(void* self);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern int D_005366A8[];
// PORT: function-local `static const float` with a dynamic initializer (guard D_004A4F64),
// spelled with splat's globals; the const object's loads are loop-invariant (RTX_UNCHANGING).
extern const float D_004A4F60;
extern int D_004A4F64;
extern char str_finishov_b[];

struct sRace_233CD8 {
    char pad0[0x48];
    signed char mode48;
    signed char flag49;
};

static inline int isMode_233CD8(sRace_233CD8* r, int m)
{
    return r->mode48 == m;
}

struct sVec4_233CD8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_233CD8(const sVec4_233CD8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

struct sVEntry_233CD8 {
    short delta;
    short index;
    sVec4_233CD8* (*fn)(void*);
};

static inline sVec4_233CD8* riderPos_233CD8(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry_233CD8* vt = *(sVEntry_233CD8**)obj;
    return vt[2].fn(obj + vt[2].delta);
}

class cFEVObj_233CD8 {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
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
    virtual int v24(int on, int a2);
};


extern "C" void func_00233CD8(void* self)
{
    char* s = (char*)self;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (*(int*)(s + 0x14) != 0) {
        if (--*(int*)(s + 0x14) == 0) {
            int n = *(int*)SSXApp->Game->Replay;
            int ok = n != 0 && n < 10;
            if (!ok) {
                func_0020CC60();
                if (*(int*)(s + 0x10) != 2) {
                    cFEVObj_233CD8* o = (cFEVObj_233CD8*)cUIStateStack_findState((char*)SSXApp->Game->UI + 0x18, GetHashValue32(str_finishov_b));
                    if (o != 0) {
                        o->v24(0, 0);
                    }
                }
            }
        }
    }
    int state = *(int*)(s + 0x10);
    switch (state) {
    case 0: {
        char* g = (char*)SSXApp;
        char* x = *(char**)(g + 0xC0);
        char* r = *(char**)(*(char**)(g + 0x84) + 0xC);
        int cnt = *(int*)(r + 0x7C);
        if (D_005366A8[0] == 0) {
            for (int i = 0; i < cnt; i++) {
                if (D_004A4F64 == 0) {
                    *(float*)&D_004A4F60 = 277.77777099609375f;
                    D_004A4F64 = 1;
                }
                if (D_004A4F60 < vLen_233CD8(*riderPos_233CD8(*(char**)(((char**)(r + 0x40))[i] + 0x18)))) {
                    return;
                }
            }
        }
        sRace_233CD8* rc = &(*(sRace_233CD8*)&cBENewRaceInterface_newRace);
        int skip = !isMode_233CD8(rc, 4) && *(int*)(x + 0x98) != 0 && *(int*)(x + 0x9C) == 0;
        if (skip) {
            return;
        }
        cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
        cSSXScriptEngine_AppendEndEventSequence(ScriptEngine);
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
        if (cSSXScriptEngine_GetNISListLength(ScriptEngine, 0) > 0) {
            *(int*)s = 0xC;
        }
        sRace_233CD8* rc2 = &(*(sRace_233CD8*)&cBENewRaceInterface_newRace);
        if (rc2->mode48 == 0) {
            func_00155E58(cBEAggressionInterface_getThis());
        }
        *(int*)(s + 0x10) = 1;
        break;
    }
    case 1: {
        if (cSSXScriptEngine_GetNISListState(ScriptEngine, 0) == 1) {
            break;
        }
        cFEVObj_233CD8* o = (cFEVObj_233CD8*)cUIStateStack_findState((char*)SSXApp->Game->UI + 0x18, GetHashValue32(str_finishov_b));
        if (o != 0) {
            o->v24(1, 0);
        }
        cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 4);
        *(int*)(s + 0x10) = 2;
        break;
    }
    case 2: {
        sRace_233CD8* rc = &(*(sRace_233CD8*)&cBENewRaceInterface_newRace);
        if (rc->flag49 == 0 && rc->mode48 == 4) {
            cGameState_gotoNextState(self);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234008);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sLoadScreen00234008 {
    int state;
    int pad4[3];
    int mode;
};
void func_00162290_v(void* self) __asm__("func_00162290__FPv");
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);
void func_00278DE8(void* self, int val);

extern "C" void func_00234008(sLoadScreen00234008* self)
{
    int n = *(int*)((char*)SSXApp->Game->ViewMan + 0x10);
    int i;
    for (i = 0; i < n; i++) {
        char* list = (char*)SSXApp->Game->ViewMan;
        char* item = *(char**)(list + (i << 2) + 4);
        func_00162290_v(*(void**)(item + 0xA8));
    }
    self->mode = 2;
    cSSXScriptEngine_PlayNISList(ScriptEngine, 0, 1, 0);
    func_00278DE8(ScriptEngine, 1);
    self->state = 7;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_002340B8);
#ifdef SKIP_ASM
struct sVEntry_func_002340B8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_002340B8(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_002340B8* e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234120);
#ifdef SKIP_ASM
struct sVEntry_func_00234120 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234120(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00234120* e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234188);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* cGFEventStartState_vtbl[];
extern void* cGameState_mpGame;

extern "C" void* func_00234188(void* self)
{
    *(int*)self = 0;
    *(void***)((char*)self + 0xC) = D_0047D6E8;
    cGameState_mpGame = 0;
    *(void***)((char*)self + 0xC) = cGFEventStartState_vtbl;
    *(int*)self = 2;
    *(int*)((char*)self + 0x1C) = 4;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGFEventStartState_gainFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_41D0(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void cAI_setAIState(void*, int);
extern "C" void cBXLoadIndicator_Hide(void* hud, int a);
extern "C" void cReplay_reset(void* p);
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);
extern "C" void cBENewPlayerInterface_setRiderCharID(void* player, int i, int id);
extern "C" void func_001473D0(void* player, int i, int v);
extern "C" void cSSXScriptEngine_ResetNISList(void* self, int i);
extern "C" void cSSXScriptEngine_AppendStartSequence(void* a, int b);
// PORT: cSSXScriptEngine_ReadyNISList returns func_00276270's result (ssxscriptengine defines it void).
extern "C" int cSSXScriptEngine_ReadyNISList(void* self, int i);
extern void* cGameState_mpGame;
extern signed char D_00535C10[];

struct sVE_2341D0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sGM_2341D0 {
    char pad0[0x10];
    int total;      // 0x10
    int humans;     // 0x14
    int chars[10];  // 0x18
    int boards[10]; // 0x40
};

static inline int isMode4_2341D0()
{
    return D_00535C10[0] == 4;
}

/**
 * Prepares an event start: resets the AI, BE state and replay and copies the game mode's AI riders into the
 * race.
 *
 * Sets the AI state to 3, hides the load indicator, sets BE state 0 and resets the replay. The AI count is the
 * game mode manager's rider total minus its humans; each AI rider from the AI's first index (its word at 0x7C)
 * gets the mode's character id and a second per-rider value (func_001473D0, probably the board), and both
 * interfaces are submitted. Mode 2 queues start sequence 1, mode 1 sequence 0 (not when the race mode byte
 * D_00535C10 is 4), mode 0 none. Finally clears the word at 0x1C.
 *
 * @param mode  startup flag (On Tour: startupFlag): 0 no start sequence, 1 or 2 as above
 */
extern "C" void cGFEventStartState_gainFocus(void* self, int mode)
{
    int i = 0;
    char* race = (char*)cBE_getInterface_41D0(cBE_getBE(), 0);
    char* player = (char*)cBE_getInterface_41D0(cBE_getBE(), 1);
    cAI_setAIState(*(void**)((char*)cGameState_mpGame + 0xC), 3);
    cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 0);
    cBE_setState(0);
    sGM_2341D0* gm = (sGM_2341D0*)SSXApp->GameModeMan;
    cReplay_reset(*(void**)((char*)cGameState_mpGame + 0x28));
    int n = gm->total - gm->humans;
    int base = *(int*)(*(char**)((char*)cGameState_mpGame + 0xC) + 0x7C);
    cBENewRaceInterface_setNumberAI(race, n);
    {
        sVE_2341D0* vt = *(sVE_2341D0**)(race + 0xC);
        vt[1].fn(race + vt[1].delta);
    }
    for (; i < n; i++) {
        cBENewPlayerInterface_setRiderCharID(player, i + base, gm->chars[base + i]);
        func_001473D0(player, i + base, gm->boards[base + i]);
    }
    {
        sVE_2341D0* vt = *(sVE_2341D0**)(player + 0xC);
        vt[1].fn(player + vt[1].delta);
    }
    switch (mode) {
    case 0:
        break;
    case 2:
        cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
        cSSXScriptEngine_AppendStartSequence(ScriptEngine, 1);
        cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
        break;
    case 1:
        if (!isMode4_2341D0()) {
            cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
            cSSXScriptEngine_AppendStartSequence(ScriptEngine, 0);
            cSSXScriptEngine_ReadyNISList(ScriptEngine, 0);
        }
        break;
    }
    *(int*)((char*)self + 0x1C) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGFEventStartState_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001F36C0(void* self, int bit);
extern "C" void cBXLoadIndicator_Hide(void* self, int bit);
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* snd, int a1);
extern "C" void cSSXScriptEngine_PauseNISList(void* snd, int a1, int a2);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* snd);
extern "C" int cSSXScriptEngine_GetNISListState(void* snd, int a1);
extern "C" int cSSXScriptEngine_PlayNISList(void* snd, int a1, int a2, int a3);
extern "C" int cSSXScriptEngine_GetNISListScriptCategory(void* snd, int a1, int a2);
extern "C" void func_00234910(void* self);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void func_0012AB20(void* o);
extern "C" void func_0012AC48(void* o);
extern "C" void cAI_initAIRiders(void* o);
extern "C" void cAI_initAIActors(void* o, int a1);
extern "C" void cAI_setRaceRankType(void* o, int a1);
extern "C" int cAI_isAILoadedAndReady(void* o);
extern "C" void cAI_loadRiders(void* o);
void func_001289F0(void* o);
extern "C" int cAI_isReadyToPurgeComputerRiders(void* o);
extern "C" int cAI_isReadyToPurgeComputerActors(void* o);
extern "C" void cAI_cleanupAfterRace(void* o);
extern "C" void cAI_cleanupActors(void* o);
extern void* cGameState_mpGame;
extern signed char D_00535C10[];

struct sLoadObj_43B0 {
    char pad0[0x14];
    int f14;                        // 0x14
    char pad18[0x80 - 0x18];
    int f80;                        // 0x80
    int f84;                        // 0x84
    char pad88[0x4];
    int f8C;                        // 0x8C
};

struct sLoadMgr_43B0 {
    char pad0[0xC];
    sLoadObj_43B0* obj;             // 0xC
};

struct sLoadScreen_43B0 {
    char pad0[0x10];
    int pending;                    // 0x10
    int f14;                        // 0x14
    char pad18[0x4];
    int state;                      // 0x1C
};

#define MGR_43B0 ((sLoadMgr_43B0*)cGameState_mpGame)

static inline void* hud_43B0()
{
    return SSXApp->Game->LoadIndicator;
}

/**
 * Steps the event-start state: readies the event, sets up and loads the AI riders, then starts the event.
 *
 * The load indicator shows (and NIS list 0 pauses) until sub-state 3. Sub-state 0 waits for NIS transitions;
 * 1 readies the event, then either prepares to purge the computer riders and actors (game mode 4, mode 5
 * unless event 4, mode 6 unless event 5) or, on game path 0, initialises the AI riders (and the actors when
 * no computers are allocated), and sets the rank type; 2 loads riders until the AI is ready; 3 waits for the
 * NIS lists, activates the computers, purges if pending and goes to the next game state (sub-state 4).
 */
// Load screen update: drives the loading state machine (stream start, track setup, wait for load, finish).
extern "C" void cGFEventStartState_update(sLoadScreen_43B0* self)
{
    if (self->state < 3) {
        func_001F36C0(hud_43B0(), 2);
        if (cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 0) == 4)
            cSSXScriptEngine_PauseNISList(ScriptEngine, 0, 1);
    } else {
        if (cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 0) == 3)
            cSSXScriptEngine_PauseNISList(ScriptEngine, 0, 0);
        cBXLoadIndicator_Hide(hud_43B0(), 2);
    }
    switch (self->state) {
    case 0:
        if (cSSXScriptEngine_IsActiveNISInTransition(ScriptEngine) == 0)
            self->state = 1;
        break;
    case 1: {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        func_00234910(self);
        signed char* cfg = ((signed char*)&cBENewRaceInterface_newRace);
        int mode = cfg[0x48];
        int skip = cfg[0x49];
        int sub = cfg[0x4A];
        if (mode == 4 || (mode == 5 && sub != 4) || (mode == 6 && sub != 5)) {
            func_0012AB20(MGR_43B0->obj);
            func_0012AC48(MGR_43B0->obj);
            self->pending = 1;
        } else if (skip == 0) {
            cAI_initAIRiders(MGR_43B0->obj);
            sLoadObj_43B0* aiObj = MGR_43B0->obj;
            if (aiObj->f80 == 0) {
                if (mode == 0)
                    cAI_initAIActors(aiObj, 0);
                else
                    cAI_initAIActors(aiObj, 2);
            }
            MGR_43B0->obj->f8C = 0;
        }
        cAI_setRaceRankType(MGR_43B0->obj, 0);
        if (mode == 5 || mode == 0)
            cAI_setRaceRankType(MGR_43B0->obj, 1);
        else if (mode != 4)
            cAI_setRaceRankType(MGR_43B0->obj, 2);
        self->state = 2;
        break;
    }
    case 2:
        if (!cAI_isAILoadedAndReady(MGR_43B0->obj))
            cAI_loadRiders(MGR_43B0->obj);
        else
            self->state = 3;
        break;
    case 3: {
        if (cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 1) != 0)
            break;
        if (MGR_43B0->obj->f84 > 0)
            MGR_43B0->obj->f8C = 1;
        if (MGR_43B0->obj->f80 > 0)
            MGR_43B0->obj->f14 = 1;
        if (cSSXScriptEngine_GetNISListState(ScriptEngine, 0) == 2) {
            cSSXScriptEngine_PlayNISList(ScriptEngine, 0, self->f14 == 0, 1);
            break;
        }
        int nisState = cSSXScriptEngine_GetNISListPlayState(ScriptEngine, 0);
        int scriptCategory = cSSXScriptEngine_GetNISListScriptCategory(ScriptEngine, 0, 0);
        int busy = nisState && scriptCategory != 5;
        if (busy)
            break;
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C10[0] != 4) {
            func_001289F0(MGR_43B0->obj);
            if (MGR_43B0->obj->f80 > 0)
                MGR_43B0->obj->f14 = 1;
        }
        if (self->pending) {
            if (!cAI_isReadyToPurgeComputerRiders(MGR_43B0->obj))
                break;
            if (!cAI_isReadyToPurgeComputerActors(MGR_43B0->obj))
                break;
            cAI_cleanupAfterRace(MGR_43B0->obj);
            cAI_cleanupActors(MGR_43B0->obj);
            self->pending = 0;
        }
        self->state = 4;
        cGameState_gotoNextState(self);
        break;
    }
    case 4:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGFEventStartState_loseFocus);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_4750(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBE_setState(int state);
extern "C" void cAI_setAIState(void*, int);
void* cBEAggressionInterface_getThis();
extern "C" void cSSXScriptEngine_ResetNISList(void* self, int i);
extern "C" void cAI_initAIRiders(void* p);
void func_001289F0(void* p);
extern "C" void cAI_initFreeride(void* p);
extern "C" void cReplay_startRace(void* p);
extern "C" void cAI_setRaceRankType(void* p, int a);
extern "C" void func_00308F38(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern "C" void func_00129160(void* p);
extern "C" void func_00155E58(void* p);
extern void* cGameState_mpGame;
extern void* D_004A3DD8;


struct sRaceMan_234750 {
    char pad0[0x8C];
    int f8C;
};

struct sVE_234750 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sTbl_234750 {
    char pad[0x250];
    struct { int f0; char pad[0x4C]; } slot[2];
};

static inline int otherSide_234750(char* p)
{
    return *(int*)(p + 0xB0) == 0;
}

/**
 * Starts the event as the event-start state ends: initialises the AI and missions and enters the race state.
 *
 * Resets NIS list 1; game mode 0 initialises the AI riders, other modes cAI_initFreeride() and the replay's
 * race start. Outside mode 4 the computer riders are made active. Sets the rank type, missions
 * (cWScriptMan_setupMissions()), AI state 3 and back-end state 1, refreshes the riders' animation, re-clamps
 * aggression on game paths other than 0 and clears mHasTransitionNIS (+0x14). When the view manager's word at
 * +0x10 is 1 it also clears a word in the world's slot for the other view.
 */
extern "C" void cGFEventStartState_loseFocus(void* self)
{
    cSSXScriptEngine_ResetNISList(ScriptEngine, 1);
    cBE_getInterface_4750(cBE_getBE(), 0);
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int mode = beRaceDB->cGameMode;
    if (mode == 0) {
        cAI_initAIRiders(*(void**)((char*)cGameState_mpGame + 0xC));
        func_001289F0(*(void**)((char*)cGameState_mpGame + 0xC));
    } else {
        cAI_initFreeride(*(void**)((char*)cGameState_mpGame + 0xC));
        cReplay_startRace(*(void**)((char*)cGameState_mpGame + 0x28));
    }
    if (mode != 4) {
        char* aiMan = *(char**)((char*)cGameState_mpGame + 0xC);
        sVE_234750* vt = *(sVE_234750**)(aiMan + 0xCC);
        vt[5].fn(aiMan + vt[5].delta, 3);
        *(int*)(*(char**)((char*)cGameState_mpGame + 0xC) + 0x14) = 1;
    }
    cAI_setRaceRankType(*(void**)((char*)cGameState_mpGame + 0xC), 0);
    if (mode == 5 || mode == 0)
        cAI_setRaceRankType(*(void**)((char*)cGameState_mpGame + 0xC), 1);
    else if (mode != 4)
        cAI_setRaceRankType(*(void**)((char*)cGameState_mpGame + 0xC), 2);
    func_00308F38(D_004A3DD8);
    cWScriptMan_setupMissions(D_004A3DD8);
    (*(sRaceMan_234750**)((char*)cGameState_mpGame + 0xC))->f8C = 0;
    cAI_setAIState(*(void**)((char*)cGameState_mpGame + 0xC), 3);
    cBE_setState(1);
    func_00129160(*(void**)((char*)cGameState_mpGame + 0xC));
    cBERaceDB* raceDB = &(*(&cBENewRaceInterface_newRace));
    if (raceDB->cGamePath != 0)
        func_00155E58(cBEAggressionInterface_getThis());
    *(int*)((char*)self + 0x14) = 0;
    char* game = (char*)SSXApp->Game;
    char* x = *(char**)(game + 0x84);
    if (*(int*)(x + 0x10) == 1) {
        sTbl_234750* slotTable = **(sTbl_234750***)(game + 0x10);
        slotTable->slot[otherSide_234750(*(char**)(x + 4))].f0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234910);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_PlayNISList(void* self, int i, int a, int b);

static inline int isState00234910(void* e, int s)
{
    return *(int*)((char*)e + 0x550) == s;
}

extern "C" void func_00234910(void* self)
{
    if (isState00234910(ScriptEngine, 2)) {
        cSSXScriptEngine_PlayNISList(ScriptEngine, 1, 1, 1);
    }
    if (isState00234910(ScriptEngine, 2)) {
        cSSXScriptEngine_PlayNISList(ScriptEngine, 0, 1, 1);
    }
    *(int*)((char*)self + 0x14) = 0;
    if (ScriptEngine->mPlayingNISList == 1) {
        *(int*)((char*)self + 0x14) = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234990);
#ifdef SKIP_ASM
// PORT: the unit declares func_00233AF0 as `void* (void*)`; the body also uses $a1 (obj).
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

struct sVEntry_func_00234990 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234990(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_00234990* e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x1C, 4);
    e = &(*(sVEntry_func_00234990**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234A30);
#ifdef SKIP_ASM
// PORT: the unit declares func_00233B28 as `void* (void*)`; the body also uses $a1 (obj).
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

struct sVEntry_func_00234A30 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234A30(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00234A30* e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x1C, 4);
    e = &(*(sVEntry_func_00234A30**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x8, 4);
}
#endif

