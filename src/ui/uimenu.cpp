#include "common.h"
#include "ui/uiprogress.h"
#include "ui/uipair.h"
#include "ui/uimenu.h"

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_setSelected);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void cUIScreen_jumpToFrame(void* self, unsigned short frame);

struct cUIMenu_sVEntryA {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct cUIMenu_sVEntryB {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Moves the menu's selection to a widget: unfocuses the old one (its "lose" animation event), focuses the new
 * one.
 *
 * Both changes are reported to the screen's state (slot 19: 2 lost, 1 gained). The new item's "gain" event is
 * played, or jumped to when @p play is 0.
 *
 * @param item  the widget to select (On Tour cUIThing* pWidget)
 * @param play  nonzero to play the selection animation, 0 to jump to its end (On Tour createContext)
 */
extern "C" void cUIMenu_setSelected(cUIMenu* self, char* item, int play)
{
    self->mStoredIndex = 0xFF;
    char* cur = (char*)self->mpSelected;
    if (cur != 0 && ((*(int*)(cur + 0x14) >> 4) & 1)) {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 1);
        if (ev != 0xFFFF) {
            void* scr = *(void**)((char*)self->mpSelected + 0x5C);
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        char* c = (char*)self->mpSelected;
        cUIMenu_sVEntryA* va = *(cUIMenu_sVEntryA**)(c + 8);
        va[7].fn(c + va[7].delta, 0);
        char* o = *(char**)((char*)self->mpScreen + 0xD0);
        cUIMenu_sVEntryB* vb = *(cUIMenu_sVEntryB**)(o + 8);
        vb[19].fn(o + vb[19].delta, self->mpSelected, 2);
    }
    self->mpSelected = (cUIThing*)item;
    char* o2 = *(char**)((char*)self->mpScreen + 0xD0);
    cUIMenu_sVEntryB* vb2 = *(cUIMenu_sVEntryB**)(o2 + 8);
    vb2[19].fn(o2 + vb2[19].delta, item, 1);
    char* c2 = (char*)self->mpSelected;
    cUIMenu_sVEntryA* va2 = *(cUIMenu_sVEntryA**)(c2 + 8);
    va2[7].fn(c2 + va2[7].delta, 1);
    unsigned short ev2 = cUIThing_getKeyframerEvent(self->mpSelected, 0);
    if (ev2 != 0xFFFF) {
        void* scr = *(void**)((char*)self->mpSelected + 0x5C);
        if (scr != 0) {
            if (play) {
                cUIScreen_playFrame(scr, ev2, 1);
            } else {
                cUIScreen_jumpToFrame(scr, ev2);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_onInputChangeItem);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelected(cUIMenu* self, char* item, int play);
// PORT: called with an int index here (the unit declares an unsigned char parameter).
void* func_00397870_i_AE98(void* list, int i) __asm__("func_00397870");

struct cUIMenu_onInputChangeItem_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

// getScreen()->getParentState()->vcall slot 20 (On Tour: same slot at +0xA0 of the vtable)
static inline void cUIMenu_onInputChangeItem_notify(char* self, int ev, unsigned char v)
{
    char* mgr = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    cUIMenu_onInputChangeItem_sVEntry* vt = *(cUIMenu_onInputChangeItem_sVEntry**)(mgr + 8);
    vt[20].fn(mgr + vt[20].delta, self, ev, v);
}

/**
 * Keeps a scrolled menu's new current item inside the visible window and selects it, notifying the scroll
 * changes.
 *
 * Scrolling up or down sends menu events 2 or 3 and 4 with the new offset; the change sends event 5.
 *
 * @param selfp  the menu
 * @param OldScrollItemCurrent  the current item before the input
 * @return 1 when the current item changed, otherwise 0
 */
// cUIMenu::onInputChangeItem(unsigned char OldScrollItemCurrent) (On Tour debug 0x0027E39C):
// keeps the selected row (0x95) inside the visible window (top 0x98, rows 0x97), then reselects.
// MenuEntryIndex is a short in On Tour's DWARF; the short is what keeps the second 0x95 load.
extern "C" int cUIMenu_onInputChangeItem(cUIMenu* selfp, unsigned char OldScrollItemCurrent)
{
    if (OldScrollItemCurrent != selfp->mScrollItemCurrent) {
        short MenuEntryIndex = selfp->mScrollItemCurrent - selfp->mScrollItemOffset;
        if (MenuEntryIndex < 0) {
            selfp->mScrollItemOffset = selfp->mScrollItemCurrent;
            MenuEntryIndex = 0;
            cUIMenu_onInputChangeItem_notify(((char*)selfp), 2, selfp->mScrollItemOffset);
            cUIMenu_onInputChangeItem_notify(((char*)selfp), 4, selfp->mScrollItemOffset);
        } else if (MenuEntryIndex >= selfp->mScrollItemNumVisible) {
            selfp->mScrollItemOffset = selfp->mScrollItemCurrent - selfp->mScrollItemNumVisible + 1;
            MenuEntryIndex = selfp->mScrollItemNumVisible - 1;
            cUIMenu_onInputChangeItem_notify(((char*)selfp), 3, selfp->mScrollItemOffset);
            cUIMenu_onInputChangeItem_notify(((char*)selfp), 4, selfp->mScrollItemOffset);
        }
        char* pNextSelected = (char*)func_00397870_i_AE98(((char*)selfp) + 0x74, MenuEntryIndex);
        cUIMenu_setSelected(selfp, pNextSelected, *(int*)&selfp->mAttrMenu & 1);
        cUIMenu_onInputChangeItem_notify(((char*)selfp), 5, selfp->mScrollItemOffset);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uimenu", cUIMenu_onInput);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00397870_i(void*, int) __asm__("func_00397870");
void* func_0039FF50(void* self);
extern "C" int func_003A0318(void* self, int a);
extern char D_004A5A58[4];
extern "C" unsigned char cUIMenu_getNext(void* self);
extern "C" unsigned char cUIMenu_getPrev(void* self);
extern "C" int cUIMenu_onInputChangeItem(cUIMenu* self, unsigned char old);

struct sVE_B000 { short delta; short index; int (*fn)(void*, ...); };
struct sVEv_B000 { short delta; short index; void (*fn)(void*, ...); };

static inline int inputIs_B000(void* in, int slot)
{
    sVE_B000* vt = *(sVE_B000**)((char*)in + 8);
    return vt[slot].fn((char*)in + vt[slot].delta);
}

static inline int inputBtn_B000(void* in, int button)
{
    sVE_B000* vt = *(sVE_B000**)((char*)in + 8);
    return vt[2].fn((char*)in + vt[2].delta, button);
}

static inline void focusNotify_B000(char* self, void* item, int what)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEv_B000* vt = *(sVEv_B000**)(o + 8);
    vt[19].fn(o + vt[19].delta, item, what);
}

static inline int focusMove_B000(char* self, int dir, int idx)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVE_B000* vt = *(sVE_B000**)(o + 8);
    return vt[20].fn(o + vt[20].delta, self, dir, idx);
}

static inline int bit_B000(char* p, int off, int bit)
{
    return (*(int*)(p + off) >> bit) & 1;
}

static inline void playEvent_B000(char* thing, int which)
{
    unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)thing, which);
    if (ev != 0xFFFF) {
        void* scr = *(void**)(thing + 0x5C);
        if (scr != 0) {
            cUIScreen_playFrame(scr, ev, 1);
        }
    }
}

/**
 * Handles input for a menu: the focused item gets it first, then the direction buttons move the selection and
 * the
 * other buttons are reported to the state.
 *
 * Horizontal menus (mAttr bit 0) use left and right. Inputs 4-8 are reported to the item (widget event) and
 * to the
 * state's menu handler, whose result flags pick the item's animation and sound; the UI sound player plays it.
 *
 * @param in  the input (On Tour const cUIInput*)
 * @return 1 when the menu used the input, otherwise 0
 */
// 2D menu input: the focused child gets the input first; then up/down/left/right move the selection
// (by row or by column), the paging slots move the focus between items, accept notifies.
extern "C" int cUIMenu_onInput(cUIMenu* self, void* in)
{
    if (((self->mAttr >> 5) & 1) || (char*)self->mpSelected == 0) {
        return 0;
    }
    char* cur = (char*)self->mpSelected;
    if (!bit_B000(((char*)self), 0x14, 4)) {
        return 0;
    }
    if (bit_B000(cur, 0x14, 3) && !bit_B000(cur, 0x14, 5)) {
        sVE_B000* vt = *(sVE_B000**)(cur + 8);
        if (vt[15].fn(cur + vt[15].delta, in) != 0) {
            return 1;
        }
    }
    int result = 0;
    unsigned char old = self->mScrollItemCurrent;
    int f;
    if ((inputBtn_B000(in, *(signed char*)(((char*)self) + 0x94)) && !(self->mAttr & 1)) ||
        (inputBtn_B000(in, *(signed char*)(((char*)self) + 0x92)) && (self->mAttr & 1))) {
        unsigned short ev;
        if (inputBtn_B000(in, *(signed char*)(((char*)self) + 0x94)) && !(self->mAttr & 1)) {
            ev = cUIThing_getKeyframerEvent((cUIThing*)self, 9);
        } else {
            ev = cUIThing_getKeyframerEvent((cUIThing*)self, 7);
        }
        if (ev != 0xFFFF) {
            void* scr = self->mpScreen;
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        self->mScrollItemCurrent = cUIMenu_getNext(((char*)self));
        int moved = cUIMenu_onInputChangeItem(self, old);
        if (!bit_B000(((char*)self), 0x90, 3)) {
            if (moved) {
                result = 1;
            } else {
                result = func_003A0318(self->mpSelected, 0);
            }
        }
    } else if ((inputBtn_B000(in, *(signed char*)(((char*)self) + 0x93)) && !(self->mAttr & 1)) ||
               (inputBtn_B000(in, *(signed char*)(((char*)self) + 0x91)) && (self->mAttr & 1))) {
        unsigned short ev;
        if (inputBtn_B000(in, *(signed char*)(((char*)self) + 0x93)) && !(self->mAttr & 1)) {
            ev = cUIThing_getKeyframerEvent((cUIThing*)self, 8);
        } else {
            ev = cUIThing_getKeyframerEvent((cUIThing*)self, 6);
        }
        if (ev != 0xFFFF) {
            void* scr = self->mpScreen;
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        self->mScrollItemCurrent = cUIMenu_getPrev(((char*)self));
        int moved = cUIMenu_onInputChangeItem(self, old);
        if (!bit_B000(((char*)self), 0x90, 3)) {
            if (moved) {
                result = 1;
            } else {
                result = func_003A0318(self->mpSelected, 0);
            }
        }
    } else if (inputIs_B000(in, 5)) {
        char* item = (char*)func_00397870_i(((char*)self) + 0x74, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 5);
        f = focusMove_B000(((char*)self), 6, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (f & 0x100) {
            playEvent_B000(item, 2);
        }
        if (f & 1) {
            result = func_003A0318(self->mpSelected, 1);
        } else if (f & 0x10) {
            result = func_003A0318(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 6)) {
        char* item = (char*)func_00397870_i(((char*)self) + 0x74, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 6);
        f = focusMove_B000(((char*)self), 7, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (f & 0x100) {
            playEvent_B000(item, 3);
        }
        if (f & 1) {
            result = func_003A0318(self->mpSelected, 1);
        } else if (f & 0x10) {
            result = func_003A0318(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 8)) {
        char* item = (char*)func_00397870_i(((char*)self) + 0x74, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 8);
        f = focusMove_B000(((char*)self), 9, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (f & 0x100) {
            playEvent_B000(item, 5);
        }
        if (f & 1) {
            result = func_003A0318(self->mpSelected, 1);
        } else if (f & 0x10) {
            result = func_003A0318(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 7)) {
        char* item = (char*)func_00397870_i(((char*)self) + 0x74, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 7);
        f = focusMove_B000(((char*)self), 8, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (f & 0x100) {
            playEvent_B000(item, 4);
        }
        if (f & 1) {
            result = func_003A0318(self->mpSelected, 1);
        } else if (f & 0x10) {
            result = func_003A0318(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 4)) {
        char* item = (char*)func_00397870_i(((char*)self) + 0x74, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 4);
    } else {
        return 0;
    }
    char* owner = (char*)func_0039FF50(((char*)self));
    char* listener = *(char**)(owner + 0x14);
    if (listener == 0) {
        listener = D_004A5A58;
    }
    if (listener != 0) {
        sVEv_B000* vt = *(sVEv_B000**)listener;
        vt[2].fn(listener + vt[2].delta, result);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_render2D);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
extern "C" void func_0039DA20(void* p, unsigned char a, unsigned char b, unsigned char c);

struct cUIMenu_render2D_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

/**
 * Draws the menu's scroll bar and its visible child widgets at the menu's position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIMenu_render2D(cUIMenu* self, float x, float y)
{
    void* p = self->mpScrollBar;
    if (p != 0) {
        func_0039DA20(p, self->mScrollItemOffset, self->mScrollItemNumVisible, self->mScrollItemNum);
    }
    void* n = cList_first((cList*)((char*)self + 0x74));
    if (n != 0) {
        do {
            if ((*(int*)((char*)n + 0x14) >> 6) & 1) {
                cUIMenu_render2D_sVEntry* vt = *(cUIMenu_render2D_sVEntry**)((char*)n + 8);
                vt[0x11].fn((char*)n + vt[0x11].delta, self->mPosition.x + x, self->mPosition.y + y);
            }
            n = *(void**)((char*)n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_setScrollableMenu);
#ifdef SKIP_ASM
extern "C" char cList_size(void*);

/**
 * Makes the menu scroll over @p NumItems items with as many visible rows as it has widgets, starting at the
 * top.
 *
 * @param NumItems  total number of items
 */
extern "C" void cUIMenu_setScrollableMenu(cUIMenu* self, char NumItems)
{
    *(char*)&self->mScrollItemCurrent = 0;
    *(char*)&self->mScrollItemNum = NumItems;
    *(int*)&self->mAttrMenu |= 4;
    *(char*)&self->mScrollItemNumVisible = cList_size((char*)self + 0x74);
    *(char*)&self->mScrollItemOffset = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", func_0039B7A8);
#ifdef SKIP_ASM
extern "C" void func_0039B7A8(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x9C))) = arg1;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_findFirstHighlightable);
#ifdef SKIP_ASM
extern "C" void* func_00397870(void* list, unsigned char i);

struct cUIMenu_findFirstHighlightable_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

/**
 * Finds the first menu item, from a start index, that can be highlighted.
 *
 * With menu attribute bit 2 set the screen's handler (+0xD0, slot 20) decides per index (called with the menu, 1
 * and the index); otherwise an item qualifies when bit 5 of its flags (+0x14) is clear.
 *
 * @param start  the first index to try (On Tour startIndex); 0xFF means 0
 * @return the item's index, or 0xFF when none qualifies
 */
extern "C" unsigned char cUIMenu_findFirstHighlightable(cUIMenu* self, unsigned char start)
{
    unsigned char i = start == 0xFF ? 0 : start;
    for (; i < self->mScrollItemNum; i++) {
        void* item = func_00397870((char*)self + 0x74, i);
        if (*(int*)&self->mAttrMenu & 4) {
            char* mgr = *(char**)((char*)self->mpScreen + 0xD0);
            cUIMenu_findFirstHighlightable_sVEntry* vt = *(cUIMenu_findFirstHighlightable_sVEntry**)(mgr + 8);
            if (vt[20].fn(mgr + vt[20].delta, self, 1, i)) {
                return i;
            }
        } else {
            int f = *(int*)((char*)item + 0x14);
            if (((f >> 5) & 1) == 0) {
                return i;
            }
        }
    }
    return 0xFF;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_reverseFindFirstHighlightable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00397870_i(void*, int) __asm__("func_00397870");

/**
 * Finds the last selectable item at or before @p idx (clamped to mScrollItemNum - 1), searching down to 0.
 *
 * Selectable as in cUIMenu_getPrev: accepted by the state's onWidgetMenu(self, 1, i) for a scrollable menu, else its
 * mStatic bit is clear.
 *
 * @param idx  the index to start from (On Tour startIndex)
 * @return the item index, or 0xFF when none is selectable
 */
extern "C" unsigned char cUIMenu_reverseFindFirstHighlightable(cUIMenu* self, unsigned char idx) {
    short i;
    unsigned char cnt;
    char* obj;
    char* vtbl;
    char* item;

    cnt = self->mScrollItemNum;
    if (idx >= cnt) {
        idx = cnt - 1;
    }
    i = idx;
    for (; i >= 0; i--) {
        item = (char*)func_00397870_i(((char*)self) + 0x74, i);
        if (*(int*)&self->mAttrMenu & 4) {
            obj = *(char**)((char*)self->mpScreen + 0xD0);
            vtbl = *(char**)(obj + 8);
            if ((*(int (**)(void*, void*, int, int))(vtbl + 0xA4))(obj + *(short*)(vtbl + 0xA0), ((char*)self), 1, i)) {
                return i;
            }
        } else if (((*(int*)(item + 0x14) >> 5) & 1) == 0) {
            return i;
        }
    }
    return 0xFF;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_setSelectedByIndex);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelected(cUIMenu* self, char* item, int play);
// PORT: called with an int index here (the unit declares an unsigned char parameter).
void* func_00397870_i(void* list, int i) __asm__("func_00397870");

static inline void cUIMenu_notify(char* self, int ev, unsigned char v)
{
    char* mgr = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    cUIMenu_findFirstHighlightable_sVEntry* vt = *(cUIMenu_findFirstHighlightable_sVEntry**)(mgr + 8);
    vt[20].fn(mgr + vt[20].delta, self, ev, v);
}

/**
 * Selects a menu item by index, scrolling the window to it; before the screen is ready it only stores the
 * index.
 *
 * @param idx  the item index (On Tour Index)
 */
extern "C" void cUIMenu_setSelectedByIndex(cUIMenu* self, unsigned char idx)
{
    if (((self->mAttr >> 4) & 1) == 0 || ((*(int*)(*(char**)((char*)self->mpScreen + 0xD0) + 0x1C) >> 5) & 1) == 0) {
        self->mStoredIndex = idx;
        return;
    }
    unsigned char top = self->mScrollItemOffset;
    unsigned char vis = self->mScrollItemNumVisible;
    self->mScrollItemCurrent = idx;
    if (idx >= top + vis) {
        self->mScrollItemOffset = idx - vis + 1;
        cUIMenu_notify(((char*)self), 3, self->mScrollItemOffset);
        cUIMenu_notify(((char*)self), 4, self->mScrollItemOffset);
    } else if (idx < top) {
        self->mScrollItemOffset = idx;
        cUIMenu_notify(((char*)self), 2, self->mScrollItemOffset);
        cUIMenu_notify(((char*)self), 4, self->mScrollItemOffset);
    }
    cUIMenu_setSelected(self, (char*)func_00397870_i(((char*)self) + 0x74, idx - self->mScrollItemOffset), 1);
}
#endif

extern "C" void* cUIThing_setUIData(void* self);

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_setUIData__FPv);
#ifdef SKIP_ASM
/**
 * Sets the pair's UI data using the base cUIThing_setUIData().
 *
 * @return the result of cUIThing_setUIData()
 */
void* cUIPair_setUIData(void* self)
{
    return cUIThing_setUIData(self);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_setActive);
#ifdef SKIP_ASM
class cUIPair_setActive_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04(int);
};

struct cUIPair_setActive_s {
    char pad0[0x14];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    char pad18[0x78 - 0x18];
    cUIPair_setActive_cVirt* a;
    cUIPair_setActive_cVirt* b;
};

/**
 * Marks a UI pair and both of its child objects as active or not.
 *
 * Sets the pair's mActive bit to @p on != 0, then calls setActive (vtable slot 4) on the first child (at 0x78,
 * not null-checked) and on the second (at 0x7C) when there is one.
 *
 * @param on  nonzero to activate (On Tour: active)
 */
extern "C" void cUIPair_setActive(cUIPair_setActive_s* self, int on)
{
    self->b1 = (on != 0);
    self->a->v04(on);
    cUIPair_setActive_cVirt* b = self->b;
    if (b != 0) {
        b->v04(on);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_setVisible);
#ifdef SKIP_ASM
struct cUIPair_setVisible_sFlags {
    unsigned int pad0 : 6;
    unsigned int on : 1;
};

struct cUIPair_setVisible_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows or hides a widget pair and both of its widgets.
 *
 * @param on  nonzero to show (On Tour visible)
 */
extern "C" void cUIPair_setVisible(cUIPair* self, int on)
{
    ((cUIPair_setVisible_sFlags*)((char*)self + 0x14))->on = (on != 0);
    void* a = self->mpEntry;
    if (a != 0) {
        cUIPair_setVisible_sVEntry* vt = *(cUIPair_setVisible_sVEntry**)((char*)a + 8);
        vt[9].fn((char*)a + vt[9].delta, on);
    }
    void* b = self->mpWidget;
    if (b != 0) {
        cUIPair_setVisible_sVEntry* vt = *(cUIPair_setVisible_sVEntry**)((char*)b + 8);
        vt[9].fn((char*)b + vt[9].delta, on);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_setFocus);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);

struct cUIPair_setFocus_sVEntryA {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct cUIPair_setFocus_sVEntryB {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};
struct cUIPair_setFocus_sFlags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int on : 1;
};

/**
 * Gives or removes focus on a widget pair: the label follows, the second widget plays its gain or lose event.
 *
 * The second widget's focus change is reported to the screen's state (slot 19: 1 gained, 2 lost).
 *
 * @param on  nonzero to focus (On Tour focus)
 */
extern "C" void cUIPair_setFocus(cUIPair* self, int on)
{
    ((cUIPair_setFocus_sFlags*)(((char*)self) + 0x14))->on = on != 0;
    char* a = (char*)self->mpEntry;
    if (a != 0) {
        cUIPair_setFocus_sVEntryA* va = *(cUIPair_setFocus_sVEntryA**)(a + 8);
        va[7].fn(a + va[7].delta, on);
    }
    char* b = (char*)self->mpWidget;
    if (b == 0) {
        return;
    }
    if (on) {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)b, 0);
        if (ev != 0xFFFF) {
            void* scr = *(void**)((char*)self->mpWidget + 0x5C);
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        char* c = (char*)self->mpWidget;
        cUIPair_setFocus_sVEntryA* vc = *(cUIPair_setFocus_sVEntryA**)(c + 8);
        vc[7].fn(c + vc[7].delta, 1);
        char* o = *(char**)((char*)self->mpScreen + 0xD0);
        cUIPair_setFocus_sVEntryB* vo = *(cUIPair_setFocus_sVEntryB**)(o + 8);
        vo[19].fn(o + vo[19].delta, self->mpWidget, 1);
    } else {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)b, 1);
        if (ev != 0xFFFF) {
            void* scr = *(void**)((char*)self->mpWidget + 0x5C);
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        char* c = (char*)self->mpWidget;
        cUIPair_setFocus_sVEntryA* vc = *(cUIPair_setFocus_sVEntryA**)(c + 8);
        vc[7].fn(c + vc[7].delta, 0);
        char* o = *(char**)((char*)self->mpScreen + 0xD0);
        cUIPair_setFocus_sVEntryB* vo = *(cUIPair_setFocus_sVEntryB**)(o + 8);
        vo[19].fn(o + vo[19].delta, self->mpWidget, 2);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_traverseFlat);
#ifdef SKIP_ASM
class cUIPair_traverseFlat_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

/**
 * Applies a list operator to the pair itself, without visiting its two child objects.
 *
 * Returns the operator's onListNode (vtable slot 2 of the operator, whose vtable pointer is its first word) for
 * the pair, the same as cListNode's own traverseFlat.
 *
 * @param self    the pair
 * @param obj  the list operator (On Tour: pOperator)
 * @return the operator's onListNode result
 */
extern "C" int cUIPair_traverseFlat(cUIPair* self, cUIPair_traverseFlat_cVirt* obj)
{
    return obj->v02(self);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_traverseRecurse);
#ifdef SKIP_ASM
class cUIPair_traverseRecurse_cPred {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void*);
};

class cUIPair_traverseRecurse_cNode {
public:
    char pad[0x8];
    // vptr at 0x8
    virtual void v01();
    virtual void v02();
    virtual void* v03(cUIPair_traverseRecurse_cPred*);
};

/**
 * Searches the pair and then its two widgets for the first one the operator accepts.
 *
 * @param pred  the operator (On Tour cListOperator* pOperator)
 * @return the widget found, or 0
 */
extern "C" void* cUIPair_traverseRecurse(cUIPair* self, cUIPair_traverseRecurse_cPred* pred)
{
    if (pred->v02(self)) {
        return ((void*)self);
    }
    cUIPair_traverseRecurse_cNode* child = (cUIPair_traverseRecurse_cNode*)self->mpEntry;
    if (child != 0 && child->v03(pred) != 0) {
        return self->mpEntry;
    }
    cUIPair_traverseRecurse_cNode* next = (cUIPair_traverseRecurse_cNode*)self->mpWidget;
    if (next != 0) {
        return next->v03(pred);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_updateLocale);
#ifdef SKIP_ASM
class cUIPair_updateLocale_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
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
};

/**
 * Refreshes the localized text of both widgets of a pair.
 */
extern "C" void cUIPair_updateLocale(cUIPair* self)
{
    cUIPair_updateLocale_cVirt* a = (cUIPair_updateLocale_cVirt*)self->mpEntry;
    if (a != 0) {
        a->v16();
    }
    cUIPair_updateLocale_cVirt* b = (cUIPair_updateLocale_cVirt*)self->mpWidget;
    if (b != 0) {
        b->v16();
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_destroyPair);
#ifdef SKIP_ASM
class cUIPair_destroyPair_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01(int);
};

/**
 * Deletes both widgets of a pair and clears the pointers.
 */
extern "C" void cUIPair_destroyPair(cUIPair* self)
{
    cUIPair_destroyPair_cVirt* a = (cUIPair_destroyPair_cVirt*)self->mpEntry;
    if (a != 0) {
        a->v01(3);
        self->mpEntry = 0;
    }
    cUIPair_destroyPair_cVirt* b = (cUIPair_destroyPair_cVirt*)self->mpWidget;
    if (b != 0) {
        b->v01(3);
        self->mpWidget = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_onInput);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void* func_0039FF50(void* self);
extern "C" int func_003A0318(void* self, int a);
extern char D_004A5A58[4];

struct sVE_BED8 { short delta; short index; int (*fn)(void*, ...); };
struct sVEv_BED8 { short delta; short index; void (*fn)(void*, ...); };

static inline int inputIs_BED8(void* in, int slot)
{
    sVE_BED8* vt = *(sVE_BED8**)((char*)in + 8);
    return vt[slot].fn((char*)in + vt[slot].delta);
}

static inline void focusNotify_BED8(char* self, int what)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEv_BED8* vt = *(sVEv_BED8**)(o + 8);
    vt[19].fn(o + vt[19].delta, self, what);
}

static inline int focusMove_BED8(char* self, int dir)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVE_BED8* vt = *(sVE_BED8**)(o + 8);
    return vt[21].fn(o + vt[21].delta, self, dir);
}

/**
 * Handles input for a widget pair: inputs 5 and 6 are reported to the state, input 4 notified, others go to
 * the second widget.
 *
 * @param in  the input (On Tour const cUIInput* pInput)
 * @return 1 when the pair used the input, otherwise the second widget's answer (0 without one)
 */
// Menu input: up/down move the selection (with transition frames and wrap events), accept notifies, else
// the sub-handler gets the input.
extern "C" int cUIPair_onInput(cUIPair* self, void* in)
{
    int result = 0;
    char* listener = D_004A5A58;
    char* owner = (char*)func_0039FF50(((char*)self));
    if (*(char**)(owner + 0x14) != 0) {
        listener = *(char**)(owner + 0x14);
    }
    int f;
    if (inputIs_BED8(in, 5)) {
        focusNotify_BED8(((char*)self), 5);
        f = focusMove_BED8(((char*)self), 0);
        if (f & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 2);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
        }
        if (f & 1) {
            result = func_003A0318(((char*)self), 1);
        } else if (f & 0x10) {
            result = func_003A0318(((char*)self), 0);
        }
        if (listener != 0) {
            sVEv_BED8* vt = *(sVEv_BED8**)listener;
            vt[2].fn(listener + vt[2].delta, result);
        }
    } else if (inputIs_BED8(in, 6)) {
        focusNotify_BED8(((char*)self), 6);
        f = focusMove_BED8(((char*)self), 1);
        if (f & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 3);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
        }
        if (f & 1) {
            result = func_003A0318(((char*)self), 1);
        } else if (f & 0x10) {
            result = func_003A0318(((char*)self), 0);
        }
        if (listener != 0) {
            sVEv_BED8* vt = *(sVEv_BED8**)listener;
            vt[2].fn(listener + vt[2].delta, result);
        }
    } else if (inputIs_BED8(in, 4)) {
        focusNotify_BED8(((char*)self), 4);
        return 1;
    } else {
        char* sub = (char*)self->mpWidget;
        if (sub != 0) {
            sVE_BED8* vt = *(sVE_BED8**)(sub + 8);
            return vt[15].fn(sub + vt[15].delta, in);
        }
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_render2D);
#ifdef SKIP_ASM
struct cUIPair_render2D_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

/**
 * Draws both widgets of a pair at the pair's position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIPair_render2D(cUIPair* self, float x, float y)
{
    void* a = self->mpEntry;
    if (a != 0) {
        cUIPair_render2D_sVEntry* e = &(*(cUIPair_render2D_sVEntry**)((char*)a + 8))[17];
        e->fn((char*)a + e->delta, x + self->mPosition.x, y + self->mPosition.y);
    }
    void* b = self->mpWidget;
    if (b != 0) {
        cUIPair_render2D_sVEntry* e = &(*(cUIPair_render2D_sVEntry**)((char*)b + 8))[17];
        e->fn((char*)b + e->delta, x + self->mPosition.x, y + self->mPosition.y);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_update);
#ifdef SKIP_ASM
extern "C" void cUIThing_update(void* self, unsigned short ev);

class cUIPair_update_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
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
    virtual void v19(unsigned short);
};

/**
 * Updates the pair's animation for a frame, then both of its widgets.
 *
 * @param ev  the frame number (On Tour FrameNum)
 */
extern "C" void cUIPair_update(cUIPair* self, unsigned short ev)
{
    cUIThing_update(self, ev);
    cUIPair_update_cVirt* a = (cUIPair_update_cVirt*)self->mpEntry;
    if (a != 0) {
        a->v19(ev);
    }
    cUIPair_update_cVirt* b = (cUIPair_update_cVirt*)self->mpWidget;
    if (b != 0) {
        b->v19(ev);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress__cUIProgress);
#ifdef SKIP_ASM
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern void* cUIProgress_vtbl[];

struct cUIProgress__cUIProgress_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Destroys a progress bar: deletes its two child widgets, then the cUIThing base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIProgress__cUIProgress(cUIProgress* self, int flags)
{
    self->mVtbl = cUIProgress_vtbl;
    void* a = self->mpObjBackground;
    if (a != 0) {
        cUIProgress__cUIProgress_sVEntry* vt = *(cUIProgress__cUIProgress_sVEntry**)((char*)a + 8);
        vt[1].fn((char*)a + vt[1].delta, 3);
    }
    void* b = self->mpObjKnob;
    if (b != 0) {
        cUIProgress__cUIProgress_sVEntry* vt = *(cUIProgress__cUIProgress_sVEntry**)((char*)b + 8);
        vt[1].fn((char*)b + vt[1].delta, 3);
    }
    cUIThing__cUIThing(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress_setUIData);
#ifdef SKIP_ASM
/**
 * Sets up a progress bar from its UI data, then sets its default range.
 *
 * Runs the base cUIThing_setUIData(), then stores 10 at 0x80 (the full-scale value that the bar setter
 * cUIProgress_setValue() divides by) and 100 at 0x84. Only @p self is declared: the UI data pointer reaches
 * cUIThing_setUIData() unchanged in its register.
 */
extern "C" void cUIProgress_setUIData(cUIProgress* self)
{
    cUIThing_setUIData(self);
    self->mNumValues = 10;
    self->mBackWidth = 100;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress_render2D);
#ifdef SKIP_ASM
struct cUIProgress_render2D_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

/**
 * Draws a progress bar's two widgets (when both exist) at its position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIProgress_render2D(cUIProgress* self, float x, float y)
{
    void* a = self->mpObjBackground;
    if (a != 0 && self->mpObjKnob != 0) {
        float px = x + self->mPosition.x;
        float py = y + self->mPosition.y;
        cUIProgress_render2D_sVEntry* vt = *(cUIProgress_render2D_sVEntry**)((char*)a + 8);
        vt[17].fn((char*)a + vt[17].delta, px, py);
        void* b = self->mpObjKnob;
        cUIProgress_render2D_sVEntry* vt2 = *(cUIProgress_render2D_sVEntry**)((char*)b + 8);
        vt2[17].fn((char*)b + vt2[17].delta, px, py);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress_update);
#ifdef SKIP_ASM
extern "C" void cUIAnimation_updateThing(void* anim, void* thing, unsigned char flags, unsigned short ev);
extern "C" void cUIAnimation_updateFrame(void* anim, unsigned char flags);

struct cUIProgress_update_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, unsigned short);
};

/**
 * Updates the progress bar's own animation for a frame, then its two widgets.
 *
 * @param ev  the frame number (On Tour FrameNum)
 */
extern "C" void cUIProgress_update(cUIProgress* self, unsigned short ev)
{
    if (self->mpAnim != 0) {
        cUIAnimation_updateThing(self->mpAnim, self, self->mAnimPlaybackStatus, ev);
        cUIAnimation_updateFrame(self->mpAnim, self->mAnimPlaybackStatus);
    }
    void* a = self->mpObjKnob;
    if (a != 0) {
        cUIProgress_update_sVEntry* vt = *(cUIProgress_update_sVEntry**)((char*)a + 8);
        vt[19].fn((char*)a + vt[19].delta, ev);
    }
    void* b = self->mpObjBackground;
    if (b != 0) {
        cUIProgress_update_sVEntry* vt = *(cUIProgress_update_sVEntry**)((char*)b + 8);
        vt[19].fn((char*)b + vt[19].delta, ev);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress_setValue);
#ifdef SKIP_ASM
struct cUIProgress_setValue_sVec3 {
    float x;
    float y;
    float z;
};
extern cUIProgress_setValue_sVec3 t3vect_Zero;

class cUIProgress_setValue_cWidget {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
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
    virtual void v20(cUIProgress_setValue_sVec3*);
    virtual void v21(cUIProgress_setValue_sVec3*);
};

extern "C" void cUIProgress_setValue(cUIProgress* self, int pos)
{
    self->mValue = pos;
    cUIProgress_setValue_cWidget* a = (cUIProgress_setValue_cWidget*)self->mpObjBackground;
    cUIProgress_setValue_sVec3 v = t3vect_Zero;
    if (a != 0) {
        a->v20(&v);
    }
    if (self->mpObjKnob != 0) {
        int n = self->mNumValues;
        if (n != 0) {
            if (!(self->mAttr & 1)) {
                v.x *= (float)self->mValue / (float)n;
            } else {
                v.y *= (float)self->mValue / (float)n;
            }
            ((cUIProgress_setValue_cWidget*)self->mpObjKnob)->v21(&v);
        }
    }
}
#endif

