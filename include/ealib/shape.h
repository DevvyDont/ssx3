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

CHECK_OFFSET(SHAPE, width, 0x4);
CHECK_OFFSET(SHAPE, height, 0x6);
CHECK_OFFSET(SHAPE, centerx, 0x8);
CHECK_OFFSET(SHAPE, centery, 0xA);
CHECK_SIZE(SHAPE, 0x10);

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
