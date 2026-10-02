#include "common.h"

extern "C" void cBucketMan_add(void* mgr, void* node, void* param);
extern void* D_00491F00[16];
extern char D_004A5988;

struct cObjNode {
    char pad_0x00[0xC];
    void* field_0xC;
};

//99.94%
INCLUDE_ASM("object/objnode", cObjNode_cObjNode__FP8cObjNodePv);
#ifdef SKIP_ASM
cObjNode* cObjNode_cObjNode(cObjNode* self, void* param2)
{
    self->field_0xC = D_00491F00;
    cBucketMan_add(&D_004A5988, self, param2);
    return self;
}
#endif

INCLUDE_ASM("object/objnode", func_003546C8);

INCLUDE_ASM("object/objnode", func_00354720);

INCLUDE_ASM("object/objnode", cTypeObjNode_cTypeObjNode);

INCLUDE_ASM("object/objnode", func_00354808);

INCLUDE_ASM("object/objnode", func_00354850);

INCLUDE_ASM("object/objnode", cSortObjNode_cSortObjNode);

extern void* D_00491E00[];
extern "C" void* func_00354808(void*);

//99.5%
INCLUDE_ASM("object/objnode", func_00354920__FPv);
#ifdef SKIP_ASM
void* func_00354920(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_00491E00;
    return func_00354808(self);
}
#endif

//100%
INCLUDE_ASM("object/objnode", func_00354948);
#ifdef SKIP_ASM
class func_00354948_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_00354948(void* self, func_00354948_cObj* obj)
{
    obj->v01((char*)self + 0x10, 4);
}
#endif

INCLUDE_ASM("object/objnode", func_00354980);

//100%
INCLUDE_ASM("object/objnode", func_003549D0__FPv);
#ifdef SKIP_ASM
void* func_003549D0(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = t0;
    return self;
}
#endif

