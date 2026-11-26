#include "vtable.h"
#include "common.h"
#include "bx/appman.h"
#include "bx/loadtimer.h"
#include "bx/execman.h"

//100%
INCLUDE_ASM("bx/appman", cAppMan_cAppMan);
#ifdef SKIP_ASM
extern void* cAppMan_vtbl[16];

/**
 * Constructs the application manager with its defaults and makes it the global instance (cAppMan_TheAppMan, On Tour
 * TheAppMan).
 *
 * Up to 12 update steps per frame, game rate 1.0, frame-rate damping 0.9, no 30 fps lock, no modules.
 *
 * @return @p self
 */
extern "C" void* cAppMan_cAppMan(cAppMan* self)
{
    self->mVtbl = (void**)cAppMan_vtbl;
    self->MaxUpdateSteps = 0xC;
    self->GameRate = 1.0f;
    self->FrameRateDamp = 0.8999999761581421f;
    self->mLock30Fps = 0;
    self->ActiveModule = 0;
    self->LoadingModule = 0;
    self->Tick = 0;
    self->TimerTick = 0;
    *(int*)&self->FracTick = 0;
    self->FrameRate = 0.0f;
    self->m_AIUpdatesPerRender = 0;
    cAppMan_TheAppMan = self;
    return ((void*)self);
}
#endif

extern void* cAppMan_vtbl[16];
void operator_delete(int* ptr);

//99.92%
INCLUDE_ASM("bx/appman", cAppMan__cAppMan__FPvi);
#ifdef SKIP_ASM
/**
 * Destroys the application manager: restores its vtable and clears the global instance, then frees it when
 * asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void cAppMan__cAppMan(void* vself, int flags)
{
    cAppMan* self = (cAppMan*)vself;
    self->mVtbl = (void**)cAppMan_vtbl;
    cAppMan_TheAppMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/appman", cAppMan_run);
#ifdef SKIP_ASM
extern "C" void SYNCTASK_run(int a);
extern "C" void cAppMan_mainLoop(void* self);
struct cAppMan;
int cAppMan_checkHalt(cAppMan* self);

struct sVt0_6D60 { short delta; short index; int (*fn)(void*); };
struct sVt1_6D60 { short delta; short index; void (*fn)(void*, int); };

static inline int call0_6D60(char* o, int vtoff, int slot)
{
    sVt0_6D60* vt = *(sVt0_6D60**)(o + vtoff);
    return vt[slot].fn(vthis(o, vt[slot]));
}

struct sVtV_6D60 { short delta; short index; void (*fn)(void*); };

static inline void callv_6D60(char* o, int vtoff, int slot)
{
    sVtV_6D60* vt = *(sVtV_6D60**)(o + vtoff);
    vt[slot].fn(vthis(o, vt[slot]));
}

static inline void call1_6D60(char* o, int vtoff, int slot, int a)
{
    sVt1_6D60* vt = *(sVt1_6D60**)(o + vtoff);
    vt[slot].fn(vthis(o, vt[slot]), a);
}

/**
 * Runs the application: sets a 60 Hz timer, starts the app, loads the first module and runs the main loop
 * until it quits.
 *
 * Waits for the first module's load (pumping the execution manager and sync tasks), activates it, runs
 * cAppMan_mainLoop(), then shuts the app and the execution manager down.
 *
 * @param exec  the execution manager (On Tour cExecutionMan* ex)
 * @param ac  command-line argument count
 * @param av  command-line arguments (char**, passed as int)
 * @return 0
 */
int cAppMan::run(char* exec, int ac, int av)
{
    TimerPeriod = 0.01666666753590107f;
    ExecutionMan = (cExecutionMan*)exec;
    TimerHz = 60;
    call1_6D60(exec, 8, 2, 60);
    sVEntry<void(void*, int, int)>* vt = (sVEntry<void(void*, int, int)>*)mVtbl;
    vt[3].fn(vthis(this, vt[3]), ac, av);
    call1_6D60((char*)ExecutionMan, 8, 9, (int)LoadingModule);
    while (call0_6D60((char*)ExecutionMan, 8, 10) == 0) {
        callv_6D60((char*)ExecutionMan, 8, 11);
        SYNCTASK_run(0);
        callv_6D60((char*)ExecutionMan, 8, 12);
        callv_6D60((char*)ExecutionMan, 8, 4);
        checkHalt();
    }
    char* next = (char*)LoadingModule;
    LoadingModule = 0;
    ActiveModule = (cAppModule*)next;
    callv_6D60(next, 0, 5);
    LoadState = 0;
    cAppMan_mainLoop(this);
    callv_6D60(((char*)this), 92, 4);
    callv_6D60((char*)ExecutionMan, 8, 3);
    char* execMan = (char*)ExecutionMan;
    if (execMan)
        call1_6D60(execMan, 8, 1, 3);
    return 0;
}
#endif


