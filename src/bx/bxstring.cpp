#include "common.h"

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" cBXString* cBXString_cBXString4(cBXString* self, const char* str);

// PORT: pointer held in int to get the target's negative header offsets.
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other)
{
    int s = *(int*)other;
    if (*(int*)(s - 0xC) >= 0)
    {
        *(int*)self = s;
        *(int*)(s - 0xC) = *(int*)(s - 0xC) + 1;
    }
    else
    {
        *(char**)self = D_004A3E90;
        cBXString_cBXString4(self, *(char**)other);
    }
    return self;
}

//100%
struct cBXString;
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label.
// Declared as an operator new so gcc treats the result as fresh (malloc-like) memory.
void* operator new(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char D_0048D850[];
extern char* D_004A3E90;

extern "C" void cBXString_Realloc(cBXString* self, int size)
{
    if (size == 0) {
        *(char**)self = D_004A3E90;
        return;
    }
    int* hdr = (int*)operator new(size + 0xD, D_0048D850, 0x20000000, 0);
    char* s = (char*)(hdr + 3);
    hdr[0] = 1;
    *(char**)self = s;
    s[size] = 0;
    hdr[1] = size;
    hdr[2] = size;
}

//100%
struct cBXString;
extern char D_0048DCD4[];
extern char* D_004A3E90;
void cMemMan_free(void*);

// PORT: pointer held in int to get the target's negative header offsets.
extern "C" void cBXString_Reset(cBXString* self)
{
    int s = *(int*)self;
    if ((char*)s != D_0048DCD4)
    {
        int rc = *(int*)(s - 0xC) - 1;
        *(int*)(s - 0xC) = rc;
        if (rc == 0)
        {
            void* h = (void*)(*(int*)self - 0xC);
            if (h != 0)
                cMemMan_free(h);
        }
        *(char**)self = D_004A3E90;
    }
}

//100% - objdiff report; single-function view differs only in a relocation name
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

//100% - objdiff report; single-function view differs only in a relocation name
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

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

// Copies len chars of src starting at pos into dst, reserving extra more.
extern "C" void func_003181E8(cBXString* src, cBXString* dst, int len, int pos, int extra)
{
    int total = len + extra;
    if (total == 0)
    {
        *(char**)dst = D_004A3E90;
        return;
    }
    cBXString_Realloc(dst, total);
    func_0041605C(*(char**)dst, *(char**)src + pos, len);
}

//100%
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

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

extern "C" cBXString* cBXString_cBXString2(cBXString* self, const char* str)
{
    *(char**)self = D_004A3E90;
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    if (len != 0)
    {
        cBXString_Realloc(self, len);
        func_0041605C(*(char**)self, str, len);
    }
    return self;
}

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

// cBXString(const char* str, int n): at most n chars of str.
extern "C" cBXString* func_00318350(cBXString* self, const char* str, int n)
{
    *(char**)self = D_004A3E90;
    if (n >= strlen(str))
        n = strlen(str);
    if (n != 0)
    {
        cBXString_Realloc(self, n);
        func_0041605C(*(char**)self, str, n);
    }
    return self;
}

//100% - objdiff report; single-function view differs only in a relocation name
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

//100% - objdiff report; single-function view differs only in a relocation name
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

//100% - objdiff report; single-function view differs only in a relocation name
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

//100% - objdiff report; single-function view differs only in a relocation name
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

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" void func_00318540(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

// cBXString(a + b). PORT: pointer held in int for the negative header offsets.
extern "C" cBXString* func_003185C8(cBXString* self, cBXString* a, cBXString* b)
{
    char* tmp = D_004A3E90;
    int sa = *(int*)a;
    int sb = *(int*)b;
    func_00318540((cBXString*)&tmp, *(int*)(sa - 8), (char*)sa, *(int*)(sb - 8), (char*)sb);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    cBXString__cBXString((cBXString*)&tmp, 2);
    return self;
}

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" unsigned int strlen(const char* s);
extern "C" void func_00318540(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

// cBXString(a + str). PORT: pointer held in int for the negative header offsets.
extern "C" cBXString* func_00318630(cBXString* self, cBXString* a, const char* str)
{
    char* tmp = D_004A3E90;
    int* lenp = (int*)(*(int*)a - 8);
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    func_00318540((cBXString*)&tmp, *lenp, *(char**)a, len, str);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    cBXString__cBXString((cBXString*)&tmp, 2);
    return self;
}

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" unsigned int strlen(const char* s);
extern "C" void func_00318540(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

// cBXString(str + b). PORT: pointer held in int for the negative header offset.
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* b)
{
    char* tmp = D_004A3E90;
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    int sb = *(int*)b;
    func_00318540((cBXString*)&tmp, len, str, *(int*)(sb - 8), (char*)sb);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    cBXString__cBXString((cBXString*)&tmp, 2);
    return self;
}

// A cBXString temporary; the destructor call frees it at scope end.
struct cBXStringTmp {
    char* str;
    ~cBXStringTmp() { cBXString__cBXString((cBXString*)this, 2); }
};

// cBXString(a + ch). PORT: pointer held in int for the negative header offset;
// c is a 2-byte buffer so it gets its own stack slot, as in the target.
extern "C" cBXString* func_00318760(cBXString* self, cBXString* a, char ch)
{
    cBXStringTmp tmp;
    char c[2];
    c[0] = ch;
    tmp.str = D_004A3E90;
    int sa = *(int*)a;
    func_00318540((cBXString*)&tmp, *(int*)(sa - 8), (char*)sa, 1, c);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    return self;
}

// cBXString(ch + b). PORT: as func_00318760.
extern "C" cBXString* func_003187C8(cBXString* self, char ch, cBXString* b)
{
    cBXStringTmp tmp;
    char c[2];
    c[0] = ch;
    tmp.str = D_004A3E90;
    int sb = *(int*)b;
    func_00318540((cBXString*)&tmp, 1, c, *(int*)(sb - 8), (char*)sb);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    return self;
}

//100% - objdiff report; single-function view differs only in a relocation name
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void func_00318540(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

struct cBXStringHdrK2 {
    int ref; // -0xC
    int len; // -0x8
    int cap; // -0x4
};

// PORT: pointer held in int to get the target's negative header offsets.
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str)
{
    int s = *(int*)self;
    cBXStringHdrK2* h = (cBXStringHdrK2*)(s - 0xC);
    if (*(int*)(s - 0xC) >= 2 || *(int*)(s - 4) < *(int*)(s - 8) + len)
    {
        char* tmp;
        cBXString_cBXString1((cBXString*)&tmp, self);
        int old = *(int*)self;
        cBXString_Reset(self);
        func_00318540(self, *(int*)(old - 8), (char*)old, len, str);
        cBXString__cBXString((cBXString*)&tmp, 2);
    }
    else
    {
        func_0041605C((char*)s + h->len, str, len);
        *(int*)(*(int*)self - 8) += len;
        (*(char**)self)[*(int*)(*(int*)self - 8)] = 0;
    }
}

//100% - objdiff report; single-function view differs only in a relocation name
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

// cBXString += char.
extern "C" cBXString* func_00318968(cBXString* self, char ch)
{
    char c = ch;
    cBXString_ConcatImpl(self, 1, &c);
    return self;
}

//100% - objdiff report; single-function view differs only in a relocation name
struct cBXString;
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str);

// cBXString += cBXString; the length lives in the header 8 bytes before the chars.
extern "C" cBXString* func_003189A0(cBXString* self, char** other)
{
    cBXString_ConcatImpl(self, *(int*)(*other - 8), *other);
    return self;
}

//100% - objdiff report; single-function view differs only in a relocation name
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);

