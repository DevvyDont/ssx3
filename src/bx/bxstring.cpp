#include "common.h"

INCLUDE_ASM("bx/bxstring", cBXString_cBXString1);

INCLUDE_ASM("bx/bxstring", cBXString_Realloc);

INCLUDE_ASM("bx/bxstring", cBXString_Reset);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_00318120);
#ifdef SKIP_ASM
struct cBXString;
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

// PORT: pointer held in int to get the target's negative load offsets.
extern "C" void func_00318120(cBXString* self)
{
    int old = *(int*)self;
    if (*(int*)(old - 0xC) >= 2)
    {
        cBXString_Reset(self);
        cBXString_Realloc(self, *(int*)(old - 8));
        func_0041605C(*(char**)self, (void*)old, *(int*)(old - 8) + 1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_Resize);
#ifdef SKIP_ASM
struct cBXString;
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void cBXString_Realloc(cBXString* self, int size);

// PORT: pointer held in int to get the target's negative load offsets.
extern "C" void cBXString_Resize(cBXString* self, int size)
{
    int s = *(int*)self;
    if (*(int*)(s - 0xC) >= 2 || *(int*)(s - 4) < size)
    {
        cBXString_Reset(self);
        cBXString_Realloc(self, size);
    }
}
#endif

INCLUDE_ASM("bx/bxstring", func_003181E8);

INCLUDE_ASM("bx/bxstring", cBXString__cBXString);

INCLUDE_ASM("bx/bxstring", cBXString_cBXString2);

INCLUDE_ASM("bx/bxstring", func_00318350);

INCLUDE_ASM("bx/bxstring", cBXString_InitFromCString);

INCLUDE_ASM("bx/bxstring", cBXString_operatorE);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_cBXString4);
#ifdef SKIP_ASM
struct cBXString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_InitFromCString(cBXString* self, int len, const char* str);

extern "C" cBXString* cBXString_cBXString4(cBXString* self, const char* str)
{
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    cBXString_InitFromCString(self, len, str);
    return self;
}
#endif

INCLUDE_ASM("bx/bxstring", func_00318540);

INCLUDE_ASM("bx/bxstring", func_003185C8);

INCLUDE_ASM("bx/bxstring", func_00318630);

INCLUDE_ASM("bx/bxstring", func_003186D0);

INCLUDE_ASM("bx/bxstring", cBXString_ConcatImpl);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_Concat);
#ifdef SKIP_ASM
struct cBXString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str);

extern "C" cBXString* cBXString_Concat(cBXString* self, const char* str)
{
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    cBXString_ConcatImpl(self, len, str);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_003189A0);
#ifdef SKIP_ASM
struct cBXString;
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str);

// cBXString += cBXString; the length lives in the header 8 bytes before the chars.
extern "C" cBXString* func_003189A0(cBXString* self, char** other)
{
    cBXString_ConcatImpl(self, *(int*)(*other - 8), *other);
    return self;
}
#endif

INCLUDE_ASM("bx/bxstring", func_003189D0);

INCLUDE_ASM("bx/bxstring", func_00318A88);

extern "C" char* strchr(char* str, int ch);

struct cBXString {
    char* str;
};

//100%
INCLUDE_ASM("bx/bxstring", cBXString_FindLastOf__FP9cBXStringci);
#ifdef SKIP_ASM
int cBXString_FindLastOf(cBXString* self, char ch, int len)
{
    char* result = strchr(self->str + len, ch);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}
#endif

extern "C" char* func_0041ACC0(char* str, int ch);

//100%
INCLUDE_ASM("bx/bxstring", cBXString_FindFirstOf__FP9cBXStringc);
#ifdef SKIP_ASM
int cBXString_FindFirstOf(cBXString* self, char ch)
{
    char* result = func_0041ACC0(self->str, ch);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}
#endif

INCLUDE_ASM("bx/bxstring", func_00318D28);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_00318E68);
#ifdef SKIP_ASM
extern "C" void func_00318120(cBXString* self);

extern "C" void func_00318E68(cBXString* self, int idx, char ch)
{
    func_00318120(self);
    self->str[idx] = ch;
}
#endif

INCLUDE_ASM("bx/bxstring", func_00319028);

INCLUDE_ASM("bx/bxstring", cBXString_cBXString5);