//100%
INCLUDE_ASM("bx/appman", cAppMan_setNextModule__FP7cAppManUi);
#ifdef SKIP_ASM
/**
 * Records which module the application manager should switch to next.
 *
 * @param module  the module, stored in LoadingModule
 */
void cAppMan::setNextModule(unsigned int module)
{
    LoadingModule = (cAppModule*)module;
}
#endif

//100%
INCLUDE_ASM("bx/appman", cAppMan_mainLoop);
#ifdef SKIP_ASM
// PORT: cAppMan_checkHalt returns int; the main loop ignores it (void call here).
void cAppMan_checkHalt_v(cAppMan* self) __asm__("cAppMan_checkHalt__FP7cAppMan");
void cLoadTimer_startload(void* self, float arg);
void cLoadTimer_update(void* self);
void cLoadTimer_endload(void* self);
// PORT: func_00319D10 takes no arguments at this call site.
void func_00319D10_0() __asm__("func_00319D10__FPv");
int func_00319D18(void* self);
// PORT: func_00319D18 is called with (1, 1) here.
int func_00319D18_2(int a, int b) __asm__("func_00319D18__FPv");

struct sVtI_6F00 { short delta; short index; int (*fn)(void*); };
struct sVtV1_6F00 { short delta; short index; void (*fn)(void*, int); };

static inline int calli_6F00(char* o, int vtoff, int slot)
{
    sVtI_6F00* vt = *(sVtI_6F00**)(o + vtoff);
    return vt[slot].fn(vthis(o, vt[slot]));
}

static inline void call1_6F00(char* o, int vtoff, int slot, int a)
{
    sVtV1_6F00* vt = *(sVtV1_6F00**)(o + vtoff);
    vt[slot].fn(vthis(o, vt[slot]), a);
}

/**
 * Runs the frame loop: fixed-rate module updates, rendering, frame-rate tracking and module switching.
 *
 * Each frame runs at most MaxUpdateSteps updates while the timer has ticks (counting Tick), renders when
 * updates
 * ran (two needed with the 30 fps lock) and keeps a damped FrameRate. LoadState kLoadActive waits for the next module
 * to
 * load, kLoadFinished swaps it in, kLoadEnd shuts the active module down and returns.
 *
 * @param appMan  the application manager
 */