// PORT: pointer held in int to get the target's negative header offsets.
extern "C" char* func_003189D0(cBXString* self, int size)
{
    int s = *(int*)self;
    if (*(int*)(s - 0xC) >= 2 || *(int*)(s - 4) < size)
    {
        char* tmp;
        cBXString_cBXString1((cBXString*)&tmp, self);
        int len = *(int*)((int)tmp - 8);
        cBXString_Reset(self);
        cBXString_Realloc(self, len < size ? size : len);
        func_0041605C(*(char**)self, tmp, len);
        *(int*)(*(int*)self - 8) = len;
        (*(char**)self)[len] = 0;
        cBXString__cBXString((cBXString*)&tmp, 2);
    }
    return *(char**)self;
}

//100% - objdiff report; single-function view differs only in a relocation name
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

extern "C" char* strchr(char* str, int ch);

struct cBXString {
    char* str;
};

// Resizes to len chars (unique buffer) and terminates it.
extern "C" char* func_00318AF8(cBXString* self, int len)
{
    func_003189D0(self, len);
    *(int*)(self->str - 8) = len;
    self->str[len] = 0;
    return self->str;
}

// Shrinks the buffer to the string's length (reallocates when capacity != length).
// PORT: pointer held in int for the negative header offsets.
extern "C" void func_00318B40(cBXString* self)
{
    int s = *(int*)self;
    if (*(int*)(s - 8) != *(int*)(s - 4))
    {
        char* tmp;
        cBXString_cBXString1((cBXString*)&tmp, self);
        int len = *(int*)((int)tmp - 8);
        cBXString_Reset(self);
        cBXString_Realloc(self, len);
        func_0041605C(*(char**)self, tmp, len);
        cBXString__cBXString((cBXString*)&tmp, 2);
    }
}

