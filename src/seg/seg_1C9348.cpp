#include "common.h"
#include "util/huff.h"

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_about);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, int flags, int align) __asm__("cMemMan_alloc");
extern "C" void* memset(void* dst, int c, unsigned int n);
extern char str_CODEXABOUT[];
extern char D_004865D8[] __attribute__((aligned(8)));
extern char D_004A3980[] __attribute__((aligned(8)));
extern char D_004A3988[] __attribute__((aligned(8)));
extern char D_004A3990[] __attribute__((aligned(8)));
extern char D_004865F0[] __attribute__((aligned(8)));

// EA codex "about" block describing the Huffman/RLE codec.
struct sCodexAbout_HUFF {
    int signature;                  // 0x0
    int size;                       // 0x4
    int version;                    // 0x8
    unsigned int b0 : 1;            // 0xC
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int pad : 27;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    char author[32];                // 0x24
    char versionstr[8];             // 0x44
    char shorttypestr[8];           // 0x4C
    char longtypestr[16];           // 0x54
    char desc[32];                  // 0x64
};

/**
 * Returns a newly allocated "CODEXABOUT" block describing EA's Huffman/RLE codec (short name "huf", version
 * 1.03).
 *
 * The caller owns the block.
 *
 * @return the block, or 0 when allocation failed
 */
