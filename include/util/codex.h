#ifndef UTIL_CODEX_H
#define UTIL_CODEX_H

// EA's codex "about" block: every EA compression codex (HUFF, REF, BTREE ...) has an <codec>_about() that allocates and
// fills one to describe itself (signature, capabilities, memory needs, name strings). Retail: HUFF_about (unit
// seg/seg_1C9348; the unit's view sCodexAbout_HUFF).
//
// Size 0x84 [R]: HUFF_about allocates 132 bytes tagged "CODEXABOUT", clears them and stores 132 in size. Member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O] struct CODEXABOUT, 0x84, the same layout). Evidence
// tags: [O] On Tour's DWARF; [R] HUFF_about (signature 'HUFF', size 132, version 100, the bits and the strings it
// copies: authorstr 18 bytes, versionstr 5 ("1.03"), shorttypestr 4 ("huf"), wordtypestr 8, longtypestr 12).

#include "layout.h"

#ifndef __cplusplus
typedef struct CODEXABOUT CODEXABOUT;
#endif

struct CODEXABOUT {
    int signature;                  // 0x00 [O]; [R] HUFF_about: 'HUFF'
    int size;                       // 0x04 [O]; [R] HUFF_about: 132 (this record's size)
    int version;                    // 0x08 [O]; [R] HUFF_about: 100
    unsigned int decode : 1;        // 0x0C bit 0 [O]; [R] HUFF_about: 1
    unsigned int encode : 1;        // 0x0C bit 1 [O]; [R] HUFF_about: 1
    unsigned int lossless : 1;      // 0x0C bit 2 [O]; [R] HUFF_about: 1
    unsigned int overlap : 1;       // 0x0C bit 3 [O]; [R] HUFF_about: 1
    unsigned int external : 1;      // 0x0C bit 4 [O]; [R] HUFF_about: 0
    unsigned int pad : 12;          // 0x0C bits 5-16 [O]
    unsigned int pad2 : 16;         // 0x10 bits 0-15 [O] (does not fit the first word, so it starts the next)
    int encodememory;               // 0x14 [O]; [R] HUFF_about: 0
    int decodememory;               // 0x18 [O]; [R] HUFF_about: 0
    int encodebuffers;              // 0x1C [O]; [R] HUFF_about: 1
    int decodebuffers;              // 0x20 [O]; [R] HUFF_about: 0
    char authorstr[32];             // 0x24 [O]; [R] HUFF_about copies 18 bytes
    char versionstr[8];             // 0x44 [O]; [R] HUFF_about copies 5 bytes
    char shorttypestr[8];           // 0x4C [O]; [R] HUFF_about copies 4 bytes
    char wordtypestr[16];           // 0x54 [O]; [R] HUFF_about copies 8 bytes (the view's longtypestr)
    char longtypestr[32];           // 0x64 [O]; [R] HUFF_about copies 12 bytes (the view's desc)
};

CHECK_OFFSET(CODEXABOUT, signature, 0x00);
// (size at 0x04: CHECK_OFFSET(CODEXABOUT, size, ...) would declare check_CODEXABOUT_size, the CHECK_SIZE typedef's name)
CHECK_OFFSET(CODEXABOUT, version, 0x08);
CHECK_OFFSET(CODEXABOUT, encodememory, 0x14);
CHECK_OFFSET(CODEXABOUT, decodememory, 0x18);
CHECK_OFFSET(CODEXABOUT, encodebuffers, 0x1C);
CHECK_OFFSET(CODEXABOUT, decodebuffers, 0x20);
CHECK_OFFSET(CODEXABOUT, authorstr, 0x24);
CHECK_OFFSET(CODEXABOUT, versionstr, 0x44);
CHECK_OFFSET(CODEXABOUT, shorttypestr, 0x4C);
CHECK_OFFSET(CODEXABOUT, wordtypestr, 0x54);
CHECK_OFFSET(CODEXABOUT, longtypestr, 0x64);
CHECK_SIZE(CODEXABOUT, 0x84);

#endif /* UTIL_CODEX_H */
