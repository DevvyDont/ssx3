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
#include "render/2drender.h"            // tRect (cExpandMenuItem::ItemRect, preRender results)

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

// ---- The row classes and derived menus (added for the readability pass) ----
//
// cMenuItem is the base of the row classes below, cMenu of cExpandMenuItemMenu and cErrorMenu. The header above
// checks cMenuItem::Flags/Arg/mVtbl only; Menu and ID are checked here.
//
// The debug-menu classes of EA's util/menu (unit src/util/menu.cpp; the inline accessors and most destructors are
// out of line in src/seg/seg_1D3510.cpp): cMenu, a menu of up to 70 rows driven by cMenuSys (util/menusys.h), and its
// row classes, all derived from cMenuItem. Derived menus (main/debugmenu, fe/debugfe, fe/festatenethelppopup,
// main/all_in_one_main, cErrorMenu below) call cMenu_cMenu, store their own vtable at 0x12C and embed their rows from
// 0x130 on. Nothing in retail allocates a menu item on the heap: every row is a member of a menu (or of an expandable
// item), so the sizes come from On Tour's DWARF ([O]) together with the retail embedding strides and the last member
// each retail constructor stores.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Bases are
// written out inline with a comment; member rows of other menu classes are embedded by value (their types are defined
// here first). g++ 2.95 puts a class's vptr after the data of the class that introduces virtuals: cMenu's at 0x12C,
// cMenuItem's at 0x10 (both exactly as in On Tour). Vtables are g++ 2.95 non-thunk tables of 8-byte entries
// {short delta; short index; fn}, entry 0 unused:
//   cMenuItem entries: 1 destructor, 2 input, 3 canHaveFocus, 4 preRender (pure; returns a tRect by value), 5 render
//     (pure), 6 gainFocus(tMIGainFocus), 7 loseFocus, 8 zeroValue; cExpandMenuItem adds 9 onCommand, 10 onChange.
//     (In On Tour's method order, counting from 0 after the destructor: gainFocus/loseFocus are slots 4/5.)
//   cMenu entries: 1 destructor, 2 input, 3 onCommand(item, arg), 4 onChange(item), 5 render, 6 onGainFocus(cMenuSys*),
//     7 onLoseFocus, 8 drawBox(rect, tARGB), 9 drawBox(rect, bool).
// Enums and bools are words in retail (On Tour's bools are 1 byte). EA's source: util/menu.cpp / menu.h (the unit's
// directory; the preview build keeps the inline methods as .gnu.linkonce.t sections, no source path string).
//
// Evidence tags:
//   [R] retail constructors / destructors (the out-of-line ones in seg/seg_1D3510 included) and retail embedding
//       strides (consecutive member constructors of the derived menus);
//   [P] the July 2003 preview's inline accessors (setCaption, setValue, setMinMax, getID, getItem, ...): every one
//       uses the same offset as retail (and as its retail copy in seg/seg_1D3510);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (member names, types and order; On Tour's offsets match retail for every class here).

CHECK_OFFSET(cMenuItem, Menu, 0x00);
CHECK_OFFSET(cMenuItem, ID, 0x08);

struct cExpandMenuItem;
struct tARGB;
struct t3Vector;

// cBoolMenuItem (0x24): an on/off row; select (or left/right when both state texts are set) flips the flag.
// [O] size 0x24; [R] the constructor's last member, Flag, is at 0x20; no retail embedding. Base cMenuItem.
struct cBoolMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cBoolMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor; [P] setCaption (sw 0x14; retail cBoolMenuItem_setCaption);
                                     //       [U] preRender/render left text
    const char* OnText;              // 0x18 [O]; [R] ctor; [U] input/preRender/render (shown when *Flag != 0)
    const char* OffText;             // 0x1C [O]; [R] ctor; [U] input/preRender/render
    int* Flag;                       // 0x20 [O] (EA: bool*; retail bools are words); [R] ctor; [P] setValue (sw 0x20; retail
                                     //       cBoolMenuItem_setValue); [U] input xors bit 0 of *Flag (lw/sw), render reads it
};

CHECK_OFFSET(cBoolMenuItem, Menu, 0x00);
CHECK_OFFSET(cBoolMenuItem, Flags, 0x04);
CHECK_OFFSET(cBoolMenuItem, ID, 0x08);
CHECK_OFFSET(cBoolMenuItem, Arg, 0x0C);
CHECK_OFFSET(cBoolMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cBoolMenuItem, Caption, 0x14);
CHECK_OFFSET(cBoolMenuItem, OnText, 0x18);
CHECK_OFFSET(cBoolMenuItem, OffText, 0x1C);
CHECK_OFFSET(cBoolMenuItem, Flag, 0x20);
CHECK_SIZE(cBoolMenuItem, 0x24);

// One {name, value} entry of a cEnumMenuItem's table (On Tour struct tCFMEnumTag, 0x8). [U] cEnumMenuItem_input/render
// read entries at 8-byte steps (name +0x0, value +0x4); setEnumTagsNullTerm stops at a null Text.
struct tCFMEnumTag {
    const char* Text;                // 0x0 [O]; [U] the name preRender/render print
    int Value;                       // 0x4 [O]; [U] compared with *Enum
};

CHECK_OFFSET(tCFMEnumTag, Text, 0x0);
CHECK_OFFSET(tCFMEnumTag, Value, 0x4);
CHECK_SIZE(tCFMEnumTag, 0x8);

