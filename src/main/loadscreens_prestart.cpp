#include "common.h"
#include "be/beinterface.h"
#include "main/loadindicator.h"
#include "ui/uiengine.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "main/loadscreens.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"
#include "render/playerlights.h"
#include "input/padcache.h"
#include "fe/feinput.h"
#include "util/locale.h"

// A tRGB built from its three components (the clear colour the render slots pass to SetScreenClearColour).
struct tRGBInit : tRGB {
    tRGBInit(float r, float g, float b) { R = r; G = g; B = b; }
};

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_update);
#ifdef SKIP_ASM
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cLoadIndicator_update(void* fade);
extern "C" int cPadCache_numButtons(cPadCache* pads, int port);
extern "C" float* cPadCache_buttonValues(cPadCache* pads, int port);
extern cPadCache* PadCache;
extern char str_InitLoadScreen[];
extern void* cInitLoadScreen_vtbl[];



/**
 * Updates the pre-start screen: animates and fades in the load indicator and queues the init load screen.
 *
 * Steps the indicator's phase (cLoadIndicator_update) and raises its fade level (alpha) by 1/12 per frame up to 1.
 * When no next module is queued it creates a cInitLoadScreen ("InitLoadScreen"), keeps it in nextscreen and queues
 * it.
 *
 * The screen's timer (progressivetimer) gains 1/60 per frame, plus 0.15 while a pad is connected (PadCache's
 * NumPadDevices). When pad 0's sample has 11 or more values with values 8 and 10 nonzero, it sets the init load
 * screen's progressive flag, which probably makes the start screen it creates open in mode 2.
 */