// Makes the buffer unique, then marks it as not shareable (refcount -1).
extern "C" char* func_00318BC0(cBXString* self)
{
    char* buf = func_003189D0(self, 0);
    *(int*)(self->str - 0xC) = -1;
    return buf;
}

// Marks the string's buffer as singly owned (refcount 1) unless it is the shared empty string.
extern "C" void func_00318BF8(cBXString* self)
{
    char* s = self->str;
    if (s != D_0048DCD4)
        *(int*)(s - 0xC) = 1;
}

//100%
int cBXString_FindLastOf(cBXString* self, char ch, int len)
{
    char* result = strchr(self->str + len, ch);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}

extern "C" char* func_0041ACC0(char* str, int ch);

extern "C" char* func_0041AC40(char* str, int ch);

// Index of the last occurrence of ch, or -1 (func_0041AC40 is strrchr).
extern "C" int func_00318C60(cBXString* self, int ch)
{
    char* result = func_0041AC40(self->str, ch);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}

//100%
int cBXString_FindFirstOf(cBXString* self, char ch)
{
    char* result = func_0041ACC0(self->str, ch);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}

extern "C" char* func_0041AD80(const char* s, const char* sub);

// Index of sub at or after pos, or -1 (func_0041AD80 is strstr).
extern "C" int func_00318CE8(cBXString* self, const char* sub, int pos)
{
    char* result = func_0041AD80(self->str + pos, sub);
    if (result == 0) {
        return -1;
    }
    return result - self->str;
}

//100% - objdiff report; single-function view differs only in a relocation name
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

static inline int to_lower(int c) { if (D_00499C81[c] & 1) return c + 0x20; return c; }
// In-place lower-case; bit 1 of the ctype table = upper-case.
// PORT: pointer held in int to get the target's negative load offset.
extern "C" void func_00318D90(cBXString* self)
{
    func_00318120(self);
    char* p = self->str;
    int i = *(int*)((int)p - 8);
    while (--i >= 0)
    {
        *p = to_lower(*p);
        p++;
    }
}

INCLUDE_ASM("bx/bxstring", func_00318DF8);

//100% - objdiff report; single-function view differs only in a relocation name
extern "C" void func_00318120(cBXString* self);

extern "C" void func_00318E68(cBXString* self, int idx, char ch)
{
    func_00318120(self);
    self->str[idx] = ch;
}

INCLUDE_ASM("bx/bxstring", func_00318EB8);

extern "C" void cBXString_InitFromCString(cBXString* self, int len, const char* str);

// cBXString(char): a one-character string.
extern "C" cBXString* func_00318F20(cBXString* self, char ch)
{
    char c = ch;
    cBXString_InitFromCString(self, 1, &c);
    return self;
}

extern "C" void func_00318F90(cBXString* self, cBXString* other, int pos, int len);

// Mid(other, pos): everything from pos to the end.
extern "C" cBXString* func_00318F58(cBXString* self, cBXString* other, int pos)
{
    func_00318F90(self, other, pos, *(int*)(other->str - 8) - pos);
    return self;
}

INCLUDE_ASM("bx/bxstring", func_00318F90);

//100%
struct cBXString;
extern char* D_004A3E90;
extern "C" void func_003181E8(cBXString* src, cBXString* dst, int len, int pos, int extra);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

// Right(n): the last n chars of other. PORT: pointer held in int for the header offset.
extern "C" cBXString* func_00319028(cBXString* self, cBXString* other, int n)
{
    if (n < 0)
        n = 0;
    else
    {
        int len = *(int*)(*(int*)other - 8);
        if (len < n)
            n = len;
    }
    char* tmp = D_004A3E90;
    func_003181E8(other, (cBXString*)&tmp, n, *(int*)(*(int*)other - 8) - n, 0);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    cBXString__cBXString((cBXString*)&tmp, 2);
    return self;
}

//100% - objdiff report; single-function view differs only in a relocation name
struct cBXString;
extern char* D_004A3E90;
extern "C" void func_003181E8(cBXString* src, cBXString* dst, int len, int pos, int extra);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

// PORT: pointer held in int to get the target's negative header offset.
extern "C" cBXString* cBXString_cBXString5(cBXString* self, cBXString* other, int n)
{
    if (n < 0)
        n = 0;
    else
    {
        int len = *(int*)(*(int*)other - 8);
        if (len < n)
            n = len;
    }
    char* tmp = D_004A3E90;
    func_003181E8(other, (cBXString*)&tmp, n, 0, 0);
    cBXString_cBXString1(self, (cBXString*)&tmp);
    cBXString__cBXString((cBXString*)&tmp, 2);
    return self;
}


INCLUDE_ASM("bx/bxstring", func_00319120);

INCLUDE_ASM("bx/bxstring", func_00319170);
