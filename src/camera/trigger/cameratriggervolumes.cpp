#include "common.h"

void operator_delete(int* ptr);

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", tCameraTrigger__tCameraTrigger__FPvi);
#ifdef SKIP_ASM
/**
 * Destroys a camera trigger; the only work is freeing the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void tCameraTrigger__tCameraTrigger(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", cActiveTriggerList_cActiveTriggerList__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty active camera-trigger list: clears its head pointer.
 *
 * cCameraTriggerMan_cCameraTriggerMan runs it on both of the camera trigger manager's active lists (+0x14, +0x18).
 *
 * @return @p self
 */
void* cActiveTriggerList_cActiveTriggerList(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggervolumes", cActiveTriggerList__cActiveTriggerList);
#ifdef SKIP_ASM
struct cActiveTriggerList;
void cActiveTriggerList_purge(cActiveTriggerList* self);

/**
 * Destroys an active camera-trigger list: empties it with cActiveTriggerList_purge() and, if asked, frees it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cActiveTriggerList__cActiveTriggerList(cActiveTriggerList* self, int flags)
{
    cActiveTriggerList_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

