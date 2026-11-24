#ifndef UI_UILISTBOX_H
#define UI_UILISTBOX_H

// cUIListBox: a list of up to 32 text entries (ASCII, Unicode or string-table id, each with a user data pointer), one of
// them current (record type 0x12). Size 0x31C: cUIScreen_createObjectByStruct does operator new(0x31C, "UIListBox"...)
// + the ctor cUIListBox_cUIListBox (cUIThing ctor cUIThing_cUIThing, vtable cUIListBox_vtbl, clears the entries, mNumEntries,
// mCurrentEntry, mStaticColour = gStaticColour); default ctor cUIListBox_cUIListBox1 (ui/uivector). Base: cUIThing (ui/uithing.h,
// 0x74) <- cListNode, written out flat below. EA source: d:/ssx/code/source/ui/uilistbox.cpp (preview assert path);
// retail keeps its methods in ui/uiengine (0x399768-0x3993F8), ui/uilistbox (0x39A3F8-0x39A928) and its inline
// accessors in ui/uivector (0x3A5658-0x3A57B8).
// Methods: vtable cUIListBox_vtbl slots 0 ~cUIListBox (cUIListBox__cUIListBox), 9 setUIData (cUIListBox_setUIData), 14 onInput
// (cUIListBox_onInput), 15 updateLocale (cUIListBox_updateLocale), 16 render2D (cUIListBox_render2D), 21 getType (cUIListBox_getType),
// 22 setShadow (cUIListBox_setShadow), 23 setUseStaticColour (cUIListBox_setUseStaticColour); cUIListBox_addEntryByAsciiString,
// cUIListBox_addEntryByUnicodeString, cUIListBox_addEntryByStringID, cUIListBox_deleteAllEntries (clear; the dtor calls it),
// cUIListBox_getCurrentAsciiText / cUIListBox_getCurrentEntryData (current entry's ASCII text / data), cUIListBox_getEntryByData (find by data), cUIListBox_setCurrentEntryByIndex
// (setCurrentEntry), cUIListBox_setEntryDataByIndex (setEntryData), cUIListBox_setEntryByAsciiString, cUIListBox_setCallbackListBox, cUIListBox_getFont,
// cUIListBox_setUnicodePrivate (setEntryUnicode).
// Class name: the preview's cUIListBox::getType (builds/x/v2/name_map_v2.tsv, cUIListBox_getType) and On Tour.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUIListBox 0x340: its cUIThing is 0x80 and it
// aligns tARGB to 16, so mAttrListBox / the colours / the entries sit 0xC / 0x18 later there; the entry array is
// the same 32 x 0x14).

#include "layout.h"
#include "ui/uithing.h"

// EA's tAttrListBox (On Tour: 1 byte). Retail's accessors read and write it as a word (lw/sw 0x74); only the ctor
// cUIListBox_cUIListBox touches it as a byte (lbu/sb 0x74).
struct tAttrListBox {
    unsigned int mFont : 2;          // bits 0-1 [O]; [U] cUIListBox_setFontIndex setFont
    unsigned int mShadow : 1;        // bit 2 [O]; [R] ctor sets it; [U] cUIListBox_hasShadow hasShadow, cUIListBox_setShadow setShadow
    unsigned int mUnicode : 1;       // bit 3 [O]; [R] ctor clears it
    unsigned int mCallback : 1;      // bit 4 [O]; [R] ctor clears it
    unsigned int mStaticColour : 1;  // bit 5 [O]; [U] cUIListBox_getUseStaticColour getUseStaticColour, cUIListBox_setUseStaticColour setUseStaticColour
};

CHECK_SIZE(tAttrListBox, 0x4);

