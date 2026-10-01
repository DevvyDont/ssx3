#include "common.h"

INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onCreateScreen);

INCLUDE_ASM("fe/festatecredits", cFEStateCredits_onGainFocus);

INCLUDE_ASM("fe/festatecredits", func_00185F40);

INCLUDE_ASM("fe/festatecredits", func_001863B8);

INCLUDE_ASM("fe/festatecredits", func_001863E8);

//100%
INCLUDE_ASM("fe/festatecredits", func_00186478);
#ifdef SKIP_ASM
extern "C" int func_00186478(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/festatecredits", func_001864B0);

INCLUDE_ASM("fe/festatecredits", func_00186518);

INCLUDE_ASM("fe/festatecredits", func_001865A8);

INCLUDE_ASM("fe/festatecredits", func_00186610);

