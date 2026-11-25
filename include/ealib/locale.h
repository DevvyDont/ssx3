#ifndef EALIB_LOCALE_H
#define EALIB_LOCALE_H

// REAL core's locale string files (.loc, locale.cpp): a chunked, little-endian file. The header chunk is followed by
// one 32-bit byte offset per language (from the file start) to that language's LOCALE_LANGUAGE chunk, whose string
// offsets (32-bit, from the language chunk) follow it; an optional index chunk (header flag bit 0) sits right after the
// header chunk and maps string ids to string numbers through 4-byte {id, number} halfword entries that follow it.
// EA source: realcore/source/std/cmn/locale.cpp (SSX On Tour: realcore 1.06.00: LOCALE_getstr(const void*
// localefile, int stringid) keeps `const LOCALE_HEADER* header`, `const LOCALE_LANGUAGE* bank`, `const LOCALE_INDEX*
// index`; LOCALE_getstate(const void* localefile, LOCALESTATE state) keeps `const LOCALE_HEADER* header` and `const
// LOCALE_LANGUAGE* language`; getstringidbyindex(unsigned short key, const LOCALE_INDEX* index)). Retail code: units
// ealib/seg_2DCAE8 (getstringidbyindex, LOCALE_getstr) and ealib/seg_2D7498 (LOCALE_getstate, LOCALE_setstate,
// compare). Users: cLocaleFile (util/localefile.h).
//
// Retail reads every field byte by byte (little-endian), except LOCALE_getstate's / LOCALE_setstate's native
// halfword access to languageid; the per-language and per-string offset tables have no struct (On Tour neither).
// Names, member order and offsets: [O] On Tour's DWARF (all three 0x10); [U] retail use as noted.
// C structs. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct LOCALE_HEADER LOCALE_HEADER;
typedef struct LOCALE_INDEX LOCALE_INDEX;
typedef struct LOCALE_LANGUAGE LOCALE_LANGUAGE;
#endif

// The file header chunk (0x10 [O]); the per-language offset table follows it.
struct LOCALE_HEADER {
    unsigned int chunkid;           // 0x0 [O]
    unsigned int chunksize;         // 0x4 [O]; [U] LOCALE_getstr: the index chunk sits this many bytes in
    unsigned int flags;             // 0x8 [O]; [U] bit 0: the file has an index (LOCALE_getstr, LOCALE_getstate 3)
    unsigned short languagecount;   // 0xC [O]; [U] LOCALE_getstate 1
    unsigned short languageid;      // 0xE [O]; [U] the current language (LOCALE_getstate 0, LOCALE_setstate 0);
                                    //      LOCALE_getstr / LOCALE_getstate 2 pick its entry of the offset table
};

CHECK_OFFSET(LOCALE_HEADER, chunkid, 0x0);
CHECK_OFFSET(LOCALE_HEADER, chunksize, 0x4);
CHECK_OFFSET(LOCALE_HEADER, flags, 0x8);
CHECK_OFFSET(LOCALE_HEADER, languagecount, 0xC);
CHECK_OFFSET(LOCALE_HEADER, languageid, 0xE);
CHECK_SIZE(LOCALE_HEADER, 0x10);

// The string-id index chunk (0x10 [O]); its 4-byte entries (sorted by id) follow it.
struct LOCALE_INDEX {
    unsigned int chunkid;           // 0x0 [O]
    unsigned int chunksize;         // 0x4 [O]
    unsigned int stringcount;       // 0x8 [O]; [U] getstringidbyindex: the bsearch entry count
    unsigned int pad;               // 0xC [O]
};

CHECK_OFFSET(LOCALE_INDEX, chunkid, 0x0);
CHECK_OFFSET(LOCALE_INDEX, chunksize, 0x4);
CHECK_OFFSET(LOCALE_INDEX, stringcount, 0x8);
CHECK_OFFSET(LOCALE_INDEX, pad, 0xC);
CHECK_SIZE(LOCALE_INDEX, 0x10);

// One language's chunk (0x10 [O]); its string offsets follow it.
struct LOCALE_LANGUAGE {
    unsigned int chunkid;           // 0x0 [O]
    unsigned int chunksize;         // 0x4 [O]
    unsigned int languageid;        // 0x8 [O]
    unsigned int stringcount;       // 0xC [O]; [U] LOCALE_getstr's range check, LOCALE_getstate 2
};

CHECK_OFFSET(LOCALE_LANGUAGE, chunkid, 0x0);
CHECK_OFFSET(LOCALE_LANGUAGE, chunksize, 0x4);
CHECK_OFFSET(LOCALE_LANGUAGE, languageid, 0x8);
CHECK_OFFSET(LOCALE_LANGUAGE, stringcount, 0xC);
CHECK_SIZE(LOCALE_LANGUAGE, 0x10);

#endif /* EALIB_LOCALE_H */
