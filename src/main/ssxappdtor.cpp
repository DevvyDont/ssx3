#include "vtable.h"
#include "common.h"
#include "ui/uithing.h"
#include "be/beinterface.h"
#include "visualfx/aivisualeffectsman.h"
#include "visualfx/visualeffectsman.h"
#include "ui/uiengine.h"
#include "wscript/missionmenuqueue.h"
#include "wscript/wscriptman.h"
#include "ui/uimenu.h"
#include "ai/rider.h"
#include "ai/ai.h"
#include "world/worldcache.h"
#include "ui/uistate.h"
#include "sound/ssxaudio.h"
#include "main/gameviewman.h"
#include "bx/appman.h"
#include "main/fadescreen.h"
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
 * @return BE (+0x78)
 */
cBE* cSSXApp::getBE()
{
    return BE;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getRealFE);
#ifdef SKIP_ASM
/**
 * Returns the application's real front end (RealFE).
 *
 * @return RealFE (+0x7C)
 */
cFE* cSSXApp::getRealFE()
{
    return RealFE;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getDebugFE);
#ifdef SKIP_ASM
/**
 * Returns the application's debug front end (DebugFE).
 *
 * @return DebugFE (+0x80)
 */
cDebugFE* cSSXApp::getDebugFE()
{
    return DebugFE;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getGame);
#ifdef SKIP_ASM
/**
 * Returns the application's game object (Game).
 *
 * @return Game (+0x84); 0 outside a game
 */
cGame* cSSXApp::getGame()
{
    return Game;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getFonts);
#ifdef SKIP_ASM
/**
 * Returns the application's UI font interface (Fonts).
 *
 * @return Fonts (+0x88)
 */
cUIFontInterface* cSSXApp::getFonts()
{
    return Fonts;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getFELocale);
#ifdef SKIP_ASM
/**
 * Returns the application's front-end locale (FELocale).
 *
 * @return FELocale (+0x8C)
 */
