#ifndef BE_BEINTREWARD_H
#define BE_BEINTREWARD_H

// cBERewardInterface: the back end's rewards interface (trophies, medals, unlockables, songs). One instance, created
// on demand by cBERewardInterface_getThis (`cMemMan_alloc(0x18, "BERewardInt", 0, 0)`) into
// cBERewardInterface_instance; the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 13). EA's source was
// d:/ssx/code/source/be/beintreward.cpp (preview build assert path).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet.
// What the real class looked like: `class cBERewardInterface : cBEInterface` (On Tour). cBEInterface is the base of
// every BE interface (BEBoltInt, cBENewPlayerInterface, ...): three bools, then the vptr. With PS2 bools being 4 bytes
// that is 0x0/0x4/0x8 + vptr at 0xC = 0x10, which is the whole object in the July 2003 preview (getThis allocates 0x10
// there) and the size of the other BE interfaces in retail. Retail adds the last-event pair at 0x10; On Tour has the
// same two members first in the class (at 0x8 after its 8-byte base), followed by the mEventUnlockables /
// mEventAccomplishments arrays that retail does not have (On Tour size 0x298).
// cBERewardInterface overrides no virtual: its getThis stores the same vtable (cBEInterface_vtbl) as BEBoltInt's
// (cBEBoltInterface_getThis), whose slots are cBEInterface's own.
//
// Evidence tags:
//   [R] retail's own constructor (the inlined body in cBERewardInterface_getThis) and allocation tag;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface / cBERewardInterface), member order and names.
// Methods use `self` only as a handle (they read the BE library globals), so the fields are reached mostly through
// cBERewardInterface_instance or the cBE_getInterface(.., 13) pointer.

#include "layout.h"
#include "be/beinterface.h"

struct cBERewardInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bCanSubmit: (On Tour: a bool at 0x0); no retail access seen
    //   bDataChanged: (On Tour: a bool at 0x1); no retail access seen
    //   bIsActive: (On Tour: a bool at 0x2); [R] getThis sets 0; [U] set 0/1 on every BE interface together
    //       (cBERaceState_onExit/cBERaceState_onEnter,
    //       cBEPostRaceState_onExit/cBEPostRaceState_onEnter/cBEPreRaceState_onExit/cBEPreRaceState_onEnter)
    //   the vptr (0x0C): [R] getThis stores cBEInterface_vtbl (cBEInterface's vtable, shared with BEBoltInt)
    // cBERewardInterface
    unsigned int mLastEventCashReward; // 0x010 [O] (On Tour 0x008, first own member); [U] cBERewardInterface_calculateUserTrackMedalRewards stores the cash
                                     //       from cBEEconInterface_getCashReward (halved when the medal isn't better); clearUserAwards
                                     //       zeroes it; cOVStateManager_gotoOverlay/cOVStateManager_gotoNextOverlay test it != 0
    int mLastEventMedal;             // 0x014 [O] eBEMedalID (On Tour 0x00C); [U] cBERewardInterface_calculateUserTrackMedalRewards stores the event's medal;
                                     //       clearUserAwards sets -1 (no medal, as in isBetterMedal); compared with
                                     //       0..3 by cOVState_RACE_FINISHOV_onCreateScreen/cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen, ==0 by func_002A3860, read by
                                     //       cOVState_REWARDS_checkRewardList (asm)
};

CHECK_OFFSET(cBERewardInterface, mLastEventCashReward, 0x10);
CHECK_OFFSET(cBERewardInterface, mLastEventMedal, 0x14);
CHECK_SIZE(cBERewardInterface, 0x18);

// The one instance (On Tour: cBERewardInterface::instance), created by cBERewardInterface_getThis.
extern cBERewardInterface* cBERewardInterface_instance;

#endif /* BE_BEINTREWARD_H */
