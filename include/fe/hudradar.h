#ifndef FE_HUDRADAR_H
#define FE_HUDRADAR_H

// cHUDRadar: the in-race course-progress tracker of the HUD (a vertical strip with the riders' and course markers'
// icons placed by distance; the 1-player view has a "trophy box" text line, split screen a thin line per panel). EA's
// source fe/hudradar.cpp (preview assert strings, e.g. "radarLoc.H > (messageHeight + (offScreenArrowHeight * 2.0f))";
// retail code sits in the decomp's unit fe/ovstatetrophy). Name: the preview's vtable section _vt$9cHUDRadar pairs with
// retail cHUDRadar_vtbl (builds/x/v2/vtables_v2.tsv) and its slot 1 (preview 0x001D1930) matches the retail destructor
// cHUDRadar__cHUDRadar; the preview also keeps the static inlines IsRadarShown__9cHUDRadar / ShowRadar__9cHUDRadarb (a gp
// global, not a member). On Tour has no cHUDRadar.
//
// Size 0x3C: one per rider info of cOVStateHUD1P (fe/ovstatehud.h: char unk_08[0x3C] at +0x8 of each tHUDRiderInfo,
// built by the constructor cHUDRadar_cHUDRadar, which stores the vptr at +0x38 - the last word - and counts live instances
// in D_004A276C; the destructor cHUDRadar__cHUDRadar undoes both). No allocation of its own. No base class; the class
// declares a virtual destructor, so g++ 2.95 puts the vptr after its data (0x38).
// Methods: ctor cHUDRadar_cHUDRadar, dtor cHUDRadar__cHUDRadar (vtable slot 1, the only slot), init/layout cHUDRadar_init (loads
// the shared icons and font, then cHUDRadar_calcLayout lays it out), 1-player render cHUDRadar_renderTracker, split-screen render
// cHUDRadar_renderSplitTracker. updateRadarRiders and setRadarRiderResetFlag ... resetRadarRiders are statics (course progress of all riders).
//
// A FLAT layout, like main/game.h. No member has a name source (the assert names radarLoc / messageHeight /
// offScreenArrowHeight may be locals), so all are unk_XXX. Evidence tags: [R] retail ctor / dtor; [U] retail usage
// (asm of cHUDRadar_calcLayout / cHUDRadar_renderTracker / cHUDRadar_renderSplitTracker; their C views sTrophyBox_EDA0 / sTrophyBox_FB40 /
// sTrophyBox_20EC18 in src/fe/ovstatetrophy.cpp); [P] the preview build.

#include "layout.h"
#include "render/2drender.h"   // tRect


struct cHUDRadar {
    float unk_000[2];                // 0x000 [U] text position x, y (On Tour t2Vector shape; no shared definition in
                                     //       include/ yet): cHUDRadar_calcLayout stores it as one unaligned doubleword
                                     //       (sdl/sdr); cHUDRadar_renderTracker prints the trophy line there
    float unk_008;                   // 0x008 [U] strip top (cHUDRadar_calcLayout swc1; cHUDRadar_renderTracker lwc1)
    float unk_00C;                   // 0x00C [U] strip bottom (ditto)
    float unk_010;                   // 0x010 [U] strip centre x (ditto)
    float unk_014;                   // 0x014 [U] top of the bar below the top arrow (ditto)
    float unk_018;                   // 0x018 [U] bottom arrow row (ditto)
    float unk_01C;                   // 0x01C [U] pixels per course unit (strip height / 10000; ditto)
    tRect unk_020;                   // 0x020 [U] the tracker rectangle: cHUDRadar_calcLayout copies its area argument (struct
                                     //       copy, sdl/sdr), cHUDRadar_renderSplitTracker reads X / Y / W / H. Possibly the assert's
                                     //       radarLoc (".H")
    float unk_030;                   // 0x030 [U] rectangle h / 10000: split-screen pixels per unit (cHUDRadar_calcLayout swc1;
                                     //       cHUDRadar_renderSplitTracker lwc1)
    int unk_034;                     // 0x034 [U] the viewer's AI index from the last frame: both renders compare and
                                     //       store it (lw/sw)
    void** mVtbl;                    // 0x038 [R] cHUDRadar_cHUDRadar stores cHUDRadar_vtbl (1 slot), cHUDRadar__cHUDRadar restores it;
                                     //       [P] _vt$9cHUDRadar
};

CHECK_OFFSET(cHUDRadar, unk_000, 0x000);
CHECK_OFFSET(cHUDRadar, unk_008, 0x008);
CHECK_OFFSET(cHUDRadar, unk_00C, 0x00C);
CHECK_OFFSET(cHUDRadar, unk_010, 0x010);
CHECK_OFFSET(cHUDRadar, unk_014, 0x014);
CHECK_OFFSET(cHUDRadar, unk_018, 0x018);
CHECK_OFFSET(cHUDRadar, unk_01C, 0x01C);
CHECK_OFFSET(cHUDRadar, unk_020, 0x020);
CHECK_OFFSET(cHUDRadar, unk_030, 0x030);
CHECK_OFFSET(cHUDRadar, unk_034, 0x034);
CHECK_OFFSET(cHUDRadar, mVtbl, 0x038);
CHECK_SIZE(cHUDRadar, 0x3C);

#endif /* FE_HUDRADAR_H */
