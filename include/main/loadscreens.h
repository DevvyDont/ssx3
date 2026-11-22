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


// ---------------------------------------------------------------------------------------------------------------------
// cFELoadScreen: the app module shown while the front end loads a race: a cLoadScreen whose UI engine ("data/ui/fl")
// runs one load state - cFELoadStateInLodge (going to the lodge), cFELoadHintState (a hint, with the rider's picture)
// or cFELoadState (both riders' stats) - plus the riders' pictures it loads for them. EA's source: probably
// d:/ssx/code/source/main/loadscreens.cpp (builds/x/v2 file_hints: retail cFELoadState_InitwidgetMultiP's assert, line
// 1191, names that file; On Tour keeps all four classes in main/all_in_one_main). Retail code: load, gainFocus, purge,
// render, update and loadTexture (cFELoadScreen_loadTexture) in fe/festateloadscreen (0x00233438 ... 0x00233A70); the dtor, the
// ctor and getTexttureID in main/ssxappdtor (0x00243C00 ... 0x00243C70).
//
// Size 0x1C:
//   [R] cPreFELoadScreen_update: cMemMan_alloc(0x1C, "FELoad") + the inlined ctor (cFadeScreen_cFadeScreen, vtable,
//       0xC / 0x10 / 0x14 = 0); the same stores in cFELoadScreen_cFELoadScreen;
//   [O] On Tour's class is 0x1C: cLoadScreen (0x10) + int mTexID[2] + int SplashTexID.
// Base: cLoadScreen (above, 0x10: mpUI at 0xC) : cFadeScreen (main/fadescreen.h, 0xC) : cAppModule (the vptr). Vtable
// cFELoadScreen_vtbl (0x0047D740, 7 slots: 1 dtor, 2 load, 3 purge, 4 cLoadScreen_loseFocus, 5 gainFocus, 6 update,
// 7 render); the dtor stores cAppModule's cAppModule_vtbl.
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's ctor / dtor / allocation; [U] how retail code uses the
// field (src/fe/festateloadscreen.cpp, src/main/ssxappdtor.cpp, the asm); [O] SSX On Tour's DWARF (class cFELoadScreen,
// cLoadScreen, cFadeScreen): names, types and order.

struct cListNode;
struct cUIState;
struct cUIScreen;
struct cFELoadScreen;

struct cFELoadScreen {
    // --- cAppModule / cFadeScreen (main/fadescreen.h, 0xC)
    void** mVtbl;                   // 0x000 [R] cFadeScreen_vtbl, then cFELoadScreen_vtbl (ctor); cAppModule_vtbl (dtor)
    float FadeTimer;                // 0x004 [O] cFadeScreen::FadeTimer; [R] cFadeScreen_cFadeScreen 0.2; [U] render:
                                    //       cFadeScreen_drawFade(this)
    int IsEnding;                   // 0x008 [O] cFadeScreen::IsEnding (bool, a word); [R] 0; [U] cLoadScreen_update /
                                    //       loseFocus
    // --- cLoadScreen (above, 0x10)
    cUIEngine* mpUI;                // 0x00C [O] cLoadScreen::mpUI; [R] ctor 0; [U] load builds it (cMemMan_alloc(0x70,
                                    //       "UIEngine"), "data/ui/fl", fonts / locale / a cFEInput) and pushes the load
                                    //       state on its stack (+0x18); purge deletes it; render / update draw and run
                                    //       it
    // --- cFELoadScreen
    int mTexID[2];                  // 0x010 [O]; [R] ctor 0, 0; [U] loadTexture (cFELoadScreen_loadTexture) fills [0] (type 1: the
                                    //       human's "CharaPics") or [0] and [1] (type 2: both players'); purge frees
                                    //       the non-zero ones; getTexttureID returns [index]; the load states read them
                                    //       through mpParentModule
    int SplashTexID;                // 0x018 [O]; [U] load sets -1; purge frees it when >= 0
};

CHECK_OFFSET(cFELoadScreen, mVtbl, 0x000);
CHECK_OFFSET(cFELoadScreen, FadeTimer, 0x004);
CHECK_OFFSET(cFELoadScreen, IsEnding, 0x008);
CHECK_OFFSET(cFELoadScreen, mpUI, 0x00C);
CHECK_OFFSET(cFELoadScreen, mTexID, 0x010);
CHECK_OFFSET(cFELoadScreen, SplashTexID, 0x018);
CHECK_SIZE(cFELoadScreen, 0x1C);

