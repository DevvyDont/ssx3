#include "vtable.h"
#include "common.h"
#include "ai/rideranim.h"
#include "ai/riderhandler.h"
#include "animation/animsequencer.h"
#include "bx/math3d.h"
#include "animation/animmodel.h"
#include "ai/hudfx.h"
#include "ai/ridermetrix.h"
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
 * @param state  the new AI state (tAIState)
 */
void cAI::setAIState(int state)
{
    int old = CurrentAIState;
    if (state != old) {
        PreviousAIState = old;
        PreviousAIStateHandler = CurrentAIStateHandler;
        cAI_forceAIState(this);
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

/**
 * Sets the AI's game-flow state with its handler and name, without any change handling.
 *
 * States: 1 GameInit, 2 Freeride, 3 PreRace, 4 Countdown, 5 Race, 6 EndRace, 7 Shutdown; anything else clears
 * all three.
 *
 * @param state  the state (On Tour tAIState)
 */
// Forces the AI into the given state: state id, its state block and its name.
void cAI::forceAIState(int state)
{
    switch (state) {
    case 1:
        CurrentAIState = 1;
        AIStateName = str_GameInit;
        CurrentAIStateHandler = (cAIStateHandler*)(&mGameInitHandler);
        break;
    case 2:
        CurrentAIState = 2;
        AIStateName = str_Freeride;
        CurrentAIStateHandler = (cAIStateHandler*)(&mFreerideHandler);
        break;
    case 3:
        CurrentAIState = 3;
        AIStateName = str_PreRace;
        CurrentAIStateHandler = (cAIStateHandler*)(&mPreRaceHandler);
        break;
    case 4:
        CurrentAIState = 4;
        AIStateName = str_Countdown;
        CurrentAIStateHandler = (cAIStateHandler*)(&mCountdownHandler);
        break;
    case 5:
        CurrentAIState = 5;
        AIStateName = str_Race;
        CurrentAIStateHandler = (cAIStateHandler*)(&mRaceHandler);
        break;
    case 6:
        CurrentAIState = 6;
        AIStateName = str_EndRace;
        CurrentAIStateHandler = (cAIStateHandler*)(&mEndRaceHandler);
        break;
    case 7:
        CurrentAIState = 7;
        AIStateName = str_Shutdown;
        CurrentAIStateHandler = (cAIStateHandler*)(&mShutdownHandler);
        break;
    default:
        CurrentAIStateHandler = 0;
        AIStateName = 0;
        CurrentAIState = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cAI_AIStateUpdate);
#ifdef SKIP_ASM
/**
 * Runs the AI's game-flow state: on a change, the old handler's exit and the new handler's enter, then the
 * update (the handler's vtable slots 2, 1 and 3: OnLoseState, OnGainState, OnUpdate).
 */
void cAI::AIStateUpdate()
{
    if (PreviousAIState != CurrentAIState) {
        cAIStateHandler* prev = PreviousAIStateHandler;
        if (prev != 0) {
            sVEntry<void(void*, cAI*)>* vtable = (sVEntry<void(void*, cAI*)>*)prev->mVtbl;
            vtable[2].fn(vthis(prev, vtable[2]), this);
        }
        sVEntry<void(void*, cAI*)>* vtable = (sVEntry<void(void*, cAI*)>*)CurrentAIStateHandler->mVtbl;
        vtable[1].fn(vthis(CurrentAIStateHandler, vtable[1]), this);
        PreviousAIStateHandler = 0;
        PreviousAIState = CurrentAIState;
    }
    sVEntry<void(void*, cAI*)>* vtable = (sVEntry<void(void*, cAI*)>*)CurrentAIStateHandler->mVtbl;
    vtable[3].fn(vthis(CurrentAIStateHandler, vtable[3]), this);
}
#endif

/**
 * Does nothing: the game-init state handler's enter step (slot 1, word 3, of its vtable cGameInitHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it with the handler and the AI when the AI enters the game-init state.
 */
extern "C" void cGameInitHandler_OnGainState(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cGameInitHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

/**
 * The game-init state handler's update step: moves the AI on to state 2 (cAI_setAIState(ai, 2)).
 *
 * Slot 3 (word 7) of the handler's vtable cGameInitHandler_vtbl; cAI_AIStateUpdate runs it every AI tick.
 *
 * @param stateHandler  the handler (unused)
 * @param aiObj  the AI
 */
extern "C" void cGameInitHandler_OnUpdate(void *stateHandler, cAI* aiObj) {
    aiObj->setAIState(2);
}
#endif

/**
 * Does nothing: the game-init state handler's exit step (slot 2, word 5, of its vtable cGameInitHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it with the handler and the AI when the AI leaves the game-init state.
 */
extern "C" void cGameInitHandler_OnLoseState(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cFreerideHandler_OnGainState);
#ifdef SKIP_ASM
/**
 * The freeride state handler's enter step: clears the AI's ComputerRidersActive (+0x14).
 *
 * Slot 1 (word 3) of the handler's vtable cFreerideHandler_vtbl; cAI_AIStateUpdate calls it when the AI enters the state.
 *
 * @param stateHandler  the handler (unused)
 * @param aiObj  the AI
 */
extern "C" void cFreerideHandler_OnGainState(void *stateHandler, cAI* aiObj) {
    aiObj->ComputerRidersActive = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cFreerideHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

/**
 * The freeride state handler's update step: moves the AI on to state 5, the race state (cAI_setAIState(ai, 5)).
 *
 * Slot 3 (word 7) of the handler's vtable cFreerideHandler_vtbl; cAI_AIStateUpdate runs it every AI tick.
 *
 * @param stateHandler  the handler (unused)
 * @param aiObj  the AI
 */
extern "C" void cFreerideHandler_OnUpdate(void *stateHandler, cAI* aiObj) {
    aiObj->setAIState(5);
}
#endif

/**
 * Does nothing: the freeride state handler's exit step (slot 2, word 5, of its vtable cFreerideHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it with the handler and the AI when the AI leaves the freeride state.
 */
extern "C" void cFreerideHandler_OnLoseState(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cPreRaceHandler_OnGainState);
#ifdef SKIP_ASM
/**
 * The pre-race state handler's enter step: clears the AI's EventTimerTick (+0xC) and the handler's word +4.
 *
 * Slot 1 (word 3) of the handler's vtable cPreRaceHandler_vtbl; cAI_AIStateUpdate calls it when the AI enters the state.
 *
 * @param aiObj  the AI
 */
void cPreRaceHandler::OnGainState(cAI* aiObj)
{
    aiObj->EventTimerTick = 0;
    unk_04 = 0;
}
#endif

/**
 * Does nothing: the pre-race state handler's update step (slot 3, word 7, of its vtable cPreRaceHandler_vtbl).
 *
 * cAI_AIStateUpdate runs it every AI tick in the pre-race state.
 */
extern "C" void cPreRaceHandler_OnUpdate(void) {
}

/**
 * Does nothing: the pre-race state handler's exit step (slot 2, word 5, of its vtable cPreRaceHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it when the AI leaves the pre-race state.
 */
extern "C" void cPreRaceHandler_OnLoseState(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnGainState);
#ifdef SKIP_ASM
/**
 * Starts the race countdown: three seconds of AI ticks.
 *
 * Sets the AI's CountdownTicksLeft (+0x1C) to 180 (3 s at 60 Hz, On Tour kUpdateHz). Slot 1 of the countdown handler's
 * vtable (cCountdownHandler_vtbl), run by cAI_AIStateUpdate when the AI enters the countdown state.
 *
 * @param handler  the handler (this; unused)
 * @param aiManager  the AI (cAI)
 */
extern "C" void cCountdownHandler_OnGainState(void *handler, cAI* aiManager) {
    aiManager->CountdownTicksLeft = 180;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);

/**
 * Ticks the race countdown and starts the race when it runs out.
 *
 * Decrements the AI's CountdownTicksLeft (+0x1C) while it is positive; at 0 it calls cAI_setAIState(ai, 5), the race
 * state. Slot 3 of the countdown handler's vtable (cCountdownHandler_vtbl), run by cAI_AIStateUpdate every AI tick.
 *
 * @param handler  the handler (this; unused)
 * @param aiManager  the AI (cAI)
 */
extern "C" void cCountdownHandler_OnUpdate(void *handler, cAI* aiManager) {
    int temp_2;

    temp_2 = (aiManager->CountdownTicksLeft);
    if (temp_2 <= 0) {
        aiManager->setAIState(5);
        return;
    }
    aiManager->CountdownTicksLeft = (int) (temp_2 - 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cCountdownHandler_OnLoseState);
#ifdef SKIP_ASM
/**
 * Ends the race countdown state by clearing the countdown.
 *
 * Sets the AI's CountdownTicksLeft (+0x1C) to 0. Slot 2 of the countdown handler's vtable (cCountdownHandler_vtbl), run by
 * cAI_AIStateUpdate when the AI leaves the countdown state.
 *
 * @param handler  the handler (this; unused)
 * @param aiManager  the AI (cAI)
 */
extern "C" void cCountdownHandler_OnLoseState(void *handler, cAI* aiManager) {
    aiManager->CountdownTicksLeft = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRaceHandler_OnGainState);
#ifdef SKIP_ASM

/**
 * Tells the online game layer that the race has started.
 *
 * Calls cGameComm_gameStartRace on cGameComm_mGameComm when an online game comm exists. Slot 1 of cRaceHandler's vtable
 * (cRaceHandler_vtbl), run by cAI_AIStateUpdate when the AI enters the race state; it
 * is called with the handler and the AI but uses neither, so the C view takes no parameters.
 */
extern "C" void cRaceHandler_OnGainState(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_mGameComm->gameStartRace();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRaceHandler_OnUpdate);
#ifdef SKIP_ASM
extern "C" void cAI_setAIState(cAI*, int);
extern "C" int cAI_isRaceFinished(void *);

/**
 * Advances the race clock and ends the race once it is finished.
 *
 * Increments the AI's EventTimerTick (+0xC). When cAI_isRaceFinished returns 1 it sets AI state 6 (end race) and the
 * game state to 5 (cGame_setState(SSXApp->Game, 5, 0, 0), On Tour cGame::setState). Slot 3 of cRaceHandler's vtable
 * (cRaceHandler_vtbl); cRaceHandler keeps the base cAIStateHandler_OnLoseState.
 *
 * @param handler  the handler (this; unused)
 * @param aiManager  the AI (cAI)
 */
extern "C" void cRaceHandler_OnUpdate(void *handler, cAI* aiManager) {
    aiManager->EventTimerTick = (int) ((aiManager->EventTimerTick) + 1);
    if (cAI_isRaceFinished(aiManager) == 1) {
        aiManager->setAIState(6);
        SSXApp->Game->setState(5, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cEndRaceHandler_OnGainState);
#ifdef SKIP_ASM

/**
 * The end-race state handler's enter step: tells the online game layer the game has ended.
 *
 * Calls cGameComm_gameEnded on cGameComm_mGameComm when an online game comm exists. Slot 1 (word 3) of the handler's
 * vtable cEndRaceHandler_vtbl; it is called with the handler and the AI but uses neither, so the C view takes no parameters.
 */
extern "C" void cEndRaceHandler_OnGainState(void) {
    if (cGameComm_mGameComm != 0) {
        cGameComm_mGameComm->gameEnded();
    }
}
#endif

/**
 * Does nothing: the end-race state handler's update step (slot 3, word 7, of its vtable cEndRaceHandler_vtbl).
 *
 * cAI_AIStateUpdate runs it every AI tick in the end-race state.
 */
extern "C" void cEndRaceHandler_OnUpdate(void) {
}

/**
 * Does nothing: the end-race state handler's exit step (slot 2, word 5, of its vtable cEndRaceHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it when the AI leaves the end-race state.
 */
extern "C" void cEndRaceHandler_OnLoseState(void) {
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

/**
 * Does nothing: the shutdown state handler's update step (slot 3, word 7, of cShutdownHandler_vtbl).
 *
 * cAI_AIStateUpdate runs it every AI tick in the shutdown state.
 */
extern "C" void cShutdownHandler_OnUpdate(void) {
}

/**
 * Does nothing: the shutdown state handler's exit step (slot 2, word 5, of cShutdownHandler_vtbl).
 *
 * cAI_AIStateUpdate calls it when the AI leaves the shutdown state.
 */
extern "C" void cShutdownHandler_OnLoseState(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupLean);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_3E80(const t4Vector& v)
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

/**
 * Sets the target lean and how fast the lean channel slews toward it.
 *
 * Below 694.5 speed (|mVelocity|) the lean is scaled by speed / 694.5. The slew rate is |lean - current| x 12.0 clamped
 * to 0.1 .. 14.0, x 0.756 on surface types 2, 3 and 0xD (mSurfaceType; On Tour kLeanRatePowderDamp), x 1/60. Writes
 * lean channel mSlewed[0]: target (+0x1F8) and rate (+0x1F4). Called by the rider controls (cCruiseControl_update,
 * cBumpControl_update, cPrewindControl_update, ...).
 *
 * @param desLean  the desired lean (the steering input)
 */
void cRider::setupLean(float desLean)
{
    float speedScale = Len_3E80(*(&mVelocity)) / 694.538818359375f;
    if (speedScale < 1.0f)
        desLean = desLean * speedScale;
    float rawRate = Abs_3E80(desLean - mSlewed[0].cur) * 12.002403259277344f;
    float leanRate = Clamp_3E80(rawRate, 0.10000000149011612f, 14.001209259033203f);
    int st = mSurfaceType;
    if (st == 2 || st == 3 || st == 13)
        leanRate = leanRate * 0.7561339735984802f;
    mSlewed[0].target = desLean;
    mSlewed[0].rate = leanRate * 0.01666666753590107f;
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

/**
 * Sets the target rail balance and how fast it slews toward it.
 *
 * The slew rate is |balance - current| x 7 clamped to 0.1 .. 8, x 1/60. Writes mSlewed[5] (+0x22C): target (+0x234) and
 * rate (+0x230). Called by cRailslideControl_update, cUberGrindControl_update and the board-press, prewind and bump
 * controls.
 *
 * @param desBal  the desired balance (the stick input)
 */
void cRider::setupRailBalance(float desBal)
{
    float rawRate = Abs_3F38(desBal - mSlewed[5].cur) * 7.0f;
    float desRate = Clamp_3F38(rawRate, 0.1f, 8.0f);
    mSlewed[5].target = desBal;
    mSlewed[5].rate = desRate * 0.01666666753590107f;
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


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_3F88(const t4Vector& a, const t4Vector& b)
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
static inline float flipBackwards_3F88(float b, char *vvself, float th)
{
    cRider* vself = (cRider*)vvself;
    cRider* self = (cRider*)vself;
    if (Abs_3F88(self->mSlewed[3].cur) <= th && self->mSlewed[0].cur < 0.0f)
        return -b;
    if (self->mSlewed[3].cur < -th && self->mSlewed[0].cur < th)
        return -b;
    return b;
}

/**
 * Sets the crouch and brake channel targets, releasing one before the other engages.
 *
 * The brake is mSlewed[3] (+0x214: current, rate, target) and the crouch mSlewed[4] (+0x220). A crouch request while
 * still braking first releases the brake (target 0 at 2.5/60), and a brake request while still crouched first releases
 * the crouch (6/60). Otherwise the crouch target is @p desCrouch at rate |delta| x 6/60 (0.6/60 below a 0.1 delta); the brake
 * request is dropped while crouched or moving backward (mVelocity . mGroundForward < 0), its sign follows the board
 * direction (current brake and lean values), and its rate is clamp(|delta|, 0.1, 1) x 2.5/60.
 *
 * @param desCrouch  the desired crouch, 0..1
 * @param desBrake  the desired brake, 0..1
 */
void cRider::setupCrouchBrake(float desCrouch, float desBrake)
{
    if (0.0f < desCrouch && mSlewed[3].cur != 0.0f) {
        mSlewed[3].target = 0.0f;
        mSlewed[3].rate = 0.04166241362690926f;
        return;
    }
    if (0.0f < desBrake && mSlewed[4].cur != 0.0f) {
        mSlewed[4].target = 0.0f;
        mSlewed[4].rate = 0.10000182688236237f;
        return;
    }
    if (0.0f < desCrouch && mSlewed[3].cur != 0.0f) {
        desCrouch = 0.0f;
    }
    float crouchDelta = Abs_3F88(desCrouch - mSlewed[4].cur);
    float crouchRate = (0.1f <= crouchDelta) ? crouchDelta * 0.10000182688236237f : 0.010000183247029781f;
    mSlewed[4].target = desCrouch;
    mSlewed[4].rate = crouchRate;
    if (0.0f < desBrake) {
        if (0.0f < mSlewed[4].cur) {
            desBrake = 0.0f;
        } else if (vu0Dot_3F88(*(&mVelocity), *(&mGroundForward)) < 0.0f) {
            desBrake = 0.0f;
        }
    }
    desBrake = flipBackwards_3F88(desBrake, ((char*)this), 0.1f);
    float brakeDelta = Clamp_3F88(Abs_3F88(desBrake - mSlewed[3].cur), 0.1f, 1.0f);
    mSlewed[3].target = desBrake;
    mSlewed[3].rate = brakeDelta * 0.04166241362690926f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_setupBoostWarp);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_bxAudio();
extern "C" void cCheatFX_setCheatInput(char *, int);
extern "C" void cSSXAudio_TurboOn(int mgr, char* rider, int b);
extern "C" void cSSXAudio_TurboDeny(int mgr, char* rider);
extern "C" void cSSXAudio_TurboOff(int mgr, char* rider);

/**
 * Handles the boost button: starts, sustains or denies a boost and burns the boost bar.
 *
 * A press sends cheat input 0 (cCheatFX_setCheatInput on the handler's +0xD20). While held with boost in the bar
 * (+0x2F8 > 0): with no boost active (mBoostStrength == 0) it sets the strength from the bar (> 2/3: 1.0, > 1/3: 0.625,
 * else 0.25) and starts the turbo sound (cSSXAudio_TurboOn, On Tour TurboOn); the bar burns 0.0015068 per tick unless
 * mPowerUpBoostTimer is running, mBoostMode is 0 or mUberLevel >= 10. A press with an empty bar plays
 * cSSXAudio_TurboDeny. In every other case an active boost is stopped (cSSXAudio_TurboOff, On Tour TurboOff) and
 * mBoostStrength cleared.
 *
 * @param isHeld  nonzero while the boost button is held
 * @param isPressed  nonzero on the frame the boost button is pressed
 */
void cRider::setupBoostWarp(int isHeld, int isPressed)
{
    if (isPressed) cCheatFX_setCheatInput((char*)&mRiderHandler->CheatFX, 0);
    if (isHeld) {
        if (BoostPowerBar > 0.0f) {
            if (mBoostStrength == 0.0f) {
                float x = BoostPowerBar;
                if (x > 0.6666666865348816f) mBoostStrength = 1.0f;
                else if (x > 0.3333333432674408f) mBoostStrength = 0.625f;
                else mBoostStrength = 0.25f;
                cSSXAudio_TurboOn(cAudioInterface_bxAudio(), ((char*)this), isPressed);
            }
            if (mPowerUpBoostTimer == 0.0f && mBoostMode != 0 && mUberLevel < 10) {
                BoostPowerBar -= 0.00150679936632514f;
            }
            return;
        }
        if (isPressed) cSSXAudio_TurboDeny(cAudioInterface_bxAudio(), ((char*)this));
    }
    if (mBoostStrength > 0.0f) cSSXAudio_TurboOff(cAudioInterface_bxAudio(), ((char*)this));
    mBoostStrength = 0.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_applyJumpPop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float fCos(float x);
extern "C" float fSin(float x);
int cRider_getMotionState(void* self);
extern "C" void cReplay_eventJump(int a, int b, void* c);
extern "C" void cWScriptMan_eventJump(void* self);
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

extern sV4_4298 t4vect_XAxis_4298 __asm__("t4vect_XAxis");
extern sV4_4298 t4vect_ZAxis_4298 __asm__("t4vect_ZAxis");

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

static inline sV4_4298 Up_4298(cRider* r) { return *(sV4_4298*)&r->mMatrix.row[2]; }
// retail reads SurfaceFlags signed (lh)
static inline int Flags_4298(cRider* r) { return *(short*)&r->SurfaceFlags; }

#define VEL_4298 (*(sV4_4298*)&this->mVelocity)

/**
 * Adds a jump's pop to the rider's mVelocity and reports the takeoff.
 *
 * A negative @p speedFactor (the resolve routines pass -1) sets the ground motion's AITickOnLoseFocus to -1 and uses a
 * fixed pop of 0.1634; otherwise the pop is speedFactor squared times a speed-dependent amount, clamped to
 * 361.2..708.4. In motion state 4 the pop goes along the up axis (mMatrix.row[2]). Otherwise it goes along
 * normalized (mGroundNormal + 0.2 * mGroundForward), unless the slope term (mGroundNormal.z between cos 50 and cos 70
 * degrees), mGroundForward.z and the vertical speed are all non-negative: then the velocity turns towards the side
 * direction (blend up to 0.95, by the side vector's z between sin 0 and sin 20 degrees) and half the pop is added. It
 * reports the jump (cReplay_eventJump, cWScriptMan_eventJump) and adds cRiderMetrix_evTakeoff's boost
 * (cRider_flagEventBoostPowerUp).
 *
 * After that: on steep ground the part of the change that opposes the old horizontal velocity is taken back, scaled in
 * between 0.5 and 0.7 of the top speed (TerminalVelocity). The flat heading of LastGroundNormal (t4vect_XAxis when
 * zero) goes to HalfPipeNormal; when the slope LastGroundNormal.z is within 0.05 of 0 the velocity loses its heading
 * part, mPosition moves 4 along it and, without surface flag 0x20 (SurfaceFlags), the velocity is scaled by 0.8. The
 * speed is capped at TerminalVelocity and, for a nonnegative factor with mGroundMaterial outside 11-13, scaled by 0.694
 * rising to 1 over the time since the ground motion gained focus. Last it stores a side-speed value in VerticalJump
 * and whether @p speedFactor is positive in DidPop.
 *
 * @param speedFactor  the jump strength, 0 to 1, or negative for a fixed small pop
 */
// Applies the rider's push (speedFactor in [0, 1]; negative = no push) and the slope/heading corrections.
void cRider::applyJumpPop(float speedFactor)
{
    int steered = 0;
    float speed = Len_4298(VEL_4298);
    sV4_4298 oldVel = VEL_4298;
    float push;
    if (speedFactor < 0.0f) {
        mRiderHandler->GroundMotion.AITickOnLoseFocus = -1;
        push = 0.16339834034442902f;
    } else {
        push = speedFactor * speedFactor;
        push *= speed < 972.1808471679688f ? speed * 0.357146680355072f + 361.203125f : 708.4142456054688f;
        push = Clamp_4298(push, 361.203125f, 708.4142456054688f);
    }
    float lo = fCos(0.8726646900177002f);
    float hi = fCos(1.2217305898666382f);
    if (cRider_getMotionState(this) == 4) {
        AddEq_4298(VEL_4298, Scale_4298(Up_4298(this), push));
    } else {
        float tParam = (mGroundNormal.z - lo) * (1.0f / (hi - lo));
        sV4_4298 dir = Normalize_4298(Add_4298(*(sV4_4298*)&mGroundNormal,
                                               Scale_4298(*(sV4_4298*)&mGroundForward, 0.2f)));
        if (mGroundForward.z < 0.0f || tParam < 0.0f || mVelocity.z < 0.0f) {
            AddEq_4298(VEL_4298, Scale_4298(dir, push));
        } else {
            sV4_4298 flat = Normalize_4298(sV4_4298(mGroundNormal.x, mGroundNormal.y, 0.0f, 0.0f));
            sV4_4298 side = Normalize_4298(
                Sub_4298(*(sV4_4298*)&mGroundForward,
                         Scale_4298(flat, Dot_4298(*(sV4_4298*)&mGroundForward, flat))));
            sV4_4298 vdir = Normalize_4298(Sub_4298(VEL_4298, Scale_4298(flat, Dot_4298(VEL_4298, flat))));
            float sinLow = fSin(0.0f);
            float sinHigh = fSin(0.349065899848938f);
            float sideBlend = tParam * (side.z - sinLow) * (1.0f / (sinHigh - sinLow));
            sideBlend = Clamp_4298(sideBlend, 0.0f, 0.949999988079071f);
            steered = 1;
            dir = Normalize_4298(Add_4298(Scale_4298(dir, 1.0f - sideBlend), Scale_4298(side, sideBlend)));
            VEL_4298 = Add_4298(Add_4298(Scale_4298(VEL_4298, 1.0f - sideBlend), Scale_4298(vdir, sideBlend * speed)), Scale_4298(Scale_4298(dir, push), 0.5f));
        }
    }
    cReplay_eventJump((int)SSXApp->Game->Replay, mAIIndex, &mMetrix->Totals);
    cWScriptMan_eventJump(cWScriptMan_TheWScriptMan);
    cRider_flagEventBoostPowerUp(
        this, 1, cRiderMetrix_evTakeoff(mMetrix, IsRidingFakie != IsGoofyStance, steered));
    if (mGroundNormal.z > hi + 0.019999999552965164f) {
        sV4_4298 dv = Sub_4298(VEL_4298, oldVel);
        sV4_4298 ov = oldVel;
        ov.z = 0.0f;
        dv.z = 0.0f;
        if (Dot_4298(ov, dv) < 0.0f) {
            float slowSpeed = TerminalVelocity * 0.5f;
            float fastSpeed = TerminalVelocity * 0.699999988079071f;
            float k;
            if (slowSpeed <= speed) {
                if (speed <= fastSpeed)
                    k = (speed - slowSpeed) / (fastSpeed - slowSpeed);
                else
                    k = 1.0f;
            } else {
                k = 0.0f;
            }
            SubEq_4298(VEL_4298, Scale_4298(dv, k));
        }
    }
    float slope = LastGroundNormal.z;
    sV4_4298 headingDir = *(sV4_4298*)&LastGroundNormal;
    headingDir.z = 0.0f;
    if (Abs_4298(headingDir.x) > 0.0f || Abs_4298(headingDir.y) > 0.0f)
        DivEq_4298(headingDir, Len_4298(headingDir));
    else
        headingDir = t4vect_XAxis_4298;
    *(sV4_4298*)&HalfPipeNormal = headingDir;
    if (slope < 0.05000000074505806f && slope > -0.05000000074505806f) {
        sV4_4298 lat = Scale_4298(headingDir, Dot_4298(VEL_4298, headingDir) / Dot_4298(headingDir, headingDir));
        SubEq_4298(VEL_4298, lat);
        AddEq_4298(*(sV4_4298*)&mPosition, Scale_4298(headingDir, 4.0f));
        if (!(Flags_4298(this) & 0x20))
            ScaleEq_4298(VEL_4298, 0.799975574016571f);
    }
    float len = Len_4298(VEL_4298);
    if (TerminalVelocity < len)
        ScaleEq_4298(VEL_4298, TerminalVelocity / len);
    if (speedFactor >= 0.0f && (mGroundMaterial < 11 || mGroundMaterial > 13)) {
        float tParam = (float)(AITick() - mRiderHandler->GroundMotion.AITickOnGainFocus) * 0.01666666753590107f;
        float k;
        if (tParam <= 0.5388872623443604f) {
            k = 0.6939882636070251f;
        } else {
            float rampUp = (tParam - 0.5388872623443604f) * 1.2326725721359253f <? 1.0f; // PORT: g++ minimum operator
            k = rampUp + (1.0f - rampUp) * 0.6939882636070251f;
        }
        ScaleEq_4298(VEL_4298, k);
    }
    sV4_4298 velCross = Cross_4298(t4vect_ZAxis_4298, VEL_4298);
    float cl = Len_4298(velCross);
    if (cl > 0.0f) {
        ScaleEq_4298(velCross, 1.0f / cl);
        sV4_4298 side = Cross_4298(velCross, *(sV4_4298*)&mGroundNormal);
        VerticalJump = Dot_4298(VEL_4298, *(sV4_4298*)&mGroundNormal) * Len_4298(side);
    } else {
        VerticalJump = 0.0f;
    }
    DidPop = speedFactor > 0.0f;
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

static inline V3_4CC0 Vec3Of_4CC0(t4Vector v)
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
static inline float Dot_4CC0(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector QuatMul_4CC0(const Q_4CC0& a, const t4Vector& b)
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
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" int cRider_isChangeFakieGround_i(char* s) __asm__("cRider_isChangeFakieGround");

/**
 * Switches the rider to fakie (or back) when it slides backward on the ground.
 *
 * Only while not braking (brake channel mSlewed[3] current == 0) and moving backward faster than 111.1 along
 * mGroundForward (+0x3A0): it turns mRotation 180 degrees about the up axis (mMatrix row 2 as a pure quaternion), re-
 * derives the orientation (cRider_updateOrientationImplicit) and flips the stance (cRider_doChangeFakie, doChangeFakie).
 *
 * @param vrider  the rider (this)
 * @return 1 when the rider switched, else 0
 */
extern "C" int cRider_isChangeFakieGround_i(char* vrider)
{
    cRider* rider = (cRider*)vrider;
    float k = 111.11111450195312f;
    if (rider->mSlewed[3].cur != 0.0f) {
        return 0;
    }
    float fwdSpeed = Dot_4CC0(rider->mVelocity, rider->mGroundForward);
    if (-k < fwdSpeed) {
        return 0;
    }
    rider->mRotation = QuatMul_4CC0(Make_4CC0(Vec3Of_4CC0(rider->mMatrix.row[2])), rider->mRotation);
    cRider_updateOrientationImplicit_4CC0(rider);
    cRider_doChangeFakie_4CC0(((char*)rider));
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_isChangeFakieAir);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRider_doChangeFakie(cRider* s);
extern "C" void cAirPredictor_startLaunchIntoAir(cAirPredictor* ap, t4Vector* pos, t4Vector* vel, float t);
extern float D_004A0EA8[];
extern t4Vector t4vect_Origin_4DB8 __asm__("t4vect_Origin");

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

/**
 * Switches the rider's stance on landing when it comes down backward.
 *
 * Only while the air predictor's state (+0xAC) is 1 or 3. The landing normal and velocity come from the predictor
 * (+0x20, +0x10), or are unk_180 and mVelocity when the landing surface (SSXApp game SurfPropCache entry, 0xB0 bytes)
 * is type 0x12 or has +0x44 set. The landing must face up along unk_180, the forward speed (unk_170 . mVelocity) must
 * be <= 277.8 and unk_170 . normal <= 0.9, and the rider's forward direction projected on the landing plane must point
 * against the velocity (<= D_004A0EA8[0]). Then mPosition moves by twice the hip bone's horizontal offset (rotated by
 * mRotation), the orientation is re-derived (cRider_updateOrientationImplicit), the stance flips (cRider_doChangeFakie) and
 * the air predictor relaunches (cAirPredictor_startLaunchIntoAir, 3333.3).
 *
 * @param rider  the rider (this)
 * @return 1 when the rider switched, else 0
 */
// FAKEMATCH: variable reuse: one `char* s` holds rider->mAirPredictor and then its state word (s = (char*)*(int*)(s + 0xAC);
// compared with (char*)1 / (char*)3), so the pointer and the state share one pseudo; plain form 98.94%. Byte-identical and
// behaves like the plain form; landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only);
// replace it when an honest spelling matches.
extern "C" int cRider_isChangeFakieAir(cRider* rider)
{
    int ok = 0;
    char* airState = (char*)rider->mAirPredictor;
    airState = (char*)*(int*)(airState + 0xAC);
    if (airState == (char*)1 || airState == (char*)3) {
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
    t4Vector up = rider->RiderRoot.row[2];
    if (n == 18 || *(int*)(tbl + 0x44) != 0) {
        dir = up;
        vel = rider->mVelocity;
    } else {
        dir = *(t4Vector*)(ap + 0x20);
        vel = *(t4Vector*)(ap + 0x10);
    }
    if (Dot_4DB8(up, dir) < 0.0f) {
        return 0;
    }
    t4Vector front = rider->RiderRoot.row[1];
    float frontDot = Dot_4DB8(front, rider->mVelocity);
    if (frontDot > 277.77777099609375f) {
        return 0;
    }
    frontDot = Dot_4DB8(front, dir);
    if (frontDot > 0.8999999761581421f) {
        return 0;
    }
    t4Vector side = Normalize_4DB8(Sub_4DB8(front, Scale_4DB8(dir, frontDot)));
    vel = Normalize_4DB8(Sub_4DB8(vel, Scale_4DB8(dir, Dot_4DB8(vel, dir))));
    if (Dot_4DB8(side, vel) > D_004A0EA8[0]) {
        return 0;
    }
    cAnimModel* animModel = rider->mAnimModel;
    float* hipBone = (float*)((rider->mBoneHips << 4) + (int)animModel->localPos);
    {
        const t4Vector& origin = t4vect_Origin_4DB8;
        t4Vector off = SubO_4DB8(sV4_4DB8(hipBone[0] * animModel->ScaleVect.x, hipBone[1] * animModel->ScaleVect.y,
                                         hipBone[2] * animModel->ScaleVect.z, hipBone[3] * animModel->ScaleVect.w),
                                origin);
        off.z = 0.0f;
        AddEq_4DB8(rider->mPosition, Rot_4DB8(rider->mRotation, Scale_4DB8(off, 2.0f)));
    }
    cRider_updateOrientationImplicit(rider);
    cRider_doChangeFakie(rider);
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

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);
extern Q_5168 t4vect_Origin;
extern Q_5168 t4vect_ZAxis;

static inline Q_5168 AxisAngle_5168(const Q_5168& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return Q_5168(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline void SetHeading_5168(cRiderAnim* a, float angle)
{
    *(Q_5168*)&a->OrientationOffset.P = t4vect_Origin;
    *(Q_5168*)&a->OrientationOffset.Q = AxisAngle_5168(t4vect_ZAxis, -angle);
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_5168(const t4Vector& v, float s)
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

/**
 * Flips the rider between regular and fakie.
 *
 * Toggles the fakie flag at +0x320 (IsRidingFakie; On Tour IsRidingFakie), turns the animation heading by pi
 * (cRiderAnimBase_changeHeadingOffset on mRiderAnim) and resets its heading pose (+0x30 = t4vect_Origin, +0x40 = a pi
 * turn about t4vect_ZAxis when fakie, none otherwise), copies the flag to mRiderAnim +0x18, negates mGroundForward
 * (+0x3A0) and mGroundSide (+0x3B0), and negates the current and target values of slewed channels 0 (lean), 1, 2, 3
 * (brake) and 12 (+0x280).
 *
 */
void cRider::doChangeFakie()
{
    IsRidingFakie ^= 1;
    cRiderAnimBase_changeHeadingOffset(mRiderAnim, 3.1415927410125732f);
    if (IsRidingFakie) {
        SetHeading_5168(mRiderAnim, 3.1415927410125732f);
    } else {
        SetHeading_5168(mRiderAnim, 0.0f);
    }
    mRiderAnim->IsFlipped = IsRidingFakie;
    mGroundForward = Scale_5168(mGroundForward, -1.0f);
    mGroundSide = Scale_5168(mGroundSide, -1.0f);
    mSlewed[0].cur = -mSlewed[0].cur;
    mSlewed[0].target = -mSlewed[0].target;
    mSlewed[3].cur = -mSlewed[3].cur;
    mSlewed[3].target = -mSlewed[3].target;
    mSlewed[2].cur = -mSlewed[2].cur;
    mSlewed[2].target = -mSlewed[2].target;
    mSlewed[1].cur = -mSlewed[1].cur;
    mSlewed[1].target = -mSlewed[1].target;
    mSlewed[12].cur = -mSlewed[12].cur;
    mSlewed[12].target = -mSlewed[12].target;
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
// PORT: the unit defines cRider_verifyRailslideModeDEBUG with no parameters; this caller passes the rider.
extern "C" void cRider_verifyRailslideModeDEBUG_5358(void* self) __asm__("cRider_verifyRailslideModeDEBUG");

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
 * Styles 1 and 2 need a stance: the rider half-turns when its current stance (IsRidingFakie) disagrees. Other
 * styles
 * half-turn back when needed, then turn the rider and its animation heading a quarter turn (+90 degrees for
 * style 4, else -90).
 */
void cRider::doRailslideModeOrientation()
{
    // Half turn about the up axis (quaternion (up, 0)) whenever the landing state disagrees with the stance flag.
    int state = RailslideMode;
    if (state == 2) {
        if (IsRidingFakie == 0) {
            mRotation = QuatMul_5358(Q_5358(V3_5358(mMatrix.row[2]), 0.0f), mRotation);
            cRider_updateOrientationImplicit(this);
            doChangeFakie();
        }
    } else if (state == 1) {
        if (IsRidingFakie != 0) {
            mRotation = QuatMul_5358(Q_5358(V3_5358(mMatrix.row[2]), 0.0f), mRotation);
            cRider_updateOrientationImplicit(this);
            doChangeFakie();
        }
    } else {
        if (IsRidingFakie != 0) {
            mRotation = QuatMul_5358(Q_5358(V3_5358(mMatrix.row[2]), 0.0f), mRotation);
            cRider_updateOrientationImplicit(this);
            doChangeFakie();
        }
        if (RailslideMode == 4) {
            cRider_rotateOrientation_5358(this, *(Q_5168*)&mMatrix.row[2], 1.5707964f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, 1.5707964f);
            SetHeading_5168(mRiderAnim, 1.5707964f);
        } else {
            cRider_rotateOrientation_5358(this, *(Q_5168*)&mMatrix.row[2], -1.5707964f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, -1.5707964f);
            SetHeading_5168(mRiderAnim, -1.5707964f);
        }
    }
    cRider_verifyRailslideModeDEBUG_5358(this);
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

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_rotateOrientation(void* self, t4Vector dir, float angle);
// PORT: the unit defines cRider_verifyRailslideModeDEBUG with no parameters; this caller passes the rider.
extern "C" void cRider_verifyRailslideModeDEBUG_p(void* self) __asm__("cRider_verifyRailslideModeDEBUG");
int cRider_getMotionState(void* self);
extern Q_5640 t4vect_Origin_5640 __asm__("t4vect_Origin");
extern Q_5640 t4vect_ZAxis_5640 __asm__("t4vect_ZAxis");

static inline Q_5640 AxisAngle_5640(const Q_5640& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return Q_5640(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline void SetHeading_5640(cRiderAnim* a, float angle)
{
    *(Q_5640*)&a->OrientationOffset.P = t4vect_Origin_5640;
    *(Q_5640*)&a->OrientationOffset.Q = AxisAngle_5640(t4vect_ZAxis_5640, -angle);
}

/**
 * Undoes the sideways rail-slide orientation when the rider leaves a rail.
 *
 * Nothing to do while the rail-slide mode (RailslideMode, +0x328; On Tour RailslideMode) is 0. In the air (motion state 1)
 * modes 3 and 4 reset the animation heading, and it plays 0x11A (mode 3), 0x115 (mode 4) or 0x10C (other modes). On the
 * ground (motion state 0) modes 3 and 4 turn the rider back by +-90 degrees about the up axis (cRider_rotateOrientation
 * and cRiderAnimBase_changeHeadingOffset) and reset the heading, then it plays 5. Finally the mode is cleared and
 * cRider_verifyRailslideModeDEBUG runs (an empty stub: On Tour verifyRailslideModeDEBUG).
 */
// Finishes a pending turn-around request (state 3 = left, 4 = right) of the rider.
void cRider::undoRailslideModeOrientation()
{
    if (RailslideMode == 0)
        return;
    if (cRider_getMotionState((char*)this) == 1) {
        if (RailslideMode == 3 || RailslideMode == 4)
            SetHeading_5640(mRiderAnim, 0.0f);
        if (RailslideMode == 3)
            cRiderAnimBase_play(mRiderAnim, 282, 0, -1.0f);
        else if (RailslideMode == 4)
            cRiderAnimBase_play(mRiderAnim, 277, 0, -1.0f);
        else
            cRiderAnimBase_play(mRiderAnim, 268, 0, -1.0f);
    } else if (cRider_getMotionState((char*)this) == 0) {
        if (RailslideMode == 3) {
            cRider_rotateOrientation(((char*)this), mMatrix.row[2], 1.5707964f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, 1.5707964f);
            SetHeading_5640(mRiderAnim, 0.0f);
        } else if (RailslideMode == 4) {
            cRider_rotateOrientation(((char*)this), mMatrix.row[2], -1.5707964f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, -1.5707964f);
            SetHeading_5640(mRiderAnim, 0.0f);
        }
        cRiderAnimBase_play(mRiderAnim, 5, 0, -1.0f);
    }
    RailslideMode = 0;
    cRider_verifyRailslideModeDEBUG_p((char*)this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_snapAxisIncrement);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);
extern "C" void fSinCos(float* s, float* c, float angle);

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

/**
 * Snaps a two-axis stick input to the nearest multiple of an angle step, keeping its length.
 *
 * Components within @p eps of 0 are zeroed first, and a zero vector is left alone. Otherwise the angle atan2(y, x) is
 * rounded to the nearest multiple of @p step and the vector rebuilt from it with the same length (fSinCos
 * sin/cos), each component clamped to -1 .. 1 and zeroed below 0.001. cPrewindControl_update and cSpinControl_update
 * call it with a pi/4 step.
 *
 * @param y  in/out: the vertical axis value
 * @param x  in/out: the horizontal axis value
 * @param step  the angle increment in radians (On Tour rad)
 * @param eps  the dead zone (On Tour deadZone)
 */
void cRider::snapAxisIncrement(float* y, float* x, float step, float eps)
{
    float px, py, len, ang, steps, rounded;
    float sc[2];
    if (Abs_58B8(*x) <= eps) *x = 0.0f;
    if (Abs_58B8(*y) <= eps) *y = 0.0f;
    px = *x;
    py = *y;
    len = Sqrt_58B8(px * px + py * py);
    if (len == 0.0f) return;
    ang = Atan2_58B8(py, px);
    steps = ang / step;
    if (ang < 0.0f) rounded = steps + -0.5f;
    else rounded = steps + 0.5f;
    fSinCos(&sc[0], &sc[1], (float)(int)rounded * step);
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
 * @return the knee offset
 */
float cRider::getStartGateKneeOffset()
{
    return (cBECharacterInterface_getScale(cBE_getInterface__Fv(cBE_getBE(), 2), (mAIIndex)) * -90.0f) + 75.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_getStartGateHorizOffset);
#ifdef SKIP_ASM
extern "C" int cBE_getBE();
extern "C" int cBE_getInterface__Fv(int, int);
extern "C" float cBECharacterInterface_getScale(int, int);

/**
 * Returns how far the rider's start position moves forward for its character's size.
 *
 * The value is the character's scale (cBECharacterInterface_getScale on BE interface 2, for the rider's +0x86C) times
 * 90, minus 90. cRider_putAtDropOffBC, cRider_initPath and cRider_DoInSeqAnimUpdate add it along the rider's front.
 *
 * @return the offset (0 at scale 1)
 */
float cRider::getStartGateHorizOffset()
{
    return (cBECharacterInterface_getScale(cBE_getInterface__Fv(cBE_getBE(), 2), (mAIIndex)) * 90.0f) - 90.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForReaction);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int AITick();
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cSSXAudio_SpeakCheer(int snd, void* rider, int side);
int cRider_getControlState(void* self);

// PORT: PS2-only abs.s asm helper (fabsf off-PS2).
static inline float vAbs_115B58(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Plays a queued reaction animation (cheer or sulk) and clears the reaction.
 *
 * The pending reaction is the type at +0x358 (header ReactionEventType; On Tour ReactionEventType: 1 Pos, 2 UberPos, 3
 * EarnComboPos, 4 Neg, 5 UberOn, 6 UberOff) flagged at the AI tick at +0x354 (header ReactionEventTick; On Tour
 * ReactionEventTick). Nothing happens while channel 2's animation category is 5 or 10 or channel 1 plays anything.
 * Within 180 ticks of the flag it plays 0x13B for types 1 .. 3 (with the animation filter at +0x8C8 for that play;
 * types 1 and 2 also cue cSSXAudio_SpeakCheer), 0x13A for type 4, and 0x13E for types 5 and 6 when |mSlewed[1].cur| < 0.75 and
 * the control state is 0. The reaction is then cleared (type 0, tick -1), also when it is older than 180 ticks.
 */
void cRider::checkForReaction()
{
    if (ReactionEventType == 0) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) == 5) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) != 0) {
        return;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) == 10) {
        return;
    }
    int delTick = AITick() - ReactionEventTick;
    if (delTick < 180) {
        if (delTick < 0) {
            return;
        }
        switch (ReactionEventType) {
        case 1:
            mRiderAnim->GraftMask = GraftFromUpperSpine;
            cRiderAnimBase_play(mRiderAnim, 315, 0, -1.0f);
            mRiderAnim->GraftMask = -1;
            cSSXAudio_SpeakCheer(cAudioInterface_bxAudio(), ((char*)this), 0);
            break;
        case 2:
            mRiderAnim->GraftMask = GraftFromUpperSpine;
            cRiderAnimBase_play(mRiderAnim, 315, 0, -1.0f);
            mRiderAnim->GraftMask = -1;
            cSSXAudio_SpeakCheer(cAudioInterface_bxAudio(), ((char*)this), 1);
            break;
        case 3:
            mRiderAnim->GraftMask = GraftFromUpperSpine;
            cRiderAnimBase_play(mRiderAnim, 315, 0, -1.0f);
            mRiderAnim->GraftMask = -1;
            break;
        case 4:
            mRiderAnim->GraftMask = GraftFromUpperSpine;
            cRiderAnimBase_play(mRiderAnim, 314, 0, -1.0f);
            mRiderAnim->GraftMask = -1;
            break;
        case 5:
        case 6:
            if (vAbs_115B58(mSlewed[1].cur) < 0.75f && cRider_getControlState((char*)this) == 0) {
                cRiderAnimBase_play(mRiderAnim, 318, 0, -1.0f);
            }
            break;
        }
    }
    ReactionEventType = 0;
    ReactionEventTick = -1;
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
void cRider::checkForProximityAction()
{
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) != 0) {
        ProximityActionDelay = 0.0f;
        return;
    }
    ProximityActionDelay += 0.01666666753590107f;
    if (ProximityActionDelay < 1.0f)
        return;

    float heading = Atan2_58B8(GetFront_5D48(this).y, GetFront_5D48(this).x);
    int i;

    if ((int)(AIrand() % 100) < 90) {
        float glance = 1.047197699546814f;
        float behind = 2.3561947345733643f;
        for (i = 0; i < 6; i++) {
            if (mRelations[i].mValid != 0 && mRelations[i].mAggressive != 0) {
                if (mRelations[i].unk_20 + 600 <= AITick()) {
                    float diff = Wrap_5D48(mRelations[i].mAngle - heading);
                    if (mRelations[i].mDistance < 1000.0f) {
                        float ad = __builtin_fabsf(diff);
                        if (ad > glance) {
                            mRelations[i].unk_20 = AITick();
                            // PORT: 64-bit long
                            mRiderAnim->GraftMask = mUpperBodyBoneMask;
                            if (ad > behind) {
                                cRiderAnimBase_play(mRiderAnim, 319, 0, -1.0f);
                            } else {
                                int left = diff < 0.0f;
                                if (left == IsRidingFakie)
                                    cRiderAnimBase_play(mRiderAnim, 321, 0, -1.0f);
                                else
                                    cRiderAnimBase_play(mRiderAnim, 320, 0, -1.0f);
                            }
                            mRiderAnim->GraftMask = -1;
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
            if (mRelations[i].mValid != 0) {
                float diff = Wrap_5D48(mRelations[i].mAngle - heading);
                if (mRelations[i].mDistance < 1000.0f) {
                    if (__builtin_fabsf(diff) > wide) {
                        mRiderAnim->GraftMask = mHeadBoneMask;
                        int left = diff < 0.0f;
                        if (left == IsRidingFakie)
                            cRiderAnimBase_play(mRiderAnim, 317, 0, -1.0f);
                        else
                            cRiderAnimBase_play(mRiderAnim, 316, 0, -1.0f);
                        mRiderAnim->GraftMask = -1;
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
extern "C" int cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayerResetEvent(int, void *);
extern "C" void cReplay_eventReset(cReplay* replay, int riderIndex);

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
int cRider::checkForReset(int isResetInput, int forceAutoReset)
{
    if (isResetInput != 0 || forceAutoReset != 0) {
        mPowerUpBoostTimer = 0.0f;
        mPowerUpTrickTimer = 0.0f;
        if (forceAutoReset == 1 || forceAutoReset == 4)
            cRiderMetrix_evAutoResetSurface(mMetrix);
        mRiderHandler->ResetControl.IsAutoReset = (forceAutoReset != 0);
        cRider_setControlState_6120(((char*)this), 9);
        cRider_setMotionState_6120(((char*)this), 3);
        cSSXAudio_PlayerResetEvent(cAudioInterface_bxAudio(), ((char*)this));
        cReplay_eventReset(SSXApp->Game->Replay, mAIIndex);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForBoardPress);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4D dot product).
static inline float Dot4_61D0(const t4Vector& a, const t4Vector& b)
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
int cRider_getMotionState(void* self);
// PORT: cRider_setControlState__FPv takes a second argument; bound by asm label
void cRider_setControlState_61D0(void* self, int on) __asm__("cRider_setControlState__FPv");

/**
 * Starts a board press from the press input.
 *
 * Nothing happens when @p dir is 0, while rail-sliding sideways (RailslideMode 3 or 4), or on the ground (motion state 0)
 * while moving backward (mVelocity . mGroundForward < 0). A positive input plays 0x18 and sets the press mode
 * (PressMode, +0x330; On Tour PressMode) to 1, a negative one plays 0x20 and sets 2; then control state 1
 * (cRider_setControlState__FPv).
 *
 * @param dir  the press input (On Tour boardPress): its sign picks the press
 * @return 1 when a press started, else 0
 */
int cRider::checkForBoardPress(float dir)
{
    int st;
    if (dir == 0.0f) return 0;
    if (RailslideMode == 3 || RailslideMode == 4) return 0;
    if (!cRider_getMotionState((char*)this)) {
        if (Dot4_61D0(*(&mVelocity), *(&mGroundForward)) < 0.0f) return 0;
    }
    if (dir > 0.0f) {
        cRiderAnimBase_play_61D0(mRiderAnim, 0x18, 0, -1.0f);
        PressMode = 1;
    } else {
        cRiderAnimBase_play_61D0(mRiderAnim, 0x20, 0, -1.0f);
        PressMode = 2;
    }
    cRider_setControlState_61D0(((char*)this), 1);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForPrewind);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_setControlState_62C8(void *, int) __asm__("cRider_setControlState__FPv");
extern "C" int cRider_getControlState_62C8(void *) __asm__("cRider_getControlState__FPv");
extern "C" void cBoardPressControl_forceExit(char *);
extern "C" void cCheatFX_setCheatInput(char *, int);

/**
 * Starts a prewind (the jump wind-up) from the jump button.
 *
 * A press sends cheat input 1 (cCheatFX_setCheatInput on the handler's +0xD20). On a press, or while the jump is held
 * and the timer at +0x360 (On Tour JumpHeldSec) is 0, it ends a board press (cBoardPressControl_forceExit on the
 * handler's +0x1D0, in control state 1) and sets control state 2. Otherwise it sets the timer to 1.0.
 *
 * @param isJumpHeld  nonzero while the jump button is held
 * @param isJumpPressed  nonzero on the frame the jump button is pressed
 * @return 1 when the prewind started, else 0
 */
int cRider::checkForPrewind(int isJumpHeld, int isJumpPressed)
{
    if (isJumpPressed != 0)
        cCheatFX_setCheatInput((char*)&mRiderHandler->CheatFX, 1);
    if ((JumpHeldSec == 0.0f && isJumpHeld != 0) || isJumpPressed != 0) {
        if (cRider_getControlState_62C8((char*)this) == 1)
            cBoardPressControl_forceExit((char*)&mRiderHandler->BoardPressControl);
        cRider_setControlState_62C8(((char*)this), 2);
        return 1;
    }
    JumpHeldSec = 1.0f;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForFinishLine);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRider_setControlState_6378(void *, int) __asm__("cRider_setControlState__FPv");

/**
 * Puts the rider into control state 10 once the value at +0x470 is not negative (cRider_setControlState).
 *
 * The control updates (cCruiseControl_update, cPrewindControl_update, cBumpControl_update, cBoardPressControl_update)
 * call it.
 *
 * @return 1 when the state was set, else 0
 */
int cRider::checkForFinishLine()
{
    if (mFinishTime >= 0.0f) {
        cRider_setControlState_6378(this, 10);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRider_checkForPunch);
#ifdef SKIP_ASM
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" cAnimSequence* cRiderAnimBase_getCurrentAnimSequence(void* anim, int layer);
extern "C" int tEventFlags_isSet1(void* p, int i);
extern "C" int tEventFlags_isJustChanged(void* p, int i);
extern "C" void cRiderAnimBase_blendOutAll(void* anim, int layer, float t);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);
struct cAnimSequence;
void cAnimSequence_fadeWeight(cAnimSequence* seq, float targetWeight, float time);

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

static inline int seqDone_163B0(tEventFlags* p)
{
    return tEventFlags_isSet1(p, 2) && tEventFlags_isJustChanged(p, 2);
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
int cRider::checkForPunch(int isLeft, int isRight)
{
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) == 3) {
        if (isLeft != 0 && isRight != 0) {
            if (seqDone_163B0(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events)) {
                cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->TimeScale = 0.0f;
                mRiderAnim->GraftMask = mUpperBodyBoneMask;
                cRiderAnimBase_play(mRiderAnim, 325, 0, -1.0f);
                float w = cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->CurrentWeight;
                float targWeight = cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->TargetWeight;
                float weightTimer = cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->WeightTimer;
                cAnimSequence* baseSeq = cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 0);
                baseSeq->TargetWeight = w;
                baseSeq->CurrentWeight = w;
                if (weightTimer != 0.0f)
                    cAnimSequence_fadeWeight(cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 0), targWeight,
                            weightTimer);
                mRiderAnim->GraftMask = -1;
            }
        } else {
            cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->TimeScale = 1.0f;
            cRiderAnimBase_blendOutAll(mRiderAnim, 0, 0.1f);
        }
    }
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) == 13) {
        int id = cRiderAnimBase_getCurrentAnimID(mRiderAnim, 1);
        int match = IsRidingFakie ? id == 323 : id == 324;
        if ((isLeft != 0 && isRight == 0 && match) || (isLeft == 0 && isRight != 0 && !match)) {
            if (seqDone_163B0(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events)) {
                cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->TimeScale = 0.0f;
                mRiderAnim->GraftMask = mUpperBodyBoneMask;
                if (id == 323)
                    cRiderAnimBase_play(mRiderAnim, 326, 0, -1.0f);
                else
                    cRiderAnimBase_play(mRiderAnim, 327, 0, -1.0f);
                mRiderAnim->GraftMask = -1;
            }
            if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 0) == 13) {
                float blendStep = mTimeScale * 0.01111111231148243f;
                float cur = PunchStrength;
                float newBlend = 1.0f;
                if (blendStep + 1.0f < cur)
                    newBlend = cur - blendStep;
                else if (cur < 1.0f - blendStep)
                    newBlend = cur + blendStep;
                PunchStrength = newBlend;
                cAnimSequence* baseSeq = cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 0);
                baseSeq->CurrentWeight = baseSeq->TargetWeight = PunchStrength;
            }
            if (match)
                PunchVect = Scale_163B0(GetSide_163B0(this), -1.0f);
            else
                PunchVect = GetSide_163B0(this);
        } else {
            cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->TimeScale = 1.0f;
            cRiderAnimBase_blendOutAll(mRiderAnim, 0, 0.1f);
        }
    }
    if (isLeft != 0 && isRight != 0) {
        if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim,
                1) == 13 && tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 0) != 0 &&
            tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1) == 0)
            return 1;
        if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim,
                1) == 3 && tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1) == 0)
            return 1;
        mRiderAnim->GraftMask = mUpperBodyBoneMask;
        cRiderAnimBase_play(mRiderAnim, 322, 1, -1.0f);
        mRiderAnim->GraftMask = -1;
        PunchStrength = 0.0f;
    } else if (isLeft != 0 || isRight != 0) {
        if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim,
                1) == 3 && tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 0) != 0 &&
            tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1) == 0)
            return 1;
        if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim,
                1) == 13 && tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1) == 0)
            return 1;
        int anim;
        if (isLeft != 0) {
            anim = IsRidingFakie ? 323 : 324;
            PunchVect = Scale_163B0(GetSide_163B0(this), -1.0f);
        } else {
            PunchVect = GetSide_163B0(this);
            anim = IsRidingFakie ? 324 : 323;
        }
        mRiderAnim->GraftMask = mUpperBodyBoneMask;
        cRiderAnimBase_play(mRiderAnim, anim, 1, -1.0f);
        mRiderAnim->GraftMask = -1;
        PunchStrength = 0.0f;
    }
    return cRiderAnimBase_getCurrentAnimCategory(mRiderAnim,
            1) == 3 || cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) == 13;
}
#endif

/**
 * Does nothing: an empty function that returns at once.
 *
 * cRider_doRailslideModeOrientation calls it with the rider as its last step; the C declares no parameters.
 */
extern "C" void cRider_verifyRailslideModeDEBUG(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", gGetGrabName);
#ifdef SKIP_ASM
extern int D_0043D160[];

/**
 * Returns a grab's name text: entry @p grabIndex of the table D_0043D160 ("", "Combo Grab ", "", "Indy ", "Method ", ...),
 * the table gGenTrickName builds trick names from.
 *
 * cFEStateUberTrick_onWidgetEvent and cFEStateUberTrick_updateTrickMenu use it.
 *
 * @param grabIndex  the table index
 * @return the name (as an int)
 */
extern "C" int gGetGrabName(int grabIndex) {
    return D_0043D160[grabIndex];
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

extern tMonsterTrick gMonsterComboTable[];

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
    tMonsterTrick* req = gMonsterComboTable;
    for (int i = 0; i < 0x18; i++, req++) {
        if (req->id == trick) {
            sTrickId_6E10 id;
            MEM_fill(&id, 0, 8);
            id.b.kind = req->reqOrient;
            id.b.dir = req->reqSpin;
            id.b.spin = req->reqReps;
            id.b.flip = req->reqFlip;
            id.b.grab = req->reqPostSpin;
            id.b.mod = req->reqGrab;
            id.b.extra = req->reqLateGrab;
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
INCLUDE_ASM("seg/seg_14B10", cHUDFX_cHUDFX);
#ifdef SKIP_ASM
/**
 * Sets up a HUD effect slot as empty: its ID (+0) becomes 52, the value cHUDFX_update stores when an effect expires.
 *
 * The cRiderMetrix HUD slots are built with it (cRiderMetrix_linkToRider).
 *
 * @param hudFx  the effect slot
 * @return @p hudFx
 */
extern "C" int* cHUDFX_cHUDFX(int *hudFx) {
    *hudFx = 52;
    return hudFx;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_update);
#ifdef SKIP_ASM
/**
 * Ages a HUD effect slot and clears it when its display time runs out.
 *
 * An empty slot (ID +0x0 == 0x34) or one with no time limit (MaxDecay +0x4 == -1) is left alone. Otherwise CurDecay
 * (+0x8) grows by 1/60 and the slot becomes empty (ID 0x34) once it reaches MaxDecay. Called for each slot by
 * cRiderMetrix_update (the rider metrics update).
 */
void cHUDFX::update()
{
    if (ID != 52) {
        float lim = MaxDecay;
        if (lim != -1.0f) {
            float curDecay = CurDecay + 0.01666666753590107f;
            CurDecay = curDecay;
            if (lim <= curDecay)
                ID = 52;
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
void cHUDFX::copyDispStr(signed char *str)
{
    signed char *src = str;
    unsigned char *dst = (unsigned char*)(&DisplayStr[0]);
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
extern "C" void cHUDFX_copyDispStr(cHUDFX*, signed char *);

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
void cHUDFX::set(int id, signed char *displayStr, int decayTime, float fparg0)
{
    copyDispStr(displayStr);
    ID = id;
    MaxDecay = fparg0;
    Data = decayTime;
    CurDecay = 0.0f;
    DisplayInt = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_set1);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cHUDFX_copyDispStr(cHUDFX*, signed char *);
extern signed char gGenIntStr_gQuickBuf[];
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
void cHUDFX::set1(int id, int displayInt, float fparg0, int data)
{
    sprintf((char*)gGenIntStr_gQuickBuf, str_pct_d, displayInt);
    copyDispStr(gGenIntStr_gQuickBuf);
    DisplayInt = displayInt;
    ID = id;
    MaxDecay = fparg0;
    Data = data;
    CurDecay = 0.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_setAsync1);
#ifdef SKIP_ASM
extern "C" void cHUDFX_copyDispStr(cHUDFX*, signed char *);

/**
 * Starts a HUD effect showing a text, timed by an animation position rather than a decay limit.
 *
 * Copies @p displayStr (cHUDFX_copyDispStr) and sets the ID (+0) to @p fxId, the data (+0xC) to @p data, the current
 * decay (+8) to -@p fparg0, the decay limit (+4) to -1 (no limit, so cHUDFX_update leaves it) and the number (+0x14) to
 * 0. The text counterpart of cHUDFX_setAsync; nothing in the build references it.
 *
 * @param hudFx  the effect slot
 * @param fxId  the effect
 * @param displayStr  the text to show
 * @param data  the effect's data
 * @param fparg0  the animation position (stored negated)
 */
extern "C" void cHUDFX_setAsync1(cHUDFX* hudFx, int fxId, signed char *displayStr, int data, float fparg0) {
    hudFx->copyDispStr(displayStr);
    hudFx->ID = fxId;
    hudFx->Data = data;
    hudFx->CurDecay = -fparg0;
    hudFx->MaxDecay = -1.0f;
    hudFx->DisplayInt = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cHUDFX_setAsync);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cHUDFX_copyDispStr(cHUDFX*, signed char *);
extern signed char gGenIntStr_gQuickBuf[];
extern char str_pct_d[];

/**
 * Starts a HUD effect showing a number, timed by an animation position rather than a decay limit.
 *
 * Formats @p displayInt with "%d" into the shared buffer gGenIntStr_gQuickBuf and copies it as the text (cHUDFX_copyDispStr). Sets the
 * number (+0x14) to @p displayInt, the ID (+0) to @p fxId, the current decay (+8) to -@p fparg0, the data (+0xC) to
 * @p fxData and the decay limit (+4) to -1 (no limit). cRiderMetrix_updateStaticHUD, cRiderMetrix_evCollectMultiplier and
 * cRiderMetrix_addToComboChain use it.
 *
 * @param hudFx  the effect slot
 * @param fxId  the effect
 * @param displayInt  the number to show
 * @param fparg0  the animation position (stored negated)
 * @param fxData  the effect's data
 */
// Variant of cHUDFX_setAsync1 that names the entry from a number (formatted into a shared buffer).
void cHUDFX::setAsync(int fxId, int displayInt, float fparg0, int fxData)
{
    sprintf((char*)gGenIntStr_gQuickBuf, str_pct_d, displayInt);
    copyDispStr(gGenIntStr_gQuickBuf);
    DisplayInt = displayInt;
    ID = fxId;
    CurDecay = -fparg0;
    Data = fxData;
    MaxDecay = -1.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", __12cRiderMetrix);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_reset(void *);

/**
 * Constructs a rider's metrics with cleared counters and a multiplier of 1.0 (+0x1C4), then resets them.
 */
cRiderMetrix::cRiderMetrix()
{
    Rider = 0;
    HUDfx = 0;
    NumHUDfx = 0;
    mPointsTotalInitial = 0;
    mPointsMultiplier = 1.0f;
    cRiderMetrix_reset(this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", cRiderMetrix__cRiderMetrix);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(void* mem);
extern "C" void operator_delete__FPi(void *);

/**
 * Destroys a rider's metrics object: frees its HUD effect slots and, if asked, itself.
 *
 * Frees the HUDfx array (+0x1B0) with cMemMan_free when it was allocated, then the object with operator_delete when bit
 * 0 of @p flags is set. Called by cRider__cRider with 3.
 *
 * @param metrix  the metrics object (this)
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRiderMetrix__cRiderMetrix(cRiderMetrix* metrix, int flags) {
    cHUDFX* hudFx = metrix->HUDfx;
    if (hudFx != 0) {
        cMemMan_free__FPv(hudFx);
    }
    if (flags & 1) {
        operator_delete__FPi(metrix);
    }
}
#endif
