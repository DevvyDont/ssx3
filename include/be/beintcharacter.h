#ifndef BE_BEINTCHARACTER_H
#define BE_BEINTCHARACTER_H

// cBECharacterInterface: the back end's character-bio interface - a rider's character name, short name, nickname,
// weight, stance (goofy), scale, blood type, sex, age, height, nationality and voice. The object holds no character
// data: like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore
// `this` and read the character table cBELibrary_characterDB (0x88-byte records; On Tour: cBELibrary::characterDB) through the
// rider's character ID. One instance,
// created on demand by cBECharacterInterface_getThis (`cMemMan_alloc(0x10, "BECharInt", 0, 0)`, bIsActive = 0,
// vtable cBECharacterInterface_vtbl) into cBECharacterInterface::instance (retail cBECharacterInterface_instance, D_004A1208); the
// rest of the game reaches it as cBE_getInterface(cBE_getBE(), 2). EA's source:
// d:/ssx/code/source/be/beintcharacter.cpp
// (preview assert string); in retail getThis and the name getters sit at the end of the be/beintnetwork unit, the rest
// in
// be/beintcharacter.
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's
// cBECharacterInterface has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBECharacterInterface_vtbl (0x45AD38, unnamed in symbol_addrs), slot 1 submit = cBECharacterInterface_submit (empty), slot 2 discard
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
#include "be/beinterface.h"

struct cBECharacterInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [U] cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis stores cBECharacterInterface_vtbl; no virtual call through it seen

    // Methods bound to their retail symbols by asm labels.
    bool getIsGoofyFoot(int riderIndex) __asm__("cBECharacterInterface_getIsGoofyFoot");
    float getScale(int riderIndex) __asm__("cBECharacterInterface_getScale");
    void* getBloodType(int riderIndex) __asm__("cBECharacterInterface_getBloodType");
    bool getIsFemale(int riderIndex) __asm__("cBECharacterInterface_getIsFemale");
    int getAge(int riderIndex) __asm__("cBECharacterInterface_getAge");
    void* getHeight(int riderIndex) __asm__("cBECharacterInterface_getHeight");
    void* getNationality(int riderIndex) __asm__("cBECharacterInterface_getNationality");
    int getVoiceID(int riderIndex) __asm__("cBECharacterInterface_getVoiceID");
    void* getName(int rider) __asm__("cBECharacterInterface_getName");
    void* getShortName(int rider) __asm__("cBECharacterInterface_getShortName");
    void* getShortNameByCharacter(int player, int index) __asm__("cBECharacterInterface_getShortNameByCharacter");
    void* getNickName(int rider) __asm__("cBECharacterInterface_getNickName");
    int getWeight(int riderIndex) __asm__("cBECharacterInterface_getWeight__FPvi");
    // submit stays a C function (cBECharacterInterface_submit): declared first, it is the vtable's key function, which
    // keeps the vtable out of the units defining the other overrides
    virtual void submit() __asm__("cBECharacterInterface_submit__FPv");
    virtual void discard() __asm__("cBECharacterInterface_discard__FPv");
};

extern cBECharacterInterface* cBECharacterInterface_instance;

CHECK_SIZE(cBECharacterInterface, 0x10);

// The one character interface (On Tour: cBECharacterInterface::instance), created by cBECharacterInterface_getThis.

#endif /* BE_BEINTCHARACTER_H */
