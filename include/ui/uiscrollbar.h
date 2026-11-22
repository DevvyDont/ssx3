#ifndef UI_UISCROLLBAR_H
#define UI_UISCROLLBAR_H

// cUIScrollBar: a scroll bar (record type 0x21): background, knob, up/down arrows and an object hidden when the list
// is empty; cUITextScroll points at one (mpScrollBar). Size 0xCC: cUIScreen_createObjectByStruct does
// operator new(0xCC, "UIScroll"...) + the cUIThing ctor cUIThing_cUIThing, zeroes 0x74..0x88 and 0xC0..0xC8, clears the
// two animation names, stores its vtable cUIScrollBar_vtbl and adopts the children named at rec+0x20 / rec+0x24.
// Default ctors cUIScrollBar_cUIScrollBar1 / cUIScrollBar_cUIScrollBar (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode,
// written out flat below. Class name: SSX On Tour's DWARF (class cUIScrollBar 0xE0, the only On Tour widget with these
// members); the preview keeps no name for it. EA's source file is not known; ui/uiscrollbar.h is our name.
// Methods: vtable cUIScrollBar_vtbl slots 0 ~cUIScrollBar (cUIScrollBar__cUIScrollBar), 9 setUIData (cUIScrollBar_setUIData), 16 render2D
// (cUIScrollBar_render2D), 21 getType (cUIScrollBar_getType); cUIScrollBar_setValues (update position: pos, visible, total), cUIScrollBar_setScrollVisibile
// (show/hide arrows); accessors cUIScrollBar_setBackgroundObject..cUIScrollBar_getValue (ui/uivector). Retail methods: ui/uiscreen 0x39D8C0-0x39DF28.
//
// A FLAT layout, like main/game.h. Members: On Tour's (each 0xC later there), placed by [R] the ctors and [U] the
// accessors, which follow On Tour's member order.

#include "layout.h"
#include "ui/uithing.h"

struct cUIScrollBar {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIScrollBar_vtbl (createObjectByStruct, cUIScrollBar_cUIScrollBar1, cUIScrollBar_cUIScrollBar, the dtor cUIScrollBar__cUIScrollBar)
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
                                     //       [U] cUIThing_getAbsPosition sums mPosition up the parent chain
    t3Vector mPosition;              // 0x044 [P] getPosition/setPosition (retail cUIThing_getPosition/cUIThing_setPosition); [U] channels 0-1
    t3Vector mScale;                 // 0x050 [P] getScale/setScale (retail cUIThing_getScale/cUIThing_setScale); [R] = {1,1,1}; [U] channels 9-10 (x 0.01)
    cUIScreen* mpScreen;             // 0x05C [P] getScreen (preview lw 0x68; retail cUIThing_getScreen lw 0x5C); [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] getSize/setSize (preview 0x5C; retail cUIThing_getSize/setSize, vtable slots 20/21);
                                     //       [U] channels 6-7; cUIText wraps at mSize.x
    int mSound[2];                   // 0x06C [O] (On Tour: int mSound[2]); [R] = {4, 6}; [U] cUIThing_getSound/cUIThing_setSound get/set by index
    // --- cUIScrollBar
    cUIThing* mpObjBackground;       // 0x074 [O]; [R] = 0, then the record's child (hash rec+0x20); [U] cUIScrollBar_setBackgroundObject (sw);
                                     //       the dtor cUIScrollBar__cUIScrollBar deletes it; render2D cUIScrollBar_render2D; cUIScrollBar_setValues
    cUIThing* mpObjKnob;             // 0x078 [O]; [R] = 0, then the child named at rec+0x24; [U] cUIScrollBar_setKnobObject; the dtor
                                     //       deletes it; cUIScrollBar_setValues / cUIScrollBar_setScrollVisibile move it
    cUIThing* mpUpArrow;             // 0x07C [O]; [R] = 0; [U] cUIScrollBar_setUpArrowObject (sw); cUIScrollBar_setScrollVisibile
    cUIThing* mpDownArrow;           // 0x080 [O]; [R] = 0; [U] cUIScrollBar_setDownArrowObject (sw); cUIScrollBar_setScrollVisibile
    cUIThing* mpHideOnEmpty;         // 0x084 [O]; [R] = 0; [U] cUIScrollBar_setHideOnEmpty (sw); cUIScrollBar_setScrollVisibile
    int meHideState;                 // 0x088 [O] eScrollBarHide; [R] = 0; [U] cUIScrollBar_setHideState (sw)
    char mszUpAnimation[25];         // 0x08C [O]; [R] [0] = 0 (sb); [U] cUIScrollBar_setUpAnimationName strcpy()s the name in; cUIScrollBar_setValues
                                     //       plays the frame it labels
    char mszDownAnimation[25];       // 0x0A5 [O]; [R] [0] = 0 (sb); [U] cUIScrollBar_setDownAnimationName strcpy; cUIScrollBar_setValues
    char unk_0BE[0x2];               // 0x0BE padding
    int mValue;                      // 0x0C0 [O]; [R] = 0; [U] cUIScrollBar_getValue getValue (lw); cUIScrollBar_setValues stores the position
    int mNumValues;                  // 0x0C4 [O]; [R] = 0; [U] cUIScrollBar_setValues (sw)
    int mPage;                       // 0x0C8 [O]; [R] = 0; [U] cUIScrollBar_setValues (sw)
};