// cEnumMenuItem (0x24): a row that sets an int to one of a table's values (left/right step through the table).
// [O] size 0x24; [R] the constructors' last member, Enum, is at 0x20; no retail embedding. Base cMenuItem.
struct cEnumMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entries 3/4 (onCommand/onChange)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cEnumMenuItem_vtbl (the three ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors (0 or caption); [P] setCaption/getCaption (retail
                                     //       cEnumMenuItem_setCaption/getCaption); [U] preRender/render
    int NumTags;                     // 0x18 [O]; [R] ctors; [P] setEnumTags (sw a1, 0x18; retail cEnumMenuItem_setEnumTags);
                                     //       [U] setEnumTagsNullTerm counts into it, input/preRender/render loop bound
    const tCFMEnumTag* Tags;         // 0x1C [O]; [R] cEnumMenuItem_cEnumMenuItem2; [P] setEnumTags (sw a2, 0x1C);
                                     //       [U] setEnumTagsNullTerm, input/preRender/render
    int* Enum;                       // 0x20 [O]; [R] ctors; [P] setEnum (sw 0x20; retail cEnumMenuItem_setEnum); [U] input
                                     //       writes *Enum, render compares it
};

CHECK_OFFSET(cEnumMenuItem, Menu, 0x00);
CHECK_OFFSET(cEnumMenuItem, Flags, 0x04);
CHECK_OFFSET(cEnumMenuItem, ID, 0x08);
CHECK_OFFSET(cEnumMenuItem, Arg, 0x0C);
CHECK_OFFSET(cEnumMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cEnumMenuItem, Caption, 0x14);
CHECK_OFFSET(cEnumMenuItem, NumTags, 0x18);
CHECK_OFFSET(cEnumMenuItem, Tags, 0x1C);
CHECK_OFFSET(cEnumMenuItem, Enum, 0x20);
CHECK_SIZE(cEnumMenuItem, 0x24);

// cSubMenuItem (0x1C): a row that opens another menu (select pushes Sub onto the menu system).
// [O] size 0x1C; [R] embedding stride 0x1C (festatenethelppopup, debugmenu). Base cMenuItem.
struct cSubMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input pushes onto Menu->MenuSys
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cSubMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor (cap); [U] preRender/render centre text
    cMenu* Sub;                      // 0x18 [O]; [R] ctor (sub); [P] setMenu/getSubMenu (sw/lw 0x18; retail
                                     //       cSubMenuItem_setMenu/getSubMenu); [U] input: cMenuSys_pushMenu(MenuSys, Sub)
};

CHECK_OFFSET(cSubMenuItem, Menu, 0x00);
CHECK_OFFSET(cSubMenuItem, Flags, 0x04);
CHECK_OFFSET(cSubMenuItem, ID, 0x08);
CHECK_OFFSET(cSubMenuItem, Arg, 0x0C);
CHECK_OFFSET(cSubMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cSubMenuItem, Caption, 0x14);
CHECK_OFFSET(cSubMenuItem, Sub, 0x18);
CHECK_SIZE(cSubMenuItem, 0x1C);

// cPopMenuItem (0x18): a "back" row; select pops the top menu. [O] size 0x18; [R] its constructor's last member,
// Caption, is at 0x14 and it is the last row of cErrorMenu (0x164 + 0x18 = cErrorMenu's size). Base cMenuItem.
struct cPopMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input pops Menu->MenuSys
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cPopMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor; [P] setCaption (sw 0x14; retail cPopMenuItem_setCaption);
                                     //       [U] preRender/render centre text
};

CHECK_OFFSET(cPopMenuItem, Menu, 0x00);
CHECK_OFFSET(cPopMenuItem, Flags, 0x04);
CHECK_OFFSET(cPopMenuItem, ID, 0x08);
CHECK_OFFSET(cPopMenuItem, Arg, 0x0C);
CHECK_OFFSET(cPopMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cPopMenuItem, Caption, 0x14);
CHECK_SIZE(cPopMenuItem, 0x18);

