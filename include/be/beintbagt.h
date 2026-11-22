#ifndef BE_BEINTBAGT_H
#define BE_BEINTBAGT_H

// cBEBAGTInterface: the back end's grab/tweak/uber-trick interface (On Tour's name; its methods: getGrabAnim,
// getGrabCycleAnim, getGrabName, getTweak*, getUber*, getGrabPoints, getGrabPointsPerSec, chooseUberVar) - which
// animation, name and points each grab input gives a rider, and each rider's uber-trick assignments (the uber-trick
// screen, cFEStateUberTrick_*, edits them through func_0014FD80 ... func_00150048). The object holds no trick data:
// like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this`
// and work on the grab table kGrabDB (0x14-byte records) and the riders' staged trick records D_00530EC0. One
// instance, created on demand by cBEBAGTInterface_getThis
// (`cMemMan_alloc(0x10, "BEBAGTInt", 0, 0)`, vtable cBEBAGTInterface_vtbl, a virtual call of slot 2 (discard), then
// bIsActive = 0) into cBEBAGTInterface::instance (retail cBEBAGTInterface_instance, D_004A1210); the rest of the game
// reaches it as cBE_getInterface(cBE_getBE(), 6). EA's source: d:/ssx/code/source/be/beintbagt.cpp (preview assert
// string).
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEBAGTInterface
// (": cBEInterface", size 0x8) has no data members of its own, and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// cBEBAGTInterface_vtbl (0x45ACD8), slot 1 submit = cBEBAGTInterface_submit, slot 2 discard = cBEBAGTInterface_discard,
// slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, vptr, bIsActive) and constructor cBEBAGTInterface_cBEBAGTInterface (vptr);
//   [P] the July 2003 preview's inline methods __16cBEBAGTInterface (stores the vtable at 0xC) and setBAGTActive;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"

struct cBEBAGTInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [P] setBAGTActive;
                                     //       [U] cBEBAGTInterface_setBAGTActive (static: instance->bIsActive = arg);
                                     //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1
    void** mVtbl;                    // 0x00C [R] getThis and cBEBAGTInterface_cBEBAGTInterface store
                                     //       cBEBAGTInterface_vtbl; [P] __16cBEBAGTInterface; [U]
                                     //       cFEStateUberTrick_onWidgetEvent / onPopupEvent call slot 1 (submit)
                                     //       through it; getThis, cBELibrary_newProfile, cBELibrary_readFromData,
                                     //       cBENetworkInterface_restoreCharProfile / setSyncData,
                                     //       cBESaveInterface_generateSaveProfile / loadSaveProfile call slot 2
                                     //       (discard)
};

extern cBEBAGTInterface* cBEBAGTInterface_instance;

CHECK_OFFSET(cBEBAGTInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEBAGTInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEBAGTInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEBAGTInterface, mVtbl, 0x0C);
CHECK_SIZE(cBEBAGTInterface, 0x10);

// The one BAGT interface (On Tour: cBEBAGTInterface::instance), created by cBEBAGTInterface_getThis.

// One uber-trick variant of a grab (On Tour struct tBEUber, 0x8: four unsigned shorts). [O] On Tour's
// cBEBAGTInterface::getUber* family; [U] func_0014FF90 returns a grab's table of them from kGrabDB (pointer +0x10,
// count +0xC); cFEStateUberTrick_updateTrickMenuEntry (view sWSMEntry4520) shows the selected entry's cost (lhu +0x6)
// with getDollarString in "cost_num".
struct tBEUber {
    unsigned short unk_000;         // 0x0 [O] anim
    unsigned short unk_002;         // 0x2 [O] cycleAnim
    unsigned short unk_004;         // 0x4 [O] id
    unsigned short cost;            // 0x6 [O]; [U] the trick's price on the uber-trick screen
};

CHECK_OFFSET(tBEUber, unk_000, 0x0);
CHECK_OFFSET(tBEUber, unk_002, 0x2);
CHECK_OFFSET(tBEUber, unk_004, 0x4);
CHECK_OFFSET(tBEUber, cost, 0x6);
CHECK_SIZE(tBEUber, 0x8);

#endif /* BE_BEINTBAGT_H */
