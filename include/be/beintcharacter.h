#ifndef BE_BEINTCHARACTER_H
#define BE_BEINTCHARACTER_H

// cBECharacterInterface: the back end's character-bio interface - a rider's character name, short name, nickname,
// weight, stance (goofy), scale, blood type, sex, age, height, nationality and voice. The object holds no character
// data: like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore
// `this` and read the character table D_00530970 (0x88-byte records; On Tour: cBELibrary::characterDB) through the
// rider's character ID. One instance,
// created on demand by cBECharacterInterface_getThis (`cMemMan_alloc(0x10, "BECharInt", 0, 0)`, bIsActive = 0,
// vtable D_0045AD38) into cBECharacterInterface::instance (retail cBECharacterInterface_instance, D_004A1208); the
// rest of the game reaches it as cBE_getInterface(cBE_getBE(), 2). EA's source:
// d:/ssx/code/source/be/beintcharacter.cpp
// (preview assert string); in retail getThis and the name getters sit at the end of the be/beintnetwork unit, the rest
// in
// be/beintcharacter.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's
// cBECharacterInterface has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// D_0045AD38 (0x45AD38, unnamed in symbol_addrs), slot 1 submit = cBECharacterInterface_submit (empty), slot 2 discard
// =
// cBECharacterInterface_discard (empty), slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (the inlined constructor: allocation, bIsActive, vptr);
//   [P] the July 2003 preview (getThis allocates 0x10; submit__21cBECharacterInterface /
// discard__21cBECharacterInterface
//       are empty inline methods);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).
// be/beintnetwork.cpp already has a local `struct cBECharacterInterface` with exactly these four members.

#include "layout.h"

struct cBECharacterInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U]
                                     //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1
    void** mVtbl;                    // 0x00C [R] getThis stores D_0045AD38; no virtual call through it seen
};

extern cBECharacterInterface* cBECharacterInterface_instance;

CHECK_OFFSET(cBECharacterInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBECharacterInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBECharacterInterface, bIsActive, 0x08);
CHECK_OFFSET(cBECharacterInterface, mVtbl, 0x0C);
CHECK_SIZE(cBECharacterInterface, 0x10);

// The one character interface (On Tour: cBECharacterInterface::instance), created by cBECharacterInterface_getThis.

#endif /* BE_BEINTCHARACTER_H */
