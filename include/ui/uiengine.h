#ifndef UI_UIENGINE_H
#define UI_UIENGINE_H
#pragma interface

// cUIEngine: EA's front-end UI engine (one per front end: cFEReal's, the in-game overlay's). It holds the UI data
// file, the interfaces the game plugs in (custom, font, input, locale, sound), the stack of cUIState screens, and the
// animation and texture banks. Every front-end state keeps it in cUIState::mpEngine (0x10); the states push and pop
// popups on mStateStack (cUIStateStack_pushPopup / pushSpecial) and play sounds through mpSound.
//
// Size 0x70 (On Tour 0x70). A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF
// (class cUIEngine), placed by:
//   [R] retail's constructor cUIEngine_cUIEngine (ui/uiengine): zeroes 0x0-0x14, builds the two cLists of the state
//       stack (0x18, 0x34), zeroes the banks and stores itself into both banks' mpEngine (0x54, 0x6C);
//   [U] how retail code uses the field;
//   [O] On Tour's member order.
// The state stack and the banks are embedded by value, from their own headers (ui/uistatestack.h, ui/uianimation.h,
// ui/uitexturebank.h).

#include "layout.h"
#include "ui/uistatestack.h"
#include "ui/uianimation.h"
#include "ui/uitexturebank.h"

class cUICustomInterface;
struct cUIState;
struct cUIScreen;
struct cUIFontInterface;
struct cUIInput;
struct cUILocaleInterface;
class cUISoundInterface;

struct cUIEngine {
    cUICustomInterface* mpCustom;          // 0x00 [O]; [R] ctor = 0
    char* mpData;                          // 0x04 [O]; [R] ctor = 0; [U] cUIEngine_loadFile's data
    cUIFontInterface* mpFontInterface;     // 0x08 [O]; [R] ctor = 0
    cUIInput* mpInput;                     // 0x0C [O]; [R] ctor = 0
    cUILocaleInterface* mpLocaleInterface; // 0x10 [O]; [R] ctor = 0
    cUISoundInterface* mpSound;            // 0x14 [O]; [R] ctor = 0; [U] the states' UI sounds
    cUIStateStack mStateStack;             // 0x18 [O] cUIStateStack (ui/uistatestack.h): cList mStack (0x18), cList
                                           //      mExplicitStack (0x34); [R] cUIEngine_cUIEngine: cList_cList on both;
                                           //      [U] cUIStateStack_push / pushPopup ... (&mStateStack)
    cUIAnimationBank mAnimBank;            // 0x50 [O] cUIAnimationBank (ui/uianimation.h): mpAnimationBlock, mpEngine
                                           //      (0x54); [R] the ctor stores this in mAnimBank.mpEngine; [U]
                                           //      cUIAnimationBank_setData (cUIEngine_loadFile)
    cUITextureBank mTextureBank;           // 0x58 [O] cUITextureBank (ui/uitexturebank.h): header, texture IDs, count,
                                           //      elements, count (0x58..0x68, zeroed by the ctor), mpEngine (0x6C = this,
                                           //      [R]); [U] cUITextureBank_getElementByHashname(&mTextureBank, ...)

    // Methods bound to their retail symbols by asm labels.
    int loadFile(int id) __asm__("cUIEngine_loadFile");
    void* addScreenByHashName(cUIState* owner, unsigned int hash, void* screen)
        __asm__("cUIEngine_addScreenByHashName");
    void render2D() __asm__("cUIEngine_render2D");
    void render3D() __asm__("cUIEngine_render3D");
    void update() __asm__("cUIEngine_update");
};

CHECK_OFFSET(cUIEngine, mpSound, 0x14);
CHECK_OFFSET(cUIEngine, mStateStack, 0x18);
CHECK_OFFSET(cUIEngine, mAnimBank, 0x50);
CHECK_OFFSET(cUIEngine, mTextureBank, 0x58);
CHECK_SIZE(cUIEngine, 0x70);


