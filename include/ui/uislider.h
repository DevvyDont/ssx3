#ifndef UI_UISLIDER_H
#define UI_UISLIDER_H

// cUISlider: a slider widget (record type 0x16): a knob moved along a background by value / number of values, stepped
// by input. Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "UISlider") + the cUIThing ctor
// func_0039FB30, zeroes 0x74..0x80 and 0x88/0x8C, stores its vtable cUISlider_vtbl and adopts the children named at
// rec+0x24 (mpObjBackground) and rec+0x28 (mpObjKnob). Default ctors func_003A6190 / func_003A61D0 (ui/uivector).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source: ../source/ui/uislider.h
// (preview assert path); retail keeps its methods in ui/uiscreen (0x39DF28-0x39E288).
// Methods: vtable cUISlider_vtbl slots 0 ~cUISlider (cUISlider__cUISlider), 9 setUIData (cUISlider_setUIData), 14 onInput
// (cUISlider_onInput), 16 render2D (cUISlider_render2D), 21 getType (func_003A6220); accessors func_003A6228..func_003A6248.
// Class name: the preview's cUISlider::getType (builds/x/v2/name_map_v2.tsv, func_003A6220) and On Tour.
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUISlider 0xA0; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

struct cUISlider {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUISlider_vtbl (createObjectByStruct, func_003A6190, func_003A61D0, the dtor cUISlider__cUISlider)
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
    // --- cUISlider
    int mValue;                      // 0x074 [O]; [R] = 0; [U] func_003A6228 getValue (lw), func_003A6230 setValue (sw);
                                     //       onInput cUISlider_onInput steps it; render2D cUISlider_render2D (lwc1 + cvt.s.w)
    int mNumValues;                  // 0x078 [O]; [R] = 0; [U] setUIData cUISlider_setUIData (sw), func_003A6238 setNumValues;
                                     //       onInput, render2D (lwc1 + cvt.s.w)
    int mStartPixel;                 // 0x07C [O]; [R] = 0 (no other retail access found)
    int mBackWidth;                  // 0x080 [O]; [R] = 0; [U] setUIData cUISlider_setUIData (sw)
    cUIThing* mSliderObj;            // 0x084 [O] (not stored by the retail ctors; no retail access found)
    cUIThing* mpObjBackground;       // 0x088 [O]; [R] = 0, then the record's child (hash rec+0x24); [U] func_003A6240
                                     //       setBackground (sw); the dtor cUISlider__cUISlider deletes it; render2D
    cUIThing* mpObjKnob;             // 0x08C [O]; [R] = 0, then the child named at rec+0x28; [U] func_003A6248 setKnob;
                                     //       the dtor deletes it; render2D moves it
};

CHECK_OFFSET(cUISlider, mpPrev, 0x000);
CHECK_OFFSET(cUISlider, mpNext, 0x004);
CHECK_OFFSET(cUISlider, mVtbl, 0x008);
CHECK_OFFSET(cUISlider, mpAnim, 0x00C);
CHECK_OFFSET(cUISlider, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUISlider, unk_011, 0x011);
CHECK_OFFSET(cUISlider, mAttr, 0x014);
CHECK_OFFSET(cUISlider, mMiscData, 0x018);
CHECK_OFFSET(cUISlider, mColour, 0x01C);
CHECK_OFFSET(cUISlider, mAngle, 0x02C);
CHECK_OFFSET(cUISlider, mHashName, 0x038);
CHECK_OFFSET(cUISlider, mpEvents, 0x03C);
CHECK_OFFSET(cUISlider, mpParent, 0x040);
CHECK_OFFSET(cUISlider, mPosition, 0x044);
CHECK_OFFSET(cUISlider, mScale, 0x050);
CHECK_OFFSET(cUISlider, mpScreen, 0x05C);
CHECK_OFFSET(cUISlider, mSize, 0x060);
CHECK_OFFSET(cUISlider, mSound, 0x06C);
CHECK_OFFSET(cUISlider, mValue, 0x074);
CHECK_OFFSET(cUISlider, mNumValues, 0x078);
CHECK_OFFSET(cUISlider, mStartPixel, 0x07C);
CHECK_OFFSET(cUISlider, mBackWidth, 0x080);
CHECK_OFFSET(cUISlider, mSliderObj, 0x084);
CHECK_OFFSET(cUISlider, mpObjBackground, 0x088);
CHECK_OFFSET(cUISlider, mpObjKnob, 0x08C);
CHECK_SIZE(cUISlider, 0x90);

#endif /* UI_UISLIDER_H */
