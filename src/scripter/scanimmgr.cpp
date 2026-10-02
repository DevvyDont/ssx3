#include "common.h"

//100%
INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_cScriptAnimBankManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004825D8[];
extern const char D_00481A80[];

struct sScAnimBank53B8 {
    int id;         // 0x0
    int index;      // 0x4
    int a;          // 0x8
    int b;          // 0xC
    int c;          // 0x10
};

struct cScriptAnimBankManager53B8 {
    int flags;                  // 0x0
    int count;                  // 0x4
    sScAnimBank53B8* banks;     // 0x8
    void* vtable;               // 0xC
};

extern "C" cScriptAnimBankManager53B8* cScriptAnimBankManager_cScriptAnimBankManager(
    cScriptAnimBankManager53B8* self, int flags, int count, int base)
{
    //S
    self->vtable = D_004825D8;
    self->flags = flags;
    self->count = count;
    //E
    self->banks = (sScAnimBank53B8*)operator_new_tag(count * sizeof(sScAnimBank53B8), D_00481A80, flags, 0);
    int idx = base;
    for (int i = 0; i < self->count; i++) {
        self->banks[i].id = -1;
        self->banks[i].index = idx++;
        self->banks[i].a = 0;
        self->banks[i].b = 0;
        self->banks[i].c = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", func_00275498);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void func_00275698(void* self);
extern char D_004825D8[];

extern "C" void func_00275498(void* self, int flags)
{
    *(void**)((char*)self + 0xC) = D_004825D8;
    func_00275698(self);
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_LinkBank);

//100%
INCLUDE_ASM("scripter/scanimmgr", func_00275650);
#ifdef SKIP_ASM
struct sScAnimVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sAnimList;
extern "C" void* func_00275718(sAnimList* self, int id);

extern "C" void func_00275650(sAnimList* self, int id)
{
    void* p = func_00275718(self, id);
    if (p != 0) {
        sScAnimVEntry* vt = *(sScAnimVEntry**)((char*)self + 0xC);
        vt[2].fn((char*)self + vt[2].delta, p);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", func_00275698);
#ifdef SKIP_ASM
struct sDmEntry275698 {
    char pad_0x00[0x8];
    int used;
    char pad_0x0C[0x8];
};

struct sDmVEntry275698 {
    short delta;
    short index;
    void (*fn)(void*, sDmEntry275698*);
};

struct sDmTable275698 {
    int unk_0x0;
    int count;
    sDmEntry275698* entries;
    sDmVEntry275698* vt;
};

extern "C" void func_00275698(void* self_)
{
    sDmTable275698* self = (sDmTable275698*)self_;
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i].used != 0) {
            self->vt[2].fn((char*)self + self->vt[2].delta, &self->entries[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scanimmgr", func_00275718);
#ifdef SKIP_ASM
struct sAnimEntry {
    int id;
    int unk4;
    void* data;
    int unkC;
    int unk10;
};

struct sAnimList {
    int unk0;
    int count;
    sAnimEntry* entries;
};

extern "C" void* func_00275718(sAnimList* self, int id)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data != 0 && self->entries[i].id == id) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("scripter/scanimmgr", func_00275760);

//100%
INCLUDE_ASM("scripter/scanimmgr", func_002757F0);
#ifdef SKIP_ASM
extern "C" sAnimEntry* func_002757F0(sAnimList* self)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data == 0) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

