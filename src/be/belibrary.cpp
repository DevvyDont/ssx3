#include "be/belibrary.h"

// Hand-written quadword block copy
extern "C" void* func_003E6574(void* dst, const void* src, uint size);
extern "C" int func_00151A88(tBEBoltCharacter* character, int id);

// Score table entries, 100 bytes each
struct tBEScoreType {
    char unk0[100];
};
extern tBEScoreType D_00535C18[];

// Score type per event (a) and mode (b): two (mode, type) pairs per event
struct tBEScoreTypeMap {
    int mode;
    int type;
};
extern const tBEScoreTypeMap D_0045A2F8[17][2];

INCLUDE_ASM("be/belibrary", getCharacterID__10cBELibraryi);
#ifdef SKIP_ASM
int cBELibrary::getCharacterID(int slot)
{
    return D_00535B20[D_005305B0[slot]].character;
}
#endif

INCLUDE_ASM("be/belibrary", getCheatCharacter__10cBELibraryi);
#ifdef SKIP_ASM
int cBELibrary::getCheatCharacter(int slot)
{
    return D_00535B20[D_005305B0[slot]].cheatCharacter;
}
#endif

INCLUDE_ASM("be/belibrary", getProfileIndex__10cBELibraryi);
#ifdef SKIP_ASM
int cBELibrary::getProfileIndex(int slot)
{
    tBERider* rider = &D_00535B20[D_005305B0[slot]];
    if (rider->profileSlot == -1)
        return 2;
    return rider->profile;
}
#endif

INCLUDE_ASM("be/belibrary", getRiderIndex__10cBELibraryi);
#ifdef SKIP_ASM
int cBELibrary::getRiderIndex(int profile)
{
    int i;
    for (i = 0; i < 6; i++) {
        if (D_00535B20[D_005305B0[i]].profile == profile)
            return i;
    }
    return -1;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014A180__10cBELibrary);
#ifdef SKIP_ASM
void cBELibrary::func_0014A180()
{
}
#endif

INCLUDE_ASM("be/belibrary", func_0014A188);

INCLUDE_ASM("be/belibrary", func_0014A5E0);

INCLUDE_ASM("be/belibrary", func_0014AB20__Fii);
#ifdef SKIP_ASM
tBEScoreType* func_0014AB20(int a, int b)
{
    int type = cBELibrary::getScoreType(a, b);
    if (type == 26)
        return 0;
    return &D_00535C18[type];
}
#endif

INCLUDE_ASM("be/belibrary", getScoreType__10cBELibraryii);
#ifdef SKIP_ASM
int cBELibrary::getScoreType(int a, int b)
{
    int i;
    if (a > 16)
        return 26;
    if (b >= 6 && b < 9)
        return b - 6;
    if (b >= 9 && b < 12)
        return b - 6;
    for (i = 0; i < 2; i++) {
        if (D_0045A2F8[a][i].mode == b)
            return D_0045A2F8[a][i].type;
    }
    return 26;
}
#endif

INCLUDE_ASM("be/belibrary", func_0014ABE0);

INCLUDE_ASM("be/belibrary", getCharacterID__16cBEBoltInterfacei);
#ifdef SKIP_ASM
int cBEBoltInterface::getCharacterID(int slot)
{
    return cBELibrary::getCharacterID(slot);
}
#endif

INCLUDE_ASM("be/belibrary", getProfileIndex__16cBEBoltInterfacei);
#ifdef SKIP_ASM
int cBEBoltInterface::getProfileIndex(int slot)
{
    return cBELibrary::getProfileIndex(slot);
}
#endif

INCLUDE_ASM("be/belibrary", getProfileAndCharacter__16cBEBoltInterfaceiPi);
#ifdef SKIP_ASM
int cBEBoltInterface::getProfileAndCharacter(int slot, int* character)
{
    *character = cBELibrary::getCharacterID(slot);
    return cBELibrary::getProfileIndex(slot);
}
#endif

INCLUDE_ASM("be/belibrary", setItems__16cBEBoltInterfaceiiPv);
#ifdef SKIP_ASM
void cBEBoltInterface::setItems(int profile, int character, void* items)
{
    func_003E6574(D_004A6CA8[profile][character].items, items, sizeof(D_004A6CA8[0][0].items));
}
#endif

INCLUDE_ASM("be/belibrary", func_0014ACF8__16cBEBoltInterfaceii);
#ifdef SKIP_ASM
int cBEBoltInterface::func_0014ACF8(int profile, int character)
{
    return func_00151A88(getCharacter(profile, character), character);
}
#endif

INCLUDE_ASM("be/belibrary", getCharacter__16cBEBoltInterfaceii);
#ifdef SKIP_ASM
tBEBoltCharacter* cBEBoltInterface::getCharacter(int profile, int character)
{
    return &D_004A6CA8[profile][character];
}
#endif

INCLUDE_ASM("be/belibrary", getCharacterForSlot__16cBEBoltInterfacei);
#ifdef SKIP_ASM
tBEBoltCharacter* cBEBoltInterface::getCharacterForSlot(int slot)
{
    int profile = getProfileIndex(slot);
    return getCharacter(profile, getCharacterID(slot));
}
#endif

