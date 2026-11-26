#ifndef BE_BEINTSAVEFILE_H
#define BE_BEINTSAVEFILE_H

// cBESaveInterface: the back end's memory-card save interface - builds and loads the game save (settings and high
// scores, 0xCCC bytes: generateSaveGame / loadSaveGame / getSaveGameSize) and the per-profile save (0x9B61 bytes:
// generateSaveProfile / loadSaveProfile / getSaveProfileSize), each ending in a CRC, and owns the 512 KB replay/save
// buffer (getReplayBuffer / getGameReplayBuffer / releaseReplayBuffer). The object holds no data: like every BE
// interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and work on the
// static members cBESaveInterface::buffer, ::replayBuffer and ::bDoReplay (retail cBESaveInterface_buffer D_004A1220,
// cBESaveInterface_replayBuffer D_004A1224, cBESaveInterface_bDoReplay D_004A1228; the units declare them with
// differing types, so they are not declared here; On Tour also has ::comparisonBuffer, retail does not). One
// instance, created on demand by cBESaveInterface_getThis (`cMemMan_alloc(0x10, "BESaveInt", 0, 0)`, bIsActive = 0,
// vtable cBESaveInterface_vtbl, then bDoReplay = 0) into cBESaveInterface::instance (retail cBESaveInterface_instance,
// D_004A121C); the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 5). EA's source:
// d:/ssx/code/source/be/beintsavefile.cpp (preview assert string); in retail getThis ... purgeInterface sit in the
// be/beintsavefile unit (0x152638-0x153087), submit / discard in be/bedbreward. Retail has no out-of-line constructor.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBESaveInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBESaveInterface_vtbl (0x0045AD08, named 2026-11-20), slot 1 submit = cBESaveInterface_submit, slot 2 discard =
// cBESaveInterface_discard, slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr);
//   [P] the July 2003 preview's getThis (0x151240: allocates 0x10, clears 0x8, stores _vt$16cBESaveInterface at
//       0xC); its only inline methods, submit / discard, are empty;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"
#include "be/beinterface.h"

struct cBESaveInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [P] the preview's getThis; [U] cBEPreRaceState_onEnter/onExit,
    //       cBEPostRaceState_onEnter/onExit, cBERaceState_onEnter/onExit set 0/1 (retail has no setSaveActive)
    //   the vptr (0x0C): [R] getThis stores cBESaveInterface_vtbl; [P] the preview's getThis stores
    //       _vt$16cBESaveInterface

    // Methods bound to their retail symbols by asm labels.
    void* generateSaveGame() __asm__("cBESaveInterface_generateSaveGame");
    void* generateSaveProfile(int idx) __asm__("cBESaveInterface_generateSaveProfile");
    bool loadSaveGame(char* buf) __asm__("cBESaveInterface_loadSaveGame");
    bool loadSaveProfile(char* buf, int idx) __asm__("cBESaveInterface_loadSaveProfile");
};

CHECK_SIZE(cBESaveInterface, 0x10);

// The one save interface (On Tour: cBESaveInterface::instance), created by cBESaveInterface_getThis.
extern cBESaveInterface* cBESaveInterface_instance;

#endif /* BE_BEINTSAVEFILE_H */
