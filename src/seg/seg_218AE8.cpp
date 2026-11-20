#include "common.h"

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317AE8);
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
extern "C" void func_00317AE8(char *self, float x) {
    if (*(int*)(self + 0x10) == 0) {
        *(float*)(self + 8) = *(float*)(self + 0xC) = x;
    } else {
        if (x < *(float*)(self + 8)) *(float*)(self + 8) = x;
        if (*(float*)(self + 0xC) < x) *(float*)(self + 0xC) = x;
    }
    *(float*)(self + 0) = *(float*)(self + 0) + x;
    *(float*)(self + 4) = *(float*)(self + 4) + x * x;
    *(int*)(self + 0x10) = *(int*)(self + 0x10) + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317B50);
#ifdef SKIP_ASM
struct sVec4_7B50 { float x, y, z, w; } __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_7B50(const sVec4_7B50& a, const sVec4_7B50& b)
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
static inline sVec4_7B50 vu0Scale_7B50(const sVec4_7B50& v, float s)
{
    sVec4_7B50 r;
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
static inline void setPlane_7B50(sVec4_7B50* out, const sVec4_7B50& n, const sVec4_7B50& p)
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
extern "C" void func_00317B50(sVec4_7B50* out, sVec4_7B50* n, sVec4_7B50* point, sVec4_7B50* frontPoint)
{
    setPlane_7B50(out, *n, *point);
    if (vu0Dot_7B50(*out, *frontPoint) < 0.0f) {
        setPlane_7B50(out, vu0Scale_7B50(*n, -1.0f), *point);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317C48);
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
extern "C" int func_00317C48(char *planeA, char *planeB) {
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
INCLUDE_ASM("seg/seg_218AE8", func_00317D10);
#ifdef SKIP_ASM
extern int D_004A3E80;

/**
 * Stores the bx output-channel mask D_004A3E80 (initially 1).
 *
 * func_00317D18 reads it back; no retail code calls either.
 *
 * @param arg0  the new mask
 */
extern "C" void func_00317D10(int arg0) {
    D_004A3E80 = arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317D18);
#ifdef SKIP_ASM
extern int D_004A3E80;

/**
 * Returns the bx output-channel mask D_004A3E80 (initially 1).
 *
 * func_00317D10 sets it; no retail code calls either.
 *
 * @return the mask
 */
extern "C" int func_00317D18(void) {
    return D_004A3E80;
}
#endif
