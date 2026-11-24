#ifndef UI_UIDATA_H
#define UI_UIDATA_H

// The UI screen data records: what a screen file holds and the UI engine reads in place (cUIScreen_createAllObjects /
// createObjectByHashName walk the object records, each object's setUIData reads its own record, the keyframer walks
// the frames and their key-channel packets, cUIAnimation reads its header). Every object record starts with the
// 0x20-byte tUIBaseObject and continues with the kind's own fields from 0x20.
//
// Names and layouts: SSX On Tour's DWARF ([O] tUIBaseObject, tUIText, tUITextScroll, tUIIcon, tUIListBox, tUIARGB,
// tUIAnimation, tUIFrameHeader, tUIKCPBase, tUIKCPObjectAnimate, tUIKCPKeyframeJump, tUIHeaderBlock, tUIScreenHeader,
// tUIKeyframeBlock, tUIObjectBlock, tUIPlaceWidget), placed by retail use ([U], the setUIData functions in
// ui/uithing, ui/uitext and ui/uiengine; cUIScreen_createObjectByHashName / playFrameByThread in ui/uiscreen;
// cUIAnimation_updateThing in ui/uianimation). The records are byte data, so On Tour's sizes and offsets hold
// unchanged (no bools, no vptrs). The derived records repeat tUIBaseObject's members (flat, like main/game.h).
//
// Evidence tags: [O] On Tour's DWARF; [U] how retail code uses the field.

#include "layout.h"

// A record colour: four bytes, scaled by 1/255 into a tARGB. [O] struct tUIARGB (0x4).
struct tUIARGB {
    unsigned char a;                // 0x0 [O]; [U] the setUIData functions scale each byte by 1/255
    unsigned char r;                // 0x1 [O]
    unsigned char g;                // 0x2 [O]
    unsigned char b;                // 0x3 [O]
};

// The common head of every object record. [O] struct tUIBaseObject (0x20).
struct tUIBaseObject {
    unsigned short mTag;            // 0x00 [O] the object kind
    unsigned short mByteSize;       // 0x02 [O]; [U] cUIScreen_createObjectByHashName steps to the next record by it
    unsigned int mHashID;           // 0x04 [O]; [U] createObjectByHashName matches the hash; the object's mHashName
    unsigned int mBaseFlags;        // 0x08 [O]; [U] the base setUIData reads the layer bits
    unsigned short mEventAnims[10]; // 0x0C [O]; [U] the object's keyframer events (cUIThing::mpEvents points here)
};

// A text object's record. [O] struct tUIText : tUIBaseObject (0x2C).
struct tUIText {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned char mFontID;          // 0x20 [O]; [U] cUIText_setUIData: & 3 (one of the four fonts)
    unsigned char mTypeFlags;       // 0x21 [O]
    unsigned char mBools1;          // 0x22 [O]; [U] cUIText_setUIData passes bit 0 to vtable slot 23
    unsigned char mPad1_tUIText;    // 0x23 [O]
    unsigned int mStringID;         // 0x24 [O]; [U] cUIText_setUnicodeStringByID's id
    tUIARGB mSColour;               // 0x28 [O]; [U] the static colour (bytes / 255)
};

// A scrolling text object's record. [O] struct tUITextScroll : tUIBaseObject (0x2C).
struct tUITextScroll {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned char mFontID;          // 0x20 [O]; [U] cUITextScroll_setUIData: the font
    unsigned char mTextFlags;       // 0x21 [O]; [U] cUITextScroll_setUIData uses bit 0
    unsigned char mNumItems;        // 0x22 [O]
    unsigned char mPad1_tUITextScroll; // 0x23 [O]
    unsigned int mStringID;         // 0x24 [O]; [U] the text's string id
    tUIARGB mSColour;               // 0x28 [O]; [U] the static colour
};

