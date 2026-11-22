#ifndef EALIB_SBALLOC_H
#define EALIB_SBALLOC_H

// BANKVER5: the header of a loaded EA SND sound bank (version 5), as SNDbankadd receives it and the bank functions read
// it. EA source: snd/source/library/cmn/sballoc.c (SSX On Tour: snd 9.06.03-psp; SNDBANKI_getppatch(BANKVER5* pb, int
// patnum)). Retail code: unit ealib/seg_2B4578 (SNDBANKI_getppatch, SNDbankadd; SNDbankadd's view Hdr_6098 is this
// header).
//
// Names, member order and offsets: [O] On Tour struct BANKVER5 (0x814); [U] retail SNDBANKI_getppatch bounds patnum by
// numpatches (lhu 0x6) and reads patch[patnum] (0x14 + 4 * i); SNDbankadd reads hdrsize 0x8, spusize 0xC, iopcpusize
// 0x10 and the same table. The patch table's 512 entries are On Tour's (the file carries numpatches of them).
// C struct. Tags: [O] SSX On Tour's DWARF; [U] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct BANKVER5 BANKVER5;
#endif

struct BANKVER5 {
    int id;                         // 0x000 [O] (the bank's FourCC)
    unsigned char ver;              // 0x004 [O]
    char resolved;                  // 0x005 [O] (patch offsets turned into pointers)
    unsigned short numpatches;      // 0x006 [O]; [U] SNDBANKI_getppatch's bound, SNDbankadd (lhu)
    int hdrsize;                    // 0x008 [O]; [U] SNDbankadd
    int spusize;                    // 0x00C [O]; [U] SNDbankadd (the sound-memory section size)
    int iopcpusize;                 // 0x010 [O]; [U] SNDbankadd (the IOP / CPU section size)
    void* patch[512];               // 0x014 [O] TAGGEDPATCH* patch[512]; [U] SNDBANKI_getppatch returns patch[patnum],
                                    //       SNDbankadd walks the table
};

CHECK_OFFSET(BANKVER5, id, 0x000);
CHECK_OFFSET(BANKVER5, ver, 0x004);
CHECK_OFFSET(BANKVER5, resolved, 0x005);
CHECK_OFFSET(BANKVER5, numpatches, 0x006);
CHECK_OFFSET(BANKVER5, hdrsize, 0x008);
CHECK_OFFSET(BANKVER5, spusize, 0x00C);
CHECK_OFFSET(BANKVER5, iopcpusize, 0x010);
CHECK_OFFSET(BANKVER5, patch, 0x014);
CHECK_SIZE(BANKVER5, 0x814);

#endif /* EALIB_SBALLOC_H */
