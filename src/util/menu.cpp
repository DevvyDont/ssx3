#include "common.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "util/menu.h"
#include "main/game.h"
#include "main/ssxapp.h"

struct cMenuItem {
    char pad_0x00[0x4];
    int field_0x4;
    void* field_0x8;
    int field_0xC;
    void* vtable; // 0x10
};
extern void* cMenuItem_vtbl[16];

//100%
INCLUDE_ASM("util/menu", cMenuItem_cMenuItem__FP9cMenuItemPv);
#ifdef SKIP_ASM
/**
 * Constructs a base menu item: active, with the given ID and no argument.
 *
 * Stores cMenuItem's vtable at +0x10, Flags (+0x4) = 1 (active), ID (+0x8) = @p text and Arg (+0xC) = 0.
 * Menu (+0x0) is left for cMenu_addItem() to fill.
 *
 * @param text  the item's ID (On Tour cMenuItem::ID); derived items pass -1 for none
 * @return @p self
 */
cMenuItem* cMenuItem_cMenuItem(cMenuItem* self, void* text)
{
    self->vtable = cMenuItem_vtbl;
    self->field_0x4 = 1;
    self->field_0x8 = text;
    self->field_0xC = 0;
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
extern "C" void cMenuItem__cMenuItem(int* self, int flags)
{
    *(void***)((char*)self + 0x10) = cMenuItem_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA2B0);
#ifdef SKIP_ASM
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CA2B0(void* self)
{
    switch (func_002CA3E8((void**)self)) {
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
extern "C" int cMenuItem_isActive(void* self);
extern "C" int cMenuItem_isLocked(void* self);

extern "C" int cMenuItem_canHaveFocus(void* self)
{
    int r = 0;
    if (cMenuItem_isActive(self) != 0) {
        r = cMenuItem_isLocked(self) == 0;
    }
    return r;
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
INCLUDE_ASM("util/menu", func_002CA370__FPv);
#ifdef SKIP_ASM
void func_002CA370(void* self)
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
extern "C" int cMenuItem_isActive(void* self)
{
    return *(int*)((char*)self + 0x4) & 1;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA388);
#ifdef SKIP_ASM
extern "C" void func_002CA388(void* self, int enable)
{
    if (enable) {
        *(int*)((char*)self + 0x4) |= 1;
    } else {
        *(int*)((char*)self + 0x4) &= ~1;
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
extern "C" int cMenuItem_isLocked(void* self)
{
    return *(int*)((char*)self + 0x4) & 8;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA3C0);
#ifdef SKIP_ASM
extern "C" void func_002CA3C0(void *self, int enable) {
    int *flags = (int *)((char *)self + 4);

    if (enable != 0) {
        *flags |= 8;
    } else {
        *flags &= ~8;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA3E8);
#ifdef SKIP_ASM
int func_002CC248(void* self);

extern "C" int func_002CA3E8(void** self)
{
    return func_002CC248(*(void**)((char*)*self + 0x124));
}
#endif

// PORT: func_002CC260 is C++ (void*); this caller passes an int handle.
void* func_002CC260(int) __asm__("func_002CC260__FPv");

//100%
INCLUDE_ASM("util/menu", func_002CA408__FPv);
#ifdef SKIP_ASM
void* func_002CA408(void* self)
{
    return func_002CC260(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: func_002CC280 is C++ (void*); this caller passes an int handle.
void* func_002CC280(int) __asm__("func_002CC280__FPv");

//100%
INCLUDE_ASM("util/menu", func_002CA428__FPv);
#ifdef SKIP_ASM
void* func_002CA428(void* self)
{
    return func_002CC280(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: func_002CC2A0 is C++ (void*); this caller passes an int handle.
void* func_002CC2A0(int) __asm__("func_002CC2A0__FPv");

//100%
INCLUDE_ASM("util/menu", func_002CA448__FPv);
#ifdef SKIP_ASM
void* func_002CA448(void* self)
{
    return func_002CC2A0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

// PORT: func_002CC2C0 is C++ (void*); this caller passes an int handle.
void* func_002CC2C0(int) __asm__("func_002CC2C0__FPv");

//100%
INCLUDE_ASM("util/menu", func_002CA468__FPv);
#ifdef SKIP_ASM
void* func_002CA468(void* self)
{
    return func_002CC2C0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC2E0(int);

//100%
INCLUDE_ASM("util/menu", func_002CA488__FPv);
#ifdef SKIP_ASM
void* func_002CA488(void* self)
{
    return func_002CC2E0(*(int*)((char*)*(void**)self + 0x124));
}
#endif

extern "C" void* func_002CC318(int);

//100%
INCLUDE_ASM("util/menu", func_002CA4A8__FPv);
#ifdef SKIP_ASM
void* func_002CA4A8(void* self)
{
    return func_002CC318(*(int*)((char*)*(void**)self + 0x124));
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA4C8);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sRect_2CA4C8 { float x, y, w, h; };
struct sVec2_2CA4C8 {
    float x, y;
};

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

// PORT: the unit's callers declare func_002CA4C8 as (void*, void*, int, int, void*) returning void;
// the body returns an sRect by value (hidden return pointer) from (item, left, center, right). Bound by asm label.
extern "C" sRect_2CA4C8 func_002CA4C8_impl(char* item, const char* left, const char* center, const char* right) __asm__("func_002CA4C8");

extern "C" sRect_2CA4C8 func_002CA4C8_impl(char* item, const char* left, const char* center, const char* right)
{
    sRect_2CA4C8 r;
    sVec2_2CA4C8 sc;
    float zero = 0.0f;
    r.y = zero;
    r.x = zero;
    char* f = font_2CA4C8(item);
    float h = (float)*(int*)(f + 0x14) * *(float*)(f + 0x34);
    float sx = *(float*)(f + 0x38) / *(float*)(f + 0x30);
    float sy = *(float*)(f + 0x3C) / *(float*)(f + 0x34);
    sc.x = sx;
    sc.y = sy;
    r.h = h * sy;
    if (left && center && right) {
        r.w = width_2CA4C8(item, center) + MAXF_2CA4C8(width_2CA4C8(item, left), width_2CA4C8(item, right)) * 2.0f + 32.0f;
    } else {
        float w = widthOr0_2CA4C8(item, left);
        if (center) {
            w += width_2CA4C8(item, center);
        }
        if (right) {
            w += width_2CA4C8(item, right);
        }
        r.w = w;
        if ((left && (center || right)) || (center && right)) {
            r.w += 16.0f;
        }
    }
    r.h += 12.0f;
    r.w = (r.w >? 100.0f) + 32.0f;
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CA768);
#ifdef SKIP_ASM
extern "C" void cFont_text(void* self, float x, float y, const char* str);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sRect_2CA768 { float x, y, w, h; };
struct sColor_2CA768 { float r, g, b, a; };
struct sVec2_2CA768 {
    float x, y;
    sVec2_2CA768(float ax, float ay) : x(ax), y(ay) {}
};

struct sVEnt_2CA768 { short delta; short index; void (*fn)(void*, sRect_2CA768*, char*); };

static inline char* font_2CA768(char* item)
{
    return *(char**)(*(char**)(*(char**)item + 0x124) + 0x58);
}

// PORT: the unit's callers declare func_002CA768 as (void*, int, int, int, void*, char*, sColor*) returning void*;
// the body takes (item, rect, left, center, right, style, color) and returns nothing. Bound by asm label.
extern "C" void func_002CA768_impl(char* item, sRect_2CA768* rect, const char* left, const char* center, const char* right, char* style, sColor_2CA768* color) __asm__("func_002CA768");

extern "C" void func_002CA768_impl(char* item, sRect_2CA768* rect, const char* left, const char* center, const char* right, char* style, sColor_2CA768* color)
{
    char* menu = *(char**)item;
    sVEnt_2CA768* vt = *(sVEnt_2CA768**)(menu + 0x12C);
    vt[8].fn(menu + vt[8].delta, rect, style);
    *(sColor_2CA768*)(font_2CA768(item) + 0x40) = *color;
    char* f0 = font_2CA768(item);
    sVec2_2CA768 sc(2.0f, 2.0f);
    *(sVec2_2CA768*)(f0 + 0x28) = sc;
    if (left) {
        cFont_text(font_2CA768(item), rect->x + 16.0f, rect->y + 6.0f - 1.0f, left);
    }
    if (center) {
        char* f = font_2CA768(item);
        float w = cFont_textExtent(f, center, 0, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        cFont_text(font_2CA768(item), rect->x + (rect->w - w) * 0.5f, rect->y + 6.0f - 1.0f, center);
    }
    if (right) {
        char* f = font_2CA768(item);
        float w = cFont_textExtent(f, right, 0, 0, *(float*)(f + 0x38), *(float*)(f + 0x3C));
        cFont_text(font_2CA768(item), rect->x + (rect->w - w) - 16.0f, rect->y + 6.0f - 1.0f, right);
    }
    *(sVec2_2CA768*)(font_2CA768(item) + 0x28) = sVec2_2CA768(0.0f, 0.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CA988);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern char D_004D5380[];
extern char D_004D5390[];

struct func_002CA988_sVec3 {
    float x;
    float y;
    float z;
};
extern func_002CA988_sVec3 D_004D53A0;
extern func_002CA988_sVec3 D_004D53B0;

struct func_002CA988_sColor {
    float a;
    float r;
    float g;
    float b;
};

extern "C" void* func_002CA768(void* self, int a1, int a2, int a3, void* a4, char* style, func_002CA988_sColor* col);

// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the body reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CA988_5(void* self, int a1, int a2, int a3, void* a4)
{
    int focused = self == cMenu_getFocusItem((cMenu*)(*(void**)self));
    char* style = focused ? D_004D5380 : D_004D5390;
    func_002CA988_sVec3* v = focused ? &D_004D53A0 : &D_004D53B0;
    func_002CA988_sColor c;
    c.a = 1.0f;
    c.r = v->x;
    c.g = v->y;
    c.b = v->z;
    return func_002CA768(self, a1, a2, a3, a4, style, &c);
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
void* cMenu_cMenu(void* self)
{
    int zeroValue = 0;
    *(int*)self = zeroValue;
    *(int*)((char*)self + 0x12c) = (int)(void*)cMenu_vtbl;
    *(int*)((char*)self + 0x4) = zeroValue;
    *(int*)((char*)self + 0x8) = zeroValue;
    *(int*)((char*)self + 0x124) = zeroValue;
    return self;
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
extern "C" void cMenu__cMenu(int* self, int flags)
{
    *(void***)((char*)self + 0x12C) = cMenu_vtbl;
    if (flags & 1) {
        operator_delete(self);
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
    virtual int v03();
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
    if (item->v03() == 0) {
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
    virtual int v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
    virtual void v07();
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
    } while (!self->items[self->cur]->v03());
    self->items[start]->v07();
    self->items[self->cur]->v06(0);
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
    virtual int v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int);
    virtual void v07();
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
    } while (!self->items[self->cur]->v03());
    self->items[start]->v07();
    self->items[self->cur]->v06(1);
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
    virtual int v02();
};

extern "C" void cMenu_prevFocus(cMenu* self);

extern "C" int cMenu_input(void* self)
{
    cMenu_checkFocus((cMenu*)self);
    cMenu_input_cItem* item = *(cMenu_input_cItem**)((char*)self + (*(int*)((char*)self + 0x4) << 2) + 0xC);
    int r = item->v02();
    switch (r) {
    case 3:
        cMenu_nextFocus((cMenu*)self);
        return 0;
    case 4:
        cMenu_prevFocus((cMenu*)self);
        return 0;
    }
    return r;
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

// cMenuSys (On Tour): getDisplayHeight() is `screen height - mF64` there (272 on PSP, 480 here).
class cMenuSys_2CAD58 {
public:
    char pad[0x64];
    int f64;                        // 0x64
    int getDisplayWidth() { return 640; }
    int getDisplayHeight() { return 480 - f64; }
};

struct sMenu_2CAD58 {
    int NumItems;                   // 0x0
    int FocusItem;                  // 0x4
    int TopItem;                    // 0x8
    cItem_2CAD58* Items[70];        // 0xC
    cMenuSys_2CAD58* MenuSys;       // 0x124
};

static const float kScreenGapY = 53.0f;
static const float kArrowWidth = 22.0f;
static const float kArrowEdgeOffset = 32.0f;
static const float kArrowHeight = 15.0f;

// PORT: these are declared later in the unit without the self/rect parameters.
void func_002CB180_s(void* self) __asm__("func_002CB180");
void func_002CB2F8_s(void* self) __asm__("func_002CB2F8");
void func_002CB8C8_r(void* self, const sRect_2CAD58& r) __asm__("func_002CB8C8");
void func_002CBAC8_r(void* self, const sRect_2CAD58& r) __asm__("func_002CBAC8");

// cMenu::render(void) (On Tour name)
extern "C" void cMenu_render(sMenu_2CAD58* self)
{
    sRect_2CAD58 total;
    sRect_2CAD58 item[70];
    int i;
    total.x = total.y = total.w = total.h = 0.0f;
    for (i = 0; i < self->NumItems; i++) {
        if (cMenuItem_isActive(self->Items[i])) {
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
    if (total.h > self->MenuSys->f64 + self->MenuSys->getDisplayHeight() - kScreenGapY * 2) {
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
    func_002CB180_s(self);
    for (i = self->TopItem; i < self->NumItems; i++) {
        if (cMenuItem_isActive(self->Items[i])) {
            if (i != self->FocusItem && total.y + item[i].h > self->MenuSys->getDisplayHeight() - kScreenGapY) {
                break;
            }
            item[i].x = total.x;
            item[i].y = self->MenuSys->f64 + total.y;
            item[i].w = total.w;
            total.y += item[i].h;
            self->Items[i]->render(&item[i]);
        }
    }
    if (self->TopItem) {
        sRect_2CAD58 up;
        up.y = self->MenuSys->f64 + kArrowEdgeOffset;
        up.x = (self->MenuSys->getDisplayWidth() - kArrowWidth) * 0.5f;
        up.w = kArrowWidth;
        up.h = kArrowHeight;
        func_002CB8C8_r(self, up);
    }
    if (i != self->NumItems) {
        sRect_2CAD58 down;
        down.x = (self->MenuSys->getDisplayWidth() - kArrowWidth) * 0.5f;
        down.y = self->MenuSys->f64 + self->MenuSys->getDisplayHeight() - kArrowHeight - kArrowEdgeOffset;
        down.w = kArrowWidth;
        down.h = kArrowHeight;
        func_002CBAC8_r(self, down);
    }
    func_002CB2F8_s(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB180);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002CB180_sRS {
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

class func_002CB180_cCtx {
public:
    char pad0[0xE84];
    func_002CB180_sRS* top;
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
    virtual void v26(int a, int b, float x, float y, float w, float h, float zn, float zf);
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34(void* p);
};

// Typed view of the render context (the unit declares GrMan with another class later).
extern char t4mat_Identity[];

static inline void func_002CB180_setF8_5(func_002CB180_sRS* rs, int v) { rs->f8_5 = v; }
static inline void func_002CB180_setF4_23(func_002CB180_sRS* rs, int v) { rs->f4_23 = v; }
static inline void func_002CB180_setF4_20(func_002CB180_sRS* rs, int v) { rs->f4_20 = v; }
static inline void func_002CB180_setF4_12(func_002CB180_sRS* rs, int v) { rs->f4_12 = v; }
static inline void func_002CB180_setF4_2(func_002CB180_sRS* rs, int v) { rs->f4_2 = v; }

extern "C" void func_002CB180()
{
    cPS2GraphicsMan* ctx = GrMan;
    ((func_002CB180_cCtx*)ctx)->top[1] = ((func_002CB180_cCtx*)ctx)->top[0];
    ((func_002CB180_cCtx*)ctx)->top++;
    ((func_002CB180_cCtx*)GrMan)->v21();
    ((func_002CB180_cCtx*)GrMan)->v31();
    func_002CB180_setF8_5(((func_002CB180_cCtx*)GrMan)->top, 0x11);
    func_002CB180_setF4_23(((func_002CB180_cCtx*)GrMan)->top, 2);
    func_002CB180_setF4_20(((func_002CB180_cCtx*)GrMan)->top, 3);
    func_002CB180_setF4_12(((func_002CB180_cCtx*)GrMan)->top, 0x14);
    func_002CB180_setF4_2(((func_002CB180_cCtx*)GrMan)->top, 5);
    ((func_002CB180_cCtx*)GrMan)->v26(0, 0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    ((func_002CB180_cCtx*)GrMan)->v34(t4mat_Identity);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB2F8);
#ifdef SKIP_ASM
class func_002CB2F8_cObj {
public:
    char data[0x10D8];
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
};


extern "C" void func_002CB2F8()
{
    ((func_002CB2F8_cObj*)GrMan)->v32();
    ((func_002CB2F8_cObj*)GrMan)->v22();
    *(int*)&GrMan->m_pCurrentDrawState = (int)GrMan->m_pCurrentDrawState - 0x14;
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

class func_002CB350_cCtx {
public:
    char pad0[0x10D8];
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
};

// Typed view of the render context (the unit declares GrMan with another class later).

extern "C" void func_002CB350(func_002CB350_sVtx* v, float* r, float* t)
{
    v[0].pos = func_002CB350_sVec4(r[0], r[1], 0.0f, 1.0f);
    v[0].u = t[0];
    v[0].v = t[1];
    v[1].pos = func_002CB350_sVec4(r[0] + r[2], r[1], 0.0f, 1.0f);
    v[1].u = t[0] + t[2];
    v[1].v = t[1];
    v[2].pos = func_002CB350_sVec4(r[0], r[1] + r[3], 0.0f, 1.0f);
    v[2].u = t[0];
    v[2].v = t[1] + t[3];
    v[3].pos = func_002CB350_sVec4(r[0] + r[2], r[1] + r[3], 0.0f, 1.0f);
    v[3].u = t[0] + t[2];
    v[3].v = t[1] + t[3];
    ((func_002CB350_cCtx*)GrMan)->v71(4, v, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB498);
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


static inline int func_002CB498_getF8_5(sRS_2CB498* rs)
{
    return (unsigned int)(rs->f8 & 0x3E0) >> 5;
}

static inline void func_002CB498_setF8_5(sRS_2CB498* rs, int v)
{
    if (v < 0) v = 0;
    rs->f8 = (rs->f8 & ~0x3E0) | ((v << 5) & 0x3E0);
}

extern "C" void func_002CB498(void* self, float* rect, func_002CA988_sColor* col)
{
    sVtx_2CB498 v[4];
    int i;
    for (i = 0; i < 4; i++) {
        v[i].a = (int)(col->a * 128.0f);
        v[i].r = (int)(col->r * 128.0f);
        v[i].g = (int)(col->g * 128.0f);
        v[i].b = (int)(col->b * 128.0f);
        v[i].q = 1.0f;
    }
    int old = func_002CB498_getF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState));
    func_002CB498_setF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState), old - 2);
    ((sRS_2CB498*)GrMan->m_pCurrentDrawState)->f4_2 = 5;
    int id = *(int*)(*(char**)((char*)self + 0x124) + 0x60);
    ((sRS_2CB498*)GrMan->m_pCurrentDrawState)->f10 = id;
    float r[4];
    float t[4];
    float left = rect[0] + 5.0f;
    float top = rect[1] + 5.0f;
    float right = rect[0] + rect[2] - 5.0f;
    float bot = rect[1] + rect[3] - 5.0f;
    float w = rect[2] - 10.0f;
    float h = rect[3] - 10.0f;
    r[0] = rect[0]; r[1] = rect[1]; r[2] = 5.0f; r[3] = 5.0f;
    t[0] = 0.0f; t[1] = 0.0f; t[2] = 0.25f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = right; r[1] = rect[1]; r[2] = 5.0f; r[3] = 5.0f;
    t[0] = 0.75f; t[1] = 0.0f; t[2] = 0.25f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = right; r[1] = bot; r[2] = 5.0f; r[3] = 5.0f;
    t[0] = 0.75f; t[1] = 0.75f; t[2] = 0.25f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = rect[0]; r[1] = bot; r[2] = 5.0f; r[3] = 5.0f;
    t[0] = 0.0f; t[1] = 0.75f; t[2] = 0.25f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = left; r[1] = rect[1]; r[2] = w; r[3] = 5.0f;
    t[0] = 0.25f; t[1] = 0.0f; t[2] = 0.5f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = left; r[1] = bot; r[2] = w; r[3] = 5.0f;
    t[0] = 0.25f; t[1] = 0.75f; t[2] = 0.5f; t[3] = 0.25f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = rect[0]; r[1] = top; r[2] = 5.0f; r[3] = h;
    t[0] = 0.0f; t[1] = 0.25f; t[2] = 0.25f; t[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = right; r[1] = top; r[2] = 5.0f; r[3] = h;
    t[0] = 0.75f; t[1] = 0.25f; t[2] = 0.25f; t[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    r[0] = left; r[1] = top; r[2] = w; r[3] = h;
    t[0] = 0.25f; t[1] = 0.25f; t[2] = 0.5f; t[3] = 0.5f;
    func_002CB350((func_002CB350_sVtx*)v, r, t);
    func_002CB498_setF8_5(((sRS_2CB498*)GrMan->m_pCurrentDrawState), old);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB880);
#ifdef SKIP_ASM
extern char D_004D5380[];
extern char D_004D5390[];

typedef void (*fn2CB880)(void*, void*, char*);

struct sVEntry2CB880 {
    short delta;
    short index;
    fn2CB880 fn;
};

extern "C" void func_002CB880(void* self, void* a1, int on)
{
    sVEntry2CB880* vt = *(sVEntry2CB880**)((char*)self + 0x12C);
    fn2CB880* f = &vt[8].fn;
    (*f)((char*)self + vt[8].delta, a1, on ? D_004D5380 : D_004D5390);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CB8C8);
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
    virtual void v71(int n, void* verts, int flags);
};


extern "C" void func_002CB8C8(void* self, float* rect)
{
    sVEntry2CB880* vt = *(sVEntry2CB880**)((char*)self + 0x12C);
    vt[8].fn((char*)self + vt[8].delta, rect, D_004D5380);
    *(short*)((char*)((cCtx_2CB8C8*)GrMan)->top + 0x10) = -1;
    ((cCtx_2CB8C8*)GrMan)->top->f4_2 = 1;
    sVtx_2CB8C8 v[6];
    int i;
    for (i = 0; i < 6; i++) {
        v[i].a = 0x80;
        v[i].r = (int)(D_004D53A0.x * 128.0f);
        v[i].g = (int)(D_004D53A0.y * 128.0f);
        v[i].b = (int)(D_004D53A0.z * 128.0f);
        v[i].u = 0;
        v[i].v = 0;
        v[i].q = 1.0f;
    }
    float bot = rect[1] + rect[3] - 4.0f;
    float left = rect[0] + 6.0f;
    float right = rect[0] + rect[2] - 6.0f;
    float mid = rect[0] + rect[2] * 0.5f;
    float top = rect[1] + 3.0f;
    sVec4_2CB8C8 p;
    p.x = left; p.y = bot; p.z = 0.0f; p.w = 1.0f;
    v[0].pos = p;
    p.x = left; p.y = bot - 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[1].pos = p;
    p.x = mid; p.y = top + 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[2].pos = p;
    p.x = mid; p.y = top; p.z = 0.0f; p.w = 1.0f;
    v[3].pos = p;
    p.x = right; p.y = bot; p.z = 0.0f; p.w = 1.0f;
    v[4].pos = p;
    p.x = right; p.y = bot - 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[5].pos = p;
    ((cCtx_2CB8C8*)GrMan)->v71(6, v, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CBAC8);
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
    virtual void v71(int n, void* verts, int flags);
};


extern "C" void func_002CBAC8(void* self, float* rect)
{
    sVEntry2CB880* vt = *(sVEntry2CB880**)((char*)self + 0x12C);
    vt[8].fn((char*)self + vt[8].delta, rect, D_004D5380);
    *(short*)((char*)((cCtx_2CBAC8*)GrMan)->top + 0x10) = -1;
    ((cCtx_2CBAC8*)GrMan)->top->f4_2 = 1;
    sVtx_2CBAC8 v[6];
    int i;
    for (i = 0; i < 6; i++) {
        v[i].a = 0x80;
        v[i].r = (int)(D_004D53A0.x * 128.0f);
        v[i].g = (int)(D_004D53A0.y * 128.0f);
        v[i].b = (int)(D_004D53A0.z * 128.0f);
        v[i].u = 0;
        v[i].v = 0;
        v[i].q = 1.0f;
    }
    float top = rect[1] + 3.0f;
    float left = rect[0] + 6.0f;
    float right = rect[0] + rect[2] - 6.0f;
    float mid = rect[0] + rect[2] * 0.5f;
    float bot = rect[1] + rect[3] - 4.0f;
    sVec4_2CBAC8 p;
    p.x = left; p.y = top; p.z = 0.0f; p.w = 1.0f;
    v[0].pos = p;
    p.x = left; p.y = top + 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[1].pos = p;
    p.x = mid; p.y = bot - 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[2].pos = p;
    p.x = mid; p.y = bot; p.z = 0.0f; p.w = 1.0f;
    v[3].pos = p;
    p.x = right; p.y = top; p.z = 0.0f; p.w = 1.0f;
    v[4].pos = p;
    p.x = right; p.y = top + 3.0f; p.z = 0.0f; p.w = 1.0f;
    v[5].pos = p;
    ((cCtx_2CBAC8*)GrMan)->v71(6, v, 0);
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
INCLUDE_ASM("util/menu", func_002CBD50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMenu_checkFocus(cMenu* self);

void func_002CBD50_impl(cMenuList* list, int index) __asm__("func_002CBD50");
void func_002CBD50_impl(cMenuList* list, int index)
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
INCLUDE_ASM("util/menu", func_002CBE10);
#ifdef SKIP_ASM
extern "C" void func_002CBE10(void *self) {
    *(int *)((char *)self + 4) = 0;
    *(int *)((char *)self + 0) = 0;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CBE20);
#ifdef SKIP_ASM
extern int (*D_004A54BC)(int, int);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));
extern "C" int func_002D4008(const void*, const void*);

extern "C" void func_002CBE20(void* self, int (*cmp)(int, int))
{
    D_004A54BC = cmp;
    func_00418EF8((char*)self + 0xC, *(int*)self, 4, func_002D4008);
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
    virtual void v06(int);
    virtual void v07();
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
        self->items[self->selected]->v07();
        self->selected = index;
        self->items[index]->v06(2);
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
    return *(void**)((char*)self + (cMenu_getFocus(self) << 2) + 0xC);
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
    virtual void v06(int);
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
    self->items[self->selected]->v06(2);
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
    void* item = self->items[self->selected];
    sVEntry2CBFD0* vt = *(sVEntry2CBFD0**)((char*)item + 0x10);
    vt[7].fn((char*)item + vt[7].delta);
    self->wrap = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_cMenuSys);
#ifdef SKIP_ASM
void cMenuSys_init(void* self);

extern "C" void* cMenuSys_cMenuSys(void* self)
{
    *(int*)self = 0;
    cMenuSys_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys__cMenuSys);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

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
void cMenuSys_init(void* self)
{
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)self = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x4c) = -1;
    *(int*)((char*)self + 0x48) = 11;
    *(int*)((char*)self + 0x50) = -1;
}
#endif

//100%
INCLUDE_ASM("util/menu", cMenuSys_isActive);
#ifdef SKIP_ASM
extern "C" int cMenuSys_isActive(void* self)
{
    int r = 0;
    if (*(int*)self != 0) {
        r = *(int*)((char*)self + 0x54) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC0B8__FPvi);
#ifdef SKIP_ASM
void func_002CC0B8(void* self, int val)
{
    *(int*)((char*)self + 0x54) = val;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC0C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
float cInput_getValue_f(void* pad, int axis) __asm__("cInput_getValue");
extern "C" int func_00266BA8(void* kb, int a1, int* key, int* chr);
extern "C" void func_002CC460(void* p);
extern void* D_004A33F0;

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

// Reads the pad (or USB keyboard) and lets the top menu handle it; pops menus on close.
extern "C" void func_002CC0C0(sMenu2CC0C0* self)
{
    if (self->depth == 0) {
        return;
    }
    if (!PadValid2CC0C0(self->pad)) {
        return;
    }
    int input = (int)cInput_getValue_f(self->pad, 0x3D);
    int* key = &self->key;
    int* chr = &self->chr;
    *key = -1;
    *chr = -1;
    self->input = input;
    if (input == -1 && D_004A33F0 != 0 && func_00266BA8(D_004A33F0, 0, key, chr) != 0) {
        if (self->chr < 0x20) {
            self->chr = input;
        }
        switch (self->key) {
        case 0x26: self->input = 0; break;
        case 0x28: self->input = 1; break;
        case 0x25: self->input = 2; break;
        case 0x27: self->input = 3; break;
        case 0x2D: self->input = 9; break;
        case 0x2E: self->input = 10; break;
        case 0xD: self->input = 6; break;
        case 0x1B: self->input = 7; break;
        }
    }
    char* item = self->items[self->depth - 1];
    sVE2CC0C0* vt = *(sVE2CC0C0**)(item + 0x12C);
    switch (vt[2].fn(item + vt[2].delta)) {
    case 1:
        func_002CC460(self);
        break;
    case 2:
        while (self->depth != 0) {
            func_002CC460(self);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC248__FPv);
#ifdef SKIP_ASM
int func_002CC248(void* self)
{
    return *(int*)((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC250__FPv);
#ifdef SKIP_ASM
int func_002CC250(void* self)
{
    return *(int*)((char*)self + 0x4C);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC258__FPv);
#ifdef SKIP_ASM
int func_002CC258(void* self)
{
    return *(int*)((char*)self + 0x50);
}
#endif

extern "C" void* cInput_getValue(int, int);

//100%
INCLUDE_ASM("util/menu", func_002CC260__FPv);
#ifdef SKIP_ASM
void* func_002CC260(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x5c), 0x3e);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC280__FPv);
#ifdef SKIP_ASM
void* func_002CC280(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x5c), 0x3f);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2A0__FPv);
#ifdef SKIP_ASM
void* func_002CC2A0(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x5c), 0x40);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2C0__FPv);
#ifdef SKIP_ASM
void* func_002CC2C0(void* self)
{
    return cInput_getValue(*(int*)((char*)self + 0x5c), 0x41);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC2E0);
#ifdef SKIP_ASM
// PORT: cInput_getValue is declared returning void* in this unit; this caller reads a float result ($f0).
float cInput_getValue_f(int, int) __asm__("cInput_getValue");
// PORT: the unit declares func_002CC2E0 as void*(int); the body takes the object pointer.
int func_002CC2E0_impl(void* self) __asm__("func_002CC2E0");

int func_002CC2E0_impl(void* self)
{
    return cInput_getValue_f(*(int*)((char*)self + 0x5c), 0x42) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC318);
#ifdef SKIP_ASM
// PORT: cInput_getValue is declared returning void* in this unit; this caller reads a float result ($f0).
float cInput_getValue_f(int, int) __asm__("cInput_getValue");
// PORT: the unit declares func_002CC318 as void*(int); the body takes the object pointer.
int func_002CC318_impl(void* self) __asm__("func_002CC318");

int func_002CC318_impl(void* self)
{
    return cInput_getValue_f(*(int*)((char*)self + 0x5c), 0x43) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC350);
#ifdef SKIP_ASM
class func_002CC350_cObj {
public:
    char data[0x12C];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

struct func_002CC350_sVec2 {
    float x;
    float y;
    func_002CC350_sVec2(float a, float b)
    {
        x = a;
        y = b;
    }
};

struct func_002CC350_sStack {
    int count;
    func_002CC350_cObj* items[1];
};

extern "C" void func_002CC350(void* self)
{
    func_002CC350_sVec2 scale(0.8500000238418579f, 0.8500000238418579f);
    char* o = *(char**)((char*)self + 0x58);
    *(float*)(o + 0x38) = *(float*)(o + 0x30) * scale.x;
    *(float*)(o + 0x3C) = *(float*)(o + 0x34) * scale.y;
    func_002CC350_sStack* st = (func_002CC350_sStack*)self;
    st->items[st->count - 1]->v05();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC3B8);
#ifdef SKIP_ASM
void func_002CC0B8(void*, int);

class func_002CC3B8_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
    virtual void v07();
};

struct func_002CC3B8_sStack {
    int count;
    func_002CC3B8_cMenu* items[1];
};

extern "C" void func_002CC3B8(void* stack, void* item)
{
    func_002CC3B8_sStack* self = (func_002CC3B8_sStack*)stack;
    if (self->count != 0) {
        self->items[self->count - 1]->v07();
    }
    self->items[self->count++] = (func_002CC3B8_cMenu*)item;
    self->items[self->count - 1]->v06(self);
    func_002CC0B8(self, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC460);
#ifdef SKIP_ASM
class func_002CC460_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
    virtual void v07();
};

struct func_002CC460_sStack {
    int count;                          // 0x0
    func_002CC460_cMenu* entries[8];    // 0x4
};

extern "C" void func_002CC460(void* p)
{
    func_002CC460_sStack* self = (func_002CC460_sStack*)p;
    self->count--;
    self->entries[self->count]->v07();
    if (self->count != 0) {
        self->entries[self->count - 1]->v06(self);
        if (self->count != 0) {
            return;
        }
    }
    func_002CC0B8(self, 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC500);
#ifdef SKIP_ASM
extern void* D_00486E80[];

extern "C" void* func_002CC500(void* self, void* text, int a2, int a3, int a4, int a5)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = a3;
    *(int*)((char*)self + 0x18) = a4;
    *(int*)((char*)self + 0x1C) = a5;
    *(int*)((char*)self + 0x20) = a2;
    *(void***)((char*)self + 0x10) = D_00486E80;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC578);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int cMenuItem_isLocked(void* self);
extern "C" int func_002CA3E8(void** self);

class func_002CC578_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(void*);
};

extern "C" int func_002CC578(void** self)
{
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) == 0 && r == 0) {
        if ((self[6] != 0 && self[7] != 0 && (func_002CA3E8(self) == 2 || func_002CA3E8(self) == 3)) || func_002CA3E8(self) == 6) {
            *(int*)self[8] ^= 1;
            ((func_002CC578_cMenu*)self[0])->v04(self);
        }
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC648);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

struct sMenuRectC648 { float x, y, z, w; };

extern "C" sMenuRectC648* func_002CC648(sMenuRectC648* ret, char* item)
{
    if (*(void**)(item + 0x18) != 0 && *(void**)(item + 0x1C) != 0) {
        sMenuRectC648 a;
        sMenuRectC648 b;
        sMenuRectC648 tmp;
        func_002CA4C8(&tmp, item, *(int*)(item + 0x14), 0, *(void**)(item + 0x18));
        a = tmp;
        func_002CA4C8(&tmp, item, *(int*)(item + 0x14), 0, *(void**)(item + 0x1C));
        b = tmp;
        *ret = (a.z > b.z) ? a : b;
    } else {
        func_002CA4C8(ret, item, 0, *(int*)(item + 0x14), 0);
    }
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CC758);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002CC758(void* self, int a1)
{
    void* a = *(void**)((char*)self + 0x18);
    void* b;
    if (a != 0 && (b = *(void**)((char*)self + 0x1C)) != 0) {
        func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, **(int**)((char*)self + 0x20) ? a : b);
    } else {
        func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC7B0);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486E30[];

extern "C" void* func_002CC7B0(void* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486E30;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x18) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CC7F8);
#ifdef SKIP_ASM
extern void* D_00486E30[];

extern "C" void* func_002CC7F8(void* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486E30;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x18) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CC838);
#ifdef SKIP_ASM
extern void* D_00486E30[];

extern "C" void* func_002CC838(void* self, void* text, int a2, int a3, int a4, int a5)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(void***)((char*)self + 0x10) = D_00486E30;
    *(int*)((char*)self + 0x14) = a3;
    *(int*)((char*)self + 0x20) = a2;
    *(int*)((char*)self + 0x18) = a4;
    *(int*)((char*)self + 0x1C) = a5;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CC8B0);
#ifdef SKIP_ASM
extern "C" void func_002CC8B0(void *self, int *table) {
    int index;

    *(int **)((char *)self + 0x1C) = table;
    *(int *)((char *)self + 0x18) = 0;
    if (*table != 0) {
        do {
            index = *(int *)((char *)self + 0x18) + 1;
            *(int *)((char *)self + 0x18) = index;
        } while (table[index * 2] != 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CC8F0);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int cMenuItem_isLocked(void* self);
extern "C" int func_002CA3E8(void** self);

struct sVEnt_2CC8F0 { short delta; short index; void* fn; };
typedef int (*Pick_2CC8F0)(void*, void*, int);
typedef void (*Notify_2CC8F0)(void*, void*);

struct sEnt_2CC8F0 {
    const char* name;
    int value;
};

struct sList_2CC8F0 {
    char* menu;
    char pad04[0x14];
    int count;
    sEnt_2CC8F0* entries;
    int* value;
};

static inline void notify_2CC8F0(sList_2CC8F0* self)
{
    sVEnt_2CC8F0* vt = *(sVEnt_2CC8F0**)(self->menu + 0x12C);
    ((Notify_2CC8F0)vt[4].fn)(self->menu + vt[4].delta, self);
}

extern "C" int func_002CC8F0(void* vself)
{
    sList_2CC8F0* self = (sList_2CC8F0*)vself;
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return r;
    }
    if (r != 0) {
        return r;
    }
    if (func_002CA3E8((void**)self) == 6) {
        sVEnt_2CC8F0* vt = *(sVEnt_2CC8F0**)(self->menu + 0x12C);
        r = ((Pick_2CC8F0)vt[3].fn)(self->menu + vt[3].delta, self, 0);
    } else {
    int n = self->count;
    int i = 0;
    for (;;) {
        if (i == n) {
            if (func_002CA3E8((void**)self) == 2 || func_002CA3E8((void**)self) == 0) {
                *self->value = self->entries[0].value;
                notify_2CC8F0(self);
            } else if (func_002CA3E8((void**)self) == 3 || func_002CA3E8((void**)self) == 1) {
                *self->value = self->entries[self->count - 1].value;
                notify_2CC8F0(self);
            }
            goto done;
        }
        if (*self->value == self->entries[i].value) {
            if (func_002CA3E8((void**)self) == 2 || func_002CA3E8((void**)self) == 0) {
                *self->value = self->entries[(i + self->count - 1) % self->count].value;
                notify_2CC8F0(self);
            } else if (func_002CA3E8((void**)self) == 3 || func_002CA3E8((void**)self) == 1) {
                *self->value = self->entries[(i + 1) % self->count].value;
                notify_2CC8F0(self);
            }
            goto done;
        }
        i++;
    }
done:
    return r;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCB18);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern char str_Invalid_b[];

struct sMenuRectCB18 { float x, y, z, w; };
struct sMenuEntCB18 { void* name; int data; };

extern "C" sMenuRectCB18* func_002CCB18(sMenuRectCB18* ret, char* item)
{
    sMenuRectCB18 best;
    sMenuRectCB18 tmp;
    func_002CA4C8(&tmp, item, *(int*)(item + 0x14), 0, str_Invalid_b);
    best = tmp;
    for (int i = 0; i < *(int*)(item + 0x18); i++) {
        func_002CA4C8(&tmp, item, *(int*)(item + 0x14), 0, (*(sMenuEntCB18**)(item + 0x1C))[i].name);
        if (tmp.z > best.z) {
            best = tmp;
        }
    }
    *ret = best;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCC38);
#ifdef SKIP_ASM
extern char str_Invalid_b[];

struct func_002CCC38_sEntry {
    char* name;  // 0x0
    int value;   // 0x4
};

extern "C" void func_002CCC38(void* self, int a1)
{
    int i = 0;
    for (;;) {
        if (i == *(int*)((char*)self + 0x18)) {
            func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, str_Invalid_b);
            break;
        }
        // PORT: pointer held in int (index-first address arithmetic)
        func_002CCC38_sEntry* e = (func_002CCC38_sEntry*)((i << 3) + *(int*)((char*)self + 0x1C));
        if (**(int**)((char*)self + 0x20) == e->value) {
            func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, e->name);
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

struct cSubMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

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
extern "C" cSubMenuItem_sItem* cSubMenuItem_cSubMenuItem(cSubMenuItem_sItem* self, void* sub, void* cap)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = cap;
    self->field_0x18 = sub;
    self->vtable = cSubMenuItem_vtbl;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_input);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);
extern "C" void func_002CC3B8(void* stack, void* item);

/**
 * Handles input for a sub-menu item: the select command pushes the item's sub-menu onto the menu system.
 *
 * When the item is not locked, the base navigation result is 0 and the menu system's current command is 6
 * (select; Enter on a USB keyboard), it pushes the sub-menu (+0x18) onto Menu->MenuSys with func_002CC3B8,
 * which takes focus from the current menu.
 *
 * @return the base navigation result from func_002CA2B0 (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
extern "C" int cSubMenuItem_input(void* self)
{
    int result = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) == 0 && result == 0 && func_002CA3E8((void**)self) == 6) {
        func_002CC3B8(*(void**)((char*)*(void**)self + 0x124), *(void**)((char*)self + 0x18));
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

/**
 * Computes a sub-menu item's row rectangle: its label (+0x14) measured as centred text.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * func_002CA4C8 measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the sub-menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cSubMenuItem_preRender(void* self, void* item)
{
    func_002CA4C8(self, item, 0, *(int*)((char*)item + 0x14), 0);
    return self;
}
#endif

extern "C" void* func_002CA988(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cSubMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

/**
 * Draws a sub-menu item: its row background and its label centred.
 *
 * func_002CA988 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 * @return no meaningful value (the draw helper returns nothing; On Tour's render is void)
 */
void* cSubMenuItem_render(void* self, int rect)
{
    return func_002CA988_5(self, rect, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCDF0);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486D90[];

struct s2CCDF0Item {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

extern "C" s2CCDF0Item* func_002CCDF0(s2CCDF0Item* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = text;
    self->vtable = D_00486D90;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCE38);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);
extern "C" void func_002CC460(void* stack);

extern "C" int func_002CCE38(void* self)
{
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        func_002CC460(*(void**)((char*)*(void**)self + 0x124));
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCEA8);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CCEA8(void* self, void* a1)
{
    func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CCEE0__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CCEE0(void* self, int a1)
{
    return func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCF08);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486D40[];

struct s2CCF08Item {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
};

extern "C" s2CCF08Item* func_002CCF08(s2CCF08Item* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = 0;
    self->vtable = D_00486D40;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->field_0x18 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCF50);
#ifdef SKIP_ASM
extern void* D_00486D40[];

extern "C" void* func_002CCF50(void* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486D40;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x18) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCF98);
#ifdef SKIP_ASM
extern void* D_00486D40[];

struct func_002CCF98_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
};

extern "C" func_002CCF98_sItem* func_002CCF98(func_002CCF98_sItem* self, void* text, int a2, int a3)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = a2;
    self->vtable = D_00486D40;
    self->field_0x1C = a3;
    self->field_0x20 = 0;
    self->field_0x18 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CCFF8);
#ifdef SKIP_ASM
extern "C" void func_002CCFF8(void *arg0, int arg1, int arg2) {
    (*(int *)((char*)(arg0) + (0x14))) = arg1;
    (*(int *)((char*)(arg0) + (0x1C))) = arg2;
    (*(int *)((char*)(arg0) + (0x18))) = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD008);
#ifdef SKIP_ASM
class func_002CD008_cMenu {
public:
    char pad[0x12C];
    // vptr at 0x12C; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(void* item, void* data);
};

extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CD008(void* self)
{
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        r = (*(func_002CD008_cMenu**)self)->v03(self, *(void**)((char*)self + 0x1C));
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD090);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

extern "C" void* func_002CD090(void* self, void* a1)
{
    void* p = *(void**)((char*)a1 + 0x18);
    if (p != 0) {
        func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, p);
    } else {
        func_002CA4C8(self, a1, 0, *(int*)((char*)a1 + 0x14), 0);
    }
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD0E8);
#ifdef SKIP_ASM
// PORT: The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002CD0E8(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x18);
    if (p != 0) {
        func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, p);
    } else {
        func_002CA988_5(self, a1, 0, *(int*)((char*)self + 0x14), 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD128);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* D_00486CF0[];

extern "C" void* func_002CD128(void* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486CF0;
    *(int*)((char*)self + 0x18) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD168);
#ifdef SKIP_ASM
extern void* D_00486CF0[];

extern "C" void* func_002CD168(void* self, void* text, int a2, int a3)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = a2;
    *(int*)((char*)self + 0x18) = a3;
    *(void***)((char*)self + 0x10) = D_00486CF0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD1C0);
#ifdef SKIP_ASM
extern "C" void func_002CD1C0(void *self, int a, int b) {
    *(int *)((char *)self + 0x14) = a;
    *(int *)((char *)self + 0x18) = b;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD1D0);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);

extern "C" int func_002CD1D0(void* self)
{
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) == 0 && r == 0 && func_002CA3E8((void**)self) == 6) {
        (*(void (**)())((char*)self + 0x18))();
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD240);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866C8[];

extern "C" void* func_002CD240(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866C8);
    return self;
}
#endif

extern void* D_004866C8[];
extern "C" void* func_002CA988(void*, int, int, int);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD278__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

void* func_002CD278(void* self, int a1)
{
    return func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, D_004866C8);
}
#endif

extern void* cNullMenuItem_vtbl[16];

struct cNullMenuItem {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

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
    self->field_0x14 = text;
    self->vtable = cNullMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD2E8__FPv);
#ifdef SKIP_ASM
int func_002CD2E8(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cNullMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);

/**
 * Computes a text-only menu item's row rectangle: its text (+0x14) measured as centred text.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * func_002CA4C8 measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the text-only item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cNullMenuItem_preRender(void* self, void* item)
{
    func_002CA4C8(self, item, 0, *(int*)((char*)item + 0x14), 0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cNullMenuItem_render__FPvi);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

/**
 * Draws a text-only menu item: its row background and its text (+0x14) centred.
 *
 * func_002CA988 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 * @return no meaningful value (the draw helper returns nothing; On Tour's render is void)
 */
void* cNullMenuItem_render(void* self, int rect)
{
    return func_002CA988_5(self, rect, 0, *(int*)((char*)self + 0x14), 0);
}
#endif

extern void* cSpaceMenuItem_vtbl[16];

struct cSpaceMenuItem {
    char pad_0x00[0x10];
    void* vtable;
    void* field_0x14;
};

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_cSpaceMenuItem__FP14cSpaceMenuItemPv);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
// A void* constant -1 materializes as lui/ori; the original passed an int.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

/**
 * Constructs a blank spacer menu item of a given size.
 *
 * Runs the base constructor with ID -1, stores the size at +0x14, then cSpaceMenuItem's vtable. preRender
 * returns it as the last component of the item's rectangle, and render draws nothing.
 *
 * @param text  the spacer's size (callers pass 10 or 30); despite the name it is a number, not text
 * @return @p self
 */
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = text;
    self->vtable = cSpaceMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD398__FPv);
#ifdef SKIP_ASM
int func_002CD398(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cSpaceMenuItem_preRender);
#ifdef SKIP_ASM
struct s2CD3A0Vec {
    float x, y, z, w;
};

/**
 * Returns a spacer item's row rectangle: zero position and width, with the spacer's size (+0x14) as the
 * height.
 *
 * The int size is converted to float; the 16-byte rectangle comes back through the hidden result pointer.
 *
 * @return the rectangle {0, 0, 0, size}
 */
extern "C" s2CD3A0Vec cSpaceMenuItem_preRender(void* self)
{
    s2CD3A0Vec rect;
    rect.x = rect.y = rect.z = 0.0f;
    rect.w = (float)*(int*)((char*)self + 0x14);
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
INCLUDE_ASM("util/menu", func_002CD3F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* D_00486C00[];

extern "C" void* func_002CD3F8(void* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486C00;
    *(int*)((char*)self + 0x18) = 0;
    *(char*)((char*)self + 0x1C) = 0;
    *(char*)((char*)self + 0x1D) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD450);
#ifdef SKIP_ASM
extern void* D_00486C00[];

extern "C" void* func_002CD450(void* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = D_00486C00;
    *(int*)((char*)self + 0x18) = 0;
    *(char*)((char*)self + 0x1C) = 0;
    *(char*)((char*)self + 0x1D) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD4A0);
#ifdef SKIP_ASM
extern void* D_00486C00[];

extern "C" void* func_002CD4A0(void* self, void* text, int a2, signed char a3, signed char a4, int a5)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = a5;
    *(int*)((char*)self + 0x18) = a2;
    *(char*)((char*)self + 0x1C) = a3;
    *(char*)((char*)self + 0x1D) = a4;
    *(void***)((char*)self + 0x10) = D_00486C00;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD528);
#ifdef SKIP_ASM
extern "C" void func_002CD528(void* self, int lo, int hi)
{
    char* s = (char*)self;
    signed char* p = *(signed char**)(s + 0x18);
    *(signed char*)(s + 0x1D) = hi;
    *(signed char*)(s + 0x1C) = lo;
    if (p != 0) {
        if (*p < *(signed char*)(s + 0x1C))
            *p = lo;
        signed char* q = *(signed char**)(s + 0x18);
        if (*(signed char*)(s + 0x1D) < *q)
            *q = *(signed char*)(s + 0x1D);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD578);
#ifdef SKIP_ASM
extern "C" void func_002CD578(void* self)
{
    if (*(signed char*)((char*)self + 0x1C) <= 0) {
        **(signed char**)((char*)self + 0x18) = 0;
    } else {
        **(signed char**)((char*)self + 0x18) = *(signed char*)((char*)self + 0x1C);
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD5A0);
#ifdef SKIP_ASM
// PORT: func_002CA408/func_002CA428 return a float (axis value, $f0); the unit declares them void*.
float func_002CA408_f(void* self) __asm__("func_002CA408__FPv");
float func_002CA428_f(void* self) __asm__("func_002CA428__FPv");

struct sVEntry002CD5A0 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

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

extern "C" int func_002CD5A0(void* self)
{
    char* s = (char*)self;
    int r = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return r;
    }
    if (r != 0) {
        return r;
    }
    float d = func_002CA428_f(self) - func_002CA408_f(self);
    int dir;
    if (d <= -0.1f || d >= 0.1f) {
        dir = 1;
        if (d < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == *(int*)(s + 0x24)) {
        *(int*)(s + 0x20) += 1;
    } else {
        *(int*)(s + 0x20) = 0;
        *(int*)(s + 0x24) = dir;
        *(float*)(s + 0x28) = (float)dir;
    }
    if (dir != 0) {
        int n = *(int*)(s + 0x20);
        d *= 0.1f;
        d *= (float)(n * n / 0x120);
        float div = 1.0f;
        if (func_002CA488(self) != 0 && func_002CA4A8(self) != 0) {
            div = 50.0f;
        } else if (func_002CA488(self) != 0) {
            div = 20.0f;
        } else if (func_002CA4A8(self) != 0) {
            div = 5.0f;
        }
        d = d / div;
        *(float*)(s + 0x28) += d;
        d = *(float*)(s + 0x28);
        signed char* p = *(signed char**)(s + 0x18);
        int cur = *p;
        int v = (int)ffloor_002CD5A0((float)cur + d);
        int lo = *(signed char*)(s + 0x1C);
        int hi = *(signed char*)(s + 0x1D);
        if (v < lo) {
            v = lo;
        }
        if (hi < v) {
            v = hi;
        }
        *(float*)(s + 0x28) = d + (float)(cur - v);
        *p = v;
        char* menu = *(char**)s;
        sVEntry002CD5A0* e = &(*(sVEntry002CD5A0**)(menu + 0x12C))[4];
        e->fn(menu + e->delta, self);
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD7B8);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866E0[];

extern "C" void* func_002CD7B8(void* self, void* a1)
{
    func_002CA4C8(self, a1, *(int*)((char*)a1 + 0x14), 0, D_004866E0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CD7F0);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern char str_pct_d_i[];

extern "C" void func_002CD7F0(void* self, int a1)
{
    char buf[0x70];
    sprintf(buf, str_pct_d_i, **(signed char**)((char*)self + 0x18));
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x14), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD850);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cIntMenuItem_vtbl[];

extern "C" void* func_002CD850(void* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = cIntMenuItem_vtbl;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CD8A8);
#ifdef SKIP_ASM
extern void* cIntMenuItem_vtbl[];

extern "C" void* func_002CD8A8(void* self, void* text)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = 0;
    *(void***)((char*)self + 0x10) = cIntMenuItem_vtbl;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_cIntMenuItem);
#ifdef SKIP_ASM
extern void* cIntMenuItem_vtbl[];

struct cIntMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1C;
    int field_0x20;
    int field_0x24;
    int field_0x28;
    int field_0x2C;
};

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
extern "C" cIntMenuItem_sItem* cIntMenuItem_cIntMenuItem(cIntMenuItem_sItem* self, void* text, int value, int mini, int maxi, int cap)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = cap;
    self->field_0x18 = value;
    self->field_0x1C = mini;
    self->field_0x20 = maxi;
    self->vtable = cIntMenuItem_vtbl;
    self->field_0x28 = 0;
    self->field_0x24 = 0;
    self->field_0x2C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD978);
#ifdef SKIP_ASM
extern "C" void func_002CD978(void *arg0, int arg1, int arg2) {
    int *temp_3;
    int *temp_3_2;
    int temp_4;

    temp_3 = (*(int **)((char*)(arg0) + (0x18)));
    (*(int *)((char*)(arg0) + (0x20))) = arg2;
    (*(int *)((char*)(arg0) + (0x1C))) = arg1;
    if (temp_3 != 0) {
        if (*temp_3 < arg1) {
            *temp_3 = arg1;
        }
        temp_3_2 = (*(int **)((char*)(arg0) + (0x18)));
        temp_4 = (*(int *)((char*)(arg0) + (0x20)));
        if (temp_4 < *temp_3_2) {
            *temp_3_2 = temp_4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CD9B8);
#ifdef SKIP_ASM
extern "C" void func_002CD9B8(void* self)
{
    int* p = *(int**)((char*)self + 0x18);
    int v = *(int*)((char*)self + 0x1c);
    if (v <= 0) {
        *p = 0;
    } else {
        *p = v;
    }
}
#endif

//100%
INCLUDE_ASM("util/menu", cIntMenuItem_input);
#ifdef SKIP_ASM
// PORT: func_002CA408/func_002CA428 return a float (axis value, $f0); the unit declares them void*.
float func_002CA408_f(void* self) __asm__("func_002CA408__FPv");
float func_002CA428_f(void* self) __asm__("func_002CA428__FPv");

struct sVEntry002CD9D8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

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
 * @return the base navigation result from func_002CA2B0 (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
extern "C" int cIntMenuItem_input(void* self)
{
    char* menuItem = (char*)self;
    int result = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = func_002CA428_f(self) - func_002CA408_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == *(int*)(menuItem + 0x28)) {
        *(int*)(menuItem + 0x24) += 1;
    } else {
        *(int*)(menuItem + 0x24) = 0;
        *(int*)(menuItem + 0x28) = dir;
        *(float*)(menuItem + 0x2C) = (float)dir;
    }
    if (dir != 0) {
        int n = *(int*)(menuItem + 0x24);
        side *= 0.1f;
        side *= (float)(n * n / 0x120);
        float div = 1.0f;
        if (func_002CA488(self) != 0 && func_002CA4A8(self) != 0) {
            div = 50.0f;
        } else if (func_002CA488(self) != 0) {
            div = 20.0f;
        } else if (func_002CA4A8(self) != 0) {
            div = 5.0f;
        }
        side = side / div;
        *(float*)(menuItem + 0x2C) += side;
        side = *(float*)(menuItem + 0x2C);
        int* valuePtr = *(int**)(menuItem + 0x18);
        int cur = *valuePtr;
        int newValue = (int)ffloor_002CD9D8((float)cur + side);
        int lo = *(int*)(menuItem + 0x1C);
        int hi = *(int*)(menuItem + 0x20);
        if (newValue < lo) {
            newValue = lo;
        }
        if (hi < newValue) {
            newValue = hi;
        }
        *(float*)(menuItem + 0x2C) = side + (float)(cur - newValue);
        *valuePtr = newValue;
        char* menu = *(char**)menuItem;
        sVEntry002CD9D8* vtEntry = &(*(sVEntry002CD9D8**)(menu + 0x12C))[4];
        vtEntry->fn(menu + vtEntry->delta, self);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cIntMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866E0[];

/**
 * Computes an int item's row rectangle: its label on the left and room for "-99999999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * func_002CA4C8 measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the int menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cIntMenuItem_preRender(void* self, void* item)
{
    func_002CA4C8(self, item, *(int*)((char*)item + 0x14), 0, D_004866E0);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cIntMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern char str_pct_d_i[];

/**
 * Draws an int item: its label on the left and the variable's value ("%d") on the right.
 *
 * func_002CA988 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cIntMenuItem_render(void* self, int rect)
{
    char buf[0x70];
    sprintf(buf, str_pct_d_i, **(int**)((char*)self + 0x18));
    func_002CA988_5(self, rect, *(int*)((char*)self + 0x14), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CDC88);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* cFloatMenuItem_vtbl[];

extern "C" void* func_002CDC88(void* self)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(void***)((char*)self + 0x10) = cFloatMenuItem_vtbl;
    *(float*)((char*)self + 0x24) = 1.0f;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x28) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_cFloatMenuItem);
#ifdef SKIP_ASM
extern void* cFloatMenuItem_vtbl[];

struct cFloatMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    int field_0x18;
    float field_0x1C;
    float field_0x20;
    float field_0x24;
    int field_0x28;
    int field_0x2C;
    int field_0x30;
};

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
extern "C" cFloatMenuItem_sItem* cFloatMenuItem_cFloatMenuItem(cFloatMenuItem_sItem* self, void* text, int value, int minf, float maxf, float cap)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    self->field_0x14 = minf;
    self->field_0x18 = value;
    self->field_0x1C = maxf;
    self->field_0x20 = cap;
    self->vtable = cFloatMenuItem_vtbl;
    self->field_0x24 = 1.0f;
    self->field_0x30 = 0;
    self->field_0x2C = 0;
    self->field_0x28 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cFloatMenuItem_input);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// PORT: func_002CA408/func_002CA428 return a float (axis value, $f0); the unit declares them void*.
float func_002CA408_f(void* self) __asm__("func_002CA408__FPv");

float func_002CA428_f(void* self) __asm__("func_002CA428__FPv");

// (prototype from their definition at line 554)
void* func_002CA488_DD78(void* self) __asm__("func_002CA488__FPv");

// (prototype from their definition at line 565)
void* func_002CA4A8_DD78(void* self) __asm__("func_002CA4A8__FPv");

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
 * @return the base navigation result from func_002CA2B0 (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cFloatMenuItem_input(void* self)
{
    char* menuItem = (char*)self;
    int result = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = func_002CA428_f(self) - func_002CA408_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == *(int*)(menuItem + 0x30)) {
        *(int*)(menuItem + 0x2C) += 1;
    } else {
        *(int*)(menuItem + 0x2C) = 0;
        *(int*)(menuItem + 0x30) = dir;
    }
    if (dir != 0) {
        float div = 1.0f;
        if (func_002CA488_DD78(self) != 0 && func_002CA4A8_DD78(self) != 0) {
            div = 50.0f;
        } else if (func_002CA488_DD78(self) != 0) {
            div = 20.0f;
        } else if (func_002CA4A8_DD78(self) != 0) {
            div = 5.0f;
        }
        int n = *(int*)(menuItem + 0x2C);
        float k = (float)(n * n) * 50.0f;
        k /= div;
        k *= 6.944444612599909e-05f;
        k = k >? 1.0f;
        float step = 4.999999873689376e-05f;
        step = k * step;
        step /= div;
        float* valuePtr = *(float**)(menuItem + 0x18);
        float value = *(float*)(menuItem + 0x24) * *valuePtr + *(float*)(menuItem + 0x28);
        value += side * step * (*(float*)(menuItem + 0x20) - *(float*)(menuItem + 0x1C));
        if (value < *(float*)(menuItem + 0x1C)) {
            value = *(float*)(menuItem + 0x1C);
        }
        if (*(float*)(menuItem + 0x20) < value) {
            value = *(float*)(menuItem + 0x20);
        }
        **(float**)(menuItem + 0x18) = (value - *(float*)(menuItem + 0x28)) / *(float*)(menuItem + 0x24);
        char* menu = *(char**)menuItem;
        sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(menu + 0x12C);
        vt[4].fn(menu + vt[4].delta, self);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CDF78);
#ifdef SKIP_ASM
extern "C" void func_002CDF78(void* self)
{
    float* p = *(float**)((char*)self + 0x18);
    float v = *(float*)((char*)self + 0x1c);
    if (v <= 0.0f) {
        *p = 0.0f;
    } else {
        *p = v;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cFloatMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866F0[];

/**
 * Computes a float item's row rectangle: its label on the left and room for "-9999999999.999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * func_002CA4C8 measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the float menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cFloatMenuItem_preRender(void* self, void* item)
{
    func_002CA4C8(self, item, *(int*)((char*)item + 0x14), 0, D_004866F0);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", prettyFloat);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cBXString_cBXString1(void* self, void* src);
extern "C" int func_003194A8(void* str, const char* fmt, ...);
extern "C" float func_0040DA10(float x);
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
    float logValue = func_0040DA10(value) / func_0040DA10(10.0f) + 4.3431620724732056e-05f;
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
    func_003194A8(&formatted, fmt, fptodp(value));
    cBXString_cBXString1(out, &formatted);
    cBXString__cBXString(&formatted, 2);
    return out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cFloatMenuItem_render);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
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
 * cBXString that is destroyed afterwards. func_002CA988 picks the focused or normal style and text colour (is
 * the item its menu's focus item?), draws the row background through the menu (vtable slot 8), then the text
 * with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cFloatMenuItem_render(void* self, int rect)
{
    cFloatMenuItem_render_sString pretty;
    prettyFloat(&pretty, **(float**)((char*)self + 0x18) * *(float*)((char*)self + 0x24) + *(float*)((char*)self + 0x28));
    func_002CA988_5(self, rect, *(int*)((char*)self + 0x14), 0, pretty.str);
    cBXString__cBXString(&pretty, 2);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CE170);
#ifdef SKIP_ASM
extern void* cAngleMenuItem_vtbl[];

extern "C" void* func_002CE170(void* self, void* text, int a2, int a3, float f1, float f2)
{
    cMenuItem_cMenuItem((cMenuItem*)self, text);
    *(int*)((char*)self + 0x14) = a3;
    *(int*)((char*)self + 0x18) = a2;
    *(float*)((char*)self + 0x1C) = f1;
    *(float*)((char*)self + 0x20) = f2;
    *(void***)((char*)self + 0x10) = cAngleMenuItem_vtbl;
    *(int*)((char*)self + 0x28) = 0;
    *(int*)((char*)self + 0x24) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cAngleMenuItem_input);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int cMenuItem_isLocked(void* self);

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
 * @return the base navigation result from func_002CA2B0 (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cAngleMenuItem_input(void* self)
{
    char* menuItem = (char*)self;
    int result = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = func_002CA428_f(self) - func_002CA408_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == *(int*)(menuItem + 0x28)) {
        *(int*)(menuItem + 0x24) += 1;
    } else {
        *(int*)(menuItem + 0x24) = 0;
        *(int*)(menuItem + 0x28) = dir;
    }
    if (dir != 0) {
        int n = *(int*)(menuItem + 0x24);
        float k = (float)(n * n) * 0.0034722222480922937f;
        k = k >? 1.0f;
        float step = 4.999999873689376e-05f;
        step = k * step;
        float* valuePtr = *(float**)(menuItem + 0x18);
        *valuePtr = *valuePtr + side * step * (*(float*)(menuItem + 0x20) - *(float*)(menuItem + 0x1C));
        if (**(float**)(menuItem + 0x18) < *(float*)(menuItem + 0x1C)) {
            **(float**)(menuItem + 0x18) = *(float*)(menuItem + 0x1C);
        }
        if (*(float*)(menuItem + 0x20) < **(float**)(menuItem + 0x18)) {
            **(float**)(menuItem + 0x18) = *(float*)(menuItem + 0x20);
        }
        char* menu = *(char**)menuItem;
        sVEntry002CD9D8* vt = *(sVEntry002CD9D8**)(menu + 0x12C);
        vt[4].fn(menu + vt[4].delta, self);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cAngleMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern void* D_004866F0[];

/**
 * Computes an angle item's row rectangle: its label on the left and room for "-9999999999.999" on the right.
 *
 * Returns a tRect by value: @p self is really the hidden result pointer and @p item the item (On Tour's this).
 * func_002CA4C8 measures the texts in the menu system's font, adds 12 to the height and makes the width at
 * least 100, plus 32.
 *
 * @param item  the angle menu item (this)
 * @return @p self, the filled rectangle
 */
extern "C" void* cAngleMenuItem_preRender(void* self, void* item)
{
    func_002CA4C8(self, item, *(int*)((char*)item + 0x14), 0, D_004866F0);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cAngleMenuItem_render);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");
extern char str_pct_3_2f[];

/**
 * Draws an angle item: its label on the left and the angle in degrees ("%3.2f") on the right.
 *
 * The variable holds radians; it is multiplied by 57.2958 and passed to sprintf as a double (fptodp).
 * func_002CA988 picks the focused or normal style and text colour (is the item its menu's focus item?), draws
 * the row background through the menu (vtable slot 8), then the text with a 2-pixel drop shadow.
 *
 * @param rect  the row rectangle (a tRect pointer passed as an int)
 */
extern "C" void cAngleMenuItem_render(void* self, int rect)
{
    char buf[0x70];
    sprintf(buf, str_pct_3_2f, fptodp(**(float**)((char*)self + 0x18) * 57.2957763671875f));
    func_002CA988_5(self, rect, *(int*)((char*)self + 0x14), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CE418);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern void* D_00486AC0[];

struct func_002CE418_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    int field_0x14;
    char name[0x10]; // 0x18
    int field_0x28;
};

extern "C" func_002CE418_sItem* func_002CE418(func_002CE418_sItem* self, int a1, const char* name)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = a1;
    self->field_0x28 = 0;
    self->vtable = D_00486AC0;
    strncpy(self->name, name, 0xF);
    self->name[0xF] = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CE488);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int func_002CA3E8(void** self);
extern "C" int strlen(const char* s);
int func_002CC250(void* self);
int func_002CC258(void* self);

struct sTextItem_E488 {
    char* menu;                 // 0x00
    char pad4[0x14];            // 0x04
    char name[0x10];            // 0x18
    int cursor;                 // 0x28
};

static inline int validChar_E488(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '!';
}

extern "C" int func_002CE488(sTextItem_E488* self)
{
    int r = func_002CA2B0(self);
    if (r == 0) {
    int len = strlen(self->name);
    int mod = len + 1;
    if (mod >= 16) {
        mod = 15;
    }
    int key = func_002CA3E8((void**)self);
    if (key == 2) {
        self->cursor = (self->cursor + mod - 1) % mod;
    } else if (key == 3) {
        self->cursor = (self->cursor + 1) % mod;
    } else if (key == 4) {
        // PORT: pointer in int (index-first address).
        char* c = (char*)(self->cursor + (int)self);
        c += 0x18;
        do {
            (*c)--;
        } while (!validChar_E488(*c));
    } else if (key == 5) {
        // PORT: pointer in int (index-first address).
        char* c = (char*)(self->cursor + (int)self);
        c += 0x18;
        do {
            (*c)++;
        } while (!validChar_E488(*c));
    } else if (key == 9) {
        if (len < 14) {
            for (int i = self->cursor; i < len; i++) {
                self->name[i + 1] = self->name[i];
            }
            self->name[self->cursor] = '_';
        }
    } else if (key == 10) {
        if (len != 0 && self->cursor < len) {
            for (int i = self->cursor; i < len - 1; i++) {
                self->name[i] = self->name[i + 1];
            }
            self->name[len - 1] = 0;
        }
    } else if (key == 6) {
        r = 3;
    } else {
        int code = func_002CC250(*(void**)(self->menu + 0x124));
        int ch = func_002CC258(*(void**)(self->menu + 0x124));
        if (code == 8) {
            if (len != 0 && self->cursor > 0) {
                for (int i = self->cursor; i < len - 1; i++) {
                    self->name[i] = self->name[i + 1];
                }
                self->name[len - 1] = 0;
                self->cursor--;
            }
        } else if (validChar_E488(ch)) {
            if (len < 14) {
                for (int i = self->cursor; i < len; i++) {
                    self->name[i + 1] = self->name[i];
                }
                self->name[self->cursor++] = ch;
            }
        }
    }
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CE820);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strlen(const char* s);
extern "C" char* strcat(char* dst, const char* src);
extern char D_004A39D0[];

struct func_002CE820_sResult {
    int a;
    int b;
    int c;
    int d;
};

static inline func_002CE820_sResult func_002CE820_make(void* item, int id, char* text)
{
    func_002CE820_sResult t;
    func_002CA4C8(&t, item, id, 0, text);
    return t;
}

extern "C" func_002CE820_sResult func_002CE820(void* item)
{
    char buf[0x20];
    char* name = (char*)item + 0x18;
    int focused = item == cMenu_getFocusItem((cMenu*)(*(void**)item));
    strcpy(buf, name);
    int len = strlen(name);
    if (focused && *(int*)((char*)item + 0x28) == len && *(int*)((char*)item + 0x28) < 15) {
        strcat(buf, D_004A39D0);
    }
    func_002CE820_sResult r = func_002CE820_make(item, *(int*)((char*)item + 0x14), buf);
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CE910);
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
    virtual void v71(int n, void* verts, int flags);
};


extern "C" void func_002CE910(void* self, float* rect)
{
    char buf[0x20];
    char* name = (char*)self + 0x18;
    int focused = self == cMenu_getFocusItem((cMenu*)(*(void**)self));
    strcpy(buf, name);
    int len = strlen(name);
    if (focused && *(int*)((char*)self + 0x28) == len && *(int*)((char*)self + 0x28) < 15) {
        strcat(buf, D_004A39D0);
    }
    {
        char* menu = *(char**)self;
        sVEntry2CB880* vt = *(sVEntry2CB880**)(menu + 0x12C);
        fn2CB880* f = &vt[8].fn;
        (*f)(menu + vt[8].delta, rect, focused ? D_004D5380 : D_004D5390);
    }
    func_002CA988_sVec3* col = self == cMenu_getFocusItem((cMenu*)(*(void**)self)) ? &D_004D53A0 : &D_004D53B0;
    func_002CA988_sColor c;
    c.a = 1.0f;
    c.r = col->x;
    c.g = col->y;
    c.b = col->z;
    *(func_002CA988_sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = c;
    *(sVec2_2CE910*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CE910(2.0f, 2.0f);
    char* label = *(char**)((char*)self + 0x14);
    if (label) {
        cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f, label);
    }
    char* font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
    float w = cFont_textExtent(font, buf, 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + (rect[2] - w) - 16.0f, rect[1] + 6.0f - 1.0f, buf);
    if (focused) {
        font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
        float w1 = cFont_textExtent(font, buf + *(int*)((char*)self + 0x28), 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
        font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
        char* next = buf + 1;
        float w2 = cFont_textExtent(font, next + *(int*)((char*)self + 0x28), 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
        float x1 = rect[0] + (rect[2] - w1) - 16.0f;
        float bot = rect[1] + rect[3] - 6.0f;
        float x2 = rect[0] + (rect[2] - w2) - 16.0f;
        float y1 = bot - 4.0f;
        float y2 = bot - 1.0f;
        sVtx_2CE910 v[4];
        float zero = 0.0f;
        float one = 1.0f;
        sVec4_2CE910 p;
        p.Set(x1, y1, zero, one);
        v[0].v = zero;
        v[0].u = zero;
        v[0].q = one;
        v[0].r = (int)(c.r * 127.0f);
        v[0].g = (int)(c.g * 127.0f);
        v[0].b = (int)(c.b * 127.0f);
        v[0].a = 0x7F;
        v[0].pos = p;
        p.Set(x1, y2, zero, one);
        v[1] = v[0];
        v[1].pos = p;
        p.Set(x2, y1, zero, one);
        v[2] = v[0];
        v[2].pos = p;
        p.Set(x2, y2, zero, one);
        v[3] = v[0];
        v[3].pos = p;
        ((cCtx_2CE910*)GrMan)->top->f4_2 = 1;
        *(short*)((char*)((cCtx_2CE910*)GrMan)->top + 0x10) = -1;
        ((cCtx_2CE910*)GrMan)->v71(4, v, 0);
    }
    *(sVec2_2CE910*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CE910(0.0f, 0.0f);
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem__cColorMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cColorMenuItem_vtbl[];

struct cColorMenuItem__cColorMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

/**
 * Constructs a menu item that edits one channel of a colour.
 *
 * Despite the destructor-style name, this is a constructor: it runs the base constructor with ID -1, stores the
 * colour at +0x14 and the channel at +0x18, then cColorMenuItem's vtable. cARGBMenuItem_cARGBMenuItem() makes
 * four, for channels 0 to 3.
 *
 * @param color  the colour being edited, stored at +0x14
 * @param adjust  channel index 0 to 3, stored at +0x18
 * @return @p self
 */
extern "C" cColorMenuItem__cColorMenuItem_sItem* cColorMenuItem__cColorMenuItem(cColorMenuItem__cColorMenuItem_sItem* self, void* color, void* adjust)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = color;
    self->field_0x18 = adjust;
    self->vtable = cColorMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cColorMenuItem_input);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int cMenuItem_isLocked(void* self);

/**
 * Handles input for a colour-channel item: left and right move the channel between 0 and 1, faster while
 * held.
 *
 * Unless the item is locked or the base navigation result is nonzero, it reads the horizontal input (cInput
 * value 0x3F minus 0x3E, dead zone 0.1) and counts the frames one direction is held (+0x1C, direction +0x20),
 * then adds input * max(1, held^2 / 288) / 2560 to channel +0x18 of the colour at +0x14, clamped to [0, 1],
 * and calls the menu's onChange (slot 4).
 *
 * @return the base navigation result from func_002CA2B0 (tMIR: 0 none, 1 close this menu, 2 close all menus,
 *         3 next item, 4 previous item)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" int cColorMenuItem_input(void* self)
{
    char* menuItem = (char*)self;
    int result = func_002CA2B0(self);
    if (cMenuItem_isLocked(self) != 0) {
        return result;
    }
    if (result != 0) {
        return result;
    }
    float side = func_002CA428_f(self) - func_002CA408_f(self);
    int dir;
    if (side <= -0.1f || side >= 0.1f) {
        dir = 1;
        if (side < 0.0f) {
            dir = -1;
        }
    } else {
        dir = 0;
    }
    if (dir == *(int*)(menuItem + 0x20)) {
        *(int*)(menuItem + 0x1C) += 1;
    } else {
        *(int*)(menuItem + 0x1C) = 0;
        *(int*)(menuItem + 0x20) = dir;
    }
    if (dir != 0) {
        int n = *(int*)(menuItem + 0x1C);
        int accel = (n * n / 0x120) >? 1;
        float* chanPtr = (float*)(*(char**)(menuItem + 0x14) + (*(int*)(menuItem + 0x18) << 2));
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
        char* menu = *(char**)menuItem;
        sVEntry002CD9D8* vt = *(sVEntry002CD9D8**)(menu + 0x12C);
        vt[4].fn(menu + vt[4].delta, self);
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
extern char* D_00487470[];

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
// Returns the rect of a value menu item: sized for the widest of the three option labels plus the formatted value.
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
    rect.w = MAX_2CEEE8(MAX_2CEEE8(textWidth_2CEEE8(item, D_00487470[0]), textWidth_2CEEE8(item, D_00487470[1])),
                     textWidth_2CEEE8(item, D_00487470[2]));
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
extern char* D_00487470[];
extern char str_pct_3f_pct_03d[];

struct sVEntry_2CF1C8 {
    short delta;
    short index;
    void (*fn)(void*, float*, int);
};

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
    virtual void v71(int n, void* verts, int flags);
};


/**
 * Draws a colour-channel item: its channel name, its value and a gradient bar with a marker at the value.
 *
 * Draws the row background through the menu (slot 9, focused or not), the channel name ("Alpha", "Red",
 * "Green", "Blue") on the left, "%.3f  %03d" (value, value * 255.99) right-aligned before the bar, an 80 x 10
 * bar fading from the colour with this channel at 0 to the colour with it at 1, and a triangle above the bar
 * at the value.
 *
 * The bar uses render state 5 instead of 1 when the colour's alpha is not 1. Unlike func_002CA768 it leaves
 * the menu font's ShadowOffset at (2, 2).
 *
 * @param rect  the row rectangle (x, y, w, h)
 */
extern "C" void cColorMenuItem_render(void* self, float* rect)
{
    {
        char* menu = *(char**)self;
        sVEntry_2CF1C8* vt = *(sVEntry_2CF1C8**)(menu + 0x12C);
        char* thisp = menu + vt[9].delta;
        vt[9].fn(thisp, rect, self == cMenu_getFocusItem((cMenu*)menu));
    }
    float barX = rect[0] + rect[2] - 16.0f - 80.0f;
    float textX = barX - 16.0f;
    func_002CA988_sVec3* col = self == cMenu_getFocusItem((cMenu*)(*(void**)self)) ? &D_004D53A0 : &D_004D53B0;
    func_002CA988_sColor textClr;
    textClr.a = 1.0f;
    textClr.r = col->x;
    textClr.g = col->y;
    textClr.b = col->z;
    *(func_002CA988_sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = textClr;
    *(sVec2_2CF1C8*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2_2CF1C8(2.0f, 2.0f);
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f,
                  D_00487470[*(int*)((char*)self + 0x18)]);
    char buf[0x10];
    sprintf(buf, str_pct_3f_pct_03d,
            fptodp(*(float*)(*(char**)((char*)self + 0x14) + (*(int*)((char*)self + 0x18) << 2))),
            (int)(*(float*)(*(char**)((char*)self + 0x14) + (*(int*)((char*)self + 0x18) << 2)) * 255.99000549316406f));
    char* font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
    float w = cFont_textExtent(font, buf, 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
    cFont_text(font, textX - w, rect[1] + 6.0f - 1.0f, buf);

    func_002CA988_sColor rampStart = **(func_002CA988_sColor**)((char*)self + 0x14);
    func_002CA988_sColor rampEnd = **(func_002CA988_sColor**)((char*)self + 0x14);
    *(float*)((char*)&rampStart + (*(int*)((char*)self + 0x18) << 2)) = 0.0f;
    *(float*)((char*)&rampEnd + (*(int*)((char*)self + 0x18) << 2)) = 1.0f;
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
    if ((*(float**)((char*)self + 0x14))[0] != 1.0f) {
        ((cCtx_2CF1C8*)GrMan)->top->f4_2 = 5;
    } else {
        ((cCtx_2CF1C8*)GrMan)->top->f4_2 = 1;
    }
    *(short*)((char*)((cCtx_2CF1C8*)GrMan)->top + 0x10) = -1;
    ((cCtx_2CF1C8*)GrMan)->v71(4, verts, 0);

    float mx = barRect.x + barRect.w * *(float*)(*(char**)((char*)self + 0x14) + (*(int*)((char*)self + 0x18) << 2));
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
    ((cCtx_2CF1C8*)GrMan)->v71(3, verts, 0);
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
    virtual int v09(int a, int b);
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
extern "C" int cExpandMenuItemMenu_onCommand(void* self, int item, int cmdArg)
{
    return (*(cExpandMenuItemMenu_onCommand_cItem**)((char*)self + 0x130))->v09(item, cmdArg);
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
    virtual void v10(int a);
};

/**
 * Reports a value change in an expandable item's inner menu to the menu that holds the expandable item.
 *
 * Calls the owning cExpandMenuItem (+0x130) through its vtable slot 10, whose handler calls the parent menu's
 * onChange with the expandable item itself, so @p item goes no further.
 *
 * @param item  the inner menu item whose value changed
 */
extern "C" void cExpandMenuItemMenu_onChange(void* self, int item)
{
    (*(cExpandMenuItemMenu_onChange_cItem**)((char*)self + 0x130))->v10(item);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CF7E8);
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

extern "C" void* func_002CF7E8(void* self)
{
    char* s = (char*)self;
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(void**)(s + 0x14) = 0;
    *(void***)(s + 0x10) = cExpandMenuItem_vtbl;
    cMenu_cMenu(s + 0x18);
    *(void***)(s + 0x144) = cExpandMenuItemMenu_vtbl;
    new (s + 0x24) sMenuSlot_F7E8[70];
    *(void**)(s + 0x148) = self;
    return self;
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
extern "C" void* cExpandMenuItem_cExpandMenuItem(void* self, void* name, void* parent)
{
    char* menuItem = (char*)self;
    cMenuItem_cMenuItem((cMenuItem*)self, name);
    *(void**)(menuItem + 0x14) = parent;
    *(void***)(menuItem + 0x10) = cExpandMenuItem_vtbl;
    cMenu_cMenu(menuItem + 0x18);
    *(void***)(menuItem + 0x144) = cExpandMenuItemMenu_vtbl;
    new (menuItem + 0x24) sMenuSlot_F860[70];
    *(void**)(menuItem + 0x148) = self;
    if (*(void**)(menuItem + 0x14) != 0) {
        cExpandMenuItem_addItem3(self, *(void**)(menuItem + 0x14), -1);
    }
    return self;
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
    virtual void v08();
};

extern "C" void* cMenu_getFocusItem(cMenu* self);

extern "C" void cExpandMenuItem_zeroValue(void* self)
{
    ((cExpandMenuItem_zeroValue_cItem*)cMenu_getFocusItem((cMenu*)((char*)self + 0x18)))->v08();
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
void cExpandMenuItem_addItem(void* self, void* item)
{
    cMenu_addItem2((char*)self + 0x18, item);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CF928);
#ifdef SKIP_ASM
extern "C" void func_002CBD50(int);

extern "C" void func_002CF928(int arg0) {
    func_002CBD50(arg0 + 0x18);
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002CF948);
#ifdef SKIP_ASM
extern "C" void func_002CF948(void* self)
{
    char* s = (char*)self;
    func_002CBE10(s + 0x18);
    void* item = *(void**)(s + 0x14);
    if (item != 0) {
        cExpandMenuItem_addItem3(self, item, -1);
    }
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002CF988);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void func_002CBD50_pi(void* list, int index) __asm__("func_002CBD50");
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");

extern "C" void func_002CF988(void* self, void* item)
{
    char* s = (char*)self;
    for (int i = 0; i < ((cMenuList*)(s + 0x18))->count; i++) {
        if (((cMenuList*)(s + 0x18))->items[i] == *(void**)(s + 0x14)) {
            func_002CBD50_pi(s + 0x18, i);
            break;
        }
    }
    *(void**)(s + 0x14) = item;
    cExpandMenuItem_addItem3(self, item, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_input);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isLocked(void* self);
extern "C" void cMenu_checkFocus(cMenu* self);
extern "C" void cMenu_nextFocus(cMenu* self);
extern "C" void cMenu_prevFocus(cMenu* self);

class cExpandMenuItem_input_cItem {
public:
    char pad[0x10];
    // vptr at 0x10; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02();
};

struct cExpandMenuItem_input_sOwner {
    char pad[0x18];
    cMenu_setFocus_sMenu menu;
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
extern "C" void* cExpandMenuItem_input(void* expandItem)
{
    cExpandMenuItem_input_sOwner* self = (cExpandMenuItem_input_sOwner*)expandItem;
    cMenu_checkFocus((cMenu*)(&self->menu));
    int prev = self->menu.selected;
    int key = ((cExpandMenuItem_input_cItem*)self->menu.items[prev])->v02();
    if (cMenuItem_isLocked(self) != 0) {
        return (void*)key;
    }
    switch (key) {
    case 3:
        cMenu_nextFocus((cMenu*)(&self->menu));
        if (prev < self->menu.selected) {
            return 0;
        }
        cMenu_setFocus(&self->menu, prev);
        return (void*)3;
    case 4:
        cMenu_prevFocus((cMenu*)(&self->menu));
        if (self->menu.selected < prev) {
            return 0;
        }
        cMenu_setFocus(&self->menu, prev);
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
    virtual int v03(int a, int b);
};

extern "C" int cExpandMenuItem_onCommand(cExpandMenuItem_onCommand_cMenu** self, int a, int b)
{
    return (*self)->v03(a, b);
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
    virtual void v04(void*);
};

extern "C" void cExpandMenuItem_onChange(cExpandMenuItem_onChange_cMenu** self)
{
    (*self)->v04(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_preRender);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isActive(void* self);
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

extern "C" sMenuRect_2CFB58* cExpandMenuItem_preRender(sMenuRect_2CFB58* ret, sMenu_2CFB58* self)
{
    sMenuRect_2CFB58 r;
    float zero = 0.0f;
    r.w = zero;
    r.z = zero;
    r.y = zero;
    r.x = zero;
    self->f13C = *(void**)(self->owner + 0x124);
    for (int i = 0; i < self->count; i++) {
        if (cMenuItem_isActive(self->items[i])) {
            sVEnt_2CFB58* vt = self->items[i]->vt;
            self->rects[i] = vt[4].fn((char*)self->items[i] + vt[4].delta);
            if (self == cMenu_getFocusItem((cMenu*)self->owner) || self->items[i] == self->sel) {
                r.w += self->rects[i].w;
            }
            float h = self->rects[i].z;
            if (self->items[i] != self->sel) {
                h += 20.0f;
            }
            if (r.z < h) {
                r.z = h;
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
    *ret = r;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", cExpandMenuItem_render);
#ifdef SKIP_ASM
extern "C" int cMenuItem_isActive(void* self);
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
    virtual void v05(cExpandMenuItem_render_sRect* r);
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

extern "C" void cExpandMenuItem_render(cExpandMenuItem_render_sOwner* self, cExpandMenuItem_render_sRect* r)
{
    cExpandMenuItem_render_sRect pos = *r;
    for (int i = 0; i < self->count; i++) {
        if (cMenuItem_isActive(self->items[i]) == 0) {
            continue;
        }
        if ((void*)self == cMenu_getFocusItem((cMenu*)self->menu) || self->items[i] == self->cur) {
            int off = self->items[i] == self->cur ? 0 : 20;
            self->rects[i].x = pos.x + (float)off;
            self->rects[i].y = pos.y;
            self->rects[i].w = pos.w - (float)off;
            self->items[i]->v05(&self->rects[i]);
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

extern "C" void cExpandMenuItem_gainFocus(void* self, int mode)
{
    cMenuItem_gainFocus_2(self, mode);
    switch (mode) {
    case 0:
        cMenu_setFocus_v((char*)self + 0x18, 0);
        break;
    case 1:
        cMenu_setFocus_v((char*)self + 0x18, -1);
        break;
    }
    cMenu_onGainFocus((cMenu*)((char*)self + 0x18), *(int*)(*(char**)self + 0x124));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cExpandMenuItem_loseFocus);
#ifdef SKIP_ASM
extern "C" void cMenu_onLoseFocus(cMenu* p);

extern "C" void cExpandMenuItem_loseFocus(void* self)
{
    func_002CA370(self);
    cMenu_onLoseFocus((cMenu*)((char*)self + 0x18));
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_cRGBTitleMenuItem);
#ifdef SKIP_ASM
// PORT: the base ctor's 2nd arg is really an int (id/text handle; -1 = none).
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");

extern void* cRGBTitleMenuItem_vtbl[];

struct cRGBTitleMenuItem_sItem {
    char pad_0x00[0x10];
    void* vtable;    // 0x10
    void* field_0x14;
    void* field_0x18;
};

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
extern "C" cRGBTitleMenuItem_sItem* cRGBTitleMenuItem_cRGBTitleMenuItem(cRGBTitleMenuItem_sItem* self, void* cap, void* color)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    self->field_0x14 = color;
    self->field_0x18 = cap;
    self->vtable = cRGBTitleMenuItem_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002CFF80__FPv);
#ifdef SKIP_ASM
int func_002CFF80(void* self)
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
 * Measures with func_002CA4C8 and returns the tRect by value through @p out.
 *
 * @param out   the hidden result pointer (the tRect to fill)
 * @param item  the colour title item (this)
 * @return @p out
 */
// PORT: `>?` (g++ max operator); returns the rect through the hidden result pointer.
extern "C" cRGBTitleMenuItem_preRender_sRect* cRGBTitleMenuItem_preRender(cRGBTitleMenuItem_preRender_sRect* out, void* item)
{
    cRGBTitleMenuItem_preRender_sRect rect;
    func_002CA4C8(&rect, item, *(int*)((char*)item + 0x18), 0, 0);
    rect.w += 36.0f;
    rect.h = rect.h >? 29.0f;
    *out = rect;
    return out;
}
#endif

//100%
INCLUDE_ASM("util/menu", cRGBTitleMenuItem_render);
#ifdef SKIP_ASM
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the body reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

struct sCol_2D0008 { float a, r, g, b; };

struct sVtx_2D0008 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    float pos[4] __attribute__((aligned(16)));
    sVtx_2D0008() {}
};

struct sVec4_2D0008 {
    float x, y, z, w;
    sVec4_2D0008(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sRect_2D0008 { float x, y, w, h; };

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
    virtual void v71(int prim, void* verts, int n);
};


/**
 * Draws a colour title row: the title on the left and a 20 x 17 swatch of the colour at the right edge.
 *
 * The text goes through func_002CA988 (row background, focused or normal colours, shadow). The swatch is a
 * 4-vertex primitive in the colour's channels (alpha included, scaled to 0-128) drawn through GrMan's slot
 * 71, with render state 5 instead of 1 when the alpha is not 1.
 *
 * @param rect  the row rectangle (x, y, w, h)
 */
// PORT: rect pointer passed through func_002CA988's int parameter
extern "C" void cRGBTitleMenuItem_render(char* self, float* rect)
{
    func_002CA988_5(self, (int)rect, *(int*)(self + 0x18), 0, 0);
    sVtx_2D0008 verts[4];
    sRect_2D0008 rc;
    rc.w = 20.0f;
    rc.h = 17.0f;
    rc.x = rect[0] + rect[2] - 16.0f - rc.w;
    rc.y = rect[1] + 6.0f + 2.0f;
    float x2 = rc.x + rc.w;
    float y2 = rc.y + rc.h;
    sCol_2D0008* colour = *(sCol_2D0008**)(self + 0x14);
    float zeroF = 0.0f;
    float one = 1.0f;
    verts[0].r = (int)(colour->r * 128.0f);
    verts[0].g = (int)(colour->g * 128.0f);
    verts[0].b = (int)(colour->b * 128.0f);
    verts[0].a = (int)(colour->a * 128.0f);
    verts[0].v = zeroF;
    verts[0].u = zeroF;
    verts[0].q = one;
    *(sVec4_2D0008*)verts[0].pos = sVec4_2D0008(rc.x, rc.y, zeroF, one);
    verts[1].r = (int)(colour->r * 128.0f);
    verts[1].g = (int)(colour->g * 128.0f);
    verts[1].b = (int)(colour->b * 128.0f);
    verts[1].a = (int)(colour->a * 128.0f);
    verts[1].v = zeroF;
    verts[1].u = zeroF;
    verts[1].q = one;
    *(sVec4_2D0008*)verts[1].pos = sVec4_2D0008(rc.x, y2, zeroF, one);
    verts[2].r = (int)(colour->r * 128.0f);
    verts[2].g = (int)(colour->g * 128.0f);
    verts[2].b = (int)(colour->b * 128.0f);
    verts[2].a = (int)(colour->a * 128.0f);
    verts[2].v = zeroF;
    verts[2].u = zeroF;
    verts[2].q = one;
    *(sVec4_2D0008*)verts[2].pos = sVec4_2D0008(x2, rc.y, zeroF, one);
    verts[3].r = (int)(colour->r * 128.0f);
    verts[3].g = (int)(colour->g * 128.0f);
    verts[3].b = (int)(colour->b * 128.0f);
    verts[3].a = (int)(colour->a * 128.0f);
    verts[3].v = zeroF;
    verts[3].u = zeroF;
    verts[3].q = one;
    *(sVec4_2D0008*)verts[3].pos = sVec4_2D0008(x2, y2, zeroF, one);
    if (colour->a != 1.0f) {
        ((cCtx_2D0008*)GrMan)->top->f4_2 = 5;
    } else {
        ((cCtx_2D0008*)GrMan)->top->f4_2 = 1;
    }
    *(short*)((char*)((cCtx_2D0008*)GrMan)->top + 0x10) = -1;
    ((cCtx_2D0008*)GrMan)->v71(4, verts, 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", cARGBMenuItem_cARGBMenuItem);
#ifdef SKIP_ASM
extern "C" void* cExpandMenuItem_cExpandMenuItem(void* self, void* name, void* title);
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
extern "C" void* cARGBMenuItem_cARGBMenuItem(void* self, void* name, void* title, void* color)
{
    char* menuItem = (char*)self;
    cExpandMenuItem_cExpandMenuItem(self, name, menuItem + 0x640);
    *(void**)(menuItem + 0x5AC) = color;
    *(void***)(menuItem + 0x10) = cARGBMenuItem_vtbl;
    // PORT: the channel index is passed in the unit's void* parameter
    cColorMenuItem__cColorMenuItem((cColorMenuItem__cColorMenuItem_sItem*)(menuItem + 0x5B0), color, (void*)0);
    cColorMenuItem__cColorMenuItem((cColorMenuItem__cColorMenuItem_sItem*)(menuItem + 0x5D4), color, (void*)1);
    cColorMenuItem__cColorMenuItem((cColorMenuItem__cColorMenuItem_sItem*)(menuItem + 0x5F8), color, (void*)2);
    cColorMenuItem__cColorMenuItem((cColorMenuItem__cColorMenuItem_sItem*)(menuItem + 0x61C), color, (void*)3);
    cRGBTitleMenuItem_cRGBTitleMenuItem((cRGBTitleMenuItem_sItem*)(menuItem + 0x640), title, color);
    cExpandMenuItem_addItem3(self, menuItem + 0x5B0, -1);
    cExpandMenuItem_addItem3(self, menuItem + 0x5D4, -1);
    cExpandMenuItem_addItem3(self, menuItem + 0x5F8, -1);
    cExpandMenuItem_addItem3(self, menuItem + 0x61C, -1);
    return self;
}
#endif

extern "C" void* cExpandMenuItem_input(void* self);

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
    return cExpandMenuItem_input(self);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D03F0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* D_004868B8[];

extern "C" void* func_002D03F0(void* self, int a1, int a2)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x14) = a2;
    *(int*)((char*)self + 0x18) = a1;
    *(void***)((char*)self + 0x10) = D_004868B8;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0448__FPv);
#ifdef SKIP_ASM
int func_002D0448(void* self)
{
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D0450);
#ifdef SKIP_ASM
extern "C" void func_002CA4C8(void*, void*, int, int, void*);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
extern char str_pct_2f_pct_2f_pct_2f[];

extern "C" void* func_002D0450(void* ret, void* item)
{
    char buf[0x80];
    const char* fmt = str_pct_2f_pct_2f_pct_2f;
    int x = fptodp((*(float**)((char*)item + 0x14))[0]);
    int y = fptodp((*(float**)((char*)item + 0x14))[1]);
    int z = fptodp((*(float**)((char*)item + 0x14))[2]);
    sprintf(buf, fmt, x, y, z);
    func_002CA4C8(ret, item, *(int*)((char*)item + 0x18), 0, buf);
    return ret;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D0500);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fptodp(float f);
extern char str_pct_2f_pct_2f_pct_2f[];
// PORT: the unit's 4-arg declaration of func_002CA988 is wrong: the callee reads $8 (5th arg).
void* func_002CA988_5(void*, int, int, int, void*) __asm__("func_002CA988");

extern "C" void func_002D0500(void* self, int a1)
{
    char buf[0x80];
    const char* fmt = str_pct_2f_pct_2f_pct_2f;
    int x = fptodp((*(float**)((char*)self + 0x14))[0]);
    int y = fptodp((*(float**)((char*)self + 0x14))[1]);
    int z = fptodp((*(float**)((char*)self + 0x14))[2]);
    sprintf(buf, fmt, x, y, z);
    func_002CA988_5(self, a1, *(int*)((char*)self + 0x18), 0, buf);
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorMenuItem_cVectorMenuItem);
#ifdef SKIP_ASM
extern "C" void* func_002D03F0(void* self, int a1, int a2);
void cExpandMenuItem_addItem3(void* self, void* item, int index) __asm__("cExpandMenuItem_addItem__FPvT0");
// PORT: the unit's definition types the value pointer and label as int; they are pointers.
void* cFloatMenuItem_ptr(void* self, int index, float* value, void* label, float f1, float f2) __asm__("cFloatMenuItem_cFloatMenuItem");
extern void* cVectorMenuItem_vtbl[];
extern char D_004A39D8[];
extern char D_004A39E0[];
extern char D_004A39E8[];

extern "C" void* cVectorMenuItem_cVectorMenuItem(void* self, void* name, float* values, int a3, float f1, float f2)
{
    char* s = (char*)self;
    cExpandMenuItem_cExpandMenuItem(self, name, s + 0x654);
    *(float**)(s + 0x5AC) = values;
    *(void***)(s + 0x10) = cVectorMenuItem_vtbl;
    cFloatMenuItem_ptr(s + 0x5B8, 0, values, D_004A39D8, f1, f2);
    cFloatMenuItem_ptr(s + 0x5EC, 1, values + 1, D_004A39E0, f1, f2);
    cFloatMenuItem_ptr(s + 0x620, 2, values + 2, D_004A39E8, f1, f2);
    func_002D03F0(s + 0x654, a3, (int)values);
    cExpandMenuItem_addItem3(self, s + 0x5B8, -1);
    cExpandMenuItem_addItem3(self, s + 0x5EC, -1);
    cExpandMenuItem_addItem3(self, s + 0x620, -1);
    *(int*)(s + 0x670) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", cVectorMenuItem_input);
#ifdef SKIP_ASM
extern "C" void* cExpandMenuItem_input(void* self);

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
extern "C" void* cVectorMenuItem_input(sMenuColD6E8* self)
{
    void* ret = cExpandMenuItem_input(self);
    if (self->linked != 0) {
        sMenuVecD6E8 n;
        sMenuVecD6E8 vec(self->ch[0].scale * self->ch[0].get() + self->ch[0].offset,
                       self->ch[1].scale * self->ch[1].get() + self->ch[1].offset,
                       self->ch[2].scale * self->ch[2].get() + self->ch[2].offset);
        menuNormalizeD6E8(n, vec);
        self->ch[0].set(n.x);
        self->ch[1].set(n.y);
        self->ch[2].set(n.z);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0808);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cMenuItem* cMenuItem_cMenuItem_id(cMenuItem* self, int id) __asm__("cMenuItem_cMenuItem__FP9cMenuItemPv");
extern void* D_00486808[];
extern void* D_00486738[];

struct sPair8_D0808 {
    int a;
    int b;
};

extern "C" void* func_002D0808(void* self, int a1, int a2, int a3, int a4, int a5, sPair8_D0808* p6, sPair8_D0808* p7)
{
    cMenuItem_cMenuItem_id((cMenuItem*)self, -1);
    *(int*)((char*)self + 0x18) = a1;
    *(int*)((char*)self + 0x28) = a5;
    *(int*)((char*)self + 0x1C) = a2;
    *(int*)((char*)self + 0x20) = a3;
    *(void***)((char*)self + 0x10) = D_00486808;
    *(int*)((char*)self + 0x3C) = a4;
    *(sPair8_D0808*)((char*)self + 0x2C) = *p6;
    *(sPair8_D0808*)((char*)self + 0x34) = *p7;
    *(void***)((char*)self + 0x14) = D_00486738;
    *(int*)((char*)self + 0x24) = 0;
    *(int*)((char*)self + 0x40) = 0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D08E0);
#ifdef SKIP_ASM
void cMenuItem_gainFocus(void* self);

extern "C" void func_002D08E0(void* self)
{
    cMenuItem_gainFocus(self);
    *(int*)((char*)self + 0x40) = 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0908);
#ifdef SKIP_ASM
extern "C" int func_002CA2B0(void* self);
extern "C" int cMenuItem_isLocked(void* self);
extern "C" int func_002CA3E8(void** self);
float func_002CA408_f(void* self) __asm__("func_002CA408__FPv");
float func_002CA428_f(void* self) __asm__("func_002CA428__FPv");
// PORT: the unit declares func_002CA448/func_002CA468 as void*; these callers use the float result.
float func_002CA448_f(void* self) __asm__("func_002CA448__FPv");
float func_002CA468_f(void* self) __asm__("func_002CA468__FPv");

struct sCurvePt_D0908 {
    float x, y;
};

struct sVE_D0908 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sCurveItem_D0908 {
    char* menu;                 // 0x00
    char pad4[0x14];            // 0x04
    int count;                  // 0x18
    unsigned int lockX;         // 0x1C
    unsigned int lockY;         // 0x20
    int cur;                    // 0x24
    sCurvePt_D0908* pts;        // 0x28
    float minX;                 // 0x2C
    float minY;                 // 0x30
    float maxX;                 // 0x34
    float maxY;                 // 0x38
    int order;                  // 0x3C
    int editing;                // 0x40
};

extern "C" int func_002D0908(sCurveItem_D0908* self)
{
    int r = 0;
    if (cMenuItem_isLocked(self) == 0) {
    int changed = 0;
    if (func_002CA3E8((void**)self) == 6) {
        self->editing ^= 1;
    }
    if (self->editing != 0) {
        if (func_002CA3E8((void**)self) == 8) {
            self->editing = 0;
        } else if (func_002CA3E8((void**)self) == 7) {
            self->editing = 0;
        } else if (self->editing != 0) {
            if (func_002CA3E8((void**)self) == 4) {
                if (self->cur > 0) {
                    self->cur--;
                    changed = 1;
                }
            } else if (func_002CA3E8((void**)self) == 5) {
                if (self->cur < self->count - 1) {
                    self->cur++;
                    changed = 1;
                }
            }
            float step = (self->maxX - self->minX) / (float)(self->count * 2 > 49 ? self->count * 2 : 50);
            float dy = func_002CA448_f(self) - func_002CA468_f(self);
            if (dy != 0.0f && (self->lockY & (1 << self->cur)) == 0) {
                self->pts[self->cur].y += dy * (self->maxY - self->minY) * 0.008333333767950535f;
                changed = 1;
                if (self->maxY < self->pts[self->cur].y) {
                    self->pts[self->cur].y = self->maxY;
                }
                if (self->pts[self->cur].y < self->minY) {
                    self->pts[self->cur].y = self->minY;
                }
                if (self->cur < self->count - 1) {
                    if (self->order > 0 && self->pts[self->cur + 1].y < self->pts[self->cur].y) {
                        self->pts[self->cur].y = self->pts[self->cur + 1].y;
                    } else if (self->order < 0 && self->pts[self->cur].y < self->pts[self->cur + 1].y) {
                        self->pts[self->cur].y = self->pts[self->cur + 1].y;
                    }
                }
                if (self->cur > 0) {
                    if (self->order > 0 && self->pts[self->cur].y < self->pts[self->cur - 1].y) {
                        self->pts[self->cur].y = self->pts[self->cur - 1].y;
                    } else if (self->order < 0 && self->pts[self->cur - 1].y < self->pts[self->cur].y) {
                        self->pts[self->cur].y = self->pts[self->cur - 1].y;
                    }
                }
            }
            float dx = func_002CA428_f(self) - func_002CA408_f(self);
            if (dx != 0.0f && (self->lockX & (1 << self->cur)) == 0) {
                self->pts[self->cur].x += dx * (self->maxX - self->minX) * 0.008333333767950535f;
                changed = 1;
                if (self->maxX < self->pts[self->cur].x) {
                    self->pts[self->cur].x = self->maxX;
                }
                if (self->pts[self->cur].x < self->minX) {
                    self->pts[self->cur].x = self->minX;
                }
                if (self->cur < self->count - 1) {
                    if (self->pts[self->cur + 1].x - step < self->pts[self->cur].x) {
                        self->pts[self->cur].x = self->pts[self->cur + 1].x - step;
                    }
                }
                if (self->cur > 0) {
                    if (self->pts[self->cur].x < self->pts[self->cur - 1].x + step) {
                        self->pts[self->cur].x = self->pts[self->cur - 1].x + step;
                    }
                }
            }
        } else {
            r = func_002CA2B0(self);
        }
    } else {
        r = func_002CA2B0(self);
    }
    if (changed) {
        sVE_D0908* vt = *(sVE_D0908**)(self->menu + 0x12C);
        vt[4].fn(self->menu + vt[4].delta, self);
    }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0D48);
#ifdef SKIP_ASM
extern "C" void* cMenu_getFocusItem(cMenu* self);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);
extern char str_9_999_9_999[];

struct sMenuRect_2D0D48 { float x, y, z, w; };

static inline char* font_2D0D48(char* item)
{
    return *(char**)(*(char**)(*(char**)item + 0x124) + 0x58);
}

extern "C" sMenuRect_2D0D48* func_002D0D48(sMenuRect_2D0D48* ret, char* item)
{
    int margin = 100;
    int focused = 0;
    if (item == cMenu_getFocusItem((cMenu*)(*(void**)item))) {
        focused = *(int*)(item + 0x40) != 0;
    }
    int pad = focused ? 0x50 : 0x14;
    char* font = font_2D0D48(item);
    float lh = (float)*(int*)(font + 0x14) * *(float*)(font + 0x34);
    float w = cFont_textExtent(font, *(char**)(item + 0x14), 0, 0, *(float*)(font + 0x38), *(float*)(font + 0x3C));
    char* font2 = font_2D0D48(item);
    float w2 = cFont_textExtent(font2, str_9_999_9_999, 0, 0, *(float*)(font2 + 0x38), *(float*)(font2 + 0x3C));
    if (w < w2) w = w2;
    sMenuRect_2D0D48 r;
    float zero = 0.0f;
    r.y = zero;
    r.x = zero;
    if (focused) {
        float h = lh + lh + 6.0f;
        float p = (float)pad;
        if (h < p) {
            h = p + 24.0f;
        } else {
            h = h + 24.0f;
        }
        r.w = h;
    } else {
        char* font3 = font_2D0D48(item);
        r.w = (float)*(int*)(font3 + 0x14) * *(float*)(font3 + 0x34) + 12.0f;
    }
    r.z = (float)margin + w + 64.0f;
    *ret = r;
    return ret;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D0EF8);
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
    virtual void v73(int n, void* verts, int flags);
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

extern func_002CA988_sVec3 D_004D53A0;

extern func_002CA988_sVec3 D_004D53B0;

// (prototype from their definition at line 858)
extern "C" void func_002CB350_0EF8(func_002CB350_sVtx* v, float* r, float* t) __asm__("func_002CB350");

// A point of the graph (cGraphMenuItem's point list).
struct sGraphPt_D0EF8 {
    float x, y;
};

// cGraphMenuItem: a menu item that plots a point list in a box at the right of its row.
struct cGraphMenuItem_D0EF8 {
    void* owner;                // 0x00
    char pad04[0x10];
    char* label;                // 0x14
    int count;                  // 0x18
    char pad1C[0x8];
    int cur;                    // 0x24
    sGraphPt_D0EF8* pts;        // 0x28
    float minX;                 // 0x2C
    float maxY;                 // 0x30
    float maxX;                 // 0x34
    float minY;                 // 0x38
    char pad3C[0x4];
    int showCur;                // 0x40
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
extern char D_004A39F0[];
extern sColor D_004D53C0;
extern sColor D_004D53D0;
extern sColor D_004D53E0;
// The focused-row style passed to the menu's frame draw is the line colour of the graph's axis.
extern sColor D_004D5380_c __asm__("D_004D5380");

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

// cGraphMenuItem::Draw (vtable slot 4): label, optional "x, y" readout of the current point, then the graph box
// with its zero line, the polyline and a marker on the current point, each one depth layer higher.
extern "C" void func_002D0EF8(cGraphMenuItem_D0EF8* self, float* rect)
{
    int focused = self == cMenu_getFocusItem((cMenu*)self->owner);
    {
        char* menu = (char*)self->owner;
        sVEntry<void(void*, float*, int)>* vt = *(sVEntry<void(void*, float*, int)>**)(menu + 0x12C);
        vt[9].fn(menu + vt[9].delta, rect, focused);
    }
    char* font = *(char**)(*(char**)(*(char**)self + 0x124) + 0x58);
    float lineH = (float)*(int*)(font + 0x14) * *(float*)(font + 0x34);
    func_002CA988_sVec3* col = self == cMenu_getFocusItem((cMenu*)self->owner) ? &D_004D53A0 : &D_004D53B0;
    sColor c;
    c.a = 1.0f;
    c.r = col->x;
    c.g = col->y;
    c.b = col->z;
    *(sColor*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x40) = c;
    *(sVec2r_D0EF8*)(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58) + 0x28) = sVec2r_D0EF8(2.0f, 2.0f);
    cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 16.0f, rect[1] + 6.0f - 1.0f, self->label);
    if (self->showCur && focused) {
        cBXStr_D0EF8 str = FormatValue_D0EF8(self->pts[self->cur].x) + D_004A39F0 + FormatValue_D0EF8(self->pts[self->cur].y);
        cFont_text(*(char**)(*(char**)(*(char**)self + 0x124) + 0x58), rect[0] + 32.0f, rect[1] + 12.0f - 1.0f + lineH, str.str);
    }
    float h;
    if (focused && self->showCur) {
        h = rect[3] - 24.0f;
    } else {
        h = rect[3] - 12.0f;
    }
    sRect gr;
    gr.w = 100.0f;
    gr.h = h;
    gr.y = rect[1] + (rect[3] - h) * 0.5f;
    gr.x = rect[0] + rect[2] - 16.0f - gr.w;
    int old = GetDepth_D0EF8(((cRenderCtx*)GrMan)->top);
    sVtxF v[4];
    int i;
    for (i = 0; i < 4; i++) {
        v[i].a = (int)(D_004D53C0.a * 128.0f);
        v[i].r = (int)(D_004D53C0.r * 128.0f);
        v[i].g = (int)(D_004D53C0.g * 128.0f);
        v[i].b = (int)(D_004D53C0.b * 128.0f);
        v[i].q = 1.0f;
    }
    ((cRenderCtx*)GrMan)->top->f10 = -1;
    {
        float t[4];
        t[0] = 0.0f;
        t[1] = 0.0f;
        t[2] = 1.0f;
        t[3] = 1.0f;
        func_002CB350_0EF8((func_002CB350_sVtx*)v, &gr.x, t);
    }
    float sx = gr.w / (self->maxX - self->minX);
    float sy = gr.h / (self->maxY - self->minY);
    float ox = gr.x - self->minX * sx;
    float oy = gr.y - self->minY * sy;
    if ((self->maxY >= 0.0f) != (self->minY > 0.0f)) {
        SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 1);
        for (i = 0; i < 2; i++) {
            v[i].a = (int)(D_004D5380_c.a * 128.0f);
            v[i].r = (int)(D_004D5380_c.r * 128.0f);
            v[i].g = (int)(D_004D5380_c.g * 128.0f);
            v[i].b = (int)(D_004D5380_c.b * 128.0f);
        }
        sVec4 p;
        p.x = gr.x;
        p.y = oy;
        p.z = 0.0f;
        p.w = 1.0f;
        *(sVec4*)v[0].pos = p;
        p.x = gr.x + gr.w;
        p.y = oy;
        p.z = 0.0f;
        p.w = 1.0f;
        *(sVec4*)v[1].pos = p;
        ((cRenderCtx*)cGraphicsMan_TheGraphicsMan)->v73(2, v, 0);
    }
    SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 2);
    for (i = 0; i < 2; i++) {
        v[i].a = (int)(D_004D53D0.a * 128.0f);
        v[i].r = (int)(D_004D53D0.r * 128.0f);
        v[i].g = (int)(D_004D53D0.g * 128.0f);
        v[i].b = (int)(D_004D53D0.b * 128.0f);
    }
    for (i = 1; i < self->count; i++) {
        float x0 = GraphX_D0EF8(self->pts[i - 1], sx, ox);
        float y0 = GraphY_D0EF8(self->pts[i - 1], sy, oy);
        float x1 = GraphX_D0EF8(self->pts[i], sx, ox);
        float y1 = GraphY_D0EF8(self->pts[i], sy, oy);
        sVec4 p;
        p.x = x0;
        p.y = y0;
        p.z = 0.0f;
        p.w = 1.0f;
        *(sVec4*)v[0].pos = p;
        p.x = x1;
        p.y = y1;
        p.z = 0.0f;
        p.w = 1.0f;
        *(sVec4*)v[1].pos = p;
        ((cRenderCtx*)cGraphicsMan_TheGraphicsMan)->v73(2, v, 0);
    }
    if (self->showCur) {
        SetDepth_D0EF8(((cRenderCtx*)GrMan)->top, old, 3);
        for (i = 0; i < 4; i++) {
            v[i].a = (int)(D_004D53E0.a * 128.0f);
            v[i].r = (int)(D_004D53E0.r * 128.0f);
            v[i].g = (int)(D_004D53E0.g * 128.0f);
            v[i].b = (int)(D_004D53E0.b * 128.0f);
        }
        float r[4];
        float tt[4];
        float px = GraphX_D0EF8(self->pts[self->cur], sx, ox);
        float py = GraphY_D0EF8(self->pts[self->cur], sy, oy);
        r[0] = px - 2.0f;
        r[1] = py - 2.0f;
        r[2] = 5.0f;
        r[3] = 5.0f;
        tt[0] = 0.0f;
        tt[1] = 0.0f;
        tt[2] = 1.0f;
        tt[3] = 1.0f;
        func_002CB350_0EF8((func_002CB350_sVtx*)v, r, tt);
    }
    // Restore the depth the item started with.
    ((cRenderCtx*)GrMan)->top->f8 = (((cRenderCtx*)GrMan)->top->f8 & ~0x3E0) | ((old << 5) & 0x3E0);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1740);
#ifdef SKIP_ASM
void* cMenu_cMenu(void* self);
extern void* D_004867B0[];

extern "C" void* func_002D1740(void* self)
{
    char* s = (char*)self;
    char* nullItem = s + 0x130;
    char* spaceItem = s + 0x14C;
    char* item3 = s + 0x164;
    cMenu_cMenu(self);
    *(void***)(s + 0x12C) = D_004867B0;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)nullItem, 0);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)spaceItem, (void*)0xA);
    func_002CCDF0((s2CCDF0Item*)item3, 0);
    cMenu_addItem((cMenuList*)self, (void**)nullItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)spaceItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)item3, -1);
    return self;
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002D17F0);
#ifdef SKIP_ASM
void* cMenu_cMenu(void* self);
extern void* D_004867B0[];

extern "C" void* func_002D17F0(void* self, void* nullText, void* text3)
{
    char* s = (char*)self;
    char* nullItem = s + 0x130;
    cMenu_cMenu(self);
    char* item3 = s + 0x164;
    *(void***)(s + 0x12C) = D_004867B0;
    char* spaceItem = s + 0x14C;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)nullItem, nullText);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)spaceItem, (void*)0x1E);
    func_002CCDF0((s2CCDF0Item*)item3, text3);
    cMenu_addItem((cMenuList*)self, (void**)nullItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)spaceItem, -1);
    cMenu_addItem((cMenuList*)self, (void**)item3, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D18B0);
#ifdef SKIP_ASM
extern float D_00445AB0[];

extern "C" float func_002D18B0(float t)
{
    t = t * 160.0f;
    int i = (int)t;
    t = t - (float)i;
    i = i % 160;
    return D_00445AB0[i] * (1.0f - t) + D_00445AB0[i + 1] * t;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/menu", func_002D1928);
#ifdef SKIP_ASM
extern "C" float func_002D18B0(float t);

extern "C" float func_002D1928(int n, float x)
{
    float sum = 0.0f;
    float amp = 1.0f;
    float v = 0.0f;
    float freq = 1.0f;
    int i;
    for (i = 0; i < n; i++) {
        float t = x * freq;
        freq = freq * 2.0f;
        v = func_002D18B0(t) * amp;
        amp = amp * 0.5f;
        sum = sum + v;
    }
    return sum + v;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runCollideScript);
#ifdef SKIP_ASM
extern "C" int cWScriptMan_doOnCollideScript(void* self, int a, int b, int c);
extern void* cWScriptMan_TheWScriptMan;

extern "C" int cObjectInterface_runCollideScript(int a, int b, int c)
{
    return cWScriptMan_doOnCollideScript(cWScriptMan_TheWScriptMan, a, b, c);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D19E8);
#ifdef SKIP_ASM
extern "C" int func_0030A598(void* self, int id);
extern "C" void func_0030A5C0(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

extern "C" int func_002D19E8(int id)
{
    if (func_0030A598(cWScriptMan_TheWScriptMan, id) != 0) {
        func_0030A5C0(cWScriptMan_TheWScriptMan, id);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_runOnLoseFocusScript);
#ifdef SKIP_ASM
extern "C" int func_0030A2E8(void* self, int id);
extern "C" void cWScriptMan_doOnLoseFocusScript(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

/**
 * Runs a world instance's on-lose-focus script, when it has one.
 *
 * Asks the world script manager (TheWScriptMan) whether the instance has an on-lose-focus script
 * (func_0030A2E8) and runs it (cWScriptMan_doOnLoseFocusScript).
 *
 * @param id  the instance record (On Tour tInstanceRecord*)
 * @return 1 when a script was run, otherwise 0
 */
extern "C" int cObjectInterface_runOnLoseFocusScript(int id)
{
    if (func_0030A2E8(cWScriptMan_TheWScriptMan, id) != 0) {
        cWScriptMan_doOnLoseFocusScript(cWScriptMan_TheWScriptMan, id);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1A78);
#ifdef SKIP_ASM
extern "C" void func_0030A610(int, int);

extern "C" void func_002D1A78(int arg0) {
    func_0030A610((*(int*)&cWScriptMan_TheWScriptMan), arg0);
}
#endif
//100%
INCLUDE_ASM("util/menu", func_002D1AA0);
#ifdef SKIP_ASM
extern "C" void func_00309D20(int, int, int);

extern "C" void func_002D1AA0(int arg0, int arg1) {
    func_00309D20((*(int*)&cWScriptMan_TheWScriptMan), arg0, arg1);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1AC8);
#ifdef SKIP_ASM
extern "C" int func_0030A688(void* self, int id);
extern void* cWScriptMan_TheWScriptMan;

extern "C" int func_002D1AC8(int id)
{
    return func_0030A688(cWScriptMan_TheWScriptMan, id);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1AF0);
#ifdef SKIP_ASM

extern "C" void* func_002D1AF0()
{
    return *(void**)((char*)SSXApp->Game->AI + 0x78);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getNumBeings);
#ifdef SKIP_ASM

/**
 * Despite the name, returns a rider's being interface rather than a count: Riders[@p i] + 0x6C0, or 0.
 *
 * Reads cAI::Riders[i] of SSXApp->Game->AI; +0x6C0 is a base-class subobject of cRider whose virtuals the
 * boost nodes call. The count is func_002D1AF0 (cAI::NumberOfRidersAllocated), so On Tour's getNumBeings
 * probably belongs there and this one is a getBeing(i).
 *
 * @param i  rider index, 0 to 5
 * @return that rider's +0x6C0 subobject, or 0 when the slot is empty
 */
extern "C" void* cObjectInterface_getNumBeings(int i)
{
    char* base = (char*)SSXApp->Game->AI;
    char* rider = *(char**)(base + (i << 2) + 0x28);
    return rider ? rider + 0x6C0 : 0;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1B30);
#ifdef SKIP_ASM
class func_002D1B30_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int v07();
};

extern "C" void func_002D1B30(func_002D1B30_cObj* obj)
{
    obj->v07();
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1B58);
#ifdef SKIP_ASM
class func_002D1B58_cObj {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int v05();
};


extern "C" void func_002D1B58(int i)
{
    char* base = (char*)SSXApp->Game->AI;
    char* p = *(char**)(base + (i << 2) + 0x28);
    ((func_002D1B58_cObj*)(p + 0x6C0))->v05();
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1BA0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_00144C98(void* iface);

extern "C" int func_002D1BA0()
{
    return func_00144C98(cBE_getInterface_Fv(cBE_getBE(), 0)) + 1;
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getWorld);
#ifdef SKIP_ASM

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
INCLUDE_ASM("util/menu", func_002D1BF0);
#ifdef SKIP_ASM
extern "C" int func_001032C0(void* self, int a);

extern "C" int func_002D1BF0(int a)
{
    return func_001032C0(*(void**)((char*)SSXApp->Game->AI + 0xA4), a);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1C20);
#ifdef SKIP_ASM

extern "C" void* func_002D1C20(int i)
{
    char* riders = (char*)SSXApp->Game->ViewMan;
    return *(char**)(riders + (i << 2) + 4) + 0x20;
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1C40);
#ifdef SKIP_ASM
extern "C" int func_002D1C40(void) {
    return (*(int *)((char*)(((void*)SSXApp->Game->ViewMan)) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1C58);
#ifdef SKIP_ASM

extern "C" void* func_002D1C58()
{
    return *(void**)((char*)SSXApp->Game->ViewMan + 0x14);
}
#endif

//100%
INCLUDE_ASM("util/menu", cObjectInterface_getUpdateTime);
#ifdef SKIP_ASM

extern "C" float cObjectInterface_getUpdateTime()
{
    return *(float*)((char*)SSXApp + 0x14);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1C80);
#ifdef SKIP_ASM
extern "C" float func_002D1C80(void) {
    return (float) (*(int *)((char*)(SSXApp) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1C98);
#ifdef SKIP_ASM

extern "C" void* func_002D1C98()
{
    return *(void**)((char*)SSXApp->Game->AI + 0x8);
}
#endif

//100%
INCLUDE_ASM("util/menu", func_002D1CB0);
#ifdef SKIP_ASM

extern "C" void* func_002D1CB0()
{
    return SSXApp->Game->FlagManager;
}
#endif

extern "C" void* func_002FC2C0(void* self);

//100%
INCLUDE_ASM("util/menu", func_002D1CC0__FPv);
#ifdef SKIP_ASM
void* func_002D1CC0(void* self)
{
    return func_002FC2C0(self);
}
#endif

