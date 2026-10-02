#include "common.h"

INCLUDE_ASM("main/sectionman", cSectionMan_setSky);

//100%
INCLUDE_ASM("main/sectionman", func_0022E228);
#ifdef SKIP_ASM
struct sSectionNode {
    int field_0x0;
    sSectionNode* next; // 0x4
};

struct sSectionMan {
    sSectionNode nodes[50]; // 0x0
    int field_0x190;
    int field_0x194;
    sSectionNode* freeList; // 0x198
    int field_0x19c;
    int field_0x1a0;
};

extern "C" void func_0022E228(sSectionMan* self)
{
    int i;
    self->freeList = self->nodes;
    for (i = 0; i < 50; i++) {
        self->nodes[i].field_0x0 = 49;
        self->nodes[i].next = &self->nodes[i + 1];
    }
    self->field_0x190 = 49;
    self->field_0x194 = 0;
    self->field_0x19c = 0;
    self->field_0x1a0 = 0;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", func_0022E278__FPv);
#ifdef SKIP_ASM
struct sSectionLink {
    int pad0;
    sSectionLink* next; // 0x4
};

void* func_0022E278(void* self)
{
    sSectionLink* node = *(sSectionLink**)((char*)self + 0x198);
    *(sSectionLink**)((char*)self + 0x198) = node->next;
    return node;
}
#endif

//100%
INCLUDE_ASM("main/sectionman", func_0022E288__FPvT0);
#ifdef SKIP_ASM
int func_0022E288(void* self, void* a1)
{
    int t0 = *(int*)((char*)self + 0x198);
    *(int*)((char*)a1 + 0x4) = t0;
    *(int*)((char*)self + 0x198) = (int)a1;
    return t0;
}
#endif

INCLUDE_ASM("main/sectionman", func_0022E298);

//100%
INCLUDE_ASM("main/sectionman", func_0022E348);
#ifdef SKIP_ASM
extern "C" int func_0022E348(void* self, void* a1)
{
    if (a1 != 0) {
        return *(int*)((char*)a1 + 0x4);
    }
    return *(int*)((char*)self + 0x19c);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/sectionman", func_0022E360);
#ifdef SKIP_ASM
// PORT: the unit defines func_0022E288 as returning int; this caller treats it as void.
void func_0022E288_v(void* self, void* node) __asm__("func_0022E288__FPvT0");

extern "C" void func_0022E360(void* self, void* prev, void* node)
{
    if (prev == 0) {
        *(int*)((char*)self + 0x19C) = *(int*)((char*)node + 0x4);
    } else {
        *(int*)((char*)prev + 0x4) = *(int*)((char*)node + 0x4);
    }
    func_0022E288_v(self, node);
    *(int*)((char*)self + 0x1A0) = *(int*)((char*)self + 0x1A0) - 1;
}
#endif

