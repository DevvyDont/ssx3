#include "common.h"
#include "ui/uiscreen.h"
#include "ui/uithing.h"
#include "ui/uilistbox.h"
#include "ui/uimenu.h"

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByAsciiString);
#ifdef SKIP_ASM
extern "C" int strlen(const char*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern char str_UILBAscii[];

/**
 * Appends a list-box entry showing an ASCII string, converted to a new Unicode copy ("UILBAscii").
 *
 * The list box has room for 32 entries and nothing checks the count.
 *
 * @param text  the entry's text (On Tour pAscii); the pointer is kept too
 * @param data  caller data stored with the entry (On Tour void* pData)
 * @return the new entry's index
 */
extern "C" unsigned char cUIListBox_addEntryByAsciiString(cUIListBox* self, const char* text, int data)
{
    unsigned short* wide = (unsigned short*)operator_new_tag((strlen(text) + 1) * 2, str_UILBAscii, 0x100, 0);
    UIAsciiToUnicode(wide, text);
    self->mpEntries[self->mNumEntries].mpAsciiString = text;
    *(unsigned short**)&self->mpEntries[self->mNumEntries].mpString = wide;
    *(int*)&self->mpEntries[self->mNumEntries].mpData = data;
    return (self->mNumEntries)++;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByUnicodeString);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern char str_UILISTBOXSTR[];

/**
 * Appends an entry with a heap copy of @p str (tag str_UILISTBOXSTR "UILISTBOXSTR") as its text, no ASCII text and @p data
 * as its data.
 *
 * There is no check against the 32-entry array; mStringID of the new entry is left as it was.
 *
 * @param str  the UTF-16 text (On Tour pUnicode)
 * @param data  the entry's user data (On Tour pData)
 * @return the new entry's index
 */
extern "C" unsigned char cUIListBox_addEntryByUnicodeString(cUIListBox* self, unsigned short* str, int data)
{
    unsigned short* tempString = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UILISTBOXSTR, 0x100, 0);
    USTR_copy(tempString, str);
    self->mpEntries[self->mNumEntries].mpAsciiString = 0;
    *(unsigned short**)&self->mpEntries[self->mNumEntries].mpString = tempString;
    *(int*)&self->mpEntries[self->mNumEntries].mpData = data;
    return (self->mNumEntries)++;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByStringID);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern char str_UILBStrID[];

struct sUIStrVtA568 { short delta; short index; unsigned short* (*fn)(void*, int); };

/**
 * Appends a list-box entry showing a localized string, copied from the UI string table ("UILBStrID").
 *
 * When the string table or the string is missing the entry is left unset but the count still grows.
 *
 * @param id  the string's ID
 * @param data  caller data stored with the entry (On Tour void* pData)
 * @return the new entry's index
 */
extern "C" unsigned char cUIListBox_addEntryByStringID(cUIListBox* self, int id, int data)
{
    char* tbl = *(char**)(*(char**)((char*)self->mpScreen->mpState + 0x10) + 0x10);
    if (tbl != 0) {
        sUIStrVtA568* vt = *(sUIStrVtA568**)(tbl + 4);
        unsigned short* str = vt[4].fn(tbl + vt[4].delta, id);
        if (str != 0) {
            unsigned short* wide = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UILBStrID, 0x100, 0);
            USTR_copy(wide, str);
            self->mpEntries[self->mNumEntries].mpAsciiString = 0;
            *(unsigned short**)&self->mpEntries[self->mNumEntries].mpString = wide;
            *(int*)&self->mpEntries[self->mNumEntries].mpData = data;
            *(int*)&self->mpEntries[self->mNumEntries].mStringID = id;
        }
    }
    return (self->mNumEntries)++;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_deleteAllEntries);
#ifdef SKIP_ASM
void cMemMan_free(void*);

struct cUIListBox_deleteAllEntries_sItem {
    void* data;
    char pad[0x10];
};

/**
 * Deletes all entries of a list box: frees their strings and sets the entry count and current index to 0.
 *
 * With bit 0x10 of the attributes (+0x74) one shared buffer at +0x9C is freed; otherwise each of the +0x318 entries
 * (0x14 bytes from +0x9C) frees its string. Then +0x319 and +0x318 are cleared. The options, rule-select and network
 * screens call it before refilling a list.
 */
