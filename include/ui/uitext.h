#ifndef UI_UITEXT_H
#define UI_UITEXT_H

// cUIText: a text object on a UI screen (object type 0x17: getType, cUIText_getType, returns 0x17).
// cUIScreen_createObjectByStruct allocates it with `cMemMan_alloc(0xBC, "UIText", ...)` and runs its constructor
// cUIText_cUIText, which calls the cUIThing constructor and stores _vt$7cUIText (cUIText_vtbl): 0xBC bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
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

struct cUIText {
    // --- cUIThing (see ui/uithing.h for the evidence of each member) ---
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext
    void** mVtbl;                    // 0x008 [R] _vt$7cUIText (cUIText_vtbl), stored by cUIText_cUIText/cUIText_cUIText1
    cUIAnimation* mpAnim;            // 0x00C [P] cUIThing::getAnimation
    unsigned char mAnimPlaybackStatus; // 0x010 [O] cUIThing
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] cUIThing's tAttrThing; [U] cUIText_render2D tests mCached/mEnabled/mStatic, reads mLayer
    int mMiscData;                   // 0x018 [P] cUIThing::getMiscData
    tARGB mColour;                   // 0x01C [P] cUIThing::setColour; [U] cUIText_render2D's default text colour
    t3Vector mAngle;                 // 0x02C [O] cUIThing; [P] getRadAngle = .z
    int mHashName;                   // 0x038 [P] cUIThing::getHashName
    unsigned short* mpEvents;        // 0x03C [O] cUIThing
    cUIThing* mpParent;              // 0x040 [P] cUIThing::getParent
    t3Vector mPosition;              // 0x044 [P] cUIThing::getPosition; [U] cUIText_render2D draws at mPosition + (x, y)
    t3Vector mScale;                 // 0x050 [P] cUIThing::getScale; [U] font scale in cUIText_render2D, func_003A1030
    cUIScreen* mpScreen;             // 0x05C [P] cUIThing::getScreen; [U] cUIText_getFont: mpScreen->mpState->mpEngine's fonts
    t3Vector mSize;                  // 0x060 [P] cUIThing::getSize; [U] cUIText_setUnicodeStringWrap wraps at mSize.x
    int mSound[2];                   // 0x06C [O] cUIThing
    // --- cUIText ---
    int mAttrText;                   // 0x074 [P] tAttrText: setFontIndex (bits 0-1), hasShadow (2), isUnicode (3), getUseStaticColour (4)
                                     //       (preview lw 0x6C; retail cUIText_setFontIndex..cUIText_setEllipses, which in On Tour's order also have
                                     //       get/setUseDisabledColour (5) and isEllipses/setEllipses (6)); [R] ctor sets bit 2, clears 3-6;
                                     //       [U] font index in cUIText_getFont, bit 3 in cUIText_getAsciiString/getUnicodeString
    tARGB mShadowColour;             // 0x078 [P] setShadowColour (preview 0x70; retail cUIText_setShadowColour); [U] cUIText_setUIData
                                     //       sets it from the record when the shadow bit is on; cUIText_render2D uses it for the shadow
    tARGB mStaticColour;             // 0x088 [P] setStaticColour (preview 0x80; retail cUIText_setStaticColour); [R] = D_004C66C8;
                                     //       [U] cUIText_render2D: the colour when the thing is static and bit 4 is set
    tARGB mDisabledColour;           // 0x098 [O]; [R] = D_004C66C8; [U] cUIText_render2D: the colour when the thing is not enabled
                                     //       and bit 5 is set
    int mNumLines;                   // 0x0A8 [O]; [R] = 0; [U] line count from cUIText_getNumTextLines (cUIText_setUnicodeStringPrivate);
                                     //       cUIText_render2D loops over it; func_003A10F0 returns it
    unsigned short** mpLineOffset;   // 0x0AC [O]; [R] = 0; [U] line start pointers (cUIText_setUnicodeStringPrivate), freed by cUIText_deleteText
    int mStringID;                   // 0x0B0 [O]; [R] = 0; [U] cUIText_setUnicodeStringByID stores the id, cUIText_updateLocale looks it
                                     //       up, setAsciiString/setUnicodeString clear it; getStringID (On Tour order) = retail cUIText_getStringID
    unsigned short* mpString;        // 0x0B4 [O]; [R] = 0; [U] the UCS-2 copy (cUIText_setUnicodeStringPrivate), returned by
                                     //       cUIText_getAsciiString/getUnicodeString, freed by cUIText_deleteText
    unsigned short mStringSize;      // 0x0B8 [O]; [R] = 0; [U] = length + 1 (cUIText_setUnicodeStringPrivate)
    unsigned short mLineSize;        // 0x0BA [O]; [R] = 0; [U] = mNumLines when there are 2+ lines (cUIText_setUnicodeStringPrivate)
};

CHECK_OFFSET(cUIText, mpPrev, 0x000);
CHECK_OFFSET(cUIText, mpNext, 0x004);
CHECK_OFFSET(cUIText, mVtbl, 0x008);
CHECK_OFFSET(cUIText, mpAnim, 0x00C);
CHECK_OFFSET(cUIText, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIText, mAttr, 0x014);
CHECK_OFFSET(cUIText, mMiscData, 0x018);
CHECK_OFFSET(cUIText, mColour, 0x01C);
CHECK_OFFSET(cUIText, mAngle, 0x02C);
CHECK_OFFSET(cUIText, mHashName, 0x038);
CHECK_OFFSET(cUIText, mpEvents, 0x03C);
CHECK_OFFSET(cUIText, mpParent, 0x040);
CHECK_OFFSET(cUIText, mPosition, 0x044);
CHECK_OFFSET(cUIText, mScale, 0x050);
CHECK_OFFSET(cUIText, mpScreen, 0x05C);
CHECK_OFFSET(cUIText, mSize, 0x060);
CHECK_OFFSET(cUIText, mSound, 0x06C);
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

#endif /* UI_UITEXT_H */
