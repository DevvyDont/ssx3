#include "common.h"

INCLUDE_ASM("fe/festatetransition", cFEStateBackground_onCreateScreen);

INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onCreateScreen);

INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onScreenEvent);

INCLUDE_ASM("fe/festatetransition", func_001946A8);

//100%
INCLUDE_ASM("fe/festatetransition", func_00194738);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046BAB8[];

extern "C" void* func_00194738(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046BAB8;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

