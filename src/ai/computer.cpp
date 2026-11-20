#include "common.h"
#include "ai/riderhandler.h"
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

struct cComputer_SIRD {
    char pad0[0x18];
    sAiTune_SIRD* tune;     // 0x18
    char pad1C[0xDF8 - 0x1C];
    float skillA;           // 0xDF8
    float skillB;           // 0xDFC
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
extern "C" void cComputer_setIndividualRiderDifficulty(cComputer_SIRD* self, int level, int mode)
{
    switch (level) {
    case 1:
        self->skillA = 100.0f;
        self->skillB = 86.9153976f;
        self->tune->accel = -49670.1406f;
        self->tune->speed = 47500.0f;
        self->tune->boost = 0;
        break;
    case 2:
        self->skillA = 80.3891449f;
        self->skillB = 79.5998383f;
        self->tune->accel = -70000.0f;
        self->tune->speed = 45000.0f;
        self->tune->boost = 1;
        break;
    case 3:
        self->skillA = 51.007576f;
        self->skillB = 68.9012375f;
        self->tune->accel = -81571.1797f;
        self->tune->speed = 40000.0f;
        self->tune->boost = 1;
        break;
    case 4:
        self->skillA = 46.6018982f;
        self->skillB = 56.8430748f;
        self->tune->accel = -80321.1797f;
        self->tune->speed = 37500.0f;
        self->tune->boost = 1;
        break;
    case 5:
        self->skillA = 14.000701f;
        self->skillB = 42.4410439f;
        self->tune->accel = -92500.0f;
        self->tune->speed = 32500.0f;
        self->tune->boost = 1;
        break;
    }
    if (mode == 0) {
        switch (level) {
        case 1:
            self->skillA = 100.0f;
            self->skillB = 83.8930817f;
            self->tune->speed = 45000.0f;
            break;
        case 2:
            self->skillA = 55.8049736f;
            self->skillB = 64.6436768f;
            self->tune->speed = 42500.0f;
            break;
        case 3:
            self->skillA = 40.2501869f;
            self->skillB = 60.0044975f;
            self->tune->speed = 37500.0f;
            break;
        case 4:
            self->skillA = 15.5423021f;
            self->skillB = 52.0049553f;
            self->tune->speed = 32500.0f;
            break;
        case 5:
            self->skillA = 7.95483208f;
            self->skillB = 35.0202637f;
            self->tune->speed = 30000.0f;
            break;
        }
    } else if (mode == 2) {
        switch (level) {
        case 1:
            self->skillA = 100.0f;
            self->skillB = 100.0f;
            self->tune->speed = 50000.0f;
            break;
        case 2:
            self->skillA = 84.9981003f;
            self->skillB = 82.6824875f;
            self->tune->speed = 47500.0f;
            break;
        case 3:
            self->skillA = 74.9987259f;
            self->skillB = 70.8223801f;
            self->tune->speed = 40000.0f;
            break;
        case 4:
            self->skillA = 54.9958344f;
            self->skillB = 59.9440651f;
            self->tune->speed = 37500.0f;
            break;
        case 5:
            self->skillA = 29.9814339f;
            self->skillB = 46.9987717f;
            self->tune->speed = 32500.0f;
            break;
        }
    }
    self->skillB *= 0.01f;
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
extern sApp* D_004A28A8_UR __asm__("SSXApp");
struct sAiTune {
    char pad0[0xDC];
    float accel;     // 0xDC
    float speed;     // 0xE0
    int boost;       // 0xE4
};

// cComputer as the difficulty code sees it: the rider at 0x18 read as its AI tuning, the skill values.
struct sCompSkill {
    char pad_0x0[0x18];
    sAiTune* tune;                  // 0x18
    char pad_0x1C[0xDF8 - 0x1C];
    float skillA;                   // 0xDF8
    float skillB;                   // 0xDFC
    short diff;                     // 0xE00
    short pad_0xE02;
    int f4;                         // 0xE04
    int f8;                         // 0xE08
};

extern int D_005308D0[];
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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

void cComputer_updateRiderDifficulty_UR(sCompSkill* self) __asm__("cComputer_updateRiderDifficulty");
/**
 * Sets this computer rider's difficulty values for the race from its AI index (1-5) and the heat number.
 *
 * The level is the rider's getAIIndex() (1 if negative), the heat SSXApp->GameModeMan->iHeatNum. It sets
 * mPathDifficulty by updateRiderPathDifficulty(), mShouldTakeShortcuts for level 1 in heat 3 and
 * mShouldBeRandom in game modes 1, 5 and 6, then calls setIndividualRiderDifficulty() with a profile
 * difficulty setting (cBENewPlayerInterface_getDifficultyLevel). Game events 4 and 5 force mPathDifficulty to 0 and 2. The Super AI race
 * rule (D_005308D0 bit 2, "kT_11SuperAI") then maxes both skills, enables shortcuts, sets the rider's
 * rubber-band thresholds to -100/100 with mode 2 (+0xE4), mPathDifficulty to 2 (levels 1-2) or 1 and returns;
 * otherwise locations 4 (+25%) and 2-3 (+10%) raise mTrickDifficulty, capped at 1.
 */
void cComputer_updateRiderDifficulty_UR(sCompSkill* self)
{
    char* rider = *(char**)((char*)self + 0x18);
    int mode = *(int*)D_004A28A8_UR->fC0;
    char* obj = rider + 0x6C0;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    int aiIndex = vt[7].fn(obj + vt[7].delta);
    int level = ValidOr_UR(aiIndex, -1, 1);
    self->diff = cComputer_updateRiderPathDifficulty(self, level, mode);
    if (level == 1 && mode == 3) {
        self->f4 = 1;
    } else {
        self->f4 = 0;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameMode == 1 || cBENewRaceInterface_newRace.cGameMode == 6 || cBENewRaceInterface_newRace.cGameMode == 5) {
        self->f8 = 1;
    } else {
        self->f8 = 0;
    }
    cComputer_setIndividualRiderDifficulty_UR(self, level, cBENewPlayerInterface_getDifficultyLevel(cBE_getInterface_Fv(cBE_getBE(), 1), cBENewRaceInterface_newRace.cGameMode == 0));
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGameEvent == 4) {
        self->diff = 0;
    } else if (cBENewRaceInterface_newRace.cGameEvent == 5) {
        self->diff = 2;
    }
    if ((D_005308D0[0] >> 2) & 1) {
        self->skillB = 1.0f;
        self->skillA = 100.0f;
        self->f4 = 1;
        self->tune->accel = -100.0f;
        self->tune->speed = 100.0f;
        self->tune->boost = 2;
        if (level == 1 || level == 2) {
            self->diff = 2;
        } else {
            self->diff = 1;
        }
        return;
    }
    if (cBENewRaceInterface_getLocationID(iface) == 4) {
        self->skillB += self->skillB * 0.25f;
    } else if (cBENewRaceInterface_getLocationID(iface) == 2 || cBENewRaceInterface_getLocationID(iface) == 3) {
        self->skillB += self->skillB * 0.10000000149011612f;
    }
    if (self->skillB > 1.0f) {
        self->skillB = 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_setAttributeLevels);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);
extern "C" void cBEStatInterface_setStatLevel(void* iface, int a1, int a2);

struct sVEntry10C9A8 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10C9A8(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10C9A8* vt = *(sVEntry10C9A8**)obj;
    return vt[7].fn(obj + vt[7].delta);
}

/**
 * Sets every stat of the computer's rider to one level chosen by the current location.
 *
 * Reads the location DB's field 0x54 for the current race location (cBENewRaceInterface_getLocationPeak1 on the new-race interface) and calls
 * cBEStatInterface_setStatLevel for the rider's AI index (rider vtable slot 7, getAIIndex) with level 7 when it is 2, 4
 * when it is 1 and 1 otherwise. Called from cAI_initComputerRiders for each new computer.
 */
extern "C" void cComputer_setAttributeLevels(cComputer* self)
{
    void* statInterface = cBE_getInterface_Fv(cBE_getBE(), 3);
    void* newRaceInterface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_getLocationPeak1(newRaceInterface) == 2)
    {
        cBEStatInterface_setStatLevel(statInterface, riderV7_10C9A8((char*)self->mRider), 7);
    }
    else if (cBENewRaceInterface_getLocationPeak1(newRaceInterface) == 1)
    {
        cBEStatInterface_setStatLevel(statInterface, riderV7_10C9A8((char*)self->mRider), 4);
    }
    else
    {
        cBEStatInterface_setStatLevel(statInterface, riderV7_10C9A8((char*)self->mRider), 1);
    }
}
#endif

extern "C" void cComputer_updateRiderDifficulty(void*);

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/computer", cComputer_initRiderDifficulty);
#ifdef SKIP_ASM
/**
 * Initialises this computer rider's difficulty by calling cComputer_updateRiderDifficulty().
 */
