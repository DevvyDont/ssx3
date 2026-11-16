#ifndef UI_UIVECTOR_H
#define UI_UIVECTOR_H

// cUIVector: a UI object drawn as a coloured polygon / polyline (the background shapes, outlines and bars of the FE and
// overlay screens: cScreenPopup's "whitefg" / "Big shape" ..., cKeyboardPopup's boxes). EA's source is
// d:/ssx/code/source/ui/uivector.cpp (preview assert path). Retail keeps its methods in ui/uivector
// (cUIVector_setUIData .. cUIVector_setSize, the default constructor func_003A64F8, getType) and ui/uitext (the
// constructor func_003A3280, the destructor cUIVector__cUIVector, cUIVector_setColour).
//
// Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "UIVector") + func_003A3280(this, pScreen, 0).
// Retail vtable _vt$9cUIVector = D_004941B8 (cUIVector_vtbl; stored by func_003A3280, func_003A64F8 and the destructor).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below; the vptr is cUIThing's (0x8).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals.
// Member names and types are EA's, from SSX On Tour's PSP DWARF (class cUIVector, size 0xA0: its cUIThing is 0x80, so
// every cUIVector member sits exactly 0xC later there), placed at retail offsets by:
//   [R] retail's constructors func_003A3280 (stores 0x74 bits and every pointer) / func_003A64F8, the destructor
//       cUIVector__cUIVector (frees 0x7C..0x8C) and the 0x90 allocation;
//   [U] how retail code uses the field: cUIVector_setUIData allocates the arrays with their tags ("UIVectorClr" 16-byte
//       tARGB, "UIVectorVtx" / "UIVectorVtxRdr" 12-byte t3Vector, "UIVectorLine" 0x30-byte tBxVertex);
//   [O] On Tour's member order and types.
// The preview keeps no out-of-line cUIVector accessors (only the tAttrVector constructor and getType).
//
// mAttrVector is EA's tAttrVector (On Tour: 4 bytes, unsigned int bitfields, LSB first), defined below. Retail reads
// and writes it as a word (lw/sw 0x74; mNumVertices = word >> 17); the unit's views sCount_36D0 / sCount_3808 /
// sCount_3A39F8 / sUIVecFlags_34C0 / sCount_6618 are this same bitfield.

#include "layout.h"
#include "ui/uithing.h"

struct cUIAnimation;
struct cUIScreen;
struct cListNode;
struct t2Vector;
struct tBxVertex;

// EA's tAttrVector (On Tour DWARF: struct tAttrVector, size 0x4). Retail: tAttrVector::tAttrVector() is func_003A6618
// (clears mFill, mLoop, mNumVertices).
struct tAttrVector {
    unsigned int mFill : 1;             // bit 0 [U] cUIVector_setUIData (record flag 1); render2D draws filled when set
    unsigned int mLoop : 1;             // bit 1 [U] cUIVector_setUIData (record flag 2)
    unsigned int mNumTriangles : 15;    // bits 2-16 [U] cUIVector_setUIData; render2D passes it to the draw call
    unsigned int mNumVertices : 15;     // bits 17-31 [U] setUIData; loop bound in render2D / getSize / setSize / setColour
};

CHECK_SIZE(tAttrVector, 0x4);

