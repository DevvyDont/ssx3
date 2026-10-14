#include "be/beintcharacter.h"

class cBE;
// Cheat character worn in the slot, 0 for none
extern "C" int func_0014A0B0(int slot);
extern "C" cBE* cBE_getBE();
extern "C" void* cBE_getInterface(cBE* be, int index);
extern "C" int cBENewPlayerInterface_isMissionMan(int slot);
// Zero in Conquer the Mountain
extern char D_00535C11[];

int cBECharacterInterface::getWeight(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].weight;
}

bool cBECharacterInterface::isGoofy(int slot)
{
    return D_00530970[cBELibrary::getCharacterID(slot)].goofy != 0;
}

// Model scale for the rider in a slot: a fixed scale per cheat character,
// otherwise the CHARDB model size in percent
float cBECharacterInterface::getModelScale(int slot)
{
    int id = cBELibrary::getCharacterID(slot);
    int cheat = func_0014A0B0(slot);

    cBE_getInterface(cBE_getBE(), 0);
    // In Conquer the Mountain the player keeps the base rider's scale
    if (cheat && (D_00535C11[0] || slot)) {
        switch (cheat) {
        case 10:
            return 0.98f;
        case 11:
        case 14:
            return 0.85f;
        case 29:
            return 2.0f;
        case 28:
            return 0.7f;
        case 26:
            return 1.5f;
        case 20:
            return 1.2f;
        case 22:
        case 23:
            return 1.3f;
        case 15:
        case 17:
            return 0.75f;
        case 13:
        case 18:
        case 19:
        case 21:
            return 1.0f;
        default:
            return 0.9f;
        }
    }
    if (cBENewPlayerInterface_isMissionMan(slot))
        return 0.8f;
    return D_00530970[id].modelSize * 0.01f;
}

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
