#include "common.h"

void* operator_new(unsigned int size);

struct cAsyncSys {
    char pad_0x00[0x1CC];
    void* field_0x1CC;
    int field_0x1D0;
};

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi);
#ifdef SKIP_ASM
extern const char D_00482988[];
// PORT: operator_new really takes (size, tag, flags, d); unit declares 1 arg
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

void cAsyncSys_ASYNCSYS_Init(cAsyncSys* self, unsigned int x, int flags) __asm__("cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi");
void cAsyncSys_ASYNCSYS_Init(cAsyncSys* self, unsigned int x, int flags)
{
    if (x != 0) {
        self->field_0x1D0 = x;
        self->field_0x1CC = operator_new_tag(x, D_00482988, flags, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", func_0028A230);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void* self, int a);
extern "C" void func_003E4FB0(int a);
void cMemMan_free(void* p);

extern "C" void func_0028A230(cAsyncSys* self)
{
    func_00289DF0(self, 1);
    while (*(int*)((char*)self + 4) != 0) {
        func_003E4FB0(2);
        func_00289DF0(self, 1);
    }
    if (self->field_0x1CC != 0) {
        cMemMan_free(self->field_0x1CC);
        self->field_0x1CC = 0;
        self->field_0x1D0 = 0;
    }
}
#endif

INCLUDE_ASM("sound/asyncsys", func_0028A298);

INCLUDE_ASM("sound/asyncsys", func_0028A558);

INCLUDE_ASM("sound/asyncsys", func_0028A728);

INCLUDE_ASM("sound/asyncsys", func_0028AAF8);

INCLUDE_ASM("sound/asyncsys", func_0028B180);

INCLUDE_ASM("sound/asyncsys", func_0028B1B0);

INCLUDE_ASM("sound/asyncsys", func_0028B1C0);

INCLUDE_ASM("sound/asyncsys", func_0028B1C8);

INCLUDE_ASM("sound/asyncsys", func_0028B1D8);

INCLUDE_ASM("sound/asyncsys", func_0028B1E8);

INCLUDE_ASM("sound/asyncsys", func_0028B1F8);

INCLUDE_ASM("sound/asyncsys", func_0028B210);

//100%
INCLUDE_ASM("sound/asyncsys", func_0028B240__FPv);
#ifdef SKIP_ASM
int func_0028B240(void* self)
{
    return 0x3C;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", func_0028B248);
#ifdef SKIP_ASM
extern "C" void* func_0028B248(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = -1;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1c) = 0;
    *(char*)((char*)self + 0x20) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", func_0028B278);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_0028B528(void* self);
extern "C" void func_0028B730(void* self);
extern "C" void func_0028B650(void* self);

extern "C" void func_0028B278(void* self, int flags)
{
    func_0028B528(self);
    func_0028B730(self);
    func_0028B650(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", func_0028B2D0);
#ifdef SKIP_ASM
extern "C" void func_0028B528(void* self);

extern "C" void func_0028B2D0(void* self)
{
    if (*(void**)((char*)self + 0x18) != 0) {
        for (void* p = *(void**)((char*)self + 0x18); p != self; p = *(void**)((char*)p + 0x18)) {
            func_0028B528(p);
        }
    }
}
#endif

INCLUDE_ASM("sound/asyncsys", func_0028B320);

//100%
INCLUDE_ASM("sound/asyncsys", func_0028B528);
#ifdef SKIP_ASM
extern "C" int func_003B6300(int id);
extern "C" void* func_002523A8(void* self);

extern "C" void func_0028B528(void* self)
{
    if (*(int*)self == 1) {
        *(int*)self = 0;
        func_003B6300(*(int*)((char*)self + 0x4));
        *(int*)((char*)self + 0x4) = -1;
        if (*(int*)((char*)self + 0x8) == 0) {
            func_002523A8(*(void**)((char*)self + 0x10));
            *(int*)((char*)self + 0x10) = 0;
            *(int*)((char*)self + 0x14) = 0;
        }
    }
}
#endif

