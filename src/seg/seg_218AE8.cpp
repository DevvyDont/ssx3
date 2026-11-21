#include "common.h"
#include "bx/math3d.h"
#include "bx/statscalc.h"

//100%
INCLUDE_ASM("seg/seg_218AE8", cStatsCalc_add);
#ifdef SKIP_ASM
/**
 * Adds a sample to running statistics: sum, sum of squares, minimum, maximum and count.
 *
 * @p self is laid out as On Tour's cStatsCalc: Sum (+0x0), SumSquare (+0x4), MinSample (+0x8), MaxSample (+0xC),
 * Samples (+0x10). The first sample sets both the minimum and the maximum; later ones widen them. No retail code calls
 * it.
 *
 * @param x  the sample
 */
extern "C" void cStatsCalc_add(cStatsCalc* self, float x) {
    if (self->Samples == 0) {
        self->MinSample = self->MaxSample = x;
    } else {
        if (x < self->MinSample) self->MinSample = x;
        if (self->MaxSample < x) self->MaxSample = x;
    }
    self->Sum = self->Sum + x;
    self->SumSquare = self->SumSquare + x * x;
    self->Samples = self->Samples + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", tSemiSpace_set);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_7B50(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Scale_7B50(const t4Vector& v, float s)
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


// Plane through p with normal n: w = -dot(n, p).
static inline void setPlane_7B50(t4Vector* out, const t4Vector& n, const t4Vector& p)
{
    *out = n;
    out->w = vu0Dot_7B50(vu0Scale_7B50(n, -1.0f), p);
}

/**
 * Sets a half-space: the plane through a point with a given normal, flipped so that a reference point lies on its
 * positive side.
 *
 * @p out gets the normal in xyz and -dot(normal, point) in w. When the 4-component dot product of that plane with @p
 * frontPoint is negative, it is rebuilt from the negated normal. No retail code calls it.
 *
 * @param out  the plane to set (normal xyz, offset w)
 * @param n  the plane normal
 * @param point  a point on the plane
 * @param frontPoint  a point that must end up on the positive side
 */
extern "C" void tSemiSpace_set(t4Vector* out, t4Vector* n, t4Vector* point, t4Vector* frontPoint)
{
    setPlane_7B50(out, *n, *point);
    if (vu0Dot_7B50(*out, *frontPoint) < 0.0f) {
        setPlane_7B50(out, vu0Scale_7B50(*n, -1.0f), *point);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", tSemiSpace_isSimilar);
#ifdef SKIP_ASM
struct sVec4_7C48 { float x, y, z, w; } __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_7C48(sVec4_7C48 a, sVec4_7C48 b)
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

static inline sVec4_7C48 mk_7C48(char *p)
{
    sVec4_7C48 v = *(sVec4_7C48*)p;
    v.w = 0.0f;
    return v;
}

// PORT: abs.s asm helper
static inline float fabs_7C48(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Returns whether two planes are nearly the same.
 *
 * The offsets (w) must agree: when @p planeA's is 0, @p planeB's must be within 0.1 of 0, otherwise their ratio must
 * lie between 0.999 and 1.001. The normals' dot product (xyz, w taken as 0) must be at least 0.999. No retail code
 * calls it.
 *
 * @param planeA  the first plane (normal xyz, offset w)
 * @param planeB  the second plane
 * @return 1 when similar, else 0
 */
extern "C" int tSemiSpace_isSimilar(char *planeA, char *planeB) {
    float distA = *(float*)(planeA + 0xC);
    if (distA == 0.0f) {
        if (fabs_7C48(*(float*)(planeB + 0xC)) > 0.1f) return 0;
    } else {
        float distRatio = *(float*)(planeB + 0xC) / distA;
        if (distRatio > 1.001f) return 0;
        if (distRatio < 0.999f) return 0;
    }
    if (vu0Dot_7C48(mk_7C48(planeA), mk_7C48(planeB)) < 0.999f) return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", bxSetOutputChannels);
#ifdef SKIP_ASM
extern int OutputChannelMask;

/**
 * Stores the bx output-channel mask OutputChannelMask (initially 1).
 *
 * bxGetOutputChannels reads it back; no retail code calls either.
 *
 * @param mask  the new mask
 */
extern "C" void bxSetOutputChannels(int mask) {
    OutputChannelMask = mask;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", bxGetOutputChannels);
#ifdef SKIP_ASM
extern int OutputChannelMask;

/**
 * Returns the bx output-channel mask OutputChannelMask (initially 1).
 *
 * bxSetOutputChannels sets it; no retail code calls either.
 *
 * @return the mask
 */
extern "C" int bxGetOutputChannels(void) {
    return OutputChannelMask;
}
#endif