// An icon's record. [O] struct tUIIcon : tUIBaseObject (0x2C).
struct tUIIcon {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned int mIconFlags;        // 0x20 [O]; [U] cUIIcon_setUIData copies bits 0-1 (alignment) and bit 2 (clip)
    unsigned int mTextureID;        // 0x24 [O]; [U] the texture-bank element's hash (getElementByHashname)
    unsigned short mTiledU;         // 0x28 [O]; [U] the tile size u in hundredths (x 0.01)
    unsigned short mTiledV;         // 0x2A [O]; [U] the tile size v in hundredths
};

// A list box's record. [O] struct tUIListBox : tUIBaseObject (0x28).
struct tUIListBox {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned char mFontID;          // 0x20 [O]; [U] cUIListBox_setUIData: & 3
    unsigned char mNumEntries;      // 0x21 [O]
    unsigned short mFlags;          // 0x22 [O]; [U] cUIListBox_setUIData passes bit 0 to vtable slot 23
    tUIARGB mSColour;               // 0x24 [O]; [U] the static colour (bytes / 255)
};

// A vector (polygon) object's record. [O] struct tUIVector : tUIBaseObject (0x24); [U] cUIVector_setUIData reads
// mNumVertices / mNumTriangles / mFlags (bit 0 fill, bit 1 loop) into tAttrVector and allocates mNumVertices
// vertices and colours (ui/uivector's view sUIVecData_34C0, which keeps these names).
struct tUIVector {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    char mNumVertices;              // 0x20 [O]; [U] tAttrVector::mNumVertices, the vertex / colour array lengths
    char mNumTriangles;             // 0x21 [O]; [U] tAttrVector::mNumTriangles
    char mFlags;                    // 0x22 [O]; [U] bit 0 -> mFill, bit 1 -> mLoop
    char mPad;                      // 0x23 [O]
};

// A child a container record places: the hash name of an object created earlier on the same screen.
// [O] struct tUIPlaceWidget (0x4) and On Tour cUIScreen::createObjectByStruct's locals tUIPlaceWidget * pPBW.
struct tUIPlaceWidget {
    unsigned int mHashID;           // 0x0 [O]; [U] cUIScreen_createObjectByStruct finds the child by it
};

// The container records (group, menu, pair, progress, scroll bar, slider). On Tour has no types for them: its
// cUIScreen::createObjectByStruct reads them through a local char * base. Type and field names ours, placed by [U]
// cUIScreen_createObjectByStruct (the only reader past 0x20; these kinds' setUIData read only tUIBaseObject).

// A group's record (type 0x10): its children. A variable-length record (name ours).
struct tUIGroup {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned int mNumWidgets;       // 0x20 (name ours) [U] createObjectByStruct: the child count (lw)
    tUIPlaceWidget mWidgets[1];     // 0x24 (name ours) [U] mNumWidgets entries: the children, in list order
};

// A menu's record (type 0x13): its items. A variable-length record (name ours).
struct tUIMenu {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned char mNumWidgets;      // 0x20 (name ours) [U] createObjectByStruct: the item count (lbu)
    char unk_021[0x3];              // 0x21 no retail access seen
    tUIPlaceWidget mWidgets[1];     // 0x24 (name ours) [U] mNumWidgets entries: the items, in list order
};

// A pair's record (type 0x15): its two children (name ours).
struct tUIPair {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    tUIPlaceWidget mWidgets[2];     // 0x20 (name ours) [U] createObjectByStruct: [0] becomes cUIPair::mpEntry,
                                    //     [1] cUIPair::mpWidget (its mAttr bit 0 cleared)
};

// A progress bar's record (type 0x19): background and knob (name ours).
struct tUIProgress {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    tUIPlaceWidget mBackground;     // 0x20 (name ours) [U] createObjectByStruct: becomes cUIProgress::mpObjBackground
    tUIPlaceWidget mKnob;           // 0x24 (name ours) [U] becomes cUIProgress::mpObjKnob
};

