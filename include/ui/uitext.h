#ifndef UI_UITEXT_H
#define UI_UITEXT_H
#pragma interface

// cUIText: a text object on a UI screen (object type 0x17: getType, cUIText_getType, returns 0x17).
// cUIScreen_createObjectByStruct allocates it with `cMemMan_alloc(0xBC, "UIText", ...)` and runs its constructor
// cUIText_cUIText, which calls the cUIThing constructor and stores _vt$7cUIText (cUIText_vtbl): 0xBC bytes.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Base class: cUIThing (0x00-0x74, written out below with the names and evidence of ui/uithing.h; cUIThing's own base
// is cListNode). cUIText's members follow at 0x74.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIText), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (setFontIndex/hasShadow/isUnicode lw 0x6C, setShadowColour 0x70,
//       setStaticColour 0x80). Retail keeps their out-of-line copies in the same order (cUIText_getType getType ...
//       cUIText_getStringID in ui/uivector), which places each one: retail = preview + 8 (cUIThing grew from 0x6C to 0x74),
//       and + 0x18 after mStaticColour (retail added mDisabledColour, as On Tour has);
//   [R] retail's constructor cUIThing_cUIThing + cUIText_cUIText;
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail = On Tour - 0xC (mAttrText) / - 0x18 (the rest): On Tour aligns tARGB to 16 bytes.
//
// mAttrText is EA's tAttrText (On Tour, LSB first): mFont:2 mShadow:1 mUnicode:1 mStaticColour:1 mDisabledColour:1
// mEllipses:1. EA's type is 1 byte (the constructor uses lbu/sb, cUIText_getFont lbu), but the accessors and most
// methods read and write the aligned word (lw/sw 0x74), so it is an int here and covers the 3 padding bytes.

#include "layout.h"
#include "ui/uithing.h" // t3Vector, tARGB and the cUIThing members' pointee types

// EA's tAttrText, the type of cUIText::mAttrText (bits named for code that reads one field, as tAttrThing for
// cUIThing::mAttr: ((tAttrText*)&text->mAttrText)->mFont). [O] On Tour struct tAttrText (1 byte, unsigned char
// bit-fields, LSB first); [P] the preview's inline accessors setFontIndex (bits 0-1), hasShadow (2), isUnicode (3),
// getUseStaticColour (4); [U] retail reads and writes the word (lw/sw 0x74; the views sUIThing03F0 mode : 2,
// cUITextScroll's sibling tAttrTextScroll is a word too), so the bit-fields are unsigned int here.
struct tAttrText {
    unsigned int mFont : 2;          // bits 0-1 [O]; [P] setFontIndex; [U] cUIText_getFont, cUIText_setUIData
    unsigned int mShadow : 1;        // bit 2 [O]; [P] hasShadow; [R] the ctor sets it
    unsigned int mUnicode : 1;       // bit 3 [O]; [P] isUnicode; [U] cUIText_getAsciiString / getUnicodeString
    unsigned int mStaticColour : 1;  // bit 4 [O]; [P] getUseStaticColour
    unsigned int mDisabledColour : 1; // bit 5 [O]; [U] get/setUseDisabledColour (On Tour's order)
    unsigned int mEllipses : 1;      // bit 6 [O]; [U] isEllipses / setEllipses (On Tour's order)
};

CHECK_SIZE(tAttrText, 0x4);

struct tARGB;

