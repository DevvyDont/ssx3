#include "common.h"
#include "vtable.h"

#include "camera/trigger/cameratriggerlist.h"

void cCameraTriggerList_initHeader(cCameraTriggerList* self);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_init__FP18cCameraTriggerList);
#ifdef SKIP_ASM
/**
 * Initialises an empty camera trigger list.
 *
 * Clears the trigger array pointer, then resets the header fields with cCameraTriggerList_initHeader().
 */
void cCameraTriggerList::init()
{
    Triggers = 0;
    initHeader();
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_reset);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void tCameraTrigger__tCameraTrigger(void* self, int flags);

struct sVEntry_16BF40 { short delta; short index; void (*fn)(void*); };

struct sTrigPart_16BF40
{
    int f0;
    sVEntry_16BF40* vt;     // 0x4
};



/**
 * Frees every camera trigger and its parts, then re-initialises the header.
 *
 * For each trigger: frees the buffer at +0x8, destroys and deletes the parts at +0xC and +0x10 (virtual slot
 * 1), then destroys and deletes the trigger itself.
 *
 */
void cCameraTriggerList::reset()
{
    cCameraTriggerList* self = this;
    for (int i = 0; i < self->NumTriggers; i++)
    {
        if (((int*)((tCameraTrigger**)self->Triggers)[i]->Volume))
        {
            operator_delete(((int*)((tCameraTrigger**)self->Triggers)[i]->Volume));
            (*(int**)&((tCameraTrigger**)self->Triggers)[i]->Volume) = 0;
        }
        if (((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->EnterAction))
        {
            sTrigPart_16BF40* part = ((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->EnterAction);
            part->vt[1].fn(vthis(part, part->vt[1]));
            operator_delete((int*)((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->EnterAction));
            (*(sTrigPart_16BF40**)&((tCameraTrigger**)self->Triggers)[i]->EnterAction) = 0;
        }
        if (((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->ExitAction))
        {
            sTrigPart_16BF40* part = ((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->ExitAction);
            part->vt[1].fn(vthis(part, part->vt[1]));
            operator_delete((int*)((sTrigPart_16BF40*)((tCameraTrigger**)self->Triggers)[i]->ExitAction));
            (*(sTrigPart_16BF40**)&((tCameraTrigger**)self->Triggers)[i]->ExitAction) = 0;
        }
        if (((tCameraTrigger**)self->Triggers)[i])
            tCameraTrigger__tCameraTrigger(((tCameraTrigger**)self->Triggers)[i], 3);
        ((tCameraTrigger**)self->Triggers)[i] = 0;
    }
    self->initHeader();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_purge);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerList_reset(cCameraTriggerList*);
void cMemMan_free(void*);

/**
 * Empties a camera trigger list and releases its trigger array.
 *
 * Calls cCameraTriggerList_reset, then frees the trigger pointer array (+0x0) with cMemMan_free when it is set and
 * clears the pointer.
 */
extern "C" void cCameraTriggerList_purge(void** self)
{
    ((cCameraTriggerList*)self)->reset();
    if (*self != 0) {
        cMemMan_free(*self);
        *self = 0;
    }
}
#endif

struct cCameraTriggerList2 {
    void** arr; // 0x0
    int count; // 0x4
};

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_findTrigger);
#ifdef SKIP_ASM
/**
 * Finds the camera trigger with a given ID.
 *
 * Walks the list's count trigger pointers (arr) and returns the first trigger whose first word (On Tour
 * tCameraTrigger::TriggerID) equals @p key. Nothing in the retail code calls it.
 *
 * @param key  the trigger ID to look for
 * @return the matching trigger, or 0
 */
int* cCameraTriggerList::findTrigger(int key)
{
    for (int i = 0; i < NumTriggers; i++) {
        int* entry = (int*)(void**)Triggers[i];
        if (*entry == key)
            return entry;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_insert__FP19cCameraTriggerList2Pvi);
#ifdef SKIP_ASM
/**
 * Stores a trigger in the list, either appended or written into a given slot.
 *
 * A negative @p idx appends the item when fewer than 120 are stored. A non-negative @p idx overwrites that slot
 * with no bounds check and does not change the count.
 *
 * @param list  list to modify
 * @param item  trigger to store
 * @param idx   slot to overwrite, or negative to append
 * @return 0 if an append found the list full, otherwise 1
 */
int cCameraTriggerList_insert(cCameraTriggerList2* list, void* item, int idx)
{
    if (idx < 0) {
        if (list->count >= 0x78) {
            return 0;
        }
        list->arr[list->count] = item;
        list->count++;
    } else {
        list->arr[idx] = item;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_loadFromFile);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" int BXFILE_size(const char* name);
extern "C" void FILE_loadpackat(const char* name, void* buf, int size);
extern "C" int cCameraTriggerList_loadFromBuffer(cCameraTriggerList* self, void* buf);
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_Read_Buffer[];

/**
 * Replaces the list's camera triggers with those loaded from a file.
 *
 * Returns 1 without changing anything if the file does not exist. Otherwise it clears the list with
 * cCameraTriggerList_reset(), reads the file into a "Read Buffer" (FILE_loadpackat unpacks it if packed),
 * parses it with cCameraTriggerList_loadFromBuffer() and frees the buffer. cCameraTriggerMan_loadTriggers()
 * passes a nonzero result on as an error.
 *
 * @param name  file to load
 * @return 0 on success, 1 if the file is missing, 2 if the buffer could not be allocated, otherwise the
 *         error from cCameraTriggerList_loadFromBuffer()
 */
int cCameraTriggerList::loadFromFile(const char* name)
{
    if (!BXFILE_exists(name))
        return 1;
    reset();
    int size = BXFILE_size(name);
    void* buf = operator_new_K2(size, str_Read_Buffer, 0x100, 0);
    FILE_loadpackat(name, buf, size);
    if (buf != 0) {
        int status = loadFromBuffer(buf);
        cMemMan_free(buf);
        return status;
    }
    return 2;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_loadFromBuffer);
#ifdef SKIP_ASM
void cCameraTriggerList_readCookie(cCameraTriggerList* self, void* buffer);
// readHeader is called with the expected version as a third argument here.
extern "C" int cCameraTriggerList_readHeader_K3(cCameraTriggerList* self, void* buffer, int version) __asm__("cCameraTriggerList_readHeader");
extern "C" int readTrigger(void* buffer, void* trigger, int version);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, int flags, int d) __asm__("cMemMan_alloc");
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K3(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_Camera_Trigger_Array[];
extern char str_Camera_Trigger[];

struct tCameraTrigger_K3 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int pad[2];
    int f1C;
    int f20;
    tCameraTrigger_K3() { f1C = 4; f8 = 0; fC = 0; f10 = 0; f0 = 0; f4 = 3; }
};

/**
 * Parses a camera trigger file image into the list's trigger array.
 *
 * Reads the version cookie, which must be 7, and the header (including the trigger count). It then allocates
 * the "Camera Trigger Array" and reads each trigger with readTrigger(). A failed trigger is destroyed and its
 * error returned, leaving the triggers before it in place.
 *
 * @param buf    start of the file image
 * @return 0 on success, 3 for a wrong version, otherwise readTrigger's error
 */
int cCameraTriggerList::loadFromBuffer(void* buf)
{
    cCameraTriggerList* self = (cCameraTriggerList*)this;
    void* cur = buf;
    self->initHeader();
    self->readCookie(&cur);
    if (self->Cookie != 7) {
        self->Cookie = 7;
        return 3;
    }
    int status = cCameraTriggerList_readHeader_K3(self, &cur, 7);
    if (status != 0)
        return status;
    self->Triggers = (tCameraTrigger**)operator_new_K3(self->NumTriggers * 4, str_Camera_Trigger_Array, 0, 0);
    for (int i = 0; i < self->NumTriggers; i++) {
        tCameraTrigger_K3* newTrig = new (str_Camera_Trigger, 0, 0) tCameraTrigger_K3;
        status = readTrigger(&cur, &newTrig, self->Cookie);
        if (status != 0) {
            if (newTrig != 0)
                tCameraTrigger__tCameraTrigger(newTrig, 3);
            return status;
        }
        cCameraTriggerList_insert((cCameraTriggerList2*)self, newTrig, i);
    }
    self->Cookie = 7;
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_headerSize);
#ifdef SKIP_ASM
/**
 * Returns 16, the size in bytes of a camera trigger list's header.
 *
 * The header words are the count (+0x4), the next id (+0x8), the version/cookie (+0xC) and the time (+0x10), the fields
 * cCameraTriggerList_initHeader resets. Nothing calls it.
 *
 * @return 16
 */
extern "C" int cCameraTriggerList_headerSize(void) {
    return 0x10;
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_initHeader__FP18cCameraTriggerList);
#ifdef SKIP_ASM
/**
 * Resets a camera trigger list's header fields.
 *
 * Sets the trigger count and the words at +0x08 and +0x10 to 0, and the version to 7. The array pointer at
 * +0x00 is left alone.
 */
void cCameraTriggerList::initHeader()
{
    NumTriggers = 0;
    Cookie = 7;
    NextID = 0;
    Time = 0;
}
#endif

extern "C" int get_uint(void* buffer, void* dest);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_readCookie__FP18cCameraTriggerListPv);
#ifdef SKIP_ASM
/**
 * Reads a trigger file's version cookie into the list.
 *
 * Stores the next uint from the stream in Cookie (the format version); cCameraTriggerList_loadFromBuffer()
 * requires 7.
 *
 * @param buffer  pointer to the stream's read cursor
 */
void cCameraTriggerList::readCookie(void* buffer)
{
    get_uint(buffer, &Cookie);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_readHeader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares get_uint with C++ linkage; the target calls the unmangled symbol.
int get_uint_c(void* buffer, void* dest) __asm__("get_uint");

/**
 * Reads a trigger file's header fields into the list.
 *
 * Reads three uints, in order, into +0x10, the trigger count (+0x04) and +0x08.
 *
 * @param buffer  pointer to the stream's read cursor
 * @return always 0
 */
int cCameraTriggerList::readHeader(void* buffer)
{
    get_uint_c(buffer, &Time);
    get_uint_c(buffer, &NumTriggers);
    get_uint_c(buffer, &NextID);
    return 0;
}
#endif

