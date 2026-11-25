#ifndef UI_UITEXTSCROLL_H
#define UI_UITEXTSCROLL_H

// cUITextScroll: a scrolling multi-line text box (record type 0x20), word-wrapped into lines and optionally driven by
// a cUIScrollBar. Size 0xD0: cUIScreen_createObjectByStruct does operator new(0xD0, str_cUITextScroll) + the ctor
// cUITextScroll_cUITextScroll (cUIThing ctor cUIThing_cUIThing, vtable cUITextScroll_vtbl, mList ctor cList_cList, zeroes 0xB8..0xCC,
// cUITextScroll_setInputMap fills mInputMap), then moves the record's children into mList. Default ctor cUITextScroll_cUITextScroll1
// (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source:
// d:/ssx/code/source/ui/uitextscroll.cpp (preview assert path); retail keeps its methods in ui/uitext
// (0x3A1148-0x3A2068) and its inline accessors in ui/uivector (0x3A6250-0x3A64F8).
// Methods: vtable cUITextScroll_vtbl slots 0 ~cUITextScroll (cUITextScroll__cUITextScroll), 9 setUIData (cUITextScroll_setUIData), 14 onInput
// (cUITextScroll_onInput), 15 updateLocale (cUITextScroll_updateLocale), 16 render2D (cUITextScroll_render2D), 21 getType (cUITextScroll_getType); line
// buffer helpers cUITextScroll_getFont, cUITextScroll_clearBuffer (free lines), cUITextScroll_allocLine, cUITextScroll_deleteLineByIndex; text setters
// cUITextScroll_addAsciiString / cUITextScroll_setAsciiString (ASCII), cUITextScroll_addUnicodeString, cUITextScroll_setUnicodeStringWrap / cUITextScroll_addUnicodeStringWrap (Unicode),
// cUITextScroll_addUnicodeStringByID (string id).
// Class name: the preview's cUITextScroll::getType (builds/x/v2/name_map_v2.tsv, cUITextScroll_getType) and On Tour.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUITextScroll 0xF0: its cUIThing is 0x80 and
// it aligns tARGB to 16, so the members sit 0xC-0x20 later there; same order and types).

#include "layout.h"
#include "ui/uithing.h"
#include "util/list.h"

// EA's tAttrTextScroll (On Tour: 1 byte). Retail's accessors read and write it as a word (lw/sw 0x74); the ctor
// cUITextScroll_cUITextScroll clears bits 0/3/4/5 as a byte (lbu, andi 0xC6, sb).
struct tAttrTextScroll {
    unsigned int mMode : 1;          // bit 0 [O]; [U] cUITextScroll_setChatWindowMode (set), cUITextScroll_isChatWindowMode (get)
    unsigned int mFont : 2;          // bits 1-2 [O]; [U] cUITextScroll_setFontIndex setFont
    unsigned int mShadow : 1;        // bit 3 [O]; [U] cUITextScroll_hasShadow (get), cUITextScroll_setShadow (set)
    unsigned int mUnicode : 1;       // bit 4 [O]; [U] cUITextScroll_isUnicode (get), cUITextScroll_setUnicode (set)
    unsigned int mStaticColour : 1;  // bit 5 [O]; [R] the ctor clears it
};

CHECK_SIZE(tAttrTextScroll, 0x4);

struct cUIInput;

