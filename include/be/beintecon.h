#ifndef BE_BEINTECON_H
#define BE_BEINTECON_H

// cBEEconInterface: the back end's economy interface - each profile's money (get/set/add/reduce, total earned), peak
// passes, buying and selling attribute levels and their costs, and the cash rewards for medals, collectibles and
// missions. The object holds no money: like every BE interface it is a cBEInterface (three word bools + vptr, 0x10
// bytes), and its methods ignore `this` and work on the profile library (cBELibrary_profileLib) and the reward tables.
// One instance, created on demand by cBEEconInterface_getThis (`cMemMan_alloc(0x10, "BEEconInt", 0, 0)`,
// bIsActive = 0, vtable cBEEconInterface_vtbl) into cBEEconInterface::instance (retail cBEEconInterface_instance, D_004A1214);
// the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 11). EA's source:
// d:/ssx/code/source/be/beintecon.cpp (preview assert string). The rest of the retail be/beintecon unit
// (cBECharProfileDB__cBECharProfileDB, cBECharProfileDB_init, func_001513B8..func_00152368) is cBECharProfileDB's.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEEconInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// cBEEconInterface_vtbl (0x45AC78, unnamed in symbol_addrs), slot 1 submit = cBEEconInterface_submit (empty), slot 2 discard =
// cBEEconInterface_discard (empty), slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (the inlined constructor: allocation, bIsActive, vptr);
//   [P] the July 2003 preview (getThis allocates 0x10; submit__16cBEEconInterface / discard__16cBEEconInterface are
//       empty inline methods);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"

struct cBEEconInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U]
                                     //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit,
                                     //       cBERaceState_onEnter/onExit set 0/1
    void** mVtbl;                    // 0x00C [R] getThis stores cBEEconInterface_vtbl; [U] cFEStateBuyAttrib_buyExperience calls
                                     //       slot 1 (submit) through it
};

extern cBEEconInterface* cBEEconInterface_instance;

CHECK_OFFSET(cBEEconInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEEconInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEEconInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEEconInterface, mVtbl, 0x0C);
CHECK_SIZE(cBEEconInterface, 0x10);

// The one economy interface (On Tour: cBEEconInterface::instance), created by cBEEconInterface_getThis.

#endif /* BE_BEINTECON_H */
