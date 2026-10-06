#include "common.h"

INCLUDE_ASM("seg/seg_1DCD10", func_002DBD10);

INCLUDE_ASM("seg/seg_1DCD10", func_002DBEE0);

INCLUDE_ASM("seg/seg_1DCD10", func_002DBF80);

INCLUDE_ASM("seg/seg_1DCD10", func_002DBF98);

//100%
INCLUDE_ASM("seg/seg_1DCD10", func_002DC168);
#ifdef SKIP_ASM
extern "C" int func_002DC168(void *arg0, void *arg1) {
    int temp_3;
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (4)));
    temp_3 = (*(int *)((char*)(arg1) + (4)));
    if (temp_4 >= temp_3) {
        return (temp_3 >= temp_4) ? 0 : -1;
    }
    return 1;
}
#endif

INCLUDE_ASM("seg/seg_1DCD10", func_002DC190);

INCLUDE_ASM("seg/seg_1DCD10", func_002DC7B0);
