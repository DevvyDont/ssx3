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

// tTextPictElemSpec (0x8): one segment. [O] float width + an anonymous union (On Tour does not list its members).

struct tTextPictElemSpec {
    float width;                     // 0x000 [O]; [U] split sums the segment widths into tTextPictLine::width
    void* unk_04;                    // 0x004 [O] anonymous union; [U] split stores the segment's start in text[]
                                     //       (unsigned short*) or its icon record; draw prints / blits it (view
                                     //       sSeg_1E92A8: unsigned short* str)
};

CHECK_OFFSET(tTextPictElemSpec, width, 0x000);
CHECK_OFFSET(tTextPictElemSpec, unk_04, 0x004);
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

#endif /* FE_TEXTPICTLINE_H */
