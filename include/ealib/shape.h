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
