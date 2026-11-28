#ifndef MAIN_FADESCREEN_H
#define MAIN_FADESCREEN_H
#pragma interface

// cFadeScreen: the app modules' screen fade (an app module that fades the screen in after it gains focus and out
// before it lets go of it). It is a base class: cGame (main/game.h), cFE / cRealFE (vtable cFE_vtbl 0x0046D878),
// cDebugFE, cLoadScreen and the load screens built on it (cStartScreen, cGameLoadScreen, cFELoadScreen,
// cPreGameLoadScreen, cPreFELoadScreen; main/loadscreens.h) run cFadeScreen_cFadeScreen first and put their own
// members from 0xC on; their loseFocus / update / render call cFadeScreen_loseFocus / update / drawFade on themselves.
// EA's source file is not known (no assert of its own; On Tour has it in main/all_in_one_main); retail code:
// main/gameload (0x00231CD0 ... 0x00231D60) and the destructor in main/ssxappdtor (0x00243A40).
//
// Size 0xC:
//   [R] the ctor writes 0x0, 0x4 and 0x8; every derived class starts its own members at 0xC (cGame's AI, cLoadScreen's
//       mpUI, cFE's mpUI; cFE_cFE builds its camera at 0x10).
//   [O] On Tour's class is 0xC too: cAppModule's vptr, float FadeTimer, bool IsEnding (1 byte there; a word in retail:
//       the methods use lw/sw).
// Base: cAppModule (On Tour size 0x4: no data, only the vptr at 0x0). Vtable cFadeScreen_vtbl (0x0047D980, 7 slots, On
// Tour cAppModule order: 1 dtor, 2 load (pure), 3 purge cAppModule_purge, 4 loseFocus, 5 gainFocus
// cAppModule_gainFocus, 6 update, 7 render (pure)); the destructor stores cAppModule's vtable cAppModule_vtbl back.
//
// A real C++ class (`#pragma interface`: no unit emits its vtable or inline functions; the equate binds g++'s name to
// the retail table): it derives from cAppModule (bx/appman.h), and its overrides are virtual in cAppModule's slot
// order. The destructor is implicit and inline: the derived modules' destructors inline it (they store
// cAppModule_vtbl); its out-of-line copy cFadeScreen__cFadeScreen (slot 1, main/ssxappdtor) is a C function.
// Evidence tags:
//   [R] retail's constructor / destructor;
//   [P] the July 2003 preview (layout_xlate: 0xC -> 0x4, 0x14 -> 0x8, from the paired update);
//   [U] how retail code uses the field (the C bodies in src/main/gameload.cpp and the asm);
//   [O] SSX On Tour's DWARF (class cFadeScreen, cAppModule): names, types and order.

#include "layout.h"
#include "bx/appman.h"
#include "bx/memman.h"

struct cFadeScreen : public cAppModule {
    //   the vptr 0x000 (cAppModule's) [R] cFadeScreen_vtbl (ctor); the dtor stores cAppModule's cAppModule_vtbl;
    //       derived ctors overwrite it with their own
    float FadeTimer;                // 0x004 [O][P] (preview 0xC); [R] ctor 0.2; [U] update adds / subtracts 1/60
                                    //       (clamped at 0), loseFocus returns FadeTimer >= 0.2, drawFade draws with it
    int IsEnding;                   // 0x008 [O][P] (bool; preview 0x14); [R] ctor 0; [U] loseFocus sets it, update
                                    //       picks the fade direction by it

    // Methods bound to their retail symbols by asm labels.
    cFadeScreen() __asm__("cFadeScreen_cFadeScreen");
    virtual bool loseFocus() __asm__("cFadeScreen_loseFocus");      // 4
    virtual void update() __asm__("cFadeScreen_update");            // 6
    bool drawFade() __asm__("cFadeScreen_drawFade");
    void allowFade() __asm__("cFadeScreen_allowFade");
};

CHECK_OFFSET(cFadeScreen, FadeTimer, 0x004);
CHECK_OFFSET(cFadeScreen, IsEnding, 0x008);
CHECK_SIZE(cFadeScreen, 0xC);
__asm__("_vt$11cFadeScreen = cFadeScreen_vtbl");   // g++'s name for the retail vtable

#endif /* MAIN_FADESCREEN_H */
