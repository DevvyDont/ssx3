#include "common.h"
#include "ai/ai.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("seg/seg_14B10", cAI_setAIState);
#ifdef SKIP_ASM
extern "C" void cAI_forceAIState(void *);

/**
 * Changes the AI state when the requested one differs from the current one.
 *
 * Saves CurrentAIState and CurrentAIStateHandler as the previous state and handler, then calls
 * cAI_forceAIState(), which receives @p state unchanged in its second argument register and installs the new
 * state's id, handler and name. Asking for the current state does nothing.
 *
 * @param self  the AI
 * @param state  the new AI state (tAIState)
 */
extern "C" void cAI_setAIState(cAI* self, int state) {
    int old = self->CurrentAIState;
    if (state != old) {
        self->PreviousAIState = old;
        self->PreviousAIStateHandler = (cAIStateHandler*)((int)self->CurrentAIStateHandler);
        cAI_forceAIState(self);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cAI_forceAIState);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_GameInit[];
extern char str_Freeride[];
extern char str_PreRace[];
extern char str_Countdown[];
extern char str_Race[];
extern char str_EndRace[];
extern char str_Shutdown[];

// PORT: the unit declares cAI_forceAIState(void*); defined under an asm label with its real type.
extern "C" void cAI_forceAIState_impl(char* self, int state) __asm__("cAI_forceAIState");

/**
 * Sets the AI's game-flow state with its handler and name, without any change handling.
 *
 * States: 1 GameInit, 2 Freeride, 3 PreRace, 4 Countdown, 5 Race, 6 EndRace, 7 Shutdown; anything else clears
 * all three.
 *
 * @param state  the state (On Tour tAIState)
 */
// Forces the AI into the given state: state id, its state block and its name.
extern "C" void cAI_forceAIState_impl(char* self, int state)
{
    switch (state) {
    case 1:
        *(int*)(self + 0x0) = 1;
        *(const char**)(self + 0xA0) = str_GameInit;
        *(void**)(self + 0x98) = self + 0xAC;
        break;
    case 2:
        *(int*)(self + 0x0) = 2;
        *(const char**)(self + 0xA0) = str_Freeride;
        *(void**)(self + 0x98) = self + 0xB0;
        break;
    case 3:
        *(int*)(self + 0x0) = 3;
        *(const char**)(self + 0xA0) = str_PreRace;
        *(void**)(self + 0x98) = self + 0xB4;
        break;
    case 4:
        *(int*)(self + 0x0) = 4;
        *(const char**)(self + 0xA0) = str_Countdown;
        *(void**)(self + 0x98) = self + 0xBC;
        break;
    case 5:
        *(int*)(self + 0x0) = 5;
        *(const char**)(self + 0xA0) = str_Race;
        *(void**)(self + 0x98) = self + 0xC0;
        break;
    case 6:
        *(int*)(self + 0x0) = 6;
        *(const char**)(self + 0xA0) = str_EndRace;
        *(void**)(self + 0x98) = self + 0xC4;
        break;
    case 7:
        *(int*)(self + 0x0) = 7;
        *(const char**)(self + 0xA0) = str_Shutdown;
        *(void**)(self + 0x98) = self + 0xC8;
        break;
    default:
        *(void**)(self + 0x98) = 0;
        *(const char**)(self + 0xA0) = 0;
        *(int*)(self + 0x0) = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cAI_AIStateUpdate);
#ifdef SKIP_ASM
class cAIState_3C20 {
public:
    virtual void m0(void *);
    virtual void m1(void *);
    virtual void m2(void *);
};

/**
 * Runs the AI's game-flow state: on a change, the old handler's exit and the new handler's enter, then the
 * update.
 */
extern "C" void cAI_AIStateUpdate(cAI* self) {
    if (self->PreviousAIState != self->CurrentAIState) {
        cAIState_3C20 *prev = (cAIState_3C20*)self->PreviousAIStateHandler;
        if (prev != 0)
            prev->m1(((char*)self));
        ((cAIState_3C20*)self->CurrentAIStateHandler)->m0(((char*)self));
        self->PreviousAIStateHandler = 0;
        self->PreviousAIState = self->CurrentAIState;
    }
    ((cAIState_3C20*)self->CurrentAIStateHandler)->m2(((char*)self));
}
#endif

extern "C" void func_00113CB8(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113CC0);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

extern "C" void func_00113CC0(void *stateHandler, void *aiObj) {
    cAI_setAIState((cAI*)aiObj, 2);
}
#endif

extern "C" void func_00113CE0(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113CE8);
#ifdef SKIP_ASM
extern "C" void func_00113CE8(void *stateHandler, void *aiObj) {
    (*(int *)((char*)(aiObj) + (0x14))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113CF0);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

extern "C" void func_00113CF0(void *stateHandler, void *aiObj) {
    cAI_setAIState((cAI*)aiObj, 5);
}
#endif

extern "C" void func_00113D10(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113D18);
#ifdef SKIP_ASM
extern "C" void func_00113D18(void *stateHandler, void *aiObj) {
    (*(int *)((char*)(aiObj) + (0xC))) = 0;
    (*(int *)((char*)(stateHandler) + (4))) = 0;
}
#endif

extern "C" void func_00113D28(void) {
}

extern "C" void func_00113D30(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnGainState);
#ifdef SKIP_ASM
extern "C" void cCountdownHandler_OnGainState(void *arg0, void *arg1) {
    (*(int *)((char*)(arg1) + (0x1C))) = 0xB4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

extern "C" void cCountdownHandler_OnUpdate(void *arg0, void *arg1) {
    int temp_2;

    temp_2 = (*(int *)((char*)(arg1) + (0x1C)));
    if (temp_2 <= 0) {
        cAI_setAIState((cAI*)arg1, 5);
        return;
    }
    (*(int *)((char*)(arg1) + (0x1C))) = (int) (temp_2 - 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnLoseState);
#ifdef SKIP_ASM
extern "C" void cCountdownHandler_OnLoseState(void *arg0, void *arg1) {
    (*(int *)((char*)(arg1) + (0x1C))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRaceHandler_OnGainState);
#ifdef SKIP_ASM
extern "C" void cGameComm_gameStartRace(int);

extern "C" void cRaceHandler_OnGainState(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_gameStartRace((int)cGameComm_mGameComm);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRaceHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);
extern "C" int cAI_isRaceFinished(void *);
extern "C" void cGame_setState(int, int, int, int);

extern "C" void cRaceHandler_OnUpdate(void *arg0, void *arg1) {
    (*(int *)((char*)(arg1) + (0xC))) = (int) ((*(int *)((char*)(arg1) + (0xC))) + 1);
    if (cAI_isRaceFinished(arg1) == 1) {
        cAI_setAIState((cAI*)arg1, 6);
        cGame_setState(((int)SSXApp->Game), 5, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113E18);
#ifdef SKIP_ASM
extern "C" void cGameComm_gameEnded(int);

extern "C" void func_00113E18(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_gameEnded((int)cGameComm_mGameComm);
    }
}
#endif

extern "C" void func_00113E40(void) {
}

extern "C" void func_00113E48(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cShutdownHandler_OnGainState);
#ifdef SKIP_ASM
extern "C" void cAI_shutdown(int);

/**
 * Shuts the AI down when its state machine enters the shutdown state.
 *
 * Calls cAI_shutdown() on the AI the state machine passes in.
 *
 * @param self  the shutdown state handler (unused)
 * @param ai  the AI that gains the state
 */
extern "C" void cShutdownHandler_OnGainState(int self, int ai) {
    cAI_shutdown(ai);
}
#endif

extern "C" void func_00113E70(void) {
}

extern "C" void func_00113E78(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupLean);
#ifdef SKIP_ASM
struct sV4_3E80 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_3E80(const sV4_3E80& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_3E80(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_3E80(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

extern "C" void cRider_setupLean(char *self, float arg1) {
    float r = Len_3E80(*(sV4_3E80 *)(self + 0x1E0)) / 694.538818359375f;
    if (r < 1.0f)
        arg1 = arg1 * r;
    float d = Abs_3E80(arg1 - *(float *)(self + 0x1F0)) * 12.002403259277344f;
    float m = Clamp_3E80(d, 0.10000000149011612f, 14.001209259033203f);
    int st = *(int *)(self + 0x438);
    if (st == 2 || st == 3 || st == 0xD)
        m = m * 0.7561339735984802f;
    *(float *)(self + 0x1F8) = arg1;
    *(float *)(self + 0x1F4) = m * 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupRailBalance);
#ifdef SKIP_ASM
// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_3F38(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_3F38(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

extern "C" void cRider_setupRailBalance(void *self, float arg1) {
    float d = Abs_3F38(arg1 - *(float *)((char*)self + 0x22C)) * 7.0f;
    float m = Clamp_3F38(d, 0.1f, 8.0f);
    *(float *)((char*)self + 0x234) = arg1;
    *(float *)((char*)self + 0x230) = m * 0.01666666753590107f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupCrouchBrake);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_3F88(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_3F88(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

struct sVec4_3F88 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_3F88(const sVec4_3F88& a, const sVec4_3F88& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// Steering flips when the board points backwards: |dir| within th while tilting back, or dir < -th while tilt < th.
static inline float flipBackwards_3F88(float b, char *self, float th)
{
    if (Abs_3F88(*(float *)(self + 0x214)) <= th && *(float *)(self + 0x1F0) < 0.0f)
        return -b;
    if (*(float *)(self + 0x214) < -th && *(float *)(self + 0x1F0) < th)
        return -b;
    return b;
}

extern "C" void cRider_setupCrouchBrake(char *self, float a, float b) {
    if (0.0f < a && *(float *)(self + 0x214) != 0.0f) {
        *(float *)(self + 0x21C) = 0.0f;
        *(float *)(self + 0x218) = 0.04166241362690926f;
        return;
    }
    if (0.0f < b && *(float *)(self + 0x220) != 0.0f) {
        *(float *)(self + 0x228) = 0.0f;
        *(float *)(self + 0x224) = 0.10000182688236237f;
        return;
    }
    if (0.0f < a && *(float *)(self + 0x214) != 0.0f) {
        a = 0.0f;
    }
    float d = Abs_3F88(a - *(float *)(self + 0x220));
    float v = (0.1f <= d) ? d * 0.10000182688236237f : 0.010000183247029781f;
    *(float *)(self + 0x228) = a;
    *(float *)(self + 0x224) = v;
    if (0.0f < b) {
        if (0.0f < *(float *)(self + 0x220)) {
            b = 0.0f;
        } else if (vu0Dot_3F88(*(sVec4_3F88 *)(self + 0x1E0), *(sVec4_3F88 *)(self + 0x3A0)) < 0.0f) {
            b = 0.0f;
        }
    }
    b = flipBackwards_3F88(b, self, 0.1f);
    float e = Clamp_3F88(Abs_3F88(b - *(float *)(self + 0x214)), 0.1f, 1.0f);
    *(float *)(self + 0x21C) = b;
    *(float *)(self + 0x218) = e * 0.04166241362690926f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupBoostWarp);
#ifdef SKIP_ASM
extern "C" int func_0028B180();
extern "C" void cCheatFX_setCheatInput(char *, int);
extern "C" void func_00298D90(int mgr, char* rider, int b);
extern "C" void cSSXAudio_TurboDeny(int mgr, char* rider);
extern "C" void func_002992D8(int mgr, char* rider);

extern "C" void cRider_setupBoostWarp(char* s, int a, int b)
{
    if (b) cCheatFX_setCheatInput(*(char**)(s + 0x77C) + 0xD20, 0);
    if (a) {
        if (*(float*)(s + 0x2F8) > 0.0f) {
            if (*(float*)(s + 0x2FC) == 0.0f) {
                float x = *(float*)(s + 0x2F8);
                if (x > 0.6666666865348816f) *(float*)(s + 0x2FC) = 1.0f;
                else if (x > 0.3333333432674408f) *(float*)(s + 0x2FC) = 0.625f;
                else *(float*)(s + 0x2FC) = 0.25f;
                func_00298D90(func_0028B180(), s, b);
            }
            if (*(float*)(s + 0x2E8) == 0.0f && *(int*)(s + 0x304) != 0 && *(int*)(s + 0x2F4) < 10) {
                *(float*)(s + 0x2F8) -= 0.00150679936632514f;
            }
            return;
        }
        if (b) cSSXAudio_TurboDeny(func_0028B180(), s);
    }
    if (*(float*)(s + 0x2FC) > 0.0f) func_002992D8(func_0028B180(), s);
    *(float*)(s + 0x2FC) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00114298);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float func_0031C040(float x);
extern "C" float func_0031BF60(float x);
int func_0011FE98(void* self);
extern "C" void cReplay_eventJump(int a, int b, void* c);
extern "C" void func_00309848(void* self);
extern "C" float cRiderMetrix_evTakeoff(void* self, int a, int b);
extern "C" void cRider_flagEventBoostPowerUp(void* self, int mask, float amt);
extern "C" int AITick();
extern void* cWScriptMan_TheWScriptMan;

// 4-float vector with a component constructor.
struct sV4_4298 {
    float x, y, z, w;
    sV4_4298() {}
    sV4_4298(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sV4_4298 D_004FF140_4298 __asm__("t4vect_XAxis");
extern sV4_4298 D_004FF160_4298 __asm__("t4vect_ZAxis");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_4298(const sV4_4298& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_4298(const sV4_4298& a, const sV4_4298& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_4298 Scale_4298(const sV4_4298& v, float s)
{
    sV4_4298 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_4298(sV4_4298& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar). Assembled in noreorder mode: retail
// leaves the final sqc2 out of a following branch delay slot (as btAddEq in visualfx/boardtrailfx).
static inline void DivEq_4298(sV4_4298& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        ".set push\n"
        ".set noreorder\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_4298 Add_4298(const sV4_4298& a, const sV4_4298& b)
{
    sV4_4298 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_4298(sV4_4298& a, const sV4_4298& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_4298 Sub_4298(const sV4_4298& a, const sV4_4298& b)
{
    sV4_4298 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void SubEq_4298(sV4_4298& a, const sV4_4298& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0). The first operand is "o" (offsettable): a global
// operand is then addressed through a base register (lui; addiu; lqc2 0($r)) as in retail.
static inline sV4_4298 Cross_4298(const sV4_4298& a, const sV4_4298& b)
{
    sV4_4298 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sV4_4298 Normalize_4298(const sV4_4298& v)
{
    sV4_4298 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_4298(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_4298(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

static inline sV4_4298 Up_4298(char* r) { return *(sV4_4298*)(r + 0x1C0); }
static inline int Flags_4298(char* r) { return *(short*)(r + 0x2D4); }

#define VEL_4298 (*(sV4_4298*)(self + 0x1E0))

// Applies the rider's push (speedFactor in [0, 1]; negative = no push) and the slope/heading corrections.
extern "C" void func_00114298(char* self, float speedFactor)
{
    int steered = 0;
    float speed = Len_4298(VEL_4298);
    sV4_4298 oldVel = VEL_4298;
    float push;
    if (speedFactor < 0.0f) {
        *(int*)(*(char**)(self + 0x77C) + 0x14) = -1;
        push = 0.16339834034442902f;
    } else {
        push = speedFactor * speedFactor;
        push *= speed < 972.1808471679688f ? speed * 0.357146680355072f + 361.203125f : 708.4142456054688f;
        push = Clamp_4298(push, 361.203125f, 708.4142456054688f);
    }
    float lo = func_0031C040(0.8726646900177002f);
    float hi = func_0031C040(1.2217305898666382f);
    if (func_0011FE98(self) == 4) {
        AddEq_4298(VEL_4298, Scale_4298(Up_4298(self), push));
    } else {
        float t = (*(float*)(self + 0x378) - lo) * (1.0f / (hi - lo));
        sV4_4298 dir = Normalize_4298(Add_4298(*(sV4_4298*)(self + 0x370), Scale_4298(*(sV4_4298*)(self + 0x3A0), 0.2f)));
        if (*(float*)(self + 0x3A8) < 0.0f || t < 0.0f || *(float*)(self + 0x1E8) < 0.0f) {
            AddEq_4298(VEL_4298, Scale_4298(dir, push));
        } else {
            sV4_4298 flat = Normalize_4298(sV4_4298(*(float*)(self + 0x370), *(float*)(self + 0x374), 0.0f, 0.0f));
            sV4_4298 side = Normalize_4298(Sub_4298(*(sV4_4298*)(self + 0x3A0), Scale_4298(flat, Dot_4298(*(sV4_4298*)(self + 0x3A0), flat))));
            sV4_4298 vdir = Normalize_4298(Sub_4298(VEL_4298, Scale_4298(flat, Dot_4298(VEL_4298, flat))));
            float s0 = func_0031BF60(0.0f);
            float s1 = func_0031BF60(0.349065899848938f);
            float f = t * (side.z - s0) * (1.0f / (s1 - s0));
            f = Clamp_4298(f, 0.0f, 0.949999988079071f);
            steered = 1;
            dir = Normalize_4298(Add_4298(Scale_4298(dir, 1.0f - f), Scale_4298(side, f)));
            VEL_4298 = Add_4298(Add_4298(Scale_4298(VEL_4298, 1.0f - f), Scale_4298(vdir, f * speed)), Scale_4298(Scale_4298(dir, push), 0.5f));
        }
    }
    cReplay_eventJump((int)SSXApp->Game->Replay, *(int*)(self + 0x86C), *(char**)(self + 0x790) + 0xFC);
    func_00309848(cWScriptMan_TheWScriptMan);
    cRider_flagEventBoostPowerUp(self, 1, cRiderMetrix_evTakeoff(*(void**)(self + 0x790), *(int*)(self + 0x320) != *(int*)(self + 0x324), steered));
    if (*(float*)(self + 0x378) > hi + 0.019999999552965164f) {
        sV4_4298 dv = Sub_4298(VEL_4298, oldVel);
        sV4_4298 ov = oldVel;
        ov.z = 0.0f;
        dv.z = 0.0f;
        if (Dot_4298(ov, dv) < 0.0f) {
            float a = *(float*)(self + 0x2E4) * 0.5f;
            float b = *(float*)(self + 0x2E4) * 0.699999988079071f;
            float k;
            if (a <= speed) {
                if (speed <= b)
                    k = (speed - a) / (b - a);
                else
                    k = 1.0f;
            } else {
                k = 0.0f;
            }
            SubEq_4298(VEL_4298, Scale_4298(dv, k));
        }
    }
    float slope = *(float*)(self + 0x388);
    sV4_4298 h = *(sV4_4298*)(self + 0x380);
    h.z = 0.0f;
    if (Abs_4298(h.x) > 0.0f || Abs_4298(h.y) > 0.0f)
        DivEq_4298(h, Len_4298(h));
    else
        h = D_004FF140_4298;
    *(sV4_4298*)(self + 0x3C0) = h;
    if (slope < 0.05000000074505806f && slope > -0.05000000074505806f) {
        sV4_4298 lat = Scale_4298(h, Dot_4298(VEL_4298, h) / Dot_4298(h, h));
        SubEq_4298(VEL_4298, lat);
        AddEq_4298(*(sV4_4298*)(self + 0x110), Scale_4298(h, 4.0f));
        if (!(Flags_4298(self) & 0x20))
            ScaleEq_4298(VEL_4298, 0.799975574016571f);
    }
    float len = Len_4298(VEL_4298);
    if (*(float*)(self + 0x2E4) < len)
        ScaleEq_4298(VEL_4298, *(float*)(self + 0x2E4) / len);
    if (speedFactor >= 0.0f && (*(int*)(self + 0x434) < 0xB || *(int*)(self + 0x434) > 0xD)) {
        float t = (float)(AITick() - *(int*)(*(char**)(self + 0x77C) + 0x10)) * 0.01666666753590107f;
        float k;
        if (t <= 0.5388872623443604f) {
            k = 0.6939882636070251f;
        } else {
            float u = (t - 0.5388872623443604f) * 1.2326725721359253f <? 1.0f; // PORT: g++ minimum operator
            k = u + (1.0f - u) * 0.6939882636070251f;
        }
        ScaleEq_4298(VEL_4298, k);
    }
    sV4_4298 c = Cross_4298(D_004FF160_4298, VEL_4298);
    float cl = Len_4298(c);
    if (cl > 0.0f) {
        ScaleEq_4298(c, 1.0f / cl);
        sV4_4298 side = Cross_4298(c, *(sV4_4298*)(self + 0x370));
        *(float*)(self + 0x5A4) = Dot_4298(VEL_4298, *(sV4_4298*)(self + 0x370)) * Len_4298(side);
    } else {
        *(float*)(self + 0x5A4) = 0.0f;
    }
    *(int*)(self + 0x5A8) = speedFactor > 0.0f;
}

#undef VEL_4298
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_isChangeFakieGround);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct Q_4CC0 {
    float x, y, z, w;
    Q_4CC0() {}
    Q_4CC0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct V3_4CC0 {
    float x, y, z;
    V3_4CC0() {}
    V3_4CC0(float ax, float ay, float az) { x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

static inline V3_4CC0 Vec3Of_4CC0(Q_4CC0 v)
{
    return V3_4CC0(v.x, v.y, v.z);
}

static inline Q_4CC0 Make_4CC0(const V3_4CC0& t)
{
    return Q_4CC0(t.x, t.y, t.z, 0.0f);
}

extern "C" void cRider_updateOrientationImplicit_4CC0(void* self) __asm__("cRider_updateOrientationImplicit");
extern "C" void cRider_doChangeFakie_4CC0(char* s) __asm__("cRider_doChangeFakie");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_4CC0(const Q_4CC0& a, const Q_4CC0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline Q_4CC0 QuatMul_4CC0(const Q_4CC0& a, const Q_4CC0& b)
{
    Q_4CC0 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" int cRider_isChangeFakieGround_i(char* s) __asm__("cRider_isChangeFakieGround");

extern "C" int cRider_isChangeFakieGround_i(char* s)
{
    float k = 111.11111450195312f;
    if (*(float*)(s + 0x214) != 0.0f) {
        return 0;
    }
    float d = Dot_4CC0(*(Q_4CC0*)(s + 0x1E0), *(Q_4CC0*)(s + 0x3A0));
    if (-k < d) {
        return 0;
    }
    *(Q_4CC0*)(s + 0x120) = QuatMul_4CC0(Make_4CC0(Vec3Of_4CC0(*(Q_4CC0*)(s + 0x1C0))), *(Q_4CC0*)(s + 0x120));
    cRider_updateOrientationImplicit_4CC0(s);
    cRider_doChangeFakie_4CC0(s);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_isChangeFakieAir);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRider_doChangeFakie(char* s);
extern "C" void cAirPredictor_startLaunchIntoAir(cAirPredictor* ap, t4Vector* pos, t4Vector* vel, float t);
extern float D_004A0EA8[];
extern t4Vector D_004FF130_4DB8 __asm__("t4vect_Origin");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_4DB8(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_4DB8(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_4DB8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b, b an offsettable memory operand).
static inline t4Vector SubO_4DB8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_4DB8(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline t4Vector Rot_4DB8(const t4Vector& q, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void AddEq_4DB8(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

// A vector built in place from four floats (the original used a vector type with a ctor).
struct sV4_4DB8 : t4Vector {
    sV4_4DB8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

// FAKEMATCH: variable reuse: one `char* s` holds rider->mAirPredictor and then its state word (s = (char*)*(int*)(s + 0xAC);
// compared with (char*)1 / (char*)3), so the pointer and the state share one pseudo; plain form 98.94%. Byte-identical and
// behaves like the plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only);
// replace it when an honest spelling matches.
extern "C" int cRider_isChangeFakieAir(cRider* rider)
{
    int ok = 0;
    char* s = (char*)rider->mAirPredictor;
    s = (char*)*(int*)(s + 0xAC);
    if (s == (char*)1 || s == (char*)3) {
        ok = 1;
    }
    if (!ok) {
        return 0;
    }
    char* ap = (char*)rider->mAirPredictor;
    int n = *(int*)(ap + 0x90);
    char* tbl = (char*)SSXApp->Game->SurfPropCache + n * 0xB0;
    t4Vector dir;
    t4Vector vel;
    t4Vector up = rider->unk_180;
    if (n == 0x12 || *(int*)(tbl + 0x44) != 0) {
        dir = up;
        vel = rider->mVelocity;
    } else {
        dir = *(t4Vector*)(ap + 0x20);
        vel = *(t4Vector*)(ap + 0x10);
    }
    if (Dot_4DB8(up, dir) < 0.0f) {
        return 0;
    }
    t4Vector front = rider->unk_170;
    float d = Dot_4DB8(front, rider->mVelocity);
    if (d > 277.77777099609375f) {
        return 0;
    }
    d = Dot_4DB8(front, dir);
    if (d > 0.8999999761581421f) {
        return 0;
    }
    t4Vector side = Normalize_4DB8(Sub_4DB8(front, Scale_4DB8(dir, d)));
    vel = Normalize_4DB8(Sub_4DB8(vel, Scale_4DB8(dir, Dot_4DB8(vel, dir))));
    if (Dot_4DB8(side, vel) > D_004A0EA8[0]) {
        return 0;
    }
    char* m = (char*)rider->mAnimModel;
    float* b = (float*)((rider->mBoneHips << 4) + *(int*)(m + 0x24));
    {
        const t4Vector& origin = D_004FF130_4DB8;
        t4Vector off = SubO_4DB8(sV4_4DB8(b[0] * *(float*)(m + 0x140), b[1] * *(float*)(m + 0x144),
                                         b[2] * *(float*)(m + 0x148), b[3] * *(float*)(m + 0x14C)),
                                origin);
        off.z = 0.0f;
        AddEq_4DB8(rider->mPosition, Rot_4DB8(rider->mRotation, Scale_4DB8(off, 2.0f)));
    }
    cRider_updateOrientationImplicit(rider);
    cRider_doChangeFakie((char*)rider);
    cAirPredictor_startLaunchIntoAir(rider->mAirPredictor, &rider->mPosition, &rider->mVelocity, 3333.33349609375f);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_doChangeFakie);
#ifdef SKIP_ASM
struct Q_5168 {
    float x, y, z, w;
    Q_5168() {}
    Q_5168(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);
extern Q_5168 t4vect_Origin;
extern Q_5168 t4vect_ZAxis;

static inline Q_5168 AxisAngle_5168(const Q_5168& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
    return Q_5168(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline void SetHeading_5168(char* a, float angle)
{
    *(Q_5168*)(a + 0x30) = t4vect_Origin;
    *(Q_5168*)(a + 0x40) = AxisAngle_5168(t4vect_ZAxis, -angle);
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline Q_5168 Scale_5168(const Q_5168& v, float s)
{
    Q_5168 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" void cRider_doChangeFakie(char* s)
{
    *(int*)(s + 0x320) ^= 1;
    cRiderAnimBase_changeHeadingOffset(*(void**)(s + 0x784), 3.1415927410125732f);
    if (*(int*)(s + 0x320)) {
        SetHeading_5168(*(char**)(s + 0x784), 3.1415927410125732f);
    } else {
        SetHeading_5168(*(char**)(s + 0x784), 0.0f);
    }
    *(int*)(*(char**)(s + 0x784) + 0x18) = *(int*)(s + 0x320);
    *(Q_5168*)(s + 0x3A0) = Scale_5168(*(Q_5168*)(s + 0x3A0), -1.0f);
    *(Q_5168*)(s + 0x3B0) = Scale_5168(*(Q_5168*)(s + 0x3B0), -1.0f);
    *(float*)(s + 0x1F0) = -*(float*)(s + 0x1F0);
    *(float*)(s + 0x1F8) = -*(float*)(s + 0x1F8);
    *(float*)(s + 0x214) = -*(float*)(s + 0x214);
    *(float*)(s + 0x21C) = -*(float*)(s + 0x21C);
    *(float*)(s + 0x208) = -*(float*)(s + 0x208);
    *(float*)(s + 0x210) = -*(float*)(s + 0x210);
    *(float*)(s + 0x1FC) = -*(float*)(s + 0x1FC);
    *(float*)(s + 0x204) = -*(float*)(s + 0x204);
    *(float*)(s + 0x280) = -*(float*)(s + 0x280);
    *(float*)(s + 0x288) = -*(float*)(s + 0x288);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_doRailslideModeOrientation);
#ifdef SKIP_ASM
struct V3_5358 {
    float x, y, z;
    V3_5358() {}
    V3_5358(t4Vector v) { x = v.x; y = v.y; z = v.z; }
};

struct Q_5358 {
    float x, y, z, w;
    Q_5358() {}
    Q_5358(const V3_5358& v, float aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRider_rotateOrientation_5358(void* self, Q_5168 dir, float angle) __asm__("cRider_rotateOrientation");
// PORT: the unit defines func_00116930 with no parameters; this caller passes the rider.
extern "C" void func_00116930_5358(void* self) __asm__("func_00116930");

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline t4Vector QuatMul_5358(const Q_5358& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Turns the rider to suit its rail-slide style, then refreshes its orientation.
 *
 * Styles 1 and 2 need a stance: the rider half-turns when its current stance (unk_320) disagrees. Other
 * styles
 * half-turn back when needed, then turn the rider and its animation heading a quarter turn (+90 degrees for
 * style 4, else -90).
 */
extern "C" void cRider_doRailslideModeOrientation(cRider* self)
{
    // Half turn about the up axis (quaternion (up, 0)) whenever the landing state disagrees with the stance flag.
    int state = self->mRailStyle;
    if (state == 2) {
        if (self->unk_320 == 0) {
            self->mRotation = QuatMul_5358(Q_5358(V3_5358(self->mMatrix.row[2]), 0.0f), self->mRotation);
            cRider_updateOrientationImplicit(self);
            cRider_doChangeFakie((char*)self);
        }
    } else if (state == 1) {
        if (self->unk_320 != 0) {
            self->mRotation = QuatMul_5358(Q_5358(V3_5358(self->mMatrix.row[2]), 0.0f), self->mRotation);
            cRider_updateOrientationImplicit(self);
            cRider_doChangeFakie((char*)self);
        }
    } else {
        if (self->unk_320 != 0) {
            self->mRotation = QuatMul_5358(Q_5358(V3_5358(self->mMatrix.row[2]), 0.0f), self->mRotation);
            cRider_updateOrientationImplicit(self);
            cRider_doChangeFakie((char*)self);
        }
        if (self->mRailStyle == 4) {
            cRider_rotateOrientation_5358(self, *(Q_5168*)&self->mMatrix.row[2], 1.5707964f);
            cRiderAnimBase_changeHeadingOffset(self->mRiderAnim, 1.5707964f);
            SetHeading_5168((char*)self->mRiderAnim, 1.5707964f);
        } else {
            cRider_rotateOrientation_5358(self, *(Q_5168*)&self->mMatrix.row[2], -1.5707964f);
            cRiderAnimBase_changeHeadingOffset(self->mRiderAnim, -1.5707964f);
            SetHeading_5168((char*)self->mRiderAnim, -1.5707964f);
        }
    }
    func_00116930_5358(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_undoRailslideModeOrientation);
#ifdef SKIP_ASM
struct Q_5640 {
    float x, y, z, w;
    Q_5640() {}
    Q_5640(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_rotateOrientation(void* self, Q_5640 dir, float angle);
// PORT: the unit defines func_00116930 with no parameters; this caller passes the rider.
extern "C" void func_00116930_p(void* self) __asm__("func_00116930");
int func_0011FE98(void* self);
extern Q_5640 D_004FF130_5640 __asm__("t4vect_Origin");
extern Q_5640 D_004FF160_5640 __asm__("t4vect_ZAxis");

static inline Q_5640 AxisAngle_5640(const Q_5640& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
    return Q_5640(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline void SetHeading_5640(char* a, float angle)
{
    *(Q_5640*)(a + 0x30) = D_004FF130_5640;
    *(Q_5640*)(a + 0x40) = AxisAngle_5640(D_004FF160_5640, -angle);
}

// Finishes a pending turn-around request (state 3 = left, 4 = right) of the rider.
extern "C" void cRider_undoRailslideModeOrientation(char* self)
{
    if (*(int*)(self + 0x328) == 0)
        return;
    if (func_0011FE98(self) == 1) {
        if (*(int*)(self + 0x328) == 3 || *(int*)(self + 0x328) == 4)
            SetHeading_5640(*(char**)(self + 0x784), 0.0f);
        if (*(int*)(self + 0x328) == 3)
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x11A, 0, -1.0f);
        else if (*(int*)(self + 0x328) == 4)
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x115, 0, -1.0f);
        else
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x10C, 0, -1.0f);
    } else if (func_0011FE98(self) == 0) {
        if (*(int*)(self + 0x328) == 3) {
            cRider_rotateOrientation(self, *(Q_5640*)(self + 0x1C0), 1.5707964f);
            cRiderAnimBase_changeHeadingOffset(*(void**)(self + 0x784), 1.5707964f);
            SetHeading_5640(*(char**)(self + 0x784), 0.0f);
        } else if (*(int*)(self + 0x328) == 4) {
            cRider_rotateOrientation(self, *(Q_5640*)(self + 0x1C0), -1.5707964f);
            cRiderAnimBase_changeHeadingOffset(*(void**)(self + 0x784), -1.5707964f);
            SetHeading_5640(*(char**)(self + 0x784), 0.0f);
        }
        cRiderAnimBase_play(*(void**)(self + 0x784), 5, 0, -1.0f);
    }
    *(int*)(self + 0x328) = 0;
    func_00116930_p(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_snapAxisIncrement);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);
extern "C" void func_0031BE50(float* s, float* c, float angle);

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_58B8(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Atan2_58B8(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

// PORT: PS2-only abs.s asm helper (fabsf off-PS2).
static inline float Abs_58B8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ minimum operator
static inline float Clamp_58B8(float v, float lo, float hi)
{
    return v >= lo ? v <? hi : lo;
}

extern "C" void cRider_snapAxisIncrement(void* self, float* y, float* x, float step, float eps)
{
    float px, py, len, ang, t, r;
    float sc[2];
    if (Abs_58B8(*x) <= eps) *x = 0.0f;
    if (Abs_58B8(*y) <= eps) *y = 0.0f;
    px = *x;
    py = *y;
    len = Sqrt_58B8(px * px + py * py);
    if (len == 0.0f) return;
    ang = Atan2_58B8(py, px);
    t = ang / step;
    if (ang < 0.0f) r = t + -0.5f;
    else r = t + 0.5f;
    func_0031BE50(&sc[0], &sc[1], (float)(int)r * step);
    *x = Clamp_58B8(sc[1] * len, -1.0f, 1.0f);
    *y = Clamp_58B8(sc[0] * len, -1.0f, 1.0f);
    if (__builtin_fabsf(*x) < 0.001f) *x = 0.0f;
    if (__builtin_fabsf(*y) < 0.001f) *y = 0.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_getStartGateKneeOffset);
#ifdef SKIP_ASM
extern "C" int cBE_getBE();
extern "C" int cBE_getInterface__Fv(int, int);
extern "C" float cBECharacterInterface_getScale(int, int);

/**
 * Returns how far the rider crouches at the start gate, from its character's model scale (75 - 90 x scale).
 *
 * @param self  the rider
 * @return the knee offset
 */
extern "C" float cRider_getStartGateKneeOffset(void *self) {
    return (cBECharacterInterface_getScale(cBE_getInterface__Fv(cBE_getBE(), 2), (*(int *)((char*)(self) + (0x86C)))) * -90.0f) + 75.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00115B08);
#ifdef SKIP_ASM
extern "C" int cBE_getBE();
extern "C" int cBE_getInterface__Fv(int, int);
extern "C" float cBECharacterInterface_getScale(int, int);

extern "C" float func_00115B08(void *rider) {
    return (cBECharacterInterface_getScale(cBE_getInterface__Fv(cBE_getBE(), 2), (*(int *)((char*)(rider) + (0x86C)))) * 90.0f) - 90.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForReaction);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int AITick();
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void func_0029FF80(int snd, void* rider, int side);
int func_0011FEE8(void* self);

// PORT: PS2-only abs.s asm helper (fabsf off-PS2).
static inline float vAbs_115B58(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void cRider_checkForReaction(char* self)
{
    if (*(int*)(self + 0x358) == 0) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(self + 0x784), 2) == 5) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(self + 0x784), 1) != 0) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(self + 0x784), 2) == 10) {
        return;
    }
    int d = AITick() - *(int*)(self + 0x354);
    if (d < 180) {
        if (d < 0) {
            return;
        }
        switch (*(int*)(self + 0x358)) {
        case 1:
            *(long*)(*(char**)(self + 0x784) + 0x20) = *(long*)(self + 0x8C8);
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x13B, 0, -1.0f);
            *(long*)(*(char**)(self + 0x784) + 0x20) = -1;
            func_0029FF80(func_0028B180(), self, 0);
            break;
        case 2:
            *(long*)(*(char**)(self + 0x784) + 0x20) = *(long*)(self + 0x8C8);
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x13B, 0, -1.0f);
            *(long*)(*(char**)(self + 0x784) + 0x20) = -1;
            func_0029FF80(func_0028B180(), self, 1);
            break;
        case 3:
            *(long*)(*(char**)(self + 0x784) + 0x20) = *(long*)(self + 0x8C8);
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x13B, 0, -1.0f);
            *(long*)(*(char**)(self + 0x784) + 0x20) = -1;
            break;
        case 4:
            *(long*)(*(char**)(self + 0x784) + 0x20) = *(long*)(self + 0x8C8);
            cRiderAnimBase_play(*(void**)(self + 0x784), 0x13A, 0, -1.0f);
            *(long*)(*(char**)(self + 0x784) + 0x20) = -1;
            break;
        case 5:
        case 6:
            if (vAbs_115B58(*(float*)(self + 0x1FC)) < 0.75f && func_0011FEE8(self) == 0) {
                cRiderAnimBase_play(*(void**)(self + 0x784), 0x13E, 0, -1.0f);
            }
            break;
        }
    }
    *(int*)(self + 0x358) = 0;
    *(int*)(self + 0x354) = -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForProximityAction);
#ifdef SKIP_ASM
unsigned int AIrand();

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float Floor_5D48(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

// Wraps an angle into [-pi, pi).
static inline float Wrap_5D48(float x)
{
    return x - Floor_5D48(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

static inline t4Vector GetFront_5D48(cRider* rider)
{
    return rider->mMatrix.row[1];
}

/**
 * Once a second, may make the rider glance at a nearby rider (within 1000 units) that is out of view.
 *
 * An aggressive rival more than 60 degrees off the heading, not glanced at in the last 600 ticks, gets an
 * upper-body
 * look (one for behind); otherwise any rider more than 120 degrees off gets a head turn. The second random
 * test
 * (below 100 of 100) always passes.
 */
// Plays a glance animation towards a nearby rider (sRiderPair mDistance/mAngle).
extern "C" void cRider_checkForProximityAction(cRider* self)
{
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) != 0) {
        self->unk_35C = 0.0f;
        return;
    }
    self->unk_35C += 0.01666666753590107f;
    if (self->unk_35C < 1.0f)
        return;

    float heading = Atan2_58B8(GetFront_5D48(self).y, GetFront_5D48(self).x);
    int i;

    if ((int)(AIrand() % 100) < 90) {
        float glance = 1.047197699546814f;
        float behind = 2.3561947345733643f;
        for (i = 0; i < 6; i++) {
            if (self->mRelations[i].mValid != 0 && self->mRelations[i].mAggressive != 0) {
                if (self->mRelations[i].unk_20 + 600 <= AITick()) {
                    float diff = Wrap_5D48(self->mRelations[i].mAngle - heading);
                    if (self->mRelations[i].mDistance < 1000.0f) {
                        float ad = __builtin_fabsf(diff);
                        if (ad > glance) {
                            self->mRelations[i].unk_20 = AITick();
                            // PORT: 64-bit long
                            *(long*)((char*)self->mRiderAnim + 0x20) = self->mUpperBodyBoneMask;
                            if (ad > behind) {
                                cRiderAnimBase_play(self->mRiderAnim, 0x13F, 0, -1.0f);
                            } else {
                                int left = diff < 0.0f;
                                if (left == self->unk_320)
                                    cRiderAnimBase_play(self->mRiderAnim, 0x141, 0, -1.0f);
                                else
                                    cRiderAnimBase_play(self->mRiderAnim, 0x140, 0, -1.0f);
                            }
                            *(long*)((char*)self->mRiderAnim + 0x20) = -1;
                            return;
                        }
                    }
                }
            }
        }
    }

    if ((int)(AIrand() % 100) < 100) {
        float wide = 2.094395399093628f;
        for (i = 0; i < 6; i++) {
            if (self->mRelations[i].mValid != 0) {
                float diff = Wrap_5D48(self->mRelations[i].mAngle - heading);
                if (self->mRelations[i].mDistance < 1000.0f) {
                    if (__builtin_fabsf(diff) > wide) {
                        *(long*)((char*)self->mRiderAnim + 0x20) = self->mHeadBoneMask;
                        int left = diff < 0.0f;
                        if (left == self->unk_320)
                            cRiderAnimBase_play(self->mRiderAnim, 0x13D, 0, -1.0f);
                        else
                            cRiderAnimBase_play(self->mRiderAnim, 0x13C, 0, -1.0f);
                        *(long*)((char*)self->mRiderAnim + 0x20) = -1;
                        return;
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForReset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cRiderMetrix_evAutoResetSurface(void *);
extern "C" void cRider_setMotionState_6120(void *, int) __asm__("cRider_setMotionState__FPv");
extern "C" void cRider_setControlState_6120(void *, int) __asm__("cRider_setControlState__FPv");
extern "C" int func_0028B180();
extern "C" void func_0029A220(int, void *);
extern "C" void cReplay_eventReset(int, int);

/**
 * Resets the rider when the reset input is pressed or an automatic reset is forced.
 *
 * Clears the power-up timers, records an auto-reset metric for reset kinds 1 and 4, enters the reset motion
 * state
 * (3) and tells the sound system and the replay.
 *
 * @param isResetInput  nonzero when the player pressed reset
 * @param forceAutoReset  automatic reset kind, 0 for none (On Tour eAutoReset)
 * @return 1 when the rider resets, otherwise 0
 */
extern "C" int cRider_checkForReset(cRider* self, int isResetInput, int forceAutoReset) {
    if (isResetInput != 0 || forceAutoReset != 0) {
        *(int*)&self->mPowerUpBoostTimer = 0;
        *(int*)&self->mPowerUpTrickTimer = 0;
        if (forceAutoReset == 1 || forceAutoReset == 4)
            cRiderMetrix_evAutoResetSurface(self->mMetrix);
        *(int *)((char*)self->mRiderHandler + 0x350) = (forceAutoReset != 0);
        cRider_setControlState_6120(((char*)self), 9);
        cRider_setMotionState_6120(((char*)self), 3);
        func_0029A220(func_0028B180(), ((char*)self));
        cReplay_eventReset((int)SSXApp->Game->Replay, self->mAIIndex);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForBoardPress);
#ifdef SKIP_ASM
struct V4_61D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4D dot product).
static inline float Dot4_61D0(const V4_61D0& a, const V4_61D0& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: returns a value (target keeps the state in v1 across the call)
extern "C" int cRiderAnimBase_play_61D0(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
int func_0011FE98(void* self);
// PORT: cRider_setControlState__FPv takes a second argument; bound by asm label
void cRider_setControlState_61D0(void* self, int on) __asm__("cRider_setControlState__FPv");

extern "C" int cRider_checkForBoardPress(char* s, float dir)
{
    int st;
    if (dir == 0.0f) return 0;
    if (*(int*)(s + 0x328) == 3 || *(int*)(s + 0x328) == 4) return 0;
    if (!func_0011FE98(s)) {
        if (Dot4_61D0(*(V4_61D0*)(s + 0x1E0), *(V4_61D0*)(s + 0x3A0)) < 0.0f) return 0;
    }
    if (dir > 0.0f) {
        cRiderAnimBase_play_61D0(*(void**)(s + 0x784), 0x18, 0, -1.0f);
        *(int*)(s + 0x330) = 1;
    } else {
        cRiderAnimBase_play_61D0(*(void**)(s + 0x784), 0x20, 0, -1.0f);
        *(int*)(s + 0x330) = 2;
    }
    cRider_setControlState_61D0(s, 1);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForPrewind);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_setControlState_62C8(void *, int) __asm__("cRider_setControlState__FPv");
extern "C" int func_0011FEE8_62C8(void *) __asm__("func_0011FEE8__FPv");
extern "C" void cBoardPressControl_forceExit(char *);
extern "C" void cCheatFX_setCheatInput(char *, int);

extern "C" int cRider_checkForPrewind(char *self, int arg1, int arg2) {
    if (arg2 != 0)
        cCheatFX_setCheatInput(*(char **)(self + 0x77C) + 0xD20, 1);
    if ((*(float *)(self + 0x360) == 0.0f && arg1 != 0) || arg2 != 0) {
        if (func_0011FEE8_62C8(self) == 1)
            cBoardPressControl_forceExit(*(char **)(self + 0x77C) + 0x1D0);
        cRider_setControlState_62C8(self, 2);
        return 1;
    }
    *(float *)(self + 0x360) = 1.0f;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00116378);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_setControlState_6378(void *, int) __asm__("cRider_setControlState__FPv");

extern "C" int func_00116378(void *rider) {
    if (*(float *)((char*)rider + 0x470) >= 0.0f) {
        cRider_setControlState_6378(rider, 10);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForPunch);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void* cRiderAnimBase_getCurrentAnimSequence(void* anim, int layer);
extern "C" int func_001446A0(void* p, int i);
extern "C" int func_001446B8(void* p, int i);
extern "C" void cRiderAnimBase_blendOutAll(void* anim, int layer, float t);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
struct cAnimSequence;
void cAnimSequence_fadeWeight(cAnimSequence* seq, float a, float b);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector GetSide_163B0(cRider* rider)
{
    return rider->mMatrix.row[0];
}

static inline t4Vector Scale_163B0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline int seqDone_163B0(char* p)
{
    return func_001446A0(p, 2) && func_001446B8(p, 2);
}

/**
 * Starts or chains the rider's punch animations from the left and right punch inputs and aims the attack
 * sideways.
 *
 * One input starts a left or right punch (by stance); both start a two-handed one; repeats chain into
 * follow-up
 * punches once the current one is done. The attack blend eases toward 1 while a punch plays.
 *
 * @param isLeft  nonzero when the left punch input is down
 * @param isRight  nonzero when the right punch input is down
 * @return 1 while a punch animation is playing, otherwise 0
 */
extern "C" int cRider_checkForPunch(cRider* self, int isLeft, int isRight)
{
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 3) {
        if (isLeft != 0 && isRight != 0) {
            if (seqDone_163B0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0)) {
                *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x90) = 0.0f;
                *(long*)((char*)self->mRiderAnim + 0x20) = self->mUpperBodyBoneMask;
                cRiderAnimBase_play(self->mRiderAnim, 0x145, 0, -1.0f);
                float w = *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x94);
                float targWeight = *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x98);
                float weightTimer = *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x9C);
                char* baseSeq = (char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 0);
                *(float*)(baseSeq + 0x98) = w;
                *(float*)(baseSeq + 0x94) = w;
                if (weightTimer != 0.0f)
                    cAnimSequence_fadeWeight((cAnimSequence*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 0), targWeight, weightTimer);
                *(long*)((char*)self->mRiderAnim + 0x20) = -1;
            }
        } else {
            *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x90) = 1.0f;
            cRiderAnimBase_blendOutAll(self->mRiderAnim, 0, 0.1f);
        }
    }
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 0xD) {
        int id = cRiderAnimBase_getCurrentAnimID(self->mRiderAnim, 1);
        int match = self->unk_320 ? id == 0x143 : id == 0x144;
        if ((isLeft != 0 && isRight == 0 && match) || (isLeft == 0 && isRight != 0 && !match)) {
            if (seqDone_163B0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0)) {
                *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x90) = 0.0f;
                *(long*)((char*)self->mRiderAnim + 0x20) = self->mUpperBodyBoneMask;
                if (id == 0x143)
                    cRiderAnimBase_play(self->mRiderAnim, 0x146, 0, -1.0f);
                else
                    cRiderAnimBase_play(self->mRiderAnim, 0x147, 0, -1.0f);
                *(long*)((char*)self->mRiderAnim + 0x20) = -1;
            }
            if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 0) == 0xD) {
                float blendStep = self->mTimeScale * 0.01111111231148243f;
                float cur = self->mAttackBlend;
                float newBlend = 1.0f;
                if (blendStep + 1.0f < cur)
                    newBlend = cur - blendStep;
                else if (cur < 1.0f - blendStep)
                    newBlend = cur + blendStep;
                self->mAttackBlend = newBlend;
                char* baseSeq = (char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 0);
                *(float*)(baseSeq + 0x94) = *(float*)(baseSeq + 0x98) = self->mAttackBlend;
            }
            if (match)
                self->mAttackDir = Scale_163B0(GetSide_163B0(self), -1.0f);
            else
                self->mAttackDir = GetSide_163B0(self);
        } else {
            *(float*)((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0x90) = 1.0f;
            cRiderAnimBase_blendOutAll(self->mRiderAnim, 0, 0.1f);
        }
    }
    if (isLeft != 0 && isRight != 0) {
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 0xD && func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 0) != 0 &&
            func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1) == 0)
            return 1;
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 3 && func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1) == 0)
            return 1;
        *(long*)((char*)self->mRiderAnim + 0x20) = self->mUpperBodyBoneMask;
        cRiderAnimBase_play(self->mRiderAnim, 0x142, 1, -1.0f);
        *(long*)((char*)self->mRiderAnim + 0x20) = -1;
        self->mAttackBlend = 0.0f;
    } else if (isLeft != 0 || isRight != 0) {
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 3 && func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 0) != 0 &&
            func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1) == 0)
            return 1;
        if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 0xD && func_001446A0((char*)cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1) == 0)
            return 1;
        int anim;
        if (isLeft != 0) {
            anim = self->unk_320 ? 0x143 : 0x144;
            self->mAttackDir = Scale_163B0(GetSide_163B0(self), -1.0f);
        } else {
            self->mAttackDir = GetSide_163B0(self);
            anim = self->unk_320 ? 0x144 : 0x143;
        }
        *(long*)((char*)self->mRiderAnim + 0x20) = self->mUpperBodyBoneMask;
        cRiderAnimBase_play(self->mRiderAnim, anim, 1, -1.0f);
        *(long*)((char*)self->mRiderAnim + 0x20) = -1;
        self->mAttackBlend = 0.0f;
    }
    return cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 3 || cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) == 0xD;
}
#endif

extern "C" void func_00116930(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", func_00116938);
#ifdef SKIP_ASM
extern int D_0043D160[];

extern "C" int func_00116938(int arg0) {
    return D_0043D160[arg0];
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", gGenTrickName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int D_0043D160[];

struct sTrickDesc {
    unsigned int f0 : 3;
    unsigned int f3 : 4;
    unsigned int f7 : 3;
    unsigned int f10 : 2;
    unsigned int f12 : 4;
    unsigned int f16 : 3;
    unsigned int f19 : 3;
    unsigned int f22 : 6;
    unsigned int f28 : 4;
    unsigned int f32 : 3;
    unsigned int f35 : 7;
    unsigned int f42 : 1;
    unsigned int f43 : 7;
    unsigned int f50 : 2;
    unsigned int f52 : 4;
    unsigned int f56 : 3;
    unsigned int f59 : 5;
};

extern char* D_0043D118[];
extern char* D_0043D130[];
extern char* D_0043CF40[];
extern char* D_0043CF60[];
extern char* D_0043CF70[];
extern char* D_0043CFB0[];
extern char* D_0043CFC8[];
extern char* D_0043CFE0[];
extern char* D_0043D0C0[];
extern char* D_004A0FF0[2];
extern char* D_0043D2B8[];
extern char* D_0043D2D0[];
extern char* D_0043D2E0[];
extern char* D_0043D100[];
extern char* D_0043D320[];

// The unit later declares gGenTrickName with a different second parameter type; bind this one by symbol.
extern "C" void gGenTrickName_desc(char* out, sTrickDesc* t, int) __asm__("gGenTrickName");
/**
 * Builds a trick's display name by joining the name pieces its descriptor's fields select from the trick-name
 * tables.
 *
 * There is no check on the output's length; the third argument is unused.
 *
 * @param out  receives the name
 * @param nameDef  the trick descriptor
 */
extern "C" void gGenTrickName_desc(char* out, sTrickDesc* nameDef, int)
{
    const char* part;
    part = D_0043D118[nameDef->f0];
    while (*part) *out++ = *part++;
    part = D_0043D130[nameDef->f3];
    while (*part) *out++ = *part++;
    part = D_0043CF40[nameDef->f7];
    while (*part) *out++ = *part++;
    part = D_0043CF60[nameDef->f10];
    while (*part) *out++ = *part++;
    part = D_0043CF70[nameDef->f12];
    while (*part) *out++ = *part++;
    part = D_0043CFB0[nameDef->f16];
    while (*part) *out++ = *part++;
    part = D_0043CFC8[nameDef->f19];
    while (*part) *out++ = *part++;
    part = D_0043CFE0[nameDef->f22];
    while (*part) *out++ = *part++;
    part = D_0043D0C0[nameDef->f28];
    while (*part) *out++ = *part++;
    part = ((char**)D_0043D160)[nameDef->f35];
    while (*part) *out++ = *part++;
    part = D_004A0FF0[nameDef->f42];
    while (*part) *out++ = *part++;
    part = ((char**)D_0043D160)[nameDef->f43];
    while (*part) *out++ = *part++;
    part = D_0043D2B8[nameDef->f32];
    while (*part) *out++ = *part++;
    part = D_0043D2D0[nameDef->f50];
    while (*part) *out++ = *part++;
    part = D_0043D2E0[nameDef->f52];
    while (*part) *out++ = *part++;
    part = D_0043D100[nameDef->f56];
    while (*part) *out++ = *part++;
    part = D_0043D320[nameDef->f59];
    while (*part) *out++ = *part++;
    *out = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_14B10", gGenMonsterTrickReqName);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void MEM_fill(void* p, int c, int n);

// Packed trick id (64-bit EE `long` bitfields).
struct sTrickBits_6E10 {
    unsigned long pad0 : 10;
    unsigned long kind : 2;     // 10
    unsigned long dir : 4;      // 12
    unsigned long spin : 3;     // 16
    unsigned long flip : 3;     // 19
    unsigned long pad22 : 6;
    unsigned long grab : 4;     // 28
    unsigned long pad32 : 3;
    unsigned long mod : 7;      // 35
    unsigned long hasExtra : 1; // 42
    unsigned long extra : 7;    // 43
    unsigned long pad50 : 14;
};

union sTrickId_6E10 {
    sTrickBits_6E10 b;
    unsigned int w[2];
};

extern "C" void gGenTrickName(char* buf, sTrickId_6E10* id, int size);

struct sMonsterReq_6E10 {
    int trick;
    int pad4;
    unsigned char kind;
    unsigned char dir;
    unsigned char spin;
    unsigned char flip;
    unsigned char grab;
    unsigned char mod;
    unsigned char extra;
    unsigned char padF;
};
extern sMonsterReq_6E10 gMonsterComboTable[];

/**
 * Writes the name of the monster trick that a trick requires, or an empty string when it is not in the
 * 24-entry table.
 *
 * @param buf  receives the name
 * @param trick  the trick to look up
 * @param size  passed on to gGenTrickName (unused there)
 */
// Name of the monster trick required for `trick` (empty when it is not one).
extern "C" void gGenMonsterTrickReqName(char* buf, int trick, int size)
{
    sMonsterReq_6E10* req = gMonsterComboTable;
    for (int i = 0; i < 0x18; i++, req++) {
        if (req->trick == trick) {
            sTrickId_6E10 id;
            MEM_fill(&id, 0, 8);
            id.b.kind = req->kind;
            id.b.dir = req->dir;
            id.b.spin = req->spin;
            id.b.flip = req->flip;
            id.b.grab = req->grab;
            id.b.mod = req->mod;
            id.b.extra = req->extra;
            if (id.w[1] & 0x3F800) {  // extra != 0
                id.b.hasExtra = 1;
            }
            gGenTrickName(buf, &id, size);
            return;
        }
    }
    buf[0] = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00116FA8);
#ifdef SKIP_ASM
extern "C" int* func_00116FA8(int *arg0) {
    *arg0 = 0x34;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_update);
#ifdef SKIP_ASM
extern "C" void cHUDFX_update(void *self) {
    if (*(int *)((char*)self + 0) != 0x34) {
        float lim = *(float *)((char*)self + 4);
        if (lim != -1.0f) {
            float t = *(float *)((char*)self + 8) + 0.01666666753590107f;
            *(float *)((char*)self + 8) = t;
            if (lim <= t)
                *(int *)((char*)self + 0) = 0x34;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_copyDispStr);
#ifdef SKIP_ASM
/**
 * Copies a HUD effect's display string into it, without a length check; null gives an empty string.
 *
 * @param str  the string, or 0 (On Tour s)
 */
extern "C" void cHUDFX_copyDispStr(char *self, signed char *str) {
    signed char *src = str;
    unsigned char *dst = (unsigned char*)(self + 0x18);
    if (src != 0) {
        while (*src != 0) {
            *dst = *src;
            src++;
            dst++;
        }
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_set);
#ifdef SKIP_ASM
extern "C" void cHUDFX_copyDispStr(char *, signed char *);

/**
 * Starts a HUD effect showing a string.
 *
 * The int and float arguments are named the wrong way round: @p decayTime carries the effect's data and @p
 * fparg0
 * its decay time.
 *
 * @param id  the effect (On Tour eHUDFXID)
 * @param displayStr  the text to show
 * @param decayTime  the effect's data (On Tour int data)
 * @param fparg0  how long the effect lasts (On Tour float decayTime)
 */
extern "C" void cHUDFX_set(char *self, int id, signed char *displayStr, int decayTime, float fparg0) {
    cHUDFX_copyDispStr(self, displayStr);
    *(int *)(self + 0) = id;
    *(float *)(self + 4) = fparg0;
    *(int *)(self + 0xC) = decayTime;
    *(int *)(self + 8) = 0;
    *(int *)(self + 0x14) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_set1);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cHUDFX_copyDispStr(char *, signed char *);
extern signed char D_004A5E20[];
extern char str_pct_d[];

/**
 * Starts a HUD effect showing a number, formatted with "%d" into a shared buffer.
 *
 * @param id  the effect (On Tour eHUDFXID)
 * @param displayInt  the number to show; also stored with the effect
 * @param fparg0  how long the effect lasts (On Tour decayTime)
 * @param data  the effect's data
 */
// Variant of cHUDFX_set that names the entry from a number (formatted into a shared buffer).
extern "C" void cHUDFX_set1(char *self, int id, int displayInt, float fparg0, int data) {
    sprintf((char*)D_004A5E20, str_pct_d, displayInt);
    cHUDFX_copyDispStr(self, D_004A5E20);
    *(int *)(self + 0x14) = displayInt;
    *(int *)(self + 0) = id;
    *(float *)(self + 4) = fparg0;
    *(int *)(self + 0xC) = data;
    *(int *)(self + 8) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00117138);
#ifdef SKIP_ASM
extern "C" void cHUDFX_copyDispStr(char *, signed char *);

extern "C" void func_00117138(char *hudFx, int fxId, signed char *displayStr, int data, float fparg0) {
    cHUDFX_copyDispStr(hudFx, displayStr);
    *(int *)(hudFx + 0) = fxId;
    *(int *)(hudFx + 0xC) = data;
    *(float *)(hudFx + 8) = -fparg0;
    *(float *)(hudFx + 4) = -1.0f;
    *(int *)(hudFx + 0x14) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_001171A8);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cHUDFX_copyDispStr(char *, signed char *);
extern signed char D_004A5E20[];
extern char str_pct_d[];

// Variant of func_00117138 that names the entry from a number (formatted into a shared buffer).
extern "C" void func_001171A8(char *arg0, int arg1, int arg2, float fparg0, int arg3) {
    sprintf((char*)D_004A5E20, str_pct_d, arg2);
    cHUDFX_copyDispStr(arg0, D_004A5E20);
    *(int *)(arg0 + 0x14) = arg2;
    *(int *)(arg0 + 0) = arg1;
    *(float *)(arg0 + 8) = -fparg0;
    *(int *)(arg0 + 0xC) = arg3;
    *(float *)(arg0 + 4) = -1.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRiderMetrix_cRiderMetrix);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_reset(void *);

/**
 * Constructs a rider's metrics with cleared counters and a multiplier of 1.0 (+0x1C4), then resets them.
 *
 * @return @p self
 */
extern "C" void *cRiderMetrix_cRiderMetrix(void *self) {
    *(int *)((char*)self + 0x1AC) = 0;
    *(int *)((char*)self + 0x1B0) = 0;
    *(int *)((char*)self + 0x1B4) = 0;
    *(int *)((char*)self + 0x1C8) = 0;
    *(float *)((char*)self + 0x1C4) = 1.0f;
    cRiderMetrix_reset(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRiderMetrix__cRiderMetrix);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);
extern "C" void operator_delete__FPi(void *);

extern "C" void cRiderMetrix__cRiderMetrix(void *arg0, int arg1) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1B0)));
    if (temp_4 != 0) {
        cMemMan_free__FPv(temp_4);
    }
    if (arg1 & 1) {
        operator_delete__FPi(arg0);
    }
}
#endif
