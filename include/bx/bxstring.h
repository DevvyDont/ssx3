#ifndef BX_BXSTRING_H
#define BX_BXSTRING_H

// cBXString: EA's reference-counted string (an MFC CString clone: On Tour's method list has getBuffer/releaseBuffer,
// lockBuffer/unlockBuffer, allocCopy, concatCopy, concatInPlace, copyBeforeWrite, allocBeforeWrite, spanIncluding ...).
//
// The object is one pointer to the text. The text lives in a heap block allocated by cBXString_Realloc as
// `operator new(len + 0xD, "cStringData", 0x20000000, 0)`: a 0xC-byte tStringData header, then len chars and a 0.
// So a string's header is `(tStringData*)Str - 1`; code reads it at Str-0xC / Str-8 / Str-4.
//
// The empty string is shared and static: cBXString::EmptyString (retail D_004A3E90 holds 0x0048DCD4), whose header at
// 0x0048DCC8 is {NumRefs -1, DataLength 0, AllocLength 0}. NumRefs < 0 also marks a locked buffer (lockBuffer,
// func_00318BC0, stores -1); the copy constructor (cBXString_cBXString1) deep-copies instead of sharing then.
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
                        //       cBXString_Resize, cBXString_ConcatImpl, func_003189D0 (getBuffer-like)
};

CHECK_OFFSET(tStringData, NumRefs, 0x0);
CHECK_OFFSET(tStringData, DataLength, 0x4);
CHECK_OFFSET(tStringData, AllocLength, 0x8);
CHECK_SIZE(tStringData, 0xC);

struct cBXString {
    char* Str;          // 0x000 [O] Str; [R] cBXString_Realloc stores the block + 0xC; [U] every method reads it as the
                        //       text (strchr in cBXString_FindFirstOf/FindLastOf, the copies in func_00318540)
#ifdef __cplusplus
    // EA's class has user-declared constructors (here the out-of-line ones are the cBXString_cBXString* functions,
    // called explicitly). Declaring them keeps the type non-POD, as in EA's source: g++ 2.95 lays out non-POD locals
    // differently (cSSXApp_parseCommandLine's frame is 192 bytes with them, 176 without).
    cBXString() {}
    cBXString(const cBXString& other);
#endif
};

CHECK_OFFSET(cBXString, Str, 0x0);
CHECK_SIZE(cBXString, 0x4);

#endif /* BX_BXSTRING_H */
