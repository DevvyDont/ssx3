#include "common.h"
#include "bx/cubicspline.h"

//100%
INCLUDE_ASM("seg/seg_21E5A8", cCSICubicSpline_evaluate);
#ifdef SKIP_ASM
// Evaluates the cubic a*t^3 + b*t^2 + c*t + d at t = x - x0.
struct Cubic_D5A8 {
    float a;
    float b;
    float c;
    float d;
    float x0; // 0x10
};
/**
 * Evaluates one cubic segment of a spline interpolant at @p x.
 *
 * With t = @p x - X0 (+0x10) it returns A t^3 + B t^2 + C t + D from the segment's coefficients (+0x0 to +0xC, On Tour
 * cCSICubicSpline).
 *
 * @param spline  the segment (A, B, C, D, X0)
 * @param x  where to evaluate
 * @return the cubic's value
 */
extern "C" float cCSICubicSpline_evaluate(Cubic_D5A8 *spline, float x) {
    float relX = x - spline->x0;
    float coefA = spline->a;
    float coefB = spline->b;
    float coefC = spline->c;
    float coefD = spline->d;
    float relXSq = relX * relX;
    float lowerTerms = coefC * relX + coefD;
    lowerTerms += coefB * relXSq;
    return relX * coefA * relXSq + lowerTerms;
}
#endif

//100%
INCLUDE_ASM("seg/seg_21E5A8", __23cCubicSplineInterpolant);
#ifdef SKIP_ASM
struct Flags_D5E8 {
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int pad : 8;
    unsigned int d : 8;
    unsigned int rest : 13;
    int w4;
    int w8;
};
/**
 * Constructs an empty cubic-spline interpolant.
 *
 * Clears CubicSplines (+0x8) and the Initialized, Loaded and NumSplines bit fields of the first word (On Tour
 * cCubicSplineInterpolant layout); the other bits and MaxX are left as they are.
 */
cCubicSplineInterpolant::cCubicSplineInterpolant()
{
    CubicSplines = 0;
    ((Flags_D5E8*)this)->b = 0;
    ((Flags_D5E8*)this)->c = 0;
    ((Flags_D5E8*)this)->d = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_21E5A8", _$_23cCubicSplineInterpolant);
#ifdef SKIP_ASM
/**
 * Destroys a cubic-spline interpolant: frees its segments (purge).
 */
cCubicSplineInterpolant::~cCubicSplineInterpolant()
{
    purge();
}
#endif