// cCommandMenuItem (0x24): a row whose select calls the menu's onCommand(item, Arg). [O] size 0x24; [R] embedding
// stride 0x24 (festatenethelppopup; all_in_one_main's lobby lists are arrays of 0x24-byte command rows). Base cMenuItem.
struct cCommandMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 3 (onCommand)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1; [U] all_in_one_main stores list indices
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cCommandMenuItem_vtbl (the three ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors, cCommandMenuItem_init; [P] setCaption/getCaption (retail
                                     //       cCommandMenuItem_setCaption/getCaption); [U] preRender/render
    const char* State;               // 0x18 [O]; [R] ctors = 0, init = 0; [P] setStateText (sw 0x18; retail
                                     //       cCommandMenuItem_setStateText); [U] preRender/render right text when set
    int CmdArg;                      // 0x1C [O] cCommandMenuItem::Arg (renamed: the base already has an Arg); [R] ctors,
                                     //       init; [U] input passes it to Menu's onCommand
    void* Data;                      // 0x20 [O]; [R] ctors = 0; [P] setData/getData (sw/lw 0x20; retail
                                     //       cCommandMenuItem_setData/getData)
};

CHECK_OFFSET(cCommandMenuItem, Menu, 0x00);
CHECK_OFFSET(cCommandMenuItem, Flags, 0x04);
CHECK_OFFSET(cCommandMenuItem, ID, 0x08);
CHECK_OFFSET(cCommandMenuItem, Arg, 0x0C);
CHECK_OFFSET(cCommandMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cCommandMenuItem, Caption, 0x14);
CHECK_OFFSET(cCommandMenuItem, State, 0x18);
CHECK_OFFSET(cCommandMenuItem, CmdArg, 0x1C);
CHECK_OFFSET(cCommandMenuItem, Data, 0x20);
CHECK_SIZE(cCommandMenuItem, 0x24);

// cCallbackMenuItem (0x1C): a row whose select calls a function. [O] size 0x1C; [R] the constructors' last member,
// Callback, is at 0x18; no retail embedding. Base cMenuItem.
struct cCallbackMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cCallbackMenuItem_vtbl (both ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors, cCallbackMenuItem_init; [P] setCaption/getCaption (retail
                                     //       cCallbackMenuItem_setCaption/getCaption); [U] preRender/render
    void (*Callback)();              // 0x18 [O]; [R] ctors, init; [U] input calls it (jalr) on select
};

CHECK_OFFSET(cCallbackMenuItem, Menu, 0x00);
CHECK_OFFSET(cCallbackMenuItem, Flags, 0x04);
CHECK_OFFSET(cCallbackMenuItem, ID, 0x08);
CHECK_OFFSET(cCallbackMenuItem, Arg, 0x0C);
CHECK_OFFSET(cCallbackMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cCallbackMenuItem, Caption, 0x14);
CHECK_OFFSET(cCallbackMenuItem, Callback, 0x18);
CHECK_SIZE(cCallbackMenuItem, 0x1C);

// cNullMenuItem (0x1C): a text-only row that never takes the focus. [O] size 0x1C; [R] embedding stride 0x1C
// (cErrorMenu 0x130 -> 0x14C, festatenethelppopup, debugmenu). Base cMenuItem.
struct cNullMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cNullMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor (text); [P] setCaption (sw 0x14; retail cNullMenuItem_setCaption);
                                     //       [U] preRender/render centre text
    const char* State;               // 0x18 [O]; [P] setStateText (sw 0x18; retail cNullMenuItem_setStateText); the ctor
                                     //       leaves it unset, preRender/render never read it
};

CHECK_OFFSET(cNullMenuItem, Menu, 0x00);
CHECK_OFFSET(cNullMenuItem, Flags, 0x04);
CHECK_OFFSET(cNullMenuItem, ID, 0x08);
CHECK_OFFSET(cNullMenuItem, Arg, 0x0C);
CHECK_OFFSET(cNullMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cNullMenuItem, Caption, 0x14);
CHECK_OFFSET(cNullMenuItem, State, 0x18);
CHECK_SIZE(cNullMenuItem, 0x1C);

// cSpaceMenuItem (0x18): an empty spacer row of a given height. [O] size 0x18; [R] embedding stride 0x18 (cErrorMenu
// 0x14C -> 0x164, festatenethelppopup, debugmenu). Base cMenuItem.
struct cSpaceMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cSpaceMenuItem_vtbl (ctor)
    int Space;                       // 0x14 [O]; [R] ctor (callers pass 10 or 30); [U] preRender returns it (as float) as
                                     //       the row height
};

CHECK_OFFSET(cSpaceMenuItem, Menu, 0x00);
CHECK_OFFSET(cSpaceMenuItem, Flags, 0x04);
CHECK_OFFSET(cSpaceMenuItem, ID, 0x08);
CHECK_OFFSET(cSpaceMenuItem, Arg, 0x0C);
CHECK_OFFSET(cSpaceMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cSpaceMenuItem, Space, 0x14);
CHECK_SIZE(cSpaceMenuItem, 0x18);

// cCharMenuItem (0x2C): a row that edits a signed byte within [Min, Max]. [O] size 0x2C; [R] the constructors' last
// member, Fraction, is at 0x28; no retail embedding. Base cMenuItem.
struct cCharMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4 (onChange)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cCharMenuItem_vtbl (the three ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors; [P] setCaption (sw 0x14; retail cCharMenuItem_setCaption);
                                     //       [U] preRender/render
    signed char* Value;              // 0x18 [O] (char*); [R] ctors; [P] setValue (sw 0x18; retail cCharMenuItem_setValue);
                                     //       [U] input/zeroValue/setMinMax write *Value (sb), render reads it (lb)
    signed char Min;                 // 0x1C [O] (char); [R] ctors (sb); [U] setMinMax (sb), zeroValue/input (lb)
    signed char Max;                 // 0x1D [O] (char); [R] ctors (sb); [U] setMinMax (sb), input (lb)
    char unk_01E[0x2];               // 0x01E padding
    int AccelCount;                  // 0x20 [O]; [R] ctors = 0; [U] input: frames the direction is held
    int AccelDir;                    // 0x24 [O]; [R] ctors = 0; [U] input: -1, 0, 1
    float Fraction;                  // 0x28 [O]; [R] ctors = 0; [U] input: the sub-step accumulator (lwc1/swc1)
};

CHECK_OFFSET(cCharMenuItem, Menu, 0x00);
CHECK_OFFSET(cCharMenuItem, Flags, 0x04);
CHECK_OFFSET(cCharMenuItem, ID, 0x08);
CHECK_OFFSET(cCharMenuItem, Arg, 0x0C);
CHECK_OFFSET(cCharMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cCharMenuItem, Caption, 0x14);
CHECK_OFFSET(cCharMenuItem, Value, 0x18);
CHECK_OFFSET(cCharMenuItem, Min, 0x1C);
CHECK_OFFSET(cCharMenuItem, Max, 0x1D);
CHECK_OFFSET(cCharMenuItem, unk_01E, 0x1E);
CHECK_OFFSET(cCharMenuItem, AccelCount, 0x20);
CHECK_OFFSET(cCharMenuItem, AccelDir, 0x24);
CHECK_OFFSET(cCharMenuItem, Fraction, 0x28);
CHECK_SIZE(cCharMenuItem, 0x2C);

