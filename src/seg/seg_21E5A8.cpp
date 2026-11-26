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
INCLUDE_ASM("seg/seg_21E5A8", cCubicSplineInterpolant_cCubicSplineInterpolant);
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
 *
 * @return @p self
 */
extern "C" Flags_D5E8 *cCubicSplineInterpolant_cCubicSplineInterpolant(cCubicSplineInterpolant* self) {
    self->CubicSplines = 0;
    ((Flags_D5E8*)self)->b = 0;
    ((Flags_D5E8*)self)->c = 0;
    ((Flags_D5E8*)self)->d = 0;
    return ((Flags_D5E8*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21E5A8", cCubicSplineInterpolant__cCubicSplineInterpolant);
#ifdef SKIP_ASM
void cCubicSplineInterpolant_purge(void *);
void operator_delete(int *);
/**
 * Destroys a cubic-spline interpolant: frees its segments and, when asked, the object.
 *
 * Calls cCubicSplineInterpolant_purge and, when bit 0 of @p flags is set, operator delete on @p self (the g++
 * deleting-destructor flag).
 *
 * @param flags  bit 0 set: also free the object
 */
extern "C" void cCubicSplineInterpolant__cCubicSplineInterpolant(cCubicSplineInterpolant* self, int flags) {
    cCubicSplineInterpolant_purge(self);
    if (flags & 1) operator_delete(((int*)self));
}
#endif
