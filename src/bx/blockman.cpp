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

//100%
// Pops a free block from the first pool whose block size is at least size, or returns null.
struct sBlock_316C00 { sBlock_316C00* next; };
struct sPool_316C00 { int base; int span; unsigned int size; sBlock_316C00* free; };
struct sBlockMan_316C00 { int pad0; int pad4; sPool_316C00* pools; int count; };

extern "C" sBlock_316C00* func_00316C00(sBlockMan_316C00* self, unsigned int size)
{
    for (int i = 0; i < self->count; i++)
    {
        if (self->pools[i].size >= size && self->pools[i].free)
        {
            sBlock_316C00* b = self->pools[i].free;
            self->pools[i].free = b->next;
            return b;
        }
    }
    return 0;
}

//100%
// Pushes block b back on the free list of the pool whose range holds it.
extern "C" void func_00316C60(sBlockMan_316C00* self, sBlock_316C00* b)
{
    for (int i = 0; i < self->count; i++)
    {
        int off = (int)b - self->pools[i].base;
        if (off >= 0 && off < self->pools[i].span)
        {
            b->next = self->pools[i].free;
            self->pools[i].free = b;
            return;
        }
    }
}

extern "C" void func_00316CC8()
{
}
