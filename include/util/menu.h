#ifndef UTIL_MENU_H
#define UTIL_MENU_H

// cMenu: a debug/front-end text menu of up to 70 cMenuItem entries, driven by cMenuSys (EA's util/menu, unit
// src/util/menu.cpp). Derived menus (debugfe, festatenethelppopup, all_in_one_main, ...) call cMenu_cMenu, then store
// their own vtable at 0x12C and add their items, which they embed from 0x130 on: the retail object is 0x130 bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// cMenu has no base class: its vtable pointer sits after its data (gcc 2.95 puts the vptr after the members of the
// class that introduces it), at 0x12C, exactly as in SSX On Tour's cMenu (size 0x130, members 0x000-0x128).
// Member names are EA's, from On Tour's PSP DWARF (class cMenu), placed by:
//   [P] the July 2003 preview's inline accessors, at the same offsets as retail (getNumItems = lw 0x0,
//       getItem = lw 0xC(this + 4i), getMenuSys = lw 0x124; layout_xlate sees 0x12C -> 0x12C);
//   [R] retail's own code: cMenu_cMenu (zeroes 0x0/0x4/0x8/0x124, stores _vt$5cMenu cMenu_vtbl at 0x12C);
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P], [R] or [U].
// Unnamed fields would be unk_XXX; rename only with evidence.

#include "layout.h"

struct cMenuItem;
struct cMenuSys;

struct cMenu {
    int NumItems;                    // 0x000 [P] getNumItems (lw 0x0); [R] zeroed by cMenu_cMenu; [U] item count in cMenu_addItem, cMenu_nextFocus (modulus)
    int FocusItem;                   // 0x004 [O]; [U] returned by cMenu_getFocus, stepped by cMenu_nextFocus/cMenu_prevFocus, set by cMenu_setFocus
    int TopItem;                     // 0x008 [O]; [U] first visible row: cMenu_render (cMenu::render) scrolls it towards FocusItem or resets it to 0
    cMenuItem* Items[70];            // 0x00C [P] getItem (lw 0xC(this + 4i), retail cMenu_getItem); [U] cMenu_addItem shifts and inserts here
    cMenuSys* MenuSys;               // 0x124 [P] getMenuSys (lw 0x124); [U] cMenu_onGainFocus(cMenuSys*) stores its argument,
                                     //       cMenu_onLoseFocus clears it; cMenu_render reads the display size through it
    int DrawLayer;                   // 0x128 [O] (On Tour: int, between MenuSys and the vptr); no retail access seen
    void** mVtbl;                    // 0x12C [R] _vt$5cMenu (cMenu_vtbl), stored by cMenu_cMenu and cMenu__cMenu
};

CHECK_OFFSET(cMenu, NumItems, 0x000);
CHECK_OFFSET(cMenu, FocusItem, 0x004);
CHECK_OFFSET(cMenu, TopItem, 0x008);
CHECK_OFFSET(cMenu, Items, 0x00C);
CHECK_OFFSET(cMenu, MenuSys, 0x124);
CHECK_OFFSET(cMenu, DrawLayer, 0x128);
CHECK_OFFSET(cMenu, mVtbl, 0x12C);
CHECK_SIZE(cMenu, 0x130);

// cMenuItem: the base of every debug-menu row (cSpaceMenuItem, cIntMenuItem, cFloatMenuItem, cColorMenuItem,
// cRGBTitleMenuItem, cExpandMenuItem, ...; each derived constructor stores its own vtable at +0x10). Size 0x14: On
// Tour's, and retail's methods reach the vptr at +0x10 (g++ 2.95 puts it after the members). Member names are EA's,
// from On Tour's DWARF ([O]); [R] retail's constructor cMenuItem_cMenuItem; [U] how retail code uses the field.
struct cMenuItem {
    cMenu* Menu;                     // 0x00 [O]; [U] cMenu_addItem fills it
    int Flags;                       // 0x04 [O]; [R] ctor = 1 (active)
    int ID;                          // 0x08 [O]; [R] ctor = its argument (-1 from derived items without one)
    void* Arg;                       // 0x0C [O]; [R] ctor = 0
    void** mVtbl;                    // 0x10 [R] ctor = cMenuItem_vtbl; derived ctors store their own
};

CHECK_OFFSET(cMenuItem, Flags, 0x04);
CHECK_OFFSET(cMenuItem, Arg, 0x0C);
CHECK_OFFSET(cMenuItem, mVtbl, 0x10);
CHECK_SIZE(cMenuItem, 0x14);

#endif /* UTIL_MENU_H */
