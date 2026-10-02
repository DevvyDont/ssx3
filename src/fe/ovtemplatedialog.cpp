#include "common.h"

INCLUDE_ASM("fe/ovtemplatedialog", cPDATemplate_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020A8B0);
#ifdef SKIP_ASM
extern "C" int func_0020A8B0(int a0, int a1)
{
    if ((unsigned int)(a0 - 6) < 6) {
        return 0x16;
    }
    if ((unsigned int)(a0 - 4) < 2 || a0 == 0 || a1 == 3) {
        return 0xb;
    }
    return 0xa;
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020A8F8);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020AB50);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CA10);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CBA0);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CBE8);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CC60);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CCF8);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020CFD0);

INCLUDE_ASM("fe/ovtemplatedialog", cOVStateManager_addPDATemplate);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020D190);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020D1D8);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D308);
#ifdef SKIP_ASM
extern "C" int func_0020D308(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020D318);

INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovtemplatedialog", func_0020D4F0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int idx);
extern char D_0046E818[];

extern "C" void func_0020D4F0(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_0046E818)) {
        if (*(int*)((char*)self + 0x9C) == 6) {
            cUIMenu_setSelectedByIndex(item, 0);
        } else {
            cUIMenu_setSelectedByIndex(item, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/ovtemplatedialog", func_0020D568);

INCLUDE_ASM("fe/ovtemplatedialog", cOVTemplate_Dialog_onWidgetEvent);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020DBD0);

INCLUDE_ASM("fe/ovtemplatedialog", func_0020DC68);

