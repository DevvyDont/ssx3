#ifndef MAIN_GAMEVIEWMAN_H
#define MAIN_GAMEVIEWMAN_H

// cGameViewMan: the game's view manager - one cCamera per human player ("GameCamera", cMemMan_alloc(0x4B0)), the
// number of views on screen (1, or 2 side by side in split screen), the current view and each view's screen rectangle.
// updateViewState re-splits the screen when the game-flow state changes; updateAll / snapAll / updateStreamFocus /
// isStreamReady run over the cameras of the active views. One instance in cGame::ViewMan (main/game.h, 0x84).
// Retail code: the decomp's unit main/gamerender.cpp (0x0022E3B8 .. 0x0022E964). EA's source file is not known (On Tour:
// all_in_one_main.cpp; the preview's cGameViewMan methods, 0x00203D20 .. 0x002042F8, make no assert, and the
// preview's main/gamerender.cpp asserts are elsewhere), so the header takes the unit's directory.
// Size 0x38: [R] cGame_load does operator new(0x38, "GameViewMan", 0x20000000, 0) + cGameViewMan_cGameViewMan(p, 1 or
// cBENewRaceInterface_newRace.iNumberHumans). No base classes, no vtable (the destructor is called directly).
//
// A FLAT layout, like main/game.h. Tags: [R] cGameViewMan_cGameViewMan / cGameViewMan__cGameViewMan and the allocation;
// [U] how retail code uses the field (main/gamerender.cpp and ~30 users of SSXApp->Game->ViewMan: game.cpp, menu.cpp,
// camera.cpp, angerfx.cpp, boardwakefx.cpp, asyncsys.cpp, ...); [O] SSX On Tour's DWARF (class cGameViewMan, size 0x60:
// the same members with 4 cameras and 4 rects; retail has 2 of each, 0x38 = 0x18 + 2 * 0x10).
// No preview evidence (no accessors survive; layout_xlate has no observations).

#include "layout.h"
#include "render/2drender.h"   // tRect (On Tour tRect: X, Y, W, H)

struct cCamera;   // camera/camera.h (0x4B0 bytes)

struct cGameViewMan {
    unsigned int m_uiNumPlayers;    // 0x000 [O]; [R] ctor: the player count; [U] the ctor creates that many cameras,
                                    //       the destructor deletes them; updateViewState returns when it is 1
    cCamera* PlayerCamera[2];       // 0x004 [O] (On Tour [4]); [R] ctor: cCamera_cCamera(cMemMan_alloc(1200, "GameCamera"), i)
                                    //       for i < m_uiNumPlayers, else 0; [U] the destructor deletes them (slot 1);
                                    //       updateAll / snapAll / updateStreamFocus / isStreamReady index it; cGame_getGameView(i)
    int m_GFState;                  // 0x00C [O] eGFState; [R] ctor: 0; [U] updateViewState compares it with
                                    //       cGame::mCurrentStateID (0x214) and stores the new state
    unsigned int m_uiNumView;       // 0x010 [O]; [R] ctor: 1; [U] updateViewState: 2 in states 3-5, else 1; the update loops
                                    //       run to it; the destructor zeroes it; cGame_getNumGameView, game.cpp, menu.cpp read it
    // m_uiNumView values and the view indices (names ours): [U] updateViewState shows one full-screen view or two
    // halves in split screen; view i belongs to player i (the ctor makes PlayerCamera[i] for each player).
    enum { kOneView = 1, kTwoViews = 2 };
    enum { kPlayer1View = 0, kPlayer2View = 1 };
    unsigned int m_uiCurrentView;   // 0x014 [O]; [R] ctor: 0; [U] updateViewState zeroes it; cGame_getCurrentRenderViewNum,
                                    //       boardwakefx / angerfx / menu.cpp read it (the view being rendered)
    tRect m_ViewRect[2];            // 0x018 [O] (On Tour [4]); [R] ctor: rect 0 = {0, 0, screen width, screen height}
                                    //       (X / Y stored as integer 0); [U] updateViewState splits it into two halves;
                                    //       avalanche.cpp reads m_ViewRect[m_uiCurrentView]

    // Methods bound to their retail symbols by asm labels.
    void updateViewState() __asm__("cGameViewMan_updateViewState");
    void snapAll() __asm__("cGameViewMan_snapAll");
    void updateAll() __asm__("cGameViewMan_updateAll");
    void updateStreamFocus() __asm__("cGameViewMan_updateStreamFocus");
    bool isStreamReady() __asm__("cGameViewMan_isStreamReady");
};
CHECK_OFFSET(cGameViewMan, m_uiNumPlayers, 0x0);
CHECK_OFFSET(cGameViewMan, PlayerCamera, 0x4);
CHECK_OFFSET(cGameViewMan, m_GFState, 0xC);
CHECK_OFFSET(cGameViewMan, m_uiNumView, 0x10);
CHECK_OFFSET(cGameViewMan, m_uiCurrentView, 0x14);
CHECK_OFFSET(cGameViewMan, m_ViewRect, 0x18);
CHECK_SIZE(cGameViewMan, 0x38);

#endif /* MAIN_GAMEVIEWMAN_H */
