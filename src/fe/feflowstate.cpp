#include "common.h"
#include "fe/feflowstate.h"

//100%
INCLUDE_ASM("fe/feflowstate", cFEFlowState_setNumStates);
#ifdef SKIP_ASM
extern "C" void cFEFlowState_purge(cFEFlowState* self);
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
extern "C" void cFEFlowState_setNumStates(cFEFlowState* self, int NumStates)
{
    signed char n = NumStates;
    cFEFlowState_purge(self);
    *(signed char*)self = n;
    void* states = operator_new_tag(n, str_FEFlowState, 0x100, 0);
    self->mpValues = (char*)states;
    func_00416210(states, 0, n);
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", cFEFlowState_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);

/**
 * Frees the flow state's value array (+0x4, cMemMan_free) when there is one and sets the state count (+0x0) to 0.
 *
 * cFEFlowState_setNumStates calls it before allocating a new array; cRealFE_loseFocus calls it on the front-end flow
 * state gFlowState.
 */
extern "C" void cFEFlowState_purge(cFEFlowState* self)
{
    void* buffer = self->mpValues;
    if (buffer != 0) {
        cMemMan_free(buffer);
        self->mpValues = 0;
    }
    *(char*)self = 0;
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", cFEFlowState_getValue);
#ifdef SKIP_ASM
/**
 * Returns the flow state's saved value for one state: the signed byte at index @p index of the array at +0x4.
 *
 * There is no range check.
 *
 * @param index  state index (used as a signed char)
 * @return the stored value
 */
extern "C" signed char cFEFlowState_getValue(cFEFlowState* self, int index)
{
    return *(signed char*)((int)self->mpValues + (signed char)index);
}
#endif

//100%
INCLUDE_ASM("fe/feflowstate", cFEFlowState_setValue);
#ifdef SKIP_ASM
/**
 * Stores the flow state's value for one state: writes @p value as a byte at index @p index of the array at +0x4.
 *
 * There is no range check.
 *
 * @param index  state index (used as a signed char)
 * @param value  value to store (its low byte)
 */
extern "C" void cFEFlowState_setValue(cFEFlowState* self, int index, int value)
{
    *(char*)((int)self->mpValues + (signed char)index) = value;
}
#endif

