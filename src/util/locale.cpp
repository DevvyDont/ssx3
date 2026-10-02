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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195B70);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195B70(void* self)
{
    int lang = -1;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        lang = 0;
        break;
    case 1:
        lang = 1;
        break;
    case 2:
        lang = 2;
        break;
    case 3:
        lang = 3;
        break;
    }
    cFELocale_addFile(self, lang);
}
#endif

INCLUDE_ASM("util/locale", func_00195BE0);

INCLUDE_ASM("util/locale", func_00195D50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195DE0);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195DE0(void* self)
{
    int file = -1;
    *(signed char*)((char*)self + 0x58) = 2;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        file = 0xC;
        break;
    case 1:
        file = 0xD;
        break;
    case 2:
        file = 0xE;
        break;
    case 3:
        file = 0xF;
        break;
    }
    cFELocale_addFile(self, file);
}
#endif

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

//100%
INCLUDE_ASM("util/locale", func_00195F70);
#ifdef SKIP_ASM
// PORT: func_002C6DC0__FPv really takes a second argument (it passes $5 through to
// func_003DBB68); bind the 2-arg view to the 1-arg mangled symbol.
void* func_002C6DC0_2(void* self, void* arg) __asm__("func_002C6DC0__FPv");

struct func_00195F70_sArr {
    void* items[20];
};

extern "C" void* func_00195F70(void* self, void* arg)
{
    func_00195F70_sArr* arr = (func_00195F70_sArr*)((char*)self + 8);
    signed char i;
    for (i = 0; i < 20; i++) {
        if (arr->items[i] != 0) {
            void* r = func_002C6DC0_2(arr->items[i], arg);
            if (r != 0) {
                return r;
            }
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("util/locale", func_00195FF0);

INCLUDE_ASM("util/locale", func_00196148);

INCLUDE_ASM("util/locale", func_00196228);

INCLUDE_ASM("util/locale", func_001962A8);

INCLUDE_ASM("util/locale", func_00196378);

