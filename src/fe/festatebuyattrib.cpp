#include "common.h"

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);

INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F49D0);
#ifdef SKIP_ASM
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);

extern "C" int func_001F49D0(void* self, int on)
{
    if (on != 0) {
        cFEStateBuyAttrib_updateExperienceDisplay(self);
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F49F8);

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A38);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F4A60);
#ifdef SKIP_ASM
extern "C" void func_001F5300(void* self);

extern "C" void func_001F4A60(void* self, void* sender, int msg)
{
    if (msg == 0x16) {
        if (*(int*)((char*)sender + 0x6C) != 0) {
            func_001F5300(self);
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55C0);
#ifdef SKIP_ASM
extern "C" void cFEStateCareerStats_setupHighlightsList(void* self);

extern "C" int func_001F55C0(void* self, int on)
{
    if (on != 0) {
        cFEStateCareerStats_setupHighlightsList(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55E8);
#ifdef SKIP_ASM
extern "C" void func_00186518(void* self);
extern "C" void cFEStateCareerStats_setupMenuFocus(void* self, int focus);

extern "C" void func_001F55E8(void* self)
{
    func_00186518(self);
    cFEStateCareerStats_setupMenuFocus(self, *(int*)((char*)self + 0x48));
}
#endif

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