struct cUIText : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] _vt$7cUIText (cUIText_vtbl), stored by cUIText_cUIText/cUIText_cUIText1
    // cUIText's uses of cUIThing's fields:
    //   mAttr: [U] cUIText_render2D tests mCached/mEnabled/mStatic, reads mLayer
    //   mColour: [U] cUIText_render2D's default text colour
    //   mPosition: [U] cUIText_render2D draws at mPosition + (x, y)
    //   mScale: [U] font scale in cUIText_render2D, cUIText_getTextExtent
    //   mpScreen: [U] cUIText_getFont: mpScreen->mpState->mpEngine's fonts
    //   mSize: [U] cUIText_setUnicodeStringWrap wraps at mSize.x
    // --- cUIText ---
    // cUIText's own slots ([R] cUIText_vtbl; types [O] On Tour's class cUIText):
    virtual char getType();                                               // 22 cUIText_getType__FPv
    virtual void setShadow(int a)
        __asm__("cUIText_setShadow");                                        // 23 cUIText_setShadow
    virtual void setUnicode(int a)
        __asm__("cUIText_setUnicode");                                       // 24 cUIText_setUnicode
    virtual void setUseStaticColour(int a)
        __asm__("cUIText_setUseStaticColour");                               // 25 cUIText_setUseStaticColour
    virtual void setUseDisabledColour(int a)
        __asm__("cUIText_setUseDisabledColour");                             // 26 cUIText_setUseDisabledColour
    virtual void setEllipses(int a)
        __asm__("cUIText_setEllipses");                                      // 27 cUIText_setEllipses
    int mAttrText;                   // 0x074 [P] tAttrText: setFontIndex (bits 0-1), hasShadow (2), isUnicode (3), getUseStaticColour (4)
                                     //       (preview lw 0x6C; retail cUIText_setFontIndex..cUIText_setEllipses, which in On Tour's order also have
                                     //       get/setUseDisabledColour (5) and isEllipses/setEllipses (6)); [R] ctor sets bit 2, clears 3-6;
                                     //       [U] font index in cUIText_getFont, bit 3 in cUIText_getAsciiString/getUnicodeString
    tARGB mShadowColour;             // 0x078 [P] setShadowColour (preview 0x70; retail cUIText_setShadowColour); [U] cUIText_setUIData
                                     //       sets it from the record when the shadow bit is on; cUIText_render2D uses it for the shadow
    tARGB mStaticColour;             // 0x088 [P] setStaticColour (preview 0x80; retail cUIText_setStaticColour); [R] = gStaticColour;
                                     //       [U] cUIText_render2D: the colour when the thing is static and bit 4 is set
    tARGB mDisabledColour;           // 0x098 [O]; [R] = gStaticColour; [U] cUIText_render2D: the colour when the thing is not enabled
                                     //       and bit 5 is set
    int mNumLines;                   // 0x0A8 [O]; [R] = 0; [U] line count from cUIText_getNumTextLines (cUIText_setUnicodeStringPrivate);
                                     //       cUIText_render2D loops over it; cUIText_getNumLinesOfString returns it
    unsigned short** mpLineOffset;   // 0x0AC [O]; [R] = 0; [U] line start pointers (cUIText_setUnicodeStringPrivate), freed by cUIText_deleteText
    int mStringID;                   // 0x0B0 [O]; [R] = 0; [U] cUIText_setUnicodeStringByID stores the id, cUIText_updateLocale looks it
                                     //       up, setAsciiString/setUnicodeString clear it; getStringID (On Tour order) = retail cUIText_getStringID
    unsigned short* mpString;        // 0x0B4 [O]; [R] = 0; [U] the UCS-2 copy (cUIText_setUnicodeStringPrivate), returned by
                                     //       cUIText_getAsciiString/getUnicodeString, freed by cUIText_deleteText
    unsigned short mStringSize;      // 0x0B8 [O]; [R] = 0; [U] = length + 1 (cUIText_setUnicodeStringPrivate)
    unsigned short mLineSize;        // 0x0BA [O]; [R] = 0; [U] = mNumLines when there are 2+ lines (cUIText_setUnicodeStringPrivate)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cUIText();   // g++'s own symbol _$_7cUIText (the retail label renamed)
    void render2D(float x, float y) __asm__("cUIText_render2D");
    void deleteText() __asm__("cUIText_deleteText");
    int getAsciiString() __asm__("cUIText_getAsciiString");
    int getUnicodeString() __asm__("cUIText_getUnicodeString");
    void setAsciiString(const char* str) __asm__("cUIText_setAsciiString__FP7cUITextPCc");
    void setAsciiStringPrivate(const char* str) __asm__("cUIText_setAsciiStringPrivate");
    void setUnicodeStringWrap(unsigned short* str) __asm__("cUIText_setUnicodeStringWrap");
    void setUnicodeString(const unsigned short* str) __asm__("cUIText_setUnicodeString");
    void setUnicodeStringPrivate(const unsigned short* str) __asm__("cUIText_setUnicodeStringPrivate");
    void setUnicodeStringByID(int id) __asm__("cUIText_setUnicodeStringByID__FP7cUITexti");
    int getNumLinesOfString() __asm__("cUIText_getNumLinesOfString");
    int getFont() __asm__("cUIText_getFont");
    void* cUIText1() __asm__("cUIText_cUIText1");
    void setFontIndex(int fontIndex) __asm__("cUIText_setFontIndex");
    int hasShadow() __asm__("cUIText_hasShadow");
    int isUnicode() __asm__("cUIText_isUnicode");
    int getUseStaticColour() __asm__("cUIText_getUseStaticColour");
    int getUseDisabledColour() __asm__("cUIText_getUseDisabledColour");
    int isEllipses() __asm__("cUIText_isEllipses");
    void setShadowColour(tARGB* in) __asm__("cUIText_setShadowColour");
    void setStaticColour(tARGB* in) __asm__("cUIText_setStaticColour");
    int getStringID() __asm__("cUIText_getStringID");
    virtual void updateLocale() __asm__("cUIText_updateLocale");
    char getNumTextLines() __asm__("cUIText_getNumTextLines");
    virtual void setUIData(tUIBaseObject* pObj) __asm__("cUIText_setUIData");
};

CHECK_OFFSET(cUIText, mAttrText, 0x074);
CHECK_OFFSET(cUIText, mShadowColour, 0x078);
CHECK_OFFSET(cUIText, mStaticColour, 0x088);
CHECK_OFFSET(cUIText, mDisabledColour, 0x098);
CHECK_OFFSET(cUIText, mNumLines, 0x0A8);
CHECK_OFFSET(cUIText, mpLineOffset, 0x0AC);
CHECK_OFFSET(cUIText, mStringID, 0x0B0);
CHECK_OFFSET(cUIText, mpString, 0x0B4);
CHECK_OFFSET(cUIText, mStringSize, 0x0B8);
CHECK_OFFSET(cUIText, mLineSize, 0x0BA);
CHECK_SIZE(cUIText, 0xBC);
__asm__("_vt$7cUIText = cUIText_vtbl");   // g++'s name for the retail vtable

#endif /* UI_UITEXT_H */
