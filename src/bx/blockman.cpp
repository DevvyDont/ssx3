#include "common.h"

INCLUDE_ASM("bx/blockman", cBlockMan_init);


//100%
INCLUDE_ASM("bx/blockman", func_00316BE0);
#ifdef SKIP_ASM
extern "C" int func_00316BE0(void *arg0, int arg1) {
    int temp_5;
    int var_3;

    temp_5 = arg1 - (*(int *)((char*)(arg0) + (0)));
    var_3 = 0;
    if (temp_5 > 0) {
        var_3 = temp_5 < (*(int *)((char*)(arg0) + (4)));
    }
    return var_3;
}
#endif
INCLUDE_ASM("bx/blockman", func_00316C00);
INCLUDE_ASM("bx/blockman", func_00316C60);
//100%
INCLUDE_ASM("bx/blockman", func_00316CC8);
#ifdef SKIP_ASM
extern "C" void func_00316CC8(void) {

}
#endif