extern "C" void* HUFF_about(void)
{
    sCodexAbout_HUFF* about = (sCodexAbout_HUFF*)operator new(132, str_CODEXABOUT, 0x20000000, 0);
    if (about != 0) {
        memset(about, 0, 132);
        about->signature = 'HUFF';
        about->size = 132;
        about->version = 100;
        about->b0 = 1;
        about->b1 = 1;
        about->b2 = 1;
        about->b3 = 1;
        about->b4 = 0;
        about->f14 = 0;
        about->f18 = 0;
        about->f1C = 1;
        about->f20 = 0;
        __builtin_memcpy(about->author, D_004865D8, 18);
        __builtin_memcpy(about->versionstr, D_004A3980, 5);
        __builtin_memcpy(about->shorttypestr, D_004A3988, 4);
        __builtin_memcpy(about->longtypestr, D_004A3990, 8);
        __builtin_memcpy(about->desc, D_004865F0, 12);
    }
    return about;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", bxggetm);
#ifdef SKIP_ASM
/**
 * Reads a big-endian unsigned number of @p n bytes.
 *
 * HUFF_is and HUFF_size read the header fields of HUFF-compressed data with it.
 *
 * @param bytes  the first byte
 * @param n  how many bytes to read (0 gives 0)
 * @return the value
 */
extern "C" unsigned bxggetm(unsigned char *bytes, int n) {
    unsigned value = 0;
    while (n--) {
        value = (value << 8) + *bytes++;
    }
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_is);
#ifdef SKIP_ASM
extern "C" unsigned bxggetm(unsigned char *p, int n);

/**
 * Tells whether a buffer starts with a HUFF compression header.
 *
 * Reads the first two bytes (big-endian, bxggetm) and gives 100 for the ids 0x30FB, 0x31FB, 0x32FB, 0x33FB,
 * 0x34FB and 0x35FB, else 0. Nothing in the build calls it.
 *
 * @param compressed  the compressed data
 * @return 100 for a HUFF header, else 0
 */
extern "C" int HUFF_is(unsigned char *compressed) {
    int confidence = 0;
    if (bxggetm(compressed, 2) == 12539
        || bxggetm(compressed, 2) == 12795
        || bxggetm(compressed, 2) == 13051
        || bxggetm(compressed, 2) == 13307
        || bxggetm(compressed, 2) == 13563
        || bxggetm(compressed, 2) == 13819) {
        confidence = 100;
    }
    return confidence;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_size);
#ifdef SKIP_ASM
extern "C" unsigned bxggetm(unsigned char *p, int n);

/**
 * Reads the uncompressed size from a HUFF compression header.
 *
 * Reads the 2-byte id (bxggetm); when bit 0x100 is set the 3-byte big-endian size at offset 5 is read, otherwise
 * the one at offset 2. The C declares no return value, but the code returns the last read (it is left in the return
 * register). Nothing in the build calls it.
 *
 * @param compressed  the compressed data
 */
extern "C" void HUFF_size(unsigned char *compressed) {
    if (bxggetm(compressed, 2) & 0x100) {
        bxggetm(compressed + 5, 3);
    } else {
        bxggetm(compressed + 2, 3);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_decode);
#ifdef SKIP_ASM
extern "C" int HUFF_decompress(int, int);

/**
 * Decompresses HUFF data into a buffer: returns HUFF_decompress(@p packBuf, @p unpackBuf).
 *
 * @p packedSizePtr is not used. The replay code (cReplayFramePtr_loadBlock, cPlaybackMan_setupFile, cReplayCache_load) calls it
 * with 0 there.
 *
 * @param packBuf  the compressed data
 * @param packedSizePtr  not used (callers pass 0)
 * @param unpackBuf  the output buffer
 * @return what HUFF_decompress returns
 */
extern "C" int HUFF_decode(int packBuf, int packedSizePtr, int unpackBuf) {
    return HUFF_decompress(packBuf, unpackBuf);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_deltabytes);
#ifdef SKIP_ASM
/**
 * Delta-encodes a byte buffer for the Huffman compressor: each output byte is the input byte minus the previous
 * one.
 *
 * The first byte is taken relative to 0.
 *
 * @param src  the bytes to encode
 * @param dst  receives the deltas (n bytes)
 * @param n    the number of bytes
 */
extern "C" void HUFF_deltabytes(unsigned char *src, unsigned char *dst, int n) {
    unsigned char *end = src + n;
    int prev = 0;
    if (src < end) {
        do {
            int curByte = *src++;
            *dst++ = curByte - prev;
            prev = curByte;
        } while (src < end);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_writebits);
#ifdef SKIP_ASM


// PORT: the unit declares this with other parameter types; defined under an asm label
extern "C" void HUFF_writebits_impl(HuffEncodeContext* s, HUFFMemStruct* out, unsigned int code, unsigned int len) __asm__("HUFF_writebits");
/**
 * Appends up to 32 bits of a code to the packed output, most significant first, flushing whole bytes.
 *
 * @param encodeCtx  the encoder context (On Tour HuffEncodeContext* EC)
 * @param out  the output buffer (On Tour HUFFMemStruct* dest)
 * @param code  the bits to write, right-aligned
 * @param len  number of bits
 */
extern "C" void HUFF_writebits_impl(HuffEncodeContext* encodeCtx, HUFFMemStruct* out, unsigned int code, unsigned int len)
{
    if (len > 16) {
        HUFF_writebits_impl(encodeCtx, out, code >> 16, len - 16);
        HUFF_writebits_impl(encodeCtx, out, code, 16);
        return;
    }
    encodeCtx->packbits += len;
    encodeCtx->workpattern += (code & encodeCtx->masks[len]) << (24 - encodeCtx->packbits);
    while (encodeCtx->packbits >= 8) {
        ((unsigned char*)out->ptr)[out->len] = (unsigned char)(encodeCtx->workpattern >> 16);
        out->len++;
        encodeCtx->workpattern <<= 8;
        encodeCtx->packbits -= 8;
        (*(int*)&encodeCtx->plen)++;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_treechase);
#ifdef SKIP_ASM
struct HS_86F8 { char pad[0x1524]; int l[0x208]; int r[0x208]; int a[256]; };
/**
 * Walks the Huffman tree from a node and records each leaf's code length.
 *
 * @param i  the node; values below 256 are byte symbols (On Tour node)
 * @param bits  the node's depth
 */
extern "C" void HUFF_treechase(HS_86F8 *self, unsigned i, int bits) {
    if (i < 0x100) {
        self->a[i] = bits;
        return;
    }
    HUFF_treechase(self, self->l[i], bits + 1);
    HUFF_treechase(self, self->r[i], bits + 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_maketree);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct HS_8770 { char pad0[0x100]; unsigned freq[256]; char pad1[0x1524 - 0x500]; int l[0x208]; int r[0x208]; int a[256]; char pad2[0x2DC4 - 0x2964]; int count; };
extern "C" void HUFF_treechase_8770(struct HS_8770 *self, unsigned i, int d) __asm__("HUFF_treechase");
/**
 * Builds the Huffman tree from the byte frequencies by joining the two lightest nodes until one is left, then
 * sets the code lengths.
 *
 * Bytes that never occur get length 99; a single used byte gets length 1.
 */
extern "C" void HUFF_maketree(struct HS_8770 *self) {
    unsigned w[260];
    unsigned id[260];
    unsigned n = 1;
    int node = 256;
    unsigned i;
    w[0] = 0;
    for (i = 0; i < 256; i++) {
        self->a[i] = 99;
        if (self->freq[i]) {
            w[n] = self->freq[i];
            id[n] = i;
            n++;
        }
    }
    self->count = n - 1;
    if (n >= 3) {
        do {
            i = n - 1;
            unsigned mx = w[i];
            int imax = i;
            i--;
            unsigned mn = w[i];
            int imin = i;
            if (mx < mn) {
                mn = mx;
                mx = w[i];
                imin = imax;
                imax = i;
            }
            if (i != 0)
                for (;;) {
                    while (mx < w[--i]) ;
                    if (i == 0)
                        break;
                    mx = w[i];
                    if (mn < mx) {
                        imax = i;
                    } else {
                        mx = mn;
                        imax = imin;
                        mn = w[i];
                        imin = i;
                    }
                }

            unsigned left = id[imax];
            unsigned right = id[imin];
            w[imax] = mx + mn;
            id[imax] = node;
            self->l[node] = left;
            n--;
            w[imin] = w[n];
            self->r[node] = right;
            node++;
            id[imin] = id[n];
        } while (n >= 3);
        HUFF_treechase_8770(self, node - 1, 0);
    } else {
        HUFF_treechase_8770(self, id[n - 1], 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_minrep);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct HS_8970 {
    char pad0[0x100];
    int used[0x311];        // 0x100
    int len[0x608];         // 0xD44
    int a[256];             // 0x2564
    int b[0x11A];           // 0x2964
    int cur;                // 0x2DCC
};
/**
 * Returns the cheapest bit cost of coding a run with the repeat codes up to @p n, or with one clue-coded run
 * length.
 *
 * @param remaining  the run length still to code
 * @param n  the longest repeat code to consider (On Tour r)
 * @return the cost in bits
 */
extern "C" int HUFF_minrep(HS_8970* self, unsigned remaining, unsigned n)
{
    int best;
    if (n != 0) {
        best = HUFF_minrep(self, remaining, n - 1);
        if (self->used[self->cur + n] != 0) {
            unsigned repCount = remaining / n;
            int cost = HUFF_minrep(self, remaining - repCount * n, n - 1) + self->a[self->cur + n] * repCount;
            if (cost < best) {
                best = cost;
            }
        }
    } else {
        best = 0;
        if (remaining != 0) {
            best = 20;
            if (remaining < 0xFC) {
                best = self->a[self->cur] + 3 + self->len[remaining] * 2;
            }
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_writenum);
#ifdef SKIP_ASM

struct HuffOut_8A78;

extern "C" void HUFF_writebits_8A78(HuffEncodeContext* s, HuffOut_8A78* out, unsigned int code, unsigned int len) __asm__("HUFF_writebits");

// PORT: the unit declares this with other parameter types; defined under an asm label
extern "C" void HUFF_writenum_impl(HuffEncodeContext* s, HuffOut_8A78* out, unsigned int v) __asm__("HUFF_writenum");
/**
 * Writes a number to the Huffman bit stream as a size prefix followed by the offset within that size class.
 *
 * For values below 0xFC the bit count and base come from the context's tables (+0xD44 and +0x1134, one entry per
 * value); larger values fall into fixed classes: 6 bits from 0xFC, 7 from 0x1FC, ... 16 from 0x3FFFC, 17 from 0x80000
 * and 18 from 0x100000. It then calls HUFF_writebits twice: the value 1 in bits + 1 bits (the prefix) and @p num minus
 * the base in bits + 2 bits. Called by HUFF_pack and by HUFF_writeexp.
 *
 * @param encodeContext    the encoder context (On Tour HuffEncodeContext)
 * @param out  the output stream (HUFFMemStruct)
 * @param num    the number to write
 */
extern "C" void HUFF_writenum_impl(HuffEncodeContext* encodeContext, HuffOut_8A78* out, unsigned int num)
{
    unsigned int base;
    unsigned int bits;
    if (num < 0xFC) {
        bits = encodeContext->repbits[num];
        base = encodeContext->repbase[num];
    } else if (num < 0x1FC) {
        bits = 6;
        base = 0xFC;
    } else if (num < 0x3FC) {
        bits = 7;
        base = 0x1FC;
    } else if (num < 0x7FC) {
        bits = 8;
        base = 0x3FC;
    } else if (num < 0xFFC) {
        bits = 9;
        base = 0x7FC;
    } else if (num < 0x1FFC) {
        bits = 10;
        base = 0xFFC;
    } else if (num < 0x3FFC) {
        bits = 11;
        base = 0x1FFC;
    } else if (num < 0x7FFC) {
        bits = 12;
        base = 0x3FFC;
    } else if (num < 0xFFFC) {
        bits = 13;
        base = 0x7FFC;
    } else if (num < 0x1FFFC) {
        bits = 14;
        base = 0xFFFC;
    } else if (num < 0x3FFFC) {
        bits = 15;
        base = 0x1FFFC;
    } else if (num < 0x80000) {
        bits = 16;
        base = 0x3FFFC;
    } else if (num < 0x100000) {
        bits = 17;
        base = 0x80000;
    } else {
        bits = 18;
        base = 0x100000;
    }
    HUFF_writebits_8A78(encodeContext, out, 1, bits + 1);
    HUFF_writebits_8A78(encodeContext, out, num - base, bits + 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1C9348", HUFF_writeexp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct HS_8C30 { char pad[0x2564]; int a[256]; int b[0x11A]; int cur; };
extern void HUFF_writebits_8C30(HS_8C30 *, int, int, int) __asm__("HUFF_writebits");
extern "C" void HUFF_writenum(HS_8C30 *, int, int);

extern "C" void HUFF_writeexp_impl(HS_8C30 *self, int x, int y) __asm__("HUFF_writeexp");
/**
 * Writes an escaped symbol: the escape code, a zero count, then the symbol itself as a raw 9-bit value.
 *
 * The escape symbol is the context's +0x2DCC entry; its bit pattern comes from the code table (+0x2964) and its length
 * from the length table (+0x2564), written with HUFF_writebits. Then HUFF_writenum writes 0 and
 * HUFF_writebits writes @p y in 9 bits.
 *
 * @param x  the output stream (HUFFMemStruct)
 * @param y  the symbol to write
 */
extern "C" void HUFF_writeexp_impl(HS_8C30 *self, int x, int y) {
    HUFF_writebits_8C30(self, x, self->b[self->cur], self->a[self->cur]);
    HUFF_writenum(self, x, 0);
    HUFF_writebits_8C30(self, x, y, 9);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1C9348", HUFF_writecode);
#ifdef SKIP_ASM
struct HS_8CA8 { char pad[0x2564]; int a[256]; int b[0x11A]; int cur; };
extern "C" void HUFF_writebits(HS_8CA8 *, int, int, int);
extern "C" void HUFF_writeexp(HS_8CA8 *, int, int);

/**
 * Writes one symbol's Huffman code to the bit stream.
 *
 * If @p idx is the escape symbol (+0x2DCC) it goes through HUFF_writeexp; otherwise HUFF_writebits writes
 * its bit pattern (code table +0x2964) with its length (length table +0x2564).
 *
 * @param x    the output stream (HUFFMemStruct)
 * @param idx  the symbol to write
 */
extern "C" void HUFF_writecode(HS_8CA8 *self, int x, int idx) {
    if (idx == self->cur) {
        HUFF_writeexp(self, x, idx);
        return;
    }
    HUFF_writebits(self, x, self->b[idx], self->a[idx]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_init);
#ifdef SKIP_ASM
struct HuffState_8CF8 {
    char pad0[0xD44];
    unsigned int len[252];      // 0xD44
    unsigned int base[252];     // 0x1134
};

/**
 * Fills the Huffman encoder's length and base tables for byte values 0 to 251.
 *
 * Values are grouped in runs of 4, 8, 16, 32, 64 and 128: each run gets the extra-bit count 0 to 5 in len[] and
 * its first value (0, 4, 12, 28, 60, 124) in base[].
 *
 * @param EC  the encoder state (On Tour HuffEncodeContext*)
 */
extern "C" void HUFF_init(HuffState_8CF8* EC)
{
    unsigned int i;
    for (i = 0; i < 4; i++) {
        EC->len[i] = 0;
        EC->base[i] = 0;
    }
    for (; i < 12; i++) {
        EC->len[i] = 1;
        EC->base[i] = 4;
    }
    for (; i < 28; i++) {
        EC->len[i] = 2;
        EC->base[i] = 12;
    }
    for (; i < 60; i++) {
        EC->len[i] = 3;
        EC->base[i] = 28;
    }
    for (; i < 124; i++) {
        EC->len[i] = 4;
        EC->base[i] = 60;
    }
    for (; i < 252; i++) {
        EC->len[i] = 5;
        EC->base[i] = 124;
    }
}
#endif

INCLUDE_ASM("seg/seg_1C9348", HUFF_analysis);

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_pack);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct HS_9A48 {
    signed char used[256];          // 0x0000
    int clue[0x300];                // 0x0100: nonzero = slot usable as a run clue
    unsigned int dlen[17];          // 0x0D00 (1-based)
    unsigned int len[252];          // 0x0D44
    char pad_0x1134[0x2564 - 0x1134];
    unsigned int bitnum[256];       // 0x2564
    unsigned int code[0x112];       // 0x2964
};


extern "C" void huffBits_9A48(void* s, void* out, unsigned int code, unsigned int len) __asm__("HUFF_writebits");
extern "C" void huffNum_9A48(void* s, void* out, unsigned int v) __asm__("HUFF_writenum");
extern "C" void huffSym_9A48(void* s, void* out, unsigned int sym) __asm__("HUFF_writecode");

/**
 * Writes the packed stream: code table, symbol order, then the data as literals, clue-coded runs and byte
 * deltas, then an end code.
 *
 * Each run is coded the cheapest of three ways (literal repeats, a clue code and length, or repeat codes).
 *
 * @param vs  the encoder context (On Tour HuffEncodeContext* EC)
 * @param out  the output buffer (On Tour HUFFMemStruct* dest)
 * @param opt  packing options (On Tour opt)
 */
// Huffman pack: writes the clue byte, code-length counts and the move-to-front symbol order, then
// codes src..srcend with literals, clue-coded runs and delta codes, and ends with an end code.
// Locals are reused the way the original does (run doubles as the MTF cursor, last holds the order
// value, k is also the "coded as delta" flag, d becomes the delta code index); the register
// allocation depends on that reuse.
extern "C" void HUFF_pack(void* vs, void* out, int opt)
{
    HS_9A48* encoder = (HS_9A48*)vs;
    HuffEncodeContext* encoderInfo = (HuffEncodeContext*)vs;
    unsigned int i;
    unsigned int k;
    unsigned int repCount;
    unsigned int rem;
    unsigned int run;
    unsigned int last;
    unsigned int mode;
    unsigned char* srcPtr;

    huffBits_9A48(encoder, out, encoderInfo->clue, 8);
    for (i = 1; i <= encoderInfo->mostbits; i++)
        huffNum_9A48(encoder, out, encoder->dlen[i]);
    for (i = 0; i < 256; i++)
        encoder->used[i] = 0;
    run = 0xFF;
    for (i = 0; i < encoderInfo->codes; i++) {
        int n = -1;
        last = encoderInfo->sortptr[i];
        do {
            run = (run + 1) & 0xFF;
            if (!encoder->used[run])
                n++;
        } while (last != run);
        encoder->used[run] = 1;
        huffNum_9A48(encoder, out, n);
    }
    mode = encoderInfo->clues;
    if (mode == 0)
        encoderInfo->clue = 32000;
    srcPtr = encoderInfo->buffer;
    last = 0x100;
    while (srcPtr < encoderInfo->bufptr) {
        i = *srcPtr++;
        if (i == last) {
            unsigned char* lim = srcPtr + 30000;
            if (encoderInfo->bufptr < lim)
                lim = encoderInfo->bufptr;
            run = 0;
            mode = encoderInfo->clues;
            if (srcPtr < lim) {
                do {
                    i = *srcPtr++;
                    run++;
                } while (i == last && srcPtr < lim);
            }
            unsigned int cost1 = run * encoder->bitnum[last];
            unsigned int cost2 = 32000;
            unsigned int cost3 = 32000;
            if (mode != 0) {
                if (encoder->clue[encoderInfo->clue] != 0) {
                    cost2 = 20;
                    if (run < 252)
                        cost2 = encoder->bitnum[encoderInfo->clue] + 3 + encoder->len[run] * 2;
                }
                if (mode >= 2) {
                    rem = run;
                    cost3 = 0;
                    for (k = mode - 1; k != 0; k--) {
                        if (encoder->clue[encoderInfo->clue + k] != 0) {
                            repCount = rem / k;
                            rem -= repCount * k;
                            cost3 += repCount * encoder->bitnum[encoderInfo->clue + k];
                        }
                    }
                    if (rem != 0)
                        cost3 = 32000;
                }
            }
            if (cost1 <= cost2 && cost1 <= cost3) {
                while (run--)
                    huffSym_9A48(encoder, out, last);
            } else if (cost2 < cost3) {
                huffBits_9A48(encoder, out, encoder->code[encoderInfo->clue], encoder->bitnum[encoderInfo->clue]);
                huffNum_9A48(encoder, out, run);
            } else {
                rem = run;
                for (k = mode - 1; k != 0; k--) {
                    if (encoder->clue[encoderInfo->clue + k] != 0) {
                        repCount = rem / k;
                        rem -= repCount * k;
                        while (repCount--)
                            huffSym_9A48(encoder, out, encoderInfo->clue + k);
                    }
                }
            }
        }
        k = 0;
        if (((int)encoderInfo->dclues) != 0) {
            int diff = i - last;
            if (diff <= encoderInfo->maxdelta && diff >= encoderInfo->mindelta) {
                diff = (diff - 1) * 2 + encoderInfo->dclue;
                if (i < last)
                    diff = ((last - i) - 1) * 2 + encoderInfo->dclue + 1;
                if (encoder->bitnum[diff] < encoder->bitnum[i]) {
                    huffBits_9A48(encoder, out, encoder->code[diff], encoder->bitnum[diff]);
                    k = 1;
                }
            }
        }
        last = i;
        if (!k)
            huffSym_9A48(encoder, out, i);
    }
    huffBits_9A48(encoder, out, encoder->code[encoderInfo->clue], encoder->bitnum[encoderInfo->clue]);
    huffNum_9A48(encoder, out, 0);
    huffBits_9A48(encoder, out, 2, 2);
    huffBits_9A48(encoder, out, 0, 7);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1C9348", HUFF_packfile);
#ifdef SKIP_ASM


extern "C" void HUFF_init_9F60(HuffEncodeContext* s) __asm__("HUFF_init");
extern "C" void HUFF_writebits_9F60(HuffEncodeContext* s, HUFFMemStruct* out, unsigned int code, unsigned int len) __asm__("HUFF_writebits");
extern "C" void HUFF_analysis(void* s, int a, int b);
extern "C" void HUFF_pack(void* s, void* out, int a);

/**
 * Packs a buffer with a Huffman header: 0x30FB-0x35FB by delta mode, plus the sizes.
 *
 * The odd header forms (0x31FB...) also store @p len when it differs from the input length.
 *
 * @param EC  the encoder context
 * @param in  the input (On Tour HUFFMemStruct* infile)
 * @param out  the output; its length is set to the packed size
 * @param len  the original (unpacked) length
 * @param mode  delta mode: 0 none, 1 or 2 delta passes
 * @return the packed size in bytes
 */
extern "C" int HUFF_packfile(HuffEncodeContext* EC, HUFFMemStruct* in, HUFFMemStruct* out, int len, int mode)
{
    unsigned int i;
    unsigned int code;
    EC->packbits = 0;
    EC->workpattern = 0;
    EC->masks[0] = 0;
    for (i = 1; i < 17; i++) {
        EC->masks[i] = EC->masks[i - 1] * 2 + 1;
    }
    HUFF_init_9F60(EC);
    EC->buffer = (unsigned char*)in->ptr;
    {
        int n = in->len;
        EC->flen = n;
        EC->ulen = n;
        EC->bufptr = EC->buffer + n;
    }
    out->len = 0;
    EC->packbits = 0;
    EC->workpattern = 0;
    EC->plen = 0;
    HUFF_analysis(EC, 57, 0xF);
    if (len == in->len) {
        code = 0;
        if (mode == 0) code = 12539;
        else if (mode == 1) code = 13051;
        else if (mode == 2) code = 13563;
        HUFF_writebits_9F60(EC, out, code, 16);
        HUFF_writebits_9F60(EC, out, in->len, 24);
    } else {
        code = 0;
        if (mode == 0) code = 12795;
        else if (mode == 1) code = 13307;
        else if (mode == 2) code = 13819;
        HUFF_writebits_9F60(EC, out, code, 16);
        HUFF_writebits_9F60(EC, out, len, 24);
        HUFF_writebits_9F60(EC, out, in->len, 24);
    }
    HUFF_pack(EC, out, 57);
    return out->len;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1C9348", HUFF_encode);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* name, int flags, int align);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void HUFF_deltabytes(unsigned char *src, unsigned char *dst, int n);
extern "C" int HUFF_packfile_A100(void* s, void* in, void* out, int len, int mode) __asm__("HUFF_packfile");
extern char str_HuffEncodeContext[];
extern char str_huffdeltadata[];


/**
 * Huffman-packs a buffer into @p dst, after 0, 1 or 2 byte-delta passes ("huffdeltadata") chosen by the
 * options.
 *
 * Allocates and frees its own "HuffEncodeContext". The output's capacity is not checked.
 *
 * @param src  the data to pack
 * @param len  its length in bytes
 * @param dst  the output buffer
 * @param opts  the delta mode in its first int, or 0 for none
 * @return the packed size, or 0 when the context could not be allocated
 */
extern "C" int HUFF_encode(void* src, int len, void* dst, int* opts)
{
    void* deltaBuf = 0;
    int mode = 0;
    int result = 0;
    void* encodeCtx;
    if (opts) mode = *opts;
    encodeCtx = cMemMan_alloc(12780, str_HuffEncodeContext, 0x20000000, 0);
    if (encodeCtx) {
        HUFFMemStruct in;
        HUFFMemStruct out;
        switch (mode) {
        case 1:
            deltaBuf = operator_new_tag(len, str_huffdeltadata, 0x20000000, 0);
            HUFF_deltabytes((unsigned char*)src, (unsigned char*)deltaBuf, len);
            (*(void**)&in.ptr) = deltaBuf;
            break;
        case 2:
            deltaBuf = operator_new_tag(len, str_huffdeltadata, 0x20000000, 0);
            HUFF_deltabytes((unsigned char*)src, (unsigned char*)deltaBuf, len);
            HUFF_deltabytes((unsigned char*)deltaBuf, (unsigned char*)deltaBuf, len);
            (*(void**)&in.ptr) = deltaBuf;
            break;
        case 0:
        default:
            (*(void**)&in.ptr) = src;
            break;
        }
        in.len = len;
        (*(void**)&out.ptr) = dst;
        out.len = len;
        result = HUFF_packfile_A100(encodeCtx, &in, &out, len, mode);
        if (deltaBuf) cMemMan_free(deltaBuf);
        operator_delete((int*)encodeCtx);
    }
    return result;
}
#endif
