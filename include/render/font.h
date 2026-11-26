#ifndef RENDER_FONT_H
#define RENDER_FONT_H

// cFont: a bitmap font loaded from an .sfn file ("FNTS" header, version >= 200): the glyph table, the font texture,
// and the draw state for text (offset, scales, colours). Size 0x84: cMemMan_alloc(0x84, "DebugFont") +
// cFont_cFont(self, "data/fonts/debugfnt.sfn", 0) in fe/debugfe.cpp and main/gameload.cpp (the third caller,
// func_003985xx, builds a table of fonts the same way). No base class, no virtuals. EA path (preview assert):
// source/render/font.cpp; the header name follows the unit.
//
// A FLAT layout: every field at its retail offset. Evidence tags:
//   [R] retail's constructor cFont_cFont (0x3915E8: MissingGlyph = 0, ShadowOffset (0,0), BaseScale (1,1),
//       Scale = BaseScale, Color (1,1,1,1), ShadowColor (1,0,0,0), FontName[0] = 0) and cFont_loadFont
//       (sprintf(FontName, "font:%24s", name));
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF class cFont: the same members in the same order at the same offsets up to FontName
//       (On Tour's size 0x90 only rounds up for its 16-aligned tARGBs; retail's are 4-aligned).
// t2Vector (2 floats) and tARGB (4 floats {A, R, G, B}) are written as float arrays: several units define their own.

#include "layout.h"

// [O] tGlyph, 0xC bytes: one character of the glyph table.
struct tGlyph {
    unsigned short code;            // 0x0 [O]; [U] cFont_charAdvance / cFont_charAdvance1 (find glyph by code)
    unsigned char width;            // 0x2 [O]
    unsigned char height;           // 0x3 [O]
    unsigned short u;               // 0x4 [O]
    unsigned short v;               // 0x6 [O]
    signed char advance;            // 0x8 [O]
    signed char xoffset;            // 0x9 [O]
    signed char yoffset;            // 0xA [O]
    unsigned char pad;              // 0xB [O]
};

struct cFont {
    // Character codes linkFont treats specially (Latin-1; names ours): [U] cFont_linkFont makes the glyph of
    // kChar_NotSign (0xAC) the MissingGlyph and gives kChar_NoBreakSpace (0xA0) the space glyph's width and height.
    enum { kChar_NoBreakSpace = 160, kChar_NotSign = 172 };
    // [U] textSmartLimit / textSmartLimit1 keep the index of the last character before a space as the place to break
    // the line; it starts at kNoBreakPoint, and while it is still there the text is cut inside the word (names ours).
    enum { kNoBreakPoint = -1 };
    int NumGlyphs;                  // 0x00 [O]; [U] cFont_linkFont, cFont_charAdvance
    tGlyph* Glyphs;                 // 0x04 [O]; [U] ditto
    tGlyph* MissingGlyph;           // 0x08 [O]; [R] ctor zeroes it; [U] cFont_charAdvance falls back to it
    int FirstCode;                  // 0x0C [O]; [U] cFont_charAdvance
    int LastLinearCode;             // 0x10 [O]; [U] cFont_charAdvance
    int MaxCharHeight;              // 0x14 [O]; [U] cFont_linkFont
    float Offset[2];                // 0x18 [O] t2Vector; [U] cFont_linkFont
    float UVScale[2];               // 0x20 [O] t2Vector; [U] cFont_linkFont / cFont_downloadTexture
    float ShadowOffset[2];          // 0x28 [O] t2Vector; [R] ctor (0, 0)
    float BaseScale[2];             // 0x30 [O] t2Vector; [R] ctor (1, 1)
    float Scale[2];                 // 0x38 [O] t2Vector; [R] ctor copies BaseScale
    float Color[4];                 // 0x40 [O] tARGB; [R] ctor (1, 1, 1, 1)
    float ShadowColor[4];           // 0x50 [O] tARGB; [R] ctor (1, 0, 0, 0)
    int TextureID;                  // 0x60 [O]; [U] cFont_downloadTexture
    char FontName[32];              // 0x64 [O]; [R] cFont_loadFont sprintf("font:%24s")

    // Methods bound to their retail symbols by asm labels.
    void linkFont(void* vdata, int flags) __asm__("cFont_linkFont");
    void purge() __asm__("cFont_purge");
    void vtextf(float x, float y) __asm__("cFont_vtextf");
    void text(float x, float y, const char* str) __asm__("cFont_text");
    void text1(float x, float y, void* unicodeStr) __asm__("cFont_text1");
    float textExtent(const char* str, void* r_, int mono, float sx, float sy) __asm__("cFont_textExtent");
    float textExtent1(const unsigned short* unicodeText, void* r_, int mono, float sx, float sy)
        __asm__("cFont_textExtent1");
    char* textSmartLimit(const char* str, char* dst, int max, float sx, float width) __asm__("cFont_textSmartLimit");
    unsigned short* textSmartLimit1(float sx, float width, const unsigned short* str, unsigned short* dst, int max)
        __asm__("cFont_textSmartLimit1");
    void centertext(float x, float y, const char* str) __asm__("cFont_centertext");
    void centertext1(const unsigned short* str, float x, float y) __asm__("cFont_centertext1");
    void vcentertextf(const char* fmt, void* ap, float x, float y) __asm__("cFont_vcentertextf");
    void righttext(const char* str, float x, float y) __asm__("cFont_righttext");
    void righttext1(const unsigned short* str, float x, float y) __asm__("cFont_righttext1");
    void vrighttextf(const char* fmt, void* ap, float x, float y) __asm__("cFont_vrighttextf");
    float charAdvance(char charCode) __asm__("cFont_charAdvance");
    float charAdvance1(unsigned short charCode) __asm__("cFont_charAdvance1");
    void* cFont1() __asm__("cFont_cFont1");
    void loadFont(const char* name, int flags) __asm__("cFont_loadFont");
};

// --- layout checks
CHECK_OFFSET(tGlyph, code, 0x0);
CHECK_OFFSET(tGlyph, width, 0x2);
CHECK_OFFSET(tGlyph, height, 0x3);
CHECK_OFFSET(tGlyph, u, 0x4);
CHECK_OFFSET(tGlyph, v, 0x6);
CHECK_OFFSET(tGlyph, advance, 0x8);
CHECK_OFFSET(tGlyph, xoffset, 0x9);
CHECK_OFFSET(tGlyph, yoffset, 0xA);
CHECK_OFFSET(tGlyph, pad, 0xB);
CHECK_SIZE(tGlyph, 0xC);

CHECK_OFFSET(cFont, NumGlyphs, 0x00);
CHECK_OFFSET(cFont, Glyphs, 0x04);
CHECK_OFFSET(cFont, MissingGlyph, 0x08);
CHECK_OFFSET(cFont, FirstCode, 0x0C);
CHECK_OFFSET(cFont, LastLinearCode, 0x10);
CHECK_OFFSET(cFont, MaxCharHeight, 0x14);
CHECK_OFFSET(cFont, Offset, 0x18);
CHECK_OFFSET(cFont, UVScale, 0x20);
CHECK_OFFSET(cFont, ShadowOffset, 0x28);
CHECK_OFFSET(cFont, BaseScale, 0x30);
CHECK_OFFSET(cFont, Scale, 0x38);
CHECK_OFFSET(cFont, Color, 0x40);
CHECK_OFFSET(cFont, ShadowColor, 0x50);
CHECK_OFFSET(cFont, TextureID, 0x60);
CHECK_OFFSET(cFont, FontName, 0x64);
CHECK_SIZE(cFont, 0x84);

#endif /* RENDER_FONT_H */
