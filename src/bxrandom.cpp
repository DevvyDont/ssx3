#include "common.h"

extern "C" void* memcpy(void* dst, const void* src, uint size);

void cBxPseudoRng_Seed(uint* state, uint seed);
uint cBxPseudoRng_NextInt(uint* state);

// Presentation and gameplay generator states, six words each
extern uint D_004FF018[6];
extern uint D_004FF030[6];
// Gameplay generator state after seeding with 0
extern const uint D_0048DCB0[6];

void BXsrand(uint seed)
{
	cBxPseudoRng_Seed(D_004FF018, seed);
	return;
}

uint BXrand()
{
    return cBxPseudoRng_NextInt(D_004FF018);
}

uint AIrand()
{
    return cBxPseudoRng_NextInt(D_004FF030);
}

float AIrandf(float a, float b)
{
	// Random mantissa with exponent 0, giving 1.0 <= u.f < 2.0
	union { uint i; float f; } u;
	u.i = (AIrand() & 0x7FFFFF) | 0x3F800000;
	return a + (b - a) * (u.f - 1.0f);
}

float func_00317890(float param_1, float param_2)
{
	float fVar1;

	fVar1 = AIrandf(-1.0, 1.0);
	return param_1 + param_2 * fVar1;
}

// Saves the gameplay generator state
void func_003178E0(uint* out)
{
    memcpy(out, D_004FF030, sizeof(D_004FF030));
}

// Restores the gameplay generator state
void func_00317908(uint* in)
{
    memcpy(D_004FF030, in, sizeof(D_004FF030));
}

// Gets the seed-0 gameplay generator state
void func_00317930(uint* out)
{
    memcpy(out, D_0048DCB0, sizeof(D_0048DCB0));
}

void cBxPseudoRng_Seed(uint* state, uint seed)
{
    ulong s = seed;

    s += 0xF22D0E56;
    state[0] = s;
    s += 0x96041893;
    state[1] = s;
    s += 0x3DF3B646;
    state[2] = s;
    s += 0x40DDE76D;
    state[3] = s;
    s += 0x97327AE1;
    state[4] = s;
    s += 0xD1A9FBE7;
    state[5] = s;
}

// Six-word add-with-carry generator. Words 4..0 are summed with carry, then
// word 5 acts as a counter that ripples up through the others on overflow.
uint cBxPseudoRng_NextInt(uint* state)
{
    uint sum;
    uint carry;

    sum = state[5] + state[4];
    carry = 0;
    if (sum < state[5] || sum < state[4])
        carry = 1;
    state[4] = sum;

    sum = sum + state[3] + carry;
    carry = sum < state[3];
    state[3] = sum;

    sum = sum + state[2] + carry;
    carry = sum < state[2];
    state[2] = sum;

    sum = sum + state[1] + carry;
    carry = sum < state[1];
    state[1] = sum;

    sum = sum + state[0] + carry;
    state[0] = sum;

    if (++state[5] == 0 && ++state[4] == 0 && ++state[3] == 0 && ++state[2] == 0 && ++state[1] == 0)
        sum = ++state[0];

    return sum;
}
