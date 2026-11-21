#ifndef BX_EXECMAN_H
#define BX_EXECMAN_H

// cExecutionMan / cPS2ExecutionMan: EA's execution manager (the threading layer under cAppMan, bx/appman.h): the main
// thread's sync point (wait / sync), a 60 Hz timer thread woken by an INTC handler, a loader thread that runs a
// module's
// load step (load / isLoadDone), the VU0 state kept across a pause, and halting (halt / signalHalt / waitHalt).
// main() (unit bx/ps2main) builds the one instance:
// cExecutionMan_cExecutionMan__FP13cExecutionMan(cMemMan_alloc(0x142C0,
// "PS2ExecutionMan", 0, 0)), stores the PS2 vtable cPS2ExecutionMan_vtbl at +0x8 and passes it to cAppMan_run;
// cAppMan::ExecutionMan
// (bx/appman.h, 0x008) keeps it and getTheExecutionMan__Fv returns it.
//
// Sizes: cExecutionMan 0xC ([O] On Tour's size; [R] its ctor writes 0x0-0x8, vptr last); cPS2ExecutionMan 0x142C0 ([R]
// the allocation in main(); the last member reached is the INTC handler id at 0x14290, so 0x14294-0x142BF is never
// touched by retail code). EA's source path is not in the preview's strings; retail code: cExecutionMan in units
// bx/appman
// (ctor, halt, checkHalt, timerFunction, getTheExecutionMan) and bx/execman (dtor, prepause, postpause);
// cPS2ExecutionMan
// in unit bx/memblockman 0x0031A6D8-0x0031AD00 (its dtor and getCurrentThreadId in bx/execman).
//
// On Tour has no cPS2ExecutionMan (its cPSPExecutionMan, size 0xC, lists no members), so cPS2ExecutionMan's own
// members are named from retail use only ([U]: the EE kernel call each value goes to or comes from); no [P] accessor
// exists (the preview's getCurrentThreadId__16cPS2ExecutionMan only calls GetThreadId). Bools are words.
//
// PS2 vtable cPS2ExecutionMan_vtbl (14 slots, the cExecutionMan_vtbl order): 1 cPS2ExecutionMan__cPS2ExecutionMan, 2 init, 3
// purge,
// 4 wait, 5 sync, 6 func_00320B08__FPv (getCurrentThreadId: GetThreadId), 7 setTimerHz__FPv, 8 getTimerHz__FPv (60),
// 9 load, 10 isLoadDone, 11 prepause, 12 postpause, 13 halt, 14 signalHalt. Not in it: cPS2ExecutionMan_waitHalt__FPv.

#include "layout.h"

struct cAppModule;

// The base class (On Tour class cExecutionMan, 0xC; vtable cExecutionMan_vtbl: dtor, prepause, postpause, the rest the
// shared empty stub func_00411530).
struct cExecutionMan {
    void (*HaltCallback)(void*);    // 0x0 [O]; [R] the ctor zeroes it; [U] cExecutionMan_halt stores its callback,
                                    //     cExecutionMan_checkHalt calls it with HaltData
    void* HaltData;                 // 0x4 [O]; [R] ctor 0; [U] cExecutionMan_halt stores it, checkHalt passes it
    void** mVtbl;                   // 0x8 [R] the ctor and dtor store cExecutionMan_vtbl; main() stores cPS2ExecutionMan_vtbl
};

CHECK_OFFSET(cExecutionMan, HaltCallback, 0x0);
CHECK_OFFSET(cExecutionMan, HaltData, 0x4);
CHECK_OFFSET(cExecutionMan, mVtbl, 0x8);
CHECK_SIZE(cExecutionMan, 0xC);

