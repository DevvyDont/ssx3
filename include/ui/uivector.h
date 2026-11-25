#ifndef UI_UIVECTOR_H
#define UI_UIVECTOR_H

// cUIVector: a UI object drawn as a coloured polygon / polyline (the background shapes, outlines and bars of the FE and
// overlay screens: cScreenPopup's "whitefg" / "Big shape" ..., cKeyboardPopup's boxes). EA's source is
// d:/ssx/code/source/ui/uivector.cpp (preview assert path). Retail keeps its methods in ui/uivector
// (cUIVector_setUIData .. cUIVector_setSize, the default constructor cUIVector_cUIVector1, getType) and ui/uitext (the
// constructor cUIVector_cUIVector, the destructor cUIVector__cUIVector, cUIVector_setColour).
//
// Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "UIVector") + cUIVector_cUIVector(this, pScreen, 0).
// Retail vtable _vt$9cUIVector = D_004941B8 (cUIVector_vtbl; stored by cUIVector_cUIVector, cUIVector_cUIVector1 and the destructor).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below; the vptr is cUIThing's (0x8).
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h: every field at its retail offset, no C++ inheritance or virtuals.
// Member names and types are EA's, from SSX On Tour's PSP DWARF (class cUIVector, size 0xA0: its cUIThing is 0x80, so
// every cUIVector member sits exactly 0xC later there), placed at retail offsets by:
//   [R] retail's constructors cUIVector_cUIVector (stores 0x74 bits and every pointer) / cUIVector_cUIVector1, the destructor
//       cUIVector__cUIVector (frees 0x7C..0x8C) and the 0x90 allocation;
//   [U] how retail code uses the field: cUIVector_setUIData allocates the arrays with their tags ("UIVectorClr" 16-byte
//       tARGB, "UIVectorVtx" / "UIVectorVtxRdr" 12-byte t3Vector, "UIVectorLine" 0x30-byte tBxVertex);
//   [O] On Tour's member order and types.
// The preview keeps no out-of-line cUIVector accessors (only the tAttrVector constructor and getType).
//
// mAttrVector is EA's tAttrVector (On Tour: 4 bytes, unsigned int bitfields, LSB first), defined below. Retail reads
// and writes it as a word (lw/sw 0x74; mNumVertices = word >> 17); setUIData, setChannelData(KCP) and render2D use it
// as this type, and the unit's view sCount_6618 (tAttrVector_tAttrVector) is the same bitfield.

#include "layout.h"
#include "ui/uithing.h"

struct cUIAnimation;
struct cUIScreen;
struct cListNode;
struct t2Vector;
struct tBxVertex;

// EA's tAttrVector (On Tour DWARF: struct tAttrVector, size 0x4). Retail: tAttrVector::tAttrVector() is tAttrVector_tAttrVector
// (clears mFill, mLoop, mNumVertices).
struct tAttrVector {
    unsigned int mFill : 1;             // bit 0 [U] cUIVector_setUIData (record flag 1); render2D draws filled when set
    unsigned int mLoop : 1;             // bit 1 [U] cUIVector_setUIData (record flag 2)
    unsigned int mNumTriangles : 15;    // bits 2-16 [U] cUIVector_setUIData; render2D passes it to the draw call
    unsigned int mNumVertices : 15;     // bits 17-31 [U] setUIData; loop bound in render2D / getSize / setSize / setColour
};

CHECK_SIZE(tAttrVector, 0x4);

struct tARGB;

struct cUIVector : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] _vt$9cUIVector D_004941B8 (cUIVector_vtbl), stored by cUIVector_cUIVector, cUIVector_cUIVector1 and
    //       cUIVector__cUIVector
    // cUIVector's uses of cUIThing's fields:
    //   mpAnim: [U] cUIVector_update
    //   mAnimPlaybackStatus: [U] cUIVector_update passes it on
    //   mAttr: [U] cUIVector_setChannelDataKCP
    //   mColour: [U] cUIVector_setColour copies the colour here
    //   mpEvents: [U] cUIThing_getKeyframerEvent
    //   mPosition: [U] cUIVector_render2D, setChannelDataKCP
    //   mScale: [U] cUIVector_render2D, setChannelDataKCP
    //   mSize: [U] cUIVector_setSize stores it
    // --- cUIVector
    // cUIVector's own slots ([R] cUIVector_vtbl; types [O] On Tour's class cUIVector):
    virtual char getType();                                               // 22 cUIVector_getType__FPv
    tAttrVector mAttrVector;         // 0x074 [O] tAttrVector (mFill:1 mLoop:1 mNumTriangles:15 mNumVertices:15); [R] the
                                     //       ctors clear mFill, mLoop, mNumVertices (tAttrVector_tAttrVector on this + 0x74);
                                     //       [U] cUIVector_setUIData fills it; render2D / getSize / setChannelData(KCP) /
                                     //       setColour read the counts (lw)
    unsigned char* mpTriangles;      // 0x078 [O]; [R] = 0; [U] cUIVector_setUIData points it after the record's header
                                     //       when mNumTriangles != 0; cUIVector_render2D reads it
    tARGB* mpVertexColour;           // 0x07C [O]; [R] = 0, freed by the dtor; [U] setUIData new[]s it ("UIVectorClr");
                                     //       cUIVector_setColour / setChannelData(KCP) / render2D
    t3Vector* mpVertexPosition;      // 0x080 [O]; [R] = 0, freed; [U] setUIData new[]s it ("UIVectorVtx", t3vect_Zero);
                                     //       getSize / setSize / setChannelData / render2D
    t3Vector* mpVertexPositionRender; // 0x084 [O]; [R] = 0, freed; [U] setUIData new[]s it ("UIVectorVtxRdr"); render2D
    t2Vector* mpUV;                  // 0x088 [O]; [R] = 0 (cUIVector_cUIVector), freed by the dtor; [U] cUIVector_setChannelData
                                     //       reads it (never allocated by setUIData)
    tBxVertex* mpVertices;           // 0x08C [O]; [R] = 0, freed; [U] setUIData new[]s mNumVertices 0x30-byte entries
                                     //       ("UIVectorLine") when mFill is clear; cUIVector_render2D fills them

    // Methods bound to their retail symbols by asm labels.
    void setColour(tARGB* colour) __asm__("cUIVector_setColour");
};

CHECK_OFFSET(cUIVector, mAttrVector, 0x074);
CHECK_OFFSET(cUIVector, mpTriangles, 0x078);
CHECK_OFFSET(cUIVector, mpVertexColour, 0x07C);
CHECK_OFFSET(cUIVector, mpVertexPosition, 0x080);
CHECK_OFFSET(cUIVector, mpVertexPositionRender, 0x084);
CHECK_OFFSET(cUIVector, mpUV, 0x088);
CHECK_OFFSET(cUIVector, mpVertices, 0x08C);
CHECK_SIZE(cUIVector, 0x90);

#endif /* UI_UIVECTOR_H */
