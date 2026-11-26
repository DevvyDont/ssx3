#ifndef EALIB_SHAPE_H
#define EALIB_SHAPE_H

// EA's realgraph shape file (.ssh): a header, a table of named entries and the shapes (images) they point to.
// The game loads them whole (front-end backgrounds, package textures, the load screens) and walks the table by hand or
// with SHAPE_locatez (retail src/seg/seg_1DCD10.cpp; On Tour: realgraph/1.00.06/source/shape/cmn/locatshp.cpp, which
// also has INLINESHAPE_count / INLINESHAPE_pointer over the same header).
//
// Names: SSX On Tour's DWARF (struct SHAPEFILE 0x18, struct SHAPETABLEENTRY 0x8; [O]). Layout placed by retail use
// ([U]): the count at 0x8 and entry i's offset at 0x14 + 8 * i (fe/festatecharequipdetail's package walk, main/loadscreens'
// background file, fe/festaterewards' file). tbl is a variable-length table (EA declares it [1]).
//
// FLAT layouts, like main/game.h. Evidence tags: [O] On Tour's DWARF; [U] how retail code uses the field.

#include "layout.h"

// C units (ealib/seg_2B4578.c, ealib/seg_2DCAE8.c) name these types without the struct keyword (views group D).
#ifndef __cplusplus
typedef struct SHAPE SHAPE;
typedef struct SHAPEIMAGE SHAPEIMAGE;
typedef struct SHAPERGBA SHAPERGBA;
typedef struct SHAPETABLEENTRY SHAPETABLEENTRY;
typedef struct SHAPEFILE SHAPEFILE;
#endif

// SHAPE: the 0x10-byte header of one shape (image) in a shape file (views group D: SH_3E40, SHAPE_3A50, Img_4010,
// sTexHdr_27C8 in ealib/seg_2AEDC0). [O] On Tour struct SHAPE (0x10, realgraph shape/cmn) and the paired functions
// SHAPE_getpixeloffset / SHAPE_getpixeladr(const SHAPE* shp, int x, int y, int mipmap) (shpgetoffset.cpp),
// SHAPE_depth(const SHAPE*) (shpdepth.cpp), SHAPE_createat(SHAPE* shp, ...) (creates.cpp), SHAPE_locatez (returns
// SHAPE*); [U] retail reads type (the low byte), width / height at 0x4 / 0x6, referenced (0x1000 = bit 12) and swizzled
// (0x2000 = bit 13) of the word at 0xC, mipmaps (bits 28..31). Bit fields are allocated from bit 0 (little-endian g++).
// A referenced shape keeps the pixel data's offset in the word after the header (*(int*)(shp + 1); the views' 0x10
// member), which is not part of SHAPE.
struct SHAPE {
    unsigned int type : 8;          // 0x0 bits 0..7 [O]; [U] the pixel format (SHAPE_depth switches on it)
    int next : 24;                  // 0x0 bits 8..31 [O] the offset of the next block (palette, comment, ...)
    short width;                    // 0x4 [O]; [U]
    short height;                   // 0x6 [O]; [U]
    short centerx;                  // 0x8 [O]
    short centery;                  // 0xA [O]
    int shapex : 12;                // 0xC bits 0..11 [O]
    unsigned int referenced : 1;    // 0xC bit 12 [O]; [U] 0x1000: the data is elsewhere (offset after the header)
    unsigned int swizzled : 1;      // 0xC bit 13 [O]; [U] 0x2000: SHAPE_getpixeloffset swizzles the address
    unsigned int transposed : 1;    // 0xC bit 14 [O]
    unsigned int reserved : 1;      // 0xC bit 15 [O]
    int shapey : 12;                // 0xC bits 16..27 [O]
    unsigned int mipmaps : 4;       // 0xC bits 28..31 [O]; [U] SHAPE_createat stores the level count
};

// A shape's pixel format (SHAPE::type, low 7 bits; bit 7 marks packed data; names ours). [R] SHAPE_depth's table
// shapetypetobpp gives 4, 8, 15, 24, 32 and 16 bits per pixel for types 1 .. 6; [U] cPS2GraphicsMan_AddTexture and
// SetNewTextureData give types 1 and 2 a palette (the next shape) and the PSMT4 / PSMT8 texel formats, type 3 the
// 16-bit and type 5 the 32-bit one.
enum eSHAPEType {
    kSHAPE_PAL4 = 1, kSHAPE_PAL8 = 2, kSHAPE_RGB15 = 3, kSHAPE_RGB24 = 4, kSHAPE_RGB32 = 5, kSHAPE_RGB16 = 6
};

// The first two bytes of RefPack-packed data (names ours). [U] unpackz decodes data whose second byte is 0xFB and whose
// first is 0x10 or 0x90 (REF_decode); cPS2GraphicsMan_SetNewTextureData unpacks shape data that starts 0x10 0xFB.
enum { kREFPACK_TYPE = 0x10, kREFPACK_MAGIC = 0xFB };

