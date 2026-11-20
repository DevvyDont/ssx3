#ifndef FE_FESTATEUBERTRICK_H
#define FE_FESTATEUBERTRICK_H

// cFEStateUberTrick: the store's uber trick screen (a cUIState). Methods in fe/festatestore and fe/festateriderbio.
// Size 0x60 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateUberTrick {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 the vtable
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateUberTrick
    int mbModelVisible;             // 0x048 [O] bool
    cUIMenu* mpMainMenu;            // 0x04C [O]
    cUIMenu* mpTrickMenu;           // 0x050 [O]
    cTextureElement* mpCheckmark;   // 0x054 [O]
    cTextureElement* mpCheckbox;    // 0x058 [O]
    cTextureElement* mpDollarSign;  // 0x05C [O]
};

CHECK_OFFSET(cFEStateUberTrick, mbModelVisible, 0x048);
CHECK_OFFSET(cFEStateUberTrick, mpMainMenu, 0x04C);
CHECK_OFFSET(cFEStateUberTrick, mpTrickMenu, 0x050);
CHECK_OFFSET(cFEStateUberTrick, mpCheckmark, 0x054);
CHECK_OFFSET(cFEStateUberTrick, mpCheckbox, 0x058);
CHECK_OFFSET(cFEStateUberTrick, mpDollarSign, 0x05C);
CHECK_SIZE(cFEStateUberTrick, 0x60);

#endif
