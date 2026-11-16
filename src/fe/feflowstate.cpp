#include "common.h"

//100%
INCLUDE_ASM("fe/feflowstate", cFEFlowState_setNumStates);
#ifdef SKIP_ASM
extern "C" void func_001A06B0(void* self);
extern "C" void* func_00416210(void* dst, int c, int n);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_FEFlowState[];

/**
 * Sets the number of flow states and allocates a zeroed byte per state ("FEFlowState"), freeing the old
 * array.
 *
 * @param NumStates  number of states
 */
extern "C" void cFEFlowState_setNumStates(void* self, int NumStates)
{
    signed char n = NumStates;
    func_001A06B0(self);
    *(signed char*)self = n;
    void* states = operator_new_tag(n, str_FEFlowState, 0x100, 0);
    *(void**)((char*)self + 0x4) = states;
    func_00416210(states, 0, n);
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", func_001A06B0);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);

extern "C" void func_001A06B0(void* self)
{
    void* buffer = *(void**)((char*)self + 0x4);
    if (buffer != 0) {
        cMemMan_free(buffer);
        *(void**)((char*)self + 0x4) = 0;
    }
    *(char*)self = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", func_001A06F0);
#ifdef SKIP_ASM
extern "C" signed char func_001A06F0(void* self, int index)
{
    return *(signed char*)(*(int*)((char*)self + 0x4) + (signed char)index);
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", func_001A0708);
#ifdef SKIP_ASM
extern "C" void func_001A0708(void* self, int index, int value)
{
    *(char*)(*(int*)((char*)self + 0x4) + (signed char)index) = value;
}
#endif

