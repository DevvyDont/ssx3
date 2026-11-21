#include "common.h"
#include "main/gamemodehandler.h"
#include "main/gameobjnodes.h"
#include "input/input.h"
#include "main/gamestate.h"
#include "main/sectionman.h"
#include "be/beintnewrace.h"
#include "main/loadscreens.h"
#include "main/game.h"
#include "movie/mcoverlaymanager.h"
#include "render/ps2graphicsman.h"
#include "main/gamemode.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "scripter/ssxscriptengine.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp__cSSXApp);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(void* self, int flags);
void cAppMan__cAppMan(void* self, int flags);

/**
 * Destroys the SSX application: destroys its three cBXString members, then runs the cAppMan destructor.
 *
 * The strings at +0xCC, +0xC8 and +0xC4 are destroyed without being freed (flags 2).
 *
 * @param flags  g++ destructor flags, passed to cAppMan__cAppMan(); bit 0 set frees the object's memory
 */
extern "C" void cSSXApp__cSSXApp(cSSXApp* self, int flags)
{
    cBXString__cBXString((char*)&self->CommandLineTrack, 2);
    cBXString__cBXString((char*)&self->Password, 2);
    cBXString__cBXString((char*)&self->UserName, 2);
    cAppMan__cAppMan(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getBE);
#ifdef SKIP_ASM
/**
 * Returns the application's back end (BE).
 *
 * @param ssxApp  the application object
 * @return BE (+0x78)
 */
extern "C" int cSSXApp_getBE(cSSXApp* ssxApp) {
    return ((int)ssxApp->BE);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getRealFE);
#ifdef SKIP_ASM
/**
 * Returns the application's real front end (RealFE).
 *
 * @param ssxApp  the application object
 * @return RealFE (+0x7C)
 */
extern "C" int cSSXApp_getRealFE(cSSXApp* ssxApp) {
    return ((int)ssxApp->RealFE);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getDebugFE);
#ifdef SKIP_ASM
/**
 * Returns the application's debug front end (DebugFE).
 *
 * @param ssxApp  the application object
 * @return DebugFE (+0x80)
 */
extern "C" int cSSXApp_getDebugFE(cSSXApp* ssxApp) {
    return ((int)ssxApp->DebugFE);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getGame);
#ifdef SKIP_ASM
/**
 * Returns the application's game object (Game).
 *
 * @param ssxApp  the application object
 * @return Game (+0x84); 0 outside a game
 */
extern "C" int cSSXApp_getGame(cSSXApp* ssxApp) {
    return ((int)ssxApp->Game);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getFonts);
#ifdef SKIP_ASM
/**
 * Returns the application's UI font interface (Fonts).
 *
 * @param ssxApp  the application object
 * @return Fonts (+0x88)
 */
extern "C" int cSSXApp_getFonts(cSSXApp* ssxApp) {
    return ((int)ssxApp->Fonts);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getFELocale);
#ifdef SKIP_ASM
/**
 * Returns the application's front-end locale (FELocale).
 *
 * @param ssxApp  the application object
 * @return FELocale (+0x8C)
 */
extern "C" int cSSXApp_getFELocale(cSSXApp* ssxApp) {
    return ((int)ssxApp->FELocale);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getInput);
#ifdef SKIP_ASM
/**
 * Returns one player's input object from Inputs.
 *
 * @param index  the player (0 or 1; not range-checked)
 * @return Inputs[@p index] (+0xB0)
 */
extern "C" int cSSXApp_getInput(cSSXApp* self, int index) {
    return *(int *)((char *)self + (index << 2) + 0xB0);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getInputMap);
#ifdef SKIP_ASM
/**
 * Returns one player's input map from InputMaps.
 *
 * @param index  the player (0 or 1; not range-checked)
 * @return InputMaps[@p index] (+0xB8)
 */
extern "C" int cSSXApp_getInputMap(cSSXApp* self, int index) {
    return *(int *)((char *)self + (index << 2) + 0xB8);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getGameManager);
#ifdef SKIP_ASM
/**
 * Returns the application's game mode manager (GameModeMan).
 *
 * @param ssxApp  the application object
 * @return GameModeMan (+0xC0)
 */
extern "C" int cSSXApp_getGameManager(cSSXApp* ssxApp) {
    return ((int)ssxApp->GameModeMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getUserName);
#ifdef SKIP_ASM
/**
 * Returns the address of the application's UserName string.
 *
 * @param ssxApp  the application object (taken as an int)
 * @return @p ssxApp + 0xC4, the UserName cBXString
 */
extern "C" int cSSXApp_getUserName(int ssxApp) {
    return ssxApp + 196;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getPassword);
#ifdef SKIP_ASM
/**
 * Returns the address of the application's Password string.
 *
 * @param ssxApp  the application object (taken as an int)
 * @return @p ssxApp + 0xC8, the Password cBXString
 */
extern "C" int cSSXApp_getPassword(int ssxApp) {
    return ssxApp + 200;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getNetMan);
#ifdef SKIP_ASM
/**
 * Returns the application's network manager (NetMan).
 *
 * @param ssxApp  the application object
 * @return NetMan (+0xD0)
 */
extern "C" int cSSXApp_getNetMan(cSSXApp* ssxApp) {
    return ((int)ssxApp->NetMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getConsoleConfig);
#ifdef SKIP_ASM
/**
 * Returns the application's console configuration (ConsoleConfig).
 *
 * @param ssxApp  the application object
 * @return ConsoleConfig (+0x9C)
 */
extern "C" int cSSXApp_getConsoleConfig(cSSXApp* ssxApp) {
    return ((int)ssxApp->ConsoleConfig);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getLoadIndicator);
#ifdef SKIP_ASM
/**
 * Returns the application's load indicator (LoadIndicator).
 *
 * @param ssxApp  the application object
 * @return LoadIndicator (+0xA4)
 */
extern "C" int cSSXApp_getLoadIndicator(cSSXApp* ssxApp) {
    return ((int)ssxApp->LoadIndicator);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getNumInputs);
#ifdef SKIP_ASM
/**
 * Returns the number of player inputs: always 2.
 *
 * @return 2
 */
extern "C" int cSSXApp_getNumInputs(void) {
    return 2;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getLoadIconTextureID);
#ifdef SKIP_ASM
/**
 * Returns the texture id of the loading icon (LoadIconTextureID).
 *
 * @param ssxApp  the application object
 * @return LoadIconTextureID (+0xA0)
 */
extern "C" int cSSXApp_getLoadIconTextureID(cSSXApp* ssxApp) {
    return (ssxApp->LoadIconTextureID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_setLoadIconTextureID);
#ifdef SKIP_ASM
/**
 * Sets the texture id of the loading icon (LoadIconTextureID).
 *
 * @param ssxApp  the application object
 * @param textureId  the new texture id
 */
extern "C" void cSSXApp_setLoadIconTextureID(cSSXApp* ssxApp, int textureId) {
    (ssxApp->LoadIconTextureID) = textureId;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_endFE);
#ifdef SKIP_ASM
/**
 * Clears both front-end pointers, RealFE and DebugFE, marking the front end as ended.
 */
extern "C" void cSSXApp_endFE(cSSXApp* self) {
    self->RealFE = 0;
    self->DebugFE = 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_endGame);
#ifdef SKIP_ASM
/**
 * Clears the Game pointer, marking the game as ended.
 *
 * @param ssxApp  the application object
 */
extern "C" void cSSXApp_endGame(void *ssxApp) {
    (*(int *)((char*)(ssxApp) + (0x84))) = 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_InGame);
#ifdef SKIP_ASM
/**
 * Returns whether a game is running (Game is set).
 *
 * @param ssxApp  the application object
 * @return 1 when Game (+0x84) is non-null, else 0
 */
extern "C" int cSSXApp_InGame(cSSXApp* ssxApp) {
    return ((int)ssxApp->Game) != 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_InFE);
#ifdef SKIP_ASM
/**
 * Returns whether the front end is running (RealFE or DebugFE is set).
 *
 * @param ssxApp  the application object
 * @return 1 when RealFE (+0x7C) or DebugFE (+0x80) is non-null, else 0
 */
extern "C" int cSSXApp_InFE(cSSXApp* ssxApp) {
    int var_3;

    var_3 = 0;
    if ((((int)ssxApp->RealFE) != 0) || (((int)ssxApp->DebugFE) != 0)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_GetFEAsyncManager);
#ifdef SKIP_ASM
/**
 * Returns the application's front-end async manager (mFEAsyncManager).
 *
 * @param ssxApp  the application object
 * @return mFEAsyncManager (+0x11C)
 */
extern "C" int cSSXApp_GetFEAsyncManager(cSSXApp* ssxApp) {
    return ((int)ssxApp->mFEAsyncManager);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFadeScreen__cFadeScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys a screen fade: resets the vtable to the root module vtable D_0046D970 and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cFadeScreen_vtbl, the vtable cFadeScreen_cFadeScreen (the fade constructor) installs
 * and whose loseFocus and update slots are cFadeScreen_loseFocus and cFadeScreen_update.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cFadeScreen__cFadeScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00243A70);
#ifdef SKIP_ASM
/**
 * Sets the screen fade's ending flag (+0x8) to 1.
 *
 * cFadeScreen_cFadeScreen (the fade constructor) clears this flag; cFadeScreen_loseFocus starts the fade-out.
 *
 * @param fadeScreen  the screen fade
 */
extern "C" void func_00243A70(void *fadeScreen) {
    (*(int *)((char*)(fadeScreen) + (8))) = 1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cPreStartScreen__cPreStartScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys the pre-start screen.
 *
 * Only resets the vtable to the root module vtable D_0046D970 (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cPreStartScreen__cPreStartScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cPreStartScreen_load__FPv);
#ifdef SKIP_ASM
/**
 * Loads nothing: the pre-start screen has no data of its own (empty, as in On Tour).
 */
void cPreStartScreen_load(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cInitLoadScreen_cInitLoadScreen);
#ifdef SKIP_ASM
extern char cInitLoadScreen_vtbl[];

/**
 * Constructs the initial load screen: installs cInitLoadScreen_vtbl and clears its two flags (+0x4 and +0x8).
 *
 * @return @p self
 */
extern "C" void *cInitLoadScreen_cInitLoadScreen(cInitLoadScreen* self) {
    self->mVtbl = (void**)cInitLoadScreen_vtbl;
    self->progressive = 0;
    self->ending = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cInitLoadScreen_activateProgressiveMode);
#ifdef SKIP_ASM
/**
 * Sets the initial load screen's flag at +0x4 to 1 (the field after the vtable, cleared by cInitLoadScreen_cInitLoadScreen).
 *
 * cInitLoadScreen_loseFocus uses the other flag (+0x8) as the ending flag.
 *
 * @param initLoadScreen  the initial load screen
 */
extern "C" void cInitLoadScreen_activateProgressiveMode(cInitLoadScreen* initLoadScreen) {
    (initLoadScreen->progressive) = 1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cLoadScreen__cLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys a plain load screen: resets the vtable to the root module vtable D_0046D970 and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cLoadScreen_vtbl, the vtable cLoadScreen_cLoadScreen installs (loseFocus is
 * cLoadScreen_loseFocus).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cLoadScreen__cLoadScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cLoadScreen_cLoadScreen);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_cFadeScreen(void *);
extern char cLoadScreen_vtbl[];

/**
 * Constructs a plain load screen: runs the fade constructor cFadeScreen_cFadeScreen, clears the UI engine pointer
 * (+0xC) and installs vtable cLoadScreen_vtbl.
 *
 * cLoadScreen_vtbl's loseFocus slot is cLoadScreen_loseFocus and its update slot is cLoadScreen_update.
 *
 * @return @p self
 */
extern "C" void *cLoadScreen_cLoadScreen(cLoadScreen* self) {
    cFadeScreen_cFadeScreen(self);
    self->mpUI = 0;
    self->mVtbl = (void**)cLoadScreen_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cStartScreen__cStartScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys the start screen: resets the vtable to the root module vtable D_0046D970 and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cStartScreen_vtbl, whose load slot is cStartScreen_load (loads the "data/ui/su"
 * start screen).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cStartScreen__cStartScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadScreen__cGameLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0046D970[];

/**
 * Destroys the game load screen: resets the vtable to the root module vtable D_0046D970 and frees the object when
 * asked.
 *
 * It is the destructor slot (+0xC) of cGameLoadScreen_vtbl, whose load slot is cGameLoadScreen_load (loads the
 * "data/ui/gl" game load screen); cGameLoadScreen_cGameLoadScreen installs that vtable.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGameLoadScreen__cGameLoadScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadScreen_cGameLoadScreen);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_cFadeScreen(void *);
extern char cGameLoadScreen_vtbl[];

/**
 * Constructs the game load screen: runs the fade constructor cFadeScreen_cFadeScreen, clears the UI engine pointer
 * (+0xC), installs vtable cGameLoadScreen_vtbl and clears its two texture ids (+0x10, +0x14).
 *
 * @return @p self
 */
extern "C" void *cGameLoadScreen_cGameLoadScreen(cGameLoadScreen* self) {
    cFadeScreen_cFadeScreen(self);
    self->mpUI = 0;
    self->mVtbl = (void**)cGameLoadScreen_vtbl;
    self->mTexID[0] = 0;
    self->mTexID[1] = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_00243BF0);
#ifdef SKIP_ASM
/**
 * Returns one of the game load screen's two texture ids (+0x10).
 *
 * @param index  which texture (0 or 1; not range-checked)
 * @return the texture id at +0x10 + 4 * @p index
 */
extern "C" int func_00243BF0(void *self, int index) {
    return *(int *)((char *)self + (index << 2) + 0x10);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFELoadScreen__cFELoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_0046D970[];

/**
 * Destroys a front-end load screen.
 *
 * Only resets the vtable to the root module vtable D_0046D970 (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cFELoadScreen__cFELoadScreen(int* self, int flags)
{
    *(void**)self = D_0046D970;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFELoadScreen_cFELoadScreen);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_cFadeScreen(void *);
extern char cFELoadScreen_vtbl[];

/**
 * Constructs a front-end load screen: runs the fade constructor cFadeScreen_cFadeScreen, clears the UI engine pointer
 * (+0xC), installs cFELoadScreen_vtbl and clears its two texture ids (+0x10, +0x14).
 *
 * @return @p self
 */
extern "C" void *cFELoadScreen_cFELoadScreen(cFELoadScreen* self) {
    cFadeScreen_cFadeScreen(self);
    self->mpUI = 0;
    self->mVtbl = (void**)cFELoadScreen_vtbl;
    self->mTexID[0] = 0;
    self->mTexID[1] = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cFELoadScreen_getTexttureID);
#ifdef SKIP_ASM
/**
 * Returns one of the front-end load screen's two texture ids (+0x10).
 *
 * @param index  which texture (0 or 1; not range-checked)
 * @return the texture id at +0x10 + 4 * @p index
 */
extern "C" int cFELoadScreen_getTexttureID(cFELoadScreen* self, int index) {
    return *(int *)((char *)self + (index << 2) + 0x10);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState__cGameState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl and frees the
 * object when asked.
 *
 * cGameState_vtbl is the base vtable whose slots are the cGameState_* defaults (preloadInit, preloadUpdate, preloadCancel,
 * loseFocus, isPreloadComplete, onWidgetEvent).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGameState__cGameState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadInit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadInit has an empty body.
 */
void cGameState_preloadInit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadUpdate has an empty body.
 */
void cGameState_preloadUpdate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadCancel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadCancel has an empty body.
 */
void cGameState_preloadCancel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_gainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's gainFocus has an empty body.
 *
 * It fills slot +0x2C of the base game-state vtable cGameState_vtbl and of the states that keep the default (the end-wait
 * and transport-wait states).
 */
void cGameState_gainFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's update has an empty body.
 *
 * It fills slot +0x34 of the base game-state vtable cGameState_vtbl and of the states without their own update (end,
 * end-wait, transport-wait, riding).
 */
void cGameState_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's loseFocus has an empty body.
 */
void cGameState_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_isPreloadComplete__FPv);
#ifdef SKIP_ASM
/**
 * Reports the base game state's preload as already complete.
 *
 * @return always 1
 */
int cGameState_isPreloadComplete(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_onWidgetEvent__FPv);
#ifdef SKIP_ASM
/**
 * Ignores widget events: the base game state's onWidgetEvent has an empty body.
 */
void cGameState_onWidgetEvent(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFEndState__cGFEndState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the end game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl and frees
 * the object when asked.
 *
 * It is the destructor slot of cGFEndState_vtbl, the vtable cGFEndState_cGFEndState installs (gainFocus cGFEndState_gainFocus, loseFocus
 * cGFEndState_loseFocus).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFEndState__cGFEndState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFEndState_cGFEndState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFEndState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the end game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then next
 * state 8 (+0x0), vtable cGFEndState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * In On Tour's eGFState numbering 8 is kGFSM_EndWait, the state that follows the end state.
 *
 * @return @p self
 */
extern "C" void *cGFEndState_cGFEndState(cGFEndState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 8;
    self->mVtbl = (void**)cGFEndState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFEndWaitState__cGFEndWaitState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the end-wait game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl and
 * frees the object when asked.
 *
 * It is the destructor slot of cGFEndWaitState_vtbl, a vtable that keeps every other default (On Tour's cGFEndWaitState overrides
 * only the replay methods).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFEndWaitState__cGFEndWaitState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFFinishlineState__cGFFinishlineState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the finish-line game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl
 * and frees the object when asked.
 *
 * It is the destructor slot of cGFFinishlineState_vtbl, whose gainFocus, update and loseFocus (cGFFinishlineState_gainFocus, cGFFinishlineState_update,
 * cGFFinishlineState_loseFocus) sit right before cGFFinishlineState_writeToReplayFrame in fe/festateloadscreen.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFFinishlineState__cGFFinishlineState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFTransportWaitState__cGFTransportWaitState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the transport-wait game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable
 * cGameState_vtbl and frees the object when asked.
 *
 * It is the destructor slot of cGFTransportWaitState_vtbl, whose loseFocus slot is cGFTransportWaitState_loseFocus.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFTransportWaitState__cGFTransportWaitState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFTransportWaitState_cGFTransportWaitState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFTransportWaitState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the transport-wait game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared),
 * then next state 10 (+0x0), vtable cGFTransportWaitState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFTransportWaitState_cGFTransportWaitState(cGFTransportWaitState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 10;
    self->mVtbl = (void**)cGFTransportWaitState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFEventStartState__cGFEventStartState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFEventStartState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees
 * the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFEventStartState__cGFEventStartState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFGateState__cGFGateState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFGateState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees the
 * object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFGateState__cGFGateState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFGateState_cGFGateState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFGateState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the gate game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then next
 * state 4 (+0x0), cGFGateState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFGateState_cGFGateState(cGFGateState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 4;
    self->mVtbl = (void**)cGFGateState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFRidingState__cGFRidingState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the riding game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl and
 * frees the object when asked.
 *
 * It is the destructor slot of cGFRidingState_vtbl, whose gainFocus is cGFRidingState_gainFocus.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFRidingState__cGFRidingState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFRidingState_cGFRidingState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFRidingState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the riding game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then
 * next state 5 (+0x0), vtable cGFRidingState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFRidingState_cGFRidingState(cGFRidingState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 5;
    self->mVtbl = (void**)cGFRidingState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFTrackStartState__cGFTrackStartState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFTrackStartState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees
 * the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFTrackStartState__cGFTrackStartState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFNextHeatState__cGFNextHeatState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFNextHeatState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees
 * the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFNextHeatState__cGFNextHeatState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFNextHeatState_cGFNextHeatState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFNextHeatState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the next-heat game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then
 * cGFNextHeatState_vtbl (+0xC), next state 1 (+0x0), startup flag 3 (+0x4) and two cleared fields (+0x14, +0x18).
 *
 * @return @p self
 */
extern "C" void *cGFNextHeatState_cGFNextHeatState(cGFNextHeatState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mVtbl = (void**)cGFNextHeatState_vtbl;
    self->mNextState = 1;
    self->mStartupFlag = 3;
    self->WaitTicks = 0;
    self->PlayTransport = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFSessionTeleportState__cGFSessionTeleportState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFSessionTeleportState game-flow state: restores the base game-state vtable (cGameState_vtbl), then
 * frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFSessionTeleportState__cGFSessionTeleportState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFSessionTeleportState_cGFSessionTeleportState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFSessionTeleportState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the session-teleport game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame
 * cleared), then next state 10 (+0x0), cGFSessionTeleportState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFSessionTeleportState_cGFSessionTeleportState(cGFSessionTeleportState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 10;
    self->mVtbl = (void**)cGFSessionTeleportState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFUserSelectState__cGFUserSelectState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFUserSelectState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees
 * the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFUserSelectState__cGFUserSelectState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFUserSelectState_setNextLocation);
#ifdef SKIP_ASM
/**
 * Sets the user-select state's next location (+0x18).
 *
 * @param userSelectState  the user-select game-flow state
 * @param location  the location to go to next
 */
extern "C" void cGFUserSelectState_setNextLocation(cGFUserSelectState* userSelectState, int location) {
    (userSelectState->mNextLocation) = location;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFSectionLoadState__cGFSectionLoadState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the section-load game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl
 * and frees the object when asked.
 *
 * It is the destructor slot of cGFSectionLoadState_vtbl, whose gainFocus/update/loseFocus are cGFSectionLoadState_*.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFSectionLoadState__cGFSectionLoadState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPreEventState__cGFPreEventState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the pre-event game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl and
 * frees the object when asked.
 *
 * It is the destructor slot of cGFPreEventState_vtbl, whose gainFocus/update/loseFocus are cGFPreEventState_*.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFPreEventState__cGFPreEventState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPreEventState_cGFPreEventState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFPreEventState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the pre-event game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then
 * next state 3 (+0x0), vtable cGFPreEventState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFPreEventState_cGFPreEventState(cGFPreEventState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 3;
    self->mVtbl = (void**)cGFPreEventState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPostEventState__cGFPostEventState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys the post-event game-flow state: resets the vtable pointer (+0xC) to the base game-state vtable cGameState_vtbl
 * and frees the object when asked.
 *
 * It is the destructor slot of cGFPostEventState_vtbl, whose gainFocus/update/loseFocus are cGFPostEventState_*.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGFPostEventState__cGFPostEventState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPostEventState_cGFPostEventState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFPostEventState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the post-event game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared),
 * then next state 8 (+0x0), vtable cGFPostEventState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFPostEventState_cGFPostEventState(cGFPostEventState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 8;
    self->mVtbl = (void**)cGFPostEventState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPodiumState__cGFPodiumState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFPodiumState game-flow state: restores the base game-state vtable (cGameState_vtbl), then frees the
 * object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFPodiumState__cGFPodiumState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFPodiumState_cGFPodiumState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFPodiumState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the podium game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared), then
 * next state 7 (+0x0), cGFPodiumState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFPodiumState_cGFPodiumState(cGFPodiumState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 7;
    self->mVtbl = (void**)cGFPodiumState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFReplayStartState__cGFReplayStartState);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameState_vtbl[];

/**
 * Destroys a cGFReplayStartState game-flow state: restores the base game-state vtable (cGameState_vtbl), then
 * frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cGFReplayStartState__cGFReplayStartState(int* self, int flags)
{
    *(void**)((char*)self + 0xC) = cGameState_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFReplayStartState_cGFReplayStartState);
#ifdef SKIP_ASM
extern char cGameState_vtbl[];
extern char cGFReplayStartState_vtbl[];
extern int cGameState_mpGame;

/**
 * Constructs the replay-start game-flow state: base game-state set-up (vtable cGameState_vtbl, cGameState_mpGame cleared),
 * then next state 4 (+0x0), cGFReplayStartState_vtbl (+0xC) and startup flag 0 (+0x4).
 *
 * @return @p self
 */
extern "C" void *cGFReplayStartState_cGFReplayStartState(cGFReplayStartState* self) {
    self->mNextState = 0;
    self->mVtbl = (void**)cGameState_vtbl;
    cGameState_mpGame = 0;
    self->mNextState = 4;
    self->mVtbl = (void**)cGFReplayStartState_vtbl;
    self->mStartupFlag = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_00244230);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body; no callers or vtable references).
 */
extern "C" void func_00244230(void) {

}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_00244238);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body; no callers or vtable references).
 */
extern "C" void func_00244238(void) {

}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00244240);
#ifdef SKIP_ASM
extern int D_004428F0[];
extern int D_005366E8[];
extern int cPauseState_Flags;
extern int D_004A2A54;

/**
 * Makes @p id the current pause owner: loads its pause flags and records it on top of the pause stack.
 *
 * Sets cPauseState_Flags to D_004428F0[@p id] (one flag word per owner) and stores @p id at D_005366E8[D_004A2A54] (the
 * stack entry at the current depth, which it does not change). cGame_cGame calls it three times.
 *
 * @param id  the pause owner (index into the flag table)
 */
extern "C" void func_00244240(int id)
{
    cPauseState_Flags = D_004428F0[id];
    D_005366E8[D_004A2A54] = id;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentState);
#ifdef SKIP_ASM
/**
 * Returns the current game-flow state number (mCurrentStateID).
 *
 * @param game  the game
 * @return mCurrentStateID (+0x214)
 */
extern "C" int cGame_getCurrentState(cGame* game) {
    return (game->mCurrentStateID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getPreviousState);
#ifdef SKIP_ASM
/**
 * Returns the previous game-flow state number (mPrevStateID).
 *
 * @param game  the game
 * @return mPrevStateID (+0x218)
 */
extern "C" int cGame_getPreviousState(cGame* game) {
    return (game->mPrevStateID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentStateInstance);
#ifdef SKIP_ASM
/**
 * Returns the current game-flow state object (mCurrentState).
 *
 * @param game  the game
 * @return mCurrentState (+0x200)
 */
extern "C" int cGame_getCurrentStateInstance(cGame* game) {
    return ((int)game->mCurrentState);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getNash);
#ifdef SKIP_ASM
/**
 * Returns the game's Nash ray-query object (BXNash).
 *
 * @param game  the game
 * @return BXNash (+0x20)
 */
extern "C" int cGame_getNash(cGame* game) {
    return ((int)game->BXNash);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorld);
#ifdef SKIP_ASM
/**
 * Returns the game's world (World).
 *
 * @param game  the game
 * @return World (+0x10)
 */
extern "C" int cGame_getWorld(cGame* game) {
    return ((int)game->World);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getDebugFont);
#ifdef SKIP_ASM
/**
 * Returns the game's debug font (DebugFont).
 *
 * @param game  the game
 * @return DebugFont (+0x24)
 */
extern "C" int cGame_getDebugFont(cGame* game) {
    return ((int)game->DebugFont);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getMenuSys);
#ifdef SKIP_ASM
/**
 * Returns the game's menu system (MenuSys).
 *
 * @param game  the game
 * @return MenuSys (+0xA4)
 */
extern "C" int cGame_getMenuSys(cGame* game) {
    return ((int)game->MenuSys);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getAI);
#ifdef SKIP_ASM
/**
 * Returns the game's AI (AI).
 *
 * @param game  the game
 * @return AI (+0xC)
 */
extern "C" int cGame_getAI(cGame* game) {
    return ((int)game->AI);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getUI);
#ifdef SKIP_ASM
/**
 * Returns the game's UI engine (UI).
 *
 * @param game  the game
 * @return UI (+0x48)
 */
extern "C" int cGame_getUI(cGame* game) {
    return ((int)game->UI);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getReplay);
#ifdef SKIP_ASM
/**
 * Returns the game's replay (Replay).
 *
 * @param game  the game
 * @return Replay (+0x28)
 */
extern "C" int cGame_getReplay(cGame* game) {
    return ((int)game->Replay);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getFXLoader);
#ifdef SKIP_ASM
/**
 * Returns the game's effects loader (FXLoader).
 *
 * @param game  the game
 * @return FXLoader (+0x2C)
 */
extern "C" int cGame_getFXLoader(cGame* game) {
    return ((int)game->FXLoader);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSurfPropCache);
#ifdef SKIP_ASM
/**
 * Returns the game's surface property cache (SurfPropCache).
 *
 * @param game  the game
 * @return SurfPropCache (+0x44)
 */
extern "C" int cGame_getSurfPropCache(cGame* game) {
    return ((int)game->SurfPropCache);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorldSound);
#ifdef SKIP_ASM
/**
 * Returns the game's world sound (WorldSound).
 *
 * @param game  the game
 * @return WorldSound (+0x4C)
 */
extern "C" int cGame_getWorldSound(cGame* game) {
    return ((int)game->WorldSound);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getAIVisualEffectsObjNode);
#ifdef SKIP_ASM
/**
 * Returns the game's AI visual-effects object node (AIVisualEffectsMan).
 *
 * @param game  the game
 * @return AIVisualEffectsMan (+0x50)
 */
extern "C" int cGame_getAIVisualEffectsObjNode(cGame* game) {
    return ((int)game->AIVisualEffectsMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getVisualEffectsObjNode);
#ifdef SKIP_ASM
/**
 * Returns the game's visual-effects object node (VisualEffectsMan).
 *
 * @param game  the game
 * @return VisualEffectsMan (+0x54)
 */
extern "C" int cGame_getVisualEffectsObjNode(cGame* game) {
    return ((int)game->VisualEffectsMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCutSceneTransitionman);
#ifdef SKIP_ASM
/**
 * Returns the game's cut-scene transition manager (CutSceneTransitionMan).
 *
 * @param game  the game
 * @return CutSceneTransitionMan (+0x64)
 */
extern "C" int cGame_getCutSceneTransitionman(cGame* game) {
    return ((int)game->CutSceneTransitionMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getFlagManager);
#ifdef SKIP_ASM
/**
 * Returns the game's flag manager (FlagManager).
 *
 * @param game  the game
 * @return FlagManager (+0x70)
 */
extern "C" int cGame_getFlagManager(cGame* game) {
    return ((int)game->FlagManager);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSnowFall);
#ifdef SKIP_ASM
/**
 * Returns the game's snowfall effect (SnowFall).
 *
 * @param game  the game
 * @return SnowFall (+0x74)
 */
extern "C" int cGame_getSnowFall(cGame* game) {
    return ((int)game->SnowFall);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getPlaybackMan);
#ifdef SKIP_ASM
/**
 * Returns the game's replay playback manager (PlaybackMan).
 *
 * @param game  the game
 * @return PlaybackMan (+0x34)
 */
extern "C" int cGame_getPlaybackMan(cGame* game) {
    return ((int)game->PlaybackMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSectionMan);
#ifdef SKIP_ASM
/**
 * Returns the game's section manager (SectionMan).
 *
 * @param game  the game
 * @return SectionMan (+0x78)
 */
extern "C" int cGame_getSectionMan(cGame* game) {
    return ((int)game->SectionMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getInputSync);
#ifdef SKIP_ASM
/**
 * Returns the game's input synchroniser (InputSync).
 *
 * @param game  the game
 * @return InputSync (+0x7C)
 */
extern "C" int cGame_getInputSync(cGame* game) {
    return ((int)game->InputSync);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSkyBox);
#ifdef SKIP_ASM
/**
 * Returns the game's sky box (SkyBox).
 *
 * @param game  the game
 * @return SkyBox (+0x14)
 */
extern "C" int cGame_getSkyBox(cGame* game) {
    return ((int)game->SkyBox);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCrowdMan2d);
#ifdef SKIP_ASM
/**
 * Returns the game's 2D crowd manager (CrowdMan2d).
 *
 * @param game  the game
 * @return CrowdMan2d (+0x38)
 */
extern "C" int cGame_getCrowdMan2d(cGame* game) {
    return ((int)game->CrowdMan2d);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCrowdMan3d);
#ifdef SKIP_ASM
/**
 * Returns the game's 3D crowd manager (CrowdMan3d).
 *
 * @param game  the game
 * @return CrowdMan3d (+0x3C)
 */
extern "C" int cGame_getCrowdMan3d(cGame* game) {
    return ((int)game->CrowdMan3d);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getMultiParticleMan);
#ifdef SKIP_ASM
/**
 * Returns the game's multi-particle manager (MultiParticleMan).
 *
 * @param game  the game
 * @return MultiParticleMan (+0x40)
 */
extern "C" int cGame_getMultiParticleMan(cGame* game) {
    return ((int)game->MultiParticleMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getInstanceMan);
#ifdef SKIP_ASM
/**
 * Returns the game's instance manager (InstanceMan).
 *
 * @param game  the game
 * @return InstanceMan (+0x88)
 */
extern "C" int cGame_getInstanceMan(cGame* game) {
    return ((int)game->InstanceMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getOcclusionMan);
#ifdef SKIP_ASM
/**
 * Returns the game's occlusion manager (OcclusionMan).
 *
 * @param game  the game
 * @return OcclusionMan (+0x8C)
 */
extern "C" int cGame_getOcclusionMan(cGame* game) {
    return ((int)game->OcclusionMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorldLightMan);
#ifdef SKIP_ASM
/**
 * Returns the game's world light manager (WorldLightMan).
 *
 * @param game  the game
 * @return WorldLightMan (+0x90)
 */
extern "C" int cGame_getWorldLightMan(cGame* game) {
    return ((int)game->WorldLightMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getLoadIndicator);
#ifdef SKIP_ASM
/**
 * Returns the game's load indicator (LoadIndicator).
 *
 * @param game  the game
 * @return LoadIndicator (+0x94)
 */
extern "C" int cGame_getLoadIndicator(cGame* game) {
    return ((int)game->LoadIndicator);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getControllerUnplugged);
#ifdef SKIP_ASM
/**
 * Returns the game's controller-unplugged flag (ControllerUnplugged).
 *
 * @param game  the game
 * @return ControllerUnplugged (+0x224)
 */
extern "C" int cGame_getControllerUnplugged(cGame* game) {
    return (game->ControllerUnplugged);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getViewMan);
#ifdef SKIP_ASM
/**
 * Returns the game's game view manager (ViewMan).
 *
 * @param game  the game
 * @return ViewMan (+0x84)
 */
extern "C" int cGame_getViewMan(cGame* game) {
    return ((int)game->ViewMan);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getGameView);
#ifdef SKIP_ASM
/**
 * Returns one camera view from the game's view manager (ViewMan): the camera pointer at +0x4 + 4 * @p index of it.
 *
 * @param index  the view number (not range-checked)
 * @return the view's camera
 */
extern "C" int cGame_getGameView(cGame* self, int index) {
    return *(int *)((char *)((int)self->ViewMan) + (index << 2) + 4);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getNumGameView);
#ifdef SKIP_ASM
/**
 * Returns the number of camera views in the game's view manager (ViewMan +0x10).
 *
 * @param game  the game
 * @return the view count
 */
extern "C" int cGame_getNumGameView(cGame* game) {
    return (*(int *)((char*)((game->ViewMan)) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentRenderView);
#ifdef SKIP_ASM
/**
 * Returns the camera of the view being rendered: ViewMan's camera indexed by its current view number (+0x14).
 *
 * @return the current render view's camera
 */
extern "C" int cGame_getCurrentRenderView(cGame* self) {
    char *list = (char*)self->ViewMan;
    return *(int *)(list + (*(int *)(list + 0x14) << 2) + 4);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentRenderViewNum);
#ifdef SKIP_ASM
/**
 * Returns the number of the view being rendered (ViewMan +0x14).
 *
 * @param game  the game
 * @return the current render view number
 */
extern "C" int cGame_getCurrentRenderViewNum(cGame* game) {
    return (*(int *)((char*)((game->ViewMan)) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002443C0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_0047DB28[];

/**
 * Destroys the game's stream-pause object: resets the vtable to D_0047DB28 and frees the object when asked.
 *
 * It is the destructor slot of D_0047D0E8, the vtable cStreamPause_construct installs ("GenericStreamPause");
 * D_0047DB28 is the base vtable (destructor func_00245F00, two pure-virtual slots __pure_virtual).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void func_002443C0(int* self, int flags)
{
    *(void**)self = D_0047DB28;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGenericStreamPause_StreamFocusRender);
#ifdef SKIP_ASM
/**
 * Reports the stream focus as rendered exactly when the stream is ready: returns @p streamReady unchanged.
 *
 * Slot +0x14 of the generic stream-pause vtable D_0047D0E8.
 *
 * @param streamReady  whether the stream is ready
 * @return @p streamReady
 */
extern "C" void* cGenericStreamPause_StreamFocusRender(void* self, void* streamReady)
{
    return streamReady;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGenericStreamPause_Flush__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1: the generic stream pause always reports the flush as done.
 *
 * Slot +0x1C of the generic stream-pause vtable D_0047D0E8.
 *
 * @return 1
 */
int cGenericStreamPause_Flush(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cInput_setPad__FPvi);
#ifdef SKIP_ASM
/**
 * Sets an input's pad (+0x0).
 *
 * cSSXApp_initload, cInputSync_createlocalinput / createremoteinput and cPlaybackMan_cPlaybackMan call it on new 8-byte
 * "cInput" objects with a 0x2A4-byte "cInputPad" block.
 *
 * @param val  the pad (a pointer passed as an int)
 */
void cInput_setPad(void* vself, int val)
{
    cInput* self = (cInput*)vself;
    self->Pad = (cInputPad*)val;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cInput_setMap__FPvi);
#ifdef SKIP_ASM
/**
 * Sets an input's input map (+0x4).
 *
 * Called right after cInput_setPad on the same new "cInput" objects.
 *
 * @param val  the input map (passed as an int)
 */
void cInput_setMap(void* vself, int val)
{
    cInput* self = (cInput*)vself;
    self->Map = (cInputMap*)val;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cVisualEffectsObjNode__cVisualEffectsObjNode);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void* self, int flags);
extern void* cVisualEffectsObjNode_vtbl[];
struct sVEntry_cVisualEffectsObjNode__cVisualEffectsObjNode {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys the visual-effects object node: deletes the cVisualEffectsMan it owns, then runs the cObjNode destructor.
 *
 * Installs cVisualEffectsObjNode_vtbl at +0xC, destroys the object at +0x10 (if any) through its virtual destructor (slot 1, vptr at
 * +0x44, flags 3: destroy and free) and calls cObjNode__cObjNode with @p flags. cGame_load builds this node for
 * "VisualEffectsMan" with vtable cVisualEffectsObjNode_vtbl.
 *
 * @param flags  destructor flags, passed on to cObjNode__cObjNode
 */
extern "C" void cVisualEffectsObjNode__cVisualEffectsObjNode(cVisualEffectsObjNode* self, int flags)
{
    self->mVtbl = cVisualEffectsObjNode_vtbl;
    void* obj = self->VisualEffectsMan;
    if (obj != 0) {
        sVEntry_cVisualEffectsObjNode__cVisualEffectsObjNode* vt = *(sVEntry_cVisualEffectsObjNode__cVisualEffectsObjNode**)((char*)obj + 0x44);
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    cObjNode__cObjNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cVisualEffectsObjNode_update);
#ifdef SKIP_ASM
class cAppDtorVirt44 {
public:
    char pad[0x44];
    // vptr lands at 0x44 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void update();
    virtual void render();
};

/**
 * Updates the visual-effects object node: calls the owned cVisualEffectsMan's (+0x10) virtual slot 2 (vptr at +0x44).
 *
 * Slot +0x14 (update) of cVisualEffectsObjNode_vtbl.
 */
extern "C" void cVisualEffectsObjNode_update(cVisualEffectsObjNode* self)
{
    ((cAppDtorVirt44*)self->VisualEffectsMan)->update();
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cVisualEffectsObjNode_render);
#ifdef SKIP_ASM
/**
 * Renders the visual-effects object node: calls the owned cVisualEffectsMan's (+0x10) virtual slot 3 (vptr at +0x44).
 *
 * Slot +0x24 (render) of cVisualEffectsObjNode_vtbl.
 */
extern "C" void cVisualEffectsObjNode_render(cVisualEffectsObjNode* self)
{
    ((cAppDtorVirt44*)self->VisualEffectsMan)->render();
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_cGameModeHandler);
#ifdef SKIP_ASM
extern char cGameModeHandler_vtbl[];

/**
 * Constructs a game mode handler: installs the base handler vtable cGameModeHandler_vtbl.
 *
 * cGameModeHandler_vtbl's slots are the base cGameModeHandler methods (cGameModeHandler_purge, cleanRiderRanksPoints,
 * cleanRiderRanksTime and the stubs after cGameModeHandler_setManager).
 *
 * @param handler  the handler
 * @return @p handler
 */
extern "C" int **cGameModeHandler_cGameModeHandler(int **handler) {
    *handler = (int*)&cGameModeHandler_vtbl;
    return handler;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler__cGameModeHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys a game mode handler: resets the vtable to the base handler vtable cGameModeHandler_vtbl and frees the object when
 * asked.
 *
 * It is the destructor slot of cGameModeHandler_vtbl itself.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGameModeHandler__cGameModeHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_setManager);
#ifdef SKIP_ASM

/**
 * Sets the game mode manager that every game mode handler uses (the static cGameModeHandler_pManager).
 *
 * @param gameModeMan  the cGameModeMan (passed as an int)
 */
extern "C" void cGameModeHandler_setManager(int gameModeMan) {
    cGameModeHandler_pManager = (cGameModeMan*)(gameModeMan);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_init__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game mode handler's init.
 *
 * Slot +0x14 of the base handler vtable cGameModeHandler_vtbl; the event handlers override it with their heat set-up (for example
 * cGMBigAirHandler_init, cGMRaceHandler_init).
 */
void cGameModeHandler_init(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_riderCheckpointHit__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1: the base game mode handler's checkpoint handler accepts every checkpoint.
 *
 * Slot +0x34 of the base handler vtable cGameModeHandler_vtbl (and of the handlers that keep it); the overrides (for example
 * func_002398E8) take the rider and the seconds added.
 *
 * @return 1
 */
int cGameModeHandler_riderCheckpointHit(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_riderGateHit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game mode handler's gate handler.
 *
 * Slot +0x3C of the base handler vtable cGameModeHandler_vtbl and of most event handlers.
 */
void cGameModeHandler_riderGateHit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_raceFinish__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game mode handler's race-finish handler.
 *
 * Slot +0x44 of the base handler vtable cGameModeHandler_vtbl; the event handlers override it to record a rider's result (for
 * example func_00239230, cGMRaceHandler_raceFinish).
 */
void cGameModeHandler_raceFinish(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_getHeatScore__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the base game mode handler has no heat scores.
 *
 * Slot +0x4C of the base handler vtable cGameModeHandler_vtbl; func_00239CE0 overrides it for the scored event.
 *
 * @return 0
 */
int cGameModeHandler_getHeatScore(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game mode handler saves nothing into a replay frame.
 *
 * Slot +0x54 of the base handler vtable cGameModeHandler_vtbl; func_00239D18 overrides it for the scored event.
 */
void cGameModeHandler_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameModeHandler_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game mode handler restores nothing from a replay frame.
 *
 * Slot +0x5C of the base handler vtable cGameModeHandler_vtbl; func_00239D50 overrides it for the scored event.
 */
void cGameModeHandler_readFromReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMBigAirHandler__cGMBigAirHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the scored three-heat event handler (handler 0, vtable D_0047CFA0): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMBigAirHandler__cGMBigAirHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMRaceHandler__cGMRaceHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the timed three-heat event handler (handler 1, vtable D_0047CF38): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMRaceHandler__cGMRaceHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPRaceHandler__cGMMPRaceHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the online race handler (handler 8, vtable D_0047CED0): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMMPRaceHandler__cGMMPRaceHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPSlopeHandler__cGMMPSlopeHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the two-player online event handler (handler 9, vtable D_0047CE68): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMMPSlopeHandler__cGMMPSlopeHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPSlopeHandler_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: handler 9 (the two-player online event, vtable D_0047CE68) saves nothing into a replay frame.
 *
 * Slot +0x54 (writeToReplayFrame) of D_0047CE68.
 */
void cGMMPSlopeHandler_writeToReplayFrame(void* vself)
{
    cGMMPSlopeHandler* self = (cGMMPSlopeHandler*)vself;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPSlopeHandler_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: handler 9 (the two-player online event, vtable D_0047CE68) restores nothing from a replay frame.
 *
 * Slot +0x5C (readFromReplayFrame) of D_0047CE68.
 */
void cGMMPSlopeHandler_readFromReplayFrame(void* vself)
{
    cGMMPSlopeHandler* self = (cGMMPSlopeHandler*)vself;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00244630);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the freeride handler (handler 2, vtable D_0047CE00): resets the vtable to the base handler vtable cGameModeHandler_vtbl
 * and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void func_00244630(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMTimeHandler__cGMTimeHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the located timed-event handler (handler 4, vtable D_0047CD98): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMTimeHandler__cGMTimeHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00244690);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the head-to-head event handler (handler 5, vtable D_0047CD30): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void func_00244690(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002446C0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the head-to-head scored event handler (handler 6, vtable D_0047CCC8): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void func_002446C0(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMPointHandler__cGMPointHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the point-target event handler (handler 7, vtable D_0047CC60): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMPointHandler__cGMPointHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00244720);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the single-heat random line-up handler: resets the vtable to the base handler vtable cGameModeHandler_vtbl and frees
 * the object when asked.
 *
 * Its vtable starts at cMCOverlayManager_vtbl + 0x20 (0x0047CBF8; the label covers both tables), with init
 * cGMDebugHandler_init.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void func_00244720(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getMemCard);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager's card interface (mpMemCard).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return mpMemCard (+0x434)
 */
extern "C" int cMCOverlayManager_getMemCard(cMCOverlayManager* overlayManager) {
    return ((int)overlayManager->mpMemCard);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_GetState);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager's current state (mState).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return mState (+0x130)
 */
extern "C" int cMCOverlayManager_GetState(cMCOverlayManager* overlayManager) {
    return (overlayManager->mState);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isInSaveOperation);
#ifdef SKIP_ASM
/**
 * Returns whether the overlay manager is running a save (mSave).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return mSave (+0xBC)
 */
extern "C" int cMCOverlayManager_isInSaveOperation(cMCOverlayManager* overlayManager) {
    return (overlayManager->mSave);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isInDeleteOperation);
#ifdef SKIP_ASM
/**
 * Returns whether the overlay manager is running a delete (mDelete).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return mDelete (+0xC0)
 */
extern "C" int cMCOverlayManager_isInDeleteOperation(cMCOverlayManager* overlayManager) {
    return (overlayManager->mDelete);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_00244770);
#ifdef SKIP_ASM
/**
 * Calls the overlay manager's callbackIsLoadPressed (+0x78).
 *
 * Declared void here, but the callback's result is left in the return register.
 *
 * @param overlayManager  the cMCOverlayManager
 */
extern "C" void func_00244770(void *overlayManager) {
    (*(int (**)())((char*)(overlayManager) + (0x78)))();
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isCardStatusErrorUnacknowledged);
#ifdef SKIP_ASM
/**
 * Returns whether a card status error is pending acknowledgement.
 *
 * @param overlayManager  the cMCOverlayManager
 * @return 1 when mCurrDeviceStateAck (+0x440) is clear and mCurrDeviceStateLast (+0x43C) is non-zero, else 0
 */
extern "C" int cMCOverlayManager_isCardStatusErrorUnacknowledged(cMCOverlayManager* overlayManager) {
    int var_3;

    var_3 = 0;
    if ((overlayManager->mCurrDeviceStateAck) == 0) {
        var_3 = (overlayManager->mCurrDeviceStateLast) != 0;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_hasCardStatusError);
#ifdef SKIP_ASM
/**
 * Returns whether the last card status is an error (mCurrDeviceStateLast is non-zero).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return 1 when mCurrDeviceStateLast (+0x43C) is non-zero, else 0
 */
extern "C" int cMCOverlayManager_hasCardStatusError(cMCOverlayManager* overlayManager) {
    return (overlayManager->mCurrDeviceStateLast) != 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getCardLastState);
#ifdef SKIP_ASM
/**
 * Returns the last card status word (mCurrDeviceStateLast; bits: 4 full, 8 no card, 0x10 unformatted).
 *
 * @param overlayManager  the cMCOverlayManager
 * @return mCurrDeviceStateLast (+0x43C)
 */
extern "C" int cMCOverlayManager_getCardLastState(cMCOverlayManager* overlayManager) {
    return (overlayManager->mCurrDeviceStateLast);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_forceCardStatusAck);
#ifdef SKIP_ASM
/**
 * Acknowledges the current card status: sets mCurrDeviceStateAck (+0x440) to 1.
 *
 * @param overlayManager  the cMCOverlayManager
 */
extern "C" void cMCOverlayManager_forceCardStatusAck(cMCOverlayManager* overlayManager) {
    (overlayManager->mCurrDeviceStateAck) = 1;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getFirstCorruptFile);
#ifdef SKIP_ASM
/**
 * Returns the first corrupt file's index, from bits 5..8 of mCurrDeviceStateLast (+0x43C), minus 1.
 *
 * @param overlayManager  the cMCOverlayManager
 * @return the 4-bit field minus 1 (-1 when it is 0)
 */
extern "C" int cMCOverlayManager_getFirstCorruptFile(cMCOverlayManager* overlayManager) {
    return (((unsigned int) ((unsigned int)overlayManager->mCurrDeviceStateLast) >> 5) & 0xF) - 1;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_002447F0);
#ifdef SKIP_ASM
extern "C" void strcpy1(int);

/**
 * Copies a 16-bit string into the new file name (mNewFileName, +0x276) with strcpy1.
 *
 * The source string is passed through in the second argument register, which this declaration does not name.
 *
 * @param overlayManager  the cMCOverlayManager (taken as an int)
 */
extern "C" void func_002447F0(int overlayManager) {
    strcpy1(overlayManager + 630);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_00244810);
#ifdef SKIP_ASM
extern "C" void func_002C2540(int);

/**
 * Converts an 8-bit string into the new file name (mNewFileName, +0x276) with func_002C2540.
 *
 * The source string is passed through in the second argument register, which this declaration does not name.
 *
 * @param overlayManager  the cMCOverlayManager (taken as an int)
 */
extern "C" void func_00244810(int overlayManager) {
    func_002C2540(overlayManager + 630);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getNewFileName);
#ifdef SKIP_ASM
/**
 * Returns the overlay manager's new file name (mNewFileName, a 16-bit string).
 *
 * @param overlayManager  the cMCOverlayManager (taken as an int)
 * @return @p overlayManager + 0x276
 */
extern "C" int cMCOverlayManager_getNewFileName(int overlayManager) {
    return overlayManager + 630;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_PollForFullCard);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager polls for a full card (mPollForFullCard, +0x11C).
 *
 * @param overlayManager  the cMCOverlayManager
 * @param pollFullCard  non-zero to poll
 */
extern "C" void cMCOverlayManager_PollForFullCard(cMCOverlayManager* overlayManager, int pollFullCard) {
    (overlayManager->mPollForFullCard) = pollFullCard;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_PollForUnformatted);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager polls for an unformatted card (mPollForUnformatted, +0x120).
 *
 * @param overlayManager  the cMCOverlayManager
 * @param pollUnformatted  non-zero to poll
 */
extern "C" void cMCOverlayManager_PollForUnformatted(cMCOverlayManager* overlayManager, int pollUnformatted) {
    (overlayManager->mPollForUnformatted) = pollUnformatted;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_PollForNoCard);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager polls for a missing card (mPollForNoCard, +0x124).
 *
 * @param overlayManager  the cMCOverlayManager
 * @param pollNoCard  non-zero to poll
 */
extern "C" void cMCOverlayManager_PollForNoCard(cMCOverlayManager* overlayManager, int pollNoCard) {
    (overlayManager->mPollForNoCard) = pollNoCard;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_ShowCardChangeNotify);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager shows the card-changed notice (mShowCardChangeNotify, +0x128).
 *
 * @param overlayManager  the cMCOverlayManager
 * @param showCardChange  non-zero to show it
 */
extern "C" void cMCOverlayManager_ShowCardChangeNotify(cMCOverlayManager* overlayManager, int showCardChange) {
    (overlayManager->mShowCardChangeNotify) = showCardChange;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_RetryToInit);
#ifdef SKIP_ASM
/**
 * Sets the overlay manager's retry-to-init flag (mRetryToInit, +0x12C).
 *
 * @param overlayManager  the cMCOverlayManager
 * @param retryToInit  the new flag value
 */
extern "C" void cMCOverlayManager_RetryToInit(cMCOverlayManager* overlayManager, int retryToInit) {
    (overlayManager->mRetryToInit) = retryToInit;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_CallbackMemcard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the default for the overlay manager's void callbacks.
 *
 * cMCOverlayManager_cMCOverlayManager stores it in every void callback slot until the front end plugs in its own.
 */
void cMCOverlayManager_CallbackMemcard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_CallbackFalse__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: a default for the overlay manager's bool callbacks.
 *
 * cMCOverlayManager_cMCOverlayManager stores it in callback slots such as callbackReadEnd.
 *
 * @return 0
 */
int cMCOverlayManager_CallbackFalse(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_CallbackTrue__FPv);
#ifdef SKIP_ASM
/**
 * Returns 1: a default for the overlay manager's bool callbacks.
 *
 * cMCOverlayManager_cMCOverlayManager stores it in callback slots such as callbackDeleteDone and callbackDeleteFailed.
 *
 * @return 1
 */
int cMCOverlayManager_CallbackTrue(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_Callbackpdate__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the default for the overlay manager's progress callbacks (callbackWriteUpdate, callbackReadUpdate).
 *
 * @return 0
 */
int cMCOverlayManager_Callbackpdate(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameComm_stopSyncInput__FPv);
#ifdef SKIP_ASM
/**
 * Stops exchanging input with the peer: clears the game comm's mSyncInput (+0x40).
 *
 * cGFFinishlineState_gainFocus calls it on cGameComm_mGameComm before the race-end flow.
 */
void cGameComm_stopSyncInput(void* vself)
{
    cGameComm* self = (cGameComm*)vself;
    self->mSyncInput = 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateStreamFocus);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_updateStreamFocus(int);
extern "C" int cGameViewMan_isStreamReady(int);

/**
 * Refreshes the views' stream focus and reports whether the stream is ready.
 *
 * Calls cGameViewMan_updateStreamFocus on ViewMan (+0x84). When IsStreamPaused (+0x9C) is already set it returns 0;
 * otherwise, when cGameViewMan_isStreamReady says the stream is not ready, it sets IsStreamPaused and m_bFlushStreamer
 * (+0xA0) and returns 0.
 *
 * @return 1 when the stream is ready and not paused, else 0
 */
extern "C" int cGame_updateStreamFocus(cGame* self) {
    cGameViewMan_updateStreamFocus((int)self->ViewMan);
    if (self->IsStreamPaused != 0) {
        return 0;
    }
    if (cGameViewMan_isStreamReady((int)self->ViewMan) == 0) {
        self->IsStreamPaused = 1;
        self->m_bFlushStreamer = 1;
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cAIVisualEffectsObjNode__cAIVisualEffectsObjNode);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void* self, int flags);
extern void* cAIVisualEffectsObjNode_vtbl[];
struct sVEntry_cAIVisualEffectsObjNode__cAIVisualEffectsObjNode {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys the AI visual-effects object node: deletes the cAIVisualEffectsMan it owns, then runs the cObjNode
 * destructor.
 *
 * Installs cAIVisualEffectsObjNode_vtbl at +0xC, destroys the object at +0x10 (if any) through its virtual destructor (slot 1, vptr at
 * +0x4, flags 3: destroy and free) and calls cObjNode__cObjNode with @p flags. cGame_load builds this node for
 * "AIVisualEffectsMan" with vtable cAIVisualEffectsObjNode_vtbl.
 *
 * @param flags  destructor flags, passed on to cObjNode__cObjNode
 */
extern "C" void cAIVisualEffectsObjNode__cAIVisualEffectsObjNode(cAIVisualEffectsObjNode* self, int flags)
{
    self->mVtbl = cAIVisualEffectsObjNode_vtbl;
    void* obj = self->AIVisualEffectsMan;
    if (obj != 0) {
        sVEntry_cAIVisualEffectsObjNode__cAIVisualEffectsObjNode* vt = *(sVEntry_cAIVisualEffectsObjNode__cAIVisualEffectsObjNode**)((char*)obj + 0x4);
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    cObjNode__cObjNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cAIVisualEffectsObjNode_update);
#ifdef SKIP_ASM
class cAppDtorVirt04 {
public:
    int field_0x0;
    // vptr lands at 0x4 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void update();
    virtual void render();
};

/**
 * Updates the AI visual-effects object node: calls the owned cAIVisualEffectsMan's (+0x10) virtual slot 2 (vptr at
 * +0x4).
 *
 * Slot +0x14 (update) of cAIVisualEffectsObjNode_vtbl.
 */
extern "C" void cAIVisualEffectsObjNode_update(cAIVisualEffectsObjNode* self)
{
    ((cAppDtorVirt04*)self->AIVisualEffectsMan)->update();
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cAIVisualEffectsObjNode_render);
#ifdef SKIP_ASM
/**
 * Renders the AI visual-effects object node: calls the owned cAIVisualEffectsMan's (+0x10) virtual slot 3 (vptr at
 * +0x4).
 *
 * Slot +0x24 (render) of cAIVisualEffectsObjNode_vtbl.
 */
extern "C" void cAIVisualEffectsObjNode_render(cAIVisualEffectsObjNode* self)
{
    ((cAppDtorVirt04*)self->AIVisualEffectsMan)->render();
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXStatsObjNode__cSSXStatsObjNode);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void* self, int flags);
extern void* cSSXStatsObjNode_vtbl[];

/**
 * Destroys a stats object node: frees the object it holds, then runs the cObjNode destructor.
 *
 * Stores cSSXStatsObjNode's vtable at +0xC and releases the pointer at +0x10 with operator delete (no
 * destructor runs on it).
 *
 * @param flags  g++ destructor flags, passed to cObjNode__cObjNode(); bit 0 set frees the object's memory
 */
extern "C" void cSSXStatsObjNode__cSSXStatsObjNode(cSSXStatsObjNode* self, int flags)
{
    self->mVtbl = cSSXStatsObjNode_vtbl;
    operator_delete((int*)self->SSXStatsMan);
    cObjNode__cObjNode(self, flags);
}
#endif

void cSSXStatsMan_update(void*);

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXStatsObjNode_update);
#ifdef SKIP_ASM
/**
 * Updates the stats node by calling cSSXStatsMan_update() on the object it holds (+0x10); that function is empty.
 */
extern "C" void cSSXStatsObjNode_update(cSSXStatsObjNode* self)
{
    cSSXStatsMan_update(self->SSXStatsMan);
}
#endif

void cSSXStatsMan_render(void*);

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXStatsObjNode_render);
#ifdef SKIP_ASM
/**
 * Renders the stats node by calling cSSXStatsMan_render() on the object it holds (+0x10); that function is empty.
 */
extern "C" void cSSXStatsObjNode_render(cSSXStatsObjNode* self)
{
    cSSXStatsMan_render(self->SSXStatsMan);
}
#endif

extern "C" void* cUIState__cUIState(void* self);

//100%
INCLUDE_ASM("main/ssxappdtor", func_00244A38);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body; no callers or vtable references).
 */
extern "C" void func_00244A38(void) {

}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateStreamLoading);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_updateAll(int);
extern "C" void cSectionMan_update(void *);
extern "C" void cGameViewMan_updateStreamFocus(int);
extern "C" int cGameViewMan_isStreamReady(int);
extern "C" void cGame_updateState(void *);
void cWorld_update(void *);
extern "C" void cWorldCache_flushInActivePages(void *);

/**
 * Updates world streaming for one frame and reports whether the game may run normally.
 *
 * When cGameViewMan_isStreamReady(ViewMan) fails or the section manager's m_bStreamReady (SectionMan +0x1D0) is clear,
 * the game waits on the stream: the streamer's bandwidth limit (world cache +0x3D4) is lifted, cWorld_update and
 * cGameViewMan_updateAll run (plus cGame_updateState and cSectionMan_update when m_bStreamReady is clear),
 * IsStreamPaused (+0x9C) is set and, the first time the views are not ready, m_bFlushStreamer (+0xA0) too; a pending
 * flush calls the graphics manager's vtable slot 19, cGameViewMan_updateStreamFocus and cWorldCache_flushInActivePages,
 * then clears the flag. Otherwise the bandwidth limit goes back on, both flags are cleared and cWorld_update and
 * cSectionMan_update run. cGame_update has the same code inline.
 *
 * @return 0 while waiting on the stream, 1 when the game can update
 */
extern "C" int cGame_updateStreamLoading(cGame* self) {
    void *vtbl;

    if ((cGameViewMan_isStreamReady((int)self->ViewMan) == 0) || (self->SectionMan->m_bStreamReady == 0)) {
        *(int *)(*(char**)self->World + 0x3D4) = 0;
        cWorld_update(self->World);
        cGameViewMan_updateAll((int)self->ViewMan);
        if (self->SectionMan->m_bStreamReady == 0) {
            cGame_updateState(self);
            cSectionMan_update(self->SectionMan);
        }
        if ((self->IsStreamPaused == 0) && (cGameViewMan_isStreamReady((int)self->ViewMan) == 0)) {
            self->m_bFlushStreamer = 1;
        }
        self->IsStreamPaused = 1;
        if (self->m_bFlushStreamer != 0) {
            vtbl = (void*)GrMan->mVtbl;
            (*(void (**)(void *))((char*)vtbl + 0x9C))((char*)GrMan + *(short *)((char*)vtbl + 0x98));
            cGameViewMan_updateStreamFocus((int)self->ViewMan);
            cWorldCache_flushInActivePages(*(void**)self->World);
            self->m_bFlushStreamer = 0;
        }
        return 0;
    }
    *(int *)(*(char**)self->World + 0x3D4) = 1;
    self->IsStreamPaused = 0;
    self->m_bFlushStreamer = 0;
    cWorld_update(self->World);
    cSectionMan_update(self->SectionMan);
    return 1;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateUI);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cOVStateManager_update(void);
extern "C" int cMenuSys_isActive(void* p);
extern "C" void cOVStateManager_triggerOverlay(int id);
extern "C" void* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_PlayIconSound(void* snd, int a, int b, float f);
extern "C" void cWScriptMan_doMissionCmd(void* mgr, int a, int b, int c);
extern "C" void cUIEngine_update(void* p);
extern "C" void func_0029ED18(void* snd);
extern "C" void cMenuSys_input(void* p);
extern "C" int cGame_activatePauseMenu(char* self, int a);
extern "C" int cOVStateManager_allowPauseMenu(void);
extern "C" void cPadCache_clearAllVibrations(void* p);
extern "C" void func_0020CA10(void);
extern "C" void cSSXAudio_PauseAudio(void* snd);
extern "C" void cSSXAudio_PlayPauseSound(void* snd);
extern "C" int cOVStateManager_pauseOverlayActive(void);
extern "C" void cPlaybackMan_update(void* p);
int GetHashValue32(char* str);
extern void* PadCache;
extern void* cWScriptMan_TheWScriptMan;
// Pending script pause request; 0xFFFFFFFF = none (the store is lui/ori, so the global is unsigned).
extern unsigned int D_004A20CC_4B48 __asm__("D_004A20CC");
extern int D_004428F8[];
extern int D_004428FC[];
extern int D_00442920[];
extern char str_90bc_fail_b[];

// A script message (cWScriptQueueEntry).
struct sMsg_4B48 {
    int a;
    int type;
    int c;
};

// The script manager's message queue (wscript/wscriptman: cWScriptQueue).
struct sQueue_4B48 {
    int head;
    int tail;
    sMsg_4B48 entries[16];
};

extern "C" sMsg_4B48* tMissionMenuQueue_getEntry(sQueue_4B48* self);
extern "C" sMsg_4B48* tMissionMenuQueue_getNextEntry(sQueue_4B48* self);

// The script manager (cWScriptMan_TheWScriptMan); hasMessage / popMessage are inline here
// (out-of-line copies: cWScriptMan_isMenuQueueReady, cWScriptMan_popMenuQueueEntry).
struct sScript_4B48 {
    char pad0[0x1C4];
    sQueue_4B48 queue;              // 0x1C4
    bool hasMessage() { return queue.head != queue.tail; }
    sMsg_4B48* popMessage() { return tMissionMenuQueue_getNextEntry(&queue); }
};

struct sVSlot_4B48 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

struct sUIObj_4B48 {
    char pad0[0x8];
    sVSlot_4B48* vt;                // 0x8
};

struct sStateStack_4B48 {
    void* top;
};

extern "C" int cUIStateStack_findState(sStateStack_4B48* p, int hash);

struct sUIEngine_4B48 {
    char pad0[0xC];
    sUIObj_4B48* fC;                // 0xC
    char pad10[0x18 - 0x10];
    sStateStack_4B48 states;        // 0x18
};


static inline bool racing_4B48(int s)
{
    return s != 0 && s < 10;
}

static inline bool pending_4B48()
{
    return D_004A20CC_4B48 != 0xFFFFFFFF;
}

static inline bool playing_4B48(char* pb)
{
    return *(int*)(pb + 0x70) == 1;
}

/**
 * Runs the in-game UI for one frame (overlays, script messages, pause requests) and decides whether the race updates.
 *
 * Calls cOVStateManager_update. While MenuSys (+0xA4) is not active, the next message of the world script manager's
 * queue is handled: type 8 opens overlay 29 and pushes pause owner 3 (its flags from D_004428FC) with an icon sound;
 * type 9 runs cWScriptMan_doMissionCmd(..., 5, ...) and either opens overlay 0x1E with pause owner 3 (when the UI has
 * no "fail" state) or pops the message. A pending script pause request (D_004A20CC not 0xFFFFFFFF) opens overlay 34
 * with pause owner 0xC and clears the request. An active MenuSys is updated (cMenuSys_input). When
 * cGame_activatePauseMenu allows it, no replay mode 1..9 is set and no playback is running, cOVStateManager_allowPauseMenu opens the
 * pause menu: pause owner 2, the UI's virtual slot 44 with PauseOwner (+0x228), audio paused. Otherwise
 * cUIEngine_update(UI), cOVStateManager_pauseOverlayActive and cPlaybackMan_update run. Each pause push writes D_005366E8[++D_004A2A54] and
 * cPauseState_Flags.
 *
 * @return true when the race should update this frame (no menu, no new pause, not just unpaused), else false
 */
// Handles script messages and pause requests; returns true when the race should run this frame.
extern "C" bool cGame_updateUI(cGame* self)
{
    cOVStateManager_update();
    if (cMenuSys_isActive(((void*)self->MenuSys)) == 0) {
        sScript_4B48* sm = ((sScript_4B48*)self->WScriptMan);
        sQueue_4B48* msgQueue = &sm->queue;
        if (sm->hasMessage()) {
            sMsg_4B48* msg = tMissionMenuQueue_getEntry(msgQueue);
            if (msg->type == 8) {
                cOVStateManager_triggerOverlay(29);
                D_005366E8[++D_004A2A54] = 3;
                cPauseState_Flags = D_004428FC[0];
                cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 4, 0, 1.0f);
                return false;
            }
            if (msg->type == 9) {
                cWScriptMan_doMissionCmd(((sScript_4B48*)self->WScriptMan), msg->a, 5, msg->c);
                if (cUIStateStack_findState(&((sUIEngine_4B48*)self->UI)->states, GetHashValue32(str_90bc_fail_b)) == 0) {
                    cOVStateManager_triggerOverlay(0x1E);
                    D_005366E8[++D_004A2A54] = 3;
                    cPauseState_Flags = D_004428FC[0];
                    cUIEngine_update(((sUIEngine_4B48*)self->UI));
                } else {
                    ((sScript_4B48*)cWScriptMan_TheWScriptMan)->popMessage();
                }
                return false;
            }
        }
    }
    if (pending_4B48()) {
        cOVStateManager_triggerOverlay(34);
        D_005366E8[++D_004A2A54] = 0xC;
        cPauseState_Flags = D_00442920[0];
        D_004A20CC_4B48 = 0xFFFFFFFF;
        func_0029ED18(cAudioInterface_bxAudio());
        return false;
    }
    if (cMenuSys_isActive(((void*)self->MenuSys)) != 0) {
        cUIEngine_update(((sUIEngine_4B48*)self->UI));
        cMenuSys_input(((void*)self->MenuSys));
        return false;
    }
    if (cGame_activatePauseMenu((char*)self, 0) != 0 && !racing_4B48(*((int*)self->Replay)) && !playing_4B48(((char*)self->PlaybackMan)) && cOVStateManager_allowPauseMenu() != 0) {
        cPadCache_clearAllVibrations(PadCache);
        func_0020CA10();
        D_005366E8[++D_004A2A54] = 2;
        cPauseState_Flags = D_004428F8[0];
        sUIObj_4B48* uiObject = ((sUIEngine_4B48*)self->UI)->fC;
        uiObject->vt[44].fn((char*)uiObject + uiObject->vt[44].delta, *(unsigned char*)&self->PauseOwner);
        cSSXAudio_PauseAudio(cAudioInterface_bxAudio());
        cSSXAudio_PlayPauseSound(cAudioInterface_bxAudio());
        return false;
    }
    int wasPaused = cPauseState_Flags & 1;
    cUIEngine_update(((sUIEngine_4B48*)self->UI));
    if (cOVStateManager_pauseOverlayActive() != 0) {
        return false;
    }
    cPlaybackMan_update(((char*)self->PlaybackMan));
    if (cMenuSys_isActive(((void*)self->MenuSys)) != 0) {
        return false;
    }
    if (wasPaused && (cPauseState_Flags & 1) == 0) {
        return false;
    }
    return true;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateAI);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_244E60 { short delta; short index; void* fn; };
extern "C" int cInputSync_aiinputready(int);
extern "C" int cInputSync_aiupdate(int);
extern "C" int cReplay_isPauseAI(int);
extern "C" void cReplay_update(int);
extern "C" int cPlaybackMan_isPauseAI(int);
extern "C" void cBucketMan_updateBucket(void*, int);
extern "C" void cBucketMan_mergeBucket(void*, int);
extern "C" void cBucketMan_applyBucket(void*, int);
extern "C" void cWScriptMan_update(int);
extern "C" void func_002294C8(int);
extern "C" void func_003440C8(int);
extern "C" void cMultiParticleMan_update(int);
extern "C" void cWorldSound_TriggerWorldAudio(int, int, int);
extern "C" void cGameViewMan_updateAll(int);
extern "C" void cGame_updateState(void*);
extern char gBucketMan[8];
extern int cPauseState_Flags;
struct sList_244E60 { int pad0; int items[3]; unsigned int count; };

/**
 * Updates the game simulation for one frame: game state, replay, scripts, AI, update buckets and views.
 *
 * When InputSync (+0x7C) is null or cInputSync_aiinputready accepts: cGame_updateState (merging bucket 1 in state 3),
 * cReplay_update unless paused (cPauseState_Flags bit 0), and ScriptEngine's virtual slot 5 unless the replay pauses
 * the AI or flag 8 is set. The AI step is skipped while paused, while the replay or the playback manager pauses the AI
 * or when cInputSync_aiupdate fails; otherwise it updates gBucketMan buckets 1 (unless flag 2), 5 and 6, applies bucket
 * 1, calls the AI's virtual slot 3 (vptr at +0xCC), updates buckets 2 (unless flag 0x10) and 3 (unless flag 4),
 * WScriptMan, both crowd managers and MultiParticleMan, and merges bucket 1. Skipped, only buckets 5 and 3 run. Then
 * bucket 4, cGameViewMan_updateAll (except in replay mode 3), mUpdateTick (+0xAC) is counted and
 * cWorldSound_TriggerWorldAudio runs for each view camera (+0x20 of it). cGame_update has the same code inline.
 */
extern "C" void cGame_updateAI(cGame* self)
{
    char* obj;
    int skip;
    if ((int)self->InputSync == 0 || cInputSync_aiinputready((int)self->InputSync) != 0) {
        if (self->mCurrentStateID == 3) {
            obj = gBucketMan;
            cGame_updateState(self);
            cBucketMan_mergeBucket(obj, 1);
        } else {
            obj = gBucketMan;
            cGame_updateState(self);
        }
        if ((cPauseState_Flags & 1) == 0)
            cReplay_update((int)self->Replay);
        if (cReplay_isPauseAI((int)self->Replay) == 0 && (cPauseState_Flags & 8) == 0) {
            sVE_244E60* vt = (sVE_244E60*)ScriptEngine->mVtbl;
            ((void (*)(void*))vt[5].fn)((char*)ScriptEngine + vt[5].delta);
        }
    }
    obj = gBucketMan;
    skip = 0;
    if ((cPauseState_Flags & 1) != 0 || cReplay_isPauseAI((int)self->Replay) != 0)
        skip = 1;
    if (skip == 0) {
        if ((int)self->InputSync == 0 || cInputSync_aiupdate((int)self->InputSync) != 0) {
            if (cPlaybackMan_isPauseAI((int)self->PlaybackMan) != 0)
                goto setskip;
        } else {
            setskip:
            skip = 1;
        }
    }
    if (!skip) {
        if ((cPauseState_Flags & 2) == 0)
            cBucketMan_updateBucket(obj, 1);
        cBucketMan_updateBucket(obj, 5);
        cBucketMan_updateBucket(obj, 6);
        if ((cPauseState_Flags & 2) == 0)
            cBucketMan_applyBucket(obj, 1);
        {
            char* aiMan = (char*)self->AI;
            sVE_244E60* vt = *(sVE_244E60**)(aiMan + 0xCC);
            ((void (*)(void*))vt[3].fn)(aiMan + vt[3].delta);
        }
        if ((cPauseState_Flags & 0x10) == 0)
            cBucketMan_updateBucket(obj, 2);
        if ((cPauseState_Flags & 4) == 0)
            cBucketMan_updateBucket(obj, 3);
        cWScriptMan_update((int)self->WScriptMan);
        func_002294C8((int)self->CrowdMan2d);
        func_003440C8((int)self->CrowdMan3d);
        cMultiParticleMan_update((int)self->MultiParticleMan);
        cBucketMan_mergeBucket(obj, 1);
    } else {
        cBucketMan_updateBucket(obj, 5);
        if ((cPauseState_Flags & 4) == 0)
            cBucketMan_updateBucket(obj, 3);
    }
    cBucketMan_updateBucket(obj, 4);
    {
        int replayMode = *(int*)self->Replay;
        int ok = replayMode != 0 && replayMode < 10;
        int x3 = replayMode ^ 3;
        if (!ok || x3 != 0)
            cGameViewMan_updateAll((int)self->ViewMan);
    }
    self->mUpdateTick = self->mUpdateTick + 1;
    {
        sList_244E60* viewMan = (sList_244E60*)self->ViewMan;
        if (viewMan->count != 0) {
            unsigned int i = 0;
            do {
                cWorldSound_TriggerWorldAudio((int)self->WorldSound, ((sList_244E60*)(self)->ViewMan)->items[i] + 0x20, i);
                i++;
            } while (i < ((sList_244E60*)(char*)self->ViewMan)->count);
        }
    }
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_preUpdateOnline);
#ifdef SKIP_ASM
extern "C" void cGameComm_update(int);

/**
 * Updates online play before the frame: cGameComm_update on cGameComm_mGameComm when one exists.
 */
extern "C" void cGame_preUpdateOnline(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_update((int)cGameComm_mGameComm);
    }
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_postUpdateOnline);
#ifdef SKIP_ASM
extern "C" void NetConnIdle();

/**
 * Idles the network connection after the frame: NetConnIdle() when cCommSystem_mCommSystem exists.
 */
extern "C" void cGame_postUpdateOnline(void) {
    if (cCommSystem_mCommSystem != 0) {
        NetConnIdle();
    }
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateScreenFade);
#ifdef SKIP_ASM
extern "C" void cFadeScreen_update(void *);

/**
 * Advances the game's screen fade and reports whether the game may update.
 *
 * Calls cFadeScreen_update (cGame's base). When the application's word at +0x4 (its cAppMan base) is set, it sets the
 * fade's ending flag (+0x8) and returns 0. cGame_update has the same code inline.
 *
 * @return 1 when the game may update, 0 when it is ending
 */
extern "C" int cGame_updateScreenFade(cGame* self) {
    cFadeScreen_update(self);
    if ((int)SSXApp->LoadingModule != 0) {
        self->unk_08 = 1;
        return 0;
    }
    return 1;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", func_002451B0);
#ifdef SKIP_ASM
/**
 * Calls virtual slot 19 (vtable +0x98, vptr at +0x6C0) of each rider in AI's Riders, up to NumberOfRidersAllocated.
 *
 * cGame_update ends its update with the same loop.
 */
extern "C" void func_002451B0(cGame* self) {
    int i;
    char *obj;
    char *vtbl;

    i = 0;
    if (*(int *)((char*)self->AI + 0x78) > 0) {
        do {
            obj = *(char **)((char*)self->AI + (i << 2) + 0x28);
            i++;
            vtbl = *(char **)(obj + 0x6C0);
            (*(void (**)(void *))(vtbl + 0x9C))(obj + *(short *)(vtbl + 0x98));
        } while (i < *(int *)((char*)self->AI + 0x78));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245228__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the progressive-scan popup state (vtable D_0047CA18) through the UI-state base destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register; unlike the other states here it does not reset its
 * own vtable first.
 *
 * @return cUIState__cUIState's result, passed through
 */
void* func_00245228(void* self)
{
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245248);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern char str_supopup[];
extern char str_Title_b[];
extern char str_Do_you_want_to_display_the_game_in_Progr[];

/**
 * Creates the progressive-scan question popup.
 *
 * Adds the "supopup" screen to the UI engine (+0x10) with this state as owner, keeps it at +0x40 and plays its first
 * frame, then sets its "Title" text to "Do you want to display the game in Progr..." (the progressive-scan question).
 * Slot +0x24 (onCreateScreen) of D_0047CA18.
 */
extern "C" void func_00245248(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_supopup), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_b));
    if (text != 0) {
        cUIText_setAsciiString(text, str_Do_you_want_to_display_the_game_in_Progr);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002452D8);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc bound as a placement operator new (gcc then treats the result as unaliased)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState(void* self, void* owner);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char D_0047B9C0[];
extern char str_yes_c[];
extern void* D_0047DA58[];

struct sUIObj_2452D8 {
    int f0;
    int f4;
    void** vt;
    char pad[0x3C];
    char flag48;
    char pad49[7];
};

struct sRVEntry_2452D8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sRCtx_2452D8 {
    char pad[0x10D8];
    sRVEntry_2452D8* vt;
};

static inline int isHash_2452D8(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline sUIObj_2452D8* newUIObj_2452D8(void* owner, char flag)
{
    sUIObj_2452D8* o = new (D_0047B9C0, 0, 0) sUIObj_2452D8;
    cUIState_cUIState(o, *(void**)((char*)owner + 0x10));
    o->vt = D_0047DA58;
    o->flag48 = flag;
    return o;
}

/**
 * Handles the progressive-scan popup's answer: pushes a new UI state that records it.
 *
 * Only widget events of type 5 with a message count. A new 0x50-byte UI state (cUIState_cUIState on the UI engine, vtable
 * D_0047DA58) is pushed on the UI state stack (engine + 0x18) with its flag at +0x48 cleared for "yes" (the message
 * hash at +0x38) or set otherwise; "yes" also calls cGraphicsMan_TheGraphicsMan's vtable slot 8 with 1. Slot +0x9C
 * (onWidgetEvent) of D_0047CA18.
 *
 * @param msg  the widget event message (ignored when null)
 * @param type  the event type (only 5 is handled)
 */
extern "C" void func_002452D8(void* self, char* msg, int type)
{
    if (msg == 0) {
        return;
    }
    if (type != 5) {
        return;
    }
    if (isHash_2452D8(*(int*)(msg + 0x38), str_yes_c)) {
        sUIObj_2452D8* newState = newUIObj_2452D8(self, 0);
        cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, newState);
        ((sRVEntry_2452D8*)cGraphicsMan_TheGraphicsMan->mVtbl)[8].fn((char*)cGraphicsMan_TheGraphicsMan + ((sRVEntry_2452D8*)cGraphicsMan_TheGraphicsMan->mVtbl)[8].delta, 1);
    } else {
        sUIObj_2452D8* newState = newUIObj_2452D8(self, 1);
        cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, newState);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002453E0);
#ifdef SKIP_ASM
/**
 * Answers the progressive-scan popup's menu query: 257 for event 6, else 0. Slot +0xA4 (onWidgetMenu) of D_0047CA18.
 *
 * @param menu  the menu widget (unused)
 * @param event  the menu event
 * @return 257 when @p event is 6, else 0
 */
extern "C" int func_002453E0(void* self, int menu, int event)
{
    return event == 6 ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState__cGameLoadState);
#ifdef SKIP_ASM
extern void* D_0047C948[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the match / event loading state: installs its vtable D_0047C948 at +0x8 and runs the UI-state base
 * destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register.
 *
 * @return cUIState__cUIState's result, passed through
 */
extern "C" void* cGameLoadState__cGameLoadState(cGameLoadState* self)
{
    self->mVtbl = (void**)D_0047C948;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_00246CF8(void* self, int a1);
extern char str_102MPMatch[];
extern char str_99QPEvent[];

/**
 * Creates the match / event loading screen: "102MPMatch" when newRace.cGamePath is 2, else "99QPEvent".
 *
 * Fetches the BE interface 0 first (result unused), adds the screen to the UI engine (+0x10) with this state as owner,
 * keeps it at +0x40 and plays its first frame, then fills both players' panels with func_00246CF8(self, 0) and (self,
 * 1). Slot +0x24 (onCreateScreen) of D_0047C948.
 */
extern "C" void cGameLoadState_onCreateScreen(cGameLoadState* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 2) {
        void* engine = self->mpEngine;
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_102MPMatch), 0);
        self->mpScreen = (cUIScreen*)screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    } else {
        void* engine = self->mpEngine;
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_99QPEvent), 0);
        self->mpScreen = (cUIScreen*)screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    }
    func_00246CF8(self, 0);
    func_00246CF8(self, 1);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002454F8);
#ifdef SKIP_ASM
extern char str_pct[];
extern char str_pct_3dpct_pct[];
extern char* cAppMan_TheAppMan;
extern "C" void sprintf2(char* dst, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);

/**
 * Shows the loading percentage on the match / event loading screen and optionally refreshes the player panels.
 *
 * Formats cAppMan_TheAppMan's float at +0x38 as an int with str_pct_3dpct_pct into the text widget hashed from
 * str_pct on the screen at +0x40; with @p refresh set it also runs func_00246CF8 for players 0 and 1. Slot +0x3C
 * (onGainTransition) of D_0047C948.
 *
 * @param refresh  non-zero to refresh both player panels
 * @return 0
 */
extern "C" int func_002454F8(cGameLoadState* self, int refresh)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)(cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    if (refresh != 0) {
        func_00246CF8(self, 0);
        func_00246CF8(self, 1);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGameLoadState_widgetCreateQP(void* self, int a1);
extern "C" void cGameLoadState_widgetCreateMultiP(void* self, int a1);

/**
 * Creates a widget of the match / event loading screen: cGameLoadState_widgetCreateMultiP when newRace.cGamePath is 2, else cGameLoadState_widgetCreateQP.
 *
 * Fetches the BE interface 0 first (result unused). Slot +0x94 (onWidgetCreate) of D_0047C948.
 *
 * @param widget  the widget being created (passed on)
 */
extern "C" void cGameLoadState_onWidgetCreate(cGameLoadState* self, int widget)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    bool ok = cBENewRaceInterface_newRace.cGamePath != 2;
    if (ok) {
        cGameLoadState_widgetCreateQP(self, widget);
    } else {
        cGameLoadState_widgetCreateMultiP(self, widget);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadStateOutLodge__cGameLoadStateOutLodge);
#ifdef SKIP_ASM
extern void* D_0047C878[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the leaving-the-lodge loading state: installs its vtable D_0047C878 at +0x8 and runs the UI-state base
 * destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register.
 *
 * @return cUIState__cUIState's result, passed through
 */
extern "C" void* cGameLoadStateOutLodge__cGameLoadStateOutLodge(cGameLoadStateOutLodge* self)
{
    self->mVtbl = (void**)D_0047C878;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadStateOutLodge_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_118loadoutlodge[];

/**
 * Creates the leaving-the-lodge loading screen ("118loadoutlodge"): adds it to the UI engine (+0x10) with this state as
 * owner, keeps it at +0x40 and plays its first frame. Slot +0x24 (onCreateScreen) of D_0047C878.
 */
extern "C" void cGameLoadStateOutLodge_onCreateScreen(cGameLoadStateOutLodge* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_118loadoutlodge), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadStateOutLodge_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the leaving-the-lodge loading state creates no widgets of its own. Slot +0x94 (onWidgetCreate) of
 * D_0047C878.
 */
void cGameLoadStateOutLodge_onWidgetCreate(void* vself)
{
    cGameLoadStateOutLodge* self = (cGameLoadStateOutLodge*)vself;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245698);
#ifdef SKIP_ASM
extern char str_pct[];
extern char str_pct_3dpct_pct[];
extern char* cAppMan_TheAppMan;
extern "C" void sprintf2(char* dst, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);

/**
 * Shows the loading percentage on the leaving-the-lodge loading screen.
 *
 * Formats cAppMan_TheAppMan's float at +0x38 as an int with str_pct_3dpct_pct into the text widget hashed from
 * str_pct on the screen at +0x40. Slot +0x3C (onGainTransition) of D_0047C878.
 *
 * @return 0
 */
extern "C" int func_00245698(cGameLoadStateOutLodge* self)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)(cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245708);
#ifdef SKIP_ASM
extern void* D_0047C7A8[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the controller-layout loading state: installs its vtable D_0047C7A8 at +0x8 and runs the UI-state base
 * destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register.
 *
 * @return cUIState__cUIState's result, passed through
 */
extern "C" void* func_00245708(void* self)
{
    *(void**)((char*)self + 0x8) = D_0047C7A8;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245730);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int a1);
extern char str_110ctrl_load[];
extern char str_CtrlPS2_default[];
extern char str_CtrlPS2_pro[];
extern char str_default_text_b[];
extern char str_pro_text_b[];

class cUIObj_245730 {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

/**
 * Creates the controller-layout loading screen ("110ctrl_load") and shows the layout player 1 uses.
 *
 * Adds the screen to the UI engine (+0x10) with this state as owner, keeps it at +0x40 and plays its first frame. Reads
 * the controller mapping of the BE player interface (cBENewPlayerInterface_getControllerMapping(interface 1, 0)) and
 * makes "CtrlPS2_default" and "default_text" visible for mapping 0, "CtrlPS2_pro" and "pro_text" otherwise (widget
 * virtual slot 9). Slot +0x24 (onCreateScreen) of D_0047C7A8.
 */
extern "C" void func_00245730(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* scr = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_110ctrl_load), 0);
    *(void**)((char*)self + 0x40) = scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
    }
    char* ctrlDefault = str_CtrlPS2_default;
    char* ctrlPro = str_CtrlPS2_pro;
    int controlConfig = cBENewPlayerInterface_getControllerMapping(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    cUIObj_245730* widget;
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(ctrlDefault));
    if (widget != 0) {
        widget->setVisible(controlConfig == 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_default_text_b));
    if (widget != 0) {
        widget->setVisible(controlConfig == 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(ctrlPro));
    if (widget != 0) {
        widget->setVisible(controlConfig != 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_pro_text_b));
    if (widget != 0) {
        widget->setVisible(controlConfig != 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002458B0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the controller-layout loading state creates no widgets of its own. Slot +0x94 (onWidgetCreate) of
 * D_0047C7A8.
 */
void func_002458B0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002458B8);
#ifdef SKIP_ASM
extern char str_pct[];
extern char str_pct_3dpct_pct[];
extern char* cAppMan_TheAppMan;
extern "C" void sprintf2(char* dst, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);

/**
 * Shows the loading percentage on the controller-layout loading screen.
 *
 * Formats cAppMan_TheAppMan's float at +0x38 as an int with str_pct_3dpct_pct into the text widget hashed from
 * str_pct on the screen at +0x40. Slot +0x3C (onGainTransition) of D_0047C7A8.
 *
 * @return 0
 */
extern "C" int func_002458B8(void* self)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)(cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFELoadHintState__cFELoadHintState);
#ifdef SKIP_ASM
extern void* cFELoadHintState_vtbl[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the front-end load-hint state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (cUIState__cUIState) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cFELoadHintState__cFELoadHintState(cFELoadHintState* self)
{
    self->mVtbl = (void**)cFELoadHintState_vtbl;
    return cUIState__cUIState(self);
}
#endif

