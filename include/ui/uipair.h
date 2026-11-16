#ifndef UI_UIPAIR_H
#define UI_UIPAIR_H

// cUIPair: a UI object that pairs a label ("entry") with a widget (record type 0x15; menus use it for option rows).
// Size 0x80: cUIScreen_createObjectByStruct does operator new(0x80, "UIPair") + the cUIThing ctor cUIThing_cUIThing, stores
// cUIPair_vtbl (D_00494868), constructs mAttrPair (func_003A5508 on this + 0x74), sets cUIThing::mAttr bit 1, zeroes
// mpEntry / mpWidget, then adopts the two children the record names. Default ctors func_003A5420 / cUIPair_cUIPair
// (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source:
// d:/ssx/code/source/ui/uipair.cpp (preview assert path); retail keeps its methods in ui/uimenu (0x39BAB0-0x39C240).
// Methods: vtable slots 0 ~cUIPair (cUIPair__cUIPair), 1-2 traverseFlat/traverseRecurse, 3 setActive, 6 setFocus,
// 8 setVisible, 9 setUIData, 14 onInput, 15 updateLocale, 16 render2D, 18 update, 21 getType; cUIPair_destroyPair;
// accessors func_003A5528..cUIPair_setWidget (ui/uivector).
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUIPair 0x90; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

// EA's tAttrPair (On Tour: 2 bytes; unsigned char bitfields, each pivot in its own byte).
struct tAttrPair {
    unsigned char mEntryPivot : 6;   // byte 0 [O]; [R] cleared by func_003A5508 (lbu/sb 0)
    unsigned char mWidgetPivot : 6;  // byte 1 [O]; [R] cleared by func_003A5508 (lbu/sb 1)
};

CHECK_SIZE(tAttrPair, 0x2);

struct cUIPair {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIPair_vtbl D_00494868 (createObjectByStruct, func_003A5420, cUIPair_cUIPair, the dtor)
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
    t3Vector mPosition;              // 0x044 [P] getPosition/setPosition (retail cUIThing_getPosition/cUIThing_setPosition); [U] channels 0-1
    t3Vector mScale;                 // 0x050 [P] getScale/setScale (retail cUIThing_getScale/cUIThing_setScale); [R] = {1,1,1}; [U] channels 9-10 (x 0.01)
    cUIScreen* mpScreen;             // 0x05C [P] getScreen (preview lw 0x68; retail func_003A4C88 lw 0x5C); [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] getSize/setSize (preview 0x5C; retail cUIThing_getSize/setSize, vtable slots 20/21);
                                     //       [U] channels 6-7; cUIText wraps at mSize.x
    int mSound[2];                   // 0x06C [O] (On Tour: int mSound[2]); [R] = {4, 6}; [U] func_003A0318/func_003A0330 get/set by index
    // --- cUIPair
    tAttrPair mAttrPair;             // 0x074 [O] tAttrPair (2 bytes); [R] its ctor func_003A5508 on this + 0x74
                                     //       (createObjectByStruct, func_003A5420, cUIPair_cUIPair)
    char unk_076[0x2];               // 0x076 padding
    cUIThing* mpEntry;               // 0x078 [O]; [R] = 0, then the record's first child (hash rec+0x20);
                                     //       [U] func_003A5528 getEntry (lw), cUIPair_setEntry setEntry (deletes the old one);
                                     //       cUIPair_destroyPair, setActive/setVisible/setFocus, traverseRecurse, render2D, update
    cUIThing* mpWidget;              // 0x07C [O]; [R] = 0, then the second child (its mAttr bit 0 cleared);
                                     //       [U] func_003A5580 getWidget (lw), cUIPair_setWidget setWidget; the same methods
};

CHECK_OFFSET(cUIPair, mpPrev, 0x000);
CHECK_OFFSET(cUIPair, mpNext, 0x004);
CHECK_OFFSET(cUIPair, mVtbl, 0x008);
CHECK_OFFSET(cUIPair, mpAnim, 0x00C);
CHECK_OFFSET(cUIPair, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIPair, unk_011, 0x011);
CHECK_OFFSET(cUIPair, mAttr, 0x014);
CHECK_OFFSET(cUIPair, mMiscData, 0x018);
CHECK_OFFSET(cUIPair, mColour, 0x01C);
CHECK_OFFSET(cUIPair, mAngle, 0x02C);
CHECK_OFFSET(cUIPair, mHashName, 0x038);
CHECK_OFFSET(cUIPair, mpEvents, 0x03C);
CHECK_OFFSET(cUIPair, mpParent, 0x040);
CHECK_OFFSET(cUIPair, mPosition, 0x044);
CHECK_OFFSET(cUIPair, mScale, 0x050);
CHECK_OFFSET(cUIPair, mpScreen, 0x05C);
CHECK_OFFSET(cUIPair, mSize, 0x060);
CHECK_OFFSET(cUIPair, mSound, 0x06C);
CHECK_OFFSET(cUIPair, mAttrPair, 0x074);
CHECK_OFFSET(cUIPair, unk_076, 0x076);
CHECK_OFFSET(cUIPair, mpEntry, 0x078);
CHECK_OFFSET(cUIPair, mpWidget, 0x07C);
CHECK_SIZE(cUIPair, 0x80);

#endif /* UI_UIPAIR_H */
