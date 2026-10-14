#ifndef BEINTCHARACTER_H
#define BEINTCHARACTER_H

#include "common.h"

// One row of DATA/BE/CHARDB.DBL: 0x88 bytes per rider
struct cBECharacterData {
    char longName[0x20];
    char firstName[0x10];
    char nickname[0x10];
    int weight;             // Collision weight
    int goofy;              // Stance: nonzero for goofy
    int modelSize;          // Model scale in percent
    char bloodType[0x10];
    int female;
    int age;
    char height[0x10];
    char nationality[0x10];
    int unk84;
};

// Indexed by character id 0..9
extern cBECharacterData D_00530970[10];

class cBELibrary {
public:
    // Character id of the rider in the given setup slot
    static int getCharacterID(int slot);
};

// Character queries by setup slot
class cBECharacterInterface {
public:
    int getWeight(int slot);
    bool isGoofy(int slot);
    float getModelScale(int slot);
    void func_0014F100();
    char* getBloodType(int slot);
    bool isFemale(int slot);
    int getAge(int slot);
    char* getHeight(int slot);
    char* getNationality(int slot);
    int func_0014F210(int slot);
};

#endif
