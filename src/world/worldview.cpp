#include "common.h"

struct cWorldViewSectionList {
    char pad_0x00[0x8];
    int mNumSections; // 0x8
};

struct cWorldView {
    char pad_0x00[0x4];
    cWorldViewSectionList* mSections; // 0x4
};

//100%
INCLUDE_ASM("world/worldview", cWorldView_getNumSections__FP10cWorldView);
#ifdef SKIP_ASM
int cWorldView_getNumSections(cWorldView* self)
{
    if (self->mSections == 0) {
        return 0;
    }
    return self->mSections->mNumSections;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003A9820);
#ifdef SKIP_ASM
extern "C" void* func_003A9820(cWorldView* self, unsigned int i)
{
    cWorldViewSectionList* list = self->mSections;
    if (list == 0 || i >= (unsigned int)list->mNumSections) {
        return 0;
    }
    return *(char**)((char*)self + 0x8) + i * 0x58;
}
#endif

struct cWorldViewEntry {
    char pad_0x00[0x14];
    int field_0x14;
};

//100%
INCLUDE_ASM("world/worldview", cWorldView_isSectionLoaded__FP10cWorldViewi);
#ifdef SKIP_ASM
int cWorldView_isSectionLoaded(cWorldView* self, int section)
{
    if (self->mSections == 0 || (unsigned int)section >= (unsigned int)self->mSections->mNumSections) {
        return 0;
    }
    cWorldViewEntry* entry = (cWorldViewEntry*)((char*)self + section * 8);
    return (unsigned int)(entry->field_0x14 - 5) < 2;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003A9890);
#ifdef SKIP_ASM
extern "C" int func_003A9890(cWorldView* self, unsigned int i)
{
    cWorldViewSectionList* list = self->mSections;
    if (list == 0 || i >= (unsigned int)list->mNumSections) {
        return 0;
    }
    return *(int*)((char*)self + (i << 3) + 0x14) == 0;
}
#endif

INCLUDE_ASM("world/worldview", func_003A98C8);

INCLUDE_ASM("world/worldview", func_003A9958);

//100%
INCLUDE_ASM("world/worldview", func_003A99D8);
#ifdef SKIP_ASM
extern "C" void func_003A8F10(void* world, unsigned int i);
struct func_003A99D8_sEntry { int state; int field_0x4; };
struct func_003A99D8_sView { void* world; cWorldViewSectionList* mSections; };
extern "C" int func_003A99D8(func_003A99D8_sView* self, unsigned int i)
{
    cWorldViewSectionList* list = self->mSections;
    if (list != 0 && i < (unsigned int)list->mNumSections) {
        int off = i << 3;
        char* base = (char*)self + 0x14;
        func_003A99D8_sEntry* e = (func_003A99D8_sEntry*)(base + off);
        int st = e->state;
        if (st == 5) {
            e->state = 6;
            func_003A8F10(self->world, i);
            return 1;
        } else if (st == 6) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003A9A98);
#ifdef SKIP_ASM
extern "C" void* func_003A9A98(void* self, int a1)
{
    return (char*)(*(void**)((char*)self + 0x10)) + a1 * 0x44;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003A9AB0__FPv);
#ifdef SKIP_ASM
int func_003A9AB0(void* self)
{
    return *(int*)((char*)*(void**)((char*)self + 0x4) + 0x10);
}
#endif

INCLUDE_ASM("world/worldview", func_003A9D60);

INCLUDE_ASM("world/worldview", func_003A9E50);

INCLUDE_ASM("world/worldview", func_003AA028);

INCLUDE_ASM("world/worldview", func_003AA2F0);

//100%
INCLUDE_ASM("world/worldview", func_003AA3F0);
#ifdef SKIP_ASM
class func_003AA3F0_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06(void* a1);
};

// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int func_003AA3F0(func_003AA3F0_cObj* self, void* a1)
{
    *(int*)((char*)a1 + 0x8) += (int)a1;
    *(int*)((char*)a1 + 0xC) += (int)a1;
    self->v06(a1);
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AA438);

INCLUDE_ASM("world/worldview", func_003AA520);

