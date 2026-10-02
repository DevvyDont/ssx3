#include "common.h"

INCLUDE_ASM("world/worldcache", cWorldBlockAllocator_init);

INCLUDE_ASM("world/worldcache", func_003A76C0);

//100%
INCLUDE_ASM("world/worldcache", func_003A7768__FPv);
#ifdef SKIP_ASM
void* func_003A7768(void* self)
{
    void* item = *(void**)((char*)self + 0x10);
    *(void**)((char*)self + 0x10) = *(void**)((char*)item + 0x14);
    *(int*)((char*)self + 0x8) -= 1;
    *(int*)((char*)item + 0x10) = 1;
    return item;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A7790__FPvT0);
#ifdef SKIP_ASM
void func_003A7790(void* self, void* item)
{
    *(int*)((char*)item + 0x10) = 0;
    *(void**)((char*)item + 0x14) = *(void**)((char*)self + 0x10);
    *(void**)((char*)self + 0x10) = item;
    *(int*)((char*)self + 0x8) += 1;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A77B0__FPvii);
#ifdef SKIP_ASM
void* func_003A77B0(void* self, int a1, int a2)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = a2;
    *(int*)self = a1;
    *(int*)((char*)self + 0x8) = t0;
    *(int*)((char*)self + 0xc) = t0;
    *(int*)((char*)self + 0x10) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A77D0);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void func_003A7818(void* self);

