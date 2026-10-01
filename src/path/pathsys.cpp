#include "common.h"

INCLUDE_ASM("path/pathsys", cPathSys_resolvePaths);

INCLUDE_ASM("path/pathsys", func_0026B410);

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

INCLUDE_ASM("path/pathsys", func_0026B7D8);

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

