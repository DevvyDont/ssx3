#ifndef BX_STATSCALC_H
#define BX_STATSCALC_H

// cStatsCalc: running statistics of a series of float samples - sum, sum of squares, minimum, maximum and count
// (add). Its one retail method, cStatsCalc_add, has no caller in retail code (a library leftover). No EA path in the
// preview (no assert or accessor names it); On Tour compiles it in its bx unit (z:/main/game/source/bx/
// all_in_one_bx.cpp), and retail keeps cStatsCalc_add in unit seg/seg_218AE8 between bx code (cBxPseudoRng_NextInt,
// tSemiSpace_set), so the header goes under bx/.
//
// Size 0x14: [O] On Tour's cStatsCalc is 0x14; [U] add's last member is the word at 0x10. No allocation or embedding
// in retail to confirm it. No base classes, no vtable.
// Tags: [U] cStatsCalc_add (src/seg/seg_218AE8.cpp: lwc1/swc1 on 0x0..0xC, lw/sw on 0x10); [O] SSX On Tour's DWARF
// (class cStatsCalc: same names, types, order and offsets).

#include "layout.h"

struct cStatsCalc {
    float Sum;                      // 0x00 [O]; [U] add: Sum += x
    float SumSquare;                // 0x04 [O]; [U] add: SumSquare += x * x
    float MinSample;                // 0x08 [O]; [U] add: = x on the first sample, else lowered to x
    float MaxSample;                // 0x0C [O]; [U] add: = x on the first sample, else raised to x
    int Samples;                    // 0x10 [O]; [U] add: 0 = first sample; incremented

    // Methods bound to their retail symbols by asm labels.
    void add(float x) __asm__("cStatsCalc_add");
};

CHECK_OFFSET(cStatsCalc, Sum, 0x00);
CHECK_OFFSET(cStatsCalc, SumSquare, 0x04);
CHECK_OFFSET(cStatsCalc, MinSample, 0x08);
CHECK_OFFSET(cStatsCalc, MaxSample, 0x0C);
CHECK_OFFSET(cStatsCalc, Samples, 0x10);
CHECK_SIZE(cStatsCalc, 0x14);

#endif /* BX_STATSCALC_H */
