#ifndef UI_UIENGINE_H
#define UI_UIENGINE_H

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
// The state stack and the banks are byte arrays here (no header has cList / cUIStateStack / the banks yet).

#include "layout.h"

struct cUICustomInterface;
struct cUIFontInterface;
struct cUIInput;
struct cUILocaleInterface;
struct cUISoundInterface;

struct cUIEngine {
    cUICustomInterface* mpCustom;          // 0x00 [O]; [R] ctor = 0
    char* mpData;                          // 0x04 [O]; [R] ctor = 0; [U] cUIEngine_loadFile's data
    cUIFontInterface* mpFontInterface;     // 0x08 [O]; [R] ctor = 0
    cUIInput* mpInput;                     // 0x0C [O]; [R] ctor = 0
    cUILocaleInterface* mpLocaleInterface; // 0x10 [O]; [R] ctor = 0
    cUISoundInterface* mpSound;            // 0x14 [O]; [R] ctor = 0; [U] the states' UI sounds
    char mStateStack[0x38];                // 0x18 [O] cUIStateStack: cList mStack (0x18), cList mExplicitStack
                                           //      (0x34); [R] cList_cList on both; [U] cUIStateStack_pushPopup ...
    char mAnimBank[0x8];                   // 0x50 [O] cUIAnimationBank: mpAnimationBlock, mpEngine (= this, [R])
    char mTextureBank[0x18];               // 0x58 [O] cUITextureBank: header, texture IDs, count, elements, count,
                                           //      mpEngine (0x6C = this, [R]); [U] cUITextureBank_getElementByHashname
};

CHECK_OFFSET(cUIEngine, mpSound, 0x14);
CHECK_OFFSET(cUIEngine, mStateStack, 0x18);
CHECK_OFFSET(cUIEngine, mAnimBank, 0x50);
CHECK_OFFSET(cUIEngine, mTextureBank, 0x58);
CHECK_SIZE(cUIEngine, 0x70);

#endif
