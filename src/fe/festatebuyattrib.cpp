#include "common.h"

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);

INCLUDE_ASM("fe/festatebuyattrib", func_001F49D0);

INCLUDE_ASM("fe/festatebuyattrib", func_001F49F8);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A38);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A60);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A90);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4C30);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateLevels);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateCostPerLevel);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateExperienceDisplay);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateTotalCost);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateBank);

INCLUDE_ASM("fe/festatebuyattrib", func_001F5300);

INCLUDE_ASM("fe/festatebuyattrib", func_001F54B0);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onCreateScreen);

INCLUDE_ASM("fe/festatebuyattrib", func_001F55C0);

INCLUDE_ASM("fe/festatebuyattrib", func_001F55E8);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5618);
#ifdef SKIP_ASM
extern "C" int func_001F5618(void* self, int a1, unsigned int a2)
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

INCLUDE_ASM("fe/festatebuyattrib", func_001F5650);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onInputBegin);

INCLUDE_ASM("fe/festatebuyattrib", func_001F5A38);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupMenuFocus);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupHighlightsList);

INCLUDE_ASM("fe/festatebuyattrib", func_001F60E0);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6490);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6840);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupRidersBest);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6F30);