extern "C" void cComputer_initRiderDifficulty(cComputer* self)
{
    cComputer_updateRiderDifficulty(self);
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
 * Returns -1 when the air time is 0.75 s or less, exactly 100 (getSpinCmd's value when the air predictor has
 * no landing), or under 1 s while the rider is heading for a rail (mToRail). Each kind is tried only when its
 * shouldRider*() roll passes and the kind before it found nothing.
 *
 * @param airTime  predicted air time left, in seconds (On Tour airTime)
 * @return the chosen grab slot (0-14), or -1 for none
 */
extern "C" int cComputer_chooseGrabMain(cComputer* self, float airTime)
{
    int currentGrab = -1;
    int skip = self->mToRail != 0 && airTime < 1.0f;
    if (!skip && airTime > 0.75f && airTime != 100.0f)
    {
        if (cComputer_shouldRiderUber(self)) currentGrab = cComputer_chooseRandomUberGrab(self, airTime);
        if (currentGrab == -1)
        {
            if (cComputer_shouldRiderTweak(self)) currentGrab = cComputer_chooseRandomTweakGrab(self, airTime);
            if (currentGrab == -1)
            {
                if (cComputer_shouldRiderGrab(self)) currentGrab = cComputer_chooseRandomGrab(self, airTime);
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CBE0 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10CBE0(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10CBE0* vt = *(sVEntry10CBE0**)obj;
    return vt[7].fn(obj + vt[7].delta);
}


/**
 * Picks a random plain grab (slot 0-14) and times it against the air time; clears mIsUber and mIsTweak.
 *
 * Stores @p time in mGrabStartTime and reads the grab animation's marker times (cRiderAnimBase_getAIAnimEventTime keys 2 and 1,
 * divided by cRider_getGrabAnimSpeedScale()). When @p time is at least the key-2 time it sets mGrabEndTime = t -
 * key 2 and mHoldToLandTime = key 2 - key 1 and counts the grab in GrabHistoryCounter, which this function
 * never reads: the pick is purely random.
 *
 * @param computer  the computer rider
 * @param time  air time available, in seconds (On Tour time)
 * @return the grab slot, or -1 when its animation does not fit in the air time
 */
extern "C" int cComputer_chooseRandomGrab(cComputer* computer, float time)
{
    cComputer* self = computer;
    self->mIsUber = 0;
    self->mIsTweak = 0;
    self->mGrabStartTime = time;
    int idx = AIrand() % 15;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    int anim = cBEBAGTInterface_getGrabAnim(iface, riderV7_10CBE0(((char*)self->mRider)), idx);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->mRider));
    float grabTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 2) / spd;
    float holdTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 1) / spd;
    if (time < grabTime) return -1;
    self->mGrabEndTime = time - grabTime;
    self->mHoldToLandTime = grabTime - holdTime;
    if (idx != -1 && idx < 15)
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getTweakAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getUberAnim(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CD20 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10CD20(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10CD20* vt = *(sVEntry10CD20**)obj;
    return vt[7].fn(obj + vt[7].delta);
}


/**
 * Picks the least-used tweak grab (slots 1-14) whose animation fits in the air time and sets mIsTweak.
 *
 * Skips slots with no tweak animation (0x1B6) and, while the rider's uber timer runs, slots that also have an
 * uber variant (only checked when mUberFlags bit 0 is set). A slot fits when its tweak's key-2 time plus the
 * plain grab's key-1 time is under @p time; the lowest TweakHistoryCounter wins (the first one on ties), gets
 * mGrabEndTime and mHoldToLandTime and is counted. Clears mIsUber; mIsTweak is left as it was when nothing
 * fits.
 *
 * @param computer  the computer rider
 * @param time  air time available, in seconds (On Tour time)
 * @return the tweak slot, or -1 when none fits
 */
extern "C" int cComputer_chooseRandomTweakGrab(cComputer* computer, float time)
{
    cComputer* self = computer;
    self->mIsUber = 0;
    self->mGrabStartTime = time;
    int found = -1;
    int best = 10000;
    int i = 1;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->mRider));
    for (; i < 15; i++)
    {
        int anim = cBEBAGTInterface_getTweakAnim(iface, riderV7_10CD20(((char*)self->mRider)), i);
        if (anim == 438)
            continue;
        int alt = (self->mRider->mUberFlags & 1) ? cBEBAGTInterface_getUberAnim(iface, riderV7_10CD20(((char*)self->mRider)), i, self->mRider->mUberLevel >= 5) : 438;
        if (0.0f < self->mRider->mUberTimer && alt != 438)
            continue;
        int anim2 = cBEBAGTInterface_getGrabAnim(iface, riderV7_10CD20(((char*)self->mRider)), i);
        float normalTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim2, 1) / spd;
        float tweakHoldTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 1) / spd;
        float tweakTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 2) / spd;
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
    if (found != -1 && found < 15)
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
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short cBEBAGTInterface_getGrabAnim(void* self, int a1, int a2);
extern "C" int cBEBAGTInterface_getUberAnim(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float cRiderAnimBase_getAIAnimEventTime(char* self, int a1, int frame);

struct sVEntry10CF68 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10CF68(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10CF68* vt = *(sVEntry10CF68**)obj;
    return vt[7].fn(obj + vt[7].delta);
}


/**
 * Picks the least-used uber grab (slots 0-14) whose animation fits in the air time and sets mIsUber.
 *
 * Fails at once unless the rider's uber timer (mUberTimer) is running. The animation is the slot's uber
 * variant (cBEBAGTInterface_getUberAnim, with a flag for mUberLevel >= 5) and slots without one (0x1B6) are skipped; the fit
 * test, timing fields and counting are as in chooseRandomTweakGrab(), on UberHistoryCounter. Clears mIsTweak
 * but never clears mIsUber, so a failed call can leave an older value.
 *
 * @param computer  the computer rider
 * @param time  air time available, in seconds (On Tour time)
 * @return the uber slot, or -1 when none fits or no uber is available
 */
extern "C" int cComputer_chooseRandomUberGrab(cComputer* computer, float time)
{
    cComputer* self = computer;
    self->mIsTweak = 0;
    if (self->mRider->mUberTimer <= 0.0f)
        return -1;
    self->mGrabStartTime = time;
    int found = -1;
    int best = 10000;
    int i = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->mRider));
    for (; i < 15; i++)
    {
        int anim = cBEBAGTInterface_getUberAnim(iface, riderV7_10CF68(((char*)self->mRider)), i, self->mRider->mUberLevel >= 5);
        if (anim == 438)
            continue;
        int anim2 = cBEBAGTInterface_getGrabAnim(iface, riderV7_10CF68(((char*)self->mRider)), i);
        float normalTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim2, 1) / spd;
        float uberHoldTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 1) / spd;
        float uberTime = cRiderAnimBase_getAIAnimEventTime((char*)self->mRider->mRiderAnim, anim, 2) / spd;
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
    if (found != -1 && found < 15)
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
extern "C" void cComputer_resetCounters(cComputer* self)
{
    for (int i = 0; i < 15; i++)
    {
        self->GrabHistoryCounter[i] = 0;
        self->UberHistoryCounter[i] = 0;
        self->TweakHistoryCounter[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForReset);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
int cRider_getControlState(void* self);
extern "C" float func_0031C228(float x);

struct sVec_0010D1A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRider_0010D1A0 {
    char pad0[0x1B0];
    sVec_0010D1A0 vel;  // 0x1B0
    char pad1C0[0x4C8 - 0x1C0];
    float speed;        // 0x4C8
    float heading;      // 0x4CC
    char pad4D0[0x788 - 0x4D0];
    char* board;        // 0x788

    sVec_0010D1A0 getVel() { return vel; }
};

struct sComputer_0010D1A0 {
    char pad0[0x18];
    sRider_0010D1A0* rider;  // 0x18
    char pad1C[0xE70 - 0x1C];
    int dir;                 // 0xE70
    int stuckTime;           // 0xE74
    int wrongTime;           // 0xE78
};

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
 * When the rider's mTargetID is negative (no target), mBadHeadingTime counts the calls in which the rider's +0x4CC
 * angle is more than 60 degrees off its facing (mMatrix front row), else it is zeroed; 61 or more means wrong way.
 * Motion state 2 (cRider_getMotionState) counts mWanderTime up and cancels wrong way; state 4 with any bad-heading count
 * returns 1 at once; state 1 or a speed (+0x4C8) under 1000 zeroes mWanderTime and, in state 1, returns 1 once the air
 * predictor's CurAirTime (mAirPredictor + 0xA0) passes 30 s; anything else counts mWanderTime up. The result is wrong
 * way or mWanderTime >= 223, and outside control state 8 (cRider_getControlState) only while mTargetID is negative.
 *
 * @return 1 when the rider should reset, else 0
 */
extern "C" int cComputer_checkForReset(sComputer_0010D1A0* self)
{
    float ang = atan2_0010D1A0(self->rider->getVel().y, self->rider->getVel().x);
    float headingDiff = wrap_0010D1A0(self->rider->heading - ang);
    if (self->dir < 0) {
        if (__builtin_fabsf(headingDiff) > 1.047197699546814f) {
            self->wrongTime++;
        } else {
            self->wrongTime = 0;
        }
    } else {
        self->wrongTime = 0;
    }
    int wrong = self->wrongTime >= 61;
    if (cRider_getMotionState(self->rider) == 2) {
        wrong = 0;
        self->stuckTime++;
    } else if (cRider_getMotionState(self->rider) == 4 && self->wrongTime > 0) {
        return 1;
    } else if (cRider_getMotionState(self->rider) == 1 || self->rider->speed < 1000.0f) {
        self->stuckTime = 0;
        wrong = 0;
        if (cRider_getMotionState(self->rider) == 1 && *(float*)(self->rider->board + 0xA0) > 30.0f) {
            return 1;
        }
    } else {
        self->stuckTime++;
    }
    int stuck = wrong || self->stuckTime >= 223;
    if (cRider_getControlState(self->rider) != 8) {
        stuck = stuck && self->dir < 0;
    }
    return stuck;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_scorePath);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" float cComputer_calculateDifficultyScore(cComputer* self, int a1, int a2, int a3);
struct sAi0010FC30;
extern "C" int cAI_aiRidersOnPath(sAi0010FC30* self, int team);
extern "C" float cRider_calculateAheadDist(cRider* self, int arg1);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);

struct sVec4_10D410 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_10D410(const sVec4_10D410& v)
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
static inline sVec4_10D410 vu0Scale_10D410(const sVec4_10D410& v, float s)
{
    sVec4_10D410 r;
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
static inline sVec4_10D410 vu0Add_10D410(const sVec4_10D410& a, const sVec4_10D410& b)
{
    sVec4_10D410 r;
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

struct sAi_10D410 {
    char pad0[0x28];
    char* riders[20];
    int count;
};

/**
 * Scores an AI path as a route choice for this computer rider (route selection); higher is better.
 *
 * The path's flag word (+0x38) holds a shortcut bit (0), hard, medium and easy bits (1-3), a speed rule (bits
 * 4-6: 0 at most, 3 at least) and a speed (bits 7-9: 20-120 km/h). A path that fails
 * calculateDifficultyScore(), is a shortcut the rider may not take or breaks the speed rule scores 0; one
 * over 5000 from the point 796 ahead of the rider scores -1, and the rival's current path 100. Otherwise it
 * sums weighted terms (each /22): nearness, the speed-rule value, how few other computer riders use the path,
 * the difficulty score, the shortcut bonus and, with mShouldBeRandom, a random term.
 *
 * @param tgt  the candidate path (On Tour cAIPath* CurrentPath)
 * @return the score: 0 unusable, -1 too far, 100 the rival's path, otherwise the weighted sum
 */
extern "C" float cComputer_scorePath(cComputer* self, char* tgt)
{
    float sc[7];
    char* ai = (char*)SSXApp->Game->AI;
    int flags = *(int*)(tgt + 0x38);
    int kind = (flags >> 7) & 7;
    int lvl = (flags >> 4) & 7;
    int solo = flags & 1;
    if (kind > 8) {
        kind = 0;
    }
    int isMedium = (flags >> 2) & 1;
    int isHard = (flags >> 1) & 1;
    int isEasy = (flags >> 3) & 1;
    float base = cComputer_calculateDifficultyScore(self, isEasy, isMedium, isHard);
    if (base <= 0.0f) {
        return 0.0f;
    }
    if (self->mShouldTakeShortcuts == 0 && solo != 0) {
        return 0.0f;
    }
    float speed = vu0Length_10D410(*(sVec4_10D410*)&self->mRider->mVelocity) * 0.035999998450279236f;
    if (lvl == 0) {
        if ((float)speedLimit_10D410(kind) < speed) {
            return 0.0f;
        }
    } else if (lvl == 3) {
        if (speed < (float)speedLimit_10D410(kind)) {
            return 0.0f;
        }
    }
    char* rider = (char*)self->mRider;
    float len = vu0Length_10D410(*(sVec4_10D410*)(rider + 0x1E0));
    float total = 22.0f;
    float lookAheadSec = cRider_calculateAheadDist((cRider*)rider, 1) / len;
    sVec4_10D410 pos = vu0Add_10D410(*(sVec4_10D410*)&self->mRider->mPosition, vu0Scale_10D410(*(sVec4_10D410*)&self->mRider->mVelocity, lookAheadSec));
    sVec4_10D410 out;
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
    char* rider2 = (char*)self->mRider;
    if (*(int*)(rider2 + 0xF0) != 0 && *(char**)(((sAi_10D410*)ai)->riders[*(int*)(rider2 + 0xF8)] + 0xAB8) == tgt) {
        return 100.0f;
    }
    float zero = 0.0f;
    sc[1] = zero;
    float w = 1.0f / total;
    sc[2] = (float)lvl * (w * 0.3333333432674408f);
    // PORT: the path pointer is passed as cAI_aiRidersOnPath's int team
    int n = cAI_aiRidersOnPath((sAi0010FC30*)ai, (int)tgt);
    n -= tgt == *(char**)((char*)self->mRider + 0xAB8);
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
    if (self->mShouldTakeShortcuts != 0 && solo != 0) {
        sc[5] = w * 100.0f;
    } else {
        sc[5] = 0.0f;
    }
    w = 2.0f / total;
    if (self->mShouldBeRandom != 0) {
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
 * Path difficulty 0, 1 and 2 require @p IsEasy, @p IsMedium and @p IsHard respectively. Any other value
 * passes only when (IsEasy == IsMedium) equals IsHard, an odd test that is unused in practice since
 * mPathDifficulty is always 0-2. A passing path scores (4 - number of flags set) * 33.3, so a path meant for
 * fewer levels scores higher.
 *
 * @param IsEasy  the path suits easy riders
 * @param IsMedium  the path suits medium riders
 * @param IsHard  the path suits hard riders
 * @return 33.3 to 99.9 for a suitable path, otherwise 0
 */
extern "C" float cComputer_calculateDifficultyScore(cComputer* self, int IsEasy, int IsMedium, int IsHard)
{
    short pathDifficulty = self->mPathDifficulty;
    if (pathDifficulty == 0)
    {
        if (!IsEasy)
            return 0.0f;
    }
    else if (pathDifficulty == 1)
    {
        if (!IsMedium)
            return 0.0f;
    }
    else if (pathDifficulty == 2)
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
struct sComputer_0010D8F8
{
    int a;
    int b;
    char pad[0x1C];
};

/**
 * Finds this rider's opponent: the first rider in its relation table that is both valid and human.
 *
 * Scans the six mRelations entries of the rider, testing mValid and the +0x4 word, which cAI_RefreshGroupAI()
 * fills with that rider's isHuman().
 *
 * @return the opponent's index (0-5), or -1 when there is none
 */
extern "C" int cComputer_myOpponentIndex(cComputer* self)
{
    sComputer_0010D8F8* relation = (sComputer_0010D8F8*)self->mRider;
    for (int i = 0; i < 6; i++, relation++)
    {
        if (relation->a != 0 && relation->b != 0)
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_isFarBehind);
#ifdef SKIP_ASM
extern "C" int cComputer_getRival(cComputer* self);
float cRider_getDistanceLeft(void* self);

/**
 * Tells whether this computer rider trails its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandBehind (a negative
 * distance set by the difficulty code). Without an opponent (cComputer_getRival, the rider at myOpponentIndex())
 * it is not behind.
 *
 * @return 1 when the difference is below mRubberBandBehind, otherwise 0
 */
extern "C" int cComputer_isFarBehind(cComputer* self)
{
    // PORT: cComputer_getRival returns a pointer as int (unit declaration).
    int opponent = cComputer_getRival(self);
    if (opponent == 0)
        return 0;
    float relDist = cRider_getDistanceLeft((void*)opponent) - (self->mRider)->mDistToFinish;
    if (relDist < self->mRider->mRubberBandBehind)
        return 1;
    return 0;
}
#endif
//100%
INCLUDE_ASM("ai/computer", cComputer_isFarAhead);
#ifdef SKIP_ASM
extern "C" int cComputer_getRival(cComputer* self);
float cRider_getDistanceLeft(void* self);

/**
 * Tells whether this computer rider leads its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandAhead. Without an
 * opponent (cComputer_getRival, the rider at myOpponentIndex()) it is not ahead.
 *
 * @return 1 when the difference is above mRubberBandAhead, otherwise 0
 */
extern "C" int cComputer_isFarAhead(cComputer* self)
{
    // PORT: cComputer_getRival returns a pointer as int (unit declaration).
    int opponent = cComputer_getRival(self);
    if (opponent == 0)
        return 0;
    float relDist = cRider_getDistanceLeft((void*)opponent) - (self->mRider)->mDistToFinish;
    if (self->mRider->mRubberBandAhead < relDist)
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForAttack);
#ifdef SKIP_ASM
/**
 * Tells whether another rider is this computer rider's rival, the one it attacks rather than avoids.
 *
 * @param oppID  index of the other rider
 * @return 1 when the rider has a rival (mHasRival) and mRivalIndex equals @p oppID, otherwise 0
 */
extern "C" int cComputer_checkForAttack(cComputer* self, int oppID)
{
    void* rider = self->mRider;
    int shouldAttack = 0;
    if (*(int*)((char*)rider + 0xf0) != 0) {
        shouldAttack = *(int*)((char*)rider + 0xf8) == oppID;
    }
    return shouldAttack;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForBlock);
#ifdef SKIP_ASM
struct sVE_0010DA10 { short delta; short index; int (*fn)(void*); };
struct sEnt_0010DA10 { int vis; int val; float dist; int c; int d; int e; int f; int g; int h; };
struct sRider_0010DA10 { sEnt_0010DA10 ent[6]; };
struct sAI_0010DA10 { char pad[0x28]; char* riders[20]; int count; };
extern "C" int AITick();
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
unsigned int AIrand();

/**
 * Decides whether this computer rider should block an attack, keeping each decision for two seconds.
 *
 * While mBlockTimer runs it counts down by 1/60 and returns mShouldIBlock; a negative timer is reset to 0.
 * Otherwise, on every sixth AI tick, it looks for another human rider within 200 whose channel-1 animation
 * class is 13 (probably a punch); the first one found starts a 2 s timer with a 25% chance of blocking.
 *
 * @return nonzero when the rider should block now, otherwise 0
 */
extern "C" int cComputer_checkForBlock(cComputer* self)
{
    float blockTimer = self->mBlockTimer;
    if (blockTimer < 0.0f)
    {
        self->mBlockTimer = 0.0f;
        return 0;
    }
    if (0.0f < blockTimer)
    {
        self->mBlockTimer = blockTimer - 0.01666666753590107f;
        return self->mShouldIBlock;
    }
    if (AITick() % 6 != 0)
        return 0;
    sAI_0010DA10* ai = (sAI_0010DA10*)SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->count; i++)
    {
        char* myBeing = (char*)self->mRider + 0x6C0;
        sVE_0010DA10* ve = &(*(sVE_0010DA10**)myBeing)[7];
        if (i == ve->fn(myBeing + ve->delta))
            continue;
        char* otherBeing = ai->riders[i] + 0x6C0;
        sVE_0010DA10* ve2 = &(*(sVE_0010DA10**)otherBeing)[8];
        if (ve2->fn(otherBeing + ve2->delta) == 0)
            continue;
        if (!(((sRider_0010DA10*)self->mRider)->ent[i].dist < 200.0f))
            continue;
        if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(ai->riders[i] + 0x784), 1) != 13)
            continue;
        unsigned int randomRoll = AIrand() % 100;
        int* pf = (int*)(((char*)self) + 0xF34);
        *pf = (float)randomRoll < 25.0f;
        self->mBlockTimer = 2.0f;
        return self->mShouldIBlock;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForPunching);
#ifdef SKIP_ASM
extern "C" int cBEAggressionInterface_getAggression(void* self, int a, int b);
void* cBEAggressionInterface_getThis();

struct sVE_0010DBF0 { short delta; short index; int (*fn)(void*); };
struct sVec_0010DBF0 { float x, y, z, w; } __attribute__((aligned(16)));
struct sEnt_0010DBF0 { int vis; int val; float dist; float angle; int d; int e; int f; int g; int h; };
struct sRider_0010DBF0 {
    sEnt_0010DBF0 ent[6];           // 0x0
    char padD8[0xF4 - 0xD8];
    int lastTaunt;                  // 0xF4
    char padF8[0x1B0 - 0xF8];
    sVec_0010DBF0 vel;              // 0x1B0
};

static inline int id_0010DBF0(char* self)
{
    char* o = *(char**)(self + 0x18) + 0x6C0;
    sVE_0010DBF0* ve = &(*(sVE_0010DBF0**)o)[7];
    return ve->fn(o + ve->delta);
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
    return cBEAggressionInterface_getAggression(agg, a, b) >= 3;
}

/**
 * Picks a rider beside the computer to punch: returns 1 and the side when one is close, at the side and disliked.
 *
 * Runs only every 6th AI tick. For each other rider i (by rider index, skipping its own getAIIndex) whose
 * mRelations[i].mDistance is under 200 and whose bearing (mRelations[i].mAngle) is 30 to 150 degrees off the rider's
 * facing (mMatrix front row), and towards whom the aggression interface (cBEAggressionInterface_getAggression on
 * cBEAggressionInterface_getThis) gives 3 or more, it takes the first one when mLastShoveFrame + 120 is before the
 * current AI tick, sets @p side and stores the tick in mLastShoveFrame.
 *
 * @param side  receives 1 when the rider is on the side of a positive angle difference, else 0
 * @return 1 when a target was found, else 0
 */
// Taunt check: finds a close rider beside us (30..150 degrees off our heading) that we're aggressive towards,
// at most every 120 frames; *side tells which side it is on.
extern "C" int cComputer_checkForPunching(cComputer* self, int* side)
{
    if (AITick() % 6 != 0)
        return 0;
    cAI* ai = SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->NumberOfRidersAllocated; i++)
    {
        float pi = 3.1415927410125732f;
        float lo = 0.5235988497734070f;
        float hi = 2.6179940700531006f;
        if (i == id_0010DBF0(((char*)self)))
            continue;
        sRider_0010DBF0* rider = (sRider_0010DBF0*)self->mRider;
        if (!(rider->ent[i].dist < 200.0f))
            continue;
        sVec_0010DBF0 velForY = rider->vel;
        sVec_0010DBF0 velForX = rider->vel;
        float ang = atan2_0010DBF0(velForY.y, velForX.x, pi);
        float angleDiff = wrap_0010DBF0(ang - ((sRider_0010DBF0*)self->mRider)->ent[i].angle);
        float ad = __builtin_fabsf(angleDiff);
        if (!(lo < ad))
            continue;
        if (!(ad < hi))
            continue;
        void* agg = cBEAggressionInterface_getThis();
        if (!isAggr_0010DBF0(agg, id_0010DBF0(((char*)self)), i))
            continue;
        if (!(((sRider_0010DBF0*)self->mRider)->lastTaunt + 120 < AITick()))
            continue;
        *side = 0.0f < angleDiff;
        ((sRider_0010DBF0*)self->mRider)->lastTaunt = AITick();
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_getRival);
#ifdef SKIP_ASM

/**
 * Returns the computer's opponent rider: SSXApp->Game->AI's Riders[myOpponentIndex()], or 0 when it has none.
 *
 * The C declaration returns the pointer as an int.
 *
 * @return the opponent's cRider, or 0
 */
extern "C" int cComputer_getRival(cComputer* self)
{
    int i = cComputer_myOpponentIndex((cComputer*)self);
    if (i < 0)
        return 0;
    return (int)SSXApp->Game->AI->Riders[i];
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_setupWarp);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
float cRider_getDistanceLeft(void* self);
extern signed char D_00535C11[];

/**
 * Returns the rubber-band time warp for the computer rider: above 1 when it is too far behind its opponent, below 1
 * when too far ahead.
 *
 * Returns 1.0 without an opponent (cComputer_getRival), when the rider's +0xE4 mode is 0, in an online race (D_00535C11 ==
 * 2) or within 50000 of the finish (mDistToFinish). Otherwise d = the opponent's distance left minus the rider's. When
 * d < mRubberBandBehind (modes 2 and 3) it returns (d - k) / k + 1 times 1.1, with k = mRubberBandBehind; when d >
 * mRubberBandAhead (modes 1 and 3) it returns 0.909 / ((d - k2) / k2 + 1), with k2 = mRubberBandAhead; else 1.0. The
 * new-race interface is fetched and not used.
 *
 * @return the desired time scale for cRider_updateTimeWarp
 */
extern "C" float cComputer_setupWarp(cComputer* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    // PORT: cComputer_getRival returns a pointer as int (unit declaration).
    int rival = cComputer_getRival(self);
    if (rival == 0) return 1.0f;
    char* rider = (char*)self->mRider;
    if (*(int*)(rider + 0xE4) == 0) return 1.0f;
    if (D_00535C11[0] == 2) return 1.0f;
    if (*(float*)(rider + 0x4D0) < 50000.0f) return 1.0f;
    float warpVal;
    float relDist = cRider_getDistanceLeft((void*)rival) - self->mRider->mDistToFinish;
    char* rider2 = (char*)self->mRider;
    float k = *(float*)(rider2 + 0xDC);
    if (relDist < k && (*(int*)(rider2 + 0xE4) == 2 || *(int*)(rider2 + 0xE4) == 3))
    {
        warpVal = ((relDist - k) / k + 1.0f) * 1.100000023841858f;
    }
    else
    {
        char* rider3 = (char*)self->mRider;
        float rubberAhead = *(float*)(rider3 + 0xE0);
        if (rubberAhead < relDist && (*(int*)(rider3 + 0xE4) == 1 || *(int*)(rider3 + 0xE4) == 3))
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
extern "C" void cRider_flagEventReaction(cRider* self, int mode, float forceDelaySec)
{
    int reactionTick;
    if (mode == 0)
    {
        reactionTick = -1;
        self->ReactionEventType = 0;
    }
    else
    {
        int frames = (int)(forceDelaySec * 60.0f);
        reactionTick = AITick() + frames;
        self->ReactionEventType = mode;
    }
    self->ReactionEventTick = reactionTick;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBoostPowerUp);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cAudioInterface_bxAudio();
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
 * Does nothing in boost mode 3 (mBoostMode) or for a zero amount. A gain first calls a BE stat getter
 * (cBEStatInterface_getTrickAttrib, result unused) and is dropped unless @p mask shares a bit with mBoostSourceMask. It
 * notifies the audio system (cSSXAudio_BoostBar), then clamps. A gain that fills the meter with uber enabled
 * (mUberFlags) shows HUD message 5 when mUberTimer is 0, raises an mUberLevel of 0 or less by one and, at
 * levels 1-9, makes mUberTimer at least 20; a loss below level 10 stops mUberTimer.
 *
 * @param mask  boost source flags (On Tour powerSourceFlags), tested against mBoostSourceMask for a gain
 * @param amt  amount to add, negative to drain (On Tour powerUp)
 */
extern "C" void cRider_flagEventBoostPowerUp(cRider* self, int mask, float amt)
{
    if (self->mBoostMode == 3)
        return;
    float floatZero = 0.0f;
    if (amt == floatZero)
        return;
    if (amt > floatZero)
    {
        cBEStatInterface_getTrickAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->mAIIndex, self->mStatsIndex);
        if ((mask & self->mBoostSourceMask) == 0)
            return;
    }
    cSSXAudio_BoostBar(cAudioInterface_bxAudio(), self, self->unk_2F8, amt);
    self->unk_2F8 = clamp_10E098(self->unk_2F8 + amt, floatZero, 1.0f);
    if (self->unk_2F8 == 1.0f && amt > floatZero && self->mUberFlags != 0)
    {
        if (self->mUberTimer == floatZero)
            cRider_flagEventReaction(self, 5, floatZero);
        if (self->mUberLevel <= 0)
            self->mUberLevel += 1;
        if (self->mUberLevel >= 1 && self->mUberLevel <= 9)
        {
            // PORT: g++ `>?` (max) operator.
            self->mUberTimer = self->mUberTimer >? 20.0f;
        }
    }
    else if (amt < 0.0f && self->mUberLevel < 10)
    {
        self->mUberTimer = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBump);
#ifdef SKIP_ASM
struct sVEntry0010E228 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_RiderCollision(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a light bump from another rider: collision sound and a mild aggression event.
 *
 * Plays the rider-collision sound (cSSXAudio_RiderCollision, On Tour cSSXAudio::RiderCollision) and func_002A0A30(audio, attacker,
 * self, 1, 0), then reports aggression behaviour 0 (kAGG_MILDCONTACT) from @p attacker against @p self to cBEAggressionInterface_applyAggression (On Tour
 * cBEAggressionInterface::applyAggression). Each rider's id comes from its cBeing vtable (+0x6C0) slot 7 (On Tour
 * cBeing::getAIIndex).
 *
 * @param self  the rider that was bumped (this)
 * @param attacker  the rider that did the bumping (On Tour attacker)
 */
extern "C" void cRider_flagEventBump(cRider* self, void* attacker)
{
    cSSXAudio_RiderCollision(cAudioInterface_bxAudio(), self, attacker);
    func_002A0A30(cAudioInterface_bxAudio(), attacker, self, 1, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)attacker + 0x6C0;
    sVEntry0010E228* eb = &(*(sVEntry0010E228**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)self + 0x6C0;
    sVEntry0010E228* ea = &(*(sVEntry0010E228**)oa)[7];
    cBEAggressionInterface_applyAggression(agg, rb, ea->fn(oa + ea->delta), 0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBumpKnockdown);
#ifdef SKIP_ASM
struct sVEntry0010E2E8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_RiderCollision(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a bump from another rider that knocked this one down: collision sound and a moderate aggression event.
 *
 * As cRider_flagEventBump, but func_002A0A30 gets (attacker, self, 1, 1) and the aggression behaviour passed to
 * cBEAggressionInterface_applyAggression (On Tour cBEAggressionInterface::applyAggression) is 1 (kAGG_MODCONTACT).
 *
 * @param self  the rider that was knocked down (this)
 * @param attacker  the rider that did the bumping (On Tour attacker)
 */
extern "C" void cRider_flagEventBumpKnockdown(cRider* self, void* attacker)
{
    cSSXAudio_RiderCollision(cAudioInterface_bxAudio(), self, attacker);
    func_002A0A30(cAudioInterface_bxAudio(), attacker, self, 1, 1);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)attacker + 0x6C0;
    sVEntry0010E2E8* eb = &(*(sVEntry0010E2E8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)self + 0x6C0;
    sVEntry0010E2E8* ea = &(*(sVEntry0010E2E8**)oa)[7];
    cBEAggressionInterface_applyAggression(agg, rb, ea->fn(oa + ea->delta), 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPunch);
#ifdef SKIP_ASM
struct sVEntry0010E3A8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);

/**
 * Reacts to a punch from another rider: punch sound and a hard-contact aggression event.
 *
 * Plays cSSXAudio_Punch(audio, attacker, self) and func_002A0A30(audio, attacker, self, 0, 0), then reports aggression
 * behaviour 2 (kAGG_HARDCONTACT) from @p attacker against @p self to cBEAggressionInterface_applyAggression (On Tour
 * cBEAggressionInterface::applyAggression), the ids from each rider's cBeing vtable slot 7.
 *
 * @param self  the rider that was punched (this)
 * @param attacker  the rider that threw the punch (On Tour attacker)
 */
extern "C" void cRider_flagEventPunch(cRider* self, void* attacker)
{
    cSSXAudio_Punch(cAudioInterface_bxAudio(), attacker, self);
    func_002A0A30(cAudioInterface_bxAudio(), attacker, self, 0, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)attacker + 0x6C0;
    sVEntry0010E3A8* eb = &(*(sVEntry0010E3A8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)self + 0x6C0;
    sVEntry0010E3A8* ea = &(*(sVEntry0010E3A8**)oa)[7];
    cBEAggressionInterface_applyAggression(agg, rb, ea->fn(oa + ea->delta), 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventPunchKnockdown);
#ifdef SKIP_ASM
struct sVEntry0010E468 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
extern "C" void cSSXAudio_KnockdownSFX(void* snd, void* obj);
void* cBEAggressionInterface_getThis();
extern "C" void cBEAggressionInterface_applyAggression(void* agg, int a, int b, int c);
extern "C" float cRiderMetrix_evFrag(char* self);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);

/**
 * Reacts to a punch that knocked this rider down: sounds, the harshest aggression event and a frag reward for the
 * attacker.
 *
 * Plays cSSXAudio_Punch(attacker, self), func_002A0A30(audio, attacker, self, 0, 1) and cSSXAudio_KnockdownSFX for the
 * attacker, reports aggression behaviour 3 (kAGG_BERTUZZI) to cBEAggressionInterface_applyAggression, then gives the attacker
 * cRider_flagEventBoostPowerUp(attacker, 2, ...) with the boost from its metrix's cRiderMetrix_evFrag (On Tour
 * cRiderMetrix::evFrag). The prototype here declares flagEventBoostPowerUp as (rider, int, float): the float goes in
 * $f12, so the call is the same as On Tour's (float powerUp, int powerSourceFlags).
 *
 * @param self  the rider that was knocked down (this)
 * @param attacker  the rider that threw the punch (On Tour attacker); it earns the frag
 */
extern "C" void cRider_flagEventPunchKnockdown(cRider* self, void* attacker)
{
    cSSXAudio_Punch(cAudioInterface_bxAudio(), attacker, self);
    func_002A0A30(cAudioInterface_bxAudio(), attacker, self, 0, 1);
    cSSXAudio_KnockdownSFX(cAudioInterface_bxAudio(), attacker);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)attacker + 0x6C0;
    sVEntry0010E468* eb = &(*(sVEntry0010E468**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)self + 0x6C0;
    sVEntry0010E468* ea = &(*(sVEntry0010E468**)oa)[7];
    cBEAggressionInterface_applyAggression(agg, rb, ea->fn(oa + ea->delta), 3);
    cRider_flagEventBoostPowerUp((cRider*)attacker, 2, cRiderMetrix_evFrag(*(char**)((char*)attacker + 0x790)));
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPathCheckpoint);
#ifdef SKIP_ASM
extern int D_005308D0[];
extern signed char D_00535C12[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evCheckpoint(void*, int);

/**
 * Handles the rider passing a path checkpoint: scores it in checkpoint events.
 *
 * Copies +0xEC into +0xE8 first. Then, unless the rules word D_005308D0 has bit 9 set (On Tour
 * cBERules::getNoCheckPoints) and only while the new-race game event (D_00535C12, cGameEvent) is 1, calls cRiderMetrix_evCheckpoint
 * (On Tour cRiderMetrix::evCheckpoint) on the rider's metrix (+0x790) with the checkpoint's first word (On Tour
 * tPathCheckpoint::TimeBonusSec).
 *
 * @param checkpoint  the checkpoint passed (On Tour tPathCheckpoint *cp): time bonus in seconds, then distance left
 */
extern "C" void cRider_flagEventPathCheckpoint(cRider* self, int* checkpoint)
{
    *(int*)((char*)self + 0xE8) = self->unk_0EC;
    if (((D_005308D0[0] >> 9) & 1) == 0)
    {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C12[0] == 1)
        {
            cRiderMetrix_evCheckpoint(self->mMetrix, *checkpoint);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventPathEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cRider_doFinishEvent(void* self);
extern "C" void cReplay_eventChapter(void* a, int b, int c);

static inline int state_0010E5D8() { return cBENewRaceInterface_newRace.cGameMode; }

struct sVEntry0010E5D8 { short delta; short index; int (*fn)(void*); };

/**
 * Handles a track-path event the rider reached: the finish line ends its race, a chapter marker tags the replay.
 *
 * A finish event (type 1) counts when the game mode (cBENewRaceInterface_newRace.cGameMode) is 0-3, or 5/6 with the
 * game-mode manager's +0x98 set (On Tour isLastHeat), or the location is 1 or 5-13, or a game path is set; then a rider
 * not yet past the line (mFinishTime < 0) gets cRider_doFinishEvent. A chapter event (type 11) calls
 * cReplay_eventChapter on the game's replay with the rider's id (cBeing vtable slot 7) and the event's second word.
 *
 * @p isPrev and @p isCur (On Tour isPrev/isCur) are not read.
 *
 * @param pathEvent  the path event node (On Tour tPathEventNode *ev): type, then a value
 * @param isPrev  On Tour isPrev; unused
 * @param isCur  On Tour isCur; unused
 */
extern "C" void cRider_flagEventPathEvent(cRider* self, int* pathEvent, int isPrev, int isCur)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char* gameModeMan = (char*)SSXApp->GameModeMan;
    int mode = *cBENewRaceInterface_getLocation(iface);
    int shouldTriggerFinishLine = 0;
    if (state_0010E5D8() == 0 || state_0010E5D8() == 1 || state_0010E5D8() == 3 || state_0010E5D8() == 2
        || (state_0010E5D8() == 5 && *(int*)(gameModeMan + 0x98) != 0)
        || (state_0010E5D8() == 6 && *(int*)(gameModeMan + 0x98) != 0)
        || mode == 5 || mode == 6 || mode == 7 || mode == 8 || mode == 9
        || mode == 10 || mode == 11 || mode == 12 || mode == 13 || mode == 1
        || cBENewRaceInterface_newRace.cGamePath != 0)
        shouldTriggerFinishLine = 1;
    if (*pathEvent == 1 && shouldTriggerFinishLine != 0)
    {
        int ok = self->mFinishTime >= 0.0f;
        if (!ok)
            cRider_doFinishEvent(self);
    }
    if (*pathEvent == 11)
    {
        char* being = (char*)self + 0x6C0;
        sVEntry0010E5D8* idSlot = &(*(sVEntry0010E5D8**)being)[7];
        void* tgt = SSXApp->Game->Replay;
        cReplay_eventChapter(tgt, idSlot->fn(being + idSlot->delta), pathEvent[1]);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventSpeedBoost);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

/**
 * Adds time to the rider's speed power-up boost and plays the boost icon sound.
 *
 * mPowerUpBoostTimer (+0x2E8) += @p amount, then cSSXAudio_PlayIconSound(audio, 0, self, 1.0f) (On Tour icon type 0;
 * flagEventTrickBoost uses 1) and func_002A3B18(audio, self, 1), which On Tour's version lacks. Virtual (cRider vtable
 * entry 13 in the kRiderVt_* numbering, after initForMission); no direct callers.
 *
 * @param amount  seconds of boost to add (On Tour sec)
 */
extern "C" void cRider_flagEventSpeedBoost(cRider* self, float amount)
{
    self->mPowerUpBoostTimer += amount;
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 0, self, 1.0f);
    func_002A3B18(cAudioInterface_bxAudio(), self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventTrickBoost);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

/**
 * Adds @p amount to the rider's mPowerUpTrickTimer and plays the icon sound for type 1.
 *
 * Also calls func_002A3B18 with 2 on the sound system. A cRider virtual (it is in six rider vtables).
 *
 * @param amount  the time to add
 */
extern "C" void cRider_flagEventTrickBoost(cRider* self, float amount)
{
    self->mPowerUpTrickTimer += amount;
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 1, self, 1.0f);
    func_002A3B18(cAudioInterface_bxAudio(), self, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventTrickMultiplier);
#ifdef SKIP_ASM
int cRider_getMotionState(void* self);
extern "C" float cRiderMetrix_evCollectMultiplier(char* self, float value);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

/**
 * Handles a collected multiplier icon: in motion states 1 and 4 hands it to the rider metrix and plays its sound.
 *
 * In those states it calls cRider_flagEventBoostPowerUp with source mask 4 and the boost returned by cRiderMetrix_evCollectMultiplier (the
 * metrix's multiplier handler, which returns 0), then cSSXAudio_PlayIconSound with type 2 (multiplier) and @p value. A
 * cRider virtual (it is in six rider vtables).
 *
 * @param value  the multiplier
 */
extern "C" void cRider_flagEventTrickMultiplier(cRider* self, float value)
{
    if (cRider_getMotionState(self) == 1 || cRider_getMotionState(self) == 4)
    {
        cRider_flagEventBoostPowerUp(self, 4, cRiderMetrix_evCollectMultiplier((char*)self->mMetrix, value));
        cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 2, self, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventAddPoints);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);
extern "C" float cRiderMetrix_evAddPoints(int);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

/**
 * Adds trick points to the rider: the metrix's points handler (cRiderMetrix_evAddPoints) and the icon sound for type 3.
 *
 * The points reach cRiderMetrix_evAddPoints untouched in a1 (the C declaration takes only self), which adds them to the score; its
 * result (0) goes to cRider_flagEventBoostPowerUp with source mask 0x10.
 */
extern "C" void cRider_flagEventAddPoints(cRider* self)
{
    cRider_flagEventBoostPowerUp(self, 0x10, cRiderMetrix_evAddPoints((int)self->mMetrix));
    cSSXAudio_PlayIconSound(cAudioInterface_bxAudio(), 3, self, 1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventLand);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRider_flagEventReaction(cRider* self, int mode, float t);
void cRider_flagEventBoostPowerUp_fi(void*, float, int) __asm__("cRider_flagEventBoostPowerUp");
void func_00111AA0_impl(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");
extern "C" float cRiderMetrix_evLand(void* self, int stance, int alternate, int style, int flag);
extern "C" int cBEStatInterface_shouldCelebrateTrick(void* self, int rider, int value);
void cReplay_eventLand_10E910(void* self, int key, void* p) __asm__("cReplay_eventLand");
extern "C" void cSSXAudio_Land(void* mgr, void* rider, float v);

struct sVE_0010E910 { short delta; short index; int (*fn)(void*); };
struct sVEf_0010E910 { short delta; short index; void (*fn)(void*, float); };

/**
 * Handles the rider landing: scores the trick, updates the uber meter, reacts, boosts and plays the landing sound.
 *
 * Takes the metrix totals before and after cRiderMetrix_evLand (On Tour cRiderMetrix::evLand(riding switch = IsRidingFakie !=
 * IsGoofyStance, @p isPipe, @p rsm, @p pressMode)), whose result is the boost earned, and logs the landing to the replay
 * (cReplay_eventLand with mAIIndex and the metrix stats at +0xFC). Uber tricks landed (Totals.NumUberTricks, metrix
 * +0x114, grew) raise mUberLevel by that many, capped at 10; reaching 10 sets mUberTimer to 60.
 *
 * func_00111AA0 (rider-handler landing effect) gets the intersection's position/normal, its surface (+0x4C) and @p speed. A
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
extern "C" void cRider_flagEventLand(cRider* self, int isPipe, int rsm, int pressMode, char* evt, float speed)
{
    char* metrix = (char*)self->mMetrix;
    int prevNumUber = *(int*)(metrix + 0x114);
    int prevNumCombo = *(int*)(metrix + 0x134);
    int prevPoints = *(int*)(metrix + 0x198);
    float powerUp = cRiderMetrix_evLand(metrix, self->IsRidingFakie != self->IsGoofyStance, isPipe, rsm, pressMode);
    char* metrixAfter = (char*)self->mMetrix;
    int curNumUber = *(int*)(metrixAfter + 0x114);
    int curNumCombo = *(int*)(metrixAfter + 0x134);
    int curPoints = *(int*)(metrixAfter + 0x198);
    cReplay_eventLand_10E910(SSXApp->Game->Replay, self->mAIIndex, metrixAfter + 0xFC);
    if (prevNumUber < curNumUber)
    {
        int uberLevel = self->mUberLevel;
        if (uberLevel < 10)
        {
            int numUber = curNumUber - prevNumUber;
            int lim = 10 - uberLevel;
            int nv = uberLevel + (numUber < lim ? numUber : lim);
            self->mUberLevel = nv;
            if (nv == 10)
                self->mUberTimer = 60.0f;
        }
    }
    func_00111AA0_impl(self->mRiderHandler, evt, evt + 0x10, *(int*)(evt + 0x4C), speed);
    char* being = ((char*)self) + 0x6C0;
    sVE_0010E910* ve = &(*(sVE_0010E910**)being)[8];
    if (ve->fn(being + ve->delta))
    {
        if (prevNumCombo < curNumCombo)
            cRider_flagEventReaction(self, 3, 0.0f);
        else if (prevNumUber < curNumUber)
            cRider_flagEventReaction(self, 2, 0.0f);
        else if (cBEStatInterface_shouldCelebrateTrick(cBE_getInterface_Fv(cBE_getBE(), 3), self->mAIIndex, curPoints - prevPoints))
            cRider_flagEventReaction(self, 1, 0.0f);
    }
    cRider_flagEventBoostPowerUp_fi(((char*)self), powerUp, 1);
    if (0.0f < powerUp)
        self->mPowerUpTrickTimer = 0.0f;
    sVEf_0010E910* vf = &((sVEf_0010E910*)self->mVtbl)[17];
    vf->fn(((char*)self) + vf->delta, speed * 0.5f);
    cSSXAudio_Land(gSSXAudio, ((char*)self), powerUp);
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
extern "C" void func_00116930(cRider* self);
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

extern sQuat_EB30 D_004FF130_EB30 __asm__("t4vect_Origin");
extern sQuat_EB30 D_004FF160_EB30 __asm__("t4vect_ZAxis");

static inline sQuat_EB30 AxisAngle_EB30(const sQuat_EB30& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_EB30(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline t4Vector GetUp_EB30(cRider* rider) { return rider->mMatrix.row[2]; }

// Clears the anim's position offset and sets its heading quaternion to -angle about t4vect_ZAxis.
static inline void ResetAnimOrientation_EB30(char* ra, float angle)
{
    *(sQuat_EB30*)(ra + 0x30) = D_004FF130_EB30;
    *(sQuat_EB30*)(ra + 0x40) = AxisAngle_EB30(D_004FF160_EB30, -angle);
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
    r.pos = Add_EB30(Scale_EB30(Rot_EB30(r.rot, Sub_EB30(a.pos, D_004FF130_EB30)), -1.0f), D_004FF130_EB30);
    return r;
}

static inline sXf_EB30 Combine_EB30(const sXf_EB30& a, const sXf_EB30& b)
{
    sXf_EB30 r;
    r.rot = QuatMul_EB30(a.rot, b.rot);
    r.pos = Add_EB30(Rot_EB30(a.rot, Sub_EB30(b.pos, D_004FF130_EB30)), a.pos);
    return r;
}


/**
 * Puts the rider into a wipeout: rumble, scoring, sounds, impact effect, then the wipeout animation and states.
 *
 * Rumbles (cBeing vtable slot 17, vibrationBump) with the hit strength (+0x30 of @p evt) * 5, takes the boost from
 * cRiderMetrix_evWipeout (On Tour cRiderMetrix::evWipeout(@p isPunch)) and passes it to cRider_flagEventBoostPowerUp(1), plays the
 * hit (cSSXAudio_WipeoutHit), a grunt and the wipeout sound (cSSXAudio_Wipeout), and spawns the impact effect (func_00111AA0) with
 * surface @p surf. Control state 13 is set first; a rider sliding sideways on a rail (RailslideMode 3 or 4) is turned +/-90
 * degrees about its up axis and its anim heading offset follows.
 *
 * It then clears RailslideMode, channels 0, 2 and 8 and PressMode, poses the first frame of @p anim
 * (cRiderAnimBase_getAIAnimFrame, func_0030ECD8), moves the rider so that pose's hip bone (mBoneHips) stays where it
 * is, stores the inverse as the anim orientation offset, plays @p anim and sets control state 8 and motion state 2.
 *
 * @param rider  the rider wiping out (this)
 * @param anim  the wipeout animation (On Tour tAIAnimID wipeoutAnim)
 * @param isPunch  On Tour isPunch: the wipeout came from a punch
 * @param surf  the surface hit (On Tour eSurfaceType surf), for the impact effect
 * @param evt  the collision (On Tour tCollideInfo): contact point/normal at +0x0/+0x20, strength at +0x30
 */
extern "C" void cRider_flagEventWipeout(cRider* rider, int anim, int isPunch, int surf, char* evt)
{
    sVE_EB30* vt = *(sVE_EB30**)&rider->mVtbl;
    vt[kRiderVt_vibrationBump].fn((char*)rider + vt[kRiderVt_vibrationBump].delta, *(float*)(evt + 0x30) * 5.0f);
    float amt = cRiderMetrix_evWipeout(rider->mMetrix, isPunch);
    cRider_flagEventBoostPowerUp(rider, 1, amt);
    cSSXAudio_WipeoutHit(cAudioInterface_bxAudio(), rider);
    cSSXAudio_PlayGrunt(cAudioInterface_bxAudio(), rider, 0, 0);
    cSSXAudio_Wipeout(cAudioInterface_bxAudio(), rider, amt);
    func_00111AA0_impl(rider->mRiderHandler, evt, evt + 0x20, surf, *(float*)(evt + 0x30));
    cRider_setControlState_EB30(rider, 13);
    if (rider->RailslideMode == 3) {
        float ang = 1.5707963705062866f;
        cRider_rotateOrientation(rider, GetUp_EB30(rider), ang);
        cRiderAnimBase_changeHeadingOffset(rider->mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)rider->mRiderAnim, 0.0f);
    } else if (rider->RailslideMode == 4) {
        float ang = -1.5707963705062866f;
        cRider_rotateOrientation(rider, GetUp_EB30(rider), ang);
        cRiderAnimBase_changeHeadingOffset(rider->mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)rider->mRiderAnim, 0.0f);
    }
    rider->RailslideMode = 0;
    func_00116930(rider);
    cRider_doLeanPoseAdjust(rider, &rider->mPosition);
    cRider_updateOrientationImplicit(rider);
    rider->mSlewed[0].target = 0.0f;
    rider->mSlewed[0].cur = 0.0f;
    rider->mSlewed[0].rate = 0.0f;
    rider->mSlewed[2].target = 0.0f;
    rider->mSlewed[2].cur = 0.0f;
    rider->mSlewed[2].rate = 0.0f;
    rider->mSlewed[8].target = 0.0f;
    rider->mSlewed[8].cur = 0.0f;
    rider->mSlewed[8].rate = 0.0f;
    rider->PressMode = 0;
    char buf[80];
    cRiderAnimBase_getAIAnimFrame(buf, rider->mRiderAnim, anim, D_004A4C08.a, rider->mSlewed[0].target);
    sXf_EB30 animPose;
    func_0030ECD8(&animPose, rider->mAnimModel, buf, 0);
    char* trk = (char*)rider->mAnimModel;
    sQuat_EB30* n = (sQuat_EB30*)((rider->mBoneHips << 4) + *(int*)(trk + 0x24));
    sXf_EB30 node;
    node.pos = sQuat_EB30(n->x * *(float*)(trk + 0x140), n->y * *(float*)(trk + 0x144),
                          n->z * *(float*)(trk + 0x148), n->w * *(float*)(trk + 0x14C));
    node.rot = *(sQuat_EB30*)(*(char**)((char*)rider->mAnimModel + 0x28) + (rider->mBoneHips << 4));
    sXf_EB30 y = Combine_EB30(node, Invert_EB30(animPose));
    *(sXf_EB30*)&rider->mPosition = Combine_EB30(*(sXf_EB30*)&rider->mPosition, y);
    cRider_updateOrientationImplicit(rider);
    sXf_EB30 w = Invert_EB30(y);
    cRiderAnimBase_changeOrientationOffset(rider->mRiderAnim, &w);
    cRiderAnimBase_play(rider->mRiderAnim, anim, 0, -1.0f);
    cRider_setControlState_EB30(rider, 8);
    cRider_setMotionState_EB30(rider, 2);
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
    vt[i].fn(obj + vt[i].delta);
}

/**
 * Handles a world (script) event on the rider.
 *
 * @p kind 1-3 call the rider's virtual slots 13, 14 and 15; 4 adds @p amt to the boost meter
 * (cRider_flagEventBoostPowerUp, source mask 8); 5 forces an automatic reset of kind 4 (cRider_checkForReset with 0,
 * 4); 6 adds (int) @p amt points (cRider_flagEventAddPoints); 0 and 7 do nothing. Called by func_002FF850 with a rider from the
 * AI's list.
 *
 * @param kind  the event id
 * @param amt  the event's value
 */
extern "C" void cRider_flagEventWorld(cRider* self, int kind, float amt)
{
    switch (kind) {
    case 0:
        break;
    case 1:
        computerV_0010F1C0(((char*)self), 13);
        break;
    case 2:
        computerV_0010F1C0(((char*)self), 14);
        break;
    case 3:
        computerV_0010F1C0(((char*)self), 15);
        break;
    case 4:
        cRider_flagEventBoostPowerUp(self, 8, amt);
        break;
    case 6:
        cRider_flagEventAddPoints_i(((char*)self), (int)amt);
        break;
    case 5:
        cRider_checkForReset(((char*)self), 0, 4);
        break;
    case 7:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", cRider_flagEventWipeoutRecover);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_evWipeoutRecover(int);
extern "C" void cRider_flagEventBoostPowerUp(cRider*, int, float);

/**
 * Tells the rider metrix the rider got up after a wipeout and adds the boost it returns.
 *
 * Calls cRider_flagEventBoostPowerUp with source mask 1 and cRiderMetrix_evWipeoutRecover's result for the rider's mMetrix. The
 * callers' second argument reaches cRiderMetrix_evWipeoutRecover untouched in a1 (the C declaration takes only self).
 */
extern "C" void cRider_flagEventWipeoutRecover(cRider* self)
{
    cRider_flagEventBoostPowerUp(self, 1, cRiderMetrix_evWipeoutRecover((int)self->mMetrix));
}
#endif

extern "C" void* cRiderMetrix_evMissionStart(int);

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventMissionStart__FPv);
#ifdef SKIP_ASM
/**
 * Tells the rider metrix a mission started: returns cRiderMetrix_evMissionStart on the rider's mMetrix.
 *
 * @return cRiderMetrix_evMissionStart's result
 */
void* cRider_flagEventMissionStart(void* self)
{
    return cRiderMetrix_evMissionStart(*(int*)((char*)self + 0x790));
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventMissionSuccess);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evMissionSuccess(int, int);
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
extern "C" void cRider_flagEventMissionSuccess(cRider* self, int peakID, int money)
{
    cRiderMetrix_evMissionSuccess((int)self->mMetrix, money);
    cBERewardInterface_awardUserBigChallenge(cBE_getInterface_Fv(cBE_getBE(), 13), self->mAIIndex, peakID);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventAddCollectible);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_evAddCash(int, int);
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
extern "C" void cRider_flagEventAddCollectible(cRider* self, int peakID)
{
    cRiderMetrix_evAddCash((int)self->mMetrix, 3);
    cBERewardInterface_awardUserCollectible(cBE_getInterface_Fv(cBE_getBE(), 13), self->mAIIndex, peakID);
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
void* cAI_InitGroupAI(void* self)
{
    return cAI_RefreshGroupAI(self);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_RefreshGroupAI);
#ifdef SKIP_ASM
struct sVE_0010F3B8 { short delta; short index; int (*fn)(void*); };
struct sEnt_0010F3B8 { int vis; int val; float dist; int c; int d; int e; int f; int g; int h; };
struct sRider_0010F3B8 { sEnt_0010F3B8 ent[6]; char pad[0x10]; int fE8; char pad2[0x4D8 - 0xEC]; float f4D8; };
struct sAI_0010F3B8 { char pad[0x28]; sRider_0010F3B8* riders[20]; int count; };
extern "C" int cRider_isLoaded(void*);
float cRider_getDistanceLeft(void* self);

// PORT: the unit declares this as returning void*; it returns nothing.
extern "C" void cAI_RefreshGroupAI_impl(sAI_0010F3B8* self) __asm__("cAI_RefreshGroupAI");
/**
 * Rebuilds every race rider's relation table (mRelations) after the AI's rider list changes.
 *
 * For rider i and slot j it sets mValid when j != i, j is a race rider and cRider_isLoaded() holds for rider
 * i (not j); fills the +0x4 word with rider j's isHuman(); sets mDistance to 1e10 and clears mAngle and the
 * five words after it. It then copies the rider's mDistToFinish to +0x4D8 and sets +0xE8 to -1.
 */
extern "C" void cAI_RefreshGroupAI_impl(sAI_0010F3B8* self)
{
    int i;
    for (i = 0; i < self->count; i++)
    {
        int j;
        for (j = 0; j < 6; j++)
        {
            sEnt_0010F3B8* relation = &self->riders[i]->ent[j];
            int visible = 0;
            if (i != j && j < self->count)
                visible = cRider_isLoaded(self->riders[i]) != 0;
            relation->vis = visible;
            if (j < self->count)
            {
                char* otherBeing = (char*)self->riders[j] + 0x6C0;
                sVE_0010F3B8* ve = &(*(sVE_0010F3B8**)otherBeing)[8];
                self->riders[i]->ent[j].val = ve->fn(otherBeing + ve->delta);
            }
            self->riders[i]->ent[j].dist = 10000000000.0f;
            self->riders[i]->ent[j].c = 0;
            self->riders[i]->ent[j].d = 0;
            self->riders[i]->ent[j].e = 0;
            self->riders[i]->ent[j].f = 0;
            self->riders[i]->ent[j].g = 0;
            self->riders[i]->ent[j].h = 0;
        }
        int none = -1;
        self->riders[i]->f4D8 = cRider_getDistanceLeft(self->riders[i]);
        self->riders[i]->fE8 = none;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateGroupAI);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* agg, int a, int b);
extern "C" float func_0031C228(float x);
struct sComputer_0010F878;
extern "C" void cAI_UpdateAggression(sComputer_0010F878* self);
extern "C" void cAI_UpdateRankingPlacement(cAI* self);

struct sVec_0010F560 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVEntry0010F560 {
    short delta;
    short index;
    sVec_0010F560* (*fn)(void*);
};

struct sRiderInfo_0010F560 {
    int active;     // 0x0
    int f4;         // 0x4
    float dist;     // 0x8
    float angle;    // 0xC
    char pad10[0xC];
    int aggressive; // 0x1C
    int f20;        // 0x20
};

struct sRider_0010F560 {
    sRiderInfo_0010F560 info[6];  // 0x0
    char padD8[0xDC - 0xD8];
    float minDist;                // 0xDC
    float maxDist;                // 0xE0

    sVec_0010F560* getPos()
    {
        char* o = (char*)this + 0x6C0;
        sVEntry0010F560* e = &(*(sVEntry0010F560**)o)[5];
        return e->fn(o + e->delta);
    }
};

struct sComputer_0010F560 {
    char pad0[0x8];
    int frame;                       // 0x8
    char padC[0x28 - 0xC];
    sRider_0010F560* riders[0x14];   // 0x28
    int count;                       // 0x78
    char pad7C[0x84 - 0x7C];
    int skip;                        // 0x84
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_0010F560 sub_0010F560(const sVec_0010F560& a, const sVec_0010F560& b)
{
    sVec_0010F560 r;
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
 * NumberOfRidersAllocated - NumberOfActorsAllocated riders (at least 2): each rider's range pair at +0xDC/+0xE0 is
 * reset to -50000/50000, and for each valid pair (i, j) (rider i's opponent record j, 0x24 bytes from +0x0, field 0
 * set) it stores the planar distance in both records, the bearing from j to i in j's record and the opposite bearing
 * (wrapped to -pi..pi) in i's, and marks the pair aggressive when cBEAggressionInterface_getAggression (On Tour getAggression) is at least 2.
 *
 * Ends with cAI_UpdateAggression (On Tour cAI::UpdateAggression).
 */
extern "C" void cAI_UpdateGroupAI(sComputer_0010F560* self)
{
    if (self->frame % 6 != 0) {
        return;
    }
    cAI_UpdateRankingPlacement((cAI*)self);
    int n = self->count - self->skip;
    if (n < 2) {
        return;
    }
    for (int i = 0; i < n; i++) {
        self->riders[i]->minDist = -50000.0f;
        self->riders[i]->maxDist = 50000.0f;
        for (int j = i + 1; j < n; j++) {
            if (self->riders[i]->info[j].active == 0) {
                continue;
            }
            sVec_0010F560 delta = sub_0010F560(*self->riders[i]->getPos(), *self->riders[j]->getPos());
            float dist;
            float dx = delta.x;
            float dy = delta.y;
            float sq = dx * dx + dy * dy;
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(sq));
            float ang = atan2_0010F560(dy, dx);
            self->riders[i]->info[j].dist = dist;
            self->riders[j]->info[i].dist = dist;
            self->riders[i]->info[j].angle = wrap_0010F560(ang + 3.1415927410125732f);
            self->riders[j]->info[i].angle = ang;
            if (cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), i, j) >= 2) {
                self->riders[i]->info[j].aggressive = 1;
            } else {
                self->riders[i]->info[j].aggressive = 0;
            }
        }
    }
    cAI_UpdateAggression((sComputer_0010F878*)self);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateAggression);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* self, int a, int b);

struct sComputer_0010F878
{
    char pad0[0x28];
    char* mRiders[20];  // 0x28
    int mCount;         // 0x78
    int pad7C;          // 0x7C
    int f80;            // 0x80
};

/**
 * Refreshes each rider's aggression toward the others and picks a chase target.
 *
 * Skipped when NumberOfComputersAllocated (+0x80) is 0. For each rider i of NumberOfRidersAllocated: clears its chase
 * flag (+0xF0) and target (+0xF8 = -1), then for every other rider j stores cBEAggressionInterface_getAggression (On Tour
 * cBEAggressionInterface::getAggression(i, j)) in +0xFC and, at level 4 (kAGG_CHASE), sets the chase flag and target j.
 * Only the last j's level survives in +0xFC (likely an oversight).
 */
extern "C" void cAI_UpdateAggression(sComputer_0010F878* self)
{
    if (self->f80 <= 0) return;
    for (int i = 0; i < self->mCount; i++)
    {
        *(int*)(self->mRiders[i] + 0xF0) = 0;
        *(int*)(self->mRiders[i] + 0xF8) = -1;
        for (int j = 0; j < self->mCount; j++)
        {
            if (i == j) continue;
            int curAgg = cBEAggressionInterface_getAggression(cBEAggressionInterface_getThis(), i, j);
            if (curAgg == 4)
            {
                *(int*)(self->mRiders[i] + 0xF0) = 1;
                *(int*)(self->mRiders[i] + 0xF8) = j;
            }
            *(int*)(self->mRiders[i] + 0xFC) = curAgg;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_UpdateRankingPlacement);
#ifdef SKIP_ASM
struct sAI_0010F998 { char pad[0x28]; cRider* riders[19]; int mode; int count; };
extern "C" int cAI_isAILoadedAndReady(void* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_ChangePosition(void* mgr, void* rider, void* next, void* prev, int oldPos, int newPos);
extern "C" void func_003E6328(int n, int* keys, int* idx);
float cRider_getDistanceLeft(void* self);

/**
 * Re-ranks the riders by the AI's RaceRankType and tells the audio system about each change of place.
 *
 * Does nothing for rank type 0 or before cAI_isAILoadedAndReady. Type 1 ranks racers by -(distance left + 20 x their
 * current place unk_0EC), so a rider must gain 20 units to pass; type 2 ranks by the metrix's +0x198 points. Keys are
 * sorted largest first (func_003E6328). For each new place that differs from unk_0EC, cSSXAudio_ChangePosition gets the
 * rider, the riders after and before it and the old and new place; the place is stored in unk_0EC. Actors get key
 * 0xFFFF in type 1, which sorts them ahead of every racer, but -0xFFFF in type 2. The two branches on mFinishTime >= 10
 * compute the same key.
 *
 * @param aiObj  the cAI (this)
 */
extern "C" void cAI_UpdateRankingPlacement(cAI* aiObj)
{
    sAI_0010F998* self = (sAI_0010F998*)aiObj;
    if (self->mode == 0)
        return;
    if (cAI_isAILoadedAndReady(self) == 0)
        return;
    int idx[8];
    int keys[8];
    int i;
    for (i = 0; i < self->count; i++)
    {
        idx[i] = i;
        cRider* rider = self->riders[i];
        switch (self->mode)
        {
        case 1:
            if (rider->mIsActor)
                keys[i] = 0xFFFF;
            else
            {
                int far = rider->mFinishTime >= 10.0f;
                if (far)
                    keys[i] = -(int)(cRider_getDistanceLeft(rider) + (float)(rider->unk_0EC * 20));
                else
                    keys[i] = -(int)(cRider_getDistanceLeft(rider) + (float)(rider->unk_0EC * 20));
            }
            break;
        case 2:
            if (rider->mIsActor)
                keys[i] = -0xFFFF;
            else
                keys[i] = *(int*)((char*)rider->mMetrix + 0x198);
            break;
        }
    }
    // Re-read count into a local: GCSE makes this load redundant with the loop-test
    // loads and leaves a copy (a0 = v0) after each of them.
    int n = self->count;
    func_003E6328(n, keys, idx);
    int j;
    for (j = 0; j < self->count; j++)
    {
        if (self->riders[idx[j]]->unk_0EC != j)
        {
            cRider* next = 0;
            cRider* prev = 0;
            if (j > 0)
                prev = self->riders[idx[j - 1]];
            if (j < self->count - 1)
                next = self->riders[idx[j + 1]];
            cSSXAudio_ChangePosition(cAudioInterface_bxAudio(), self->riders[idx[j]], next, prev, self->riders[idx[j]]->unk_0EC, j);
        }
        self->riders[idx[j]]->unk_0EC = j;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_ridersOnPath);
#ifdef SKIP_ASM
/**
 * Counts the riders whose AI path (+0xAB8, the word cRider_getAIPath returns) equals @p value.
 *
 * Goes over the AI's NumberOfRidersAllocated Riders. Nothing in the source calls it.
 *
 * @param value  the value to look for (cComputer_scorePath passes a path pointer to the similar cAI_aiRidersOnPath)
 * @return the number of matching riders
 */
extern "C" int cAI_ridersOnPath(cAI* self, int value)
{
    int count = 0;
    for (int i = 0; i < self->NumberOfRidersAllocated; i++) {
        cRider* rider = *(cRider**)(((char*)self) + 0x28 + i * 4);
        if (*(int*)((char*)rider + 0xAB8) == value) {
            count++;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAI_aiRidersOnPath);
#ifdef SKIP_ASM
struct sVEntry0010FC30 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sAi0010FC30
{
    char pad0[0x28];
    char* mRiders[20];
    int mCount;
};

/**
 * Counts the computer riders whose AI path (+0xAB8, the word cRider_getAIPath returns) equals @p team.
 *
 * A rider counts when its isComputer virtual (slot 9) returns true. cComputer_scorePath passes a path pointer as @p
 * team.
 *
 * @param team  the value to look for (a path pointer)
 * @return the number of matching computer riders
 */
extern "C" int cAI_aiRidersOnPath(sAi0010FC30* self, int team)
{
    int n = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* obj = self->mRiders[i] + 0x6C0;
        sVEntry0010FC30* vtEntry = &(*(sVEntry0010FC30**)obj)[9];
        if (vtEntry->fn(obj + vtEntry->delta))
        {
            if (*(int*)(self->mRiders[i] + 0xAB8) == team)
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
    fDE0 = 6;
    fDE4 = 14;
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
        "__15cCompPart30_cmpi = cCheatFX_cCheatFX\n");

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
    cRiderHandler_gainMotionState(self, old);
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
 * Ground, air, wipeout, railslide and handplant have one; the static motion (3) and other values do nothing.
 *
 * @param a1  the motion state being switched to, passed on to loseFocus (On Tour next)
 */
extern "C" void cRiderHandler_loseMotionState(cRiderHandler* self, int a1)
{
    switch (self->MotionState) {
    case 0:
        cGroundMotion_loseFocus(self, a1);
        break;
    case 1:
        cAirMotion_loseFocus_a1((char*)self + 0x20, a1);
        break;
    case 2:
        cWipeoutMotion_loseFocus_a1((char*)self + 0x30, a1);
        break;
    case 4:
        cRailslideMotion_loseFocus((char*)self + 0xB0, a1);
        break;
    case 5:
        cHandplantMotion_loseFocus((char*)self + 0x110, a1);
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
 * Ground, air, wipeout, railslide and handplant have one; the static motion (3) and other values do nothing.
 *
 * @param prev  the motion state being left, passed on to gainFocus
 */
extern "C" void cRiderHandler_gainMotionState(cRiderHandler* self, int prev)
{
    switch (self->MotionState) {
    case 0:
        cGroundMotion_gainFocus(self, prev);
        break;
    case 1:
        cAirMotion_gainFocus((char*)self + 0x20, prev);
        break;
    case 2:
        cWipeoutMotion_gainFocus((char*)self + 0x30, prev);
        break;
    case 4:
        cRailslideMotion_gainFocus((char*)self + 0xB0, prev);
        break;
    case 5:
        cHandplantMotion_gainFocus((char*)self + 0x110, prev);
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
extern "C" void cRiderHandler_stepMotion(cRiderHandler* self)
{
    switch (self->MotionState) {
    case 0:
        cGroundMotion_step(self);
        break;
    case 1:
        cAirMotion_step((char*)self + 0x20);
        break;
    case 2:
        cWipeoutMotion_step((char*)self + 0x30);
        break;
    case 3:
        cStaticMotion_step((char*)self + 0xA0);
        break;
    case 4:
        cRailslideMotion_step((char*)self + 0xB0);
        break;
    case 5:
        cHandplantMotion_step((char*)self + 0x110);
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
extern "C" void cRiderHandler_resolveMotion(cRiderHandler* self)
{
    switch (self->MotionState) {
    case 0:
        cGroundMotion_resolve(self);
        break;
    case 1:
        cAirMotion_resolve((char*)self + 0x20);
        break;
    case 2:
        cWipeoutMotion_resolve((char*)self + 0x30);
        break;
    case 3:
        cStaticMotion_resolve((char*)self + 0xA0);
        break;
    case 4:
        cRailslideMotion_resolve((char*)self + 0xB0);
        break;
    case 5:
        cHandplantMotion_resolve((char*)self + 0x110);
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
    cRiderHandler_gainControlState(self, old);
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
extern "C" void cRiderHandler_loseControlState(cRiderHandler* self, int a1)
{
    switch (self->ControlState) {
    case 0:
        cCruiseControl_loseFocus((char*)self + 0x1C0, a1);
        break;
    case 1:
        cBoardPressControl_loseFocus((char*)self + 0x1D0, a1);
        break;
    case 2:
        cPrewindControl_loseFocus_a1((char*)self + 0x1F0, a1);
        break;
    case 4:
        cNaturalAirControl_loseFocus((char*)self + 0x210, a1);
        break;
    case 5:
        cSpinControl_loseFocus((char*)self + 0x230, a1);
        break;
    case 7:
        cRailslideControl_loseFocus((char*)self + 0x2B0, a1);
        break;
    case 8:
        cWipeoutControl_loseFocus((char*)self + 0x2C0, a1);
        break;
    case 11:
        cHandplantControl_loseFocus((char*)self + 0x370, a1);
        break;
    case 3: case 6: case 9: case 10: case 12: case 13:
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
 * Every control state but bump (3) and void (13) has one.
 *
 * @param prev  the control state being left, passed on to gainFocus
 */
// Forwards to the handler for the AI's current state.
extern "C" void cRiderHandler_gainControlState(cRiderHandler* self, int prev)
{
    switch (self->ControlState) {
    case 0:
        cCruiseControl_gainFocus((char*)self + 0x1C0);
        break;
    case 1:
        cBoardPressControl_gainFocus((char*)self + 0x1D0);
        break;
    case 2:
        cPrewindControl_gainFocus((char*)self + 0x1F0, prev);
        break;
    case 4:
        cNaturalAirControl_gainFocus((char*)self + 0x210, prev);
        break;
    case 5:
        cSpinControl_gainFocus((char*)self + 0x230, prev);
        break;
    case 6:
        cGateControl_gainFocus((char*)self + 0x290, prev);
        break;
    case 7:
        cRailslideControl_gainFocus((char*)self + 0x2B0, prev);
        break;
    case 8:
        cWipeoutControl_gainFocus((char*)self + 0x2C0, prev);
        break;
    case 9:
        cResetControl_gainFocus((char*)self + 0x350, prev);
        break;
    case 10:
        cFinishLineControl_gainFocus((char*)self + 0x360, prev);
        break;
    case 11:
        cHandplantControl_gainFocus((char*)self + 0x370, prev);
        break;
    case 12:
        cUberGrindControl_gainFocus((char*)self + 0x390, prev);
        break;
    case 3:
    case 13:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_updateControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cCruiseControl_update(void* p, int a1);
extern "C" void cBoardPressControl_update(void* p, int a1);
extern "C" void cPrewindControl_update(void* p, int a1);
extern "C" void cBumpControl_update(void* p, int a1);
extern "C" void cNaturalAirControl_update(void* p, int a1);
extern "C" void cSpinControl_update(void* p, int a1);
extern "C" void cGateControl_update(void* p, int a1);
extern "C" void cRailslideControl_update(void* p, int a1);
extern "C" void cWipeoutControl_update(void* p, int a1);
extern "C" void cResetControl_update(void* p, int a1);
extern "C" void cFinishLineControl_update(void* p, int a1);
extern "C" void cHandplantControl_update(void* p, int a1);
extern "C" void cUberGrindControl_update(void* p, int a1);
/**
 * Feeds this frame's control command to the current control part (by ControlState) through its update
 * function.
 *
 * States 0-12 each have one; the void state (13) does nothing.
 *
 * @param cmd  the control command (On Tour const tControlCmd&), passed on to the part's update
 */
// Forwards to the second handler for the AI's current state.
extern "C" void cRiderHandler_updateControl(cRiderHandler* self, int cmd)
{
    switch (self->ControlState) {
    case 0:
        cCruiseControl_update((char*)self + 0x1C0, cmd);
        break;
    case 1:
        cBoardPressControl_update((char*)self + 0x1D0, cmd);
        break;
    case 2:
        cPrewindControl_update((char*)self + 0x1F0, cmd);
        break;
    case 3:
        cBumpControl_update((char*)self + 0x200, cmd);
        break;
    case 4:
        cNaturalAirControl_update((char*)self + 0x210, cmd);
        break;
    case 5:
        cSpinControl_update((char*)self + 0x230, cmd);
        break;
    case 6:
        cGateControl_update((char*)self + 0x290, cmd);
        break;
    case 7:
        cRailslideControl_update((char*)self + 0x2B0, cmd);
        break;
    case 8:
        cWipeoutControl_update((char*)self + 0x2C0, cmd);
        break;
    case 9:
        cResetControl_update((char*)self + 0x350, cmd);
        break;
    case 10:
        cFinishLineControl_update((char*)self + 0x360, cmd);
        break;
    case 11:
        cHandplantControl_update((char*)self + 0x370, cmd);
        break;
    case 12:
        cUberGrindControl_update((char*)self + 0x390, cmd);
        break;
    case 13:
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
extern const char* D_00459DC8[];
extern char str_Invalid[];

/**
 * Returns the name of the current control state, "Cruise" to "Void", for debug display.
 *
 * The bound check accepts 0-14 but the table D_00459DC8 has 14 entries, so CS_Num (14, the constructor's
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
    return D_00459DC8[state];
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
extern "C" void func_002D4BE0(void*);
extern "C" void cPlayerIndicatorFX_reset(void*);
extern "C" void func_002DF3B0(void*);
extern "C" void cPunchFX_reset(void*);
void cCheatFX_reset(void*);
// PORT: cWorldPainterMan_reset is defined with an unused self param; this caller passes none.
void cWorldPainterMan_reset_noarg() __asm__("cWorldPainterMan_reset__FPv");
void* cGame_getSplashManager(void* self, int i);

struct sVEntry00111890 { short delta; short index; void (*fn)(void*); };

/**
 * Resets all of the rider's visual FX parts, the world painter and the splash manager of the rider's view.
 *
 * Calls the reset entry of each of the 11 FX parts (BoardWake to Cheat; for most parts the function named
 * readFromReplayFrame, called without a stream), then cWorldPainterMan_reset() and, when the rider has a view
 * (mViewIndex >= 0), virtual slot 14 of cGame::SplashManager[mViewIndex].
 */
extern "C" void cRiderHandler_resetRiderFX(cRiderHandler* self)
{
    cBoardWakeFX_reset(((char*)self) + 0x3B0);
    cBoardSparkFX_reset(((char*)self) + 0x470);
    cBoardTrailFX_reset(((char*)self) + 0x520);
    cSpeedBoostFX_reset(((char*)self) + 0x610);
    cTrickBoostFX_reset(((char*)self) + 0x9C0);
    cAirStreamerFX_reset(((char*)self) + 0xAD0);
    func_002D4BE0(((char*)self) + 0xAF0);
    cPlayerIndicatorFX_reset(((char*)self) + 0xB00);
    func_002DF3B0(((char*)self) + 0xB40);
    cPunchFX_reset(((char*)self) + 0xC70);
    cCheatFX_reset(((char*)self) + 0xD20);
    cWorldPainterMan_reset_noarg();
    int id = self->GroundMotion.mRider->mViewIndex;
    if (id >= 0)
    {
        char* obj = (char*)cGame_getSplashManager(SSXApp->Game, id);
        if (obj != 0)
        {
            sVEntry00111890* vt = *(sVEntry00111890**)(obj + 0xC);
            vt[14].fn(obj + vt[14].delta);
        }
    }
}
#endif

extern "C" void* func_002E23E0(void*);

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_updateRiderFX);
#ifdef SKIP_ASM
extern "C" void func_002D4C08(void*);
extern "C" void cBoardSparkFX_update(void*);
extern "C" void cBoardWakeFX_update(void*);
extern "C" void func_002DF920(void*);
extern "C" void cPlayerIndicatorFX_update(void*);
extern "C" void cSpeedBoostFX_update(void*);
extern "C" void cBoardTrailFX_update(void*);
extern "C" void cTrickBoostFX_update(void*);
extern "C" void cAirStreamerFX_update(void*);
extern "C" void cPunchFX_update(void*);
extern "C" void cPlayerLights_Clear(void*);
extern "C" void cCheatFX_update(void*);
extern int cRenderToggles_m_DisableAIVisualFX;

struct sVE_00111948 { short delta; short index; int (*fn)(void*); };

/**
 * Updates the rider's player lights and all 11 visual FX parts for this frame.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (cRenderToggles_m_DisableAIVisualFX) is on. The lights
 * (PlayerLights, +0xD30) are reset first by cPlayerLights_Clear. On Tour's version takes an a_bIsLocalHuman flag
 * that this one lacks.
 */
extern "C" void cRiderHandler_updateRiderFX(cRiderHandler* self)
{
    if (cRenderToggles_m_DisableAIVisualFX != 0) {
        cRider* rider = self->GroundMotion.mRider;
        char* riderBeing = (char*)&rider->mVtbl;
        sVE_00111948* isComputerSlot = &(*(sVE_00111948**)riderBeing)[kRiderVt_isComputer];
        if (isComputerSlot->fn(riderBeing + isComputerSlot->delta) != 0)
            return;
    }
    cPlayerLights_Clear((char*)self + 0xD30);
    cBoardWakeFX_update((char*)self + 0x3B0);
    cBoardSparkFX_update((char*)self + 0x470);
    cBoardTrailFX_update((char*)self + 0x520);
    cSpeedBoostFX_update((char*)self + 0x610);
    cTrickBoostFX_update((char*)self + 0x9C0);
    cAirStreamerFX_update((char*)self + 0xAD0);
    func_002D4C08((char*)self + 0xAF0);
    cPlayerIndicatorFX_update((char*)self + 0xB00);
    func_002DF920((char*)self + 0xB40);
    cPunchFX_update((char*)self + 0xC70);
    cCheatFX_update((char*)self + 0xD20);
}
#endif
//100%
INCLUDE_ASM("ai/computer", cRiderHandler_renderRiderFX);
#ifdef SKIP_ASM
extern "C" void func_002D5048(void*);
extern "C" void cBoardSparkFX_render(void*);
extern "C" void cBoardWakeFX_render(void*);
extern "C" void func_002E24D0(void*);
extern "C" void cPlayerIndicatorFX_render(void*);
extern "C" void cSpeedBoostFX_render(void*);
extern "C" void cBoardTrailFX_render(void*);
extern "C" void cTrickBoostFX_render(void*);
extern "C" void cAirStreamerFX_render(void*);
extern "C" void cPunchFX_render(void*);
void cCheatFX_render(void*);
extern int cRenderToggles_m_DisableAIVisualFX;

struct sVE_001119F8 { short delta; short index; int (*fn)(void*); };

/**
 * Draws all 11 of the rider's visual FX parts.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (cRenderToggles_m_DisableAIVisualFX) is on.
 */
extern "C" void cRiderHandler_renderRiderFX(cRiderHandler* self)
{
    if (cRenderToggles_m_DisableAIVisualFX != 0) {
        cRider* rider = self->GroundMotion.mRider;
        char* riderBeing = (char*)&rider->mVtbl;
        sVE_001119F8* isComputerSlot = &(*(sVE_001119F8**)riderBeing)[kRiderVt_isComputer];
        if (isComputerSlot->fn(riderBeing + isComputerSlot->delta) != 0)
            return;
    }
    cBoardWakeFX_render((char*)self + 0x3B0);
    cBoardSparkFX_render((char*)self + 0x470);
    cBoardTrailFX_render((char*)self + 0x520);
    cSpeedBoostFX_render((char*)self + 0x610);
    cTrickBoostFX_render((char*)self + 0x9C0);
    cAirStreamerFX_render((char*)self + 0xAD0);
    func_002D5048((char*)self + 0xAF0);
    cPlayerIndicatorFX_render((char*)self + 0xB00);
    func_002E24D0((char*)self + 0xB40);
    cPunchFX_render((char*)self + 0xC70);
    cCheatFX_render((char*)self + 0xD20);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00111AA0__FPv);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The project names this func_00111AA0__FPv (one
// void* param), but every caller passes (self, vecA, vecB, int, float) and the
// body forwards them to func_002E23E0 with an extra 0 flag. The unit declares
// func_002E23E0 as (void*), so both are bound by asm label.
void func_002E23E0_impl(void* self, void* a, void* b, int c, int flag, float x) __asm__("func_002E23E0");

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");

/**
 * Spawns the rider handler's landing / impact effect: forwards to func_002E23E0 on the effect at self + 0xB40.
 *
 * Callers pass the cRiderHandler as self; the extra 0 flag is inserted before @p x.
 *
 * @param pos  the contact position
 * @param dir  the contact normal
 * @param surfaceType  the surface type
 * @param x  the impact speed
 */
void func_00111AA0_impl(void* self, void* pos, void* dir, int surfaceType, float x)
{
    func_002E23E0_impl((char*)self + 0xB40, pos, dir, surfaceType, 0, x);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_00111AC0 {
public:
    virtual void Write(void* p, int n);
};

extern char gPaths[];
extern "C" void func_00104D68(void* self, void* s);
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
 * line and wall hints (each a world-object ID from +0x150 of what it points at, or -1, then the hint's 16
 * bytes); MotionState and ControlState; each motion and control part that has state, and cCheatFX; 0x20 bytes
 * each of mAIPathHint and mTrackPathHint; and the IDs of the two paths at +0xAB4 and +0xAB8.
 *
 * @param data  the replay stream (On Tour cReplayFramePtr*), written through its virtual Write
 */
extern "C" void cRiderHandler_writeToReplayFrame(cRiderHandler* self, char* data)
{
    cStream_00111AC0* st = (cStream_00111AC0*)data;
    st->Write(self->GroundMotion.mRider, 1728);
    func_00104D68(*(void**)((char*)self->GroundMotion.mRider + 0x784), data);
    cAnimModel_writeToReplayFrame(*(void**)((char*)self->GroundMotion.mRider + 0x780), data);
    cAirPredictor_writeToReplayFrame(*(void**)((char*)self->GroundMotion.mRider + 0x788), data);
    cRiderMetrix_writeToReplayFrame(*(void**)((char*)self->GroundMotion.mRider + 0x790), data);
    unsigned int id0 = 0xFFFFFFFF;
    char* line = **(char***)((char*)self->GroundMotion.mRider + 0x864);
    if (line != 0) {
        id0 = *(unsigned int*)(line + 0x150);
    }
    st->Write(&id0, 4);
    st->Write(*(void**)((char*)self->GroundMotion.mRider + 0x864), 0x10);
    unsigned int id1 = 0xFFFFFFFF;
    char* wall = **(char***)((char*)self->GroundMotion.mRider + 0x868);
    if (wall != 0) {
        id1 = *(unsigned int*)(wall + 0x150);
    }
    st->Write(&id1, 4);
    st->Write(*(void**)((char*)self->GroundMotion.mRider + 0x868), 0x10);
    st->Write(((char*)self) + 0xDE0, 4);
    st->Write(((char*)self) + 0xDE4, 4);
    cGroundMotion_writeToReplayFrame(((char*)self), data);
    cAirMotion_writeToReplayFrame(((char*)self) + 0x20, data);
    cWipeoutMotion_writeToReplayFrame(((char*)self) + 0x30, data);
    cRailslideMotion_writeToReplayFrame(((char*)self) + 0xB0, data);
    cHandplantMotion_writeToReplayFrame(((char*)self) + 0x110, data);
    cBoardPressControl_writeToReplayFrame(((char*)self) + 0x1D0, data);
    cNaturalAirControl_writeToReplayFrame(((char*)self) + 0x210, data);
    cSpinControl_writeToReplayFrame(((char*)self) + 0x230, data);
    cGateControl_writeToReplayFrame(((char*)self) + 0x290, data);
    cRailslideControl_writeToReplayFrame(((char*)self) + 0x2B0, data);
    cWipeoutControl_writeToReplayFrame(((char*)self) + 0x2C0, data);
    cResetControl_writeToReplayFrame(((char*)self) + 0x350, data);
    cFinishLineControl_writeToReplayFrame(((char*)self) + 0x360, data);
    cHandplantControl_writeToReplayFrame(((char*)self) + 0x370, data);
    cUberGrindControl_writeToReplayFrame(((char*)self) + 0x390, data);
    cCheatFX_writeToReplayFrame(((char*)self) + 0xD20, data);
    st->Write(*(void**)((char*)self->GroundMotion.mRider + 0xABC), 0x20);
    st->Write(*(void**)((char*)self->GroundMotion.mRider + 0xAC0), 0x20);
    int ids[2];
    ids[0] = cPathSys_getTrackPathIndex(gPaths, *(void**)((char*)self->GroundMotion.mRider + 0xAB4));
    ids[1] = cPathSys_getAIPathIndex(gPaths, *(void**)((char*)self->GroundMotion.mRider + 0xAB8));
    st->Write(ids, 8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_00111D98 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* p, int n);
    virtual void Read(void* p, int n);
};

struct sRefSet_00111D98 {
    char pad_0x0[0xC];
    unsigned int* refs;     // 0xC
};

struct sRefWorld_00111D98 {
    char pad_0x0[0x8];
    sRefSet_00111D98** sets;    // 0x8
};

extern char gPaths[];
extern "C" void func_00104DE0(void* self, void* s);
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
extern "C" void func_002D4BE0(void*);
extern "C" void cPlayerIndicatorFX_reset(void*);
extern "C" void cPunchFX_reset(void*);
extern "C" void cCheatFX_readFromReplayFrame(void* self, void* s);
extern "C" void* cPathSys_getTrackPath(void* tree, int id);
extern "C" void* cPathSys_getAIPath(void* tree, int id);
extern "C" void cRiderAnimBase_setupAnimation(void* self, int a1);
extern "C" void cAnimModel_buildSkelMatrices(void* self);
extern "C" void cRider_updateCollideInfo(void* self);

static inline void* idxToPtr_00111D98(unsigned int i)
{
    return (void*)(i << 2);
}

static inline void* refToPtr_00111D98(unsigned int v)
{
    unsigned int i = v >> 8;
    if (i == 0) {
        return 0;
    }
    return idxToPtr_00111D98(i);
}

struct sRef_00111D98 {
    unsigned int id;
    sRef_00111D98() : id(0xFFFFFFFF) {}

    void* get()
    {
        sRefSet_00111D98* set = ((sRefWorld_00111D98*)cWorld_m_pTheWorld->m_pWorldCache)->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return refToPtr_00111D98(set->refs[id >> 8]);
    }
};

struct sQuad_00111D98 {
    int v[4];
} __attribute__((aligned(16)));

/**
 * Restores the rider from one replay frame, reading back what cRiderHandler_writeToReplayFrame() wrote.
 *
 * Turns the saved world-object IDs back into hint pointers (through the world cache) and the path IDs into
 * the paths at +0xAB4 and +0xAB8, resets the FX parts (all but the dynamic spray, which resetRiderFX() does
 * reset) and reads cCheatFX. It then copies mPosition into the focus box when there is one, updates the rider
 * anim (cRiderAnimBase_setupAnimation), rebuilds the skeleton matrices and refreshes the body spheres (cRider_updateCollideInfo).
 *
 * @param data  the replay stream (On Tour cReplayFramePtr*), read through its virtual Read
 */
extern "C" void cRiderHandler_readFromReplayFrame(cRiderHandler* self, char* data)
{
    cStream_00111D98* st = (cStream_00111D98*)data;
    st->Read(self->GroundMotion.mRider, 1728);
    func_00104DE0(self->GroundMotion.mRider->mRiderAnim, data);
    cAnimModel_readFromReplayFrame(self->GroundMotion.mRider->mAnimModel, data);
    cAirPredictor_readFromReplayFrame(self->GroundMotion.mRider->mAirPredictor, data);
    cRiderMetrix_readFromReplayFrame(self->GroundMotion.mRider->mMetrix, data);
    int ids[2];
    sRef_00111D98 lineId;
    st->Read(&lineId, 4);
    st->Read(self->GroundMotion.mRider->mLineHint, 0x10);
    if (~lineId.id != 0) {
        *(void**)self->GroundMotion.mRider->mLineHint = lineId.get();
    }
    sRef_00111D98 wallId;
    st->Read(&wallId, 4);
    st->Read(self->GroundMotion.mRider->mWallHint, 0x10);
    if (~wallId.id != 0) {
        *(void**)self->GroundMotion.mRider->mWallHint = wallId.get();
    }
    st->Read(((char*)self) + 0xDE0, 4);
    st->Read(((char*)self) + 0xDE4, 4);
    cGroundMotion_readFromReplayFrame(((char*)self), data);
    cAirMotion_readFromReplayFrame(((char*)self) + 0x20, data);
    cWipeoutMotion_readFromReplayFrame(((char*)self) + 0x30, data);
    cRailslideMotion_readFromReplayFrame(((char*)self) + 0xB0, data);
    cHandplantMotion_readFromReplayFrame(((char*)self) + 0x110, data);
    cBoardPressControl_readFromReplayFrame(((char*)self) + 0x1D0, data);
    cNaturalAirControl_readFromReplayFrame(((char*)self) + 0x210, data);
    cSpinControl_readFromReplayFrame(((char*)self) + 0x230, data);
    cGateControl_readFromReplayFrame(((char*)self) + 0x290, data);
    cRailslideControl_readFromReplayFrame(((char*)self) + 0x2B0, data);
    cWipeoutControl_readFromReplayFrame(((char*)self) + 0x2C0, data);
    cResetControl_readFromReplayFrame(((char*)self) + 0x350, data);
    cFinishLineControl_readFromReplayFrame(((char*)self) + 0x360, data);
    cHandplantControl_readFromReplayFrame(((char*)self) + 0x370, data);
    cUberGrindControl_readFromReplayFrame(((char*)self) + 0x390, data);
    cBoardWakeFX_reset(((char*)self) + 0x3B0);
    cBoardSparkFX_reset(((char*)self) + 0x470);
    cBoardTrailFX_reset(((char*)self) + 0x520);
    cSpeedBoostFX_reset(((char*)self) + 0x610);
    cTrickBoostFX_reset(((char*)self) + 0x9C0);
    cAirStreamerFX_reset(((char*)self) + 0xAD0);
    func_002D4BE0(((char*)self) + 0xAF0);
    cPlayerIndicatorFX_reset(((char*)self) + 0xB00);
    cPunchFX_reset(((char*)self) + 0xC70);
    cCheatFX_readFromReplayFrame(((char*)self) + 0xD20, data);
    st->Read(self->GroundMotion.mRider->mAIPathHint, 0x20);
    st->Read(self->GroundMotion.mRider->mTrackPathHint, 0x20);
    st->Read(ids, 8);
    *(void**)((char*)self->GroundMotion.mRider + 0xAB4) = cPathSys_getTrackPath(gPaths, ids[0]);
    *(void**)((char*)self->GroundMotion.mRider + 0xAB8) = cPathSys_getAIPath(gPaths, ids[1]);
    sQuad_00111D98* focusBox = (sQuad_00111D98*)self->GroundMotion.mRider->mFocusBox;
    if (focusBox != 0) {
        *focusBox = *(sQuad_00111D98*)&self->GroundMotion.mRider->mPosition;
    }
    cRiderAnimBase_setupAnimation(self->GroundMotion.mRider->mRiderAnim, 0);
    cAnimModel_buildSkelMatrices(self->GroundMotion.mRider->mAnimModel);
    cRider_updateCollideInfo(self->GroundMotion.mRider);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_initPath);
#ifdef SKIP_ASM
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];
extern "C" float func_00115B08(void* self);

struct sVec4_00112180
{
    float x, y, z, w;
} __attribute__((aligned(16)));

void cRider_putAt_112180(void* self, sVec4_00112180* a, sVec4_00112180* b, int n, float t) __asm__("cRider_putAt");
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
sVec4_00112180 cPath_getPathPos_112180(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_00112180 vu0Scale_112180(const sVec4_00112180& v, float s)
{
    sVec4_00112180 r;
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
static inline void vu0AddEq_112180(sVec4_00112180& dst, sVec4_00112180 b)
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
 * rider is put at the start position moved along the start direction by func_00115B08 (On Tour
 * getStartGateHorizOffset), facing that direction, with AI anim 5 (cRider_putAt, putAt).
 *
 * Then the path hints (mAIPathHint, mTrackPathHint) are reset (+0x14 = -1), the start point's track and AI paths are
 * stored at +0xAB4/+0xAB8, the distance to the finish (mDistToFinish and +0x4D4) is the track length (+0x38) minus the
 * rider's position on the track path (0 if past the end), the AI path position goes to +0x4C0/+0x4C4, and the aim point
 * +0x4A0 is the AI path point 796 units ahead.
 *
 * @param flag  On Tour resetPosition: also move the rider to the start gate
 */
extern "C" void cRider_initPath(cRider* self, int flag)
{
    float* startPoint = (float*)cPathSys_getStartpoint(gPaths, 1, self->mAIIndex);
    if (flag)
    {
        sVec4_00112180 dir;
        sVec4_00112180 pos;
        dir.x = startPoint[5];
        dir.y = startPoint[6];
        dir.z = startPoint[7];
        dir.w = 0.0f;
        pos.x = startPoint[2];
        pos.y = startPoint[3];
        pos.z = startPoint[4];
        pos.w = 1.0f;
        vu0AddEq_112180(pos, vu0Scale_112180(dir, func_00115B08((char*)self)));
        cRider_putAt_112180(((char*)self), &pos, &dir, 5, 0.0f);
    }
    *(int*)((char*)self->mAIPathHint + 0x14) = -1;
    *(int*)((char*)self->mTrackPathHint + 0x14) = -1;
    *(void**)(((char*)self) + 0xAB4) = ((void**)startPoint)[8];
    *(void**)(((char*)self) + 0xAB8) = ((void**)startPoint)[9];
    sVec4_00112180 up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    up.w = 1.0f;
    float out;
    float res = cPath_intersectPath1(*(void**)(((char*)self) + 0xAB4), ((char*)self) + 0x110, &up, &out, self->mTrackPathHint, 1);
    float trackLen = *(float*)(*(char**)(((char*)self) + 0xAB4) + 0x38);
    if (trackLen < res)
        res = 0.0f;
    self->mDistToFinish = *(float*)(((char*)self) + 0x4D4) = trackLen - res;
    res = cPath_intersectPath1(*(void**)(((char*)self) + 0xAB8), ((char*)self) + 0x110, ((char*)self) + 0x490, ((char*)self) + 0x4C8, self->mAIPathHint, 1);
    *(float*)(((char*)self) + 0x4C0) = *(float*)(((char*)self) + 0x4C4) = res;
    sVec4_00112180 pos2 = cPath_getPathPos_112180(*(void**)(((char*)self) + 0xAB8), 0, res + 796.0f);
    *(sVec4_00112180*)(((char*)self) + 0x4A0) = pos2;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_updateTrackPath);
#ifdef SKIP_ASM
struct sEnt_00112338 { int a; int b; int c; int d; };
struct sComp_00112338 { char pad[0x4DC]; sEnt_00112338 ent[12]; int count; };
struct sVec4_00112338 { float x, y, z, w; } __attribute__((aligned(16)));
struct sVE_00112338 { short delta; short index; int (*fn)(void*, float, float); };

extern sEnt_00112338 D_004A5D60[];
extern "C" void* MEM_copy(void*, void*, int);
extern "C" void cRider_checkTrackPathEvents(cRider* self, float* a, float* b, sVec4_00112338* c);
extern "C" void cRider_checkTrackPathSwitch(cRider* self, int a);
extern "C" float cPath_getLength(void* self);
extern "C" void cRider_flagEventPathEvent(cRider* self, int* arg, int a2, int a3);

/**
 * Tracks the rider along its track path: switches path when needed and raises the path events it enters and leaves.
 *
 * Copies the rider's current event list (count at +0x50C, 16-byte entries from +0x4DC) to D_004A5D60, then
 * cRider_checkTrackPathEvents (On Tour checkTrackPathEvents) gives the distance from the path, the path position and point. While the
 * rider has distance left (+0x4D4 > 0) it calls cRider_checkTrackPathSwitch (On Tour checkTrackPathSwitch) with 0 when more than 500
 * off the path (only every 60th AITick), with 1 within 200 of the path's end, or with 0 when the path's vtable slot 2
 * says to; a path change resets mTrackPathHint and re-measures.
 *
 * Each event in the new list is reported with cRider_flagEventPathEvent (flagEventPathEvent) as current, flagged as previous too
 * when it was in the old list; old events no longer present are reported as previous only.
 */
extern "C" void cRider_updateTrackPath(cRider* self)
{
    sComp_00112338* rider = (sComp_00112338*)self;
    MEM_copy(D_004A5D60, rider->ent, rider->count << 4);
    int n = rider->count;
    sVec4_00112338 pathPos;
    float pathDistXY;
    float pathParam;
    cRider_checkTrackPathEvents(self, &pathDistXY, &pathParam, &pathPos);
    if (0.0f < *(float*)(((char*)self) + 0x4D4))
    {
        void* old = *(void**)(((char*)self) + 0xAB4);
        if (500.0f < pathDistXY && SSXApp->Game->AI->AITick % 60 == 0)
            cRider_checkTrackPathSwitch(self, 0);
        else if (cPath_getLength(*(void**)(((char*)self) + 0xAB4)) - 200.0f < pathParam)
            cRider_checkTrackPathSwitch(self, 1);
        else
        {
            char* trackPath = *(char**)(((char*)self) + 0xAB4);
            sVE_00112338* ve = &(*(sVE_00112338**)(trackPath + 0x34))[2];
            if (ve->fn(trackPath + ve->delta, *(float*)(((char*)self) + 0x4D4), self->mDistToFinish))
                cRider_checkTrackPathSwitch(self, 0);
        }
        if (*(void**)(((char*)self) + 0xAB4) != old)
        {
            *(int*)((char*)self->mTrackPathHint + 0x14) = -1;
            cRider_checkTrackPathEvents(self, &pathDistXY, &pathParam, &pathPos);
        }
    }
    int i;
    for (i = 0; i < rider->count; i++)
    {
        int j;
        for (j = 0; j < n; j++)
        {
            if (rider->ent[i].a == D_004A5D60[j].a && rider->ent[i].b == D_004A5D60[j].b)
            {
                D_004A5D60[j].a = 0;
                break;
            }
        }
        cRider_flagEventPathEvent(self, (int*)&rider->ent[i], j < n, 1);
    }
    for (i = 0; i < n; i++)
    {
        if (D_004A5D60[i].a)
            cRider_flagEventPathEvent(self, (int*)&D_004A5D60[i], 1, 0);
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
extern "C" float cRider_calculateAheadDist(cRider* self, int useNormalDistance)
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
    cQuad128 aimPoint = *(cQuad128*)((char*)self + 0x4A0);
    *(cQuad128*)((char*)self + 0x4B0) = aimPoint;
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
int cRider_isPipeJump(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_updateAIPath);
#ifdef SKIP_ASM
struct sVec4_001125C0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_001125C0 { short delta; short index; int (*fn)(void*, float, float); };

// PORT: the unit defines cRider_isPipeJump as int; here its result is used as a bool (xori 1).
bool cRider_isPipeJump_b(void* self) __asm__("cRider_isPipeJump__FPv");
// PORT: the unit defines cRider_setAimTarget(void* self); callers pass a second arg in $5.
cQuad128 cRider_setAimTarget_2(void* self, int a) __asm__("cRider_setAimTarget");
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
sVec4_001125C0 cPath_getPathPos_1125C0(void* path, int* inRange, float t) __asm__("cPath_getPathPos");
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
 * is the path point cRider_calculateAheadDist (calculateAheadDist) further on, and cRider_setAimTarget (setAimTarget) copies it to the
 * aim target.
 *
 * While the rider still has distance left (+0x4D4 > 0) it calls cRider_checkAIPathSwitch (checkAIPathSwitch) with 0 when more than
 * 500 off the path (only every 60th AITick), with 1 within 200 of the path's end, or with 0 when the path's vtable slot
 * 2 says so. Finally +0x4CC = atan2 of the aim point minus the closest point (x/y).
 */
extern "C" void cRider_updateAIPath(cRider* self)
{
    *(float*)(((char*)self) + 0x4C0) = *(float*)(((char*)self) + 0x4C4);
    int nf = !cRider_isPipeJump_b((char*)self);
    if (nf)
        *(float*)(((char*)self) + 0x4C4) = cPath_intersectPath1(*(void**)(((char*)self) + 0xAB8), ((char*)self) + 0x110, ((char*)self) + 0x490, ((char*)self) + 0x4C8, self->mAIPathHint, nf);
    else
        *(float*)(((char*)self) + 0x4C4) = cPath_intersectPath(*(void**)(((char*)self) + 0xAB8), ((char*)self) + 0x110, ((char*)self) + 0x490, ((char*)self) + 0x4C8, 0);
    float k = cRider_calculateAheadDist(self, nf);
    *(sVec4_001125C0*)(((char*)self) + 0x4A0) = cPath_getPathPos_1125C0(*(void**)(((char*)self) + 0xAB8), 0, *(float*)(((char*)self) + 0x4C4) + k);
    cRider_setAimTarget_2(((char*)self), nf);
    if (0.0f < *(float*)(((char*)self) + 0x4D4))
    {
        if (500.0f < *(float*)(((char*)self) + 0x4C8) && SSXApp->Game->AI->AITick % 60 == 0)
            cRider_checkAIPathSwitch(self, 0);
        else if (cPath_getLength(*(void**)(((char*)self) + 0xAB8)) - 200.0f < *(float*)(((char*)self) + 0x4C4))
            cRider_checkAIPathSwitch(self, 1);
        else
        {
            char* aiPath = *(char**)(((char*)self) + 0xAB8);
            sVE_001125C0* ve = &(*(sVE_001125C0**)(aiPath + 0x34))[2];
            if (ve->fn(aiPath + ve->delta, *(float*)(((char*)self) + 0x4C0), *(float*)(((char*)self) + 0x4C4)))
                cRider_checkAIPathSwitch(self, 0);
        }
    }
    float dx = *(float*)(((char*)self) + 0x4A0) - *(float*)(((char*)self) + 0x490);
    float dy = *(float*)(((char*)self) + 0x4A4) - *(float*)(((char*)self) + 0x494);
    *(float*)(((char*)self) + 0x4CC) = atan2_001125C0(dy, dx);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_checkTrackPathSwitch);
#ifdef SKIP_ASM
struct sVec4_1127F0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern char gPaths[];
extern "C" int cPathSys_getNearTrackPaths(void* tree, void* pos, void** out, int maxOut);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
sVec4_1127F0 cPath_getPathPos_1127F0(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_1127F0(const sVec4_1127F0& v)
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
static inline sVec4_1127F0 vu0Scale_1127F0(const sVec4_1127F0& v, float s)
{
    sVec4_1127F0 r;
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
static inline sVec4_1127F0 vu0Add_1127F0(const sVec4_1127F0& a, const sVec4_1127F0& b)
{
    sVec4_1127F0 r;
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
static inline sVec4_1127F0 vu0Sub_1127F0(const sVec4_1127F0& a, const sVec4_1127F0& b)
{
    sVec4_1127F0 r;
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
static inline float vu0Dot_1127F0(const sVec4_1127F0& a, const sVec4_1127F0& b)
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
 * @param vself  the rider (this)
 * @param skipCur  On Tour mustSwitch: never keep the current track path
 */
extern "C" void cRider_checkTrackPathSwitch(cRider* vself, int skipCur)
{
    void* paths[3];
    sVec4_1127F0 out;
    sVec4_1127F0 pos;
    float len = vu0Length_1127F0(*(sVec4_1127F0*)&vself->mVelocity);
    void* best = *(void**)(((char*)vself) + 0xAB4);
    float bestD = 9.999999933815813e+36f;
    if (len == 0.0f) {
        pos = *(sVec4_1127F0*)&vself->mPosition;
    } else {
        float aheadScale = cRider_calculateAheadDist(vself, 1) / len;
        pos = vu0Add_1127F0(*(sVec4_1127F0*)&vself->mPosition, vu0Scale_1127F0(*(sVec4_1127F0*)&vself->mVelocity, aheadScale));
    }
    int n = cPathSys_getNearTrackPaths(gPaths, &pos, paths, 3);
    for (int i = 0; i < n; i++) {
        if (skipCur && paths[i] == *(void**)(((char*)vself) + 0xAB4)) {
            continue;
        }
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], &pos, &out, &tOut, 1);
        if (paths[i] == *(void**)(((char*)vself) + 0xAB4)) {
            if (1592.0f < __builtin_fabsf(*(float*)((char*)paths[i] + 0x38) - vself->mDistToFinish - nearParam)) {
                *(int*)((char*)vself->mTrackPathHint + 0x14) = -1;
            }
        }
        nearParam += 796.0f;
        sVec4_1127F0 aheadDelta = vu0Sub_1127F0(pos, cPath_getPathPos_1127F0(paths[i], 0, nearParam));
        sVec4_1127F0 nearDelta = vu0Sub_1127F0(*(sVec4_1127F0*)&vself->mPosition, out);
        float dist = vu0Dot_1127F0(nearDelta, nearDelta) + vu0Dot_1127F0(aheadDelta, aheadDelta);
        if (dist < bestD) {
            best = paths[i];
            bestD = dist;
        }
    }
    *(void**)(((char*)vself) + 0xAB4) = best;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_checkAIPathSwitch);
#ifdef SKIP_ASM
struct sVec4_112A50 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern char gPaths[];
extern "C" int cPathSys_getNearAIPaths(void* tree, void* pos, void** out, int maxOut, int a4);
extern "C" float cPath_getLength(void* self);
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
sVec4_112A50 cPath_getPathPos_112A50(void* path, int* inRange, float t) __asm__("cPath_getPathPos");

// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Length_112A50(const sVec4_112A50& v)
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
static inline sVec4_112A50 vu0Scale_112A50(const sVec4_112A50& v, float s)
{
    sVec4_112A50 r;
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
static inline sVec4_112A50 vu0Add_112A50(const sVec4_112A50& a, const sVec4_112A50& b)
{
    sVec4_112A50 r;
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
static inline sVec4_112A50 vu0Sub_112A50(const sVec4_112A50& a, const sVec4_112A50& b)
{
    sVec4_112A50 r;
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
static inline float vu0Dot_112A50(const sVec4_112A50& a, const sVec4_112A50& b)
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

struct sVEi_112A50 { short delta; short index; int (*fn)(void*); };
struct sVEf_112A50 { short delta; short index; float (*fn)(void*, void*); };

/**
 * Picks a better AI path near the rider and moves the rider onto it.
 *
 * The probe point is the rider's position moved 796 along its velocity (or the position when still). Up to 6 nearby AI
 * paths come from cPathSys_getNearAIPaths (On Tour cPathSys::getNearAIPaths); a path is skipped when it is the current one and @p
 * keep is set, or when the rider is within 200 of its end. A rider whose vtable slot 9 (isComputer) returns 1 picks the
 * path with the highest slot-20 score (cComputer_scorePath; cPlayer_scorePath returns 0), others the one whose near and
 * look-ahead points are closest. A winner equal to the current path counts as no change.
 *
 * On a change: the AI path (+0xAB8) is replaced, mAIPathHint reset, and the path position (+0x4C4 and +0x4C0), closest
 * point (+0x490), distance off the path (+0x4C8) and aim point (+0x4A0, 796 ahead) are recomputed.
 *
 * @param vself  the rider (this)
 * @param keep  On Tour mustSwitch: do not consider the current AI path
 */
extern "C" void cRider_checkAIPathSwitch(cRider* vself, int keep)
{
    void* paths[6];
    sVec4_112A50 out;
    sVec4_112A50 pos;
    float len = vu0Length_112A50(*(sVec4_112A50*)&vself->mVelocity);
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    float bestW = -1.0f;
    if (len == 0.0f) {
        pos = *(sVec4_112A50*)&vself->mPosition;
    } else {
        pos = vu0Add_112A50(*(sVec4_112A50*)&vself->mPosition, vu0Scale_112A50(*(sVec4_112A50*)&vself->mVelocity, 796.0f / len));
    }
    int n = cPathSys_getNearAIPaths(gPaths, &pos, paths, 6, 0);
    for (int i = 0; i < n; i++) {
        if (paths[i] == *(void**)(((char*)vself) + 0xAB8) && keep) {
            continue;
        }
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], &pos, &out, &tOut, 1);
        if (cPath_getLength(paths[i]) - 200.0f < nearParam) {
            continue;
        }
        sVec4_112A50 aheadDelta = vu0Sub_112A50(pos, cPath_getPathPos_112A50(paths[i], 0, nearParam + 796.0f));
        sVec4_112A50 nearDelta = vu0Sub_112A50(*(sVec4_112A50*)&vself->mPosition, out);
        float dist = vu0Dot_112A50(nearDelta, nearDelta) + vu0Dot_112A50(aheadDelta, aheadDelta);
        char* being = ((char*)vself) + 0x6C0;
        sVEi_112A50* ve = &(*(sVEi_112A50**)being)[9];
        if (ve->fn(being + ve->delta) == 1) {
            sVEf_112A50* vf = &((sVEf_112A50*)vself->mVtbl)[20];
            dist = vf->fn(((char*)vself) + vf->delta, paths[i]);
            if (bestW < dist) {
                best = paths[i];
                bestW = dist;
                if (best == *(void**)(((char*)vself) + 0xAB8)) {
                    best = 0;
                }
            }
        } else {
            if (dist < bestD) {
                best = paths[i];
                bestD = dist;
                if (best == *(void**)(((char*)vself) + 0xAB8)) {
                    best = 0;
                }
            }
        }
    }
    if (best != 0) {
        *(void**)(((char*)vself) + 0xAB8) = best;
        *(int*)((char*)vself->mAIPathHint + 0x14) = -1;
        float pathParam = cPath_intersectPath1(*(void**)(((char*)vself) + 0xAB8), ((char*)vself) + 0x110, ((char*)vself) + 0x490, ((char*)vself) + 0x4C8, vself->mAIPathHint, 1);
        *(float*)(((char*)vself) + 0x4C4) = pathParam;
        *(sVec4_112A50*)(((char*)vself) + 0x4A0) = cPath_getPathPos_112A50(*(void**)(((char*)vself) + 0xAB8), 0, pathParam + 796.0f);
        *(float*)(((char*)vself) + 0x4C0) = *(float*)(((char*)vself) + 0x4C4);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_switchToResetPath);
#ifdef SKIP_ASM
struct sVec4_112D58 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_112D58 { short delta; short index; int (*fn)(void*); };

extern char gPaths[];
int cPathSys_getNearResetPaths_112D58(void* self, void* a1, void** out, int maxOut) __asm__("cPathSys_getNearResetPaths__FPv");
extern "C" float cPath_intersectPath(void* path, void* a1, void* a2, void* a3, int n);
extern "C" float cPath_intersectPath1(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float cPath_getLength(void* self);
sVec4_112D58 cAIPath_getResetPos_112D58(void* path, float* t) __asm__("cAIPath_getResetPos");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_112D58 vu0Sub_112D58(const sVec4_112D58& a, const sVec4_112D58& b)
{
    sVec4_112D58 r;
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
static inline float vu0Dot_112D58(const sVec4_112D58& a, const sVec4_112D58& b)
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
extern "C" void cRider_switchToResetPath(cRider* self)
{
    void* paths[6];
    sVec4_112D58 pos;
    int n = cPathSys_getNearResetPaths_112D58(gPaths, ((char*)self) + 0x110, paths, 6);
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    int i;
    for (i = 0; i < n; i++)
    {
        float tOut;
        float nearParam = cPath_intersectPath(paths[i], ((char*)self) + 0x110, &pos, &tOut, 1);
        float len = cPath_getLength(paths[i]);
        char* being = ((char*)self) + 0x6C0;
        sVE_112D58* ve = &(*(sVE_112D58**)being)[8];
        if (ve->fn(being + ve->delta) == 0 && len - 200.0f < nearParam)
            continue;
        float aheadParam = nearParam + 796.0f;
        sVec4_112D58 aheadDelta = vu0Sub_112D58(*(sVec4_112D58*)&self->mPosition, cAIPath_getResetPos_112D58(paths[i], &aheadParam));
        sVec4_112D58 nearDelta = vu0Sub_112D58(*(sVec4_112D58*)&self->mPosition, pos);
        float dist = vu0Dot_112D58(nearDelta, nearDelta) + vu0Dot_112D58(aheadDelta, aheadDelta);
        if (dist < bestD)
        {
            best = paths[i];
            bestD = dist;
            if (best == *(void**)(((char*)self) + 0xAB8))
                best = 0;
        }
    }
    if (*(void**)(((char*)self) + 0xAB8) == 0)
        return;
    if (best)
        *(void**)(((char*)self) + 0xAB8) = best;
    *(int*)((char*)self->mAIPathHint + 0x14) = -1;
    *(float*)(((char*)self) + 0x4C4) = cPath_intersectPath1(*(void**)(((char*)self) + 0xAB8), ((char*)self) + 0x110, ((char*)self) + 0x490, ((char*)self) + 0x4C8, self->mAIPathHint, 1);
    *(sVec4_112D58*)(((char*)self) + 0x490) = cAIPath_getResetPos_112D58(*(void**)(((char*)self) + 0xAB8), (float*)(((char*)self) + 0x4C4));
    float aimParam = *(float*)(((char*)self) + 0x4C4) + 796.0f;
    *(sVec4_112D58*)(((char*)self) + 0x4A0) = cAIPath_getResetPos_112D58(*(void**)(((char*)self) + 0xAB8), &aimParam);
    *(float*)(((char*)self) + 0x4C0) = *(float*)(((char*)self) + 0x4C4);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_checkTrackPathEvents);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: an extra `dist` read and the `cp` pointer (cp->DistanceLeft re-read) keep jump.c under its duplicate-insn limit; plain form 97.81%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern float cPath_intersectPath1_2FB0(void* track, void* pos, void* iPos, float* iDistXY, int* pi, int is2D) __asm__("cPath_intersectPath1");
extern char gPaths[];

// cTrackPath's vtable entry (g++ 2.95 layout: this-delta, index, fn).
struct sVEntry_112FB0 { short delta; short index; int (*fn)(void*, float, float, void*, int); };
// On Tour tPathCheckpoint.
struct tPathCheckpoint_112FB0 { int TimeBonusSec; float DistanceLeft; };
struct sPathSys_112FB0 { char pad0[0x18]; tPathCheckpoint_112FB0 Checkpoints[6]; };

/**
 * Measures the rider's progress on its track path, fires the checkpoints it passed and gathers the events in range.
 *
 * Intersects the rider with the track path (+0xAB4, mTrackPathHint) and sets mDistToFinish = path length (+0x38) - path
 * position. When the rider moved forward (below the previous distance at +0x4D4), every one of gPaths' 6 checkpoints
 * between the old and new distance (the list ends at a zero entry) goes to cRider_flagEventPathCheckpoint (flagEventPathCheckpoint), the
 * path's vtable slot 1 collects up to 12 events in the covered range into +0x4DC (count in mOverlapCount), and +0x4D4
 * advances. Otherwise the same query runs on the empty range at the old distance.
 *
 * @param vself  the rider (this)
 * @param pathDistXY  receives the rider's distance from the path
 * @param pathParam  receives the rider's position along the path
 * @param pathPos  receives the nearest point on the path
 */
// cRider::checkTrackPathEvents(float& pathDistXY, float& pathParam, t4Vector& pathPos) (On Tour name)
extern "C" void cRider_checkTrackPathEvents(cRider* vself, float* pathDistXY, float* pathParam, sVec4_00112338* pathPos)
{
    cRider* self = (cRider*)vself;
    float trackParam = cPath_intersectPath1_2FB0(*(void**)((char*)self + 0xAB4), &self->mPosition, pathPos, pathDistXY, self->mTrackPathHint, 1);
    *pathParam = trackParam;
    char* track = *(char**)((char*)self + 0xAB4);
    float cur = *(float*)((char*)self + 0x4D4);
    float distLeft = *(float*)(track + 0x38) - trackParam;
    self->mDistToFinish = distLeft;
    if (distLeft < cur)
    {
        sPathSys_112FB0* tbl = (sPathSys_112FB0*)gPaths;
        for (int i = 0; i < 6; i++)
        {
            tPathCheckpoint_112FB0* cp = &tbl->Checkpoints[i];
            float dist = tbl->Checkpoints[i].DistanceLeft;
            if (dist == 0.0f || dist < self->mDistToFinish)
                break;
            if (cp->DistanceLeft < *(float*)((char*)self + 0x4D4))
                cRider_flagEventPathCheckpoint(self, (int*)cp);
        }
        char* curTrack = *(char**)((char*)self + 0xAB4);
        sVEntry_112FB0* vt = *(sVEntry_112FB0**)(curTrack + 0x34);
        self->mOverlapCount = vt[1].fn(curTrack + vt[1].delta, *(float*)((char*)self + 0x4D4), self->mDistToFinish, (char*)self + 0x4DC, 0xC);
        *(float*)((char*)self + 0x4D4) = self->mDistToFinish;
    }
    else
    {
        sVEntry_112FB0* vt = *(sVEntry_112FB0**)(track + 0x34);
        self->mOverlapCount = vt[1].fn(track + vt[1].delta, cur, cur, (char*)self + 0x4DC, 0xC);
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
float cRider_getDistanceLeft(void* self)
{
    return *(float*)((char*)self + 0x4D0);
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
float cRider_getTotalPathDistance(void* self)
{
    return *(float*)((char*)self + 0x4D8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAirPredictor_cAirPredictor);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_reset(char* self);

/**
 * Constructs an air predictor: marks it unused and resets its flight state.
 *
 * Stores -1 at +0x30, then cAirPredictor_reset.
 *
 * @return this
 */
extern "C" void* cAirPredictor_cAirPredictor(void* self)
{
    *(unsigned int*)((char*)self + 0x30) = 0xFFFFFFFF;
    cAirPredictor_reset((char*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cAirPredictor__cAirPredictor);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys an air predictor; it owns nothing, so this only frees the object when asked.
 *
 * @param flags  g++ in-charge flags: bit 0 set means delete the object (operator_delete)
 */
extern "C" void cAirPredictor__cAirPredictor(void* self, int flags)
{
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

