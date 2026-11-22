#ifndef UI_UITEXTUREBANK_H
#define UI_UITEXTUREBANK_H

// cUITextureBank: the UI engine's texture bank (cUIEngine::mTextureBank, ui/uiengine.h): the texture table header, its
// texture IDs and elements, and the owning engine. Methods in ui/uiengine: setData, getElementByHashname,
// getTextureByHashname.
//
// Size 0x18 (On Tour's; it fills cUIEngine 0x58-0x6F, whose layout is verified). A FLAT layout, like main/game.h:
// member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this`
// in the methods falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"

struct cTextureElement;
struct cUIEngine;
// The UI file's texture table: a header word, then mNumEntries entries. [O] struct tUITextureTableHeader (0x4) and
// struct tUITextureTableEntry (0x18), cUITextureBank::mpTextureTableHeader's type; [R] cUITextureBank_setData (On
// Tour cUITextureBank::setData(pShapeName, pData)) stores pData there, takes the element count from its first word
// and reads entries of 0x18 bytes from +0x4 (ui/uiengine's views sUITexData8138 / sUITexSrc8138).
struct tUITextureTableHeader {
    unsigned int mNumEntries;       // 0x000 [O]; [U] setData: mNumElements (the entries follow the header)
};

// [O] struct tUIuv (0x8).
struct tUIuv {
    float u;                        // 0x000 [O]
    float v;                        // 0x004 [O]
};

struct tUITextureTableEntry {
    unsigned int mHashID;           // 0x000 [O]; [U] setData copies it to the element's mHashID (getElementByHashname's key)
    unsigned char mShapeFileID;     // 0x004 [O]
    unsigned char mTPageID;         // 0x005 [O]; [U] setData: the element's texture = mpTexturesIDs[mTPageID]
    unsigned char mCharPad[2];      // 0x006 [O]
    tUIuv mTopLeft;                 // 0x008 [O]; [U] setData copies the element's texture coordinates
    tUIuv mBottomRight;             // 0x010 [O]
};

CHECK_SIZE(tUITextureTableHeader, 0x4);
CHECK_SIZE(tUIuv, 0x8);
CHECK_OFFSET(tUITextureTableEntry, mHashID, 0x000);
CHECK_OFFSET(tUITextureTableEntry, mShapeFileID, 0x004);
CHECK_OFFSET(tUITextureTableEntry, mTPageID, 0x005);
CHECK_OFFSET(tUITextureTableEntry, mTopLeft, 0x008);
CHECK_OFFSET(tUITextureTableEntry, mBottomRight, 0x010);
CHECK_SIZE(tUITextureTableEntry, 0x18);

struct cUITextureBank {
    tUITextureTableHeader* mpTextureTableHeader; // 0x000 [O]
    int* mpTexturesIDs;                          // 0x004 [O]
    int mNumTextureIDs;                          // 0x008 [O]
    cTextureElement* mpElements;                 // 0x00C [O]
    int mNumElements;                            // 0x010 [O]
    cUIEngine* mpEngine;                         // 0x014 [O]
};

CHECK_OFFSET(cUITextureBank, mpTextureTableHeader, 0x000);
CHECK_OFFSET(cUITextureBank, mpTexturesIDs, 0x004);
CHECK_OFFSET(cUITextureBank, mNumTextureIDs, 0x008);
CHECK_OFFSET(cUITextureBank, mpElements, 0x00C);
CHECK_OFFSET(cUITextureBank, mNumElements, 0x010);
CHECK_OFFSET(cUITextureBank, mpEngine, 0x014);
CHECK_SIZE(cUITextureBank, 0x18);

#endif
