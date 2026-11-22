#include "common.h"
#include "bx/execman.h"
#include "input/input.h"
#include "bx/appman.h"


extern void* cExecutionMan_vtbl[16];
void operator_delete(int* ptr);

//100%
INCLUDE_ASM("bx/execman", cExecutionMan__cExecutionMan__FP13cExecutionMani);
#ifdef SKIP_ASM
/**
 * Destroys an execution manager: restores the base vtable at +0x8, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void cExecutionMan__cExecutionMan(cExecutionMan* self, int flags)
{
    (*(void**)&self->mVtbl) = cExecutionMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/execman", cExecutionMan_prepause__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing before a pause: the execution manager's prepause hook has an empty body.
 */
void cExecutionMan_prepause()
{
}
#endif

//100%
INCLUDE_ASM("bx/execman", cExecutionMan_postpause__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing after a pause: the execution manager's postpause hook has an empty body.
 */
void cExecutionMan_postpause()
{
}
#endif

//100%
INCLUDE_ASM("bx/execman", cAppMan_isLoading);
#ifdef SKIP_ASM
/**
 * Returns whether a module is loading: the loading module (+0x4) is not null.
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return 1 while a module is loading, else 0
 */
extern "C" int cAppMan_isLoading(cAppMan* appMan) {
    return ((int)appMan->LoadingModule) != 0;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_setGameRate);
#ifdef SKIP_ASM
/**
 * Sets the game rate (+0x24), the ticks per timer interrupt that cAppMan_timerFunction accumulates.
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @param fparg0  the new game rate
 */
extern "C" void cAppMan_setGameRate(cAppMan* appMan, float fparg0) {
    appMan->GameRate = fparg0;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getTimerHz);
#ifdef SKIP_ASM
/**
 * Returns the timer frequency (+0x10).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the timer rate in Hz
 */
extern "C" int cAppMan_getTimerHz(cAppMan* appMan) {
    return (appMan->TimerHz);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getTimerPeriod);
#ifdef SKIP_ASM
/**
 * Returns the timer period (+0x14).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the timer period
 */
extern "C" float cAppMan_getTimerPeriod(cAppMan* appMan) {
    return (appMan->TimerPeriod);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getGameRate);
#ifdef SKIP_ASM
/**
 * Returns the game rate (+0x24).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the game rate
 */
extern "C" float cAppMan_getGameRate(cAppMan* appMan) {
    return (appMan->GameRate);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getFrameRate);
#ifdef SKIP_ASM
/**
 * Returns the damped frame rate (+0x2C) that cAppMan_mainLoop keeps.
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the frame rate
 */
extern "C" float cAppMan_getFrameRate(cAppMan* appMan) {
    return (appMan->FrameRate);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getTick);
#ifdef SKIP_ASM
/**
 * Returns the update tick count (+0x1C).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the tick count
 */
extern "C" int cAppMan_getTick(cAppMan* appMan) {
    return (appMan->Tick);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getTimerTick);
#ifdef SKIP_ASM
/**
 * Returns the timer interrupt count (+0x18).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the timer tick count
 */
extern "C" int cAppMan_getTimerTick(cAppMan* appMan) {
    return (appMan->TimerTick);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getAIUpdatesPerRender);
#ifdef SKIP_ASM
/**
 * Returns the AI updates per render (+0xC).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the AI updates per render
 */
extern "C" int cAppMan_getAIUpdatesPerRender(cAppMan* appMan) {
    return (appMan->m_AIUpdatesPerRender);
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_setAIUpdatesPerRender);
#ifdef SKIP_ASM
/**
 * Sets the AI updates per render (+0xC).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @param value  the new count
 */
extern "C" void cAppMan_setAIUpdatesPerRender(cAppMan* appMan, int value) {
    appMan->m_AIUpdatesPerRender = value;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getActiveModule);
#ifdef SKIP_ASM
/**
 * Returns the active module (+0x0).
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return the active module
 */
extern "C" int cAppMan_getActiveModule(int *appMan) {
    return *appMan;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_wakeup);
#ifdef SKIP_ASM
class cExecObj {
public:
    int pad0;
    int pad4;
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void wait();
    virtual void sync();
    virtual unsigned int v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void prepause();
    virtual void postpause();
};

/**
 * Wakes the execution manager: calls virtual entry 5 of the app manager's execution manager (+0x8).
 *
 * Nothing in the C calls it.
 */
extern "C" void cAppMan_wakeup(cAppMan* self) {
    ((cExecObj *)self->ExecutionMan)->sync();
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_wait);
#ifdef SKIP_ASM
/**
 * Waits on the execution manager: calls virtual entry 4 of the app manager's execution manager (+0x8).
 *
 * cAppMan_mainLoop calls the same entry when a frame has nothing to render. Nothing in the C calls this copy.
 */
extern "C" void cAppMan_wait(cAppMan* self) {
    ((cExecObj *)self->ExecutionMan)->wait();
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getCurrentThreadId);
#ifdef SKIP_ASM
/**
 * Returns the current thread id from the execution manager (+0x8, virtual entry 6), or -1 without one.
 *
 * Entry 6 of the PS2 execution manager's vtable cPS2ExecutionMan_vtbl is cPS2ExecutionMan_getCurrentThreadId, which returns GetThreadId(). Nothing in
 * the C calls it.
 *
 * @return the thread id, or 0xFFFFFFFF
 */
extern "C" unsigned int cAppMan_getCurrentThreadId(cAppMan* self) {
    cExecObj *obj = (cExecObj *)self->ExecutionMan;
    if (obj != 0) {
        return obj->v5();
    }
    return 0xFFFFFFFF;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_prepause);
#ifdef SKIP_ASM
/**
 * Calls the execution manager's (+0x8) virtual entry 11, which is cExecutionMan_prepause in cExecutionMan_vtbl.
 *
 * Nothing in the C calls it.
 */
extern "C" void cAppMan_prepause(cAppMan* self) {
    ((cExecObj *)self->ExecutionMan)->prepause();
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_postpause);
#ifdef SKIP_ASM
/**
 * Calls the execution manager's (+0x8) virtual entry 12, which is cExecutionMan_postpause in cExecutionMan_vtbl.
 *
 * Nothing in the C calls it.
 */
extern "C" void cAppMan_postpause(cAppMan* self) {
    ((cExecObj *)self->ExecutionMan)->postpause();
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_getLoadTimer);
#ifdef SKIP_ASM
/**
 * Returns the address of the app manager's load-progress timer (+0x38).
 *
 * The timer functions are cLoadTimer_startload, cLoadTimer_setnextpercent, cLoadTimer_update and cLoadTimer_endload. Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @return @p appMan + 0x38
 */
extern "C" int cAppMan_getLoadTimer(int appMan) {
    return appMan + 0x38;
}
#endif
//100%
INCLUDE_ASM("bx/execman", getAppMan);
#ifdef SKIP_ASM
/**
 * Returns the application manager, cAppMan_TheAppMan.
 *
 * Nothing in the C calls it.
 *
 * @return the application manager
 */
extern "C" cAppMan* getAppMan(void) {
    return cAppMan_TheAppMan;
}
#endif
//100%
INCLUDE_ASM("bx/execman", cAppMan_lockThirtyFps);
#ifdef SKIP_ASM
/**
 * Sets the 30 fps lock flag (+0x34) that cAppMan_mainLoop reads when deciding to render.
 *
 * Nothing in the C calls it.
 *
 * @param appMan  the application manager
 * @param value  non-zero to lock to 30 fps
 */
extern "C" void cAppMan_lockThirtyFps(cAppMan* appMan, int value) {
    appMan->mLock30Fps = value;
}
#endif

//100%
INCLUDE_ASM("bx/execman", cPS2ExecutionMan__cPS2ExecutionMan);
#ifdef SKIP_ASM
/**
 * Destroys the PS2 execution manager: restores cExecutionMan_vtbl at +0x8 and frees the object when bit 0 of @p flags
 * is set.
 *
 * Entry 1 (the destructor) of the PS2 execution manager's vtable cPS2ExecutionMan_vtbl; same body as cExecutionMan__cExecutionMan.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2ExecutionMan__cPS2ExecutionMan(cPS2ExecutionMan* self, int flags)
{
    (*(void**)&self->mVtbl) = cExecutionMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* GetThreadId(void* self);

//100%
INCLUDE_ASM("bx/execman", cPS2ExecutionMan_getCurrentThreadId__FPv);
#ifdef SKIP_ASM
/**
 * Returns the current thread's id (GetThreadId).
 *
 * Entry 6 of the PS2 execution manager's vtable cPS2ExecutionMan_vtbl, the entry cAppMan_getCurrentThreadId calls.
 *
 * @return GetThreadId's result
 */
void* cPS2ExecutionMan_getCurrentThreadId(void* self)
{
    return GetThreadId(self);
}
#endif

extern "C" void* __sti__all_in_one_bx_cpp(int, int);

//100%
INCLUDE_ASM("bx/execman", func_00320B28__FPv);
#ifdef SKIP_ASM
/**
 * Runs the bx unit's static constructors: __sti__all_in_one_bx_cpp(1, 0xFFFF).
 *
 * Entry +0x80 of the global constructor table D_0043CE38. The argument is not read.
 *
 * @return no meaningful value (declared void*)
 */
void* func_00320B28(void* self)
{
    return __sti__all_in_one_bx_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__all_in_one_bx_cpp(int, int);

//100%
INCLUDE_ASM("bx/execman", func_00320B48__FPv);
#ifdef SKIP_ASM
/**
 * Runs the bx unit's static destructors: __sti__all_in_one_bx_cpp(0, 0xFFFF).
 *
 * Entry +0x20 of the global destructor table D_0043CEE4. The argument is not read.
 *
 * @return no meaningful value (declared void*)
 */
void* func_00320B48(void* self)
{
    return __sti__all_in_one_bx_cpp(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("bx/execman", memlog2);
#ifdef SKIP_ASM
/**
 * Returns the base-2 logarithm of a number, rounded up.
 *
 * Counts leading zeros by halving the search window (16, 8, 4, 2 bits) and adds one when @p x is not a power of two;
 * the result is ceil(log2(@p x)) for positive @p x.
 *
 * @param x  the value, > 0 (On Tour val)
 * @return ceil(log2(x))
 */
// Branchless ceil(log2(x)): count leading zeros by halving the search
// window (16/8/4/2 bits), then subtract one more if x isn't a power of two.
extern "C" int memlog2(int x)
{
    int n, shift, y;
    n = ((x - 0x10000) >> 31) & 16;
    y = x << n;
    shift = ((y - 0x1000000) >> 31) & 8;
    y <<= shift;
    n += shift;
    shift = ((y - 0x10000000) >> 31) & 4;
    y <<= shift;
    n += shift;
    shift = ((y - 0x40000000) >> 31) & 2;
    y <<= shift;
    n += shift;
    n += 1 + (y >> 31);
    n += -(x & (x - 1)) >> 31;
    return n ^ 31;
}
#endif

//100%
INCLUDE_ASM("bx/execman", cInput_getValue);
#ifdef SKIP_ASM
extern "C" void* cInputMap_getValue(cInputMap* map, cInputPad* pad, int state);

/**
 * Reads the analogue value of one input state from the input's map and pad.
 *
 * Calls cInputMap_getValue(Map, Pad, state). On Tour's getValue returns a float; this
 * declaration returns void*, so the value really comes back in the float return register.
 *
 * @param state  input state id (On Tour eInputStates), the index of the compiled map statement to evaluate
 * @return the state's value (a float in practice; meaningless as a pointer)
 */
extern "C" void* cInput_getValue(cInput* self, int state)
{
    return cInputMap_getValue(self->Map, self->Pad, state);
}
#endif

extern "C" void* cInputMap_getState(cInputMap* map, cInputPad* pad, int state);

//100%
INCLUDE_ASM("bx/execman", cInput_getValue1);
#ifdef SKIP_ASM
extern "C" void cInputMap_getValue1(cInputMap* map, cInputPad* pad, int state, int modifier);

/**
 * Reads an input state's analogue value with a button modifier and eight float arguments, through the input's map and
 * pad.
 *
 * Calls cInputMap_getValue1(Map, Pad, @p state, modifier); the eight floats reach it untouched in the float
 * argument registers, which the C does not declare. The value comes back in the float return register although the C
 * returns void. Nothing in the C calls it.
 *
 * @param input  the input
 * @param state  the input state id
 * @param modifier  the button modifier
 */
extern "C" void cInput_getValue1(cInput* input, int state, int modifier) {
    cInputMap_getValue1(input->Map, input->Pad, state, modifier);
}
#endif

//100%
INCLUDE_ASM("bx/execman", cInput_getState__FPvi);
#ifdef SKIP_ASM
/**
 * Tells whether one input state is active, using the input's map and pad.
 *
 * Calls cInputMap_getState(Map, Pad, state), which is true when the state's value is nonzero.
 *
 * @param state  input state id (On Tour eInputStates)
 * @return nonzero when the state is active (On Tour returns bool)
 */
void* cInput_getState(void* self, int state)
{
    cInput* input = (cInput*)self;
    return cInputMap_getState(input->Map, input->Pad, state);
}
#endif


//100%
INCLUDE_ASM("bx/execman", cInput_getState1);
#ifdef SKIP_ASM
extern "C" void cInputMap_getState1(cInputMap* map, cInputPad* pad, int state, int modifier);

/**
 * Tells whether an input state is active with a button modifier and eight float arguments, through the input's map and
 * pad.
 *
 * Calls cInputMap_getState1(Map, Pad, @p state, modifier); the eight floats reach it untouched in the float
 * argument registers, which the C does not declare. The result is left in the return register although the C returns
 * void. Nothing in the C calls it.
 *
 * @param input  the input
 * @param state  the input state id
 * @param modifier  the button modifier
 */
extern "C" void cInput_getState1(cInput* input, int state, int modifier) {
    cInputMap_getState1(input->Map, input->Pad, state, modifier);
}
#endif