extern "C" void cAppMan_mainLoop(void* appMan)
{
    cAppMan* self = (cAppMan*)appMan;
    int count = 0;
    calli_6F00((char*)self, 92, 5);
    for (;;) {
        callv_6D60((char*)self->ExecutionMan, 8, 11);
        SYNCTASK_run(0);
        callv_6D60((char*)self->ExecutionMan, 8, 12);
        switch (*(volatile int*)&self->LoadState) {
        case cAppMan::kLoadActive:
            if (calli_6F00((char*)self->ExecutionMan, 8, 10)) {
                *(volatile int*)&self->LoadState = cAppMan::kLoadFinished;
                self->LoadTimer.endload();
            } else {
                self->LoadTimer.update();
            }
            break;
        case cAppMan::kLoadFinished:
            if (calli_6F00((char*)self->ActiveModule, 0, 4)) {
                cAppMan_checkHalt_v(self);
                callv_6D60((char*)self->ActiveModule, 0, 3);
                cAppMan_checkHalt_v(self);
                char* cur = (char*)self->ActiveModule;
                if (cur)
                    call1_6F00(cur, 0, 1, 3);
                func_00319D18_2(1, 1);
                func_00319D10_0();
                count = 0;
                cAppModule* next = self->LoadingModule;
                self->LoadingModule = 0;
                self->ActiveModule = next;
                callv_6D60((char*)next, 0, 5);
                cAppMan_checkHalt_v(self);
                calli_6F00((char*)self, 92, 5);
                self->FrameRate = 0.0f;
                *(volatile int*)&self->LoadState = cAppMan::kLoadIdle;
            }
            break;
        case cAppMan::kLoadEnd:
            if (calli_6F00((char*)self->ActiveModule, 0, 4)) {
                cAppMan_checkHalt_v(self);
                callv_6D60((char*)self->ActiveModule, 0, 3);
                cAppMan_checkHalt_v(self);
                char* cur = (char*)self->ActiveModule;
                if (cur)
                    call1_6F00(cur, 0, 1, 3);
                self->ActiveModule = 0;
                return;
            }
            break;
        case cAppMan::kLoadIdle:
            if (self->LoadingModule) {
                self->LoadTimer.startload(10.0f);
                call1_6F00((char*)self->ExecutionMan, 8, 9, (int)self->LoadingModule);
                *(volatile int*)&self->LoadState = cAppMan::kLoadActive;
            }
            break;
        }
        while (calli_6F00((char*)self, 92, 6)) {
            cAppMan_checkHalt_v(self);
            count++;
            if (self->MaxUpdateSteps < count) {
                count += calli_6F00((char*)self, 92, 5);
                break;
            }
            self->Tick++;
            callv_6D60((char*)self->ActiveModule, 0, 6);
            cAppMan_checkHalt_v(self);
        }
        if ((self->mLock30Fps == true && count > 1 && calli_6F00((char*)self->ActiveModule, 0, 7))
            || (self->mLock30Fps == 0 && count != 0 && calli_6F00((char*)self->ActiveModule, 0, 7))) {
            cAppMan_checkHalt_v(self);
            if (0.0f < self->FrameRate) {
                float k = self->FrameRateDamp;
                self->FrameRate = (1.0f - k) * self->GameRate * (float)self->TimerHz / (float)count
                                  + k * self->FrameRate;
            } else {
                self->FrameRate = self->GameRate * (float)self->TimerHz / (float)count;
            }
            count = 0;
        } else {
            callv_6D60((char*)self->ExecutionMan, 8, 4);
            cAppMan_checkHalt_v(self);
        }
    }
}
#endif

int cExecutionMan_checkHalt(void* self, int flag);

//100%
INCLUDE_ASM("bx/appman", cAppMan_halt);
#ifdef SKIP_ASM
extern "C" void cExecutionMan_halt(int);

/**
 * Halts through the execution manager: calls cExecutionMan_halt on the app manager's execution manager (+0x8).
 *
 * The second and third argument registers (the halt callback and its data) pass through unchanged, though the C
 * declares only the app manager. Nothing in the C calls it.
 *
 */
void cAppMan::halt()
{
    cExecutionMan_halt((int)ExecutionMan);
}
#endif

//100%
INCLUDE_ASM("bx/appman", cAppMan_checkHalt__FP7cAppMan);
#ifdef SKIP_ASM
/**
 * Asks the execution manager whether execution should halt.
 *
 * @return the result of cExecutionMan_checkHalt(ExecutionMan, 1)
 */
int cAppMan::checkHalt()
{
    return ExecutionMan->checkHalt(1);
}
#endif

