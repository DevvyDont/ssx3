#include "common.h"

INCLUDE_ASM("util/locale", cFELocale_addFile);

//100%
INCLUDE_ASM("util/locale", func_00195A50);
#ifdef SKIP_ASM
extern "C" void func_002C6D70(void*, int);

struct s195A50Arr {
    void* items[4];
};

extern "C" void func_00195A50(void* self, signed char i)
{
    s195A50Arr* arr = (s195A50Arr*)((char*)self + 8);
    if (arr->items[i] != 0) {
        func_002C6D70(arr->items[i], 3);
        arr->items[i] = 0;
    }
}
#endif

INCLUDE_ASM("util/locale", func_00195AA0);

INCLUDE_ASM("util/locale", func_00195B70);

INCLUDE_ASM("util/locale", func_00195BE0);

INCLUDE_ASM("util/locale", func_00195D50);

INCLUDE_ASM("util/locale", func_00195DE0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195E58);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, signed char i);

extern "C" void func_00195E58(void* self)
{
    int flags = *(signed char*)((char*)self + 3);
    if ((flags & 1) == 0) {
        func_00195A50(self, 0xC);
        func_00195A50(self, 0xD);
        func_00195A50(self, 0xE);
        func_00195A50(self, 0xF);
    }
}
#endif

INCLUDE_ASM("util/locale", func_00195EB8);

INCLUDE_ASM("util/locale", func_00195F70);

INCLUDE_ASM("util/locale", func_00195FF0);

INCLUDE_ASM("util/locale", func_00196148);

INCLUDE_ASM("util/locale", func_00196228);

INCLUDE_ASM("util/locale", func_001962A8);

INCLUDE_ASM("util/locale", func_00196378);

