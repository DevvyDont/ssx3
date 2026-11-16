#ifndef UI_UIICON_H
#define UI_UIICON_H

// cUIIcon: a UI object drawn as a textured sprite (record type 0x11). Size 0x88: cUIScreen_createObjectByStruct does
// operator new(0x88, "UIIcon") + the cUIThing ctor func_0039FB30, stores cUIIcon_vtbl (D_00494AA8), constructs mAttrIcon
// (func_0021CCC8 on this + 0x74), mTextureID = -1, mpTextureElem = 0. Default ctors func_003A4DF0 / func_003A4E38
// (ui/uivector) do the same. Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below.
// Methods: vtable slots 0 ~cUIIcon (cUIIcon__cUIIcon__FPv), 9 setUIData (cUIIcon_setUIData, ui/uiengine), 16 render2D
// (cUIIcon_render2D = func_00398A78, ui/uiengine), 21 getType; out-of-line inline accessors in ui/uivector
// 0x3A4EC8-0x3A4FB8; func_00399730 (ui/uiengine) looks a texture element up by id.
// EA's source file is not known (no uiicon.cpp in the preview's assert paths; On Tour: ui/all_in_one_ui.cpp).
//
// A FLAT layout, like main/game.h. Members: SSX On Tour's DWARF (class cUIIcon 0xA0: its cUIThing is 0x80, so each
// member sits 0xC later there), placed by [R] the constructors and [U] the accessors.
// mAttrIcon is EA's tAttrIcon (On Tour 1 byte: mIconTiled:1 mIconFlip:1 mIconRotate:2); retail reads and writes it as a
// word (lw/sw 0x74: isTiled bit 0, isFlipped bit 1, getRotate bits 2-3).

#include "layout.h"
#include "ui/uithing.h"

struct cTextureElement;

// EA's tAttrIcon (On Tour: 1 byte). Retail accesses it as a 32-bit word, so the bitfield is unsigned int here.
struct tAttrIcon {
    unsigned int mIconTiled : 1;     // bit 0 [O]; [U] func_003A4EC8 (get), func_003A4ED8 (set)
    unsigned int mIconFlip : 1;      // bit 1 [O]; [U] func_003A4EF8 (get), func_003A4F08 (set)
    unsigned int mIconRotate : 2;    // bits 2-3 [O]; [U] func_003A4F28 (!= 0), func_003A4F38 (set)
};

CHECK_SIZE(tAttrIcon, 0x4);

struct cUIIcon {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIIcon_vtbl D_00494AA8 (createObjectByStruct, func_003A4DF0, func_003A4E38, the dtor)
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
    // --- cUIIcon
    tAttrIcon mAttrIcon;             // 0x074 [O] tAttrIcon; [R] its ctor func_0021CCC8 (cUIScreen_createObjectByStruct) /
                                     //       func_003A4EB0 clears the low nibble; [U] accessors func_003A4EC8..func_003A4F38
                                     //       (lw/sw 0x74), cUIIcon_setUIData stores the record flags
    int mTextureID;                  // 0x078 [O]; [R] = -1; [U] func_003A4F70 getTextureID (lw), func_003A4F78 setTextureID
                                     //       (sw, clears mpTextureElem), func_003A4F60 stores -1; render2D
    const cTextureElement* mpTextureElem; // 0x07C [O]; [R] = 0; [U] func_003A4F58 getElement (lw), func_003A4F60
                                     //       setElement (sw); func_00399730 / cUIIcon_setUIData look it up by id; render2D
    float mTileUV[2];                // 0x080 [O] t2Vector (x, y); [U] func_003A4F88 getTileUV, func_003A4FA0 / func_003A4FB8
                                     //       setTileUV (8-byte ldl/ldr copies); cUIIcon_setUIData; render2D
};

CHECK_OFFSET(cUIIcon, mpPrev, 0x000);
CHECK_OFFSET(cUIIcon, mpNext, 0x004);
CHECK_OFFSET(cUIIcon, mVtbl, 0x008);
CHECK_OFFSET(cUIIcon, mpAnim, 0x00C);
CHECK_OFFSET(cUIIcon, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIIcon, unk_011, 0x011);
CHECK_OFFSET(cUIIcon, mAttr, 0x014);
CHECK_OFFSET(cUIIcon, mMiscData, 0x018);
CHECK_OFFSET(cUIIcon, mColour, 0x01C);
CHECK_OFFSET(cUIIcon, mAngle, 0x02C);
CHECK_OFFSET(cUIIcon, mHashName, 0x038);
CHECK_OFFSET(cUIIcon, mpEvents, 0x03C);
CHECK_OFFSET(cUIIcon, mpParent, 0x040);
CHECK_OFFSET(cUIIcon, mPosition, 0x044);
CHECK_OFFSET(cUIIcon, mScale, 0x050);
CHECK_OFFSET(cUIIcon, mpScreen, 0x05C);
CHECK_OFFSET(cUIIcon, mSize, 0x060);
CHECK_OFFSET(cUIIcon, mSound, 0x06C);
CHECK_OFFSET(cUIIcon, mAttrIcon, 0x074);
CHECK_OFFSET(cUIIcon, mTextureID, 0x078);
CHECK_OFFSET(cUIIcon, mpTextureElem, 0x07C);
CHECK_OFFSET(cUIIcon, mTileUV, 0x080);
CHECK_SIZE(cUIIcon, 0x88);

#endif /* UI_UIICON_H */
