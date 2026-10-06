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
int cBECharacterInterface_getWeight(void* self, int riderIndex)
{
    return D_00530970[cBELibrary_getCharacterID(riderIndex)].mWeight;
}
#endif

//100%
INCLUDE_ASM("be/beintcharacter", func_0014EF70);
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

extern "C" bool func_0014EF70(void* self, int riderIndex)
{
    int charID = cBELibrary_getCharacterID(riderIndex);
    return D_00530970_view[charID].field_0x44 != 0;
}
#endif

INCLUDE_ASM("be/beintcharacter", func_0014EFA8);

