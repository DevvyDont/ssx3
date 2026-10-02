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

INCLUDE_ASM("util/locale", func_00195E58);

INCLUDE_ASM("util/locale", func_00195EB8);

INCLUDE_ASM("util/locale", func_00195F70);

INCLUDE_ASM("util/locale", func_00195FF0);

INCLUDE_ASM("util/locale", func_00196148);

INCLUDE_ASM("util/locale", func_00196228);

INCLUDE_ASM("util/locale", func_001962A8);

INCLUDE_ASM("util/locale", func_00196378);