void cPreStartScreen::update()
{
    cLoadIndicator_update(SSXApp->LoadIndicator);
    float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
    *fade += 0.0833333358168602f;
    if (*fade > 1.0f) {
        *fade = 1.0f;
    }
    if ((int)SSXApp->LoadingModule == 0) {
        cInitLoadScreen* loadScreen = (cInitLoadScreen*)cMemMan_alloc(0xC, str_InitLoadScreen, 0x100, 0);
        cAppMan* app = (cAppMan*)SSXApp;
        loadScreen->mVtbl = cInitLoadScreen_vtbl;
        loadScreen->progressive = 0;
        loadScreen->ending = 0;
        *(cInitLoadScreen**)&nextscreen = loadScreen;
        // PORT: setNextModule takes the module pointer as an unsigned int
        cAppMan_setNextModule(app, (unsigned int)loadScreen);
    }
    progressivetimer += 0.01666666753590107f;
    if (PadCache->NumPadDevices > 0) {
        progressivetimer += 0.15000000596046448f;
        if (cPadCache_numButtons(PadCache, 0) >= 11) {
            if (cPadCache_buttonValues(PadCache, 0)[8] != 0.0f && cPadCache_buttonValues(PadCache, 0)[10] != 0.0f) {
                nextscreen->progressive = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_render);
#ifdef SKIP_ASM
extern "C" void cLoadIndicator_render(void* a);

/**
 * Renders the pre-start screen: clears to black and draws the load indicator once the renderer is ready.
 *
 * Returns 0 without drawing while GrMan's IsRenderReady (slot 17) is 0. Otherwise sets the clear colour to (0, 0, 0)
 * (SetScreenClearColour, slot 15), draws SSXApp's load indicator (cLoadIndicator_render) and the frame (DrawFrame, slot 20).
 * Render slot (+0x3C) of cPreStartScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
extern "C" int cPreStartScreen_render(void)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_loseFocus);
#ifdef SKIP_ASM
/**
 * Lets the pre-start screen lose focus once its timer (+0x4) reaches 0.75.
 *
 * LoseFocus slot (+0x24) of cPreStartScreen_vtbl.
 *
 * @return 1 when the timer is at least 0.75, otherwise 0
 */
int cPreStartScreen::loseFocus()
{
    if (progressivetimer < 0.75f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen__cInitLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cAppModule_vtbl[];

/**
 * Destroys the start-up load screen.
 *
 * Only resets the vtable to the root module vtable cAppModule_vtbl (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cInitLoadScreen__cInitLoadScreen(cInitLoadScreen* self, int flags)
{
    *(void***)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen_load);
#ifdef SKIP_ASM
extern "C" void cSSXApp_initload(void* app);

/**
 * Runs the application's initial load for the init load screen via cSSXApp_initload(SSXApp).
 */
extern "C" void cInitLoadScreen_load(void)
{
    cSSXApp_initload(SSXApp);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen_update);
#ifdef SKIP_ASM
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cLoadIndicator_update(void* fade);
extern "C" void* cStartScreen_cStartScreen(cStartScreen* self, int arg);
extern char str_StartScreen[];

/**
 * Updates the initial load screen: animates and fades the load indicator, then queues the start screen.
 *
 * Steps the indicator's phase (cLoadIndicator_update); its alpha (SSXApp's load indicator + 0x4) falls by 1/12 per
 * frame while the ending flag (+0x8) is set and rises otherwise, clamped to [0, 1]. When no next module is queued
 * (SSXApp + 0x4), creates a start screen (0x1C bytes, tag "StartScreen", cStartScreen_cStartScreen) with type 2 when the
 * progressive flag (+0x4) is set, otherwise 0, and queues it with cAppMan_setNextModule. Update slot (+0x34) of
 * cInitLoadScreen_vtbl.
 */
void cInitLoadScreen::update()
{
    cLoadIndicator_update(SSXApp->LoadIndicator);
    if (ending != 0) {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade -= 0.0833333358168602f;
        if (*fade < 0.0f) {
            *fade = 0.0f;
        }
    } else {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade += 0.0833333358168602f;
        if (*fade > 1.0f) {
            *fade = 1.0f;
        }
    }
    if ((int)SSXApp->LoadingModule == 0) {
        cAppMan_setNextModule((cAppMan*)SSXApp,
            (unsigned int)cStartScreen_cStartScreen((cStartScreen*)cMemMan_alloc(0x1C, str_StartScreen, 0x100, 0),
                    progressive ? 2 : 0));
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen_render);
#ifdef SKIP_ASM
extern "C" void cLoadIndicator_render(void* a);

/**
 * Renders the initial load screen: clears to black and draws the load indicator once the renderer is ready.
 *
 * Same body as cPreStartScreen_render: IsRenderReady (slot 17), SetScreenClearColour (slot 15) with (0, 0, 0), the load
 * indicator (cLoadIndicator_render) and DrawFrame (slot 20). Render slot (+0x3C) of cInitLoadScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
extern "C" int cInitLoadScreen_render(void)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen_loseFocus);
#ifdef SKIP_ASM
struct sLoadScreen2326D8 {
    int f0;
    int f4;
    int done;
};

/**
 * Asks the initial load screen to finish: sets the ending flag (+0x8) and lets it lose focus once the load indicator
 * has faded out.
 *
 * LoseFocus slot (+0x24) of cInitLoadScreen_vtbl.
 *
 * @return 1 when the load indicator's alpha (+0x4) is 0, otherwise 0
 */
int cInitLoadScreen::loseFocus()
{
    ending = 1;
    return SSXApp->LoadIndicator->alpha == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_load__FPv);
#ifdef SKIP_ASM
/**
 * Loads nothing: the pre-game load screen has no data of its own (empty, as in On Tour).
 */
void cPreGameLoadScreen_load(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_load__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the load slot (+0x14) of cPreFELoadScreen_vtbl, the pre-front-end load screen's vtable.
 */
void cPreFELoadScreen_load(void* self)
{
}
#endif

extern void* cPreGameLoadScreen_vtbl[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_cPreGameLoadScreen__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a pre-game load screen: clears the word at +0x4 and stores the vtable at +0x0.
 *
 * @return @p self
 */
void* cPreGameLoadScreen_cPreGameLoadScreen(void* vself)
{
    cPreGameLoadScreen* self = (cPreGameLoadScreen*)vself;
    self->ending = 0;
    *(int*)self = (int)(void*)cPreGameLoadScreen_vtbl;
    return ((void*)self);
}
#endif

extern void* cPreFELoadScreen_vtbl[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_cPreFELoadScreen__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the pre-front-end load screen: clears the ending flag (+0x4) and stores the vtable cPreFELoadScreen_vtbl.
 *
 * cFECustom_getNextState, cGame_exit and cDebugMenuScreen_update build it when they go back to the front end.
 *
 * @return @p self
 */
void* cPreFELoadScreen_cPreFELoadScreen(void* vself)
{
    cPreFELoadScreen* self = (cPreFELoadScreen*)vself;
    self->ending = 0;
    *(int*)self = (int)(void*)cPreFELoadScreen_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen__cPreGameLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cAppModule_vtbl[];

/**
 * Destroys the pre-game load screen.
 *
 * Only resets the vtable to the root module vtable cAppModule_vtbl (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cPreGameLoadScreen__cPreGameLoadScreen(cPreGameLoadScreen* self, int flags)
{
    *(void***)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen__cPreFELoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cAppModule_vtbl[];

/**
 * Destroys the pre-front-end load screen: restores the base module vtable cAppModule_vtbl and frees the object when bit 0
 * of @p flags is set.
 *
 * Destructor slot (+0xC) of cPreFELoadScreen_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cPreFELoadScreen__cPreFELoadScreen(cPreFELoadScreen* self, int flags)
{
    *(void***)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_loseFocus);
#ifdef SKIP_ASM
struct sLoadScreen2327B0 {
    int f0;
    int done;
};

/**
 * Asks the pre-game load screen to finish: sets the ending flag (+0x4) and lets it lose focus once the load indicator
 * has faded out.
 *
 * LoseFocus slot (+0x24) of cPreGameLoadScreen_vtbl.
 *
 * @return 1 when the load indicator's alpha (+0x4) is 0, otherwise 0
 */
int cPreGameLoadScreen::loseFocus()
{
    ending = 1;
    return SSXApp->LoadIndicator->alpha == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_loseFocus);
#ifdef SKIP_ASM
struct sLoadScreen2327E8 {
    int f0;
    int done;
};

/**
 * Asks the pre-front-end load screen to finish: sets the ending flag (+0x4) and lets it lose focus once the load
 * indicator has faded out.
 *
 * LoseFocus slot (+0x24) of cPreFELoadScreen_vtbl.
 *
 * @return 1 when the load indicator's alpha (+0x4) is 0, otherwise 0
 */
int cPreFELoadScreen::loseFocus()
{
    ending = 1;
    return SSXApp->LoadIndicator->alpha == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_render);
#ifdef SKIP_ASM
extern "C" void cLoadIndicator_render(void* a);

/**
 * Renders the pre-game load screen: clears to black and draws the load indicator once the renderer is ready.
 *
 * Same body as cPreStartScreen_render (IsRenderReady, SetScreenClearColour (0, 0, 0), cLoadIndicator_render, DrawFrame). Render slot
 * (+0x3C) of cPreGameLoadScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
extern "C" int cPreGameLoadScreen_render(void)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_render);
#ifdef SKIP_ASM
extern "C" void cLoadIndicator_render(void* a);

/**
 * Renders the pre-front-end load screen: clears to black and draws the load indicator once the renderer is ready.
 *
 * Same body as cPreStartScreen_render (IsRenderReady, SetScreenClearColour (0, 0, 0), cLoadIndicator_render, DrawFrame). Render slot
 * (+0x3C) of cPreFELoadScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
extern "C" int cPreFELoadScreen_render(void)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cLoadIndicator_update(void* fade);
extern "C" void* cFadeScreen_cFadeScreen_r(void* self) __asm__("cFadeScreen_cFadeScreen");
extern char str_cGameLoadScreen[];
extern void* cGameLoadScreen_vtbl[];

/**
 * Updates the pre-game load screen: animates and fades the load indicator, then queues the game load screen.
 *
 * Steps the indicator's phase (cLoadIndicator_update); its alpha falls by 1/12 per frame while the ending flag is set
 * and rises otherwise, clamped to [0, 1]. When no next module is queued (SSXApp's LoadingModule), allocates 0x18 bytes
 * tagged "cGameLoadScreen", builds the fade base (cFadeScreen_cFadeScreen), stores cGameLoadScreen_vtbl, clears mpUI
 * and both mTexID and queues it with cAppMan_setNextModule. Update slot (+0x34) of cPreGameLoadScreen_vtbl.
 */
// Matches only with exceptions enabled for this unit (UNIT_CFLAGS "-fexceptions" in configure.py).
// `endingThenModule` is reused for the fade direction and the module handle: one variable spanning both blocks is
// what gives the retail register allocation (the handle is taken before the fields are filled in).
void cPreGameLoadScreen::update()
{
    int endingThenModule;
    cLoadIndicator_update(SSXApp->LoadIndicator);
    endingThenModule = ending != 0;
    if (endingThenModule) {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade -= 0.0833333358168602f;
        if (*fade < 0.0f) {
            *fade = 0.0f;
        }
    } else {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade += 0.0833333358168602f;
        if (*fade > 1.0f) {
            *fade = 1.0f;
        }
    }
    if ((int)SSXApp->LoadingModule == 0) {
        cGameLoadScreen* loadScreen = (cGameLoadScreen*)cMemMan_alloc(0x18, str_cGameLoadScreen, 0x100, 0);
        cFadeScreen_cFadeScreen_r(loadScreen);
        endingThenModule = (unsigned int)loadScreen;
        loadScreen->mVtbl = cGameLoadScreen_vtbl;
        loadScreen->mpUI = 0;
        loadScreen->mTexID[0] = 0;
        loadScreen->mTexID[1] = 0;
        cAppMan_setNextModule((cAppMan*)SSXApp, endingThenModule);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void cAppMan_setNextModule(cAppMan* self, unsigned int module);

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void cLoadIndicator_update(void* fade);

extern "C" void* cFadeScreen_cFadeScreen_r(void* self) __asm__("cFadeScreen_cFadeScreen");


extern char str_FELoad[];
extern void* cFELoadScreen_vtbl[];

/**
 * Updates the pre-front-end load screen: animates and fades the load indicator and queues the front-end load.
 *
 * Steps the indicator's phase (cLoadIndicator_update); its fade level (alpha) falls by 1/12 per frame while the
 * ending flag is set and rises otherwise. When no next module is queued it creates a cFELoadScreen ("FELoad",
 * 0x1C bytes; mpUI and both mTexID cleared) and queues it.
 */
// Same shape as cPreGameLoadScreen_update (see the note there): exceptions enabled for this unit, `endingThenModule`
// reused.
void cPreFELoadScreen::update()
{
    int endingThenModule;
    cLoadIndicator_update(SSXApp->LoadIndicator);
    endingThenModule = ending != 0;
    if (endingThenModule) {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade -= 0.0833333358168602f;
        if (*fade < 0.0f) {
            *fade = 0.0f;
        }
    } else {
        float* fade = (float*)(&SSXApp->LoadIndicator->alpha);
        *fade += 0.0833333358168602f;
        if (*fade > 1.0f) {
            *fade = 1.0f;
        }
    }
    if ((int)SSXApp->LoadingModule == 0) {
        cFELoadScreen* loadScreen = (cFELoadScreen*)cMemMan_alloc(0x1C, str_FELoad, 0x100, 0);
        cFadeScreen_cFadeScreen_r(loadScreen);
        endingThenModule = (unsigned int)loadScreen;
        loadScreen->mVtbl = cFELoadScreen_vtbl;
        loadScreen->mpUI = 0;
        loadScreen->mTexID[0] = 0;
        loadScreen->mTexID[1] = 0;
        cAppMan_setNextModule((cAppMan*)SSXApp, endingThenModule);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_cStartScreen);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_cFadeScreen(void* self);
extern void* cStartScreen_vtbl[];

/**
 * Constructs a start screen of a type: builds the fade base (cFadeScreen_cFadeScreen), stores the vtable cStartScreen_vtbl, the type
 * (+0x18) and clears the UI engine pointer (+0xC).
 *
 * cInitLoadScreen's update (cInitLoadScreen_update) builds it with type 2 (progressive) or 0.
 *
 * @param screenType  the start screen type (0 shows the memory card state, 2 the progressive-scan state)
 * @return @p self
 */
extern "C" void* cStartScreen_cStartScreen(cStartScreen* self, int screenType)
{
    cFadeScreen_cFadeScreen(self);
    *(void***)self = cStartScreen_vtbl;
    self->meStartScreen_type = screenType;
    self->mpUI = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// malloc-like allocation (gcc treats operator new results as unaliased)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cUIEngine_cUIEngine(void* p);
extern "C" int cUIEngine_loadFile(void* ui, const char* name);
extern "C" void* func_00186A08(void* p, void* ui);
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_UIEngine_b[];
extern char str_data_ui_su[];
extern char str_GLUIInput[];
extern char str_cFEStateMemCard[];
extern char str_cFEStateProgressive[];
extern void* cFEInput_vtbl[];
extern void* D_0047CA18[];


/**
 * Loads the start screen: creates its UI engine from "data/ui/su" and pushes the memory card or progressive-scan state.
 *
 * Calls GrMan's InitTextures (slot 44) with 0x100, builds a 0x70-byte UI engine (tag "UIEngine",
 * cUIEngine_cUIEngine) into mpUI, loads "data/ui/su" (cUIEngine_loadFile), gives it SSXApp's FELocale
 * (mpLocaleInterface) and Fonts (mpFontInterface) and a new 0x1C-byte cFEInput (tag "GLUIInput", vtable
 * cFEInput_vtbl, fields cleared) as its mpInput. Type (meStartScreen_type) 0 pushes a cFEStateMemCard (584 bytes,
 * func_00186A08); type 2 pushes a 72-byte "cFEStateProgressive" (cUIState base cUIState_cUIState, vtable D_0047CA18);
 * any other type pushes nothing. Load slot (+0x14) of cStartScreen_vtbl.
 */
void cStartScreen::load()
{
    GrMan->InitTextures(0x100);
    void* ui = cUIEngine_cUIEngine(operator new(0x70, str_UIEngine_b, 0x100, 0));
    mpUI = (cUIEngine*)ui;
    cUIEngine_loadFile(ui, str_data_ui_su);
    mpUI->mpLocaleInterface = SSXApp->FELocale;
    cFEInput* input = (cFEInput*)operator new(0x1C, str_GLUIInput, 0x100, 0);
    input->mPadNum = 0;
    VPTR_OF(cUIInput, input) = cFEInput_vtbl;
    input->mpInput = 0;
    input->mbKeyReady = 0;
    input->miVKey = 0;
    input->miAsciiKey = 0;
    mpUI->mpInput = input;
    mpUI->mpFontInterface = SSXApp->Fonts;
    int mode = meStartScreen_type;
    if (mode == 0) {
        void* item = func_00186A08(operator new(584, str_cFEStateMemCard, 0, 0), mpUI);
        cUIStateStack_push(&mpUI->mStateStack, item);
    } else if (mode == 2) {
        cUIState* progressiveState = (cUIState*)operator new(72, str_cFEStateProgressive, 0, 0);
        cUIState_cUIState(progressiveState, mpUI);
        VPTR_OF(cListNode, progressiveState) = D_0047CA18;
        cUIStateStack_push(&mpUI->mStateStack, progressiveState);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_gainFocus);
#ifdef SKIP_ASM
extern int cDepthFog_m_Enable;
extern int cScreenTint_m_Enable;
extern int cLightGlow_m_Enable;
extern "C" void cGraphicsMan_UploadTextures(void* g);


// Clearing through a pointer keeps the stores ahead of the reload of GrMan.
static inline void clear_2C98(int* p)
{
    *p = 0;
}

/**
 * Starts the start screen: uploads textures, resets the display tick count (+0x14) and turns off depth fog, screen tint
 * and light glow.
 *
 * Calls cGraphicsMan_UploadTextures, clears cDepthFog_m_Enable, cScreenTint_m_Enable and cLightGlow_m_Enable and sets
 * the clear colour to (0, 0, 0) (SetScreenClearColour, slot 15). GainFocus slot (+0x2C) of cStartScreen_vtbl.
 */
void cStartScreen::gainFocus()
{
    cGraphicsMan_UploadTextures(GrMan);
    DisplayTicks = 0;
    clear_2C98(&cDepthFog_m_Enable);
    clear_2C98(&cScreenTint_m_Enable);
    clear_2C98(&cLightGlow_m_Enable);
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
}
#endif

extern "C" void* cFadeScreen_loseFocus(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Lets the start screen lose focus when its fade allows it: returns cFadeScreen_loseFocus.
 *
 * LoseFocus slot (+0x24) of cStartScreen_vtbl.
 *
 * @return cFadeScreen_loseFocus's result
 */
void* cStartScreen_loseFocus(void* self)
{
    return cFadeScreen_loseFocus(self);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_update);
#ifdef SKIP_ASM
void cLoadScreen_update(void*);
extern "C" void cUIEngine_update(void*);

/**
 * Updates the start screen: runs the load-screen update (cLoadScreen_update), updates the UI engine (cUIEngine_update on +0xC)
 * and counts a display tick (+0x14).
 *
 * Update slot (+0x34) of cStartScreen_vtbl.
 */
void cStartScreen::update()
{
    cLoadScreen_update(this);
    cUIEngine_update(mpUI);
    (DisplayTicks)++;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_purge);
#ifdef SKIP_ASM
extern "C" void cUIEngine__cUIEngine(void* obj, int a);
extern "C" void cGraphicsMan_PurgeTextures(void* world);

/**
 * Purges the start screen: flushes the renderer, destroys the UI engine and purges textures.
 *
 * Calls GrMan's FlushRenderer (slot 19), destroys the engine at +0xC with cUIEngine__cUIEngine(engine, 3) when set, clears the
 * pointer and calls cGraphicsMan_PurgeTextures. Purge slot (+0x1C) of cStartScreen_vtbl.
 */
void cStartScreen::purge()
{
    GrMan->FlushRenderer();
    void* uiEngine = mpUI;
    if (uiEngine != 0) {
        cUIEngine__cUIEngine(uiEngine, 3);
    }
    mpUI = 0;
    cGraphicsMan_PurgeTextures(GrMan);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cStartScreen_render);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void cUIEngine_render2D(void* obj);

/**
 * Renders the start screen once the renderer is ready: the UI engine unless the fade covers the screen, then the frame.
 *
 * Returns 0 while GrMan's IsRenderReady (slot 17) is 0. Otherwise draws the fade (cFadeScreen_drawFade) and, when it is not
 * fully opaque, the UI engine (cUIEngine_render2D on +0xC), then DrawFrame (slot 20). Render slot (+0x3C) of cStartScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
int cStartScreen::render()
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    if (cFadeScreen_drawFade(this) == 0) {
        cUIEngine_render2D(mpUI);
    }
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_load);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// malloc-like allocation (gcc treats operator new results as unaliased)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cUIEngine_cUIEngine(void* p);
extern "C" int cUIEngine_loadFile(void* ui, const char* name);
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern "C" void cUIStateStack_push(void* list, void* item);
// PORT: cGameLoadScreen_loadTexture takes (self) but this caller passes (self, 0).
extern "C" void cGameLoadScreen_loadTexture_2E20(void* self, int a1) __asm__("cGameLoadScreen_loadTexture");
extern int gInLodge;
extern int D_004A19DC;
extern char str_UIEngine_b[];
extern char str_data_ui_gl[];
extern char str_GLUIInput[];
extern char str_cGameLoadStateOutLodge[];
extern char str_cGameLoadStateConquer[];
extern char str_cGameLoadState[];
extern void* cFEInput_vtbl[];
extern char cGameLoadStateOutLodge_vtbl[];
extern char D_0047C7A8[];
extern char cGameLoadState_vtbl[];


struct sNode_2E20 {
    int f0;
    char f4;
    void* f8;
    int fC;
    int f10;
    int f14;
    int f18;
};

struct sUI_2E20 {
    int f0;
    int f4;
    int f8;
    sNode_2E20* fC;
    int f10;
    int f14;
    char list[4];
};

struct cLoadItemA_2E20 {
    char pad[0x48];
    cLoadItemA_2E20(void* ui)
    {
        cUIState_cUIState(this, ui);
        *(void**)(pad + 8) = cGameLoadStateOutLodge_vtbl;
        gInLodge = 0;
    }
};

struct cLoadItemB_2E20 {
    char pad[0x48];
    cLoadItemB_2E20(void* ui)
    {
        cUIState_cUIState(this, ui);
        *(void**)(pad + 8) = D_0047C7A8;
        D_004A19DC = 0;
    }
};

struct cLoadItemC_2E20 {
    char pad[0x48];
    void* owner;    // 0x48
    cLoadItemC_2E20(void* ui)
    {
        cUIState_cUIState(this, ui);
        *(void**)(pad + 8) = cGameLoadState_vtbl;
        owner = 0;
    }
};

/**
 * Loads the game load screen: creates its UI engine from "data/ui/gl" and pushes the loading state for the race.
 *
 * Calls GrMan's InitTextures (slot 44) with 0x100 and cBE_getInterface (result unused), builds a 0x70-byte UI engine
 * (tag "UIEngine", cUIEngine_cUIEngine) into +0xC, loads "data/ui/gl", gives it SSXApp's Fonts and FELocale and a 0x1C-byte
 * "GLUIInput" input. Then pushes one 0x48-byte state built on cUIState_cUIState:
 * - "cGameLoadStateOutLodge" (vtable cGameLoadStateOutLodge_vtbl, clears gInLodge) when gInLodge is set, or on the career path
 *   (cGamePath 0) in game mode 5 or 6;
 * - "cGameLoadStateConquer" (D_0047C7A8, clears D_004A19DC) on the career path in any other mode;
 * - otherwise "cGameLoadState" (cGameLoadState_vtbl) with this screen as its owner (+0x48), after
 *   cGameLoadScreen_loadTexture(this, 0).
 *
 * Load slot (+0x14) of cGameLoadScreen_vtbl.
 */
void cGameLoadScreen::load()
{
    cPS2GraphicsMan* w = GrMan;
    w->InitTextures(0x100);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* ui = cUIEngine_cUIEngine(operator new(0x70, str_UIEngine_b, 0x100, 0));
    mpUI = (cUIEngine*)ui;
    cUIEngine_loadFile(ui, str_data_ui_gl);
    ((sUI_2E20*)mpUI)->f8 = (int)SSXApp->Fonts;
    ((sUI_2E20*)mpUI)->f10 = (int)SSXApp->FELocale;
    sNode_2E20* n = (sNode_2E20*)operator new(0x1C, str_GLUIInput, 0x100, 0);
    n->f4 = 0;
    n->f8 = cFEInput_vtbl;
    n->f18 = 0;
    n->fC = 0;
    n->f10 = 0;
    n->f14 = 0;
    ((sUI_2E20*)mpUI)->fC = n;
    void* item;
    if (gInLodge == 0) {
        signed char* newRaceBytes = (signed char*)((char*)&cBENewRaceInterface_newRace);
        if (newRaceBytes[0x49] != 0)
            goto other;
        if (newRaceBytes[0x48] == 5)
            goto makeA;
        if (newRaceBytes[0x48] != 6)
            goto other;
    }
makeA:
    item = new (str_cGameLoadStateOutLodge, 0, 0) cLoadItemA_2E20(mpUI);
    goto done;
other:
    {
        signed char* raceDbBytes = (signed char*)((char*)&cBENewRaceInterface_newRace);
        int x = 0;
        if (raceDbBytes[0x49] == 0) {
            int notMode5 = raceDbBytes[0x48] != 5;
            if (notMode5)
                x = raceDbBytes[0x48] != 6;
        }
        if (x) {
            item = new (str_cGameLoadStateConquer, 0, 0) cLoadItemB_2E20(mpUI);
        } else {
            cLoadItemC_2E20* loadState = new (str_cGameLoadState, 0, 0) cLoadItemC_2E20(mpUI);
            item = loadState;
            loadState->owner = ((void*)this);
            cGameLoadScreen_loadTexture_2E20(this, 0);
        }
    }
done:
    cUIStateStack_push(&mpUI->mStateStack, item);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_loadTexture);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int cBENewPlayerInterface_getPlayerCharID(void* self, int idx);
#include "bx/bigfile.h"
// PORT: cBigFile_cBigFile1 really takes (self, path, flags); bound by asm label.
cBigFile* cBigFile_cBigFile1_3048(cBigFile* self, const char* path, int flags) __asm__("cBigFile_cBigFile1__FP8cBigFile");
void cBigFile__cBigFile(cBigFile* self, int flags);
void cMemMan_free(void*);
extern "C" int cBENewRaceInterface_getLocationID(void* self);
extern "C" char* FILE_loadpackz(const char* name, int flags);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_data_ui_charpic_big[];
extern char str_pct_spct_s_ssh[];
extern char str_CharaPics[];
extern char str_data_ui_courspic_big[];
extern char str_coursePics[];
extern char str_pct_s_ssh[];
extern char* D_00442928[];
extern char* str_at_plus_J[];
extern int D_004A2B3C;

static inline int loadTex_3048(char* data, const char* name)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    return ps2GraphicsMan->AddTexture((const SHAPE*)(data + *(int*)(data + 0x14)), name, 0, 1, -1);
}

/**
 * Loads the game load screen's picture: both players' character pictures online, the course picture
 * otherwise.
 *
 * On the online game path (newRace.cGamePath == 2) it loads "|<character>_small_flat.ssh" for each player from
 * data/ui/charpic.big as "CharaPics" into +0x10 and +0x14; otherwise "|<location>.ssh" for the race's
 * location (0 when above 16) from data/ui/courspic.big as "coursePics" into +0x10.
 *
 * Textures go through GrMan's slot 46, the file data is freed and the big file closed. On Tour's version also
 * takes a state argument.
 */
void cGameLoadScreen::loadTexture()
{
    char name[0x70];
    cBigFile bf;
    void* plr = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* fe = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 2) {
        cBigFile_cBigFile1_3048(&bf, str_data_ui_charpic_big, 0x100);
        int* slot = (int*)(&mTexID[0]);
        for (int i = 0; i < 2; i++) {
            sprintf(name, str_pct_spct_s_ssh, D_00442928[cBENewPlayerInterface_getPlayerCharID(plr, i != 0)], D_004A2B3C);
            char* data = FILE_loadpackz(name, 0x3000100);
            *slot = loadTex_3048(data, str_CharaPics);
            if (data != 0)
                cMemMan_free(data);
            slot++;
        }
        cBigFile__cBigFile(&bf, 2);
    } else {
        cBigFile_cBigFile1_3048(&bf, str_data_ui_courspic_big, 0x100);
        signed char n = cBENewRaceInterface_getLocationID(fe);
        if (n > 16)
            n = 0;
        sprintf(name, str_pct_s_ssh, str_at_plus_J[n]);
        char* data = FILE_loadpackz(name, 0x3000100);
        mTexID[0] = loadTex_3048(data, str_coursePics);
        if (data != 0)
            cMemMan_free(data);
        cBigFile__cBigFile(&bf, 2);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_gainFocus);
#ifdef SKIP_ASM
extern int cDepthFog_m_Enable;
extern "C" void cGraphicsMan_UploadTextures(void* g);
extern "C" void cSSXApp_startGameLoad(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F768(void* snd);


/**
 * Starts the game load screen: uploads textures, starts the game load, turns off depth fog and clears to black.
 *
 * Calls cGraphicsMan_UploadTextures and cSSXApp_startGameLoad(SSXApp), clears cDepthFog_m_Enable, sets the clear colour
 * (0, 0, 0) (SetScreenClearColour, slot 15) and calls func_0028F768 on the audio (cAudioInterface_bxAudio). GainFocus slot
 * (+0x2C) of cGameLoadScreen_vtbl.
 */
extern "C" void cGameLoadScreen_gainFocus(void)
{
    cGraphicsMan_UploadTextures(GrMan);
    cSSXApp_startGameLoad(SSXApp);
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* fogEnable = &cDepthFog_m_Enable;
    *fogEnable = 0;
    GrMan->SetScreenClearColour(tRGBInit(0.0f, 0.0f, 0.0f));
    func_0028F768(cAudioInterface_bxAudio());
}
#endif

extern "C" void* cLoadScreen_loseFocus(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Lets the game load screen lose focus: returns whether cLoadScreen_loseFocus returns non-zero.
 *
 * LoseFocus slot (+0x24) of cGameLoadScreen_vtbl.
 *
 * @return 1 or 0
 */
int cGameLoadScreen_loseFocus(void* self)
{
    return (cLoadScreen_loseFocus(self) != 0);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_purge);
#ifdef SKIP_ASM
extern "C" void cGraphicsMan_PurgeTextures(void* world);
extern "C" void cUIEngine__cUIEngine(void* obj, int a);
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void func_0028FA98(void* self, float v);

/**
 * Purges the game load screen: flushes the renderer, destroys the UI engine, removes its two textures and purges
 * textures.
 *
 * Calls GrMan's FlushRenderer (slot 19), destroys the engine at +0xC with cUIEngine__cUIEngine(engine, 3) when set (the
 * pointer is not cleared), removes each non-zero texture id of the pair at +0x10 (RemoveTexture, slot 50), calls
 * cGraphicsMan_PurgeTextures and func_0028FA98(audio, 1.0) on the audio (cAudioInterface_bxAudio). Purge slot (+0x1C) of
 * cGameLoadScreen_vtbl.
 */
void cGameLoadScreen::purge()
{
    GrMan->FlushRenderer();
    void* uiEngine = mpUI;
    if (uiEngine != 0) {
        cUIEngine__cUIEngine(uiEngine, 3);
    }
    int i;
    for (i = 0; i < 2; i++) {
        int textureId = mTexID[i];
        if (textureId != 0) {
            GrMan->RemoveTexture(textureId);
        }
    }
    cGraphicsMan_PurgeTextures(GrMan);
    func_0028FA98(cAudioInterface_bxAudio(), 1.0f);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_render);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void cUIEngine_render2D(void* obj);

/**
 * Renders the game load screen once the renderer is ready: the UI engine unless the fade covers the screen, then the
 * frame.
 *
 * Same body as cStartScreen_render (IsRenderReady, cFadeScreen_drawFade, cUIEngine_render2D on +0xC, DrawFrame). Render slot (+0x3C) of
 * cGameLoadScreen_vtbl.
 *
 * @return 1 when a frame was drawn, otherwise 0
 */
int cGameLoadScreen::render()
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    if (cFadeScreen_drawFade(this) == 0) {
        cUIEngine_render2D(mpUI);
    }
    GrMan->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_update);
#ifdef SKIP_ASM
extern "C" void cUIEngine_update(void*);
void cLoadScreen_update(void*);

/**
 * Updates the game load screen: updates the UI engine (cUIEngine_update on +0xC), then runs the load-screen update
 * (cLoadScreen_update).
 *
 * Update slot (+0x34) of cGameLoadScreen_vtbl.
 */
void cGameLoadScreen::update()
{
    cUIEngine_update(mpUI);
    cLoadScreen_update(this);
}
#endif

