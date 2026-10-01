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

INCLUDE_ASM("sound/asyncsys", func_0028A230);

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

INCLUDE_ASM("sound/asyncsys", func_0028B278);

INCLUDE_ASM("sound/asyncsys", func_0028B2D0);

INCLUDE_ASM("sound/asyncsys", func_0028B320);

INCLUDE_ASM("sound/asyncsys", func_0028B528);

