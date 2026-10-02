#include "common.h"

struct cListNode {
    cListNode* next; // 0x0
    cListNode* prev; // 0x4
};

struct cList {
    char pad_0x00[0x4];
    cListNode* head; // 0x4
    char pad_0x08[0x4];
    cListNode* tail; // 0xC
};

//100%
INCLUDE_ASM("util/list", cListNode_isSentinel__FP9cListNode);
#ifdef SKIP_ASM
int cListNode_isSentinel(cListNode* node)
{
    return (node->prev == node) || (node->next == node);
}
#endif

//100%
INCLUDE_ASM("util/list", cListNode_removeFromList);
#ifdef SKIP_ASM
extern "C" void cListNode_removeFromList(cListNode* self)
{
    cListNode* next = self->next;
    cListNode* prev = self->prev;
    if (next != 0 && next != self) {
        if (prev == self) {
            next->prev = next;
        } else {
            next->prev = prev;
        }
    }
    if (prev != 0 && prev != self) {
        if (next == self) {
            prev->next = prev;
        } else {
            prev->next = next;
        }
    }
    self->prev = self;
    self->next = self;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397788);
#ifdef SKIP_ASM
class func_00397788_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_00397788(void* a, func_00397788_cVirt* obj)
{
    return obj->v02(a);
}
#endif

//100%
INCLUDE_ASM("util/list", func_003977B8);
#ifdef SKIP_ASM
class func_003977B8_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

extern "C" int func_003977B8(void* a, func_003977B8_cVirt* obj)
{
    return obj->v02(a);
}
#endif

INCLUDE_ASM("util/list", func_003977E8);

//100%
INCLUDE_ASM("util/list", cList_first__FP5cList);
#ifdef SKIP_ASM
cListNode* cList_first(cList* list)
{
    cListNode* head = list->head;
    return (head->prev == head) ? 0 : head;
}
#endif

INCLUDE_ASM("util/list", func_00397870);

//100%
INCLUDE_ASM("util/list", cList_addToFront__FP5cListP9cListNode);
#ifdef SKIP_ASM
void cList_addToFront(cList* list, cListNode* newNode)
{
    cListNode* head = list->head;
    newNode->prev = head;
    newNode->next = head->next;
    head->next = newNode;
    list->head = newNode;
}
#endif

//100%
INCLUDE_ASM("util/list", cList_addToEnd__FP5cListP9cListNode);
#ifdef SKIP_ASM
void cList_addToEnd(cList* list, cListNode* newNode)
{
    cListNode* tail = list->tail;
    newNode->next = tail;
    newNode->prev = tail->prev;
    tail->prev = newNode;
    list->tail = newNode;
}
#endif

//100%
INCLUDE_ASM("util/list", func_00397930__FPv);
#ifdef SKIP_ASM
void func_00397930(void* self)
{
    void* t0 = (char*)self + 0xc;
    *(void**)((char*)self + 0x4) = t0;
    *(void**)((char*)self + 0xc) = self;
    *(void**)((char*)self + 0x10) = t0;
    *(void**)self = self;
}
#endif

INCLUDE_ASM("util/list", func_00397948);

//100%
INCLUDE_ASM("util/list", func_003979C0);
#ifdef SKIP_ASM
extern "C" int func_003979C0(cList* list)
{
    cListNode* node = list->head;
    int count = 0;
    while (node->prev != node) {
        node = node->prev;
        count++;
    }
    return count;
}
#endif

INCLUDE_ASM("util/list", func_003979F8);

INCLUDE_ASM("util/list", func_00397A68);

