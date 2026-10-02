#include "common.h"

INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onCreateScreen);

INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onWidgetCreate);

INCLUDE_ASM("fe/festatelodge", func_001F3A38);

INCLUDE_ASM("fe/festatelodge", func_001F3CC8);

INCLUDE_ASM("fe/festatelodge", func_001F3CF0);

//100%
INCLUDE_ASM("fe/festatelodge", func_001F3FF8);
#ifdef SKIP_ASM
extern void* D_004739D8[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_001F3FF8(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_004739D8;
    *(int*)((char*)self + 0xC) = 0x28;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    return self;
}
#endif

INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onCreateScreen);

INCLUDE_ASM("fe/festatelodge", func_001F4108);

INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onWidgetCreate);

//100%
INCLUDE_ASM("fe/festatelodge", func_001F4380);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00194498(void* obj);
extern char D_0046EF70[];

extern "C" void func_001F4380(void* self)
{
    if (*(int*)((char*)self + 0x48) == 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046EF70));
        if (obj != 0) {
            func_00194498(obj);
        }
    }
}
#endif

INCLUDE_ASM("fe/festatelodge", func_001F43D8);

INCLUDE_ASM("fe/festatelodge", func_001F4400);

INCLUDE_ASM("fe/festatelodge", func_001F4728);

