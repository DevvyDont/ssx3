#ifndef UI_UILISTBOX_H
#define UI_UILISTBOX_H

// cUIListBox: a list of up to 32 text entries (ASCII, Unicode or string-table id, each with a user data pointer), one of
// them current (record type 0x12). Size 0x31C: cUIScreen_createObjectByStruct does operator new(0x31C, "UIListBox"...)
// + the ctor cUIListBox_cUIListBox (cUIThing ctor func_0039FB30, vtable D_00494798, clears the entries, mNumEntries,
// mCurrentEntry, mStaticColour = D_004C66C8); default ctor func_003A55D8 (ui/uivector). Base: cUIThing (ui/uithing.h,
// 0x74) <- cListNode, written out flat below. EA source: d:/ssx/code/source/ui/uilistbox.cpp (preview assert path);
// retail keeps its methods in ui/uiengine (0x399768-0x3993F8), ui/uilistbox (0x39A3F8-0x39A928) and its inline
// accessors in ui/uivector (0x3A5658-0x3A57B8).
// Methods: vtable D_00494798 slots 0 ~cUIListBox (cUIListBox__cUIListBox), 9 setUIData (func_00399820), 14 onInput
// (func_00399970), 15 updateLocale (func_00399E28), 16 render2D (func_00399F00), 21 getType (func_003A56A0),
// 22 setShadow (func_003A56D8), 23 setUseStaticColour (func_003A5708); cUIListBox_addEntryByAsciiString,
// func_0039A4B0 (addEntryByUnicodeString), cUIListBox_addEntryByStringID, func_0039A670 (clear; the dtor calls it),
// func_0039A708 / func_0039A738 (current entry's ASCII text / data), func_0039A768 (find by data), func_0039A7A8
// (setCurrentEntry), func_0039A7C8 (setEntryData), cUIListBox_setEntryByAsciiString, func_0039A8D8, func_00399920,
// func_00399D80 (setEntryUnicode).
// Class name: the preview's cUIListBox::getType (builds/x/v2/name_map_v2.tsv, func_003A56A0) and On Tour.
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUIListBox 0x340: its cUIThing is 0x80 and it
// aligns tARGB to 16, so mAttrListBox / the colours / the entries sit 0xC / 0x18 later there; the entry array is
// the same 32 x 0x14).

#include "layout.h"
#include "ui/uithing.h"

// EA's tAttrListBox (On Tour: 1 byte). Retail's accessors read and write it as a word (lw/sw 0x74); only the ctor
// cUIListBox_cUIListBox touches it as a byte (lbu/sb 0x74).
struct tAttrListBox {
    unsigned int mFont : 2;          // bits 0-1 [O]; [U] func_003A56A8 setFont
    unsigned int mShadow : 1;        // bit 2 [O]; [R] ctor sets it; [U] func_003A56C8 hasShadow, func_003A56D8 setShadow
    unsigned int mUnicode : 1;       // bit 3 [O]; [R] ctor clears it
    unsigned int mCallback : 1;      // bit 4 [O]; [R] ctor clears it
    unsigned int mStaticColour : 1;  // bit 5 [O]; [U] func_003A56F8 getUseStaticColour, func_003A5708 setUseStaticColour
};

CHECK_SIZE(tAttrListBox, 0x4);

// EA's tListBoxEntry (On Tour: 0x14, same in retail: the ctor's clear loop steps 20 bytes, 32 times).
struct tListBoxEntry {
    unsigned int mStringID;          // 0x00 [O]; [U] cUIListBox_addEntryByStringID; updateLocale func_00399E28 (lw 0x98 + i*0x14)
    const unsigned short* mpString;  // 0x04 [O]; [U] func_00399D80 / setEntryByAsciiString free and replace it (lw/sw 0x9C + i*0x14)
    const char* mpAsciiString;       // 0x08 [O]; [U] cUIListBox_addEntryBy* (sw 0xA0 + i*0x14), func_0039A708 (lw)
    void* mpData;                    // 0x0C [O]; [U] cUIListBox_addEntryBy* (sw 0xA4 + i*0x14), func_0039A738 / func_0039A768 (lw),
                                     //      func_0039A7C8 (sw)
    char mUnicodeBufferSize;         // 0x10 [O]; [U] cUIListBox_setEntryByAsciiString (lb 0xA8 + i*0x14)
    char unk_11[0x3];                // 0x11 padding
};

