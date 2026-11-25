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
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
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
#include "be/beinterface.h"

struct cBEBAGTInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] setBAGTActive; [U] cBEBAGTInterface_setBAGTActive (static:
    //       instance->bIsActive = arg); cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis and cBEBAGTInterface_cBEBAGTInterface store cBEBAGTInterface_vtbl; [P]
    //       __16cBEBAGTInterface; [U] cFEStateUberTrick_onWidgetEvent / onPopupEvent call slot 1 (submit) through it;
    //       getThis, cBELibrary_newProfile, cBELibrary_readFromData, cBENetworkInterface_restoreCharProfile /
    //       setSyncData, cBESaveInterface_generateSaveProfile / loadSaveProfile call slot 2 (discard)

    // Methods bound to their retail symbols by asm labels.
    int chooseUberVar(int rider, int idx, int second) __asm__("cBEBAGTInterface_chooseUberVar");
    unsigned short getGrabAnim(int rider, int grabIndex) __asm__("cBEBAGTInterface_getGrabAnim");
    unsigned short getGrabCycleAnim(int rider, int grabIndex) __asm__("cBEBAGTInterface_getGrabCycleAnim");
    unsigned short getGrabName(int rider, int grabIndex) __asm__("cBEBAGTInterface_getGrabName");
    unsigned short getTweakAnim(int rider, int grabIndex) __asm__("cBEBAGTInterface_getTweakAnim");
    unsigned short getTweakCycleAnim(int rider, int grabIndex) __asm__("cBEBAGTInterface_getTweakCycleAnim");
    unsigned short getTweakName(int rider, int grabIndex) __asm__("cBEBAGTInterface_getTweakName");
    int getUberAnim(int rider, int idx, int second) __asm__("cBEBAGTInterface_getUberAnim");
    int getUberCycleAnim(int rider, int idx, int second) __asm__("cBEBAGTInterface_getUberCycleAnim");
    int getUberName(int rider, int idx, int second) __asm__("cBEBAGTInterface_getUberName");
};

extern cBEBAGTInterface* cBEBAGTInterface_instance;

CHECK_SIZE(cBEBAGTInterface, 0x10);

// The one BAGT interface (On Tour: cBEBAGTInterface::instance), created by cBEBAGTInterface_getThis.

// One uber-trick variant of a grab (On Tour struct tBEUber, 0x8: four unsigned shorts). [O] On Tour's
// cBEBAGTInterface::getUber* family; [U] func_0014FF90 returns a grab's table of them from kGrabDB (pointer +0x10,
// count +0xC); cFEStateUberTrick_updateTrickMenuEntry (view sWSMEntry4520) shows the selected entry's cost (lhu +0x6)
// with getDollarString in "cost_num".
struct tBEUber {
    unsigned short anim;            // 0x0 [O]; [U] the uber-trick screen plays it when the trick is previewed
    unsigned short cycleAnim;       // 0x2 [O]
    unsigned short id;              // 0x4 [O]; [U] the trick id (gGetGrabName gives its name for the buy popup)
    unsigned short cost;            // 0x6 [O]; [U] the trick's price on the uber-trick screen
};

CHECK_OFFSET(tBEUber, anim, 0x0);
CHECK_OFFSET(tBEUber, cycleAnim, 0x2);
CHECK_OFFSET(tBEUber, id, 0x4);
CHECK_OFFSET(tBEUber, cost, 0x6);
CHECK_SIZE(tBEUber, 0x8);

// One grab's record in kGrabDB (On Tour struct tBEGrabDB, 0x14; [O] `const tBEGrabDB kGrabDB[7]`, retail has 15
// entries). [U] cBEBAGTInterface_getGrabAnim ... getTweakName return the six halfwords; chooseUberVar,
// func_0014FF90 and func_00150558 read numUbers; func_0014FF90 and the getUber* getters read ubers.
struct tBEGrabDB {
    unsigned short grabAnim;        // 0x00 [O]; [U] getGrabAnim: the start animation
    unsigned short grabCycleAnim;   // 0x02 [O]; [U] getGrabCycleAnim: the held animation (438 = none)
    unsigned short grabID;          // 0x04 [O]; [U] getGrabName: the trick id scored for the grab
    unsigned short tweakAnim;       // 0x06 [O]; [U] getTweakAnim
    unsigned short tweakCycleAnim;  // 0x08 [O]; [U] getTweakCycleAnim (438 = none)
    unsigned short tweakID;         // 0x0A [O]; [U] getTweakName: the trick id scored for the tweak
    unsigned short numUbers;        // 0x0C [O]; [U] chooseUberVar (0 = no uber variants), func_0014FF90
    unsigned short pad_0x0E;
    const tBEUber* ubers;           // 0x10 [O]; [U] func_0014FF90 returns it, getUber* index it by the chosen variant
};

CHECK_OFFSET(tBEGrabDB, grabAnim, 0x00);
CHECK_OFFSET(tBEGrabDB, grabCycleAnim, 0x02);
CHECK_OFFSET(tBEGrabDB, grabID, 0x04);
CHECK_OFFSET(tBEGrabDB, tweakAnim, 0x06);
CHECK_OFFSET(tBEGrabDB, tweakCycleAnim, 0x08);
CHECK_OFFSET(tBEGrabDB, tweakID, 0x0A);
CHECK_OFFSET(tBEGrabDB, numUbers, 0x0C);
CHECK_OFFSET(tBEGrabDB, ubers, 0x10);
CHECK_SIZE(tBEGrabDB, 0x14);

// The grab table (On Tour: const tBEGrabDB kGrabDB[]), indexed by grab.
extern const tBEGrabDB kGrabDB[];

// A grab's points (On Tour struct tBEGrabPointsDB, 0x8). [U] cBELibrary_loadData fills cBELibrary_grabPointsDB
// (85 entries); cBEBAGTInterface_getGrabPoints / getGrabPointsPerSec read them.
struct tBEGrabPointsDB {
    int pointsPerGrab;              // 0x0 [O]; [U] getGrabPoints
    int pointsPerSec;               // 0x4 [O]; [U] getGrabPointsPerSec
};

CHECK_OFFSET(tBEGrabPointsDB, pointsPerGrab, 0x0);
CHECK_OFFSET(tBEGrabPointsDB, pointsPerSec, 0x4);
CHECK_SIZE(tBEGrabPointsDB, 0x8);

#endif /* BE_BEINTBAGT_H */