// A scroll bar's record (type 0x21): background and knob, as tUIProgress (name ours).
struct tUIScrollBar {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    tUIPlaceWidget mBackground;     // 0x20 (name ours) [U] createObjectByStruct: becomes cUIScrollBar::mpObjBackground
    tUIPlaceWidget mKnob;           // 0x24 (name ours) [U] becomes cUIScrollBar::mpObjKnob
};

// A slider's record (type 0x16): background and knob (name ours).
struct tUISlider {
    unsigned short mTag;            // 0x00 [O] tUIBaseObject
    unsigned short mByteSize;       // 0x02 [O]
    unsigned int mHashID;           // 0x04 [O]
    unsigned int mBaseFlags;        // 0x08 [O]
    unsigned short mEventAnims[10]; // 0x0C [O]
    unsigned int unk_020;           // 0x20 no retail access seen
    tUIPlaceWidget mBackground;     // 0x24 (name ours) [U] createObjectByStruct: becomes cUISlider::mpObjBackground
    tUIPlaceWidget mKnob;           // 0x28 (name ours) [U] becomes cUISlider::mpObjKnob
};

// An animation's header. [O] struct tUIAnimation (0x10).
struct tUIAnimation {
    unsigned short mTag;            // 0x00 [O]
    unsigned short mNumFrames;      // 0x02 [O]; [U] cUIAnimation_updateThing: the frame count
    unsigned int mNumChannels;      // 0x04 [O]; [U] cUIAnimation_updateThing: the channel count
    unsigned int mHashID;           // 0x08 [O]
    unsigned int mByteSize;         // 0x0C [O]
};

// One animation channel's record (cUIAnimationChannel::mpAnimChan, ui/uianimation.h); the channel records follow the
// tUIAnimation header from 0x10, each mByteSize long, and a record's keys follow it from 0x8 ([O] tUIAnimLine
// {unsigned short mFrames; short mValue}, the {dt, value} keys of cUIAnimationChannel_evalAtFrameFloat's view).
// [O] struct tUIAnimChannel (0x8).
struct tUIAnimChannel {
    unsigned short mTag;            // 0x0 [O]
    unsigned short mType;           // 0x2 [O]; [U] cUIAnimationChannel_getAnimChanType returns it (lhu 0x2), the
                                    //     property cUIAnimation_updateThing hands to the object's setChannelData
    unsigned short mByteSize;       // 0x4 [O]; [U] cUIAnimation_cUIAnimation steps to the next record by it
    unsigned short mPoints;         // 0x6 [O]; [U] cUIAnimationChannel_evalAtFrameFloat: the key count (lhu 0x6)
};

// A keyframe's header; its key-channel packets follow from 0xC. [O] struct tUIFrameHeader (0xC).
struct tUIFrameHeader {
    unsigned int mHashID;           // 0x00 [O]
    unsigned int mByteSize;         // 0x04 [O]
    unsigned short mFrameNumber;    // 0x08 [O]
    unsigned short mNumKCP;         // 0x0A [O]; [U] cUIScreen_playFrameByThread runs this many packets
};

// The head of every key-channel packet in a frame. [O] struct tUIKCPBase (0x4).
struct tUIKCPBase {
    unsigned char mTag;             // 0x0 [O]; [U] cUIScreen_playFrameByThread switches on it
    unsigned char mPad1_tUIKCPBase; // 0x1 [O]
    unsigned short mPacketSize;     // 0x2 [O]; [U] playFrameByThread steps to the next packet by it
};

