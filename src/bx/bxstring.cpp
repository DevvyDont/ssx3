#include "common.h"
#include "bx/bxstring.h"

//100%
INCLUDE_ASM("bx/bxstring", cBXString_cBXString1);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" cBXString* cBXString_operatorE1(cBXString* self, const char* str);

/**
 * Copy-constructs a string: shares the other string's buffer, or copies it when that buffer is static or
 * locked.
 *
 * A NumRefs of 0 or more means a shared heap buffer: the pointer is copied and NumRefs incremented. A
 * negative count (the static empty string or a locked buffer) makes a fresh copy through
 * cBXString_operatorE1().
 *
 * @param other  the string to copy
 * @return @p self
 */
cBXString* cBXString::cBXString1(cBXString* other)
{
    char* buffer = other->Str;
    tStringData* data = other->getData();
    if (data->NumRefs >= 0)
    {
        Str = buffer;
        data->NumRefs = data->NumRefs + 1;
    }
    else
    {
        Str = cBXString_EmptyString;
        operatorE1(other->Str);
    }
    return this;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_Realloc);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label.
// Declared as an operator new so gcc treats the result as fresh (malloc-like) memory.
void* operator new(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_cStringData[];
extern char* cBXString_EmptyString;

/**
 * Gives the string a fresh, unshared buffer for @p size characters, or the shared empty string for 0.
 *
 * Allocates size + 0xD bytes tagged "cStringData" (flags 0x20000000): a tStringData header {NumRefs 1,
 * DataLength size, AllocLength size}, the text, and a 0 at [size]. It does not release the old buffer;
 * callers run cBXString_Reset() first.
 *
 * @param size  number of characters to make room for
 */
void cBXString::Realloc(int size)
{
    if (size == 0) {
        Str = cBXString_EmptyString;
        return;
    }
    tStringData* hdr = (tStringData*)operator new(size + 13, str_cStringData, 0x20000000, 0);
    char* buffer = hdr->data();
    hdr->NumRefs = 1;
    Str = buffer;
    buffer[size] = 0;
    hdr->DataLength = size;
    hdr->AllocLength = size;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_Reset);
#ifdef SKIP_ASM
extern char D_0048DCD4[];
extern char* cBXString_EmptyString;
void cMemMan_free(void*);

/**
 * Releases the string's buffer and makes it the shared empty string.
 *
 * Unless it already is the empty string, decrements NumRefs, frees the block (Str - 0xC) when the count
 * reaches exactly 0, and points Str at cBXString::EmptyString.
 */
// PORT: pointer held in int to get the target's negative header offsets.
void cBXString::Reset()
{
    int buffer = (int)Str;
    if ((char*)buffer != D_0048DCD4)
    {
        int rc = *(int*)(buffer - 0xC) - 1;
        *(int*)(buffer - 0xC) = rc;
        if (rc == 0)
        {
            void* header = (void*)((int)Str - 0xC);
            if (header != 0)
                cMemMan_free(header);
        }
        Str = cBXString_EmptyString;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_copyBeforeWrite);
#ifdef SKIP_ASM
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Unshares the string before a write: gives it a private copy of the text when its buffer is shared.
 *
 * When NumRefs is 2 or more, releases the shared buffer, allocates one of the same length and copies the text
 * with its terminator.
 */
void cBXString::copyBeforeWrite()
{
    // ORIGINAL-PTR: the text's tStringData header sits right before it (On Tour getData() is `(tStringData*)Str - 1`);
    // retail keeps only the old text pointer and reads each header field at a negative offset from it (-0xC, -8).
    // A held getData() pointer or a typed `- 1` builds other code, so the header is located from the text each time.
    int old = (int)Str;
    if (((tStringData*)(old - 0xC))->NumRefs >= 2)
    {
        Reset();
        Realloc(((tStringData*)(old - 0xC))->DataLength);
        func_0041605C(Str, (void*)old, ((tStringData*)(old - 0xC))->DataLength + 1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_Resize);
#ifdef SKIP_ASM
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void cBXString_Realloc(cBXString* self, int size);

/**
 * Makes sure the string owns a buffer for at least @p size characters, discarding the text if it reallocates.
 *
 * When the buffer is shared (NumRefs 2 or more) or its AllocLength is below @p size, releases it and
 * allocates a new one of exactly @p size, with DataLength = size and the text left uninitialised.
 *
 * @param size  number of characters needed
 */
void cBXString::Resize(int size)
{
    // ORIGINAL-PTR: the text's tStringData header sits right before it (On Tour getData() is `(tStringData*)Str - 1`);
    // retail reads both fields at negative offsets from the one text pointer (-0xC, -4).
    int text = (int)Str;
    if (((tStringData*)(text - 0xC))->NumRefs >= 2 || ((tStringData*)(text - 0xC))->AllocLength < size)
    {
        Reset();
        Realloc(size);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_allocCopy);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Copies @p len characters of @p src, from offset @p pos, into a fresh buffer of @p dst with room for @p extra more.
 *
 * When len + extra is 0, @p dst becomes the shared empty string (cBXString_EmptyString). Otherwise cBXString_Realloc
 * gives @p dst a new buffer of len + extra characters and func_0041605C copies the text; the old buffer of @p dst is
 * not released, so it is meant to be empty.
 *
 * @param dst  the string that receives the copy
 * @param len  number of characters to copy
 * @param pos  offset of the first character to copy
 * @param extra  extra characters to allocate
 */
void cBXString::allocCopy(cBXString* dst, int len, int pos, int extra)
{
    int total = len + extra;
    if (total == 0)
    {
        dst->Str = cBXString_EmptyString;
        return;
    }
    dst->Realloc(total);
    func_0041605C(dst->Str, Str + pos, len);
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString__cBXString);
#ifdef SKIP_ASM
extern char D_0048DCD4[];
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a string: drops its reference to the buffer, then frees the object when asked to.
 *
 * Unless it is the empty string, decrements NumRefs and frees the block when the count drops to 0 or below
 * (so a locked buffer, count -1, is freed as well).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// PORT: pointer held in int to get the target's negative header offsets.
extern "C" void cBXString__cBXString(cBXString* self, int flags)
{
    int strAddr = (int)self->Str;
    if ((char*)strAddr != D_0048DCD4)
    {
        int rc = *(int*)(strAddr - 0xC) - 1;
        *(int*)(strAddr - 0xC) = rc;
        if (rc <= 0)
        {
            void* header = (void*)((int)self->Str - 0xC);
            if (header != 0)
                cMemMan_free(header);
        }
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_cBXString2);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Constructs a string object holding a copy of a C string.
 *
 * Starts from the shared empty string (cBXString_EmptyString); for a non-empty @p str, cBXString_Realloc() allocates room
 * for its length and the characters are copied in.
 *
 * @param str  the text to copy; null counts as empty
 * @return @p self
 */
cBXString* cBXString::cBXString2(const char* str)
{
    Str = cBXString_EmptyString;
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    if (len != 0)
    {
        Realloc(len);
        func_0041605C(Str, str, len);
    }
    return this;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_cBXString3);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Constructs a string from at most @p n characters of a C string.
 *
 * Starts as the shared empty string (cBXString_EmptyString); @p n is clamped to strlen(@p str), and a non-zero count
 * gets a buffer from cBXString_Realloc and a copy of the text. @p str must not be null. The camera trigger factory uses
 * it to cut a word out of a line.
 *
 * @param str  the source text
 * @param n  the most characters to take
 * @return @p self
 */
cBXString* cBXString::cBXString3(const char* str, int n)
{
    Str = cBXString_EmptyString;
    if (n >= strlen(str))
        n = strlen(str);
    if (n != 0)
    {
        Realloc(n);
        func_0041605C(Str, str, n);
    }
    return this;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_InitFromCString);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

/**
 * Sets the string to the first @p len characters of a C string.
 *
 * Makes sure it owns a buffer of @p len characters (cBXString_Resize()), copies the text, sets DataLength and
 * the terminator. When the result is the empty string (len 0 on a shared buffer) nothing is written.
 *
 * @param len  number of characters to copy
 * @param str  the source text (it need not end at @p len)
 */
// PORT: pointer held in int to get the target's negative store offset.
void cBXString::InitFromCString(int len, const char* str)
{
    Resize(len);
    if (Str != D_0048DCD4)
    {
        func_0041605C(Str, str, len);
        *(int*)((int)Str - 8) = len;
        Str[len] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_operatorE);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

/**
 * Assigns another string to this one (operator=), sharing its buffer when it can.
 *
 * Does nothing when both hold the same text. When this buffer is locked (negative NumRefs, not the empty
 * string) or the other's is static or locked, copies the characters with cBXString_InitFromCString();
 * otherwise releases this buffer and shares the other's, incrementing NumRefs.
 *
 * @param other  the string to assign
 * @return @p self
 */
// PORT: pointers held in int to get the target's negative header offsets
// (refcount at -0xC, length at -8).
cBXString* cBXString::operatorE(cBXString* other)
{
    if (Str != other->Str)
    {
        if ((*(int*)((int)Str - 0xC) < 0 && Str != D_0048DCD4) ||
            *(int*)((int)other->Str - 0xC) < 0)
        {
            InitFromCString(*(int*)((int)other->Str - 8), other->Str);
        }
        else
        {
            Reset();
            Str = other->Str;
            (*(int*)((int)Str - 0xC))++;
        }
    }
    return this;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_operatorE1);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_InitFromCString(cBXString* self, int len, const char* str);

/**
 * Constructs a string from a C string.
 *
 * Initialises it with cBXString_InitFromCString() using strlen(@p str); a null @p str gives an empty string.
 *
 * @param str  NUL-terminated text, or 0
 * @return self
 */
cBXString* cBXString::operatorE1(const char* str)
{
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    InitFromCString(len, str);
    return this;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_concatCopy);
#ifdef SKIP_ASM
extern "C" void cBXString_Realloc(cBXString* self, int size);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Builds the string from two pieces: @p len1 characters of @p src1 followed by @p len2 characters of @p src2.
 *
 * Allocates a fresh buffer of len1 + len2 (cBXString_Realloc()) and copies both pieces; when both lengths are
 * 0 it leaves the string untouched. Callers release the old buffer first.
 *
 * @param len1  length of the first piece
 * @param src1    the first piece
 * @param len2  length of the second piece
 * @param src2    the second piece
 */
// Builds the string from two pieces: (len1, s1) followed by (len2, s2).
void cBXString::concatCopy(int len1, const char* src1, int len2, const char* src2)
{
    int total = len1 + len2;
    if (total != 0)
    {
        Realloc(total);
        func_0041605C(Str, src1, len1);
        func_0041605C(Str + len1, src2, len2);
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", func_003185C8);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" void cBXString_concatCopy(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Builds @p self as the concatenation of two strings.
 *
 * cBXString_concatCopy joins the texts into a temporary, @p self is copy-constructed from it (cBXString_cBXString1) and
 * the temporary is destroyed (cBXString__cBXString). The result is returned by value: @p self is the caller's result
 * slot.
 *
 * @param left  the first string
 * @param right  the second string
 * @return @p self
 */
// cBXString(a + b). PORT: pointer held in int for the negative header offsets.
extern "C" cBXString* func_003185C8(cBXString* self, cBXString* left, cBXString* right)
{
    char* joined = cBXString_EmptyString;
    // PORT: read as int; reading a->Str (char*) schedules these loads differently (refactor gate, 2026-11-16)
    int sa = *(int*)left;
    int sb = *(int*)right;
    ((cBXString*)&joined)->concatCopy(*(int*)(sa - 8), (char*)sa, *(int*)(sb - 8), (char*)sb);
    self->cBXString1((cBXString*)&joined);
    cBXString__cBXString((cBXString*)&joined, 2);
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", func_00318630);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_concatCopy(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Builds @p self as a string followed by a C string (null counts as empty).
 *
 * cBXString_concatCopy joins the texts into a temporary, @p self is copy-constructed from it (cBXString_cBXString1) and
 * the temporary is destroyed (cBXString__cBXString). The result is returned by value: @p self is the caller's result
 * slot. The world, world cache and game loader use it to build file paths.
 *
 * @param bxString  the string
 * @param str  the C string to append, or null
 * @return @p self
 */
// cBXString(a + str). PORT: pointer held in int for the negative header offsets.
extern "C" cBXString* func_00318630(cBXString* self, cBXString* bxString, const char* str)
{
    char* joined = cBXString_EmptyString;
    int* lenp = (int*)((int)bxString->Str - 8);
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    ((cBXString*)&joined)->concatCopy(*lenp, bxString->Str, len, str);
    self->cBXString1((cBXString*)&joined);
    cBXString__cBXString((cBXString*)&joined, 2);
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", func_003186D0);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_concatCopy(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Builds @p self as a C string (null counts as empty) followed by a string.
 *
 * cBXString_concatCopy joins the texts into a temporary, @p self is copy-constructed from it (cBXString_cBXString1) and
 * the temporary is destroyed (cBXString__cBXString). The result is returned by value: @p self is the caller's result
 * slot. The world, world cache and game loader use it to put a directory in front of a name.
 *
 * @param str  the C string, or null
 * @param bxString  the string to append
 * @return @p self
 */
// cBXString(str + b). PORT: pointer held in int for the negative header offset.
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* bxString)
{
    char* joined = cBXString_EmptyString;
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    int sb = (int)bxString->Str;
    ((cBXString*)&joined)->concatCopy(len, str, *(int*)(sb - 8), (char*)sb);
    self->cBXString1((cBXString*)&joined);
    cBXString__cBXString((cBXString*)&joined, 2);
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", func_00318760);
#ifdef SKIP_ASM
// local string temp: non-POD (user copy ctor) so it gets a 16-byte BLKmode stack slot
struct sBXTmp_318760 {
    char* p;
    sBXTmp_318760(char* s) : p(s) {}
    sBXTmp_318760(const sBXTmp_318760& o) : p(o.p) {}
};

/**
 * Builds @p self as a string followed by one character.
 *
 * cBXString_concatCopy joins the texts into a temporary, @p self is copy-constructed from it (cBXString_cBXString1) and
 * the temporary is destroyed (cBXString__cBXString). The result is returned by value: @p self is the caller's result
 * slot. Nothing in the C calls it.
 *
 * @param bxString  the string
 * @param ch  the character to append
 * @return @p self
 */
// cBXString(a + ch). PORT: pointer held in int for the negative header offset.
extern "C" cBXString* func_00318760(cBXString* self, cBXString* bxString, char ch)
{
    sBXTmp_318760 joined(cBXString_EmptyString);
    char charBuf = ch;
    int sa = (int)bxString->Str;
    ((cBXString*)&joined)->concatCopy(*(int*)(sa - 8), (char*)sa, 1, &charBuf);
    self->cBXString1((cBXString*)&joined);
    cBXString__cBXString((cBXString*)&joined, 2);
    return self;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", func_003187C8);
#ifdef SKIP_ASM
// local string temp: non-POD (user copy ctor) so it gets a 16-byte BLKmode stack slot
struct sBXTmp_3187C8 {
    char* p;
    sBXTmp_3187C8(char* s) : p(s) {}
    sBXTmp_3187C8(const sBXTmp_3187C8& o) : p(o.p) {}
};

/**
 * Builds @p self as one character followed by a string.
 *
 * cBXString_concatCopy joins the texts into a temporary, @p self is copy-constructed from it (cBXString_cBXString1) and
 * the temporary is destroyed (cBXString__cBXString). The result is returned by value: @p self is the caller's result
 * slot. Nothing in the C calls it.
 *
 * @param ch  the leading character
 * @param bxString  the string
 * @return @p self
 */
// cBXString(ch + b). PORT: pointer held in int for the negative header offset.
extern "C" cBXString* func_003187C8(cBXString* self, char ch, cBXString* bxString)
{
    sBXTmp_3187C8 joined(cBXString_EmptyString);
    char charBuf = ch;
    int sb = (int)bxString->Str;
    ((cBXString*)&joined)->concatCopy(1, &charBuf, *(int*)(sb - 8), (char*)sb);
    self->cBXString1((cBXString*)&joined);
    cBXString__cBXString((cBXString*)&joined, 2);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_ConcatImpl);
#ifdef SKIP_ASM
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString_Reset(cBXString* self);
extern "C" void cBXString_concatCopy(cBXString* self, int len1, const char* s1, int len2, const char* s2);
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Appends @p len characters of @p str to the string, in place when its buffer is private and big enough.
 *
 * When the buffer is shared or AllocLength < DataLength + len, it keeps a reference to the old text, releases
 * the buffer and rebuilds the string with cBXString_concatCopy(); otherwise it copies the text to the end,
 * adds @p len to DataLength and writes the terminator.
 *
 * @param len  number of characters to append
 * @param str  the text to append
 */
void cBXString::ConcatImpl(int len, const char* str)
{
    // ORIGINAL-PTR: the text's tStringData header sits right before it (On Tour getData() is `(tStringData*)Str - 1`);
    // retail reads its fields at negative offsets from the text pointer (-0xC, -8, -4), the old text's too.
    int text = (int)Str;
    tStringData* header = (tStringData*)(text - 0xC);
    if (((tStringData*)(text - 0xC))->NumRefs >= 2
        || ((tStringData*)(text - 0xC))->AllocLength < ((tStringData*)(text - 0xC))->DataLength + len)
    {
        char* copy;
        ((cBXString*)&copy)->cBXString1(this);
        int old = (int)Str;
        Reset();
        concatCopy(((tStringData*)(old - 0xC))->DataLength, (char*)old, len, str);
        cBXString__cBXString((cBXString*)&copy, 2);
    }
    else
    {
        func_0041605C((char*)text + header->DataLength, str, len);
        ((tStringData*)((int)Str - 0xC))->DataLength += len;
        Str[getData()->DataLength] = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_Concat);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str);

/**
 * Appends a C string to this string.
 *
 * Passes strlen(@p str) and the text to cBXString_ConcatImpl(); a null @p str appends nothing.
 *
 * @param str  NUL-terminated text to append, or 0
 * @return self
 */
cBXString* cBXString::Concat(const char* str)
{
    int len;
    if (str != 0)
        len = strlen(str);
    else
        len = 0;
    ConcatImpl(len, str);
    return this;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", func_00318968);
#ifdef SKIP_ASM
/**
 * Appends one character to the string (cBXString_ConcatImpl with a length of 1).
 *
 * Nothing in the C calls it.
 *
 * @param ch  the character to append
 * @return @p self
 */
extern "C" cBXString* func_00318968(cBXString* self, char ch)
{
    char buf = ch;
    self->ConcatImpl(1, &buf);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", func_003189A0);
#ifdef SKIP_ASM
extern "C" void cBXString_ConcatImpl(cBXString* self, int len, const char* str);

/**
 * Appends another string to this one (cBXString_ConcatImpl with the other's DataLength, read 8 bytes before its text).
 *
 * The async file front end uses it to build a message text.
 *
 * @param other  the string to append
 * @return @p self
 */
extern "C" cBXString* func_003189A0(cBXString* self, char** other)
{
    self->ConcatImpl(*(int*)(*other - 8), *other);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_getBuffer);
#ifdef SKIP_ASM
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);

/**
 * Returns a writable buffer for the string with room for at least @p size characters.
 *
 * When the buffer is shared or too small it reallocates it to max(length, size) characters and copies the
 * current text over, keeping DataLength. Pair with cBXString_releaseBuffer() after writing.
 *
 * @param size  the minimum number of characters the buffer must hold (On Tour minlen)
 * @return the string's text buffer (Str)
 */
char* cBXString::getBuffer(int size)
{
    // ORIGINAL-PTR: the text's tStringData header sits right before it (On Tour getData() is `(tStringData*)Str - 1`);
    // retail reads both fields at negative offsets from the one text pointer (-0xC, -4).
    int text = (int)Str;
    if (((tStringData*)(text - 0xC))->NumRefs >= 2 || ((tStringData*)(text - 0xC))->AllocLength < size)
    {
        char* copy;
        ((cBXString*)&copy)->cBXString1(this);
        int len = ((cBXString*)&copy)->getData()->DataLength;
        Reset();
        Realloc(len < size ? size : len);
        func_0041605C(Str, copy, len);
        getData()->DataLength = len;
        Str[len] = 0;
        cBXString__cBXString((cBXString*)&copy, 2);
    }
    return Str;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_releaseBuffer);
#ifdef SKIP_ASM
extern char D_0048DCD4[];

/**
 * Finishes a direct write into the string's buffer: stores the new length and the terminator.
 *
 * Unshares the buffer first (cBXString_copyBeforeWrite()); a length of kMeasureLength (-1) means strlen() of the
 * text. Nothing is stored when the string is the empty string.
 *
 * @param len  the new length (On Tour newlen), or kMeasureLength to measure the text
 */
// PORT: pointer held in int to get the target's negative store offset.
void cBXString::releaseBuffer(int len)
{
    copyBeforeWrite();
    if (len == kMeasureLength)
        len = strlen(Str);
    if (Str != D_0048DCD4)
    {
        *(int*)((int)Str - 8) = len;
        Str[len] = 0;
    }
}
#endif

extern "C" char* strchr(char* str, int ch);

//100%
INCLUDE_ASM("bx/bxstring", cBXString_getBufferSetLength);
#ifdef SKIP_ASM
/**
 * Returns a writable buffer of at least @p len characters and sets the string's length to exactly @p len.
 *
 * Calls cBXString_getBuffer, stores @p len in the header's DataLength and terminates the string at it.
 *
 * @param len  the new length (On Tour newlen)
 * @return the string's character buffer
 */
char* cBXString::getBufferSetLength(int len)
{
    getBuffer(len);
    getData()->DataLength = len;
    Str[len] = 0;
    return Str;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_freeExtra);
#ifdef SKIP_ASM
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Shrinks the string's buffer to its length when the allocation is larger.
 *
 * When the header's DataLength and AllocLength differ, copies the string, resets and reallocates this one
 * to the exact length and copies the characters back (func_0041605C, memcpy).
 */
// Make the string unique (copy-on-write detach) when its two header counts differ.
void cBXString::freeExtra()
{
    // ORIGINAL-PTR: the text's tStringData header sits right before it (On Tour getData() is `(tStringData*)Str - 1`);
    // retail reads both fields at negative offsets from the one text pointer (-8, -4).
    int text = (int)Str;
    if (((tStringData*)(text - 0xC))->DataLength != ((tStringData*)(text - 0xC))->AllocLength) {
        cBXString copy;
        copy.cBXString1(this);
        int len = copy.getData()->DataLength;
        Reset();
        Realloc(len);
        func_0041605C(Str, copy.Str, len);
        cBXString__cBXString(&copy, 2);
    }
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_lockBuffer);
#ifdef SKIP_ASM
/**
 * Returns a writable buffer and locks it by setting the header's NumRefs to -1, so it is never shared.
 *
 * @return the string's character buffer
 */
char* cBXString::lockBuffer()
{
    char* str = getBuffer(0);
    getData()->NumRefs = -1;
    return str;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_unlockBuffer);
#ifdef SKIP_ASM
/**
 * Unlocks a string's buffer: sets the reference count (Str - 0xC) back to 1.
 *
 * Undoes cBXString_lockBuffer, which sets it to -1. The shared empty string's text (D_0048DCD4) is left alone. Nothing
 * in the C calls it.
 *
 * @param bxString  the string
 */
extern "C" void cBXString_unlockBuffer(void **bxString) {
    tStringData* temp_4;

    temp_4 = (tStringData *)*bxString;
    if ((void *)temp_4 != &D_0048DCD4) {
        temp_4[-1].NumRefs = 1;
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_find__FP9cBXStringci);
#ifdef SKIP_ASM
/**
 * Finds a character in the string, searching from a start offset.
 *
 * A forward search: it calls strchr() on the text from @p start onwards, so it returns the first occurrence at or
 * after that offset. (EA's symbol called it FindLastOf.)
 *
 * @param ch   character to look for
 * @param start  offset into the string to start searching at
 * @return the offset of the match from the start of the string, or -1 if there is none
 */
int cBXString::find(char ch, int start)
{
    char* result = strchr(Str + start, ch);
    if (result == 0) {
        return -1;
    }
    return result - Str;
}
#endif

extern "C" char* strrchr(char* str, int ch);

//100%
INCLUDE_ASM("bx/bxstring", cBXString_findOneOf);
#ifdef SKIP_ASM
extern "C" int strpbrk(int);

/**
 * Returns the offset of the string's first character that belongs to a character set, or -1 when none does.
 *
 * strpbrk behaves like strpbrk. The set pointer is the second argument, which the C does not declare; it reaches
 * strpbrk untouched in its register. Nothing in the C calls it.
 *
 * @param string  the string
 * @return the offset from the start of the text, or -1
 */
extern "C" int cBXString_findOneOf(int *string) {
    int temp_2;

    temp_2 = strpbrk(*string);
    if (temp_2 == 0) {
        return -1;
    }
    return temp_2 - *string;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_reverseFind__FP9cBXStringc);
#ifdef SKIP_ASM
/**
 * Finds a character in the string, searching from the end.
 *
 * A reverse search: it calls strrchr() (newlib strrchr), so it returns the last occurrence of @p ch. (EA's
 * symbol called it FindFirstOf.)
 *
 * @param ch  character to look for
 * @return the offset of the last match from the start of the string, or -1 if there is none
 */
int cBXString::reverseFind(char ch)
{
    char* result = strrchr(Str, ch);
    if (result == 0) {
        return -1;
    }
    return result - Str;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_find1);
#ifdef SKIP_ASM
extern "C" char* strstr(char* str);

/**
 * Returns the offset of the first occurrence of a substring at or after @p start, or -1 when there is none.
 *
 * strstr behaves like strstr. The substring is the second argument, declared as int @p subString; it reaches
 * strstr untouched in its register. Nothing in the C calls it.
 *
 * @param subString  the substring to find (declared int; it is a const char*)
 * @param start  offset to start searching at
 * @return the offset from the start of the text, or -1
 */
int cBXString::find1(int subString, int start)
{
    char* result = strstr(Str + start);
    if (result == 0) {
        return -1;
    }
    return result - Str;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_makeUpper);
#ifdef SKIP_ASM
extern unsigned char D_00499C81[];
static inline int to_upper(int c) { if (D_00499C81[c] & 2) return c - 0x20; return c; }
/**
 * Converts the string to upper case in place.
 *
 * Unshares the buffer first (cBXString_copyBeforeWrite()), then subtracts 0x20 from each character that the
 * ctype table D_00499C81 marks lower-case (mask 2).
 */
// In-place upper-case; D_00499C81 is the ctype table (+1), bit 2 = lower-case.
void cBXString::makeUpper()
{
    copyBeforeWrite();
    char* cursor = Str;
    int i = getData()->DataLength;
    while (--i >= 0)
    {
        *cursor = to_upper(*cursor);
        cursor++;
    }
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_makeLower);
#ifdef SKIP_ASM
extern unsigned char D_00499C81[];
static inline int to_lower(int c) { if (D_00499C81[c] & 1) return c + 0x20; return c; }
/**
 * Converts the string to lower case in place, after detaching it from any shared copy (cBXString_copyBeforeWrite).
 */
// In-place lower-case; D_00499C81 is the ctype table (+1), bit 1 = upper-case.
void cBXString::makeLower()
{
    copyBeforeWrite();
    char* cursor = Str;
    int i = getData()->DataLength;
    while (--i >= 0)
    {
        *cursor = to_lower(*cursor);
        cursor++;
    }
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_substitute);
#ifdef SKIP_ASM
/**
 * Replaces every occurrence of one character with another, in place, after detaching shared copies.
 *
 * @param from  the character to replace
 * @param to  its replacement
 */
// Replace every `from` with `to` in place.
void cBXString::substitute(char from, char to)
{
    copyBeforeWrite();
    char* cursor = Str;
    int i = getData()->DataLength;
    while (--i >= 0)
    {
        if (*cursor == from)
            *cursor = to;
        cursor++;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_setAt);
#ifdef SKIP_ASM
extern "C" void cBXString_copyBeforeWrite(cBXString* self);

/**
 * Sets one character of the string, after detaching it from any shared copy (cBXString_copyBeforeWrite).
 *
 * The index is not range-checked.
 *
 * @param idx  the character index
 * @param ch  the new character
 */
void cBXString::setAt(int idx, char ch)
{
    copyBeforeWrite();
    Str[idx] = ch;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_cBXString6);
#ifdef SKIP_ASM
extern "C" void* memset(void*, int, unsigned int);

/**
 * Constructs a string of @p n copies of a character.
 *
 * Starts as the shared empty string (cBXString_EmptyString); for @p n above 0, cBXString_Realloc allocates @p n
 * characters and memset fills them. Nothing in the C calls it.
 *
 * @param ch  the character
 * @param n  the number of copies
 * @return @p self
 */
cBXString* cBXString::cBXString6(char ch, int n)
{
    Str = cBXString_EmptyString;
    if (n > 0) {
        Realloc(n);
        memset(Str, ch, n);
    }
    return this;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_operatorE2);
#ifdef SKIP_ASM
/**
 * Assigns a single character to the string (cBXString_InitFromCString with a length of 1).
 *
 * It does not reset the text pointer first, so it works on an existing string. Nothing in the C calls it.
 *
 * @param ch  the character
 * @return @p self
 */
cBXString* cBXString::operatorE2(char ch)
{
    char buf = ch;
    InitFromCString(1, &buf);
    return this;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_mid);
#ifdef SKIP_ASM
extern "C" cBXString* cBXString_mid1(cBXString* self, cBXString* other, int start, int count);

/**
 * Builds @p self from @p other's characters from @p start to the end (cBXString_mid1 with count = length - start).
 *
 * The result is returned by value: @p self is the caller's result slot. Nothing in the C calls it.
 *
 * @param other  the source string
 * @param start  offset of the first character
 * @return @p self
 */
cBXString* cBXString::mid(cBXString* other, int start)
{
    mid1(other, start, other->getData()->DataLength - start);
    return this;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_mid1);
#ifdef SKIP_ASM
/**
 * Builds @p self from @p count characters of @p other starting at @p start.
 *
 * Negative @p start or @p count become 0, @p count is cut to the end of the text, and a @p start past the end gives an
 * empty result. The characters are copied with cBXString_allocCopy into a temporary, @p self is copy-constructed from it
 * (cBXString_cBXString1) and the temporary is destroyed. The result is returned by value: @p self is the caller's
 * result slot.
 *
 * @param other  the source string
 * @param start  offset of the first character
 * @param count  the number of characters
 * @return @p self
 */
// Mid(start, count).
cBXString* cBXString::mid1(cBXString* other, int start, int count)
{
    if (start < 0)
        start = 0;
    if (count < 0)
        count = 0;
    int len = other->getData()->DataLength;
    if (len < start + count)
        count = len - start;
    if (len < start)
        count = 0;
    char* substr = cBXString_EmptyString;
    other->allocCopy((cBXString*)&substr, count, start, 0);
    cBXString1((cBXString*)&substr);
    cBXString__cBXString((cBXString*)&substr, 2);
    return this;
}
#endif

//100%
INCLUDE_ASM("bx/bxstring", cBXString_right);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" void cBXString_allocCopy(cBXString* src, cBXString* dst, int len, int pos, int extra);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Returns the last @p n characters of a string (right(n)), with @p n clamped to 0 and the string's length.
 *
 * Returns a cBXString by value: @p self is the hidden result pointer and @p other the source (On Tour's
 * this). The tail is copied into a temporary with cBXString_allocCopy, @p self is copy-constructed from it and the
 * temporary destroyed.
 *
 * @param other  the source string (On Tour this)
 * @param n      number of characters to take from the end (On Tour count)
 * @return @p self
 */
// Right(n): the last n chars of other.
cBXString* cBXString::right(cBXString* other, int n)
{
    if (n < 0)
        n = 0;
    else
    {
        int len = other->getData()->DataLength;
        if (len < n)
            n = len;
    }
    char* dest = cBXString_EmptyString;
    // read through a plain int* (not a member access), so the load stays after the store to dest, as in retail
    int* length = &other->getData()->DataLength;
    other->allocCopy((cBXString*)&dest, n, *length - n, 0);
    cBXString1((cBXString*)&dest);
    cBXString__cBXString((cBXString*)&dest, 2);
    return this;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/bxstring", cBXString_cBXString5);
#ifdef SKIP_ASM
extern char* cBXString_EmptyString;
extern "C" void cBXString_allocCopy(cBXString* src, cBXString* dst, int len, int pos, int extra);
extern "C" cBXString* cBXString_cBXString1(cBXString* self, cBXString* other);
extern "C" void cBXString__cBXString(cBXString* self, int flags);

/**
 * Makes @p self a copy of the first @p n characters of another string (like left(n)), @p n clamped to its
 * length.
 *
 * The same shape as cBXString_right() with copy position 0: cBXString_allocCopy copies into a temporary, @p self is
 * copy-constructed from it and the temporary destroyed. cSSXApp_parseCommandLine() uses it to cut a file
 * extension.
 *
 * @param other  the source string
 * @param n      number of leading characters to keep; negative gives an empty string
 * @return @p self
 */
cBXString* cBXString::cBXString5(cBXString* other, int n)
{
    if (n < 0)
        n = 0;
    else
    {
        int len = other->getData()->DataLength;
        if (len < n)
            n = len;
    }
    char* dest = cBXString_EmptyString;
    other->allocCopy((cBXString*)&dest, n, 0, 0);
    cBXString1((cBXString*)&dest);
    cBXString__cBXString((cBXString*)&dest, 2);
    return this;
}
#endif


//100%
INCLUDE_ASM("bx/bxstring", cBXString_spanIncluding);
#ifdef SKIP_ASM
// PORT: the unit defines cBXString_cBXString5 returning cBXString*; the caller ignores the result.
void cBXString_cBXString5_v(cBXString* self, cBXString* other, int n) __asm__("cBXString_cBXString5");
extern "C" int strspn(char* str, int ch);

/**
 * Returns the leading run of a string made of characters from a set (MFC CString::SpanIncluding).
 *
 * Constructs @p self from the first strspn(other->Str, ch) characters of @p other (cBXString_cBXString5).
 *
 * @param other  the source string (On Tour this)
 * @param ch  the character set (On Tour const char* set; declared int here)
 * @return @p self, the result string
 */
cBXString* cBXString::spanIncluding(cBXString* other, int ch)
{
    cBXString_cBXString5_v(this, other, strspn(other->Str, ch));
    return this;
}
#endif
//100%
INCLUDE_ASM("bx/bxstring", cBXString_spanExcluding);
#ifdef SKIP_ASM
// PORT: the unit defines cBXString_cBXString5 returning cBXString*; the caller ignores the result.
void cBXString_cBXString5_v(cBXString* self, cBXString* other, int n) __asm__("cBXString_cBXString5");
extern "C" int strcspn(char* str, int ch);

/**
 * Returns the leading run of a string that contains no character from a set (MFC CString::SpanExcluding).
 *
 * Constructs @p self from the first strcspn(other->Str, ch) characters of @p other (cBXString_cBXString5).
 *
 * @param other  the source string (On Tour this)
 * @param ch  the character set (declared int here)
 * @return @p self, the result string
 */
cBXString* cBXString::spanExcluding(cBXString* other, int ch)
{
    cBXString_cBXString5_v(this, other, strcspn(other->Str, ch));
    return this;
}
#endif
