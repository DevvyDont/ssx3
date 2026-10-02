#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cInstanceNode_getBoundBoxInfo(void* self, void* out);
extern const char D_0048E890[];

struct cFloatingNode {
    char pad_0x00[0x78];
    void* field_0x78;
};

//100%
INCLUDE_ASM("object/floatingnode", cFloatingNode_initInfo__FP13cFloatingNode);
#ifdef SKIP_ASM
// PORT: the unit declares cInstanceNode_getBoundBoxInfo with 2 args; its body uses 3 ($6 is written).
void cInstanceNode_getBoundBoxInfo_3(void* self, void* box, void* out) __asm__("cInstanceNode_getBoundBoxInfo");

void cFloatingNode_initInfo(cFloatingNode* self)
{
    void* mem = cMemMan_alloc(0x30, D_0048E890, 0x20000000, 0);
    self->field_0x78 = mem;
    cInstanceNode_getBoundBoxInfo_3(self, mem, (char*)mem + 0x20);
}
#endif

//100%
INCLUDE_ASM("object/floatingnode", func_0034EF08);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0034EF08(cFloatingNode* self)
{
    if (self->field_0x78 != 0) {
        operator_delete((int*)self->field_0x78);
        self->field_0x78 = 0;
    }
}
#endif

INCLUDE_ASM("object/floatingnode", func_0034EF40);

INCLUDE_ASM("object/floatingnode", func_0034F048);

INCLUDE_ASM("object/floatingnode", func_0034F120);

INCLUDE_ASM("object/floatingnode", func_0034F600);

INCLUDE_ASM("object/floatingnode", func_0034F790);

INCLUDE_ASM("object/floatingnode", func_0034F930);

//100%
INCLUDE_ASM("object/floatingnode", func_0034FA88);
#ifdef SKIP_ASM
struct sFloatingVEntryFA88 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00356B30(void* self, void* stream);

extern "C" void func_0034FA88(cFloatingNode* self, void* stream)
{
    int has;
    func_00356B30(self, stream);
    has = self->field_0x78 != 0;
    sFloatingVEntryFA88* e = &(*(sFloatingVEntryFA88**)stream)[1];
    e->fn((char*)stream + e->delta, &has, 4);
    e = &(*(sFloatingVEntryFA88**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x2C, 0x48);
}
#endif

INCLUDE_ASM("object/floatingnode", func_0034FB00);

