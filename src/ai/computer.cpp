#include "common.h"
#include "ai/riderhandler.h"
#include "sound/ssxaudio.h"
#include "ai/computer.h"
#include "be/beintnewrace.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

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
extern "C" int func_00147CB8(void* iface, int flag);
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
 * difficulty setting (func_00147CB8). Game events 4 and 5 force mPathDifficulty to 0 and 2. The Super AI race
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
    int r = vt[7].fn(obj + vt[7].delta);
    int level = ValidOr_UR(r, -1, 1);
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
    cComputer_setIndividualRiderDifficulty_UR(self, level, func_00147CB8(cBE_getInterface_Fv(cBE_getBE(), 1), cBENewRaceInterface_newRace.cGameMode == 0));
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
INCLUDE_ASM("ai/computer", func_0010C9A8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00144C98(void* iface);
extern "C" void func_00148AA8(void* iface, int a1, int a2);

struct sVEntry10C9A8 { short delta; short index; int (*fn)(void*); };

static inline int riderV7_10C9A8(char* rider)
{
    char* obj = rider + 0x6C0;
    sVEntry10C9A8* vt = *(sVEntry10C9A8**)obj;
    return vt[7].fn(obj + vt[7].delta);
}

extern "C" void func_0010C9A8(cComputer* self)
{
    void* s = cBE_getInterface_Fv(cBE_getBE(), 3);
    void* g = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (func_00144C98(g) == 2)
    {
        func_00148AA8(s, riderV7_10C9A8((char*)self->mRider), 7);
    }
    else if (func_00144C98(g) == 1)
    {
        func_00148AA8(s, riderV7_10C9A8((char*)self->mRider), 4);
    }
    else
    {
        func_00148AA8(s, riderV7_10C9A8((char*)self->mRider), 1);
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
    int r = -1;
    int skip = self->mToRail != 0 && airTime < 1.0f;
    if (!skip && airTime > 0.75f && airTime != 100.0f)
    {
        if (cComputer_shouldRiderUber(self)) r = cComputer_chooseRandomUberGrab(self, airTime);
        if (r == -1)
        {
            if (cComputer_shouldRiderTweak(self)) r = cComputer_chooseRandomTweakGrab(self, airTime);
            if (r == -1)
            {
                if (cComputer_shouldRiderGrab(self)) r = cComputer_chooseRandomGrab(self, airTime);
            }
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_chooseRandomGrab);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned short func_001500D8(void* self, int a1, int a2);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float func_00312820(char* self, int a1, int frame);

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
 * Stores @p time in mGrabStartTime and reads the grab animation's marker times (func_00312820 keys 2 and 1,
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
    int anim = func_001500D8(iface, riderV7_10CBE0(((char*)self->mRider)), idx);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->mRider));
    float a = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 2) / spd;
    float b = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 1) / spd;
    if (time < a) return -1;
    self->mGrabEndTime = time - a;
    self->mHoldToLandTime = a - b;
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
extern "C" unsigned short func_001500D8(void* self, int a1, int a2);
extern "C" int func_00150138(void* self, int a1, int a2);
extern "C" int func_00150198(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float func_00312820(char* self, int a1, int frame);

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
        int anim = func_00150138(iface, riderV7_10CD20(((char*)self->mRider)), i);
        if (anim == 0x1B6)
            continue;
        int alt = (*(int*)(((char*)self->mRider) + 0xB2C) & 1) ? func_00150198(iface, riderV7_10CD20(((char*)self->mRider)), i, *(int*)(((char*)self->mRider) + 0x2F4) >= 5) : 0x1B6;
        if (0.0f < *(float*)(((char*)self->mRider) + 0x2F0) && alt != 0x1B6)
            continue;
        int anim2 = func_001500D8(iface, riderV7_10CD20(((char*)self->mRider)), i);
        float a = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim2, 1) / spd;
        float b = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 1) / spd;
        float c = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 2) / spd;
        float sum = c + a;
        if (sum < time && self->TweakHistoryCounter[i] < best)
        {
            self->mIsTweak = 1;
            found = i;
            self->mHoldToLandTime = c - b;
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
extern "C" unsigned short func_001500D8(void* self, int a1, int a2);
extern "C" int func_00150198(void* self, int a1, int a2, int a3);
extern "C" float cRider_getGrabAnimSpeedScale(void* self);
extern "C" float func_00312820(char* self, int a1, int frame);

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
 * variant (func_00150198, with a flag for mUberLevel >= 5) and slots without one (0x1B6) are skipped; the fit
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
    if (*(float*)(((char*)self->mRider) + 0x2F0) <= 0.0f)
        return -1;
    self->mGrabStartTime = time;
    int found = -1;
    int best = 10000;
    int i = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float spd = cRider_getGrabAnimSpeedScale(((char*)self->mRider));
    for (; i < 15; i++)
    {
        int anim = func_00150198(iface, riderV7_10CF68(((char*)self->mRider)), i, *(int*)(((char*)self->mRider) + 0x2F4) >= 5);
        if (anim == 0x1B6)
            continue;
        int anim2 = func_001500D8(iface, riderV7_10CF68(((char*)self->mRider)), i);
        float a = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim2, 1) / spd;
        float b = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 1) / spd;
        float c = func_00312820(*(char**)(((char*)self->mRider) + 0x784), anim, 2) / spd;
        float sum = c + a;
        if (sum < time && self->UberHistoryCounter[i] < best)
        {
            self->mIsUber = 1;
            found = i;
            self->mHoldToLandTime = c - b;
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
INCLUDE_ASM("ai/computer", func_0010D1A0);
#ifdef SKIP_ASM
int func_0011FE98(void* self);
int func_0011FEE8(void* self);
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

extern "C" int func_0010D1A0(sComputer_0010D1A0* self)
{
    float ang = atan2_0010D1A0(self->rider->getVel().y, self->rider->getVel().x);
    float d = wrap_0010D1A0(self->rider->heading - ang);
    if (self->dir < 0) {
        if (__builtin_fabsf(d) > 1.047197699546814f) {
            self->wrongTime++;
        } else {
            self->wrongTime = 0;
        }
    } else {
        self->wrongTime = 0;
    }
    int wrong = self->wrongTime >= 0x3D;
    if (func_0011FE98(self->rider) == 2) {
        wrong = 0;
        self->stuckTime++;
    } else if (func_0011FE98(self->rider) == 4 && self->wrongTime > 0) {
        return 1;
    } else if (func_0011FE98(self->rider) == 1 || self->rider->speed < 1000.0f) {
        self->stuckTime = 0;
        wrong = 0;
        if (func_0011FE98(self->rider) == 1 && *(float*)(self->rider->board + 0xA0) > 30.0f) {
            return 1;
        }
    } else {
        self->stuckTime++;
    }
    int stuck = wrong || self->stuckTime >= 0xDF;
    if (func_0011FEE8(self->rider) != 8) {
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
extern "C" int func_0010FC30(sAi0010FC30* self, int team);
extern "C" float func_00112588(void* self, int arg1);
extern "C" float func_0026A428(void* path, void* a1, void* a2, void* a3, int n);

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
    int b2 = (flags >> 2) & 1;
    int b1 = (flags >> 1) & 1;
    int b3 = (flags >> 3) & 1;
    float base = cComputer_calculateDifficultyScore(self, b3, b2, b1);
    if (base <= 0.0f) {
        return 0.0f;
    }
    if (self->mShouldTakeShortcuts == 0 && solo != 0) {
        return 0.0f;
    }
    float speed = vu0Length_10D410(*(sVec4_10D410*)((char*)self->mRider + 0x1E0)) * 0.035999998450279236f;
    if (lvl == 0) {
        if ((float)speedLimit_10D410(kind) < speed) {
            return 0.0f;
        }
    } else if (lvl == 3) {
        if (speed < (float)speedLimit_10D410(kind)) {
            return 0.0f;
        }
    }
    char* r = (char*)self->mRider;
    float len = vu0Length_10D410(*(sVec4_10D410*)(r + 0x1E0));
    float total = 22.0f;
    float s = func_00112588(r, 1) / len;
    sVec4_10D410 pos = vu0Add_10D410(*(sVec4_10D410*)((char*)self->mRider + 0x110), vu0Scale_10D410(*(sVec4_10D410*)((char*)self->mRider + 0x1E0), s));
    sVec4_10D410 out;
    float t;
    func_0026A428(tgt, &pos, &out, &t, 1);
    float d = t;
    float v = 100.0f - d * d * 1.5999999959603883e-05f;
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (5000.0f < d) {
        return -1.0f;
    }
    sc[0] = v * 0.22727273404598236f;
    char* r2 = (char*)self->mRider;
    if (*(int*)(r2 + 0xF0) != 0 && *(char**)(((sAi_10D410*)ai)->riders[*(int*)(r2 + 0xF8)] + 0xAB8) == tgt) {
        return 100.0f;
    }
    float zero = 0.0f;
    sc[1] = zero;
    float w = 1.0f / total;
    sc[2] = (float)lvl * (w * 0.3333333432674408f);
    // PORT: the path pointer is passed as func_0010FC30's int team
    int n = func_0010FC30((sAi0010FC30*)ai, (int)tgt);
    n -= tgt == *(char**)((char*)self->mRider + 0xAB8);
    float c;
    if (n <= 0) {
        c = 100.0f;
    } else if (n == 1) {
        c = 25.0f;
    } else {
        c = zero;
    }
    w = 5.0f / total;
    sc[3] = c * w;
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
    short s = self->mPathDifficulty;
    if (s == 0)
    {
        if (!IsEasy)
            return 0.0f;
    }
    else if (s == 1)
    {
        if (!IsMedium)
            return 0.0f;
    }
    else if (s == 2)
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
    sComputer_0010D8F8* p = (sComputer_0010D8F8*)self->mRider;
    for (int i = 0; i < 6; i++, p++)
    {
        if (p->a != 0 && p->b != 0)
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_isFarBehind);
#ifdef SKIP_ASM
extern "C" int func_0010DEB0(void* self);
float func_00113128(void* self);

/**
 * Tells whether this computer rider trails its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandBehind (a negative
 * distance set by the difficulty code). Without an opponent (func_0010DEB0, the rider at myOpponentIndex())
 * it is not behind.
 *
 * @return 1 when the difference is below mRubberBandBehind, otherwise 0
 */
extern "C" int cComputer_isFarBehind(cComputer* self)
{
    // PORT: func_0010DEB0 returns a pointer as int (unit declaration).
    int p = func_0010DEB0(self);
    if (p == 0)
        return 0;
    float d = func_00113128((void*)p) - (self->mRider)->mDistToFinish;
    if (d < *(float*)((char*)(self->mRider) + 0xDC))
        return 1;
    return 0;
}
#endif
//100%
INCLUDE_ASM("ai/computer", cComputer_isFarAhead);
#ifdef SKIP_ASM
extern "C" int func_0010DEB0(void* self);
float func_00113128(void* self);

/**
 * Tells whether this computer rider leads its opponent by more than its rubber-band threshold.
 *
 * Compares the opponent's mDistToFinish minus this rider's with the rider's mRubberBandAhead. Without an
 * opponent (func_0010DEB0, the rider at myOpponentIndex()) it is not ahead.
 *
 * @return 1 when the difference is above mRubberBandAhead, otherwise 0
 */
extern "C" int cComputer_isFarAhead(cComputer* self)
{
    // PORT: func_0010DEB0 returns a pointer as int (unit declaration).
    int p = func_0010DEB0(self);
    if (p == 0)
        return 0;
    float d = func_00113128((void*)p) - (self->mRider)->mDistToFinish;
    if (*(float*)((char*)self->mRider + 0xE0) < d)
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
    void* p = self->mRider;
    int r = 0;
    if (*(int*)((char*)p + 0xf0) != 0) {
        r = *(int*)((char*)p + 0xf8) == oppID;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cComputer_checkForBlock);
#ifdef SKIP_ASM
struct sVE_0010DA10 { short delta; short index; int (*fn)(void*); };
struct sEnt_0010DA10 { int vis; int val; float dist; int c; int d; int e; int f; int g; int h; };
struct sRider_0010DA10 { sEnt_0010DA10 ent[6]; };
struct sAI_0010DA10 { char pad[0x28]; char* riders[20]; int count; };
extern "C" int func_001298C8();
extern "C" int func_00311AE8(void*, int);
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
    float t = self->mBlockTimer;
    if (t < 0.0f)
    {
        self->mBlockTimer = 0.0f;
        return 0;
    }
    if (0.0f < t)
    {
        self->mBlockTimer = t - 0.01666666753590107f;
        return self->mShouldIBlock;
    }
    if (func_001298C8() % 6 != 0)
        return 0;
    sAI_0010DA10* ai = (sAI_0010DA10*)SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->count; i++)
    {
        char* o = (char*)self->mRider + 0x6C0;
        sVE_0010DA10* ve = &(*(sVE_0010DA10**)o)[7];
        if (i == ve->fn(o + ve->delta))
            continue;
        char* o2 = ai->riders[i] + 0x6C0;
        sVE_0010DA10* ve2 = &(*(sVE_0010DA10**)o2)[8];
        if (ve2->fn(o2 + ve2->delta) == 0)
            continue;
        if (!(((sRider_0010DA10*)self->mRider)->ent[i].dist < 200.0f))
            continue;
        if (func_00311AE8(*(void**)(ai->riders[i] + 0x784), 1) != 13)
            continue;
        unsigned int r = AIrand() % 100;
        int* pf = (int*)(((char*)self) + 0xF34);
        *pf = (float)r < 25.0f;
        self->mBlockTimer = 2.0f;
        return self->mShouldIBlock;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010DBF0);
#ifdef SKIP_ASM
extern "C" int func_00155B50(void* self, int a, int b);
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
struct sAI_0010DBF0 { char pad[0x28]; char* riders[20]; int count; };

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
    return func_00155B50(agg, a, b) >= 3;
}

// Taunt check: finds a close rider beside us (30..150 degrees off our heading) that we're aggressive towards,
// at most every 120 frames; *side tells which side it is on.
extern "C" int func_0010DBF0(char* self, int* side)
{
    if (func_001298C8() % 6 != 0)
        return 0;
    sAI_0010DBF0* ai = (sAI_0010DBF0*)SSXApp->Game->AI;
    int i;
    for (i = 0; i < ai->count; i++)
    {
        float pi = 3.1415927410125732f;
        float lo = 0.5235988497734070f;
        float hi = 2.6179940700531006f;
        if (i == id_0010DBF0(self))
            continue;
        sRider_0010DBF0* r = *(sRider_0010DBF0**)(self + 0x18);
        if (!(r->ent[i].dist < 200.0f))
            continue;
        sVec_0010DBF0 a = r->vel;
        sVec_0010DBF0 b = r->vel;
        float ang = atan2_0010DBF0(a.y, b.x, pi);
        float d = wrap_0010DBF0(ang - (*(sRider_0010DBF0**)(self + 0x18))->ent[i].angle);
        float ad = __builtin_fabsf(d);
        if (!(lo < ad))
            continue;
        if (!(ad < hi))
            continue;
        void* agg = cBEAggressionInterface_getThis();
        if (!isAggr_0010DBF0(agg, id_0010DBF0(self), i))
            continue;
        if (!((*(sRider_0010DBF0**)(self + 0x18))->lastTaunt + 120 < func_001298C8()))
            continue;
        *side = 0.0f < d;
        (*(sRider_0010DBF0**)(self + 0x18))->lastTaunt = func_001298C8();
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010DEB0);
#ifdef SKIP_ASM

extern "C" int func_0010DEB0(void* self)
{
    int i = cComputer_myOpponentIndex((cComputer*)self);
    if (i < 0)
        return 0;
    return *(int*)((char*)SSXApp->Game->AI + (i << 2) + 0x28);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010DEF0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
float func_00113128(void* self);
extern signed char D_00535C11[];

extern "C" float func_0010DEF0(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    // PORT: func_0010DEB0 returns a pointer as int (unit declaration).
    int p = func_0010DEB0(self);
    if (p == 0) return 1.0f;
    char* r = *(char**)((char*)self + 0x18);
    if (*(int*)(r + 0xE4) == 0) return 1.0f;
    if (D_00535C11[0] == 2) return 1.0f;
    if (*(float*)(r + 0x4D0) < 50000.0f) return 1.0f;
    float m;
    float d = func_00113128((void*)p) - *(float*)(*(char**)((char*)self + 0x18) + 0x4D0);
    char* r2 = *(char**)((char*)self + 0x18);
    float k = *(float*)(r2 + 0xDC);
    if (d < k && (*(int*)(r2 + 0xE4) == 2 || *(int*)(r2 + 0xE4) == 3))
    {
        m = ((d - k) / k + 1.0f) * 1.100000023841858f;
    }
    else
    {
        char* r3 = *(char**)((char*)self + 0x18);
        float k2 = *(float*)(r3 + 0xE0);
        if (k2 < d && (*(int*)(r3 + 0xE4) == 1 || *(int*)(r3 + 0xE4) == 3))
        {
            m = 1.0f / ((d - k2) / k2 + 1.0f);
            m *= 0.9090908765792847f;
        }
        else
            m = 1.0f;
    }
    return m;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E028);
#ifdef SKIP_ASM
extern "C" int func_001298C8();

extern "C" void func_0010E028(void* self, int mode, float t)
{
    int v;
    if (mode == 0)
    {
        v = -1;
        *(int*)((char*)self + 0x358) = 0;
    }
    else
    {
        int frames = (int)(t * 60.0f);
        v = func_001298C8() + frames;
        *(int*)((char*)self + 0x358) = mode;
    }
    *(int*)((char*)self + 0x354) = v;
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRider_flagEventBoostPowerUp);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* func_0028B180();
extern "C" void func_00149690(void* iface, int rider, int b);
extern "C" void func_0029AB08(void* mgr, void* rider, float cur, float amt);
extern "C" void func_0010E028(void* self, int mode, float t);

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
 * (func_00149690, result unused) and is dropped unless @p mask shares a bit with mBoostSourceMask. It
 * notifies the audio system (func_0029AB08), then clamps. A gain that fills the meter with uber enabled
 * (mUberFlags) shows HUD message 5 when mUberTimer is 0, raises an mUberLevel of 0 or less by one and, at
 * levels 1-9, makes mUberTimer at least 20; a loss below level 10 stops mUberTimer.
 *
 * @param mask  boost source flags (On Tour powerSourceFlags), tested against mBoostSourceMask for a gain
 * @param amt  amount to add, negative to drain (On Tour powerUp)
 */
extern "C" void cRider_flagEventBoostPowerUp(void* self, int mask, float amt)
{
    if (*(int*)((char*)self + 0x304) == 3)
        return;
    float zero = 0.0f;
    if (amt == zero)
        return;
    if (amt > zero)
    {
        func_00149690(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)((char*)self + 0x86C), *(int*)((char*)self + 0xB34));
        if ((mask & *(int*)((char*)self + 0xB28)) == 0)
            return;
    }
    func_0029AB08(func_0028B180(), self, *(float*)((char*)self + 0x2F8), amt);
    *(float*)((char*)self + 0x2F8) = clamp_10E098(*(float*)((char*)self + 0x2F8) + amt, zero, 1.0f);
    if (*(float*)((char*)self + 0x2F8) == 1.0f && amt > zero && *(int*)((char*)self + 0xB2C) != 0)
    {
        if (*(float*)((char*)self + 0x2F0) == zero)
            func_0010E028(self, 5, zero);
        if (*(int*)((char*)self + 0x2F4) <= 0)
            *(int*)((char*)self + 0x2F4) += 1;
        if (*(int*)((char*)self + 0x2F4) >= 1 && *(int*)((char*)self + 0x2F4) <= 9)
        {
            // PORT: g++ `>?` (max) operator.
            *(float*)((char*)self + 0x2F0) = *(float*)((char*)self + 0x2F0) >? 20.0f;
        }
    }
    else if (amt < 0.0f && *(int*)((char*)self + 0x2F4) < 10)
    {
        *(float*)((char*)self + 0x2F0) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E228);
#ifdef SKIP_ASM
struct sVEntry0010E228 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298488(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E228(void* a, void* b)
{
    func_00298488(func_0028B180(), a, b);
    func_002A0A30(func_0028B180(), b, a, 1, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E228* eb = &(*(sVEntry0010E228**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E228* ea = &(*(sVEntry0010E228**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E2E8);
#ifdef SKIP_ASM
struct sVEntry0010E2E8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void func_00298488(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E2E8(void* a, void* b)
{
    func_00298488(func_0028B180(), a, b);
    func_002A0A30(func_0028B180(), b, a, 1, 1);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E2E8* eb = &(*(sVEntry0010E2E8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E2E8* ea = &(*(sVEntry0010E2E8**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E3A8);
#ifdef SKIP_ASM
struct sVEntry0010E3A8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);

extern "C" void func_0010E3A8(void* a, void* b)
{
    cSSXAudio_Punch(func_0028B180(), b, a);
    func_002A0A30(func_0028B180(), b, a, 0, 0);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E3A8* eb = &(*(sVEntry0010E3A8**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E3A8* ea = &(*(sVEntry0010E3A8**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E468);
#ifdef SKIP_ASM
struct sVEntry0010E468 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* func_0028B180();
extern "C" void cSSXAudio_Punch(void* snd, void* a, void* b);
extern "C" void func_002A0A30(void* snd, void* a, void* b, int c, int d);
extern "C" void cSSXAudio_KnockdownSFX(void* snd, void* obj);
void* cBEAggressionInterface_getThis();
extern "C" void func_00155BF0(void* agg, int a, int b, int c);
extern "C" float func_00119400(char* self);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);

extern "C" void func_0010E468(void* a, void* b)
{
    cSSXAudio_Punch(func_0028B180(), b, a);
    func_002A0A30(func_0028B180(), b, a, 0, 1);
    cSSXAudio_KnockdownSFX(func_0028B180(), b);
    void* agg = cBEAggressionInterface_getThis();
    char* ob = (char*)b + 0x6C0;
    sVEntry0010E468* eb = &(*(sVEntry0010E468**)ob)[7];
    int rb = eb->fn(ob + eb->delta);
    char* oa = (char*)a + 0x6C0;
    sVEntry0010E468* ea = &(*(sVEntry0010E468**)oa)[7];
    func_00155BF0(agg, rb, ea->fn(oa + ea->delta), 3);
    cRider_flagEventBoostPowerUp(b, 2, func_00119400(*(char**)((char*)b + 0x790)));
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E558);
#ifdef SKIP_ASM
extern int D_005308D0[];
extern signed char D_00535C12[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001194C0(void*, int);

extern "C" void func_0010E558(void* self, int* arg)
{
    *(int*)((char*)self + 0xE8) = *(int*)((char*)self + 0xEC);
    if (((D_005308D0[0] >> 9) & 1) == 0)
    {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (D_00535C12[0] == 1)
        {
            func_001194C0(*(void**)((char*)self + 0x790), *arg);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E5D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cRider_doFinishEvent(void* self);
extern "C" void cReplay_eventChapter(void* a, int b, int c);

static inline int state_0010E5D8() { return cBENewRaceInterface_newRace.cGameMode; }

struct sVEntry0010E5D8 { short delta; short index; int (*fn)(void*); };

extern "C" void func_0010E5D8(void* self, int* arg, int a2, int a3)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char* s = (char*)SSXApp->GameModeMan;
    int mode = *cBENewRaceInterface_getLocation(iface);
    int v = 0;
    if (state_0010E5D8() == 0 || state_0010E5D8() == 1 || state_0010E5D8() == 3 || state_0010E5D8() == 2
        || (state_0010E5D8() == 5 && *(int*)(s + 0x98) != 0)
        || (state_0010E5D8() == 6 && *(int*)(s + 0x98) != 0)
        || mode == 5 || mode == 6 || mode == 7 || mode == 8 || mode == 9
        || mode == 10 || mode == 11 || mode == 12 || mode == 13 || mode == 1
        || cBENewRaceInterface_newRace.cGamePath != 0)
        v = 1;
    if (*arg == 1 && v != 0)
    {
        int ok = *(float*)((char*)self + 0x470) >= 0.0f;
        if (!ok)
            cRider_doFinishEvent(self);
    }
    if (*arg == 11)
    {
        char* o = (char*)self + 0x6C0;
        sVEntry0010E5D8* e = &(*(sVEntry0010E5D8**)o)[7];
        void* tgt = SSXApp->Game->Replay;
        cReplay_eventChapter(tgt, e->fn(o + e->delta), arg[1]);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E770);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E770(void* self, float amount)
{
    *(float*)((char*)self + 0x2E8) += amount;
    cSSXAudio_PlayIconSound(func_0028B180(), 0, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E7D0);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E7D0(void* self, float amount)
{
    *(float*)((char*)self + 0x2EC) += amount;
    cSSXAudio_PlayIconSound(func_0028B180(), 1, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E830);
#ifdef SKIP_ASM
int func_0011FE98(void* self);
extern "C" float func_00119448(char* self, float value);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

extern "C" void func_0010E830(void* self, float value)
{
    if (func_0011FE98(self) == 1 || func_0011FE98(self) == 4)
    {
        cRider_flagEventBoostPowerUp(self, 4, func_00119448(*(char**)((char*)self + 0x790), value));
        cSSXAudio_PlayIconSound(func_0028B180(), 2, self, value);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E8B8);
#ifdef SKIP_ASM
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" float func_00119608(int);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_PlayIconSound(void*, int, void*, float);

extern "C" void func_0010E8B8(void* self)
{
    cRider_flagEventBoostPowerUp(self, 0x10, func_00119608(*(int*)((char*)self + 0x790)));
    cSSXAudio_PlayIconSound(func_0028B180(), 3, self, 1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E910);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0010E028(void* self, int mode, float t);
void cRider_flagEventBoostPowerUp_fi(void*, float, int) __asm__("cRider_flagEventBoostPowerUp");
void func_00111AA0_impl(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");
extern "C" float func_00119D40(void* self, int stance, int alternate, int style, int flag);
extern "C" int func_00149778(void* self, int rider, int value);
void func_00270870_10E910(void* self, int key, void* p) __asm__("func_00270870");
extern "C" void func_002948D0(void* mgr, void* rider, float v);

struct sVE_0010E910 { short delta; short index; int (*fn)(void*); };
struct sVEf_0010E910 { short delta; short index; void (*fn)(void*, float); };

extern "C" void func_0010E910(char* self, int a1, int a2, int a3, char* evt, float t)
{
    char* m = *(char**)(self + 0x790);
    int c0 = *(int*)(m + 0x114);
    int b0 = *(int*)(m + 0x134);
    int d0 = *(int*)(m + 0x198);
    float r = func_00119D40(m, *(int*)(self + 0x320) != *(int*)(self + 0x324), a1, a2, a3);
    char* m1 = *(char**)(self + 0x790);
    int c1 = *(int*)(m1 + 0x114);
    int b1 = *(int*)(m1 + 0x134);
    int d1 = *(int*)(m1 + 0x198);
    func_00270870_10E910(SSXApp->Game->Replay, *(int*)(self + 0x86C), m1 + 0xFC);
    if (c0 < c1)
    {
        int v = *(int*)(self + 0x2F4);
        if (v < 10)
        {
            int d = c1 - c0;
            int lim = 10 - v;
            int nv = v + (d < lim ? d : lim);
            *(int*)(self + 0x2F4) = nv;
            if (nv == 10)
                *(float*)(self + 0x2F0) = 60.0f;
        }
    }
    func_00111AA0_impl(*(void**)(self + 0x77C), evt, evt + 0x10, *(int*)(evt + 0x4C), t);
    char* o = self + 0x6C0;
    sVE_0010E910* ve = &(*(sVE_0010E910**)o)[8];
    if (ve->fn(o + ve->delta))
    {
        if (b0 < b1)
            func_0010E028(self, 3, 0.0f);
        else if (c0 < c1)
            func_0010E028(self, 2, 0.0f);
        else if (func_00149778(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(self + 0x86C), d1 - d0))
            func_0010E028(self, 1, 0.0f);
    }
    cRider_flagEventBoostPowerUp_fi(self, r, 1);
    if (0.0f < r)
        *(float*)(self + 0x2EC) = 0.0f;
    sVEf_0010E910* vf = &(*(sVEf_0010E910**)(self + 0x6C0))[17];
    vf->fn(self + vf->delta, t * 0.5f);
    func_002948D0(gSSXAudio, self, r);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010EB30);
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
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" float func_00119B08(cRiderMetrix* m, int a);
extern "C" void func_00296310(void* mgr, cRider* rider);
extern "C" void cSSXAudio_PlayGrunt(void* mgr, void* rider, int a, int b);
extern "C" void func_002961F0(void* mgr, cRider* rider, float v);
void func_0011FEC8_EB30(void* self, int v) __asm__("func_0011FEC8__FPv");
void cRider_setMotionState_EB30(void* self, int v) __asm__("cRider_setMotionState__FPv");
struct sCfg_EB30 { int a; int b; };
extern sCfg_EB30 D_004A4C08;

struct sQuat_EB30
{
    float x, y, z, w;
    sQuat_EB30() {}
    sQuat_EB30(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_EB30 D_004FF130_EB30 __asm__("D_004FF130");
extern sQuat_EB30 D_004FF160_EB30 __asm__("D_004FF160");

static inline sQuat_EB30 AxisAngle_EB30(const sQuat_EB30& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
    return sQuat_EB30(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline t4Vector GetUp_EB30(cRider* rider) { return rider->mMatrix.row[2]; }

// Clears the anim's position offset and sets its heading quaternion to -angle about D_004FF160.
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


extern "C" void func_0010EB30(cRider* rider, int anim, int a2, int a3, char* evt)
{
    sVE_EB30* vt = *(sVE_EB30**)&rider->mVtbl;
    vt[kRiderVt_vibrationBump].fn((char*)rider + vt[kRiderVt_vibrationBump].delta, *(float*)(evt + 0x30) * 5.0f);
    float amt = func_00119B08(rider->mMetrix, a2);
    cRider_flagEventBoostPowerUp(rider, 1, amt);
    func_00296310(func_0028B180(), rider);
    cSSXAudio_PlayGrunt(func_0028B180(), rider, 0, 0);
    func_002961F0(func_0028B180(), rider, amt);
    func_00111AA0_impl(rider->mRiderHandler, evt, evt + 0x20, a3, *(float*)(evt + 0x30));
    func_0011FEC8_EB30(rider, 0xD);
    if (rider->mRailStyle == 3) {
        float ang = 1.5707963705062866f;
        cRider_rotateOrientation(rider, GetUp_EB30(rider), ang);
        cRiderAnimBase_changeHeadingOffset(rider->mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)rider->mRiderAnim, 0.0f);
    } else if (rider->mRailStyle == 4) {
        float ang = -1.5707963705062866f;
        cRider_rotateOrientation(rider, GetUp_EB30(rider), ang);
        cRiderAnimBase_changeHeadingOffset(rider->mRiderAnim, ang);
        ResetAnimOrientation_EB30((char*)rider->mRiderAnim, 0.0f);
    }
    rider->mRailStyle = 0;
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
    rider->mPressType = 0;
    char buf[0x50];
    cRiderAnimBase_getAIAnimFrame(buf, rider->mRiderAnim, anim, D_004A4C08.a, rider->mSlewed[0].target);
    sXf_EB30 a;
    func_0030ECD8(&a, rider->mAnimModel, buf, 0);
    char* trk = (char*)rider->mAnimModel;
    sQuat_EB30* n = (sQuat_EB30*)((rider->mBoneHips << 4) + *(int*)(trk + 0x24));
    sXf_EB30 node;
    node.pos = sQuat_EB30(n->x * *(float*)(trk + 0x140), n->y * *(float*)(trk + 0x144),
                          n->z * *(float*)(trk + 0x148), n->w * *(float*)(trk + 0x14C));
    node.rot = *(sQuat_EB30*)(*(char**)((char*)rider->mAnimModel + 0x28) + (rider->mBoneHips << 4));
    sXf_EB30 y = Combine_EB30(node, Invert_EB30(a));
    *(sXf_EB30*)&rider->mPosition = Combine_EB30(*(sXf_EB30*)&rider->mPosition, y);
    cRider_updateOrientationImplicit(rider);
    sXf_EB30 w = Invert_EB30(y);
    cRiderAnimBase_changeOrientationOffset(rider->mRiderAnim, &w);
    cRiderAnimBase_play(rider->mRiderAnim, anim, 0, -1.0f);
    func_0011FEC8_EB30(rider, 8);
    cRider_setMotionState_EB30(rider, 2);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F1C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_0010F1C0 { short delta; short index; int (*fn)(void*); };
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);
extern "C" void func_0010E8B8_i(void* self, int v) __asm__("func_0010E8B8");
extern "C" void cRider_checkForReset(void* rider, int a, int b);

static inline void computerV_0010F1C0(char* self, int i)
{
    char* obj = self + 0x6C0;
    sVE_0010F1C0* vt = *(sVE_0010F1C0**)obj;
    vt[i].fn(obj + vt[i].delta);
}

extern "C" void func_0010F1C0(char* self, int kind, float amt)
{
    switch (kind) {
    case 0:
        break;
    case 1:
        computerV_0010F1C0(self, 13);
        break;
    case 2:
        computerV_0010F1C0(self, 14);
        break;
    case 3:
        computerV_0010F1C0(self, 15);
        break;
    case 4:
        cRider_flagEventBoostPowerUp(self, 8, amt);
        break;
    case 6:
        func_0010E8B8_i(self, (int)amt);
        break;
    case 5:
        cRider_checkForReset(self, 0, 4);
        break;
    case 7:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010F280);
#ifdef SKIP_ASM
extern "C" float func_00119BB0(int);
extern "C" void cRider_flagEventBoostPowerUp(void*, int, float);

extern "C" void func_0010F280(void* self)
{
    cRider_flagEventBoostPowerUp(self, 1, func_00119BB0(*(int*)((char*)self + 0x790)));
}
#endif

extern "C" void* func_0011A0E0(int);

//100%
INCLUDE_ASM("ai/computer", func_0010F2B8__FPv);
#ifdef SKIP_ASM
void* func_0010F2B8(void* self)
{
    return func_0011A0E0(*(int*)((char*)self + 0x790));
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F2D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0011A110(int, int);
extern "C" void func_00159B08(void*, int, int);

extern "C" void func_0010F2D8(void* self, int a, int b)
{
    func_0011A110(*(int*)((char*)self + 0x790), b);
    func_00159B08(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(int*)((char*)self + 0x86C), a);
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
    cBERewardInterface_awardUserCollectible(cBE_getInterface_Fv(cBE_getBE(), 0xD), self->mAIIndex, peakID);
}
#endif

extern "C" void* cAI_RefreshGroupAI(void* self);

//100%
INCLUDE_ASM("ai/computer", func_0010F398__FPv);
#ifdef SKIP_ASM
void* func_0010F398(void* self)
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
float func_00113128(void* self);

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
            sEnt_0010F3B8* e = &self->riders[i]->ent[j];
            int v = 0;
            if (i != j && j < self->count)
                v = cRider_isLoaded(self->riders[i]) != 0;
            e->vis = v;
            if (j < self->count)
            {
                char* o = (char*)self->riders[j] + 0x6C0;
                sVE_0010F3B8* ve = &(*(sVE_0010F3B8**)o)[8];
                self->riders[i]->ent[j].val = ve->fn(o + ve->delta);
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
        self->riders[i]->f4D8 = func_00113128(self->riders[i]);
        self->riders[i]->fE8 = none;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F560);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int func_00155B50(void* agg, int a, int b);
extern "C" float func_0031C228(float x);
struct sComputer_0010F878;
extern "C" void func_0010F878(sComputer_0010F878* self);
extern "C" void func_0010F998(void* self);

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

extern "C" void func_0010F560(sComputer_0010F560* self)
{
    if (self->frame % 6 != 0) {
        return;
    }
    func_0010F998(self);
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
            sVec_0010F560 d = sub_0010F560(*self->riders[i]->getPos(), *self->riders[j]->getPos());
            float dist;
            float dx = d.x;
            float dy = d.y;
            float sq = dx * dx + dy * dy;
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(sq));
            float ang = atan2_0010F560(dy, dx);
            self->riders[i]->info[j].dist = dist;
            self->riders[j]->info[i].dist = dist;
            self->riders[i]->info[j].angle = wrap_0010F560(ang + 3.1415927410125732f);
            self->riders[j]->info[i].angle = ang;
            if (func_00155B50(cBEAggressionInterface_getThis(), i, j) >= 2) {
                self->riders[i]->info[j].aggressive = 1;
            } else {
                self->riders[i]->info[j].aggressive = 0;
            }
        }
    }
    func_0010F878((sComputer_0010F878*)self);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F878);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int func_00155B50(void* self, int a, int b);

struct sComputer_0010F878
{
    char pad0[0x28];
    char* mRiders[20];  // 0x28
    int mCount;         // 0x78
    int pad7C;          // 0x7C
    int f80;            // 0x80
};

extern "C" void func_0010F878(sComputer_0010F878* self)
{
    if (self->f80 <= 0) return;
    for (int i = 0; i < self->mCount; i++)
    {
        *(int*)(self->mRiders[i] + 0xF0) = 0;
        *(int*)(self->mRiders[i] + 0xF8) = -1;
        for (int j = 0; j < self->mCount; j++)
        {
            if (i == j) continue;
            int a = func_00155B50(cBEAggressionInterface_getThis(), i, j);
            if (a == 4)
            {
                *(int*)(self->mRiders[i] + 0xF0) = 1;
                *(int*)(self->mRiders[i] + 0xF8) = j;
            }
            *(int*)(self->mRiders[i] + 0xFC) = a;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F998);
#ifdef SKIP_ASM
struct sAI_0010F998 { char pad[0x28]; cRider* riders[19]; int mode; int count; };
extern "C" int cAI_isAILoadedAndReady(void* self);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_ChangePosition(void* mgr, void* rider, void* next, void* prev, int oldPos, int newPos);
extern "C" void func_003E6328(int n, int* keys, int* idx);
float func_00113128(void* self);

extern "C" void func_0010F998(void* p)
{
    sAI_0010F998* self = (sAI_0010F998*)p;
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
        cRider* r = self->riders[i];
        switch (self->mode)
        {
        case 1:
            if (r->mIsActor)
                keys[i] = 0xFFFF;
            else
            {
                int far = r->mFinishTime >= 10.0f;
                if (far)
                    keys[i] = -(int)(func_00113128(r) + (float)(r->unk_0EC * 20));
                else
                    keys[i] = -(int)(func_00113128(r) + (float)(r->unk_0EC * 20));
            }
            break;
        case 2:
            if (r->mIsActor)
                keys[i] = -0xFFFF;
            else
                keys[i] = *(int*)((char*)r->mMetrix + 0x198);
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
            cSSXAudio_ChangePosition(func_0028B180(), self->riders[idx[j]], next, prev, self->riders[idx[j]]->unk_0EC, j);
        }
        self->riders[idx[j]]->unk_0EC = j;
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010FBF0);
#ifdef SKIP_ASM
extern "C" int func_0010FBF0(char* self, int value)
{
    int count = 0;
    for (int i = 0; i < *(int*)(self + 0x78); i++) {
        cRider* rider = *(cRider**)(self + 0x28 + i * 4);
        if (*(int*)((char*)rider + 0xAB8) == value) {
            count++;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010FC30);
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

extern "C" int func_0010FC30(sAi0010FC30* self, int team)
{
    int n = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* obj = self->mRiders[i] + 0x6C0;
        sVEntry0010FC30* e = &(*(sVEntry0010FC30**)obj)[9];
        if (e->fn(obj + e->delta))
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
extern char D_00456C48[];

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
        return D_00456C48;
    }
    return D_00459D70[state];
}
#endif
//100%
INCLUDE_ASM("ai/computer", cRiderHandler__getControlStateName);
#ifdef SKIP_ASM
extern const char* D_00459DC8[];
extern char D_00456C48[];

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
        return D_00456C48;
    }
    return D_00459DC8[state];
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_resetRiderFX);
#ifdef SKIP_ASM
extern "C" void cBoardWakeFX_readFromReplayFrame(void*);
extern "C" void cBoardSparkFX_readFromReplayFrame(void*);
extern "C" void cBoardTrailFX_reset(void*);
extern "C" void cSpeedBoostFX_readFromReplayFrame(void*);
void cTrickBoostFX_readFromReplayFrame(void*);
extern "C" void cAirStreamerFX_readFromReplayFrame(void*);
extern "C" void func_002D4BE0(void*);
extern "C" void cPlayerIndicatorFX_readFromReplayFrame(void*);
extern "C" void func_002DF3B0(void*);
extern "C" void cPunchFX_readFromReplayFrame(void*);
void cCheatFX_reset(void*);
// PORT: cWorldPainterMan_reset is defined with an unused self param; this caller passes none.
void cWorldPainterMan_reset_noarg() __asm__("cWorldPainterMan_reset__FPv");
void* func_002306A8(void* self, int i);

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
    cBoardWakeFX_readFromReplayFrame(((char*)self) + 0x3B0);
    cBoardSparkFX_readFromReplayFrame(((char*)self) + 0x470);
    cBoardTrailFX_reset(((char*)self) + 0x520);
    cSpeedBoostFX_readFromReplayFrame(((char*)self) + 0x610);
    cTrickBoostFX_readFromReplayFrame(((char*)self) + 0x9C0);
    cAirStreamerFX_readFromReplayFrame(((char*)self) + 0xAD0);
    func_002D4BE0(((char*)self) + 0xAF0);
    cPlayerIndicatorFX_readFromReplayFrame(((char*)self) + 0xB00);
    func_002DF3B0(((char*)self) + 0xB40);
    cPunchFX_readFromReplayFrame(((char*)self) + 0xC70);
    cCheatFX_reset(((char*)self) + 0xD20);
    cWorldPainterMan_reset_noarg();
    int id = *(int*)((char*)self->GroundMotion.mRider + 0x870);
    if (id >= 0)
    {
        char* obj = (char*)func_002306A8(SSXApp->Game, id);
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
extern "C" void func_00392D18(void*);
extern "C" void cCheatFX_update(void*);
extern int D_004A4534;

struct sVE_00111948 { short delta; short index; int (*fn)(void*); };

/**
 * Updates the rider's player lights and all 11 visual FX parts for this frame.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (D_004A4534) is on. The lights
 * (PlayerLights, +0xD30) are reset first by func_00392D18. On Tour's version takes an a_bIsLocalHuman flag
 * that this one lacks.
 */
extern "C" void cRiderHandler_updateRiderFX(cRiderHandler* self)
{
    if (D_004A4534 != 0) {
        cRider* r = self->GroundMotion.mRider;
        char* o = (char*)&r->mVtbl;
        sVE_00111948* e = &(*(sVE_00111948**)o)[kRiderVt_isComputer];
        if (e->fn(o + e->delta) != 0)
            return;
    }
    func_00392D18((char*)self + 0xD30);
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
extern int D_004A4534;

struct sVE_001119F8 { short delta; short index; int (*fn)(void*); };

/**
 * Draws all 11 of the rider's visual FX parts.
 *
 * Skipped for computer riders while the "Disable AI Visual FX" debug option (D_004A4534) is on.
 */
extern "C" void cRiderHandler_renderRiderFX(cRiderHandler* self)
{
    if (D_004A4534 != 0) {
        cRider* r = self->GroundMotion.mRider;
        char* o = (char*)&r->mVtbl;
        sVE_001119F8* e = &(*(sVE_001119F8**)o)[kRiderVt_isComputer];
        if (e->fn(o + e->delta) != 0)
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

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x)
{
    func_002E23E0_impl((char*)self + 0xB40, a, b, c, 0, x);
}
#endif

//100%
INCLUDE_ASM("ai/computer", cRiderHandler_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_00111AC0 {
public:
    virtual void Write(void* p, int n);
};

extern char D_004D33A0[];
extern "C" void func_00104D68(void* self, void* s);
extern "C" void cAnimModel_writeToReplayFrame(void* self, void* s);
extern "C" void func_00113AA0(void* self, void* s);
extern "C" void func_001174B0(void* self, void* s);
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
extern "C" int func_0026B508(void* tree, void* path);
extern "C" int func_0026B550(void* tree, void* path);

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
    st->Write(self->GroundMotion.mRider, 0x6C0);
    func_00104D68(*(void**)((char*)self->GroundMotion.mRider + 0x784), data);
    cAnimModel_writeToReplayFrame(*(void**)((char*)self->GroundMotion.mRider + 0x780), data);
    func_00113AA0(*(void**)((char*)self->GroundMotion.mRider + 0x788), data);
    func_001174B0(*(void**)((char*)self->GroundMotion.mRider + 0x790), data);
    unsigned int id0 = 0xFFFFFFFF;
    char* p0 = **(char***)((char*)self->GroundMotion.mRider + 0x864);
    if (p0 != 0) {
        id0 = *(unsigned int*)(p0 + 0x150);
    }
    st->Write(&id0, 4);
    st->Write(*(void**)((char*)self->GroundMotion.mRider + 0x864), 0x10);
    unsigned int id1 = 0xFFFFFFFF;
    char* p1 = **(char***)((char*)self->GroundMotion.mRider + 0x868);
    if (p1 != 0) {
        id1 = *(unsigned int*)(p1 + 0x150);
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
    ids[0] = func_0026B508(D_004D33A0, *(void**)((char*)self->GroundMotion.mRider + 0xAB4));
    ids[1] = func_0026B550(D_004D33A0, *(void**)((char*)self->GroundMotion.mRider + 0xAB8));
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

extern char D_004D33A0[];
extern "C" void func_00104DE0(void* self, void* s);
extern "C" void cAnimModel_readFromReplayFrame(void* self, void* s);
extern "C" void func_00113AD8(void* self, void* s);
extern "C" void func_001174E8(void* self, void* s);
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
extern "C" void cBoardWakeFX_readFromReplayFrame(void*);
extern "C" void cBoardSparkFX_readFromReplayFrame(void*);
extern "C" void cBoardTrailFX_reset(void*);
extern "C" void cSpeedBoostFX_readFromReplayFrame(void*);
void cTrickBoostFX_readFromReplayFrame(void*);
extern "C" void cAirStreamerFX_readFromReplayFrame(void*);
extern "C" void func_002D4BE0(void*);
extern "C" void cPlayerIndicatorFX_readFromReplayFrame(void*);
extern "C" void cPunchFX_readFromReplayFrame(void*);
extern "C" void cCheatFX_readFromReplayFrame(void* self, void* s);
extern "C" void* func_0026AF70(void* tree, int id);
extern "C" void* func_0026AF88(void* tree, int id);
extern "C" void func_00312598(void* self, int a1);
extern "C" void cAnimModel_buildSkelMatrices(void* self);
extern "C" void func_00106828(void* self);

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
 * anim (func_00312598), rebuilds the skeleton matrices and refreshes the body spheres (func_00106828).
 *
 * @param data  the replay stream (On Tour cReplayFramePtr*), read through its virtual Read
 */
extern "C" void cRiderHandler_readFromReplayFrame(cRiderHandler* self, char* data)
{
    cStream_00111D98* st = (cStream_00111D98*)data;
    st->Read(self->GroundMotion.mRider, 0x6C0);
    func_00104DE0(*(void**)((char*)self->GroundMotion.mRider + 0x784), data);
    cAnimModel_readFromReplayFrame(*(void**)((char*)self->GroundMotion.mRider + 0x780), data);
    func_00113AD8(*(void**)((char*)self->GroundMotion.mRider + 0x788), data);
    func_001174E8(*(void**)((char*)self->GroundMotion.mRider + 0x790), data);
    int ids[2];
    sRef_00111D98 r0;
    st->Read(&r0, 4);
    st->Read(*(void**)((char*)self->GroundMotion.mRider + 0x864), 0x10);
    if (~r0.id != 0) {
        **(void***)((char*)self->GroundMotion.mRider + 0x864) = r0.get();
    }
    sRef_00111D98 r1;
    st->Read(&r1, 4);
    st->Read(*(void**)((char*)self->GroundMotion.mRider + 0x868), 0x10);
    if (~r1.id != 0) {
        **(void***)((char*)self->GroundMotion.mRider + 0x868) = r1.get();
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
    cBoardWakeFX_readFromReplayFrame(((char*)self) + 0x3B0);
    cBoardSparkFX_readFromReplayFrame(((char*)self) + 0x470);
    cBoardTrailFX_reset(((char*)self) + 0x520);
    cSpeedBoostFX_readFromReplayFrame(((char*)self) + 0x610);
    cTrickBoostFX_readFromReplayFrame(((char*)self) + 0x9C0);
    cAirStreamerFX_readFromReplayFrame(((char*)self) + 0xAD0);
    func_002D4BE0(((char*)self) + 0xAF0);
    cPlayerIndicatorFX_readFromReplayFrame(((char*)self) + 0xB00);
    cPunchFX_readFromReplayFrame(((char*)self) + 0xC70);
    cCheatFX_readFromReplayFrame(((char*)self) + 0xD20, data);
    st->Read(*(void**)((char*)self->GroundMotion.mRider + 0xABC), 0x20);
    st->Read(*(void**)((char*)self->GroundMotion.mRider + 0xAC0), 0x20);
    st->Read(ids, 8);
    *(void**)((char*)self->GroundMotion.mRider + 0xAB4) = func_0026AF70(D_004D33A0, ids[0]);
    *(void**)((char*)self->GroundMotion.mRider + 0xAB8) = func_0026AF88(D_004D33A0, ids[1]);
    sQuad_00111D98* q = *(sQuad_00111D98**)((char*)self->GroundMotion.mRider + 0x78C);
    if (q != 0) {
        *q = *(sQuad_00111D98*)((char*)self->GroundMotion.mRider + 0x110);
    }
    func_00312598(*(void**)((char*)self->GroundMotion.mRider + 0x784), 0);
    cAnimModel_buildSkelMatrices(*(void**)((char*)self->GroundMotion.mRider + 0x780));
    func_00106828(self->GroundMotion.mRider);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112180);
#ifdef SKIP_ASM
extern "C" void* func_0026B5E0(void*, int, int);
extern char D_004D33A0[];
extern "C" float func_00115B08(void* self);

struct sVec4_00112180
{
    float x, y, z, w;
} __attribute__((aligned(16)));

void func_0011D660_112180(void* self, sVec4_00112180* a, sVec4_00112180* b, int n, float t) __asm__("func_0011D660");
extern "C" float func_0026A638(void* path, void* a1, void* a2, void* a3, void* hint, int n);
sVec4_00112180 func_0026AB20_112180(void* path, int* inRange, float t) __asm__("func_0026AB20");

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

extern "C" void func_00112180(char* self, int flag)
{
    float* p = (float*)func_0026B5E0(D_004D33A0, 1, *(int*)(self + 0x86C));
    if (flag)
    {
        sVec4_00112180 dir;
        sVec4_00112180 pos;
        dir.x = p[5];
        dir.y = p[6];
        dir.z = p[7];
        dir.w = 0.0f;
        pos.x = p[2];
        pos.y = p[3];
        pos.z = p[4];
        pos.w = 1.0f;
        vu0AddEq_112180(pos, vu0Scale_112180(dir, func_00115B08(self)));
        func_0011D660_112180(self, &pos, &dir, 5, 0.0f);
    }
    *(int*)(*(char**)(self + 0xABC) + 0x14) = -1;
    *(int*)(*(char**)(self + 0xAC0) + 0x14) = -1;
    *(void**)(self + 0xAB4) = ((void**)p)[8];
    *(void**)(self + 0xAB8) = ((void**)p)[9];
    sVec4_00112180 up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    up.w = 1.0f;
    float out;
    float res = func_0026A638(*(void**)(self + 0xAB4), self + 0x110, &up, &out, *(void**)(self + 0xAC0), 1);
    float t = *(float*)(*(char**)(self + 0xAB4) + 0x38);
    if (t < res)
        res = 0.0f;
    *(float*)(self + 0x4D0) = *(float*)(self + 0x4D4) = t - res;
    res = func_0026A638(*(void**)(self + 0xAB8), self + 0x110, self + 0x490, self + 0x4C8, *(void**)(self + 0xABC), 1);
    *(float*)(self + 0x4C0) = *(float*)(self + 0x4C4) = res;
    sVec4_00112180 pos2 = func_0026AB20_112180(*(void**)(self + 0xAB8), 0, res + 796.0f);
    *(sVec4_00112180*)(self + 0x4A0) = pos2;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112338);
#ifdef SKIP_ASM
struct sEnt_00112338 { int a; int b; int c; int d; };
struct sComp_00112338 { char pad[0x4DC]; sEnt_00112338 ent[12]; int count; };
struct sVec4_00112338 { float x, y, z, w; } __attribute__((aligned(16)));
struct sVE_00112338 { short delta; short index; int (*fn)(void*, float, float); };

extern sEnt_00112338 D_004A5D60[];
extern "C" void* MEM_copy(void*, void*, int);
extern "C" void func_00112FB0(void* self, float* a, float* b, sVec4_00112338* c);
extern "C" void func_001127F0(void* self, int a);
extern "C" float func_0026AC48(void* self);
extern "C" void func_0010E5D8(void* self, int* arg, int a2, int a3);

extern "C" void func_00112338(char* self)
{
    sComp_00112338* s = (sComp_00112338*)self;
    MEM_copy(D_004A5D60, s->ent, s->count << 4);
    int n = s->count;
    sVec4_00112338 c;
    float a;
    float b;
    func_00112FB0(self, &a, &b, &c);
    if (0.0f < *(float*)(self + 0x4D4))
    {
        void* old = *(void**)(self + 0xAB4);
        if (500.0f < a && *(int*)((char*)SSXApp->Game->AI + 0x8) % 60 == 0)
            func_001127F0(self, 0);
        else if (func_0026AC48(*(void**)(self + 0xAB4)) - 200.0f < b)
            func_001127F0(self, 1);
        else
        {
            char* o = *(char**)(self + 0xAB4);
            sVE_00112338* ve = &(*(sVE_00112338**)(o + 0x34))[2];
            if (ve->fn(o + ve->delta, *(float*)(self + 0x4D4), *(float*)(self + 0x4D0)))
                func_001127F0(self, 0);
        }
        if (*(void**)(self + 0xAB4) != old)
        {
            *(int*)(*(char**)(self + 0xAC0) + 0x14) = -1;
            func_00112FB0(self, &a, &b, &c);
        }
    }
    int i;
    for (i = 0; i < s->count; i++)
    {
        int j;
        for (j = 0; j < n; j++)
        {
            if (s->ent[i].a == D_004A5D60[j].a && s->ent[i].b == D_004A5D60[j].b)
            {
                D_004A5D60[j].a = 0;
                break;
            }
        }
        func_0010E5D8(self, (int*)&s->ent[i], j < n, 1);
    }
    for (i = 0; i < n; i++)
    {
        if (D_004A5D60[i].a)
            func_0010E5D8(self, (int*)&D_004A5D60[i], 1, 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112588);
#ifdef SKIP_ASM
extern "C" float func_00112588(void* self, int arg1)
{
    float v = 796.0f;
    if (arg1 == 0) {
        v = 200.0f;
    }
    return v;
}
#endif

// R5900 128-bit GPR quadword, for functions that copy/return a 16-byte
// block via a single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/computer", func_001125A8);
#ifdef SKIP_ASM
extern "C" cQuad128 func_001125A8(void* self)
{
    cQuad128 v = *(cQuad128*)((char*)self + 0x4A0);
    *(cQuad128*)((char*)self + 0x4B0) = v;
    return v;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_001125B8__FPv);
#ifdef SKIP_ASM
int func_001125B8(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_001125C0);
#ifdef SKIP_ASM
struct sVec4_001125C0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_001125C0 { short delta; short index; int (*fn)(void*, float, float); };

// PORT: the unit defines func_001125B8 as int; here its result is used as a bool (xori 1).
bool func_001125B8_b(void* self) __asm__("func_001125B8__FPv");
// PORT: the unit defines func_001125A8(void* self); callers pass a second arg in $5.
cQuad128 func_001125A8_2(void* self, int a) __asm__("func_001125A8");
extern "C" float func_0026A638(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float func_0026A428(void* path, void* a1, void* a2, void* a3, int n);
sVec4_001125C0 func_0026AB20_1125C0(void* path, int* inRange, float t) __asm__("func_0026AB20");
extern "C" float func_0026AC48(void* self);
extern "C" void func_00112A50(void* self, int a);

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

extern "C" void func_001125C0(char* self)
{
    *(float*)(self + 0x4C0) = *(float*)(self + 0x4C4);
    int nf = !func_001125B8_b(self);
    if (nf)
        *(float*)(self + 0x4C4) = func_0026A638(*(void**)(self + 0xAB8), self + 0x110, self + 0x490, self + 0x4C8, *(void**)(self + 0xABC), nf);
    else
        *(float*)(self + 0x4C4) = func_0026A428(*(void**)(self + 0xAB8), self + 0x110, self + 0x490, self + 0x4C8, 0);
    float k = func_00112588(self, nf);
    *(sVec4_001125C0*)(self + 0x4A0) = func_0026AB20_1125C0(*(void**)(self + 0xAB8), 0, *(float*)(self + 0x4C4) + k);
    func_001125A8_2(self, nf);
    if (0.0f < *(float*)(self + 0x4D4))
    {
        if (500.0f < *(float*)(self + 0x4C8) && *(int*)((char*)SSXApp->Game->AI + 0x8) % 60 == 0)
            func_00112A50(self, 0);
        else if (func_0026AC48(*(void**)(self + 0xAB8)) - 200.0f < *(float*)(self + 0x4C4))
            func_00112A50(self, 1);
        else
        {
            char* o = *(char**)(self + 0xAB8);
            sVE_001125C0* ve = &(*(sVE_001125C0**)(o + 0x34))[2];
            if (ve->fn(o + ve->delta, *(float*)(self + 0x4C0), *(float*)(self + 0x4C4)))
                func_00112A50(self, 0);
        }
    }
    float dx = *(float*)(self + 0x4A0) - *(float*)(self + 0x490);
    float dy = *(float*)(self + 0x4A4) - *(float*)(self + 0x494);
    *(float*)(self + 0x4CC) = atan2_001125C0(dy, dx);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_001127F0);
#ifdef SKIP_ASM
struct sVec4_1127F0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern char D_004D33A0[];
extern "C" int func_0026B178(void* tree, void* pos, void** out, int maxOut);
extern "C" float func_0026A428(void* path, void* a1, void* a2, void* a3, int n);
sVec4_1127F0 func_0026AB20_1127F0(void* path, int* inRange, float t) __asm__("func_0026AB20");

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

extern "C" void func_001127F0(void* vself, int skipCur)
{
    char* self = (char*)vself;
    void* paths[3];
    sVec4_1127F0 out;
    sVec4_1127F0 pos;
    float len = vu0Length_1127F0(*(sVec4_1127F0*)(self + 0x1E0));
    void* best = *(void**)(self + 0xAB4);
    float bestD = 9.999999933815813e+36f;
    if (len == 0.0f) {
        pos = *(sVec4_1127F0*)(self + 0x110);
    } else {
        float s = func_00112588(self, 1) / len;
        pos = vu0Add_1127F0(*(sVec4_1127F0*)(self + 0x110), vu0Scale_1127F0(*(sVec4_1127F0*)(self + 0x1E0), s));
    }
    int n = func_0026B178(D_004D33A0, &pos, paths, 3);
    for (int i = 0; i < n; i++) {
        if (skipCur && paths[i] == *(void**)(self + 0xAB4)) {
            continue;
        }
        float tOut;
        float t = func_0026A428(paths[i], &pos, &out, &tOut, 1);
        if (paths[i] == *(void**)(self + 0xAB4)) {
            if (1592.0f < __builtin_fabsf(*(float*)((char*)paths[i] + 0x38) - *(float*)(self + 0x4D0) - t)) {
                *(int*)(*(char**)(self + 0xAC0) + 0x14) = -1;
            }
        }
        t += 796.0f;
        sVec4_1127F0 d1 = vu0Sub_1127F0(pos, func_0026AB20_1127F0(paths[i], 0, t));
        sVec4_1127F0 d0 = vu0Sub_1127F0(*(sVec4_1127F0*)(self + 0x110), out);
        float dist = vu0Dot_1127F0(d0, d0) + vu0Dot_1127F0(d1, d1);
        if (dist < bestD) {
            best = paths[i];
            bestD = dist;
        }
    }
    *(void**)(self + 0xAB4) = best;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112A50);
#ifdef SKIP_ASM
struct sVec4_112A50 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern char D_004D33A0[];
extern "C" int func_0026AFB8(void* tree, void* pos, void** out, int maxOut, int a4);
extern "C" float func_0026AC48(void* self);
extern "C" float func_0026A638(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float func_0026A428(void* path, void* a1, void* a2, void* a3, int n);
sVec4_112A50 func_0026AB20_112A50(void* path, int* inRange, float t) __asm__("func_0026AB20");

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

extern "C" void func_00112A50(void* vself, int keep)
{
    char* self = (char*)vself;
    void* paths[6];
    sVec4_112A50 out;
    sVec4_112A50 pos;
    float len = vu0Length_112A50(*(sVec4_112A50*)(self + 0x1E0));
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    float bestW = -1.0f;
    if (len == 0.0f) {
        pos = *(sVec4_112A50*)(self + 0x110);
    } else {
        pos = vu0Add_112A50(*(sVec4_112A50*)(self + 0x110), vu0Scale_112A50(*(sVec4_112A50*)(self + 0x1E0), 796.0f / len));
    }
    int n = func_0026AFB8(D_004D33A0, &pos, paths, 6, 0);
    for (int i = 0; i < n; i++) {
        if (paths[i] == *(void**)(self + 0xAB8) && keep) {
            continue;
        }
        float tOut;
        float t = func_0026A428(paths[i], &pos, &out, &tOut, 1);
        if (func_0026AC48(paths[i]) - 200.0f < t) {
            continue;
        }
        sVec4_112A50 d1 = vu0Sub_112A50(pos, func_0026AB20_112A50(paths[i], 0, t + 796.0f));
        sVec4_112A50 d0 = vu0Sub_112A50(*(sVec4_112A50*)(self + 0x110), out);
        float dist = vu0Dot_112A50(d0, d0) + vu0Dot_112A50(d1, d1);
        char* o = self + 0x6C0;
        sVEi_112A50* ve = &(*(sVEi_112A50**)o)[9];
        if (ve->fn(o + ve->delta) == 1) {
            sVEf_112A50* vf = &(*(sVEf_112A50**)(self + 0x6C0))[20];
            dist = vf->fn(self + vf->delta, paths[i]);
            if (bestW < dist) {
                best = paths[i];
                bestW = dist;
                if (best == *(void**)(self + 0xAB8)) {
                    best = 0;
                }
            }
        } else {
            if (dist < bestD) {
                best = paths[i];
                bestD = dist;
                if (best == *(void**)(self + 0xAB8)) {
                    best = 0;
                }
            }
        }
    }
    if (best != 0) {
        *(void**)(self + 0xAB8) = best;
        *(int*)(*(char**)(self + 0xABC) + 0x14) = -1;
        float r = func_0026A638(*(void**)(self + 0xAB8), self + 0x110, self + 0x490, self + 0x4C8, *(void**)(self + 0xABC), 1);
        *(float*)(self + 0x4C4) = r;
        *(sVec4_112A50*)(self + 0x4A0) = func_0026AB20_112A50(*(void**)(self + 0xAB8), 0, r + 796.0f);
        *(float*)(self + 0x4C0) = *(float*)(self + 0x4C4);
    }
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112D58);
#ifdef SKIP_ASM
struct sVec4_112D58 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVE_112D58 { short delta; short index; int (*fn)(void*); };

extern char D_004D33A0[];
int func_0026AF98_112D58(void* self, void* a1, void** out, int maxOut) __asm__("func_0026AF98__FPv");
extern "C" float func_0026A428(void* path, void* a1, void* a2, void* a3, int n);
extern "C" float func_0026A638(void* path, void* a1, void* a2, void* a3, void* hint, int n);
extern "C" float func_0026AC48(void* self);
sVec4_112D58 func_00269F18_112D58(void* path, float* t) __asm__("func_00269F18");

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

extern "C" void func_00112D58(char* self)
{
    void* paths[6];
    sVec4_112D58 pos;
    int n = func_0026AF98_112D58(D_004D33A0, self + 0x110, paths, 6);
    void* best = 0;
    float bestD = 9.999999933815813e+36f;
    int i;
    for (i = 0; i < n; i++)
    {
        float tOut;
        float t = func_0026A428(paths[i], self + 0x110, &pos, &tOut, 1);
        float len = func_0026AC48(paths[i]);
        char* o = self + 0x6C0;
        sVE_112D58* ve = &(*(sVE_112D58**)o)[8];
        if (ve->fn(o + ve->delta) == 0 && len - 200.0f < t)
            continue;
        float t2 = t + 796.0f;
        sVec4_112D58 d1 = vu0Sub_112D58(*(sVec4_112D58*)(self + 0x110), func_00269F18_112D58(paths[i], &t2));
        sVec4_112D58 d0 = vu0Sub_112D58(*(sVec4_112D58*)(self + 0x110), pos);
        float dist = vu0Dot_112D58(d0, d0) + vu0Dot_112D58(d1, d1);
        if (dist < bestD)
        {
            best = paths[i];
            bestD = dist;
            if (best == *(void**)(self + 0xAB8))
                best = 0;
        }
    }
    if (*(void**)(self + 0xAB8) == 0)
        return;
    if (best)
        *(void**)(self + 0xAB8) = best;
    *(int*)(*(char**)(self + 0xABC) + 0x14) = -1;
    *(float*)(self + 0x4C4) = func_0026A638(*(void**)(self + 0xAB8), self + 0x110, self + 0x490, self + 0x4C8, *(void**)(self + 0xABC), 1);
    *(sVec4_112D58*)(self + 0x490) = func_00269F18_112D58(*(void**)(self + 0xAB8), (float*)(self + 0x4C4));
    float t3 = *(float*)(self + 0x4C4) + 796.0f;
    *(sVec4_112D58*)(self + 0x4A0) = func_00269F18_112D58(*(void**)(self + 0xAB8), &t3);
    *(float*)(self + 0x4C0) = *(float*)(self + 0x4C4);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00112FB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: an extra `dist` read and the `cp` pointer (cp->DistanceLeft re-read) keep jump.c under its duplicate-insn limit; plain form 97.81%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern float func_0026A638_2FB0(void* track, void* pos, void* iPos, float* iDistXY, int* pi, int is2D) __asm__("func_0026A638");
extern char D_004D33A0[];

// cTrackPath's vtable entry (g++ 2.95 layout: this-delta, index, fn).
struct sVEntry_112FB0 { short delta; short index; int (*fn)(void*, float, float, void*, int); };
// On Tour tPathCheckpoint.
struct tPathCheckpoint_112FB0 { int TimeBonusSec; float DistanceLeft; };
struct sPathSys_112FB0 { char pad0[0x18]; tPathCheckpoint_112FB0 Checkpoints[6]; };

// cRider::checkTrackPathEvents(float& pathDistXY, float& pathParam, t4Vector& pathPos) (On Tour name)
extern "C" void func_00112FB0(void* vself, float* pathDistXY, float* pathParam, sVec4_00112338* pathPos)
{
    cRider* self = (cRider*)vself;
    float r = func_0026A638_2FB0(*(void**)((char*)self + 0xAB4), &self->mPosition, pathPos, pathDistXY, self->mTrackPathHint, 1);
    *pathParam = r;
    char* track = *(char**)((char*)self + 0xAB4);
    float cur = *(float*)((char*)self + 0x4D4);
    float d = *(float*)(track + 0x38) - r;
    self->mDistToFinish = d;
    if (d < cur)
    {
        sPathSys_112FB0* tbl = (sPathSys_112FB0*)D_004D33A0;
        for (int i = 0; i < 6; i++)
        {
            tPathCheckpoint_112FB0* cp = &tbl->Checkpoints[i];
            float dist = tbl->Checkpoints[i].DistanceLeft;
            if (dist == 0.0f || dist < self->mDistToFinish)
                break;
            if (cp->DistanceLeft < *(float*)((char*)self + 0x4D4))
                func_0010E558(self, (int*)cp);
        }
        char* t = *(char**)((char*)self + 0xAB4);
        sVEntry_112FB0* vt = *(sVEntry_112FB0**)(t + 0x34);
        self->mOverlapCount = vt[1].fn(t + vt[1].delta, *(float*)((char*)self + 0x4D4), self->mDistToFinish, (char*)self + 0x4DC, 0xC);
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
INCLUDE_ASM("ai/computer", func_00113128__FPv);
#ifdef SKIP_ASM
float func_00113128(void* self)
{
    return *(float*)((char*)self + 0x4D0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113130__FPv);
#ifdef SKIP_ASM
float func_00113130(void* self)
{
    return *(float*)((char*)self + 0x4D8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113138);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_reset(char* self);

extern "C" void* func_00113138(void* self)
{
    *(unsigned int*)((char*)self + 0x30) = 0xFFFFFFFF;
    cAirPredictor_reset((char*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113170);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00113170(void* self, int flags)
{
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