cFELocale* cSSXApp::getFELocale()
{
    return FELocale;
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
cInput* cSSXApp::getInput(int index)
{
    return Inputs[index];
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
cInputMap* cSSXApp::getInputMap(int index)
{
    return InputMaps[index];
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getGameManager);
#ifdef SKIP_ASM
/**
 * Returns the application's game mode manager (GameModeMan).
 *
 * @return GameModeMan (+0xC0)
 */
cGameModeMan* cSSXApp::getGameManager()
{
    return GameModeMan;
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
 * @return NetMan (+0xD0)
 */
cNetMan* cSSXApp::getNetMan()
{
    return NetMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getConsoleConfig);
#ifdef SKIP_ASM
/**
 * Returns the application's console configuration (ConsoleConfig).
 *
 * @return ConsoleConfig (+0x9C)
 */
cConsoleConfig* cSSXApp::getConsoleConfig()
{
    return ConsoleConfig;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_getLoadIndicator);
#ifdef SKIP_ASM
/**
 * Returns the application's load indicator (LoadIndicator).
 *
 * @return LoadIndicator (+0xA4)
 */
cLoadIndicator* cSSXApp::getLoadIndicator()
{
    return LoadIndicator;
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
 * @return LoadIconTextureID (+0xA0)
 */
int cSSXApp::getLoadIconTextureID()
{
    return (LoadIconTextureID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_setLoadIconTextureID);
#ifdef SKIP_ASM
/**
 * Sets the texture id of the loading icon (LoadIconTextureID).
 *
 * @param textureId  the new texture id
 */
void cSSXApp::setLoadIconTextureID(int textureId)
{
    LoadIconTextureID = textureId;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_endFE);
#ifdef SKIP_ASM
/**
 * Clears both front-end pointers, RealFE and DebugFE, marking the front end as ended.
 */
void cSSXApp::endFE()
{
    RealFE = 0;
    DebugFE = 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_endGame);
#ifdef SKIP_ASM
/**
 * Clears the Game pointer, marking the game as ended.
 *
 */
void cSSXApp::endGame()
{
    Game = 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_InGame);
#ifdef SKIP_ASM
/**
 * Returns whether a game is running (Game is set).
 *
 * @return true when Game (+0x84) is non-null
 */
bool cSSXApp::InGame()
{
    return Game != 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cSSXApp_InFE);
#ifdef SKIP_ASM
/**
 * Returns whether the front end is running (RealFE or DebugFE is set).
 *
 * @return 1 when RealFE (+0x7C) or DebugFE (+0x80) is non-null, else 0
 */
int cSSXApp::InFE()
{
    int var_3;

    var_3 = 0;
    if (RealFE != 0 || DebugFE != 0) {
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
 * @return mFEAsyncManager (+0x11C)
 */
cFEAsyncManager* cSSXApp::GetFEAsyncManager()
{
    return mFEAsyncManager;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFadeScreen__cFadeScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cAppModule_vtbl[];

/**
 * Destroys a screen fade: resets the vtable to the root module vtable cAppModule_vtbl and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cFadeScreen_vtbl, the vtable cFadeScreen_cFadeScreen (the fade constructor) installs
 * and whose loseFocus and update slots are cFadeScreen_loseFocus and cFadeScreen_update.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cFadeScreen__cFadeScreen(cFadeScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFadeScreen_allowFade);
#ifdef SKIP_ASM
/**
 * Sets the screen fade's ending flag (+0x8) to 1.
 *
 * cFadeScreen_cFadeScreen (the fade constructor) clears this flag; cFadeScreen_loseFocus starts the fade-out.
 *
 */
void cFadeScreen::allowFade()
{
    IsEnding = 1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cPreStartScreen__cPreStartScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cAppModule_vtbl[];

/**
 * Destroys the pre-start screen.
 *
 * Only resets the vtable to the root module vtable cAppModule_vtbl (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cPreStartScreen__cPreStartScreen(cPreStartScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cPreStartScreen_load__FPv);
#ifdef SKIP_ASM
/**
 * Loads nothing: the pre-start screen has no data of its own (empty, as in On Tour).
 */
void cPreStartScreen::load()
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
 */
void cInitLoadScreen::activateProgressiveMode()
{
    progressive = 1;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cLoadScreen__cLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cAppModule_vtbl[];

/**
 * Destroys a plain load screen: resets the vtable to the root module vtable cAppModule_vtbl and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cLoadScreen_vtbl, the vtable cLoadScreen_cLoadScreen installs (loseFocus is
 * cLoadScreen_loseFocus).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cLoadScreen__cLoadScreen(cLoadScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern char cAppModule_vtbl[];

/**
 * Destroys the start screen: resets the vtable to the root module vtable cAppModule_vtbl and frees the object when asked.
 *
 * It is the destructor slot (+0xC) of cStartScreen_vtbl, whose load slot is cStartScreen_load (loads the "data/ui/su"
 * start screen).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cStartScreen__cStartScreen(cStartScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadScreen__cGameLoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cAppModule_vtbl[];

/**
 * Destroys the game load screen: resets the vtable to the root module vtable cAppModule_vtbl and frees the object when
 * asked.
 *
 * It is the destructor slot (+0xC) of cGameLoadScreen_vtbl, whose load slot is cGameLoadScreen_load (loads the
 * "data/ui/gl" game load screen); cGameLoadScreen_cGameLoadScreen installs that vtable.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGameLoadScreen__cGameLoadScreen(cGameLoadScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
INCLUDE_ASM("main/ssxappdtor", cGameLoadScreen_getTexttureID);
#ifdef SKIP_ASM
/**
 * Returns one of the game load screen's two texture ids (+0x10).
 *
 * @param index  which texture (0 or 1; not range-checked)
 * @return the texture id at +0x10 + 4 * @p index
 */
int cGameLoadScreen::getTexttureID(int index)
{
    return *(int *)((char *)this + (index << 2) + 0x10);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cFELoadScreen__cFELoadScreen);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cAppModule_vtbl[];

/**
 * Destroys a front-end load screen.
 *
 * Only resets the vtable to the root module vtable cAppModule_vtbl (the base destructors have nothing else to do) and
 * frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cFELoadScreen__cFELoadScreen(cFELoadScreen* self, int flags)
{
    *(void**)self = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
int cFELoadScreen::getTexttureID(int index)
{
    return mTexID[index];
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
extern "C" void cGameState__cGameState(cGameState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadInit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadInit has an empty body.
 */
void cGameState::preloadInit()
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadUpdate has an empty body.
 */
void cGameState::preloadUpdate()
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_preloadCancel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's preloadCancel has an empty body.
 */
void cGameState::preloadCancel()
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
void cGameState::gainFocus()
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
void cGameState::update()
{
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameState_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base game state's loseFocus has an empty body.
 */
void cGameState::loseFocus()
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
int cGameState::isPreloadComplete()
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
void cGameState::onWidgetEvent()
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
extern "C" void cGFEndState__cGFEndState(cGFEndState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFEndWaitState__cGFEndWaitState(cGFEndWaitState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFFinishlineState__cGFFinishlineState(cGFFinishlineState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFTransportWaitState__cGFTransportWaitState(cGFTransportWaitState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFEventStartState__cGFEventStartState(cGFEventStartState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFGateState__cGFGateState(cGFGateState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFRidingState__cGFRidingState(cGFRidingState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFTrackStartState__cGFTrackStartState(cGFTrackStartState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFNextHeatState__cGFNextHeatState(cGFNextHeatState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFSessionTeleportState__cGFSessionTeleportState(cGFSessionTeleportState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFUserSelectState__cGFUserSelectState(cGFUserSelectState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGFUserSelectState_setNextLocation);
#ifdef SKIP_ASM
/**
 * Sets the user-select state's next location (+0x18).
 *
 * @param location  the location to go to next
 */
void cGFUserSelectState::setNextLocation(int location)
{
    mNextLocation = location;
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
extern "C" void cGFSectionLoadState__cGFSectionLoadState(cGFSectionLoadState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFPreEventState__cGFPreEventState(cGFPreEventState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFPostEventState__cGFPostEventState(cGFPostEventState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFPodiumState__cGFPodiumState(cGFPodiumState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cGFReplayStartState__cGFReplayStartState(cGFReplayStartState* self, int flags)
{
    self->mVtbl = (void**)cGameState_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
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
INCLUDE_ASM("main/ssxappdtor", cPauseState_set);
#ifdef SKIP_ASM
extern int cPauseState_FlagsTable[];
extern int cPauseState_Stack[];
extern int cPauseState_Flags;
extern int cPauseState_StackIdx;

/**
 * Makes @p id the current pause owner: loads its pause flags and records it on top of the pause stack.
 *
 * Sets cPauseState_Flags to cPauseState_FlagsTable[@p id] (one flag word per owner) and stores @p id at cPauseState_Stack[cPauseState_StackIdx] (the
 * stack entry at the current depth, which it does not change). cGame_cGame calls it three times.
 *
 * @param id  the pause owner (index into the flag table)
 */
extern "C" void cPauseState_set(int id)
{
    cPauseState_Flags = cPauseState_FlagsTable[id];
    cPauseState_Stack[cPauseState_StackIdx] = id;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentState);
#ifdef SKIP_ASM
/**
 * Returns the current game-flow state number (mCurrentStateID).
 *
 * @return mCurrentStateID (+0x214)
 */
int cGame::getCurrentState()
{
    return (mCurrentStateID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getPreviousState);
#ifdef SKIP_ASM
/**
 * Returns the previous game-flow state number (mPrevStateID).
 *
 * @return mPrevStateID (+0x218)
 */
int cGame::getPreviousState()
{
    return (mPrevStateID);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentStateInstance);
#ifdef SKIP_ASM
/**
 * Returns the current game-flow state object (mCurrentState).
 *
 * @return mCurrentState (+0x200)
 */
cGameState* cGame::getCurrentStateInstance()
{
    return mCurrentState;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getNash);
#ifdef SKIP_ASM
/**
 * Returns the game's Nash ray-query object (BXNash).
 *
 * @return BXNash (+0x20)
 */
cBXNash* cGame::getNash()
{
    return BXNash;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorld);
#ifdef SKIP_ASM
/**
 * Returns the game's world (World).
 *
 * @return World (+0x10)
 */
cWorld* cGame::getWorld()
{
    return World;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getDebugFont);
#ifdef SKIP_ASM
/**
 * Returns the game's debug font (DebugFont).
 *
 * @return DebugFont (+0x24)
 */
cFont* cGame::getDebugFont()
{
    return DebugFont;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getMenuSys);
#ifdef SKIP_ASM
/**
 * Returns the game's menu system (MenuSys).
 *
 * @return MenuSys (+0xA4)
 */
cMenuSys* cGame::getMenuSys()
{
    return MenuSys;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getAI);
#ifdef SKIP_ASM
/**
 * Returns the game's AI (AI).
 *
 * @return AI (+0xC)
 */
cAI* cGame::getAI()
{
    return AI;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getUI);
#ifdef SKIP_ASM
/**
 * Returns the game's UI engine (UI).
 *
 * @return UI (+0x48)
 */
cUIEngine* cGame::getUI()
{
    return UI;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getReplay);
#ifdef SKIP_ASM
/**
 * Returns the game's replay (Replay).
 *
 * @return Replay (+0x28)
 */
cReplay* cGame::getReplay()
{
    return Replay;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getFXLoader);
#ifdef SKIP_ASM
/**
 * Returns the game's effects loader (FXLoader).
 *
 * @return FXLoader (+0x2C)
 */
cFXLoader* cGame::getFXLoader()
{
    return FXLoader;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSurfPropCache);
#ifdef SKIP_ASM
/**
 * Returns the game's surface property cache (SurfPropCache).
 *
 * @return SurfPropCache (+0x44)
 */
cSurfPropCache* cGame::getSurfPropCache()
{
    return SurfPropCache;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorldSound);
#ifdef SKIP_ASM
/**
 * Returns the game's world sound (WorldSound).
 *
 * @return WorldSound (+0x4C)
 */
cWorldSound* cGame::getWorldSound()
{
    return WorldSound;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getAIVisualEffectsObjNode);
#ifdef SKIP_ASM
/**
 * Returns the game's AI visual-effects object node (AIVisualEffectsMan).
 *
 * @return AIVisualEffectsMan (+0x50)
 */
cAIVisualEffectsObjNode* cGame::getAIVisualEffectsObjNode()
{
    return AIVisualEffectsMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getVisualEffectsObjNode);
#ifdef SKIP_ASM
/**
 * Returns the game's visual-effects object node (VisualEffectsMan).
 *
 * @return VisualEffectsMan (+0x54)
 */
cVisualEffectsObjNode* cGame::getVisualEffectsObjNode()
{
    return VisualEffectsMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCutSceneTransitionman);
#ifdef SKIP_ASM
/**
 * Returns the game's cut-scene transition manager (CutSceneTransitionMan).
 *
 * @return CutSceneTransitionMan (+0x64)
 */
cCutSceneTransitionMan* cGame::getCutSceneTransitionman()
{
    return CutSceneTransitionMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getFlagManager);
#ifdef SKIP_ASM
/**
 * Returns the game's flag manager (FlagManager).
 *
 * @return FlagManager (+0x70)
 */
cFlagManager* cGame::getFlagManager()
{
    return FlagManager;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSnowFall);
#ifdef SKIP_ASM
/**
 * Returns the game's snowfall effect (SnowFall).
 *
 * @return SnowFall (+0x74)
 */
cSnowFall* cGame::getSnowFall()
{
    return SnowFall;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getPlaybackMan);
#ifdef SKIP_ASM
/**
 * Returns the game's replay playback manager (PlaybackMan).
 *
 * @return PlaybackMan (+0x34)
 */
cPlaybackMan* cGame::getPlaybackMan()
{
    return PlaybackMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSectionMan);
#ifdef SKIP_ASM
/**
 * Returns the game's section manager (SectionMan).
 *
 * @return SectionMan (+0x78)
 */
cSectionMan* cGame::getSectionMan()
{
    return SectionMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getInputSync);
#ifdef SKIP_ASM
/**
 * Returns the game's input synchroniser (InputSync).
 *
 * @return InputSync (+0x7C)
 */
cInputSync* cGame::getInputSync()
{
    return InputSync;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getSkyBox);
#ifdef SKIP_ASM
/**
 * Returns the game's sky box (SkyBox).
 *
 * @return SkyBox (+0x14)
 */
cSkyBox* cGame::getSkyBox()
{
    return SkyBox;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCrowdMan2d);
#ifdef SKIP_ASM
/**
 * Returns the game's 2D crowd manager (CrowdMan2d).
 *
 * @return CrowdMan2d (+0x38)
 */
cCrowdMan2d* cGame::getCrowdMan2d()
{
    return CrowdMan2d;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCrowdMan3d);
#ifdef SKIP_ASM
/**
 * Returns the game's 3D crowd manager (CrowdMan3d).
 *
 * @return CrowdMan3d (+0x3C)
 */
cCrowdMan3d* cGame::getCrowdMan3d()
{
    return CrowdMan3d;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getMultiParticleMan);
#ifdef SKIP_ASM
/**
 * Returns the game's multi-particle manager (MultiParticleMan).
 *
 * @return MultiParticleMan (+0x40)
 */
cMultiParticleMan* cGame::getMultiParticleMan()
{
    return MultiParticleMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getInstanceMan);
#ifdef SKIP_ASM
/**
 * Returns the game's instance manager (InstanceMan).
 *
 * @return InstanceMan (+0x88)
 */
cInstanceMan* cGame::getInstanceMan()
{
    return InstanceMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getOcclusionMan);
#ifdef SKIP_ASM
/**
 * Returns the game's occlusion manager (OcclusionMan).
 *
 * @return OcclusionMan (+0x8C)
 */
cOcclusionMan* cGame::getOcclusionMan()
{
    return OcclusionMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getWorldLightMan);
#ifdef SKIP_ASM
/**
 * Returns the game's world light manager (WorldLightMan).
 *
 * @return WorldLightMan (+0x90)
 */
cWorldLightMan* cGame::getWorldLightMan()
{
    return WorldLightMan;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getLoadIndicator);
#ifdef SKIP_ASM
/**
 * Returns the game's load indicator (LoadIndicator).
 *
 * @return LoadIndicator (+0x94)
 */
cBXLoadIndicator* cGame::getLoadIndicator()
{
    return LoadIndicator;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getControllerUnplugged);
#ifdef SKIP_ASM
/**
 * Returns the game's controller-unplugged flag (ControllerUnplugged).
 *
 * @return ControllerUnplugged (+0x224)
 */
int cGame::getControllerUnplugged()
{
    return (ControllerUnplugged);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getViewMan);
#ifdef SKIP_ASM
/**
 * Returns the game's game view manager (ViewMan).
 *
 * @return ViewMan (+0x84)
 */
cGameViewMan* cGame::getViewMan()
{
    return ViewMan;
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
int cGame::getGameView(int index)
{
    return *(int *)((char *)((int)ViewMan) + (index << 2) + 4);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getNumGameView);
#ifdef SKIP_ASM
/**
 * Returns the number of camera views in the game's view manager (ViewMan +0x10).
 *
 * @return the view count
 */
int cGame::getNumGameView()
{
    return ((int)ViewMan->m_uiNumView);
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
int cGame::getCurrentRenderView()
{
    cGameViewMan* list = ViewMan;
    return (int)list->PlayerCamera[(int)list->m_uiCurrentView];
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_getCurrentRenderViewNum);
#ifdef SKIP_ASM
/**
 * Returns the number of the view being rendered (ViewMan +0x14).
 *
 * @return the current render view number
 */
int cGame::getCurrentRenderViewNum()
{
    return ((int)ViewMan->m_uiCurrentView);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGenericStreamPause__cGenericStreamPause);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cStreamPause_vtbl[];

/**
 * Destroys the game's stream-pause object: resets the vtable to cStreamPause_vtbl and frees the object when asked.
 *
 * It is the destructor slot of cGenericStreamPause_vtbl, the vtable cStreamPause_construct installs ("GenericStreamPause");
 * cStreamPause_vtbl is the base vtable (destructor cStreamPause__cStreamPause, two pure-virtual slots __pure_virtual).
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGenericStreamPause__cGenericStreamPause(int* self, int flags)
{
    *(void**)self = cStreamPause_vtbl;
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
 * Slot +0x14 of the generic stream-pause vtable cGenericStreamPause_vtbl.
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
 * Slot +0x1C of the generic stream-pause vtable cGenericStreamPause_vtbl.
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
void cInput::setPad(int val)
{
    Pad = (cInputPad*)val;
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
void cInput::setMap(int val)
{
    Map = (cInputMap*)val;
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
    VPTR_OF(cObjNode, self) = cVisualEffectsObjNode_vtbl;
    cVisualEffectsMan* obj = self->VisualEffectsMan;
    if (((void*)obj) != 0) {
        sVEntry_cVisualEffectsObjNode__cVisualEffectsObjNode* vt = (sVEntry_cVisualEffectsObjNode__cVisualEffectsObjNode*)obj->mVtbl;
        vt[1].fn(vthis(obj, vt[1]), 3);
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
void cVisualEffectsObjNode::update()
{
    ((cAppDtorVirt44*)VisualEffectsMan)->update();
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
void cVisualEffectsObjNode::render()
{
    ((cAppDtorVirt44*)VisualEffectsMan)->render();
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
 * cGMBigAirHandler_riderCheckpointHit) take the rider and the seconds added.
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
 * example cGMBigAirHandler_raceFinish, cGMRaceHandler_raceFinish).
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
 * Slot +0x4C of the base handler vtable cGameModeHandler_vtbl; cGMBigAirHandler_getHeatScore overrides it for the scored event.
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
 * Slot +0x54 of the base handler vtable cGameModeHandler_vtbl; cGMBigAirHandler_writeToReplayFrame overrides it for the scored event.
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
 * Slot +0x5C of the base handler vtable cGameModeHandler_vtbl; cGMBigAirHandler_readFromReplayFrame overrides it for the scored event.
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
 * Destroys the scored three-heat event handler (handler 0, vtable cGMBigAirHandler_vtbl): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMBigAirHandler__cGMBigAirHandler(cGMBigAirHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMRaceHandler__cGMRaceHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the timed three-heat event handler (handler 1, vtable cGMRaceHandler_vtbl): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMRaceHandler__cGMRaceHandler(cGMRaceHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPRaceHandler__cGMMPRaceHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the online race handler (handler 8, vtable cGMMPRaceHandler_vtbl): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMMPRaceHandler__cGMMPRaceHandler(cGMMPRaceHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPSlopeHandler__cGMMPSlopeHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the two-player online event handler (handler 9, vtable cGMMPSlopeHandler_vtbl): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMMPSlopeHandler__cGMMPSlopeHandler(cGMMPSlopeHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMMPSlopeHandler_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: handler 9 (the two-player online event, vtable cGMMPSlopeHandler_vtbl) saves nothing into a replay frame.
 *
 * Slot +0x54 (writeToReplayFrame) of cGMMPSlopeHandler_vtbl.
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
 * Does nothing: handler 9 (the two-player online event, vtable cGMMPSlopeHandler_vtbl) restores nothing from a replay frame.
 *
 * Slot +0x5C (readFromReplayFrame) of cGMMPSlopeHandler_vtbl.
 */
void cGMMPSlopeHandler_readFromReplayFrame(void* vself)
{
    cGMMPSlopeHandler* self = (cGMMPSlopeHandler*)vself;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMFreerideHandler__cGMFreerideHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the freeride handler (handler 2, vtable cGMFreerideHandler_vtbl): resets the vtable to the base handler vtable cGameModeHandler_vtbl
 * and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMFreerideHandler__cGMFreerideHandler(int* self, int flags)
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
 * Destroys the located timed-event handler (handler 4, vtable cGMTimeHandler_vtbl): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMTimeHandler__cGMTimeHandler(cGMTimeHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMRivalTimeHandler__cGMRivalTimeHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the head-to-head event handler (handler 5, vtable cGMRivalTimeHandler_vtbl): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMRivalTimeHandler__cGMRivalTimeHandler(int* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMRivalPointHandler__cGMRivalPointHandler);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char cGameModeHandler_vtbl[];

/**
 * Destroys the head-to-head scored event handler (handler 6, vtable cGMRivalPointHandler_vtbl): resets the vtable to the base handler
 * vtable cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMRivalPointHandler__cGMRivalPointHandler(int* self, int flags)
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
 * Destroys the point-target event handler (handler 7, vtable cGMPointHandler_vtbl): resets the vtable to the base handler vtable
 * cGameModeHandler_vtbl and frees the object when asked.
 *
 * @param flags  destructor flags; bit 0 frees the memory with operator_delete()
 */
extern "C" void cGMPointHandler__cGMPointHandler(cGMPointHandler* self, int flags)
{
    *(void**)self = cGameModeHandler_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGMDebugHandler__cGMDebugHandler);
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
extern "C" void cGMDebugHandler__cGMDebugHandler(int* self, int flags)
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
 * @return mpMemCard (+0x434)
 */
cMemCard* cMCOverlayManager::getMemCard()
{
    return mpMemCard;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_GetState);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager's current state (mState).
 *
 * @return mState (+0x130)
 */
int cMCOverlayManager::GetState()
{
    return (mState);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isInSaveOperation);
#ifdef SKIP_ASM
/**
 * Returns whether the overlay manager is running a save (mSave).
 *
 * @return mSave (+0xBC)
 */
int cMCOverlayManager::isInSaveOperation()
{
    return (mSave);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isInDeleteOperation);
#ifdef SKIP_ASM
/**
 * Returns whether the overlay manager is running a delete (mDelete).
 *
 * @return mDelete (+0xC0)
 */
int cMCOverlayManager::isInDeleteOperation()
{
    return (mDelete);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isInLoadOperation);
#ifdef SKIP_ASM
/**
 * Calls the overlay manager's callbackIsLoadPressed (+0x78).
 *
 * Declared void here, but the callback's result is left in the return register.
 *
 */
void cMCOverlayManager::isInLoadOperation()
{
    (callbackIsLoadPressed)();
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_isCardStatusErrorUnacknowledged);
#ifdef SKIP_ASM
/**
 * Returns whether a card status error is pending acknowledgement.
 *
 * @return 1 when mCurrDeviceStateAck (+0x440) is clear and mCurrDeviceStateLast (+0x43C) is non-zero, else 0
 */
int cMCOverlayManager::isCardStatusErrorUnacknowledged()
{
    int var_3;

    var_3 = 0;
    if (mCurrDeviceStateAck == 0) {
        var_3 = mCurrDeviceStateLast != 0;
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
 * @return true when mCurrDeviceStateLast (+0x43C) is non-zero
 */
bool cMCOverlayManager::hasCardStatusError()
{
    return (mCurrDeviceStateLast) != 0;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getCardLastState);
#ifdef SKIP_ASM
/**
 * Returns the last card status word (mCurrDeviceStateLast; bits: 4 full, 8 no card, 0x10 unformatted).
 *
 * @return mCurrDeviceStateLast (+0x43C)
 */
int cMCOverlayManager::getCardLastState()
{
    return (mCurrDeviceStateLast);
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_forceCardStatusAck);
#ifdef SKIP_ASM
/**
 * Acknowledges the current card status: sets mCurrDeviceStateAck (+0x440) to 1.
 *
 */
void cMCOverlayManager::forceCardStatusAck()
{
    mCurrDeviceStateAck = 1;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_getFirstCorruptFile);
#ifdef SKIP_ASM
/**
 * Returns the first corrupt file's index, from bits 5..8 of mCurrDeviceStateLast (+0x43C), minus 1.
 *
 * @return the 4-bit field minus 1 (-1 when it is 0)
 */
int cMCOverlayManager::getFirstCorruptFile()
{
    return (((unsigned int) ((unsigned int)mCurrDeviceStateLast) >> 5) & 0xF) - 1;
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
 * @param pollFullCard  non-zero to poll
 */
void cMCOverlayManager::PollForFullCard(int pollFullCard)
{
    mPollForFullCard = pollFullCard;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_PollForUnformatted);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager polls for an unformatted card (mPollForUnformatted, +0x120).
 *
 * @param pollUnformatted  non-zero to poll
 */
void cMCOverlayManager::PollForUnformatted(int pollUnformatted)
{
    mPollForUnformatted = pollUnformatted;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_PollForNoCard);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager polls for a missing card (mPollForNoCard, +0x124).
 *
 * @param pollNoCard  non-zero to poll
 */
void cMCOverlayManager::PollForNoCard(int pollNoCard)
{
    mPollForNoCard = pollNoCard;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_ShowCardChangeNotify);
#ifdef SKIP_ASM
/**
 * Sets whether the overlay manager shows the card-changed notice (mShowCardChangeNotify, +0x128).
 *
 * @param showCardChange  non-zero to show it
 */
void cMCOverlayManager::ShowCardChangeNotify(int showCardChange)
{
    mShowCardChangeNotify = showCardChange;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cMCOverlayManager_RetryToInit);
#ifdef SKIP_ASM
/**
 * Sets the overlay manager's retry-to-init flag (mRetryToInit, +0x12C).
 *
 * @param retryToInit  the new flag value
 */
void cMCOverlayManager::RetryToInit(int retryToInit)
{
    mRetryToInit = retryToInit;
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
void cMCOverlayManager::CallbackMemcard()
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
int cMCOverlayManager::CallbackFalse()
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
int cMCOverlayManager::CallbackTrue()
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
int cMCOverlayManager::Callbackpdate()
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
void cGameComm::stopSyncInput()
{
    mSyncInput = 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateStreamFocus);
#ifdef SKIP_ASM
extern "C" void cGameViewMan_updateStreamFocus(cGameViewMan* viewMan);
extern "C" int cGameViewMan_isStreamReady(cGameViewMan* viewMan);

/**
 * Refreshes the views' stream focus and reports whether the stream is ready.
 *
 * Calls cGameViewMan_updateStreamFocus on ViewMan (+0x84). When IsStreamPaused (+0x9C) is already set it returns false;
 * otherwise, when cGameViewMan_isStreamReady says the stream is not ready, it sets IsStreamPaused and m_bFlushStreamer
 * (+0xA0) and returns false.
 *
 * @return true when the stream is ready and not paused
 */
bool cGame::updateStreamFocus()
{
    cGameViewMan_updateStreamFocus(ViewMan);
    if (IsStreamPaused != 0) {
        return false;
    }
    if (cGameViewMan_isStreamReady(ViewMan) == 0) {
        IsStreamPaused = 1;
        m_bFlushStreamer = 1;
        return false;
    }
    return true;
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
    VPTR_OF(cObjNode, self) = cAIVisualEffectsObjNode_vtbl;
    cAIVisualEffectsMan* obj = self->AIVisualEffectsMan;
    if (((void*)obj) != 0) {
        sVEntry_cAIVisualEffectsObjNode__cAIVisualEffectsObjNode* vt = (sVEntry_cAIVisualEffectsObjNode__cAIVisualEffectsObjNode*)obj->mVtbl;
        vt[1].fn(vthis(obj, vt[1]), 3);
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
void cAIVisualEffectsObjNode::update()
{
    ((cAppDtorVirt04*)AIVisualEffectsMan)->update();
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
void cAIVisualEffectsObjNode::render()
{
    ((cAppDtorVirt04*)AIVisualEffectsMan)->render();
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
    VPTR_OF(cObjNode, self) = cSSXStatsObjNode_vtbl;
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
void cSSXStatsObjNode::update()
{
    cSSXStatsMan_update(SSXStatsMan);
}
#endif

void cSSXStatsMan_render(void*);

//100%
INCLUDE_ASM("main/ssxappdtor", cSSXStatsObjNode_render);
#ifdef SKIP_ASM
/**
 * Renders the stats node by calling cSSXStatsMan_render() on the object it holds (+0x10); that function is empty.
 */
void cSSXStatsObjNode::render()
{
    cSSXStatsMan_render(SSXStatsMan);
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
extern "C" void cGameViewMan_updateAll(cGameViewMan* viewMan);
extern "C" void cSectionMan_update(void *);
extern "C" void cGameViewMan_updateStreamFocus(cGameViewMan* viewMan);
extern "C" int cGameViewMan_isStreamReady(cGameViewMan* viewMan);
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
 * @return false while waiting on the stream, true when the game can update
 */
bool cGame::updateStreamLoading()
{
    void *vtbl;

    if ((cGameViewMan_isStreamReady(ViewMan) == 0) || (SectionMan->m_bStreamReady == 0)) {
        *(int *)(*(char**)World + 0x3D4) = 0;
        cWorld_update(World);
        cGameViewMan_updateAll(ViewMan);
        if (SectionMan->m_bStreamReady == 0) {
            cGame_updateState(this);
            cSectionMan_update(SectionMan);
        }
        if ((IsStreamPaused == 0) && (cGameViewMan_isStreamReady(ViewMan) == 0)) {
            m_bFlushStreamer = 1;
        }
        IsStreamPaused = 1;
        if (m_bFlushStreamer != 0) {
            vtbl = (void*)VPTR_OF(cGraphicsMan, GrMan);
            (*(void (**)(void *))((char*)vtbl + 0x9C))((char*)GrMan + *(short *)((char*)vtbl + 0x98));
            cGameViewMan_updateStreamFocus(ViewMan);
            cWorldCache_flushInActivePages(*(void**)World);
            m_bFlushStreamer = 0;
        }
        return false;
    }
    *(int *)(*(char**)World + 0x3D4) = 1;
    IsStreamPaused = 0;
    m_bFlushStreamer = 0;
    cWorld_update(World);
    cSectionMan_update(SectionMan);
    return true;
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
extern "C" cSSXAudio* cAudioInterface_bxAudio(void);
extern "C" void cSSXAudio_PlayIconSound(void* snd, int a, int b, float f);
extern "C" void cWScriptMan_doMissionCmd(void* mgr, int a, int b, int c);
extern "C" void cUIEngine_update(void* p);
extern "C" void func_0029ED18(void* snd);
extern "C" void cMenuSys_input(void* p);
extern "C" int cGame_activatePauseMenu(char* self, int a);
extern "C" int cOVStateManager_allowPauseMenu(void);
extern "C" void cPadCache_clearAllVibrations(void* p);
extern "C" void cOVStateManager_triggerPauseMenu(void);
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



extern "C" tMissionMenuQueueEntry* tMissionMenuQueue_getEntry(tMissionMenuQueue* self);
extern "C" tMissionMenuQueueEntry* tMissionMenuQueue_getNextEntry(tMissionMenuQueue* self);

// The script manager (cWScriptMan_TheWScriptMan); hasMessage / popMessage are inline here
// (out-of-line copies: cWScriptMan_isMenuQueueReady, cWScriptMan_popMenuQueueEntry).
struct sScript_4B48 {
    char pad0[0x1C4];
    tMissionMenuQueue queue;              // 0x1C4
    bool hasMessage() { return queue.Current != queue.Next; }
    tMissionMenuQueueEntry* popMessage() { return tMissionMenuQueue_getNextEntry(&queue); }
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
 * queue is handled: kMissionCmd_MenuStart opens overlay 29 and pushes pause owner 3 (its flags from D_004428FC) with an
 * icon sound; kMissionCmd_MenuFail runs cWScriptMan_doMissionCmd(..., kMissionCmd_Fail, ...) and either opens overlay
 * 0x1E with pause owner 3 (when the UI has no "fail" state) or pops the message. A pending script pause request
 * (D_004A20CC not 0xFFFFFFFF) opens overlay 34 with pause owner 0xC and clears the request. An active MenuSys is
 * updated (cMenuSys_input). When cGame_activatePauseMenu allows it, no replay mode 1..9 is set and no playback is
 * running, cOVStateManager_allowPauseMenu opens the pause menu: pause owner 2, the UI's virtual slot 44 with PauseOwner
 * (+0x228), audio paused. Otherwise cUIEngine_update(UI), cOVStateManager_pauseOverlayActive and cPlaybackMan_update
 * run. Each pause push writes cPauseState_Stack[++cPauseState_StackIdx] and cPauseState_Flags.
 *
 * @return true when the race should update this frame (no menu, no new pause, not just unpaused), else false
 */
// Handles script messages and pause requests; returns true when the race should run this frame.
bool cGame::updateUI()
{
    cOVStateManager_update();
    if (cMenuSys_isActive(((void*)MenuSys)) == 0) {
        sScript_4B48* sm = ((sScript_4B48*)WScriptMan);
        tMissionMenuQueue* msgQueue = &sm->queue;
        if (sm->hasMessage()) {
            tMissionMenuQueueEntry* msg = tMissionMenuQueue_getEntry(msgQueue);
            if (msg->Cmd == kMissionCmd_MenuStart) {
                cOVStateManager_triggerOverlay(29);
                cPauseState_Stack[++cPauseState_StackIdx] = 3;
                cPauseState_Flags = D_004428FC[0];
                cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 4, 0, 1.0f);
                return false;
            }
            if (msg->Cmd == kMissionCmd_MenuFail) {
                cWScriptMan_doMissionCmd(((sScript_4B48*)WScriptMan), ((int)msg->MissionId), kMissionCmd_Fail,
                                         msg->Being);
                if (cUIStateStack_findState(&(*(sStateStack_4B48*)&UI->mStateStack),
                        GetHashValue32(str_90bc_fail_b)) == 0) {
                    cOVStateManager_triggerOverlay(0x1E);
                    cPauseState_Stack[++cPauseState_StackIdx] = 3;
                    cPauseState_Flags = D_004428FC[0];
                    cUIEngine_update(UI);
                } else {
                    ((sScript_4B48*)cWScriptMan_TheWScriptMan)->popMessage();
                }
                return false;
            }
        }
    }
    if (pending_4B48()) {
        cOVStateManager_triggerOverlay(34);
        cPauseState_Stack[++cPauseState_StackIdx] = 0xC;
        cPauseState_Flags = D_00442920[0];
        D_004A20CC_4B48 = 0xFFFFFFFF;
        func_0029ED18(cAudioInterface_bxAudio());
        return false;
    }
    if (cMenuSys_isActive(((void*)MenuSys)) != 0) {
        cUIEngine_update(UI);
        cMenuSys_input(((void*)MenuSys));
        return false;
    }
    if (cGame_activatePauseMenu((char*)this, 0) != 0 && !racing_4B48(*((int*)Replay))
        && !playing_4B48(((char*)PlaybackMan)) && cOVStateManager_allowPauseMenu() != 0) {
        cPadCache_clearAllVibrations(PadCache);
        cOVStateManager_triggerPauseMenu();
        cPauseState_Stack[++cPauseState_StackIdx] = 2;
        cPauseState_Flags = D_004428F8[0];
        sUIObj_4B48* uiObject = ((sUIObj_4B48*)(UI)->mpInput);
        uiObject->vt[44].fn(vthis(uiObject, uiObject->vt[44]), *(unsigned char*)&PauseOwner);
        cSSXAudio_PauseAudio(cAudioInterface_bxAudio());
        cSSXAudio_PlayPauseSound(cAudioInterface_bxAudio());
        return false;
    }
    int wasPaused = cPauseState_Flags & 1;
    cUIEngine_update(UI);
    if (cOVStateManager_pauseOverlayActive() != 0) {
        return false;
    }
    cPlaybackMan_update(((char*)PlaybackMan));
    if (cMenuSys_isActive(((void*)MenuSys)) != 0) {
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
extern "C" int cInputSync_aiinputready(cInputSync* inputSync);
extern "C" int cInputSync_aiupdate(cInputSync* inputSync);
extern "C" int cReplay_isPauseAI(cReplay* replay);
extern "C" void cReplay_update(cReplay* replay);
extern "C" int cPlaybackMan_isPauseAI(int);
extern "C" void cBucketMan_updateBucket(void*, int);
extern "C" void cBucketMan_mergeBucket(void*, int);
extern "C" void cBucketMan_applyBucket(void*, int);
extern "C" void cWScriptMan_update(int);
extern "C" void func_002294C8(int);
extern "C" void func_003440C8(int);
extern "C" void cMultiParticleMan_update(int);
extern "C" void cWorldSound_TriggerWorldAudio(int, int, int);
extern "C" void cGameViewMan_updateAll(cGameViewMan* viewMan);
extern "C" void cGame_updateState(void*);
extern char gBucketMan[8];
extern int cPauseState_Flags;
struct sList_244E60 { int pad0; int items[3]; unsigned int count; };

/**
 * Updates the game simulation for one frame: game state, replay, scripts, AI, update buckets and views.
 *
 * When InputSync (+0x7C) is null or cInputSync_aiinputready accepts: cGame_updateState (merging bucket 1 in
 * kGFSM_Gate), cReplay_update unless paused (cPauseState_Flags bit 0), and ScriptEngine's virtual slot 5 unless the
 * replay pauses the AI or flag 8 is set. The AI step is skipped while paused, while the replay or the playback manager
 * pauses the AI or when cInputSync_aiupdate fails; otherwise it updates gBucketMan buckets 1 (unless flag 2), 5 and 6,
 * applies bucket 1, calls the AI's virtual slot 3 (vptr at +0xCC), updates buckets 2 (unless flag 0x10) and 3 (unless
 * flag 4), WScriptMan, both crowd managers and MultiParticleMan, and merges bucket 1. Skipped, only buckets 5 and 3
 * run. Then bucket 4, cGameViewMan_updateAll (except in replay mode 3), mUpdateTick (+0xAC) is counted and
 * cWorldSound_TriggerWorldAudio runs for each view camera (+0x20 of it). cGame_update has the same code inline.
 */
void cGame::updateAI()
{
    char* obj;
    int skip;
    if (InputSync == 0 || cInputSync_aiinputready(InputSync) != 0) {
        if (mCurrentStateID == kGFSM_Gate) {
            obj = gBucketMan;
            cGame_updateState(this);
            cBucketMan_mergeBucket(obj, 1);
        } else {
            obj = gBucketMan;
            cGame_updateState(this);
        }
        if ((cPauseState_Flags & 1) == 0)
            cReplay_update(Replay);
        if (cReplay_isPauseAI(Replay) == 0 && (cPauseState_Flags & 8) == 0) {
            sVEntry<void>* vt = (sVEntry<void>*)ScriptEngine->mVtbl;
            ((void (*)(void*))vt[5].fn)(vthis(ScriptEngine, vt[5]));
        }
    }
    obj = gBucketMan;
    skip = 0;
    if ((cPauseState_Flags & 1) != 0 || cReplay_isPauseAI(Replay) != 0)
        skip = 1;
    if (skip == 0) {
        if (InputSync == 0 || cInputSync_aiupdate(InputSync) != 0) {
            if (cPlaybackMan_isPauseAI((int)PlaybackMan) != 0)
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
            cAI* aiMan = AI;
            sVEntry<void>* vt = (sVEntry<void>*)aiMan->mVtbl;
            ((void (*)(void*))vt[3].fn)(vthis(aiMan, vt[3]));
        }
        if ((cPauseState_Flags & 0x10) == 0)
            cBucketMan_updateBucket(obj, 2);
        if ((cPauseState_Flags & 4) == 0)
            cBucketMan_updateBucket(obj, 3);
        cWScriptMan_update((int)WScriptMan);
        func_002294C8((int)CrowdMan2d);
        func_003440C8((int)CrowdMan3d);
        cMultiParticleMan_update((int)MultiParticleMan);
        cBucketMan_mergeBucket(obj, 1);
    } else {
        cBucketMan_updateBucket(obj, 5);
        if ((cPauseState_Flags & 4) == 0)
            cBucketMan_updateBucket(obj, 3);
    }
    cBucketMan_updateBucket(obj, 4);
    {
        int replayMode = *(int*)Replay;
        int ok = replayMode != 0 && replayMode < 10;
        int x3 = replayMode ^ 3;
        if (!ok || x3 != 0)
            cGameViewMan_updateAll(ViewMan);
    }
    mUpdateTick = mUpdateTick + 1;
    {
        sList_244E60* viewMan = (sList_244E60*)ViewMan;
        if (viewMan->count != 0) {
            unsigned int i = 0;
            do {
                cWorldSound_TriggerWorldAudio((int)WorldSound, ((sList_244E60*)ViewMan)->items[i] + 0x20, i);
                i++;
            } while (i < ((sList_244E60*)(char*)ViewMan)->count);
        }
    }
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_preUpdateOnline);
#ifdef SKIP_ASM
extern "C" void cGameComm_update(cGameComm* gameComm);

/**
 * Updates online play before the frame: cGameComm_update on cGameComm_mGameComm when one exists.
 */
extern "C" void cGame_preUpdateOnline(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_update(cGameComm_mGameComm);
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
 * fade's ending flag (+0x8) and returns false. cGame_update has the same code inline.
 *
 * @return true when the game may update, false when it is ending
 */
bool cGame::updateScreenFade()
{
    cFadeScreen_update(this);
    if (SSXApp->LoadingModule != 0) {
        unk_08 = 1;
        return false;
    }
    return true;
}
#endif
//100%
INCLUDE_ASM("main/ssxappdtor", cGame_updateVibration);
#ifdef SKIP_ASM
/**
 * Calls virtual slot 19 (vtable +0x98, vptr at +0x6C0) of each rider in AI's Riders, up to NumberOfRidersAllocated.
 *
 * cGame_update ends its update with the same loop.
 */
void cGame::updateVibration()
{
    int i;
    cRider* obj;
    char *vtbl;

    i = 0;
    if (AI->NumberOfRidersAllocated > 0) {
        do {
            obj = (cRider*)AI->Riders[i];
            i++;
            vtbl = (char*)VPTR_AT(obj, sizeof(cRiderState));
            (*(void (**)(void *))(vtbl + 0x9C))(((char*)obj) + *(short *)(vtbl + 0x98));
        } while (i < AI->NumberOfRidersAllocated);
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
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
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
extern "C" void func_00245248(cUIState* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_supopup), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Title_b));
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
 * Only widget events of type cUIState::kONNEXT with a message count. A new 0x50-byte UI state (cUIState_cUIState on the
 * UI engine, vtable D_0047DA58) is pushed on the UI state stack (engine + 0x18) with its flag at +0x48 cleared for
 * "yes" (the message hash at +0x38) or set otherwise; "yes" also calls cGraphicsMan_TheGraphicsMan's vtable slot 8 with
 * 1. Slot +0x9C (onWidgetEvent) of D_0047CA18.
 *
 * @param msg  the widget event message (ignored when null)
 * @param type  the event type (only cUIState::kONNEXT is handled)
 */
extern "C" void func_002452D8(cUIState* self, char* msg, int type)
{
    if (msg == 0) {
        return;
    }
    if (type != cUIState::kONNEXT) {
        return;
    }
    if (isHash_2452D8(*(int*)(msg + 0x38), str_yes_c)) {
        sUIObj_2452D8* newState = newUIObj_2452D8(self, 0);
        cUIStateStack_push(&self->mpEngine->mStateStack, newState);
        ((sRVEntry_2452D8*)VPTR_OF(cGraphicsMan, cGraphicsMan_TheGraphicsMan))[8].fn(vthis(cGraphicsMan_TheGraphicsMan, ((sRVEntry_2452D8*)VPTR_OF(cGraphicsMan, cGraphicsMan_TheGraphicsMan))[8]), 1);
    } else {
        sUIObj_2452D8* newState = newUIObj_2452D8(self, 1);
        cUIStateStack_push(&self->mpEngine->mStateStack, newState);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_002453E0);
#ifdef SKIP_ASM
/**
 * Answers the progressive-scan popup's menu query: 257 for cUIMenu::kONNEXT, else 0. Slot +0xA4 (onWidgetMenu) of
 * D_0047CA18.
 *
 * @param menu  the menu widget (unused)
 * @param event  the menu event
 * @return 257 when @p event is cUIMenu::kONNEXT, else 0
 */
extern "C" int func_002453E0(void* self, int menu, int event)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState__cGameLoadState);
#ifdef SKIP_ASM
extern void* cGameLoadState_vtbl[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the match / event loading state: installs its vtable cGameLoadState_vtbl at +0x8 and runs the UI-state base
 * destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register.
 *
 * @return cUIState__cUIState's result, passed through
 */
extern "C" void* cGameLoadState__cGameLoadState(cGameLoadState* self)
{
    VPTR_OF(cListNode, self) = (void**)cGameLoadState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void func_00246CF8(void* self, int a1);
extern char str_102MPMatch[];
extern char str_99QPEvent[];

/**
 * Creates the match / event loading screen: "102MPMatch" when newRace.cGamePath is kGAMEPATH_MULTIPLAYER, else
 * "99QPEvent".
 *
 * Fetches the BE interface 0 first (result unused), adds the screen to the UI engine (+0x10) with this state as owner,
 * keeps it at +0x40 and plays its first frame, then fills both players' panels with func_00246CF8(self, 0) and (self,
 * 1). Slot +0x24 (onCreateScreen) of cGameLoadState_vtbl.
 */
void cGameLoadState::onCreateScreen()
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        void* engine = mpEngine;
        void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_102MPMatch), 0);
        mpScreen = (cUIScreen*)screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    } else {
        void* engine = mpEngine;
        void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_99QPEvent), 0);
        mpScreen = (cUIScreen*)screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    }
    func_00246CF8(this, 0);
    func_00246CF8(this, 1);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState_onGainTransition);
#ifdef SKIP_ASM
extern char str_pct[];
extern char str_pct_3dpct_pct[];
extern "C" void sprintf2(char* dst, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);

/**
 * Shows the loading percentage on the match / event loading screen and optionally refreshes the player panels.
 *
 * Formats cAppMan_TheAppMan's float at +0x38 as an int with str_pct_3dpct_pct into the text widget hashed from
 * str_pct on the screen at +0x40; with @p refresh set it also runs func_00246CF8 for players 0 and 1. Slot +0x3C
 * (onGainTransition) of cGameLoadState_vtbl.
 *
 * @param refresh  non-zero to refresh both player panels
 * @return 0
 */
bool cGameLoadState::onGainTransition(int refresh)
{
    void* obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)cAppMan_TheAppMan->LoadTimer.loadpercent);
        cUIText_setUnicodeString(obj, buf);
    }
    if (refresh != 0) {
        func_00246CF8(this, 0);
        func_00246CF8(this, 1);
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadState_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGameLoadState_widgetCreateQP(void* self, int a1);
extern "C" void cGameLoadState_widgetCreateMultiP(void* self, int a1);

/**
 * Creates a widget of the match / event loading screen: cGameLoadState_widgetCreateMultiP when newRace.cGamePath is
 * kGAMEPATH_MULTIPLAYER, else cGameLoadState_widgetCreateQP.
 *
 * Fetches the BE interface 0 first (result unused). Slot +0x94 (onWidgetCreate) of cGameLoadState_vtbl.
 *
 * @param widget  the widget being created (passed on)
 */
extern "C" void cGameLoadState_onWidgetCreate(cGameLoadState* self, int widget)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    bool ok = cBENewRaceInterface_newRace.cGamePath != kGAMEPATH_MULTIPLAYER;
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
extern void* cGameLoadStateOutLodge_vtbl[];
extern "C" void* cUIState__cUIState(void* self);

/**
 * Destroys the leaving-the-lodge loading state: installs its vtable cGameLoadStateOutLodge_vtbl at +0x8 and runs the UI-state base
 * destructor cUIState__cUIState.
 *
 * The destructor flags pass through in the second argument register.
 *
 * @return cUIState__cUIState's result, passed through
 */
extern "C" void* cGameLoadStateOutLodge__cGameLoadStateOutLodge(cGameLoadStateOutLodge* self)
{
    VPTR_OF(cListNode, self) = (void**)cGameLoadStateOutLodge_vtbl;
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
 * owner, keeps it at +0x40 and plays its first frame. Slot +0x24 (onCreateScreen) of cGameLoadStateOutLodge_vtbl.
 */
void cGameLoadStateOutLodge::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_118loadoutlodge), 0);
    mpScreen = (cUIScreen*)screen;
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
 * cGameLoadStateOutLodge_vtbl.
 */
void cGameLoadStateOutLodge_onWidgetCreate(void* vself)
{
    cGameLoadStateOutLodge* self = (cGameLoadStateOutLodge*)vself;
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", cGameLoadStateOutLodge_onGainTransition);
#ifdef SKIP_ASM
extern char str_pct[];
extern char str_pct_3dpct_pct[];
extern "C" void sprintf2(char* dst, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);

/**
 * Shows the loading percentage on the leaving-the-lodge loading screen.
 *
 * Formats cAppMan_TheAppMan's float at +0x38 as an int with str_pct_3dpct_pct into the text widget hashed from
 * str_pct on the screen at +0x40. Slot +0x3C (onGainTransition) of cGameLoadStateOutLodge_vtbl.
 *
 * @return 0
 */
extern "C" int cGameLoadStateOutLodge_onGainTransition(cGameLoadStateOutLodge* self)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)cAppMan_TheAppMan->LoadTimer.loadpercent);
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
extern "C" void* func_00245708(cGameLoadStateConquer* self)
{
    VPTR_OF(cListNode, self) = (void**)D_0047C7A8;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxappdtor", func_00245730);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern "C" void func_00245730(cUIState* self)
{
    void* engine = self->mpEngine;
    void* scr = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_110ctrl_load), 0);
    self->mpScreen = (cUIScreen*)scr;
    if (scr != 0) {
        cUIScreen_playFrame(scr, 0, 0);
    }
    char* ctrlDefault = str_CtrlPS2_default;
    char* ctrlPro = str_CtrlPS2_pro;
    int controlConfig = cBENewPlayerInterface_getControllerMapping(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    cUIObj_245730* widget;
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(ctrlDefault));
    if (widget != 0) {
        widget->setVisible(controlConfig == 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_default_text_b));
    if (widget != 0) {
        widget->setVisible(controlConfig == 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(ctrlPro));
    if (widget != 0) {
        widget->setVisible(controlConfig != 0);
    }
    widget = (cUIObj_245730*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pro_text_b));
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
extern "C" int func_002458B8(cGameLoadStateConquer* self)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pct));
    if (obj != 0) {
        char buf[208];
        sprintf2(buf, str_pct_3dpct_pct, (int)cAppMan_TheAppMan->LoadTimer.loadpercent);
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
    VPTR_OF(cListNode, self) = (void**)cFELoadHintState_vtbl;
    return cUIState__cUIState(self);
}
#endif

