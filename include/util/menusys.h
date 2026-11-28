#ifndef UTIL_MENUSYS_H
#define UTIL_MENUSYS_H

// cMenuSys: the debug-menu system (cGame's MenuSys, allocated in main/gameload). A stack of up to 16 open cMenu
// (util/menu.h), the last pad keys read for them, and the font, input and box texture they draw with. Methods in
// util/menu: init, input, render, activate / isActive, the key getters.
// Size 0x68 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words (type in the comment); members of class type are byte
// arrays (type in the comment).

#include "layout.h"

struct cFont;
struct cInput;
struct cMenu;

// A menu key (cMenuSys::Key, and the value cInput_getValue reports for the menu-key input state 61, On Tour
// kIS_MenuKey). [O] On Tour's enum tMenuKeys, names and values; [R] cMenuSys_init stores kMenuKeyNull (11); [U]
// cDebugMenuScreen_update moves its cursor with kMenuKeyUp / kMenuKeyDown, changes a row with kMenuKeyLeft /
// kMenuKeyRight and runs the network and lobby rows on kMenuKeySelect.
enum tMenuKeys {
    kMenuKeyUp = 0,
    kMenuKeyDown = 1,
    kMenuKeyLeft = 2,
    kMenuKeyRight = 3,
    kMenuKeyLeftShift = 4,
    kMenuKeyRightShift = 5,
    kMenuKeySelect = 6,
    kMenuKeyCancel = 7,
    kMenuKeyBack = 8,
    kMenuKeyOpt1 = 9,
    kMenuKeyOpt2 = 10,
    kMenuKeyNull = 11
};

void operator_delete(int* p);

struct cMenuSys {
    // Key, VKey and AsciiKey when nothing was read this frame (name ours). [U] cMenuSys_input stores -1 in VKey and
    // AsciiKey before reading, and asks the USB keyboard only when the pad's menu key is -1.
    enum { kNoKey = -1 };

    int MenuDepth;                  // 0x000 [O]
    cMenu* MenuStack[16];           // 0x004 [O]
    int Owner;                      // 0x044 [O]
    int Key;                        // 0x048 [O] tMenuKeys (an enum, 4 bytes: kMenuKeyUp 0 .. kMenuKeyNull 11);
                                    //       [R] cMenuSys_init stores 11 (kMenuKeyNull, sw); [U] cMenuSys_getKey returns it (lw),
                                    //       cMenuSys_input stores the pad's menu key
    int VKey;                       // 0x04C [O]
    int AsciiKey;                   // 0x050 [O]
    int Active;                     // 0x054 [O]
    cFont* Font;                    // 0x058 [O]
    cInput* Input;                  // 0x05C [O]
    int BoxTexture;                 // 0x060 [O]
    int MenuYOffset;                // 0x064 [O]; [U] cMenu's layout (util/menu) reads it with getDisplayHeight

#ifdef __cplusplus
    // [O] cMenuSys::getDisplayWidth / getDisplayHeight (On Tour: the screen size, height less MenuYOffset); [U] retail
    // inlines them in cMenu's layout (util/menu: 640 and 480 - MenuYOffset, the unit's view cMenuSys_2CAD58).
    int getDisplayWidth() { return 640; }
    int getDisplayHeight() { return 480 - MenuYOffset; }
#endif

    // Methods bound to their retail symbols by asm labels.
    int isActive() __asm__("cMenuSys_isActive");
    void activate(int val) __asm__("cMenuSys_activate__FPvi");
    int getKey() __asm__("cMenuSys_getKey__FPv");
    int getVKey() __asm__("cMenuSys_getVKey__FPv");
    int getAsciiKey() __asm__("cMenuSys_getAsciiKey__FPv");
    void* getLeft() __asm__("cMenuSys_getLeft__FPv");
    void* getRight() __asm__("cMenuSys_getRight__FPv");
    void* getUp() __asm__("cMenuSys_getUp__FPv");
    void* getDown() __asm__("cMenuSys_getDown__FPv");
    void render() __asm__("cMenuSys_render");
    void pushMenu(cMenu* item) __asm__("cMenuSys_pushMenu");
    void popMenu() __asm__("cMenuSys_popMenu");
    bool getLeftShift() __asm__("cMenuSys_getLeftShift");
    bool getRightShift() __asm__("cMenuSys_getRightShift");
    void init() __asm__("cMenuSys_init__FPv");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cMenuSys();
    ~cMenuSys();
    static void operator delete(void* p) { operator_delete((int*)p); }
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