extern "C" void func_003A77D0(void* self, int flags)
{
    func_003A7818(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", func_003A7818);
#ifdef SKIP_ASM
void func_003A7790(void* self, void* item);

extern "C" void func_003A7818(void* self)
{
    if (*(void**)((char*)self + 0xC) != 0) {
        void* next;
        do {
            void* item = *(void**)((char*)self + 0xC);
            next = *(void**)((char*)item + 0x14);
            func_003A7790(*(void**)self, item);
            *(void**)((char*)self + 0xC) = next;
            *(int*)((char*)self + 0x8) = *(int*)((char*)self + 0x8) - 1;
        } while (next != 0);
    }
}
#endif

INCLUDE_ASM("world/worldcache", func_003A7878);

INCLUDE_ASM("world/worldcache", func_003A7A20);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", func_003A7AA8);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void func_003A76C0(void* self, int flags);

struct func_003A7AA8_sElem {
    char pad[0x14];
};

extern "C" void func_003A7AA8(func_003A7AA8_sElem* self, int flags)
{
    if (self != 0) {
        func_003A7AA8_sElem* p = self + 3;
        while (self != p) {
            p--;
            func_003A76C0(p, 0);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan_activateSectionMem);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* func_003A77B0(void* self, int section, int index);
extern char D_00494DE8[];

struct cWorldMemoryMan_sSection {
    char pad[0x14];
};

// PORT: the section pointer is passed as int (the callee's mangled name is __FPvii).
extern "C" void cWorldMemoryMan_activateSectionMem(void* self, int index, int section)
{
    void** mem = (void**)((char*)self + 0x3C);
    void** slot = &mem[index];
    *slot = func_003A77B0(cMemMan_alloc(0x14, D_00494DE8, 0x20000000, 0),
                          (int)((cWorldMemoryMan_sSection*)self + section), index);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", func_003A7B98);
#ifdef SKIP_ASM
extern "C" void func_003A77D0(void* self, int flags);

extern "C" void func_003A7B98(void* self, int i)
{
    char* base = (char*)self + 0x3C;
    void** p = (void**)(base + (i << 2));
    if (*p != 0) {
        func_003A77D0(*p, 3);
    }
    *p = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cHullPage_cHullPage);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00494E00[];

extern "C" void* cHullPage_cHullPage(void* self, unsigned short* data)
{
    *(unsigned short**)self = data;
    unsigned short n = *data;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x4) = n;
    *(void**)((char*)self + 0xC) = operator_new_tag(n * 4, D_00494E00, 0x20000000, 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A7C30);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_003A7C30(int* self, int flags)
{
    *(int*)((char*)self + 0x4) = 0;
    if (*(void**)((char*)self + 0xC) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xC));
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("world/worldcache", cWorldCacheTable_cWorldCacheTable);

//100%
INCLUDE_ASM("world/worldcache", func_003A7D80);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

extern "C" void func_003A7D80(int* self, int flags)
{
    if (*(void**)self != 0) {
        cMemMan_free(*(void**)self);
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("world/worldcache", func_003A7DD0);

//100%
INCLUDE_ASM("world/worldcache", func_003A7E38);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void func_003A7E98(void* self);
extern "C" void func_003A6E20(void* p, int flags);
extern "C" void func_003A8FB8(void* p, int flags);

extern "C" void func_003A7E38(int* self, int flags)
{
    func_003A7E98(self);
    func_003A6E20((char*)self + 0x300, 2);
    func_003A8FB8((char*)self + 0x10, 2);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("world/worldcache", func_003A7E98);

//100%
INCLUDE_ASM("world/worldcache", func_003A7F90);
#ifdef SKIP_ASM
extern "C" void func_003A7E98(void* self);

extern "C" void func_003A7F90(void* self)
{
    func_003A7E98(self);
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x3EC) = 0;
    *(int*)((char*)self + 0x3E8) = 0;
}
#endif

INCLUDE_ASM("world/worldcache", cWorldCache_init);

INCLUDE_ASM("world/worldcache", cWorldCache_activateSectionMem);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", func_003A8230);
#ifdef SKIP_ASM
extern "C" void func_003A7B98(void* self, int i);
extern "C" void func_003A7D80(int* self, int flags);

extern "C" void func_003A8230(void* self, int i)
{
    func_003A7B98(*(void**)((char*)self + 0xC), i);
    int* e = (*(int***)((char*)self + 0x8))[i];
    if (e != 0) {
        func_003A7D80(e, 3);
    }
    (*(int***)((char*)self + 0x8))[i] = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A8290);
#ifdef SKIP_ASM
extern "C" void func_003A9258(void* p);
extern "C" void cWorldCache_updatePages(void* self);
extern "C" void func_003A7098(void* p);

extern "C" void func_003A8290(void* self)
{
    func_003A9258((char*)self + 0x10);
    cWorldCache_updatePages(self);
    func_003A7098((char*)self + 0x300);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A82C8);
#ifdef SKIP_ASM
int func_003A9AB0(void*);

struct func_003A82C8_sEntry {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

struct func_003A82C8_sCache {
    char pad[0x3EC];
    unsigned int count;   // 0x3EC
    func_003A82C8_sEntry entries[1]; // 0x3F0
};

extern "C" void func_003A82C8(func_003A82C8_sCache* self)
{
    unsigned int i;
    self->count = func_003A9AB0((char*)self + 0x10);
    for (i = 0; i < self->count; i++) {
        self->entries[i].a = 0;
        self->entries[i].b = 0;
        self->entries[i].c = 0;
        self->entries[i].e = 0;
        self->entries[i].f = 0;
        self->entries[i].d = 0;
    }
}
#endif

INCLUDE_ASM("world/worldcache", func_003A8330);

INCLUDE_ASM("world/worldcache", func_003A8448);

INCLUDE_ASM("world/worldcache", func_003A84C8);

//100%
INCLUDE_ASM("world/worldcache", func_003A84F8);
#ifdef SKIP_ASM
extern "C" void func_003A84F8(void* self, int a1, int a2)
{
    self = (char*)self + a1 * 0x18;
    *(int*)((char*)self + 0x3f4) = a2;
}
#endif

INCLUDE_ASM("world/worldcache", func_003A8528);

//100%
INCLUDE_ASM("world/worldcache", func_003A8618);
#ifdef SKIP_ASM
extern "C" int func_003A8618(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x18;
    return *(int*)(p + 0x3f0) == 3;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A8638);
#ifdef SKIP_ASM
extern "C" int func_003A8638(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x18;
    return *(int*)(p + 0x3f0) == 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A8650);
#ifdef SKIP_ASM
extern "C" void func_003A8650(void* self, int a1)
{
    char* p = (char*)self + a1 * 0x18;
    *(int*)(p + 0x3f0) = 3;
}
#endif

INCLUDE_ASM("world/worldcache", cWorldCache_updatePages);

INCLUDE_ASM("world/worldcache", func_003A88A8);

INCLUDE_ASM("world/worldcache", cWorldCache_addBxStreamDataTest);

INCLUDE_ASM("world/worldcache", func_003A8CD0);

INCLUDE_ASM("world/worldcache", func_003A8E20);

INCLUDE_ASM("world/worldcache", func_003A8F10);

INCLUDE_ASM("world/worldcache", func_003A8F80);

//100%
INCLUDE_ASM("world/worldcache", func_003A8FB8);
#ifdef SKIP_ASM
void operator_delete(int*);
void func_003A9180(void* self);

extern "C" void func_003A8FB8(void* self, int flags)
{
    func_003A9180(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A9000);
#ifdef SKIP_ASM
struct sWCQuad {
    float v[4];
} __attribute__((aligned(16)));

struct sWCNode {
    sWCQuad min;
    sWCQuad max;
    char pad20[0x30];
    int left;
    int right;
    int leaf;
    char pad5C[4];
};

struct sWCLeaf {
    char pad00[0x44];
};

struct sWCSect {
    char pad00[0x1C];
    int node;
    char pad20[0x38];
};

struct sWCHdr {
    char pad00[8];
    unsigned int numSects;
    unsigned int numNodes;
    char pad10[0x40];
};

struct sWCPair {
    int a;
    int b;
};

struct sWorldCacheMap {
    int id;
    sWCHdr* hdr;
    sWCSect* sects;
    sWCNode* nodes;
    sWCLeaf* leaves;
    sWCPair pairs[65];
    char pad21C[4];
    sWCQuad min;
    sWCQuad max;
};

// PORT: offsets in the loaded data are rewritten in place as pointers (pointer in int)
extern "C" int func_003A9000(sWorldCacheMap* self, sWCHdr* hdr, int id)
{
    unsigned int i;

    char* p;
    self->id = id;
    self->hdr = hdr;
    self->sects = (sWCSect*)((char*)hdr + 0x50);
    p = (char*)self->sects + hdr->numSects * 0x58;
    p = (char*)(((unsigned int)p + 0xF) & 0xFFFFFFF0);
    self->nodes = (sWCNode*)p;
    self->leaves = (sWCLeaf*)(p + hdr->numNodes * 0x60);

    for (i = 0; i < self->hdr->numSects; i++) {
        self->sects[i].node = (int)&self->nodes[self->sects[i].node];
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }

    for (i = 0; i < self->hdr->numNodes; i++) {
        if (self->nodes[i].left >= 0) {
            self->nodes[i].left = (int)&self->nodes[self->nodes[i].left];
        } else {
            self->nodes[i].left = 0;
        }
        if (self->nodes[i].right >= 0) {
            self->nodes[i].right = (int)&self->nodes[self->nodes[i].right];
        } else {
            self->nodes[i].right = 0;
        }
        if (self->nodes[i].leaf >= 0) {
            self->nodes[i].leaf = (int)&self->leaves[self->nodes[i].leaf];
        } else {
            self->nodes[i].leaf = 0;
        }
    }

    self->min = ((sWCNode*)self->sects[0].node)->min;
    self->max = ((sWCNode*)self->sects[0].node)->max;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", func_003A9180__FPv);
#ifdef SKIP_ASM
void func_003A9180(void* self)
{
}
#endif

INCLUDE_ASM("world/worldcache", func_003A9188);

INCLUDE_ASM("world/worldcache", func_003A9258);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", func_003A9558);
#ifdef SKIP_ASM
extern "C" void func_003A84C8(void* cache, unsigned short id);

struct func_003A9558_sNode {
    char pad_0x00[0x50];
    func_003A9558_sNode* left;   // 0x50
    func_003A9558_sNode* right;  // 0x54
    unsigned short* section;     // 0x58
};

extern "C" void func_003A9558(void** self, func_003A9558_sNode* node)
{
    if (node->section != 0) {
        func_003A84C8(*self, node->section[1]);
    }
    if (node->left != 0) {
        func_003A9558(self, node->left);
    }
    if (node->right != 0) {
        func_003A9558(self, node->right);
    }
}
#endif

INCLUDE_ASM("world/worldcache", func_003A95C0);

INCLUDE_ASM("world/worldcache", func_003A9658);

INCLUDE_ASM("world/worldcache", func_003A96E0);

INCLUDE_ASM("world/worldcache", func_003A9770);

