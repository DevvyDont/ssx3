#include "common.h"

struct cBigFile {
    int field_0x0;
    int field_0x4;
};

//100%
INCLUDE_ASM("bx/bigfile", cBigFile_cBigFile__FP8cBigFile);
#ifdef SKIP_ASM
/**
 * Constructs a closed big file: no handle (-1) and the open flag cleared.
 *
 * @return @p self
 */
cBigFile* cBigFile_cBigFile(cBigFile* self)
{
    self->field_0x4 = 0;
    self->field_0x0 = -1;
    return self;
}
#endif

extern "C" void cBigFile_open(cBigFile* self);

//100%
INCLUDE_ASM("bx/bigfile", cBigFile_cBigFile1__FP8cBigFile);
#ifdef SKIP_ASM
/**
 * Constructs a big file and opens it at once.
 *
 * Sets the handle to -1 and the open flag to 0, then calls cBigFile_open(). Only @p self is declared here: the
 * path and memory class the callers pass (for example the load screens' 0x100) reach cBigFile_open() in registers.
 *
 * @return @p self
 */
cBigFile* cBigFile_cBigFile1(cBigFile* self)
{
    self->field_0x4 = 0;
    self->field_0x0 = -1;
    cBigFile_open(self);
    return self;
}
#endif

int cBigFile_close(cBigFile* self);
void operator_delete(int* ptr);

//100%
INCLUDE_ASM("bx/bigfile", cBigFile__cBigFile__FP8cBigFilei);
#ifdef SKIP_ASM
/**
 * Destroys a big file: closes it, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void cBigFile__cBigFile(cBigFile* self, int flags)
{
    cBigFile_close(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void func_003DEDC0(void* handle, int arg);

//100%
INCLUDE_ASM("bx/bigfile", cBigFile_close__FP8cBigFile);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit declares func_003DEDC0 as returning void; it returns int (the result is tested).
int func_003DEDC0_ret(int handle, int arg) __asm__("func_003DEDC0");

/**
 * Closes the big file if it is open (func_003DEDC0 with 100) and marks it closed.
 *
 * @return 1 when it was not open or closed cleanly, otherwise 0
 */
int cBigFile_close(cBigFile* self)
{
    int result = 1;
    if (self->field_0x4 != 0) {
        result = func_003DEDC0_ret(self->field_0x0, 100) != 0;
    }
    self->field_0x4 = 0;
    return result;
}
#endif

//100%
INCLUDE_ASM("bx/bigfile", cBigFile_open);
#ifdef SKIP_ASM
extern int D_004A2E78;
extern "C" int func_003DEE18(const char* name, int a1);
extern "C" int func_003DED50(const char* name, int memclass, int a2, cBigFile* out);

// PORT: the unit declares cBigFile_open(cBigFile*) returning void; the body takes
// (self, name, memclass) and returns field_0x4. Bound by asm label.
int cBigFile_open_impl(cBigFile* self, const char* name, int memclass) __asm__("cBigFile_open");

/**
 * Opens a big (archive) file in a memory class, if it exists.
 *
 * The memory class is set in D_004A2E78 for the duration of the open (func_003DED50) and then restored.
 *
 * @param name  the big file's path (On Tour bigname)
 * @param memclass  memory class for its data (On Tour memtype)
 * @return 1 when the file is open, otherwise 0
 */
int cBigFile_open_impl(cBigFile* self, const char* name, int memclass)
{
    if (func_003DEE18(name, 100) == 0) {
        return 0;
    }
    int saved = D_004A2E78;
    D_004A2E78 = memclass;
    if (func_003DED50(name, memclass, 100, self) != 0) {
        self->field_0x4 = 1;
    }
    D_004A2E78 = saved;
    return self->field_0x4;
}
#endif

//100%
INCLUDE_ASM("bx/bigfile", func_00316A00__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty block manager: no memory, size or groups, and an alignment (+0x10) of 16.
 *
 * Called on D_004FF000 during static initialisation.
 *
 * @return @p self
 */
void* func_00316A00(void* self)
{
    int zero = 0;
    *(int*)self = zero;
    *(int*)((char*)self + 0x10) = 0x10;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)((char*)self + 0x8) = zero;
    *(int*)((char*)self + 0xc) = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/bigfile", func_00316A20);
#ifdef SKIP_ASM
/**
 * Rounds a size up to a multiple of the block manager's alignment (+0x10).
 *
 * cBlockMan_init uses it for the block sizes and the total memory size.
 *
 * @param size  the size in bytes
 * @return @p size rounded up
 */
extern "C" int func_00316A20(void* self, int size)
{
    int align = *(int*)((char*)self + 0x10);
    return align * ((size + align - 1) / align);
}
#endif

