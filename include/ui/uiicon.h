#ifndef UI_UIICON_H
#define UI_UIICON_H

// cUIIcon: a UI object drawn as a textured sprite (record type 0x11). Size 0x88: cUIScreen_createObjectByStruct does
// operator new(0x88, "UIIcon") + the cUIThing ctor cUIThing_cUIThing, stores cUIIcon_vtbl (D_00494AA8), constructs mAttrIcon
// (tAttrIcon_tAttrIcon1 on this + 0x74), mTextureID = -1, mpTextureElem = 0. Default ctors cUIIcon_cUIIcon1 / cUIIcon_cUIIcon
// (ui/uivector) do the same. Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below.
// Methods: vtable slots 0 ~cUIIcon (cUIIcon__cUIIcon__FPv), 9 setUIData (cUIIcon_setUIData, ui/uiengine), 16 render2D
// (cUIIcon_render2D = func_00398A78, ui/uiengine), 21 getType; out-of-line inline accessors in ui/uivector
// 0x3A4EC8-0x3A4FB8; cUIIcon_setTextureByHashValue (ui/uiengine) looks a texture element up by id.
// EA's source file is not known (no uiicon.cpp in the preview's assert paths; On Tour: ui/all_in_one_ui.cpp).
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUIIcon 0xA0: its cUIThing is 0x80, so each
// member sits 0xC later there), placed by [R] the constructors and [U] the accessors.
// mAttrIcon is EA's tAttrIcon (On Tour 1 byte: mIconTiled:1 mIconFlip:1 mIconRotate:2); retail reads and writes it as a
// word (lw/sw 0x74: isTiled bit 0, isFlipped bit 1, getRotate bits 2-3).

#include "layout.h"
#include "ui/uithing.h"

struct cTextureElement;

// EA's tAttrIcon (On Tour: 1 byte). Retail accesses it as a 32-bit word, so the bitfield is unsigned int here.
struct tAttrIcon {
    unsigned int mIconTiled : 1;     // bit 0 [O]; [U] cUIIcon_isTiled (get), cUIIcon_setTiled (set)
    unsigned int mIconFlip : 1;      // bit 1 [O]; [U] cUIIcon_isIconFlip (get), cUIIcon_setIconFlip (set)
    unsigned int mIconRotate : 2;    // bits 2-3 [O]; [U] cUIIcon_isIconRotate (!= 0), cUIIcon_setIconRotate (set)
};

CHECK_SIZE(tAttrIcon, 0x4);

struct cUIIcon : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIIcon_vtbl D_00494AA8 (createObjectByStruct, cUIIcon_cUIIcon1, cUIIcon_cUIIcon, the dtor)
    // --- cUIIcon
    // cUIIcon's own slots ([R] cUIIcon_vtbl; types [O] On Tour's class cUIIcon):
    virtual char getType();                                               // 22 cUIIcon_getType__FPv
    tAttrIcon mAttrIcon;             // 0x074 [O] tAttrIcon; [R] its ctor tAttrIcon_tAttrIcon1 (cUIScreen_createObjectByStruct) /
                                     //       tAttrIcon_tAttrIcon clears the low nibble; [U] accessors cUIIcon_isTiled..cUIIcon_setIconRotate
                                     //       (lw/sw 0x74), cUIIcon_setUIData stores the record flags
    int mTextureID;                  // 0x078 [O]; [R] = -1; [U] cUIIcon_getTexture getTextureID (lw), cUIIcon_setTexture setTextureID
                                     //       (sw, clears mpTextureElem), cUIIcon_setElement stores -1; render2D
    const cTextureElement* mpTextureElem; // 0x07C [O]; [R] = 0; [U] cUIIcon_getElement getElement (lw), cUIIcon_setElement
                                     //       setElement (sw); cUIIcon_setTextureByHashValue / cUIIcon_setUIData look it up by id; render2D
    float mTileUV[2];                // 0x080 [O] t2Vector (x, y); [U] cUIIcon_getTileUV getTileUV, cUIIcon_setTileUV / cUIIcon_setTileUV1
                                     //       setTileUV (8-byte ldl/ldr copies); cUIIcon_setUIData; render2D

    // Methods bound to their retail symbols by asm labels.
    void setTextureByHashValue(int id) __asm__("cUIIcon_setTextureByHashValue");
    void* cUIIcon1() __asm__("cUIIcon_cUIIcon1");
    int isTiled() __asm__("cUIIcon_isTiled");
    void setTiled(int tiled) __asm__("cUIIcon_setTiled");
    int isIconFlip() __asm__("cUIIcon_isIconFlip");
    void setIconFlip(int enable) __asm__("cUIIcon_setIconFlip");
    bool isIconRotate() __asm__("cUIIcon_isIconRotate");
    void setIconRotate(int enable) __asm__("cUIIcon_setIconRotate");
    const cTextureElement* getElement() __asm__("cUIIcon_getElement");
    void setElement(const cTextureElement* textureElem) __asm__("cUIIcon_setElement");
    int getTexture() __asm__("cUIIcon_getTexture");
    void setTexture(int textureId) __asm__("cUIIcon_setTexture");
    void setTileUV1(float x, float y) __asm__("cUIIcon_setTileUV1");
    virtual void setUIData(tUIBaseObject* pObj) __asm__("cUIIcon_setUIData");
};

CHECK_OFFSET(cUIIcon, mAttrIcon, 0x074);
CHECK_OFFSET(cUIIcon, mTextureID, 0x078);
CHECK_OFFSET(cUIIcon, mpTextureElem, 0x07C);
CHECK_OFFSET(cUIIcon, mTileUV, 0x080);
CHECK_SIZE(cUIIcon, 0x88);

#endif /* UI_UIICON_H */