//100%
INCLUDE_ASM("world/worldview", func_003AA5C8);
#ifdef SKIP_ASM
class func_003AA5C8_cObj {
public:
    char pad00[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual int v15(void* a1);
};

// PORT: stores a pointer in an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int func_003AA5C8(func_003AA5C8_cObj* self, void* a1)
{
    *(int*)((char*)a1 + 0x8) += (int)a1;
    self->v15(a1);
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AA608);

//100%
INCLUDE_ASM("world/worldview", func_003AA6E8);
#ifdef SKIP_ASM
extern "C" int func_003AA6E8(void* self, void* a1)
{
    *(int*)((char*)a1 + 0x20) += (int)a1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AA700);
#ifdef SKIP_ASM
// PORT: decodes an int handle into a pointer ((void*)(i << 2)); not 64-bit safe.
extern "C" int func_003AA700(void* self, void* a1)
{
    unsigned int h = *(unsigned int*)((char*)a1 + 0x64);
    char* sec = (*(char***)(**(char***)self + 8))[h & 0xFF];
    void* res;
    unsigned int i;
    if (sec == 0 || (i = (*(unsigned int**)(sec + 0x24))[h >> 8] >> 8) == 0) {
        res = 0;
    } else {
        res = (void*)(i << 2);
    }
    *(void**)((char*)a1 + 0x64) = res;
    *(int*)((char*)a1 + 0x84) = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AA758);
#ifdef SKIP_ASM
// PORT: stores a pointer in an int field (int typing needed to match); not 64-bit safe.
extern "C" int func_003AA758(void* self, void* a1)
{
    if (*(int*)((char*)a1 + 0x10) == -1) {
        *(int*)((char*)a1 + 0x10) = 0;
    } else {
        *(int*)((char*)a1 + 0x10) = (int)((char*)a1 + 0x14);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AA780);
#ifdef SKIP_ASM
// PORT: decodes int handles into pointers ((void*)(i << 2)); not 64-bit safe.
extern "C" int func_003AA780(void* self, int n, int* data)
{
    int g;
    for (g = 0; g < n; g++) {
        int j;
        unsigned int* hp = (unsigned int*)(data + 1);
        for (j = 0; j < *data; j++, hp++) {
            unsigned int h = *hp;
            char* sec = (*(char***)(**(char***)self + 8))[h & 0xFF];
            void* res;
            unsigned int i;
            if (sec == 0 || (i = (*(unsigned int**)(sec + 0x4))[h >> 8] >> 8) == 0) {
                res = 0;
            } else {
                res = (void*)(i << 2);
            }
            *(void**)hp = res;
        }
        data += *data + 1;
    }
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AA830);

//100%
INCLUDE_ASM("world/worldview", func_003AA8D8);
#ifdef SKIP_ASM
extern "C" int func_003AA8D8(void* self, void* a1, int a2)
{
    *(int*)((char*)a1 + 0x4) += a2;
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AA8F0);

//100%
INCLUDE_ASM("world/worldview", func_003AA960);
#ifdef SKIP_ASM
struct func_003AA960_sNode {
    char pad[0x60];
    int prev;       // 0x60 (index fixed up into a pointer)
    int next;       // 0x64
    void* owner;    // 0x68
    char pad6C[0x90 - 0x6C];
};

// PORT: stores pointers in int fields (int typing needed to match); not 64-bit safe.
extern "C" int func_003AA960(void* self, void* list)
{
    int i;
    int count = *(int*)((char*)list + 0x20);
    func_003AA960_sNode* nodes = (func_003AA960_sNode*)((char*)list + 0x30);
    int next = 1;
    *(func_003AA960_sNode**)((char*)list + 0x24) = nodes;
    int prev = count - 1;
    for (i = 0; i < *(int*)((char*)list + 0x20); i++) {
        if (nodes[i].prev != -1) {
            nodes[i].prev = (int)&nodes[prev];
        } else {
            nodes[i].prev = 0;
        }
        if (nodes[i].next != -1) {
            nodes[i].next = (int)&nodes[next];
        } else {
            nodes[i].next = 0;
        }
        prev++;
        next++;
        if (prev >= *(int*)((char*)list + 0x20)) {
            prev = 0;
        }
        if (next >= *(int*)((char*)list + 0x20)) {
            next = 0;
        }
        nodes[i].owner = list;
    }
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AAA08);

//100%
INCLUDE_ASM("world/worldview", func_003AABD8);
#ifdef SKIP_ASM
// PORT: func_003AD230__FPv is declared (void*) but takes a flags int and returns a float.
float func_003AD230_f(int flags) __asm__("func_003AD230__FPv");

struct func_003AABD8_sObj {
    char pad_0x00[0xC];
    int flags;          // 0xC
    char pad_0x10[0xC];
    float pos[3];       // 0x1C
    float min[3];       // 0x28
    float max[3];       // 0x34
    int field_0x40;
    int field_0x44;
};

extern "C" int func_003AABD8(void* self, func_003AABD8_sObj* obj)
{
    float r = func_003AD230_f(obj->flags & 0x70);
    obj->field_0x44 = 0;
    obj->field_0x40 = 0;
    obj->min[0] = obj->pos[0] - r;
    obj->min[1] = obj->pos[1] - r;
    obj->min[2] = obj->pos[2] - r;
    obj->max[0] = obj->pos[0] + r;
    obj->max[1] = obj->pos[1] + r;
    obj->max[2] = obj->pos[2] + r;
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AAC50);

INCLUDE_ASM("world/worldview", func_003AACA8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldview", func_003AAD98);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void func_003AA2F0(void* self);
extern char D_00494F70[];
extern void* D_00495090[];

extern "C" void* func_003AAD98()
{
    void* p = cMemMan_alloc(0x14, D_00494F70, 0, 0);
    func_003AA2F0(p);
    *(void***)((char*)p + 0x8) = D_00495090;
    return p;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003AADE8);
#ifdef SKIP_ASM
void func_003AAE60(void*);
void operator_delete(int*);
extern void* D_00495090[];
extern void* D_00495150[];

extern "C" void func_003AADE8(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00495090;
    func_003AAE60(self);
    *(void***)((char*)self + 0x8) = D_00495150;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("world/worldview", func_003AAE40);

//100%
INCLUDE_ASM("world/worldview", func_003AAE60__FPv);
#ifdef SKIP_ASM
void func_003AAE60(void* self)
{
}
#endif

INCLUDE_ASM("world/worldview", func_003AAE68);

INCLUDE_ASM("world/worldview", func_003AB498);

INCLUDE_ASM("world/worldview", func_003ABE40);

//100%
INCLUDE_ASM("world/worldview", func_003ABF20);
#ifdef SKIP_ASM
extern "C" void func_003ABF50(void* self, void* a1);

// PORT: adds a pointer to an int field (offset -> pointer fixup); not 64-bit safe.
extern "C" int func_003ABF20(void* self, void* a1)
{
    *(int*)((char*)a1 + 0x24) += (int)a1;
    func_003ABF50(self, a1);
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003ABF50);
#ifdef SKIP_ASM
extern "C" void func_003ABFC0(void* self, void* item, void* list);

struct func_003ABF50_sEntry {
    int field_0x0;
    void* item;     // 0x4
    int field_0x8;
    int field_0xC;
};

struct func_003ABF50_sList {
    int field_0x0;
    int count;                       // 0x4
    func_003ABF50_sEntry* entries;   // 0x8
};

// PORT: the unit declares func_003ABF50 as returning void (func_003ABF20 calls it that way),
// but the body returns 1; bind the int-returning body to the symbol with an asm label.
int func_003ABF50_impl(void* self, func_003ABF50_sList* list) __asm__("func_003ABF50");

int func_003ABF50_impl(void* self, func_003ABF50_sList* list)
{
    int n = list->count;
    func_003ABF50_sEntry* e = list->entries;
    int i;
    for (i = 0; i < n; i++, e++) {
        if (e->item != 0) {
            func_003ABFC0(self, e->item, list);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldview", func_003ABFC0);
#ifdef SKIP_ASM
extern "C" void func_003AC048(void* self, void* obj, void* list, int index);

// PORT: the unit declares func_003ABFC0 as returning void (func_003ABF50 calls it that way),
// but the body returns 1; bind the int-returning body to the symbol with an asm label.
int func_003ABFC0_impl(void* self, void* item, void* list) __asm__("func_003ABFC0");

int func_003ABFC0_impl(void* self, void* item, void* list)
{
    void** p = *(void***)((char*)item + 0x20);
    int i;
    for (i = 0; i < *(int*)((char*)item + 0x1C); i++, p++) {
        func_003AC048(self, *p, list, i);
    }
    return 1;
}
#endif

INCLUDE_ASM("world/worldview", func_003AC048);

INCLUDE_ASM("world/worldview", func_003AC2A8);

INCLUDE_ASM("world/worldview", func_003AC358);

INCLUDE_ASM("world/worldview", func_003AC508);

//100%
INCLUDE_ASM("world/worldview", func_003AC7C8);
#ifdef SKIP_ASM
extern "C" void cWScriptCache_init(void* self, int a1);

extern "C" void* func_003AC7C8(void* self, int a1)
{
    *(int*)((char*)self + 0x0) = a1;
    *(int*)((char*)self + 0x4) = 0;
    cWScriptCache_init(self, 1);
    return self;
}
#endif

