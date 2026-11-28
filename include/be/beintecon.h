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
// (cBECharProfileDB::~cBECharProfileDB, cBECharProfileDB_init, cBECharProfileDB_applyStartingBolts..cBECharProfileDB_hasBoltFlags) is cBECharProfileDB's.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEEconInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
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
#include "be/beinterface.h"

struct cBEEconInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [U] cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit,
    //       cBERaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis stores cBEEconInterface_vtbl; [U] cFEStateBuyAttrib_buyExperience calls slot 1
    //       (submit) through it

    // Methods bound to their retail symbols by asm labels.
    int getMoney(int iHuman, int charType) __asm__("cBEEconInterface_getMoney");
    int getMoney1(int rider) __asm__("cBEEconInterface_getMoney1");
    int getTotalMoneyEarned1(int rider) __asm__("cBEEconInterface_getTotalMoneyEarned1");
    void setMoney(int profileIndex, int characterId, int value) __asm__("cBEEconInterface_setMoney");
    int addMoney(int humanIndex, int characterId, int amount) __asm__("cBEEconInterface_addMoney");
    int addMoney1(int rider, int amount) __asm__("cBEEconInterface_addMoney1");
    int reduceMoney(int profileIndex, int characterId, int amount) __asm__("cBEEconInterface_reduceMoney");
    bool getPeakPass(int profileIndex, int characterId, int peak, int unlocked) __asm__("cBEEconInterface_getPeakPass");
    bool buyAttribute(int humanIndex, int characterId, unsigned int attribute) __asm__("cBEEconInterface_buyAttribute");
    bool sellAttribute(int profile, int characterId, unsigned int kind) __asm__("cBEEconInterface_sellAttribute");
    int getCashReward(int medal, int key, int place) __asm__("cBEEconInterface_getCashReward");
    int getCollectableCash(int level) __asm__("cBEEconInterface_getCollectableCash");
    int getMissionCash(int level) __asm__("cBEEconInterface_getMissionCash");
    int getCostForNextIncrement(int humanIndex, int characterId, unsigned int attribute)
        __asm__("cBEEconInterface_getCostForNextIncrement");
    int getTotalMoneyEarned(int iHuman, int charType) __asm__("cBEEconInterface_getTotalMoneyEarned__FPvii");
    int getAttribCost(int levelIndex) __asm__("cBEEconInterface_getAttribCost__FPvi");
    // submit stays a C function (cBEEconInterface_submit): declared first, it is the vtable's key function, which keeps
    // the vtable out of the units defining the other overrides
    virtual void submit() __asm__("cBEEconInterface_submit__FPv");
    virtual void discard() __asm__("cBEEconInterface_discard__FPv");
};

extern cBEEconInterface* cBEEconInterface_instance;

CHECK_SIZE(cBEEconInterface, 0x10);

// The one economy interface (On Tour: cBEEconInterface::instance), created by cBEEconInterface_getThis.

#endif /* BE_BEINTECON_H */
