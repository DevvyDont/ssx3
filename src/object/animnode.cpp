#include "common.h"

//100%
INCLUDE_ASM("object/animnode", cAnimNode_setAnimMeshCache);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_003513D0(void* self);
extern "C" void func_003612C0(void* self);
extern "C" void func_00351398(void* self, void* src);
extern char D_0048E860[];
extern char D_0048E870[];
extern void* D_00490AD0[];
extern void* D_00490AF0[];

struct sMeshInst_E448 {
    char pad_0x0[0x84];
    void** vtable;          // 0x84
    char pad_0x88[0x48];
    sMeshInst_E448()
    {
        vtable = D_00490AF0;
        func_003513D0(this);
        vtable = D_00490AD0;
        func_003612C0(this);
    }
    void operator delete[](void* p, unsigned int size);
};

struct sMeshRef_E448 {
    char data[0x20];
    sMeshRef_E448() {}
};

struct sAnimEntry_E448 {
    int pad_0x0[2];
    void* src;              // 0x8
    int pad_0xC;
};

struct sAnimDef_E448 {
    int pad_0x0;
    int count;                  // 0x4
    sAnimEntry_E448* entries;   // 0x8
};

struct sAnimNode_E448 {
    char pad_0x0[0x2C];
    char* res;                  // 0x2C
    char pad_0x30[0x10];
    int numMeshes;              // 0x40
    int pad_0x44;
    sMeshInst_E448* meshes;     // 0x48
    sMeshRef_E448* refs;        // 0x4C
};

extern "C" void cAnimNode_setAnimMeshCache(sAnimNode_E448* self)
{
    sAnimDef_E448* def = *(sAnimDef_E448**)(self->res + 0x80);
    sAnimEntry_E448* e = def->entries;
    int i;
    self->numMeshes = 0;
    for (i = 0; i < def->count; i++, e++) {
        if (e->src != 0)
            self->numMeshes++;
    }
    if (self->numMeshes != 0) {
        sMeshInst_E448*& meshes = self->meshes;
        meshes = new (D_0048E860, 0x20000000, 0) sMeshInst_E448[self->numMeshes];
    } else {
        self->meshes = 0;
    }
    sMeshRef_E448*& refs = self->refs;
    refs = new (D_0048E870, 0x20000000, 0) sMeshRef_E448[def->count];
    e = def->entries;
    int k = 0;
    for (i = 0; i < def->count; i++, e++) {
        if (e->src != 0) {
            func_00351398(&self->meshes[k++], e->src);
        }
    }
}
#endif

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