CHECK_OFFSET(tListBoxEntry, mStringID, 0x00);
CHECK_OFFSET(tListBoxEntry, mpString, 0x04);
CHECK_OFFSET(tListBoxEntry, mpAsciiString, 0x08);
CHECK_OFFSET(tListBoxEntry, mpData, 0x0C);
CHECK_OFFSET(tListBoxEntry, mUnicodeBufferSize, 0x10);
CHECK_SIZE(tListBoxEntry, 0x14);

struct cUIListBox {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] D_00494798 (the ctor cUIListBox_cUIListBox, func_003A55D8, the dtor cUIListBox__cUIListBox)
    cUIAnimation* mpAnim;            // 0x00C [P] getAnimation/isPlayingAnimation (lw 0xC; retail func_003A4950/func_003A4940);
                                     //       [U] cUIThing_playAnimation stores it, cUIThing_deleteAnimation deletes it
    unsigned char mAnimPlaybackStatus; // 0x010 [O]; [R] = 0; [U] cUIThing_playAnimation stores the playback mode, cUIThing_update passes it on
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] tAttrThing: isActive..isWrap, get/setDrawLayer, get/setPivot (lw/sw 0x14;
                                     //       retail func_003A4958..func_003A4BB8, cUIThing_setActive..setVisible); [R] ctor sets it
    int mMiscData;                   // 0x018 [P] getMiscData/setMiscData (lw/sw 0x18; retail func_003A4AC8/func_003A4AD0); [R] = 0
    tARGB mColour;                   // 0x01C [P] setColour/getColour (0x1C; retail cUIThing_setColour, func_003A4B20/func_003A4B28);
                                     //       [R] = {1,1,1,1}; [U] keyframe channels 13-16 (x 1/255) in cUIThing_setChannelData
    t3Vector mAngle;                 // 0x02C [O]; [P] getRadAngle/setRadAngle = mAngle.z (lwc1/swc1 0x34; retail func_003A4B58/func_003A4B50);
                                     //       [R] = t3vect_Zero; [U] channels 3-5 (degrees -> radians)
    int mHashName;                   // 0x038 [P] getHashName/setHashName (lw/sw 0x38; retail func_003A4BD0/func_003A4BC8);
                                     //       [U] compared by cUIThing_getObjectByHashName; set from the object record (cUIScreen_createAllObjects)
    unsigned short* mpEvents;        // 0x03C [O]; [R] = 0; [U] keyframer event per index, 0xFFFF = none (cUIThing_getKeyframerEvent);
                                     //       cUIThing_setUIData points it after the object record
    cUIThing* mpParent;              // 0x040 [P] getParent/setParent (lw/sw 0x40; retail func_003A4C00/func_003A4BF8); [R] = pParent;
                                     //       [U] func_003A0290 sums mPosition up the parent chain
    t3Vector mPosition;              // 0x044 [P] getPosition/setPosition (retail func_003A4C08/func_003A4C28); [U] channels 0-1
    t3Vector mScale;                 // 0x050 [P] getScale/setScale (retail func_003A4C48/func_003A4C68); [R] = {1,1,1}; [U] channels 9-10 (x 0.01)
    cUIScreen* mpScreen;             // 0x05C [P] getScreen (preview lw 0x68; retail func_003A4C88 lw 0x5C); [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] getSize/setSize (preview 0x5C; retail cUIThing_getSize/setSize, vtable slots 20/21);
                                     //       [U] channels 6-7; cUIText wraps at mSize.x
    int mSound[2];                   // 0x06C [O] (On Tour: int mSound[2]); [R] = {4, 6}; [U] func_003A0318/func_003A0330 get/set by index
    // --- cUIListBox
    tAttrListBox mAttrListBox;       // 0x074 [O] tAttrListBox; [R] the ctor cUIListBox_cUIListBox sets mShadow, clears mUnicode /
                                     //       mCallback (lbu/sb 0x74), func_003A55D8 calls its ctor func_003A65A0; [U] setUIData
                                     //       func_00399820 (lw/sw), accessors func_003A56A8..func_003A5708 (lw/sw)
    tARGB mShadowColour;             // 0x078 [O]; [U] setUIData func_00399820 copies it from the record; func_003A5758
                                     //       setShadowColour (ldl/ldr); render2D
    tARGB mStaticColour;             // 0x088 [O]; [R] = D_004C66C8 (the ctor); [U] setUIData; func_003A5780 setStaticColour
    tListBoxEntry mpEntries[32];     // 0x098 [O]; [R] the ctor clears all 32 (sw/sb loop + memset(this + 0x98, 0, 640));
                                     //       [U] cUIListBox_addEntryBy*, cUIListBox_setEntryByAsciiString, func_0039A670 (clear),
                                     //       func_0039A708..func_0039A7C8, updateLocale func_00399E28, render2D
    unsigned char mNumEntries;       // 0x318 [O]; [R] = 0; [U] lbu/sb in the add/clear functions; func_003A57B0 (lbu)
    unsigned char mCurrentEntry;     // 0x319 [O]; [R] = 0; [U] onInput func_00399970 (lbu/sb), func_0039A7A8 setCurrent;
                                     //       func_003A57A8 getCurrentEntryIndex (lbu)
    char unk_31A[0x2];               // 0x31A padding
};

