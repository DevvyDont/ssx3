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

//100%
INCLUDE_ASM("object/spring", func_00354648);
#ifdef SKIP_ASM
extern "C" void cBucketMan_add(void* mgr, void* node, void* param);
extern void* D_00491F00[16];
extern char D_004A5988;

struct cObjNode {
    char pad_0x00[0xC];
    void* field_0xC;
};

extern "C" cObjNode* func_00354648(cObjNode* self, void* param2)
{
    self->field_0xC = D_00491F00;
    cBucketMan_add(&D_004A5988, self, param2);
    return self;
}
#endif

