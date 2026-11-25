#ifndef FE_FEFLOWSTATE_H
#define FE_FEFLOWSTATE_H

// cFEFlowState: a front-end flow state: a count and an array of values. Methods in fe/feflowstate.
//
// Size 0x8 (On Tour's; retail's methods reach mpValues at size - 4). Member names, order and offsets are EA's, from SSX
// On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"


struct cFEFlowState {
    char mNumStates;                // 0x000 [O]
    char unk_001[0x3];              // 0x001 padding
    char* mpValues;                 // 0x004 [O]

    // Methods bound to their retail symbols by asm labels.
    signed char getValue(int index) __asm__("cFEFlowState_getValue");
    void setValue(int index, int value) __asm__("cFEFlowState_setValue");
};

CHECK_OFFSET(cFEFlowState, mNumStates, 0x000);
CHECK_OFFSET(cFEFlowState, mpValues, 0x004);
CHECK_SIZE(cFEFlowState, 0x8);

#endif