// cIntMenuItem (0x30): a row that edits an int within [Min, Max]. [O] size 0x30; [R] embedding stride 0x30 (debugmenu's
// switch lists). Base cMenuItem.
struct cIntMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4 (onChange)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cIntMenuItem_vtbl (the three ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors; [P] setCaption (sw 0x14; retail cIntMenuItem_setCaption);
                                     //       [U] preRender/render
    int* Value;                      // 0x18 [O]; [R] ctors; [P] setValue (sw 0x18; retail cIntMenuItem_setValue); [U] input,
                                     //       zeroValue, setMinMax write *Value, render prints it
    int Min;                         // 0x1C [O]; [R] ctors; [U] setMinMax, zeroValue, input clamp
    int Max;                         // 0x20 [O]; [R] ctors; [U] setMinMax, input clamp
    int AccelCount;                  // 0x24 [O]; [R] ctors = 0; [U] input: frames the direction is held
    int AccelDir;                    // 0x28 [O]; [R] ctors = 0; [U] input: -1, 0, 1
    float Fraction;                  // 0x2C [O]; [R] ctors = 0; [U] input: the sub-step accumulator (lwc1/swc1)
};

CHECK_OFFSET(cIntMenuItem, Menu, 0x00);
CHECK_OFFSET(cIntMenuItem, Flags, 0x04);
CHECK_OFFSET(cIntMenuItem, ID, 0x08);
CHECK_OFFSET(cIntMenuItem, Arg, 0x0C);
CHECK_OFFSET(cIntMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cIntMenuItem, Caption, 0x14);
CHECK_OFFSET(cIntMenuItem, Value, 0x18);
CHECK_OFFSET(cIntMenuItem, Min, 0x1C);
CHECK_OFFSET(cIntMenuItem, Max, 0x20);
CHECK_OFFSET(cIntMenuItem, AccelCount, 0x24);
CHECK_OFFSET(cIntMenuItem, AccelDir, 0x28);
CHECK_OFFSET(cIntMenuItem, Fraction, 0x2C);
CHECK_SIZE(cIntMenuItem, 0x30);

// cFloatMenuItem (0x34): a row that edits a float, shown as Value * ValueScale + ValueOffset within [Min, Max].
// [O] size 0x34; [R] embedding stride 0x34 (debugmenu; cVectorMenuItem's X/Y/Z rows). Base cMenuItem.
struct cFloatMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4 (onChange)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cFloatMenuItem_vtbl (both ctors)
    const char* Caption;             // 0x14 [O]; [R] ctors; [P] setCaption (sw 0x14; retail cFloatMenuItem_setCaption);
                                     //       [U] preRender/render
    float* Value;                    // 0x18 [O]; [R] ctors; [P] setValue(float*) (sw 0x18; retail cFloatMenuItem_setValue),
                                     //       setValue(float)/getValue (lw 0x18, then *Value; retail
                                     //       cFloatMenuItem_setValue1/getValue); [U] input, zeroValue, render
    float Min;                       // 0x1C [O]; [R] ctors; [P] setMinMax (swc1 0x1C; retail cFloatMenuItem_setMinMax);
                                     //       [U] input clamp, zeroValue
    float Max;                       // 0x20 [O]; [R] ctors; [P] setMinMax (swc1 0x20); [U] input clamp
    float ValueScale;                // 0x24 [O]; [R] ctors = 1.0; [P] setValueScale/getValue (0x24; retail
                                     //       cFloatMenuItem_setValueScale/getValue); [U] input, render; cVectorMenuItem_input
    float ValueOffset;               // 0x28 [O]; [R] ctors = 0; [P] setValueScale/getValue (0x28); [U] input, render;
                                     //       cVectorMenuItem_input
    int AccelCount;                  // 0x2C [O]; [R] ctors = 0; [U] input: frames the direction is held
    int AccelDir;                    // 0x30 [O]; [R] ctors = 0; [U] input: -1, 0, 1
};

CHECK_OFFSET(cFloatMenuItem, Menu, 0x00);
CHECK_OFFSET(cFloatMenuItem, Flags, 0x04);
CHECK_OFFSET(cFloatMenuItem, ID, 0x08);
CHECK_OFFSET(cFloatMenuItem, Arg, 0x0C);
CHECK_OFFSET(cFloatMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cFloatMenuItem, Caption, 0x14);
CHECK_OFFSET(cFloatMenuItem, Value, 0x18);
CHECK_OFFSET(cFloatMenuItem, Min, 0x1C);
CHECK_OFFSET(cFloatMenuItem, Max, 0x20);
CHECK_OFFSET(cFloatMenuItem, ValueScale, 0x24);
CHECK_OFFSET(cFloatMenuItem, ValueOffset, 0x28);
CHECK_OFFSET(cFloatMenuItem, AccelCount, 0x2C);
CHECK_OFFSET(cFloatMenuItem, AccelDir, 0x30);
CHECK_SIZE(cFloatMenuItem, 0x34);

// cAngleMenuItem (0x2C): a row that edits an angle in radians (shown in degrees) within [Min, Max]. [O] size 0x2C;
// [R] the constructor's last member, AccelDir, is at 0x28; no retail embedding. Base cMenuItem.
struct cAngleMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4 (onChange)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = id
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cAngleMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor; [P] setCaption (sw 0x14; retail cAngleMenuItem_setCaption);
                                     //       [U] preRender/render
    float* Value;                    // 0x18 [O]; [R] ctor; [P] setValue (sw 0x18; retail cAngleMenuItem_setValue);
                                     //       [U] input writes *Value, render prints it * 57.2958
    float Min;                       // 0x1C [O]; [R] ctor; [P] setMinMax (swc1 0x1C; retail cAngleMenuItem_setMinMax);
                                     //       [U] input clamp
    float Max;                       // 0x20 [O]; [R] ctor; [P] setMinMax (swc1 0x20); [U] input clamp
    int AccelCount;                  // 0x24 [O]; [R] ctor = 0; [U] input: frames the direction is held
    int AccelDir;                    // 0x28 [O]; [R] ctor = 0; [U] input: -1, 0, 1
};

