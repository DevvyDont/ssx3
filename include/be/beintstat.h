#ifndef BE_BEINTSTAT_H
#define BE_BEINTSTAT_H

// cBEStatInterface: the back end's rider-attribute interface - each rider's and each human profile's attribute levels
// (speed, acceleration, trick, edging, spin, toughness, stability, the stat level) and the physics values derived from
// them (getBrakingAttrib ... getTrickAttrib, shouldCelebrateTrick). The object holds no stats: like every BE interface
// it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and work on the static
// attribute table cBEStatInterface_currentStat (probably On Tour's cBEStatInterface::currentStat; 7-byte rows per profile and character,
// which submit / discard copy to and from the profile library) and the profile library. One instance, created on demand
// by cBEStatInterface_getThis (`cMemMan_alloc(0x10, "BECharInt", 0, 0)` - EA reused cBECharacterInterface's tag -
// bIsActive = 0, vtable cBEStatInterface_vtbl, then a virtual call of slot 2, discard) into cBEStatInterface::instance
// (retail cBEStatInterface_instance, D_004A11C4); the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 3).
// EA's source: d:/ssx/code/source/be/beintstat.cpp (preview assert string); in retail getThis ... getEdgingAttrib sit
// in the be/beintplayer unit (0x147F78-0x148F4F), getCollisionAttrib ... shouldCelebrateTrick in be/beintstat, and
// purgeInterface / setStatActive / the constructor in be/bedbreward.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEStatInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBEStatInterface_vtbl (0x45AD68), slot 1 submit = cBEStatInterface_submit, slot 2 discard = cBEStatInterface_discard,
// slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr) and constructor cBEStatInterface_cBEStatInterface (vptr);
//   [P] the July 2003 preview's inline methods __16cBEStatInterface (stores the vtable at 0xC) and setStatActive;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"
#include "be/beinterface.h"

struct cBEStatInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] setStatActive; [U] cBEStatInterface_setStatActive (static:
    //       instance->bIsActive = arg); cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis and cBEStatInterface_cBEStatInterface store cBEStatInterface_vtbl; [P]
    //       __16cBEStatInterface; [U] slot 2 (discard) is called through it by getThis, cBEEconInterface_buyAttribute,
    //       cBELibrary_newProfile, cBELibrary_readFromData, cBENetworkInterface_postInitBE,
    //       cBESaveInterface_generateSaveProfile / loadSaveProfile and cGameComm_gameExit
};

extern cBEStatInterface* cBEStatInterface_instance;

CHECK_SIZE(cBEStatInterface, 0x10);

// The one stat interface (On Tour: cBEStatInterface::instance), created by cBEStatInterface_getThis.

#endif /* BE_BEINTSTAT_H */