//100%
INCLUDE_ASM("bx/appman", cAppMan_timerFunction__FPv);
#ifdef SKIP_ASM
class cAppSubK2 {
public:
    char pad_0x00[8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void sync();
};

class cAppTickK2 {
public:
    char pad_0x00[8];
    cAppSubK2* sub; // 0x8
    char pad_0x0C[0xC];
    int ticks; // 0x18
    char pad_0x1C[8];
    float step; // 0x24
    float accum; // 0x28
    char pad_0x2C[0x5C - 0x2C];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void timerCallback();
};

/**
 * Advances the app manager's timer by one interrupt: counts it and runs the per-tick virtual for each whole game tick.
 *
 * Increments TimerTick (+0x18), adds GameRate (+0x24) to the fractional tick FracTick (+0x28), calls the app manager's
 * virtual slot 7 once per whole tick crossed and keeps the remainder, then calls the execution manager's (+0x8) virtual
 * slot 5. cExecutionMan_timerFunction calls it on cAppMan_TheAppMan.
 *
 * @param appMan  the application manager
 * @return no meaningful value; the body sets no return value
 */
int cAppMan_timerFunction(void* appMan)
{
    cAppTickK2* self = (cAppTickK2*)appMan;
    float fracTick = self->accum;
    float gameRate = self->step;
    int wholeTicks = (int)fracTick;
    self->ticks++;
    float sum = fracTick + gameRate;
    int n = (int)sum - wholeTicks;
    self->accum = sum - (float)n;
    while (n--) {
        self->timerCallback();
    }
    self->sub->sync();
}
#endif

//100%
INCLUDE_ASM("bx/appman", cAppMan_loadexecpurge);
#ifdef SKIP_ASM
/**
 * Tells the execution manager to purge (virtual slot 3), then deletes it (slot 1, its destructor, with flags 3).
 */
void cAppMan::loadexecpurge()
{
    cExecutionMan* execMan = ExecutionMan;
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)execMan->mVtbl;
    vt[3].fn(vthis(execMan, vt[3]));
    cExecutionMan* execMan2 = ExecutionMan;
    if (execMan2 != 0)
    {
        sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)execMan2->mVtbl;
        vt2[1].fn(vthis(execMan2, vt2[1]), 3);
    }
}
#endif


extern void* cExecutionMan_vtbl[16];

//100%
INCLUDE_ASM("bx/appman", cExecutionMan_cExecutionMan__FP13cExecutionMan);
#ifdef SKIP_ASM
/**
 * Constructs the base execution manager.
 *
 * Clears the halt callback (+0x0) and its argument (+0x4) and stores the base cExecutionMan vtable (+0x8).
 * main() calls it on the "PS2ExecutionMan" object before storing the PS2 vtable.
 *
 * @return @p self
 */
cExecutionMan* cExecutionMan_cExecutionMan(cExecutionMan* self)
{
    (*(int*)&self->HaltCallback) = 0;
    (*(void**)&self->mVtbl) = cExecutionMan_vtbl;
    (*(int*)&self->HaltData) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/appman", cExecutionMan_halt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// LANDING: the unit declares `extern "C" void cExecutionMan_halt(int);` (line ~245, used by cAppMan_halt);
// retype it to (cExecutionMan*, int, int) or keep the alias below.


/**
 * Installs a halt callback and its data, then calls the execution manager's virtual slot 13 (vtable +0x68).
 *
 * The callback (+0x0) and its data (+0x4) are what cExecutionMan_checkHalt() later runs; the constructor clears both.
 *
 * @param callback  the halt callback
 * @param data  the data passed to it
 */
void cExecutionMan::halt(int callback, int data)
{
    (*(int*)&HaltData) = data;
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)((void*)mVtbl);
    (*(int*)&HaltCallback) = callback;
    vt[13].fn(vthis(this, vt[13]));
}
#endif

//100%
INCLUDE_ASM("bx/appman", cExecutionMan_checkHalt__FPvi);
#ifdef SKIP_ASM
/**
 * Runs the execution manager's halt callback, when one is set, and optionally its virtual halt.
 *
 * Calls HaltCallback with HaltData. When @p flag is set it then calls virtual slot 14, which
 * is pure in the base class; the PS2 execution manager's version (cPS2ExecutionMan_signalHalt) wakes a thread and then idles
 * forever.
 *
 * @param flag  nonzero to also call the virtual halt (cAppMan_checkHalt() passes 1, cPS2ExecutionMan_halt() passes 0)
 * @return no meaningful value; the body sets no return value
 */
int cExecutionMan::checkHalt(int flag)
{
    void (*fn)(void*) = HaltCallback;
    if (fn != 0) {
        fn(HaltData);
        if (flag != 0) {
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)mVtbl;
            short delta = vt[14].delta;
            void (*signalHalt)(void*) = vt[14].fn;
            signalHalt(vthis(this, delta));
        }
    }
}
#endif

