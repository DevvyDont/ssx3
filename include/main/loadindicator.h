#ifndef MAIN_LOADINDICATOR_H
#define MAIN_LOADINDICATOR_H

// cLoadIndicator: the pulsing load icon drawn by the pre-start load screens (cSSXApp::LoadIndicator). Methods in
// main/gameload: cLoadIndicator_update (advances phase), cLoadIndicator_render (draws the "loadicon" sprite).
// Size 0x1C: [R] cSSXApp_init: cMemMan_alloc(0x1C, "LoadIndicator") with the inline ctor (view sTint_6900 in
// main/ssxapp: three zero words, then a colour of (1, 1, 1, 1)); [O] On Tour class cLoadIndicator {float phase; float
// alpha; unsigned int locale; tARGB colour} (0x20 there: On Tour aligns tARGB to 16, retail's tARGB is 4-aligned).
// Tags: [O] On Tour's DWARF; [R] retail allocation / ctor; [U] retail use.

#include "layout.h"
#include "ui/uithing.h"     // tARGB

struct cLoadIndicator {
    float phase;                    // 0x00 [O]; [R] ctor 0; [U] cLoadIndicator_update adds 1/60 and wraps it to [0, 1);
                                    //      cLoadIndicator_render's pulse is cos(phase x 2 pi)
    float alpha;                    // 0x04 [O]; [R] ctor 0; [U] the load screens fade it by 1/12 per frame
                                    //      (cInitLoadScreen_update ...; loseFocus waits for 0); render copies it to colour.A
    int unk_08;                     // 0x08 [O] locale (unsigned int); [R] ctor 0; no retail use seen
    tARGB colour;                   // 0x0C [O]; [R] ctor (1, 1, 1, 1); [U] render passes it to DrawSprite
};

CHECK_OFFSET(cLoadIndicator, phase, 0x00);
CHECK_OFFSET(cLoadIndicator, alpha, 0x04);
CHECK_OFFSET(cLoadIndicator, unk_08, 0x08);
CHECK_OFFSET(cLoadIndicator, colour, 0x0C);
CHECK_SIZE(cLoadIndicator, 0x1C);

#endif /* MAIN_LOADINDICATOR_H */