// ---------------------------------------------------------------------------------------------------------------------
// cFELoadState: cFELoadScreen's race-stats load state: "103MPStat" (two players on the online game path:
// InitwidgetMultiP fills each player's rank, time / score, knockdowns, wipeouts, tricks, handplants, ubertricks,
// ubergrinds and picture) or "100QPStat" (widgetCreateQP fills the quick-play stats); onGainTransition prints the load
// percentage into "%%".
// Retail code: all in fe/festateloadhint: the dtor (0x00245B58), onCreateScreen, onGainTransition, onWidgetCreate,
// InitwidgetMultiP (0x002472C0) and widgetCreateQP (0x00247AB0); there is no out-of-line ctor.
//
// Size 0x4C: [R] cFELoadScreen_load: cMemMan_alloc(0x4C, "cFELoadState") + cUIState_cUIState(mem, mpUI), word 0x48 = 0,
// vtable, then 0x48 = the load screen; [O] On Tour's class is 0x4C: cFELoadScreen* mpParentModule at 0x48.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (the dtor stores cFELoadState_vtbl and
// runs cUIState__cUIState). Vtable cFELoadState_vtbl (0x0047C608, 24 slots: 1 dtor, 4 onCreateScreen,
// 7 onGainTransition, 18 onWidgetCreate).
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's dtor and the inlined ctors / allocations in
// cFELoadScreen_load; [U] how retail code uses the field (src/fe/festateloadhint.cpp and the asm); [O] SSX On Tour's
// DWARF (class cFELoadState, cUIState): names, types and order.

