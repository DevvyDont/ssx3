#include "common.h"

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_cDataManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void func_00275CD0(void* self, void* node);
extern char D_004825A0[];
extern const char D_00481AB8[];

struct sDataNode5828 {
    char pad[0x18];
};

struct cDataManager5828 {
    int count;              // 0x0
    sDataNode5828* nodes;   // 0x4
    int field_0x8;          // 0x8
    void* vtable;           // 0xC
};

extern "C" cDataManager5828* cDataManager_cDataManager(cDataManager5828* self, int flags, int count)
{
    self->vtable = D_004825A0;
    self->count = count;
    self->nodes = (sDataNode5828*)operator_new_tag(count * sizeof(sDataNode5828), D_00481AB8, flags, 0);
    self->field_0x8 = 0;
    for (int i = 0; i < count; i++) {
        func_00275CD0(self, &self->nodes[i]);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002758C0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char D_004825A0[];

extern "C" void func_002758C0(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_004825A0;
    void* p = *(void**)((char*)self + 0x4);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00275920);

INCLUDE_ASM("scripter/datamanager", func_00275A20);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_00275B08);
#ifdef SKIP_ASM
extern "C" void func_00275A20(void* self, void* entry);

struct sDmEntry275B08 {
    char pad_0x00[0x10];
    int used;
    int unk_0x14;
};

struct sDmTable275B08 {
    int count;
    sDmEntry275B08* entries;
};

extern "C" void func_00275B08(sDmTable275B08* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i].used != 0) {
            func_00275A20(self, &self->entries[i]);
        }
    }
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00275B98);

//100%
INCLUDE_ASM("scripter/datamanager", func_00275CD0__FPvT0);
#ifdef SKIP_ASM
void func_00275CD0(void* self, void* node)
{
    *(int*)((char*)node + 0x8) = 0;
    *(int*)((char*)node + 0xc) = 0;
    *(int*)((char*)node + 0x10) = 0;
    *(int*)((char*)node + 0x0) = 0;
    *(int*)((char*)node + 0x4) = 0;
    *(void**)((char*)node + 0x14) = *(void**)((char*)self + 0x8);
    *(void**)((char*)self + 0x8) = node;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275CF8__FPv);
#ifdef SKIP_ASM
void* func_00275CF8(void* self)
{
    void* head = *(void**)((char*)self + 0x8);
    *(void**)((char*)self + 0x8) = *(void**)((char*)head + 0x14);
    *(void**)((char*)head + 0x14) = 0;
    return head;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275D10);
#ifdef SKIP_ASM
extern "C" void func_00283C30(void* self);
void func_00283C20(void* self);

extern "C" void* func_00275D10(void* self)
{
    void* list = (char*)self + 0xC;
    func_00283C30(list);
    func_00283C20(list);
    *(int*)((char*)self + 0xBC) = -1;
    *(int*)((char*)self + 0x4) = 1;
    *(int*)((char*)self + 0xA4) = 0;
    *(int*)((char*)self + 0xA8) = 0;
    *(int*)((char*)self + 0xC8) = 0;
    *(int*)((char*)self + 0xB0) = 0;
    *(int*)((char*)self + 0xAC) = 0;
    *(int*)((char*)self + 0xB8) = 0;
    *(int*)((char*)self + 0xC0) = 0;
    *(int*)((char*)self + 0xC4) = 0;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0xB4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00275D90);
#ifdef SKIP_ASM
extern "C" void func_00276868(void* self, int a1);
void operator_delete(int* ptr);

extern "C" void func_00275D90(int* self, int flags)
{
    func_00276868(self, 1);
    *self = 0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00275DD8);

INCLUDE_ASM("scripter/datamanager", func_00275ED0);

INCLUDE_ASM("scripter/datamanager", func_00276048);

INCLUDE_ASM("scripter/datamanager", func_00276270);

INCLUDE_ASM("scripter/datamanager", func_00276388);

extern "C" void* func_00277C08(void*, int, int);

//100%
INCLUDE_ASM("scripter/datamanager", func_002766B0__FPvi);
#ifdef SKIP_ASM
void* func_002766B0(void* self, int a1)
{
    return func_00277C08(self, a1, 0);
}
#endif

INCLUDE_ASM("scripter/datamanager", func_002766D0);

INCLUDE_ASM("scripter/datamanager", func_00276868);

//100%
INCLUDE_ASM("scripter/datamanager", func_00276998);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00283D70(void* list, int i);
// The unit declares func_00283D28 as returning void*, but it returns the list's count.
int func_00283D28_count(void* list) __asm__("func_00283D28");

extern "C" int func_00276998(void* self, int i)
{
    void* list = (char*)self + 0xC;
    if (i < func_00283D28_count(list) && *(int*)func_00283D70(list, i) < 0x1C) {
        return *(int*)func_00283D70(list, i);
    }
    return 0x1C;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00276B98);
#ifdef SKIP_ASM
extern "C" void func_00277778(void* self);

extern "C" int func_00276B98(void* self)
{
    if (*(int*)((char*)self + 0xA4) == 1) {
        func_00277778(self);
    }
    return *(int*)((char*)self + 0xA4);
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00276BD8);

extern "C" void* func_00283D28(void*);

//100%
INCLUDE_ASM("scripter/datamanager", func_00276CA8__FPv);
#ifdef SKIP_ASM
void* func_00276CA8(void* self)
{
    return func_00283D28((char*)self + 0xc);
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00276CC8);

INCLUDE_ASM("scripter/datamanager", func_00276F48);

//100%
INCLUDE_ASM("scripter/datamanager", func_00277060);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);

