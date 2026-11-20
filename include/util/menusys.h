#ifndef UTIL_MENUSYS_H
#define UTIL_MENUSYS_H

// cMenuSys: the debug-menu system (cGame's MenuSys, allocated in main/gameload). A stack of up to 16 open cMenu
// (util/menu.h), the last pad keys read for them, and the font, input and box texture they draw with. Methods in
// util/menu: init, input, render, activate / isActive, the key getters.
// Size 0x68 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cFont;
struct cInput;
struct cMenu;

struct cMenuSys {
    int MenuDepth;                  // 0x000 [O]
    cMenu* MenuStack[16];           // 0x004 [O]
    int Owner;                      // 0x044 [O]
    char Key[0x4];                  // 0x048 [O] tMenuKeys
    int VKey;                       // 0x04C [O]
    int AsciiKey;                   // 0x050 [O]
    int Active;                     // 0x054 [O]
    cFont* Font;                    // 0x058 [O]
    cInput* Input;                  // 0x05C [O]
    int BoxTexture;                 // 0x060 [O]
    int MenuYOffset;                // 0x064 [O]
};

CHECK_OFFSET(cMenuSys, MenuDepth, 0x000);
CHECK_OFFSET(cMenuSys, MenuStack, 0x004);
CHECK_OFFSET(cMenuSys, Owner, 0x044);
CHECK_OFFSET(cMenuSys, Key, 0x048);
CHECK_OFFSET(cMenuSys, VKey, 0x04C);
CHECK_OFFSET(cMenuSys, AsciiKey, 0x050);
CHECK_OFFSET(cMenuSys, Active, 0x054);
CHECK_OFFSET(cMenuSys, Font, 0x058);
CHECK_OFFSET(cMenuSys, Input, 0x05C);
CHECK_OFFSET(cMenuSys, BoxTexture, 0x060);
CHECK_OFFSET(cMenuSys, MenuYOffset, 0x064);
CHECK_SIZE(cMenuSys, 0x68);

#endif
