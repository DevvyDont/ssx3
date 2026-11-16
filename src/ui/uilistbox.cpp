#include "common.h"
#include "ui/uilistbox.h"
#include "ui/uimenu.h"

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_addEntryByAsciiString);
#ifdef SKIP_ASM
extern "C" int strlen(const char*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern char D_00493DD0[];

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
    unsigned short* u = (unsigned short*)operator_new_tag((strlen(text) + 1) * 2, D_00493DD0, 0x100, 0);
    UIAsciiToUnicode(u, text);
    *(const char**)((char*)self + self->mNumEntries * 0x14 + 0xA0) = text;
    *(unsigned short**)((char*)self + self->mNumEntries * 0x14 + 0x9C) = u;
    *(int*)((char*)self + self->mNumEntries * 0x14 + 0xA4) = data;
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
extern char D_00493DC0[];

extern "C" unsigned char cUIListBox_addEntryByUnicodeString(cUIListBox* self, unsigned short* str, int data)
{
    unsigned short* u = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_00493DC0, 0x100, 0);
    USTR_copy(u, str);
    *(const char**)((char*)self + self->mNumEntries * 0x14 + 0xA0) = 0;
    *(unsigned short**)((char*)self + self->mNumEntries * 0x14 + 0x9C) = u;
    *(int*)((char*)self + self->mNumEntries * 0x14 + 0xA4) = data;
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
extern char D_00493DE0[];

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
    char* tbl = *(char**)(*(char**)(*(char**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x10);
    if (tbl != 0) {
        sUIStrVtA568* vt = *(sUIStrVtA568**)(tbl + 4);
        unsigned short* str = vt[4].fn(tbl + vt[4].delta, id);
        if (str != 0) {
            unsigned short* u = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_00493DE0, 0x100, 0);
            USTR_copy(u, str);
            *(const char**)((char*)self + self->mNumEntries * 0x14 + 0xA0) = 0;
            *(unsigned short**)((char*)self + self->mNumEntries * 0x14 + 0x9C) = u;
            *(int*)((char*)self + self->mNumEntries * 0x14 + 0xA4) = data;
            *(int*)((char*)self + self->mNumEntries * 0x14 + 0x98) = id;
        }
    }
    return (self->mNumEntries)++;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", func_0039A670);
#ifdef SKIP_ASM
void cMemMan_free(void*);

struct func_0039A670_sItem {
    void* data;
    char pad[0x10];
};

extern "C" void func_0039A670(void* self)
{
    if (*(int*)((char*)self + 0x74) & 0x10) {
        if (*(void**)((char*)self + 0x9C) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x9C));
        }
    } else {
        for (int i = 0; i < *(unsigned char*)((char*)self + 0x318); i++) {
            func_0039A670_sItem* it = (func_0039A670_sItem*)((char*)self + 0x9C) + i;
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
extern "C" int cUIListBox_getCurrentAsciiText(cUIListBox* self)
{
    if (self->mCurrentEntry < self->mNumEntries) {
        return *(int*)((char*)self + self->mCurrentEntry * 0x14 + 0xA0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_getCurrentEntryData);
#ifdef SKIP_ASM
extern "C" int cUIListBox_getCurrentEntryData(cUIListBox* self)
{
    if (self->mCurrentEntry < self->mNumEntries) {
        return *(int*)((char*)self + self->mCurrentEntry * 0x14 + 0xA4);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_getEntryByData);
#ifdef SKIP_ASM
extern "C" int cUIListBox_getEntryByData(cUIListBox* self, int key, unsigned char* out)
{
    int i;
    for (i = 0; i < self->mNumEntries; i++) {
        if (*(int*)((char*)self + i * 0x14 + 0xA4) == key) {
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
extern "C" void cUIListBox_setCurrentEntryByIndex(cUIListBox* self, int a1)
{
    unsigned char v = (unsigned char)a1;
    if (v < self->mNumEntries) {
        *(char*)&self->mCurrentEntry = v;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_setEntryDataByIndex);
#ifdef SKIP_ASM
extern "C" void cUIListBox_setEntryDataByIndex(cUIListBox* self, int a1, int value)
{
    unsigned char v = (unsigned char)a1;
    if (v < self->mNumEntries) {
        *(int*)((char*)self + v * 0x14 + 0xA4) = value;
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
extern char D_00493DF0[];

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
    unsigned short* u = self->entries[idx].str;
    if (u != 0) {
        if (len < self->entries[idx].cap) {
            cMemMan_free(u);
            u = 0;
        }
    }
    if (u == 0) {
        u = (unsigned short*)operator_new_tag(len * 2, D_00493DF0, 0x100, 0);
    }
    UIAsciiToUnicode(u, text);
    self->entries[idx].str = u;
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

extern "C" void cUIListBox_setCallbackListBox(cUIListBox* self, int count)
{
    *(char*)&self->mNumEntries = count;
    *(char*)&self->mCurrentEntry = 0;
    *(int*)&self->mAttrListBox |= 0x10;
    void* obj = *(void**)((char*)self->mpScreen + 0xD0);
    sVEntry39A8D8* vt = *(sVEntry39A8D8**)((char*)obj + 8);
    vt[0x13].fn((char*)obj + vt[0x13].delta, self, 9);
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
    ((sUIFlags39A928*)(((char*)self) + 0x14))->active = on != 0;
    if (on) {
        if (self->mStoredIndex != 0xFF) {
            cUIMenu_setSelectedByIndex(((char*)self), self->mStoredIndex);
            return;
        }
        char* cur = (char*)self->mpSelected;
        if (cur == 0) {
            unsigned char idx = cUIMenu_findFirstHighlightable(((char*)self), 0);
            if (idx == 0xFF)
                idx = 0;
            cUIMenu_setSelectedByIndex(((char*)self), idx);
        } else if (((*(int*)(cur + 0x14) >> 4) & 1) == 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 0);
            if (ev != 0xFFFF) {
                void* scr = *(void**)((char*)self->mpSelected + 0x5C);
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            char* c = (char*)self->mpSelected;
            sVEntry39A928a* va = *(sVEntry39A928a**)(c + 8);
            va[7].fn(c + va[7].delta, 1);
            char* o = *(char**)((char*)self->mpScreen + 0xD0);
            sVEntry39A928b* vb = *(sVEntry39A928b**)(o + 8);
            vb[19].fn(o + vb[19].delta, self->mpSelected, 1);
        }
    } else {
        char* cur = (char*)self->mpSelected;
        if (cur != 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 1);
            if (ev != 0xFFFF) {
                void* scr = *(void**)((char*)self->mpSelected + 0x5C);
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            char* c = (char*)self->mpSelected;
            sVEntry39A928a* va = *(sVEntry39A928a**)(c + 8);
            va[7].fn(c + va[7].delta, 0);
            char* o = *(char**)((char*)self->mpScreen + 0xD0);
            sVEntry39A928b* vb = *(sVEntry39A928b**)(o + 8);
            vb[19].fn(o + vb[19].delta, self->mpSelected, 2);
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
    char v = cList_size((char*)self + 0x74);
    *(char*)&self->mScrollItemNumVisible = v;
    *(char*)&self->mScrollItemNum = v;
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
        *(char*)((char*)self + 0x91) = 0;
        *(char*)((char*)self + 0x92) = 1;
        *(char*)((char*)self + 0x93) = 2;
        *(char*)((char*)self + 0x94) = 3;
    } else {
        *(char*)((char*)self + 0x91) = 4;
        *(char*)((char*)self + 0x92) = 5;
        *(char*)((char*)self + 0x93) = 6;
        *(char*)((char*)self + 0x94) = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_getNext);
#ifdef SKIP_ASM
extern "C" void* func_00397870(void* list, unsigned char i);

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
                char* o = *(char**)((char*)self->mpScreen + 0xD0);
                sUIVtAB50* vt = *(sUIVtAB50**)(o + 8);
                if (vt[20].fn(o + vt[20].delta, self, 1, i) != 0) return i;
            } else {
                char* e = (char*)func_00397870((char*)self + 0x74, i);
                int f = *(int*)(e + 0x14) >> 5;
                if ((f & 1) == 0) return i;
            }
            i = i + 1;
        } else {
            int f = self->mAttr >> 7;
            if ((f & 1) == 0) return self->mScrollItemCurrent;
            i = 0;
        }
    } while (n-- != 0);
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_getPrev);
#ifdef SKIP_ASM
extern "C" void* func_00397870(void* list, unsigned char i);

struct cUIMenu_getPrev_sVEntry {
    short delta;
    short index;
    int (*fn)(void*, void*, int, unsigned char);
};

extern "C" unsigned char cUIMenu_getPrev(cUIMenu* self)
{
    unsigned char n = self->mScrollItemNum;
    unsigned char i = self->mScrollItemCurrent - 1;
    do {
        if (i != 0xFF) {
            if (*(int*)&self->mAttrMenu & 4) {
                char* mgr = *(char**)((char*)self->mpScreen + 0xD0);
                cUIMenu_getPrev_sVEntry* vt = *(cUIMenu_getPrev_sVEntry**)(mgr + 8);
                if (vt[20].fn(mgr + vt[20].delta, self, 1, i)) {
                    return i;
                }
            } else {
                void* item = func_00397870((char*)self + 0x74, i);
                int f = *(int*)((char*)item + 0x14);
                if (((f >> 5) & 1) == 0) {
                    return i;
                }
            }
            i--;
        } else {
            int f = self->mAttr >> 7;
            if ((f & 1) == 0) {
                return self->mScrollItemCurrent;
            }
            i = self->mScrollItemNum - 1;
        }
        n--;
    } while (n != 0xFF);
    return 0;
}
#endif