extern "C" int func_00277060(void* self)
{
    if (*(int*)((char*)self + 0xA4) != 3) {
        return 0;
    }
    int r = 0;
    void* e = func_00283D70((char*)self + 0xC, 0);
    if (*(int*)((char*)e + 0x8) & 1) {
        r = *(int*)((char*)self + 0xA8) == 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002770C0);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);

extern "C" int func_002770C0(void* self)
{
    if (*(int*)((char*)self + 0xA4) != 3) {
        return 1;
    }
    void* e = func_00283D70((char*)self + 0xC, 0);
    return (*(int*)((char*)e + 0x8) >> 2) & 1;
}
#endif

INCLUDE_ASM("scripter/datamanager", func_002771C8);

//100%
INCLUDE_ASM("scripter/datamanager", func_00277298);
#ifdef SKIP_ASM
extern "C" int func_00277298(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0xa4) == 3) {
        r = *(int*)((char*)self + 0xa8) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_002772B8__FPv);
#ifdef SKIP_ASM
int func_002772B8(void* self)
{
    return *(int*)((char*)self + 0xB0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_002772C0);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" void func_002EF378(int a0);
extern char D_004A34A8[];

extern "C" void func_002772C0(void* self, int a1, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        char* name = func_0027C070(obj);
        char* msg = D_004A34A8;
        if (*name != 0) {
            func_002EF378((int)msg); // PORT: string pointer passed as int (unit declares func_002EF378(int))
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_00277310);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" int func_00274D08(void* obj);
extern "C" void func_00277980(void* self);

extern "C" void func_00277310(void* self, int a1, void* obj)
{
    if (func_002771C8(self, obj) != 0 && *(int*)((char*)self + 0x4) != 0 &&
        *(int*)((char*)obj + 0x1C) == 0) {
        char* name = func_0027C070(obj);
        if (func_00274D08(obj) + 1 <= *(short*)(name + 0x12)) {
            func_00277980(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_002773A0);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
extern "C" void func_002776E0(void* self);
extern "C" void func_002826D8(void* self, int a1);

extern "C" void func_002773A0(void* self, int a1, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        if (*(int*)((char*)self + 0xB8) != 0) {
            func_002826D8(*(void**)self, a1);
        }
        func_002776E0(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", func_00277400);
#ifdef SKIP_ASM
extern "C" int func_002771C8(void* self, void* obj);
char* func_0027C070(void* obj);
extern "C" void func_002EF378(int a0);

extern "C" void func_00277400(void* self, int a1, void* obj)
{
    if (func_002771C8(self, obj) != 0) {
        if (*func_0027C070(obj) != 0) {
            func_002EF378(0);
        }
    }
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00277450);

INCLUDE_ASM("scripter/datamanager", func_00277598);

//100%
INCLUDE_ASM("scripter/datamanager", func_002776E0);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282798(void* self, int a1);

extern "C" void func_002776E0(void* self)
{
    int idx = *(int*)((char*)self + 0xBC);
    if (idx >= 0) {
        void* list = (char*)self + 0xC;
        char* e = (char*)func_00283D70(list, idx);
        int ok;
        if (func_00277DD8(self, e) == 0) {
            ok = func_00282798(*(void**)self,
                               *(int*)((char*)func_00283D70(list, *(int*)((char*)self + 0xBC)) + 0xC)) == 1;
        } else {
            ok = *(int*)(e + 0xC) < 0;
        }
        if (ok) {
            func_00277C08(self, 1, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277778);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* list, int i);
extern "C" int func_00277DD8(void* self, void* a1);
extern "C" int func_00282798(void* self, int a1);

extern "C" void func_00277778(void* self)
{
    if (*(int*)((char*)self + 0xA4) == 1) {
        char* e = (char*)func_00283D70((char*)self + 0xC, 0);
        if (func_00277DD8(self, e) == 0) {
            int r = func_00282798(*(void**)self, *(int*)(e + 0xC));
            if (r == 2) {
                *(int*)((char*)self + 0xA4) = r;
            }
        } else {
            *(int*)((char*)self + 0xA4) = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", func_00277800);
#ifdef SKIP_ASM
extern "C" void func_002776E0(void* self);
extern "C" void func_00277838(void* self);

extern "C" void func_00277800(void* self)
{
    func_002776E0(self);
    if (*(int*)((char*)self + 0xA8) == 0) {
        func_00277838(self);
    }
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00277838);

INCLUDE_ASM("scripter/datamanager", func_00277980);

INCLUDE_ASM("scripter/datamanager", func_00277C08);

//100%
INCLUDE_ASM("scripter/datamanager", func_00277DD8);
#ifdef SKIP_ASM
extern "C" int func_00277DD8(void* self, void* a1)
{
    return *(int*)a1 >= 0x1d;
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00277DE8);

//100%
INCLUDE_ASM("scripter/datamanager", func_00277F08__FPv);
#ifdef SKIP_ASM
void func_00277F08(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
}
#endif

INCLUDE_ASM("scripter/datamanager", func_00277F20);

INCLUDE_ASM("scripter/datamanager", func_002780B8);

INCLUDE_ASM("scripter/datamanager", func_00278210);

//100%
INCLUDE_ASM("scripter/datamanager", func_00278308);
#ifdef SKIP_ASM
extern "C" void func_00282020(void* self);
extern "C" void* func_002523A8(void* self);

extern "C" void func_00278308(void* self)
{
    func_00282020(self);
    func_002523A8(*(void**)((char*)self + 0x52C));
    if (*(void**)((char*)self + 0x534) != 0) {
        func_002523A8(*(void**)((char*)self + 0x534));
    }
    *(int*)((char*)self + 0x52C) = 0;
    *(int*)((char*)self + 0x530) = 0;
    *(int*)((char*)self + 0x534) = 0;
    *(int*)((char*)self + 0x554) = 0;
}
#endif

