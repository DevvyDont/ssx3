#include "common.h"

INCLUDE_ASM("seg/seg_2125C8", cAIAnimEventMap_getBlendInTime);

INCLUDE_ASM("seg/seg_2125C8", func_003115E0);

INCLUDE_ASM("seg/seg_2125C8", func_00311710);

INCLUDE_ASM("seg/seg_2125C8", func_00311860);

INCLUDE_ASM("seg/seg_2125C8", func_00311870);

//100%
INCLUDE_ASM("seg/seg_2125C8", func_003118A8);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int);
extern int D_004A3DFC;

extern "C" void func_003118A8(void) {
    operator_delete__FPi(D_004A3DFC);
    D_004A3DFC = 0;
}
#endif
