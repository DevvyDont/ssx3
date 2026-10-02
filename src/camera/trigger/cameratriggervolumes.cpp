#include "common.h"

void operator_delete(int* ptr);

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", tCameraTrigger__tCameraTrigger__FPvi);
#ifdef SKIP_ASM
void tCameraTrigger__tCameraTrigger(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", func_0016C5D0__FPv);
#ifdef SKIP_ASM
void* func_0016C5D0(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", func_0016C5E0);
#ifdef SKIP_ASM
struct cActiveTriggerList;
void cActiveTriggerList_purge(cActiveTriggerList* self);

extern "C" void func_0016C5E0(cActiveTriggerList* self, int flags)
{
    cActiveTriggerList_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

