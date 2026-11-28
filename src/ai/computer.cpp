#include "common.h"
#include "be/beinterface.h"
#include "path/basepath.h"
#include "path/trackpath.h"
#include "visualfx/splashfx.h"
#include "ai/ridermetrix.h"
#include "main/gamemode.h"
#include "path/aipath.h"
#include "ai/rideranim.h"
#include "ai/focusbox.h"
#include "path/pathsys.h"
#include "bx/math3d.h"
#include "animation/animmodel.h"
#include "ai/airpredictor.h"
#include "replay/frameptr.h"
#include "ai/riderhandler.h"
#include "ai/controlcmd.h"
#include "sound/ssxaudio.h"
#include "ai/computer.h"
#include "be/beintnewrace.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("ai/computer", cComputer_setIndividualRiderDifficulty);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sAiTune_SIRD {
    char pad0[0xDC];
    float accel;     // 0xDC
    float speed;     // 0xE0
    int boost;       // 0xE4
};


/**
 * Applies the AI tuning for one difficulty level to this computer rider.
 *
 * Levels 1-5 each set skillA, skillB and the tune block's accel, speed and boost flag; level 1 has the highest
 * skill values and top speed, level 5 the lowest, and only level 1 leaves boost off. Mode 0 and mode 2 then
 * override skillA, skillB and speed with their own per-level tables (mode 0 lower, mode 2 higher). skillB is
 * written as a percentage and scaled by 0.01 at the end, even when @p level is outside 1-5 and nothing else changed.
 *
 * @param level  difficulty level, 1 to 5; other values leave the tuning as it was
 * @param mode   tuning variant: 0 and 2 apply their override tables, any other value keeps the base values
 */
// Applies the per-level AI tuning; mode 0 and mode 2 override the skill values with their own tables.
// skillB is stored as a percentage and scaled to 0..1 at the end.
void cComputer::setIndividualRiderDifficulty(int level, int mode)
{
    switch (level) {
    case 1:
        mRiderDifficulty = 100.0f;
        mTrickDifficulty = 86.9153976f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->accel = -49670.1406f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 47500.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->boost = 0;
        break;
    case 2:
        mRiderDifficulty = 80.3891449f;
        mTrickDifficulty = 79.5998383f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->accel = -70000.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 45000.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->boost = 1;
        break;
    case 3:
        mRiderDifficulty = 51.007576f;
        mTrickDifficulty = 68.9012375f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->accel = -81571.1797f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 40000.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->boost = 1;
        break;
    case 4:
        mRiderDifficulty = 46.6018982f;
        mTrickDifficulty = 56.8430748f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->accel = -80321.1797f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 37500.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->boost = 1;
        break;
    case 5:
        mRiderDifficulty = 14.000701f;
        mTrickDifficulty = 42.4410439f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->accel = -92500.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 32500.0f;
        ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->boost = 1;
        break;
    }
    if (mode == 0) {
        switch (level) {
        case 1:
            mRiderDifficulty = 100.0f;
            mTrickDifficulty = 83.8930817f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 45000.0f;
            break;
        case 2:
            mRiderDifficulty = 55.8049736f;
            mTrickDifficulty = 64.6436768f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 42500.0f;
            break;
        case 3:
            mRiderDifficulty = 40.2501869f;
            mTrickDifficulty = 60.0044975f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 37500.0f;
            break;
        case 4:
            mRiderDifficulty = 15.5423021f;
            mTrickDifficulty = 52.0049553f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 32500.0f;
            break;
        case 5:
            mRiderDifficulty = 7.95483208f;
            mTrickDifficulty = 35.0202637f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 30000.0f;
            break;
        }
    } else if (mode == 2) {
        switch (level) {
        case 1:
            mRiderDifficulty = 100.0f;
            mTrickDifficulty = 100.0f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 50000.0f;
            break;
        case 2:
            mRiderDifficulty = 84.9981003f;
            mTrickDifficulty = 82.6824875f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 47500.0f;
            break;
        case 3:
            mRiderDifficulty = 74.9987259f;
            mTrickDifficulty = 70.8223801f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 40000.0f;
            break;
        case 4:
            mRiderDifficulty = 54.9958344f;
            mTrickDifficulty = 59.9440651f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 37500.0f;
            break;
        case 5:
            mRiderDifficulty = 29.9814339f;
            mTrickDifficulty = 46.9987717f;
            ((sAiTune_SIRD*)RiderHandler.GroundMotion.mRider)->speed = 32500.0f;
            break;
        }
    }
    mTrickDifficulty *= 0.01f;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_updateRiderDifficulty);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"
// The AI's rider list (the game's +0xC object; ai/ai's sAiRiderList): riders at 0x28, their count at 0x78.
struct sAiRiderList
{
    char pad0[0x8];
    int frame;                  // 0x8: frame counter
    char padC[0x1C];
    char* mRiders[19];          // 0x28
    int mMode;                  // 0x74
    int mCount;                 // 0x78
    int pad7C;                  // 0x7C
    int f80;                    // 0x80
};

// The game singleton SSXApp (seg/seg_1FBE38's sApp / sGame; only the members this unit reads).
struct sGame {
    char pad_0x0[0xC];
    sAiRiderList* race;         // 0xC
    char pad_0x10[0x18];
    void* f28;                  // 0x28
};
struct sApp {
    char pad_0x0[0x84];
    sGame* game;                // 0x84
    char pad_0x88[0x38];
    char* fC0;                  // 0xC0 (ai/ai: p)
};
extern sApp* SSXApp_UR __asm__("SSXApp");
extern int D_005308D0[];
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" short cComputer_updateRiderPathDifficulty(void* self, int level, int mode);
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int cBENewPlayerInterface_getDifficultyLevel(void* iface, int flag);
void cComputer_setIndividualRiderDifficulty_UR(void* self, int level, int mode) __asm__("cComputer_setIndividualRiderDifficulty");

static inline int ValidOr_UR(int v, int invalid, int def)
{
    if (v <= invalid)
        v = def;
    return v;
}

/**
 * Sets this computer rider's difficulty values for the race from its AI index (1-5) and the heat number.
 *
 * The level is the rider's getAIIndex() (1 if negative), the heat SSXApp->GameModeMan->iHeatNum. It sets
 * mPathDifficulty by updateRiderPathDifficulty(), mShouldTakeShortcuts for level 1 in the final heat
 * (cGameModeMan::kFinalHeat) and mShouldBeRandom in game modes kGAMEMODE_SLOPESTYLE, kGAMEMODE_TIMECHALLENGE and
 * kGAMEMODE_PNTSCHALLENGE, then calls setIndividualRiderDifficulty() with a profile difficulty setting
 * (cBENewPlayerInterface_getDifficultyLevel). Game events kGAMEEVENT_RIVALTIME and kGAMEEVENT_RIVALPOINTS force
 * mPathDifficulty to kPD_Easy and kPD_Hard. The
 * Super AI race rule (D_005308D0 bit 2, "kT_11SuperAI") then maxes both skills, enables shortcuts, sets the rider's
 * rubber-band thresholds to -100/100 with glue level AI_GLUE_SPEEDUP (+0xE4, mGlueStrengthLevel), mPathDifficulty to
 * kPD_Hard (levels 1-2) or kPD_Medium and returns; otherwise locations kLOCATION_ERA5 (+25%) and kLOCATION_CRA3 /
 * kLOCATION_DRA4 (+10%) raise mTrickDifficulty, capped at 1.
 */
void cComputer::updateRiderDifficulty()
{
    cRider* rider = RiderHandler.GroundMotion.mRider;
    int mode = *(int*)SSXApp_UR->fC0;
    int aiIndex = rider->getAIIndex();
    int level = ValidOr_UR(aiIndex, -1, 1);
    mPathDifficulty = cComputer_updateRiderPathDifficulty(this, level, mode);
    if (level == 1 && mode == cGameModeMan::kFinalHeat) {
        mShouldTakeShortcuts = 1;
    } else {
        mShouldTakeShortcuts = 0;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_SLOPESTYLE
        || cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_PNTSCHALLENGE
        || cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_TIMECHALLENGE) {
        mShouldBeRandom = 1;
    } else {
        mShouldBeRandom = 0;
    }
    cComputer_setIndividualRiderDifficulty_UR(this, level,
            cBENewPlayerInterface_getDifficultyLevel(cBE_getInterface_Fv(cBE_getBE(), 1),
            cBENewRaceInterface_newRace.cGameMode == 0));
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME) {
        mPathDifficulty = cComputer::kPD_Easy;
    } else if (cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALPOINTS) {
        mPathDifficulty = cComputer::kPD_Hard;
    }
    if ((D_005308D0[0] >> 2) & 1) {
        mTrickDifficulty = 1.0f;
        mRiderDifficulty = 100.0f;
        mShouldTakeShortcuts = 1;
        RiderHandler.GroundMotion.mRider->mRubberBandBehind = -100.0f;
        RiderHandler.GroundMotion.mRider->mRubberBandAhead = 100.0f;
        RiderHandler.GroundMotion.mRider->mGlueStrengthLevel = AI_GLUE_SPEEDUP;
        if (level == 1 || level == 2) {
            mPathDifficulty = cComputer::kPD_Hard;
        } else {
            mPathDifficulty = cComputer::kPD_Medium;
        }
        return;
    }
    if (cBENewRaceInterface_getLocationID(iface) == kLOCATION_ERA5) {
        mTrickDifficulty += mTrickDifficulty * 0.25f;
    } else if (cBENewRaceInterface_getLocationID(iface) == kLOCATION_CRA3
               || cBENewRaceInterface_getLocationID(iface) == kLOCATION_DRA4) {
        mTrickDifficulty += mTrickDifficulty * 0.10000000149011612f;
    }
    if (mTrickDifficulty > 1.0f) {
        mTrickDifficulty = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_setAttributeLevels);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);
extern "C" void cBEStatInterface_setStatLevel(void* iface, int a1, int a2);

struct sVEntry10C9A8 { short delta; short index; int (*fn)(void*); };

/**
 * Sets every stat of the computer's rider to one level chosen by the current location.
 *
 * Reads the location DB's field 0x54 (segmentID, the peak) for the current race location
 * (cBENewRaceInterface_getLocationPeak1 on the new-race interface) and calls cBEStatInterface_setStatLevel for the
 * rider's AI index (rider vtable slot 7, getAIIndex) with level 7 on kPEAK_3, 4 on kPEAK_2 and 1 otherwise. Called from
 * cAI_initComputerRiders for each new computer.
 */
void cComputer::setAttributeLevels()
{
    void* statInterface = cBE_getInterface_Fv(cBE_getBE(), 3);
    void* newRaceInterface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_getLocationPeak1(newRaceInterface) == kPEAK_3)
    {
        cBEStatInterface_setStatLevel(statInterface, RiderHandler.GroundMotion.mRider->getAIIndex(), 7);
    }
    else if (cBENewRaceInterface_getLocationPeak1(newRaceInterface) == kPEAK_2)
    {
        cBEStatInterface_setStatLevel(statInterface, RiderHandler.GroundMotion.mRider->getAIIndex(), 4);
    }
    else
    {
        cBEStatInterface_setStatLevel(statInterface, RiderHandler.GroundMotion.mRider->getAIIndex(), 1);
    }
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/computer", cComputer_initRiderDifficulty);
#ifdef SKIP_ASM
/**
 * Initialises this computer rider's difficulty by calling cComputer_updateRiderDifficulty().
 */
void cComputer::initRiderDifficulty()
{
    updateRiderDifficulty();
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_chooseGrabMain);
#ifdef SKIP_ASM
extern "C" int cComputer_shouldRiderUber(void* self);
extern "C" int cComputer_shouldRiderTweak(void* self);
extern "C" int cComputer_shouldRiderGrab(void* self);
extern "C" int cComputer_chooseRandomUberGrab(cComputer* self, float t);
extern "C" int cComputer_chooseRandomTweakGrab(cComputer* self, float t);
extern "C" int cComputer_chooseRandomGrab(cComputer* self, float t);

/**
 * Picks the grab for a jump with the given air time: an uber, a tweak or a plain grab, tried in that order.
 *
 * Returns kNoGrab when the air time is 0.75 s or less, exactly 100 (getSpinCmd's value when the air predictor has no
 * landing), or under 1 s while the rider is heading for a rail (mToRail). Each kind is tried only when its
 * shouldRider*() roll passes and the kind before it found nothing.
 *
 * @param airTime  predicted air time left, in seconds (On Tour airTime)
 * @return the chosen grab slot (0-14), or kNoGrab for none
 */
int cComputer::chooseGrabMain(float airTime)
{
    int currentGrab = kNoGrab;
    int skip = mToRail != 0 && airTime < 1.0f;
    if (!skip && airTime > 0.75f && airTime != 100.0f)
    {
        if (cComputer_shouldRiderUber(this)) currentGrab = chooseRandomUberGrab(airTime);
        if (currentGrab == kNoGrab)
        {
            if (cComputer_shouldRiderTweak(this)) currentGrab = chooseRandomTweakGrab(airTime);
            if (currentGrab == kNoGrab)
            {
                if (cComputer_shouldRiderGrab(this)) currentGrab = chooseRandomGrab(airTime);
            }
        }
    }
    return currentGrab;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_chooseRandomGrab);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CBE0 { short delta; short index; int (*fn)(void*); };


/**
 * Picks a random plain grab (slot 0-14) and times it against the air time; clears mIsUber and mIsTweak.
 *
 * Stores @p time in mGrabStartTime and reads the grab animation's marker times (cRiderAnimBase_getAIAnimEventTime keys 2 and 1,
 * divided by cRider_getGrabAnimSpeedScale()). When @p time is at least the key-2 time it sets mGrabEndTime = t -
 * key 2 and mHoldToLandTime = key 2 - key 1 and counts the grab in GrabHistoryCounter, which this function
 * never reads: the pick is purely random.
 *
 * @param time  air time available, in seconds (On Tour time)
 * @return the grab slot, or kNoGrab when its animation does not fit in the air time
 */
int cComputer::chooseRandomGrab(float time)
{
    cComputer* self = this;
    self->mIsUber = 0;
    self->mIsTweak = 0;
    self->mGrabStartTime = time;
    int idx = AIrand() % 15;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    int anim = cBEBAGTInterface_getGrabAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), idx);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->RiderHandler.GroundMotion.mRider));
    float grabTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 2) / spd;
    float holdTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 1) / spd;
    if (time < grabTime) return kNoGrab;
    self->mGrabEndTime = time - grabTime;
    self->mHoldToLandTime = grabTime - holdTime;
    if (idx != kNoGrab && idx < 15)
    {
        self->GrabHistoryCounter[idx]++;
    }
    return idx;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_chooseRandomTweakGrab);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getTweakAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getUberAnim(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CD20 { short delta; short index; int (*fn)(void*); };


/**
 * Picks the least-used tweak grab (slots 1-14) whose animation fits in the air time and sets mIsTweak.
 *
 * Skips slots with no tweak animation (kAID_Invalid) and, while the rider's uber timer runs, slots that also have an
 * uber variant (only checked when mUberFlags bit 0 is set). A slot fits when its tweak's key-2 time plus the plain
 * grab's key-1 time is under @p time; the lowest TweakHistoryCounter wins (the first one on ties), gets mGrabEndTime
 * and mHoldToLandTime and is counted. Clears mIsUber; mIsTweak is left as it was when nothing fits.
 *
 * @param time  air time available, in seconds (On Tour time)
 * @return the tweak slot, or kNoGrab when none fits
 */
int cComputer::chooseRandomTweakGrab(float time)
{
    cComputer* self = this;
    self->mIsUber = 0;
    self->mGrabStartTime = time;
    int found = kNoGrab;
    int best = 10000;
    int i = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->RiderHandler.GroundMotion.mRider));
    for (; i < 15; i++)
    {
        int anim = cBEBAGTInterface_getTweakAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), i);
        if (anim == kAID_Invalid)
            continue;
        int alt = (self->RiderHandler.GroundMotion.mRider->mUberFlags & 1)
            ? cBEBAGTInterface_getUberAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), i,
                                           self->RiderHandler.GroundMotion.mRider->mUberLevel >= 5)
            : kAID_Invalid;
        if (0.0f < self->RiderHandler.GroundMotion.mRider->mUberTimer && alt != kAID_Invalid)
            continue;
        int anim2 = cBEBAGTInterface_getGrabAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), i);
        float normalTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim2, 1) / spd;
        float tweakHoldTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 1) / spd;
        float tweakTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 2) / spd;
        float sum = tweakTime + normalTime;
        if (sum < time && self->TweakHistoryCounter[i] < best)
        {
            self->mIsTweak = 1;
            found = i;
            self->mHoldToLandTime = tweakTime - tweakHoldTime;
            self->mGrabEndTime = time - sum;
            best = self->TweakHistoryCounter[i];
        }
    }
    if (found != kNoGrab && found < 15)
    {
        self->TweakHistoryCounter[found]++;
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_chooseRandomUberGrab);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getUberAnim(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CF68 { short delta; short index; int (*fn)(void*); };


/**
 * Picks the least-used uber grab (slots 0-14) whose animation fits in the air time and sets mIsUber.
 *
 * Fails at once unless the rider's uber timer (mUberTimer) is running. The animation is the slot's uber variant
 * (cBEBAGTInterface_getUberAnim, with a flag for mUberLevel >= 5) and slots without one (kAID_Invalid) are skipped; the
 * fit test, timing fields and counting are as in chooseRandomTweakGrab(), on UberHistoryCounter. Clears mIsTweak but
 * never clears mIsUber, so a failed call can leave an older value.
 *
 * @param time  air time available, in seconds (On Tour time)
 * @return the uber slot, or kNoGrab when none fits or no uber is available
 */
int cComputer::chooseRandomUberGrab(float time)
{
    cComputer* self = this;
    self->mIsTweak = 0;
    if (self->RiderHandler.GroundMotion.mRider->mUberTimer <= 0.0f)
        return kNoGrab;
    self->mGrabStartTime = time;
    int found = kNoGrab;
    int best = 10000;
    int i = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->RiderHandler.GroundMotion.mRider));
    for (; i < 15; i++)
    {
        int anim = cBEBAGTInterface_getUberAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), i, self->RiderHandler.GroundMotion.mRider->mUberLevel >= 5);
        if (anim == kAID_Invalid)
            continue;
        int anim2 = cBEBAGTInterface_getGrabAnim(iface, self->RiderHandler.GroundMotion.mRider->getAIIndex(), i);
        float normalTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim2, 1) / spd;
        float uberHoldTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 1) / spd;
        float uberTime = cRiderAnimBase_getAIAnimEventTime((char*)self->RiderHandler.GroundMotion.mRider->mRiderAnim, anim, 2) / spd;
        float sum = uberTime + normalTime;
        if (sum < time && self->UberHistoryCounter[i] < best)
        {
            self->mIsUber = 1;
            found = i;
            self->mHoldToLandTime = uberTime - uberHoldTime;
            self->mGrabEndTime = time - sum;
            best = self->UberHistoryCounter[i];
        }
    }
    if (found != kNoGrab && found < 15)
    {
        self->UberHistoryCounter[found]++;
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_resetCounters);
#ifdef SKIP_ASM
struct sComputer_0010D170
{
    char pad[0xE7C];
    int a[15];
    int b[15];
    int c[15];
};

/**
 * Clears the three grab history tables: GrabHistoryCounter, UberHistoryCounter and TweakHistoryCounter (15
 * entries each).
 */
