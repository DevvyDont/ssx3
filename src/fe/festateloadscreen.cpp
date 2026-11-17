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
extern int cRealFE_mStartState;
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
 * app's fonts and locale and a cFEInput, then pushes cFELoadStateInLodge when cRealFE_mStartState is 0x27 (going to
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
    if (cRealFE_mStartState == 39) {
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
extern "C" char* FILE_loadpackz(const char* name, int flags);
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

/**
 * Loads the character portrait textures for the load screen from "data/ui/charpic.big".
 *
 * Opens the big file (cBigFile_cBigFile1, flags 0x100) for the duration. Type 1 loads one portrait into +0x10: in a
 * network game (cBENetworkInterface_networkOpt.bIsNetworkGame) rider 0's character when hosting (bIsHost) and rider 1's
 * otherwise, else the first human rider's character (cBENewPlayerInterface_getRiderCharID()). Type 2, only when the
 * game path (cGamePath, D_00535C11) is 2 (online), loads players 0 and 1's characters
 * (cBENewPlayerInterface_getPlayerCharID()) into +0x10 and +0x14. Each file is "%s%s.ssh" of the character's name
 * D_00442928[id] and a suffix (D_004A2B38, "_flat", for type 1; D_004A2B3C for type 2), loaded with FILE_loadpackz()
 * and uploaded through the graphics manager's slot 46 (func_0037C8C0) as "CharaPics"; the file buffer is then freed.
 * cFELoadScreen_load() calls it with 1 or 2 depending on the screen it pushes.
 *
 * @param type  1 for the single-player portrait, 2 for the two online players' portraits
 */
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
        char* data = FILE_loadpackz(name, 0x3000100);
        *(int*)((char*)self + 0x10) = loadTex_3640(data, str_CharaPics);
        if (data != 0)
            cMemMan_free(data);
    } else if (type == 2 && D_00535C11[0] == 2) {
        int* slot = (int*)((char*)self + 0x10);
        for (int i = 0; i < 2; i++) {
            sprintf(name, str_pct_spct_s_ssh, D_00442928[cBENewPlayerInterface_getPlayerCharID(plr, i)], D_004A2B3C);
            char* data = FILE_loadpackz(name, 0x3000100);
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
INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_gainFocus);
#ifdef SKIP_ASM
extern int cDepthFog_m_Enable;
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

/**
 * Starts loading the front end when the load screen gains focus.
 *
 * Uploads the textures (cGraphicsMan_UploadTextures()), starts the front-end load (cSSXApp_startFELoad()), turns depth
 * fog off, sets the graphics manager's clear colour to black (slot 15 with (0, 0, 0)) and calls func_0028F768 on the
 * audio object. The object itself is not used, so the C takes no arguments.
 */
extern "C" void cFELoadScreen_gainFocus(void)
{
    cGraphicsMan_UploadTextures(GrMan);
    cSSXApp_startFELoad(SSXApp);
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* fogEnable = &cDepthFog_m_Enable;
    *fogEnable = 0;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_38C8* vt = (sVE_38C8*)ps2GraphicsMan->mVtbl;
    vt[15].fn((char*)ps2GraphicsMan + vt[15].delta, sVec3i_38C8(0, 0, 0));
    func_0028F768(func_0028B180());
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_purge);
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

/**
 * Unloads the front-end load screen's textures and UI object.
 *
 * Flushes the renderer (graphics manager slot 19), removes the texture id at +0x18 when it is >= 0 (slot 50,
 * RemoveTexture), destroys the object at +0xC (func_00397B70 with flags 3), removes the texture ids at +0x10 and +0x14
 * when non-zero, purges the textures (cGraphicsMan_PurgeTextures()) and calls func_0028FA98 on the audio object with
 * 1.0.
 */
extern "C" void cFELoadScreen_purge(void* self)
{
    ((cGame00233930*)GrMan)->v19();
    int id = *(int*)((char*)self + 0x18);
    if (id >= 0) {
        ((cGame00233930*)GrMan)->v50(id);
    }
    void* uiObject = *(void**)((char*)self + 0xC);
    if (uiObject != 0) {
        func_00397B70(uiObject, 3);
    }
    int i;
    for (i = 0; i < 2; i++) {
        int textureId = *(int*)((char*)self + 0x10 + i * 4);
        if (textureId != 0) {
            ((cGame00233930*)GrMan)->v50(textureId);
        }
    }
    cGraphicsMan_PurgeTextures(GrMan);
    func_0028FA98(func_0028B180(), 1.0f);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_render);
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

extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void func_00397DF8(void* engine);

/**
 * Draws the front-end load screen.
 *
 * Returns 0 when the graphics manager's slot 17 (IsRenderReady) says no. Unless cFadeScreen_drawFade (the base fade) draws
 * this frame, the UI object at +0xC is drawn (func_00397DF8); then the frame is flipped (slot 20) and 1 returned.
 *
 * @return 1 when a frame was drawn, 0 when the renderer was not ready
 */
extern "C" int cFELoadScreen_render(void* self)
{
    char* graphicsMan = (char*)GrMan;
    sVEntryI002339F8* vt = *(sVEntryI002339F8**)(graphicsMan + 0x10D8);
    if (vt[17].fn(graphicsMan + vt[17].delta) == 0) {
        return 0;
    }
    if (cFadeScreen_drawFade(self) == 0) {
        func_00397DF8(*(void**)((char*)self + 0xC));
    }
    graphicsMan = (char*)GrMan;
    sVEntryV002339F8* vt2 = *(sVEntryV002339F8**)(graphicsMan + 0x10D8);
    vt2[20].fn(graphicsMan + vt2[20].delta);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_update);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void cLoadScreen_update(void*);

/**
 * Per-frame update of the front-end load screen: updates its UI object (+0xC, func_00398038), then runs the base
 * update cLoadScreen_update (which calls cFadeScreen_update()).
 */
extern "C" void cFELoadScreen_update(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    cLoadScreen_update(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGameState_gotoNextState);
#ifdef SKIP_ASM
extern void* cGameState_mpGame;
extern "C" void cGame_setState(void* self, int a, int b, int refresh);

/**
 * Switches the game to the state's next state (mNextState) with its startup flag, when one is set.
 */
extern "C" void cGameState_gotoNextState(void* self)
{
    int nextState = *(int*)self;
    if (nextState != 0) {
        cGame_setState(cGameState_mpGame, nextState, *(int*)((char*)self + 4), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AD0);
#ifdef SKIP_ASM
extern "C" void cGame_setPreloadState__FPvi(int);

/**
 * Asks the game to preload a game-flow state: calls cGame_setPreloadState() on cGameState_mpGame.
 *
 * The state id passes through untouched in the second argument register (the declaration has no parameters).
 */
extern "C" void func_00233AD0(void) {
    cGame_setPreloadState__FPvi((*(int*)&cGameState_mpGame));
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGameState_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry_cGameState_writeToReplayFrame {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares cGameState_writeToReplayFrame as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* cGameState_writeToReplayFrame_2(void* self, void* obj) __asm__("cGameState_writeToReplayFrame");

/**
 * Saves the base game-state data into a replay frame: the state's first word.
 *
 * Writes 4 bytes from @p self (mNextState, +0x0) through the stream's virtual slot 1 (write). Every game-flow state's
 * own writeToReplayFrame calls this (cGFEndState_writeToReplayFrame__FPv, cGFGateState_writeToReplayFrame__FPv, ...).
 *
 * @param obj  the replay frame stream (On Tour cReplayFramePtr)
 * @return the stream write's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGameState_writeToReplayFrame_2(void* self, void* obj)
{
    sVEntry_cGameState_writeToReplayFrame* vt = *(sVEntry_cGameState_writeToReplayFrame**)obj;
    return vt[1].fn((char*)obj + vt[1].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", cGameState_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry_cGameState_readFromReplayFrame {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares cGameState_readFromReplayFrame as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* cGameState_readFromReplayFrame_2(void* self, void* obj) __asm__("cGameState_readFromReplayFrame");

/**
 * Restores the base game-state data from a replay frame: the state's first word.
 *
 * Reads 4 bytes into @p self (mNextState, +0x0) through the stream's virtual slot 2 (read); the mirror of
 * cGameState_writeToReplayFrame.
 *
 * @param obj  the replay frame stream (On Tour cReplayFramePtr)
 * @return the stream read's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGameState_readFromReplayFrame_2(void* self, void* obj)
{
    sVEntry_cGameState_readFromReplayFrame* vt = *(sVEntry_cGameState_readFromReplayFrame**)obj;
    return vt[2].fn((char*)obj + vt[2].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B60);
#ifdef SKIP_ASM
extern void* cGameState_mpGame;
extern "C" void cGame_setState(void* self, int a, int b, int refresh);

/**
 * Switches the game to game-flow state 10 at its next state update: cGame_setState(cGameState_mpGame, 10, 0, 0).
 *
 * It is the gainFocus slot of vtable D_0047D690, the game-flow state func_00243D20 constructs (mNextState 8).
 */
extern "C" void func_00233B60(void)
{
    cGame_setState(cGameState_mpGame, 10, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B88__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the loseFocus slot of vtable D_0047D690.
 */
void func_00233B88(void* self)
{
}
#endif

extern "C" void* cGameState_writeToReplayFrame(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", cGFEndState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the end game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFEndState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

extern "C" void* cGameState_readFromReplayFrame(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", cGFEndState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the end game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFEndState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", cGFEndWaitState_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Saves the end-wait game-flow state into a replay frame: only the base game-state part.
 *
 * Calls cGameState_writeToReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_writeToReplayFrame's result, passed through (On Tour's writeToReplayFrame returns nothing)
 */
void* cGFEndWaitState_writeToReplayFrame(void* self)
{
    return cGameState_writeToReplayFrame(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", cGFEndWaitState_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Restores the end-wait game-flow state from a replay frame: only the base game-state part.
 *
 * Calls cGameState_readFromReplayFrame; the replay frame stream passes through in the second argument register, which
 * this declaration does not name.
 *
 * @return cGameState_readFromReplayFrame's result, passed through (On Tour's readFromReplayFrame returns nothing)
 */
void* cGFEndWaitState_readFromReplayFrame(void* self)
{
    return cGameState_readFromReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233C10);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* D_0047D5E0[];
extern void* cGameState_mpGame;

/**
 * Constructs the finish-line game-flow state in place: base cGameState, vtable D_0047D5E0, next state 7.
 *
 * Runs the inlined cGameState constructor (mNextState 0, vtable D_0047D6E8 at +0xC, cGameState_mpGame cleared), then
 * stores D_0047D5E0, mNextState (+0x0) 7, mStartupFlag (+0x4) 0, the sub-state (+0x10) 2 and the results delay (+0x14)
 * 0. cGame_cGame() builds it at +0x100 of the game.
 *
 * @return @p self
 */
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
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cSSXScriptEngine_ResetNISList(void* self, int i);
extern "C" void cBXLoadIndicator_Show(void* self, int bit);
void func_00244880(void* self);

/**
 * Starts the finish-line state: ends the replay, clears the UI states and NIS list, and starts the 180-frame results
 * delay.
 *
 * Calls cReplay_endReplay() on the game's Replay, cUIStateStack_purgeNormalStates() on the game UI's state stack and
 * cSSXScriptEngine_ResetNISList() for list 0, sets the sub-state (+0x10) to 0 and the delay (+0x14) to 180, shows load
 * indicator bit 4 (cBXLoadIndicator_Show()) and, with a cGameComm_mGameComm, calls func_00244880() on it and sets its
 * unk_090 to 1. gainFocus slot of vtable D_0047D5E0.
 */
extern "C" void func_00233C50(void* self)
{
    cReplay_endReplay(*(void**)((char*)cGameState_mpGame + 0x28));
    cUIStateStack_purgeNormalStates(*(char**)((char*)cGameState_mpGame + 0x48) + 0x18);
    cSSXScriptEngine_ResetNISList(ScriptEngine, 0);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 180;
    cBXLoadIndicator_Show(SSXApp->Game->LoadIndicator, 4);
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


/**
 * Runs the finish-line state each frame: counts down the results delay, then plays the end-of-event NIS and moves on
 * when the race says so.
 *
 * When the delay (+0x14) reaches 0 and no replay is running (the Replay's first word is 0 or 10 or more), it calls
 * func_0020CC60() and, unless the sub-state (+0x10) is already 2, handleEvent (slot 24) with (0, 0) on the "finishov"
 * UI state. Sub-state 0: unless D_005366A8[0] is set, waits while any AI rider's vector from slot 2 of its +0x6C0
 * object is longer than 277.78 (a function-local static float); also waits while the race mode is not 4 and the game
 * mode manager has +0x98 set and +0x9C clear. Then it builds and readies NIS list 0 with the end-event sequence
 * (cSSXScriptEngine_AppendEndEventSequence()), sets mNextState (+0x0) to 12 when the list is not empty, calls
 * func_00155E58() on the aggression interface in race mode 0, and moves to sub-state 1. Sub-state 1: once the NIS list
 * is no longer playing (state 1), calls "finishov"'s handleEvent with (1, 0), hides load indicator bit 4 and moves to
 * 2. Sub-state 2: calls cGameState_gotoNextState() when the race's mode (+0x48) is 4 and its +0x49 flag is clear.
 * update slot of vtable D_0047D5E0.
 */
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

/**
 * Leaves the finish-line state: updates every game view's camera, starts NIS list 0 and sets the next state to 7.
 *
 * For each view of the game's ViewMan (count at +0x10) calls func_00162290() on the view's +0xA8 object, sets the
 * sub-state (+0x10) to 2, calls cSSXScriptEngine_PlayNISList(ScriptEngine, 0, 1, 0) and func_00278DE8(ScriptEngine, 1),
 * and stores mNextState (+0x0) 7. loseFocus slot of vtable D_0047D5E0.
 */
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
INCLUDE_ASM("fe/festateloadscreen", cGFFinishlineState_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry_cGFFinishlineState_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Finish-line state: saves its replay data.
 *
 * Writes the base game-state part (cGameState_writeToReplayFrame), then the words at +0x10 and +0x14 through the stream's
 * virtual slot 1 (write).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFFinishlineState_writeToReplayFrame(void* self, void* stream)
{
    cGameState_writeToReplayFrame_2(self, stream);
    sVEntry_cGFFinishlineState_writeToReplayFrame* writeEntry = &(*(sVEntry_cGFFinishlineState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x10, 4);
    writeEntry = &(*(sVEntry_cGFFinishlineState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x14, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", cGFFinishlineState_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry_cGFFinishlineState_readFromReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Finish-line state: restores its replay data.
 *
 * Reads the base game-state part (cGameState_readFromReplayFrame), then the words at +0x10 and +0x14 through the stream's
 * virtual slot 2 (read).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFFinishlineState_readFromReplayFrame(void* self, void* stream)
{
    cGameState_readFromReplayFrame_2(self, stream);
    sVEntry_cGFFinishlineState_readFromReplayFrame* readEntry = &(*(sVEntry_cGFFinishlineState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x10, 4);
    readEntry = &(*(sVEntry_cGFFinishlineState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x14, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00234188);
#ifdef SKIP_ASM
extern void* D_0047D6E8[];
extern void* cGFEventStartState_vtbl[];
extern void* cGameState_mpGame;

/**
 * Constructs the event-start game-flow state in place: base cGameState, cGFEventStartState_vtbl, next state 2.
 *
 * Runs the inlined cGameState constructor (vtable D_0047D6E8, cGameState_mpGame cleared), then stores
 * cGFEventStartState_vtbl, mNextState (+0x0) 2, the sub-state (+0x14) 0, +0x1C 4 and clears mStartupFlag (+0x4), the
 * bytes at +0x10 and +0x8. cGame_cGame() builds it at +0xB0 of the game.
 *
 * @return @p self
 */
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
extern "C" void cBENewPlayerInterface_setRiderCheatCharID(void* player, int i, int v);
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
 * gets the mode's character id and a second per-rider value (cBENewPlayerInterface_setRiderCheatCharID, probably the board), and both
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
        cBENewPlayerInterface_setRiderCheatCharID(player, i + base, gm->boards[base + i]);
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
extern "C" void cBXLoadIndicator_Show(void* self, int bit);
extern "C" void cBXLoadIndicator_Hide(void* self, int bit);
extern "C" int cSSXScriptEngine_GetNISListPlayState(void* snd, int a1);
extern "C" void cSSXScriptEngine_PauseNISList(void* snd, int a1, int a2);
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(void* snd);
extern "C" int cSSXScriptEngine_GetNISListState(void* snd, int a1);
extern "C" int cSSXScriptEngine_PlayNISList(void* snd, int a1, int a2, int a3);
extern "C" int cSSXScriptEngine_GetNISListScriptCategory(void* snd, int a1, int a2);
extern "C" void func_00234910(void* self);
extern "C" void cGameState_gotoNextState(void* self);
extern "C" void cAI_prepareToPurgeComputerRiders(void* o);
extern "C" void cAI_prepareToPurgeComputerActors(void* o);
extern "C" void cAI_initAIRiders(void* o);
extern "C" void cAI_initAIActors(void* o, int a1);
extern "C" void cAI_setRaceRankType(void* o, int a1);
extern "C" int cAI_isAILoadedAndReady(void* o);
extern "C" void cAI_loadRiders(void* o);
void cAI_resolveRace(void* o);
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
        cBXLoadIndicator_Show(hud_43B0(), 2);
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
            cAI_prepareToPurgeComputerRiders(MGR_43B0->obj);
            cAI_prepareToPurgeComputerActors(MGR_43B0->obj);
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
            cAI_resolveRace(MGR_43B0->obj);
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
void cAI_resolveRace(void* p);
extern "C" void cAI_initFreeride(void* p);
extern "C" void cReplay_startRace(void* p);
extern "C" void cAI_setRaceRankType(void* p, int a);
extern "C" void cWScriptMan_purgeMissions(void* a);
extern "C" void cWScriptMan_setupMissions(void* a);
extern "C" void cAI_updateAnimation(void* p);
extern "C" void func_00155E58(void* p);
extern void* cGameState_mpGame;
extern void* cWScriptMan_TheWScriptMan;


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
        cAI_resolveRace(*(void**)((char*)cGameState_mpGame + 0xC));
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
    cWScriptMan_purgeMissions(cWScriptMan_TheWScriptMan);
    cWScriptMan_setupMissions(cWScriptMan_TheWScriptMan);
    (*(sRaceMan_234750**)((char*)cGameState_mpGame + 0xC))->f8C = 0;
    cAI_setAIState(*(void**)((char*)cGameState_mpGame + 0xC), 3);
    cBE_setState(1);
    cAI_updateAnimation(*(void**)((char*)cGameState_mpGame + 0xC));
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

/**
 * Starts the event's opening NIS lists: plays list 1 and then list 0 when the script engine is in state 2, and records
 * whether a list is playing.
 *
 * Each call is cSSXScriptEngine_PlayNISList(ScriptEngine, list, 1, 1), made only while the engine's +0x550
 * (mPlayingNISList) is 2. Then +0x14 is 1 when mPlayingNISList is 1, else 0. cGFEventStartState_update() calls it.
 */
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
INCLUDE_ASM("fe/festateloadscreen", cGFEventStartState_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: the unit declares cGameState_writeToReplayFrame as `void* (void*)`; the body also uses $a1 (obj).
void* cGameState_writeToReplayFrame_2(void* self, void* obj) __asm__("cGameState_writeToReplayFrame");

struct sVEntry_cGFEventStartState_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Event-start state: saves its replay data.
 *
 * Writes the base game-state part (cGameState_writeToReplayFrame), then the words at +0x10, +0x14, +0x1C and +0x8 through the
 * stream's virtual slot 1 (write).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFEventStartState_writeToReplayFrame(void* self, void* stream)
{
    cGameState_writeToReplayFrame_2(self, stream);
    sVEntry_cGFEventStartState_writeToReplayFrame* writeEntry = &(*(sVEntry_cGFEventStartState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x10, 4);
    writeEntry = &(*(sVEntry_cGFEventStartState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x14, 4);
    writeEntry = &(*(sVEntry_cGFEventStartState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x1C, 4);
    writeEntry = &(*(sVEntry_cGFEventStartState_writeToReplayFrame**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x8, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", cGFEventStartState_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: the unit declares cGameState_readFromReplayFrame as `void* (void*)`; the body also uses $a1 (obj).
void* cGameState_readFromReplayFrame_2(void* self, void* obj) __asm__("cGameState_readFromReplayFrame");

struct sVEntry_cGFEventStartState_readFromReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Event-start state: restores its replay data.
 *
 * Reads the base game-state part (cGameState_readFromReplayFrame), then the words at +0x10, +0x14, +0x1C and +0x8 through the
 * stream's virtual slot 2 (read).
 *
 * @param stream  the replay frame stream (On Tour cReplayFramePtr)
 */
extern "C" void cGFEventStartState_readFromReplayFrame(void* self, void* stream)
{
    cGameState_readFromReplayFrame_2(self, stream);
    sVEntry_cGFEventStartState_readFromReplayFrame* readEntry = &(*(sVEntry_cGFEventStartState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x10, 4);
    readEntry = &(*(sVEntry_cGFEventStartState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x14, 4);
    readEntry = &(*(sVEntry_cGFEventStartState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x1C, 4);
    readEntry = &(*(sVEntry_cGFEventStartState_readFromReplayFrame**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x8, 4);
}
#endif

