#ifndef BX_APPMAN_H
#define BX_APPMAN_H

// cAppMan: EA's application manager (bx/appman, bx/execman): the active and loading app modules, the execution manager,
// the frame timer (Hz, period, tick, game rate, frame rate and its damping, the 30 fps lock) and the load timer and
// load state. Methods: run / mainLoop, the timer getters, halt / checkHalt, prepause / postpause, lockThirtyFps.
//
// Size 0x60 (On Tour's; retail's constructor and destructor store the vptr at 0x5C, the last word). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// through `this` in the methods falls on a member start (tools/gen/self_offsets.py). Enums and bools are words.

#include "layout.h"
#include "bx/loadtimer.h"

struct cAppModule;
struct cExecutionMan;

struct cAppMan {
    cAppModule* ActiveModule;       // 0x000 [O]
    cAppModule* LoadingModule;      // 0x004 [O]
    cExecutionMan* ExecutionMan;    // 0x008 [O]
    int m_AIUpdatesPerRender;       // 0x00C [O]
    int TimerHz;                    // 0x010 [O]
    float TimerPeriod;              // 0x014 [O]
    int TimerTick;                  // 0x018 [O]
    int Tick;                       // 0x01C [O]
    int MaxUpdateSteps;             // 0x020 [O]
    float GameRate;                 // 0x024 [O]
    float FracTick;                 // 0x028 [O]
    float FrameRate;                // 0x02C [O]
    float FrameRateDamp;            // 0x030 [O]
    int mLock30Fps;                 // 0x034 [O] bool
    cLoadTimer LoadTimer;           // 0x038 [O]
    int LoadState;                  // 0x058 [O] volatile eLoadState
    void** mVtbl;                   // 0x05C [R] ctor / dtor store cAppMan_vtbl (g++ 2.95 puts the vptr after the
                                    //       members)
};

CHECK_OFFSET(cAppMan, ActiveModule, 0x000);
CHECK_OFFSET(cAppMan, LoadingModule, 0x004);
CHECK_OFFSET(cAppMan, ExecutionMan, 0x008);
CHECK_OFFSET(cAppMan, m_AIUpdatesPerRender, 0x00C);
CHECK_OFFSET(cAppMan, TimerHz, 0x010);
CHECK_OFFSET(cAppMan, TimerPeriod, 0x014);
CHECK_OFFSET(cAppMan, TimerTick, 0x018);
CHECK_OFFSET(cAppMan, Tick, 0x01C);
CHECK_OFFSET(cAppMan, MaxUpdateSteps, 0x020);
CHECK_OFFSET(cAppMan, GameRate, 0x024);
CHECK_OFFSET(cAppMan, FracTick, 0x028);
CHECK_OFFSET(cAppMan, FrameRate, 0x02C);
CHECK_OFFSET(cAppMan, FrameRateDamp, 0x030);
CHECK_OFFSET(cAppMan, mLock30Fps, 0x034);
CHECK_OFFSET(cAppMan, LoadTimer, 0x038);
CHECK_OFFSET(cAppMan, LoadState, 0x058);
CHECK_OFFSET(cAppMan, mVtbl, 0x05C);
CHECK_SIZE(cAppMan, 0x60);

#endif
