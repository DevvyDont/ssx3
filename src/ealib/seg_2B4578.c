#include "common.h"
#include "ealib/sballoc.h"
#include "ealib/eaxadecf.h"
#include "ealib/shape.h"
#include "ealib/sst.h"
#include "ealib/mtdecf.h"
#include "ealib/sfilter.h"
#include "ealib/snd.h"
//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_unpack);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct SHAPE_3578 {
    int w0;
    short w;
    short h;
    int x8;
    unsigned int wC;
} SHAPE_3578;
#define TYPE_3578(s) (*(unsigned char *)(s))
#define NEXT_3578(s) (((s)->w0 >> 8) ? (SHAPE_3578 *)((char *)(s) + ((s)->w0 >> 8)) : 0)
extern void *(*SHAPE_memalloc_3578)(char *, int, int) __asm__("SHAPE_memalloc");
extern char D_004956E0_3578[] __asm__("D_004956E0");
extern char D_004956E8_3578[] __asm__("D_004956E8");
extern int f_4478_3578(SHAPE_3578 *) __asm__("SHAPE_clonesize");
extern int f_3BB0_3578(SHAPE_3578 *) __asm__("SHAPE_size");
extern void memcpy_3578(void *, void *, int) __asm__("MEM_copy");
// unpack returns unpackz's decoded length (int).
extern int f_47F8_3578(void *, void *) __asm__("unpack");
extern SHAPE_3578 *f_4690_3578(SHAPE_3578 *) __asm__("SHAPE_clone");
extern char *f_4010_3578(SHAPE_3578 *, int, int, int) __asm__("SHAPE_getpixeladr");
extern void f_33B0_3578(char *, char *, int, int, int) __asm__("undxtpack");
extern void SHAPE_cloneat_3578(void *, SHAPE_3578 *) __asm__("SHAPE_cloneat");

/**
 * Returns a usable copy of a SHAPE image: decompressed when packed, decoded to 32 bpp when DXT, otherwise a
 * plain clone.
 *
 * Packed shapes (type bit 0x80) unpack into an "unShp" allocation; DXT1/3/5 (types 0x60-0x62) decode every
 * mipmap into an "undxt" allocation with the size rounded up to a multiple of 4. Attached blocks (palette,
 * name...)
 * are copied after the image. The original is left alone.
 *
 * @param shape  the shape, or 0
 * @return the new shape, or 0 for a null input
 */
SHAPE_3578 *SHAPE_unpack(SHAPE_3578 *shape) {
    SHAPE_3578 hdr;
    SHAPE_3578 *old;
    int clonelen;
    int size;
    int fmt;
    int w;
    int alignedHeight;
    int w2;
    int mipHeight;
    short rw;
    short rh;
    int i;
    char *srcPixels;

    old = shape;
    clonelen = 0;
    size = 0;
    if (shape == 0) {
        return 0;
    }
    if (NEXT_3578(shape)) {
        clonelen = f_4478_3578(NEXT_3578(shape));
    }
    if (TYPE_3578(shape) & 0x80) {
        hdr = *old;
        TYPE_3578(&hdr) &= 0x7F;
        size = f_3BB0_3578(&hdr);
        shape = SHAPE_memalloc_3578(D_004956E0_3578, size + clonelen, 0);
        memcpy_3578(shape, old, 16);
        f_47F8_3578(old + 1, shape + 1);
        TYPE_3578(shape) &= 0x7F;
    } else {
        fmt = 0;
        if (TYPE_3578(shape) == 0x60) {
            fmt = 1;
        } else if (TYPE_3578(shape) == 97) {
            fmt = 3;
        } else if (TYPE_3578(shape) == 98) {
            fmt = 5;
        }
        if (fmt) {
            hdr = *old;
            rw = hdr.w % 4;
            rh = hdr.h % 4;
            w = hdr.w;
            alignedHeight = hdr.h;
            if (rw) {
                w += 4 - rw;
            }
            if (rh) {
                alignedHeight += 4 - rh;
            }
            hdr.w = w;
            hdr.h = alignedHeight;
            TYPE_3578(&hdr) = 5;
            size = f_3BB0_3578(&hdr);
            shape = SHAPE_memalloc_3578(D_004956E8_3578, size + clonelen, 0);
            memcpy_3578(shape, old, 16);
            TYPE_3578(shape) = 5;
            shape->w = w;
            shape->h = alignedHeight;
            for (i = 0, w2 = w, mipHeight = alignedHeight; i < (int)(shape->wC >> 28) + 1; i++) {
                srcPixels = f_4010_3578(old, 0, 0, i);
                f_33B0_3578(srcPixels, f_4010_3578(shape, 0, 0, i), w2, mipHeight, fmt);
                if (w2 >= 2) {
                    w2 >>= 1;
                }
                if (mipHeight >= 2) {
                    mipHeight >>= 1;
                }
            }
        } else {
            clonelen = 0;
            shape = f_4690_3578(shape);
        }
    }
    if (clonelen) {
        shape->w0 = TYPE_3578(shape) | (size << 8);
        SHAPE_cloneat_3578((char *)shape + size, NEXT_3578(old));
    }
    return shape;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_cluttype);
#ifdef SKIP_ASM
/**
 * Maps a palette's bits per pixel to the shape library's CLUT type code.
 *
 * 15 and 16 bpp give 0x20, 32 bpp gives 0x21, and 18, 24 and anything else give 0.
 *
 * @param bpp  the palette's bits per pixel
 * @return the CLUT type code
 */
int SHAPE_cluttype(int bpp) {
    int var_5;

    var_5 = 0;
    switch (bpp) {
    case 15:
    case 16:
        var_5 = 0x20;
        break;
    case 18:
    case 24:
        var_5 = 0;
        break;
    case 32:
        var_5 = 33;
        break;
    }
    return var_5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_locatez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *f_3A10_3900(char *) __asm__("SHAPE_longname");
extern void f_3D00_3900(char *, int, void *) __asm__("SHAPE_name");
extern int f_65A8_3900(void *, int) __asm__("strcmp");
struct B_3900 { int pad; char c; };
struct H_3900 { char pad[8]; int n; char pad2[8]; struct { int off; int x; } t[1]; };

char *SHAPE_locatez_3900(struct H_3900 *, int) __asm__("SHAPE_locatez");
/**
 * Finds a shape in a shape file (FSH) by name: its long name when it has one, else its directory name.
 *
 * @param shpfile  the shape file (On Tour const void* shpfile)
 * @param name  the name to find (const char*, passed as int)
 * @return the shape, or 0 when not found
 */
char *SHAPE_locatez_3900(struct H_3900 *shpfile, int name) {
    int i;
    int n = shpfile->n;
    struct B_3900 buf;
    char *shapeName;
    for (i = 0; i < n; i++) {
        shapeName = f_3A10_3900((char *)shpfile + shpfile->t[i].off);
        if (shapeName == 0) {
            buf.c = 0;
            f_3D00_3900((char *)shpfile, i, &buf);
            shapeName = (char *)&buf;
        }
        if (f_65A8_3900(shapeName, name) == 0)
            return (char *)shpfile + shpfile->t[i].off;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_locate);
#ifdef SKIP_ASM
extern void SHAPE_locatez();

/**
 * Thin wrapper that calls SHAPE_locatez().
 *
 * Declared without parameters: the caller's arguments reach SHAPE_locatez() unchanged in their registers.
 */
void SHAPE_locate(void) {
    SHAPE_locatez();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_depth);
#ifdef SKIP_ASM
extern unsigned char shapetypetobpp[];

/**
 * Returns the bits per pixel of a shape from its type byte.
 *
 * Looks up the low 7 bits of the shape header's first byte in shapetypetobpp.
 *
 * @param shp  the shape header
 * @return bits per pixel, or 0 for types with no entry
 */
unsigned char SHAPE_depth(unsigned char *shp) {
    return shapetypetobpp[*shp & 0x7F];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_depth1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char shapetypetobpp_39F8[128] __asm__("shapetypetobpp");

/**
 * Returns the bits per pixel for a shape type code.
 *
 * Same lookup as SHAPE_depth() (shapetypetobpp at the low 7 bits), but takes the type itself.
 *
 * @param shptype  shape type code
 * @return bits per pixel, or 0 for types with no entry
 */
unsigned char SHAPE_depth1(int shptype) {
    return shapetypetobpp_39F8[shptype & 0x7F];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_longname);
#ifdef SKIP_ASM
/**
 * Finds a shape's long name in its chain of attached blocks.
 *
 * Follows each block's 24-bit next offset (the upper three bytes of its first word) until it reaches a
 * block of type 0x70.
 *
 * @param shp  the shape header
 * @return the name text just after that block's 4-byte header, or 0 if the chain has none
 */
unsigned char *SHAPE_longname(unsigned char *shp) {
    while (shp) {
        int off;
        unsigned char *n;
        if (*shp == 0x70) {
            return shp + 4;
        }
        off = *(int *)shp >> 8;
        n = shp + off;
        shp = off ? n : 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_size1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// SHAPE_size (On Tour realgraph shpsize.cpp; this build has no swizzlesize helpers): byte size of a
// shape of the given type, dimensions and mip count, plus the 16-byte header (0 if empty).
typedef struct SHAPE_3A50 {
    unsigned char type;
    char rest[15];
} SHAPE_3A50;

extern int SHAPE_depth_3A50(SHAPE_3A50 *) __asm__("SHAPE_depth");

/**
 * Computes the byte size of a shape of a given type, size and mip count, header included.
 *
 * Gets the bits per pixel from SHAPE_depth on a dummy header (15 counts as 16). Palette types 0x20 and 0x21 when
 * swizzled are one row: the width is padded for the PS2 CLUT order (16..23 mod 32 rounds to 24, 8..15 adds 8) and the
 * size is width * bpp / 8. Otherwise it adds each level 0..mipmaps (dimensions halved, at least 1, rounded up to
 * multiples of 4 for the DXT types 0x60..0x62), rows rounded to whole bytes and levels aligned to 16 bytes. Called by
 * SHAPE_size.
 *
 * @param shptype  the shape type code
 * @param width  width in pixels
 * @param height  height in pixels
 * @param mipmaps  number of extra mip levels
 * @param swizzled  nonzero for PS2-swizzled palette data
 * @return the size plus the 16-byte header, or 0 when the image is empty
 */
int SHAPE_size1(int shptype, int width, int height, unsigned int mipmaps, int swizzled) {
    unsigned int i;
    int shpsize;
    SHAPE_3A50 shp;
    int bpp;
    int mipwidth;
    int mipheight;
    int bitsPerLine;

    shpsize = 0;
    shp.type = shptype;
    bpp = SHAPE_depth_3A50(&shp);
    if (bpp == 15) {
        bpp = 16;
    }
    if ((shptype == 0x20 || shptype == 33) && swizzled) {
        if ((unsigned int)((width & 0x1F) - 0x10) < 8U) {
            width = (width & ~0x1F) + 0x18;
        } else if ((unsigned int)((width & 0x1F) - 8) < 8U) {
            width += 8;
        }
        shpsize = width * bpp / 8;
    } else {
        for (i = 0; i <= mipmaps; i++) {
            mipwidth = width >> i;
            mipheight = height >> i;
            if (mipwidth < 1) {
                mipwidth = 1;
            }
            if (mipheight < 1) {
                mipheight = 1;
            }
            if (shptype == 0x60 || shptype == 97 || shptype == 98) {
                mipwidth = (mipwidth + 3) & ~3;
                mipheight = (mipheight + 3) & ~3;
            }
            if (shpsize) {
                shpsize = (shpsize + 15) & ~15;
            }
            bitsPerLine = mipwidth * bpp;
            shpsize += (bitsPerLine + 7) / 8 * mipheight;
        }
    }
    return shpsize ? shpsize + 16 : 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_size);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SHAPE_size1_3BB0(unsigned char, short, short, unsigned int, int) __asm__("SHAPE_size1");
extern int sizerefpack_3BB0(char *, int *) __asm__("sizerefpack");
extern int strlen_3BB0(char *) __asm__("strlen");

/**
 * Returns the size in bytes of one block of a shape chain, by block type.
 *
 * Name (0x70), comment (0x6F), 0x7C table and metal-bin (0x69) blocks carry their sizes; packed images read
 * it from
 * the packed header; other images are sized from format, dimensions and mipmap count.
 *
 * @param shp  the block
 * @return the size, or 0 when it cannot be worked out
 */
int SHAPE_size(SHAPE* shp) {
    int metalBinSize;
    int shapeSize = 0;
    int w;
    unsigned int payloadSize;
    char *metalData;
    unsigned int shapeType = *(unsigned char *)shp;
    switch (shapeType) {
    case 0x70:
        return strlen_3BB0((char*)&shp->width) + 5;
    case 111:
        return *(int*)&shp->width + 8;
    case 0x7C:
        return (*(int*)&shp->width << 3) + 8;
    case 105:
        if (*(unsigned short*)&shp->height & 0x10) {
            payloadSize = *(unsigned short*)&shp->width;
            if (payloadSize == 0) payloadSize = 0xF0;
            return payloadSize + 0x10;
        }
        return 0x10;
    default:
        if (*(unsigned char *)shp & 0x80) {
            metalBinSize = 0;
            if (*(int *)(((char*)shp) + 0xC) & 0x1000) {
                metalData = ((char*)shp) + *(int *)(((char*)shp) + 0x10);
            } else {
                metalData = ((char*)shp) + 0x10;
            }
            if (sizerefpack_3BB0(metalData, &metalBinSize) != 0) {
                shapeSize = metalBinSize + 0x10;
            }
        } else {
            w = *(int *)(((char*)shp) + 0xC);
            shapeSize = SHAPE_size1_3BB0(*(unsigned char *)shp, shp->width, shp->height, (unsigned int)w >> 28, (w >> 13) & 1);
        }
        return shapeSize;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_count);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: hand-written shape-table accessor returning the entry count at +8 (starts 4 mod 8 where every gcc function in seg_2B4578 is .align 3, unfilled jr delay slot, and it sits in the hand-written run with SHAPE_pointer and SHAPE_name, which landed as PORT asm); a port replaces it with `int SHAPE_count(void *s) { return *(int *)((char *)s + 8); }`.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 2\n.globl SHAPE_count\n.type SHAPE_count, @function\nSHAPE_count:\n"
    "lw $2, 0x8($4)\n"
    "jr $31\n"
    "nop\n"
    ".size SHAPE_count, . - SHAPE_count\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_pointer);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: hand-written bounds-checked shape lookup returning base + table[i].off (the assembler bgeu macro sltu $1 + beqz $1, $9/$11 temporaries, li as addiu $2,$0,0 where gcc emits move; same body as the landed PORT SHAPE_name with offset 0x14 for 0x10); a port replaces it with `char *f(char *s, unsigned i) { return i < *(unsigned *)(s + 8) ? s + *(int *)(s + 0x14 + i * 8) : 0; }`.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl SHAPE_pointer\n.type SHAPE_pointer, @function\nSHAPE_pointer:\n"
    "lw $9, 0x8($4)\n"
    "sll $11, $5, 3\n"
    "sltu $1, $5, $9\n"
    "beqz $1, .L003B3CF8\n"
    "addu $11, $4, $11\n"
    "lw $2, 0x14($11)\n"
    "jr $31\n"
    "addu $2, $4, $2\n"
    ".L003B3CF8:\n"
    "jr $31\n"
    "addiu $2, $0, 0x0\n"
    ".size SHAPE_pointer, . - SHAPE_pointer\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_name);
#ifdef SKIP_ASM
/**
 * Copies the four-character name of a shape directory entry into a buffer.
 *
 * Hand-written: if @p index is below the entry count at offset 8 of the shape file, the name word of entry
 * @p index (at 0x10 + index * 8) is stored to @p out with an unaligned store; otherwise 0 is stored.
 *
 * @param shape  shape file to read
 * @param index  directory entry index
 * @param out    receives the 4-byte name; need not be aligned
 * @return @p out
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written bounds-checked unaligned store: the assembler bgeu macro (sltu $1 + beqz $1), an explicit load-delay nop, swr/swl; the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl SHAPE_name\n.type SHAPE_name, @function\nSHAPE_name:\n"
    "lw $9, 0x8($4)\n"
    "sll $11, $5, 3\n"
    "sltu $1, $5, $9\n"
    "beqz $1, .L003B3D2C\n"
    "addu $11, $4, $11\n"
    "lw $2, 0x10($11)\n"
    "nop\n"
    "swr $2, 0x0($6)\n"
    "swl $2, 0x3($6)\n"
    "jr $31\n"
    "daddu $2, $6, $0\n"
    ".L003B3D2C:\n"
    "addiu $2, $0, 0x0\n"
    "swr $2, 0x0($6)\n"
    "swl $2, 0x3($6)\n"
    "jr $31\n"
    "daddu $2, $6, $0\n"
    ".size SHAPE_name, . - SHAPE_name\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B3D40);
#ifdef SKIP_ASM
/**
 * Finds the shape's attached block of type 0x69 and returns its data when its flag bit 0x10 is set.
 *
 * Starts with the shape itself and follows each block's next offset (the top 24 bits of its first word, 0 ends the
 * chain). The data starts after the 16-byte block header; the flag is the halfword at +6. Called by the movie frame
 * constructor func_003B07F8 on a shape made with SHAPE_create.
 *
 * @param shape  the shape
 * @return the block data, or 0 when there is no such block or the flag is clear
 */
void *func_003B3D40(int *shape) {
    int *temp_4;
    int *var_5;
    int temp_2;
    int var_2;
    unsigned short var_2_2;
    void *var_6;

    var_5 = shape;
    var_6 = 0;
    if (var_5 != 0) {
        if (*(unsigned char *)var_5 == 105) {
            var_6 = (char *)var_5 + 0x10;
            var_2_2 = *(unsigned short *)((char *)var_5 + 6);
            goto block_10;
        } else {
loop_5:
            var_2 = *var_5;
            temp_2 = var_2 >> 8;
            temp_4 = (temp_2 != 0) ? (int *)((char *)var_5 + temp_2) : 0;
            var_5 = temp_4;
            if (temp_4 != 0) {
                if (*(unsigned char *)temp_4 != 105) {
                    goto loop_5;
                }
                var_2_2 = *(unsigned short *)((char *)temp_4 + 6);
                var_6 = (char *)temp_4 + 0x10;
block_10:
                if (!(var_2_2 & 0x10)) var_6 = 0;
            }
        }
    }
    return var_6;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_type);
#ifdef SKIP_ASM
/**
 * Maps a pixel depth or format code to a SHAPE type class.
 *
 * @param bpp  bits per pixel or format code
 * @return 1 for 4 bpp, 2 for 8 bpp, 3 for 15/16-bit formats, 5 for 32-bit formats, otherwise 0
 */
int SHAPE_type(int bpp) {
    int type = 0;
    switch (bpp) {
    case 4: type = 1; break;
    case 8: type = 2; break;
    case 0xF: case 0x10: case 555: case 1555: type = 3; break;
    case 0x20: case 8888: type = 5; break;
    }
    return type;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_getpixeloffset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { unsigned int fmt : 8; unsigned int size : 24; short w; short h; char pad8[4]; int flags; } SH_3E40;
extern int SHAPE_depth_3E40(SH_3E40 *) __asm__("SHAPE_depth");
extern int GetSwizzledPixelOffset_3E40(int, int, int, int, int) __asm__("GetSwizzledPixelOffset");

/**
 * Returns the bit offset of a pixel in a shape's image data at a mip level.
 *
 * Adds up the earlier levels (DXT type 0x60 counts 4x4 blocks of 64 bits), then: palette types 0x20/0x21 with the
 * swizzle flag (0x2000) swap entries 8..23 of each 32 for the PS2 CLUT order and return without the level offset; other
 * swizzled shapes use GetSwizzledPixelOffset; linear ones use y * (row bits rounded to 16) + x * bpp. Quirk: earlier levels are
 * aligned to 16 bits here but to 16 bytes in SHAPE_size1. Nothing in the code or data calls it.
 *
 * @param shp  the shape
 * @param x  pixel column
 * @param y  pixel row
 * @param level  mip level
 * @return the offset in bits from the start of the image data
 */
int SHAPE_getpixeloffset(SH_3E40 *shp, int x, int y, int level) {
    int w, height, bpp, off, i, bitOffset, px, py;

    px = x;
    py = y;
    off = 0;
    w = shp->w;
    height = shp->h;
    bpp = SHAPE_depth_3E40(shp);
    if (bpp == 15) {
        bpp = 16;
    }
    if (shp->fmt == 0x60) {
        bpp = 64;
    }
    if (shp->fmt == 0x60) {
        w = (w + 3) >> 2;
        height = (height + 3) >> 2;
    }
    for (i = 0; i < level; i++) {
        off = (off + 15) & ~15;
        off += w * height * bpp;
        if (w >= 2) {
            w >>= 1;
        }
        if (height >= 2) {
            height >>= 1;
        }
    }
    if (shp->fmt == 0x60) {
        px = x / 4;
        py = y / 4;
    }
    if ((shp->fmt == 0x20 || shp->fmt == 33) && (shp->flags & 0x2000)) {
        bitOffset = px;
        if ((unsigned int)((px & 0x1F) - 8) <= 15) {
            bitOffset = px ^ 0x18;
        }
        if (shp->fmt == 33) {
            bitOffset <<= 5;
        } else if (shp->fmt == 0x20) {
            bitOffset <<= 4;
        }
        return bitOffset;
    } else if (shp->flags & 0x2000) {
        bitOffset = GetSwizzledPixelOffset_3E40(px, py, w, height, bpp);
    } else {
        bitOffset = py * ((w * bpp + 15) & ~15) + px * bpp;
    }
    return bitOffset + off;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_getpixeladr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Img_4010 {
    unsigned char fmt;
    char pad01[3];
    short w;
    short h;
    char pad08[4];
    int flags;
    int off;
};
extern int SHAPE_depth_4010(struct Img_4010 *img) __asm__("SHAPE_depth");
extern int GetSwizzledPixelOffset_4010(int x, int y, int w, int h, int bpp) __asm__("GetSwizzledPixelOffset");

/**
 * Returns the address of a pixel in a shape's image data at a mip level.
 *
 * The image data starts after the 16-byte header, or at the header's offset word (+0x10) when flag 0x1000 is set. The
 * bit offset is worked out as in SHAPE_getpixeloffset (same quirks) and divided by 8. Called by SHAPE_unpack.
 *
 * @param img  the shape
 * @param x  pixel column
 * @param y  pixel row
 * @param mip  mip level
 * @return the address of the byte holding the pixel
 */
char *SHAPE_getpixeladr(struct Img_4010 *img, int x, int y, int mip) {
    char *data;
    int w;
    int height;
    int bpp;
    int off;
    int i;
    int px;
    int py;
    int pos;
    int ofs;

    if (img->flags & 0x1000) {
        data = (char *)img + img->off;
    } else {
        data = (char *)img + 0x10;
    }
    w = img->w;
    height = img->h;
    px = x;
    py = y;
    bpp = SHAPE_depth_4010(img);
    if (bpp == 15) {
        bpp = 16;
    }
    off = 0;
    if (img->fmt == 0x60) {
        bpp = 64;
    }
    if (img->fmt == 0x60) {
        w = (w + 3) >> 2;
        height = (height + 3) >> 2;
    }
    for (i = 0; i < mip; i++) {
        off = (off + 15) & ~15;
        off += w * height * bpp;
        if (w >= 2) {
            w >>= 1;
        }
        if (height >= 2) {
            height >>= 1;
        }
    }
    if (img->fmt == 0x60) {
        px = x / 4;
        py = y / 4;
    }
    if ((img->fmt == 0x20 || img->fmt == 33) && (img->flags & 0x2000)) {
        pos = px;
        if ((unsigned int)((pos & 0x1F) - 8) < 16) {
            pos ^= 0x18;
        }
        if (img->fmt == 33) {
            pos <<= 5;
        } else if (img->fmt == 0x20) {
            pos <<= 4;
        }
        ofs = pos;
    } else {
        if (img->flags & 0x2000) {
            pos = GetSwizzledPixelOffset_4010(px, py, w, height, bpp);
        } else {
            pos = py * ((w * bpp + 15) & ~15) + px * bpp;
        }
        ofs = pos + off;
    }
    return data + (ofs >> 3);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", GetSwizzledPixelOffset);
#ifdef SKIP_ASM
extern int Pixel4Offset(int, int, int);
extern int Pixel8Offset(int, int, int);
extern int func_003B43C0(int, int, int);
extern int Pixel32Offset(int, int, int);

/**
 * Returns the bit offset of a texel in PS2-swizzled image data, by bits per pixel.
 *
 * 4 bpp uses Pixel4Offset, 8 bpp Pixel8Offset, 15/16 bpp func_003B43C0, 32 bpp Pixel32Offset; other depths give 0.
 * The height is unused. Called by SHAPE_getpixeloffset and SHAPE_getpixeladr.
 *
 * @param texelX  texel column
 * @param texelY  texel row
 * @param width  image width
 * @param height  image height (unused)
 * @param kind  bits per pixel
 * @return the offset in bits
 */
int GetSwizzledPixelOffset(int texelX, int texelY, int width, int height, int kind) {
    int result = 0;
    switch (kind) {
    case 4:
        result = Pixel4Offset(texelX, texelY, width);
        break;
    case 8:
        result = Pixel8Offset(texelX, texelY, width);
        break;
    case 15:
        result = func_003B43C0(texelX, texelY, width);
        break;
    case 16:
        result = func_003B43C0(texelX, texelY, width);
        break;
    case 32:
        result = Pixel32Offset(texelX, texelY, width);
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", Pixel4Offset);
#ifdef SKIP_ASM
/**
 * Returns the bit offset of a 4-bit texel in PS2-swizzled image data.
 *
 * Rearranges x and y into the swizzled column, row and in-block position (rows of 128 x 128 texel blocks) and returns
 * ((row * width / 2 + column) << 5) plus the low position bits. Called by GetSwizzledPixelOffset.
 *
 * @param x  texel column
 * @param y  texel row
 * @param w  image width
 * @return the offset in bits
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
int Pixel4Offset(int x, int y, int w) {
    int blockRow = (y & 0x70) >> 1;
    int pairSwap = (y & 2) << 1;
    int lo = blockRow + ((x & 7) ^ pairSwap ^ (y & 4));
    int lx = (x & 0x18) + pairSwap;
    y = ((y >> 7) << 5) + (((x & 0x60) >> 2) + ((y & 0xC) >> 1) + (y & 1));
    x = ((x >> 7) << 6) + lo;
    w = (y * w / 2 + x) << 5;
    return w + lx;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", Pixel8Offset);
#ifdef SKIP_ASM
#include "fakematch.h"
/**
 * Returns the bit offset of an 8-bit texel in PS2-swizzled image data.
 *
 * Same scheme as Pixel4Offset with 8-bit blocks (128 x 64 texels). Called by GetSwizzledPixelOffset.
 *
 * @param x  texel column
 * @param y  texel row
 * @param w  image width
 * @return the offset in bits
 */
// FAKEMATCH: register pin `register int hx FM_REG("$5") = x >> 7; hx <<= 6;` (fakematch.h) for the swizzle's column term
// (retail shifts x >> 7 in place in a1 and keeps the quotient's sign bit in a3; unpinned, the allocator also swaps y and y & 2
// between a3 and a4); plain form 82.27% (honest best, the one-expression `x = (x >> 7) << 6` form, 91.06%).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int Pixel8Offset(int x, int y, int w) {
    int lo = ((x & 0x70) >> 1) + ((x & 7) ^ ((y & 2) << 1) ^ (y & 4));
    int lx = ((x & 8) << 1) + ((y & 2) << 2);
    y = ((y >> 6) << 5) + (((y & 0x3C) >> 1) + (y & 1));
    {
        register int hx FM_REG("$5") = x >> 7;
        hx <<= 6;
        x = hx + lo;
    }
    w = (y * w / 2 + x) << 5;
    return w + lx;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003B43C0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", Pixel32Offset);
#ifdef SKIP_ASM
/**
 * Returns the bit offset of a 32-bit texel: (y * width + x) * 32.
 *
 * 32-bit data is not reordered. Called by GetSwizzledPixelOffset.
 *
 * @param texelX  texel column
 * @param texelY  texel row
 * @param width  image width
 * @return the offset in bits
 */
int Pixel32Offset(int texelX, int texelY, int width) {
    return ((texelY * width) + texelX) << 5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_setmemcallbacks);
#ifdef SKIP_ASM
extern int SHAPE_memalloc[];
extern int SHAPE_memallocalign[];
extern int SHAPE_memfree[];

/**
 * Sets the SHAPE library's memory callbacks: allocate, aligned allocate and free.
 *
 * Stores them in SHAPE_memalloc, SHAPE_memallocalign and SHAPE_memfree; SHAPE_destroy-like SHAPE_destroy frees through the
 * last. Nothing in the code or data calls it.
 *
 * @param memAlloc  the allocate function
 * @param memAllocAlign  the aligned allocate function
 * @param memFree  the free function
 */
void SHAPE_setmemcallbacks(int memAlloc, int memAllocAlign, int memFree) {
    SHAPE_memalloc[0] = memAlloc;
    SHAPE_memallocalign[0] = memAllocAlign;
    SHAPE_memfree[0] = memFree;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_clonesize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int SHAPE_size_4478(unsigned char *) __asm__("SHAPE_size");
extern int SHAPE_isclut_4478(int) __asm__("SHAPE_isclut");

/**
 * Computes how many bytes SHAPE_clone() needs to copy a shape and its attached blocks.
 *
 * Adds SHAPE_size() for each block in the chain, starting each block after the first at the next 16-byte
 * boundary relative to @p shp's own alignment. The palette and non-palette branches compute the same rounding.
 *
 * @param shp  the shape header
 * @return total size in bytes, or 0 for a null @p shp
 */
int SHAPE_clonesize(unsigned char *shp) {
    int size = 0;
    int align = (((int)shp + 15) & ~15) - (int)shp;

    if (shp) {
        do {
            int n = SHAPE_size_4478(shp);
            int off;
            if (size) {
                if (SHAPE_isclut_4478(*shp)) {
                    int alignedSize = (size - align + 0x1F) & ~0xF;
                    int clutAlign = align - 0x10;
                    size = alignedSize + clutAlign;
                } else {
                    size = ((size - align + 0xF) & ~0xF) + align;
                }
            }
            off = *(int *)shp >> 8;
            { unsigned char *nextShape = shp + off; shp = off ? nextShape : 0; }
            size += n;
        } while (shp);
    }
    return size;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_cloneat);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SHAPE_size_neat(char *) __asm__("SHAPE_size");
extern int SHAPE_isclut_neat(int) __asm__("SHAPE_isclut");
extern void MEM_move_neat(char *, char *, int) __asm__("MEM_move");
extern int sprintf_neat(char *, char *, ...) __asm__("sprintf");
extern char str_EAGLpct_d_neat[] __asm__("str_EAGLpct_d");

/**
 * Copies a shape and its whole block chain to new memory, realigning each block and relinking the chain.
 *
 * Palette blocks get the stricter alignment. Metal-bin blocks of 0x15 bytes or more get "EAGL<n>" written at
 * +0x10.
 *
 * @param dst  destination memory, large enough for the realigned chain
 * @param src  the first block of the source chain, or 0
 */
void SHAPE_cloneat(char *dst, char *src) {
    char *curShape = src;
    int off = 0;
    char *prev = 0;
    char *lastClone;
    char *np;
    int align = (((int)curShape + 0xF) & -16) - (int)curShape;
    int clutAlign;
    int mask;
    int size, n;
    if (curShape != 0) {
        mask = -16;
        clutAlign = align - 0x10;
        do {
            size = SHAPE_size_neat(curShape);
            if (off != 0) {
                if (SHAPE_isclut_neat(*(unsigned char *)curShape) != 0) {
                    off = (((off - align) + 0x1F) & mask) + clutAlign;
                } else {
                    off = (((off - align) + 0xF) & mask) + align;
                }
            }
            lastClone = prev;
            prev = dst + off;
            if (lastClone != 0) {
                *(int *)lastClone = *(unsigned char *)lastClone | ((prev - lastClone) << 8);
            }
            *(int *)prev = *(unsigned char *)prev;
            if (size != 0) {
                MEM_move_neat(prev, curShape, size);
                if (*(unsigned char *)prev == 105 && size >= 21) {
                    sprintf_neat(prev + 0x10, str_EAGLpct_d_neat, *(short *)(prev + 4));
                }
            }
            n = *(int *)curShape >> 8;
            np = 0;
            if (n != 0) np = curShape + n;
            curShape = np;
            off += size;
        } while (curShape != 0);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_clone);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int SHAPE_clonesize_4690(int) __asm__("SHAPE_clonesize");
extern void *SHAPE_longname_4690(int) __asm__("SHAPE_longname");
extern void SHAPE_cloneat_4690(void *, int) __asm__("SHAPE_cloneat");
extern void *(*SHAPE_memallocalign_4690)(void *, int, int, int, int) __asm__("SHAPE_memallocalign");

/**
 * Makes a copy of a shape, with its attached blocks, in newly allocated memory.
 *
 * Allocates SHAPE_clonesize() bytes through the SHAPE_memallocalign hook (16-byte aligned, tagged with the
 * shape's long name) and fills it with SHAPE_cloneat(). The allocation is not checked for failure.
 *
 * @param shp  the shape to copy
 * @return the copy
 */
void *SHAPE_clone(int shp) {
    int sz = SHAPE_clonesize_4690(shp);
    void *newShape = SHAPE_memallocalign_4690(SHAPE_longname_4690(shp), sz, 0, 0x10, 0x400);

    SHAPE_cloneat_4690(newShape, shp);
    return newShape;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SHAPE_isclut);
#ifdef SKIP_ASM
/**
 * Tells whether a shape block type is a palette (CLUT) block.
 *
 * @param shptype  the block type
 * @return 1 for the palette types (0x20-0x38 set), otherwise 0
 */
int SHAPE_isclut(int shptype) {
    int var_5;

    var_5 = 0;
    switch (shptype) {
    case 0x20:
    case 33:
    case 34:
    case 36:
    case 41:
    case 42:
    case 44:
    case 45:
    case 46:
    case 0x30:
    case 49:
    case 50:
    case 51:
    case 0x38:
        var_5 = 1;
        break;
    }
    return var_5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", unpackz);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int REF_decode(unsigned char *dst, unsigned char *src, int *used);
int f_4740(unsigned char *src, unsigned char *dst) __asm__("unpackz");

/**
 * Unpacks compressed data when it is in a known format; returns the unpacked size.
 *
 * Only RefPack data (second byte 0xFB) of type 0x10 or 0x90 (the flag 0x01 ignored) is decoded, with
 * REF_decode(dst, src, 0); the other recognised type codes (0x18..0x1E, 0x30..0x46 and their 0x80 forms) are
 * accepted but not decoded in this build, and give 0 like unknown data.
 *
 * @param src  the packed data
 * @param dst  where to write the unpacked bytes
 * @return the number of bytes written, or 0
 */
int f_4740(unsigned char *src, unsigned char *dst) {
    int len = 0;
    if (src[1] == 251) {
        switch (src[0] & 0xFE) {
        case 0x10:
        case 144:
            len = REF_decode(dst, src, 0);
            break;
        case 0x18: case 26: case 0x1C: case 0x1E:
        case 0x30: case 50: case 52: case 70:
        case 152: case 154: case 156: case 158:
        case 176: case 178: case 180: case 198:
            break;
        }
    }
    return len;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", unpack);
#ifdef SKIP_ASM
extern void unpackz();

/**
 * Unpacks compressed data (the public entry point); forwards to unpackz.
 *
 * Declared without parameters: the source and destination pointers pass through in $4 and $5 untouched, and
 * unpackz's unpacked size is returned.
 */
void unpack(void) {
    unpackz();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", unpacksizez);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int f_4818(unsigned char *p) __asm__("unpacksizez");
static __inline__ int get24_4818(unsigned char *q) { return (q[0] << 16) | (q[1] << 8) | q[2]; }
static __inline__ int get32_4818(unsigned char *q) { return (q[0] << 24) | (q[1] << 16) | (q[2] << 8) | q[3]; }
/**
 * Reads the unpacked size of compressed data from its header.
 *
 * For RefPack data (second byte 0xFB) the size follows the two type bytes: 24 bits big-endian for types
 * 0x10..0x46, 32 bits for their 0x80 forms (0x90..0xC6); type 0xC0 and unknown data give 0.
 *
 * @param src  the packed data
 * @return the unpacked size in bytes, or 0
 */
int f_4818(unsigned char *src) {
    int len = 0;
    if (src[1] == 251) {
        switch (src[0] & 0xFE) {
        case 0x10: case 0x18: case 26: case 0x1E:
        case 0x30: case 50: case 52: case 70:
            len = get24_4818(src + 2);
            break;
        case 144: case 152: case 154: case 158:
        case 176: case 178: case 180: case 198:
            len = get32_4818(src + 2);
            break;
        case 0xC0:
            break;
        }
    }
    return len;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", unpacksize);
#ifdef SKIP_ASM
extern void unpacksizez();

/**
 * Returns the unpacked size of compressed data (forwards to unpacksizez).
 *
 * The source pointer passes through in $a0 although the declaration takes none. Nothing in the code or data calls it.
 */
void unpacksize(void) {
    unpacksizez();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", REF_decode);
#ifdef SKIP_ASM
/**
 * Decodes RefPack-compressed data (EA's LZ77 variant).
 *
 * Reads the two-byte header (0x8000: 32-bit sizes, 0x100: a compressed-size field to skip), the unpacked length,
 * then executes the literal and back-reference commands until the end code, writing to @p dst.
 *
 * @param dst   where to write the unpacked bytes
 * @param src   the compressed data
 * @param used  when not null, receives the number of compressed bytes read
 * @return the unpacked length, or 0 for a null @p src
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int REF_decode(unsigned char *dst, unsigned char *src, int *used) {
    unsigned char *srcPos = src;
    unsigned char *ref;
    unsigned int type;
    unsigned char first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;
    unsigned int run;
    int len = 0;

    if (src != 0) {
        type = srcPos[0];
        type = (type << 8) + srcPos[1];
        srcPos = src + 2;
        if (type & 0x8000) {
            if (type & 0x100) {
                srcPos = src + 6;
            }
            len = *srcPos++;
            len = (len << 8) + *srcPos++;
            len = (len << 8) + *srcPos++;
            len = (len << 8) + *srcPos++;
        } else {
            if (type & 0x100) {
                srcPos = src + 5;
            }
            len = *srcPos++;
            len = (len << 8) + *srcPos++;
            len = (len << 8) + *srcPos++;
        }
        for (;;) {
            first = *srcPos++;
            if (!(first & 0x80)) {
                second = *srcPos++;
                run = first & 3;
                while (run--) *dst++ = *srcPos++;
                ref = dst - 1 - (((first & 0x60) << 3) + second);
                run = ((first & 0x1C) >> 2) + 3 - 1;
                do *dst++ = *ref++; while (run--);
                continue;
            }
            if (!(first & 0x40)) {
                second = *srcPos++;
                third = *srcPos++;
                run = second >> 6;
                while (run--) *dst++ = *srcPos++;
                ref = dst - 1 - (((second & 0x3F) << 8) + third);
                run = (first & 0x3F) + 4 - 1;
                do *dst++ = *ref++; while (run--);
                continue;
            }
            if (!(first & 0x20)) {
                second = *srcPos++;
                third = *srcPos++;
                fourth = *srcPos++;
                run = first & 3;
                while (run--) *dst++ = *srcPos++;
                ref = dst - 1 - ((((first & 0x10) >> 4) << 16) + (second << 8) + third);
                run = ((first & 0x0C) << 6) + fourth + 5 - 1;
                do *dst++ = *ref++; while (run--);
                continue;
            }
            run = ((first & 0x1F) << 2) + 4;
            if (run <= 0x70) {
                while (run--) *dst++ = *srcPos++;
                continue;
            }
            run = first & 3;
            while (run--) *dst++ = *srcPos++;
            break;
        }
    }
    if (used != 0) {
        *used = srcPos - src;
    }
    return len;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", sizerefpack);
#ifdef SKIP_ASM
/**
 * Reads a RefPack stream's unpacked size and, optionally, measures its packed length.
 *
 * The 24-bit big-endian unpacked size follows the 2-byte header (or 5 bytes when bit 0 of the first byte says a packed
 * size is stored). With outlen it walks the commands (2-, 3- and 4-byte copies with their 0..3 literals, literal runs
 * of 4..112 bytes) up to the end command 0xFC..0xFF. Called by SHAPE_size.
 *
 * @param packed  the compressed data, or 0
 * @param outlen  receives the packed length in bytes, or 0 to skip
 * @return the unpacked size, or 0 without data
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
int sizerefpack(unsigned char *packed, int *outlen) {
    unsigned char *cursor;
    int n = 0;
    unsigned int command;
    unsigned int k;

    if (packed) {
        cursor = packed + 2;
        if ((packed[0] << 8) & 0x100) cursor = packed + 5;
        n = *cursor++;
        n = (n << 8) + *cursor++;
        n = (n << 8) + *cursor++;
        if (outlen) {
            for (;;) {
                command = *cursor++;
                if (!(command & 0x80)) {
                    cursor++;
                    cursor += command & 3;
                    continue;
                }
                if (!(command & 0x40)) {
                    unsigned int nextByte = cursor[0];
                    cursor += 2;
                    cursor += nextByte >> 6;
                    continue;
                }
                if (!(command & 0x20)) {
                    cursor += 3;
                    cursor += command & 3;
                    continue;
                }
                {
                    k = ((command & 0x1F) << 2) + 4;
                    if (k < 113) {
                        cursor += k;
                        continue;
                    }
                }
                cursor += command & 3;
                break;
            }
            k = cursor - packed;
            *outlen = k;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_getopts);
#ifdef SKIP_ASM
struct Cfg_4DD8 {
    char pad00[0x20];
    struct SNDSYSSET sub;
    char padC0[0xD4 - 0xC0];
};
struct G_4DD8 {
    struct Cfg_4DD8 cfg;
    struct SNDSYSSET saved;
};
extern int gotcaps;
extern int D_0044C460;
extern int SNDPLATFORM_outputcaps_4DD8() __asm__("SNDPLATFORM_outputcaps");

int SNDSYS_getopts_4DD8(struct Cfg_4DD8 *out) __asm__("SNDSYS_getopts");

/**
 * Copies the sound system's options into a caller's buffer, setting the defaults on first use.
 *
 * The first call stores SNDPLATFORM_outputcaps's result (D_0044C460) and sets 16 bank slots (+0x08 of the options at
 * D_0050A908), +0x2B to 90, +0x1C to 1, +0x14 to -1.0f, and saves a copy (sndgs +0xD4). Every call copies the
 * 0xD4-byte block at sndgs out. Called by SNDSYSI_init and the game's sound setup (System_SetRandomSeed ...
 * System_Init), which change fields and pass them to SNDSYS_setopts.
 *
 * @param out  receives the 0xD4-byte options
 * @return SNDPLATFORM_outputcaps's result from the first call
 */
int SNDSYS_getopts_4DD8(struct Cfg_4DD8 *out) {
    if (gotcaps == 0) {
        D_0044C460 = SNDPLATFORM_outputcaps_4DD8();
        gotcaps = 1;
        sndgs.sso.set.maxbanks = 0x10;
        sndgs.sso.set.sndheapthreshold = 90;
        sndgs.sso.set.stealequalpriorityvoices = 1;
        sndgs.sso.set.updateperiod = -1.0f;
        *(struct SNDSYSSET*)&sndgs.prevset = *(struct SNDSYSSET*)&sndgs.sso.set;
    }
    *out = *(struct Cfg_4DD8*)&sndgs.sso;
    return D_0044C460;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_setopts);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
struct sA8E8Cfg {                   /* 0xA0: the current setup (+0x20, D_0050A908) and a saved copy (+0xD4) */
    char pad00[0x4];
    int f04;
    unsigned short f08;             /* number of slots (f1F4) */
    unsigned short f0A;
    unsigned short f0C;
    unsigned char f0E;
    unsigned char f0F;
    unsigned char f10;
    char pad11[0x14 - 0x11];
    float f14;
    char pad18[0x1C - 0x18];
    unsigned char f1C;
    char pad1D;
    unsigned char f1E;
    unsigned char f1F;
    unsigned char f20;
    unsigned char f21;
    unsigned char f22;
    char pad23[0x26 - 0x23];
    unsigned char f26;
    unsigned char f27;
    unsigned char f28;
    unsigned char f29;
    unsigned char f2A;
    char f2B;
    unsigned char f2C;
    char pad2D;
    short f2E[3];
    char pad34[0x44 - 0x34];
    unsigned int f44;
    int f48;
    char *f4C;                      /* also D_0050A954 */
    char pad50[0x58 - 0x50];
    unsigned short f58[5][6];       /* row n: n + 1 angles */
    char pad94[0xA0 - 0x94];
};
struct sA8E8Tail {                  /* +0xC0 (D_0050A9A8) */
    int f00;
    int f04;
    void (*f08)(void);              /* D_0050A9B0 */
    int f0C;
    int f10;
};
struct sA8E8Setup {                 /* 0xD4: what SNDSYS_getopts hands out */
    unsigned short f00;
    unsigned short f02;
    unsigned char f04;
    unsigned char f05;
    unsigned char f06;
    unsigned char f07;
    unsigned char f08;
    char pad09[0xC - 0x9];
    unsigned char f0C;
    char pad0D[0x13 - 0xD];
    unsigned char f13;
    short f14[3];
    short f1A[3];
    struct sA8E8Cfg cfg;
    struct sA8E8Tail tail;
};
struct sA8E8Node {                  /* +0x1B4 list */
    struct sA8E8Node *next;
    int f04;
    void (*fn)(int);
    int arg;
};
struct sA8E8HeapEnt { int key; int val; };
struct sA8E8Heap {                  /* +0x234 (also D_0050AB1C): block heap, entry table grows down from f04 */
    int f00;
    struct sA8E8HeapEnt *f04;
    int f08;
    unsigned int f0C;
    int f10;
    int f14;
};
struct sA8E8Bank {                  /* a loaded bank's header */
    int f00;
    short f04;
    unsigned short f06;             /* number of f14 entries */
    int f08;                        /* header size */
    int f0C;                        /* size of the kind-8 data */
    int f10;                        /* size of the kind-0x100 data */
    int f14[1];                     /* self-relative offsets */
};
struct sA8E8Pan { float l; float r; };
struct sA8E8Bus {                   /* 0x14, at +0x1F8 / +0x20C / +0x220, indexed by id */
    short f00;
    char f02;
    char pad03;
    int f04;
    int f08;
    struct sA8E8Pan f0C;
};
struct sA8E8 {
    struct sA8E8Setup setup;
    struct sA8E8Cfg saved;
    signed char f174;               /* also D_0050AA5C */
    char f175;
    volatile unsigned char f176;    /* lock depth */
    signed char f177;
    signed char f178;
    signed char f179;
    unsigned char f17A;             /* also D_0050AA62 */
    char pad17B[0x17E - 0x17B];
    short f17E;                     /* number of channels (f1F0) */
    volatile unsigned int f180;     /* tick */
    void (*f184[6])(void);          /* f177 entries */
    void (*f19C[6])(void);          /* f178 entries */
    struct sA8E8Node *f1B4;
    char pad1B8[0x1C0 - 0x1B8];
    int f1C0;
    char pad1C4[0x1C8 - 0x1C4];
    void (*f1C8[4])(void *);        /* f179 entries */
    void (*f1D8)(void);
    void (*f1DC)(void);
    void (*f1E0)(void);
    void (*f1E4)(int);
    void (*f1E8)(void);
    void (*f1EC)(void);
    struct CHANPUB *f1F0;
    struct BANKLIST *f1F4;
    struct sA8E8Bus f1F8[1];        /* kind 4 */
    struct sA8E8Bus f20C[1];        /* kind 0x100 */
    struct sA8E8Bus f220[1];        /* kind 8 */
    struct sA8E8Heap *f234;
    char pad238[0x2E0 - 0x238];
    unsigned short f2E0[6][6];      /* also D_0050ABC8: row n - 1 = n channel angles */
    void (*f328)(void);             /* sound thread hooks: before / after a tick */
    void (*f32C)(void);
    volatile unsigned int f330;
    volatile unsigned short f334;
};
extern struct sA8E8Cfg D_0050A908;      /* sndgs.setup.cfg */
extern struct sA8E8Tail D_0050A9A8;     /* sndgs.setup.tail */
extern int SNDPLATFORM_outputset(void);
int SNDSYS_setopts_impl(struct sA8E8Setup *p) __asm__("SNDSYS_setopts");
/**
 * Applies sound system options: copies them in, updates the driver and saves a copy.
 *
 * Copies the 0xA0-byte configuration (+0x20) to D_0050A908 and the 0x14-byte tail (+0xC0) to D_0050A9A8, calls
 * SNDPLATFORM_outputset, then copies the configuration into the saved block (sndgs +0xD4). The first 0x20 bytes of the
 * options are not applied.
 *
 * @param sysOpts  the options (as filled by SNDSYS_getopts)
 * @return 0
 */
int SNDSYS_setopts_impl(struct sA8E8Setup *sysOpts) {
    __builtin_memcpy(&D_0050A908, &sysOpts->cfg, sizeof(struct sA8E8Cfg));
    D_0050A9A8 = sysOpts->tail;
    SNDPLATFORM_outputset();
    /* sndgs.saved = sndgs.setup.cfg, addressed from D_0050A9A8 as in the target */
    __builtin_memcpy((char *)&D_0050A9A8 + 0x14, (char *)&D_0050A9A8 - 0xA0, sizeof(struct sA8E8Cfg));
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYSI_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern char str_SNDAUTHOR_Rgerhards_Thursday_10_51AM_Aug;
extern void memset_5158(void *, int, int) __asm__("memset");
void SNDMEMI_init_5158(struct sA8E8Heap *a0, int a1) __asm__("SNDMEMI_init");
int SNDSYS_getopts_5158(struct sA8E8Setup *out) __asm__("SNDSYS_getopts");
int SNDSYS_setopts_impl(struct sA8E8Setup *p) __asm__("SNDSYS_setopts");
void SNDI_randomseed(int arg0);
void SNDI_mutexalloc(void);
void SNDSYS_entercritical(void);
void SNDSYS_leavecritical(void);
extern void *SNDMEMI_alloc_5158(int) __asm__("SNDMEMI_alloc");
extern int SNDPLATFORM_init(void);
extern void SNDPLATFORM_restore_5320() __asm__("SNDPLATFORM_restore");
extern void SNDI_mutexfree();
extern void func_003BC0B0(void);

int SNDSYSI_init_impl(void *buf, int size, int arg2) __asm__("SNDSYSI_init");

/**
 * Starts the EA sound system in a heap: memory, options, channel and bank tables, platform.
 *
 * Touches the "SNDAUTHOR" string, returns 0 at once when already started (sndgs +0x174). Clears the heap
 * (memset) and hands it to SNDMEMI_init; without channels configured it applies the default options
 * (SNDSYS_getopts, SNDSYS_setopts). Then SNDI_randomseed, SNDI_mutexalloc, marks itself started and, in a critical section
 * (SNDSYS_entercritical/SNDSYS_leavecritical), allocates 0x8C bytes per channel (+0x1F0) and 16 per bank slot (+0x1F4). On a
 * SNDPLATFORM_init failure it undoes the platform (SNDPLATFORM_restore, SNDI_mutexfree) and returns the error. Finally it fills
 * the speaker angle table (+0x2E0) for 2..6 outputs and calls func_003BC0B0.
 *
 * Called by cPS2SoundManager_Init and System_Init.
 *
 * @param buf  the sound heap
 * @param size  its size
 * @param headerVersion  the snd.h version the caller was built with (cPS2SoundManager_Init passes 0x80303, V8.03.03;
 *                       System_Init passes its own argument); this build does not check it
 * @return 0, or a negative error
 */
int SNDSYSI_init_impl(void *buf, int size, int arg2) {
    int result;
    str_SNDAUTHOR_Rgerhards_Thursday_10_51AM_Aug = 'S';
    if (sndgs.installed != 0) return 0;
    memset_5158(buf, 0, size);
    SNDMEMI_init_5158(buf, size);
    if (sndgs.voicestotal == 0) {
        result = SNDSYS_getopts_5158(&(*(struct sA8E8Setup*)&sndgs.sso));
        if (result < 0) return result;
        SNDSYS_setopts_impl(&(*(struct sA8E8Setup*)&sndgs.sso));
    }
    SNDI_randomseed(((int)sndgs.sso.set.randomseed));
    SNDI_mutexalloc();
    sndgs.installed = 1;
    SNDSYS_entercritical();
    *(struct CHANPUB**)&sndgs.chan = SNDMEMI_alloc_5158(sndgs.voicestotal * 140);
    *(struct BANKLIST**)&sndgs.banklist = SNDMEMI_alloc_5158(sndgs.sso.set.maxbanks << 4);
    SNDSYS_leavecritical();
    sndgs.servertick = 0;
    sndgs.mastervol = 0x7F;
    sndgs.audiotick = 0;
    sndgs.numuserdataclients = 0;
    sndgs.numserverclients100hz = 0;
    sndgs.numserverclients = 0;
    result = SNDPLATFORM_init();
    if (result < 0) {
        SNDPLATFORM_restore_5320();
        SNDI_mutexfree();
        sndgs.installed = 0;
        return result;
    }
    if (sndgs.sso.set.outputchannels == 2) {
        sndgs.srcchancfg3d[1][0] = 0xC000;
        sndgs.srcchancfg3d[1][1] = 0x4000;
    } else {
        sndgs.srcchancfg3d[1][0] = 0xE000;
        sndgs.srcchancfg3d[1][1] = 0x2000;
    }
    sndgs.srcchancfg3d[2][0] = sndgs.sso.set.virtualspkrcfg3d[2][2];
    sndgs.srcchancfg3d[2][1] = sndgs.sso.set.virtualspkrcfg3d[2][0];
    sndgs.srcchancfg3d[2][2] = sndgs.sso.set.virtualspkrcfg3d[2][1];
    sndgs.srcchancfg3d[3][0] = 0xE000;
    sndgs.srcchancfg3d[3][1] = 0x2000;
    sndgs.srcchancfg3d[3][2] = 40960;
    sndgs.srcchancfg3d[3][3] = 0x6000;
    sndgs.srcchancfg3d[4][0] = 0xE000;
    sndgs.srcchancfg3d[4][1] = 0;
    sndgs.srcchancfg3d[4][2] = 0x2000;
    sndgs.srcchancfg3d[4][3] = 40960;
    sndgs.srcchancfg3d[4][4] = 0x6000;
    sndgs.srcchancfg3d[5][0] = 0xE000;
    sndgs.srcchancfg3d[5][1] = 0;
    sndgs.srcchancfg3d[5][2] = 0x2000;
    sndgs.srcchancfg3d[5][3] = 40960;
    sndgs.srcchancfg3d[5][4] = 0x6000;
    sndgs.srcchancfg3d[5][5] = 0;
    func_003BC0B0();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003B5440_5320() __asm__("func_003B5440");
extern int func_003B82B8_5320(int, int, int, int, int) __asm__("func_003B82B8");
extern void SNDI_freespkrtable_5320() __asm__("SNDI_freespkrtable");
extern void func_003B8A38_5320() __asm__("func_003B8A38");
extern void SNDPLATFORM_restore_5320() __asm__("SNDPLATFORM_restore");
extern void SNDSYS_entercritical_5320() __asm__("SNDSYS_entercritical");
extern void SNDMEMI_free_5320(int) __asm__("SNDMEMI_free");
extern void SNDSYS_leavecritical_5320() __asm__("SNDSYS_leavecritical");
extern int SNDMEMI_restore_5320() __asm__("SNDMEMI_restore");
extern void SNDI_mutexfree_5320() __asm__("SNDI_mutexfree");

/**
 * Shuts the EA sound system down and releases its tables.
 *
 * Returns -14 when it is not started. Stops everything (func_003B82B8(0, 0, 0, -1, -1)), runs the shutdown hooks
 * (+0x1D8, +0x1DC, +0x1E0, +0x1EC, +0x1E8), SNDI_freespkrtable, func_003B8A38, hook +0x1E4 with -1, SNDPLATFORM_restore, frees the
 * channel and bank-slot tables with SNDMEMI_free in a critical section, restores memory (SNDMEMI_restore), clears the
 * started flag and calls SNDI_mutexfree. Called by cPS2SoundManager_Shutdown and System_Restore.
 *
 * @return SNDMEMI_restore's result, or -14 when not started
 */
int SNDSYS_restore(void) {
    int memResult;
    if (func_003B5440_5320() == 0) return -14;
    func_003B82B8_5320(0, 0, 0, -1, -1);
    if (((void (*)(void))sndgs.unk_1D8)) ((void (*)(void))sndgs.unk_1D8)();
    if (((void (*)(void))sndgs.unk_1DC)) ((void (*)(void))sndgs.unk_1DC)();
    if (((void (*)(void))sndgs.unk_1E0)) ((void (*)(void))sndgs.unk_1E0)();
    if (((void (*)(void))sndgs.streamrestore)) ((void (*)(void))sndgs.streamrestore)();
    if (((void (*)(void))sndgs.midirestore)) ((void (*)(void))sndgs.midirestore)();
    SNDI_freespkrtable_5320();
    func_003B8A38_5320();
    if (sndgs.bankremove) sndgs.bankremove(-1);
    SNDPLATFORM_restore_5320();
    SNDSYS_entercritical_5320();
    SNDMEMI_free_5320(((int)(struct CHANPUB*)sndgs.chan));
    SNDMEMI_free_5320(((int)(struct BANKLIST*)sndgs.banklist));
    SNDSYS_leavecritical_5320();
    memResult = SNDMEMI_restore_5320();
    sndgs.installed = 0;
    SNDI_mutexfree_5320();
    return memResult;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5440);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns whether the EA sound system is started (D_0050AA5C).
 *
 * Called by SNDSYS_restore and, before using sound, the movie player (func_003ADEC8, func_003AED40).
 *
 * @return nonzero when started
 */
signed char func_003B5440(void) {
    return sndgs.installed;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDREAL_systemtask);
#ifdef SKIP_ASM
extern void SNDSYS_service();

/**
 * Runs the sound system's per-frame clients (a SYNCTASK body).
 *
 * Calls SNDSYS_service, which runs the client callbacks at the sound driver state sndgs +0x19C while the system is
 * started. System_VectorToReal5 registers it with SYNCTASK_add.
 *
 * @return 0
 */
int SNDREAL_systemtask(void) {
    SNDSYS_service();
    return 0;
}
#endif

/**
 * Does nothing; installed as a hook by System_VectorToReal5.
 *
 * System_VectorToReal5 stores it in D_0050A9B0, the function word at +0x08 of the options tail.
 */
void SNDREAL_abortmsg(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_VectorToReal5);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDREAL_exithandler(void);
extern void SYNCTASK_add_5478(void *, int, int) __asm__("SYNCTASK_add");
extern void REAL_addexit_5478(void *) __asm__("REAL_addexit");
extern int systaskadded_C464[] __asm__("systaskadded");
extern void (*D_0050A9B0_A9B0[])() __asm__("D_0050A9B0");

/**
 * Hooks the EA sound system into the game's frame and timer services.
 *
 * Stores SNDREAL_abortmsg in the options-tail hook D_0050A9B0, registers SNDREAL_systemtask once as a SYNCTASK (SYNCTASK_add
 * with 0, 1; systaskadded remembers it) and passes SNDREAL_exithandler to REAL_addexit. Called by cPS2SoundManager_Init.
 *
 * @return 0
 */
int System_VectorToReal5(void) {
    D_0050A9B0_A9B0[0] = SNDREAL_abortmsg;
    if (systaskadded_C464[0] == 0) {
        SYNCTASK_add_5478(SNDREAL_systemtask, 0, 1);
        systaskadded_C464[0] = 1;
    }
    REAL_addexit_5478(SNDREAL_exithandler);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYSI_variabletimerservice);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct N_54E0 { struct N_54E0 *next; int pad; void (*fn)(int); int arg; } N_54E0;
typedef struct { char pad[0x1B4]; N_54E0 *list; int pad2[2]; int cnt; } G_54E0;
extern int lastTick_C468[] __asm__("lastTick");

/**
 * Runs the registered notify callbacks when the change counter has moved.
 *
 * Compares the counter at the sound driver state sndgs +0x1C0 with the copy in lastTick; when they differ it
 * updates the copy and calls every node of the list at +0x1B4 as fn(arg). Called by SNDDRV_thread.
 */
void SNDSYSI_variabletimerservice(void) {
    N_54E0 *n;
    G_54E0 *sndState = (G_54E0 *)((int*)&sndgs);

    if (sndState->cnt != lastTick_C468[0]) {
        n = sndState->list;
        lastTick_C468[0] = sndState->cnt;
        if (n != 0) {
            do {
                n->fn(n->arg);
                n = n->next;
            } while (n != 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYSI_timerservice);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern void SNDSYSI_100hzserver_5540(void) __asm__("SNDSYSI_100hzserver");

/**
 * Catches the 100 Hz sound server up with a timer that runs at the given rate.
 *
 * While started: a new rate resets the call count (+0x330) and the tick (+0x180); each call adds one and runs
 * SNDSYSI_100hzserver until the tick passes calls * 100 / rate. Called by SNDDRV_thread.
 *
 * @param timerHertz  the calling timer's rate in Hz
 */
void SNDSYSI_timerservice(unsigned timerHertz) {
    unsigned lim;
    if (sndgs.installed != 0) {
        if ((int)timerHertz != (short)sndgs.currenttimerhz) {
            sndgs.currenttimerhz = timerHertz;
            sndgs.audiotick = 0;
            sndgs.servertick = 0;
        }
        sndgs.servertick++;
        lim = sndgs.servertick * 100 / timerHertz;
        while (lim >= sndgs.audiotick) {
            SNDSYSI_100hzserver_5540();
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYSI_100hzserver);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern void iSNDserve(void);
extern void f_89E0_55F0(int) __asm__("SNDstop");
extern void f_A7B0_55F0(int) __asm__("func_003BA7B0");
extern void f_25E0_55F0(int) __asm__("SNDPLATFORM_setpitch");
extern void f_2A50_55F0(int) __asm__("func_003C2A50");

/**
 * Runs one 100 Hz sound tick: clients, then per-channel modulation, fades and envelopes.
 *
 * Increments the tick (+0x180), calls iSNDserve and each 100 Hz client (+0x184, count +0x177). For every active
 * channel (+0x1F0, 0x8C bytes each, state +0x69 == 1): steps the +0x7C/+0x78 tables (iSNDcalcpitch, SNDPLATFORM_setpitch),
 * moves the volume fade (+0x38 toward +0x34 by +0x30; a fade below 0 stops the voice with SNDstop), the pitch
 * slide (+0x40 by +0x3C), and when the envelope step count (+0x44) runs out advances to the next envelope step (+0x6C,
 * count +0x5C; past the end stops the voice). Changed channels are recomputed with func_003BA7B0 and func_003C2A50.
 */
void SNDSYSI_100hzserver(void) {
    int i;
    int dirty;
    struct CHANPUB *channel;
    struct ENVELOPE *envStep;

    sndgs.audiotick++;
    iSNDserve();
    for (i = 0; i < sndgs.numserverclients100hz; i++) {
        sndgs.serverclient100hz[i]();
    }
    for (i = 0; i < sndgs.voicestotal; i++) {
        channel = &sndgs.chan[i];
        if (channel->status != 1 || channel->handle < 0) {
            continue;
        }
        if (channel->ppitchlfo != 0) {
            if (++channel->curpitchlfoentry >= channel->pitchlfolength) {
                channel->curpitchlfoentry = 0;
            }
            channel->detunepitch = 0;
            iSNDcalcpitch(i);
            f_25E0_55F0(i);
        }
        dirty = 0;
        if (channel->pvollfo != 0) {
            dirty = 1;
            if (++channel->curvollfoentry >= channel->vollfolength) {
                channel->curvollfoentry = 0;
            }
        }
        if (channel->fadePer100Hz != 0) {
            channel->programmedVol += channel->fadePer100Hz;
            dirty = 1;
            if (channel->fadePer100Hz < 0) {
                if (channel->programmedVol <= channel->fadeTargetVol) {
                    channel->programmedVol = channel->fadeTargetVol;
                    channel->fadePer100Hz = 0;
                }
            } else if (channel->programmedVol >= channel->fadeTargetVol) {
                channel->programmedVol = channel->fadeTargetVol;
                channel->fadePer100Hz = 0;
            }
            if (channel->programmedVol < 0) {
                f_89E0_55F0(channel->handle);
                continue;
            }
        }
        channel->envduration--;
        if (channel->envpertick != 0) {
            channel->envvol += channel->envpertick;
            dirty = 1;
        }
        if (channel->envduration == 0) {
            if (++channel->curenvelope >= channel->numenvelopes) {
                f_89E0_55F0(channel->handle);
                continue;
            }
            envStep = &channel->paenvelope[channel->curenvelope];
            channel->envduration = envStep->duration;
            if (channel->envduration < 0) {
                channel->envduration = 0x7FFFFFFF;
            }
            channel->envpertick = ((envStep->targetvol << 16) - channel->envvol) / channel->envduration;
        }
        if (dirty) {
            f_A7B0_55F0(i);
            if (channel->handle >= 0) {
                f_2A50_55F0(i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", DefaultMutexLockFn);
#ifdef SKIP_ASM
extern void SNDI_mutexlock();

/**
 * Locks the sound system's mutex (default lock function).
 *
 * Calls SNDI_mutexlock. gMutexLockFn holds it as the lock function that SNDSYS_entercritical calls.
 */
void DefaultMutexLockFn(void) {
    SNDI_mutexlock();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", DefaultMutexUnlockFn);
#ifdef SKIP_ASM
extern void SNDI_mutexunlock();

/**
 * Unlocks the sound system's mutex (default unlock function).
 *
 * Calls SNDI_mutexunlock. gMutexUnlockFn holds it as the unlock function that SNDSYS_leavecritical calls.
 */
void DefaultMutexUnlockFn(void) {
    SNDI_mutexunlock();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_entercritical);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*gMutexLockFn[])(void);

extern volatile char sndgs_58A0[] __asm__("sndgs");
/**
 * Enters the sound system's critical section.
 *
 * Calls the lock function in gMutexLockFn (DefaultMutexLockFn by default) and increments the lock depth (the sound driver
 * state sndgs +0x176). Paired with SNDSYS_leavecritical around every access to shared sound state, including the movie
 * player's audio calls.
 */
void SNDSYS_entercritical(void) {
    gMutexLockFn[0]();
    sndgs_58A0[0x176]++;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_leavecritical);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*gMutexUnlockFn_58D8[])(void) __asm__("gMutexUnlockFn");

/**
 * Leaves the sound system's critical section.
 *
 * Decrements the lock depth (the sound driver state sndgs +0x176) and calls the unlock function in gMutexUnlockFn
 * (DefaultMutexUnlockFn by default).
 */
void SNDSYS_leavecritical(void) {
    sndgs.incritical--;
    gMutexUnlockFn_58D8[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_add100hzclient);
#ifdef SKIP_ASM

/**
 * Adds a client to the sound system's 100 Hz client list.
 *
 * Only while started; appends to the sound driver state sndgs +0x184 and increments the count (+0x177) without a
 * bound check. SNDSYSI_100hzserver calls each client every tick. Used by cMonitor_MONITOR_BeginMonitoring__FPv, SNDmidiinit
 * and func_002AE048.
 *
 * @param client  the client function
 */
void SNDSYS_add100hzclient(int client) {
    if (sndgs.installed != 0) {
        sndgs.serverclient100hz[sndgs.numserverclients100hz] = (void (*)(void))client;
        sndgs.numserverclients100hz++;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_remove100hzclient);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_5948 { char pad[0x174]; char on; char p1; char p2; signed char n; char pad2[12]; int arr[8]; };
extern void SNDSYS_entercritical();
extern void SNDSYS_leavecritical();

extern struct S_5948 D_a8e8_5948 __asm__("sndgs");
/**
 * Removes a client from the sound system's 100 Hz client list.
 *
 * Only while started, inside the critical section; shifts the later entries down. Used by cMonitor__cMonitor,
 * SNDmidirestore and func_002AE100.
 *
 * @param client  the client function
 */
void SNDSYS_remove100hzclient(int client) {
    int i;
    if (D_a8e8_5948.on) {
        SNDSYS_entercritical();
        for (i = 0; i < D_a8e8_5948.n; i++) {
            if (D_a8e8_5948.arr[i] == client) {
                D_a8e8_5948.n--;
                for (; i < D_a8e8_5948.n; i++) D_a8e8_5948.arr[i] = D_a8e8_5948.arr[i + 1];
                break;
            }
        }
        SNDSYS_leavecritical();
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_adduserdataclient);
#ifdef SKIP_ASM

/**
 * Adds a callback to the sound system's user-data client list.
 *
 * Only while started; appends to the sound driver state sndgs +0x1C8 (count +0x179, room for 4) without a bound
 * check. The clients get user-data event records from SNDBANKI_userdatacallback and the stream parser SNDSTRMI_parseheader.
 * cSSXAudio_cSSXAudio registers cSSXAudio_HandleUserChunk with it.
 *
 * @param client  the callback, called with an event record
 */
void SNDSYS_adduserdataclient(int client) {
    if (sndgs.installed != 0) {
        sndgs.userdataclient[sndgs.numuserdataclients] = (void (*)(void*))client;
        sndgs.numuserdataclients++;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_removeuserdataclient);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_5A60 { char pad[0x179]; signed char cnt; char pad2[0x4E]; int list[1]; };

extern struct G_5A60 sndgs_5A60[] __asm__("sndgs");
/**
 * Removes a callback from the sound system's user-data client list.
 *
 * Inside the critical section; shifts the later entries down. cSSXAudio__cSSXAudio removes cSSXAudio_HandleUserChunk with it.
 *
 * @param client  the callback
 */
void SNDSYS_removeuserdataclient(int client) {
    int i;
    SNDSYS_entercritical();
    for (i = 0; i < sndgs_5A60[0].cnt; i++) {
        if (sndgs_5A60[0].list[i] == client) {
            sndgs_5A60[0].cnt--;
            if (i < sndgs_5A60[0].cnt) {
                do {
                    sndgs_5A60[0].list[i] = sndgs_5A60[0].list[i + 1];
                    i++;
                } while (i < sndgs_5A60[0].cnt);
            }
            break;
        }
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5B20);
#ifdef SKIP_ASM
extern void SNDSYS_entercritical();
extern void SNDSYS_leavecritical();
extern int SNDPLATFORM_memlimits(int, int);

/**
 * Sets the address range of sound memory the driver may use.
 *
 * Calls SNDPLATFORM_memlimits inside the critical section. Called by cPS2SoundManager_SetSoundMemLoadLimits,
 * cPS2SoundManager_RestoreSoundMemLoadLimits, cPS2SoundManager_AllocSoundMem, cPS2SoundManager_Shutdown and
 * func_002A92F8.
 *
 * @param startAddr  first usable address
 * @param endAddr  end of the usable range
 * @return SNDPLATFORM_memlimits's result
 */
int func_003B5B20(int startAddr, int endAddr) {
    int result;

    SNDSYS_entercritical();
    result = SNDPLATFORM_memlimits(startAddr, endAddr);
    SNDSYS_leavecritical();
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B5B68);
#ifdef SKIP_ASM
extern void SNDSYS_entercritical();
extern void SNDSYS_leavecritical();
extern int SNDPLATFORM_memlargestunused(int);

/**
 * Finds the largest unused block of sound memory.
 *
 * Calls SNDPLATFORM_memlargestunused inside the critical section. Called by cPS2SoundManager_Init and cPS2SoundManager_AllocSoundMem.
 *
 * @param outAddr  receives the block's address
 * @return the block's size
 */
int func_003B5B68(int outAddr) {
    int largest;

    SNDSYS_entercritical();
    largest = SNDPLATFORM_memlargestunused(outAddr);
    SNDSYS_leavecritical();
    return largest;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEMI_constrain);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_5BA0 { char pad[0xC]; unsigned int lim; };
/**
 * Shrinks a free span so that it ends at the sound heap's limit.
 *
 * Clamps the space so that address + space does not pass the heap state's end (+0xC of D_0050AB1C). Called by
 * SNDMEMI_alloc for every candidate gap.
 *
 * @param freeAddr  in: the span's heap offset
 * @param freeSpace  in/out: the span's size
 */
void SNDMEMI_constrain(int *freeAddr, int *freeSpace) {
    int x = *freeAddr;
    unsigned int lim = ((struct G_5BA0*)sndgs.mm)->lim;
    if (lim < (unsigned int)(x + *freeSpace))
        *freeSpace = lim - x;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEMI_init);
#ifdef SKIP_ASM
struct G_5BD0 { char pad[0x234]; struct SNDMEMSTATE *cur; };

extern struct G_5BD0 sndgs_5BD0 __asm__("sndgs");
/**
 * Sets up the sound system's memory heap in a buffer.
 *
 * Uses the start of the buffer as the heap state (the sound driver state sndgs +0x234): the data start 16-byte
 * aligned after 0x18 bytes, the record table growing down from the end - 8, the size, the free end (size - 15 - 0x20)
 * and the low-water mark (size - 15). Called by SNDSYSI_init.
 *
 * @param heap  the buffer
 * @param heapSize  its size in bytes
 */
void SNDMEMI_init(struct SNDMEMSTATE *heap, int heapSize) {
    sndgs_5BD0.cur = heap;
    heap->heapsize = heapSize;
    (*(int*)&sndgs_5BD0.cur->r) = (int)heap + heapSize - 8;
    heapSize -= 0xF;
    (*(int*)&sndgs_5BD0.cur->pheap) = (int)heap + 0x18;
    (*(int*)&sndgs_5BD0.cur->pheap) += 0xF;
    (*(int*)&sndgs_5BD0.cur->pheap) &= 0xFFFFFFF0;
    sndgs_5BD0.cur->endaddr = heapSize - 0x20;
    sndgs_5BD0.cur->lowmark = heapSize;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEMI_restore);
#ifdef SKIP_ASM
extern void SNDMEM_gethighwater();

/**
 * Releases the sound system's memory (forwards to SNDMEM_gethighwater).
 *
 * Called by SNDSYS_restore during shutdown; the result passes through in $v0 although it is declared void.
 */
void SNDMEMI_restore(void) {
    SNDMEM_gethighwater();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEMI_alloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* SNDMEMI_alloc (EA snd library, smemman.c; On Tour's SNDMEMI_allocz minus its debug checks) */
struct SNDMEMSTATE_5C60 {
    char *pheap;
    struct SNDMEMREC *r;   /* grows down from endaddr; indexed 0, -1, -2, ... */
    int heapsize;
    unsigned int endaddr;
    int lowmark;
    int nummallocs;             /* counts down */
};
struct sndgs_5C60 { char pad[0x234]; struct SNDMEMSTATE_5C60 *pmem; };
extern void SNDMEMI_constrain_5C60(unsigned int *pfreeaddr, int *pfreespace) __asm__("SNDMEMI_constrain");

void *SNDMEMI_alloc_5C60(int size) __asm__("SNDMEMI_alloc");
/**
 * Allocates a 16-byte-aligned block from the sound heap.
 *
 * First fit over the gaps between the allocation records (sorted, stored downward from the end of the heap), each gap
 * clamped with SNDMEMI_constrain; the new record is inserted in order, the free end shrinks by 8 for it and the low-water
 * mark (+0x10) is updated. Used by SNDSYSI_init and the driver's table and voice setup.
 *
 * @param size  bytes wanted (rounded up to 16)
 * @return the block, or 0 when nothing fits
 */
void *SNDMEMI_alloc_5C60(int size) {
    struct SNDMEMREC *pcurrec;
    struct SNDMEMREC *pprevrec;
    void *allocated;
    unsigned int freeaddr;
    int freespace;
    int lowmark;
    int i = 0;
    int j;

    size += 15;
    size &= ~15;
    if (((struct SNDMEMSTATE_5C60*)sndgs.mm)->nummallocs == 0) {
        freeaddr = 0;
        freespace = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->endaddr;
        SNDMEMI_constrain_5C60(&freeaddr, &freespace);
        if (freespace >= size) goto alloc;
        else goto fail;
    } else {
        for (i = 0; ((struct SNDMEMSTATE_5C60*)sndgs.mm)->nummallocs < i; i--) {
            pcurrec = &((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[i];
            if (i == 0) {
                freeaddr = 0;
                freespace = pcurrec->addr;
            } else {
                pprevrec = &((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[i + 1];
                freeaddr = pprevrec->addr + pprevrec->size;
                freespace = pcurrec->addr - freeaddr;
            }
            SNDMEMI_constrain_5C60(&freeaddr, &freespace);
            if (freespace >= size) {
                for (j = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->nummallocs; j < i; j++) {
                    ((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[j] = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[j + 1];
                }
                goto alloc;
            }
        }
        pprevrec = &((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[i + 1];
        freeaddr = pprevrec->addr + pprevrec->size;
        freespace = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->endaddr - freeaddr;
        SNDMEMI_constrain_5C60(&freeaddr, &freespace);
        if (freespace < size) {
            goto fail;
        }
    }
alloc:
    pcurrec = &((struct SNDMEMSTATE_5C60*)sndgs.mm)->r[i];
    pcurrec->addr = freeaddr;
    pcurrec->size = size;
    ((struct SNDMEMSTATE_5C60*)sndgs.mm)->nummallocs--;
    ((struct SNDMEMSTATE_5C60*)sndgs.mm)->endaddr -= sizeof(struct SNDMEMREC);
    allocated = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->pheap + freeaddr;
    lowmark = ((struct SNDMEMSTATE_5C60*)sndgs.mm)->endaddr - (freeaddr + size);
    if (lowmark < ((struct SNDMEMSTATE_5C60*)sndgs.mm)->lowmark) {
        ((struct SNDMEMSTATE_5C60*)sndgs.mm)->lowmark = lowmark;
    }
    return allocated;
fail:
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEMI_free);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SNDMEMI_free_5E98(int) __asm__("SNDMEMI_free");
struct G_5E98 { char pad[0x234]; struct SNDMEMSTATE *s; };

/**
 * Frees a block from the sound heap.
 *
 * Finds the record whose offset matches (address - heap start), removes it, closes the gap in the record table and
 * gives its 8 bytes back to the free end. An unknown address is ignored.
 *
 * @param address  the block's address
 */
void SNDMEMI_free_5E98(int address) {
    int i = 0;
    int heapOffset = address - ((int)((struct SNDMEMSTATE*)sndgs.mm)->pheap);
    if (((struct SNDMEMSTATE*)sndgs.mm)->nummallocs >= 0) return;
    while (1) {
        struct SNDMEMSTATE *memState = (struct SNDMEMSTATE*)sndgs.mm;
        if (((int)((struct SNDMEMREC *)((char *)((struct SNDMEMREC*)memState->r) + (i << 3)))->addr) == heapOffset) {
            memState->nummallocs++;
            (*(int*)&((struct SNDMEMSTATE*)sndgs.mm)->endaddr) += 8;
            if (!(((struct SNDMEMSTATE*)sndgs.mm)->nummallocs < i)) return;
            do {
                ((struct SNDMEMREC*)((struct SNDMEMSTATE*)sndgs.mm)->r)[i] = ((struct SNDMEMREC*)((struct SNDMEMSTATE*)sndgs.mm)->r)[i - 1];
                i--;
            } while (((struct SNDMEMSTATE*)sndgs.mm)->nummallocs < i);
            return;
        }
        i--;
        if (!(memState->nummallocs < i)) return;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPROFILE_outputlatency);
#ifdef SKIP_ASM
int SNDPLATFORM_outputlatency(void);

/**
 * Returns the sound output latency, or 0 when the sound system is not started.
 *
 * Forwards to SNDPLATFORM_outputlatency. The movie player's func_003AE9A0 subtracts it from the audio play position.
 *
 * @return the latency (in ms, as func_003AE9A0 uses it)
 */
int SNDPROFILE_outputlatency(void) {
    int latencyMs;
    if (*(char*)&sndgs.installed != 0) {
        latencyMs = SNDPLATFORM_outputlatency();
    } else {
        latencyMs = 0;
    }
    return latencyMs;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_userdatacallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct S_5F98 { char pad[0x17]; signed char n; char pad2[0x1C]; int a[4]; int b[4]; };
struct M_5F98 { int arg; int a; int b; int pad[5]; };
struct G_5F98 { char pad[0x179]; signed char cnt; char pad2[0x1C8 - 0x17A]; void (*fn[1])(struct M_5F98 *); };

extern struct G_5F98 sndgs_5F98 __asm__("sndgs");
/**
 * Hands a channel's pending user-data events to the user-data clients.
 *
 * Pops the pending pairs (count at +0x17, values at +0x34 and +0x44) last first and calls every client of the sound
 * driver state sndgs +0x1C8 with a record {arg, first value, second value}.
 *
 * @param source  the source holding the pending events
 * @param eventArg  passed as the record's first word
 */
void SNDBANKI_userdatacallback(struct S_5F98 *source, int eventArg) {
    struct M_5F98 eventRec;
    int i;

    while (source->n > 0) {
        source->n--;
        eventRec.arg = eventArg;
        eventRec.a = source->a[source->n];
        eventRec.b = source->b[source->n];
        for (i = 0; i < sndgs_5F98.cnt; i++) {
            sndgs_5F98.fn[i](&eventRec);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDbankadd);
#ifdef SKIP_ASM
struct Ent_6098 { struct BANKVER5 *hdr; char *buf1; char *buf2; char busy; };
struct G_6098 { char pad[0x1E4]; void (*cb)(); char pad2[0x1F4 - 0x1E8]; struct Ent_6098 *ents; };
extern void SNDbankremove_6098() __asm__("SNDbankremove");
extern int f_6500_6098(void) __asm__("SNDBANKI_alloc");
extern char *f_4B70_6098(int, int) __asm__("SNDPLATFORM_memalloc");
extern int f_42D8_6098(int, char *, char *, int) __asm__("SNDPLATFORM_download");
extern int f_42F8_6098(int) __asm__("SNDPLATFORM_downloadcomplete");
extern int f_6788_6098(int, char *, char *, int *) __asm__("func_003B6788");

extern struct G_6098 sndgs_6098 __asm__("sndgs");
/**
 * Adds a sound bank: takes a bank slot and uploads its sample data to sound memory.
 *
 * Installs SNDbankremove as the bank-removal hook (+0x1E4), takes a slot with SNDBANKI_alloc and records the header.
 * Each of the two sample sections (sizes at +0x10 for memory kind 0x100 and +0x0C for kind 8, data at the header's
 * offset +0x08) gets memory from SNDPLATFORM_memalloc (plus 0x40 / 0x30 bytes) and is copied in 4 KB pieces with
 * SNDPLATFORM_download, polling SNDPLATFORM_downloadcomplete until each piece is done; then every patch (self-relative offsets at +0x14,
 * count +0x06) is resolved against it with func_003B6788.
 *
 * @param out  receives the bank handle
 * @param hdr  the bank in main memory
 * @return 7 when sample data was uploaded, 8 for a bank without any, -6 when memory runs out
 */
int SNDbankadd(int *out, struct BANKVER5 *hdr) {
    struct Ent_6098 *ent;
    int i, n, xferHandle;
    int firstOffset;

    sndgs_6098.cb = SNDbankremove_6098;
    *out = f_6500_6098();
    ent = &sndgs_6098.ents[*out];
    ent->busy = 1;
    ent->hdr = hdr;
    ent->buf1 = 0;
    if (hdr->iopcpusize != 0) {
        ent->buf1 = f_4B70_6098(0x100, hdr->iopcpusize + 0x40);
        if (ent->buf1 == 0) {
            goto fail;
        }
        for (i = 0; i < hdr->iopcpusize; i += 0x1000) {
            n = hdr->iopcpusize - i;
            xferHandle = f_42D8_6098(0x100, (char *)hdr + hdr->hdrsize + i, ent->buf1 + i, n < 4097 ? n : 0x1000);
            while (f_42F8_6098(xferHandle) == 0) {
            }
        }
        firstOffset = 0;
        for (i = 0; i < hdr->numpatches; i++) {
            if (((int*)&hdr->patch[0])[i] != 0) {
                f_6788_6098(0x100, (char *)&((int*)&hdr->patch[0])[i] + ((int*)&hdr->patch[0])[i], ent->buf1, &firstOffset);
            }
        }
    }
    ent->buf2 = 0;
    if (hdr->spusize != 0) {
        ent->buf2 = f_4B70_6098(8, hdr->spusize + 0x30);
        if (ent->buf2 == 0) {
fail:
            ent->hdr = 0;
            ent->busy = 0;
            return -6;
        }
        for (i = 0; i < hdr->spusize; i += 0x1000) {
            n = hdr->spusize - i;
            xferHandle = f_42D8_6098(8, (char *)hdr + hdr->hdrsize + hdr->iopcpusize + i, ent->buf2 + i, n < 4097 ? n : 0x1000);
            while (f_42F8_6098(xferHandle) == 0) {
            }
        }
        firstOffset = 0;
        for (i = 0; i < hdr->numpatches; i++) {
            if (((int*)&hdr->patch[0])[i] != 0) {
                f_6788_6098(8, (char *)&((int*)&hdr->patch[0])[i] + ((int*)&hdr->patch[0])[i], ent->buf2, &firstOffset);
            }
        }
    }
    ent->busy = 0;
    if (hdr->spusize != 0 || hdr->iopcpusize != 0) {
        return 7;
    }
    return 8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDbankremove);
#ifdef SKIP_ASM
struct R_6300 {
    void *p0;
    int p4;
    int p8;
    char fC;
};
extern void SNDSYS_entercritical_6300() __asm__("SNDSYS_entercritical");
extern void SNDSYS_leavecritical_6300() __asm__("SNDSYS_leavecritical");
extern void SNDBANKI_userdatacallback_6300(void *buf, int n) __asm__("SNDBANKI_userdatacallback");
extern int SNDBANKI_valid_6300(int i) __asm__("SNDBANKI_valid");
extern void SNDstop_6300(void *p) __asm__("SNDstop");
extern int SNDI_parsetimbre_6300(char **pp, void *buf) __asm__("SNDI_parsetimbre");
extern void SNDPLATFORM_memfree_6300(int a, int p) __asm__("SNDPLATFORM_memfree");
extern char *SNDBANKI_getppatch_6300(void *p, int i) __asm__("SNDBANKI_getppatch");

/**
 * Removes a sound bank (or all banks for -1): stops its voices and frees its sample memory.
 *
 * Fails with -8 when SNDBANKI_valid rejects the handle. In the critical section it stops every channel playing from the
 * bank (SNDstop), sends each patch's user data with type 2 (SNDI_parsetimbre, SNDBANKI_userdatacallback), frees the two sample
 * areas with SNDPLATFORM_memfree (kinds 0x100 and 8) and clears the slot.
 *
 * @param bankHandle  the bank handle, or -1 for every slot
 * @return 0, or -8 for an invalid or busy bank
 */
int SNDbankremove(int bankHandle) {
    char buf[208];
    char *patchData;
    void *obj;
    int i;
    int moreTags;

    if (bankHandle == -1) {
        for (i = 0; i < sndgs.sso.set.maxbanks; i++) {
            SNDbankremove(i);
        }
        return 0;
    }
    if (SNDBANKI_valid_6300(bankHandle) != 0) {
        return -8;
    }
    SNDSYS_entercritical_6300();
    obj = ((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p0;
    for (i = 0; i < sndgs.voicestotal; i++) {
        struct CHANPUB *channel = &((struct CHANPUB*)(struct CHANPUB*)sndgs.chan)[i];
        if (channel->bhandle == bankHandle) {
            SNDstop_6300(((void*)channel->handle));
        }
    }
    for (i = 0; i < *(unsigned short *)((char *)obj + 6); i++) {
        patchData = SNDBANKI_getppatch_6300(obj, i);
        if (patchData != 0) {
            patchData += 4;
            do {
                moreTags = SNDI_parsetimbre_6300(&patchData, buf);
                SNDBANKI_userdatacallback_6300(buf, 2);
            } while (moreTags != 0);
        }
    }
    if (((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p4 != 0) {
        SNDPLATFORM_memfree_6300(0x100, ((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p4);
    }
    if (((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p8 != 0) {
        SNDPLATFORM_memfree_6300(8, ((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p8);
    }
    ((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].p0 = 0;
    ((struct R_6300*)(struct BANKLIST*)sndgs.banklist)[bankHandle].fC = 0;
    SNDSYS_leavecritical_6300();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_valid);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
/**
 * Checks a bank handle: in range, loaded and not busy.
 *
 * Looks at the slot in the sound driver state sndgs +0x1F4 (count at +0x08 of the options).
 *
 * @param bankHandle  the bank handle
 * @return 0 when usable, -18 when the slot is busy, -8 otherwise
 */
int SNDBANKI_valid(int bankHandle) {
    struct BANKLIST *slot;
    if (bankHandle < 0 || bankHandle >= sndgs.sso.set.maxbanks) {
        return -8;
    }
    slot = (struct BANKLIST *)(bankHandle * 16 + (int)(struct BANKLIST*)sndgs.banklist);
    if (((struct sA8E8Bank*)slot->phdr) != 0) {
        return slot->locked ? -18 : 0;
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDbankheadersize);
#ifdef SKIP_ASM

/**
 * Returns the header size of a loaded bank.
 *
 * Reads +0x08 of the bank header in slot arg0 (no validation). Called by SNDbankheadercopy.
 *
 * @param bankHandle  the bank handle
 * @return the header size in bytes
 */
int SNDbankheadersize(int bankHandle) {
    return sndgs.banklist[bankHandle].phdr->hdrsize;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDbankheadercopy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Passes a and slot n's old data to func_0041610C, then stores a in the slot.
extern int SNDbankheadersize_6548(int) __asm__("SNDbankheadersize");
extern int func_0041610C_6548(int, int, int) __asm__("func_0041610C");
struct G_6548 { char p[0x1F4]; struct BANKLIST *slots; };
/**
 * Copies a bank's header to new memory and makes the bank use the copy.
 *
 * Gets the size with SNDbankheadersize, copies with func_0041610C(dest, old header, size) and stores the destination in the
 * slot.
 *
 * @param destMem  where to copy the header
 * @param n  the bank handle
 * @return 0, or SNDbankheadersize's negative result
 */
int SNDbankheadercopy(int destMem, int n) {
    int result = 0;
    int x = SNDbankheadersize_6548(n);
    if (x < 0) {
        result = x;
    } else {
        func_0041610C_6548(destMem, ((int)sndgs.banklist[n].phdr), x);
        (*(int*)&sndgs.banklist[n].phdr) = destMem;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDbankinuse);
#ifdef SKIP_ASM
struct G_65D0 { char p[0x17E]; short n; char p2[0x1F0 - 0x180]; struct CHANPUB *tab; };
extern int SNDBANKI_valid_65D0(int) __asm__("SNDBANKI_valid");
extern int SNDover_65D0(int) __asm__("SNDover");
/**
 * Tells whether any channel is still playing from a bank.
 *
 * Validates with SNDBANKI_valid, then in the critical section looks for a channel of the bank (+0x12) whose voice
 * SNDover reports active (0). Used by the game's bank manager (bankmonitor.cpp) before reusing a slot.
 *
 * @param id  the bank handle
 * @return 1 when in use, 0 when not, -8 for an invalid bank
 */
int SNDbankinuse(int id) {
    int found = 0;
    int i;
    if (SNDBANKI_valid_65D0(id) != 0) return -8;
    SNDSYS_entercritical();
    for (i = 0; i < sndgs.voicestotal; i++) {
        struct CHANPUB *channel = &sndgs.chan[i];
        if (channel->bhandle == id && SNDover_65D0(channel->handle) == 0) {
            found = 1;
            break;
        }
    }
    SNDSYS_leavecritical();
    return found;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B6670);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDBANKI_valid_6670() __asm__("SNDBANKI_valid");
extern void SNDI_patchtohdr_6670(int, int, SNDSAMPLEFORMAT *, SNDSAMPLEATTR *, int, int) __asm__("SNDI_patchtohdr");
extern int SNDBANKI_getppatch_6670(int, int) __asm__("SNDBANKI_getppatch");

/**
 * Reads a bank patch's sample format, attributes and description without keeping its data.
 *
 * Validates the bank, finds the patch with SNDBANKI_getppatch (-8 when missing), and in the critical section has
 * SNDI_patchtohdr fill the three records; then frees the time-stretch data copies SNDI_patchtohdr made (+0x18 + 4n of the
 * attributes, one per channel) with SNDMEMI_free and clears them and their sizes (+0x30 + 4n). Used by the game's bank
 * monitor.
 *
 * @param bankHandle  the bank handle
 * @param patchNum  the patch number
 * @param formatOut  receives the sample format (+2 is the channel count)
 * @param attrOut  receives the sample attributes
 * @param descOut  receives the sample description
 * @return 0, or -8
 */
int func_003B6670(int bankHandle, int patchNum, SNDSAMPLEFORMAT* formatOut, SNDSAMPLEATTR *attrOut, int descOut) {
    int off, patch, i;
    int *chanBuf;
    if (SNDBANKI_valid_6670() < 0) return -8;
    off = bankHandle * 0x10;
    patch = SNDBANKI_getppatch_6670(*(int *)(off + ((int)(struct BANKLIST*)sndgs.banklist)), patchNum);
    if (patch == 0) return -8;
    i = 0;
    SNDSYS_entercritical();
    SNDI_patchtohdr_6670(*(int *)(off + ((int)(struct BANKLIST*)sndgs.banklist)), patch, formatOut, attrOut, descOut, 0);
    if (formatOut->channels != 0) {
        chanBuf = (int *)attrOut->ptsdata;
        do {
            if (*chanBuf != 0) {
                SNDMEMI_free(*chanBuf);
                *chanBuf = 0;
                chanBuf[6] = 0;
            }
            i++;
            chanBuf++;
        } while (i < formatOut->channels);
    }
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B6788);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDBANKI_userdatacallback_6788(void *, int) __asm__("SNDBANKI_userdatacallback");
extern int SNDI_parsetimbre_6788(char **, void *) __asm__("SNDI_parsetimbre");
extern void SNDPLATFORM_asyncresolvetimbre_6788(void *, int, int) __asm__("SNDPLATFORM_asyncresolvetimbre");

/**
 * Resolves a patch's sample addresses against uploaded sample data and sends its user data.
 *
 * Walks the patch's tags (SNDI_parsetimbre into a 208-byte header); for each whose render bits (+0x18) match the mask it
 * resolves the sample with SNDPLATFORM_asyncresolvetimbre and dispatches its user data with type 0 (SNDBANKI_userdatacallback). Called by
 * SNDbankadd.
 *
 * @param renderMask  which render kinds to resolve (0x100 or 8)
 * @param taggedPatch  the patch
 * @param sampleData  where the samples were uploaded
 * @param firstOffset  in/out offset passed to SNDPLATFORM_asyncresolvetimbre
 * @return 0
 */
int func_003B6788(int renderMask, char *taggedPatch, int sampleData, int firstOffset) {
    char sp[208];
    char *cursor;
    int moreTags;

    cursor = taggedPatch + 4;
    do {
        moreTags = SNDI_parsetimbre_6788(&cursor, sp);
        if (*(unsigned short *)(sp + 0x18) & renderMask) {
            SNDPLATFORM_asyncresolvetimbre_6788(sp, sampleData, firstOffset);
            SNDBANKI_userdatacallback_6788(sp, 0);
        }
    } while (moreTags != 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANK_play);
#ifdef SKIP_ASM
extern int SNDBANKI_valid_6820(int) __asm__("SNDBANKI_valid");
extern int SNDBANKI_getppatch_6820(void *, int) __asm__("SNDBANKI_getppatch");
extern int SNDBANKI_playpatch_6820(void *, int, int, int, int) __asm__("SNDBANKI_playpatch");
/**
 * Plays a patch from a bank.
 *
 * Validates the bank (-8) and the patch number against the bank's count (+0x06), then calls SNDBANKI_playpatch with the
 * bank, the patch (SNDBANKI_getppatch) and the arguments. Used by the game's stream system and bank monitor.
 *
 * @param bankHandle  the bank handle
 * @param patchNum  the patch number
 * @param playOpts  the play options
 * @return the sound handle from SNDBANKI_playpatch, or -8
 */
int SNDBANK_play(int bankHandle, int patchNum, int playOpts) {
    struct BANKVER5 *ch;
    if (SNDBANKI_valid_6820(bankHandle) < 0) return -8;
    ch = sndgs.banklist[bankHandle].phdr;
    if (patchNum < 0 || patchNum >= ch->numpatches) return -8;
    return SNDBANKI_playpatch_6820(ch, SNDBANKI_getppatch_6820(ch, patchNum), bankHandle, patchNum, playOpts);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_startstream);
#ifdef SKIP_ASM
extern int SNDCTRL_filteradd(int, int);
extern int SNDPKTPLAY_start(int, int, int, int);

/**
 * Starts a stream's voice with its current format and header.
 *
 * Gets a voice from SNDPKTPLAY_start (queue +0x8, format +0x14, header +0x1C, parameters +0xEC) into +0x4, adds the first
 * track's setting with SNDCTRL_filteradd when its word (+0x114) is set, and sets the state (+0x10) to 1. Quirk: the track
 * loop runs exactly once.
 *
 * @param stream  the stream (0x138 bytes)
 */
void SNDSTRMI_startstream(char *stream) {
    int i = 0;
    char *trackEntry = stream + 0x104;
    int *trackFlag = (int*)(stream + 0x114);
    *(volatile int *)(stream + 4) = SNDPKTPLAY_start(*(volatile int *)(stream + 8), (int)(stream + 0x14), (int)(stream + 0x1C), (int)(stream + 0xEC));
    do {
        if (*trackFlag != 0) {
            SNDCTRL_filteradd(*(volatile int *)(stream + 4), (int)trackEntry);
        }
        trackEntry += 0x18;
        i++;
        trackFlag += 6;
    } while (i == 0);
    *(volatile char *)(stream + 0x10) = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_calcdatarate);
#ifdef SKIP_ASM
/**
 * Returns a sample format's data rate in bytes per second.
 *
 * rate * channels * ratio / 256, the ratio depending on the codec byte (+3): 5 gives 146, 10 gives 136, 4 gives 51, 22
 * gives 102, 8 gives 512 (16-bit PCM); codecs 16 and 23 return channels * 8; others 0.
 *
 * @param sampleFormat  rate (u16), channels, codec
 * @return bytes per second
 */
int SNDSTRMI_calcdatarate(unsigned char *sampleFormat) {
    int ratio = 0;
    int w = *(unsigned short *)sampleFormat;
    int numChannels = sampleFormat[2];
    int sampleRep = sampleFormat[3];
    w = w * numChannels;
    if (sampleRep == 5) ratio = 146;
    else if (sampleRep == 10) ratio = 136;
    else if (sampleRep == 4) ratio = 51;
    else if (sampleRep == 22) ratio = 102;
    else if (sampleRep == 8) ratio = 0x200;
    else if (sampleRep == 16) return numChannels << 3;
    else if (sampleRep == 23) return numChannels << 3;
    return (w * ratio) >> 8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_destroyall);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x46]; unsigned char cnt; } G_69D8;
extern void SNDSTRM_destroy(int);

/**
 * Destroys every stream (SNDSTRM_destroy on each).
 *
 * The stream count is the byte at +0x46 of the sound driver state sndgs.
 *
 * @return 0
 */
int SNDSTRMI_destroyall(void) {
    int i;

    i = 0;
    if (sndgs.sso.set.maxstreams != 0) {
        do {
            SNDSTRM_destroy(i);
            i += 1;
        } while (i < (int)sndgs.sso.set.maxstreams);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_getstreamptr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int sndss[];
extern unsigned char D_0050A92E_6A38[] __asm__("D_0050A92E");

/**
 * Returns the stream for a stream slot number.
 *
 * Looks the slot up in sndss; the count is the halfword D_0050A92E.
 *
 * @param slotNum  the slot number (low byte of a stream handle)
 * @return the stream, or 0 when out of range
 */
int SNDSTRMI_getstreamptr(int slotNum) {
    if (slotNum >= D_0050A92E_6A38[0] || slotNum < 0) {
        return 0;
    }
    return sndss[slotNum];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_removerequest);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char *sndss_6A70[] __asm__("sndss");
extern int SNDSTRMI_getrequestptr_6A70(int) __asm__("SNDSTRMI_getrequestptr");
extern void SNDLINKI_remove_6A70(void *, int) __asm__("SNDLINKI_remove");
extern void SNDLINKI_push_6A70(void *, int) __asm__("SNDLINKI_push");
/**
 * Retires a stream request: unlinks it from the queue and returns it to the free list.
 *
 * The stream is chosen by the handle's low byte; the request comes from SNDSTRMI_getrequestptr. It is removed from the queued
 * list (+0x11C, SNDLINKI_remove), added to the free list (+0x128, SNDLINKI_push) and cleared as current (+0x134).
 *
 * @param requestHandle  the request handle
 */
void SNDSTRMI_removerequest(int requestHandle) {
    char *stream = sndss_6A70[requestHandle & 0xFF];
    int request = SNDSTRMI_getrequestptr_6A70(requestHandle);
    SNDLINKI_remove_6A70(stream + 0x11C, request);
    SNDLINKI_push_6A70(stream + 0x128, request);
    if (*(int *)(stream + 0x134) == request) *(int *)(stream + 0x134) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_releasecallback);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void STREAM_release_6AD8(int, unsigned char *) __asm__("STREAM_release");
extern int *sndss_6AD8[] __asm__("sndss");

/**
 * Gives a played stream data block back to its stream's file queue.
 *
 * The block's header pointer sits just before the samples; its first byte is the stream slot. Calls STREAM_release with
 * that stream's queue (first word).
 *
 * @param samples  the sample data pointer
 */
void SNDSTRMI_releasecallback(void *samples) {
    unsigned char *chunkHdr = ((unsigned char **)samples)[-1];
    STREAM_release_6AD8(*sndss_6AD8[*chunkHdr], chunkHdr);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_framescallback);
#ifdef SKIP_ASM
struct C_6B10 { char pad[0x11C]; struct SNDSTREAMREQUEST *q; };

/**
 * Advances a stream's requests by the amount just played and retires finished ones.
 *
 * Adds the amount to the current request's position and takes it off its remainder; a request whose position reaches
 * its end is retired with SNDSTRMI_removerequest, and any overflow carries on to the next request. After 200 passes it retires
 * the current request and stops.
 *
 * @param packetChan  unused
 * @param n  the amount played
 * @param stream  the stream
 */
void SNDSTRMI_framescallback(int packetChan, unsigned int n, struct C_6B10 *stream) {
    unsigned int rem = 0;
    int i = 0;
    while (1) {
        struct SNDSTREAMREQUEST *request;
        i++;
        request = stream->q;
        if (i >= 201) {
            SNDSTRMI_removerequest(request->sndrequesthandle);
            return;
        }
        if (request->outstandingframes < n) {
            rem = n - request->outstandingframes;
            n -= rem;
        }
        request->currentframe += n;
        request->outstandingframes -= n;
        if (request->currentframe >= ((volatile unsigned int)request->totalframes)) {
            SNDSTRMI_removerequest(request->sndrequesthandle);
        }
        if (rem == 0) return;
        n = rem;
        rem = 0;
    }
}
#endif

#ifdef SKIP_ASM
/* sndss: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. Shared by SNDSTRMI_parseheader .. func_003B8050. */
struct sA0A0Node {                  /* 0x28: a queued stream request */
    struct sA0A0Node *f00;          /* next */
    int f04;
    int f08;
    volatile int f0C;               /* handle */
    volatile unsigned int f10;
    volatile unsigned int f14;
    volatile unsigned int f18;
    volatile unsigned int f1C;
    volatile int f20;
    unsigned char f24;
    char f25[0x28 - 0x25];
};
struct sA0A0List {                  /* 0xC */
    struct sA0A0Node *f00;          /* first */
    struct sA0A0Node *f04;
    int f08;                        /* count */
};
struct sA0A0Fmt {                   /* 4 bytes, copied as a whole: rate (u16), channels, codec (SNDSTRMI_calcdatarate) */
    unsigned char b[4];
};
struct sA0A0Hdr {                   /* 0x68 */
    char pad00[0x18];
    int f18[6];                     /* one per channel */
    char pad30[0x48 - 0x30];
    int f48[4];
    int f58[4];
};
struct sA0A0Parm {                  /* 0x18, handed to SNDPKTPLAY_start */
    signed char f00;
    signed char f01;
    char pad02[0x4 - 0x2];
    char f04;
    char f05;
    char pad06[0x8 - 0x6];
    short f08;
    short f0A;
    short f0C;
    short f0E;
    char pad10[0x14 - 0x10];
    short f14;
    char pad16[0x18 - 0x16];
};
struct sA0A0Trk {                   /* 0x18 */
    char pad00[0x10];
    int f10;
    char pad14[0x18 - 0x14];
};
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_parseheader);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss_6BD0: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */

extern struct SNDSTREAMCHANNEL *sndss_6BD0[32] __asm__("sndss");

// (prototype from their definition at line 2237)
void SNDSTRMI_startstream_6BD0(struct SNDSTREAMCHANNEL *p) __asm__("SNDSTRMI_startstream");

// (prototype from their definition at line 2257)
int SNDSTRMI_calcdatarate_6BD0(unsigned char *p) __asm__("SNDSTRMI_calcdatarate");

struct Ext_6BD0 { int f00; int pos[6]; };
struct Ev_6BD0 { int a; int b; int c; int d; int e; int rest[3]; };
extern void SNDI_patchtohdr_6BD0(int, int *, struct sA0A0Fmt *, struct sA0A0Hdr *, struct Ext_6BD0 *, char *) __asm__("SNDI_patchtohdr");
extern void STREAM_release_6BD0(int, int *) __asm__("STREAM_release");
extern int memcmp(void *, void *, int);

/**
 * Parses a stream header chunk ("SCHl") for the next queued request and starts or switches the stream.
 *
 * Moves the current request (+0x134) to the next queued one, parses the chunk into the next format and header with
 * SNDI_patchtohdr, sends pending user-data markers (type 3, with the request handle) to the user-data clients, gives the
 * chunk back to the file queue (STREAM_release) and stores the request's data rate (SNDSTRMI_calcdatarate). When the new format
 * or header differs (memcmp) and a stream is already running it sets state 2 (switch pending) and returns;
 * otherwise the next format becomes current and, unless it is playing, the stream starts (SNDSTRMI_startstream).
 *
 * Streams live in sndss (0x138 bytes each): +0x0 file STREAM, +0x4 voice, +0x8 packet player, +0x10 state,
 * +0x14/+0x18 current/next format, +0x1C/+0x84 current/next header, +0x11C queued requests, +0x128 free requests,
 * +0x134 current request.
 *
 * @param streamSlot  the stream slot
 * @param chunk  the header chunk
 * @return 0
 */
int SNDSTRMI_parseheader(int streamSlot, int *chunk) {
    struct SNDSTREAMCHANNEL *stream = sndss_6BD0[streamSlot];
    struct sA0A0Node *n;
    struct Ext_6BD0 ext;
    struct Ev_6BD0 ev;
    int i = 0;
    int j;

    if (((volatile struct sA0A0Node*)stream->parsingrequest) == 0) {
        (*(volatile struct sA0A0Node**)&stream->parsingrequest) = (*(struct sA0A0List*)&stream->allocatedrequests).f00;
    } else {
        (*(volatile struct sA0A0Node**)&stream->parsingrequest) = ((volatile struct sA0A0Node*)stream->parsingrequest)->f00;
    }
    n = (struct sA0A0Node *)((volatile struct sA0A0Node*)stream->parsingrequest);
    SNDI_patchtohdr_6BD0(0, chunk + 2, &(*(struct sA0A0Fmt*)&stream->newsf), &(*(struct sA0A0Hdr*)&stream->newsa), &ext, n->f25);
    n->f18 = ext.f00;
    n->f24 = 0;
    while ((*(struct sA0A0Hdr*)&stream->newsa).f48[i] != 0) {
        ev.a = 3;
        ev.b = (*(struct sA0A0Hdr*)&stream->newsa).f48[i];
        ev.c = (*(struct sA0A0Hdr*)&stream->newsa).f58[i];
        ev.e = n->f0C;
        (*(struct sA0A0Hdr*)&stream->newsa).f48[i] = 0;
        (*(struct sA0A0Hdr*)&stream->newsa).f58[i++] = 0;
        for (j = 0; j < sndgs.numuserdataclients; j++) {
            sndgs.userdataclient[j](&ev);
        }
    }
    STREAM_release_6BD0(stream->streamhandle, chunk);
    n->f10 = SNDSTRMI_calcdatarate_6BD0((unsigned char *)&(*(struct sA0A0Fmt*)&stream->newsf));
    if (memcmp(&(*(struct sA0A0Fmt*)&stream->cursf), &(*(struct sA0A0Fmt*)&stream->newsf), 4) != 0 || memcmp(&(*(struct sA0A0Hdr*)&stream->cursa), &(*(struct sA0A0Hdr*)&stream->newsa), 104) != 0 || (*(struct sA0A0Hdr*)&stream->newsa).f18[0] != 0) {
        if (*(unsigned short *)&(*(struct sA0A0Fmt*)&stream->cursf) != 0) {
            stream->state = 2;
            return 0;
        }
        (*(struct sA0A0Fmt*)&stream->cursf) = (*(struct sA0A0Fmt*)&stream->newsf);
        __builtin_memcpy(&(*(struct sA0A0Hdr*)&stream->cursa), &(*(struct sA0A0Hdr*)&stream->newsa), sizeof((*(struct sA0A0Hdr*)&stream->cursa)));
        (*(struct sA0A0Hdr*)&stream->newsa).f18[0] = 0;
    }
    if (stream->state != 1) {
        SNDSTRMI_startstream_6BD0(stream);
        stream->state = 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_parsedata);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
#include "fakematch.h"
typedef struct { int f0; int pad4; unsigned int f8; int offs[1]; } FILE_6EA8;
typedef struct {
    unsigned int lo;
    unsigned int size : 31;
    unsigned int flag : 1;
    int f8;
    char *ptrs[9];
} REQ_6EA8;
extern void STREAM_release_6EA8(int, FILE_6EA8 *) __asm__("STREAM_release");
extern int SNDPKTPLAY_submit_6EA8(int, REQ_6EA8 *) __asm__("SNDPKTPLAY_submit");

static inline void swap_6EA8(void *dst, unsigned int x) {
    unsigned char *s = (unsigned char *)&x;
    unsigned char *d = (unsigned char *)dst;

    d[0] = s[3];
    d[1] = s[2];
    d[2] = s[1];
    d[3] = s[0];
}

static inline unsigned int reqsize_6EA8(REQ_6EA8 *r) {
    return r->size;
}

/**
 * Submits a stream data chunk ("SCDl") to the stream's packet player.
 *
 * Reads the sample count (+0x8) and per-channel data offsets, byte-swapped when the current request is big-endian (its
 * +0x25 byte). An empty chunk goes straight back to the file queue. Otherwise it stores the chunk pointer just before
 * the first channel's data (SNDSTRMI_releasecallback finds it there), tags the chunk with the request handle, adds the count to
 * the request's total (+0x1C) and submits {count, continuation flag, channel pointers} with SNDPKTPLAY_submit on the packet
 * player (+0x8).
 *
 * @param own  the stream
 * @param dataChunk  the data chunk
 */
// FAKEMATCH: per-function volatile view of the stream's current-request pointer, `hdr = *(struct sA0A0Node *volatile *)&own->f134;`
// (evidence: SNDSTRMI_parseheader stores f134 and immediately reloads it), plus the register pin
// `register FILE_6EA8 **p0 FM_REG("$4") = (FILE_6EA8 **)req.ptrs[0];` (fakematch.h; retail loads ptrs[0] into a0 in the bnez slot
// and keeps the 0x7FFFFFFF size mask in a2, ours swaps them); plain form 97.43% (volatile view alone 99.70%).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
void SNDSTRMI_parsedata(struct SNDSTREAMCHANNEL *own, FILE_6EA8 *dataChunk) {
    REQ_6EA8 req;
    unsigned int swapped;
    struct sA0A0Node *hdr;
    char *base;
    int *offs;
    int i;

    hdr = *(struct sA0A0Node *volatile *)&(*(volatile struct sA0A0Node**)&own->parsingrequest);
    if (((unsigned char *)hdr->f25)[0]) {
        swap_6EA8(&swapped, dataChunk->f8);
        req.size = swapped;
    } else {
        req.size = dataChunk->f8;
    }
    offs = dataChunk->offs;
    base = (char *)(offs + (*(struct sA0A0Fmt*)&own->cursf).b[2]);
    for (i = 0; i < (*(struct sA0A0Fmt*)&own->cursf).b[2]; i++) {
        if (((unsigned char *)hdr->f25)[0]) {
            swap_6EA8(&swapped, offs[i]);
            req.ptrs[i] = base + swapped;
        } else {
            req.ptrs[i] = base + offs[i];
        }
    }
    if (!reqsize_6EA8(&req)) {
        STREAM_release_6EA8(own->streamhandle, dataChunk);
    } else {
        {
            register FILE_6EA8 **firstChan FM_REG("$4") = (FILE_6EA8 **)req.ptrs[0];
            firstChan[-1] = dataChunk;
        }
        dataChunk->f0 = hdr->f0C;
        hdr->f1C += req.size;
        req.flag = hdr->f24;
        SNDPKTPLAY_submit_6EA8(own->packetinstancehandle, &req);
        hdr->f24 = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_parsechunk);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss_7080: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */
extern struct SNDSTREAMCHANNEL *sndss_7080[32] __asm__("sndss");
extern void SNDSTRMI_parsedata_7080(struct SNDSTREAMCHANNEL *, int *) __asm__("SNDSTRMI_parsedata");
extern void SNDSTRMI_parseheader_7080(int) __asm__("SNDSTRMI_parseheader");
extern void STREAM_release_7080(int, int *) __asm__("STREAM_release");

/**
 * Dispatches one stream chunk by its tag.
 *
 * "SCDl" goes to SNDSTRMI_parsedata, "SCHl" to SNDSTRMI_parseheader (the chunk passes through in $a1); anything else goes back to
 * the file queue. Called by SNDSTRMI_service.
 *
 * @param streamSlot  the stream slot
 * @param chunk  the chunk
 * @return 0 after a header (stop reading this pass), else 1
 */
int SNDSTRMI_parsechunk(int streamSlot, int *chunk) {
    struct SNDSTREAMCHANNEL *stream = sndss_7080[streamSlot];
    int readMore;
    if (*chunk == 0x6C444353) {
        SNDSTRMI_parsedata_7080(stream, chunk);
        readMore = 1;
    } else if (*chunk == 0x6C484353) {
        SNDSTRMI_parseheader_7080(streamSlot);
        readMore = 0;
    } else {
        STREAM_release_7080(stream->streamhandle, chunk);
        readMore = 1;
    }
    return readMore;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_isheld);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern unsigned int f_14C8_70F8(int) __asm__("STREAM_gettable");
extern int f_14F8_70F8(int) __asm__("STREAM_state");
/**
 * Tells whether a stream's current request is still held (not yet to be parsed).
 *
 * A hold (+0x20 of the request) below 0 holds until changed (SNDSTRM_modifyhold); a positive hold ends once the file queue
 * has that many ms buffered (STREAM_gettable bytes, capped at 4000000, * 1000 / data rate), when STREAM_state reports
 * state 2 or 0, and otherwise holds while free requests remain. An ended hold is cleared.
 *
 * @return 1 while held, else 0
 */
int SNDSTRMI_isheld(struct SNDSTREAMCHANNEL *self) {
    struct sA0A0Node *request = ((volatile struct sA0A0Node*)self->parsingrequest);
    if (request == 0) {
        return 0;
    }
    if (request->f10 == 0) {
        return 0;
    }
    if (request->f20 < 0) {
        return 1;
    }
    if (request->f20 != 0) {
        unsigned int buffered = f_14C8_70F8(self->streamhandle);
        if (buffered > 4000000) buffered = 4000000;
        if (buffered * 1000 / request->f10 >= request->f20) {
            request->f20 = 0;
            return 0;
        }
        if (f_14F8_70F8(self->streamhandle) == 2) {
            request->f20 = 0;
            return 0;
        }
        if ((*(struct sA0A0List*)&self->freerequests).f08 > 0) return 1;
        if (f_14F8_70F8(self->streamhandle) == 0) {
            request->f20 = 0;
            return 0;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_service);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).


extern struct SNDSTREAMCHANNEL *sndss_71D8[32] __asm__("sndss");
extern int SNDPKTPLAY_framesoutstanding(int);
extern int SNDPKTPLAY_stop(int);
extern int SNDPKTPLAY_submitspace(int);
extern int *STREAM_get(int);

/**
 * Services every stream: completes format switches and parses newly read chunks.
 *
 * In the critical section, for each stream with queued requests: a pending switch (state 2) waits until the packet
 * player has no frames left (SNDPKTPLAY_framesoutstanding), then takes the next format and header, stops (SNDPKTPLAY_stop) and restarts
 * the voice (SNDSTRMI_startstream). Unless held (SNDSTRMI_isheld) it reads up to the packet player's free space (SNDPKTPLAY_submitspace;
 * 8 when not playing) of chunks from the file queue (STREAM_get) through SNDSTRMI_parsechunk, stopping after a header.
 * SNDSTRMI_create registers it with iSNDserveraddclient for the first stream.
 */
void SNDSTRMI_service(void) {
    int i = 0;
    struct SNDSTREAMCHANNEL *stream;
    int n;
    int readMore;
    int *chunk;
    SNDSYS_entercritical();
    for (; i < sndgs.sso.set.maxstreams; i++) {
        stream = sndss_71D8[i];
        if (stream == 0 || (*(struct sA0A0List*)&stream->allocatedrequests).f08 == 0) continue;
        if (stream->state == 2) {
            if (SNDPKTPLAY_framesoutstanding(stream->packetinstancehandle) > 0) continue;
            (*(struct sA0A0Fmt*)&stream->cursf) = (*(struct sA0A0Fmt*)&stream->newsf);
            __builtin_memcpy(&(*(struct sA0A0Hdr*)&stream->cursa), &(*(struct sA0A0Hdr*)&stream->newsa), sizeof((*(struct sA0A0Hdr*)&stream->cursa)));
            (*(struct sA0A0Hdr*)&stream->newsa).f18[0] = 0;
            SNDPKTPLAY_stop(stream->packetinstancehandle);
            SNDSTRMI_startstream(stream);
        }
        if (SNDSTRMI_isheld(stream) == 0) {
            if (stream->state == 1) {
                n = SNDPKTPLAY_submitspace(stream->packetinstancehandle);
                if (n <= 0) continue;
            } else {
                n = 8;
            }
            readMore = 0;
            do {
                n--;
                chunk = STREAM_get(stream->streamhandle);
                if (chunk != 0) readMore = SNDSTRMI_parsechunk(i, chunk);
            } while (readMore != 0 && n > 0);
        }
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_numcreated);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int sndss_7410[] __asm__("sndss");
extern unsigned char D_0050A92E_7410[] __asm__("D_0050A92E");

/**
 * Counts the streams that exist.
 *
 * Counts the non-null entries of sndss up to the count in D_0050A92E.
 *
 * @return the number of streams
 */
int SNDSTRMI_numcreated(void) {
    int n = 0;
    int i;

    for (i = 0; i < D_0050A92E_7410[0]; i++) {
        if (sndss_7410[i] != 0) {
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss_7450: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */



extern struct SNDSTREAMCHANNEL *sndss_7450[32] __asm__("sndss");

extern void SNDLINKI_push_7450(void *, void *) __asm__("SNDLINKI_push");

// (prototype from their definition at line 2527)
int SNDSTRMI_numcreated_7450(void) __asm__("SNDSTRMI_numcreated");

// (prototype from their definition at line 2333)
void SNDSTRMI_framescallback_7450(struct sA0A0Node *node, unsigned n, struct SNDSTREAMCHANNEL *s) __asm__("SNDSTRMI_framescallback");

// (prototype from their definition at line 2280)
int SNDSTRMI_destroyall_7450(void) __asm__("SNDSTRMI_destroyall");

// (prototype from their definition at line 2324)
void SNDSTRMI_releasecallback_7450(void *arg0) __asm__("SNDSTRMI_releasecallback");

// (prototype from their definition at line 2487)
void SNDSTRMI_service_7450(void) __asm__("SNDSTRMI_service");

extern void f_6210_7450(void *, int, int) __asm__("memset");
extern int f_6620_7450(void (*)(void *), void (*)(struct sA0A0Node *, unsigned, struct SNDSTREAMCHANNEL *), struct SNDSTREAMCHANNEL *, void *, int) __asm__("SNDPKTPLAY_create");
extern int STREAM_create(int, int, int, void *, int);
extern int STREAM_buffersize(int);
extern int SNDPKTPLAY_overhead_7450(int) __asm__("SNDPKTPLAY_overhead");
extern void SNDLINKI_init_7450(void *) __asm__("SNDLINKI_init");
extern int SNDPKTPLAY_destroy_7450(int) __asm__("SNDPKTPLAY_destroy");
extern void iSNDserveraddclient_7450(void (*)(void)) __asm__("iSNDserveraddclient");
extern int SNDSTRM_setgreedylevel_7450(int, int) __asm__("SNDSTRM_setgreedylevel");
extern int SNDSTRM_purge_7450(int) __asm__("SNDSTRM_purge");

int f_7450(struct sA0A0Parm *src, int n, int a2, char *buf, int size, int h, int ext) __asm__("SNDSTRMI_create");
extern struct sA8E8 sndgs_7450 __asm__("sndgs");
/**
 * Creates a stream in caller memory: request nodes, packet player and file queue.
 *
 * Takes the first free slot (count at byte +0x46 of the sound driver state sndgs; -9 when full), clears the
 * 0x138-byte stream at the start of the memory, puts n 40-byte request nodes on the free list, creates the packet
 * player with SNDPKTPLAY_create (release SNDSTRMI_releasecallback, frames SNDSTRMI_framescallback, the stream as client data, SNDPKTPLAY_overhead
 * bytes) and either uses the given file STREAM (from tap: +0x11 set) or creates one with STREAM_create in the rest of
 * the memory. It copies the play options to +0xEC; the first stream registers the service SNDSTRMI_service (iSNDserveraddclient)
 * and the shutdown hook SNDSTRMI_destroyall (+0x1EC). A created file queue gets a third of its size as greedy level
 * (SNDSTRM_setgreedylevel). Ends with SNDSTRM_purge.
 *
 * @param src  the play options (0x18 bytes)
 * @param n  maximum queued requests
 * @param maxChunks  maximum chunks for the packet player
 * @param buf  the stream's memory
 * @param size  its size
 * @param tapStream  an existing file STREAM to read from (when ext is set)
 * @param ext  nonzero to read from that STREAM instead of creating one
 * @return the stream handle (slot), or a negative error
 */
int f_7450(struct sA0A0Parm *src, int n, int maxChunks, char *buf, int size, int tapStream, int ext) {
    struct SNDSTREAMCHANNEL *stream;
    int i;
    int slot;
    int k;
    struct sA0A0Node *ents;
    char *packetMem;
    int fileStream;
    for (i = 0; i < sndgs_7450.setup.cfg.f26; i++) {
        if (sndss_7450[i] == 0) goto found;
    }
    return -9;
found:
    slot = i;
    stream = (struct SNDSTREAMCHANNEL *)buf;
    f_6210_7450(stream, 0, 312);
    buf += 0x138;
    ents = (struct sA0A0Node *)buf;
    buf += n * 40;
    size -= 312;
    size -= n * 40;
    k = SNDPKTPLAY_overhead_7450(maxChunks);
    packetMem = buf;
    buf += k;
    size -= k;
    SNDLINKI_init_7450(&(*(struct sA0A0List*)&stream->allocatedrequests));
    SNDLINKI_init_7450(&(*(struct sA0A0List*)&stream->freerequests));
    for (i = 0; i < n; i++) {
        SNDLINKI_push_7450(&(*(struct sA0A0List*)&stream->freerequests), &ents[i]);
    }
    stream->packetinstancehandle = f_6620_7450(SNDSTRMI_releasecallback_7450, SNDSTRMI_framescallback_7450, stream, packetMem, SNDPKTPLAY_overhead_7450(maxChunks));
    if (stream->packetinstancehandle < 0) return stream->packetinstancehandle;
    if (ext) {
        stream->fromtap = 1;
        stream->streamhandle = tapStream;
    } else {
        fileStream = STREAM_create(n + 2, 1, 1, buf, size);
        if (fileStream == 0) {
            SNDPKTPLAY_destroy_7450(stream->packetinstancehandle);
            return -9;
        }
        stream->streamhandle = fileStream;
        stream->fromtap = 0;
    }
    stream->lastsndrequesthandle = 0;
    stream->shandle = -1;
    (*(struct sA0A0Parm*)&stream->po) = *src;
    if (SNDSTRMI_numcreated_7450() == 0) {
        iSNDserveraddclient_7450(SNDSTRMI_service_7450);
        sndgs_7450.f1EC = (void (*)(void))SNDSTRMI_destroyall_7450;
    }
    sndss_7450[slot] = stream;
    if (ext == 0) {
        SNDSTRM_setgreedylevel_7450(slot, STREAM_buffersize(stream->streamhandle) / 3);
    }
    (*(struct sA0A0Hdr*)&stream->newsa).f18[0] = 0;
    SNDSTRM_purge_7450(slot);
    return slot;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_queue);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern int f_0C88_76E8(int, int, int, int) __asm__("STREAM_queuefile");
extern int f_0D80_76E8(int, int, int, int) __asm__("STREAM_queuemem");
extern struct sA0A0Node *f_62E8_76E8(struct sA0A0List *) __asm__("SNDLINKI_pop");
extern void f_6210_76E8(void *, int, int) __asm__("memset");
extern void f_62B8_76E8(struct sA0A0List *, struct sA0A0Node *) __asm__("SNDLINKI_pushtail");

int SNDSTRMI_queue_76E8(int, int, int, int, int) __asm__("SNDSTRMI_queue");
extern struct SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_76E8(int) __asm__("SNDSTRMI_getstreamptr");
/**
 * Queues a request on a stream: a file, a block in memory, or an existing file-queue request.
 *
 * Fails with -8 for an unknown stream and -13 when no request node is free. Type 0 queues a file with
 * STREAM_queuefile(file STREAM, a2, a3, "SCEl"), type 1 a memory block with STREAM_queuemem(file STREAM, a2, 0, "SCEl"),
 * any other type takes a3 as the file-queue request; a 0 result fails with -1. In the critical section it takes a free
 * node, clears it, appends it to the queued list (+0x11C), stores the file-queue request (+0x08), bumps the stream's
 * sequence (+0x0C, by 0x100, back to 0 on overflow) and gives the node the handle sequence | stream and the hold time
 * (+0x20). Called by SNDSTRM_queuefile (type 0) and func_003B7DE8 (type 2).
 *
 * @param streamHandle  the stream handle
 * @param holdTime  the hold time (ms; -1 holds until changed)
 * @param source  the file name (type 0) or memory block (type 1)
 * @param offsetOrRequest  the file offset (type 0) or file-queue request (type 2)
 * @param queueType  the queue type
 * @return the request handle, or a negative error
 */
int SNDSTRMI_queue_76E8(int streamHandle, int holdTime, int source, int offsetOrRequest, int queueType) {
    struct SNDSTREAMCHANNEL *stream = SNDSTRMI_getstreamptr_76E8(streamHandle);
    struct sA0A0Node *request;
    int fileRequest;
    if (stream == 0) return -8;
    if ((*(struct sA0A0List*)&stream->freerequests).f08 == 0) return -13;
    if (queueType == 0) {
        fileRequest = f_0C88_76E8(stream->streamhandle, source, offsetOrRequest, 0x6C454353);
    } else if (queueType == 1) {
        fileRequest = f_0D80_76E8(stream->streamhandle, source, 0, 0x6C454353);
    } else {
        fileRequest = offsetOrRequest;
    }
    if (fileRequest == 0) return -1;
    SNDSYS_entercritical();
    queueType = (int)f_62E8_76E8(&(*(struct sA0A0List*)&stream->freerequests));
    request = (struct sA0A0Node *)queueType;
    f_6210_76E8(request, 0, 40);
    f_62B8_76E8(&(*(struct sA0A0List*)&stream->allocatedrequests), request);
    request->f08 = fileRequest;
    stream->lastsndrequesthandle += 0x100;
    if (stream->lastsndrequesthandle < 0) stream->lastsndrequesthandle = 0;
    request->f0C = stream->lastsndrequesthandle | streamHandle;
    request->f20 = holdTime;
    stream = (struct SNDSTREAMCHANNEL *)request->f0C;
    SNDSYS_leavecritical();
    return (int)stream;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_create);
#ifdef SKIP_ASM
/**
 * Creates a stream that reads its own file queue (SNDSTRMI_create with no tap stream).
 *
 * The handle passes through in $v0 although the declaration is void; its caller, func_002AE4C0 (sound/bankmonitor),
 * declares it returning int and stores the handle.
 *
 * @param playOpts  the play options
 * @param maxRequests  maximum queued requests
 * @param maxChunks  maximum packet-player chunks
 * @param mem  the stream's memory
 * @param memSize  its size
 */
void SNDSTRM_create(int playOpts, int maxRequests, int maxChunks, int mem, int memSize) {
    SNDSTRMI_create(playOpts, maxRequests, maxChunks, mem, memSize, 0, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern void (*D_0050AAD4)(void);        /* sndgs.f1EC */

extern struct SNDSTREAMCHANNEL *sndss_7838[32] __asm__("sndss");
extern int SNDSTRMI_numcreated(void);
extern int SNDSTRM_purge(int);
extern void iSNDserverremoveclient_7838(void (*)(void)) __asm__("iSNDserverremoveclient");
extern int SNDPKTPLAY_destroy(int);
extern void STREAM_destroy(int);

extern void SNDSTRMI_service_7838(void) __asm__("SNDSTRMI_service");
extern struct SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_7838(int) __asm__("SNDSTRMI_getstreamptr");
int SNDSTRM_destroy_7838(int) __asm__("SNDSTRM_destroy");
/**
 * Destroys a stream: purges it, frees its packet player and file queue.
 *
 * Purges with SNDSTRM_purge; the last stream unregisters the service (iSNDserverremoveclient) and clears the shutdown hook
 * D_0050AAD4. Destroys the packet player (SNDPKTPLAY_destroy), frees the slot and destroys the file STREAM unless it was
 * borrowed (+0x11). Used by SNDSTRMI_destroyall and the movie player's audio object (func_003AEE30).
 *
 * @param streamHandle  the stream handle
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_destroy_7838(int streamHandle) {
    struct SNDSTREAMCHANNEL *stream = SNDSTRMI_getstreamptr_7838(streamHandle);
    int w;
    signed char borrowed;
    if (stream == 0) return -8;
    SNDSTRM_purge(streamHandle);
    if (SNDSTRMI_numcreated() == 1) {
        iSNDserverremoveclient_7838(SNDSTRMI_service_7838);
        D_0050AAD4 = 0;
    }
    SNDPKTPLAY_destroy(stream->packetinstancehandle);
    w = stream->streamhandle;
    borrowed = (*(volatile unsigned char*)&stream->fromtap);
    sndss_7838[streamHandle] = 0;
    if (borrowed == 0)
        STREAM_destroy(w);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_queuefile);
#ifdef SKIP_ASM
extern void SNDSTRMI_queue(int, int, int, int, int);

/**
 * Queues a file on a stream (SNDSTRMI_queue with queue type 0).
 *
 * The request handle passes through in $v0 although the declaration is void. Nothing in the code calls it.
 *
 * @param streamHandle  the stream
 * @param holdTime  hold before playing (ms, -1 until released)
 * @param fileName  the file name
 * @param offset  the start offset in the file
 */
void SNDSTRM_queuefile(int streamHandle, int holdTime, int fileName, int offset) {
    SNDSTRMI_queue(streamHandle, holdTime, fileName, offset, 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

extern int SNDPKTPLAY_stop(int);
extern void f_1110_78F8(int) __asm__("STREAM_kill");
extern void f_5E98_78F8(int) __asm__("SNDMEMI_free");
extern struct sA0A0Node *f_62E8_78F8(struct sA0A0List *) __asm__("SNDLINKI_pop");
extern void SNDLINKI_push(SNDLINKLIST*, void *);
extern void f_6210_78F8(void *, int, int) __asm__("memset");

/**
 * Purges a stream: stops its voice, cancels its reads and drops every queued request.
 *
 * In the critical section: stops the packet player (SNDPKTPLAY_stop) when a voice is set, clears the voice, cancels the
 * own file queue (STREAM_kill), frees the next header's time-stretch data copies (SNDMEMI_free), moves all queued
 * requests to the free list, clears the current request and state and zeroes both formats and headers. Called by
 * SNDSTRMI_create and SNDSTRM_destroy.
 *
 * @param streamHandle  the stream handle
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_purge(int streamHandle) {
    struct SNDSTREAMCHANNEL *stream;
    struct sA0A0Node *request;
    int i;
    SNDSYS_entercritical();
    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) {
        SNDSYS_leavecritical();
        return -8;
    }
    if (stream->shandle >= 0) {
        SNDPKTPLAY_stop(stream->packetinstancehandle);
    }
    stream->shandle = -1;
    if ((*(volatile unsigned char*)&stream->fromtap) == 0) {
        f_1110_78F8(stream->streamhandle);
    }
    if ((*(struct sA0A0Hdr*)&stream->newsa).f18[0] != 0) {
        i = 0;
        if ((*(struct sA0A0Fmt*)&stream->newsf).b[2] != 0) {
            int *chanBuf = &(*(struct sA0A0Hdr*)&stream->newsa).f18[0];
            do {
                i++;
                f_5E98_78F8(*chanBuf++);
            } while (i < (*(struct sA0A0Fmt*)&stream->newsf).b[2]);
        }
    }
    do {
        request = f_62E8_78F8(&(*(struct sA0A0List*)&stream->allocatedrequests));
        if (request == 0) break;
        SNDLINKI_push((SNDLINKLIST*)(&(*(struct sA0A0List*)&stream->freerequests)), request);
    } while (1);
    (*(volatile struct sA0A0Node**)&stream->parsingrequest) = 0;
    stream->state = 0;
    f_6210_78F8(&(*(struct sA0A0Fmt*)&stream->cursf), 0, 4);
    f_6210_78F8(&(*(struct sA0A0Fmt*)&stream->newsf), 0, 4);
    f_6210_78F8(&(*(struct sA0A0Hdr*)&stream->cursa), 0, 104);
    f_6210_78F8(&(*(struct sA0A0Hdr*)&stream->newsa), 0, 104);
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRMI_getrequestptr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
char *SNDSTRMI_getstreamptr_7A70(int) __asm__("SNDSTRMI_getstreamptr");

/**
 * Finds a queued stream request by its handle.
 *
 * The stream is the handle's low byte; the request is searched on its queued list (+0x11C) by handle (+0xC).
 *
 * @param requestHandle  the request handle
 * @return the request, or 0
 */
char *SNDSTRMI_getrequestptr(int requestHandle) {
    char *stream;
    char *request;
    if (requestHandle < 0) return 0;
    stream = SNDSTRMI_getstreamptr_7A70(requestHandle & 0xFF);
    if (stream == 0) return 0;
    for (request = *(char **)(stream + 0x11C); request != 0; request = *(char **)request) {
        if (*(volatile int *)(request + 0xC) == requestHandle) break;
    }
    return request;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_overheadtap);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDPKTPLAY_overhead(int);
int SNDSTRM_overheadtap_7AC8(int arg0, int arg1) __asm__("SNDSTRM_overheadtap");

/**
 * Returns the memory a stream needs when it reads from an existing file STREAM.
 *
 * 312 bytes for the stream, 40 per request and SNDPKTPLAY_overhead(maxChunks) for the packet player. The movie player's audio
 * object sizes its buffer with SNDSTRM_overheadtap(1, 0x1E).
 *
 * @param maxRequests  maximum queued requests
 * @param maxChunks  maximum packet-player chunks
 * @return bytes needed
 */
int SNDSTRM_overheadtap_7AC8(int maxRequests, int maxChunks) {
    int overhead = maxRequests * 40 + 312;
    return overhead + SNDPKTPLAY_overhead(maxChunks);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_overhead);
#ifdef SKIP_ASM
extern int SNDSTRM_overheadtap();
extern int STREAM_overhead(int, int, int);

/**
 * Returns the memory a stream with its own file queue needs.
 *
 * SNDSTRM_overheadtap (arguments passed through in registers) plus STREAM_overhead(maxRequests + 2, 1, 1).
 *
 * @param maxRequests  maximum queued requests
 * @return bytes needed
 */
int SNDSTRM_overhead(int maxRequests) {
    int overhead;

    overhead = SNDSTRM_overheadtap();
    return overhead + STREAM_overhead(maxRequests + 2, 1, 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_status);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */
extern unsigned int f_6C98_7B48(int) __asm__("SNDPKTPLAY_framesoutstanding");
extern int STREAM_gettable(int);

int SNDSTRM_status_impl(int, int *) __asm__("SNDSTRM_status");
/**
 * Fills a stream's status: queued requests, current request and buffered time.
 *
 * out[0] is the number of queued requests, out[1] the first one's handle and out[2] the buffered time in ms: the packet
 * player's outstanding frames (SNDPKTPLAY_framesoutstanding) * 1000 / the current sample rate, or when that is 0, the file queue's
 * buffered bytes (STREAM_gettable, capped at 4000000) * 1000 / the request's data rate. Returns early with out[2] = 0
 * when the current rate or the data rate is 0. Used by the movie player's audio object and the game's stream system.
 *
 * @param streamHandle  the stream handle
 * @param out  receives the three status words
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_status_impl(int streamHandle, int *out) {
    struct SNDSTREAMCHANNEL *stream;
    struct sA0A0Node *n;
    unsigned int bufferedBytes;
    int requestCount;
    
    out[2] = 0;
    out[1] = 0;
    out[0] = 0;
    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) return -8;
    requestCount = (*(struct sA0A0List*)&stream->allocatedrequests).f08;
    out[0] = requestCount;
    if (requestCount != 0) {
        n = (*(struct sA0A0List*)&stream->allocatedrequests).f00;
        out[1] = n->f0C;
        if (*(unsigned short *)(*(struct sA0A0Fmt*)&stream->cursf).b == 0) return 0;
        do {
            out[2] = f_6C98_7B48(stream->packetinstancehandle) * 1000 / *(unsigned short *)(*(struct sA0A0Fmt*)&stream->cursf).b;
            if (out[2] != 0) return 0;
            n = SNDSTRMI_getrequestptr(out[1]);
            if (n->f10 == 0) return 0;
        } while (0);
        bufferedBytes = STREAM_gettable(stream->streamhandle);
        out[2] = (bufferedBytes > 0x3D0900 ? 0x3D0900 : bufferedBytes) * 1000 / n->f10;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_requeststatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */
typedef struct { unsigned int lo, hi; } L64_7C40;
extern L64_7C40 f6380_7C40(int, int) __asm__("func_003C6380");
extern int f64A8_7C40(L64_7C40, int) __asm__("func_003C64A8");

/**
 * Fills a stream request's status: state, play position, time to end and time submitted.
 *
 * out[0] is 3 when the request is no longer queued (finished), 0 before its header was parsed (+0x24 clear), 2 when it
 * is the request at the head of the queue and 1 when it waits behind it. For states 1 and 2, out[1] = position (+0x14)
 * * 1000 / sample rate and out[2] = (end (+0x18) - position) * 1000 / rate, both through 64-bit helpers (func_00416380,
 * func_004164A8), and out[3] = submitted samples (+0x1C) * 1000 / rate; the rate comes from the current format for the
 * head request and from the next format otherwise. Used by the movie player's audio object and the game's stream
 * system.
 *
 * @param requestHandle  the request handle
 * @param out  receives the four status words
 * @return 0, or -8 for an invalid handle
 */
int SNDSTRM_requeststatus(int requestHandle, int *out) {
    struct SNDSTREAMCHANNEL *stream;
    struct sA0A0Node *request;
    unsigned int sampleRate;
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    if (requestHandle < 0 || (stream = SNDSTRMI_getstreamptr(requestHandle & 0xFF)) == 0) {
        return -8;
    }
    request = SNDSTRMI_getrequestptr(requestHandle);
    if (request != 0) {
        if (request->f24 == 0) {
            out[0] = 0;
        } else {
            if ((*(struct sA0A0List*)&stream->allocatedrequests).f00 == request) {
                out[0] = 2;
                sampleRate = *(unsigned short *)(*(struct sA0A0Fmt*)&stream->cursf).b;
            } else {
                out[0] = 1;
                sampleRate = *(unsigned short *)(*(struct sA0A0Fmt*)&stream->newsf).b;
            }
            out[1] = f64A8_7C40(f6380_7C40(request->f14, 1000), sampleRate);
            out[2] = f64A8_7C40(f6380_7C40(request->f18 - request->f14, 1000), sampleRate);
            out[3] = request->f1C * 1000 / sampleRate;
        }
    } else {
        out[0] = 3;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_modifyhold);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *SNDSTRMI_getrequestptr_7D80(int) __asm__("SNDSTRMI_getrequestptr");

/**
 * Changes a queued stream request's hold time.
 *
 * Stores the hold (+0x20) in the critical section; 0 releases a held request. Quirk: the value is truncated to 16 bits
 * (sign-extended), so holds above 32767 ms wrap. The movie player and the game's stream system call it with 0 to start
 * playback.
 *
 * @param requestHandle  the request handle
 * @param holdTime  the new hold (ms; -1 holds until changed)
 * @return 0, or -8 for an unknown request
 */
int SNDSTRM_modifyhold(int requestHandle, int holdTime) {
    int result;
    SNDSTREAMREQUEST* temp_2;

    result = -8;
    SNDSYS_entercritical();
    temp_2 = (SNDSTREAMREQUEST*)SNDSTRMI_getrequestptr_7D80(requestHandle);
    if (temp_2 != 0) {
        result = 0;
        temp_2->holdtime = (short)holdTime;
    }
    SNDSYS_leavecritical();
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7DE8);
#ifdef SKIP_ASM
extern void SNDSTRMI_queue(int, int, int, int, int);

/**
 * Queues an existing file-queue request on a stream (SNDSTRMI_queue with queue type 2).
 *
 * The request handle passes through in $v0 although the declaration is void. The movie player's audio object uses it
 * with a hold time of -1.
 *
 * @param streamHandle  the stream
 * @param holdTime  hold before playing (ms, -1 until released)
 * @param fileRequest  the file STREAM request to play
 */
void func_003B7DE8(int streamHandle, int holdTime, int fileRequest) {
    SNDSTRMI_queue(streamHandle, holdTime, 0, fileRequest, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7E10);
#ifdef SKIP_ASM
extern void SNDSTRMI_queue(int, int, int, int, int);

/**
 * Queues a block of memory on a stream (SNDSTRMI_queue with queue type 1).
 *
 * Passes buffer + offset as the block. The request handle passes through in $v0 although the declaration is void. Used
 * by the game's bank monitor.
 *
 * @param streamHandle  the stream
 * @param holdTime  hold before playing (ms, -1 until released)
 * @param buffer  the stream data in memory
 * @param offset  byte offset into it
 */
void func_003B7E10(int streamHandle, int holdTime, int buffer, int offset) {
    SNDSTRMI_queue(streamHandle, holdTime, buffer + offset, 0, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003B7E38);
#ifdef SKIP_ASM
extern void SNDSTRMI_create(int, int, int, int, int, int, int);

/**
 * Creates a stream that reads from an existing file STREAM (SNDSTRMI_create with fromtap set).
 *
 * The handle passes through in $v0 although the declaration is void. The movie player's audio object creates its stream
 * with it (1 request, 0x1E chunks).
 *
 * @param streamHandle  the file STREAM to read from
 * @param playOpts  the play options
 * @param maxRequests  maximum queued requests
 * @param maxChunks  maximum packet-player chunks
 * @param mem  the stream's memory
 * @param memSize  its size
 */
void func_003B7E38(int streamHandle, int playOpts, int maxRequests, int maxChunks, int mem, int memSize) {
    SNDSTRMI_create(playOpts, maxRequests, maxChunks, mem, memSize, streamHandle, 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7E70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_func_003B7E70(void) __asm__("SNDSTRMI_getstreamptr");
void SNDvol_func_003B7E70(int, int) __asm__("SNDvol");

/**
 * Sets a stream's volume.
 *
 * Stores it in the stream's play options (+0xEC) and passes it to the voice with SNDvol. Used by the movie
 * player and the game's bank monitor.
 *
 * @param streamHandle  the stream
 * @param volume  the volume (0..127)
 * @return 0, or -8 for an unknown stream
 */
int func_003B7E70(int streamHandle, int volume) {
    SNDSTREAMCHANNEL* stream;

    stream = SNDSTRMI_getstreamptr_func_003B7E70();
    if (stream == 0) {
        return -8;
        do { } while (0);
    }
    stream->po.vol = volume;
    SNDvol_func_003B7E70(stream->shandle, volume);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7EB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void SNDSTRMI_create(int, int, int, int, int, int, int);



// Looks up a handle and sets its byte option at 0xEC.
extern SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_7E70(int) __asm__("SNDSTRMI_getstreamptr");
extern void SNDvol_7E70(int, int) __asm__("SNDvol");


// Looks up a handle, stores the byte option at 0xEC and forwards a/10 with it.
extern void SNDautovol_7EB0(int, int, int) __asm__("SNDautovol");
/**
 * Fades a stream's volume to a target over a time.
 *
 * Stores the target in the stream's play options (+0xEC) and starts the fade on the voice with SNDautovol,
 * converting the time to 100 Hz ticks (/10). The game's bank monitor uses its result although the unit declares it int.
 *
 * @param streamHandle  the stream
 * @param fadeTime  the fade time in ms
 * @param targetVol  the final volume (0..127)
 * @return 0, or -8 for an unknown stream
 */
int func_003B7EB0(int streamHandle, int fadeTime, int targetVol) {
    SNDSTREAMCHANNEL* stream = SNDSTRMI_getstreamptr_7E70(streamHandle);
    if (stream == 0) return -8;
    stream->po.vol = targetVol;
    SNDautovol_7EB0(stream->shandle, fadeTime / 10, targetVol);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7F18);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

int func_003B86C0_7F18(int) __asm__("func_003B86C0");

/**
 * Returns a stream's current volume.
 *
 * Asks the voice with func_003B86C0; when the stream has no live voice it returns the volume in the stream's play
 * options (+0xEC).
 *
 * @param streamHandle  the stream handle
 * @return the volume, or -8 for an unknown stream
 */
int func_003B7F18(int streamHandle) {
    struct SNDSTREAMCHANNEL *stream;
    int volume;

    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) {
        return -8;
    }
    volume = func_003B86C0_7F18(stream->shandle);
    if (volume < 0) {
        volume = (*(struct sA0A0Parm*)&stream->po).f00;
    }
    return volume;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B7F60);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

int SND3dpos(int, int, int);

/**
 * Sets a stream's 3D position (azimuth and elevation).
 *
 * Stores both in the stream's play options (+0xEC) (+0x08, +0x0A) and passes them to the voice with SND3dpos. Used
 * by the game's stream system.
 *
 * @param streamHandle  the stream handle
 * @param azimuth  the azimuth
 * @param elevation  the elevation
 * @return 0, or -8 for an unknown stream
 */
int func_003B7F60(int streamHandle, int azimuth, int elevation) {
    struct SNDSTREAMCHANNEL *stream;

    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) {
        return -8;
    }
    (*(struct sA0A0Parm*)&stream->po).f08 = azimuth;
    (*(struct sA0A0Parm*)&stream->po).f0A = elevation;
    SND3dpos(stream->shandle, azimuth, elevation);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_pitchmult);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndss: the streams (one per stream slot, SNDSTRMI_getstreamptr looks one up). Layout from every use in this unit;
   field names are their offsets. */
int SNDpitchmult(int, int);

/**
 * Sets a stream's pitch multiplier.
 *
 * Caps it at 0x4000 (4.0 in 4.12 fixed point), stores it in the stream's play options (+0xEC) (+0x0C) and passes it to
 * the voice with SNDpitchmult. The movie player sets its playback speed with it.
 *
 * @param streamHandle  the stream handle
 * @param pitchMult  the pitch multiplier (0x1000 = 1.0)
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_pitchmult(int streamHandle, int pitchMult) {
    struct SNDSTREAMCHANNEL *stream;
    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) {
        return -8;
    }
    if (pitchMult > 0x4000) {
        pitchMult = 0x4000;
    }
    (*(struct sA0A0Parm*)&stream->po).f0C = pitchMult;
    SNDpitchmult(stream->shandle, pitchMult);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8010);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_func_003B8010(void) __asm__("SNDSTRMI_getstreamptr");
void func_003B8700_func_003B8010(int, int) __asm__("func_003B8700");

/**
 * Sets a stream's dry level.
 *
 * Stores it in the stream's play options (+0xEC) (+0x04) and passes it to the voice with func_003B8700.
 *
 * @param streamHandle  the stream
 * @param dryLevel  the dry level (0..127)
 * @return 0, or -8 for an unknown stream
 */
int func_003B8010(int streamHandle, int dryLevel) {
    SNDSTREAMCHANNEL* stream;

    stream = SNDSTRMI_getstreamptr_func_003B8010();
    if (stream == 0) {
        return -8;
        do { } while (0);
    }
    stream->po.drylevel = dryLevel;
    func_003B8700_func_003B8010(stream->shandle, dryLevel);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8050);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).



/**
 * Sets a stream's effect send level for an effect bus.
 *
 * Stores the level in the stream's play options (+0xEC) (+0x05, whatever the bus) and passes both to the voice with
 * func_003B88C8. Used by the game's bank monitor.
 *
 * @param streamHandle  the stream handle
 * @param busIndex  the effect bus
 * @param fxLevel  the send level (0..127)
 * @return 0, or -8 for an unknown stream
 */
int func_003B8050(int streamHandle, int busIndex, int fxLevel) {
    struct SNDSTREAMCHANNEL *stream;

    stream = SNDSTRMI_getstreamptr(streamHandle);
    if (stream == 0) {
        return -8;
    }
    (*(struct sA0A0Parm*)&stream->po).f05 = fxLevel;
    func_003B88C8(stream->shandle, busIndex, fxLevel);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B80A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_func_003B80A0(void) __asm__("SNDSTRMI_getstreamptr");
void func_003B8978_func_003B80A0(int, int) __asm__("func_003B8978");

/**
 * Sets a stream's low-pass cutoff.
 *
 * Stores it in the stream's play options (+0xEC) (+0x14, default 0xFFFF from SNDplaysetdef) and passes it to the voice
 * with func_003B8978.
 *
 * @param streamHandle  the stream
 * @param lowPassCutoff  the cutoff
 * @return 0, or -8 for an unknown stream
 */
int func_003B80A0(int streamHandle, int lowPassCutoff) {
    SNDSTREAMCHANNEL* stream;

    stream = SNDSTRMI_getstreamptr_func_003B80A0();
    if (stream == 0) {
        return -8;
        do { } while (0);
    }
    stream->po.lowpasscutoff = lowPassCutoff;
    func_003B8978_func_003B80A0(stream->shandle, lowPassCutoff);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_timemult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
SNDSTREAMCHANNEL *SNDSTRMI_getstreamptr_SNDSTRM_timemult(void) __asm__("SNDSTRMI_getstreamptr");
void SNDCTRL_timemult_SNDSTRM_timemult(int, int) __asm__("SNDCTRL_timemult");

/**
 * Sets a stream's time multiplier.
 *
 * Stores it in the stream's play options (+0xEC) (+0x0E, default 0x1000 from SNDplaysetdef) and passes it to the voice
 * with SNDCTRL_timemult.
 *
 * @param streamHandle  the stream
 * @param timeMult  the time multiplier (0x1000 = 1.0)
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_timemult(int streamHandle, int timeMult) {
    SNDSTREAMCHANNEL* stream;

    stream = SNDSTRMI_getstreamptr_SNDSTRM_timemult();
    if (stream == 0) {
        return -8;
        do { } while (0);
    }
    stream->po.timemult = timeMult;
    SNDCTRL_timemult_SNDSTRM_timemult(stream->shandle, timeMult);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSTRM_setgreedylevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int *SNDSTRMI_getstreamptr_8120(void) __asm__("SNDSTRMI_getstreamptr");
void STREAM_setgreedylevel_8120(int, int) __asm__("STREAM_setgreedylevel");

/**
 * Sets how much a stream's file queue reads ahead (forwards to STREAM_setgreedylevel).
 *
 * SNDSTRMI_create sets it to a third of the file queue's size for every stream it creates its own queue for.
 *
 * @param streamHandle  the stream
 * @param greedyLevel  the read-ahead threshold
 * @return 0, or -8 for an unknown stream
 */
int SNDSTRM_setgreedylevel(int streamHandle, int greedyLevel) {
    int *temp_2;

    temp_2 = SNDSTRMI_getstreamptr_8120();
    if (temp_2 == 0) {
        return -8;
    }
    STREAM_setgreedylevel_8120(*temp_2, greedyLevel);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDmastervol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_A7B0_8160(int) __asm__("func_003BA7B0");
extern void f_2A50_8160(int) __asm__("func_003C2A50");
struct G_8160 { char pad[0x175]; char c; char pad2[8]; short n; char pad3[0x6F]; struct CHANPUB *tab; };

/**
 * Sets the sound system's master volume and reapplies it to every playing channel.
 *
 * Stores the volume at the sound driver state sndgs +0x175 in the critical section, then recomputes each active
 * channel's volume (func_003BA7B0, func_003C2A50). cSSXAudio's volume control passes out * 127.
 *
 * @param masterVol  the master volume (0..127)
 * @return 0
 */
int SNDmastervol(int masterVol) {
    int i;
    struct CHANPUB *channel;
    SNDSYS_entercritical();
    sndgs.mastervol = masterVol;
    for (i = 0; i < sndgs.voicestotal; i++) {
        channel = &((struct CHANPUB*)sndgs.chan)[i];
        if ((*(char*)&channel->status) == 1 && channel->handle >= 0) {
            f_A7B0_8160(i);
            f_2A50_8160(i);
        }
    }
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8218);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050AAE0_8218[] __asm__("D_0050AAE0");
extern char D_0050AAF4_8218[] __asm__("D_0050AAF4");
extern char D_0050AB08_8218[] __asm__("D_0050AB08");

/**
 * Returns the effect bus record for a bus number and render kind.
 *
 * Kind 8 selects the table D_0050AB08, 4 D_0050AAE0 and 0x100 D_0050AAF4 (0x14-byte records, the buses at +0x220,
 * +0x1F8 and +0x20C of the sound driver state sndgs); other kinds give 0.
 *
 * @param busIndex  the bus number
 * @param renderMode  the render mode (masked with 0x71C)
 * @return the bus record, or 0
 */
void *func_003B8218(int busIndex, int renderMode) {
    switch (renderMode & 0x71C) {
    case 8:
        return (busIndex * 20) + D_0050AB08_8218;
    case 4:
        return (busIndex * 20) + D_0050AAE0_8218;
    case 0x100:
        return (busIndex * 20) + D_0050AAF4_8218;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8290);
#ifdef SKIP_ASM
/**
 * Converts bus selector flags into render-kind bits.
 *
 * 0x10 gives 4, 0x20 gives 0x100 and 0x40 gives 8.
 *
 * @param flags  the selector flags
 * @return the render-kind bits
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int func_003B8290(unsigned long flags) {
    int result = (flags & 0x40) >> 3;
    if (flags & 0x10) result |= 4;
    if (flags & 0x20) result |= 0x100;
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B82B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_82B8 { short s0; short pad; int w4; int w8; };
extern int func_003B8290_82B8(int) __asm__("func_003B8290");
extern struct E_82B8 *func_003B8218_82B8(int, int) __asm__("func_003B8218");
extern int func_003C36A8_82B8(int, int) __asm__("func_003C36A8");
extern void func_003B83A0_82B8(int, int) __asm__("func_003B83A0");

/**
 * Configures the effect buses selected by the flags and sets their level.
 *
 * No selector bits (0x1F8) means all. For each selected kind it writes b, c and d into the bus record (func_003B8218)
 * and applies it with func_003C36A8; then sets the level with func_003B83A0, forcing 0 when b is 0.
 *
 * @param flags  bus number plus selector bits (0x10, 0x20, 0x40)
 * @param busLevel  the level
 * @param b  stored in the record's first halfword; 0 also zeroes the level
 * @param c  stored in the record's +4 word
 * @param d  stored in the record's +8 word
 * @return 0
 */
int func_003B82B8(int flags, int busLevel, int b, int c, int d) {
    int bits;
    int kindBit = 4;
    int base;
    struct E_82B8 *busRec;

    if (!(flags & 0x1F8)) flags |= 0x1F8;
    bits = func_003B8290_82B8(flags);
    base = flags & ~0x1F8;
    while (bits) {
        if (kindBit & bits) {
            busRec = func_003B8218_82B8(base, kindBit);
            busRec->s0 = b;
            busRec->w4 = c;
            busRec->w8 = d;
            func_003C36A8_82B8(base, kindBit);
            bits &= ~kindBit;
        }
        kindBit <<= 1;
    }
    func_003B83A0_82B8(flags, b ? busLevel : 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B83A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
extern void f84E0_83Ab(int, int, int, struct sA8E8Pan *) __asm__("func_003B84E0");
extern int func_003B88C8(int, int, int);

extern struct sA8E8 sndgs_83A0 __asm__("sndgs");
/**
 * Sets the level of the effect buses selected by the flags and reapplies every channel's sends.
 *
 * No selector bits means all; 0x10, 0x20 and 0x40 set the level (+0x02) of the kind 4, 0x100 and 8 bus records. For
 * kind 8 it also sets both sides of the bus pan pair to level / 127 (func_003B84E0). Then, in the critical section,
 * every channel's send for the bus is recomputed through func_003B88C8.
 *
 * @param busFlags  bus number plus selector bits
 * @param busLevel  the level (0..127)
 * @return 0
 */
int func_003B83A0(int busFlags, int busLevel) {
    int id = busFlags & ~0x1F8;
    int i;
    int selectsKind8;
    if ((busFlags & 0x1F8) == 0) {
        busFlags |= 0x1F8;
    }
    if (busFlags & 0x10) {
        sndgs_83A0.f1F8[id].f02 = busLevel;
    }
    if (busFlags & 0x20) {
        sndgs_83A0.f20C[id].f02 = busLevel;
    }
    selectsKind8 = busFlags & 0x40;
    if (selectsKind8) {
        sndgs_83A0.f220[id].f02 = busLevel;
    }
    SNDSYS_entercritical();
    if (selectsKind8) {
        struct sA8E8Pan panPair;
        panPair.r = panPair.l = (float)busLevel * 0.007874016f;
        f84E0_83Ab(id, 8, 0, &panPair);
    }
    for (i = 0; i < sndgs_83A0.f17E; i++) {
        struct CHANPUB *channel = &sndgs_83A0.f1F0[i];
        func_003B88C8(channel->handle, id, channel->unk_61[id]);
    }
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B84E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V_84E0 { int a; int b; };
struct E_84E0 { struct V_84E0 v; char pad[0xC]; };
extern struct E_84E0 D_0050AB14_84E0[] __asm__("D_0050AB14");
extern void func_003C36C8_84E0(int, int, int, struct V_84E0 *) __asm__("func_003C36C8");

/**
 * Stores a kind-8 effect bus's pan pair and passes it to the platform.
 *
 * With flag 8 the pair goes to D_0050AB14[bus]; func_003C36C8 then writes the scaled values to the output state.
 *
 * @param busIndex  the bus number
 * @param flags  render-kind flags
 * @param arg2  passed on to func_003C36C8
 * @param panPair  the left/right gains
 * @return 0
 */
int func_003B84E0(int busIndex, int flags, int arg2, struct V_84E0 *panPair) {
    if (flags & 8) {
        D_0050AB14_84E0[busIndex].v = *panPair;
    }
    func_003C36C8_84E0(busIndex, flags, arg2, panPair);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDautovol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8530(int, int *) __asm__("iSNDpatchkey");
extern int SNDVOICEI_get_8530(void) __asm__("SNDVOICEI_get");
struct G_8530 { char pad[0x1F0]; char *tab; };

/**
 * Fades a sound's volume to a target over a number of 100 Hz ticks.
 *
 * Resolves the handle (passed through in $a0) with SNDVOICEI_get and, for every channel of its patch-key group
 * (iSNDpatchkey), sets the fade target (+0x34) and the per-tick step (+0x30) from the current volume (+0x38); the 100
 * Hz server SNDSYSI_100hzserver then moves the volume. A time of 0 or less counts as 1.
 *
 * @param soundHandle  the sound handle
 * @param n  the fade time in 100 Hz ticks
 * @param targetVol  the target volume (0..127)
 * @return the channel index, or -8 for a dead handle
 */
int SNDautovol(int soundHandle, int n, int targetVol) {
    int chanIndex = SNDVOICEI_get_8530();
    int setChan;
    if (chanIndex >= 0) {
        if (n <= 0) n = 1;
        setChan = -1;
        targetVol <<= 16;
        while (iSNDpatchkey_8530(chanIndex, &setChan)) {
            char *channel = (char*)&sndgs.chan[setChan];
            *(int *)(channel + 0x34) = targetVol;
            *(int *)(channel + 0x30) = (targetVol - *(int *)(channel + 0x38)) / n;
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDvol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int SNDVOICEI_get_85F0(void) __asm__("SNDVOICEI_get");
extern int iSNDpatchkey_85F0(int, int *) __asm__("iSNDpatchkey");
extern void func_003BA7B0_85F0(int) __asm__("func_003BA7B0");
extern void func_003C2A50_85F0(int) __asm__("func_003C2A50");
extern void SNDPLATFORM_setfxlevel_85F0(int, int) __asm__("SNDPLATFORM_setfxlevel");
struct G_85F0 { char pad[0x1F0]; struct CHANPUB *tab; };

int SNDvol_85F0(int a, int v) __asm__("SNDvol");

int SNDvol_impl(int a, int v) __asm__("SNDvol");
/**
 * Sets a sound's volume immediately, cancelling any fade.
 *
 * For each channel of the handle's patch-key group: clears the fade step (+0x30), stores the volume as 16.16 in +0x38,
 * recomputes the volume (func_003BA7B0, func_003C2A50) and refreshes the send through SNDPLATFORM_setfxlevel when +0x62 is
 * positive. Quirk: a channel already at that volume makes it return 0 at once, skipping the rest of the group.
 *
 * @param soundHandle  the sound handle
 * @param volume  the volume (0..127)
 * @return the channel index, -8 for a dead handle, or 0 when unchanged
 */
int SNDvol_impl(int soundHandle, int volume) {
    int setChan;
    int chanIndex;
    struct CHANPUB *channel;

    chanIndex = SNDVOICEI_get_85F0();
    if (chanIndex >= 0) {
        setChan = -1;
        while (iSNDpatchkey_85F0(chanIndex, &setChan) != 0) {
            channel = &sndgs.chan[setChan];
            channel->fadePer100Hz = 0;
            if (channel->programmedVol == volume << 16) return 0;
            channel->programmedVol = volume << 16;
            func_003BA7B0_85F0(setChan);
            func_003C2A50_85F0(setChan);
            if (channel->unk_62[0] > 0) SNDPLATFORM_setfxlevel_85F0(channel->voices[0], 0);
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B86C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns a sound's current volume.
 *
 * The handle passes through in $a0 to SNDVOICEI_get; the result is the integer part of the channel's 16.16 volume
 * (+0x3A). Used by func_003B7F18 and the game's bank monitor.
 *
 * @return the volume, or -8 for a dead handle
 */
short func_003B86C0(void) {
    int i = (int)SNDVOICEI_get();
    if (i < 0) {
        return -8;
    }
    return *(short *)((char*)&sndgs.chan[i] + 0x3A);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8700);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8700(int, int *) __asm__("iSNDpatchkey");
extern void func_003C2EC8_8700(int) __asm__("func_003C2EC8");
struct G_8700 { char pad[0x1F0]; char *tbl; };

/**
 * Sets a sound's dry level.
 *
 * Stores it in each grouped channel's +0x5F and applies it with func_003C2EC8.
 *
 * @param soundHandle  the sound handle
 * @param dryLevel  the dry level (0..127)
 * @return the channel index, or -8 for a dead handle
 */
int func_003B8700(int soundHandle, int dryLevel) {
    int setChan;
    int chanIndex;

    chanIndex = SNDVOICEI_get();
    if (chanIndex >= 0) {
        setChan = -1;
        while (iSNDpatchkey_8700(chanIndex, &setChan) != 0) {
            
            *(char*)&sndgs.chan[setChan].drylevel = dryLevel;
            func_003C2EC8_8700(setChan);
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDpitchmult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDVOICEI_get_8790(void) __asm__("SNDVOICEI_get");
extern void iSNDcalcpitch_8790(int, int) __asm__("iSNDcalcpitch");
extern int iSNDpatchkey_8790(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_setpitch_8790(int) __asm__("SNDPLATFORM_setpitch");
int SNDpitchmult_8790(int a0, int arg1) __asm__("SNDpitchmult");

/**
 * Sets a sound's pitch multiplier and recomputes its pitch.
 *
 * Stores it in each grouped channel's +0x88 and recomputes with iSNDcalcpitch and SNDPLATFORM_setpitch. Quirk: a channel
 * already at that value makes it return 0 at once, skipping the rest of the group.
 *
 * @param soundHandle  the sound handle
 * @param pitchMult  the pitch multiplier (0x1000 = 1.0)
 * @return the channel index, -8 for a dead handle, or 0 when unchanged
 */
int SNDpitchmult_8790(int soundHandle, int pitchMult) {
    int setChan;
    int chanIndex;
    char *channel;
    char *sndState;
    int i;
    chanIndex = SNDVOICEI_get_8790();
    if (chanIndex >= 0) {
        setChan = -1;
        sndState = ((char*)&sndgs);
        while (iSNDpatchkey_8790(chanIndex, &setChan) != 0) {
            i = setChan;
            channel = *(char **)(sndState + 0x1F0) + i * 140;
            if (*(unsigned short *)(channel + 0x88) == pitchMult) {
                return 0;
            }
            *(unsigned short *)(channel + 0x88) = pitchMult;
            iSNDcalcpitch_8790(i, i);
            SNDPLATFORM_setpitch_8790(setChan);
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDCTRL_timemult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8838(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_timemult_8838(int, int) __asm__("SNDPLATFORM_timemult");
struct G_8838 { char pad[0x1F0]; char *tbl; };

/**
 * Sets a sound's time multiplier.
 *
 * Stores it in each grouped channel's +0x6A (SNDtimeremaining divides the remaining time by it) and applies it with
 * SNDPLATFORM_timemult. Always returns 0, even for a dead handle.
 *
 * @param soundHandle  the sound handle
 * @param timeMult  the time multiplier (0x1000 = 1.0)
 * @return 0
 */
int SNDCTRL_timemult(int soundHandle, int timeMult) {
    int setChan;
    int chanIndex;

    chanIndex = SNDVOICEI_get();
    if (chanIndex >= 0) {
        setChan = -1;
        while (iSNDpatchkey_8838(chanIndex, &setChan) != 0) {
            int i = setChan;
            *(short*)&sndgs.chan[i].timemult = timeMult;
            SNDPLATFORM_timemult_8838(i, timeMult);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B88C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDVOICEI_get_88C8() __asm__("SNDVOICEI_get");
extern void func_003BA740_88C8(int, int) __asm__("func_003BA740");
extern int iSNDpatchkey_88C8(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_setfxlevel_88C8(int, int) __asm__("SNDPLATFORM_setfxlevel");

/**
 * Sets a sound's send level to one effect bus.
 *
 * Stores the level in each grouped channel's per-bus byte (+0x61 + bus), recomputes the send with func_003BA740 and
 * applies it with SNDPLATFORM_setfxlevel.
 *
 * @param soundHandle  the sound handle
 * @param busIndex  the effect bus
 * @param fxLevel  the send level (0..127)
 * @return the channel index, or -8 for a dead handle
 */
int func_003B88C8(int soundHandle, int busIndex, int fxLevel) {
    int setChan;
    int chanIndex;
    char *sndState;
    chanIndex = SNDVOICEI_get_88C8();
    if (chanIndex >= 0) {
        setChan = -1;
        sndState = ((char*)&sndgs);
        while (iSNDpatchkey_88C8(chanIndex, &setChan) != 0) {
            *(char *)(setChan * 0x8C + *(int *)(sndState + 0x1F0) + busIndex + 0x61) = fxLevel;
            func_003BA740_88C8(busIndex, setChan);
            SNDPLATFORM_setfxlevel_88C8(setChan, busIndex);
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8978);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8978(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_lowpass_8978(int, int) __asm__("SNDPLATFORM_lowpass");

/**
 * Sets a sound's low-pass cutoff on each of its voices (SNDPLATFORM_lowpass).
 *
 * @param soundHandle  the sound handle
 * @param lowPassCutoff  the cutoff
 * @return the channel index, or -8 for a dead handle
 */
int func_003B8978(int soundHandle, int lowPassCutoff) {
    int setVoice;
    int voice;

    voice = SNDVOICEI_get();
    if (voice >= 0) {
        setVoice = -1;
        while (iSNDpatchkey_8978(voice, &setVoice) != 0) {
            SNDPLATFORM_lowpass_8978(setVoice, lowPassCutoff);
        }
    }
    return voice;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDstop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SNDVOICEI_get_89E0(void) __asm__("SNDVOICEI_get");
int iSNDpatchkey_89E0(int, int *) __asm__("iSNDpatchkey");
void SNDPLATFORM_stop_89E0(int) __asm__("SNDPLATFORM_stop");

/**
 * Stops a sound: every voice of its patch-key group.
 *
 * The handle passes through in $a0 to SNDVOICEI_get; each grouped channel (iSNDpatchkey) is stopped with
 * SNDPLATFORM_stop. Used by the 100 Hz server when a fade drops below 0, by the bank remover, the voice allocator when
 * stealing, and the MIDI code.
 *
 * @return the channel index, or -8 for a dead handle
 */
int SNDstop(void) {
    int setVoice;
    int temp_2;

    temp_2 = SNDVOICEI_get_89E0();
    if (temp_2 >= 0) {
        setVoice = -1;
        while (iSNDpatchkey_89E0(temp_2, &setVoice) != 0) {
            SNDPLATFORM_stop_89E0(setVoice);
        }
    }
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003B8A38);
#ifdef SKIP_ASM
struct G_8A38 { char p[0x17E]; short n; char p2[0x1F0 - 0x180]; struct CHANPUB *ents; };
extern int SNDstop_8A38(int) __asm__("SNDstop");
extern void SNDSYS_entercritical_8A38(void) __asm__("SNDSYS_entercritical");
extern void SNDSYS_leavecritical_8A38(void) __asm__("SNDSYS_leavecritical");
/**
 * Stops every sound (SNDstop on each channel's handle) inside the critical section.
 */
void func_003B8A38(void) {
    int i = 0;
    SNDSYS_entercritical_8A38();
    for (; i < sndgs.voicestotal; i++) SNDstop_8A38(sndgs.chan[i].handle);
    SNDSYS_leavecritical_8A38();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDover);
#ifdef SKIP_ASM
extern unsigned int SNDVOICEI_get();

/**
 * Tells whether a sound has ended (its handle no longer names a live channel).
 *
 * The handle passes through in $a0 to SNDVOICEI_get.
 *
 * @return 1 when ended, 0 while playing
 */
unsigned int SNDover(void) {
    return SNDVOICEI_get() >> 0x1F;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDtimeremaining);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
extern int iSNDpatchkey_8AD8(int, int *) __asm__("iSNDpatchkey");
extern int SNDVOICEI_get_8AD8(void) __asm__("SNDVOICEI_get");
extern int SNDPLATFORM_getcurframe_8AD8(int) __asm__("SNDPLATFORM_getcurframe");
/**
 * Returns how long a sound still plays, in 1/100 s.
 *
 * For each grouped channel of the handle (passed through in $a0): a looping channel (sustain end +0x18 > 0) counts as
 * -7; otherwise (frames +0x14 - current frame from SNDPLATFORM_getcurframe) * 100 / sample rate (+0x20), scaled by 4096 / pitch
 * (+0x8A) and 4096 / time multiplier (+0x6A), or 0x7FFFFFFF when the pitch is 0x10 or less. The largest value wins.
 * Used by the game's bank monitor.
 *
 * @return the remaining time, -7 when looping or unknown, or -8 for a dead handle
 */
int SNDtimeremaining(void) {
    int best = -7;
    int chanIndex = SNDVOICEI_get_8AD8();
    int setChan;
    if (chanIndex >= 0) {
        setChan = -1;
        while (iSNDpatchkey_8AD8(chanIndex, &setChan) != 0) {
            struct CHANPUB *channel = &sndgs.chan[setChan];
            int curFrame;
            if (channel->sustainend > 0) {
                curFrame = -7;
            } else {
                curFrame = SNDPLATFORM_getcurframe_8AD8(setChan);
            }
            if (curFrame >= 0) {
                int remaining = (((int)channel->frames) - curFrame) * 100 / channel->samplerate;
                if (channel->finalpitch > 0x10) {
                    remaining = ((remaining << 12) / channel->finalpitch << 12) / channel->timemult;
                } else {
                    remaining = 0x7FFFFFFF;
                }
                if (best < remaining) {
                    best = remaining;
                }
            }
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SND3dpos);
#ifdef SKIP_ASM
struct G_8C00 { char pad[0x1F0]; struct CHANPUB *tab; };

/**
 * Sets a sound's 3D position: azimuth relative to its built-in pan, and elevation.
 *
 * The elevation is clamped to -0x4000..0x3FFF. For each grouped channel the azimuth (+0x1C) becomes the built-in
 * azimuth (+0x4C) plus the argument, the elevation goes to +0x1E, and func_003C38E0 applies them.
 *
 * @param soundHandle  the sound handle
 * @param azimuth  the azimuth (0x10000 = full circle)
 * @param elevation  the elevation
 * @return the channel index, or -8 for a dead handle
 */
int SND3dpos(int soundHandle, int azimuth, int elevation) {
    int chanIndex;
    int setChan;
    if (elevation >= 0x4000) {
        elevation = 0x3FFF;
    } else if (elevation < -0x4000) {
        elevation = -0x4000;
    }
    chanIndex = SNDVOICEI_get();
    if (chanIndex >= 0) {
        setChan = -1;
        while (iSNDpatchkey(chanIndex, &setChan)) {
            struct CHANPUB *channel = &sndgs.chan[setChan];
            channel->azimuth = channel->builtinazimuth + azimuth;
            channel->elevation = elevation;
            func_003C38E0(setChan);
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDCTRL_filteradd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8CC8(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_filteradd_8CC8(int, int) __asm__("SNDPLATFORM_filteradd");

/**
 * Applies a setting to each voice of a sound through SNDPLATFORM_filteradd.
 *
 * SNDPLATFORM_filteradd acts only on render kind 4 channels. SNDSTRMI_startstream passes the stream's first track record.
 *
 * @param soundHandle  the sound handle
 * @param arg1  passed to SNDPLATFORM_filteradd
 * @return the channel index, or -8 for a dead handle
 */
int SNDCTRL_filteradd(int soundHandle, int arg1) {
    int setVoice;
    int voice;

    voice = SNDVOICEI_get();
    if (voice >= 0) {
        setVoice = -1;
        while (iSNDpatchkey_8CC8(voice, &setVoice) != 0) {
            SNDPLATFORM_filteradd_8CC8(setVoice, arg1);
        }
    }
    return voice;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDCTRL_filterremove);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int iSNDpatchkey_8D30(int, int *) __asm__("iSNDpatchkey");
extern void SNDPLATFORM_filterremove_8D30(int, int) __asm__("SNDPLATFORM_filterremove");

/**
 * Applies a setting to each voice of a sound through SNDPLATFORM_filterremove.
 *
 * @param soundHandle  the sound handle
 * @param arg1  passed to SNDPLATFORM_filterremove
 * @return the channel index, or -8 for a dead handle
 */
int SNDCTRL_filterremove(int soundHandle, int arg1) {
    int setVoice;
    int voice;

    voice = SNDVOICEI_get();
    if (voice >= 0) {
        setVoice = -1;
        while (iSNDpatchkey_8D30(voice, &setVoice) != 0) {
            SNDPLATFORM_filterremove_8D30(setVoice, arg1);
        }
    }
    return voice;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midigetseqindex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x10]; int key; char pad2[0x5C]; } E_8D98;
extern E_8D98 sndms_8D98[] __asm__("sndms");

/**
 * Returns the sequence slot for a sequence handle.
 *
 * The slot is the low 5 bits (below 16) and the slot's handle (+0x10) must equal the argument.
 *
 * @param seqHandle  the sequence handle
 * @return the slot, or -8
 */
int SNDI_midigetseqindex(int seqHandle) {
    int idx;
    int handleValue = seqHandle;
    if (handleValue < 0)
        return -8;
    idx = handleValue & 0x1F;
    if (idx < 0x10)
        return (sndms_8D98[idx].key ^ handleValue) == 0 ? idx : -8;
    return -8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiupdatefade);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndms: the MIDI sequence player (16 songs, then 24 voices, also D_0050A820). Layout from every use in this
   unit; field names are their offsets. */
struct sA120Chan {                  /* 4 bytes, one per MIDI channel (sA120Song.f30[]) */
    unsigned int b00 : 10;
    unsigned int b10 : 7;           /* volume */
    unsigned int b17 : 7;
    int b24 : 8;                    /* program */
};
struct sA120Song {                  /* 0x70, sndms.f000[] */
    int f00;
    unsigned int f04;
    unsigned char *f08;             /* read position */
    unsigned char *f0C;             /* loop start */
    int f10;                        /* handle (sequence number | index) */
    int f14;                        /* volume, 16.16 */
    int f18;                        /* volume step */
    int f1C;                        /* volume target */
    unsigned short f20;
    unsigned short f22;
    unsigned short f24;             /* tempo, percent */
    unsigned char f26;
    unsigned char f27;              /* ticks to the next event */
    signed char f28;                /* state */
    char pad29[0x2C - 0x29];
    short f2C;
    short f2E;
    struct sA120Chan f30[16];
};
struct sA120Voice {                 /* 8 bytes, sndms.f700[] (also D_0050A820) */
    int f00;                        /* voice id */
    signed char f04;                /* owning song */
    signed char f05;                /* channel */
    signed char f06;
    signed char f07;
};
struct sA120 {
    struct sA120Song f000[16];
    struct sA120Voice f700[24];
    int f7C0;                       /* sequence counter */
};
extern struct sA120 sndms;
extern struct sA120Voice D_0050A820[];  /* sndms.f700 */
extern int SNDseqstop_8DE8(int) __asm__("SNDseqstop");
extern int f_85F0_8DE8(int, int) __asm__("SNDvol");

int SNDI_midiupdatefade_impl(int) __asm__("SNDI_midiupdatefade");
/**
 * Steps a sequence's volume fade and applies it to its playing notes.
 *
 * Adds the step (+0x18) to the volume (+0x14) and stops at the target (+0x1C). Below 0 the sequence is stopped
 * (SNDseqstop) and it returns 1; otherwise every note voice of the sequence gets channel volume * sequence volume /
 * 128 through SNDvol.
 *
 * Sequences live in sndms (16 records of 0x70 bytes: +0x08 read position, +0x0C loop start, +0x10 handle, +0x14
 * volume 16.16, +0x18 fade step, +0x1C fade target, +0x20 base azimuth, +0x22 pitch multiplier, +0x24 tempo percent,
 * +0x27 ticks to wait, +0x28 state, +0x2C/+0x2E bank handles, +0x30 sixteen channel words); D_0050A820 holds 24 note
 * voices (handle, sequence, channel, key, velocity).
 *
 * @param id  the sequence slot
 * @return 1 when the fade stopped the sequence, else 0
 */
int SNDI_midiupdatefade_impl(int id) {
    struct sA120Song *seqInst = &sndms.f000[id];
    struct sA120Voice *voiceSlot;
    int i;
    seqInst->f14 = seqInst->f14 + seqInst->f18;
    if (seqInst->f18 < 0) {
        if (!(seqInst->f1C < seqInst->f14)) {
            seqInst->f14 = seqInst->f1C;
            seqInst->f18 = 0;
        }
    } else {
        if (!(seqInst->f14 < seqInst->f1C)) {
            seqInst->f14 = seqInst->f1C;
            seqInst->f18 = 0;
        }
    }
    if (seqInst->f14 < 0) {
        SNDseqstop_8DE8(seqInst->f10);
        return 1;
    }
    i = 23;
    voiceSlot = D_0050A820;
    do {
        if (voiceSlot->f04 == id && voiceSlot->f00 >= 0) {
            f_85F0_8DE8(voiceSlot->f00, (unsigned int)(seqInst->f30[voiceSlot->f05].b10 * (seqInst->f14 >> 16)) >> 7);
        }
        i--;
        voiceSlot++;
    } while (i >= 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiseqover);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_8EE8 { int id; signed char owner; char pad[3]; };
extern struct E_8EE8 D_0050A820_8EE8[] __asm__("D_0050A820");
extern int SNDVOICEI_get_8EE8(int) __asm__("SNDVOICEI_get");

/**
 * Tells whether all of a sequence's notes have finished.
 *
 * Looks for a note voice of the slot whose handle is still live (SNDVOICEI_get).
 *
 * @param seqIndex  the sequence slot
 * @return 1 when none is playing, else 0
 */
int SNDI_midiseqover(int seqIndex) {
    int i;
    int isOver = 1;
    for (i = 0; i < 0x18; i++) {
        if (D_0050A820_8EE8[i].owner == seqIndex && D_0050A820_8EE8[i].id >= 0 && SNDVOICEI_get_8EE8(D_0050A820_8EE8[i].id) >= 0) {
            isOver = 0;
            break;
        }
    }
    return isOver;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiparsenoteon);
#ifdef SKIP_ASM
struct sSongHandle_8F78 { int id; };
extern int D_0044C474;              /* last voice slot handed out */
extern int SNDVOICEI_get_8F78(int) __asm__("SNDVOICEI_get");
extern void SNDplaysetdef_8F78(struct SNDPLAYOPTS *) __asm__("SNDplaysetdef");
extern int SNDBANK_play_8F78(int, int, struct SNDPLAYOPTS *) __asm__("SNDBANK_play");

/**
 * Plays a MIDI note-on: picks a free note voice and plays the channel's patch.
 *
 * Searches the 24 note voices round-robin from D_0044C474 for one whose sound has ended; with none it only skips the
 * velocity byte. Otherwise it fills play options from SNDplaysetdef defaults: volume = channel volume * sequence volume
 * / 128, velocity from the stream, azimuth = channel pan + the sequence's base azimuth, pitch multiplier and bend from
 * the sequence and channel, and plays through SNDBANK_play from the drum bank (+0x2E, key as patch, key 0x3C) on
 * channel 9 or the channel's program from the main bank (+0x2C). The voice remembers sequence, channel, key and
 * velocity.
 *
 * Sequences live in sndms (16 records of 0x70 bytes: +0x08 read position, +0x0C loop start, +0x10 handle, +0x14
 * volume 16.16, +0x18 fade step, +0x1C fade target, +0x20 base azimuth, +0x22 pitch multiplier, +0x24 tempo percent,
 * +0x27 ticks to wait, +0x28 state, +0x2C/+0x2E bank handles, +0x30 sixteen channel words); D_0050A820 holds 24 note
 * voices (handle, sequence, channel, key, velocity).
 *
 * @param seqSlot  the sequence slot
 * @param midiChan  the MIDI channel state
 * @param ch  the MIDI channel
 * @param key  the key
 */
void SNDI_midiparsenoteon(struct sSongHandle_8F78 seqSlot, struct sA120Chan *midiChan, int ch, int key) {
    struct SNDPLAYOPTS parm;
    struct sA120Song *seqInst = &sndms.f000[seqSlot.id];
    struct sA120Voice *voiceSlot = 0;
    int found = 0;
    int i, end;
    int bankHandle, patchNum;
    struct sA120Chan cc;
    unsigned short azimuth;
    unsigned char chanBend;

    end = D_0044C474 + 25;
    i = ++D_0044C474;
    for (; i < end; i++) {
        D_0044C474 = i % 24;
        voiceSlot = &D_0050A820[D_0044C474];
        if (SNDVOICEI_get_8F78(voiceSlot->f00) < 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        SNDplaysetdef_8F78(&parm);
        cc = *midiChan;
        parm.vol = (unsigned int)(cc.b10 * (seqInst->f14 >> 16)) >> 7;
        parm.velocity = *seqInst->f08;
        azimuth = cc.b00 << 6;
        chanBend = cc.b17; /* FAKEMATCH: early read, see header */
        azimuth += seqInst->f20;
        parm.azimuth = azimuth;
        parm.pitchmult = seqInst->f22;
        parm.bend = chanBend;
        if (ch == 9 && seqInst->f2E >= 0) {
            bankHandle = seqInst->f2E;
            patchNum = key;
            parm.keynum = 0x3C;
        } else {
            patchNum = midiChan->b24;
            bankHandle = seqInst->f2C;
            parm.keynum = key;
        }
        voiceSlot->f00 = SNDBANK_play_8F78(bankHandle, patchNum, &parm);
        voiceSlot->f07 = *seqInst->f08++;
        if (voiceSlot->f00 >= 0) {
            voiceSlot->f04 = seqSlot.id;
            voiceSlot->f05 = ch;
            voiceSlot->f06 = key;
        }
    } else {
        seqInst->f08++;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiparsenoteoff);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct V_9140 { int h; signed char ch; signed char a; signed char b; signed char st; };
struct Ch_9140 { char pad[8]; int cnt; char pad2[0x64]; };
extern char sndms_9140[] __asm__("sndms");
extern void SNDrelease_9140(int) __asm__("SNDrelease");

/**
 * Plays a MIDI note-off: releases the matching note voice.
 *
 * Skips the velocity byte (advances the sequence read position +0x08) and releases the first note voice of the sequence
 * with that channel and key that is still sounding (SNDrelease), marking it released.
 *
 * @param ch  the sequence slot
 * @param midiChannel  the MIDI channel
 * @param keyNum  the key
 */
void SNDI_midiparsenoteoff(int ch, int midiChannel, int keyNum) {
    struct V_9140 *voiceSlot;
    int i;

    ((struct Ch_9140 *)sndms_9140)[ch].cnt++;
    voiceSlot = (struct V_9140 *)(sndms_9140 + 0x700);
    for (i = 0; i < 0x18; i++, voiceSlot++) {
        if (voiceSlot->ch == ch && voiceSlot->a == midiChannel && voiceSlot->b == keyNum && voiceSlot->st >= 0) {
            SNDrelease_9140(voiceSlot->h);
            voiceSlot->st = -1;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiparsecontrol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Ch_91E0 {
    char pad00[0x8];
    unsigned char *p8;
    unsigned char *pC;
    char pad10[0x16 - 0x10];
    short f16;
    char pad18[0x20 - 0x18];
    unsigned short f20;
    char pad22[0x26 - 0x22];
    unsigned char f26;
    char pad27[0x70 - 0x27];
};
struct V_91E0 {
    int h;
    signed char ch;
    signed char x;
    char pad[2];
};
struct G_91E0 {
    struct Ch_91E0 ch[16];
    struct V_91E0 v[24];
};
struct Bits_91E0 {
    unsigned int a : 10;
    unsigned int b : 7;
};
extern struct G_91E0 sndms_91E0 __asm__("sndms");
extern void SNDvol(int, unsigned int);
extern int SNDI_pantoazimuth_91E0(int v) __asm__("SNDI_pantoazimuth");

/**
 * Handles a MIDI control change for a sequence channel.
 *
 * Controller 20 marks the loop start (+0x0C) after the value byte; 30 loops back to it, the value byte giving the
 * repeat count (0x7F or more means forever); 7 sets the channel volume and reapplies it to the channel's notes (channel
 * volume * sequence volume / 128 through SNDvol); 10 sets the channel pan (SNDI_pantoazimuth) and repositions the
 * channel's notes with SND3dpos (pan + base azimuth, elevation 0). Other controllers skip their value byte.
 *
 * @param seqIndex  the sequence slot
 * @param bits  the MIDI channel state
 * @param x  the MIDI channel
 * @param cmd  the controller number
 */
void SNDI_midiparsecontrol(int seqIndex, struct Bits_91E0 *bits, int x, int cmd) {
    struct V_91E0 *voiceSlot = (struct V_91E0 *)&sndms_91E0;
    struct Ch_91E0 *seqInst = (struct Ch_91E0 *)((char *)voiceSlot + seqIndex * 0x70);
    unsigned char *readPos;
    int i;

    if (cmd == 20) {
        unsigned char *loopStart = seqInst->p8 + 1;
        seqInst->p8 = loopStart;
        seqInst->pC = loopStart;
        return;
    } else if (cmd == 30) {
        if (seqInst->f26 >= 0x7F) {
            seqInst->f26 = *seqInst->p8;
        }
        if (seqInst->f26 != 0) {
            seqInst->f26--;
            seqInst->p8 = seqInst->pC;
            return;
        }
    } else if (cmd == 7) {
        readPos = seqInst->p8;
        voiceSlot = (struct V_91E0 *)((char *)voiceSlot + 0x700);
        i = 23;
        bits->b = *readPos;
        seqInst->p8 = readPos + 1;
        do {
            if (voiceSlot->ch == seqIndex && voiceSlot->x == x) {
                SNDvol(voiceSlot->h, (unsigned int)(bits->b * seqInst->f16) >> 7);
            }
            i--;
            voiceSlot++;
        } while (i >= 0);
        return;
    } else if (cmd == 10) {
        voiceSlot = (struct V_91E0 *)((char *)voiceSlot + 0x700);
        i = 23;
        bits->a = SNDI_pantoazimuth_91E0(*seqInst->p8++) >> 6;
        do {
            if (voiceSlot->ch == seqIndex && voiceSlot->x == x) {
                int azimuthValue = ((*(unsigned int *)bits & 0x3FF) << 6) + seqInst->f20;
                SND3dpos(voiceSlot->h, (unsigned short)azimuthValue, 0);
            }
            i--;
            voiceSlot++;
        } while (i >= 0);
        return;
    }
    seqInst->p8++;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiparsepitchbend);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct B_93C8 { unsigned int lo : 17; unsigned int f : 7; unsigned int hi : 8; };
struct E_93C8 { int id; signed char owner; signed char sub; short pad; };
struct R_93C8 { char pad0[8]; unsigned char *cur; char pad1[0x70 - 12]; };
struct G_93C8 { struct R_93C8 r[16]; struct E_93C8 e[24]; };
extern struct G_93C8 D_a120_93C8 __asm__("sndms");
extern void fC6B8_93C8(int, int) __asm__("SNDpitchbend");

/**
 * Handles a MIDI pitch bend for a sequence channel.
 *
 * Combines the next stream byte (high) with the data byte (low) and stores value / 128 (so only the high byte) as the
 * channel bend, then bends each of the channel's notes with SNDpitchbend.
 *
 * @param seqIndex  the sequence slot
 * @param midiChan  the MIDI channel state
 * @param channel  the MIDI channel
 * @param data  the low data byte
 */
void SNDI_midiparsepitchbend(int seqIndex, struct B_93C8 *midiChan, int channel, int data) {
    struct R_93C8 *seqInst = &D_a120_93C8.r[seqIndex];
    int value, i;
    struct E_93C8 *partial = &D_a120_93C8.e[0];
    i = 23;
    value = (*seqInst->cur++ << 7) + data;
    midiChan->f = value / 128;
    do {
        i -= 1;
        if (partial->owner == seqIndex && partial->sub == channel) {
            fC6B8_93C8(partial->id, midiChan->f);
        }
        partial += 1;
    } while (i >= 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiprocesstick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndms: the MIDI sequence player (16 songs, then 24 voices, also D_0050A820). Layout from every use in this
   unit; field names are their offsets. */
extern struct sA120 sndms;
extern int f8EE8_94B0(int) __asm__("SNDI_midiseqover");
extern void f9920_94B0(int) __asm__("SNDseqstop");
extern void f8F78_94B0(int, struct sA120Chan *, int, int) __asm__("SNDI_midiparsenoteon");
extern void f9140_94B0(int, int, int) __asm__("SNDI_midiparsenoteoff");
extern void f91E0_94B0(int, struct sA120Chan *, int, int) __asm__("SNDI_midiparsecontrol");
extern void f93C8_94B0(int, struct sA120Chan *, int, int) __asm__("SNDI_midiparsepitchbend");

/**
 * Runs one tick of a sequence: waits out the delay, then parses events until the next delay.
 *
 * Events are three bytes (ticks to wait after it, status, first data byte). An end of track (status 0xF0-0xFF, data 47)
 * stops the sequence (SNDseqstop) once all notes are over (SNDI_midiseqover), otherwise it waits on it. Status 0x90
 * goes to SNDI_midiparsenoteon, 0x80 to SNDI_midiparsenoteoff, 0xB0 to SNDI_midiparsecontrol, 0xE0 to SNDI_midiparsepitchbend and 0xC0 sets the channel
 * program; parsing stops at a nonzero delay or when the sequence leaves state 3.
 *
 * @param seqIndex  the sequence slot
 */
void SNDI_midiprocesstick(int seqIndex) {
    struct sA120Song *seqInst = &sndms.f000[seqIndex];
    unsigned char ev, statusByte, dataByte;
    unsigned char *readPos;
    int hi, lo;

    readPos = seqInst->f08;
    readPos++;
    seqInst->f08 = readPos;
    seqInst->f27--;
    hi = *readPos & 0xF0;
    seqInst->f08 = readPos + 1;
    dataByte = readPos[1];
    seqInst->f08 = readPos - 1;
    if (hi == 0xF0 && dataByte == 47) {
        if (f8EE8_94B0(seqIndex) != 0) f9920_94B0(seqInst->f10);
        return;
    }
    if (seqInst->f27 != 0) return;
    for (;;) {
        struct sA120Chan *midiChan;
        ev = *seqInst->f08++;
        statusByte = *seqInst->f08++;
        dataByte = *seqInst->f08++;
        hi = statusByte & 0xF0;
        lo = statusByte & 0xF;
        midiChan = &seqInst->f30[lo];
        switch (hi) {
        case 144:
            f8F78_94B0(seqIndex, midiChan, lo, dataByte);
            break;
        case 0x80:
            f9140_94B0(seqIndex, lo, dataByte);
            break;
        case 176:
            f91E0_94B0(seqIndex, midiChan, lo, dataByte);
            break;
        case 0xE0:
            f93C8_94B0(seqIndex, midiChan, lo, dataByte);
            break;
        case 0xC0:
            midiChan->b24 = dataByte;
            break;
        case 0xF0:
            if (dataByte == 47) {
                if (f8EE8_94B0(seqIndex) != 0) {
                    f9920_94B0(seqInst->f10);
                } else {
                    ev = 1;
                    seqInst->f08 -= 3;
                    seqInst->f27++;
                }
            }
            break;
        }
        if (ev != 0) break;
        if (seqInst->f28 != 3) break;
    }
    seqInst->f27 = ev;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDI_midiserver);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDI_midiupdatefade_96B8(int) __asm__("SNDI_midiupdatefade");
extern void SNDI_midiprocesstick_96B8(int) __asm__("SNDI_midiprocesstick");
struct E_96B8 { int f0; unsigned int f4; char pad[0x18 - 8]; int f18; char pad2[0x24 - 0x1C]; unsigned short u24; char pad4[2]; signed char st; char pad3[0x70 - 0x29]; };
extern struct E_96B8 sndms_96B8[] __asm__("sndms");

/**
 * Runs the sequencer at 100 Hz: fades and ticks every playing sequence.
 *
 * For each sequence in state 3 whose fade (SNDI_midiupdatefade, only while a step is set) did not stop it, advances its tick
 * counter and runs SNDI_midiprocesstick until the processed ticks catch up with counter * tempo percent (+0x24) / 100.
 */
void SNDI_midiserver(void) {
    int i;
    struct E_96B8 *seqInst;
    signed char st;
    unsigned int lim;
    int tickCount;
    for (i = 0; i < 16; i++) {
        seqInst = &sndms_96B8[i];
        st = seqInst->st;
        if (st == 3 && (seqInst->f18 == 0 || SNDI_midiupdatefade_96B8(i) == 0)) {
            tickCount = seqInst->f0;
            lim = (unsigned int)(tickCount * seqInst->u24) / 100u;
            seqInst->f0 = tickCount + 1;
            if (lim >= seqInst->f4 && seqInst->st == st) {
                do {
                    SNDI_midiprocesstick_96B8(i);
                    seqInst->f4++;
                } while (lim >= seqInst->f4 && seqInst->st == 3);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDmidiinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_97D8 { int id; signed char owner; signed char b5; char b6; char b7; };
struct C_97D8 { char pad[0x10]; int f10; char pad2[0x14]; char f28; char pad3[0x47]; };
struct G_97D8 { char pad[0x1E8]; int (*f1E8)(void); };
extern struct E_97D8 D_0050A820_97D8[] __asm__("D_0050A820");
extern struct C_97D8 sndms_97D8[] __asm__("sndms");
extern void SNDSYS_add100hzclient_97D8(void *) __asm__("SNDSYS_add100hzclient");
extern void SNDI_midiserver_97D8(void) __asm__("SNDI_midiserver");
extern int SNDmidirestore_97D8(void) __asm__("SNDmidirestore");

extern struct G_97D8 sndgs_97D8 __asm__("sndgs");
/**
 * Starts the MIDI sequencer: clears all sequences and note voices and adds its 100 Hz client.
 *
 * In the critical section: frees the 24 note voices and the 16 sequence slots, adds SNDI_midiserver as a 100 Hz client
 * and installs SNDmidirestore as the sequencer shutdown hook (the sound driver state sndgs +0x1E8).
 *
 * @return 0
 */
int SNDmidiinit(void) {
    int i;
    struct C_97D8 *seqInst;

    SNDSYS_entercritical();
    for (i = 0; i < 24; i++) {
        D_0050A820_97D8[i].b5 = -1;
        D_0050A820_97D8[i].b6 = 0;
        D_0050A820_97D8[i].b7 = 0;
        D_0050A820_97D8[i].id = -1;
    }
    seqInst = sndms_97D8;
    do {
        seqInst->f10 = -1;
        seqInst->f28 = 0;
        seqInst++;
    } while ((int)seqInst < (int)(sndms_97D8 + 16));
    SNDSYS_add100hzclient_97D8(SNDI_midiserver_97D8);
    sndgs_97D8.f1E8 = SNDmidirestore_97D8;
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDmidirestore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_remove100hzclient_9880(void (*)(void)) __asm__("SNDSYS_remove100hzclient");
extern void SNDI_midiserver_9880(void) __asm__("SNDI_midiserver");
extern void SNDmidistopall_9880(void) __asm__("SNDmidistopall");

/**
 * Shuts the MIDI sequencer down: stops all sequences and removes its 100 Hz client.
 *
 * In the critical section: SNDmidistopall, then SNDSYS_remove100hzclient(SNDI_midiserver), and clears the shutdown hook D_0050AAD0.
 *
 * @return 0
 */
int SNDmidirestore(void) {
    SNDSYS_entercritical();
    SNDmidistopall_9880();
    SNDSYS_remove100hzclient_9880(SNDI_midiserver_9880);
    *(int*)&sndgs.midirestore = 0;
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDmidistopall);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a[4]; int v; int pad[0x17]; } E_98C0;
extern void SNDSYS_entercritical();
extern void SNDSYS_leavecritical();
extern void SNDseqstop(int);
extern E_98C0 sndms_A120_98C0[] __asm__("sndms");

/**
 * Stops all 16 sequences (SNDseqstop on each slot's handle) inside the critical section.
 *
 * @return 0
 */
int SNDmidistopall(void) {
    E_98C0 *tbl;
    int *seqHandlePtr;
    int i;

    i = 0xF;
    SNDSYS_entercritical();
    tbl = sndms_A120_98C0;
    seqHandlePtr = &tbl->v;
    do {
        i -= 1;
        SNDseqstop(*seqHandlePtr);
        seqHandlePtr += 0x1C;
    } while (i >= 0);
    SNDSYS_leavecritical();
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqstop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct C_9920 { char pad[0x10]; int f10; char pad2[0x14]; signed char f28; char pad3[0x47]; };
struct V_9920 { int h; signed char ch; signed char b5; char b6; char b7; };
extern struct C_9920 sndms_9920[] __asm__("sndms");
extern void SNDstop_9920(int) __asm__("SNDstop");

int SNDseqstop_9920(int h) __asm__("SNDseqstop");

int SNDseqstop_impl(int h) __asm__("SNDseqstop");
/**
 * Stops a sequence and its notes.
 *
 * Fails with -1 when the slot is free (state 0). Otherwise stops every note voice of the sequence (SNDstop),
 * frees them, clears the slot handle and sets state 2.
 *
 * @param seqHandle  the sequence handle
 * @return 0, -1 when not active, or -8 for an invalid handle
 */
int SNDseqstop_impl(int seqHandle) {
    int ch = SNDI_midigetseqindex(seqHandle);
    struct C_9920 *seqInst;
    struct V_9920 *voiceSlot;
    int i;

    if (ch < 0) return ch;
    seqInst = &sndms_9920[ch];
    if (seqInst->f28 != 0) {
        voiceSlot = (struct V_9920 *)((char *)sndms_9920 + 0x700);
        for (i = 0; i < 24; i++, voiceSlot++) {
            if (voiceSlot->ch == ch) {
                if (voiceSlot->h >= 0) SNDstop_9920(voiceSlot->h);
                voiceSlot->b5 = -1;
                voiceSlot->b6 = 0;
                voiceSlot->b7 = 0;
                voiceSlot->h = -1;
            }
        }
        seqInst->f10 = -1;
        seqInst->f28 = 2;
        return 0;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIDI_play);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndms: the MIDI sequence player (16 songs, then 24 voices, also D_0050A820). Layout from every use in this
   unit; field names are their offsets. */
extern struct sA120 sndms;
struct H_9A00 { signed char f00; char pad01[7]; unsigned short f08; char pad0A[2]; unsigned short f0C; char pad0E[2]; unsigned short f10; };

/**
 * Starts playing a MIDI sequence in a free sequence slot.
 *
 * Skips an "MD" header (8 bytes) or a "MIDx" tag (4 bytes), bumps the handle counter (+0x7C0 by 0x20) and takes the
 * first free slot: volume from the options' +0x00, base azimuth +0x08, pitch multiplier +0x0C and tempo (+0x10 * 100 >>
 * 12, in percent), the two banks, state 3 (playing) and default channels (volume 0x7F, bend 0x40, no program).
 *
 * Sequences live in sndms (16 records of 0x70 bytes: +0x08 read position, +0x0C loop start, +0x10 handle, +0x14
 * volume 16.16, +0x18 fade step, +0x1C fade target, +0x20 base azimuth, +0x22 pitch multiplier, +0x24 tempo percent,
 * +0x27 ticks to wait, +0x28 state, +0x2C/+0x2E bank handles, +0x30 sixteen channel words); D_0050A820 holds 24 note
 * voices (handle, sequence, channel, key, velocity).
 *
 * @param hdr  the play options
 * @param data  the sequence data
 * @param bankHandle  the bank for normal channels
 * @param drumBankHandle  the bank for channel 9 (drums)
 * @return the sequence handle, or -9 when all slots are busy
 */
int SNDMIDI_play(struct H_9A00 *hdr, unsigned char *data, int bankHandle, int drumBankHandle) {
    int ret = -9;
    struct sA120Song *seqInst;
    int i;
    int j;
    if (*(short *)data == 17485) {
        data += 8;
    } else if (*(int *)data == 0x7844494D) {
        data += 4;
    }
    sndms.f7C0 += 0x20;
    if (sndms.f7C0 < 0) sndms.f7C0 = 0;
    for (i = 0; i < 16; i++) {
        seqInst = &sndms.f000[i];
        if (seqInst->f10 < 0) {
            seqInst->f04 = 0;
            seqInst->f00 = 0;
            seqInst->f24 = (hdr->f10 * 100) >> 12;
            seqInst->f10 = sndms.f7C0 | i;
            seqInst->f08 = data;
            seqInst->f2C = bankHandle;
            seqInst->f2E = drumBankHandle;
            seqInst->f26 = 0xFF;
            seqInst->f27 = 1;
            seqInst->f18 = 0;
            seqInst->f0C = 0;
            seqInst->f14 = hdr->f00 << 16;
            seqInst->f28 = 3;
            seqInst->f20 = hdr->f08;
            seqInst->f22 = hdr->f0C;
            for (j = 0; j < 16; j++) {
                struct sA120Chan *midiChan = &seqInst->f30[j];
                midiChan->b00 = 0;
                midiChan->b10 = 0x7F;
                midiChan->b17 = 0x40;
                midiChan->b24 = -1;
            }
            ret = seqInst->f10;
            break;
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIDI_pitchmult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct C_9B68 { char pad[0x22]; unsigned short f22; char pad2[4]; signed char f28; char pad3[0x47]; };
struct V_9B68 { int h; signed char ch; char pad[3]; };
extern struct C_9B68 sndms_9B68[] __asm__("sndms");
extern int SNDpitchmult_9B68(int, int) __asm__("SNDpitchmult");

/**
 * Sets a playing sequence's pitch multiplier.
 *
 * Stores it (+0x22; new notes copy it) and passes it to every playing note voice with SNDpitchmult. Fails with -1 when
 * the value changes and the sequence is not playing.
 *
 * @param seqHandle  the sequence handle
 * @param pitchMult  the pitch multiplier (0x1000 = 1.0)
 * @return 0, -1, or -8 for an invalid handle
 */
int SNDMIDI_pitchmult(int seqHandle, int pitchMult) {
    int result = -1;
    int ch = SNDI_midigetseqindex(seqHandle);
    struct C_9B68 *seqInst;
    struct V_9B68 *voiceSlot;
    int i;

    if (ch < 0) return ch;
    seqInst = &sndms_9B68[ch];
    if (pitchMult != seqInst->f22) {
        if (seqInst->f28 != 3) goto out;
        seqInst->f22 = pitchMult;
        voiceSlot = (struct V_9B68 *)((char *)sndms_9B68 + 0x700);
        for (i = 23; i >= 0; i--, voiceSlot++) {
            if (voiceSlot->ch == ch && voiceSlot->h >= 0) SNDpitchmult_9B68(voiceSlot->h, pitchMult);
        }
    }
    result = 0;
out:
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIDI_azimuth);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct C_9C20 { char pad[0x20]; unsigned short f20; char pad2[6]; signed char f28; char pad3[7]; int w30[16]; };
struct V_9C20 { int h; signed char ch; signed char b5; char pad[2]; };
extern struct C_9C20 sndms_9C20[] __asm__("sndms");
extern void SND3dpos_9C20(int, unsigned short, int) __asm__("SND3dpos");

int SNDMIDI_azimuth(int h, int v) __asm__("SNDMIDI_azimuth");

/**
 * Sets a playing sequence's base azimuth.
 *
 * Stores it (+0x20) and repositions every playing note with SND3dpos at base + the note channel's pan (elevation
 * 0). Fails with -1 when the value changes and the sequence is not playing.
 *
 * @param seqHandle  the sequence handle
 * @param azimuth  the azimuth
 * @return 0, -1, or -8 for an invalid handle
 */
int SNDMIDI_azimuth(int seqHandle, int azimuth) {
    int result = -1;
    int ch = SNDI_midigetseqindex(seqHandle);
    struct C_9C20 *seqInst;
    struct V_9C20 *voiceSlot;
    int i;

    if (ch < 0) return ch;
    seqInst = &sndms_9C20[ch];
    if (azimuth != seqInst->f20) {
        if (seqInst->f28 != 3) goto out;
        seqInst->f20 = azimuth;
        voiceSlot = (struct V_9C20 *)((char *)sndms_9C20 + 0x700);
        for (i = 23; i >= 0; i--, voiceSlot++) {
            if (voiceSlot->ch == ch && voiceSlot->h >= 0) {
                azimuth = seqInst->f20 + ((*(int *)((char *)seqInst + (voiceSlot->b5 << 2) + 0x30) & 0x3FF) << 6);
                SND3dpos_9C20(voiceSlot->h, azimuth, 0);
            }
        }
    }
    result = 0;
out:
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqpause);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDstop_9D00(int) __asm__("SNDstop");
extern int sndms_9D00[] __asm__("sndms");
typedef struct { int a; signed char b; char pad[3]; } E_9D00;

/**
 * Pauses a sequence: stops its notes and sets state 5.
 *
 * Only a playing sequence (state 3) changes. Runs in the critical section.
 *
 * @param seqHandle  the sequence handle
 * @return the new status from SNDseqstatus, or -8 for an invalid handle
 */
int SNDseqpause(int seqHandle) {
    int status, seqIndex, i;

    E_9D00 *voiceSlot;
    char *seqInst;

    SNDSYS_entercritical();
    seqIndex = SNDI_midigetseqindex(seqHandle);
    if (seqIndex < 0) {
        SNDSYS_leavecritical();
        return seqIndex;
    }
    seqInst = (char*)sndms_9D00 + seqIndex * 0x70;
    if (*(signed char*)(seqInst + 0x28) == 3) {
        voiceSlot = (E_9D00*)(((char*)sndms_9D00) + 0x700);
        *(signed char*)(seqInst + 0x28) = 5;
        i = 23;
        do {
            i--;
            if (voiceSlot->b == seqIndex) {
                if (voiceSlot->a >= 0) SNDstop_9D00(voiceSlot->a);
            }
            voiceSlot++;
        } while (i >= 0);
    }
    status = SNDseqstatus(seqHandle);
    SNDSYS_leavecritical();
    return status;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqresume);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_entercritical();
extern void SNDSYS_leavecritical();
extern int SNDI_midigetseqindex(int);
extern int SNDseqstatus_9DC8(int) __asm__("SNDseqstatus");
extern char sndms_9DC8[] __asm__("sndms");

/**
 * Resumes a paused sequence (state 5 back to 3).
 *
 * Runs in the critical section. Notes stopped by the pause are not restarted.
 *
 * @param seqHandle  the sequence handle
 * @return the new status from SNDseqstatus, or -8 for an invalid handle
 */
int SNDseqresume(int seqHandle) {
    int var_16;
    char *temp_4;

    SNDSYS_entercritical();
    var_16 = SNDI_midigetseqindex(seqHandle);
    if (var_16 < 0) {
        SNDSYS_leavecritical();
        return var_16;
    }
    temp_4 = sndms_9DC8 + var_16 * 0x70;
    if (*(signed char *)(temp_4 + 0x28) == 5) {
        *(signed char *)(temp_4 + 0x28) = 3;
    }
    var_16 = SNDseqstatus_9DC8(seqHandle);
    SNDSYS_leavecritical();
    return var_16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqvol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char sndms_9E48[] __asm__("sndms");
extern int SNDI_midigetseqindex_9E48() __asm__("SNDI_midigetseqindex");
extern void SNDvol(int, unsigned int);

/**
 * Sets a playing sequence's volume immediately, cancelling its fade.
 *
 * The handle passes through in $a0 to SNDI_midigetseqindex. Clears the fade step and, when the volume changes, stores it
 * (16.16) and updates every note (channel volume * volume / 128 through SNDvol); a change on a sequence that is
 * not playing fails with -1.
 *
 * @param seqHandle  the sequence handle
 * @param vol  the volume (0..127)
 * @return 0, -1, or -8 for an invalid handle
 */
int SNDseqvol(int seqHandle, int vol) {
    int ret = 0;
    int idx = SNDI_midigetseqindex_9E48();
    if (idx < 0) {
        return idx;
    }
    {
        int key = vol << 16;
        char *seqInst = sndms_9E48 + idx * 0x70;
        *(int *)(seqInst + 0x18) = 0;
        if (*(int *)(seqInst + 0x14) != key) {
            if (*(signed char *)(seqInst + 0x28) == 3) {
                char *voiceSlot;
                int i;
                *(int *)(seqInst + 0x14) = key;
                voiceSlot = sndms_9E48 + 0x700;
                i = 23;
                do {
                    if (*(signed char *)(voiceSlot + 4) == idx) {
                        int voiceHandle = *(int *)voiceSlot;
                        if (voiceHandle >= 0) {
                            unsigned x = *(unsigned *)(seqInst + (*(signed char *)(voiceSlot + 5) << 2) + 0x30);
                            SNDvol(voiceHandle, ((x >> 10 & 0x7F) * vol) >> 7);
                        }
                    }
                    i -= 1;
                    voiceSlot += 8;
                } while (i >= 0);
            } else {
                ret = -1;
            }
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqautovol);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char sndms_9F38[] __asm__("sndms");

/**
 * Fades a playing sequence's volume to a target over a number of 100 Hz ticks.
 *
 * Sets the target (+0x1C) and the per-tick step (+0x18) from the current volume; SNDI_midiupdatefade applies it. A time of 0
 * or less counts as 1; a sequence that is not playing is left alone.
 *
 * @param seqHandle  the sequence handle
 * @param fadeTicks  the fade time in ticks
 * @param targetVol  the target volume (0..127)
 * @return 0, or -8 for an invalid handle
 */
int SNDseqautovol(int seqHandle, int fadeTicks, int targetVol) {
    int seqIndex;
    char *seqInst;
    int targetFixed;
    seqIndex = SNDI_midigetseqindex(seqHandle);
    if (seqIndex < 0) return seqIndex;
    seqInst = sndms_9F38 + seqIndex * 0x70;
    if (*(signed char *)(seqInst + 0x28) == 3) {
        if (fadeTicks <= 0) fadeTicks = 1;
        targetFixed = targetVol << 16;
        *(int *)(seqInst + 0x1C) = targetFixed;
        *(int *)(seqInst + 0x18) = (targetFixed - *(int *)(seqInst + 0x14)) / fadeTicks;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDseqstatus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char SNDI_midigetseqindex_9FC8(void) __asm__("SNDI_midigetseqindex");
extern char sndms_A120[] __asm__("sndms");

/**
 * Returns a sequence's status.
 *
 * The handle passes through in $a0 to SNDI_midigetseqindex. The state byte (+0x28): 3 playing, 5 paused, 2 stopped; a playing
 * sequence with a fade step set reports 6.
 *
 * @return the status, or -8 for an invalid handle
 */
signed char SNDseqstatus(void) {
    signed char var_5;
    char *temp_2;

    var_5 = SNDI_midigetseqindex_9FC8();
    if (var_5 >= 0) {
        temp_2 = sndms_A120 + var_5 * 0x70;
        var_5 = *(signed char *)(temp_2 + 0x28);
        if (var_5 == 3) {
            var_5 = (*(int *)(temp_2 + 0x18) != 0) ? 6 : var_5;
        }
    }
    return var_5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDplaysetdef);
#ifdef SKIP_ASM

/**
 * Fills play options with their defaults.
 *
 * Volume, velocity, dry and effect levels 0x7F, bend 0x40, key 0x3C, azimuth and elevation 0, pitch, time and tempo
 * multipliers 0x1000 (1.0), low-pass 0xFFFF and high-pass 0.
 *
 * @param playOpts  the play options (0x18 bytes)
 * @return 0
 */
int SNDplaysetdef(struct SNDPLAYOPTS *playOpts) {
    playOpts->pitchmult = 0x1000;
    playOpts->keynum = 0x3C;
    playOpts->timemult = 0x1000;
    playOpts->tempomult = 0x1000;
    playOpts->pad2 = 0;
    playOpts->lowpasscutoff = 0xFFFF;
    playOpts->highpasscutoff = 0;
    playOpts->bend = 0x40;
    playOpts->velocity = 0x7F;
    playOpts->vol = 0x7F;
    playOpts->drylevel = 0x7F;
    playOpts->fxlevel0 = 0x7F;
    playOpts->azimuth = 0;
    playOpts->elevation = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDVOICEI_isreserved);
#ifdef SKIP_ASM
extern short sndvoicereserved[];

/**
 * Tells whether a channel is already in the allocator's pick list.
 *
 * Searches the first entries of sndvoicereserved.
 *
 * @param voice  the channel
 * @param voicesAllocated  how many entries are filled
 * @return 1 when listed, else 0
 */
int SNDVOICEI_isreserved(int voice, int voicesAllocated) {
    int i;
    for (i = 0; i < voicesAllocated; i++) {
        if (sndvoicereserved[i] == voice) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDVOICEI_alloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
extern short sndvoicereserved_A0B0[] __asm__("sndvoicereserved");
extern int D_0044C478;
extern int f_A078_A0B0(int, int) __asm__("SNDVOICEI_isreserved");
extern void f_89E0_A0B0(int) __asm__("SNDstop");

/**
 * Allocates channels for a sound, stealing lower-priority ones when needed, and makes its handle.
 *
 * Within the channel range, takes free channels oldest first (timestamp +0x2C); the rest are stolen from busy channels
 * with a lower priority (+0x2A, at most 100), equal priority only when the steal option (options +0x1C) is set. With
 * enough channels it sorts them, builds the handle from a counter (D_0044C478, steps of 0x100) and the first channel,
 * stops stolen sounds (SNDstop), marks each busy (state 1, tick, priority) and makes the first the master (voice
 * list, handle) with the others pointing to it.
 *
 * @param n  channels needed
 * @param tag  the priority
 * @param out  receives the handle
 * @param lo  first channel to use
 * @param hi  end of the range
 * @return the master channel, or -9 when not enough could be had
 */
short SNDVOICEI_alloc(int n, int tag, unsigned int *out, int lo, int hi) {
    int result = -9;
    int sel = 0;
    int i;
    int k;
    int best;
    unsigned int key;
    int lim;
    struct CHANPUB *channel;

    for (i = 0; i < n; i++) {
        sndvoicereserved_A0B0[i] = -1;
    }
    D_0044C478 += 0x100;
    if (D_0044C478 < 0) {
        D_0044C478 = 0;
    }
    for (i = 0; i < n; i++) {
        best = -1;
        key = 0xFFFFFFFF;
        for (k = lo; k < hi; k++) {
            channel = &sndgs.chan[k];
            if (channel->status == 0 && f_A078_A0B0(k, sel) == 0 && channel->timestamp < key) {
                key = channel->timestamp;
                best = k;
            }
        }
        if (best >= 0) {
            sndvoicereserved_A0B0[sel] = best;
            sel++;
        }
    }
    for (i = sel; i < n; i++) {
        best = -1;
        lim = tag;
        if (sndgs.sso.set.stealequalpriorityvoices == 0) lim = tag - 1;
        key = 0xFFFFFFFF;
        for (k = lo; k < hi; k++) {
            if (f_A078_A0B0(k, sel) == 0) {
                channel = &sndgs.chan[k];
                if (channel->priority < 101) {
                    if (channel->priority < lim) {
                        lim = channel->priority;
                        key = channel->timestamp;
                        best = k;
                    } else if (channel->priority == lim && channel->timestamp < key) {
                        key = channel->timestamp;
                        best = k;
                    }
                }
            }
        }
        if (best >= 0) {
            sndvoicereserved_A0B0[sel] = best;
            sel++;
            if (sel >= n) break;
        }
    }
    if (sel == n) {
        int sorted;
        do {
            sorted = 1;
            for (i = 0; i < sel - 1; i++) {
                short earlierChan = sndvoicereserved_A0B0[i];
                if (sndvoicereserved_A0B0[i + 1] < earlierChan) {
                    sndvoicereserved_A0B0[i] = sndvoicereserved_A0B0[i + 1];
                    sorted = 0;
                    sndvoicereserved_A0B0[i + 1] = earlierChan;
                }
            }
        } while (!sorted);
        *out = D_0044C478 | sndvoicereserved_A0B0[0];
        result = sndvoicereserved_A0B0[0];
        for (i = 0; i < sel; i++) {
            int w;
            channel = &sndgs.chan[sndvoicereserved_A0B0[i]];
            w = channel->handle;
            if (channel->status != 0) {
                if (w < 0) {
                    w = sndgs.chan[channel->masterchan].handle;
                }
                f_89E0_A0B0(w);
            }
            channel->status = 1;
            channel->timestamp = sndgs.audiotick;
            channel->priority = tag;
        }
        sndgs.chan[sndvoicereserved_A0B0[0]].handle = *out;
        sndgs.chan[sndvoicereserved_A0B0[0]].voices[0] = sndvoicereserved_A0B0[0];
        sndgs.chan[sndvoicereserved_A0B0[0]].masterchan = -1;
        for (i = 1; i < sel; i++) {
            sndgs.chan[sndvoicereserved_A0B0[0]].voices[i] = sndvoicereserved_A0B0[i];
            sndgs.chan[sndvoicereserved_A0B0[i]].handle = -1;
            sndgs.chan[sndvoicereserved_A0B0[i]].masterchan = sndvoicereserved_A0B0[0];
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDVOICEI_free);
#ifdef SKIP_ASM
struct G_A550 { char pad[0x17E]; short cnt; volatile int f180; char pad2[0x1F0 - 0x184]; struct CHANPUB *tab; };
/**
 * Frees a channel, keeping a patch-key group's master until its last member goes.
 *
 * Clears the state, patch key and master flag and stamps the tick, except: when the channel is the group's master
 * (+0x27) and still playing (state 1) it only moves to state 2; when the other remaining member is a master in state 2,
 * both are freed. Likely bug: a group without a master leaves the master index at -1, so the state check reads the
 * entry before the channel table. Called by SNDBANKI_playtimbre and SNDDRV_mixvoicefree.
 *
 * @param i  the channel
 * @param u  unused
 */
void SNDVOICEI_free(int i, unsigned char u) {
    struct CHANPUB *channel = &sndgs.chan[i];
    unsigned char k = channel->patchkey;
    int n = 0;
    int sel = -1;
    int j;
    if (k != 0) {
        for (j = 0; j < sndgs.voicestotal; j++) {
            struct CHANPUB *otherChan = &sndgs.chan[j];
            if (otherChan->patchkey == k && otherChan->handle >= 0 && otherChan->status != 0) {
                n++;
                if (otherChan->ismaster) sel = j;
            }
        }
        channel = &sndgs.chan[i];
        if (n == 1) {
            channel->status = 0;
            channel->patchkey = 0;
            channel->ismaster = 0;
            channel->timestamp = (volatile int)sndgs.audiotick;
            return;
        }
        if (sndgs.chan[sel].status == 2 && i != sel && n == 2) {
            channel->status = 0;
            channel->patchkey = 0;
            channel->ismaster = 0;
            channel->timestamp = (volatile int)sndgs.audiotick;
            sndgs.chan[sel].status = 0;
            sndgs.chan[sel].patchkey = 0;
            sndgs.chan[sel].ismaster = 0;
            sndgs.chan[sel].timestamp = (volatile int)sndgs.audiotick;
            return;
        }
        if (sndgs.chan[sel].status == 1 && i == sel) {
            sndgs.chan[sel].status = 2;
            return;
        }
        channel->status = 0;
        channel->patchkey = 0;
        channel->ismaster = 0;
        channel->timestamp = (volatile int)sndgs.audiotick;
        return;
    }
    channel->status = 0;
    channel->patchkey = 0;
    channel->ismaster = 0;
    channel->timestamp = (volatile int)sndgs.audiotick;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDVOICEI_get);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

int SNDVOICEI_get_A6E8(int arg0) __asm__("SNDVOICEI_get");
/**
 * Resolves a sound handle to its channel.
 *
 * The channel is the handle's low byte; it must be in range, busy (+0x69) and hold the same handle.
 *
 * @param soundHandle  the sound handle
 * @return the channel index, or -8
 */
int SNDVOICEI_get_A6E8(int soundHandle) {
    int var_5;
    char *temp_2;
    char *sndState;

    if ((soundHandle < 0) || (sndState = ((char*)&sndgs), var_5 = soundHandle & 0xFF, (var_5 < *(short *)(sndState + 0x17E)) == 0)) {
        return -8;
    }
    temp_2 = *(char **)(sndState + 0x1F0) + (var_5 * 140);
    if ((*(signed char *)(temp_2 + 0x69) == 0) || (*(int *)temp_2 != soundHandle)) {
        var_5 = -8;
    }
    return var_5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BA740);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
/**
 * Recomputes a sound channel's send level to one effect bus.
 *
 * Takes channel @p i of D_0050AAD8 (0x8C-byte channel records), looks up its bus record with func_003B8218 (the bus
 * number and the channel's render mode, f24), and stores f60[bus] * f61[bus] * the bus record's level byte (f02) >> 6
 * in the channel's per-bus short f62[bus]. f61[bus] is the send level func_003B88C8 stores (0..127). The per-bus byte
 * arrays f60/f61 are both indexed by the bus number from adjacent bases, so f60[bus + 1] is the same byte as f61[bus].
 * Callers apply the result with SNDPLATFORM_setfxlevel; func_003B88C8, SNDBANKI_playtimbre and SNDPKTPLAY_start call
 * it.
 *
 * @param busIndex  the effect bus number
 * @param i  the channel index
 */
void func_003BA740(int busIndex, int i) {
    struct CHANPUB *channel = &sndgs.chan[i];
    struct sA8E8Bus *busRec = func_003B8218(busIndex, channel->rendermode);
    channel->unk_62[busIndex] = (channel->progfxlevel[busIndex] * channel->unk_61[busIndex] * busRec->f02) >> 6;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BA7B0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDdetunetolinear);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char sndcents_A888[] __asm__("sndcents");

/**
 * Converts a detune in cents to a 4.12 pitch multiplier.
 *
 * Whole octaves (1200 cents) double or halve 0x1000; the rest goes through the 256-entry table sndcents.
 *
 * @param x  the detune in cents
 * @return the multiplier (0x1000 = 1.0)
 */
int iSNDdetunetolinear(int x) {
    int scale = 0x1000;
    int tableIndex;
    while (x >= 1200) {
        x -= 1200;
        scale <<= 1;
    }
    while (x < -1199) {
        x += 1200;
        scale >>= 1;
    }
    tableIndex = (x * 13981) >> 16;
    if (tableIndex <= -0x100)
        tableIndex = -0xFF;
    if (tableIndex < 0)
        scale = (scale * (sndcents_A888[tableIndex + 0x100] + 0x100)) >> 9;
    else
        scale = (scale * (sndcents_A888[tableIndex] + 0x100)) >> 8;
    return scale;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDcalcpitch);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
extern short f_A888_A938(int) __asm__("iSNDdetunetolinear");

/**
 * Recomputes a channel's pitch from detune, bend, pitch LFO and pitch multiplier.
 *
 * When the cached linear pitch (+0x86) is 0 it rebuilds it from the detune (+0x84), the bend (+0x64, through the bend
 * table +0x74 when set) times the bend range (+0x82) and the pitch LFO (+0x7C table, depth +0x80) with iSNDdetunetolinear;
 * the final pitch (+0x8A) is that times the pitch multiplier (+0x88) >> 12.
 *
 * @param idx  the channel
 */
void iSNDcalcpitch(int idx) {
    struct CHANPUB *e = &sndgs.chan[idx];
    int v;
    if (e->detunepitch == 0) {
        int a = e->initialdetune;
        if (e->bendrange != 0) {
            int c = e->pbendtable != 0 ? e->pbendtable[e->pitchbend] : e->pitchbend;
            a += ((c - 0x40) * e->bendrange) >> 6;
        }
        if (e->ppitchlfo != 0) {
            a += ((e->ppitchlfo[e->curpitchlfoentry] - 0x40) * e->pitchlfodepth) >> 6;
        }
        e->detunepitch = f_A888_A938(a);
    }
    v = e->detunepitch;
    e->finalpitch = (v * e->progpitch) >> 12;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDpatchkey);
#ifdef SKIP_ASM
struct G_AA00 { char pad[0x17E]; short cnt; char pad2[0x70]; struct CHANPUB *tab; };

/**
 * Steps through the channels of a sound's patch-key group.
 *
 * Start with *pos = -1. A channel without a patch key (+0x26) yields only itself; otherwise every later channel with
 * the same key, busy (state 1) and holding a handle is returned in turn.
 *
 * @param chanIndex  the sound's channel
 * @param pos  in/out: the last channel returned
 * @return 1 while there is another channel, else 0
 */
int iSNDpatchkey(int chanIndex, int *pos) {
    struct CHANPUB *chanEntry = &sndgs.chan[chanIndex];
    unsigned char patchKey = chanEntry->patchkey;
    if (patchKey != 0) {
        (*pos)++;
        while (*pos < sndgs.voicestotal) {
            chanEntry = &sndgs.chan[*pos];
            if (chanEntry->patchkey == patchKey && chanEntry->status == 1 && chanEntry->handle >= 0) return 1;
            (*pos)++;
        }
    } else if (*pos < 0) {
        *pos = chanIndex;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_parsetimbre);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* SNDI_parsetimbre (EA snd library, stpparse.c): PATS tag stream into stc/pstc (0x00-0x2A) and stp/pstp (0x80-0xAD) */
struct SNDIPATCHHEADER_AAC0 {
    short masterranddetune;
    signed char platformver;
    signed char numchan;
    short detune;
    short randdetune;
    signed char velmin;
    signed char velmax;
    signed char keymin;
    signed char keymax;
    signed char priority;
    signed char basekey;
    signed char releaseenvelope;
    signed char numenvelopes;
    signed char bendrange;
    signed char pan;
    signed char randpan;
    unsigned char samplerep;
    signed char vol;
    signed char randvol;
    signed char fxlevel0;
    signed char numuserdata;
    unsigned short rendermode;
    signed char initialenvvol;
    unsigned char lfovollength;
    signed char lfovolrand;
    unsigned char lfopitchlength;
    unsigned char lfopitchrand;
    signed char panmult;
    void *pvoltable;
    void *pbendtable;
    void *penvelope;
    void *plfovol;
    void *plfopitch;
    void *puserdata[4];
    int userdatasize[4];
    short lfopitchdepth;
    unsigned short samplerate;
    int totalframes;
    int sustainstart;
    int sustainend;
    int *paudioramaddr;
    int sampleoffset[6];
    int loopoffset[6];
    void *ptimestretchdata[6];
    unsigned short timestretchsize[6];
    unsigned short azimuth[6];
    void *ptimbre;
};
struct SNDTAGINFO_AAC0 {
    unsigned char *ptag;
    int tagtype;
    int tagval;
    void *tagptr;
    unsigned int tagsize;
};
extern char snddefaultenvelope_AAC0[] __asm__("D_0044C580");
extern void memset_AAC0(void *, int, int) __asm__("memset");
extern int SNDI_gettag_AAC0(struct SNDTAGINFO_AAC0 *psti) __asm__("SNDI_gettag");

int SNDI_parsetimbre_AAC0(void **pptp, struct SNDIPATCHHEADER_AAC0 *pph) __asm__("SNDI_parsetimbre");
/**
 * Parses one timbre of a tagged patch into a patch header.
 *
 * Sets defaults for every common tag (0..42) and platform tag (128..173), then reads tags with SNDI_gettag until they
 * end or a 0xFE tag says another timbre follows. Tag 20 adds a user-data block, tags 152-155, 164 and 165 give
 * per-channel time-stretch data. The header gets key and velocity ranges, priority, base key, envelopes (default
 * D_0044C580), pan, volume, detune, LFOs, sample rate (default 22050), frames, sustain points, codec, render mode,
 * sample offsets, and per-channel azimuths from the angle table (+0x2E0) plus the patch's offsets (offsets ignored for
 * stereo when the speaker count option is 2). Advances *pptp past the parsed tags. Quirk: user-data blocks are not
 * bounded by the 4 header slots.
 *
 * @param pptp  in/out: the read position in the patch
 * @param pph  receives the patch header
 * @return 1 when another timbre follows, else 0
 */
int SNDI_parsetimbre_AAC0(void **pptp, struct SNDIPATCHHEADER_AAC0 *pph) {
    int stc[43];
    void *pstc[43];
    int stp[46];
    void *pstp[46];
    int azoffsets[6];
    struct SNDTAGINFO_AAC0 sti;
    int moretags;
    int i;

    memset_AAC0(azoffsets, 0, sizeof(azoffsets));
    sti.ptag = (unsigned char *)*pptp;
    pph->ptimbre = *pptp;
    for (i = 0; i < 43; i++) {
        pstc[i] = 0;
    }
    for (i = 0; i < 46; i++) {
        pstp[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        pph->ptimestretchdata[i] = 0;
        pph->timestretchsize[i] = 0;
    }
    stc[1] = 0;
    stc[2] = 0x7F;
    stc[3] = 0;
    stc[4] = 0x7F;
    stc[6] = 0;
    stc[7] = 0x3C;
    stc[8] = -1;
    stc[9] = 1;
    stc[10] = 0;
    stc[12] = 0x40;
    stc[13] = 0;
    stc[14] = 0x7F;
    stc[15] = 0;
    stc[16] = 0;
    stc[17] = 0;
    stc[18] = 0;
    stc[19] = 0;
    pph->numuserdata = 0;
    stc[23] = 0;
    stc[25] = (int)snddefaultenvelope_AAC0;
    stc[26] = 0;
    stc[38] = 0;
    stc[39] = 0;
    stc[40] = 0;
    stc[41] = 0;
    stc[42] = 0;
    stc[28] = 0x7F;
    stc[29] = 0;
    stc[30] = 0;
    stc[31] = 0;
    stc[32] = 0;
    stc[33] = 0;
    stc[34] = 0;
    stc[35] = 0;
    stc[36] = 0;
    stc[37] = 1;
    stp[0] = 0;
    stp[2] = 1;
    stp[4] = 22050;
    stp[5] = 0;
    stp[6] = -1;
    stp[7] = -1;
    stp[8] = 0;
    stp[0x1C] = 0;
    stp[9] = 0;
    stp[29] = 0;
    stp[20] = 0;
    stp[0x1E] = 0;
    stp[21] = 0;
    stp[0x1F] = 0;
    stp[34] = 0;
    stp[38] = 0;
    stp[35] = 0;
    stp[39] = 0;
    stp[0x20] = 5;
    stp[0xC] = 8;
    moretags = 0;
    while (SNDI_gettag_AAC0(&sti)) {
        if (sti.tagtype < 43) {
            stc[sti.tagtype] = sti.tagval;
            pstc[sti.tagtype] = sti.tagptr;
            if (sti.tagtype == 20) {
                pph->puserdata[pph->numuserdata] = sti.tagptr;
                pph->userdatasize[pph->numuserdata] = sti.tagsize;
                pph->numuserdata++;
            }
        } else if (sti.tagtype >= 128 && sti.tagtype < 174) {
            stp[sti.tagtype - 128] = sti.tagval;
            pstp[sti.tagtype - 128] = sti.tagptr;
            if (sti.tagtype == 152) {
                pph->ptimestretchdata[0] = sti.tagptr;
                pph->timestretchsize[0] = sti.tagsize;
            } else if (sti.tagtype == 153) {
                pph->ptimestretchdata[1] = sti.tagptr;
                pph->timestretchsize[1] = sti.tagsize;
            } else if (sti.tagtype == 154) {
                pph->ptimestretchdata[2] = sti.tagptr;
                pph->timestretchsize[2] = sti.tagsize;
            } else if (sti.tagtype == 155) {
                pph->ptimestretchdata[3] = sti.tagptr;
                pph->timestretchsize[3] = sti.tagsize;
            } else if (sti.tagtype == 164) {
                pph->ptimestretchdata[4] = sti.tagptr;
                pph->timestretchsize[4] = sti.tagsize;
            } else if (sti.tagtype == 165) {
                pph->ptimestretchdata[5] = sti.tagptr;
                pph->timestretchsize[5] = sti.tagsize;
            }
        } else if (sti.tagtype == 0xFE) {
            moretags = 1;
            break;
        }
    }
    pph->velmin = stc[1];
    pph->velmax = stc[2];
    pph->keymin = stc[3];
    pph->keymax = stc[4];
    pph->priority = stc[6];
    pph->basekey = stc[7];
    pph->releaseenvelope = stc[8];
    pph->numenvelopes = stc[9];
    pph->bendrange = stc[10];
    pph->pan = stc[12];
    pph->randpan = stc[13];
    pph->vol = stc[14];
    pph->randvol = stc[15];
    pph->detune = stc[16];
    pph->randdetune = stc[17];
    pph->pvoltable = (char *)pstc[18] + stc[18];
    pph->fxlevel0 = stc[19];
    pph->pbendtable = (char *)pstc[23] + stc[23];
    pph->penvelope = (char *)pstc[25] + stc[25];
    pph->loopoffset[0] = stc[26];
    pph->loopoffset[1] = stc[38];
    pph->loopoffset[2] = stc[39];
    pph->loopoffset[3] = stc[40];
    pph->loopoffset[4] = stc[41];
    pph->loopoffset[5] = stc[42];
    pph->initialenvvol = stc[28];
    pph->plfovol = (char *)pstc[29] + stc[29];
    pph->lfovollength = stc[30];
    pph->lfovolrand = stc[31];
    pph->plfopitch = (char *)pstc[32] + stc[32];
    pph->lfopitchlength = stc[33];
    pph->lfopitchdepth = stc[34];
    pph->lfopitchrand = stc[35];
    pph->masterranddetune = stc[36];
    pph->panmult = stc[37];
    pph->platformver = stp[0];
    pph->numchan = stp[2];
    pph->samplerep = stp[0x20];
    pph->samplerate = stp[4];
    pph->totalframes = stp[5];
    pph->sustainstart = stp[6];
    pph->sustainend = stp[7];
    pph->rendermode = stp[0xC];
    pph->paudioramaddr = (int *)pstp[10];
    pph->sampleoffset[0] = stp[8];
    azoffsets[0] = stp[0x1C];
    pph->sampleoffset[1] = stp[9];
    azoffsets[1] = stp[29];
    pph->sampleoffset[2] = stp[20];
    azoffsets[2] = stp[0x1E];
    pph->sampleoffset[3] = stp[21];
    azoffsets[3] = stp[0x1F];
    pph->sampleoffset[4] = stp[34];
    azoffsets[4] = stp[38];
    pph->sampleoffset[5] = stp[35];
    azoffsets[5] = stp[39];
    for (i = 0; i < pph->numchan; i++) {
        if (sndgs.sso.set.outputchannels == 2 && pph->numchan == 2) {
            azoffsets[i] = 0;
        }
        pph->azimuth[i] = sndgs.srcchancfg3d[pph->numchan - 1][i] + azoffsets[i];
    }
    *pptp = sti.ptag;
    return moretags;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_findfreekey);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern unsigned char freekey_B0C8 __asm__("freekey");
struct G_B0C8 { char pad[0x17E]; short n; char pad2[0x1F0 - 0x180]; struct CHANPUB *ents; };

/**
 * Returns a new patch key that no busy channel uses.
 *
 * Advances the 8-bit counter freekey (skipping 0) until no busy channel (+0x69) has that key (+0x26).
 *
 * @return the key (1..255)
 */
unsigned char SNDBANKI_findfreekey(void) {
    int i;
again:
    freekey_B0C8++;
    if (freekey_B0C8 == 0) freekey_B0C8++;
    for (i = 0; i < sndgs.voicestotal; i++) {
        if (sndgs.chan[i].status && sndgs.chan[i].patchkey == freekey_B0C8) goto again;
    }
    return freekey_B0C8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_playtimbre);
#ifdef SKIP_ASM
struct Smp_B140 { int w0; int w4; };
struct Ch_B140 {
    char pad0[4];
    short ids[6];
    unsigned short h10;
    unsigned short h12;
    int w14;
    int w18;
    unsigned short h1C;
    unsigned short h1E;
    unsigned short h20;
    unsigned char b22;
    unsigned char b23;
    unsigned short h24;
    char pad26[5];
    unsigned char b2B;
    char pad2C[4];
    int w30;
    char pad34[4];
    int w38;
    int w3C;
    int w40;
    int w44;
    char pad48[4];
    unsigned short h4C;
    char pad4E[2];
    unsigned short h50[6];
    unsigned char b5C;
    unsigned char b5D;
    unsigned char b5E;
    unsigned char b5F;
    unsigned char b60;
    unsigned char b61;
    char pad62[2];
    unsigned char b64;
    unsigned char b65;
    unsigned char b66;
    unsigned char b67;
    unsigned char b68;
    char pad69;
    unsigned short h6A;
    struct Smp_B140 *smp;
    int w70;
    int w74;
    int w78;
    int w7C;
    unsigned short h80;
    unsigned short h82;
    unsigned short h84;
    unsigned short h86;
    unsigned short h88;
    char pad8A[2];
};
struct G_B140 {
    char pad0[0x1F0];
    struct Ch_B140 *chans;
};
extern int f6158_B140(int *, struct SNDIPATCHHEADER *) __asm__("SNDI_validrendermode");
extern void f0C18_B140(int, int *, int *) __asm__("SNDPLATFORM_getvoicerange");
extern int fA0B0_B140(int, int, int *, int, int) __asm__("SNDVOICEI_alloc");
extern int f72A0_B140(int) __asm__("randrange");
extern int fC570_B140(int) __asm__("SNDI_pantoazimuth");
extern unsigned int fC5C8_B140(void) __asm__("iSNDrandom");
extern void fA938_B140(int) __asm__("iSNDcalcpitch");
extern void fA740_B140(int, int) __asm__("func_003BA740");
extern void fA7B0_B140(int) __asm__("func_003BA7B0");
extern int f3EE0_B140(struct SNDIPATCHHEADER *, int, int, int, int, int, int) __asm__("SNDPLATFORM_playtimbre");
extern void fA550_B140(int) __asm__("SNDVOICEI_free");

// Locals and statement order from On Tour's PSP debug build (SNDBANKI_playtimbre, stagpat.c).
/**
 * Starts one timbre of a patch on newly allocated channels.
 *
 * For each render mode the header allows (SNDI_validrendermode) it gets a channel range (SNDPLATFORM_getvoicerange) and allocates channels
 * (SNDVOICEI_alloc). The master channel gets: built-in azimuth from the pan (random pan added, SNDI_pantoazimuth), detune
 * from base key vs key (100 cents a key) plus master and random detune, volume and bend tables, the options' volume
 * (16.16, no fade), built-in volume (patch volume + random) * velocity / 127, bend range, envelope start and first
 * step, LFO tables with random start, time and pitch multipliers, bank and patch numbers, azimuth (built-in + options)
 * and elevation, frames, sustain, sample rate, codec, channel count, render mode, per-channel azimuth offsets, dry and
 * effect levels. It then computes pitch (iSNDcalcpitch), send (func_003BA740) and volume (func_003BA7B0) and starts the
 * voices with SNDPLATFORM_playtimbre; when that fails it frees the channels (SNDVOICEI_free) and tries the next render mode.
 *
 * @param bhandle  the bank handle
 * @param patnum  the patch number
 * @param psampledata  the bank's sample data
 * @param pspo  the play options
 * @param pph  the timbre's patch header
 * @param keynum  the key
 * @param velocity  the velocity
 * @param masterranddetune  a detune shared by all timbres of the note (cents)
 * @return the sound handle, or -9
 */
int SNDBANKI_playtimbre(int bhandle, int patnum, void *psampledata, struct SNDPLAYOPTS *pspo, struct SNDIPATCHHEADER *pph, int keynum, int velocity, int masterranddetune) {
    struct Ch_B140 *pv;
    int rendermodeindex;
    int rendermode;
    int minvoicerange;
    int maxvoicerange;
    int voice;
    int vol;
    int bend;
    int builtinvol;
    int fxlevel0;
    int pan;
    int handle;
    int i;
    int done;

    handle = -9;
    done = 0;
    vol = pspo->vol;
    bend = pspo->bend;
    fxlevel0 = pspo->fxlevel0;
    rendermodeindex = 0;
    while (!done) {
        rendermode = f6158_B140(&rendermodeindex, pph);
        if (!rendermode) {
            break;
        }
        f0C18_B140(rendermode, &minvoicerange, &maxvoicerange);
        voice = fA0B0_B140(pph->numchan, pph->priority, &handle, minvoicerange, maxvoicerange);
        if (voice < 0) {
            continue;
        }
        pv = &((struct Ch_B140*)sndgs.chan)[voice];
        pan = pph->pan;
        if (pph->randpan) {
            pan += f72A0_B140(pph->randpan);
            if (pan < 0) {
                pan = 0;
            } else if (pan > 127) {
                pan = 127;
            }
        }
        pv->h4C = fC570_B140(pan);
        pv->h84 = (*(unsigned short*)&pph->detune);
        pv->h84 = pv->h84 - (pph->basekey - keynum) * 100;
        if (masterranddetune) {
            pv->h84 = pv->h84 + masterranddetune;
        }
        if (pph->randdetune) {
            pv->h84 = pv->h84 + f72A0_B140(pph->randdetune);
        }
        pv->w70 = ((int)pph->pvoltable);
        pv->w74 = ((int)pph->pbendtable);
        pv->w38 = vol << 16;
        pv->w30 = 0;
        builtinvol = pph->vol;
        if (pph->randvol) {
            builtinvol += f72A0_B140(pph->randvol);
        }
        if (builtinvol > 127) {
            builtinvol = 127;
        }
        if (builtinvol < 0) {
            builtinvol = 0;
        }
        builtinvol = builtinvol * velocity / 127;
        pv->b2B = builtinvol;
        pv->h82 = pph->bendrange * 100;
        pv->b64 = bend;
        pv->smp = ((struct Smp_B140*)pph->penvelope);
        pv->w40 = pph->initialenvvol << 16;
        pv->b5D = 0;
        pv->b5C = (*(unsigned char*)&pph->numenvelopes);
        pv->b5E = (*(unsigned char*)&pph->releaseenvelope);
        pv->w44 = pv->smp->w0;
        if (pv->w44 < 0) {
            pv->w44 = 0x7FFFFFFF;
        }
        pv->w3C = ((pv->smp->w4 << 16) - pv->w40) / pv->w44;
        pv->w78 = ((int)pph->plfovol);
        pv->w7C = ((int)pph->plfopitch);
        pv->b65 = pph->lfovollength;
        pv->b66 = pph->lfopitchlength;
        pv->h80 = (*(unsigned short*)&pph->lfopitchdepth);
        if (pph->lfovolrand) {
            pv->b67 = fC5C8_B140() % pph->lfovolrand;
        } else {
            pv->b67 = 0;
        }
        if (pph->lfopitchrand) {
            pv->b68 = fC5C8_B140() % pph->lfopitchlength;
        } else {
            pv->b68 = 0;
        }
        pv->h6A = pspo->timemult;
        pv->h88 = pspo->pitchmult;
        pv->h86 = 0;
        fA938_B140(voice);
        pv->h12 = bhandle;
        pv->h10 = patnum;
        pv->h1C = pv->h4C + pspo->azimuth;
        pv->h1E = (*(unsigned short*)&pspo->elevation);
        pv->w18 = pph->sustainend;
        pv->w14 = pph->totalframes;
        pv->h20 = pph->samplerate;
        pv->b22 = pph->samplerep;
        pv->b23 = pph->numchan;
        pv->h24 = rendermode;
        for (i = 0; i < pph->numchan; i++) {
            pv->h50[i] = pph->azimuth[i];
        }
        pv->b5F = (*(unsigned char*)&pspo->drylevel);
        pv->b60 = (*(unsigned char*)&pph->fxlevel0);
        pv->b61 = fxlevel0;
        fA740_B140(0, voice);
        fA7B0_B140(voice);
        if (f3EE0_B140(pph, (int)psampledata, voice, pspo->timemult, pspo->pad2, pspo->lowpasscutoff, pspo->highpasscutoff) >= 0) {
            return handle;
        }
        for (i = 0; i < pph->numchan; i++) {
            fA550_B140(((struct Ch_B140*)sndgs.chan)[voice].ids[i]);
        }
        handle = -9;
    }
    return handle;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_playpatch);
#ifdef SKIP_ASM
struct G_B588 { char pad[0x1F0]; char *ents; };
extern int f_B0C8_B588(void) __asm__("SNDBANKI_findfreekey");
extern int f_AAC0_B588(int **, struct SNDIPATCHHEADER *) __asm__("SNDI_parsetimbre");
extern int f_72A0_B588(int) __asm__("randrange");
extern int f_B140_B588(int, int, int *, signed char *, struct SNDIPATCHHEADER *, int, int, int) __asm__("SNDBANKI_playtimbre");
extern void f_5F98_B588(struct SNDIPATCHHEADER *, int) __asm__("SNDBANKI_userdatacallback");
extern int f_8AB8_B588(int) __asm__("SNDover");
extern int f_A6E8_B588(int) __asm__("SNDVOICEI_get");
extern void f_89E0_B588(int) __asm__("SNDstop");

int SNDBANKI_playpatch_impl(int *self, int *cur, int a2, int a3, signed char *pos) __asm__("SNDBANKI_playpatch");
/**
 * Plays a patch: starts every timbre that matches the note's key and velocity.
 *
 * Fails with -8 for a null patch; null sample data (self) defaults to the patch. Takes the key (options +2) and
 * velocity (+3) and a fresh patch key (SNDBANKI_findfreekey), then parses the timbres (SNDI_parsetimbre); the first one's master
 * random detune is rolled once (randrange). Each timbre whose velocity and key ranges contain the note is started
 * with SNDBANKI_playtimbre and its user data sent with type 1 (SNDBANKI_userdatacallback); a failure stops the voices already started
 * (SNDstop). Voices that already ended (SNDover) are dropped; one remaining voice gets no patch key,
 * several share the new key with the last as master (+0x27).
 *
 * @param cur  the patch
 * @param bankHandle  the bank handle
 * @param patchNum  the patch number
 * @param pos  the play options
 * @return the last sound handle, -9 when nothing played, or -8
 */
int SNDBANKI_playpatch_impl(int *self, int *cur, int bankHandle, int patchNum, signed char *pos) {
    struct SNDIPATCHHEADER rec;
    int ids[12];
    int n;
    int startedCount;
    int i;
    int k;
    char *chanEntry;
    int ret;
    int first;
    int masterDetune;
    int x;
    int y;
    int tag;
    int more;

    n = 0;
    chanEntry = 0;
    ret = -9;
    first = 0;
    masterDetune = 0;
    if (cur == 0) {
        return -8;
    }
    if (self == 0) {
        self = cur;
    }
    cur++;
    y = pos[2];
    x = pos[3];
    tag = f_B0C8_B588();
    do {
        more = f_AAC0_B588(&cur, &rec);
        if (!first) {
            if (rec.masterranddetune) {
                masterDetune = f_72A0_B588(rec.masterranddetune);
            }
            first = 1;
        }
        if (x >= (signed char)(*(unsigned char*)&rec.velmin) && x <= (signed char)(*(unsigned char*)&rec.velmax) && y >= (signed char)(*(unsigned char*)&rec.keymin) && y <= (signed char)(*(unsigned char*)&rec.keymax)) {
            ret = f_B140_B588(bankHandle, patchNum, self, pos, &rec, y, x, masterDetune);
            if (ret < 0) {
                goto fail;
            }
            ids[n++] = ret;
            f_5F98_B588(&rec, 1);
        }
    } while (more);

    startedCount = n;
    n = 0;
    for (i = 0; i < startedCount; i++) {
        if (f_8AB8_B588(ids[i]) == 0) {
            ids[n++] = ids[i];
        }
    }
    if (n == 0) {
        goto fail;
    }
    if (n == 1) {
        k = f_A6E8_B588(ids[0]);
        if (k >= 0) {
            chanEntry = (char*)&sndgs.chan[k];
            chanEntry[0x26] = 0;
            chanEntry[0x27] = 0;
        }
    } else {
        for (i = 0; i < n; i++) {
            k = f_A6E8_B588(ids[i]);
            if (k >= 0) {
                chanEntry = (char*)&sndgs.chan[k];
                chanEntry[0x26] = tag;
                chanEntry[0x27] = 0;
            }
        }
        chanEntry[0x27] = 1;
    }
    return ret;
fail:
    for (i = 0; i < n; i++) {
        f_89E0_B588(ids[i]);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_patchtohdr);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* PT / GSTR sound header parser */
struct Out_B820 {
    char pad0[2];
    unsigned char f2;
    char pad3[2];
    unsigned char f5;
    unsigned char f6;
    unsigned char f7;
    unsigned short flags;
    char padA[2];
    unsigned short chanOff[6];
    void *data[6];
    int size[6];
    int a48[4];
    int a58[4];
};
struct Extra_B820 { int f0; int offs[6]; };
struct G_B820 {
    char pad0[0x47];
    unsigned char f47;
    char pad48[0x2E0 - 0x48];
    unsigned short tbl[8][6];
};
extern void f_6210_B820(void *, int, int) __asm__("memset");
extern int f_7218_B820(struct Out_B820 *) __asm__("SND_attrsetdef");
extern int f_7388_B820(struct SNDTAGINFO *) __asm__("SNDI_gettag");
extern void *f_5C60_B820(int) __asm__("SNDMEMI_alloc");
extern void f_51E8_B820(void *, void *, int) __asm__("Util_MemCpy");
extern void f_BDA0_B820(int, unsigned char *, struct SNDSAMPLEFORMAT *, struct Out_B820 *, struct Extra_B820 *) __asm__("SNDI_patchtohdrgen");

extern struct G_B820 sndgs_B820 __asm__("sndgs");
/**
 * Reads a patch's sample format, attributes and description.
 *
 * A "GSTR" patch goes to SNDI_patchtohdrgen and sets *isGstr. Otherwise it clears the outputs, sets attribute defaults
 * (SND_attrsetdef) and format defaults (22050 Hz, 1 channel, codec 5) and reads the tags (SNDI_gettag): codec (160),
 * channels (130), rate (132), frames (133), azimuth offsets (156-159, 166, 167), time-stretch data copied into new
 * sound memory (152-155, 164, 165; SNDMEMI_alloc, Util_MemCpy), sample-data reference (138), render mode (140,
 * default 8), per-channel sample offsets (136, 137, 148, 149, 162, 163), user data (20), platform version (0x80),
 * effect level (19), bend range (10) and priority (6). Each channel gets azimuth = angle table + offset (offsets
 * ignored for stereo with 2 speakers); sample offsets become base-relative for render modes 0x14 / 0 and relative to
 * the referenced sample address for 0x308. Used by func_003B6670 and the stream header parser SNDSTRMI_parseheader.
 *
 * @param base  the bank (0 for streams)
 * @param src  the patch tags
 * @param hdr  receives the sample format
 * @param out  receives the sample attributes (0x68 bytes)
 * @param extra  receives the sample description (frames, offsets)
 * @param isGstr  if not null, receives 1 for a GSTR patch, else 0
 */
void SNDI_patchtohdr(int base, unsigned char *src, struct SNDSAMPLEFORMAT *hdr, struct Out_B820 *out,
                   struct Extra_B820 *extra, char *isGstr) {
    struct SNDTAGINFO rec;
    int vals[6];
    int n;
    int flags;
    int *ref;
    int ch;

    f_6210_B820(vals, 0, 0x18);
    n = 0;
    flags = 8;
    f_6210_B820(hdr, 0, 4);
    ref = 0;
    f_6210_B820(out, 0, 104);
    f_6210_B820(extra, 0, 0x1C);
    if (!(src[0] == 'P' && src[1] == 'T') && src[0] == 'G' && src[1] == 'S' && src[2] == 'T' && src[3] == 'R') {
        f_BDA0_B820(0, src, hdr, out, extra);
        if (isGstr) {
            *isGstr = 1;
        }
        return;
    }
    if (isGstr) {
        *isGstr = 0;
    }
    f_7218_B820(out);
    rec.ptag = src + 4;
    hdr->samplerate = 22050;
    hdr->channels = 1;
    hdr->samplerep = 5;
    extra->f0 = 0;
    while (f_7388_B820(&rec)) {
        if (rec.tagtype == 160) {
            hdr->samplerep = rec.tagval;
        } else if (rec.tagtype == 156) {
            vals[0] = rec.tagval;
        } else if (rec.tagtype == 157) {
            vals[1] = rec.tagval;
        } else if (rec.tagtype == 158) {
            vals[2] = rec.tagval;
        } else if (rec.tagtype == 159) {
            vals[3] = rec.tagval;
        } else if (rec.tagtype == 166) {
            vals[4] = rec.tagval;
        } else if (rec.tagtype == 167) {
            vals[5] = rec.tagval;
        } else if (rec.tagtype == 152) {
            out->data[0] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[0], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[0] = ((int)rec.tagsize);
        } else if (rec.tagtype == 153) {
            out->data[1] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[1], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[1] = ((int)rec.tagsize);
        } else if (rec.tagtype == 154) {
            out->data[2] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[2], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[2] = ((int)rec.tagsize);
        } else if (rec.tagtype == 155) {
            out->data[3] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[3], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[3] = ((int)rec.tagsize);
        } else if (rec.tagtype == 164) {
            out->data[4] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[4], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[4] = ((int)rec.tagsize);
        } else if (rec.tagtype == 165) {
            out->data[5] = f_5C60_B820(((int)rec.tagsize));
            f_51E8_B820(out->data[5], (void *)((int)rec.tagptr), ((int)rec.tagsize));
            out->size[5] = ((int)rec.tagsize);
        } else if (rec.tagtype == 0x80) {
            out->f7 = rec.tagval;
        } else if (rec.tagtype == 130) {
            hdr->channels = rec.tagval;
        } else if (rec.tagtype == 132) {
            hdr->samplerate = rec.tagval;
        } else if (rec.tagtype == 133) {
            extra->f0 = rec.tagval;
        } else if (rec.tagtype == 138) {
            ref = (int *)((int)rec.tagptr);
        } else if (rec.tagtype == 19) {
            out->f5 = rec.tagval;
        } else if (rec.tagtype == 10) {
            out->f6 = rec.tagval;
        } else if (rec.tagtype == 0x06) {
            out->f2 = rec.tagval;
        } else if (rec.tagtype == 140) {
            flags = rec.tagval;
        } else if (rec.tagtype == 136) {
            extra->offs[0] = rec.tagval;
        } else if (rec.tagtype == 137) {
            extra->offs[1] = rec.tagval;
        } else if (rec.tagtype == 148) {
            extra->offs[2] = rec.tagval;
        } else if (rec.tagtype == 149) {
            extra->offs[3] = rec.tagval;
        } else if (rec.tagtype == 162) {
            extra->offs[4] = rec.tagval;
        } else if (rec.tagtype == 163) {
            extra->offs[5] = rec.tagval;
        } else if (rec.tagtype == 20) {
            out->a48[n] = ((int)rec.tagptr);
            out->a58[n] = ((int)rec.tagsize);
            n++;
        }
    }
    out->flags = flags;
    for (ch = hdr->channels - 1; ch >= 0; ch--) {
        if (sndgs_B820.f47 == 2 && hdr->channels == sndgs_B820.f47) {
            vals[ch] = 0;
        }
        out->chanOff[ch] = sndgs_B820.tbl[hdr->channels - 1][ch] + (unsigned short)vals[ch];
        if ((out->flags & 0x14) || out->flags == 0) {
            extra->offs[ch] = base + extra->offs[ch];
        }
        if ((out->flags & 0x308) && ref) {
            if (ch == 0) {
                extra->offs[0] = *ref;
            } else {
                extra->offs[ch] = *ref + (extra->offs[ch] - extra->offs[0]);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_patchtohdrgen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct O_BDA0 { char pad[0x48]; int a[4]; int b[4]; };
extern void f_6210_BDA0(void *, int, int) __asm__("memset");
extern void f_7218_BDA0(char *) __asm__("SND_attrsetdef");
extern int f_7388_BDA0(struct SNDTAGINFO *) __asm__("SNDI_gettag");
extern unsigned short D_0050ABC8_BDA0[][6] __asm__("D_0050ABC8");

/**
 * Reads a generic ("GSTR") patch's sample format, attributes and description.
 *
 * Clears the outputs, sets attribute defaults (SND_attrsetdef) and reads the tags after the 8-byte header: user data
 * (tag 20) into the attributes; platform tags (128..173) give channels (default 1), codec (default 10), rate (default
 * 48000) and frames. Render mode is 4, platform version 3, and the azimuths come from the angle table D_0050ABC8.
 * Quirk: the common-tag values and the platform-tag pointers are collected but never used.
 *
 * @param unused  unused (SNDI_patchtohdr passes 0)
 * @param src  the patch
 * @param hdr  receives the sample format
 * @param out  receives the sample attributes
 * @param extra  receives the sample description (frames first)
 */
void SNDI_patchtohdrgen(int unused, int src, unsigned char *hdr, char *out, int *extra) {
    int commonVals[43];
    int commonPtrs[43];
    int platformVals[46];
    int platformPtrs[46];
    struct SNDTAGINFO rec;
    int i;
    int k;

    k = 0;
    f_6210_BDA0(hdr, 0, 4);
    f_6210_BDA0(out, 0, 104);
    f_6210_BDA0(extra, 0, 0x1C);
    f_7218_BDA0(out);
    (*(int*)&rec.ptag) = src + 8;
    for (i = 42; i >= 0; i--) {
        commonPtrs[i] = 0;
    }
    for (i = 45; i >= 0; i--) {
        platformPtrs[i] = 0;
    }
    platformVals[2] = 1;
    platformVals[4] = 48000;
    platformVals[0x20] = 10;
    platformVals[5] = 0;
    while (f_7388_BDA0(&rec) != 0) {
        if (rec.tagtype < 43) {
            commonVals[rec.tagtype] = rec.tagval;
            commonPtrs[rec.tagtype] = ((int)rec.tagptr);
            if (rec.tagtype == 20) {
                ((struct O_BDA0 *)out)->a[k] = ((int)rec.tagptr);
                ((struct O_BDA0 *)out)->b[k] = ((int)rec.tagsize);
                k++;
            }
        } else if ((unsigned int)(rec.tagtype - 0x80) >= 46) {
            if (rec.tagtype != 0xFE) {
                continue;
            }
            break;
        } else {
            platformVals[rec.tagtype - 0x80] = rec.tagval;
            platformPtrs[rec.tagtype - 0x80] = ((int)rec.tagptr);
        }
    }
    hdr[2] = platformVals[2];
    hdr[3] = platformVals[0x20];
    *(unsigned short *)hdr = platformVals[4];
    *extra = platformVals[5];
    *(short *)(out + 8) = 4;
    out[7] = 3;
    for (i = 0; i < hdr[2]; i++) {
        ((unsigned short *)(out + 0xC))[i] = D_0050ABC8_BDA0[hdr[2] - 1][i];
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_equalpower);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern float SNDI_sin_C000(float) __asm__("SNDI_sin");
extern float SNDI_cos_C000(float) __asm__("SNDI_cos");

/**
 * Computes an equal-power gain pair for an angle between two speaker angles.
 *
 * Angles are in 1/65535 of a turn. t = (angle - fromAngle) / (toAngle - fromAngle) * pi / 2; out[0] = sine(t)
 * (SNDI_sin), out[1] = cosine(t) (SNDI_cos). Called three times by func_003BC0B0.
 *
 * @param out  receives the two gains
 * @param toAngle  the second speaker's angle
 * @param fromAngle  the first speaker's angle
 * @param angle  the source angle
 */
void SNDI_equalpower(float *out, int toAngle, int fromAngle, int angle) {
    float fa = (float)toAngle / 65535.0f * 2.0f * 3.1415927f;
    float fb = (float)fromAngle / 65535.0f * 2.0f * 3.1415927f;
    float fc = (float)angle / 65535.0f * 2.0f * 3.1415927f;
    float blendAngle = (fc - fb) / (fa - fb) * 1.5707964f;
    out[0] = SNDI_sin_C000(blendAngle);
    out[1] = SNDI_cos_C000(blendAngle);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003BC0B0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_aztospkrvol);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/* sndgs: the sound driver's state block (layout worked out from every use in this unit; field names are
   their offsets). Some fields are also reached through their own splat symbols (declared below), which the
   functions that address them directly must keep using. */
extern unsigned char* sndaztospkr[];
/**
 * Looks up the per-speaker gains for an azimuth.
 *
 * out[i] = speaker table i (sndaztospkr) at the azimuth's high byte / 255, for each configured speaker (options speaker
 * count). Called by func_003C38E0.
 *
 * @param azimuth  the azimuth (16 bits a turn)
 * @param out  receives one gain per speaker
 */
void SNDI_aztospkrvol(int azimuth, float* out)
{
    int idx = (azimuth & 0xFFFF) >> 8;
    int i;
    for (i = 0; i < sndgs.sso.set.outputchannels; i++) {
        out[i] = (float)sndaztospkr[i][idx] / 255.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_freespkrtable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SNDMEMI_free_C530(int) __asm__("SNDMEMI_free");
extern int sndaztospkr_buf_C530[] __asm__("sndaztospkr_buf");
extern char sndaztospkr_C530[] __asm__("sndaztospkr");

/**
 * Frees the speaker gain tables (sndaztospkr_buf) inside the critical section.
 *
 * Likely bug: the guard tests the address of sndaztospkr instead of its contents, so it is always true and the free
 * (SNDMEMI_free) runs even when no table was allocated (the free ignores unknown addresses). Called by the shutdown
 * SNDSYS_restore.
 */
void SNDI_freespkrtable(void) {
    SNDSYS_entercritical();
    if (sndaztospkr_C530 != 0) {
        SNDMEMI_free_C530(sndaztospkr_buf_C530[0]);
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_pantoazimuth);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short sndpantoazimuthtab_C570[128] __asm__("sndpantoazimuthtab");

/**
 * Converts a MIDI pan (0..127) to an azimuth through the table sndpantoazimuthtab.
 *
 * @param pan  the pan
 * @return the azimuth
 */
unsigned short SNDI_pantoazimuth(int pan) {
    return sndpantoazimuthtab_C570[pan];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_randomseed);
#ifdef SKIP_ASM
extern int SNDIrandseedorig[];
extern int SNDIrandseed[];

/**
 * Seeds the sound system's random generator.
 *
 * Each of the six state words (SNDIrandseed) becomes its constant (SNDIrandseedorig) plus the seed. The init SNDSYSI_init
 * passes the options' seed field.
 *
 * @param seed  the seed
 */
void SNDI_randomseed(int seed) {
    int i;
    for (i = 0; i < 6; i++) {
        SNDIrandseed[i] = SNDIrandseedorig[i] + seed;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDrandom);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int SNDIrandseed_C5C8[6] __asm__("SNDIrandseed");

/**
 * Returns the next number from the sound system's random generator.
 *
 * Adds the six state words (SNDIrandseed) into each other from the last towards the first with carries, then increments
 * the last word, carrying into the others when it wraps. The randrange-style caller at randrange scales its low 15
 * bits into a signed range.
 *
 * @return the new first state word
 */
unsigned int iSNDrandom(void) {
    unsigned int runningSum, carryBit, n;

    runningSum = SNDIrandseed_C5C8[5] + SNDIrandseed_C5C8[4];
    carryBit = 0;
    if (runningSum < SNDIrandseed_C5C8[5] || runningSum < SNDIrandseed_C5C8[4]) {
        carryBit = 1;
    }
    SNDIrandseed_C5C8[4] = runningSum;
    runningSum = runningSum + SNDIrandseed_C5C8[3] + carryBit;
    carryBit = runningSum < SNDIrandseed_C5C8[3];
    SNDIrandseed_C5C8[3] = runningSum;
    runningSum = runningSum + SNDIrandseed_C5C8[2] + carryBit;
    carryBit = runningSum < SNDIrandseed_C5C8[2];
    SNDIrandseed_C5C8[2] = runningSum;
    n = ++SNDIrandseed_C5C8[5];
    runningSum = runningSum + SNDIrandseed_C5C8[1] + carryBit;
    carryBit = runningSum < SNDIrandseed_C5C8[1];
    SNDIrandseed_C5C8[1] = runningSum;
    runningSum = runningSum + SNDIrandseed_C5C8[0] + carryBit;
    SNDIrandseed_C5C8[0] = runningSum;
    if (n == 0) {
        if (++SNDIrandseed_C5C8[4] == 0) {
            if (++SNDIrandseed_C5C8[3] == 0) {
                if (++SNDIrandseed_C5C8[2] == 0) {
                    if (++SNDIrandseed_C5C8[1] == 0) {
                        runningSum = ++SNDIrandseed_C5C8[0];
                    }
                }
            }
        }
    }
    return runningSum;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDpitchbend);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int f_A6E8_C6B8() __asm__("SNDVOICEI_get");
extern void f_A938_C6B8(int) __asm__("iSNDcalcpitch");
extern int f_AA00_C6B8(int, int *) __asm__("iSNDpatchkey");
extern void f_25E0_C6B8(int) __asm__("SNDPLATFORM_setpitch");
struct G_C6B8 { char pad[0x1F0]; char *tab; };

/**
 * Sets a sound's pitch bend and recomputes its pitch.
 *
 * For each grouped channel with a bend range (+0x82): stores the bend (+0x64), clears the cached pitch (+0x86) and
 * recomputes it (iSNDcalcpitch, SNDPLATFORM_setpitch). Quirk: a channel already at that bend, or one without a bend range,
 * makes it return 0 at once, skipping the rest of the group.
 *
 * @param soundHandle  the sound handle
 * @param bend  the bend (0x40 = centre)
 * @return the channel index, -8 for a dead handle, or 0
 */
int SNDpitchbend(int soundHandle, int bend) {
    int setChan;
    int chanIndex = f_A6E8_C6B8();
    char *channel;
    if (chanIndex >= 0) {
        setChan = -1;
        while (f_AA00_C6B8(chanIndex, &setChan) != 0) {
            channel = (char*)&sndgs.chan[setChan];
            if (*(char *)(channel + 0x64) == bend)
                return 0;
            if (*(short *)(channel + 0x82) != 0) {
                *(short *)(channel + 0x86) = 0;
                *(char *)(channel + 0x64) = bend;
                f_A938_C6B8(setChan);
                f_25E0_C6B8(setChan);
            } else
                return 0;
        }
    }
    return chanIndex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_sin);
#ifdef SKIP_ASM
/**
 * Returns the sine of an angle in radians from a 13th-order Taylor series.
 *
 * Only angles above 2 pi are wrapped; negative ones go into the series unchanged, and accuracy falls off far from 0.
 * SNDI_equalpower uses it for equal-power gains (angles 0..pi/2).
 *
 * @param x  the angle
 * @return the sine
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
float SNDI_sin(float x) {
    float x2, x3, x5, x7, x9, x11, x13;

    while (x > 6.28318548f) {
        x -= 6.28318548f;
    }
    x2 = x * x;
    x3 = x2 * x;
    x5 = x2 * x3;
    x7 = x2 * x5;
    x9 = x2 * x7;
    x11 = x2 * x9;
    x13 = x2 * x11;
    return x - x3 * 0.166666672f + x5 * 0.00833333004f - x7 * 0.000198412701f + x9 * 2.75573188e-06f - x11 * 2.50521079e-08f + x13 * 1.6059e-10f;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_cos);
#ifdef SKIP_ASM
/**
 * Returns the cosine of an angle in radians from a 12th-order Taylor series.
 *
 * Only angles above 2 pi are wrapped; negative ones go into the series unchanged. SNDI_equalpower uses it for equal-power
 * gains.
 *
 * @param x  the angle
 * @return the cosine
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
float SNDI_cos(float x) {
    float x2, x4, x6, x8, x10, x12;

    while (x > 6.28318548f) {
        x -= 6.28318548f;
    }
    x2 = x * x;
    x4 = x2 * x2;
    x6 = x2 * x4;
    x8 = x4 * x4;
    x10 = x2 * x8;
    x12 = x6 * x6;
    return 1.0f - x2 * 0.5f + x4 * 0.0416666679f - x6 * 0.00138888892f + x8 * 2.48015876e-05f - x10 * 2.755732e-07f + x12 * 2.08767581e-09f;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDREAL_exithandler);
#ifdef SKIP_ASM
extern void System_Restore();

/**
 * Shuts the sound system down at program exit (calls System_Restore).
 *
 * System_VectorToReal5 registers it with REAL_addexit in the exit-handler table that REAL_restore runs; System_Restore calls
 * the sound shutdown SNDSYS_restore.
 */
void SNDREAL_exithandler(void) {
    System_Restore();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC930);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003BECA0_C930(void) __asm__("func_003BECA0");
extern void func_003BED30_C930(void) __asm__("func_003BED30");
extern void func_003BF0F0_C930(void) __asm__("func_003BF0F0");
extern void *D_0050C840[];
extern void *D_0050C844[];
extern void *D_0050C848[];

/**
 * Installs the output effect hooks: func_003BECA0, func_003BED30 and func_003BF0F0.
 *
 * They go to D_0050C840 (asked for buffer sizes), D_0050C844 (set up with the allocated buffer) and D_0050C848 (run on
 * the output), which the driver uses when option bit 2 of +0x49 is set. cPS2SoundManager_SetMode calls it for mode 3.
 */
void func_003BC930(void) {
    D_0050C840[0] = func_003BECA0_C930;
    D_0050C844[0] = func_003BED30_C930;
    D_0050C848[0] = func_003BF0F0_C930;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BC968);
#ifdef SKIP_ASM
/**
 * Copies quadwords from main memory to the scratchpad with DMA channel 9 (to SPR).
 *
 * Waits for the channel to be idle, sets MADR (0x1000D410), SADR (0x1000D480) and QWC (0x1000D420) and starts it (CHCR
 * 0x1000D400 = 0x100); flag 0x10000 waits for the transfer to finish. Used by func_003BD220.
 *
 * @param memAddr  the source in main memory
 * @param sprAddr  the destination in the scratchpad
 * @param qwordCount  the number of quadwords
 * @param flags  0x10000 to wait for completion
 */
void func_003BC968(int memAddr, int sprAddr, int qwordCount, int flags) {
    if (*(volatile int *)0x1000D400 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
    *(volatile int *)0x1000D410 = memAddr;
    *(volatile int *)0x1000D480 = sprAddr;
    *(volatile int *)0x1000D420 = qwordCount;
    *(volatile int *)0x1000D400 = 0x100;
    if ((flags & 0x10000) && (*(volatile int *)0x1000D400 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BCA28);
#ifdef SKIP_ASM
/**
 * Copies quadwords from the scratchpad to main memory with DMA channel 8 (from SPR).
 *
 * Waits for the channel to be idle, sets MADR (0x1000D010), SADR (0x1000D080) and QWC (0x1000D020) and starts it (CHCR
 * 0x1000D000 = 0x100); flag 0x10000 waits for the transfer to finish. Used by func_003BD220.
 *
 * @param sprAddr  the source in the scratchpad
 * @param memAddr  the destination in main memory
 * @param qwordCount  the number of quadwords
 * @param flags  0x10000 to wait for completion
 */
void func_003BCA28(int sprAddr, int memAddr, int qwordCount, int flags) {
    if (*(volatile int *)0x1000D000 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
    *(volatile int *)0x1000D010 = memAddr;
    *(volatile int *)0x1000D080 = sprAddr;
    *(volatile int *)0x1000D020 = qwordCount;
    *(volatile int *)0x1000D000 = 0x100;
    if ((flags & 0x10000) && (*(volatile int *)0x1000D000 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BCAE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char p0[6]; unsigned short pos; unsigned char *buf; } BW_CAE8;
extern BW_CAE8 *D_0050AC68_CAE8[] __asm__("D_0050AC68");

/**
 * Appends the low @p n bits of a value, most significant first, to the DTS encoder's output frame.
 *
 * Writes at the bit position +0x06 of the encoder state D_0050AC68 into its output buffer (+0x08), byte index XOR 1
 * (the frame is stored as byte-swapped 16-bit words, as IEC 61937 carries it); each byte is cleared when first written,
 * and the position advances by @p n. Used by the frame-header writer func_003BCBA0.
 *
 * @param value  the value
 * @param n  the number of bits (1-32)
 */
void func_003BCAE8(int value, int n) {
    unsigned int fieldBits = value << (0x20 - n);
    int byte;
    int room;
    int take;
    int rem;
    fieldBits = fieldBits >> (0x20 - n);
    while (n > 0) {
        byte = D_0050AC68_CAE8[0]->pos >> 3;
        if (byte & 1)
            byte = byte - 1;
        else
            byte = byte + 1;
        room = 8 - (D_0050AC68_CAE8[0]->pos & 7);
        if (room == 8)
            D_0050AC68_CAE8[0]->buf[byte] = 0;
        take = (room >= n) ? n : room;
        rem = n - take;
        D_0050AC68_CAE8[0]->buf[byte] |= (fieldBits >> rem) << (room - take);
        n = rem;
        D_0050AC68_CAE8[0]->pos += take;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BCBA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct sBitWriter_CBA0 {
    unsigned char unk0;
    unsigned char unk1;
    unsigned char pad2[4];
    unsigned short unk6;
    unsigned char *unk8;
} sBitWriter;

extern sBitWriter *D_0050AC68_CBA0 __asm__("D_0050AC68");
void func_003BCAE8(int value, int nbits);

/**
 * Writes the fixed header of a DTS frame into the encoder's output buffer.
 *
 * Resets the bit position (+0x06 of D_0050AC68) and emits with func_003BCAE8: the IEC 61937 burst preamble (F872 4E1F,
 * data type 0x000B, length 0x3EE8 bits), the DTS sync word 7FFE8001, the frame header (normal frame, 16 blocks of 32
 * samples, 2013-byte frame), channel arrangement 8 (2/2, four channels) when mode +0x00 is 1 or 9 (3/2) otherwise, 48
 * kHz at 1536 kbit/s, the LFE / version field with the channel count (+0x01) minus 1, then per channel the fixed side
 * information (subband counts, code books, quantizer selections, zeroed scale fields) and the per-band bit-allocation
 * codes of the mode. Finally pokes the frame's trailing pad bytes (+0xE8, +0x7CA/+0x7CB for mode 1; +0x7D4, +0x7D5,
 * +0x7D7 otherwise). func_003BF0F0 calls it whenever the output buffer changes.
 */
void func_003BCBA0(void) {
    int i;
    int j;

    D_0050AC68_CBA0->unk6 = 0;
    func_003BCAE8(0xF8724E1F, 0x20);
    func_003BCAE8(0xB3EE8, 0x20);
    func_003BCAE8(0x7FFE8001, 0x20);
    func_003BCAE8(0x0FC3C7DC, 0x1C);
    if (D_0050AC68_CBA0->unk0 == 1) {
        func_003BCAE8(8, 6);
    } else {
        func_003BCAE8(9, 6);
    }
    func_003BCAE8(0x6E000, 19);
    func_003BCAE8((D_0050AC68_CBA0->unk1 - 1) | 0x021C0000, 26);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(22, 5);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(23, 5);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(0, 3);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(3, 2);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(6, 3);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(6, 3);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(1, 1);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(3, 2);
    }
    for (j = 6; j < 11; j++) {
        for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(7, 3);
    }
    func_003BCAE8(8, 5);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) func_003BCAE8(0, 0x18);
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) {
        j = 0;
        switch (D_0050AC68_CBA0->unk0) {
        case 1:
            for (; j < 14; j++) func_003BCAE8(13, 5);
            for (; j < 24; j++) func_003BCAE8(11, 5);
            break;
        case 2:
        default:
            for (; j < 5; j++) func_003BCAE8(13, 5);
            for (; j < 14; j++) func_003BCAE8(11, 5);
            for (; j < 24; j++) func_003BCAE8(8, 5);
            break;
        }
    }
    for (i = 0; i < D_0050AC68_CBA0->unk1; i++) {
        func_003BCAE8(0, 0x18);
        func_003BCAE8(0, 0x18);
    }
    if (D_0050AC68_CBA0->unk0 == 1) {
        D_0050AC68_CBA0->unk8[0xE8] = 0;
        D_0050AC68_CBA0->unk8[0x7CB] = 0xFF;
        D_0050AC68_CBA0->unk8[0x7CA] = 0xFF;
    } else {
        D_0050AC68_CBA0->unk8[0x7D5] |= 7;
        D_0050AC68_CBA0->unk8[0x7D4] = 0xFF;
        D_0050AC68_CBA0->unk8[0x7D7] = 0xF8;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BD078);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C06B0_D078(int a0, float *buf, void *p, signed char *tbl) __asm__("func_003C06B0");
extern signed char D_0044DD00_D078[] __asm__("D_0044DD00");
extern int D_0044C760_D078[] __asm__("D_0044C760");
extern signed char D_0044DD38_D078[] __asm__("D_0044DD38");
extern void *D_0050AC68;

/**
 * Encodes a frame of the LFE channel: 8 decimated samples as 8-bit codes plus their scale-factor index.
 *
 * func_003C06B0 filters and decimates the input into 8 floats (filter state at +0x6C of the encoder state D_0050AC68,
 * coefficients D_0044DD00). The peak magnitude picks the scale index: through the small table D_0044DD38 below 4,
 * otherwise from a log2 estimate taken from the float's bits, corrected against the scale-factor table D_0044C760. Each
 * sample is then scaled by 28.57 / D_0044C760[index] and truncated to a signed byte. Called by func_003BE6E0.
 *
 * @param lfeInput  the LFE input samples
 * @param out  receives the scale-factor index
 * @param out8  receives the 8 quantized samples
 */
void func_003BD078(int lfeInput, signed char *out, signed char *out8) {
    float buf[8];
    float max;
    float min;
    float scale;
    int i;
    int n;
    int idx;

    max = 0.0f;
    min = 0.0f;
    func_003C06B0_D078(lfeInput, buf, (char *)D_0050AC68 + 0x6C, D_0044DD00_D078);
    for (i = 0; i < 8; i++) {
        if (max < buf[i]) {
            max = buf[i];
        } else if (buf[i] < min) {
            min = buf[i];
        }
    }
    min = -min;
    if (max < min) {
        max = min;
    }
    if (max < 4.0f) {
        idx = D_0044DD38_D078[(int)max];
    } else {
        n = (int)((float)*(int *)&max * 6.524646209982166e-07f) - 696;
        idx = n + 1;
        if (max < (float)D_0044C760_D078[n]) {
            idx = n;
        }
    }
    *out = idx;
    scale = 1.0f / (float)D_0044C760_D078[idx] * 28.571428298950195f;
    for (i = 0; i < 8; i++) {
        out8[i] = (int)(buf[i] * scale);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BD220);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AC68_D220 / D_0050AC6C_D220: the ADPCM encoder's state and its work area (func_003BF0F0 sets them up). Layout from
   every use in this unit; field names are their offsets. */
struct sAC68Blk {                   /* one per channel */
    int f00;                        /* input */
    unsigned char f04[24];          /* scale factor per band */
};
struct sAC68 {
    unsigned char f00;              /* mode */
    unsigned char f01;              /* number of channels */
    char pad02[0x4 - 0x2];
    unsigned short f04;             /* frame size: 0xE0, 0x160 or 0x1E0 */
    unsigned short f06;             /* output bit position */
    unsigned char *f08;             /* output */
    float f0C[24];                  /* band weights */
    int f6C[6];
    struct sAC68Blk *f84[6];
};
struct sAC6C {
    float f000[24][16];             /* per band */
    char f600[0x200];
    char f800[0x200];
    unsigned short fA00[16];
    float *fA20;
    char *fA24;
    char *fA28;
    char *fA2C;
    char *fA30;
    char *fA34;
    char *fA38;
    unsigned char fA3C;             /* the work area is in scratchpad memory */
};
extern struct sAC68 *D_0050AC68_D220 __asm__("D_0050AC68");
extern struct sAC6C *D_0050AC6C_D220 __asm__("D_0050AC6C");
extern void func_003BC968(int, int, int, int);
extern void func_003BCA28(int, int, int, int);
extern void fF3E0_D220(int, int, int) __asm__("func_003BF3E0");
extern void fF440_D220(int, int, int) __asm__("func_003BF440");
extern void fF4C0_D220(int, int, void *) __asm__("func_003BF4C0");
extern void fF5A0_D220(int, int, void *) __asm__("func_003BF5A0");
extern void fF6B0_D220(int, int, void *) __asm__("func_003BF6B0");
extern void fF800_D220(void *) __asm__("func_003BF800");
extern void fF8D0_D220(void *, void *) __asm__("func_003BF8D0");
extern void fF960_D220(void *, void *) __asm__("func_003BF960");
extern void fFAC0_D220(void *, void *) __asm__("func_003BFAC0");
extern void fFB90_D220(void *, void *) __asm__("func_003BFB90");
extern void fFC50_D220(void *, int) __asm__("func_003BFC50");
extern void fFD60_D220(void *, int, void *) __asm__("func_003BFD60");
extern void fFE80_D220(void *, int, void *) __asm__("func_003BFE80");
extern void fFFA0_D220(void *, int, void *) __asm__("func_003BFFA0");
extern void f01A0_D220(void *, int, int, int) __asm__("func_003C01A0");
/**
 * Runs one channel's 512 input samples through the DTS encoder's subband analysis filterbank.
 *
 * When the work area D_0050AC6C is in the scratchpad (+0xA3C) and neither @p pcmInput nor @p history is, the filter history (+0x04
 * words of the encoder state D_0050AC68) is DMA'd in with func_003BC968 and all 512 16-bit samples are converted to
 * floats behind it at once (func_003BF440); otherwise the history is copied with func_003BF3E0 and each 128-sample
 * block is converted on its own and the history shifted afterwards. For each of the 4 blocks the window stage
 * (func_003BF4C0, func_003BF5A0 or func_003BF6B0 for a filter length of 0xE0, 0x160 or 0x1E0) and the transform chain
 * func_003BF800 .. func_003C01A0 produce 4 samples of every band at @p bandSamples (advanced 0x10 bytes per block). The history
 * goes back with func_003BCA28 or func_003BF3E0.
 *
 * @param pcmInput  the channel's 16-bit PCM input (16-byte aligned)
 * @param bandSamples  the per-band sample array the results go to (the work area)
 * @param history  the channel's filter history buffer
 */
void func_003BD220(unsigned int pcmInput, int bandSamples, unsigned int history) {
    short useSprDma;
    int i;
    if (!(D_0050AC6C_D220->fA3C == 0)) {
        useSprDma = 0;
        if ((pcmInput & 0x70000000) != 0x70000000) {
            useSprDma = (history & 0x70000000) != 0x70000000;
        }
    } else {
        useSprDma = 0;
    }
    if (useSprDma) {
        func_003BC968(history, (int)D_0050AC6C_D220->fA20, D_0050AC68_D220->f04 >> 2, 0);
        fF440_D220(pcmInput, (int)D_0050AC6C_D220->fA20 + D_0050AC68_D220->f04 * 4, 0x200);
        if (*(volatile int *)0x1000D400 & 0x100) {
            do {
            } while (*(volatile int *)0x1000D400 & 0x100);
        }
    } else {
        fF3E0_D220(history, (int)D_0050AC6C_D220->fA20, D_0050AC68_D220->f04);
    }
    i = 0;
    do {
        if (useSprDma) {
            if (D_0050AC68_D220->f04 == 0xE0) {
                fF4C0_D220((int)D_0050AC6C_D220->fA20 + i * 0x200, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            } else if (D_0050AC68_D220->f04 == 352) {
                fF5A0_D220((int)D_0050AC6C_D220->fA20 + i * 0x200, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            } else {
                fF6B0_D220((int)D_0050AC6C_D220->fA20 + i * 0x200, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            }
        } else {
            fF440_D220(pcmInput, (int)D_0050AC6C_D220->fA20 + D_0050AC68_D220->f04 * 4, 0x80);
            if (D_0050AC68_D220->f04 == 0xE0) {
                fF4C0_D220((int)D_0050AC6C_D220->fA20, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            } else if (D_0050AC68_D220->f04 == 352) {
                fF5A0_D220((int)D_0050AC6C_D220->fA20, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            } else {
                fF6B0_D220((int)D_0050AC6C_D220->fA20, (int)D_0050AC6C_D220->fA24, D_0050AC6C_D220->f600);
            }
            fF3E0_D220((int)D_0050AC6C_D220->fA20 + 0x200, (int)D_0050AC6C_D220->fA20, D_0050AC68_D220->f04);
        }
        i++;
        pcmInput += 0x100;
        fF800_D220(D_0050AC6C_D220->f600);
        fF8D0_D220(D_0050AC6C_D220->f600, D_0050AC6C_D220->f800);
        fF960_D220(D_0050AC6C_D220->f800, D_0050AC6C_D220->f600);
        fFAC0_D220(D_0050AC6C_D220->f600, D_0050AC6C_D220->f800);
        fFB90_D220(D_0050AC6C_D220->f800, D_0050AC6C_D220->f600);
        fFC50_D220(D_0050AC6C_D220->f600, (int)D_0050AC6C_D220->fA28);
        fFD60_D220(D_0050AC6C_D220->f600, (int)D_0050AC6C_D220->fA2C, D_0050AC6C_D220->f800);
        fFE80_D220(D_0050AC6C_D220->f800, (int)D_0050AC6C_D220->fA30, D_0050AC6C_D220->f600);
        fFFA0_D220(D_0050AC6C_D220->f600, (int)D_0050AC6C_D220->fA34, D_0050AC6C_D220->f800);
        f01A0_D220(D_0050AC6C_D220->f800, (int)D_0050AC6C_D220->fA38, bandSamples, 0x40);
        bandSamples += 0x10;
    } while (i < 4);
    if (useSprDma) {
        func_003BCA28((int)D_0050AC6C_D220->fA20 + 0x800, history, D_0050AC68_D220->f04 >> 2, 0x10000);
    } else {
        fF3E0_D220((int)D_0050AC6C_D220->fA20, history, D_0050AC68_D220->f04);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BD5B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Blk_D5B8 {
    int *data;
    unsigned char sf[24];
};
struct S_D5B8 {
    unsigned char mode;
    unsigned char n;
    char pad2[6];
    unsigned char *out;
    float fC[24];
    int a6C[6];
    struct Blk_D5B8 *ptrs[5];
};
struct C_D5B8 {
    char pad0[0xA00];
    unsigned short q[16];
    char padA20[0xA20 - 0xA20];
    float *pA20;
};
extern struct S_D5B8 *D_0050AC68_D5B8 __asm__("D_0050AC68");
extern struct C_D5B8 *D_0050AC6C_D5B8 __asm__("D_0050AC6C");
extern int D_0044C760_D5B8[] __asm__("D_0044C760");
extern signed char D_0044DD38_D5B8[] __asm__("D_0044DD38");
extern void f03E0_D5B8(struct C_D5B8 *, float *) __asm__("func_003C03E0");
extern void f0564_D5B8(void *, void *, float) __asm__("func_003C0564");
extern void f05E0_D5B8(void *, void *, float) __asm__("func_003C05E0");

/**
 * Quantizes and packs one channel's subband samples for a DTS 4.0 frame (mode 1).
 *
 * func_003C03E0 finds each band's peak into +0xA20 of the work area D_0050AC6C; peak times the band weight (+0x0C of
 * D_0050AC68) gives the band's scale-factor index (D_0044DD38 below 4, else the log2 estimate checked against
 * D_0044C760), stored in the channel block's +0x04 array. Bands 0-13 are quantized to 10-bit codes (func_003C0564) and
 * bands 14-23 to 8-bit codes (func_003C05E0), each scaled by 1 / D_0044C760[index], and written byte-swapped into the
 * output frame at ch * 220 + 234, the second 8 samples of each band 0x370 bytes further on.
 *
 * @param ch  the channel index
 */
void func_003BD5B8(int ch) {
    struct Blk_D5B8 *blk;
    unsigned char *sf;
    unsigned char *out;
    unsigned short *quantized;
    unsigned char *codeBytes;
    float x;
    int i;
    int n;
    int idx;

    blk = D_0050AC68_D5B8->ptrs[ch];
    f03E0_D5B8(D_0050AC6C_D5B8, D_0050AC6C_D5B8->pA20);
    sf = blk->sf;
    for (i = 0; i < 24; i++) {
        x = D_0050AC6C_D5B8->pA20[i] * D_0050AC68_D5B8->fC[i];
        if (x < 4.0f) {
            idx = D_0044DD38_D5B8[(int)x];
        } else {
            n = (int)((float)*(int *)&x * 6.5246462099821656e-07f) - 696;
            idx = n + 1;
            if (x < (float)D_0044C760_D5B8[n]) {
                idx = n;
            }
        }
        blk->sf[i] = idx;
    }
    out = D_0050AC68_D5B8->out + (ch * 220 + 234);
    quantized = D_0050AC6C_D5B8->q;
    for (i = 0; i < 14; i++) {
        f0564_D5B8((char *)D_0050AC6C_D5B8 + i * 0x40, quantized, 1.0f / (float)D_0044C760_D5B8[sf[i]]);
        out[1] = quantized[0] >> 2;
        out[0] = (quantized[0] << 6) | (quantized[1] >> 4);
        out[3] = (quantized[1] << 4) | (quantized[2] >> 6);
        out[2] = (quantized[2] << 2) | (quantized[3] >> 8);
        out[5] = quantized[3];
        out[4] = quantized[4] >> 2;
        out[7] = (quantized[4] << 6) | (quantized[5] >> 4);
        out[6] = (quantized[5] << 4) | (quantized[6] >> 6);
        out[9] = (quantized[6] << 2) | (quantized[7] >> 8);
        out[8] = quantized[7];
        out[0x371] = quantized[8] >> 2;
        out[0x370] = (quantized[8] << 6) | (quantized[9] >> 4);
        out[0x373] = (quantized[9] << 4) | (quantized[10] >> 6);
        out[0x372] = (quantized[10] << 2) | (quantized[11] >> 8);
        out[0x375] = quantized[11];
        out[0x374] = quantized[12] >> 2;
        out[0x377] = (quantized[12] << 6) | (quantized[13] >> 4);
        out[0x376] = (quantized[13] << 4) | (quantized[14] >> 6);
        out[0x379] = (quantized[14] << 2) | (quantized[15] >> 8);
        out[0x378] = quantized[15];
        out += 10;
    }
    for (; i < 24; i++) {
        f05E0_D5B8((char *)D_0050AC6C_D5B8 + i * 0x40, quantized, 1.0f / (float)D_0044C760_D5B8[sf[i]]);
        codeBytes = (unsigned char *)quantized;
        out[1] = codeBytes[0];
        out[0] = codeBytes[1];
        out[3] = codeBytes[2];
        out[2] = codeBytes[3];
        out[5] = codeBytes[4];
        out[4] = codeBytes[5];
        out[7] = codeBytes[6];
        out[6] = codeBytes[7];
        out[0x371] = codeBytes[8];
        out[0x370] = codeBytes[9];
        out[0x373] = codeBytes[10];
        out[0x372] = codeBytes[11];
        out[0x375] = codeBytes[12];
        out[0x374] = codeBytes[13];
        out[0x377] = codeBytes[14];
        out[0x376] = codeBytes[15];
        out += 8;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BD9C0);
#ifdef SKIP_ASM
extern void *D_0050AC68;

/**
 * Packs every channel's 24 scale-factor indices (7 bits each) into a DTS 4.0 frame.
 *
 * Starting at +0x8C of the output frame (+0x08 of the encoder state D_0050AC68), two channels per pass (42 bytes, as
 * byte-swapped 16-bit words), for as many channels as +0x01 says, rounded up to an even count. Called by func_003BF0F0
 * in mode 1.
 */
void func_003BD9C0(void) {
    int temp_3;
    int var_7;
    int var_8;
    void *temp_4;
    void *temp_4_2;
    void *temp_5;
    void *temp_5_2;
    void *var_6;

    var_7 = 0;
    var_6 = (*(int *)((char*)(D_0050AC68) + (8))) + 141;
    if ((*(unsigned char *)((char*)(D_0050AC68) + (1))) != 0) {
        var_8 = 4;
        do {
            temp_3 = var_7 * 4;
            var_7 += 2;
            temp_4 = (*(void **)((char*)((D_0050AC68 + temp_3)) + (0x84)));
            temp_5 = temp_4 + 4;
            (*(signed char *)((char*)(var_6) + (-1))) = (signed char) (((*(unsigned char *)((char*)(temp_4) + (4))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (1))) >> 6));
            (*(signed char *)((char*)(var_6) + (2))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (1))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (2))) >> 5));
            (*(signed char *)((char*)(var_6) + (1))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (2))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (3))) >> 4));
            (*(signed char *)((char*)(var_6) + (4))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (3))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (4))) >> 3));
            (*(signed char *)((char*)(var_6) + (3))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (4))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (5))) >> 2));
            (*(signed char *)((char*)(var_6) + (6))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (5))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (6))) >> 1));
            (*(signed char *)((char*)(var_6) + (5))) = (signed char) ((*(unsigned char *)((char*)(temp_5) + (7))) | ((*(unsigned char *)((char*)(temp_5) + (6))) << 7));
            (*(signed char *)((char*)(var_6) + (8))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (8))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (9))) >> 6));
            (*(signed char *)((char*)(var_6) + (7))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (9))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0xA))) >> 5));
            (*(signed char *)((char*)(var_6) + (0xA))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0xA))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0xB))) >> 4));
            (*(signed char *)((char*)(var_6) + (9))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0xB))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0xC))) >> 3));
            (*(signed char *)((char*)(var_6) + (0xC))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0xC))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0xD))) >> 2));
            (*(signed char *)((char*)(var_6) + (0xB))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0xD))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0xE))) >> 1));
            (*(signed char *)((char*)(var_6) + (0xE))) = (signed char) ((*(unsigned char *)((char*)(temp_5) + (0xF))) | ((*(unsigned char *)((char*)(temp_5) + (0xE))) << 7));
            (*(signed char *)((char*)(var_6) + (0xD))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x10))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x11))) >> 6));
            (*(signed char *)((char*)(var_6) + (0x10))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x11))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x12))) >> 5));
            (*(signed char *)((char*)(var_6) + (0xF))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x12))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x13))) >> 4));
            (*(signed char *)((char*)(var_6) + (0x12))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x13))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x14))) >> 3));
            (*(signed char *)((char*)(var_6) + (0x11))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x14))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x15))) >> 2));
            (*(signed char *)((char*)(var_6) + (0x14))) = (signed char) (((*(unsigned char *)((char*)(temp_5) + (0x15))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5) + (0x16))) >> 1));
            (*(signed char *)((char*)(var_6) + (0x13))) = (signed char) ((*(unsigned char *)((char*)(temp_5) + (0x17))) | ((*(unsigned char *)((char*)(temp_5) + (0x16))) << 7));
            temp_4_2 = (*(void **)((char*)((D_0050AC68 + var_8)) + (0x84)));
            var_8 += 8;
            temp_5_2 = temp_4_2 + 4;
            (*(signed char *)((char*)(var_6) + (0x16))) = (signed char) (((*(unsigned char *)((char*)(temp_4_2) + (4))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (1))) >> 6));
            (*(signed char *)((char*)(var_6) + (0x15))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (1))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (2))) >> 5));
            (*(signed char *)((char*)(var_6) + (0x18))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (2))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (3))) >> 4));
            (*(signed char *)((char*)(var_6) + (0x17))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (3))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (4))) >> 3));
            (*(signed char *)((char*)(var_6) + (0x1A))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (4))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (5))) >> 2));
            (*(signed char *)((char*)(var_6) + (0x19))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (5))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (6))) >> 1));
            (*(signed char *)((char*)(var_6) + (0x1C))) = (signed char) ((*(unsigned char *)((char*)(temp_5_2) + (7))) | ((*(unsigned char *)((char*)(temp_5_2) + (6))) << 7));
            (*(signed char *)((char*)(var_6) + (0x1B))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (8))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (9))) >> 6));
            (*(signed char *)((char*)(var_6) + (0x1E))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (9))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0xA))) >> 5));
            (*(signed char *)((char*)(var_6) + (0x1D))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0xA))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0xB))) >> 4));
            (*(signed char *)((char*)(var_6) + (0x20))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0xB))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0xC))) >> 3));
            (*(signed char *)((char*)(var_6) + (0x1F))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0xC))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0xD))) >> 2));
            (*(signed char *)((char*)(var_6) + (0x22))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0xD))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0xE))) >> 1));
            (*(signed char *)((char*)(var_6) + (0x21))) = (signed char) ((*(unsigned char *)((char*)(temp_5_2) + (0xF))) | ((*(unsigned char *)((char*)(temp_5_2) + (0xE))) << 7));
            (*(signed char *)((char*)(var_6) + (0x24))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x10))) * 2) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x11))) >> 6));
            (*(signed char *)((char*)(var_6) + (0x23))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x11))) * 4) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x12))) >> 5));
            (*(signed char *)((char*)(var_6) + (0x26))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x12))) * 8) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x13))) >> 4));
            (*(signed char *)((char*)(var_6) + (0x25))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x13))) * 0x10) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x14))) >> 3));
            (*(signed char *)((char*)(var_6) + (0x28))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x14))) << 5) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x15))) >> 2));
            (*(signed char *)((char*)(var_6) + (0x27))) = (signed char) (((*(unsigned char *)((char*)(temp_5_2) + (0x15))) << 6) | ((unsigned char) (*(unsigned char *)((char*)(temp_5_2) + (0x16))) >> 1));
            (*(signed char *)((char*)(var_6) + (0x2A))) = (signed char) ((*(unsigned char *)((char*)(temp_5_2) + (0x17))) | ((*(unsigned char *)((char*)(temp_5_2) + (0x16))) << 7));
            var_6 += 0x2A;
        } while (var_7 < (int) (*(unsigned char *)((char*)(D_0050AC68) + (1))));
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BDE08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Blk_DE08 {
    int *data;
    unsigned char sf[24];
};
struct S_DE08 {
    unsigned char mode;
    unsigned char n;
    char pad2[6];
    unsigned char *out;
    float fC[24];
    int a6C[6];
    struct Blk_DE08 *ptrs[5];
};
struct C_DE08 {
    char pad0[0xA00];
    unsigned short q[16];
    float *pA20;
};
extern struct S_DE08 *D_0050AC68_DE08 __asm__("D_0050AC68");
extern struct C_DE08 *D_0050AC6C_DE08 __asm__("D_0050AC6C");
extern int D_0044C760_DE08[] __asm__("D_0044C760");
extern signed char D_0044DD38_DE08[] __asm__("D_0044DD38");
extern void f03E0_DE08(struct C_DE08 *, float *) __asm__("func_003C03E0");
extern void f0564_DE08(void *, void *, float) __asm__("func_003C0564");
extern void f05E0_DE08(void *, void *, float) __asm__("func_003C05E0");
extern void f0640_DE08(void *, void *, float) __asm__("func_003C0640");

/**
 * Quantizes and packs one channel's subband samples for a DTS 5.1 frame (mode 2).
 *
 * Same scale-factor step as func_003BD5B8 (func_003C03E0 peaks times the band weights, index stored in the channel
 * block). Bands 0-4 become 10-bit codes (func_003C0564), bands 5-13 8-bit codes (func_003C05E0) and bands 14-23 5-bit
 * codes (func_003C0640, two bands per pass), packed 3 bits out of byte alignment and byte-swapped into the output frame
 * at ch * 172 + 284, the second 8 samples of each band 0x35C bytes further on.
 *
 * @param ch  the channel index
 */
void func_003BDE08(int ch) {
    struct Blk_DE08 *blk;
    unsigned char *sf;
    unsigned char *out;
    unsigned short *quantized;
    float x;
    int i;
    int n;
    int idx;

    blk = D_0050AC68_DE08->ptrs[ch];
    f03E0_DE08(D_0050AC6C_DE08, D_0050AC6C_DE08->pA20);
    sf = blk->sf;
    for (i = 0; i < 24; i++) {
        x = D_0050AC6C_DE08->pA20[i] * D_0050AC68_DE08->fC[i];
        if (x < 4.0f) {
            idx = D_0044DD38_DE08[(int)x];
        } else {
            n = (int)((float)*(int *)&x * 6.5246462099821656e-07f) - 696;
            idx = n + 1;
            if (x < (float)D_0044C760_DE08[n]) {
                idx = n;
            }
        }
        blk->sf[i] = idx;
    }
    out = D_0050AC68_DE08->out + (ch * 172 + 284);
    quantized = D_0050AC6C_DE08->q;
    for (i = 0; i < 5; i++) {
        f0564_DE08((char *)D_0050AC6C_DE08 + i * 0x40, quantized, 1.0f / (float)D_0044C760_DE08[sf[i]]);
        out[1] = (out[1] & 0xF8) | (quantized[0] >> 7);
        out[0] = (quantized[0] << 1) | (quantized[1] >> 9);
        out[3] = quantized[1] >> 1;
        out[2] = (quantized[1] << 7) | (quantized[2] >> 3);
        out[5] = (quantized[2] << 5) | (quantized[3] >> 5);
        out[4] = (quantized[3] << 3) | (quantized[4] >> 7);
        out[7] = (quantized[4] << 1) | (quantized[5] >> 9);
        out[6] = quantized[5] >> 1;
        out[9] = (quantized[5] << 7) | (quantized[6] >> 3);
        out[8] = (quantized[6] << 5) | (quantized[7] >> 5);
        out[0xB] = (out[0xB] & 7) | (quantized[7] << 3);
        out[0x35D] = (out[0x35D] & 0xF8) | (quantized[8] >> 7);
        out[0x35C] = (quantized[8] << 1) | (quantized[9] >> 9);
        out[0x35F] = quantized[9] >> 1;
        out[0x35E] = (quantized[9] << 7) | (quantized[10] >> 3);
        out[0x361] = (quantized[10] << 5) | (quantized[11] >> 5);
        out[0x360] = (quantized[11] << 3) | (quantized[12] >> 7);
        out[0x363] = (quantized[12] << 1) | (quantized[13] >> 9);
        out[0x362] = quantized[13] >> 1;
        out[0x365] = (quantized[13] << 7) | (quantized[14] >> 3);
        out[0x364] = (quantized[14] << 5) | (quantized[15] >> 5);
        out[0x367] = (out[0x367] & 7) | (quantized[15] << 3);
        out += 10;
    }
    for (; i < 14; i++) {
        f05E0_DE08((char *)D_0050AC6C_DE08 + i * 0x40, quantized, 1.0f / (float)D_0044C760_DE08[sf[i]]);
        out[1] = (out[1] & 0xF8) | (((unsigned char *)quantized)[0] >> 5);
        out[0] = (((unsigned char *)quantized)[0] << 3) | (((unsigned char *)quantized)[1] >> 5);
        out[3] = (((unsigned char *)quantized)[1] << 3) | (((unsigned char *)quantized)[2] >> 5);
        out[2] = (((unsigned char *)quantized)[2] << 3) | (((unsigned char *)quantized)[3] >> 5);
        out[5] = (((unsigned char *)quantized)[3] << 3) | (((unsigned char *)quantized)[4] >> 5);
        out[4] = (((unsigned char *)quantized)[4] << 3) | (((unsigned char *)quantized)[5] >> 5);
        out[7] = (((unsigned char *)quantized)[5] << 3) | (((unsigned char *)quantized)[6] >> 5);
        out[6] = (((unsigned char *)quantized)[6] << 3) | (((unsigned char *)quantized)[7] >> 5);
        out[9] = (out[9] & 7) | (((unsigned char *)quantized)[7] << 3);
        out[0x35D] = (out[0x35D] & 0xF8) | (((unsigned char *)quantized)[8] >> 5);
        out[0x35C] = (((unsigned char *)quantized)[8] << 3) | (((unsigned char *)quantized)[9] >> 5);
        out[0x35F] = (((unsigned char *)quantized)[9] << 3) | (((unsigned char *)quantized)[10] >> 5);
        out[0x35E] = (((unsigned char *)quantized)[10] << 3) | (((unsigned char *)quantized)[11] >> 5);
        out[0x361] = (((unsigned char *)quantized)[11] << 3) | (((unsigned char *)quantized)[12] >> 5);
        out[0x360] = (((unsigned char *)quantized)[12] << 3) | (((unsigned char *)quantized)[13] >> 5);
        out[0x363] = (((unsigned char *)quantized)[13] << 3) | (((unsigned char *)quantized)[14] >> 5);
        out[0x362] = (((unsigned char *)quantized)[14] << 3) | (((unsigned char *)quantized)[15] >> 5);
        out[0x365] = (out[0x365] & 7) | (((unsigned char *)quantized)[15] << 3);
        out += 8;
    }
    for (; i < 24; i += 2) {
        f0640_DE08((char *)D_0050AC6C_DE08 + i * 0x40, quantized, 1.0f / (float)D_0044C760_DE08[sf[i]]);
        out[1] = (out[1] & 0xF8) | (((unsigned char *)quantized)[0] >> 2);
        out[0] = (((unsigned char *)quantized)[0] << 6) | (((unsigned char *)quantized)[1] << 1) | (((unsigned char *)quantized)[2] >> 4);
        out[3] = (((unsigned char *)quantized)[2] << 4) | (((unsigned char *)quantized)[3] >> 1);
        out[2] = (((unsigned char *)quantized)[3] << 7) | (((unsigned char *)quantized)[4] << 2) | (((unsigned char *)quantized)[5] >> 3);
        out[5] = ((unsigned char *)quantized)[6] | (((unsigned char *)quantized)[5] << 5);
        out[4] = (out[4] & 7) | (((unsigned char *)quantized)[7] << 3);
        out[0x35D] = (out[0x35D] & 0xF8) | (((unsigned char *)quantized)[8] >> 2);
        out[0x35C] = (((unsigned char *)quantized)[8] << 6) | (((unsigned char *)quantized)[9] << 1) | (((unsigned char *)quantized)[10] >> 4);
        out[0x35F] = (((unsigned char *)quantized)[10] << 4) | (((unsigned char *)quantized)[11] >> 1);
        out[0x35E] = (((unsigned char *)quantized)[11] << 7) | (((unsigned char *)quantized)[12] << 2) | (((unsigned char *)quantized)[13] >> 3);
        out[0x361] = ((unsigned char *)quantized)[14] | (((unsigned char *)quantized)[13] << 5);
        out[0x360] = (out[0x360] & 7) | (((unsigned char *)quantized)[15] << 3);
        f0640_DE08((char *)D_0050AC6C_DE08 + (i + 1) * 0x40, quantized, 1.0f / (float)D_0044C760_DE08[sf[i + 1]]);
        out[4] = (out[4] & 0xF8) | (((unsigned char *)quantized)[0] >> 2);
        out[7] = (((unsigned char *)quantized)[0] << 6) | (((unsigned char *)quantized)[1] << 1) | (((unsigned char *)quantized)[2] >> 4);
        out[6] = (((unsigned char *)quantized)[2] << 4) | (((unsigned char *)quantized)[3] >> 1);
        out[9] = (((unsigned char *)quantized)[3] << 7) | (((unsigned char *)quantized)[4] << 2) | (((unsigned char *)quantized)[5] >> 3);
        out[8] = ((unsigned char *)quantized)[6] | (((unsigned char *)quantized)[5] << 5);
        out[0xB] = (out[0xB] & 7) | (((unsigned char *)quantized)[7] << 3);
        out[0x360] = (out[0x360] & 0xF8) | (((unsigned char *)quantized)[8] >> 2);
        out[0x363] = (((unsigned char *)quantized)[8] << 6) | (((unsigned char *)quantized)[9] << 1) | (((unsigned char *)quantized)[10] >> 4);
        out[0x362] = (((unsigned char *)quantized)[10] << 4) | (((unsigned char *)quantized)[11] >> 1);
        out[0x365] = (((unsigned char *)quantized)[11] << 7) | (((unsigned char *)quantized)[12] << 2) | (((unsigned char *)quantized)[13] >> 3);
        out[0x364] = ((unsigned char *)quantized)[14] | (((unsigned char *)quantized)[13] << 5);
        out[0x367] = (out[0x367] & 7) | (((unsigned char *)quantized)[15] << 3);
        out += 10;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BE6E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* packs 24 7-bit scale factors per channel (21 bytes, 16-bit byte-swapped) */
struct Blk_E6E0 {
    int *data;
    unsigned char sf[24];
};
struct S_E6E0 {
    unsigned char mode;
    unsigned char n;
    char pad2[6];
    unsigned char *out;
    char padC[0x84 - 0xC];
    struct Blk_E6E0 *ptrs[5];
};
extern struct S_E6E0 *D_0050AC68_E6E0 __asm__("D_0050AC68");

/**
 * Packs the scale factors and the LFE channel into a DTS 5.1 frame (mode 2).
 *
 * Writes each channel's 24 7-bit scale-factor indices from +0xAA of the output frame (21 bytes per channel, 3 bits out
 * of byte alignment, odd channels shifted one byte), then encodes the LFE input (the sixth input pointer, @p channelInputs +
 * 0x14) with func_003BD078 and packs its 8 samples and scale index from +0x112. Called by func_003BF0F0.
 *
 * @param channelInputs  the encoder's per-channel input pointer array
 */
void func_003BE6E0(char *channelInputs) {
    signed char vals[8];
    signed char idx;
    unsigned char *outCursor;
    unsigned char *sf;
    struct Blk_E6E0 *blk;
    int i;

    outCursor = D_0050AC68_E6E0->out + 0xAA;
    for (i = 0; i < D_0050AC68_E6E0->n; i++) {
        blk = D_0050AC68_E6E0->ptrs[i];
        sf = blk->sf;
        if (i & 1) {
            outCursor[-1] = (outCursor[-1] & 0xF8) | (sf[0] >> 4);
            outCursor[2] = (sf[0] << 4) | (sf[1] >> 3);
            outCursor[1] = (sf[1] << 5) | (sf[2] >> 2);
            outCursor[4] = (sf[2] << 6) | (sf[3] >> 1);
            outCursor[3] = (sf[3] << 7) | sf[4];
            outCursor[6] = (sf[5] << 1) | (sf[6] >> 6);
            outCursor[5] = (sf[6] << 2) | (sf[7] >> 5);
            outCursor[8] = (sf[7] << 3) | (sf[8] >> 4);
            outCursor[7] = (sf[8] << 4) | (sf[9] >> 3);
            outCursor[10] = (sf[9] << 5) | (sf[10] >> 2);
            outCursor[9] = (sf[10] << 6) | (sf[11] >> 1);
            outCursor[12] = (sf[11] << 7) | sf[12];
            outCursor[11] = (sf[13] << 1) | (sf[14] >> 6);
            outCursor[14] = (sf[14] << 2) | (sf[15] >> 5);
            outCursor[13] = (sf[15] << 3) | (sf[16] >> 4);
            outCursor[16] = (sf[16] << 4) | (sf[17] >> 3);
            outCursor[15] = (sf[17] << 5) | (sf[18] >> 2);
            outCursor[18] = (sf[18] << 6) | (sf[19] >> 1);
            outCursor[17] = (sf[19] << 7) | sf[20];
            outCursor[20] = (sf[21] << 1) | (sf[22] >> 6);
            outCursor[19] = (sf[22] << 2) | (sf[23] >> 5);
            outCursor[21] = (outCursor[21] & 7) | (sf[23] << 3);
        } else {
            outCursor[1] = (outCursor[1] & 0xF8) | (sf[0] >> 4);
            outCursor[0] = (sf[0] << 4) | (sf[1] >> 3);
            outCursor[3] = (sf[1] << 5) | (sf[2] >> 2);
            outCursor[2] = (sf[2] << 6) | (sf[3] >> 1);
            outCursor[5] = (sf[3] << 7) | sf[4];
            outCursor[4] = (sf[5] << 1) | (sf[6] >> 6);
            outCursor[7] = (sf[6] << 2) | (sf[7] >> 5);
            outCursor[6] = (sf[7] << 3) | (sf[8] >> 4);
            outCursor[9] = (sf[8] << 4) | (sf[9] >> 3);
            outCursor[8] = (sf[9] << 5) | (sf[10] >> 2);
            outCursor[11] = (sf[10] << 6) | (sf[11] >> 1);
            outCursor[10] = (sf[11] << 7) | sf[12];
            outCursor[13] = (sf[13] << 1) | (sf[14] >> 6);
            outCursor[12] = (sf[14] << 2) | (sf[15] >> 5);
            outCursor[15] = (sf[15] << 3) | (sf[16] >> 4);
            outCursor[14] = (sf[16] << 4) | (sf[17] >> 3);
            outCursor[17] = (sf[17] << 5) | (sf[18] >> 2);
            outCursor[16] = (sf[18] << 6) | (sf[19] >> 1);
            outCursor[19] = (sf[19] << 7) | sf[20];
            outCursor[18] = (sf[21] << 1) | (sf[22] >> 6);
            outCursor[21] = (sf[22] << 2) | (sf[23] >> 5);
            outCursor[20] = (outCursor[20] & 7) | (sf[23] << 3);
        }
        outCursor += 21;
    }
    outCursor = D_0050AC68_E6E0->out + 0x113;
    func_003BD078(*(int *)(channelInputs + 0x14), &idx, vals);
    outCursor[-1] = (outCursor[-1] & 0xF8) | (vals[0] >> 5);
    outCursor[2] = (vals[0] << 5) | (vals[1] >> 5);
    outCursor[1] = (vals[1] << 5) | (vals[2] >> 5);
    outCursor[4] = (vals[2] << 5) | (vals[3] >> 5);
    outCursor[3] = (vals[3] << 5) | (vals[4] >> 5);
    outCursor[6] = (vals[4] << 5) | (vals[5] >> 5);
    outCursor[5] = (vals[5] << 5) | (vals[6] >> 5);
    outCursor[8] = (vals[6] << 5) | (vals[7] >> 5);
    outCursor[7] = (vals[7] << 5) | ((unsigned char)idx >> 5);
    outCursor[10] = (outCursor[10] & 7) | (idx << 5);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BECA0);
#ifdef SKIP_ASM
/**
 * Returns the two buffer sizes the DTS encoder needs for a mode and quality.
 *
 * Quality below 34 gives a filter length of 0xE0, below 67 0x160 and up to 100 0x1E0; mode 1 (DTS 4.0) has 4 channels
 * and mode 2 (5.1) 5. The state size is (length * 4 + 0x3C) * channels + 168 and the work size (2 * length + 0x200) * 4
 * + 0xC00. func_003BC930 installs it as the size hook D_0050C840, which SNDPLATFORM_init calls before allocating.
 *
 * @param mode  the mode (1 = DTS 4.0, 2 = DTS 5.1)
 * @param quality  the quality (0-100)
 * @param outStateSize  receives the state size
 * @param outWorkSize  receives the work-area size
 * @return 0, -1 for a bad mode or -2 for a quality out of range
 */
int func_003BECA0(int mode, int quality, int *outStateSize, int *outWorkSize) {
    int w, channelCount;
    if (quality < 0) return -2;
    if (quality < 34) w = 0xE0;
    else if (quality < 67) w = 352;
    else if (quality < 101) w = 0x1E0;
    else return -2;
    if (mode == 1) channelCount = 4;
    else if (mode == 2) channelCount = 5;
    else return -1;
    *outStateSize = (w * 4 + 0x3C) * channelCount + 168;
    *outWorkSize = (w + (w + 0x200)) * 4 + 0xC00;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BED30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct sAC68 *D_0050AC68_ED30 __asm__("D_0050AC68");

extern float D_0044CA40_ED30[] __asm__("D_0044CA40");
extern float D_0044C700_ED30[] __asm__("D_0044C700");

/**
 * Sets up the DTS encoder state in a buffer for a mode and quality.
 *
 * Aligns @p buf to 16 and places the state D_0050AC68 there (0x98 bytes): filter length +0x04 from the quality (0xE0,
 * 0x160 or 0x1E0, as func_003BECA0), channel count +0x01 (4 for mode 1, 5 for mode 2), no output buffer (+0x08), mode
 * +0x00; then a block per channel (+0x84 array) with a zeroed filter history of +0x04 words, and the LFE state +0x6C
 * cleared. The band weights +0x0C are 1/511 (10-bit bands), 1/127 (8-bit) and 1/15 (5-bit) per the mode's band split,
 * and the coefficient table D_0044CA40 is rebuilt from D_0044C700 divided by a per-split constant. Installed by
 * func_003BC930 as the setup hook D_0050C844.
 *
 * @param mode  1 for DTS 4.0, 2 for DTS 5.1
 * @param rate  the quality (0-100)
 * @param buf  the state buffer (sized by func_003BECA0)
 * @return 0, -1 for a bad mode or -2 for a quality out of range
 */
int func_003BED30(int mode, int rate, char *buf) {
    int i;
    int j;

    buf = (char *)((((int)buf - 1) | 0xF) + 1);
    D_0050AC68_ED30 = (struct sAC68 *)buf;
    buf += 0x98;
    if (rate < 0) {
        return -2;
    }
    if (rate < 34) {
        D_0050AC68_ED30->f04 = 0xE0;
    } else if (rate < 67) {
        D_0050AC68_ED30->f04 = 352;
    } else if (rate < 101) {
        D_0050AC68_ED30->f04 = 0x1E0;
    } else {
        return -2;
    }
    if (mode == 1) {
        D_0050AC68_ED30->f01 = 4;
    } else if (mode == 2) {
        D_0050AC68_ED30->f01 = 5;
    } else {
        return -1;
    }
    D_0050AC68_ED30->f08 = 0;
    D_0050AC68_ED30->f00 = mode;
    for (i = 0; i < D_0050AC68_ED30->f01; i++) {
        struct sAC68Blk *blk;
        buf = (char *)((((int)buf - 1) | 0xF) + 1);
        blk = (struct sAC68Blk *)buf;
        D_0050AC68_ED30->f84[i] = blk;
        buf = (char *)((((int)buf + 0x1C - 1) | 0xF) + 1);
        blk->f00 = (int)buf;
        buf += D_0050AC68_ED30->f04 * 4;
        for (j = 0; j < D_0050AC68_ED30->f04; j++) {
            ((int *)D_0050AC68_ED30->f84[i]->f00)[j] = 0;
        }
    }
    for (i = 0; i < 6; i++) {
        D_0050AC68_ED30->f6C[i] = 0;
    }
    i = 0;
    switch (mode) {
    case 1:
        for (; i < 14; i++) {
            D_0050AC68_ED30->f0C[i] = 0.00195694715f;
            D_0044CA40_ED30[i] = D_0044C700_ED30[i] / 0.010999918f;
        }
        for (; i < 24; i++) {
            D_0050AC68_ED30->f0C[i] = 0.00787401572f;
            D_0044CA40_ED30[i] = D_0044C700_ED30[i] / 0.0350000858f;
        }
        break;
    case 2:
    default:
        for (; i < 5; i++) {
            D_0050AC68_ED30->f0C[i] = 0.00195694715f;
            D_0044CA40_ED30[i] = D_0044C700_ED30[i] / 0.010999918f;
        }
        for (; i < 14; i++) {
            D_0050AC68_ED30->f0C[i] = 0.00787401572f;
            D_0044CA40_ED30[i] = D_0044C700_ED30[i] / 0.0350000858f;
        }
        for (; i < 24; i++) {
            D_0050AC68_ED30->f0C[i] = 0.0666666701f;
            D_0044CA40_ED30[i] = D_0044C700_ED30[i] / 0.190000057f;
        }
        break;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003BF0F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct C_F0F0 {
    char pad0[0xA20];
    char *pA20;
    char *pA24;
    char *pA28;
    char *pA2C;
    char *pA30;
    char *pA34;
    char *pA38;
    unsigned char flagA3C;
};
struct S_F0F0 {
    unsigned char mode;
    unsigned char n;
    char pad2[2];
    unsigned short n4;
    short h6;
    int key;
    char padC[0x84 - 0xC];
    int *ptrs[1];
};
extern struct S_F0F0 *D_0050AC68_F0F0 __asm__("D_0050AC68");
extern struct C_F0F0 *D_0050AC6C_F0F0 __asm__("D_0050AC6C");
extern char D_0044C980_F0F0[] __asm__("D_0044C980");
extern char D_0044C990_F0F0[] __asm__("D_0044C990");
extern char D_0044C9B0_F0F0[] __asm__("D_0044C9B0");
extern char D_0044C9E0_F0F0[] __asm__("D_0044C9E0");
extern char D_0044CA40_F0F0[] __asm__("D_0044CA40");
extern char D_0044CB00_F0F0[] __asm__("D_0044CB00");
extern char D_0044CF00_F0F0[] __asm__("D_0044CF00");
extern char D_0044D500_F0F0[] __asm__("D_0044D500");
extern void f_C968_F0F0(char *, char *, int, int) __asm__("func_003BC968");
extern void f_CBA0_F0F0(void) __asm__("func_003BCBA0");
extern void f_D220_F0F0(int, struct C_F0F0 *, int) __asm__("func_003BD220");
extern void f_D5B8_F0F0(int) __asm__("func_003BD5B8");
extern void f_DE08_F0F0(int) __asm__("func_003BDE08");
extern void f_D9C0_F0F0(void) __asm__("func_003BD9C0");
extern void f_E6E0_F0F0(int *) __asm__("func_003BE6E0");

/**
 * Encodes one DTS frame from the channels' PCM input into an output buffer.
 *
 * Fails with -3 for an odd @p key or an input not 16-byte aligned. Places the work area D_0050AC6C in @p buf (aligned
 * to 16): in the scratchpad (+0xA3C set) it DMAs the window table for the filter length (D_0044CB00, D_0044CF00 or
 * D_0044D500) and the transform tables (D_0044C980) in with func_003BC968, otherwise it points at them in place. When
 * @p key differs from the state's output buffer (+0x08 of D_0050AC68) it stores it, resets the bit position and writes
 * the frame header (func_003BCBA0). Each channel then goes through func_003BD220 and is packed by func_003BD5B8 (mode
 * 1) or func_003BDE08 (mode 2), and the scale factors by func_003BD9C0 or func_003BE6E0. Installed by func_003BC930 as
 * the encode hook D_0050C848.
 *
 * @param data  the per-channel PCM input pointers
 * @param key  the output frame buffer (misnamed: it is a pointer, not a key)
 * @param buf  the work-area buffer (sized by func_003BECA0)
 * @return 0, or -3 for a misaligned input or output
 */
int func_003BF0F0(int *data, int key, char *buf) {
    int i;

    if (key & 1) {
        return -3;
    }
    buf = (char *)((((int)buf - 1) | 0xF) + 1);
    D_0050AC6C_F0F0 = (struct C_F0F0 *)buf;
    D_0050AC6C_F0F0->flagA3C = ((int)buf & 0x70000000) == 0x70000000;
    if (D_0050AC6C_F0F0->flagA3C) {
        buf += 0xA40;
        D_0050AC6C_F0F0->pA24 = buf;
        if (D_0050AC68_F0F0->n4 == 0xE0) {
            f_C968_F0F0(D_0044CB00_F0F0, buf, 0x40, 0);
            buf += 0x400;
        } else if (D_0050AC68_F0F0->n4 == 352) {
            f_C968_F0F0(D_0044CF00_F0F0, buf, 0x60, 0);
            buf += 0x600;
        } else {
            f_C968_F0F0(D_0044D500_F0F0, buf, 0x80, 0);
            buf += 0x800;
        }
        D_0050AC6C_F0F0->pA20 = buf;
        buf += (D_0050AC68_F0F0->n4 + 0x200) * 4;
        D_0050AC6C_F0F0->pA28 = buf;
        D_0050AC6C_F0F0->pA2C = buf + 0x10;
        D_0050AC6C_F0F0->pA30 = buf + 0x30;
        D_0050AC6C_F0F0->pA34 = buf + 0x60;
        D_0050AC6C_F0F0->pA38 = buf + 0xC0;
        f_C968_F0F0(D_0044C980_F0F0, buf, 18, 0x10000);
    } else {
        D_0050AC6C_F0F0->pA20 = buf + 0xA40;
        if (D_0050AC68_F0F0->n4 == 0xE0) {
            D_0050AC6C_F0F0->pA24 = D_0044CB00_F0F0;
        } else if (D_0050AC68_F0F0->n4 == 352) {
            D_0050AC6C_F0F0->pA24 = D_0044CF00_F0F0;
        } else if (D_0050AC68_F0F0->n4 == 0x1E0) {
            D_0050AC6C_F0F0->pA24 = D_0044D500_F0F0;
        }
        D_0050AC6C_F0F0->pA28 = D_0044C980_F0F0;
        D_0050AC6C_F0F0->pA2C = D_0044C990_F0F0;
        D_0050AC6C_F0F0->pA30 = D_0044C9B0_F0F0;
        D_0050AC6C_F0F0->pA34 = D_0044C9E0_F0F0;
        D_0050AC6C_F0F0->pA38 = D_0044CA40_F0F0;
    }
    if (D_0050AC68_F0F0->key != key) {
        D_0050AC68_F0F0->key = key;
        D_0050AC68_F0F0->h6 = 0;
        f_CBA0_F0F0();
    }
    for (i = 0; i < D_0050AC68_F0F0->n; i++) {
        if (data[i] & 0xF) {
            return -3;
        }
        f_D220_F0F0(data[i], D_0050AC6C_F0F0, *D_0050AC68_F0F0->ptrs[i]);
        if (D_0050AC68_F0F0->mode == 1) {
            f_D5B8_F0F0(i);
        } else {
            f_DE08_F0F0(i);
        }
    }
    if (D_0050AC68_F0F0->mode == 1) {
        f_D9C0_F0F0();
    } else {
        f_E6E0_F0F0(data);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF3E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "fakematch.h"
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F3E0;

// PORT: PS2-only VU0 inline asm (load eight consecutive vec4 rows into vf1..vf8).
static inline void vu0Load8_F3E0(const V4_F3E0 *p) {
    __asm__ volatile(
        "lqc2       $vf1, 0x0(%0)\n"
        "lqc2       $vf2, 0x10(%0)\n"
        "lqc2       $vf3, 0x20(%0)\n"
        "lqc2       $vf4, 0x30(%0)\n"
        "lqc2       $vf5, 0x40(%0)\n"
        "lqc2       $vf6, 0x50(%0)\n"
        "lqc2       $vf7, 0x60(%0)\n"
        "lqc2       $vf8, 0x70(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf1..vf6 to rows 0..5).
static inline void vu0Store6_F3E0(V4_F3E0 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf2, 0x10(%0)\n"
        "sqc2       $vf3, 0x20(%0)\n"
        "sqc2       $vf4, 0x30(%0)\n"
        "sqc2       $vf5, 0x40(%0)\n"
        "sqc2       $vf6, 0x50(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf7 to row 6).
static inline void vu0StoreV7_F3E0(V4_F3E0 *d) {
    __asm__ volatile("sqc2       $vf7, 0x60(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf8 to row 7).
static inline void vu0StoreV8_F3E0(V4_F3E0 *d) {
    __asm__ volatile("sqc2       $vf8, 0x70(%0)\n" : : "r"(d) : "memory");
}

// FAKEMATCH: FM_REG("$2") pin on the block counter i (global.c's set_preference ties `i = n >> 5` to n's a2;
// retail counts in v0); plain form 99.13%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
/* Copies n words (n >> 5 blocks of 128 bytes) through VU0 registers. The goto loop has no loop notes, so gcc
   doesn't align its label (a do-while gets an alignment slot: 94.78). */
/**
 * Copies a block of words through VU0 registers, 128 bytes (eight quadwords) per pass.
 *
 * Runs n >> 5 passes, each loading eight consecutive quadwords from @p src into vf1..vf8 and storing them to @p dst;
 * both must be 16-byte aligned and any remainder below 32 words is not copied. The DTS encoder's analysis filterbank
 * func_003BD220 uses it to copy a channel's filter history in and out of the work area when it cannot use scratchpad
 * DMA.
 *
 * @param src  the source (16-byte aligned)
 * @param dst  the destination (16-byte aligned)
 * @param n  the number of 32-bit words (a multiple of 32)
 */
void func_003BF3E0(V4_F3E0 *src, V4_F3E0 *dst, unsigned int n) {
    register int i FM_REG("$2") = n >> 5;
top:
    vu0Load8_F3E0(src);
    vu0Store6_F3E0(dst);
    i--;
    vu0StoreV7_F3E0(dst);
    src += 8;
    vu0StoreV8_F3E0(dst);
    dst += 8;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF440);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "fakematch.h"
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F440;

// PORT: hand-written MMI + VU0 asm (pextlh/psraw have no gcc 2.95 pattern; the fixed $7..$10 are registers no
// allocator picks here: gcc's own choice was v1/a0..a2, 87.74): loads 16 16-bit samples, widens each to a 32-bit
// lane as sample << 8 (pextlh with $0, then psraw 8), moves them to vf1..vf4 and converts them to float. A port
// writes dst[k] = (float)(src[k] * 256) in C.
static inline void mmiSamplesToFloat_F440(const long *s) {
    __asm__ volatile(
        "ld         $7, 0x0(%0)\n"
        "ld         $8, 0x8(%0)\n"
        "ld         $9, 0x10(%0)\n"
        "ld         $10, 0x18(%0)\n"
        "pextlh     $7, $7, $0\n"
        "pextlh     $8, $8, $0\n"
        "pextlh     $9, $9, $0\n"
        "pextlh     $10, $10, $0\n"
        "psraw      $7, $7, 8\n"
        "psraw      $8, $8, 8\n"
        "psraw      $9, $9, 8\n"
        "psraw      $10, $10, 8\n"
        "qmtc2.ni   $7, $vf1\n"
        "qmtc2.ni   $8, $vf2\n"
        "qmtc2.ni   $9, $vf3\n"
        "qmtc2.ni   $10, $vf4\n"
        "vitof0.xyzw $vf1, $vf1\n"
        "vitof0.xyzw $vf2, $vf2\n"
        "vitof0.xyzw $vf3, $vf3\n"
        "vitof0.xyzw $vf4, $vf4\n"
        : : "r"(s) : "$7", "$8", "$9", "$10", "memory");
}

// PORT: PS2-only VU0 inline asm (store vf1, vf2 to rows 0, 1).
static inline void vu0Store12_F440(V4_F440 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf2, 0x10(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf3 to row 2).
static inline void vu0StoreV3_F440(V4_F440 *d) {
    __asm__ volatile("sqc2       $vf3, 0x20(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf4 to row 3).
static inline void vu0StoreV4_F440(V4_F440 *d) {
    __asm__ volatile("sqc2       $vf4, 0x30(%0)\n" : : "r"(d) : "memory");
}

// PORT: hand-written MMI sample conversion (mmiSamplesToFloat_F440 above: pextlh/psraw, fixed $7..$10); a port
// replaces it with a C int16-to-float loop.
// FAKEMATCH: FM_REG("$2") pin on the block counter i (global.c's set_preference ties `i = n >> 4` to n's a2;
// retail counts in v0); plain form 99.35%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
/* Converts n 16-bit samples to floats, 16 per pass (n >> 4 passes). */
/**
 * Converts 16-bit PCM samples to floats scaled by 256, sixteen samples per pass.
 *
 * Runs n >> 4 passes: each loads 32 bytes of samples, widens them to 32-bit lanes as sample << 8 with MMI (pextlh,
 * psraw), moves them to vf1..vf4, converts them to float on VU0 and stores four quadwords to @p dst. Called by the DTS
 * encoder's analysis filterbank func_003BD220 on a channel's PCM input (all 512 samples at once, or 128 per block).
 *
 * @param src  the 16-bit samples (16-byte aligned)
 * @param dst  the float output (16-byte aligned)
 * @param n  the number of samples (a multiple of 16)
 */
void func_003BF440(long *src, V4_F440 *dst, unsigned int n) {
    register int i FM_REG("$2") = n >> 4;
top:
    mmiSamplesToFloat_F440(src);
    vu0Store12_F440(dst);
    i--;
    vu0StoreV3_F440(dst);
    src += 4;
    vu0StoreV4_F440(dst);
    dst += 4;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF4C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: retail pads this function's object with two nops up to the next 16-byte boundary; that padding is a pad
// segment in the yaml (seg_2B4578 split), not code, so nothing here emits it.
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F4C0;

/**
 * Windows one block for the DTS analysis filterbank with the 0xE0-length prototype filter (4 taps per output).
 *
 * Each of 16 passes forms four outputs out_j = sum over t < 4 of weights[t] * input[j + 2t] on VU0 and stores them
 * transposed with MMI, 64 outputs in all. func_003BD220 uses it for the lowest quality setting.
 *
 * @param input  the sample history and new block
 * @param weights  the window coefficients
 * @param out  the windowed block
 */
// PORT: hand-written VU0 + MMI loop: each pass computes out_j = sum_t b[t] * a[j + 2t] (t < 4, j < 4; b rows at
// stride 0x100, a rows at stride 0x80) in ACC (vmula/vmadda), moves the four sums to $7..$10, transposes them with
// pextlw/pextuw/pcpyld/pcpyud (no gcc 2.95 pattern) and stores them with sq. The counter and pointer steps are
// hand-scheduled between the sq stores; a C loop gets there only by carrying $9/$11/$12 across separate asm
// statements behind gcc's back, so the loop stays one block. A port replaces it with a C multiply-accumulate
// loop and a 4x4 transpose.
void func_003BF4C0(V4_F4C0 *input, V4_F4C0 *weights, V4_F4C0 *out) {
    int i = 16; /* 16 passes, four outputs each */
    __asm__ volatile(
        ".set noreorder\n"
        "1:\n"
        "lqc2       $vf1, 0x0(%2)\n"
        "lqc2       $vf2, 0x100(%2)\n"
        "lqc2       $vf3, 0x200(%2)\n"
        "lqc2       $vf4, 0x300(%2)\n"
        "lqc2       $vf5, 0x0(%1)\n"
        "lqc2       $vf6, 0x80(%1)\n"
        "lqc2       $vf7, 0x100(%1)\n"
        "lqc2       $vf8, 0x180(%1)\n"
        "lqc2       $vf9, 0x200(%1)\n"
        "lqc2       $vf10, 0x280(%1)\n"
        "lqc2       $vf11, 0x300(%1)\n"
        "lqc2       $vf12, 0x380(%1)\n"
        "lqc2       $vf13, 0x400(%1)\n"
        "lqc2       $vf14, 0x480(%1)\n"
        "vmula.xyzw ACC, $vf1, $vf5\n"
        "vmadda.xyzw ACC, $vf2, $vf7\n"
        "vmadda.xyzw ACC, $vf3, $vf9\n"
        "vmadd.xyzw $vf5, $vf4, $vf11\n"
        "vmula.xyzw ACC, $vf1, $vf6\n"
        "vmadda.xyzw ACC, $vf2, $vf8\n"
        "vmadda.xyzw ACC, $vf3, $vf10\n"
        "vmadd.xyzw $vf6, $vf4, $vf12\n"
        "vmula.xyzw ACC, $vf1, $vf7\n"
        "vmadda.xyzw ACC, $vf2, $vf9\n"
        "vmadda.xyzw ACC, $vf3, $vf11\n"
        "vmadd.xyzw $vf7, $vf4, $vf13\n"
        "vmula.xyzw ACC, $vf1, $vf8\n"
        "vmadda.xyzw ACC, $vf2, $vf10\n"
        "vmadda.xyzw ACC, $vf3, $vf12\n"
        "vmadd.xyzw $vf8, $vf4, $vf14\n"
        "qmfc2.ni   $7, $vf5\n"
        "qmfc2.ni   $8, $vf6\n"
        "qmfc2.ni   $9, $vf7\n"
        "qmfc2.ni   $10, $vf8\n"
        "pextlw     $11, $8, $7\n"
        "pextuw     $8, $8, $7\n"
        "pextlw     $12, $10, $9\n"
        "pextuw     $10, $10, $9\n"
        "pcpyld     $7, $12, $11\n"
        "pcpyud     $9, $11, $12\n"
        "pcpyld     $11, $10, $8\n"
        "pcpyud     $12, $8, $10\n"
        "sq         $7, 0x0(%3)\n"
        "addiu      %0, %0, -0x1\n"
        "sq         $9, 0x10(%3)\n"
        "addiu      %1, %1, 0x10\n"
        "sq         $11, 0x20(%3)\n"
        "addiu      %2, %2, 0x10\n"
        "sq         $12, 0x30(%3)\n"
        "bnez       %0, 1b\n"
        "addiu      %3, %3, 0x40\n"
        ".set reorder\n"
        : "+r"(i), "+r"(input), "+r"(weights), "+r"(out)
        :
        : "$7", "$8", "$9", "$10", "$11", "$12", "memory");
}

// Retail pads two nops after this function (the next EA object file's .text starts 16-byte aligned);
// tools/gen/hybrid_src.py keeps it as asm in the hybrid build, whose .s carries the pad.
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF5A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F5A0;

/**
 * Windows one block for the DTS analysis filterbank with the 0x160-length prototype filter (6 taps per output).
 *
 * As func_003BF4C0 with t < 6: 16 passes of four multiply-accumulated, transposed outputs. func_003BD220 uses it for
 * the middle quality setting.
 *
 * @param input  the sample history and new block
 * @param weights  the window coefficients
 * @param out  the windowed block
 */
// PORT: hand-written VU0 + MMI loop: each pass computes out_j = sum_t b[t] * a[j + 2t] (t < 6, j < 4; b rows at
// stride 0x100, a rows at stride 0x80) in ACC (vmula/vmadda), moves the four sums to $7..$10, transposes them with
// pextlw/pextuw/pcpyld/pcpyud (no gcc 2.95 pattern) and stores them with sq. The counter and pointer steps are
// hand-scheduled between the sq stores; a C loop gets there only by carrying $9/$11/$12 across separate asm
// statements behind gcc's back, so the loop stays one block. A port replaces it with a C multiply-accumulate
// loop and a 4x4 transpose.
void func_003BF5A0(V4_F5A0 *input, V4_F5A0 *weights, V4_F5A0 *out) {
    int i = 16; /* 16 passes, four outputs each */
    __asm__ volatile(
        ".set noreorder\n"
        "1:\n"
        "lqc2       $vf1, 0x0(%2)\n"
        "lqc2       $vf2, 0x100(%2)\n"
        "lqc2       $vf3, 0x200(%2)\n"
        "lqc2       $vf4, 0x300(%2)\n"
        "lqc2       $vf5, 0x400(%2)\n"
        "lqc2       $vf6, 0x500(%2)\n"
        "lqc2       $vf7, 0x0(%1)\n"
        "lqc2       $vf8, 0x80(%1)\n"
        "lqc2       $vf9, 0x100(%1)\n"
        "lqc2       $vf10, 0x180(%1)\n"
        "lqc2       $vf11, 0x200(%1)\n"
        "lqc2       $vf12, 0x280(%1)\n"
        "lqc2       $vf13, 0x300(%1)\n"
        "lqc2       $vf14, 0x380(%1)\n"
        "lqc2       $vf15, 0x400(%1)\n"
        "lqc2       $vf16, 0x480(%1)\n"
        "lqc2       $vf17, 0x500(%1)\n"
        "lqc2       $vf18, 0x580(%1)\n"
        "lqc2       $vf19, 0x600(%1)\n"
        "lqc2       $vf20, 0x680(%1)\n"
        "vmula.xyzw ACC, $vf1, $vf7\n"
        "vmadda.xyzw ACC, $vf2, $vf9\n"
        "vmadda.xyzw ACC, $vf3, $vf11\n"
        "vmadda.xyzw ACC, $vf4, $vf13\n"
        "vmadda.xyzw ACC, $vf5, $vf15\n"
        "vmadd.xyzw $vf7, $vf6, $vf17\n"
        "vmula.xyzw ACC, $vf1, $vf8\n"
        "vmadda.xyzw ACC, $vf2, $vf10\n"
        "vmadda.xyzw ACC, $vf3, $vf12\n"
        "vmadda.xyzw ACC, $vf4, $vf14\n"
        "vmadda.xyzw ACC, $vf5, $vf16\n"
        "vmadd.xyzw $vf8, $vf6, $vf18\n"
        "vmula.xyzw ACC, $vf1, $vf9\n"
        "vmadda.xyzw ACC, $vf2, $vf11\n"
        "vmadda.xyzw ACC, $vf3, $vf13\n"
        "vmadda.xyzw ACC, $vf4, $vf15\n"
        "vmadda.xyzw ACC, $vf5, $vf17\n"
        "vmadd.xyzw $vf9, $vf6, $vf19\n"
        "vmula.xyzw ACC, $vf1, $vf10\n"
        "vmadda.xyzw ACC, $vf2, $vf12\n"
        "vmadda.xyzw ACC, $vf3, $vf14\n"
        "vmadda.xyzw ACC, $vf4, $vf16\n"
        "vmadda.xyzw ACC, $vf5, $vf18\n"
        "vmadd.xyzw $vf10, $vf6, $vf20\n"
        "qmfc2.ni   $7, $vf7\n"
        "qmfc2.ni   $8, $vf8\n"
        "qmfc2.ni   $9, $vf9\n"
        "qmfc2.ni   $10, $vf10\n"
        "pextlw     $11, $8, $7\n"
        "pextuw     $8, $8, $7\n"
        "pextlw     $12, $10, $9\n"
        "pextuw     $10, $10, $9\n"
        "pcpyld     $7, $12, $11\n"
        "pcpyud     $9, $11, $12\n"
        "pcpyld     $11, $10, $8\n"
        "pcpyud     $12, $8, $10\n"
        "sq         $7, 0x0(%3)\n"
        "addiu      %0, %0, -0x1\n"
        "sq         $9, 0x10(%3)\n"
        "addiu      %1, %1, 0x10\n"
        "sq         $11, 0x20(%3)\n"
        "addiu      %2, %2, 0x10\n"
        "sq         $12, 0x30(%3)\n"
        "bnez       %0, 1b\n"
        "addiu      %3, %3, 0x40\n"
        ".set reorder\n"
        : "+r"(i), "+r"(input), "+r"(weights), "+r"(out)
        :
        : "$7", "$8", "$9", "$10", "$11", "$12", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF6B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: retail pads this function's object with two nops up to the next 16-byte boundary; that padding is a pad
// segment in the yaml (seg_2B4578 split), not code, so nothing here emits it.
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F6B0;

/**
 * Windows one block for the DTS analysis filterbank with the 0x1E0-length prototype filter (8 taps per output).
 *
 * As func_003BF4C0 with t < 8: 16 passes of four multiply-accumulated, transposed outputs. func_003BD220 uses it for
 * the highest quality setting.
 *
 * @param input  the sample history and new block
 * @param weights  the window coefficients
 * @param out  the windowed block
 */
// PORT: hand-written VU0 + MMI loop: each pass computes out_j = sum_t b[t] * a[j + 2t] (t < 8, j < 4; b rows at
// stride 0x100, a rows at stride 0x80) in ACC (vmula/vmadda), moves the four sums to $7..$10, transposes them with
// pextlw/pextuw/pcpyld/pcpyud (no gcc 2.95 pattern) and stores them with sq. The counter and pointer steps are
// hand-scheduled between the sq stores; a C loop gets there only by carrying $9/$11/$12 across separate asm
// statements behind gcc's back, so the loop stays one block. A port replaces it with a C multiply-accumulate
// loop and a 4x4 transpose.
void func_003BF6B0(V4_F6B0 *input, V4_F6B0 *weights, V4_F6B0 *out) {
    int i = 16; /* 16 passes, four outputs each */
    __asm__ volatile(
        ".set noreorder\n"
        "1:\n"
        "lqc2       $vf1, 0x0(%2)\n"
        "lqc2       $vf2, 0x100(%2)\n"
        "lqc2       $vf3, 0x200(%2)\n"
        "lqc2       $vf4, 0x300(%2)\n"
        "lqc2       $vf5, 0x400(%2)\n"
        "lqc2       $vf6, 0x500(%2)\n"
        "lqc2       $vf7, 0x600(%2)\n"
        "lqc2       $vf8, 0x700(%2)\n"
        "lqc2       $vf9, 0x0(%1)\n"
        "lqc2       $vf10, 0x80(%1)\n"
        "lqc2       $vf11, 0x100(%1)\n"
        "lqc2       $vf12, 0x180(%1)\n"
        "lqc2       $vf13, 0x200(%1)\n"
        "lqc2       $vf14, 0x280(%1)\n"
        "lqc2       $vf15, 0x300(%1)\n"
        "lqc2       $vf16, 0x380(%1)\n"
        "lqc2       $vf17, 0x400(%1)\n"
        "lqc2       $vf18, 0x480(%1)\n"
        "lqc2       $vf19, 0x500(%1)\n"
        "lqc2       $vf20, 0x580(%1)\n"
        "lqc2       $vf21, 0x600(%1)\n"
        "lqc2       $vf22, 0x680(%1)\n"
        "lqc2       $vf23, 0x700(%1)\n"
        "lqc2       $vf24, 0x780(%1)\n"
        "lqc2       $vf25, 0x800(%1)\n"
        "lqc2       $vf26, 0x880(%1)\n"
        "vmula.xyzw ACC, $vf1, $vf9\n"
        "vmadda.xyzw ACC, $vf2, $vf11\n"
        "vmadda.xyzw ACC, $vf3, $vf13\n"
        "vmadda.xyzw ACC, $vf4, $vf15\n"
        "vmadda.xyzw ACC, $vf5, $vf17\n"
        "vmadda.xyzw ACC, $vf6, $vf19\n"
        "vmadda.xyzw ACC, $vf7, $vf21\n"
        "vmadd.xyzw $vf9, $vf8, $vf23\n"
        "vmula.xyzw ACC, $vf1, $vf10\n"
        "vmadda.xyzw ACC, $vf2, $vf12\n"
        "vmadda.xyzw ACC, $vf3, $vf14\n"
        "vmadda.xyzw ACC, $vf4, $vf16\n"
        "vmadda.xyzw ACC, $vf5, $vf18\n"
        "vmadda.xyzw ACC, $vf6, $vf20\n"
        "vmadda.xyzw ACC, $vf7, $vf22\n"
        "vmadd.xyzw $vf10, $vf8, $vf24\n"
        "vmula.xyzw ACC, $vf1, $vf11\n"
        "vmadda.xyzw ACC, $vf2, $vf13\n"
        "vmadda.xyzw ACC, $vf3, $vf15\n"
        "vmadda.xyzw ACC, $vf4, $vf17\n"
        "vmadda.xyzw ACC, $vf5, $vf19\n"
        "vmadda.xyzw ACC, $vf6, $vf21\n"
        "vmadda.xyzw ACC, $vf7, $vf23\n"
        "vmadd.xyzw $vf11, $vf8, $vf25\n"
        "vmula.xyzw ACC, $vf1, $vf12\n"
        "vmadda.xyzw ACC, $vf2, $vf14\n"
        "vmadda.xyzw ACC, $vf3, $vf16\n"
        "vmadda.xyzw ACC, $vf4, $vf18\n"
        "vmadda.xyzw ACC, $vf5, $vf20\n"
        "vmadda.xyzw ACC, $vf6, $vf22\n"
        "vmadda.xyzw ACC, $vf7, $vf24\n"
        "vmadd.xyzw $vf12, $vf8, $vf26\n"
        "qmfc2.ni   $7, $vf9\n"
        "qmfc2.ni   $8, $vf10\n"
        "qmfc2.ni   $9, $vf11\n"
        "qmfc2.ni   $10, $vf12\n"
        "pextlw     $11, $8, $7\n"
        "pextuw     $8, $8, $7\n"
        "pextlw     $12, $10, $9\n"
        "pextuw     $10, $10, $9\n"
        "pcpyld     $7, $12, $11\n"
        "pcpyud     $9, $11, $12\n"
        "pcpyld     $11, $10, $8\n"
        "pcpyud     $12, $8, $10\n"
        "sq         $7, 0x0(%3)\n"
        "addiu      %0, %0, -0x1\n"
        "sq         $9, 0x10(%3)\n"
        "addiu      %1, %1, 0x10\n"
        "sq         $11, 0x20(%3)\n"
        "addiu      %2, %2, 0x10\n"
        "sq         $12, 0x30(%3)\n"
        "bnez       %0, 1b\n"
        "addiu      %3, %3, 0x40\n"
        ".set reorder\n"
        : "+r"(i), "+r"(input), "+r"(weights), "+r"(out)
        :
        : "$7", "$8", "$9", "$10", "$11", "$12", "memory");
}

// Retail pads two nops after this function (the next EA object file's .text starts 16-byte aligned);
// tools/gen/hybrid_src.py keeps it as asm in the hybrid build, whose .s carries the pad.
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF800);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "fakematch.h"
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F800;

// PORT: PS2-only VU0 inline asm (lo: rows 0..3 to vf1..vf4 and rows 32..35 to vf9..vf12; hi = lo + 31: rows 28..31
// to vf5..vf8 and rows 60..63 to vf13..vf16).
static inline void vu0LoadButterfly_F800(const V4_F800 *lo, const V4_F800 *hi) {
    __asm__ volatile(
        "lqc2       $vf9, 0x200(%0)\n"
        "lqc2       $vf10, 0x210(%0)\n"
        "lqc2       $vf11, 0x220(%0)\n"
        "lqc2       $vf12, 0x230(%0)\n"
        "lqc2       $vf13, 0x1D0(%1)\n"
        "lqc2       $vf14, 0x1E0(%1)\n"
        "lqc2       $vf15, 0x1F0(%1)\n"
        "lqc2       $vf16, 0x200(%1)\n"
        "lqc2       $vf1, 0x0(%0)\n"
        "lqc2       $vf2, 0x10(%0)\n"
        "lqc2       $vf3, 0x20(%0)\n"
        "lqc2       $vf4, 0x30(%0)\n"
        "lqc2       $vf5, -0x30(%1)\n"
        "lqc2       $vf6, -0x20(%1)\n"
        "lqc2       $vf7, -0x10(%1)\n"
        "lqc2       $vf8, 0x0(%1)\n"
        : : "r"(lo), "r"(hi) : "memory");
}

// PORT: PS2-only VU0 inline asm (butterfly: sums and differences of the mirrored rows).
static inline void vu0Butterfly_F800(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf16, $vf9, $vf16\n"
        "vadd.xyzw  $vf15, $vf10, $vf15\n"
        "vadd.xyzw  $vf14, $vf11, $vf14\n"
        "vadd.xyzw  $vf13, $vf12, $vf13\n"
        "vsub.xyzw  $vf9, $vf1, $vf8\n"
        "vsub.xyzw  $vf10, $vf2, $vf7\n"
        "vsub.xyzw  $vf11, $vf3, $vf6\n"
        "vsub.xyzw  $vf12, $vf4, $vf5\n"
        "vsub.xyzw  $vf8, $vf8, $vf1\n"
        "vsub.xyzw  $vf7, $vf7, $vf2\n"
        "vsub.xyzw  $vf6, $vf6, $vf3\n"
        "vsub.xyzw  $vf5, $vf5, $vf4\n"
        "vsub.xyzw  $vf1, $vf9, $vf16\n"
        "vsub.xyzw  $vf2, $vf10, $vf15\n"
        "vsub.xyzw  $vf3, $vf11, $vf14\n"
        "vsub.xyzw  $vf4, $vf12, $vf13\n"
        "vsub.xyzw  $vf8, $vf8, $vf16\n"
        "vsub.xyzw  $vf7, $vf7, $vf15\n"
        "vsub.xyzw  $vf6, $vf6, $vf14\n"
        "vsub.xyzw  $vf5, $vf5, $vf13\n");
}

// PORT: PS2-only VU0 inline asm (store vf1..vf4 to the front rows, vf5 and vf6 to the back rows).
static inline void vu0StoreFront4Back2_F800(V4_F800 *lo, V4_F800 *hi) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf2, 0x10(%0)\n"
        "sqc2       $vf3, 0x20(%0)\n"
        "sqc2       $vf4, 0x30(%0)\n"
        "sqc2       $vf5, -0x30(%1)\n"
        "sqc2       $vf6, -0x20(%1)\n"
        : : "r"(lo), "r"(hi) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf7 to back row -1).
static inline void vu0StoreV7_F800(V4_F800 *hi) {
    __asm__ volatile("sqc2       $vf7, -0x10(%0)\n" : : "r"(hi) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf8 to back row 0).
static inline void vu0StoreV8_F800(V4_F800 *hi) {
    __asm__ volatile("sqc2       $vf8, 0x0(%0)\n" : : "r"(hi) : "memory");
}

// FAKEMATCH: FM_REG("$5") pin on the back pointer q (global-alloc gives it v0, the highest-priority free register,
// and the counter v1; retail keeps q in a1 and the counter in v0); plain form 98.17%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
/* In-place butterfly over 64 vec4 rows: front rows p[0..15], p[32..47] against mirrored back rows, four per pass. */
/**
 * Runs the first butterfly stage of the DTS encoder's subband transform in place on 64 vec4 rows.
 *
 * Four passes of four rows each: the front rows front[0..15] and front[32..47] are combined on VU0 with the mirrored
 * back rows front[31..16] and front[63..48] (sums go to the front rows, differences to the back rows). It is the first
 * step of the transform chain func_003BF800 .. func_003C01A0 that the analysis filterbank func_003BD220 runs on each
 * windowed block.
 *
 * @param front  the 64-row block (16-byte aligned quadwords)
 */
void func_003BF800(V4_F800 *front) {
    int i = 4;
    register V4_F800 *back FM_REG("$5") = front + 31;
top:
    vu0LoadButterfly_F800(front, back);
    vu0Butterfly_F800();
    vu0StoreFront4Back2_F800(front, back);
    i--;
    vu0StoreV7_F800(back);
    front += 4;
    vu0StoreV8_F800(back);
    back -= 4;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF8D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: retail pads this function's object with nops up to the next 16-byte boundary; that padding comes from the build
// (a file-boundary .align 4 after the body, or a pad segment in the seg_2B4578 split), not from code: nothing here emits it.
/* Types and VU0 helpers shared by the func_003BF8D0 .. func_003C01A0 family (EA audio engine transform
   routines). Kept in one unguarded block so that no function's block depends on another's (types and unused
   static inline helpers emit no code when the functions are built from asm). */
/* --- used by func_003BF8D0 and later family members --- */
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F8D0;

// PORT: PS2-only VU0 inline asm (clear running-sum register vf1).
static inline void vu0ClearSum(void) {
    __asm__ volatile("vsub.xyzw  $vf1, $vf0, $vf0\n");
}

// PORT: PS2-only VU0 inline asm (load eight vec4 rows into vf2..vf9).
static inline void vu0LoadRows2to9(const V4_F8D0 *p) {
    __asm__ volatile(
        "lqc2       $vf2, 0x0(%0)\n"
        "lqc2       $vf3, 0x10(%0)\n"
        "lqc2       $vf4, 0x20(%0)\n"
        "lqc2       $vf5, 0x30(%0)\n"
        "lqc2       $vf6, 0x40(%0)\n"
        "lqc2       $vf7, 0x50(%0)\n"
        "lqc2       $vf8, 0x60(%0)\n"
        "lqc2       $vf9, 0x70(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (add each row to its successor: vf1..vf8 += vf2..vf9).
static inline void vu0AddNext1to8(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf1, $vf1, $vf2\n"
        "vadd.xyzw  $vf2, $vf2, $vf3\n"
        "vadd.xyzw  $vf3, $vf3, $vf4\n"
        "vadd.xyzw  $vf4, $vf4, $vf5\n"
        "vadd.xyzw  $vf5, $vf5, $vf6\n"
        "vadd.xyzw  $vf6, $vf6, $vf7\n"
        "vadd.xyzw  $vf7, $vf7, $vf8\n"
        "vadd.xyzw  $vf8, $vf8, $vf9\n");
}

// PORT: PS2-only VU0 inline asm (store first six sums).
static inline void vu0StoreSums(V4_F8D0 *d) {
    __asm__ volatile(
        "sqc2       $vf2, 0x0(%0)\n"
        "sqc2       $vf4, 0x10(%0)\n"
        "sqc2       $vf6, 0x20(%0)\n"
        "sqc2       $vf8, 0x30(%0)\n"
        "sqc2       $vf1, 0x100(%0)\n"
        "sqc2       $vf3, 0x110(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store sum vf5).
static inline void vu0StoreSum5(V4_F8D0 *d) {
    __asm__ volatile("sqc2       $vf5, 0x120(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store sum vf7, carry last row vf9 into vf1).
static inline void vu0StoreSum7Carry(V4_F8D0 *d) {
    __asm__ volatile("sqc2       $vf7, 0x130(%0)\n" "vmove.xyzw $vf1, $vf9\n" : : "r"(d) : "memory");
}

/**
 * Transform stage of the DTS filterbank: running sums of adjacent rows.
 *
 * Four passes of eight vec4 rows on VU0, each row added to its successor with the last row carried into the next pass.
 * Called by func_003BD220.
 *
 * @param src  the input rows
 * @param dst  the output rows
 */
void func_003BF8D0(V4_F8D0 *src, V4_F8D0 *dst) {
    int i = 4;
    vu0ClearSum();
    do {
        vu0LoadRows2to9(src);
        vu0AddNext1to8();
        vu0StoreSums(dst);
        i--;
        vu0StoreSum5(dst);
        src += 8;
        vu0StoreSum7Carry(dst);
        dst += 4;
    } while (i != 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BF960);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_F960;

// PORT: PS2-only VU0 inline asm (load sixteen consecutive vec4 rows into vf1..vf16).
static inline void vu0LoadRows16(const V4_F960 *p) {
    __asm__ volatile(
        "lqc2       $vf1, 0x0(%0)\n"
        "lqc2       $vf2, 0x10(%0)\n"
        "lqc2       $vf3, 0x20(%0)\n"
        "lqc2       $vf4, 0x30(%0)\n"
        "lqc2       $vf5, 0x40(%0)\n"
        "lqc2       $vf6, 0x50(%0)\n"
        "lqc2       $vf7, 0x60(%0)\n"
        "lqc2       $vf8, 0x70(%0)\n"
        "lqc2       $vf9, 0x80(%0)\n"
        "lqc2       $vf10, 0x90(%0)\n"
        "lqc2       $vf11, 0xA0(%0)\n"
        "lqc2       $vf12, 0xB0(%0)\n"
        "lqc2       $vf13, 0xC0(%0)\n"
        "lqc2       $vf14, 0xD0(%0)\n"
        "lqc2       $vf15, 0xE0(%0)\n"
        "lqc2       $vf16, 0xF0(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store the first row unchanged to the middle of the output).
static inline void vu0StoreFirstRow(V4_F960 *d) {
    __asm__ volatile("sqc2       $vf1, 0x80(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (add each row to its successor: vf1..vf15 += vf2..vf16).
static inline void vu0AddNext1to15(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf1, $vf1, $vf2\n"
        "vadd.xyzw  $vf2, $vf2, $vf3\n"
        "vadd.xyzw  $vf3, $vf3, $vf4\n"
        "vadd.xyzw  $vf4, $vf4, $vf5\n"
        "vadd.xyzw  $vf5, $vf5, $vf6\n"
        "vadd.xyzw  $vf6, $vf6, $vf7\n"
        "vadd.xyzw  $vf7, $vf7, $vf8\n"
        "vadd.xyzw  $vf8, $vf8, $vf9\n"
        "vadd.xyzw  $vf9, $vf9, $vf10\n"
        "vadd.xyzw  $vf10, $vf10, $vf11\n"
        "vadd.xyzw  $vf11, $vf11, $vf12\n"
        "vadd.xyzw  $vf12, $vf12, $vf13\n"
        "vadd.xyzw  $vf13, $vf13, $vf14\n"
        "vadd.xyzw  $vf14, $vf14, $vf15\n"
        "vadd.xyzw  $vf15, $vf15, $vf16\n");
}

// PORT: PS2-only VU0 inline asm (store the odd-numbered sums to the low half of the output).
static inline void vu0StoreOddSums(V4_F960 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf3, 0x10(%0)\n"
        "sqc2       $vf5, 0x20(%0)\n"
        "sqc2       $vf7, 0x30(%0)\n"
        "sqc2       $vf9, 0x40(%0)\n"
        "sqc2       $vf11, 0x50(%0)\n"
        "sqc2       $vf13, 0x60(%0)\n"
        "sqc2       $vf15, 0x70(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store the even-numbered sums to the upper half of the output).
static inline void vu0StoreEvenSums(V4_F960 *d) {
    __asm__ volatile(
        "sqc2       $vf2, 0x90(%0)\n"
        "sqc2       $vf4, 0xA0(%0)\n"
        "sqc2       $vf6, 0xB0(%0)\n"
        "sqc2       $vf8, 0xC0(%0)\n"
        "sqc2       $vf10, 0xD0(%0)\n"
        "sqc2       $vf12, 0xE0(%0)\n"
        "sqc2       $vf14, 0xF0(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (load every other row of the second block, vf1..vf8).
static inline void vu0LoadEvenRows(const V4_F960 *p) {
    __asm__ volatile(
        "lqc2       $vf1, 0x100(%0)\n"
        "lqc2       $vf2, 0x120(%0)\n"
        "lqc2       $vf3, 0x140(%0)\n"
        "lqc2       $vf4, 0x160(%0)\n"
        "lqc2       $vf5, 0x180(%0)\n"
        "lqc2       $vf6, 0x1A0(%0)\n"
        "lqc2       $vf7, 0x1C0(%0)\n"
        "lqc2       $vf8, 0x1E0(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (copy vf1..vf8 to eight consecutive output rows).
static inline void vu0StoreCopy8(V4_F960 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x100(%0)\n"
        "sqc2       $vf2, 0x110(%0)\n"
        "sqc2       $vf3, 0x120(%0)\n"
        "sqc2       $vf4, 0x130(%0)\n"
        "sqc2       $vf5, 0x140(%0)\n"
        "sqc2       $vf6, 0x150(%0)\n"
        "sqc2       $vf7, 0x160(%0)\n"
        "sqc2       $vf8, 0x170(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (load the other interleaved rows of the second block, vf1..vf8).
static inline void vu0LoadOddRows(const V4_F960 *p) {
    __asm__ volatile(
        "lqc2       $vf1, 0x110(%0)\n"
        "lqc2       $vf2, 0x130(%0)\n"
        "lqc2       $vf3, 0x150(%0)\n"
        "lqc2       $vf4, 0x170(%0)\n"
        "lqc2       $vf5, 0x190(%0)\n"
        "lqc2       $vf6, 0x1B0(%0)\n"
        "lqc2       $vf7, 0x1D0(%0)\n"
        "lqc2       $vf8, 0x1F0(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store the first row unchanged).
static inline void vu0StoreFirstRow2(V4_F960 *d) {
    __asm__ volatile("sqc2       $vf1, 0x180(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (add each row to its successor: vf1..vf7 += vf2..vf8).
static inline void vu0AddNext1to7(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf1, $vf1, $vf2\n"
        "vadd.xyzw  $vf2, $vf2, $vf3\n"
        "vadd.xyzw  $vf3, $vf3, $vf4\n"
        "vadd.xyzw  $vf4, $vf4, $vf5\n"
        "vadd.xyzw  $vf5, $vf5, $vf6\n"
        "vadd.xyzw  $vf6, $vf6, $vf7\n"
        "vadd.xyzw  $vf7, $vf7, $vf8\n");
}

// PORT: PS2-only VU0 inline asm (store sums vf1..vf7; trailing label keeps gas from filling a delay slot).
static inline void vu0StoreSums7(V4_F960 *d) {
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2       $vf1, 0x190(%0)\n"
        "sqc2       $vf2, 0x1A0(%0)\n"
        "sqc2       $vf3, 0x1B0(%0)\n"
        "sqc2       $vf4, 0x1C0(%0)\n"
        "sqc2       $vf5, 0x1D0(%0)\n"
        "sqc2       $vf6, 0x1E0(%0)\n"
        "sqc2       $vf7, 0x1F0(%0)\n"
        ".set reorder\n"
        : : "r"(d) : "memory");
}

/**
 * Transform stage of the DTS filterbank: sums of adjacent rows reordered into even and odd halves.
 *
 * On VU0: sixteen rows are loaded, the first stored as is and the pairwise sums split into the low and upper halves of
 * the output; the interleaved rows of the second block are copied and summed the same way. Called by func_003BD220.
 *
 * @param src  the input rows
 * @param dst  the output rows
 */
void func_003BF960(V4_F960 *src, V4_F960 *dst) {
    vu0LoadRows16(src);
    vu0StoreFirstRow(dst);
    vu0AddNext1to15();
    vu0StoreOddSums(dst);
    vu0StoreEvenSums(dst);
    vu0LoadEvenRows(src);
    vu0StoreCopy8(dst);
    vu0LoadOddRows(src);
    vu0StoreFirstRow2(dst);
    vu0AddNext1to7();
    vu0StoreSums7(dst);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFAC0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FAC0;

// PORT: PS2-only VU0 inline asm (add each row to its successor: vf1..vf7 += vf2..vf8).

// PORT: PS2-only VU0 inline asm (load eight vec4 rows into vf1..vf8).
static inline void vu0LoadRows1to8(const void *p) {
    __asm__ volatile(
        "lqc2       $vf1, 0x0(%0)\n"
        "lqc2       $vf2, 0x10(%0)\n"
        "lqc2       $vf3, 0x20(%0)\n"
        "lqc2       $vf4, 0x30(%0)\n"
        "lqc2       $vf5, 0x40(%0)\n"
        "lqc2       $vf6, 0x50(%0)\n"
        "lqc2       $vf7, 0x60(%0)\n"
        "lqc2       $vf8, 0x70(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store row vf1 unchanged).
static inline void vu0StoreRow1At40(V4_FAC0 *d) {
    __asm__ volatile("sqc2       $vf1, 0x40(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store sums vf1, vf3, vf5, vf7 and vf2).
static inline void vu0StoreHeadSums(V4_FAC0 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf3, 0x10(%0)\n"
        "sqc2       $vf5, 0x20(%0)\n"
        "sqc2       $vf7, 0x30(%0)\n"
        "sqc2       $vf2, 0x50(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf4 to row 6).
static inline void vu0StoreV4At60(void *d) {
    __asm__ volatile("sqc2       $vf4, 0x60(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf6 to row 7).
static inline void vu0StoreV6At70(void *d) {
    __asm__ volatile("sqc2       $vf6, 0x70(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf1, vf3, vf5, vf7 to rows 0..3).
static inline void vu0StoreOddRows(V4_FAC0 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf3, 0x10(%0)\n"
        "sqc2       $vf5, 0x20(%0)\n"
        "sqc2       $vf7, 0x30(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (pairwise sums of even rows: vf3 = vf2+vf4, vf4 = vf4+vf6, vf5 = vf6+vf8).
static inline void vu0AddEvenPairs(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf3, $vf2, $vf4\n"
        "vadd.xyzw  $vf4, $vf4, $vf6\n"
        "vadd.xyzw  $vf5, $vf6, $vf8\n");
}

// PORT: PS2-only VU0 inline asm (store vf2 and vf3 to rows 4 and 5).
static inline void vu0StoreV2V3(V4_FAC0 *d) {
    __asm__ volatile(
        "sqc2       $vf2, 0x40(%0)\n"
        "sqc2       $vf3, 0x50(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf5 to row 7).
static inline void vu0StoreV5At70(V4_FAC0 *d) {
    __asm__ volatile("sqc2       $vf5, 0x70(%0)\n" : : "r"(d) : "memory");
}

/**
 * Transform stage of the DTS filterbank: adjacent and even-row pairwise sums over four blocks of eight rows.
 *
 * VU0 code; the first block stores its row 1 and the running sums, the next three the odd rows and the sums of even-row
 * pairs. Called by func_003BD220.
 *
 * @param src  the input rows
 * @param dst  the output rows
 */
void func_003BFAC0(V4_FAC0 *src, V4_FAC0 *dst) {
    int i;
    vu0LoadRows1to8(src);
    vu0StoreRow1At40(dst);
    vu0AddNext1to7();
    vu0StoreHeadSums(dst);
    i = 3;
    vu0StoreV4At60(dst);
    src += 8;
    vu0StoreV6At70(dst);
    dst += 8;
    do {
        vu0LoadRows1to8(src);
        vu0StoreOddRows(dst);
        vu0AddEvenPairs();
        vu0StoreV2V3(dst);
        i--;
        vu0StoreV4At60(dst);
        src += 8;
        vu0StoreV5At70(dst);
        dst += 8;
    } while (i != 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFB90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FB90;

// PORT: PS2-only VU0 inline asm (load eight vec4 rows into vf1..vf8).
// PORT: PS2-only VU0 inline asm (first-block sums: vf9 = vf1+vf2, vf10 = vf3+vf4, vf11 = vf2+vf3, vf12 = vf6+vf8).
static inline void vu0AddHeadSums(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf1, $vf2\n"
        "vadd.xyzw  $vf10, $vf3, $vf4\n"
        "vadd.xyzw  $vf11, $vf2, $vf3\n"
        "vadd.xyzw  $vf12, $vf6, $vf8\n");
}

// PORT: PS2-only VU0 inline asm (store first-block results to rows 0..5).
static inline void vu0StoreHeadRows(V4_FB90 *d) {
    __asm__ volatile(
        "sqc2       $vf9, 0x0(%0)\n"
        "sqc2       $vf10, 0x10(%0)\n"
        "sqc2       $vf1, 0x20(%0)\n"
        "sqc2       $vf11, 0x30(%0)\n"
        "sqc2       $vf5, 0x40(%0)\n"
        "sqc2       $vf7, 0x50(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf6 to row 6).
static inline void vu0StoreV6At60(V4_FB90 *d) {
    __asm__ volatile("sqc2       $vf6, 0x60(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf12 to row 7).
static inline void vu0StoreV12At70(V4_FB90 *d) {
    __asm__ volatile("sqc2       $vf12, 0x70(%0)\n" : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (loop sums: vf9 = vf2+vf4, vf10 = vf6+vf8).
static inline void vu0AddLoopSums(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf2, $vf4\n"
        "vadd.xyzw  $vf10, $vf6, $vf8\n");
}

// PORT: PS2-only VU0 inline asm (store loop results to rows 0..5).
static inline void vu0StoreLoopRows(V4_FB90 *d) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf3, 0x10(%0)\n"
        "sqc2       $vf2, 0x20(%0)\n"
        "sqc2       $vf9, 0x30(%0)\n"
        "sqc2       $vf5, 0x40(%0)\n"
        "sqc2       $vf7, 0x50(%0)\n"
        : : "r"(d) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf10 to row 7).
static inline void vu0StoreV10At70(V4_FB90 *d) {
    __asm__ volatile("sqc2       $vf10, 0x70(%0)\n" : : "r"(d) : "memory");
}

/**
 * Transform stage of the DTS filterbank: pairwise row sums over four blocks of eight rows.
 *
 * VU0 code; the first block forms vf1+vf2, vf3+vf4, vf2+vf3 and vf6+vf8, the next three vf2+vf4 and vf6+vf8, other rows
 * passing through. Called by func_003BD220.
 *
 * @param src  the input rows
 * @param dst  the output rows
 */
void func_003BFB90(V4_FB90 *src, V4_FB90 *dst) {
    int i;
    vu0LoadRows1to8(src);
    vu0AddHeadSums();
    vu0StoreHeadRows(dst);
    i = 3;
    vu0StoreV6At60(dst);
    src += 8;
    vu0StoreV12At70(dst);
    dst += 8;
top:
    vu0LoadRows1to8(src);
    vu0AddLoopSums();
    vu0StoreLoopRows(dst);
    i--;
    vu0StoreV6At60(dst);
    src += 8;
    vu0StoreV10At70(dst);
    dst += 8;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFC50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FC50;

// PORT: PS2-only VU0 inline asm (load eight vec4 rows into vf1..vf8).
// PORT: PS2-only VU0 inline asm (load the twiddle vector into vf16).
static inline void vu0LoadTwiddle(const V4_FC50 *p) {
    __asm__ volatile("lqc2       $vf16, 0x0(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (head: vf9 = (vf1+vf2)*w.x, vf10/vf11/vf12 = vf4/vf6/vf8 * w.x).
static inline void vu0ScaleHead(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf1, $vf2\n"
        "vmulx.xyzw $vf10, $vf4, $vf16x\n"
        "vmulx.xyzw $vf11, $vf6, $vf16x\n"
        "vmulx.xyzw $vf12, $vf8, $vf16x\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n");
}

// PORT: PS2-only VU0 inline asm (head butterfly: sub/add of rows against the scaled terms).
static inline void vu0ButterflyHead(void) {
    __asm__ volatile(
        "vsub.xyzw  $vf4, $vf3, $vf10\n"
        "vadd.xyzw  $vf3, $vf3, $vf10\n"
        "vsub.xyzw  $vf6, $vf5, $vf11\n"
        "vadd.xyzw  $vf5, $vf5, $vf11\n"
        "vsub.xyzw  $vf2, $vf1, $vf9\n"
        "vadd.xyzw  $vf1, $vf1, $vf9\n"
        "vsub.xyzw  $vf8, $vf7, $vf12\n"
        "vadd.xyzw  $vf7, $vf7, $vf12\n");
}

// PORT: PS2-only VU0 inline asm (scale vf2 by w.y and vf1 by w.z).
static inline void vu0ScaleY2Z1(void) {
    __asm__ volatile(
        "vmuly.xyzw $vf2, $vf2, $vf16y\n"
        "vmulz.xyzw $vf1, $vf1, $vf16z\n");
}

// PORT: PS2-only VU0 inline asm (store head results in place, vf3..vf8 then vf1).
static inline void vu0StoreHeadInPlace(void *p) {
    __asm__ volatile(
        "sqc2       $vf3, 0x20(%0)\n"
        "sqc2       $vf4, 0x30(%0)\n"
        "sqc2       $vf5, 0x40(%0)\n"
        "sqc2       $vf6, 0x50(%0)\n"
        "sqc2       $vf7, 0x60(%0)\n"
        "sqc2       $vf8, 0x70(%0)\n"
        "sqc2       $vf1, 0x0(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf2 to row 1).
static inline void vu0StoreV2At10(void *p) {
    __asm__ volatile("sqc2       $vf2, 0x10(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (scale even rows vf2/vf4/vf6/vf8 by w.x).
static inline void vu0ScaleEvenRows(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf9, $vf2, $vf16x\n"
        "vmulx.xyzw $vf10, $vf4, $vf16x\n"
        "vmulx.xyzw $vf11, $vf6, $vf16x\n"
        "vmulx.xyzw $vf12, $vf8, $vf16x\n");
}

// PORT: PS2-only VU0 inline asm (butterfly of row pairs against the scaled terms).
static inline void vu0ButterflyPairs(void) {
    __asm__ volatile(
        "vsub.xyzw  $vf2, $vf1, $vf9\n"
        "vadd.xyzw  $vf1, $vf1, $vf9\n"
        "vsub.xyzw  $vf4, $vf3, $vf10\n"
        "vadd.xyzw  $vf3, $vf3, $vf10\n"
        "vsub.xyzw  $vf6, $vf5, $vf11\n"
        "vadd.xyzw  $vf5, $vf5, $vf11\n"
        "vsub.xyzw  $vf8, $vf7, $vf12\n"
        "vadd.xyzw  $vf7, $vf7, $vf12\n");
}

// PORT: PS2-only VU0 inline asm (store butterfly rows vf1..vf7 in place).
static inline void vu0StoreRows1to7(void *p) {
    __asm__ volatile(
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf2, 0x10(%0)\n"
        "sqc2       $vf3, 0x20(%0)\n"
        "sqc2       $vf4, 0x30(%0)\n"
        "sqc2       $vf5, 0x40(%0)\n"
        "sqc2       $vf6, 0x50(%0)\n"
        "sqc2       $vf7, 0x60(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf8 to row 7).
static inline void vu0StoreV8At70(void *p) {
    __asm__ volatile("sqc2       $vf8, 0x70(%0)\n" : : "r"(p) : "memory");
}

/**
 * Transform stage of the DTS filterbank: twiddle-scaled butterflies done in place over four blocks of eight rows.
 *
 * Loads the twiddle vector from @p dst, scales the even rows (and the first block's head terms) by its lanes and stores
 * the sums and differences back over @p src. Called by func_003BD220 with the table +0xA28 of the work area.
 *
 * @param src  the rows, transformed in place
 * @param dst  the twiddle-factor table (misnamed: nothing is stored there)
 */
void func_003BFC50(V4_FC50 *src, V4_FC50 *dst) {
    int i;
    vu0LoadTwiddle(dst);
    vu0LoadRows1to8(src);
    vu0ScaleHead();
    vu0ButterflyHead();
    vu0ScaleY2Z1();
    vu0StoreHeadInPlace(src);
    i = 3;
    vu0StoreV2At10(src);
    src += 8;
top:
    vu0LoadRows1to8(src);
    vu0ScaleEvenRows();
    vu0ButterflyPairs();
    vu0StoreRows1to7(src);
    i--;
    vu0StoreV8At70(src);
    src += 8;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFD60);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE: retail pads this function's object with nops up to the next 16-byte boundary; that padding comes from the build
// (a file-boundary .align 4 after the body, or a pad segment in the seg_2B4578 split), not from code: nothing here emits it.
/* --- used by func_003BFD60 and later family members --- */
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FD60_FD60;

// PORT: PS2-only VU0 inline asm (load two twiddle vectors into vf15 and vf16).
static inline void vu0LoadTwiddles15and16_FD60(const V4_FD60_FD60 *p) {
    __asm__ volatile(
        "lqc2       $vf15, 0x0(%0)\n"
        "lqc2       $vf16, 0x10(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (head sums and differences of rows 1..4).
static inline void vu0AddSubRows1to4Head(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf1, $vf3\n"
        "vadd.xyzw  $vf10, $vf2, $vf4\n"
        "vsub.xyzw  $vf11, $vf4, $vf2\n"
        "vsub.xyzw  $vf12, $vf3, $vf1\n");
}

// PORT: PS2-only VU0 inline asm (scale vf9..vf12 per lane by vf15 and vf7/vf8 by vf16.x/y).
static inline void vu0ScaleHeadLanes(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf9, $vf9, $vf15x\n"
        "vmuly.xyzw $vf10, $vf10, $vf15y\n"
        "vmulz.xyzw $vf11, $vf11, $vf15z\n"
        "vmulw.xyzw $vf12, $vf12, $vf15w\n"
        "vmulx.xyzw $vf7, $vf7, $vf16x\n"
        "vmuly.xyzw $vf8, $vf8, $vf16y\n");
}

// PORT: PS2-only VU0 inline asm (store vf9..vf12 to rows 0..3).
static inline void vu0StoreResults4_FD60(void *p) {
    __asm__ volatile(
        "sqc2       $vf9, 0x0(%0)\n"
        "sqc2       $vf10, 0x10(%0)\n"
        "sqc2       $vf11, 0x20(%0)\n"
        "sqc2       $vf12, 0x30(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (sums and differences of rows 5..8).
static inline void vu0AddSubRows5to8(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf5, $vf7\n"
        "vadd.xyzw  $vf10, $vf6, $vf8\n"
        "vsub.xyzw  $vf11, $vf6, $vf8\n"
        "vsub.xyzw  $vf12, $vf5, $vf7\n");
}

// PORT: PS2-only VU0 inline asm (store vf9 and vf10 to rows 4 and 5).
static inline void vu0StoreResults5and6_FD60(void *p) {
    __asm__ volatile(
        "sqc2       $vf9, 0x40(%0)\n"
        "sqc2       $vf10, 0x50(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf11 to row 6).
static inline void vu0StoreV11At60_FD60(void *p) {
    __asm__ volatile("sqc2       $vf11, 0x60(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf12 to row 7).
static inline void vu0StoreV12At70Of_FD60(void *p) {
    __asm__ volatile("sqc2       $vf12, 0x70(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (loop: scale rows 3, 4, 7, 8 by vf16.x/y).
static inline void vu0ScaleLoopRows(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf3, $vf3, $vf16x\n"
        "vmuly.xyzw $vf4, $vf4, $vf16y\n"
        "vmulx.xyzw $vf7, $vf7, $vf16x\n"
        "vmuly.xyzw $vf8, $vf8, $vf16y\n");
}

// PORT: PS2-only VU0 inline asm (loop sums and differences of rows 1..4).
static inline void vu0AddSubRows1to4Loop(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf1, $vf3\n"
        "vadd.xyzw  $vf10, $vf2, $vf4\n"
        "vsub.xyzw  $vf11, $vf2, $vf4\n"
        "vsub.xyzw  $vf12, $vf1, $vf3\n");
}

/**
 * Transform stage of the DTS filterbank: sums and differences of row halves scaled by two twiddle vectors.
 *
 * Loads the twiddles from @p dst into vf15/vf16 and, for four blocks of eight rows, writes the scaled butterflies to
 * @p output. Called by func_003BD220 with the table +0xA2C of the work area.
 *
 * @param src  the input rows
 * @param dst  the twiddle-factor table (misnamed: it is only read)
 * @param output  the output rows
 */
void func_003BFD60(V4_FD60_FD60 *src, V4_FD60_FD60 *dst, V4_FD60_FD60 *output) {
    int i;
    vu0LoadTwiddles15and16_FD60(dst);
    vu0LoadRows1to8(src);
    vu0AddSubRows1to4Head();
    vu0ScaleHeadLanes();
    vu0StoreResults4_FD60(output);
    vu0AddSubRows5to8();
    vu0StoreResults5and6_FD60(output);
    i = 3;
    vu0StoreV11At60_FD60(output);
    src += 8;
    vu0StoreV12At70Of_FD60(output);
    output += 8;
top:
    vu0LoadRows1to8(src);
    vu0ScaleLoopRows();
    vu0AddSubRows1to4Loop();
    vu0StoreResults4_FD60(output);
    vu0AddSubRows5to8();
    vu0StoreResults5and6_FD60(output);
    i--;
    vu0StoreV11At60_FD60(output);
    src += 8;
    vu0StoreV12At70Of_FD60(output);
    output += 8;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFE80);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (load eight vec4 rows into vf1..vf8).
/* --- used by func_003BFD60 and later family members --- */
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FD60;

// PORT: PS2-only VU0 inline asm (load two twiddle vectors into vf15 and vf16).
static inline void vu0LoadTwiddles15and16(const V4_FD60 *p) {
    __asm__ volatile(
        "lqc2       $vf15, 0x0(%0)\n"
        "lqc2       $vf16, 0x10(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf9..vf12 to rows 0..3).
static inline void vu0StoreResults4(void *p) {
    __asm__ volatile(
        "sqc2       $vf9, 0x0(%0)\n"
        "sqc2       $vf10, 0x10(%0)\n"
        "sqc2       $vf11, 0x20(%0)\n"
        "sqc2       $vf12, 0x30(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf9 and vf10 to rows 4 and 5).
static inline void vu0StoreResults5and6(void *p) {
    __asm__ volatile(
        "sqc2       $vf9, 0x40(%0)\n"
        "sqc2       $vf10, 0x50(%0)\n"
        : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf11 to row 6).
static inline void vu0StoreV11At60(void *p) {
    __asm__ volatile("sqc2       $vf11, 0x60(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (store vf12 to row 7).
static inline void vu0StoreV12At70Of(void *p) {
    __asm__ volatile("sqc2       $vf12, 0x70(%0)\n" : : "r"(p) : "memory");
}

/* --- used by func_003BFE80 and later family members --- */
typedef struct { int w[4]; } __attribute__((aligned(16))) V4_FE80;

// PORT: PS2-only VU0 inline asm (add the upper four rows to the lower four: vf9..vf12 = vf1..vf4 + vf5..vf8).
static inline void vu0AddRowHalves(void) {
    __asm__ volatile(
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n");
}

// PORT: PS2-only VU0 inline asm (scale vf9..vf12 per lane by vf15).
static inline void vu0ScaleResultsBy15(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf9, $vf9, $vf15x\n"
        "vmuly.xyzw $vf10, $vf10, $vf15y\n"
        "vmulz.xyzw $vf11, $vf11, $vf15z\n"
        "vmulw.xyzw $vf12, $vf12, $vf15w\n");
}

// PORT: PS2-only VU0 inline asm (subtract lower four rows from the upper four, reversed order).
static inline void vu0SubRowHalvesRev(void) {
    __asm__ volatile(
        "vsub.xyzw  $vf9, $vf8, $vf4\n"
        "vsub.xyzw  $vf10, $vf7, $vf3\n"
        "vsub.xyzw  $vf11, $vf6, $vf2\n"
        "vsub.xyzw  $vf12, $vf5, $vf1\n");
}

// PORT: PS2-only VU0 inline asm (scale vf9..vf12 per lane by vf16).
static inline void vu0ScaleResultsBy16(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n");
}

// PORT: PS2-only VU0 inline asm (load the third twiddle vector into vf16).
static inline void vu0LoadTwiddle16At20(const V4_FE80 *p) {
    __asm__ volatile("lqc2       $vf16, 0x20(%0)\n" : : "r"(p) : "memory");
}

// PORT: PS2-only VU0 inline asm (loop: scale rows 5..8 per lane by vf16).
static inline void vu0ScaleRows5to8By16(void) {
    __asm__ volatile(
        "vmulx.xyzw $vf5, $vf5, $vf16x\n"
        "vmuly.xyzw $vf6, $vf6, $vf16y\n"
        "vmulz.xyzw $vf7, $vf7, $vf16z\n"
        "vmulw.xyzw $vf8, $vf8, $vf16w\n");
}

// PORT: PS2-only VU0 inline asm (loop: subtract scaled upper rows from lower rows).
static inline void vu0SubRowHalves(void) {
    __asm__ volatile(
        "vsub.xyzw  $vf9, $vf4, $vf8\n"
        "vsub.xyzw  $vf10, $vf3, $vf7\n"
        "vsub.xyzw  $vf11, $vf2, $vf6\n"
        "vsub.xyzw  $vf12, $vf1, $vf5\n");
}

/**
 * Transform stage of the DTS filterbank: butterflies of upper and lower row halves scaled per lane.
 *
 * Loads twiddles from @p dst (vf15, vf16, then the third vector at +0x20 for the later blocks) and writes four blocks
 * of eight rows to @p output. Called by func_003BD220 with the table +0xA30 of the work area.
 *
 * @param src  the input rows
 * @param dst  the twiddle-factor table (misnamed: it is only read)
 * @param output  the output rows
 */
void func_003BFE80(V4_FE80 *src, V4_FE80 *dst, V4_FE80 *output) {
    int i;
    vu0LoadTwiddles15and16(dst);
    vu0LoadRows1to8(src);
    vu0AddRowHalves();
    vu0ScaleResultsBy15();
    vu0StoreResults4(output);
    vu0SubRowHalvesRev();
    vu0ScaleResultsBy16();
    vu0StoreResults5and6(output);
    i = 3;
    vu0StoreV11At60(output);
    src += 8;
    vu0StoreV12At70Of(output);
    output += 8;
    vu0LoadTwiddle16At20(dst);
top:
    vu0LoadRows1to8(src);
    vu0ScaleRows5to8By16();
    vu0AddRowHalves();
    vu0StoreResults4(output);
    vu0SubRowHalves();
    vu0StoreResults5and6(output);
    i--;
    vu0StoreV11At60(output);
    src += 8;
    vu0StoreV12At70Of(output);
    output += 8;
    if (i != 0) goto top;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003BFFA0);
#ifdef SKIP_ASM
/**
 * Transform stage of the DTS filterbank: butterfly add/sub of matrix rows, scaled per lane (VU0 asm).
 *
 * Called by func_003BD220 with the table +0xA34 of the work area.
 *
 * @param in  the input rows
 * @param scale  the per-lane scale table
 * @param out  the output rows
 */
// PORT: PS2-only VU0 inline asm (butterfly add/sub of matrix rows, scaled per lane).
void func_003BFFA0(void *in, void *scale, void *out) {
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "lqc2       $vf15, 0x0(%1)\n"
        "lqc2       $vf16, 0x30(%1)\n"
        "lqc2       $vf1, 0x0(%0)\n"
        "lqc2       $vf2, 0x10(%0)\n"
        "lqc2       $vf3, 0x20(%0)\n"
        "lqc2       $vf4, 0x30(%0)\n"
        "lqc2       $vf5, 0x80(%0)\n"
        "lqc2       $vf6, 0x90(%0)\n"
        "lqc2       $vf7, 0xA0(%0)\n"
        "lqc2       $vf8, 0xB0(%0)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf15x\n"
        "vmuly.xyzw $vf10, $vf10, $vf15y\n"
        "vmulz.xyzw $vf11, $vf11, $vf15z\n"
        "vmulw.xyzw $vf12, $vf12, $vf15w\n"
        "sqc2       $vf9, 0x0(%2)\n"
        "sqc2       $vf10, 0x10(%2)\n"
        "sqc2       $vf11, 0x20(%2)\n"
        "sqc2       $vf12, 0x30(%2)\n"
        "vsub.xyzw  $vf9, $vf8, $vf4\n"
        "vsub.xyzw  $vf10, $vf7, $vf3\n"
        "vsub.xyzw  $vf11, $vf6, $vf2\n"
        "vsub.xyzw  $vf12, $vf5, $vf1\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0xC0(%2)\n"
        "sqc2       $vf10, 0xD0(%2)\n"
        "sqc2       $vf11, 0xE0(%2)\n"
        "sqc2       $vf12, 0xF0(%2)\n"
        "lqc2       $vf15, 0x10(%1)\n"
        "lqc2       $vf16, 0x20(%1)\n"
        "lqc2       $vf1, 0x40(%0)\n"
        "lqc2       $vf2, 0x50(%0)\n"
        "lqc2       $vf3, 0x60(%0)\n"
        "lqc2       $vf4, 0x70(%0)\n"
        "lqc2       $vf5, 0xC0(%0)\n"
        "lqc2       $vf6, 0xD0(%0)\n"
        "lqc2       $vf7, 0xE0(%0)\n"
        "lqc2       $vf8, 0xF0(%0)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf15x\n"
        "vmuly.xyzw $vf10, $vf10, $vf15y\n"
        "vmulz.xyzw $vf11, $vf11, $vf15z\n"
        "vmulw.xyzw $vf12, $vf12, $vf15w\n"
        "sqc2       $vf9, 0x40(%2)\n"
        "sqc2       $vf10, 0x50(%2)\n"
        "sqc2       $vf11, 0x60(%2)\n"
        "sqc2       $vf12, 0x70(%2)\n"
        "vsub.xyzw  $vf9, $vf8, $vf4\n"
        "vsub.xyzw  $vf10, $vf7, $vf3\n"
        "vsub.xyzw  $vf11, $vf6, $vf2\n"
        "vsub.xyzw  $vf12, $vf5, $vf1\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x80(%2)\n"
        "sqc2       $vf10, 0x90(%2)\n"
        "sqc2       $vf11, 0xA0(%2)\n"
        "sqc2       $vf12, 0xB0(%2)\n"
        "lqc2       $vf1, 0x40(%1)\n"
        "lqc2       $vf2, 0x50(%1)\n"
        "lqc2       $vf9, 0x180(%0)\n"
        "lqc2       $vf10, 0x190(%0)\n"
        "lqc2       $vf11, 0x1A0(%0)\n"
        "lqc2       $vf12, 0x1B0(%0)\n"
        "lqc2       $vf13, 0x1C0(%0)\n"
        "lqc2       $vf14, 0x1D0(%0)\n"
        "lqc2       $vf15, 0x1E0(%0)\n"
        "lqc2       $vf16, 0x1F0(%0)\n"
        "vmulx.xyzw $vf9, $vf9, $vf1x\n"
        "vmuly.xyzw $vf10, $vf10, $vf1y\n"
        "vmulz.xyzw $vf11, $vf11, $vf1z\n"
        "vmulw.xyzw $vf12, $vf12, $vf1w\n"
        "vmulx.xyzw $vf13, $vf13, $vf2x\n"
        "vmuly.xyzw $vf14, $vf14, $vf2y\n"
        "vmulz.xyzw $vf15, $vf15, $vf2z\n"
        "vmulw.xyzw $vf16, $vf16, $vf2w\n"
        "lqc2       $vf4, 0x130(%0)\n"
        "lqc2       $vf3, 0x120(%0)\n"
        "lqc2       $vf2, 0x110(%0)\n"
        "lqc2       $vf1, 0x100(%0)\n"
        "vsub.xyzw  $vf5, $vf4, $vf12\n"
        "vsub.xyzw  $vf6, $vf3, $vf11\n"
        "vsub.xyzw  $vf7, $vf2, $vf10\n"
        "vsub.xyzw  $vf8, $vf1, $vf9\n"
        "vadd.xyzw  $vf1, $vf1, $vf9\n"
        "vadd.xyzw  $vf2, $vf2, $vf10\n"
        "vadd.xyzw  $vf3, $vf3, $vf11\n"
        "vadd.xyzw  $vf4, $vf4, $vf12\n"
        "sqc2       $vf1, 0x100(%2)\n"
        "sqc2       $vf2, 0x110(%2)\n"
        "sqc2       $vf3, 0x120(%2)\n"
        "sqc2       $vf4, 0x130(%2)\n"
        "sqc2       $vf5, 0x1C0(%2)\n"
        "sqc2       $vf6, 0x1D0(%2)\n"
        "sqc2       $vf7, 0x1E0(%2)\n"
        "sqc2       $vf8, 0x1F0(%2)\n"
        "lqc2       $vf4, 0x170(%0)\n"
        "lqc2       $vf3, 0x160(%0)\n"
        "lqc2       $vf2, 0x150(%0)\n"
        "lqc2       $vf1, 0x140(%0)\n"
        "vsub.xyzw  $vf5, $vf4, $vf16\n"
        "vsub.xyzw  $vf6, $vf3, $vf15\n"
        "vsub.xyzw  $vf7, $vf2, $vf14\n"
        "vsub.xyzw  $vf8, $vf1, $vf13\n"
        "vadd.xyzw  $vf1, $vf1, $vf13\n"
        "vadd.xyzw  $vf2, $vf2, $vf14\n"
        "vadd.xyzw  $vf3, $vf3, $vf15\n"
        "vadd.xyzw  $vf4, $vf4, $vf16\n"
        "sqc2       $vf1, 0x140(%2)\n"
        "sqc2       $vf2, 0x150(%2)\n"
        "sqc2       $vf3, 0x160(%2)\n"
        "sqc2       $vf4, 0x170(%2)\n"
        "sqc2       $vf5, 0x180(%2)\n"
        "sqc2       $vf6, 0x190(%2)\n"
        "sqc2       $vf7, 0x1A0(%2)\n"
        "sqc2       $vf8, 0x1B0(%2)\n"
        ".set pop\n"
        :
        : "r"(in), "r"(scale), "r"(out)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C01A0);
#ifdef SKIP_ASM
/**
 * Last stage of the DTS filterbank: scaled sums and differences of 8 vec4 pairs, stored with a stride (VU0 asm).
 *
 * Its strided stores deliver each block's band samples into the per-band array; func_003BD220 passes the table +0xA38
 * of the work area and a stride of 0x40.
 *
 * @param in  the input rows
 * @param scale  the per-lane scale table
 * @param out  where the band samples go
 * @param stride  the output stride
 */
// PORT: PS2-only VU0 inline asm (8 vec4 pairs: sums and differences scaled per lane, strided stores).
void func_003C01A0(void *in, void *scale, void *out, int stride) {
    __asm__ volatile(
        "lqc2       $vf16, 0x0(%2)\n"
        "lqc2       $vf1, 0x0(%1)\n"
        "lqc2       $vf2, 0x10(%1)\n"
        "lqc2       $vf3, 0x20(%1)\n"
        "lqc2       $vf4, 0x30(%1)\n"
        "lqc2       $vf5, 0x100(%1)\n"
        "lqc2       $vf6, 0x110(%1)\n"
        "lqc2       $vf7, 0x120(%1)\n"
        "lqc2       $vf8, 0x130(%1)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "lqc2       $vf16, 0x10(%2)\n"
        "lqc2       $vf1, 0x40(%1)\n"
        "lqc2       $vf2, 0x50(%1)\n"
        "lqc2       $vf3, 0x60(%1)\n"
        "lqc2       $vf4, 0x70(%1)\n"
        "lqc2       $vf5, 0x140(%1)\n"
        "lqc2       $vf6, 0x150(%1)\n"
        "lqc2       $vf7, 0x160(%1)\n"
        "lqc2       $vf8, 0x170(%1)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "lqc2       $vf16, 0x20(%2)\n"
        "lqc2       $vf1, 0x80(%1)\n"
        "lqc2       $vf2, 0x90(%1)\n"
        "lqc2       $vf3, 0xA0(%1)\n"
        "lqc2       $vf4, 0xB0(%1)\n"
        "lqc2       $vf5, 0x180(%1)\n"
        "lqc2       $vf6, 0x190(%1)\n"
        "lqc2       $vf7, 0x1A0(%1)\n"
        "lqc2       $vf8, 0x1B0(%1)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "lqc2       $vf16, 0x30(%2)\n"
        "lqc2       $vf1, 0xC0(%1)\n"
        "lqc2       $vf2, 0xD0(%1)\n"
        "lqc2       $vf3, 0xE0(%1)\n"
        "lqc2       $vf4, 0xF0(%1)\n"
        "lqc2       $vf5, 0x1C0(%1)\n"
        "lqc2       $vf6, 0x1D0(%1)\n"
        "lqc2       $vf7, 0x1E0(%1)\n"
        "lqc2       $vf8, 0x1F0(%1)\n"
        "vadd.xyzw  $vf9, $vf1, $vf5\n"
        "vadd.xyzw  $vf10, $vf2, $vf6\n"
        "vadd.xyzw  $vf11, $vf3, $vf7\n"
        "vadd.xyzw  $vf12, $vf4, $vf8\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "lqc2       $vf16, 0x40(%2)\n"
        "vsub.xyzw  $vf9, $vf8, $vf4\n"
        "vsub.xyzw  $vf10, $vf7, $vf3\n"
        "vsub.xyzw  $vf11, $vf6, $vf2\n"
        "vsub.xyzw  $vf12, $vf5, $vf1\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "lqc2       $vf16, 0x50(%2)\n"
        "lqc2       $vf8, 0x1B0(%1)\n"
        "lqc2       $vf7, 0x1A0(%1)\n"
        "lqc2       $vf6, 0x190(%1)\n"
        "lqc2       $vf5, 0x180(%1)\n"
        "lqc2       $vf4, 0xB0(%1)\n"
        "lqc2       $vf3, 0xA0(%1)\n"
        "lqc2       $vf2, 0x90(%1)\n"
        "lqc2       $vf1, 0x80(%1)\n"
        "vsub.xyzw  $vf9, $vf8, $vf4\n"
        "vsub.xyzw  $vf10, $vf7, $vf3\n"
        "vsub.xyzw  $vf11, $vf6, $vf2\n"
        "vsub.xyzw  $vf12, $vf5, $vf1\n"
        "vmulx.xyzw $vf9, $vf9, $vf16x\n"
        "vmuly.xyzw $vf10, $vf10, $vf16y\n"
        "vmulz.xyzw $vf11, $vf11, $vf16z\n"
        "vmulw.xyzw $vf12, $vf12, $vf16w\n"
        "sqc2       $vf9, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf11, 0x0(%0)\n"
        "addu       %0, %0, %3\n"
        "sqc2       $vf12, 0x0(%0)\n"
        ".set noreorder\n"
        ".set reorder\n"
        : "+r"(out)
        : "r"(in), "r"(scale), "r"(stride)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C03E0);
#ifdef SKIP_ASM
/**
 * Finds the peak magnitude of each of the 24 subbands' 16 samples.
 *
 * Hand-written MMI: for 6 blocks of 4 rows x 16 floats it stores the largest |x| of each row to @p dst (as an integer
 * max of the sign-masked bits). The DTS packers func_003BD5B8 and func_003BDE08 turn the peaks into scale factors.
 *
 * @param src  the per-band samples (24 x 16 floats)
 * @param dst  receives the 24 peaks
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: hand-written MMI routine (no gcc frame; fixed $2/$6-$14; pnor/psrlw sign mask, lq + pand/pmaxw, pextlw/pextuw/
// pcpyld/pcpyud transpose, loop counter and pointer steps hand-interleaved into the MMI schedule: MMI has no gcc 2.95
// pattern): for each of 6 blocks of 4 rows x 16 floats it stores the largest |x| of every row (integer max of the
// sign-masked bits); only the final `jr $31; nop` is the compiler's. A port replaces it with
// `for (b = 0; b < 6; b++) for (r = 0; r < 4; r++) dst[b*4+r] = max over k < 16 of fabsf(src[b*64 + r*16 + k]);`.
void func_003C03E0(void *src, float *dst) {
    __asm__ __volatile__(
        ".set noreorder\n\t"
        "pnor $14, $0, $0\n\t"
        "psrlw $14, $14, 1\n\t"
        "addiu $2, $0, 0x6\n\t"
        "1:\n\t"
        "lq $6, 0x0(%0)\n\t"
        "lq $7, 0x40(%0)\n\t"
        "lq $8, 0x80(%0)\n\t"
        "lq $9, 0xC0(%0)\n\t"
        "pand $6, $6, $14\n\t"
        "pand $7, $7, $14\n\t"
        "pand $8, $8, $14\n\t"
        "pand $9, $9, $14\n\t"
        "lq $10, 0x10(%0)\n\t"
        "lq $11, 0x50(%0)\n\t"
        "lq $12, 0x90(%0)\n\t"
        "lq $13, 0xD0(%0)\n\t"
        "pand $10, $10, $14\n\t"
        "pand $11, $11, $14\n\t"
        "pand $12, $12, $14\n\t"
        "pand $13, $13, $14\n\t"
        "pmaxw $6, $6, $10\n\t"
        "pmaxw $7, $7, $11\n\t"
        "pmaxw $8, $8, $12\n\t"
        "pmaxw $9, $9, $13\n\t"
        "lq $10, 0x20(%0)\n\t"
        "lq $11, 0x60(%0)\n\t"
        "lq $12, 0xA0(%0)\n\t"
        "lq $13, 0xE0(%0)\n\t"
        "pand $10, $10, $14\n\t"
        "pand $11, $11, $14\n\t"
        "pand $12, $12, $14\n\t"
        "pand $13, $13, $14\n\t"
        "pmaxw $6, $6, $10\n\t"
        "pmaxw $7, $7, $11\n\t"
        "pmaxw $8, $8, $12\n\t"
        "pmaxw $9, $9, $13\n\t"
        "lq $10, 0x30(%0)\n\t"
        "lq $11, 0x70(%0)\n\t"
        "lq $12, 0xB0(%0)\n\t"
        "lq $13, 0xF0(%0)\n\t"
        "pand $10, $10, $14\n\t"
        "pand $11, $11, $14\n\t"
        "pand $12, $12, $14\n\t"
        "pand $13, $13, $14\n\t"
        "pmaxw $6, $6, $10\n\t"
        "pmaxw $7, $7, $11\n\t"
        "pmaxw $8, $8, $12\n\t"
        "pmaxw $9, $9, $13\n\t"
        "pextlw $10, $7, $6\n\t"
        "pextuw $7, $7, $6\n\t"
        "pextlw $11, $9, $8\n\t"
        "pextuw $9, $9, $8\n\t"
        "pcpyld $6, $11, $10\n\t"
        "pcpyud $8, $10, $11\n\t"
        "pcpyld $10, $9, $7\n\t"
        "pcpyud $11, $7, $9\n\t"
        "pmaxw $6, $6, $8\n\t"
        "pmaxw $10, $10, $11\n\t"
        "addiu $2, $2, -0x1\n\t"
        "pmaxw $6, $6, $10\n\t"
        "addiu %0, %0, 0x100\n\t"
        "sq $6, 0x0(%1)\n\t"
        "bnez $2, 1b\n\t"
        "addiu %1, %1, 0x10\n\t"
        ".set reorder"
        : "+r"(src), "+r"(dst)
        :
        : "$2", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C04F0);
#ifdef SKIP_ASM
/**
 * Quantizes 16 floats to 12-bit codes: scales them, truncates and packs them as halfwords masked with 0xFFF (VU0 +
 * MMI).
 *
 * Nothing in the decomp calls it; the DTS packers use the 10-, 8- and 5-bit siblings func_003C0564, func_003C05E0 and
 * func_003C0640.
 *
 * @param in  the 16 samples
 * @param out  receives 16 halfwords
 * @param scale  the factor applied before truncation
 */
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed halfwords).
void func_003C04F0(void *in, void *out, float scale) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0xFFF\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        "vftoi0.xyzw $vf1, $vf1\n"
        "vftoi0.xyzw $vf2, $vf2\n"
        "vftoi0.xyzw $vf3, $vf3\n"
        "vftoi0.xyzw $vf4, $vf4\n"
        "qmfc2.ni  $6, $vf1\n"
        "qmfc2.ni  $7, $vf2\n"
        "qmfc2.ni  $8, $vf3\n"
        "qmfc2.ni  $9, $vf4\n"
        "ppach     $6, $7, $6\n"
        "ppach     $8, $9, $8\n"
        "pand      $6, $6, $10\n"
        "pand      $8, $8, $10\n"
        "sq        $6, 0x0(%1)\n"
        "sq        $8, 0x10(%1)\n"
        :
        : "r"(in), "r"(out), "f"(scale)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0564);
#ifdef SKIP_ASM
/**
 * Quantizes 16 floats to 10-bit codes: scales them, truncates and packs them as halfwords masked with 0x3FF (VU0 +
 * MMI).
 *
 * Used by func_003BD5B8 and func_003BDE08 for the low subbands.
 *
 * @param in  the 16 samples
 * @param out  receives 16 halfwords
 * @param scale  the factor applied before truncation (1 / the band's scale factor)
 */
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed halfwords).
void func_003C0564(void *in, void *out, float scale) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0x3FF\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        "vftoi0.xyzw $vf1, $vf1\n"
        "vftoi0.xyzw $vf2, $vf2\n"
        "vftoi0.xyzw $vf3, $vf3\n"
        "vftoi0.xyzw $vf4, $vf4\n"
        "qmfc2.ni  $6, $vf1\n"
        "qmfc2.ni  $7, $vf2\n"
        "qmfc2.ni  $8, $vf3\n"
        "qmfc2.ni  $9, $vf4\n"
        "ppach     $6, $7, $6\n"
        "ppach     $8, $9, $8\n"
        "pand      $6, $6, $10\n"
        "pand      $8, $8, $10\n"
        "sq        $6, 0x0(%1)\n"
        "sq        $8, 0x10(%1)\n"
        :
        : "r"(in), "r"(out), "f"(scale)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C05E0);
#ifdef SKIP_ASM
/**
 * Quantizes 16 floats to 8-bit codes: scales them, truncates and packs them as bytes (VU0 + MMI).
 *
 * Used by func_003BD5B8 and func_003BDE08 for the middle subbands.
 *
 * @param in  the 16 samples
 * @param out  receives 16 bytes
 * @param scale  the factor applied before truncation (1 / the band's scale factor)
 */
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed bytes).
void func_003C05E0(void *in, void *out, float scale) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        "vftoi0.xyzw $vf1, $vf1\n"
        "vftoi0.xyzw $vf2, $vf2\n"
        "vftoi0.xyzw $vf3, $vf3\n"
        "vftoi0.xyzw $vf4, $vf4\n"
        "qmfc2.ni  $6, $vf1\n"
        "qmfc2.ni  $7, $vf2\n"
        "qmfc2.ni  $8, $vf3\n"
        "qmfc2.ni  $9, $vf4\n"
        "ppach     $6, $7, $6\n"
        "ppach     $8, $9, $8\n"
        "ppacb     $6, $8, $6\n"
        "sq        $6, 0x0(%1)\n"
        :
        : "r"(in), "r"(out), "f"(scale)
        : "$6", "$7", "$8", "$9", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0640);
#ifdef SKIP_ASM
/**
 * Quantizes 16 floats to 5-bit codes: scales them, truncates and packs them as bytes masked with 0x1F (VU0 + MMI).
 *
 * Used by func_003BDE08 for the high subbands of a DTS 5.1 frame.
 *
 * @param in  the 16 samples
 * @param out  receives 16 bytes
 * @param scale  the factor applied before truncation (1 / the band's scale factor)
 */
// PORT: PS2-only VU0 inline asm (scale 4x4 float matrix, convert to packed bytes).
void func_003C0640(void *in, void *out, float scale) {
    __asm__ volatile(
        "mfc1      $6, %2\n"
        "qmtc2.ni  $6, $vf5\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "addiu     $10, $0, 0x1F1F\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "pcpyh     $10, $10\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "pcpyld    $10, $10, $10\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "vmulx.xyzw $vf1, $vf1, $vf5x\n"
        "vmulx.xyzw $vf2, $vf2, $vf5x\n"
        "vmulx.xyzw $vf3, $vf3, $vf5x\n"
        "vmulx.xyzw $vf4, $vf4, $vf5x\n"
        "vftoi0.xyzw $vf1, $vf1\n"
        "vftoi0.xyzw $vf2, $vf2\n"
        "vftoi0.xyzw $vf3, $vf3\n"
        "vftoi0.xyzw $vf4, $vf4\n"
        "qmfc2.ni  $6, $vf1\n"
        "qmfc2.ni  $7, $vf2\n"
        "qmfc2.ni  $8, $vf3\n"
        "qmfc2.ni  $9, $vf4\n"
        "ppach     $6, $7, $6\n"
        "ppach     $8, $9, $8\n"
        "ppacb     $6, $8, $6\n"
        "pand      $6, $6, $10\n"
        "sq        $6, 0x0(%1)\n"
        :
        : "r"(in), "r"(out), "f"(scale)
        : "$6", "$7", "$8", "$9", "$10", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C06B0);
#ifdef SKIP_ASM
/**
 * Low-pass filters and decimates the LFE input into 8 samples for a DTS frame.
 *
 * Hand-written FPU code: for each of the 8 outputs it runs 64 input samples through a three-stage filter with the
 * coefficients @p coef, keeping the filter state in @p state[0..5] across calls. Called by func_003BD078.
 *
 * @param in  the LFE input (16-bit samples)
 * @param out  receives the 8 decimated samples
 * @param state  the 6-float filter state (+0x6C of the DTS encoder state)
 * @param coef  the filter coefficients (D_0044DD00)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: hand-written FPU filter (mula.s/madda.s/madd.s accumulator ops and trapping addi have no gcc 2.95 pattern;
// coefficients and filter state live in fixed $f1..$f13/$f20..$f25 across both loops, and the callee-saved
// $f20..$f25/$f31 are used without being saved, which no compiled function does, so they are deliberately left out
// of the clobber list). For each of 8 outputs it runs 64 input samples through a three-stage filter whose state is
// read from and written back to state[0..5]. A port replaces the block with the same filter in C.
void func_003C06B0(short *in, float *out, float *state, const float *coef) {
    __asm__ volatile(
        ".set noreorder\n"
        "addi       $8, %1, 0x20\n"
        "mtc1       $0, $f0\n"
        "lwc1       $f1, 0x0(%3)\n"
        "lwc1       $f2, 0x4(%3)\n"
        "lwc1       $f3, 0x8(%3)\n"
        "lwc1       $f4, 0xC(%3)\n"
        "lwc1       $f5, 0x10(%3)\n"
        "lwc1       $f6, 0x14(%3)\n"
        "lwc1       $f7, 0x18(%3)\n"
        "lwc1       $f8, 0x1C(%3)\n"
        "lwc1       $f9, 0x20(%3)\n"
        "lwc1       $f10, 0x24(%3)\n"
        "lwc1       $f11, 0x28(%3)\n"
        "lwc1       $f12, 0x2C(%3)\n"
        "lwc1       $f13, 0x30(%3)\n"
        "lwc1       $f20, 0x0(%2)\n"
        "lwc1       $f21, 0x4(%2)\n"
        "lwc1       $f22, 0x8(%2)\n"
        "lwc1       $f23, 0xC(%2)\n"
        "lwc1       $f24, 0x10(%2)\n"
        "lwc1       $f25, 0x14(%2)\n"
        "1:\n"
        "addi       $9, %0, 0x80\n"
        "2:\n"
        "lh         $12, 0x0(%0)\n"
        "mtc1       $12, $f31\n"
        "cvt.s.w    $f31, $f31\n"
        "mula.s     $f1, $f31\n"
        "madda.s    $f20, $f2\n"
        "madda.s    $f21, $f3\n"
        "addi       %0, %0, 0x2\n"
        "madd.s     $f31, $f0, $f0\n"
        "madda.s    $f20, $f4\n"
        "madda.s    $f21, $f5\n"
        "mov.s      $f20, $f21\n"
        "mov.s      $f21, $f31\n"
        "madda.s    $f22, $f6\n"
        "madda.s    $f23, $f7\n"
        "madd.s     $f31, $f0, $f0\n"
        "madda.s    $f22, $f8\n"
        "madda.s    $f23, $f9\n"
        "mov.s      $f22, $f23\n"
        "mov.s      $f23, $f31\n"
        "madda.s    $f24, $f10\n"
        "madda.s    $f25, $f11\n"
        "madd.s     $f31, $f0, $f0\n"
        "madda.s    $f24, $f12\n"
        "madda.s    $f25, $f13\n"
        "mov.s      $f24, $f25\n"
        "bne        $9, %0, 2b\n"
        "mov.s      $f25, $f31\n"
        "addi       %1, %1, 0x4\n"
        "madd.s     $f31, $f0, $f0\n"
        "bne        $8, %1, 1b\n"
        "swc1       $f31, -0x4(%1)\n"
        "swc1       $f20, 0x0(%2)\n"
        "swc1       $f21, 0x4(%2)\n"
        "swc1       $f22, 0x8(%2)\n"
        "swc1       $f23, 0xC(%2)\n"
        "swc1       $f24, 0x10(%2)\n"
        "swc1       $f25, 0x14(%2)\n"
        ".set reorder\n"
        : "+r"(in), "+r"(out)
        : "r"(state), "r"(coef)
        : "$8", "$9", "$12", "$f0", "$f1", "$f2", "$f3", "$f4", "$f5", "$f6", "$f7", "$f8", "$f9", "$f10", "$f11",
          "$f12", "$f13", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C07A8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 FPU-flag probe: builds two float4s from flag bits and multiplies them on VU0.
void func_003C07A8_impl(int flags) __asm__("func_003C07A8");
/**
 * Recreates saved VU0 MAC flags by performing one vector multiply that sets them.
 *
 * For each lane (four, shifting @p flags left each time) it picks a pair of operands: 0 for the zero bit, 1 otherwise,
 * or 1e-30 / 1e30 when the underflow / overflow bits (0x8800) are set, the second operand negated for the sign bit
 * (0x80); vf1 and vf2 are preserved around the vmul. Called by func_003C08F8 with the flags func_003C0858 saved.
 *
 * @param flags  the saved MAC flag word
 */
void func_003C07A8_impl(int flags)
{
    float lhs[4] __attribute__((aligned(16)));
    float rhs[4] __attribute__((aligned(16)));
    int i;
    for (i = 0; i < 4; i++) {
        float laneValue;
        if ((flags & 0x8800) == 0) {
            laneValue = 0.0f;
            if (!(flags & 8))
                laneValue = 1.0f;
        } else {
            laneValue = 1.0000000031710769e-30f;
            if (!(flags & 8))
                laneValue = 1.0000000150474662e+30f;
        }
        lhs[i] = laneValue;
        if (flags & 0x80)
            laneValue = -laneValue;
        rhs[i] = laneValue;
        flags <<= 1;
    }
    __asm__ __volatile__(
        "qmfc2.ni   $8, $vf1\n"
        "qmfc2.ni   $9, $vf2\n"
        "lqc2       $vf1, %0\n"
        "lqc2       $vf2, %1\n"
        "vmul.xyzw  $vf1, $vf1, $vf2\n"
        "qmtc2.ni   $8, $vf1\n"
        "qmtc2.ni   $9, $vf2\n"
        "vnop\n"
        :
        : "m"(lhs[0]), "m"(rhs[0])
        : "$8", "$9");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0858);
#ifdef SKIP_ASM
/* PORT: PS2-only VU0 state save; func_0031A308 without the $vi22/$vi18
   saves, and with ctc2 (not cfc2) on $vi16 as in the shipped code. */
/**
 * Saves the VU0 register file into a buffer (0x208 bytes used) before the sound thread uses VU0.
 *
 * Stores vf1..vf31 at 0x10 * (n - 1), the accumulator (read into vf1 with vmaddx after vf1 is saved) at 0x1F0 and the
 * MAC flags ($vi17) at 0x200. The sound thread SNDDRV_thread calls it and restores with func_003C08F8. It is
 * func_0031A308 without the $vi22/$vi18 saves. Likely bug, kept from the shipped code: where the status flags should be
 * read it does ctc2 $9, $vi16, which writes the MAC flags into the status register, and then stores the MAC flags again
 * at 0x204.
 *
 * @param buf  the save area (16-byte aligned, at least 0x208 bytes)
 */
void func_003C0858(void* buf)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "daddu      $8, %0, $0\n"
        "cfc2.i     $9, $vi17\n"
        "sqc2       $vf1, 0x0($8)\n"
        "sqc2       $vf2, 0x10($8)\n"
        "sw         $9, 0x200($8)\n"
        "sqc2       $vf3, 0x20($8)\n"
        "sqc2       $vf4, 0x30($8)\n"
        "ctc2.ni    $9, $vi16\n"
        "sqc2       $vf5, 0x40($8)\n"
        "sqc2       $vf6, 0x50($8)\n"
        "sw         $9, 0x204($8)\n"
        "sqc2       $vf7, 0x60($8)\n"
        "sqc2       $vf8, 0x70($8)\n"
        "vmaddx.xyzw $vf1, $vf0, $vf0x\n"
        "sqc2       $vf9, 0x80($8)\n"
        "sqc2       $vf10, 0x90($8)\n"
        "sqc2       $vf11, 0xA0($8)\n"
        "sqc2       $vf12, 0xB0($8)\n"
        "sqc2       $vf13, 0xC0($8)\n"
        "sqc2       $vf14, 0xD0($8)\n"
        "sqc2       $vf15, 0xE0($8)\n"
        "sqc2       $vf16, 0xF0($8)\n"
        "sqc2       $vf17, 0x100($8)\n"
        "sqc2       $vf18, 0x110($8)\n"
        "sqc2       $vf19, 0x120($8)\n"
        "sqc2       $vf20, 0x130($8)\n"
        "sqc2       $vf21, 0x140($8)\n"
        "sqc2       $vf22, 0x150($8)\n"
        "sqc2       $vf23, 0x160($8)\n"
        "sqc2       $vf24, 0x170($8)\n"
        "sqc2       $vf25, 0x180($8)\n"
        "sqc2       $vf26, 0x190($8)\n"
        "sqc2       $vf27, 0x1A0($8)\n"
        "sqc2       $vf28, 0x1B0($8)\n"
        "sqc2       $vf29, 0x1C0($8)\n"
        "sqc2       $vf30, 0x1D0($8)\n"
        "sqc2       $vf31, 0x1E0($8)\n"
        "sqc2       $vf1, 0x1F0($8)\n"
        ".set pop\n"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C08F8);
#ifdef SKIP_ASM
void func_003C07A8(int status);

/* PORT: PS2-only VU0 state restore; same as func_0031A3C0 without the
   $vi22/$vi18 restore. */
/**
 * Restores the VU0 register file that func_003C0858 saved.
 *
 * Reloads the accumulator from 0x1F0 (vaddax with vf0.x), vf1..vf6, then re-creates the MAC flags from the word at
 * 0x200 with func_003C07A8 (which preserves vf1/vf2 around its vmul), then reloads vf7..vf31. The status word at 0x204
 * is not used. It is func_0031A3C0 without the $vi22/$vi18 restore; the sound thread SNDDRV_thread calls it after
 * mixing.
 *
 * @param buf  the save area written by func_003C0858
 */
void func_003C08F8(void* buf)
{
    __asm__ __volatile__(
        ".set push
"
        ".set noreorder
"
        "daddu      $8, %0, $0
"
        "lqc2       $vf1, 0x1F0($8)
"
        "vaddax.xyzw ACC, $vf1, $vf0x
"
        "lqc2       $vf1, 0x0($8)
"
        "lqc2       $vf2, 0x10($8)
"
        "lqc2       $vf3, 0x20($8)
"
        "lqc2       $vf4, 0x30($8)
"
        "lqc2       $vf5, 0x40($8)
"
        "lqc2       $vf6, 80($8)
"
        ".set pop
"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
    func_003C07A8(*(int*)((char*)buf + 0x200));
    __asm__ __volatile__(
        ".set push
"
        ".set noreorder
"
        "daddu      $8, %0, $0
"
        "lqc2       $vf7, 0x60($8)
"
        "lqc2       $vf8, 0x70($8)
"
        "lqc2       $vf9, 0x80($8)
"
        "lqc2       $vf10, 144($8)
"
        "lqc2       $vf11, 160($8)
"
        "lqc2       $vf12, 176($8)
"
        "lw         $9, 516($8)
"
        "lqc2       $vf13, 0xC0($8)
"
        "lqc2       $vf14, 208($8)
"
        "ctc2.ni    $9, $vi16
"
        "lqc2       $vf15, 0xE0($8)
"
        "lqc2       $vf16, 0xF0($8)
"
        "lqc2       $vf17, 0x100($8)
"
        "lqc2       $vf18, 272($8)
"
        "lqc2       $vf19, 288($8)
"
        "lqc2       $vf20, 304($8)
"
        "lqc2       $vf21, 320($8)
"
        "lqc2       $vf22, 336($8)
"
        "lqc2       $vf23, 352($8)
"
        "lqc2       $vf24, 368($8)
"
        "lqc2       $vf25, 0x180($8)
"
        "lqc2       $vf26, 400($8)
"
        "lqc2       $vf27, 416($8)
"
        "lqc2       $vf28, 432($8)
"
        "lqc2       $vf29, 0x1C0($8)
"
        "lqc2       $vf30, 464($8)
"
        "lqc2       $vf31, 0x1E0($8)
"
        ".set pop
"
        :
        : "r"(buf)
        : "$8", "$9", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C09B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Saves seven DMA channel registers (0x1000D010..D420) into the block, then calls func_003C4A38.
struct G_09B8 { char p[0x1AAE]; unsigned short rate; char p2[0x1B1C - 0x1AB0]; int save[7]; int h; };
extern struct G_09B8 D_0050AD00_09B8 __asm__("D_0050AD00");
extern int D_0050A94C_09B8[] __asm__("D_0050A94C");
extern void func_003C4A38_09B8(int, int, int, int) __asm__("func_003C4A38");
/**
 * Saves the scratchpad DMA registers and copies the scratchpad area the DTS encoder will use out to main memory.
 *
 * Keeps the seven registers 0x1000D010, D080, D020 (channel 8) and D410, D430, D480, D420 (channel 9) in +0x1B1C of the
 * IOP sound state D_0050AD00, then has func_003C4A38 copy (+0x1AAE >> 4) quadwords from D_0050A94C to the save buffer
 * +0x1B38, waiting for it. The sound thread SNDDRV_thread calls it before encoding when +0x1AAE is set; func_003C0A60
 * undoes it.
 */
void func_003C09B8(void) {
    D_0050AD00_09B8.save[0] = *(volatile int *)0x1000D010;
    D_0050AD00_09B8.save[1] = *(volatile int *)0x1000D080;
    D_0050AD00_09B8.save[2] = *(volatile int *)0x1000D020;
    D_0050AD00_09B8.save[3] = *(volatile int *)0x1000D410;
    D_0050AD00_09B8.save[4] = *(volatile int *)0x1000D430;
    D_0050AD00_09B8.save[5] = *(volatile int *)0x1000D480;
    D_0050AD00_09B8.save[6] = *(volatile int *)0x1000D420;
    func_003C4A38_09B8(D_0050A94C_09B8[0], D_0050AD00_09B8.h, D_0050AD00_09B8.rate >> 4, 0x10000);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0A60);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C4978_0A60(int, int, unsigned int, int) __asm__("func_003C4978");
extern int D_0050A94C_0A60[] __asm__("D_0050A94C");
extern char D_0050AD00_0A60[] __asm__("D_0050AD00");

/**
 * Copies the saved scratchpad area back and restores the scratchpad DMA registers func_003C09B8 saved.
 *
 * func_003C4978 copies (+0x1AAE >> 4) quadwords from the save buffer +0x1B38 of D_0050AD00 back to D_0050A94C, waiting
 * for it; then the seven channel 8 / 9 registers are rewritten from +0x1B1C. Called by the sound thread SNDDRV_thread
 * after encoding.
 */
void func_003C0A60(void) {
    char *soundState = D_0050AD00_0A60;
    func_003C4978_0A60(*(int *)(soundState + 0x1B38), D_0050A94C_0A60[0], *(unsigned short *)(soundState + 0x1AAE) >> 4, 0x10000);
    *(volatile int *)0x1000D010 = *(int *)(soundState + 0x1B1C);
    *(volatile int *)0x1000D080 = *(int *)(soundState + 0x1B20);
    *(volatile int *)0x1000D020 = *(int *)(soundState + 0x1B24);
    *(volatile int *)0x1000D410 = *(int *)(soundState + 0x1B28);
    *(volatile int *)0x1000D430 = *(int *)(soundState + 0x1B2C);
    *(volatile int *)0x1000D480 = *(int *)(soundState + 0x1B30);
    *(volatile int *)0x1000D420 = *(int *)(soundState + 0x1B34);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C0B10);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the per-function sAD00 view with volatile fields (the unit's other accessors read them plain); the `volatile int i` busy-wait counter itself is honest (without it the delay loop is deleted, 66.64%). Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
/* D_0050AD00_0B10: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
struct sAD00Voice_0B10 { int key; int val; };

struct sAD00SpuVoice_0B10 {              /* 8 bytes, sAD00Spu_0B10.f20[] */
    int key;
    short pitch;
    signed char l;
    signed char r;
};

struct sAD00SpuChan_0B10 {               /* 16 bytes, sAD00Spu_0B10.f1A0[] */
    int key;
    short pitch;
    signed char vol[6];
    signed char f0C[4];
};

struct sAD00Spu_0B10 {                   /* 0x2A0, block 3 of the shared buffer (D_0050AD00_0B10.fCB0) */
    long f00;                       /* voice on bits */
    unsigned long f08;
    char f10;
    char f11;
    char f12;
    unsigned char f13;
    short f14;
    short f16;
    char pad18[0x20 - 0x18];
    struct sAD00SpuVoice_0B10 f20[48];
    struct sAD00SpuChan_0B10 f1A0[16];
};

struct sAD00Xfer_0B10 {                  /* 16 bytes, D_0050AD00_0B10.fCC8[] (also D_0050B9C8) */
    unsigned int key;
    int f04;
    int f08;
    unsigned short f0C;
    unsigned char f0E;
    char pad0F;
};

struct sAD00Spk_0B10 {                   /* 0x20, D_0050AD00_0B10.fE98[] (also D_0050BB98) */
    float f00[6];
    unsigned int f18;
    short f1C;
    short f1E;
};

struct sAD00Seg_0B10 { unsigned short a; unsigned short b; };

struct sAD00Port_0B10 { int f00; int f04; int pad08[2]; };   /* 16 bytes, D_0050AD00_0B10.fE88[] */

struct sAD00_0B10 {
    char f000[0x83C];               /* RPC send buffer */
    struct sAD00Voice_0B10 f83C[48];
    struct sAD00Voice_0B10 f9BC[16];
    char padA3C[0xC80 - 0xA3C];
    char fC80[0x30];                /* RPC client data */
    struct sAD00Spu_0B10 *fCB0;          /* also D_0050B9B0 */
    int *fCB4;
    void *fCB8;
    volatile int fCBC;              /* last transfer id */
    volatile unsigned char fCC0;    /* queued transfers */
    volatile unsigned char fCC1;    /* selected transfer */
    volatile unsigned char fCC2;    /* transfer running */
    char padCC3;
    int fCC4;                       /* tick of the last transfer */
    struct sAD00Xfer_0B10 fCC8[28];
    struct sAD00Port_0B10 fE88[1];
    struct sAD00Spk_0B10 fE98[96];
    unsigned short f1A98;           /* number of f1AA0 segments */
    unsigned short f1A9A;
    unsigned short f1A9C;
    unsigned short f1A9E;
    struct sAD00Seg_0B10 *f1AA0;
    int f1AA4;
    int f1AA8;
    char f1AAC;
    unsigned char f1AAD;
    unsigned short f1AAE;
    char pad1AB0[0x1AB2 - 0x1AB0];
    unsigned short f1AB2;           /* fill offset while func_003C1638 lays out f1AB4 */
    char *f1AB4;
    int f1AB8;
    void *f1ABC[6];
    int f1AD4;
    int f1AD8;
    char pad1ADC[0x1AF4 - 0x1ADC];
    void *f1AF4;
    int f1AF8;
    char pad1AFC[0x1B00 - 0x1AFC];
    void *f1B00;
    int f1B04;
    int f1B08;
    int f1B0C;
    volatile int f1B10;             /* lock: semaphore, owner thread, depth */
    volatile int f1B14;
    volatile int f1B18;
    int f1B1C[7];                   /* saved SPU2 DMA registers */
    int f1B38;
};

extern struct sAD00_0B10 D_0050AD00_0B10 __asm__("D_0050AD00");

extern void Util_MemCpy(void *, void *, int);
extern int sceSifCheckStatRpc(void *);
extern void sceSifCallRpc(void *, int, int, void *, int, int, int, int, int);

/**
 * Sends a command to the IOP sound driver over SIF RPC.
 *
 * Copies @p sendSize bytes of @p sendData into the RPC send buffer (+0x000 of D_0050AD00, Util_MemCpy) when the size is non-zero,
 * waits (10000-iteration delay loops) while the client +0xC80 is busy (sceSifCheckStatRpc), then calls the function with
 * sceSifCallRpc (sceSifCallRpc), no-wait mode for function 2. Used by the platform init SNDPLATFORM_init (function 0),
 * SNDPLATFORM_restore and func_003C3450.
 *
 * @param rpcFunction  the RPC function number
 * @param sendData  the command data
 * @param sendSize  its size in bytes
 */
void func_003C0B10(int rpcFunction, void *sendData, int sendSize) {
    volatile int i;
    int two = rpcFunction == 2;
    if (sendSize) {
        Util_MemCpy(D_0050AD00_0B10.f000, sendData, sendSize);
    }
    while (sceSifCheckStatRpc(D_0050AD00_0B10.fC80)) {
        for (i = 0; i < 10000; i++) {
        }
    }
    sceSifCallRpc(D_0050AD00_0B10.fC80, rpcFunction, two, D_0050AD00_0B10.f000, sendSize, 0, 0, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDDRV_mixvoicefree);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDVOICEI_free(int, unsigned char);
struct S_0BE8 { char pad[0x2F]; unsigned char a; unsigned char b; };

/**
 * Frees a mixer voice, given its index among the software voices, with SNDVOICEI_free.
 *
 * The driver voice is the index plus the counts +0x30 and +0x2F of sndgs (the voices before the mixer's), and that
 * second count is passed too. The platform init SNDPLATFORM_init hands it to the mixer setup MIX_create as its
 * voice-free callback.
 *
 * @param voiceIndex  the mixer voice index
 */
void SNDDRV_mixvoicefree(int voiceIndex) {
    SNDVOICEI_free(voiceIndex + sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu, sndgs.sso.set.voicesiopcpu);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_getvoicerange);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns the range of driver voices a render mode plays on.
 *
 * Mode bit 8 (SPU hardware voices) gives [0, +0x30 of sndgs); bit 0x100 gives [+0x30, +0x30 + +0x2F); bit 4 (the
 * software mixer) gives [+0x30 + +0x2F, +0x17E, the channel count). Other modes leave the outputs alone.
 * SNDBANKI_playtimbre and SNDPKTPLAY_start pass the range to SNDVOICEI_alloc.
 *
 * @param renderMode  the render mode
 * @param minVoice  receives the first voice
 * @param maxVoice  receives the end of the range (exclusive)
 */
void SNDPLATFORM_getvoicerange(int renderMode, int *minVoice, int *maxVoice) {
    if (renderMode & 8) {
        *minVoice = 0;
        *maxVoice = sndgs.sso.set.voicesspu;
        return;
    }
    if (renderMode & 0x100) {
        char *driverState = ((char*)&sndgs);
        *minVoice = *(unsigned char *)(driverState + 0x30);
        *maxVoice = *(unsigned char *)(driverState + 0x30) + *(unsigned char *)(driverState + 0x2F);
        return;
    }
    if (renderMode & 4) {
        char *driverState = ((char*)&sndgs);
        *minVoice = *(unsigned char *)(driverState + 0x30) + *(unsigned char *)(driverState + 0x2F);
        *maxVoice = *(short *)(driverState + 0x17E);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_outputcaps);
#ifdef SKIP_ASM
extern int D_0044DD3C_0C90[] __asm__("D_0044DD3C");
/**
 * Fills the platform part of the default sound options in sndgs and returns 0.
 *
 * Rates 24000 (+0x00) and 48000 (+0x02, +0x2A, +0x2C), voice limits (+0x04..+0x08: 1, 5, 32, 16, 48), 6 at +0x13, voice
 * counts +0x2F (8) and +0x30 (48), output channels +0x47 (2), option bits +0x49 (1), 3 output ports at +0x4C, the
 * module directory +0x6C from D_0044DD3C, and the default speaker angle tables (+0x84..+0xB0: 0x4000 / 0xC000 for two
 * speakers, then 3, 4 and 5 speaker layouts). SNDSYS_getopts calls it on first use and keeps its result.
 *
 * @return 0
 */
int SNDPLATFORM_outputcaps(void) {
    sndgs.sso.cap.unk_0C = 0;
    sndgs.sso.cap.outputratemin = 24000;
    sndgs.sso.cap.voicesmaincpumax = 0x20;
    sndgs.sso.cap.outputratemax = 48000;
    sndgs.sso.cap.voicesiopcpumax = 0x10;
    sndgs.sso.cap.outputchannelsmax = 5;
    sndgs.sso.cap.outputchannelsmin = 1;
    sndgs.sso.set.outputspdifmode = 1;
    sndgs.sso.cap.voicesspumax = 0x30;
    sndgs.sso.cap.rendermode[0] = 40;
    sndgs.sso.cap.numrendermodes = 6;
    sndgs.sso.set.useeax = 0;
    sndgs.sso.set.voicesmaincpu = 8;
    sndgs.sso.cap.rendermode[3] = 72;
    sndgs.sso.cap.rendermode[1] = 36;
    sndgs.sso.cap.rendermode[4] = 68;
    sndgs.sso.cap.rendermode[2] = 288;
    sndgs.sso.set.voicesiopcpu = 8;
    sndgs.sso.set.rendermode[0] = 40;
    sndgs.sso.set.rendermode[1] = 36;
    sndgs.sso.set.outputrate = 48000;
    sndgs.sso.cap.rendermode[5] = 320;
    sndgs.sso.set.numrendermodes = 3;
    sndgs.sso.set.rendermode[2] = 288;
    sndgs.sso.set.voicesspu = 0x30;
    sndgs.sso.set.maxstreams = 8;
    sndgs.sso.set.outputrateiopcpu = 48000;
    sndgs.sso.set.outputchannels = 2;
    sndgs.sso.set.routemaincpufxtoiopcpu = 0;
    sndgs.sso.set.resamplequality = 25;
    sndgs.sso.set.loadiopmodules = 1;
    ((int)sndgs.sso.set.iopmodulepath) = D_0044DD3C_0C90[0];
    sndgs.sso.set.iopcpuresamplequality = 25;
    sndgs.sso.set.virtualspkrcfg3d[0][0] = 0;
    sndgs.sso.set.virtualspkrcfg3d[1][0] = 0x4000;
    sndgs.sso.set.virtualspkrcfg3d[1][1] = 0xC000;
    sndgs.sso.set.virtualspkrcfg3d[2][0] = 10922;
    sndgs.sso.set.virtualspkrcfg3d[2][1] = 0x8000;
    sndgs.sso.set.virtualspkrcfg3d[2][2] = 54613;
    sndgs.sso.set.virtualspkrcfg3d[3][0] = 0x2000;
    sndgs.sso.set.virtualspkrcfg3d[3][1] = 0x6000;
    sndgs.sso.set.virtualspkrcfg3d[3][2] = 40960;
    sndgs.sso.set.virtualspkrcfg3d[3][3] = 0xE000;
    sndgs.sso.set.virtualspkrcfg3d[4][0] = 0;
    sndgs.sso.set.virtualspkrcfg3d[4][1] = 0x2000;
    sndgs.sso.set.virtualspkrcfg3d[4][2] = 0x6000;
    sndgs.sso.set.virtualspkrcfg3d[4][3] = 40960;
    sndgs.sso.set.virtualspkrcfg3d[4][4] = 0xE000;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_outputset);
#ifdef SKIP_ASM
struct G_0E28 {
    char pad0[4];
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    unsigned char b8;
    char pad9[0x20 - 9];
    struct SNDSYSSET sub;
    char padC0[0xD4 - 0xC0];
    struct SNDSYSSET saved;
    char pad174[0x174 - 0x174];
    signed char f174;
    char pad175[0x17E - 0x175];
    short h17E;
    char pad180[0x1F0 - 0x180];
    struct CHANPUB *chans;
};
struct O_0E28 { char pad[0x13]; unsigned char b13; };
extern struct SNDSYSSET D_0050A908_0E28 __asm__("D_0050A908");
extern struct O_0E28 *D_0050B9B0_0E28 __asm__("D_0050B9B0");
extern void f8148_0E28(int) __asm__("MIX_setresamplequality");
extern void f38E0_0E28(int) __asm__("func_003C38E0");

/**
 * Validates the requested sound options (D_0050A908) and applies them to the driver.
 *
 * Rounds both sample rates (+0x0A, +0x0C) up to 24000, 36000 or 48000; option bit 2 (+0x29, DTS) forces +0x10 to 0x18
 * and at least 4 output channels, bit 1 at most 2; clamps the DTS quality +0x2A to 100, the output channels +0x27 to
 * the platform range (+0x04/+0x05 of sndgs) with +0x28 set only for 5, the three voice counts +0x0E..+0x10 to the
 * platform limits (+0x06..+0x08) and +0x26 to 0x20; passes +0x20 to MIX_setresamplequality and rounds the buffer +0x44 up to 16
 * (shrinking +0x48). Before the system is started (+0x174) it stores everything and sets the channel count +0x17E to
 * the sum of the voice counts. Once started, a change of the option bits is refused (returns 0 with the saved options
 * put back) and only +0x20, +0x21 (also to +0x13 of the voice block D_0050B9B0) and the output channels are applied,
 * the latter re-panning (func_003C38E0) every live master channel (+0x69 == 1, +0x28 < 0). Called by SNDSYS_setopts.
 *
 * @return 0
 */
int SNDPLATFORM_outputset(void) {
    struct SNDSYSSET requested;
    int i;

    requested = D_0050A908_0E28;
    if (sndgs.installed) {
        *(struct SNDSYSSET*)&sndgs.sso.set = *(struct SNDSYSSET*)&sndgs.prevset;
        if (sndgs.prevset.outputspdifmode != requested.outputspdifmode) {
            return 0;
        }
    }
    if (requested.outputrate <= 24000) {
        requested.outputrate = 24000;
    } else if (requested.outputrate <= 36000) {
        requested.outputrate = 36000;
    } else {
        requested.outputrate = 48000;
    }
    if (requested.outputrateiopcpu <= 24000) {
        requested.outputrateiopcpu = 24000;
    } else if (requested.outputrateiopcpu <= 36000) {
        requested.outputrateiopcpu = 36000;
    } else {
        requested.outputrateiopcpu = 48000;
    }
    if (requested.outputspdifmode & 2) {
        requested.voicesspu = 0x18;
        if (requested.outputchannels < 4) {
            requested.outputchannels = 4;
        }
    }
    if (requested.outputspdifmode & 1) {
        if (requested.outputchannels > 2) {
            requested.outputchannels = 2;
        }
        requested.outputlfe = 0;
    }
    if (requested.dtsquality > 100) {
        requested.dtsquality = 100;
    }
    if (requested.outputchannels < sndgs.sso.cap.outputchannelsmin) {
        requested.outputchannels = sndgs.sso.cap.outputchannelsmin;
    } else if (requested.outputchannels > sndgs.sso.cap.outputchannelsmax) {
        requested.outputchannels = sndgs.sso.cap.outputchannelsmax;
    }
    if (requested.outputchannels == 5) {
        requested.outputlfe = 1;
    } else {
        requested.outputlfe = 0;
    }
    if (requested.voicesmaincpu > sndgs.sso.cap.voicesmaincpumax) {
        requested.voicesmaincpu = sndgs.sso.cap.voicesmaincpumax;
    }
    if (requested.voicesiopcpu > sndgs.sso.cap.voicesiopcpumax) {
        requested.voicesiopcpu = sndgs.sso.cap.voicesiopcpumax;
    }
    if (requested.voicesspu > sndgs.sso.cap.voicesspumax) {
        requested.voicesspu = sndgs.sso.cap.voicesspumax;
    }
    if (requested.maxstreams > 0x20) {
        requested.maxstreams = 0x20;
    }
    f8148_0E28(requested.resamplequality);
    if ((unsigned int)sndgs.sso.set.fastramaddr != 0 && (((unsigned int)requested.fastramaddr) & 0xF)) {
        (*(unsigned int*)&requested.fastramaddr) = ((((unsigned int)requested.fastramaddr) + 0xF) >> 4) << 4;
        requested.fastramsize -= 0x10;
    }
    if (sndgs.installed) {
        sndgs.sso.set.resamplequality = requested.resamplequality;
        sndgs.sso.set.iopcpuresamplequality = requested.iopcpuresamplequality;
        D_0050B9B0_0E28->b13 = requested.iopcpuresamplequality;
        if (sndgs.prevset.outputchannels != requested.outputchannels) {
            sndgs.sso.set.outputchannels = requested.outputchannels;
            for (i = 0; i < sndgs.voicestotal; i++) {
                if (sndgs.chan[i].status == 1 && sndgs.chan[i].masterchan < 0) {
                    f38E0_0E28(i);
                }
            }
        }
        return 0;
    }
    sndgs.voicestotal = requested.voicesmaincpu + requested.voicesiopcpu + requested.voicesspu;
    *(struct SNDSYSSET*)&sndgs.sso.set = requested;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDDRV_getEndedVoices);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct KV_1298 { int key; int val; };
struct T_1298 {
    char pad0[0x20];
    struct KV_1298 e[(0x1A0 - 0x20) / 8];
    struct { int a; int b; int c; int d; } f[1];
};
struct H_1298 {
    char pad0[0x83C];
    struct KV_1298 k[(0x9BC - 0x83C) / 8];
    struct KV_1298 k2[(0xCAC - 0x9BC) / 8];
    int padCAC;
    struct T_1298 *tbl;
};
struct G_1298 {
    char pad0[0x2F];
    unsigned char cnt2;
    unsigned char cnt1;
    char pad31[0x49 - 0x31];
    unsigned char mode;
    char pad4A[0x1F0 - 0x4A];
    struct CHANPUB *ents;
};
extern struct H_1298 D_0050AD00_1298 __asm__("D_0050AD00");
extern void f_A550_1298(int) __asm__("SNDVOICEI_free");

/**
 * Frees the driver voices whose sound has ended on the IOP.
 *
 * For each SPU voice (< +0x30 of sndgs) whose entry in the IOP voice block (+0xCB0 of D_0050AD00, +0x20 table) is
 * keyed and whose channel is a group master (+0x28 == -1): a key different from the EE copy (+0x83C) resets the copy's
 * remaining count to key & 0x7FFFFF, and a count of 0 clears every grouped voice's key (and the +0x18 twin in mode 2,
 * +0x49) and frees it with SNDVOICEI_free. The +0x2F mixer voices (table +0x1A0, copies +0x9BC) end on a count of -1.
 * Called by the sound thread SNDDRV_thread each tick.
 */
void SNDDRV_getEndedVoices(void) {
    int i;
    int j;
    int groupVoice;

    for (i = 0; i < sndgs.sso.set.voicesspu; i++) {
        if (D_0050AD00_1298.tbl->e[i].key != 0 && sndgs.chan[i].masterchan == -1) {
            if (D_0050AD00_1298.k[i].key != D_0050AD00_1298.tbl->e[i].key) {
                D_0050AD00_1298.k[i].val = D_0050AD00_1298.tbl->e[i].key & 0x7FFFFF;
            }
            if (D_0050AD00_1298.k[i].val == 0) {
                for (j = 0; j < sndgs.chan[i].numchan; j++) {
                    groupVoice = sndgs.chan[i].voices[j];
                    D_0050AD00_1298.tbl->e[groupVoice].key = 0;
                    f_A550_1298(groupVoice);
                    if (sndgs.sso.set.outputspdifmode == 2) {
                        D_0050AD00_1298.tbl->e[groupVoice + 0x18].key = 0;
                    }
                }
            }
        }
    }
    for (i = 0; i < sndgs.sso.set.voicesiopcpu; i++) {
        if (D_0050AD00_1298.tbl->f[i].a != 0 && sndgs.chan[i + sndgs.sso.set.voicesspu].masterchan == -1) {
            if (D_0050AD00_1298.k2[i].key != D_0050AD00_1298.tbl->f[i].a) {
                D_0050AD00_1298.k2[i].val = 0;
            }
            if (D_0050AD00_1298.k2[i].val == -1) {
                for (j = 0; j < sndgs.chan[i + sndgs.sso.set.voicesspu].numchan; j++) {
                    groupVoice = sndgs.chan[i + sndgs.sso.set.voicesspu].voices[j];
                    D_0050AD00_1298.tbl->f[groupVoice - sndgs.sso.set.voicesspu].a = 0;
                    f_A550_1298(groupVoice);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C1578);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0xC]; int v; int mode; } A_1578;
typedef struct { char pad[0x1AA4]; int a; char pad2[0x5C]; int b; } G_1578;
extern void func_003C4918_1578(int) __asm__("func_003C4918");
extern void iSignalSema(int);
extern G_1578 D_0050AD00_AD00 __asm__("D_0050AD00");

/**
 * Handles a SIF command from the IOP sound driver.
 *
 * Mode (+0x10) 2 passes the value (+0x0C) to func_003C4918, mode 0 stores it in +0x1B04 of D_0050AD00 and signals the
 * sound thread's semaphore (+0x1AA4) with iSignalSema; mode 3 and others are ignored. The platform init SNDPLATFORM_init
 * registers it as SIF command handler 1.
 *
 * @param command  the command packet
 */
void func_003C1578(A_1578 *command) {
    switch (command->mode) {
    case 2:
        func_003C4918_1578(command->v);
        return;
    case 3:
        break;
    case 0:
        D_0050AD00_AD00.b = command->v;
        iSignalSema(D_0050AD00_AD00.a);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C15D8);
#ifdef SKIP_ASM
/**
 * Appends the string @p src to the string @p dst (strcat without a return value).
 *
 * The platform init SNDPLATFORM_init builds the IOP module paths with it.
 *
 * @param dst  the string to extend
 * @param src  the string to append
 */
void func_003C15D8(char *dst, char *src) {
    while (*dst) dst++;
    while (*src) {
        *dst++ = *src++;
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C1638);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// FAKEMATCH: unused local `p` with a dead store `p = (int *)(buf + pos);` in the b49 & 2 block (the 0xC header it would point at is
// never written here); plain form 99.23%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct H_1638 {
    char pad0[0xCB0];
    char *cb0;
    char *cb4;
    char *cb8;
    char padCBC[0x1AB2 - 0xCBC];
    unsigned short pos;
    char *buf;
    int pad1AB8;
    char *chan[(0x1AF4 - 0x1ABC) / 4];
    char *x1AF4;
    char pad1AF8[0x1B00 - 0x1AF8];
    char *x1B00;
    int pad1B04;
    int x1B08;
    int x1B0C;
};
struct G_1638 {
    char pad0[0x2A];
    unsigned short rate;
    char pad2C[0x42 - 0x2C];
    unsigned char b42;
    char pad43[0x47 - 0x43];
    unsigned char b47;
    unsigned char b48;
    unsigned char b49;
};
extern struct H_1638 D_0050AD00_1638 __asm__("D_0050AD00");
extern char D_00512B80_1638[] __asm__("D_00512B80");
extern void *memset_1638(void *, int, int) __asm__("memset");
extern void f_BC00_1638(char *) __asm__("func_003CBC00");

/**
 * Lays out the shared EE-to-IOP command buffer the sound thread sends each tick.
 *
 * Uses D_00512B80 through its uncached mirror (+0x1AB4 of D_0050AD00), clears 11264 bytes and appends tagged blocks
 * (offset +0x1AB2): tag 0 with a 0xC-byte header (+0xCB4), tag 3 with the 672-byte voice block (+0xCB0, padded to 16),
 * tag 2 when +0x42 of sndgs is set (initialised by func_003CBC00, rate * 4 bytes), tag 1 with one PCM buffer of
 * (rate * 512 / 48000) samples per output channel (+0x47 + +0x48; a second buffer for mono) in +0x1ABC, tag 4 with a
 * 0x800-byte buffer (+0x1AF4) when option bit 2 (+0x49) is set, tag 5 with a 0xC-byte block (+0xCB8), and the end tag
 * 6. The total length goes to the header and +0x1B08; +0x1B00 gets the cached address. Called by SNDPLATFORM_init.
 */
void func_003C1638(void) {
    int rate;
    int len;
    int *sizeField;
    unsigned short *pcmSize;
    int n;
    int i;

    D_0050AD00_1638.buf = (char *)((int)D_00512B80_1638 | 0x20000000);
    memset_1638(D_0050AD00_1638.buf, 0, 11264);
    len = rate = (sndgs.sso.set.outputrate << 9) / 48000;
    D_0050AD00_1638.pos = 0;
    *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 0;
    D_0050AD00_1638.pos += 4;
    D_0050AD00_1638.cb4 = D_0050AD00_1638.buf + D_0050AD00_1638.pos;
    D_0050AD00_1638.pos += 0xC;
    *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 3;
    D_0050AD00_1638.pos += 4;
    sizeField = (int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos);
    D_0050AD00_1638.pos += 4;
    D_0050AD00_1638.cb0 = D_0050AD00_1638.buf + D_0050AD00_1638.pos;
    *sizeField = 672;
    D_0050AD00_1638.pos += 672;
    while (D_0050AD00_1638.pos & 0xF) {
        D_0050AD00_1638.pos++;
        (*sizeField)++;
    }
    if (sndgs.sso.set.routemaincpufxtoiopcpu) {
        *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 2;
        D_0050AD00_1638.pos += 0x10;
        f_BC00_1638(D_0050AD00_1638.buf + D_0050AD00_1638.pos);
        D_0050AD00_1638.pos += rate * 4;
    }
    *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 1;
    D_0050AD00_1638.pos += 4;
    pcmSize = (unsigned short *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos);
    D_0050AD00_1638.pos += 0xC;
    n = sndgs.sso.set.outputchannels + sndgs.sso.set.outputlfe;
    for (i = 0; i < n; i++) {
        D_0050AD00_1638.chan[i] = D_0050AD00_1638.buf + (D_0050AD00_1638.pos + i * len * 2);
    }
    *pcmSize = len * ((sndgs.sso.set.outputchannels + sndgs.sso.set.outputlfe) << 1);
    if (sndgs.sso.set.outputchannels == 1) {
        D_0050AD00_1638.chan[1] = D_0050AD00_1638.buf + (D_0050AD00_1638.pos + rate * 2);
        *pcmSize += rate * 2;
    }
    D_0050AD00_1638.pos += *pcmSize;
    if (sndgs.sso.set.outputspdifmode & 2) {
        *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 4;
        D_0050AD00_1638.pos += 4;
        sizeField = (int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos);
        D_0050AD00_1638.pos += 0xC;
        D_0050AD00_1638.x1AF4 = D_0050AD00_1638.buf + D_0050AD00_1638.pos;
        D_0050AD00_1638.pos += 0x800;
    }
    *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 5;
    D_0050AD00_1638.pos += 4;
    D_0050AD00_1638.cb8 = D_0050AD00_1638.buf + D_0050AD00_1638.pos;
    D_0050AD00_1638.pos += 0xC;
    *(int *)(D_0050AD00_1638.buf + D_0050AD00_1638.pos) = 6;
    D_0050AD00_1638.pos += 0x10;
    *(int *)(D_0050AD00_1638.cb4 + 4) = D_0050AD00_1638.pos;
    D_0050AD00_1638.x1B00 = D_00512B80_1638;
    D_0050AD00_1638.x1B0C = 0;
    D_0050AD00_1638.x1B08 = D_0050AD00_1638.pos;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C1980);
#ifdef SKIP_ASM
struct S_1980 { char pad[0x1C0]; int cnt; };
/**
 * VBlank interrupt handler that counts sound ticks: bumps +0x1C0 of sndgs, then sync and ei.
 *
 * The platform init SNDPLATFORM_init installs it on INTC 2 when no update rate was set (+0x34 == -1.0f).
 *
 * @return 0
 */
int func_003C1980(void) {
    sndgs.unk_1C0 = sndgs.unk_1C0 + 1;
    // PORT: MIPS sync + EE ei (no C equivalent)
    __asm__ volatile("sync\n\tei");
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDDRV_thread);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (disable / enable interrupts, pipeline sync, read the Status register).
#define DI_2B45_19A8() __asm__ volatile("di")

#define EI_2B45_19A8() __asm__ volatile("ei")

#define SYNCP_2B45_19A8() __asm__ volatile("sync.p")

#define MFC0_STATUS_2B45_19A8(v) __asm__ volatile("mfc0 %0, $12" : "=r"(v))


/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
struct sAD00Voice_19A8 { int key; int val; };

struct sAD00SpuVoice_19A8 {              /* 8 bytes, sAD00Spu_19A8.f20[] */
    int key;
    short pitch;
    signed char l;
    signed char r;
};

struct sAD00SpuChan_19A8 {               /* 16 bytes, sAD00Spu_19A8.f1A0[] */
    int key;
    short pitch;
    signed char vol[6];
    signed char f0C[4];
};

struct sAD00Spu_19A8 {                   /* 0x2A0, block 3 of the shared buffer (D_0050AD00.fCB0) */
    long f00;                       /* voice on bits */
    unsigned long f08;
    char f10;
    char f11;
    char f12;
    unsigned char f13;
    short f14;
    short f16;
    char pad18[0x20 - 0x18];
    struct sAD00SpuVoice_19A8 f20[48];
    struct sAD00SpuChan_19A8 f1A0[16];
};

struct sAD00Xfer_19A8 {                  /* 16 bytes, D_0050AD00.fCC8[] (also D_0050B9C8) */
    unsigned int key;
    int f04;
    int f08;
    unsigned short f0C;
    unsigned char f0E;
    char pad0F;
};

struct sAD00Spk_19A8 {                   /* 0x20, D_0050AD00.fE98[] (also D_0050BB98) */
    float f00[6];
    unsigned int f18;
    short f1C;
    short f1E;
};

struct sAD00Seg_19A8 { unsigned short a; unsigned short b; };

struct sAD00Port_19A8 { int f00; int f04; int pad08[2]; };

   /* 16 bytes, D_0050AD00.fE88[] */
struct sAD00_19A8 {
    char f000[0x83C];               /* RPC send buffer */
    struct sAD00Voice_19A8 f83C[48];
    struct sAD00Voice_19A8 f9BC[16];
    char padA3C[0xC80 - 0xA3C];
    char fC80[0x30];                /* RPC client data */
    struct sAD00Spu_19A8 *fCB0;          /* also D_0050B9B0 */
    int *fCB4;
    void *fCB8;
    int fCBC;              /* last transfer id */
    unsigned char fCC0;    /* queued transfers */
    unsigned char fCC1;    /* selected transfer */
    unsigned char fCC2;    /* transfer running */
    char padCC3;
    int fCC4;                       /* tick of the last transfer */
    struct sAD00Xfer_19A8 fCC8[28];
    struct sAD00Port_19A8 fE88[1];
    struct sAD00Spk_19A8 fE98[96];
    unsigned short f1A98;           /* number of f1AA0 segments */
    unsigned short f1A9A;
    unsigned short f1A9C;
    unsigned short f1A9E;
    struct sAD00Seg_19A8 *f1AA0;
    int f1AA4;
    int f1AA8;
    char f1AAC;
    signed char f1AAD;
    unsigned short f1AAE;
    short f1AB0;
    unsigned short f1AB2;           /* fill offset while func_003C1638 lays out f1AB4 */
    char *f1AB4;
    int f1AB8;
    void *f1ABC[6];
    int f1AD4;
    int f1AD8;
    char f1ADC[0x1AF4 - 0x1ADC];
    void *f1AF4;
    int f1AF8;
    int f1AFC;
    void *f1B00;
    int f1B04;
    int f1B08;
    int f1B0C;
    int f1B10;             /* lock: semaphore, owner thread, depth */
    int f1B14;
    int f1B18;
    int f1B1C[7];                   /* saved SPU2 DMA registers */
    int f1B38;
};

extern struct sAD00_19A8 D_0050AD00_19A8 __asm__("D_0050AD00");

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void (*gMutexLockFn_19A8)(void) __asm__("gMutexLockFn");

extern void *D_0050C848_19A8 __asm__("D_0050C848");

// (prototype from their definition at line 8102)
void func_003C09B8_19A8(void) __asm__("func_003C09B8");

// (prototype from their definition at line 1557)
void SNDSYSI_variabletimerservice_19A8(void) __asm__("SNDSYSI_variabletimerservice");

// (prototype from their definition at line 8119)
void func_003C0A60_19A8(void) __asm__("func_003C0A60");

// (prototype from their definition at line 7983)
/* PORT: PS2-only VU0 state save; func_0031A308 without the $vi22/$vi18
   saves, and with ctc2 (not cfc2) on $vi16 as in the shipped code. */
void func_003C0858_19A8(void* buf) __asm__("func_003C0858");

// (prototype from their definition at line 1578)
void SNDSYSI_timerservice_19A8(unsigned arg0) __asm__("SNDSYSI_timerservice");

// (prototype from their definition at line 8040)
/* PORT: PS2-only VU0 state restore; same as func_0031A3C0 without the
   $vi22/$vi18 restore. */
void func_003C08F8_19A8(void* buf) __asm__("func_003C08F8");

// (prototype from their definition at line 8288)
void SNDDRV_getEndedVoices_19A8(void) __asm__("SNDDRV_getEndedVoices");

extern int WaitSema_19A8(int) __asm__("WaitSema");
extern void MIX_audio_19A8(int *, int) __asm__("MIX_audio");
extern void Util_MemCpy(void *, void *, int);
extern void SNDPKTPLAYI_flushcallbackdata_19A8(void) __asm__("SNDPKTPLAYI_flushcallbackdata");
extern void FlushCache(int);
extern int sceSifSetDma(void *, int);
extern void (*gMutexUnlockFn)(void);
extern char D_0050C910[];
extern char D_0050C800[];

/**
 * The sound thread: mixes, encodes and sends one buffer to the IOP every time its semaphore is signalled.
 *
 * Waits on +0x1AA4 of D_0050AD00 and skips until started (+0x1AAD). Each pass: lock hook gMutexLockFn, pre-tick hook
 * +0x328 of sndgs, SNDSYSI_timerservice(94), +0x1C0 bumped when there is no VBlank handler, SNDSYSI_variabletimerservice (notify
 * callbacks), copies the IOP's 576-byte reply (+0x800) to +0xA40, frees ended voices (SNDDRV_getEndedVoices), saves VU0
 * (func_003C0858) and the scratchpad (func_003C09B8, when +0x1AAE), mixes into the PCM buffers (MIX_audio), runs
 * the DTS encoder hook D_0050C848 when option bit 2 (+0x49) is set or else copies channel 0 to channel 1 for mono
 * output, restores the scratchpad and VU0, runs SNDPKTPLAYI_flushcallbackdata, stamps the tick +0x330 into the buffer headers, flushes
 * the cache, starts the SIF DMA of D_0050C800 with interrupts off, then the post-tick hook +0x32C and the unlock hook
 * gMutexUnlockFn. Created by SNDPLATFORM_init; never returns.
 */
void SNDDRV_thread(void) {
    int s;

    for (;;) {
        WaitSema_19A8(D_0050AD00_19A8.f1AA4);
        if (D_0050AD00_19A8.f1AAD == 0) continue;
        gMutexLockFn_19A8();
        if ((void (*)(void))sndgs.profileenter != 0) ((void (*)(void))sndgs.profileenter)();
        SNDSYSI_timerservice_19A8(94);
        if (D_0050AD00_19A8.f1AA8 < 0) sndgs.unk_1C0++;
        SNDSYSI_variabletimerservice_19A8();
        Util_MemCpy(D_0050AD00_19A8.f000 + 0x800, (char *)&D_0050AD00_19A8 + 0xA40, 576);
        SNDDRV_getEndedVoices_19A8();
        func_003C0858_19A8(D_0050C910);
        if (D_0050AD00_19A8.f1AAE != 0) func_003C09B8_19A8();
        MIX_audio_19A8((int *)D_0050AD00_19A8.f1ABC, D_0050AD00_19A8.f1AB0);
        if (sndgs.sso.set.outputspdifmode & 2) {
            ((void (*)(void *, void *, int))D_0050C848_19A8)(D_0050AD00_19A8.f1ADC, D_0050AD00_19A8.f1AF4, D_0050AD00_19A8.f1AFC);
        } else if (sndgs.sso.set.outputchannels == 1) {
            Util_MemCpy(D_0050AD00_19A8.f1ABC[1], D_0050AD00_19A8.f1ABC[0], D_0050AD00_19A8.f1AB0 << 1);
        }
        if (D_0050AD00_19A8.f1AAE != 0) func_003C0A60_19A8();
        func_003C08F8_19A8(D_0050C910);
        SNDPKTPLAYI_flushcallbackdata_19A8();
        *D_0050AD00_19A8.fCB4 = sndgs.servertick;
        *(int *)D_0050AD00_19A8.fCB8 = sndgs.servertick;
        FlushCache(0);
        do {
            DI_2B45_19A8();
            SYNCP_2B45_19A8();
            MFC0_STATUS_2B45_19A8(s);
        } while (s & 0x10000);
        sceSifSetDma(D_0050C800, 1);
        EI_2B45_19A8();
        if ((void (*)(void))sndgs.profileleave != 0) ((void (*)(void))sndgs.profileleave)();
        gMutexUnlockFn();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm (EE di/ei interrupt mask); needs a C fallback off-PS2.
typedef struct {
    char pad0[0x8];
    unsigned long f8;
    char pad10[0x3];
    unsigned char f13;
} P_1B80;
typedef struct { int h; int pad4; int f8; int padC; } V_1B80;
typedef struct {
    char pad0[0xC80];
    char fC80[0x24];
    int fCA4;
    char padCA8[0xCB0 - 0xCA8];
    P_1B80 *fCB0;
    char padCB4[0xE8C - 0xCB4];
    V_1B80 v[1];
    char padE9C[0x1A9A - 0xE9C];
    short f1A9A;
    unsigned short f1A9C;
    char pad1A9E[0x1AA0 - 0x1A9E];
    int f1AA0;
    int f1AA4;
    int f1AA8;
    char pad1AAC;
    unsigned char f1AAD;
    unsigned short f1AAE;
    short f1AB0;
    char pad1AB2[0x1AD4 - 0x1AB2];
    int f1AD4;
    int f1AD8;
    int f1ADC;
    int f1AE0;
    int f1AE4;
    int f1AE8;
    int f1AEC;
    int f1AF0;
    char pad1AF4[0x1AF8 - 0x1AF4];
    int f1AF8;
    int f1AFC;
    char pad1B00[0x1B38 - 0x1B00];
    int f1B38;
} S_1B80;
typedef struct {
    char pad0[0x28];
    unsigned short f28;
    unsigned short f2A;
    unsigned short f2C;
    unsigned char f2E;
    unsigned char f2F;
    char pad30[0x34 - 0x30];
    float f34;
    char pad38[0x3F - 0x38];
    unsigned char f3F;
    char pad40;
    unsigned char f41;
    char pad42;
    unsigned char f43;
    char pad44[0x47 - 0x44];
    unsigned char f47;
    unsigned char f48;
    unsigned char f49;
    unsigned char f4A;
    char pad4B;
    unsigned char f4C;
    char pad4D;
    unsigned short f4E[(0x64 - 0x4E) / 2];
    int f64;
    unsigned int f68;
    char *f6C;
    char pad70[0x1C4 - 0x70];
    float f1C4;
} C_1B80;
typedef struct { int f0; int f4; int f8; } R_1B80;
typedef struct { int f0; void *f4; void *f8; int fC; void *f10; int f14; char pad18[0x20 - 0x18]; int f20; } T_1B80;
typedef struct {
    int f0;
    void *f4;
    int f8;
    unsigned short fC;
    unsigned short fE;
    unsigned char f10;
    unsigned char f11;
    unsigned char f12;
    unsigned char f13;
} K_1B80;
typedef struct { char pad[0x1098]; int f1098; } B_1B80;
typedef struct { char pad[2]; short f2; } M_1B80;
typedef struct {
    int f0;
    unsigned char f4;
    unsigned char f5;
    unsigned char f6;
    void *f8;
} L_1B80;

extern S_1B80 D_0050AD00_1B80 __asm__("D_0050AD00");
extern R_1B80 D_0050ACB0_1B80 __asm__("D_0050ACB0");
extern T_1B80 D_0050AC80_1B80 __asm__("D_0050AC80");
extern K_1B80 D_0050ACC8_1B80 __asm__("D_0050ACC8");
extern B_1B80 D_0050B740_1B80 __asm__("D_0050B740");
extern int (*D_0050C840_1B80)(int, int, int *, int *) __asm__("D_0050C840");
extern int (*D_0050C844_1B80)(int, int, int) __asm__("D_0050C844");
extern int D_0044DD40_1B80 __asm__("D_0044DD40");
extern int D_00512B20_1B80 __asm__("D_00512B20");
extern unsigned int D_00515D24_1B80 __asm__("D_00515D24");
extern char D_00495830_1B80[] __asm__("D_00495830");
extern char D_00495840_1B80[] __asm__("D_00495840");
extern char D_00495870_1B80[] __asm__("D_00495870");
extern char D_00495880_1B80[] __asm__("D_00495880");
extern char D_004A30F0_1B80[] __asm__("D_004A30F0");
extern char D_0050CB20_1B80[] __asm__("D_0050CB20");
extern char D_0050C880_1B80[] __asm__("D_0050C880");
extern char D_0050C900_1B80[] __asm__("D_0050C900");

extern void SNDSYS_entercritical_1B80(void) __asm__("SNDSYS_entercritical");
extern void SNDSYS_leavecritical_1B80(void) __asm__("SNDSYS_leavecritical");
extern int SNDMEMI_alloc_1B80(int) __asm__("SNDMEMI_alloc");
extern void func_003C0B10_1B80(int, void *, int) __asm__("func_003C0B10");
extern void func_003C15D8_1B80(char *, char *) __asm__("func_003C15D8");
extern void func_003C1638_1B80(int) __asm__("func_003C1638");
extern void func_003C35F8_1B80(int) __asm__("func_003C35F8");
extern void func_003C36C8_1B80(int, int) __asm__("func_003C36C8");
extern void func_003C7480_1B80(void) __asm__("func_003C7480");
extern void MIX_create_1B80(L_1B80 *) __asm__("MIX_create");
extern M_1B80 *sceGsGetGParam_1B80(void) __asm__("sceGsGetGParam");
extern void *memset_1B80(int, int, int) __asm__("memset");
extern int AddIntcHandler_1B80(int, void *, int) __asm__("AddIntcHandler");
extern int CreateThread_1B80(T_1B80 *) __asm__("CreateThread");
extern void StartThread_1B80(int, int) __asm__("StartThread");
extern int ChangeThreadPriority_1B80(int, int) __asm__("ChangeThreadPriority");
extern int GetThreadId_1B80(void) __asm__("GetThreadId");
extern int CreateSema_1B80(R_1B80 *) __asm__("CreateSema");
extern void DisableIntc_1B80(int) __asm__("DisableIntc");
extern void EnableIntc_1B80(int) __asm__("EnableIntc");
extern void sceSifSetCmdBuffer_1B80(void *, int) __asm__("sceSifSetCmdBuffer");
extern void sceSifAddCmdHandler_1B80(int, void *, void *) __asm__("sceSifAddCmdHandler");
extern void func_00426408_1B80(int) __asm__("func_00426408");
extern int sceSifBindRpc_1B80(void *, int, int) __asm__("sceSifBindRpc");
extern void sceSifInitIopHeap_1B80(void) __asm__("sceSifInitIopHeap");
extern int sceSifAllocIopHeap_1B80(int) __asm__("sceSifAllocIopHeap");
extern int sceSifLoadModule_1B80(char *, int, int) __asm__("sceSifLoadModule");
extern char SNDDRV_mixvoicefree_1B80[] __asm__("SNDDRV_mixvoicefree");
extern char func_003C1578_1B80[] __asm__("func_003C1578");
extern char func_003C1980_1B80[] __asm__("func_003C1980");
extern char SNDDRV_thread_1B80[] __asm__("SNDDRV_thread");

extern C_1B80 sndgs_1B80 __asm__("sndgs");
/**
 * Starts the PS2 sound platform: buffers, IOP modules and RPC, the sound thread and the timer.
 *
 * Allocates the sound-RAM segment table (+0x1AA0), picks the frame length (+0x1AB0: 0x100/0x180/0x200 samples at
 * 24/36/48 kHz), lays out the command buffer (func_003C1638). With option bit 2 (+0x49, DTS) it sizes and sets up the
 * encoder through the hooks D_0050C840 / D_0050C844 (mode 2 for 5 output channels, else 1; quality +0x4A), using the
 * caller's buffer +0x64 as work area when it is big enough, and allocates the per-channel input buffers. Creates the
 * thread semaphore, loads the IOP modules from the directory +0x6C (with fallbacks D_00495840 / D_00495880) unless done
 * before (D_0044DD40), allocates IOP memory, starts the sound thread SNDDRV_thread (priority 6, 0x6000 stack), sets up
 * the mixer (MIX_create) for a software output port, registers the SIF command handler func_003C1578, binds RPC
 * 0x534E44 ("SND"), installs the VBlank handler func_003C1980 (16.68 ms or 20 ms per tick) or uses 10.67 ms, sends the
 * init command (func_003C0B10, function 0) and marks itself started (+0x1AAD, D_0044DD40). Called by SNDSYSI_init.
 *
 * @return 0, or -1 when an IOP module fails to load or the RPC bind fails
 */
int SNDPLATFORM_init(void) {
    char path[0x80];
    L_1B80 st;
    int size0;
    int size1;
    int hz;
    int encoderState;
    int size;
    unsigned int base;
    unsigned int i;
    int stat;

    D_0050AD00_1B80.f1AAE = 0;
    SNDSYS_entercritical_1B80();
    D_0050AD00_1B80.f1AA0 = SNDMEMI_alloc_1B80(sndgs_1B80.f28 * 4);
    hz = sndgs_1B80.f2A;
    if (hz == 24000) {
        D_0050AD00_1B80.f1AB0 = 0x100;
    } else if (hz == 36000) {
        D_0050AD00_1B80.f1AB0 = 0x180;
    } else {
        D_0050AD00_1B80.f1AB0 = 0x200;
    }
    func_003C1638_1B80(hz);
    D_0050AD00_1B80.fCB0->f8 = 0xFFFFFFFFFFFFUL;
    if (sndgs_1B80.f49 & 2) {
        if (sndgs_1B80.f47 == 5) {
            D_0050C840_1B80(2, sndgs_1B80.f4A, &size0, &size1);
        } else {
            D_0050C840_1B80(1, sndgs_1B80.f4A, &size0, &size1);
        }
        if (sndgs_1B80.f64 != 0 && size1 < (int)sndgs_1B80.f68) {
            encoderState = SNDMEMI_alloc_1B80(size0);
            D_0050AD00_1B80.f1AF8 = encoderState;
            D_0050AD00_1B80.f1AFC = sndgs_1B80.f64;
            D_0050AD00_1B80.f1AAE = size1;
        } else {
            D_0050AD00_1B80.f1AF8 = SNDMEMI_alloc_1B80(size0 + size1);
            D_0050AD00_1B80.f1AFC = D_0050AD00_1B80.f1AF8 + size0;
        }
        if (sndgs_1B80.f47 == 5) {
            D_0050C844_1B80(2, sndgs_1B80.f4A, D_0050AD00_1B80.f1AF8);
        } else {
            D_0050C844_1B80(1, sndgs_1B80.f4A, D_0050AD00_1B80.f1AF8);
        }
        size = ((sndgs_1B80.f47 + sndgs_1B80.f48) << 10) + 0x40;
        D_0050AD00_1B80.f1AD4 = SNDMEMI_alloc_1B80(size);
        memset_1B80(D_0050AD00_1B80.f1AD4, 0, size);
        base = ((unsigned int)(D_0050AD00_1B80.f1AD4 + 0x3F) >> 6) << 6;
        D_0050AD00_1B80.f1AD8 = base;
        if (sndgs_1B80.f47 == 4) {
            D_0050AD00_1B80.f1AE0 = base;
            D_0050AD00_1B80.f1ADC = base + 0xC00;
            D_0050AD00_1B80.f1AE4 = base + 0x800;
            D_0050AD00_1B80.f1AE8 = base + 0x400;
        } else {
            D_0050AD00_1B80.f1ADC = base;
            D_0050AD00_1B80.f1AE0 = base + 0x1000;
            D_0050AD00_1B80.f1AE4 = base + 0x400;
            D_0050AD00_1B80.f1AE8 = base + 0xC00;
            D_0050AD00_1B80.f1AEC = base + 0x800;
            D_0050AD00_1B80.f1AF0 = base + 5120;
        }
    }
    func_003C36C8_1B80(0, 8);
    D_0050AD00_1B80.fCB0->f13 = sndgs_1B80.f41;
    D_0050AD00_1B80.f1A9A = 321;
    D_0050AD00_1B80.f1A9C = 0x8000;
    SNDSYS_leavecritical_1B80();
    D_0050ACB0_1B80.f4 = 1;
    D_0050ACB0_1B80.f8 = 0;
    D_0050AD00_1B80.f1AA4 = CreateSema_1B80(&D_0050ACB0_1B80);
    if (D_0044DD40_1B80 == 0) {
        func_00426408_1B80(0);
        sceSifInitIopHeap_1B80();
    }
    if (sndgs_1B80.f3F != 0 && D_0044DD40_1B80 == 0) {
        path[0] = 0;
        func_003C15D8_1B80(path, sndgs_1B80.f6C);
        func_003C15D8_1B80(path, D_00495830_1B80);
        if (sceSifLoadModule_1B80(path, 0, 0) < 0 && sceSifLoadModule_1B80(D_00495840_1B80, 0, 0) < 0) {
            return -1;
        }
        path[0] = 0;
        func_003C15D8_1B80(path, sndgs_1B80.f6C);
        func_003C15D8_1B80(path, D_00495870_1B80);
        if (sceSifLoadModule_1B80(path, 0, 0) < 0 && sceSifLoadModule_1B80(D_00495880_1B80, 0, 0) < 0) {
            return -1;
        }
    }
    for (i = 0; i < 1; i++) {
        D_0050AD00_1B80.v[i].h = sceSifAllocIopHeap_1B80(0x1000);
        D_0050AD00_1B80.v[i].f8 = 0;
    }
    D_0050AC80_1B80.f4 = SNDDRV_thread_1B80;
    D_0050AC80_1B80.f8 = D_0050CB20_1B80;
    D_0050AC80_1B80.fC = 0x6000;
    D_0050AC80_1B80.f14 = 6;
    D_0050AC80_1B80.f10 = D_004A30F0_1B80;
    D_0050AC80_1B80.f20 = 0;
    D_00512B20_1B80 = CreateThread_1B80(&D_0050AC80_1B80);
    StartThread_1B80(D_00512B20_1B80, 0);
    ChangeThreadPriority_1B80(GetThreadId_1B80(), 100);
    func_003C35F8_1B80(8);
    func_003C35F8_1B80(4);
    for (i = 0; i < sndgs_1B80.f4C; i++) {
        if (sndgs_1B80.f4E[i] & 4) {
            st.f5 = sndgs_1B80.f47 + sndgs_1B80.f48;
            st.f4 = sndgs_1B80.f2E;
            st.f0 = sndgs_1B80.f2A;
            st.f8 = SNDDRV_mixvoicefree_1B80;
            st.f6 = 0;
            MIX_create_1B80(&st);
            break;
        }
    }
    if (sndgs_1B80.f64 != 0 && sndgs_1B80.f68 - D_00515D24_1B80 > D_0050AD00_1B80.f1AAE) {
        D_0050AD00_1B80.f1AAE = sndgs_1B80.f68 - D_00515D24_1B80;
    }
    if (D_0050AD00_1B80.f1AAE != 0) {
        SNDSYS_entercritical_1B80();
        D_0050AD00_1B80.f1B38 = SNDMEMI_alloc_1B80(D_0050AD00_1B80.f1AAE + 0x10);
        SNDSYS_leavecritical_1B80();
    }
    if (D_0044DD40_1B80 == 0) {
        do {
            __asm__ volatile("di\n\tsync.p\n\tmfc0 %0, $12" : "=r"(stat));
        } while (stat & 0x10000);
        sceSifSetCmdBuffer_1B80(D_0050C880_1B80, 0x10);
        sceSifAddCmdHandler_1B80(1, func_003C1578_1B80, D_0050C900_1B80);
        __asm__ volatile("ei");
        do {
            if (sceSifBindRpc_1B80(D_0050AD00_1B80.fC80, 0x534E44, 0) < 0) {
                return -1;
            }
        } while (D_0050AD00_1B80.fCA4 == 0);
    }
    if (sndgs_1B80.f34 == -1.0f) {
        sndgs_1B80.f1C4 = (sceGsGetGParam_1B80()->f2 == 2) ? 16.683349609375f : 20.0f;
        DisableIntc_1B80(2);
        D_0050AD00_1B80.f1AA8 = AddIntcHandler_1B80(2, func_003C1980_1B80, 0);
        EnableIntc_1B80(2);
    } else {
        sndgs_1B80.f1C4 = 10.666666984558105f;
        D_0050AD00_1B80.f1AA8 = -1;
    }
    D_0050ACC8_1B80.f0 = 0x80303;
    D_0050ACC8_1B80.f4 = &D_0050B740_1B80;
    D_0050ACC8_1B80.f8 = D_0050B740_1B80.f1098;
    D_0050ACC8_1B80.f10 = sndgs_1B80.f47;
    D_0050ACC8_1B80.f11 = sndgs_1B80.f48;
    D_0050ACC8_1B80.f12 = sndgs_1B80.f2F;
    D_0050ACC8_1B80.fC = sndgs_1B80.f2A;
    D_0050ACC8_1B80.fE = sndgs_1B80.f2C;
    if (sndgs_1B80.f47 >= 4) {
        D_0050ACC8_1B80.f13 = sndgs_1B80.f43;
    } else {
        D_0050ACC8_1B80.f13 = 0;
    }
    func_003C0B10_1B80(0, &D_0050ACC8_1B80, 20);
    D_0050AD00_1B80.f1AAD = D_0044DD40_1B80 = 1;
    func_003C7480_1B80();
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x176];
    unsigned char f176;
    char pad177[0x180 - 0x177];
    unsigned int f180;
} G_2268;
typedef struct { int h; int pad[3]; } V_2268;
typedef struct {
    char pad0[0xE8C];
    V_2268 v[1];
    char padE9C[0x1AA0 - 0xE9C];
    int f1AA0;
    int f1AA4;
    int f1AA8;
    char pad1AAC;
    unsigned char f1AAD;
    char pad1AAE[0x1AD4 - 0x1AAE];
    int f1AD4;
    int f1AD8;
    char pad1ADC[0x1AF8 - 0x1ADC];
    int f1AF8;
    char pad1AFC[0x1B38 - 0x1AFC];
    int f1B38;
} H_2268;
extern H_2268 D_0050AD00_2268 __asm__("D_0050AD00");
extern int D_00512B20_2268[] __asm__("D_00512B20");
extern void SNDSYS_leavecritical_2268(void) __asm__("SNDSYS_leavecritical");
extern void SNDSYS_entercritical_2268(void) __asm__("SNDSYS_entercritical");
extern void SNDMEMI_free_2268(int) __asm__("SNDMEMI_free");
extern void func_003C0B10_2268(int, int, int) __asm__("func_003C0B10");
extern void SNDSYS_service_2268(void) __asm__("SNDSYS_service");
extern void MIX_destroy_2268(void) __asm__("MIX_destroy");
extern void func_003CBC00_2268(int) __asm__("func_003CBC00");
extern int RemoveIntcHandler_2268(int, int) __asm__("RemoveIntcHandler");
extern void DeleteThread_2268(int) __asm__("DeleteThread");
extern void TerminateThread_2268(int) __asm__("TerminateThread");
extern void ReferThreadStatus_2268(int, void *) __asm__("ReferThreadStatus");
extern void DeleteSema_2268(int) __asm__("DeleteSema");
extern void DisableIntc_2268(int) __asm__("DisableIntc");
extern void EnableIntc_2268(int) __asm__("EnableIntc");
extern void func_0042AD08_2268(int) __asm__("func_0042AD08");

/**
 * Shuts the PS2 sound platform down: everything SNDPLATFORM_init set up.
 *
 * Leaves any critical sections still held (+0x176 of sndgs), lets 10 ticks pass (SNDSYS_service until +0x180 has
 * advanced), removes the VBlank handler (+0x1AA8 of D_0050AD00) with INTC 2 masked around it, sends RPC function 1
 * (func_003C0B10), deletes the sound thread (terminating it unless it is dormant, status 0x10) and the semaphore
 * +0x1AA4, frees the IOP heap block (func_0042AD08), resets func_003CBC00, clears the started flag +0x1AAD and, in a
 * critical section, frees the DTS buffers (+0x1AD4, +0x1AF8), the scratchpad save buffer (+0x1B38), the mixer
 * (MIX_destroy) and the sound-RAM segment table (+0x1AA0). Called by SNDSYS_restore and by SNDSYSI_init when the
 * start fails.
 *
 * @return 0
 */
int SNDPLATFORM_restore(void) {
    int st[12];
    unsigned int targetTick;
    int i;

    while (sndgs.incritical != 0) {
        SNDSYS_leavecritical_2268();
    }
    targetTick = sndgs.audiotick + 10;
    while (sndgs.audiotick < targetTick) {
        SNDSYS_service_2268();
    }
    if (D_0050AD00_2268.f1AA8 >= 0) {
        DisableIntc_2268(2);
        if (RemoveIntcHandler_2268(2, D_0050AD00_2268.f1AA8) > 0) {
            EnableIntc_2268(2);
        }
    }
    func_003C0B10_2268(1, 0, 0);
    ReferThreadStatus_2268(D_00512B20_2268[0], st);
    if (st[0] == 0x10) {
        DeleteThread_2268(D_00512B20_2268[0]);
    } else {
        TerminateThread_2268(D_00512B20_2268[0]);
        DeleteThread_2268(D_00512B20_2268[0]);
    }
    DeleteSema_2268(D_0050AD00_2268.f1AA4);
    for (i = 0; i < 1; i++) {
        func_0042AD08_2268(D_0050AD00_2268.v[i].h);
    }
    func_003CBC00_2268(0);
    D_0050AD00_2268.f1AAD = 0;
    SNDSYS_entercritical_2268();
    if (D_0050AD00_2268.f1AD4 != 0) {
        SNDMEMI_free_2268(D_0050AD00_2268.f1AD4);
        D_0050AD00_2268.f1AD4 = 0;
        D_0050AD00_2268.f1AD8 = 0;
    }
    if (D_0050AD00_2268.f1AF8 != 0) {
        SNDMEMI_free_2268(D_0050AD00_2268.f1AF8);
        D_0050AD00_2268.f1AF8 = 0;
    }
    if (D_0050AD00_2268.f1B38 != 0) {
        SNDMEMI_free_2268(D_0050AD00_2268.f1B38);
        D_0050AD00_2268.f1B38 = 0;
    }
    MIX_destroy_2268();
    SNDMEMI_free_2268(D_0050AD00_2268.f1AA0);
    SNDSYS_leavecritical_2268();
    return 0;
}
#endif

/**
 * Does nothing: the platform's per-tick service hook, empty on PS2.
 *
 * SNDSYSI_100hzserver calls it first on every tick.
 */
void iSNDserve(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_stop);
#ifdef SKIP_ASM
struct G_2440 { char pad[0x2F]; unsigned char b2F; unsigned char b30; char pad1[0x49 - 0x31]; unsigned char mode; char pad2[0x1F0 - 0x4A]; struct CHANPUB *ents; };
struct P_2440 { int v; int pad; };
struct Q_2440 { int v; int pad[3]; };
struct V_2440 { char pad[0x20]; struct P_2440 a[0x30]; };
struct V2_2440 { char pad[0x1A0]; struct Q_2440 a[0x30]; };

struct H_2440 { char pad[0xCB0]; struct V_2440 *m; };
extern struct H_2440 D_0050AD00_2440 __asm__("D_0050AD00");
extern void f_A550_2440(int) __asm__("SNDVOICEI_free");
extern void f_8468_2440(int) __asm__("MIX_stop");

/**
 * Stops all voices of a channel and frees them.
 *
 * For each voice in the channel's list (+0x04, count +0x23 of the sndgs channel table): render kind 8 (SPU voices,
 * +0x24) clears its key in the IOP voice block (+0xCB0 of D_0050AD00, and the +0x18 twin when +0x49 is 2), kind 0x100
 * clears its key in the block's +0x1A0 table, and mixer voices are stopped with MIX_stop; every voice is then
 * freed with SNDVOICEI_free. Called by SNDstop and SNDrelease.
 *
 * @param idx  the channel index
 * @return 0
 */
int SNDPLATFORM_stop(int idx) {
    struct CHANPUB *channel = &sndgs.chan[idx];
    int i;
    if (channel->rendermode & 8) {
        for (i = 0; i < channel->numchan; i++) {
            D_0050AD00_2440.m->a[channel->voices[i]].v = 0;
            f_A550_2440(channel->voices[i]);
            if (sndgs.sso.set.outputspdifmode == 2)
                D_0050AD00_2440.m->a[channel->voices[i] + 0x18].v = 0;
        }
    } else if (channel->rendermode & 0x100) {
        for (i = 0; i < channel->numchan; i++) {
            ((struct V2_2440 *)D_0050AD00_2440.m)->a[channel->voices[i] - sndgs.sso.set.voicesspu].v = 0;
            f_A550_2440(channel->voices[i]);
        }
    } else {
        for (i = 0; i < channel->numchan; i++) {
            f_8468_2440(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu));
            f_A550_2440(channel->voices[i]);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_setpitch);
#ifdef SKIP_ASM
struct G_25E0 {
    char pad000[0x2A];
    unsigned short f2A;
    unsigned short f2C;
    char pad02E;
    unsigned char f2F;
    unsigned char f30;
    char pad031[0x49 - 0x31];
    unsigned char f49;
    char pad04A[0x1F0 - 0x4A];
    struct CHANPUB *f1F0;
};
struct A8_25E0 {
    int a;
    short v;
    short b;
};
struct B16_25E0 {
    int a;
    short v;
    char pad[10];
};
struct T_25E0 {
    char pad[0x20];
    struct A8_25E0 a[48];
    struct B16_25E0 b[1];
};
struct H_25E0 {
    char pad[0xCB0];
    struct T_25E0 *tbl;
};
extern struct H_25E0 D_0050AD00_25E0 __asm__("D_0050AD00");
extern void MIX_setpitch_25E0(int a, unsigned int v) __asm__("MIX_setpitch");

/**
 * Sends a channel's current pitch to each of its voices.
 *
 * The pitch is the channel's sample rate (+0x20) << 12 divided by the output rate, times the 4.12 pitch +0x8A. Render
 * kind 8 (SPU) uses 48000, shifts back by 12 and caps it at 0x3FFF, storing it in each voice's entry of the IOP voice
 * block (+0xCB0 of D_0050AD00, plus the +0x18 twin when +0x49 is 2); kind 0x100 divides by +0x2C into the block's
 * +0x1A0 table; mixer voices divide by +0x2A, shift by 8 and go through MIX_setpitch. Called by SNDSYSI_100hzserver,
 * SNDpitchbend, SNDpitchmult, SNDPLATFORM_playtimbre and SNDPLATFORM_packetplay.
 *
 * @param idx  the channel index
 * @return 0
 */
int SNDPLATFORM_setpitch(int idx) {
    struct CHANPUB *channel = &sndgs.chan[idx];
    unsigned int hwPitch;
    int i;

    if (channel->rendermode & 8) {
        hwPitch = (unsigned int)((channel->samplerate << 12) / 48000 * channel->finalpitch) >> 12;
        if (hwPitch >= 0x4000) {
            hwPitch = 0x3FFF;
        }
        for (i = 0; i < channel->numchan; i++) {
            D_0050AD00_25E0.tbl->a[channel->voices[i]].v = hwPitch;
            if (sndgs.sso.set.outputspdifmode == 2) {
                D_0050AD00_25E0.tbl->a[channel->voices[i] + 0x18].v = hwPitch;
            }
        }
    } else if (channel->rendermode & 0x100) {
        hwPitch = (unsigned int)((channel->samplerate << 12) / sndgs.sso.set.outputrateiopcpu * channel->finalpitch) >> 12;
        for (i = 0; i < channel->numchan; i++) {
            D_0050AD00_25E0.tbl->b[channel->voices[i] - sndgs.sso.set.voicesspu].v = hwPitch;
        }
    } else {
        hwPitch = (unsigned int)((channel->samplerate << 12) / sndgs.sso.set.outputrate * channel->finalpitch) >> 8;
        for (i = 0; i < channel->numchan; i++) {
            MIX_setpitch_25E0(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), hwPitch);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_setfxlevel);
#ifdef SKIP_ASM
struct V_27E0 { char pad[0x1AC]; char v[1][16]; };
struct G_27E0 { char pad[0x2F]; unsigned char base2; unsigned char base; char pad31[0x49 - 0x31]; unsigned char mode; char pad4A[0x1F0 - 0x4A]; struct CHANPUB *chans; };
struct A_27E0 { char pad[0xCB0]; long *mask; };
extern struct A_27E0 D_0050AD00_27E0 __asm__("D_0050AD00");
extern void f_8598_27E0(int, int, float) __asm__("SNDMIX_setwetgain");

/**
 * Applies a channel's effect-send level for bus @p k to its voices.
 *
 * Render kind 8 (SPU): sets or clears each voice's bit (and its +0x18 twin when +0x49 is 2) in the effect mask at the
 * start of the IOP voice block (+0xCB0 of D_0050AD00), on when the bus-0 send (+0x62) is non-zero. Kind 0x100: stores
 * volume * send * 0.003876 into byte @p k of the voice's entry at +0x1AC. Mixer voices: SNDMIX_setwetgain with volume *
 * send / 32767. Called by SNDvol, func_003B88C8 (send level), SNDPLATFORM_playtimbre and SNDPLATFORM_packetplay.
 *
 * @param id  the channel index
 * @param k  the effect bus
 * @return 0
 */
int SNDPLATFORM_setfxlevel(int id, int k) {
    struct CHANPUB *ch;
    int i, sendLevel;
    float sendGain;

    ch = &((struct CHANPUB*)sndgs.chan)[id];
    if (ch->rendermode & 8) {
        for (i = 0; i < ch->numchan; i++) {
            if (ch->unk_62[0] != 0) {
                *D_0050AD00_27E0.mask |= 1L << ch->voices[i];
            } else {
                *D_0050AD00_27E0.mask &= ~(1L << ch->voices[i]);
            }
            if (sndgs.sso.set.outputspdifmode == 2) {
                if (ch->unk_62[0] != 0) {
                    *D_0050AD00_27E0.mask |= 1L << (ch->voices[i] + 0x18);
                } else {
                    *D_0050AD00_27E0.mask &= ~(1L << (ch->voices[i] + 0x18));
                }
            }
        }
    } else if (ch->rendermode & 0x100) {
        sendLevel = ch->finalvol * ch->unk_62[k] * 0.003875850699841976f;
        for (i = 0; i < ch->numchan; i++) {
            ((struct V_27E0 *)D_0050AD00_27E0.mask)->v[ch->voices[i] - sndgs.sso.set.voicesspu][k] = sendLevel;
        }
    } else {
        sendGain = ch->finalvol * ch->unk_62[k] / 32767.0f;
        for (i = 0; i < ch->numchan; i++) {
            f_8598_27E0(ch->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), k, sendGain);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C2A50);
#ifdef SKIP_ASM
struct G_2A50 {
    char pad0[0x2F];
    unsigned char base2;
    unsigned char base;
    char pad31[0x47 - 0x31];
    unsigned char b47;
    unsigned char b48;
    unsigned char mode;
    char pad4A[0x1F0 - 0x4A];
    struct CHANPUB *chans;
};
struct W8_2A50 { char pad[6]; signed char l; signed char r; };
struct W16_2A50 { char pad[6]; signed char c[10]; };
struct T_2A50 {
    char pad0[0x20];
    struct W8_2A50 a[48];
    struct W16_2A50 b[1];
};
struct Spk_2A50 { float p[5]; char pad14[0x20 - 0x14]; };
struct A_2A50 {
    char pad0[0xCB0];
    struct T_2A50 *tbl;
    char padCB4[0xE98 - 0xCB4];
    struct Spk_2A50 spk[1];
};
extern struct A_2A50 D_0050AD00_2A50 __asm__("D_0050AD00");
extern struct Spk_2A50 D_0050BB98_2A50[] __asm__("D_0050BB98");
extern void f8560_2A50(int, int, float) __asm__("SNDMIX_setdrygain");

/**
 * Applies a channel's volume and speaker gains to each of its voices.
 *
 * Uses the voice's speaker gains (+0xE98 of D_0050AD00, D_0050BB98, six floats) and the channel volume (float). Render
 * kind 8 (SPU): left/right = gain[1] / gain[0] * volume * 127 in the IOP voice block; with +0x49 == 2 each voice and
 * its +0x18 twin carry two speaker pairs (gains 3,0 and 2,1 for four outputs, 4,1 and 3,2 otherwise). Kind 0x100 writes
 * volume * dry level (+0x5F / 127) * gain[j] * 127 for every output j (+0x47 + +0x48) to the +0x1A0 table; mixer voices
 * get the same factor per output through SNDMIX_setdrygain. Called by SNDSYSI_100hzserver, SNDmastervol, SNDvol,
 * func_003C2EC8 and func_003C38E0.
 *
 * @param id  the channel index
 */
void func_003C2A50(int id) {
    struct CHANPUB *ch;
    float sc;
    int i;
    int j;
    int outputCount;

    ch = &((struct CHANPUB*)sndgs.chan)[id];
    if (ch->rendermode & 8) {
        for (i = 0; i < ch->numchan; i++) {
            if (sndgs.sso.set.outputspdifmode != 2) {
                D_0050AD00_2A50.tbl->a[ch->voices[i]].l = D_0050AD00_2A50.spk[ch->voices[i]].p[1] * ch->finalvol * 127.0f;
                D_0050AD00_2A50.tbl->a[ch->voices[i]].r = D_0050AD00_2A50.spk[ch->voices[i]].p[0] * ch->finalvol * 127.0f;
            } else {
                if (sndgs.sso.set.outputchannels == 4) {
                    D_0050AD00_2A50.tbl->a[ch->voices[i]].l = D_0050AD00_2A50.spk[ch->voices[i]].p[3] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i]].r = D_0050AD00_2A50.spk[ch->voices[i]].p[0] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i] + 0x18].l = D_0050AD00_2A50.spk[ch->voices[i]].p[2] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i] + 0x18].r = D_0050AD00_2A50.spk[ch->voices[i]].p[1] * ch->finalvol * 127.0f;
                } else {
                    D_0050AD00_2A50.tbl->a[ch->voices[i]].l = D_0050AD00_2A50.spk[ch->voices[i]].p[4] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i]].r = D_0050AD00_2A50.spk[ch->voices[i]].p[1] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i] + 0x18].l = D_0050AD00_2A50.spk[ch->voices[i]].p[3] * ch->finalvol * 127.0f;
                    D_0050AD00_2A50.tbl->a[ch->voices[i] + 0x18].r = D_0050AD00_2A50.spk[ch->voices[i]].p[2] * ch->finalvol * 127.0f;
                }
            }
        }
    } else if (ch->rendermode & 0x100) {
        outputCount = sndgs.sso.set.outputchannels + sndgs.sso.set.outputlfe;
        sc = ch->finalvol * (float)ch->drylevel * 0.0078740157186985016f;
        for (i = 0; i < ch->numchan; i++) {
            for (j = 0; j < outputCount; j++) {
                D_0050AD00_2A50.tbl->b[ch->voices[i] - sndgs.sso.set.voicesspu].c[j] = sc * D_0050AD00_2A50.spk[ch->voices[i]].p[j] * 127.0f;
            }
        }
    } else {
        outputCount = sndgs.sso.set.outputchannels + sndgs.sso.set.outputlfe;
        sc = ch->finalvol * (float)ch->drylevel * 0.0078740157186985016f;
        for (i = 0; i < ch->numchan; i++) {
            for (j = 0; j < outputCount; j++) {
                f8560_2A50(ch->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), j, sc * D_0050BB98_2A50[ch->voices[i]].p[j]);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C2EC8);
#ifdef SKIP_ASM
struct G_2EC8 { char pad[0x49]; unsigned char mode; char pad2[0x1F0 - 0x4A]; struct CHANPUB *ents; };
struct M_2EC8 { char pad[8]; long mask; };
struct H_2EC8 { char pad[0xCB0]; struct M_2EC8 *m; };
extern struct H_2EC8 D_0050AD00_2EC8 __asm__("D_0050AD00");
extern void f_2A50_2EC8(int) __asm__("func_003C2A50");

/**
 * Applies a channel's dry level to its voices.
 *
 * Render kind 8 (SPU): sets or clears each voice's bit (and the +0x18 twin when +0x49 is 2) in the dry mask at +0x08 of
 * the IOP voice block, on when the dry level +0x5F is non-zero. Other kinds fold the dry level into the volume, so they
 * recompute it with func_003C2A50. Called by func_003B8700 (dry level) and SNDPLATFORM_playtimbre.
 *
 * @param idx  the channel index
 */
void func_003C2EC8(int idx) {
    struct CHANPUB *channel = &sndgs.chan[idx];
    int i;
    if (channel->rendermode & 8) {
        for (i = 0; i < channel->numchan; i++) {
            if (channel->drylevel) D_0050AD00_2EC8.m->mask |= 1L << channel->voices[i];
            else D_0050AD00_2EC8.m->mask &= ~(1L << channel->voices[i]);
            if (sndgs.sso.set.outputspdifmode == 2) {
                if (channel->drylevel) D_0050AD00_2EC8.m->mask |= 1L << (channel->voices[i] + 0x18);
                else D_0050AD00_2EC8.m->mask &= ~(1L << (channel->voices[i] + 0x18));
            }
        }
    } else if (channel->rendermode & 0x100) {
        f_2A50_2EC8(idx);
    } else {
        f_2A50_2EC8(idx);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_timemult);
#ifdef SKIP_ASM
extern void MIX_settimemult(int, int);
struct G_3010 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };

/**
 * Applies a time multiplier to a channel's software-mixer voices.
 *
 * Only render kind 4 (mixer) channels: each voice, as a mixer index (driver voice minus +0x30 and +0x2F of sndgs),
 * goes to MIX_settimemult. Called by SNDCTRL_timemult, SNDPLATFORM_playtimbre and SNDPLATFORM_packetplay.
 *
 * @param channelIndex  the channel index
 * @param timeMult  the time multiplier (0x1000 = 1.0)
 * @return 0
 */
int SNDPLATFORM_timemult(int channelIndex, int timeMult) {
    struct CHANPUB *channel;
    int i;
    channel = &sndgs.chan[channelIndex];
    if (channel->rendermode & 4) {
        for (i = 0; i < channel->numchan; i++)
            MIX_settimemult(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), timeMult);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C30C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C8B80(int, int);
struct G_30C8 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };

/**
 * Sets the clip level of a channel's software-mixer voices (a clipping, distortion-like filter).
 *
 * Only render kind 4 (mixer) channels: each voice (as a mixer index) goes to func_003C8B80, which adds the voice's
 * 80-byte clipping node for a positive value (or updates it) and removes it for 0 or less. SNDPLATFORM_playtimbre passes its
 * fifth argument (between the time multiplier and the low-pass cutoff); SNDPLATFORM_packetplay calls it too.
 *
 * @param channelIndex  the channel index
 * @param amount  the clip level (0 or less removes the filter)
 */
void func_003C30C8(int channelIndex, int amount) {
    struct CHANPUB *channel;
    int i;
    channel = &sndgs.chan[channelIndex];
    if (channel->rendermode & 4) {
        for (i = 0; i < channel->numchan; i++)
            func_003C8B80(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), amount);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_lowpass);
#ifdef SKIP_ASM
extern void func_003C8D00_3178(int, float) __asm__("func_003C8D00");

/**
 * Sets the low-pass cutoff of a channel's software-mixer voices.
 *
 * Only render kind 4 (mixer) channels: the cutoff is divided by half the mixer rate (+0x2A of sndgs) and passed
 * with each voice's mixer index to func_003C8D00. Called by func_003B8978, SNDPLATFORM_playtimbre and SNDPLATFORM_packetplay.
 *
 * @param idx  the channel index
 * @param cutoff  the cutoff frequency in Hz
 */
void SNDPLATFORM_lowpass(int idx, int cutoff) {
    char *channel = (char*)&((struct CHANPUB*)sndgs.chan)[idx];
    int i;
    short *voiceList;
    float normCutoff;

    if (*(unsigned short *)(channel + 0x24) & 4) {
        normCutoff = (float)cutoff * (1.0f / ((float)sndgs.sso.set.outputrate * 0.5f));
        i = 0;
        if (i < *(unsigned char *)(channel + 0x23)) {
            voiceList = (short *)(channel + 4);
            do {
                func_003C8D00_3178(*voiceList++ - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), normCutoff);
            } while (++i < *(unsigned char *)(channel + 0x23));
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_highpass);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MIX_sethighpass(int, int);
struct G_3250 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };

/**
 * Sets the high-pass cutoff of a channel's software-mixer voices.
 *
 * Only render kind 4 (mixer) channels: each voice's mixer index and the cutoff go to MIX_sethighpass, which adds (or
 * updates) the voice's 288-byte filter for a positive cutoff and removes it for 0. Called by SNDPLATFORM_playtimbre and
 * SNDPLATFORM_packetplay.
 *
 * @param channelIndex  the channel index
 * @param cutoff  the cutoff (0 or less removes the filter)
 */
void SNDPLATFORM_highpass(int channelIndex, int cutoff) {
    struct CHANPUB *channel;
    int i;
    channel = &sndgs.chan[channelIndex];
    if (channel->rendermode & 4) {
        for (i = 0; i < channel->numchan; i++)
            MIX_sethighpass(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), cutoff);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_mutexalloc);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
struct sAD00Voice { int key; int val; };

struct sAD00SpuVoice {              /* 8 bytes, sAD00Spu.f20[] */
    int key;
    short pitch;
    signed char l;
    signed char r;
};

struct sAD00SpuChan {               /* 16 bytes, sAD00Spu.f1A0[] */
    int key;
    short pitch;
    signed char vol[6];
    signed char f0C[4];
};

struct sAD00Spu {                   /* 0x2A0, block 3 of the shared buffer (D_0050AD00.fCB0) */
    long f00;                       /* voice on bits */
    unsigned long f08;
    char f10;
    char f11;
    char f12;
    unsigned char f13;
    short f14;
    short f16;
    char pad18[0x20 - 0x18];
    struct sAD00SpuVoice f20[48];
    struct sAD00SpuChan f1A0[16];
};

struct sAD00Xfer {                  /* 16 bytes, D_0050AD00.fCC8[] (also D_0050B9C8) */
    unsigned int key;
    int f04;
    int f08;
    unsigned short f0C;
    unsigned char f0E;
    char pad0F;
};

struct sAD00Spk {                   /* 0x20, D_0050AD00.fE98[] (also D_0050BB98) */
    float f00[6];
    unsigned int f18;
    short f1C;
    short f1E;
};

struct sAD00Seg { unsigned short a; unsigned short b; };

struct sAD00Port { int f00; int f04; int pad08[2]; };   /* 16 bytes, D_0050AD00.fE88[] */

struct sAD00 {
    char f000[0x83C];               /* RPC send buffer */
    struct sAD00Voice f83C[48];
    struct sAD00Voice f9BC[16];
    char padA3C[0xC80 - 0xA3C];
    char fC80[0x30];                /* RPC client data */
    struct sAD00Spu *fCB0;          /* also D_0050B9B0 */
    int *fCB4;
    void *fCB8;
    volatile int fCBC;              /* last transfer id */
    volatile unsigned char fCC0;    /* queued transfers */
    volatile unsigned char fCC1;    /* selected transfer */
    volatile unsigned char fCC2;    /* transfer running */
    char padCC3;
    int fCC4;                       /* tick of the last transfer */
    struct sAD00Xfer fCC8[28];
    struct sAD00Port fE88[1];
    struct sAD00Spk fE98[96];
    unsigned short f1A98;           /* number of f1AA0 segments */
    unsigned short f1A9A;
    unsigned short f1A9C;
    unsigned short f1A9E;
    struct sAD00Seg *f1AA0;
    int f1AA4;
    int f1AA8;
    char f1AAC;
    unsigned char f1AAD;
    unsigned short f1AAE;
    char pad1AB0[0x1AB2 - 0x1AB0];
    unsigned short f1AB2;           /* fill offset while func_003C1638 lays out f1AB4 */
    char *f1AB4;
    int f1AB8;
    void *f1ABC[6];
    int f1AD4;
    int f1AD8;
    char pad1ADC[0x1AF4 - 0x1ADC];
    void *f1AF4;
    int f1AF8;
    char pad1AFC[0x1B00 - 0x1AFC];
    void *f1B00;
    int f1B04;
    int f1B08;
    int f1B0C;
    volatile int f1B10;             /* lock: semaphore, owner thread, depth */
    volatile int f1B14;
    volatile int f1B18;
    int f1B1C[7];                   /* saved SPU2 DMA registers */
    int f1B38;
};

extern struct sAD00 D_0050AD00;

extern int D_0050ACE0_3300[] __asm__("D_0050ACE0");
extern int CreateSema_3300(int *) __asm__("CreateSema");

/**
 * Creates the sound driver's recursive lock: a semaphore (D_0050ACE0 parameters) in +0x1B10 of D_0050AD00.
 *
 * The owner (+0x1B14) starts at -1 and the depth (+0x1B18) at 0. Called by SNDSYSI_init at start-up; SNDI_mutexfree
 * deletes it.
 */
void SNDI_mutexalloc(void) {
    D_0050ACE0_3300[1] = 20;
    D_0050AD00.f1B14 = -1;
    D_0050ACE0_3300[2] = 1;
    D_0050AD00.f1B18 = 0;
    D_0050AD00.f1B10 = CreateSema_3300(D_0050ACE0_3300);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_mutexfree);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern void DeleteSema(int);

/**
 * Deletes the sound driver's lock semaphore (+0x1B10 of D_0050AD00).
 *
 * Called by SNDSYS_restore and by SNDSYSI_init when the start fails.
 */
void SNDI_mutexfree(void) {
    DeleteSema(D_0050AD00.f1B10);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_mutexlock);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern int GetThreadId(void);
extern void WaitSema(int);

/* Recursive lock (pairs with SNDI_mutexunlock). The owner's re-entry bumps the count and returns early; jump2
   cross-jumps the two count increments into one copy, and the return label left behind keeps the epilogue
   restores below the final store, as in the target. */
void SNDI_mutexlock(void) {
    int threadId = GetThreadId();
    if (D_0050AD00.f1B14 == threadId) {
        D_0050AD00.f1B18++;
        return;
    }
    WaitSema(D_0050AD00.f1B10);
    D_0050AD00.f1B14 = threadId;
    D_0050AD00.f1B18++;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_mutexunlock);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern int GetThreadId(void);
extern void SignalSema(int);

/**
 * Releases the sound driver's recursive lock.
 *
 * For the owning thread it drops the depth (+0x1B18 of D_0050AD00) and clears the owner at 0. Whenever the depth is 0
 * or less it signals the semaphore +0x1B10, also when called by a thread that does not own the lock. Called through
 * DefaultMutexUnlockFn.
 */
void SNDI_mutexunlock(void) {
    int threadId = GetThreadId();
    if (D_0050AD00.f1B14 == threadId) {
        D_0050AD00.f1B18--;
        if (D_0050AD00.f1B18 == 0) {
            D_0050AD00.f1B14 = -1;
        }
    }
    if (D_0050AD00.f1B18 <= 0) {
        SignalSema(D_0050AD00.f1B10);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C3450);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_0B10_3450(int, void *, int) __asm__("func_003C0B10");
extern int f_72F8_3450(void *, int) __asm__("SNDI_getb");
extern char D_0050AAF4_3450[] __asm__("D_0050AAF4");
struct G_3450 { char pad[0xF]; char c; void *fn; };
extern char str_FX_3450[] __asm__("str_FX");
extern char f_C878_3450[] __asm__("func_003CC878");

extern struct G_3450 sndmix_3450[] __asm__("sndmix");
/**
 * Sets up the IOP effect for the kind-0x100 bus (D_0050AAF4, its type passed in).
 *
 * With +0x42 of sndgs set it installs func_003CC878 as the IOP effect callback (sndmix). Type 1 (a custom
 * effect) sends the effect data from the bus record's +0x04 with RPC function 2 (func_003C0B10), its length read from
 * the data by SNDI_getb; any other type sends the 4-byte str_FX (effect off) and clears the callback flag.
 * Called by func_003C35F8.
 *
 * @param busType  the bus type (1 = custom effect data)
 */
void func_003C3450(int busType) {
    char *busRecord = D_0050AAF4_3450;
    char *driverState = busRecord - 0x20C;
    char *effectData;
    if (*(unsigned char *)(driverState + 0x42) != 0) {
        sndmix_3450[0].fn = f_C878_3450;
        sndmix_3450[0].c = 1;
    }
    if (busType != 1) {
        f_0B10_3450(2, str_FX_3450, 4);
        if (*(unsigned char *)(driverState + 0x42) != 0)
            sndmix_3450[0].c = 0;
    } else {
        effectData = *(char **)(busRecord + 4);
        if (*(unsigned char *)(driverState + 0x42) != 0)
            sndmix_3450[0].c = busType;
        f_0B10_3450(2, effectData, f_72F8_3450(effectData + 4, 4));
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3500);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044DD48_3500[] __asm__("D_0044DD48");
struct A_3500 {
    char pad0[0xCB0];
    char *p;
    char pad1[0x1A9E - 0xCB4];
    short s;
};
struct B_3500 {
    char pad0[0x224];
    unsigned char a;
    char pad1[3];
    unsigned char b;
};
extern struct A_3500 D_0050AD00_3500 __asm__("D_0050AD00");

extern struct B_3500 sndgs_3500 __asm__("sndgs");
/**
 * Sets up the SPU2 reverb for the kind-8 bus from its type (D_0050AB08).
 *
 * Maps the type to the hardware reverb mode: below 2 off, 5 room, 10 / 20 / 30 studio A/B/C, 40 hall, 50 space, 100
 * pipe, 110 delay, 120 echo (other values pass through). Stores the work-area start ((0x200000 - D_0044DD48[mode]) >>
 * 6) in +0x1A9E of D_0050AD00 and the mode, then the bus record's +0x04 and +0x08 bytes (depth / delay), in
 * +0x10..+0x12 of the IOP voice block. Called by func_003C35F8.
 *
 * @param busType  the bus type
 */
void func_003C3500(unsigned busType) {
    unsigned reverbMode = busType;
    if (busType < 2) {
        reverbMode = 0;
    } else if (busType == 5) {
        reverbMode = 1;
    } else if (busType == 10) {
        reverbMode = 2;
    } else if (busType == 20) {
        reverbMode = 3;
    } else if (busType == 30) {
        reverbMode = 4;
    } else if (busType == 40) {
        reverbMode = 5;
    } else if (busType == 50) {
        reverbMode = 6;
    } else if (busType == 100) {
        reverbMode = 9;
    } else if (busType == 110) {
        reverbMode = 8;
    } else if (busType == 120) {
        reverbMode = 7;
    }
    D_0050AD00_3500.s = (0x200000 - D_0044DD48_3500[reverbMode]) >> 6;
    D_0050AD00_3500.p[0x10] = reverbMode;
    D_0050AD00_3500.p[0x11] = sndgs_3500.a;
    D_0050AD00_3500.p[0x12] = sndgs_3500.b;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C35F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct G_35F8 {
    char pad0[0x2A];
    unsigned short u2A;
    char pad1[0x42 - 0x2C];
    unsigned char b42;
    char pad2[0x1F8 - 0x43];
    short s1F8;
    short pad3;
    int w1FC;
};
extern short D_0050AAF4_35F8 __asm__("D_0050AAF4");
extern short D_0050AB08_35F8 __asm__("D_0050AB08");
extern int D_00515B50_35F8 __asm__("D_00515B50");
extern void func_003CC630(int, unsigned char *);
extern void func_003CC848_35F8(void) __asm__("func_003CC848");

extern struct G_35F8 sndgs_35F8 __asm__("sndgs");
/**
 * Applies the effect-bus configuration of one render kind, inside a critical section.
 *
 * Kind 8 sets up the SPU reverb (func_003C3500), kind 0x100 the IOP effect (func_003C3450), kind 4 the mixer reverb:
 * func_003CC630 with the mixer rate (+0x2A) and the bus's effect data (+0x1FC) when the kind-4 bus type (+0x1F8) is 1
 * and +0x42 is clear, else func_003CC848 (clearing D_00515B50 when +0x42 is clear). Called by SNDPLATFORM_init (kinds 8
 * and 4) and func_003C36A8.
 *
 * @param renderKind  the render kind (8, 0x100 or 4)
 */
void func_003C35F8(int renderKind) {
    SNDSYS_entercritical();
    if (renderKind == 8) {
        func_003C3500(D_0050AB08_35F8);
    } else if (renderKind == 0x100) {
        func_003C3450(D_0050AAF4_35F8);
    } else if (renderKind == 4) {
        if (sndgs_35F8.s1F8 != 1 || sndgs_35F8.b42 != 0) {
            if (sndgs_35F8.b42 == 0) D_00515B50_35F8 = 0;
            func_003CC848_35F8();
        } else {
            func_003CC630(sndgs_35F8.u2A, (unsigned char *)sndgs_35F8.w1FC);
        }
    }
    SNDSYS_leavecritical();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C36A8);
#ifdef SKIP_ASM
extern void func_003C35F8(int);

/**
 * Applies a bus configuration change for one render kind (func_003C35F8) and returns 0.
 *
 * func_003B82B8 calls it after writing a bus record; the bus number is not used.
 *
 * @param busIndex  the bus number (unused)
 * @param renderKind  the render kind
 * @return 0
 */
int func_003C36A8(int busIndex, int renderKind) {
    func_003C35F8(renderKind);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C36C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct O_36C8 { char pad[0x14]; short x; short y; };
struct G_36C8 { char pad[0xCB0]; struct O_36C8 *o; };
extern struct G_36C8 D_0050AD00_36C8[] __asm__("D_0050AD00");
extern float D_0050AB08_36C8[] __asm__("D_0050AB08");

/**
 * Writes the kind-8 bus's pan pair to the SPU reverb output levels.
 *
 * With flag 8 and the IOP voice block set up, the bus's left/right gains (D_0050AB08 +0x0C / +0x10) times 32767 go to
 * +0x14 / +0x16 of the block. Called by func_003B84E0 and SNDPLATFORM_init.
 *
 * @param busIndex  the bus number (unused)
 * @param flags  render-kind flags (8 applies)
 */
void func_003C36C8(int busIndex, int flags) {
    struct G_36C8 *soundState = &D_0050AD00_36C8[0];
    if (soundState->o != 0 && (flags & 8)) {
        soundState->o->x = (int)(D_0050AB08_36C8[3] * 32767.0f);
        soundState->o->y = (int)(D_0050AB08_36C8[4] * 32767.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3728);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct S2_3728 { short v[2]; };
extern struct S2_3728 D_0044DD78_3728[] __asm__("D_0044DD78");

/**
 * Looks up a pair of pan gains for an azimuth from the 256-entry table D_0044DD78.
 *
 * Bits 8-15 of the azimuth pick the entry; both of its shorts are divided by 32767. Used by func_003C38E0.
 *
 * @param azimuth  the azimuth (0x10000 = full circle)
 * @param out  receives the two gains
 */
void func_003C3728(unsigned int azimuth, float *out) {
    int i;
    for (i = 0; i < 2; i++) {
        out[i] = (float)D_0044DD78_3728[(azimuth >> 8) & 0xFF].v[i] / 32767.0f;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C3780);
#ifdef SKIP_ASM
/**
 * Mirrors a pair of speaker gains into the quadrant of a direction.
 *
 * Classifies the direction (@p dx, @p dy) into one of four regions by its signs and which component is larger, and the
 * gains into another by @p ang (front or rear half) and which gain is larger; negates @p x for a negative @p dx and @p
 * y for a negative @p dy, then one more axis when the two regions disagree. Used by func_003C38E0.
 *
 * @param ang  the azimuth
 * @param x  the first gain, adjusted in place
 * @param y  the second gain, adjusted in place
 * @param dx  the direction's x component
 * @param dy  the direction's y component
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void func_003C3780(int ang, float *x, float *y, int dx, int dy) {
    int deltaRegion, flipAxis, pointRegion;
    if (dx < 0 && dy < 0) {
        deltaRegion = dx < dy ? 1 : 2;
    } else if (dx >= 0 && dy >= 0) {
        deltaRegion = dx < dy ? 2 : 1;
    } else {
        int ax = dx > -1 ? dx : -dx;
        if (dy < 0 ? -dy < ax : dy < ax)
            deltaRegion = 4;
        else
            deltaRegion = 3;
    }
    flipAxis = 0;
    if ((unsigned)(ang - 0x4000) < 0x8000) {
        pointRegion = 4;
        if (*x < *y) pointRegion = 3;
        if (deltaRegion < 3) flipAxis = (pointRegion == 3 && deltaRegion == 2) ? 1 : 2;
    } else {
        pointRegion = 1;
        if (*x < *y) pointRegion = 2;
        if (deltaRegion >= 3) flipAxis = (pointRegion == 2 && deltaRegion == 3) ? 1 : 2;
    }
    if (dx < 0) *x = -*x;
    if (dy < 0) *y = -*y;
    if (flipAxis == 1) *x = -*x;
    else if (flipAxis == 2) *y = -*y;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C38E0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_asyncresolvetimbre);
#ifdef SKIP_ASM
/**
 * Points a patch header's sample at its place in uploaded sample data.
 *
 * The first call (*@p firstOffset == 0) records the header's sample offset (+0x68); the address @p sampleData + (offset
 * - first offset) is stored through the header's pointer +0x64. Called by func_003B6788.
 *
 * @param patchHeader  the parsed patch header
 * @param sampleData  where the samples were uploaded
 * @param firstOffset  in/out: the offset of the first sample
 * @return 0
 */
int SNDPLATFORM_asyncresolvetimbre(void *patchHeader, int sampleData, int *firstOffset) {
    if (*firstOffset == 0) {
        *firstOffset = (*(int *)((char*)(patchHeader) + (0x68)));
    }
    *(*(int **)((char*)(patchHeader) + (0x64))) = sampleData + ((*(int *)((char*)(patchHeader) + (0x68))) - *firstOffset);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_playtimbre);
#ifdef SKIP_ASM
struct G_3EE0 {
    char pad0[0x2F];
    unsigned char base2;
    unsigned char base;
    char pad31[0x49 - 0x31];
    unsigned char mode;
    char pad4A[0x1F0 - 0x4A];
    struct CHANPUB *chans;
};
struct W8_3EE0 { int w; int pad; };
struct W16_3EE0 { int w; char pad[12]; };
struct T_3EE0 {
    char pad0[0x20];
    struct W8_3EE0 a[48];
    struct W16_3EE0 b[1];
};
struct E32_3EE0 { int w; char pad[28]; };
struct A_3EE0 {
    char pad0[0x840];
    struct W8_3EE0 x840[48];
    struct W8_3EE0 x9C0[(0xCB0 - 0x9C0) / 8];
    struct T_3EE0 *tbl;
    char padCB4[0xEB0 - 0xCB4];
    struct E32_3EE0 e[96];
    char pad1AB0[0x1AB8 - 0x1AB0];
    int x1AB8;
};
extern struct A_3EE0 D_0050AD00_3EE0 __asm__("D_0050AD00");
extern void f2EC8_3EE0(int) __asm__("func_003C2EC8");
extern void f38E0_3EE0(int) __asm__("func_003C38E0");
extern void f3178_3EE0(int, int) __asm__("SNDPLATFORM_lowpass");
extern void f83A0_3EE0(int) __asm__("MIX_play");
extern void f8158_3EE0(int, int, int, int, int, int, int, int, int, int, int, int, int, int) __asm__("MIX_playinit");

/**
 * Starts a timbre's voices on a channel through its render path.
 *
 * Render kind 8 (SPU): pitch (SNDPLATFORM_setpitch), pan (func_003C38E0), dry (func_003C2EC8) and send (SNDPLATFORM_setfxlevel), then
 * per voice the sample address (the header's +0x64 pointer plus the voice's offset +0x68[i]) goes to the +0xEB0 table
 * of D_0050AD00 and, tagged with a new sequence number (+0x1AB8 += 0x800000), as the key in the IOP voice block (with
 * the +0x18 twin when +0x49 is 2) and the EE copy +0x840. Kind 0x100 does the same into the +0x1A0 table without the
 * dry step. Mixer voices are set up one by one with MIX_playinit (sample data, loop points, format), get pitch, pan
 * and send on the first voice, then SNDPLATFORM_timemult, func_003C30C8, SNDPLATFORM_lowpass and SNDPLATFORM_highpass, and are started with
 * MIX_play. Called by SNDBANKI_playtimbre.
 *
 * @param patchHeader  the parsed patch header
 * @param base  the sample data address
 * @param id  the channel index
 * @param timeMult  the time multiplier
 * @param filterAmount  the clip level for func_003C30C8
 * @param lowPassCutoff  the low-pass cutoff
 * @param highPassCutoff  the high-pass cutoff
 * @return 0
 */
int SNDPLATFORM_playtimbre(struct SNDIPATCHHEADER *patchHeader, int base, int id, int timeMult, int filterAmount, int lowPassCutoff, int highPassCutoff) {
    struct CHANPUB *ch;
    int i;

    ch = &((struct CHANPUB*)sndgs.chan)[id];
    if (ch->rendermode & 8) {
        SNDPLATFORM_setpitch(id);
        f38E0_3EE0(id);
        f2EC8_3EE0(id);
        SNDPLATFORM_setfxlevel(id, 0);
        for (i = 0; i < ch->numchan; i++) {
            D_0050AD00_3EE0.e[ch->voices[i]].w = *patchHeader->paudioramaddr + (patchHeader->sampleoffset[i] - patchHeader->sampleoffset[0]);
            D_0050AD00_3EE0.x1AB8 += 0x800000;
            D_0050AD00_3EE0.tbl->a[ch->voices[i]].w = D_0050AD00_3EE0.e[ch->voices[i]].w | D_0050AD00_3EE0.x1AB8;
            D_0050AD00_3EE0.x840[ch->voices[i]].w = D_0050AD00_3EE0.e[ch->voices[i]].w;
            if (sndgs.sso.set.outputspdifmode == 2) {
                D_0050AD00_3EE0.tbl->a[ch->voices[i] + 0x18].w = D_0050AD00_3EE0.e[ch->voices[i]].w | D_0050AD00_3EE0.x1AB8;
                D_0050AD00_3EE0.x840[ch->voices[i] + 0x18].w = D_0050AD00_3EE0.e[ch->voices[i]].w;
            }
        }
    } else if (ch->rendermode & 0x100) {
        SNDPLATFORM_setpitch(id);
        f38E0_3EE0(id);
        SNDPLATFORM_setfxlevel(id, 0);
        for (i = 0; i < ch->numchan; i++) {
            D_0050AD00_3EE0.e[ch->voices[i]].w = *patchHeader->paudioramaddr + (patchHeader->sampleoffset[i] - patchHeader->sampleoffset[0]);
            D_0050AD00_3EE0.x1AB8 += 0x800000;
            D_0050AD00_3EE0.tbl->b[ch->voices[i] - sndgs.sso.set.voicesspu].w = D_0050AD00_3EE0.e[ch->voices[i]].w | D_0050AD00_3EE0.x1AB8;
            D_0050AD00_3EE0.x9C0[ch->voices[i] - sndgs.sso.set.voicesspu].w = 0;
        }
    } else {
        for (i = 0; i < ch->numchan; i++) {
            f8158_3EE0(ch->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), patchHeader->samplerep, 1,
                       base + patchHeader->sampleoffset[i], base + patchHeader->sampleoffset[0] + patchHeader->loopoffset[i], ((int*)&patchHeader->ptimestretchdata[0])[i], ch->numchan, (int)ch->frames,
                       patchHeader->sustainstart, patchHeader->sustainend, 0, 0, patchHeader->platformver, i);
        }
        SNDPLATFORM_setpitch(ch->voices[0]);
        f38E0_3EE0(ch->voices[0]);
        SNDPLATFORM_setfxlevel(ch->voices[0], 0);
        SNDPLATFORM_timemult(ch->voices[0], timeMult);
        func_003C30C8(ch->voices[0], filterAmount);
        f3178_3EE0(ch->voices[0], lowPassCutoff);
        SNDPLATFORM_highpass(ch->voices[0], highPassCutoff);
        for (i = 0; i < ch->numchan; i++) {
            f83A0_3EE0(ch->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_download);
#ifdef SKIP_ASM
extern void func_003C4788(int, int, int, int, int);

/**
 * Queues an upload of sample data to sound memory with priority 10 (func_003C4788) and returns its handle.
 *
 * SNDbankadd uploads banks with it (play location 0x100 or 8), polling the handle with SNDPLATFORM_downloadcomplete. Declared void,
 * but func_003C4788's handle passes through.
 *
 * @param playLoc  the play location (render kind)
 * @param srcAddr  the source in main memory
 * @param dstAddr  the destination in sound memory
 * @param byteCount  the size in bytes
 */
void SNDPLATFORM_download(int playLoc, int srcAddr, int dstAddr, int byteCount) {
    func_003C4788(playLoc, srcAddr, dstAddr, byteCount, 10);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_downloadcomplete);
#ifdef SKIP_ASM
extern void func_003C4898();

/**
 * Reports whether a sound-memory upload has finished (forwards to func_003C4898).
 *
 * Declared without parameters, but the handle the caller passes stays in $a0 for func_003C4898 and its result passes
 * through. SNDbankadd polls it until it is non-zero.
 */
void SNDPLATFORM_downloadcomplete(void) {
    func_003C4898();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_getcurframe);
#ifdef SKIP_ASM
struct G_4318 { char pad[0x2F]; unsigned char b2F; unsigned char b30; char pad2[0x1F0 - 0x31]; struct CHANPUB *e; };
struct P_4318 { int a; int b; };
struct Q_4318 { char pad[0x18]; unsigned int v; char pad2[4]; };
extern struct Q_4318 D_0050BB98_4318[] __asm__("D_0050BB98");
extern unsigned int MIX_getframe_4318(int) __asm__("MIX_getframe");

/**
 * Returns a channel's current play position.
 *
 * Render kind 8 (SPU): the voice's current address (+0x840 table of D_0050AD00) minus its sample start (+0x18 of its
 * D_0050BB98 record), * 0x1C >> 4 (SPU-ADPCM bytes to samples); kind 0x100: the voice's word in the +0x9C0 table; mixer
 * voices: MIX_getframe. Called by SNDtimeremaining.
 *
 * @param n  the channel index
 * @return the position in samples
 */
unsigned int SNDPLATFORM_getcurframe(int n) {
    struct CHANPUB *channel = &sndgs.chan[n];
    struct Q_4318 *speakerRecord = &D_0050BB98_4318[n];
    if (channel->rendermode & 8) {
        return ((((struct P_4318 *)D_0050BB98_4318)[n - 203].a - speakerRecord->v) * 0x1C) >> 4;
    }
    if (channel->rendermode & 0x100) {
        return ((struct P_4318 *)D_0050BB98_4318)[n - sndgs.sso.set.voicesspu - 155].a;
    }
    return MIX_getframe_4318(n - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C43C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_43C0 { int a, b, c, d; };
extern void FlushCache(int);
extern void iFlushCache(int);
extern int sceSifDmaStat(int);
extern int isceSifDmaStat(int);
extern int sceSifSetDma(void *, int);
extern int isceSifSetDma(void *, int);
extern int D_0044E178_43C0[] __asm__("D_0044E178");

/**
 * Copies a block from EE memory to IOP memory with one SIF DMA, waiting for the previous one first.
 *
 * Flushes the data cache, waits until the last transfer (D_0044E178) is done and starts the new one, remembering its
 * id; the interrupt-safe variants (iFlushCache, isceSifDmaStat, isceSifSetDma) are used when @p inInterrupt is set. Used by
 * the sound-memory transfer queue (func_003C4450, func_003C4668).
 *
 * @param srcAddr  the source in EE memory
 * @param iopAddr  the destination in IOP memory
 * @param byteCount  the size in bytes
 * @param inInterrupt  non-zero when called from an interrupt handler
 */
void func_003C43C0(int srcAddr, int iopAddr, int byteCount, int inInterrupt) {
    struct S_43C0 dmaDesc;
    dmaDesc.a = srcAddr;
    dmaDesc.c = byteCount;
    dmaDesc.b = iopAddr;
    dmaDesc.d = 0;
    if (inInterrupt != 0) {
        iFlushCache(0);
        while (isceSifDmaStat(D_0044E178_43C0[0]) >= 0) {}
        D_0044E178_43C0[0] = isceSifSetDma(&dmaDesc, 1);
    } else {
        FlushCache(0);
        while (sceSifDmaStat(D_0044E178_43C0[0]) >= 0) {}
        D_0044E178_43C0[0] = sceSifSetDma(&dmaDesc, 1);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4450);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3); loop rewritten without the D_0050B9C8 inner label (cfg_1).

extern struct sAD00 D_0050AD00;

struct Dst_4450 { char b0; char pad1[0xF]; int f10; int f14; int f18; int f1C; unsigned short h20; short h22; };
extern struct Dst_4450 D_00515780_4450 __asm__("D_00515780");
extern void func_003C43C0(int, int, int, int);
extern void sceSifSendCmd(int, void *, int, int, int, int);
extern void f61F0_4450(int, void *, int, int, int, int) __asm__("isceSifSendCmd");

/**
 * Starts the next queued sound-memory transfer.
 *
 * Picks among the queued entries (+0xCC8 of D_0050AD00, 28 slots) the one with the highest priority (+0x0E), the oldest
 * (lowest id) among equals, as +0xCC1; stamps the tick (+0xCC4), DMAs its data to the IOP staging block (+0xE88 port,
 * func_003C43C0), marks a transfer running (+0xCC2) and sends the IOP a 0x30-byte request (D_00515780: id, staging
 * address, destination, size, priority) with isceSifSendCmd (from an interrupt) or sceSifSendCmd.
 *
 * @param inInterrupt  non-zero when called from an interrupt handler
 */
void func_003C4450(int inInterrupt) {
    int cnt = 0;
    unsigned best = 0xFFFFFFFF;
    unsigned char pri = 0;
    int i;
    struct sAD00Xfer *chosen;
    for (i = 0; i < 0x1C; i++) {
        struct sAD00Xfer *e = &D_0050AD00.fCC8[i];
        if (e->key != 0) {
            cnt++;
            if (pri < e->f0E) {
                D_0050AD00.fCC1 = i;
                best = e->key;
                pri = e->f0E;
            } else if (e->f0E == pri && e->key < best) {
                D_0050AD00.fCC1 = i;
                best = e->key;
            }
            if (cnt >= (signed char)D_0050AD00.fCC0) break;
        }
    }
    chosen = &D_0050AD00.fCC8[(signed char)D_0050AD00.fCC1];
    D_0050AD00.fCC4 = sndgs.audiotick;
    func_003C43C0(chosen->f04, D_0050AD00.fE88[0].f04, chosen->f0C, inInterrupt);
    D_0050AD00.fCC2 = 1;
    D_00515780_4450.b0 = 0x30;
    D_00515780_4450.f10 = 1;
    D_00515780_4450.f14 = chosen->key;
    D_00515780_4450.f18 = D_0050AD00.fE88[0].f04;
    D_00515780_4450.f1C = chosen->f08;
    D_00515780_4450.h20 = chosen->f0C;
    D_00515780_4450.h22 = chosen->f0E;
    if (inInterrupt != 0) {
        f61F0_4450(0, &D_00515780_4450, 0x30, 0, 0, 0);
        return;
    }
    sceSifSendCmd(0, &D_00515780_4450, 0x30, 0, 0, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C45E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (disable / enable interrupts, pipeline sync, read the Status register).
#define DI_2B45() __asm__ volatile("di")

#define EI_2B45() __asm__ volatile("ei")

#define SYNCP_2B45() __asm__ volatile("sync.p")

#define MFC0_STATUS_2B45(v) __asm__ volatile("mfc0 %0, $12" : "=r"(v))

/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern void func_003C4450(int);

/**
 * Starts the next sound-memory transfer if none is running and some are queued (func_003C4450).
 *
 * Outside an interrupt handler it runs with interrupts disabled. Called when a transfer is queued (func_003C4668),
 * while waiting for room (func_003C4788) and when one completes (func_003C4918).
 *
 * @param inInterrupt  non-zero when called from an interrupt handler
 */
void func_003C45E8(int inInterrupt) {
    int s;
    if (inInterrupt == 0) {
        do {
            DI_2B45();
            SYNCP_2B45();
            MFC0_STATUS_2B45(s);
        } while (s & 0x10000);
    }
    if (D_0050AD00.fCC2 == 0 && (signed char)D_0050AD00.fCC0 > 0) {
        func_003C4450(inInterrupt);
    }
    if (inInterrupt == 0) {
        EI_2B45();
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4668);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only COP0 (disable / enable interrupts, pipeline sync, read the Status register).
#define DI_2B45() __asm__ volatile("di")

#define EI_2B45() __asm__ volatile("ei")

#define SYNCP_2B45() __asm__ volatile("sync.p")

#define MFC0_STATUS_2B45(v) __asm__ volatile("mfc0 %0, $12" : "=r"(v))

/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern void func_003C45E8_4668(int) __asm__("func_003C45E8");
extern void func_003C43C0_4668(int, int, int, int) __asm__("func_003C43C0");
/**
 * Queues one transfer to sound memory and returns its id.
 *
 * Rounds the size up to 64 bytes and, with interrupts disabled, takes the next transfer id (+0xCBC of D_0050AD00, never
 * 0). Play location 0x100 copies straight to IOP memory (func_003C43C0) and returns. Otherwise the first free slot of
 * +0xCC8 gets {id, source, destination, size, priority}, the queue count +0xCC0 grows and func_003C45E8 starts it if
 * idle. Called by func_003C4788.
 *
 * @param op  the play location (0x100 for a direct copy)
 * @param srcAddr  the source in main memory
 * @param dstAddr  the destination
 * @param size  the size in bytes
 * @param priority  the priority
 * @return the id, or 0 when all 28 slots are taken
 */
int func_003C4668(int op, int srcAddr, int dstAddr, int size, int priority) {
    int s, i;
    if (size & 0x3F) {
        size += 0x40;
        size &= 0xFFFFFFC0;
    }
    do {
        DI_2B45();
        SYNCP_2B45();
        MFC0_STATUS_2B45(s);
    } while (s & 0x10000);
    D_0050AD00.fCBC++;
    if (D_0050AD00.fCBC == 0) {
        D_0050AD00.fCBC = 1;
    }
    if (op == 0x100) {
        func_003C43C0_4668(srcAddr, dstAddr, size, 0);
        EI_2B45();
        return D_0050AD00.fCBC;
    }
    for (i = 0; i < 28; i++) {
        struct sAD00Xfer *slot = &D_0050AD00.fCC8[i];
        if (slot->key == 0) {
            D_0050AD00.fCC0++;
            slot->key = D_0050AD00.fCBC;
            slot->f04 = srcAddr;
            slot->f08 = dstAddr;
            slot->f0C = size;
            slot->f0E = priority;
            EI_2B45();
            func_003C45E8_4668(0);
            return D_0050AD00.fCBC;
        }
    }
    EI_2B45();
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C4788);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C45E8_4788(int) __asm__("func_003C45E8");
extern int func_003C4668_4788(int, int, int, int, int) __asm__("func_003C4668");
extern volatile unsigned char D_0050AD00_4788[] __asm__("D_0050AD00");
int func_003C4788_4788(int, int, int, int, int) __asm__("func_003C4788");

/**
 * Queues a transfer to sound memory in pieces of up to 4 KB and returns the last piece's id.
 *
 * Before each piece it waits (driving func_003C45E8) until at least 5 of the 28 queue slots are free, then queues it
 * with func_003C4668. Called by SNDPLATFORM_download.
 *
 * @param playLoc  the play location
 * @param srcAddr  the source
 * @param dstAddr  the destination
 * @param byteCount  the size in bytes
 * @param priority  the priority
 * @return the id of the last piece, or 0 for a size of 0
 */
int func_003C4788_4788(int playLoc, int srcAddr, int dstAddr, int byteCount, int priority) {
    int transferId = 0;
    int chunkSize;
    int queuedCount;
    while (byteCount > 0) {
        chunkSize = 0x1000;
        if (!(byteCount > 0xFFF)) chunkSize = byteCount;
        queuedCount = D_0050AD00_4788[0xCC0];
        while (0x1C - (signed char)queuedCount < 5) {
            func_003C45E8_4788(0);
            queuedCount = D_0050AD00_4788[0xCC0];
        }
        transferId = func_003C4668_4788(playLoc, srcAddr, dstAddr, chunkSize, priority);
        srcAddr += chunkSize;
        dstAddr += chunkSize;
        byteCount -= chunkSize;
    }
    return transferId;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003C4898);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).


/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

extern void func_003C4450(int);

int func_003C4898_4898(int) __asm__("func_003C4898");
/**
 * Returns whether the sound-memory transfer with id @p key has finished.
 *
 * Returns 1 when nothing is queued or the id is not in the queue; otherwise 0, restarting the transfer (func_003C4450)
 * when the current one has run for more than 500 ticks. Reached through SNDPLATFORM_downloadcomplete.
 *
 * @param key  the transfer id
 * @return 1 when done, 0 while pending
 */
int func_003C4898_4898(int key) {
    int i;
    if ((signed char)D_0050AD00.fCC0 <= 0) return 1;
    for (i = 0; i < 28; i++) {
        if (D_0050AD00.fCC8[i].key == key) {
            if (D_0050AD00.fCC4 + 500 < sndgs.audiotick) {
                func_003C4450(0);
            }
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4918);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003C45E8(int);

/**
 * Completes a sound-memory transfer the IOP reports done and starts the next one.
 *
 * Frees the queue slot with that id, clears the running flag (+0xCC2 of D_0050AD00), drops the queue count and calls
 * func_003C45E8 in interrupt mode. Called from the SIF command handler func_003C1578 (mode 2).
 *
 * @param key  the finished transfer's id
 */
void func_003C4918(int key) {
    int i;
    for (i = 0; i < 28; i++) {
        if (D_0050AD00.fCC8[i].key == key) {
            D_0050AD00.fCC2 = 0;
            D_0050AD00.fCC0--;
            D_0050AD00.fCC8[i].key = 0;
            func_003C45E8(1);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4978);
#ifdef SKIP_ASM
/**
 * Copies quadwords from main memory to the scratchpad with DMA channel 9 (to SPR), the same code as func_003BC968.
 *
 * Waits for the channel to be idle, sets MADR (0x1000D410), SADR (0x1000D480) and QWC (0x1000D420) and starts it (CHCR
 * 0x1000D400 = 0x100); flag 0x10000 waits for the end. func_003C0A60 restores the DTS encoder's scratchpad area with
 * it.
 *
 * @param memAddr  the source in main memory
 * @param sprAddr  the destination in the scratchpad
 * @param qwordCount  the number of quadwords
 * @param flags  0x10000 to wait for completion
 */
void func_003C4978(int memAddr, int sprAddr, int qwordCount, int flags) {
    if (*(volatile int *)0x1000D400 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
    *(volatile int *)0x1000D410 = memAddr;
    *(volatile int *)0x1000D480 = sprAddr;
    *(volatile int *)0x1000D420 = qwordCount;
    *(volatile int *)0x1000D400 = 0x100;
    if ((flags & 0x10000) && (*(volatile int *)0x1000D400 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D400 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4A38);
#ifdef SKIP_ASM
/**
 * Copies quadwords from the scratchpad to main memory with DMA channel 8 (from SPR), the same code as func_003BCA28.
 *
 * Waits for the channel to be idle, sets MADR (0x1000D010), SADR (0x1000D080) and QWC (0x1000D020) and starts it (CHCR
 * 0x1000D000 = 0x100); flag 0x10000 waits for the end. func_003C09B8 saves the scratchpad area with it.
 *
 * @param sprAddr  the source in the scratchpad
 * @param memAddr  the destination in main memory
 * @param qwordCount  the number of quadwords
 * @param flags  0x10000 to wait for completion
 */
void func_003C4A38(int sprAddr, int memAddr, int qwordCount, int flags) {
    if (*(volatile int *)0x1000D000 & 0x100) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
    *(volatile int *)0x1000D010 = memAddr;
    *(volatile int *)0x1000D080 = sprAddr;
    *(volatile int *)0x1000D020 = qwordCount;
    *(volatile int *)0x1000D000 = 0x100;
    if ((flags & 0x10000) && (*(volatile int *)0x1000D000 & 0x100)) {
        do {
        } while (*(volatile int *)0x1000D000 & 0x100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C4AF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char D_0050AD00_4AF8[] __asm__("D_0050AD00");

/**
 * Clips a free sound-RAM gap to the usable range (in 64-byte units).
 *
 * Raises @p start to the low limit (+0x1A9A of D_0050AD00, shrinking the length to match) and cuts the length so the gap
 * ends by the high limit (+0x1A9C) and by the reverb work area (+0x1A9E). Used by SNDPLATFORM_memalloc and SNDPLATFORM_memlargestunused.
 *
 * @param start  in/out: the gap's start
 * @param length  in/out: the gap's length
 */
void func_003C4AF8(int *start, int *length) {
    char *soundState = D_0050AD00_4AF8;
    int temp_4;
    int temp_6;
    int lo = *(unsigned short *)(soundState + 0x1A9A);

    temp_6 = *start;
    if (temp_6 < lo) {
        *start = lo;
        *length -= lo - temp_6;
    }
    if ((int)*(unsigned short *)(soundState + 0x1A9C) < *start + *length) {
        *length = *(unsigned short *)(soundState + 0x1A9C) - *start;
    }
    temp_4 = *start;
    if ((int)*(unsigned short *)(soundState + 0x1A9E) < temp_4 + *length) {
        *length = *(unsigned short *)(soundState + 0x1A9E) - temp_4;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_memalloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
#include "fakematch.h"
struct Blk_4B70 {
    unsigned short start;
    unsigned short size;
};
struct G_4B70 {
    char pad0000[0x1A98];
    unsigned short count;
    unsigned short base;
    unsigned short end;
    char pad1A9E[2];
    struct Blk_4B70 *list;
};
extern struct G_4B70 D_0050AD00_4B70 __asm__("D_0050AD00");
extern unsigned short D_0050A910_4B70 __asm__("D_0050A910");
extern void func_003C4AF8_4B70(int *start, int *len) __asm__("func_003C4AF8");
extern int sceSifAllocIopHeap_4B70(int size) __asm__("sceSifAllocIopHeap");
/**
 * Allocates sound memory for a play location and returns its address.
 *
 * Play location 0x100 takes @p size + 0x40 bytes of IOP heap (sceSifAllocIopHeap). Otherwise, while fewer than D_0050A910
 * segments are in use, it finds the first gap of the sorted segment table (+0x1AA0 of D_0050AD00, count +0x1A98)
 * between the low and high limits, clipped by func_003C4AF8, that holds the size in 64-byte units, inserts the segment
 * there and returns its byte address. Called by SNDbankadd.
 *
 * @param type  the play location (0x100 IOP memory, otherwise SPU RAM)
 * @param size  the size in bytes
 * @return the address, or 0 when nothing fits
 */
// FAKEMATCH: an empty `do {} while (0);` before the in-loop call (its loop notes act as a sched barrier: retail's unscheduled
// join block, probably a disabled debug macro), a `do { ... } while (0)` around the insert block (doubles its refs of the
// GCSE-hoisted %hi(D_0050AD00), so that pseudo outranks its loop copy: retail s5 / s6, ours swapped), and two fakematch.h
// pins: `register int s FM_REG("$3")` (slot start in the loop) and `register struct G_4B70 *g FM_REG("$4") = &D_0050AD00_4B70;`
// for the insert block's list access (retail a0, ours v1/a1); plain form 94.61%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
int SNDPLATFORM_memalloc(int type, int size) {
    int start;
    int len;
    int i;
    int j;
    register int gapStart FM_REG("$3");
    struct Blk_4B70 *segment;
    struct Blk_4B70 *prevSegment;

    i = 0;
    if (type == 0x100) return sceSifAllocIopHeap_4B70(size + 0x40);
    if (D_0050AD00_4B70.count >= D_0050A910_4B70) return 0;
    size += 0x3F;
    size >>= 6;
    if (D_0050AD00_4B70.count == 0) {
        start = D_0050AD00_4B70.base;
        len = D_0050AD00_4B70.end - D_0050AD00_4B70.base;
        func_003C4AF8_4B70(&start, &len);
        if (len < size) return 0;
    } else {
        for (i = 0; i < D_0050AD00_4B70.count; i++) {
            segment = &D_0050AD00_4B70.list[i];
            if (i == 0) {
                gapStart = D_0050AD00_4B70.base;
            } else {
                prevSegment = &D_0050AD00_4B70.list[i - 1];
                gapStart = prevSegment->start;
                gapStart += prevSegment->size;
            }
            start = gapStart;
            len = segment->start - gapStart;
            do {} while (0);
            func_003C4AF8_4B70(&start, &len);
            if (len >= size) {
                for (j = D_0050AD00_4B70.count; i < j; j--) D_0050AD00_4B70.list[j] = D_0050AD00_4B70.list[j - 1];
                goto insert;
            }
        }
        prevSegment = &D_0050AD00_4B70.list[i - 1];
        start = prevSegment->start + prevSegment->size;
        len = D_0050AD00_4B70.end - start;
        func_003C4AF8_4B70(&start, &len);
        if (len >= size) goto insert;
        return 0;
    }
insert:
    do {
        register struct G_4B70 *soundState FM_REG("$4") = &D_0050AD00_4B70;
        segment = &soundState->list[i];
        segment->start = start;
        segment->size = size;
        D_0050AD00_4B70.count++;
        return start << 6;
    } while (0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_memfree);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct K_4D78 { unsigned short key; unsigned short v; };
struct G_4D78 { char pad[0x1A98]; unsigned short cnt; char pad2[6]; struct K_4D78 *list; };
extern struct G_4D78 D_0050AD00_4D78 __asm__("D_0050AD00");
extern int func_0042AD08_4D78(unsigned int) __asm__("func_0042AD08");

/**
 * Frees sound memory a play location's allocation took.
 *
 * Play location 0x100 frees the IOP heap block (func_0042AD08). Otherwise it removes the segment starting at that
 * address (in 64-byte units) from the sorted table +0x1AA0 of D_0050AD00, closing the gap. Called by SNDbankremove.
 *
 * @param playLoc  the play location
 * @param address  the address SNDPLATFORM_memalloc returned
 * @return 0, or -8 when no segment starts there
 */
int SNDPLATFORM_memfree(int playLoc, unsigned int address) {
    int i;
    if (playLoc == 0x100) return func_0042AD08_4D78(address);
    address >>= 6;
    for (i = 0; i < D_0050AD00_4D78.cnt; i++) {
        struct K_4D78 *segment = &D_0050AD00_4D78.list[i];
        if (segment->key == address) {
            D_0050AD00_4D78.cnt--;
            for (; i < D_0050AD00_4D78.cnt; i++) {
                D_0050AD00_4D78.list[i] = D_0050AD00_4D78.list[i + 1];
            }
            return 0;
        }
    }
    return -8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_memlimits);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* D_0050AD00: the IOP sound side's state (RPC buffers, the shared voice block func_003C1638 lays out, the transfer
   queue, speaker gains, sound-RAM segments, the recursive lock). Layout from every use in this unit; field names are
   their offsets. */
/* 16 bytes, D_0050AD00.fE88[] */

extern struct sAD00 D_0050AD00;

/**
 * Sets the range of SPU RAM the allocator may use.
 *
 * Both limits are rounded up to 64-byte units: the start (+0x1A9A of D_0050AD00) must be at least 321 units (0x5040,
 * past the reserved area), the end (+0x1A9C) at most 0x8000 units (2 MB); -1 selects those defaults. Called by
 * func_003B5B20.
 *
 * @param startAddr  the first usable byte address, or -1
 * @param endAddr  the end address, or -1
 * @return 0, or -8 for a limit out of range
 */
int SNDPLATFORM_memlimits(int startAddr, int endAddr) {
    if (startAddr == -1) {
        D_0050AD00.f1A9A = 321;
    } else {
        startAddr = (startAddr + 0x3F) >> 6;
        if (startAddr < 321) return -8;
        D_0050AD00.f1A9A = startAddr;
    }
    if (endAddr == -1) {
        D_0050AD00.f1A9C = 0x8000;
    } else {
        endAddr = (endAddr + 0x3F) >> 6;
        if (endAddr > 0x8000) return -8;
        D_0050AD00.f1A9C = endAddr;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_memlargestunused);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_4EC8 { unsigned short a; unsigned short b; };
struct G_4EC8 { char pad[0x1A98]; unsigned short n; unsigned short lo; unsigned short hi; unsigned short max; struct S_4EC8 *segs; };
extern struct G_4EC8 D_0050AD00_4EC8 __asm__("D_0050AD00");
int f_4EC8(int *out) __asm__("SNDPLATFORM_memlargestunused");
/**
 * Finds the largest free block of SPU RAM and returns its size.
 *
 * Checks every gap of the segment table (+0x1AA0 of D_0050AD00) clipped by func_003C4AF8: the whole low-to-high range
 * when nothing is allocated, else before, between and after the segments (up to +0x1A9E). Likely bug: the gap before
 * the first segment is measured from the segment's own start with the fixed 321 as low limit (len = start - 321), not
 * from +0x1A9A. Called by func_003B5B68.
 *
 * @param out  receives the block's byte address
 * @return the block's size in bytes
 */
int f_4EC8(int *out) {
    int start;
    int len;
    int best = 0;
    int i;
    struct S_4EC8 *seg;
    struct S_4EC8 *prev;

    if (D_0050AD00_4EC8.n == 0) {
        start = D_0050AD00_4EC8.lo;
        len = D_0050AD00_4EC8.hi - D_0050AD00_4EC8.lo;
        func_003C4AF8(&start, &len);
        best = len;
        *out = start;
    } else {
        for (i = 0; i < D_0050AD00_4EC8.n; i++) {
            seg = &D_0050AD00_4EC8.segs[i];
            if (i == 0) {
                start = seg->a;
                len = seg->a - 321;
                func_003C4AF8(&start, &len);
                if (best < len) {
                    best = len;
                    *out = start;
                }
            } else {
                prev = &D_0050AD00_4EC8.segs[i - 1];
                start = prev->a + prev->b;
                len = seg->a - start;
                func_003C4AF8(&start, &len);
                if (best < len) {
                    best = len;
                    *out = start;
                }
            }
        }
        prev = &D_0050AD00_4EC8.segs[i - 1];
        start = prev->a + prev->b;
        len = D_0050AD00_4EC8.max - start;
        func_003C4AF8(&start, &len);
        if (best < len) {
            best = len;
            *out = start;
        }
    }
    *out <<= 6;
    return best << 6;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_filteradd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MIX_filteradd_5068(int, int, int) __asm__("MIX_filteradd");
struct G_5068 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };

/**
 * Adds a custom filter to each software-mixer voice of a channel.
 *
 * Only render kind 4 (mixer) channels: each voice's mixer index, its position in the channel and the descriptor go to
 * MIX_filteradd, which builds the filter and links it into the voice's chain. Reached through SNDCTRL_filteradd, which
 * SNDSTRMI_startstream calls with the stream's first track record.
 *
 * @param channelIndex  the channel index
 * @param filterDesc  the filter descriptor (+0x00 parameter, +0x04 state size, +0x08 order, +0x0C init, +0x10 process, +0x14
 * release)
 * @return 0, or -7 for a channel that is not on the mixer
 */
int SNDPLATFORM_filteradd(int channelIndex, int filterDesc) {
    struct CHANPUB *channel;
    int i;
    channel = &sndgs.chan[channelIndex];
    if (channel->rendermode & 4) {
        for (i = 0; i < channel->numchan; i++)
            MIX_filteradd_5068(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), i, filterDesc);
        return 0;
    }
    return -7;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_filterremove);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void MIX_filterremove(int, int);
struct G_5128 { char pad[0x2F]; unsigned char a; unsigned char b; char pad2[0x1BF]; char *tab; };

/**
 * Removes a custom filter from each software-mixer voice of a channel: the one with the given order (MIX_filterremove).
 *
 * Only render kind 4 (mixer) channels; reached through SNDCTRL_filterremove.
 *
 * @param channelIndex  the channel index
 * @param filterOrder  the filter's order (+0x18 of the node)
 * @return 0
 */
int SNDPLATFORM_filterremove(int channelIndex, int filterOrder) {
    struct CHANPUB *channel;
    int i;
    channel = &sndgs.chan[channelIndex];
    if (channel->rendermode & 4) {
        for (i = 0; i < channel->numchan; i++)
            MIX_filterremove(channel->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), filterOrder);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_outputlatency);
#ifdef SKIP_ASM
/**
 * Returns the platform's output latency: always 0 on PS2.
 *
 * SNDPROFILE_outputlatency returns it when the system is started.
 *
 * @return 0
 */
int SNDPLATFORM_outputlatency(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", Util_MemCpy);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).


// PORT: PS2-only hand-written memcpy (16-byte lq/sq blocks of 0x80, ld/sd and byte tails, unfilled delay slots; it branches into func_003C52C8, which loops back into it); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl Util_MemCpy\n.type Util_MemCpy, @function\nUtil_MemCpy:\n"
    "slti $8, $6, 0x12\n"
    ".word 0x15000029 /* bnez $8, .L003C5294: raw word */\n"
    "andi $8, $4, 0xF\n"
    ".word 0x15000034 /* bnez $8, func_003C52C8: raw word (cross-function branch) */\n"
    "nop\n"
    ".L003C51FC:\n"
    "andi $8, $5, 0xF\n"
    ".word 0x15000024 /* bnez $8, .L003C5294: raw word */\n"
    "nop\n"
    "addiu $6, $6, -0x80\n"
    ".word 0x04C00015 /* bltz $6, .L003C5264: raw word */\n"
    "nop\n"
    ".L003C5214:\n"
    "lq $9, 0x0($5)\n"
    "lq $10, 0x10($5)\n"
    "lq $11, 0x20($5)\n"
    "lq $12, 0x30($5)\n"
    "sq $9, 0x0($4)\n"
    "sq $10, 0x10($4)\n"
    "sq $11, 0x20($4)\n"
    "sq $12, 0x30($4)\n"
    "lq $9, 0x40($5)\n"
    "lq $10, 0x50($5)\n"
    "lq $11, 0x60($5)\n"
    "lq $12, 0x70($5)\n"
    "sq $9, 0x40($4)\n"
    "sq $10, 0x50($4)\n"
    "sq $11, 0x60($4)\n"
    "sq $12, 0x70($4)\n"
    "addiu $6, $6, -0x80\n"
    "addiu $5, $5, 0x80\n"
    ".word 0x04C1FFED /* bgez $6, .L003C5214: raw word */\n"
    "addiu $4, $4, 0x80\n"
    ".L003C5264:\n"
    "addiu $6, $6, 0x78\n"
    ".word 0x04C00009 /* bltz $6, .L003C5290: raw word */\n"
    "nop\n"
    ".L003C5270:\n"
    "ld $9, 0x0($5)\n"
    "addiu $5, $5, 0x8\n"
    "sd $9, 0x0($4)\n"
    "addiu $6, $6, -0x8\n"
    ".word 0x04C1FFFB\n"
    "addiu $4, $4, 0x8\n"
    ".word 0x04C1FFF9 /* bgez $6, .L003C5270: raw word */\n"
    "nop\n"
    ".L003C5290:\n"
    "addiu $6, $6, 0x8\n"
    ".L003C5294:\n"
    "addiu $6, $6, -0x1\n"
    ".word 0x04C00009 /* bltz $6, .L003C52C0: raw word */\n"
    "nop\n"
    ".L003C52A0:\n"
    "addiu $6, $6, -0x1\n"
    "lbu $9, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $9, 0x0($4)\n"
    ".word 0x04C1FFFB\n"
    "addiu $4, $4, 0x1\n"
    ".word 0x04C1FFF9 /* bgez $6, .L003C52A0: raw word */\n"
    "nop\n"
    ".L003C52C0:\n"
    "jr $31\n"
    "nop\n"
    ".size Util_MemCpy, . - Util_MemCpy\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C52C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).


// PORT: PS2-only hand-written byte-align head of the memcpy above (jumps back into Util_MemCpy once the destination is 16-byte aligned); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl func_003C52C8\n.type func_003C52C8, @function\nfunc_003C52C8:\n"
    "andi $8, $4, 0xF\n"
    ".word 0x1100FFCB /* beqz $8, .L003C51FC: raw word (cross-function branch) */\n"
    "nop\n"
    "addiu $6, $6, -0x1\n"
    ".word 0x18C00005 /* blez $6, .L003C52F0: raw word */\n"
    "lbu $9, 0x0($5)\n"
    "addiu $5, $5, 0x1\n"
    "sb $9, 0x0($4)\n"
    "j func_003C52C8\n"
    "addiu $4, $4, 0x1\n"
    ".L003C52F0:\n"
    "jr $31\n"
    "nop\n"
    ".size func_003C52C8, . - func_003C52C8\n"
    ".set pop\n");
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetMaxBanks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_52F8(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_52F8(void *) __asm__("SNDSYS_setopts");

/**
 * Sets the number of bank slots in the sound options (+0x28) and applies them.
 *
 * Reads the options with SNDSYS_getopts, changes the field and writes them back with SNDSYS_setopts. No callers in the
 * game.
 *
 * @param value  the number of bank slots
 * @return 0
 */
int System_SetMaxBanks(int value) {
    char sp[0xE0];
    SNDSYS_getopts_52F8(sp);
    *(short *)(sp + 0x28) = value;
    SNDSYS_setopts_52F8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_GetMaxBanks);
#ifdef SKIP_ASM
extern void SNDSYS_getopts(void *);

/**
 * Reads the number of bank slots (+0x28) from the sound options (SNDSYS_getopts). No callers in the game.
 *
 * @param outValue  receives the count
 * @return 0
 */
int System_GetMaxBanks(int *outValue) {
    char sp[0xE0];
    SNDSYS_getopts(sp);
    *outValue = *(unsigned short *)(sp + 0x28);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetMaxStreams);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5368(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5368(void *) __asm__("SNDSYS_setopts");

/**
 * Sets the maximum number of streams in the sound options (+0x46) and applies them.
 *
 * SNDPLATFORM_outputset caps the value at 32. No callers in the game.
 *
 * @param value  the number of streams
 * @return 0
 */
int System_SetMaxStreams(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5368(sp);
    *(signed char *)(sp + 0x46) = value;
    SNDSYS_setopts_5368(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetStealEqualPriorityVoices);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_53A0(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_53A0(void *) __asm__("SNDSYS_setopts");

/**
 * Sets whether a new sound may steal a voice of equal priority (option byte +0x3C) and applies the options.
 *
 * SNDVOICEI_alloc only steals lower priorities when it is 0; SNDSYS_getopts defaults it to 1. No callers in the game.
 *
 * @param value  non-zero to allow it
 * @return 0
 */
int System_SetStealEqualPriorityVoices(int value) {
    char sp[0xE0];
    SNDSYS_getopts_53A0(sp);
    *(signed char *)(sp + 0x3C) = value;
    SNDSYS_setopts_53A0(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetRandomSeed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_53D8(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_53D8(void *) __asm__("SNDSYS_setopts");

/**
 * Sets the random seed in the sound options (+0x24) and applies them.
 *
 * The start-up SNDSYSI_init seeds the generator with it (SNDI_randomseed). No callers in the game.
 *
 * @param value  the seed
 * @return 0
 */
int System_SetRandomSeed(int value) {
    char sp[0xE0];
    SNDSYS_getopts_53D8(sp);
    *(int *)(sp + 0x24) = value;
    SNDSYS_setopts_53D8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetUpdateRate);
#ifdef SKIP_ASM
extern void SNDSYS_setopts(void *);

/**
 * Sets the sound update rate: stores the period 1000 / rate ms in the options (+0x34) and applies them.
 *
 * A negative rate is stored as is (the -1.0f default makes SNDPLATFORM_init drive the sound thread from VBlank). No
 * callers in the game.
 *
 * @param rate  updates per second, or negative for the default
 * @return 0
 */
int System_SetUpdateRate(float rate) {
    char buf[0xE0];

    if (rate >= 0.0f) {
        rate = 1000.0f / rate;
    }
    SNDSYS_getopts(buf);
    *(float *)(buf + 0x34) = rate;
    SNDSYS_setopts(buf);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_GetUpdateRate);
#ifdef SKIP_ASM
/**
 * Reads the sound update rate: 1000 / the period in the options (+0x34), or the negative default as is.
 *
 * No callers in the game.
 *
 * @param outRate  receives the rate
 * @return 0
 */
int System_GetUpdateRate(float *outRate) {
    char buf[0xE0];
    float period;

    SNDSYS_getopts(buf);
    period = *(float *)(buf + 0x34);
    *outRate = period;
    if (period >= 0.0f) {
        *outRate = 1000.0f / period;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_CapOutputMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SNDSYS_getopts_i(void *) __asm__("SNDSYS_getopts");
/**
 * Reports whether an output mode is supported.
 *
 * Mono (0), DTS 4.0 (4), DTS 5.1 (5) and Dolby Pro Logic II (8) always are; stereo (1) when the platform allows at
 * least two output channels (options byte +0x05); the others (4.0, 5.1, Dolby Digital, Pro Logic) are not. Called by
 * System_SetOutputMode.
 *
 * @param kind  the output mode (as System_GetOutputModeName names them)
 * @param out  receives 1 when supported, else 0
 * @return 0
 */
int System_CapOutputMode(unsigned int kind, int *out) {
    unsigned char info[0xE0];
    *out = 0;
    SNDSYS_getopts_i(info);
    switch (kind) {
    case 1:
        if (info[5] >= 2U) {
            *out = 1;
        }
        break;
    case 0:
    case 4:
    case 5:
    case 8:
        *out = 1;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_GetOutputModeName);
#ifdef SKIP_ASM
extern char str_Mono[];
extern char str_Stereo[];
extern char str_4_0[];
extern char str_5_1[];
extern char str_DTS_4_0[];
extern char str_DTS_5_1[];
extern char str_Dolby_Digital_5_1[];
extern char str_Dolby_ProLogic[];
extern char str_Dolby_ProLogic_II[];

/**
 * Returns the name of an output mode.
 *
 * 0 "Mono", 1 "Stereo", 2 "4.0", 3 "5.1", 4 "DTS 4.0", 5 "DTS 5.1", 6 "Dolby Digital 5.1", 7 "Dolby ProLogic", 8 "Dolby
 * ProLogic II". No callers in the game.
 *
 * @param kind  the output mode
 * @param out  receives the name
 * @return 0, or -5 for an unknown mode
 */
int System_GetOutputModeName(unsigned int kind, char **out) {
    switch (kind) {
    case 0:
        *out = str_Mono;
        break;
    case 1:
        *out = str_Stereo;
        break;
    case 2:
        *out = str_4_0;
        break;
    case 3:
        *out = str_5_1;
        break;
    case 4:
        *out = str_DTS_4_0;
        break;
    case 5:
        *out = str_DTS_5_1;
        break;
    case 6:
        *out = str_Dolby_Digital_5_1;
        break;
    case 7:
        *out = str_Dolby_ProLogic;
        break;
    case 8:
        *out = str_Dolby_ProLogic_II;
        break;
    default:
        return -5;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetOutputMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int outputmode_5600[] __asm__("outputmode");
int System_CapOutputMode_5600(int, int *) __asm__("System_CapOutputMode");

/**
 * Selects the output mode used at the next start (outputmode), if System_CapOutputMode says it is supported.
 *
 * System_Init builds the render modes and output channels from it. No callers in the game.
 *
 * @param outputMode  the output mode
 * @return 0, or -5 when unsupported
 */
int System_SetOutputMode(int outputMode) {
    int buf[4];
    System_CapOutputMode_5600(outputMode, buf);
    if (buf[0] == 0) {
        return -5;
    }
    outputmode_5600[0] = outputMode;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_GetOutputMode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int outputmode_5648[4] __asm__("outputmode");

/**
 * Reads the selected output mode (outputmode). No callers in the game.
 *
 * @param outMode  receives the mode
 * @return 0
 */
int System_GetOutputMode(int *outMode) {
    *outMode = outputmode_5648[0];
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_CapOutputSampleRate);
#ifdef SKIP_ASM
extern int SNDSYS_getopts_5660(struct SNDSYSOPTS *) __asm__("SNDSYS_getopts");

/**
 * Returns the supported output rate closest to a requested one for a device.
 *
 * Devices 0 and 1 round up to 24000, 36000 or 48000 (anything above 36000 gives 48000). Reads the options but does not
 * use them. No callers in the game.
 *
 * @param mode  the device (0 or 1)
 * @param rate  the requested rate
 * @param out  receives the supported rate
 * @return 0, or -5 for an unknown device
 */
int System_CapOutputSampleRate(int mode, int rate, int *out) {
    struct SNDSYSOPTS cfg;
    SNDSYS_getopts_5660(&cfg);
    switch (mode) {
    case 0:
        if (rate <= 24000) *out = 24000;
        else if (rate <= 36000) *out = 36000;
        else *out = 48000;
        break;
    case 1:
        if (rate <= 24000) *out = 24000;
        else if (rate <= 36000) *out = 36000;
        else *out = 48000;
        break;
    default:
        return -5;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetOutputSampleRate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int SNDSYS_getopts_5710(void *) __asm__("SNDSYS_getopts");


/**
 * Sets a device's output rate in the sound options and applies them.
 *
 * The value is capped at 0xFFFF; device 0 sets +0x2A (the mixer rate), device 1 +0x2C. A stray store first puts the low
 * byte into +0x2A, which device 1 leaves behind (likely bug). No callers in the game.
 *
 * @param index  the device (0 or 1)
 * @param value  the rate in Hz
 * @return 0, or -5 for an unknown device
 */
int System_SetOutputSampleRate(int index, int value) {
    char buf[0xE0];
    SNDSYS_getopts_5710(buf);
    { int lim = 0xFFFF; value = value > lim ? lim : value; }
    *(short *)(buf + 0x2A) = value & 0xFF;
    switch (index) {
    case 0:
        *(short *)(buf + 0x2A) = value;
        break;
    case 1:
        *(short *)(buf + 0x2C) = value;
        break;
    default:
        return -5;
    }
    SNDSYS_setopts(buf);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_GetOutputSampleRate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct Buf_5788 { char pad[0x2A]; unsigned short a; unsigned short b; char rest[0xE0 - 0x2E]; } Buf_5788;
extern void SNDSYS_getopts_5788(Buf_5788 *) __asm__("SNDSYS_getopts");

/**
 * Reads a device's output rate from the sound options (+0x2A for 0, +0x2C for 1). No callers in the game.
 *
 * @param index  the device
 * @param outValue  receives the rate (0 for an unknown device)
 * @return 0, or -5 for an unknown device
 */
int System_GetOutputSampleRate(int index, int *outValue) {
    Buf_5788 sp;

    *outValue = 0;
    SNDSYS_getopts_5788(&sp);
    switch (index) {
    case 0:
        *outValue = sp.a;
        break;
    case 1:
        *outValue = sp.b;
        break;
    default:
        return -5;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_CapVoices);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDSYS_getopts_57F0(void *) __asm__("SNDSYS_getopts");

/**
 * Returns the most voices a device supports: options byte +0x06, +0x07 or +0x08 for device 0, 1 or 2.
 *
 * No callers in the game.
 *
 * @param index  the device
 * @param outValue  receives the limit (0 for an unknown device)
 * @return 0, or -5 for an unknown device
 */
int System_CapVoices(int index, int *outValue) {
    unsigned char buf[0xE0];
    *outValue = 0;
    SNDSYS_getopts_57F0(buf);
    switch (index) {
    case 0: *outValue = buf[6]; break;
    case 1: *outValue = buf[7]; break;
    case 2: *outValue = buf[8]; break;
    default: return -5;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetVoices);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Stores v into config byte 0x2E/0x2F/0x30 (which 0..2) and writes the config back.
extern int SNDSYS_getopts_5880(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5880(void *) __asm__("SNDSYS_setopts");

/**
 * Sets a device's voice count (options byte +0x2E, +0x2F or +0x30) and applies the options.
 *
 * No callers in the game.
 *
 * @param which  the device (0-2)
 * @param value  the number of voices
 * @return 0, or -5 for an unknown device
 */
int System_SetVoices(int which, int value) {
    char sp[0xE0];
    SNDSYS_getopts_5880(sp);
    switch (which) {
    case 0:
        sp[0x2E] = value;
        break;
    case 1:
        sp[0x2F] = value;
        break;
    case 2:
        sp[0x30] = value;
        break;
    default:
        return -5;
    }
    SNDSYS_setopts_5880(sp);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_GetVoices);
#ifdef SKIP_ASM
/**
 * Reads a device's voice count (options byte +0x2E, +0x2F or +0x30). No callers in the game.
 *
 * @param index  the device (0-2)
 * @param outValue  receives the count
 * @return 0, or -5 for an unknown device
 */
int System_GetVoices(int index, int *outValue) {
    unsigned char buf[0xE0];
    SNDSYS_getopts_5880(buf);
    switch (index) {
    case 0: *outValue = buf[0x2E]; break;
    case 1: *outValue = buf[0x2F]; break;
    case 2: *outValue = buf[0x30]; break;
    default: return -5;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetResampleQuality);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5998(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5998(void *) __asm__("SNDSYS_setopts");

/**
 * Sets a device's resampling quality: quality * 100 into options byte +0x40 (device 0) or +0x41 (device 1).
 *
 * No callers in the game.
 *
 * @param index  the device
 * @param value  the quality (0-1)
 * @return 0, or -5 for an unknown device
 */
int System_SetResampleQuality(int index, float value) {
    char sp[0xE0];
    SNDSYS_getopts_5998(sp);
    switch (index) {
    case 0:
        *(signed char *)(sp + 0x40) = (int)(value * 100.0f);
        break;
    case 1:
        *(signed char *)(sp + 0x41) = (int)(value * 100.0f);
        break;
    default:
        return -5;
    }
    SNDSYS_setopts_5998(sp);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_GetResampleQuality);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Reads byte 0x40 (which 0) or 0x41 (which 1) of the current config as a 0..1 float.
extern void SNDSYS_getopts_5A28(void *) __asm__("SNDSYS_getopts");

/**
 * Reads a device's resampling quality: options byte +0x40 or +0x41 times 0.01. No callers in the game.
 *
 * @param which  the device
 * @param out  receives the quality
 * @return 0, or -5 for an unknown device
 */
int System_GetResampleQuality(int which, float *out) {
    unsigned char sp[0xE0];
    SNDSYS_getopts_5A28(sp);
    switch (which) {
    case 0:
        *out = sp[0x40] * 0.01f;
        break;
    case 1:
        *out = sp[0x41] * 0.01f;
        break;
    default:
        return -5;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AA0);
#ifdef SKIP_ASM
/**
 * Unsupported option setter: returns -5. No callers in the game.
 *
 * @return -5
 */
int func_003C5AA0(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AA8);
#ifdef SKIP_ASM
/**
 * Unsupported option setter: returns -5. No callers in the game.
 *
 * @return -5
 */
int func_003C5AA8(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AB0);
#ifdef SKIP_ASM
/**
 * Unsupported option setter: returns -5. No callers in the game.
 *
 * @return -5
 */
int func_003C5AB0(void) {
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5AB8);
#ifdef SKIP_ASM
/**
 * Unsupported option setter: returns -5. No callers in the game.
 *
 * @return -5
 */
int func_003C5AB8(void) {
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetSndLoadsIopModules);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5AC0(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5AC0(void *) __asm__("SNDSYS_setopts");

/**
 * Sets whether the sound system loads its own IOP modules (options byte +0x3F) and applies the options.
 *
 * SNDPLATFORM_init loads them at the first start when it is set. No callers in the game.
 *
 * @param value  non-zero to load them
 * @return 0
 */
int System_SetSndLoadsIopModules(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5AC0(sp);
    *(signed char *)(sp + 0x3F) = value;
    SNDSYS_setopts_5AC0(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetIopModulePath);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5AF8(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5AF8(void *) __asm__("SNDSYS_setopts");

/**
 * Sets the directory the IOP sound modules are loaded from (options word +0x6C) and applies the options.
 *
 * SNDPLATFORM_init builds the module paths from it. No callers in the game.
 *
 * @param value  the directory string
 * @return 0
 */
int System_SetIopModulePath(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5AF8(sp);
    *(int *)(sp + 0x6C) = value;
    SNDSYS_setopts_5AF8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetDtsQuality);
#ifdef SKIP_ASM
typedef struct { char pad[0x4A]; signed char v; char pad2[0xE0 - 0x4B]; } S_5B30;
extern void SNDSYS_setopts(void *);

/**
 * Sets the DTS encoder quality: quality * 100 into options byte +0x4A, then applies the options.
 *
 * SNDPLATFORM_outputset caps it at 100 and SNDPLATFORM_init hands it to the encoder's size and setup hooks. No callers in the
 * game.
 *
 * @param value  the quality (0-1)
 * @return 0
 */
int System_SetDtsQuality(float value) {
    S_5B30 sysOpts;
    SNDSYS_getopts(&sysOpts);
    sysOpts.v = (int)(value * 100.0f);
    SNDSYS_setopts(&sysOpts);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetRouteMainFxToIop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5B80(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5B80(void *) __asm__("SNDSYS_setopts");

/**
 * Sets options byte +0x42 and applies the options.
 *
 * When set, func_003C1638 adds the extra tagged block to the IOP command buffer and func_003C3450 / func_003C35F8 route
 * the mixer's effect to the IOP (func_003CC878). No callers in the game.
 *
 * @param value  the flag
 * @return 0
 */
int System_SetRouteMainFxToIop(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5B80(sp);
    *(signed char *)(sp + 0x42) = value;
    SNDSYS_setopts_5B80(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetRouteHardwareFxToIop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5BB8(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5BB8(void *) __asm__("SNDSYS_setopts");

/**
 * Sets options byte +0x43 and applies the options.
 *
 * SNDPLATFORM_init passes it in the IOP init command when there are at least four output channels. No callers in the game.
 *
 * @param value  the flag
 * @return 0
 */
int System_SetRouteHardwareFxToIop(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5BB8(sp);
    *(signed char *)(sp + 0x43) = value;
    SNDSYS_setopts_5BB8(sp);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C5BF0);
#ifdef SKIP_ASM
/**
 * Unsupported option setter: returns -5. No callers in the game.
 *
 * @return -5
 */
int func_003C5BF0(void) {
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_SetAbortHandler);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDSYS_getopts_5BF8(void *) __asm__("SNDSYS_getopts");
extern void SNDSYS_setopts_5BF8(void *) __asm__("SNDSYS_setopts");

/**
 * Installs the sound library's abort-message handler: stores @p handler in the options' vector block (SNDSYSVEC abortmsg,
 * options +0xC8, D_0050A9B0) and applies the options.
 *
 * System_VectorToReal5 fills the same slot with SNDREAL_abortmsg. No callers in the game.
 *
 * @param handler  the abort-message routine
 * @return 0
 */
int System_SetAbortHandler(int value) {
    char sp[0xE0];
    SNDSYS_getopts_5BF8(sp);
    *(int *)(sp + 0xC8) = value;
    SNDSYS_setopts_5BF8(sp);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_Init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SNDSYS_setopts_i(void *) __asm__("SNDSYS_setopts");
extern int SNDSYSI_init(int, int, int);
extern int pheap;
extern unsigned int outputmode;
extern int heapsize;

struct sInfo_5C30 {
    char pad0[0x2A];
    unsigned short w2A;
    unsigned short w2C;
    char pad2E;
    unsigned char b2F;
    char pad30[0x47 - 0x30];
    signed char f47;
    signed char f48;
    signed char f49;
    char pad4A[2];
    unsigned char n;
    char pad4D;
    short list[8];
    char tail[0xE0 - 0x5E];
};

/**
 * Starts the sound system with the selected output mode, in the heap set by Memory_SetHeap.
 *
 * Builds the render-mode list (options +0x4C count, +0x4E list): 0x24 and 0x28 always, 0x120 when there are kind-0x100
 * voices (+0x2F), and 0x44, 0x140, 0x48 for Dolby Pro Logic II (mode 8, outputmode). Output channels +0x47: 1 for mono,
 * 4 / 5 for DTS 4.0 / 5.1 (option bits +0x49 = 2, DTS), 2 otherwise; +0x48 is cleared. The second device rate +0x2C
 * follows the first unless they are equal or 36000 / 24000. Then SNDSYS_setopts and SNDSYSI_init(pheap,
 * heapsize, @p headerVersion).
 *
 * @param headerVersion  passed to SNDSYSI_init as its third argument
 * @return 0
 */
int System_Init(int headerVersion) {
    struct sInfo_5C30 info;
    int mainRate;
    int ia;

    SNDSYS_getopts(&info);
    info.n = 0;
    if (outputmode == 8) {
        info.list[0] = 68;
        info.n = 1;
    }
    info.list[info.n] = 36;
    info.n += 1;
    if (info.b2F != 0) {
        if (outputmode == 8) {
            info.list[info.n] = 320;
            info.n += 1;
        }
        info.list[info.n] = 288;
        info.n += 1;
    }
    if (outputmode == 8) {
        info.list[info.n] = 72;
        info.n += 1;
    }
    info.list[info.n] = 40;
    info.f49 = 1;
    info.f48 = 0;
    info.n += 1;
    switch (outputmode) {
    case 0:
        info.f47 = 1;
        break;
    case 4:
        info.f47 = 4;
        info.f49 = 2;
        break;
    case 5:
        info.f47 = 5;
        info.f49 = 2;
        break;
    case 1:
    case 8:
        info.f47 = 2;
        break;
    }
    ia = info.w2A;
    mainRate = ia & 0xFFFF;
    if (mainRate != info.w2C && (mainRate != 36000 || info.w2C != 24000)) {
        info.w2C = ia;
    }
    SNDSYS_setopts_i(&info);
    SNDSYSI_init(pheap, heapsize, headerVersion);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", System_IsInited);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns whether the sound system is started (+0x174 of sndgs, D_0050AA5C, equal to 1).
 *
 * @return 1 when started, else 0
 */
int System_IsInited(void) {
    return sndgs.installed == 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", System_Restore);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int pheap_5E08[] __asm__("pheap");
void SNDSYS_restore_5E08(void) __asm__("SNDSYS_restore");
int System_Restore_5E08(void) __asm__("System_Restore");
/**
 * Shuts the sound system down (SNDSYS_restore) and clears the heap pointer pheap.
 *
 * SNDREAL_exithandler calls it at program exit.
 *
 * @return 0
 */
int System_Restore_5E08(void) {
    SNDSYS_restore_5E08();
    pheap_5E08[0] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", Memory_SetHeap);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int pheap_5E30[] __asm__("pheap");
extern int heapsize_5E30[] __asm__("heapsize");
/**
 * Sets the heap the sound system starts in: pheap and heapsize for device 0.
 *
 * System_Init passes them to SNDSYSI_init. No callers in the game.
 *
 * @param device  the device (only 0)
 * @param heap  the heap
 * @param heapSize  its size
 * @return 0, or -5 for another device
 */
int Memory_SetHeap(int device, int heap, int heapSize) {
    if (device == 0) {
        pheap_5E30[0] = heap;
        heapsize_5E30[0] = heapSize;
        return 0;
    }
    return -5;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", Memory_GetHeap);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int System_IsInited();
extern int pheap_5E58[] __asm__("pheap");
extern int heapsize_5E58[] __asm__("heapsize");

/**
 * Returns the sound heap of device 0 (pheap, heapsize), once the system is started (System_IsInited).
 *
 * Both outputs are cleared first. No callers in the game.
 *
 * @param device  the device (only 0)
 * @param outHeap  receives the heap
 * @param outSize  receives its size
 * @return 0, -4 when not started, -5 for another device
 */
int Memory_GetHeap(int device, int *outHeap, int *outSize) {
    *outHeap = 0;
    *outSize = 0;
    if (device == 0) {
        if (System_IsInited() == 0) {
            return -4;
        }
        *outHeap = pheap_5E58[0];
        *outSize = heapsize_5E58[0];
    } else {
        return -5;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", Memory_SetHeapFastRam);
#ifdef SKIP_ASM
typedef struct { char pad0[0x64]; int a; int b; char pad1[0xE0 - 0x6C]; } S_5ED0;
extern void SNDSYS_setopts(void *);

/**
 * Sets a fast work buffer and its size (options +0x64, +0x68) and applies the options.
 *
 * SNDPLATFORM_init uses it as the DTS encoder's work area (typically the scratchpad) when it is big enough, saving and
 * restoring it around each encode. No callers in the game.
 *
 * @param firstValue  the buffer
 * @param secondValue  its size
 * @return 0
 */
int Memory_SetHeapFastRam(int firstValue, int secondValue) {
    S_5ED0 sysOpts;
    SNDSYS_getopts(&sysOpts);
    sysOpts.a = firstValue;
    sysOpts.b = secondValue;
    SNDSYS_setopts(&sysOpts);
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", Memory_SetHeapThreshold);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SNDSYS_setopts_5F18(char *) __asm__("SNDSYS_setopts");

/**
 * Sets device 0's heap threshold: threshold * 100 into options byte +0x4B, then applies the options.
 *
 * SNDSYS_getopts defaults the byte to 90. No callers in the game.
 *
 * @param index  the device (only 0)
 * @param value  the threshold (0-1)
 * @return 0, or -5 for another device
 */
int Memory_SetHeapThreshold(int index, float value) {
    char buf[0xE0];
    if (index == 0) {
        SNDSYS_getopts(buf);
        buf[0x4B] = (int)(value * 100.0f);
        SNDSYS_setopts_5F18(buf);
        return 0;
    }
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", Memory_GetHeapThreshold);
#ifdef SKIP_ASM
extern void SNDSYS_getopts(void *);

/**
 * Reads device 0's heap threshold: options byte +0x4B times 0.01. No callers in the game.
 *
 * @param index  the device (only 0)
 * @param outValue  receives the threshold (0 for another device)
 * @return 0, or -5 for another device
 */
int Memory_GetHeapThreshold(int index, float *outValue) {
    unsigned char buf[0xE0];

    *outValue = 0.0f;
    if (index == 0) {
        SNDSYS_getopts(buf);
        *outValue = (float)buf[0x4B] * 0.01f;
        return 0;
    }
    return -5;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", Profile_SetFunctionProfiler);
#ifdef SKIP_ASM
typedef struct { char pad0[0xCC]; int a; int b; char pad1[0xE0 - 0xD4]; } S_5FD0;
extern void SNDSYS_setopts(void *);

/**
 * Installs the sound library's function-profiler hooks: stores @p enter and @p leave in the options' vector block
 * (SNDSYSVEC profileenter / profileleave, options +0xCC / +0xD0) and applies the options.
 *
 * Nothing in this build calls the hooks back. No callers in the game.
 *
 * @param enter  the hook for function entry
 * @param leave  the hook for function exit
 * @return 0
 */
int Profile_SetFunctionProfiler(int firstValue, int secondValue) {
    S_5FD0 sysOpts;
    SNDSYS_getopts(&sysOpts);
    sysOpts.a = firstValue;
    sysOpts.b = secondValue;
    SNDSYS_setopts(&sysOpts);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDserveraddclient);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_6018 { char pad[0x178]; signed char n; char pad2[0x23]; int v[1]; };
/**
 * Adds a function to the sound server's client list.
 *
 * Appends it to the table at +0x17C of sndgs (count +0x178), whose entries SNDSYS_service calls while the system
 * runs. No bounds check. SNDSTRMI_create registers the stream service this way.
 *
 * @param client  the client function
 */
void iSNDserveraddclient(int client) {
    sndgs.serverclient[sndgs.numserverclients] = (void (*)(void))client;
    sndgs.numserverclients = sndgs.numserverclients + 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", iSNDserverremoveclient);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x178]; signed char n; char pad2[0x23]; int arr[1]; } S_6040;

extern S_6040 sndgs_6040 __asm__("sndgs");
/**
 * Removes a function from the sound server's client list (+0x17C of sndgs), closing the gap.
 *
 * Does nothing when it is not listed. Called by SNDSTRM_destroy.
 *
 * @param client  the client function
 */
void iSNDserverremoveclient(int client) {
    int i;

    for (i = 0; i < sndgs_6040.n; i++) {
        if (sndgs_6040.arr[i] == client) {
            sndgs_6040.n--;
            while (i < sndgs_6040.n) {
                sndgs_6040.arr[i] = sndgs_6040.arr[i + 1];
                i++;
            }
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDSYS_service);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// When the flag at 0x174 is set, calls each callback in the list at 0x19C.
struct G_60E0 { char p[0x174]; signed char on; char p1[3]; signed char n; char p2[0x19C - 0x179]; void (*fn[1])(void); };
/**
 * Runs the sound server's clients: while the system is started (+0x174 of sndgs), calls each function in the
 * client list (+0x17C, count +0x178).
 *
 * SNDREAL_systemtask calls it every frame and SNDPLATFORM_restore once more at shutdown.
 */
void SNDSYS_service(void) {
    int i;
    if (sndgs.installed) {
        for (i = 0; i < sndgs.numserverclients; i++) sndgs.serverclient[i]();
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_validrendermode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Scans the mask table at 0x4E from *idx for an entry whose 0x71C/0xE0 bits
// match the element's flags at 0x18; returns the matching bits (0 if none).
struct G_6158 { char p[0x4C]; unsigned char n; char p1; unsigned short tbl[1]; };

extern struct G_6158 sndgs_6158 __asm__("sndgs");
/**
 * Finds the next render mode, from the list index on, that a sound can be played in.
 *
 * Walks the render-mode list (+0x4C count, +0x4E 16-bit modes of sndgs, built by System_Init). A mode fits when
 * its kind bits (mask 0x71C) overlap the sound's flags at +0x18 of the header (any kind when the sound names none), and
 * likewise its format bits (mask 0xE0); a mode with bit 0x80 only fits a one-channel sound (header byte +0x03). Leaves
 * the index past the mode it returns, or at the end. Called by SNDBANKI_playtimbre and SNDPKTPLAY_start.
 *
 * @param idx  in/out: the list index to start at
 * @param patchHeader  the sound's header (+0x03 channel count, +0x18 flags)
 * @return the fitting mode's matching bits, or 0 when none is left
 */
int SNDI_validrendermode(int *idx, struct SNDIPATCHHEADER *patchHeader) {
    int matchBits = 0;
    while (*idx < sndgs_6158.n) {
        int modeKindBits = sndgs_6158.tbl[*idx] & 0x71C;
        if (patchHeader->rendermode & 0x71C) {
            matchBits = patchHeader->rendermode & modeKindBits;
            if (matchBits != 0) goto ok;
            goto next;
        } else {
            matchBits = modeKindBits;
        }
    ok:
        {
            int modeFormatBits = sndgs_6158.tbl[*idx] & 0xE0;
            if (patchHeader->rendermode & 0xE0) {
                int formatMatch = patchHeader->rendermode & modeFormatBits;
                matchBits |= formatMatch;
                if (formatMatch == 0) { matchBits = 0; goto next; }
            } else {
                matchBits |= modeFormatBits;
            }
        }
        if (patchHeader->numchan != 1 && (matchBits & 0x80)) { matchBits = 0; goto next; }
        if (matchBits) { (*idx)++; break; }
    next:
        (*idx)++;
    }
    return matchBits;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMEM_gethighwater);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SNDMEM_gethighwater_6240(void) __asm__("SNDMEM_gethighwater");

/**
 * Returns the sound heap's high-water mark as a percentage: (size - lowest free mark) * 100 / size.
 *
 * Reads the heap state (D_0050AB1C: +0x08 size, +0x10 low mark). Called by SNDMEMI_restore.
 *
 * @return the most of the heap ever used, in percent
 */
int SNDMEM_gethighwater_6240(void) {
    int temp_3;

    temp_3 = (*(int *)((char*)((char*)sndgs.mm) + (8)));
    return (int) ((temp_3 - (*(int *)((char*)((char*)sndgs.mm) + (0x10)))) * 100) / temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDLINKI_init);
#ifdef SKIP_ASM
/**
 * Empties a linked list: clears its head (+0x00), tail (+0x04) and count (+0x08). Called by SNDSTRMI_create.
 *
 * @param list  the list
 */
void SNDLINKI_init(SNDLINKLIST* list) {
    list->phead = 0;
    list->ptail = 0;
    list->items = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDLINKI_push);
#ifdef SKIP_ASM
/**
 * Inserts a node at the head of a linked list.
 *
 * Lists are {head, tail, count}, nodes {next, prev}; the old head's prev (or the tail of an empty list) becomes the
 * node and the count grows. Called by SNDSTRMI_create, SNDSTRMI_removerequest and SNDSTRM_purge.
 *
 * @param list  the list
 * @param node  the node
 */
void SNDLINKI_push(SNDLINKLIST* list, void *node) {
    void *temp_2;

    (*(void **)((char*)(node) + (0))) = (void *) (list->phead);
    (*(int *)((char*)(node) + (4))) = 0;
    temp_2 = ((void*)list->phead);
    if (temp_2 != 0) {
        (*(void **)((char*)(temp_2) + (4))) = node;
    } else {
        ((void*)list->ptail) = node;
    }
    ((void*)list->phead) = node;
    list->items = (int) ((list->items) + 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDLINKI_pushtail);
#ifdef SKIP_ASM
/**
 * Appends a node at the tail of a linked list ({head, tail, count}; nodes {next, prev}), growing the count.
 *
 * Called by SNDSTRMI_queue.
 *
 * @param list  the list
 * @param node  the node
 */
void SNDLINKI_pushtail(SNDLINKLIST* list, SNDLINKNODE* node) {
    SNDLINKNODE *temp_2;

    node->pnext = 0;
    node->pprev = list->ptail;
    temp_2 = list->ptail;
    if (temp_2 != 0) {
        temp_2->pnext = node;
    } else {
        list->phead = node;
    }
    list->ptail = node;
    list->items = (int) ((list->items) + 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDLINKI_pop);
#ifdef SKIP_ASM
/**
 * Removes the head node of a linked list and returns it.
 *
 * The new head's prev is cleared (or the tail, when the list empties) and the count drops. Called by SNDSTRMI_queue and
 * SNDSTRM_purge.
 *
 * @param list  the list
 * @return the node, or 0 when the list is empty
 */
void *SNDLINKI_pop(SNDLINKLIST* list) {
    SNDLINKNODE* temp_2;
    SNDLINKNODE *temp_3;

    temp_3 = list->phead;
    if (temp_3 != 0) {
        temp_2 = temp_3->pnext;
        list->phead = temp_2;
        if (temp_2 == 0) {
            list->ptail = 0;
        } else {
            temp_2->pprev = 0;
        }
        list->items = (int) ((list->items) - 1);
    }
    return temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDLINKI_remove);
#ifdef SKIP_ASM
/**
 * Unlinks a node from anywhere in a linked list, fixing the head, tail and both neighbours, and drops the count.
 *
 * The node's own links are left as they were. Called by SNDSTRMI_removerequest.
 *
 * @param list  the list
 * @param node  the node, which must be in it
 */
void SNDLINKI_remove(SNDLINKLIST* list, SNDLINKNODE* node) {
    void **temp_3;
    SNDLINKNODE* temp_3_2;

    if (((void*)node) == ((void*)list->phead)) {
        ((void*)list->phead) = (void *) (node->pnext);
    }
    if (((void*)node) == ((void**)list->ptail)) {
        ((void**)list->ptail) = (void **) ((void**)node->pprev);
    }
    temp_3 = ((void**)node->pprev);
    if (temp_3 != 0) {
        *temp_3 = ((void*)node->pnext);
    }
    temp_3_2 = node->pnext;
    if (temp_3_2 != 0) {
        temp_3_2->pprev = node->pprev;
    }
    list->items = (int) ((list->items) - 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C6380);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned int lo, hi; } L64_6380;
extern unsigned long f14D0_6380(unsigned long, unsigned long) __asm__("__muldi3");

/**
 * Multiplies two unsigned 32-bit values into a 64-bit result (low word, high word).
 *
 * Builds it from the four 16 x 16-bit partial products (__muldi3). SNDSTRM_requeststatus uses it with
 * func_003C64A8 to scale a position without overflow.
 *
 * @param lhs  the first factor
 * @param rhs  the second factor
 * @return the product as {low, high}
 */
L64_6380 func_003C6380(unsigned int lhs, unsigned int rhs) {
    unsigned long al = lhs & 0xFFFF;
    unsigned long bl = rhs & 0xFFFF;
    unsigned long ah = (lhs >> 16) & 0xFFFF;
    unsigned long bh = (rhs >> 16) & 0xFFFF;
    unsigned long ll, lh, hl, hh, mid;
    L64_6380 product;
    L64_6380 result;
    ll = f14D0_6380(al, bl);
    lh = f14D0_6380(al, bh);
    hl = f14D0_6380(ah, bl);
    hh = f14D0_6380(ah, bh);
    mid = (ll >> 16) + (lh & 0xFFFF) + (hl & 0xFFFF);
    product.lo = ll + (lh << 16) + (hl << 16);
    product.hi = (lh >> 16) + (hl >> 16) + hh + (mid >> 16);
    result = product;
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C64A8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// 64-bit by 32-bit unsigned shift-subtract divide; the dividend arrives as a by-value {lo, hi} pair.
typedef struct { unsigned int lo, hi; } U64_64A8;

/**
 * Divides a 64-bit unsigned value by a 32-bit one (shift and subtract) and returns the 32-bit quotient.
 *
 * Only the low 32 bits of the quotient survive; the remainder is dropped. Used by SNDSTRM_requeststatus with
 * func_003C6380.
 *
 * @param x  the dividend as {low, high}
 * @param divisor  the divisor
 * @return the quotient
 */
unsigned int func_003C64A8(U64_64A8 x, unsigned int divisor) {
    unsigned int lo, hi, quotient;
    int i;
    lo = x.lo;
    hi = x.hi;
    quotient = 0;
    for (i = 31; i >= 0; i--) {
        hi <<= 1;
        hi += lo >> 31;
        quotient <<= 1;
        lo <<= 1;
        if (hi >= divisor) {
            hi -= divisor;
            quotient++;
        }
    }
    return quotient;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_alloc);
#ifdef SKIP_ASM
struct G_6500 { char pad[0x28]; unsigned short count; char pad2[0x1F4 - 0x2A]; struct BANKLIST *slots; };

/**
 * Returns the first free bank slot: the index of the first unused 16-byte entry of the bank table (+0x1F4 of
 * sndgs, +0x28 entries).
 *
 * Called by SNDbankadd.
 *
 * @return the slot index, or -9 when all are in use
 */
int SNDBANKI_alloc(void) {
    int i;
    for (i = 0; i < sndgs.sso.set.maxbanks; i++) {
        if (((int)sndgs.banklist[i].phdr) == 0) {
            return i;
        }
    }
    return -9;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDBANKI_getppatch);
#ifdef SKIP_ASM
/**
 * Returns a bank's patch by number.
 *
 * The bank holds +0x06 patches and a table of 32-bit offsets at +0x14, each relative to its own entry. Called by
 * SNDBANK_play, SNDbankremove and func_003B6670.
 *
 * @param bank  the bank
 * @param patchNum  the patch number
 * @return the patch, or 0 when the number is out of range or the entry is empty
 */
void *SNDBANKI_getppatch(BANKVER5* bank, int patchNum) {
    int temp_3;

    if (patchNum >= bank->numpatches) {
        return 0;
    }
    temp_3 = (int)bank->patch[patchNum];
    if (temp_3 == 0) {
        return 0;
    }
    return (char*)&bank->patch[patchNum] + temp_3;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_gethighchannel);
#ifdef SKIP_ASM

/**
 * Returns which of a voice's channels runs on the highest hardware voice.
 *
 * Scans the first n entries of the voice's 16-bit hardware voice list (+0x04 of its record in the D_0050AAD8 table).
 * SNDPKTPLAY_start stores the result in the packet instance (+0x42); SNDPKTPLAYI_freeframes only counts frames played on that one.
 *
 * @param voice  the voice
 * @param n  the number of channels
 * @return the channel index, or -1 when n is 0
 */
int SNDPKTPLAYI_gethighchannel(int voice, int n) {
    int best = -1;
    int idx = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (best < sndgs.chan[voice].voices[i]) {
            best = sndgs.chan[voice].voices[i];
            idx = i;
        }
    }
    return idx;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_overhead);
#ifdef SKIP_ASM
/**
 * Returns the bytes a packet player instance needs for its own bookkeeping: maxPackets * 32 + 156.
 *
 * That is the instance header plus one 32-byte record per packet (SNDPKTPLAY_create sizes the ring the same way). Called by
 * SNDPKTPLAY_overhead.
 *
 * @param maxPackets  the number of packets the instance can hold
 * @return the size in bytes
 */
int SNDPKTPLAYI_overhead(int maxPackets) {
    return (maxPackets << 5) + 156;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_overhead);
#ifdef SKIP_ASM
int SNDPLATFORM_packetoverhead(void);
/**
 * Returns the memory a packet player instance needs: SNDPKTPLAYI_overhead's size plus the platform's (SNDPLATFORM_packetoverhead).
 *
 * Called by SNDSTRMI_create and SNDSTRM_overheadtap.
 *
 * @param maxPackets  the number of packets the instance can hold
 * @return the size in bytes
 */
int SNDPKTPLAY_overhead(int maxPackets) {
    int overhead = SNDPKTPLAYI_overhead(maxPackets);
    return overhead + SNDPLATFORM_packetoverhead();
}
#endif

#ifdef SKIP_ASM
/* EA SNDSAMPLEFORMAT (On Tour DWARF, 4 bytes): ealib/snd.h. */


/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
struct s7B8Rec {                    /* 0x20, s7B8Queue.f7C[] */
    int f00;                        /* sequence number */
    int len : 31;
    int flag : 1;
    unsigned int f08[6];            /* one per reader */
};

struct s7B8Queue {
    volatile int f00;               /* voice */
    volatile int f04;               /* records written */
    volatile unsigned int f08;
    volatile unsigned int f0C;
    volatile unsigned int f10[6];   /* per reader: records read */
    volatile short f28[6];          /* per reader: read index */
    volatile short f34[6];          /* per reader: records pending */
    volatile short f40;             /* number of records */
    volatile unsigned char f42;
    char pad43;
    volatile short f44;
    volatile short f46;             /* write index */
    volatile int f48;
    volatile int f4C;
    int f50;
    void (*f54)(int, int);
    void (*f58)(int, int, int);
    int f5C;
    SNDSAMPLEFORMAT f60;            /* sample format; f60.channels = number of readers */
    int f64[6];
    struct s7B8Rec f7C[1];
};

struct s7B8Event {                  /* 8 bytes: a callback to run (f00 == 0: f58, else f54) */
    unsigned short f00;
    unsigned short f02;             /* queue */
    int f04;
};

struct s7B8 {
    int f000;                       /* number of f004 entries */
    struct s7B8Event f004[0x60];
    struct s7B8Queue *f304[32];
};

extern struct s7B8 sndpps;
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_create);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
/* Uses the unit's struct s7B8 / s7B8Queue (sndpps, the stream queues). */
extern unsigned char D_0050A92E;
int SNDPLATFORM_packetplaycreate_6620(int, char *) __asm__("SNDPLATFORM_packetplaycreate");

/**
 * Creates a packet player instance in caller-supplied memory and returns its handle.
 *
 * Takes the first free slot of sndpps (+0x304 table, as many as the stream limit cfg +0x26), lets the platform set
 * up its part at the start of the memory (SNDPLATFORM_packetplaycreate, SNDPLATFORM_packetoverhead bytes), then places the instance after it:
 * capacity (memsize - 156) / 32 packets, the callbacks and client data, no voice yet (-1). All inside a critical
 * section. Called by SNDSTRMI_create.
 *
 * @param preleasefunc  called when a packet's samples are no longer needed
 * @param pframesfunc  called as frames finish playing
 * @param pclientdata  passed back to the callbacks
 * @param pmem  the memory for the platform and the instance
 * @param memsize  its size in bytes
 * @return the handle, -9 when no slot is free, -6 when the platform part fails
 */
int SNDPKTPLAY_create(void (*preleasefunc)(int, int), void (*pframesfunc)(int, int, int), int pclientdata, char *pmem, unsigned int memsize) {
    struct s7B8Queue *ppchan;
    int packetinstancehandle;
    char *pplatformmem;
    int i;

    SNDSYS_entercritical();
    for (i = 0; i < D_0050A92E; i++) {
        if (sndpps.f304[i] == 0) {
            packetinstancehandle = i;
            goto found;
        }
    }
    SNDSYS_leavecritical();
    return -9;
found:
    if (SNDPLATFORM_packetplaycreate_6620(packetinstancehandle, pmem) < 0) {
        SNDSYS_leavecritical();
        return -6;
    }
    pplatformmem = pmem;
    pmem += SNDPLATFORM_packetoverhead();
    memsize -= SNDPLATFORM_packetoverhead();
    ppchan = (struct s7B8Queue *)pmem;
    ppchan->f40 = (memsize - 156) >> 5;
    ppchan->f50 = (int)pplatformmem;
    ppchan->f54 = preleasefunc;
    ppchan->f58 = pframesfunc;
    ppchan->f5C = pclientdata;
    ppchan->f00 = -1;
    sndpps.f304[packetinstancehandle] = (struct s7B8Queue *)pmem;
    SNDSYS_leavecritical();
    return packetinstancehandle;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_start);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct SNDSAMPLEATTR SNDSAMPLEATTR_6760;
typedef struct SNDPLAYOPTS SNDPLAYOPTS_6760;
/* The packet channel is the unit's struct s7B8Queue (sndpps.f304[], same volatile fields). */
typedef struct SNDIPATCHHEADER SNDIPATCHHEADER_6760;
extern int f_6158_6760(int *, SNDIPATCHHEADER_6760 *) __asm__("SNDI_validrendermode");
extern void f_0C18_6760(int, int *, int *) __asm__("SNDPLATFORM_getvoicerange");
extern int f_A0B0_6760(int, int, int *, int, int) __asm__("SNDVOICEI_alloc");
extern unsigned char f_6580_6760(int, int) __asm__("SNDPKTPLAYI_gethighchannel");
extern void f_A938_6760(int) __asm__("iSNDcalcpitch");
extern void f_A7B0_6760(int) __asm__("func_003BA7B0");
extern void f_A740_6760(int, int) __asm__("func_003BA740");
extern int f_E1B0_6760(int, int, int, int, int, int, SNDSAMPLEFORMAT *, SNDSAMPLEATTR_6760 *) __asm__("SNDPLATFORM_packetplay");
extern void f_A550_6760(int) __asm__("SNDVOICEI_free");

int SNDPKTPLAY_start_impl(int packetinstancehandle, SNDSAMPLEFORMAT *pssf, SNDSAMPLEATTR_6760 *pssa, SNDPLAYOPTS_6760 *pspo) __asm__("SNDPKTPLAY_start");
/**
 * Starts a packet player instance playing: allocates voices and sets them up for the sample format and options.
 *
 * Resets the instance's queue from the format (one reader per channel; the attribute memory blocks become the
 * per-channel buffers freed at stop). For each render mode SNDI_validrendermode offers, it asks for the mode's voice range
 * (SNDPLATFORM_getvoicerange) and SNDVOICEI_alloc (priority 101); on success it records the voice handle and the highest channel
 * (SNDPKTPLAYI_gethighchannel), fills the channel record (levels, pitch, rate, channel count, render mode), updates pitch and volume
 * (iSNDcalcpitch, func_003BA7B0, func_003BA740) and starts the platform player (SNDPLATFORM_packetplay). If that fails the
 * voices are freed and the next mode is tried. Called through SNDSTRMI_startstream.
 *
 * @param packetinstancehandle  the instance
 * @param pssf  the sample format (rate, channel count)
 * @param pssa  the sample attributes (levels, per-channel memory)
 * @param pspo  the play options
 * @return the sound handle, -9 when no voice could be had, or the platform's error
 */
int SNDPKTPLAY_start_impl(int packetinstancehandle, SNDSAMPLEFORMAT *pssf, SNDSAMPLEATTR_6760 *pssa, SNDPLAYOPTS_6760 *pspo) {
    SNDIPATCHHEADER_6760 ph;
    struct s7B8Queue *ppchan;
    int minvoicerange;
    int maxvoicerange;
    int shandle;
    int rendermodeindex = 0;
    int rendermode;
    int voice;
    int ret;
    int voicesneeded = 1;
    int i;
    int done = 0;
    struct CHANPUB *pVoice;

    voicesneeded = pssf->channels;
    ppchan = sndpps.f304[packetinstancehandle];
    rendermodeindex = 0;
    ph.numchan = pssf->channels;
    ph.rendermode = pssa->rendermode;
    ppchan->f60 = *pssf;
    ppchan->f04 = 0;
    ppchan->f48 = 0;
    ppchan->f4C = 0;
    ppchan->f0C = 0;
    ppchan->f08 = 0;
    ppchan->f46 = 0;
    ppchan->f44 = 0;
    for (i = 0; i < ppchan->f60.channels; i++) {
        ppchan->f10[i] = 0;
        ppchan->f34[i] = 0;
        ppchan->f28[i] = 0;
        ppchan->f64[i] = ((int*)&pssa->ptsdata[0])[i];
    }
    while (!done) {
        rendermode = f_6158_6760(&rendermodeindex, &ph);
        if (rendermode == 0) {
            break;
        }
        f_0C18_6760(rendermode, &minvoicerange, &maxvoicerange);
        voice = f_A0B0_6760(voicesneeded, 101, &shandle, minvoicerange, maxvoicerange);
        if (voice < 0) {
            ppchan->f00 = -9;
        } else {
            ppchan->f00 = shandle;
            ppchan->f42 = f_6580_6760(voice, pssf->channels);
            pVoice = &sndgs.chan[voice];
            pVoice->builtinazimuth = 0;
            pVoice->azimuth = pspo->azimuth;
            pVoice->bhandle = -1;
            pVoice->initialdetune = (*(unsigned short*)&pssa->detune);
            pVoice->progpitch = pspo->pitchmult;
            pVoice->fadePer100Hz = 0;
            pVoice->programmedVol = pspo->vol << 16;
            pVoice->envduration = 0x7FFFFFFF;
            pVoice->envvol = 0x7F0000;
            pVoice->envpertick = 0;
            pVoice->builtinvol = (*(unsigned char*)&pssa->vol);
            pVoice->pitchbend = (*(unsigned char*)&pspo->bend);
            pVoice->numenvelopes = 1;
            pVoice->curenvelope = 0;
            pVoice->releaseenvelope = 0;
            pVoice->progfxlevel[0] = (*(unsigned char*)&pssa->fxlevel0);
            pVoice->drylevel = (*(unsigned char*)&pspo->drylevel);
            pVoice->unk_61[0] = (*(unsigned char*)&pspo->fxlevel0);
            pVoice->bendrange = pssa->bendrange * 100;
            pVoice->patchkey = 0;
            pVoice->ismaster = 0;
            (*(struct ENVELOPE**)&pVoice->paenvelope) = 0;
            (*(int*)&pVoice->pvoltable) = 0;
            pVoice->pbendtable = 0;
            pVoice->pvollfo = 0;
            pVoice->ppitchlfo = 0;
            for (i = 0; i < pssf->channels; i++) {
                pVoice->azimuthOffsets[i] = pssa->azimuth[i];
            }
            pVoice->frames = 0;
            pVoice->sustainend = -1;
            pVoice->samplerate = pssf->samplerate;
            pVoice->samplerep = pssf->samplerep;
            pVoice->numchan = pssf->channels;
            pVoice->rendermode = rendermode;
            pVoice->detunepitch = 0;
            f_A938_6760(voice);
            f_A7B0_6760(voice);
            f_A740_6760(0, voice);
            ret = f_E1B0_6760(packetinstancehandle, voice, pspo->timemult, pspo->pad2, pspo->lowpasscutoff, pspo->highpasscutoff, pssf, pssa);
            if (ret >= 0) {
                break;
            }
            for (i = 0; i < voicesneeded; i++) {
                f_A550_6760(sndgs.chan[voice].voices[i]);
            }
            ppchan->f00 = ret;
        }
    }
    return ppchan->f00;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_submit);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

/* 97.69: only missing "move $5,$2" copy of lbu result before loop (n vs r pseudo); volatile f48/f46/f4/f40/a[] needed */
struct A_6AB8 { char pad0[4]; unsigned int sz : 31; unsigned int flag : 1; char pad8[4]; int w[6]; };

/**
 * Queues a packet on a packet player instance and returns its sequence number.
 *
 * Fails when any channel already has capacity - 1 packets pending. Otherwise it writes the next ring record: the
 * sequence number, the frame count (31 bits) and its last-packet flag, and one sample pointer per channel; each
 * channel's pending count, the queued frames (+0x48) and the write index (wrapping) advance. Called by
 * SNDSTRMI_parsedata.
 *
 * @param instanceHandle  the instance
 * @param packet  the packet: +0x04 frames and flag (top bit), +0x0C one sample pointer per channel
 * @return the packet's sequence number, or -13 when the ring is full
 */
int SNDPKTPLAY_submit(int instanceHandle, struct A_6AB8 *packet)
{
  struct s7B8Queue *instance = sndpps.f304[instanceHandle];
  struct s7B8Rec *record;
  int i;
  int packetHandle;
  for (i = 0; i < instance->f60.channels; i++)
  {
    if (instance->f34[i] >= instance->f40 - 1)
    {
      packetHandle = -13;
      goto done;
    }
  }

  record = &instance->f7C[instance->f46];
  record->len = packet->sz;
  record->flag = packet->flag;
  record->f00 = instance->f04;
  for (i = 0; i < instance->f60.channels; i++)
  {
    record->f08[i] = packet->w[i];
    instance->f34[i] += 1;
  }

  instance->f48 += packet->sz;
  packetHandle = instance->f04;
  instance->f04 = instance->f04 + 1;
  instance->f46 = instance->f46 + 1;
  if (instance->f46 >= instance->f40)
  {
    instance->f46 = 0;
  }
  done:
  return packetHandle;

}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_submitspace);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
extern struct s7B8 sndpps;

/**
 * Returns how many more packets a packet player instance can take: capacity - 1 - the most any channel has pending.
 *
 * Called by SNDSTRMI_service.
 *
 * @param instanceHandle  the instance
 * @return the free packet count
 */
int SNDPKTPLAY_submitspace(int instanceHandle) {
    struct s7B8Queue *instance = sndpps.f304[instanceHandle];
    int mostPending = 0;
    int i;
    for (i = 0; i < instance->f60.channels; i++) {
        if (!(instance->f34[i] < mostPending))
            mostPending = instance->f34[i];
    }
    return instance->f40 - mostPending - 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_framesoutstanding);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Inner_6C98 { char pad[0x48]; volatile int a; volatile int b; };
struct S_6C98 { char pad[0x304]; struct Inner_6C98 *p[1]; };
extern struct S_6C98 sndpps_6C98 __asm__("sndpps");
/**
 * Returns the frames a packet player instance still has to play: those queued (+0x48) plus those on the voices (+0x4C).
 *
 * Called by SNDSTRMI_service, SNDSTRM_status and SFILTER_timestretch.
 *
 * @param i  the instance
 * @return the frame count
 */
int SNDPKTPLAY_framesoutstanding(int i) {
    struct Inner_6C98 *instance = (&sndpps_6C98)[0].p[i];
    return instance->a + instance->b;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_hardwareframesoutstanding);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
extern struct s7B8 sndpps;

/**
 * Returns the frames a packet player instance has handed to its voices but not yet played (+0x4C).
 *
 * No callers.
 *
 * @param i  the instance
 * @return the frame count
 */
int SNDPKTPLAY_hardwareframesoutstanding(int i) {
    return sndpps.f304[i]->f4C;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_purge);
#ifdef SKIP_ASM
/**
 * Unsupported packet purge: always returns -15. SNDPKTPLAY_stop calls it with the full range.
 *
 * @return -15
 */
int SNDPKTPLAY_purge(void) {
    return -0xF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_stop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
extern struct s7B8 sndpps;

extern void SNDMEMI_free();

extern void f_89E0_6CE8(int) __asm__("SNDstop");
extern void SNDPKTPLAYI_flushcallbackdata(void);
extern int f_6CE0_6CE8(int, int, int) __asm__("SNDPKTPLAY_purge");

/**
 * Stops a packet player instance.
 *
 * Stops its sound (SNDstop), runs the pending callbacks (SNDPKTPLAYI_flushcallbackdata), calls SNDPKTPLAY_purge over the whole range,
 * forgets the voice (-1) and frees each channel's buffer (SNDMEMI_free). Called by SNDSTRMI_service and SNDSTRM_purge.
 *
 * @param instanceHandle  the instance
 * @return 0
 */
int SNDPKTPLAY_stop(int instanceHandle) {
    struct s7B8Queue *instance = sndpps.f304[instanceHandle];
    int i = 0;

    f_89E0_6CE8(instance->f00);
    SNDPKTPLAYI_flushcallbackdata();
    f_6CE0_6CE8(instanceHandle, 0, 0x7FFFFFFF);
    instance->f00 = -1;

    for (; i < instance->f60.channels; i++) {
        if (instance->f64[i]) SNDMEMI_free(instance->f64[i]);

    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAY_destroy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SNDPLATFORM_packetplaydestroy_6DA0(void) __asm__("SNDPLATFORM_packetplaydestroy");
typedef struct { char pad[0x304]; int arr[100]; } W_6DA0;
extern W_6DA0 sndpps_6DA0 __asm__("sndpps");

/**
 * Destroys a packet player instance: releases the platform part (SNDPLATFORM_packetplaydestroy) and frees its slot in sndpps.
 *
 * Called by SNDSTRMI_create (on failure) and SNDSTRM_destroy.
 *
 * @param instanceHandle  the instance
 * @return 0
 */
int SNDPKTPLAY_destroy(int instanceHandle) {
    SNDPLATFORM_packetplaydestroy_6DA0();
    sndpps_6DA0.arr[instanceHandle] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_get);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_6DE0 { int pad; int len:31; int flag:1; unsigned int val[6]; };
struct Q_6DE0 {
    int pad0[2];
    volatile unsigned int wr;
    volatile unsigned int rd;
    volatile unsigned int pos[6];
    volatile short ridx[6];
    volatile short cnt[6];
    volatile short size;
    short pad42;
    volatile short widx;
    short pad46;
    volatile int a;
    volatile int b;
    int pad50;
    int flag;
    char pad58[10];
    unsigned char n;
    char pad63[0x19];
    struct E_6DE0 e[1];
};
struct Ev_6DE0 { short type; short id; int val; };
struct G_6DE0 { int count; struct Ev_6DE0 ev[0x60]; struct Q_6DE0 *q[1]; };
extern struct G_6DE0 sndpps_6DE0 __asm__("sndpps");

/**
 * Hands a channel the next packet of a packet player instance: returns its sample pointer, frame count and flag.
 *
 * First releases the oldest packet once every channel has read past it: when a release callback is set (+0x54), an
 * event (type 1, with the packet's first sample pointer) is queued in sndpps and the release index advances. Then,
 * if the channel has a packet pending, it is taken from the channel's read index; once all channels have read it, its
 * frames move from queued (+0x48) to playing (+0x4C). Called by the platform's packet players (SFILTER_unpackmtpf,
 * SFILTER_unpackpf, SFILTER_unpackxapf).
 *
 * @param id  the instance
 * @param channel  the channel
 * @param len  receives the packet's frame count
 * @param flag  receives the packet's flag bit
 * @return the samples (0xFFFFFFFF for a null pointer), or 0 when the channel has nothing pending
 */
unsigned int SNDPKTPLAYI_get(int id, int channel, int *len, int *flag) {
    struct Q_6DE0 *instance;
    struct E_6DE0 *packet;
    struct Ev_6DE0 *ev;
    int ok, ok2, i;
    unsigned int samples;

    ok = 1;
    instance = sndpps_6DE0.q[id];
    ok2 = 1;
    for (i = 0; i < instance->n; i++) {
        if (instance->pos[i] <= instance->wr) {
            ok = 0;
        }
    }
    if (ok) {
        struct E_6DE0 *w = &instance->e[instance->widx];
        if (instance->flag) {
            ev = &sndpps_6DE0.ev[sndpps_6DE0.count];
            ev->type = 1;
            ev->id = id;
            ev->val = w->val[0];
            sndpps_6DE0.count++;
        }
        instance->widx++;
        instance->wr++;
        if (instance->widx >= instance->size) {
            instance->widx = 0;
        }
    }
    if (instance->cnt[channel] == 0) {
        return 0;
    }
    packet = &instance->e[instance->ridx[channel]];
    *len = packet->len;
    *flag = packet->flag;
    instance->ridx[channel]++;
    instance->pos[channel]++;
    i = 0;
    do {
        if (instance->pos[i] <= instance->rd) {
            ok2 = 0;
        }
        i++;
    } while (ok2 == 1 && i < instance->n);
    if (ok2) {
        instance->rd++;
        instance->a -= packet->len;
        instance->b += packet->len;
    }
    if (instance->ridx[channel] >= instance->size) {
        instance->ridx[channel] = 0;
    }
    instance->cnt[channel]--;
    samples = packet->val[channel];
    return samples ? samples : 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_freeframes);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
extern struct s7B8 sndpps;

/**
 * Reports frames a packet player has finished playing.
 *
 * Only the instance's highest channel (+0x42, from SNDPKTPLAYI_gethighchannel) counts: the frames come off the playing count (+0x4C)
 * and, when a frames callback is set (+0x58), an event (type 0, with the count) is queued in sndpps. Called by
 * SFILTER_unpackmtpf and SFILTER_unpackpf.
 *
 * @param id  the instance
 * @param channel  the channel
 * @param frames  the number of frames
 */
void SNDPKTPLAYI_freeframes(int id, int channel, int frames)
{
    struct s7B8Queue* instance = sndpps.f304[id];
    if (channel == (signed char)instance->f42) {

        instance->f4C -= frames;
        if (instance->f58) {
            struct s7B8Event* event = &sndpps.f004[sndpps.f000];
            event->f04 = frames;
            event->f02 = id;
            event->f00 = 0;
            sndpps.f000++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_flushcallbackdata);
#ifdef SKIP_ASM
struct G_7080 { int count; struct SNDIPACKETCALLBACK ev[0x60]; struct SNDPACKETCHAN *q[1]; };
extern struct G_7080 sndpps_7080 __asm__("sndpps");

/**
 * Runs the packet player callbacks queued since the last call, then empties the queue.
 *
 * Each 8-byte event of sndpps names its instance: type 0 calls the frames callback (+0x58) with the instance, the
 * frame count and the client data; other types call the release callback (+0x54) with the samples and the client data.
 * Called by SNDDRV_thread (the sound thread) and SNDPKTPLAY_stop.
 */
void SNDPKTPLAYI_flushcallbackdata(void) {
    int i;
    for (i = 0; i < sndpps_7080.count; i++) {
        struct SNDIPACKETCALLBACK *ev = &sndpps_7080.ev[i];
        struct SNDPACKETCHAN *instance = sndpps_7080.q[ev->chan];
        if (ev->type == 0) {
            instance->pframesfunc(ev->chan, ev->u, instance->pclientdata);
        } else {
            instance->preleasefunc(ev->u, instance->pclientdata);
        }
    }
    sndpps_7080.count = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDrelease);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int fA6E8_7138(void) __asm__("SNDVOICEI_get");
extern int fAA00_7138(int, int *) __asm__("iSNDpatchkey");
extern void f2440_7138(int) __asm__("SNDPLATFORM_stop");

/**
 * Releases a sound: moves each of its voices into the release part of its volume envelope.
 *
 * Looks the sound up (SNDVOICEI_get, same argument) and walks its voices with iSNDpatchkey. A voice whose release
 * segment (+0x5E) is negative is stopped (SNDPLATFORM_stop); otherwise, if it has not reached that segment yet (+0x5D), it
 * jumps there: the segment's duration goes to +0x44 and the slope from the current level (+0x40) to the segment's
 * target to +0x3C. Called by SNDI_midiparsenoteoff.
 *
 * @return the voice found, or a negative value when the sound is gone
 */
int SNDrelease(void) {
    int setVoice;
    int voice = fA6E8_7138();
    if (voice >= 0) {
        char *sndGlobals;
        setVoice = -1;
        sndGlobals = ((char*)&sndgs);
        while (fAA00_7138(voice, &setVoice) != 0) {
            char *chanPub = *(char **)(sndGlobals + 0x1F0) + setVoice * 140;
            signed char nextSegment = *(signed char *)(chanPub + 0x5E);
            if (nextSegment < 0) {
                f2440_7138(setVoice);
            } else if (*(signed char *)(chanPub + 0x5D) < nextSegment) {
                char *segment;
                int duration;
                *(signed char *)(chanPub + 0x5D) = (unsigned char)*(signed char *)(chanPub + 0x5E);
                segment = (char *)(nextSegment * 8 + *(int *)(chanPub + 0x6C));
                duration = *(int *)segment;
                *(int *)(chanPub + 0x44) = duration;
                *(int *)(chanPub + 0x3C) = ((*(int *)(segment + 4) << 16) - *(int *)(chanPub + 0x40)) / duration;
            }
        }
    }
    return voice;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SND_attrsetdef);
#ifdef SKIP_ASM

/**
 * Fills sample attributes with their defaults.
 *
 * Volume +0x03 = 0x7F, pan +0x04 = 0x40, +0x08 = 8, the other bytes and +0x00 zero; clears the four words at +0x48 and
 * +0x58 and the six 16-bit levels (+0x0C) and words (+0x18). Called by SNDI_patchtohdr and SNDI_patchtohdrgen.
 *
 * @param sampleAttr  the attributes
 * @return 0
 */
int SND_attrsetdef(struct SNDSAMPLEATTR *sampleAttr) {
    int i;
    sampleAttr->vol = 0x7F;
    sampleAttr->pan = 0x40;
    sampleAttr->rendermode = 8;
    sampleAttr->priority = 0;
    sampleAttr->detune = 0;
    sampleAttr->fxlevel0 = 0;
    sampleAttr->bendrange = 0;
    sampleAttr->platformver = 0;
    for (i = 0; i < 4; i++) {
        ((int*)&sampleAttr->puserdata[0])[i] = 0;
        sampleAttr->userdatasize[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        ((short*)&sampleAttr->azimuth[0])[i] = 0;
        ((int*)&sampleAttr->ptsdata[0])[i] = 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", randrange);
#ifdef SKIP_ASM
/**
 * Returns a random value between -range and +range.
 *
 * The range is clamped to 0..0x10000; the result is ((iSNDrandom() & 0x7FFF) - 0x4000) * range >> 14. Called by
 * SNDBANKI_playtimbre and SNDBANKI_playpatch.
 *
 * @param range  the largest magnitude
 * @return the random offset
 */
int randrange(int range) {
    if (range > 0x10000) {
        range = 0x10000;
    } else {
        range = (range <= -1) ? 0 : range;
    }
    return (int)(((iSNDrandom() & 0x7FFF) - 0x4000) * range) >> 0xE;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_getb);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Reads an n-byte big-endian value from p and sign-extends it for 1, 2 or 3 bytes.
int SNDI_getb_72F8(unsigned char *p, int n) __asm__("SNDI_getb");
int SNDI_getb_impl(unsigned char *p, int n) __asm__("SNDI_getb");
/**
 * Reads a big-endian integer of 1 to 4 bytes; 1-, 2- and 3-byte values are sign-extended.
 *
 * Used by the tag parser SNDI_gettag and the effect and decoder set-up code (func_003C3450, func_003CBC58,
 * func_003CC070, func_003CC630).
 *
 * @param bytes  the first byte
 * @param n  the number of bytes
 * @return the value
 */
int SNDI_getb_impl(unsigned char *bytes, int n) {
    int value = 0;
    int i = n;
    while (i--)
        value = (value << 8) + *bytes++;
    if (n == 1 && value >= 0x80)
        value -= 0x100;
    else if (n == 2 && value > 0x7FFF)
        value -= 0x10000;
    else if (n == 3 && value > 0x7FFFFF)
        value -= 0x1000000;
    return value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_gettag);
#ifdef SKIP_ASM
extern unsigned SNDI_getb_7388(unsigned char *, unsigned) __asm__("SNDI_getb");

/**
 * Reads the next tag of a patch's tag stream and advances past it.
 *
 * Skips 0xFC padding bytes and stores the tag byte (+0x04). 0xFF ends the stream; 0xFD and 0xFE are tags without data.
 * Other tags have a length byte (0xFF: a 4-byte length follows), then the data: +0x0C points to it, +0x10 holds the
 * length and, for data under 5 bytes, +0x08 its value (SNDI_getb). Called by SNDI_parsetimbre, SNDI_patchtohdr and
 * SNDI_patchtohdrgen.
 *
 * @param tagInfo  the tag reader: +0x00 the stream position, then the fields above
 * @return 1 when a tag was read, 0 at the end of the stream
 */
int SNDI_gettag(struct SNDTAGINFO *tagInfo) {
    unsigned char *cursor = tagInfo->ptag;
    unsigned char *prevByte, *tagStart;
    unsigned cur, len;
    if (*cursor == 0xFC) {
        do {
            prevByte = cursor;
            tagInfo->ptag = prevByte + 1;
            cursor = prevByte + 1;
        } while (prevByte[1] == 0xFC);
    }
    tagStart = tagInfo->ptag;
    cur = *tagStart;
    tagInfo->tagtype = cur;
    if (cur == 0xFF) {
        return 0;
    }
    tagInfo->ptag = tagStart + 1;
    if (cur == 253) {
        return 1;
    }
    if (cur == 0xFE) {
        return 1;
    }
    len = tagStart[1];
    tagInfo->tagsize = len;
    if (len == 0xFF) {
        tagInfo->tagsize = SNDI_getb_7388(tagStart + 2, 4);
        tagInfo->ptag = tagInfo->ptag + 4;
    }
    tagInfo->ptag = tagInfo->ptag + 1;
    (*(unsigned char**)&tagInfo->tagptr) = tagInfo->ptag;
    if (tagInfo->tagsize < 5) {
        tagInfo->tagval = SNDI_getb_7388(tagInfo->ptag, tagInfo->tagsize);
    }
    tagInfo->ptag = tagInfo->ptag + tagInfo->tagsize;
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C7480);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S24_7480 { int w[6]; };
struct S16_7480 { int w[4]; };
struct T_7480 { struct S24_7480 a, b, c, d, e; struct S16_7480 f; int pad[2]; struct S16_7480 g; };
extern struct T_7480 D_0050AB20_7480 __asm__("D_0050AB20");
extern struct S24_7480 D_0044E1C8_7480 __asm__("D_0044E1C8");
extern struct S24_7480 D_0044E1E0_7480 __asm__("D_0044E1E0");
extern struct S24_7480 D_0044E1F8_7480 __asm__("D_0044E1F8");
extern struct S24_7480 D_0044E210_7480 __asm__("D_0044E210");
extern struct S24_7480 D_0044E228_7480 __asm__("D_0044E228");
extern struct S16_7480 D_0044E240_7480 __asm__("D_0044E240");
extern struct S16_7480 D_0044E250_7480 __asm__("D_0044E250");

/**
 * Restores the default speaker layouts.
 *
 * Copies the seven tables D_0044E1C8 .. D_0044E250 (five of six speakers, two of four; each word a 16-bit angle and a
 * 16-bit gain of 0x7FFF) into D_0050AB20 (+0x238 of sndgs), the layouts func_003C38E0 pans with. Called by
 * SNDPLATFORM_init at start-up.
 */
void func_003C7480(void) {
    D_0050AB20_7480.a = D_0044E1C8_7480;
    D_0050AB20_7480.b = D_0044E1E0_7480;
    D_0050AB20_7480.c = D_0044E1F8_7480;
    D_0050AB20_7480.d = D_0044E210_7480;
    D_0050AB20_7480.e = D_0044E228_7480;
    D_0050AB20_7480.f = D_0044E240_7480;
    D_0050AB20_7480.g = D_0044E250_7480;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_interpolateto0);
#ifdef SKIP_ASM
/**
 * Fades a leftover level out over the first 16 samples of a buffer, then clears it.
 *
 * Adds level * (16 - i) / 17 to sample i. The level is what voices stopped since the last slice left on that output
 * (MIX_stop), so they end without a click. Called by MIX_audioslice for each output and the effect bus.
 *
 * @param amount  in/out: the level, cleared afterwards
 * @param buckets  the 16 float samples
 */
void MIXI_interpolateto0(float *amount, void *buckets) {
    (*(float *)((char*)(buckets) + (0))) = (float) ((*(float *)((char*)(buckets) + (0))) + (*amount * 0.9411765f));
    (*(float *)((char*)(buckets) + (4))) = (float) ((*(float *)((char*)(buckets) + (4))) + (*amount * 0.88235295f));
    (*(float *)((char*)(buckets) + (8))) = (float) ((*(float *)((char*)(buckets) + (8))) + (*amount * 0.8235294f));
    (*(float *)((char*)(buckets) + (0xC))) = (float) ((*(float *)((char*)(buckets) + (0xC))) + (*amount * 0.7647059f));
    (*(float *)((char*)(buckets) + (0x10))) = (float) ((*(float *)((char*)(buckets) + (0x10))) + (*amount * 0.7058824f));
    (*(float *)((char*)(buckets) + (0x14))) = (float) ((*(float *)((char*)(buckets) + (0x14))) + (*amount * 0.64705884f));
    (*(float *)((char*)(buckets) + (0x18))) = (float) ((*(float *)((char*)(buckets) + (0x18))) + (*amount * 0.5882353f));
    (*(float *)((char*)(buckets) + (0x1C))) = (float) ((*(float *)((char*)(buckets) + (0x1C))) + (*amount * 0.5294118f));
    (*(float *)((char*)(buckets) + (0x20))) = (float) ((*(float *)((char*)(buckets) + (0x20))) + (*amount * 0.47058824f));
    (*(float *)((char*)(buckets) + (0x24))) = (float) ((*(float *)((char*)(buckets) + (0x24))) + (*amount * 0.4117647f));
    (*(float *)((char*)(buckets) + (0x28))) = (float) ((*(float *)((char*)(buckets) + (0x28))) + (*amount * 0.3529412f));
    (*(float *)((char*)(buckets) + (0x2C))) = (float) ((*(float *)((char*)(buckets) + (0x2C))) + (*amount * 0.29411766f));
    (*(float *)((char*)(buckets) + (0x30))) = (float) ((*(float *)((char*)(buckets) + (0x30))) + (*amount * 0.23529412f));
    (*(float *)((char*)(buckets) + (0x34))) = (float) ((*(float *)((char*)(buckets) + (0x34))) + (*amount * 0.1764706f));
    (*(float *)((char*)(buckets) + (0x38))) = (float) ((*(float *)((char*)(buckets) + (0x38))) + (*amount * 0.11764706f));
    (*(float *)((char*)(buckets) + (0x3C))) = (float) ((*(float *)((char*)(buckets) + (0x3C))) + (*amount * 0.05882353f));
    *amount = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_interpolatemix);
#ifdef SKIP_ASM
/**
 * Mixes 16 samples into a buffer while ramping the gain from one value to another.
 *
 * Sample i is added with the gain fparg0 + (fparg1 - fparg0) * (i + 1) / 17. Called by SNDMIXI_volramp when a voice's
 * gain changes.
 *
 * @param input  the 16 source samples
 * @param accum  the 16 destination samples
 * @param fparg0  the gain to start from
 * @param fparg1  the gain to ramp to
 */
void MIXI_interpolatemix(void *input, void *accum, float fparg0, float fparg1) {
    float temp_f13;

    temp_f13 = (fparg1 - fparg0) * 0.05882353f;
    (*(float *)((char*)(accum) + (0))) = (float) ((*(float *)((char*)(accum) + (0))) + ((*(float *)((char*)(input) + (0))) * (fparg0 + temp_f13)));
    (*(float *)((char*)(accum) + (4))) = (float) ((*(float *)((char*)(accum) + (4))) + ((*(float *)((char*)(input) + (4))) * (fparg0 + (2.0f * temp_f13))));
    (*(float *)((char*)(accum) + (8))) = (float) ((*(float *)((char*)(accum) + (8))) + ((*(float *)((char*)(input) + (8))) * (fparg0 + (temp_f13 * 3.0f))));
    (*(float *)((char*)(accum) + (0xC))) = (float) ((*(float *)((char*)(accum) + (0xC))) + ((*(float *)((char*)(input) + (0xC))) * (fparg0 + (temp_f13 * 4.0f))));
    (*(float *)((char*)(accum) + (0x10))) = (float) ((*(float *)((char*)(accum) + (0x10))) + ((*(float *)((char*)(input) + (0x10))) * (fparg0 + (temp_f13 * 5.0f))));
    (*(float *)((char*)(accum) + (0x14))) = (float) ((*(float *)((char*)(accum) + (0x14))) + ((*(float *)((char*)(input) + (0x14))) * (fparg0 + (temp_f13 * 6.0f))));
    (*(float *)((char*)(accum) + (0x18))) = (float) ((*(float *)((char*)(accum) + (0x18))) + ((*(float *)((char*)(input) + (0x18))) * (fparg0 + (temp_f13 * 7.0f))));
    (*(float *)((char*)(accum) + (0x1C))) = (float) ((*(float *)((char*)(accum) + (0x1C))) + ((*(float *)((char*)(input) + (0x1C))) * (fparg0 + (temp_f13 * 8.0f))));
    (*(float *)((char*)(accum) + (0x20))) = (float) ((*(float *)((char*)(accum) + (0x20))) + ((*(float *)((char*)(input) + (0x20))) * (fparg0 + (temp_f13 * 9.0f))));
    (*(float *)((char*)(accum) + (0x24))) = (float) ((*(float *)((char*)(accum) + (0x24))) + ((*(float *)((char*)(input) + (0x24))) * (fparg0 + (temp_f13 * 10.0f))));
    (*(float *)((char*)(accum) + (0x28))) = (float) ((*(float *)((char*)(accum) + (0x28))) + ((*(float *)((char*)(input) + (0x28))) * (fparg0 + (temp_f13 * 11.0f))));
    (*(float *)((char*)(accum) + (0x2C))) = (float) ((*(float *)((char*)(accum) + (0x2C))) + ((*(float *)((char*)(input) + (0x2C))) * (fparg0 + (temp_f13 * 12.0f))));
    (*(float *)((char*)(accum) + (0x30))) = (float) ((*(float *)((char*)(accum) + (0x30))) + ((*(float *)((char*)(input) + (0x30))) * (fparg0 + (temp_f13 * 13.0f))));
    (*(float *)((char*)(accum) + (0x34))) = (float) ((*(float *)((char*)(accum) + (0x34))) + ((*(float *)((char*)(input) + (0x34))) * (fparg0 + (temp_f13 * 14.0f))));
    (*(float *)((char*)(accum) + (0x38))) = (float) ((*(float *)((char*)(accum) + (0x38))) + ((*(float *)((char*)(input) + (0x38))) * (fparg0 + (temp_f13 * 15.0f))));
    (*(float *)((char*)(accum) + (0x3C))) = (float) ((*(float *)((char*)(accum) + (0x3C))) + ((*(float *)((char*)(input) + (0x3C))) * (fparg0 + (temp_f13 * 16.0f))));
}
#endif

/**
 * Does nothing: an empty stub. MIX_destroy calls it with 2.0f at mixer shutdown.
 */
void SNDMIX_setmasterlowpass(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIXI_volramp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Obj_7AB0 {
    int (*fn)(struct Obj_7AB0 *self, int a, int b, void *c, int d);
};
struct Snd_7AB0 {
    char pad00[0x3C];
    float f3C;
};
struct S_7AB0 {
    char pad00[0x4];
    float cur[6];
    float want[6];
    float a[1];
    float b[1];
    float f3C;
    struct Obj_7AB0 *obj;
};
extern char D_00515D80_7AB0[] __asm__("D_00515D80");
extern void MIXI_interpolatemix_7AB0(struct Snd_7AB0 *snd, void *name, float from, float to) __asm__("MIXI_interpolatemix");

/**
 * Plays the first 16 frames of a mixer voice while ramping its gains to their new values.
 *
 * Pulls 16 frames through the voice's filter chain (+0x40) into the mix buffer. If none come, the gains just snap to
 * their targets and 0 is returned. Otherwise it keeps the last sample (+0x3C) and, for the effect send and each output,
 * ramps from the current gain to the wanted one (MIXI_interpolatemix) or, when unchanged and non-zero, mixes at that gain
 * (the hook at +0x1E0 of sndmix). Called by MIX_audioslice.
 *
 * @param mixVoice  the voice's mixer state (current gains +0x04, wanted +0x1C, effect send +0x34 / +0x38)
 * @return the frames mixed: 16, or 0
 */
int SNDMIXI_volramp(struct S_7AB0 *mixVoice) {
    int i;

    if (mixVoice->obj->fn(mixVoice->obj, 0x10, ((int)sndmix.ptempbuf[0]), ((struct Snd_7AB0*)sndmix.ptempbuf[1]), 0) <= 0) {
        for (i = 0; i < 1; i++) {
            mixVoice->a[i] = mixVoice->b[i];
        }
        for (i = 0; i < sndmix.mc.outputchannels; i++) {
            mixVoice->cur[i] = mixVoice->want[i];
        }
        return 0;
    }
    mixVoice->f3C = ((struct Snd_7AB0*)sndmix.ptempbuf[1])->f3C;
    for (i = 0; i < 1; i++) {
        if (sndmix.unk_0F[i] != 0) {
            if (mixVoice->a[i] != mixVoice->b[i]) {
                MIXI_interpolatemix_7AB0(((struct Snd_7AB0*)sndmix.ptempbuf[1]), D_00515D80_7AB0, mixVoice->a[i], mixVoice->b[i]);
                mixVoice->a[i] = mixVoice->b[i];
            } else if (mixVoice->b[i] != 0.0f) {
                ((void (*)(int, struct Snd_7AB0*, void*, float))sndmix.mix)(0x10, ((struct Snd_7AB0*)sndmix.ptempbuf[1]), D_00515D80_7AB0, mixVoice->b[i]);
            }
        }
    }
    for (i = 0; i < sndmix.mc.outputchannels; i++) {
        if (mixVoice->cur[i] != mixVoice->want[i]) {
            MIXI_interpolatemix_7AB0(((struct Snd_7AB0*)sndmix.ptempbuf[1]), sndmix.pmixbuf[i], mixVoice->cur[i], mixVoice->want[i]);
            mixVoice->cur[i] = mixVoice->want[i];
        } else if (mixVoice->want[i] != 0.0f) {
            ((void (*)(int, struct Snd_7AB0*, void*, float))sndmix.mix)(0x10, ((struct Snd_7AB0*)sndmix.ptempbuf[1]), sndmix.pmixbuf[i], mixVoice->want[i]);
        }
    }
    return 0x10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_New);
#ifdef SKIP_ASM
extern void SNDMEMI_alloc();

/**
 * Allocates memory for the decoder library from the sound heap: passes its arguments to SNDMEMI_alloc.
 *
 * MIX_create installs it as the allocator through CODASetNew.
 */
void SNDI_New(void) {
    SNDMEMI_alloc();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_Delete);
#ifdef SKIP_ASM
extern void SNDMEMI_free();

/**
 * Frees decoder library memory: passes its argument to SNDMEMI_free.
 *
 * MIX_create installs it as the deallocator through CODASetDelete.
 */
void SNDI_Delete(void) {
    SNDMEMI_free();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_create);
#ifdef SKIP_ASM
/* 28-byte mixer record handed to SFILTER_ft16init: word-aligned (int members), which keeps d[i].h at base+28*i+0x14C */
struct D_7D00 { int w[6]; short h; char b; char pad1B; };
struct S_7D00 {
    struct MIXCONFIG cfg;
    char padC[3];
    char clr[1];
    char pad10[0xA8 - 0x10];
    char *a1[2];
    char *a2[2];
    char *b1[6];
    char *b2[13];
    int c[12];
    struct D_7D00 d[4];
    char pad1A4[0x1DC - 0x1A4];
    char *p1DC;
    int pad1E0;
    unsigned int remain;
};
struct G_7D00 { char pad0[0x64]; char *pool; unsigned int size; };
/* sndmix's free-pool counter: addressed as its own symbol, as in landed SNDPLATFORM_init */
extern unsigned int D_00515D24_7D00 __asm__("D_00515D24");
extern void SNDI_New_7D00() __asm__("SNDI_New");
extern void SNDI_Delete_7D00() __asm__("SNDI_Delete");
extern void f_C918_7D00(void (*)()) __asm__("CODASetNew");
extern void f_C928_7D00(void (*)()) __asm__("CODASetDelete");
extern void f_58A0_7D00(void) __asm__("SNDSYS_entercritical");
extern void f_58D8_7D00(void) __asm__("SNDSYS_leavecritical");
extern char *f_5C60_7D00(int) __asm__("SNDMEMI_alloc");
extern void *memset_7D00(void *, int, int) __asm__("memset");
extern void f_9268_7D00(void) __asm__("MIXI_initunpack16");
extern void f_92B0_7D00(void) __asm__("MIXI_initunpackxa");
extern void f_92F8_7D00(void) __asm__("MIXI_initunpackmt");
extern void f_8128_7D00(void) __asm__("MIX_reset");
extern void f_A7A0_7D00(struct D_7D00 *) __asm__("SFILTER_ft16init");
extern void f_8F48_7D00(int *, struct D_7D00 *) __asm__("SFILTER_add");

/* MIX_create (On Tour smixer.c, PSP debug 0x00354D18): allocation statements in On Tour's `a[i] = alloc(); b[i] = a[i];` form. */
extern struct S_7D00 sndmix_7D00 __asm__("sndmix");
void MIX_create(struct MIXCONFIG *cfg) {
    int i;
    char *pool;

    f_C918_7D00(SNDI_New_7D00);
    f_C928_7D00(SNDI_Delete_7D00);
    for (i = 0; i < 1; i++) {
        sndmix_7D00.clr[i] = 0;
    }
    sndmix_7D00.cfg = *cfg;
    f_58A0_7D00();
    D_00515D24_7D00 = (unsigned int)sndgs.sso.set.fastramsize;
    pool = (char*)sndgs.sso.set.fastramaddr;
    for (i = 0; i < 2; i++) {
        if (D_00515D24_7D00 >= 8280) {
            D_00515D24_7D00 -= 8280;
            sndmix_7D00.a1[i] = pool;
            sndmix_7D00.a2[i] = pool;
            pool += 0x2058;
        } else {
            sndmix_7D00.a1[i] = f_5C60_7D00(8280);
            sndmix_7D00.a2[i] = sndmix_7D00.a1[i];
        }
        while ((int)sndmix_7D00.a2[i] & 0x3F) {
            sndmix_7D00.a2[i] += 4;
        }
    }
    for (i = 0; i < sndmix_7D00.cfg.outputchannels; i++) {
        if (D_00515D24_7D00 >= 2112) {
            D_00515D24_7D00 -= 2112;
            sndmix_7D00.b1[i] = pool;
            sndmix_7D00.b2[i] = pool;
            pool += 0x840;
        } else {
            sndmix_7D00.b1[i] = f_5C60_7D00(2112);
            sndmix_7D00.b2[i] = sndmix_7D00.b1[i];
        }
        while ((int)sndmix_7D00.b2[i] & 0x3F) {
            sndmix_7D00.b2[i] += 4;
        }
    }
    if (sndmix_7D00.cfg.inputchannels != 0) {
        sndmix_7D00.p1DC = f_5C60_7D00(sndmix_7D00.cfg.inputchannels * 0x60);
        memset_7D00(sndmix_7D00.p1DC, 0, sndmix_7D00.cfg.inputchannels * 0x60);
    }
    f_58D8_7D00();
    f_9268_7D00();
    f_92B0_7D00();
    f_92F8_7D00();
    f_8128_7D00();
    for (i = 0; i < sndmix_7D00.cfg.outputchannels; i++) {
        sndmix_7D00.c[i] = 0;
        f_A7A0_7D00(&sndmix_7D00.d[i]);
        sndmix_7D00.d[i].h = 0;
        sndmix_7D00.d[i].b = 0;
        f_8F48_7D00(&sndmix_7D00.c[i], &sndmix_7D00.d[i]);
        memset_7D00(sndmix_7D00.b2[i], 0, 0x800);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_destroy);
#ifdef SKIP_ASM
extern void f_58A0_8018(void) __asm__("SNDSYS_entercritical");
extern void f_58D8_8018(void) __asm__("SNDSYS_leavecritical");
extern void f_5E98_8018(int) __asm__("SNDMEMI_free");
extern void f_7AA8_8018(float) __asm__("SNDMIX_setmasterlowpass");
/**
 * Destroys the software mixer: frees its buffers.
 *
 * In a critical section, frees the voice states (+0x1DC), the two mix buffers and each output buffer of sndmix;
 * buffers in the scratchpad (0x70000000) are only forgotten. Then calls SNDMIX_setmasterlowpass with 2.0f. Called by
 * SNDPLATFORM_restore at shutdown.
 */
void MIX_destroy(void) {
    int i;
    int buffer;
    f_58A0_8018();
    if (sndmix.smv != 0) {
        f_5E98_8018((int)sndmix.smv);
        sndmix.smv = 0;
    }
    for (i = 0; i < 2; i++) {
        buffer = (int)sndmix.punalignedtempbuf[i];
        if ((buffer & 0x70000000) == 0x70000000) {
            sndmix.punalignedtempbuf[i] = 0;
        } else if (buffer != 0) {
            f_5E98_8018(buffer);
            sndmix.punalignedtempbuf[i] = 0;
        }
    }
    for (i = 0; i < sndmix.mc.outputchannels; i++) {
        buffer = (int)sndmix.punalignedmixbuf[i];
        if ((buffer & 0x70000000) == 0x70000000) {
            sndmix.punalignedmixbuf[i] = 0;
        } else if (buffer != 0) {
            f_5E98_8018(buffer);
            sndmix.punalignedmixbuf[i] = 0;
        }
    }
    f_7AA8_8018(2.0f);
    f_58D8_8018();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_reset);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad0[0xC]; short s; char pad1[0x1E0 - 0xE]; void (*f)(void); } C8128_S;
extern void func_003CBB78_8128(void) __asm__("func_003CBB78");

extern C8128_S sndmix_8128 __asm__("sndmix");
/**
 * Resets the mixer's output settings: the mix function (+0x1E0 of sndmix) to func_003CBB78 and the per-slice output
 * step (+0x0C) to 0x400 bytes.
 *
 * Called by MIX_create.
 */
void MIX_reset(void) {
    sndmix_8128.f = func_003CBB78_8128;
    sndmix_8128.s = 0x400;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_setresamplequality);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Sets the mixer's resampling quality (byte +0x0E of sndmix). Called by SNDPLATFORM_outputset.
 *
 * @param quality  the quality
 */
void MIX_setresamplequality(signed char quality) {
    sndmix.resamplequality = quality;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_playinit);
#ifdef SKIP_ASM
struct Obj_8158 { int pad0; int x4; char pad8[0x10]; short x18; signed char x1A; };
struct G_8158 {
    char pad[0x18];
    void (*fn[6][3])(struct Obj_8158 *, struct UNPACKINITPARAMS *);
    int size[6][3];
    char pad2[0x1DC - 0xA8];
    struct SNDMIXVOICE *ents;
};
extern unsigned char D_0050AA62_8158 __asm__("D_0050AA62");
extern struct Obj_8158 *f_5C60_8158(int) __asm__("SNDMEMI_alloc");
extern void f_8F48_8158(int *, struct Obj_8158 *) __asm__("SFILTER_add");
extern void f_B280_8158(struct Obj_8158 *, int, int) __asm__("SFILTER_timestretchinit");

extern struct G_8158 sndmix_8158 __asm__("sndmix");
/**
 * Sets up a mixer voice for playback: builds its decoder and, for time-stretched sounds, a stretch filter.
 *
 * Clears the voice's state (0x60-byte entry in the +0x1DC table of sndmix). The sample format picks the decoder
 * kind (8, 10, 4, 22, 16, 23 -> 0..5) and the play type a variant (0 -> 2; 1 -> 1 when f >= 1, else 0; others 0). When
 * that factory exists, its state is allocated (size table), given the init block (a, b, c, d, e, f, g, the voice and
 * byte D_0050AA62) with order 0xF0, and linked into the voice's filter chain (SFILTER_add). Stretch data adds a
 * 6184-byte node (order 200) set up by SFILTER_timestretchinit. Marks the voice set up (state 1). Called by SNDPLATFORM_playtimbre and
 * SNDPLATFORM_packetplay.
 *
 * @param id  the mixer voice
 * @param fmt  the sample format code
 * @param ch  the play type
 * @param sampleData  passed to the decoder
 * @param loopStart  passed to the decoder
 * @param strm  the time-stretch data, or 0
 * @param channelCount  passed to the decoder
 * @param totalFrames  passed to the decoder
 * @param sustainStart  passed to the decoder
 * @param sustainEnd  passed to the decoder; also picks the variant of play type 1
 * @param u10  unused
 * @param u11  unused
 * @param platformVersion  passed to the decoder
 * @param prio  stored in each node (+0x1A)
 */
void MIX_playinit(int id, int fmt, int ch, int sampleData, int loopStart, int strm, int channelCount, int totalFrames,
                   int sustainStart, int sustainEnd, int u10, int u11, int platformVersion, int prio) {
    struct UNPACKINITPARAMS initParams;
    struct SNDMIXVOICE *ent;
    int n, k;

    ent = &sndmix_8158.ents[id];
    (*(int*)&ent->filterhead) = 0;
    (*(struct Obj_8158**)&ent->pups) = 0;
    ent->unk_50 = 0;
    ent->unk_58 = 0;
    (*(int*)&ent->phps) = 0;
    (*(struct Obj_8158**)&ent->ptss) = 0;
    initParams.bits16 = 1;
    (*(int*)&initParams.unpackgetframe) = 0;
    n = -1;
    if (fmt == 8) {
        n = 0;
    } else if (fmt == 10) {
        n = 1;
    } else if (fmt == 4) {
        n = 2;
    } else if (fmt == 22) {
        n = 3;
    } else if (fmt == 16) {
        n = 4;
    } else if (fmt == 23) {
        n = 5;
    }
    if (ch == 1) {
        k = 0;
        if (sustainEnd >= 1) {
            k = 1;
        }
    } else {
        k = 2;
        if (ch != 0) {
            k = 0;
        }
    }
    if (sndmix_8158.fn[n][k] != 0) {
        (*(struct Obj_8158**)&ent->pups) = f_5C60_8158(sndmix_8158.size[n][k]);
        (*(int*)&initParams.psample) = sampleData;
        (*(int*)&initParams.ploopstart) = loopStart;
        initParams.numchan = channelCount;
        initParams.platformver = platformVersion;
        initParams.totalframes = totalFrames;
        initParams.sustainstart = sustainStart;
        initParams.sustainend = sustainEnd;
        initParams.cpu = D_0050AA62_8158;
        initParams.voice = id;
        ((struct Obj_8158*)ent->pups)->x4 = 0;
        ((struct Obj_8158*)ent->pups)->x18 = 0xF0;
        ((struct Obj_8158*)ent->pups)->x1A = prio;
        sndmix_8158.fn[n][k](((struct Obj_8158*)ent->pups), &initParams);
        (*(int*)&ent->unpackgetframe) = ((int)initParams.unpackgetframe);
        f_8F48_8158(&(*(int*)&ent->filterhead), ((struct Obj_8158*)ent->pups));
    }
    if (strm != 0) {
        if (ch != 0) {
            id = -1;
        }
        (*(struct Obj_8158**)&ent->ptss) = f_5C60_8158(6184);
        ((struct Obj_8158*)ent->ptss)->x4 = 0;
        ((struct Obj_8158*)ent->ptss)->x18 = 200;
        ((struct Obj_8158*)ent->ptss)->x1A = prio;
        f_B280_8158(((struct Obj_8158*)ent->ptss), strm, id);
        f_8F48_8158(&(*(int*)&ent->filterhead), ((struct Obj_8158*)ent->ptss));
    }
    ent->status = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_play);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct G_83A0 { char p0[5]; unsigned char cnt; char pad[0x1D6]; char *tbl; };

/**
 * Starts a mixer voice set up by MIX_playinit.
 *
 * Clears the last sample (+0x3C) and the changed flag (+0x01), makes the wanted effect send and channel gains current
 * and sets the state to 2 (playing). Called by SNDPLATFORM_playtimbre and SNDPLATFORM_packetplay.
 *
 * @param voice  the mixer voice
 */
void MIX_play(int voice) {
    int i;
    int j;
    SNDMIXVOICE* voiceState;
    float *sendGain;
    float *channelGain;
    i = 0;
    voiceState = &sndmix.smv[voice];
    *(int*)&voiceState->lastmixed = 0;
    sendGain = (float *)(&voiceState->wetbusgainprev[0]);
    voiceState->gainchanged = 0;
    do {
        i -= 1;
        sendGain[0] = sendGain[1];
        sendGain++;
    } while (i >= 0);
    j = 0;
    if (sndmix.mc.outputchannels != 0) {
        channelGain = (float *)(&voiceState->drybusgainprev[0]);
        do {
            j += 1;
            channelGain[0] = channelGain[6];
            channelGain++;
        } while (j < sndmix.mc.outputchannels);
    }
    voiceState->status = 2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_stop);
#ifdef SKIP_ASM
extern void MIX_stop();
struct G_8430 { char pad[8]; int (*handler)(int); };

/**
 * Stops a mixer voice whose decoder ran out (MIX_stop), then reports it through the hook at +0x08 of sndmix.
 *
 * Called by MIX_audioslice.
 *
 * @param voice  the mixer voice
 */
void MIXI_stop(int voice) {
    MIX_stop();
    ((int (*)(int))sndmix.mc.chanovercallback)(voice);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_stop);
#ifdef SKIP_ASM
extern void SNDMEMI_free_8468(struct SFILTERNODE *) __asm__("SNDMEMI_free");

/**
 * Stops a mixer voice.
 *
 * Adds the voice's last sample (+0x3C) times its effect send and channel gains to the leftover levels of the mixer, so
 * MIXI_interpolateto0 fades them out; then runs each filter node's clean-up (+0x04), frees the node and sets the state to 0.
 * Called by MIXI_stop and SNDPLATFORM_stop.
 *
 * @param idx  the mixer voice
 */
void MIX_stop(int idx) {
    struct SNDMIXVOICE *voiceState = &sndmix.smv[idx];
    int i;
    struct SFILTERNODE *nd;
    struct SFILTERNODE *next;
    for (i = 0; i < 1; i++) {
        sndmix.stoppedvoiceaccwet[i] += ((float*)&voiceState->wetbusgainprev[0])[i] * voiceState->lastmixed;
    }
    for (i = 0; i < sndmix.mc.outputchannels; i++) {
        sndmix.stoppedvoiceaccdry[i] += ((float*)&voiceState->drybusgainprev[0])[i] * voiceState->lastmixed;
    }
    do {
        if (((void (*)(struct Node_8468*))((struct SFILTERNODE*)voiceState->filterhead)->restorefn) != 0) {
            ((void (*)(struct Node_8468*))((struct SFILTERNODE*)voiceState->filterhead)->restorefn)(((struct SFILTERNODE*)voiceState->filterhead));
        }
        nd = ((struct SFILTERNODE*)voiceState->filterhead);
        next = ((struct Node_8468*)nd->pfnnext);
        SNDMEMI_free_8468(nd);
        (*(struct SFILTERNODE**)&voiceState->filterhead) = next;
    } while (next != 0);
    voiceState->status = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIX_setdrygain);
#ifdef SKIP_ASM
struct G_8560 {
    char pad[0x1DC];
    struct SNDMIXVOICE *tab;
};

/**
 * Sets a mixer voice's gain for one output channel (wanted gain +0x1C of its state) and flags the change (+0x01).
 *
 * MIX_audioslice ramps to it. Called by func_003C2A50.
 *
 * @param idx  the mixer voice
 * @param k  the output channel
 * @param gain  the gain
 */
void SNDMIX_setdrygain(int idx, int k, float gain) {
    sndmix.smv[idx].drybusgain[k] = gain;
    sndmix.smv[idx].gainchanged = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDMIX_setwetgain);
#ifdef SKIP_ASM
struct G_8598 {
    char pad[0x1DC];
    struct SNDMIXVOICE *tab;
};

/**
 * Sets a mixer voice's effect-send gain (wanted gain +0x38 of its state) and flags the change (+0x01).
 *
 * Called by SNDPLATFORM_setfxlevel.
 *
 * @param idx  the mixer voice
 * @param k  the effect bus (only 0)
 * @param gain  the gain
 */
void SNDMIX_setwetgain(int idx, int k, float gain) {
    sndmix.smv[idx].wetbusgain[k] = gain;
    sndmix.smv[idx].gainchanged = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_audioslice);
#ifdef SKIP_ASM
struct G_85D0 {
    char pad000[0x4];
    unsigned char cnt;
    unsigned char n;
    char pad006[0xF - 0x6];
    signed char on[1];
    void (*fn10)(int);
    char pad014[0xB0 - 0x14];
    int fB0;
    float *snd;
    char pad0B8[0xD0 - 0xB8];
    float *names[6];
    float f[6];
    float g[1];
    struct SFILTERNODE *objB[6];
    struct SFILTERNODE *objA[6];
    char pad134[0x1DC - 0x134];
    struct SNDMIXVOICE *tab;
    void (*set)(int a, float *snd, float *name, float v);
};
extern float D_00515D80_85D0[] __asm__("D_00515D80");
extern void f6210_85D0(void *, int, int) __asm__("memset");
extern int f7AB0_85D0(struct SNDMIXVOICE *) __asm__("SNDMIXI_volramp");
extern void f8430_85D0(int) __asm__("MIXI_stop");

extern struct G_85D0 sndmix_85D0 __asm__("sndmix");
/**
 * Mixes one slice of the software mixer into the output buffers.
 *
 * Clears each output's mix buffer and fades out leftover levels (MIXI_interpolateto0). For each playing voice (state 2) it
 * first ramps changed gains over 16 frames (SNDMIXI_volramp), then pulls the rest of the frames through its filter chain;
 * a negative result stops the voice (MIXI_stop), otherwise the frames are mixed at the wanted gains into each
 * output and the effect bus (hook +0x1E0). Then the hook at +0x10 runs, and each output's filters (+0x11C / +0x104
 * tables) write the result into the caller's buffer. Called by MIX_audio.
 *
 * @param outs  one destination pointer per output
 * @param n  the number of frames (up to 512)
 */
void MIX_audioslice(int *outs, int n) {
    struct SNDMIXVOICE *voiceState;
    int i;
    int j;
    int k;
    int framesLeft;
    int framesMixed;

    for (i = 0; i < 1; i++) {
        if (sndmix_85D0.on[i] != 0 && sndmix_85D0.g[i] != 0.0f) {
            MIXI_interpolateto0(&sndmix_85D0.g[i], D_00515D80_85D0);
        }
    }
    for (i = 0; i < sndmix_85D0.n; i++) {
        f6210_85D0(sndmix_85D0.names[i], 0, n * 4);
        if (sndmix_85D0.f[i] != 0.0f) {
            MIXI_interpolateto0(&sndmix_85D0.f[i], sndmix_85D0.names[i]);
        }
    }
    for (j = 0; j < sndmix_85D0.cnt; j++) {
        voiceState = &sndmix_85D0.tab[j];
        if ((*(signed char*)&voiceState->status) == 2) {
            if (voiceState->gainchanged != 0) {
                k = f7AB0_85D0(voiceState);
                voiceState->gainchanged = 0;
                framesLeft = n - k;
            } else {
                k = 0;
                framesLeft = n;
            }
            if (framesLeft != 0) {
                framesMixed = ((int (*)(struct Obj_85D0*, int, int, float*, int))((struct SFILTERNODE*)voiceState->filterhead)->filterfn)(((struct SFILTERNODE*)voiceState->filterhead), framesLeft, sndmix_85D0.fB0, sndmix_85D0.snd, 0);
                if (framesMixed < 0) {
                    f8430_85D0(j);
                } else if (framesMixed != 0) {
                    voiceState->lastmixed = sndmix_85D0.snd[framesMixed - 1];
                    for (i = 0; i < sndmix_85D0.n; i++) {
                        if (voiceState->drybusgain[i] != 0.0f) {
                            sndmix_85D0.set(framesMixed, sndmix_85D0.snd, sndmix_85D0.names[i] + k, voiceState->drybusgain[i]);
                        }
                    }
                    for (i = 0; i < 1; i++) {
                        if (sndmix_85D0.on[i] != 0 && voiceState->wetbusgain[i] != 0.0f) {
                            sndmix_85D0.set(framesMixed, sndmix_85D0.snd, D_00515D80_85D0 + k, voiceState->wetbusgain[i]);
                            *(int *)((char *)D_00515D80_85D0 + 0x1800) = 0;
                        }
                    }
                }
            }
        }
    }
    if (sndmix_85D0.fn10 != 0) {
        sndmix_85D0.fn10(n);
    }
    for (i = 0; i < sndmix_85D0.n; i++) {
        if (sndmix_85D0.objA[i] != 0) {
            ((int (*)(struct Obj_85D0*, int, int, float*, int))sndmix_85D0.objA[i]->filterfn)(sndmix_85D0.objA[i], n, (int)sndmix_85D0.names[i], (float *)sndmix_85D0.fB0, 0);
            ((int (*)(struct Obj_85D0*, int, int, float*, int))sndmix_85D0.objB[i]->filterfn)(sndmix_85D0.objB[i], n, sndmix_85D0.fB0, (float *)outs[i], 0);
        } else {
            ((int (*)(struct Obj_85D0*, int, int, float*, int))sndmix_85D0.objB[i]->filterfn)(sndmix_85D0.objB[i], n, (int)sndmix_85D0.names[i], (float *)outs[i], 0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", MIX_audio);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[5]; unsigned char cnt; char pad2[6]; unsigned short add; } H_8968;
extern void MIX_audioslice_8968(int *, int) __asm__("MIX_audioslice");

/**
 * Mixes a block of frames into the output buffers, 512 frames at a time.
 *
 * Copies the output pointers (one per output, +0x05 of sndmix) and calls MIX_audioslice for each slice, advancing
 * every pointer by the slice step (+0x0C). Called by SNDDRV_thread.
 *
 * @param src  one destination pointer per output
 * @param len  the number of frames
 */
void MIX_audio(int *src, int len) {
    int buf[8];
    int i, n;

    for (i = 0; i < sndmix.mc.outputchannels; i++) {
        buf[i] = src[i];
    }
    while (len > 0) {
        MIX_audioslice_8968(buf, len < 513 ? len : 0x200);
        for (i = 0; i < sndmix.mc.outputchannels; i++) {
            buf[i] += sndmix.outputframeincr;
        }
        len -= 0x200;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_getframe);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns a mixer voice's play position in frames.
 *
 * Calls the get-frame function its decoder registered (+0x44 of the voice's state in D_00515D1C) on the decoder node
 * (+0x48). Called by SNDPLATFORM_getcurframe.
 *
 * @param idx  the mixer voice
 * @return the frame, or -1 when the voice has no decoder
 */
int MIX_getframe(int idx) {
    char *voiceState = (char*)&sndmix.smv[idx];
    int (*fn)(int) = *(int (**)(int))(voiceState + 0x44);
    if (fn != 0) {
        return fn(*(int *)(voiceState + 0x48));
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_setpitch);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned char D_0050AA62;        /* sndgs.f17A */

/* sndmix: the software mixer (channel buffers, per-voice gain entries with their filter chains, the filter
   factories). Layout from every use in this unit; field names are their offsets. */
struct sB40Node {                   /* a filter in an entry's chain (allocated, size per kind) */
    int (*f00)(struct sB40Node *, int, int, float *, int);     /* process */
    void (*f04)(struct sB40Node *);                             /* release */
    struct sB40Node *f08;           /* next */
    struct sB40Node *f0C;
    struct sB40Node *f10;
    struct sB40Node *f14;
    unsigned short f18;             /* order in the chain */
    signed char f1A;
    unsigned char f1B;
};

struct sB40Init {                   /* what MIX_playinit hands a filter factory */
    int f00;
    int f04;
    int f08;
    int f0C;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int (*f28)(struct sB40Node *);
};

struct sB40Ent {                    /* 0x60, sndmix_8A88.f1DC[] (one per mixer voice) */
    signed char f00;                /* state */
    signed char f01;                /* gains changed */
    char pad02[0x4 - 0x2];
    float f04[6];                   /* current channel gains */
    float f1C[6];                   /* wanted channel gains */
    float f34[1];                   /* current effect send */
    float f38[1];                   /* wanted effect send */
    float f3C;                      /* last sample */
    struct sB40Node *f40;           /* filter chain */
    int (*f44)(struct sB40Node *);
    struct sB40Node *f48;
    struct sB40Node *f4C;
    struct sB40Node *f50;
    struct sB40Node *f54;
    int f58;
    struct sB40Node *f5C;
};



extern struct sB40Node *SNDMEMI_alloc_8A88(int) __asm__("SNDMEMI_alloc");
extern void SFILTER_rsfinit_8A88(struct sB40Node *, int, int) __asm__("SFILTER_rsfinit");
extern void SFILTER_add_8A88(struct sB40Node **, struct sB40Node *) __asm__("SFILTER_add");
extern void SFILTER_rsfsetpitch_8A88(struct sB40Node *, int) __asm__("SFILTER_rsfsetpitch");

/**
 * Sets a mixer voice's pitch, adding its resampling filter on first use.
 *
 * The pitch is capped at 0x20000 (a ratio of 2.0 in 16.16). The first call allocates a 0x38-byte filter node (order
 * 160) at +0x50 of the voice's state, sets it up with SFILTER_rsfinit (byte D_0050AA62, and whether the resampling
 * quality +0x0E is at least 50) and links it into the chain (SFILTER_add). Then SFILTER_rsfsetpitch stores the pitch in it.
 * Called by SNDPLATFORM_setpitch.
 *
 * @param idx  the mixer voice
 * @param n  the pitch (16.16)
 */
void MIX_setpitch(int idx, int n) {
    struct sB40Ent *voiceState;
    if (n > 0x20000) n = 0x20000;
    voiceState = (struct sB40Ent*)&(sndmix.smv)[idx];
    if (voiceState->f50 == 0) {
        int big;
        voiceState->f50 = SNDMEMI_alloc_8A88(0x38);
        big = sndmix.resamplequality >= 50;
        voiceState->f50->f04 = 0;
        voiceState->f50->f18 = 160;
        voiceState->f50->f1A = 0;
        SFILTER_rsfinit_8A88(voiceState->f50, D_0050AA62, big);
        SFILTER_add_8A88(&voiceState->f40, voiceState->f50);
    }
    SFILTER_rsfsetpitch_8A88(voiceState->f50, n);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_settimemult);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x4C]; int f; char pad2[0x10]; } E_8B40;
typedef struct { E_8B40 arr[100]; } W_8B40;
void SFILTER_timestretchsetratio_8B40(int, int) __asm__("SFILTER_timestretchsetratio");

/**
 * Sets a mixer voice's time multiplier: passes it to the voice's time-stretch node (+0x4C), if it has one, through
 * SFILTER_timestretchsetratio.
 *
 * Called by SNDPLATFORM_timemult.
 *
 * @param voice  the mixer voice
 * @param timeMult  the time multiplier
 */
void MIX_settimemult(int voice, int timeMult) {
    int temp_4;

    temp_4 = ((W_8B40*)sndmix.smv)->arr[voice].f;
    if (temp_4 != 0) {
        SFILTER_timestretchsetratio_8B40(temp_4, timeMult);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C8B80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *f_5C60_8B80(int) __asm__("SNDMEMI_alloc");
extern void f_5E98_8B80(void *) __asm__("SNDMEMI_free");
extern void f_8F48_8B80(void *, void *) __asm__("SFILTER_add");
extern void f_8FB0_8B80(void *, void *) __asm__("SFILTER_remove");
extern void f_B4A8_8B80(void *, int) __asm__("func_003CB4A8");
extern void f_B528_8B80(void *) __asm__("func_003CB528");
struct E_8B80 { char pad[0x40]; char sub[0x14]; struct SFILTERNODE *q; char pad2[8]; };

/**
 * Adds, updates or removes a mixer voice's clipping filter.
 *
 * A positive value creates the filter on first use (an 80-byte node, order 0x78, at +0x54 of the voice's state; set up
 * by func_003CB528 and linked by SFILTER_add) and sets its clip level from the value (func_003CB4A8). Zero or less
 * unlinks the node (SFILTER_remove) and frees it. Called by func_003C30C8.
 *
 * @param voice  the mixer voice
 * @param level  the clip level (up to 0x8000), 0 or less to remove the filter
 */
void func_003C8B80(int voice, int level) {
    struct E_8B80 *voiceState = (struct E_8B80*)sndmix.smv + voice;
    if (level > 0) {
        if (voiceState->q == 0) {
            struct SFILTERNODE *n = (struct SFILTERNODE *)f_5C60_8B80(80);
            voiceState->q = n;
            (*(int*)&n->restorefn) = 0;
            voiceState->q->priority = 0x78;
            f_B528_8B80(voiceState->q);
            f_8F48_8B80(voiceState->sub, voiceState->q);
        }
        f_B4A8_8B80(voiceState->q, level);
        return;
    }
    if (voiceState->q != 0) {
        f_8FB0_8B80(voiceState->sub, voiceState->q);
        f_5E98_8B80(voiceState->q);
        voiceState->q = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_sethighpass);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned short D_0050A912_8C30[] __asm__("D_0050A912");
extern void *SNDMEMI_alloc_8C30(int) __asm__("SNDMEMI_alloc");
extern void SNDMEMI_free_8C30(void *) __asm__("SNDMEMI_free");
extern void SFILTER_createHPFFIR8_8C30(void *) __asm__("SFILTER_createHPFFIR8");
extern void SFILTER_add_8C30(void *, void *) __asm__("SFILTER_add");
extern void SFILTER_remove_8C30(void *, void *) __asm__("SFILTER_remove");
extern void SFILTER_modifyHPFFIR8_8C30(void *, int *) __asm__("SFILTER_modifyHPFFIR8");

/**
 * Sets a mixer voice's high-pass cutoff, adding or removing its high-pass filter.
 *
 * A positive cutoff creates the filter on first use (a 288-byte node, order 80, at +0x5C of the voice's state; set up
 * by SFILTER_createHPFFIR8 and linked by SFILTER_add) and passes {cutoff << 8, output rate (D_0050A912) << 8} to
 * SFILTER_modifyHPFFIR8. Zero or less unlinks (SFILTER_remove) and frees the filter. Called by SNDPLATFORM_highpass.
 *
 * @param voice  the mixer voice
 * @param cutoff  the cutoff in Hz, 0 or less to remove the filter
 */
void MIX_sethighpass(int voice, int cutoff) {
    int sp[2];
    SNDMIXVOICE* voiceState;
    char *buf;

    voiceState = &sndmix.smv[voice];
    if (cutoff > 0) {
        if ((char*)voiceState->phps == 0) {
            char *hpfState = SNDMEMI_alloc_8C30(288);
            voiceState->phps = (HPFSTATE*)hpfState;
            *(int*)(hpfState + 4) = 0;
            *(short*)&voiceState->phps->sfn.priority = 80;
            voiceState->phps->sfn.requester = 0;
            SFILTER_createHPFFIR8_8C30((char*)voiceState->phps);
            SFILTER_add_8C30(&voiceState->filterhead, (char*)voiceState->phps);
        }
        sp[0] = cutoff << 8;
        sp[1] = D_0050A912_8C30[0] << 8;
        SFILTER_modifyHPFFIR8_8C30((char*)voiceState->phps, sp);
        return;
    }
    buf = (char*)voiceState->phps;
    if (buf != 0) {
        SFILTER_remove_8C30(&voiceState->filterhead, buf);
        SNDMEMI_free_8C30((char*)voiceState->phps);
        voiceState->phps = 0;
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003C8D00);

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_filteradd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *SNDMEMI_alloc_8E28(int) __asm__("SNDMEMI_alloc");
extern void SFILTER_add_8E28(int, void *) __asm__("SFILTER_add");

/**
 * Builds a custom filter for a mixer voice from a descriptor and links it into the voice's chain.
 *
 * Allocates the state size (+0x04), calls the descriptor's init (+0x0C) with the state, its parameter (+0x00) and the
 * voice's channel index, then sets the node's order (+0x08), process (+0x10) and release (+0x14) functions and links it
 * (SFILTER_add). Called by SNDPLATFORM_filteradd.
 *
 * @param voice  the mixer voice
 * @param channelIndex  the voice's channel index within its sound
 * @param filterDesc  the filter descriptor
 */
void MIX_filteradd(int voice, int channelIndex, SNDFILTERDEF* filterDesc) {
    int voiceState;
    int temp_7;
    SFILTERNODE* temp_2;

    temp_7 = voice * 0x60;
    voiceState = (int)sndmix.smv + temp_7;
    temp_2 = (SFILTERNODE*)SNDMEMI_alloc_8E28((filterDesc->statesize));
    ((int (*)(void*, int, int))filterDesc->filterinit)(temp_2, (filterDesc->id), channelIndex);
    temp_2->priority = (unsigned short) (*(unsigned short*)&filterDesc->priority);
    ((int)temp_2->filterfn) = (int) ((int)filterDesc->filter);
    ((int)temp_2->restorefn) = (int) ((int)filterDesc->filterrestore);
    (*(signed char*)&temp_2->requester) = 0;
    SFILTER_add_8E28(voiceState + 0x40, temp_2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIX_filterremove);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SFILTER_remove_8EC0(char *, char *) __asm__("SFILTER_remove");
extern void SNDMEMI_free_8EC0(char *) __asm__("SNDMEMI_free");

/**
 * Removes the filter with a given order from a mixer voice's chain: runs its release function, unlinks it
 * (SFILTER_remove) and frees it.
 *
 * Assumes the chain is not empty. Called by SNDPLATFORM_filterremove.
 *
 * @param voice  the mixer voice
 * @param filterOrder  the filter's order (+0x18 of the node)
 */
void MIX_filterremove(int voice, int filterOrder) {
    char *rec = (char*)&sndmix.smv[voice];
    SFILTERNODE* node = (SFILTERNODE*)(*(char **)(rec + 0x40));
    void (*fn)(char *);
    do {
        if (node->priority == filterOrder) {
            fn = (void (*)(char*))node->restorefn;
            if (fn != 0) {
                fn(((char*)node));
            }
            SFILTER_remove_8EC0(rec + 0x40, ((char*)node));
            SNDMEMI_free_8EC0(((char*)node));
            return;
        }
        node = (SFILTERNODE*)node->pfnnext;
    } while (((char*)node) != 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_add);
#ifdef SKIP_ASM
/**
 * Inserts a filter node into a chain, keeping the chain sorted by the node's order (+0x18, ascending).
 *
 * Nodes link forward at +0x08 and back at +0x10. Used by every mixer function that adds a filter (MIX_create,
 * MIX_playinit, MIX_setpitch, func_003C8B80, MIX_sethighpass, MIX_filteradd).
 *
 * @param head  the chain's head pointer
 * @param node  the node
 */
void SFILTER_add(char **head, SFILTERNODE* node) {
    SFILTERNODE* cur;
    SFILTERNODE* prev;

    cur = (SFILTERNODE*)(*head);
    prev = 0;
    while (((char*)cur) != 0 && cur->priority < node->priority) {
        prev = cur;
        cur = (SFILTERNODE*)cur->pfnnext;
    }
    node->pfnnext = (SFILTERNODE*)cur;
    if (((char*)cur) != 0) {
        cur->pfnprev1 = (SFILTERNODE*)node;
    }
    if (prev == 0) {
        *head = ((char*)node);
    } else {
        prev->pfnnext = node;
        node->pfnprev1 = prev;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_remove);
#ifdef SKIP_ASM
typedef struct N_8FB0 { char pad0[8]; struct N_8FB0 *next; int pad1; struct N_8FB0 *prev; } N_8FB0;

/**
 * Unlinks a filter node from a chain (forward link +0x08, back link +0x10), without freeing it.
 *
 * Does nothing when the node is not in the chain. Called by func_003C8B80, MIX_sethighpass and MIX_filterremove.
 *
 * @param head  the chain's head pointer
 * @param node  the node
 */
void SFILTER_remove(N_8FB0 **head, N_8FB0 *node) {
    N_8FB0 *temp_5;
    N_8FB0 *var_2;
    N_8FB0 *var_6;

    var_6 = *head;
    var_2 = var_6->next;
    if (node == var_6) {
        *head = var_2;
        return;
    }
    while (var_2 != 0 && var_2 != node) {
        var_6 = var_2;
        var_2 = var_6->next;
    }
    if ((var_2 != 0) && (var_2 == node)) {
        temp_5 = node->next;
        if (temp_5 != 0) {
            temp_5->prev = var_6;
        }
        var_6->next = var_6->next->next;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_destroy);
#ifdef SKIP_ASM
/**
 * Runs a filter node's release function (+0x04), if any, passing the node on. No callers.
 *
 * @param node  the node
 * @return 0
 */
int SFILTER_destroy(SFILTERNODE* node) {
    int (*temp_2)();

    temp_2 = (int (*)())node->restorefn;
    if (temp_2 != 0) {
        temp_2();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_cleanup);
#ifdef SKIP_ASM
/**
 * Releases a filter graph: walks the nodes feeding this one (+0x08, +0x0C) depth first, then runs this node's release
 * function (+0x04) and marks it done (+0x1B), so shared nodes are released once.
 *
 * Calls itself; no other callers.
 *
 * @param node  the node to start from
 */
void SFILTER_cleanup(SFILTERNODE* node) {
    int (*temp_2)(void *);
    void *temp_4;
    void *temp_4_2;

    temp_4 = ((void*)node->pfnnext);
    if ((temp_4 != 0) && ((*(unsigned char *)((char*)(temp_4) + (0x1B))) == 0)) {
        SFILTER_cleanup((SFILTERNODE*)temp_4);
    }
    temp_4_2 = ((void*)node->pfnnext2);
    if ((temp_4_2 != 0) && ((*(unsigned char *)((char*)(temp_4_2) + (0x1B))) == 0)) {
        SFILTER_cleanup((SFILTERNODE*)temp_4_2);
    }
    temp_2 = ((int (*)(void*))node->restorefn);
    if (temp_2 != 0) {
        temp_2(node);
    }
    (*(signed char*)&node->clean) = 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_connect);
#ifdef SKIP_ASM
/**
 * Connects an output of one filter node to an input of another.
 *
 * Output slot 1 or 2 is the source's +0x10 or +0x14, input slot 1 or 2 the destination's +0x08 or +0x0C; the input must
 * be free. The destination also records the output slot (+0x1A). Called by func_003CC070 when building an effect graph.
 *
 * @param source  the node whose output is used
 * @param dest  the node that receives it
 * @param outSlot  the output slot (1 or 2)
 * @param inSlot  the input slot (1 or 2)
 * @return 0, or -1 for a bad slot or a used input
 */
int SFILTER_connect(SFILTERNODE* source, SFILTERNODE* dest, int outSlot, int inSlot) {
    if (outSlot == 1 && inSlot == 1 && (int)dest->pfnnext == 0) {
        source->pfnprev1 = (SFILTERNODE*)((int)dest);
        dest->pfnnext = (SFILTERNODE*)((int)source);
        *(char*)&dest->requester = inSlot;
        return 0;
    }
    if (outSlot == 1 && inSlot == 2 && (int)dest->pfnnext2 == 0) {
        source->pfnprev1 = (SFILTERNODE*)((int)dest);
        dest->pfnnext2 = (SFILTERNODE*)((int)source);
        *(char*)&dest->requester = outSlot;
        return 0;
    }
    if (outSlot == 2 && inSlot == 1 && (int)dest->pfnnext == 0) {
        source->pfnprev2 = (SFILTERNODE*)((int)dest);
        dest->pfnnext = (SFILTERNODE*)((int)source);
        *(char*)&dest->requester = outSlot;
        return 0;
    }
    if (outSlot == 2 && inSlot == 2 && (int)dest->pfnnext2 == 0) {
        source->pfnprev2 = (SFILTERNODE*)((int)dest);
        dest->pfnnext2 = (SFILTERNODE*)((int)source);
        *(char*)&dest->requester = inSlot;
        return 0;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_disconnect);
#ifdef SKIP_ASM
/**
 * Disconnects an output of one filter node from an input of another, if exactly that link exists (the reverse of
 * SFILTER_connect). No callers.
 *
 * @param source  the node whose output was used
 * @param dest  the node that received it
 * @param x  the output slot (1 or 2)
 * @param y  the input slot (1 or 2)
 * @return 0, or -1 when they are not linked that way
 */
int SFILTER_disconnect(int *source, int *dest, int x, int y) {
    if (x == 1 && y == 1 && source[4] == (int)dest && dest[2] == (int)source) {
        source[4] = 0;
        dest[2] = 0;
        return 0;
    }
    if (x == 1 && y == 2 && source[4] == (int)dest && dest[3] == (int)source) {
        source[4] = 0;
        dest[3] = 0;
        return 0;
    }
    if (x == 2 && y == 1 && source[5] == (int)dest && dest[2] == (int)source) {
        source[5] = 0;
        dest[2] = 0;
        return 0;
    }
    if (x == 2 && y == 2 && source[5] == (int)dest && dest[3] == (int)source) {
        source[5] = 0;
        dest[3] = 0;
        return 0;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_initunpack16);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x18];
    void *p18, *p1c, *p20;
    char pad1[0x60 - 0x24];
    int i60, i64, i68;
} G_9268;
extern void f_9268_a(void) __asm__("SFILTER_unpackfinit");
extern void f_9268_b(void) __asm__("SFILTER_unpacklfinit");
extern void f_9268_c(void) __asm__("SFILTER_unpackpfinit");

extern G_9268 sndmix_9268 __asm__("sndmix");
/**
 * Registers the 16-bit PCM decoders as mixer filter factories for sample format 8 (row 0 of the table at +0x18 of
 * sndmix).
 *
 * One-shot: SFILTER_unpackfinit (44-byte node); looped: SFILTER_unpacklfinit (48); packet-fed: SFILTER_unpackpfinit (52). Called by
 * MIX_create.
 */
void MIXI_initunpack16(void) {
sndmix_9268.p18 = f_9268_a;
    sndmix_9268.i60 = 44;
    sndmix_9268.p1c = f_9268_b;
    sndmix_9268.i64 = 0x30;
    sndmix_9268.p20 = f_9268_c;
    sndmix_9268.i68 = 52;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_initunpackxa);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad0[0x24];
    void *p24, *p28, *p2c;
    char pad1[0x6C - 0x30];
    int i6c, i70, i74;
} G_92B0;
extern void f_92B0_a(void) __asm__("SFILTER_unpackxafinit");
extern void f_92B0_b(void) __asm__("SFILTER_unpackxalfinit");
extern void f_92B0_c(void) __asm__("SFILTER_unpackxapfinit");

extern G_92B0 sndmix_92B0 __asm__("sndmix");
/**
 * Registers the EA-XA ADPCM decoders as mixer filter factories for sample format 10 (row 1 of the table at +0x18 of
 * sndmix).
 *
 * One-shot: SFILTER_unpackxafinit (44-byte node); looped: SFILTER_unpackxalfinit (84); packet-fed: SFILTER_unpackxapfinit (60). Called by
 * MIX_create.
 */
void MIXI_initunpackxa(void) {
sndmix_92B0.p24 = f_92B0_a;
    sndmix_92B0.i6c = 44;
    sndmix_92B0.i70 = 84;
    sndmix_92B0.p28 = f_92B0_b;
    sndmix_92B0.p2c = f_92B0_c;
    sndmix_92B0.i74 = 0x3C;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", MIXI_initunpackmt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct {
    char pad[0x30];
    void *f30, *f34, *f38, *f3C, *f40, *f44;
    char pad2[0x30];
    int i78, i7C, i80, i84, i88, i8C;
} G_92F8;
extern void SFILTER_unpackmtfinit_9420(void) __asm__("SFILTER_unpackmtfinit");
extern void SFILTER_unpackmtlfinit_9618(void) __asm__("SFILTER_unpackmtlfinit");
extern void SFILTER_unpackmtpfinit_9960(void) __asm__("SFILTER_unpackmtpfinit");

extern G_92F8 sndmix_5B40 __asm__("sndmix");
/**
 * Registers the MicroTalk decoders as mixer filter factories for sample formats 4 and 22 (rows 2 and 3 of the table at
 * +0x18 of sndmix, both the same).
 *
 * One-shot: SFILTER_unpackmtfinit (44-byte node); looped: SFILTER_unpackmtlfinit (48); packet-fed: SFILTER_unpackmtpfinit (60). Called by
 * MIX_create.
 */
void MIXI_initunpackmt(void) {
    sndmix_5B40.f30 = SFILTER_unpackmtfinit_9420;
    sndmix_5B40.i78 = 44;
    sndmix_5B40.f38 = SFILTER_unpackmtpfinit_9960;
    sndmix_5B40.i80 = 0x3C;
    sndmix_5B40.f34 = SFILTER_unpackmtlfinit_9618;
    sndmix_5B40.i7C = 0x30;
sndmix_5B40.f3C = SFILTER_unpackmtfinit_9420;
    sndmix_5B40.i84 = 44;
    sndmix_5B40.f44 = SFILTER_unpackmtpfinit_9960;
    sndmix_5B40.i8C = 0x3C;
    sndmix_5B40.f40 = SFILTER_unpackmtlfinit_9618;
    sndmix_5B40.i88 = 0x30;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackgetframemtf);
#ifdef SKIP_ASM
/**
 * Returns a one-shot MicroTalk decoder's position: the frames decoded so far (+0x20).
 *
 * SFILTER_unpackmtfinit installs it as the voice's get-frame function (read by MIX_getframe).
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpackgetframemtf(UNPACKMTFSTATE* node) {
    return ((UNPACKMTFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CMTBLKDecf_Decode_9360(int, int **, int) __asm__("CMTBLKDecf_Decode");
extern void memset_9360(int *, int, int) __asm__("memset");

/**
 * Decodes frames of a one-shot MicroTalk sample into a float buffer (a filter node's process function).
 *
 * Once all frames (+0x1C) are decoded it returns -1, ending the voice. Otherwise it decodes up to @p count frames with
 * the decoder (+0x28; CMTBLKDecf_Decode), advances the position (+0x20) and zero-fills what the sample could not supply.
 *
 * @param node  the decoder node
 * @param count  the frames wanted
 * @param srcBuffer  unused (the source; decoders read their own data)
 * @param buffer  the float destination
 * @return 1, or -1 when the sample has ended
 */
int SFILTER_unpackmtf(UNPACKMTFSTATE* node, int count, int srcBuffer, int *buffer) {
    int *cursor;
    int temp_2;

    cursor = buffer;
    if ((unsigned int) (node->curframe) >= (unsigned int) (node->totalframes)) {
        return -1;
    }
    temp_2 = CMTBLKDecf_Decode_9360(((int)node->pDecoder), &cursor, count);
    node->curframe = (unsigned int) ((node->curframe) + temp_2);
    cursor = cursor + temp_2;
    if (temp_2 < count) {
        memset_9360(cursor, 0, (count -= temp_2) * 4);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtfrestore);
#ifdef SKIP_ASM
extern void CMTBLKDecf_operator_delete(int);

/**
 * Release function of a one-shot MicroTalk node: destroys its decoder (+0x28) with CMTBLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackmtfrestore(UNPACKMTFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKMTFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CMTBLKDecf_operator_delete(temp_4);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtfinit);
#ifdef SKIP_ASM
extern void SFILTER_unpackmtf_9420(void) __asm__("SFILTER_unpackmtf");
extern void SFILTER_unpackmtfrestore_9420(void) __asm__("SFILTER_unpackmtfrestore");
extern void SFILTER_unpackgetframemtf_9420(void) __asm__("SFILTER_unpackgetframemtf");
extern int CMTBLKDecf_operator_new_9420(int) __asm__("CMTBLKDecf_operator_new");
extern int CMTBLKDecf_CMTBLKDecf_9420(int) __asm__("CMTBLKDecf_CMTBLKDecf");
extern struct MTFSTATE CMTBLKDecf_GetState_9420(int) __asm__("CMTBLKDecf_GetState");
extern void CMTBLKDecf_SetState_9420(int, struct MTFSTATE *, int) __asm__("CMTBLKDecf_SetState");
extern void CMTBLKDecf_SetCodecVersion_9420(int, int) __asm__("CMTBLKDecf_SetCodecVersion");
extern int CMTBLKDecf_Feed_9420(int, int, int, int) __asm__("CMTBLKDecf_Feed");

/**
 * Sets up a one-shot MicroTalk decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackmtf) and release (SFILTER_unpackmtfrestore) functions, stores the frame count (init +0x0C) and
 * codec version (init +0x24), registers SFILTER_unpackgetframemtf as get-frame (init +0x28), creates a 3432-byte decoder
 * (CMTBLKDecf_operator_new, CMTBLKDecf_CMTBLKDecf), resets its state (CMTBLKDecf_GetState / CMTBLKDecf_SetState), selects codec 0 for versions below
 * 3, else 1 (CMTBLKDecf_SetCodecVersion), and feeds it the whole sample (init +0x00; CMTBLKDecf_Feed).
 *
 * @param cfg  the init block from MIX_playinit
 */
void SFILTER_unpackmtfinit(struct UNPACKMTFSTATE *self, struct UNPACKINITPARAMS *cfg) {
    struct MTFSTATE newState;
    struct MTFSTATE curState;
    (*(void**)&self->sfn) = SFILTER_unpackmtf_9420;
    self->totalframes = cfg->totalframes;
    self->curframe = 0;
    self->platformver = cfg->platformver;
    cfg->unpackgetframe = SFILTER_unpackgetframemtf_9420;
    (*(void**)&self->sfn.restorefn) = SFILTER_unpackmtfrestore_9420;
    (*(int*)&self->pDecoder) = CMTBLKDecf_CMTBLKDecf_9420(CMTBLKDecf_operator_new_9420(3432));
    curState = CMTBLKDecf_GetState_9420(((int)self->pDecoder));
    newState = curState;
    newState.doWholeInit = 1;
    CMTBLKDecf_SetState_9420(((int)self->pDecoder), &newState, 0);
    if (self->platformver < 3) {
        CMTBLKDecf_SetCodecVersion_9420(((int)self->pDecoder), 0);
    } else {
        CMTBLKDecf_SetCodecVersion_9420(((int)self->pDecoder), 1);
    }
    CMTBLKDecf_Feed_9420(((int)self->pDecoder), ((int)cfg->psample), ((int)self->totalframes) * 4, ((int)self->totalframes));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackgetframemtlf);
#ifdef SKIP_ASM
/**
 * Returns a looped MicroTalk decoder's position (+0x20). SFILTER_unpackmtlfinit installs it as the voice's get-frame function.
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpackgetframemtlf(UNPACKMTLFSTATE* node) {
    return ((UNPACKMTLFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtlf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a, b, c; } Q_9520;
extern int CMTBLKDecf_Decode_9520(int, int *, int) __asm__("CMTBLKDecf_Decode");
extern void CMTBLKDecf_SetState_9520(int, void *, int) __asm__("CMTBLKDecf_SetState");
extern void CMTBLKDecf_Feed_9520(int, int, int, int) __asm__("CMTBLKDecf_Feed");

/**
 * Decodes frames of a looped MicroTalk sample into a float buffer (a filter node's process function).
 *
 * Decodes with the decoder (+0x1C; CMTBLKDecf_Decode). When the decoder runs dry it loops: the position (+0x20) goes back
 * to the sustain start (+0x24), the decoder state is reset and it is fed the loop data (+0x2C) from sustain start to
 * sustain end (+0x28). Never ends on its own.
 *
 * @param node  the decoder node
 * @param count  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buffer  the float destination
 * @return 1
 */
int SFILTER_unpackmtlf(UNPACKMTLFSTATE* node, int count, int srcBuffer, int buffer) {
    Q_9520 state;
    int cursor;
    int n, loopLength;

    cursor = buffer;
    if (count > 0) {
        do {
            n = CMTBLKDecf_Decode_9520((int)node->pDecoder, &cursor, count);
            count -= n;
            cursor += n * 4;
            node->curframe = node->curframe + n;
            if (n < count) {
                node->curframe = node->loopstart;
                state.b = 0;
                state.a = 0;
                state.c = 1;
                CMTBLKDecf_SetState_9520((int)node->pDecoder, &state, 0);
                loopLength = (node->loopend - node->loopstart) + 1;
                CMTBLKDecf_Feed_9520((int)node->pDecoder, (int)node->ploop, loopLength * 4, loopLength);
            }
        } while (count > 0);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtlfrestore);
#ifdef SKIP_ASM
extern void CMTBLKDecf_operator_delete(int);

/**
 * Release function of a looped MicroTalk node: destroys its decoder (+0x1C) with CMTBLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackmtlfrestore(UNPACKMTLFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKMTLFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CMTBLKDecf_operator_delete(temp_4);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtlfinit);
#ifdef SKIP_ASM
extern void SFILTER_unpackmtlf_9618(void) __asm__("SFILTER_unpackmtlf");
extern void SFILTER_unpackmtlfrestore_9618(void) __asm__("SFILTER_unpackmtlfrestore");
extern void SFILTER_unpackgetframemtlf_9618(void) __asm__("SFILTER_unpackgetframemtlf");
extern int CMTBLKDecf_operator_new_9618(int) __asm__("CMTBLKDecf_operator_new");
extern int CMTBLKDecf_CMTBLKDecf_9618(int) __asm__("CMTBLKDecf_CMTBLKDecf");
extern struct MTFSTATE CMTBLKDecf_GetState_9618(int) __asm__("CMTBLKDecf_GetState");
extern void CMTBLKDecf_SetState_9618(int, struct MTFSTATE *, int) __asm__("CMTBLKDecf_SetState");
extern int CMTBLKDecf_Feed_9618(int, int, int, int) __asm__("CMTBLKDecf_Feed");

/**
 * Sets up a looped MicroTalk decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackmtlf) and release (SFILTER_unpackmtlfrestore) functions, keeps the loop data (init +0x04), sustain
 * start (+0x10) and end (+0x14), creates a 3432-byte decoder (CMTBLKDecf_operator_new, CMTBLKDecf_CMTBLKDecf), resets its state and feeds
 * it the sample (init +0x00) up to the sustain start; registers SFILTER_unpackgetframemtlf as get-frame (init +0x28).
 *
 * @param cfg  the init block from MIX_playinit
 */
void SFILTER_unpackmtlfinit(struct UNPACKMTLFSTATE *self, struct UNPACKINITPARAMS *cfg) {
    struct MTFSTATE newState;
    struct MTFSTATE curState;
    (*(void**)&self->sfn) = SFILTER_unpackmtlf_9618;
    (*(void**)&self->sfn.restorefn) = SFILTER_unpackmtlfrestore_9618;
    (*(int*)&self->ploop) = ((int)cfg->ploopstart);
    self->curframe = 0;
    self->loopstart = cfg->sustainstart;
    self->loopend = cfg->sustainend;
    (*(int*)&self->pDecoder) = CMTBLKDecf_CMTBLKDecf_9618(CMTBLKDecf_operator_new_9618(3432));
    curState = CMTBLKDecf_GetState_9618(((int)self->pDecoder));
    newState = curState;
    newState.doWholeInit = 1;
    CMTBLKDecf_SetState_9618(((int)self->pDecoder), &newState, 0);
    CMTBLKDecf_Feed_9618(((int)self->pDecoder), ((int)cfg->psample), self->loopstart * 4, self->loopstart);
    cfg->unpackgetframe = SFILTER_unpackgetframemtlf_9618;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtpf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { int a, b, c; } Q_96F0;
extern void f_7010_96F0(int, int, int) __asm__("SNDPKTPLAYI_freeframes");
extern int f_6DE0_96F0(int, int, char *, int *) __asm__("SNDPKTPLAYI_get");
extern int f_DE68_96F0(int, int *, int) __asm__("CMTBLKDecf_Decode");
extern Q_96F0 f_E078_96F0(int) __asm__("CMTBLKDecf_GetState");
extern void f_E0B8_96F0(int, Q_96F0 *, int) __asm__("CMTBLKDecf_SetState");
extern void f_E0D8_96F0(int, int) __asm__("CMTBLKDecf_SetCodecVersion");
extern void f_DDE8_96F0(int, int, int, int) __asm__("CMTBLKDecf_Feed");
extern void f_6210_96F0(int, int, int) __asm__("memset");

/**
 * Decodes MicroTalk audio fed from a packet player into a float buffer (a filter node's process function).
 *
 * First reports the frames played since the last call (+0x28) to the player (SNDPKTPLAYI_freeframes; instance +0x24, channel
 * +0x38). Then decodes (+0x34; CMTBLKDecf_Decode); when the decoder runs dry it takes the channel's next packet
 * (SNDPKTPLAYI_get): the decoder state is reset (kept across packets marked as continued), the codec picked from the
 * version (+0x30) and the packet fed in. Without a packet the rest is zero-filled once anything played, and decoding
 * stops.
 *
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buf  the float destination
 * @return the frames decoded
 */
int SFILTER_unpackmtpf(UNPACKMTPFSTATE* self, int n, int srcBuffer, int buf) {
    Q_96F0 newState;
    Q_96F0 curState;
    int last;
    int total;
    int got;

    total = 0;
    if ((int)self->prevframesunpacked != 0) {
        f_7010_96F0(self->packetinstancehandle, self->samplechan, (int)self->prevframesunpacked);
        self->prevframesunpacked = 0;
    }
    while (n > 0) {
        got = f_DE68_96F0((int)self->pDecoder, &buf, n);
        self->curframe += got;
        total += got;
        buf += got * 4;
        *(int*)&self->prevframesunpacked += got;
        if (got < n) {
            n -= got;
            self->psample = (unsigned char*)f_6DE0_96F0(self->packetinstancehandle, self->samplechan, (char*)&self->totalframes, &last);
            if ((int)self->psample != 0) {
                self->curframe = 0;
                if (self->platformver < 3) {
                    if (last == 0) {
                        curState = f_E078_96F0((int)self->pDecoder);
                        newState = curState;
                        newState.c = 1;
                        f_E0B8_96F0((int)self->pDecoder, &newState, 0);
                    } else {
                        curState = f_E078_96F0((int)self->pDecoder);
                        newState = curState;
                        newState.c = 0;
                        f_E0B8_96F0((int)self->pDecoder, &newState, 0);
                    }
                    f_E0D8_96F0((int)self->pDecoder, 0);
                } else {
                    if (last == 0) {
                        curState = f_E078_96F0((int)self->pDecoder);
                        newState = curState;
                        newState.b = 0;
                        newState.c = 1;
                    } else {
                        curState = f_E078_96F0((int)self->pDecoder);
                        newState = curState;
                        newState.c = 0;
                    }
                    f_E0B8_96F0((int)self->pDecoder, &newState, 0);
                    f_E0D8_96F0((int)self->pDecoder, 1);
                }
                f_DDE8_96F0((int)self->pDecoder, (int)self->psample + 1, self->totalframes * 4, self->totalframes);
            } else {
                if (n > 0 && (int)self->prevframesunpacked != 0) {
                    f_6210_96F0(buf, 0, n * 4);
                }
                self->totalframes = 0;
                n = 0;
            }
        } else {
            n -= got;
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtpfrestore);
#ifdef SKIP_ASM
extern void CMTBLKDecf_operator_delete(int);

/**
 * Release function of a packet-fed MicroTalk node: destroys its decoder (+0x34) with CMTBLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackmtpfrestore(UNPACKMTPFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKMTPFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CMTBLKDecf_operator_delete(temp_4);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackmtpfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int SFILTER_unpackmtpf_9960[] __asm__("SFILTER_unpackmtpf");
extern int SFILTER_unpackmtpfrestore_9960[] __asm__("SFILTER_unpackmtpfrestore");
extern int SNDDRV_getmastervoice_9960(int) __asm__("SNDDRV_getmastervoice");
extern signed char SNDDRV_getsamplechan_9960(int) __asm__("SNDDRV_getsamplechan");
extern int SNDPKTPLAYI_voicetopackethandle_9960(int) __asm__("SNDPKTPLAYI_voicetopackethandle");
extern int CMTBLKDecf_operator_new_9960(int) __asm__("CMTBLKDecf_operator_new");
extern int CMTBLKDecf_CMTBLKDecf_9960(int) __asm__("CMTBLKDecf_CMTBLKDecf");

/**
 * Sets up a packet-fed MicroTalk decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackmtpf) and release (SFILTER_unpackmtpfrestore) functions, finds the voice's packet player instance
 * (SNDPKTPLAYI_voicetopackethandle of SNDDRV_getmastervoice of the voice, init +0x1C) and channel (SNDDRV_getsamplechan), keeps the codec version (init
 * +0x24) and frame count (init +0x0C) and creates a 3432-byte decoder (CMTBLKDecf_operator_new, CMTBLKDecf_CMTBLKDecf).
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackmtpfinit(UNPACKMTPFSTATE* node, UNPACKINITPARAMS* source) {
    (*(int**)&node->sfn) = SFILTER_unpackmtpf_9960;
    node->packetinstancehandle = SNDPKTPLAYI_voicetopackethandle_9960(SNDDRV_getmastervoice_9960((source->voice)));
    (*(signed char*)&node->samplechan) = SNDDRV_getsamplechan_9960((source->voice));
    ((int)node->psample) = 0;
    (*(short*)&node->curframe) = 0;
    node->platformver = (source->platformver);
    node->totalframes = (source->totalframes);
    ((int*)node->sfn.restorefn) = SFILTER_unpackmtpfrestore_9960;
    ((int)node->pDecoder) = CMTBLKDecf_CMTBLKDecf_9960(CMTBLKDecf_operator_new_9960(3432));
    ((int)node->prevframesunpacked) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackfgetframe);
#ifdef SKIP_ASM
/**
 * Returns a one-shot 16-bit PCM decoder's position (+0x20). SFILTER_unpackfinit installs it as the voice's get-frame
 * function.
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpackfgetframe(UNPACKFSTATE* node) {
    return ((UNPACKFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackf);
#ifdef SKIP_ASM

/**
 * Converts frames of a one-shot 16-bit PCM sample to floats (a filter node's process function).
 *
 * Returns -1 once the position (+0x20) has reached the frame count (+0x24). Otherwise it advances the position and
 * converts the samples (+0x1C data; func_003C9E50, when +0x28 is set), zero-filling anything past the end.
 *
 * @param reader  the decoder node
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buf  the float destination
 * @return 1, or -1 when the sample has ended
 */
int SFILTER_unpackf(struct UNPACKFSTATE *reader, unsigned int n, int srcBuffer, int *buf) {
    unsigned int pos = reader->curframe;
    unsigned int cap = reader->totalframes;
    if (pos >= cap) return -1;
    reader->curframe = pos + n;
    if (pos + n < cap) {
        if (reader->bits16) {
            func_003C9E50(n, ((short*)reader->psample) + pos, buf);
        }
    } else {
        unsigned int first = cap - pos;
        if (reader->bits16) {
            func_003C9E50(first, ((short*)reader->psample) + pos, buf);
        }
        memset(buf + first, 0, (n - first) * 4);
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_unpackfgetframe_9AC8(void) __asm__("SFILTER_unpackfgetframe");
void SFILTER_unpackf_9AC8(void) __asm__("SFILTER_unpackf");

/**
 * Sets up a one-shot 16-bit PCM decoder node (a filter factory).
 *
 * Installs the process function SFILTER_unpackf and no release, keeps the sample (init +0x00), frame count (+0x0C) and
 * convert flag (+0x18), starts at frame 0 and registers SFILTER_unpackfgetframe as get-frame (init +0x28).
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackfinit(UNPACKFSTATE* node, UNPACKINITPARAMS* source) {
    int dataPtr;
    *(void (**)(void))&node->sfn = SFILTER_unpackf_9AC8;
    dataPtr = (int)source->psample;
    node->curframe = 0;
    node->psample = (char*)dataPtr;
    node->totalframes = source->totalframes;
    node->bits16 = source->bits16;
    source->unpackgetframe = (void*)SFILTER_unpackfgetframe_9AC8;
    node->sfn.restorefn = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpacklfgetframe);
#ifdef SKIP_ASM
/**
 * Returns a looped 16-bit PCM decoder's position (+0x20). SFILTER_unpacklfinit installs it as the voice's get-frame function.
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpacklfgetframe(UNPACKLFSTATE* node) {
    return ((UNPACKLFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpacklf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_9E50_9B10(int, void *, void *) __asm__("func_003C9E50");

/**
 * Converts frames of a looped 16-bit PCM sample to floats (a filter node's process function).
 *
 * Converts runs from the position (+0x20) up to the sustain end (+0x28), wrapping back to the sustain start (+0x24)
 * (func_003C9E50, when +0x2C is set). Never ends on its own.
 *
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param dst  the float destination
 * @return 1
 */
int SFILTER_unpacklf(struct UNPACKLFSTATE *self, int n, int srcBuffer, int *dst) {
    int runLength;
    while (n > 0) {
        runLength = self->sustainend - self->curframe + 1;
        if (n < runLength)
            runLength = n;
        if (self->bits16 != 0)
            f_9E50_9B10(runLength, ((short*)self->psample) + self->curframe, dst);
        dst += runLength;
        self->curframe = self->curframe + runLength;
        n -= runLength;
        if (self->sustainend < self->curframe)
            self->curframe = self->sustainstart;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpacklfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_unpacklf_9BC8(void) __asm__("SFILTER_unpacklf");
void SFILTER_unpacklfgetframe_9BC8(void) __asm__("SFILTER_unpacklfgetframe");

/**
 * Sets up a looped 16-bit PCM decoder node (a filter factory).
 *
 * Installs the process function SFILTER_unpacklf, keeps the sample (init +0x00), sustain start (+0x10) and end (+0x14) and
 * convert flag (+0x18), starts at frame 0 and registers SFILTER_unpacklfgetframe as get-frame (init +0x28).
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpacklfinit(int *node, int *source) {
    node[0] = (int)SFILTER_unpacklf_9BC8;
    node[7] = source[0];
    node[8] = 0;
    node[9] = source[4];
    node[10] = source[5];
    node[11] = source[6];
    source[10] = (int)SFILTER_unpacklfgetframe_9BC8;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackpf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDPKTPLAYI_get_9C08(int, unsigned char, int *, int *) __asm__("SNDPKTPLAYI_get");
extern void SNDPKTPLAYI_freeframes_9C08(int, unsigned char, int) __asm__("SNDPKTPLAYI_freeframes");
extern void func_003C9E50_9C08(int, int, int *) __asm__("func_003C9E50");
extern void memset_9C08(int *, int, int) __asm__("memset");

int SFILTER_unpackpf_9C08(char *o, int n, int unused, int *buf) __asm__("SFILTER_unpackpf");
/**
 * Converts 16-bit PCM audio fed from a packet player to floats (a filter node's process function).
 *
 * First reports the frames played since the last call (+0x2C) to the player (SNDPKTPLAYI_freeframes; instance +0x28, channel
 * +0x31). Then converts the current packet (+0x1C, position +0x24 of +0x20 frames; func_003C9E50 when +0x30 is set),
 * fetching the channel's next packet (SNDPKTPLAYI_get) when it is used up. Without a packet the rest is zero-filled once
 * anything played.
 *
 * @param node  the decoder node
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buf  the float destination
 * @return the frames produced since the report (0 when nothing was available)
 */
int SFILTER_unpackpf_9C08(char *vvnode, int n, int srcBuffer, int *buf) {
    UNPACKPFSTATE* vnode = (UNPACKPFSTATE*)vvnode;
    UNPACKPFSTATE* node = (UNPACKPFSTATE*)vnode;
    int blockLen, last;
    int runLength;
    if ((int)node->prevframesunpacked != 0) {
        SNDPKTPLAYI_freeframes_9C08(node->packetinstancehandle, node->samplechan, (int)node->prevframesunpacked);
        node->prevframesunpacked = 0;
    }
    while (n > 0) {
        if (node->curframe >= node->totalframes) {
            node->psample = (signed char*)SNDPKTPLAYI_get_9C08(node->packetinstancehandle, node->samplechan, &blockLen, &last);
            if ((int)node->psample == 0) {
                if ((int)node->prevframesunpacked != 0) {
                    memset_9C08(buf, 0, n * 4);
                }
                return (int)node->prevframesunpacked;
            }
            node->curframe = 0;
            node->totalframes = blockLen;
        }
        runLength = node->totalframes - node->curframe;
        if (n < runLength) runLength = n;
        if (node->bits16 != 0) {
            func_003C9E50_9C08(runLength, (int)node->psample + node->curframe * 2, buf);
        }
        buf += runLength;
        n -= runLength;
        node->curframe += runLength;
        *(int*)&node->prevframesunpacked += runLength;
    }
    return (int)node->prevframesunpacked;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackpfinit);
#ifdef SKIP_ASM
extern int SNDDRV_getmastervoice(int);
extern signed char SNDDRV_getsamplechan(int);
extern int SNDPKTPLAYI_voicetopackethandle(int);
extern void SFILTER_unpackpf(void);

/**
 * Sets up a packet-fed 16-bit PCM decoder node (a filter factory).
 *
 * Installs the process function SFILTER_unpackpf, finds the voice's packet player instance (SNDPKTPLAYI_voicetopackethandle of SNDDRV_getmastervoice
 * of the voice, init +0x1C) and channel (SNDDRV_getsamplechan), keeps the convert flag (init +0x18) and marks the current
 * packet as used up.
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackpfinit(UNPACKPFSTATE* node, UNPACKINITPARAMS* source) {
    (*(int**)&node->sfn) = (int*)&SFILTER_unpackpf;
    node->packetinstancehandle = SNDPKTPLAYI_voicetopackethandle(SNDDRV_getmastervoice((source->voice)));
    (*(signed char*)&node->samplechan) = SNDDRV_getsamplechan((source->voice));
    node->totalframes = -1;
    ((int)node->psample) = 0;
    node->curframe = 0;
    node->bits16 = (*(unsigned char*)&source->bits16);
    ((int)node->prevframesunpacked) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9DA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003C9DA0(int n, short *src, float *dst) {
    float *d = dst;
    int r = n & 3;
    float *end = d + n;
    short *s = src;
    if (r != 0) {
        int c = r;
        do {
            *d++ = (float)*s++;
        } while (--c != 0);
    }
    if (n >= 4) {
        // PORT: PS2-only hand-written inner loop (4 int16 -> float per iteration), one inline asm block.
        __asm__ volatile(
            ".set noreorder\n\t"
            "move $4, %0\n\t"
            "move $5, %1\n\t"
            "move $6, %2\n"
            "1:\n\t"
            "lh $8, 0x0($4)\n\t"
            "lh $9, 0x2($4)\n\t"
            "lh $10, 0x4($4)\n\t"
            "lh $11, 0x6($4)\n\t"
            "mtc1 $8, $f0\n\t"
            "mtc1 $9, $f1\n\t"
            "mtc1 $10, $f2\n\t"
            "mtc1 $11, $f3\n\t"
            "addi $5, $5, 0x10\n\t"
            "cvt.s.w $f0, $f0\n\t"
            "cvt.s.w $f1, $f1\n\t"
            "cvt.s.w $f2, $f2\n\t"
            "cvt.s.w $f3, $f3\n\t"
            "swc1 $f0, -0x10($5)\n\t"
            "swc1 $f1, -0xC($5)\n\t"
            "swc1 $f2, -0x8($5)\n\t"
            "swc1 $f3, -0x4($5)\n\t"
            "bne $5, $6, 1b\n\t"
            "addi $4, $4, 0x8\n\t"
            ".set reorder\n"
            : : "r"(s), "r"(d), "r"(end)
            : "$4", "$5", "$6", "$8", "$9", "$10", "$11", "$f0", "$f1", "$f2", "$f3", "memory");
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003C9E50);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void func_003C9DA0(int n, short *src, float *dst);

/**
 * Converts 16-bit samples to floats.
 *
 * Uses an MMI / VU0 loop for 16 samples at a time when the source is 8-byte and the destination 16-byte aligned; the
 * remainder, or everything when unaligned, goes through func_003C9DA0. Called by the PCM decoders (SFILTER_unpackf,
 * SFILTER_unpacklf, SFILTER_unpackpf).
 *
 * @param n  the number of samples
 * @param src  the 16-bit samples
 * @param dst  the float destination
 */
// PORT: hand-written MMI/VU0 loop converting 16 int16 samples to floats per pass (ld + pextlh/psraw sign extension,
// qmtc2.ni/vitof0, sqc2; fixed $4-$6/$8-$11 with hand-interleaved pointer steps and a bgtzl loop: MMI has no gcc 2.95
// pattern); the C around it (alignment checks, the func_003C9DA0 scalar fallback) is the compiler's. A port replaces
// the asm with `for (i = 0; i < n; i++) dst[i] = (float)src[i];`.
void func_003C9E50(int n, short *src, float *dst) {
    int rem;
    if (((int)src & 7) != 0 || ((int)dst & 0xF) != 0) {
        func_003C9DA0(n, src, dst);
        return;
    }
    rem = n & 0xF;
    if (rem != 0) {
        n &= 0x7FF0;
        func_003C9DA0(rem, src + n, dst + n);
        if (n <= 0) return;
    }
    /* PORT: the hand-written conversion loop (see above); n > 0 and a multiple of 16 here. */
    __asm__ volatile(
        ".set noreorder\n\t"
        "move $4, %0\n\t"
        "move $5, %1\n\t"
        "move $6, %2\n"
        "1:\n\t"
        "ld $8, 0x0($5)\n\t"
        "ld $9, 0x8($5)\n\t"
        "ld $10, 0x10($5)\n\t"
        "ld $11, 0x18($5)\n\t"
        "pextlh $8, $8, $0\n\t"
        "pextlh $9, $9, $0\n\t"
        "pextlh $10, $10, $0\n\t"
        "pextlh $11, $11, $0\n\t"
        "psraw $8, $8, 16\n\t"
        "psraw $9, $9, 16\n\t"
        "psraw $10, $10, 16\n\t"
        "psraw $11, $11, 16\n\t"
        "qmtc2.ni $8, $vf1\n\t"
        "qmtc2.ni $9, $vf2\n\t"
        "qmtc2.ni $10, $vf3\n\t"
        "qmtc2.ni $11, $vf4\n\t"
        "vitof0.xyzw $vf1, $vf1\n\t"
        "vitof0.xyzw $vf2, $vf2\n\t"
        "vitof0.xyzw $vf3, $vf3\n\t"
        "vitof0.xyzw $vf4, $vf4\n\t"
        "sqc2 $vf1, 0x0($6)\n\t"
        "sqc2 $vf2, 0x10($6)\n\t"
        "addiu $4, $4, -0x10\n\t"
        "sqc2 $vf3, 0x20($6)\n\t"
        "addiu $5, $5, 0x20\n\t"
        "sqc2 $vf4, 0x30($6)\n\t"
        "bgtzl $4, 1b\n\t"
        "addiu $6, $6, 0x40\n\t"
        ".set reorder\n"
        : : "r"(n), "r"(src), "r"(dst)
        : "$4", "$5", "$6", "$8", "$9", "$10", "$11", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxaf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CEAXABLKDecf_Decode_9F58(int, char **, int) __asm__("CEAXABLKDecf_Decode");
extern void memset_9F58(char *, int, int) __asm__("memset");

/**
 * Decodes frames of a one-shot EA-XA ADPCM sample into a float buffer (a filter node's process function).
 *
 * Returns -1 once the position (+0x28) has reached the frame count (+0x24). Otherwise it decodes up to @p count frames
 * (decoder +0x1C; CEAXABLKDecf_Decode), advances the position and zero-fills what the sample could not supply.
 *
 * @param node  the decoder node
 * @param count  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buffer  the float destination
 * @return the frames written (always @p count), or -1 when the sample has ended
 */
int SFILTER_unpackxaf(UNPACKXAFSTATE* node, int count, int srcBuffer, int buffer) {
    char *cursor;
    char *zeroFrom;
    int n;
    cursor = (char *)buffer;
    if (node->curframe >= node->totalframes) {
        return -1;
    }
    n = CEAXABLKDecf_Decode_9F58((int)node->pDecoder, &cursor, count);
    node->curframe += n;
    cursor += n * 4;
    zeroFrom = cursor;
    if (n < count) {
        count = count - n;
        n += count;
        memset_9F58(zeroFrom, 0, count * 4);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackgetframexaf);
#ifdef SKIP_ASM
/**
 * Returns a one-shot EA-XA decoder's position (+0x28). SFILTER_unpackxafinit installs it as the voice's get-frame function.
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpackgetframexaf(UNPACKXAFSTATE* node) {
    return ((UNPACKXAFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxafrestore);
#ifdef SKIP_ASM
extern void CEAXABLKDecf_operator_delete(int);

/**
 * Release function of a one-shot EA-XA node: destroys its decoder (+0x1C) with CEAXABLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackxafrestore(UNPACKXAFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKXAFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CEAXABLKDecf_operator_delete(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxafinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char SFILTER_unpackxaf_A028[] __asm__("SFILTER_unpackxaf");
extern char SFILTER_unpackgetframexaf_A028[] __asm__("SFILTER_unpackgetframexaf");
extern char SFILTER_unpackxafrestore_A028[] __asm__("SFILTER_unpackxafrestore");
extern int CEAXABLKDecf_operator_new_A028(int) __asm__("CEAXABLKDecf_operator_new");
extern int CEAXABLKDecf_CEAXABLKDecf_A028(int) __asm__("CEAXABLKDecf_CEAXABLKDecf");
extern void CEAXABLKDecf_Feed_A028(int, int, int, int) __asm__("CEAXABLKDecf_Feed");

/**
 * Sets up a one-shot EA-XA ADPCM decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackxaf) and release (SFILTER_unpackxafrestore) functions, keeps the sample (init +0x00) and frame
 * count (+0x0C), creates a 168-byte decoder (CEAXABLKDecf_operator_new, CEAXABLKDecf_CEAXABLKDecf), registers SFILTER_unpackgetframexaf as get-frame (init
 * +0x28) and feeds the decoder the whole sample (CEAXABLKDecf_Feed).
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackxafinit(UNPACKXAFSTATE* node, UNPACKINITPARAMS* source) {
    int n;
    *(char**)&node->sfn = SFILTER_unpackxaf_A028;
    node->sfn.restorefn = (void (*)(void*))SFILTER_unpackxafrestore_A028;
    node->psample = (unsigned char*)((int)source->psample);
    node->totalframes = source->totalframes;
    node->curframe = 0;
    node->pDecoder = (struct CEAXABLKDecf*)CEAXABLKDecf_CEAXABLKDecf_A028(CEAXABLKDecf_operator_new_A028(168));
    source->unpackgetframe = (void*)SFILTER_unpackgetframexaf_A028;
    n = node->totalframes;
    CEAXABLKDecf_Feed_A028((int)node->pDecoder, (int)node->psample, n * 4, n);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxalf);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { float x, y; } V2_A0B8;
extern int CEAXABLKDecf_Decode_A0B8(int, int **, int) __asm__("CEAXABLKDecf_Decode");
extern int CEAXABLKDecf_Feed_A0B8(int, int, int, int) __asm__("CEAXABLKDecf_Feed");
extern V2_A0B8 CEAXABLKDecf_GetState_A0B8(int) __asm__("CEAXABLKDecf_GetState");
extern void CEAXABLKDecf_SetState_A0B8(int, V2_A0B8 *) __asm__("CEAXABLKDecf_SetState");

/**
 * Decodes frames of a looped EA-XA ADPCM sample into a float buffer (a filter node's process function).
 *
 * Codec versions below 3 (+0x30) work in 28-frame blocks of 15 bytes: the first call feeds the sample (+0x34) up to the
 * block before the sustain start (+0x44); whenever the decoder runs dry the position goes back to that block and the
 * blocks up to the sustain end (+0x48) are fed again. The first time round the decoder history is saved (+0x4C, +0x50);
 * later rounds restore it and decode the frames before the sustain start into a scratch buffer. Version 3 and up simply
 * refeed the loop data (+0x2C) from the sustain start (+0x24) to the end (+0x28).
 *
 * @param node  the decoder node
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buf  the float destination
 * @return the frames decoded
 */
int SFILTER_unpackxalf(UNPACKXALFSTATE* node, int n, int srcBuffer, int *buf) {
    int used = 0;
    int got;
    int *dst = buf;
    V2_A0B8 history;
    struct { int hdr[2]; int samples[30]; } scratch;
    int *scratchCursor;

    if (node->platformver < 3) {
        int k;
        if (node->feddatabeforess == 0) {
            k = node->sustainstart / 0x1C;
            k = k * 0x1C;
            CEAXABLKDecf_Feed_A0B8((int)node->pDecoder, (int)node->psample, k * 4, k);
            node->feddatabeforess = 1;
        }
        while (n != 0) {
            got = CEAXABLKDecf_Decode_A0B8((int)node->pDecoder, &dst, n);
            dst += got;
            used += got;
            node->curframe += got;
            if (got < n) {
                int rem;
                int cnt;
                n -= got;
                rem = node->sustainstart % 0x1C;
                node->curframe = node->sustainstart - rem;
                cnt = node->sustainend - node->curframe + 1;
                CEAXABLKDecf_Feed_A0B8((int)node->pDecoder, (int)node->psample + (node->curframe / 0x1C) * 15, cnt * 4, cnt);
                if (node->startblockmet == 0) {
                    history = CEAXABLKDecf_GetState_A0B8((int)node->pDecoder);
                    node->sustainstarts1 = history.x;
                    node->sustainstarts2 = history.y;
                    got = CEAXABLKDecf_Decode_A0B8((int)node->pDecoder, &dst, n);
                    dst += got;
                    node->curframe += got;
                    node->startblockmet = 1;
                    used += got;
                    n -= got;
                } else {
                    history.x = node->sustainstarts1;
                    history.y = node->sustainstarts2;
                    CEAXABLKDecf_SetState_A0B8((int)node->pDecoder, &history);
                    scratchCursor = scratch.samples;
                    got = CEAXABLKDecf_Decode_A0B8((int)node->pDecoder, &scratchCursor, rem);
                    node->curframe += got;
                }
            } else {
                n -= got;
            }
        }
        return used;
    }
    while (n > 0) {
        got = CEAXABLKDecf_Decode_A0B8((int)node->pDecoder, &dst, n);
        dst += got;
        n -= got;
        node->curframe += got;
        used += got;
        if (got < n) {
            int cnt = node->loopend - node->loopstart + 1;
            node->curframe = node->loopstart;
            CEAXABLKDecf_Feed_A0B8((int)node->pDecoder, (int)node->ploop, cnt * 4, cnt);
        }
    }
    return used;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackgetframexalf);
#ifdef SKIP_ASM
/**
 * Returns a looped EA-XA decoder's position (+0x20). SFILTER_unpackxalfinit installs it as the voice's get-frame function.
 *
 * @param node  the decoder node
 * @return the frame
 */
int SFILTER_unpackgetframexalf(UNPACKXALFSTATE* node) {
    return ((UNPACKXALFSTATE *)node)->curframe;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxalfrestore);
#ifdef SKIP_ASM
extern void CEAXABLKDecf_operator_delete(int);

/**
 * Release function of a looped EA-XA node: destroys its decoder (+0x1C) with CEAXABLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackxalfrestore(UNPACKXALFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKXALFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CEAXABLKDecf_operator_delete(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxalfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char SFILTER_unpackxalf_A388[] __asm__("SFILTER_unpackxalf");
extern char SFILTER_unpackgetframexalf_A388[] __asm__("SFILTER_unpackgetframexalf");
extern char SFILTER_unpackxalfrestore_A388[] __asm__("SFILTER_unpackxalfrestore");
extern int CEAXABLKDecf_operator_new_A388(int) __asm__("CEAXABLKDecf_operator_new");
extern int CEAXABLKDecf_CEAXABLKDecf_A388(int) __asm__("CEAXABLKDecf_CEAXABLKDecf");
extern void CEAXABLKDecf_Feed_A388(int, int, int, int) __asm__("CEAXABLKDecf_Feed");

/**
 * Sets up a looped EA-XA ADPCM decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackxalf) and release (SFILTER_unpackxalfrestore) functions, keeps the loop data (init +0x04), codec
 * version (+0x24), sustain start (+0x10) and end (+0x14), creates a 168-byte decoder (CEAXABLKDecf_operator_new, CEAXABLKDecf_CEAXABLKDecf) and
 * registers SFILTER_unpackgetframexalf as get-frame (init +0x28). Versions below 3 keep the sample and frame count for block-aligned
 * looping; later versions feed the sample up to the sustain start right away (CEAXABLKDecf_Feed).
 *
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackxalfinit(int *self, int *source) {
    int n;

    self[0] = (int)SFILTER_unpackxalf_A388;
    self[1] = (int)SFILTER_unpackxalfrestore_A388;
    self[11] = source[1];
    self[8] = 0;
    self[12] = source[9];
    self[9] = source[4];
    self[10] = source[5];
    self[7] = CEAXABLKDecf_CEAXABLKDecf_A388(CEAXABLKDecf_operator_new_A388(168));
    source[10] = (int)SFILTER_unpackgetframexalf_A388;
    if (self[12] < 3) {
        self[13] = source[0];
        self[14] = source[3];
        self[15] = 0;
        self[16] = 0;
        self[17] = source[4];
        self[18] = source[5];
    } else {
        n = self[9];
        CEAXABLKDecf_Feed_A388(self[7], source[0], n * 4, n);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxapf);
#ifdef SKIP_ASM
extern void SNDPKTPLAYI_freeframes_A460(int, int, int) __asm__("SNDPKTPLAYI_freeframes");
extern int CEAXABLKDecf_Decode_A460(void *, int **, int) __asm__("CEAXABLKDecf_Decode");
extern short *SNDPKTPLAYI_get_A460(int, int, int *, int *) __asm__("SNDPKTPLAYI_get");
extern void CEAXABLKDecf_SetState_A460(void *, float *) __asm__("CEAXABLKDecf_SetState");
extern void CEAXABLKDecf_Feed_A460(void *, short *, int, int) __asm__("CEAXABLKDecf_Feed");
extern void memset_A460(void *, int, int) __asm__("memset");

/**
 * Decodes EA-XA audio fed from a packet player into a float buffer (a filter node's process function).
 *
 * First reports the frames played since the last call (+0x34) to the player (SNDPKTPLAYI_freeframes; instance +0x30, channel
 * +0x38). Then decodes (+0x1C; CEAXABLKDecf_Decode); when the decoder runs dry it takes the channel's next packet
 * (SNDPKTPLAYI_get). For codec versions up to 2 (+0x2C) each packet starts with the two 16-bit history samples, loaded
 * with CEAXABLKDecf_SetState and skipped. Without a packet the rest is zero-filled once anything played, and decoding stops.
 *
 * @param n  the frames wanted
 * @param srcBuffer  unused (the source)
 * @param buf  the float destination
 * @return the frames decoded
 */
int SFILTER_unpackxapf(struct UNPACKXAPFSTATE *self, int n, int srcBuffer, int *buf) {
    int flag = 0;
    int done = 0;
    if (((int)self->prevframesunpacked)) {
        SNDPKTPLAYI_freeframes_A460(self->packetinstancehandle, self->samplechan, ((int)self->prevframesunpacked));
        self->prevframesunpacked = 0;
    }
    while (n > 0) {
        int k = CEAXABLKDecf_Decode_A460(((void*)self->pDecoder), &buf, n);
        self->curframe += k;
        buf += k;
        done += k;
        (*(int*)&self->prevframesunpacked) += k;
        if (k < n) {
            int continued;
            short *packetSamples;
            n -= k;
            packetSamples = SNDPKTPLAYI_get_A460(self->packetinstancehandle, self->samplechan, &self->totalframes, &continued);
            (*(short**)&self->psample) = packetSamples;
            if (packetSamples) {
                if (self->platformver <= 2) flag = 1;
                if (flag) {
                    float history[2];
                    history[0] = packetSamples[0];
                    history[1] = packetSamples[1];
                    CEAXABLKDecf_SetState_A460(((void*)self->pDecoder), history);
                    (*(short**)&self->psample) += 2;
                }
                self->curframe = 0;
                CEAXABLKDecf_Feed_A460(((void*)self->pDecoder), ((short*)self->psample), self->totalframes * 4, self->totalframes);
            } else {
                if (n > 0 && ((int)self->prevframesunpacked)) memset_A460(buf, 0, n * 4);
                n = 0;
            }
        } else {
            n -= k;
        }
    }
    return done;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxapfrestore);
#ifdef SKIP_ASM
extern void CEAXABLKDecf_operator_delete(int);

/**
 * Release function of a packet-fed EA-XA node: destroys its decoder (+0x1C) with CEAXABLKDecf_operator_delete.
 *
 * @param node  the decoder node
 */
void SFILTER_unpackxapfrestore(UNPACKXAPFSTATE* node) {
    int temp_4;

    temp_4 = (int)((UNPACKXAPFSTATE *)node)->pDecoder;
    if (temp_4 != 0) {
        CEAXABLKDecf_operator_delete(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_unpackxapfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CEAXABLKDecf_operator_new_A610(int) __asm__("CEAXABLKDecf_operator_new");
extern int CEAXABLKDecf_CEAXABLKDecf_A610(int) __asm__("CEAXABLKDecf_CEAXABLKDecf");
extern int SNDDRV_getmastervoice_A610(int) __asm__("SNDDRV_getmastervoice");
extern signed char SNDDRV_getsamplechan_A610(int) __asm__("SNDDRV_getsamplechan");
extern int SNDPKTPLAYI_voicetopackethandle_A610(int) __asm__("SNDPKTPLAYI_voicetopackethandle");
extern int SFILTER_unpackxapf_A610[] __asm__("SFILTER_unpackxapf");
extern int SFILTER_unpackxapfrestore_A610[] __asm__("SFILTER_unpackxapfrestore");

/**
 * Sets up a packet-fed EA-XA ADPCM decoder node (a filter factory).
 *
 * Installs the process (SFILTER_unpackxapf) and release (SFILTER_unpackxapfrestore) functions, finds the voice's packet player instance
 * (SNDPKTPLAYI_voicetopackethandle of SNDDRV_getmastervoice of the voice, init +0x1C) and channel (SNDDRV_getsamplechan), keeps the codec version (init
 * +0x24) and frame count (+0x0C) and creates a 168-byte decoder (CEAXABLKDecf_operator_new, CEAXABLKDecf_CEAXABLKDecf).
 *
 * @param node  the node
 * @param source  the init block from MIX_playinit
 */
void SFILTER_unpackxapfinit(UNPACKXAPFSTATE* node, UNPACKINITPARAMS* source) {
    (*(int**)&node->sfn) = SFILTER_unpackxapf_A610;
    ((int*)node->sfn.restorefn) = SFILTER_unpackxapfrestore_A610;
    node->packetinstancehandle = SNDPKTPLAYI_voicetopackethandle_A610(SNDDRV_getmastervoice_A610((source->voice)));
    (*(signed char*)&node->samplechan) = SNDDRV_getsamplechan_A610((source->voice));
    node->platformver = (int) (source->platformver);
    ((int)node->psample) = 0;
    node->totalframes = (int) (source->totalframes);
    node->curframe = 0;
    ((int)node->prevframesunpacked) = 0;
    ((int)node->pDecoder) = CEAXABLKDecf_CEAXABLKDecf_A610(CEAXABLKDecf_operator_new_A610(168));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_ft16);
#ifdef SKIP_ASM
int SFILTER_ft16_C(struct FT16STATE *self, int n, float *src, short *dst) __asm__("SFILTER_ft16");
/**
 * Converts a mixer output from floats to 16-bit samples (a filter node's process function).
 *
 * If the node has an input (+0x08) it first pulls the frames from it into @p src, passing on a result of 0 or less.
 * Then an MMI / VU0 loop clamps the floats to -32768..32767 and packs them, 16 at a time.
 *
 * @param n  the number of frames
 * @param src  the float samples
 * @param dst  the 16-bit destination
 * @return n, or the input's result when it is 0 or less
 */
// PORT: hand-written VU0 + MMI loop (ppach and trapping addi have no gcc 2.95 pattern; bgtzl with the pointer step in
// its slot; the block keeps its count/source/destination in fixed $4/$5/$6, gcc's own choice is the s-registers:
// 95.40): clamps n floats to [-32768, 32767], converts them to int and packs them to 16-bit samples, 16 per pass.
// A port replaces it with a C clamp-and-convert loop.
int SFILTER_ft16_C(struct FT16STATE *self, int n, float *src, short *dst) {
    if (((int*)self->sfn.pfnnext)) {
        int framesIn = ((int (*)(int *, int, short *, float *, int))*((int*)self->sfn.pfnnext))(((int*)self->sfn.pfnnext), n, dst, src, self->sfn.requester);
        if (framesIn <= 0) return framesIn;
    }
    // PORT: PS2-only VU0 hand-written loop (clamp floats to the 16-bit range and pack); one inline asm block.
    __asm__ volatile(
        ".set push\n\t"
        ".set noreorder\n\t"
        ".set noat\n\t"
        "move $4, %0\n\t"
        "move $5, %1\n\t"
        "move $6, %2\n\t"
        "addiu $8, $0, -0x8000\n\t"
        "addiu $9, $0, 0x7FFF\n\t"
        "qmtc2.ni $8, $vf1\n\t"
        "qmtc2.ni $9, $vf2\n\t"
        "vitof0.xyzw $vf1, $vf1\n\t"
        "vitof0.xyzw $vf2, $vf2\n"
        "1:\n\t"
        "lqc2 $vf3, 0x0($5)\n\t"
        "lqc2 $vf4, 0x10($5)\n\t"
        "lqc2 $vf5, 0x20($5)\n\t"
        "lqc2 $vf6, 0x30($5)\n\t"
        "vmaxx.xyzw $vf3, $vf3, $vf1x\n\t"
        "vmaxx.xyzw $vf4, $vf4, $vf1x\n\t"
        "vmaxx.xyzw $vf5, $vf5, $vf1x\n\t"
        "vmaxx.xyzw $vf6, $vf6, $vf1x\n\t"
        "vminix.xyzw $vf3, $vf3, $vf2x\n\t"
        "vminix.xyzw $vf4, $vf4, $vf2x\n\t"
        "vminix.xyzw $vf5, $vf5, $vf2x\n\t"
        "vminix.xyzw $vf6, $vf6, $vf2x\n\t"
        "vftoi0.xyzw $vf3, $vf3\n\t"
        "vftoi0.xyzw $vf4, $vf4\n\t"
        "vftoi0.xyzw $vf5, $vf5\n\t"
        "vftoi0.xyzw $vf6, $vf6\n\t"
        "addi $4, $4, -0x10\n\t"
        "addi $5, $5, 0x40\n\t"
        "qmfc2.ni $7, $vf3\n\t"
        "qmfc2.ni $8, $vf4\n\t"
        "qmfc2.ni $9, $vf5\n\t"
        "qmfc2.ni $10, $vf6\n\t"
        "ppach $7, $8, $7\n\t"
        "ppach $9, $10, $9\n\t"
        "sq $7, 0x0($6)\n\t"
        "sq $9, 0x10($6)\n\t"
        "bgtzl $4, 1b\n\t"
        "addi $6, $6, 0x20\n\t"
        ".set pop\n"
        : : "r"(n), "r"(src), "r"(dst)
        : "$4", "$5", "$6", "$7", "$8", "$9", "$10", "memory");
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_ft16init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_ft16_A7A0(void) __asm__("SFILTER_ft16");

/**
 * Sets up a float-to-16-bit conversion node: process function SFILTER_ft16, no release.
 *
 * MIX_create gives every mixer output one.
 *
 * @param node  the node
 */
void SFILTER_ft16init(FT16STATE* node) {
    node->sfn.restorefn = 0;
    node->sfn.filterfn = (int (*)(void *, int, void *, void *, int))SFILTER_ft16_A7A0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_lpfRC);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef int (*vfn_A7B8)(void *, int, float *, float *, unsigned char);
int SFILTER_lpfRC_A7B8(char *, int, float *, float *) __asm__("SFILTER_lpfRC");
/**
 * Runs a one-pole low-pass filter over frames (a filter node's process function).
 *
 * Pulls the input from the node feeding it (+0x08), if any, stopping on a result of 0 or less; then each output is
 * state = state * +0x20 + +0x24 * input (state at +0x1C).
 *
 * @param n  the number of frames
 * @param in  the input samples (filled by the feeding node)
 * @param out  the output samples
 * @return n, or the feeding node's result when it is 0 or less
 */
int SFILTER_lpfRC_A7B8(char *vself, int n, float *in, float *out) {
    LPFRCSTATE* self = (LPFRCSTATE*)vself;
    void **upstream = (void**)self->sfn.pfnnext;
    int result;
    float *st;
    int i;
    if (upstream == 0 || (result = ((vfn_A7B8)(*(void **)upstream))(upstream, n, out, in, self->sfn.requester)) > 0) {
        st = (float *)(&self->lpf);
        for (i = 0; i < n; i++) {
            st[0] = st[0] * st[1] + st[2] * in[i];
            out[i] = st[0];
        }
        result = n;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createLPFRC);
#ifdef SKIP_ASM
extern void SFILTER_lpfRC();

/**
 * Sets up a one-pole low-pass node: process function SFILTER_lpfRC, no links, no release, state cleared.
 *
 * Used by func_003CC070 for effect node type 1 and by func_003C8D00.
 *
 * @param node  the node
 * @return 0
 */
int SFILTER_createLPFRC(LPFRCSTATE* node) {
    node->sfn.pfnnext = 0;
    (*(void (**)())&node->sfn) = SFILTER_lpfRC;
    node->sfn.pfnnext2 = 0;
    node->sfn.pfnprev1 = 0;
    node->sfn.pfnprev2 = 0;
    (*(signed char*)&node->sfn.clean) = 0;
    node->sfn.restorefn = 0;
    (*(int*)&node->lpf) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyLPFRC);
#ifdef SKIP_ASM
/**
 * Sets a one-pole low-pass node's coefficients from its parameters.
 *
 * With k = 2 * cutoff / rate: input gain +0x24 = k * gain / 256 and feedback +0x20 = (1 - k) * gain / 256.
 *
 * @param node  the node
 * @param params  {cutoff, rate (both in the same fixed point), gain (256 = 1.0)}
 */
void SFILTER_modifyLPFRC(LPFRCSTATE* node, void *params) {
    float temp_f1;
    float temp_f2;

    temp_f1 = (2.0f * (float) (*(int *)((char*)(params) + (0)))) / (float) (*(int *)((char*)(params) + (4)));
    temp_f2 = 1.0f - temp_f1;
    node->lpf.inverse = temp_f1;
    node->lpf.leakage = temp_f2;
    node->lpf.inverse = (float) (temp_f1 * ((float) (*(int *)((char*)(params) + (8))) * 0.00390625f));
    node->lpf.leakage = (float) (temp_f2 * ((float) (*(int *)((char*)(params) + (8))) * 0.00390625f));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_hpfFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void SNDI_fir8_A900(void *, int, int, int) __asm__("SNDI_fir8");

/**
 * Runs the high-pass FIR filter over frames (a filter node's process function).
 *
 * Pulls the input from the node feeding it (+0x08), if any, stopping on a result of 0 or less, then filters with the
 * FIR state at +0x20 (SNDI_fir8).
 *
 * @param node  the filter node
 * @param count  the number of frames
 * @param src  the input samples (filled by the feeding node)
 * @param dst  the output samples
 * @return the frames filtered, or the feeding node's result when it is 0 or less
 */
int SFILTER_hpfFIR8(HPFSTATE* node, int count, int src, int dst) {
    int (**obj)(void *, int, int, int, int);
    int n;
    obj = (int (**)(void*, int, int, int, int))node->sfn.pfnnext;
    if (obj != 0) {
        n = (*obj)(obj, count, dst, src, node->sfn.requester);
        if (n <= 0) {
            return n;
        }
    } else {
        n = count;
    }
    SNDI_fir8_A900(&node->fir, n, src, dst);
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createHPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_A990;
extern void SNDI_fir8init(SNDFIRSTATE*);
extern void SFILTER_hpfFIR8_x(void) __asm__("SFILTER_hpfFIR8");

/**
 * Sets up a high-pass FIR node: process function SFILTER_hpfFIR8, no links, no release, FIR state at +0x20 initialised by
 * SNDI_fir8init.
 *
 * Used by MIX_sethighpass and by func_003CC070 for effect node type 3.
 *
 * @return 0
 */
int SFILTER_createHPFFIR8(S_A990 *self) {
    self->vt = SFILTER_hpfFIR8_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    SNDI_fir8init((SNDFIRSTATE*)(&self->sub));
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyHPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Sets the ratio at 0x114 from two fixed-point values, then calls calcFIRCoeffs with 3.
extern void calcFIRCoeffs_A9D8(void *, int) __asm__("calcFIRCoeffs");
/**
 * Sets a high-pass FIR node's cutoff: stores the ratio (params[0] >> 7) / (params[1] >> 8), twice cutoff over rate, at
 * +0x114 and recomputes the coefficients of the FIR state (calcFIRCoeffs, filter type 3).
 *
 * @param params  {cutoff << 8, rate << 8}
 */
void SFILTER_modifyHPFFIR8(HPFSTATE* self, int *params) {
    self->fir.cutLow = (float)(params[0] >> 7) / (float)(params[1] >> 8);
    calcFIRCoeffs_A9D8(&self->fir, 3);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", crossfade);
#ifdef SKIP_ASM
/**
 * Cross-fades a window of samples with itself shifted by k frames, the splice of the time-stretcher.
 *
 * For a positive k the weight runs from 1 to 0 over the window, blending cur[i] with cur[i - k] and, for the last k
 * frames, the next window with the current one; a negative k fades the other way. Called by applyaction.
 *
 * @param curBuf  the current window
 * @param nextBuf  the following window
 * @param out  the destination
 * @param n  the window size
 * @param k  the shift in frames
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void crossfade(float *curBuf, float *nextBuf, float *out, int n, int k) {
    float weight, step;
    int i;
    if (k > 0) {
        step = -1.0f / n;
        weight = 1.0f;
    } else {
        k = -k;
        step = 1.0f / n;
        weight = 0.0f;
    }
    for (i = k; i < n; i++) {
        out[i - k] = weight * curBuf[i] + (1.0f - weight) * curBuf[i - k];
        weight += step;
    }
    for (i = 0; i < k; i++) {
        out[n - k + i] = weight * nextBuf[i] + (1.0f - weight) * curBuf[n - k + i];
        weight += step;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", determineaction);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#define FABS_AB28(x) ((x) < 0.0f ? -(x) : (x))
/**
 * Decides the time-stretcher's next action for one window: keep it, or splice by a shift.
 *
 * The side-band byte at *pp plus half the window gives the shift. Slowing down (ratio below 1) compares the error of
 * keeping the window with that of dropping a shifted window; speeding up compares it with repeating a shifted part. The
 * smaller error wins: the error is carried in *err and the side-band pointer advances (by 2 when a window is dropped).
 *
 * @param n  the window size
 * @param pp  in/out: the side-band data
 * @param err  in/out: the accumulated timing error
 * @param timeRatio  the stretch ratio
 * @return 0 to keep the window, otherwise the shift (negative when slowing down)
 */
int determineaction(int n, unsigned char **pp, float *err, float timeRatio) {
    int k = **pp + n / 2;
    float e1, e2;
    if (timeRatio < 1.0f) {
        e1 = ((float)n + *err) - timeRatio * n;
        e2 = ((float)(n * 2 - k) + *err) - (timeRatio + timeRatio) * n;
        if (FABS_AB28(e1) < FABS_AB28(e2)) {
            *pp += 1;
            *err = e1;
            return 0;
        }
        *pp += 2;
        *err = e2;
        return -k;
    } else {
        float tn = timeRatio * n;
        e1 = ((float)n + *err) - tn;
        e2 = ((float)(n + k) + *err) - tn;
        if (FABS_AB28(e1) < FABS_AB28(e2)) {
            *pp += 1;
            *err = e1;
            return 0;
        }
        *pp += 1;
        *err = e2;
        return k;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", applyaction);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void Util_MemCpy_AC88(void *, void *, int) __asm__("Util_MemCpy");
extern void crossfade_AC88(void *, void *, void *, int, int) __asm__("crossfade");

/**
 * Applies the time-stretcher's action to the next window of input.
 *
 * With no shift the window is copied to the output buffer (+0x838); a positive shift copies part of it and cross-fades
 * the rest with the next window (crossfade); a negative shift cross-fades and then copies the extra frames. The
 * input left (+0x30), the output frames ready (+0x38) and the input cursor are updated; the pending-window count +0x34
 * and the output read index +0x3C are reset.
 *
 * @param stretcher  the time-stretch state
 * @param cursor  in/out: the input samples
 * @param shift  the shift from determineaction (0 keeps the window)
 */
void applyaction(TIMESTRETCHSTATE* stretcher, char **cursor, int shift) {
    char *prevBlock;
    char *nextBlock;
    if (stretcher->inputsamples > 0) {
        prevBlock = (char*)&stretcher->input[0];
        nextBlock = *cursor;
    } else {
        prevBlock = *cursor;
        nextBlock = prevBlock + stretcher->windowsize * 4;
    }
    if (shift == 0) {
        Util_MemCpy_AC88(&stretcher->output[0], prevBlock, stretcher->windowsize * 4);
        stretcher->framesleft = stretcher->framesleft - stretcher->windowsize;
        stretcher->outputsamples = stretcher->windowsize;
        *cursor = nextBlock;
    } else {
        if (shift > 0) {
            int shiftBytes = shift * 4;
            Util_MemCpy_AC88(&stretcher->output[0], prevBlock, shiftBytes);
            crossfade_AC88(prevBlock, nextBlock, ((char*)stretcher) + (shiftBytes + 2104), stretcher->windowsize, shift);
            stretcher->framesleft = stretcher->framesleft - stretcher->windowsize;
            stretcher->outputsamples = stretcher->windowsize + shift;
            *cursor = nextBlock;
        } else {
            int blockLen, consumed, n;
            crossfade_AC88(prevBlock, nextBlock, &stretcher->output[0], stretcher->windowsize, shift);
            blockLen = stretcher->windowsize;
            Util_MemCpy_AC88(&stretcher->output[blockLen], nextBlock - shift * 4, (blockLen + shift) * 4);
            n = stretcher->windowsize;
            consumed = n * 2;
            stretcher->framesleft = stretcher->framesleft - consumed;
            stretcher->outputsamples = consumed + shift;
            *cursor = nextBlock + n * 4;
        }
    }
    stretcher->inputsamples = 0;
    stretcher->outputstart = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", transferframes);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void Util_MemCpy_ADF0(int, void *, int) __asm__("Util_MemCpy");

/**
 * Copies stretched frames that are ready (+0x38, from read index +0x3C of the buffer at +0x838) to the output.
 *
 * Copies at most *remaining frames, then advances the output cursor and lowers both counts.
 *
 * @param stretcher  the time-stretch state
 * @param remaining  in/out: the frames still wanted
 * @param outCursor  in/out: the output position
 * @return the frames copied
 */
int transferframes(TIMESTRETCHSTATE* stretcher, int *remaining, int *outCursor) {
    int copied;
    int temp_16_2;
    int byteCount;
    int temp_3;

    temp_16_2 = (stretcher->outputsamples);
    temp_3 = *remaining;
    copied = temp_16_2;
    if (temp_16_2 >= temp_3) copied = temp_3;
    byteCount = copied * 4;
    Util_MemCpy_ADF0(*outCursor, &stretcher->output[stretcher->outputstart], byteCount);
    stretcher->outputstart = (int) ((stretcher->outputstart) + copied);
    stretcher->outputsamples = (int) ((stretcher->outputsamples) - copied);
    *remaining -= copied;
    *outCursor += byteCount;
    return copied;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", stretchframesneeded);
#ifdef SKIP_ASM
extern int determineaction_AEA0(float, int, int *, float *) __asm__("determineaction");
/**
 * Returns how many more input frames the time-stretcher needs to produce a number of output frames.
 *
 * Beyond the frames already ready (+0x38), it simulates determineaction's decisions on copies of the side-band position
 * and error, counting the window steps the available input (+0x30) allows, and returns the shortfall over the buffered
 * input (+0x34), if any.
 *
 * @param stretchState  the time-stretch state
 * @param n  the output frames wanted
 * @return the input frames to fetch, or 0
 */
int stretchframesneeded(struct TIMESTRETCHSTATE *stretchState, int n) {
    int pos;
    float err;
    int tot;
    int shift;
    int avail;
    int step;
    int used;
    if (stretchState->outputsamples < n) {
        n -= stretchState->outputsamples;
        pos = ((int)stretchState->psideband);
        err = stretchState->sampleerror;
        tot = 0;
        shift = 0;
        avail = stretchState->framesleft;
        while (n > 0) {
            int lo = stretchState->windowsize;
            if (avail < lo) break;
            shift = 0;
            if (avail >= stretchState->windowsize * 2) shift = determineaction_AEA0(stretchState->stretchratio, stretchState->windowsize, &pos, &err);
            if (shift >= 0) { step = stretchState->windowsize; used = step + shift; } else { step = stretchState->windowsize * 2; used = step + shift; }
            tot += step;
            avail -= step;
            if (used >= n) n = 0; else n -= used;
        }
        if (shift > 0) tot += stretchState->windowsize;
        if (n > 0) {
            if (avail < n) tot += avail; else tot += n;
        }
        if (stretchState->inputsamples < tot) return tot - stretchState->inputsamples;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", stretch);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int fADF0_AFD0(void *, int *, int *) __asm__("transferframes");
extern void fAC88_AFD0(void *, char **, long) __asm__("applyaction");
extern long fAB28_AFD0(float, int, void *, void *) __asm__("determineaction");
extern void f51E8_AFD0(void *, void *, int) __asm__("Util_MemCpy");

/**
 * Produces time-scaled output frames from input frames, at the ratio +0x24.
 *
 * Hands out ready frames (transferframes), then while input remains for a window decides an action (determineaction, when
 * two windows are available), applies it (applyaction) and hands out again. A positive last shift keeps one window
 * back (+0x40); leftover requests are served straight from the input.
 *
 * @param cnt  the output frames wanted
 * @param buf  the input samples
 * @param dst  the output samples
 * @return the frames produced
 */
int stretch(TIMESTRETCHSTATE* self, int cnt, char *buf, char *dst) {
    int total = fADF0_AFD0(self, &cnt, (int *)&dst);
    long shift;
    if (cnt == 0) return total;
    shift = 0;
    while (cnt > 0) {
        int hi = self->framesleft;
        int lo = self->windowsize;
        if (hi < lo)
            break;
        shift = 0;
        if (self->framesleft >= self->windowsize * 2)
            shift = fAB28_AFD0(self->stretchratio, self->windowsize, &self->psideband, &self->sampleerror);
        fAC88_AFD0(self, &buf, shift);
        total += fADF0_AFD0(self, &cnt, (int *)&dst);
    }
    if (shift > 0) {
        f51E8_AFD0(&self->input[0], buf, self->windowsize * 4);
        self->inputsamples = self->windowsize;
        buf += self->windowsize * 4;
    }
    if (cnt > 0) {
        int n = self->framesleft;
        if (n >= cnt)
            n = cnt;
        total += n;
        f51E8_AFD0(dst, buf, n * 4);
        self->framesleft -= n;
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_timestretchsetratio);
#ifdef SKIP_ASM
extern long fptodp_B110(float) __asm__("fptodp");
extern int dpcmp_B110(long, long) __asm__("dpcmp");
/**
 * Sets the time-stretch ratio: timeMult / 4096, clamped to 0.5 .. 2.0 (+0x24). Called by MIX_settimemult.
 *
 * @param stretcher  the time-stretch state
 * @param timeMult  the time multiplier (4096 = 1.0)
 */
void SFILTER_timestretchsetratio(struct TIMESTRETCHSTATE *stretcher, int timeMult) {
    stretcher->stretchratio = timeMult * (1.0f / 4096.0f);
    if (dpcmp_B110(fptodp_B110(stretcher->stretchratio), 0x3FE0000000000000L) < 0) {
        stretcher->stretchratio = 0.5f;
    } else if (dpcmp_B110(fptodp_B110(stretcher->stretchratio), 0x4000000000000000L) > 0) {
        stretcher->stretchratio = 2.0f;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_timestretch);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct O_B1A8 { int (*fn)(struct O_B1A8 *, int, int *, int, int); };
extern int stretchframesneeded_B1A8(struct TIMESTRETCHSTATE *, int) __asm__("stretchframesneeded");
extern int stretch_B1A8(struct TIMESTRETCHSTATE *, int, int, int *) __asm__("stretch");
extern int SNDPKTPLAY_framesoutstanding_B1A8(int) __asm__("SNDPKTPLAY_framesoutstanding");
extern void memset_B1A8(void *, int, int) __asm__("memset");

/**
 * Time-stretches a voice's audio (a filter node's process function).
 *
 * Works out the input it needs (stretchframesneeded). For packet-fed voices (+0x20 not negative) it waits, returning 0, until
 * the packet player holds that many frames (SNDPKTPLAY_framesoutstanding). It pulls that input from the feeding node, passing on a
 * result of 0 or less, stretches it (stretch) and zero-fills any shortfall.
 *
 * @param n  the output frames wanted
 * @param srcBuffer  the buffer for the input
 * @param buf  the output samples
 * @return n, 0 while waiting for packets, or the feeding node's result when it is 0 or less
 */
int SFILTER_timestretch(struct TIMESTRETCHSTATE *self, int n, int srcBuffer, int *buf) {
    int avail = stretchframesneeded_B1A8(self, n);
    int framesDone;
    if (self->packetinstancehandle >= 0) {
        if (SNDPKTPLAY_framesoutstanding_B1A8(self->packetinstancehandle) < avail) return 0;
    }
    if (avail > 0) {
        framesDone = ((struct O_B1A8*)self->sfn.pfnnext)->fn(((struct O_B1A8*)self->sfn.pfnnext), avail, buf, srcBuffer, self->sfn.requester);
        if (framesDone <= 0) return framesDone;
    }
    framesDone = stretch_B1A8(self, n, srcBuffer, buf);
    if (framesDone < n) memset_B1A8(buf + framesDone, 0, (n - framesDone) * 4);
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_timestretchinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDI_getb(int, int);
extern int SNDDRV_getmastervoice(int);
extern int SNDPKTPLAYI_voicetopackethandle(int);
extern char SFILTER_timestretch_B280[] __asm__("SFILTER_timestretch");

/**
 * Sets up a time-stretch node from its side-band data.
 *
 * Installs the process function SFILTER_timestretch; for a voice (not negative) it binds the voice's packet player
 * (SNDPKTPLAYI_voicetopackethandle of SNDDRV_getmastervoice), otherwise -1. The side-band entries start at +6; the window is twice the header's
 * byte +1 and the input total the 4-byte value at +2 (SNDI_getb). Ratio 1.0, error and counters cleared.
 *
 * @param node  the time-stretch node
 * @param header  the side-band data
 * @param voice  the voice, or -1
 * @return 0
 */
int SFILTER_timestretchinit(TIMESTRETCHSTATE* node, void *header, int voice) {
    (*(char**)&node->sfn) = SFILTER_timestretch_B280;
    ((int)node->sfn.restorefn) = 0;
    if (voice >= 0) {
        node->packetinstancehandle = SNDPKTPLAYI_voicetopackethandle(SNDDRV_getmastervoice(voice));
    } else {
        node->packetinstancehandle = -1;
    }
    ((int)node->psideband) = (int) (header + 6);
    node->stretchratio = 1.0f;
    (*(int*)&node->sampleerror) = 0;
    node->windowsize = (int) ((*(unsigned char *)((char*)(header) + (1))) * 2);
    node->framesleft = SNDI_getb(header + 2, 4);
    node->inputsamples = 0;
    node->outputsamples = 0;
    node->outputstart = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CB328);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct O_B328 {
    char pad0[8]; int (**f8)(); char padC[0x1A - 0xC]; unsigned char b1A; char pad1B[0x20 - 0x1B];
    short s20; char pad22[0x30 - 0x22]; short s30; char pad32[0x40 - 0x32]; short s40;
};

/**
 * Clips samples and scales them back up (a filter node's process function, set up by func_003CB528).
 *
 * Pulls the input from the node feeding it (+0x08), if any, passing on a result of 0 or less. Each 16-bit sample is
 * clamped to the node's range (+0x20 .. +0x30) and multiplied by +0x40 >> 7, four at a time.
 *
 * @param clipper  the filter node
 * @param n  the number of samples (a multiple of 4)
 * @param src  the input samples
 * @param dst  the output samples
 * @return n, or the feeding node's result when it is 0 or less
 */
int func_003CB328(struct O_B328 *clipper, int n, short *src, short *dst) {
    int i;
    short *sp;
    short *dp;
    int framesIn;
    if (clipper->f8) {
        framesIn = (*clipper->f8)(clipper->f8, n, dst, src, clipper->b1A);
        if (framesIn <= 0) {
            return framesIn;
        }
    }
    sp = src;
    dp = dst;
    for (i = 0; i < n; i += 4) {
        int sample0, sample1, sample2, sample3; short clipped0, clipped1, clipped2, clipped3;
        sample0 = sp[0];
        if (sample0 > clipper->s30) clipped0 = clipper->s30; else if (sample0 < clipper->s20) clipped0 = clipper->s20; else clipped0 = sample0;
        sample1 = sp[1];
        if (sample1 > clipper->s30) clipped1 = clipper->s30; else if (sample1 < clipper->s20) clipped1 = clipper->s20; else clipped1 = sample1;
        sample2 = sp[2];
        if (sample2 > clipper->s30) clipped2 = clipper->s30; else if (sample2 < clipper->s20) clipped2 = clipper->s20; else clipped2 = sample2;
        sample3 = sp[3];
        if (sample3 > clipper->s30) clipped3 = clipper->s30; else if (sample3 < clipper->s20) clipped3 = clipper->s20; else clipped3 = sample3;
        dp[0] = (clipped0 * clipper->s40) >> 7;
        dp[1] = (clipped1 * clipper->s40) >> 7;
        dp[2] = (clipped2 * clipper->s40) >> 7;
        dp[3] = (clipped3 * clipper->s40) >> 7;
        sp += 4;
        dp += 4;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CB4A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct P { char pad[0x20]; short A[8]; short B[8]; short C[8]; };
/**
 * Sets the clipping filter's range from a level: the bounds become -(32768 - t) and 32767 - t with t = x * 32000 >> 15,
 * and the gain 0x3FDE00 / (32767 - t), so the clipped signal is brought back to full scale.
 *
 * Each value is stored eight times (+0x20, +0x30, +0x40). Called by func_003C8B80.
 *
 * @param filter  the filter node
 * @param x  the level (0 to 0x8000)
 */
void func_003CB4A8(struct P* filter, int x)
{
    int clipLevel = (x * 32000) >> 15;
    int ceiling = 0x7FFF - clipLevel;
    int i;
    filter->A[0] = clipLevel - 0x8000;
    filter->B[0] = ceiling;
    filter->C[0] = 0x3FDE00 / (short)ceiling;
    for (i = 1; i < 8; i++) {
        filter->A[i] = filter->A[0];
        filter->B[i] = filter->B[0];
        filter->C[i] = filter->C[0];
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CB528);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_003CB328_B528(void) __asm__("func_003CB328");

/**
 * Sets up a mixer voice's clipping filter node: its process function becomes func_003CB328.
 *
 * Stores func_003CB328 in the node's first word, the slot SFILTER nodes call to process samples. func_003C8B80 calls it
 * on a newly allocated 80-byte node before linking it with SFILTER_add.
 *
 * @param node  the filter node
 */
void func_003CB528(void (**node)(void)) {
    *node = func_003CB328_B528;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_rsfsetpitch);
#ifdef SKIP_ASM
/**
 * Sets a resampling filter's increment (+0x1C), the pitch in 16.16. Called by MIX_setpitch.
 *
 * @param node  the resampling node
 * @param value  the increment
 */
void SFILTER_rsfsetpitch(RSFSTATE* node, int value) {
    node->incr16_16 = value;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CB540);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_rsfinit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_BB28_vt(void) __asm__("func_003CB540");

/**
 * Sets up a resampling filter node: process function func_003CB540, no release, position and history cleared.
 *
 * Called by MIX_setpitch (which passes byte D_0050AA62 and the quality flag; neither is stored here).
 */
void SFILTER_rsfinit(int *self) {
    int i;
    self[1] = 0;
    self[8] = 0;
    self[0] = (int)f_BB28_vt;
    ((short *)self)[0x24 / 2] = 0;
    ((short *)self)[0x26 / 2] = 0;
    for (i = 3; i >= 0; i--)
        self[10 + i] = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBB78);
#ifdef SKIP_ASM
/**
 * Adds a block of float samples, scaled, into another: dst[k] += src[k] * scale (MMI / VU0 loop, 16 at a time).
 *
 * MIX_reset installs it as the mixer's mix function (+0x1E0 of sndmix).
 *
 * @param n  the number of samples
 * @param src  the source samples
 * @param dst  the destination samples
 * @param scale  the gain
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: hand-written VU0 loop dst[k] += src[k] * scale over n floats, 16 per pass (trapping add/addi have no gcc 2.95
// pattern; the block moves its operands into fixed $4/$6/$7 itself: retail's double copies `daddu $3,$4` then
// `daddu $4,$3` are gcc moving n out of the clobbered $4 for the asm's own `move $4, %0`). A port replaces it with
// the C loop.
void func_003CBB78(int n, float *src, float *dst, float scale) {
    __asm__ volatile(
        ".set noreorder\n"
        "mfc1 $8, %3\n"
        "move $4, %0\n"
        "qmtc2.ni $8, $vf1\n"
        "sll $4, $4, 2\n"
        "move $6, %1\n"
        "move $7, %2\n"
        "add $4, $4, $6\n"
        "1:\n"
        "lqc2 $vf2, 0x0($6)\n"
        "lqc2 $vf3, 0x10($6)\n"
        "lqc2 $vf4, 0x20($6)\n"
        "lqc2 $vf5, 0x30($6)\n"
        "lqc2 $vf6, 0x0($7)\n"
        "lqc2 $vf7, 0x10($7)\n"
        "lqc2 $vf8, 0x20($7)\n"
        "lqc2 $vf9, 0x30($7)\n"
        "vmulx.xyzw $vf2, $vf2, $vf1x\n"
        "vmulx.xyzw $vf3, $vf3, $vf1x\n"
        "vmulx.xyzw $vf4, $vf4, $vf1x\n"
        "vmulx.xyzw $vf5, $vf5, $vf1x\n"
        "vadd.xyzw $vf6, $vf6, $vf2\n"
        "vadd.xyzw $vf7, $vf7, $vf3\n"
        "addi $6, $6, 0x40\n"
        "vadd.xyzw $vf8, $vf8, $vf4\n"
        "vadd.xyzw $vf9, $vf9, $vf5\n"
        "sqc2 $vf6, 0x0($7)\n"
        "sqc2 $vf7, 0x10($7)\n"
        "sqc2 $vf8, 0x20($7)\n"
        "sqc2 $vf9, 0x30($7)\n"
        "bne $4, $6, 1b\n"
        "addi $7, $7, 0x40\n"
        ".set reorder\n"
        : : "r"(n), "r"(src), "r"(dst), "f"(scale)
        : "$4", "$6", "$7", "$8", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Sets where the effect send is delivered (D_00515B54, +0x14 of the mixer state): func_003CC878 converts the send
 * buffer into it and func_003CBC58 copies into it instead of running the effect.
 *
 * func_003C1638 sets it and SNDPLATFORM_restore clears it at shutdown.
 *
 * @param sendOutput  the destination, or 0
 */
void func_003CBC00(int sendOutput) {
    *(int*)&sndmix.wetbusactive = sendOutput;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_00515D80_BC10[4] __asm__("D_00515D80");

/**
 * Returns the mixer's effect send buffer (D_00515D80). No callers.
 *
 * @return the buffer
 */
int *func_003CBC10(void) {
    return D_00515D80_BC10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044E770_BC20[][6][6] __asm__("D_0044E770");

/**
 * Looks up which effect output feeds a speaker: entry [effect layout - 1][mixer outputs - 1][code - 1] of the 6 x 6 x 6
 * table D_0044E770.
 *
 * Called by func_003CC070 when it wires an effect's output nodes.
 *
 * @param effectChannels  the channel count the effect was made for
 * @param mixerChannels  the mixer's output count
 * @param channelCode  the node's channel code (1-6)
 * @return the output slot
 */
unsigned char func_003CBC20(int effectChannels, int mixerChannels, int channelCode) {
    return D_0044E770_BC20[effectChannels - 1][mixerChannels - 1][channelCode - 1];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CBC58);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Q_BC58 { int x; int arr[1]; };
struct Obj_BC58 { int (*fn)(struct Obj_BC58 *, int, int, int, int); };
extern char D_00515D80_BC58[] __asm__("D_00515D80");
extern int D_00515D40_BC58 __asm__("D_00515D40");
extern int *D_00515D44_BC58 __asm__("D_00515D44");
extern struct Obj_BC58 **D_00515D48_BC58 __asm__("D_00515D48");
extern int D_00515D58_BC58 __asm__("D_00515D58");
extern int D_00517600_BC58 __asm__("D_00517600");
extern signed char D_0044E164_BC58[][6][6][6] __asm__("D_0044E164");
extern void *f_6210_BC58(void *, int, int) __asm__("memset");
extern void f_51E8_BC58(int, void *, int) __asm__("Util_MemCpy");
extern int f_72F8_BC58(int, int) __asm__("SNDI_getb");
extern void f_F798_BC58(int, void *) __asm__("SFILTER_initSOURCE");

/**
 * Runs the software effect after each mix slice (installed as the mixer's output hook by func_003CC630).
 *
 * When a send destination is set (D_00515B54) the send buffer is just copied there and cleared. Otherwise the effect
 * stops after 3000 slices without send input (the counter at +0x1800 of the send buffer, cleared by MIX_audioslice).
 * When the layout has nodes, the send buffer feeds the graph (SFILTER_initSOURCE) and each output node is pulled and mixed
 * into its mixer output and, per the routing table D_0044E164, into the others it covers. The send buffer is then
 * cleared.
 *
 * @param n  the frames in the slice
 */
void func_003CBC58(int n) {
    int done[6];
    int scratchBuffer, wetBuffer, i, j;
    struct Obj_BC58 *obj;
    signed char *routeEntry;
    signed char *routeTable;
    char *buf;

    f_6210_BC58(done, 0, 0x18);
    if (((int)sndmix.wetbusactive) != 0) {
        f_51E8_BC58(((int)sndmix.wetbusactive), D_00515D80_BC58, n * 4);
        f_6210_BC58(D_00515D80_BC58, 0, n * 4);
        return;
    }
    buf = D_00515D80_BC58;
    scratchBuffer = ((int)sndmix.ptempbuf[0]);
    wetBuffer = ((int)sndmix.ptempbuf[1]);
    if (++*(int *)(buf + 0x1800) >= 3000) {
        return;
    }
    if (f_72F8_BC58((int)&((struct Q_BC58 *)D_00515D40_BC58)->arr[D_00515D58_BC58], 4) != 0) {
        f_F798_BC58(D_00515D44_BC58[D_00517600_BC58], buf);
        for (i = 0; i < sndmix.mc.outputchannels; i++) {
            if (done[i] != 0) continue;
            obj = D_00515D48_BC58[i + 1];
            if (obj == 0) continue;
            obj->fn(obj, n, scratchBuffer, wetBuffer, i + 1);
            ((void (*)(int, int, int, float))sndmix.mix)(n, wetBuffer, (int)sndmix.pmixbuf[i], 1.0f);
            done[i] = 1;
            routeTable = (signed char *)D_0044E164_BC58[D_00515D58_BC58];
            routeEntry = routeTable + sndmix.mc.outputchannels * 36 + i * 6;
            for (j = 0; j < sndmix.mc.outputchannels; j++, routeEntry++) {
                if (*routeEntry >= 0) {
                    ((void (*)(int, int, int, float))sndmix.mix)(n, wetBuffer, (int)sndmix.pmixbuf[*routeEntry], 1.0f);
                    done[*routeEntry] = 1;
                }
            }
        }
    }
    f_6210_BC58(D_00515D80_BC58, 0, n * 4);
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CBEC8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CC070);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Hdr_C070 {
    unsigned char b0;
    unsigned char b1;
    unsigned char ver;
    unsigned char b3;
    int f4;
    int cnt[6];
    int len[6];
};
extern struct Hdr_C070 *D_00515D40;
extern void **D_00515D44;
extern void **D_00515D48;
extern unsigned char *D_00515D4C;
extern unsigned char *D_00515D50;
extern int D_00515D58;
extern int D_00515D80_C070[] __asm__("D_00515D80");
extern struct Hdr_C070 D_005175C0_C070 __asm__("D_005175C0");
extern unsigned char *D_005175F8;
extern unsigned char *D_005175FC;
extern int D_00517600;
extern int D_0044E848[];
extern int D_0044E870_C070[] __asm__("D_0044E870");
extern void *f_5C60_C070(int) __asm__("SNDMEMI_alloc");
extern void *memset_C070(void *, int, unsigned int) __asm__("memset");
extern int f_72F8_C070(void *, int) __asm__("SNDI_getb");
extern int f_EE88_C070(void *) __asm__("SFILTER_createLPFFIR8");
extern void f_EED0_C070(void *, int *) __asm__("SFILTER_modifyLPFFIR8");
extern void f_A9D8_C070(void *, int *) __asm__("SFILTER_modifyHPFFIR8");
extern int f_EFB0_C070(void *) __asm__("SFILTER_createBPFFIR8");
extern void f_EFF8_C070(void *, int *) __asm__("SFILTER_modifyBPFFIR8");
extern int f_F128_C070(void *) __asm__("SFILTER_createAMPLF");
extern void f_F158_C070(void *, int *) __asm__("SFILTER_modifyAMPLF");
extern int f_F2C0_C070(void *) __asm__("SFILTER_createMIX");
extern int f_F498_C070(void *) __asm__("SFILTER_createRESON");
extern void f_F4D0_C070(void *, int *) __asm__("SFILTER_modifyRESON");
extern int f_F6F8_C070(void *) __asm__("SFILTER_createSPLIT");
extern int f_F7B0_C070(void *) __asm__("SFILTER_createSOURCE");
extern int f_F998_C070(void *) __asm__("SFILTER_createECHO");
extern void f_F9D8_C070(void *, int *) __asm__("SFILTER_modifyECHO");

/**
 * Builds the software effect's filter graph from its description (D_005175C0, read by func_003CC630).
 *
 * Allocates the node list and output slots (D_00515D44, D_00515D48), then for each node record of the chosen layout
 * creates the node by type (0-9; type 1 a one-pole low-pass, type 3 a high-pass FIR, the others set up by SFILTER_createLPFFIR8
 * .. SFILTER_createECHO) with its parameters at the output rate, and maps nodes with a channel code to their output slot
 * (func_003CBC20). Then it connects the links (12-byte records: source, destination, slots; SFILTER_connect), primes the
 * send buffer's idle counter at 3000, clears the buffer and counts the nodes (D_00517600).
 *
 * @param rate  the output sample rate
 */
void func_003CC070(int rate) {
    int i;
    int last;
    int count;
    unsigned char *nodeRecord;
    unsigned char *link;
    void *obj;
    int params[4];

    D_00515D40 = &D_005175C0_C070;
    D_00515D48 = f_5C60_C070(0x1C);
    memset_C070(D_00515D48, 0, 0x1C);
    last = D_00515D58 - 1;
    D_00515D4C = D_005175F8;
    D_00515D50 = D_005175FC;
    count = f_72F8_C070(&D_00515D40->cnt[last], 4);
    D_00515D44 = f_5C60_C070(count * 4);
    nodeRecord = D_00515D4C;
    for (i = 0; i < count; i++) {
        D_00515D44[i] = f_5C60_C070(D_0044E848[*nodeRecord]);
        memset_C070(D_00515D44[i], 0, D_0044E848[*nodeRecord]);
        memset_C070(params, 0, 0x10);
        switch (*nodeRecord) {
        case 0:
            f_F7B0_C070(D_00515D44[i]);
            break;
        case 1:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 4, 2) << 8;
            params[1] = rate << 8;
            params[2] = f_72F8_C070(nodeRecord + 6, 2);
            SFILTER_createLPFRC((LPFRCSTATE*)obj);
            SFILTER_modifyLPFRC((LPFRCSTATE*)obj, params);
            break;
        case 2:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 2, 2) << 8;
            params[1] = rate << 8;
            f_EE88_C070(obj);
            f_EED0_C070(obj, params);
            break;
        case 3:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 2, 2) << 8;
            params[1] = rate << 8;
            SFILTER_createHPFFIR8(obj);
            f_A9D8_C070(obj, params);
            break;
        case 4:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 4, 2) << 8;
            params[1] = f_72F8_C070(nodeRecord + 6, 2) << 8;
            params[2] = rate << 8;
            f_EFB0_C070(obj);
            f_EFF8_C070(obj, params);
            break;
        case 5:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 2, 2) << 8;
            params[1] = rate << 8;
            params[2] = f_72F8_C070(nodeRecord + 4, 2) << 8;
            params[3] = f_72F8_C070(nodeRecord + 6, 2);
            f_F498_C070(obj);
            f_F4D0_C070(obj, params);
            break;
        case 6:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 2, 2);
            f_F128_C070(obj);
            f_F158_C070(obj, params);
            break;
        case 7:
            obj = D_00515D44[i];
            params[0] = f_72F8_C070(nodeRecord + 2, 2);
            params[1] = rate << 8;
            params[2] = f_72F8_C070(nodeRecord + 4, 2) << 8;
            f_F998_C070(obj);
            f_F9D8_C070(obj, params);
            break;
        case 8:
            f_F2C0_C070(D_00515D44[i]);
            break;
        case 9:
            f_F6F8_C070(D_00515D44[i]);
            break;
        }
        if (nodeRecord[1] != 0) {
            if (nodeRecord[0] == 9 && D_00515D40->ver >= 11) {
                unsigned char ch = nodeRecord[1];
                D_00515D48[func_003CBC20(D_00515D58, sndmix.mc.outputchannels, ch & 0xF)] = D_00515D44[i];
                D_00515D48[func_003CBC20(D_00515D58, sndmix.mc.outputchannels, ch >> 4)] = D_00515D44[i];
            } else {
                D_00515D48[func_003CBC20(D_00515D58, sndmix.mc.outputchannels, nodeRecord[1])] = D_00515D44[i];
            }
        }
        nodeRecord += D_0044E870_C070[*nodeRecord];
    }
    link = D_00515D50;
    for (i = 0; i < f_72F8_C070(&D_00515D40->len[last], 4); i++) {
        SFILTER_connect((SFILTERNODE*)D_00515D44[f_72F8_C070(link, 4)], (SFILTERNODE*)D_00515D44[f_72F8_C070(link + 4, 4)], link[8], link[9]);
        link += 12;
    }
    D_00515D80_C070[0x1800 / 4] = 3000;
    memset_C070(D_00515D80_C070, 0, 0x800);
    nodeRecord = D_00515D4C;
    D_00517600 = 0;
    while (*nodeRecord != 0) {
        unsigned char nodeKind = *nodeRecord;
        D_00517600++;
        nodeRecord += D_0044E870_C070[nodeKind];
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CC630);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Hdr_C630 {
    int f0;
    int f4;
    int cnt[6];
    int len[6];
};
struct G_C630 {
    char pad00[0x5];
    unsigned char n;
    char pad06[0xF - 0x6];
    signed char fF;
    void *f10;
};
extern struct Hdr_C630 D_005175C0_C630 __asm__("D_005175C0");
extern int D_005175C8_C630[] __asm__("D_005175C8");
extern int D_0044E870_C630[] __asm__("D_0044E870");
extern int D_0044E898;
extern int D_00515D54;
extern int D_00515D58;
extern unsigned char *D_005175F8;
extern unsigned char *D_005175FC;
extern void func_003CBC58();
extern void func_003CC848_C630(void) __asm__("func_003CC848");
extern void Util_MemCpy_C630(void *dst, void *src, int size) __asm__("Util_MemCpy");
extern int SNDI_getb_C630(void *p, int size) __asm__("SNDI_getb");
extern void func_003CC070_C630(int a, unsigned char *data) __asm__("func_003CC070");

extern struct G_C630 sndmix_C630 __asm__("sndmix");
/**
 * Loads a software effect (reverb) and starts it.
 *
 * Removes the current one (func_003CC848), copies the 0x38-byte header, picks the largest channel layout the effect has
 * nodes for up to the mixer's output count (D_00515D58), finds that layout's node and link lists (D_005175F8,
 * D_005175FC), builds the graph (func_003CC070), installs func_003CBC58 as the mixer's output hook (+0x10) and turns
 * the send on (+0x0F). Called by func_003C35F8.
 *
 * @param outputRate  the output sample rate
 * @param data  the effect description
 */
void func_003CC630(int outputRate, unsigned char *data) {
    unsigned char *nodeCursor = data + 0x38;
    int n;
    int i;
    int j;
    int k;

    func_003CC848_C630();
    D_0044E898 = 1;
    Util_MemCpy_C630(&D_005175C0_C630, data, 0x38);
    n = sndmix_C630.n - 1;
    if (SNDI_getb_C630(&D_005175C0_C630.cnt[n], 4) != 0) {
        D_00515D58 = sndmix_C630.n;
    } else {
        while (SNDI_getb_C630(&D_005175C8_C630[n], 4) == 0) {
            n--;
        }
        D_00515D58 = n + 1;
    }
    D_00515D54 = D_00515D58;
    for (i = 0; i < n; i++) {
        k = SNDI_getb_C630(&D_005175C8_C630[i], 4);
        for (j = 0; j < k; j++) {
            nodeCursor += D_0044E870_C630[*nodeCursor];
        }
        nodeCursor += SNDI_getb_C630(&D_005175C8_C630[i + 6], 4) * 12;
    }
    D_005175F8 = nodeCursor;
    k = SNDI_getb_C630(&D_005175C8_C630[n], 4);
    for (j = 0; j < k; j++) {
        nodeCursor += D_0044E870_C630[*nodeCursor];
    }
    D_005175FC = nodeCursor;
    func_003CC070_C630(outputRate, data);
    sndmix_C630.f10 = func_003CBC58;
    sndmix_C630.fF = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003CC848);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003CBEC8();
struct S_C848 { char pad[0xF]; char a; int b; };

extern struct S_C848 sndmix_C848 __asm__("sndmix");
/**
 * Stops the software effect: clears the mixer's output hook (+0x10), frees the graph (func_003CBEC8) and turns the send
 * off (+0x0F).
 *
 * Called by func_003C35F8 and func_003CC630.
 */
void func_003CC848(void) {
    sndmix_C848.b = 0;
    func_003CBEC8();
    sndmix_C848.a = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CC878);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

struct s5D80 {                      /* D_00515D80: the effect send buffer */
    float f0000[0x600];
    int f1800;
};

extern struct s5D80 D_00515D80;

/**
 * Delivers the effect send to the IOP instead of running it here (an output hook installed by func_003C3450).
 *
 * When a destination is set (D_00515B54), an MMI / VU0 loop scales the send buffer's floats by 127, converts them to
 * integers into it and clears the send buffer, 16 at a time.
 *
 * @param n  the number of samples
 */
// PORT: hand-written VU0 loop (trapping add/addi have no gcc 2.95 pattern; the block moves its operands into fixed
// $4/$5/$6 itself: retail's `daddu $7,$4` then `daddu $4,$7` is gcc moving n out of the clobbered $4 for the asm's own
// `move $4, %0`): scales n floats of the send buffer by 127, converts them to int into the output buffer and clears
// the send buffer, 16 per pass. A port replaces it with the C loop.
void func_003CC878(int n) {
    int *dst = sndmix.wetbusactive;
    if (dst != 0) {
        // PORT: PS2-only VU0 hand-written loop (scale by 127, convert, clear source); one inline asm block.
        __asm__ volatile(
            ".set push\n\t"
            ".set noreorder\n\t"
            ".set noat\n\t"
            "move $4, %0\n\t"
            "move $5, %1\n\t"
            "move $6, %2\n\t"
            "addiu $8, $0, 0x7F\n\t"
            "sll $4, $4, 2\n\t"
            "qmtc2.ni $8, $vf1\n\t"
            "add $4, $4, $5\n\t"
            "vitof0.xyzw $vf1, $vf1\n"
            "1:\n\t"
            "lqc2 $vf2, 0x0($5)\n\t"
            "lqc2 $vf3, 0x10($5)\n\t"
            "lqc2 $vf4, 0x20($5)\n\t"
            "lqc2 $vf5, 0x30($5)\n\t"
            "vmulx.xyzw $vf2, $vf2, $vf1x\n\t"
            "sq $0, 0x0($5)\n\t"
            "vmulx.xyzw $vf3, $vf3, $vf1x\n\t"
            "sq $0, 0x10($5)\n\t"
            "vmulx.xyzw $vf4, $vf4, $vf1x\n\t"
            "sq $0, 0x20($5)\n\t"
            "vmulx.xyzw $vf5, $vf5, $vf1x\n\t"
            "sq $0, 0x30($5)\n\t"
            "addi $5, $5, 0x40\n\t"
            "addi $6, $6, 0x40\n\t"
            "vftoi0.xyzw $vf2, $vf2\n\t"
            "vftoi0.xyzw $vf3, $vf3\n\t"
            "vftoi0.xyzw $vf4, $vf4\n\t"
            "vftoi0.xyzw $vf5, $vf5\n\t"
            "sqc2 $vf2, -0x40($6)\n\t"
            "sqc2 $vf3, -0x30($6)\n\t"
            "sqc2 $vf4, -0x20($6)\n\t"
            "bne $4, $5, 1b\n\t"
            "sqc2 $vf5, -0x10($6)\n\t"
            ".set pop\n"
            : : "r"(n), "r"(D_00515D80.f0000), "r"(dst)
            : "$4", "$5", "$6", "$8", "memory");
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CODASetNew);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CODANew_C918[4] __asm__("CODANew");

/**
 * Sets the allocator the decoder library uses (CODANew). MIX_create installs SNDI_New.
 *
 * @param allocFunc  the allocator
 */
void CODASetNew(int allocFunc) {
    CODANew_C918[0] = allocFunc;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CODASetDelete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int CODADelete_C928[4] __asm__("CODADelete");

/**
 * Sets the deallocator the decoder library uses (CODADelete). MIX_create installs SNDI_Delete.
 *
 * @param freeFunc  the deallocator
 */
void CODASetDelete(int freeFunc) {
    CODADelete_C928[0] = freeFunc;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", process_raw_block);
#ifdef SKIP_ASM
typedef struct { int pad0; float x; float y; unsigned char *src; float *dst; } S_C938;
typedef union { short s; unsigned char c[2]; } U_C938;

/**
 * Decodes an uncompressed EA-XA block, one whose first byte is 0xEE.
 *
 * Reads two big-endian 16-bit history values (+0x04, +0x08) and 28 big-endian 16-bit samples into the float output,
 * advancing the source (+0x0C) and destination (+0x10). Other blocks are left alone. Called by func_003CCA38.
 *
 * @param decoder  the block decoder state
 */
void process_raw_block(S_C938 *decoder) {
    unsigned char swapped[2];
    unsigned int i;

    if (*decoder->src == 238) {
        decoder->src++;
        swapped[0] = decoder->src[1];
        swapped[1] = decoder->src[0];
        decoder->src += 2;
        decoder->x = (float)*(short*)swapped;
        swapped[0] = decoder->src[1];
        swapped[1] = decoder->src[0];
        decoder->src += 2;
        decoder->y = (float)*(short*)swapped;
        for (i = 0; i < 0x1C; i++) {
            swapped[0] = decoder->src[1];
            swapped[1] = decoder->src[0];
            *decoder->dst = (float)*(short*)swapped;
            decoder->dst++;
            decoder->src += 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", decodexac);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written entry stub that falls through into func_003CCA38; the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
__asm__(
    ".set push\n.set noreorder\n.set noat\n"
    ".text\n.align 3\n.globl decodexac\n.type decodexac, @function\ndecodexac:\n"
    "lui $2, %hi(D_0044E8A0)\n"
    "daddu $3, $4, $0\n"
    "addiu $2, $2, %lo(D_0044E8A0)\n"
    "daddu $4, $3, $0\n"
    "daddu $5, $2, $0\n"
    "addi $29, $29, -0x10\n"
    "sw $31, 0x0($29)\n"
    "lw $6, 0x0($4)\n"
    "lwc1 $f10, 0x4($4)\n"
    "lwc1 $f11, 0x8($4)\n"
    "lw $7, 0xC($4)\n"
    "lw $8, 0x10($4)\n"
    ".size decodexac, . - decodexac\n"
    ".set pop\n");
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CCA38);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written FPU routine (mula.s/madda.s accumulator ops, unsaved $f20-$f25); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
void func_003CCA38(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "blez $6, 2f\n"
        "nop\n"
        "lbu $9, 0x0($7)\n"
        "addi $6, $6, -0x1C\n"
        "addiu $13, $0, 0xEE\n"
        "lbu $14, 0x0($7)\n"
        "bne $13, $14, 1f\n"
        "nop\n"
        "addi $29, $29, -0x30\n"
        "sw $4, 0x20($29)\n"
        "sw $5, 0x1C($29)\n"
        "sw $6, 0x18($29)\n"
        "sw $7, 0x14($29)\n"
        "sw $8, 0x10($29)\n"
        "sw $9, 0xC($29)\n"
        "sw $10, 0x8($29)\n"
        "sw $11, 0x4($29)\n"
        "sw $12, 0x0($29)\n"
        "jal process_raw_block\n"
        "nop\n"
        "lw $12, 0x0($29)\n"
        "lw $11, 0x4($29)\n"
        "lw $10, 0x8($29)\n"
        "lw $9, 0xC($29)\n"
        "lw $8, 0x10($29)\n"
        "lw $7, 0x14($29)\n"
        "lw $6, 0x18($29)\n"
        "lw $5, 0x1C($29)\n"
        "lw $4, 0x20($29)\n"
        "addi $29, $29, 0x30\n"
        "lwc1 $f10, 0x4($4)\n"
        "lwc1 $f11, 0x8($4)\n"
        "lw $7, 0xC($4)\n"
        "lw $8, 0x10($4)\n"
        "j func_003CCA38\n"
        "nop\n"
        "1:\n"
        "srl $10, $9, 4\n"
        "andi $9, $9, 0xF\n"
        "sll $10, $10, 2\n"
        "add $10, $10, $5\n"
        "lwc1 $f0, 0x0($10)\n"
        "lwc1 $f1, 0x10($10)\n"
        "addi $10, $0, 0xC\n"
        "sub $10, $10, $9\n"
        "addi $9, $9, 0x10\n"
        "lb $11, 0x1($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "mtc1 $0, $f31\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x2($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x0($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x4($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x3($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x8($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0xC($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x4($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x10($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x14($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x5($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x18($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0x1C($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x6($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x20($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x24($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x7($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x28($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0x2C($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x8($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x30($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x34($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0x9($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x38($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0x3C($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0xA($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x40($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x44($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0xB($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x48($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0x4C($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0xC($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x50($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x54($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0xD($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x58($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f2\n"
        "mtc1 $11, $f3\n"
        "cvt.s.w $f2, $f2\n"
        "cvt.s.w $f3, $f3\n"
        "swc1 $f10, 0x5C($8)\n"
        "adda.s $f2, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "lb $11, 0xE($7)\n"
        "sra $12, $11, 4\n"
        "sll $11, $11, 28\n"
        "sllv $12, $12, $10\n"
        "srav $11, $11, $9\n"
        "swc1 $f11, 0x60($8)\n"
        "adda.s $f3, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "mtc1 $12, $f4\n"
        "mtc1 $11, $f5\n"
        "cvt.s.w $f4, $f4\n"
        "cvt.s.w $f5, $f5\n"
        "swc1 $f10, 0x64($8)\n"
        "adda.s $f4, $f31\n"
        "madda.s $f10, $f0\n"
        "madd.s $f11, $f11, $f1\n"
        "swc1 $f11, 0x68($8)\n"
        "adda.s $f5, $f31\n"
        "madda.s $f11, $f0\n"
        "madd.s $f10, $f10, $f1\n"
        "swc1 $f10, 0x6C($8)\n"
        "addi $7, $7, 0xF\n"
        "addi $8, $8, 0x70\n"
        "swc1 $f10, 0x4($4)\n"
        "swc1 $f11, 0x8($4)\n"
        "sw $7, 0xC($4)\n"
        "sw $8, 0x10($4)\n"
        "j func_003CCA38\n"
        "nop\n"
        "2:\n"
        "sw $6, 0x0($4)\n"
        "swc1 $f10, 0x4($4)\n"
        "swc1 $f11, 0x8($4)\n"
        "sw $7, 0xC($4)\n"
        "sw $8, 0x10($4)\n"
        "lq $31, 0x0($29)\n"
        "addi $29, $29, 0x10\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_operator_new);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*CODANew_CEF0[])(void) __asm__("CODANew");
/**
 * Allocates memory for an EA-XA decoder through the decoder library's allocator (CODANew, set by CODASetNew); the
 * size is passed on.
 *
 * The decoder inits call it with 168, then construct the decoder with CEAXABLKDecf_CEAXABLKDecf.
 *
 * @return the memory
 */
int CEAXABLKDecf_operator_new(void) {
    return CODANew_CEF0[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_operator_delete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*CODADelete_CF18[])(void) __asm__("CODADelete");
int CEAXABLKDecf_operator_delete_CF18(int) __asm__("CEAXABLKDecf_operator_delete");
/**
 * Frees an EA-XA decoder through the decoder library's deallocator (CODADelete, set by CODASetDelete).
 *
 * Called by the EA-XA release functions.
 *
 * @param decoder  the decoder
 * @return what the deallocator returns
 */
int CEAXABLKDecf_operator_delete_CF18(int decoder) {
    return CODADelete_CF18[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_CEAXABLKDecf);
#ifdef SKIP_ASM
/**
 * Constructs an EA-XA decoder: clears the pending (+0x04) and remaining (+0x08) frame counts, the data size (+0x0C) and
 * the history (+0x98, +0x9C).
 *
 * @param decoder  the decoder
 * @return the decoder
 */
void *CEAXABLKDecf_CEAXABLKDecf(CEAXABLKDecf* decoder) {
    decoder->xav.sampledatasize = 0;
    decoder->xav.numsamples = 0;
    decoder->xav.residual = 0;
    (*(int*)&decoder->xap.sample1) = 0;
    (*(int*)&decoder->xap.sample2) = 0;
    return ((void*)decoder);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_Feed);
#ifdef SKIP_ASM
/**
 * Gives an EA-XA decoder data to decode: the frame count (+0x08), size (+0x0C) and data (+0xA0).
 *
 * @param decoder  the decoder
 * @param data  the EA-XA data
 * @param byteSize  its size
 * @param sampleCount  the frames it holds
 * @return 0, or -1 for null data or while the decoder still has frames left
 */
int CEAXABLKDecf_Feed(CEAXABLKDecf* decoder, int data, int byteSize, int sampleCount) {
    if (data == 0) {
        return -1;
    }
    if ((decoder->xav.numsamples) != 0) {
        return -1;
    }
    decoder->xav.numsamples = sampleCount;
    decoder->xav.sampledatasize = byteSize;
    ((int)decoder->xap.psrc) = data;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_Decode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_CF90 { int f0; int pending; int remaining; int fC; float *rd; char pad[0x10]; float buf[28]; int n; int pad2[3]; float *out; };
extern void decodexac_CF90(int *) __asm__("decodexac");

/**
 * Decodes up to n frames of an EA-XA decoder's data into a float buffer.
 *
 * First hands out frames left over from a partly used block, then decodes whole 28-frame blocks straight into the
 * output (decodexac) and finally one more block into the internal buffer (+0x18), keeping its unused frames
 * pending. The caller's pointer itself is not advanced.
 *
 * @param decoder  the decoder
 * @param pout  the output pointer
 * @param n  the frames wanted
 * @return the frames decoded (0 when the data is used up)
 */
int CEAXABLKDecf_Decode(struct S_CF90 *decoder, float **pout, int n) {
    int done = 0;
    int toRead;
    int k;
    int i;
    int rem;
    float *dst;

    decoder->out = *pout;
    if (decoder->remaining == 0) {
        return 0;
    }
    toRead = decoder->remaining;
    if (n < toRead) {
        toRead = n;
    }
    if (decoder->pending != 0) {
        if (toRead < decoder->pending) {
            k = toRead;
        } else {
            k = decoder->pending;
        }
        for (i = 0; i < k; i++) {
            *decoder->out = *decoder->rd;
            decoder->out++;
            decoder->rd++;
        }
        toRead -= k;
        decoder->remaining -= k;
        decoder->pending -= k;
        done = k;
    }
    i = toRead / 28 * 28;
    rem = toRead - i;
    decoder->n = i;
    if (i > 0) {
        done += i;
        decodexac_CF90(&decoder->n);
        decoder->remaining -= i;
    }
    if (rem > 0) {
        dst = decoder->out;
        decoder->n = rem;
        decoder->out = decoder->buf;
        decodexac_CF90(&decoder->n);
        decoder->remaining -= rem;
        decoder->rd = decoder->out + decoder->n;
        decoder->pending = -decoder->n;
        decoder->out -= 28;
        for (i = 0; i < rem; i++) {
            dst[i] = decoder->out[i];
        }
        done += rem;
    }
    if (decoder->remaining <= 0) {
        decoder->pending = 0;
    }
    return done;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_GetState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { float x; float y; } V2_D138;
V2_D138 CEAXABLKDecf_GetState_D138(char *p) __asm__("CEAXABLKDecf_GetState");

/**
 * Returns an EA-XA decoder's predictor history (+0x98, +0x9C). Called by SFILTER_unpackxalf to save it at a loop point.
 *
 * @param decoder  the decoder
 * @return the two history values
 */
V2_D138 CEAXABLKDecf_GetState_D138(char *decoder) {
    V2_D138 history;
    V2_D138 w;
    history.x = *(float *)(decoder + 0x98);
    history.y = *(float *)(decoder + 0x9C);
    w = history;
    return w;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CEAXABLKDecf_SetState);
#ifdef SKIP_ASM
/**
 * Sets an EA-XA decoder's predictor history (+0x98, +0x9C) from two floats.
 *
 * Called by SFILTER_unpackxalf and SFILTER_unpackxapf.
 *
 * @param decoder  the decoder
 * @param history  the two history values
 */
void CEAXABLKDecf_SetState(void *decoder, void *history) {
    (*(float *)((char*)(decoder) + (0x98))) = (float) (*(float *)((char*)(history) + (0)));
    (*(float *)((char*)(decoder) + (0x9C))) = (float) (*(float *)((char*)(history) + (4)));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_operator_new);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*CODANew_D1A8[])(void) __asm__("CODANew");
int CMTBLKDecf_operator_new_D1A8(int) __asm__("CMTBLKDecf_operator_new");
/**
 * Allocates memory for a MicroTalk decoder through the decoder library's allocator (CODANew); the size is passed on.
 *
 * The MicroTalk inits call it with 3432, then construct the decoder with CMTBLKDecf_CMTBLKDecf.
 *
 * @param size  the size in bytes
 * @return the memory
 */
int CMTBLKDecf_operator_new_D1A8(int size) {
    return CODANew_D1A8[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_operator_delete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int (*CODADelete_D1D0[])(void) __asm__("CODADelete");
int CMTBLKDecf_operator_delete_D1D0(int) __asm__("CMTBLKDecf_operator_delete");
/**
 * Frees a MicroTalk decoder through the decoder library's deallocator (CODADelete).
 *
 * Called by the MicroTalk release functions.
 *
 * @param decoder  the decoder
 * @return what the deallocator returns
 */
int CMTBLKDecf_operator_delete_D1D0(int decoder) {
    return CODADelete_D1D0[0]();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", getbits);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct BS_D1F8 { unsigned char *ptr; unsigned int bits; int cnt; } BS_D1F8;
extern unsigned int bitmask_E8C0[] __asm__("bitmask");

/**
 * Reads bits from a MicroTalk bit stream, least significant first.
 *
 * Returns the low n bits of the bit buffer (+0x04; masks bitmask), shifts them out and, when fewer than 8 bits
 * remain (+0x08), appends the next byte (+0x00).
 *
 * @param bitReader  the stream state
 * @param n  the number of bits
 * @return the bits
 */
int getbits(BS_D1F8 *bitReader, int n) {
    unsigned int bitBuffer = bitReader->bits;
    int ret = bitBuffer & bitmask_E8C0[n];
    bitReader->bits >>= n;
    bitReader->cnt -= n;
    if (bitReader->cnt < 8) {
        bitReader->bits |= *bitReader->ptr++ << bitReader->cnt;
        bitReader->cnt += 8;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", discardbits);
#ifdef SKIP_ASM
typedef struct { unsigned char *p; unsigned int bits; int n; } B_D260;

/**
 * Drops bits from a MicroTalk bit stream: shifts out k bits and appends the next byte when fewer than 8 remain.
 *
 * Called by readsamples after a table lookup.
 *
 * @param reader  the stream state
 * @param k  the number of bits
 */
void discardbits(B_D260 *reader, int k) {
    unsigned int shifted = reader->bits >> k;
    int n = reader->n - k;
    reader->bits = shifted;
    reader->n = n;
    if (n < 8) {
        unsigned char *cursor = reader->p;
        unsigned int nextByte = *cursor;
        reader->n = n + 8;
        reader->p = cursor + 1;
        reader->bits = shifted | (nextByte << n);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", readsamples);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Hc_D2B0 { int next; int bits; float val; };
extern unsigned char D_0044E9E8_D2B0[][256] __asm__("D_0044E9E8");
extern struct Hc_D2B0 D_0044EBE8_D2B0[] __asm__("D_0044EBE8");
extern int f_D1F8_D2B0(char *, int) __asm__("getbits");
extern void f_D260_D2B0(char *, int) __asm__("discardbits");

void readsamples_impl(char *bs, int flag, float *out, int stride) __asm__("readsamples");
/**
 * Decodes the excitation of one MicroTalk subframe (108 samples, every stride-th one).
 *
 * Voiced frames use the variable-length code of tables D_0044E9E8 / D_0044EBE8: symbols 4 and up are values, 2-3 a run
 * of 7 to 70 zeros (6-bit length), 0-1 an escape (7 plus a unary count, then a sign bit). Unvoiced frames use 2-bit
 * codes: -2, +2, or 0 in a single bit.
 *
 * @param bs  the stream state
 * @param flag  non-zero for a voiced frame
 * @param out  the samples
 * @param stride  1, or 2 for the reduced-bandwidth mode
 */
void readsamples_impl(char *bs, int flag, float *out, int stride) {
    int state, pos, sym, n, i, p;

    if (flag != 0) {
        state = 0;
        pos = 0;
        do {
        sym = D_0044E9E8_D2B0[state][*(int *)(bs + 4) & 0xFF];
        state = (D_0044EBE8_D2B0 + sym)->next;
        f_D260_D2B0(bs, D_0044EBE8_D2B0[sym].bits);
        if (sym >= 4) {
            out[pos] = D_0044EBE8_D2B0[sym].val;
            pos += stride;
        } else if (sym >= 2) {
            n = f_D1F8_D2B0(bs, 6) + 7;
            if (stride * n + pos > 108) {
                n = (108 - pos) / stride;
            }
            for (i = 0; i < n; i++) {
                out[pos] = 0.0f;
                pos += stride;
            }
        } else {
            i = 7;
            while (f_D1F8_D2B0(bs, 1) == 1) {
                i++;
            }
            if (f_D1F8_D2B0(bs, 1) == 1) {
                out[pos] = (float)i;
            } else {
                out[pos] = (float)-i;
            }
            pos += stride;
        }
        } while (pos < 108);
    } else {
        for (i = 0; i < 108; i += stride) {
            pos = *(int *)(bs + 4) & 3;
            if (pos == 1) {
                out[i] = -2.0f;
                f_D260_D2B0(bs, 2);
            } else if (pos == 3) {
                out[i] = 2.0f;
                f_D260_D2B0(bs, 2);
            } else {
                out[i] = 0.0f;
                f_D260_D2B0(bs, 1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", interpolate);
#ifdef SKIP_ASM
/**
 * Fills in the skipped samples of a half-rate MicroTalk subframe.
 *
 * Every second sample becomes 0.5974 * (x[-1] + x[+1]) - 0.1146 * (x[-3] + x[+3]) + 0.0180 * (x[-5] + x[+5]), for 54
 * samples. Called by decodemut.
 *
 * @param samples  the first sample to fill
 */
void interpolate(void *samples) {
    int var_2;
    void *var_4;

    var_4 = samples;
    var_2 = 106;
    do {
        var_2 -= 2;
        (*(float *)((char*)(var_4) + (0))) = (float) ((((*(float *)((char*)(var_4) + (-4))) + (*(float *)((char*)(var_4) + (4)))) * 0.59738594f) + (((*(float *)((char*)(var_4) + (-0xC))) + (*(float *)((char*)(var_4) + (0xC)))) * -0.11459156f) + (((*(float *)((char*)(var_4) + (-0x14))) + (*(float *)((char*)(var_4) + (0x14)))) * 0.01803268f));
        var_4 += 8;
    } while (var_2 >= 0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", reftolpc);
#ifdef SKIP_ASM
/**
 * Converts MicroTalk's 12 reflection coefficients into the 12 coefficients of the synthesis filter (lattice to direct
 * form, done through the impulse response).
 *
 * Called by filter.
 *
 * @param a_  the reflection coefficients
 * @param out_  the filter coefficients
 */
void reftolpc(char *a_, void *out_) {
    float *coeffs = (float *)a_;
    float *out = (float *)out_;
    float work[12];
    float y[12];
    int i, j;
    float sum;
    for (j = 10; j >= 0; j--)
        work[j + 1] = coeffs[j];
    work[0] = 1.0f;
    for (i = 0; i < 12; i++) {
        sum = -coeffs[11] * work[11];
        for (j = 10; j >= 0; j--) {
            sum -= coeffs[j] * work[j];
            work[j + 1] = coeffs[j] * sum + work[j];
        }
        work[0] = sum;
        y[i] = sum;
        { int k; for (k = 0; k < i; k++) sum -= out[k] * y[i - k - 1]; }
        out[i] = sum;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", filter);
#ifdef SKIP_ASM
extern void reftolpc(char *, void *);
extern void func_003CE410(int, void *, char *, char *);

/**
 * Runs the MicroTalk synthesis filter over decoded samples.
 *
 * Converts the current reflection coefficients (+0x114) to filter coefficients (reftolpc) and filters count blocks
 * of 12 samples of the decoded buffer (+0x684) from the index on, with the filter history at +0x144 (func_003CE410).
 * Called by decodemut.
 *
 * @param utkState  the decoder state
 * @param sampleIndex  the first sample
 * @param blockCount  the number of 12-sample blocks
 */
void filter(UTALKSTATE_CODA* utkState, int sampleIndex, int blockCount) {
    char buf[0x30];

    reftolpc((char*)&utkState->coeff[0], buf);
    func_003CE410(blockCount, buf, (char*)&utkState->output[0], (char*)&utkState->data[sampleIndex]);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_initmut);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct S_D6F0 {
    unsigned char *src; int w4; int w8; int wC; int w10; float tab[64];
    int z1[12]; int z2[12]; int z3[0x144];
};

/**
 * Starts a MicroTalk decoder on new data.
 *
 * Points the bit reader at the data with its first byte loaded. For a whole init it also reads the stream header: the
 * reduced-bandwidth flag (1 bit), the voiced threshold (32 minus 4 bits) and the 64-entry fixed-gain table (base (4
 * bits + 1) * 8, each entry 1.04 + (6 bits) * 0.001 times the last); and it clears the reflection coefficients, filter
 * history and adaptive codebook.
 *
 * @param decoder  the decoder (unused)
 * @param data  the data
 * @param stateMem  the decoder state
 * @param wholeInit  non-zero for a whole init
 */
void CMTBLKDecf_initmut(char *decoder, unsigned char *data, char *stateMem, int wholeInit) {
    struct S_D6F0 *utkState = (struct S_D6F0 *)stateMem;
    int n;
    float gainStep;
    int i;

    utkState->w8 = 8;
    utkState->src = data + 1;
    utkState->w4 = *data;
    if (wholeInit != 0) {
        utkState->wC = getbits((BS_D1F8 *)utkState, 1);
        utkState->w10 = 0x20 - getbits((BS_D1F8 *)utkState, 4);
        n = getbits((BS_D1F8 *)utkState, 4);
        utkState->tab[0] = (float)(unsigned)(n + 1) * 8.0f;
        i = getbits((BS_D1F8 *)utkState, 6);
        gainStep = (float)(unsigned)i * 0.001f + 1.04f;
        for (i = 1; i < 64; i++) {
            utkState->tab[i] = utkState->tab[i - 1] * gainStep;
        }
        for (i = 0; i < 12; i++) {
            utkState->z1[i] = 0;
            utkState->z2[i] = 0;
        }
        for (i = 323; i >= 0; i--) {
            utkState->z3[i] = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", decodemut);
#ifdef SKIP_ASM
/* EA MicroTalk (UTK) frame decoder: reflection-coefficient deltas, 4 subframes
   of 108 samples, adaptive codebook shift and per-subframe synthesis. */
typedef struct sUTK_D878 {
    unsigned char *ptr;
    unsigned int bits;
    int cnt;
    int reducedBw;
    int voicedThreshold;
    float fixedGains[64];
    float rc[12];
    float synth[12];
    float adaptCb[324];
    float decoded[432];
} sUTK_D878;
typedef struct { float f[324]; } sF324_D878;
extern float coeff_table[];
void readsamples(sUTK_D878 *ctx, int voiced, float *out, int stride);

/**
 * Decodes one MicroTalk frame (432 samples) into the decoder's output buffer.
 *
 * Reads the targets of the 12 reflection coefficients (4 of 6 bits, 8 of 5 bits, table coeff_table); the first index
 * decides whether the frame is voiced. Each of the 4 subframes of 108 samples has a pitch lag (8 bits), pitch gain (4
 * bits / 15) and fixed gain (6-bit index); its excitation (readsamples, at half rate with interpolate or zeros in
 * the reduced-bandwidth mode) plus the adaptive codebook gives the samples. The codebook then shifts, and the filter
 * coefficients move to their targets in 4 steps while filter synthesises 12, 12, 12 and 396 samples.
 *
 * @param ctx  the decoder state
 */
void decodemut(sUTK_D878 *ctx) {
    float exc[118];
    float delta[12];
    int i;
    int j;
    int idx;
    int voiced;
    int pos;
    float pitchGain;
    float fixedGain;
    int align;
    int zero;
    int base;

    idx = getbits(ctx, 6);
    voiced = idx < ctx->voicedThreshold;
    delta[0] = (coeff_table[idx] - ctx->rc[0]) * 0.25f;
    for (j = 1; j < 4; j++)
        delta[j] = (coeff_table[getbits(ctx, 6)] - ctx->rc[j]) * 0.25f;
    for (j = 4; j < 12; j++)
        delta[j] = (coeff_table[getbits(ctx, 5) + 16] - ctx->rc[j]) * 0.25f;

    for (i = 0; i < 4; i++) {
        pos = (i + 2) * 108 - getbits(ctx, 8);
        pitchGain = (float)(unsigned int)getbits(ctx, 4) * 0.06666667014360428f;
        fixedGain = ctx->fixedGains[getbits(ctx, 6)];
        if (ctx->reducedBw == 0) {
            readsamples(ctx, voiced, &exc[5], 1);
        } else {
            align = getbits(ctx, 1);
            zero = getbits(ctx, 1);
            readsamples(ctx, voiced, &exc[5 + align], 2);
            if (zero) {
                for (j = 0; j < 108; j += 2)
                    exc[j + 6 - align] = 0.0f;
            } else {
                for (j = 0; j < 5; j++)
                    exc[j] = 0.0f;
                for (j = 113; j < 118; j++)
                    exc[j] = 0.0f;
                interpolate(exc + 6 - align);
                fixedGain *= 0.5f;
            }
        }
        base = i * 108;
        for (j = 0; j < 108; j++)
            ctx->decoded[base + j] = pitchGain * ctx->adaptCb[pos + j] + fixedGain * exc[5 + j];
    }

    *(sF324_D878 *)ctx->adaptCb = *(sF324_D878 *)&ctx->decoded[108];

    for (j = 0; j < 12; j++)
        ctx->rc[j] += delta[j];
    filter((UTALKSTATE_CODA*)ctx, 0, 1);
    for (j = 0; j < 12; j++)
        ctx->rc[j] += delta[j];
    filter((UTALKSTATE_CODA*)ctx, 12, 1);
    for (j = 0; j < 12; j++)
        ctx->rc[j] += delta[j];
    filter((UTALKSTATE_CODA*)ctx, 24, 1);
    for (j = 0; j < 12; j++)
        ctx->rc[j] += delta[j];
    filter((UTALKSTATE_CODA*)ctx, 36, 33);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_CMTBLKDecf);
#ifdef SKIP_ASM
/**
 * Constructs a MicroTalk decoder: clears its position (+0xD44), frame and byte counts (+0xD54, +0xD58), +0xD50 /
 * +0xD52, +0xD5C and +0xD60, and sets the codec version (+0xD64) to 1.
 *
 * @param decoder  the decoder
 * @return the decoder
 */
void *CMTBLKDecf_CMTBLKDecf(CMTBLKDecf* decoder) {
    ((int)decoder->mbufframes) = 0;
    decoder->mCodecVersion = 1;
    decoder->mRemainingSamples = 0;
    decoder->mSampleDataSize = 0;
    decoder->mRawframes = 0;
    decoder->mOffset = 0;
    decoder->mdoWholeInit = 0;
    decoder->mHasPCMData = 0;
    return ((void*)decoder);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_Feed);
#ifdef SKIP_ASM
extern void CMTBLKDecf_initmut(char *, unsigned char *, char *, int);

/**
 * Gives a MicroTalk decoder data to decode and starts it (CMTBLKDecf_initmut).
 *
 * Stores the frame count (+0xD54), size (+0xD58) and data (+0xD48) and rewinds (+0xD44). With codec version 1 (+0xD64)
 * the first byte is a marker skipped before decoding (0xEE sets +0xD60); the whole-init flag comes from +0xD5C.
 *
 * @param decoder  the decoder
 * @param data  the MicroTalk data
 * @param byteSize  its size
 * @param sampleCount  the frames it holds
 * @return 0, or -1 for null data or while the decoder still has frames left
 */
int CMTBLKDecf_Feed(CMTBLKDecf* decoder, unsigned char *data, int byteSize, int sampleCount) {
    int temp_8;

    if (data == 0) return -1;
    if (decoder->mRemainingSamples != 0) return -1;
    temp_8 = decoder->mCodecVersion;
    decoder->mRemainingSamples = sampleCount;
    decoder->mSampleDataSize = byteSize;
    decoder->mbufframes = 0;
    decoder->mpSrc = data;
    if (temp_8 == 1) {
        if (*data == 238) {
            decoder->mHasPCMData = temp_8;
        } else {
            decoder->mHasPCMData = 0;
        }
        CMTBLKDecf_initmut(((char*)decoder), data + 1, ((char*)decoder), decoder->mdoWholeInit);
    } else {
        CMTBLKDecf_initmut(((char*)decoder), data, ((char*)decoder), decoder->mdoWholeInit);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_Decode);
#ifdef SKIP_ASM
extern int func_0041605C_DE68(void *, void *, int) __asm__("func_0041605C");
extern void decodemut_DE68(void *) __asm__("decodemut");
/**
 * Decodes up to @p max samples of a fed MicroTalk (UTK) stream into a float buffer and advances the caller's pointer.
 *
 * The decoder (0xD68 bytes, made by CMTBLKDecf_CMTBLKDecf and fed by CMTBLKDecf_Feed) keeps one decoded 432-sample frame at
 * +0x684; +0xD44 counts the samples of it not yet handed out and +0xD54 the samples left in the whole fed block.
 * Returns 0 when +0xD54 is 0. Otherwise copies min(max, +0xD54) floats out (func_0041605C, a memcpy) to *@p dstBuf
 * (+0xD4C is the write cursor), decoding the next frame with decodemut whenever the current one runs out. With
 * codec version 1 (+0xD64, set by CMTBLKDecf_SetCodecVersion) the stream is the revision with raw PCM patches: after each frame the
 * bit reader is backed up one byte, and when the previous frame's marker byte was 0xEE (+0xD60) a big-endian offset
 * (+0xD52) and count (+0xD50) follow, then that many big-endian 16-bit samples that overwrite the decoded frame from
 * that offset; the next byte is checked for 0xEE again and the bit reader restarts after it. Quirk: *@p dstBuf itself is
 * not advanced; the caller adds the returned count.
 *
 * @param decoder  the decoder
 * @param dstBuf  points to the float output pointer (read once into +0xD4C)
 * @param max  the most samples to produce
 * @return the number of samples written (0 at the end of the fed data)
 */
int CMTBLKDecf_Decode(struct CMTBLKDecf *decoder, char **dstBuf, int max) {
    int chunk = ((int)decoder->mbufframes);
    int n, rem;
    if (decoder->mRemainingSamples == 0) {
        return 0;
    }
    n = decoder->mRemainingSamples;
    if (max < n) n = max;
    (*(char**)&decoder->mpDst) = *dstBuf;
    rem = n;
    while (rem > 0) {
        int bytes;
        if (rem < chunk) chunk = rem;
        bytes = chunk * 4;
        rem -= chunk;
        func_0041605C_DE68(((char*)decoder->mpDst), (char *)((0x1B0 - ((int)decoder->mbufframes)) * 4 + (int)decoder) + 0x684, bytes);
        (*(char**)&decoder->mpDst) += bytes;
        (*(int*)&decoder->mbufframes) -= chunk;
        if (rem > 0) {
            decodemut_DE68(decoder);
            decoder->mbufframes = 432;
            chunk = 432;
            if (decoder->mCodecVersion == 1) {
                unsigned char *src = decoder->mutstate.dataptr;
                unsigned char *pcmSrc;
                pcmSrc = decoder->mpSrc = src - 1;
                if (decoder->mHasPCMData != 0) {
                    unsigned char *pi = (unsigned char *)&decoder->mOffset;
                    unsigned char *pn = (unsigned char *)&decoder->mRawframes;
                    float *pcmDst;
                    short j;
                    char swapped[2];
                    pi[0] = pcmSrc[1];
                    pi[1] = pcmSrc[0];
                    pn[0] = src[2];
                    pn[1] = src[1];
                    pcmSrc += 4;
                    decoder->mpSrc = decoder->mpSrc + decoder->mRawframes * 2 + 4;
                    pcmDst = &decoder->mutstate.data[decoder->mOffset];
                    decoder->mutstate.dataptr = decoder->mpSrc + 1;
                    for (j = 0; j < decoder->mRawframes; j++) {
                        swapped[0] = pcmSrc[1];
                        swapped[1] = pcmSrc[0];
                        pcmSrc += 2;
                        *pcmDst++ = (float)*(short *)swapped;
                    }
                    decoder->mHasPCMData = 0;
                }
                if (*decoder->mpSrc == 238) decoder->mHasPCMData = 1;
                decoder->mutstate.bitcount = 8;
                decoder->mutstate.shiftreg = *decoder->mutstate.dataptr;
                decoder->mutstate.dataptr++;
            }
        }
    }
    decoder->mRemainingSamples -= n;
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_GetState);
#ifdef SKIP_ASM

/**
 * Returns a snapshot of a MicroTalk decoder's resumable state.
 *
 * The 12-byte result holds the bit-reader shift register (+0x4), the samples left in the decoded frame (+0xD44) and the
 * whole-init flag (+0xD5C, which makes the next CMTBLKDecf_Feed re-read the codec header). CMTBLKDecf_SetState restores it;
 * SFILTER_unpackmtfinit and SFILTER_unpackmtlfinit take one, set the flag and put it back.
 *
 * @return the snapshot {shift register, frames left, whole-init flag}
 */
struct MTFSTATE CMTBLKDecf_GetState(CMTBLKDecf* self) {
    struct MTFSTATE snapshot;
    snapshot.shiftreg = self->mutstate.shiftreg;
    snapshot.doWholeInit = self->mdoWholeInit;
    snapshot.bufframes = self->mbufframes;
    return snapshot;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_SetState);
#ifdef SKIP_ASM
/**
 * Restores a MicroTalk decoder's resumable state from a snapshot.
 *
 * Copies the shift register to +0x4, the frames-left count to +0xD44 and the whole-init flag to +0xD5C (the layout
 * CMTBLKDecf_GetState returns). The third argument the callers pass (0) is not declared or used here.
 *
 * @param decoder  the decoder
 * @param snapshot  the 12-byte state from CMTBLKDecf_GetState
 */
void CMTBLKDecf_SetState(CMTBLKDecf* decoder, MTFSTATE* snapshot) {
    ((int)decoder->mutstate.shiftreg) = (int) ((int)snapshot->shiftreg);
    decoder->mdoWholeInit = (int) (snapshot->doWholeInit);
    ((int)decoder->mbufframes) = (int) ((int)snapshot->bufframes);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", CMTBLKDecf_SetCodecVersion);
#ifdef SKIP_ASM
/**
 * Sets a MicroTalk decoder's codec version (+0xD64): 1 enables the revision with 0xEE-marked raw PCM patches.
 *
 * SFILTER_unpackmtfinit passes 0 for stream revisions below 3 and 1 otherwise; CMTBLKDecf_CMTBLKDecf defaults it to 1.
 *
 * @param decoder  the decoder
 * @param codecVersion  the codec version (0 or 1)
 */
void CMTBLKDecf_SetCodecVersion(CMTBLKDecf* decoder, int codecVersion) {
    decoder->mCodecVersion = codecVersion;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDDRV_getmastervoice);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { char pad[0x28]; short f; char pad2[0x62]; } R_E0E0;
typedef struct { R_E0E0 arr[100]; } W_E0E0;

/**
 * Returns the master voice of a voice: the channel's link (+0x28) when it has one, else the voice itself.
 *
 * The channel is entry @p voice + sndgs +0x2F + sndgs +0x30 of the driver's channel table (sndgs +0x1F0,
 * 0x8C bytes each); a link of -1 means the voice is its own master. Used by the unpack setup callbacks (SFILTER_unpackmtpfinit,
 * SFILTER_unpackxapfinit) before SNDPKTPLAYI_voicetopackethandle.
 *
 * @param voice  the voice number
 * @return the channel index of the master voice (biased by the two reserved counts)
 */
int SNDDRV_getmastervoice(int voice) {
    char *driverState = ((char*)&sndgs);
    int temp_2;
    int temp_4;

    temp_4 = voice + *(unsigned char *)(driverState + 0x30) + *(unsigned char *)(driverState + 0x2F);
    temp_2 = (*(W_E0E0 **)(driverState + 0x1F0))->arr[temp_4].f;
    return (temp_2 == -1) ? temp_4 : temp_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDDRV_getsamplechan);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

struct R_E118 { struct CHANPUB e[1]; };

int SNDDRV_getsamplechan_impl(int arg0) __asm__("SNDDRV_getsamplechan");
/**
 * Returns which sample channel (1-5) of its master voice a voice is, or 0 when it has no master.
 *
 * Looks up the voice's channel entry (index @p voice + sndgs +0x2F + +0x30 in the table at sndgs +0x1F0); when
 * its master link (+0x28) is set, scans the master's member list (shorts at +0x6..+0xE) for this channel and returns
 * the 1-based slot. Quirk: the master's own channel (slot 0) also returns 0.
 *
 * @param voice  the voice number
 * @return the sample channel within the group, 0 for the master or an unlinked voice
 */
int SNDDRV_getsamplechan_impl(int voice) {
    short *var_3;
    int temp_2;
    int temp_4;
    int var_5;
    char *driverState = ((char*)&sndgs);

    temp_4 = voice + *(unsigned char *)(driverState + 0x30) + *(unsigned char *)(driverState + 0x2F);
    temp_2 = ((struct R_E118 *)*(char **)(driverState + 0x1F0))->e[temp_4].masterchan;
    if (temp_2 == -1) {
        return 0;
    }
    var_3 = (short *)(temp_2 * 0x8C + *(int *)(driverState + 0x1F0) + 6);
    for (var_5 = 1; var_5 < 6; var_5++, var_3++) {
        if (*var_3 == temp_4) {
            return var_5;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_packetoverhead);
#ifdef SKIP_ASM
/**
 * Returns 0: the platform layer needs no extra memory per packet player.
 *
 * Added to the packet player's own overhead by SNDPKTPLAY_overhead and skipped at the start of the player memory by
 * SNDPKTPLAY_create.
 *
 * @return 0
 */
int SNDPLATFORM_packetoverhead(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_packetplaycreate);
#ifdef SKIP_ASM
/**
 * Returns 0: creating a packet player needs no platform work.
 *
 * SNDPKTPLAY_create fails with -6 if it is negative.
 *
 * @return 0
 */
int SNDPLATFORM_packetplaycreate(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_packetplaydestroy);
#ifdef SKIP_ASM
/**
 * Returns 0: destroying a packet player needs no platform work.
 *
 * Called by SNDPKTPLAY_destroy before it clears the player's slot (sndpps +0x304).
 *
 * @return 0
 */
int SNDPLATFORM_packetplaydestroy(void) {
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SNDPLATFORM_packetplay);
#ifdef SKIP_ASM
typedef struct {
    char pad0[0x2F];
    unsigned char f2F;
    unsigned char f30;
    char pad31[0x1F0 - 0x31];
    CHANPUB *ents;
} G_E1B0;
typedef struct { char pad0[3]; unsigned char b3; } A6_E1B0;
typedef struct { char pad0[7]; unsigned char b7; char pad8[0x18 - 8]; int arr[8]; } A7_E1B0;
extern void MIX_playinit_E1B0(int, int, int, int, unsigned int, int, int, int, int, int, int, int, int, int) __asm__("MIX_playinit");
extern void SNDPLATFORM_setpitch_E1B0(int) __asm__("SNDPLATFORM_setpitch");
extern void func_003C38E0_E1B0(int) __asm__("func_003C38E0");
extern void SNDPLATFORM_setfxlevel_E1B0(int, int) __asm__("SNDPLATFORM_setfxlevel");
extern void SNDPLATFORM_timemult_E1B0(int, int) __asm__("SNDPLATFORM_timemult");
extern void func_003C30C8_E1B0(int, int) __asm__("func_003C30C8");
extern void SNDPLATFORM_lowpass_E1B0(int, int) __asm__("SNDPLATFORM_lowpass");
extern void SNDPLATFORM_highpass_E1B0(int, int) __asm__("SNDPLATFORM_highpass");
extern void MIX_play_E1B0(int) __asm__("MIX_play");

/**
 * Starts a packet-player voice on the software mixer: sets up each of its channels, applies the play options and starts
 * them.
 *
 * Only for a mixer voice: the channel entry (sndgs +0x1F0, index @p idx) must lack flag 8 and have flag 4 in
 * +0x24. For each of its +0x23 channels (shorts at +0x4, biased by sndgs +0x2F/+0x30) MIX_playinit prepares the
 * mixer voice from the sample representation (@p sampleFormat +3), the channel's sample pointer (@p sampleAttr +0x18 + 4*i), the
 * channel count, +0x14 and the attribute byte @p sampleAttr +7. Then, on the first channel, SNDPLATFORM_setpitch (pitch),
 * func_003C38E0 (azimuth), SNDPLATFORM_setfxlevel (send, 0), and SNDPLATFORM_timemult, func_003C30C8, SNDPLATFORM_lowpass and SNDPLATFORM_highpass
 * with the four option values; finally MIX_play starts each channel. The packet handle is unused.
 *
 * @param pktChan  the packet player handle (unused)
 * @param idx  the voice
 * @param timeMult  the time multiplier (play options +0xE)
 * @param arg3  the play option at +0x12
 * @param lowpassCutoff  the low-pass cutoff (play options +0x14)
 * @param highpassCutoff  the high-pass cutoff (play options +0x16)
 * @param sampleFormat  the sample format (+3: sample representation)
 * @param sampleAttr  the sample attributes (+7: platform version, +0x18: per-channel sample data)
 * @return 0
 */
int SNDPLATFORM_packetplay(int pktChan, int idx, int timeMult, int arg3, int lowpassCutoff, int highpassCutoff, A6_E1B0 *sampleFormat, A7_E1B0 *sampleAttr) {
    CHANPUB *chanEntry;
    int i;

    chanEntry = &sndgs.chan[idx];
    if (!(chanEntry->rendermode & 8)) {
    if (chanEntry->rendermode & 4) {
    for (i = 0; i < chanEntry->numchan; i++) {
        MIX_playinit_E1B0(chanEntry->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu), sampleFormat->b3, 0, 0, 0xFFFFFFFF,
                           sampleAttr->arr[i], chanEntry->numchan, ((int)chanEntry->frames), -1, -1, 0, 0, sampleAttr->b7, i);
    }
    SNDPLATFORM_setpitch_E1B0(chanEntry->voices[0]);
    func_003C38E0_E1B0(chanEntry->voices[0]);
    SNDPLATFORM_setfxlevel_E1B0(chanEntry->voices[0], 0);
    SNDPLATFORM_timemult_E1B0(chanEntry->voices[0], timeMult);
    func_003C30C8_E1B0(chanEntry->voices[0], arg3);
    SNDPLATFORM_lowpass_E1B0(chanEntry->voices[0], lowpassCutoff);
    SNDPLATFORM_highpass_E1B0(chanEntry->voices[0], highpassCutoff);
    for (i = 0; i < chanEntry->numchan; i++) {
        MIX_play_E1B0(chanEntry->voices[i] - (sndgs.sso.set.voicesspu + sndgs.sso.set.voicesiopcpu));
    }
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDPKTPLAYI_voicetopackethandle);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).


/* sndpps: the stream queues (a pending-callback list, then one queue per stream). Layout from every use in
   this unit; field names are their offsets. */
extern struct s7B8 sndpps;

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int SNDVOICEI_get_E378(int arg0) __asm__("SNDVOICEI_get");

/**
 * Returns the packet player whose voice is a given channel, or -1.
 *
 * Scans the packet player slots (sndpps +0x304, sndgs setup.cfg.f26 of them) for a live one whose voice handle
 * (+0x0) resolves through SNDVOICEI_get to @p key.
 *
 * @param key  the channel (a master voice from SNDDRV_getmastervoice)
 * @return the packet player handle, -1 if none
 */
int SNDPKTPLAYI_voicetopackethandle(int key) {
    int i;
    for (i = 0; i < sndgs.sso.set.maxstreams; i++) {
        struct s7B8Queue *pktChan = sndpps.f304[i];
        if (pktChan != 0 && SNDVOICEI_get_E378(pktChan->f00) == key) return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CE410);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only hand-written FPU routine (mula.s/madda.s accumulator ops no compiler emits); the whole
// body is one asm block so the PS2 build stays byte-exact. A port replaces this function.
void func_003CE410_asmbody(void) __asm__("func_003CE410");
void func_003CE410_asmbody(void) {
    __asm__ volatile(
        ".set push\n.set noreorder\n.set noat\n"
        "lwc1 $f0, 0x0($5)\n"
        "lwc1 $f1, 0x4($5)\n"
        "lwc1 $f2, 0x8($5)\n"
        "lwc1 $f3, 0xC($5)\n"
        "lwc1 $f4, 0x10($5)\n"
        "lwc1 $f5, 0x14($5)\n"
        "lwc1 $f6, 0x18($5)\n"
        "lwc1 $f7, 0x1C($5)\n"
        "lwc1 $f8, 0x20($5)\n"
        "lwc1 $f9, 0x24($5)\n"
        "lwc1 $f10, 0x28($5)\n"
        "lwc1 $f11, 0x2C($5)\n"
        "mtc1 $0, $f31\n"
        "lwc1 $f12, 0x0($6)\n"
        "lwc1 $f13, 0x4($6)\n"
        "lwc1 $f14, 0x8($6)\n"
        "lwc1 $f15, 0xC($6)\n"
        "lwc1 $f16, 0x10($6)\n"
        "lwc1 $f17, 0x14($6)\n"
        "lwc1 $f18, 0x18($6)\n"
        "lwc1 $f19, 0x1C($6)\n"
        "lwc1 $f20, 0x20($6)\n"
        "lwc1 $f21, 0x24($6)\n"
        "lwc1 $f22, 0x28($6)\n"
        "lwc1 $f23, 0x2C($6)\n"
        "1:\n"
        "lwc1 $f30, 0x0($7)\n"
        "adda.s $f30, $f31\n"
        "madda.s $f0, $f12\n"
        "madda.s $f1, $f13\n"
        "madda.s $f2, $f14\n"
        "madda.s $f3, $f15\n"
        "addi $4, $4, -0x1\n"
        "madda.s $f4, $f16\n"
        "madda.s $f5, $f17\n"
        "madda.s $f6, $f18\n"
        "madda.s $f7, $f19\n"
        "lwc1 $f30, 0x4($7)\n"
        "madda.s $f8, $f20\n"
        "madda.s $f9, $f21\n"
        "madda.s $f10, $f22\n"
        "madda.s $f11, $f23\n"
        "madd.s $f23, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f12\n"
        "madda.s $f2, $f13\n"
        "madda.s $f3, $f14\n"
        "madda.s $f4, $f15\n"
        "swc1 $f23, 0x0($7)\n"
        "madda.s $f5, $f16\n"
        "madda.s $f6, $f17\n"
        "madda.s $f7, $f18\n"
        "madda.s $f8, $f19\n"
        "lwc1 $f30, 0x8($7)\n"
        "madda.s $f9, $f20\n"
        "madda.s $f10, $f21\n"
        "madda.s $f11, $f22\n"
        "madda.s $f0, $f23\n"
        "madd.s $f22, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f23\n"
        "madda.s $f2, $f12\n"
        "madda.s $f3, $f13\n"
        "madda.s $f4, $f14\n"
        "swc1 $f22, 0x4($7)\n"
        "madda.s $f5, $f15\n"
        "madda.s $f6, $f16\n"
        "madda.s $f7, $f17\n"
        "madda.s $f8, $f18\n"
        "lwc1 $f30, 0xC($7)\n"
        "madda.s $f9, $f19\n"
        "madda.s $f10, $f20\n"
        "madda.s $f11, $f21\n"
        "madda.s $f0, $f22\n"
        "madd.s $f21, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f22\n"
        "madda.s $f2, $f23\n"
        "madda.s $f3, $f12\n"
        "madda.s $f4, $f13\n"
        "swc1 $f21, 0x8($7)\n"
        "madda.s $f5, $f14\n"
        "madda.s $f6, $f15\n"
        "madda.s $f7, $f16\n"
        "madda.s $f8, $f17\n"
        "lwc1 $f30, 0x10($7)\n"
        "madda.s $f9, $f18\n"
        "madda.s $f10, $f19\n"
        "madda.s $f11, $f20\n"
        "madda.s $f0, $f21\n"
        "madd.s $f20, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f21\n"
        "madda.s $f2, $f22\n"
        "madda.s $f3, $f23\n"
        "swc1 $f20, 0xC($7)\n"
        "madda.s $f4, $f12\n"
        "madda.s $f5, $f13\n"
        "madda.s $f6, $f14\n"
        "madda.s $f7, $f15\n"
        "madda.s $f8, $f16\n"
        "lwc1 $f30, 0x14($7)\n"
        "madda.s $f9, $f17\n"
        "madda.s $f10, $f18\n"
        "madda.s $f11, $f19\n"
        "madda.s $f0, $f20\n"
        "madd.s $f19, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f20\n"
        "madda.s $f2, $f21\n"
        "madda.s $f3, $f22\n"
        "madda.s $f4, $f23\n"
        "swc1 $f19, 0x10($7)\n"
        "madda.s $f5, $f12\n"
        "madda.s $f6, $f13\n"
        "madda.s $f7, $f14\n"
        "madda.s $f8, $f15\n"
        "lwc1 $f30, 0x18($7)\n"
        "madda.s $f9, $f16\n"
        "madda.s $f10, $f17\n"
        "madda.s $f11, $f18\n"
        "madda.s $f0, $f19\n"
        "madd.s $f18, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f19\n"
        "madda.s $f2, $f20\n"
        "madda.s $f3, $f21\n"
        "madda.s $f4, $f22\n"
        "swc1 $f18, 0x14($7)\n"
        "madda.s $f5, $f23\n"
        "madda.s $f6, $f12\n"
        "madda.s $f7, $f13\n"
        "madda.s $f8, $f14\n"
        "lwc1 $f30, 0x1C($7)\n"
        "madda.s $f9, $f15\n"
        "madda.s $f10, $f16\n"
        "madda.s $f11, $f17\n"
        "madda.s $f0, $f18\n"
        "madd.s $f17, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f18\n"
        "madda.s $f2, $f19\n"
        "madda.s $f3, $f20\n"
        "madda.s $f4, $f21\n"
        "swc1 $f17, 0x18($7)\n"
        "madda.s $f5, $f22\n"
        "madda.s $f6, $f23\n"
        "madda.s $f7, $f12\n"
        "madda.s $f8, $f13\n"
        "lwc1 $f30, 0x20($7)\n"
        "madda.s $f9, $f14\n"
        "madda.s $f10, $f15\n"
        "madda.s $f11, $f16\n"
        "madda.s $f0, $f17\n"
        "madd.s $f16, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f17\n"
        "madda.s $f2, $f18\n"
        "madda.s $f3, $f19\n"
        "madda.s $f4, $f20\n"
        "swc1 $f16, 0x1C($7)\n"
        "madda.s $f5, $f21\n"
        "madda.s $f6, $f22\n"
        "madda.s $f7, $f23\n"
        "madda.s $f8, $f12\n"
        "lwc1 $f30, 0x24($7)\n"
        "madda.s $f9, $f13\n"
        "madda.s $f10, $f14\n"
        "madda.s $f11, $f15\n"
        "madda.s $f0, $f16\n"
        "madd.s $f15, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f16\n"
        "madda.s $f2, $f17\n"
        "madda.s $f3, $f18\n"
        "madda.s $f4, $f19\n"
        "swc1 $f15, 0x20($7)\n"
        "madda.s $f5, $f20\n"
        "madda.s $f6, $f21\n"
        "madda.s $f7, $f22\n"
        "madda.s $f8, $f23\n"
        "lwc1 $f30, 0x28($7)\n"
        "madda.s $f9, $f12\n"
        "madda.s $f10, $f13\n"
        "madda.s $f11, $f14\n"
        "madda.s $f0, $f15\n"
        "madd.s $f14, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f15\n"
        "madda.s $f2, $f16\n"
        "madda.s $f3, $f17\n"
        "madda.s $f4, $f18\n"
        "swc1 $f14, 0x24($7)\n"
        "madda.s $f5, $f19\n"
        "madda.s $f6, $f20\n"
        "madda.s $f7, $f21\n"
        "madda.s $f8, $f22\n"
        "lwc1 $f30, 0x2C($7)\n"
        "madda.s $f9, $f23\n"
        "madda.s $f10, $f12\n"
        "madda.s $f11, $f13\n"
        "madda.s $f0, $f14\n"
        "madd.s $f13, $f31, $f31\n"
        "adda.s $f30, $f31\n"
        "madda.s $f1, $f14\n"
        "madda.s $f2, $f15\n"
        "madda.s $f3, $f16\n"
        "madda.s $f4, $f17\n"
        "swc1 $f13, 0x28($7)\n"
        "madda.s $f5, $f18\n"
        "madda.s $f6, $f19\n"
        "madda.s $f7, $f20\n"
        "madda.s $f8, $f21\n"
        "addi $7, $7, 0x30\n"
        "madda.s $f9, $f22\n"
        "madda.s $f10, $f23\n"
        "madda.s $f11, $f12\n"
        "madda.s $f0, $f13\n"
        "madd.s $f12, $f31, $f31\n"
        "swc1 $f12, -0x4($7)\n"
        "bgtzl $4, 1b\n"
        "nop\n"
        "swc1 $f12, 0x0($6)\n"
        "swc1 $f13, 0x4($6)\n"
        "swc1 $f14, 0x8($6)\n"
        "swc1 $f15, 0xC($6)\n"
        "swc1 $f16, 0x10($6)\n"
        "swc1 $f17, 0x14($6)\n"
        "swc1 $f18, 0x18($6)\n"
        "swc1 $f19, 0x1C($6)\n"
        "swc1 $f20, 0x20($6)\n"
        "swc1 $f21, 0x24($6)\n"
        "swc1 $f22, 0x28($6)\n"
        "swc1 $f23, 0x2C($6)\n"
        ".set pop\n"
        : : : "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", calcFIRCoeffs);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct {
    float m[0x38];
    float c[5];
    float f1;
    float f2;
    char dirty;
} sFir_E7C0;

/**
 * Computes the 5 coefficients of a symmetric windowed-sinc FIR filter of the given type and marks the filter dirty.
 *
 * Type 2 is a low-pass with normalised cutoff +0xF8, type 3 a high-pass at +0xF4 (clamped to 0.8, as for type 4), type
 * 4 a band-pass between them. Each coefficient c[k] (+0xE0) is sin(pi*i*f)/(pi*i) (SNDI_sin, SNDI_cos for the
 * band-pass), shaped by a Hamming window 0.54 - 0.46cos(i*pi/4); the high-pass spectrally inverts the low-pass. The
 * coefficients are then divided by the absolute gain of the 9-tap symmetric kernel (the high-pass by its alternating
 * sum). Sets +0xFC (dirty) first. Its callers SFILTER_modifyHPFFIR8, SFILTER_modifyLPFFIR8 and SFILTER_modifyBPFFIR8 set the cutoffs from
 * fixed-point ratios and pass 3, 2 or 4 respectively.
 *
 * @param firState  the filter state (+0xE0 coefficients, +0xF4 low cutoff, +0xF8 high cutoff, +0xFC dirty)
 * @param type  2 low-pass, 3 high-pass, 4 band-pass
 */
void calcFIRCoeffs(sFir_E7C0 *firState, int type) {
    /* order stays in $19: the target tests it with slti/beqz and folds order & 1 only via nonzero_bits */
    int order = 4;
    int i;
    float angle, sum;

    firState->dirty = 1;
    if ((unsigned)(type - 3) < 2 && firState->f1 > 0.8f) {
        firState->f1 = 0.8f;
    }
    switch (type) {
    case 2:
        firState->c[order] = firState->f2;
        for (i = 1; i <= order; i++) {
            angle = (float)i * 3.1415927f;
            firState->c[order - i] = SNDI_sin(firState->f2 * angle) / angle;
        }
        break;
    case 3:
        firState->c[order] = firState->f1;
        for (i = 1; i <= order; i++) {
            angle = (float)i * 3.1415927f;
            firState->c[order - i] = SNDI_sin(firState->f1 * angle) / angle;
        }
        break;
    case 4:
        firState->c[order] = firState->f2 - firState->f1;
        for (i = 1; i <= order; i++) {
            float sinVal;
            angle = (float)i * 3.1415927f;
            sinVal = SNDI_sin((firState->f2 - firState->f1) * 0.5f * angle);
            firState->c[order - i] = (sinVal + sinVal) * SNDI_cos((firState->f2 + firState->f1) * 0.5f * angle) / angle;
        }
        break;
    }
    /* window step pi/4 is set at the switch join before the order < 0 loop guard (a literal is hoisted after it) */
    angle = 0.7853982f;
    sum = 0.0f;
    for (i = 0; i <= order; i++) {
        if (type == 2 || type == 4) {
            float windowed = firState->c[i] * (0.54f - SNDI_cos(angle * (float)i) * 0.46f);
            firState->c[i] = windowed;
            sum += windowed;
        } else if (type == 3) {
            float windowed = firState->c[i] * -(0.54f - SNDI_cos(angle * (float)i) * 0.46f);
            firState->c[i] = windowed;
            if ((i & 1) == 0) {
                sum += windowed;
            } else {
                sum -= windowed;
            }
        }
    }
    if (type == 3) {
        sum += 1.0f;
        firState->c[order] += 1.0f;
    }
    if (order & 1) {
        sum = sum + sum;
    } else {
        sum = sum + sum - firState->c[order];
    }
    if (sum < 0.0f) {
        sum = -sum;
    }
    for (i = 0; i <= order; i++) {
        firState->c[i] /= sum;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_fir8init);
#ifdef SKIP_ASM
/**
 * Resets an FIR filter state: clears its 8-sample history and the always-zero entries of its coefficient matrix.
 *
 * The state (16-byte aligned, +0x20 inside a filter node) holds a 12x4 float matrix at +0x0 that SNDI_fir8 fills
 * from the 5 coefficients, and the last 8 input samples at +0xC0..+0xDC. This zeroes the history and the matrix cells
 * outside the band (+0x4..+0xC, +0x18, +0x1C, +0x2C, +0x90, +0xA0, +0xA4, +0xB0..+0xB8). Called by the FIR node
 * constructors SFILTER_createHPFFIR8, SFILTER_createLPFFIR8 and SFILTER_createBPFFIR8.
 *
 * @param firState  the FIR state
 */
void SNDI_fir8init(SNDFIRSTATE* firState) {
    (*(int *)((char*)(firState) + (0xC0))) = 0;
    (*(int *)((char*)(firState) + (0xC4))) = 0;
    (*(int *)((char*)(firState) + (0xC8))) = 0;
    (*(int *)((char*)(firState) + (0xCC))) = 0;
    (*(int *)((char*)(firState) + (0xD0))) = 0;
    (*(int *)((char*)(firState) + (0xD4))) = 0;
    (*(int *)((char*)(firState) + (0xD8))) = 0;
    (*(int *)((char*)(firState) + (0xDC))) = 0;
    (*(int *)((char*)(firState) + (4))) = 0;
    (*(int *)((char*)(firState) + (8))) = 0;
    (*(int *)((char*)(firState) + (0xC))) = 0;
    (*(int *)((char*)(firState) + (0x18))) = 0;
    (*(int *)((char*)(firState) + (0x1C))) = 0;
    (*(int *)((char*)(firState) + (0x2C))) = 0;
    (*(int *)((char*)(firState) + (0x90))) = 0;
    (*(int *)((char*)(firState) + (0xA0))) = 0;
    (*(int *)((char*)(firState) + (0xA4))) = 0;
    (*(int *)((char*)(firState) + (0xB0))) = 0;
    (*(int *)((char*)(firState) + (0xB4))) = 0;
    (*(int *)((char*)(firState) + (0xB8))) = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_fir8);
#ifdef SKIP_ASM
/**
 * Runs a symmetric 9-tap FIR filter over @p n float samples from @p src to @p dst on VU0, 16 samples per pass.
 *
 * When the dirty flag (+0xFC) is set it first expands the 5 coefficients (+0xE0, from calcFIRCoeffs) into the banded
 * 12x4 matrix at +0x0 and clears the flag. The VU0 macro-mode loop then turns each quad of new samples, with the 8
 * previous ones (history at +0xC0), into 4 outputs, and stores the last 8 inputs back into the history. Quirk: @p n
 * must be a positive multiple of 16; the loop only stops when the source pointer lands exactly on the end.
 *
 * @param filter  the FIR state (16-byte aligned)
 * @param n  the number of samples
 * @param src  the input floats (16-byte aligned)
 * @param dst  the output floats (16-byte aligned)
 */
// PORT: PS2-only VU0 inline asm (FIR filter over 4-float blocks: 12x4 banded matrix times sliding window).
void SNDI_fir8(SNDFIRSTATE* filter, int n, int src, int dst) {
    if (*(signed char*)&filter->unk_FC != 0) {
        float *matrix = (float *)filter;
        float coef0 = matrix[0xE0 / 4];
        float coef1 = matrix[228 / 4];
        float coef2 = matrix[232 / 4];
        float coef3 = matrix[236 / 4];
        float coef4 = matrix[0xF0 / 4];
        matrix[0] = coef0;
        matrix[5] = coef0;
        matrix[10] = coef0;
        matrix[15] = coef0;
        matrix[4] = coef1;
        matrix[9] = coef1;
        matrix[14] = coef1;
        matrix[19] = coef1;
        matrix[8] = coef2;
        matrix[13] = coef2;
        matrix[18] = coef2;
        matrix[23] = coef2;
        matrix[12] = coef3;
        matrix[17] = coef3;
        matrix[22] = coef3;
        matrix[27] = coef3;
        matrix[16] = coef4;
        matrix[21] = coef4;
        matrix[26] = coef4;
        matrix[31] = coef4;
        matrix[20] = coef3;
        matrix[25] = coef3;
        matrix[30] = coef3;
        matrix[35] = coef3;
        matrix[24] = coef2;
        matrix[29] = coef2;
        matrix[34] = coef2;
        matrix[39] = coef2;
        matrix[28] = coef1;
        matrix[33] = coef1;
        matrix[38] = coef1;
        matrix[43] = coef1;
        matrix[32] = coef0;
        matrix[37] = coef0;
        matrix[42] = coef0;
        matrix[47] = coef0;
        *(signed char*)&filter->unk_FC = 0;
    }
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "move         $6, %1\n"
        "move         $4, %0\n"
        "move         $5, %2\n"
        "sll          $6, $6, 2\n"
        "move         $7, %3\n"
        "move         $8, %4\n"
        "add          $6, $6, $7\n"
        "lqc2         $vf1, 0x0($4)\n"
        "lqc2         $vf2, 0x10($4)\n"
        "lqc2         $vf3, 0x20($4)\n"
        "lqc2         $vf4, 0x30($4)\n"
        "lqc2         $vf5, 0x40($4)\n"
        "lqc2         $vf6, 0x50($4)\n"
        "lqc2         $vf7, 0x60($4)\n"
        "lqc2         $vf8, 0x70($4)\n"
        "lqc2         $vf9, 0x80($4)\n"
        "lqc2         $vf10, 0x90($4)\n"
        "lqc2         $vf11, 0xA0($4)\n"
        "lqc2         $vf12, 0xB0($4)\n"
        "lqc2         $vf20, 0x0($5)\n"
        "lqc2         $vf21, 0x10($5)\n"
        "1:\n"
        "lqc2         $vf22, 0x0($7)\n"
        "lqc2         $vf23, 0x10($7)\n"
        "lqc2         $vf24, 0x20($7)\n"
        "lqc2         $vf25, 0x30($7)\n"
        "vmulax.xyzw  ACC, $vf1, $vf20x\n"
        "vmadday.xyzw ACC, $vf2, $vf20y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf20z\n"
        "vmaddaw.xyzw ACC, $vf4, $vf20w\n"
        "vmaddax.xyzw ACC, $vf5, $vf21x\n"
        "vmadday.xyzw ACC, $vf6, $vf21y\n"
        "vmaddaz.xyzw ACC, $vf7, $vf21z\n"
        "vmaddaw.xyzw ACC, $vf8, $vf21w\n"
        "vmaddax.xyzw ACC, $vf9, $vf22x\n"
        "vmadday.xyzw ACC, $vf10, $vf22y\n"
        "vmaddaz.xyzw ACC, $vf11, $vf22z\n"
        "vmaddw.xyzw  $vf28, $vf12, $vf22w\n"
        "vmulax.xyzw  ACC, $vf1, $vf21x\n"
        "vmadday.xyzw ACC, $vf2, $vf21y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf21z\n"
        "vmaddaw.xyzw ACC, $vf4, $vf21w\n"
        "vmaddax.xyzw ACC, $vf5, $vf22x\n"
        "vmadday.xyzw ACC, $vf6, $vf22y\n"
        "vmaddaz.xyzw ACC, $vf7, $vf22z\n"
        "vmaddaw.xyzw ACC, $vf8, $vf22w\n"
        "vmaddax.xyzw ACC, $vf9, $vf23x\n"
        "vmadday.xyzw ACC, $vf10, $vf23y\n"
        "vmaddaz.xyzw ACC, $vf11, $vf23z\n"
        "vmaddw.xyzw  $vf29, $vf12, $vf23w\n"
        "vmulax.xyzw  ACC, $vf1, $vf22x\n"
        "vmadday.xyzw ACC, $vf2, $vf22y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf22z\n"
        "vmaddaw.xyzw ACC, $vf4, $vf22w\n"
        "vmaddax.xyzw ACC, $vf5, $vf23x\n"
        "vmadday.xyzw ACC, $vf6, $vf23y\n"
        "vmaddaz.xyzw ACC, $vf7, $vf23z\n"
        "vmaddaw.xyzw ACC, $vf8, $vf23w\n"
        "vmaddax.xyzw ACC, $vf9, $vf24x\n"
        "vmadday.xyzw ACC, $vf10, $vf24y\n"
        "vmaddaz.xyzw ACC, $vf11, $vf24z\n"
        "vmaddw.xyzw  $vf30, $vf12, $vf24w\n"
        "vmulax.xyzw  ACC, $vf1, $vf23x\n"
        "vmadday.xyzw ACC, $vf2, $vf23y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf23z\n"
        "vmaddaw.xyzw ACC, $vf4, $vf23w\n"
        "vmaddax.xyzw ACC, $vf5, $vf24x\n"
        "vmadday.xyzw ACC, $vf6, $vf24y\n"
        "vmaddaz.xyzw ACC, $vf7, $vf24z\n"
        "vmaddaw.xyzw ACC, $vf8, $vf24w\n"
        "vmaddax.xyzw ACC, $vf9, $vf25x\n"
        "vmadday.xyzw ACC, $vf10, $vf25y\n"
        "vmaddaz.xyzw ACC, $vf11, $vf25z\n"
        "vmaddw.xyzw  $vf31, $vf12, $vf25w\n"
        "addi         $7, $7, 0x40\n"
        "sqc2         $vf28, 0x0($8)\n"
        "sqc2         $vf29, 0x10($8)\n"
        "sqc2         $vf30, 0x20($8)\n"
        "sqc2         $vf31, 0x30($8)\n"
        "vmove.xyzw   $vf20, $vf24\n"
        "vmove.xyzw   $vf21, $vf25\n"
        "bne          $6, $7, 1b\n"
        "addi         $8, $8, 0x40\n"
        "sqc2         $vf20, 0x0($5)\n"
        "sqc2         $vf21, 0x10($5)\n"
        ".set pop\n"
        :
        : "r"(((char*)filter)), "r"(n), "r"(((char*)filter) + 0xC0), "r"(src), "r"(dst)
        : "$4", "$5", "$6", "$7", "$8", "memory");
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_lpfFIR8);
#ifdef SKIP_ASM
extern void SNDI_fir8(SNDFIRSTATE*, int, int, int);
/**
 * Filter-graph callback of the low-pass FIR node: pulls @p count frames from its input node and low-passes them.
 *
 * The input node (+0x8) is called through its first word as (node, count, dst, src, requester +0x1A), which writes into
 * @p src; when there is no input node, or it returned more than 0, SNDI_fir8 filters @p count frames from
 * @p src to @p dst with the node's FIR state (+0x20). Quirk: unlike SFILTER_bpfFIR8 and SFILTER_hpfFIR8 it filters and
 * returns the full @p count even when the input produced fewer frames (but more than 0). Installed by SFILTER_createLPFFIR8.
 *
 * @param node  the low-pass filter node
 * @param count  the number of frames wanted
 * @param src  the buffer the input fills
 * @param dst  the output buffer
 * @return @p count, or the input's result when it is 0 or less
 */
int SFILTER_lpfFIR8(LPFSTATE* node, int count, int src, int dst) {
    int **obj = (int**)node->sfn.pfnnext;
    int result;
    if (obj == 0 || (result = ((int (*)(int **, int, int, int, unsigned char))(*obj))(obj, count, dst, src, node->sfn.requester), result > 0)) {
        SNDI_fir8((SNDFIRSTATE*)(&node->fir), count, src, dst);
        result = count;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createLPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_EE88;
extern void SNDI_fir8init(SNDFIRSTATE*);
extern void SFILTER_lpfFIR8_x(void) __asm__("SFILTER_lpfFIR8");

/**
 * Initialises a low-pass FIR filter node: installs SFILTER_lpfFIR8 as its filter function and resets it.
 *
 * Clears the restore function (+0x4), the four links (+0x8..+0x14) and the clean flag (+0x1B), then resets the FIR
 * state at +0x20 with SNDI_fir8init. func_003CC070 calls it for effect type 2, followed by SFILTER_modifyLPFFIR8.
 *
 * @return 0
 */
int SFILTER_createLPFFIR8(S_EE88 *self) {
    self->vt = SFILTER_lpfFIR8_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    SNDI_fir8init((SNDFIRSTATE*)(&self->sub));
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyLPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void calcFIRCoeffs_A9D8(void *, int) __asm__("calcFIRCoeffs");
/**
 * Sets a low-pass FIR node's cutoff from fixed-point parameters and recomputes its coefficients.
 *
 * The cutoff (+0x118, the FIR state's high cutoff) becomes 2 * (params[0] >> 8) / (params[1] >> 8), the cutoff
 * frequency over the sample rate (func_003CC070 passes freq << 8 and rate << 8); then calcFIRCoeffs recomputes the
 * state at +0x20 as a low-pass (type 2).
 *
 * @param params  [0] cutoff frequency << 8, [1] sample rate << 8
 */
void SFILTER_modifyLPFFIR8(LPFSTATE* self, int *params) {
    self->fir.cutHigh = ((float)(params[0] >> 8) * 2.0f) / (float)(params[1] >> 8);
    calcFIRCoeffs_A9D8(&self->fir, 2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_bpfFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Identical copy of SFILTER_hpfFIR8.
extern void SNDI_fir8_EF20(void *, int, int, int) __asm__("SNDI_fir8");

/**
 * Filter-graph callback of the band-pass FIR node: pulls up to @p count frames from its input node and filters them.
 *
 * The input node (+0x8) is called as (node, count, dst, src, requester +0x1A) and fills @p src; its result (or
 * @p count without an input) is how many frames SNDI_fir8 filters into @p dst. A byte-for-byte copy of
 * SFILTER_hpfFIR8. Installed by SFILTER_createBPFFIR8.
 *
 * @param node  the band-pass filter node
 * @param count  the number of frames wanted
 * @param src  the buffer the input fills
 * @param dst  the output buffer
 * @return the frames produced, or the input's result when it is 0 or less
 */
int SFILTER_bpfFIR8(BPFSTATE* node, int count, int src, int dst) {
    int (**obj)(void *, int, int, int, int);
    int n;
    obj = (int (**)(void*, int, int, int, int))node->sfn.pfnnext;
    if (obj != 0) {
        n = (*obj)(obj, count, dst, src, node->sfn.requester);
        if (n <= 0) {
            return n;
        }
    } else {
        n = count;
    }
    SNDI_fir8_EF20(&node->fir, n, src, dst);
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createBPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; int f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int pad1c; char sub[1]; } S_EFB0;
extern void SNDI_fir8init(SNDFIRSTATE*);
extern void SFILTER_bpfFIR8_x(void) __asm__("SFILTER_bpfFIR8");

/**
 * Initialises a band-pass FIR filter node: installs SFILTER_bpfFIR8 as its filter function and resets it.
 *
 * Clears the restore function (+0x4), the links (+0x8..+0x14) and the clean flag (+0x1B), then resets the FIR state at
 * +0x20 with SNDI_fir8init. func_003CC070 calls it for effect type 4, followed by SFILTER_modifyBPFFIR8.
 *
 * @return 0
 */
int SFILTER_createBPFFIR8(S_EFB0 *self) {
    self->vt = SFILTER_bpfFIR8_x;
    self->f8 = 0;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f4 = 0;
    SNDI_fir8init((SNDFIRSTATE*)(&self->sub));
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyBPFFIR8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void calcFIRCoeffs_A9D8(void *, int) __asm__("calcFIRCoeffs");
/**
 * Sets a band-pass FIR node's two cutoffs from fixed-point parameters and recomputes its coefficients.
 *
 * Low cutoff (+0x114) = (params[0] >> 7) / (params[2] >> 8) and high cutoff (+0x118) = (params[1] >> 7) / (params[2] >>
 * 8), i.e. twice each frequency over the sample rate; then calcFIRCoeffs recomputes the state at +0x20 as a band-pass
 * (type 4).
 *
 * @param params  [0] low frequency << 8, [1] high frequency << 8, [2] sample rate << 8
 */
void SFILTER_modifyBPFFIR8(BPFSTATE* self, int *params) {
    float x = (float)(params[0] >> 7), y = (float)(params[1] >> 7), divisor = (float)(params[2] >> 8);
    self->fir.cutLow = x / divisor;
    self->fir.cutHigh = y / divisor;
    calcFIRCoeffs_A9D8(&self->fir, 4);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_amplf);
#ifdef SKIP_ASM
int SFILTER_amplf_C(struct AMPLFSTATE *self, int n, float *src, float *dst) __asm__("SFILTER_amplf");
/**
 * Filter-graph callback of the gain node: pulls @p n frames from its input node and scales them by the node's gain.
 *
 * The input node (+0x8) is called as (node, n, dst, src, requester +0x1A) and fills @p src; a result of 0 or less is
 * returned as is. Then a VU0 loop writes dst[k] = src[k] * gain (+0x1C), 16 floats per pass. Quirk: it scales the full
 * @p n even when the input produced fewer frames, and @p n must be a multiple of 16. Installed by SFILTER_createAMPLF.
 *
 * @param n  the number of frames
 * @param src  the buffer the input fills
 * @param dst  the output buffer
 * @return @p n, or the input's result when it is 0 or less
 */
// PORT: hand-written VU0 loop (trapping add/addi have no gcc 2.95 pattern; the block moves its operands into fixed
// $4/$6/$7 and the scale through $2 itself): dst[k] = src[k] * self->f1C over n floats, 16 per pass. A port replaces it
// with the C loop.
int SFILTER_amplf_C(struct AMPLFSTATE *self, int n, float *src, float *dst) {
    if (((int*)self->sfn.pfnnext)) {
        int r = ((int (*)(int *, int, float *, float *, int))*((int*)self->sfn.pfnnext))(((int*)self->sfn.pfnnext), n, dst, src, self->sfn.requester);
        if (r <= 0) return r;
    }
    // PORT: PS2-only VU0 hand-written loop (scale 4x4 blocks by a factor); one inline asm block.
    __asm__ volatile(
        ".set push\n\t"
        ".set noreorder\n\t"
        ".set noat\n\t"
        "mfc1 $2, %3\n\t"
        "move $4, %0\n\t"
        "qmtc2.ni $2, $vf1\n\t"
        "sll $4, $4, 2\n\t"
        "move $6, %1\n\t"
        "move $7, %2\n\t"
        "add $4, $4, $6\n"
        "1:\n\t"
        "lqc2 $vf2, 0x0($6)\n\t"
        "lqc2 $vf3, 0x10($6)\n\t"
        "lqc2 $vf4, 0x20($6)\n\t"
        "lqc2 $vf5, 0x30($6)\n\t"
        "addi $6, $6, 0x40\n\t"
        "vmulx.xyzw $vf2, $vf2, $vf1x\n\t"
        "vmulx.xyzw $vf3, $vf3, $vf1x\n\t"
        "vmulx.xyzw $vf4, $vf4, $vf1x\n\t"
        "vmulx.xyzw $vf5, $vf5, $vf1x\n\t"
        "sqc2 $vf2, 0x0($7)\n\t"
        "sqc2 $vf3, 0x10($7)\n\t"
        "sqc2 $vf4, 0x20($7)\n\t"
        "sqc2 $vf5, 0x30($7)\n\t"
        "bne $4, $6, 1b\n\t"
        "addi $7, $7, 0x40\n\t"
        ".set pop\n"
        : : "r"(n), "r"(src), "r"(dst), "f"(self->gain)
        : "$2", "$4", "$6", "$7", "memory");
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createAMPLF);
#ifdef SKIP_ASM
extern void SFILTER_amplf();

/**
 * Initialises a gain filter node: installs SFILTER_amplf as its filter function, clears the links and the gain.
 *
 * Clears the restore function (+0x4), the links (+0x8..+0x14), the clean flag (+0x1B) and the gain (+0x1C).
 * func_003CC070 calls it for effect type 6, followed by SFILTER_modifyAMPLF.
 *
 * @param node  the node
 * @return 0
 */
int SFILTER_createAMPLF(AMPLFSTATE* node) {
    node->sfn.pfnnext = 0;
    (*(void (**)())&node->sfn) = SFILTER_amplf;
    node->sfn.pfnnext2 = 0;
    node->sfn.pfnprev1 = 0;
    node->sfn.pfnprev2 = 0;
    (*(signed char*)&node->sfn.clean) = 0;
    node->sfn.restorefn = 0;
    (*(int*)&node->gain) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyAMPLF);
#ifdef SKIP_ASM
/**
 * Sets a gain filter node's gain (+0x1C) to params[0] / 256.
 *
 * @param node  the gain node
 * @param params  [0] the gain in 24.8 fixed point
 */
void SFILTER_modifyAMPLF(AMPLFSTATE* node, int *params) {
    node->gain = (float) ((float) *params * 0.00390625f);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_mixer);
#ifdef SKIP_ASM
extern void ff_F178(void *) __asm__("SNDMEMI_free");
extern void *fa_F178(int) __asm__("SNDMEMI_alloc");
int SFILTER_mixer_C(struct MIXERSTATE *self, int n, int a2, float *a3) __asm__("SFILTER_mixer");
/**
 * Filter-graph callback of the mixer node: sums its two input nodes into @p dstBuf.
 *
 * First grows the scratch buffer (+0x1C, capacity +0x20 frames) to @p n floats (SNDMEMI_free, SNDMEMI_alloc). The first
 * input (+0x8) is called with requester 1 and writes @p n frames into @p dstBuf; the second (+0xC) with requester 2 into
 * the scratch buffer; a result of 0 or less from either is returned at once. A VU0 loop then adds the scratch buffer
 * into @p dstBuf, 16 floats per pass. Installed by SFILTER_createMIX; SFILTER_mixerrestore frees the buffer.
 *
 * @param n  the number of frames
 * @param srcBuf  the source buffer, passed through to both inputs
 * @param dstBuf  the output buffer
 * @return @p n, or an input's result when it is 0 or less
 */
// PORT: hand-written VU0 loop (trapping add/addi have no gcc 2.95 pattern; the block moves its operands into fixed
// $4/$5/$6 itself): a3[k] += self->f1C[k] over n floats, 16 per pass. A port replaces it with the C loop.
int SFILTER_mixer_C(struct MIXERSTATE *self, int n, int srcBuf, float *dstBuf) {
    int inputResult;
    if (self->tempbufframes < n) {
        if (((void*)self->ptempbuf)) ff_F178(((void*)self->ptempbuf));
        self->tempbufframes = n;
        (*(void**)&self->ptempbuf) = fa_F178(n << 2);
    }
    inputResult = ((int (*)(int *, int, int, float *, int))*((int*)self->sfn.pfnnext))(((int*)self->sfn.pfnnext), n, srcBuf, dstBuf, 1);
    if (inputResult <= 0) return inputResult;
    inputResult = ((int (*)(int *, int, int, void *, int))*((int*)self->sfn.pfnnext2))(((int*)self->sfn.pfnnext2), n, srcBuf, ((void*)self->ptempbuf), 2);
    if (inputResult <= 0) return inputResult;
    {
            // PORT: PS2-only VU0 hand-written loop (add a scratch buffer into the destination, 4x4 floats per step).
            __asm__ volatile(
                ".set push\n\t"
                ".set noreorder\n\t"
                ".set noat\n\t"
                "move $4, %0\n\t"
                "sll $4, $4, 2\n\t"
                "move $5, %1\n\t"
                "move $6, %2\n\t"
                "add $4, $4, $5\n"
                "1:\n\t"
                "lqc2 $vf2, 0x0($5)\n\t"
                "lqc2 $vf3, 0x10($5)\n\t"
                "lqc2 $vf4, 0x20($5)\n\t"
                "lqc2 $vf5, 0x30($5)\n\t"
                "lqc2 $vf6, 0x0($6)\n\t"
                "lqc2 $vf7, 0x10($6)\n\t"
                "lqc2 $vf8, 0x20($6)\n\t"
                "lqc2 $vf9, 0x30($6)\n\t"
                "addi $5, $5, 0x40\n\t"
                "vadd.xyzw $vf6, $vf6, $vf2\n\t"
                "vadd.xyzw $vf7, $vf7, $vf3\n\t"
                "vadd.xyzw $vf8, $vf8, $vf4\n\t"
                "vadd.xyzw $vf9, $vf9, $vf5\n\t"
                "sqc2 $vf6, 0x0($6)\n\t"
                "sqc2 $vf7, 0x10($6)\n\t"
                "sqc2 $vf8, 0x20($6)\n\t"
                "sqc2 $vf9, 0x30($6)\n\t"
                "bne $4, $5, 1b\n\t"
                "addi $6, $6, 0x40\n\t"
                ".set pop\n"
                : : "r"(n), "r"(((void*)self->ptempbuf)), "r"(dstBuf)
                : "$4", "$5", "$6", "memory");
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_mixerrestore);
#ifdef SKIP_ASM
extern void SNDMEMI_free(int);

/**
 * Frees a mixer node's scratch buffer (+0x1C) with SNDMEMI_free, if it has one. The node's restore function.
 *
 * Quirk: the pointer and its capacity (+0x20) are not cleared.
 *
 * @param node  the mixer node
 */
void SFILTER_mixerrestore(MIXERSTATE* node) {
    int temp_4;

    temp_4 = (int)((MIXERSTATE *)node)->ptempbuf;
    if (temp_4 != 0) {
        SNDMEMI_free(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createMIX);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_mixer_F2C0(void) __asm__("SFILTER_mixer");
void SFILTER_mixerrestore_F2C0(void) __asm__("SFILTER_mixerrestore");

/**
 * Initialises a mixer filter node: installs SFILTER_mixer as its filter and SFILTER_mixerrestore as its restore function.
 *
 * Clears the links (+0x8..+0x14), the clean flag (+0x1B), the scratch buffer (+0x1C) and its capacity (+0x20).
 * func_003CC070 calls it for effect type 8.
 *
 * @param node  the node
 * @return 0
 */
int SFILTER_createMIX(MIXERSTATE* node) {
    node->sfn.restorefn = (void (*)(void*))SFILTER_mixerrestore_F2C0;
    *(void (**)(void))&node->sfn = SFILTER_mixer_F2C0;
    node->sfn.pfnnext = 0;
    node->sfn.pfnnext2 = 0;
    node->sfn.pfnprev1 = 0;
    node->sfn.pfnprev2 = 0;
    *(signed char*)&node->sfn.clean = 0;
    node->ptempbuf = 0;
    node->tempbufframes = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003CF300);
#ifdef SKIP_ASM
/**
 * Runs a two-pole resonator over @p n float samples, or copies them through when its parameters are out of range.
 *
 * Active when the bandwidth (+0x24) is below 2 * rate / pi and the centre frequency (+0x20) is above 0 and below rate /
 * 2 (rate: +0x1C). Then y = a0 * (x + 1e-20) + 2 * r * cos(theta) * y1 - r * r * y2 with gain a0 (+0x30), pole radius r
 * (+0x28), cos(theta) (+0x2C) and the last two outputs y1 (+0x38) and y2 (+0x3C), which it updates; the 1e-20 keeps
 * denormals away. Called by SFILTER_reson.
 *
 * @param sv  the resonator state
 * @param n  the number of samples
 * @param in_  the input floats
 * @param out_  the output floats
 */
void func_003CF300(void *sv, int n, int in_, int out_) {
    struct RESONSTATE *resonState = (struct RESONSTATE *)sv;
    float *in = (float *)in_;
    float *out = (float *)out_;
    int sampleRate = resonState->samplerate;
    float t, radius, scaledIn, sample;
    if (resonState->bw < (2.0f * (float)sampleRate) / 3.1415927f && (t = resonState->resonfreq, t > 0.0f) && t < (float)(sampleRate >> 1)) {
        if (n > 0) {
            do {
                radius = resonState->r;
                n -= 1;
                scaledIn = resonState->a0 * (*in + 1e-20f);
                in += 1;
                *out = (scaledIn + (2.0f * radius * resonState->costheta * resonState->ynminus1)) - (radius * radius * resonState->ynminus2);
                resonState->ynminus2 = resonState->ynminus1;
                t = *out;
                out += 1;
                resonState->ynminus1 = t;
            } while (n != 0);
        }
    } else if (n > 0) {
        do {
            sample = *in;
            n -= 1;
            in += 1;
            *out = sample;
            out += 1;
        } while (n != 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_reson);
#ifdef SKIP_ASM
extern void func_003CF300(void *, int, int, int);

/**
 * Filter-graph callback of the resonator node: pulls up to @p count frames from its input node and resonates them.
 *
 * The input node (+0x8) is called as (node, count, arg3, arg2, requester +0x1A) and fills @p srcBuf; its result (or
 * @p count without an input) is how many frames func_003CF300 processes into @p dstBuf. Installed by SFILTER_createRESON.
 *
 * @param node  the resonator node
 * @param count  the number of frames wanted
 * @param srcBuf  the buffer the input fills
 * @param dstBuf  the output buffer
 * @return the frames produced, or the input's result when it is 0 or less
 */
int SFILTER_reson(RESONSTATE* node, int count, int srcBuf, int dstBuf) {
    int (**obj)(void *, int, int, int, int);
    int n;
    obj = (int (**)(void *, int, int, int, int))((RESONSTATE *)node)->sfn.pfnnext;
    if (obj != 0) {
        n = (*obj)(obj, count, dstBuf, srcBuf, ((RESONSTATE *)node)->sfn.requester);
        if (n <= 0) {
            return n;
        }
    } else {
        n = count;
    }
    func_003CF300(node, n, srcBuf, dstBuf);
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createRESON);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_reson_F498(void) __asm__("SFILTER_reson");

/**
 * Initialises a resonator filter node: installs SFILTER_reson as its filter function and clears its history.
 *
 * Clears the restore function (+0x4), the links (+0x8..+0x14), the clean flag (+0x1B), the input sample (+0x34) and the
 * two previous outputs (+0x38, +0x3C). func_003CC070 calls it for effect type 5, followed by SFILTER_modifyRESON.
 *
 * @param effect  the node
 * @return 0
 */
int SFILTER_createRESON(RESONSTATE* effect) {
    effect->sfn.pfnnext = 0;
    *(void (**)(void))&effect->sfn = SFILTER_reson_F498;
    effect->sfn.pfnnext2 = 0;
    effect->sfn.pfnprev1 = 0;
    effect->sfn.pfnprev2 = 0;
    *(signed char*)&effect->sfn.clean = 0;
    effect->sfn.restorefn = 0;
    *(int*)&effect->xn = 0;
    *(int*)&effect->ynminus1 = 0;
    *(int*)&effect->ynminus2 = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyRESON);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern float f_C840_F4D0(float) __asm__("SNDI_cos");
extern float f_FA98_F4D0(float) __asm__("SNDI_rootof1plusx");
/**
 * Configures a resonator filter node from fixed-point parameters: centre frequency, sample rate, bandwidth and gain.
 *
 * Stores the rate (params[1] >> 8) at +0x1C, the centre frequency (params[0] >> 8) at +0x20 and the bandwidth
 * (params[2] >> 8) at +0x24. The pole radius r = 1 - bw * pi / rate goes to +0x28, the cosine term 2r / (1 + r^2) *
 * cos(2 * pi * freq / rate) (SNDI_cos) to +0x2C, and the gain (1 - r^2) * (params[3] / 256) * SNDI_rootof1plusx(-c^2)
 * (about sqrt(1 - c^2)) to +0x30. func_003CC070 calls it after SFILTER_createRESON for effect type 5.
 *
 * @param effect  the resonator node
 * @param params  [0] centre frequency << 8, [1] sample rate << 8, [2] bandwidth << 8, [3] gain in 24.8
 */
void SFILTER_modifyRESON(RESONSTATE* effect, void *params) {
    float fa, fb, fc, fd, radiusSq, radius, cosCoef;
    int ia = (*(int *)((char*)params + 0)) >> 8;
    int ib = (*(int *)((char*)params + 4)) >> 8;
    int ic = (*(int *)((char*)params + 8)) >> 8;
    fa = (float)ia;
    fd = (float)(*(int *)((char*)params + 0xC));
    fc = (float)ic;
    fb = (float)ib;
    effect->samplerate = ib;
    effect->resonfreq = fa;
    fd = fd * 0.00390625f;
    effect->bw = fc;
    radius = 1.0f - (fc * 3.1415927f) / fb;
    effect->r = radius;
    radiusSq = radius * radius;
    cosCoef = (2.0f * effect->r) / (radiusSq + 1.0f) * f_C840_F4D0((fa * 6.2831855f) / fb);
    effect->costheta = cosCoef;
    effect->a0 = (1.0f - radiusSq) * fd * f_FA98_F4D0(-cosCoef * cosCoef);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_splitter);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDMEMI_alloc_F5D0(int) __asm__("SNDMEMI_alloc");
extern void SNDMEMI_free_F5D0(int) __asm__("SNDMEMI_free");
extern void Util_MemCpy_F5D0(char *, char *, int) __asm__("Util_MemCpy");

/**
 * Filter-graph callback of the splitter node: feeds one input to two consumers, pulling it once and replaying a copy.
 *
 * If the "must empty" flag (+0x24) is set it is cleared and "is empty" (+0x26) is set. The copy buffer (+0x1C) grows to
 * @p n floats when its capacity (+0x20) is smaller (SNDMEMI_free, SNDMEMI_alloc). When empty, the input node (+0x8) is
 * called as (node, n, dst, workBuf, 1) and fills @p workBuf, which Util_MemCpy copies to @p dst and into the copy
 * buffer, and the flag is cleared; otherwise the copy buffer goes to @p dst and "must empty" is set, so the next call
 * pulls fresh input. Installed by SFILTER_createSPLIT.
 *
 * @param n  the number of frames
 * @param workBuf  the buffer the input fills
 * @param dst  the output buffer
 * @return @p n, or the input's result when it is 0 or less
 */
int SFILTER_splitter(SPLITSTATE* self, int n, char *workBuf, char *dst) {
    int bytes, oldBuf, result;
    int *obj;
    if (self->mustempty != 0) {
        self->mustempty = 0;
        self->isempty = 1;
    }
    bytes = n * 4;
    if (self->sizebuf < n) {
        oldBuf = (int)self->pdatabuf;
        if (oldBuf != 0) {
            SNDMEMI_free_F5D0(oldBuf);
        }
        self->pdatabuf = (void*)SNDMEMI_alloc_F5D0(bytes);
        self->sizebuf = n;
    }
    if (self->isempty != 0) {
        int (**vt)(void *, int, char *, char *, int) = (int (**)(void*, int, char*, char*, int))self->sfn.pfnnext;
        result = (*vt)(vt, n, dst, workBuf, 1);
        if (result <= 0) {
            return result;
        }
        Util_MemCpy_F5D0(dst, workBuf, bytes);
        Util_MemCpy_F5D0((char*)self->pdatabuf, workBuf, bytes);
        self->isempty = 0;
    } else {
        Util_MemCpy_F5D0(dst, (char*)self->pdatabuf, bytes);
        self->mustempty = 1;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_splitrestore);
#ifdef SKIP_ASM
extern void SNDMEMI_free(int);

/**
 * Frees a splitter node's copy buffer (+0x1C) with SNDMEMI_free, if it has one. The node's restore function.
 *
 * Quirk: the pointer and its capacity (+0x20) are not cleared.
 *
 * @param effect  the splitter node
 */
void SFILTER_splitrestore(SPLITSTATE* effect) {
    int temp_4;

    temp_4 = (int)((SPLITSTATE *)effect)->pdatabuf;
    if (temp_4 != 0) {
        SNDMEMI_free(temp_4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createSPLIT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct { void *vt; void *f4; int f8; int fc; int f10; int f14; char pad18[3]; char f1b; int f1c; int f20; short f24; short f26; } S_F6F8;
extern void f_F6F8_a(void) __asm__("SFILTER_splitter");
extern void f_F6F8_b(void) __asm__("SFILTER_splitrestore");

/**
 * Initialises a splitter filter node: installs SFILTER_splitter as its filter and SFILTER_splitrestore as its restore function.
 *
 * Sets "is empty" (+0x26) to 1 and clears the links (+0x8..+0x14), the clean flag (+0x1B), the copy buffer (+0x1C), its
 * capacity (+0x20) and "must empty" (+0x24). func_003CC070 calls it for effect type 9.
 *
 * @return 0
 */
int SFILTER_createSPLIT(S_F6F8 *self) {
self->f26 = 1;
    self->f4 = f_F6F8_b;
    self->vt = f_F6F8_a;
    self->fc = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f1b = 0;
    self->f20 = 0;
    self->f1c = 0;
    self->f24 = 0;
    self->f8 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_src);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void Util_MemCpy_F740(void *, void *, int) __asm__("Util_MemCpy");

/**
 * Filter-graph callback of the source node: copies @p n floats from its data pointer (+0x1C) to @p dst and advances it.
 *
 * The copy is Util_MemCpy of n * 4 bytes; nothing checks the end of the data. Installed by SFILTER_initSOURCE.
 *
 * @param n  the number of frames
 * @param workBuf  unused
 * @param dst  the output buffer
 * @return @p n
 */
int SFILTER_src(SRCSTATE* self, int n, int workBuf, void *dst) {
    Util_MemCpy_F740(dst, ((SRCSTATE *)self)->pdata, n * 4);
    ((SRCSTATE *)self)->pdata += n;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_initSOURCE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_src_F798(void) __asm__("SFILTER_src");

/**
 * Points a source filter node at its sample data and installs SFILTER_src as its filter function.
 *
 * Stores @p buffer at +0x1C and clears the restore function (+0x4). Called by func_003CBC58.
 *
 * @param effect  the source node
 * @param buffer  the float samples it will hand out
 */
void SFILTER_initSOURCE(SRCSTATE* effect, int buffer) {
    effect->pdata = (float*)buffer;
    effect->sfn.restorefn = 0;
    *(void (**)(void))&effect->sfn = SFILTER_src_F798;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createSOURCE);
#ifdef SKIP_ASM
/**
 * Resets a source filter node: clears its links (+0x8..+0x14), the clean flag (+0x1B) and the data pointer (+0x1C).
 *
 * Quirk: it installs no filter function; SFILTER_initSOURCE does that when the data is attached. func_003CC070 calls it for
 * effect type 0.
 *
 * @param effect  the source node
 * @return 0
 */
int SFILTER_createSOURCE(SRCSTATE* effect) {
    ((int)effect->sfn.pfnnext) = 0;
    ((int)effect->sfn.pfnnext2) = 0;
    ((int)effect->sfn.pfnprev1) = 0;
    ((int)effect->sfn.pfnprev2) = 0;
    (*(signed char*)&effect->sfn.clean) = 0;
    ((int)effect->pdata) = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_echo);
#ifdef SKIP_ASM
int SFILTER_echo_C(struct ECHOSTATE *self, int n, float *a2, float *a3) __asm__("SFILTER_echo");
/**
 * Filter-graph callback of the echo node: pulls frames from its input and runs them through a feedback delay line.
 *
 * The input node (+0x8) is called as (node, n, a3, a2, requester +0x1A) and fills @p srcBuf (a result of 0 or less is
 * returned). Then, in chunks that stop at the end of the circular delay line (+0x1C, length +0x28, position +0x24), a
 * VU0 loop computes per sample d' = d * k.x + in, out = d' * k.y + d with k the quad at +0x30 (feedback, negative
 * feedback from SFILTER_modifyECHO, so an all-pass), writing d' back into the line and out to @p dstBuf. Quirk: it processes the
 * full @p n even when the input produced fewer frames; chunks must be multiples of 16. Installed by SFILTER_createECHO.
 *
 * @param n  the number of frames
 * @param srcBuf  the buffer the input fills
 * @param dstBuf  the output buffer
 * @return @p n, or the input's result when it is 0 or less
 */
// PORT: hand-written VU0 loop (trapping add/addi have no gcc 2.95 pattern; the block moves its operands into fixed
// $4..$8 itself): per chunk, a delay-line step d' = d * f.x + in, out = d' * f.y + d with f = self->f30, d' written back
// to the delay line at self->f1C + pos and out to dst, 16 floats per pass. A port replaces it with the C loop.
int SFILTER_echo_C(struct ECHOSTATE *self, int n, float *srcBuf, float *dstBuf) {
    float *src;
    float *dst;
    int rem;
    int chunk;
    if (((int*)self->sfn.pfnnext)) {
        chunk = ((int (*)(int *, int, float *, float *, int))*((int*)self->sfn.pfnnext))(((int*)self->sfn.pfnnext), n, dstBuf, srcBuf, self->sfn.requester);
        if (chunk <= 0) return chunk;
    }
    src = srcBuf;
    dst = dstBuf;
    rem = n;
    if (rem > 0) {
        int size = self->bufsamples;
        do {
            int pos;
            if (!(self->sampleindex < size)) self->sampleindex = 0;
            pos = self->sampleindex;
            chunk = self->bufsamples - pos;
            if (rem < chunk) chunk = rem;
            // PORT: PS2-only VU0 hand-written loop (blend two 4x4 float blocks with a 2-term factor); one inline asm block.
            __asm__ volatile(
                ".set push\n\t"
                ".set noreorder\n\t"
                ".set noat\n\t"
                "move $6, %0\n\t"
                "move $4, %1\n\t"
                "move $5, %2\n\t"
                "sll $6, $6, 2\n\t"
                "move $7, %3\n\t"
                "move $8, %4\n\t"
                "add $6, $6, $7\n\t"
                "lqc2 $vf1, 0x0($4)\n"
                "1:\n\t"
                "lqc2 $vf4, 0x0($5)\n\t"
                "lqc2 $vf5, 0x10($5)\n\t"
                "lqc2 $vf6, 0x20($5)\n\t"
                "lqc2 $vf7, 0x30($5)\n\t"
                "lqc2 $vf8, 0x0($7)\n\t"
                "lqc2 $vf9, 0x10($7)\n\t"
                "lqc2 $vf10, 0x20($7)\n\t"
                "lqc2 $vf11, 0x30($7)\n\t"
                "vmulx.xyzw $vf12, $vf4, $vf1x\n\t"
                "vmulx.xyzw $vf13, $vf5, $vf1x\n\t"
                "vmulx.xyzw $vf14, $vf6, $vf1x\n\t"
                "vmulx.xyzw $vf15, $vf7, $vf1x\n\t"
                "vadd.xyzw $vf12, $vf12, $vf8\n\t"
                "vadd.xyzw $vf13, $vf13, $vf9\n\t"
                "vadd.xyzw $vf14, $vf14, $vf10\n\t"
                "vadd.xyzw $vf15, $vf15, $vf11\n\t"
                "vmuly.xyzw $vf8, $vf12, $vf1y\n\t"
                "vmuly.xyzw $vf9, $vf13, $vf1y\n\t"
                "vmuly.xyzw $vf10, $vf14, $vf1y\n\t"
                "vmuly.xyzw $vf11, $vf15, $vf1y\n\t"
                "vadd.xyzw $vf8, $vf8, $vf4\n\t"
                "vadd.xyzw $vf9, $vf9, $vf5\n\t"
                "vadd.xyzw $vf10, $vf10, $vf6\n\t"
                "vadd.xyzw $vf11, $vf11, $vf7\n\t"
                "addi $7, $7, 0x40\n\t"
                "addi $8, $8, 0x40\n\t"
                "sqc2 $vf12, 0x0($5)\n\t"
                "sqc2 $vf13, 0x10($5)\n\t"
                "sqc2 $vf14, 0x20($5)\n\t"
                "sqc2 $vf15, 0x30($5)\n\t"
                "sqc2 $vf8, -0x40($8)\n\t"
                "sqc2 $vf9, -0x30($8)\n\t"
                "sqc2 $vf10, -0x20($8)\n\t"
                "sqc2 $vf11, -0x10($8)\n\t"
                "bne $6, $7, 1b\n\t"
                "addi $5, $5, 0x40\n\t"
                ".set pop\n"
                : : "r"(chunk), "r"(((float*)&self->fblevels)), "r"(self->pdelaybuf + pos), "r"(src), "r"(dst)
                : "$4", "$5", "$6", "$7", "$8", "memory");
            self->sampleindex = pos + chunk;
            rem -= chunk;
            src += chunk;
            dst += chunk;
        } while (rem > 0);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_echorestore);
#ifdef SKIP_ASM
void SNDMEMI_free(int);

/**
 * Frees an echo node's delay line: SNDMEMI_free on the allocation (+0x20), then clears it and the aligned pointer
 * (+0x1C). The node's restore function; SFILTER_modifyECHO also calls it before reallocating.
 *
 * @param effect  the echo node
 */
void SFILTER_echorestore(ECHOSTATE* effect) {
    int temp_4;

    temp_4 = (int)effect->punaligneddelaybuf;
    if (temp_4 != 0) {
        SNDMEMI_free(temp_4);
        effect->punaligneddelaybuf = 0;
        effect->pdelaybuf = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_createECHO);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void SFILTER_echo_F998(void) __asm__("SFILTER_echo");
void SFILTER_echorestore_F998(void) __asm__("SFILTER_echorestore");

/**
 * Initialises an echo filter node: installs SFILTER_echo as its filter and SFILTER_echorestore as its restore function.
 *
 * Clears the links (+0x8..+0x14), the clean flag (+0x1B) and both delay-line pointers (+0x1C, +0x20). func_003CC070
 * calls it for effect type 7, followed by SFILTER_modifyECHO.
 *
 * @param effect  the node
 * @return 0
 */
int SFILTER_createECHO(ECHOSTATE* effect) {
    effect->sfn.restorefn = (void (*)(void*))SFILTER_echorestore_F998;
    *(void (**)(void))&effect->sfn = SFILTER_echo_F998;
    effect->sfn.pfnnext = 0;
    effect->sfn.pfnnext2 = 0;
    effect->sfn.pfnprev1 = 0;
    effect->sfn.pfnprev2 = 0;
    *(signed char*)&effect->sfn.clean = 0;
    effect->punaligneddelaybuf = 0;
    effect->pdelaybuf = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SFILTER_modifyECHO);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void f_F958_F9D8(void) __asm__("SFILTER_echorestore");
extern int f_FB38_F9D8(int, int) __asm__("SNDI_findmult16");
extern char *f_5C60_F9D8(int) __asm__("SNDMEMI_alloc");
extern void f_6210_F9D8(char *, int, int) __asm__("memset");
/**
 * Configures an echo node: sets its feedback and allocates a zeroed delay line of a prime multiple of 16 samples.
 *
 * Frees any old line (SFILTER_echorestore). The feedback (params[0] >> 1) / 127 goes to +0x30 and its negation to +0x34. The
 * length SNDI_findmult16(rate, delay ms) is stored at +0x28; SNDMEMI_alloc allocates length * 4 + 16 bytes (+0x20), the
 * 16-byte aligned start goes to +0x1C and is cleared (memset); the position (+0x24) restarts at 0.
 *
 * @param effect  the echo node
 * @param params  [0] feedback level (0..254), [1] sample rate << 8, [2] delay in ms << 8
 */
void SFILTER_modifyECHO(ECHOSTATE* effect, void *params) {
    float temp_f0;
    int gain;
    int temp_16_2;
    int rate;
    int delayMs;
    int temp_2;
    char *temp_2_2;
    int temp_3;

    gain = (*(int *)((char*)(params) + (0))) >> 1;
    rate = (*(int *)((char*)(params) + (4))) >> 8;
    delayMs = (*(int *)((char*)(params) + (8))) >> 8;
    f_F958_F9D8();
    temp_f0 = (float) gain * 0.007874016f;
    effect->fblevels = temp_f0;
    effect->fbneglevels = -temp_f0;
    temp_2 = f_FB38_F9D8(rate, delayMs);
    temp_16_2 = temp_2 * 4;
    effect->bufsamples = temp_2;
    temp_2_2 = f_5C60_F9D8(temp_16_2 + 0x10);
    ((char*)effect->punaligneddelaybuf) = temp_2_2;
    temp_3 = ((unsigned int) ((int)temp_2_2 + 0xF) >> 4) * 0x10;
    ((int)effect->pdelaybuf) = temp_3;
    f_6210_F9D8((char*)temp_3, 0, temp_16_2);
    effect->sampleindex = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_rootof1plusx);
#ifdef SKIP_ASM
/**
 * Returns an 8-term series approximation of sqrt(1 + @p fparg0), good for small arguments.
 *
 * The terms are 1 + x/2 - x^2/8 + x^3/16 - 5x^4/128 ... built by multiplying each by -0.25, -0.5, -0.625, -0.7, -0.75
 * and -0.7857 times x. SFILTER_modifyRESON uses it for sqrt(1 - c^2).
 *
 * @param fparg0  x
 * @return about sqrt(1 + x)
 */
float SNDI_rootof1plusx(float fparg0) {
    float temp_f0;
    float temp_f1;
    float temp_f2;
    float temp_f4;
    float temp_f5;
    float temp_f6;

    temp_f0 = fparg0 * 0.5f;
    temp_f1 = temp_f0 * -0.25f * fparg0;
    temp_f2 = temp_f1 * -0.5f * fparg0;
    temp_f4 = temp_f2 * -0.625f * fparg0;
    temp_f5 = temp_f4 * -0.7f * fparg0;
    temp_f6 = temp_f5 * -0.75f * fparg0;
    return temp_f0 + 1.0f + temp_f1 + temp_f2 + temp_f4 + temp_f5 + temp_f6 + (temp_f6 * -0.7857f * fparg0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_findmult16);
#ifdef SKIP_ASM
extern int SNDI_findprime(int, int);

/**
 * Returns a delay length in samples that is 16 times a prime: SNDI_findprime(rate, delay / 16) * 16.
 *
 * @param rate  the sample rate
 * @param delayMs  the delay in milliseconds
 * @return the length in samples, a multiple of 16
 */
int SNDI_findmult16(int rate, int delayMs) {
    return SNDI_findprime(rate, delayMs / 16) * 16;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_cheapsqrt);
#ifdef SKIP_ASM
/**
 * Returns the integer square root of @p n (the largest i with i * i <= n), found by counting up from 0.
 *
 * @param n  a non-negative number
 * @return floor(sqrt(n))
 */
int SNDI_cheapsqrt(int n) {
    int i = 0, sq;
    for (;;) {
        sq = i * i;
        i++;
        if (sq != n) {
            if (n < sq) return i - 2;
        } else
            return i - 1;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", SNDI_findprime);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SNDI_cheapsqrt_FB98(int) __asm__("SNDI_cheapsqrt");

/**
 * Returns the smallest prime number of samples at least @p duration ms long at @p rate (at least 3).
 *
 * Converts to samples (duration * rate / 1000) and tests candidates by trial division up to SNDI_cheapsqrt's integer
 * square root + 1, stepping by 1 until one has no divisor.
 *
 * @param rate  the sample rate
 * @param duration  the length in milliseconds
 * @return the prime sample count
 */
int SNDI_findprime(int rate, int duration) {
    int lim;
    int length;
    int n;
    int divisor;

    length = (int) (duration * rate) / 1000;
    n = (length <= 2) ? 3 : length;
    for (;;) {
        lim = SNDI_cheapsqrt_FB98(n) + 1;
        for (divisor = 2; lim >= divisor; divisor++) {
            if ((n % divisor) == 0) {
                n += 1;
                break;
            }
            if (divisor == lim) {
                return n;
            }
        }
    }
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003CFC30);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0B18);
#ifdef SKIP_ASM
typedef struct { int a, b, c, d; } E_0B18;
typedef struct { int p0, p1, p2; unsigned lo : 16; unsigned idx : 6; unsigned hi : 10; E_0B18 e[1]; } Q_0B18;

/**
 * Appends a 16-byte item to a path event: copies @p src to the item slot the event's 6-bit count (+0xC bits 16-21)
 * points at, after the 16-byte header, and increments the count. No bounds check.
 *
 * @param queue  the event (header followed by its items)
 * @param src  the item to append
 */
void func_003D0B18(Q_0B18 *queue, E_0B18 *src) {
    E_0B18 *arr = queue->e;
    unsigned i = queue->idx;
    queue->idx = i + 1;
    arr[i] = *src;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0B78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void func_003D0B78_impl(char *base, int n, char *lim) __asm__("func_003D0B78");
/**
 * Marks items of a path event as skipped (flag 0x4000000 in +0x8) between unmatched block ends.
 *
 * Walks the 16-byte items after @p base up to @p lim. Items of type 1 (byte +0x9) open a nested block when bits
 * 0x300000 of +0x8 are 0x100000 and close one when they are 0x200000 or 0x300000; a close at nesting depth 0 counts
 * @p n down. Every item reached while @p n is below 1 is flagged, and the walk stops once @p n is below 0: with
 * @p n = 0 it flags up to and including the first unmatched end, with 1 from after the first up to the second. The path
 * interpreter (func_003CFC30) passes the current item's own skip bit as @p n.
 *
 * @param base  the current item
 * @param n  the number of unmatched block ends to pass before flagging
 * @param lim  the event's last item
 */
void func_003D0B78_impl(char *base, int n, char *lim) {
    int depth = 0;
    char *item = base + 0x10;
    while (item <= lim && n >= 0) {
        if (((unsigned char *)item)[9] == 1) {
            unsigned blockBits = *(unsigned *)(item + 8) & 0x300000;
            if (blockBits == 0x100000) {
                depth++;
            } else if (blockBits == 0x300000) {
                if (depth-- < 1) {
                    n--;
                }
            } else if (blockBits == 0x200000) {
                if (depth-- < 1) {
                    n--;
                }
            }
        }
        if (n < 1) {
            *(unsigned *)(item + 8) |= 0x4000000;
        }
        item += 0x10;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0C20);
#ifdef SKIP_ASM
/**
 * Marks the items after @p base as skipped (flag 0x4000000 in +0x8), up to the next item of type 1 (byte +0x9) or
 * @p limit. Called by the path interpreter (func_003CFC30).
 *
 * @param base  the current item
 * @param limit  the event's last item
 */
void func_003D0C20(unsigned char *base, unsigned char *limit) {
    unsigned char *entry = base + 0x10;

    while (limit >= entry && entry[9] != 1) {
        *(unsigned int *)(entry + 8) |= 0x04000000;
        entry += 0x10;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0C78);
#ifdef SKIP_ASM
/**
 * Marks the item right after @p base as skipped (flag 0x4000000 in +0x8), if it is not past @p limit.
 *
 * @param base  the current item
 * @param limit  the event's last item
 */
void func_003D0C78(int base, unsigned int limit) {
    unsigned int temp_4;

    temp_4 = base + 0x10;
    if (limit >= temp_4) {
        (*(int *)((char*)(temp_4) + (8))) = (int) ((*(int *)((char*)(temp_4) + (8))) | 0x04000000);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0CA0);
#ifdef SKIP_ASM
extern void func_003D0CC8(int);
extern int func_003D5290();

/**
 * Computes the play state of a path voice: func_003D0CC8 on the voice func_003D5290 finds.
 *
 * Quirk: declared without parameters and void, it hands its caller's first argument (the voice id, still in $a0) to
 * func_003D5290 untouched and leaves func_003D0CC8's result in the return register. Only referenced from the unwind
 * table.
 */
void func_003D0CA0(void) {
    func_003D0CC8(func_003D5290());
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0CC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D32B8_0CC8(char *, int *) __asm__("func_003D32B8");
extern int D_0044F438_0CC8[] __asm__("D_0044F438");
extern char *D_0044F420_0CC8[] __asm__("D_0044F420");
int func_003D0CC8_0CC8(char *arg0) __asm__("func_003D0CC8");

/**
 * Works out and stores a path voice's play state (+0x100): 2 idle, 3 waiting, 4 due, 7 playing, 5 near the end.
 *
 * A voice with a pending start tick (+0x4C) is 4 once the path tick D_0044F438 has reached it and 3 before. Otherwise a
 * voice with a current node (+0x36 >= 0) is 7, or 5 when func_003D32B8 fills its timing and the voice's position
 * (+0x24) plus the selected player's +0xC has reached the node's end (field 6 of that result). Everything else is 2.
 *
 * @param pathVoice  the path voice (sPathVoice), or 0
 * @return the state, 1 for a null voice
 */
int func_003D0CC8_0CC8(char *pathVoice) {
    int buf[8];
    unsigned int startTick;
    int state;
    if (pathVoice == 0) {
        return 1;
    }
    {
        startTick = *(unsigned int *)(pathVoice + 0x4C);
        *(int *)(pathVoice + 0x100) = 2;
        if (startTick != 0) {
            if ((unsigned int)D_0044F438_0CC8[0] >= startTick) {
                *(int *)(pathVoice + 0x100) = 4;
            } else {
                *(int *)(pathVoice + 0x100) = 3;
            }
            goto done;
        }
        if (*(short *)(pathVoice + 0x36) >= 0) {
            *(int *)(pathVoice + 0x100) = 7;
            if (func_003D32B8_0CC8(pathVoice, buf) >= 0
                && (unsigned int)(*(int *)(pathVoice + 0x24) + *(int *)(D_0044F420_0CC8[0] + 0xC)) >= (unsigned int)buf[6]) {
                *(int *)(pathVoice + 0x100) = 5;
            }
        }
done:
        state = *(int *)(pathVoice + 0x100);
    }
    return state;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0D70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_0D70 { char b0; unsigned char b1; unsigned char b2; };
struct G_0D70 { char pad[0x58]; struct E_0D70 *list[24]; };
struct T_0D70 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_0D70 __asm__("D_0044F42C");
extern int D_0044F430_0D70 __asm__("D_0044F430");
extern struct G_0D70 *D_0044F420_0D70 __asm__("D_0044F420");
extern struct T_0D70 D_00517610_0D70[] __asm__("D_00517610");
extern int func_003D5450_0D70(int) __asm__("func_003D5450");

static inline unsigned int has_0D70(unsigned int a, unsigned int b) { return a & b; }

/**
 * Sets a value on every voice of the path players a mask selects, keeping the previous one: +0x1 = @p val, old +0x1 to
 * +0x2.
 *
 * Returns -18 unless the path system is up (D_0044F42C) and idle (D_0044F430 clear, func_003D5450(-1) non-zero). Only
 * values below 0x80 are applied; each of the 4 players (D_00517610) that is in use (+0x34) and shares a class bit
 * (0xF0000000) and a group bit (0x0F000000) with @p mask is selected with func_003D5450 and all 24 of its voices
 * (+0x58) are updated, whatever the low mask bits say. Clears the selection (D_0044F430, D_0044F420) at the end.
 * func_002B2120 calls it for 0x11000001, passing its own second argument through.
 *
 * @param mask  the path id mask (class, group and voice bits)
 * @param val  the new value (0..0x7F)
 * @return 0, or -18 when the system is not ready
 */
int func_003D0D70(unsigned int mask, unsigned int val) {
    int i, j;
    if (D_0044F42C_0D70 == 0 || D_0044F430_0D70 != 0 || func_003D5450_0D70(-1) == 0) {
        return -18;
    }
    if (val < 0x80) {
        for (i = 0; i < 4; i++) {
            unsigned int playerMask;
            if (D_00517610_0D70[i].f34 == 0) continue;
            playerMask = D_00517610_0D70[i].f30;
            if (has_0D70(playerMask & 0xF0000000, mask) == 0) continue;
            if (has_0D70(playerMask & 0x0F000000, mask) == 0) continue;
            if (func_003D5450_0D70(i & 0xFF) == 0) continue;
            for (j = 0; j < 24; j++) {
                struct E_0D70 *pathVoice = D_0044F420_0D70->list[j];
                if (pathVoice != 0) {
                    unsigned char oldValue = pathVoice->b1;
                    pathVoice->b1 = val;
                    pathVoice->b2 = oldValue;
                }
            }
        }
    }
    D_0044F430_0D70 = 0;
    D_0044F420_0D70 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D0ED0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct A_0ED0 { int v[4]; };
struct B_0ED0 { unsigned int f0; char pad4[4]; char b8; char b9; char padA[2]; short hC; char bE; char padF; };
struct T_0ED0 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_0ED0 __asm__("D_0044F42C");
extern int D_0044F430_0ED0 __asm__("D_0044F430");
extern void *D_0044F420_0ED0 __asm__("D_0044F420");
extern struct T_0ED0 D_00517610_0ED0[] __asm__("D_00517610");
extern int func_003D5450_0ED0(int) __asm__("func_003D5450");
extern void memset_0ED0(void *, int, int) __asm__("memset");
extern void func_003D0B18_0ED0(struct A_0ED0 *, struct B_0ED0 *) __asm__("func_003D0B18");
extern int func_003D2068_0ED0(struct A_0ED0 *) __asm__("PATHI_copyevent");
extern void func_003D1F80_0ED0(unsigned int, int) __asm__("func_003D1F80");

static inline unsigned int has_0ED0(unsigned int a, unsigned int b) { return a & b; }

/**
 * Queues a type-4 item for the path players a mask selects: builds a one-item event and posts a copy to each.
 *
 * Returns -18 unless the path system is up and idle (D_0044F42C, D_0044F430, func_003D5450(-1)). The item gets
 * @p mask at +0x0, type 4 (+0x9), -1 at +0x8 and +0xE and @p val at +0xC; func_003D0B18 appends it to a cleared 16-byte
 * event header. For each player in use whose class and group bits overlap @p mask, func_003D5450 selects it,
 * PATHI_copyevent copies the event into the player's buffer and func_003D1F80 schedules the copy. Clears the selection
 * at the end.
 *
 * @param mask  the path id mask
 * @param val  the value stored in the item (+0xC)
 * @return 0, or -18 when the system is not ready
 */
int func_003D0ED0(unsigned int mask, int val) {
    struct B_0ED0 item;
    struct A_0ED0 eventHeader;
    unsigned int i;
    if (D_0044F42C_0ED0 == 0 || D_0044F430_0ED0 != 0 || func_003D5450_0ED0(-1) == 0) {
        return -18;
    }
    memset_0ED0(&eventHeader, 0, 16);
    memset_0ED0(&item, 0, 16);
    item.b9 = 4;
    item.f0 = mask;
    item.b8 = -1;
    item.bE = -1;
    item.hC = val;
    func_003D0B18_0ED0(&eventHeader, &item);
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_0ED0[i].f34 == 0) continue;
        playerMask = D_00517610_0ED0[i].f30;
        if (has_0ED0(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_0ED0(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_0ED0(i & 0xFF) == 0) continue;
        func_003D1F80_0ED0(mask, func_003D2068_0ED0(&eventHeader));
    }
    D_0044F430_0ED0 = 0;
    D_0044F420_0ED0 = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1038);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_1038 { char pad[3]; unsigned char b3; char pad2[0x50]; int f54; };
struct G_1038 { char pad[0x58]; struct E_1038 *list[24]; };
struct T_1038 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_1038 __asm__("D_0044F42C");
extern unsigned char D_0044F42D_1038 __asm__("D_0044F42D");
extern int D_0044F430_1038 __asm__("D_0044F430");
extern struct G_1038 *D_0044F420_1038 __asm__("D_0044F420");
extern struct T_1038 D_00517610_1038[] __asm__("D_00517610");
extern int func_003D5450_1038(int) __asm__("func_003D5450");
extern void func_002AF7A0_1038(int, int) __asm__("func_002AF7A0");

static inline unsigned int has_1038(unsigned int a, unsigned int b) { return a & b; }

/**
 * Pauses or resumes the voices of the path players a mask selects (func_002AF7A0 on each voice's sound, +0x3 = @p val).
 *
 * Does nothing unless the path system is up and idle. A mask of -1 also stores @p val as the global state (D_0044F42D).
 * For each player in use whose class and group bits overlap @p mask (selected with func_003D5450), every voice whose
 * bit is set in the low 24 bits of @p mask gets func_002AF7A0(+0x54, val) and +0x3 = val. Clears the selection at the
 * end. func_002B2018 passes 1 after setting the stream pitch to 0, func_002B2070 passes 0 after restoring it.
 *
 * @param mask  the path id mask
 * @param val  1 to pause, 0 to resume
 */
void func_003D1038(unsigned int mask, unsigned char val) {
    int i, j;
    if (D_0044F42C_1038 == 0) return;
    if (D_0044F430_1038 != 0) return;
    if (func_003D5450_1038(-1) == 0) return;
    if (mask == 0xFFFFFFFF) D_0044F42D_1038 = val;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_1038[i].f34 == 0) continue;
        playerMask = D_00517610_1038[i].f30;
        if (has_1038(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_1038(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_1038(i & 0xFF) == 0) continue;
        for (j = 0; j < 24; j++) {
            struct E_1038 *pathVoice = D_0044F420_1038->list[j];
            if (pathVoice != 0 && (((mask >> j) ^ 1) & 1) == 0) {
                func_002AF7A0_1038(pathVoice->f54, val);
                pathVoice->b3 = val;
            }
        }
    }
    D_0044F430_1038 = 0;
    D_0044F420_1038 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", PATH_status);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Info_11B8 {
    int a;
    int b;
    int c;
    unsigned int total;
    unsigned int f10;
    unsigned int f14;
    unsigned int len;
    unsigned int f1C;
    unsigned int f20;
    int f24;
    int f28;
};
struct Ent_11B8 { int a; int b; int c; int key; };
struct Fmt_11B8 {
    unsigned int pad0 : 20;
    unsigned int ch : 4;
    unsigned int mul : 4;
    unsigned int pad1 : 4;
};
struct Rec_11B8 { int w0; unsigned int w; };
struct S_11B8 {
    char pad0[0x20];
    unsigned int pad20a : 14;
    unsigned int n : 5;
    unsigned int pad20b : 13;
    char pad24[0x36 - 0x24];
    short f36;
    char pad38[0x54 - 0x38];
    int f54;
    char pad58[0xDC - 0x58];
    char name[0xFC - 0xDC];
    struct Ent_11B8 *tbl;
};
struct G_11B8 {
    char pad0[0x34];
    char *hdr;
    short *a;
};
extern unsigned char D_0044F42C_11B8 __asm__("D_0044F42C");
extern int D_0044F430_11B8 __asm__("D_0044F430");
extern int D_0044F43C_11B8 __asm__("D_0044F43C");
extern struct G_11B8 *D_0044F420_11B8 __asm__("D_0044F420");
extern char D_00495B10_11B8[] __asm__("D_00495B10");
extern int f_5450_11B8(int) __asm__("func_003D5450");
extern struct S_11B8 *f_5290_11B8(unsigned int) __asm__("func_003D5290");
extern int f_4BB0_11B8(unsigned int) __asm__("func_003D4BB0");
extern int f_F2F0_11B8(int, int *, int *, int) __asm__("func_002AF2F0");
extern void f_1658_11B8(char *, char *, int) __asm__("func_003D1658");

int PATH_status_impl(unsigned int id, struct Info_11B8 *info) __asm__("PATH_status");
/**
 * Fills a status block for the path voice an id names: the current node, part and the position within it.
 *
 * Returns -18 unless the path system is up and idle. The block (11 words) starts as -1 for node, part and the last two
 * fields and 0 elsewhere. When func_003D5290 finds the voice and func_003D4BB0 accepts its group, func_002AF2F0 on the
 * voice's sound (+0x54) gives the current request, its offset and length: the block gets the length (+0x18), offset +
 * length (+0xC) and the voice's node (+0x36 to +0x24). The request is looked up in the voice's request table (+0xFC,
 * +0x20 bits 14-18 entries) for the node (+0x0) and part (+0x8); the node's format word gives a channel divisor (bits
 * 20-23) and multiplier (bits 24-27) for the per-channel totals (+0x10, +0x14) and the remainders of the length (+0x1C,
 * +0x20). When the request is not in the table it prints "( %.8s ) PATH_status - currentrequest %d not found"
 * (func_003D1658, if D_0044F43C has 0x100) with the voice's name.
 *
 * @param id  the path id
 * @param info  the status block to fill
 * @return the current request (also when it is missing from the table), -8 when there is no voice or request, a
 * negative func_002AF2F0 error, or -18 when the system is not ready
 */
int PATH_status_impl(unsigned int id, struct Info_11B8 *info) {
    int ret;
    struct S_11B8 *pathVoice;
    struct G_11B8 *pathPlayer;
    struct Rec_11B8 *nodeFormat;
    unsigned int i;
    unsigned int total;
    unsigned int perChannel;
    int off;
    int len;

    ret = -8;
    if (D_0044F42C_11B8 == 0 || D_0044F430_11B8 != 0 || f_5450_11B8(-1) == 0) {
        return -18;
    }
    info->a = -1;
    info->b = -1;
    info->c = -1;
    info->total = 0;
    info->f10 = 0;
    info->f14 = 0;
    info->len = 0;
    info->f20 = 0;
    info->f1C = 0;
    info->f24 = -1;
    info->f28 = -1;
    pathVoice = f_5290_11B8(id);
    if (pathVoice == 0) {
        goto done;
    }
    if (f_4BB0_11B8(id & 0xFF000000) != 0) {
        ret = f_F2F0_11B8(pathVoice->f54, &off, &len, 0);
        if (ret >= 0) {
            info->len = len;
            info->total = off + len;
            info->f24 = pathVoice->f36;
            for (i = 0; i < pathVoice->n; i++) {
                if (pathVoice->tbl[i].key == ret) {
                    pathPlayer = D_0044F420_11B8;
                    info->a = pathVoice->tbl[i].a;
                    total = info->total;
                    info->c = pathVoice->tbl[i].c;
                    nodeFormat = (struct Rec_11B8 *)(pathPlayer->hdr + pathPlayer->a[info->a] * 4);
                    perChannel = total;
                    if (nodeFormat->w & 0xF00000) {
                        info->f10 = total / ((nodeFormat->w >> 20) & 0xF);
                        perChannel = total / ((nodeFormat->w >> 20) & 0xF);
                        if (nodeFormat->w & 0xF000000) {
                            info->f14 = total / (((nodeFormat->w >> 24) & 0xF) * ((nodeFormat->w >> 20) & 0xF));
                            total = total / (((nodeFormat->w >> 24) & 0xF) * ((nodeFormat->w >> 20) & 0xF));
                        }
                    }
                    if (perChannel != 0) {
                        info->f1C = info->len % perChannel;
                    } else {
                        info->f1C = 0;
                    }
                    if (total != 0) {
                        info->f20 = info->len % total;
                    } else {
                        info->f20 = 0;
                    }
                    goto done;
                }
            }
            if (D_0044F43C_11B8 & 0x100) {
                f_1658_11B8(D_00495B10_11B8, pathVoice->name, ret);
            }
        }
    }
done:
    D_0044F430_11B8 = 0;
    D_0044F420_11B8 = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1470);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_1470 { char pad[0x36]; short h36; char pad2[0x1C]; int f54; };
struct G_1470 { char pad[0x58]; struct E_1470 *list[24]; };
extern unsigned char D_0044F42C_1470 __asm__("D_0044F42C");
extern int D_0044F430_1470 __asm__("D_0044F430");
extern struct G_1470 *D_0044F420_1470 __asm__("D_0044F420");
struct T_1470 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern struct T_1470 D_00517610_1470[] __asm__("D_00517610");
extern int func_003D5450_1470(int) __asm__("func_003D5450");
extern void func_002AF838_1470(int) __asm__("func_002AF838");

static inline unsigned int has_1470(unsigned int a, unsigned int b) { return a & b; }

/**
 * Stops every voice of the path players a mask selects: func_002AF838 on each voice's sound (+0x54) and node +0x36 =
 * -1.
 *
 * Does nothing unless the path system is up and idle. Players are matched by class and group bits only; all 24 voices
 * of each are stopped. Clears the selection (D_0044F420, D_0044F430) at the end.
 *
 * @param mask  the path id mask
 */
void func_003D1470(unsigned int mask) {
    int i, j;
    if (D_0044F42C_1470 == 0) return;
    if (D_0044F430_1470 != 0) return;
    if (func_003D5450_1470(-1) == 0) return;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_1470[i].f34 == 0) continue;
        playerMask = D_00517610_1470[i].f30;
        if (has_1470(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_1470(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_1470(i) == 0) continue;
        for (j = 0; j < 24; j++) {
            struct E_1470 *pathVoice = D_0044F420_1470->list[j];
            if (pathVoice != 0) {
                func_002AF838_1470(pathVoice->f54);
                pathVoice->h36 = -1;
            }
        }
    }
    D_0044F420_1470 = 0;
    D_0044F430_1470 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D15C8);
#ifdef SKIP_ASM
/**
 * Sets the play position (+0x24) of the path voice func_003D5290 finds to @p value & 0xFFFF.
 *
 * func_003D5290 is called without arguments here, so it gets @p mask from $a0. func_002B2140 calls it for 0x11000001,
 * passing its own second argument through.
 *
 * @param mask  the path id (reaches func_003D5290 through the register)
 * @param value  the new position
 * @return 0, or -8 when no voice matches
 */
int func_003D15C8(int mask, int value) {
    char *channel = (char *)func_003D5290();
    if (channel == 0) {
        return -8;
    }
    *(int *)(channel + 0x24) = value & 0xFFFF;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1608);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned int D_0044F430_1608[] __asm__("D_0044F430");
extern void exit_1608(int, ...) __asm__("exit") __attribute__((noreturn));

/**
 * Fatal-error handler of the path library: sets D_0044F430 to -1 (busy, so every entry point refuses) and calls
 * exit(0), which exits. The printf-style arguments are ignored.
 *
 * @param format  the format string (ignored)
 */
void func_003D1608(int format, ...) {
    D_0044F430_1608[0] = 0xFFFFFFFF;
    exit_1608(0);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1658);
#ifdef SKIP_ASM
/**
 * Does nothing: the path library's debug print, compiled out (the format and arguments are ignored).
 *
 * Called with messages such as "PATHI_copyevent : can't add new event..." when D_0044F43C enables them.
 *
 * @param fmt  the printf format (ignored)
 */
void func_003D1658(int fmt, ...) {
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1690);
#ifdef SKIP_ASM
/**
 * Does nothing: a second compiled-out printf-style debug hook of the path library.
 *
 * @param format  the format string (ignored)
 */
void func_003D1690(int format, ...) {
}
#endif

/**
 * Does nothing (an empty hook of the path library).
 */
void func_003D16C8(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D16D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F43C_16D0[4] __asm__("D_0044F43C");

int func_003D16D0_16D0(void) __asm__("func_003D16D0");
/**
 * Returns the path library's debug-message flags (D_0044F43C; 0x100 after func_003D2290, 0x120 enables the
 * PATHI_copyevent message).
 *
 * @return the flags
 */
int func_003D16D0_16D0(void) {
    return D_0044F43C_16D0[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D16E0);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged.
 *
 * @param value  any value
 * @return @p value
 */
int func_003D16E0(int value) {
    return value;
}
#endif

/**
 * Does nothing (an empty hook of the path library).
 */
void func_003D16E8(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D16F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct H_16F0 { int f0; int f4; int f8; };
struct E_16F0 { int w0; int w4; int w8; unsigned int wC; };
struct Hdr_16F0 { char pad[0xF]; unsigned char bF; };
struct G_16F0 { char pad[0x34]; struct Hdr_16F0 *hdr; char pad2[0x40 - 0x38]; short *idx; };
struct T_16F0 { char pad[0x30]; unsigned int f30; int f34; char pad2[0xDC - 0x38]; struct H_16F0 *list[16]; char pad3[0x91C - 0x11C]; int n; char pad4[0x928 - 0x920]; };
extern unsigned char D_0044F42C_16F0 __asm__("D_0044F42C");
extern int D_0044F430_16F0 __asm__("D_0044F430");
extern struct G_16F0 *D_0044F420_16F0 __asm__("D_0044F420");
extern struct T_16F0 D_00517610_16F0[] __asm__("D_00517610");
extern int func_003D5450_16F0(int) __asm__("func_003D5450");
extern struct H_16F0 *func_003D2068_16F0(struct E_16F0 *) __asm__("PATHI_copyevent");
extern int func_002AF370_16F0(void) __asm__("func_002AF370");

static inline unsigned int has_16F0(unsigned int a, unsigned int b) { return a & b; }

/**
 * Posts a path event: copies event definition @p prog of the path data and queues it on every player of its group.
 *
 * Returns -18 unless the path system is up and idle. Bits 8-11 of @p prog pick the group (0x1000000 << n) and the low
 * byte the event. For each player in use that has that group bit and shares class and group bits with @p mask, it is
 * selected (func_003D5450); an event number at or above the path data's event count (+0xF) fails with -13, and an event
 * whose definition (data +0x34, offsets +0x40) is disabled (0x20000000 in +0xC) is skipped. Otherwise PATHI_copyevent
 * copies it into the player's buffer and the copy, stamped with the current time (func_002AF370) at +0x0 and +0x8,
 * joins the pending list (+0xDC, count +0x91C) of every player in use with the same class and group byte (a full list
 * gives -13). Likely bug: PATHI_copyevent's 0 (buffer full) is not checked, so the stamp is written through a null
 * pointer. func_002B20C8 posts the music events with mask -1.
 *
 * @param mask  the path id mask (-1 for all)
 * @param prog  the event: group in bits 8-11, event number in the low byte
 * @return 0, -8 when nothing was queued, -13 for a bad event or a full list, -18 when the system is not ready
 */
int func_003D16F0(unsigned int mask, short prog) {
    int result = 0;
    unsigned int bit;
    unsigned int i;
    if (D_0044F42C_16F0 == 0 || D_0044F430_16F0 != 0 || func_003D5450_16F0(-1) == 0) {
        return -18;
    }
    bit = 0x1000000 << ((prog & 0xF00) >> 8);
    prog &= 0xFF;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        struct Hdr_16F0 *hdr;
        struct E_16F0 *event;
        struct H_16F0 *pending;
        unsigned int top;
        int k, err;
        if (D_00517610_16F0[i].f34 == 0) continue;
        playerMask = D_00517610_16F0[i].f30;
        if (has_16F0(playerMask, bit) == 0) continue;
        if (has_16F0(playerMask & 0x0F000000, mask) == 0) continue;
        if (has_16F0(playerMask & 0xF0000000, mask) == 0) continue;
        if (func_003D5450_16F0(i & 0xFF) == 0) continue;
        hdr = D_0044F420_16F0->hdr;
        if (prog >= hdr->bF) { result = -13; break; }
        event = (struct E_16F0 *)((char *)hdr + D_0044F420_16F0->idx[prog] * 4);
        if (event->wC & 0x20000000) continue;
        err = -8;
        event = (struct E_16F0 *)func_003D2068_16F0(event);
        top = D_00517610_16F0[i].f30;
        top &= 0xFF000000;
        for (k = 0; k < 4; k++) {
            int n;
            if (D_00517610_16F0[k].f34 == 0) continue;
            if ((D_00517610_16F0[k].f30 & top) != top) continue;
            n = D_00517610_16F0[k].n;
            if (n >= 16) { result = -13; goto next; }
            err = 0;
            pending = (struct H_16F0 *)event;
            pending->f0 = pending->f8 = func_002AF370_16F0();
            D_00517610_16F0[k].list[n] = pending;
            n++;
            D_00517610_16F0[k].n = n;
        }
        result = err;
    next:;
    }
    D_0044F430_16F0 = 0;
    D_0044F420_16F0 = 0;
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1940);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003D1BA8_1940(int, int) __asm__("func_003D1BA8");
extern int func_003D5450_1940(int) __asm__("func_003D5450");
extern int D_0044F420_1940[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_1940[] __asm__("D_0044F42C");
extern int D_0044F430_1940[] __asm__("D_0044F430");
extern char D_00517610_1940[] __asm__("D_00517610");

struct P_1940 { char pad[0xDC]; int a[16]; };

/**
 * Cancels the pending events of the path players a mask selects (func_003D1BA8 with mode 2).
 *
 * Does nothing unless the path system is up and idle. Players are matched by class and group bits. Likely bug:
 * func_003D1BA8 shifts the pending list down over the removed slot, but the loop moves on to the next slot, so only
 * every other pending event is cancelled (func_003D1E80 steps back after a removal). Clears the selection at the end.
 *
 * @param mask  the path id mask
 */
void func_003D1940(unsigned int mask) {
    int i, j;
    unsigned int fl, classBits, groupBits;
    if (D_0044F42C_1940[0] != 0) {
        if (D_0044F430_1940[0] == 0) {
            if (func_003D5450_1940(-1) != 0) {
                for (i = 0; i < 4; i++) {
                    if (*(int *)(D_00517610_1940 + 0x34 + i * 0x928) != 0) {
                        fl = *(unsigned int *)(D_00517610_1940 + 0x30 + i * 0x928);
                        classBits = fl & 0xF0000000;
                        if ((classBits & mask) != 0 && (groupBits = fl & 0x0F000000, (groupBits & mask) != 0)) {
                            if (func_003D5450_1940(i & 0xFF) != 0) {
                                for (j = 0; j < 0x10; j++) {
                                    if (((struct P_1940 *)D_0044F420_1940[0])->a[j] != 0) {
                                        func_003D1BA8_1940(j, 2);
                                    }
                                }
                            }
                        }
                    }
                }
                D_0044F420_1940[0] = 0;
                D_0044F430_1940[0] = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1A88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct E_1A88 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
struct G_1A88 { char pad[0xDC]; void *arr[16]; };
extern unsigned char D_0044F42C_1A88 __asm__("D_0044F42C");
extern int D_0044F430_1A88 __asm__("D_0044F430");
extern struct G_1A88 *D_0044F420_1A88 __asm__("D_0044F420");
extern struct E_1A88 D_00517610_1A88[] __asm__("D_00517610");
extern int func_003D5450_1A88(int) __asm__("func_003D5450");

/**
 * Returns the number of pending events of the first path player in use whose group bits overlap @p mask.
 *
 * Returns -18 unless the path system is up and idle; counts the non-null slots of the player's pending list (+0xDC, 16
 * entries) and clears the selection.
 *
 * @param mask  the path id mask (only the group bits 0x0F000000 count)
 * @return the number of pending events, or -18
 */
int func_003D1A88(unsigned int mask) {
    int cnt = 0;
    int i, j;
    if (D_0044F42C_1A88 == 0 || D_0044F430_1A88 != 0 || func_003D5450_1A88(-1) == 0) {
        return -18;
    }
    mask &= 0x0F000000;
    for (i = 0; i < 4; i++) {
        if (D_00517610_1A88[i].f34 == 0) continue;
        if ((D_00517610_1A88[i].f30 & mask) == 0) continue;
        if (func_003D5450_1A88(i & 0xFF)) {
            for (j = 0; j < 16; j++) {
                if (D_0044F420_1A88->arr[j]) cnt++;
            }
            break;
        }
    }
    D_0044F430_1A88 = 0;
    D_0044F420_1A88 = 0;
    return cnt;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1BA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Frees voice slot idx: compacts the sample pool over it (fixing up the other
// slot pointers) or hands it to the release hook, then shifts the slot list down.
struct H_1BA8 { short h0; short h2; short h4; short h6; short h8; short hA; short hC; unsigned short hE; };
struct G_1BA8 { char pad[0xDC]; struct H_1BA8 *slots[17]; char data[0x91C - 0x120]; int n91C; };
extern struct G_1BA8 *D_0044F420_1BA8 __asm__("D_0044F420");
extern void (*D_0044F428_1BA8)(struct H_1BA8 *, int) __asm__("D_0044F428");
extern void func_0041605C_1BA8(void *, void *, int) __asm__("func_0041605C");

/**
 * Removes pending event @p idx from the selected path player (D_0044F420), freeing its storage.
 *
 * An event stored in the player's own event buffer (+0x11C..+0x91C) is removed by moving the rest of the buffer down
 * over it (func_0041605C) and adjusting the other 17 slot pointers that lay above it; any other event goes to the
 * release hook D_0044F428 (if set) with @p mode. The slot list (+0xDC) is then shifted down and the count (+0x91C)
 * decremented.
 *
 * @param idx  the pending slot (0..15)
 * @param mode  why it ends, passed to the release hook (1 timed out, 2 cancelled, 3 finished)
 */
void func_003D1BA8(int idx, int mode) {
    struct H_1BA8 *event = D_0044F420_1BA8->slots[idx];
    int size = (event->hE & 0x3F) * 16 + 16;
    struct H_1BA8 save;
    char *end;
    struct G_1BA8 *pathPlayer;
    unsigned int i;
    save = *event; /* dead copy, kept: retail copies the header to the stack */
    end = (char *)D_0044F420_1BA8 + 0x91C;
    if ((char *)event >= (char *)D_0044F420_1BA8 + 0x11C && (char *)event < end) {
        func_0041605C_1BA8(event, (char *)event + size, end - (char *)event);
        for (i = 0; i < 17; i++) {
            struct H_1BA8 *slotEvent = D_0044F420_1BA8->slots[i];
            if (slotEvent > event && (char *)slotEvent < end)
                D_0044F420_1BA8->slots[i] = (struct H_1BA8 *)((char *)slotEvent - size);
        }
    } else if (D_0044F428_1BA8) {
        D_0044F428_1BA8(event, mode);
    }
    pathPlayer = D_0044F420_1BA8;
    pathPlayer->slots[idx] = 0;
    for (idx++; idx < 16; idx++) pathPlayer->slots[idx - 1] = pathPlayer->slots[idx];
    D_0044F420_1BA8->n91C--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D1D28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct P_1D28 { unsigned int f0; unsigned int f4; unsigned int f8; unsigned int fC; };
struct E_1D28 {
    unsigned int f0; unsigned int f4; unsigned int f8;
    unsigned int lo:16; unsigned int cnt:6; unsigned int idx:6; unsigned int hi:4;
    struct P_1D28 p[1];
};
struct G_1D28 { char pad[0xDC]; struct E_1D28 *arr[16]; };
extern struct G_1D28 *D_0044F420_1D28 __asm__("D_0044F420");
extern unsigned int D_0044F438_1D28 __asm__("D_0044F438");
extern int func_003CFC30_1D28(struct E_1D28 *, struct P_1D28 *) __asm__("func_003CFC30");

/**
 * Runs the due items of pending event @p n of the selected path player; returns how many it got through.
 *
 * From the event's current item (+0xC bits 22-27) to its item count (bits 16-21): an item time (+0x4) earlier than the
 * event's start (+0x0) is made absolute by adding the path tick D_0044F438, and an item whose time has passed is
 * flagged skipped (0x4000000 in +0x8). Items not skipped are executed by func_003CFC30; the first that returns 0 (not
 * ready yet) stops the walk. Each item passed advances the current index and stamps the event's +0x8 with the tick.
 *
 * @param n  the pending slot
 * @return the number of items executed or skipped
 */
int func_003D1D28(int n) {
    struct E_1D28 *event = D_0044F420_1D28->arr[n];
    unsigned int i = event->idx;
    int ret = 0;
    struct P_1D28 *item;
    if (i < event->cnt) {
        do {
            item = &event->p[i];
            if (item->f4 != 0) {
                if (item->f4 < event->f0) item->f4 += D_0044F438_1D28;
                if (item->f4 < D_0044F438_1D28) item->f8 |= 0x4000000;
            }
            if (!(item->f8 & 0x4000000)) {
                if (!func_003CFC30_1D28(event, item)) break;
            }
            ret++;
            i++;
            event->f8 = D_0044F438_1D28;
            event->idx++;
        } while (i < event->cnt);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1E80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
struct sPathVoice {                 /* 0x110, sPath.f58[] */
    char pad00;
    unsigned char f01;
    unsigned char f02;
    unsigned char f03;
    signed char f04;
    char pad05[0x8 - 0x5];
    int f08;
    signed char f0C;
    signed char f0D;
    signed char f0E;
    char pad0F;
    int f10;
    signed char f14;
    signed char f15;
    signed char f16;
    char pad17;
    int f18;
    signed char f1C;
    signed char f1D;
    signed char f1E;
    char pad1F;
    int lo : 4;
    int ch : 5;
    unsigned int hi : 23;
    int f24;
    char pad28[0x2C - 0x28];
    short f2C;
    char pad2E;
    unsigned char f2F;
    unsigned int *f30;
    signed char f34;
    char pad35;
    short f36;
    char pad38[0x48 - 0x38];
    unsigned int f48;
    unsigned int f4C;
    char pad50[0x54 - 0x50];
    int f54;
    char pad58[0x110 - 0x58];
};

struct sPathEvent {                 /* sPath.f0DC[] */
    unsigned int f00;
    unsigned int f04;
    unsigned int f08;
    union {
        unsigned int w;
        struct {
            unsigned int pad : 16;
            unsigned int end : 6;   /* number of 16-byte items after the header */
            unsigned int start : 6;
            unsigned int rest : 4;
        } b;
    } f0C;
};

struct sPathRamp { int dur; unsigned int mode; };

struct sPath {                      /* 0x928 */
    int f00;
    unsigned int f04;
    unsigned int f08;
    unsigned int f0C;
    unsigned int f10;
    struct sPathRamp f14[3];
    char pad2C[0x30 - 0x2C];
    unsigned int f30;
    unsigned int *f34;              /* the loaded path data (32-bit words) */
    short *f38;                     /* word offsets of its nodes */
    short *f3C;
    short *f40;
    short *f44;
    int *f48;
    int *f4C;
    char *f50;
    unsigned int *f54;
    struct sPathVoice *f58[24];
    int fB8;
    char padBC[0xDC - 0xBC];
    struct sPathEvent *f0DC[16];
    char buf[0x91C - 0x11C];
    int f91C;
    char pad920[0x924 - 0x920];
    int *f924;
};

extern struct sPath *D_0044F420;

extern unsigned int D_0044F438;         /* tick */

extern void f_1BA8_1E80(int, int) __asm__("func_003D1BA8");
extern int f_1D28_1E80(int) __asm__("func_003D1D28");

int func_003D1E80_impl(void) __asm__("func_003D1E80");
/**
 * Services the selected path player's pending events: drops finished and timed-out ones and runs the rest.
 *
 * Slots at or past the count (+0x91C) are cleared. An event whose current item reached its item count is removed with
 * mode 3 (func_003D1BA8), one with a timeout (+0x4) whose start (+0x0) plus timeout is before the tick D_0044F438 with
 * mode 1, and the loop steps back to re-check the slot; others run through func_003D1D28.
 *
 * @return the total number of items processed
 */
int func_003D1E80_impl(void) {
    int i;
    int sum = 0;
    struct sPathEvent *event;
    int st;
    unsigned int itemCounts;
    for (i = 0; i < 16; i++) {
        if (i >= D_0044F420->f91C) {
            D_0044F420->f0DC[i] = 0;
            continue;
        }
        event = D_0044F420->f0DC[i];
        if (event == 0) break;
        itemCounts = event->f0C.w;
        st = ((itemCounts >> 22) & 0x3F) < ((itemCounts >> 16) & 0x3F) ? 0 : 3;
        if (event->f04 != 0) {
            if (event->f00 + event->f04 < D_0044F438) st = 1;
        }
        if (st != 0) {
            f_1BA8_1E80(i, st);
            i--;
        } else {
            sum += f_1D28_1E80(i);
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D1F80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_1F80 { char pad[0x30]; unsigned int m_a; int m_b; char pad2[0xDC - 0x38]; char *ptrs[16]; char pad3[0x91C - 0x11C]; int count; char pad4[0x928 - 0x920]; };
extern int f2AF370_1F80(void) __asm__("func_002AF370");
extern struct P_1F80 D_7610_1F80[4] __asm__("D_00517610");

/**
 * Queues an already copied path event on every player in use whose class and group bits include @p mask's top byte.
 *
 * The event's start (+0x0) and last-run (+0x8) times are set to the current time (func_002AF370) and it is appended to
 * each matching player's pending list (+0xDC, count +0x91C).
 *
 * @param mask  the path id mask (only the top byte counts)
 * @param entry  the event (from PATHI_copyevent)
 * @return 0, -8 when no player matched, -13 at once when a list is full
 */
int func_003D1F80(unsigned int mask, char *entry) {
    unsigned int groupMask = mask & 0xFF000000;
    int i, ret = -8;
    for (i = 0; i < 4; i++) {
        if (D_7610_1F80[i].m_b != 0 && (D_7610_1F80[i].m_a & groupMask) == groupMask) {
            int slot = D_7610_1F80[i].count;
            if (slot >= 0x10) return -13;
            ret = 0;
            {
                int nowTime = f2AF370_1F80();
                *(int *)(entry + 8) = nowTime;
                *(int *)entry = nowTime;
            }
            D_7610_1F80[i].ptrs[slot] = entry;
            slot++;
            D_7610_1F80[i].count = slot;
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", PATHI_copyevent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F420_2068[] __asm__("D_0044F420");
extern int D_0044F43C_2068[] __asm__("D_0044F43C");
extern char str_PATHI_copyevent_can_t_add_new_event_size_2068[] __asm__("str_PATHI_copyevent_can_t_add_new_event_size");
extern void func_0041605C_2068(unsigned int, void *, int) __asm__("func_0041605C");

/**
 * Copies a path-interpreter event into free space of the event buffer, after the events the 16 slots already
 * use.
 *
 * The event's size is 16 bytes per unit in the low 6 bits of its word at +0xE, plus 16.
 *
 * @param event  the event to copy
 * @return the copy's address, or 0 (with a debug message when enabled) when the buffer is full
 */
unsigned int PATHI_copyevent(void *event) {
    int temp_9;
    unsigned int *eventSlot;
    unsigned int temp_3;
    unsigned int temp_6;
    unsigned int cur;
    unsigned int i;

    temp_6 = D_0044F420_2068[0] + 2332;
    cur = D_0044F420_2068[0] + 284;
    eventSlot = (unsigned int*)(D_0044F420_2068[0] + 0xDC);
    temp_9 = (((*(unsigned short*)((char*)event + 0xE)) & 0x3F) * 0x10) + 0x10;
    for (i = 0; i < 0x10; i++, eventSlot++) {
        temp_3 = *eventSlot;
        if (temp_3 >= cur) {
            if (temp_3 < temp_6) {
                int sz = (((*(unsigned short*)((char*)temp_3 + 0xE)) & 0x3F) * 0x10) + 0x10;
                cur += sz;
            }
        }
    }
    if (cur + temp_9 < temp_6) {
        func_0041605C_2068(cur, event, temp_9);
        return cur;
    }
    if (D_0044F43C_2068[0] & 0x120) {
        func_003D1658(str_PATHI_copyevent_can_t_add_new_event_size_2068, temp_9, temp_6 - cur);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2138);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct I2138 { int w0; int w4; unsigned int w8; int wC; };
struct E2138 { char pad[0xC]; unsigned char b; char padd; unsigned short h; struct I2138 items[1]; };
extern struct E2138 **D_0044F420_2138[] __asm__("D_0044F420");

/**
 * Returns the skip bit of the first item of a given type in the selected player's pending events of the same number.
 *
 * Looks through the 16 pending slots for an event whose number (byte +0xC) matches @p event's and whose items include
 * one of type @p itemType (byte +0x9); returns bit 26 (0x4000000, skipped) of that item's +0x8, else 0.
 *
 * @param event  an event (its number at +0xC)
 * @param itemType  the item type to look for
 * @return 1 when that item is skipped, 0 otherwise or when not found
 */
int func_003D2138(char *event, int itemType) {
    int i;
    unsigned int j;
    struct E2138 *pending;
    struct I2138 *it;
    for (i = 0; i < 16; i++) {
        pending = *(struct E2138 **)((char *)D_0044F420_2138[0] + 0xDC + i * 4);
        if (pending != 0 && pending->b == *(unsigned char *)(event + 0xC)) {
            for (j = 0; j < (pending->h & 0x3F); j++) {
                it = &pending->items[j];
                if (((unsigned char *)it)[9] == itemType) {
                    return (it->w8 >> 26) & 1;
                }
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D21C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *D_0044F420_f420[] __asm__("D_0044F420");

/**
 * Enables or disables an event definition in the selected player's path data: sets bit 29 (0x20000000) of its +0xC to
 * @p disable & 1.
 *
 * The definition is found through the event number at @p event +0xC: path data (+0x34) plus 4 times the event's offset
 * (+0x40 table). func_003D16F0 skips disabled events; func_003D2218 re-enables them all.
 *
 * @param event  an event (its number at +0xC)
 * @param disable  1 to disable, 0 to enable
 */
void func_003D21C0(char *event, int disable) {
    char *pathPlayer = D_0044F420_f420[0];
    char *eventDef = *(char **)(pathPlayer + 0x34) + (*(short *)(*(char **)(pathPlayer + 0x40) + *(unsigned char *)(event + 0xC) * 2) * 4);
    if (eventDef != 0) {
        *(unsigned *)(eventDef + 0xC) = (*(unsigned *)(eventDef + 0xC) & 0xDFFFFFFF) | ((disable & 1) << 29);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2218);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

struct E_2218 { int w0; int w4; int w8; unsigned int wC; };

/**
 * Re-enables every event definition in the selected player's path data: clears bit 29 (0x20000000) of each one's +0xC,
 * for the event count at data byte +0xF.
 */
void func_003D2218(void) {
    int i;
    for (i = 0; i < ((unsigned char *)D_0044F420->f34)[0xF]; i++) {
        struct E_2218 *eventDef = (struct E_2218 *)(D_0044F420->f34 + D_0044F420->f40[i]);
        if (eventDef != 0) {
            eventDef->wC &= 0xDFFFFFFF;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2290);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

extern struct sPath D_00517610[4];

extern unsigned char D_0044F42C;

extern unsigned char D_0044F42D;

extern unsigned int D_0044F430;

extern int D_0044F43C;                  /* flags */


/**
 * Initialises the path library: clears the 4 players and marks the system ready.
 *
 * Returns -18 while a selection is active (D_0044F430). Each player (D_00517610, 0x928 bytes) gets its in-use flag
 * (+0x34), +0x924 and its 24 voice pointers (+0x58) cleared; then D_0044F42C = 1 (initialised), the debug flags
 * D_0044F43C = 0x100, the selection D_0044F420 and the global pause D_0044F42D are reset.
 *
 * @return 0, or -18
 */
int func_003D2290(void) {
    int i, j;
    if (D_0044F430 != 0) return -18;
    for (i = 0; i < 4; i++) {
        D_00517610[i].f34 = 0;
        D_00517610[i].f924 = 0;
        for (j = 0; j < 24; j++) {
            D_00517610[i].f58[j] = 0;
        }
    }
    D_0044F42C = 1;
    D_0044F43C = 0x100;
    D_0044F420 = 0;
    D_0044F42D = 0;
    return 0;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D2350);

INCLUDE_ASM("ealib/seg_2B4578", func_003D25F0);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2A68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

extern struct sPath D_00517610[4];

extern unsigned char D_0044F42C;

extern unsigned int D_0044F430;

extern int f5450_2A68(int) __asm__("func_003D5450");
extern int func_003D5330(unsigned int mask);
extern void func_003D4C40(void);
extern void fAE100_2A68(void) __asm__("func_002AE100");
extern void fAE690_2A68(int) __asm__("func_002AE690");

static inline int isOff_2A68(unsigned int m, int b) { return ((m >> b) & 1) == 0; }
struct sPathVoice *func_003D2A68_impl(unsigned int mask) __asm__("func_003D2A68");
/**
 * Closes the first path voice a mask names, and shuts the player (and the path system) down when nothing is left.
 *
 * Returns 0 unless the path system is up and idle. Finds the first player in use whose class and group bits overlap
 * @p mask and, in it, the first voice whose bit is set in @p mask; the slot (+0x58) is cleared and func_002AE690
 * releases the voice's sound (+0x54). When that player has no voices left (func_003D5330) its in-use flag (+0x34) and
 * +0xB8 are cleared and func_003D4C40 re-sorts the players; when no group has voices any more and a voice was closed,
 * func_002AE100 stops the bank monitor's service and D_0044F42C is cleared. Likely bug: when nothing matches, the
 * "player empty" step still runs on the stale selection D_0044F420 (the loop index is 4, so func_003D5330 finds no
 * voices). The music monitor closes 0x11000001 and 0x11000002 on destruction.
 *
 * @param mask  the path id mask
 * @return the closed voice, or 0
 */
struct sPathVoice *func_003D2A68_impl(unsigned int mask) {
    struct sPathVoice *pathVoice = 0;
    int i, j;
    unsigned int fl, classBits, groupBits;
    int st;
    if (D_0044F42C == 0) return 0;
    if (D_0044F430 != 0) return 0;
    if (f5450_2A68(-1) == 0) return 0;
    for (i = 0; i < 4; i++) {
        if (D_00517610[i].f34 != 0) {
            fl = D_00517610[i].f30;
            classBits = fl & 0xF0000000;
            if ((classBits & mask) != 0) {
                groupBits = fl & 0x0F000000;
                if ((groupBits & mask) != 0 && f5450_2A68(i & 0xFF) != 0) {
                    for (j = 0; j < 24; j++) {
                        int handle;
                        pathVoice = D_0044F420->f58[j];
                        if (pathVoice == 0 || isOff_2A68(mask, j)) continue;
                        handle = pathVoice->f54;
                        D_0044F420->f58[j] = 0;
                        fAE690_2A68(handle);
                        goto done;
                    }
                }
            }
        }
    }
done:
    if (func_003D5330(0x1000000 << i) == 0) {
        D_0044F420->f34 = 0;
        D_0044F420->fB8 = 0;
        func_003D4C40();
    }
    st = func_003D5330(0x0F000000);
    if (D_0044F42C != 0 && st == 0 && pathVoice != 0) {
        fAE100_2A68();
        D_0044F42C = 0;
    }
    D_0044F430 = 0;
    D_0044F420 = 0;
    return pathVoice;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2C20);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D5450_2C20(int) __asm__("func_003D5450");
extern int func_003D4BB0_2C20(int) __asm__("func_003D4BB0");
extern double func_00413AF8_2C20(float) __asm__("fptodp");
extern double func_00411F18_2C20(double, double) __asm__("dpadd");
extern int func_004126A0_2C20(double) __asm__("dptoul");
extern char *D_0044F420_2C20[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_2C20[] __asm__("D_0044F42C");
extern int D_0044F430_2C20[] __asm__("D_0044F430");

static inline int bitclear_2C20(int x) {
    return !(x & 1);
}

/**
 * Returns a rounded scale of a path voice's last stream-segment ratio: (int)(ratio * @p multiplier + 0.5).
 *
 * Returns -18 unless the path system is up and idle. func_003D4BB0 selects the player with exactly @p pathId's top byte
 * (else 0). The voice is the lowest set bit of @p pathId's low 24 bits; past the data's voice count (byte +0xD of the path
 * data +0x34) it returns 0. For each entry pair of the voice's table (player +0x4C) the ratio is ((e[2] - e[0]) << 7) /
 * e[1]. Likely bug: the largest ratio is tracked but the last one is scaled; also both early returns of 0 leave the
 * busy flag D_0044F430 set, so later calls get -18.
 *
 * @param pathId  the path id
 * @param multiplier  the multiplier
 * @return the rounded product, 0 when the player or voice is missing, -18 when the system is not ready
 */
int func_003D2C20(int pathId, int multiplier) {
    float ratio, max;
    unsigned int i;
    char *pathPlayer;
    int *base;
    int *tab;
    int *segment;
    int rounded;

    ratio = 0.0f;
    max = 0.0f;
    if (D_0044F42C_2C20[0] == 0 || D_0044F430_2C20[0] != 0 || func_003D5450_2C20(-1) == 0) {
        return -18;
    }
    if (func_003D4BB0_2C20(pathId & 0xFF000000) == 0) {
        return 0;
    }
    i = 0;
    while (i < 24 && bitclear_2C20((long)pathId >> i)) {
        i++;
    }
    pathPlayer = D_0044F420_2C20[0];
    base = *(int **)(pathPlayer + 0x34);
    if (*((unsigned char *)base + 0xD) < i) {
        return 0;
    }
    tab = *(int **)(pathPlayer + 0x4C);
    for (segment = base + tab[i]; segment < base + tab[i + 1] - 2; segment += 2) {
        ratio = (float)((unsigned int)(segment[2] - segment[0]) << 7) / (float)(unsigned int)segment[1];
        if (max < ratio) {
            max = ratio;
        }
    }
    rounded = func_004126A0_2C20(func_00411F18_2C20(func_00413AF8_2C20(ratio * multiplier), 0.5));
    D_0044F430_2C20[0] = 0;
    D_0044F420_2C20[0] = 0;
    return rounded;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2DF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_2DF8 { unsigned int f0; unsigned int f4; unsigned int f8; };
struct R_2DF8 { unsigned short n; unsigned short type; unsigned int f4; unsigned int f8; };
struct T_2DF8 { char pad[0x54]; int f54; char pad2[4]; char f5C[0x104 - 0x5C]; unsigned int f104; unsigned int f108; };
extern struct T_2DF8 *func_003D5290_2DF8(int) __asm__("func_003D5290");
extern int func_003D5450_2DF8(int) __asm__("func_003D5450");
extern struct R_2DF8 *func_003D4A50_2DF8(int) __asm__("func_003D4A50");
extern int func_003D3140_2DF8(int, int) __asm__("func_003D3140");
extern int func_002AE3A8_2DF8(int) __asm__("func_002AE3A8");
extern int func_002AE298_2DF8(int, char *, int, int) __asm__("func_002AE298");
extern int D_0044F420_2DF8[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_2DF8[] __asm__("D_0044F42C");
extern int D_0044F430_2DF8[] __asm__("D_0044F430");

/**
 * Requests part @p idx on a path voice, first evicting a buffered part when the voice's limits would be exceeded.
 *
 * Returns -18 unless the path system is up and idle. With the voice (func_003D5290) and its request table
 * (func_003D4A50: count +0x0, policy +0x2, limits +0x4 and +0x8, then 12-byte entries), a policy other than 2 checks
 * the voice's totals (+0x104, +0x108) plus the part's sizes against the limits: policy 1 releases the entry with the
 * smallest first word (func_003D3140) unless that is @p idx itself (then it returns 0), policy 0 lets func_002AE3A8
 * free the oldest. Then func_002AE298 records the request on the voice's sound (+0x54) with +0x5C, @p idx and
 * @p userHandle. Likely bug: the early return for a self-eviction leaves the busy flag D_0044F430 set.
 *
 * @param pathId  the path id
 * @param idx  the part to request
 * @param userHandle  the caller's handle, stored with the request
 * @return func_002AE298's result, 0 when nothing was requested, -18 when the system is not ready
 */
int func_003D2DF8(int pathId, int idx, int userHandle) {
    int ret = 0;
    struct T_2DF8 *pathVoice;
    struct R_2DF8 *reqTable;
    struct E_2DF8 *reqEntry;

    if (D_0044F42C_2DF8[0] != 0) {
        if (D_0044F430_2DF8[0] != 0) {
            return -18;
        }
        if (func_003D5450_2DF8(-1) != 0) {
            goto ok;
        }
    }
    return -18;
ok:
    pathVoice = func_003D5290_2DF8(pathId);
    if (pathVoice != 0) {
        reqTable = func_003D4A50_2DF8(pathId);
        reqEntry = (struct E_2DF8 *)(reqTable + 1);
        reqEntry += idx;
        if (reqTable != 0 && reqTable->type != 2) {
            if (reqTable->f4 < pathVoice->f104 + reqEntry->f4 || reqTable->f8 < pathVoice->f108 + reqEntry->f8) {
                if (reqTable->type == 1) {
                    unsigned int min = 0x7FFFFFFF;
                    int sel = -1;
                    int i;
                    reqEntry = (struct E_2DF8 *)(reqTable + 1);
                    for (i = 0; i < reqTable->n; i++, reqEntry++) {
                        if (reqEntry->f0 <= min) {
                            min = reqEntry->f0;
                            sel = i;
                        }
                    }
                    if (sel == idx) {
                        return 0;
                    }
                    func_003D3140_2DF8(pathId, sel);
                } else if (reqTable->type == 0) {
                    func_002AE3A8_2DF8(pathId);
                }
            }
        }
        ret = func_002AE298_2DF8(pathVoice->f54, pathVoice->f5C, idx, userHandle);
    }
    D_0044F430_2DF8[0] = 0;
    D_0044F420_2DF8[0] = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D2FA0);
#ifdef SKIP_ASM
/**
 * Calls the empty hook func_003D16C8.
 */
void func_003D2FA0(void) {
    func_003D16C8();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D2FC0);
#ifdef SKIP_ASM
extern void func_003D16D0();

/**
 * Calls func_003D16D0 (which reads the debug flags D_0044F43C) and discards the value.
 */
void func_003D2FC0(void) {
    func_003D16D0();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D2FE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044F42C_2FE0 __asm__("D_0044F42C");
extern int D_0044F430_2FE0 __asm__("D_0044F430");
int func_003D2A68(int);
/**
 * Shuts the path system down: func_003D2A68(-1), then clears the initialised (D_0044F42C) and busy (D_0044F430) flags.
 * Quirk: func_003D2A68 closes only the first voice it finds.
 *
 * @return 0
 */
int func_003D2FE0(void) {
    func_003D2A68(-1);
    D_0044F42C_2FE0 = 0;
    D_0044F430_2FE0 = 0;
    return 0;
}
#endif

/**
 * Does nothing (an empty hook of the path library).
 */
void func_003D3010(void) {
}

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3018);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F424_3018[4] __asm__("D_0044F424");
extern int D_0044F428_3018[4] __asm__("D_0044F428");

/**
 * Installs the path library's two callbacks: the node-change callback (D_0044F424, called by func_003D3900 with the
 * selected id and the new node) and the event release callback (D_0044F428, called by func_003D1BA8 for events outside
 * the player's buffer).
 *
 * @param nodeCallback  the node-change callback, or 0
 * @param releaseCallback  the event release callback, or 0
 */
void func_003D3018(int nodeCallback, int releaseCallback) {
    D_0044F424_3018[0] = nodeCallback;
    D_0044F428_3018[0] = releaseCallback;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3030);
#ifdef SKIP_ASM
/**
 * Returns the sound-monitor index (+0x54) of the path voice func_003D5290 finds, or -8.
 *
 * Declared without parameters: the caller's id reaches func_003D5290 through $a0 (func_002B2488 passes 0x11000001,
 * func_003D53B8 its mask).
 *
 * @return the voice's monitor index, or -8 when no voice matches
 */
int func_003D3030(void) {
    char *temp_2;

    temp_2 = (char *)func_003D5290();
    if (temp_2 == 0) {
        return -8;
    }
    return *(int *)(temp_2 + 0x54);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3058);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002AE478_3058() __asm__("func_002AE478");

/**
 * Returns the memory a path player needs for @p n voices: func_002AE478's monitor size + 272 + 16 * n, rounded down to
 * 16 and plus 16.
 *
 * func_002AE478 is called without arguments here, so it gets @p n and the caller's second argument through the
 * registers (the music monitor calls func_003D3058(1, 0) for its "PathfinderOverlay" buffer).
 *
 * @param n  the number of voices
 * @return the size in bytes
 */
int func_003D3058(int n) {
    int size = func_002AE478_3058() + 272;
    size += n * 0x10;
    return (size / 16) * 16 + 0x10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D30A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_002AE768_30A8(int, int, int) __asm__("func_002AE768");
extern void *func_003D5290_30A8(int) __asm__("func_003D5290");
extern int func_003D5450_30A8(int) __asm__("func_003D5450");
extern int D_0044F420_30A8[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_30A8[] __asm__("D_0044F42C");
extern int D_0044F430_30A8[] __asm__("D_0044F430");

/**
 * Gives a path voice's sound monitor a memory-resident copy of its data: func_002AE768(monitor index, @p memBuffer,
 * @p memLimit).
 *
 * The monitor index is the voice's +0x54 (-8 when func_003D5290 finds no voice, which func_002AE768 rejects). Stream
 * requests below the limit are then read from the buffer instead of the file. Does nothing unless the path system is up
 * and idle; clears the selection at the end.
 *
 * @param pathId  the path id
 * @param memBuffer  the buffer
 * @param memLimit  the stream offset limit the buffer covers (in the monitor's units)
 */
void func_003D30A8(int pathId, int memBuffer, int memLimit) {
    int var_4;
    void *temp_2;

    if ((D_0044F42C_30A8[0] != 0) && (D_0044F430_30A8[0] == 0) && (func_003D5450_30A8(-1) != 0)) {
        temp_2 = func_003D5290_30A8(pathId);
        var_4 = -8;
        if (temp_2 != 0) {
            var_4 = (*(int *)((char*)(temp_2) + (0x54)));
        }
        func_002AE768_30A8(var_4, memBuffer, memLimit);
        D_0044F430_30A8[0] = 0;
        D_0044F420_30A8[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3140);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002AE328_3140(int, int) __asm__("func_002AE328");
extern char *func_003D5290_3140(int) __asm__("func_003D5290");
extern int func_003D5450_3140(int) __asm__("func_003D5450");
extern int D_0044F420_3140[] __asm__("D_0044F420");
extern unsigned char D_0044F42C_3140[] __asm__("D_0044F42C");
extern int D_0044F430_3140[] __asm__("D_0044F430");
int func_003D3140_3140(int arg0, int arg1) __asm__("func_003D3140");

/**
 * Releases request @p requestIndex of a path voice's sound monitor (func_002AE328 on the voice's +0x54).
 *
 * Returns -18 unless the path system is up and idle; clears the selection at the end.
 *
 * @param pathId  the path id
 * @param requestIndex  the request index
 * @return func_002AE328's result, 0 when no voice matches, -18 when the system is not ready
 */
int func_003D3140_3140(int pathId, int requestIndex) {
    int result;
    char *pathVoice;
    result = 0;
    if (D_0044F42C_3140[0] != 0) {
        if (D_0044F430_3140[0] != 0) {
            return -18;
        }
        if (func_003D5450_3140(-1) != 0) {
            goto ok;
        }
    }
    return -18;
ok:
    pathVoice = func_003D5290_3140(pathId);
    if (pathVoice != 0) {
        result = func_002AE328_3140(*(int *)(pathVoice + 0x54), requestIndex);
    }
    D_0044F430_3140[0] = 0;
    D_0044F420_3140[0] = 0;
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D31E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct S_31E8 { char p0[0xC]; int f0c; int f10; char p1[0x1C]; unsigned int f30; int f34; char p2[0x928 - 0x38]; };
extern struct S_31E8 D_00517610_31E8[] __asm__("D_00517610");
extern unsigned char D_0044F42C_31E8[] __asm__("D_0044F42C");
extern int D_0044F430_31E8[] __asm__("D_0044F430");
extern int D_0044F420_31E8[] __asm__("D_0044F420");

/**
 * Sets the two look-ahead times of every path player in use whose group bits overlap @p pathMask: +0xC (used by the normal
 * service pass) and +0x10 (used by the 'B' pass of func_003D4F10).
 *
 * Does nothing unless the path system is up and idle; clears the selection at the end.
 *
 * @param pathMask  the path id mask (only the group bits 0x0F000000 count)
 * @param lookAhead  the look-ahead for +0xC
 * @param lookAheadB  the look-ahead for +0x10
 */
void func_003D31E8(unsigned int pathMask, int lookAhead, int lookAheadB) {
    int i;
    if (D_0044F42C_31E8[0] != 0 && D_0044F430_31E8[0] == 0) {
        if (func_003D5450(-1)) {
            pathMask &= 0x0F000000;
            for (i = 0; i < 4; i++) {
                if (D_00517610_31E8[i].f34 != 0 && (D_00517610_31E8[i].f30 & pathMask) != 0) {
                    D_00517610_31E8[i].f0c = lookAhead;
                    D_00517610_31E8[i].f10 = lookAheadB;
                }
            }
            D_0044F430_31E8[0] = 0;
            D_0044F420_31E8[0] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D32A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044ED48_32A8[4] __asm__("D_0044ED48");

/**
 * Returns the address of D_0044ED48 (a 16-byte block of the path library).
 *
 * @return D_0044ED48
 */
int *func_003D32A8(void) {
    return D_0044ED48_32A8;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D32B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct T_32B8 { int a; unsigned int len; };
struct E_32B8 {
    short h0;
    unsigned short h2;
    unsigned int w4;
    unsigned int w8;
    unsigned int wC;
};
struct V_32B8 {
    char pad0[0x20];
    unsigned int lo : 4;
    int ch : 5;
    unsigned int hi : 23;
    char pad24[0x36 - 0x24];
    short h36;
    char pad38[0x40 - 0x38];
    unsigned int x40;
    char pad44[0x54 - 0x44];
    int x54;
};
struct O_32B8 {
    unsigned int lo : 4;
    unsigned int f4 : 4;
    unsigned int n : 8;
    unsigned int rest : 16;
    int w4;
    int w8;
    unsigned int wC;
    int w10;
    int w14;
    int w18;
};
struct G_32B8 {
    char pad0[0x34];
    char *hdr;
    short *a;
    char pad3C[0x50 - 0x3C];
    struct T_32B8 *tab;
};
extern struct G_32B8 *D_0044F420_32B8 __asm__("D_0044F420");
extern int fAEC58_32B8(int, unsigned int *, unsigned int *) __asm__("func_002AEC58");

int func_003D32B8_32B8(struct V_32B8 *v, struct O_32B8 *o) __asm__("func_003D32B8");

/**
 * Works out where a path voice is within its current node: returns the segment index and optionally fills the timing.
 *
 * The node record is the selected player's path data (+0x34) at 4 * its offset (+0x38 table) for the voice's node
 * (+0x36); its length entry is player table +0x50 [record +0x0 - 1]. The segment length is that length divided by the
 * node's channel count times multiplier (bits 24-27 and 20-23 of record +0x4) when the multiplier is set; the
 * per-channel length divides by bits 20-23 only. func_002AEC58 gives the voice's stream positions (+0x54). For a voice
 * on a channel (+0x20 bits 4-8 > 0) of a node with flag 2 (+0xC) the start is offset by +0x40 per remaining channel and
 * the end is length - start. @p timing gets the multiplier (bits 0-3), record +0x8 bits 4-7, the segment index (bits 8-15),
 * the segment length (+0x4), per-channel length (+0x8), total (+0xC), the distance to the next segment boundary
 * (+0x10), per-channel length minus the end position (+0x14) and the start (+0x18).
 *
 * @param pathVoice  the path voice
 * @param timing  the timing to fill, or 0
 * @return the index of the segment the voice is in
 */
int func_003D32B8_32B8(struct V_32B8 *pathVoice, struct O_32B8 *timing) {
    struct E_32B8 *nodeRec;
    struct T_32B8 lenEntry;
    unsigned int end;
    unsigned int start;
    float len;
    float rate;
    float startPos;
    float endPos;
    float x;
    float p;
    float q;
    float toNextSeg;
    int n;

    nodeRec = (struct E_32B8 *)(D_0044F420_32B8->hdr + D_0044F420_32B8->a[pathVoice->h36] * 4);
    lenEntry = D_0044F420_32B8->tab[nodeRec->h0 - 1];
    len = lenEntry.len;
    if (nodeRec->w4 & 0x0F000000) {
        len /= (unsigned int)(((nodeRec->w4 >> 24) & 0xF) * ((nodeRec->w4 >> 20) & 0xF));
    }
    rate = (float)lenEntry.len / (float)((nodeRec->w4 >> 20) & 0xF);
    fAEC58_32B8(pathVoice->x54, &end, &start);
    if (pathVoice->ch > 0 && (nodeRec->wC & 2)) {
        startPos = (float)start + (float)pathVoice->x40 * (float)(unsigned int)(((nodeRec->w4 >> 24) & 0xF) * ((nodeRec->w4 >> 20) & 0xF) + (1 - pathVoice->ch));
        endPos = (float)lenEntry.len - startPos;
    } else {
        startPos = start;
        endPos = end;
    }
    x = endPos / len;
    n = (int)x;
    p = x * len;
    q = rate - p;
    toNextSeg = (float)(n + 1) * len - p;
    if (timing != 0) {
        timing->lo = (nodeRec->w4 >> 24) & 0xF;
        timing->f4 = (nodeRec->w8 >> 4) & 0xF;
        timing->w18 = (int)startPos;
        timing->wC = lenEntry.len;
        timing->w4 = (int)len;
        timing->w8 = (int)rate;
        timing->w14 = (int)q;
        timing->w10 = (int)toNextSeg;
        timing->n = n;
    }
    return n;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D3658);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D37A0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

struct R_37A0 { signed char lo; signed char hi; short v; };
/**
 * Picks the node that follows path node @p node for a selector value, from the node's branch table.
 *
 * The node record (selected player's data +0x34 at its +0x38 offset) names its voice (bits 16-20 of word 0). When that
 * voice is repeating this node (+0x2C), the selector becomes the voice's remaining repeat count (+0x20 low bits), which
 * is decremented for nodes with a repeat count (bits 28-31) and ends the repeat (+0x2C = -1) when it passes 0. The
 * branches (4 bytes each from record +0x10, count in bits 12-16 of word 1) hold a selector range and a target: the
 * first range containing the selector wins, else the branch with the nearest range edge.
 *
 * @param node  the current node
 * @param selector  the selector value (0..127)
 * @return the next node, or -1 when the node has no branches
 */
int func_003D37A0(int node, int selector) {
    struct sPath *pathPlayer = D_0044F420;
    int nextNode = -1;
    unsigned int *nodeRec = pathPlayer->f34 + pathPlayer->f38[node];
    unsigned int w0 = nodeRec[0];
    struct sPathVoice *pathVoice = pathPlayer->f58[(w0 >> 16) & 0x1F];
    struct R_37A0 *branch = (struct R_37A0 *)(nodeRec + 4);
    unsigned int i;
    if (pathVoice->f2C == node) {
        selector = pathVoice->lo & 0x7F;
        if ((int)w0 >> 28 > 0) {
            if (--pathVoice->lo == -1) {
                pathVoice->f2C = -1;
            }
        }
    }
    for (i = 0; i < ((nodeRec[1] >> 12) & 0x1F); i++) {
        if (selector >= branch[i].lo && branch[i].hi >= selector) {
            nextNode = branch[i].v;
            break;
        }
    }
    if (nextNode < 0) {
        int n = (nodeRec[1] >> 12) & 0x1F;
        if (n <= 0) {
            nextNode = -1;
        } else {
            struct R_37A0 *best = branch;
            int bd = 0x7F;
            int k;
            for (k = 0; k < n; k++) {
                int loDist = branch[k].lo - selector;
                int hiDist = branch[k].hi - selector;
                int nearDist;
                if (loDist < 0) loDist = -loDist;
                if (hiDist < 0) hiDist = -hiDist;
                nearDist = loDist < hiDist ? loDist : hiDist;
                if (nearDist < bd) {
                    bd = nearDist;
                    best = &branch[k];
                }
            }
            nextNode = (unsigned short)best->v;
        }
    }
    return nextNode;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D3900);
#ifdef SKIP_ASM
// PORT: 64-bit `long` bitfields (8 bytes on EE); use uint64_t off-PS2.
struct E_3900 {
    short type;
    unsigned short h2;
    unsigned int w4;
    unsigned int w8;
    unsigned int wC;
};
struct V_3900 {
    char pad0[0x20];
    unsigned int lo : 4;
    int ch : 5;
    unsigned int hi : 23;
    char pad24[0x2C - 0x24];
    short h2C;
    char pad2E[2];
    struct E_3900 *cur;
};
struct G_3900 {
    char pad0[0x30];
    unsigned int x30;
    char *hdr;
    short *a;
    char pad3C[4];
    short *b;
    char pad44[0x54 - 0x44];
    unsigned int *tab54;
    struct V_3900 *voices[32];
};
struct Ev_3900 {
    int w0;
    int w4;
    signed char b8;
    unsigned char b9;
    short pad;
    unsigned char bC;
    unsigned long f7 : 7;
    unsigned long f1 : 1;
    unsigned short hE;
};
struct Q_3900 {
    int p0;
    int p1;
    int p2;
    unsigned long lo : 12;
    unsigned long b : 4;
    unsigned long idx : 6;
    unsigned long hi : 10;
};
struct P_3900 { char pad[0x30]; unsigned int x30; char pad2[0x928 - 0x34]; };
extern struct G_3900 *D_0044F420_3900 __asm__("D_0044F420");
extern void (*D_0044F424_3900)(int, int) __asm__("D_0044F424");
extern int D_0044F430_3900 __asm__("D_0044F430");
extern unsigned int D_0044F438_3900 __asm__("D_0044F438");
extern struct P_3900 D_00517610_3900[4] __asm__("D_00517610");
extern int f3C90_3900(int, int) __asm__("func_003D3C90");
extern int f37A0_3900(int, int) __asm__("func_003D37A0");
extern void f6210_3900(void *, int, int) __asm__("memset");
extern void f0B18_3900(void *, void *) __asm__("func_003D0B18");
extern char *fcopy_3900(void *) __asm__("PATHI_copyevent");
extern int f1F80_3900(unsigned int, char *) __asm__("func_003D1F80");

/**
 * Follows a path from node @p start through its control nodes until it reaches a playable node, and returns that node.
 *
 * Every node number is first remapped through node @p id's list (func_003D3C90). While the record's type (+0x0) is 0 or
 * less: type 0 makes it the voice's current record (+0x30); when its +0x8 bits 10-14 name a cue, a timed type-6 item
 * (time tick + 9632, cue index, flag from player table +0x54) is built (func_003D0B18), copied (PATHI_copyevent) and
 * queued (func_003D1F80); a random-selector flag (+0x4 bits 17-19 == 2) draws the selector from the tick, and the
 * node-change callback D_0044F424 (func_003D3018) is called. Type -1 starts a repeat on its voice (+0x2C node, count
 * from bits 28-31), type -2 draws a random selector, type -3 queues the event its +0xC names. The next node is
 * func_003D37A0's branch for the selector, which then reverts to @p vel.
 *
 * @param id  the node whose remap list applies
 * @param start  the first node
 * @param vel  the selector value
 * @return the playable node reached (remapped)
 */
int func_003D3900(int id, int start, int vel) {
    struct Ev_3900 ev;
    struct Q_3900 eventHeader;
    struct E_3900 *nodeRec;
    struct V_3900 *pathVoice;
    unsigned int t;
    unsigned int w;
    short k;
    short type;
    int save;

    save = vel;
    k = f3C90_3900(id, start);
    nodeRec = (struct E_3900 *)(D_0044F420_3900->hdr + D_0044F420_3900->a[k] * 4);
    pathVoice = D_0044F420_3900->voices[nodeRec->h2 & 0x1F];
    while (nodeRec->type <= 0) {
        type = nodeRec->type;
        if (type == 0) {
            pathVoice->cur = nodeRec;
            if (nodeRec->w8 & 0x7C00) {
                f6210_3900(&eventHeader, 0, 0x10);
                f6210_3900(&ev, 0, 0x10);
                t = D_0044F420_3900->tab54[((nodeRec->w8 >> 10) & 0x1F) - 1];
                if (t & 0x80) {
                    ev.bC = 100;
                } else {
                    ev.bC = 0;
                }
                ev.f1 = (t >> 7) & 1;
                ev.b9 = 6;
                ev.hE = 9632;
                ev.f7 = ((nodeRec->w8 >> 10) & 0x1F) - 1;
                ev.b8 = -1;
                eventHeader.p1 = ev.hE;
                ev.w4 = D_0044F438_3900 + ev.hE;
                ev.w0 = 0x1F000001;
                eventHeader.b = 0;
                f0B18_3900(&eventHeader, &ev);
                f1F80_3900(D_00517610_3900[0].x30, fcopy_3900(&eventHeader));
            }
            if ((nodeRec->w4 & 0xE0000) == 0x20000) {
                vel = (D_0044F438_3900 / 23) & 0x7F;
            }
            if (D_0044F424_3900 != 0) {
                D_0044F424_3900(D_0044F430_3900, k);
            }
        } else if (type == -1) {
            w = *(unsigned int *)nodeRec;
            if (w & 0xF0000000) {
                if (k != pathVoice->h2C) {
                    pathVoice->h2C = k;
                    pathVoice->lo = w >> 28;
                }
            }
        } else if (type == -2) {
            vel = (D_0044F438_3900 / 23) & 0x7F;
        } else if (type == -3) {
            f1F80_3900(D_0044F420_3900->x30, (D_0044F420_3900->hdr + D_0044F420_3900->b[(nodeRec->wC >> 2) & 0xFFF] * 4));
        }
        t = f37A0_3900(k, vel);
        vel = save;
        k = f3C90_3900(id, t);
        nodeRec = (struct E_3900 *)(D_0044F420_3900->hdr + D_0044F420_3900->a[k] * 4);
    }
    return (short)f3C90_3900(id, k);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D3C90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Looks up id's remap list in the table at D_0044F420 and returns what value maps to, or value.
struct T_3C90 { char pad[0x34]; int *words; short *index; char pad2[0xC]; int *ranges; };
extern struct T_3C90 *D_0044F420_3C90 __asm__("D_0044F420");
/**
 * Remaps a node number through node @p id's remap list; returns @p value unchanged when there is none.
 *
 * The list number is byte +0x4 of the node's record (selected player's data +0x34 at its +0x38 offset); 0 means no
 * list. The list's words lie between ranges (player +0x48) [k - 1] and [k]; each maps its high half to its low half,
 * applied in order, so mappings can chain. A negative @p id returns @p value.
 *
 * @param id  the node owning the list
 * @param value  the node number to remap
 * @return the remapped node number
 */
int func_003D3C90(int id, int value) {
    struct T_3C90 *pathPlayer;
    int *words;
    int k, start, n, w, i;
    int *remapList;
    int mapped = value;
    if (id < 0)
        return mapped;
    pathPlayer = D_0044F420_3C90;
    words = pathPlayer->words;
    k = *((unsigned char *)&words[pathPlayer->index[id]] + 4);
    if (k == 0)
        return mapped;
    start = pathPlayer->ranges[k - 1];
    n = pathPlayer->ranges[k] - start;
    remapList = &words[start];
    for (i = 0; i < n; i++) {
        w = remapList[i];
        if (mapped == (w >> 16))
            mapped = w & 0xFFFF;
    }
    return mapped;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D3D20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V_3D20 {
    char pad0;
    unsigned char b1;
    char pad2[0x20 - 2];
    unsigned int lo : 4;
    int ch : 5;
    unsigned int hi : 23;
    char pad24[0x2F - 0x24];
    unsigned char b2F;
    char pad30[6];
    short h36;
    char pad38[0x110 - 0x38];
};
struct E_3D20 { unsigned short h0; unsigned short h2; unsigned int w4; int w8; int wC; };
struct G_3D20 {
    char pad0[0x34];
    char *hdr;
    short *a;
    char pad3C[0x58 - 0x3C];
    struct V_3D20 *voices[32];
    char padD8[0x91C - 0xD8];
    int x91C;
};
extern struct G_3D20 *D_0044F420_3D20 __asm__("D_0044F420");
extern int f_37A0_3D20(int, int) __asm__("func_003D37A0");
extern int f_3900_3D20(int, int, int) __asm__("func_003D3900");
extern void f_4050_3D20(int) __asm__("func_003D4050");
extern int f_41A8_3D20(struct V_3D20 *) __asm__("func_003D41A8");

/**
 * Moves path voice @p idx of the selected player on to its next node and queues it, or hands the path to another voice.
 *
 * Saves the voice and the pending-event count. A voice on a channel of a node with flag 2 (+0xC) stays on that node for
 * the next channel; otherwise func_003D37A0 picks the branch for its selector (+0x1), and func_003D3900 walks the
 * control nodes. If the node reached belongs to another existing voice, this voice stops (+0x36 = -1) and func_003D4050
 * posts the hand-over; otherwise the node becomes the voice's (+0x36) and func_003D41A8 queues it. When that fails
 * (negative, other than -9999) the voice and the event count are restored.
 *
 * @param idx  the voice slot (0..23)
 */
void func_003D3D20(int idx) {
    struct V_3D20 save;
    struct V_3D20 *pathVoice;
    struct V_3D20 *w;
    struct E_3D20 *nodeRec;
    int old;
    int ch;
    int sel;
    int n;
    int k;
    int queueResult;

    sel = -1;
    old = D_0044F420_3D20->x91C;
    pathVoice = D_0044F420_3D20->voices[idx];
    save = *pathVoice;
    ch = -1;
    k = pathVoice->h36;
    nodeRec = (struct E_3D20 *)(D_0044F420_3D20->hdr + D_0044F420_3D20->a[k] * 4);
    n = pathVoice->ch;
    if (pathVoice->b2F) {
        ch = 1;
    }
    if (n > 0 && (nodeRec->wC & 2) && (int)(((nodeRec->w4 >> 24) & 0xF) * ((nodeRec->w4 >> 20) & 0xF)) >= n) {
        sel = k;
        ch = n;
    } else if (pathVoice->h36 >= 0) {
        sel = f_37A0_3D20(pathVoice->h36, pathVoice->b1);
    }
    if (sel >= 0) {
        sel = f_3900_3D20(pathVoice->h36, sel, pathVoice->b1);
    }
    nodeRec = (struct E_3D20 *)(D_0044F420_3D20->hdr + D_0044F420_3D20->a[sel] * 4);
    w = D_0044F420_3D20->voices[nodeRec->h2 & 0x1F];
    if (w != 0 && w != pathVoice) {
        pathVoice->h36 = -1;
        pathVoice->ch = ch;
        f_4050_3D20(sel);
        return;
    }
    pathVoice->h36 = sel;
    pathVoice->ch = ch;
    queueResult = f_41A8_3D20(pathVoice);
    if (queueResult >= 0 || queueResult == -9999) {
        return;
    }
    D_0044F420_3D20->x91C = old;
    *pathVoice = save;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4050);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_003D0B18_4050(void *, void *) __asm__("func_003D0B18");
extern void func_003D1F80_4050(int, int) __asm__("func_003D1F80");
extern int func_003D2068_4050(void *) __asm__("PATHI_copyevent");
extern void func_003D32B8_4050(int, void *) __asm__("func_003D32B8");
extern void *memset_4050(void *, int, int) __asm__("memset");
extern char *D_0044F420_4050[] __asm__("D_0044F420");
struct M_4050 {
    int w0;
    int w4;
    unsigned char b8;
    unsigned char b9;
    unsigned char ba, bb;
    short sc;
    char be;
    char bf;
};
struct G_4050 { char pad[0x58]; int a[1]; };
struct B_4050 { char pad[0x18]; int w18; char rest[4]; };

/**
 * Posts an event that starts path node @p idx on its own voice, after the current voice's segment when one is playing.
 *
 * Does nothing when the node's voice slot (record +0x2 bits 0-4) is empty. The event, for id 0x11000000 | (1 << voice)
 * (always class 1, group 0), gets a type-2 item timed 10 before the current voice's segment start (func_003D32B8 on
 * voice player +0x0) when the player's current voice (+0x0) is set, then a type-4 item for node @p idx (+0xC) with the
 * node's bits 21-26 at +0x8. PATHI_copyevent copies it and func_003D1F80 queues the copy.
 *
 * @param idx  the node
 * @return the queued copy, or 0
 */
int func_003D4050(int idx) {
    struct M_4050 item;
    struct M_4050 eventHeader;
    struct B_4050 buf;
    char *pathPlayer = D_0044F420_4050[0];
    char *nodeRec = *(char **)(pathPlayer + 0x34) + *(short *)(*(char **)(pathPlayer + 0x38) + idx * 2) * 4;
    int eventCopy = 0;
    int k, voiceMask;
    if (nodeRec != 0) {
        k = *(unsigned short *)(nodeRec + 2) & 0x1F;
        if (((struct G_4050 *)pathPlayer)->a[k] != 0) {
            voiceMask = (int)((1L << k) | 0x11000000);
            memset_4050(&eventHeader, 0, 0x10);
            memset_4050(&item, 0, 0x10);
            if (*(int *)D_0044F420_4050[0] >= 0) {
                func_003D32B8_4050(((struct G_4050 *)D_0044F420_4050[0])->a[*(int *)D_0044F420_4050[0]], &buf);
                item.b9 = 2;
                item.w0 = voiceMask;
                *(int *)&item.sc = buf.w18 - 10;
                func_003D0B18_4050(&eventHeader, &item);
            }
            item.b9 = 4;
            item.w0 = voiceMask;
            item.b8 = (*(unsigned int *)nodeRec >> 21) & 0x3F;
            item.sc = idx;
            item.be = -1;
            func_003D0B18_4050(&eventHeader, &item);
            eventCopy = func_003D2068_4050(&eventHeader);
            func_003D1F80_4050(voiceMask, eventCopy);
        }
    }
    return eventCopy;
}
#endif

INCLUDE_ASM("ealib/seg_2B4578", func_003D41A8);

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4898);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
typedef struct E_4898 { short v; short w; } E_4898;
typedef struct B_4898 { E_4898 ent[0x40]; } B_4898;
typedef struct A_4898 { char pad0[0x34]; B_4898 *b; short *idx; char pad1[0x50 - 0x3C]; char *arr; } A_4898;
extern A_4898 *D_0044F420_F420[] __asm__("D_0044F420");

static inline int get_4898(char *arr, int i) {
    return *(int *)(arr + i * 8);
}

/**
 * Returns the first word of path node @p node's length entry, or 0.
 *
 * Valid nodes are below the path data's node count (+0x12); the record's +0x0 is the 1-based index into the player's
 * length table (+0x50, 8 bytes per entry), 0 meaning none.
 *
 * @param node  the node
 * @return the entry's first word, 0 for an invalid node or no entry
 */
int func_003D4898(int node) {
    int var_7;
    A_4898 *pathPlayer;
    B_4898 *pathData;
    short lenIndex;

    var_7 = 0;
    if (node < 0) return 0;
    pathPlayer = D_0044F420_F420[0];
    pathData = pathPlayer->b;
    if (node >= *(unsigned short *)((char *)pathData + 0x12)) return 0;
    lenIndex = pathData->ent[pathPlayer->idx[node]].v;
    if (lenIndex > 0) {
        var_7 = get_4898(pathPlayer->arr, lenIndex - 1);
    }
    return var_7;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4900);
#ifdef SKIP_ASM
struct S_4900 {
    int a0;
    int a4;
    int a8;
    int ac;
    unsigned int a10;
    int a14;
    int a18;
    int a1c;
};
extern int func_003D32B8(int, struct S_4900 *);

/**
 * Returns how far a path voice is from its next segment boundary, and optionally the segment length.
 *
 * Uses func_003D32B8's timing; a negative result there would give length 0 and -1.
 *
 * @param pathVoice  the path voice
 * @param segLenOut  receives the segment length, or 0
 * @return the distance to the next segment boundary
 */
int func_003D4900(int pathVoice, int *segLenOut) {
    struct S_4900 timing;

    if (func_003D32B8(pathVoice, &timing) < 0) {
        timing.a4 = 0;
        timing.a10 = 0xFFFFFFFF;
    }
    if (segLenOut != 0) {
        *segLenOut = timing.a4;
    }
    return timing.a10;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4950);
#ifdef SKIP_ASM
extern int func_002AEAA8(int, unsigned int);

/**
 * Returns the total amount a path voice has queued on its sound monitor: the sum of func_002AEAA8 over the voice's
 * request entries (+0x20 bits 14-18), skipping negative results.
 *
 * @param pathVoice  the path voice
 * @return the total
 */
int func_003D4950(char *pathVoice) {
    int temp_2;
    int total = 0;
    unsigned int i;

    for (i = 0; i < ((*(unsigned int *)(pathVoice + 0x20) >> 0xE) & 0x1F); i++) {
        temp_2 = func_002AEAA8(*(int *)(pathVoice + 0x54), i);
        if (temp_2 >= 0) {
            total += temp_2;
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D49D0);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Returns v of the entry whose a/b byte is closest to t (first 127-or-better wins), -1 for an empty table.
struct E_49D0 { signed char a; signed char b; unsigned short v; };
/**
 * Returns the target of the branch whose selector range edge is nearest @p target, or -1 for an empty table.
 *
 * Each 4-byte entry holds a low and a high selector byte and a target; the distance is the smaller of the two edge
 * distances and the first entry under 0x7F that improves on the best wins. The same search is inlined in func_003D37A0
 * as its fallback.
 *
 * @param n  the number of entries
 * @param target  the selector value
 * @param tab  the branch entries
 * @return the chosen target, -1 when @p n is 0 or less
 */
int func_003D49D0(int n, int target, struct E_49D0 *tab) {
    int best;
    struct E_49D0 *bp = tab;
    int i;
    if (n <= 0) return -1;
    best = 0x7F;
    for (i = 0; i < n; i++) {
        int distA = tab[i].a - target;
        int distB = tab[i].b - target;
        int dist;
        if (distA <= -1) distA = -distA;
        if (distB <= -1) distB = -distB;
        dist = distA < distB ? distA : distB;
        if (dist < best) {
            best = dist;
            bp = &tab[i];
        }
    }
    return bp->v;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4A50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_4A50 { char pad[0x30]; unsigned mask; int w34; char pad2[0x8EC]; int *tab; };
extern struct E_4A50 D_00517610_4A50[] __asm__("D_00517610");

/**
 * Returns the request table of the path voice an id names: player +0x924 [voice] for the lowest voice bit of
 * @p pathId, in the first player with tables, in use, sharing class and group bits.
 *
 * @param pathId  the path id
 * @return the voice's request table, or 0
 */
int func_003D4A50(unsigned pathId) {
    unsigned i, voiceBit;
    unsigned hi = pathId & 0xF0000000;
    unsigned mid = pathId & 0x0F000000;
    unsigned voiceBits = pathId & 0xFFFFFF;
    for (i = 0; i < 4; i++) {
        unsigned mk = D_00517610_4A50[i].mask;
        if (D_00517610_4A50[i].tab != 0 && D_00517610_4A50[i].w34 != 0 &&
            (mk & hi) && (mk & mid)) {
            for (voiceBit = 0; voiceBit < 0x18; voiceBit++) {
                if ((voiceBits >> voiceBit) & 1)
                    return D_00517610_4A50[i].tab[voiceBit];
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4B00);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;


static inline int valid_4B00(struct sPathVoice *e) { return e->f36 >= 0 && e->f30 != 0; }

/**
 * Records the selected path player's leading voice: the active voice with the largest position (+0x24).
 *
 * Active means not paused (+0x3 == 0) with a node (+0x36 >= 0) and a record (+0x30). The player's +0x0 gets its index
 * (-1 when none), +0x4 its position and +0x8 bits 21-26 of its record's first word.
 */
void func_003D4B00(void) {
    int i;
    D_0044F420->f00 = -1;
    D_0044F420->f04 = 0;
    D_0044F420->f08 = 0;
    for (i = 0; i < 24; i++) {
        struct sPathVoice *pathVoice = D_0044F420->f58[i];
        if (pathVoice != 0 && pathVoice->f03 == 0 && valid_4B00(pathVoice) && D_0044F420->f04 < pathVoice->f24) {
            D_0044F420->f04 = pathVoice->f24;
            D_0044F420->f00 = i;
            D_0044F420->f08 = (*pathVoice->f30 >> 21) & 0x3F;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4BB0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Finds the active entry whose id (f30) matches and selects it through the inlined func_003D5450 body.
struct E_4BB0 { char pad[0x30]; int f30; int f34; char pad2[0x928 - 0x38]; };
extern struct E_4BB0 D_00517610_4BB0[] __asm__("D_00517610");
extern unsigned int D_0044F430_4BB0 __asm__("D_0044F430");
extern struct E_4BB0 *D_0044F420_4BB0 __asm__("D_0044F420");
static inline int select_4BB0(int i) {
    if (D_0044F430_4BB0 == 0) D_0044F430_4BB0 = 0xFFFFFFFF;
    if ((unsigned int)i < 4 && D_00517610_4BB0[i].f34 != 0) {
        D_0044F430_4BB0 = D_00517610_4BB0[i].f30;
        D_0044F420_4BB0 = &D_00517610_4BB0[i];
        return D_0044F430_4BB0;
    }
    if (i != -1) D_0044F430_4BB0 = 0;
    return D_0044F430_4BB0;
}
/**
 * Selects the path player in use whose id (+0x30) equals @p id exactly (the inlined func_003D5450).
 *
 * @param id  the player id (class and group byte)
 * @return the selected id, 0 when no player matches
 */
int func_003D4BB0(int id) {
    int i;
    for (i = 0; i < 4; i++) {
        if (D_00517610_4BB0[i].f34 != 0 && D_00517610_4BB0[i].f30 == id) {
            return select_4BB0(i);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D4C40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct E_4C40 {
    char pad0[0x30];
    unsigned int mask;
    void *hdr;
    char pad38[0x928 - 0x38];
};
extern struct E_4C40 D_00517610_4C40[4] __asm__("D_00517610");

/**
 * Sorts the path players (D_00517610, 0x928 bytes each) that have path data (+0x34) by id (+0x30), lowest first, by
 * swapping whole records. Players without data stay where they are. Called by func_003D2A68 when a player empties.
 */
void func_003D4C40(void) {
    struct E_4C40 swapRec;
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        if (D_00517610_4C40[i].hdr != 0) {
            for (j = i + 1; j < 4; j++) {
                if (D_00517610_4C40[j].hdr != 0 && D_00517610_4C40[j].mask < D_00517610_4C40[i].mask) {
                    swapRec = D_00517610_4C40[i];
                    D_00517610_4C40[i] = D_00517610_4C40[j];
                    D_00517610_4C40[j] = swapRec;
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D4F10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Str_4F10 {
    char pad00[0x3];
    unsigned char f3;
    char pad04[0xD - 0x4];
    signed char fD;
    signed char fE;
    char pad0F[0x15 - 0xF];
    signed char f15;
    char pad16[0x1D - 0x16];
    signed char f1D;
    char pad1E[0x24 - 0x1E];
    int f24;
    char pad28[0x2F - 0x28];
    unsigned char f2F;
    int f30;
    char pad34[0x36 - 0x34];
    short f36;
    char pad38[0x48 - 0x38];
    unsigned int f48;
    unsigned int f4C;
    char pad50[0x54 - 0x50];
    int f54;
};
struct Mgr_4F10 {
    char pad00[0xC];
    unsigned int fC;
    unsigned int f10;
    char pad14[0x58 - 0x14];
    struct Str_4F10 *slot[24];
};
extern struct Mgr_4F10 *D_0044F420_4F10 __asm__("D_0044F420");
extern unsigned char D_0044F434_4F10 __asm__("D_0044F434");
extern unsigned int D_0044F438_4F10 __asm__("D_0044F438");
extern void func_002AE9F8_4F10(int h, int a) __asm__("func_002AE9F8");
extern int func_002AEBD0_4F10(int h) __asm__("func_002AEBD0");
extern int func_002AF428_4F10(int h) __asm__("func_002AF428");
extern int func_003D1E80_4F10(void) __asm__("func_003D1E80");
extern void func_003D3D20_4F10(unsigned int i) __asm__("func_003D3D20");
extern int func_003D4950_4F10(struct Str_4F10 *s) __asm__("func_003D4950");
extern void func_003D5A98_4F10(struct Str_4F10 *s) __asm__("func_003D5A98");
extern void func_003D5CC0_4F10(struct Str_4F10 *s) __asm__("func_003D5CC0");
extern void func_003D5EE8_4F10(struct Str_4F10 *s) __asm__("func_003D5EE8");

/**
 * Services the voices of the selected path player for one pass: runs their ramps and moves due voices to their next
 * node.
 *
 * The pass kind is D_0044F434 ('B' or ' ', set by func_003D5128); its look-ahead is the player's +0x10 for 'B' and +0xC
 * otherwise, and a normal pass first services the pending events (func_003D1E80) and afterwards repeats that until it
 * does nothing. For each voice not paused (+0x3), one with a node and a record runs its three ramps (func_003D5A98,
 * func_003D5CC0, func_003D5EE8 when +0xD, +0x15, +0x1D are set). Voices of the other pass kind (+0x2F) or without a
 * node stop there. A voice with a release time (+0x4C) has its hold released (func_002AE9F8) once tick + look-ahead / 2
 * reaches it. Otherwise, when its monitor is streaming (func_002AEBD0), it advances (func_003D3D20) once tick +
 * position + look-ahead passes its end time (+0x48), or, without one, once the queued amount (func_003D4950) no longer
 * exceeds its position (+0x24); +0xE becomes -1 first when it was 0 and func_002AF428 reports 0.
 */
void func_003D4F10(void) {
    int isB = D_0044F434_4F10 == 'B';
    unsigned int step;
    unsigned int i;
    struct Str_4F10 *pathVoice;
    unsigned int end;
    int ok;
    int queued;
    int skip;

    if (isB) {
        step = D_0044F420_4F10->f10;
    } else {
        step = D_0044F420_4F10->fC;
    }
    if (!isB) {
        func_003D1E80_4F10();
    }
    for (i = 0; i < 24; i++) {
        pathVoice = D_0044F420_4F10->slot[i];
        if (pathVoice == 0 || pathVoice->f3 != 0) {
            continue;
        }
        ok = 0;
        end = D_0044F438_4F10 + pathVoice->f24;
        if (pathVoice->f36 >= 0) {
            ok = pathVoice->f30 != 0;
        }
        if (ok) {
            if (pathVoice->fD >= 0) {
                func_003D5A98_4F10(pathVoice);
            }
            if (pathVoice->f15 >= 0) {
                func_003D5CC0_4F10(pathVoice);
            }
            if (pathVoice->f1D >= 0) {
                func_003D5EE8_4F10(pathVoice);
            }
        }
        if ((pathVoice->f2F != 0) != isB || !ok) {
            continue;
        }
        if (pathVoice->f4C != 0) {
            if (D_0044F438_4F10 + (step >> 1) >= pathVoice->f4C) {
                func_002AE9F8_4F10(pathVoice->f54, 0);
                pathVoice->f4C = 0;
            }
        } else if (func_002AEBD0_4F10(pathVoice->f54) != 0) {
            queued = func_003D4950_4F10(pathVoice);
            if (pathVoice->f48 != 0) {
                end += step;
                skip = end < pathVoice->f48;
            } else {
                skip = pathVoice->f24 < queued;
            }
            if (!skip) {
                if (pathVoice->fE == 0 && func_002AF428_4F10(pathVoice->f54) == 0) {
                    pathVoice->fE = -1;
                }
                func_003D3D20_4F10(i);
            }
        }
    }
    while (!isB && func_003D1E80_4F10() != 0) {
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D5128);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct T_5128 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_5128 __asm__("D_0044F42C");
extern unsigned char D_0044F42D_5128 __asm__("D_0044F42D");
extern unsigned char D_0044F434_5128 __asm__("D_0044F434");
extern unsigned int D_0044F430_5128 __asm__("D_0044F430");
extern int D_0044F438_5128 __asm__("D_0044F438");
extern struct T_5128 *D_0044F420_5128 __asm__("D_0044F420");
extern struct T_5128 D_00517610_5128[] __asm__("D_00517610");
extern int func_002AF370_5128(void) __asm__("func_002AF370");
extern void func_002AEFA8_5128(int) __asm__("func_002AEFA8");
extern void func_003D4B00_5128(void) __asm__("func_003D4B00");
extern void func_003D4F10_5128(void) __asm__("func_003D4F10");

static inline unsigned int select_5128(int i) {
    if ((unsigned int)i < 4) {
        D_0044F420_5128 = &D_00517610_5128[i];
        return D_0044F430_5128 = D_00517610_5128[i].f30;
    } else if (i != -1) {
        D_0044F430_5128 = 0;
    }
    return D_0044F430_5128;
}

void func_003D5128_5128(unsigned char) __asm__("func_003D5128");
void func_003D5128_impl(unsigned char k) __asm__("func_003D5128");
/**
 * Runs one service tick of the path system over every player in use, unless it is paused, down or busy.
 *
 * Does nothing when the global pause (D_0044F42D) is set, the system is not initialised (D_0044F42C) or a selection is
 * active (D_0044F430). Stores the pass kind @p k in D_0044F434, marks the system busy, reads the tick D_0044F438 from
 * func_002AF370 and brackets the work with func_002AEFA8(0) / (1). Each player in use is selected and gets
 * func_003D4B00 (leading voice) and func_003D4F10 (voice service). Clears the selection at the end.
 *
 * @param k  the pass kind: ' ' (func_003D54C8) or 'B' (func_003D54E8)
 */
void func_003D5128_impl(unsigned char k) {
    unsigned char i;
    if (D_0044F42D_5128 != 0) return;
    if (D_0044F42C_5128 == 0) return;
    if (D_0044F430_5128 != 0) return;
    D_0044F434_5128 = k;
    D_0044F430_5128 = 0xFFFFFFFF;
    D_0044F438_5128 = func_002AF370_5128();
    func_002AEFA8_5128(0);
    for (i = 0; i < 4; i++) {
        if (D_00517610_5128[i].f34 == 0) continue;
        if (D_0044F430_5128 == 0) D_0044F430_5128 = 0xFFFFFFFF;
        if (select_5128(i) != 0) {
            func_003D4B00_5128();
            func_003D4F10_5128();
        }
    }
    func_002AEFA8_5128(1);
    D_0044F430_5128 = 0;
    D_0044F420_5128 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5290);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath D_00517610[4];

/**
 * Returns the path voice an id names: in the first player in use whose id shares a class bit (0xF0000000) and a group
 * bit (0x0F000000) with @p pathId, the voice (+0x58) for the lowest set bit of the low 24 bits.
 *
 * Quirk: once a player matches, an empty slot for that bit returns 0 without trying other players.
 *
 * @param pathId  the path id
 * @return the voice (sPathVoice), or 0
 */
int func_003D5290(unsigned pathId) {
    unsigned i, voiceBit;
    unsigned hi = pathId & 0xF0000000;
    unsigned mid = pathId & 0x0F000000;
    unsigned voiceBits = pathId & 0xFFFFFF;
    for (i = 0; i < 4; i++) {
        unsigned mk = D_00517610[i].f30;
        if (D_00517610[i].f34 != 0 &&
            (mk & hi) && (mk & mid)) {
            for (voiceBit = 0; voiceBit < 0x18; voiceBit++) {
                if ((voiceBits >> voiceBit) & 1)
                    return (int)D_00517610[i].f58[voiceBit];
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5330);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// D_00517610: an array of 0x928-byte records (the unit's other views read f30/f34 too).
struct E_5330 { char pad00[0x30]; unsigned int f30; int f34; char pad38[0x20]; int w[24]; char padB8[0x928 - 0xB8]; };
extern struct E_5330 D_00517610_5330[] __asm__("D_00517610");

/**
 * Counts the voices in use (+0x58, 24 slots) of the path players in use whose group bit (0x1000000 << player index) is
 * set in @p mask.
 *
 * @param mask  group bits, one per player slot
 * @return the number of voices
 */
int func_003D5330(unsigned int mask) {
    int n = 0;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        if (mask & (0x1000000 << i)) {
            if (D_00517610_5330[i].f34 != 0) {
                for (j = 0; j < 24; j++) {
                    if (D_00517610_5330[i].w[j] != 0) {
                        n++;
                    }
                }
            }
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D53B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int PATH_status_53B8(int, int *) __asm__("PATH_status");
extern int func_003D3030_53B8(int) __asm__("func_003D3030");

/**
 * Returns the id of a playing path voice for a group and voice slot, trying class 2 before class 1; 0 when neither has
 * a voice with a current node.
 *
 * The id is (0x10000000 << class) | (0x1000000 << @p groupIndex) | (1 << @p channelIndex); func_003D3030 must find a
 * voice and PATH_status must report a node.
 *
 * @param groupIndex  the player group (0..3)
 * @param channelIndex  the voice slot (0..23)
 * @return the id, or 0
 */
int func_003D53B8(int groupIndex, int channelIndex) {
    int info[12];
    int i;
    int mask;

    for (i = 1; i >= 0; i--) {
        mask = ((0x10000000 << i) | (0x01000000 << groupIndex)) + (1 << channelIndex);
        if (func_003D3030_53B8(mask) >= 0) {
            PATH_status_53B8(mask, info);
            if (info[0] >= 0) return mask;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5450);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Selects entry i (0..3) of D_00517610 as current; any other i except -1 clears the current id.
struct E_5450 { char pad[0x30]; int f30; int f34; char pad2[0x928 - 0x38]; };
extern struct E_5450 D_00517610_5450[] __asm__("D_00517610");
extern unsigned int D_0044F430_5450 __asm__("D_0044F430");
extern struct E_5450 *D_0044F420_5450 __asm__("D_0044F420");
/**
 * Selects path player @p i (D_0044F420 and its id in D_0044F430), or with -1 just marks the system busy.
 *
 * A clear D_0044F430 becomes -1 first. A player index below 4 that is in use (+0x34) is selected and its id returned;
 * -1 leaves the busy mark; any other value clears D_0044F430. Every path entry point calls it with -1 first and fails
 * with -18 when it returns 0 or the system was already busy.
 *
 * @param i  the player (0..3), or -1
 * @return the selected id, -1 for busy, 0 when the player is not in use
 */
int func_003D5450(int i) {
    if (D_0044F430_5450 == 0) D_0044F430_5450 = 0xFFFFFFFF;
    if ((unsigned int)i < 4 && D_00517610_5450[i].f34 != 0) {
        D_0044F430_5450 = D_00517610_5450[i].f30;
        D_0044F420_5450 = &D_00517610_5450[i];
        return D_0044F430_5450;
    }
    if (i != -1) D_0044F430_5450 = 0;
    return D_0044F430_5450;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D54C8);
#ifdef SKIP_ASM
extern void func_003D5128(int);

/**
 * Runs a normal path service tick: func_003D5128(' ').
 */
void func_003D54C8(void) {
    func_003D5128(0x20);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2B4578", func_003D54E8);
#ifdef SKIP_ASM
extern void func_003D5128(int);

/**
 * Runs a 'B' path service tick: func_003D5128('B'), which services only the voices flagged for it (+0x2F).
 */
void func_003D54E8(void) {
    func_003D5128(66);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5508);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Same walk as func_003D1038: clamps val to 0..100 and applies it to every voice
// in mask through func_003D62C8.
struct G_5508 { char pad[0x58]; void *list[24]; };
struct T_5508 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_5508 __asm__("D_0044F42C");
extern signed char D_0044F42E_5508 __asm__("D_0044F42E");
extern int D_0044F430_5508 __asm__("D_0044F430");
extern struct G_5508 *D_0044F420_5508 __asm__("D_0044F420");
extern struct T_5508 D_00517610_5508[] __asm__("D_00517610");
extern int func_003D5450_5508(int) __asm__("func_003D5450");
extern void func_003D62C8_5508(void *, int) __asm__("func_003D62C8");

static inline unsigned int has_5508(unsigned int a, unsigned int b) { return a & b; }

/**
 * Sets a level (0..100) on the voices a path id mask selects, through func_003D62C8.
 *
 * Does nothing unless the path system is up and idle. @p val is clamped to 0..100; a mask of -1 also stores it as the
 * global level D_0044F42E. For each player in use sharing class and group bits with @p mask, every voice whose bit is
 * set gets func_003D62C8(voice, val). Clears the selection at the end. The music monitor calls it for 0x11000002.
 *
 * @param mask  the path id mask
 * @param val  the level, clamped to 0..100
 */
void func_003D5508(unsigned int mask, signed char val) {
    int i, j;
    if (D_0044F42C_5508 == 0) return;
    if (D_0044F430_5508 != 0) return;
    if (func_003D5450_5508(-1) == 0) return;
    if (val > 100) val = 100;
    if (val < 0) val = 0;
    if (mask == 0xFFFFFFFF) D_0044F42E_5508 = val;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_5508[i].f34 == 0) continue;
        playerMask = D_00517610_5508[i].f30;
        if (has_5508(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_5508(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_5508(i) == 0) continue;
        for (j = 0; j < 24; j++) {
            void *pathVoice = D_0044F420_5508->list[j];
            if (pathVoice != 0 && (((mask >> j) ^ 1) & 1) == 0) {
                func_003D62C8_5508(pathVoice, val);
            }
        }
    }
    D_0044F430_5508 = 0;
    D_0044F420_5508 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5698);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct G_5698 { char pad[0x18]; int f18; char pad2[0x58 - 0x1C]; void *list[24]; };
struct T_5698 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_5698 __asm__("D_0044F42C");
extern int D_0044F430_5698 __asm__("D_0044F430");
extern struct G_5698 *D_0044F420_5698 __asm__("D_0044F420");
extern struct T_5698 D_00517610_5698[] __asm__("D_00517610");
extern int func_003D5450_5698(int) __asm__("func_003D5450");
extern void func_003D6318_5698(void *, int, int, int) __asm__("func_003D6318");

static inline unsigned int has_5698(unsigned int a, unsigned int b) { return a & b; }

/**
 * Fades the voices a path id mask selects to a volume over a time (func_003D6318 on each).
 *
 * Does nothing unless the path system is up and idle. For each player in use sharing class and group bits with
 * @p mask, every voice whose bit is set gets func_003D6318(voice, @p targetVol, @p fadeMs, player +0x18) (the fourth value is
 * ignored there). Clears the selection at the end.
 *
 * @param mask  the path id mask
 * @param targetVol  the target volume (0..127)
 * @param fadeMs  the fade time in ms
 */
void func_003D5698(unsigned int mask, int targetVol, int fadeMs) {
    int i, j;
    if (D_0044F42C_5698 == 0) return;
    if (D_0044F430_5698 != 0) return;
    if (func_003D5450_5698(-1) == 0) return;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_5698[i].f34 == 0) continue;
        playerMask = D_00517610_5698[i].f30;
        if (has_5698(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_5698(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_5698(i) == 0) continue;
        for (j = 0; j < 24; j++) {
            void *pathVoice = D_0044F420_5698->list[j];
            if (pathVoice != 0 && (((mask >> j) ^ 1) & 1) == 0) {
                func_003D6318_5698(pathVoice, targetVol, fadeMs, D_0044F420_5698->f18);
            }
        }
    }
    D_0044F430_5698 = 0;
    D_0044F420_5698 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5800);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct G_5800 { char pad[0x18]; int f18; char pad2[0x58 - 0x1C]; void *list[24]; };
struct T_5800 { char pad[0x30]; unsigned int f30; int f34; char pad2[0x928 - 0x38]; };
extern unsigned char D_0044F42C_5800 __asm__("D_0044F42C");
extern int D_0044F430_5800 __asm__("D_0044F430");
extern struct G_5800 *D_0044F420_5800 __asm__("D_0044F420");
extern struct T_5800 D_00517610_5800[] __asm__("D_00517610");
extern int func_003D5450_5800(int) __asm__("func_003D5450");
extern void func_003D6360_5800(void *, int, int, int) __asm__("func_003D6360");

static inline unsigned int has_5800(unsigned int a, unsigned int b) { return a & b; }

/**
 * Sets the effect-send target of the voices a path id mask selects (func_003D6360 on each).
 *
 * Does nothing unless the path system is up and idle. Each selected voice gets func_003D6360(voice, @p sendTarget, @p b, player
 * +0x18): the send target +0x16 = @p sendTarget and, when the player's +0x18 is not negative, +0x15 = that value;
 * @p b is not used. Clears the selection at the end.
 *
 * @param mask  the path id mask
 * @param sendTarget  the send target
 * @param b  unused by the callee
 */
void func_003D5800(unsigned int mask, int sendTarget, int b) {
    int i, j;
    if (D_0044F42C_5800 == 0) return;
    if (D_0044F430_5800 != 0) return;
    if (func_003D5450_5800(-1) == 0) return;
    for (i = 0; i < 4; i++) {
        unsigned int playerMask;
        if (D_00517610_5800[i].f34 == 0) continue;
        playerMask = D_00517610_5800[i].f30;
        if (has_5800(playerMask & 0xF0000000, mask) == 0) continue;
        if (has_5800(playerMask & 0x0F000000, mask) == 0) continue;
        if (func_003D5450_5800(i) == 0) continue;
        for (j = 0; j < 24; j++) {
            void *pathVoice = D_0044F420_5800->list[j];
            if (pathVoice != 0 && (((mask >> j) ^ 1) & 1) == 0) {
                func_003D6360_5800(pathVoice, sendTarget, b, D_0044F420_5800->f18);
            }
        }
    }
    D_0044F430_5800 = 0;
    D_0044F420_5800 = 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5968);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_002AF428_5968(int) __asm__("func_002AF428");
extern void func_002AF6C0_5968(int, int) __asm__("func_002AF6C0");
extern void func_002AF838_5968(int) __asm__("func_002AF838");
extern int D_0044F420_5968[] __asm__("D_0044F420");

/**
 * Settles the volume fades of the selected path player's voices in a mask, stopping voices that faded to silence.
 *
 * For each voice whose bit is set and that has a node (+0x36 >= 0): when it has a fade target (+0xE >= 0) scaled by its
 * level (+0x4, percent) equal to the monitor's current volume (func_002AF428), a target of 0 stops the voice
 * (func_002AF838, node -1) and any other target applies the node volume (+0x34 scaled by +0x4) with func_002AF6C0; a
 * voice without a target gets the node volume directly (when not negative). The target is then cleared (-1). A fade
 * still in progress is left alone.
 *
 * @param mask  the voice bits
 */
void func_003D5968(unsigned int mask) {
    int i;
    for (i = 0; i < 0x18; i++) {
        char *pathVoice = *(char **)((char *)D_0044F420_5968[0] + (i << 2) + 0x58);
        if (pathVoice != 0 && !(((mask >> i) ^ 1) & 1) && *(short *)(pathVoice + 0x36) >= 0) {
            int nodeVol = *(signed char *)(pathVoice + 0x34) * *(signed char *)(pathVoice + 4) / 100;
            signed char fadeTarget = *(signed char *)(pathVoice + 0xE);
            if (fadeTarget >= 0) {
                int scaledTarget = fadeTarget * *(signed char *)(pathVoice + 4) / 100;
                if (scaledTarget == func_002AF428_5968(*(int *)(pathVoice + 0x54))) {
                    if (*(signed char *)(pathVoice + 0xE) == 0) {
                        func_002AF838_5968(*(int *)(pathVoice + 0x54));
                        *(short *)(pathVoice + 0x36) = -1;
                    } else {
                        goto b8;
                    }
                    goto b10;
                }
            } else {
b8:
                if (nodeVol >= 0) {
                    func_002AF6C0_5968(*(int *)(pathVoice + 0x54), nodeVol);
                }
b10:
                *(signed char *)(pathVoice + 0xE) = -1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5A98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

extern unsigned int D_0044F438;         /* tick */

extern int func_002AF6C0_5A98(int, int) __asm__("func_002AF6C0");

void func_003D5A98_impl(struct sPathVoice *v) __asm__("func_003D5A98");
/**
 * Steps a path voice's volume ramp: sets the volume along the curve, or the node volume once the ramp has run out.
 *
 * The ramp goes from +0xC to +0xE starting at tick +0x8; its length and curve come from the selected player's first
 * ramp setting (+0x14 duration, +0x18 mode). With t the elapsed fraction: when t >= 1 the volume becomes the node
 * volume +0x34 (func_002AF6C0 on the monitor +0x54) and the ramp and +0x34 are cleared to -1. Otherwise the curve value
 * (mode 1: 1 - t, 2: 1 - t^2, 3: 1 / t / 25; t is mirrored for a rising ramp) gives min(start, end) + |end - start| *
 * value, applied only when the value is within 0..1.
 *
 * @param pathVoice  the path voice
 */
void func_003D5A98_impl(struct sPathVoice *pathVoice) {
    struct {
        unsigned mode : 7;
        int neg : 1;
        unsigned mid : 8;
        unsigned dur : 16;
    } rampSetting;
    float curve = -1.0f;
    float diff;
    float elapsed;
    float lowLevel;
    int pos;
    int none;
    unsigned sel;
    diff = (float)pathVoice->f0E - (float)pathVoice->f0C;
    rampSetting.dur = D_0044F420->f14[0].dur;
    rampSetting.mode = D_0044F420->f14[0].mode;
    pos = 1;
    if (diff < 0.0f) {
        pos = 0;
    }
    rampSetting.neg = pos;
    elapsed = (float)(unsigned)(D_0044F438 - pathVoice->f08) / (float)rampSetting.dur;
    if (elapsed >= 1.0f) {
        func_002AF6C0_5A98(pathVoice->f54, pathVoice->f34);
        none = -1;
        pathVoice->f0C = none;
        sel = none;
        pathVoice->f0D = pathVoice->f0E = sel;
        pathVoice->f08 = 0;
        pathVoice->f34 = sel;
        return;
    }
    if (rampSetting.neg) {
        elapsed = 1.0f - elapsed;
    }
    sel = rampSetting.mode;
    switch (sel) {
    case 1:
        curve = 1.0f - elapsed;
        break;
    case 2:
        curve = 1.0f - elapsed * elapsed;
        break;
    case 3:
        curve = 1.0f / elapsed / 25.0f;
        break;
    }
    lowLevel = pathVoice->f0C;
    if (pathVoice->f0E < pathVoice->f0C) {
        lowLevel = pathVoice->f0E;
    }
    if (diff < 0.0f) {
        diff = -diff;
    }
    if (curve >= 0.0f && curve <= 1.0f) {
        curve = lowLevel + diff * curve;
        func_002AF6C0_5A98(pathVoice->f54, (int)curve);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5CC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

extern unsigned int D_0044F438;         /* tick */

extern int func_002AF4E0(int, int, int);

void func_003D5CC0_impl(struct sPathVoice *v) __asm__("func_003D5CC0");
/**
 * Steps a path voice's effect-send ramp: the same curve as func_003D5A98 on +0x14 -> +0x16 from tick +0x10, with the
 * player's second ramp setting (+0x1C, +0x20), applied with func_002AF4E0 (bus 0).
 *
 * When the ramp has run out the target +0x16 is applied and +0x14..+0x16 are cleared to -1, +0x10 to 0.
 *
 * @param pathVoice  the path voice
 */
void func_003D5CC0_impl(struct sPathVoice *pathVoice) {
    struct {
        unsigned mode : 7;
        int neg : 1;
        unsigned mid : 8;
        unsigned dur : 16;
    } rampSetting;
    float curve = -1.0f;
    float diff;
    float elapsed;
    float lowLevel;
    int pos;
    int none;
    unsigned sel;
    diff = (float)pathVoice->f16 - (float)pathVoice->f14;
    rampSetting.dur = D_0044F420->f14[1].dur;
    rampSetting.mode = D_0044F420->f14[1].mode;
    pos = 1;
    if (diff < 0.0f) {
        pos = 0;
    }
    rampSetting.neg = pos;
    elapsed = (float)(unsigned)(D_0044F438 - pathVoice->f10) / (float)rampSetting.dur;
    if (elapsed >= 1.0f) {
        func_002AF4E0(pathVoice->f54, 0, pathVoice->f16);
        none = -1;
        pathVoice->f14 = none;
        pathVoice->f15 = pathVoice->f16 = none;
        pathVoice->f10 = 0;
        return;
    }
    if (rampSetting.neg) {
        elapsed = 1.0f - elapsed;
    }
    sel = rampSetting.mode;
    switch (sel) {
    case 1:
        curve = 1.0f - elapsed;
        break;
    case 2:
        curve = 1.0f - elapsed * elapsed;
        break;
    case 3:
        curve = 1.0f / elapsed / 25.0f;
        break;
    }
    lowLevel = pathVoice->f14;
    if (pathVoice->f16 < pathVoice->f14) {
        lowLevel = pathVoice->f16;
    }
    if (diff < 0.0f) {
        diff = -diff;
    }
    if (curve >= 0.0f && curve <= 1.0f) {
        curve = lowLevel + diff * curve;
        func_002AF4E0(pathVoice->f54, 0, (int)curve);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D5EE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* PATHI players: D_00517610[4] (0x928 each), D_0044F420 points at the one selected by func_003D5450. Layout
   from every use in this unit; field names are their offsets. */
extern struct sPath *D_0044F420;

extern unsigned int D_0044F438;         /* tick */

extern int func_002AF580(int, int);

void func_003D5EE8_impl(struct sPathVoice *v) __asm__("func_003D5EE8");
/**
 * Steps a path voice's dry-level ramp: the same curve as func_003D5A98 on +0x1C -> +0x1E from tick +0x18, with the
 * player's third ramp setting (+0x24, +0x28), applied with func_002AF580.
 *
 * When the ramp has run out the target +0x1E is applied and +0x1C..+0x1E are cleared to -1, +0x18 to 0.
 *
 * @param pathVoice  the path voice
 */
void func_003D5EE8_impl(struct sPathVoice *pathVoice) {
    struct {
        unsigned mode : 7;
        int neg : 1;
        unsigned mid : 8;
        unsigned dur : 16;
    } rampSetting;
    float curve = -1.0f;
    float diff;
    float elapsed;
    float lowLevel;
    int pos;
    int none;
    unsigned sel;
    diff = (float)pathVoice->f1E - (float)pathVoice->f1C;
    rampSetting.dur = D_0044F420->f14[2].dur;
    rampSetting.mode = D_0044F420->f14[2].mode;
    pos = 1;
    if (diff < 0.0f) {
        pos = 0;
    }
    rampSetting.neg = pos;
    elapsed = (float)(unsigned)(D_0044F438 - pathVoice->f18) / (float)rampSetting.dur;
    if (elapsed >= 1.0f) {
        func_002AF580(pathVoice->f54, pathVoice->f1E);
        none = -1;
        pathVoice->f1C = none;
        pathVoice->f1D = pathVoice->f1E = none;
        pathVoice->f18 = 0;
        return;
    }
    if (rampSetting.neg) {
        elapsed = 1.0f - elapsed;
    }
    sel = rampSetting.mode;
    switch (sel) {
    case 1:
        curve = 1.0f - elapsed;
        break;
    case 2:
        curve = 1.0f - elapsed * elapsed;
        break;
    case 3:
        curve = 1.0f / elapsed / 25.0f;
        break;
    }
    lowLevel = pathVoice->f1C;
    if (pathVoice->f1E < pathVoice->f1C) {
        lowLevel = pathVoice->f1E;
    }
    if (diff < 0.0f) {
        diff = -diff;
    }
    if (curve >= 0.0f && curve <= 1.0f) {
        curve = lowLevel + diff * curve;
        func_002AF580(pathVoice->f54, (int)curve);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6108);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_6108 { char pad[0x30]; int m_a; int m_b; char pad2[0x928 - 0x38]; };
extern int f5450_6108(int) __asm__("func_003D5450");
extern int D_f420_6108[] __asm__("D_0044F420");
extern unsigned char D_f42C_6108[] __asm__("D_0044F42C");
extern int D_f430_6108[] __asm__("D_0044F430");
extern struct P_6108 D_7610_6108[4] __asm__("D_00517610");

/**
 * Sets the volume-ramp duration (+0x14) of every path player in use whose group bits overlap @p mask.
 *
 * Does nothing unless the path system is up and idle; clears the selection at the end.
 *
 * @param mask  the path id mask (only the group bits count)
 * @param value  the ramp duration in ticks
 */
void func_003D6108(int mask, int value) {
    int i;
    if (D_f42C_6108[0] != 0 && D_f430_6108[0] == 0 && f5450_6108(-1) != 0) {
        mask &= 0x0F000000;
        for (i = 0; i < 4; i++) {
            if (D_7610_6108[i].m_b != 0 && (D_7610_6108[i].m_a & mask) && f5450_6108(i & 0xFF) != 0) {
                *(int *)(D_f420_6108[0] + 0x14) = value;
            }
        }
        D_f430_6108[0] = 0;
        D_f420_6108[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D61E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_61E8 { char pad[0x30]; int m_a; int m_b; char pad2[0x928 - 0x38]; };
extern int f5450_61E8(int) __asm__("func_003D5450");
extern int D_f420_61E8[] __asm__("D_0044F420");
extern unsigned char D_f42C_61E8[] __asm__("D_0044F42C");
extern int D_f430_61E8[] __asm__("D_0044F430");
extern struct P_61E8 D_7610_61E8[4] __asm__("D_00517610");

/**
 * Sets the volume-ramp curve mode (+0x18) of every path player in use whose group bits overlap @p mask.
 *
 * Does nothing unless the path system is up and idle; clears the selection at the end.
 *
 * @param mask  the path id mask (only the group bits count)
 * @param value  the curve mode (1 linear, 2 quadratic, 3 inverse)
 */
void func_003D61E8(int mask, int value) {
    int i;
    if (D_f42C_61E8[0] != 0 && D_f430_61E8[0] == 0 && f5450_61E8(-1) != 0) {
        mask &= 0x0F000000;
        for (i = 0; i < 4; i++) {
            if (D_7610_61E8[i].m_b != 0 && (D_7610_61E8[i].m_a & mask) && f5450_61E8(i & 0xFF) != 0) {
                *(int *)(D_f420_61E8[0] + 0x18) = value;
            }
        }
        D_f430_61E8[0] = 0;
        D_f420_61E8[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D62C8);
#ifdef SKIP_ASM
extern void func_002AF6C0(int, int);

/**
 * Sets a path voice's level (+0x4, percent) and, when no fade target is pending (+0xE < 0), applies node volume +0x34
 * times the level / 100 with func_002AF6C0.
 *
 * @param channel  the path voice
 * @param volume  the level, 0..100
 */
void func_003D62C8(void *channel, signed char volume) {
    (*(signed char *)((char*)(channel) + (4))) = volume;
    if ((*(signed char *)((char*)(channel) + (0xE))) < 0) {
        func_002AF6C0((*(int *)((char*)(channel) + (0x54))), (int) ((*(signed char *)((char*)(channel) + (0x34))) * volume) / 100);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6318);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int func_003D6318_cb(int, int, int, char) __asm__("func_002AF608");

/**
 * Starts a volume fade on a path voice: target +0xE = @p level, +0xD = 0, and func_002AF608 fades the monitor to
 * @p level times the voice level (+0x4) / 100 over @p w ms.
 *
 * @param level  the target volume
 * @param w  the fade time in ms
 */
void func_003D6318(char *self, int level, int w) {
    int x;
    self[0xE] = level;
    x = (level * self[4]) / 100;
    self[0xD] = 0;
    func_003D6318_cb(*(int *)(self + 0x54), x, w, 1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6360);
#ifdef SKIP_ASM
/**
 * Sets a path voice's effect-send target (+0x16) to @p sendTarget and, when @p rampFlag is not negative, +0x15 to @p rampFlag.
 *
 * @param channel  the path voice
 * @param sendTarget  the send target
 * @param arg2  unused
 * @param rampFlag  the value for +0x15, ignored when negative (func_003D5800 passes the player's +0x18)
 */
void func_003D6360(void *channel, int sendTarget, int arg2, int rampFlag) {
    *(signed char *)((char *)channel + 0x16) = sendTarget;
    if (rampFlag >= 0) {
        *(signed char *)((char *)channel + 0x15) = rampFlag;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6378);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the dead `q = a / 0x7F;` (q is never used), which keeps retail's divide-by-zero trap check (`li 0x7F; beqzl; break 7`) after the divide itself is deleted; plain form 38.71%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern int func_002AF428(int);

/**
 * Starts a volume ramp on a path voice from its current volume to @p targetVol.
 *
 * Stamps the start tick (+0x8 = D_0044F438), stores @p rampFlag at +0xD, the current volume (func_002AF428) at +0xC and
 * @p targetVol as the target (+0xE) and node volume (+0x34). func_003D5A98 then steps it.
 *
 * @param targetVol  the target volume
 * @param rampFlag  the ramp flag kept at +0xD
 */
void func_003D6378(char *self, int targetVol, int rampFlag) {
    int curVol;
    int q;
    *(int *)(self + 8) = D_0044F438;
    self[0xD] = rampFlag;
    curVol = func_002AF428(*(int *)(self + 0x54));
    self[0xC] = (int)(float)curVol;
    self[0xE] = targetVol;
    self[0x34] = (int)(float)targetVol;
    q = targetVol / 0x7F;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D63F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F438_63F8[] __asm__("D_0044F438");
extern signed char f_63F8_cb(int) __asm__("func_002AF4B0");

/**
 * Starts an effect-send ramp on a path voice from its current send level (func_002AF4B0) to @p targetLevel.
 *
 * Stamps the start tick (+0x10), stores @p rampFlag at +0x15, the current level at +0x14 and the target at +0x16;
 * func_003D5CC0 steps it.
 *
 * @param targetLevel  the target send level
 * @param rampFlag  the ramp flag kept at +0x15
 */
void func_003D63F8(char *self, int targetLevel, int rampFlag) {
    signed char curLevel;
    *(int *)(self + 0x10) = D_0044F438_63F8[0];
    self[0x15] = rampFlag;
    curLevel = f_63F8_cb(*(int *)(self + 0x54));
    self[0x16] = targetLevel;
    self[0x14] = curLevel;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2B4578", func_003D6448);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_0044F438_6448[] __asm__("D_0044F438");
extern signed char f_6448_cb(int) __asm__("func_002AF480");

/**
 * Starts a dry-level ramp on a path voice from its current dry level (func_002AF480) to @p targetLevel.
 *
 * Stamps the start tick (+0x18), stores @p rampFlag at +0x1D, the current level at +0x1C and the target at +0x1E;
 * func_003D5EE8 steps it.
 *
 * @param targetLevel  the target dry level
 * @param rampFlag  the ramp flag kept at +0x1D
 */
void func_003D6448(char *self, int targetLevel, int rampFlag) {
    signed char curLevel;
    *(int *)(self + 0x18) = D_0044F438_6448[0];
    self[0x1D] = rampFlag;
    curLevel = f_6448_cb(*(int *)(self + 0x54));
    self[0x1E] = targetLevel;
    self[0x1C] = curLevel;
}
#endif
