#ifndef BE_BEINTAGGRESSION_H
#define BE_BEINTAGGRESSION_H

// cBEAggressionInterface: the back end's rider-aggression interface - how each rider feels about the others (getState:
// friend / neutral / rival / enemy) and how hard they go after them (getAggression: none ... chase), raised by bumps
// and punches (applyAggression, from cRider_flagEventBump / cRider_flagEventPunchKnockdown), cooled down between runs
// (regressAggression) and written to / read from replays (writeToReplayFrame / readFromReplayFrame). The object holds
// no relations: like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods
// ignore `this` and work on the 3-byte relation records {state, level, ticks} in each character profile
// (cBELibrary_profileLib[profile][character] + 0xBC1, 16 records; On Tour tBERelationDB {aggState, aggLevel, tick},
// which On Tour keeps in the object instead as mAggression[4][4]). One instance, created on demand by
// cBEAggressionInterface_getThis (`cMemMan_alloc(0x10, "BEAggressionInt", 0, 0)`, bIsActive = 0, vtable
// cBEAggressionInterface_vtbl) into cBEAggressionInterface::instance (retail cBEAggressionInterface_instance,
// D_004A124C); the rest of the game reaches it through getThis or as cBE_getInterface(cBE_getBE(), 12). EA's source:
// d:/ssx/code/source/be/beintaggression.cpp (preview assert string); in retail getThis ... readFromReplayFrame sit in
// the be/beintaggression unit (0x155A50-0x1562B7), purgeInterface / setAggressionActive / the constructor in
// be/bedbreward.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's
// cBEAggressionInterface (size 0x38) adds mAggression[4][4] at 0x8; retail's has no data members of its own.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBEAggressionInterface_vtbl (0x45AC48), slot 1 submit = cBEAggressionInterface_submit, slot 2 discard =
// cBEAggressionInterface_discard, slots 3/4 its own cBEAggressionInterface_writeToReplayFrame /
// cBEAggressionInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr) and constructor cBEAggressionInterface_cBEAggressionInterface
//       (vptr);
//   [P] the July 2003 preview's inline methods __22cBEAggressionInterface (stores the vtable at 0xC) and
//       setAggressionActive__22cBEAggressionInterfaceb (instance->bIsActive = arg);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"
#include "be/beinterface.h"

struct cBEAggressionInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] setAggressionActive; [U] cBEAggressionInterface_setAggressionActive
    //       (static: instance->bIsActive = arg); cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit,
    //       cBERaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis and cBEAggressionInterface_cBEAggressionInterface store
    //       cBEAggressionInterface_vtbl; [P] __22cBEAggressionInterface

    // Methods bound to their retail symbols by asm labels.
    int getAggression(int targetRider, int rider) __asm__("cBEAggressionInterface_getAggression");
    void applyAggression(int iRider1, int iRider2, int event) __asm__("cBEAggressionInterface_applyAggression");
    int getState(int targetRider, int rider) __asm__("cBEAggressionInterface_getState");
    // submit stays a C function (cBEAggressionInterface_submit): declared first, it is the vtable's key function, which
    // keeps the vtable out of the units defining the other overrides
    virtual void submit() __asm__("cBEAggressionInterface_submit__FPv");
    virtual void discard() __asm__("cBEAggressionInterface_discard__FPv");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cBEAggressionInterface_writeToReplayFrame");
    virtual void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cBEAggressionInterface_readFromReplayFrame");
};

CHECK_SIZE(cBEAggressionInterface, 0x10);

// The one aggression interface (On Tour: cBEAggressionInterface::instance), created by cBEAggressionInterface_getThis.
extern cBEAggressionInterface* cBEAggressionInterface_instance;

#endif /* BE_BEINTAGGRESSION_H */