// EA's tListBoxEntry (On Tour: 0x14, same in retail: the ctor's clear loop steps 20 bytes, 32 times).
struct tListBoxEntry {
    unsigned int mStringID;          // 0x00 [O]; [U] cUIListBox_addEntryByStringID; updateLocale cUIListBox_updateLocale (lw 0x98 + i*0x14)
    const unsigned short* mpString;  // 0x04 [O]; [U] cUIListBox_setUnicodePrivate / setEntryByAsciiString free and replace it (lw/sw 0x9C + i*0x14)
    const char* mpAsciiString;       // 0x08 [O]; [U] cUIListBox_addEntryBy* (sw 0xA0 + i*0x14), cUIListBox_getCurrentAsciiText (lw)
    void* mpData;                    // 0x0C [O]; [U] cUIListBox_addEntryBy* (sw 0xA4 + i*0x14), cUIListBox_getCurrentEntryData / cUIListBox_getEntryByData (lw),
                                     //      cUIListBox_setEntryDataByIndex (sw)
    char mUnicodeBufferSize;         // 0x10 [O]; [U] cUIListBox_setEntryByAsciiString (lb 0xA8 + i*0x14)
    char unk_11[0x3];                // 0x11 padding
};

CHECK_OFFSET(tListBoxEntry, mStringID, 0x00);
CHECK_OFFSET(tListBoxEntry, mpString, 0x04);
CHECK_OFFSET(tListBoxEntry, mpAsciiString, 0x08);
CHECK_OFFSET(tListBoxEntry, mpData, 0x0C);
CHECK_OFFSET(tListBoxEntry, mUnicodeBufferSize, 0x10);
CHECK_SIZE(tListBoxEntry, 0x14);

struct cUIListBox : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIListBox_vtbl (the ctor cUIListBox_cUIListBox, cUIListBox_cUIListBox1, the dtor cUIListBox__cUIListBox)
    // --- cUIListBox
    // cUIListBox's own slots ([R] cUIListBox_vtbl; types [O] On Tour's class cUIListBox):
    virtual char getType();                                               // 22 cUIListBox_getType__FPv
    virtual void setShadow(int a);                                        // 23 cUIListBox_setShadow
    virtual void setUseStaticColour(int a);                               // 24 cUIListBox_setUseStaticColour
    tAttrListBox mAttrListBox;       // 0x074 [O] tAttrListBox; [R] the ctor cUIListBox_cUIListBox sets mShadow, clears mUnicode /
                                     //       mCallback (lbu/sb 0x74), cUIListBox_cUIListBox1 calls its ctor tAttrListBox_tAttrListBox; [U] setUIData
                                     //       cUIListBox_setUIData (lw/sw), accessors cUIListBox_setFontIndex..cUIListBox_setUseStaticColour (lw/sw)
    tARGB mShadowColour;             // 0x078 [O]; [U] setUIData cUIListBox_setUIData copies it from the record; cUIListBox_setShadowColour
                                     //       setShadowColour (ldl/ldr); render2D
    tARGB mStaticColour;             // 0x088 [O]; [R] = gStaticColour (the ctor); [U] setUIData; cUIListBox_setStaticColour setStaticColour
    tListBoxEntry mpEntries[32];     // 0x098 [O]; [R] the ctor clears all 32 (sw/sb loop + memset(this + 0x98, 0, 640));
                                     //       [U] cUIListBox_addEntryBy*, cUIListBox_setEntryByAsciiString, cUIListBox_deleteAllEntries (clear),
                                     //       cUIListBox_getCurrentAsciiText..cUIListBox_setEntryDataByIndex, updateLocale cUIListBox_updateLocale, render2D
    unsigned char mNumEntries;       // 0x318 [O]; [R] = 0; [U] lbu/sb in the add/clear functions; cUIListBox_getNumEntries (lbu)
    unsigned char mCurrentEntry;     // 0x319 [O]; [R] = 0; [U] onInput cUIListBox_onInput (lbu/sb), cUIListBox_setCurrentEntryByIndex setCurrent;
                                     //       cUIListBox_getCurrentEntryIndex getCurrentEntryIndex (lbu)
    char unk_31A[0x2];               // 0x31A padding
};

CHECK_OFFSET(cUIListBox, mAttrListBox, 0x074);
CHECK_OFFSET(cUIListBox, mShadowColour, 0x078);
CHECK_OFFSET(cUIListBox, mStaticColour, 0x088);
CHECK_OFFSET(cUIListBox, mpEntries, 0x098);
CHECK_OFFSET(cUIListBox, mNumEntries, 0x318);
CHECK_OFFSET(cUIListBox, mCurrentEntry, 0x319);
CHECK_OFFSET(cUIListBox, unk_31A, 0x31A);
CHECK_SIZE(cUIListBox, 0x31C);

#endif /* UI_UILISTBOX_H */
