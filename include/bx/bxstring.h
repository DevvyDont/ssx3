#ifndef BX_BXSTRING_H
#define BX_BXSTRING_H

// cBXString: EA's reference-counted string (an MFC CString clone: On Tour's method list has getBuffer/releaseBuffer,
// lockBuffer/unlockBuffer, allocCopy, concatCopy, concatInPlace, copyBeforeWrite, allocBeforeWrite, spanIncluding ...).
//
// The object is one pointer to the text. The text lives in a heap block allocated by cBXString_Realloc as
// `operator new(len + 0xD, "cStringData", 0x20000000, 0)`: a 0xC-byte tStringData header, then len chars and a 0.
// So a string's header is `(tStringData*)Str - 1`; code reads it at Str-0xC / Str-8 / Str-4.
//
// The empty string is shared and static: cBXString::EmptyString (retail cBXString_EmptyString holds 0x0048DCD4), whose header at
// 0x0048DCC8 is {NumRefs -1, DataLength 0, AllocLength 0}. NumRefs < 0 also marks a locked buffer (lockBuffer,
// cBXString_lockBuffer, stores -1); the copy constructor (cBXString_cBXString1) deep-copies instead of sharing then.
//
// A FLAT, POD layout (no constructors): matched code builds and destroys strings through the C-linkage functions
// (cBXString_cBXString1/2/4/5, cBXString__cBXString, ...). Code whose stack frame needs a non-POD local string (gcc
// 2.95 gives a class with a user copy constructor a BLKmode stack slot, 16 bytes) derives a local wrapper from this
// struct and adds the constructors there.
// Tags: [R] retail code/data; [U] how matched code uses the field; [O] SSX On Tour DWARF (class cBXString, size 0x4;
// struct tStringData, size 0xC).

#include "layout.h"

// The header in front of every cBXString's text (On Tour: struct tStringData, `char* data()` returns this + 1).
struct tStringData {
    int NumRefs;        // 0x000 [O]; [R] cBXString_Realloc stores 1; ++ by copy (cBXString_cBXString1, operatorE),
                        //       -- by cBXString_Reset / cBXString__cBXString (freed at 0); < 0 = static or locked
    int DataLength;     // 0x004 [O]; [R] cBXString_Realloc stores len; [U] read as the length (Str-8) everywhere
    int AllocLength;    // 0x008 [O]; [R] cBXString_Realloc stores len; [U] compared with the needed size by
                        //       cBXString_Resize, cBXString_ConcatImpl, cBXString_getBuffer (getBuffer-like)
#ifdef __cplusplus
    // The text right after the header. [O] On Tour `char* tStringData::data()` (returns this + 1); [R]
    // cBXString_Realloc stores the block + 0xC as the string's Str.
    char* data() { return (char*)(this + 1); }
#endif
};

CHECK_OFFSET(tStringData, NumRefs, 0x0);
CHECK_OFFSET(tStringData, DataLength, 0x4);
CHECK_OFFSET(tStringData, AllocLength, 0x8);
CHECK_SIZE(tStringData, 0xC);

struct cBXString {
    char* Str;          // 0x000 [O] Str; [R] cBXString_Realloc stores the block + 0xC; [U] every method reads it as the
                        //       text (strchr / strrchr in cBXString_find / cBXString_reverseFind, the copies in cBXString_concatCopy)
#ifdef __cplusplus
    // EA's class has user-declared constructors (here the out-of-line ones are the cBXString_cBXString* functions,
    // called explicitly). Declaring them keeps the type non-POD, as in EA's source: g++ 2.95 lays out non-POD locals
    // differently (cSSXApp_parseCommandLine's frame is 192 bytes with them, 176 without).
    cBXString() {}
    cBXString(const cBXString& other);
    // The header in front of the text. [O] On Tour `tStringData* cBXString::getData() const` (an inline: out-of-line
    // copies in all_in_one_bx / fe / main); [R] retail reads the header at Str-0xC / Str-8 / Str-4 (see above).
    tStringData* getData() const { return (tStringData*)Str - 1; }
#endif

    // Methods bound to their retail symbols by asm labels.
    cBXString* cBXString1(cBXString* other) __asm__("cBXString_cBXString1");
    void Realloc(int size) __asm__("cBXString_Realloc");
    void Reset() __asm__("cBXString_Reset");
    void copyBeforeWrite() __asm__("cBXString_copyBeforeWrite");
    void Resize(int size) __asm__("cBXString_Resize");
    void allocCopy(cBXString* dst, int len, int pos, int extra) __asm__("cBXString_allocCopy");
    cBXString* cBXString2(const char* str) __asm__("cBXString_cBXString2");
    cBXString* cBXString3(const char* str, int n) __asm__("cBXString_cBXString3");
    void InitFromCString(int len, const char* str) __asm__("cBXString_InitFromCString");
    cBXString* operatorE(cBXString* other) __asm__("cBXString_operatorE");
    cBXString* operatorE1(const char* str) __asm__("cBXString_operatorE1");
    void concatCopy(int len1, const char* src1, int len2, const char* src2) __asm__("cBXString_concatCopy");
    void ConcatImpl(int len, const char* str) __asm__("cBXString_ConcatImpl");
    cBXString* Concat(const char* str) __asm__("cBXString_Concat");
    char* getBuffer(int size) __asm__("cBXString_getBuffer");
    // releaseBuffer's length that means "measure the text" (name ours). [U] releaseBuffer takes strlen for it, as MFC's
    // CString::ReleaseBuffer does for its default of -1.
    enum { kMeasureLength = -1 };
    void releaseBuffer(int len) __asm__("cBXString_releaseBuffer");
    char* getBufferSetLength(int len) __asm__("cBXString_getBufferSetLength");
    void freeExtra() __asm__("cBXString_freeExtra");
    char* lockBuffer() __asm__("cBXString_lockBuffer");
    int find(char ch, int start) __asm__("cBXString_find__FP9cBXStringci");
    int reverseFind(char ch) __asm__("cBXString_reverseFind__FP9cBXStringc");
    int find1(int subString, int start) __asm__("cBXString_find1");
    void makeUpper() __asm__("cBXString_makeUpper");
    void makeLower() __asm__("cBXString_makeLower");
    void substitute(char from, char to) __asm__("cBXString_substitute");
    void setAt(int idx, char ch) __asm__("cBXString_setAt");
    cBXString* cBXString6(char ch, int n) __asm__("cBXString_cBXString6");
    cBXString* operatorE2(char ch) __asm__("cBXString_operatorE2");
    cBXString* mid(cBXString* other, int start) __asm__("cBXString_mid");
    cBXString* mid1(cBXString* other, int start, int count) __asm__("cBXString_mid1");
    cBXString* right(cBXString* other, int n) __asm__("cBXString_right");
    cBXString* cBXString5(cBXString* other, int n) __asm__("cBXString_cBXString5");
    cBXString* spanIncluding(cBXString* other, int ch) __asm__("cBXString_spanIncluding");
    cBXString* spanExcluding(cBXString* other, int ch) __asm__("cBXString_spanExcluding");
    void vprintf(const char* fmt, char* args) __asm__("cBXString_vprintf");
    void trimRight() __asm__("cBXString_trimRight");
    void trimLeft() __asm__("cBXString_trimLeft");
};

CHECK_OFFSET(cBXString, Str, 0x0);
CHECK_SIZE(cBXString, 0x4);

#endif /* BX_BXSTRING_H */
