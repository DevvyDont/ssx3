#include "be/beintcharacter.h"

int cBECharacterInterface::getWeight(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].weight;
}

bool cBECharacterInterface::isGoofy(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].goofy != 0;
}

// Not yet decompiled: its switch over cheat characters needs a jump table in
// .rodata, which is still one unsplit block
INCLUDE_ASM("be/beintcharacter", func_0014EFA8);

void cBECharacterInterface::func_0014F100()
{
}

char* cBECharacterInterface::getBloodType(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].bloodType;
}

bool cBECharacterInterface::isFemale(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].female != 0;
}

int cBECharacterInterface::getAge(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].age;
}

char* cBECharacterInterface::getHeight(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].height;
}

char* cBECharacterInterface::getNationality(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].nationality;
}

int cBECharacterInterface::func_0014F210(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].unk84;
}
