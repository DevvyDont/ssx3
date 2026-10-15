#include "common.h"

void operator_delete(int* ptr);

//100%
void tCameraTrigger__tCameraTrigger(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}

//100%
void* func_0016C5D0(void* self)
{
    *(int*)self = 0;
    return self;
}

//100%
struct cActiveTriggerList;
void cActiveTriggerList_purge(cActiveTriggerList* self);

extern "C" void func_0016C5E0(cActiveTriggerList* self, int flags)
{
    cActiveTriggerList_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}

