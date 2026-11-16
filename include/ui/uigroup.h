#ifndef UI_UIGROUP_H
#define UI_UIGROUP_H

// cUIGroup: a UI object that owns child objects (record type 0x10; also the root group of every cUIScreen and the base
// of cUIMenu). Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "cUIGroup") + the cUIThing ctor
// func_0039FB30, stores cUIGroup_vtbl (D_004949E8) and constructs mList (func_003977E8 on this + 0x74); the default
// ctors func_003A4FE0 / func_003A5028 (ui/uivector) do the same. Base: cUIThing (ui/uithing.h, 0x74) <- cListNode,
// written out flat below.
// Methods: vtable slots 0 ~cUIGroup (cUIGroup__cUIGroup), 1-2 traverseFlat/traverseRecurse, 6 setFocus, 9 setUIData,
// 13 getObjectByHashName, 14 onInput, 15 updateLocale, 16 render2D, 17 render3D, 18 update (ui/uiengine
// 0x398618-0x398998), 21 getType; the list wrappers func_003A5100..func_003A5180 (ui/uivector). cUIMenu (ui/uimenu.h)
// derives from it and reuses most of these slots.
// EA's source file is not known (On Tour: ui/all_in_one_ui.cpp); ui/uigroup.h is our name.
//
// A FLAT layout, like main/game.h. Member: SSX On Tour's DWARF (class cUIGroup 0xA0: cList mList at 0x80 behind its
// 0x80-byte cUIThing).

#include "layout.h"
#include "ui/uithing.h"
#include "util/list.h"

struct cUIGroup {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74), copied field for field
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] cUIGroup_vtbl D_004949E8 (createObjectByStruct, func_003A4FE0, func_003A5028, the dtor;
                                     //       cUIMenu / cUIScreen dtors restore it)
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
    // --- cUIGroup
    cList mList;                     // 0x074 [O] the children (util/list.h, 0x1C); [R] ctor func_003977E8 on this + 0x74
                                     //       (createObjectByStruct, func_003A4FE0, func_003A5028), dtor cUIGroup__cUIGroup
                                     //       (cList_destroyList on this + 0x74, vptrs 0x8C / 0x88 / 0x7C);
                                     //       [U] func_003A5100..func_003A5180 (first/last/getByIndex/addToFront/addToEnd
                                     //       on this + 0x74), every cUIGroup_* walker
};

CHECK_OFFSET(cUIGroup, mpPrev, 0x000);
CHECK_OFFSET(cUIGroup, mpNext, 0x004);
CHECK_OFFSET(cUIGroup, mVtbl, 0x008);
CHECK_OFFSET(cUIGroup, mpAnim, 0x00C);
CHECK_OFFSET(cUIGroup, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIGroup, unk_011, 0x011);
CHECK_OFFSET(cUIGroup, mAttr, 0x014);
CHECK_OFFSET(cUIGroup, mMiscData, 0x018);
CHECK_OFFSET(cUIGroup, mColour, 0x01C);
CHECK_OFFSET(cUIGroup, mAngle, 0x02C);
CHECK_OFFSET(cUIGroup, mHashName, 0x038);
CHECK_OFFSET(cUIGroup, mpEvents, 0x03C);
CHECK_OFFSET(cUIGroup, mpParent, 0x040);
CHECK_OFFSET(cUIGroup, mPosition, 0x044);
CHECK_OFFSET(cUIGroup, mScale, 0x050);
CHECK_OFFSET(cUIGroup, mpScreen, 0x05C);
CHECK_OFFSET(cUIGroup, mSize, 0x060);
CHECK_OFFSET(cUIGroup, mSound, 0x06C);
CHECK_OFFSET(cUIGroup, mList, 0x074);
CHECK_SIZE(cUIGroup, 0x90);

#endif /* UI_UIGROUP_H */