CHECK_OFFSET(cAngleMenuItem, Menu, 0x00);
CHECK_OFFSET(cAngleMenuItem, Flags, 0x04);
CHECK_OFFSET(cAngleMenuItem, ID, 0x08);
CHECK_OFFSET(cAngleMenuItem, Arg, 0x0C);
CHECK_OFFSET(cAngleMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cAngleMenuItem, Caption, 0x14);
CHECK_OFFSET(cAngleMenuItem, Value, 0x18);
CHECK_OFFSET(cAngleMenuItem, Min, 0x1C);
CHECK_OFFSET(cAngleMenuItem, Max, 0x20);
CHECK_OFFSET(cAngleMenuItem, AccelCount, 0x24);
CHECK_OFFSET(cAngleMenuItem, AccelDir, 0x28);
CHECK_SIZE(cAngleMenuItem, 0x2C);

// cTextMenuItem (0x2C): a text-entry row (16-byte buffer edited with the pad or a USB keyboard). [O] size 0x2C;
// [R] embedding stride 0x2C (festatenethelppopup 0x168 -> 0x194 -> 0x1C0). Base cMenuItem.
struct cTextMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input reads Menu->MenuSys's VKey/AsciiKey
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cTextMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor; [U] preRender/render left text
    char Text[16];                   // 0x18 [O]; [R] ctor strncpy(Text, name, 15), Text[15] = 0; [P] gettext (addiu 0x18),
                                     //       settext (sb 0, then copy; retail cTextMenuItem_gettext/settext); [U] input
                                     //       edits it byte by byte, preRender/render print it
    int CursorPos;                   // 0x28 [O]; [R] ctor = 0; [U] input moves it, preRender/render draw the cursor there
};

CHECK_OFFSET(cTextMenuItem, Menu, 0x00);
CHECK_OFFSET(cTextMenuItem, Flags, 0x04);
CHECK_OFFSET(cTextMenuItem, ID, 0x08);
CHECK_OFFSET(cTextMenuItem, Arg, 0x0C);
CHECK_OFFSET(cTextMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cTextMenuItem, Caption, 0x14);
CHECK_OFFSET(cTextMenuItem, Text, 0x18);
CHECK_OFFSET(cTextMenuItem, CursorPos, 0x28);
CHECK_SIZE(cTextMenuItem, 0x2C);

// cColorMenuItem (0x24): a row that edits one channel (0 A, 1 R, 2 G, 3 B; On Tour enum tCMIColor) of a tARGB.
// [O] size 0x24; [R] embedding stride 0x24 (cARGBMenuItem's Alpha..Blue at 0x5B0/0x5D4/0x5F8/0x61C). Base cMenuItem.
struct cColorMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4, render entry 9 (drawBox)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cColorMenuItem_vtbl (ctor)
    tARGB* Color;                    // 0x14 [O]; [R] ctor; [U] input/preRender/render read and write channel Adjust of it
    int Adjust;                      // 0x18 [O] (tCMIColor); [R] ctor; [U] channel index (<< 2) and name index (ColorNames)
    int AccelCount;                  // 0x1C [O]; [U] input: frames the direction is held (the ctor leaves it unset)
    int AccelDir;                    // 0x20 [O]; [U] input: -1, 0, 1 (the ctor leaves it unset)
};

CHECK_OFFSET(cColorMenuItem, Menu, 0x00);
CHECK_OFFSET(cColorMenuItem, Flags, 0x04);
CHECK_OFFSET(cColorMenuItem, ID, 0x08);
CHECK_OFFSET(cColorMenuItem, Arg, 0x0C);
CHECK_OFFSET(cColorMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cColorMenuItem, Color, 0x14);
CHECK_OFFSET(cColorMenuItem, Adjust, 0x18);
CHECK_OFFSET(cColorMenuItem, AccelCount, 0x1C);
CHECK_OFFSET(cColorMenuItem, AccelDir, 0x20);
CHECK_SIZE(cColorMenuItem, 0x24);

// cRGBTitleMenuItem (0x1C): the title row of a cARGBMenuItem (caption plus a colour swatch); never takes the focus.
// [O] size 0x1C; [R] embedded at cARGBMenuItem+0x640, which ends at 0x65C (cARGBMenuItem's embedding stride).
// Base cMenuItem.
struct cRGBTitleMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cRGBTitleMenuItem_vtbl (ctor)
    tARGB* Color;                    // 0x14 [O]; [R] ctor; [U] render draws the swatch in it
    const char* Caption;             // 0x18 [O]; [R] ctor; [P] setCaption (sw 0x18; retail cRGBTitleMenuItem_setCaption);
                                     //       [U] preRender/render left text
};

CHECK_OFFSET(cRGBTitleMenuItem, Menu, 0x00);
CHECK_OFFSET(cRGBTitleMenuItem, Flags, 0x04);
CHECK_OFFSET(cRGBTitleMenuItem, ID, 0x08);
CHECK_OFFSET(cRGBTitleMenuItem, Arg, 0x0C);
CHECK_OFFSET(cRGBTitleMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cRGBTitleMenuItem, Color, 0x14);
CHECK_OFFSET(cRGBTitleMenuItem, Caption, 0x18);
CHECK_SIZE(cRGBTitleMenuItem, 0x1C);

// cVectorTitleMenuItem (0x1C): the title row of a cVectorMenuItem (caption plus "x, y, z"); never takes the focus.
// [O] size 0x1C; [R] embedded at cVectorMenuItem+0x654, followed by cVectorMenuItem::Normalize at 0x670.
// Base cMenuItem.
struct cVectorTitleMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cVectorTitleMenuItem_vtbl (ctor)
    t3Vector* Vector;                // 0x14 [O]; [R] ctor; [U] preRender/render print its three floats
    const char* Caption;             // 0x18 [O]; [R] ctor; [P] setCaption (sw 0x18; retail cVectorTitleMenuItem_setCaption);
                                     //       [U] preRender/render left text
};

