#include "common.h"

INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_cScriptAnimBankManager);

INCLUDE_ASM("scripter/scanimmgr", func_00275498);

INCLUDE_ASM("scripter/scanimmgr", cScriptAnimBankManager_LinkBank);

INCLUDE_ASM("scripter/scanimmgr", func_00275650);

INCLUDE_ASM("scripter/scanimmgr", func_00275698);

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

INCLUDE_ASM("scripter/scanimmgr", func_002757F0);

