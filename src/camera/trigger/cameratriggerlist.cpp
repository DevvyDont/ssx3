#include "common.h"

struct cCameraTriggerList {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
    int field_0x10;
};

void cCameraTriggerList_initHeader(cCameraTriggerList* self);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_init__FP18cCameraTriggerList);
#ifdef SKIP_ASM
void cCameraTriggerList_init(cCameraTriggerList* self)
{
    self->field_0x0 = 0;
    cCameraTriggerList_initHeader(self);
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerlist", func_0016BF40);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/trigger/cameratriggerlist", func_0016C098);
#ifdef SKIP_ASM
extern "C" void func_0016BF40(void*);
void cMemMan_free(void*);

extern "C" void func_0016C098(void** self)
{
    func_0016BF40(self);
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
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_insert__FP19cCameraTriggerList2Pvi);
#ifdef SKIP_ASM
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
extern "C" int func_00317F98(const char* name);
extern "C" void FILE_loadpackat(const char* name, void* buf, int size);
extern "C" int cCameraTriggerList_loadFromBuffer(void* self, void* buf);
// PORT: operator_new__FUi takes (size, tag, flags, align) here.
void* operator_new_K2(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_0045BF30[];

extern "C" int cCameraTriggerList_loadFromFile(void* self, const char* name)
{
    if (!BXFILE_exists(name))
        return 1;
    func_0016BF40(self);
    int size = func_00317F98(name);
    void* buf = operator_new_K2(size, D_0045BF30, 0x100, 0);
    FILE_loadpackat(name, buf, size);
    if (buf != 0) {
        int r = cCameraTriggerList_loadFromBuffer(self, buf);
        cMemMan_free(buf);
        return r;
    }
    return 2;
}
#endif

INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_loadFromBuffer);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_initHeader__FP18cCameraTriggerList);
#ifdef SKIP_ASM
void cCameraTriggerList_initHeader(cCameraTriggerList* self)
{
    self->field_0x4 = 0;
    self->field_0xC = 7;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
}
#endif

void get_uint(void* buffer, void* dest);

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_readCookie__FP18cCameraTriggerListPv);
#ifdef SKIP_ASM
void cCameraTriggerList_readCookie(cCameraTriggerList* self, void* buffer)
{
    get_uint(buffer, (char*)self + 0xC);
}
#endif

//100%
INCLUDE_ASM("camera/trigger/cameratriggerlist", cCameraTriggerList_readHeader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares get_uint with C++ linkage; the target calls the unmangled symbol.
int get_uint_c(void* buffer, void* dest) __asm__("get_uint");

extern "C" int cCameraTriggerList_readHeader(cCameraTriggerList* self, void* buffer)
{
    get_uint_c(buffer, &self->field_0x10);
    get_uint_c(buffer, &self->field_0x4);
    get_uint_c(buffer, &self->field_0x8);
    return 0;
}
#endif

