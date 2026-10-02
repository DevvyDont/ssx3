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

//100%
INCLUDE_ASM("bx/bxstring", cBXString__cBXString);
#ifdef SKIP_ASM
extern char D_0048DCD4[];
void cMemMan_free(void*);
void operator_delete(int*);

// PORT: pointer held in int to get the target's negative header offsets.
extern "C" void cBXString__cBXString(cBXString* self, int flags)
{
    int s = *(int*)self;
    if ((char*)s != D_0048DCD4)
    {
        int rc = *(int*)(s - 0xC) - 1;
        *(int*)(s - 0xC) = rc;
        if (rc <= 0)
        {
            void* h = (void*)(*(int*)self - 0xC);
            if (h != 0)
                cMemMan_free(h);
        }
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

INCLUDE_ASM("bx/bxstring", cBXString_cBXString2);

INCLUDE_ASM("bx/bxstring", func_00318350);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_InitFromCString);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

// PORT: pointer held in int to get the target's negative store offset.
extern "C" void cBXString_InitFromCString(cBXString* self, int len, const char* str)
{
    cBXString_Resize(self, len);
    if (*(char**)self != D_0048DCD4)
    {
        func_0041605C(*(char**)self, str, len);
        *(int*)(*(int*)self - 8) = len;
        (*(char**)self)[len] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_operatorE);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

// PORT: pointers held in int to get the target's negative header offsets
// (refcount at -0xC, length at -8).
extern "C" cBXString* cBXString_operatorE(cBXString* self, cBXString* other)
{
    if (*(char**)self != *(char**)other)
    {
        if ((*(int*)(*(int*)self - 0xC) < 0 && *(char**)self != D_0048DCD4) ||
            *(int*)(*(int*)other - 0xC) < 0)
        {
            cBXString_InitFromCString(self, *(int*)(*(int*)other - 8), *(char**)other);
        }
        else
        {
            cBXString_Reset(self);
            *(char**)self = *(char**)other;
            (*(int*)(*(int*)self - 0xC))++;
        }
    }
    return self;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_00318540);
#ifdef SKIP_ASM
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

// Builds the string from two pieces: (len1, s1) followed by (len2, s2).
extern "C" void func_00318540(cBXString* self, int len1, const char* s1, int len2, const char* s2)
{
    int total = len1 + len2;
    if (total != 0)
    {
        cBXString_Realloc(self, total);
        func_0041605C(*(char**)self, s1, len1);
        func_0041605C(*(char**)self + len1, s2, len2);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_00318A88);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

// PORT: pointer held in int to get the target's negative store offset.
extern "C" void func_00318A88(cBXString* self, int len)
{
    func_00318120(self);
    if (len == -1)
        len = strlen(*(char**)self);
    if (*(char**)self != D_0048DCD4)
    {
        *(int*)(*(int*)self - 8) = len;
        (*(char**)self)[len] = 0;
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_00318D28);
#ifdef SKIP_ASM
extern unsigned char D_00499C81[];
static inline int to_upper(int c) { if (D_00499C81[c] & 2) return c - 0x20; return c; }
// In-place upper-case; D_00499C81 is the ctype table (+1), bit 2 = lower-case.
// PORT: pointer held in int to get the target's negative load offset.
extern "C" void func_00318D28(cBXString* self)
{
    func_00318120(self);
    char* p = self->str;
    int i = *(int*)((int)p - 8);
    while (--i >= 0)
    {
        *p = to_upper(*p);
        p++;
    }
}
#endif

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

