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
/**
 * Frees every node of the active trigger list and leaves the list empty.
 */
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
INCLUDE_ASM("camera/trigger/triggeralgorithms", cActiveTriggerList_clearFlags);
#ifdef SKIP_ASM
/**
 * Clears the two flag words (+0x8 and +0xC) of every node in the active trigger list.
 */
extern "C" void cActiveTriggerList_clearFlags(cActiveTriggerList* self)
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
/**
 * Finds the node of an active camera-trigger list that holds a given trigger.
 *
 * Walks the list from the head (+0x0) and returns the first node whose trigger (node +0x0) equals @p key.
 *
 * @param key  the camera trigger to look for (On Tour tCameraTrigger*)
 * @return the node, or 0 when the trigger is not listed
 */
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
/**
 * Finds the trigger with a given id in an active camera-trigger list.
 *
 * Walks the list from the head and returns the first node's trigger whose first word (TriggerID) equals @p key.
 *
 * @param key  the trigger id to look for (On Tour triggerID)
 * @return the trigger, or 0 when none is listed with that id
 */
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
extern char str_Active_Camera_Trigger[];

/**
 * Adds a camera trigger to the front of the active trigger list.
 *
 * Allocates a 16-byte "Active Camera Trigger" node, stores @p item in its first word (the struct calls it
 * pad) and sets both node flags (+0x8, +0xC; cActiveTriggerList_clearFlags clears them on every node) to 1.
 *
 * @param item  the trigger to add (On Tour tCameraTrigger*)
 */
extern "C" void cActiveTriggerList_add(cActiveTriggerList* self, void* item)
{
    sTriggerNode* n = (sTriggerNode*)cMemMan_alloc(0x10, str_Active_Camera_Trigger, 0x20000000, 0);
    sTriggerNode* old = self->head;
    self->head = n;
    n->pad = item;
    n->next = old;
    *(int*)((char*)n + 0xC) = 1;
    *(int*)((char*)n + 0x8) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", cActiveTriggerList_remove);
#ifdef SKIP_ASM
/**
 * Unlinks a node from the active trigger list and frees it.
 *
 * @param target  the node to remove
 * @return the node that followed it (the new head when it was the head), or 0 when it was last or not in the list
 */
extern "C" sTriggerNode* cActiveTriggerList_remove(cActiveTriggerList* self, sTriggerNode* target)
{
    sTriggerNode* head = self->head;
    if (target == head) {
        sTriggerNode* next = target->next;
        operator_delete((int*)target);
        self->head = next;
        return next;
    }
    sTriggerNode* prev = head;
    for (sTriggerNode* node = prev->next; node != 0; node = node->next) {
        if (node == target) {
            prev->next = node->next;
            operator_delete((int*)node);
            return prev->next;
        }
        prev = node;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", func_0016C800);
#ifdef SKIP_ASM
void* func_0016BEE8(void* self);
void* func_0016C5D0(void* self);
extern "C" void* cCameraTriggerStack_cCameraTriggerStack(void* self);

/**
 * Constructs the camera trigger manager: its trigger list, two active trigger lists and two trigger stacks.
 *
 * Runs func_0016BEE8 on the trigger list (+0x0), func_0016C5D0 on the active lists at +0x14 and +0x18 and
 * cCameraTriggerStack_cCameraTriggerStack on the stacks at +0x1C and +0x3C. The unit's static initializer func_00175A20
 * calls it on gCamTriggerMan.
 *
 * @return @p self
 */
extern "C" void* func_0016C800(void* self)
{
    char* cursor;
    int i;
    func_0016BEE8(self);
    cursor = (char*)self + 0x14;
    for (i = 1; i != -1; i--, cursor += 4) {
        func_0016C5D0(cursor);
    }
    cursor = (char*)self + 0x1C;
    for (i = 1; i != -1; i--, cursor += 0x20) {
        cCameraTriggerStack_cCameraTriggerStack(cursor);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", cCameraTriggerMan__cCameraTriggerMan);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_purge(void* self);
extern "C" void cActiveTriggerList__cActiveTriggerList(void* self, int flags);
extern "C" void func_0016BEF8(void* self, int flags);

/**
 * Destroys the camera trigger manager: purges it and destroys its member lists.
 *
 * Calls cCameraTriggerMan_purge, runs cActiveTriggerList__cActiveTriggerList on the two active lists (+0x14,
 * +0x18) from last to first, destroys the embedded trigger list with func_0016BEF8(self, 2) and frees the object
 * when @p flags bit 0 is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCameraTriggerMan__cCameraTriggerMan(void* self, int flags)
{
    cCameraTriggerMan_purge(self);
    if ((char*)self + 0x14 != 0) {
        char* activeList = (char*)self + 0x14 + 8;
        while ((char*)self + 0x14 != activeList) {
            activeList -= 4;
            cActiveTriggerList__cActiveTriggerList(activeList, 0);
        }
    }
    func_0016BEF8(self, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/triggeralgorithms", cCameraTriggerMan_init);
#ifdef SKIP_ASM
struct cCameraTriggerList;
struct cCameraTriggerStack;
void cCameraTriggerList_init(cCameraTriggerList* self);
void cCameraTriggerStack_init(cCameraTriggerStack* self);
extern "C" void cCamera_resetChaseControllerSwitches(void);

/**
 * Initialises the camera trigger manager for a new track.
 *
 * Initialises the trigger list (cCameraTriggerList_init), purges both active trigger lists (+0x14, +0x18), resets
 * both trigger stacks (+0x1C, +0x3C) with cCameraTriggerStack_init and calls cCamera_resetChaseControllerSwitches.
 */
extern "C" void cCameraTriggerMan_init(void* self)
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
INCLUDE_ASM("camera/trigger/triggeralgorithms", cCameraTriggerMan_initOnRestart);
#ifdef SKIP_ASM
struct cCameraTriggerStack;
void cCameraTriggerStack_init(cCameraTriggerStack* self);
extern "C" void cCamera_resetChaseControllerSwitches(void);

/**
 * Resets the camera trigger state for a restart, keeping the loaded triggers.
 *
 * Purges both active trigger lists (+0x14, +0x18), resets both trigger stacks (+0x1C, +0x3C) and calls
 * cCamera_resetChaseControllerSwitches.
 */
extern "C" void cCameraTriggerMan_initOnRestart(void* self)
{
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x14));
    cActiveTriggerList_purge((cActiveTriggerList*)((char*)self + 0x18));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x1C));
    cCameraTriggerStack_init((cCameraTriggerStack*)((char*)self + 0x3C));
    cCamera_resetChaseControllerSwitches();
}
#endif

