#ifndef BELIBRARY_H
#define BELIBRARY_H

#include "common.h"

// A rider in the race setup: 0x1C bytes
struct tBERider {
    int unk0;
    int unk4;
    int unk8;
    int profileSlot;        // -1 when the rider has no saved profile
    uint profile : 1;       // Memory card profile: player 1 or 2
    char character;         // Character id 0..9
    char cheatCharacter;    // Cheat character worn, 0 for none
    char unk13[0x1C - 0x13];
};

// Setup slot -> index into D_00535B20
extern int D_005305B0[6];
extern tBERider D_00535B20[];

class cBELibrary {
public:
    // Character id of the rider in the given setup slot
    static int getCharacterID(int slot);
    // Cheat character worn by the rider in the slot, 0 for none
    static int getCheatCharacter(int slot);
    // Profile of the rider in the slot: 0 or 1, 2 for none
    static int getProfileIndex(int slot);
    // First slot whose rider has the given profile, -1 if none
    static int getRiderIndex(int profile);
    static void func_0014A180();
    static int getScoreType(int a, int b);
};

// A bolt-on (gear item) owned by a character
struct tBEBoltOn {
    ushort id;
    ushort flags;
};

// Bolt-ons of one character in one profile: 0xF88 bytes
struct tBEBoltCharacter {
    char unk0[0x28C];
    int count;
    tBEBoltOn items[525];
    char unkAC4[0xF88 - 0xAC4];
};

// Per profile (player 1, player 2, none), per character
extern tBEBoltCharacter D_004A6CA8[3][10];

// The bolt-on store. The singleton is allocated as "BEBoltInt"
class cBEBoltInterface {
public:
    static int getCharacterID(int slot);
    static int getProfileIndex(int slot);
    int getProfileAndCharacter(int slot, int* character);
    void setItems(int profile, int character, void* items);
    int func_0014ACF8(int profile, int character);
    tBEBoltCharacter* getCharacter(int profile, int character);
    tBEBoltCharacter* getCharacterForSlot(int slot);
    tBEBoltOn* getItem(int profile, int character, int index);
    tBEBoltOn* getItemForSlot(int slot, int index);
    tBEBoltOn* getItems(int profile, int character, int* count);
    tBEBoltOn* getItemsForSlot(int slot, int* count);
};

#endif