extern "C" void cUIListBox_deleteAllEntries(void* self)
{
    if (*(int*)((char*)self + 0x74) & 0x10) {
        if (*(void**)((char*)self + 0x9C) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x9C));
        }
    } else {
        for (int i = 0; i < *(unsigned char*)((char*)self + 0x318); i++) {
            cUIListBox_deleteAllEntries_sItem* it = (cUIListBox_deleteAllEntries_sItem*)((char*)self + 0x9C) + i;
            if (it->data != 0) {
                cMemMan_free(it->data);
            }
        }
    }
    *(unsigned char*)((char*)self + 0x319) = 0;
    *(unsigned char*)((char*)self + 0x318) = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_getCurrentAsciiText);
#ifdef SKIP_ASM
/**
 * Returns the ASCII text of the current entry (mpAsciiString), or 0 when mCurrentEntry is out of range.
 *
 * @return the text pointer (as an int)
 */
extern "C" int cUIListBox_getCurrentAsciiText(cUIListBox* self)
{
    if (self->mCurrentEntry < self->mNumEntries) {
        return (int)self->mpEntries[self->mCurrentEntry].mpAsciiString;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_getCurrentEntryData);
#ifdef SKIP_ASM
/**
 * Returns the user data of the current entry (mpData), or 0 when mCurrentEntry is out of range.
 *
 * @return the data (as an int)
 */
extern "C" int cUIListBox_getCurrentEntryData(cUIListBox* self)
{
    if (self->mCurrentEntry < self->mNumEntries) {
        return (int)self->mpEntries[self->mCurrentEntry].mpData;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_getEntryByData);
#ifdef SKIP_ASM
/**
 * Finds the first entry whose user data equals @p key and stores its index in @p out.
 *
 * @param key  the data to look for (On Tour pData)
 * @param out  receives the entry index (On Tour pIndex)
 * @return 1 when found, 0 otherwise
 */
extern "C" int cUIListBox_getEntryByData(cUIListBox* self, int key, unsigned char* out)
{
    int i;
    for (i = 0; i < self->mNumEntries; i++) {
        if ((int)self->mpEntries[i].mpData == key) {
            *out = i;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_setCurrentEntryByIndex);
#ifdef SKIP_ASM
/**
 * Makes entry @p Index current; the index is truncated to a byte and ignored when not below mNumEntries.
 *
 * @param Index  the entry index
 */
extern "C" void cUIListBox_setCurrentEntryByIndex(cUIListBox* self, int Index)
{
    unsigned char entryIndex = (unsigned char)Index;
    if (entryIndex < self->mNumEntries) {
        *(char*)&self->mCurrentEntry = entryIndex;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_setEntryDataByIndex);
#ifdef SKIP_ASM
/**
 * Sets the user data of entry @p Index; the index is truncated to a byte and ignored when not below mNumEntries.
 *
 * @param Index  the entry index
 * @param value  the new user data (On Tour pData)
 */
extern "C" void cUIListBox_setEntryDataByIndex(cUIListBox* self, int Index, int value)
{
    unsigned char entryIndex = (unsigned char)Index;
    if (entryIndex < self->mNumEntries) {
        *(int*)&self->mpEntries[entryIndex].mpData = value;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_setEntryByAsciiString);
#ifdef SKIP_ASM
extern "C" int strlen(const char*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern char str_UIListBoxEntry[];

struct cUIListBox_sEntry {
    int id;
    unsigned short* str;
    const char* text;
    int data;
    signed char cap;
};

struct cUIListBox_sObj {
    char pad[0x98];
    cUIListBox_sEntry entries[32];
    unsigned char count;
    unsigned char cur;
};

/**
 * Sets a list box entry's text from an ASCII string, plus its data word.
 *
 * The text is converted to 16-bit characters in the entry's buffer. An existing buffer is reused unless the new
 * length is below the entry's recorded size, in which case it is freed and a new "UIListBoxEntry" buffer of the
 * new length is allocated; the size is not updated. The ASCII pointer is kept too.
 *
 * @param idx   the entry index
 * @param text  the ASCII text
 * @param data  the data word stored with the entry
 */
extern "C" void cUIListBox_setEntryByAsciiString(cUIListBox_sObj* self, unsigned char idx, const char* text, int data)
{
    int len = strlen(text) + 1;
    unsigned short* wide = self->entries[idx].str;
    if (wide != 0) {
        if (len < self->entries[idx].cap) {
            cMemMan_free(wide);
            wide = 0;
        }
    }
    if (wide == 0) {
        wide = (unsigned short*)operator_new_tag(len * 2, str_UIListBoxEntry, 0x100, 0);
    }
    UIAsciiToUnicode(wide, text);
    self->entries[idx].str = wide;
    self->entries[idx].text = text;
    self->entries[idx].data = data;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_setCallbackListBox);
#ifdef SKIP_ASM
struct sVEntry39A8D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Turns the list box into a callback list of @p count entries: mNumEntries = count, mCurrentEntry 0, sets mCallback
 * (bit 4 of mAttrListBox) and notifies the owning state (mpScreen->mpState) through its virtual slot 19 with (this,
 * 9).
 *
 * @param count  the number of entries (On Tour NumEntries)
 */
extern "C" void cUIListBox_setCallbackListBox(cUIListBox* self, int count)
{
    *(char*)&self->mNumEntries = count;
    *(char*)&self->mCurrentEntry = 0;
    *(int*)&self->mAttrListBox |= 0x10;
    void* obj = self->mpScreen->mpState;
    sVEntry39A8D8* vt = *(sVEntry39A8D8**)((char*)obj + 8);
    vt[19].fn((char*)obj + vt[0x13].delta, self, 9);
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_setFocus);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" unsigned char cUIMenu_findFirstHighlightable(void* self, int a1);

struct sVEntry39A928a {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEntry39A928b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sUIFlags39A928 {
    unsigned int pad0 : 4;
    unsigned int active : 1;
};

/**
 * Gives or removes focus on a menu, refocusing its stored, selected or first selectable item.
 *
 * A stored index (set before the screen was ready) wins. Focus changes of the item are reported to the
 * screen's
 * state (slot 19: 1 gained, 2 lost).
 *
 * @param on  nonzero to focus (On Tour focus)
 */
extern "C" void cUIMenu_setFocus(cUIMenu* self, int on)
{
    ((sUIFlags39A928*)(&self->mAttr))->active = on != 0;
    if (on) {
        if (self->mStoredIndex != 0xFF) {
            cUIMenu_setSelectedByIndex(((char*)self), self->mStoredIndex);
            return;
        }
        cUIThing* cur = self->mpSelected;
        if (((char*)cur) == 0) {
            unsigned char idx = cUIMenu_findFirstHighlightable(((char*)self), 0);
            if (idx == 0xFF)
                idx = 0;
            cUIMenu_setSelectedByIndex(((char*)self), idx);
        } else if (((cur->mAttr >> 4) & 1) == 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 0);
            if (ev != 0xFFFF) {
                void* scr = self->mpSelected->mpScreen;
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            cUIThing* selected = self->mpSelected;
            sVEntry39A928a* va = (sVEntry39A928a*)selected->mVtbl;
            va[7].fn(((char*)selected) + va[7].delta, 1);
            char* owner = (char*)self->mpScreen->mpState;
            sVEntry39A928b* vb = *(sVEntry39A928b**)(owner + 8);
            vb[19].fn(owner + vb[19].delta, self->mpSelected, 1);
        }
    } else {
        cUIThing* cur = self->mpSelected;
        if (((char*)cur) != 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 1);
            if (ev != 0xFFFF) {
                void* scr = self->mpSelected->mpScreen;
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            cUIThing* selected = self->mpSelected;
            sVEntry39A928a* va = (sVEntry39A928a*)selected->mVtbl;
            va[7].fn(((char*)selected) + va[7].delta, 0);
            char* owner = (char*)self->mpScreen->mpState;
            sVEntry39A928b* vb = *(sVEntry39A928b**)(owner + 8);
            vb[19].fn(owner + vb[19].delta, self->mpSelected, 2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_setUIData);
#ifdef SKIP_ASM
extern "C" void* cUIThing_setUIData(void* self);
extern "C" char cList_size(void*);
extern "C" void cUIMenu_setInputMap(cUIMenu* self, char a1);

/**
 * Sets a menu up from its screen data: one row per child widget, all visible, with the vertical input map.
 *
 * On Tour passes the screen-data record; this build's declaration drops it.
 */
extern "C" void cUIMenu_setUIData(cUIMenu* self)
{
    cUIThing_setUIData(self);
    char count = cList_size((char*)&self->mList);
    *(char*)&self->mScrollItemNumVisible = count;
    *(char*)&self->mScrollItemNum = count;
    cUIMenu_setInputMap(self, 0);
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_setInputMap);
#ifdef SKIP_ASM
/**
 * Chooses which inputs move a menu's selection: 0 maps inputs 0-3, anything else inputs 4-7.
 *
 * @param value  0 for the first input set, nonzero for the second
 */
extern "C" void cUIMenu_setInputMap(cUIMenu* self, char value)
{
    if (value == 0) {
        self->mInputMap[0] = 0;
        self->mInputMap[1] = 1;
        self->mInputMap[2] = 2;
        self->mInputMap[3] = 3;
    } else {
        self->mInputMap[0] = 4;
        self->mInputMap[1] = 5;
        self->mInputMap[2] = 6;
        self->mInputMap[3] = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_getNext);
#ifdef SKIP_ASM
extern "C" void* cList_getByIndex(void* list, unsigned char i);

struct sUIVtAB50 { short delta; short index; int (*fn)(void*, void*, int, int); };

/**
 * Finds the next selectable item after the current one, wrapping to the start when the menu wraps (mAttr bit
 * 7).
 *
 * Scrollable menus ask the screen's state (slot 20); others skip disabled child widgets.
 *
 * @return the next item's index, or the current one at the end of a non-wrapping menu
 */
extern "C" unsigned char cUIMenu_getNext(cUIMenu* self)
{
    unsigned char i = self->mScrollItemCurrent + 1;
    unsigned char n = self->mScrollItemNum;
    do {
        if (i < self->mScrollItemNum) {
            if (*(int*)&self->mAttrMenu & 4) {
                char* owner = (char*)self->mpScreen->mpState;
                sUIVtAB50* vt = *(sUIVtAB50**)(owner + 8);
                if (vt[20].fn(owner + vt[20].delta, self, 1, i) != 0) return i;
            } else {
                char* item = (char*)cList_getByIndex((char*)&self->mList, i);
                int bits = *(int*)(item + 0x14) >> 5;
                if ((bits & 1) == 0) return i;
            }
            i = i + 1;
        } else {
            int bits = self->mAttr >> 7;
            if ((bits & 1) == 0) return self->mScrollItemCurrent;
            i = 0;
        }
    } while (n-- != 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_getPrev);
#ifdef SKIP_ASM
extern "C" void* cList_getByIndex(void* list, unsigned char i);

struct cUIMenu_getPrev_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

/**
 * Finds the selectable item before the current one (mScrollItemCurrent), wrapping to the last when mWrap is set.
 *
 * An item is selectable when the screen's state accepts it (onWidgetMenu(self, 1, i), slot 20) for a scrollable menu
 * (mScrollMenu), else when its mStatic bit is clear. Without mWrap, running off the start returns the current index.
 * Gives up after mScrollItemNum + 1 tries.
 *
 * @return the item index, the current index (start reached without wrap), or 0 when none is selectable
 */
extern "C" unsigned char cUIMenu_getPrev(cUIMenu* self)
{
    unsigned char n = self->mScrollItemNum;
    unsigned char i = self->mScrollItemCurrent - 1;
    do {
        if (i != 0xFF) {
            if (*(int*)&self->mAttrMenu & 4) {
                char* mgr = (char*)self->mpScreen->mpState;
                cUIMenu_getPrev_sVEntry* vt = *(cUIMenu_getPrev_sVEntry**)(mgr + 8);
                if (vt[20].fn(mgr + vt[20].delta, self, 1, i)) {
                    return i;
                }
            } else {
                void* item = cList_getByIndex((char*)&self->mList, i);
                int bits = *(int*)((char*)item + 0x14);
                if (((bits >> 5) & 1) == 0) {
                    return i;
                }
            }
            i--;
        } else {
            int bits = self->mAttr >> 7;
            if ((bits & 1) == 0) {
                return self->mScrollItemCurrent;
            }
            i = self->mScrollItemNum - 1;
        }
        n--;
    } while (n != 0xFF);
    return 0;
}
#endif

