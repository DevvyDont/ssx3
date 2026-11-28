#ifndef BX_APPMAN_H
#define BX_APPMAN_H
#pragma interface

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
// The vptr is the class's only word (0x0). The destructor is inline: every module's destructor inlines it (each stores
// cAppModule_vtbl itself); its out-of-line copy cAppModule__cAppModule (slot 1, main/all_in_one_main) is a C
// function. The virtuals are in retail's slot order (On Tour's declaration order).
struct cAppModule {
    virtual ~cAppModule() {}                                        // 1 inline (copy: cAppModule__cAppModule)
    virtual void load() = 0;                                        // 2
    virtual void purge() __asm__("cAppModule_purge");               // 3
    virtual bool loseFocus() = 0;                                   // 4
    virtual void gainFocus() __asm__("cAppModule_gainFocus");       // 5
    virtual void update() = 0;                                      // 6
    virtual bool render() = 0;                                      // 7
};

CHECK_SIZE(cAppModule, 0x4);
__asm__("_vt$10cAppModule = cAppModule_vtbl");   // g++'s name for the retail vtable

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
    // The module-switch state (LoadState). [O] On Tour's cAppMan::eLoadState, names and values; [U] mainLoop starts
    // loading the next module from kLoadIdle (-> kLoadActive), swaps it in once the execution manager's load is done
    // (kLoadFinished -> kLoadIdle), and shuts the active module down and returns in kLoadEnd.
    enum eLoadState { kLoadIdle = 0, kLoadActive = 1, kLoadFinished = 2, kLoadEnd = 3 };
    void** mVtbl;                   // 0x05C [R] ctor / dtor store cAppMan_vtbl (g++ 2.95 puts the vptr after the
                                    //       members)

    // Methods bound to their retail symbols by asm labels.
    int checkHalt() __asm__("cAppMan_checkHalt__FP7cAppMan");
    bool isLoading() __asm__("cAppMan_isLoading");
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
    int run(cExecutionMan* exec, int ac, char** av) __asm__("cAppMan_run");
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
