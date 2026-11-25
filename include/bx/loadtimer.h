#ifndef BX_LOADTIMER_H
#define BX_LOADTIMER_H

// cLoadTimer: the load-progress timer (cAppMan::LoadTimer, bx/appman): the shown and target percentages, load time,
// target tick and the offset / scale / decay of the bar. Methods: startload, setnextpercent, update, endload.
//
// Size 0x20 (On Tour's; it fills cAppMan 0x38-0x57, whose LoadState follows at 0x58). A FLAT layout, like main/game.h:
// member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this`
// in the methods falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"


struct cLoadTimer {
    float loadpercent;              // 0x000 [O]
    float lasttarget;               // 0x004 [O]
    float target;                   // 0x008 [O]
    float loadtime;                 // 0x00C [O]
    int targettick;                 // 0x010 [O]
    float offset;                   // 0x014 [O]
    float scale;                    // 0x018 [O]
    float decay;                    // 0x01C [O]

    // Methods bound to their retail symbols by asm labels.
    void startload(float rateScale) __asm__("cLoadTimer_startload__FPvf");
    void setnextpercent(float newValue) __asm__("cLoadTimer_setnextpercent__FPvf");
    void update() __asm__("cLoadTimer_update__FPv");
    void endload() __asm__("cLoadTimer_endload__FPv");
};

CHECK_OFFSET(cLoadTimer, loadpercent, 0x000);
CHECK_OFFSET(cLoadTimer, lasttarget, 0x004);
CHECK_OFFSET(cLoadTimer, target, 0x008);
CHECK_OFFSET(cLoadTimer, loadtime, 0x00C);
CHECK_OFFSET(cLoadTimer, targettick, 0x010);
CHECK_OFFSET(cLoadTimer, offset, 0x014);
CHECK_OFFSET(cLoadTimer, scale, 0x018);
CHECK_OFFSET(cLoadTimer, decay, 0x01C);
CHECK_SIZE(cLoadTimer, 0x20);

#endif
