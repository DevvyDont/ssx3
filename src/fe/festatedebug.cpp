#include "common.h"

INCLUDE_ASM("fe/festatedebug", cScreenKeyboard_initScreenKeyboard);

INCLUDE_ASM("fe/festatedebug", func_00180EF0);

//100%
INCLUDE_ASM("fe/festatedebug", func_00181090);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* owner);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern void* D_0046D000[];

extern "C" void* func_00181090(void* self, void* owner, int count)
{
    signed char n = count;
    int i = 0;
    unsigned char mask = 0xFF;
    func_0039E2A0(self, owner);
    *(void***)((char*)self + 0x8) = D_0046D000;
    *(int*)((char*)self + 0xC) = 9;
    *(int*)((char*)self + 0x54) = 1;
    *(signed char*)((char*)self + 0x44) = n;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    void* mgr = **(void***)((char*)self + 0x10);
    for (; i < n; i++) {
        mask &= ~func_001A1CD0(mgr, i);
    }
    *(unsigned char*)((char*)self + 0x15) = mask;
    return self;
}
#endif

