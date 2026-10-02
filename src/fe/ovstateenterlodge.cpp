#include "common.h"

INCLUDE_ASM("fe/ovstateenterlodge", cOVState_ENTERLODGE_onCreateScreen);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7210);

extern "C" void* func_0020A430(void*);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7298__FPv);
#ifdef SKIP_ASM
void* func_001F7298(void* self)
{
    *(int*)((char*)self + 0x50) = 0;
    return func_0020A430(self);
}
#endif

INCLUDE_ASM("fe/ovstateenterlodge", func_001F72B8);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F72E0);

INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onCreateScreen);

INCLUDE_ASM("fe/ovstateenterlodge", cOVState_BIGCHALLENGE_START_onGainTransition);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F74E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A23E0[];
extern char D_004A23E8[];

extern "C" void func_001F74E0(void* self, void* item)
{
    int id = *(int*)((char*)item + 0x38);
    if (id == GetHashValue32(D_004A23E0)) {
        *(int*)((char*)item + 0x18) = 0;
    } else {
        int id2 = *(int*)((char*)item + 0x38);
        if (id2 == GetHashValue32(D_004A23E8)) {
            *(int*)((char*)item + 0x18) = 1;
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7548);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F7590);
#ifdef SKIP_ASM
extern "C" int func_001F7590(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/ovstateenterlodge", func_001F75A0);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F76B8);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7738);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7770);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F77F0);
#ifdef SKIP_ASM
extern "C" int func_001F77F0(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7800);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7958);

INCLUDE_ASM("fe/ovstateenterlodge", cOVState_TOPTIMES_onWidgetCreate);

INCLUDE_ASM("fe/ovstateenterlodge", func_001F7F08);

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F80C8);
#ifdef SKIP_ASM
extern "C" int func_001F80C8(void* self, int a1, int a2)
{
    return a2 == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateenterlodge", func_001F80D8);
#ifdef SKIP_ASM
struct sVEntry_001F80D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001F80D8(void* self, void* a1, int a2)
{
    if (a1 != 0) {
        if (a2 == 5) {
            sVEntry_001F80D8* vt = *(sVEntry_001F80D8**)((char*)self + 0x8);
            vt[26].fn((char*)self + vt[26].delta);
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstateenterlodge", func_001F8118);

