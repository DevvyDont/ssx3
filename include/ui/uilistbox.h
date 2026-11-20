#ifndef UI_UILISTBOX_H
#define UI_UILISTBOX_H

// cUIListBox: a list of up to 32 text entries (ASCII, Unicode or string-table id, each with a user data pointer), one of
// them current (record type 0x12). Size 0x31C: cUIScreen_createObjectByStruct does operator new(0x31C, "UIListBox"...)
// + the ctor cUIListBox_cUIListBox (cUIThing ctor cUIThing_cUIThing, vtable cUIListBox_vtbl, clears the entries, mNumEntries,
// mCurrentEntry, mStaticColour = D_004C66C8); default ctor cUIListBox_cUIListBox1 (ui/uivector). Base: cUIThing (ui/uithing.h,
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
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUIListBox 0x340: its cUIThing is 0x80 and it
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

struct cUIListBox {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIListBox_vtbl (the ctor cUIListBox_cUIListBox, cUIListBox_cUIListBox1, the dtor cUIListBox__cUIListBox)
    cUIAnimation* mpAnim;            // 0x00C [P] getAnimation/isPlayingAnimation (lw 0xC; retail cUIThing_getAnimation/cUIThing_isPlayingAnimation);
                                     //       [U] cUIThing_playAnimation stores it, cUIThing_deleteAnimation deletes it
    unsigned char mAnimPlaybackStatus; // 0x010 [O]; [R] = 0; [U] cUIThing_playAnimation stores the playback mode, cUIThing_update passes it on
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] tAttrThing: isActive..isWrap, get/setDrawLayer, get/setPivot (lw/sw 0x14;
                                     //       retail cUIThing_isHorizontal..cUIThing_getPivot, cUIThing_setActive..setVisible); [R] ctor sets it
    int mMiscData;                   // 0x018 [P] getMiscData/setMiscData (lw/sw 0x18; retail cUIThing_getMiscData/cUIThing_setMiscData); [R] = 0
    tARGB mColour;                   // 0x01C [P] setColour/getColour (0x1C; retail cUIThing_setColour, cUIThing_getColour/cUIThing_getColour1);
                                     //       [R] = {1,1,1,1}; [U] keyframe channels 13-16 (x 1/255) in cUIThing_setChannelData
    t3Vector mAngle;                 // 0x02C [O]; [P] getRadAngle/setRadAngle = mAngle.z (lwc1/swc1 0x34; retail cUIThing_getRadAngle/cUIThing_setRadAngle);
                                     //       [R] = t3vect_Zero; [U] channels 3-5 (degrees -> radians)
    int mHashName;                   // 0x038 [P] getHashName/setHashName (lw/sw 0x38; retail cUIThing_getHashName/cUIThing_setHashName);
                                     //       [U] compared by cUIThing_getObjectByHashName; set from the object record (cUIScreen_createAllObjects)
    unsigned short* mpEvents;        // 0x03C [O]; [R] = 0; [U] keyframer event per index, 0xFFFF = none (cUIThing_getKeyframerEvent);
                                     //       cUIThing_setUIData points it after the object record
    cUIThing* mpParent;              // 0x040 [P] getParent/setParent (lw/sw 0x40; retail cUIThing_getParent/cUIThing_setParent); [R] = pParent;
                                     //       [U] func_003A0290 sums mPosition up the parent chain
    t3Vector mPosition;              // 0x044 [P] getPosition/setPosition (retail cUIThing_getPosition/cUIThing_setPosition); [U] channels 0-1
    t3Vector mScale;                 // 0x050 [P] getScale/setScale (retail cUIThing_getScale/cUIThing_setScale); [R] = {1,1,1}; [U] channels 9-10 (x 0.01)
    cUIScreen* mpScreen;             // 0x05C [P] getScreen (preview lw 0x68; retail cUIThing_getScreen lw 0x5C); [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] getSize/setSize (preview 0x5C; retail cUIThing_getSize/setSize, vtable slots 20/21);
                                     //       [U] channels 6-7; cUIText wraps at mSize.x
    int mSound[2];                   // 0x06C [O] (On Tour: int mSound[2]); [R] = {4, 6}; [U] cUIThing_getSound/cUIThing_setSound get/set by index
    // --- cUIListBox
    tAttrListBox mAttrListBox;       // 0x074 [O] tAttrListBox; [R] the ctor cUIListBox_cUIListBox sets mShadow, clears mUnicode /
                                     //       mCallback (lbu/sb 0x74), cUIListBox_cUIListBox1 calls its ctor tAttrListBox_tAttrListBox; [U] setUIData
                                     //       cUIListBox_setUIData (lw/sw), accessors cUIListBox_setFontIndex..cUIListBox_setUseStaticColour (lw/sw)
    tARGB mShadowColour;             // 0x078 [O]; [U] setUIData cUIListBox_setUIData copies it from the record; cUIListBox_setShadowColour
                                     //       setShadowColour (ldl/ldr); render2D
    tARGB mStaticColour;             // 0x088 [O]; [R] = D_004C66C8 (the ctor); [U] setUIData; cUIListBox_setStaticColour setStaticColour
    tListBoxEntry mpEntries[32];     // 0x098 [O]; [R] the ctor clears all 32 (sw/sb loop + memset(this + 0x98, 0, 640));
                                     //       [U] cUIListBox_addEntryBy*, cUIListBox_setEntryByAsciiString, cUIListBox_deleteAllEntries (clear),
                                     //       cUIListBox_getCurrentAsciiText..cUIListBox_setEntryDataByIndex, updateLocale cUIListBox_updateLocale, render2D
    unsigned char mNumEntries;       // 0x318 [O]; [R] = 0; [U] lbu/sb in the add/clear functions; cUIListBox_getNumEntries (lbu)
    unsigned char mCurrentEntry;     // 0x319 [O]; [R] = 0; [U] onInput cUIListBox_onInput (lbu/sb), cUIListBox_setCurrentEntryByIndex setCurrent;
                                     //       cUIListBox_getCurrentEntryIndex getCurrentEntryIndex (lbu)
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
