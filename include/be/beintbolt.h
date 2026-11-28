#ifndef BE_BEINTBOLT_H
#define BE_BEINTBOLT_H

// cBEBoltInterface: the back end's bolt-on (equipment item) interface - BE interface 9 (kInt_Bolt): a character's
// owned / equipped bolt flags in its career record (cBELibrary_profileLib[profile][character],
// cBECharProfileDB::mBolts), buying bolts, the bolts' model / icon file names, and access to the bolt database
// (cBEBoltDB, be/bedbbolt.h). The object holds no bolt data: like every BE interface it is a cBEInterface (three word
// bools + vptr, 0x10 bytes), and its methods ignore `this` and work on cBELibrary_profileLib and the bolt database
// (cBELibrary_boltDB). One instance, created on demand by cBEBoltInterface_getThis
// (`cMemMan_alloc(0x10, "BEBoltInt", 0, 0)`, bIsActive = 0, vtable cBEInterface_vtbl) into cBEBoltInterface::instance
// (retail cBEBoltInterface_instance, D_004A11C8); cBELibrary's constructor creates it with the other interfaces and
// its destructor deletes it; the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 9).
// EA's source: d:/ssx/code/source/be/beintbolt.cpp (July 2003 preview assert path; the header name is ours, after
// be/beintnewrace.h etc.).
//
// Size 0x10: [R] the allocation in cBEBoltInterface_getThis; the July 2003 preview's getThis (0x148220) allocates 0x10
// "BEBoltInt" too, clears +0x8 and stores a vtable at +0xC.
// Base cBEInterface (be/beinterface.h), written out inline. cBEBoltInterface overrides no virtual: retail's getThis
// stores cBEInterface's own vtable cBEInterface_vtbl (0x45AE88: slots 1-4 cBEInterface_submit / _discard /
// _writeToReplayFrame / _readFromReplayFrame, all empty), the same one cBERewardInterface_getThis stores.
// [O] SSX On Tour has no bolt interface (no class of that name in its DWARF); its cBEInterface gives the base members.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's getThis (allocation, bIsActive, vptr);
//   [P] the preview's methods (0x1482F0 ...) start with `lw v1, 8(a0)` and assert
//       `bIsActive == true && "The bolt interface is not active!"` (beintbolt.cpp); retail compiled the asserts out,
//       so no retail method reads `this`;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's cBEInterface (bCanSubmit, bDataChanged, bIsActive, then the vptr; bools are 1 byte there, words
//       in retail).

#include "layout.h"

struct tBEBoltDef;

struct cBEBoltInterface {
    // --- cBEInterface (0x00-0x0F, be/beinterface.h)
    int bCanSubmit;                  // 0x00 [O] cBEInterface::bCanSubmit (bool, a word); no retail access seen
    int bDataChanged;                // 0x04 [O] cBEInterface::bDataChanged (bool, a word); no retail access seen
    int bIsActive;                   // 0x08 [O] cBEInterface::bIsActive (bool, a word); [P] the preview's method
                                     //      asserts read it at +0x8; [R] getThis sets 0; [U] through
                                     //      cBEBoltInterface_instance: cBEPreRaceState_onExit / cBERaceState_onExit
                                     //      clear it, cBERaceState_onEnter / cBEPostRaceState_onEnter set it
    void** mVtbl;                    // 0x0C [R] getThis stores cBEInterface_vtbl; [U] cBE_writeToReplayFrame /
                                     //      cBE_readFromReplayFrame call slots 3 / 4 through it (interface 9)

    // Methods bound to their retail symbols by asm labels.
    int getProfileAndCharacter(int rider, int* outChar) __asm__("cBEBoltInterface_getProfileAndCharacter");
    void restoreBolts(int profileIndex, int characterId, void* src) __asm__("cBEBoltInterface_restoreBolts");
    void buildBoltList(int profileIndex, int characterId) __asm__("cBEBoltInterface_buildBoltList");
    void* getCharProfile(int profileIndex, int characterId) __asm__("cBEBoltInterface_getCharProfile");
    void* getRiderCharProfile(int rider) __asm__("cBEBoltInterface_getRiderCharProfile");
    void* getBoltEntry(int profileIndex, int characterId, int slot) __asm__("cBEBoltInterface_getBoltEntry");
    void* getRiderBoltEntry(int rider, int slot) __asm__("cBEBoltInterface_getRiderBoltEntry");
    void* getBolts(int profileIndex, int characterId, int* out) __asm__("cBEBoltInterface_getBolts");
    void* getRiderBolts(int rider, int* out) __asm__("cBEBoltInterface_getRiderBolts");
    void storeVisibleFlags(int profileIndex, int characterId) __asm__("cBEBoltInterface_storeVisibleFlags");
    bool trySetBoltVisible(int profileIndex, int characterId, int idx, int on)
        __asm__("cBEBoltInterface_trySetBoltVisible");
    void toggleBoltVisible(int profileIndex, int characterId, int idx) __asm__("cBEBoltInterface_toggleBoltVisible");
    void hasBoltFlags(int profileIndex, int characterId, int boltId, int flagMask)
        __asm__("cBEBoltInterface_hasBoltFlags");
    void riderHasBoltFlags(int rider, int boltId, int flagMask) __asm__("cBEBoltInterface_riderHasBoltFlags");
    int countBoltsWithFlags(int profileIndex, int characterId, unsigned int mask)
        __asm__("cBEBoltInterface_countBoltsWithFlags");
    int getUsedEquipPoints(int profileIndex, int characterId) __asm__("cBEBoltInterface_getUsedEquipPoints");
    void buyBolt(int profileIndex, int characterId, int key, int flag) __asm__("cBEBoltInterface_buyBolt");
    char* getBigIconFileName(unsigned int x) __asm__("cBEBoltInterface_getBigIconFileName");
    int getVisibleModelFiles(int profile, int ch, char* out) __asm__("cBEBoltInterface_getVisibleModelFiles");
    int getRiderVisibleModelFiles(int rider, char* out) __asm__("cBEBoltInterface_getRiderVisibleModelFiles");
    void* loadBoltDB() __asm__("cBEBoltInterface_loadBoltDB__FPv");
    void* refreshVisibleBolts() __asm__("cBEBoltInterface_refreshVisibleBolts__FPv");
    bool buildBoltModelFileName(int profileIndex, int characterId, tBEBoltDef* boltDef, char* out)
        __asm__("cBEBoltInterface_buildBoltModelFileName");
    int buildRiderBoltModelFileName(int rider, tBEBoltDef* def, char* out)
        __asm__("cBEBoltInterface_buildRiderBoltModelFileName");
};

CHECK_OFFSET(cBEBoltInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEBoltInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEBoltInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEBoltInterface, mVtbl, 0x0C);
CHECK_SIZE(cBEBoltInterface, 0x10);

// The interface singleton (On Tour pattern: `static cBEBoltInterface* instance`), created by cBEBoltInterface_getThis.
extern cBEBoltInterface* cBEBoltInterface_instance;

#endif /* BE_BEINTBOLT_H */