CHECK_OFFSET(cVectorTitleMenuItem, Menu, 0x00);
CHECK_OFFSET(cVectorTitleMenuItem, Flags, 0x04);
CHECK_OFFSET(cVectorTitleMenuItem, ID, 0x08);
CHECK_OFFSET(cVectorTitleMenuItem, Arg, 0x0C);
CHECK_OFFSET(cVectorTitleMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cVectorTitleMenuItem, Vector, 0x14);
CHECK_OFFSET(cVectorTitleMenuItem, Caption, 0x18);
CHECK_SIZE(cVectorTitleMenuItem, 0x1C);

// cGraphMenuItem (0x44): an editable curve of NumPoints (x, y) points with per-point axis locks and a slope rule.
// [O] size 0x44; [R] the constructor's last member, IsEditMode, is at 0x40; no retail embedding. Base cMenuItem.
struct cGraphMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x00 [O] cMenuItem::Menu; [U] input calls Menu's entry 4, render entry 9 (drawBox)
    int Flags;                       // 0x04 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x08 [O] cMenuItem::ID; [R] = -1
    void* Arg;                       // 0x0C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x10 [R] cGraphMenuItem_vtbl (ctor)
    const char* Caption;             // 0x14 [O]; [R] ctor = "Hello there!" (D_00486738); [P] setCaption (sw 0x14; retail
                                     //       cGraphMenuItem_setCaption); [U] preRender/render
    int NumPoints;                   // 0x18 [O]; [R] ctor; [U] input/render loop bound
    unsigned int XLock;              // 0x1C [O]; [R] ctor; [U] input: bit i set = point i's x is fixed
    unsigned int YLock;              // 0x20 [O]; [R] ctor; [U] input: bit i set = point i's y is fixed
    int OnPoint;                     // 0x24 [O]; [R] ctor = 0; [U] input selects it (keys 4/5), render marks it
    float* Points;                   // 0x28 [O] (t2Vector*: NumPoints {x, y} float pairs); [R] ctor; [U] input/render
    float MinPoint[2];               // 0x2C [O] t2Vector (x, y); [R] ctor copies *minPoint (sdl/sdr, 8 bytes); [U] input
                                     //       clamps, render scales by it (lwc1 0x2C/0x30)
    float MaxPoint[2];               // 0x34 [O] t2Vector (x, y); [R] ctor copies *maxPoint (sdl/sdr); [U] input, render
                                     //       (lwc1 0x34/0x38)
    int Slope;                       // 0x3C [O]; [R] ctor; [U] input: > 0 keeps y rising, < 0 falling, 0 free
    int IsEditMode;                  // 0x40 [O] (bool; retail word); [R] ctor = 0; [U] input toggles it (key 6),
                                     //       cGraphMenuItem_gainFocus clears it, preRender/render test it
};

CHECK_OFFSET(cGraphMenuItem, Menu, 0x00);
CHECK_OFFSET(cGraphMenuItem, Flags, 0x04);
CHECK_OFFSET(cGraphMenuItem, ID, 0x08);
CHECK_OFFSET(cGraphMenuItem, Arg, 0x0C);
CHECK_OFFSET(cGraphMenuItem, mVtbl, 0x10);
CHECK_OFFSET(cGraphMenuItem, Caption, 0x14);
CHECK_OFFSET(cGraphMenuItem, NumPoints, 0x18);
CHECK_OFFSET(cGraphMenuItem, XLock, 0x1C);
CHECK_OFFSET(cGraphMenuItem, YLock, 0x20);
CHECK_OFFSET(cGraphMenuItem, OnPoint, 0x24);
CHECK_OFFSET(cGraphMenuItem, Points, 0x28);
CHECK_OFFSET(cGraphMenuItem, MinPoint, 0x2C);
CHECK_OFFSET(cGraphMenuItem, MaxPoint, 0x34);
CHECK_OFFSET(cGraphMenuItem, Slope, 0x3C);
CHECK_OFFSET(cGraphMenuItem, IsEditMode, 0x40);
CHECK_SIZE(cGraphMenuItem, 0x44);

// cExpandMenuItemMenu (0x134): the inner menu of a cExpandMenuItem; it forwards onCommand / onChange to the item that
// owns it. [O] size 0x134; [R] embedded at cExpandMenuItem+0x18 with cExpandMenuItem::ItemRect at 0x14C. Base cMenu.
struct cExpandMenuItemMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu; [U] cExpandMenuItem_preRender/render/setTitle
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [U] cExpandMenuItem_input
    int TopItem;                     // 0x008 [O] cMenu::TopItem
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cExpandMenuItem_input/preRender/render/setTitle
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [U] cExpandMenuItem_preRender copies the parent menu's into it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] cExpandMenuItem ctors store cExpandMenuItemMenu_vtbl (at item+0x144)
    cExpandMenuItem* Item;           // 0x130 [O]; [R] cExpandMenuItem ctors store the item (at item+0x148); [U]
                                     //       cExpandMenuItemMenu_onCommand/onChange call its entries 9/10
};

CHECK_OFFSET(cExpandMenuItemMenu, NumItems, 0x000);
CHECK_OFFSET(cExpandMenuItemMenu, FocusItem, 0x004);
CHECK_OFFSET(cExpandMenuItemMenu, TopItem, 0x008);
CHECK_OFFSET(cExpandMenuItemMenu, Items, 0x00C);
CHECK_OFFSET(cExpandMenuItemMenu, MenuSys, 0x124);
CHECK_OFFSET(cExpandMenuItemMenu, DrawLayer, 0x128);
CHECK_OFFSET(cExpandMenuItemMenu, mVtbl, 0x12C);
CHECK_OFFSET(cExpandMenuItemMenu, Item, 0x130);
CHECK_SIZE(cExpandMenuItemMenu, 0x134);