INCLUDE_ASM("be/belibrary", getItem__16cBEBoltInterfaceiii);
#ifdef SKIP_ASM
tBEBoltOn* cBEBoltInterface::getItem(int profile, int character, int index)
{
    return &D_004A6CA8[profile][character].items[index];
}
#endif

INCLUDE_ASM("be/belibrary", getItemForSlot__16cBEBoltInterfaceii);
#ifdef SKIP_ASM
tBEBoltOn* cBEBoltInterface::getItemForSlot(int slot, int index)
{
    int character = 0;
    int profile = getProfileAndCharacter(slot, &character);
    return getItem(profile, character, index);
}
#endif

INCLUDE_ASM("be/belibrary", getItems__16cBEBoltInterfaceiiPi);
#ifdef SKIP_ASM
tBEBoltOn* cBEBoltInterface::getItems(int profile, int character, int* count)
{
    *count = D_004A6CA8[profile][character].count;
    return D_004A6CA8[profile][character].items;
}
#endif

INCLUDE_ASM("be/belibrary", getItemsForSlot__16cBEBoltInterfaceiPi);
#ifdef SKIP_ASM
tBEBoltOn* cBEBoltInterface::getItemsForSlot(int slot, int* count)
{
    int character;
    int profile = getProfileAndCharacter(slot, &character);
    return getItems(profile, character, count);
}
#endif

INCLUDE_ASM("be/belibrary", func_0014AEA8);

INCLUDE_ASM("be/belibrary", func_0014AF10);

INCLUDE_ASM("be/belibrary", func_0014AFB0);

INCLUDE_ASM("be/belibrary", func_0014B0F0);

INCLUDE_ASM("be/belibrary", func_0014B170);

INCLUDE_ASM("be/belibrary", func_0014B1B0);

INCLUDE_ASM("be/belibrary", func_0014B218);

INCLUDE_ASM("be/belibrary", func_0014B290);

INCLUDE_ASM("be/belibrary", func_0014B2F8);

INCLUDE_ASM("be/belibrary", func_0014B378);

INCLUDE_ASM("be/belibrary", func_0014B478);

INCLUDE_ASM("be/belibrary", func_0014B560);

INCLUDE_ASM("be/belibrary", func_0014B700);

INCLUDE_ASM("be/belibrary", func_0014B7A8);

INCLUDE_ASM("be/belibrary", func_0014B920);

INCLUDE_ASM("be/belibrary", func_0014B988);

INCLUDE_ASM("be/belibrary", func_0014BBA0);

INCLUDE_ASM("be/belibrary", func_0014BC18);

INCLUDE_ASM("be/belibrary", func_0014BD78);

INCLUDE_ASM("be/belibrary", func_0014BD98);

INCLUDE_ASM("be/belibrary", func_0014BDB8);

INCLUDE_ASM("be/belibrary", func_0014BDC8);

INCLUDE_ASM("be/belibrary", func_0014BE00);

INCLUDE_ASM("be/belibrary", func_0014BE38);

INCLUDE_ASM("be/belibrary", func_0014BE70);

INCLUDE_ASM("be/belibrary", func_0014C068);

INCLUDE_ASM("be/belibrary", func_0014C2B0);

INCLUDE_ASM("be/belibrary", func_0014C320);

INCLUDE_ASM("be/belibrary", func_0014C3C8);

INCLUDE_ASM("be/belibrary", func_0014C488);

INCLUDE_ASM("be/belibrary", func_0014C548);

INCLUDE_ASM("be/belibrary", func_0014C590);

INCLUDE_ASM("be/belibrary", func_0014C5D8);

INCLUDE_ASM("be/belibrary", func_0014C620);

INCLUDE_ASM("be/belibrary", func_0014C658);

INCLUDE_ASM("be/belibrary", func_0014C6A0);

INCLUDE_ASM("be/belibrary", func_0014C7A0);

INCLUDE_ASM("be/belibrary", func_0014D068);

INCLUDE_ASM("be/belibrary", func_0014D240);

INCLUDE_ASM("be/belibrary", func_0014D368);

INCLUDE_ASM("be/belibrary", func_0014D448);

INCLUDE_ASM("be/belibrary", func_0014D4C8);

INCLUDE_ASM("be/belibrary", func_0014D558);

INCLUDE_ASM("be/belibrary", func_0014D608);

INCLUDE_ASM("be/belibrary", func_0014D7E8);

INCLUDE_ASM("be/belibrary", func_0014D908);

INCLUDE_ASM("be/belibrary", func_0014D988);

INCLUDE_ASM("be/belibrary", func_0014D998);

INCLUDE_ASM("be/belibrary", func_0014D9D0);

INCLUDE_ASM("be/belibrary", func_0014DA08);

INCLUDE_ASM("be/belibrary", func_0014DA18);

INCLUDE_ASM("be/belibrary", func_0014DA28);

INCLUDE_ASM("be/belibrary", func_0014DAA0);

INCLUDE_ASM("be/belibrary", func_0014DAC0);

INCLUDE_ASM("be/belibrary", func_0014DB40);

INCLUDE_ASM("be/belibrary", func_0014DC00);

INCLUDE_ASM("be/belibrary", func_0014DC10);

INCLUDE_ASM("be/belibrary", func_0014DC40);

INCLUDE_ASM("be/belibrary", func_0014DC50);
