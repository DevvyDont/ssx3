#include "common.h"

INCLUDE_ASM("bx/blockman", cBlockMan_init);


//100%
// Returns whether addr falls inside the block (base at 0x0, size at 0x4), excluding the base itself.
extern "C" int func_00316BE0(int* block, int addr)
{
    int r = 0;
    addr -= block[0];
    if (addr > 0)
        r = addr < block[1];
    return r;
}

INCLUDE_ASM("bx/blockman", func_00316C00);

INCLUDE_ASM("bx/blockman", func_00316C60);

extern "C" void func_00316CC8()
{
}