struct cUIVector {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the cUIThing ctor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked
    void** mVtbl;                    // 0x008 [R] _vt$9cUIVector D_004941B8 (cUIVector_vtbl), stored by func_003A3280,
                                     //       func_003A64F8 and cUIVector__cUIVector
    cUIAnimation* mpAnim;            // 0x00C [P] cUIThing::getAnimation (ui/uithing.h); [U] cUIVector_update
    unsigned char mAnimPlaybackStatus; // 0x010 [O]; [R] = 0 (cUIThing ctor); [U] cUIVector_update passes it on
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] cUIThing's tAttrThing word (lw/sw 0x14); [U] cUIVector_setChannelDataKCP
    int mMiscData;                   // 0x018 [P] cUIThing::getMiscData
    tARGB mColour;                   // 0x01C [P] cUIThing::setColour; [U] cUIVector_setColour copies the colour here
    t3Vector mAngle;                 // 0x02C [O]; [P] cUIThing::getRadAngle (mAngle.z)
    int mHashName;                   // 0x038 [P] cUIThing::getHashName
    unsigned short* mpEvents;        // 0x03C [O]; [U] cUIThing_getKeyframerEvent
    cUIThing* mpParent;              // 0x040 [P] cUIThing::getParent; [R] = pParent
    t3Vector mPosition;              // 0x044 [P] cUIThing::getPosition; [U] cUIVector_render2D, setChannelDataKCP
    t3Vector mScale;                 // 0x050 [P] cUIThing::getScale; [U] cUIVector_render2D, setChannelDataKCP
    cUIScreen* mpScreen;             // 0x05C [P] cUIThing::getScreen; [R] = pScreen
    t3Vector mSize;                  // 0x060 [P] cUIThing::getSize/setSize; [U] cUIVector_setSize stores it
    int mSound[2];                   // 0x06C [O]; [R] = {4, 6} (cUIThing ctor)
    // --- cUIVector
    tAttrVector mAttrVector;         // 0x074 [O] tAttrVector (mFill:1 mLoop:1 mNumTriangles:15 mNumVertices:15); [R] the
                                     //       ctors clear mFill, mLoop, mNumVertices (func_003A6618 on this + 0x74);
                                     //       [U] cUIVector_setUIData fills it; render2D / getSize / setChannelData(KCP) /
                                     //       setColour read the counts (lw)
    unsigned char* mpTriangles;      // 0x078 [O]; [R] = 0; [U] cUIVector_setUIData points it after the record's header
                                     //       when mNumTriangles != 0; cUIVector_render2D reads it
    tARGB* mpVertexColour;           // 0x07C [O]; [R] = 0, freed by the dtor; [U] setUIData new[]s it ("UIVectorClr");
                                     //       cUIVector_setColour / setChannelData(KCP) / render2D
    t3Vector* mpVertexPosition;      // 0x080 [O]; [R] = 0, freed; [U] setUIData new[]s it ("UIVectorVtx", t3vect_Zero);
                                     //       getSize / setSize / setChannelData / render2D
    t3Vector* mpVertexPositionRender; // 0x084 [O]; [R] = 0, freed; [U] setUIData new[]s it ("UIVectorVtxRdr"); render2D
    t2Vector* mpUV;                  // 0x088 [O]; [R] = 0 (func_003A3280), freed by the dtor; [U] cUIVector_setChannelData
                                     //       reads it (never allocated by setUIData)
    tBxVertex* mpVertices;           // 0x08C [O]; [R] = 0, freed; [U] setUIData new[]s mNumVertices 0x30-byte entries
                                     //       ("UIVectorLine") when mFill is clear; cUIVector_render2D fills them
};

CHECK_OFFSET(cUIVector, mpPrev, 0x000);
CHECK_OFFSET(cUIVector, mpNext, 0x004);
CHECK_OFFSET(cUIVector, mVtbl, 0x008);
CHECK_OFFSET(cUIVector, mpAnim, 0x00C);
CHECK_OFFSET(cUIVector, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIVector, unk_011, 0x011);
CHECK_OFFSET(cUIVector, mAttr, 0x014);
CHECK_OFFSET(cUIVector, mMiscData, 0x018);
CHECK_OFFSET(cUIVector, mColour, 0x01C);
CHECK_OFFSET(cUIVector, mAngle, 0x02C);
CHECK_OFFSET(cUIVector, mHashName, 0x038);
CHECK_OFFSET(cUIVector, mpEvents, 0x03C);
CHECK_OFFSET(cUIVector, mpParent, 0x040);
CHECK_OFFSET(cUIVector, mPosition, 0x044);
CHECK_OFFSET(cUIVector, mScale, 0x050);
CHECK_OFFSET(cUIVector, mpScreen, 0x05C);
CHECK_OFFSET(cUIVector, mSize, 0x060);
CHECK_OFFSET(cUIVector, mSound, 0x06C);
CHECK_OFFSET(cUIVector, mAttrVector, 0x074);
CHECK_OFFSET(cUIVector, mpTriangles, 0x078);
CHECK_OFFSET(cUIVector, mpVertexColour, 0x07C);
CHECK_OFFSET(cUIVector, mpVertexPosition, 0x080);
CHECK_OFFSET(cUIVector, mpVertexPositionRender, 0x084);
CHECK_OFFSET(cUIVector, mpUV, 0x088);
CHECK_OFFSET(cUIVector, mpVertices, 0x08C);
CHECK_SIZE(cUIVector, 0x90);

#endif /* UI_UIVECTOR_H */
