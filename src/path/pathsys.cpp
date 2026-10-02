#include "common.h"

//100%
INCLUDE_ASM("path/pathsys", cPathSys_resolvePaths);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" int func_0026A180(void* path, char* data);
extern char D_00481438[];
extern void* D_00481448[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sPathB310 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    sPathB310() { vtable = D_00481448; }
};

struct sPathSysB310 {
    char pad_0x0[0x10];
    int count;              // 0x10
    sPathB310* paths;       // 0x14
};

extern "C" int cPathSys_resolvePaths(sPathSysB310* self, char* p)
{
    int count;
    sPathB310** slot = &self->paths;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB310), D_00481438, 0, 0)) sPathB310[count];
    for (int i = 0; i < self->count; i++) {
        p += func_0026A180(&self->paths[i], p);
    }
    return p - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B410);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" int func_0026A180(void* path, char* data);
extern char D_004A3408[];
extern void* D_00481478[];

struct sPathB410 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    int field_0x3C;         // 0x3C
    sPathB410() { vtable = D_00481478; }
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sPathSysB410 {
    char pad_0x0[0x8];
    int count;              // 0x8
    sPathB410* paths;       // 0xC
};

extern "C" int func_0026B410(sPathSysB410* self, char* p)
{
    int count;
    sPathB410** slot = &self->paths;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB410), D_004A3408, 0, 0)) sPathB410[count];
    for (int i = 0; i < self->count; i++) {
        p += func_0026A180(&self->paths[i], p);
    }
    return p - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B508);
#ifdef SKIP_ASM
struct sPathEntry28 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x20];
};

struct sPathEntry40 {
    char pad_0x0[0x40];
};

struct sPathEntry3C {
    char pad_0x0[0x3c];
};

struct sPathSys {
    int count28;             // 0x0
    sPathEntry28* entries28; // 0x4
    int count40;             // 0x8
    sPathEntry40* entries40; // 0xc
    int count3C;             // 0x10
    sPathEntry3C* entries3C; // 0x14
};

extern "C" int func_0026B508(sPathSys* self, sPathEntry3C* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count3C; i++) {
            if (&self->entries3C[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B550);
#ifdef SKIP_ASM
extern "C" int func_0026B550(sPathSys* self, sPathEntry40* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count40; i++) {
            if (&self->entries40[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B598);
#ifdef SKIP_ASM
extern "C" int func_0026B598(sPathSys* self, int a, int b)
{
    int i;
    int count = self->count28;
    for (i = 0; i < count; i++) {
        if (self->entries28[i].field_0x4 == a && self->entries28[i].field_0x0 == b) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026B5E0);
#ifdef SKIP_ASM
struct sPathEntry {
    int id;                 // 0x0
    int type;               // 0x4
    char pad_0x8[0x20];
};

struct sPathEntryList {
    int count;              // 0x0
    sPathEntry* entries;    // 0x4
};

extern "C" sPathEntry* func_0026B5E0(sPathEntryList* list, int type, int id)
{
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == type && list->entries[i].id == id) {
            return &list->entries[i];
        }
    }
    if (list->count > 0) {
        return list->entries;
    }
    return 0;
}
#endif

INCLUDE_ASM("path/pathsys", func_0026B680);

//100%
INCLUDE_ASM("path/pathsys", func_0026B7D8);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* dst, void* src);
extern "C" void func_0026BA88(void* dst, void* src);

struct sPathPair_B7D8 {
    int a;      // 0x0
    int b;      // 0x4
};

struct sPathSys_B7D8 {
    char pad_0x0[0x18];
    sPathPair_B7D8 pairs[1];    // 0x18
};

extern "C" int func_0026B7D8(sPathSys_B7D8* self, char* p)
{
    int count;
    char* start = p;
    func_0026BA68(&count, p);
    p += 4;
    for (int i = 0; i < count; i++) {
        func_0026BA88(&self->pairs[i].b, p);
        p += 4;
        func_0026BA68(&self->pairs[i].a, p);
        p += 4;
    }
    return p - start;
}
#endif

INCLUDE_ASM("path/pathsys", func_0026B880);

extern "C" void* func_003E6574(void*, void*, int);

//100%
INCLUDE_ASM("path/pathsys", func_0026BA48);
#ifdef SKIP_ASM
extern "C" void func_0026BA48(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0xc);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BA68);
#ifdef SKIP_ASM
extern "C" void func_0026BA68(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0x4);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BA88);
#ifdef SKIP_ASM
extern "C" void func_0026BA88(void* dst, void* src)
{
    if (dst != src) {
        func_003E6574(dst, src, 0x4);
    }
}
#endif

INCLUDE_ASM("path/pathsys", func_0026BAE8);

//100%
INCLUDE_ASM("path/pathsys", func_0026C410__FPv);
#ifdef SKIP_ASM
void func_0026C410(void* self)
{
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C418__FPv);
#ifdef SKIP_ASM
void func_0026C418(void* self)
{
}
#endif

extern "C" void* func_0026BAE8(int, int);

//99.38%
INCLUDE_ASM("path/pathsys", func_0026C438__FPv);
#ifdef SKIP_ASM
void* func_0026C438(void* self)
{
    return func_0026BAE8(1, 0xffff);
}
#endif