// ---------------------------------------------------------------------------------------------------------------------
// cUICustomInterface: the game's hook into the UI engine (cUIEngine::mpCustom above): per-frame update, screen
// events and the next / previous state factories. The front end's cFECustom (fe/fecustom.h) derives from it.
// [O] On Tour's class: size 0x8, member mpEngine, virtual dtor + onUpdate / onScreenEvent / getNextState /
// getPrevState; [R] retail vtable cUICustomInterface_vtbl (0x0046DD28) has exactly those 5 slots (the base versions
// are in the decomp's unit fe/fepopupmisc: cUICustomInterface__cUICustomInterface, _onUpdate, _onScreenEvent,
// _getNextState, _getPrevState), its destructor stores the table at +0x4 (the vptr follows the one member, as
// g++ 2.95 places it), and cFECustom's own members start at 0x8. Never allocated on its own; its constructor is
// inlined into cFECustom's. A real polymorphic class (nothing defines a member here, so no vtable is emitted).
// Evidence tags as above.
class cUICustomInterface {
public:
    cUIEngine* mpEngine;             // 0x000 [O]; [R] cFECustom's ctors store the engine argument (fe/fecustom.h); [U]
                                     //       the derived getNextState / getPrevState build their states on it
    // 0x004: the vptr. [R] cUICustomInterface__cUICustomInterface stores cUICustomInterface_vtbl (0x0046DD28), the
    // cFECustom ctors cFECustom_vtbl. Slots [R] that table, names and return types [O]; parameters from the
    // cFECustom implementations (fe/fereal), which take and every caller passes an int state argument (On Tour:
    // void* pData):
    // slot 1; inline and empty: cFECustom's destructor inlines it (retail's out-of-line copy
    // cUICustomInterface__cUICustomInterface stays a C function)
    virtual ~cUICustomInterface() {}
    virtual void onUpdate()
        __asm__("cUICustomInterface_onUpdate__FPv");                                            // slot 2
    virtual bool onScreenEvent(cUIScreen* pScreen, unsigned int Event)
        __asm__("cUICustomInterface_onScreenEvent__FPv"); // slot 3
    virtual cUIState* getNextState(cUIState* pCurrentState, int pData)
        __asm__("cUICustomInterface_getNextState__FPv"); // slot 4
    virtual cUIState* getPrevState(cUIState* pCurrentState, int pData)
        __asm__("cUICustomInterface_getPrevState__FPv"); // slot 5
};

CHECK_OFFSET(cUICustomInterface, mpEngine, 0x000);
CHECK_SIZE(cUICustomInterface, 0x8);
__asm__("_vt$18cUICustomInterface = cUICustomInterface_vtbl");   // g++'s name for the retail vtable

// cUISoundInterface: the UI engine's sound hook (cUIEngine::mpSound above): the states play their UI sounds through
// its slot 2 (playSound). The front end's cFESound and the overlays' cOVSound derive from it (fe/fesound.h, 0x8 each);
// gUISoundNull is a bare instance the engine falls back to when no sound interface is set (cUIEngine's listener,
// cFEStateStore's sound). [O] On Tour's class: size 0x4, no data, virtual ~ / playSound / soundInitialize /
// soundShutdown; [R] retail vtable cUISoundInterface_vtbl (0x0046DD60, 4 slots: cUISoundInterface__cUISoundInterface,
// _playSound, _soundInitialize, _soundShutdown), stored back at +0x0 by the cFESound / cOVSound destructors. A real
// polymorphic class. Evidence tags as above.
class cUISoundInterface {
public:
    // 0x000: the vptr only ([O] On Tour size 0x4). [R] cUISoundInterface_vtbl (cUISoundInterface_cUISoundInterface;
    // the cFESound / cOVSound dtors restore it); cFESound_cFESound stores cFESound_vtbl. Slots [R], names [O]:
    // slot 1; inline and empty: cFESound's and cOVSound's destructors inline it (retail's out-of-line copy
    // cUISoundInterface__cUISoundInterface stays a C function)
    virtual ~cUISoundInterface() {}
    virtual void playSound(int e) __asm__("cUISoundInterface_playSound__FPv");     // slot 2
    virtual void soundInitialize() __asm__("cUISoundInterface_soundInitialize__FPv");    // slot 3
    virtual void soundShutdown() __asm__("cUISoundInterface_soundShutdown__FPv");      // slot 4
};

CHECK_SIZE(cUISoundInterface, 0x4);
__asm__("_vt$17cUISoundInterface = cUISoundInterface_vtbl");   // g++'s name for the retail vtable

#endif