CHECK_OFFSET(cUIScrollBar, mpPrev, 0x000);
CHECK_OFFSET(cUIScrollBar, mpNext, 0x004);
CHECK_OFFSET(cUIScrollBar, mVtbl, 0x008);
CHECK_OFFSET(cUIScrollBar, mpAnim, 0x00C);
CHECK_OFFSET(cUIScrollBar, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIScrollBar, unk_011, 0x011);
CHECK_OFFSET(cUIScrollBar, mAttr, 0x014);
CHECK_OFFSET(cUIScrollBar, mMiscData, 0x018);
CHECK_OFFSET(cUIScrollBar, mColour, 0x01C);
CHECK_OFFSET(cUIScrollBar, mAngle, 0x02C);
CHECK_OFFSET(cUIScrollBar, mHashName, 0x038);
CHECK_OFFSET(cUIScrollBar, mpEvents, 0x03C);
CHECK_OFFSET(cUIScrollBar, mpParent, 0x040);
CHECK_OFFSET(cUIScrollBar, mPosition, 0x044);
CHECK_OFFSET(cUIScrollBar, mScale, 0x050);
CHECK_OFFSET(cUIScrollBar, mpScreen, 0x05C);
CHECK_OFFSET(cUIScrollBar, mSize, 0x060);
CHECK_OFFSET(cUIScrollBar, mSound, 0x06C);
CHECK_OFFSET(cUIScrollBar, mpObjBackground, 0x074);
CHECK_OFFSET(cUIScrollBar, mpObjKnob, 0x078);
CHECK_OFFSET(cUIScrollBar, mpUpArrow, 0x07C);
CHECK_OFFSET(cUIScrollBar, mpDownArrow, 0x080);
CHECK_OFFSET(cUIScrollBar, mpHideOnEmpty, 0x084);
CHECK_OFFSET(cUIScrollBar, meHideState, 0x088);
CHECK_OFFSET(cUIScrollBar, mszUpAnimation, 0x08C);
CHECK_OFFSET(cUIScrollBar, mszDownAnimation, 0x0A5);
CHECK_OFFSET(cUIScrollBar, unk_0BE, 0x0BE);
CHECK_OFFSET(cUIScrollBar, mValue, 0x0C0);
CHECK_OFFSET(cUIScrollBar, mNumValues, 0x0C4);
CHECK_OFFSET(cUIScrollBar, mPage, 0x0C8);
CHECK_SIZE(cUIScrollBar, 0xCC);

#endif /* UI_UISCROLLBAR_H */
