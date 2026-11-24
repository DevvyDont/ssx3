#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "main/gameviewman.h"
#include "render/font.h"
#include "bx/math3d.h"
#include "render/2drender.h"
#include "ui/uithing.h"
#include "util/menusys.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "util/menu.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/rider.h"

extern void* cMenuItem_vtbl[16];

//100%
INCLUDE_ASM("util/menu", cMenuItem_cMenuItem__FP9cMenuItemPv);
#ifdef SKIP_ASM
/**
 * Constructs a base menu item: active, with the given ID and no argument.
 *
 * Stores cMenuItem's vtable in mVtbl, Flags = 1 (active), ID = @p id and Arg = 0. Menu is left for cMenu_addItem()
 * to fill.
 *
 * @param id  the item's ID (On Tour cMenuItem::ID); derived items pass -1 for none
 * @return @p self
 */
cMenuItem* cMenuItem_cMenuItem(cMenuItem* self, void* id)
{
    self->mVtbl = cMenuItem_vtbl;
    self->Flags = 1;
    self->ID = (int)id;
    self->Arg = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem__cMenuItem);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a menu item: restores cMenuItem's vtable at +0x10, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMenuItem__cMenuItem(cMenuItem* self, int flags)
{
    self->mVtbl = cMenuItem_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_getKey(void** self);

/**
 * Returns the base menu navigation for the current key (cMenuItem_getKey): key 8 (Back) gives 1 (pop this menu), 7
 * (Cancel) gives 2 (exit all menus), 0 (Up) gives 4 (previous item), 1 (Down) gives 3 (next item), anything else 0.
 *
 * @return the navigation result (0 none, 1 pop menu, 2 exit menus, 3 next item, 4 previous item)
 */
extern "C" int cMenuItem_input(cMenuItem* self)
{
    switch (cMenuItem_getKey((void**)self)) {
    case 8:
        return 1;
    case 7:
        return 2;
    case 0:
        return 4;
    case 1:
        return 3;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_canHaveFocus);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isActive(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);

/**
 * Tells whether a menu item can take the focus.
 *
 * @return 1 when the item is active (cMenuItem_isActive) and not locked (cMenuItem_isLocked), else 0
 */
extern "C" int cMenuItem_canHaveFocus(cMenuItem* self)
{
    int canFocus = 0;
    if (cMenuItem_isActive(self) != 0) {
        canFocus = cMenuItem_isLocked(self) == 0;
    }
    return canFocus;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_gainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base menu item's gainFocus has an empty body.
 */
void cMenuItem_gainFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base menu item's loseFocus (cMenuItem_vtbl slot 7).
 */
void cMenuItem_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_isActive);
#ifdef SKIP_ASM
/**
 * Tells whether the menu item is active: bit 0 of its Flags (+0x4).
 *
 * @return 1 when the item is active, otherwise 0
 */
extern "C" int cMenuItem_isActive(cMenuItem* self)
{
    return self->Flags & 1;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_activate);
#ifdef SKIP_ASM
/**
 * Activates or deactivates a menu item: sets bit 0 of Flags (+0x4) when @p enable is nonzero, else clears it
 * (cMenuItem_isActive reads it).
 *
 * @param enable  nonzero to activate
 */
extern "C" void cMenuItem_activate(cMenuItem* self, int enable)
{
    if (enable) {
        self->Flags |= 1;
    } else {
        self->Flags &= ~1;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_isLocked);
#ifdef SKIP_ASM
/**
 * Tells whether the menu item is locked: bit 3 of its Flags (+0x4).
 *
 * A locked item still takes input but ignores value changes (the item input handlers return early).
 *
 * @return 8 when the item is locked, otherwise 0
 */
extern "C" int cMenuItem_isLocked(cMenuItem* self)
{
    return self->Flags & 8;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_lock);
#ifdef SKIP_ASM
/**
 * Locks or unlocks a menu item: sets bit 3 of Flags (+0x4) when @p enable is nonzero, else clears it
 * (cMenuItem_isLocked reads it; a locked item ignores value changes).
 *
 * @param enable  nonzero to lock
 */
extern "C" void cMenuItem_lock(cMenuItem* self, int enable) {
    int *flags = (int *)&self->Flags;

    if (enable != 0) {
        *flags |= 8;
    } else {
        *flags &= ~8;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_getKey);
#ifdef SKIP_ASM
int cMenuSys_getKey(void* self);

/**
 * Returns the menu system's current key: cMenuSys_getKey__FPv (the word at +0x48) on the item's menu's MenuSys
 * (Menu->MenuSys, +0x124 of the menu).
 *
 * @return the key (0 Up, 1 Down, 2 Left, 3 Right, 4 / 5 left / right shift, 6 Select, 7 Cancel, 8 Back, ...)
 */
extern "C" int cMenuItem_getKey(void** self)
{
    return cMenuSys_getKey(((cMenu*)*self)->MenuSys);
}
#endif

// PORT: cMenuSys_getLeft is C++ (void*); this caller passes an int handle.
void* cMenuSys_getLeft(int) __asm__("cMenuSys_getLeft__FPv");

//100%
INCLUDE_ASM("util/menu", cMenuItem_getLeft__FPv);
#ifdef SKIP_ASM
/**
 * Returns the menu system's left-direction input value: cMenuSys_getLeft__FPv (cInput_getValue of input 0x3E) on the
 * item's menu's MenuSys. The float comes back in f0; the C return type here is a placeholder.
 *
 * @return cMenuSys_getLeft__FPv's result
 */
void* cMenuItem_getLeft(void* self)
{
    return cMenuSys_getLeft(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: cMenuSys_getRight is C++ (void*); this caller passes an int handle.
void* cMenuSys_getRight(int) __asm__("cMenuSys_getRight__FPv");

//100%
INCLUDE_ASM("util/menu", cMenuItem_getRight__FPv);
#ifdef SKIP_ASM
/**
 * Returns the menu system's right-direction input value: cMenuSys_getRight__FPv (cInput_getValue of input 0x3F) on the
 * item's menu's MenuSys. The float comes back in f0; the C return type here is a placeholder.
 *
 * @return cMenuSys_getRight__FPv's result
 */
void* cMenuItem_getRight(void* self)
{
    return cMenuSys_getRight(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: cMenuSys_getUp is C++ (void*); this caller passes an int handle.
void* cMenuSys_getUp(int) __asm__("cMenuSys_getUp__FPv");

//100%
INCLUDE_ASM("util/menu", cMenuItem_getUp__FPv);
#ifdef SKIP_ASM
/**
 * Returns the menu system's up-direction input value: cMenuSys_getUp__FPv (cInput_getValue of input 0x40) on the item's
 * menu's MenuSys. The float comes back in f0; the C return type here is a placeholder.
 *
 * @return cMenuSys_getUp__FPv's result
 */
void* cMenuItem_getUp(void* self)
{
    return cMenuSys_getUp(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: cMenuSys_getDown is C++ (void*); this caller passes an int handle.
void* cMenuSys_getDown(int) __asm__("cMenuSys_getDown__FPv");

//100%
INCLUDE_ASM("util/menu", cMenuItem_getDown__FPv);
#ifdef SKIP_ASM
/**
 * Returns the menu system's down-direction input value: cMenuSys_getDown__FPv (cInput_getValue of input 0x41) on the
 * item's menu's MenuSys. The float comes back in f0; the C return type here is a placeholder.
 *
 * @return cMenuSys_getDown__FPv's result
 */
void* cMenuItem_getDown(void* self)
{
    return cMenuSys_getDown(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* cMenuSys_getLeftShift(int);

//100%
INCLUDE_ASM("util/menu", cMenuItem_getLeftShift__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the menu's left-shift key is held, asked of the item's Menu->MenuSys.
 *
 * Forwards to cMenuSys_getLeftShift (cMenuSys: cInput value 66 is nonzero) on the cMenuSys at Menu (+0x0) + 0x124. The
 * expandable items (cExpandMenuItem_input, cVectorMenuItem_input) test it together with cMenuItem_getRightShift.
 *
 * @return nonzero while left shift is held
 */
void* cMenuItem_getLeftShift(void* self)
{
    return cMenuSys_getLeftShift(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* cMenuSys_getRightShift(int);

//100%
INCLUDE_ASM("util/menu", cMenuItem_getRightShift__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the menu's right-shift key is held, asked of the item's Menu->MenuSys.
 *
 * Forwards to cMenuSys_getRightShift (cMenuSys: cInput value 67 is nonzero) on the cMenuSys at Menu (+0x0) + 0x124. The
 * expandable items test it together with cMenuItem_getLeftShift.
 *
 * @return nonzero while right shift is held
 */
void* cMenuItem_getRightShift(void* self)
{
    return cMenuSys_getRightShift(*(int*)((char*)*(void**)self + 0x124));
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_textPreRender);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);


static inline char* font_2CA4C8(char* item)
{
    return *(char**)(*(char**)(*(char**)item + 0x124) + 0x58);
}

static inline float width_2CA4C8(char* item, const char* s)
{
    char* f = font_2CA4C8(item);
    return cFont_textExtent(f, s, 0, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
}

static inline float widthOr0_2CA4C8(char* item, const char* s)
{
    float w;
    if (s)
        w = width_2CA4C8(item, s);
    else
        w = 0.0f;
    return w;
}

#define MAXF_2CA4C8(a, b) ((a) > (b) ? (a) : (b))

// PORT: the unit's callers declare cMenuItem_textPreRender as (void*, void*, int, int, void*) returning void;
// the body returns an sRect by value (hidden return pointer) from (item, left, center, right). Bound by asm label.
extern "C" tRect cMenuItem_textPreRender_impl(char* item, const char* left, const char* center, const char* right) __asm__("cMenuItem_textPreRender");

/**
 * Measures a menu row made of up to three texts (left, centre, right) and returns its rect, by value.
 *
 * Uses the menu system's font (Menu->MenuSys + 0x58): the rect starts at (0, 0) and is MaxCharHeight * Scale.y high;
 * text widths come from cFont_textExtent at the font's current Scale. With all three texts the width is the centre text
 * plus twice the wider of left and right, plus 32; otherwise it is the sum of the given texts, plus 16 when at least
 * two are given. Finally the height grows by 12 and the width becomes max(width, 100) + 32. The x scale ratio Scale.x /
 * BaseScale.x is computed but never used. The unit's callers declare it with a hidden return pointer first.
 *
 * @param item  the menu item whose menu system's font measures the texts
 * @param left  text drawn at the left edge, or 0
 * @param center  text drawn centred, or 0
 * @param right  text drawn at the right edge, or 0
 * @return the row's rect (x, y, w, h)
 */
extern "C" tRect cMenuItem_textPreRender_impl(char* item, const char* left, const char* center, const char* right)
{
    tRect rect;
    t2Vector sc;
    float zero = 0.0f;
    rect.Y = zero;
    rect.X = zero;
    cFont* font = (cFont*)font_2CA4C8(item);
    float lineHeight = (float)font->MaxCharHeight * font->BaseScale[1];
    float sx = font->Scale[0] / font->BaseScale[0];
    float sy = font->Scale[1] / font->BaseScale[1];
    sc.x = sx;
    sc.y = sy;
    rect.H = lineHeight * sy;
    if (left && center && right) {
        rect.W = width_2CA4C8(item, center) + MAXF_2CA4C8(width_2CA4C8(item, left), width_2CA4C8(item, right)) * 2.0f + 32.0f;
    } else {
        float w = widthOr0_2CA4C8(item, left);
        if (center) {
            w += width_2CA4C8(item, center);
        }
        if (right) {
            w += width_2CA4C8(item, right);
        }
        rect.W = w;
        if ((left && (center || right)) || (center && right)) {
            rect.W += 16.0f;
        }
    }
    rect.H += 12.0f;
    rect.W = (rect.W >? 100.0f) + 32.0f;
    return rect;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuItem_textRender);
#ifdef SKIP_ASM
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sVec2_2CA768 {
    float x, y;
    sVec2_2CA768(float ax, float ay) : x(ax), y(ay) {}
};


static inline char* font_2CA768(char* item)
{
    return *(char**)(*(char**)(*(char**)item + 0x124) + 0x58);
}

// PORT: the unit's callers declare cMenuItem_textRender as (void*, int, int, int, void*, char*, sColor*) returning void*;
// the body takes (item, rect, left, center, right, style, color) and returns nothing. Bound by asm label.
extern "C" void cMenuItem_textRender_impl(char* item, tRect* rect, const char* left, const char* center, const char* right, char* style, tARGB* color) __asm__("cMenuItem_textRender");

/**
 * Draws a menu row: its box in @p style, then up to three texts in colour @p color.
 *
 * Calls the menu's vtable slot 8 (cMenu's cMenu_drawBox, the box drawer) with @p rect and @p style, sets the menu
 * font's Color (+0x40) to @p color and its ShadowOffset (+0x28) to (2, 2), and draws with cFont_text at y = rect.y + 5:
 * @p left at rect.x + 16, @p center centred in the rect, @p right ending 16 before the right edge (widths from
 * cFont_textExtent). Resets ShadowOffset to (0, 0) at the end. The unit's callers declare it with other types.
 *
 * @param item  the menu item being drawn (its Menu supplies the box drawer and font)
 * @param rect  the row's rect (x, y, w, h)
 * @param left  text at the left edge, or 0
 * @param center  centred text, or 0
 * @param right  text at the right edge, or 0
 * @param style  the box colour (a tARGB) passed to the box drawer
 * @param color  the text colour (tARGB) copied into the font
 */
extern "C" void cMenuItem_textRender_impl(char* item, tRect* rect, const char* left, const char* center, const char* right, char* style, tARGB* color)
{
    char* menu = *(char**)item;
    sVEntry<void(void*, tRect*, char*)>* vt = *(sVEntry<void(void*, tRect*, char*)>**)(menu + 0x12C);
    vt[8].fn(vthis(menu, vt[8]), rect, style);
    *(tARGB*)(font_2CA768(item) + 0x40) = *color;
    char* font = font_2CA768(item);
    sVec2_2CA768 sc(2.0f, 2.0f);
    *(sVec2_2CA768*)(font + 0x28) = sc;
    if (left) {
        cFont_text(font_2CA768(item), rect->X + 16.0f, rect->Y + 6.0f - 1.0f, left);
    }
    if (center) {
        cFont* measureFont = (cFont*)font_2CA768(item);
        float w = cFont_textExtent(measureFont, center, 0, 0, measureFont->Scale[0], measureFont->Scale[1]);
        cFont_text(font_2CA768(item), rect->X + (rect->W - w) * 0.5f, rect->Y + 6.0f - 1.0f, center);
    }
    if (right) {
        cFont* measureFont = (cFont*)font_2CA768(item);
        float w = cFont_textExtent(measureFont, right, 0, 0, measureFont->Scale[0], measureFont->Scale[1]);
        cFont_text(font_2CA768(item), rect->X + (rect->W - w) - 16.0f, rect->Y + 6.0f - 1.0f, right);
    }
    *(sVec2_2CA768*)(font_2CA768(item) + 0x28) = sVec2_2CA768(0.0f, 0.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenuItem_textRender1);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern char kBoxSelectedColor[];
extern char kBoxUnselectedColor[];

struct cMenuItem_textRender1_sVec3 {
    float x;
    float y;
    float z;
};
extern cMenuItem_textRender1_sVec3 kTextSelectedColor;
extern cMenuItem_textRender1_sVec3 kTextUnselectedColor;

struct cMenuItem_textRender1_sColor {
    float a;
    float r;
    float g;
    float b;
};

extern "C" void* cMenuItem_textRender(void* self, int a1, int a2, int a3, void* a4, char* style, cMenuItem_textRender1_sColor* col);

// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the body reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a menu row in the focused or normal colours, through cMenuItem_textRender.
 *
 * When the item is its menu's focus item (cMenu_getFocusItem) it uses box colour kBoxSelectedColor and text colour kTextSelectedColor;
 * otherwise kBoxUnselectedColor and kTextUnselectedColor. The text colour is built with alpha 1 and the stored r, g, b. The unit's other
 * declarations give it four arguments; the body reads the fifth.
 *
 * @param rect  the row's rect
 * @param leftText  text at the left edge, or 0
 * @param centerText  centred text, or 0
 * @param rightText  text at the right edge, or 0
 * @return cMenuItem_textRender's return value (it returns nothing)
 */
void* cMenuItem_textRender1_5(void* self, int rect, int leftText, int centerText, void* rightText)
{
    int focused = self == cMenu_getFocusItem((cMenu*)(*(void**)self));
    char* style = focused ? kBoxSelectedColor : kBoxUnselectedColor;
    cMenuItem_textRender1_sVec3* textRgb = focused ? &kTextSelectedColor : &kTextUnselectedColor;
    cMenuItem_textRender1_sColor textColor;
    textColor.a = 1.0f;
    textColor.r = textRgb->x;
    textColor.g = textRgb->y;
    textColor.b = textRgb->z;
    return cMenuItem_textRender(self, rect, leftText, centerText, rightText, style, &textColor);
}
#endif

extern void* cMenu_vtbl[];

//100%
INCLUDE_ASM("util/menu", cMenu_cMenu__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty menu: no items, focus and top row 0, no menu system, and cMenu's vtable at +0x12C.
 *
 * Zeroes NumItems, FocusItem, TopItem and MenuSys; the Items array and DrawLayer are left as they are.
 * Derived menus store their own vtable over it afterwards.
 *
 * @return @p self
 */
void* cMenu_cMenu(void* vself)
{
    cMenu* self = (cMenu*)vself;
    int zeroValue = 0;
    *(int*)self = zeroValue;
    self->mVtbl = (void**)((int)(void*)cMenu_vtbl);
    self->FocusItem = zeroValue;
    self->TopItem = zeroValue;
    self->MenuSys = (cMenuSys*)zeroValue;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu__cMenu);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cMenu_vtbl[];

/**
 * Destroys a menu: restores cMenu's vtable at +0x12C, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMenu__cMenu(cMenu* self, int flags)
{
    self->mVtbl = cMenu_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_checkFocus);
#ifdef SKIP_ASM
class cMenu_checkFocus_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int canHaveFocus();
};

extern "C" void cMenu_nextFocus(cMenu* self);

/**
 * Moves focus off the focused item when that item can no longer hold it.
 *
 * Asks Items[FocusItem] through its virtual slot 3 (the base version: active and not locked); on 0, calls
 * cMenu_nextFocus().
 */
extern "C" void cMenu_checkFocus(cMenu* self)
{
    cMenu_checkFocus_cItem* item = (cMenu_checkFocus_cItem*)self->Items[self->FocusItem];
    if (item->canHaveFocus() == 0) {
        cMenu_nextFocus(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_nextFocus);
#ifdef SKIP_ASM
class cMenu_nextFocus_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int canHaveFocus();
    virtual void v04();
    virtual void v05();
    virtual void gainFocus(int);
    virtual void loseFocus();
};

struct cMenu_nextFocus_sMenu {
    int count;
    int cur;
    int f8;
    cMenu_nextFocus_cItem* items[1];
};

/**
 * Moves the menu's focus forward to the next item that can take it, wrapping past the last item.
 *
 * Steps FocusItem by one modulo NumItems until an item's virtual slot 3 accepts focus. If the search comes back
 * to the starting item, focus stays there and nothing is notified; otherwise the old item gets its loseFocus
 * (slot 7) and the new one gainFocus(0) (slot 6).
 *
 * @param menu  the menu to update
 */
extern "C" void cMenu_nextFocus(cMenu* menu)
{
    cMenu_nextFocus_sMenu* self = (cMenu_nextFocus_sMenu*)menu;
    int start = self->cur;
    do {
        self->cur = (self->cur + 1) % self->count;
        if (self->cur == start) {
            return;
        }
    } while (!self->items[self->cur]->canHaveFocus());
    self->items[start]->loseFocus();
    self->items[self->cur]->gainFocus(0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_prevFocus);
#ifdef SKIP_ASM
class cMenu_prevFocus_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int canHaveFocus();
    virtual void v04();
    virtual void v05();
    virtual void gainFocus(int);
    virtual void loseFocus();
};

struct cMenu_prevFocus_sMenu {
    int count;
    int cur;
    int f8;
    cMenu_prevFocus_cItem* items[1];
};

/**
 * Moves the menu's focus back to the previous item that can take it, wrapping past the first item.
 *
 * Steps FocusItem down by one modulo NumItems until an item's virtual slot 3 accepts focus. If the search comes
 * back to the starting item, focus stays there and nothing is notified; otherwise the old item gets its
 * loseFocus (slot 7) and the new one gainFocus(1) (slot 6).
 *
 * @param menu  the menu to update
 */
extern "C" void cMenu_prevFocus(cMenu* menu)
{
    cMenu_prevFocus_sMenu* self = (cMenu_prevFocus_sMenu*)menu;
    int start = self->cur;
    do {
        self->cur = (self->cur + self->count - 1) % self->count;
        if (self->cur == start) {
            return;
        }
    } while (!self->items[self->cur]->canHaveFocus());
    self->items[start]->loseFocus();
    self->items[self->cur]->gainFocus(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenu_input);
#ifdef SKIP_ASM
class cMenu_input_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual int input();
};

extern "C" void cMenu_prevFocus(cMenu* self);

/**
 * Runs one frame of menu input.
 *
 * Calls cMenu_checkFocus, then the focus item's input (virtual slot 2 of the item at Items[FocusItem]). Result 3 moves
 * the focus down (cMenu_nextFocus) and 4 up (cMenu_prevFocus), both returning 0; any other result is returned.
 *
 * @return the focus item's input result, or 0 when the focus moved
 */
extern "C" int cMenu_input(cMenu* self)
{
    cMenu_checkFocus((cMenu*)self);
    cMenu_input_cItem* item = (cMenu_input_cItem*)self->Items[self->FocusItem];
    int inputResult = item->input();
    switch (inputResult) {
    case 3:
        cMenu_nextFocus((cMenu*)self);
        return 0;
    case 4:
        cMenu_prevFocus((cMenu*)self);
        return 0;
    }
    return inputResult;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_onCommand__FPv);
#ifdef SKIP_ASM
/**
 * Handles no command for the base menu and reports it as not handled.
 *
 * @return always 0
 */
int cMenu_onCommand(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_onChange__FPv);
#ifdef SKIP_ASM
/**
 * Default notification that a menu item's value changed; does nothing.
 */
void cMenu_onChange(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_render);
#ifdef SKIP_ASM
struct sRect_2CAD58 {
    float x, y, w, h;
    sRect_2CAD58() {}
    sRect_2CAD58(float ax, float ay, float aw, float ah) { x = ax; y = ay; w = aw; h = ah; }
};

class cItem_2CAD58 {
public:
    char pad[0x10];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual sRect_2CAD58 getRect();
    virtual void render(sRect_2CAD58* r);
};


struct sMenu_2CAD58 {
    int NumItems;                   // 0x0
    int FocusItem;                  // 0x4
    int TopItem;                    // 0x8
    cItem_2CAD58* Items[70];        // 0xC
    cMenuSys* MenuSys;       // 0x124
};

static const float kScreenGapY = 53.0f;
static const float kArrowWidth = 22.0f;
static const float kArrowEdgeOffset = 32.0f;
static const float kArrowHeight = 15.0f;

// PORT: these are declared later in the unit without the self/rect parameters.
void cMenu_preRender_s(void* self) __asm__("cMenu_preRender");
void cMenu_postRender_s(void* self) __asm__("cMenu_postRender");
void cMenu_renderUpArrow_r(void* self, const sRect_2CAD58& r) __asm__("cMenu_renderUpArrow");
void cMenu_renderDownArrow_r(void* self, const sRect_2CAD58& r) __asm__("cMenu_renderDownArrow");

/**
 * Draws a menu: its background, scroll arrows and visible items.
 *
 * Asks each active item for its rect, fits as many as the display height (480 minus the menu system's offset) allows
 * from the top item, then renders them, with the up and down arrows when items are hidden above or below.
 */
extern "C" void cMenu_render(sMenu_2CAD58* self)
{
    sRect_2CAD58 total;
    sRect_2CAD58 item[70];
    int i;
    total.x = total.y = total.w = total.h = 0.0f;
    for (i = 0; i < self->NumItems; i++) {
        if (cMenuItem_isActive((cMenuItem*)self->Items[i])) {
            item[i] = self->Items[i]->getRect();
            total.h += item[i].h;
            if (item[i].w > total.w) {
                total.w = item[i].w;
            }
        } else {
            item[i].w = item[i].h = 0.0f;
        }
    }
    total.x = (self->MenuSys->getDisplayWidth() - total.w) * 0.5f;
    if (total.h > self->MenuSys->MenuYOffset + self->MenuSys->getDisplayHeight() - kScreenGapY * 2) {
        if (self->FocusItem < self->TopItem) {
            self->TopItem = self->FocusItem;
        }
        float th = 0.0f;
        for (int i = self->TopItem; i <= self->FocusItem; i++) {
            th += item[i].h;
        }
        while (self->TopItem < self->FocusItem && th > self->MenuSys->getDisplayHeight() - kScreenGapY * 2) {
            th -= item[self->TopItem].h;
            self->TopItem++;
        }
        total.y = kScreenGapY;
    } else {
        self->TopItem = 0;
        total.y = (self->MenuSys->getDisplayHeight() - total.h) * 0.5f;
    }
    cMenu_preRender_s(self);
    for (i = self->TopItem; i < self->NumItems; i++) {
        if (cMenuItem_isActive((cMenuItem*)self->Items[i])) {
            if (i != self->FocusItem && total.y + item[i].h > self->MenuSys->getDisplayHeight() - kScreenGapY) {
                break;
            }
            item[i].x = total.x;
            item[i].y = self->MenuSys->MenuYOffset + total.y;
            item[i].w = total.w;
            total.y += item[i].h;
            self->Items[i]->render(&item[i]);
        }
    }
    if (self->TopItem) {
        sRect_2CAD58 up;
        up.y = self->MenuSys->MenuYOffset + kArrowEdgeOffset;
        up.x = (self->MenuSys->getDisplayWidth() - kArrowWidth) * 0.5f;
        up.w = kArrowWidth;
        up.h = kArrowHeight;
        cMenu_renderUpArrow_r(self, up);
    }
    if (i != self->NumItems) {
        sRect_2CAD58 down;
        down.x = (self->MenuSys->getDisplayWidth() - kArrowWidth) * 0.5f;
        down.y = self->MenuSys->MenuYOffset + self->MenuSys->getDisplayHeight() - kArrowHeight - kArrowEdgeOffset;
        down.w = kArrowWidth;
        down.h = kArrowHeight;
        cMenu_renderDownArrow_r(self, down);
    }
    cMenu_postRender_s(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_preRender);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cMenu_preRender_sRS {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 22;
    int fC;
    int f10;
};

class cMenu_preRender_cCtx {
public:
    char pad0[0xE84];
    cMenu_preRender_sRS* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void PushViewport();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void SetViewportOrthographic(int a, int b, float x, float y, float w, float h, float zn, float zf);
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void PushView();
    virtual void v32();
    virtual void v33();
    virtual void SetView1(void* p);
};

// Typed view of the render context (the unit declares GrMan with another class later).
extern t4Matrix t4mat_Identity;

static inline void cMenu_preRender_setF8_5(cMenu_preRender_sRS* rs, int v) { rs->f8_5 = v; }
static inline void cMenu_preRender_setF4_23(cMenu_preRender_sRS* rs, int v) { rs->f4_23 = v; }
static inline void cMenu_preRender_setF4_20(cMenu_preRender_sRS* rs, int v) { rs->f4_20 = v; }
static inline void cMenu_preRender_setF4_12(cMenu_preRender_sRS* rs, int v) { rs->f4_12 = v; }
static inline void cMenu_preRender_setF4_2(cMenu_preRender_sRS* rs, int v) { rs->f4_2 = v; }

/**
 * Sets up the 2D state for drawing a menu: pushes a draw state, the viewport and the view, then a 640x480 screen.
 *
 * Copies GrMan's current draw state (m_pCurrentDrawState) to the next stack entry and moves to it, calls GrMan's vtable
 * slots 21 (cPS2GraphicsMan_PushViewport) and 31 (cPS2GraphicsMan_PushView), sets bit fields of the new draw state
 * (word +0x8 bits 5-9 = 17, word +0x4: bits 23-24 = 2, bits 20-21 = 3, bits 12-19 = 20, bits 2-6 = 5), then calls slot
 * 26 (cPS2GraphicsMan_SetViewportOrthographic) with (0, 0, 0, 0, 640, 480, 0, 1) and slot 34 (cPS2GraphicsMan_SetView1) with t4mat_Identity. cMenu_render
 * calls it before drawing and cMenu_postRender undoes it.
 */
extern "C" void cMenu_preRender()
{
    cPS2GraphicsMan* ctx = GrMan;
    ((cMenu_preRender_cCtx*)ctx)->top[1] = ((cMenu_preRender_cCtx*)ctx)->top[0];
    ((cMenu_preRender_cCtx*)ctx)->top++;
    GrMan->PushViewport();
    GrMan->PushView();
    cMenu_preRender_setF8_5(((cMenu_preRender_cCtx*)GrMan)->top, 17);
    cMenu_preRender_setF4_23(((cMenu_preRender_cCtx*)GrMan)->top, 2);
    cMenu_preRender_setF4_20(((cMenu_preRender_cCtx*)GrMan)->top, 3);
    cMenu_preRender_setF4_12(((cMenu_preRender_cCtx*)GrMan)->top, 20);
    cMenu_preRender_setF4_2(((cMenu_preRender_cCtx*)GrMan)->top, 5);
    GrMan->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0, 0);
    GrMan->SetView1(t4mat_Identity);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_postRender);
#ifdef SKIP_ASM

/**
 * Undoes cMenu_preRender: pops the view and the viewport and drops the pushed draw state.
 *
 * Calls GrMan's vtable slots 32 (cPS2GraphicsMan_PopView) and 22 (cPS2GraphicsMan_PopViewport), then moves
 * m_pCurrentDrawState back one 20-byte entry. cMenu_render calls it last.
 */
extern "C" void cMenu_postRender()
{
    GrMan->PopView();
    GrMan->PopViewport();
    *(int*)&GrMan->m_pCurrentDrawState = (int)GrMan->m_pCurrentDrawState - 20;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB350);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002CB350_sVec4 {
    float x, y, z, w;
    func_002CB350_sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct func_002CB350_sVtx {
    float u, v;
    char pad[0x18];
    func_002CB350_sVec4 pos;
};

// Typed view of the render context (the unit declares GrMan with another class later).

/**
 * Draws one textured screen quad: rect @p screenRect with texture window @p texRect, as a 4-vertex strip.
 *
 * Fills the positions (x, y, 0, 1) of the four corners (top-left, top-right, bottom-left, bottom-right) of @p screenRect and
 * their u, v from @p texRect, leaving the vertices' colours as the caller set them, and sends them through GrMan's vtable
 * slot 71 (cPS2GraphicsMan_RenderTriStrip) with flags 0. cMenu_drawBox draws the nine pieces of a menu box with it.
 *
 * @param quadVerts  four vertices (colour already set) to fill and draw
 * @param screenRect  screen rect (x, y, w, h)
 * @param texRect  texture rect (u, v, du, dv)
 */
extern "C" void func_002CB350(func_002CB350_sVtx* quadVerts, float* screenRect, float* texRect)
{
    quadVerts[0].pos = func_002CB350_sVec4(screenRect[0], screenRect[1], 0.0f, 1.0f);
    quadVerts[0].u = texRect[0];
    quadVerts[0].v = texRect[1];
    quadVerts[1].pos = func_002CB350_sVec4(screenRect[0] + screenRect[2], screenRect[1], 0.0f, 1.0f);
    quadVerts[1].u = texRect[0] + texRect[2];
    quadVerts[1].v = texRect[1];
    quadVerts[2].pos = func_002CB350_sVec4(screenRect[0], screenRect[1] + screenRect[3], 0.0f, 1.0f);
    quadVerts[2].u = texRect[0];
    quadVerts[2].v = texRect[1] + texRect[3];
    quadVerts[3].pos = func_002CB350_sVec4(screenRect[0] + screenRect[2], screenRect[1] + screenRect[3], 0.0f, 1.0f);
    quadVerts[3].u = texRect[0] + texRect[2];
    quadVerts[3].v = texRect[1] + texRect[3];
    GrMan->RenderTriStrip(4, (tBxVertex*)quadVerts, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_drawBox);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVtx_2CB498 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    float pos[4] __attribute__((aligned(16)));
    sVtx_2CB498() {}
};

struct sRS_2CB498 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    short f10;
};

class cCtx_2CB498 {
public:
    char pad0[0xE84];
    sRS_2CB498* top;
};


static inline int cMenu_drawBox_getF8_5(sRS_2CB498* rs)
{
    return (unsigned int)(rs->f8 & 0x3E0) >> 5;
}

static inline void cMenu_drawBox_setF8_5(sRS_2CB498* rs, int v)
{
    if (v < 0) v = 0;
    rs->f8 = (rs->f8 & ~0x3E0) | ((v << 5) & 0x3E0);
}

/**
 * Draws a menu box in colour @p col: the menu system's box texture stretched as a nine-piece frame over @p rect.
 *
 * Sets all four vertices' colour to @p col (each component * 128) with q = 1, lowers the current draw state's 5-bit
 * field at word +0x8 bits 5-9 by 2 (clamped at 0), sets word +0x4 bits 2-6 to 5 and the texture id (+0x10) to
 * MenuSys->BoxTexture (+0x60). Then draws, with func_002CB350, the four 5x5 corners (texture quarters at the corners),
 * the top and bottom 5-high edges, the left and right 5-wide edges and the centre, using the texture's outer 0.25 for
 * the borders and its middle 0.5 for the rest. Restores the bit field at the end. cMenu's vtable slot 8.
 *
 * @param rect  the box (x, y, w, h)
 * @param col  the box colour, a tARGB (a, r, g, b)
 */
extern "C" void cMenu_drawBox(cMenu* self, float* rect, cMenuItem_textRender1_sColor* col)
{
    sVtx_2CB498 quadVerts[4];
    int i;
    for (i = 0; i < 4; i++) {
        quadVerts[i].a = (int)(col->a * 128.0f);
        quadVerts[i].r = (int)(col->r * 128.0f);
        quadVerts[i].g = (int)(col->g * 128.0f);
        quadVerts[i].b = (int)(col->b * 128.0f);
        quadVerts[i].q = 1.0f;
    }
    int old = cMenu_drawBox_getF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState));
    cMenu_drawBox_setF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState), old - 2);
    ((sRS_2CB498*)GrMan->m_pCurrentDrawState)->f4_2 = 5;
    int id = self->MenuSys->BoxTexture;
    ((sRS_2CB498*)GrMan->m_pCurrentDrawState)->f10 = id;
    float pieceRect[4];
    float pieceUv[4];
    float left = rect[0] + 5.0f;
    float top = rect[1] + 5.0f;
    float right = rect[0] + rect[2] - 5.0f;
    float bot = rect[1] + rect[3] - 5.0f;
    float w = rect[2] - 10.0f;
    float innerHeight = rect[3] - 10.0f;
    pieceRect[0] = rect[0]; pieceRect[1] = rect[1]; pieceRect[2] = 5.0f; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.0f; pieceUv[1] = 0.0f; pieceUv[2] = 0.25f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = right; pieceRect[1] = rect[1]; pieceRect[2] = 5.0f; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.75f; pieceUv[1] = 0.0f; pieceUv[2] = 0.25f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = right; pieceRect[1] = bot; pieceRect[2] = 5.0f; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.75f; pieceUv[1] = 0.75f; pieceUv[2] = 0.25f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = rect[0]; pieceRect[1] = bot; pieceRect[2] = 5.0f; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.0f; pieceUv[1] = 0.75f; pieceUv[2] = 0.25f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = left; pieceRect[1] = rect[1]; pieceRect[2] = w; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.25f; pieceUv[1] = 0.0f; pieceUv[2] = 0.5f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = left; pieceRect[1] = bot; pieceRect[2] = w; pieceRect[3] = 5.0f;
    pieceUv[0] = 0.25f; pieceUv[1] = 0.75f; pieceUv[2] = 0.5f; pieceUv[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = rect[0]; pieceRect[1] = top; pieceRect[2] = 5.0f; pieceRect[3] = innerHeight;
    pieceUv[0] = 0.0f; pieceUv[1] = 0.25f; pieceUv[2] = 0.25f; pieceUv[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = right; pieceRect[1] = top; pieceRect[2] = 5.0f; pieceRect[3] = innerHeight;
    pieceUv[0] = 0.75f; pieceUv[1] = 0.25f; pieceUv[2] = 0.25f; pieceUv[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    pieceRect[0] = left; pieceRect[1] = top; pieceRect[2] = w; pieceRect[3] = innerHeight;
    pieceUv[0] = 0.25f; pieceUv[1] = 0.25f; pieceUv[2] = 0.5f; pieceUv[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)quadVerts, pieceRect, pieceUv);
    cMenu_drawBox_setF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState), old);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_drawBox1);
#ifdef SKIP_ASM
extern char kBoxSelectedColor[];
extern char kBoxUnselectedColor[];

typedef void (*fn2CB880)(void*, void*, char*);

struct sVEntry2CB880 {
    short delta;
    short index;
    fn2CB880 fn;
};

/**
 * Draws a menu box in the selected or normal colour: calls the menu's vtable slot 8 (cMenu_drawBox for cMenu) with
 * @p rect and kBoxSelectedColor when @p on is nonzero, else kBoxUnselectedColor.
 *
 * cMenu's vtable slot 9.
 *
 * @param rect  the box rect
 * @param on  nonzero for the selected colour
 */
extern "C" void cMenu_drawBox1(cMenu* self, void* rect, int on)
{
    sVEntry2CB880* vt = (sVEntry2CB880*)self->mVtbl;
    fn2CB880* drawBoxFn = &vt[8].fn;
    (*drawBoxFn)(vthis(self, vt[8]), rect, on ? kBoxSelectedColor : kBoxUnselectedColor);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_renderUpArrow);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_2CB8C8 {
    float x, y, z, w;
    sVec4_2CB8C8() {}
    sVec4_2CB8C8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sVtx_2CB8C8 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sVec4_2CB8C8 pos;
    sVtx_2CB8C8() {}
};

struct sRS_2CB8C8 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    short f10;
};

class cCtx_2CB8C8 {
public:
    char pad0[0xE84];
    sRS_2CB8C8* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, void* verts, int flags);
};


/**
 * Draws the menu's "more above" arrow: a box in the selected colour with an upward chevron in it.
 *
 * Draws the box through the menu's vtable slot 8 with colour kBoxSelectedColor, sets the current draw state's texture id
 * (+0x10) to -1 (untextured) and word +0x4 bits 2-6 to 1, and draws a 6-vertex strip in colour kTextSelectedColor (alpha 0x80):
 * a 3-unit-thick chevron from the bottom corners (rect inset 6 at the sides, 4 at the bottom) up to the middle of the
 * top (inset 3), via GrMan's vtable slot 71 (cPS2GraphicsMan_RenderTriStrip). cMenu_render calls it when TopItem is not
 * 0.
 *
 * @param rect  the arrow's rect (x, y, w, h)
 */
extern "C" void cMenu_renderUpArrow(cMenu* self, float* rect)
{
    sVEntry2CB880* vt = (sVEntry2CB880*)self->mVtbl;
    vt[8].fn(vthis(self, vt[8]), rect, kBoxSelectedColor);
    *(short*)((char*)((cCtx_2CB8C8*)GrMan)->top + 0x10) = -1;
    ((cCtx_2CB8C8*)GrMan)->top->f4_2 = 1;
    sVtx_2CB8C8 arrowVerts[6];
    int i;
    for (i = 0; i < 6; i++) {
        arrowVerts[i].a = 0x80;
        arrowVerts[i].r = (int)(kTextSelectedColor.x * 128.0f);
        arrowVerts[i].g = (int)(kTextSelectedColor.y * 128.0f);
        arrowVerts[i].b = (int)(kTextSelectedColor.z * 128.0f);
        arrowVerts[i].u = 0;
        arrowVerts[i].v = 0;
        arrowVerts[i].q = 1.0f;
    }
    float bot = rect[1] + rect[3] - 4.0f;
    float left = rect[0] + 6.0f;
    float right = rect[0] + rect[2] - 6.0f;
    float mid = rect[0] + rect[2] * 0.5f;
    float top = rect[1] + 3.0f;
    sVec4_2CB8C8 vertexPos;
    vertexPos.x = left; vertexPos.y = bot; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[0].pos = vertexPos;
    vertexPos.x = left; vertexPos.y = bot - 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[1].pos = vertexPos;
    vertexPos.x = mid; vertexPos.y = top + 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[2].pos = vertexPos;
    vertexPos.x = mid; vertexPos.y = top; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[3].pos = vertexPos;
    vertexPos.x = right; vertexPos.y = bot; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[4].pos = vertexPos;
    vertexPos.x = right; vertexPos.y = bot - 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[5].pos = vertexPos;
    GrMan->RenderTriStrip(6, (tBxVertex*)arrowVerts, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_renderDownArrow);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_2CBAC8 {
    float x, y, z, w;
    sVec4_2CBAC8() {}
    sVec4_2CBAC8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sVtx_2CBAC8 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sVec4_2CBAC8 pos;
    sVtx_2CBAC8() {}
};

struct sRS_2CBAC8 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    short f10;
};

class cCtx_2CBAC8 {
public:
    char pad0[0xE84];
    sRS_2CBAC8* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, void* verts, int flags);
};


/**
 * Draws the menu's "more below" arrow: a box in the selected colour with a downward chevron in it.
 *
 * The mirror of cMenu_renderUpArrow: the box through the menu's vtable slot 8 with colour kBoxSelectedColor, then an untextured
 * 6-vertex strip in colour kTextSelectedColor from the top corners (rect inset 6 at the sides, 3 at the top) down to the middle
 * of the bottom (inset 4), via GrMan's vtable slot 71. cMenu_render calls it when items are hidden below.
 *
 * @param rect  the arrow's rect (x, y, w, h)
 */
extern "C" void cMenu_renderDownArrow(cMenu* self, float* rect)
{
    sVEntry2CB880* vt = (sVEntry2CB880*)self->mVtbl;
    vt[8].fn(vthis(self, vt[8]), rect, kBoxSelectedColor);
    *(short*)((char*)((cCtx_2CBAC8*)GrMan)->top + 0x10) = -1;
    ((cCtx_2CBAC8*)GrMan)->top->f4_2 = 1;
    sVtx_2CBAC8 arrowVerts[6];
    int i;
    for (i = 0; i < 6; i++) {
        arrowVerts[i].a = 0x80;
        arrowVerts[i].r = (int)(kTextSelectedColor.x * 128.0f);
        arrowVerts[i].g = (int)(kTextSelectedColor.y * 128.0f);
        arrowVerts[i].b = (int)(kTextSelectedColor.z * 128.0f);
        arrowVerts[i].u = 0;
        arrowVerts[i].v = 0;
        arrowVerts[i].q = 1.0f;
    }
    float top = rect[1] + 3.0f;
    float left = rect[0] + 6.0f;
    float right = rect[0] + rect[2] - 6.0f;
    float mid = rect[0] + rect[2] * 0.5f;
    float bot = rect[1] + rect[3] - 4.0f;
    sVec4_2CBAC8 vertexPos;
    vertexPos.x = left; vertexPos.y = top; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[0].pos = vertexPos;
    vertexPos.x = left; vertexPos.y = top + 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[1].pos = vertexPos;
    vertexPos.x = mid; vertexPos.y = bot - 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[2].pos = vertexPos;
    vertexPos.x = mid; vertexPos.y = bot; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[3].pos = vertexPos;
    vertexPos.x = right; vertexPos.y = top; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[4].pos = vertexPos;
    vertexPos.x = right; vertexPos.y = top + 3.0f; vertexPos.z = 0.0f; vertexPos.w = 1.0f;
    arrowVerts[5].pos = vertexPos;
    GrMan->RenderTriStrip(6, (tBxVertex*)arrowVerts, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_addItem);
#ifdef SKIP_ASM
struct cMenuList {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    void* items[70];    // 0xC
    int wrap;           // 0x124
};

/**
 * Inserts an item into the menu at an index, shifting later items up.
 *
 * Stores @p menu in the item's first word, then inserts the item at @p index (or appends it when @p index is
 * negative). If menu->wrap is set, the menu was not empty and the selection is at or after the insertion point,
 * the selected index moves up by one. There is no check against the 70-item capacity.
 *
 * @param menu   menu to add to
 * @param item   item to insert; its first word is set to @p menu
 * @param index  position to insert at, or negative to append
 */
extern "C" void cMenu_addItem(cMenuList* menu, void** item, int index)
{
    *item = menu;
    if (index < 0) {
        index = menu->count;
    }
    for (int i = menu->count; index < i; i--) {
        menu->items[i] = menu->items[i - 1];
    }
    menu->items[index] = item;
    if (menu->wrap != 0 && menu->selected >= index && menu->count != 0) {
        menu->selected++;
    }
    menu->count++;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_remItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMenu_checkFocus(cMenu* self);

void cMenu_remItem_impl(cMenuList* list, int index) __asm__("cMenu_remItem");
/**
 * Removes the item at @p index from a menu, closing the gap in Items and keeping the focus valid.
 *
 * Shifts the later entries of Items down one and decrements NumItems; moves FocusItem back one when the removed item
 * was before it and clamps it to the last item; when the removed index is the focus, re-checks it with
 * cMenu_checkFocus; finally clamps FocusItem at 0. cExpandMenuItem_remItem and cExpandMenuItem_setTitle call it on a cExpandMenuItem's
 * embedded menu (+0x18).
 *
 * @param list  the menu (NumItems, FocusItem, Items)
 * @param index  the position to remove
 */
void cMenu_remItem_impl(cMenuList* list, int index)
{
    for (int i = index; i < list->count - 1; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
    if (index < list->selected) {
        list->selected--;
    }
    if (list->selected >= list->count) {
        list->selected = list->count - 1;
    }
    if (index == list->selected) {
        cMenu_checkFocus((cMenu*)list);
    }
    if (list->selected < 0) {
        list->selected = 0;
    }
}
#endif
//100%
INCLUDE_ASM("util/menu", cMenu_remAllItems);
#ifdef SKIP_ASM
/**
 * Empties a menu: sets FocusItem (+0x4) and NumItems (+0x0) to 0 (TopItem and the Items pointers stay).
 */
extern "C" void cMenu_remAllItems(cMenu* self) {
    self->FocusItem = 0;
    self->NumItems = 0;
}
#endif
//100%
INCLUDE_ASM("util/menu", cMenu_sortItems);
#ifdef SKIP_ASM
extern int (*D_004A54BC)(int, int);
extern "C" void qsort(void* base, int n, int size, int (*cmp)(const void*, const void*));
extern "C" int func_002D4008(const void*, const void*);

/**
 * Sorts a menu's items with @p cmp, then re-checks the focus.
 *
 * Stores @p cmp in D_004A54BC for the trampoline func_002D4008, sorts the NumItems pointers of Items (+0xC) with
 * qsort (a qsort: base, count, element size 4, compare), clears D_004A54BC and calls cMenu_checkFocus.
 * FocusItem keeps its index, so the focus moves to whichever item lands there.
 *
 * @param cmp  compares two items; called through func_002D4008
 */
extern "C" void cMenu_sortItems(cMenu* self, int (*cmp)(int, int))
{
    D_004A54BC = cmp;
    qsort(&self->Items[0], *(int*)self, 4, func_002D4008);
    D_004A54BC = 0;
    cMenu_checkFocus((cMenu*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenu_setFocus);
#ifdef SKIP_ASM
class cMenu_setFocus_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void gainFocus(int);
    virtual void loseFocus();
};

struct cMenu_setFocus_sMenu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    cMenu_setFocus_cItem* items[70];    // 0xC
    int wrap;           // 0x124
};

/**
 * Gives focus to the item at an index, notifying the old and new items.
 *
 * A negative index counts from the end (NumItems is added once). Nothing happens when the index is already
 * focused; otherwise the old item gets loseFocus (slot 7), FocusItem is set, the new item gets gainFocus(2)
 * (slot 6), and cMenu_checkFocus() moves on if that item cannot hold focus.
 *
 * @param index  index of the item to focus; -1 is the last item
 */
extern "C" void cMenu_setFocus(cMenu_setFocus_sMenu* self, int index)
{
    if (index < 0) {
        index += self->count;
    }
    if (index != self->selected) {
        self->items[self->selected]->loseFocus();
        self->selected = index;
        self->items[index]->gainFocus(2);
        cMenu_checkFocus((cMenu*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenu_getFocus);
#ifdef SKIP_ASM
extern "C" void cMenu_checkFocus(cMenu* self);

/**
 * Returns the index of the menu's focused item after checking that the item can hold focus.
 *
 * cMenu_checkFocus() first moves focus on with cMenu_nextFocus() when the focused item's third virtual returns
 * 0.
 *
 * @return FocusItem after the check
 */
extern "C" int cMenu_getFocus(cMenu* self)
{
    cMenu_checkFocus(self);
    return self->FocusItem;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenu_getFocusItem);
#ifdef SKIP_ASM
/**
 * Returns the menu's focused item after checking that it can hold focus.
 *
 * @return Items[cMenu_getFocus()], the focused cMenuItem
 */
extern "C" void* cMenu_getFocusItem(cMenu* self)
{
    return (void*)self->Items[cMenu_getFocus(self)];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenu_onGainFocus);
#ifdef SKIP_ASM
class cMenu_onGainFocus_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void gainFocus(int);
};

struct cMenu_onGainFocus_sMenu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    cMenu_onGainFocus_cItem* items[70];    // 0xC
    int wrap;           // 0x124
};

/**
 * Activates the menu: records its owner, clamps the focus index and tells the focused item it has focus.
 *
 * Clamps FocusItem to NumItems - 1, calls that item's gainFocus(2) (slot 6), then cMenu_checkFocus().
 *
 * @param menu     the menu (this)
 * @param wrap  the owning cMenuSys (On Tour's onGainFocus parent), stored in MenuSys (+0x124)
 */
extern "C" void cMenu_onGainFocus(cMenu* menu, int wrap)
{
    cMenu_onGainFocus_sMenu* self = (cMenu_onGainFocus_sMenu*)menu;
    self->wrap = wrap;
    if (self->selected >= self->count) {
        self->selected = self->count - 1;
    }
    self->items[self->selected]->gainFocus(2);
    cMenu_checkFocus((cMenu*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenu_onLoseFocus);
#ifdef SKIP_ASM
struct sVEntry2CBFD0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct s2CBFD0Menu {
    int count;          // 0x0
    int selected;       // 0x4
    int unk8;           // 0x8
    void* items[70];    // 0xC
    int wrap;           // 0x124
};

/**
 * Deactivates the menu: tells its focused item it lost focus, then forgets the owning menu system.
 *
 * Calls Items[FocusItem]'s loseFocus (cMenuItem vtable slot 7, empty in the base item) and clears MenuSys
 * (+0x124, which the local struct calls wrap). cMenuSys calls it on the menu a newly pushed menu covers.
 *
 * @param menu  the menu (this)
 */
extern "C" void cMenu_onLoseFocus(cMenu* menu)
{
    s2CBFD0Menu* self = (s2CBFD0Menu*)menu;
    cMenuItem* item = (cMenuItem*)self->items[self->selected];
    sVEntry2CBFD0* vt = (sVEntry2CBFD0*)item->mVtbl;
    vt[7].fn(vthis(item, vt[7]));
    self->wrap = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_cMenuSys);
#ifdef SKIP_ASM
void cMenuSys_init(void* self);

/**
 * Constructs the debug menu system.
 *
 * Clears the first word and calls cMenuSys_init__FPv to reset the rest.
 *
 * @return this
 */
extern "C" void* cMenuSys_cMenuSys(cMenuSys* self)
{
    *(int*)self = 0;
    cMenuSys_init(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys__cMenuSys);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys the debug menu system.
 *
 * Has no members to release; frees the object with operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cMenuSys__cMenuSys(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_init__FPv);
#ifdef SKIP_ASM
/**
 * Resets the debug menu system's state.
 *
 * Zeroes +0x0, +0x54, +0x58 and +0x64, sets +0x48 to 11 and +0x4C and +0x50 to -1.
 */
void cMenuSys_init(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    self->Font = 0;
    self->Active = 0;
    *(int*)self = 0;
    self->MenuYOffset = 0;
    self->VKey = -1;
    self->Key = 11;
    self->AsciiKey = -1;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_isActive);
#ifdef SKIP_ASM
/**
 * Tells whether the debug menu system is open.
 *
 * @return 1 when +0x0 and +0x54 are both non-zero, else 0
 */
extern "C" int cMenuSys_isActive(cMenuSys* self)
{
    int active = 0;
    if (*(int*)self != 0) {
        active = self->Active != 0;
    }
    return active;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_activate__FPvi);
#ifdef SKIP_ASM
/**
 * Switches the menu system on or off: stores @p val in its active flag (+0x54).
 *
 * cMenuSys_pushMenu sets it when a menu is pushed and cMenuSys_popMenu clears it when the last one is popped;
 * cMenuSys_isActive reads it.
 *
 * @param val  nonzero to activate
 */
void cMenuSys_activate(void* vself, int val)
{
    cMenuSys* self = (cMenuSys*)vself;
    self->Active = val;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_input);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
float cInput_getValue_f(void* pad, int axis) __asm__("cInput_getValue");
extern "C" int func_00266BA8(void* kb, int a1, int* key, int* chr);
extern "C" void cMenuSys_popMenu(cMenuSys* p);
extern void* Keyboard;

struct sVE2CC0C0 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMenu2CC0C0 {
    int depth;              // 0x00
    char* items[17];        // 0x04
    int input;              // 0x48
    int key;                // 0x4C
    int chr;                // 0x50
    char pad54[0x8];
    int** pad;              // 0x5C
};

static inline int PadValid2CC0C0(int** pad)
{
    return *pad != 0 && **pad != 0;
}

/**
 * Reads this frame's menu key and lets the top menu handle it; pops one menu or all of them as the menu asks.
 *
 * Does nothing with no menu open (MenuDepth +0x0 is 0) or no pad (the cInput at +0x5C, or its first word, is 0). Stores
 * the pad's menu key (cInput_getValue 61) in Key (+0x48) and sets VKey (+0x4C) and AsciiKey (+0x50) to -1. When the pad
 * has no key (-1) and a USB keyboard exists (Keyboard), func_00266BA8 reads its key into VKey and AsciiKey: an ASCII
 * value below 0x20 becomes -1, and the virtual keys up (38), down (40), left (37), right (39), insert (45), delete
 * (46), enter (13) and escape (27) become menu keys 0, 1, 2, 3, 9, 10, 6 and 7. Then calls the top menu's vtable slot 2
 * (input): result 1 pops one menu (cMenuSys_popMenu), 2 pops until none is left.
 */
extern "C" void cMenuSys_input(sMenu2CC0C0* self)
{
    if (self->depth == 0) {
        return;
    }
    if (!PadValid2CC0C0(self->pad)) {
        return;
    }
    int input = (int)cInput_getValue_f(self->pad, 61);
    int* key = &self->key;
    int* chr = &self->chr;
    *key = -1;
    *chr = -1;
    self->input = input;
    if (input == -1 && Keyboard != 0 && func_00266BA8(Keyboard, 0, key, chr) != 0) {
        if (self->chr < 0x20) {
            self->chr = input;
        }
        switch (self->key) {
        case 38: self->input = 0; break;
        case 40: self->input = 1; break;
        case 37: self->input = 2; break;
        case 39: self->input = 3; break;
        case 45: self->input = 9; break;
        case 46: self->input = 10; break;
        case 13: self->input = 6; break;
        case 27: self->input = 7; break;
        }
    }
    char* item = self->items[self->depth - 1];
    sVE2CC0C0* vt = *(sVE2CC0C0**)(item + 0x12C);
    switch (vt[2].fn(vthis(item, vt[2]))) {
    case 1:
        cMenuSys_popMenu((cMenuSys*)self);
        break;
    case 2:
        while (self->depth != 0) {
            cMenuSys_popMenu((cMenuSys*)self);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getKey__FPv);
#ifdef SKIP_ASM
/**
 * Returns the menu key read this frame (Key, +0x48; -1 for none).
 *
 * @return the menu key code (0 up, 1 down, 2 left, 3 right, 6 select, 7 back, ...)
 */
int cMenuSys_getKey(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return self->Key;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getVKey__FPv);
#ifdef SKIP_ASM
/**
 * Returns the keyboard's virtual key read this frame (VKey, +0x4C; -1 for none).
 *
 * @return the virtual key code
 */
int cMenuSys_getVKey(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return self->VKey;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getAsciiKey__FPv);
#ifdef SKIP_ASM
/**
 * Returns the keyboard's ASCII key read this frame (AsciiKey, +0x50; -1 for none or a control character).
 *
 * @return the character code
 */
int cMenuSys_getAsciiKey(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return self->AsciiKey;
}
#endif

extern "C" void* cInput_getValue(int, int);

//100%
INCLUDE_ASM("util/menu", cMenuSys_getLeft__FPv);
#ifdef SKIP_ASM
/**
 * Returns the pad's menu-left value: cInput_getValue on the cInput at +0x5C, value 62.
 *
 * Declared returning a pointer; callers read it as a float (cIntMenuItem_input and the others use right minus left).
 *
 * @return the left value
 */
void* cMenuSys_getLeft(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue((int)self->Input, 0x3e);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getRight__FPv);
#ifdef SKIP_ASM
/**
 * Returns the pad's menu-right value: cInput_getValue on the cInput at +0x5C, value 63.
 *
 * Declared returning a pointer; callers read it as a float.
 *
 * @return the right value
 */
void* cMenuSys_getRight(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue((int)self->Input, 0x3f);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getUp__FPv);
#ifdef SKIP_ASM
/**
 * Returns the pad's menu-up value: cInput_getValue on the cInput at +0x5C, value 64.
 *
 * Declared returning a pointer; callers read it as a float.
 *
 * @return the up value
 */
void* cMenuSys_getUp(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue((int)self->Input, 0x40);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getDown__FPv);
#ifdef SKIP_ASM
/**
 * Returns the pad's menu-down value: cInput_getValue on the cInput at +0x5C, value 65.
 *
 * Declared returning a pointer; callers read it as a float.
 *
 * @return the down value
 */
void* cMenuSys_getDown(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue((int)self->Input, 65);
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getLeftShift);
#ifdef SKIP_ASM
// PORT: cInput_getValue is declared returning void* in this unit; this caller reads a float result ($f0).
float cInput_getValue_f(int, int) __asm__("cInput_getValue");
// PORT: the unit declares cMenuSys_getLeftShift as void*(int); the body takes the object pointer.
int cMenuSys_getLeftShift_impl(void* self) __asm__("cMenuSys_getLeftShift");

/**
 * Tells whether the menu's left-shift key is held: cInput_getValue on the cInput at +0x5C, value 66, is not 0.
 *
 * @return 1 while held, else 0
 */
int cMenuSys_getLeftShift_impl(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue_f((int)self->Input, 66) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_getRightShift);
#ifdef SKIP_ASM
// PORT: cInput_getValue is declared returning void* in this unit; this caller reads a float result ($f0).
float cInput_getValue_f(int, int) __asm__("cInput_getValue");
// PORT: the unit declares cMenuSys_getRightShift as void*(int); the body takes the object pointer.
int cMenuSys_getRightShift_impl(void* self) __asm__("cMenuSys_getRightShift");

/**
 * Tells whether the menu's right-shift key is held: cInput_getValue on the cInput at +0x5C, value 67, is not 0.
 *
 * @return 1 while held, else 0
 */
int cMenuSys_getRightShift_impl(void* vself)
{
    cMenuSys* self = (cMenuSys*)vself;
    return cInput_getValue_f((int)self->Input, 67) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_render);
#ifdef SKIP_ASM
class cMenuSys_render_cObj {
public:
    char data[0x12C];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void render();
};

struct cMenuSys_render_sVec2 {
    float x;
    float y;
    cMenuSys_render_sVec2(float a, float b)
    {
        x = a;
        y = b;
    }
};

struct cMenuSys_render_sStack {
    int count;
    cMenuSys_render_cObj* items[1];
};

/**
 * Draws the menu system's top menu with the font scaled to 0.85 of its base size.
 *
 * Sets the font's (+0x58) Scale to BaseScale * 0.85 on both axes and calls the top menu's (MenuStack[MenuDepth - 1])
 * vtable slot 5 (render). Callers check that the system is active (cMenuSys_isActive) first.
 */
extern "C" void cMenuSys_render(cMenuSys* self)
{
    cMenuSys_render_sVec2 scale(0.8500000238418579f, 0.8500000238418579f);
    cFont* font = self->Font;
    font->Scale[0] = font->BaseScale[0] * scale.x;
    font->Scale[1] = font->BaseScale[1] * scale.y;
    cMenuSys_render_sStack* st = (cMenuSys_render_sStack*)self;
    st->items[st->count - 1]->render();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenuSys_pushMenu);
#ifdef SKIP_ASM
void cMenuSys_activate(void*, int);

class cMenuSys_pushMenu_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void onGainFocus(void*);
    virtual void onLoseFocus();
};

struct cMenuSys_pushMenu_sStack {
    int count;
    cMenuSys_pushMenu_cMenu* items[1];
};

/**
 * Opens a menu: pushes @p item on top of the menu stack @p stack and activates the menu system.
 *
 * Calls the old top menu's vtable slot 7 (onLoseFocus), stores @p item at MenuStack[MenuDepth++], calls its slot 6
 * (onGainFocus) with the menu system, then cMenuSys_activate with 1. There is no overflow check.
 *
 * @param stack  the menu system
 * @param item  the menu to open
 */
extern "C" void cMenuSys_pushMenu(cMenuSys* stack, cMenu* item)
{
    cMenuSys_pushMenu_sStack* self = (cMenuSys_pushMenu_sStack*)stack;
    if (self->count != 0) {
        self->items[self->count - 1]->onLoseFocus();
    }
    self->items[self->count++] = (cMenuSys_pushMenu_cMenu*)item;
    self->items[self->count - 1]->onGainFocus(self);
    cMenuSys_activate(self, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cMenuSys_popMenu);
#ifdef SKIP_ASM
class cMenuSys_popMenu_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void onGainFocus(void*);
    virtual void onLoseFocus();
};

struct cMenuSys_popMenu_sStack {
    int count;                          // 0x0
    cMenuSys_popMenu_cMenu* entries[8];    // 0x4
};

/**
 * Closes the top menu of the menu system and deactivates the system when none is left.
 *
 * Decrements MenuDepth, calls the popped menu's vtable slot 7 (onLoseFocus) and, when a menu remains, the new top's
 * slot 6 (onGainFocus) with the menu system; with none left it calls cMenuSys_activate with 0. There is no
 * underflow check.
 *
 * @param menuSys  the menu system
 */
extern "C" void cMenuSys_popMenu(cMenuSys* menuSys)
{
    cMenuSys_popMenu_sStack* self = (cMenuSys_popMenu_sStack*)menuSys;
    self->count--;
    self->entries[self->count]->onLoseFocus();
    if (self->count != 0) {
        self->entries[self->count - 1]->onGainFocus(self);
        if (self->count != 0) {
            return;
        }
    }
    cMenuSys_activate(self, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cBoolMenuItem_cBoolMenuItem);
#ifdef SKIP_ASM
extern void* cBoolMenuItem_vtbl[];

/**
 * Constructs an on/off menu item: a caption plus the text shown for each state of an int flag.
 *
 * Runs the base cMenuItem_cMenuItem with @p text as the ID, then sets the caption (+0x14) = @p caption, on text (+0x18) =
 * @p onText, off text (+0x1C) = @p offText, flag pointer (+0x20) = @p flagPtr, and the vtable cBoolMenuItem_vtbl (input cBoolMenuItem_input,
 * preRender cBoolMenuItem_preRender, render cBoolMenuItem_render).
 *
 * @param text  the item's ID
 * @param flagPtr  the flag the item toggles
 * @param caption  caption
 * @param onText  text shown when the flag is set, or 0
 * @param offText  text shown when it is clear, or 0
 * @return @p self
 */
extern "C" void* cBoolMenuItem_cBoolMenuItem(cBoolMenuItem* self, void* text, int flagPtr, int caption, int onText, int offText)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)caption;
    self->OnText = (const char*)onText;
    self->OffText = (const char*)offText;
    self->Flag = (int*)flagPtr;
    self->mVtbl = cBoolMenuItem_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cBoolMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);

class cBoolMenuItem_input_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void onChange(void*);
};

/**
 * Handles input for an on/off item: toggles its flag on select, or on left/right when it has both state texts.
 *
 * Runs the base item input (cMenuItem_input) first. Unless the item is locked (cMenuItem_isLocked) or the base input
 * returned nonzero, a key of 6 (select), or 2/3 (left/right) when both the on (+0x18) and off (+0x1C) texts are set,
 * flips bit 0 of the int at the flag pointer (+0x20) and calls the menu's vtable slot 4 (onChange) with the item.
 *
 * @return the base input's result
 */
extern "C" int cBoolMenuItem_input(void** self)
{
    int baseResult = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) == 0 && baseResult == 0) {
        if ((self[6] != 0 && self[7] != 0 && (cMenuItem_getKey(self) == 2 || cMenuItem_getKey(self) == 3)) || cMenuItem_getKey(self) == 6) {
            *(int*)self[8] ^= 1;
            ((cBoolMenuItem_input_cMenu*)self[0])->onChange(self);
        }
    }
    return baseResult;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cBoolMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);


/**
 * Measures an on/off item: caption on the left with the wider of the two state texts on the right, or the caption
 * alone, centred, when either state text is missing.
 *
 * Calls cMenuItem_textPreRender (caption, 0, on text) and (caption, 0, off text) and keeps the wider rect.
 *
 * @param ret  where the rect is returned
 * @param item  the on/off item
 * @return @p ret
 */
extern "C" tRect* cBoolMenuItem_preRender(tRect* ret, cBoolMenuItem* item)
{
    if (item->OnText != 0 && item->OffText != 0) {
        tRect trueRect;
        tRect falseRect;
        tRect measured;
        cMenuItem_textPreRender(&measured, item, (int)item->Caption, 0, (void*)item->OnText);
        trueRect = measured;
        cMenuItem_textPreRender(&measured, item, (int)item->Caption, 0, (void*)item->OffText);
        falseRect = measured;
        *ret = (trueRect.W > falseRect.W) ? trueRect : falseRect;
    } else {
        cMenuItem_textPreRender(ret, item, 0, (int)item->Caption, 0);
    }
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cBoolMenuItem_render);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws an on/off item: caption on the left and the OnText or OffText on the right, chosen by the int at the
 * flag pointer Flag; with either text missing, just the caption, centred.
 *
 * Draws through cMenuItem_textRender1 (the focus-coloured row).
 *
 * @param rect  the row's rect
 */
extern "C" void cBoolMenuItem_render(cBoolMenuItem* self, int rect)
{
    const char* trueText = self->OnText;
    const char* falseText;
    if (trueText != 0 && (falseText = self->OffText) != 0) {
        cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, (void*)(*self->Flag ? trueText : falseText));
    } else {
        cMenuItem_textRender1_5(self, rect, 0, (int)self->Caption, 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_cEnumMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cEnumMenuItem_vtbl[];

/**
 * Constructs an empty list-choice item: ID -1, no caption (+0x14), no value pointer (+0x20), no entries (+0x18).
 *
 * Stores the vtable cEnumMenuItem_vtbl (input cEnumMenuItem_input, preRender cEnumMenuItem_preRender, render cEnumMenuItem_render). The entry table
 * pointer (+0x1C) is left unset.
 *
 * @return @p self
 */
extern "C" void* cEnumMenuItem_cEnumMenuItem(cEnumMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = 0;
    self->mVtbl = cEnumMenuItem_vtbl;
    self->Enum = 0;
    self->NumTags = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_cEnumMenuItem1);
#ifdef SKIP_ASM
extern void* cEnumMenuItem_vtbl[];

/**
 * Constructs an empty list-choice item with an ID: like cEnumMenuItem_cEnumMenuItem, but @p text is the ID.
 *
 * Caption (+0x14), value pointer (+0x20) and entry count (+0x18) start at 0; vtable cEnumMenuItem_vtbl.
 *
 * @param text  the item's ID
 * @return @p self
 */
extern "C" void* cEnumMenuItem_cEnumMenuItem1(cEnumMenuItem* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = 0;
    self->mVtbl = cEnumMenuItem_vtbl;
    self->Enum = 0;
    self->NumTags = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_cEnumMenuItem2);
#ifdef SKIP_ASM
extern void* cEnumMenuItem_vtbl[];

/**
 * Constructs a list-choice item: a caption and an int that takes one of the values of a {name, value} table.
 *
 * Runs cMenuItem_cMenuItem with @p text as the ID, stores the vtable cEnumMenuItem_vtbl, the caption (+0x14) = @p caption, the value
 * pointer (+0x20) = @p enumPtr, the entry count (+0x18) = @p numTags and the entry table (+0x1C) = @p tags.
 *
 * @param text  the item's ID
 * @param enumPtr  the int the item sets
 * @param caption  caption
 * @param numTags  number of entries in @p tags
 * @param tags  the entries, 8 bytes each: name, value
 * @return @p self
 */
extern "C" void* cEnumMenuItem_cEnumMenuItem2(cEnumMenuItem* self, void* text, int enumPtr, int caption, int numTags, int tags)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->mVtbl = cEnumMenuItem_vtbl;
    self->Caption = (const char*)caption;
    self->Enum = (int*)enumPtr;
    self->NumTags = numTags;
    self->Tags = (const tCFMEnumTag*)tags;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_setEnumTagsNullTerm);
#ifdef SKIP_ASM
/**
 * Sets a list-choice item's entry table (+0x1C) to @p table and counts its entries into +0x18, up to the first one
 * whose name word is 0.
 *
 * @param table  {name, value} pairs ending with a null name
 */
extern "C" void cEnumMenuItem_setEnumTagsNullTerm(cEnumMenuItem* self, int *table) {
    int index;

    self->Tags = (const tCFMEnumTag*)table;
    self->NumTags = 0;
    if (*table != 0) {
        do {
            index = self->NumTags + 1;
            self->NumTags = index;
        } while (table[index * 2] != 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);

typedef int (*Pick_2CC8F0)(void*, void*, int);
typedef void (*Notify_2CC8F0)(void*, void*);

struct sEnt_2CC8F0 {
    const char* name;
    int value;
};


static inline void notify_2CC8F0(cEnumMenuItem* self)
{
    sVEntry<void>* vt = (sVEntry<void>*)self->Menu->mVtbl;
    ((Notify_2CC8F0)vt[4].fn)(vthis(self->Menu, vt[4]), self);
}

/**
 * Handles input for a list-choice item: left steps the int to the previous table value, right to the next (both wrap);
 * select calls the menu's onCommand.
 *
 * Runs the base item input (cMenuItem_input); a locked item or a nonzero base result returns that result. Key 6 (select)
 * returns the menu's vtable slot 3 (onCommand) with (item, 0). Otherwise it finds the current value (*(+0x20)) in the
 * table (+0x1C, count +0x18): keys 2 or 0 pick the previous entry and 3 or 1 the next, modulo the count; when the value
 * is not in the table, 2/0 pick the first entry and 3/1 the last. Each change calls the menu's slot 4 (onChange). (Keys
 * 0 and 1 never get here: the base input already returned for them.)
 *
 * @param vself  the list-choice item
 * @return the base input's result, or onCommand's on select
 */
extern "C" int cEnumMenuItem_input(cEnumMenuItem* vself)
{
    cEnumMenuItem* self = vself;
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    if (cMenuItem_getKey((void**)self) == 6) {
        sVEntry<void>* vt = (sVEntry<void>*)self->Menu->mVtbl;
        result = ((Pick_2CC8F0)vt[3].fn)(vthis(self->Menu, vt[3]), self, 0);
    } else {
    int n = self->NumTags;
    int i = 0;
    for (;;) {
        if (i == n) {
            if (cMenuItem_getKey((void**)self) == 2 || cMenuItem_getKey((void**)self) == 0) {
                *self->Enum = ((sEnt_2CC8F0*)self->Tags)[0].value;
                notify_2CC8F0(self);
            } else if (cMenuItem_getKey((void**)self) == 3 || cMenuItem_getKey((void**)self) == 1) {
                *self->Enum = ((sEnt_2CC8F0*)self->Tags)[self->NumTags - 1].value;
                notify_2CC8F0(self);
            }
            goto done;
        }
        if (*self->Enum == ((sEnt_2CC8F0*)self->Tags)[i].value) {
            if (cMenuItem_getKey((void**)self) == 2 || cMenuItem_getKey((void**)self) == 0) {
                *self->Enum = ((sEnt_2CC8F0*)self->Tags)[(i + self->NumTags - 1) % self->NumTags].value;
                notify_2CC8F0(self);
            } else if (cMenuItem_getKey((void**)self) == 3 || cMenuItem_getKey((void**)self) == 1) {
                *self->Enum = ((sEnt_2CC8F0*)self->Tags)[(i + 1) % self->NumTags].value;
                notify_2CC8F0(self);
            }
            goto done;
        }
        i++;
    }
done:
    return result;
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cEnumMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern char str_Invalid_b[];

struct sMenuRectCB18 { float x, y, z, w; };
struct sMenuEntCB18 { void* name; int data; };

/**
 * Measures a list-choice item: the caption on the left with the widest of "Invalid" (str_Invalid_b) and every entry
 * name on the right (cMenuItem_textPreRender for each).
 *
 * @param ret  where the rect is returned
 * @param item  the list-choice item
 * @return @p ret
 */
extern "C" sMenuRectCB18* cEnumMenuItem_preRender(sMenuRectCB18* ret, cEnumMenuItem* item)
{
    sMenuRectCB18 best;
    sMenuRectCB18 measured;
    cMenuItem_textPreRender(&measured, item, (int)item->Caption, 0, str_Invalid_b);
    best = measured;
    for (int i = 0; i < item->NumTags; i++) {
        cMenuItem_textPreRender(&measured, item, (int)item->Caption, 0, ((sMenuEntCB18*)item->Tags)[i].name);
        if (measured.z > best.z) {
            best = measured;
        }
    }
    *ret = best;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cEnumMenuItem_render);
#ifdef SKIP_ASM
extern char str_Invalid_b[];

struct cEnumMenuItem_render_sEntry {
    char* name;  // 0x0
    int value;   // 0x4
};

/**
 * Draws a list-choice item: the caption on the left and, on the right, the name of the entry whose value equals the int
 * at +0x20, or "Invalid" (str_Invalid_b) when none does (cMenuItem_textRender1).
 *
 * @param rect  the row's rect
 */
extern "C" void cEnumMenuItem_render(cEnumMenuItem* self, int rect)
{
    int i = 0;
    for (;;) {
        if (i == self->NumTags) {
            cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, str_Invalid_b);
            break;
        }
        // PORT: pointer held in int (index-first address arithmetic)
        cEnumMenuItem_render_sEntry* entry = (cEnumMenuItem_render_sEntry*)((i << 3) + (int)self->Tags);
        if (*self->Enum == entry->value) {
            cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, entry->name);
            return;
        }
        i++;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cSubMenuItem_cSubMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cSubMenuItem_vtbl[];


/**
 * Constructs a menu item that opens a sub-menu.
 *
 * Runs the base constructor with ID -1, stores the label at +0x14 and the sub-menu at +0x18, then
 * cSubMenuItem's vtable. The help popup menus (festatenethelppopup.cpp) pass another embedded cMenu and a
 * string.
 *
 * @param sub  the sub-menu the item opens, stored at +0x18
 * @param cap  the item's label text, stored at +0x14
 * @return @p self
 */
extern "C" cSubMenuItem* cSubMenuItem_cSubMenuItem(cSubMenuItem* self, cMenu* sub, void* cap)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = (const char*)cap;
    self->Sub = (cMenu*)((void*)sub);
    self->mVtbl = (void**)cSubMenuItem_vtbl;
    return (self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);
extern "C" void cMenuSys_pushMenu(cMenuSys* stack, cMenu* item);

/**
 * Handles input for a sub-menu item: the select command pushes the item's sub-menu onto the menu system.
 *
 * When the item is not locked, the base navigation result is 0 and the menu system's current command is 6
 * (select; Enter on a USB keyboard), it pushes the sub-menu (+0x18) onto Menu->MenuSys with cMenuSys_pushMenu,
 * which takes focus from the current menu.
 *
 * @return the base navigation result from cMenuItem_input (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
extern "C" int cSubMenuItem_input(cSubMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) == 0 && result == 0 && cMenuItem_getKey((void**)self) == 6) {
        cMenuSys_pushMenu(self->Menu->MenuSys, (cMenu*)(self->Sub));
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);

/**
 * Computes a sub-menu item's row rectangle: its label (+0x14) measured as centred text.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * cMenuItem_textPreRender measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the sub-menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cSubMenuItem_preRender(cSubMenuItem* self, cSubMenuItem* item)
{
    cMenuItem_textPreRender(self, item, 0, (int)item->Caption, 0);
    return ((void*)self);
}
#endif

extern "C" void* cMenuItem_textRender1(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a sub-menu item: its row background and its label centred.
 *
 * cMenuItem_textRender1 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 * @return no meaningful value (the draw helper returns nothing; On Tour's render is void)
 */
void* cSubMenuItem_render(void* vself, int rect)
{
    cSubMenuItem* self = (cSubMenuItem*)vself;
    return cMenuItem_textRender1_5(self, rect, 0, (int)self->Caption, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cPopMenuItem_cPopMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cPopMenuItem_vtbl[];


/**
 * Constructs a "back" menu item with ID -1 and caption (+0x14) @p text; selecting it closes the menu (cPopMenuItem_input).
 *
 * Stores the vtable cPopMenuItem_vtbl (input cPopMenuItem_input, preRender cPopMenuItem_preRender, render cPopMenuItem_render). The help and debug
 * menus use it for their "Cancel" and "Continue" entries.
 *
 * @param text  caption
 * @return @p self
 */
extern "C" cPopMenuItem* cPopMenuItem_cPopMenuItem(cPopMenuItem* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = (const char*)text;
    self->mVtbl = (void**)cPopMenuItem_vtbl;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cPopMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);
extern "C" void cMenuSys_popMenu(cMenuSys* stack);

/**
 * Handles input for a "back" item: on select (key 6), unless locked or already handled by the base input
 * (cMenuItem_input), closes the top menu (cMenuSys_popMenu on Menu->MenuSys).
 *
 * @return the base input's result
 */
extern "C" int cPopMenuItem_input(cPopMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) == 0 && result == 0 && cMenuItem_getKey((void**)self) == 6) {
        cMenuSys_popMenu(self->Menu->MenuSys);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cPopMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);

/**
 * Measures a "back" item: its caption alone, centred (cMenuItem_textPreRender with (0, caption, 0)).
 *
 * @param item  the item
 * @return @p self, the hidden return rect
 */
extern "C" void* cPopMenuItem_preRender(cPopMenuItem* self, cPopMenuItem* item)
{
    cMenuItem_textPreRender(self, item, 0, (int)item->Caption, 0);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cPopMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a "back" item: its caption, centred, through cMenuItem_textRender1.
 *
 * @param rect  the row's rect
 * @return cMenuItem_textRender1's return value
 */
void* cPopMenuItem_render(void* vself, int rect)
{
    cPopMenuItem* self = (cPopMenuItem*)vself;
    return cMenuItem_textRender1_5(self, rect, 0, (int)self->Caption, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cCommandMenuItem_cCommandMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cCommandMenuItem_vtbl[];


/**
 * Constructs an empty command item: ID -1, no caption (+0x14), state text (+0x18), argument (+0x1C) or data (+0x20).
 *
 * Stores the vtable cCommandMenuItem_vtbl (input cCommandMenuItem_input, preRender cCommandMenuItem_preRender, render cCommandMenuItem_render). The net help
 * popups build their persona, room and chat lists from these and fill them in later (cCommandMenuItem_init).
 *
 * @return @p self
 */
extern "C" cCommandMenuItem* cCommandMenuItem_cCommandMenuItem(cCommandMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = 0;
    self->mVtbl = (void**)cCommandMenuItem_vtbl;
    self->CmdArg = 0;
    self->Data = 0;
    self->State = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cCommandMenuItem_cCommandMenuItem1);
#ifdef SKIP_ASM
extern void* cCommandMenuItem_vtbl[];

/**
 * Constructs an empty command item with ID @p text: caption, state text, argument and data start at 0; vtable
 * cCommandMenuItem_vtbl.
 *
 * @param text  the item's ID
 * @return @p self
 */
extern "C" void* cCommandMenuItem_cCommandMenuItem1(cCommandMenuItem* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = 0;
    self->mVtbl = cCommandMenuItem_vtbl;
    self->CmdArg = 0;
    self->Data = 0;
    self->State = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cCommandMenuItem_cCommandMenuItem2);
#ifdef SKIP_ASM
extern void* cCommandMenuItem_vtbl[];


/**
 * Constructs a command item: ID @p text, caption (+0x14) @p caption and argument (+0x1C) @p commandArg, which selecting the item
 * passes to the menu's onCommand.
 *
 * State text (+0x18) and data (+0x20) start at 0; vtable cCommandMenuItem_vtbl. Used for "Abort", "Create", "Chat" and similar
 * entries.
 *
 * @param text  the item's ID
 * @param caption  caption
 * @param commandArg  argument for onCommand
 * @return @p self
 */
extern "C" cCommandMenuItem* cCommandMenuItem_cCommandMenuItem2(cCommandMenuItem* self, void* text, int caption, int commandArg)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)caption;
    self->mVtbl = (void**)cCommandMenuItem_vtbl;
    self->CmdArg = commandArg;
    self->Data = 0;
    self->State = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cCommandMenuItem_init);
#ifdef SKIP_ASM
/**
 * Sets a command item's caption (+0x14) and argument (+0x1C) and clears its state text (+0x18).
 *
 * @param item  the command item
 * @param label  caption
 * @param commandArg  argument for onCommand
 */
extern "C" void cCommandMenuItem_init(cCommandMenuItem* item, int label, int commandArg) {
    item->Caption = (const char*)label;
    item->CmdArg = commandArg;
    item->State = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cCommandMenuItem_input);
#ifdef SKIP_ASM
class cCommandMenuItem_input_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int onCommand(void* item, void* data);
};

extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);

/**
 * Handles input for a command item: on select (key 6), unless locked or handled by the base input (cMenuItem_input),
 * returns the menu's vtable slot 3 (onCommand) called with the item and its argument (+0x1C).
 *
 * @return the base input's result, or onCommand's
 */
extern "C" int cCommandMenuItem_input(cCommandMenuItem* self)
{
    int baseResult = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) == 0 && baseResult == 0 && cMenuItem_getKey((void**)self) == 6) {
        baseResult = (*(cCommandMenuItem_input_cMenu**)self)->onCommand(self, (void*)self->CmdArg);
    }
    return baseResult;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCommandMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);

/**
 * Measures a command item: caption on the left with the State text on the right, or the caption alone, centred,
 * without one (cMenuItem_textPreRender).
 *
 * @param ret  where the rect is returned (the hidden return pointer)
 * @param item  the command item
 * @return @p ret
 */
extern "C" tRect* cCommandMenuItem_preRender(tRect* ret, cCommandMenuItem* item)
{
    const char* rightText = item->State;
    if (rightText != 0) {
        cMenuItem_textPreRender(ret, item, (int)item->Caption, 0, (void*)rightText);
    } else {
        cMenuItem_textPreRender(ret, item, 0, (int)item->Caption, 0);
    }
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCommandMenuItem_render);
#ifdef SKIP_ASM
// PORT: The unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a command item: caption on the left with the State text on the right, or the caption alone, centred,
 * without one (cMenuItem_textRender1).
 *
 * @param rect  the row's rect
 */
extern "C" void cCommandMenuItem_render(cCommandMenuItem* self, int rect)
{
    const char* rightText = self->State;
    if (rightText != 0) {
        cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, (void*)rightText);
    } else {
        cMenuItem_textRender1_5(self, rect, 0, (int)self->Caption, 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cCallbackMenuItem_cCallbackMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cCallbackMenuItem_vtbl[];

/**
 * Constructs an empty callback item: ID -1, no caption (+0x14) and no callback (+0x18).
 *
 * Stores the vtable cCallbackMenuItem_vtbl (input cCallbackMenuItem_input, preRender cCallbackMenuItem_preRender, render cCallbackMenuItem_render).
 *
 * @return @p self
 */
extern "C" void* cCallbackMenuItem_cCallbackMenuItem(cCallbackMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = 0;
    self->mVtbl = cCallbackMenuItem_vtbl;
    self->Callback = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cCallbackMenuItem_cCallbackMenuItem1);
#ifdef SKIP_ASM
extern void* cCallbackMenuItem_vtbl[];

/**
 * Constructs a callback item: ID @p text, caption (+0x14) @p caption and the function (+0x18) @p callback that selecting the item
 * calls.
 *
 * Stores the vtable cCallbackMenuItem_vtbl.
 *
 * @param text  the item's ID
 * @param caption  caption
 * @param callback  function called on select
 * @return @p self
 */
extern "C" void* cCallbackMenuItem_cCallbackMenuItem1(cCallbackMenuItem* self, void* text, int caption, int callback)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)caption;
    self->Callback = (void (*)())callback;
    self->mVtbl = cCallbackMenuItem_vtbl;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cCallbackMenuItem_init);
#ifdef SKIP_ASM
/**
 * Sets a callback item's caption (+0x14) and function (+0x18).
 *
 * @param label  caption
 * @param callback  function called on select
 */
extern "C" void cCallbackMenuItem_init(cCallbackMenuItem* self, int label, int callback) {
    self->Caption = (const char*)label;
    self->Callback = (void (*)())callback;
}
#endif

//100%
INCLUDE_ASM("util/menu", cCallbackMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);

/**
 * Handles input for a callback item: on select (key 6), unless locked or handled by the base input (cMenuItem_input),
 * calls the function at +0x18 with no arguments.
 *
 * @return the base input's result
 */
extern "C" int cCallbackMenuItem_input(cCallbackMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) == 0 && result == 0 && cMenuItem_getKey((void**)self) == 6) {
        (self->Callback)();
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCallbackMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern void* D_004866C8[];

/**
 * Measures a callback item: its caption on the left and "[Callback on Select]" (D_004866C8) on the right
 * (cMenuItem_textPreRender).
 *
 * @param item  the callback item
 * @return @p self, the hidden return rect
 */
extern "C" void* cCallbackMenuItem_preRender(cCallbackMenuItem* self, cCallbackMenuItem* item)
{
    cMenuItem_textPreRender(self, item, (int)item->Caption, 0, D_004866C8);
    return ((void*)self);
}
#endif

extern void* D_004866C8[];
extern "C" void* cMenuItem_textRender1(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCallbackMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a callback item: its caption on the left and "[Callback on Select]" (D_004866C8) on the right (cMenuItem_textRender1).
 *
 * @param rect  the row's rect
 * @return cMenuItem_textRender1's return value
 */
void* cCallbackMenuItem_render(void* vself, int rect)
{
    cCallbackMenuItem* self = (cCallbackMenuItem*)vself;
    return cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, D_004866C8);
}
#endif

extern void* cNullMenuItem_vtbl[16];

//100%
INCLUDE_ASM("util/menu", cNullMenuItem_cNullMenuItem__FP13cNullMenuItemPv);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
// A void* constant -1 materializes as lui/ori; the original passed an int.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

/**
 * Constructs a text-only menu item that has no value to edit.
 *
 * Runs the base constructor with ID -1, stores the text at +0x14 (render draws it), then cNullMenuItem's
 * vtable.
 *
 * @param text  the text the item shows, stored at +0x14
 * @return @p self
 */
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = (const char*)text;
    self->mVtbl = cNullMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cNullMenuItem_canHaveFocus__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether this menu item can take the focus: never.
 *
 * @return 0
 */
int cNullMenuItem_canHaveFocus(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cNullMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);

/**
 * Computes a text-only menu item's row rectangle: its text (+0x14) measured as centred text.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * cMenuItem_textPreRender measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the text-only item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cNullMenuItem_preRender(cNullMenuItem* self, cNullMenuItem* item)
{
    cMenuItem_textPreRender(self, item, 0, (int)item->Caption, 0);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cNullMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a text-only menu item: its row background and its text (+0x14) centred.
 *
 * cMenuItem_textRender1 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 * @return no meaningful value (the draw helper returns nothing; On Tour's render is void)
 */
void* cNullMenuItem_render(void* vself, int rect)
{
    cNullMenuItem* self = (cNullMenuItem*)vself;
    return cMenuItem_textRender1_5(self, rect, 0, (int)self->Caption, 0);
}
#endif

extern void* cSpaceMenuItem_vtbl[16];

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_cSpaceMenuItem__FP14cSpaceMenuItemPv);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
// A void* constant -1 materializes as lui/ori; the original passed an int.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

/**
 * Constructs a blank spacer menu item of a given size.
 *
 * Runs the base constructor with ID -1, stores the size in Space (+0x14), then cSpaceMenuItem's vtable. preRender
 * returns it as the last component of the item's rectangle, and render draws nothing.
 *
 * @param space  the spacer's size (callers pass 10 or 30); a number, though the mangled signature says void*
 * @return @p self
 */
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* space)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Space = (int)space;
    self->mVtbl = cSpaceMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_canHaveFocus__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether this menu item can take the focus: never.
 *
 * @return 0
 */
int cSpaceMenuItem_canHaveFocus(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_preRender);
#ifdef SKIP_ASM

/**
 * Returns a spacer item's row rectangle: zero position and width, with the spacer's size (+0x14) as the
 * height.
 *
 * The int size is converted to float; the 16-byte rectangle comes back through the hidden result pointer.
 *
 * @return the rectangle {0, 0, 0, size}
 */
extern "C" tRect cSpaceMenuItem_preRender(cSpaceMenuItem* self)
{
    tRect rect;
    rect.X = rect.Y = rect.W = 0.0f;
    rect.H = (float)self->Space;
    return rect;
}
#endif

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: the spacer menu item's render has an empty body.
 */
void cSpaceMenuItem_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("util/menu", cCharMenuItem_cCharMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cCharMenuItem_vtbl[];

/**
 * Constructs an empty signed-byte item: ID -1, no caption (+0x14) or value pointer (+0x18), range 0..0 (+0x1C, +0x1D),
 * and cleared acceleration state (+0x20, +0x24, +0x28).
 *
 * Stores the vtable cCharMenuItem_vtbl (input cCharMenuItem_input, preRender cCharMenuItem_preRender, render cCharMenuItem_render, zeroValue
 * cCharMenuItem_zeroValue).
 *
 * @return @p self
 */
extern "C" void* cCharMenuItem_cCharMenuItem(cCharMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = 0;
    self->mVtbl = cCharMenuItem_vtbl;
    self->Value = 0;
    *(char*)&self->Min = 0;
    *(char*)&self->Max = 0;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cCharMenuItem_cCharMenuItem1);
#ifdef SKIP_ASM
extern void* cCharMenuItem_vtbl[];

/**
 * Constructs an empty signed-byte item with ID @p text: the same zeroed fields as cCharMenuItem_cCharMenuItem; vtable cCharMenuItem_vtbl.
 *
 * @param text  the item's ID
 * @return @p self
 */
extern "C" void* cCharMenuItem_cCharMenuItem1(cCharMenuItem* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = 0;
    self->mVtbl = cCharMenuItem_vtbl;
    self->Value = 0;
    *(char*)&self->Min = 0;
    *(char*)&self->Max = 0;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cCharMenuItem_cCharMenuItem2);
#ifdef SKIP_ASM
extern void* cCharMenuItem_vtbl[];

/**
 * Constructs a signed-byte item that edits the byte at @p valuePtr within [@p minValue, @p maxValue].
 *
 * Runs cMenuItem_cMenuItem with @p text as the ID, then caption (+0x14) = @p caption, value pointer (+0x18) = @p valuePtr, minimum
 * (+0x1C) = @p minValue, maximum (+0x1D) = @p maxValue, vtable cCharMenuItem_vtbl, and zeroes the held-frames counter (+0x20), direction
 * (+0x24) and fraction (+0x28).
 *
 * @param text  the item's ID
 * @param valuePtr  the signed byte the item edits
 * @param minValue  minimum value
 * @param maxValue  maximum value
 * @param caption  caption
 * @return @p self
 */
extern "C" void* cCharMenuItem_cCharMenuItem2(cCharMenuItem* self, void* text, int valuePtr, signed char minValue, signed char maxValue, int caption)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)caption;
    self->Value = (signed char*)valuePtr;
    *(char*)&self->Min = minValue;
    *(char*)&self->Max = maxValue;
    self->mVtbl = cCharMenuItem_vtbl;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cCharMenuItem_setMinMax);
#ifdef SKIP_ASM
/**
 * Sets a signed-byte item's range to [@p lo, @p hi] (+0x1C, +0x1D) and clamps the current value into it, when the item
 * has a value pointer (+0x18).
 *
 * @param lo  minimum value
 * @param hi  maximum value
 */
extern "C" void cCharMenuItem_setMinMax(cCharMenuItem* self, int lo, int hi)
{
    signed char* valuePtr = self->Value;
    self->Max = hi;
    self->Min = lo;
    if (valuePtr != 0) {
        if (*valuePtr < self->Min)
            *valuePtr = lo;
        signed char* valuePtrAgain = self->Value;
        if (self->Max < *valuePtrAgain)
            *valuePtrAgain = self->Max;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cCharMenuItem_zeroValue);
#ifdef SKIP_ASM
/**
 * Resets a signed-byte item's value: the byte at +0x18 becomes the minimum (+0x1C) when that is positive, else 0.
 *
 * Slot 8 (zeroValue) of the item's vtable cCharMenuItem_vtbl.
 */
extern "C" void cCharMenuItem_zeroValue(cCharMenuItem* self)
{
    if (self->Min <= 0) {
        *self->Value = 0;
    } else {
        *self->Value = self->Min;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cCharMenuItem_input);
#ifdef SKIP_ASM
// PORT: cMenuItem_getLeft/cMenuItem_getRight return a float (axis value, $f0); the unit declares them void*.
float cMenuItem_getLeft_f(void* self) __asm__("cMenuItem_getLeft__FPv");
float cMenuItem_getRight_f(void* self) __asm__("cMenuItem_getRight__FPv");


// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_002CD5A0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

/**
 * Handles input for a signed-byte item: left and right step the value, faster the longer one direction is held.
 *
 * Unless the item is locked or the base input (cMenuItem_input) returned nonzero, it takes right minus left
 * (cMenuItem_getRight - cMenuItem_getLeft, dead zone 0.1) as the direction and counts the frames it is held (+0x20, direction
 * +0x24); a new direction seeds the fraction (+0x28) with 1 or -1 so the value steps at once. While held it adds input
 * * 0.1 * (held^2 / 288, an integer), divided by 50 with both shift keys (cMenuItem_getLeftShift, cMenuItem_getRightShift), 20 with left
 * shift and 5 with right shift. The byte (+0x18) becomes floor(value + fraction) clamped to [+0x1C, +0x1D]; the
 * leftover stays in the fraction, and the menu's onChange (slot 4) is called.
 *
 * @return the base input's result (tMIR)
 */
extern "C" int cCharMenuItem_input(cCharMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float step = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
    int dir;
    if (step <= -0.1f || step >= 0.1f) {
        dir = 1;
        if (step < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == self->AccelDir) {
        self->AccelCount += 1;
    } else {
        self->AccelCount = 0;
        self->AccelDir = dir;
        self->Fraction = (float)dir;
    }
    if (dir != 0) {
        int n = self->AccelCount;
        step *= 0.1f;
        step *= (float)(n * n / 288);
        float div = 1.0f;
        if (cMenuItem_getLeftShift(self) != 0 && cMenuItem_getRightShift(self) != 0) {
            div = 50.0f;
        } else if (cMenuItem_getLeftShift(self) != 0) {
            div = 20.0f;
        } else if (cMenuItem_getRightShift(self) != 0) {
            div = 5.0f;
        }
        step = step / div;
        self->Fraction += step;
        step = self->Fraction;
        signed char* valuePtr = self->Value;
        int cur = *valuePtr;
        int newValue = (int)ffloor_002CD5A0((float)cur + step);
        int lo = self->Min;
        int hi = self->Max;
        if (newValue < lo) {
            newValue = lo;
        }
        if (hi < newValue) {
            newValue = hi;
        }
        self->Fraction = step + (float)(cur - newValue);
        *valuePtr = newValue;
        cMenu* menu = (cMenu*)(*(char**)self);
        sVEntry<void(void*, void*)>* entry = &((sVEntry<void (void*, void*)>*)menu->mVtbl)[4];
        entry->fn(vthis(menu, *entry), self);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCharMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern void* D_004866E0[];

/**
 * Measures a signed-byte item: its caption on the left with room for "-99999999" (D_004866E0) on the right
 * (cMenuItem_textPreRender).
 *
 * @param item  the item
 * @return @p self, the hidden return rect
 */
extern "C" void* cCharMenuItem_preRender(cCharMenuItem* self, cCharMenuItem* item)
{
    cMenuItem_textPreRender(self, item, (int)item->Caption, 0, D_004866E0);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cCharMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");
extern char str_pct_d_i[];

/**
 * Draws a signed-byte item: its caption on the left and the byte at +0x18, printed with "%d", on the right
 * (cMenuItem_textRender1).
 *
 * @param rect  the row's rect
 */
extern "C" void cCharMenuItem_render(cCharMenuItem* self, int rect)
{
    char buf[0x70];
    sprintf(buf, str_pct_d_i, *self->Value);
    cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_cIntMenuItem1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cIntMenuItem_vtbl[];

/**
 * Constructs an empty int item: ID -1, caption, value pointer, range, held-frames counter, direction and fraction all
 * 0; vtable cIntMenuItem_vtbl.
 *
 * @return @p self
 */
extern "C" void* cIntMenuItem_cIntMenuItem1(cIntMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Caption = 0;
    self->mVtbl = cIntMenuItem_vtbl;
    self->Value = 0;
    self->Min = 0;
    self->Max = 0;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cIntMenuItem_cIntMenuItem2);
#ifdef SKIP_ASM
extern void* cIntMenuItem_vtbl[];

/**
 * Constructs an empty int item with ID @p text: the same zeroed fields as cIntMenuItem_cIntMenuItem1; vtable cIntMenuItem_vtbl.
 *
 * @param text  the item's ID
 * @return @p self
 */
extern "C" void* cIntMenuItem_cIntMenuItem2(cIntMenuItem* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = 0;
    self->mVtbl = cIntMenuItem_vtbl;
    self->Value = 0;
    self->Min = 0;
    self->Max = 0;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_cIntMenuItem);
#ifdef SKIP_ASM
extern void* cIntMenuItem_vtbl[];


/**
 * Constructs a menu item that edits an integer variable within a range.
 *
 * Runs cMenuItem_cMenuItem() with the ID, then stores the label (+0x14), the variable's address (+0x18), the
 * minimum (+0x1C) and maximum (+0x20), and zeroes the input repeat state (+0x24 to +0x2C). The debug menu
 * (main/debugmenu.cpp) builds its switches this way.
 *
 * @param text  the item's ID, passed to the base constructor
 * @param value    address of the int the item edits
 * @param mini    smallest allowed value
 * @param maxi    largest allowed value
 * @param cap    the item's label text
 * @return @p self
 */
extern "C" cIntMenuItem* cIntMenuItem_cIntMenuItem(cIntMenuItem* self, void* text, int value, int mini, int maxi, int cap)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)cap;
    self->Value = (int*)value;
    self->Min = mini;
    self->Max = maxi;
    self->mVtbl = (void**)cIntMenuItem_vtbl;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->Fraction = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_setMinMax);
#ifdef SKIP_ASM
/**
 * Sets an int item's range to [@p minValue, @p maxValue] (+0x1C, +0x20) and clamps its value (*(+0x18)) into it when it
 * has one.
 *
 * @param item  the int item
 * @param minValue  minimum value
 * @param maxValue  maximum value
 */
extern "C" void cIntMenuItem_setMinMax(cIntMenuItem* item, int minValue, int maxValue) {
    int *temp_3;
    int *temp_3_2;
    int temp_4;

    temp_3 = item->Value;
    item->Max = maxValue;
    item->Min = minValue;
    if (temp_3 != 0) {
        if (*temp_3 < minValue) {
            *temp_3 = minValue;
        }
        temp_3_2 = item->Value;
        temp_4 = item->Max;
        if (temp_4 < *temp_3_2) {
            *temp_3_2 = temp_4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_zeroValue);
#ifdef SKIP_ASM
/**
 * Resets the edited integer to its zero point: 0, or the minimum when the range starts above 0.
 *
 * Writes through the value pointer at +0x18: 0 when the minimum (+0x1C) is 0 or less, else the minimum. Virtual slot 8.
 */
extern "C" void cIntMenuItem_zeroValue(cIntMenuItem* self)
{
    int* valuePtr = self->Value;
    int minValue = self->Min;
    if (minValue <= 0) {
        *valuePtr = 0;
    } else {
        *valuePtr = minValue;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_input);
#ifdef SKIP_ASM
// PORT: cMenuItem_getLeft/cMenuItem_getRight return a float (axis value, $f0); the unit declares them void*.
float cMenuItem_getLeft_f(void* self) __asm__("cMenuItem_getLeft__FPv");
float cMenuItem_getRight_f(void* self) __asm__("cMenuItem_getRight__FPv");


// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_002CD9D8(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

/**
 * Handles input for an int item: left and right step the value, faster the longer the direction is held.
 *
 * Unless the item is locked or the base navigation result is nonzero, it reads the horizontal input (cInput
 * value 0x3F minus 0x3E, dead zone 0.1) and counts the frames one direction is held (+0x24, direction +0x28).
 * A new direction seeds the accumulator (+0x2C) with 1 or -1, so the value steps at once; while held it adds
 * input * 0.1 * (held^2 / 288, an integer), divided by 20 with input 0x42, 5 with 0x43 and 50 with both.
 *
 * The new value is floor(value + accumulator) clamped to [+0x1C, +0x20]; the fraction left over stays in the
 * accumulator, and the menu's onChange (slot 4) is called.
 *
 * @return the base navigation result from cMenuItem_input (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
extern "C" int cIntMenuItem_input(cIntMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == self->AccelDir) {
        self->AccelCount += 1;
    } else {
        self->AccelCount = 0;
        self->AccelDir = dir;
        self->Fraction = (float)dir;
    }
    if (dir != 0) {
        int n = self->AccelCount;
        side *= 0.1f;
        side *= (float)(n * n / 288);
        float div = 1.0f;
        if (cMenuItem_getLeftShift(self) != 0 && cMenuItem_getRightShift(self) != 0) {
            div = 50.0f;
        } else if (cMenuItem_getLeftShift(self) != 0) {
            div = 20.0f;
        } else if (cMenuItem_getRightShift(self) != 0) {
            div = 5.0f;
        }
        side = side / div;
        self->Fraction += side;
        side = self->Fraction;
        int* valuePtr = self->Value;
        int cur = *valuePtr;
        int newValue = (int)ffloor_002CD9D8((float)cur + side);
        int lo = self->Min;
        int hi = self->Max;
        if (newValue < lo) {
            newValue = lo;
        }
        if (hi < newValue) {
            newValue = hi;
        }
        self->Fraction = side + (float)(cur - newValue);
        *valuePtr = newValue;
        cMenu* menu = (cMenu*)(*(char**)self);
        sVEntry<void(void*, void*)>* vtEntry = &((sVEntry<void (void*, void*)>*)menu->mVtbl)[4];
        vtEntry->fn(vthis(menu, *vtEntry), self);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cIntMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern void* D_004866E0[];

/**
 * Computes an int item's row rectangle: its label on the left and room for "-99999999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * cMenuItem_textPreRender measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the int menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cIntMenuItem_preRender(cIntMenuItem* self, cIntMenuItem* item)
{
    cMenuItem_textPreRender(self, item, (int)item->Caption, 0, D_004866E0);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cIntMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");
extern char str_pct_d_i[];

/**
 * Draws an int item: its label on the left and the variable's value ("%d") on the right.
 *
 * cMenuItem_textRender1 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cIntMenuItem_render(cIntMenuItem* self, int rect)
{
    char buf[0x70];
    sprintf(buf, str_pct_d_i, *self->Value);
    cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_cFloatMenuItem1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cFloatMenuItem_vtbl[];

/**
 * Constructs an empty float item: ID -1, value scale (+0x24) 1, and caption, value pointer, range, offset (+0x28),
 * held-frames counter (+0x2C) and direction (+0x30) all 0; vtable cFloatMenuItem_vtbl.
 *
 * @return @p self
 */
extern "C" void* cFloatMenuItem_cFloatMenuItem1(cFloatMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->mVtbl = cFloatMenuItem_vtbl;
    self->ValueScale = 1.0f;
    self->Caption = 0;
    self->Value = 0;
    *(int*)&self->Min = 0;
    *(int*)&self->Max = 0;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->ValueOffset = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_cFloatMenuItem);
#ifdef SKIP_ASM
extern void* cFloatMenuItem_vtbl[];


/**
 * Constructs a menu item that edits a float variable within a range.
 *
 * Runs cMenuItem_cMenuItem() with the ID, then stores the label (+0x14), the variable's address (+0x18), the
 * range (+0x1C, +0x20), a display scale of 1.0 (+0x24), and zeroes the display offset and input repeat state
 * (+0x28 to +0x30). cFloatMenuItem_input() edits the value as scale * value + offset.
 *
 * @param text  the item's ID, passed to the base constructor
 * @param value    address of the float the item edits
 * @param minf    the item's label text
 * @param maxf    smallest allowed value
 * @param cap    largest allowed value
 * @return @p self
 */
extern "C" cFloatMenuItem* cFloatMenuItem_cFloatMenuItem(cFloatMenuItem* self, void* text, int value, int minf, float maxf, float cap)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)minf;
    self->Value = (float*)value;
    self->Min = maxf;
    self->Max = cap;
    self->mVtbl = (void**)cFloatMenuItem_vtbl;
    self->ValueScale = 1.0f;
    self->AccelDir = 0;
    self->AccelCount = 0;
    *(int*)&self->ValueOffset = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_input);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// PORT: cMenuItem_getLeft/cMenuItem_getRight return a float (axis value, $f0); the unit declares them void*.
float cMenuItem_getLeft_f(void* self) __asm__("cMenuItem_getLeft__FPv");

float cMenuItem_getRight_f(void* self) __asm__("cMenuItem_getRight__FPv");

// (prototype from their definition at line 554)
void* cMenuItem_getLeftShift_DD78(void* self) __asm__("cMenuItem_getLeftShift__FPv");

// (prototype from their definition at line 565)
void* cMenuItem_getRightShift_DD78(void* self) __asm__("cMenuItem_getRightShift__FPv");

/**
 * Handles input for a float item: left and right move the value by a fraction of its range, faster while
 * held.
 *
 * Unless the item is locked or the base navigation result is nonzero, it reads the horizontal input (cInput
 * value 0x3F minus 0x3E, dead zone 0.1) and counts the frames one direction is held (+0x2C, direction +0x30).
 * The step is input * (max - min) * 5e-5 * max(1, held^2 * 50 / div / 14400) / div, where div is 1, or 20
 * with input 0x42, 5 with 0x43 and 50 with both.
 *
 * It works on the displayed value (scale * value + offset), clamps that to [+0x1C, +0x20], converts it back
 * into the variable and calls the menu's onChange (slot 4).
 *
 * @return the base navigation result from cMenuItem_input (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cFloatMenuItem_input(cFloatMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == self->AccelDir) {
        self->AccelCount += 1;
    } else {
        self->AccelCount = 0;
        self->AccelDir = dir;
    }
    if (dir != 0) {
        float div = 1.0f;
        if (cMenuItem_getLeftShift_DD78(self) != 0 && cMenuItem_getRightShift_DD78(self) != 0) {
            div = 50.0f;
        } else if (cMenuItem_getLeftShift_DD78(self) != 0) {
            div = 20.0f;
        } else if (cMenuItem_getRightShift_DD78(self) != 0) {
            div = 5.0f;
        }
        int n = self->AccelCount;
        float k = (float)(n * n) * 50.0f;
        k /= div;
        k *= 6.944444612599909e-05f;
        k = k >? 1.0f;
        float step = 4.999999873689376e-05f;
        step = k * step;
        step /= div;
        float* valuePtr = self->Value;
        float value = self->ValueScale * *valuePtr + self->ValueOffset;
        value += side * step * (self->Max - self->Min);
        if (value < self->Min) {
            value = self->Min;
        }
        if (self->Max < value) {
            value = self->Max;
        }
        *self->Value = (value - self->ValueOffset) / self->ValueScale;
        cMenu* menu = (cMenu*)(*(char**)self);
        sVEntry<void(void*, void*)>* vt = (sVEntry<void (void*, void*)>*)menu->mVtbl;
        vt[4].fn(vthis(menu, vt[4]), self);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_zeroValue);
#ifdef SKIP_ASM
/**
 * Resets the edited float to its zero point: 0.0, or the minimum when the range starts above 0.
 *
 * Writes through the value pointer at +0x18: 0.0 when the minimum (+0x1C) is 0 or less, else the minimum. Virtual slot
 * 8.
 */
extern "C" void cFloatMenuItem_zeroValue(cFloatMenuItem* self)
{
    float* valuePtr = self->Value;
    float minValue = self->Min;
    if (minValue <= 0.0f) {
        *valuePtr = 0.0f;
    } else {
        *valuePtr = minValue;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cFloatMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern void* D_004866F0[];

/**
 * Computes a float item's row rectangle: its label on the left and room for "-9999999999.999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * cMenuItem_textPreRender measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the float menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cFloatMenuItem_preRender(cFloatMenuItem* self, cFloatMenuItem* item)
{
    cMenuItem_textPreRender(self, item, (int)item->Caption, 0, D_004866F0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", prettyFloat);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cBXString_cBXString1(void* self, void* src);
extern "C" int cBXString_printf(void* str, const char* fmt, ...);
extern "C" float logf(float x);
extern "C" int fptodp(float f);
extern char str_pct_4f[];
extern char str_pct_3f[];
extern char str_pct_2f[];
extern char str_pct_1f_b[];
extern char str_pct_0f[];
extern char str_pct_3g[];

struct sBXStrDFD8 { char* p; };

// returns a cBXString by value (hidden result pointer in $4); the unit's later declaration says void.
void* prettyFloat_impl(void* out, float v) __asm__("prettyFloat");

/**
 * Formats a float as a string with fewer decimals the larger it is.
 *
 * Takes floor(log10(v)) (plus a small epsilon) and picks "%.4f" below 1, "%.3f" up to 10, "%.2f" up to 100,
 * "%.1f" up to 1000, "%.0f" below 10^7 and "%.3g" above, printing through a temporary cBXString.
 *
 * @param out  the cBXString to construct with the result (the hidden return slot)
 * @param value    the value to format
 * @return @p out
 */
void* prettyFloat_impl(void* out, float value)
{
    float logValue = logf(value) / logf(10.0f) + 4.3431620724732056e-05f;
    float logFloor;
    // PORT: FPU-only float->int->float round trip (floor helper); no C cast reproduces it.
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(logFloor) : "f"(logValue));
    if (logValue < logFloor) logFloor -= 1.0f;
    int n = (int)logFloor;
    const char* fmt;
    if (n < 0) {
        fmt = str_pct_4f;
    } else if (n < 1) {
        fmt = str_pct_3f;
    } else if (n < 2) {
        fmt = str_pct_2f;
    } else if (n < 3) {
        fmt = str_pct_1f_b;
    } else if (n < 7) {
        fmt = str_pct_0f;
    } else {
        fmt = str_pct_3g;
    }
    sBXStrDFD8 formatted;
    formatted.p = (char*)cBXString_EmptyString;
    cBXString_printf(&formatted, fmt, fptodp(value));
    cBXString_cBXString1(out, &formatted);
    cBXString__cBXString(&formatted, 2);
    return out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cFloatMenuItem_render);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");
extern "C" void cBXString__cBXString(void* self, int flags);

struct cFloatMenuItem_render_sString {
    char* str;
};

// returns a cBXString by value (hidden result pointer in $4)
extern "C" void prettyFloat(cFloatMenuItem_render_sString* out, float v);

/**
 * Draws a float item: its label on the left and the displayed value on the right.
 *
 * The value is scale (+0x24) * variable + offset (+0x28), formatted by prettyFloat() into a temporary
 * cBXString that is destroyed afterwards. cMenuItem_textRender1 picks the focused or normal style and text colour (is
 * the item its menu's focus item?), draws the row background through the menu (vtable slot 8), then the text
 * with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cFloatMenuItem_render(cFloatMenuItem* self, int rect)
{
    cFloatMenuItem_render_sString pretty;
    prettyFloat(&pretty, *self->Value * self->ValueScale + self->ValueOffset);
    cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, pretty.str);
    cBXString__cBXString(&pretty, 2);
}
#endif

//100%
INCLUDE_ASM("util/menu", cAngleMenuItem_cAngleMenuItem);
#ifdef SKIP_ASM
extern void* cAngleMenuItem_vtbl[];

/**
 * Constructs an angle item that edits the float at @p valuePtr within [@p minValue, @p maxValue].
 *
 * Runs cMenuItem_cMenuItem with @p text as the ID, then caption (+0x14) = @p caption, value pointer (+0x18) = @p valuePtr, minimum
 * (+0x1C) = @p minValue, maximum (+0x20) = @p maxValue, vtable cAngleMenuItem_vtbl, and zeroes the direction (+0x28) and
 * held-frames counter (+0x24).
 *
 * @param text  the item's ID
 * @param valuePtr  the float the item edits
 * @param caption  caption
 * @param minValue  minimum value
 * @param maxValue  maximum value
 * @return @p self
 */
extern "C" void* cAngleMenuItem_cAngleMenuItem(cAngleMenuItem* self, void* text, int valuePtr, int caption, float minValue, float maxValue)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->Caption = (const char*)caption;
    self->Value = (float*)valuePtr;
    self->Min = minValue;
    self->Max = maxValue;
    self->mVtbl = cAngleMenuItem_vtbl;
    self->AccelDir = 0;
    self->AccelCount = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cAngleMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);

/**
 * Handles input for an angle item: left and right move the angle by a fraction of its range, faster while
 * held.
 *
 * Unless the item is locked or the base navigation result is nonzero, it reads the horizontal input (cInput
 * value 0x3F minus 0x3E, dead zone 0.1) and counts the frames one direction is held (+0x24, direction +0x28),
 * then adds input * (max - min) * 5e-5 * max(1, held^2 / 288) to the variable (+0x18), clamped to [+0x1C,
 * +0x20], and calls the menu's onChange (slot 4). Unlike the int and float items there is no modifier
 * slow-down.
 *
 * @return the base navigation result from cMenuItem_input (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cAngleMenuItem_input(cAngleMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == self->AccelDir) {
        self->AccelCount += 1;
    } else {
        self->AccelCount = 0;
        self->AccelDir = dir;
    }
    if (dir != 0) {
        int n = self->AccelCount;
        float k = (float)(n * n) * 0.0034722222480922937f;
        k = k >? 1.0f;
        float step = 4.999999873689376e-05f;
        step = k * step;
        float* valuePtr = self->Value;
        *valuePtr = *valuePtr + side * step * (self->Max - self->Min);
        if (*self->Value < self->Min) {
            *self->Value = self->Min;
        }
        if (self->Max < *self->Value) {
            *self->Value = self->Max;
        }
        cMenu* menu = (cMenu*)(*(char**)self);
        sVEntry<void(void*, void*)>* vt = (sVEntry<void (void*, void*)>*)menu->mVtbl;
        vt[4].fn(vthis(menu, vt[4]), self);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cAngleMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern void* D_004866F0[];

/**
 * Computes an angle item's row rectangle: its label on the left and room for "-9999999999.999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * cMenuItem_textPreRender measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the angle menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cAngleMenuItem_preRender(cAngleMenuItem* self, cAngleMenuItem* item)
{
    cMenuItem_textPreRender(self, item, (int)item->Caption, 0, D_004866F0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cAngleMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");
extern char str_pct_3_2f[];

/**
 * Draws an angle item: its label on the left and the angle in degrees ("%3.2f") on the right.
 *
 * The variable holds radians; it is multiplied by 57.2958 and passed to sprintf as a double (fptodp).
 * cMenuItem_textRender1 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cAngleMenuItem_render(cAngleMenuItem* self, int rect)
{
    char buf[0x70];
    sprintf(buf, str_pct_3_2f, fptodp(*self->Value * 57.2957763671875f));
    cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cTextMenuItem_cTextMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern void* cTextMenuItem_vtbl[];


/**
 * Constructs a text-entry item: ID -1, caption (+0x14) @p caption, the text (+0x18, 16 bytes) copied from @p name (at most
 * 15 characters, always terminated) and the cursor (+0x28) at 0.
 *
 * Stores the vtable cTextMenuItem_vtbl (input cTextMenuItem_input, preRender cTextMenuItem_preRender, render cTextMenuItem_render). The net help
 * popups use it for "Login Name", "Password", "Persona Name" and "Enter Chat".
 *
 * @param caption  caption
 * @param name  initial text
 * @return @p self
 */
extern "C" cTextMenuItem* cTextMenuItem_cTextMenuItem(cTextMenuItem* self, int caption, const char* name)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    (*(int*)&self->Caption) = caption;
    self->CursorPos = 0;
    (*(void**)&self->mVtbl) = cTextMenuItem_vtbl;
    strncpy(self->Text, name, 0xF);
    self->Text[0xF] = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cTextMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);
extern "C" int strlen(const char* s);
int cMenuSys_getVKey(void* self);
int cMenuSys_getAsciiKey(void* self);


static inline int validChar_E488(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '!';
}

/**
 * Handles input for a text-entry item: moves the cursor, cycles, inserts and deletes characters, and takes typed keys.
 *
 * Only when the base input (cMenuItem_input) returns 0. With len the text's length, keys 2/3 move the cursor (+0x28)
 * left/right modulo min(len + 1, 15); keys 4/5 step the character under the cursor down/up until it is a letter, '_' or
 * '!'; key 9 inserts '_' at the cursor and key 10 deletes the character there; key 6 returns 3 (focus the next item).
 * Otherwise it reads the keyboard (cMenuSys_getVKey VKey, cMenuSys_getAsciiKey AsciiKey of Menu->MenuSys): VKey 8 (backspace)
 * deletes and moves the cursor back, and a letter, '_' or '!' is inserted at the cursor, both only while the text is
 * shorter than 14. Likely bugs: the insert loops copy forwards (text[i + 1] = text[i] from the cursor up), smearing the
 * character at the cursor over the rest of the text instead of shifting it, and backspace removes the character under
 * the cursor rather than the one before it. The item's lock flag is not checked.
 *
 * @return the base input's result, or 3 on select
 */
extern "C" int cTextMenuItem_input(cTextMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (result == 0) {
    int len = strlen(self->Text);
    int mod = len + 1;
    if (mod >= 16) {
        mod = 15;
    }
    int key = cMenuItem_getKey((void**)self);
    if (key == 2) {
        self->CursorPos = (self->CursorPos + mod - 1) % mod;
    } else if (key == 3) {
        self->CursorPos = (self->CursorPos + 1) % mod;
    } else if (key == 4) {
        // PORT: pointer in int (index-first address).
        char* cursorChar = (char*)(self->CursorPos + (int)self);
        cursorChar += 0x18;
        do {
            (*cursorChar)--;
        } while (!validChar_E488(*cursorChar));
    } else if (key == 5) {
        // PORT: pointer in int (index-first address).
        char* cursorChar = (char*)(self->CursorPos + (int)self);
        cursorChar += 0x18;
        do {
            (*cursorChar)++;
        } while (!validChar_E488(*cursorChar));
    } else if (key == 9) {
        if (len < 14) {
            for (int i = self->CursorPos; i < len; i++) {
                self->Text[i + 1] = self->Text[i];
            }
            self->Text[self->CursorPos] = '_';
        }
    } else if (key == 10) {
        if (len != 0 && self->CursorPos < len) {
            for (int i = self->CursorPos; i < len - 1; i++) {
                self->Text[i] = self->Text[i + 1];
            }
            self->Text[len - 1] = 0;
        }
    } else if (key == 6) {
        result = 3;
    } else {
        int code = cMenuSys_getVKey(self->Menu->MenuSys);
        int ch = cMenuSys_getAsciiKey(self->Menu->MenuSys);
        if (code == 8) {
            if (len != 0 && self->CursorPos > 0) {
                for (int i = self->CursorPos; i < len - 1; i++) {
                    self->Text[i] = self->Text[i + 1];
                }
                self->Text[len - 1] = 0;
                self->CursorPos--;
            }
        } else if (validChar_E488(ch)) {
            if (len < 14) {
                for (int i = self->CursorPos; i < len; i++) {
                    self->Text[i + 1] = self->Text[i];
                }
                self->Text[self->CursorPos++] = ch;
            }
        }
    }
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cTextMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" char* strcat(char* dst, const char* src);
extern char str_underscore[];

struct cTextMenuItem_preRender_sResult {
    int a;
    int b;
    int c;
    int d;
};

static inline cTextMenuItem_preRender_sResult cTextMenuItem_preRender_make(void* item, int id, char* text)
{
    cTextMenuItem_preRender_sResult t;
    cMenuItem_textPreRender(&t, item, id, 0, text);
    return t;
}

/**
 * Measures a text-entry item: its caption on the left and its text on the right, with a trailing "_" (str_underscore) when
 * the item has the focus and the cursor (+0x28) is at the end of the text and below 15 (cMenuItem_textPreRender).
 *
 * @param item  the text-entry item
 * @return the rect
 */
extern "C" cTextMenuItem_preRender_sResult cTextMenuItem_preRender(cTextMenuItem* item)
{
    char buf[0x20];
    char* name = item->Text;
    int focused = ((void*)item) == cMenu_getFocusItem((cMenu*)(*(void**)item));
    strcpy(buf, name);
    int len = strlen(name);
    if (focused && item->CursorPos == len && item->CursorPos < 15) {
        strcat(buf, str_underscore);
    }
    cTextMenuItem_preRender_sResult rowRect = cTextMenuItem_preRender_make(item, (int)item->Caption, buf);
    return rowRect;
}
#endif

//100%
INCLUDE_ASM("util/menu", cTextMenuItem_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sVec2_2CE910 {
    float x, y;
    sVec2_2CE910(float ax, float ay) { x = ax; y = ay; }
};

struct sVec4_2CE910 {
    float x, y, z, w;
    sVec4_2CE910() {}
    void Set(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sVtx_2CE910 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sVec4_2CE910 pos;
    sVtx_2CE910() {}
};

struct sRS_2CE910 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
};

class cCtx_2CE910 {
public:
    char pad0[0xE84];
    sRS_2CE910* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, void* verts, int flags);
};


/**
 * Draws a text-entry item: box, caption, the text right-aligned and, when it has the focus, an underline cursor.
 *
 * Builds the text as cTextMenuItem_preRender does (a trailing "_" when the focused cursor is at the end), draws the box through
 * the menu's vtable slot 8 in kBoxSelectedColor (focused) or kBoxUnselectedColor, sets the menu font's Color to kTextSelectedColor or
 * kTextUnselectedColor (alpha 1) and ShadowOffset to (2, 2), and draws the caption at rect.x + 16 and the text ending 16 before
 * the right edge (cFont_text, y = rect.y + 5). When focused it draws an untextured quad (draw state bits 2-6 = 1,
 * texture -1) under the character at the cursor: from the right-aligned x of the text from the cursor on to that of the
 * text from the cursor + 1 on, 3 units high, 4 to 7 units above the bottom (rect.y + rect.h - 6), in the text colour (*
 * 127, alpha 0x7F), via GrMan's vtable slot 71. Resets ShadowOffset to (0, 0).
 *
 * @param rect  the row's rect (x, y, w, h)
 */
extern "C" void cTextMenuItem_render(cTextMenuItem* self, float* rect)
{
    char buf[0x20];
    char* name = self->Text;
    int focused = ((void*)self) == cMenu_getFocusItem((cMenu*)(*(void**)self));
    strcpy(buf, name);
    int len = strlen(name);
    if (focused && self->CursorPos == len && self->CursorPos < 15) {
        strcat(buf, str_underscore);
    }
    {
        char* menu = *(char**)self;
        sVEntry2CB880* vt = *(sVEntry2CB880**)(menu + 0x12C);
        fn2CB880* drawBoxFn = &vt[8].fn;
        (*drawBoxFn)(vthis(menu, vt[8]), rect, focused ? kBoxSelectedColor : kBoxUnselectedColor);
    }
    cMenuItem_textRender1_sVec3* col = ((void*)self) == cMenu_getFocusItem((cMenu*)(*(void**)self)) ? &kTextSelectedColor : &kTextUnselectedColor;
    cMenuItem_textRender1_sColor textColor;
    textColor.a = 1.0f;
    textColor.r = col->x;
    textColor.g = col->y;
    textColor.b = col->z;
    *(cMenuItem_textRender1_sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = textColor;
    *(sVec2_2CE910*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CE910(2.0f, 2.0f);
    char* label = (char*)self->Caption;
    if (label) {
        cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f, label);
    }
    cFont* font = (cFont*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58));
    float w = cFont_textExtent(font, buf, 0, 0, font->Scale[0], font->Scale[1]);
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + (rect[2] - w) - 16.0f, rect[1] + 6.0f - 1.0f, buf);
    if (focused) {
        font = (cFont*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58));
        float w1 = cFont_textExtent(font, buf + self->CursorPos, 0, 0, font->Scale[0], font->Scale[1]);
        font = (cFont*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58));
        char* next = buf + 1;
        float w2 = cFont_textExtent(font, next + self->CursorPos, 0, 0, font->Scale[0], font->Scale[1]);
        float x1 = rect[0] + (rect[2] - w1) - 16.0f;
        float bot = rect[1] + rect[3] - 6.0f;
        float x2 = rect[0] + (rect[2] - w2) - 16.0f;
        float y1 = bot - 4.0f;
        float y2 = bot - 1.0f;
        sVtx_2CE910 cursorVerts[4];
        float zero = 0.0f;
        float one = 1.0f;
        sVec4_2CE910 vertexPos;
        vertexPos.Set(x1, y1, zero, one);
        cursorVerts[0].v = zero;
        cursorVerts[0].u = zero;
        cursorVerts[0].q = one;
        cursorVerts[0].r = (int)(textColor.r * 127.0f);
        cursorVerts[0].g = (int)(textColor.g * 127.0f);
        cursorVerts[0].b = (int)(textColor.b * 127.0f);
        cursorVerts[0].a = 0x7F;
        cursorVerts[0].pos = vertexPos;
        vertexPos.Set(x1, y2, zero, one);
        cursorVerts[1] = cursorVerts[0];
        cursorVerts[1].pos = vertexPos;
        vertexPos.Set(x2, y1, zero, one);
        cursorVerts[2] = cursorVerts[0];
        cursorVerts[2].pos = vertexPos;
        vertexPos.Set(x2, y2, zero, one);
        cursorVerts[3] = cursorVerts[0];
        cursorVerts[3].pos = vertexPos;
        ((cCtx_2CE910*)GrMan)->top->f4_2 = 1;
        *(short*)((char*)((cCtx_2CE910*)GrMan)->top + 0x10) = -1;
        GrMan->RenderTriStrip(4, (tBxVertex*)cursorVerts, 0);
    }
    *(sVec2_2CE910*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CE910(0.0f, 0.0f);
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem_cColorMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cColorMenuItem_vtbl[];


/**
 * Constructs a menu item that edits one channel of a colour.
 *
 * Runs the base constructor with ID -1, stores the colour at +0x14 and the channel at +0x18, then cColorMenuItem's
 * vtable. cARGBMenuItem_cARGBMenuItem() makes four, for channels 0 to 3.
 *
 * @param color  the colour being edited, stored at +0x14
 * @param adjust  channel index 0 to 3, stored at +0x18
 * @return @p self
 */
extern "C" cColorMenuItem* cColorMenuItem_cColorMenuItem(cColorMenuItem* self, tARGB* color, void* adjust)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Color = (tARGB*)((void*)color);
    self->Adjust = (int)adjust;
    self->mVtbl = (void**)cColorMenuItem_vtbl;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);

/**
 * Handles input for a colour-channel item: left and right move the channel between 0 and 1, faster while
 * held.
 *
 * Unless the item is locked or the base navigation result is nonzero, it reads the horizontal input (cInput
 * value 0x3F minus 0x3E, dead zone 0.1) and counts the frames one direction is held (+0x1C, direction +0x20),
 * then adds input * max(1, held^2 / 288) / 2560 to channel +0x18 of the colour at +0x14, clamped to [0, 1],
 * and calls the menu's onChange (slot 4).
 *
 * @return the base navigation result from cMenuItem_input (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cColorMenuItem_input(cColorMenuItem* self)
{
    int result = cMenuItem_input((cMenuItem*)self);
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == self->AccelDir) {
        self->AccelCount += 1;
    } else {
        self->AccelCount = 0;
        self->AccelDir = dir;
    }
    if (dir != 0) {
        int n = self->AccelCount;
        int accel = (n * n / 288) >? 1;
        float* chanPtr = (float*)((char*)self->Color + (self->Adjust << 2));
        float step = 0.0003906250058207661f;
        step = (float)accel * step;
        float value = *chanPtr;
        value += side * step;
        if (value < 0.0f) {
            value = 0.0f;
        }
        if (1.0f < value) {
            value = 1.0f;
        }
        *chanPtr = value;
        char* menu = *(char**)self;
        sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(menu + 0x12C);
        vt[4].fn(vthis(menu, vt[4]), self);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
extern char str_pct_3f_999[];
extern char* ColorNames[];

struct sRect_2CEEE8 { float x, y, w, h; };

struct sFont_2CEEE8 {
    char pad_0x0[0x14];
    int lineHeight;                 // 0x14
    char pad_0x18[0x18];
    float scaleX;                   // 0x30
    float scaleY;                   // 0x34
    float drawScaleX;               // 0x38
    float drawScaleY;               // 0x3C
};

struct sMenuValueItem_2CEEE8 {
    char* menu;                     // 0x00
    char pad_0x4[0x10];
    float* values;                  // 0x14
    int index;                      // 0x18
};

static inline sFont_2CEEE8* font_2CEEE8(sMenuValueItem_2CEEE8* item)
{
    return *(sFont_2CEEE8**)(*(char**)(item->menu + 0x124) + 0x58);
}

static inline float textWidth_2CEEE8(sMenuValueItem_2CEEE8* item, const char* str)
{
    sFont_2CEEE8* f = font_2CEEE8(item);
    return cFont_textExtent(f, str, 0, 0, f->drawScaleX, f->drawScaleY);
}

#define MAX_2CEEE8(a, b) ((a) > (b) ? (a) : (b))

/**
 * Computes a colour-channel item's row rectangle: the widest channel name, the value text and room for the
 * bar.
 *
 * Width = the widest of the "Alpha", "Red" and "Green" labels ("Blue" is not measured) + the channel value
 * formatted as "%.3f   999" + 144; height = the menu font's MaxCharHeight * BaseScale.y + 12. The tRect is
 * returned by value through @p ret.
 *
 * @param ret   the hidden result pointer (the tRect to fill)
 * @param item  the colour-channel menu item (this)
 * @return @p ret
 */
extern "C" sRect_2CEEE8* cColorMenuItem_preRender(sRect_2CEEE8* ret, sMenuValueItem_2CEEE8* item)
{
    sRect_2CEEE8 rect;
    char buf[64];
    sprintf(buf, str_pct_3f_999, fptodp(*(float*)((char*)item->values + (item->index << 2))));
    float z = 0.0f;
    rect.y = z;
    rect.x = z;
    sFont_2CEEE8* font = font_2CEEE8(item);
    rect.h = (float)font->lineHeight * font->scaleY + 12.0f;
    rect.w = MAX_2CEEE8(MAX_2CEEE8(textWidth_2CEEE8(item, ColorNames[0]), textWidth_2CEEE8(item, ColorNames[1])),
                     textWidth_2CEEE8(item, ColorNames[2]));
    rect.w = rect.w + textWidth_2CEEE8(item, buf) + 144.0f;
    *ret = rect;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern "C" int fptodp(float f);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* ColorNames[];
extern char str_pct_3f_pct_03d[];


struct sVec2_2CF1C8 {
    float x, y;
    sVec2_2CF1C8(float ax, float ay) { x = ax; y = ay; }
};

struct sRect_2CF1C8 {
    float x, y, w, h;
};

struct sVec4_2CF1C8 {
    float x, y, z, w;
    sVec4_2CF1C8() {}
    void Set(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sVtx_2CF1C8 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sVec4_2CF1C8 pos;
    sVtx_2CF1C8() {}
};

struct sRS_2CF1C8 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
};

class cCtx_2CF1C8 {
public:
    char pad0[0xE84];
    sRS_2CF1C8* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, void* verts, int flags);
};


/**
 * Draws a colour-channel item: its channel name, its value and a gradient bar with a marker at the value.
 *
 * Draws the row background through the menu (slot 9, focused or not), the channel name ("Alpha", "Red",
 * "Green", "Blue") on the left, "%.3f  %03d" (value, value * 255.99) right-aligned before the bar, an 80 x 10
 * bar fading from the colour with this channel at 0 to the colour with it at 1, and a triangle above the bar
 * at the value.
 *
 * The bar uses render state 5 instead of 1 when the colour's alpha is not 1. Unlike cMenuItem_textRender it leaves
 * the menu font's ShadowOffset at (2, 2).
 *
 * @param rect  the row rectangle (x, y, w, h)
 */
extern "C" void cColorMenuItem_render(cColorMenuItem* self, float* rect)
{
    {
        cMenu* menu = (cMenu*)(*(char**)self);
        sVEntry<void(void*, float*, int)>* vt = (sVEntry<void(void*, float*, int)>*)menu->mVtbl;
        char* thisp = ((char*)menu) + vt[9].delta;
        vt[9].fn(thisp, rect, ((void*)self) == cMenu_getFocusItem((cMenu*)menu));
    }
    float barX = rect[0] + rect[2] - 16.0f - 80.0f;
    float textX = barX - 16.0f;
    cMenuItem_textRender1_sVec3* col = ((void*)self) == cMenu_getFocusItem((cMenu*)(*(void**)self)) ? &kTextSelectedColor : &kTextUnselectedColor;
    cMenuItem_textRender1_sColor textClr;
    textClr.a = 1.0f;
    textClr.r = col->x;
    textClr.g = col->y;
    textClr.b = col->z;
    *(cMenuItem_textRender1_sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = textClr;
    *(sVec2_2CF1C8*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CF1C8(2.0f, 2.0f);
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f,
                  ColorNames[self->Adjust]);
    char buf[0x10];
    sprintf(buf, str_pct_3f_pct_03d,
            fptodp(*(float*)((char*)self->Color + (self->Adjust << 2))),
            (int)(*(float*)((char*)self->Color + (self->Adjust << 2)) * 255.99000549316406f));
    char* font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
    float w = cFont_textExtent(font, buf, 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
    cFont_text(font, textX - w, rect[1] + 6.0f - 1.0f, buf);

    cMenuItem_textRender1_sColor rampStart = *(cMenuItem_textRender1_sColor*)self->Color;
    cMenuItem_textRender1_sColor rampEnd = *(cMenuItem_textRender1_sColor*)self->Color;
    *(float*)((char*)&rampStart + (self->Adjust << 2)) = 0.0f;
    *(float*)((char*)&rampEnd + (self->Adjust << 2)) = 1.0f;
    sRect_2CF1C8 barRect;
    barRect.x = barX;
    barRect.y = rect[1] + rect[3] - 6.0f - 10.0f;
    barRect.w = 80.0f;
    barRect.h = 10.0f;
    sVtx_2CF1C8 verts[4];
    float zero = 0.0f;
    float one = 1.0f;
    sVec4_2CF1C8 vertPos;
    verts[0].r = (int)(rampStart.r * 128.0f);
    verts[0].g = (int)(rampStart.g * 128.0f);
    verts[0].b = (int)(rampStart.b * 128.0f);
    verts[0].a = 0x80;
    verts[0].v = zero;
    verts[0].u = zero;
    verts[0].q = one;
    vertPos.Set(barRect.x, barRect.y, zero, one);
    verts[0].pos = vertPos;
    verts[1].r = (int)(rampStart.r * 128.0f);
    verts[1].g = (int)(rampStart.g * 128.0f);
    verts[1].b = (int)(rampStart.b * 128.0f);
    verts[1].a = 0x80;
    verts[1].v = zero;
    verts[1].u = zero;
    verts[1].q = one;
    vertPos.Set(barRect.x, barRect.y + barRect.h, zero, one);
    verts[1].pos = vertPos;
    verts[2].r = (int)(rampEnd.r * 128.0f);
    verts[2].g = (int)(rampEnd.g * 128.0f);
    verts[2].b = (int)(rampEnd.b * 128.0f);
    verts[2].a = 0x80;
    verts[2].v = zero;
    verts[2].u = zero;
    verts[2].q = one;
    vertPos.Set(barRect.x + barRect.w, barRect.y, zero, one);
    verts[2].pos = vertPos;
    verts[3].r = (int)(rampEnd.r * 128.0f);
    verts[3].g = (int)(rampEnd.g * 128.0f);
    verts[3].b = (int)(rampEnd.b * 128.0f);
    verts[3].a = 0x80;
    verts[3].v = zero;
    verts[3].u = zero;
    verts[3].q = one;
    vertPos.Set(barRect.x + barRect.w, barRect.y + barRect.h, zero, one);
    verts[3].pos = vertPos;
    if (self->Color->A != 1.0f) {
        ((cCtx_2CF1C8*)GrMan)->top->f4_2 = 5;
    } else {
        ((cCtx_2CF1C8*)GrMan)->top->f4_2 = 1;
    }
    *(short*)((char*)((cCtx_2CF1C8*)GrMan)->top + 0x10) = -1;
    GrMan->RenderTriStrip(4, (tBxVertex*)verts, 0);

    float mx = barRect.x + barRect.w * *(float*)((char*)self->Color + (self->Adjust << 2));
    verts[0].r = (int)(textClr.r * 128.0f);
    verts[0].g = (int)(textClr.g * 128.0f);
    verts[0].b = (int)(textClr.b * 128.0f);
    vertPos.x = mx - 5.0f; vertPos.y = barRect.y - 7.0f; vertPos.z = 0.0f; vertPos.w = 1.0f;
    verts[0].pos = vertPos;
    verts[1].r = (int)(textClr.r * 128.0f);
    verts[1].g = (int)(textClr.g * 128.0f);
    verts[1].b = (int)(textClr.b * 128.0f);
    vertPos.x = mx; vertPos.y = barRect.y - 2.0f; vertPos.z = 0.0f; vertPos.w = 1.0f;
    verts[1].pos = vertPos;
    verts[2].r = (int)(textClr.r * 128.0f);
    verts[2].g = (int)(textClr.g * 128.0f);
    verts[2].b = (int)(textClr.b * 128.0f);
    vertPos.x = mx + 5.0f; vertPos.y = barRect.y - 7.0f; vertPos.z = 0.0f; vertPos.w = 1.0f;
    verts[2].pos = vertPos;
    GrMan->RenderTriStrip(3, (tBxVertex*)verts, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItemMenu_onCommand);
#ifdef SKIP_ASM
class cExpandMenuItemMenu_onCommand_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int onCommand(int a, int b);
    virtual int v10(int a);
};

/**
 * Passes a command from an expandable item's inner menu on to the menu that holds the expandable item.
 *
 * Calls the owning cExpandMenuItem (+0x130) through its vtable slot 9, whose handler calls the parent menu's
 * onCommand with the same two arguments.
 *
 * @param item  the inner menu item the command came from
 * @param cmdArg     the command's argument (item-specific: 0, or a word of the item's data; On Tour arg)
 * @return the parent menu's tMIR result
 */
extern "C" int cExpandMenuItemMenu_onCommand(cExpandMenuItemMenu* self, int item, int cmdArg)
{
    return ((cExpandMenuItemMenu_onCommand_cItem*)self->Item)->onCommand(item, cmdArg);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItemMenu_onChange);
#ifdef SKIP_ASM
class cExpandMenuItemMenu_onChange_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09(int a, int b);
    virtual void onChange(int a);
};

/**
 * Reports a value change in an expandable item's inner menu to the menu that holds the expandable item.
 *
 * Calls the owning cExpandMenuItem (+0x130) through its vtable slot 10, whose handler calls the parent menu's
 * onChange with the expandable item itself, so @p item goes no further.
 *
 * @param item  the inner menu item whose value changed
 */
extern "C" void cExpandMenuItemMenu_onChange(cExpandMenuItemMenu* self, int item)
{
    ((cExpandMenuItemMenu_onChange_cItem*)self->Item)->onChange(item);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_cExpandMenuItem1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cMenu_cMenu(void* self);
extern void* cExpandMenuItem_vtbl[];
extern void* cExpandMenuItemMenu_vtbl[];
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

// Element type of an inline-constructed array; its empty constructor leaves only the loop counters.
struct sMenuSlot_F7E8 {
    void* item;
    sMenuSlot_F7E8() {}
    // NOTE: class-scope placement new[] only so this candidate can be tested before the unit's global
    // inline operator new[] (defined later, in cExpandMenuItem_cExpandMenuItem's block) is moved above this function.
    static void* operator new[](unsigned int, void* p) { return p; }
};

/**
 * Constructs an empty expandable item: ID -1, no title (+0x14), and its embedded sub-menu (+0x18, a cMenu with the
 * cExpandMenuItemMenu vtable) pointing back at the item (+0x148).
 *
 * Stores cExpandMenuItem_vtbl, runs cMenu_cMenu on +0x18, then stores cExpandMenuItemMenu_vtbl at +0x144 (the
 * sub-menu's vtable slot) and runs the empty constructor loop over the 70 rects at +0x24 (as written; On Tour has the
 * rects at +0x14C).
 *
 * @return @p self
 */
extern "C" void* cExpandMenuItem_cExpandMenuItem1(cExpandMenuItem* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Title = 0;
    self->mVtbl = cExpandMenuItem_vtbl;
    cMenu_cMenu(&self->ExpandMenu);
    self->ExpandMenu.mVtbl = cExpandMenuItemMenu_vtbl;
    new ((char*)&self->ExpandMenu.Items[0]) sMenuSlot_F7E8[70];
    self->ExpandMenu.Item = (cExpandMenuItem*)((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_cExpandMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new[](unsigned int, void* p) { return p; }
void* cMenu_cMenu(void* self);
extern void* cExpandMenuItem_vtbl[];
extern void* cExpandMenuItemMenu_vtbl[];
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");

// Element type of an inline-constructed array; its empty constructor leaves only the loop counters.
struct sMenuSlot_F860 {
    void* item;
    sMenuSlot_F860() {}
};

/**
 * Constructs an expandable menu item: an item that holds its own inner menu of sub-items.
 *
 * Runs the base constructor with @p name as the ID, stores @p parent at +0x14 and the vtable, constructs the
 * inner cMenu at +0x18 with cExpandMenuItemMenu's vtable and a back pointer to this item (+0x148), and adds
 * @p parent to the inner menu as its first row when it is set.
 *
 * @param name    the item's ID (On Tour id), passed to the base constructor
 * @param parent  despite the name, the title item shown as the inner menu's first row (On Tour title), or 0
 * @return @p self
 */
extern "C" void* cExpandMenuItem_cExpandMenuItem(cExpandMenuItem* self, void* name, void* parent)
{
    cMenuItem_cMenuItem((cMenuItem*)self, name);
    self->Title = (cMenuItem*)parent;
    self->mVtbl = cExpandMenuItem_vtbl;
    cMenu_cMenu(&self->ExpandMenu);
    self->ExpandMenu.mVtbl = cExpandMenuItemMenu_vtbl;
    new ((char*)&self->ExpandMenu.Items[0]) sMenuSlot_F860[70];
    self->ExpandMenu.Item = (cExpandMenuItem*)((void*)self);
    if (self->Title != 0) {
        cExpandMenuItem_addItem3(self, self->Title, -1);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_zeroValue);
#ifdef SKIP_ASM
class cExpandMenuItem_zeroValue_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void zeroValue();
};

extern "C" void* cMenu_getFocusItem(cMenu* self);

/**
 * Resets the value of the item that has the focus inside an expandable menu item.
 *
 * Gets the focus item of the menu embedded at +0x18 (cMenu_getFocusItem) and calls its virtual slot 8.
 */
extern "C" void cExpandMenuItem_zeroValue(cExpandMenuItem* self)
{
    ((cExpandMenuItem_zeroValue_cItem*)cMenu_getFocusItem((cMenu*)(&self->ExpandMenu)))->zeroValue();
}
#endif

// PORT: cMenu_addItem is extern "C" (list, item, index); this caller leaves $a2 as-is.
void cMenu_addItem2(void* menu, void* item) __asm__("cMenu_addItem");

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_addItem__FPvT0);
#ifdef SKIP_ASM
/**
 * Adds an item to an expandable menu item's embedded sub-menu (at +0x18) through cMenu_addItem2().
 *
 * The third argument register (the insert index, -1 to append) passes through untouched.
 *
 * @param item  the menu item to add
 */
void cExpandMenuItem_addItem(void* vself, void* item)
{
    cExpandMenuItem* self = (cExpandMenuItem*)vself;
    cMenu_addItem2(&self->ExpandMenu, item);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_remItem);
#ifdef SKIP_ASM
extern "C" void cMenu_remItem(int);

/**
 * Removes an item from an expandable item's sub-menu: forwards to cMenu_remItem on the embedded menu (+0x18).
 *
 * The position argument passes through in the second register; this definition does not declare it.
 *
 * @param expandItem  the expandable item
 */
extern "C" void cExpandMenuItem_remItem(int expandItem) {
    cMenu_remItem(expandItem + 0x18);
}
#endif
//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_remAllItems);
#ifdef SKIP_ASM
/**
 * Empties an expandable item's sub-menu (cMenu_remAllItems on +0x18), then adds the title item (+0x14) back to it, when
 * there is one.
 */
extern "C" void cExpandMenuItem_remAllItems(cExpandMenuItem* self)
{
    cMenu_remAllItems((cMenu*)(&self->ExpandMenu));
    void* item = self->Title;
    if (item != 0) {
        cExpandMenuItem_addItem3(self, item, -1);
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_setTitle);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMenu_remItem_pi(void* list, int index) __asm__("cMenu_remItem");
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");

/**
 * Replaces an expandable item's title: removes the old title (+0x14) from the sub-menu (+0x18) if it is there, stores
 * @p item and adds it at position 0 (cExpandMenuItem_addItem).
 *
 * @param item  the new title item
 */
extern "C" void cExpandMenuItem_setTitle(cExpandMenuItem* self, cMenuItem* item)
{
    for (int i = 0; i < ((cMenuList*)(&self->ExpandMenu))->count; i++) {
        if (((cMenuList*)(&self->ExpandMenu))->items[i] == (void*)self->Title) {
            cMenu_remItem_pi(&self->ExpandMenu, i);
            break;
        }
    }
    self->Title = (cMenuItem*)((void*)item);
    cExpandMenuItem_addItem3(self, item, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isLocked(cMenuItem* self);
extern "C" void cMenu_checkFocus(cMenu* self);
extern "C" void cMenu_nextFocus(cMenu* self);
extern "C" void cMenu_prevFocus(cMenu* self);

class cExpandMenuItem_input_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual int input();
};


/**
 * Handles input for an expandable menu item by passing it to the focused item of its embedded menu.
 *
 * Checks the embedded menu's focus, then calls the focused item's input (slot 2). If this item is locked, the
 * result is returned as is. Result 3 moves to the next sub-item and 4 to the previous one; when that wraps
 * around, the focus is put back and the 3 or 4 is returned so the parent menu moves on instead.
 *
 * @param expandItem  the expandable menu item
 * @return 0 when the move was handled inside the sub-menu, otherwise the focused item's input result (tMIR)
 */
// PORT: returns a key code (int) through the unit's void* declaration.
extern "C" void* cExpandMenuItem_input(cExpandMenuItem* expandItem)
{
    cExpandMenuItem* self = expandItem;
    cMenu_checkFocus((cMenu*)(&(*(cMenu_setFocus_sMenu*)&self->ExpandMenu)));
    int prev = (*(cMenu_setFocus_sMenu*)&self->ExpandMenu).selected;
    int key = ((cExpandMenuItem_input_cItem*)(*(cMenu_setFocus_sMenu*)&self->ExpandMenu).items[prev])->input();
    if (cMenuItem_isLocked((cMenuItem*)self) != 0) {
        return (void*)key;
    }
    switch (key) {
    case 3:
        cMenu_nextFocus((cMenu*)(&(*(cMenu_setFocus_sMenu*)&self->ExpandMenu)));
        if (prev < (*(cMenu_setFocus_sMenu*)&self->ExpandMenu).selected) {
            return 0;
        }
        cMenu_setFocus(&(*(cMenu_setFocus_sMenu*)&self->ExpandMenu), prev);
        return (void*)3;
    case 4:
        cMenu_prevFocus((cMenu*)(&(*(cMenu_setFocus_sMenu*)&self->ExpandMenu)));
        if ((*(cMenu_setFocus_sMenu*)&self->ExpandMenu).selected < prev) {
            return 0;
        }
        cMenu_setFocus(&(*(cMenu_setFocus_sMenu*)&self->ExpandMenu), prev);
        return (void*)4;
    }
    return (void*)key;
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_onCommand);
#ifdef SKIP_ASM
class cExpandMenuItem_onCommand_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int onCommand(int a, int b);
};

/**
 * Passes a command from an item inside an expandable menu item on to the menu that owns it.
 *
 * Calls virtual slot 3 (cMenu onCommand) of the parent menu stored at +0x0 with @p item and @p commandArg and returns its result.
 *
 * @param item  the item that issued the command (On Tour item)
 * @param commandArg  the command argument (On Tour arg)
 * @return the parent menu's result
 */
extern "C" int cExpandMenuItem_onCommand(cExpandMenuItem_onCommand_cMenu** self, int item, int commandArg)
{
    return (*self)->onCommand(item, commandArg);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_onChange);
#ifdef SKIP_ASM
class cExpandMenuItem_onChange_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void onChange(void*);
};

/**
 * Tells the parent menu that this expandable menu item's value changed.
 *
 * Calls virtual slot 4 (cMenu onChange) of the parent menu stored at +0x0 with this item.
 */
extern "C" void cExpandMenuItem_onChange(cExpandMenuItem_onChange_cMenu** self)
{
    (*self)->onChange(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isActive(cMenuItem* self);
extern "C" void* cMenu_getFocusItem(cMenu* self);

struct sMenuRect_2CFB58 { float x, y, z, w; };

struct sVEnt_2CFB58 { short delta; short index; sMenuRect_2CFB58 (*fn)(void*); };

struct sItem_2CFB58 {
    char pad00[0x10];
    sVEnt_2CFB58* vt;
};

struct sMenu_2CFB58 {
    char* owner;
    char pad04[0x10];
    sItem_2CFB58* sel;
    int count;
    char pad1C[8];
    sItem_2CFB58* items[70];
    void* f13C;
    char pad140[0xC];
    sMenuRect_2CFB58 rects[1];
};

/**
 * Measures an expandable menu item, including its sub-items when it is expanded.
 *
 * Copies the menu system from the parent menu, then for each active sub-item stores the rect its virtual slot 4
 * returns. Heights are summed while the item has the focus (or for the title item), widths take the widest item, plus
 * 20 for indented sub-items. The total is returned through @p ret.
 *
 * @param ret  where the total rect is returned (hidden struct-return pointer)
 * @return @p ret
 */
extern "C" sMenuRect_2CFB58* cExpandMenuItem_preRender(sMenuRect_2CFB58* ret, sMenu_2CFB58* self)
{
    sMenuRect_2CFB58 total;
    float zero = 0.0f;
    total.w = zero;
    total.z = zero;
    total.y = zero;
    total.x = zero;
    self->f13C = *(void**)(self->owner + 0x124);
    for (int i = 0; i < self->count; i++) {
        if (cMenuItem_isActive((cMenuItem*)self->items[i])) {
            sVEnt_2CFB58* vt = self->items[i]->vt;
            self->rects[i] = vt[4].fn(vthis(self->items[i], vt[4]));
            if (self == cMenu_getFocusItem((cMenu*)self->owner) || self->items[i] == self->sel) {
                total.w += self->rects[i].w;
            }
            float itemWidth = self->rects[i].z;
            if (self->items[i] != self->sel) {
                itemWidth += 20.0f;
            }
            if (total.z < itemWidth) {
                total.z = itemWidth;
            }
        } else {
            sMenuRect_2CFB58 z;
            z.x = 0;
            z.y = 0;
            z.z = 0;
            z.w = 0;
            self->rects[i] = z;
        }
    }
    *ret = total;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_render);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isActive(cMenuItem* self);
extern "C" void* cMenu_getFocusItem(cMenu* self);

struct cExpandMenuItem_render_sRect {
    float x, y, w, h;
};

class cExpandMenuItem_render_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void render(cExpandMenuItem_render_sRect* r);
};

struct cExpandMenuItem_render_sOwner {
    void* menu;                         // 0x0
    char pad4[0x10];
    cExpandMenuItem_render_cItem* cur;           // 0x14
    int count;                          // 0x18
    int selected;                       // 0x1C
    int unk20;                          // 0x20
    cExpandMenuItem_render_cItem* items[70];     // 0x24
    int wrap;                           // 0x13C
    char pad140[0xC];
    cExpandMenuItem_render_sRect rects[70];      // 0x14C
};

/**
 * Draws an expandable menu item: its title and, while it has the focus, its sub-items.
 *
 * Starting from @p irect, each active item that is the title, or every active item while this item has its parent menu's
 * focus, gets a rect indented by 20 (none for the title), is drawn through its virtual slot 5 and moves the next
 * position down by its height.
 *
 * @param irect  the area to draw in (On Tour irect)
 */
extern "C" void cExpandMenuItem_render(cExpandMenuItem_render_sOwner* self, cExpandMenuItem_render_sRect* irect)
{
    cExpandMenuItem_render_sRect pos = *irect;
    for (int i = 0; i < self->count; i++) {
        if (cMenuItem_isActive((cMenuItem*)self->items[i]) == 0) {
            continue;
        }
        if ((void*)self == cMenu_getFocusItem((cMenu*)self->menu) || self->items[i] == self->cur) {
            int off = self->items[i] == self->cur ? 0 : 20;
            self->rects[i].x = pos.x + (float)off;
            self->rects[i].y = pos.y;
            self->rects[i].w = pos.w - (float)off;
            self->items[i]->render(&self->rects[i]);
            pos.y += self->rects[i].h;
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_gainFocus);
#ifdef SKIP_ASM
// PORT: the target leaves mode in $a1 for cMenuItem_gainFocus; bind a 2-arg form to that symbol.
void cMenuItem_gainFocus_2(void* self, int a1) __asm__("cMenuItem_gainFocus__FPv");
extern "C" void cMenu_setFocus_v(void* self, int index) __asm__("cMenu_setFocus");

/**
 * Gives an expandable menu item the focus and selects the sub-item matching the direction it was entered from.
 *
 * Calls cMenuItem_gainFocus, then sets the embedded menu (+0x18) focus to the first item for @p mode 0 or the last (-1)
 * for 1, and calls cMenu_onGainFocus with the parent menu's menu system.
 *
 * @param mode  how the focus arrived (On Tour tMIGainFocus whence)
 */
extern "C" void cExpandMenuItem_gainFocus(cExpandMenuItem* self, int mode)
{
    cMenuItem_gainFocus_2(self, mode);
    switch (mode) {
    case 0:
        cMenu_setFocus_v(&self->ExpandMenu, 0);
        break;
    case 1:
        cMenu_setFocus_v(&self->ExpandMenu, -1);
        break;
    }
    cMenu_onGainFocus((cMenu*)(&self->ExpandMenu), (int)self->Menu->MenuSys);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_loseFocus);
#ifdef SKIP_ASM
extern "C" void cMenu_onLoseFocus(cMenu* p);

/**
 * Takes the focus away from an expandable menu item and its sub-menu.
 *
 * Calls cMenuItem_loseFocus (base cMenuItem loseFocus) and cMenu_onLoseFocus on the menu embedded at +0x18.
 */
extern "C" void cExpandMenuItem_loseFocus(cExpandMenuItem* self)
{
    cMenuItem_loseFocus(self);
    cMenu_onLoseFocus((cMenu*)(&self->ExpandMenu));
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_cRGBTitleMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cRGBTitleMenuItem_vtbl[];


/**
 * Constructs the title row of a colour editor menu item.
 *
 * Runs the base constructor with ID -1, stores the colour at +0x14 and the title at +0x18, then
 * cRGBTitleMenuItem's vtable. cARGBMenuItem_cARGBMenuItem() builds one as its expand title.
 *
 * @param cap  the title text, stored at +0x18
 * @param color  the colour the row shows, stored at +0x14
 * @return @p self
 */
extern "C" cRGBTitleMenuItem* cRGBTitleMenuItem_cRGBTitleMenuItem(cRGBTitleMenuItem* self, void* cap, tARGB* color)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Color = (tARGB*)((void*)color);
    self->Caption = (const char*)cap;
    self->mVtbl = (void**)cRGBTitleMenuItem_vtbl;
    return (self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_canHaveFocus__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether this menu item can take the focus: never.
 *
 * @return 0
 */
int cRGBTitleMenuItem_canHaveFocus(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_preRender);
#ifdef SKIP_ASM
struct cRGBTitleMenuItem_preRender_sRect {
    float x;
    float y;
    float w;
    float h;
};

/**
 * Computes a colour title row's rectangle: the title as left text, 36 wider for the swatch and at least 29
 * high.
 *
 * Measures with cMenuItem_textPreRender and returns the tRect by value through @p out.
 *
 * @param out   the hidden result pointer (the tRect to fill)
 * @param item  the colour title item (this)
 * @return @p out
 */
// PORT: `>?` (g++ max operator); returns the rect through the hidden result pointer.
extern "C" cRGBTitleMenuItem_preRender_sRect* cRGBTitleMenuItem_preRender(cRGBTitleMenuItem_preRender_sRect* out, cRGBTitleMenuItem* item)
{
    cRGBTitleMenuItem_preRender_sRect rect;
    cMenuItem_textPreRender(&rect, item, (int)item->Caption, 0, 0);
    rect.w += 36.0f;
    rect.h = rect.h >? 29.0f;
    *out = rect;
    return out;
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_render);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the body reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");



struct sVec4_2D0008 {
    float x, y, z, w;
    sVec4_2D0008(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));


struct sRS_2D0008 {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    short f10;
};

class cCtx_2D0008 {
public:
    char pad0[0xE84];
    sRS_2D0008* top;
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int prim, void* verts, int n);
};


/**
 * Draws a colour title row: the title on the left and a 20 x 17 swatch of the colour at the right edge.
 *
 * The text goes through cMenuItem_textRender1 (row background, focused or normal colours, shadow). The swatch is a
 * 4-vertex primitive in the colour's channels (alpha included, scaled to 0-128) drawn through GrMan's slot
 * 71, with render state 5 instead of 1 when the alpha is not 1.
 *
 * @param rect  the row rectangle (x, y, w, h)
 */
// PORT: rect pointer passed through cMenuItem_textRender1's int parameter
extern "C" void cRGBTitleMenuItem_render(cRGBTitleMenuItem* self, float* rect)
{
    cMenuItem_textRender1_5(((char*)self), (int)rect, (int)self->Caption, 0, 0);
    tBxVertex verts[4];
    tRect rc;
    rc.W = 20.0f;
    rc.H = 17.0f;
    rc.X = rect[0] + rect[2] - 16.0f - rc.W;
    rc.Y = rect[1] + 6.0f + 2.0f;
    float x2 = rc.X + rc.W;
    float y2 = rc.Y + rc.H;
    tARGB* colour = self->Color;
    float zeroF = 0.0f;
    float one = 1.0f;
    verts[0].r = (int)(colour->R * 128.0f);
    verts[0].g = (int)(colour->G * 128.0f);
    verts[0].b = (int)(colour->B * 128.0f);
    verts[0].a = (int)(colour->A * 128.0f);
    verts[0].v = zeroF;
    verts[0].u = zeroF;
    verts[0].one = one;
    *(sVec4_2D0008*)((float*)&verts[0].p) = sVec4_2D0008(rc.X, rc.Y, zeroF, one);
    verts[1].r = (int)(colour->R * 128.0f);
    verts[1].g = (int)(colour->G * 128.0f);
    verts[1].b = (int)(colour->B * 128.0f);
    verts[1].a = (int)(colour->A * 128.0f);
    verts[1].v = zeroF;
    verts[1].u = zeroF;
    verts[1].one = one;
    *(sVec4_2D0008*)((float*)&verts[1].p) = sVec4_2D0008(rc.X, y2, zeroF, one);
    verts[2].r = (int)(colour->R * 128.0f);
    verts[2].g = (int)(colour->G * 128.0f);
    verts[2].b = (int)(colour->B * 128.0f);
    verts[2].a = (int)(colour->A * 128.0f);
    verts[2].v = zeroF;
    verts[2].u = zeroF;
    verts[2].one = one;
    *(sVec4_2D0008*)((float*)&verts[2].p) = sVec4_2D0008(x2, rc.Y, zeroF, one);
    verts[3].r = (int)(colour->R * 128.0f);
    verts[3].g = (int)(colour->G * 128.0f);
    verts[3].b = (int)(colour->B * 128.0f);
    verts[3].a = (int)(colour->A * 128.0f);
    verts[3].v = zeroF;
    verts[3].u = zeroF;
    verts[3].one = one;
    *(sVec4_2D0008*)((float*)&verts[3].p) = sVec4_2D0008(x2, y2, zeroF, one);
    if (colour->A != 1.0f) {
        ((cCtx_2D0008*)GrMan)->top->f4_2 = 5;
    } else {
        ((cCtx_2D0008*)GrMan)->top->f4_2 = 1;
    }
    *(short*)((char*)((cCtx_2D0008*)GrMan)->top + 0x10) = -1;
    GrMan->RenderTriStrip(4, verts, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cARGBMenuItem_cARGBMenuItem);
#ifdef SKIP_ASM
extern "C" void* cExpandMenuItem_cExpandMenuItem(cExpandMenuItem* self, void* name, void* title);
// PORT: cExpandMenuItem_addItem forwards $a2 (insert index) to cMenu_addItem; bound with that third argument.
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");
extern void* cARGBMenuItem_vtbl[];

/**
 * Constructs a colour editor menu item: an expandable item with a colour title row and one slider per
 * channel.
 *
 * Builds the cExpandMenuItem base with the embedded title row (+0x640) as its first row, stores @p color at
 * +0x5AC and the vtable, constructs cColorMenuItems at +0x5B0, +0x5D4, +0x5F8 and +0x61C for channels 0-3
 * (alpha, red, green, blue), constructs the title row, then adds the four channel items.
 *
 * The title row is added before its constructor runs; that works because cMenuItem's constructor leaves the
 * Menu word (+0x0) alone.
 *
 * @param name   the item's ID (On Tour id)
 * @param title  the title text (On Tour caption)
 * @param color  the tARGB colour the item edits
 * @return @p self
 */
extern "C" void* cARGBMenuItem_cARGBMenuItem(cARGBMenuItem* self, void* name, void* title, tARGB* color)
{
    cExpandMenuItem_cExpandMenuItem((cExpandMenuItem*)self, name, &self->TitleItem);
    self->Color = (tARGB*)((void*)color);
    self->mVtbl = cARGBMenuItem_vtbl;
    // PORT: the channel index is passed in the unit's void* parameter
    cColorMenuItem_cColorMenuItem((cColorMenuItem*)(((&self->Alpha))), color, (void*)0);
    cColorMenuItem_cColorMenuItem((cColorMenuItem*)(((&self->Red))), color, (void*)1);
    cColorMenuItem_cColorMenuItem((cColorMenuItem*)(((&self->Green))), color, (void*)2);
    cColorMenuItem_cColorMenuItem((cColorMenuItem*)(((&self->Blue))), color, (void*)3);
    cRGBTitleMenuItem_cRGBTitleMenuItem((cRGBTitleMenuItem*)(((&self->TitleItem))), title, (tARGB*)color);
    cExpandMenuItem_addItem3(self, &self->Alpha, -1);
    cExpandMenuItem_addItem3(self, &self->Red, -1);
    cExpandMenuItem_addItem3(self, &self->Green, -1);
    cExpandMenuItem_addItem3(self, &self->Blue, -1);
    return ((void*)self);
}
#endif

extern "C" void* cExpandMenuItem_input(cExpandMenuItem* self);

//99.29%
INCLUDE_ASM("util/menu", cARGBMenuItem_input__FPv);
#ifdef SKIP_ASM
/**
 * Handles input for an ARGB menu item using cExpandMenuItem_input().
 *
 * @return the result of cExpandMenuItem_input()
 */
void* cARGBMenuItem_input(void* self)
{
    return cExpandMenuItem_input((cExpandMenuItem*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorTitleMenuItem_cVectorTitleMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cVectorTitleMenuItem_vtbl[];

/**
 * Constructs a vector title item: ID -1, caption (+0x18) @p caption and the vector (+0x14) @p vector that it shows; vtable
 * cVectorTitleMenuItem_vtbl.
 *
 * cVectorMenuItem_cVectorMenuItem builds its title row (+0x654) with it.
 *
 * @param caption  caption
 * @param vector  the vector (three floats) shown
 * @return @p self
 */
extern "C" void* cVectorTitleMenuItem_cVectorTitleMenuItem(cVectorTitleMenuItem* self, int caption, int vector)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->Vector = (t3Vector*)vector;
    self->Caption = (const char*)caption;
    self->mVtbl = cVectorTitleMenuItem_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorTitleMenuItem_canHaveFocus__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: a vector title item never takes the focus.
 *
 * @return 0
 */
int cVectorTitleMenuItem_canHaveFocus(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cVectorTitleMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void cMenuItem_textPreRender(void*, void*, int, int, void*);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
extern char str_pct_2f_pct_2f_pct_2f[];

/**
 * Measures a vector title item: its caption on the left and the vector at +0x14 printed with "%.2f, %.2f, %.2f" on the
 * right (cMenuItem_textPreRender).
 *
 * @param ret  where the rect is returned
 * @param item  the vector title item
 * @return @p ret
 */
extern "C" void* cVectorTitleMenuItem_preRender(void* ret, cVectorTitleMenuItem* item)
{
    char buf[0x80];
    const char* fmt = str_pct_2f_pct_2f_pct_2f;
    int x = fptodp(item->Vector->x);
    int y = fptodp(item->Vector->y);
    int z = fptodp(item->Vector->z);
    sprintf(buf, fmt, x, y, z);
    cMenuItem_textPreRender(ret, item, (int)item->Caption, 0, buf);
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cVectorTitleMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
extern char str_pct_2f_pct_2f_pct_2f[];
// PORT: the unit's 4-arg declaration of cMenuItem_textRender1 is wrong: the callee reads $8 (5th arg).
void* cMenuItem_textRender1_5(void*, int, int, int, void*) __asm__("cMenuItem_textRender1");

/**
 * Draws a vector title item: its caption on the left and the vector at +0x14 printed with "%.2f, %.2f, %.2f" on the
 * right (cMenuItem_textRender1).
 *
 * @param rect  the row's rect
 */
extern "C" void cVectorTitleMenuItem_render(cVectorTitleMenuItem* self, int rect)
{
    char buf[0x80];
    const char* fmt = str_pct_2f_pct_2f_pct_2f;
    int x = fptodp(self->Vector->x);
    int y = fptodp(self->Vector->y);
    int z = fptodp(self->Vector->z);
    sprintf(buf, fmt, x, y, z);
    cMenuItem_textRender1_5(self, rect, (int)self->Caption, 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorMenuItem_cVectorMenuItem);
#ifdef SKIP_ASM
extern "C" void* cVectorTitleMenuItem_cVectorTitleMenuItem(cVectorTitleMenuItem* self, int a1, int a2);
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");
// PORT: the unit's definition types the value pointer and label as int; they are pointers.
void* cFloatMenuItem_ptr(void* self, int index, float* value, void* label, float f1, float f2) __asm__("cFloatMenuItem_cFloatMenuItem");
extern void* cVectorMenuItem_vtbl[];
extern char D_004A39D8[];
extern char D_004A39E0[];
extern char D_004A39E8[];

/**
 * Constructs a debug menu item that edits the three components of a vector.
 *
 * Builds the cExpandMenuItem base with the title item at +0x654, keeps the vector at +0x5AC, installs
 * cVectorMenuItem_vtbl and makes three cFloatMenuItem children (ids 0 to 2, labels D_004A39D8..D_004A39E8) bound to x,
 * y and z with the given range. The title item is built from the caption and the vector (cVectorTitleMenuItem_cVectorTitleMenuItem), the three
 * children are added at the end and +0x670 is cleared.
 *
 * @param name  the menu item id (On Tour id; passed on as the cExpandMenuItem id, not a name)
 * @param values  the vector to edit, as three floats (On Tour v)
 * @param caption  the title caption (On Tour caption)
 * @param minValue  smallest value each component may take
 * @param maxValue  largest value each component may take
 * @return this
 */
extern "C" void* cVectorMenuItem_cVectorMenuItem(cVectorMenuItem* self, void* name, float* values, int caption, float minValue, float maxValue)
{
    cExpandMenuItem_cExpandMenuItem((cExpandMenuItem*)self, name, &self->TitleItem);
    self->Vector = (t3Vector*)values;
    self->mVtbl = cVectorMenuItem_vtbl;
    cFloatMenuItem_ptr(&self->X, 0, values, D_004A39D8, minValue, maxValue);
    cFloatMenuItem_ptr(&self->Y, 1, values + 1, D_004A39E0, minValue, maxValue);
    cFloatMenuItem_ptr(&self->Z, 2, values + 2, D_004A39E8, minValue, maxValue);
    cVectorTitleMenuItem_cVectorTitleMenuItem((cVectorTitleMenuItem*)(&self->TitleItem), caption, (int)values);
    cExpandMenuItem_addItem3(self, &self->X, -1);
    cExpandMenuItem_addItem3(self, &self->Y, -1);
    cExpandMenuItem_addItem3(self, &self->Z, -1);
    self->Normalize = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorMenuItem_input);
#ifdef SKIP_ASM
extern "C" void* cExpandMenuItem_input(cExpandMenuItem* self);

struct sMenuChanD6E8 {
    char pad0[0x18];
    float* target;              // 0x18
    char pad1C[0x24 - 0x1C];
    float scale;                // 0x24
    float offset;               // 0x28
    char pad2C[0x34 - 0x2C];
    float get() { return *target; }
    void set(float v) { *target = v; }
};
struct sMenuVecD6E8 {
    float x, y, z;
    sMenuVecD6E8() {}
    sMenuVecD6E8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sMenuColD6E8 {
    char pad0[0x5B8];
    sMenuChanD6E8 ch[3];        // 0x5B8
    char pad654[0x670 - 0x654];
    int linked;                 // 0x670
};

static inline void menuNormalizeD6E8(sMenuVecD6E8& out, const sMenuVecD6E8& v)
{
    float d = v.x * v.x + v.y * v.y + v.z * v.z;
    float len;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(len) : "f"(d));
    if (len != 0.0f) {
        float inv = 1.0f / len;
        out.x = v.x * inv;
        out.y = v.y * inv;
        out.z = v.z * inv;
    } else {
        out.x = v.x;
        out.y = v.y;
        out.z = v.z;
    }
}

/**
 * Handles input for a vector editor menu item, keeping the vector normalised when its channels are linked.
 *
 * Runs cExpandMenuItem_input(); then, when the linked flag is set, rebuilds the vector from its three channels
 * (scale * value + offset), normalises it and writes the components back.
 *
 * @return cExpandMenuItem_input()'s result
 */
extern "C" void* cVectorMenuItem_input(cVectorMenuItem* self)
{
    void* ret = cExpandMenuItem_input((cExpandMenuItem*)self);
    if (self->Normalize != 0) {
        sMenuVecD6E8 n;
        sMenuVecD6E8 vec(((sMenuChanD6E8*)&self->X)[0].scale * ((sMenuChanD6E8*)&self->X)[0].get() + ((sMenuChanD6E8*)&self->X)[0].offset,
                       ((sMenuChanD6E8*)&self->X)[1].scale * ((sMenuChanD6E8*)&self->X)[1].get() + ((sMenuChanD6E8*)&self->X)[1].offset,
                       ((sMenuChanD6E8*)&self->X)[2].scale * ((sMenuChanD6E8*)&self->X)[2].get() + ((sMenuChanD6E8*)&self->X)[2].offset);
        menuNormalizeD6E8(n, vec);
        ((sMenuChanD6E8*)&self->X)[0].set(n.x);
        ((sMenuChanD6E8*)&self->X)[1].set(n.y);
        ((sMenuChanD6E8*)&self->X)[2].set(n.z);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cGraphMenuItem_cGraphMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cGraphMenuItem_vtbl[];
extern void* D_00486738[];


/**
 * Constructs a graph item: an editable curve of @p numPoints points with per-point axis locks, a slope rule and a range.
 *
 * ID -1, caption (+0x14) "Hello there!" (D_00486738), point count (+0x18) = @p numPoints, x-lock bits (+0x1C) = @p xLockBits, y-lock
 * bits (+0x20) = @p yLockBits, slope rule (+0x3C) = @p slope, points (+0x28) = @p points, minimum point (+0x2C) = *@p minPoint, maximum
 * point (+0x34) = *@p maxPoint, current point (+0x24) and edit mode (+0x40) 0; vtable cGraphMenuItem_vtbl (input cGraphMenuItem_input,
 * preRender cGraphMenuItem_preRender, render cGraphMenuItem_render, gainFocus cGraphMenuItem_gainFocus).
 *
 * @param numPoints  number of points
 * @param xLockBits  bit i set: point i's x cannot move
 * @param yLockBits  bit i set: point i's y cannot move
 * @param slope  slope rule: > 0 keeps the y values rising, < 0 falling, 0 free
 * @param points  the points (x, y floats)
 * @param minPoint  the minimum point (x, y)
 * @param maxPoint  the maximum point (x, y)
 * @return @p self
 */
extern "C" void* cGraphMenuItem_cGraphMenuItem(cGraphMenuItem* self, int numPoints, int xLockBits, int yLockBits, int slope, int points, t2Vector* minPoint, t2Vector* maxPoint)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->NumPoints = numPoints;
    self->Points = (float*)points;
    self->XLock = (unsigned int)xLockBits;
    self->YLock = (unsigned int)yLockBits;
    self->mVtbl = cGraphMenuItem_vtbl;
    self->Slope = slope;
    *(t2Vector*)&self->MinPoint[0] = *minPoint;
    *(t2Vector*)&self->MaxPoint[0] = *maxPoint;
    self->Caption = (const char*)D_00486738;
    self->OnPoint = 0;
    self->IsEditMode = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cGraphMenuItem_gainFocus);
#ifdef SKIP_ASM
void cMenuItem_gainFocus(void* self);

/**
 * Gives a graph item the focus: runs the base cMenuItem_gainFocus and leaves edit mode (+0x40 = 0).
 */
extern "C" void cGraphMenuItem_gainFocus(cGraphMenuItem* self)
{
    cMenuItem_gainFocus(self);
    self->IsEditMode = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cGraphMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_input(cMenuItem* self);
extern "C" int cMenuItem_isLocked(cMenuItem* self);
extern "C" int cMenuItem_getKey(void** self);
float cMenuItem_getLeft_f(void* self) __asm__("cMenuItem_getLeft__FPv");
float cMenuItem_getRight_f(void* self) __asm__("cMenuItem_getRight__FPv");
// PORT: the unit declares cMenuItem_getUp/cMenuItem_getDown as void*; these callers use the float result.
float cMenuItem_getUp_f(void* self) __asm__("cMenuItem_getUp__FPv");
float cMenuItem_getDown_f(void* self) __asm__("cMenuItem_getDown__FPv");


struct sVE_D0908 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


/**
 * Handles input for a graph item: select toggles edit mode; in edit mode keys pick a point and the stick moves it
 * within the range, the locks and the slope rule.
 *
 * Does nothing when locked. Key 6 flips the edit flag (+0x40). Outside edit mode it returns the base input
 * (cMenuItem_input). In edit mode keys 8 and 7 leave it; keys 4/5 select the previous/next point (+0x24, kept within the
 * point count +0x18). Up minus down (cMenuItem_getUp - cMenuItem_getDown) moves the point's y by that much times the y range
 * / 120 unless its bit in the y-lock mask (+0x20) is set, clamped to [+0x30, +0x38] and, with a slope rule (+0x3C) > 0,
 * kept between its neighbours' y (< 0: the reverse). Right minus left moves its x the same way unless x-locked (+0x1C),
 * clamped to [+0x2C, +0x34] and kept at least (x range) / max(2 * count, 50) from its neighbours. Any change calls the
 * menu's vtable slot 4 (onChange).
 *
 * @return the base input's result outside edit mode, else 0
 */
extern "C" int cGraphMenuItem_input(cGraphMenuItem* self)
{
    int result = 0;
    if (cMenuItem_isLocked((cMenuItem*)self) == 0) {
    int changed = 0;
    if (cMenuItem_getKey((void**)self) == 6) {
        self->IsEditMode ^= 1;
    }
    if (self->IsEditMode != 0) {
        if (cMenuItem_getKey((void**)self) == 8) {
            self->IsEditMode = 0;
        } else if (cMenuItem_getKey((void**)self) == 7) {
            self->IsEditMode = 0;
        } else if (self->IsEditMode != 0) {
            if (cMenuItem_getKey((void**)self) == 4) {
                if (self->OnPoint > 0) {
                    self->OnPoint--;
                    changed = 1;
                }
            } else if (cMenuItem_getKey((void**)self) == 5) {
                if (self->OnPoint < self->NumPoints - 1) {
                    self->OnPoint++;
                    changed = 1;
                }
            }
            float step = (self->MaxPoint[0] - self->MinPoint[0]) / (float)(self->NumPoints * 2 > 49 ? self->NumPoints * 2 : 50);
            float dy = cMenuItem_getUp_f(self) - cMenuItem_getDown_f(self);
            if (dy != 0.0f && (self->YLock & (1 << self->OnPoint)) == 0) {
                ((t2Vector*)self->Points)[self->OnPoint].y += dy * (self->MaxPoint[1] - self->MinPoint[1]) * 0.008333333767950535f;
                changed = 1;
                if (self->MaxPoint[1] < ((t2Vector*)self->Points)[self->OnPoint].y) {
                    ((t2Vector*)self->Points)[self->OnPoint].y = self->MaxPoint[1];
                }
                if (((t2Vector*)self->Points)[self->OnPoint].y < self->MinPoint[1]) {
                    ((t2Vector*)self->Points)[self->OnPoint].y = self->MinPoint[1];
                }
                if (self->OnPoint < self->NumPoints - 1) {
                    if (self->Slope > 0 && ((t2Vector*)self->Points)[self->OnPoint + 1].y < ((t2Vector*)self->Points)[self->OnPoint].y) {
                        ((t2Vector*)self->Points)[self->OnPoint].y = ((t2Vector*)self->Points)[self->OnPoint + 1].y;
                    } else if (self->Slope < 0 && ((t2Vector*)self->Points)[self->OnPoint].y < ((t2Vector*)self->Points)[self->OnPoint + 1].y) {
                        ((t2Vector*)self->Points)[self->OnPoint].y = ((t2Vector*)self->Points)[self->OnPoint + 1].y;
                    }
                }
                if (self->OnPoint > 0) {
                    if (self->Slope > 0 && ((t2Vector*)self->Points)[self->OnPoint].y < ((t2Vector*)self->Points)[self->OnPoint - 1].y) {
                        ((t2Vector*)self->Points)[self->OnPoint].y = ((t2Vector*)self->Points)[self->OnPoint - 1].y;
                    } else if (self->Slope < 0 && ((t2Vector*)self->Points)[self->OnPoint - 1].y < ((t2Vector*)self->Points)[self->OnPoint].y) {
                        ((t2Vector*)self->Points)[self->OnPoint].y = ((t2Vector*)self->Points)[self->OnPoint - 1].y;
                    }
                }
            }
            float dx = cMenuItem_getRight_f(self) - cMenuItem_getLeft_f(self);
            if (dx != 0.0f && (self->XLock & (1 << self->OnPoint)) == 0) {
                ((t2Vector*)self->Points)[self->OnPoint].x += dx * (self->MaxPoint[0] - self->MinPoint[0]) * 0.008333333767950535f;
                changed = 1;
                if (self->MaxPoint[0] < ((t2Vector*)self->Points)[self->OnPoint].x) {
                    ((t2Vector*)self->Points)[self->OnPoint].x = self->MaxPoint[0];
                }
                if (((t2Vector*)self->Points)[self->OnPoint].x < self->MinPoint[0]) {
                    ((t2Vector*)self->Points)[self->OnPoint].x = self->MinPoint[0];
                }
                if (self->OnPoint < self->NumPoints - 1) {
                    if (((t2Vector*)self->Points)[self->OnPoint + 1].x - step < ((t2Vector*)self->Points)[self->OnPoint].x) {
                        ((t2Vector*)self->Points)[self->OnPoint].x = ((t2Vector*)self->Points)[self->OnPoint + 1].x - step;
                    }
                }
                if (self->OnPoint > 0) {
                    if (((t2Vector*)self->Points)[self->OnPoint].x < ((t2Vector*)self->Points)[self->OnPoint - 1].x + step) {
                        ((t2Vector*)self->Points)[self->OnPoint].x = ((t2Vector*)self->Points)[self->OnPoint - 1].x + step;
                    }
                }
            }
        } else {
            result = cMenuItem_input((cMenuItem*)self);
        }
    } else {
        result = cMenuItem_input((cMenuItem*)self);
    }
    if (changed) {
        sVE_D0908* vt = (sVE_D0908*)self->Menu->mVtbl;
        vt[4].fn(vthis(self->Menu, vt[4]), self);
    }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("util/menu", cGraphMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern char str_9_999_9_999[];


static inline char* font_2D0D48(char* item)
{
    return *(char**)(*(char**)(*(char**)item + 0x124) + 0x58);
}

/**
 * Measures a graph item: one text line high (+12) normally, or a taller box while it is being edited with the focus.
 *
 * The width is 100 + the wider of the caption (+0x14) and "9.999,9.999" + 64. When the item has the focus and is in
 * edit mode (+0x40) the height is max(2 * line height + 6, 80) + 24 (the minimum is 20 otherwise, unused); else the
 * font's MaxCharHeight * BaseScale.y + 12. Starts at (0, 0).
 *
 * @param ret  where the rect is returned
 * @param item  the graph item
 * @return @p ret
 */
extern "C" tRect* cGraphMenuItem_preRender(tRect* ret, cGraphMenuItem* item)
{
    int margin = 100;
    int focused = 0;
    if (((char*)item) == cMenu_getFocusItem((cMenu*)(*(void**)item))) {
        focused = item->IsEditMode != 0;
    }
    int minHeight = focused ? 80 : 20;
    cFont* font = (cFont*)font_2D0D48(((char*)item));
    float lh = (float)font->MaxCharHeight * font->BaseScale[1];
    float w = cFont_textExtent(font, (char*)item->Caption, 0, 0, font->Scale[0], font->Scale[1]);
    char* font2 = font_2D0D48(((char*)item));
    float w2 = cFont_textExtent(font2, str_9_999_9_999, 0, 0, *(float*)(font2 + 0x38), *(float*)(font2 + 0x3C));
    if (w < w2) w = w2;
    tRect rect;
    float zero = 0.0f;
    rect.Y = zero;
    rect.X = zero;
    if (focused) {
        float height = lh + lh + 6.0f;
        float minHeightF = (float)minHeight;
        if (height < minHeightF) {
            height = minHeightF + 24.0f;
        } else {
            height = height + 24.0f;
        }
        rect.H = height;
    } else {
        char* font3 = font_2D0D48(((char*)item));
        rect.H = (float)*(int*)(font3 + 0x14) * *(float*)(font3 + 0x34) + 12.0f;
    }
    rect.W = (float)margin + w + 64.0f;
    *ret = rect;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cGraphMenuItem_render);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// Menu rectangles (item extents, text boxes): position and size.
struct sRect {
    float x, y, w, h;
};

// Menu colours, alpha first (cARGBMenuItem); the font colour at font + 0x40.
struct sColor {
    float a, r, g, b;
};

// One entry of the render-state stack (0x14 bytes); f8 holds 5-bit fields that are read through masks, f10 an id
// (-1 = none).
struct sRenderState {
    int f0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    short f10;
};

// The render context (D_004A289C_0EF8): render-state stack top at 0xE84, vtable pointer at 0x10D8.
class cRenderCtx {
public:
    char pad0[0xE84];
    sRenderState* top;          // 0xE84
    char pad1[0x10D8 - 0xE88];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71(int n, void* verts, int flags);
    virtual void v72();
    virtual void RenderLineStrip(int n, void* verts, int flags);
};


// 16-byte vector for the menu's vertices (built with the constructor or with Set).
struct sVec4 {
    float x, y, z, w;
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    void Set(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// The same vertex with the position as a plain aligned float[4] (filled through casts).
struct sVtxF {
    float u, v, q;
    int pad;
    int r, g, b, a;
    float pos[4] __attribute__((aligned(16)));
    sVtxF() {}
};

extern "C" void cFont_text(void* self, float x, float y, const char* str);

extern cMenuItem_textRender1_sVec3 kTextSelectedColor;

extern cMenuItem_textRender1_sVec3 kTextUnselectedColor;

// (prototype from their definition at line 858)
extern "C" void func_002CB350_0EF8(func_002CB350_sVtx* v, float* r, float* t) __asm__("func_002CB350");

// A point of the graph (cGraphMenuItem's point list).
struct sGraphPt_D0EF8 {
    float x, y;
};


// cBXString as a value class (one pointer); the operators are the game's out-of-line functions.
// PORT: asm labels are not accepted on destructors, so the dtor forwards to the out-of-line one (in-charge flag 2).
struct cBXStr_D0EF8;
extern "C" void cBXString_dtor_D0EF8(cBXStr_D0EF8* self, int flags) __asm__("cBXString__cBXString");
struct cBXStr_D0EF8 {
    char* str;
    cBXStr_D0EF8(const cBXStr_D0EF8& o) : str(o.str) {}
    ~cBXStr_D0EF8() { cBXString_dtor_D0EF8(this, 2); }
};
// Formats a value for display (prettyFloat returns a cBXString by value).
cBXStr_D0EF8 FormatValue_D0EF8(float v) __asm__("prettyFloat");
cBXStr_D0EF8 operator+(const cBXStr_D0EF8& a, const char* b) __asm__("func_00318630");
cBXStr_D0EF8 operator+(const cBXStr_D0EF8& a, const cBXStr_D0EF8& b) __asm__("func_003185C8");
extern char str_comma[];
extern sColor D_004D53C0;
extern sColor D_004D53D0;
extern sColor D_004D53E0;
// The focused-row style passed to the menu's frame draw is the line colour of the graph's axis.
extern sColor kBoxSelectedColor_c __asm__("kBoxSelectedColor");

// The font scale (font + 0x28), built from references like sVec4_2D0008: with the unit's by-value sVec2 the
// second store keeps the temporary's address in a register (the target stores both straight to the frame).
struct sVec2r_D0EF8 {
    float x, y;
    sVec2r_D0EF8(const float& ax, const float& ay) : x(ax), y(ay) {}
};

// Screen position of a graph point (scale and offset map the value range onto the graph box).
static inline float GraphX_D0EF8(const sGraphPt_D0EF8& pt, float scale, float offset)
{
    return pt.x * scale + offset;
}

static inline float GraphY_D0EF8(const sGraphPt_D0EF8& pt, float scale, float offset)
{
    return pt.y * scale + offset;
}

static inline int GetDepth_D0EF8(sRenderState* rs)
{
    return (unsigned int)(rs->f8 & 0x3E0) >> 5;
}

// Puts the render state's 5-bit depth field `layer` steps above `base` (at most 31).
static inline void SetDepth_D0EF8(sRenderState* rs, int base, int layer)
{
    int d = base + layer;
    if (d > 31) d = 31;
    rs->f8 = (rs->f8 & ~0x3E0) | ((d << 5) & 0x3E0);
}

/**
 * Draws a graph item: its box and caption, the selected point's "x,y" readout while edited, and a 100-wide plot of the
 * points at the right of the row.
 *
 * Draws the box through the menu's vtable slot 9 (selected when focused), sets the font colour (kTextSelectedColor or
 * kTextUnselectedColor, alpha 1) and ShadowOffset (2, 2), and draws the caption at rect.x + 16. With edit mode (+0x40) and the
 * focus it prints prettyFloat(x) + "," + prettyFloat(y) of the current point (+0x24) one line lower at rect.x + 32. The
 * plot box (100 wide, rect.h - 12 high, or - 24 when edited) is centred vertically, 16 from the right edge, and filled
 * in D_004D53C0 with texture -1 (func_002CB350); the y axis is flipped (MaxPoint.y +0x38 at the top). Each further
 * layer is drawn one step higher in the draw state's 5-bit field at word +0x8 bits 5-9: the y = 0 line in kBoxSelectedColor
 * when the y range spans 0, the polyline through the points in D_004D53D0 (GrMan's vtable slot 73, 2-vertex lines), and
 * in edit mode a 5x5 marker on the current point in D_004D53E0. Restores the field at the end.
 *
 * @param rect  the row's rect (x, y, w, h)
 */
extern "C" void cGraphMenuItem_render(cGraphMenuItem* self, float* rect)
{
    int focused = self == cMenu_getFocusItem((cMenu*)((void*)self->Menu));
    {
        cMenu* menu = self->Menu;
        sVEntry<void(void*, float*, int)>* vt = (sVEntry<void (void*, float*, int)>*)menu->mVtbl;
        vt[9].fn(vthis(menu, vt[9]), rect, focused);
    }
    char* font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
    float lineH = (float)*(int*)(font + 0x14) * *(float*)(font + 0x34);
    cMenuItem_textRender1_sVec3* col = self == cMenu_getFocusItem((cMenu*)((void*)self->Menu)) ? &kTextSelectedColor : &kTextUnselectedColor;
    sColor textColor;
    textColor.a = 1.0f;
    textColor.r = col->x;
    textColor.g = col->y;
    textColor.b = col->z;
    *(sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = textColor;
    *(sVec2r_D0EF8*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2r_D0EF8(2.0f, 2.0f);
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f, ((char*)self->Caption));
    if (self->IsEditMode && focused) {
        cBXStr_D0EF8 str = FormatValue_D0EF8(((sGraphPt_D0EF8*)self->Points)[self->OnPoint].x) + str_comma + FormatValue_D0EF8(((sGraphPt_D0EF8*)self->Points)[self->OnPoint].y);
        cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 32.0f, rect[1] + 12.0f - 1.0f + lineH, str.str);
    }
    float graphHeight;
    if (focused && self->IsEditMode) {
        graphHeight = rect[3] - 24.0f;
    } else {
        graphHeight = rect[3] - 12.0f;
    }
    sRect gr;
    gr.w = 100.0f;
    gr.h = graphHeight;
    gr.y = rect[1] + (rect[3] - graphHeight) * 0.5f;
    gr.x = rect[0] + rect[2] - 16.0f - gr.w;
    int old = GetDepth_D0EF8(((cRenderCtx*)GrMan)->top);
    sVtxF quadVerts[4];
    int i;
    for (i = 0; i < 4; i++) {
        quadVerts[i].a = (int)(D_004D53C0.a * 128.0f);
        quadVerts[i].r = (int)(D_004D53C0.r * 128.0f);
        quadVerts[i].g = (int)(D_004D53C0.g * 128.0f);
        quadVerts[i].b = (int)(D_004D53C0.b * 128.0f);
        quadVerts[i].q = 1.0f;
    }
    ((cRenderCtx*)GrMan)->top->f10 = -1;
    {
        float fullUv[4];
        fullUv[0] = 0.0f;
        fullUv[1] = 0.0f;
        fullUv[2] = 1.0f;
        fullUv[3] = 1.0f;
        func_002CB350_0EF8((func_002CB350_sVtx*)quadVerts, &gr.x, fullUv);
    }
    float sx = gr.w / (self->MaxPoint[0] - self->MinPoint[0]);
    float sy = gr.h / (self->MinPoint[1] - self->MaxPoint[1]);
    float ox = gr.x - self->MinPoint[0] * sx;
    float oy = gr.y - self->MaxPoint[1] * sy;
    if ((self->MinPoint[1] >= 0.0f) != (self->MaxPoint[1] > 0.0f)) {
        SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 1);
        for (i = 0; i < 2; i++) {
            quadVerts[i].a = (int)(kBoxSelectedColor_c.a * 128.0f);
            quadVerts[i].r = (int)(kBoxSelectedColor_c.r * 128.0f);
            quadVerts[i].g = (int)(kBoxSelectedColor_c.g * 128.0f);
            quadVerts[i].b = (int)(kBoxSelectedColor_c.b * 128.0f);
        }
        sVec4 vertexPos;
        vertexPos.x = gr.x;
        vertexPos.y = oy;
        vertexPos.z = 0.0f;
        vertexPos.w = 1.0f;
        *(sVec4*)quadVerts[0].pos = vertexPos;
        vertexPos.x = gr.x + gr.w;
        vertexPos.y = oy;
        vertexPos.z = 0.0f;
        vertexPos.w = 1.0f;
        *(sVec4*)quadVerts[1].pos = vertexPos;
        cGraphicsMan_TheGraphicsMan->RenderLineStrip(2, (const tBxVertex*)quadVerts, 0);
    }
    SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 2);
    for (i = 0; i < 2; i++) {
        quadVerts[i].a = (int)(D_004D53D0.a * 128.0f);
        quadVerts[i].r = (int)(D_004D53D0.r * 128.0f);
        quadVerts[i].g = (int)(D_004D53D0.g * 128.0f);
        quadVerts[i].b = (int)(D_004D53D0.b * 128.0f);
    }
    for (i = 1; i < self->NumPoints; i++) {
        float x0 = GraphX_D0EF8(((sGraphPt_D0EF8*)self->Points)[i - 1], sx, ox);
        float y0 = GraphY_D0EF8(((sGraphPt_D0EF8*)self->Points)[i - 1], sy, oy);
        float x1 = GraphX_D0EF8(((sGraphPt_D0EF8*)self->Points)[i], sx, ox);
        float y1 = GraphY_D0EF8(((sGraphPt_D0EF8*)self->Points)[i], sy, oy);
        sVec4 vertexPos;
        vertexPos.x = x0;
        vertexPos.y = y0;
        vertexPos.z = 0.0f;
        vertexPos.w = 1.0f;
        *(sVec4*)quadVerts[0].pos = vertexPos;
        vertexPos.x = x1;
        vertexPos.y = y1;
        vertexPos.z = 0.0f;
        vertexPos.w = 1.0f;
        *(sVec4*)quadVerts[1].pos = vertexPos;
        cGraphicsMan_TheGraphicsMan->RenderLineStrip(2, (const tBxVertex*)quadVerts, 0);
    }
    if (self->IsEditMode) {
        SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 3);
        for (i = 0; i < 4; i++) {
            quadVerts[i].a = (int)(D_004D53E0.a * 128.0f);
            quadVerts[i].r = (int)(D_004D53E0.r * 128.0f);
            quadVerts[i].g = (int)(D_004D53E0.g * 128.0f);
            quadVerts[i].b = (int)(D_004D53E0.b * 128.0f);
        }
        float markerRect[4];
        float tt[4];
        float px = GraphX_D0EF8(((sGraphPt_D0EF8*)self->Points)[self->OnPoint], sx, ox);
        float py = GraphY_D0EF8(((sGraphPt_D0EF8*)self->Points)[self->OnPoint], sy, oy);
        markerRect[0] = px - 2.0f;
        markerRect[1] = py - 2.0f;
        markerRect[2] = 5.0f;
        markerRect[3] = 5.0f;
        tt[0] = 0.0f;
        tt[1] = 0.0f;
        tt[2] = 1.0f;
        tt[3] = 1.0f;
        func_002CB350_0EF8((func_002CB350_sVtx*)quadVerts, markerRect, tt);
    }
    // Restore the depth the item started with.
    ((cRenderCtx*)GrMan)->top->f8 = (((cRenderCtx*)GrMan)->top->f8 & ~0x3E0) | ((old << 5) & 0x3E0);
}
#endif

//100%
INCLUDE_ASM("util/menu", cErrorMenu_cErrorMenu);
#ifdef SKIP_ASM
void* cMenu_cMenu(void* self);
extern void* cErrorMenu_vtbl[];

/**
 * Constructs an empty error menu: a cMenu (cMenu_cMenu) with vtable cErrorMenu_vtbl and three items, added in order.
 *
 * The items are a cNullMenuItem with no caption (+0x130), a cSpaceMenuItem of 10 (+0x14C) and a "back" item
 * (cPopMenuItem_cPopMenuItem) with no caption (+0x164).
 *
 * @return @p self
 */
extern "C" void* cErrorMenu_cErrorMenu(cErrorMenu* self)
{
    char* nullItem = (char*)&self->Error;
    char* spaceItem = (char*)&self->Space;
    char* item3 = (char*)&self->Ok;
    cMenu_cMenu(self);
    self->mVtbl = cErrorMenu_vtbl;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)nullItem, 0);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)spaceItem, (void*)10);
    cPopMenuItem_cPopMenuItem((cPopMenuItem*)((cPopMenuItem*)item3), 0);
    cMenu_addItem((cMenuList*)self, (void**)nullItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)spaceItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)item3, -1);
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("util/menu", cErrorMenu_cErrorMenu1);
#ifdef SKIP_ASM
void* cMenu_cMenu(void* self);
extern void* cErrorMenu_vtbl[];

/**
 * Constructs an error menu with an error text and a button caption: like cErrorMenu_cErrorMenu, but the cNullMenuItem (+0x130)
 * shows @p nullText, the space (+0x14C) is 30 and the "back" item (+0x164) is labelled @p text3.
 *
 * @param nullText  the error message
 * @param text3  caption of the item that closes the menu
 * @return @p self
 */
extern "C" void* cErrorMenu_cErrorMenu1(cErrorMenu* self, void* nullText, void* text3)
{
    char* nullItem = (char*)&self->Error;
    cMenu_cMenu(self);
    char* item3 = (char*)&self->Ok;
    self->mVtbl = cErrorMenu_vtbl;
    char* spaceItem = (char*)&self->Space;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)nullItem, nullText);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)spaceItem, (void*)0x1E);
    cPopMenuItem_cPopMenuItem((cPopMenuItem*)((cPopMenuItem*)item3), text3);
    cMenu_addItem((cMenuList*)self, (void**)nullItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)spaceItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)item3, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("util/menu", noiseValue);
#ifdef SKIP_ASM
extern float kClintsNoiseFunction[];

/**
 * Returns a smooth 1D noise value: @p phase * 160 split into an index and a fraction, interpolated linearly between entries
 * index % 160 and index % 160 + 1 of the float table kClintsNoiseFunction.
 *
 * Negative inputs give a negative index (truncation and % keep the sign), so they read before the table.
 *
 * @param phase  noise coordinate (one period is 1.0)
 * @return the interpolated table value
 */
extern "C" float noiseValue(float phase)
{
    phase = phase * 160.0f;
    int i = (int)phase;
    phase = phase - (float)i;
    i = i % 160;
    return kClintsNoiseFunction[i] * (1.0f - phase) + kClintsNoiseFunction[i + 1] * phase;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", perlinNoise);
#ifdef SKIP_ASM
extern "C" float noiseValue(float t);

/**
 * Returns fractal noise: the sum of @p n octaves of noiseValue, octave k sampled at @p x * 2^k with weight 0.5^k.
 *
 * Likely bug: the last octave's value is added once more after the loop, so the result is sum + last octave (and 0 for
 * @p n = 0). The board wake and seg_1F1548's pulse use 3 octaves.
 *
 * @param n  number of octaves
 * @param x  noise coordinate
 * @return the summed noise
 */
extern "C" float perlinNoise(int n, float x)
{
    float sum = 0.0f;
    float amp = 1.0f;
    float octave = 0.0f;
    float freq = 1.0f;
    int i;
    for (i = 0; i < n; i++) {
        float scaledX = x * freq;
        freq = freq * 2.0f;
        octave = noiseValue(scaledX) * amp;
        amp = amp * 0.5f;
        sum = sum + octave;
    }
    return sum + octave;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runCollideScript);
#ifdef SKIP_ASM
extern "C" int cWScriptMan_doOnCollideScript(void* self, int a, int b, int c);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs a world object's collision script through the world script manager.
 *
 * Calls cWScriptMan_doOnCollideScript on cWScriptMan_TheWScriptMan with the three arguments and returns its result.
 *
 * @param collideInfo  collision info (On Tour tc)
 * @param intersectInfo  intersection info (On Tour ti)
 * @param pBeing  the being that collided (On Tour pBeing)
 * @return cWScriptMan_doOnCollideScript's result
 */
extern "C" int cObjectInterface_runCollideScript(int collideInfo, int intersectInfo, int pBeing)
{
    return cWScriptMan_doOnCollideScript(cWScriptMan_TheWScriptMan, collideInfo, intersectInfo, pBeing);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runExpireScript);
#ifdef SKIP_ASM
extern "C" int cWScriptMan_hasOnExpireScript(void* self, int id);
extern "C" void cWScriptMan_doOnExpireScript(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs the expire script of world instance @p id, if it has one, through the world script manager.
 *
 * Asks cWScriptMan_hasOnExpireScript on cWScriptMan_TheWScriptMan and, when it says yes, runs
 * cWScriptMan_doOnExpireScript. cInstanceNode's expire handler calls it with its instance.
 *
 * @param id  the instance (record)
 * @return 1 when a script ran, else 0
 */
extern "C" int cObjectInterface_runExpireScript(int id)
{
    if (cWScriptMan_hasOnExpireScript(cWScriptMan_TheWScriptMan, id) != 0) {
        cWScriptMan_doOnExpireScript(cWScriptMan_TheWScriptMan, id);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runOnLoseFocusScript);
#ifdef SKIP_ASM
extern "C" int cWScriptMan_hasOnLoseFocusScript(void* self, int id);
extern "C" void cWScriptMan_doOnLoseFocusScript(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs a world instance's on-lose-focus script, when it has one.
 *
 * Asks the world script manager (TheWScriptMan) whether the instance has an on-lose-focus script
 * (cWScriptMan_hasOnLoseFocusScript) and runs it (cWScriptMan_doOnLoseFocusScript).
 *
 * @param id  the instance record (On Tour tInstanceRecord*)
 * @return 1 when a script was run, otherwise 0
 */
extern "C" int cObjectInterface_runOnLoseFocusScript(int id)
{
    if (cWScriptMan_hasOnLoseFocusScript(cWScriptMan_TheWScriptMan, id) != 0) {
        cWScriptMan_doOnLoseFocusScript(cWScriptMan_TheWScriptMan, id);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runInitScript);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_doOnInitScript(int, int);

/**
 * Runs the init script of world instance @p inst: cWScriptMan_doOnInitScript on cWScriptMan_TheWScriptMan.
 *
 * cInstanceNode's init handler calls it with its Instance.
 *
 * @param inst  the instance record
 */
extern "C" void cObjectInterface_runInitScript(int inst) {
    cWScriptMan_doOnInitScript(*(int*)&cWScriptMan_TheWScriptMan, inst);
}
#endif
//100%
INCLUDE_ASM("util/menu", cObjectInterface_runScript);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScript1(int, int, int);

/**
 * Runs world script @p scriptId for instance @p inst: cWScriptMan_runScript1 on cWScriptMan_TheWScriptMan.
 *
 * @param scriptId  the script to run
 * @param inst  the instance record it runs for
 */
extern "C" void cObjectInterface_runScript(int scriptId, int inst) {
    cWScriptMan_runScript1(*(int*)&cWScriptMan_TheWScriptMan, scriptId, inst);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runFrameUpdateScript);
#ifdef SKIP_ASM
extern "C" int cWScriptMan_doOnFrameUpdateScript(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs the frame-update script of world instance @p id: cWScriptMan_doOnFrameUpdateScript on cWScriptMan_TheWScriptMan.
 *
 * cAnimNode's update passes its Instance.
 *
 * @param id  the instance record
 * @return the script manager's result
 */
extern "C" int cObjectInterface_runFrameUpdateScript(int id)
{
    return cWScriptMan_doOnFrameUpdateScript(cWScriptMan_TheWScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getNumBeings);
#ifdef SKIP_ASM

/**
 * Returns how many riders the AI has allocated: cAI::NumberOfRidersAllocated (+0x78) of SSXApp->Game->AI.
 *
 * The world-sphere tree loops over the riders with it. (The neighbouring cObjectInterface_getBeing returns a
 * rider's being instead.)
 *
 * @return the number of riders
 */
extern "C" void* cObjectInterface_getNumBeings()
{
    return (void*)SSXApp->Game->AI->NumberOfRidersAllocated;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getBeing);
#ifdef SKIP_ASM

/**
 * Returns rider @p i's being interface: Riders[@p i] + 0x6C0, or 0.
 *
 * Reads cAI::Riders[i] of SSXApp->Game->AI; +0x6C0 is a base-class subobject of cRider whose virtuals the
 * boost nodes call. The count of beings is cObjectInterface_getNumBeings (cAI::NumberOfRidersAllocated), just before it, as
 * On Tour's getNumBeings comes just before its getBeing(int).
 *
 * @param i  rider index, 0 to 5
 * @return that rider's +0x6C0 subobject, or 0 when the slot is empty
 */
extern "C" cBeing* cObjectInterface_getBeing(int i)
{
    return SSXApp->Game->AI->Riders[i];
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getAIIndex);
#ifdef SKIP_ASM
class cObjectInterface_getAIIndex_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};

/**
 * Asks being @p obj for an int through its vtable slot 7 (the first virtual of the interface it is called on).
 *
 * cMoveNode_onCollide passes the colliding being's result to its root transform modifier's slot-9 test. Declared void
 * here; the caller uses the value.
 *
 * @param obj  the being interface (a rider's +0x6C0 subobject, as cObjectInterface_getBeing returns)
 */
extern "C" void cObjectInterface_getAIIndex(cObjectInterface_getAIIndex_cObj* obj)
{
    obj->getAIIndex();
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getPosition);
#ifdef SKIP_ASM
class cObjectInterface_getPosition_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int getPosition();
};


/**
 * Returns rider @p i's position: calls slot 5 of the being interface at Riders[@p i] + 0x6C0 (cAI::Riders, +0x28 of
 * SSXApp->Game->AI).
 *
 * Declared void here; cMoveNode reads the result as a pointer to a 4-float position. Unlike
 * cObjectInterface_getBeing it does not check for an empty rider slot.
 *
 * @param i  rider index
 */
extern "C" void cObjectInterface_getPosition(int i)
{
    char* base = (char*)SSXApp->Game->AI;
    char* rider = *(char**)(base + (i << 2) + 0x28);
    ((cObjectInterface_getPosition_cObj*)(rider + 0x6C0))->getPosition();
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getPeakNumber);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);

/**
 * Returns the current peak's number: cBENewRaceInterface_getLocationPeak1 of the new-race interface (cBE_getInterface
 * kind 0 of cBE_getBE()) plus 1.
 *
 * cFlagSet uses it to pick its wind range (0.15, 0.3 or 0.45).
 *
 * @return the peak number (1-based)
 */
extern "C" int cObjectInterface_getPeakNumber()
{
    return cBENewRaceInterface_getLocationPeak1(cBE_getInterface_Fv(cBE_getBE(), 0)) + 1;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getWorld);
#ifdef SKIP_ASM

/**
 * Returns the current world, for world objects that only see the object interface.
 *
 * @return cWorld_m_pTheWorld
 */
extern "C" void* cObjectInterface_getWorld()
{
    return cWorld_m_pTheWorld;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getNash);
#ifdef SKIP_ASM

/**
 * Returns the game's BXNash object (SSXApp->Game->BXNash).
 *
 * @return the BXNash pointer
 */
extern "C" void* cObjectInterface_getNash()
{
    return SSXApp->Game->BXNash;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_addToFocusBox);
#ifdef SKIP_ASM
extern "C" int cFocusMan_addfocusinstance(void* self, int a);

/**
 * Hands instance record @p instanceRecord to the AI's focus manager (cFocusMan_addfocusinstance on SSXApp->Game->AI->FocusMan), which marks it
 * (record flags bit 0x100) and adds it to the focus boxes.
 *
 * cInstanceNode's constructor calls it for records without that bit, except types 6, 0x10 and 22.
 *
 * @param instanceRecord  the instance record
 * @return cFocusMan_addfocusinstance's result
 */
extern "C" int cObjectInterface_addToFocusBox(int instanceRecord)
{
    return cFocusMan_addfocusinstance(SSXApp->Game->AI->FocusMan, instanceRecord);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getCameraPosition);
#ifdef SKIP_ASM

/**
 * Returns the position of game camera @p i: the camera pointer at SSXApp->Game->ViewMan + 4 + 4 * @p i, plus 0x20.
 *
 * cModifierBlock's render takes the current camera's (cObjectInterface_getCurrentCameraNum) position with it.
 *
 * @param i  camera (view) index
 * @return pointer to the camera's position vector
 */
extern "C" void* cObjectInterface_getCameraPosition(int i)
{
    char* riders = (char*)SSXApp->Game->ViewMan;
    return *(char**)(riders + (i << 2) + 4) + 0x20;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getNumCameras);
#ifdef SKIP_ASM
/**
 * Returns the game view manager's view count, the int at SSXApp->Game->ViewMan + 0x10 (cGame's render tests it for
 * split screen).
 *
 * @return the number of views (cameras)
 */
extern "C" int cObjectInterface_getNumCameras(void) {
    return SSXApp->Game->ViewMan->m_uiNumView;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getCurrentCameraNum);
#ifdef SKIP_ASM

/**
 * Returns the game view manager's current view number, the word at SSXApp->Game->ViewMan + 0x14.
 *
 * Declared returning a pointer; cModifierBlock uses it as the camera index for cObjectInterface_getCameraPosition.
 *
 * @return the current camera number
 */
extern "C" void* cObjectInterface_getCurrentCameraNum()
{
    return (void*)SSXApp->Game->ViewMan->m_uiCurrentView;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getUpdateTime);
#ifdef SKIP_ASM

/**
 * Returns the length of the current update step, for world objects.
 *
 * @return the float at SSXApp+0x14
 */
extern "C" float cObjectInterface_getUpdateTime()
{
    return SSXApp->TimerPeriod;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getUpdateHz);
#ifdef SKIP_ASM
/**
 * Returns the update rate as a float: the int at SSXApp + 0x10 (the frames per second that cMoveNode halves for a
 * half-second debounce).
 *
 * @return updates per second
 */
extern "C" float cObjectInterface_getUpdateHz(void) {
    return (float) (SSXApp->TimerHz);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getAITick);
#ifdef SKIP_ASM

/**
 * Returns the AI tick counter, cAI::AITick (+0x8) of SSXApp->Game->AI.
 *
 * Declared returning a pointer; cFlagSet takes it modulo 2 to update its grid on alternate ticks.
 *
 * @return the AI tick
 */
extern "C" void* cObjectInterface_getAITick()
{
    return (void*)SSXApp->Game->AI->AITick;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getFlagManager);
#ifdef SKIP_ASM

/**
 * Returns the game's flag manager, SSXApp->Game->FlagManager (+0x70).
 *
 * The flag-rail nodes add and remove flags through it and cFlagSet reads its wind factor (+0x10).
 *
 * @return the cFlagManager
 */
extern "C" void* cObjectInterface_getFlagManager()
{
    return SSXApp->Game->FlagManager;
}
#endif

extern "C" void* makeDead(void* self);

//100%
INCLUDE_ASM("util/menu", cObjectInterface_makeDead__FPv);
#ifdef SKIP_ASM
/**
 * Applies an instance event: forwards to makeDead with @p self and, in the second register, the event.
 *
 * cDeadFadeNode calls it with its instance record (+0x18) and stored event (+0x1C) once the fade ends or the node loses
 * focus; makeDead acts by the event's kind (e.g. 1 deletes the record's node). This definition declares only the
 * first argument.
 *
 * @return makeDead's result
 */
void* cObjectInterface_makeDead(void* self)
{
    return makeDead(self);
}
#endif