// The object-animate key-channel packet: plays an animation on an object. [O] struct tUIKCPObjectAnimate : tUIKCPBase
// (0x10) and On Tour cUIScreen::parseObjectAnim's local tUIKCPObjectAnimate * pPacket. tUIKCPBase repeated flat.
struct tUIKCPObjectAnimate {
    unsigned char mTag;             // 0x0 [O] tUIKCPBase
    unsigned char mPad1_tUIKCPBase; // 0x1 [O]
    unsigned short mPacketSize;     // 0x2 [O]
    unsigned int mAnimationID;      // 0x4 [O]; [U] cUIScreen_parseObjectAnim: cUIAnimationBank_getAnimationByHashName's hash
    unsigned int mObjectHashID;     // 0x8 [O]; [U] cUIScreen_parseObjectAnim: cUIScreen_getObjectByHashName's hash
    unsigned int mPlaybackStatus;   // 0xC [O]; [U] cUIScreen_parseObjectAnim passes it (read as a byte) to
                                    //     cUIThing_playAnimation
};

// The keyframe jump / spawn key-channel packet. [O] struct tUIKCPKeyframeJump : tUIKCPBase (0x8) and On Tour
// cUIScreen::parseKeyframeJump / parseKeyframeSpawn's local tUIKCPKeyframeJump * pPacket. tUIKCPBase repeated flat.
struct tUIKCPKeyframeJump {
    unsigned char mTag;             // 0x0 [O] tUIKCPBase
    unsigned char mPad1_tUIKCPBase; // 0x1 [O]
    unsigned short mPacketSize;     // 0x2 [O]
    unsigned int mFrameHashName;    // 0x4 [O]; [U] cUIScreen_parseKeyframeJump / parseKeyframeSpawn find the frame
                                    //     whose mHashID it is
};

// The UI file's header: offsets (from the file start) of its blocks. [O] struct tUIHeaderBlock (0x18) and On Tour
// cUIEngine::loadFile's local const tUIHeaderBlock * pHeaderBlock.
struct tUIHeaderBlock {
    char unk_000[4];                // 0x00 [O] mSignature; no retail access seen
    int unk_004;                    // 0x04 [O] mCookie (float); no retail access seen
    unsigned int mAnimBlockOffset;  // 0x08 [O]; [U] cUIEngine_loadFile hands file + it to cUIAnimationBank_setData
    unsigned int mScreenBlockOffset; // 0x0C [O]; [U] cUIEngine_addScreenByHashName finds the screen table at file + it
    unsigned int mTextureBlockOffset; // 0x10 [O]; [U] cUIEngine_loadFile hands file + it to cUITextureBank_setData
    unsigned int unk_014;           // 0x14 [O] mFontBlockOffset; no retail access seen
};

// A screen's header: its hash and the offsets (from the header) of its blocks. [O] struct tUIScreenHeader (0x10) and
// On Tour cUIScreen::setData(tUIScreenHeader * pScreenHeader); cUIScreen::mpScreenHeader (ui/uiscreen.h).
struct tUIScreenHeader {
    unsigned int mHashID;           // 0x0 [O]; [U] cUIScreen_getHashName returns it
    unsigned int mObjectBlockOffset; // 0x4 [O]; [U] cUIScreen_setData: mpObjectBlock = header + it
    unsigned int mFrameBlockOffset; // 0x8 [O]; [U] cUIScreen_setData: mpKeyframeBlock = header + it
    unsigned int unk_00C;           // 0xC [O] mAnimBlockOffset; no retail access seen
};

// A screen's keyframe block: the frame count, then the frames (tUIFrameHeader, each mByteSize long) from 0x4.
// [O] struct tUIKeyframeBlock (0x4); cUIScreen::mpKeyframeBlock (ui/uiscreen.h).
struct tUIKeyframeBlock {
    unsigned int mNumFrames;        // 0x0 [O]; [U] cUIScreen_getFrameByIndex loops over this many frames from +0x4
};

// A screen's object block: the object count, then the object records (each mByteSize long) from 0x4.
// [O] struct tUIObjectBlock (0x4); cUIScreen::mpObjectBlock (ui/uiscreen.h).
struct tUIObjectBlock {
    unsigned int mNumObjects;       // 0x0 [O]; [U] cUIScreen_createAllObjects / createObjectByHashName /
                                    //     getObjectByHashName loop over this many records from +0x4
};