struct cFELoadState {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFELoadState_vtbl (cFELoadScreen_load, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI; [U] onCreateScreen adds the screen through
                                    //       it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onGainTransition and
                                    //       InitwidgetMultiP look the widgets up on it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cFELoadState
    cFELoadScreen* mpParentModule;  // 0x048 [O]; [R] 0, then cFELoadScreen_load stores the load screen; [U]
                                    //       InitwidgetMultiP shows mpParentModule->mTexID[player] as the player's
                                    //       picture
};

CHECK_OFFSET(cFELoadState, mpPrev, 0x000);
CHECK_OFFSET(cFELoadState, mpNext, 0x004);
CHECK_OFFSET(cFELoadState, mVtbl, 0x008);
CHECK_OFFSET(cFELoadState, mHashName, 0x00C);
CHECK_OFFSET(cFELoadState, mpEngine, 0x010);
CHECK_OFFSET(cFELoadState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFELoadState, mCtrlBits, 0x015);
CHECK_OFFSET(cFELoadState, unk_016, 0x016);
CHECK_OFFSET(cFELoadState, miMiscData, 0x018);
CHECK_OFFSET(cFELoadState, mAttrState, 0x01C);
CHECK_OFFSET(cFELoadState, mpParentState, 0x020);
CHECK_OFFSET(cFELoadState, mScreens, 0x024);
CHECK_OFFSET(cFELoadState, mpScreen, 0x040);
CHECK_OFFSET(cFELoadState, mUserNum, 0x044);
CHECK_OFFSET(cFELoadState, unk_045, 0x045);
CHECK_OFFSET(cFELoadState, mpParentModule, 0x048);
CHECK_SIZE(cFELoadState, 0x4C);

// ---------------------------------------------------------------------------------------------------------------------
// cFELoadHintState: cFELoadScreen's hint load state ("101QPMPHints": a hint title and body, and the rider's picture
// mpParentModule->mTexID[0] on "Bitmap0000"); onGainTransition prints the load percentage. Retail code: onCreateScreen,
// onGainTransition, onWidgetCreate (empty) in fe/festateloadhint (0x00245950 ... 0x00245B50); the dtor sits in
// main/ssxappdtor (0x00245928); there is no out-of-line ctor.
//
// Size 0x4C: [R] cFELoadScreen_load: cMemMan_alloc(0x4C, "cFELoadHintState") + cUIState_cUIState(mem, mpUI), word 0x48
// = 0, vtable, then 0x48 = the load screen; [O] On Tour's class is 0x4C: cFELoadScreen* mpParentModule at 0x48.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable cFELoadHintState_vtbl (0x0047C6D8, 24 slots:
// 1 dtor, 4 onCreateScreen, 7 onGainTransition, 18 onWidgetCreate).
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's dtor and the inlined ctors / allocations in
// cFELoadScreen_load; [U] how retail code uses the field (src/fe/festateloadhint.cpp and the asm); [O] SSX On Tour's
// DWARF (class cFELoadHintState, cUIState): names, types and order.

struct cFELoadHintState {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFELoadHintState_vtbl (cFELoadScreen_load, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI; [U] onCreateScreen adds the screen through
                                    //       it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen and looks "hint_title",
                                    //       "hint_body" and "Bitmap0000" up on it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cFELoadHintState
    cFELoadScreen* mpParentModule;  // 0x048 [O]; [R] 0, then cFELoadScreen_load stores the load screen; [U]
                                    //       onCreateScreen shows mpParentModule->mTexID[0] on "Bitmap0000" when both
                                    //       are set
};

CHECK_OFFSET(cFELoadHintState, mpPrev, 0x000);
CHECK_OFFSET(cFELoadHintState, mpNext, 0x004);
CHECK_OFFSET(cFELoadHintState, mVtbl, 0x008);
CHECK_OFFSET(cFELoadHintState, mHashName, 0x00C);
CHECK_OFFSET(cFELoadHintState, mpEngine, 0x010);
CHECK_OFFSET(cFELoadHintState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFELoadHintState, mCtrlBits, 0x015);
CHECK_OFFSET(cFELoadHintState, unk_016, 0x016);
CHECK_OFFSET(cFELoadHintState, miMiscData, 0x018);
CHECK_OFFSET(cFELoadHintState, mAttrState, 0x01C);
CHECK_OFFSET(cFELoadHintState, mpParentState, 0x020);
CHECK_OFFSET(cFELoadHintState, mScreens, 0x024);
CHECK_OFFSET(cFELoadHintState, mpScreen, 0x040);
CHECK_OFFSET(cFELoadHintState, mUserNum, 0x044);
CHECK_OFFSET(cFELoadHintState, unk_045, 0x045);
CHECK_OFFSET(cFELoadHintState, mpParentModule, 0x048);
CHECK_SIZE(cFELoadHintState, 0x4C);

// ---------------------------------------------------------------------------------------------------------------------
// cFELoadStateInLodge: cFELoadScreen's load state when the front end starts in the lodge (cRealFE_mStartState 39):
// "117loadinlodge" and the load percentage. Retail code: the dtor, onCreateScreen, onWidgetCreate (empty) and
// onGainTransition in fe/festateloadhint (0x00245D28 ... 0x00245DC0); there is no out-of-line ctor.
//
// Size 0x48: [R] cFELoadScreen_load: cMemMan_alloc(0x48, "cFELoadStateInLodge") + cUIState_cUIState(mem, mpUI) +
// vtable; [O] On Tour's class is 0x48 (no members).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable cFELoadStateInLodge_vtbl (0x0047C538, 24 slots:
// 1 dtor, 4 onCreateScreen, 7 onGainTransition, 18 onWidgetCreate).
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's dtor and the inlined ctors / allocations in
// cFELoadScreen_load; [U] how retail code uses the field (src/fe/festateloadhint.cpp and the asm); [O] SSX On Tour's
// DWARF (class cFELoadStateInLodge, cUIState): names, types and order.

struct cFELoadStateInLodge {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFELoadStateInLodge_vtbl (cFELoadScreen_load, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI; [U] onCreateScreen adds "117loadinlodge"
                                    //       through it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onGainTransition looks "%" up on
                                    //       it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cFELoadStateInLodge, mpPrev, 0x000);
CHECK_OFFSET(cFELoadStateInLodge, mpNext, 0x004);
CHECK_OFFSET(cFELoadStateInLodge, mVtbl, 0x008);
CHECK_OFFSET(cFELoadStateInLodge, mHashName, 0x00C);
CHECK_OFFSET(cFELoadStateInLodge, mpEngine, 0x010);
CHECK_OFFSET(cFELoadStateInLodge, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFELoadStateInLodge, mCtrlBits, 0x015);
CHECK_OFFSET(cFELoadStateInLodge, unk_016, 0x016);
CHECK_OFFSET(cFELoadStateInLodge, miMiscData, 0x018);
CHECK_OFFSET(cFELoadStateInLodge, mAttrState, 0x01C);
CHECK_OFFSET(cFELoadStateInLodge, mpParentState, 0x020);
CHECK_OFFSET(cFELoadStateInLodge, mScreens, 0x024);
CHECK_OFFSET(cFELoadStateInLodge, mpScreen, 0x040);
CHECK_OFFSET(cFELoadStateInLodge, mUserNum, 0x044);
CHECK_OFFSET(cFELoadStateInLodge, unk_045, 0x045);
CHECK_SIZE(cFELoadStateInLodge, 0x48);

// ---------------------------------------------------------------------------------------------------------------------
// cGameLoadState: cGameLoadScreen's load state for a race / event: "102MPMatch" on the online game path (cGamePath 2,
// both players' panels: level and seven attribute bars) or "99QPEvent" (the event's location, titles, course stats,
// rider and map); onGainTransition prints the load percentage into "%%". The class name: the allocation tag
// "cGameLoadState" in cGameLoadScreen_load and the preview's _vt$14cGameLoadState (builds/x/v2 pairs it with retail
// 0x0047C948; its onWidgetCreate__14cGameLoadStateP8cUIThing is an exact body match of cGameLoadState_onWidgetCreate).
// Retail code: the dtor, onCreateScreen, onGainTransition and onWidgetCreate in main/ssxappdtor (0x002453F0 ...
// 0x00245598); the preview's widgetCreateMultiP / widgetCreateQP (cGameLoadState_widgetCreateMultiP / cGameLoadState_widgetCreateQP) and the player-panel
// helper func_00246CF8 in fe/festateloadhint; there is no out-of-line ctor. No On Tour class.
//
// Size 0x4C: [R] cGameLoadScreen_load: operator new(0x4C, "cGameLoadState") + cUIState_cUIState(mem, mpUI), vtable,
// word 0x48 = 0, then 0x48 = the load screen.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable D_0047C948 (24 slots: 1 dtor, 4 onCreateScreen,
// 7 onGainTransition, 18 onWidgetCreate).
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's dtor and the inlined ctor / allocation in
// cGameLoadScreen_load; [U] how retail code uses the field (src/main/ssxappdtor.cpp, src/fe/festateloadhint.cpp);
// [O] SSX On Tour's DWARF for the cUIState part.

struct cGameLoadState {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_0047C948 (cGameLoadScreen_load, dtor cGameLoadState__cGameLoadState)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI; [U] onCreateScreen adds the screen through
                                    //       it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onGainTransition looks "%" up on
                                    //       it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cGameLoadState
    cGameLoadScreen* mpParentModule; // 0x048 [R] 0, then cGameLoadScreen_load stores the load screen (and loads its
                                    //       texture 0); [U] func_00246CF8 and cGameLoadState_widgetCreateQP show
                                    //       mpParentModule->mTexID[player] (views sLoadInfo_6CF8 / image[2] at 0x10).
                                    //       Name taken from cFELoadState's [O] member in the same role (no DWARF here)
};

CHECK_OFFSET(cGameLoadState, mpPrev, 0x000);
CHECK_OFFSET(cGameLoadState, mpNext, 0x004);
CHECK_OFFSET(cGameLoadState, mVtbl, 0x008);
CHECK_OFFSET(cGameLoadState, mHashName, 0x00C);
CHECK_OFFSET(cGameLoadState, mpEngine, 0x010);
CHECK_OFFSET(cGameLoadState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cGameLoadState, mCtrlBits, 0x015);
CHECK_OFFSET(cGameLoadState, unk_016, 0x016);
CHECK_OFFSET(cGameLoadState, miMiscData, 0x018);
CHECK_OFFSET(cGameLoadState, mAttrState, 0x01C);
CHECK_OFFSET(cGameLoadState, mpParentState, 0x020);
CHECK_OFFSET(cGameLoadState, mScreens, 0x024);
CHECK_OFFSET(cGameLoadState, mpScreen, 0x040);
CHECK_OFFSET(cGameLoadState, mUserNum, 0x044);
CHECK_OFFSET(cGameLoadState, unk_045, 0x045);
CHECK_OFFSET(cGameLoadState, mpParentModule, 0x048);
CHECK_SIZE(cGameLoadState, 0x4C);

// ---------------------------------------------------------------------------------------------------------------------
// cGameLoadStateOutLodge: cGameLoadScreen's load state when the game leaves the lodge (no race pending:
// gInLodge set, or game modes 5 / 6 off the online path): "118loadoutlodge" and the load percentage. The class name:
// the allocation tag "cGameLoadStateOutLodge" in cGameLoadScreen_load and the preview's _vt$22cGameLoadStateOutLodge
// with linkonce _$_22cGameLoadStateOutLodge, onCreateScreen__22cGameLoadStateOutLodge and onWidgetCreate__22...P8cUIThing.
// Retail code: the dtor, onCreateScreen, onWidgetCreate (empty) and onGainTransition in main/ssxappdtor (0x00245600 ...
// 0x00245698); there is no out-of-line ctor. No On Tour class.
//
// Size 0x48: [R] cGameLoadScreen_load: operator new(0x48, "cGameLoadStateOutLodge") + cUIState_cUIState(mem, mpUI) +
// vtable (no own members).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable D_0047C878 (24 slots: 1 dtor, 4 onCreateScreen,
// 7 onGainTransition, 18 onWidgetCreate).
// A FLAT layout, like main/game.h. Evidence tags as for cGameLoadState.

struct cGameLoadStateOutLodge {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_0047C878 (cGameLoadScreen_load, dtor cGameLoadStateOutLodge__cGameLoadStateOutLodge)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI; [U] onCreateScreen adds "118loadoutlodge"
                                    //       through it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; onGainTransition looks "%" up on
                                    //       it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cGameLoadStateOutLodge, mpPrev, 0x000);
CHECK_OFFSET(cGameLoadStateOutLodge, mpNext, 0x004);
CHECK_OFFSET(cGameLoadStateOutLodge, mVtbl, 0x008);
CHECK_OFFSET(cGameLoadStateOutLodge, mHashName, 0x00C);
CHECK_OFFSET(cGameLoadStateOutLodge, mpEngine, 0x010);
CHECK_OFFSET(cGameLoadStateOutLodge, mCurrentCtrl, 0x014);
CHECK_OFFSET(cGameLoadStateOutLodge, mCtrlBits, 0x015);
CHECK_OFFSET(cGameLoadStateOutLodge, unk_016, 0x016);
CHECK_OFFSET(cGameLoadStateOutLodge, miMiscData, 0x018);
CHECK_OFFSET(cGameLoadStateOutLodge, mAttrState, 0x01C);
CHECK_OFFSET(cGameLoadStateOutLodge, mpParentState, 0x020);
CHECK_OFFSET(cGameLoadStateOutLodge, mScreens, 0x024);
CHECK_OFFSET(cGameLoadStateOutLodge, mpScreen, 0x040);
CHECK_OFFSET(cGameLoadStateOutLodge, mUserNum, 0x044);
CHECK_OFFSET(cGameLoadStateOutLodge, unk_045, 0x045);
CHECK_SIZE(cGameLoadStateOutLodge, 0x48);

// ---------------------------------------------------------------------------------------------------------------------
// cGameLoadStateConquer: cGameLoadScreen's load state on the career (Conquer the Mountain) path: cGameLoadScreen_load
// picks it when the race DB's online flag (+0x49) is clear and the game mode (+0x48) is neither 5 nor 6. The class name:
// the retail allocation tag "cGameLoadStateConquer" (str_cGameLoadStateConquer in cGameLoadScreen_load). Retail code: the
// dtor func_00245708, onCreateScreen func_00245730, onGainTransition func_002458B8 and onWidgetCreate func_002458B0 in
// main/ssxappdtor; there is no out-of-line ctor (cGameLoadScreen_load also clears D_004A19DC). No On Tour class, no
// preview symbol.
//
// Size 0x48: [R] cGameLoadScreen_load: operator new(0x48, "cGameLoadStateConquer") + cUIState_cUIState(mem, mpUI) +
// vtable (no own members, like cGameLoadStateOutLodge).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable D_0047C7A8 (24 slots: 1 dtor func_00245708,
// 4 onCreateScreen func_00245730, 7 onGainTransition func_002458B8, 18 onWidgetCreate func_002458B0; builds/x/vtables).
// A FLAT layout, like main/game.h. Evidence tags as for cGameLoadState.

struct cGameLoadStateConquer {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_0047C7A8 (cGameLoadScreen_load, dtor func_00245708)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the load screen's mpUI
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onGainTransition (func_002458B8) looks "%" up on it
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cGameLoadStateConquer, mpPrev, 0x000);
CHECK_OFFSET(cGameLoadStateConquer, mpNext, 0x004);
CHECK_OFFSET(cGameLoadStateConquer, mVtbl, 0x008);
CHECK_OFFSET(cGameLoadStateConquer, mHashName, 0x00C);
CHECK_OFFSET(cGameLoadStateConquer, mpEngine, 0x010);
CHECK_OFFSET(cGameLoadStateConquer, mCurrentCtrl, 0x014);
CHECK_OFFSET(cGameLoadStateConquer, mCtrlBits, 0x015);
CHECK_OFFSET(cGameLoadStateConquer, unk_016, 0x016);
CHECK_OFFSET(cGameLoadStateConquer, miMiscData, 0x018);
CHECK_OFFSET(cGameLoadStateConquer, mAttrState, 0x01C);
CHECK_OFFSET(cGameLoadStateConquer, mpParentState, 0x020);
CHECK_OFFSET(cGameLoadStateConquer, mScreens, 0x024);
CHECK_OFFSET(cGameLoadStateConquer, mpScreen, 0x040);
CHECK_OFFSET(cGameLoadStateConquer, mUserNum, 0x044);
CHECK_OFFSET(cGameLoadStateConquer, unk_045, 0x045);
CHECK_SIZE(cGameLoadStateConquer, 0x48);

#endif /* MAIN_LOADSCREENS_H */
