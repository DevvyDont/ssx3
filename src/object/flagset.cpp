#include "common.h"

INCLUDE_ASM("object/flagset", cFlagSet_CreateMesh);

INCLUDE_ASM("object/flagset", func_0034B7B8);

INCLUDE_ASM("object/flagset", func_0034B818);

INCLUDE_ASM("object/flagset", func_0034B9B0);

INCLUDE_ASM("object/flagset", func_0034BCA0);

INCLUDE_ASM("object/flagset", func_0034C2E0);

INCLUDE_ASM("object/flagset", func_0034C378);

INCLUDE_ASM("object/flagset", func_0034C428);

INCLUDE_ASM("object/flagset", func_0034C4B8);

INCLUDE_ASM("object/flagset", func_0034C548);

//100%
INCLUDE_ASM("object/flagset", func_0034C600);
#ifdef SKIP_ASM
struct sFlagSetElem_C600 {
    char pad0[0x5C];
    int active; // 0x5C
    char pad60[0x188 - 0x60];
};

struct sFlagSet_C600 {
    char pad0[0x20];
    sFlagSetElem_C600 elems[15]; // 0x20
};

extern "C" int func_0034B168(void* elem, void* arg);

extern "C" void func_0034C600(sFlagSet_C600* self, void* arg)
{
    int i;
    sFlagSetElem_C600* e = self->elems;
    for (i = 0; i < 15; i++, e++) {
        if (e->active != 0 && func_0034B168(e, arg) != 0) {
            return;
        }
    }
}
#endif

INCLUDE_ASM("object/flagset", func_0034C668);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", func_0034C7F8);
#ifdef SKIP_ASM
struct sFlagSetEntry {
    char pad_0x0[0x5C];
    int active; // 0x5C
    char pad_0x60[0x128];
};

struct sFlagSetOwner {
    char pad_0x0[0x20];
    sFlagSetEntry entries[15]; // 0x20
};

extern "C" void func_0034B9B0(sFlagSetEntry* e);

extern "C" void func_0034C7F8(sFlagSetOwner* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        sFlagSetEntry* e = &self->entries[i];
        if (e->active != 0) {
            func_0034B9B0(e);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", func_0034C848);
#ifdef SKIP_ASM
struct sFlagSetElem_C848 {
    char data[0x188];
};

struct sFlagSet_C848 {
    char pad0[0x20];
    sFlagSetElem_C848 elems[15]; // 0x20
};

extern "C" void func_0034C2E0(void* elem, void* arg);

extern "C" void func_0034C848(sFlagSet_C848* self, void* arg)
{
    int i;
    for (i = 0; i < 15; i++) {
        func_0034C2E0(&self->elems[i], arg);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", func_0034C898);
#ifdef SKIP_ASM
struct sFlagSetElem_C898 {
    char data[0x188];
};

struct sFlagSet_C898 {
    char pad0[0x20];
    sFlagSetElem_C898 elems[15]; // 0x20
};

extern "C" void func_0034C378(void* elem, void* arg);

extern "C" void func_0034C898(sFlagSet_C898* self, void* arg)
{
    int i;
    for (i = 0; i < 15; i++) {
        func_0034C378(&self->elems[i], arg);
    }
}
#endif

INCLUDE_ASM("object/flagset", func_0034CAB8);

INCLUDE_ASM("object/flagset", func_0034CB80);

INCLUDE_ASM("object/flagset", func_0034CBE8);

INCLUDE_ASM("object/flagset", func_0034CC80);

//100%
INCLUDE_ASM("object/flagset", func_0034CDE8);
#ifdef SKIP_ASM
extern "C" void func_0034D6F0(void* self, float dt);

extern "C" void func_0034CDE8(void* self, int on, float dt)
{
    func_0034D6F0(self, dt);
    if (on) {
        if (*(int*)((char*)self + 0x38) == 0) {
            *(int*)((char*)self + 0x38) = 4;
        }
    } else {
        if (*(int*)((char*)self + 0x3C) == 0) {
            *(int*)((char*)self + 0x3C) = 4;
        }
    }
}
#endif

INCLUDE_ASM("object/flagset", func_0034CE48);

INCLUDE_ASM("object/flagset", func_0034CF98);

INCLUDE_ASM("object/flagset", func_0034D1E8);

INCLUDE_ASM("object/flagset", func_0034D650);

INCLUDE_ASM("object/flagset", func_0034D6F0);

INCLUDE_ASM("object/flagset", func_0034D778);

//100%
INCLUDE_ASM("object/flagset", func_0034D960);
#ifdef SKIP_ASM
struct sSerVEntry_0034D960 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_0034D960(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sSerVEntry_0034D960* vt = *(sSerVEntry_0034D960**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x24);
}
#endif

INCLUDE_ASM("object/flagset", func_0034D9B0);

INCLUDE_ASM("object/flagset", func_0034DAC8);

INCLUDE_ASM("object/flagset", func_0034DBA8);

INCLUDE_ASM("object/flagset", func_0034DC90);

INCLUDE_ASM("object/flagset", func_0034DD18);

//100%
INCLUDE_ASM("object/flagset", func_0034E320);
#ifdef SKIP_ASM
extern char D_004FF1A0[];

extern "C" void* func_0034E320(void* self, int i) {
    char* base = *(char**)((char*)self + 0x48);
    if (base == 0) {
        return D_004FF1A0;
    }
    return base + i * 0xD0 + 0x90;
}
#endif

INCLUDE_ASM("object/flagset", func_0034E348);

//100%
INCLUDE_ASM("object/flagset", func_0034E3D8);
#ifdef SKIP_ASM
struct sSerVEntry_0034E3D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00356B30(void* self, void* stream);

extern "C" void func_0034E3D8(void* self, void* stream)
{
    int dummy;
    func_00356B30((char*)self + 0x14, stream);
    sSerVEntry_0034E3D8* e = &(*(sSerVEntry_0034E3D8**)stream)[1];
    e->fn((char*)stream + e->delta, self, 0x14);
    e = &(*(sSerVEntry_0034E3D8**)stream)[1];
    e->fn((char*)stream + e->delta, &dummy, 4);
}
#endif