CHECK_OFFSET(tUIARGB, a, 0x0);
CHECK_OFFSET(tUIARGB, r, 0x1);
CHECK_OFFSET(tUIARGB, g, 0x2);
CHECK_OFFSET(tUIARGB, b, 0x3);
CHECK_SIZE(tUIARGB, 0x4);

CHECK_OFFSET(tUIBaseObject, mTag, 0x00);
CHECK_OFFSET(tUIBaseObject, mByteSize, 0x02);
CHECK_OFFSET(tUIBaseObject, mHashID, 0x04);
CHECK_OFFSET(tUIBaseObject, mBaseFlags, 0x08);
CHECK_OFFSET(tUIBaseObject, mEventAnims, 0x0C);
CHECK_SIZE(tUIBaseObject, 0x20);

CHECK_OFFSET(tUIText, mFontID, 0x20);
CHECK_OFFSET(tUIText, mTypeFlags, 0x21);
CHECK_OFFSET(tUIText, mBools1, 0x22);
CHECK_OFFSET(tUIText, mPad1_tUIText, 0x23);
CHECK_OFFSET(tUIText, mStringID, 0x24);
CHECK_OFFSET(tUIText, mSColour, 0x28);
CHECK_SIZE(tUIText, 0x2C);

CHECK_OFFSET(tUITextScroll, mFontID, 0x20);
CHECK_OFFSET(tUITextScroll, mTextFlags, 0x21);
CHECK_OFFSET(tUITextScroll, mNumItems, 0x22);
CHECK_OFFSET(tUITextScroll, mPad1_tUITextScroll, 0x23);
CHECK_OFFSET(tUITextScroll, mStringID, 0x24);
CHECK_OFFSET(tUITextScroll, mSColour, 0x28);
CHECK_SIZE(tUITextScroll, 0x2C);

CHECK_OFFSET(tUIIcon, mIconFlags, 0x20);
CHECK_OFFSET(tUIIcon, mTextureID, 0x24);
CHECK_OFFSET(tUIIcon, mTiledU, 0x28);
CHECK_OFFSET(tUIIcon, mTiledV, 0x2A);
CHECK_SIZE(tUIIcon, 0x2C);

CHECK_OFFSET(tUIListBox, mFontID, 0x20);
CHECK_OFFSET(tUIListBox, mNumEntries, 0x21);
CHECK_OFFSET(tUIListBox, mFlags, 0x22);
CHECK_OFFSET(tUIListBox, mSColour, 0x24);
CHECK_SIZE(tUIListBox, 0x28);
CHECK_OFFSET(tUIVector, mNumVertices, 0x20);
CHECK_OFFSET(tUIVector, mNumTriangles, 0x21);
CHECK_OFFSET(tUIVector, mFlags, 0x22);
CHECK_SIZE(tUIVector, 0x24);

CHECK_OFFSET(tUIPlaceWidget, mHashID, 0x0);
CHECK_SIZE(tUIPlaceWidget, 0x4);

CHECK_OFFSET(tUIGroup, mNumWidgets, 0x20);
CHECK_OFFSET(tUIGroup, mWidgets, 0x24);
CHECK_OFFSET(tUIMenu, mNumWidgets, 0x20);
CHECK_OFFSET(tUIMenu, unk_021, 0x21);
CHECK_OFFSET(tUIMenu, mWidgets, 0x24);
CHECK_OFFSET(tUIPair, mWidgets, 0x20);
CHECK_OFFSET(tUIProgress, mBackground, 0x20);
CHECK_OFFSET(tUIProgress, mKnob, 0x24);
CHECK_OFFSET(tUIScrollBar, mBackground, 0x20);
CHECK_OFFSET(tUIScrollBar, mKnob, 0x24);
CHECK_OFFSET(tUISlider, unk_020, 0x20);
CHECK_OFFSET(tUISlider, mBackground, 0x24);
CHECK_OFFSET(tUISlider, mKnob, 0x28);

