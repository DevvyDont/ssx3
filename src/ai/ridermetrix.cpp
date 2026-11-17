#include "common.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_linkToRider);
#ifdef SKIP_ASM
extern "C" void func_00116FA8(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_cHUDFX[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sMetrixEntry_001172E0
{
    char pad_0x00[0x98];
    int index;
    sMetrixEntry_001172E0() { func_00116FA8(this); }
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
extern "C" void cRiderMetrix_linkToRider(char* self, char* rider)
{
    *(char**)(self + 0x1AC) = rider;
    if (*(int*)(rider + 0x870) >= 0)
    {
        sMetrixEntry_001172E0** slot = (sMetrixEntry_001172E0**)(self + 0x1B0);
        *(int*)(self + 0x1B4) = 44;
        *slot = new (operator_new_tag(44 * sizeof(sMetrixEntry_001172E0), str_cHUDFX, 0, 0)) sMetrixEntry_001172E0[44];
        for (int i = 0; i < *(int*)(self + 0x1B4); i++)
        {
            ((sMetrixEntry_001172E0*)(i * 0x9C + *(int*)(self + 0x1B0)))->index = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetHUDfx);
#ifdef SKIP_ASM
struct sRiderMetrixEntry_001173B8
{
    int state;
    char pad[0x98];
};

struct sRiderMetrix_001173B8
{
    char pad[0x1B0];
    sRiderMetrixEntry_001173B8* entries;
    int count;
};

/**
 * Frees every slot of the HUD message bank (type 0x34), when the score object has a bank.
 */
extern "C" void cRiderMetrix_resetHUDfx(sRiderMetrix_001173B8* self)
{
    if (self->entries != 0)
    {
        for (int i = 0; i < self->count; i++)
        {
            self->entries[i].state = 52;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_hasActiveHUDfx);
#ifdef SKIP_ASM
struct sRiderMetrixEntry_00117400
{
    int type;
    char pad[0x98];
};

struct sRiderMetrix_00117400
{
    char pad[0x1B0];
    sRiderMetrixEntry_00117400* entries;
    int count;
    int bits[2];
    int dirty;
};

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
extern "C" int cRiderMetrix_hasActiveHUDfx(sRiderMetrix_00117400* self, int type)
{
    if (self->entries == 0)
        return 0;
    if (self->dirty)
    {
        for (int i = 0; i < self->count; i++)
        {
            int entryType = self->entries[i].type;
            if (entryType != 52)
            {
                int off = (entryType & ~0x1F) >> 3;
                int* bitArray = self->bits;
                *(int*)((char*)bitArray + off) |= 1 << entryType;
            }
        }
        self->dirty = 0;
    }
    return rmBitTest00117400(self->bits, type);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001174B0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the rider metrix's first 428 bytes (0x1AC) into a replay frame through @p obj's virtual slot 1.
 *
 * cRiderHandler_writeToReplayFrame calls it for the rider's mMetrix.
 *
 * @param obj  the replay frame
 */
extern "C" void cRiderMetrix_writeToReplayFrame(void* self, void* obj)
{
    sVEntry001174B0* vt = *(sVEntry001174B0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 428);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry001174E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the rider metrix's first 428 bytes (0x1AC) back from a replay frame through @p obj's virtual slot 2.
 *
 * cRiderHandler_readFromReplayFrame calls it for the rider's mMetrix.
 *
 * @param obj  the replay frame
 */
extern "C" void cRiderMetrix_readFromReplayFrame(void* self, void* obj)
{
    sVEntry001174E8* vt = *(sVEntry001174E8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 428);
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
extern "C" void cRiderMetrix_setInitialPointsTotal(void* self, int pointsTotal)
{
    int delta = pointsTotal - *(int*)((char*)self + 0x1c8);
    *(int*)((char*)self + 0x1c8) = pointsTotal;
    *(int*)((char*)self + 0x198) += delta;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_reset);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_resetTotals(char* self);
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void cRiderMetrix_resetCurTrick(void* self);
extern "C" void cRiderMetrix_resetComboChain(void* self);

/**
 * Resets the score object for a new run: totals, HUD messages, trick history, current trick and combo.
 *
 * Calls cRiderMetrix_resetTotals() and cRiderMetrix_resetHUDfx(), clears the repeat history (+0xA8, 0x50
 * bytes) and its cursor (+0xF8), calls cRiderMetrix_resetCurTrick() and cRiderMetrix_resetComboChain(), then
 * zeroes the smoothed speed (+0x1A8), suspends the combo clock (+0xA4 = -1), clears the renderer bits at
 * +0x1B8 / +0x1BC and sets +0x1C0 to 1.
 */
extern "C" void cRiderMetrix_reset(char* self)
{
    cRiderMetrix_resetTotals(self);
    cRiderMetrix_resetHUDfx((sRiderMetrix_001173B8*)self);
    MEM_fill(self + 0xA8, 0, 80);
    *(int*)(self + 0xF8) = 0;
    cRiderMetrix_resetCurTrick(self);
    cRiderMetrix_resetComboChain(self);
    *(int*)(self + 0x1A8) = 0;
    *(float*)(self + 0xA4) = -1.0f;
    MEM_fill(self + 0x1B8, 0, 8);
    *(int*)(self + 0x1C0) = 1;
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
extern "C" void cRiderMetrix_resetTotals(char* self)
{
    MEM_fill(self + 0xFC, 0, 172);
    *(int*)(self + 0x18C) = -1;
    *(int*)(self + 0x198) = *(int*)(self + 0x1C8);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_resetComboChain);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_removeHUDfx(void* self, int type);

/**
 * Ends the rider's trick combo chain and removes its HUD effect.
 *
 * Removes HUD effect 3 (the combo display that cRiderMetrix_addToComboChain() shows), zeroes the combo count (+0x9C) and combo
 * score (+0xA0), and sets the chain timer (+0xA4) back to -1.
 */
extern "C" void cRiderMetrix_resetComboChain(void* self)
{
    cRiderMetrix_removeHUDfx(self, 3);
    *(int*)((char*)self + 0x9C) = 0;
    *(int*)((char*)self + 0xA0) = 0;
    *(float*)((char*)self + 0xA4) = -1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_addToComboChain);
#ifdef SKIP_ASM
extern "C" void func_001171A8(void* self, int type, int value, int arg, float f);

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
 * (func_001171A8 on bank + 0x1D4) with its current value (0 when that slot's type is 52) and the new count.
 *
 * @param points  the points earned
 */
// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" void cRiderMetrix_addToComboChain(void* self, int points)
{
    char* metrix = (char*)self;
    if (points > 0)
    {
        int n = *(int*)(metrix + 0x9C) + 1;
        *(int*)(metrix + 0x9C) = n;
        float rawMult = ((float)n + 10.0f) * 0.05000000074505806f;
        float k = rmClamp_117638(rawMult, 0.5f, 2.0f);
        *(float*)(metrix + 0xA4) = -1.0f;
        *(int*)(metrix + 0xA0) += (int)(points * k);
        char* hudBank = *(char**)(metrix + 0x1B0);
        if (hudBank != 0)
        {
            int curValue = 0;
            char* hudSlot = hudBank + 0x1D4;
            if (!rmIsState_117638(hudSlot, 52))
                curValue = *(int*)(hudBank + 0x1E8);
            func_001171A8(hudSlot, 3, curValue, n, 0.0f);
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
extern "C" void cRiderMetrix_cancelComboChainReset(void* self)
{
    *(float*)((char*)self + 0xa4) = -1.0f;
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
extern "C" void cRiderMetrix_delayComboChainReset(void* self, float seconds)
{
    *(float*)((char*)self + 0xa4) = *(float*)((char*)self + 0xa4) >? seconds;
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
extern "C" void cRiderMetrix_earnComboChain(void* self_)
{
    char* self = (char*)self_;
    int n = *(int*)(self + 0x9C);
    if (n >= 2) {
        int pts = *(int*)(self + 0xA0);
        if (pts > 0) {
            *(int*)(self + 0x198) += pts;
            *(int*)(self + 0x134) += n;
            if (*(int*)(self + 0x188) < pts)
                *(int*)(self + 0x188) = pts;
            if (*(int*)(self + 0x9C) > *(int*)(self + 0x184))
                *(int*)(self + 0x184) = *(int*)(self + 0x9C);
            cBE_getInterface_Fv(cBE_getBE(), 0);
            int offline = 0;
            if (D_00535BC8_117718.mode == 4)
                offline = D_00535BC8_117718.online == 0;
            if (!offline) {
                cRiderMetrix_addHUDfx1_f(self, 38, *(int*)(self + 0xA0), 1.5f, *(int*)(self + 0x9C));
                cRiderMetrix_addHUDfx1_f(self, 0x18, *(int*)(self + 0xA0), 0.7f, 0);
            } else {
                int cashAmount = *(int*)(self + 0xA0) / 500;
                cRiderMetrix_evAddCash(self, 2, cashAmount > 20 ? 20 : cashAmount);
            }
        }
    }
    cRiderMetrix_resetComboChain(self);
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
extern "C" void cRiderMetrix_resetCurTrick(void* riderMetrix)
{
    char* self = (char*)riderMetrix;
    *(int*)(self + 0x14) = 0;
    *(float*)(self + 0x18) = 1.0f;
    *(int*)(self + 0x1C) = 0;
    *(int*)(self + 0x0) = 0;
    *(int*)(self + 0x4) = 0;
    *(int*)(self + 0x8) = 0;
    *(int*)(self + 0xC) = 0;
    *(int*)(self + 0x10) = 0;
    *(int*)(self + 0x20) = 0;
    *(float*)(self + 0x24) = -1.0f;
    *(int*)(self + 0x28) = 0;
    *(float*)(self + 0x2C) = -1.0f;
    *(float*)(self + 0x30) = -1.0f;
    *(int*)(self + 0x34) = 0;
    *(int*)(self + 0x38) = 0;
    *(int*)(self + 0x3C) = 0;
    *(float*)(self + 0x40) = -1.0f;
    *(int*)(self + 0x44) = 0;
    *(int*)(self + 0x48) = 0;
    *(int*)(self + 0x4C) = 0;
    *(int*)(self + 0x50) = 0;
    *(int*)(self + 0x54) = 0;
    *(int*)(self + 0x58) = 0;
    *(int*)(self + 0x5C) = 0;
    *(int*)(self + 0x70) = 0;
    *(float*)(self + 0x6C) = -1.0f;
    *(int*)(self + 0x74) = 0;
    *(float*)(self + 0x78) = -1.0f;
    *(int*)(self + 0x7C) = 0;
    *(int*)(self + 0x80) = 0;
    *(int*)(self + 0x84) = 0;
    *(int*)(self + 0x88) = 0;
    *(int*)(self + 0x8C) = 0;
    *(int*)(self + 0x90) = 0;
    *(int*)(self + 0x94) = 0;
    *(int*)(self + 0x98) = 0;
    MEM_fill(self + 0x60, 0, 0xC);
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
float cRiderMetrix_getCurTrickPower(void* self)
{
    return *(float*)((char*)self + 0x14);
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
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(void* self)
{
    int trickPoints = (int)(*(float*)((char*)self + 0x1C) * 10000.0f + 5.0f);
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
extern "C" int cRiderMetrix_getCurTrickPoints(void* self)
{
    int trickPoints = (int)(*(float*)((char*)self + 0x1C4) * *(float*)((char*)self + 0x14) * 10000.0f + 5.0f);
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
extern "C" int cRiderMetrix_getCurTrickPointsWithMult(void* self)
{
    int trickPoints = (int)(*(float*)((char*)self + 0x1C4) * *(float*)((char*)self + 0x18) * *(float*)((char*)self + 0x14) * 10000.0f + 5.0f);
    return trickPoints - trickPoints % 10;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_removeHUDfx);
#ifdef SKIP_ASM
struct sMetrixSlot {
    int type;
    char pad_0x04[0x98];
};

struct sMetrixSlots {
    char pad_0x00[0x1B0];
    sMetrixSlot* slots;
    int count;
};

/**
 * Removes the rider's HUD effects of one type by marking their slots free (type 0x34).
 *
 * A type below 0x22 has its own fixed slot; any other type is searched for in the slots from 0x23 up to the
 * slot count. Does nothing when the slot array (+0x1B0) is not allocated.
 *
 * @param self_  the rider metrics (this)
 * @param type   the HUD effect type (On Tour eHUDFXID)
 */
extern "C" void cRiderMetrix_removeHUDfx(void* self_, int type)
{
    sMetrixSlots* self = (sMetrixSlots*)self_;
    if (self->slots != 0)
    {
        if (type < 34)
        {
            self->slots[type].type = 52;
        }
        else
        {
            for (int i = 35; i < self->count; i++)
            {
                if (self->slots[i].type == type)
                {
                    self->slots[i].type = 52;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_allocHUDfx);
#ifdef SKIP_ASM
struct sMetrixSlotR {
    int type;
    float total;
    float value;
    char pad_0x0C[0x90];
};

struct sMetrixSlotsR {
    char pad_0x00[0x1B0];
    sMetrixSlotR* slots;
    int count;
};

static inline bool slotIsFreeR(const sMetrixSlotR& s)
{
    return s.type == 0x34;
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
extern "C" int cRiderMetrix_allocHUDfx(void* self_, int type)
{
    sMetrixSlotsR* self = (sMetrixSlotsR*)self_;
    if (type < 34)
    {
        return type;
    }
    float best = 1.0f;
    int bestIdx = -1;
    for (int i = 35; i < self->count; i++)
    {
        if (slotIsFreeR(self->slots[i]))
        {
            return i;
        }
        float ratio = self->slots[i].value / self->slots[i].total;
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
int cRiderMetrix_addHUDfx_impl(void* self, int hfx, const char* displayStr, float decayTime, int data)
{
    if (*(sMetrixSlotAE8**)((char*)self + 0x1B0) == 0)
        return -1;
    int idx = cRiderMetrix_allocHUDfx(self, hfx);
    cHUDFX_set(&(*(sMetrixSlotAE8**)((char*)self + 0x1B0))[idx], hfx, displayStr, decayTime, data);
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
int cRiderMetrix_addHUDfx1_impl(void* self, int type, int value, int data, float duration)
{
    if (*(sMetrixSlotB88**)((char*)self + 0x1B0) == 0)
        return -1;
    int idx = cRiderMetrix_allocHUDfx(self, type);
    cHUDFX_set1(&(*(sMetrixSlotB88**)((char*)self + 0x1B0))[idx], type, value, duration, data);
    return idx;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_update);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_earnComboChain(void*);
extern "C" void* func_0028B180();
struct sGradeStep_119210;
extern "C" int cRiderMetrix_updateBonusLevelAndCalcPoints(void* self, int* idx, sGradeStep_119210* tbl, int type, float x);
extern "C" void cHUDFX_update(void* slot);
extern "C" void cRiderMetrix_updateStaticHUD(void* self);
extern "C" void cSSXAudio_SetCurTrickPower(void* audio, void* rider, float mult);
extern "C" void* func_00416210(void* dst, int c, int n);
int cRider_getMotionState(void* rider);
int cRider_getControlState(void* rider);

extern sGradeStep_119210 D_00459E00[];
extern sGradeStep_119210 D_00459E20[];
extern sGradeStep_119210 D_00459F68[];
extern sGradeStep_119210 D_00459E40[];
extern sGradeStep_119210 D_00459F08[];

struct sVec17C28 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRider17C28 {
    char pad_0x000[0x1C0];
    sVec17C28 up;        // 0x1C0
    char pad_0x1D0[0x10];
    sVec17C28 vel;       // 0x1E0
    char pad_0x1F0[0x110];
    float timeScale;     // 0x300
};

struct sMetrixSlot17C28 {
    char pad_0x00[0x9C];
};

struct sMetrix17C28 {
    char pad_0x00[0x14];
    float mult;          // 0x14
    char pad_0x18[0x4];
    float f1C;           // 0x1C
    char pad_0x20[0x4];
    float airTime;       // 0x24
    char pad_0x28[0x4];
    float t2C;           // 0x2C
    float t30;           // 0x30
    char pad_0x34[0x8];
    float add3C;         // 0x3C
    float t40;           // 0x40
    char pad_0x44[0x28];
    float t6C;           // 0x6C
    char pad_0x70[0x8];
    float t78;           // 0x78
    char pad_0x7C[0x8];
    int points;          // 0x84
    int idx88;           // 0x88
    int idx8C;           // 0x8C
    int idx90;           // 0x90
    int idx94;           // 0x94
    int idx98;           // 0x98
    char pad_0x9C[0x8];
    float tA4;           // 0xA4
    char pad_0xA8[0xB4];
    float maxSpeed;      // 0x15C
    float maxAvg;        // 0x160
    float speedSum;      // 0x164
    float frames;        // 0x168
    char pad_0x16C[0x3C];
    float avgSpeed;      // 0x1A8
    sRider17C28* rider;  // 0x1AC
    sMetrixSlot17C28* slots; // 0x1B0
    int count;           // 0x1B4
    char clear[8];       // 0x1B8
    int dirty;           // 0x1C0
};

// PORT: PS2-only VU0 inline asm (vector length).
static inline float len17C28(const sVec17C28& v)
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
extern "C" void cRiderMetrix_update(sMetrix17C28* self)
{
    float dt = self->rider->timeScale * 0.016666668f;
    float speed = len17C28(self->rider->vel);
    if (self->tA4 >= 0.0f) {
        if (cRider_getMotionState(self->rider) == 0 && cRider_getControlState(self->rider) != 1) {
            self->tA4 = self->tA4 - dt;
            if (self->tA4 <= 0.0f) {
                cRiderMetrix_earnComboChain(self);
            }
        }
    }
    if (self->t30 >= 0.0f) {
        self->t30 += dt;
        self->points += cRiderMetrix_updateBonusLevelAndCalcPoints(self, &self->idx88, D_00459E00, 0x1C, self->t30);
    }
    if (self->t40 >= 0.0f) {
        self->t40 += dt;
        self->mult += self->add3C;
        self->points += cRiderMetrix_updateBonusLevelAndCalcPoints(self, &self->idx8C, D_00459E20, 0x20, self->t40);
    }
    if (self->airTime >= 0.0f) {
        float stepDist = speed * dt;
        self->airTime += stepDist;
        self->mult += stepDist * 5e-05f;
        sVec17C28 up = self->rider->up;
        if (up.z < 0.0f) {
            self->f1C += stepDist * 0.0002f;
        }
        self->points += cRiderMetrix_updateBonusLevelAndCalcPoints(self, &self->idx98, D_00459F68, 29, self->airTime);
    }
    if (self->t2C >= 0.0f) {
        self->t2C += dt;
        self->mult += dt * 0.01f;
        self->points += cRiderMetrix_updateBonusLevelAndCalcPoints(self, &self->idx90, D_00459E40, 0x1E, self->t2C);
    }
    if (self->t6C >= 0.0f) {
        self->t6C += dt;
        self->mult += dt * 0.04999999701976776f;
    }
    if (self->t78 >= 0.0f) {
        self->t78 += dt;
        self->mult += dt * 0.04999999701976776f;
        self->points += cRiderMetrix_updateBonusLevelAndCalcPoints(self, &self->idx94, D_00459F08, 0x1F, self->t78);
    }
    self->avgSpeed = self->avgSpeed * 0.9f + speed * 0.1f;
    self->speedSum += speed;
    self->frames += 1.0f;
    if (self->maxAvg < self->avgSpeed) {
        self->maxAvg = self->avgSpeed;
    }
    if (self->maxSpeed < speed) {
        self->maxSpeed = speed;
    }
    cRiderMetrix_updateStaticHUD(self);
    if (self->slots != 0) {
        for (int i = 0; i < self->count; i++) {
            cHUDFX_update(&self->slots[i]);
        }
    }
    func_00416210(self->clear, 0, 8);
    self->dirty = 1;
    cSSXAudio_SetCurTrickPower(func_0028B180(), self->rider, self->mult);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_updateStaticHUD);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addHUDfx(void* self, int a, const char* b, float c, int d);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" int cRiderMetrix_calcTrickLevel(void* self, int points);
int cRider_getControlState(void* rider);
extern "C" int AITick();
extern "C" float func_0031C040(float a);
extern "C" float cPrewindControl_getFlipSpinInputAng(void* p, int* fail);
extern "C" float cSpinControl_getFlipSpinInputAng(void* p, int* fail);
extern "C" float cSpinControl_getFlipSpinAng(void* p, int* fail);
extern "C" float cSpinControl_getInvertScale(void* p);
extern "C" float cSpinControl_getOffAxisScale(void* p);
extern "C" int func_00150960(void* iface, int a);
extern "C" void cSSXAudio_PointsEvent(void* audio, void* rider, int points);
extern "C" void cSSXAudio_StartUberSpin(void* audio, void* rider);
extern "C" void cSSXAudio_EndUberSpin(void* audio, void* rider);
extern "C" void func_00299638(void* audio, void* rider);
extern "C" void cSSXAudio_LoseMaxPowerEvent(void* audio, void* rider);
extern char str_STALLED[];
extern char str_OFF_AXIS[];
extern char str_INVERTED[];
extern char str_LATE_SPIN[];
extern char str_LATE_FLIP[];

struct sSlot_117FE0 {
    int type;               // 0x0
    float total;            // 0x4
    float value;            // 0x8
    int fC;                 // 0xC
    int f10;                // 0x10
    int f14;                // 0x14
    char pad[0x9C - 0x18];
};

struct sRiderSub_117FE0 {
    char pad[0x2C0];
    int state;              // 0x2C0
    char pad2C4[0x330 - 0x2C4];
    float f330;             // 0x330
};

struct sVE_117FE0 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sRider_117FE0 {
    char pad[0x2F0];
    float f2F0;             // 0x2F0
    int f2F4;               // 0x2F4
    float f2F8;             // 0x2F8
    char pad2FC[8];
    int f304;               // 0x304
    char pad308[0x6C0 - 0x308];
    char obj6C0[0x77C - 0x6C0]; // 0x6C0: embedded object, vtable pointer first
    sRiderSub_117FE0* sub;  // 0x77C
};

struct sMetrix_117FE0 {
    char pad[0x14];
    float f14;              // 0x14
    char pad18[4];
    float f1C;              // 0x1C
    char pad20[0x34];
    int f54;                // 0x54
    char pad58[4];
    int f5C;                // 0x5C
    char pad60[0x24];
    int f84;                // 0x84
    char pad88[0x14];
    int f9C;                // 0x9C
    int fA0;                // 0xA0
    float fA4;              // 0xA4
    char padA8[0xF0];
    int f198;               // 0x198
    char pad19C[0x10];
    sRider_117FE0* rider;   // 0x1AC
    sSlot_117FE0* slots;    // 0x1B0
};

static inline bool slotFree_117FE0(const sSlot_117FE0* s)
{
    return s->type == 0x34;
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

static inline bool subStateIs_117FE0(sRiderSub_117FE0* s, int st)
{
    return s->state == st;
}

// PORT: abs.s helper.
static inline float abs_117FE0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Same function as func_001171A8(slot, type, value, arg, f): the float travels in $f12 either way.
extern "C" void func_001171A8(void* self, int type, int value, int arg, float f);
extern "C" void func_001171A8_f(void* slot, int type, int value, float f, int arg) __asm__("func_001171A8");

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
extern "C" void cRiderMetrix_updateStaticHUD(void* vself)
{
    sMetrix_117FE0* self = (sMetrix_117FE0*)vself;
    if (self->slots == 0)
        return;
    int pts = cRiderMetrix_getCurTrickPoints(self);
    if (pts > 0) {
        cRiderMetrix_addHUDfx1(self, 1, pts, cRiderMetrix_calcTrickLevel(self, pts), -1.0f);
        cSSXAudio_PointsEvent(func_0028B180(), self->rider, pts);
    }
    if (cRider_getControlState(self->rider) == 8 && !subStateIs_117FE0(self->rider->sub, 3))
        func_001171A8_f(&self->slots[11], 11, 0, self->rider->sub->f330 <? 1.0f, 0);
    else
        cRiderMetrix_removeHUDfx(self, 11);
    cRiderMetrix_removeHUDfx(self, 0x10);
    cRiderMetrix_removeHUDfx(self, 17);
    cRiderMetrix_removeHUDfx(self, 18);
    cRiderMetrix_removeHUDfx(self, 19);
    cRiderMetrix_removeHUDfx(self, 20);
    cRiderMetrix_removeHUDfx(self, 21);
    cRiderMetrix_removeHUDfx(self, 22);
    cRiderMetrix_removeHUDfx(self, 23);
    if (cRider_getControlState(self->rider) != 2 && cRider_getControlState(self->rider) != 5) {
        cRiderMetrix_removeHUDfx(self, 0xC);
        cRiderMetrix_removeHUDfx(self, 13);
        cRiderMetrix_removeHUDfx(self, 0xF);
        cRiderMetrix_removeHUDfx(self, 0xE);
    } else {
        int fail = 0;
        float angle;
        if (cRider_getControlState(self->rider) == 2)
            angle = cPrewindControl_getFlipSpinInputAng((char*)self->rider->sub + 0x1F0, &fail);
        else
            angle = cSpinControl_getFlipSpinInputAng((char*)self->rider->sub + 0x230, &fail);
        if (fail != 0) {
            cRiderMetrix_removeHUDfx(self, 13);
        } else {
            float dialPct = angle * 0.15915493667125702f + func_0031C040((float)AITick() * 0.10471976548433304f) * 0.029999999329447746f;
            if (dialPct < 0.0f)
                dialPct += 1.0f;
            if (!slotFree_117FE0(&self->slots[13])) {
                float cur = self->slots[13].value / self->slots[13].total;
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
            func_001171A8_f(&self->slots[13], 13, 0, dialPct, 0);
        }
        if (cRider_getControlState(self->rider) == 2) {
            if (fail != 0)
                cRiderMetrix_removeHUDfx(self, 0xC);
            else
                cRiderMetrix_addHUDfx1(self, 0xC, 0, 0, -1.0f);
        } else {
            int fail2 = 0;
            float yaw = cSpinControl_getFlipSpinAng((char*)self->rider->sub + 0x230, &fail2);
            if (fail2 != 0) {
                cRiderMetrix_removeHUDfx(self, 0xC);
                cRiderMetrix_removeHUDfx(self, 13);
                cRiderMetrix_removeHUDfx(self, 0xF);
                cRiderMetrix_removeHUDfx(self, 0xE);
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
                float w1 = cSpinControl_getInvertScale((char*)self->rider->sub + 0x230);
                float w2 = cSpinControl_getOffAxisScale((char*)self->rider->sub + 0x230);
                func_001171A8_f(&self->slots[main], main, 0, 1.0f, 0);
                if (invertZone1 != 52)
                    func_001171A8_f(&self->slots[invertZone1], invertZone1, 0, w1, 1);
                if (s2 != 52)
                    func_001171A8_f(&self->slots[s2], s2, 0, w1, 1);
                if (offAxisZone1 != 52)
                    func_001171A8_f(&self->slots[offAxisZone1], offAxisZone1, 0, w2, 2);
                if (offAxisZone2 != 52)
                    func_001171A8_f(&self->slots[offAxisZone2], offAxisZone2, 0, w2, 2);
                int prev = 0;
                if (!slotFree_117FE0(&self->slots[14]))
                    prev = self->slots[14].fC;
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
                        cRiderMetrix_removeHUDfx(self, 0xF);
                        cRiderMetrix_removeHUDfx(self, 0xE);
                    } else {
                        cRiderMetrix_addHUDfx1(self, 0xF, (int)styleMult, 0, -1.0f);
                        self->f1C += styleMult * 0.0008333333535119891f;
                        if (kind != prev) {
                            int x = cRiderMetrix_getCurTrickStyleSubPoints(self);
                            int dispPoints = self->f84 + x;
                            func_001171A8_f(&self->slots[2], 2, dispPoints, 0.0f, x);
                        }
                    }
                }
            }
        }
    }
    float potentialPct;
    if (slotFree_117FE0(&self->slots[5])) {
        potentialPct = self->rider->f2F8;
    } else {
        float actualLevel = self->rider->f2F8;
        float target;
        if (self->rider->f304 != 3)
            target = actualLevel + self->f14;
        else
            target = actualLevel;
        potentialPct = approach_117FE0(target <? 1.0f, self->slots[5].value / self->slots[5].total, 0.01666666753590107f);
    }
    float actualPct;
    if (slotFree_117FE0(&self->slots[6]))
        actualPct = self->rider->f2F8;
    else
        actualPct = approach_117FE0(self->rider->f2F8, self->slots[6].value / self->slots[6].total, 0.01666666753590107f);
    func_001171A8_f(&self->slots[5], 5, 0, potentialPct, 0);
    func_001171A8_f(&self->slots[6], 6, 0, actualPct, 0);
    if (self->rider->f2F4 == 0) {
        cRiderMetrix_removeHUDfx(self, 8);
    } else {
        if (slotFree_117FE0(&self->slots[8]))
            func_001171A8_f(&self->slots[8], 8, 0, 0.0f, 0);
        int n = self->rider->f2F4;
        if (n >= 11)
            n = 10;
        int lvl = n - 1;
        float cur = self->slots[8].value / self->slots[8].total;
        float curPct = approach_117FE0((float)lvl * 0.1111111119389534f <? 1.0f, cur, 0.0066666672937572f);
        func_001171A8(&self->slots[8], 8, 0, lvl, curPct);
    }
    int uberCount = self->rider->f2F4;
    if (uberCount >= 11)
        uberCount = 10;
    int left = 10 - uberCount;
    if ((self->f5C != 0 || self->f54 != 0) && left > 0) {
        int k = self->f54;
        if (self->f5C != 0)
            k++;
        if (left < k)
            k = left;
        if (slotFree_117FE0(&self->slots[10])) {
            func_001171A8_f(&self->slots[10], 10, 0, 0.0f, 0);
            cSSXAudio_StartUberSpin(func_0028B180(), self->rider);
        }
        float cur = self->slots[10].value / self->slots[10].total;
        float curPct = approach_117FE0((float)k * 0.1111111119389534f <? 1.0f, cur, 0.01666666753590107f);
        func_001171A8_f(&self->slots[10], 10, 0, curPct, k);
    } else {
        if (!slotFree_117FE0(&self->slots[10]))
            cSSXAudio_EndUberSpin(func_0028B180(), self->rider);
        cRiderMetrix_removeHUDfx(self, 10);
    }
    if (slotFree_117FE0(&self->slots[9])) {
        if (self->rider->f2F0 > 0.0f) {
            func_001171A8_f(&self->slots[9], 9, 0, 0.0f, 0);
            func_00299638(func_0028B180(), self->rider);
        }
    } else {
        float cur = self->slots[9].value / self->slots[9].total;
        if (cur == 1.0f && self->rider->f2F0 == 0.0f) {
            cRiderMetrix_removeHUDfx(self, 9);
            cSSXAudio_LoseMaxPowerEvent(func_0028B180(), self->rider);
        } else {
            func_001171A8_f(&self->slots[9], 9, 0, approach_117FE0(1.0f - self->rider->f2F0 * 0.05000000074505806f, cur, 0.01666666753590107f), self->rider->f2F4 > 11 ? 11 : self->rider->f2F4);
        }
    }
    func_001171A8_f(&self->slots[7], 7, self->f198, 0.0f, 0);
    sSlot_117FE0* comboSlot = &self->slots[3];
    if (!slotFree_117FE0(comboSlot)) {
        int want = self->fA0;
        int have = self->slots[3].f14;
        float cur = self->slots[3].value / self->slots[3].total;
        int dispPoints;
        if (want + 70 < have)
            dispPoints = have - 70;
        else if (have < want - 70)
            dispPoints = have + 70;
        else
            dispPoints = want;
        func_001171A8_f(comboSlot, 3, dispPoints, approach_117FE0(1.0f, cur, 0.01666666753590107f), self->f9C);
        if (self->fA4 < 0.0f)
            self->slots[3].f10 = 4;
        else
            self->slots[3].f10 = (int)(self->fA4 * 3.266666889190674f);
    }
    int x = cRiderMetrix_getCurTrickStyleSubPoints(self);
    sSlot_117FE0* s2 = &self->slots[2];
    int val = self->f84 + x;
    if (!slotFree_117FE0(s2)) {
        float curPct;
        if (self->slots[2].f14 - self->slots[2].fC == self->f84)
            curPct = approach_117FE0(1.0f, self->slots[2].value / self->slots[2].total, 0.01666666753590107f);
        else
            curPct = 0.0f;
        func_001171A8_f(s2, 2, val, curPct, x);
    } else if (val > 0) {
        func_001171A8_f(s2, 2, val, 0.0f, x);
    }
    void* stats = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* obj = (char*)self->rider + 0x6C0;
    sVE_117FE0* vt = *(sVE_117FE0**)obj;
    int moneyTotal = func_00150960(stats, vt[7].fn(obj + vt[7].delta));
    func_001171A8_f(&self->slots[25], 25, moneyTotal, 0.0f, 0);
    if (!slotFree_117FE0(&self->slots[4])) {
        int have = self->slots[4].f14;
        float cur = self->slots[4].value / self->slots[4].total;
        func_001171A8_f(&self->slots[4], 4, have, approach_117FE0(1.0f, cur, 0.01666666753590107f), 0);
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
extern "C" void cRiderMetrix_addTrick(void* self, sTrickId* id, int repeat)
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
struct sMetrixRecent {
    char pad_0x00[0x60];
    int recent[3];
};

/**
 * Adds a grab to the current trick's three-entry grab list (+0x60).
 *
 * The grab goes into the first empty entry; when it equals the last entry already filled, it is not added again.
 *
 * @param self_  the cRiderMetrix (this)
 * @param value  the grab id
 */
extern "C" void cRiderMetrix_addGrab(void* self_, int value)
{
    sMetrixRecent* self = (sMetrixRecent*)self_;
    for (int i = 0; i < 3; i++)
    {
        if (self->recent[i] == 0)
        {
            self->recent[i] = value;
            break;
        }
        if (self->recent[i] == value && i < 2 && self->recent[i + 1] == 0)
        {
            self->recent[i] = value;
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

struct sTrickHist190F0 {
    char pad0[0x8];
    int u8;                       // 0x8
    char padC[0x20 - 0xC];
    int u20;                      // 0x20
    int u24;
    int u28;                      // 0x28
    char pad2C[0x70 - 0x2C];
    int u70;                      // 0x70
    char pad74[0x7C - 0x74];
    int u7C;                      // 0x7C
    char pad80[0xA8 - 0x80];
    sTrickId190F0 hist[10];       // 0xA8
    int histIdx;                  // 0xF8
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
extern "C" int cRiderMetrix_addToHistory(void* self_, sTrickId* id_)
{
    sTrickHist190F0* self = (sTrickHist190F0*)self_;
    int count;

    if (self->u8 != 0 || self->u20 != 0 || self->u70 != 0 || self->u7C != 0 || self->u28 != 0) {
        return 0;
    }
    count = 0;
    if ((((sTrickId190F0*)id_)->w1 & 0x3F800) != 0x800 && (((sTrickId190F0*)id_)->w0 & 0xFC00000) != 0x400000) {
        for (int i = 0; i < 10; i++) {
            if (func_trickIdEqual(id_, &self->hist[i], 8)) {
                count++;
            }
        }
    }
    self->hist[self->histIdx] = *(sTrickId190F0*)id_;
    self->histIdx++;
    self->histIdx %= 10;
    return count;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_updateBonusLevelAndCalcPoints);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_removeHUDfx(void* self, int type);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);

struct sGradeStep_119210
{
    float threshold;
    float mult;
};

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
extern "C" int cRiderMetrix_updateBonusLevelAndCalcPoints(void* self, int* idx, sGradeStep_119210* tbl, int type, float x)
{
    // PORT: pointer arithmetic through int.
    sGradeStep_119210* bonusStep = (sGradeStep_119210*)(*idx * 8 + (int)tbl);
    float stepThreshold = bonusStep->threshold;
    if (stepThreshold < 0.0f) return 0;
    if (stepThreshold <= x)
    {
        float pointsMult = *(float*)((char*)self + 0x1C4);
        int pointsEarned = (int)(bonusStep->mult * pointsMult + 0.5f);
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
extern "C" int cRiderMetrix_calcTrickLevel(void* self, int points)
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
extern "C" int cRiderMetrix_losePoints(void* self);

/**
 * Scores a reset: the trick in progress is lost, the combo ends and a boost penalty is returned.
 *
 * Adds cRiderMetrix_losePoints (losePoints) to Totals.TotalPointsLostToResets (+0x1A4), resets the current trick and combo chain
 * and increments Totals.NumResets (+0x120).
 *
 * @param isUserReset  On Tour isUserReset: the player asked for the reset
 * @return the boost change: -0.7 x the rider's unk_2F8 for a user reset, else -0.1 x it
 */
extern "C" float cRiderMetrix_evReset(void* self, int isUserReset)
{
    *(int*)((char*)self + 0x1A4) += cRiderMetrix_losePoints(self);
    cRiderMetrix_resetCurTrick(self);
    cRiderMetrix_resetComboChain(self);
    *(int*)((char*)self + 0x120) += 1;
    if (!isUserReset)
        return *(float*)(*(char**)((char*)self + 0x1AC) + 0x2F8) * -0.10000000149011612f;
    return *(float*)(*(char**)((char*)self + 0x1AC) + 0x2F8) * -0.699999988079071f;
}
#endif

extern "C" void cRiderMetrix_earnComboChain(void*);

//99.38% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evFinishRace);
#ifdef SKIP_ASM
/**
 * Scores crossing the finish line: banks the current combo.
 *
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evFinishRace(void* self)
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
extern "C" float cRiderMetrix_evFrag(char* self)
{
    *(int*)(self + 0x128) += 1;
    cRiderMetrix_addHUDfx1(self, 44, 0, 0, 1.5f);
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evCollectMultiplier);
#ifdef SKIP_ASM
extern "C" void func_001171A8(void* self, int type, int value, int arg, float f);

/**
 * Records a collected multiplier and shows it when it beats the best so far.
 *
 * Increments the multiplier count (+0x130); with a HUD bank (+0x1B0) and @p value above the stored best (+0x18) it
 * shows HUD effect 4 with (int) @p value (func_001171A8 on bank + 0x270) and stores the new best.
 *
 * @param value  the multiplier
 * @return 0.0 (no boost)
 */
extern "C" float cRiderMetrix_evCollectMultiplier(char* self, float value)
{
    *(int*)(self + 0x130) += 1;
    char* hudBank = *(char**)(self + 0x1B0);
    if (hudBank != 0)
    {
        if (*(float*)(self + 0x18) < value)
        {
            func_001171A8(hudBank + 0x270, 4, (int)value, 0, 0.0f);
            *(float*)(self + 0x18) = value;
        }
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evCheckpoint);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" int cGameModeMan_riderCheckpointHit(void* mgr, int id, int a2);
extern "C" void func_002A3CE8(void* snd, void* rider, int a2);

struct sVEntry001194C0 { short delta; short index; int (*fn)(void*); };

static inline bool rmRiderFlags_1194C0(char* r)
{
    return *(int*)(r + 0x874) != 0 && *(int*)(r + 0x87C) != 0;
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
extern "C" float cRiderMetrix_evCheckpoint(char* self, int timeBonusSec)
{
    char* sub = *(char**)(self + 0x1AC) + 0x6C0;
    sVEntry001194C0* vt = *(sVEntry001194C0**)sub;
    if (vt[8].fn(sub + vt[8].delta) != 0)
    {
        void* mgr = SSXApp->GameModeMan;
        char* sub2 = *(char**)(self + 0x1AC) + 0x6C0;
        sVEntry001194C0* vt2 = *(sVEntry001194C0**)sub2;
        if (cGameModeMan_riderCheckpointHit(mgr, vt2[7].fn(sub2 + vt2[7].delta), timeBonusSec) != 0)
        {
            cRiderMetrix_addHUDfx1(self, 41, timeBonusSec, 0, 2.5f);
            if (rmRiderFlags_1194C0(*(char**)(self + 0x1AC)))
            {
                func_002A3CE8(func_0028B180(), *(void**)(self + 0x1AC), 1);
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
 * Called by func_0023B5F8 (game mode) with a time difference in seconds.
 *
 * @param value  the value to show
 * @return 0.0
 */
extern "C" float cRiderMetrix_evSplitTime(void* self, int value)
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
 * Called by func_0023C560 (game mode) with a points difference.
 *
 * @param value  the value to show
 * @return 0.0
 */
extern "C" float cRiderMetrix_evSplitPoints(void* self, int value)
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
extern "C" float cRiderMetrix_evAddPoints(void* self, int pts)
{
    int points = (int)(pts * *(float*)((char*)self + 0x1C4) + 0.5f);
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
    *(int*)((char*)self + 0x198) += points;
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
extern "C" float cRiderMetrix_evGrabHold(char* self, int id)
{
    float zero = 0.0f;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 6);
    float perGrabPower = cBEBAGTInterface_getGrabPoints(iface, id) * 9.999999747378752e-05f;
    float perGrabTickPower = cBEBAGTInterface_getGrabPointsPerSec(iface, id) * 1.6666666624587378e-06f;
    *(float*)(self + 0x14) += perGrabPower;
    *(float*)(self + 0x3C) = perGrabTickPower;
    if (*(float*)(self + 0x40) < zero)
    {
        *(float*)(self + 0x40) = zero;
    }
    if (id >= 35)
    {
        *(int*)(self + 0x5C) = 1;
    }
    cRiderMetrix_cancelComboChainReset(self);
    return zero;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evReleaseGrab);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_addGrab(void* self_, int value);

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
extern "C" float cRiderMetrix_evReleaseGrab(void* riderMetrix, int grabID)
{
    char* self = (char*)riderMetrix;
    cRiderMetrix_addGrab(self, grabID);
    if (grabID >= 35)
    {
        (*(int*)(self + 0x54))++;
        if (*(int*)(*(char**)(self + 0x1AC) + 0x2F4) >= 6)
            (*(int*)(self + 0x58))++;
    }
    else if (grabID >= 19)
    {
        (*(int*)(self + 0x50))++;
    }
    else
    {
        (*(int*)(self + 0x4C))++;
    }
    float holdTime = *(float*)(self + 0x40);
    *(float*)(self + 0x48) = holdTime >? *(float*)(self + 0x48);
    *(int*)(self + 0x5C) = 0;
    *(float*)(self + 0x40) = -1.0f;
    *(float*)(self + 0x44) += holdTime;
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
float cRiderMetrix_evSpin_impl(void* self, float x)
{
    *(float*)((char*)self + 0x14) -= metrixAbs_00119898(*(float*)((char*)self + 0x34)) * 0.015915492549538612f;
    *(float*)((char*)self + 0x34) = x;
    *(float*)((char*)self + 0x14) += metrixAbs_00119898(x) * 0.015915492549538612f;
    *(float*)((char*)self + 0x14) = *(float*)((char*)self + 0x14) >? 0.0f;
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
extern "C" float cRiderMetrix_evFlip(void* self, float x)
{
    *(float*)((char*)self + 0x14) -= metrixAbs_001198D8(*(float*)((char*)self + 0x38)) * 0.028647884726524353f;
    *(float*)((char*)self + 0x38) = x;
    *(float*)((char*)self + 0x14) += metrixAbs_001198D8(x) * 0.028647884726524353f;
    *(float*)((char*)self + 0x14) = *(float*)((char*)self + 0x14) >? 0.0f;
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
 * Stores @p rsm as the current rail-slide mode (+0x20) and tail-calls cRiderMetrix_evSpin (evSpin); the angle passes through
 * untouched in $f12, which the C prototype does not show.
 *
 * @param rsm  the rail-slide mode (On Tour rsm)
 * @return evSpin's result (0.0)
 */
void* cRiderMetrix_evRailSpin(void* self, int rsm)
{
    *(int*)((char*)self + 0x20) = rsm;
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
float cRiderMetrix_evUberGrindStart(void* self, int grindID, int rsm)
{
    *(float*)((char*)self + 0x6c) = 0.0f;
    *(int*)((char*)self + 0xc) = rsm;
    *(int*)((char*)self + 0x20) = rsm;
    *(int*)((char*)self + 0x70) = grindID;
    *(int*)((char*)self + 0x5c) = 1;
    return *(float*)((char*)self + 0x6c);
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evUberGrindEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores the end of an uber grind: earns the trick and starts a new one on the same rail.
 *
 * Increments the uber (+0x54) and uber-grind (+0x74) counts, calls cRiderMetrix_earnNameAndPoints (earnNameAndPoints) with no flags,
 * resets the current trick and restores the rail distance (+0x24) and air time (+0x30) with @p rsm as the initial and
 * current rail-slide mode.
 *
 * @param rsm  the rail-slide mode to continue with (On Tour rsm)
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evUberGrindEnd(void* self, int rsm)
{
    float curDist = *(float*)((char*)self + 0x24);
    float curAir = *(float*)((char*)self + 0x30);
    (*(int*)((char*)self + 0x54))++;
    (*(int*)((char*)self + 0x74))++;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 0);
    cRiderMetrix_resetCurTrick(self);
    *(int*)((char*)self + 0xC) = rsm;
    *(int*)((char*)self + 0x20) = rsm;
    *(float*)((char*)self + 0x24) = curDist;
    *(float*)((char*)self + 0x30) = curAir;
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
extern "C" float cRiderMetrix_evPressStart(char* self, int pressMode)
{
    *(int*)(self + 0x10) = pressMode;
    *(int*)(self + 0x28) = pressMode;
    *(float*)(self + 0x2C) = *(float*)(self + 0x2C) >? 0.0f;
    cRiderMetrix_cancelComboChainReset(self);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evPressEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores the end of a board press: earns the trick and starts a new one.
 *
 * Calls cRiderMetrix_earnNameAndPoints (earnNameAndPoints) with the current rail-slide mode (+0x20) and an in-the-air flag (air time
 * +0x30 >= 0), then resets the current trick, restoring the rail mode (initial and current), rail distance (+0x24) and
 * air time.
 *
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evPressEnd(void* self)
{
    float curDist = *(float*)((char*)self + 0x24);
    float airSec = *(float*)((char*)self + 0x30);
    int rsm = *(int*)((char*)self + 0x20);
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, rsm, 0, airSec >= 0.0f);
    cRiderMetrix_resetCurTrick(self);
    *(int*)((char*)self + 0xC) = rsm;
    *(int*)((char*)self + 0x20) = rsm;
    *(float*)((char*)self + 0x24) = curDist;
    *(float*)((char*)self + 0x30) = airSec;
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
 * Stores @p pressMode as the current press mode (+0x28) and calls cRiderMetrix_evSpin (evSpin) with the spin angle (+0x34) + pi.
 *
 * @param pressMode  the press mode after the pivot (On Tour pressMode)
 * @return evSpin's result (0.0)
 */
extern "C" float cRiderMetrix_evPressPivot(void* self, int pressMode)
{
    *(int*)((char*)self + 0x28) = pressMode;
    return cRiderMetrix_evSpin_impl(self, *(float*)((char*)self + 0x34) + 3.1415927410125732f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evWipeout);
#ifdef SKIP_ASM
extern "C" int cRiderMetrix_losePoints(void* self);
extern "C" int cRiderMetrix_getCurTrickPoints(void* self);

/**
 * Scores a wipeout: counts it, loses the trick in progress and ends the combo.
 *
 * With @p flag (knocked down by another rider) Totals.NumDeaths (+0x12C) grows and HUD effect 0x2D shows; otherwise
 * Totals.NumWipeouts (+0x124) grows. cRiderMetrix_losePoints (losePoints) is added to Totals.TotalPointsLostToWipeouts (+0x1A0);
 * then the trick and combo are reset.
 *
 * @param flag  On Tour isFrag: the wipeout came from another rider's attack
 * @return -0.25 when a trick with points was lost, else 0.0
 */
extern "C" float cRiderMetrix_evWipeout(void* self, int flag)
{
    if (flag)
    {
        (*(int*)((char*)self + 0x12C))++;
        cRiderMetrix_addHUDfx1(self, 45, 0, 0, 1.5f);
    }
    else
    {
        (*(int*)((char*)self + 0x124))++;
    }
    float powerLoss = 0.0f;
    *(int*)((char*)self + 0x1A0) += cRiderMetrix_losePoints(self);
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
extern "C" float cRiderMetrix_evWipeoutRecover(void* self, int isUserRecover)
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
extern "C" float cRiderMetrix_evHandplantStart(void* self, int plant)
{
    *(int*)((char*)self + 0x7C) = plant;
    *(float*)((char*)self + 0x30) = -1.0f;
    *(int*)((char*)self + 0x78) = 0;
    *(float*)((char*)self + 0x14) += 0.04999999701976776f;
    cRiderMetrix_cancelComboChainReset(self);
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evHandplantEnd);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff);
extern "C" void cRiderMetrix_resetCurTrick(void* self);

/**
 * Scores the end of a handplant: earns the trick and starts a new one.
 *
 * Increments the handplant count (+0x80), calls cRiderMetrix_earnNameAndPoints (earnNameAndPoints) with only the last flag set, resets
 * the current trick and zeroes the air time (+0x30).
 *
 * @return the boost earnNameAndPoints returns
 */
extern "C" float cRiderMetrix_evHandplantEnd(void* self)
{
    *(int*)((char*)self + 0x80) += 1;
    float powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 1);
    cRiderMetrix_resetCurTrick(self);
    *(int*)((char*)self + 0x30) = 0;
    return powerUp;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evLateSpin);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_earnName(void* self, int a, int b, int c, int d);

/**
 * Scores a late spin: names the trick so far and restarts scoring as a late-spin trick.
 *
 * Calls cRiderMetrix_earnName (earnName) with no flags. If the trick had power (+0x14 > 0): the starting stance (+0x0) becomes
 * @p isFakieInit, spin (+0x34) and flip (+0x38) angles and the pipe, rail and press starts are cleared, the hold timer (+0x40) is
 * set to -1, the late-spin flag (+0x8) set and the 3 grab slots (+0x60) zeroed. The combo timer is suspended either
 * way.
 *
 * @param isFakieInit  On Tour isFakieInit: the late spin starts fakie
 * @return 0.0 (no boost change)
 */
extern "C" float cRiderMetrix_evLateSpin(void* self, int isFakieInit)
{
    cRiderMetrix_earnName(self, 0, 0, 0, 0);
    float powerUp = 0.0f;
    if (*(float*)((char*)self + 0x14) > powerUp)
    {
        *(int*)((char*)self + 0x0) = isFakieInit;
        *(float*)((char*)self + 0x34) = powerUp;
        *(float*)((char*)self + 0x38) = powerUp;
        *(int*)((char*)self + 0x4) = 0;
        *(int*)((char*)self + 0xC) = 0;
        *(int*)((char*)self + 0x10) = 0;
        *(float*)((char*)self + 0x40) = -1.0f;
        *(int*)((char*)self + 0x8) = 1;
        MEM_fill((char*)self + 0x60, 0, 0xC);
    }
    cRiderMetrix_cancelComboChainReset(self);
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evLand);
#ifdef SKIP_ASM
extern "C" void cRiderMetrix_resetCurTrick(void* self);
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff);

/**
 * Scores a landing: earns the trick and starts the next one from the landing state.
 *
 * Landing on a rail with trick power adds 0.13 power first. Calls cRiderMetrix_earnNameAndPoints (earnNameAndPoints) with the landing
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
extern "C" float cRiderMetrix_evLand(void* self, int stance, int alternate, int style, int flag)
{
    if (style != 0 && *(float*)((char*)self + 0x14) > 0.0f)
    {
        *(float*)((char*)self + 0x14) += 0.12999999523162842f;
    }
    int saved = *(int*)((char*)self + 0x70);
    float powerUp = cRiderMetrix_earnNameAndPoints(self, stance, alternate, style, flag, 0);
    cRiderMetrix_resetCurTrick(self);
    *(int*)((char*)self + 0x0) = stance;
    *(int*)((char*)self + 0x4) = alternate;
    *(int*)((char*)self + 0xC) = style;
    *(int*)((char*)self + 0x10) = flag;
    if (style != 0)
    {
        *(int*)((char*)self + 0x20) = style;
        *(int*)((char*)self + 0x24) = 0;
    }
    if (flag != 0)
    {
        *(int*)((char*)self + 0x28) = flag;
        *(int*)((char*)self + 0x2C) = 0;
    }
    if (saved != 0)
    {
        *(int*)((char*)self + 0x70) = saved;
        *(int*)((char*)self + 0x5C) = 1;
        *(int*)((char*)self + 0x6C) = 0;
    }
    return powerUp;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_evTakeoff);
#ifdef SKIP_ASM
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff);

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
extern "C" float cRiderMetrix_evTakeoff(void* self, int isFakieTakeoff, int isPipeTakeoff)
{
    float powerUp = 0.0f;
    int curRailMode = *(int*)((char*)self + 0x20);
    int curPressMode = *(int*)((char*)self + 0x28);
    if (*(int*)((char*)self + 0x70))
    {
        *(float*)((char*)self + 0x34) = powerUp;
        *(float*)((char*)self + 0x24) = -1.0f;
        *(int*)((char*)self + 0x20) = 0;
    }
    else
    {
        powerUp = cRiderMetrix_earnNameAndPoints(self, 0, 0, 0, 0, 1);
        cRiderMetrix_resetCurTrick(self);
    }
    *(int*)((char*)self + 0xC) = curRailMode;
    *(int*)((char*)self + 0x10) = curPressMode;
    *(int*)((char*)self + 0x4) = isPipeTakeoff;
    *(int*)((char*)self + 0x0) = isFakieTakeoff;
    *(int*)((char*)self + 0x30) = 0;
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
extern "C" void func_001597B0(void* iface, int id, int amount);
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
    return vt[7].fn(obj + vt[7].delta);
}

// PORT: callers in this unit declare it void; the function returns 0.0f.
float cRiderMetrix_evAddCash_impl(char* self, unsigned int kind, int amount) __asm__("cRiderMetrix_evAddCash");

/**
 * Awards career money to the rider and posts the matching HUD popups.
 *
 * Does nothing for 0, or while the replay is in a playback state (State 1 to 9). Otherwise it checks the
 * earnings unlocks (func_001E3760) and credits the money through the back end's reward interface
 * (func_001597B0), both for the rider's AI index (being slot 7), then posts the total flash (0x18, 0.7 s)
 * and, by @p kind, popup 0x2F (0), 0x2E (1), 0x30 (2, data = the combo count at +0x9C) for 2.5 s or 0x31 (3,
 * data = the amount) for 1.5 s; kind 4 posts no popup.
 *
 * @param kind    the cash source (On Tour eRiderMetrixCashSource), 0 to 4
 * @param amount  money to award
 * @return always 0.0
 */
// Awards trick/score metrics of the given kind and plays the matching reward popups.
float cRiderMetrix_evAddCash_impl(char* self, unsigned int kind, int amount)
{
    if (amount != 0 && !IsBusy119EF8()) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
        func_001E3760(GetId119EF8(self), amount);
        func_001597B0(iface, GetId119EF8(self), amount);
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
            cRiderMetrix_addHUDfx1_i(self, 0x30, amount, *(int*)(self + 0x9C), 2.5f);
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
extern "C" float cRiderMetrix_evMissionStart(void* self)
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
extern "C" float cRiderMetrix_evMissionSuccess(void* self, int amount)
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
extern "C" int cRiderMetrix_buildTrickName(void* self, sTrickId* id, int stance, int alternate, int style, int flag);
extern "C" int cRiderMetrix_checkForMonsterTrick(void* self, sTrickId* id);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_LandedMonsterCombo(void* audio, int rider);
extern "C" int cRiderMetrix_addToHistory(void* self, sTrickId* id);
extern "C" void cRiderMetrix_addTrick(void* self, sTrickId* id, int repeat);

// Same layout as sTrickId, which the unit defines after this function.
struct sTrickIdA168 {
    int w0;
    int w1;
};

/**
 * Names the trick so far without banking its points, paying any monster-trick bonus into the trick power.
 *
 * When cRiderMetrix_buildTrickName (buildTrickName) gives a name: a monster-trick bonus from cRiderMetrix_checkForMonsterTrick adds bonus / 10000 to the
 * trick power (+0x14), shows HUD effect 0x32 and plays the monster-combo sound; then cRiderMetrix_addTrick (addTrick) announces
 * it, as a repeat when cRiderMetrix_addToHistory finds it in the trick history.
 *
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand)
 * @param flag  the press landed in (On Tour pressLand)
 */
extern "C" void cRiderMetrix_earnName(void* self, int stance, int alternate, int style, int flag)
{
    sTrickIdA168 idv;
    sTrickId* id = (sTrickId*)&idv;
    if (cRiderMetrix_buildTrickName(self, id, stance, alternate, style, flag) != 0)
    {
        int pts = cRiderMetrix_checkForMonsterTrick(self, id);
        if (pts > 0)
        {
            *(float*)((char*)self + 0x14) += pts * 9.999999747378752e-05f;
            cRiderMetrix_addHUDfx1(self, 50, pts, 0, 1.5f);
            cSSXAudio_LandedMonsterCombo(func_0028B180(), *(int*)((char*)self + 0x1AC));
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

struct sScoreGrades {
    char pad_0x00[0x16c];
    int gradeCounts[6]; // 0x16c: landed tricks per grade (cRiderMetrix_calcTrickLevel)
};

extern sGameSettings cBENewRaceInterface_newRace;

extern "C" void cRiderMetrix_removeHUDfx(void* self, int type);
extern "C" int cAI_isRaceFinished(void* race);
extern "C" int cRiderMetrix_buildTrickName(void* self, sTrickId* id, int stance, int alternate, int style, int flag);
extern "C" int cRiderMetrix_getCurTrickPoints(void* self);
extern "C" int cRiderMetrix_checkForMonsterTrick(void* self, sTrickId* id);
extern "C" void cRiderMetrix_addHUDfx1(void* self, int type, int value, int arg, float duration);
extern "C" void* func_0028B180();
extern "C" void cSSXAudio_LandedMonsterCombo(void* audio, int rider);
extern "C" int cRiderMetrix_addToHistory(void* self, sTrickId* id);
extern "C" void cRiderMetrix_addTrick(void* self, sTrickId* id, int repeat);
extern "C" int cRiderMetrix_getCurTrickPointsWithMult(void* self);
extern "C" int cRiderMetrix_calcTrickLevel(void* self, int points);
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(void* self);
extern "C" void cRiderMetrix_addToComboChain(void* self, int points);
extern "C" void cSSXAudio_LandedUberTrick(void* audio, int rider, int ubers, int runUbers);
extern "C" void cRiderMetrix_evAddCash(void* self, int kind, int amount);
extern "C" void cRiderMetrix_delayComboChainReset(void* self, float seconds);
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
extern "C" float cRiderMetrix_earnNameAndPoints(void* self, int stance, int alternate, int style, int flag, int takeoff)
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
                *(float*)((char*)self + 0x14) += (float)bonus * 0.0001f;
                cRiderMetrix_addHUDfx1(self, 50, bonus, 0, 1.5f);
                cSSXAudio_LandedMonsterCombo(func_0028B180(), *(int*)((char*)self + 0x1ac));
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
        *(int*)((char*)self + 0x198) += points + inverted;
        *(int*)((char*)self + 0x19c) += inverted;
        *(int*)((char*)self + 0x110) += 1;
        ((sScoreGrades*)self)->gradeCounts[grade]++;

        if (*(int*)((char*)self + 0x18c) < pending) {
            *(int*)((char*)self + 0x18c) = pending;
            *(sTrickId*)((char*)self + 0x190) = id;
        }

        spin = func_roundDegrees((int)(*(float*)((char*)self + 0x34) * 57.295776f));
        flip = func_roundDegrees((int)(*(float*)((char*)self + 0x38) * 57.295776f));
        *(int*)((char*)self + 0xfc) += abs(spin);
        *(int*)((char*)self + 0x100) += abs(flip);
        *(float*)((char*)self + 0x144) += *(float*)((char*)self + 0x44);
        *(int*)((char*)self + 0x104) += *(int*)((char*)self + 0x4c);
        *(int*)((char*)self + 0x108) += *(int*)((char*)self + 0x80);

        if (*(int*)((char*)self + 0x54) > 0) {
            cSSXAudio_LandedUberTrick(func_0028B180(), *(int*)((char*)self + 0x1ac), *(int*)((char*)self + 0x54),
                          *(int*)((char*)self + 0x114));
        }

        *(int*)((char*)self + 0x114) += *(int*)((char*)self + 0x54);
        *(int*)((char*)self + 0x118) += *(int*)((char*)self + 0x58);
        *(int*)((char*)self + 0x11c) += *(int*)((char*)self + 0x74);
        *(float*)((char*)self + 0x14c) = MAX(*(float*)((char*)self + 0x30), *(float*)((char*)self + 332));
        *(float*)((char*)self + 0x150) = MAX(*(float*)((char*)self + 72), *(float*)((char*)self + 336));
        *(float*)((char*)self + 0x154) = MAX(*(float*)((char*)self + 36), *(float*)((char*)self + 340));
        *(float*)((char*)self + 0x158) = MAX(*(float*)((char*)self + 0x78), *(float*)((char*)self + 344));
        *(int*)((char*)self + 0x198) += *(int*)((char*)self + 0x84);
        total = points + *(int*)((char*)self + 0x84) + inverted;

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
    } else if (*(int*)((char*)self + 0x20) != 0 || *(int*)((char*)self + 0x28) != 0) {
        return delta;
    }
    if (*(float*)((char*)self + 0x14) > 0.0f || *(float*)((char*)self + 0xa4) < 0.0f) {
        cRiderMetrix_delayComboChainReset(self, 1.5f);
    }
    return delta;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_losePoints);
#ifdef SKIP_ASM
extern "C" int cRiderMetrix_getCurTrickStyleSubPoints(void* self);
extern "C" int cRiderMetrix_getCurTrickPoints(void* self);
extern "C" void cRiderMetrix_removeHUDfx(void* self, int type);
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
extern "C" int cRiderMetrix_losePoints(void* self)
{
    int n = cRiderMetrix_getCurTrickPoints(self);
    if (n > 0)
    {
        cRiderMetrix_removeHUDfx(self, 1);
        cRiderMetrix_addHUDfx1(self, 37, n, 0, 1.5f);
    }
    if (*(int*)((char*)self + 0xA0) > 0)
    {
        cRiderMetrix_removeHUDfx(self, 3);
        cRiderMetrix_addHUDfx1(self, 39, *(int*)((char*)self + 0xA0), 0, 1.5f);
    }
    int total = *(int*)((char*)self + 0x84) + cRiderMetrix_getCurTrickStyleSubPoints(self);
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
extern unsigned char D_0043D388[][11];
extern unsigned char D_0043D4C8[][11];

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

struct sTrickState_A8C8 {
    int u0;          // 0x00
    int u4;          // 0x04
    int u8;          // 0x08
    int uC;          // 0x0C
    int u10;         // 0x10
    char pad14[0xC];
    int u20;         // 0x20
    float f24;       // 0x24
    int u28;         // 0x28
    float f2C;       // 0x2C
    int pad30;
    float f34;       // 0x34
    float f38;       // 0x38
    char pad3C[0x24];
    int u60[3];      // 0x60
    char pad6C[4];
    int u70;         // 0x70
    char pad74[8];
    int u7C;         // 0x7C
    char pad80[0x1AC - 0x80];
    char* rider;     // 0x1AC
};

/**
 * Builds the name record of the trick in progress from its spins, flips, grabs and landing.
 *
 * Zeroes the 8-byte name (On Tour tTrickNameDef) and packs its bit fields from the trick state: uber grind and rail
 * mode, takeoff and landing stance and pipe flags, spin and flip counts rounded from the angles (+0x34, +0x38), up to 3
 * grabs (+0x60), handplant (+0x7C) and press, using the tables D_0043D388 / D_0043D4C8.
 *
 * @param vself  the score object (this)
 * @param vid  receives the trick name (On Tour tTrickNameDef &tn)
 * @param stance  On Tour isFakieLand: landed fakie
 * @param alternate  On Tour isPipeLand: landed in a pipe
 * @param style  the rail-slide mode landed in (On Tour rsmLand)
 * @param flag  the press landed in (On Tour pressLand)
 * @return nonzero when the trick has a name
 */
extern "C" int cRiderMetrix_buildTrickName(void* vself, sTrickId* vid, int stance, int alternate, int style, int flag)
{
    sTrickState_A8C8* self = (sTrickState_A8C8*)vself;
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
    if (self->u70 != 0) {
        id->k0 = self->uC;
        id->m0 = self->u70;
        if (style != 0) {
            id->q = 3;
        }
        return 1;
    }
    if (self->u0 != 0) {
        self->f34 = -self->f34;
    }
    if (self->u20 == 4) {
        self->f38 = -self->f38;
    }
    spinDeg = func_roundDegrees((int)(self->f34 * 57.2957763671875f));
    flipDeg = func_roundDegrees((int)(self->f38 * 57.2957763671875f));
    flips = abs(flipDeg / 360);
    spins = abs(spinDeg / 180);
    if (self->u28 != 0) {
        int nameCode;
        if (spins >= 2) {
            if (spins < 12) {
                if (spins >= 6) nameCode = 14; else nameCode = 13;
            } else {
                nameCode = 15;
            }
            id->p = nameCode;
        } else if (spins == 1 && self->f2C >= 3.0f) {
            if (self->u28 == spins) {
                if (self->f2C < 12.0f) {
                    if (self->f2C < 9.0f) {
                        if (self->f2C >= 6.0f) nameCode = 7; else nameCode = 5;
                    } else nameCode = 9;
                } else nameCode = 11;
            } else {
                if (self->f2C < 12.0f) {
                    if (self->f2C < 9.0f) {
                        if (self->f2C >= 6.0f) nameCode = 8; else nameCode = 6;
                    } else nameCode = 10;
                } else nameCode = 12;
            }
            id->p = nameCode;
        } else {
            if (self->u28 == 1) {
                id->p = self->u20 ? 3 : 1;
            } else {
                id->p = self->u20 == 0 ? 2 : 4;
            }
            id->k1 = spins < 11 ? spins : 10;
        }
        return 1;
    }
    if (self->u20 != 0) {
        if (self->f24 <= 50.0f) {
            id->k0 = 5;
        } else {
            id->k0 = self->u20;
            id->k1 = spins < 11 ? spins : 10;
            if (*(int*)(self->rider + 0x324) != 0) {
                if (self->u20 == 1) {
                    id->k0 = 2;
                } else if (self->u20 == 2) {
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
    for (n = 0; n < 3 && self->u60[n] != 0; n++) {
    }
    if (self->u10 == 1) {
        id->k2 = 5;
    } else if (self->u10 == 2) {
        id->k2 = 6;
    } else if (self->uC != 0) {
        if (self->u0 != 0) {
            id->k2 = 3;
        } else {
            id->k2 = 2;
        }
    } else if (self->u8 != 0) {
        id->k2 = 4;
    } else if (self->u0 != 0) {
        id->k2 = 1;
    }
    if (idx < 29 && fidx < 11) {
        if (self->u4 != 0 && alternate != 0) {
            id->tbl = D_0043D4C8[idx][fidx];
        } else {
            id->tbl = D_0043D388[idx][fidx];
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
                if (self->f38 > 0.0f) {
                    id->d = 4;
                } else {
                    id->d = 3;
                }
            } else {
                if (self->f38 > 0.0f) {
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
        if (stance != 0 && self->u0 == 0) {
            id->q = 2;
        } else if (spins == 0 && flips == 0 && n > 0) {
            id->q = 1;
        }
    }
    if (n > 0) {
        id->f = self->u60[0];
        if (n >= 2) {
            id->h = 1;
            if (n == 2) {
                id->g = self->u60[1];
            } else {
                id->g = 1;
            }
        }
    }
    if (self->u7C != 0) {
        id->n = self->u7C;
    }
    return flips > 0 || spins > 0 || n > 0 || self->u7C != 0;
}
#endif

//100%
INCLUDE_ASM("ai/ridermetrix", cRiderMetrix_checkForMonsterTrick);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int value, int size);

struct sTrickBonus_11B1A8
{
    int grade;              // 0x0
    int points;             // 0x4
    unsigned char a;        // 0x8
    unsigned char b;        // 0x9
    unsigned char c;        // 0xA
    unsigned char d;        // 0xB
    unsigned char e;        // 0xC
    unsigned char f;        // 0xD
    unsigned char g;        // 0xE
    unsigned char pad;      // 0xF
};
extern sTrickBonus_11B1A8 gMonsterComboTable[];

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
extern "C" int cRiderMetrix_checkForMonsterTrick(void* self, sTrickId* id)
{
    sTrickBits_11B1A8* trick = (sTrickBits_11B1A8*)id;
    sTrickBonus_11B1A8* comboEntry = gMonsterComboTable;
    for (int i = 0; i < 24; i++, comboEntry++)
    {
        if (trick->a != comboEntry->a) continue;
        if (trick->b != comboEntry->b) continue;
        if (trick->c != comboEntry->c) continue;
        if (trick->d != comboEntry->d) continue;
        if (trick->e != comboEntry->e) continue;
        if (trick->f != comboEntry->f) continue;
        if (trick->g != comboEntry->g) continue;
        MEM_fill(id, 0, 8);
        trick->grade = comboEntry->grade;
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
extern "C" float func_001494C0(void* iface, int a1, int a2);
int cRider_getMotionState(void* self);
extern float kTerminalVelocityTable[2][3][4][2];

/**
 * Recomputes the rider's top speed from snow type, tuck, boost level and the rider's speed stat.
 *
 * Interpolates kTerminalVelocityTable (filled by InitTerminalVelocityTable) by surface property and tuck amount for each
 * speed-attribute row, mixes the rows by the rider's speed attribute (func_001494C0 on the stat interface), scales by
 * 27.78 and smooths it into the old value (+0x2E4) with 0.9 (0.97 when slowing), losing 2% when off natural stance
 * without boost. In motion state 1 the limit is 3333.33. At uber level 11+ while boosting it never drops below +0x30C.
 */
// cRider::updateTerminalVelocity (On Tour name; pairing unverified, structure fits the retail asm)
extern "C" void cRider_updateTerminalVelocity(char* self)
{
    float speedAttribute = func_001494C0(cBE_getInterface(cBE_getBE(), 3), *(int*)(self + 0x86C), *(int*)(self + 0xB34));
    // NOTE: the retail code adds and subtracts 1.0 (likely an inlined bx range helper).
    speedAttribute = speedAttribute + 1.0f - 1.0f;
    char* surfPropCache = (char*)SSXApp->Game;
    char* snow = *(char**)(surfPropCache + 0x44) + *(int*)(self + 0x438) * 176;
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
 * The sort comparator func_0011BD60 and func_0011BE88 pass to func_00418EF8; the second string arrives in a1.
 *
 * @return strcasecmp's result
 */
void* func_0011B678(void* self)
{
    return strcasecmp(self);
}
#endif

