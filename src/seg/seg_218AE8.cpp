#include "common.h"

INCLUDE_ASM("seg/seg_218AE8", func_00317AE8);

INCLUDE_ASM("seg/seg_218AE8", func_00317B50);

INCLUDE_ASM("seg/seg_218AE8", func_00317C48);

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317D10);
#ifdef SKIP_ASM
extern int D_004A3E80;

extern "C" void func_00317D10(int arg0) {
    D_004A3E80 = arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_218AE8", func_00317D18);
#ifdef SKIP_ASM
extern int D_004A3E80;

extern "C" int func_00317D18(void) {
    return D_004A3E80;
}
#endif
