#ifndef BX_CUBICSPLINE_H
#define BX_CUBICSPLINE_H

// cCubicSplineInterpolant: a 1-D cubic spline y(x) through up to 256 keys, natural or clamped (given end slopes).
// Use: initAsNatural / initAsClamped(startSlope, endSlope) allocate NumSplines = n - 1 segments (initCommon,
// operator new(NumSplines * 20, "cCSICubicSpline")); addPoint(x, y) n times stores the keys into the segment array and,
// on the last one, calculateSplines solves the tridiagonal system and rewrites each segment as its coefficients
// (cCSICubicSpline, 5 floats: d, c, b, a, x); evaluate(x) finds the segment (starting at LastEvaluatedIndex) and
// calls cCSICubicSpline_evaluate. The camera algorithms embed them (cSplineCamAlgorithm: 3; cLookAtChaseAlgorithm: 5
// at +0x304..+0x334). EA's source: d:/ssx/code/source/bx/cubicspline.cpp and cubicspline.h (preview asserts); retail
// methods: seg/seg_21E5A8 (ctor, dtor; 0x0031D5E8..0x0031D660) and bx/cubicspline (0x0031D660..0x0031E1C0). No vtable
// (the dtor is non-virtual: called directly).
// Size 0xC: [R] cLookAtChaseAlgorithm_cLookAtChaseAlgorithm constructs five back to back at +0x304, +0x310, +0x31C,
// +0x328, +0x334 (stride 0xC); the ctor writes words 0x0 and 0x8, the methods touch nothing past 0x8; [O] On Tour's
// cCubicSplineInterpolant is 0xC with the same members. No base class.
//
// A FLAT layout, like main/game.h. Word 0x0 holds EA's six bitfields (On Tour: unsigned int Type : 1 ... ; LSB first,
// as the retail masks show), declared directly as in EA's class; CHECK_OFFSET cannot name a bitfield, so the word is
// pinned by CHECK_OFFSET(MaxX, 0x4). Evidence tags:
//   [R] the ctor cCubicSplineInterpolant_cCubicSplineInterpolant (CubicSplines = 0; Initialized, Loaded,
//       NumSplines = 0) and the camera ctors above;
//   [U] how retail code uses the field (src/bx/cubicspline.cpp: views sCubicSpline / sCubicSplineFlags in
//       initCommon / calculateSplines, this class in addPoint / evaluate, raw word masks; src/seg/seg_21E5A8.cpp:
//       Flags_D5E8);
//   [O] SSX On Tour's DWARF (class cCubicSplineInterpolant: Type : 1, Initialized : 1, Loaded : 1, LoadingIndex : 8,
//       NumSplines : 8, LastEvaluatedIndex : 8, MaxX 0x4, CubicSplines 0x8).

#include "layout.h"

// cCSICubicSpline: one segment of the interpolant, y = A t^3 + B t^2 + C t + D with t = x - X0 (0x14).
// [O] On Tour struct cCSICubicSpline {float A, B, C, D, X0} (0x14) with evaluate(float x); [R] the retail array is
// NumSplines * 0x14 bytes ("cCSICubicSpline", initCommon); [U] cCSICubicSpline_evaluate (seg/seg_21E5A8) reads X0 at
// 0x10 and the coefficients at 0x0..0xC; cCubicSplineInterpolant_addPoint / calcCoeffs fill them.
struct cCSICubicSpline {
    float A;                        // 0x00 [O]; [U] evaluate: the t^3 coefficient
    float B;                        // 0x04 [O]; [U] the t^2 coefficient
    float C;                        // 0x08 [O]; [U] the t coefficient
    float D;                        // 0x0C [O]; [U] the constant
    float X0;                       // 0x10 [O]; [U] evaluate subtracts it from x
};

CHECK_OFFSET(cCSICubicSpline, A, 0x00);
CHECK_OFFSET(cCSICubicSpline, B, 0x04);
CHECK_OFFSET(cCSICubicSpline, C, 0x08);
CHECK_OFFSET(cCSICubicSpline, D, 0x0C);
CHECK_OFFSET(cCSICubicSpline, X0, 0x10);
CHECK_SIZE(cCSICubicSpline, 0x14);


struct cCubicSplineInterpolant {
    unsigned int Type : 1;          // 0x000 bit 0 [O]; [U] initAsNatural clears it (& -2), initAsClamped sets it (| 1);
                                    //       calculateSplines switches on it (0 natural, 1 clamped)
    unsigned int Initialized : 1;   // 0x000 bit 1 [O]; [R] ctor 0; [U] initCommon sets it, purge clears it (& -3)
    unsigned int Loaded : 1;        // 0x000 bit 2 [O]; [R] ctor 0; [U] addPoint sets it after the last key, purge
                                    //       clears it (& -5)
    unsigned int LoadingIndex : 8;  // 0x000 bits 3-10 [O]; [U] initCommon 0; addPoint counts the keys added
    unsigned int NumSplines : 8;    // 0x000 bits 11-18 [O]; [R] ctor 0; [U] initCommon = n - 1; addPoint /
                                    //       calculateSplines / evaluate / SpewSplineNumbers read it
    unsigned int LastEvaluatedIndex : 8; // 0x000 bits 19-26 [O]; [U] evaluate's cached segment; calculateSplines
                                    //       clears it (& 0xF807FFFF)
    float MaxX;                     // 0x004 [O]; [U] initAsClamped parks the end slope here, calculateSplines reads it
                                    //       (clamped) and then stores the last key's x; evaluate / SpewGraphPoints use it
                                    //       as the end of the last segment
    cCSICubicSpline* CubicSplines;  // 0x008 [O]; [R] ctor 0; [U] initCommon allocates NumSplines * 0x14 bytes
                                    //       ("cCSICubicSpline"), purge cMemMan_free's and zeroes it; addPoint /
                                    //       calculateSplines / evaluate / initAsClamped (start slope at +0x10) use it
};

CHECK_OFFSET(cCubicSplineInterpolant, MaxX, 0x004);
CHECK_OFFSET(cCubicSplineInterpolant, CubicSplines, 0x008);
CHECK_SIZE(cCubicSplineInterpolant, 0xC);

#endif /* BX_CUBICSPLINE_H */
