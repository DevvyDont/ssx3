#include "vtable.h"
#include "common.h"
#include "ui/uiengine.h"
#include "ui/uistate.h"
#include "ui/uiscreen.h"
#include "ui/uiinput.h"
#include "ui/uithing.h"
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
void cUIMenu::setSelected(cUIThing* item, int play)
{
    mStoredIndex = 0xFF;
    cUIThing* cur = mpSelected;
    if (((char*)cur) != 0 && ((cur->mAttr >> 4) & 1)) {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 1);
        if (ev != 0xFFFF) {
            void* scr = mpSelected->mpScreen;
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        cUIThing* oldSelected = mpSelected;
        oldSelected->setFocus(0);
        char* screenState = (char*)mpScreen->mpState;
        cUIMenu_sVEntryB* vb = *(cUIMenu_sVEntryB**)(screenState + 8);
        vb[19].fn(vthis(screenState, vb[19]), mpSelected, 2);
    }
    mpSelected = (cUIThing*)item;
    char* selState = (char*)mpScreen->mpState;
    cUIMenu_sVEntryB* vb2 = *(cUIMenu_sVEntryB**)(selState + 8);
    vb2[19].fn(vthis(selState, vb2[19]), ((char*)item), 1);
    cUIThing* newSelected = mpSelected;
    newSelected->setFocus(1);
    unsigned short ev2 = cUIThing_getKeyframerEvent(mpSelected, 0);
    if (ev2 != 0xFFFF) {
        void* scr = mpSelected->mpScreen;
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
extern "C" void cUIMenu_setSelected(cUIMenu* self, cUIThing* item, int play);
// PORT: called with an int index here (the unit declares an unsigned char parameter).
void* cList_getByIndex_i_AE98(void* list, int i) __asm__("cList_getByIndex");

struct cUIMenu_onInputChangeItem_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

// getScreen()->getParentState()->vcall slot 20 (On Tour: same slot at +0xA0 of the vtable)
static inline void cUIMenu_onInputChangeItem_notify(char* vself, int ev, unsigned char v)
{
    cUIMenu* self = (cUIMenu*)vself;
    char* mgr = (char*)self->mpScreen->mpState;
    cUIMenu_onInputChangeItem_sVEntry* vt = *(cUIMenu_onInputChangeItem_sVEntry**)(mgr + 8);
    vt[20].fn(vthis(mgr, vt[20]), ((char*)self), ev, v);
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
int cUIMenu::onInputChangeItem(unsigned char OldScrollItemCurrent)
{
    if (OldScrollItemCurrent != mScrollItemCurrent) {
        short MenuEntryIndex = mScrollItemCurrent - mScrollItemOffset;
        if (MenuEntryIndex < 0) {
            mScrollItemOffset = mScrollItemCurrent;
            MenuEntryIndex = 0;
            cUIMenu_onInputChangeItem_notify(((char*)this), 2, mScrollItemOffset);
            cUIMenu_onInputChangeItem_notify(((char*)this), 4, mScrollItemOffset);
        } else if (MenuEntryIndex >= mScrollItemNumVisible) {
            mScrollItemOffset = mScrollItemCurrent - mScrollItemNumVisible + 1;
            MenuEntryIndex = mScrollItemNumVisible - 1;
            cUIMenu_onInputChangeItem_notify(((char*)this), 3, mScrollItemOffset);
            cUIMenu_onInputChangeItem_notify(((char*)this), 4, mScrollItemOffset);
        }
        char* pNextSelected = (char*)cList_getByIndex_i_AE98(&mList, MenuEntryIndex);
        setSelected((cUIThing*)pNextSelected, *(int*)&mAttrMenu & 1);
        cUIMenu_onInputChangeItem_notify(((char*)this), 5, mScrollItemOffset);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uimenu", cUIMenu_onInput);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cList_getByIndex_i(void*, int) __asm__("cList_getByIndex");
void* cUIThing_getEngine(void* self);
extern "C" int cUIThing_getSound(void* self, int a);
extern char gUISoundNull[4];
extern "C" unsigned char cUIMenu_getNext(void* self);
extern "C" unsigned char cUIMenu_getPrev(void* self);
extern "C" int cUIMenu_onInputChangeItem(cUIMenu* self, unsigned char old);


static inline int inputIs_B000(void* in, int slot)
{
    sVEntry<int(void*, ...)>* vt = *(sVEntry<int(void*, ...)>**)((char*)in + 8);
    return vt[slot].fn(vthis(in, vt[slot]));
}

static inline int inputBtn_B000(void* in, int button)
{
    sVEntry<int(void*, ...)>* vt = *(sVEntry<int(void*, ...)>**)((char*)in + 8);
    return vt[2].fn(vthis(in, vt[2]), button);
}

static inline void focusNotify_B000(char* self, void* item, int what)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(o + 8);
    vt[19].fn(vthis(o, vt[19]), item, what);
}

static inline int focusMove_B000(char* self, int dir, int idx)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEntry<int(void*, ...)>* vt = *(sVEntry<int(void*, ...)>**)(o + 8);
    return vt[20].fn(vthis(o, vt[20]), self, dir, idx);
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
extern "C" int cUIMenu_onInput(cUIMenu* self, cUIInput* in)
{
    if (((self->mAttr >> 5) & 1) || (char*)self->mpSelected == 0) {
        return 0;
    }
    cUIThing* cur = self->mpSelected;
    if (!bit_B000(((char*)self), 20, 4)) {
        return 0;
    }
    if (bit_B000(((char*)cur), 20, 3) && !bit_B000(((char*)cur), 20, 5)) {
        if (cur->onInput(in) != 0) {
            return 1;
        }
    }
    int result = 0;
    unsigned char old = self->mScrollItemCurrent;
    int moveResult;
    if ((inputBtn_B000(in, self->mInputMap[3]) && !(self->mAttr & 1)) ||
        (inputBtn_B000(in, self->mInputMap[1]) && (self->mAttr & 1))) {
        unsigned short ev;
        if (inputBtn_B000(in, self->mInputMap[3]) && !(self->mAttr & 1)) {
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
        int moved = self->onInputChangeItem(old);
        if (!bit_B000(((char*)self), 144, 3)) {
            if (moved) {
                result = 1;
            } else {
                result = cUIThing_getSound(self->mpSelected, 0);
            }
        }
    } else if ((inputBtn_B000(in, self->mInputMap[2]) && !(self->mAttr & 1)) ||
               (inputBtn_B000(in, self->mInputMap[0]) && (self->mAttr & 1))) {
        unsigned short ev;
        if (inputBtn_B000(in, self->mInputMap[2]) && !(self->mAttr & 1)) {
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
        int moved = self->onInputChangeItem(old);
        if (!bit_B000(((char*)self), 144, 3)) {
            if (moved) {
                result = 1;
            } else {
                result = cUIThing_getSound(self->mpSelected, 0);
            }
        }
    } else if (inputIs_B000(in, 5)) {
        char* item = (char*)cList_getByIndex_i(&self->mList, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 5);
        moveResult = focusMove_B000(((char*)self), 6, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (moveResult & 0x100) {
            playEvent_B000(item, 2);
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(self->mpSelected, 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 6)) {
        char* item = (char*)cList_getByIndex_i(&self->mList, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 6);
        moveResult = focusMove_B000(((char*)self), 7, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (moveResult & 0x100) {
            playEvent_B000(item, 3);
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(self->mpSelected, 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 8)) {
        char* item = (char*)cList_getByIndex_i(&self->mList, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 8);
        moveResult = focusMove_B000(((char*)self), 9, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (moveResult & 0x100) {
            playEvent_B000(item, 5);
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(self->mpSelected, 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 7)) {
        char* item = (char*)cList_getByIndex_i(&self->mList, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 7);
        moveResult = focusMove_B000(((char*)self), 8, self->mScrollItemCurrent - self->mScrollItemOffset);
        if (moveResult & 0x100) {
            playEvent_B000(item, 4);
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(self->mpSelected, 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(self->mpSelected, 0);
        }
    } else if (inputIs_B000(in, 4)) {
        char* item = (char*)cList_getByIndex_i(&self->mList, self->mScrollItemCurrent - self->mScrollItemOffset);
        focusNotify_B000(((char*)self), item, 4);
    } else {
        return 0;
    }
    cUIEngine* owner = (cUIEngine*)((char*)cUIThing_getEngine(((char*)self)));
    char* listener = (char*)owner->mpSound;
    if (listener == 0) {
        listener = gUISoundNull;
    }
    if (listener != 0) {
        sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)listener;
        vt[2].fn(vthis(listener, vt[2]), result);
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
extern "C" void cUIScrollBar_setValues(void* p, unsigned char a, unsigned char b, unsigned char c);

/**
 * Draws the menu's scroll bar and its visible child widgets at the menu's position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIMenu_render2D(cUIMenu* self, float x, float y)
{
    void* scrollBar = self->mpScrollBar;
    if (scrollBar != 0) {
        cUIScrollBar_setValues(scrollBar, self->mScrollItemOffset, self->mScrollItemNumVisible, self->mScrollItemNum);
    }
    cUIThing* n = (cUIThing*)cList_first((cList*)&self->mList);
    if (((void*)n) != 0) {
        do {
            if ((n->mAttr >> 6) & 1) {
                n->render2D(self->mPosition.x + x, self->mPosition.y + y);
            }
            n = (cUIThing*)((void*)n->mpNext);
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
void cUIMenu::setScrollableMenu(char NumItems)
{
    *(char*)&mScrollItemCurrent = 0;
    *(char*)&mScrollItemNum = NumItems;
    *(int*)&mAttrMenu |= 4;
    *(char*)&mScrollItemNumVisible = cList_size((char*)&mList);
    *(char*)&mScrollItemOffset = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_setScrollBar);
#ifdef SKIP_ASM
/**
 * Sets a menu's scroll bar (+0x9C).
 *
 * Nothing in the retail code references it.
 *
 * @param scrollBar  the scroll bar widget
 */
void cUIMenu::setScrollBar(int scrollBar)
{
    mpScrollBar = (cUIScrollBar*)scrollBar;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIMenu_findFirstHighlightable);
#ifdef SKIP_ASM
extern "C" void* cList_getByIndex(void* list, unsigned char i);

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
unsigned char cUIMenu::findFirstHighlightable(unsigned char start)
{
    unsigned char i = start == 0xFF ? 0 : start;
    for (; i < mScrollItemNum; i++) {
        void* item = cList_getByIndex((char*)&mList, i);
        if (*(int*)&mAttrMenu & 4) {
            cUIState* mgr = mpScreen->mpState;
            if (mgr->onWidgetMenu(this, 1, i)) {
                return i;
            }
        } else {
            int itemAttr = *(int*)((char*)item + 0x14);
            if (((itemAttr >> 5) & 1) == 0) {
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
extern "C" void* cList_getByIndex_i(void*, int) __asm__("cList_getByIndex");

/**
 * Finds the last selectable item at or before @p idx (clamped to mScrollItemNum - 1), searching down to 0.
 *
 * Selectable as in cUIMenu_getPrev: accepted by the state's onWidgetMenu(self, 1, i) for a scrollable menu, else its
 * mStatic bit is clear.
 *
 * @param idx  the index to start from (On Tour startIndex)
 * @return the item index, or 0xFF when none is selectable
 */
unsigned char cUIMenu::reverseFindFirstHighlightable(unsigned char idx)
{
    short i;
    unsigned char cnt;
    char* obj;
    char* vtbl;
    char* item;

    cnt = mScrollItemNum;
    if (idx >= cnt) {
        idx = cnt - 1;
    }
    i = idx;
    for (; i >= 0; i--) {
        item = (char*)cList_getByIndex_i(&mList, i);
        if (*(int*)&mAttrMenu & 4) {
            obj = (char*)mpScreen->mpState;
            vtbl = *(char**)(obj + 8);
            if ((*(int (**)(void*, void*, int, int))(vtbl + 0xA4))(obj + *(short*)(vtbl + 0xA0), ((char*)this), 1, i)) {
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
extern "C" void cUIMenu_setSelected(cUIMenu* self, cUIThing* item, int play);
// PORT: called with an int index here (the unit declares an unsigned char parameter).
void* cList_getByIndex_i(void* list, int i) __asm__("cList_getByIndex");

static inline void cUIMenu_notify(char* vself, int ev, unsigned char v)
{
    cUIMenu* self = (cUIMenu*)vself;
    char* mgr = (char*)self->mpScreen->mpState;
    cUIMenu_findFirstHighlightable_sVEntry* vt = *(cUIMenu_findFirstHighlightable_sVEntry**)(mgr + 8);
    vt[20].fn(vthis(mgr, vt[20]), ((char*)self), ev, v);
}

/**
 * Selects a menu item by index, scrolling the window to it; before the screen is ready it only stores the
 * index.
 *
 * @param idx  the item index (On Tour Index)
 */
void cUIMenu::setSelectedByIndex(unsigned char idx)
{
    if (((mAttr >> 4) & 1) == 0 || ((mpScreen->mpState->mAttrState >> 5) & 1) == 0) {
        mStoredIndex = idx;
        return;
    }
    unsigned char top = mScrollItemOffset;
    unsigned char vis = mScrollItemNumVisible;
    mScrollItemCurrent = idx;
    if (idx >= top + vis) {
        mScrollItemOffset = idx - vis + 1;
        cUIMenu_notify(((char*)this), 3, mScrollItemOffset);
        cUIMenu_notify(((char*)this), 4, mScrollItemOffset);
    } else if (idx < top) {
        mScrollItemOffset = idx;
        cUIMenu_notify(((char*)this), 2, mScrollItemOffset);
        cUIMenu_notify(((char*)this), 4, mScrollItemOffset);
    }
    setSelected((cUIThing*)cList_getByIndex_i(&mList, idx - mScrollItemOffset), 1);
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
    virtual void setActive(int);
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
    self->a->setActive(on);
    cUIPair_setActive_cVirt* widget = self->b;
    if (widget != 0) {
        widget->setActive(on);
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

/**
 * Shows or hides a widget pair and both of its widgets.
 *
 * @param on  nonzero to show (On Tour visible)
 */
extern "C" void cUIPair_setVisible(cUIPair* self, int on)
{
    ((cUIPair_setVisible_sFlags*)&self->mAttr)->on = (on != 0);
    cUIThing* entry = self->mpEntry;
    if (entry != 0) {
        entry->setVisible(on);
    }
    cUIThing* widget = self->mpWidget;
    if (widget != 0) {
        widget->setVisible(on);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_setFocus);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);

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
    ((cUIPair_setFocus_sFlags*)(&self->mAttr))->on = on != 0;
    cUIThing* entry = self->mpEntry;
    if (((char*)entry) != 0) {
        entry->setFocus(on);
    }
    char* widget = (char*)self->mpWidget;
    if (widget == 0) {
        return;
    }
    if (on) {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)widget, 0);
        if (ev != 0xFFFF) {
            void* scr = self->mpWidget->mpScreen;
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        cUIThing* widgetObj = self->mpWidget;
        widgetObj->setFocus(1);
        cUIState* screenState = self->mpScreen->mpState;
        screenState->onWidgetEvent(self->mpWidget, 1);
    } else {
        unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)widget, 1);
        if (ev != 0xFFFF) {
            void* scr = self->mpWidget->mpScreen;
            if (scr != 0) {
                cUIScreen_playFrame(scr, ev, 1);
            }
        }
        cUIThing* widgetObj = self->mpWidget;
        widgetObj->setFocus(0);
        cUIState* screenState = self->mpScreen->mpState;
        screenState->onWidgetEvent(self->mpWidget, 2);
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
    virtual int onListNode(void*);
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
    return obj->onListNode(self);
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_traverseRecurse);
#ifdef SKIP_ASM
class cUIPair_traverseRecurse_cPred {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int onListNode(void*);
};

class cUIPair_traverseRecurse_cNode {
public:
    char pad[0x8];
    // vptr at 0x8
    virtual void v01();
    virtual void v02();
    virtual void* traverseRecurse(cUIPair_traverseRecurse_cPred*);
};

/**
 * Searches the pair and then its two widgets for the first one the operator accepts.
 *
 * @param pred  the operator (On Tour cListOperator* pOperator)
 * @return the widget found, or 0
 */
extern "C" void* cUIPair_traverseRecurse(cUIPair* self, cUIPair_traverseRecurse_cPred* pred)
{
    if (pred->onListNode(self)) {
        return ((void*)self);
    }
    cUIPair_traverseRecurse_cNode* child = (cUIPair_traverseRecurse_cNode*)self->mpEntry;
    if (child != 0 && child->traverseRecurse(pred) != 0) {
        return self->mpEntry;
    }
    cUIPair_traverseRecurse_cNode* next = (cUIPair_traverseRecurse_cNode*)self->mpWidget;
    if (next != 0) {
        return next->traverseRecurse(pred);
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
    virtual void updateLocale();
};

/**
 * Refreshes the localized text of both widgets of a pair.
 */
extern "C" void cUIPair_updateLocale(cUIPair* self)
{
    cUIPair_updateLocale_cVirt* entry = (cUIPair_updateLocale_cVirt*)self->mpEntry;
    if (entry != 0) {
        entry->updateLocale();
    }
    cUIPair_updateLocale_cVirt* widget = (cUIPair_updateLocale_cVirt*)self->mpWidget;
    if (widget != 0) {
        widget->updateLocale();
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_destroyPair);
#ifdef SKIP_ASM
/**
 * Deletes both widgets of a pair and clears the pointers.
 */
void cUIPair::destroyPair()
{
    cUIThing* entry = mpEntry;
    if (entry != 0) {
        delete entry;
        mpEntry = 0;
    }
    cUIThing* widget = mpWidget;
    if (widget != 0) {
        delete widget;
        mpWidget = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_onInput);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void* cUIThing_getEngine(void* self);
extern "C" int cUIThing_getSound(void* self, int a);
extern char gUISoundNull[4];

struct sVE_BED8 { short delta; short index; int (*fn)(void*, ...); };
struct sVEv_BED8 { short delta; short index; void (*fn)(void*, ...); };

static inline int inputIs_BED8(void* in, int slot)
{
    sVE_BED8* vt = *(sVE_BED8**)((char*)in + 8);
    return vt[slot].fn(vthis(in, vt[slot]));
}

static inline void focusNotify_BED8(char* self, int what)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEv_BED8* vt = *(sVEv_BED8**)(o + 8);
    vt[19].fn(vthis(o, vt[19]), self, what);
}

static inline int focusMove_BED8(char* self, int dir)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVE_BED8* vt = *(sVE_BED8**)(o + 8);
    return vt[21].fn(vthis(o, vt[21]), self, dir);
}

/**
 * Handles input for a widget pair: inputs 5 and 6 are reported to the state, input 4 notified, others go to
 * the second widget.
 *
 * @param in  the input (On Tour const cUIInput* pInput)
 * @return 1 when the pair used the input, otherwise the second widget's answer (0 without one)
 */
extern "C" int cUIPair_onInput(cUIPair* self, cUIInput* in)
{
    int result = 0;
    char* listener = gUISoundNull;
    cUIEngine* owner = (cUIEngine*)((char*)cUIThing_getEngine(((char*)self)));
    if ((char*)owner->mpSound != 0) {
        listener = (char*)owner->mpSound;
    }
    int moveResult;
    if (inputIs_BED8(in, 5)) {
        focusNotify_BED8(((char*)self), 5);
        moveResult = focusMove_BED8(((char*)self), 0);
        if (moveResult & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 2);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(((char*)self), 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(((char*)self), 0);
        }
        if (listener != 0) {
            sVEv_BED8* vt = *(sVEv_BED8**)listener;
            vt[2].fn(vthis(listener, vt[2]), result);
        }
    } else if (inputIs_BED8(in, 6)) {
        focusNotify_BED8(((char*)self), 6);
        moveResult = focusMove_BED8(((char*)self), 1);
        if (moveResult & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 3);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(((char*)self), 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(((char*)self), 0);
        }
        if (listener != 0) {
            sVEv_BED8* vt = *(sVEv_BED8**)listener;
            vt[2].fn(vthis(listener, vt[2]), result);
        }
    } else if (inputIs_BED8(in, 4)) {
        focusNotify_BED8(((char*)self), 4);
        return 1;
    } else {
        cUIThing* sub = self->mpWidget;
        if (((char*)sub) != 0) {
            return sub->onInput(in);
        }
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIPair_render2D);
#ifdef SKIP_ASM
/**
 * Draws both widgets of a pair at the pair's position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIPair_render2D(cUIPair* self, float x, float y)
{
    cUIThing* entry = self->mpEntry;
    if (entry != 0) {
        entry->render2D(x + self->mPosition.x, y + self->mPosition.y);
    }
    cUIThing* widget = self->mpWidget;
    if (widget != 0) {
        widget->render2D(x + self->mPosition.x, y + self->mPosition.y);
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
    virtual void update(unsigned short);
};

/**
 * Updates the pair's animation for a frame, then both of its widgets.
 *
 * @param ev  the frame number (On Tour FrameNum)
 */
extern "C" void cUIPair_update(cUIPair* self, unsigned short ev)
{
    cUIThing_update(self, ev);
    cUIPair_update_cVirt* entry = (cUIPair_update_cVirt*)self->mpEntry;
    if (entry != 0) {
        entry->update(ev);
    }
    cUIPair_update_cVirt* widget = (cUIPair_update_cVirt*)self->mpWidget;
    if (widget != 0) {
        widget->update(ev);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress__cUIProgress);
#ifdef SKIP_ASM
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern void* cUIProgress_vtbl[];

/**
 * Destroys a progress bar: deletes its two child widgets, then the cUIThing base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIProgress__cUIProgress(cUIProgress* self, int flags)
{
    VPTR_OF(cListNode, self) = cUIProgress_vtbl;
    cUIThing* background = self->mpObjBackground;
    delete background;
    cUIThing* knob = self->mpObjKnob;
    delete knob;
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
/**
 * Draws a progress bar's two widgets (when both exist) at its position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIProgress_render2D(cUIProgress* self, float x, float y)
{
    cUIThing* background = self->mpObjBackground;
    if (background != 0 && self->mpObjKnob != 0) {
        float px = x + self->mPosition.x;
        float py = y + self->mPosition.y;
        background->render2D(px, py);
        cUIThing* knob = self->mpObjKnob;
        knob->render2D(px, py);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uimenu", cUIProgress_update);
#ifdef SKIP_ASM
extern "C" void cUIAnimation_updateThing(void* anim, void* thing, unsigned char flags, unsigned short ev);
extern "C" void cUIAnimation_updateFrame(void* anim, unsigned char flags);

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
    cUIThing* knob = self->mpObjKnob;
    if (knob != 0) {
        knob->update(ev);
    }
    cUIThing* background = self->mpObjBackground;
    if (background != 0) {
        background->update(ev);
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
    virtual void getSize(cUIProgress_setValue_sVec3*);
    virtual void setSize(cUIProgress_setValue_sVec3*);
};

/**
 * Sets the progress value and resizes the knob to match (On Tour setValue(int value)).
 *
 * Stores mValue, takes the background's size (virtual slot 20, getSize; zero without a background) and, when there is
 * a knob and mNumValues is non-zero, scales the width (horizontal, mAttr bit 0 clear) or the height (vertical) by
 * mValue / mNumValues and gives it to the knob (slot 21, setSize). The value is not clamped.
 *
 * @param pos  the new value (On Tour value)
 */
void cUIProgress::setValue(int pos)
{
    mValue = pos;
    cUIProgress_setValue_cWidget* background = (cUIProgress_setValue_cWidget*)mpObjBackground;
    cUIProgress_setValue_sVec3 newSize = t3vect_Zero;
    if (background != 0) {
        background->getSize(&newSize);
    }
    if (mpObjKnob != 0) {
        int n = mNumValues;
        if (n != 0) {
            if (!(mAttr & 1)) {
                newSize.x *= (float)mValue / (float)n;
            } else {
                newSize.y *= (float)mValue / (float)n;
            }
            ((cUIProgress_setValue_cWidget*)mpObjKnob)->setSize(&newSize);
        }
    }
}
#endif

