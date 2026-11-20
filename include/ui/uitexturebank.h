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
struct tUITextureTableHeader;

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