// cExpandMenuItem (0x5AC): a row that holds its own inner menu of rows, shown while it has the focus (title first).
// [O] size 0x5AC; [R] cARGBMenuItem's and cVectorMenuItem's own members start at 0x5AC. Base cMenuItem.
struct cExpandMenuItem {
    // cMenuItem (0x00-0x13)
    cMenu* Menu;                     // 0x000 [O] cMenuItem::Menu; [U] onCommand/onChange call Menu's entries 3/4;
                                     //       gainFocus/preRender read Menu->MenuSys
    int Flags;                       // 0x004 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x008 [O] cMenuItem::ID; [R] = id or -1
    void* Arg;                       // 0x00C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x010 [R] cExpandMenuItem_vtbl (both ctors)
    cMenuItem* Title;                // 0x014 [O]; [R] ctors (title or 0); [U] setTitle, remAllItems; preRender/render compare
                                     //       rows with it
    cExpandMenuItemMenu ExpandMenu;  // 0x018 [O]; [R] ctors: cMenu_cMenu(this + 0x18), vtable at 0x144, Item at 0x148;
                                     //       destructor cMenu__cMenu(this + 0x18, 2); [U] addItem/remItem/setFocus on it
    tRect ItemRect[70];              // 0x14C [O]; [U] preRender stores each row's rect (sq/lq at 0x14C + 16i), render
                                     //       draws each row in it
};

CHECK_OFFSET(cExpandMenuItem, Menu, 0x000);
CHECK_OFFSET(cExpandMenuItem, Flags, 0x004);
CHECK_OFFSET(cExpandMenuItem, ID, 0x008);
CHECK_OFFSET(cExpandMenuItem, Arg, 0x00C);
CHECK_OFFSET(cExpandMenuItem, mVtbl, 0x010);
CHECK_OFFSET(cExpandMenuItem, Title, 0x014);
CHECK_OFFSET(cExpandMenuItem, ExpandMenu, 0x018);
CHECK_OFFSET(cExpandMenuItem, ItemRect, 0x14C);
CHECK_SIZE(cExpandMenuItem, 0x5AC);

// cARGBMenuItem (0x65C): an expandable row that edits a tARGB: a title row with a swatch and one cColorMenuItem per
// channel. [O] size 0x65C; [R] embedding stride 0x65C (debugmenu's colour lists); the destructor's last member is
// Title at 0x640. Base cExpandMenuItem (itself on cMenuItem).
struct cARGBMenuItem {
    // cExpandMenuItem (0x000-0x5AB), with its cMenuItem base (0x000-0x013)
    cMenu* Menu;                     // 0x000 [O] cMenuItem::Menu
    int Flags;                       // 0x004 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x008 [O] cMenuItem::ID; [R] = id
    void* Arg;                       // 0x00C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x010 [R] cARGBMenuItem_vtbl (ctor, after cExpandMenuItem's)
    cMenuItem* Title;                // 0x014 [O] cExpandMenuItem::Title; [R] = &this->TitleItem (the expand ctor's title)
    cExpandMenuItemMenu ExpandMenu;  // 0x018 [O] cExpandMenuItem::ExpandMenu; [R] destructor cMenu__cMenu(this + 0x18, 2)
    tRect ItemRect[70];              // 0x14C [O] cExpandMenuItem::ItemRect
    tARGB* Color;                    // 0x5AC [O]; [R] ctor
    cColorMenuItem Alpha;            // 0x5B0 [O]; [R] ctor cColorMenuItem_cColorMenuItem(this + 0x5B0, color, 0), dtor
    cColorMenuItem Red;              // 0x5D4 [O]; [R] ctor (channel 1), dtor
    cColorMenuItem Green;            // 0x5F8 [O]; [R] ctor (channel 2), dtor
    cColorMenuItem Blue;             // 0x61C [O]; [R] ctor (channel 3), dtor
    cRGBTitleMenuItem TitleItem;     // 0x640 [O] cARGBMenuItem::Title (renamed: the base already has a Title); [R] ctor,
                                     //       dtor; [P] setCaption writes TitleItem.Caption (sw 0x658; retail
                                     //       cARGBMenuItem_setCaption)
};

CHECK_OFFSET(cARGBMenuItem, Menu, 0x000);
CHECK_OFFSET(cARGBMenuItem, Flags, 0x004);
CHECK_OFFSET(cARGBMenuItem, ID, 0x008);
CHECK_OFFSET(cARGBMenuItem, Arg, 0x00C);
CHECK_OFFSET(cARGBMenuItem, mVtbl, 0x010);
CHECK_OFFSET(cARGBMenuItem, Title, 0x014);
CHECK_OFFSET(cARGBMenuItem, ExpandMenu, 0x018);
CHECK_OFFSET(cARGBMenuItem, ItemRect, 0x14C);
CHECK_OFFSET(cARGBMenuItem, Color, 0x5AC);
CHECK_OFFSET(cARGBMenuItem, Alpha, 0x5B0);
CHECK_OFFSET(cARGBMenuItem, Red, 0x5D4);
CHECK_OFFSET(cARGBMenuItem, Green, 0x5F8);
CHECK_OFFSET(cARGBMenuItem, Blue, 0x61C);
CHECK_OFFSET(cARGBMenuItem, TitleItem, 0x640);
CHECK_SIZE(cARGBMenuItem, 0x65C);

