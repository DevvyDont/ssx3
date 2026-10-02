#include "common.h"

INCLUDE_ASM("object/flagset", cFlagSet_CreateMesh);

INCLUDE_ASM("object/flagset", func_0034B7B8);

INCLUDE_ASM("object/flagset", func_0034B818);

INCLUDE_ASM("object/flagset", func_0034B9B0);

INCLUDE_ASM("object/flagset", func_0034BCA0);

//100%
INCLUDE_ASM("object/flagset", func_0034C2E0);
#ifdef SKIP_ASM
struct sFlagSetVEntryC2E0a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFlagSetVEntryC2E0b {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sFlagSet_C2E0 {
    char pad0[0x5C];
    int count;          // 0x5C
    char pad60[0x34];
    void* items[1];     // 0x94
};

extern "C" void func_0034C2E0(void* elem, void* stream)
{
    int i;
    sFlagSet_C2E0* self = (sFlagSet_C2E0*)elem;
    sFlagSetVEntryC2E0a* e = &(*(sFlagSetVEntryC2E0a**)stream)[1];
    e->fn((char*)stream + e->delta, self, 0x60);
    for (i = 0; i < self->count; i++) {
        sFlagSetVEntryC2E0b* e2 = &(*(sFlagSetVEntryC2E0b**)stream)[5];
        e2->fn((char*)stream + e2->delta, self->items[i]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", func_0034C378);
#ifdef SKIP_ASM
struct sFlagSetVEntryC378a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFlagSetVEntryC378b {
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void cFlagSet_CreateMesh(void* self, void* item);

extern "C" void func_0034C378(void* elem, void* stream)
{
    int i;
    sFlagSet_C2E0* self = (sFlagSet_C2E0*)elem;
    sFlagSetVEntryC378a* e = &(*(sFlagSetVEntryC378a**)stream)[2];
    e->fn((char*)stream + e->delta, self, 0x60);
    for (i = 0; i < self->count; i++) {
        sFlagSetVEntryC378b* e2 = &(*(sFlagSetVEntryC378b**)stream)[3];
        self->items[i] = e2->fn((char*)stream + e2->delta);
    }
    if (self->count > 0) {
        cFlagSet_CreateMesh(self, self->items[0]);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", func_0034C428);
#ifdef SKIP_ASM
extern "C" void* func_00354648(void* self, void* a1);
extern "C" void* func_0034AF38(void* self);
extern void* D_0048FB80[];

struct sFlagSetElem_C428 {
    char data[0x188];
};

struct sFlagSet_C428 {
    char pad0[0xC];
    void** vtable;                  // 0xC
    int field_0x10;                 // 0x10
    float field_0x14;               // 0x14
    float field_0x18;               // 0x18
    int field_0x1c;                 // 0x1C
    sFlagSetElem_C428 elems[15];    // 0x20
};

extern "C" sFlagSet_C428* func_0034C428(sFlagSet_C428* self, void* a1)
{
    int i;
    sFlagSetElem_C428* p = self->elems;
    func_00354648(self, a1);
    self->vtable = D_0048FB80;
    for (i = 14; i != -1; i--, p++) {
        func_0034AF38(p);
    }
    self->field_0x14 = 0.5f;
    self->field_0x18 = 0.25f;
    self->field_0x1c = 0;
    self->field_0x10 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flagset", func_0034C4B8);
#ifdef SKIP_ASM
extern "C" void func_003546C8(void* self, int flags);
extern void* D_0048FB80[];

struct sFlagSetVEntry_C4B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sFlagSetElem_C4B8 {
    char pad0[0x184];
    sFlagSetVEntry_C4B8* vt;        // 0x184
};

struct sFlagSet_C4B8 {
    char pad0[0xC];
    void** vtable;                  // 0xC
    char pad10[0x10];
    sFlagSetElem_C4B8 elems[15];    // 0x20
};

extern "C" void func_0034C4B8(sFlagSet_C4B8* self, int flags)
{
    self->vtable = D_0048FB80;
    if (self->elems != 0) {
        sFlagSetElem_C4B8* p = self->elems + 15;
        while (self->elems != p) {
            p--;
            p->vt[1].fn((char*)p + p->vt[1].delta, 0);
        }
    }
    func_003546C8(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flagset", func_0034C548);
#ifdef SKIP_ASM
struct sFlagSetElem_C548 {
    char pad0[0x5C];
    int active; // 0x5C
    char pad60[0x188 - 0x60];
};

struct sFlagSet_C548 {
    char pad0[0x20];
    sFlagSetElem_C548 elems[15]; // 0x20
};

extern "C" int func_0034AFE8(void* self, void* data, void* ctx);
extern "C" void func_0034B038(void* self, void* data, void* ctx);

extern "C" void func_0034C548(sFlagSet_C548* self, void* node, void* desc)
{
    int i;
    sFlagSetElem_C548* e;
    for (i = 0, e = self->elems; i < 15; i++, e++) {
        if (e->active != 0 && func_0034AFE8(e, desc, node) != 0) {
            func_0034B038(e, desc, node);
            return;
        }
    }
    sFlagSetElem_C548* f = self->elems;
    for (int j = 0; j < 15; j++, f++) {
        if (f->active == 0) {
            func_0034B038(f, desc, node);
            return;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("object/flagset", func_0034DBA8);
#ifdef SKIP_ASM
struct sFlagVEntry_DBA8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sFlagItem_DBA8 {
    char pad_0x0[0x84];
    sFlagVEntry_DBA8* vt;       // 0x84
    char pad_0x88[0x48];
};

struct sFlagOwner_DBA8 {
    char pad_0x0[0x14];
    char node[0xC];             // 0x14
    void** vtable;              // 0x20
    char pad_0x24[0x20];
    void* cache;                // 0x44
    sFlagItem_DBA8* items;      // 0x48, array-new block (count at -0x10)
    void* buf;                  // 0x4C
};

void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void func_003553C0(void* self, int flags);
extern void* D_00490CC8[];

extern "C" void func_0034DBA8(sFlagOwner_DBA8* self, int flags)
{
    self->vtable = D_00490CC8;
    if (self->cache != 0) {
        cMemMan_free(self->cache);
    }
    sFlagItem_DBA8* items = self->items;
    if (items != 0) {
        sFlagItem_DBA8* p = items + ((int*)items)[-4];
        while (self->items != p) {
            p--;
            p->vt[1].fn((char*)p + p->vt[1].delta, 0);
        }
        cMemMan_free((char*)self->items - 0x10);
    }
    if (self->buf != 0) {
        cMemMan_free(self->buf);
    }
    func_003553C0(self->node, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", func_0034DC90);
#ifdef SKIP_ASM
struct sFlagSetVEntry_DC90a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sFlagSetVEntry_DC90b {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sFlagSetVEntry_DC90c {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034DC90(void* self)
{
    sFlagSetVEntry_DC90a* vt = *(sFlagSetVEntry_DC90a**)((char*)self + 0x20);
    vt[52].fn((char*)self + vt[52].delta);
    char* sub = (char*)self + 0x14;
    sFlagSetVEntry_DC90c* vt2 = *(sFlagSetVEntry_DC90c**)((char*)self + 0x20);
    void* objB = sub + vt2[33].delta;
    void* r = ((sFlagSetVEntry_DC90b*)vt2)[24].fn(sub + vt2[24].delta);
    vt2[33].fn(objB, r, *(int*)((char*)self + 0x44));
    *(unsigned short*)((char*)self + 0x26) &= ~1;
}
#endif

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

//100%
INCLUDE_ASM("object/flagset", func_0034E348);
#ifdef SKIP_ASM
struct sFlagSetVEntry_E348 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sFlagItem_E348 {
    char pad0[0x84];
    sFlagSetVEntry_E348* vt;    // 0x84
    char pad88[0x48];
};

struct sFlagOwner_E348 {
    int field_0x0;
    float time;                 // 0x4
    char pad8[0x8];
    int dirty;                  // 0x10
    char pad14[0x2C];
    int count;                  // 0x40
    int field_0x44;
    sFlagItem_E348* items;      // 0x48
};

extern "C" void func_0034E348(sFlagOwner_E348* self)
{
    int i;
    if (self->items != 0) {
        for (i = 0; i < self->count; i++) {
            // PORT: pointer arithmetic done in int (gives the target's offset-first addu).
            sFlagItem_E348* e = (sFlagItem_E348*)(i * 0xD0 + (int)self->items);
            e->vt[2].fn((char*)e + e->vt[2].delta, self->time);
        }
    }
    self->dirty = 1;
}
#endif

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