struct cPS2ExecutionMan {
    // --- cExecutionMan
    void (*HaltCallback)(void*);    // 0x00000 [O] cExecutionMan; see above
    void* HaltData;                 // 0x00004 [O] cExecutionMan
    void** mVtbl;                   // 0x00008 [R] main() stores cPS2ExecutionMan_vtbl, the dtor cPS2ExecutionMan__cPS2ExecutionMan
                                    //         cExecutionMan_vtbl; [U] wait / halt call slots 11 / 12 / 5 through it
    // --- cPS2ExecutionMan
    int mHaltSema;                  // 0x0000C [U] init: CreateSema; waitHalt__FPv WaitSema, signalHalt SignalSema,
                                    //         purge DeleteSema
    int mMainThreadId;              // 0x00010 [U] init: GetThreadId; halt compares the caller with it; the timer and
                                    //         loader threads WakeupThread it on exit
    int mMainThreadPriority;        // 0x00014 [U] init: ReferThreadStatus(mMainThreadId).currentPriority; the loader
                                    //         thread runs at +1, the timer thread at -1
    int mSyncSema;                  // 0x00018 [U] init: CreateSema; wait WaitSema / PollSema, sync SignalSema, purge
                                    //         DeleteSema
    int mIsWaiting;                 // 0x0001C [U] bool: init 0; wait sets 1 around WaitSema(mSyncSema); sync signals
                                    //         only while it is set
    int mQuit;                      // 0x00020 [U] bool: init 0, purge 1; the timer and loader threads exit when set
                                    //         (a per-function view declares it volatile)
    char unk_00024[0xC];            // 0x00024 untouched (aligns the stack below to 16 bytes)
    char mTimerThreadStack[0x4000]; // 0x00030 [U] init: CreateThread stack (0x4000 bytes) of the timer thread
                                    //         (func_0031AC08)
    int mTimerThreadId;             // 0x04030 [U] init: CreateThread / StartThread(id, self); purge
                                    //       Terminate/DeleteThread
    int mTimerSema;                 // 0x04034 [U] init: CreateSema (max 0x400); the INTC handler func_0031ABD0 signals
                                    //       it
                                    //         (iSignalSema), the timer thread waits on it; purge DeleteSema
    int mTimerHalted;               // 0x04038 [U] bool: init 0, halt 1; func_0031ABD0 stops signalling mTimerSema
    char unk_0403C[0x4];            // 0x0403C untouched
    char mLoadThreadStack[0x10000]; // 0x04040 [U] init: CreateThread stack (0x10000 bytes) of the loader thread
                                    //         (func_0031AC60)
    int mLoadThreadId;              // 0x14040 [U] init: CreateThread (also kept in D_004A3ECC); halt suspends it when
                                    //         called from another thread; purge Terminate/DeleteThread
    int mLoadSema;                  // 0x14044 [U] init: CreateSema (max 0x400); load SignalSema, the loader thread
                                    //       waits;
                                    //         purge DeleteSema
    cAppModule* mpLoadModule;       // 0x14048 [U] init 0; load stores its module argument, the loader thread runs its
                                    //         virtual load step and clears it; isLoadDone tests == 0; prepause /
                                    //         postpause test it
    char unk_1404C[0x34];           // 0x1404C untouched (aligns the VU0 buffer to 16 bytes)
    char mVU0State[0x210];          // 0x14080 [U] VU0 register save area: init and postpause save it (func_0031A308),
                                    //         prepause restores it (func_0031A3C0); VF1-31, ACC, MAC/status/Q/clip
    int mIntcHandlerId;             // 0x14290 [U] init: the INTC 3 handler id (func_00423A90, handler func_0031A490
                                    //       with
                                    //         func_0031ABD0); purge RemoveIntcHandler
    char unk_14294[0x2C];           // 0x14294 never accessed by retail code
};

CHECK_OFFSET(cPS2ExecutionMan, HaltCallback, 0x00000);
CHECK_OFFSET(cPS2ExecutionMan, HaltData, 0x00004);
CHECK_OFFSET(cPS2ExecutionMan, mVtbl, 0x00008);
CHECK_OFFSET(cPS2ExecutionMan, mHaltSema, 0x0000C);
CHECK_OFFSET(cPS2ExecutionMan, mMainThreadId, 0x00010);
CHECK_OFFSET(cPS2ExecutionMan, mMainThreadPriority, 0x00014);
CHECK_OFFSET(cPS2ExecutionMan, mSyncSema, 0x00018);
CHECK_OFFSET(cPS2ExecutionMan, mIsWaiting, 0x0001C);
CHECK_OFFSET(cPS2ExecutionMan, mQuit, 0x00020);
CHECK_OFFSET(cPS2ExecutionMan, unk_00024, 0x00024);
CHECK_OFFSET(cPS2ExecutionMan, mTimerThreadStack, 0x00030);
CHECK_OFFSET(cPS2ExecutionMan, mTimerThreadId, 0x04030);
CHECK_OFFSET(cPS2ExecutionMan, mTimerSema, 0x04034);
CHECK_OFFSET(cPS2ExecutionMan, mTimerHalted, 0x04038);
CHECK_OFFSET(cPS2ExecutionMan, unk_0403C, 0x0403C);
CHECK_OFFSET(cPS2ExecutionMan, mLoadThreadStack, 0x04040);
CHECK_OFFSET(cPS2ExecutionMan, mLoadThreadId, 0x14040);
CHECK_OFFSET(cPS2ExecutionMan, mLoadSema, 0x14044);
CHECK_OFFSET(cPS2ExecutionMan, mpLoadModule, 0x14048);
CHECK_OFFSET(cPS2ExecutionMan, unk_1404C, 0x1404C);
CHECK_OFFSET(cPS2ExecutionMan, mVU0State, 0x14080);
CHECK_OFFSET(cPS2ExecutionMan, mIntcHandlerId, 0x14290);
CHECK_OFFSET(cPS2ExecutionMan, unk_14294, 0x14294);
CHECK_SIZE(cPS2ExecutionMan, 0x142C0);

#endif /* BX_EXECMAN_H */
