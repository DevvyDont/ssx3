#ifndef FE_TEXTPICTLINE_H
#define FE_TEXTPICTLINE_H

// tTextPictLine: one line of HUD text with embedded pictures (button icons such as "@skip"), laid out once and drawn
// every frame: the wide string, the font and its base scale, and up to 6 segments (text runs or icons) with their
// widths. Plain struct, no vtable, no methods; the free functions that take it as their first argument (On Tour's
// DWARF: splitTextPictLine(tTextPictLine*), makeTextPictLine(tTextPictLine*, const unsigned short*, cFont*, const
// t2Vector&), drawTextPictLine(tTextPictLine*, t2Vector, const t2Vector&, const tARGB&, int, int)) are retail
// splitTextPictLine (split), makeTextPictLine (make; func_001E94E0 is a char* variant that widens the string first) and
// drawTextPictLine (draw), all in the decomp's unit fe/ovstatehud. cOVStateHUD1P embeds four (fe/ovstatehud.h:
// mTextUberHelp 0x560, mTextRecoverHelp 0x6AC, mTextRecoverHelpAlt 0x7F8, mSkipMessage 0xB48, each char[0x14C]).
// Size 0x14C: [O] On Tour's struct is 0x14C with the same members; [R] the four members of cOVStateHUD1P are 0x14C
// apart; every retail access (make: font 0x8, scale 0x10 / 0x14, text 0x4C; split: all members; draw: all members)
// lands on an On Tour member start. EA's source file is not known (On Tour lists it in fe's all_in_one unit).
//
// A FLAT layout. Evidence tags: [O] SSX On Tour's DWARF (names, types, order and offsets: no bools or vptrs, so they
// carry over); [U] retail usage in splitTextPictLine / makeTextPictLine / drawTextPictLine (src/fe/ovstatehud.cpp views
// sLine_1E92A8 / sHudLine_E95A0).

#include "layout.h"

struct cFont;
struct tHUDPictureDB;

// tTextPictElemSpec (0x8): one segment. [O] float width + an anonymous union (On Tour does not list its members; the
// two below are ours). Even segments are text runs, odd ones pictures.

struct tTextPictElemSpec {
    float width;                     // 0x000 [O]; [U] split sums the segment widths into tTextPictLine::width
    union {                          // 0x004 [O] anonymous union
        unsigned short* pText;       //       [U] a text segment: split stores its start in text[], draw prints it
                                     //       (name ours)
        tHUDPictureDB* pPicture;     //       [U] a picture segment: split stores its gTextPictDB record, draw blits
                                     //       it (name ours)
    };
};

CHECK_OFFSET(tTextPictElemSpec, width, 0x000);
CHECK_OFFSET(tTextPictElemSpec, pText, 0x004);
CHECK_OFFSET(tTextPictElemSpec, pPicture, 0x004);
CHECK_SIZE(tTextPictElemSpec, 0x8);

struct tTextPictLine {
    float width;                     // 0x000 [O]; [U] split stores the total width (swc1)
    float height;                    // 0x004 [O]; [U] split stores the line height; draw centres on it
    cFont* pFont;                    // 0x008 [O]; [U] make stores its font argument; split / draw read it
    float fontOffsetY;               // 0x00C [O]; [U] split stores it, draw adds it
    float fontScaleBase[2];          // 0x010 [O] t2Vector fontScaleBase (no shared t2Vector definition in include/
                                     //       yet); [U] make copies its scale argument; split / draw read x / y (lwc1
                                     //       0x10 / 0x14)
    int numElems;                    // 0x018 [O]; [U] split counts the segments (sw); draw returns early at 0
    tTextPictElemSpec elems[6];      // 0x01C [O]; [U] split fills them; draw walks them
    unsigned short text[128];        // 0x04C [O]; [U] make copies the message (wide chars) here; split scans it (lhu)
};

CHECK_OFFSET(tTextPictLine, width, 0x000);
CHECK_OFFSET(tTextPictLine, height, 0x004);
CHECK_OFFSET(tTextPictLine, pFont, 0x008);
CHECK_OFFSET(tTextPictLine, fontOffsetY, 0x00C);
CHECK_OFFSET(tTextPictLine, fontScaleBase, 0x010);
CHECK_OFFSET(tTextPictLine, numElems, 0x018);
CHECK_OFFSET(tTextPictLine, elems, 0x01C);
CHECK_OFFSET(tTextPictLine, text, 0x04C);
CHECK_SIZE(tTextPictLine, 0x14C);

// tHUDPictureDB (0x24): one picture a text-picture line can embed ("@skip" and the other button icons), looked up by
// the hash of its name. [O] On Tour struct tHUDPictureDB (0x24: ident, texHashID, texID, t2vect size, uv0, uv1), the
// element of gTextPictDB[8] that splitTextPictLine searches; [R] retail's splitTextPictLine walks the 0x24-stride table
// gTextPictDB until a zero ident and compares GetHashValue32(name) with the word at 0x0; [U] split reads the size at
// 0xC / 0x10 (the segment's width, the line height), drawTextPictLine the texture at 0x8 and the UVs at 0x14 .. 0x20
// (src/fe/ovstatehud.cpp views sIcon_1E92A8 / sHudImg_E95A0). The t2vect members are float[2], as fontScaleBase above.
struct tHUDPictureDB {
    unsigned int ident;              // 0x000 [O]; [U] the name's hash (GetHashValue32); 0 ends the table
    unsigned int texHashID;          // 0x004 [O]
    int texID;                       // 0x008 [O]; [U] drawTextPictLine's texture
    float size[2];                   // 0x00C [O] t2vect size; [U] split: width (lwc1 0xC), height (lwc1 0x10)
    float uv0[2];                    // 0x014 [O] t2vect; [U] drawTextPictLine
    float uv1[2];                    // 0x01C [O] t2vect; [U] drawTextPictLine
};

CHECK_OFFSET(tHUDPictureDB, ident, 0x000);
CHECK_OFFSET(tHUDPictureDB, texHashID, 0x004);
CHECK_OFFSET(tHUDPictureDB, texID, 0x008);
CHECK_OFFSET(tHUDPictureDB, size, 0x00C);
CHECK_OFFSET(tHUDPictureDB, uv0, 0x014);
CHECK_OFFSET(tHUDPictureDB, uv1, 0x01C);
CHECK_SIZE(tHUDPictureDB, 0x24);

#endif /* FE_TEXTPICTLINE_H */
