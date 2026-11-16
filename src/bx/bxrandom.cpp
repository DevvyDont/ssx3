#include "common.h"
extern "C" void cBxPseudoRng_Seed(uint* state, uint seed);
extern "C" uint cBxPseudoRng_NextInt(uint* state);
void BXsrand(uint seed);
extern const uint D_004FF018[];

//100%
//https://decomp.me/scratch/ViFdz
INCLUDE_ASM("bx/bxrandom", BXsrand__FUi);
#ifdef SKIP_ASM
/**
 * Seeds the game's main pseudo-random generator (the state at D_004FF018) through cBxPseudoRng_Seed().
 *
 * @param seed  the seed value
 */
void BXsrand(uint seed)
{
	cBxPseudoRng_Seed((uint*)D_004FF018, seed);
	return;
}
#endif

INCLUDE_ASM("bx/bxrandom", BXrand__Fv);
#ifdef SKIP_ASM

/**
 * Advances the game's main pseudo-random generator (D_004FF018) through cBxPseudoRng_NextInt().
 *
 * The new value is left in the return register although the C declaration returns void.
 */
void BXrand()
{
    cBxPseudoRng_NextInt((uint*)D_004FF018);
}
#endif


//100%
INCLUDE_ASM("bx/bxrandom", AIrand__Fv);
#ifdef SKIP_ASM
extern const uint D_004FF030[];

/**
 * Draws the next number from the AI's own pseudo-random generator (the state at D_004FF030).
 *
 * @return the next pseudo-random value
 */
unsigned int AIrand()
{
    return cBxPseudoRng_NextInt((uint*)D_004FF030);
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", AIrandf__Fff);
#ifdef SKIP_ASM
float AIrandf(float a, float b)
{
    union {
        unsigned int i;
        float f;
    } r;
    r.i = (AIrand() & 0x7FFFFF) | 0x3F800000; // float in [1.0, 2.0)
    return a + (b - a) * (r.f - 1.0f);
}
#endif

INCLUDE_ASM("bx/bxrandom", func_00317890__Fff);
#ifdef SKIP_ASM
float func_00317890(float param_1, float param_2)
{
	float fVar1;

	fVar1 = AIrandf(-1.0, 1.0);
	return param_1 + param_2 * fVar1;
}
#endif

extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("bx/bxrandom", func_003178E0__FPv);
#ifdef SKIP_ASM
void* func_003178E0(void* self)
{
    return MEM_copy(self, (void*)D_004FF030, 0x18);
}
#endif

//100%
INCLUDE_ASM("bx/bxrandom", func_00317908__FPv);
#ifdef SKIP_ASM
void* func_00317908(void* self)
{
    return MEM_copy((void*)D_004FF030, self, 0x18);
}
#endif

extern void* D_0048DCB0[];
extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("bx/bxrandom", func_00317930__FPv);
#ifdef SKIP_ASM
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
    uint a = state[5];
    uint b = state[4];
    uint t = a + b;
    uint c = 0;

    if (t < a || t < b)
        c = 1;
    state[4] = t;
    t = t + state[3] + c; c = t < state[3]; state[3] = t;
    t = t + state[2] + c; c = t < state[2]; state[2] = t;
    t = t + state[1] + c; c = t < state[1]; state[1] = t;
    t = t + state[0] + c; state[0] = t;

    if (++state[5] == 0 && ++state[4] == 0 && ++state[3] == 0 &&
        ++state[2] == 0 && ++state[1] == 0)
    {
        state[0] = t + 1;
        t = state[0];
    }
    return t;
}
#endif
