#include "common.h"

signed char cBELibrary_getCharacterID(int index);

struct sCharWeightEntry {
    char pad_0x00[0x40];
    int mWeight;
    char pad_0x44[0x88 - 0x40 - 4];
};
extern sCharWeightEntry D_00530970[];

//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getWeight__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the weight of a rider's character.
 *
 * Looks up the rider's character id with cBELibrary_getCharacterID() and reads that character's mWeight from the
 * character table D_00530970.
 *
 * @param riderIndex  rider whose character is looked up
 * @return the character's weight
 */
int cBECharacterInterface_getWeight(void* self, int riderIndex)
{
    return D_00530970[cBELibrary_getCharacterID(riderIndex)].mWeight;
}
#endif

//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getIsGoofyFoot);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// sCharWeightEntry (declared above in the unit) has no field at 0x44 yet;
// this view of the same table binds to D_00530970.
struct sCharWeightEntryView {
    char pad_0x00[0x40];
    int mWeight;        // 0x40
    int field_0x44;     // 0x44
    char pad_0x48[0x88 - 0x48];
};
extern sCharWeightEntryView D_00530970_view[] __asm__("D_00530970");

/**
 * Tells whether a rider's character rides goofy (right foot forward).
 *
 * Looks up the character of @p riderIndex (cBELibrary_getCharacterID) in the 0x88-byte entries of D_00530970 and tests
 * the word at +0x44 (field_0x44 in the C view).
 *
 * @param riderIndex  the rider (On Tour iRiderID)
 * @return true for a goofy-footed character
 */
extern "C" bool cBECharacterInterface_getIsGoofyFoot(void* self, int riderIndex)
{
    int charID = cBELibrary_getCharacterID(riderIndex);
    return D_00530970_view[charID].field_0x44 != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getScale);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
signed char cBELibrary_getCharacterID(int index);
extern "C" int cBELibrary_getCheatCharacterID(int riderIndex);
extern "C" void* cBE_getBE();
void* cBE_getInterface_EFA8(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_isMissionMan(int riderIndex);
extern signed char D_00535C11[];

struct sCharScaleEntry_EFA8 {
    char pad_0x00[0x48];
    int mScale;         // 0x48, percent
    char pad_0x4C[0x88 - 0x4C];
};
extern sCharScaleEntry_EFA8 D_00530970_EFA8[] __asm__("D_00530970");

/**
 * Returns the size a rider's model is drawn at.
 *
 * A cheat character (cBELibrary_getCheatCharacterID, used for rider 0 only when D_00535C11 is set) gets a fixed scale
 * from 0.7 to 2.0 by id (default 0.9); a mission man (cBENewPlayerInterface_isMissionMan) 0.8; otherwise the character
 * entry's +0x48 percentage / 100 from D_00530970.
 *
 * @param riderIndex  the rider (On Tour iRiderID)
 * @return the scale factor, 1.0 = normal size
 */
// Model scale for a rider: fixed values for the cheat characters,
// 0.8 for mission men, otherwise the character table's percentage.
extern "C" float cBECharacterInterface_getScale(void* self, int riderIndex)
{
    int charID = cBELibrary_getCharacterID(riderIndex);
    int cheat = cBELibrary_getCheatCharacterID(riderIndex);
    cBE_getInterface_EFA8(cBE_getBE(), 0);
    if (cheat != 0 && (D_00535C11[0] != 0 || riderIndex != 0)) {
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
    if (cBENewPlayerInterface_isMissionMan(riderIndex)) {
        return 0.8f;
    }
    return D_00530970_EFA8[charID].mScale * 0.01f;
}
#endif


//100%
INCLUDE_ASM("be/beintcharacter", func_0014F100);
#ifdef SKIP_ASM
/**
 * Does nothing.
 */
extern "C" void func_0014F100(void) {

}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getBloodType);
#ifdef SKIP_ASM
extern char D_005309BC[];

/**
 * Returns the 16-byte text at +0x4C of @p riderIndex's character record (D_00530970, 0x88-byte entries; D_005309BC =
 * +0x4C).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return the text
 */
extern "C" void* cBECharacterInterface_getBloodType(void* self, int riderIndex)
{
    return D_005309BC + cBELibrary_getCharacterID(riderIndex) * 136;
}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getIsFemale);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// View of the character table D_00530970 (0x88-byte entries) for fields at 0x5C/0x60/0x84.
struct sCharEntryView {
    char pad_0x00[0x5C];
    int field_0x5C;
    int field_0x60;
    char pad_0x64[0x84 - 0x64];
    int field_0x84;
};
extern sCharEntryView D_00530970_flags[] __asm__("D_00530970");

/**
 * Tests the word at +0x5C of @p riderIndex's character record (D_00530970, 0x88-byte entries).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return true when the word is nonzero
 */
extern "C" bool cBECharacterInterface_getIsFemale(void* self, int riderIndex)
{
    return D_00530970_flags[cBELibrary_getCharacterID(riderIndex)].field_0x5C != 0;
}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getAge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sCharEntryView D_00530970_flags[] __asm__("D_00530970");

/**
 * Returns the word at +0x60 of @p riderIndex's character record (D_00530970, 0x88-byte entries).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return the value
 */
extern "C" int cBECharacterInterface_getAge(void* self, int riderIndex)
{
    return D_00530970_flags[cBELibrary_getCharacterID(riderIndex)].field_0x60;
}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getHeight);
#ifdef SKIP_ASM
extern char D_005309D4[];

/**
 * Returns the 16-byte text at +0x64 of @p riderIndex's character record (D_00530970, 0x88-byte entries; D_005309D4 =
 * +0x64).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return the text
 */
extern "C" void* cBECharacterInterface_getHeight(void* self, int riderIndex)
{
    return D_005309D4 + cBELibrary_getCharacterID(riderIndex) * 136;
}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getNationality);
#ifdef SKIP_ASM
extern char D_005309E4[];

/**
 * Returns the 16-byte text at +0x74 of @p riderIndex's character record (D_00530970, 0x88-byte entries; D_005309E4 =
 * +0x74).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return the text
 */
extern "C" void* cBECharacterInterface_getNationality(void* self, int riderIndex)
{
    return D_005309E4 + cBELibrary_getCharacterID(riderIndex) * 136;
}
#endif
//100%
INCLUDE_ASM("be/beintcharacter", cBECharacterInterface_getVoiceID);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sCharEntryView D_00530970_flags[] __asm__("D_00530970");

/**
 * Returns the word at +0x84 (the last) of @p riderIndex's character record (D_00530970, 0x88-byte entries).
 *
 * @param riderIndex  the rider (cBELibrary_getCharacterID selects the record)
 * @return the value
 */
extern "C" int cBECharacterInterface_getVoiceID(void* self, int riderIndex)
{
    return D_00530970_flags[cBELibrary_getCharacterID(riderIndex)].field_0x84;
}
#endif
