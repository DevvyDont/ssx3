#ifndef UI_UIPROGRESS_H
#define UI_UIPROGRESS_H

// cUIProgress: a progress bar (record type 0x19): a background object and a knob moved along it by value / number of
// values. Size 0x88: cUIScreen_createObjectByStruct does operator new(0x88, "UIProgress") + the cUIThing ctor
// func_0039FB30, zeroes 0x74..0x84, stores cUIProgress_vtbl (D_00494408) and adopts the two children the record names
// (rec+0x20 -> mpObjBackground, rec+0x24 -> mpObjKnob). Default ctors func_003A60E0 / cUIProgress_cUIProgress (ui/uivector).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source: d:/ssx/code/source/ui/
// uiprogress.cpp (preview assert path); retail keeps its methods in ui/uimenu (cUIProgress__cUIProgress ..
// cUIProgress_setValue) and its inline accessors in ui/uivector (func_003A6170..func_003A6188).
// Methods: vtable slots 0 ~cUIProgress, 9 setUIData, 16 render2D, 18 update, 21 getType; cUIProgress_setValue setValue(int).
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUIProgress 0xA0; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

struct cUIProgress {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIProgress_vtbl D_00494408 (createObjectByStruct, func_003A60E0, cUIProgress_cUIProgress, the dtor)
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
    // --- cUIProgress
    cUIThing* mpObjBackground;       // 0x074 [O]; [R] = 0, then the record's child (hash rec+0x20); [U] func_003A6170
                                     //       setBackground (sw); the dtor deletes it; render2D, update, cUIProgress_setValue
    cUIThing* mpObjKnob;             // 0x078 [O]; [R] = 0, then the child named at rec+0x24; [U] func_003A6178 setKnob;
                                     //       the dtor deletes it; update, cUIProgress_setValue (setValue) moves it
    int mValue;                      // 0x07C [O]; [R] = 0; [U] func_003A6180 getValue (lw), cUIProgress_setValue setValue (sw)
    int mNumValues;                  // 0x080 [O]; [R] = 0; [U] cUIProgress_setUIData (sw), func_003A6188 setNumValues
    int mBackWidth;                  // 0x084 [O]; [R] = 0; [U] cUIProgress_setUIData (sw); cUIProgress_setValue
};

CHECK_OFFSET(cUIProgress, mpPrev, 0x000);
CHECK_OFFSET(cUIProgress, mpNext, 0x004);
CHECK_OFFSET(cUIProgress, mVtbl, 0x008);
CHECK_OFFSET(cUIProgress, mpAnim, 0x00C);
CHECK_OFFSET(cUIProgress, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIProgress, unk_011, 0x011);
CHECK_OFFSET(cUIProgress, mAttr, 0x014);
CHECK_OFFSET(cUIProgress, mMiscData, 0x018);
CHECK_OFFSET(cUIProgress, mColour, 0x01C);
CHECK_OFFSET(cUIProgress, mAngle, 0x02C);
CHECK_OFFSET(cUIProgress, mHashName, 0x038);
CHECK_OFFSET(cUIProgress, mpEvents, 0x03C);
CHECK_OFFSET(cUIProgress, mpParent, 0x040);
CHECK_OFFSET(cUIProgress, mPosition, 0x044);
CHECK_OFFSET(cUIProgress, mScale, 0x050);
CHECK_OFFSET(cUIProgress, mpScreen, 0x05C);
CHECK_OFFSET(cUIProgress, mSize, 0x060);
CHECK_OFFSET(cUIProgress, mSound, 0x06C);
CHECK_OFFSET(cUIProgress, mpObjBackground, 0x074);
CHECK_OFFSET(cUIProgress, mpObjKnob, 0x078);
CHECK_OFFSET(cUIProgress, mValue, 0x07C);
CHECK_OFFSET(cUIProgress, mNumValues, 0x080);
CHECK_OFFSET(cUIProgress, mBackWidth, 0x084);
CHECK_SIZE(cUIProgress, 0x88);

#endif /* UI_UIPROGRESS_H */
