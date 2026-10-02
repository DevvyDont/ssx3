#include "common.h"

INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onCreateScreen);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211220);
#ifdef SKIP_ASM
extern "C" void func_00211380(void* self);
extern "C" void func_00211270(void* self);

extern "C" void func_00211220(void* self)
{
    if (*(int*)((char*)self + 0x1C0) != 0) {
        *(int*)((char*)self + 0x22C) = 1;
    }
    if (*(int*)((char*)self + 0x214) == 3) {
        func_00211380(self);
    } else {
        func_00211270(self);
    }
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211270);

INCLUDE_ASM("fe/ovstateprofile", func_00211380);

INCLUDE_ASM("fe/ovstateprofile", func_00211850);

INCLUDE_ASM("fe/ovstateprofile", func_00211970);

INCLUDE_ASM("fe/ovstateprofile", func_00211A08);

INCLUDE_ASM("fe/ovstateprofile", func_00211AA8);

extern "C" void* func_001D58B8(void*);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_00211B10__FPv);
#ifdef SKIP_ASM
void func_00211B10(void* self)
{
    func_001D58B8(self);
    *(int*)((char*)self + 0x1a4) = 0;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00211B38);

INCLUDE_ASM("fe/ovstateprofile", func_00211BC8);

INCLUDE_ASM("fe/ovstateprofile", func_00212080);

INCLUDE_ASM("fe/ovstateprofile", func_00212138);

INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onCreateScreen);

INCLUDE_ASM("fe/ovstateprofile", func_00212208);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_002122E0__FPv);
#ifdef SKIP_ASM
int func_002122E0(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_002122E8);

INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_displayOn);

//100%
INCLUDE_ASM("fe/ovstateprofile", func_002124C8);
#ifdef SKIP_ASM
extern "C" void func_0039F718(void* self);

extern "C" void func_002124C8(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        func_0039F718(*(char**)((char*)self + 0x10) + 0x18);
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

INCLUDE_ASM("fe/ovstateprofile", func_00212508);