CHECK_OFFSET(cUIListBox, mpPrev, 0x000);
CHECK_OFFSET(cUIListBox, mpNext, 0x004);
CHECK_OFFSET(cUIListBox, mVtbl, 0x008);
CHECK_OFFSET(cUIListBox, mpAnim, 0x00C);
CHECK_OFFSET(cUIListBox, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIListBox, unk_011, 0x011);
CHECK_OFFSET(cUIListBox, mAttr, 0x014);
CHECK_OFFSET(cUIListBox, mMiscData, 0x018);
CHECK_OFFSET(cUIListBox, mColour, 0x01C);
CHECK_OFFSET(cUIListBox, mAngle, 0x02C);
CHECK_OFFSET(cUIListBox, mHashName, 0x038);
CHECK_OFFSET(cUIListBox, mpEvents, 0x03C);
CHECK_OFFSET(cUIListBox, mpParent, 0x040);
CHECK_OFFSET(cUIListBox, mPosition, 0x044);
CHECK_OFFSET(cUIListBox, mScale, 0x050);
CHECK_OFFSET(cUIListBox, mpScreen, 0x05C);
CHECK_OFFSET(cUIListBox, mSize, 0x060);
CHECK_OFFSET(cUIListBox, mSound, 0x06C);
CHECK_OFFSET(cUIListBox, mAttrListBox, 0x074);
CHECK_OFFSET(cUIListBox, mShadowColour, 0x078);
CHECK_OFFSET(cUIListBox, mStaticColour, 0x088);
CHECK_OFFSET(cUIListBox, mpEntries, 0x098);
CHECK_OFFSET(cUIListBox, mNumEntries, 0x318);
CHECK_OFFSET(cUIListBox, mCurrentEntry, 0x319);
CHECK_OFFSET(cUIListBox, unk_31A, 0x31A);
CHECK_SIZE(cUIListBox, 0x31C);

#endif /* UI_UILISTBOX_H */
