#include "common.h"

INCLUDE_ASM("fe/festateselectmp", cFEStateSelectMultiplayerMode_onCreateScreen);

INCLUDE_ASM("fe/festateselectmp", func_001A30F8);

INCLUDE_ASM("fe/festateselectmp", func_001A3160);

//100%
INCLUDE_ASM("fe/festateselectmp", func_001A3218);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_00469178[];

extern "C" void* func_001A3218(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_00469178;
    *(int*)((char*)self + 0xC) = 0x17;
    return self;
}
#endif

