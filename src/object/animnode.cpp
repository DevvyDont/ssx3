#include "common.h"

INCLUDE_ASM("object/animnode", cAnimNode_setAnimMeshCache);

INCLUDE_ASM("object/animnode", func_0034E600);

INCLUDE_ASM("object/animnode", func_0034E698);

INCLUDE_ASM("object/animnode", func_0034E798);

//100%
INCLUDE_ASM("object/animnode", func_0034EBA0);
#ifdef SKIP_ASM
struct sAnimLinkList_EBA0;

struct sAnimLink_EBA0 {
    char pad_0x00[0x18];
    sAnimLinkList_EBA0* list; // 0x18
};

struct sAnimLinkList_EBA0 {
    char pad_0x00[0xC];
    sAnimLink_EBA0* head; // 0x0C
};

extern "C" void func_002D1AC8(sAnimLinkList_EBA0* list);

struct sAnimNode_EBA0 {
    char pad_0x00[0x14];
    sAnimLink_EBA0 link; // 0x14
};

// is this node's link at the head of its list, after func_002D1AC8?
extern "C" int func_0034EBA0(sAnimNode_EBA0* self)
{
    sAnimLink_EBA0* link = &self->link;
    func_002D1AC8(self->link.list);
    return link->list->head == link;
}
#endif

INCLUDE_ASM("object/animnode", func_0034EBE0);

INCLUDE_ASM("object/animnode", func_0034EC58);

INCLUDE_ASM("object/animnode", func_0034ED88);

INCLUDE_ASM("object/animnode", func_0034EE68);

