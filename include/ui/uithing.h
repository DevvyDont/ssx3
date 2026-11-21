#ifndef UI_UITHING_H
#define UI_UITHING_H

// cUIThing: the base of every object on a UI screen (cUIGroup, cUIText, cUIIcon, cUIMenu, cUIPair, cUIListBox,
// cUIProgress, cUIScroll, cUISlider, cUITextScroll, cUIVector). cUIScreen_createObjectByStruct builds each kind by
// calling the cUIThing constructor cUIThing_cUIThing(this, pScreen, pParent) and then storing the derived vtable; derived
// members all start at 0x74 (cUIGroup = 0x74 + a 0x1C cList = 0x90 bytes, tag "cUIGroup"), so cUIThing is 0x74 bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Base class: cListNode (On Tour: mpPrev 0x0, mpNext 0x4; its vptr follows at 0x8). cUIThing's vtable
// _vt$8cUIThing (cUIThing_vtbl) keeps cListNode's traverseFlat/traverseRecurse (cListNode_traverseFlat/cListNode_traverseRecurse) in
// slots 2-3. Objects are linked into their group's cList through mpPrev/mpNext.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIThing), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (getAnimation lw 0xC ... getScreen lw 0x68). Retail keeps their
//       out-of-line copies in the same order as the preview (cUIThing_isPlayingAnimation isPlayingAnimation ... func_003A4CB0 setSize,
//       in ui/uivector), which places each one: unshifted up to mScale, then mpScreen moved in front of mSize;
//   [R] retail's constructor cUIThing_cUIThing (stores every field; pScreen -> 0x5C, pParent -> 0x40);
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail = On Tour - 4 from mColour on (On Tour aligns tARGB to 16), and has no field after mSound.
//
// The attribute word mAttr is EA's tAttrThing bitfield (On Tour, LSB first): mOrientation:1 mActive:1 mCached:1
// mEnabled:1 mFocused:1 mStatic:1 mVisible:1 mWrap:1 mLayer:5 mPivot:6 (struct tAttrThing below). Retail code reads and
// writes it as a whole word (lw/sw 0x14), so it is an int here.

#include "layout.h"

struct cUIAnimation;
struct cUIScreen;
struct cListNode;

// EA's 3-float vector (type name from the preview's setSize__8cUIThingRC8t3Vector). 12 bytes, 4-byte aligned: retail
// copies it with ldl/ldr + lw. Member names are our guess (On Tour: x()/y()/z() accessors over an anonymous union).
struct t3Vector {
    float x, y, z;
};

// EA's colour (type name from the preview's setColour__8cUIThingRC5tARGB). 16 bytes, only 4-byte aligned on the PS2
// (it sits at cUIThing+0x1C; copied with two ldl/ldr pairs). Members from On Tour's DWARF (struct tARGB: A R G B).
struct tARGB {
    float A, R, G, B;
};

// EA's tAttrThing, the type of cUIThing::mAttr (On Tour struct tAttrThing, size 0x4, LSB first). cUIThing keeps mAttr
// an int (every accessor uses lw/sw 0x14); this type names the bits for code that reads one field, e.g.
// ((tAttrThing*)&thing->mAttr)->mPivot. [R] tAttrThing_tAttrThing (the ctor: sets bits 0, 1, 3, 6, clears 2, 4, 5, 7,
// mLayer = 0, mPivot = 9); [P] the inline accessors isActive .. isWrap, get/setDrawLayer, get/setPivot.
struct tAttrThing {
    unsigned int mOrientation : 1;   // bit 0 [O]; [P] isHorizontal
    unsigned int mActive : 1;        // bit 1 [O]; [P] isActive / setActive
    unsigned int mCached : 1;        // bit 2 [O]; [P] isCached / setCached (vtable slot 5)
    unsigned int mEnabled : 1;       // bit 3 [O]; [P] isEnabled / setEnabled (slot 6)
    unsigned int mFocused : 1;       // bit 4 [O]; [P] isFocused / setFocus (slot 7); [U] cUIMenu_setFocus sets bit 4
    unsigned int mStatic : 1;        // bit 5 [O]; [P] isStatic / setStatic (slot 8)
    unsigned int mVisible : 1;       // bit 6 [O]; [P] isVisible / setVisible (slot 9)
    unsigned int mWrap : 1;          // bit 7 [O]; [P] isWrap
    unsigned int mLayer : 5;         // bits 8-12 [O]; [P] getDrawLayer / setDrawLayer
    unsigned int mPivot : 6;         // bits 13-18 [O]; [P] getPivot / setPivot; [U] cScreenPopup_changePivot and
                                     //       cUIVector_setChannelDataKCP channel 12 write bits 13-18
};

struct cUIThing {
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] walked by the cList code
    void** mVtbl;                    // 0x008 [R] _vt$8cUIThing (cUIThing_vtbl), stored by cUIThing_cUIThing and cUIThing_cUIThing1
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
};

CHECK_SIZE(tAttrThing, 0x4);

CHECK_OFFSET(cUIThing, mpPrev, 0x000);
CHECK_OFFSET(cUIThing, mpNext, 0x004);
CHECK_OFFSET(cUIThing, mVtbl, 0x008);
CHECK_OFFSET(cUIThing, mpAnim, 0x00C);
CHECK_OFFSET(cUIThing, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIThing, mAttr, 0x014);
CHECK_OFFSET(cUIThing, mMiscData, 0x018);
CHECK_OFFSET(cUIThing, mColour, 0x01C);
CHECK_OFFSET(cUIThing, mAngle, 0x02C);
CHECK_OFFSET(cUIThing, mHashName, 0x038);
CHECK_OFFSET(cUIThing, mpEvents, 0x03C);
CHECK_OFFSET(cUIThing, mpParent, 0x040);
CHECK_OFFSET(cUIThing, mPosition, 0x044);
CHECK_OFFSET(cUIThing, mScale, 0x050);
CHECK_OFFSET(cUIThing, mpScreen, 0x05C);
CHECK_OFFSET(cUIThing, mSize, 0x060);
CHECK_OFFSET(cUIThing, mSound, 0x06C);
CHECK_SIZE(cUIThing, 0x74);

#endif /* UI_UITHING_H */