void cComputer::resetCounters()
{
    for (int i = 0; i < 15; i++)
    {
        GrabHistoryCounter[i] = 0;
        UberHistoryCounter[i] = 0;
        TweakHistoryCounter[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForReset);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
int cRider_getControlState(void* self);
extern "C" float func_0031C228(float x);


// The rider's facing direction (mMatrix front row), returned by value: retail copies it out (lq / sq) for each use.
static inline t4Vector riderFacing_0010D1A0(cRider* rider) { return rider->mMatrix.row[1]; }

static inline float atan2_0010D1A0(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += 3.1415927410125732f;
        } else {
            a -= 3.1415927410125732f;
        }
    }
    return a;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_0010D1A0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_0010D1A0(float x)
{
    return x - ffloor_0010D1A0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

/**
 * Returns whether the computer rider should reset: it has been going the wrong way or has been stuck for too long.
 *
 * When the rider's mTargetID is negative (no target), mBadHeadingTime counts the calls in which the rider's PathHeading
 * is more than 60 degrees off its facing (mMatrix front row), else it is zeroed; 61 or more means wrong way. MS_Wipeout
 * (cRider_getMotionState) counts mWanderTime up and cancels wrong way; MS_Railslide with any bad-heading count returns
 * 1 at once; MS_Air or a PathDistXY under 1000 zeroes mWanderTime and, in MS_Air, returns 1 once the air predictor's
 * CurAirTime passes 30 s; anything else counts mWanderTime up. The result is wrong way or mWanderTime >= 223, and
 * outside CS_Wipeout (cRider_getControlState) only while mTargetID is negative.
 *
 * @return 1 when the rider should reset, else 0
 */
int cComputer::checkForReset()
{
    float ang = atan2_0010D1A0(riderFacing_0010D1A0(RiderHandler.GroundMotion.mRider).y,
                               riderFacing_0010D1A0(RiderHandler.GroundMotion.mRider).x);
    float headingDiff = wrap_0010D1A0(RiderHandler.GroundMotion.mRider->PathHeading - ang);
    if (mTargetID < 0) {
        if (__builtin_fabsf(headingDiff) > 1.047197699546814f) {
            mBadHeadingTime++;
        } else {
            mBadHeadingTime = 0;
        }
    } else {
        mBadHeadingTime = 0;
    }
    int wrong = mBadHeadingTime >= 61;
    if (cRider_getMotionState(RiderHandler.GroundMotion.mRider) == MS_Wipeout) {
        wrong = 0;
        mWanderTime++;
    } else if (cRider_getMotionState(RiderHandler.GroundMotion.mRider) == MS_Railslide && mBadHeadingTime > 0) {
        return 1;
    } else if (cRider_getMotionState(RiderHandler.GroundMotion.mRider) == MS_Air ||
               RiderHandler.GroundMotion.mRider->PathDistXY < 1000.0f) {
        mWanderTime = 0;
        wrong = 0;
        if (cRider_getMotionState(RiderHandler.GroundMotion.mRider) == MS_Air &&
            RiderHandler.GroundMotion.mRider->mAirPredictor->CurAirTime > 30.0f) {
            return 1;
        }
    } else {
        mWanderTime++;
    }
    int stuck = wrong || mWanderTime >= 223;
    if (cRider_getControlState(RiderHandler.GroundMotion.mRider) != CS_Wipeout) {
        stuck = stuck && mTargetID < 0;
    }
    return stuck;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_scorePath);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" float cComputer_calculateDifficultyScore(cComputer* self, int a1, int a2, int a3);
extern "C" int cAI_aiRidersOnPath(cAI* self, cAIPath* path);
extern "C" float cRider_calculateAheadDist(cRider* self, int arg1);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);


// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_10D410(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_10D410(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_10D410(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

static inline int speedLimit_10D410(int kind)
{
    if (kind == 1) {
        return 20;
    }
    if (kind == 2) {
        return 40;
    }
    if (kind == 3) {
        return 60;
    }
    if (kind == 4) {
        return 80;
    }
    if (kind == 5) {
        return 100;
    }
    if (kind == 6) {
        return 120;
    }
    return 0;
}

/**
 * Scores an AI path as a route choice for this computer rider (route selection); higher is better.
 *
 * The path's flag word (+0x38) holds a shortcut bit (0), hard, medium and easy bits (1-3), a speed rule (bits 4-6,
 * cAIPath::tPathSpeed: PS_SpeedLimit at most, PS_SpeedMinimum at least) and a speed (bits 7-9: 20-120 km/h). A path
 * that fails calculateDifficultyScore(), is a shortcut the rider may not take or breaks the speed rule scores 0; one
 * over 5000 from the point 796 ahead of the rider scores -1, and the rival's current path 100. Otherwise it sums
 * weighted terms (each /22): nearness, the speed-rule value, how few other computer riders use the path, the difficulty
 * score, the shortcut bonus and, with mShouldBeRandom, a random term.
 *
 * @param tgt  the candidate path (On Tour cAIPath* CurrentPath)
 * @return the score: 0 unusable, -1 too far, 100 the rival's path, otherwise the weighted sum
 */
float cComputer::scorePath(cAIPath* tgt)
{
    float sc[7];
    cAI* ai = SSXApp->Game->AI;
    int flags = tgt->PathAttributes;
    int kind = (flags >> 7) & 7;
    int lvl = (flags >> 4) & 7;
    int solo = flags & 1;
    if (kind > 8) {
        kind = 0;
    }
    int isMedium = (flags >> 2) & 1;
    int isHard = (flags >> 1) & 1;
    int isEasy = (flags >> 3) & 1;
    float base = calculateDifficultyScore(isEasy, isMedium, isHard);
    if (base <= 0.0f) {
        return 0.0f;
    }
    if (mShouldTakeShortcuts == 0 && solo != 0) {
        return 0.0f;
    }
    float speed = vu0Length_10D410(*(&RiderHandler.GroundMotion.mRider->mVelocity)) * 0.035999998450279236f;
    if (lvl == cAIPath::PS_SpeedLimit) {
        if ((float)speedLimit_10D410(kind) < speed) {
            return 0.0f;
        }
    } else if (lvl == cAIPath::PS_SpeedMinimum) {
        if (speed < (float)speedLimit_10D410(kind)) {
            return 0.0f;
        }
    }
    cRider* rider = RiderHandler.GroundMotion.mRider;
    float len = vu0Length_10D410(*(&rider->mVelocity));
    float total = 22.0f;
    float lookAheadSec = rider->calculateAheadDist(1) / len;
    t4Vector pos = vu0Add_10D410(*(&RiderHandler.GroundMotion.mRider->mPosition),
            vu0Scale_10D410(*(&RiderHandler.GroundMotion.mRider->mVelocity), lookAheadSec));
    t4Vector out;
    float distOut;
    cPath_intersectPath(tgt, &pos, &out, &distOut, 1);
    float distToPath = distOut;
    float distScore = 100.0f - distToPath * distToPath * 1.5999999959603883e-05f;
    if (distScore < 0.0f) {
        distScore = 0.0f;
    }
    if (5000.0f < distToPath) {
        return -1.0f;
    }
    sc[0] = distScore * 0.22727273404598236f;
    cRider* rider2 = RiderHandler.GroundMotion.mRider;
    if (rider2->PissedOff != 0 && ai->Riders[rider2->PissedAIIndex]->AIPath == tgt) {
        return 100.0f;
    }
    float zero = 0.0f;
    sc[1] = zero;
    float w = 1.0f / total;
    sc[2] = (float)lvl * (w * 0.3333333432674408f);
    int n = ai->aiRidersOnPath(tgt);
    n -= ((char*)tgt) == (char*)RiderHandler.GroundMotion.mRider->AIPath;
    float occupiedScore;
    if (n <= 0) {
        occupiedScore = 100.0f;
    } else if (n == 1) {
        occupiedScore = 25.0f;
    } else {
        occupiedScore = zero;
    }
    w = 5.0f / total;
    sc[3] = occupiedScore * w;
    w = base / total;
    sc[4] = w;
    w = 5.0f / total;
    if (mShouldTakeShortcuts != 0 && solo != 0) {
        sc[5] = w * 100.0f;
    } else {
        sc[5] = 0.0f;
    }
    w = 2.0f / total;
    if (mShouldBeRandom != 0) {
        sc[6] = (float)(AIrand() % 100) * w;
    } else {
        sc[6] = 0.0f;
    }
    float sum = 0.0f;
    for (int i = 0; i < 7; i++) {
        sum += sc[i];
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_calculateDifficultyScore);
#ifdef SKIP_ASM
/**
 * Rates how well a path's difficulty flags suit this rider's mPathDifficulty; 0 means unsuitable.
 *
 * Path difficulty kPD_Easy, kPD_Medium and kPD_Hard require @p IsEasy, @p IsMedium and @p IsHard respectively. Any
 * other value passes only when (IsEasy == IsMedium) equals IsHard, an odd test that is unused in practice since
 * mPathDifficulty is always one of the three. A passing path scores (4 - number of flags set) * 33.3, so a path meant
 * for fewer levels scores higher.
 *
 * @param IsEasy  the path suits easy riders
 * @param IsMedium  the path suits medium riders
 * @param IsHard  the path suits hard riders
 * @return 33.3 to 99.9 for a suitable path, otherwise 0
 */
float cComputer::calculateDifficultyScore(int IsEasy, int IsMedium, int IsHard)
{
    short pathDifficulty = mPathDifficulty;
    if (pathDifficulty == kPD_Easy)
    {
        if (!IsEasy)
            return 0.0f;
    }
    else if (pathDifficulty == kPD_Medium)
    {
        if (!IsMedium)
            return 0.0f;
    }
    else if (pathDifficulty == kPD_Hard)
    {
        if (!IsHard)
            return 0.0f;
    }
    else if ((IsEasy == IsMedium) != IsHard)
        return 0.0f;
    int n = 4;
    if (IsEasy)
        n--;
    if (IsMedium)
        n--;
    if (IsHard)
        n--;
    return n * 33.29999923706055f;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_myOpponentIndex);
#ifdef SKIP_ASM

/**
 * Finds this rider's opponent: the first rider in its relation table that is both valid and human.
 *
 * Scans the six mRelations entries of the rider, testing mValid and the +0x4 word, which cAI_RefreshGroupAI()
 * fills with that rider's isHuman().
 *
 * @return the opponent's index (0-5), or -1 when there is none
 */
int cComputer::myOpponentIndex()
{
    sRiderPair* relation = (sRiderPair*)RiderHandler.GroundMotion.mRider;
    for (int i = 0; i < 6; i++, relation++)
    {
        if (relation->mValid != 0 && relation->IsHuman != 0)
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_isFarBehind);
#ifdef SKIP_ASM
extern "C" cRider* cComputer_getRival(cComputer* self);
float cRider_getDistanceLeft(void* self);

/**
 * Tells whether this computer rider trails its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandBehind (a negative
 * distance set by the difficulty code). Without an opponent (cComputer_getRival, the rider at myOpponentIndex())
 * it is not behind.
 *
 * @return true when the difference is below mRubberBandBehind, otherwise false
 */
bool cComputer::isFarBehind()
{
    cRider* opponent = getRival();
    if (opponent == 0)
        return false;
    float relDist = cRider_getDistanceLeft(opponent) - (RiderHandler.GroundMotion.mRider)->mDistToFinish;
    if (relDist < RiderHandler.GroundMotion.mRider->mRubberBandBehind)
        return true;
    return false;
}
#endif
//100%
INCLUDE_ASM("ai/computer", cComputer_isFarAhead);
#ifdef SKIP_ASM
extern "C" cRider* cComputer_getRival(cComputer* self);
float cRider_getDistanceLeft(void* self);

/**
 * Tells whether this computer rider leads its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandAhead. Without an
 * opponent (cComputer_getRival, the rider at myOpponentIndex()) it is not ahead.
 *
 * @return true when the difference is above mRubberBandAhead, otherwise false
 */
bool cComputer::isFarAhead()
{
    cRider* opponent = getRival();
    if (opponent == 0)
        return false;
    float relDist = cRider_getDistanceLeft(opponent) - (RiderHandler.GroundMotion.mRider)->mDistToFinish;
    if (RiderHandler.GroundMotion.mRider->mRubberBandAhead < relDist)
        return true;
    return false;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForAttack);
#ifdef SKIP_ASM
/**
 * Tells whether another rider is this computer rider's rival, the one it attacks rather than avoids.
 *
 * @param oppID  index of the other rider
 * @return 1 when the rider is after an opponent (PissedOff) and PissedAIIndex equals @p oppID, otherwise 0
 */
int cComputer::checkForAttack(int oppID)
{
    cRider* rider = RiderHandler.GroundMotion.mRider;
    int shouldAttack = 0;
    if (rider->PissedOff != 0) {
        shouldAttack = rider->PissedAIIndex == oppID;
    }
    return shouldAttack;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForBlock);
#ifdef SKIP_ASM
extern "C" int AITick();
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
unsigned int AIrand();

/**
 * Decides whether this computer rider should block an attack, keeping each decision for two seconds.
 *
 * While mBlockTimer runs it counts down by 1/60 and returns mShouldIBlock; a negative timer is reset to 0. Otherwise,
 * on every sixth AI tick, it looks for another human rider within 200 whose channel-1 animation category is
 * kANCAT_Punch; the first one found starts a 2 s timer with a 25% chance of blocking.
 *
 * @return nonzero when the rider should block now, otherwise 0
 */
int cComputer::checkForBlock()
{
    float blockTimer = mBlockTimer;
    if (blockTimer < 0.0f)
    {
        mBlockTimer = 0.0f;
        return 0;
    }
    if (0.0f < blockTimer)
    {
        mBlockTimer = blockTimer - 0.01666666753590107f;
        return mShouldIBlock;
    }
    if (AITick() % 6 != 0)
        return 0;
    cAI* ai = SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->NumberOfRidersAllocated; i++)
    {
        if (i == RiderHandler.GroundMotion.mRider->getAIIndex())
            continue;
        if (!ai->Riders[i]->isHuman())
            continue;
        if (!(RiderHandler.GroundMotion.mRider->mRelations[i].mDistance < 200.0f))
            continue;
        if (cRiderAnimBase_getCurrentAnimCategory(ai->Riders[i]->mRiderAnim, 1) != kANCAT_Punch)
            continue;
        unsigned int randomRoll = AIrand() % 100;
        int* pf = (int*)(&mShouldIBlock);
        *pf = (float)randomRoll < 25.0f;
        mBlockTimer = 2.0f;
        return mShouldIBlock;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForPunching);
#ifdef SKIP_ASM
extern "C" int cBEAggressionInterface_getAggression(void* self, int a, int b);
void* cBEAggressionInterface_getThis();

static inline int id_0010DBF0(char* vvself)
{
    cComputer* vself = (cComputer*)vvself;
    cComputer* self = (cComputer*)vself;
    return self->RiderHandler.GroundMotion.mRider->getAIIndex();
}

static inline float atan2_0010DBF0(float y, float x, float pi)
{
    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += pi;
        } else {
            a -= pi;
        }
    }
    return a;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_0010DBF0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_0010DBF0(float x)
{
    return x - ffloor_0010DBF0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

static inline bool isAggr_0010DBF0(void* agg, int a, int b)
{
    return cBEAggressionInterface_getAggression(agg, a, b) >= kAGG_HARD;
}

/**
 * Picks a rider beside the computer to punch: returns true and the side when one is close, at the side and disliked.
 *
 * Runs only every 6th AI tick. For each other rider i (by rider index, skipping its own getAIIndex) whose
 * mRelations[i].mDistance is under 200 and whose bearing (mRelations[i].mAngle) is 30 to 150 degrees off the rider's
 * facing (mMatrix front row), and towards whom the aggression interface (cBEAggressionInterface_getAggression on
 * cBEAggressionInterface_getThis) gives kAGG_HARD or more, it takes the first one when LastPunchTick + 120 is before
 * the current AI tick, sets @p side and stores the tick in LastPunchTick.
 *
 * @param side  receives 1 when the rider is on the side of a positive angle difference, else 0
 * @return true when a target was found, else false
 */
// Taunt check: finds a close rider beside us (30..150 degrees off our heading) that we're aggressive towards,
// at most every 120 frames; *side tells which side it is on.
bool cComputer::checkForPunching(int* side)
{
    if (AITick() % 6 != 0)
        return false;
    cAI* ai = SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->NumberOfRidersAllocated; i++)
    {
        float pi = 3.1415927410125732f;
        float lo = 0.5235988497734070f;
        float hi = 2.6179940700531006f;
        if (i == id_0010DBF0(((char*)this)))
            continue;
        cRider* rider = RiderHandler.GroundMotion.mRider;
        if (!(rider->mRelations[i].mDistance < 200.0f))
            continue;
        t4Vector facingForY = rider->mMatrix.row[1];
        t4Vector facingForX = rider->mMatrix.row[1];
        float ang = atan2_0010DBF0(facingForY.y, facingForX.x, pi);
        float angleDiff = wrap_0010DBF0(ang - RiderHandler.GroundMotion.mRider->mRelations[i].mAngle);
        float ad = __builtin_fabsf(angleDiff);
        if (!(lo < ad))
            continue;
        if (!(ad < hi))
            continue;
        void* agg = cBEAggressionInterface_getThis();
        if (!isAggr_0010DBF0(agg, id_0010DBF0(((char*)this)), i))
            continue;
        if (!(RiderHandler.GroundMotion.mRider->LastPunchTick + 120 < AITick()))
            continue;
        *side = 0.0f < angleDiff;
        RiderHandler.GroundMotion.mRider->LastPunchTick = AITick();
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_getRival);
#ifdef SKIP_ASM

/**
 * Returns the computer's opponent rider: SSXApp->Game->AI's Riders[myOpponentIndex()], or 0 when it has none.
 *
 * @return the opponent's cRider, or 0
 */
cRider* cComputer::getRival()
{
    int i = myOpponentIndex();
    if (i < 0)
        return 0;
    return SSXApp->Game->AI->Riders[i];
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_setupWarp);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
float cRider_getDistanceLeft(void* self);

/**
 * Returns the rubber-band time warp for the computer rider: above 1 when it is too far behind its opponent, below 1
 * when too far ahead.
 *
 * Returns 1.0 without an opponent (cComputer_getRival), when the rider's mGlueStrengthLevel is AI_GLUE_NONE, in an
 * online race (newRace.cGamePath == 2) or within 50000 of the finish (mDistToFinish). Otherwise d = the opponent's
 * distance left minus the rider's. When d < mRubberBandBehind (AI_GLUE_SPEEDUP and AI_GLUE_ALL) it returns (d - k) / k
 * + 1 times 1.1, with k = mRubberBandBehind; when d > mRubberBandAhead (AI_GLUE_SLOWDOWN and AI_GLUE_ALL) it returns
 * 0.909 / ((d - k2) / k2 + 1), with k2 = mRubberBandAhead; else 1.0. The new-race interface is fetched and not used.
 *
 * @return the desired time scale for cRider_updateTimeWarp
 */
float cComputer::setupWarp()
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    cRider* rival = getRival();
    if (rival == 0) return 1.0f;
    cRider* rider = RiderHandler.GroundMotion.mRider;
    if (rider->mGlueStrengthLevel == AI_GLUE_NONE) return 1.0f;
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) return 1.0f;
    if (rider->mDistToFinish < 50000.0f) return 1.0f;
    float warpVal;
    float relDist = cRider_getDistanceLeft(rival) - RiderHandler.GroundMotion.mRider->mDistToFinish;
    cRider* rider2 = RiderHandler.GroundMotion.mRider;
    float k = rider2->mRubberBandBehind;
    if (relDist < k && (rider2->mGlueStrengthLevel == AI_GLUE_SPEEDUP || rider2->mGlueStrengthLevel == AI_GLUE_ALL))
    {
        warpVal = ((relDist - k) / k + 1.0f) * 1.100000023841858f;
    }
    else
    {
        cRider* rider3 = RiderHandler.GroundMotion.mRider;
        float rubberAhead = rider3->mRubberBandAhead;
        if (rubberAhead < relDist
            && (rider3->mGlueStrengthLevel == AI_GLUE_SLOWDOWN || rider3->mGlueStrengthLevel == AI_GLUE_ALL))
        {
            warpVal = 1.0f / ((relDist - rubberAhead) / rubberAhead + 1.0f);
            warpVal *= 0.9090908765792847f;
        }
        else
            warpVal = 1.0f;
    }
    return warpVal;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventReaction);
#ifdef SKIP_ASM
extern "C" int AITick();

/**
 * Flags a rider reaction (positive, uber, negative...) to show for a while, or clears it.
 *
 * With @p mode 0 (On Tour kREACT_None) ReactionEventType (+0x358) is cleared and ReactionEventTick (+0x354) set to -1. Otherwise
 * ReactionEventType = @p mode and ReactionEventTick = AITick (AITick) + (int)(@p forceDelaySec * 60), so the reaction lasts @p forceDelaySec seconds
 * of 60 Hz ticks. On Tour's eReactionType: 1 Pos, 2 UberPos, 3 EarnComboPos, 4 Neg, 5 UberOn, 6 UberOff.
 *
 * @param mode  the reaction (On Tour eReactionType react); 0 clears it
 * @param forceDelaySec  how long it lasts, in seconds (On Tour forceDelaySec)
 */
void cRider::flagEventReaction(int mode, float forceDelaySec)
{
    int reactionTick;
    if (mode == 0)
    {
        reactionTick = -1;
        ReactionEventType = 0;
    }
    else
    {
        int frames = (int)(forceDelaySec * 60.0f);
        reactionTick = AITick() + frames;
        ReactionEventType = mode;
    }
    ReactionEventTick = reactionTick;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBoostPowerUp);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cBEStatInterface_getTrickAttrib(void* iface, int rider, int b);
extern "C" void cSSXAudio_BoostBar(void* mgr, void* rider, float cur, float amt);
extern "C" void cRider_flagEventReaction(cRider* self, int mode, float t);

// PORT: g++ `<?` (min) operator.
static inline float clamp_10E098(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

/**
 * Adds an amount to the rider's boost meter (+0x2F8, kept in 0-1) and handles the uber unlock at a full
 * meter.
 *
 * Does nothing in kBOOST_Off (mBoostMode) or for a zero amount. A gain first calls a BE stat getter
 * (cBEStatInterface_getTrickAttrib, result unused) and is dropped unless @p mask shares a bit with mBoostSourceMask. It
 * notifies the audio system (cSSXAudio_BoostBar), then clamps. A gain that fills the meter with uber enabled
 * (mUberFlags) shows HUD message 5 when mUberTimer is 0, raises an mUberLevel of 0 or less by one and, at levels 1-9,
 * makes mUberTimer at least 20; a loss below eUS_SuperUberMax stops mUberTimer.
 *
 * @param mask  boost source flags (On Tour powerSourceFlags), tested against mBoostSourceMask for a gain
 * @param amt  amount to add, negative to drain (On Tour powerUp)
 */
void cRider::flagEventBoostPowerUp(int mask, float amt)
{
    if (mBoostMode == kBOOST_Off)
        return;
    float floatZero = 0.0f;
    if (amt == floatZero)
        return;
    if (amt > floatZero)
    {
        cBEStatInterface_getTrickAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), mAIIndex, mStatsIndex);
        if ((mask & mBoostSourceMask) == 0)
            return;
    }
    cSSXAudio_BoostBar(cAudioInterface_bxAudio(), this, BoostPowerBar, amt);
    BoostPowerBar = clamp_10E098(BoostPowerBar + amt, floatZero, 1.0f);
    if (BoostPowerBar == 1.0f && amt > floatZero && mUberFlags != 0)
    {
        if (mUberTimer == floatZero)
            this->flagEventReaction(5, floatZero);
        if (mUberLevel <= 0)
            mUberLevel += 1;
        if (mUberLevel >= 1 && mUberLevel <= 9)
        {
            // PORT: g++ `>?` (max) operator.
            mUberTimer = mUberTimer >? 20.0f;
        }
    }
    else if (amt < 0.0f && mUberLevel < eUS_SuperUberMax)
    {
        mUberTimer = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBump);
#ifdef SKIP_ASM

extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_RiderCollision(void* snd, void* a, void* b);
extern "C" void cSSXAudio_SpeakRiderHit(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a light bump from another rider: collision sound and a mild aggression event.
 *
 * Plays the rider-collision sound (cSSXAudio_RiderCollision, On Tour cSSXAudio::RiderCollision) and cSSXAudio_SpeakRiderHit(audio, attacker,
 * self, 1, 0), then reports aggression behaviour 0 (kAGG_MILDCONTACT) from @p attacker against @p self to cBEAggressionInterface_applyAggression (On Tour
 * cBEAggressionInterface::applyAggression). Each rider's id comes from its cBeing vtable (+0x6C0) slot 7 (On Tour
 * cBeing::getAIIndex).
 *
 * @param attacker  the rider that did the bumping (On Tour attacker)
 */
void cRider::flagEventBump(cRider* attacker)
{
    cSSXAudio_RiderCollision(cAudioInterface_bxAudio(), this, attacker);
    cSSXAudio_SpeakRiderHit(cAudioInterface_bxAudio(), attacker, this, 1, 0);
    void* agg = cBEAggressionInterface_getThis();
    int rb = attacker->getAIIndex();
    cBEAggressionInterface_applyAggression(agg, rb, getAIIndex(), 0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBumpKnockdown);
#ifdef SKIP_ASM

extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_RiderCollision(void* snd, void* a, void* b);
extern "C" void cSSXAudio_SpeakRiderHit(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a bump from another rider that knocked this one down: collision sound and a moderate aggression event.
 *
 * As cRider_flagEventBump, but cSSXAudio_SpeakRiderHit gets (attacker, self, 1, 1) and the aggression behaviour passed to
 * cBEAggressionInterface_applyAggression (On Tour cBEAggressionInterface::applyAggression) is 1 (kAGG_MODCONTACT).
 *
 * @param attacker  the rider that did the bumping (On Tour attacker)
 */
void cRider::flagEventBumpKnockdown(cRider* attacker)
{
    cSSXAudio_RiderCollision(cAudioInterface_bxAudio(), this, attacker);
    cSSXAudio_SpeakRiderHit(cAudioInterface_bxAudio(), attacker, this, 1, 1);
    void* agg = cBEAggressionInterface_getThis();
    int rb = attacker->getAIIndex();
    cBEAggressionInterface_applyAggression(agg, rb, getAIIndex(), 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPunch);
#ifdef SKIP_ASM

extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void cSSXAudio_SpeakRiderHit(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a punch from another rider: punch sound and a hard-contact aggression event.
 *
 * Plays cSSXAudio_Punch(audio, attacker, self) and cSSXAudio_SpeakRiderHit(audio, attacker, self, 0, 0), then reports aggression
 * behaviour 2 (kAGG_HARDCONTACT) from @p attacker against @p self to cBEAggressionInterface_applyAggression (On Tour
 * cBEAggressionInterface::applyAggression), the ids from each rider's cBeing vtable slot 7.
 *
 * @param attacker  the rider that threw the punch (On Tour attacker)
 */
void cRider::flagEventPunch(cRider* attacker)
{
    cSSXAudio_Punch(cAudioInterface_bxAudio(), attacker, this);
    cSSXAudio_SpeakRiderHit(cAudioInterface_bxAudio(), attacker, this, 0, 0);
    void* agg = cBEAggressionInterface_getThis();
    int rb = attacker->getAIIndex();
    cBEAggressionInterface_applyAggression(agg, rb, getAIIndex(), 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventPunchKnockdown);
#ifdef SKIP_ASM

extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void cSSXAudio_SpeakRiderHit(void* snd, void* a, void* b, int c, int d);
extern "C" void cSSXAudio_KnockdownSFX(void* snd, void* obj);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);
extern "C" float cRiderMetrix_evFrag(char* self);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);

/**
 * Reacts to a punch that knocked this rider down: sounds, the harshest aggression event and a frag reward for the
 * attacker.
 *
 * Plays cSSXAudio_Punch(attacker, self), cSSXAudio_SpeakRiderHit(audio, attacker, self, 0, 1) and cSSXAudio_KnockdownSFX for the
 * attacker, reports aggression behaviour 3 (kAGG_BERTUZZI) to cBEAggressionInterface_applyAggression, then gives the attacker
 * cRider_flagEventBoostPowerUp(attacker, 2, ...) with the boost from its metrix's cRiderMetrix_evFrag (On Tour
 * cRiderMetrix::evFrag). The prototype here declares flagEventBoostPowerUp as (rider, int, float): the float goes in
 * $f12, so the call is the same as On Tour's (float powerUp, int powerSourceFlags).
 *
 * @param attacker  the rider that threw the punch (On Tour attacker); it earns the frag
 */
void cRider::flagEventPunchKnockdown(cRider* attacker)
{
    cSSXAudio_Punch(cAudioInterface_bxAudio(), attacker, this);
    cSSXAudio_SpeakRiderHit(cAudioInterface_bxAudio(), attacker, this, 0, 1);
    cSSXAudio_KnockdownSFX(cAudioInterface_bxAudio(), attacker);
    void* agg = cBEAggressionInterface_getThis();
    int rb = attacker->getAIIndex();
    cBEAggressionInterface_applyAggression(agg, rb, getAIIndex(), 3);
    attacker->flagEventBoostPowerUp(2, cRiderMetrix_evFrag((char*)attacker->mMetrix));
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPathCheckpoint);
#ifdef SKIP_ASM
extern int D_005308D0[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evCheckpoint(void*, int);

/**
 * Handles the rider passing a path checkpoint: scores it in checkpoint events.
 *
 * Copies +0xEC into +0xE8 first. Then, unless the rules word D_005308D0 has bit 9 set (On Tour
 * cBERules::getNoCheckPoints) and only while the new-race game event (newRace.cGameEvent) is kGAMEEVENT_SLOPESTYLE,
 * calls cRiderMetrix_evCheckpoint
 * (On Tour cRiderMetrix::evCheckpoint) on the rider's metrix (+0x790) with the checkpoint's first word (On Tour
 * tPathCheckpoint::TimeBonusSec).
 *
 * @param checkpoint  the checkpoint passed (On Tour tPathCheckpoint *cp): time bonus in seconds, then distance left
 */
void cRider::flagEventPathCheckpoint(int* checkpoint)
{
    PrevRanking = RaceRanking;
    if (((D_005308D0[0] >> 9) & 1) == 0)
    {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_SLOPESTYLE)
        {
            cRiderMetrix_evCheckpoint(mMetrix, *checkpoint);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPathEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cRider_doFinishEvent(void* self);
extern "C" void cReplay_eventChapter(void* a, int b, int c);

static inline int state_0010E5D8() { return cBENewRaceInterface_newRace.cGameMode; }


/**
 * Handles a track-path event the rider reached: the finish line ends its race, a chapter marker tags the replay.
 *
 * A finish event (PE_FinishLine) counts when the game mode (cBENewRaceInterface_newRace.cGameMode) is kGAMEMODE_RACE to
 * kGAMEMODE_HALFPIPE, or kGAMEMODE_TIMECHALLENGE / kGAMEMODE_PNTSCHALLENGE with the game-mode manager's +0x98 set (On
 * Tour isLastHeat), or the location is kLOCATION_BRA2 or kLOCATION_ASS1 to kLOCATION_EHP3, or a game path is set; then
 * a rider not yet past the line (mFinishTime < 0) gets cRider_doFinishEvent. A chapter event (PE_Chapter) calls
 * cReplay_eventChapter on the game's replay with the rider's id (cBeing vtable slot 7) and the event's Arg.
 *
 * @p isPrev and @p isCur (On Tour isPrev/isCur) are not read.
 *
 * @param pathEvent  the path event node (On Tour tPathEventNode *ev)
 * @param isPrev  On Tour isPrev; unused
 * @param isCur  On Tour isCur; unused
 */
void cRider::flagEventPathEvent(tPathEventNode* pathEvent, int isPrev, int isCur)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cGameModeMan* gameModeMan = SSXApp->GameModeMan;
    int location = *cBENewRaceInterface_getLocation(iface);
    int shouldTriggerFinishLine = 0;
    if (state_0010E5D8() == kGAMEMODE_RACE || state_0010E5D8() == kGAMEMODE_SLOPESTYLE
        || state_0010E5D8() == kGAMEMODE_HALFPIPE || state_0010E5D8() == kGAMEMODE_BIGAIR
        || (state_0010E5D8() == kGAMEMODE_TIMECHALLENGE && gameModeMan->bIsLastHeat != 0)
        || (state_0010E5D8() == kGAMEMODE_PNTSCHALLENGE && gameModeMan->bIsLastHeat != 0)
        || location == kLOCATION_ASS1 || location == kLOCATION_DSS2 || location == kLOCATION_ESS3
        || location == kLOCATION_ABA1 || location == kLOCATION_CBA2 || location == kLOCATION_EBA3
        || location == kLOCATION_BHP1 || location == kLOCATION_CHP2 || location == kLOCATION_EHP3
        || location == kLOCATION_BRA2
        || cBENewRaceInterface_newRace.cGamePath != 0)
        shouldTriggerFinishLine = 1;
    if (pathEvent->Type == PE_FinishLine && shouldTriggerFinishLine != 0)
    {
        int ok = mFinishTime >= 0.0f;
        if (!ok)
            cRider_doFinishEvent(this);
    }
    if (pathEvent->Type == PE_Chapter)
    {
        void* tgt = SSXApp->Game->Replay;
        cReplay_eventChapter(tgt, getAIIndex(), pathEvent->Arg);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventSpeedBoost);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void cSSXAudio_SpeakArcadeBoost(void*, void*, int);

/**
 * Adds time to the rider's speed power-up boost and plays the boost icon sound.
 *
 * mPowerUpBoostTimer (+0x2E8) += @p amount, then cSSXAudio_PlayIconSound(audio, 0, self, 1.0f) (On Tour icon type 0;
 * flagEventTrickBoost uses 1) and cSSXAudio_SpeakArcadeBoost(audio, self, 1), which On Tour's version lacks. Virtual (cRider vtable
 * entry 13 in the kRiderVt_* numbering, after initForMission); no direct callers.
 *
 * @param amount  seconds of boost to add (On Tour sec)
 */
void cRider::flagEventSpeedBoost(float amount)
{
    mPowerUpBoostTimer += amount;
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 0, this, 1.0f);
    cSSXAudio_SpeakArcadeBoost(cAudioInterface_bxAudio(), this, 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventTrickBoost);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void cSSXAudio_SpeakArcadeBoost(void*, void*, int);

/**
 * Adds @p amount to the rider's mPowerUpTrickTimer and plays the icon sound for type 1.
 *
 * Also calls cSSXAudio_SpeakArcadeBoost with 2 on the sound system. A cRider virtual (it is in six rider vtables).
 *
 * @param amount  the time to add
 */
void cRider::flagEventTrickBoost(float amount)
{
    mPowerUpTrickTimer += amount;
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 1, this, 1.0f);
    cSSXAudio_SpeakArcadeBoost(cAudioInterface_bxAudio(), this, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventTrickMultiplier);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
extern "C" float cRiderMetrix_evCollectMultiplier(char* self, float value);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

/**
 * Handles a collected multiplier icon: in MS_Air and MS_Railslide hands it to the rider metrix and plays its sound.
 *
 * In those states it calls cRider_flagEventBoostPowerUp with source mask 4 and the boost returned by cRiderMetrix_evCollectMultiplier (the
 * metrix's multiplier handler, which returns 0), then cSSXAudio_PlayIconSound with type 2 (multiplier) and @p value. A
 * cRider virtual (it is in six rider vtables).
 *
 * @param value  the multiplier
 */
void cRider::flagEventTrickMultiplier(float value)
{
    if (cRider_getMotionState(this) == MS_Air || cRider_getMotionState(this) == MS_Railslide)
    {
        flagEventBoostPowerUp(4, cRiderMetrix_evCollectMultiplier((char*)mMetrix, value));
        cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 2, this, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventAddPoints);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);
extern "C" float cRiderMetrix_evAddPoints(cRiderMetrix* metrix);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

/**
 * Adds trick points to the rider: the metrix's points handler (cRiderMetrix_evAddPoints) and the icon sound for type 3.
 *
 * The points reach cRiderMetrix_evAddPoints untouched in a1 (the C declaration takes only self), which adds them to the score; its
 * result (0) goes to cRider_flagEventBoostPowerUp with source mask 0x10.
 */
void cRider::flagEventAddPoints()
{
    flagEventBoostPowerUp(0x10, cRiderMetrix_evAddPoints(mMetrix));
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 3, this, 1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventLand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRider_flagEventReaction(cRider* self, int mode, float t);
void cRider_flagEventBoostPowerUp_fi(void*, float, int) __asm__("cRider_flagEventBoostPowerUp");
void cRiderHandler_recordSprayBurst_impl(void* self, void* a, void* b, int c, float x) __asm__("cRiderHandler_recordSprayBurst__FPv");
extern "C" float cRiderMetrix_evLand(void* self, int stance, int alternate, int style, int flag);
extern "C" int cBEStatInterface_shouldCelebrateTrick(void* self, int rider, int value);
void cReplay_eventLand_10E910(void* self, int key, void* p) __asm__("cReplay_eventLand");
extern "C" void cSSXAudio_Land(void* mgr, void* rider, float v);


/**
 * Handles the rider landing: scores the trick, updates the uber meter, reacts, boosts and plays the landing sound.
 *
 * Takes the metrix totals before and after cRiderMetrix_evLand (On Tour cRiderMetrix::evLand(riding switch =
 * IsRidingFakie != IsGoofyStance, @p isPipe, @p rsm, @p pressMode)), whose result is the boost earned, and logs the
 * landing to the replay (cReplay_eventLand with mAIIndex and the metrix stats at +0xFC). Uber tricks landed
 * (Totals.NumUberTricks, metrix +0x114, grew) raise mUberLevel by that many, capped at eUS_SuperUberMax; reaching it
 * sets mUberTimer to 60.
 *
 * cRiderHandler_recordSprayBurst (rider-handler landing effect) gets the intersection's position/normal, its surface (+0x4C) and @p speed. A
 * local rider (cBeing vtable slot 8, isHuman) gets a reaction through cRider_flagEventReaction: 3 (EarnComboPos) when the
 * combo-link count (+0x134) grew, 2 (UberPos) when an uber trick landed, else 1 (Pos) when cBEStatInterface_shouldCelebrateTrick (On Tour
 * cBEStatInterface::shouldCelebrateTrick) agrees for the points gained (Totals.TotalPoints, +0x198). Then
 * cRider_flagEventBoostPowerUp(boost, 1), a positive boost clears mPowerUpTrickTimer, vtable slot 17 (vibrationBump)
 * rumbles with @p speed / 2, and cSSXAudio_Land (On Tour cSSXAudio::Land) plays the landing.
 *
 * @param isPipe  On Tour isPipe: landing in a half-pipe
 * @param rsm  On Tour eRailslideMode rsm: the rail-slide mode landed in
 * @param pressMode  On Tour ePressMode: the board press held on landing
 * @param evt  the ground intersection landed on (On Tour tIntersectInfo): position, normal, surface at +0x4C
 * @param speed  the landing speed (On Tour speed)
 */
void cRider::flagEventLand(int isPipe, int rsm, int pressMode, tIntersectInfo* evt, float speed)
{
    cRiderMetrix* metrix = mMetrix;
    int prevNumUber = metrix->Totals.NumUberTricks;
    int prevNumCombo = metrix->Totals.NumComboChainLinks;
    int prevPoints = metrix->Totals.TotalPoints;
    float powerUp = cRiderMetrix_evLand(metrix, IsRidingFakie != IsGoofyStance, isPipe, rsm, pressMode);
    cRiderMetrix* metrixAfter = mMetrix;
    int curNumUber = metrixAfter->Totals.NumUberTricks;
    int curNumCombo = metrixAfter->Totals.NumComboChainLinks;
    int curPoints = metrixAfter->Totals.TotalPoints;
    cReplay_eventLand_10E910(SSXApp->Game->Replay, mAIIndex, &metrixAfter->Totals);
    if (prevNumUber < curNumUber)
    {
        int uberLevel = mUberLevel;
        if (uberLevel < eUS_SuperUberMax)
        {
            int numUber = curNumUber - prevNumUber;
            int lim = eUS_SuperUberMax - uberLevel;
            int nv = uberLevel + (numUber < lim ? numUber : lim);
            mUberLevel = nv;
            if (nv == eUS_SuperUberMax)
                mUberTimer = 60.0f;
        }
    }
    mRiderHandler->recordSprayBurst(evt, &evt->Inorm, evt->Stype, speed);
    if (isHuman())
    {
        if (prevNumCombo < curNumCombo)
            this->flagEventReaction(3, 0.0f);
        else if (prevNumUber < curNumUber)
            this->flagEventReaction(2, 0.0f);
        else if (cBEStatInterface_shouldCelebrateTrick(cBE_getInterface_Fv(cBE_getBE(), 3), mAIIndex,
                curPoints - prevPoints))
            this->flagEventReaction(1, 0.0f);
    }
    cRider_flagEventBoostPowerUp_fi(((char*)this), powerUp, 1);
    if (0.0f < powerUp)
        mPowerUpTrickTimer = 0.0f;
    sVEntry<void(void*, float)>* vtable = (sVEntry<void(void*, float)>*)VPTR_AT(this, sizeof(cRiderState));
    sVEntry<void(void*, float)>* vf = &vtable[17];
    vf->fn(vthis(this, *vf), speed * 0.5f);
    cSSXAudio_Land(gSSXAudio, ((char*)this), powerUp);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventWipeout);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_getAIAnimFrame(void* buf, void* anim, int id, int a3, float t);
extern "C" void func_0030ECD8(void* xf, void* model, void* buf, int a3);
extern "C" void cRiderAnimBase_changeOrientationOffset(void* self, void* xf);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float a);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
extern "C" void cRider_doLeanPoseAdjust(cRider* self, t4Vector* pos);
extern "C" void cRider_verifyRailslideModeDEBUG(cRider* self);
extern "C" void cRider_rotateOrientation(cRider* self, const t4Vector& axis, float angle);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float cRiderMetrix_evWipeout(cRiderMetrix* m, int a);
extern "C" void cSSXAudio_WipeoutHit(void* mgr, cRider* rider);
extern "C" void cSSXAudio_PlayGrunt(void* mgr, void* rider, int a, int b);
extern "C" void cSSXAudio_Wipeout(void* mgr, cRider* rider, float v);
void cRider_setControlState_EB30(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_EB30(void* self, int v) __asm__("cRider_setMotionState__FPv");
struct sCfg_EB30 { int a; int b; };
extern sCfg_EB30 D_004A4C08;

struct sQuat_EB30
{
    float x, y, z, w;
    sQuat_EB30() {}
    sQuat_EB30(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_EB30 t4vect_Origin_EB30 __asm__("t4vect_Origin");
extern sQuat_EB30 t4vect_ZAxis_EB30 __asm__("t4vect_ZAxis");

static inline sQuat_EB30 AxisAngle_EB30(const sQuat_EB30& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_EB30(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline t4Vector GetUp_EB30(cRider* rider) { return rider->mMatrix.row[2]; }

// Clears the anim's position offset and sets its heading quaternion to -angle about t4vect_ZAxis.
static inline void ResetAnimOrientation_EB30(char* vra, float angle)
{
    cRiderAnim* ra = (cRiderAnim*)vra;
    *(sQuat_EB30*)&ra->OrientationOffset = t4vect_Origin_EB30;
    *(sQuat_EB30*)&ra->OrientationOffset.Q = AxisAngle_EB30(t4vect_ZAxis_EB30, -angle);
}

struct sVE_EB30 { short delta; short index; void (*fn)(void*, float); };

struct sXf_EB30
{
    sQuat_EB30 pos;
    sQuat_EB30 rot;
};

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_EB30 QuatMul_EB30(const sQuat_EB30& a, const sQuat_EB30& b)
{
    sQuat_EB30 r;
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sQuat_EB30 Sub_EB30(const sQuat_EB30& a, const sQuat_EB30& b)
{
    sQuat_EB30 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sQuat_EB30 Add_EB30(const sQuat_EB30& a, const sQuat_EB30& b)
{
    sQuat_EB30 r;
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

// PORT: PS2-only VU0 inline asm (v rotated by quaternion q).
static inline sQuat_EB30 Rot_EB30(const sQuat_EB30& q, const sQuat_EB30& v)
{
    sQuat_EB30 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sQuat_EB30 Scale_EB30(const sQuat_EB30& v, float s)
{
    sQuat_EB30 r;
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

static inline sQuat_EB30 Conj_EB30(const sXf_EB30& p)
{
    sQuat_EB30 r;
    r.w = p.rot.w;
    r.x = p.rot.x;
    r.y = p.rot.y;
    r.z = p.rot.z;
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

static inline sXf_EB30 Invert_EB30(const sXf_EB30& a)
{
    sXf_EB30 r;
    r.rot = Conj_EB30(a);
    r.pos = Add_EB30(Scale_EB30(Rot_EB30(r.rot, Sub_EB30(a.pos, t4vect_Origin_EB30)), -1.0f), t4vect_Origin_EB30);
    return r;
}

static inline sXf_EB30 Combine_EB30(const sXf_EB30& a, const sXf_EB30& b)
{
    sXf_EB30 r;
    r.rot = QuatMul_EB30(a.rot, b.rot);
    r.pos = Add_EB30(Rot_EB30(a.rot, Sub_EB30(b.pos, t4vect_Origin_EB30)), a.pos);
    return r;
}


/**
 * Puts the rider into a wipeout: rumble, scoring, sounds, impact effect, then the wipeout animation and states.
 *
 * Rumbles (cBeing vtable slot 17, vibrationBump) with the hit strength (+0x30 of @p evt) * 5, takes the boost from
 * cRiderMetrix_evWipeout (On Tour cRiderMetrix::evWipeout(@p isPunch)) and passes it to
 * cRider_flagEventBoostPowerUp(1), plays the hit (cSSXAudio_WipeoutHit), a grunt and the wipeout sound
 * (cSSXAudio_Wipeout), and spawns the impact effect (cRiderHandler_recordSprayBurst) with surface @p surf. CS_Void is set first; a rider
 * sliding sideways on a rail (RailslideMode kRSM_FS or kRSM_BS) is turned +/-90 degrees about its up axis and its anim
 * heading offset follows.
 *
 * It then clears RailslideMode, channels 0, 2 and 8 and PressMode, poses the first frame of @p anim
 * (cRiderAnimBase_getAIAnimFrame, func_0030ECD8), moves the rider so that pose's hip bone (mBoneHips) stays where it
 * is, stores the inverse as the anim orientation offset, plays @p anim and sets CS_Wipeout and MS_Wipeout.
 *
 * @param anim  the wipeout animation (On Tour tAIAnimID wipeoutAnim)
 * @param isPunch  On Tour isPunch: the wipeout came from a punch
 * @param surf  the surface hit (On Tour eSurfaceType surf), for the impact effect
 * @param evt  the collision (On Tour tCollideInfo): contact point/normal at +0x0/+0x20, strength at +0x30
 */
void cRider::flagEventWipeout(int anim, int isPunch, int surf, tCollideInfo* evt)
{
    sVE_EB30* vt = (sVE_EB30*)VPTR_AT(this, sizeof(cRiderState));
    vt[kRiderVt_vibrationBump].fn(vthis(this, vt[kRiderVt_vibrationBump]), evt->Vel * 5.0f);
    float amt = cRiderMetrix_evWipeout(mMetrix, isPunch);
    flagEventBoostPowerUp(1, amt);
    cSSXAudio_WipeoutHit(cAudioInterface_bxAudio(), this);
    cSSXAudio_PlayGrunt(cAudioInterface_bxAudio(), this, 0, 0);
    cSSXAudio_Wipeout(cAudioInterface_bxAudio(), this, amt);
    mRiderHandler->recordSprayBurst(evt, &evt->Reflect, surf, evt->Vel);
    cRider_setControlState_EB30(this, CS_Void);
    if (RailslideMode == kRSM_FS) {
        float ang = 1.5707963705062866f;
        cRider_rotateOrientation(this, GetUp_EB30(this), ang);
        cRiderAnimBase_changeHeadingOffset(mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)mRiderAnim, 0.0f);
    } else if (RailslideMode == kRSM_BS) {
        float ang = -1.5707963705062866f;
        cRider_rotateOrientation(this, GetUp_EB30(this), ang);
        cRiderAnimBase_changeHeadingOffset(mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)mRiderAnim, 0.0f);
    }
    RailslideMode = kRSM_None;
    cRider_verifyRailslideModeDEBUG(this);
    cRider_doLeanPoseAdjust(this, &mPosition);
    cRider_updateOrientationImplicit(this);
    mSlewed[0].target = 0.0f;
    mSlewed[0].cur = 0.0f;
    mSlewed[0].rate = 0.0f;
    mSlewed[2].target = 0.0f;
    mSlewed[2].cur = 0.0f;
    mSlewed[2].rate = 0.0f;
    mSlewed[8].target = 0.0f;
    mSlewed[8].cur = 0.0f;
    mSlewed[8].rate = 0.0f;
    PressMode = kPM_None;
    char buf[80];
    cRiderAnimBase_getAIAnimFrame(buf, mRiderAnim, anim, D_004A4C08.a, mSlewed[0].target);
    sXf_EB30 animPose;
    func_0030ECD8(&animPose, mAnimModel, buf, 0);
    cAnimModel* trk = mAnimModel;
    sQuat_EB30* n = (sQuat_EB30*)((mBoneHips << 4) + (int)trk->localPos);
    sXf_EB30 node;
    node.pos = sQuat_EB30(n->x * trk->ScaleVect.x, n->y * trk->ScaleVect.y,
                          n->z * trk->ScaleVect.z, n->w * trk->ScaleVect.w);
    node.rot = *(sQuat_EB30*)((char*)mAnimModel->localRot + (mBoneHips << 4));
    sXf_EB30 y = Combine_EB30(node, Invert_EB30(animPose));
    *(sXf_EB30*)&mPosition = Combine_EB30(*(sXf_EB30*)&mPosition, y);
    cRider_updateOrientationImplicit(this);
    sXf_EB30 w = Invert_EB30(y);
    cRiderAnimBase_changeOrientationOffset(mRiderAnim, &w);
    cRiderAnimBase_play(mRiderAnim, anim, 0, -1.0f);
    cRider_setControlState_EB30(this, CS_Wipeout);
    cRider_setMotionState_EB30(this, MS_Wipeout);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventWorld);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_0010F1C0 { short delta; short index; int (*fn)(void*); };
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);
extern "C" void cRider_flagEventAddPoints_i(void* self, int v) __asm__("cRider_flagEventAddPoints");
extern "C" void cRider_checkForReset(void* rider, int a, int b);

static inline void computerV_0010F1C0(char* self, int i)
{
    char* obj = self + 0x6C0;
    sVE_0010F1C0* vt = *(sVE_0010F1C0**)obj;
    vt[i].fn(vthis(obj, vt[i]));
}

/**
 * Handles a world (script) event on the rider.
 *
 * @p kind 1-3 call the rider's virtual slots 13, 14 and 15; 4 adds @p amt to the boost meter
 * (cRider_flagEventBoostPowerUp, source mask 8); 5 forces an automatic reset of kind 4 (cRider_checkForReset with 0,
 * 4); 6 adds (int) @p amt points (cRider_flagEventAddPoints); 0 and 7 do nothing. Called by AIEventFunc with a rider from the
 * AI's list.
 *
 * @param kind  the event id
 * @param amt  the event's value
 */
void cRider::flagEventWorld(int kind, float amt)
{
    switch (kind) {
    case 0:
        break;
    case 1:
        computerV_0010F1C0(((char*)this), 13);
        break;
    case 2:
        computerV_0010F1C0(((char*)this), 14);
        break;
    case 3:
        computerV_0010F1C0(((char*)this), 15);
        break;
    case 4:
        flagEventBoostPowerUp(8, amt);
        break;
    case 6:
        cRider_flagEventAddPoints_i(((char*)this), (int)amt);
        break;
    case 5:
        cRider_checkForReset(((char*)this), 0, 4);
        break;
    case 7:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventWipeoutRecover);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_evWipeoutRecover(cRiderMetrix* metrix);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);

/**
 * Tells the rider metrix the rider got up after a wipeout and adds the boost it returns.
 *
 * Calls cRider_flagEventBoostPowerUp with source mask 1 and cRiderMetrix_evWipeoutRecover's result for the rider's mMetrix. The
 * callers' second argument reaches cRiderMetrix_evWipeoutRecover untouched in a1 (the C declaration takes only self).
 */
void cRider::flagEventWipeoutRecover()
{
    flagEventBoostPowerUp(1, cRiderMetrix_evWipeoutRecover(mMetrix));
}
#endif

extern "C" void* cRiderMetrix_evMissionStart(cRiderMetrix* metrix);

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventMissionStart__FPv);
#ifdef SKIP_ASM
/**
 * Tells the rider metrix a mission started: returns cRiderMetrix_evMissionStart on the rider's mMetrix.
 *
 * @return cRiderMetrix_evMissionStart's result
 */
void* cRider::flagEventMissionStart()
{
    return cRiderMetrix_evMissionStart(mMetrix);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventMissionSuccess);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evMissionSuccess(cRiderMetrix* metrix, int money);
extern "C" void cBERewardInterface_awardUserBigChallenge(void*, int, int);

/**
 * Rewards the rider for passing a mission: shows and adds the cash, and records the challenge award.
 *
 * Calls cRiderMetrix_evMissionSuccess (On Tour cRiderMetrix::evMissionSuccess) on the metrix (+0x790) with @p money, then cBERewardInterface_awardUserBigChallenge (On
 * Tour cBERewardInterface::awardUserBigChallenge) on BE interface 0xD with mAIIndex and @p peakID. On Tour adds a third
 * argument (ML) that retail does not have.
 *
 * @param peakID  the peak the mission belongs to (On Tour peakID)
 * @param money  the cash reward (On Tour money)
 */
void cRider::flagEventMissionSuccess(int peakID, int money)
{
    cRiderMetrix_evMissionSuccess(mMetrix, money);
    cBERewardInterface_awardUserBigChallenge(cBE_getInterface_Fv(cBE_getBE(), 13), mAIIndex, peakID);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventAddCollectible);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evAddCash(cRiderMetrix* metrix, int amount);
extern "C" void cBERewardInterface_awardUserCollectible(void*, int, int);

/**
 * Credits the rider with a collected peak collectible: a cash event and the BE collectible reward check.
 *
 * Calls cRiderMetrix_evAddCash(mMetrix, 3) and cBERewardInterface_awardUserCollectible() for the rider's
 * mAIIndex. The C declares no amount, but the caller's third argument (On Tour's money, passed by
 * cWScriptMan_collect) stays in its register and reaches evAddCash as the cash amount.
 *
 * @param peakID  the peak whose collectible was taken (On Tour peakID)
 */
void cRider::flagEventAddCollectible(int peakID)
{
    cRiderMetrix_evAddCash(mMetrix, 3);
    cBERewardInterface_awardUserCollectible(cBE_getInterface_Fv(cBE_getBE(), 13), mAIIndex, peakID);
}
#endif

extern "C" void* cAI_RefreshGroupAI(void* self);

//100%
INCLUDE_ASM("ai/computer", cAI_InitGroupAI__FPv);
#ifdef SKIP_ASM
/**
 * Initialises the group AI: just calls cAI_RefreshGroupAI.
 *
 * Called from cAI_commonInit.
 *
 * @return cAI_RefreshGroupAI's result
 */
void* cAI::InitGroupAI()
{
    return cAI_RefreshGroupAI(this);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_RefreshGroupAI);
#ifdef SKIP_ASM
extern "C" bool cRider_isLoaded(void*);
float cRider_getDistanceLeft(void* self);

// PORT: the unit declares this as returning void*; it returns nothing.
extern "C" void cAI_RefreshGroupAI_impl(cAI* self) __asm__("cAI_RefreshGroupAI");
/**
 * Rebuilds every race rider's relation table (mRelations) after the AI's rider list changes.
 *
 * For rider i and slot j it sets mValid when j != i, j is a race rider and cRider_isLoaded() holds for rider
 * i (not j); fills IsHuman with rider j's isHuman(); sets mDistance to 1e10 and clears mAngle and the
 * five words after it. It then stores the rider's distance left (cRider_getDistanceLeft) in TotalPathDistance and
 * sets PrevRanking to -1.
 */
void cAI::RefreshGroupAI()
{
    int i;
    for (i = 0; i < NumberOfRidersAllocated; i++)
    {
        int j;
        for (j = 0; j < 6; j++)
        {
            sRiderPair* relation = &Riders[i]->mRelations[j];
            int visible = 0;
            if (i != j && j < NumberOfRidersAllocated)
                visible = cRider_isLoaded(Riders[i]) != 0;
            relation->mValid = visible;
            if (j < NumberOfRidersAllocated)
            {
                Riders[i]->mRelations[j].IsHuman = Riders[j]->isHuman();
            }
            Riders[i]->mRelations[j].mDistance = 10000000000.0f;
            Riders[i]->mRelations[j].mAngle = 0.0f;
            Riders[i]->mRelations[j].CollideTick = 0;
            Riders[i]->mRelations[j].OverlapTick = 0;
            Riders[i]->mRelations[j].PunchTick = 0;
            Riders[i]->mRelations[j].mAggressive = 0;
            Riders[i]->mRelations[j].unk_20 = 0;
        }
        int none = -1;
        Riders[i]->TotalPathDistance = cRider_getDistanceLeft(Riders[i]);
        Riders[i]->PrevRanking = none;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateGroupAI);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* agg, int a, int b);
extern "C" float func_0031C228(float x);
extern "C" void cAI_UpdateAggression(cAI* self);
extern "C" void cAI_UpdateRankingPlacement(cAI* self);


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector sub_0010F560(const t4Vector& a, const t4Vector& b)
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

static inline float atan2_0010F560(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += 3.1415927410125732f;
        } else {
            a -= 3.1415927410125732f;
        }
    }
    return a;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_0010F560(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float wrap_0010F560(float x)
{
    return x - ffloor_0010F560(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

/**
 * Updates every rider's view of its opponents every sixth tick: distance, bearing and whether they are aggressive.
 *
 * Runs when AITick % 6 == 0. Calls cAI_UpdateRankingPlacement (On Tour cAI::UpdateRankingPlacement) first, then works on the first
 * NumberOfRidersAllocated - NumberOfActorsAllocated riders (at least 2): each rider's mRubberBandBehind /
 * mRubberBandAhead pair is reset to -50000/50000, and for each valid pair (i, j) (rider i's mRelations[j].mValid set)
 * it stores the planar distance in both records, the bearing from j to i in j's record and the opposite bearing
 * (wrapped to -pi..pi) in i's, and marks the pair aggressive when cBEAggressionInterface_getAggression (On Tour
 * getAggression) is at least 2.
 *
 * Ends with cAI_UpdateAggression (On Tour cAI::UpdateAggression).
 */
void cAI::UpdateGroupAI()
{
    if (AITick % 6 != 0) {
        return;
    }
    UpdateRankingPlacement();
    int n = NumberOfRidersAllocated - NumberOfActorsAllocated;
    if (n < 2) {
        return;
    }
    for (int i = 0; i < n; i++) {
        Riders[i]->mRubberBandBehind = -50000.0f;
        Riders[i]->mRubberBandAhead = 50000.0f;
        for (int j = i + 1; j < n; j++) {
            if (Riders[i]->mRelations[j].mValid == 0) {
                continue;
            }
            t4Vector delta = sub_0010F560(*Riders[i]->getPosition(), *Riders[j]->getPosition());
            float dist;
            float dx = delta.x;
            float dy = delta.y;
            float sq = dx * dx + dy * dy;
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(sq));
            float ang = atan2_0010F560(dy, dx);
            Riders[i]->mRelations[j].mDistance = dist;
            Riders[j]->mRelations[i].mDistance = dist;
            Riders[i]->mRelations[j].mAngle = wrap_0010F560(ang + 3.1415927410125732f);
            Riders[j]->mRelations[i].mAngle = ang;
            if (cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), i, j) >= 2) {
                Riders[i]->mRelations[j].mAggressive = 1;
            } else {
                Riders[i]->mRelations[j].mAggressive = 0;
            }
        }
    }
    UpdateAggression();
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateAggression);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* self, int a, int b);

/**
 * Refreshes each rider's aggression toward the others and picks a chase target.
 *
 * Skipped when NumberOfComputersAllocated is 0. For each rider i of NumberOfRidersAllocated: clears its chase flag
 * (PissedOff) and target (PissedAIIndex = -1), then for every other rider j stores cBEAggressionInterface_getAggression
 * (On Tour cBEAggressionInterface::getAggression(i, j)) in PissedOffType and, at kAGG_CHASE, sets the chase flag and
 * target j. Only the last j's level survives in PissedOffType (likely an oversight).
 */
void cAI::UpdateAggression()
{
    if (NumberOfComputersAllocated <= 0) return;
    for (int i = 0; i < NumberOfRidersAllocated; i++)
    {
        Riders[i]->PissedOff = 0;
        Riders[i]->PissedAIIndex = -1;
        for (int j = 0; j < NumberOfRidersAllocated; j++)
        {
            if (i == j) continue;
            int curAgg = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), i, j);
            if (curAgg == kAGG_CHASE)
            {
                Riders[i]->PissedOff = 1;
                Riders[i]->PissedAIIndex = j;
            }
            Riders[i]->PissedOffType = curAgg;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateRankingPlacement);
#ifdef SKIP_ASM
extern "C" int cAI_isAILoadedAndReady(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_ChangePosition(void* mgr, void* rider, void* next, void* prev, int oldPos, int newPos);
extern "C" void SORT_shell(int n, int* keys, int* idx);
float cRider_getDistanceLeft(void* self);

/**
 * Re-ranks the riders by the AI's RaceRankType and tells the audio system about each change of place.
 *
 * Does nothing for AI_RR_None or before cAI_isAILoadedAndReady. AI_RR_DistToFinish ranks racers by -(distance left + 20
 * x their current place RaceRanking), so a rider must gain 20 units to pass; AI_RR_TotalPoints ranks by the metrix's
 * +0x198 points. Keys are sorted largest first (SORT_shell). For each new place that differs from RaceRanking,
 * cSSXAudio_ChangePosition gets the rider, the riders after and before it and the old and new place; the place is
 * stored in RaceRanking. Actors get key 0xFFFF in AI_RR_DistToFinish, which sorts them ahead of every racer, but
 * -0xFFFF in AI_RR_TotalPoints. The two branches on mFinishTime >= 10 compute the same key.
 *
 */
void cAI::UpdateRankingPlacement()
{
    cAI* self = this;
    if (self->RaceRankType == AI_RR_None)
        return;
    if (cAI_isAILoadedAndReady(self) == 0)
        return;
    int idx[8];
    int keys[8];
    int i;
    for (i = 0; i < self->NumberOfRidersAllocated; i++)
    {
        idx[i] = i;
        cRider* rider = self->Riders[i];
        switch (self->RaceRankType)
        {
        case AI_RR_DistToFinish:
            if (rider->mIsActor)
                keys[i] = 0xFFFF;
            else
            {
                int far = rider->mFinishTime >= 10.0f;
                if (far)
                    keys[i] = -(int)(rider->getDistanceLeft() + (float)(rider->RaceRanking * 20));
                else
                    keys[i] = -(int)(rider->getDistanceLeft() + (float)(rider->RaceRanking * 20));
            }
            break;
        case AI_RR_TotalPoints:
            if (rider->mIsActor)
                keys[i] = -0xFFFF;
            else
                keys[i] = rider->mMetrix->Totals.TotalPoints;
            break;
        }
    }
    // Re-read count into a local: GCSE makes this load redundant with the loop-test
    // loads and leaves a copy (a0 = v0) after each of them.
    int n = self->NumberOfRidersAllocated;
    SORT_shell(n, keys, idx);
    int j;
    for (j = 0; j < self->NumberOfRidersAllocated; j++)
    {
        if (self->Riders[idx[j]]->RaceRanking != j)
        {
            cRider* next = 0;
            cRider* prev = 0;
            if (j > 0)
                prev = self->Riders[idx[j - 1]];
            if (j < self->NumberOfRidersAllocated - 1)
                next = self->Riders[idx[j + 1]];
            cSSXAudio_ChangePosition(cAudioInterface_bxAudio(), self->Riders[idx[j]], next, prev, self->Riders[idx[j]]->RaceRanking, j);
        }
        self->Riders[idx[j]]->RaceRanking = j;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_ridersOnPath);
#ifdef SKIP_ASM
/**
 * Counts the riders whose AI path (cRider::AIPath, the pointer cRider_getAIPath returns) equals @p value.
 *
 * Goes over the AI's NumberOfRidersAllocated Riders. Nothing in the source calls it.
 *
 * @param value  the path to look for (On Tour cAIPath* path; cComputer_scorePath passes one to the similar
 *               cAI_aiRidersOnPath)
 * @return the number of matching riders
 */
int cAI::ridersOnPath(cAIPath* value)
{
    int count = 0;
    for (int i = 0; i < NumberOfRidersAllocated; i++) {
        cRider* rider = Riders[i];
        if (rider->AIPath == value) {
            count++;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_aiRidersOnPath);
#ifdef SKIP_ASM

/**
 * Counts the computer riders whose AI path (cRider::AIPath, the pointer cRider_getAIPath returns) is @p path.
 *
 * A rider counts when its isComputer virtual (slot 9) returns true. cComputer_scorePath uses it.
 *
 * @param path  the path to look for
 * @return the number of matching computer riders
 */
int cAI::aiRidersOnPath(cAIPath* path)
{
    int n = 0;
    for (int i = 0; i < NumberOfRidersAllocated; i++)
    {
        if (Riders[i]->isComputer())
        {
            if (Riders[i]->AIPath == path)
                n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_cRiderHandler);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/computer_classes.h"
#include "replay/bxreplayframe.h"
#include "object/being.h"
#include "world/patchrecord.h"

/**
 * Constructs a computer rider's handler, a composite of 31 rider behaviour parts sharing one virtual cRider base.
 *
 * Compiler-generated g++ 2.95 code builds the cRider base and every part (motion, control and FX parts such as
 * cGroundMotion, cWipeoutMotion, cBoardTrailFX, cPunchFX, cCheatFX) and fixes up their vtables; the body then
 * points mRiderHandler at the object itself and sets the handler's two trailing fields to 6 and 14.
 */
cComp_cmp::cComp_cmp()
{
    mRiderHandler = this;
    MotionState = 6;
    ControlState = 14;
}

// PORT: the compiler names these constructors __<len><Class>[i]; map them to the address-derived names.
__asm__(".globl cRiderHandler_cRiderHandler\n.type cRiderHandler_cRiderHandler,@function\ncRiderHandler_cRiderHandler = __9cComp_cmpi\n.size cRiderHandler_cRiderHandler,0x15DC\n");

__asm__("__12cRiderVb_cmp = cRider_cRider\n"
        "__15cCompPart00_cmpi = cGroundMotion_cGroundMotion\n"
        "__15cCompPart02_cmpi = cWipeoutMotion_cWipeoutMotion\n"
        "__15cCompPart05_cmpi = cHandplantMotion_cHandplantMotion\n"
        "__15cCompPart14_cmpi = cWipeoutControl_cWipeoutControl\n"
        "__15cCompPart20_cmpi = cBoardWakeFX_cBoardWakeFX\n"
        "__15cCompPart21_cmpi = cBoardSparkFX_cBoardSparkFX\n"
        "__15cCompPart22_cmpi = cBoardTrailFX_initialize\n"
        "__15cCompPart23_cmpi = cSpeedBoostFX_cSpeedBoostFX\n"
        "__15cCompPart24_cmpi = cTrickBoostFX_cTrickBoostFX\n"
        "__15cCompPart25_cmpi = cAirStreamerFX_cAirStreamerFX\n"
        "__15cCompPart26_cmpi = cAngerFX_cAngerFX\n"
        "__15cCompPart27_cmpi = cPlayerIndicatorFX_cPlayerIndicatorFX\n"
        "__15cCompPart28_cmpi = cDynamicSprayFX_cDynamicSprayFX\n"
        "__15cCompPart29_cmpi = cPunchFX_cPunchFX\n"
        "__15cCompPart30_cmpi = __8cCheatFXi\n");

// PORT: vtables this TU does not emit (their key functions are elsewhere), under their retail names.
__asm__("_vt$14cRiderVbB0_cmp = D_00459B90\n"
        "_vt$9cComp_cmp = cRiderHandler_vtbl\n"
        "_vt$9cComp_cmp$12cRiderVb_cmp = D_00458780\n"
        "_vt$9cComp_cmp$13cRiderVbC_cmp = D_00458738\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cCompPart01_cmp$13cRiderVbC_cmp = D_004599A0\n"
        "_vt$15cCompPart01_cmp$12cRiderVb_cmp = D_004599E8\n"
        "_vt$15cCompPart03_cmp$13cRiderVbC_cmp = D_004596B8\n"
        "_vt$15cCompPart03_cmp$12cRiderVb_cmp = D_00459700\n"
        "_vt$15cCompPart04_cmp$13cRiderVbC_cmp = D_004598A8\n"
        "_vt$15cCompPart04_cmp$12cRiderVb_cmp = D_004598F0\n"
        "_vt$15cCompPart06_cmp$13cRiderVbC_cmp = D_004594C8\n"
        "_vt$15cCompPart06_cmp$12cRiderVb_cmp = D_00459510\n"
        "_vt$15cCompPart07_cmp$13cRiderVbC_cmp = D_004593D0\n"
        "_vt$15cCompPart07_cmp$12cRiderVb_cmp = D_00459418\n"
        "_vt$15cCompPart08_cmp$13cRiderVbC_cmp = D_004592D8\n"
        "_vt$15cCompPart08_cmp$12cRiderVb_cmp = D_00459320\n"
        "_vt$15cCompPart09_cmp$13cRiderVbC_cmp = D_004591E0\n"
        "_vt$15cCompPart09_cmp$12cRiderVb_cmp = D_00459228\n"
        "_vt$15cCompPart10_cmp$13cRiderVbC_cmp = D_004590E8\n"
        "_vt$15cCompPart10_cmp$12cRiderVb_cmp = D_00459130\n"
        "_vt$15cCompPart11_cmp$13cRiderVbC_cmp = D_00458FF0\n"
        "_vt$15cCompPart11_cmp$12cRiderVb_cmp = D_00459038\n"
        "_vt$15cCompPart12_cmp$13cRiderVbC_cmp = D_00458EF8\n"
        "_vt$15cCompPart12_cmp$12cRiderVb_cmp = D_00458F40\n"
        "_vt$15cCompPart13_cmp$13cRiderVbC_cmp = D_00458E00\n"
        "_vt$15cCompPart13_cmp$12cRiderVb_cmp = D_00458E48\n"
        "_vt$15cCompPart15_cmp$13cRiderVbC_cmp = D_00458A20\n"
        "_vt$15cCompPart15_cmp$12cRiderVb_cmp = D_00458A68\n"
        "_vt$15cCompPart16_cmp$13cRiderVbC_cmp = D_00458B18\n"
        "_vt$15cCompPart16_cmp$12cRiderVb_cmp = D_00458B60\n"
        "_vt$15cCompPart17_cmp$13cRiderVbC_cmp = D_00458830\n"
        "_vt$15cCompPart17_cmp$12cRiderVb_cmp = D_00458878\n"
        "_vt$15cCompPart18_cmp$13cRiderVbC_cmp = D_00458D08\n"
        "_vt$15cCompPart18_cmp$12cRiderVb_cmp = D_00458D50\n"
        "_vt$15cCompPart19_cmp$13cRiderVbC_cmp = D_00458928\n"
        "_vt$15cCompPart19_cmp$12cRiderVb_cmp = D_00458970\n");
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler__setMotionState);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_loseMotionState();
extern "C" void cRiderHandler_gainMotionState(cRiderHandler*, int);

/**
 * Switches the rider handler to a new motion state, notifying the old and new states.
 *
 * Calls cRiderHandler_loseMotionState(), stores @p mstate in MotionState, then calls cRiderHandler_gainMotionState()
 * with the previous state.
 *
 * @param mstate  the new motion state
 */
extern "C" void cRiderHandler__setMotionState(cRiderHandler* self, int mstate)
{
    cRiderHandler_loseMotionState();
    int old = self->MotionState;
    self->MotionState = mstate;
    self->gainMotionState(old);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_loseMotionState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Motion-state dispatch: the second argument is passed through to the handler.
extern "C" void cGroundMotion_loseFocus(void* self, int a1);
void cAirMotion_loseFocus_a1(void* motion, int a1) __asm__("cAirMotion_loseFocus__FPv");
void cWipeoutMotion_loseFocus_a1(void* motion, int a1) __asm__("cWipeoutMotion_loseFocus__FPv");
extern "C" void cRailslideMotion_loseFocus(void* motion, int a1);
extern "C" void cHandplantMotion_loseFocus(void* motion, int a1);
extern "C" void cGroundMotion_gainFocus(void* self, int a1);
extern "C" void cAirMotion_gainFocus(void* motion, int a1);
extern "C" void cWipeoutMotion_gainFocus(void* motion, int a1);
extern "C" void cRailslideMotion_gainFocus(void* motion, int a1);
extern "C" void cHandplantMotion_gainFocus(void* motion, int a1);

/**
 * Tells the current motion part (by MotionState) that it is losing focus, through its loseFocus function.
 *
 * Ground, air, wipeout, railslide and handplant have one; the static motion (MS_Static) and other values do nothing.
 *
 * @param a1  the motion state being switched to, passed on to loseFocus (On Tour next)
 */
void cRiderHandler::loseMotionState(int a1)
{
    switch (MotionState) {
    case MS_Ground:
        cGroundMotion_loseFocus(this, a1);
        break;
    case MS_Air:
        cAirMotion_loseFocus_a1((char*)&AirMotion, a1);
        break;
    case MS_Wipeout:
        cWipeoutMotion_loseFocus_a1((char*)&WipeoutMotion, a1);
        break;
    case MS_Railslide:
        cRailslideMotion_loseFocus((char*)&RailslideMotion, a1);
        break;
    case MS_Handplant:
        cHandplantMotion_loseFocus((char*)&HandplantMotion, a1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_gainMotionState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGroundMotion_loseFocus(void* self, int a1);
void cAirMotion_loseFocus_a1(void* motion, int a1) __asm__("cAirMotion_loseFocus__FPv");
void cWipeoutMotion_loseFocus_a1(void* motion, int a1) __asm__("cWipeoutMotion_loseFocus__FPv");
extern "C" void cRailslideMotion_loseFocus(void* motion, int a1);
extern "C" void cHandplantMotion_loseFocus(void* motion, int a1);
extern "C" void cGroundMotion_gainFocus(void* self, int a1);
extern "C" void cAirMotion_gainFocus(void* motion, int a1);
extern "C" void cWipeoutMotion_gainFocus(void* motion, int a1);
extern "C" void cRailslideMotion_gainFocus(void* motion, int a1);
extern "C" void cHandplantMotion_gainFocus(void* motion, int a1);

/**
 * Tells the new current motion part (by MotionState) that it has gained focus, through its gainFocus
 * function.
 *
 * Ground, air, wipeout, railslide and handplant have one; the static motion (MS_Static) and other values do nothing.
 *
 * @param prev  the motion state being left, passed on to gainFocus
 */
void cRiderHandler::gainMotionState(int prev)
{
    switch (MotionState) {
    case MS_Ground:
        cGroundMotion_gainFocus(this, prev);
        break;
    case MS_Air:
        cAirMotion_gainFocus((char*)&AirMotion, prev);
        break;
    case MS_Wipeout:
        cWipeoutMotion_gainFocus((char*)&WipeoutMotion, prev);
        break;
    case MS_Railslide:
        cRailslideMotion_gainFocus((char*)&RailslideMotion, prev);
        break;
    case MS_Handplant:
        cHandplantMotion_gainFocus((char*)&HandplantMotion, prev);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_stepMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGroundMotion_step(void* self);
extern "C" void cAirMotion_step(void* motion);
extern "C" void cWipeoutMotion_step(void* motion);
extern "C" void cStaticMotion_step(void* motion);
extern "C" void cRailslideMotion_step(void* motion);
extern "C" void cHandplantMotion_step(void* motion);

/**
 * Runs one step of the current motion part, chosen by MotionState (all six motions have a step function).
 */
void cRiderHandler::stepMotion()
{
    switch (MotionState) {
    case MS_Ground:
        cGroundMotion_step(this);
        break;
    case MS_Air:
        cAirMotion_step((char*)&AirMotion);
        break;
    case MS_Wipeout:
        cWipeoutMotion_step((char*)&WipeoutMotion);
        break;
    case MS_Static:
        cStaticMotion_step((char*)&StaticMotion);
        break;
    case MS_Railslide:
        cRailslideMotion_step((char*)&RailslideMotion);
        break;
    case MS_Handplant:
        cHandplantMotion_step((char*)&HandplantMotion);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_resolveMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGroundMotion_resolve(void* self);
extern "C" void cAirMotion_resolve(void* motion);
extern "C" void cWipeoutMotion_resolve(void* motion);
void cStaticMotion_resolve(void* motion);
extern "C" void cRailslideMotion_resolve(void* motion);
void cHandplantMotion_resolve(void* motion);

/**
 * Runs the resolve function of the current motion part, chosen by MotionState (all six motions have one).
 */
void cRiderHandler::resolveMotion()
{
    switch (MotionState) {
    case MS_Ground:
        cGroundMotion_resolve(this);
        break;
    case MS_Air:
        cAirMotion_resolve((char*)&AirMotion);
        break;
    case MS_Wipeout:
        cWipeoutMotion_resolve((char*)&WipeoutMotion);
        break;
    case MS_Static:
        cStaticMotion_resolve((char*)&StaticMotion);
        break;
    case MS_Railslide:
        cRailslideMotion_resolve((char*)&RailslideMotion);
        break;
    case MS_Handplant:
        cHandplantMotion_resolve((char*)&HandplantMotion);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler__setControlState);
#ifdef SKIP_ASM
extern "C" void cRiderHandler_loseControlState();
extern "C" void cRiderHandler_gainControlState(cRiderHandler*, int);

/**
 * Switches the rider handler to a new control state, telling the old and the new control part.
 *
 * Calls cRiderHandler_loseControlState() (declared without arguments, but self and @p cstate are still in the
 * argument registers, so the old part learns the next state), stores @p cstate in ControlState, then calls
 * cRiderHandler_gainControlState() with the previous state.
 *
 * @param cstate  the new control state (eControlState)
 */
extern "C" void cRiderHandler__setControlState(cRiderHandler* self, int cstate)
{
    cRiderHandler_loseControlState();
    int old = self->ControlState;
    self->ControlState = cstate;
    self->gainControlState(old);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_loseControlState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cCruiseControl_loseFocus(void* m, int a1);
extern "C" void cBoardPressControl_loseFocus(void* m, int a1);
void cPrewindControl_loseFocus_a1(void* m, int a1) __asm__("cPrewindControl_loseFocus__FPv");
extern "C" void cNaturalAirControl_loseFocus(void* m, int a1);
extern "C" void cSpinControl_loseFocus(void* m, int a1);
extern "C" void cRailslideControl_loseFocus(void* m, int a1);
extern "C" void cWipeoutControl_loseFocus(void* m, int a1);
extern "C" void cHandplantControl_loseFocus(void* m, int a1);

/**
 * Tells the current control part (by ControlState) that it is losing focus, through its loseFocus function.
 *
 * Only cruise, board press, prewind, natural air, spin, railslide, wipeout and handplant have one; bump,
 * gate, reset, finish line, uber grind and void do nothing.
 *
 * @param a1  the control state being switched to, passed on to loseFocus (On Tour next)
 */
// Controller-state dispatch: the second argument is passed through to the handler.
void cRiderHandler::loseControlState(int a1)
{
    switch (ControlState) {
    case CS_Cruise:
        cCruiseControl_loseFocus((char*)&CruiseControl, a1);
        break;
    case CS_BoardPress:
        cBoardPressControl_loseFocus((char*)&BoardPressControl, a1);
        break;
    case CS_Prewind:
        cPrewindControl_loseFocus_a1((char*)&PrewindControl, a1);
        break;
    case CS_NaturalAir:
        cNaturalAirControl_loseFocus((char*)&NaturalAirControl, a1);
        break;
    case CS_Spin:
        cSpinControl_loseFocus((char*)&SpinControl, a1);
        break;
    case CS_Railslide:
        cRailslideControl_loseFocus((char*)&RailslideControl, a1);
        break;
    case CS_Wipeout:
        cWipeoutControl_loseFocus((char*)&WipeoutControl, a1);
        break;
    case CS_Handplant:
        cHandplantControl_loseFocus((char*)&HandplantControl, a1);
        break;
    case CS_Bump: case CS_Gate: case CS_Reset: case CS_FinishLine: case CS_UberGrind: case CS_Void:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_gainControlState);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void cCruiseControl_gainFocus(void* p);
void cBoardPressControl_gainFocus(void* p);
extern "C" void cPrewindControl_gainFocus(void* p, int a1);
extern "C" void cNaturalAirControl_gainFocus(void* p, int a1);
extern "C" void cSpinControl_gainFocus(void* p, int a1);
extern "C" void cGateControl_gainFocus(void* p, int a1);
extern "C" void cRailslideControl_gainFocus(void* p, int a1);
extern "C" void cWipeoutControl_gainFocus(void* p, int a1);
extern "C" void cResetControl_gainFocus(void* p, int a1);
extern "C" void cFinishLineControl_gainFocus(void* p, int a1);
extern "C" void cHandplantControl_gainFocus(void* p, int a1);
extern "C" void cUberGrindControl_gainFocus(void* p, int a1);
/**
 * Tells the new current control part (by ControlState) that it has gained focus, through its gainFocus
 * function.
 *
 * Every control state but bump (CS_Bump) and void (CS_Void) has one.
 *
 * @param prev  the control state being left, passed on to gainFocus
 */
// Forwards to the handler for the AI's current state.
void cRiderHandler::gainControlState(int prev)
{
    switch (ControlState) {
    case CS_Cruise:
        cCruiseControl_gainFocus((char*)&CruiseControl);
        break;
    case CS_BoardPress:
        cBoardPressControl_gainFocus((char*)&BoardPressControl);
        break;
    case CS_Prewind:
        cPrewindControl_gainFocus((char*)&PrewindControl, prev);
        break;
    case CS_NaturalAir:
        cNaturalAirControl_gainFocus((char*)&NaturalAirControl, prev);
        break;
    case CS_Spin:
        cSpinControl_gainFocus((char*)&SpinControl, prev);
        break;
    case CS_Gate:
        cGateControl_gainFocus((char*)&GateControl, prev);
        break;
    case CS_Railslide:
        cRailslideControl_gainFocus((char*)&RailslideControl, prev);
        break;
    case CS_Wipeout:
        cWipeoutControl_gainFocus((char*)&WipeoutControl, prev);
        break;
    case CS_Reset:
        cResetControl_gainFocus((char*)&ResetControl, prev);
        break;
    case CS_FinishLine:
        cFinishLineControl_gainFocus((char*)&FinishLineControl, prev);
        break;
    case CS_Handplant:
        cHandplantControl_gainFocus((char*)&HandplantControl, prev);
        break;
    case CS_UberGrind:
        cUberGrindControl_gainFocus((char*)&UberGrindControl, prev);
        break;
    case CS_Bump:
    case CS_Void:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_updateControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cCruiseControl_update(cCruiseControl* self, tCruiseCmd* cmd);
extern "C" void cBoardPressControl_update(cBoardPressControl* self, tBoardPressCmd* cmd);
extern "C" void cPrewindControl_update(cPrewindControl* self, tPrewindCmd* cmd);
extern "C" void cBumpControl_update(cBumpControl* self, tBumpCmd* cmd);
extern "C" void cNaturalAirControl_update(cNaturalAirControl* self, tNaturalAirCmd* cmd);
extern "C" void cSpinControl_update(cSpinControl* self, tSpinCmd* cmd);
extern "C" void cGateControl_update(cGateControl* self, tGateCmd* cmd);
extern "C" void cRailslideControl_update(cRailslideControl* self, tRailslideCmd* cmd);
extern "C" void cWipeoutControl_update(cWipeoutControl* self, tWipeoutCmd* cmd);
extern "C" void cResetControl_update(cResetControl* self, tResetCmd* cmd);
extern "C" void cFinishLineControl_update(cFinishLineControl* self, tFinishLineCmd* cmd);
extern "C" void cHandplantControl_update(cHandplantControl* self, tHandplantCmd* cmd);
extern "C" void cUberGrindControl_update(cUberGrindControl* self, tUberGrindCmd* cmd);
/**
 * Feeds this frame's control command to the current control part (by ControlState) through its update
 * function.
 *
 * States CS_Cruise .. CS_UberGrind each have one; the void state (CS_Void) does nothing.
 *
 * @param cmd  the control command (On Tour const tControlCmd&), passed on to the part's update
 */
// Forwards to the second handler for the AI's current state.
void cRiderHandler::updateControl(tControlCmd* cmd)
{
    switch (ControlState) {
    case CS_Cruise:
        cCruiseControl_update(&CruiseControl, &cmd->CruiseCmd);
        break;
    case CS_BoardPress:
        cBoardPressControl_update(&BoardPressControl, &cmd->BoardPressCmd);
        break;
    case CS_Prewind:
        cPrewindControl_update(&PrewindControl, &cmd->PrewindCmd);
        break;
    case CS_Bump:
        cBumpControl_update(&BumpControl, &cmd->BumpCmd);
        break;
    case CS_NaturalAir:
        cNaturalAirControl_update(&NaturalAirControl, &cmd->NaturalAirCmd);
        break;
    case CS_Spin:
        cSpinControl_update(&SpinControl, &cmd->SpinCmd);
        break;
    case CS_Gate:
        cGateControl_update(&GateControl, &cmd->GateCmd);
        break;
    case CS_Railslide:
        cRailslideControl_update(&RailslideControl, &cmd->RailslideCmd);
        break;
    case CS_Wipeout:
        cWipeoutControl_update(&WipeoutControl, &cmd->WipeoutCmd);
        break;
    case CS_Reset:
        cResetControl_update(&ResetControl, &cmd->ResetCmd);
        break;
    case CS_FinishLine:
        cFinishLineControl_update(&FinishLineControl, &cmd->FinishLineCmd);
        break;
    case CS_Handplant:
        cHandplantControl_update(&HandplantControl, &cmd->HandplantCmd);
        break;
    case CS_UberGrind:
        cUberGrindControl_update(&UberGrindControl, &cmd->UberGrindCmd);
        break;
    case CS_Void:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler__getMotionStateName);
#ifdef SKIP_ASM
extern const char* D_00459D70[];
extern char str_Invalid[];

/**
 * Returns the name of the current motion state, "Ground" to "Handplant", for debug display.
 *
 * The bound check accepts 0-6 but the table D_00459D70 has six entries, so MS_Num (6, the constructor's
 * initial value) reads the next data word as a string pointer.
 *
 * @return the state's name, or " -- Invalid -- " for a state above 6
 */
extern "C" const char* cRiderHandler__getMotionStateName(cRiderHandler* self)
{
    unsigned int state = (unsigned int)self->MotionState;
    if (state >= 7U) {
        return str_Invalid;
    }
    return D_00459D70[state];
}
#endif
//100%
INCLUDE_ASM("ai/computer", cRiderHandler__getControlStateName);
#ifdef SKIP_ASM
extern const char* ControlStateNames[];
extern char str_Invalid[];

/**
 * Returns the name of the current control state, "Cruise" to "Void", for debug display.
 *
 * The bound check accepts 0-14 but the table ControlStateNames has 14 entries, so CS_Num (14, the constructor's
 * initial value) reads the next data word as a string pointer.
 *
 * @return the state's name, or " -- Invalid -- " for a state above 14
 */
extern "C" const char* cRiderHandler__getControlStateName(cRiderHandler* self)
{
    unsigned int state = (unsigned int)self->ControlState;
    if (state >= 15U) {
        return str_Invalid;
    }
    return ControlStateNames[state];
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_resetRiderFX);
#ifdef SKIP_ASM
extern "C" void cBoardWakeFX_reset(void*);
extern "C" void cBoardSparkFX_reset(void*);
extern "C" void cBoardTrailFX_reset(void*);
extern "C" void cSpeedBoostFX_reset(void*);
void cTrickBoostFX_reset(void*);
extern "C" void cAirStreamerFX_reset(void*);
extern "C" void cAngerFX_clearViewState(void*);
extern "C" void cPlayerIndicatorFX_reset(void*);
extern "C" void cDynamicSprayFX_reset(void*);
extern "C" void cPunchFX_reset(void*);
void cCheatFX_reset(void*);
// PORT: cWorldPainterMan_reset is defined with an unused self param; this caller passes none.
void cWorldPainterMan_reset_noarg() __asm__("cWorldPainterMan_reset__FPv");
void* cGame_getSplashManager(void* self, int i);


/**
 * Resets all of the rider's visual FX parts, the world painter and the splash manager of the rider's view.
 *
 * Calls the reset entry of each of the 11 FX parts (BoardWake to Cheat; for most parts the function named
 * readFromReplayFrame, called without a stream), then cWorldPainterMan_reset() and, when the rider has a view
 * (mViewIndex >= 0), virtual slot 14 of cGame::SplashManager[mViewIndex].
 */
void cRiderHandler::resetRiderFX()
{
    cBoardWakeFX_reset(&BoardWakeFX);
    cBoardSparkFX_reset(&BoardSparkFX);
    cBoardTrailFX_reset(&BoardTrailFX);
    cSpeedBoostFX_reset(&SpeedBoostFX);
    cTrickBoostFX_reset(&TrickBoostFX);
    cAirStreamerFX_reset(&AirStreamerFX);
    cAngerFX_clearViewState(&AngerFX);
    cPlayerIndicatorFX_reset(&PlayerIndicatorFX);
    cDynamicSprayFX_reset(&DynamicSprayFX);
    cPunchFX_reset(&PunchFX);
    cCheatFX_reset(&CheatFX);
    cWorldPainterMan_reset_noarg();
    int id = GroundMotion.mRider->mViewIndex;
    if (id >= 0)
    {
        cSplashManager* obj = (cSplashManager*)cGame_getSplashManager(SSXApp->Game, id);
        if (((char*)obj) != 0)
        {
            sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)VPTR_OF(cObjNode, obj);
            vt[14].fn(vthis(obj, vt[14]));
        }
    }
}
#endif

extern "C" void* cDynamicSprayFX_recordBurst(void*);

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_updateRiderFX);
#ifdef SKIP_ASM
extern "C" void cAngerFX_update(void*);
extern "C" void cBoardSparkFX_update(void*);
extern "C" void cBoardWakeFX_update(void*);
extern "C" void cDynamicSprayFX_update(void*);
extern "C" void cPlayerIndicatorFX_update(void*);
extern "C" void cSpeedBoostFX_update(void*);
extern "C" void cBoardTrailFX_update(void*);
extern "C" void cTrickBoostFX_update(void*);
extern "C" void cAirStreamerFX_update(void*);
extern "C" void cPunchFX_update(void*);
extern "C" void cPlayerLights_Clear(void*);
extern "C" void cCheatFX_update(void*);
extern int cRenderToggles_m_DisableAIVisualFX;


/**
 * Updates the rider's player lights and all 11 visual FX parts for this frame.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (cRenderToggles_m_DisableAIVisualFX) is on. The lights
 * (PlayerLights, +0xD30) are reset first by cPlayerLights_Clear. On Tour's version takes an a_bIsLocalHuman flag
 * that this one lacks.
 */
void cRiderHandler::updateRiderFX()
{
    if (cRenderToggles_m_DisableAIVisualFX != 0) {
        cRider* rider = GroundMotion.mRider;
        char* riderBeing = (char*)&VPTR_AT(rider, sizeof(cRiderState));
        sVEntry<int(void*)>* isComputerSlot = &(*(sVEntry<int(void*)>**)riderBeing)[kRiderVt_isComputer];
        if (isComputerSlot->fn(vthis(riderBeing, *isComputerSlot)) != 0)
            return;
    }
    cPlayerLights_Clear((char*)&PlayerLights);
    cBoardWakeFX_update((char*)&BoardWakeFX);
    cBoardSparkFX_update((char*)&BoardSparkFX);
    cBoardTrailFX_update((char*)&BoardTrailFX);
    cSpeedBoostFX_update((char*)&SpeedBoostFX);
    cTrickBoostFX_update((char*)&TrickBoostFX);
    cAirStreamerFX_update((char*)&AirStreamerFX);
    cAngerFX_update((char*)&AngerFX);
    cPlayerIndicatorFX_update((char*)&PlayerIndicatorFX);
    cDynamicSprayFX_update((char*)&DynamicSprayFX);
    cPunchFX_update((char*)&PunchFX);
    cCheatFX_update((char*)&CheatFX);
}
#endif
//100%
INCLUDE_ASM("ai/computer", cRiderHandler_renderRiderFX);
#ifdef SKIP_ASM
extern "C" void cAngerFX_render(void*);
extern "C" void cBoardSparkFX_render(void*);
extern "C" void cBoardWakeFX_render(void*);
extern "C" void cDynamicSprayFX_render(void*);
extern "C" void cPlayerIndicatorFX_render(void*);
extern "C" void cSpeedBoostFX_render(void*);
extern "C" void cBoardTrailFX_render(void*);
extern "C" void cTrickBoostFX_render(void*);
extern "C" void cAirStreamerFX_render(void*);
extern "C" void cPunchFX_render(void*);
void cCheatFX_render(void*);
extern int cRenderToggles_m_DisableAIVisualFX;


/**
 * Draws all 11 of the rider's visual FX parts.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (cRenderToggles_m_DisableAIVisualFX) is on.
 */
void cRiderHandler::renderRiderFX()
{
    if (cRenderToggles_m_DisableAIVisualFX != 0) {
        cRider* rider = GroundMotion.mRider;
        char* riderBeing = (char*)&VPTR_AT(rider, sizeof(cRiderState));
        sVEntry<int(void*)>* isComputerSlot = &(*(sVEntry<int(void*)>**)riderBeing)[kRiderVt_isComputer];
        if (isComputerSlot->fn(vthis(riderBeing, *isComputerSlot)) != 0)
            return;
    }
    cBoardWakeFX_render((char*)&BoardWakeFX);
    cBoardSparkFX_render((char*)&BoardSparkFX);
    cBoardTrailFX_render((char*)&BoardTrailFX);
    cSpeedBoostFX_render((char*)&SpeedBoostFX);
    cTrickBoostFX_render((char*)&TrickBoostFX);
    cAirStreamerFX_render((char*)&AirStreamerFX);
    cAngerFX_render((char*)&AngerFX);
    cPlayerIndicatorFX_render((char*)&PlayerIndicatorFX);
    cDynamicSprayFX_render((char*)&DynamicSprayFX);
    cPunchFX_render((char*)&PunchFX);
    cCheatFX_render((char*)&CheatFX);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_recordSprayBurst__FPv);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The project names this cRiderHandler_recordSprayBurst__FPv (one
// void* param), but every caller passes (self, vecA, vecB, int, float) and the
// body forwards them to cDynamicSprayFX_recordBurst with an extra 0 flag. The unit declares
// cDynamicSprayFX_recordBurst as (void*), so both are bound by asm label.
void cDynamicSprayFX_recordBurst_impl(void* self, void* a, void* b, int c, int flag, float x) __asm__("cDynamicSprayFX_recordBurst");

void cRiderHandler_recordSprayBurst_impl(void* self, void* a, void* b, int c, float x) __asm__("cRiderHandler_recordSprayBurst__FPv");

/**
 * Spawns the rider handler's landing / impact effect: forwards to cDynamicSprayFX_recordBurst on the effect at self + 0xB40.
 *
 * Callers pass the cRiderHandler as self; the extra 0 flag is inserted before @p x.
 *
 * @param pos  the contact position
 * @param dir  the contact normal
 * @param surfaceType  the surface type
 * @param x  the impact speed
 */
void cRiderHandler::recordSprayBurst(void* pos, void* dir, int surfaceType, float x)
{
    cDynamicSprayFX_recordBurst_impl(&DynamicSprayFX, pos, dir, surfaceType, 0, x);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_writeToReplayFrame);
#ifdef SKIP_ASM
extern char gPaths[];
extern "C" void cRiderAnim_writeToReplayFrame(void* self, void* s);
extern "C" void cAnimModel_writeToReplayFrame(void* self, void* s);
extern "C" void cAirPredictor_writeToReplayFrame(void* self, void* s);
extern "C" void cRiderMetrix_writeToReplayFrame(void* self, void* s);
extern "C" void cGroundMotion_writeToReplayFrame(void* self, void* s);
extern "C" void cAirMotion_writeToReplayFrame(void* self, void* s);
extern "C" void cWipeoutMotion_writeToReplayFrame(void* self, void* s);
extern "C" void cRailslideMotion_writeToReplayFrame(void* self, void* s);
extern "C" void cHandplantMotion_writeToReplayFrame(void* self, void* s);
extern "C" void cBoardPressControl_writeToReplayFrame(void* self, void* s);
extern "C" void cNaturalAirControl_writeToReplayFrame(void* self, void* s);
extern "C" void cSpinControl_writeToReplayFrame(void* self, void* s);
extern "C" void cGateControl_writeToReplayFrame(void* self, void* s);
extern "C" void cRailslideControl_writeToReplayFrame(void* self, void* s);
extern "C" void cWipeoutControl_writeToReplayFrame(void* self, void* s);
extern "C" void cResetControl_writeToReplayFrame(void* self, void* s);
extern "C" void cFinishLineControl_writeToReplayFrame(void* self, void* s);
extern "C" void cHandplantControl_writeToReplayFrame(void* self, void* s);
extern "C" void cUberGrindControl_writeToReplayFrame(void* self, void* s);
extern "C" void cCheatFX_writeToReplayFrame(void* self, void* s);
extern "C" int cPathSys_getTrackPathIndex(void* tree, void* path);
extern "C" int cPathSys_getAIPathIndex(void* tree, void* path);

/**
 * Writes the rider's state into one replay frame: the cRider, its helpers, every stateful part and its paths.
 *
 * In order: the first 0x6C0 bytes of the cRider; the rider anim, anim model, air predictor and metrix; the
 * line and wall hints (each the m_RID of the hint's patch, or -1, then the hint's 16 bytes); MotionState and
 * ControlState; each motion and control part that has state, and cCheatFX; 0x20 bytes each of mAIPathHint and
 * mTrackPathHint; and the IDs of the two paths (TrackPath and AIPath).
 *
 * @param data  the replay stream (On Tour cReplayFramePtr*), written through its virtual Write
 */
void cRiderHandler::writeToReplayFrame(cBxReplayFrame* data)
{
    cBxReplayFrame* st = data;
    st->write((const char*)GroundMotion.mRider, 1728);
    cRiderAnim_writeToReplayFrame(GroundMotion.mRider->mRiderAnim, data);
    cAnimModel_writeToReplayFrame(GroundMotion.mRider->mAnimModel, data);
    cAirPredictor_writeToReplayFrame(GroundMotion.mRider->mAirPredictor, data);
    cRiderMetrix_writeToReplayFrame(GroundMotion.mRider->mMetrix, data);
    unsigned int id0 = 0xFFFFFFFF;
    // The hint pointer, its mPatch and the patch's m_RID are read through plain pointers: direct member reads move
    // the rider load above the id0 store.
    tIntHint** lineHint = &GroundMotion.mRider->mLineHint;
    tPatchRecord** linePatch = &(*lineHint)->mPatch;
    tPatchRecord* line = *linePatch;
    if (line != 0) {
        unsigned int* lineId = &line->m_RID;
        id0 = *lineId;
    }
    st->write((const char*)&id0, 4);
    st->write((const char*)GroundMotion.mRider->mLineHint, 0x10);
    unsigned int id1 = 0xFFFFFFFF;
    tIntHint** wallHint = &GroundMotion.mRider->mWallHint;
    tPatchRecord** wallPatch = &(*wallHint)->mPatch;
    tPatchRecord* wall = *wallPatch;
    if (wall != 0) {
        unsigned int* wallId = &wall->m_RID;
        id1 = *wallId;
    }
    st->write((const char*)&id1, 4);
    st->write((const char*)GroundMotion.mRider->mWallHint, 0x10);
    st->write((const char*)&MotionState, 4);
    st->write((const char*)&ControlState, 4);
    cGroundMotion_writeToReplayFrame(&GroundMotion, data);
    cAirMotion_writeToReplayFrame(&AirMotion, data);
    cWipeoutMotion_writeToReplayFrame(&WipeoutMotion, data);
    cRailslideMotion_writeToReplayFrame(&RailslideMotion, data);
    cHandplantMotion_writeToReplayFrame(&HandplantMotion, data);
    cBoardPressControl_writeToReplayFrame(&BoardPressControl, data);
    cNaturalAirControl_writeToReplayFrame(&NaturalAirControl, data);
    cSpinControl_writeToReplayFrame(&SpinControl, data);
    cGateControl_writeToReplayFrame(&GateControl, data);
    cRailslideControl_writeToReplayFrame(&RailslideControl, data);
    cWipeoutControl_writeToReplayFrame(&WipeoutControl, data);
    cResetControl_writeToReplayFrame(&ResetControl, data);
    cFinishLineControl_writeToReplayFrame(&FinishLineControl, data);
    cHandplantControl_writeToReplayFrame(&HandplantControl, data);
    cUberGrindControl_writeToReplayFrame(&UberGrindControl, data);
    cCheatFX_writeToReplayFrame(&CheatFX, data);
    st->write((const char*)GroundMotion.mRider->mAIPathHint, 0x20);
    st->write((const char*)GroundMotion.mRider->mTrackPathHint, 0x20);
    int ids[2];
    ids[0] = cPathSys_getTrackPathIndex(gPaths, GroundMotion.mRider->TrackPath);
    ids[1] = cPathSys_getAIPathIndex(gPaths, GroundMotion.mRider->AIPath);
    st->write((const char*)ids, 8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_readFromReplayFrame);
#ifdef SKIP_ASM
#include "world/worldcache.h"

extern char gPaths[];
extern "C" void cRiderAnim_readFromReplayFrame(void* self, void* s);
extern "C" void cAnimModel_readFromReplayFrame(void* self, void* s);
extern "C" void cAirPredictor_readFromReplayFrame(void* self, void* s);
extern "C" void cRiderMetrix_readFromReplayFrame(void* self, void* s);
extern "C" void cGroundMotion_readFromReplayFrame(void* self, void* s);
extern "C" void cAirMotion_readFromReplayFrame(void* self, void* s);
extern "C" void cWipeoutMotion_readFromReplayFrame(void* self, void* s);
extern "C" void cRailslideMotion_readFromReplayFrame(void* self, void* s);
extern "C" void cHandplantMotion_readFromReplayFrame(void* self, void* s);
extern "C" void cBoardPressControl_readFromReplayFrame(void* self, void* s);
extern "C" void cNaturalAirControl_readFromReplayFrame(void* self, void* s);
extern "C" void cSpinControl_readFromReplayFrame(void* self, void* s);
extern "C" void cGateControl_readFromReplayFrame(void* self, void* s);
extern "C" void cRailslideControl_readFromReplayFrame(void* self, void* s);
extern "C" void cWipeoutControl_readFromReplayFrame(void* self, void* s);
extern "C" void cResetControl_readFromReplayFrame(void* self, void* s);
extern "C" void cFinishLineControl_readFromReplayFrame(void* self, void* s);
extern "C" void cHandplantControl_readFromReplayFrame(void* self, void* s);
extern "C" void cUberGrindControl_readFromReplayFrame(void* self, void* s);
extern "C" void cBoardWakeFX_reset(void*);
extern "C" void cBoardSparkFX_reset(void*);
extern "C" void cBoardTrailFX_reset(void*);
extern "C" void cSpeedBoostFX_reset(void*);
void cTrickBoostFX_reset(void*);
extern "C" void cAirStreamerFX_reset(void*);
extern "C" void cAngerFX_clearViewState(void*);
extern "C" void cPlayerIndicatorFX_reset(void*);
extern "C" void cPunchFX_reset(void*);
extern "C" void cCheatFX_readFromReplayFrame(void* self, void* s);
extern "C" void* cPathSys_getTrackPath(void* tree, int id);
extern "C" void* cPathSys_getAIPath(void* tree, int id);
extern "C" void cRiderAnimBase_setupAnimation(void* self, int a1);
extern "C" void cAnimModel_buildSkelMatrices(void* self);
extern "C" void cRider_updateCollideInfo(void* self);

static inline tPatchRecord* idxToPatch(unsigned int i)
{
    return (tPatchRecord*)(i << 2);
}

// A cache tag's record: the tag keeps its address >> 2 (0 = not loaded).
static inline tPatchRecord* tagToPatch(unsigned int memLoc)
{
    if (memLoc == 0) {
        return 0;
    }
    return idxToPatch(memLoc);
}

// A saved resource id: EA's local starts as tResourceID::Invalid() (retail stores -1 before reading it).
struct tSavedResourceID : public tResourceID {
    tSavedResourceID() : tResourceID(0xFFFFFFFF) {}
};

// The patch record that resource id @p id names, or 0 when its section is not loaded or its slot is empty: a section's
// cache table keeps each patch (type 1) as its address >> 2 (EA's inline world-cache lookup).
static inline tPatchRecord* getPatchRecord(tResourceID& id)
{
    cWorldCacheTable* table = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id.m_uiRID & 0xFF];
    if (table == 0) {
        return 0;
    }
    return tagToPatch(table->m_pCacheTable[1].m_pDataTag[id.m_uiRID >> 8].m_bfuiMemLoc);
}


/**
 * Restores the rider from one replay frame, reading back what cRiderHandler_writeToReplayFrame() wrote.
 *
 * Turns the saved world-object IDs back into hint patches (through the world cache) and the path IDs into
 * TrackPath and AIPath, resets the FX parts (all but the dynamic spray, which resetRiderFX() does
 * reset) and reads cCheatFX. It then copies mPosition into the focus box when there is one, updates the rider
 * anim (cRiderAnimBase_setupAnimation), rebuilds the skeleton matrices and refreshes the body spheres (cRider_updateCollideInfo).
 *
 * @param data  the replay stream (On Tour cReplayFramePtr*), read through its virtual Read
 */
void cRiderHandler::readFromReplayFrame(cBxReplayFrame* data)
{
    cBxReplayFrame* st = data;
    st->read((char*)GroundMotion.mRider, 1728);
    cRiderAnim_readFromReplayFrame(GroundMotion.mRider->mRiderAnim, data);
    cAnimModel_readFromReplayFrame(GroundMotion.mRider->mAnimModel, data);
    cAirPredictor_readFromReplayFrame(GroundMotion.mRider->mAirPredictor, data);
    cRiderMetrix_readFromReplayFrame(GroundMotion.mRider->mMetrix, data);
    int ids[2];
    tSavedResourceID lineId;
    st->read((char*)&lineId, 4);
    st->read((char*)GroundMotion.mRider->mLineHint, 0x10);
    if (~lineId.m_uiRID != 0) {
        GroundMotion.mRider->mLineHint->mPatch = getPatchRecord(lineId);
    }
    tSavedResourceID wallId;
    st->read((char*)&wallId, 4);
    st->read((char*)GroundMotion.mRider->mWallHint, 0x10);
    if (~wallId.m_uiRID != 0) {
        GroundMotion.mRider->mWallHint->mPatch = getPatchRecord(wallId);
    }
    st->read((char*)&MotionState, 4);
    st->read((char*)&ControlState, 4);
    cGroundMotion_readFromReplayFrame(((char*)this), data);
    cAirMotion_readFromReplayFrame(&AirMotion, data);
    cWipeoutMotion_readFromReplayFrame(&WipeoutMotion, data);
    cRailslideMotion_readFromReplayFrame(&RailslideMotion, data);
    cHandplantMotion_readFromReplayFrame(&HandplantMotion, data);
    cBoardPressControl_readFromReplayFrame(&BoardPressControl, data);
    cNaturalAirControl_readFromReplayFrame(&NaturalAirControl, data);
    cSpinControl_readFromReplayFrame(&SpinControl, data);
    cGateControl_readFromReplayFrame(&GateControl, data);
    cRailslideControl_readFromReplayFrame(&RailslideControl, data);
    cWipeoutControl_readFromReplayFrame(&WipeoutControl, data);
    cResetControl_readFromReplayFrame(&ResetControl, data);
    cFinishLineControl_readFromReplayFrame(&FinishLineControl, data);
    cHandplantControl_readFromReplayFrame(&HandplantControl, data);
    cUberGrindControl_readFromReplayFrame(&UberGrindControl, data);
    cBoardWakeFX_reset(&BoardWakeFX);
    cBoardSparkFX_reset(&BoardSparkFX);
    cBoardTrailFX_reset(&BoardTrailFX);
    cSpeedBoostFX_reset(&SpeedBoostFX);
    cTrickBoostFX_reset(&TrickBoostFX);
    cAirStreamerFX_reset(&AirStreamerFX);
    cAngerFX_clearViewState(&AngerFX);
    cPlayerIndicatorFX_reset(&PlayerIndicatorFX);
    cPunchFX_reset(&PunchFX);
    cCheatFX_readFromReplayFrame(&CheatFX, data);
    st->read((char*)GroundMotion.mRider->mAIPathHint, 0x20);
    st->read((char*)GroundMotion.mRider->mTrackPathHint, 0x20);
    st->read((char*)ids, 8);
    GroundMotion.mRider->TrackPath = (cTrackPath*)cPathSys_getTrackPath(gPaths, ids[0]);
    GroundMotion.mRider->AIPath = (cAIPath*)cPathSys_getAIPath(gPaths, ids[1]);
    cFocusBox* focusBox = (cFocusBox*)GroundMotion.mRider->mFocusBox;
    if (focusBox != 0) {
        focusBox->Position = GroundMotion.mRider->mPosition;
    }
    cRiderAnimBase_setupAnimation(GroundMotion.mRider->mRiderAnim, 0);
    cAnimModel_buildSkelMatrices(GroundMotion.mRider->mAnimModel);
    cRider_updateCollideInfo(GroundMotion.mRider);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_initPath);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
extern "C" float cRider_getStartGateHorizOffset(void* self);


void cRider_putAt_112180(void* self, t4Vector* a, t4Vector* b, int n, float t) __asm__("cRider_putAt");
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
t4Vector cPath_getPathPos_112180(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_112180(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_112180(t4Vector& dst, t4Vector b)
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
        : "m"(dst), "m"(b));
}

/**
 * Attaches the rider to its start point's track and AI paths, optionally placing it at the start gate first.
 *
 * Gets start point mAIIndex of type 1 from gPaths (cPathSys_getStartpoint, On Tour cPathSys::getStartpoint). With @p flag the
 * rider is put at the start position moved along the start direction by cRider_getStartGateHorizOffset (On Tour
 * getStartGateHorizOffset), facing that direction, with AI anim 5 (cRider_putAt, putAt).
 *
 * Then the path hints (mAIPathHint, mTrackPathHint) are reset (NodeID = -1), the start point's InitTrackPath and
 * InitAIPath are stored in TrackPath / AIPath, the distance to the finish (mDistToFinish and MinDistanceLeft) is the
 * track length (+0x38) minus the rider's position on the track path (0 if past the end), the AI path position goes to
 * PrevPathParam / PathParam, and IntersectPos is the AI path point 796 units ahead.
 *
 * @param flag  On Tour resetPosition: also move the rider to the start gate
 */
void cRider::initPath(int flag)
{
    tPathStartpoint* startPoint = (tPathStartpoint*)cPathSys_getStartpoint(gPaths, 1, mAIIndex);
    if (flag)
    {
        t4Vector dir;
        t4Vector pos;
        dir.x = startPoint->Dir.x;
        dir.y = startPoint->Dir.y;
        dir.z = startPoint->Dir.z;
        dir.w = 0.0f;
        pos.x = startPoint->Pos.x;
        pos.y = startPoint->Pos.y;
        pos.z = startPoint->Pos.z;
        pos.w = 1.0f;
        vu0AddEq_112180(pos, vu0Scale_112180(dir, cRider_getStartGateHorizOffset((char*)this)));
        cRider_putAt_112180(((char*)this), &pos, &dir, 5, 0.0f);
    }
    mAIPathHint->NodeID = -1;
    mTrackPathHint->NodeID = -1;
    TrackPath = startPoint->InitTrackPath;
    AIPath = startPoint->InitAIPath;
    t4Vector up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    up.w = 1.0f;
    float out;
    float res = cPath_intersectPath1(TrackPath, &mPosition, &up, &out, mTrackPathHint, 1);
    float trackLen = TrackPath->BaseDistanceLeft;
    if (trackLen < res)
        res = 0.0f;
    mDistToFinish = MinDistanceLeft = trackLen - res;
    res = cPath_intersectPath1(AIPath, &mPosition, &PathPos, &PathDistXY, mAIPathHint, 1);
    PrevPathParam = PathParam = res;
    t4Vector pos2 = cPath_getPathPos_112180(AIPath, 0, res + 796.0f);
    *(&IntersectPos) = pos2;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_updateTrackPath);
#ifdef SKIP_ASM
extern tPathEventNode cRider_updateTrackPath_pathEventsPrev[];
extern "C" void* MEM_copy(void*, void*, int);
extern "C" void cRider_checkTrackPathEvents(cRider* self, float* a, float* b, t4Vector* c);
extern "C" void cRider_checkTrackPathSwitch(cRider* self, int a);
extern "C" float cPath_getLength(void* self);


/**
 * Tracks the rider along its track path: switches path when needed and raises the path events it enters and leaves.
 *
 * Copies the rider's current event list (PathEventsCur, NumPathEventsCur entries) to
 * cRider_updateTrackPath_pathEventsPrev, then cRider_checkTrackPathEvents (On Tour checkTrackPathEvents) gives the
 * distance from the path, the path position and point. While the rider has distance left (MinDistanceLeft > 0) it
 * calls cRider_checkTrackPathSwitch (On Tour checkTrackPathSwitch) with 0 when more than 500 off the path (only
 * every 60th AITick), with 1 within 200 of the path's end, or with 0 when the path's vtable slot 2 says to; a path
 * change resets mTrackPathHint and re-measures.
 *
 * Each event in the new list is reported with cRider_flagEventPathEvent as current, flagged as previous too
 * when it was in the old list; old events no longer present are reported as previous only.
 */
void cRider::updateTrackPath()
{
    MEM_copy(cRider_updateTrackPath_pathEventsPrev, PathEventsCur, NumPathEventsCur << 4);
    int n = NumPathEventsCur;
    t4Vector pathPos;
    float pathDistXY;
    float pathParam;
    checkTrackPathEvents(&pathDistXY, &pathParam, &pathPos);
    if (0.0f < MinDistanceLeft)
    {
        void* old = TrackPath;
        if (500.0f < pathDistXY && SSXApp->Game->AI->AITick % 60 == 0)
            this->checkTrackPathSwitch(0);
        else if (cPath_getLength(TrackPath) - 200.0f < pathParam)
            this->checkTrackPathSwitch(1);
        else
        {
            cTrackPath* trackPath = TrackPath;
            sVEntry<int(void*, float, float)>* vtable = (sVEntry<int(void*, float, float)>*)trackPath->mVtbl;
            sVEntry<int(void*, float, float)>* ve = &vtable[2];
            if (ve->fn(vthis(trackPath, *ve), MinDistanceLeft, mDistToFinish))
                this->checkTrackPathSwitch(0);
        }
        if ((void*)TrackPath != old)
        {
            mTrackPathHint->NodeID = -1;
            checkTrackPathEvents(&pathDistXY, &pathParam, &pathPos);
        }
    }
    int i;
    for (i = 0; i < NumPathEventsCur; i++)
    {
        int j;
        for (j = 0; j < n; j++)
        {
            if (PathEventsCur[i].Type == cRider_updateTrackPath_pathEventsPrev[j].Type
                && PathEventsCur[i].Arg == cRider_updateTrackPath_pathEventsPrev[j].Arg)
            {
                cRider_updateTrackPath_pathEventsPrev[j].Type = 0;
                break;
            }
        }
        flagEventPathEvent(&PathEventsCur[i], j < n, 1);
    }
    for (i = 0; i < n; i++)
    {
        if (cRider_updateTrackPath_pathEventsPrev[i].Type)
            flagEventPathEvent(&cRider_updateTrackPath_pathEventsPrev[i], 1, 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_calculateAheadDist);
#ifdef SKIP_ASM
/**
 * Returns how far ahead along the path the rider aims: the normal look-ahead or a short one.
 *
 * @param useNormalDistance  On Tour UseNormalDistance: use the normal look-ahead distance
 * @return 796.0 when @p useNormalDistance is set, else 200.0
 */
float cRider::calculateAheadDist(int useNormalDistance)
{
    float aheadDist = 796.0f;
    if (useNormalDistance == 0) {
        aheadDist = 200.0f;
    }
    return aheadDist;
}
#endif

// R5900 128-bit GPR quadword, for functions that copy/return a 16-byte
// block via a single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/computer", cRider_setAimTarget);
#ifdef SKIP_ASM
/**
 * Makes the path look-ahead point the rider's aim target.
 *
 * Copies the 16-byte vector at +0x4A0 (the look-ahead point cRider_initPath and the path updates set) to +0x4B0. On
 * Tour's version takes a UseNormalDistance bool it never reads; retail's has no argument.
 *
 * @return the copied vector (an artefact of the C; the caller ignores it)
 */
extern "C" cQuad128 cRider_setAimTarget(cRider* self)
{
    cQuad128 aimPoint = *(cQuad128*)&self->IntersectPos;
    *(cQuad128*)&self->AimTargetPos = aimPoint;
    return aimPoint;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_isPipeJump__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the rider is doing a pipe jump; a stub that always says no.
 *
 * @return always 0
 */
int cRider::isPipeJump()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_updateAIPath);
#ifdef SKIP_ASM


// PORT: the unit defines cRider_isPipeJump as int; here its result is used as a bool (xori 1).
bool cRider_isPipeJump_b(void* self) __asm__("cRider_isPipeJump__FPv");
// PORT: the unit defines cRider_setAimTarget(void* self); callers pass a second arg in $5.
cQuad128 cRider_setAimTarget_2(void* self, int a) __asm__("cRider_setAimTarget");
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
t4Vector cPath_getPathPos_1125C0(void* path, int* inRange, float t) __asm__("cPath_getPathPos");
extern "C" float cPath_getLength(void* self);
extern "C" void cRider_checkAIPathSwitch(cRider* self, int a);

static inline float atan2_001125C0(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += 3.1415927410125732f;
        } else {
            a -= 3.1415927410125732f;
        }
    }
    return a;
}

/**
 * Follows the rider along its AI path: position on it, look-ahead aim point, path switching and the heading to aim for.
 *
 * Keeps the previous path position (+0x4C0 = +0x4C4), then intersects the rider's position (+0x110) with the AI path
 * (+0xAB8): cPath_intersectPath1 with mAIPathHint normally, or the hint-less cPath_intersectPath for a pipe jump (cRider_isPipeJump__FPv,
 * which always says no). The closest point goes to +0x490 and the distance off the path to +0x4C8. The aim point +0x4A0
 * is the path point cRider_calculateAheadDist further on, and cRider_setAimTarget copies it to the
 * aim target.
 *
 * While the rider still has distance left (+0x4D4 > 0) it calls cRider_checkAIPathSwitch with 0 when more than
 * 500 off the path (only every 60th AITick), with 1 within 200 of the path's end, or with 0 when the path's vtable slot
 * 2 says so. Finally +0x4CC = atan2 of the aim point minus the closest point (x/y).
 */
void cRider::updateAIPath()
{
    PrevPathParam = PathParam;
    int nf = !cRider_isPipeJump_b((char*)this);
    if (nf)
        PathParam = cPath_intersectPath1(AIPath, &mPosition, &PathPos, &PathDistXY, mAIPathHint, nf);
    else
        PathParam = cPath_intersectPath(AIPath, &mPosition, &PathPos, &PathDistXY, 0);
    float k = calculateAheadDist(nf);
    *(&IntersectPos) = cPath_getPathPos_1125C0(AIPath, 0, PathParam + k);
    cRider_setAimTarget_2(((char*)this), nf);
    if (0.0f < MinDistanceLeft)
    {
        if (500.0f < PathDistXY && SSXApp->Game->AI->AITick % 60 == 0)
            checkAIPathSwitch(0);
        else if (cPath_getLength(AIPath) - 200.0f < PathParam)
            checkAIPathSwitch(1);
        else
        {
            cAIPath* aiPath = AIPath;
            sVEntry<int(void*, float, float)>* vtable = (sVEntry<int(void*, float, float)>*)aiPath->mVtbl;
            sVEntry<int(void*, float, float)>* ve = &vtable[2];
            if (ve->fn(vthis(aiPath, *ve), PrevPathParam, PathParam))
                checkAIPathSwitch(0);
        }
    }
    float dx = IntersectPos.x - PathPos.x;
    float dy = IntersectPos.y - PathPos.y;
    PathHeading = atan2_001125C0(dy, dx);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_checkTrackPathSwitch);
#ifdef SKIP_ASM

extern char gPaths[];
extern "C" int cPathSys_getNearTrackPaths(void* tree, void* pos, cTrackPath** out, int maxOut);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
t4Vector cPath_getPathPos_1127F0(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_1127F0(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_1127F0(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_1127F0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_1127F0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_1127F0(const t4Vector& a, const t4Vector& b)
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

/**
 * Picks the best track path near the rider, optionally ignoring the current one.
 *
 * The probe point is the rider's position (+0x110) moved along its velocity (mVelocity) by the normal look-ahead
 * (cRider_calculateAheadDist(self, 1)), or the position itself when standing still. Up to 3 nearby track paths come from
 * cPathSys_getNearTrackPaths (On Tour cPathSys::getNearTrackPaths). Each (except the current one when @p skipCur) is scored by the
 * squared distance from the rider to its nearest point plus that from the probe to the point 796 further along; the
 * lowest wins and becomes the track path (+0xAB4).
 *
 * For the current path, a position more than 1592 away from the expected distance-to-finish (+0x4D0) resets
 * mTrackPathHint.
 *
 * @param skipCur  On Tour mustSwitch: never keep the current track path
 */
void cRider::checkTrackPathSwitch(int skipCur)
{
    cTrackPath* paths[3];
    t4Vector out;
    t4Vector pos;
    float len = vu0Length_1127F0(*(&mVelocity));
    void* best = TrackPath;
    float bestD = 9.999999933815813e+36f;
    if (len == 0.0f) {
        pos = *(&mPosition);
    } else {
        float aheadScale = calculateAheadDist(1) / len;
        pos = vu0Add_1127F0(*(&mPosition), vu0Scale_1127F0(*(&mVelocity), aheadScale));
    }
    int n = cPathSys_getNearTrackPaths(gPaths, &pos, paths, 3);
    for (int i = 0; i < n; i++) {
        if (skipCur && paths[i] == (void*)TrackPath) {
            continue;
        }
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], &pos, &out, &tOut, 1);
        if (paths[i] == (void*)TrackPath) {
            if (1592.0f < __builtin_fabsf(paths[i]->BaseDistanceLeft - mDistToFinish - nearParam)) {
                mTrackPathHint->NodeID = -1;
            }
        }
        nearParam += 796.0f;
        t4Vector aheadDelta = vu0Sub_1127F0(pos, cPath_getPathPos_1127F0(paths[i], 0, nearParam));
        t4Vector nearDelta = vu0Sub_1127F0(*(&mPosition), out);
        float dist = vu0Dot_1127F0(nearDelta, nearDelta) + vu0Dot_1127F0(aheadDelta, aheadDelta);
        if (dist < bestD) {
            best = paths[i];
            bestD = dist;
        }
    }
    TrackPath = (cTrackPath*)best;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_checkAIPathSwitch);
#ifdef SKIP_ASM

extern char gPaths[];
extern "C" int cPathSys_getNearAIPaths(void* tree, void* pos, void** out, int maxOut, int a4);
extern "C" float cPath_getLength(void* self);
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
t4Vector cPath_getPathPos_112A50(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_112A50(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_112A50(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_112A50(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_112A50(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_112A50(const t4Vector& a, const t4Vector& b)
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


/**
 * Picks a better AI path near the rider and moves the rider onto it.
 *
 * The probe point is the rider's position moved 796 along its velocity (or the position when still). Up to 6 nearby AI
 * paths come from cPathSys_getNearAIPaths (On Tour cPathSys::getNearAIPaths); a path is skipped when it is the current
 * one and @p keep is set, or when the rider is within 200 of its end. A rider whose vtable slot 9 (isComputer) returns
 * true picks the path with the highest slot-20 score (cComputer_scorePath; cPlayer_scorePath returns 0), others the one
 * whose near and look-ahead points are closest. A winner equal to the current path counts as no change.
 *
 * On a change: the AI path (+0xAB8) is replaced, mAIPathHint reset, and the path position (+0x4C4 and +0x4C0), closest
 * point (+0x490), distance off the path (+0x4C8) and aim point (+0x4A0, 796 ahead) are recomputed.
 *
 * @param keep  On Tour mustSwitch: do not consider the current AI path
 */
void cRider::checkAIPathSwitch(int keep)
{
    void* paths[6];
    t4Vector out;
    t4Vector pos;
    float len = vu0Length_112A50(*(&mVelocity));
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    float bestW = -1.0f;
    if (len == 0.0f) {
        pos = *(&mPosition);
    } else {
        pos = vu0Add_112A50(*(&mPosition), vu0Scale_112A50(*(&mVelocity), 796.0f / len));
    }
    int n = cPathSys_getNearAIPaths(gPaths, &pos, paths, 6, 0);
    for (int i = 0; i < n; i++) {
        if (paths[i] == (void*)AIPath && keep) {
            continue;
        }
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], &pos, &out, &tOut, 1);
        if (cPath_getLength(paths[i]) - 200.0f < nearParam) {
            continue;
        }
        t4Vector aheadDelta = vu0Sub_112A50(pos, cPath_getPathPos_112A50(paths[i], 0, nearParam + 796.0f));
        t4Vector nearDelta = vu0Sub_112A50(*(&mPosition), out);
        float dist = vu0Dot_112A50(nearDelta, nearDelta) + vu0Dot_112A50(aheadDelta, aheadDelta);
        if (isComputer() == true) {
            sVEntry<float(void*, void*)>* vtable = (sVEntry<float(void*, void*)>*)VPTR_AT(this, sizeof(cRiderState));
            sVEntry<float(void*, void*)>* vf = &vtable[20];
            dist = vf->fn(vthis(this, *vf), paths[i]);
            if (bestW < dist) {
                best = paths[i];
                bestW = dist;
                if (best == (void*)AIPath) {
                    best = 0;
                }
            }
        } else {
            if (dist < bestD) {
                best = paths[i];
                bestD = dist;
                if (best == (void*)AIPath) {
                    best = 0;
                }
            }
        }
    }
    if (best != 0) {
        AIPath = (cAIPath*)best;
        mAIPathHint->NodeID = -1;
        float pathParam = cPath_intersectPath1(AIPath, &mPosition, &PathPos, &PathDistXY, mAIPathHint, 1);
        PathParam = pathParam;
        *(&IntersectPos) = cPath_getPathPos_112A50(AIPath, 0, pathParam + 796.0f);
        PrevPathParam = PathParam;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_switchToResetPath);
#ifdef SKIP_ASM


extern char gPaths[];
int cPathSys_getNearResetPaths_112D58(void* self, void* a1, void** out, int maxOut) __asm__("cPathSys_getNearResetPaths__FPv");
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_getLength(void* self);
t4Vector cAIPath_getResetPos_112D58(void* path, float* t) __asm__("cAIPath_getResetPos");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_112D58(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_112D58(const t4Vector& a, const t4Vector& b)
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

/**
 * Moves the rider onto the nearest reset path, ready to be put back on the course.
 *
 * Up to 6 reset paths near the rider (+0x110) come from cPathSys_getNearResetPaths__FPv (On Tour cPathSys::getNearResetPaths). For a
 * non-human rider (cBeing vtable slot 8) a path within 200 of its end is skipped; each candidate is scored by the
 * squared distance to its nearest point plus that to its reset position 796 further on (cAIPath_getResetPos), and the lowest
 * that differs from the current AI path wins.
 *
 * With no AI path at all it returns; otherwise the winner (if any) becomes the AI path, mAIPathHint is reset, the path
 * position (+0x4C4, +0x4C0) re-measured, and the closest point (+0x490) and aim point (+0x4A0, 796 on) are taken from
 * the path's reset positions.
 */
void cRider::switchToResetPath()
{
    void* paths[6];
    t4Vector pos;
    int n = cPathSys_getNearResetPaths_112D58(gPaths, &mPosition, paths, 6);
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    int i;
    for (i = 0; i < n; i++)
    {
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], &mPosition, &pos, &tOut, 1);
        float len = cPath_getLength(paths[i]);
        if (isHuman() == 0 && len - 200.0f < nearParam)
            continue;
        float aheadParam = nearParam + 796.0f;
        t4Vector aheadDelta = vu0Sub_112D58(*(&mPosition), cAIPath_getResetPos_112D58(paths[i], &aheadParam));
        t4Vector nearDelta = vu0Sub_112D58(*(&mPosition), pos);
        float dist = vu0Dot_112D58(nearDelta, nearDelta) + vu0Dot_112D58(aheadDelta, aheadDelta);
        if (dist < bestD)
        {
            best = paths[i];
            bestD = dist;
            if (best == (void*)AIPath)
                best = 0;
        }
    }
    if (AIPath == 0)
        return;
    if (best)
        AIPath = (cAIPath*)best;
    mAIPathHint->NodeID = -1;
    PathParam = cPath_intersectPath1(AIPath, &mPosition, &PathPos, &PathDistXY, mAIPathHint, 1);
    *(&PathPos) = cAIPath_getResetPos_112D58(AIPath, (float*)(&PathParam));
    float aimParam = PathParam + 796.0f;
    *(&IntersectPos) = cAIPath_getResetPos_112D58(AIPath, &aimParam);
    PrevPathParam = PathParam;
}
#endif

extern "C" void cRider_flagEventPathCheckpoint(cRider* self, int* checkpoint);

//100%
INCLUDE_ASM("ai/computer", cRider_checkTrackPathEvents);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: an extra `dist` read and the `cp` pointer (cp->DistanceLeft re-read) keep jump.c under its duplicate-insn
// limit; plain form 97.81%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern float cPath_intersectPath1_2FB0(void* track, void* pos, void* iPos, float* iDistXY, tPathIntersectInfo* pi, int is2D) __asm__("cPath_intersectPath1");
extern char gPaths[];

// cTrackPath's vtable entry (g++ 2.95 layout: this-delta, index, fn).

/**
 * Measures the rider's progress on its track path, fires the checkpoints it passed and gathers the events in range.
 *
 * Intersects the rider with the track path (+0xAB4, mTrackPathHint) and sets mDistToFinish = path length (+0x38) - path
 * position. When the rider moved forward (below the previous distance at +0x4D4), every one of gPaths' 6 checkpoints
 * between the old and new distance (the list ends at a zero entry) goes to cRider_flagEventPathCheckpoint, the
 * path's vtable slot 1 collects up to 12 events in the covered range into +0x4DC (count in NumPathEventsCur), and +0x4D4
 * advances. Otherwise the same query runs on the empty range at the old distance.
 *
 * @param vself  the rider (this)
 * @param pathDistXY  receives the rider's distance from the path
 * @param pathParam  receives the rider's position along the path
 * @param pathPos  receives the nearest point on the path
 */
// cRider::checkTrackPathEvents(float& pathDistXY, float& pathParam, t4Vector& pathPos) (On Tour name)
extern "C" void cRider_checkTrackPathEvents(cRider* vself, float* pathDistXY, float* pathParam, t4Vector* pathPos)
{
    cRider* self = (cRider*)vself;
    float trackParam = cPath_intersectPath1_2FB0(self->TrackPath, &self->mPosition, pathPos, pathDistXY, self->mTrackPathHint, 1);
    *pathParam = trackParam;
    cTrackPath* track = self->TrackPath;
    float cur = self->MinDistanceLeft;
    float distLeft = track->BaseDistanceLeft - trackParam;
    self->mDistToFinish = distLeft;
    if (distLeft < cur)
    {
        cPathSys* tbl = (cPathSys*)gPaths;
        for (int i = 0; i < 6; i++)
        {
            tPathCheckpoint* cp = &tbl->CheckpointList[i];
            float dist = tbl->CheckpointList[i].DistanceLeft;
            if (dist == 0.0f || dist < self->mDistToFinish)
                break;
            if (cp->DistanceLeft < self->MinDistanceLeft)
                cRider_flagEventPathCheckpoint(self, (int*)cp);
        }
        cTrackPath* curTrack = self->TrackPath;
        sVEntry<int(void*, float, float, void*, int)>* vt = (sVEntry<int (void*, float, float, void*, int)>*)curTrack->mVtbl;
        self->NumPathEventsCur = vt[1].fn(vthis(curTrack, vt[1]), self->MinDistanceLeft, self->mDistToFinish, (char*)&self->PathEventsCur[0], 0xC);
        self->MinDistanceLeft = self->mDistToFinish;
    }
    else
    {
        sVEntry<int(void*, float, float, void*, int)>* vt = (sVEntry<int(void*, float, float, void*, int)>*)track->mVtbl;
        self->NumPathEventsCur = vt[1].fn(vthis(track, vt[1]), cur, cur, (char*)&self->PathEventsCur[0], 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_getDistanceLeft__FPv);
#ifdef SKIP_ASM
/**
 * Returns the distance the rider still has to go along the track path.
 *
 * @return mDistToFinish (+0x4D0)
 */
float cRider::getDistanceLeft()
{
    return mDistToFinish;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_getTotalPathDistance__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's total path distance, the float at +0x4D8 that cRider_setTotalPathDistance sets.
 *
 * cRider_estimateFinishTick and cRider_estimateFinishScore subtract mDistToFinish from it to get the distance covered.
 *
 * @return the float at +0x4D8
 */
float cRider::getTotalPathDistance()
{
    return TotalPathDistance;
}
#endif

//100%
INCLUDE_ASM("ai/computer", __13cAirPredictor);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_reset(char* self);

/**
 * Constructs an air predictor: marks it unused and resets its flight state.
 *
 * Stores -1 at +0x30, then cAirPredictor_reset.
 */
cAirPredictor::cAirPredictor()
{
    GuessLandPatchRID = 0xFFFFFFFF;
    cAirPredictor_reset((char*)this);
}
#endif

//100%
INCLUDE_ASM("ai/computer", _$_13cAirPredictor);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys an air predictor; it owns nothing, so this only frees the object when asked.
 */
cAirPredictor::~cAirPredictor()
{
}
#endif

