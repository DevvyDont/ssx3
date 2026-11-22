#include "common.h"
#include "ai/riderhandler.h"
#include "vtable.h"
#include "sound/ssxaudio.h"
#include "bx/math3d.h"
#include "ai/rider.h"
#include "ai/hudfx.h"
#include "ai/ridermetrix.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_linkToRider);
#ifdef SKIP_ASM
extern "C" void cHUDFX_cHUDFX(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_cHUDFX[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sMetrixEntry_001172E0
{
    char pad_0x00[0x98];
    int index;
    sMetrixEntry_001172E0() { cHUDFX_cHUDFX(this); }
};

/**
 * Links the score object to its rider and, for a rider with a view, creates its HUD message bank.
 *
 * Stores @p rider at +0x1AC. When the rider's mViewIndex is not negative it allocates 44 (0x2C) cHUDFX slots
 * of 0x9C bytes (tag "cHUDFX"), each constructed free (type 0x34) with its index at +0x98, and stores the
 * array and the count at +0x1B0 / +0x1B4. Other riders keep a null bank.
 *
 * @param rider  the owning rider
 */
// PORT: element address built through int (pointer in int).
extern "C" void cRiderMetrix_linkToRider(cRiderMetrix* self, cRider* rider)
{
    self->Rider = (cRider*)rider;
    if (rider->mViewIndex >= 0)
    {
        sMetrixEntry_001172E0** slot = (sMetrixEntry_001172E0**)(&self->HUDfx);
        self->NumHUDfx = 44;
        *slot = new (operator_new_tag(44 * sizeof(sMetrixEntry_001172E0), str_cHUDFX, 0, 0)) sMetrixEntry_001172E0[44];
        for (int i = 0; i < self->NumHUDfx; i++)
        {
            ((sMetrixEntry_001172E0*)(i * 0x9C + (int)self->HUDfx))->index = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetHUDfx);
#ifdef SKIP_ASM


/**
 * Frees every slot of the HUD message bank (type 0x34), when the score object has a bank.
 */
extern "C" void cRiderMetrix_resetHUDfx(cRiderMetrix* self)
{
    if ((self->HUDfx) != 0)
    {
        for (int i = 0; i < self->NumHUDfx; i++)
        {
            (self->HUDfx)[i].ID = 52;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_hasActiveHUDfx);
#ifdef SKIP_ASM


static inline bool rmBitTest00117400(int* bits, int i)
{
    int mask = 1 << i;
    return (*(int*)((char*)bits + ((i & ~0x1F) >> 3)) & mask) != 0;
}

/**
 * Returns whether a HUD effect of @p type is active, rebuilding the 64-bit cache of active types first when it is
 * marked stale.
 *
 * Returns 0 without an effect list (+0x1B0). When the stale flag (+0x1C0) is set, sets the bit of every effect's type
 * (entries of 0x9C bytes, count at +0x1B4) except 52 (a free entry) in the cache (+0x1B8), then clears the flag.
 * cRiderMetrix_resetHUDfx clears the cache and marks it stale. Nothing in the source calls it.
 *
 * @param type  the HUD effect id
 * @return 1 when the type's bit is set, else 0
 */
extern "C" int cRiderMetrix_hasActiveHUDfx(cRiderMetrix* self, int type)
{
    if ((self->HUDfx) == 0)
        return 0;
    if (self->IsActiveFXListCacheInvalid)
    {
        for (int i = 0; i < self->NumHUDfx; i++)
        {
            int entryType = (self->HUDfx)[i].ID;
            if (entryType != 52)
            {
                int off = (entryType & ~0x1F) >> 3;
                int* bitArray = ((int*)&self->ActiveFXListCache[0]);
                *(int*)((char*)bitArray + off) |= 1 << entryType;
            }
        }
        self->IsActiveFXListCacheInvalid = 0;
    }
    return rmBitTest00117400(((int*)&self->ActiveFXListCache[0]), type);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes the rider metrix's first 428 bytes (0x1AC) into a replay frame through @p obj's virtual slot 1.
 *
 * cRiderHandler_writeToReplayFrame calls it for the rider's mMetrix.
 *
 * @param obj  the replay frame
 */
extern "C" void cRiderMetrix_writeToReplayFrame(cRiderMetrix* self, void* obj)
{
    sVEntry<void(void*, void*, int)>* vt = *(sVEntry<void(void*, void*, int)>**)obj;
    vt[1].fn(vthis(obj, vt[1]), self, 428);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads the rider metrix's first 428 bytes (0x1AC) back from a replay frame through @p obj's virtual slot 2.
 *
 * cRiderHandler_readFromReplayFrame calls it for the rider's mMetrix.
 *
 * @param obj  the replay frame
 */
extern "C" void cRiderMetrix_readFromReplayFrame(cRiderMetrix* self, void* obj)
{
    sVEntry<void(void*, void*, int)>* vt = *(sVEntry<void(void*, void*, int)>**)obj;
    vt[2].fn(vthis(obj, vt[2]), self, 428);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_setInitialPointsTotal);
#ifdef SKIP_ASM
/**
 * Sets the score a rider starts the run with, adjusting the current total by the difference.
 *
 * The starting total (+0x1C8, On Tour mPointsTotalInitial) becomes @p pointsTotal and Totals.TotalPoints (+0x198) moves by @p pointsTotal
 * minus the old starting total.
 *
 * @param pointsTotal  the new starting score (On Tour pointsTotal)
 */
extern "C" void cRiderMetrix_setInitialPointsTotal(cRiderMetrix* self, int pointsTotal)
{
    int delta = pointsTotal - self->mPointsTotalInitial;
    self->mPointsTotalInitial = pointsTotal;
    self->Totals.TotalPoints += delta;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_reset);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_resetTotals(cRiderMetrix* self);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void cRiderMetrix_resetCurTrick(cRiderMetrix* self);
extern "C" void cRiderMetrix_resetComboChain(cRiderMetrix* self);

/**
 * Resets the score object for a new run: totals, HUD messages, trick history, current trick and combo.
 *
 * Calls cRiderMetrix_resetTotals() and cRiderMetrix_resetHUDfx(), clears the repeat history (+0xA8, 0x50
 * bytes) and its cursor (+0xF8), calls cRiderMetrix_resetCurTrick() and cRiderMetrix_resetComboChain(), then
 * zeroes the smoothed speed (+0x1A8), suspends the combo clock (+0xA4 = -1), clears the renderer bits at
 * +0x1B8 / +0x1BC and sets +0x1C0 to 1.
 */
extern "C" void cRiderMetrix_reset(cRiderMetrix* self)
{
    cRiderMetrix_resetTotals(self);
    cRiderMetrix_resetHUDfx(self);
    MEM_fill(&self->TrickHistory[0], 0, 80);
    self->TrickHistoryPtr = 0;
    cRiderMetrix_resetCurTrick(self);
    cRiderMetrix_resetComboChain(self);
    *(int*)&self->CurAvgSpeedCPS = 0;
    self->ComboResetDelay = -1.0f;
    MEM_fill(&self->ActiveFXListCache[0], 0, 8);
    self->IsActiveFXListCacheInvalid = 1;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetTotals);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Resets the run statistics: zeroes +0xFC to +0x1A7 and restarts the total score from the starting total.
 *
 * Sets the best-trick points (+0x18C) to -1 and the total (+0x198) to the starting total at +0x1C8.
 */
extern "C" void cRiderMetrix_resetTotals(cRiderMetrix* self)
{
    MEM_fill(&self->Totals, 0, 172);
    self->Totals.BestTrickPoints = -1;
    self->Totals.TotalPoints = self->mPointsTotalInitial;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetComboChain);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_removeHUDfx(cRiderMetrix* self, int type);

/**
 * Ends the rider's trick combo chain and removes its HUD effect.
 *
 * Removes HUD effect 3 (the combo display that cRiderMetrix_addToComboChain() shows), zeroes the combo count (+0x9C) and combo
 * score (+0xA0), and sets the chain timer (+0xA4) back to -1.
 */
extern "C" void cRiderMetrix_resetComboChain(cRiderMetrix* self)
{
    cRiderMetrix_removeHUDfx(self, 3);
    self->NumCombos = 0;
    self->ComboPoints = 0;
    self->ComboResetDelay = -1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addToComboChain);
#ifdef SKIP_ASM
extern "C" void cHUDFX_setAsync(void* self, int type, int value, int arg, float f);

// PORT: g++ <? (min) operator, removed in GCC 4.3.
static inline float rmClamp_117638(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

static inline bool rmIsState_117638(char* p, int st)
{
    return *(int*)p == st;
}

/**
 * Adds a trick's points to the combo chain.
 *
 * For @p points above 0: increments the combo count (+0x9C), adds points x clamp((count + 10) x 0.05, 0.5, 2) to the
 * combo points (+0xA0), sets the reset delay (+0xA4) to -1 and, with a HUD bank (+0x1B0), updates HUD effect 3
 * (cHUDFX_setAsync on bank + 0x1D4) with its current value (0 when that slot's type is 52) and the new count.
 *
 * @param points  the points earned
 */
// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" void cRiderMetrix_addToComboChain(cRiderMetrix* self, int points)
{
    if (points > 0)
    {
        int n = self->NumCombos + 1;
        self->NumCombos = n;
        float rawMult = ((float)n + 10.0f) * 0.05000000074505806f;
        float k = rmClamp_117638(rawMult, 0.5f, 2.0f);
        self->ComboResetDelay = -1.0f;
        self->ComboPoints += (int)(points * k);
        cHUDFX* hudBank = self->HUDfx;
        if (((char*)hudBank) != 0)
        {
            int curValue = 0;
            char* hudSlot = ((char*)hudBank) + 0x1D4;
            if (!rmIsState_117638(hudSlot, 52))
                curValue = *(int*)(((char*)hudBank) + 0x1E8);
            cHUDFX_setAsync(hudSlot, 3, curValue, n, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_cancelComboChainReset);
#ifdef SKIP_ASM
/**
 * Stops the combo-chain timer so the current combo cannot run out while a trick is held.
 *
 * Sets the combo reset delay (+0xA4) to -1.
 */
extern "C" void cRiderMetrix_cancelComboChainReset(cRiderMetrix* self)
{
    self->ComboResetDelay = -1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_delayComboChainReset);
#ifdef SKIP_ASM
/**
 * Gives the combo chain at least @p seconds before it resets.
 *
 * Combo reset delay (+0xA4) = max(itself, @p seconds), using g++'s >? operator.
 *
 * @param seconds  the minimum time left before the combo resets (On Tour delaySec)
 */
// PORT: g++ >? (max) operator, removed in GCC 4.3.
extern "C" void cRiderMetrix_delayComboChainReset(cRiderMetrix* self, float seconds)
{
    self->ComboResetDelay = self->ComboResetDelay >? seconds;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_earnComboChain);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// Same function as cRiderMetrix_addHUDfx1(self, type, value, arg, duration): the float travels in $f12 either way,
// but the original prototype lists it before arg, which decides the argument evaluation order.
extern "C" void cRiderMetrix_addHUDfx1_f(void* self, int type, int value, float duration, int arg) __asm__("cRiderMetrix_addHUDfx1");
extern "C" void cRiderMetrix_evAddCash(void* self, int type, int value);

struct sRaceSettings_117718 {
    char pad[0x48];
    signed char mode;       // 0x48
    signed char online;     // 0x49
};
extern sRaceSettings_117718 D_00535BC8_117718 __asm__("cBENewRaceInterface_newRace");

/**
 * Cashes in the current combo chain: adds its points to the run totals and shows it, then resets the chain.
 *
 * With at least 2 combo links (+0x9C, On Tour NumCombos) and positive combo points (+0xA0): Totals.TotalPoints +=
 * points, NumComboChainLinks += links, BestComboPoints and LongestCombo are raised. When cBENewRaceInterface_newRace
 * has +0x48 == 4 and +0x49 == 0 (the free-ride case cRiderMetrix_earnNameAndPoints also checks; +0x49 is the game path cBENewRaceInterface_setGamePath
 * sets, which this C names online) the combo pays cash instead (cRiderMetrix_evAddCash(2, min(points / 500, 20)));
 * otherwise HUD effects 0x26 (points and links, 1.5 s) and 0x18 (0.7 s) show it. Always ends with
 * cRiderMetrix_resetComboChain.
 *
 * Retail cRiderMetrix fields sit 8 bytes after On Tour's cRiderMetrixState ones up to the combo fields; the run totals
 * (On Tour tRiderStats) start at +0xFC.
 *
 * @param self_  the score object (this)
 */
// Records a trick combo (count at 0x9C, score at 0xA0) in the rider metrics and awards it.
extern "C" void cRiderMetrix_earnComboChain(cRiderMetrix* self_)
{
    int n = self_->NumCombos;
    if (n >= 2) {
        int pts = self_->ComboPoints;
        if (pts > 0) {
            self_->Totals.TotalPoints += pts;
            self_->Totals.NumComboChainLinks += n;
            if (self_->Totals.BestComboPoints < pts)
                self_->Totals.BestComboPoints = pts;
            if (self_->NumCombos > self_->Totals.LongestCombo)
                self_->Totals.LongestCombo = self_->NumCombos;
            cBE_getInterface_Fv(cBE_getBE(), 0);
            int offline = 0;
            if (D_00535BC8_117718.mode == 4)
                offline = D_00535BC8_117718.online == 0;
            if (!offline) {
                cRiderMetrix_addHUDfx1_f(((char*)self_), 38, self_->ComboPoints, 1.5f, self_->NumCombos);
                cRiderMetrix_addHUDfx1_f(((char*)self_), 0x18, self_->ComboPoints, 0.7f, 0);
            } else {
                int cashAmount = self_->ComboPoints / 500;
                cRiderMetrix_evAddCash(((char*)self_), 2, cashAmount > 20 ? 20 : cashAmount);
            }
        }
    }
    cRiderMetrix_resetComboChain(self_);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetCurTrick);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

/**
 * Clears the current-trick state of the score object before a new trick.
 *
 * Zeroes the trick fields +0x0 to +0x98 (stance, pending points, counts, grabs[3] at +0x60, threshold
 * indices), sets the pickup multiplier (+0x18) to 1 and the trackers (rail +0x24, press +0x2C, air +0x30,
 * hold +0x40, +0x6C, handplant +0x78) to -1 (inactive). The combo fields from +0x9C on are left alone.
 *
 * @param riderMetrix  the score object (this)
 */
extern "C" void cRiderMetrix_resetCurTrick(cRiderMetrix* riderMetrix)
{
    *(int*)&riderMetrix->CurTrickPower = 0;
    riderMetrix->CurTrickPointMult = 1.0f;
    *(int*)&riderMetrix->CurTrickStylePower = 0;
    riderMetrix->InitTrickIsFakie = 0;
    riderMetrix->InitTrickIsPipe = 0;
    riderMetrix->InitTrickIsLateSpin = 0;
    riderMetrix->InitTrickRailslideMode = 0;
    riderMetrix->InitTrickPressMode = 0;
    riderMetrix->CurTrickRailslideMode = 0;
    riderMetrix->CurTrickRailslideDist = -1.0f;
    riderMetrix->CurTrickPressMode = 0;
    riderMetrix->CurTrickPressSec = -1.0f;
    riderMetrix->CurTrickAirSec = -1.0f;
    *(int*)&riderMetrix->CurTrickSpinAng = 0;
    *(int*)&riderMetrix->CurTrickFlipAng = 0;
    *(int*)&riderMetrix->CurGrabPowerPerTick = 0;
    riderMetrix->CurGrabHoldSec = -1.0f;
    *(int*)&riderMetrix->CurTrickTotalGrabHoldSec = 0;
    *(int*)&riderMetrix->CurTrickMaxGrabHoldSec = 0;
    riderMetrix->CurTrickNumGrabs = 0;
    riderMetrix->CurTrickNumTweaks = 0;
    riderMetrix->CurTrickNumUbers = 0;
    riderMetrix->CurTrickNumSuperUbers = 0;
    riderMetrix->CurTrickIsUber = 0;
    riderMetrix->CurTrickUberGrind = 0;
    riderMetrix->CurTrickUberGrindHoldSec = -1.0f;
    riderMetrix->CurTrickNumUberGrinds = 0;
    riderMetrix->CurTrickHandplantHoldSec = -1.0f;
    riderMetrix->CurTrickHandplant = 0;
    riderMetrix->CurTrickNumHandplants = 0;
    riderMetrix->CurTrickBonusPoints = 0;
    riderMetrix->CurTrickBigAirBonusLevel = 0;
    riderMetrix->CurTrickLongGrabBonusLevel = 0;
    riderMetrix->CurTrickLongPressBonusLevel = 0;
    riderMetrix->CurTrickLongPlantBonusLevel = 0;
    riderMetrix->CurTrickLongRailBonusLevel = 0;
    MEM_fill(&riderMetrix->CurTrickGrabList[0], 0, 0xC);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_getCurTrickPower__FPv);
#ifdef SKIP_ASM
/**
 * Returns the current trick's power, the float at +0x14 that cRiderMetrix_getCurTrickPoints scales.
 *
 * cRiderMetrix_earnNameAndPoints divides it by 1 + the trick's repeats to get the boost earned.
 *
 * @return the trick power
 */
float cRiderMetrix_getCurTrickPower(void* vself)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    return self->CurTrickPower;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_getCurTrickStyleSubPoints);
#ifdef SKIP_ASM
/**
 * Returns the style points of the trick in progress.
 *
 * (int)(style power (+0x1C) * 10000 + 5), rounded down to a multiple of 10; no points multiplier.
 *
 * @return the style points, a multiple of 10
 */
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(cRiderMetrix* self)
{
    int trickPoints = (int)(self->CurTrickStylePower * 10000.0f + 5.0f);
    return trickPoints - trickPoints % 10;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_getCurTrickPoints);
#ifdef SKIP_ASM
/**
 * Returns the points of the trick in progress, before the trick multiplier.
 *
 * (int)(mPointsMultiplier (+0x1C4) * trick power (+0x14) * 10000 + 5), rounded down to a multiple of 10.
 *
 * @return the trick points, a multiple of 10
 */
extern "C" int cRiderMetrix_getCurTrickPoints(cRiderMetrix* self)
{
    int trickPoints = (int)(self->mPointsMultiplier * self->CurTrickPower * 10000.0f + 5.0f);
    return trickPoints - trickPoints % 10;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_getCurTrickPointsWithMult);
#ifdef SKIP_ASM
/**
 * Returns the points of the trick in progress including its pickup multiplier.
 *
 * (int)(mPointsMultiplier (+0x1C4) * point multiplier (+0x18) * trick power (+0x14) * 10000 + 5), rounded down to a
 * multiple of 10.
 *
 * @return the multiplied trick points, a multiple of 10
 */
extern "C" int cRiderMetrix_getCurTrickPointsWithMult(cRiderMetrix* self)
{
    int trickPoints = (int)(self->mPointsMultiplier * self->CurTrickPointMult * self->CurTrickPower * 10000.0f + 5.0f);
    return trickPoints - trickPoints % 10;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_removeHUDfx);
#ifdef SKIP_ASM


/**
 * Removes the rider's HUD effects of one type by marking their slots free (type 0x34).
 *
 * A type below 0x22 has its own fixed slot; any other type is searched for in the slots from 0x23 up to the
 * slot count. Does nothing when the slot array (+0x1B0) is not allocated.
 *
 * @param self_  the rider metrics (this)
 * @param type   the HUD effect type (On Tour eHUDFXID)
 */
extern "C" void cRiderMetrix_removeHUDfx(cRiderMetrix* self_, int type)
{
    cRiderMetrix* self = self_;
    if (self->HUDfx != 0)
    {
        if (type < 34)
        {
            self->HUDfx[type].ID = 52;
        }
        else
        {
            for (int i = 35; i < self->NumHUDfx; i++)
            {
                if (self->HUDfx[i].ID == type)
                {
                    self->HUDfx[i].ID = 52;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_allocHUDfx);
#ifdef SKIP_ASM


static inline bool slotIsFreeR(const cHUDFX& s)
{
    return s.ID == 0x34;
}

/**
 * Picks the HUD message slot for a message type.
 *
 * Types below 0x22 own slot [type]. Other types take the first free slot (type 0x34) from 0x23 up; with none
 * free, the busy slot with the smallest value / maximum ratio below 1 (the least-aged one) is reused.
 *
 * @param self_  the score object (this)
 * @param type   the message type (On Tour eHUDFXID hfx)
 * @return the slot index, or -1 when no slot could be chosen
 */
extern "C" int cRiderMetrix_allocHUDfx(cRiderMetrix* self_, int type)
{
    cRiderMetrix* self = self_;
    if (type < 34)
    {
        return type;
    }
    float best = 1.0f;
    int bestIdx = -1;
    for (int i = 35; i < self->NumHUDfx; i++)
    {
        if (slotIsFreeR(self->HUDfx[i]))
        {
            return i;
        }
        float ratio = self->HUDfx[i].CurDecay / self->HUDfx[i].MaxDecay;
        if (ratio < best)
        {
            best = ratio;
            bestIdx = i;
        }
    }
    return bestIdx;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addHUDfx);
#ifdef SKIP_ASM
struct sMetrixSlotAE8 {
    char pad_0x00[0x9C];
};

extern "C" void cHUDFX_set(void* slot, int a, const char* b, float c, int d);

// PORT: returns the slot index, but the unit's later declaration (used by
// cRiderMetrix_evAutoResetSurface) says void; bind the int-returning body here.
int cRiderMetrix_addHUDfx_impl(void* self, int a, const char* b, float c, int d) __asm__("cRiderMetrix_addHUDfx");

/**
 * Posts a text message of a type to the HUD message bank.
 *
 * Without a bank it does nothing and returns -1. Otherwise it takes a slot from cRiderMetrix_allocHUDfx() and
 * fills it with cHUDFX_set(): type, text, decay time, data, elapsed 0. A -1 from allocHUDfx is not checked.
 *
 * @param hfx  message type (On Tour eHUDFXID hfx)
 * @param displayStr  text to show, copied into the slot
 * @param decayTime  seconds the message stays
 * @param data  extra data stored in the slot (+0xC)
 * @return the slot index used, or -1 without a bank
 */
int cRiderMetrix_addHUDfx_impl(void* vself, int hfx, const char* displayStr, float decayTime, int data)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    if ((sMetrixSlotAE8*)self->HUDfx == 0)
        return -1;
    int idx = cRiderMetrix_allocHUDfx((cRiderMetrix*)self, hfx);
    cHUDFX_set(&((sMetrixSlotAE8*)self->HUDfx)[idx], hfx, displayStr, decayTime, data);
    return idx;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addHUDfx1);
#ifdef SKIP_ASM
struct sMetrixSlotB88 {
    char pad_0x00[0x9C];
};

extern "C" void cHUDFX_set1(void* slot, int type, int value, float duration, int arg);

// PORT: returns the slot index, but the unit's later declarations say void;
// bind the int-returning body here.
int cRiderMetrix_addHUDfx1_impl(void* self, int type, int value, int arg, float duration) __asm__("cRiderMetrix_addHUDfx1");

/**
 * Posts a number message of a type to the HUD message bank, its text being the number formatted with "%d".
 *
 * Without a bank it does nothing and returns -1. Otherwise it takes a slot from cRiderMetrix_allocHUDfx() and
 * fills it with cHUDFX_set1(): type, the number (+0x14 and as text), duration, data, elapsed 0.
 *
 * @param type      message type (On Tour eHUDFXID hfx)
 * @param value     number to show (On Tour displayInt)
 * @param data       extra data stored in the slot (+0xC), such as a grade or a count
 * @param duration  seconds the message stays
 * @return the slot index used, or -1 without a bank
 */
int cRiderMetrix_addHUDfx1_impl(void* vself, int type, int value, int data, float duration)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    if ((sMetrixSlotB88*)self->HUDfx == 0)
        return -1;
    int idx = cRiderMetrix_allocHUDfx((cRiderMetrix*)self, type);
    cHUDFX_set1(&((sMetrixSlotB88*)self->HUDfx)[idx], type, value, duration, data);
    return idx;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_update);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_earnComboChain(cRiderMetrix*);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
struct sGradeStep_119210;
extern "C" int cRiderMetrix_updateBonusLevelAndCalcPoints(cRiderMetrix* self, int* idx, tHoldBonusDB* tbl, int type, float x);
extern "C" void cHUDFX_update(void* slot);
extern "C" void cRiderMetrix_updateStaticHUD(cRiderMetrix* self);
extern "C" void cSSXAudio_SetCurTrickPower(void* audio, void* rider, float mult);
extern "C" void* memset(void* dst, int c, int n);
int cRider_getMotionState(void* rider);
int cRider_getControlState(void* rider);

extern tHoldBonusDB kBigAirBonusDB[];
extern tHoldBonusDB kLongGrabBonusDB[];
extern tHoldBonusDB kLongRailBonusDB[];
extern tHoldBonusDB kLongPressBonusDB[];
extern tHoldBonusDB kLongPlantBonusDB[];





// PORT: PS2-only VU0 inline asm (vector length).
static inline float len17C28(const t4Vector& v)
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

/**
 * Per-frame update of a rider's scoring: combo timer, hold and air bonuses, speed statistics and HUD effects.
 *
 * Counts the combo reset delay down and cashes the combo in (cRiderMetrix_earnComboChain) when it runs out, advances the big-air and
 * long grab/press/plant/rail bonus levels through cRiderMetrix_updateBonusLevelAndCalcPoints with their bonus tables, accumulates the speed totals
 * (max, average), updates the HUD (cRiderMetrix_updateStaticHUD) and each HUD effect slot (cHUDFX_update), and passes the trick power
 * to cSSXAudio_SetCurTrickPower.
 *
 * Retail cRiderMetrix fields sit 8 bytes after On Tour's cRiderMetrixState ones up to the combo fields; the run totals
 * (On Tour tRiderStats) start at +0xFC.
 */
extern "C" void cRiderMetrix_update(cRiderMetrix* self)
{
    float dt = (self->Rider)->mTimeScale * 0.016666668f;
    float speed = len17C28((*(&(self->Rider)->mVelocity)));
    if (self->ComboResetDelay >= 0.0f) {
        if (cRider_getMotionState((self->Rider)) == 0 && cRider_getControlState((self->Rider)) != 1) {
            self->ComboResetDelay = self->ComboResetDelay - dt;
            if (self->ComboResetDelay <= 0.0f) {
                cRiderMetrix_earnComboChain((cRiderMetrix*)self);
            }
        }
    }
    if (self->CurTrickAirSec >= 0.0f) {
        self->CurTrickAirSec += dt;
        self->CurTrickBonusPoints += cRiderMetrix_updateBonusLevelAndCalcPoints((cRiderMetrix*)self, &self->CurTrickBigAirBonusLevel, kBigAirBonusDB, 0x1C, self->CurTrickAirSec);
    }
    if (self->CurGrabHoldSec >= 0.0f) {
        self->CurGrabHoldSec += dt;
        self->CurTrickPower += self->CurGrabPowerPerTick;
        self->CurTrickBonusPoints += cRiderMetrix_updateBonusLevelAndCalcPoints((cRiderMetrix*)self, &self->CurTrickLongGrabBonusLevel, kLongGrabBonusDB, 0x20, self->CurGrabHoldSec);
    }
    if (self->CurTrickRailslideDist >= 0.0f) {
        float stepDist = speed * dt;
        self->CurTrickRailslideDist += stepDist;
        self->CurTrickPower += stepDist * 5e-05f;
        t4Vector up = (*(&(self->Rider)->mMatrix.row[2]));
        if (up.z < 0.0f) {
            self->CurTrickStylePower += stepDist * 0.0002f;
        }
        self->CurTrickBonusPoints += cRiderMetrix_updateBonusLevelAndCalcPoints((cRiderMetrix*)self, &self->CurTrickLongRailBonusLevel, kLongRailBonusDB, 29, self->CurTrickRailslideDist);
    }
    if (self->CurTrickPressSec >= 0.0f) {
        self->CurTrickPressSec += dt;
        self->CurTrickPower += dt * 0.01f;
        self->CurTrickBonusPoints += cRiderMetrix_updateBonusLevelAndCalcPoints((cRiderMetrix*)self, &self->CurTrickLongPressBonusLevel, kLongPressBonusDB, 0x1E, self->CurTrickPressSec);
    }
    if (self->CurTrickUberGrindHoldSec >= 0.0f) {
        self->CurTrickUberGrindHoldSec += dt;
        self->CurTrickPower += dt * 0.04999999701976776f;
    }
    if (self->CurTrickHandplantHoldSec >= 0.0f) {
        self->CurTrickHandplantHoldSec += dt;
        self->CurTrickPower += dt * 0.04999999701976776f;
        self->CurTrickBonusPoints += cRiderMetrix_updateBonusLevelAndCalcPoints((cRiderMetrix*)self, &self->CurTrickLongPlantBonusLevel, kLongPlantBonusDB, 0x1F, self->CurTrickHandplantHoldSec);
    }
    self->CurAvgSpeedCPS = self->CurAvgSpeedCPS * 0.9f + speed * 0.1f;
    self->Totals.SumSpeedCPS += speed;
    self->Totals.SumTick += 1.0f;
    if (self->Totals.MaxAvgSpeedCPS < self->CurAvgSpeedCPS) {
        self->Totals.MaxAvgSpeedCPS = self->CurAvgSpeedCPS;
    }
    if (self->Totals.MaxSpeedCPS < speed) {
        self->Totals.MaxSpeedCPS = speed;
    }
    cRiderMetrix_updateStaticHUD((cRiderMetrix*)self);
    if ((self->HUDfx) != 0) {
        for (int i = 0; i < self->NumHUDfx; i++) {
            cHUDFX_update(&(self->HUDfx)[i]);
        }
    }
    memset(((char*)&self->ActiveFXListCache[0]), 0, 8);
    self->IsActiveFXListCacheInvalid = 1;
    cSSXAudio_SetCurTrickPower(cAudioInterface_bxAudio(), (self->Rider), self->CurTrickPower);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_updateStaticHUD);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx(void* self, int a, const char* b, float c, int d);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" int cRiderMetrix_calcTrickLevel(cRiderMetrix* self, int points);
int cRider_getControlState(void* rider);
extern "C" int AITick();
extern "C" float fCos(float a);
extern "C" float cPrewindControl_getFlipSpinInputAng(void* p, int* fail);
extern "C" float cSpinControl_getFlipSpinInputAng(void* p, int* fail);
extern "C" float cSpinControl_getFlipSpinAng(void* p, int* fail);
extern "C" float cSpinControl_getInvertScale(void* p);
extern "C" float cSpinControl_getOffAxisScale(void* p);
extern "C" int cBEEconInterface_getMoney1(void* iface, int a);
extern "C" void cSSXAudio_PointsEvent(void* audio, void* rider, int points);
extern "C" void cSSXAudio_StartUberSpin(void* audio, void* rider);
extern "C" void cSSXAudio_EndUberSpin(void* audio, void* rider);
extern "C" void cSSXAudio_MaxPowerEvent(void* audio, void* rider);
extern "C" void cSSXAudio_LoseMaxPowerEvent(void* audio, void* rider);
extern char str_STALLED[];
extern char str_OFF_AXIS[];
extern char str_INVERTED[];
extern char str_LATE_SPIN[];
extern char str_LATE_FLIP[];



struct sVE_117FE0 {
    short delta;
    short index;
    int (*fn)(void*);
};



static inline bool slotFree_117FE0(const cHUDFX* s)
{
    return s->ID == 0x34;
}

static inline float approach_117FE0(float target, float cur, float step)
{
    float r;
    if (target + step < cur)
        r = cur - step;
    else if (cur < target - step)
        r = cur + step;
    else
        r = target;
    return r;
}

static inline bool subStateIs_117FE0(cRiderHandler* s, int st)
{
    return s->WipeoutControl.RiderState == st;
}

// PORT: abs.s helper.
static inline float abs_117FE0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Same function as cHUDFX_setAsync(slot, type, value, arg, f): the float travels in $f12 either way.
extern "C" void cHUDFX_setAsync(void* self, int type, int value, int arg, float f);
extern "C" void cHUDFX_setAsync_f(void* slot, int type, int value, float f, int arg) __asm__("cHUDFX_setAsync");

// PORT: float to int to float in FPU registers.
static inline float trunc_117FE0(float x)
{
    float r;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Refreshes the rider's on-screen trick HUD: current trick points, recovery meter and the spin/flip direction
 * indicators.
 *
 * Skipped for a rider with no HUD bank. Shows the current trick points (cRiderMetrix_getCurTrickPoints) at their level (cRiderMetrix_calcTrickLevel)
 * and plays the points sound; in control state 8 shows the wipeout-recovery meter (effect 11). In the prewind (2) or
 * spin (5) control states it places the input dial (effect 13, eased toward the stick angle) and lights one of eight
 * direction effects (0x10-0x17) from the flip/spin angle with its neighbours weighted by cSpinControl_getInvertScale /
 * getOffAxisScale, then labels the trick STALLED, OFF_AXIS and so on from the difference between input and actual
 * angle; other states clear those effects.
 *
 * @param vself  the score object (this), viewed as self
 */
extern "C" void cRiderMetrix_updateStaticHUD(cRiderMetrix* vself)
{
    cRiderMetrix* self = vself;
    if (self->HUDfx == 0)
        return;
    int pts = cRiderMetrix_getCurTrickPoints((cRiderMetrix*)self);
    if (pts > 0) {
        cRiderMetrix_addHUDfx1(self, 1, pts, cRiderMetrix_calcTrickLevel((cRiderMetrix*)self, pts), -1.0f);
        cSSXAudio_PointsEvent(cAudioInterface_bxAudio(), self->Rider, pts);
    }
    if (cRider_getControlState(self->Rider) == 8 && !subStateIs_117FE0(((self->Rider)->mRiderHandler), 3))
        cHUDFX_setAsync_f(&self->HUDfx[11], 11, 0, ((self->Rider)->mRiderHandler)->WipeoutControl.RecoverPercent <? 1.0f, 0);
    else
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 11);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0x10);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 17);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 18);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 19);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 20);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 21);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 22);
    cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 23);
    if (cRider_getControlState(self->Rider) != 2 && cRider_getControlState(self->Rider) != 5) {
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xC);
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 13);
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xF);
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xE);
    } else {
        int fail = 0;
        float angle;
        if (cRider_getControlState(self->Rider) == 2)
            angle = cPrewindControl_getFlipSpinInputAng(&(self->Rider)->mRiderHandler->PrewindControl, &fail);
        else
            angle = cSpinControl_getFlipSpinInputAng(&(self->Rider)->mRiderHandler->SpinControl, &fail);
        if (fail != 0) {
            cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 13);
        } else {
            float dialPct = angle * 0.15915493667125702f + fCos((float)AITick() * 0.10471976548433304f) * 0.029999999329447746f;
            if (dialPct < 0.0f)
                dialPct += 1.0f;
            if (!slotFree_117FE0(&self->HUDfx[13])) {
                float cur = self->HUDfx[13].CurDecay / self->HUDfx[13].MaxDecay;
                float dialDelta = cur - dialPct;
                if (dialDelta > 0.5f) {
                    dialPct = approach_117FE0(dialPct, cur - 1.0f, 0.03333333507180214f);
                    if (dialPct < 0.0f)
                        dialPct += 1.0f;
                } else if (dialDelta < -0.5f) {
                    dialPct = approach_117FE0(dialPct, cur + 1.0f, 0.03333333507180214f);
                    if (dialPct > 1.0f)
                        dialPct -= 1.0f;
                } else {
                    dialPct = approach_117FE0(dialPct, cur, 0.03333333507180214f);
                }
            }
            cHUDFX_setAsync_f(&self->HUDfx[13], 13, 0, dialPct, 0);
        }
        if (cRider_getControlState(self->Rider) == 2) {
            if (fail != 0)
                cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xC);
            else
                cRiderMetrix_addHUDfx1(self, 0xC, 0, 0, -1.0f);
        } else {
            int fail2 = 0;
            float yaw = cSpinControl_getFlipSpinAng(&(self->Rider)->mRiderHandler->SpinControl, &fail2);
            if (fail2 != 0) {
                cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xC);
                cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 13);
                cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xF);
                cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xE);
            } else {
                cRiderMetrix_addHUDfx1(self, 0xC, 0, 0, -1.0f);
                int invertZone1 = 52;
                int s2 = 52;
                int offAxisZone1 = 52;
                int offAxisZone2 = 52;
                int main;
                                if (abs_117FE0(yaw) > 2.7488937377929688f) {
                    main = 0x10;
                } else if (yaw > 1.9634956121444702f) {
                    main = 23;
                    invertZone1 = 21;
                    s2 = 22;
                    offAxisZone1 = 17;
                    offAxisZone2 = 0x10;
                } else if (yaw > 1.1780973672866821f) {
                    main = 22;
                } else if (yaw > 0.39269912242889404f) {
                    main = 21;
                    invertZone1 = 23;
                    s2 = 22;
                    offAxisZone1 = 19;
                    offAxisZone2 = 20;
                } else if (yaw < -1.9634956121444702f) {
                    main = 17;
                    invertZone1 = 19;
                    s2 = 18;
                    offAxisZone1 = 23;
                    offAxisZone2 = 0x10;
                } else if (yaw < -1.1780973672866821f) {
                    main = 18;
                } else if (yaw < -0.39269912242889404f) {
                    main = 19;
                    invertZone1 = 17;
                    s2 = 18;
                    offAxisZone1 = 21;
                    offAxisZone2 = 20;
                } else {
                    main = 20;
                }
                float w1 = cSpinControl_getInvertScale(&(self->Rider)->mRiderHandler->SpinControl);
                float w2 = cSpinControl_getOffAxisScale(&(self->Rider)->mRiderHandler->SpinControl);
                cHUDFX_setAsync_f(&self->HUDfx[main], main, 0, 1.0f, 0);
                if (invertZone1 != 52)
                    cHUDFX_setAsync_f(&self->HUDfx[invertZone1], invertZone1, 0, w1, 1);
                if (s2 != 52)
                    cHUDFX_setAsync_f(&self->HUDfx[s2], s2, 0, w1, 1);
                if (offAxisZone1 != 52)
                    cHUDFX_setAsync_f(&self->HUDfx[offAxisZone1], offAxisZone1, 0, w2, 2);
                if (offAxisZone2 != 52)
                    cHUDFX_setAsync_f(&self->HUDfx[offAxisZone2], offAxisZone2, 0, w2, 2);
                int prev = 0;
                if (!slotFree_117FE0(&self->HUDfx[14]))
                    prev = self->HUDfx[14].Data;
                int kind = 0;
                if (fail == 0) {
                    float diff = yaw - angle;
                    float wrapTurns = diff * 0.15915493667125702f + 0.5f;
                    float fl = trunc_117FE0(wrapTurns);
                    if (wrapTurns < fl)
                        fl -= 1.0f;
                    diff -= fl * 6.2831854820251465f;
                    float styleMult = 0.0f;
                    float ad = abs_117FE0(diff);
                    if (ad > 2.7488937377929688f) {
                        cRiderMetrix_addHUDfx(self, 0xE, str_STALLED, -1.0f, 1);
                        kind = 1;
                        if (abs_117FE0(yaw) > 2.7488937377929688f || abs_117FE0(yaw) < 0.39269912242889404f) {
                            styleMult = 1.0f;
                        } else {
                            styleMult = 2.0f;
                            if (abs_117FE0(yaw) > 1.1780973672866821f && abs_117FE0(yaw) < 1.9634956121444702f)
                                styleMult = 5.0f;
                        }
                    } else if (ad > 1.1780973672866821f) {
                        float delSign = diff * yaw;
                        if ((delSign > 0.0f && abs_117FE0(yaw) > 1.5707964897155762f) || (delSign < 0.0f && abs_117FE0(yaw) < 1.5707964897155762f)) {
                            if (w2 > 0.10000000149011612f) {
                                cRiderMetrix_addHUDfx(self, 0xE, str_OFF_AXIS, -1.0f, 2);
                                styleMult = 3.0f;
                                kind = 2;
                            }
                        } else if (w1 > 0.10000000149011612f) {
                            cRiderMetrix_addHUDfx(self, 0xE, str_INVERTED, -1.0f, 3);
                            styleMult = 2.0f;
                            kind = 3;
                        }
                    } else if (ad > 0.39269912242889404f) {
                        float aa = abs_117FE0(angle);
                        if (aa < 0.7853982448577881f || aa > 2.3561947345733643f) {
                            cRiderMetrix_addHUDfx(self, 0xE, str_LATE_SPIN, -1.0f, 4);
                            styleMult = 1.0f;
                            kind = 4;
                        } else {
                            styleMult = 2.0f;
                            cRiderMetrix_addHUDfx(self, 0xE, str_LATE_FLIP, -1.0f, 5);
                            kind = 5;
                        }
                    }
                    if (styleMult == 0.0f) {
                        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xF);
                        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 0xE);
                    } else {
                        cRiderMetrix_addHUDfx1(self, 0xF, (int)styleMult, 0, -1.0f);
                        self->CurTrickStylePower += styleMult * 0.0008333333535119891f;
                        if (kind != prev) {
                            int x = cRiderMetrix_getCurTrickStyleSubPoints((cRiderMetrix*)self);
                            int dispPoints = self->CurTrickBonusPoints + x;
                            cHUDFX_setAsync_f(&self->HUDfx[2], 2, dispPoints, 0.0f, x);
                        }
                    }
                }
            }
        }
    }
    float potentialPct;
    if (slotFree_117FE0(&self->HUDfx[5])) {
        potentialPct = self->Rider->unk_2F8;
    } else {
        float actualLevel = self->Rider->unk_2F8;
        float target;
        if (self->Rider->mBoostMode != 3)
            target = actualLevel + self->CurTrickPower;
        else
            target = actualLevel;
        potentialPct = approach_117FE0(target <? 1.0f, self->HUDfx[5].CurDecay / self->HUDfx[5].MaxDecay, 0.01666666753590107f);
    }
    float actualPct;
    if (slotFree_117FE0(&self->HUDfx[6]))
        actualPct = self->Rider->unk_2F8;
    else
        actualPct = approach_117FE0(self->Rider->unk_2F8, self->HUDfx[6].CurDecay / self->HUDfx[6].MaxDecay, 0.01666666753590107f);
    cHUDFX_setAsync_f(&self->HUDfx[5], 5, 0, potentialPct, 0);
    cHUDFX_setAsync_f(&self->HUDfx[6], 6, 0, actualPct, 0);
    if (self->Rider->mUberLevel == 0) {
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 8);
    } else {
        if (slotFree_117FE0(&self->HUDfx[8]))
            cHUDFX_setAsync_f(&self->HUDfx[8], 8, 0, 0.0f, 0);
        int n = self->Rider->mUberLevel;
        if (n >= 11)
            n = 10;
        int lvl = n - 1;
        float cur = self->HUDfx[8].CurDecay / self->HUDfx[8].MaxDecay;
        float curPct = approach_117FE0((float)lvl * 0.1111111119389534f <? 1.0f, cur, 0.0066666672937572f);
        cHUDFX_setAsync(&self->HUDfx[8], 8, 0, lvl, curPct);
    }
    int uberCount = self->Rider->mUberLevel;
    if (uberCount >= 11)
        uberCount = 10;
    int left = 10 - uberCount;
    if ((self->CurTrickIsUber != 0 || self->CurTrickNumUbers != 0) && left > 0) {
        int k = self->CurTrickNumUbers;
        if (self->CurTrickIsUber != 0)
            k++;
        if (left < k)
            k = left;
        if (slotFree_117FE0(&self->HUDfx[10])) {
            cHUDFX_setAsync_f(&self->HUDfx[10], 10, 0, 0.0f, 0);
            cSSXAudio_StartUberSpin(cAudioInterface_bxAudio(), self->Rider);
        }
        float cur = self->HUDfx[10].CurDecay / self->HUDfx[10].MaxDecay;
        float curPct = approach_117FE0((float)k * 0.1111111119389534f <? 1.0f, cur, 0.01666666753590107f);
        cHUDFX_setAsync_f(&self->HUDfx[10], 10, 0, curPct, k);
    } else {
        if (!slotFree_117FE0(&self->HUDfx[10]))
            cSSXAudio_EndUberSpin(cAudioInterface_bxAudio(), self->Rider);
        cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 10);
    }
    if (slotFree_117FE0(&self->HUDfx[9])) {
        if (self->Rider->mUberTimer > 0.0f) {
            cHUDFX_setAsync_f(&self->HUDfx[9], 9, 0, 0.0f, 0);
            cSSXAudio_MaxPowerEvent(cAudioInterface_bxAudio(), self->Rider);
        }
    } else {
        float cur = self->HUDfx[9].CurDecay / self->HUDfx[9].MaxDecay;
        if (cur == 1.0f && self->Rider->mUberTimer == 0.0f) {
            cRiderMetrix_removeHUDfx((cRiderMetrix*)self, 9);
            cSSXAudio_LoseMaxPowerEvent(cAudioInterface_bxAudio(), self->Rider);
        } else {
            cHUDFX_setAsync_f(&self->HUDfx[9], 9, 0, approach_117FE0(1.0f - self->Rider->mUberTimer * 0.05000000074505806f, cur, 0.01666666753590107f), self->Rider->mUberLevel > 11 ? 11 : self->Rider->mUberLevel);
        }
    }
    cHUDFX_setAsync_f(&self->HUDfx[7], 7, self->Totals.TotalPoints, 0.0f, 0);
    cHUDFX* comboSlot = &self->HUDfx[3];
    if (!slotFree_117FE0(comboSlot)) {
        int want = self->ComboPoints;
        int have = self->HUDfx[3].DisplayInt;
        float cur = self->HUDfx[3].CurDecay / self->HUDfx[3].MaxDecay;
        int dispPoints;
        if (want + 70 < have)
            dispPoints = have - 70;
        else if (have < want - 70)
            dispPoints = have + 70;
        else
            dispPoints = want;
        cHUDFX_setAsync_f(comboSlot, 3, dispPoints, approach_117FE0(1.0f, cur, 0.01666666753590107f), self->NumCombos);
        if (self->ComboResetDelay < 0.0f)
            self->HUDfx[3].AdditionalData = 4;
        else
            self->HUDfx[3].AdditionalData = (int)(self->ComboResetDelay * 3.266666889190674f);
    }
    int x = cRiderMetrix_getCurTrickStyleSubPoints((cRiderMetrix*)self);
    cHUDFX* s2 = &self->HUDfx[2];
    int val = self->CurTrickBonusPoints + x;
    if (!slotFree_117FE0(s2)) {
        float curPct;
        if (self->HUDfx[2].DisplayInt - self->HUDfx[2].Data == self->CurTrickBonusPoints)
            curPct = approach_117FE0(1.0f, self->HUDfx[2].CurDecay / self->HUDfx[2].MaxDecay, 0.01666666753590107f);
        else
            curPct = 0.0f;
        cHUDFX_setAsync_f(s2, 2, val, curPct, x);
    } else if (val > 0) {
        cHUDFX_setAsync_f(s2, 2, val, 0.0f, x);
    }
    void* stats = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* obj = (char*)&self->Rider->mVtbl;
    sVE_117FE0* vt = *(sVE_117FE0**)obj;
    int moneyTotal = cBEEconInterface_getMoney1(stats, vt[7].fn(vthis(obj, vt[7])));
    cHUDFX_setAsync_f(&self->HUDfx[25], 25, moneyTotal, 0.0f, 0);
    if (!slotFree_117FE0(&self->HUDfx[4])) {
        int have = self->HUDfx[4].DisplayInt;
        float cur = self->HUDfx[4].CurDecay / self->HUDfx[4].MaxDecay;
        cHUDFX_setAsync_f(&self->HUDfx[4], 4, have, approach_117FE0(1.0f, cur, 0.01666666753590107f), 0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addTrick);
#ifdef SKIP_ASM
struct sTrickId;
extern "C" void gGenTrickName(char* buf, sTrickId* id, int size);
extern "C" void cRiderMetrix_addHUDfx(void* self, int a, const char* b, float c, int d);
extern "C" void cWScriptMan_eventTrick(void* self, void* d);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Announces a landed trick: shows its generated name on the HUD and tells the world scripts.
 *
 * gGenTrickName into a 256-byte buffer, cRiderMetrix_addHUDfx(0, name, 3.0f, @p repeat), then cWScriptMan_eventTrick (On Tour
 * cWScriptMan::eventTrick) on cWScriptMan_TheWScriptMan.
 *
 * @param id  the trick (On Tour tTrickNameDef *td)
 * @param repeat  On Tour isRepeat: the trick repeats a recent one
 */
extern "C" void cRiderMetrix_addTrick(cRiderMetrix* self, sTrickId* id, int repeat)
{
    char buf[0x100];
    gGenTrickName(buf, id, 0xFF);
    cRiderMetrix_addHUDfx(self, 0, buf, 3.0f, repeat);
    cWScriptMan_eventTrick(cWScriptMan_TheWScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addGrab);
#ifdef SKIP_ASM

/**
 * Adds a grab to the current trick's three-entry grab list (+0x60).
 *
 * The grab goes into the first empty entry; when it equals the last entry already filled, it is not added again.
 *
 * @param self_  the cRiderMetrix (this)
 * @param value  the grab id
 */
extern "C" void cRiderMetrix_addGrab(cRiderMetrix* self_, int value)
{
    cRiderMetrix* self = self_;
    for (int i = 0; i < 3; i++)
    {
        if (self->CurTrickGrabList[i] == 0)
        {
            self->CurTrickGrabList[i] = value;
            break;
        }
        if (self->CurTrickGrabList[i] == value && i < 2 && self->CurTrickGrabList[i + 1] == 0)
        {
            self->CurTrickGrabList[i] = value;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addToHistory);
#ifdef SKIP_ASM
struct sTrickId;

struct sTrickId190F0 {
    int w0;
    int w1;
};


static inline int func_trickIdEqual(const void* a, const void* b, int size)
{
    const unsigned char* p = (const unsigned char*)a;
    const unsigned char* q = (const unsigned char*)b;
    while (size--) {
        if (*p != *q) {
            return 0;
        }
        p++;
        q++;
    }
    return 1;
}

/**
 * Records a trick in the ten-entry history (+0xA8, index +0xF8) and returns how many times it was already there.
 *
 * Returns 0 without recording when any of the words at +0x8, +0x20, +0x28, +0x70 or +0x7C is set. The 8-byte trick id
 * is compared with all ten entries only when its flag fields are not the two excluded values ((w1 & 0x3F800) == 0x800
 * or (w0 & 0xFC00000) == 0x400000); it is then stored at the ring index.
 *
 * @param self_  the cRiderMetrix (this)
 * @param id_  the trick name id (8 bytes)
 * @return the number of earlier copies
 */
// sTrickId is defined later in the unit, hence the casts. Don't copy id_ into a typed
// local: a separate local shifts every register by one.
extern "C" int cRiderMetrix_addToHistory(cRiderMetrix* self_, sTrickId* id_)
{
    cRiderMetrix* self = self_;
    int count;

    if (self->InitTrickIsLateSpin != 0 || self->CurTrickRailslideMode != 0 || self->CurTrickUberGrind != 0 || self->CurTrickHandplant != 0 || self->CurTrickPressMode != 0) {
        return 0;
    }
    count = 0;
    if ((((sTrickId190F0*)id_)->w1 & 0x3F800) != 0x800 && (((sTrickId190F0*)id_)->w0 & 0xFC00000) != 0x400000) {
        for (int i = 0; i < 10; i++) {
            if (func_trickIdEqual(id_, &((sTrickId190F0*)&self->TrickHistory[0])[i], 8)) {
                count++;
            }
        }
    }
    ((sTrickId190F0*)&self->TrickHistory[0])[self->TrickHistoryPtr] = *(sTrickId190F0*)id_;
    self->TrickHistoryPtr++;
    self->TrickHistoryPtr %= 10;
    return count;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_updateBonusLevelAndCalcPoints);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_removeHUDfx(cRiderMetrix* self, int type);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);


/**
 * Steps a hold/air bonus up a level when its measure passes the next threshold, and returns the bonus points earned.
 *
 * Looks at entry *@p idx of @p tbl ({threshold, points}); a negative threshold ends the table. When @p x reaches it:
 * points = (int)(entry points * mPointsMultiplier (+0x1C4) + 0.5), the level advances, HUD effects 0x1C-0x20 are
 * removed and effect @p type shows the points and threshold for 1.5 s.
 *
 * @param idx  the bonus level to test and advance (On Tour curBonusLevel)
 * @param tbl  the bonus table (On Tour tHoldBonusDB): {threshold, points} pairs ending with a negative threshold
 * @param type  the HUD effect to show on advancing (On Tour hfxAdvance)
 * @param x  the current measure, e.g. air or hold seconds (On Tour curMeasure)
 * @return the bonus points earned, or 0
 */
extern "C" int cRiderMetrix_updateBonusLevelAndCalcPoints(cRiderMetrix* self, int* idx, tHoldBonusDB* tbl, int type, float x)
{
    // PORT: pointer arithmetic through int.
    tHoldBonusDB* bonusStep = (tHoldBonusDB*)(*idx * 8 + (int)tbl);
    float stepThreshold = bonusStep->minTarget;
    if (stepThreshold < 0.0f) return 0;
    if (stepThreshold <= x)
    {
        float pointsMult = self->mPointsMultiplier;
        int pointsEarned = (int)(bonusStep->points * pointsMult + 0.5f);
        *idx = *idx + 1;
        cRiderMetrix_removeHUDfx(self, 0x1C);
        cRiderMetrix_removeHUDfx(self, 29);
        cRiderMetrix_removeHUDfx(self, 0x1E);
        cRiderMetrix_removeHUDfx(self, 0x1F);
        cRiderMetrix_removeHUDfx(self, 0x20);
        cRiderMetrix_addHUDfx1(self, type, pointsEarned, (int)stepThreshold, 1.5f);
        return pointsEarned;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_calcTrickLevel);
#ifdef SKIP_ASM
/**
 * Rates a trick's points on a 0-5 level scale.
 *
 * Thresholds: < 1000 -> 0, < 2500 -> 1, < 4000 -> 2, < 7500 -> 3, < 11500 -> 4, else 5.
 *
 * @param points  the trick's points (On Tour trickPoints)
 * @return the level, 0 to 5
 */
extern "C" int cRiderMetrix_calcTrickLevel(cRiderMetrix* self, int points)
{
    if (points < 1000)
    {
        return 0;
    }
    if (points < 2500)
    {
        return 1;
    }
    if (points < 4000)
    {
        return 2;
    }
    if (points < 7500)
    {
        return 3;
    }
    if (points < 11500)
    {
        return 4;
    }
    return 5;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evReset);
#ifdef SKIP_ASM
extern "C" int cRiderMetrix_losePoints(cRiderMetrix* self);

/**
 * Scores a reset: the trick in progress is lost, the combo ends and a boost penalty is returned.
 *
 * Adds cRiderMetrix_losePoints to Totals.TotalPointsLostToResets (+0x1A4), resets the current trick and combo chain
 * and increments Totals.NumResets (+0x120).
 *
 * @param isUserReset  On Tour isUserReset: the player asked for the reset
 * @return the boost change: -0.7 x the rider's unk_2F8 for a user reset, else -0.1 x it
 */
extern "C" float cRiderMetrix_evReset(cRiderMetrix* self, int isUserReset)
{
    self->Totals.TotalPointsLostToResets += cRiderMetrix_losePoints(self);
    cRiderMetrix_resetCurTrick(self);
    cRiderMetrix_resetComboChain(self);
    self->Totals.NumResets += 1;
    if (!isUserReset)
        return self->Rider->unk_2F8 * -0.10000000149011612f;
    return self->Rider->unk_2F8 * -0.699999988079071f;
}
#endif

extern "C" void cRiderMetrix_earnComboChain(cRiderMetrix*);

//99.38% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evFinishRace);
#ifdef SKIP_ASM
/**
 * Scores crossing the finish line: banks the current combo.
 *
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evFinishRace(cRiderMetrix* self)
{
    cRiderMetrix_earnComboChain(self);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evFrag);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

/**
 * Scores knocking another rider down: counts the frag, shows it and earns a boost.
 *
 * Totals.NumFrags (+0x128) += 1 and HUD effect 0x2C for 1.5 s.
 *
 * @return 1.0, the boost the frag earns
 */
extern "C" float cRiderMetrix_evFrag(cRiderMetrix* self)
{
    self->Totals.NumFrags += 1;
    cRiderMetrix_addHUDfx1(((char*)self), 44, 0, 0, 1.5f);
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evCollectMultiplier);
#ifdef SKIP_ASM
extern "C" void cHUDFX_setAsync(void* self, int type, int value, int arg, float f);

/**
 * Records a collected multiplier and shows it when it beats the best so far.
 *
 * Increments the multiplier count (+0x130); with a HUD bank (+0x1B0) and @p value above the stored best (+0x18) it
 * shows HUD effect 4 with (int) @p value (cHUDFX_setAsync on bank + 0x270) and stores the new best.
 *
 * @param value  the multiplier
 * @return 0.0 (no boost)
 */
extern "C" float cRiderMetrix_evCollectMultiplier(cRiderMetrix* self, float value)
{
    self->Totals.NumMultipliers += 1;
    cHUDFX* hudBank = self->HUDfx;
    if (((char*)hudBank) != 0)
    {
        if (self->CurTrickPointMult < value)
        {
            cHUDFX_setAsync(((char*)hudBank) + 0x270, 4, (int)value, 0, 0.0f);
            self->CurTrickPointMult = value;
        }
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evCheckpoint);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" int cGameModeMan_riderCheckpointHit(void* mgr, int id, int a2);
extern "C" void func_002A3CE8(void* snd, void* rider, int a2);


static inline bool rmRiderFlags_1194C0(char* vvr)
{
    cRider* vr = (cRider*)vvr;
    cRider* r = (cRider*)vr;
    return r->mIsHuman != 0 && r->mIsLocal != 0;
}

/**
 * Scores passing a checkpoint: a human rider gets the checkpoint's time bonus shown.
 *
 * Only for a human rider (cBeing vtable slot 8): cGameModeMan_riderCheckpointHit (On Tour cGameModeMan::riderCheckpointHit) with the
 * rider's id (slot 7) and @p timeBonusSec; when accepted, HUD effect 0x29 shows @p timeBonusSec for 2.5 s and, when the rider's +0x874 and
 * +0x87C are both set, func_002A3CE8 plays the checkpoint sound.
 *
 * @param timeBonusSec  the checkpoint's time bonus in seconds (On Tour sec)
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evCheckpoint(cRiderMetrix* self, int timeBonusSec)
{
    char* sub = (char*)&self->Rider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)sub;
    if (vt[8].fn(vthis(sub, vt[8])) != 0)
    {
        void* mgr = SSXApp->GameModeMan;
        char* sub2 = (char*)&self->Rider->mVtbl;
        sVEntry<int(void*)>* vt2 = *(sVEntry<int(void*)>**)sub2;
        if (cGameModeMan_riderCheckpointHit(mgr, vt2[7].fn(vthis(sub2, vt2[7])), timeBonusSec) != 0)
        {
            cRiderMetrix_addHUDfx1(((char*)self), 41, timeBonusSec, 0, 2.5f);
            if (rmRiderFlags_1194C0((char*)self->Rider))
            {
                func_002A3CE8(cAudioInterface_bxAudio(), self->Rider, 1);
            }
        }
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evSplitTime);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

/**
 * Shows HUD effect 42 with @p value for 5 seconds.
 *
 * Called by cGMTimeHandler_riderCheckpointHit (game mode) with a time difference in seconds.
 *
 * @param value  the value to show
 * @return 0.0
 */
extern "C" float cRiderMetrix_evSplitTime(cRiderMetrix* self, int value)
{
    cRiderMetrix_addHUDfx1(self, 42, value, 0, 5.0f);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evSplitPoints);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

/**
 * Shows HUD effect 43 with @p value for 5 seconds.
 *
 * Called by cGMPointHandler_riderCheckpointHit (game mode) with a points difference.
 *
 * @param value  the value to show
 * @return 0.0
 */
extern "C" float cRiderMetrix_evSplitPoints(cRiderMetrix* self, int value)
{
    cRiderMetrix_addHUDfx1(self, 43, value, 0, 5.0f);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evAddPoints);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" void cRiderMetrix_evAddCash(void* self, int kind, int amount);

// View of sGameSettings (defined later in this unit); bound to the same symbol by asm label.
struct sGameSettings_119608 {
    char pad_0x00[0x48];
    signed char eventKind; // 0x48: 4 = free ride
    signed char gameType;  // 0x49: 0 = Conquer the Mountain
};
extern sGameSettings_119608 D_00535BC8_119608 __asm__("cBENewRaceInterface_newRace");

/**
 * Adds trick points to the score (+0x198), or turns them into cash in a career free ride.
 *
 * The points are scaled by the multiplier at +0x1C4 and rounded. In a free ride (cBENewRaceInterface_newRace eventKind
 * 4) of game type 0 they become cash: cRiderMetrix_evAddCash(1, points / 500, at most 20); otherwise HUD effects 35
 * (2.5 s) and 0x18 (0.7 s) show them. The score grows either way. The new-race interface is fetched and not used.
 *
 * @param pts  the points
 * @return 0.0
 */
extern "C" float cRiderMetrix_evAddPoints(cRiderMetrix* self, int pts)
{
    int points = (int)(pts * self->mPointsMultiplier + 0.5f);
    cBE_getInterface(cBE_getBE(), 0);
    int career = D_00535BC8_119608.eventKind == 4 && D_00535BC8_119608.gameType == 0;
    if (!career)
    {
        cRiderMetrix_addHUDfx1(self, 35, points, 0, 2.5f);
        cRiderMetrix_addHUDfx1(self, 0x18, points, 0, 0.699999988079071f);
    }
    else
    {
        int amount = points / 500;
        if (amount > 20)
        {
            amount = 20;
        }
        cRiderMetrix_evAddCash(self, 1, amount);
    }
    self->Totals.TotalPoints += points;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evGrabHold);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEBAGTInterface_getGrabPoints(void* iface, int id);
int cBEBAGTInterface_getGrabPointsPerSec(void* iface, int id);

/**
 * Scores the start of a grab: its base points, its hold rate and the uber flag.
 *
 * From the BAGT interface (BE interface 6): trick power (+0x14, CurTrickPower) += getGrabPoints(@p id) / 10000 and the
 * grab power per tick (+0x3C) = getGrabPointsPerSec(@p id) / 600000. The hold timer (+0x40) starts at 0 if idle, grab
 * ids >= 0x23 (ubers) set CurTrickIsUber (+0x5C), and the combo timer is suspended (cRiderMetrix_cancelComboChainReset). Field names are On
 * Tour's cRiderMetrixState ones, which sit 8 bytes lower there.
 *
 * @param id  the grab (On Tour eTrickNameGrab grabID); ids from 0x23 are uber tricks
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evGrabHold(cRiderMetrix* self, int id)
{
    float zero = 0.0f;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float perGrabPower = cBEBAGTInterface_getGrabPoints(iface, id) * 9.999999747378752e-05f;
    float perGrabTickPower = cBEBAGTInterface_getGrabPointsPerSec(iface, id) * 1.6666666624587378e-06f;
    self->CurTrickPower += perGrabPower;
    self->CurGrabPowerPerTick = perGrabTickPower;
    if (self->CurGrabHoldSec < zero)
    {
        self->CurGrabHoldSec = zero;
    }
    if (id >= 35)
    {
        self->CurTrickIsUber = 1;
    }
    cRiderMetrix_cancelComboChainReset(self);
    return zero;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evReleaseGrab);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addGrab(cRiderMetrix* self_, int value);

/**
 * Scores the end of a grab: records it in the trick and banks its hold time.
 *
 * cRiderMetrix_addGrab (On Tour addGrab) adds @p grabID to the grab list. Ids >= 0x23 count as ubers (+0x54), and as super ubers
 * (+0x58) while the rider's mUberLevel >= 6; ids >= 0x13 as tweaks (+0x50); others as grabs (+0x4C). The hold time
 * (+0x40) raises the longest hold (+0x48) and adds to the total (+0x44); the uber flag clears and the timer goes back
 * to -1. Field names are On Tour's cRiderMetrixState ones, which sit 8 bytes lower there.
 *
 * @param riderMetrix  the score object (this)
 * @param grabID  the grab released (On Tour eTrickNameGrab grabID)
 * @return 0.0 (no boost change)
 */
// PORT: g++ >? (max) operator.
extern "C" float cRiderMetrix_evReleaseGrab(cRiderMetrix* riderMetrix, int grabID)
{
    cRiderMetrix_addGrab(riderMetrix, grabID);
    if (grabID >= 35)
    {
        (riderMetrix->CurTrickNumUbers)++;
        if (riderMetrix->Rider->mUberLevel >= 6)
            (riderMetrix->CurTrickNumSuperUbers)++;
    }
    else if (grabID >= 19)
    {
        (riderMetrix->CurTrickNumTweaks)++;
    }
    else
    {
        (riderMetrix->CurTrickNumGrabs)++;
    }
    float holdTime = riderMetrix->CurGrabHoldSec;
    riderMetrix->CurTrickMaxGrabHoldSec = holdTime >? riderMetrix->CurTrickMaxGrabHoldSec;
    riderMetrix->CurTrickIsUber = 0;
    riderMetrix->CurGrabHoldSec = -1.0f;
    riderMetrix->CurTrickTotalGrabHoldSec += holdTime;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (float absolute value).
static inline float metrixAbs_00119898(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `>?` (max) operator. Prototype mismatch: the unit declares
// `void* cRiderMetrix_evSpin(void*)` for an existing caller, but the body takes a float in $f12.
float cRiderMetrix_evSpin_impl(void* self, float x) __asm__("cRiderMetrix_evSpin");

/**
 * Scores the trick's spin: the spin angle so far is worth 0.1 trick power per full turn.
 *
 * Trick power (+0x14) loses |old spin angle (+0x34)| / (20 pi) and gains |@p x| / (20 pi), floored at 0; the spin angle
 * becomes @p x. The C definition is cRiderMetrix_evSpin (the unit also declares cRiderMetrix_evSpin with another prototype).
 * Field names are On Tour's cRiderMetrixState ones, which sit 8 bytes lower there.
 *
 * @param x  the trick's total spin so far, in radians (On Tour rad)
 * @return 0.0 (no boost change)
 */
float cRiderMetrix_evSpin_impl(void* vself, float x)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    self->CurTrickPower -= metrixAbs_00119898(self->CurTrickSpinAng) * 0.015915492549538612f;
    self->CurTrickSpinAng = x;
    self->CurTrickPower += metrixAbs_00119898(x) * 0.015915492549538612f;
    self->CurTrickPower = self->CurTrickPower >? 0.0f;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evFlip);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (float absolute value).
static inline float metrixAbs_001198D8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Scores the trick's flip: the flip angle so far is worth 0.18 trick power per full flip.
 *
 * Trick power (+0x14) loses |old flip angle (+0x38)| * 0.18 / (2 pi) and gains |@p x| * 0.18 / (2 pi), floored at 0;
 * the flip angle becomes @p x.
 *
 * @param x  the trick's total flip so far, in radians (On Tour rad)
 * @return 0.0 (no boost change)
 */
// PORT: g++ `>?` (max) operator.
extern "C" float cRiderMetrix_evFlip(cRiderMetrix* self, float x)
{
    self->CurTrickPower -= metrixAbs_001198D8(self->CurTrickFlipAng) * 0.028647884726524353f;
    self->CurTrickFlipAng = x;
    self->CurTrickPower += metrixAbs_001198D8(x) * 0.028647884726524353f;
    self->CurTrickPower = self->CurTrickPower >? 0.0f;
    return 0.0f;
}
#endif

extern "C" void* cRiderMetrix_evSpin(void*);

//99.29%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evRailSpin__FPvi);
#ifdef SKIP_ASM
/**
 * Scores a spin onto or off a rail: records the rail-slide mode, then scores the spin angle.
 *
 * Stores @p rsm as the current rail-slide mode (+0x20) and tail-calls cRiderMetrix_evSpin; the angle passes through
 * untouched in $f12, which the C prototype does not show.
 *
 * @param rsm  the rail-slide mode (On Tour rsm)
 * @return evSpin's result (0.0)
 */
void* cRiderMetrix_evRailSpin(void* vself, int rsm)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    self->CurTrickRailslideMode = rsm;
    return cRiderMetrix_evSpin(self);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evUberGrindStart__FPvii);
#ifdef SKIP_ASM
/**
 * Scores the start of an uber grind: records which grind and the rail mode, and marks the trick as uber.
 *
 * Zeroes the grind hold time (+0x6C), stores @p rsm as the initial and current rail-slide mode (+0xC, +0x20), @p grindID as
 * the uber grind (+0x70) and sets CurTrickIsUber (+0x5C).
 *
 * @param grindID  the uber grind (On Tour eTrickNameUberGrind grindID)
 * @param rsm  the rail-slide mode (On Tour rsm)
 * @return 0.0, read back from the hold time just zeroed
 */
float cRiderMetrix_evUberGrindStart(void* vself, int grindID, int rsm)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    self->CurTrickUberGrindHoldSec = 0.0f;
    self->InitTrickRailslideMode = rsm;
    self->CurTrickRailslideMode = rsm;
    self->CurTrickUberGrind = grindID;
    self->CurTrickIsUber = 1;
    return self->CurTrickUberGrindHoldSec;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evUberGrindEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores the end of an uber grind: earns the trick and starts a new one on the same rail.
 *
 * Increments the uber (+0x54) and uber-grind (+0x74) counts, calls cRiderMetrix_earnNameAndPoints with no flags,
 * resets the current trick and restores the rail distance (+0x24) and air time (+0x30) with @p rsm as the initial and
 * current rail-slide mode.
 *
 * @param rsm  the rail-slide mode to continue with (On Tour rsm)
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evUberGrindEnd(cRiderMetrix* self, int rsm)
{
    float curDist = self->CurTrickRailslideDist;
    float curAir = self->CurTrickAirSec;
    (self->CurTrickNumUbers)++;
    (self->CurTrickNumUberGrinds)++;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 0);
    cRiderMetrix_resetCurTrick(self);
    self->InitTrickRailslideMode = rsm;
    self->CurTrickRailslideMode = rsm;
    self->CurTrickRailslideDist = curDist;
    self->CurTrickAirSec = curAir;
    return powerUp;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evPressStart);
#ifdef SKIP_ASM
/**
 * Scores the start of a board press: records the press and keeps the combo alive.
 *
 * Stores @p pressMode as the initial (+0x10) and current (+0x28) press mode, starts the press timer (+0x2C) at 0 if idle and
 * suspends the combo timer (cRiderMetrix_cancelComboChainReset).
 *
 * @param pressMode  the press (On Tour pressMode)
 * @return 0.0 (no boost change)
 */
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" float cRiderMetrix_evPressStart(cRiderMetrix* self, int pressMode)
{
    self->InitTrickPressMode = pressMode;
    self->CurTrickPressMode = pressMode;
    self->CurTrickPressSec = self->CurTrickPressSec >? 0.0f;
    cRiderMetrix_cancelComboChainReset(self);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evPressEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores the end of a board press: earns the trick and starts a new one.
 *
 * Calls cRiderMetrix_earnNameAndPoints with the current rail-slide mode (+0x20) and an in-the-air flag (air time
 * +0x30 >= 0), then resets the current trick, restoring the rail mode (initial and current), rail distance (+0x24) and
 * air time.
 *
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evPressEnd(cRiderMetrix* self)
{
    float curDist = self->CurTrickRailslideDist;
    float airSec = self->CurTrickAirSec;
    int rsm = self->CurTrickRailslideMode;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, rsm, 0, airSec >= 0.0f);
    cRiderMetrix_resetCurTrick(self);
    self->InitTrickRailslideMode = rsm;
    self->CurTrickRailslideMode = rsm;
    self->CurTrickRailslideDist = curDist;
    self->CurTrickAirSec = airSec;
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evPressPivot);
#ifdef SKIP_ASM
// PORT: prototype mismatch (see cRiderMetrix_evSpin).
float cRiderMetrix_evSpin_impl(void* self, float x) __asm__("cRiderMetrix_evSpin");

/**
 * Scores a pivot during a board press: half a turn of extra spin.
 *
 * Stores @p pressMode as the current press mode (+0x28) and calls cRiderMetrix_evSpin with the spin angle (+0x34) + pi.
 *
 * @param pressMode  the press mode after the pivot (On Tour pressMode)
 * @return evSpin's result (0.0)
 */
extern "C" float cRiderMetrix_evPressPivot(cRiderMetrix* self, int pressMode)
{
    self->CurTrickPressMode = pressMode;
    return cRiderMetrix_evSpin_impl(self, self->CurTrickSpinAng + 3.1415927410125732f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evWipeout);
#ifdef SKIP_ASM
extern "C" int cRiderMetrix_losePoints(cRiderMetrix* self);
extern "C" int cRiderMetrix_getCurTrickPoints(cRiderMetrix* self);

/**
 * Scores a wipeout: counts it, loses the trick in progress and ends the combo.
 *
 * With @p flag (knocked down by another rider) Totals.NumDeaths (+0x12C) grows and HUD effect 0x2D shows; otherwise
 * Totals.NumWipeouts (+0x124) grows. cRiderMetrix_losePoints is added to Totals.TotalPointsLostToWipeouts (+0x1A0);
 * then the trick and combo are reset.
 *
 * @param flag  On Tour isFrag: the wipeout came from another rider's attack
 * @return -0.25 when a trick with points was lost, else 0.0
 */
extern "C" float cRiderMetrix_evWipeout(cRiderMetrix* self, int flag)
{
    if (flag)
    {
        (self->Totals.NumDeaths)++;
        cRiderMetrix_addHUDfx1(self, 45, 0, 0, 1.5f);
    }
    else
    {
        (self->Totals.NumWipeouts)++;
    }
    float powerLoss = 0.0f;
    self->Totals.TotalPointsLostToWipeouts += cRiderMetrix_losePoints(self);
    if (cRiderMetrix_getCurTrickPoints(self) > 0)
        powerLoss = -0.25f;
    cRiderMetrix_resetCurTrick(self);
    cRiderMetrix_resetComboChain(self);
    return powerLoss;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evWipeoutRecover);
#ifdef SKIP_ASM
/**
 * Scores getting up after a wipeout: with @p isUserRecover set shows HUD effect 33 for 1.5 s and returns a 0.1 boost.
 *
 * @param isUserRecover  set when the player recovered by input
 * @return 0.1, or 0.0 when @p isUserRecover is 0
 */
extern "C" float cRiderMetrix_evWipeoutRecover(cRiderMetrix* self, int isUserRecover)
{
    float result;
    if (isUserRecover)
    {
        cRiderMetrix_addHUDfx1(self, 33, 0, 0, 1.5f);
        result = 0.10000000149011612f;
    }
    else
        result = 0.0f;
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evHandplantStart);
#ifdef SKIP_ASM
/**
 * Scores the start of a handplant: records which one and adds its base power.
 *
 * Stores @p plant as the handplant (+0x7C), sets the air time (+0x30) to -1, zeroes the handplant hold time (+0x78), adds
 * 0.05 to the trick power (+0x14) and suspends the combo timer (cRiderMetrix_cancelComboChainReset).
 *
 * @param plant  the handplant (On Tour eTrickNameHandplant plant)
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evHandplantStart(cRiderMetrix* self, int plant)
{
    self->CurTrickHandplant = plant;
    self->CurTrickAirSec = -1.0f;
    *(int*)&self->CurTrickHandplantHoldSec = 0;
    self->CurTrickPower += 0.04999999701976776f;
    cRiderMetrix_cancelComboChainReset(self);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evHandplantEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff);
extern "C" void cRiderMetrix_resetCurTrick(cRiderMetrix* self);

/**
 * Scores the end of a handplant: earns the trick and starts a new one.
 *
 * Increments the handplant count (+0x80), calls cRiderMetrix_earnNameAndPoints with only the last flag set, resets
 * the current trick and zeroes the air time (+0x30).
 *
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evHandplantEnd(cRiderMetrix* self)
{
    self->CurTrickNumHandplants += 1;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 1);
    cRiderMetrix_resetCurTrick(self);
    *(int*)&self->CurTrickAirSec = 0;
    return powerUp;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evLateSpin);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_earnName(cRiderMetrix* self, int a, int b, int c, int d);

/**
 * Scores a late spin: names the trick so far and restarts scoring as a late-spin trick.
 *
 * Calls cRiderMetrix_earnName with no flags. If the trick had power (+0x14 > 0): the starting stance (+0x0) becomes
 * @p isFakieInit, spin (+0x34) and flip (+0x38) angles and the pipe, rail and press starts are cleared, the hold timer (+0x40) is
 * set to -1, the late-spin flag (+0x8) set and the 3 grab slots (+0x60) zeroed. The combo timer is suspended either
 * way.
 *
 * @param isFakieInit  On Tour isFakieInit: the late spin starts fakie
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evLateSpin(cRiderMetrix* self, int isFakieInit)
{
    cRiderMetrix_earnName(self, 0, 0, 0, 0);
    float powerUp = 0.0f;
    if (self->CurTrickPower > powerUp)
    {
        self->InitTrickIsFakie = isFakieInit;
        self->CurTrickSpinAng = powerUp;
        self->CurTrickFlipAng = powerUp;
        self->InitTrickIsPipe = 0;
        self->InitTrickRailslideMode = 0;
        self->InitTrickPressMode = 0;
        self->CurGrabHoldSec = -1.0f;
        self->InitTrickIsLateSpin = 1;
        MEM_fill((char*)&self->CurTrickGrabList, 0, 0xC);
    }
    cRiderMetrix_cancelComboChainReset(self);
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evLand);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_resetCurTrick(cRiderMetrix* self);
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores a landing: earns the trick and starts the next one from the landing state.
 *
 * Landing on a rail with trick power adds 0.13 power first. Calls cRiderMetrix_earnNameAndPoints with the landing
 * arguments, resets the trick and seeds the new one: stance (+0x0), pipe (+0x4), rail mode (+0xC, and +0x20 with rail
 * distance +0x24 = 0 when on a rail), press (+0x10, and +0x28 with press time +0x2C = 0 when pressing). An uber grind
 * in progress (+0x70) survives with the uber flag set and its hold time zeroed.
 *
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand), 0 = not on a rail
 * @param flag  the press landed in (On Tour pressLand), 0 = none
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evLand(cRiderMetrix* self, int stance, int alternate, int style, int flag)
{
    if (style != 0 && self->CurTrickPower > 0.0f)
    {
        self->CurTrickPower += 0.12999999523162842f;
    }
    int saved = self->CurTrickUberGrind;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, stance, alternate, style, flag, 0);
    cRiderMetrix_resetCurTrick(self);
    self->InitTrickIsFakie = stance;
    self->InitTrickIsPipe = alternate;
    self->InitTrickRailslideMode = style;
    self->InitTrickPressMode = flag;
    if (style != 0)
    {
        self->CurTrickRailslideMode = style;
        *(int*)&self->CurTrickRailslideDist = 0;
    }
    if (flag != 0)
    {
        self->CurTrickPressMode = flag;
        *(int*)&self->CurTrickPressSec = 0;
    }
    if (saved != 0)
    {
        self->CurTrickUberGrind = saved;
        self->CurTrickIsUber = 1;
        *(int*)&self->CurTrickUberGrindHoldSec = 0;
    }
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evTakeoff);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores a takeoff: earns the ground trick and starts the air trick.
 *
 * During an uber grind (+0x70 set) only the spin angle, rail distance (-1) and rail mode are cleared; otherwise
 * cRiderMetrix_earnNameAndPoints (earnNameAndPoints, last flag set) and cRiderMetrix_resetCurTrick run. The new trick starts from the
 * rail mode and press it left (+0xC, +0x10), the stance @p isFakieTakeoff (+0x0) and pipe flag @p isPipeTakeoff (+0x4), with air time (+0x30) 0.
 *
 * @param isFakieTakeoff  On Tour isFakieTakeoff: took off fakie
 * @param isPipeTakeoff  On Tour isPipeTakeoff: took off from a pipe
 * @return the boost earnNameAndPoints returns, or 0.0 during an uber grind
 */
extern "C" float cRiderMetrix_evTakeoff(cRiderMetrix* self, int isFakieTakeoff, int isPipeTakeoff)
{
    float powerUp = 0.0f;
    int curRailMode = self->CurTrickRailslideMode;
    int curPressMode = self->CurTrickPressMode;
    if (self->CurTrickUberGrind)
    {
        self->CurTrickSpinAng = powerUp;
        self->CurTrickRailslideDist = -1.0f;
        self->CurTrickRailslideMode = 0;
    }
    else
    {
        powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 1);
        cRiderMetrix_resetCurTrick(self);
    }
    self->InitTrickRailslideMode = curRailMode;
    self->InitTrickPressMode = curPressMode;
    self->InitTrickIsPipe = isPipeTakeoff;
    self->InitTrickIsFakie = isFakieTakeoff;
    *(int*)&self->CurTrickAirSec = 0;
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evAddCash);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001E3760(int id, int amount);
extern "C" void cBERewardInterface_awardUserCash(void* iface, int id, int amount);
int cRiderMetrix_addHUDfx1_i(void* self, int type, int value, int arg, float duration) __asm__("cRiderMetrix_addHUDfx1");

struct sVE119EF8 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int IsBusy119EF8()
{
    int n = *(int*)SSXApp->Game->Replay;
    return n != 0 && n < 10;
}

static inline int GetId119EF8(char* self)
{
    char* obj = *(char**)(self + 0x1AC) + 0x6C0;
    sVE119EF8* vt = *(sVE119EF8**)obj;
    return vt[7].fn(vthis(obj, vt[7]));
}

// PORT: callers in this unit declare it void; the function returns 0.0f.
float cRiderMetrix_evAddCash_impl(char* self, unsigned int kind, int amount) __asm__("cRiderMetrix_evAddCash");

/**
 * Awards career money to the rider and posts the matching HUD popups.
 *
 * Does nothing for 0, or while the replay is in a playback state (State 1 to 9). Otherwise it checks the
 * earnings unlocks (func_001E3760) and credits the money through the back end's reward interface
 * (cBERewardInterface_awardUserCash), both for the rider's AI index (being slot 7), then posts the total flash (0x18, 0.7 s)
 * and, by @p kind, popup 0x2F (0), 0x2E (1), 0x30 (2, data = the combo count at +0x9C) for 2.5 s or 0x31 (3,
 * data = the amount) for 1.5 s; kind 4 posts no popup.
 *
 * @param kind    the cash source (On Tour eRiderMetrixCashSource), 0 to 4
 * @param amount  money to award
 * @return always 0.0
 */
// Awards trick/score metrics of the given kind and plays the matching reward popups.
float cRiderMetrix_evAddCash_impl(char* vself, unsigned int kind, int amount)
{
    cRiderMetrix* self = (cRiderMetrix*)vself;
    if (amount != 0 && !IsBusy119EF8()) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
        func_001E3760(GetId119EF8(((char*)self)), amount);
        cBERewardInterface_awardUserCash(iface, GetId119EF8(((char*)self)), amount);
        cRiderMetrix_addHUDfx1_i(self, 0x18, amount, 0, 0.7f);
        switch (kind) {
        case 3:
            cRiderMetrix_addHUDfx1_i(self, 49, amount, amount, 1.5f);
            break;
        case 1:
            cRiderMetrix_addHUDfx1_i(self, 46, amount, 0, 2.5f);
            break;
        case 0:
            cRiderMetrix_addHUDfx1_i(self, 47, amount, 0, 2.5f);
            break;
        case 2:
            cRiderMetrix_addHUDfx1_i(self, 0x30, amount, self->NumCombos, 2.5f);
            break;
        case 4:
            break;
        }
    }
    return 0.0f;
}
#endif

extern "C" void cRiderMetrix_addHUDfx(void* self, int a, const char* b, float c, int d);
extern const char str_Wrong_Way[];

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evAutoResetSurface__FPv);
#ifdef SKIP_ASM
/**
 * Event for an automatic rider reset: posts the "Wrong Way!" HUD message (type 0x33, 1.5 s).
 *
 * @return always 0.0
 */
float cRiderMetrix_evAutoResetSurface(void* self)
{
    cRiderMetrix_addHUDfx(self, 51, str_Wrong_Way, 1.5f, 0);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evDangerLevelChange__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing and returns 0.0.
 *
 * cRider_updateDangerLevel calls it on mMetrix when the rider's danger level changes.
 *
 * @return 0.0
 */
float cRiderMetrix_evDangerLevelChange(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evDangerFail);
#ifdef SKIP_ASM
/**
 * Does nothing and returns 0.0. Nothing in the source calls it.
 *
 * @return 0.0
 */
extern "C" float cRiderMetrix_evDangerFail(void) {
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evMissionStart);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

/**
 * Shows HUD effect 26 for 1 second when a mission starts.
 *
 * @return 0.0
 */
extern "C" float cRiderMetrix_evMissionStart(cRiderMetrix* self)
{
    cRiderMetrix_addHUDfx1(self, 26, 0, 0, 1.0f);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evMissionSuccess);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_evAddCash(void* self, int kind, int amount);
/**
 * Scores a passed mission: shows the reward and adds it as cash.
 *
 * HUD effect 0x1B with @p amount for 1.5 s, then cRiderMetrix_evAddCash(4, @p amount) (On Tour cash source 4).
 *
 * @param amount  the cash reward (On Tour money)
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evMissionSuccess(cRiderMetrix* self, int amount)
{
    cRiderMetrix_addHUDfx1(self, 27, amount, 0, 1.5f);
    cRiderMetrix_evAddCash(self, 4, amount);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_earnName);
#ifdef SKIP_ASM
struct sTrickId;
extern "C" int cRiderMetrix_buildTrickName(cRiderMetrix* self, sTrickId* id, int stance, int alternate, int style, int flag);
extern "C" int cRiderMetrix_checkForMonsterTrick(cRiderMetrix* self, sTrickId* id);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_LandedMonsterCombo(void* audio, int rider);
extern "C" int cRiderMetrix_addToHistory(cRiderMetrix* self, sTrickId* id);
extern "C" void cRiderMetrix_addTrick(cRiderMetrix* self, sTrickId* id, int repeat);


/**
 * Names the trick so far without banking its points, paying any monster-trick bonus into the trick power.
 *
 * When cRiderMetrix_buildTrickName gives a name: a monster-trick bonus from cRiderMetrix_checkForMonsterTrick adds bonus / 10000 to the
 * trick power (+0x14), shows HUD effect 0x32 and plays the monster-combo sound; then cRiderMetrix_addTrick announces
 * it, as a repeat when cRiderMetrix_addToHistory finds it in the trick history.
 *
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand)
 * @param flag  the press landed in (On Tour pressLand)
 */
extern "C" void cRiderMetrix_earnName(cRiderMetrix* self, int stance, int alternate, int style, int flag)
{
    tTrickNameDef idv;
    sTrickId* id = (sTrickId*)&idv;
    if (cRiderMetrix_buildTrickName(self, id, stance, alternate, style, flag) != 0)
    {
        int pts = cRiderMetrix_checkForMonsterTrick(self, id);
        if (pts > 0)
        {
            self->CurTrickPower += pts * 9.999999747378752e-05f;
            cRiderMetrix_addHUDfx1(self, 50, pts, 0, 1.5f);
            cSSXAudio_LandedMonsterCombo(cAudioInterface_bxAudio(), (int)self->Rider);
        }
        cRiderMetrix_addTrick(self, id, cRiderMetrix_addToHistory(self, id) > 0);
    }
}
#endif

// 8-byte packed trick identity built by cRiderMetrix_buildTrickName
struct sTrickId {
    int w0;
    int w1;
};

struct sGameSettings {
    char pad_0x00[0x48];
    signed char eventKind; // 0x48: 4 = free ride
    signed char gameType;  // 0x49: 0 = Conquer the Mountain
};


extern sGameSettings cBENewRaceInterface_newRace;

extern "C" void cRiderMetrix_removeHUDfx(cRiderMetrix* self, int type);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cRiderMetrix_buildTrickName(cRiderMetrix* self, sTrickId* id, int stance, int alternate, int style, int flag);
extern "C" int cRiderMetrix_getCurTrickPoints(cRiderMetrix* self);
extern "C" int cRiderMetrix_checkForMonsterTrick(cRiderMetrix* self, sTrickId* id);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_LandedMonsterCombo(void* audio, int rider);
extern "C" int cRiderMetrix_addToHistory(cRiderMetrix* self, sTrickId* id);
extern "C" void cRiderMetrix_addTrick(cRiderMetrix* self, sTrickId* id, int repeat);
extern "C" int cRiderMetrix_getCurTrickPointsWithMult(cRiderMetrix* self);
extern "C" int cRiderMetrix_calcTrickLevel(cRiderMetrix* self, int points);
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(cRiderMetrix* self);
extern "C" void cRiderMetrix_addToComboChain(cRiderMetrix* self, int points);
extern "C" void cSSXAudio_LandedUberTrick(void* audio, int rider, int ubers, int runUbers);
extern "C" void cRiderMetrix_evAddCash(void* self, int kind, int amount);
extern "C" void cRiderMetrix_delayComboChainReset(cRiderMetrix* self, float seconds);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int abs(int);

// g++ max operator: the target uses max.s, which only `>?` produces
#define MAX(a, b) ((a) >? (b))

// Rounds whole degrees to the nearest multiple of 180.
static inline int func_roundDegrees(int deg)
{
    int r = deg % 180;
    if (r < -90) {
        return deg - r - 180;
    }
    if (r > 90) {
        return deg - r + 180;
    }
    return deg - r;
}

// Trick landing commit: names and scores the trick that just ended, updates
// the run statistics and the combo, and returns the meter delta.
//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_earnNameAndPoints);
#ifdef SKIP_ASM
/**
 * Ends the trick in progress: names it, banks its points and statistics into the run totals and shows the result.
 *
 * Clears the live trick HUD effects and does nothing more once the race is over (cAI_isRaceFinished). With trick
 * points: names the trick (cRiderMetrix_buildTrickName), adds any monster bonus, announces it (cRiderMetrix_addTrick) and divides points and
 * boost by 1 + its repeats in the trick history (cRiderMetrix_addToHistory). The result goes into Totals: TotalPoints (+0x198),
 * TotalStylePoints (+0x19C), NumTricksLanded (+0x110), the trick-level counts, best trick (+0x18C, with its name at
 * +0x190), spin and flip degrees, grab time, ubers and the air/grab/rail/plant records; the combo chain grows
 * (cRiderMetrix_addToComboChain). It then shows HUD effects 0x23 (0x24 for a repeat) and 0x18, or in free ride
 * (cBENewRaceInterface_newRace +0x48 == 4, +0x49 == 0) pays cash via cRiderMetrix_evAddCash(0, min(total / 500, 20)).
 * Unless a rail or press continues the line, the combo gets at least 1.5 s more (cRiderMetrix_delayComboChainReset).
 *
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand)
 * @param flag  the press landed in (On Tour pressLand)
 * @param takeoff  On Tour isTakeoff: called from a takeoff rather than a landing
 * @return the boost the trick earns (cRiderMetrix_getCurTrickPower divided by 1 + repeats), 0.0 without points
 */
extern "C" float cRiderMetrix_earnNameAndPoints(cRiderMetrix* self, int stance, int alternate, int style, int flag, int takeoff)
{
    sTrickId id;
    float delta;
    int valid;
    int repeats;
    int divisor;
    int pending;
    int grade;
    int inverted;
    int points;
    int total;
    int spin;
    int flip;
    int career;

    cRiderMetrix_removeHUDfx(self, 1);
    cRiderMetrix_removeHUDfx(self, 2);
    cRiderMetrix_removeHUDfx(self, 4);
    cRiderMetrix_removeHUDfx(self, 0x1c);
    cRiderMetrix_removeHUDfx(self, 29);
    cRiderMetrix_removeHUDfx(self, 0x1e);
    cRiderMetrix_removeHUDfx(self, 0x1f);
    cRiderMetrix_removeHUDfx(self, 0x20);

    if (cAI_isRaceFinished(SSXApp->Game->AI)) {
        return 0.0f;
    }

    valid = cRiderMetrix_buildTrickName(self, &id, stance, alternate, style, flag);
    delta = 0.0f;
    if (cRiderMetrix_getCurTrickPoints(self) > 0) {
        repeats = 0;
        if (valid) {
            int bonus = cRiderMetrix_checkForMonsterTrick(self, &id);
            if (bonus > 0) {
                self->CurTrickPower += (float)bonus * 0.0001f;
                cRiderMetrix_addHUDfx1(self, 50, bonus, 0, 1.5f);
                cSSXAudio_LandedMonsterCombo(cAudioInterface_bxAudio(), (int)self->Rider);
            }
            repeats = cRiderMetrix_addToHistory(self, &id);
            cRiderMetrix_addTrick(self, &id, repeats > 0);
        }
        divisor = repeats + 1;
        pending = cRiderMetrix_getCurTrickPoints(self);
        points = cRiderMetrix_getCurTrickPointsWithMult(self);
        grade = cRiderMetrix_calcTrickLevel(self, pending);
        inverted = cRiderMetrix_getCurTrickStyleSubPoints(self);
        cRiderMetrix_addToComboChain(self, pending / divisor);
        delta = cRiderMetrix_getCurTrickPower(self) / (float)divisor;

        points /= divisor;
        points -= points % 10;
        self->Totals.TotalPoints += points + inverted;
        self->Totals.TotalStylePoints += inverted;
        self->Totals.NumTricksLanded += 1;
        (self)->Totals.TricksAtLevel[grade]++;

        if (self->Totals.BestTrickPoints < pending) {
            self->Totals.BestTrickPoints = pending;
            *(sTrickId*)&self->Totals.BestTrick[0] = id;
        }

        spin = func_roundDegrees((int)(self->CurTrickSpinAng * 57.295776f));
        flip = func_roundDegrees((int)(self->CurTrickFlipAng * 57.295776f));
        self->Totals.SpinDegs += abs(spin);
        self->Totals.FlipDegs += abs(flip);
        self->Totals.GrabTime += self->CurTrickTotalGrabHoldSec;
        self->Totals.NumGrabs += self->CurTrickNumGrabs;
        self->Totals.NumHandplants += self->CurTrickNumHandplants;

        if (self->CurTrickNumUbers > 0) {
            cSSXAudio_LandedUberTrick(cAudioInterface_bxAudio(), (int)self->Rider, self->CurTrickNumUbers,
                          self->Totals.NumUberTricks);
        }

        self->Totals.NumUberTricks += self->CurTrickNumUbers;
        self->Totals.NumSuperUberTricks += self->CurTrickNumSuperUbers;
        self->Totals.NumUberGrindTricks += self->CurTrickNumUberGrinds;
        self->Totals.BiggestAirTime = MAX(self->CurTrickAirSec, *(float*)((char*)self + 332));
        self->Totals.BiggestGrabTime = MAX(self->CurTrickMaxGrabHoldSec, *(float*)((char*)self + 336));
        self->Totals.BiggestRailDist = MAX(self->CurTrickRailslideDist, *(float*)((char*)self + 340));
        self->Totals.BiggestHandplantTime = MAX(self->CurTrickHandplantHoldSec, *(float*)((char*)self + 344));
        self->Totals.TotalPoints += self->CurTrickBonusPoints;
        total = points + self->CurTrickBonusPoints + inverted;

        cBE_getInterface(cBE_getBE(), 0);
        // free ride in Conquer the Mountain pays the career instead of the HUD
        career = cBENewRaceInterface_newRace.eventKind == 4 && cBENewRaceInterface_newRace.gameType == 0;
        if (!career) {
            if (repeats > 0) {
                cRiderMetrix_addHUDfx1(self, 36, total, grade, 2.5f);
            } else {
                cRiderMetrix_addHUDfx1(self, 35, total, grade, 2.5f);
            }
            cRiderMetrix_addHUDfx1(self, 0x18, total, 0, 0.7f);
        } else {
            int amount = total / 500;
            if (amount > 20) {
                amount = 20;
            }
            cRiderMetrix_evAddCash(self, 0, amount);
        }
    }

    if (!takeoff) {
        if (style != 0 || flag != 0) {
            return delta;
        }
    } else if (self->CurTrickRailslideMode != 0 || self->CurTrickPressMode != 0) {
        return delta;
    }
    if (self->CurTrickPower > 0.0f || self->ComboResetDelay < 0.0f) {
        cRiderMetrix_delayComboChainReset(self, 1.5f);
    }
    return delta;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_losePoints);
#ifdef SKIP_ASM
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(cRiderMetrix* self);
extern "C" int cRiderMetrix_getCurTrickPoints(cRiderMetrix* self);
extern "C" void cRiderMetrix_removeHUDfx(cRiderMetrix* self, int type);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

/**
 * Shows the points a wipeout or reset throws away and returns the lost trick points.
 *
 * Trick points (cRiderMetrix_getCurTrickPoints) replace effect 1 with 'lost' effect 0x25; combo points (+0xA0) replace effect 3 with
 * 0x27; pending bonus (+0x84) plus style points (cRiderMetrix_getCurTrickStyleSubPoints) clear effects 2 and 0x1C-0x20 and show 0x28. Effect 4
 * is removed. Nothing is subtracted from the totals here.
 *
 * @return the trick points lost (cRiderMetrix_getCurTrickPoints)
 */
extern "C" int cRiderMetrix_losePoints(cRiderMetrix* self)
{
    int n = cRiderMetrix_getCurTrickPoints(self);
    if (n > 0)
    {
        cRiderMetrix_removeHUDfx(self, 1);
        cRiderMetrix_addHUDfx1(self, 37, n, 0, 1.5f);
    }
    if (self->ComboPoints > 0)
    {
        cRiderMetrix_removeHUDfx(self, 3);
        cRiderMetrix_addHUDfx1(self, 39, self->ComboPoints, 0, 1.5f);
    }
    int total = self->CurTrickBonusPoints + cRiderMetrix_getCurTrickStyleSubPoints(self);
    if (total > 0)
    {
        cRiderMetrix_removeHUDfx(self, 2);
        cRiderMetrix_removeHUDfx(self, 29);
        cRiderMetrix_removeHUDfx(self, 0x1C);
        cRiderMetrix_removeHUDfx(self, 0x1E);
        cRiderMetrix_removeHUDfx(self, 0x1F);
        cRiderMetrix_removeHUDfx(self, 0x20);
        cRiderMetrix_addHUDfx1(self, 40, total, 0, 1.5f);
    }
    cRiderMetrix_removeHUDfx(self, 4);
    return n;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_buildTrickName);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);
extern unsigned char gKickerSpecial[][11];
extern unsigned char gWallSpecial[][11];

// Bit layout of the 8-byte trick identity (same fields cRiderMetrix_checkForMonsterTrick compares).
struct sTrickBits_A8C8 {
    unsigned int k0 : 3;    // w0 bits 0-2
    unsigned int k1 : 4;    // w0 bits 3-6
    unsigned int k2 : 3;    // w0 bits 7-9
    unsigned int a : 2;     // w0 bits 10-11
    unsigned int b : 4;     // w0 bits 12-15
    unsigned int c : 3;     // w0 bits 16-18
    unsigned int d : 3;     // w0 bits 19-21
    unsigned int tbl : 6;   // w0 bits 22-27
    unsigned int e : 4;     // w0 bits 28-31
    unsigned int m0 : 3;    // w1 bits 0-2
    unsigned int f : 7;     // w1 bits 3-9
    unsigned int h : 1;     // w1 bit 10
    unsigned int g : 7;     // w1 bits 11-17
    unsigned int n : 2;     // w1 bits 18-19
    unsigned int p : 4;     // w1 bits 20-23
    unsigned int q : 3;     // w1 bits 24-26
    unsigned int grade : 5; // w1 bits 27-31
};


/**
 * Builds the name record of the trick in progress from its spins, flips, grabs and landing.
 *
 * Zeroes the 8-byte name (On Tour tTrickNameDef) and packs its bit fields from the trick state: uber grind and rail
 * mode, takeoff and landing stance and pipe flags, spin and flip counts rounded from the angles (+0x34, +0x38), up to 3
 * grabs (+0x60), handplant (+0x7C) and press, using the tables gKickerSpecial / gWallSpecial.
 *
 * @param vself  the score object (this)
 * @param vid  receives the trick name (On Tour tTrickNameDef &tn)
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand)
 * @param flag  the press landed in (On Tour pressLand)
 * @return nonzero when the trick has a name
 */
extern "C" int cRiderMetrix_buildTrickName(cRiderMetrix* vself, sTrickId* vid, int stance, int alternate, int style, int flag)
{
    cRiderMetrix* self = vself;
    sTrickBits_A8C8* id = (sTrickBits_A8C8*)vid;
    int spinDeg;
    int flipDeg;
    int flips;
    int spins;
    int cnt;
    int n;
    int idx;
    int fidx;

    MEM_fill(id, 0, 8);
    if (self->CurTrickUberGrind != 0) {
        id->k0 = self->InitTrickRailslideMode;
        id->m0 = self->CurTrickUberGrind;
        if (style != 0) {
            id->q = 3;
        }
        return 1;
    }
    if (self->InitTrickIsFakie != 0) {
        self->CurTrickSpinAng = -self->CurTrickSpinAng;
    }
    if (self->CurTrickRailslideMode == 4) {
        self->CurTrickFlipAng = -self->CurTrickFlipAng;
    }
    spinDeg = func_roundDegrees((int)(self->CurTrickSpinAng * 57.2957763671875f));
    flipDeg = func_roundDegrees((int)(self->CurTrickFlipAng * 57.2957763671875f));
    flips = abs(flipDeg / 360);
    spins = abs(spinDeg / 180);
    if (self->CurTrickPressMode != 0) {
        int nameCode;
        if (spins >= 2) {
            if (spins < 12) {
                if (spins >= 6) nameCode = 14; else nameCode = 13;
            } else {
                nameCode = 15;
            }
            id->p = nameCode;
        } else if (spins == 1 && self->CurTrickPressSec >= 3.0f) {
            if (self->CurTrickPressMode == spins) {
                if (self->CurTrickPressSec < 12.0f) {
                    if (self->CurTrickPressSec < 9.0f) {
                        if (self->CurTrickPressSec >= 6.0f) nameCode = 7; else nameCode = 5;
                    } else nameCode = 9;
                } else nameCode = 11;
            } else {
                if (self->CurTrickPressSec < 12.0f) {
                    if (self->CurTrickPressSec < 9.0f) {
                        if (self->CurTrickPressSec >= 6.0f) nameCode = 8; else nameCode = 6;
                    } else nameCode = 10;
                } else nameCode = 12;
            }
            id->p = nameCode;
        } else {
            if (self->CurTrickPressMode == 1) {
                id->p = self->CurTrickRailslideMode ? 3 : 1;
            } else {
                id->p = self->CurTrickRailslideMode == 0 ? 2 : 4;
            }
            id->k1 = spins < 11 ? spins : 10;
        }
        return 1;
    }
    if (self->CurTrickRailslideMode != 0) {
        if (self->CurTrickRailslideDist <= 50.0f) {
            id->k0 = 5;
        } else {
            id->k0 = self->CurTrickRailslideMode;
            id->k1 = spins < 11 ? spins : 10;
            if (self->Rider->IsGoofyStance != 0) {
                if (self->CurTrickRailslideMode == 1) {
                    id->k0 = 2;
                } else if (self->CurTrickRailslideMode == 2) {
                    id->k0 = 1;
                }
            }
        }
        return 1;
    }
    idx = spins;
    fidx = flips;
    cnt = 0;
    while (flips > 0 && spins >= 3) {
        flips--;
        spins -= 3;
        cnt++;
    }
    flips = fidx;
    spins = idx;
    if (spinDeg > 0 || spins >= 15) {
        idx = spins + 14;
    }
    if (flipDeg < 0 || flips >= 6) {
        fidx = flips + 5;
    }
    for (n = 0; n < 3 && self->CurTrickGrabList[n] != 0; n++) {
    }
    if (self->InitTrickPressMode == 1) {
        id->k2 = 5;
    } else if (self->InitTrickPressMode == 2) {
        id->k2 = 6;
    } else if (self->InitTrickRailslideMode != 0) {
        if (self->InitTrickIsFakie != 0) {
            id->k2 = 3;
        } else {
            id->k2 = 2;
        }
    } else if (self->InitTrickIsLateSpin != 0) {
        id->k2 = 4;
    } else if (self->InitTrickIsFakie != 0) {
        id->k2 = 1;
    }
    if (idx < 29 && fidx < 11) {
        if (self->InitTrickIsPipe != 0 && alternate != 0) {
            id->tbl = gWallSpecial[idx][fidx];
        } else {
            id->tbl = gKickerSpecial[idx][fidx];
        }
    } else {
        MEM_fill(id, 0, 8);
        id->tbl = 1;
        return 1;
    }
    if (id->tbl == 0) {
        if (spins > 0) {
            if (spinDeg > 0) {
                id->a = 2;
            } else {
                id->a = 1;
            }
        }
        if (flips == 0) {
            id->b = spins;
        }
        id->c = flips;
        if (flips > 0) {
            if (cnt > 0 && cnt == flips) {
                if (self->CurTrickFlipAng > 0.0f) {
                    id->d = 4;
                } else {
                    id->d = 3;
                }
            } else {
                if (self->CurTrickFlipAng > 0.0f) {
                    id->d = 1;
                } else {
                    id->d = 2;
                }
            }
        }
    }
    if (flips > 0 && spins != flips * 3 && spins < 15) {
        id->e = spins;
    }
    if (flag == 1) {
        id->q = 4;
    } else if (flag == 2) {
        id->q = 5;
    } else if (style != 0) {
        id->q = 3;
    } else if (id->tbl == 0 && (cnt == 0 || id->e != 0)) {
        if (stance != 0 && self->InitTrickIsFakie == 0) {
            id->q = 2;
        } else if (spins == 0 && flips == 0 && n > 0) {
            id->q = 1;
        }
    }
    if (n > 0) {
        id->f = self->CurTrickGrabList[0];
        if (n >= 2) {
            id->h = 1;
            if (n == 2) {
                id->g = self->CurTrickGrabList[1];
            } else {
                id->g = 1;
            }
        }
    }
    if (self->CurTrickHandplant != 0) {
        id->n = self->CurTrickHandplant;
    }
    return flips > 0 || spins > 0 || n > 0 || self->CurTrickHandplant != 0;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_checkForMonsterTrick);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

extern tMonsterTrick gMonsterComboTable[];

struct sTrickBits_11B1A8
{
    unsigned int pad0 : 10;
    unsigned int a : 2;     // bits 10-11
    unsigned int b : 4;     // bits 12-15
    unsigned int c : 3;     // bits 16-18
    unsigned int d : 3;     // bits 19-21
    unsigned int pad1 : 6;
    unsigned int e : 4;     // bits 28-31
    unsigned int pad2 : 3;
    unsigned int f : 7;     // bits 3-9
    unsigned int pad3 : 1;
    unsigned int g : 7;     // bits 11-17
    unsigned int pad4 : 9;
    unsigned int grade : 5; // bits 27-31
};

/**
 * Checks whether the trick is one of the named monster tricks and, if so, renames it and returns its bonus.
 *
 * Compares 7 bit fields of @p id with each of the 24 gMonsterComboTable entries; the first match clears the name and
 * keeps only that entry's grade, which names the monster trick.
 *
 * @param id  the trick name (On Tour tTrickNameDef &tn); replaced on a match
 * @return the monster trick's bonus points, or 0
 */
extern "C" int cRiderMetrix_checkForMonsterTrick(cRiderMetrix* self, sTrickId* id)
{
    sTrickBits_11B1A8* trick = (sTrickBits_11B1A8*)id;
    tMonsterTrick* comboEntry = gMonsterComboTable;
    for (int i = 0; i < 24; i++, comboEntry++)
    {
        if (trick->a != comboEntry->reqOrient) continue;
        if (trick->b != comboEntry->reqSpin) continue;
        if (trick->c != comboEntry->reqReps) continue;
        if (trick->d != comboEntry->reqFlip) continue;
        if (trick->e != comboEntry->reqPostSpin) continue;
        if (trick->f != comboEntry->reqGrab) continue;
        if (trick->g != comboEntry->reqLateGrab) continue;
        MEM_fill(id, 0, 8);
        trick->grade = comboEntry->id;
        return comboEntry->points;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", InitTerminalVelocityTable);
#ifdef SKIP_ASM
extern float kTerminalVelocityTable[2][3][4][2];

/**
 * Fills the terminal-velocity table used by cRider_updateTerminalVelocity.
 *
 * Each kTerminalVelocityTable[a][b][c][d] entry is 120 minus penalties: 5 when a == 0, 20/11/0 for b = 0/1/2,
 * 20/15/10/0 for c = 0..3 and 5 when d == 0 (cRider_updateTerminalVelocity indexes it as [speed attribute][surface][boost
 * level][tuck]).
 */
extern "C" void InitTerminalVelocityTable(void)
{
    for (int speedAttrib = 0; speedAttrib < 2; speedAttrib++) {
        for (int surface = 0; surface < 3; surface++) {
            for (int boostLevel = 0; boostLevel < 4; boostLevel++) {
                for (int tuck = 0; tuck < 2; tuck++) {
                    int va = speedAttrib ? 0 : 5;
                    int vb = surface ? (surface == 1 ? 11 : 0) : 20;
                    int vc = boostLevel ? (boostLevel == 1 ? 15 : (boostLevel == 2 ? 10 : 0)) : 20;
                    int vd = tuck ? 0 : 5;
                    kTerminalVelocityTable[speedAttrib][surface][boostLevel][tuck] = 120.0f - (float)(va + vb + vc + vd);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRider_updateTerminalVelocity);
#ifdef SKIP_ASM
extern "C" float cBEStatInterface_getSpeedAttrib(void* iface, int a1, int a2);
int cRider_getMotionState(void* self);
extern float kTerminalVelocityTable[2][3][4][2];

/**
 * Recomputes the rider's top speed from snow type, tuck, boost level and the rider's speed stat.
 *
 * Interpolates kTerminalVelocityTable (filled by InitTerminalVelocityTable) by surface property and tuck amount for each
 * speed-attribute row, mixes the rows by the rider's speed attribute (cBEStatInterface_getSpeedAttrib on the stat interface), scales by
 * 27.78 and smooths it into the old value (+0x2E4) with 0.9 (0.97 when slowing), losing 2% when off natural stance
 * without boost. In motion state 1 the limit is 3333.33. At uber level 11+ while boosting it never drops below +0x30C.
 */
// cRider::updateTerminalVelocity (On Tour name; pairing unverified, structure fits the retail asm)
extern "C" void cRider_updateTerminalVelocity(char* self)
{
    float speedAttribute = cBEStatInterface_getSpeedAttrib(cBE_getInterface(cBE_getBE(), 3), *(int*)(self + 0x86C), *(int*)(self + 0xB34));
    // NOTE: the retail code adds and subtracts 1.0 (likely an inlined bx range helper).
    speedAttribute = speedAttribute + 1.0f - 1.0f;
    cGame* surfPropCache = SSXApp->Game;
    char* snow = (char*)surfPropCache->SurfPropCache + *(int*)(self + 0x438) * 176;
    const int motionState = cRider_getMotionState(self);
    const float surfaceProperty = (motionState == 0) ? *(float*)(snow + 4) : 1.0f;
    const bool fasterThanPacked = 0.0f < surfaceProperty;
    const int lowSurfaceIndex = fasterThanPacked ? 1 : 0;
    const float surfaceFactor = fasterThanPacked ? surfaceProperty : surfaceProperty + 1.0f;
    float boostStrength = *(float*)(self + 0x2FC);
    int boostFactor = 0;
    if (0.1f <= boostStrength) {
        boostFactor = 1;
        if (0.4f <= boostStrength) {
            boostFactor = 3;
            if (boostStrength < 0.8f) {
                boostFactor = 2;
            }
        }
    }
    const float crouchFactor = (motionState == 0) ? *(float*)(self + 0x220) : 1.0f;
    float speeds[2];
    float newTerminalVelocity;
    float historyFactor;
    int i;
    for (i = 0; i < 2; i++) {
        const float lowTucLowSurface = kTerminalVelocityTable[i][lowSurfaceIndex][boostFactor][0];
        const float hiTucLowSurface = kTerminalVelocityTable[i][lowSurfaceIndex][boostFactor][1];
        const float lowTucHiSurface = kTerminalVelocityTable[i][lowSurfaceIndex + 1][boostFactor][0];
        const float hiTucHiSurface = kTerminalVelocityTable[i][lowSurfaceIndex + 1][boostFactor][1];
        const float lowTucLowSurfaceArea = (1.0f - surfaceFactor) * (1.0f - crouchFactor);
        const float lowTucHiSurfaceArea = surfaceFactor * (1.0f - crouchFactor);
        const float hiTucLowSurfaceArea = (1.0f - surfaceFactor) * crouchFactor;
        const float hiTucHiSurfaceArea = surfaceFactor * crouchFactor;
        speeds[i] = lowTucLowSurfaceArea * lowTucLowSurface + lowTucHiSurfaceArea * lowTucHiSurface
                  + hiTucLowSurfaceArea * hiTucLowSurface + hiTucHiSurfaceArea * hiTucHiSurface;
    }
    if (motionState == 1) {
        newTerminalVelocity = 3333.33349609375f;
    } else {
        newTerminalVelocity = (speedAttribute * speeds[1] + (1.0f - speedAttribute) * speeds[0]) * 27.77777862548828f;
        *(int*)(self + 0x5A0) = 0;
        historyFactor = 0.8999999761581421f;
        if (*(int*)(self + 0x320) != *(int*)(self + 0x324)) {
            if (*(float*)(self + 0x2FC) <= 0.0f) {
                newTerminalVelocity *= 0.9800000190734863f;
            }
        }
        if (newTerminalVelocity < *(float*)(self + 0x2E4)) {
            historyFactor = 0.9700000286102295f;
        }
        newTerminalVelocity = (1.0f - historyFactor) * newTerminalVelocity + historyFactor * *(float*)(self + 0x2E4);
    }
    *(float*)(self + 0x2E4) = newTerminalVelocity;
    if (*(int*)(self + 0x2F4) >= 11 && 0.0f < *(float*)(self + 0x2FC)) {
        *(float*)(self + 0x2E4) = MAX(*(float*)(self + 780), newTerminalVelocity);
    }
}
#endif

extern "C" void* strcasecmp(void* self);

//100%
INCLUDE_ASM("ai/ridermetrix", func_0011B678__FPv);
#ifdef SKIP_ASM
/**
 * Compares two load-list names without case: forwards to strcasecmp.
 *
 * The sort comparator func_0011BD60 and func_0011BE88 pass to qsort; the second string arrives in a1.
 *
 * @return strcasecmp's result
 */
void* func_0011B678(void* self)
{
    return strcasecmp(self);
}
#endif

