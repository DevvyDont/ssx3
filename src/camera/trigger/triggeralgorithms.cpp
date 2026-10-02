#include "common.h"

void operator_delete(int* ptr);

struct sTriggerNode {
    void* pad;
    sTriggerNode* next;
};

struct cActiveTriggerList {
    sTriggerNode* head;
};

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", cActiveTriggerList_purge__FP18cActiveTriggerList);
#ifdef SKIP_ASM
void cActiveTriggerList_purge(cActiveTriggerList* self)
{
    sTriggerNode* node = self->head;
    if (node != 0) {
        do {
            sTriggerNode* next = node->next;
            operator_delete((int*)node);
            node = next;
        } while (node != 0);
    }
    self->head = 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C678);
#ifdef SKIP_ASM
extern "C" void func_0016C678(cActiveTriggerList* self)
{
    sTriggerNode* node = self->head;
    if (node != 0) {
        do {
            *(int*)((char*)node + 0xC) = 0;
            *(int*)((char*)node + 0x8) = 0;
            node = node->next;
        } while (node != 0);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C6B0);
#ifdef SKIP_ASM
extern "C" sTriggerNode* func_0016C6B0(cActiveTriggerList* self, void* key)
{
    sTriggerNode* node = self->head;
    if (node != 0) {
        do {
            if (node->pad == key) {
                return node;
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C6E0);
#ifdef SKIP_ASM
extern "C" void* func_0016C6E0(cActiveTriggerList* self, void* key)
{
    sTriggerNode* node = self->head;
    if (node != 0) {
        do {
            void* item = node->pad;
            if (*(void**)item == key) {
                return item;
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", cActiveTriggerList_add);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char D_0045BF68[];

extern "C" void cActiveTriggerList_add(cActiveTriggerList* self, void* item)
{
    sTriggerNode* n = (sTriggerNode*)cMemMan_alloc(0x10, D_0045BF68, 0x20000000, 0);
    sTriggerNode* old = self->head;
    self->head = n;
    n->pad = item;
    n->next = old;
    *(int*)((char*)n + 0xC) = 1;
    *(int*)((char*)n + 0x8) = 1;
}
#endif

INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C778);

INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C800);

INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C888);

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C918);
#ifdef SKIP_ASM
struct cCameraTriggerList;
struct cCameraTriggerStack;
void cCameraTriggerList_init(cCameraTriggerList* self);
void cCameraTriggerStack_init(cCameraTriggerStack* self);
extern "C" void cCamera_resetChaseControllerSwitches(void);

extern "C" void func_0016C918(void* self)
{
    cCameraTriggerList_init((cCameraTriggerList*)self);
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x14));
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x18));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x3C));
    cCamera_resetChaseControllerSwitches();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C968);
#ifdef SKIP_ASM
struct cCameraTriggerStack;
void cCameraTriggerStack_init(cCameraTriggerStack* self);
extern "C" void cCamera_resetChaseControllerSwitches(void);

extern "C" void func_0016C968(void* self)
{
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x14));
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x18));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x3C));
    cCamera_resetChaseControllerSwitches();
}
#endif

