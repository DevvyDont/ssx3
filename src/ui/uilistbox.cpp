#include "vtable.h"
#include "common.h"
#include "ui/uiengine.h"
#include "ui/uistate.h"
#include "ui/uiscreen.h"
#include "ui/uithing.h"
#include "ui/uilistbox.h"
#include "ui/uimenu.h"
#include "util/locale.h"

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
unsigned char cUIListBox::addEntryByAsciiString(const char* text, int data)
{
    unsigned short* wide = (unsigned short*)operator_new_tag((strlen(text) + 1) * 2, str_UILBAscii, 0x100, 0);
    UIAsciiToUnicode(wide, text);
    mpEntries[mNumEntries].mpAsciiString = text;
    mpEntries[mNumEntries].mpString = wide;
    *(int*)&mpEntries[mNumEntries].mpData = data;
    return (mNumEntries)++;
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
unsigned char cUIListBox::addEntryByUnicodeString(unsigned short* str, int data)
{
    unsigned short* tempString = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UILISTBOXSTR, 0x100, 0);
    USTR_copy(tempString, str);
    mpEntries[mNumEntries].mpAsciiString = 0;
    mpEntries[mNumEntries].mpString = tempString;
    *(int*)&mpEntries[mNumEntries].mpData = data;
    return (mNumEntries)++;
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


/**
 * Appends a list-box entry showing a localized string, copied from the UI string table ("UILBStrID").
 *
 * When the string table or the string is missing the entry is left unset but the count still grows.
 *
 * @param id  the string's ID
 * @param data  caller data stored with the entry (On Tour void* pData)
 * @return the new entry's index
 */
unsigned char cUIListBox::addEntryByStringID(int id, void* data)
{
    cUILocaleInterface* loc = mpScreen->mpState->mpEngine->mpLocaleInterface;
    if (loc != 0) {
        unsigned short* str = loc->getStringByHash(id);
        if (str != 0) {
            unsigned short* wide = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UILBStrID, 0x100, 0);
            USTR_copy(wide, str);
            mpEntries[mNumEntries].mpAsciiString = 0;
            mpEntries[mNumEntries].mpString = wide;
            mpEntries[mNumEntries].mpData = data;
            mpEntries[mNumEntries].mStringID = id;
        }
    }
    return (mNumEntries)++;
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIListBox_deleteAllEntries);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Deletes all entries of a list box: frees their strings and sets the entry count and current index to 0.
 *
 * With mAttrListBox.mCallback (bit 0x10) set, one shared buffer (the first entry's mpString) is freed; otherwise each
 * of the mNumEntries entries frees its mpString. Then mCurrentEntry and mNumEntries are cleared. The options,
 * rule-select and network screens call it before refilling a list.
 */
void cUIListBox::deleteAllEntries()
{
    if (mAttrListBox.mCallback) {
        if (mpEntries[0].mpString != 0) {
            cMemMan_free((void*)mpEntries[0].mpString);
        }
    } else {
        for (int i = 0; i < mNumEntries; i++) {
            tListBoxEntry* entry = &mpEntries[i];
            if (entry->mpString != 0) {
                cMemMan_free((void*)entry->mpString);
            }
        }
    }
    mCurrentEntry = 0;
    mNumEntries = 0;
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
int cUIListBox::getCurrentAsciiText()
{
    if (mCurrentEntry < mNumEntries) {
        return (int)mpEntries[mCurrentEntry].mpAsciiString;
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
int cUIListBox::getCurrentEntryData()
{
    if (mCurrentEntry < mNumEntries) {
        return (int)mpEntries[mCurrentEntry].mpData;
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
 * @return true when found, false otherwise
 */
bool cUIListBox::getEntryByData(int key, unsigned char* out)
{
    int i;
    for (i = 0; i < mNumEntries; i++) {
        if ((int)mpEntries[i].mpData == key) {
            *out = i;
            return true;
        }
    }
    return false;
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
void cUIListBox::setCurrentEntryByIndex(int Index)
{
    unsigned char entryIndex = (unsigned char)Index;
    if (entryIndex < mNumEntries) {
        mCurrentEntry = entryIndex;
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
void cUIListBox::setEntryDataByIndex(int Index, int value)
{
    unsigned char entryIndex = (unsigned char)Index;
    if (entryIndex < mNumEntries) {
        *(int*)&mpEntries[entryIndex].mpData = value;
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

/**
 * Turns the list box into a callback list of @p count entries: mNumEntries = count, mCurrentEntry 0, sets mCallback
 * (bit 4 of mAttrListBox) and notifies the owning state (mpScreen->mpState) through its virtual slot 19 with (this,
 * 9).
 *
 * @param count  the number of entries (On Tour NumEntries)
 */
void cUIListBox::setCallbackListBox(int count)
{
    mNumEntries = count;
    mCurrentEntry = 0;
    *(int*)&mAttrListBox |= 0x10;
    cUIState* obj = mpScreen->mpState;
    obj->onWidgetEvent((cUIThing*)this, 9);
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


/**
 * Gives or removes focus on a menu, refocusing its stored, selected or first selectable item.
 *
 * Sets mAttr's mFocused bit. A stored index (set before the screen was ready) wins. Focus changes of the item are
 * reported to the screen's state (slot 19: 1 gained, 2 lost).
 *
 * @param on  nonzero to focus (On Tour focus)
 */
void cUIMenu::setFocus(int on)
{
    ((tAttrThing*)&mAttr)->mFocused = on != 0;
    if (on) {
        if (mStoredIndex != cUIMenu::kNoItem) {
            cUIMenu_setSelectedByIndex(((char*)this), mStoredIndex);
            return;
        }
        cUIThing* cur = mpSelected;
        if (((char*)cur) == 0) {
            unsigned char idx = cUIMenu_findFirstHighlightable(((char*)this), 0);
            if (idx == cUIMenu::kNoItem)
                idx = 0;
            cUIMenu_setSelectedByIndex(((char*)this), idx);
        } else if (((cur->mAttr >> 4) & 1) == 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 0);
            if (ev != cUIScreen::kNoFrame) {
                void* scr = mpSelected->mpScreen;
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            cUIThing* selected = mpSelected;
            selected->setFocus(1);
            cUIState* owner = mpScreen->mpState;
            owner->onWidgetEvent(mpSelected, 1);
        }
    } else {
        cUIThing* cur = mpSelected;
        if (((char*)cur) != 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)cur, 1);
            if (ev != cUIScreen::kNoFrame) {
                void* scr = mpSelected->mpScreen;
                if (scr != 0) {
                    cUIScreen_playFrame(scr, ev, 1);
                }
            }
            cUIThing* selected = mpSelected;
            selected->setFocus(0);
            cUIState* owner = mpScreen->mpState;
            owner->onWidgetEvent(mpSelected, 2);
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
void cUIMenu::setUIData(tUIBaseObject*)
{
    cUIThing_setUIData(this);
    char count = cList_size((char*)&mList);
    mScrollItemNumVisible = count;
    mScrollItemNum = count;
    setInputMap(0);
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
void cUIMenu::setInputMap(char value)
{
    if (value == 0) {
        mInputMap[0] = 0;
        mInputMap[1] = 1;
        mInputMap[2] = 2;
        mInputMap[3] = 3;
    } else {
        mInputMap[0] = 4;
        mInputMap[1] = 5;
        mInputMap[2] = 6;
        mInputMap[3] = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uilistbox", cUIMenu_getNext);
#ifdef SKIP_ASM
extern "C" void* cList_getByIndex(void* list, unsigned char i);

/**
 * Finds the next selectable item after the current one, wrapping to the start when the menu wraps (mAttr bit
 * 7).
 *
 * Scrollable menus ask the screen's state (slot 20); others skip child widgets whose mAttr mStatic bit is set.
 *
 * @return the next item's index, or the current one at the end of a non-wrapping menu
 */
unsigned char cUIMenu::getNext()
{
    unsigned char i = mScrollItemCurrent + 1;
    unsigned char n = mScrollItemNum;
    do {
        if (i < mScrollItemNum) {
            if (*(int*)&mAttrMenu & 4) {
                cUIState* owner = mpScreen->mpState;
                if (owner->onWidgetMenu(this, 1, i) != 0) return i;
            } else {
                cUIThing* item = (cUIThing*)cList_getByIndex((char*)&mList, i);
                int bits = item->mAttr >> 5;
                if ((bits & 1) == 0) return i;
            }
            i = i + 1;
        } else {
            int bits = mAttr >> 7;
            if ((bits & 1) == 0) return mScrollItemCurrent;
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

/**
 * Finds the selectable item before the current one (mScrollItemCurrent), wrapping to the last when mWrap is set.
 *
 * An item is selectable when the screen's state accepts it (onWidgetMenu(self, 1, i), slot 20) for a scrollable menu
 * (mScrollMenu), else when its mStatic bit is clear. Without mWrap, running off the start (the index wraps to kNoItem)
 * returns the current index. Gives up after mScrollItemNum + 1 tries.
 *
 * @return the item index, the current index (start reached without wrap), or 0 when none is selectable
 */
unsigned char cUIMenu::getPrev()
{
    unsigned char n = mScrollItemNum;
    unsigned char i = mScrollItemCurrent - 1;
    do {
        if (i != kNoItem) {
            if (*(int*)&mAttrMenu & 4) {
                cUIState* mgr = mpScreen->mpState;
                if (mgr->onWidgetMenu(this, 1, i)) {
                    return i;
                }
            } else {
                cUIThing* item = (cUIThing*)cList_getByIndex((char*)&mList, i);
                int bits = item->mAttr;
                if (((bits >> 5) & 1) == 0) {
                    return i;
                }
            }
            i--;
        } else {
            int bits = mAttr >> 7;
            if ((bits & 1) == 0) {
                return mScrollItemCurrent;
            }
            i = mScrollItemNum - 1;
        }
        n--;
    } while (n != 0xFF);
    return 0;
}
#endif