struct cUITextScroll : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUITextScroll_vtbl (the ctor cUITextScroll_cUITextScroll, cUITextScroll_cUITextScroll1, the dtor
    //       cUITextScroll__cUITextScroll)
    // --- cUITextScroll
    // cUITextScroll's own slots ([R] cUITextScroll_vtbl; types [O] On Tour's class cUITextScroll):
    virtual char getType();                                               // 22 cUITextScroll_getType__FPv
    virtual void setShadow(int a);                                        // 23 cUITextScroll_setShadow
    virtual void setUnicode(int a);                                       // 24 cUITextScroll_setUnicode
    tAttrTextScroll mAttrTextScroll; // 0x074 [O]; [R] ctor cUITextScroll_cUITextScroll clears mMode/mShadow/mUnicode/mStaticColour
                                     //       (lbu/andi 0xC6/sb 0x74), cUITextScroll_cUITextScroll1 calls its ctor tAttrTextScroll_tAttrTextScroll; [U] setUIData
                                     //       cUITextScroll_setUIData (lw/sw), accessors cUITextScroll_setFontIndex..cUITextScroll_setUnicode, cUITextScroll_setChatWindowMode/90
    tARGB mShadowColour;             // 0x078 [O]; [U] setUIData cUITextScroll_setUIData copies it (ldl/ldr); cUITextScroll_setShadowColour (set)
    char mInputMap[4];               // 0x088 [O]; [R] cUITextScroll_setInputMap (called by the ctor) fills the four bytes; [U] onInput
                                     //       cUITextScroll_onInput (lb 0x88 / 0x89)
    cList mList;                     // 0x08C [O] the text lines' child objects (util/list.h); [R] ctor cList_cList on
                                     //       this + 0x8C, dtor cUITextScroll__cUITextScroll (cList_destroyList, vptrs 0xA4 / 0xA0 / 0x94);
                                     //       createObjectByStruct adds the record's children (cList_addToEnd on o + 0x8C);
                                     //       [U] cUITextScroll_first..cUITextScroll_addToEnd (first/last/getByIndex/addToFront/addToEnd)
    tARGB mSelectedColour;           // 0x0A8 [O]; [U] cUITextScroll_setSelectedColour (set, ldl/ldr), render2D cUITextScroll_render2D
    cUIThing* mpScrollBar;           // 0x0B8 [O] cUIScrollBar*; [R] = 0; [U] cUITextScroll_setScrollBar setScrollBar (sw); render2D
    unsigned short** mpLineOffset;   // 0x0BC [O] line start offsets (allocated); [R] = 0; [U] cUITextScroll_allocLine,
                                     //       cUITextScroll_clearBuffer (free), cUITextScroll_deleteLineByIndex, cUITextScroll_addUnicodeString, render2D
    unsigned int mStringID;          // 0x0C0 [O]; [R] = 0; [U] cUITextScroll_setAsciiString / cUITextScroll_addUnicodeStringByID (sw), cUITextScroll_updateLocale (lw)
    unsigned short mSelectedIndex;   // 0x0C4 [O]; [R] = 0 (sh); [U] cUITextScroll_setSelectedIndex (sh), cUITextScroll_getSelectedIndex (lhu), onInput
    unsigned short mLinesOffset;     // 0x0C6 [O]; [R] = 0; [U] cUITextScroll_setLineOffset (sh), cUITextScroll_getLineOffset (lhu), onInput, render2D
    unsigned short mLineSize;        // 0x0C8 [O] line count; [R] = 0; [U] cUITextScroll_getLineSize (lhu), allocLine / cUITextScroll_clearBuffer
    char unk_0CA[0x2];               // 0x0CA padding
    float mPixelOffset;              // 0x0CC [O] float; [R] = 0 (sw zero); [U] render2D cUITextScroll_render2D (lwc1 $f13 = y offset)

    // Methods bound to their retail symbols by asm labels.
    void updateLocale() __asm__("cUITextScroll_updateLocale");
    int onInput(cUIInput* input) __asm__("cUITextScroll_onInput");
    void render2D(float x, float y) __asm__("cUITextScroll_render2D");
    void clearBuffer(void* ignored) __asm__("cUITextScroll_clearBuffer");
    int allocLine() __asm__("cUITextScroll_allocLine");
    void addAsciiString(char* str) __asm__("cUITextScroll_addAsciiString");
    void addUnicodeString(unsigned short* str) __asm__("cUITextScroll_addUnicodeString");
    void setAsciiString(char* str) __asm__("cUITextScroll_setAsciiString");
    void setUnicodeStringWrap(void* pString) __asm__("cUITextScroll_setUnicodeStringWrap");
    void addUnicodeStringWrap(void* vstr) __asm__("cUITextScroll_addUnicodeStringWrap");
    void addUnicodeStringByID(int id) __asm__("cUITextScroll_addUnicodeStringByID");
    void deleteLineByIndex(int idx) __asm__("cUITextScroll_deleteLineByIndex");
    int getStringByIndex(int i) __asm__("cUITextScroll_getStringByIndex");
};

CHECK_OFFSET(cUITextScroll, mAttrTextScroll, 0x074);
CHECK_OFFSET(cUITextScroll, mShadowColour, 0x078);
CHECK_OFFSET(cUITextScroll, mInputMap, 0x088);
CHECK_OFFSET(cUITextScroll, mList, 0x08C);
CHECK_OFFSET(cUITextScroll, mSelectedColour, 0x0A8);
CHECK_OFFSET(cUITextScroll, mpScrollBar, 0x0B8);
CHECK_OFFSET(cUITextScroll, mpLineOffset, 0x0BC);
CHECK_OFFSET(cUITextScroll, mStringID, 0x0C0);
CHECK_OFFSET(cUITextScroll, mSelectedIndex, 0x0C4);
CHECK_OFFSET(cUITextScroll, mLinesOffset, 0x0C6);
CHECK_OFFSET(cUITextScroll, mLineSize, 0x0C8);
CHECK_OFFSET(cUITextScroll, unk_0CA, 0x0CA);
CHECK_OFFSET(cUITextScroll, mPixelOffset, 0x0CC);
CHECK_SIZE(cUITextScroll, 0xD0);

#endif /* UI_UITEXTSCROLL_H */