CHECK_OFFSET(SHAPE, width, 0x4);
CHECK_OFFSET(SHAPE, height, 0x6);
CHECK_OFFSET(SHAPE, centerx, 0x8);
CHECK_OFFSET(SHAPE, centery, 0xA);
CHECK_SIZE(SHAPE, 0x10);

// SHAPEIMAGE (0x14, name ours): a SHAPE header the way retail's realgraph code reads it, followed by the word a
// referenced shape keeps after the header. Same layout as SHAPE up to 0xF, but retail tests the word at 0xC whole
// (masks 0x1000 / 0x2000, >> 28; SHAPE's bitfields compile to shift + mask instead), so the readers that do take this
// type. [U] SHAPE_getpixeloffset (flags & 0x2000), SHAPE_getpixeladr (flags & 0x1000 / 0x2000, dataoffset),
// SHAPE_size (flags & 0x1000, >> 28, >> 13), SHAPE_unpack (flags >> 28) in ealib/seg_2B4578; SHAPE_createat in
// ealib/seg_2AEDC0 (flags & 0x1000, dataoffset); cFont_downloadTexture (render/font: flags & 0x1000, offset 0x10).
struct SHAPEIMAGE {
    unsigned int type : 8;          // 0x0 bits 0..7: SHAPE::type
    int next : 24;                  // 0x0 bits 8..31: SHAPE::next
    short width;                    // 0x4 SHAPE::width
    short height;                   // 0x6 SHAPE::height
    short centerx;                  // 0x8 SHAPE::centerx
    short centery;                  // 0xA SHAPE::centery
    int flags;                      // 0xC SHAPE's shapex .. mipmaps word: 0x1000 referenced, 0x2000 swizzled,
                                    //     bits 28..31 the mipmap count
    int dataoffset;                 // 0x10 a referenced shape's pixel data offset from the header (flag 0x1000);
                                    //      otherwise its first pixels
};

CHECK_OFFSET(SHAPEIMAGE, width, 0x4);
CHECK_OFFSET(SHAPEIMAGE, height, 0x6);
CHECK_OFFSET(SHAPEIMAGE, centerx, 0x8);
CHECK_OFFSET(SHAPEIMAGE, centery, 0xA);
CHECK_OFFSET(SHAPEIMAGE, flags, 0xC);
CHECK_OFFSET(SHAPEIMAGE, dataoffset, 0x10);
CHECK_SIZE(SHAPEIMAGE, 0x14);

// SHAPERGBA (0x4, name ours): one entry of a 32-bit shape palette (palette block type 0x21). [U] SHAPE_createat
// (ealib/seg_2AEDC0) starts a new 32-bit palette as a grey ramp: entry i gets r = g = b = i, a = 0xFF.
struct SHAPERGBA {
    unsigned char r;                // 0x0 [U]
    unsigned char g;                // 0x1 [U]
    unsigned char b;                // 0x2 [U]
    unsigned char a;                // 0x3 [U] 0xFF
};

CHECK_OFFSET(SHAPERGBA, r, 0x0);
CHECK_OFFSET(SHAPERGBA, g, 0x1);
CHECK_OFFSET(SHAPERGBA, b, 0x2);
CHECK_OFFSET(SHAPERGBA, a, 0x3);
CHECK_SIZE(SHAPERGBA, 0x4);

struct SHAPETABLEENTRY {
    char id[4];                     // 0x0 [O] the entry's 4-character name (SHAPE_locatez looks entries up by name)
    int offset;                     // 0x4 [O]; [U] the shape's offset from the start of the file (read at 0x14 + 8 * i)
};

struct SHAPEFILE {
    char type[4];                   // 0x00 [O] the file tag
    int len;                        // 0x04 [O] the file length
    int num;                        // 0x08 [O]; [U] the entry count every walk loops to
    char creator[4];                // 0x0C [O] the creating tool's tag
    SHAPETABLEENTRY tbl[1];         // 0x10 [O] num entries; [U] tbl[i].offset at 0x14 + 8 * i
};

CHECK_OFFSET(SHAPETABLEENTRY, id, 0x0);
CHECK_OFFSET(SHAPETABLEENTRY, offset, 0x4);
CHECK_SIZE(SHAPETABLEENTRY, 0x8);
CHECK_OFFSET(SHAPEFILE, type, 0x00);
CHECK_OFFSET(SHAPEFILE, len, 0x04);
CHECK_OFFSET(SHAPEFILE, num, 0x08);
CHECK_OFFSET(SHAPEFILE, creator, 0x0C);
CHECK_OFFSET(SHAPEFILE, tbl, 0x10);
CHECK_SIZE(SHAPEFILE, 0x18);

#endif /* EALIB_SHAPE_H */
