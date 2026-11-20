#ifndef MAIN_LOADSCREENS_H
#define MAIN_LOADSCREENS_H

// The load screens cSSXApp shows while it boots and loads: cInitLoadScreen (the first, with the progressive-scan
// prompt), cPreStartScreen / cPreGameLoadScreen / cPreFELoadScreen (short hand-overs that wait for the previous screen
// to end), and the two cLoadScreen screens that draw through a UI engine: cStartScreen (the splash screens) and
// cGameLoadScreen (the in-game load). Methods in main/loadscreens_prestart, main/gameload and main/ssxappdtor.
//
// FLAT layouts, like main/game.h: each derived class repeats its base's part. Member names are EA's, from SSX On Tour's
// PSP DWARF ([O]); every retail access through `this` in the classes' methods falls on a member start
// (tools/gen/self_offsets.py), and every constructor stores its vtable at +0x0 ([R]). Sizes are the retail
// allocations (cMemMan_alloc with the class name) and On Tour's, except cInitLoadScreen: retail keeps On Tour's two
// bools as words (0xC, On Tour 0x8). On Tour's eStartScreenType is an int here.

#include "layout.h"

struct cUIEngine;

struct cInitLoadScreen {
    void** mVtbl;                   // 0x00 [R] ctor = cInitLoadScreen_vtbl
    int progressive;                // 0x04 [O] (On Tour bool); [U] activateProgressiveMode sets it
    int ending;                     // 0x08 [O] (On Tour bool, +0x5 there); [U] loseFocus sets it, update reads it
};

CHECK_OFFSET(cInitLoadScreen, ending, 0x08);
CHECK_SIZE(cInitLoadScreen, 0xC);

struct cLoadScreen {
    void** mVtbl;                   // 0x00 [R] ctor = its vtable (cLoadScreen_vtbl)
    char unk_004[0x8];              // 0x04 On Tour's base part; no retail access
    cUIEngine* mpUI;                // 0x0C [O]; [R] ctor
};

CHECK_OFFSET(cLoadScreen, mpUI, 0x0C);
CHECK_SIZE(cLoadScreen, 0x10);

struct cPreStartScreen {
    void** mVtbl;                   // 0x00 [R] ctor = cPreStartScreen_vtbl
    float progressivetimer;         // 0x04 [O]; [U] update counts it, loseFocus
    cInitLoadScreen* nextscreen;    // 0x08 [O]; [U] update
};

CHECK_OFFSET(cPreStartScreen, nextscreen, 0x08);
CHECK_SIZE(cPreStartScreen, 0xC);

struct cPreGameLoadScreen {
    void** mVtbl;                   // 0x00 [R] ctor = cPreGameLoadScreen_vtbl
    int ending;                     // 0x04 [O] (On Tour bool); [U] loseFocus, update
};

CHECK_OFFSET(cPreGameLoadScreen, ending, 0x04);
CHECK_SIZE(cPreGameLoadScreen, 0x8);

struct cPreFELoadScreen {
    void** mVtbl;                   // 0x00 [R] ctor = its vtable (cPreFELoadScreen_vtbl)
    int ending;                     // 0x04 [O] (On Tour bool); [U] loseFocus, update
};

CHECK_OFFSET(cPreFELoadScreen, ending, 0x04);
CHECK_SIZE(cPreFELoadScreen, 0x8);

struct cStartScreen {
    // --- cLoadScreen
    void** mVtbl;                   // 0x00 [R] ctor = cStartScreen_vtbl
    char unk_004[0x8];              // 0x04 cLoadScreen's base part; no retail access
    cUIEngine* mpUI;                // 0x0C [O]; [U] load, purge, render
    // --- cStartScreen
    int SplashTexID;                // 0x10 [O]
    int DisplayTicks;               // 0x14 [O]; [U] gainFocus, update
    int meStartScreen_type;         // 0x18 [O] eStartScreenType; [R] ctor; [U] load
};

CHECK_OFFSET(cStartScreen, mpUI, 0x0C);
CHECK_OFFSET(cStartScreen, SplashTexID, 0x10);
CHECK_OFFSET(cStartScreen, meStartScreen_type, 0x18);
CHECK_SIZE(cStartScreen, 0x1C);

struct cGameLoadScreen {
    // --- cLoadScreen
    void** mVtbl;                   // 0x00 [R] ctor = cGameLoadScreen_vtbl
    char unk_004[0x8];              // 0x04 cLoadScreen's base part; no retail access
    cUIEngine* mpUI;                // 0x0C [O]; [U] load, purge, render
    // --- cGameLoadScreen
    int mTexID[2];                  // 0x10 [O]; [R] ctor
};

CHECK_OFFSET(cGameLoadScreen, mpUI, 0x0C);
CHECK_OFFSET(cGameLoadScreen, mTexID, 0x10);
CHECK_SIZE(cGameLoadScreen, 0x18);

#endif /* MAIN_LOADSCREENS_H */
