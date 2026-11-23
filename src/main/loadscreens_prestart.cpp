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

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_update);
#ifdef SKIP_ASM
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cLoadIndicator_update(void* fade);
extern "C" int cPadCache_numButtons(void* pads, int port);
extern "C" char* cPadCache_buttonValues(void* pads, int port);
extern char* PadCache;
extern char str_InitLoadScreen[];
extern void* cInitLoadScreen_vtbl[];



/**
 * Updates the pre-start screen: animates and fades in the load indicator and queues the init load screen.
 *
 * Steps the indicator's phase (cLoadIndicator_update) and raises its fade level (+0x4) by 1/12 per frame up to 1.
 * When no next module is queued it creates a cInitLoadScreen ("InitLoadScreen"), keeps it at +0x8 and queues
 * it.
 *
 * The screen's timer (+0x4) gains 1/60 per frame, plus 0.15 while a pad is connected. When pad 0's sample has
 * 11 or more values with values 8 and 10 nonzero, it sets the init load screen's +0x4 flag, which probably
 * makes the start screen it creates open in mode 2.
 */
extern "C" void cPreStartScreen_update(cPreStartScreen* self)
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
        *(cInitLoadScreen**)&self->nextscreen = loadScreen;
        // PORT: setNextModule takes the module pointer as an unsigned int
        cAppMan_setNextModule(app, (unsigned int)loadScreen);
    }
    self->progressivetimer += 0.01666666753590107f;
    if (*(int*)(PadCache + 0x2EE8) > 0) {
        self->progressivetimer += 0.15000000596046448f;
        if (cPadCache_numButtons(PadCache, 0) >= 11) {
            if (*(float*)(cPadCache_buttonValues(PadCache, 0) + 0x20) != 0.0f && *(float*)(cPadCache_buttonValues(PadCache, 0) + 0x28) != 0.0f) {
                (self->nextscreen)->progressive = 1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_render);
#ifdef SKIP_ASM
struct sVec3_232488 {
    float x, y, z;
    sVec3_232488(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232488 {
public:
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
    virtual void SetScreenClearColour(const sVec3_232488&);
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18();
    virtual void v19();
    virtual void DrawFrame();
};
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
    if (((cWorld232488*)GrMan)->IsRenderReady() == 0) {
        return 0;
    }
    ((cWorld232488*)GrMan)->SetScreenClearColour(sVec3_232488(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    ((cWorld232488*)GrMan)->DrawFrame();
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
extern "C" int cPreStartScreen_loseFocus(cPreStartScreen* self)
{
    if (self->progressivetimer < 0.75f) {
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
extern "C" void cInitLoadScreen_update(cInitLoadScreen* self)
{
    cLoadIndicator_update(SSXApp->LoadIndicator);
    if (self->ending != 0) {
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
            (unsigned int)cStartScreen_cStartScreen((cStartScreen*)cMemMan_alloc(0x1C, str_StartScreen, 0x100, 0), self->progressive ? 2 : 0));
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cInitLoadScreen_render);
#ifdef SKIP_ASM
struct sVec3_232650 {
    float x, y, z;
    sVec3_232650(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232650 {
public:
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
    virtual void SetScreenClearColour(const sVec3_232650&);
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18();
    virtual void v19();
    virtual void DrawFrame();
};
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
    if (((cWorld232650*)GrMan)->IsRenderReady() == 0) {
        return 0;
    }
    ((cWorld232650*)GrMan)->SetScreenClearColour(sVec3_232650(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    ((cWorld232650*)GrMan)->DrawFrame();
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
extern "C" int cInitLoadScreen_loseFocus(cInitLoadScreen* self)
{
    self->ending = 1;
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
extern "C" int cPreGameLoadScreen_loseFocus(cPreGameLoadScreen* self)
{
    self->ending = 1;
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
extern "C" int cPreFELoadScreen_loseFocus(cPreFELoadScreen* self)
{
    self->ending = 1;
    return SSXApp->LoadIndicator->alpha == 0.0f;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreGameLoadScreen_render);
#ifdef SKIP_ASM
struct sVec3_232820 {
    float x, y, z;
    sVec3_232820(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld232820 {
public:
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
    virtual void SetScreenClearColour(const sVec3_232820&);
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18();
    virtual void v19();
    virtual void DrawFrame();
};
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
    if (((cWorld232820*)GrMan)->IsRenderReady() == 0) {
        return 0;
    }
    ((cWorld232820*)GrMan)->SetScreenClearColour(sVec3_232820(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    ((cWorld232820*)GrMan)->DrawFrame();
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_render);
#ifdef SKIP_ASM
struct sVec3_2328A8 {
    float x, y, z;
    sVec3_2328A8(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};
class cWorld2328A8 {
public:
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
    virtual void SetScreenClearColour(const sVec3_2328A8&);
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18();
    virtual void v19();
    virtual void DrawFrame();
};
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
    if (((cWorld2328A8*)GrMan)->IsRenderReady() == 0) {
        return 0;
    }
    ((cWorld2328A8*)GrMan)->SetScreenClearColour(sVec3_2328A8(0.0f, 0.0f, 0.0f));
    cLoadIndicator_render(SSXApp->LoadIndicator);
    ((cWorld2328A8*)GrMan)->DrawFrame();
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

struct sModule_232930 {
    void** vt;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};

/**
 * Updates the pre-game load screen: animates and fades the load indicator, then queues the game load screen.
 *
 * Steps the indicator's phase (cLoadIndicator_update); its alpha (+0x4) falls by 1/12 per frame while the ending flag
 * (+0x4 of this) is set and rises otherwise, clamped to [0, 1]. When no next module is queued (SSXApp + 0x4), allocates
 * 0x18 bytes tagged "cGameLoadScreen", builds the fade base (cFadeScreen_cFadeScreen), stores cGameLoadScreen_vtbl, clears +0xC, +0x10 and
 * +0x14 and queues it with cAppMan_setNextModule. Update slot (+0x34) of cPreGameLoadScreen_vtbl.
 */
// Matches only with exceptions enabled for this unit (UNIT_CFLAGS "-fexceptions" in configure.py).
// `tmp` is reused for the fade direction and the module handle: one variable spanning both blocks is what
// gives the retail register allocation (the handle is taken before the fields are filled in).
extern "C" void cPreGameLoadScreen_update(cPreGameLoadScreen* self)
{
    int tmp;
    cLoadIndicator_update(SSXApp->LoadIndicator);
    tmp = self->ending != 0;
    if (tmp) {
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
        sModule_232930* loadScreen = (sModule_232930*)cMemMan_alloc(0x18, str_cGameLoadScreen, 0x100, 0);
        cFadeScreen_cFadeScreen_r(loadScreen);
        tmp = (unsigned int)loadScreen;
        loadScreen->vt = cGameLoadScreen_vtbl;
        loadScreen->fC = 0;
        loadScreen->f10 = 0;
        loadScreen->f14 = 0;
        cAppMan_setNextModule((cAppMan*)SSXApp, tmp);
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

struct sModule_232A00 {
    void** vt;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
};

/**
 * Updates the pre-front-end load screen: animates and fades the load indicator and queues the front-end load.
 *
 * Steps the indicator's phase (cLoadIndicator_update); its fade level (+0x4) falls by 1/12 per frame while the word
 * at +0x4 is set and rises otherwise. When no next module is queued it creates a cFELoadScreen ("FELoad",
 * 0x1C bytes) and queues it.
 */
// Same shape as cPreGameLoadScreen_update (see the note there): exceptions enabled for this unit, `tmp` reused.
extern "C" void cPreFELoadScreen_update(cPreFELoadScreen* self)
{
    int tmp;
    cLoadIndicator_update(SSXApp->LoadIndicator);
    tmp = self->ending != 0;
    if (tmp) {
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
        sModule_232A00* loadScreen = (sModule_232A00*)cMemMan_alloc(0x1C, str_FELoad, 0x100, 0);
        cFadeScreen_cFadeScreen_r(loadScreen);
        tmp = (unsigned int)loadScreen;
        loadScreen->vt = cFELoadScreen_vtbl;
        loadScreen->fC = 0;
        loadScreen->f10 = 0;
        loadScreen->f14 = 0;
        cAppMan_setNextModule((cAppMan*)SSXApp, tmp);
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
extern char cFEInput_vtbl[];
extern char D_0047CA18[];

struct sNode_232B28 {
    int f0;
    char f4;
    void* f8;
    int fC;
    int f10;
    int f14;
    int f18;
};

struct sUI_232B28 {
    int f0;
    int f4;
    int f8;
    sNode_232B28* fC;
    int f10;
    int f14;
    char list[4];
};

/**
 * Loads the start screen: creates its UI engine from "data/ui/su" and pushes the memory card or progressive-scan state.
 *
 * Calls GrMan's InitTextures (slot 44) with 0x100, builds a 0x70-byte UI engine (tag "UIEngine", cUIEngine_cUIEngine) into
 * +0xC, loads "data/ui/su" (cUIEngine_loadFile), gives it SSXApp's FELocale (+0x10) and Fonts (+0x8) and a new
 * 0x1C-byte input (tag "GLUIInput", cFEInput_vtbl at +0x8) at +0xC of the engine. Type (+0x18) 0 pushes a
 * cFEStateMemCard (584 bytes, func_00186A08); type 2 pushes a 72-byte "cFEStateProgressive" (cUIState base
 * cUIState_cUIState, vtable D_0047CA18); any other type pushes nothing. Load slot (+0x14) of cStartScreen_vtbl.
 */
extern "C" void cStartScreen_load(cStartScreen* self)
{
    GrMan->InitTextures(0x100);
    void* ui = cUIEngine_cUIEngine(operator new(0x70, str_UIEngine_b, 0x100, 0));
    self->mpUI = (cUIEngine*)ui;
    cUIEngine_loadFile(ui, str_data_ui_su);
    ((sUI_232B28*)self->mpUI)->f10 = (int)SSXApp->FELocale;
    sNode_232B28* n = (sNode_232B28*)operator new(0x1C, str_GLUIInput, 0x100, 0);
    n->f4 = 0;
    n->f8 = cFEInput_vtbl;
    n->f18 = 0;
    n->fC = 0;
    n->f10 = 0;
    n->f14 = 0;
    ((sUI_232B28*)self->mpUI)->fC = n;
    ((sUI_232B28*)self->mpUI)->f8 = (int)SSXApp->Fonts;
    int mode = self->meStartScreen_type;
    if (mode == 0) {
        void* item = func_00186A08(operator new(584, str_cFEStateMemCard, 0, 0), self->mpUI);
        cUIStateStack_push(&self->mpUI->mStateStack, item);
    } else if (mode == 2) {
        void* progressiveState = operator new(72, str_cFEStateProgressive, 0, 0);
        cUIState_cUIState(progressiveState, self->mpUI);
        *(void**)((char*)progressiveState + 8) = D_0047CA18;
        cUIStateStack_push(&self->mpUI->mStateStack, progressiveState);
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

struct sVec3i_2C98 {
    int x, y, z;
    sVec3i_2C98(int a, int b, int c) { x = a; y = b; z = c; }
};

struct sVE_2C98 {
    short delta;
    short index;
    void (*fn)(void*, sVec3i_2C98);
};

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
extern "C" void cStartScreen_gainFocus(cStartScreen* self)
{
    cGraphicsMan_UploadTextures(GrMan);
    self->DisplayTicks = 0;
    clear_2C98(&cDepthFog_m_Enable);
    clear_2C98(&cScreenTint_m_Enable);
    clear_2C98(&cLightGlow_m_Enable);
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_2C98* vt = (sVE_2C98*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    vt[15].fn(vthis(ps2GraphicsMan, vt[15]), sVec3i_2C98(0, 0, 0));
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
extern "C" void cStartScreen_update(cStartScreen* self)
{
    cLoadScreen_update(self);
    cUIEngine_update(self->mpUI);
    (self->DisplayTicks)++;
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
extern "C" void cStartScreen_purge(cStartScreen* self)
{
    GrMan->FlushRenderer();
    void* uiEngine = self->mpUI;
    if (uiEngine != 0) {
        cUIEngine__cUIEngine(uiEngine, 3);
    }
    self->mpUI = 0;
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
extern "C" int cStartScreen_render(cStartScreen* self)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    if (cFadeScreen_drawFade(self) == 0) {
        cUIEngine_render2D(self->mpUI);
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
extern char cFEInput_vtbl[];
extern char D_0047C878[];
extern char D_0047C7A8[];
extern char D_0047C948[];


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
        *(void**)(pad + 8) = D_0047C878;
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
        *(void**)(pad + 8) = D_0047C948;
        owner = 0;
    }
};

/**
 * Loads the game load screen: creates its UI engine from "data/ui/gl" and pushes the loading state for the race.
 *
 * Calls GrMan's InitTextures (slot 44) with 0x100 and cBE_getInterface (result unused), builds a 0x70-byte UI engine
 * (tag "UIEngine", cUIEngine_cUIEngine) into +0xC, loads "data/ui/gl", gives it SSXApp's Fonts and FELocale and a 0x1C-byte
 * "GLUIInput" input. Then pushes one 0x48-byte state built on cUIState_cUIState:
 * - "cGameLoadStateOutLodge" (vtable D_0047C878, clears gInLodge) when gInLodge is set, or on the career path
 *   (cGamePath 0) in game mode 5 or 6;
 * - "cGameLoadStateConquer" (D_0047C7A8, clears D_004A19DC) on the career path in any other mode;
 * - otherwise "cGameLoadState" (D_0047C948) with this screen as its owner (+0x48), after
 *   cGameLoadScreen_loadTexture(this, 0).
 *
 * Load slot (+0x14) of cGameLoadScreen_vtbl.
 */
extern "C" void cGameLoadScreen_load(cGameLoadScreen* self)
{
    cPS2GraphicsMan* w = GrMan;
    w->InitTextures(0x100);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* ui = cUIEngine_cUIEngine(operator new(0x70, str_UIEngine_b, 0x100, 0));
    self->mpUI = (cUIEngine*)ui;
    cUIEngine_loadFile(ui, str_data_ui_gl);
    ((sUI_2E20*)self->mpUI)->f8 = (int)SSXApp->Fonts;
    ((sUI_2E20*)self->mpUI)->f10 = (int)SSXApp->FELocale;
    sNode_2E20* n = (sNode_2E20*)operator new(0x1C, str_GLUIInput, 0x100, 0);
    n->f4 = 0;
    n->f8 = cFEInput_vtbl;
    n->f18 = 0;
    n->fC = 0;
    n->f10 = 0;
    n->f14 = 0;
    ((sUI_2E20*)self->mpUI)->fC = n;
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
    item = new (str_cGameLoadStateOutLodge, 0, 0) cLoadItemA_2E20(self->mpUI);
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
            item = new (str_cGameLoadStateConquer, 0, 0) cLoadItemB_2E20(self->mpUI);
        } else {
            cLoadItemC_2E20* loadState = new (str_cGameLoadState, 0, 0) cLoadItemC_2E20(self->mpUI);
            item = loadState;
            loadState->owner = ((void*)self);
            cGameLoadScreen_loadTexture_2E20(self, 0);
        }
    }
done:
    cUIStateStack_push(&self->mpUI->mStateStack, item);
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
extern "C" void cGameLoadScreen_loadTexture(cGameLoadScreen* self)
{
    char name[0x70];
    cBigFile bf;
    void* plr = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* fe = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 2) {
        cBigFile_cBigFile1_3048(&bf, str_data_ui_charpic_big, 0x100);
        int* slot = (int*)(&self->mTexID[0]);
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
        self->mTexID[0] = loadTex_3048(data, str_coursePics);
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

struct sVec3i_3260 {
    int x, y, z;
    sVec3i_3260(int a, int b, int c) { x = a; y = b; z = c; }
};

struct sVE_3260 {
    short delta;
    short index;
    void (*fn)(void*, sVec3i_3260);
};

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
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_3260* vt = (sVE_3260*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    vt[15].fn(vthis(ps2GraphicsMan, vt[15]), sVec3i_3260(0, 0, 0));
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
extern "C" void cGameLoadScreen_purge(cGameLoadScreen* self)
{
    GrMan->FlushRenderer();
    void* uiEngine = self->mpUI;
    if (uiEngine != 0) {
        cUIEngine__cUIEngine(uiEngine, 3);
    }
    int i;
    for (i = 0; i < 2; i++) {
        void* textureId = ((void**)&self->mTexID)[i];
        if (textureId != 0) {
            GrMan->RemoveTexture((int)textureId);
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
extern "C" int cGameLoadScreen_render(cGameLoadScreen* self)
{
    if (GrMan->IsRenderReady() == 0) {
        return 0;
    }
    if (cFadeScreen_drawFade(self) == 0) {
        cUIEngine_render2D(self->mpUI);
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
extern "C" void cGameLoadScreen_update(cGameLoadScreen* self)
{
    cUIEngine_update(self->mpUI);
    cLoadScreen_update(self);
}
#endif

