#ifndef UTIL_HUFF_H
#define UTIL_HUFF_H

// EA's Huffman / RLE packer (the "Huffman RLE" compressor that writes the game's packed blobs; HUFF_encode and its
// helpers HUFF_init / HUFF_analysis / HUFF_maketree / HUFF_treechase / HUFF_minrep / HUFF_writebits / HUFF_writenum /
// HUFF_writeexp / HUFF_writecode, retail unit seg/seg_1C9348) (views group D: HS_86F8, HS_8770, HS_8970, HS_8C30,
// HS_9A48, HS2_9A48, HuffState_8600, HuffState_8A78, HuffState_9F60 -> HuffEncodeContext; HuffBuf_A100, HuffBuf_9F60,
// HuffOut_8600, HuffOut_8A78 -> HUFFMemStruct).
// EA source: game/source/util (On Tour: util/all_in_one_util.cpp holds the HUFF_* functions).
// Tags: [O] SSX On Tour's DWARF (struct names, members, offsets; the paired functions HUFF_treechase(HuffEncodeContext*
// EC, unsigned node, unsigned bits), HUFF_maketree(HuffEncodeContext*), HUFF_minrep(HuffEncodeContext*, unsigned,
// unsigned), HUFF_writebits(HuffEncodeContext*, HUFFMemStruct* dest, unsigned bitpattern, unsigned len),
// HUFF_writeexp / writenum / writecode(HuffEncodeContext*, HUFFMemStruct*, unsigned)); [R] retail HUFF_encode allocates
// the context as 12780 = 0x31EC bytes tagged "HuffEncodeContext" (the string is in the retail ELF); [U] retail use.
// Some retail views index across member boundaries (HUFF_minrep's used[0x311] from count[] into bitnum[]); those
// accesses stay array arithmetic on the first member.

#include "layout.h"

struct HuffEncodeContext {
    char qleapcode[256];                // 0x0000 [O]; [U] HUFF_* (views HS_9A48 used / HuffState_*)
    unsigned int count[768];            // 0x0100 [O]; [U] HUFF_maketree / HUFF_minrep (HS_8770 freq, HS_8970 used[] spans count + bitnum)
    unsigned int bitnum[17];            // 0x0D00 [O]
    unsigned int repbits[252];          // 0x0D44 [O]; [U] HuffState_8A78 len
    unsigned int repbase[252];          // 0x1134 [O]; [U] HuffState_8A78 base
    unsigned int tree_left[520];        // 0x1524 [O]; [U] HUFF_treechase (HS_86F8 l)
    unsigned int tree_right[520];       // 0x1D44 [O]; [U] HUFF_treechase (HS_86F8 r)
    unsigned int bitsarray[256];        // 0x2564 [O]; [U] HUFF_treechase / HUFF_writeexp (HS_86F8 a, HS_8C30 a)
    unsigned int patternarray[256];     // 0x2964 [O]; [U] HUFF_writeexp (HS_8C30 b[])
    unsigned int masks[17];             // 0x2D64 [O]; [U] HUFF_writebits (HuffState_8600 / _9F60 mask)
    unsigned int packbits;              // 0x2DA8 [O]; [U] HUFF_writebits (bitcount)
    unsigned int workpattern;           // 0x2DAC [O]
    unsigned char* buffer;              // 0x2DB0 [O]; [U] HS2_9A48 src
    unsigned char* bufptr;              // 0x2DB4 [O]; [U] HS2_9A48 srcend
    int flen;                           // 0x2DB8 [O]
    unsigned int csum;                  // 0x2DBC [O]
    unsigned int mostbits;              // 0x2DC0 [O]
    unsigned int codes;                 // 0x2DC4 [O]; [U] HUFF_maketree (HS_8770 count)
    unsigned int chainused;             // 0x2DC8 [O]
    unsigned int clue;                  // 0x2DCC [O]; [U] HUFF_writeexp (HS_8C30 cur)
    unsigned int dclue;                 // 0x2DD0 [O]
    unsigned int clues;                 // 0x2DD4 [O]
    unsigned int dclues;                // 0x2DD8 [O]
    int mindelta;                       // 0x2DDC [O]
    int maxdelta;                       // 0x2DE0 [O]
    unsigned int plen;                  // 0x2DE4 [O]
    unsigned int ulen;                  // 0x2DE8 [O]
    unsigned int sortptr[256];          // 0x2DEC [O]
};

CHECK_OFFSET(HuffEncodeContext, qleapcode, 0x0000);
CHECK_OFFSET(HuffEncodeContext, count, 0x0100);
CHECK_OFFSET(HuffEncodeContext, bitnum, 0x0D00);
CHECK_OFFSET(HuffEncodeContext, repbits, 0x0D44);
CHECK_OFFSET(HuffEncodeContext, repbase, 0x1134);
CHECK_OFFSET(HuffEncodeContext, tree_left, 0x1524);
CHECK_OFFSET(HuffEncodeContext, tree_right, 0x1D44);
CHECK_OFFSET(HuffEncodeContext, bitsarray, 0x2564);
CHECK_OFFSET(HuffEncodeContext, patternarray, 0x2964);
CHECK_OFFSET(HuffEncodeContext, masks, 0x2D64);
CHECK_OFFSET(HuffEncodeContext, packbits, 0x2DA8);
CHECK_OFFSET(HuffEncodeContext, workpattern, 0x2DAC);
CHECK_OFFSET(HuffEncodeContext, buffer, 0x2DB0);
CHECK_OFFSET(HuffEncodeContext, bufptr, 0x2DB4);
CHECK_OFFSET(HuffEncodeContext, flen, 0x2DB8);
CHECK_OFFSET(HuffEncodeContext, csum, 0x2DBC);
CHECK_OFFSET(HuffEncodeContext, mostbits, 0x2DC0);
CHECK_OFFSET(HuffEncodeContext, codes, 0x2DC4);
CHECK_OFFSET(HuffEncodeContext, chainused, 0x2DC8);
CHECK_OFFSET(HuffEncodeContext, clue, 0x2DCC);
CHECK_OFFSET(HuffEncodeContext, dclue, 0x2DD0);
CHECK_OFFSET(HuffEncodeContext, clues, 0x2DD4);
CHECK_OFFSET(HuffEncodeContext, dclues, 0x2DD8);
CHECK_OFFSET(HuffEncodeContext, mindelta, 0x2DDC);
CHECK_OFFSET(HuffEncodeContext, maxdelta, 0x2DE0);
CHECK_OFFSET(HuffEncodeContext, plen, 0x2DE4);
CHECK_OFFSET(HuffEncodeContext, ulen, 0x2DE8);
CHECK_OFFSET(HuffEncodeContext, sortptr, 0x2DEC);
CHECK_SIZE(HuffEncodeContext, 0x31EC);

// [O] struct HUFFMemStruct {char* ptr; int len;} (0x8): the output cursor the HUFF_write* helpers take; [U] HUFF_encode
// builds one on its stack ({buffer, length}); the writers advance ptr and count len.
struct HUFFMemStruct {
    char* ptr;                          // 0x0 [O]; [U] the write pointer
    int len;                            // 0x4 [O]; [U] the bytes written / the buffer length
};

CHECK_OFFSET(HUFFMemStruct, ptr, 0x0);
CHECK_OFFSET(HUFFMemStruct, len, 0x4);
CHECK_SIZE(HUFFMemStruct, 0x8);

#endif /* UTIL_HUFF_H */