CHECK_OFFSET(tUIAnimation, mTag, 0x00);
CHECK_OFFSET(tUIAnimation, mNumFrames, 0x02);
CHECK_OFFSET(tUIAnimation, mNumChannels, 0x04);
CHECK_OFFSET(tUIAnimation, mHashID, 0x08);
CHECK_OFFSET(tUIAnimation, mByteSize, 0x0C);
CHECK_SIZE(tUIAnimation, 0x10);

CHECK_OFFSET(tUIAnimChannel, mTag, 0x0);
CHECK_OFFSET(tUIAnimChannel, mType, 0x2);
CHECK_OFFSET(tUIAnimChannel, mByteSize, 0x4);
CHECK_OFFSET(tUIAnimChannel, mPoints, 0x6);
CHECK_SIZE(tUIAnimChannel, 0x8);

CHECK_OFFSET(tUIFrameHeader, mHashID, 0x00);
CHECK_OFFSET(tUIFrameHeader, mByteSize, 0x04);
CHECK_OFFSET(tUIFrameHeader, mFrameNumber, 0x08);
CHECK_OFFSET(tUIFrameHeader, mNumKCP, 0x0A);
CHECK_SIZE(tUIFrameHeader, 0xC);

CHECK_OFFSET(tUIKCPBase, mTag, 0x0);
CHECK_OFFSET(tUIKCPBase, mPad1_tUIKCPBase, 0x1);
CHECK_OFFSET(tUIKCPBase, mPacketSize, 0x2);
CHECK_SIZE(tUIKCPBase, 0x4);

CHECK_OFFSET(tUIKCPObjectAnimate, mTag, 0x0);
CHECK_OFFSET(tUIKCPObjectAnimate, mPad1_tUIKCPBase, 0x1);
CHECK_OFFSET(tUIKCPObjectAnimate, mPacketSize, 0x2);
CHECK_OFFSET(tUIKCPObjectAnimate, mAnimationID, 0x4);
CHECK_OFFSET(tUIKCPObjectAnimate, mObjectHashID, 0x8);
CHECK_OFFSET(tUIKCPObjectAnimate, mPlaybackStatus, 0xC);
CHECK_SIZE(tUIKCPObjectAnimate, 0x10);

CHECK_OFFSET(tUIKCPKeyframeJump, mTag, 0x0);
CHECK_OFFSET(tUIKCPKeyframeJump, mPacketSize, 0x2);
CHECK_OFFSET(tUIKCPKeyframeJump, mFrameHashName, 0x4);
CHECK_SIZE(tUIKCPKeyframeJump, 0x8);

CHECK_OFFSET(tUIHeaderBlock, unk_000, 0x00);
CHECK_OFFSET(tUIHeaderBlock, unk_004, 0x04);
CHECK_OFFSET(tUIHeaderBlock, mAnimBlockOffset, 0x08);
CHECK_OFFSET(tUIHeaderBlock, mScreenBlockOffset, 0x0C);
CHECK_OFFSET(tUIHeaderBlock, mTextureBlockOffset, 0x10);
CHECK_OFFSET(tUIHeaderBlock, unk_014, 0x14);
CHECK_SIZE(tUIHeaderBlock, 0x18);

CHECK_OFFSET(tUIScreenHeader, mHashID, 0x0);
CHECK_OFFSET(tUIScreenHeader, mObjectBlockOffset, 0x4);
CHECK_OFFSET(tUIScreenHeader, mFrameBlockOffset, 0x8);
CHECK_OFFSET(tUIScreenHeader, unk_00C, 0xC);
CHECK_SIZE(tUIScreenHeader, 0x10);

CHECK_OFFSET(tUIKeyframeBlock, mNumFrames, 0x0);
CHECK_SIZE(tUIKeyframeBlock, 0x4);

CHECK_OFFSET(tUIObjectBlock, mNumObjects, 0x0);
CHECK_SIZE(tUIObjectBlock, 0x4);

#endif /* UI_UIDATA_H */
