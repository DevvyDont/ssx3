#ifndef UTIL_LOCALEFILE_H
#define UTIL_LOCALEFILE_H

// cLocaleFile: one loaded locale string file (data/locale/*.loc). loadFile loads the file whole (FILE_load), counts its
// strings (LOCALE_getstate), and when the chunk after the first is "LOCT" points at that hash-table chunk and converts
// its {hash, index} entries to {hash, string}; getStringByHash binary-searches that table, getStringByIndex goes through
// LOCALE_getstr. The front-end locale (cFELocale, util/locale.h) keeps up to 20 of them (mpLocaleFile[], +0x8);
// cFELocale_addFile allocates and constructs them (the ctor is inlined there) and cFELocale_deleteFile deletes them.
// EA's source: d:/ssx/code/source/util/localefile.cpp (July 2003 preview assert strings). Retail methods: splat unit
// worldpainter/quadtree (0x002C6D70 dtor .. 0x002C6E98 loadFile); the ctor is inlined in cFELocale_addFile
// (util/locale).
//
// Size 0x14: [R] cFELocale_addFile does operator new(20, "LocaleFile") (cMemMan_alloc) and the inlined ctor writes
// 0x0, 0x4 and 0xC; loadFile writes 0x0-0x10. On Tour: 0x124 (the same five members at the same offsets, then four
// statics-turned-members g_StringDatabase .. g_CurrentLanguageID and m_StringNotFound[128] from 0x14, which retail
// does not have: the allocation is 0x14).
// No base class, no vtable (the dtor is called directly: cFELocale_deleteFile -> cLocaleFile__cLocaleFile(file, 3)).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the inlined ctor in cFELocale_addFile, the dtor cLocaleFile__cLocaleFile and the 0x14 allocation;
//   [P] none (no paired methods, no inline accessors);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cLocaleFile: mLanguage 0x0, mpStringTableData 0x4, mNumEntries 0x8,
//       mpHashTableHeader 0xC, mpHashTableData 0x10; struct tHashEntry 0x8: mHashID, mID).

#include "layout.h"

// One entry of a locale file's hash table (On Tour tHashEntry, 0x8): the string id's hash and, after loadFile, the
// string itself (the file stores the string index; loadFile replaces it with getStringByIndex's result).
struct tHashEntry {
    unsigned int mHashID;               // 0x0 [O]; [U] getStringByHash compares the key with it (sorted, unsigned)
    unsigned int mID;                   // 0x4 [O]; [U] loadFile: index -> string pointer; getStringByHash returns it
};

CHECK_OFFSET(tHashEntry, mHashID, 0x0);
CHECK_OFFSET(tHashEntry, mID, 0x4);
CHECK_SIZE(tHashEntry, 0x8);

struct cLocaleFile {
    signed char mLanguage;              // 0x00 [O] char; [R] inlined ctor -1 (sb); [U] loadFile stores its Language
                                        //      argument (sb); cFELocale_getStringByHash compares it (lb) with the
                                        //      current language
    char unk_01[0x3];                   // 0x01 padding
    char* mpStringTableData;            // 0x04 [O]; [R] inlined ctor 0; [U] loadFile = FILE_load(...); the dtor frees it
                                        //      (MEM_free) when set; getStringByIndex passes it to LOCALE_getstr
    unsigned int mNumEntries;           // 0x08 [O]; [U] loadFile = LOCALE_getstate(data, 2) and loops over it
                                        //      (unsigned); getStringByHash uses it as the last index of the search
    char* mpHashTableHeader;            // 0x0C [O]; [R] inlined ctor 0; [U] loadFile points it at the "LOCT" chunk (0 when
                                        //      missing); getStringByHash returns 0 when it is 0
    tHashEntry* mpHashTableData;        // 0x10 [O]; [U] loadFile points it after the LOCT chunk header and converts the
                                        //      entries; getStringByHash binary-searches it
};

CHECK_OFFSET(cLocaleFile, mLanguage, 0x00);
CHECK_OFFSET(cLocaleFile, unk_01, 0x01);
CHECK_OFFSET(cLocaleFile, mpStringTableData, 0x04);
CHECK_OFFSET(cLocaleFile, mNumEntries, 0x08);
CHECK_OFFSET(cLocaleFile, mpHashTableHeader, 0x0C);
CHECK_OFFSET(cLocaleFile, mpHashTableData, 0x10);
CHECK_SIZE(cLocaleFile, 0x14);

#endif /* UTIL_LOCALEFILE_H */
