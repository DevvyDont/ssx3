#ifndef FE_FESOUND_H
#define FE_FESOUND_H

// cFESound / cOVSound: the front end's and the in-game overlays' UI sound interfaces (cUISoundInterface subclasses
// the UI engine calls through cUIEngine::mpSound, ui/uiengine.h): playSound(int e) maps a UI sound event to a sound,
// soundInitialize / soundShutdown do nothing. Names, two signals each: SSX On Tour's DWARF has both classes (size 0x8,
// one member mDummyVar, methods playSound / soundInitialize / soundShutdown), and the preview's vtable sections
// _vt$8cFESound / _vt$8cOVSound pair with the retail vtables 0x00469348 / 0x00469318 (builds/x/v2/vtables_v2.tsv); the
// retail functions already carry these names.
// Size 0x8: [R] cMemMan_alloc(8, "FEUISound") + cFESound_cFESound (fe/fereal) and cMemMan_alloc(8, "OVUISound") +
// cOVSound_cOVSound (main/gameload); [O] On Tour 0x8. Base cUISoundInterface (On Tour size 0x4: no data, only the
// vptr at 0x0). Vtables (4 slots): 1 dtor (cFESound__cFESound 0x001E0740 / cOVSound__cOVSound 0x001E0780;
// fe/fepopupmisc), 2 playSound
// (cFESound_playSound 0x001A2E58 / cOVSound_playSound 0x001A2F70), 3 soundInitialize, 4 soundShutdown.
// EA's source file is not known (On Tour lists them in fe's all_in_one unit); retail code in fe/fereal and
// fe/fepopupmisc.
//
// FLAT layouts. Evidence tags: [R] retail ctor / dtor / allocation; [O] SSX On Tour's DWARF; [P] the preview build.

#include "layout.h"


struct cFESound {
    // --- cUISoundInterface (On Tour: size 0x4, the vptr only)
    void** mVtbl;                    // 0x000 [R] cFESound_cFESound stores cFESound_vtbl (0x00469348) after
                                     //       cUISoundInterface_cUISoundInterface; the dtor stores cUISoundInterface's
                                     //       table cUISoundInterface_vtbl back; [P] _vt$8cFESound
    // --- cFESound
    int mDummyVar;                   // 0x004 [O] (0x4 in On Tour too); [R] cFESound_cFESound zeroes it (sw $0, 0x4); no
                                     //       other access
};

CHECK_OFFSET(cFESound, mVtbl, 0x000);
CHECK_OFFSET(cFESound, mDummyVar, 0x004);
CHECK_SIZE(cFESound, 0x8);

struct cOVSound {
    // --- cUISoundInterface (On Tour: size 0x4, the vptr only)
    void** mVtbl;                    // 0x000 [R] cOVSound_cOVSound stores cOVSound_vtbl (0x00469318) after
                                     //       cUISoundInterface_cUISoundInterface; the dtor stores cUISoundInterface's
                                     //       table cUISoundInterface_vtbl back; [P] _vt$8cOVSound
    // --- cOVSound
    int mDummyVar;                   // 0x004 [O] (0x4 in On Tour too); [R] cOVSound_cOVSound zeroes it (sw $0, 0x4); no
                                     //       other access
};

CHECK_OFFSET(cOVSound, mVtbl, 0x000);
CHECK_OFFSET(cOVSound, mDummyVar, 0x004);
CHECK_SIZE(cOVSound, 0x8);

#endif /* FE_FESOUND_H */
