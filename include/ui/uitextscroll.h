#ifndef UI_UITEXTSCROLL_H
#define UI_UITEXTSCROLL_H

// cUITextScroll: a scrolling multi-line text box (record type 0x20), word-wrapped into lines and optionally driven by
// a cUIScrollBar. Size 0xD0: cUIScreen_createObjectByStruct does operator new(0xD0, D_00493F50) + the ctor
// func_003A1148 (cUIThing ctor func_0039FB30, vtable D_00494278, mList ctor func_003977E8, zeroes 0xB8..0xCC,
// func_003A1310 fills mInputMap), then moves the record's children into mList. Default ctor func_003A6250
// (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source:
// d:/ssx/code/source/ui/uitextscroll.cpp (preview assert path); retail keeps its methods in ui/uitext
// (0x3A1148-0x3A2068) and its inline accessors in ui/uivector (0x3A6250-0x3A64F8).
// Methods: vtable D_00494278 slots 0 ~cUITextScroll (func_003A62A0), 9 setUIData (func_003A11B8), 14 onInput
// (func_003A13E0), 15 updateLocale (func_003A1360), 16 render2D (func_003A1588), 21 getType (func_003A6330); line
// buffer helpers func_003A12D0, func_003A18C0 (free lines), cUITextScroll_allocLine, func_003A1F90; text setters
// func_003A19F8 / func_003A1C80 (ASCII), cUITextScroll_addUnicodeString, func_003A1CF0 / func_003A1D30 (Unicode),
// func_003A1F18 (string id).
// Class name: the preview's cUITextScroll::getType (builds/x/v2/name_map_v2.tsv, func_003A6330) and On Tour.
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUITextScroll 0xF0: its cUIThing is 0x80 and
// it aligns tARGB to 16, so the members sit 0xC-0x20 later there; same order and types).

#include "layout.h"
#include "ui/uithing.h"
#include "util/list.h"

// EA's tAttrTextScroll (On Tour: 1 byte). Retail's accessors read and write it as a word (lw/sw 0x74); the ctor
// func_003A1148 clears bits 0/3/4/5 as a byte (lbu, andi 0xC6, sb).
struct tAttrTextScroll {
    unsigned int mMode : 1;          // bit 0 [O]; [U] func_003A6480 (set), func_003A6490 (get)
    unsigned int mFont : 2;          // bits 1-2 [O]; [U] func_003A6338 setFont
    unsigned int mShadow : 1;        // bit 3 [O]; [U] func_003A6358 (get), func_003A6378 (set)
    unsigned int mUnicode : 1;       // bit 4 [O]; [U] func_003A6368 (get), func_003A6398 (set)
    unsigned int mStaticColour : 1;  // bit 5 [O]; [R] the ctor clears it
};

CHECK_SIZE(tAttrTextScroll, 0x4);

struct cUITextScroll {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] D_00494278 (the ctor func_003A1148, func_003A6250, the dtor func_003A62A0)
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
    // --- cUITextScroll
    tAttrTextScroll mAttrTextScroll; // 0x074 [O]; [R] ctor func_003A1148 clears mMode/mShadow/mUnicode/mStaticColour
                                     //       (lbu/andi 0xC6/sb 0x74), func_003A6250 calls its ctor func_003A6600; [U] setUIData
                                     //       func_003A11B8 (lw/sw), accessors func_003A6338..func_003A6398, func_003A6480/90
    tARGB mShadowColour;             // 0x078 [O]; [U] setUIData func_003A11B8 copies it (ldl/ldr); func_003A63B8 (set)
    char mInputMap[4];               // 0x088 [O]; [R] func_003A1310 (called by the ctor) fills the four bytes; [U] onInput
                                     //       func_003A13E0 (lb 0x88 / 0x89)
    cList mList;                     // 0x08C [O] the text lines' child objects (util/list.h); [R] ctor func_003977E8 on
                                     //       this + 0x8C, dtor func_003A62A0 (cList_destroyList, vptrs 0xA4 / 0xA0 / 0x94);
                                     //       createObjectByStruct adds the record's children (cList_addToEnd on o + 0x8C);
                                     //       [U] func_003A63E0..func_003A6460 (first/last/getByIndex/addToFront/addToEnd)
    tARGB mSelectedColour;           // 0x0A8 [O]; [U] func_003A64A0 (set, ldl/ldr), render2D func_003A1588
    cUIThing* mpScrollBar;           // 0x0B8 [O] cUIScrollBar*; [R] = 0; [U] func_003A64C8 setScrollBar (sw); render2D
    unsigned short* mpLineOffset;    // 0x0BC [O] line start offsets (allocated); [R] = 0; [U] cUITextScroll_allocLine,
                                     //       func_003A18C0 (free), func_003A1F90, cUITextScroll_addUnicodeString, render2D
    unsigned int mStringID;          // 0x0C0 [O]; [R] = 0; [U] func_003A1C80 / func_003A1F18 (sw), func_003A1360 (lw)
    unsigned short mSelectedIndex;   // 0x0C4 [O]; [R] = 0 (sh); [U] func_003A64E8 (sh), func_003A64F0 (lhu), onInput
    unsigned short mLinesOffset;     // 0x0C6 [O]; [R] = 0; [U] func_003A64D0 (sh), func_003A64D8 (lhu), onInput, render2D
    unsigned short mLineSize;        // 0x0C8 [O] line count; [R] = 0; [U] func_003A64E0 (lhu), allocLine / func_003A18C0
    char unk_0CA[0x2];               // 0x0CA padding
    float mPixelOffset;              // 0x0CC [O] float; [R] = 0 (sw zero); [U] render2D func_003A1588 (lwc1 $f13 = y offset)
};

CHECK_OFFSET(cUITextScroll, mpPrev, 0x000);
CHECK_OFFSET(cUITextScroll, mpNext, 0x004);
CHECK_OFFSET(cUITextScroll, mVtbl, 0x008);
CHECK_OFFSET(cUITextScroll, mpAnim, 0x00C);
CHECK_OFFSET(cUITextScroll, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUITextScroll, unk_011, 0x011);
CHECK_OFFSET(cUITextScroll, mAttr, 0x014);
CHECK_OFFSET(cUITextScroll, mMiscData, 0x018);
CHECK_OFFSET(cUITextScroll, mColour, 0x01C);
CHECK_OFFSET(cUITextScroll, mAngle, 0x02C);
CHECK_OFFSET(cUITextScroll, mHashName, 0x038);
CHECK_OFFSET(cUITextScroll, mpEvents, 0x03C);
CHECK_OFFSET(cUITextScroll, mpParent, 0x040);
CHECK_OFFSET(cUITextScroll, mPosition, 0x044);
CHECK_OFFSET(cUITextScroll, mScale, 0x050);
CHECK_OFFSET(cUITextScroll, mpScreen, 0x05C);
CHECK_OFFSET(cUITextScroll, mSize, 0x060);
CHECK_OFFSET(cUITextScroll, mSound, 0x06C);
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
