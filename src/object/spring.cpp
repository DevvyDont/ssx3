#include "common.h"

INCLUDE_ASM("object/spring", cSpring_setupNodes);

INCLUDE_ASM("object/spring", func_00353FC0);

//100%
INCLUDE_ASM("object/spring", func_003545D8);
#ifdef SKIP_ASM
struct sSpringVEntry45D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_003545D8(void* self, void* stream)
{
    sSpringVEntry45D8* e = &(*(sSpringVEntry45D8**)stream)[1];
    e->fn((char*)stream + e->delta, self, 0x24);
    e = &(*(sSpringVEntry45D8**)stream)[1];
    e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x24), *(int*)self * 0x50);
}
#endif

INCLUDE_ASM("object/spring", func_00354648);

