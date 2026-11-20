#include "common.h"
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed);
extern "C" uint cBxPseudoRng_NextInt(uint* state);
void BXsrand(uint seed);
extern const uint BXrandom[];

//100%
//https://decomp.me/scratch/ViFdz
INCLUDE_ASM("bx/bxrandom", BXsrand__FUi);
#ifdef SKIP_ASM
/**
 * Seeds the game's main pseudo-random generator (the state at BXrandom) through cBxPseudoRng_Seed().
 *
 * @param seed  the seed value
 */
void BXsrand(uint seed)
{
	cBxPseudoRng_Seed((uint*)BXrandom, seed);
	return;
}
#endif

INCLUDE_ASM("bx/bxrandom", BXrand__Fv);
#ifdef SKIP_ASM

/**
 * Advances the game's main pseudo-random generator (BXrandom) through cBxPseudoRng_NextInt().
 *
 * The new value is left in the return register although the C declaration returns void.
 */
void BXrand()
{
    cBxPseudoRng_NextInt((uint*)BXrandom);
}
#endif


//100%
INCLUDE_ASM("bx/bxrandom", AIrand__Fv);
#ifdef SKIP_ASM
extern const uint AIrandom[];

/**
 * Draws the next number from the AI's own pseudo-random generator (the state at AIrandom).
 *
 * @return the next pseudo-random value
 */
unsigned int AIrand()
{
    return cBxPseudoRng_NextInt((uint*)AIrandom);
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", AIrandf__Fff);
#ifdef SKIP_ASM
/**
 * Draws a random float between two bounds from the AI's random generator.
 *
 * Builds a float in [1, 2) from 23 random mantissa bits of AIrand(), so the result is uniform in [a, b).
 *
 * @param low  lower bound (returned when the random fraction is 0)
 * @param high  upper bound (never quite reached)
 * @return a + (b - a) * fraction, with fraction in [0, 1)
 */
float AIrandf(float low, float high)
{
    union {
        unsigned int i;
        float f;
    } floatBits;
    floatBits.i = (AIrand() & 0x7FFFFF) | 0x3F800000; // float in [1.0, 2.0)
    return low + (high - low) * (floatBits.f - 1.0f);
}
#endif

INCLUDE_ASM("bx/bxrandom", func_00317890__Fff);
#ifdef SKIP_ASM
/**
 * Returns @p param_1 plus @p param_2 times a random value in [-1, 1) from the AI generator (AIrandf).
 *
 * The world sphere tree uses it to vary a speed around its base value.
 *
 * @param param_1  the base value
 * @param param_2  the largest deviation
 * @return the varied value
 */
float func_00317890(float param_1, float param_2)
{
	float fVar1;

	fVar1 = AIrandf(-1.0, 1.0);
	return param_1 + param_2 * fVar1;
}
#endif

extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("bx/bxrandom", AIgetseed__FPv);
#ifdef SKIP_ASM
/**
 * Saves the AI random generator's state, so replays and online games can reproduce the AI's choices.
 *
 * MEM_copy of the 0x18-byte AIrandom state into @p self (here the destination buffer, not an object).
 *
 * @return MEM_copy's result
 */
void* AIgetseed(void* self)
{
    return MEM_copy(self, (void*)AIrandom, 0x18);
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", AIsetseed__FPv);
#ifdef SKIP_ASM
/**
 * Restores the AI random generator's state saved by AIgetseed__FPv.
 *
 * MEM_copy of 0x18 bytes from @p self (the saved state, not an object) into AIrandom.
 *
 * @return MEM_copy's result
 */
void* AIsetseed(void* self)
{
    return MEM_copy((void*)AIrandom, self, 0x18);
}
#endif

extern void* D_0048DCB0[];
extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("bx/bxrandom", func_00317930__FPv);
#ifdef SKIP_ASM
/**
 * Resets a random generator's six-word state to the fixed default state D_0048DCB0 (MEM_copy of 0x18 bytes).
 *
 * Static initialisation runs it on BXrandom, AIrandom and GMrandom.
 *
 * @return MEM_copy's result (@p self)
 */
void* func_00317930(void* self)
{
    return MEM_copy(self, (void*)D_0048DCB0, 0x18);
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", cBxPseudoRng_Seed);
#ifdef SKIP_ASM
/**
 * Seeds a pseudo-random generator state from one value.
 *
 * Fills the six state words with @p seed plus a running sum of fixed constants: state[0] = seed + 0xF22D0E56,
 * state[1] = state[0] + 0x96041893, and so on.
 *
 * @param state  six-word generator state to fill
 * @param seed   seed value
 */
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed)
{
    ulong x = seed;

    x += 0xF22D0E56; state[0] = x;
    x += 0x96041893; state[1] = x;
    x += 0x3DF3B646; state[2] = x;
    x += 0x40DDE76D; state[3] = x;
    x += 0x97327AE1; state[4] = x;
    x += 0xD1A9FBE7; state[5] = x;
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", cBxPseudoRng_NextInt);
#ifdef SKIP_ASM
/**
 * Advances a six-word pseudo-random state and returns its top word.
 *
 * Adds state[5] into state[4], then ripples the running sum with carries up through state[3] to state[0]. Then
 * state[5] is incremented as a counter that carries into the higher words when it wraps.
 *
 * @param state  the six-word generator state
 * @return the new state[0]
 */
extern "C" uint cBxPseudoRng_NextInt(uint* state)
{
    uint word5 = state[5];
    uint word4 = state[4];
    uint sum = word5 + word4;
    uint carry = 0;

    if (sum < word5 || sum < word4)
        carry = 1;
    state[4] = sum;
    sum = sum + state[3] + carry; carry = sum < state[3]; state[3] = sum;
    sum = sum + state[2] + carry; carry = sum < state[2]; state[2] = sum;
    sum = sum + state[1] + carry; carry = sum < state[1]; state[1] = sum;
    sum = sum + state[0] + carry; state[0] = sum;

    if (++state[5] == 0 && ++state[4] == 0 && ++state[3] == 0 &&
        ++state[2] == 0 && ++state[1] == 0)
    {
        state[0] = sum + 1;
        sum = state[0];
    }
    return sum;
}
#endif
