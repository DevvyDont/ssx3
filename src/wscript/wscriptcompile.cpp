#include "common.h"

INCLUDE_ASM("wscript/wscriptcompile", cWScriptCompile_parseKeywords);

INCLUDE_ASM("wscript/wscriptcompile", func_00351120);

//100%
INCLUDE_ASM("wscript/wscriptcompile", func_00351170);
#ifdef SKIP_ASM
struct sWSNode {
    sWSNode* next;
    int pad4;
    unsigned int flags;
    char pad0C[0x78 - 0xC];
    unsigned int type : 8;
    unsigned int index : 24;
    char pad7C[0xB0 - 0x7C];
};

struct sWSPool {
    unsigned char type;
    char pad1[7];
    sWSNode* free;
    sWSNode* base;
};

extern "C" sWSNode* func_00351170(sWSPool* self)
{
    sWSNode* p = self->free;
    if (p != 0) {
        self->free = p->next;
    }
    p->type = self->type;
    p->index = p - self->base;
    p->flags |= 0x2000;
    return p;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", func_003511D0);
#ifdef SKIP_ASM
extern "C" sWSNode* func_003511D0(sWSPool* self, unsigned int id)
{
    sWSNode** link = &self->free;
    sWSNode* p = self->free;
    while (p != 0) {
        if (p - self->base == (id >> 8)) {
            *link = p->next;
            p->type = self->type;
            int idx = p - self->base;
            p->flags |= 0x2000;
            p->index = idx;
            return p;
        }
        link = &p->next;
        p = p->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("wscript/wscriptcompile", func_00351260__FPvT0);
#ifdef SKIP_ASM
int func_00351260(void* self, void* a1)
{
    int t0 = *(int*)((char*)self + 0x8);
    *(int*)a1 = t0;
    *(int*)((char*)self + 0x8) = (int)a1;
    return t0;
}
#endif