//99.25%
INCLUDE_ASM("bx/appman", cExecutionMan_timerFunction__Fv);
#ifdef SKIP_ASM
/**
 * Runs the app manager's timer step (cAppMan_timerFunction) on cAppMan_TheAppMan.
 *
 * The PS2 execution manager's timer thread (func_0031AC08) calls it after each semaphore wait, passing itself, which is
 * ignored.
 *
 * @return no meaningful value (cAppMan_timerFunction sets none)
 */
int cExecutionMan_timerFunction(void)
{
    return cAppMan_timerFunction(cAppMan_TheAppMan);
}
#endif

//99.67%
INCLUDE_ASM("bx/appman", getTheExecutionMan__Fv);
#ifdef SKIP_ASM
/**
 * Returns the execution manager (+0x8 of cAppMan_TheAppMan).
 *
 * func_0031ABD0 uses it to reach the PS2 execution manager's semaphore.
 *
 * @return the execution manager
 */
int getTheExecutionMan(void)
{
    return (int)cAppMan_TheAppMan->ExecutionMan;
}
#endif

//100%
INCLUDE_ASM("bx/appman", cLoadTimer_startload__FPvf);
#ifdef SKIP_ASM
/**
 * Starts a load-progress timer: stores the estimated load time (+0xC) and zeroes the percentage, target, offset, scale
 * and decay.
 *
 * The previous target (+0x4) and target tick (+0x10) are left as they were. cAppMan_mainLoop starts the app manager's
 * timer (+0x38) with 10 when a module begins loading; the game and front-end loaders restart it with their own
 * estimates.
 *
 * @param rateScale  the estimated load time; cLoadTimer_setnextpercent scales the approach rate by it
 */
void cLoadTimer::startload(float rateScale)
{
    loadtime = rateScale;
    *(int*)&loadpercent = 0;
    *(int*)&target = 0;
    *(int*)&offset = 0;
    *(int*)&scale = 0;
    *(int*)&decay = 0;
}
#endif


//99.9%
INCLUDE_ASM("bx/appman", cLoadTimer_setnextpercent__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the next target percentage of a load-progress timer, from which the shown percentage eases towards it.
 *
 * Moves the old target to +0x4, stores the new one as target (+0x8) and offset (+0x14), the gap from the current
 * percentage as scale (+0x18) and the app manager's Tick as the target tick (+0x10). The decay (+0x1C) is -1.5 / (load
 * time * step * 0.01), so a larger step or estimate eases more slowly. The game loader (main/gameload) steps it from
 * 13.2 to 99.
 *
 * @param newValue  the new target percentage
 */
void cLoadTimer::setnextpercent(float newValue)
{
    float old8 = target;
    float raw = loadtime;
    float diff = newValue - old8;
    float x0 = loadpercent;
    float rate = raw * diff;
    int now = cAppMan_TheAppMan->Tick;
    x0 = x0 - newValue;
    rate = rate * 0.01f;
    lasttarget = old8;
    scale = x0;
    targettick = now;
    float result = -1.5f / rate;
    offset = newValue;
    target = newValue;
    decay = result;
}
#endif

extern "C" float expf(float);

//99.96%
INCLUDE_ASM("bx/appman", cLoadTimer_update__FPv);
#ifdef SKIP_ASM
/**
 * Updates a load-progress timer's shown percentage from the time since its last target was set.
 *
 * percentage (+0x0) = offset (+0x14) + scale (+0x18) * expf((Tick - target tick) * TimerPeriod * decay), using
 * the app manager's Tick (+0x1C) and TimerPeriod (+0x14); expf is the exponential. cAppMan_mainLoop calls it
 * each frame while a module is loading.
 */
void cLoadTimer::update()
{
    int delta = cAppMan_TheAppMan->Tick - targettick;
    float ret = expf((float)delta * cAppMan_TheAppMan->TimerPeriod * decay);
    loadpercent = offset + scale * ret;
}
#endif

//100%
INCLUDE_ASM("bx/appman", cLoadTimer_endload__FPv);
#ifdef SKIP_ASM
/**
 * Ends a load-progress timer: sets the offset and the shown percentage to 100 and the scale to 0.
 *
 * cAppMan_mainLoop calls it once the loading module reports it is done.
 */
void cLoadTimer::endload()
{
    offset = 100.0f;
    *(int*)&scale = 0;
    loadpercent = 100.0f;
}
#endif