// cVectorMenuItem (0x674): an expandable row that edits a t3Vector: a title row and one cFloatMenuItem per component,
// optionally kept normalised. [O] size 0x674; [R] the constructor's last member, Normalize, is at 0x670; no retail
// embedding. Base cExpandMenuItem (itself on cMenuItem).
struct cVectorMenuItem {
    // cExpandMenuItem (0x000-0x5AB), with its cMenuItem base (0x000-0x013)
    cMenu* Menu;                     // 0x000 [O] cMenuItem::Menu
    int Flags;                       // 0x004 [O] cMenuItem::Flags; [R] = 1 (base ctor)
    int ID;                          // 0x008 [O] cMenuItem::ID; [R] = id
    void* Arg;                       // 0x00C [O] cMenuItem::Arg; [R] = 0
    void** mVtbl;                    // 0x010 [R] cVectorMenuItem_vtbl (ctor, after cExpandMenuItem's)
    cMenuItem* Title;                // 0x014 [O] cExpandMenuItem::Title; [R] = &this->TitleItem (the expand ctor's title)
    cExpandMenuItemMenu ExpandMenu;  // 0x018 [O] cExpandMenuItem::ExpandMenu; [R] destructor cMenu__cMenu(this + 0x18, 2)
    tRect ItemRect[70];              // 0x14C [O] cExpandMenuItem::ItemRect
    t3Vector* Vector;                // 0x5AC [O]; [R] ctor
    float Min;                       // 0x5B0 [O]; no retail access seen (the ctor passes min/max straight to X, Y, Z)
    float Max;                       // 0x5B4 [O]; no retail access seen
    cFloatMenuItem X;                // 0x5B8 [O]; [R] ctor cFloatMenuItem_cFloatMenuItem(this + 0x5B8, 0, &v->x, ...), dtor;
                                     //       [U] input reads Value/ValueScale/ValueOffset (0x5D0/0x5DC/0x5E0)
    cFloatMenuItem Y;                // 0x5EC [O]; [R] ctor, dtor; [U] input
    cFloatMenuItem Z;                // 0x620 [O]; [R] ctor, dtor; [U] input
    cVectorTitleMenuItem TitleItem;  // 0x654 [O] cVectorMenuItem::Title (renamed: the base already has a Title); [R] ctor,
                                     //       dtor; [P] setCaption writes TitleItem.Caption (sw 0x66C; retail
                                     //       cVectorMenuItem_setCaption)
    int Normalize;                   // 0x670 [O] (bool; retail word); [R] ctor = 0; [U] input renormalises the vector when set
};

CHECK_OFFSET(cVectorMenuItem, Menu, 0x000);
CHECK_OFFSET(cVectorMenuItem, Flags, 0x004);
CHECK_OFFSET(cVectorMenuItem, ID, 0x008);
CHECK_OFFSET(cVectorMenuItem, Arg, 0x00C);
CHECK_OFFSET(cVectorMenuItem, mVtbl, 0x010);
CHECK_OFFSET(cVectorMenuItem, Title, 0x014);
CHECK_OFFSET(cVectorMenuItem, ExpandMenu, 0x018);
CHECK_OFFSET(cVectorMenuItem, ItemRect, 0x14C);
CHECK_OFFSET(cVectorMenuItem, Vector, 0x5AC);
CHECK_OFFSET(cVectorMenuItem, Min, 0x5B0);
CHECK_OFFSET(cVectorMenuItem, Max, 0x5B4);
CHECK_OFFSET(cVectorMenuItem, X, 0x5B8);
CHECK_OFFSET(cVectorMenuItem, Y, 0x5EC);
CHECK_OFFSET(cVectorMenuItem, Z, 0x620);
CHECK_OFFSET(cVectorMenuItem, TitleItem, 0x654);
CHECK_OFFSET(cVectorMenuItem, Normalize, 0x670);
CHECK_SIZE(cVectorMenuItem, 0x674);

// cErrorMenu (0x17C): a menu that shows an error text and a button that closes it. [O] size 0x17C; [R] its rows at
// 0x130/0x14C/0x164 (ctors and the destructor, seg/seg_1D3510's cErrorMenu__cErrorMenu). Vtable cErrorMenu_vtbl. Base cMenu.
struct cErrorMenu {
    // cMenu (0x000-0x12F)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem (the three rows)
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer
    void** mVtbl;                    // 0x12C [R] both ctors store cErrorMenu_vtbl
    cNullMenuItem Error;             // 0x130 [O]; [R] ctors cNullMenuItem_cNullMenuItem(this + 0x130, text), dtor
    cSpaceMenuItem Space;            // 0x14C [O]; [R] ctors (space 10 or 30), dtor
    cPopMenuItem Ok;                 // 0x164 [O]; [R] ctors cPopMenuItem_cPopMenuItem(this + 0x164, caption), dtor
};

CHECK_OFFSET(cErrorMenu, NumItems, 0x000);
CHECK_OFFSET(cErrorMenu, FocusItem, 0x004);
CHECK_OFFSET(cErrorMenu, TopItem, 0x008);
CHECK_OFFSET(cErrorMenu, Items, 0x00C);
CHECK_OFFSET(cErrorMenu, MenuSys, 0x124);
CHECK_OFFSET(cErrorMenu, DrawLayer, 0x128);
CHECK_OFFSET(cErrorMenu, mVtbl, 0x12C);
CHECK_OFFSET(cErrorMenu, Error, 0x130);
CHECK_OFFSET(cErrorMenu, Space, 0x14C);
CHECK_OFFSET(cErrorMenu, Ok, 0x164);
CHECK_SIZE(cErrorMenu, 0x17C);

#endif /* UTIL_MENU_H */
