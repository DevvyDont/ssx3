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

struct cExecutionMan;

// cAppModule: the base of every application module cAppMan runs (the front end, the game, the load screens, the debug
// FE, ...). Size 4: [O] On Tour class cAppModule (size 0x4, no data members: load / purge / loseFocus / gainFocus /
// update / render are virtual); [R] vtable cAppModule_vtbl (0x0046D970, 7 slots, slot 1 cAppModule__cAppModule) is
// stored at 0x0 by the dtor and by every module dtor chaining to the base (builds/x/vtables/vtables.tsv storers).
struct cAppModule {
    void** mVtbl;                   // 0x0 [O] the vptr (the class's only word); [R] cAppModule__cAppModule stores
                                    //      cAppModule_vtbl here
};

CHECK_OFFSET(cAppModule, mVtbl, 0x0);
CHECK_SIZE(cAppModule, 0x4);

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

    // Methods bound to their retail symbols by asm labels.
    int checkHalt() __asm__("cAppMan_checkHalt__FP7cAppMan");
    int isLoading() __asm__("cAppMan_isLoading");
    void setGameRate(float fparg0) __asm__("cAppMan_setGameRate");
    int getTimerHz() __asm__("cAppMan_getTimerHz");
    float getTimerPeriod() __asm__("cAppMan_getTimerPeriod");
    float getGameRate() __asm__("cAppMan_getGameRate");
    float getFrameRate() __asm__("cAppMan_getFrameRate");
    int getTick() __asm__("cAppMan_getTick");
    int getTimerTick() __asm__("cAppMan_getTimerTick");
    int getAIUpdatesPerRender() __asm__("cAppMan_getAIUpdatesPerRender");
    void setAIUpdatesPerRender(int value) __asm__("cAppMan_setAIUpdatesPerRender");
    void wakeup() __asm__("cAppMan_wakeup");
    void wait() __asm__("cAppMan_wait");
    unsigned int getCurrentThreadId() __asm__("cAppMan_getCurrentThreadId");
    void prepause() __asm__("cAppMan_prepause");
    void postpause() __asm__("cAppMan_postpause");
    void lockThirtyFps(int value) __asm__("cAppMan_lockThirtyFps");
    int run(char* exec, int ac, int av) __asm__("cAppMan_run");
    void setNextModule(unsigned int module) __asm__("cAppMan_setNextModule__FP7cAppManUi");
    void halt() __asm__("cAppMan_halt");
    void loadexecpurge() __asm__("cAppMan_loadexecpurge");
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

// cAppMan::TheAppMan, the one instance ([O] static cAppMan* TheAppMan; [U] every module reads the timer / tick here)
extern cAppMan* cAppMan_TheAppMan;

#endif
