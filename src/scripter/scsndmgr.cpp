#include "common.h"

INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_cScriptSoundBankManager);

INCLUDE_ASM("scripter/scsndmgr", func_00283518);

INCLUDE_ASM("scripter/scsndmgr", func_00283580);

INCLUDE_ASM("scripter/scsndmgr", func_00283610);

INCLUDE_ASM("scripter/scsndmgr", func_00283658);

//100%
INCLUDE_ASM("scripter/scsndmgr", func_002836D8);
#ifdef SKIP_ASM
struct sSndEntry {
    int id;
    int unk4;
    void* data;
};

struct sSndList {
    int unk0;
    int count;
    sSndEntry* entries;
};

extern "C" sSndEntry* func_002836D8(sSndList* self, int id)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data != 0 && self->entries[i].id == id) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("scripter/scsndmgr", func_00283720);

//100%
INCLUDE_ASM("scripter/scsndmgr", func_00283760);
#ifdef SKIP_ASM
extern "C" sSndEntry* func_00283760(sSndList* self)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data == 0) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", func_00283798);
#ifdef SKIP_ASM
extern char D_00482228[];

extern "C" void* func_00283798(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(void**)((char*)self + 0x18) = D_00482228;
    return self;
}
#endif

INCLUDE_ASM("scripter/scsndmgr", func_002837C8);

INCLUDE_ASM("scripter/scsndmgr", func_00283818);

