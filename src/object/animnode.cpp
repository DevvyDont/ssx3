#include "common.h"

INCLUDE_ASM("object/animnode", cAnimNode_setAnimMeshCache);

//100%
INCLUDE_ASM("object/animnode", func_0034E600);
#ifdef SKIP_ASM
struct sVec4_E600 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_E600 {
    sVec4_E600 r[4];
    sMat_E600() {}
};

struct sVEntry_E600a {
    short delta;
    short index;
    sMat_E600* (*fn)(void*, int);
};

struct sVEntry_E600b {
    short delta;
    short index;
    void (*fn)(void*, int, sMat_E600*);
};

extern "C" void func_0034E600(void* self, int id, int idx, sVec4_E600* out)
{
    void* obj = (char*)self + 0x14;
    if (id == *(int*)((char*)self + 0x2C)) {
        sVEntry_E600a* vt = *(sVEntry_E600a**)((char*)self + 0x20);
        sVec4_E600 t = vt[29].fn((char*)obj + vt[29].delta, idx)->r[3];
        *out = t;
        return;
    }
    sMat_E600 buf[24];
    sVEntry_E600b* vt = *(sVEntry_E600b**)((char*)self + 0x20);
    vt[33].fn((char*)obj + vt[33].delta, id + 0x10, buf);
    sVec4_E600 t = buf[idx].r[3];
    *out = t;
}
#endif

INCLUDE_ASM("object/animnode", func_0034E698);

INCLUDE_ASM("object/animnode", func_0034E798);

//100%
INCLUDE_ASM("object/animnode", func_0034EBA0);
#ifdef SKIP_ASM
struct sAnimLinkList_EBA0;

struct sAnimLink_EBA0 {
    char pad_0x00[0x18];
    sAnimLinkList_EBA0* list; // 0x18
};

struct sAnimLinkList_EBA0 {
    char pad_0x00[0xC];
    sAnimLink_EBA0* head; // 0x0C
};

extern "C" void func_002D1AC8(sAnimLinkList_EBA0* list);

struct sAnimNode_EBA0 {
    char pad_0x00[0x14];
    sAnimLink_EBA0 link; // 0x14
};

// is this node's link at the head of its list, after func_002D1AC8?
extern "C" int func_0034EBA0(sAnimNode_EBA0* self)
{
    sAnimLink_EBA0* link = &self->link;
    func_002D1AC8(self->link.list);
    return link->list->head == link;
}
#endif

INCLUDE_ASM("object/animnode", func_0034EBE0);

INCLUDE_ASM("object/animnode", func_0034EC58);

INCLUDE_ASM("object/animnode", func_0034ED88);

//100%
INCLUDE_ASM("object/animnode", func_0034EE68);
#ifdef SKIP_ASM
extern void* D_0048F858[];
extern "C" void func_0034F048(void* self);
extern "C" void func_003553C0(void* self, int flags);
void operator_delete(int*);

extern "C" void func_0034EE68(void* self, int flags)
{
    *(void***)((char*)self + 0xC) = D_0048F858;
    func_0034F048(self);
    operator_delete(*(int**)((char*)self + 0x74));
    func_003553C0(self, flags);
}
#endif

