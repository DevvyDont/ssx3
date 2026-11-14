#include "common.h"

//100%
INCLUDE_ASM("seg/ctor_1218", func_00100218);
#ifdef SKIP_ASM
extern void func_00414780(void* p);
extern char D_0043CE00[];

/* Static constructor (last .ctors entry, D_0043CEE4): registers D_0043CE00 with func_00414780. The object was
   built as C at -O0 (frame pointer, unfilled delay slots), so its unit has its own flags. */
void func_00100218(void)
{
    func_00414780(D_0043CE00);
}
#endif
