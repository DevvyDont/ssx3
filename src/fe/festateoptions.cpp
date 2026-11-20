#include "common.h"
#include "fe/festateoptions.h"
#include "ui/uimenu.h"
#include "ui/uiinput.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festateoptionssound.h"
#include "fe/festateoptionshud.h"
#include "fe/festateoptionsgame.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_18options[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the options menu screen ("18options") and shows its first frame.
 *
 * Then clears the word at +0x48 and sets the word at +0x4C to 1 (onPopupEvent writes the same pair after the
 * save-options prompt).
 */
extern "C" void cFEStateOptions_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_18options), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];
extern char D_004A1510[];
extern char D_004A1518[];
extern char D_004A1520[];
extern char D_004A1528[];
extern char D_004A1530[];

static inline int Is_00188870(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Tags each options menu entry ("1" to "8") with the id of the state it opens.
 *
 * Stores the id in the widget's misc data (+0x18): 0x1C, 0x1D, 0x1F, 0x1E, 0x20, 0x24, 0x24 and 0x22;
 * onWidgetEvent passes it to the custom interface to get the next state.
 *
 * @param item  the widget being created; matched by its hash name (+0x38)
 */
extern "C" void cFEStateOptions_onWidgetCreate(void* self, cUIThing* item)
{
    if (Is_00188870(item->mHashName, D_004A14F8)) {
        item->mMiscData = 0x1C;
    } else if (Is_00188870(item->mHashName, D_004A1500)) {
        item->mMiscData = 29;
    } else if (Is_00188870(item->mHashName, D_004A1508)) {
        item->mMiscData = 0x1F;
    } else if (Is_00188870(item->mHashName, D_004A1510)) {
        item->mMiscData = 0x1E;
    } else if (Is_00188870(item->mHashName, D_004A1518)) {
        item->mMiscData = 0x20;
    } else if (Is_00188870(item->mHashName, D_004A1520)) {
        item->mMiscData = 36;
    } else if (Is_00188870(item->mHashName, D_004A1528)) {
        item->mMiscData = 36;
    } else if (Is_00188870(item->mHashName, D_004A1530)) {
        item->mMiscData = 34;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* self, void* engine, void* owner, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* self, char* s);
extern "C" void cKeyboardPopup_setStatic2(void* self, char* s);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int a1, int a2, int a3);
extern "C" int func_00187D38(void* self);
extern "C" void cFEStateOptions_saveOptionPrompt(void* self);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cKeyboardPopup[];
extern char str_TransitionOut[];
extern char D_004A1408[];
extern char D_004A1520[];
extern char D_004A1530[];
extern int D_004A14D0;
extern int D_004A14D4;
extern int D_004A14D8;
extern int D_004A14DC;
extern int D_004A14E0;
extern char gUISoundNull;

struct sVE_188980p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

class cListener_188980 {
public:
    virtual void v01();
    virtual void playSound(int msg);
};

struct sOpt_188980 {
    char pad0[0x10];
    char* engine;           // 0x10
    char pad14;
    unsigned char port;     // 0x15
    char pad16[0x1C - 0x16];
    int flags;              // 0x1C
    char pad20[0x40 - 0x20];
    cUIScreen* screen;      // 0x40
    int pad44;
    int f48;                // 0x48
    int pad4C;
    void* popup;            // 0x50
};

static inline int Is_00188980(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Handles a widget event on the options menu (select, back, keyboard popup closed).
 *
 * Event 5 (select): entry "6" opens a cKeyboardPopup with empty text; entry "8" asks to save first
 * (cFEStateOptions_saveOptionPrompt, probably saveOptionPrompt) when an options-changed flag (D_004A14D0/D4/D8/DC) is set and
 * D_004A14E0 is clear, otherwise it sets the pending-delete bit 0x80 of mAttrState (+0x1C), plays
 * "TransitionOut" and, when +0x48 is set, stores 0x18 at +0xC of the custom interface; every other entry (and
 * "8" with +0x48 set) pushes the state that the engine's custom interface (engine + 0x0) virtual slot 4,
 * probably getNextState returns for the widget's misc data.
 *
 * Event 6 (back) does the same save check or transition-out. Event 13 (the keyboard popup is done) checks the
 * typed text with func_00187D38 (unknown) and calls slot 2 of the engine's listener at +0x14 (default
 * gUISoundNull) with 6 on a result of 1, else 4, then forgets the popup.
 *
 * @param widget  the widget the event is for
 * @param event  UI event code (5, 6 or 13 are handled)
 */
extern "C" void cFEStateOptions_onWidgetEvent(sOpt_188980* self, cUIThing* widget, unsigned int event)
{
    switch (event) {
    case 5:
        if (Is_00188980(widget->mHashName, D_004A1520)) {
            void* mem = cMemMan_alloc(1092, str_cKeyboardPopup, 0x100, 0);
            void* pop = cKeyboardPopup_cKeyboardPopup(mem, self->engine, self, 0, self->port);
            self->popup = pop;
            cKeyboardPopup_setInputText(pop, D_004A1408);
            cKeyboardPopup_setStatic2(self->popup, D_004A1408);
            cKeyboardPopup_setIncludeKey(self->popup, 75, 1, 2);
            cUIStateStack_pushPopup(self->engine + 0x18, self->popup);
            break;
        } else if (Is_00188980(widget->mHashName, D_004A1530)) {
            if (D_004A14E0 == 0 && (D_004A14D0 != 0 || D_004A14D4 != 0 || D_004A14DC != 0 || D_004A14D8 != 0)) {
                cFEStateOptions_saveOptionPrompt(self);
                break;
            }
            D_004A14E0 = 0;
            self->flags |= 0x80;
            int frame = cUIScreen_getFrameByLabel(self->screen, GetHashValue32(str_TransitionOut));
            if (frame != 0xFFFF)
                cUIScreen_playFrame(self->screen, frame, 1);
            if (self->f48 == 0)
                break;
            char* custom = *(char**)self->engine;
            if (custom != 0)
                *(int*)(custom + 0xC) = 0x18;
        }
        {
            char* obj = *(char**)self->engine;
            sVE_188980p* vt = *(sVE_188980p**)(obj + 4);
            void* r = vt[4].fn(obj + vt[4].delta, self, widget->mMiscData);
            if (r != 0)
                cUIStateStack_push(self->engine + 0x18, r);
        }
        break;
    case 6: {
        if (D_004A14E0 == 0 && (D_004A14D0 != 0 || D_004A14D4 != 0 || D_004A14DC != 0 || D_004A14D8 != 0)) {
            cFEStateOptions_saveOptionPrompt(self);
            break;
        }
        D_004A14E0 = 0;
        self->flags |= 0x80;
        int frame = cUIScreen_getFrameByLabel(self->screen, GetHashValue32(str_TransitionOut));
        if (frame != 0xFFFF)
            cUIScreen_playFrame(self->screen, frame, 1);
        break;
    }
    case 13: {
        int r = func_00187D38((char*)self->popup + 0x74);
        char* obj = &gUISoundNull;
        char* sound = *(char**)(self->engine + 0x14);
        if (sound != 0)
            obj = sound;
        if (obj != 0) {
            if (r == 1) {
                ((cListener_188980*)obj)->playSound(6);
            } else {
                ((cListener_188980*)obj)->playSound(4);
            }
        }
        self->popup = 0;
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onInputBegin);
#ifdef SKIP_ASM
extern int D_004A3484;

/**
 * Records which controller started input on the options menu.
 *
 * Stores the input's byte at +4 (probably the controller port) in D_004A3484.
 *
 * @param pInput  the UI input object
 * @return always 0
 */
extern "C" int cFEStateOptions_onInputBegin(void* self, cUIInput* pInput)
{
    D_004A3484 = pInput->mPadNum;
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onWidgetMenu);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cList_getByIndex(void* list, int index);
extern char D_004A1530[];

/**
 * Filters menu keys on the options menu.
 *
 * Keys 8 and 9 give 0x100. Key 6 on the entry hashed "8", or key 7 anywhere, gives 1 when the word at +0x4C
 * is 0 and 0x101 otherwise; every other key gives 0x101.
 *
 * @param menu  the menu; its entry list is at +0x74
 * @param key  menu key/event code
 * @param index  index of the current entry in the menu list
 * @return 0x100, 0x101 or 1 (menu handling codes)
 */
extern "C" int cFEStateOptions_onWidgetMenu(void* self, cUIMenu* menu, unsigned int key, int index)
{
    if (key < 10) {
        if (key >= 8) {
            return 0x100;
        }
    }
    if (key == 6) {
        int itemHash = *(int*)((char*)cList_getByIndex((char*)&menu->mList, index) + 0x38);
        if (itemHash == GetHashValue32(D_004A1530)) {
            goto check;
        }
    }
    if (key == 7) {
    check:
        int result = 257;
        if (*(int*)((char*)self + 0x4C) == 0) {
            result = 1;
        }
        return result;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_reset_text[];
extern char str_kT_FESaveOptionsQuesPS2[];
extern char str_111resetopts[];
extern char D_004A1530[];

struct sVEK188D08a {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sVEK188D08b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

static inline int IsK188D08(int id, char* s)
{
    return id == GetHashValue32(s);
}

/**
 * Handles events from the save-options popup.
 *
 * Event 0x15 sets the popup screen's "reset text" to kT_FESaveOptionsQuesPS2. Event 0x16 from the popup
 * hashed "111resetopts" reads the answer (popup virtual slot 23), stores it at +0x48, sets +0x4C to 1 and
 * re-sends entry "8" to this state's virtual slot 19 (probably onWidgetEvent) as event 5 (yes) or 6 (no).
 *
 * @param popup  the popup state; its hash (+0xC) and screen (+0x40) are read
 * @param event  popup event code (0x15 or 0x16 are handled)
 */
extern "C" void cFEStateOptions_onPopupEvent(void* self, cUIState* popup, int event)
{
    switch (event) {
    case 21: {
        void* scr = popup->mpScreen;
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(scr, GetHashValue32(str_reset_text));
        if (text != 0) {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_FESaveOptionsQuesPS2));
        }
        break;
    }
    case 22:
        if (IsK188D08(popup->mHashName, str_111resetopts)) {
            void* uiObj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1530));
            sVEK188D08a* vt = (sVEK188D08a*)popup->mVtbl;
            if (vt[23].fn(((char*)popup) + vt[23].delta, 0)) {
                if (uiObj != 0) {
                    *(int*)((char*)self + 0x48) = 1;
                    *(int*)((char*)self + 0x4C) = 1;
                    sVEK188D08b* vt2 = *(sVEK188D08b**)((char*)self + 8);
                    vt2[19].fn((char*)self + vt2[19].delta, uiObj, 5);
                }
            } else {
                if (uiObj != 0) {
                    *(int*)((char*)self + 0x48) = 0;
                    *(int*)((char*)self + 0x4C) = 1;
                    sVEK188D08b* vt2 = *(sVEK188D08b**)((char*)self + 8);
                    vt2[19].fn((char*)self + vt2[19].delta, uiObj, 6);
                }
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptions_saveOptionPrompt);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern void* cFEPopupOptionReset_vtbl[];
extern int D_004A14D0;
extern int D_004A14D4;
extern int D_004A14D8;
extern int D_004A14DC;
extern int D_004A14E0;

/**
 * Pushes the save-options popup (a cFEPopupOptionReset) over the options menu and clears every options-changed flag.
 *
 * Allocates the 76-byte popup (tag "cFEPopupOptionReset"), constructs it as a child of this state (cUIState_cUIState1 with
 * the engine at +0x10), stores its vtable cFEPopupOptionReset_vtbl, clears its byte at +0x48 and pushes it onto the engine's state
 * stack (engine + 0x18) with cUIStateStack_pushPopup. cFEStateOptions_onPopupEvent sets the popup's question to
 * kT_FESaveOptionsQuesPS2 and handles the answer.
 *
 * It then clears the game, sound, HUD and controller changed flags (D_004A14D0, D_004A14D4, D_004A14D8, D_004A14DC),
 * sets D_004A14E0 to 1 (cFEStateOptions_onWidgetEvent only prompts while it is 0 and clears it when the menu is left)
 * and clears +0x4C, which cFEStateOptions_onPopupEvent sets to 1 once the popup is answered.
 */
extern "C" void cFEStateOptions_saveOptionPrompt(void* self)
{
    char* savePopup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
    cUIState_cUIState1(savePopup, *(void**)((char*)self + 0x10), self);
    *(void***)(savePopup + 8) = cFEPopupOptionReset_vtbl;
    *(char*)(savePopup + 0x48) = 0;
    cUIStateStack_pushPopup(*(char**)((char*)self + 0x10) + 0x18, savePopup);
    D_004A14D8 = 0;
    D_004A14E0 = 1;
    D_004A14DC = 0;
    D_004A14D4 = 0;
    D_004A14D0 = 0;
    *(int*)((char*)self + 0x4C) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00188EE8);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateOptionsGame_vtbl[];

/**
 * Constructs the game options state (tag "cFEStateOptionsGame", 76 bytes), which cFECustom_getNextState creates for
 * state 28.
 *
 * Runs cUIState's constructor cUIState_cUIState (the engine pointer the caller passes in the second argument register goes
 * straight through; it is not declared here), stores cFEStateOptionsGame_vtbl in mVtbl (+0x8) and sets mHashName (+0xC)
 * to 0x1C. Unlike the sound, HUD and controller constructors it leaves its changed flag D_004A14D0 alone.
 * cFEStateOptionsGame_cFEStateOptionsGame is the variant built with a null engine.
 *
 * @return @p self
 */
extern "C" void* func_00188EE8(void* self)
{
    cUIState_cUIState(self);
    *(void***)((char*)self + 0x8) = cFEStateOptionsGame_vtbl;
    *(int*)((char*)self + 0xC) = 0x1C;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int cUIListBox_addEntryByStringID_i(void* box, int id, int idx) __asm__("cUIListBox_addEntryByStringID");
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_19game_opt[];
extern char str_kT_LANGAmerican[];
extern char str_kT_LANGFrench[];
extern char str_kT_LANGGerman[];
extern char str_kT_LANGSpanish[];
extern char str_kT_CMNMPH[];
extern char str_kT_CMNKPH[];
extern char str_kT_CMNOff[];
extern char str_kT_19Widescreen169[];
extern char str_kT_19WidescreenAnimorphic[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char str_Menu[];
extern char* D_00440FB8[];
extern char* D_00440FC8[];
extern char* D_00440FD8[];
extern char* D_00440FE8[];
extern char* D_00440FF8[];

// cBEOptionInterface_mOption's first word (field names from On Tour's tBEOptionDB bitfields).
struct sGameCfg_188F28 {
    unsigned int pad0 : 19;
    unsigned int iUseMetricUnits : 1;       // bit 19: MPH (0) / KPH (1)
    unsigned int iUseWidescreen : 2;           // bits 20-21: off / 16:9 / anamorphic
    unsigned int iLanguage : 3;          // bits 22-24 (FELocale's language)
    unsigned int pad25 : 7;
    signed char f4;
    signed char f5;                 // 0x5
    signed char f6;                 // 0x6
    char pad7[0x288 - 7];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sGameCfg_188F28 D_00535610_188F28 __asm__("cBEOptionInterface_mOption");

struct sOptionIface_188F28 {
    sGameCfg_188F28 getOptions() { return D_00535610_188F28; }
};

class cUIObj_188F28 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

struct sOptGame_188F28 {
    char pad0[0x10];
    void* engine;                   // 0x10
    char pad14[0x2C];
    char* screen;                   // 0x40
    char pad44[0x4];
    signed char item[3];            // 0x48
};

static inline void* find_188F28(sOptGame_188F28* self, char** names, int i)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(names[self->item[i]]));
}

static inline void setText_188F28(sOptGame_188F28* self, char** names, int i, char** ids)
{
    cUIText* t = (cUIText*)find_188F28(self, names, i);
    if (t != 0)
        cUIText_setUnicodeStringByID(t, GetHashValue32(ids[i]));
}

/**
 * Creates the game options screen ("19game_opt") and fills its option rows.
 *
 * Row indices are item[0] = -1, item[1] = 1, item[2] = 2, so the language row (four kT_LANG entries) is
 * skipped. Row 1 gets MPH/KPH and selects the speed-unit setting (iUseMetricUnits); row 2
 * gets Off/16:9/anamorphic and selects the widescreen setting (iUseWidescreen). Each row also gets its title and help
 * text, the entry "1" is hidden but enabled, and the "Menu" selection starts at entry 1.
 */
extern "C" void cFEStateOptionsGame_onCreateScreen(cFEStateOptionsGame* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, ((sOptGame_188F28*)self), GetHashValue32(str_19game_opt), 0);
    self->mpScreen = (cUIScreen*)((char*)screen);
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    sOptionIface_188F28* iface = (sOptionIface_188F28*)cBE_getInterface_Fv(cBE_getBE(), 4);
    ((signed char*)&self->mLanguageSlot)[0] = -1;
    ((signed char*)&self->mLanguageSlot)[1] = 1;
    ((signed char*)&self->mLanguageSlot)[2] = 2;
    cUIObj_188F28* uiObj = (cUIObj_188F28*)cUIScreen_getObjectByHashName((char*)self->mpScreen, GetHashValue32(D_004A14F8));
    cUIScreen_getObjectByHashName((char*)self->mpScreen, GetHashValue32(D_004A1500));
    uiObj->setVisible(0);
    uiObj->setStatic(1);
    void* menu = cUIScreen_getObjectByHashName((char*)self->mpScreen, GetHashValue32(str_Menu));
    if (menu != 0)
        cUIMenu_setSelectedByIndex(menu, 1);
    if (((signed char*)&self->mLanguageSlot)[0] >= 0) {
        void* w = find_188F28(((sOptGame_188F28*)self), D_00440FD8, 0);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGAmerican), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGFrench), 1);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGGerman), 2);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGSpanish), 3);
            unsigned char sel;
            if (cUIListBox_getEntryByData(w, iface->getOptions().iLanguage, &sel))
                cUIListBox_setCurrentEntryByIndex(w, sel);
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FB8, 0);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[0]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FF8, 0);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FE8[0]));
        }
    }
    if (((signed char*)&self->mLanguageSlot)[1] >= 0) {
        void* w = find_188F28(((sOptGame_188F28*)self), D_00440FD8, 1);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNMPH), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNKPH), 0);
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().iUseMetricUnits);
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FB8, 1);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[1]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FF8, 1);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FE8[1]));
        }
    }
    if (((signed char*)&self->mLanguageSlot)[2] >= 0) {
        void* w = find_188F28(((sOptGame_188F28*)self), D_00440FD8, 2);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_19Widescreen169), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_19WidescreenAnimorphic), 0);
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().iUseWidescreen);
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FB8, 2);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[2]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(((sOptGame_188F28*)self), D_00440FF8, 2);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FE8[2]));
        }
    }
}
#endif

extern "C" void* cUIState_onGainFocus(void* self);

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the game-options state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateOptionsGame_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBEOptionInterface_defaultGame(void* iface);
extern "C" void cFEStateOptionsGame_updateWidget(cFEStateOptionsGame* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0029ED90(void* p, int a);
extern char str_111resetopts[];
extern char str_24screen_position[];
extern int D_004A14D0;

struct sGameCfg_189610 {
    unsigned int flags;     // 0x0
    signed char f4;
    signed char f5;         // 0x5
    signed char f6;         // 0x6
    char pad7[0x288 - 7];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sGameCfg_189610 D_00535610_189610 __asm__("cBEOptionInterface_mOption");

struct sVE_189610a {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sVE_189610b {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};
struct sVE_189610c {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline int Is_189610(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline int GetOpt_189610(char* w, int i)
{
    sVE_189610a* vt = *(sVE_189610a**)(w + 8);
    return vt[23].fn(w + vt[23].delta, i);
}

static inline sGameCfg_189610 Cfg_189610()
{
    return D_00535610_189610;
}

/**
 * Handles the results of the game options screen's popups: reset to defaults, screen position and video calibration.
 *
 * Event 21 does nothing. On event 22 the popup's answer is its virtual slot 23 (getData) with 2, nonzero meaning
 * accepted, and the popup is told apart by its mHashName (+0xC):
 * - "111resetopts" accepted: resets the game options to their defaults (cBEOptionInterface_defaultGame), sets the changed flag
 *   D_004A14D0, refreshes the widgets (cFEStateOptionsGame_updateWidget) and reloads the language audio (func_0029ED90 on cAudioInterface_bxAudio's
 *   gSSXAudio, restart 1).
 * - "24screen_position" accepted: stores the popup's getData(0) and getData(1) into bytes 6 and 5 (f6, f5) of
 *   cBEOptionInterface_mOption, the horizontal and vertical screen offsets, and sets D_004A14D0. Declined: sends the
 *   stored f6 and f5 back to GrMan's virtual slot 12.
 * - 27 (the cFEPopupVideoCalibration popup) accepted: the same reset as "111resetopts" without the audio reload.
 *
 * After any event 22 it calls virtual slot 1 of the option interface (cBE interface 4).
 *
 * @param popup  the popup state; its mHashName (+0xC) says which popup it is
 * @param event  popup event code (22 is handled; 21 and the rest are ignored)
 */
extern "C" void cFEStateOptionsGame_onPopupEvent(cFEStateOptionsGame* self, cUIState* popup, int event)
{
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 21:
        break;
    case 22:
        if (Is_189610(popup->mHashName, str_111resetopts)) {
            if (GetOpt_189610(((char*)popup), 2)) {
                cBEOptionInterface_defaultGame(iface);
                D_004A14D0 = 1;
                cFEStateOptionsGame_updateWidget(self);
                func_0029ED90(cAudioInterface_bxAudio(), 1);
            }
        } else if (Is_189610(popup->mHashName, str_24screen_position)) {
            if (GetOpt_189610(((char*)popup), 2)) {
                int hzOffset = GetOpt_189610(((char*)popup), 0);
                int vtOffset = GetOpt_189610(((char*)popup), 1);
                sGameCfg_189610* gameOptions = &D_00535610_189610;
                D_004A14D0 = 1;
                gameOptions->f6 = hzOffset;
                gameOptions->f5 = vtOffset;
            } else {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                sVE_189610b* vt = (sVE_189610b*)ps2GraphicsMan->mVtbl;
                char* thisp = (char*)ps2GraphicsMan + vt[12].delta;
                void (**pf)(void*, int, int) = &vt[12].fn;
                (*pf)(thisp, Cfg_189610().f6, Cfg_189610().f5);
            }
        } else if (popup->mHashName == 27) {
            if (GetOpt_189610(((char*)popup), 2)) {
                cBEOptionInterface_defaultGame(iface);
                D_004A14D0 = 1;
                cFEStateOptionsGame_updateWidget(self);
            }
        }
        {
            sVE_189610c* vt = *(sVE_189610c**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters menu events on the game options screen: returns 0x100 for event 9 and 0x101 for every other event.
 *
 * The menu index the caller passes as a fourth argument is not declared.
 *
 * @param menu  the menu (unused)
 * @param event  menu event code
 * @return 0x100 or 0x101 (cUIMenu reply codes)
 */
extern "C" int cFEStateOptionsGame_onWidgetMenu(cFEStateOptionsGame* self, int menu, int event)
{
    return event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onWidgetPair);
#ifdef SKIP_ASM
/**
 * Returns 0x100 for any nonzero pair event on the game options screen, and 0 for event 0.
 *
 * @param pair  the pair widget (unused)
 * @param event  pair event code
 * @return 0x100 when @p event is nonzero, else 0
 */
extern "C" int cFEStateOptionsGame_onWidgetPair(cFEStateOptionsGame* self, int pair, int event)
{
    return event ? 0x100 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cSSXApp_activateBEOptions(void* p);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0029ED90(void* p, int a);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern char str_cFEPopupScreenPos[];
extern char str_cFEPopupVideoCalibration[];
extern void* cFEPopupScreenPos_vtbl[];
extern void* cFEPopupVideoCalibration_vtbl[];
extern void* cFEPopupOptionReset_vtbl[];
extern char D_004A1520[];
extern char D_004A1528[];
extern char* D_00440FD8[];
extern int D_004A14D0;

// cBEOptionInterface_mOption's first word (field names from On Tour's tBEOptionDB bitfields).
struct sGameCfg_189980 {
    unsigned int pad0 : 19;
    bool iUseMetricUnits : 1;           // bit 19: MPH (0) / KPH (1)
    unsigned int iUseWidescreen : 2;       // bits 20-21: off / 16:9 / anamorphic
    unsigned int iLanguage : 3;      // bits 22-24 (FELocale's language)
    unsigned int pad25 : 7;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sGameCfg_189980 D_00535610_189980 __asm__("cBEOptionInterface_mOption");

struct sVE_189980a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_189980b {
    short delta;
    short index;
    void (*fn)(void*, char);
};
struct sVE_189980c {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sOptGame_189980 {
    char pad0[0x10];
    char* engine;           // 0x10
    char pad14[0x40 - 0x14];
    char* screen;           // 0x40
    char pad44[0x48 - 0x44];
    signed char item[3];    // 0x48
};

static inline int Is_189980(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline void Refresh_189980(sOptGame_189980* self)
{
    char* scr = self->screen;
    sVE_189980a* vt = *(sVE_189980a**)(scr + 0x48);
    char* sub = scr + 0x40;
    vt[16].fn(sub + vt[16].delta);
}

/**
 * Handles a widget event on the game options screen.
 *
 * Event 7 pushes a cFEPopupOptionReset popup. Event 5 opens cFEPopupScreenPos (entry "6") or
 * cFEPopupVideoCalibration (entry "7"). Event 6 (back) applies the options (interface 4 slot 1,
 * cSSXApp_activateBEOptions), sets the locale's language, refreshes and pushes the state from the engine's
 * custom interface (engine + 0x0) virtual slot 5, probably getPrevState.
 *
 * Event 9 (a list changed) stores the language, speed-unit or widescreen choice into the options block
 * cBEOptionInterface_mOption (the language also goes to the locale), re-applies the options and sets the changed flag
 * D_004A14D0. A null widget is ignored.
 *
 * @param widget  the widget the event is for (null: nothing happens)
 * @param event  UI event code (5, 6, 7 or 9 are handled)
 */
extern "C" void cFEStateOptionsGame_onWidgetEvent(cFEStateOptionsGame* self, cUIThing* widget, unsigned int event)
{
    if (((char*)widget) == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 7: {
        char* popup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        cUIState_cUIState1(popup, (char*)self->mpEngine, ((sOptGame_189980*)self));
        *(void***)(popup + 8) = cFEPopupOptionReset_vtbl;
        popup[0x48] = 0;
        cUIStateStack_pushPopup(self->mpEngine->mStateStack, popup);
        break;
    }
    case 5:
        if (Is_189980(widget->mHashName, D_004A1520)) {
            char* popup = (char*)operator new(84, str_cFEPopupScreenPos, 0x100, 0);
            cUIState_cUIState1(popup, (char*)self->mpEngine, ((sOptGame_189980*)self));
            *(void***)(popup + 8) = cFEPopupScreenPos_vtbl;
            *(int*)(popup + 0x4C) = 0;
            *(int*)(popup + 0x48) = 0;
            popup[0x50] = 0;
            cUIStateStack_pushPopup(self->mpEngine->mStateStack, popup);
        } else if (Is_189980(widget->mHashName, D_004A1528)) {
            char* popup = (char*)operator new(76, str_cFEPopupVideoCalibration, 0x100, 0);
            cUIState_cUIState1(popup, (char*)self->mpEngine, ((sOptGame_189980*)self));
            *(void***)(popup + 8) = cFEPopupVideoCalibration_vtbl;
            *(int*)(popup + 0xC) = 27;
            cUIStateStack_pushPopup(self->mpEngine->mStateStack, popup);
        }
        break;
    case 6: {
        {
            sVE_189980a* vt = *(sVE_189980a**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        cSSXApp_activateBEOptions(SSXApp);
        {
            char* locale = (char*)SSXApp->FELocale;
            sVE_189980b* vt = *(sVE_189980b**)(locale + 4);
            vt[3].fn(locale + vt[3].delta, D_00535610_189980.iLanguage);
        }
        Refresh_189980(((sOptGame_189980*)self));
        char* eng = *(char**)(char*)self->mpEngine;
        sVE_189980c* vt = *(sVE_189980c**)(eng + 4);
        void* nextState = vt[5].fn(eng + vt[5].delta, ((sOptGame_189980*)self), widget->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 9:
        if (((signed char*)&self->mLanguageSlot)[0] >= 0 && Is_189980(widget->mHashName, D_00440FD8[((signed char*)&self->mLanguageSlot)[0]])) {
            D_00535610_189980.iLanguage = cUIListBox_getCurrentEntryData(((char*)widget));
            char* locale = (char*)SSXApp->FELocale;
            sVE_189980b* vt = *(sVE_189980b**)(locale + 4);
            char* thisp = locale + vt[3].delta;
            vt[3].fn(thisp, cUIListBox_getCurrentEntryData(((char*)widget)));
            Refresh_189980(((sOptGame_189980*)self));
            func_0029ED90(cAudioInterface_bxAudio(), 1);
        }
        if (((signed char*)&self->mLanguageSlot)[1] >= 0 && Is_189980(widget->mHashName, D_00440FD8[((signed char*)&self->mLanguageSlot)[1]])) {
            bool on = *(unsigned char*)(((char*)widget) + 0x319);
            D_00535610_189980.iUseMetricUnits = on;
        }
        if (((signed char*)&self->mLanguageSlot)[2] >= 0 && Is_189980(widget->mHashName, D_00440FD8[((signed char*)&self->mLanguageSlot)[2]])) {
            D_00535610_189980.iUseWidescreen = *(unsigned char*)(((char*)widget) + 0x319);
        }
        cSSXApp_activateBEOptions(SSXApp);
        D_004A14D0 = 1;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsGame_updateWidget);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cSSXApp_activateBEOptions(void* p);
extern char* D_00440FD8[];

// cBEOptionInterface_mOption's first word (field names from On Tour's tBEOptionDB bitfields).
struct sGameCfg_189D60 {
    unsigned int pad0 : 19;
    unsigned int iUseMetricUnits : 1;       // bit 19: MPH (0) / KPH (1)
    unsigned int iUseWidescreen : 2;           // bits 20-21: off / 16:9 / anamorphic
    unsigned int iLanguage : 3;          // bits 22-24 (FELocale's language)
    unsigned int pad25 : 7;
    signed char f4;
    signed char f5;                 // 0x5
    signed char f6;                 // 0x6
    char pad7[0x288 - 7];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sGameCfg_189D60 D_00535610_189D60 __asm__("cBEOptionInterface_mOption");

struct sOptionIface_189D60 {
    sGameCfg_189D60 getOptions() { return D_00535610_189D60; }
};

struct sVE_189D60a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVE_189D60b {
    short delta;
    short index;
    void (*fn)(void*, char);
};
struct sVE_189D60c {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

struct sOptGame_189D60 {
    char pad0[0x40];
    char* screen;                   // 0x40
    char pad44[0x4];
    signed char item[3];            // 0x48
};

static inline void* find_189D60(sOptGame_189D60* self, int i)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00440FD8[self->item[i]]));
}

/**
 * Pushes the current game options into the game options screen's widgets and applies them.
 *
 * The three list boxes are found on mpScreen (+0x40) by the names D_00440FD8[item[i]] (item[0..2] at +0x48: language,
 * speed units and widescreen; a negative slot is skipped, as is a missing widget). Their current entries come from
 * cBEOptionInterface_mOption: item[0] from bits 22-24 (the view's `mode`), whose selected entry data is also passed to
 * SSXApp->FELocale's virtual slot 3 before virtual slot 16 of the screen's sub-object at +0x40 is called; item[1] from
 * bit 19 (iUseMetricUnits, the speed-unit choice); item[2]
 * from bits 20-21 (`f20`).
 *
 * It then sends the screen offsets f6 and f5 to GrMan's virtual slot 12 and calls cSSXApp_activateBEOptions.
 * cFEStateOptionsGame_onPopupEvent calls it after resetting the options to their defaults.
 *
 * @param self_  the cFEStateOptionsGame state
 */
extern "C" void cFEStateOptionsGame_updateWidget(cFEStateOptionsGame* self_)
{
    sOptGame_189D60* self = (sOptGame_189D60*)self_;
    sOptionIface_189D60* iface = (sOptionIface_189D60*)cBE_getInterface_Fv(cBE_getBE(), 4);
    if (self->item[0] >= 0) {
        void* w = find_189D60(self, 0);
        if (w != 0) {
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().iLanguage);
            char* locale = (char*)SSXApp->FELocale;
            sVE_189D60b* vt = *(sVE_189D60b**)(locale + 4);
            vt[3].fn(locale + vt[3].delta, cUIListBox_getCurrentEntryData(w));
            char* scr = self->screen;
            sVE_189D60a* vt2 = *(sVE_189D60a**)(scr + 0x48);
            char* sub = scr + 0x40;
            vt2[16].fn(sub + vt2[16].delta);
        }
    }
    if (self->item[1] >= 0) {
        void* w = find_189D60(self, 1);
        if (w != 0)
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().iUseMetricUnits);
    }
    if (self->item[2] >= 0) {
        void* w = find_189D60(self, 2);
        if (w != 0)
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().iUseWidescreen);
    }
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_189D60c* vt = (sVE_189D60c*)ps2GraphicsMan->mVtbl;
    char* thisp = (char*)ps2GraphicsMan + vt[12].delta;
    void (**pf)(void*, int, int) = &vt[12].fn;
    (*pf)(thisp, iface->getOptions().f6, iface->getOptions().f5);
    cSSXApp_activateBEOptions(SSXApp);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A258);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateOptionsSound_vtbl[];
extern int D_004A14D4;

/**
 * Constructs the sound options state (tag "cFEStateOptionsSound", 72 bytes), which cFECustom_getNextState creates for
 * state 29.
 *
 * Runs cUIState's constructor cUIState_cUIState (the engine pointer from the second argument register goes straight
 * through; it is not declared here), stores cFEStateOptionsSound_vtbl in mVtbl (+0x8), sets mHashName (+0xC) to 29 and
 * clears the sound changed flag D_004A14D4. cFEStateOptionsSound_cFEStateOptionsSound is the variant built with a null
 * engine.
 *
 * @return @p self
 */
extern "C" void* func_0018A258(void* self)
{
    cUIState_cUIState(self);
    *(void***)((char*)self + 8) = cFEStateOptionsSound_vtbl;
    *(int*)((char*)self + 0xC) = 29;
    D_004A14D4 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateOptionsSound_updateWidget(cFEStateOptionsSound* self);
extern char str_141advsettings[];
extern char str_ongroup[];
extern char str_fegroup[];
extern char D_004A1528[];
extern int D_004A1A70;

class cUIObj_18A298 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Creates the sound options screen ("141advsettings") and fills its widgets.
 *
 * Calls cFEStateOptionsSound_updateWidget, then shows "ongroup" and entry "7" (disabled) when D_004A1A70 is
 * 1, and "fegroup" otherwise (D_004A1A70's meaning is unknown).
 */
extern "C" void cFEStateOptionsSound_onCreateScreen(cFEStateOptionsSound* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_141advsettings), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateOptionsSound_updateWidget(self);
    int on = D_004A1A70 == 1;
    cUIObj_18A298* onObj = (cUIObj_18A298*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ongroup));
    if (onObj != 0) {
        onObj->setVisible(on);
    }
    cUIObj_18A298* offObj = (cUIObj_18A298*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_fegroup));
    if (offObj != 0) {
        offObj->setVisible(on ^ 1);
    }
    cUIObj_18A298* item7 = (cUIObj_18A298*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A1528));
    if (item7 != 0) {
        item7->setVisible(on);
        item7->setStatic(on ^ 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBEOptionInterface_defaultSound(void* iface);
extern "C" void cFEStateOptionsSound_setSoundMode(cFEStateOptionsSound* self, unsigned int mode);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void cFEStateOptionsSound_updateWidget(cFEStateOptionsSound* self);
extern char str_111resetopts[];
extern int D_004A14D4;

struct sSndCfg_18A3D8 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int iUseArcadeSFX : 1;          // bit 16
    int iUseDJSpeech : 1;           // bit 17
    int pad18 : 14;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sSndCfg_18A3D8 D_00535610_18A3D8 __asm__("cBEOptionInterface_mOption");

struct sVE_18A3D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Is_18A3D8(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline int GetOpt_18A3D8(char* w, int i)
{
    sVE_18A3D8* vt = *(sVE_18A3D8**)(w + 8);
    return vt[23].fn(w + vt[23].delta, i);
}

static inline sSndCfg_18A3D8 Cfg_18A3D8()
{
    return D_00535610_18A3D8;
}

/**
 * Handles the sound options screen's reset popup: on acceptance it restores the default sound options and re-applies
 * them.
 *
 * Only event 22 from the popup hashed "111resetopts" does anything. When the popup's virtual slot 23 (getData) with 2
 * is nonzero it resets the sound options (cBEOptionInterface_defaultSound, the sound step of cBEOptionInterface_revert), switches the
 * speaker mode to bits 12-13 (`output`) with cFEStateOptionsSound_setSoundMode, re-applies the effects, speech and music volumes
 * (func_00287410, cSSXAudio_SetSFXVolume, func_00287520 on gSSXAudio), bit 17 (`iUseDJSpeech`) through cSSXAudio_SetDJSpeech and bit 16
 * (`iUseArcadeSFX`) through cSSXAudio_SetArcadeSFX, and refreshes the widgets with cFEStateOptionsSound_updateWidget.
 *
 * It sets the sound changed flag D_004A14D4 even when the reset is declined, unlike the game and HUD popup handlers.
 *
 * @param popup  the popup state; its mHashName (+0xC) is checked
 * @param event  popup event code (22 is handled)
 */
extern "C" void cFEStateOptionsSound_onPopupEvent(cFEStateOptionsSound* self, cUIState* popup, int event)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 21:
        break;
    case 22:
        if (Is_18A3D8(popup->mHashName, str_111resetopts)) {
            if (GetOpt_18A3D8(((char*)popup), 2)) {
                cBEOptionInterface_defaultSound(iface);
                cFEStateOptionsSound_setSoundMode(self, D_00535610_18A3D8.output);
                func_00287410(cAudioInterface_bxAudio(), Cfg_18A3D8().sfxVol);
                cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_18A3D8().speechVol);
                func_00287520(cAudioInterface_bxAudio(), Cfg_18A3D8().musicVol);
                cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_18A3D8().iUseDJSpeech != 0);
                cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_18A3D8().iUseArcadeSFX != 0);
                cFEStateOptionsSound_updateWidget(self);
            }
            D_004A14D4 = 1;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the sound-options state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateOptionsSound_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters menu events on the sound options screen: 0x100 for event 9, 0 for event 6, 0x101 for anything else.
 *
 * The menu index the caller passes as a fourth argument is not declared.
 *
 * @param menu  the menu (unused)
 * @param event  menu event code
 * @return 0x100, 0 or 0x101 (cUIMenu reply codes)
 */
extern "C" int cFEStateOptionsSound_onWidgetMenu(cFEStateOptionsSound* self, int menu, int event)
{
    switch (event) {
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onWidgetPair);
#ifdef SKIP_ASM
/**
 * Returns 0x100 for any nonzero pair event on the sound options screen, and 0 for event 0.
 *
 * @param pair  the pair widget (unused)
 * @param event  pair event code
 * @return 0x100 when @p event is nonzero, else 0
 */
extern "C" int cFEStateOptionsSound_onWidgetPair(cFEStateOptionsSound* self, int pair, int event)
{
    return event ? 0x100 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cFEStateOptionsSound_setSoundMode(cFEStateOptionsSound* self, unsigned int mode);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern char str_cFEPopupOptionReset[];
extern void* cFEPopupOptionReset_vtbl[];
extern char str_1lb[];
extern char str_2sl[];
extern char str_3sl[];
extern char str_4sl[];
extern char str_5lb[];
extern char str_6lb[];
extern char str_7lb[];
extern int D_004A1A70;
extern int D_004A14D4;

struct sSndCfg_18A8D0 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int iUseArcadeSFX : 1;          // bit 16
    int iUseDJSpeech : 1;           // bit 17
    int f18 : 1;                    // bit 18
    int pad19 : 13;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sSndCfg_18A8D0 D_00535610_18A8D0 __asm__("cBEOptionInterface_mOption");

struct sVE_18A8D0a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE_18A8D0c {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

static inline int Is_18A8D0(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline sSndCfg_18A8D0 Cfg_18A8D0()
{
    return D_00535610_18A8D0;
}


/**
 * Handles a widget event on the sound options screen: the reset popup, leaving the screen and value changes.
 *
 * A null widget is ignored, and event 5 does nothing. Event 7 pushes a cFEPopupOptionReset popup (vtable cFEPopupOptionReset_vtbl)
 * onto the engine's state stack. Event 6 (back) calls virtual slot 1 of the option interface (cBE interface 4),
 * re-applies the effects, speech and music volumes and the two switches (`iUseDJSpeech` through cSSXAudio_SetDJSpeech, `iUseArcadeSFX`
 * through cSSXAudio_SetArcadeSFX), then pushes the state that virtual slot 5 of the engine's custom interface (engine + 0x0)
 * returns for the widget's mMiscData.
 *
 * Event 9 (a value changed) stores the widget's value into cBEOptionInterface_mOption and applies it: "1lb" the speaker
 * mode `output` (cFEStateOptionsSound_setSoundMode), the sliders "2sl" `sfxVol` (func_00287410), "3sl" `speechVol` (cSSXAudio_SetSFXVolume) and
 * "4sl" `musicVol` (func_00287520) from the slider value at +0x74, "5lb" bit 17 (cSSXAudio_SetDJSpeech) and "6lb" bit
 * 16 (cSSXAudio_SetArcadeSFX); "7lb" sets bit 18 (`f18`) only while D_004A1A70 is 1. Every event 9 sets the sound changed flag
 * D_004A14D4.
 *
 * @param widget  the widget the event is for (null: nothing happens)
 * @param event  UI event code (6, 7 and 9 are handled)
 */
extern "C" void cFEStateOptionsSound_onWidgetEvent(cFEStateOptionsSound* self, cUIThing* widget, unsigned int event)
{
    if (((char*)widget) == 0)
        return;
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 5: // empty case; needed for the original compare-tree shape
        break;
    case 7: {
        char* resetPopup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        cUIState_cUIState1(resetPopup, (char*)self->mpEngine, (self));
        *(void***)(resetPopup + 8) = cFEPopupOptionReset_vtbl;
        resetPopup[0x48] = 0;
        cUIStateStack_pushPopup(self->mpEngine->mStateStack, resetPopup);
        break;
    }
    case 6: {
        {
            sVE_18A8D0a* vt = *(sVE_18A8D0a**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        func_00287410(cAudioInterface_bxAudio(), Cfg_18A8D0().sfxVol);
        cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_18A8D0().speechVol);
        func_00287520(cAudioInterface_bxAudio(), Cfg_18A8D0().musicVol);
        cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_18A8D0().iUseDJSpeech != 0);
        cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_18A8D0().iUseArcadeSFX != 0);
        char* eng = *(char**)(char*)self->mpEngine;
        sVE_18A8D0c* vt = *(sVE_18A8D0c**)(eng + 4);
        void* nextState = vt[5].fn(eng + vt[5].delta, (self), widget->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(self->mpEngine->mStateStack, nextState);
        break;
    }
    case 9:
        if (Is_18A8D0(widget->mHashName, str_1lb)) {
            D_00535610_18A8D0.output = cUIListBox_getCurrentEntryData(((char*)widget));
            cFEStateOptionsSound_setSoundMode(self, cUIListBox_getCurrentEntryData(((char*)widget)));
        }
        else if (Is_18A8D0(widget->mHashName, str_2sl)) {
            unsigned char vol = *(unsigned char*)(((char*)widget) + 0x74);
            D_00535610_18A8D0.sfxVol = vol;
            func_00287410(cAudioInterface_bxAudio(), Cfg_18A8D0().sfxVol);
        }
        else if (Is_18A8D0(widget->mHashName, str_3sl)) {
            unsigned char vol = *(unsigned char*)(((char*)widget) + 0x74);
            D_00535610_18A8D0.speechVol = vol;
            cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_18A8D0().speechVol);
        }
        else if (Is_18A8D0(widget->mHashName, str_4sl)) {
            unsigned char vol = *(unsigned char*)(((char*)widget) + 0x74);
            D_00535610_18A8D0.musicVol = vol;
            func_00287520(cAudioInterface_bxAudio(), Cfg_18A8D0().musicVol);
        }
        else if (Is_18A8D0(widget->mHashName, str_5lb)) {
            D_00535610_18A8D0.iUseDJSpeech = cUIListBox_getCurrentEntryData(((char*)widget)) != 0;
            cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_18A8D0().iUseDJSpeech != 0);
        }
        else if (Is_18A8D0(widget->mHashName, str_6lb)) {
            D_00535610_18A8D0.iUseArcadeSFX = cUIListBox_getCurrentEntryData(((char*)widget)) != 0;
            cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_18A8D0().iUseArcadeSFX != 0);
        }
        bool on = D_004A1A70 == 1;
        if (on && Is_18A8D0(widget->mHashName, str_7lb))
            D_00535610_18A8D0.f18 = cUIListBox_getCurrentEntryData(((char*)widget)) != 0;
        D_004A14D4 = 1;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_updateWidget);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int cUIListBox_addEntryByStringID_i(void* box, int id, int idx) __asm__("cUIListBox_addEntryByStringID");
extern "C" void cUIListBox_deleteAllEntries(void* box);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
extern "C" void* cAudioInterface_bxAudio();
int cSSXAudio_GetCurrentSoundMode(void* snd);
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern char str_kT_CMNOff[];
extern char str_kT_20SpeakerStereo[];
extern char str_kT_20SpeakerSurround[];
extern char str_kT_20SpeakerMono[];
extern char str_kT_20SpeakerDTS[];
extern char str_kT_CMNOn[];
extern char str_1lb[];
extern char str_2sl[];
extern char str_3sl[];
extern char str_4sl[];
extern char str_5lb[];
extern char str_6lb[];
extern char str_7lb[];
extern int D_004A1A70;

struct sSndCfg_18B380 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int iUseArcadeSFX : 1;          // bit 16
    int iUseDJSpeech : 1;           // bit 17
    int f18 : 1;                    // bit 18
    int pad19 : 13;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the unit; view bound by asm label
extern sSndCfg_18B380 D_00535610_18B380 __asm__("cBEOptionInterface_mOption");

// The options interface's view of the same settings word (unsigned flags).
struct sSndOpts_18B380 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    unsigned int iUseArcadeSFX : 1; // bit 16
    unsigned int iUseDJSpeech : 1;  // bit 17
    unsigned int f18 : 1;           // bit 18
    unsigned int pad19 : 13;
    char rest[0x288 - 4];
};

struct sOptionIface_18B380 {
    // PORT: reinterprets the settings global with unsigned flag bits
    sSndOpts_18B380 getOptions() { return *(sSndOpts_18B380*)&D_00535610_18B380; }
};

static inline sSndCfg_18B380 Cfg_18B380()
{
    return D_00535610_18B380;
}

struct sOptSound_18B380 {
    char pad0[0x40];
    char* screen;                   // 0x40
};

static inline void* Find_18B380(sOptSound_18B380* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
}

/**
 * Fills the sound options widgets from the current sound settings and applies them.
 *
 * "1lb" lists stereo/surround/mono/DTS and selects the sound manager's current mode (cSSXAudio_GetCurrentSoundMode); the
 * sliders "2sl", "3sl", "4sl" (range 12) show the effects, speech and music volumes and push them to the
 * sound manager (cAudioInterface_bxAudio); "5lb" and "6lb" are off/on switches applied the same way; "7lb" is filled
 * only when D_004A1A70 is 1.
 *
 * @param vself  the cFEStateOptionsSound state
 */
// Sound options screen: fills the option widgets from the current sound settings.
extern "C" void cFEStateOptionsSound_updateWidget(cFEStateOptionsSound* vself)
{
    sOptSound_18B380* self = (sOptSound_18B380*)vself;
    sOptionIface_18B380* iface = (sOptionIface_18B380*)cBE_getInterface_Fv(cBE_getBE(), 4);
    {
    char* w = (char*)Find_18B380(self, str_1lb);
    if (w != 0) {
        cUIListBox_deleteAllEntries(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerStereo), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerSurround), 1);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerMono), 2);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerDTS), 3);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, cSSXAudio_GetCurrentSoundMode(cAudioInterface_bxAudio()), &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_2sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().sfxVol;
        func_00287410(cAudioInterface_bxAudio(), Cfg_18B380().sfxVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_3sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().speechVol;
        cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_18B380().speechVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_4sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().musicVol;
        func_00287520(cAudioInterface_bxAudio(), Cfg_18B380().musicVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_5lb);
    if (w != 0) {
        cUIListBox_deleteAllEntries(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOn), 1);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, iface->getOptions().iUseDJSpeech, &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
        cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_18B380().iUseDJSpeech != 0);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_6lb);
    if (w != 0) {
        cUIListBox_deleteAllEntries(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOn), 1);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, iface->getOptions().iUseArcadeSFX, &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
        cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_18B380().iUseArcadeSFX != 0);
    }
    }
    bool on = D_004A1A70 == 1;
    if (on) {
        char* w = (char*)Find_18B380(self, str_7lb);
        if (w != 0) {
            cUIListBox_deleteAllEntries(w);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOn), 1);
            unsigned char sel;
            if (cUIListBox_getEntryByData(w, iface->getOptions().f18, &sel))
                cUIListBox_setCurrentEntryByIndex(w, sel);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSound_setSoundMode);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" int func_00287670(void* snd);
extern "C" int cSSXAudio_GetSFXVolume(void* snd);
extern "C" int func_002876D0(void* snd);
extern "C" void cSSXAudio_FrontEndPurge(void* snd);
extern "C" void SSXAUDIO_DeInit();
extern "C" void SSXAUDIO_Init(int mode);
extern "C" void func_00287410(void* snd, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void cSSXAudio_FrontEndLoad(void* snd);

struct sVE_18BEF8i {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sVE_18BEF8v {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Switches the audio output mode and, when the audio device accepts it, restarts the audio system in that mode.
 *
 * @p mode is mapped to an audio mode: 0 (stereo) to 1, 1 (surround) to 2, 2 (mono) to 0, 3 (DTS) to 3, anything else to
 * 1. The device object is reached from gSSXAudio (cAudioInterface_bxAudio) + 0x118 through two loads and then +0x1D8; its virtual
 * slot 2 is asked to take the audio mode, and only a return of 1 goes on.
 *
 * The restart saves the effects, speech and music levels (func_00287670, cSSXAudio_GetSFXVolume, func_002876D0, 0-11 scale), the
 * switches at gSSXAudio + 0x62B8 (cSSXAudio_SetDJSpeech's flag) and + 0x62B4 (cSSXAudio_SetArcadeSFX's flag) and the word at + 0x534;
 * calls cSSXAudio_FrontEndPurge, SSXAUDIO_DeInit and SSXAUDIO_Init(audio mode); re-applies the volumes (func_00287410,
 * cSSXAudio_SetSFXVolume, func_00287520), the two switches (cSSXAudio_SetDJSpeech, cSSXAudio_SetArcadeSFX) and cSSXAudio_FrontEndLoad; and
 * finally passes the saved + 0x534 word to virtual slot 4 of the sub-object at gSSXAudio + 0x118 (its vtable pointer is
 * at + 0x5558).
 *
 * @param mode  the speaker list choice ("1lb" entry data): 0 stereo, 1 surround, 2 mono, 3 DTS
 */
extern "C" void cFEStateOptionsSound_setSoundMode(cFEStateOptionsSound* self, unsigned int mode)
{
    int audioMode;
    switch (mode) {
    case 1:
        audioMode = 2;
        break;
    case 2:
        audioMode = 0;
        break;
    case 3:
        audioMode = 3;
        break;
    case 0:
    default:
        audioMode = 1;
        break;
    }
    char* obj = *(char**)(**(char***)((char*)cAudioInterface_bxAudio() + 0x118) + 0x1D8);
    sVE_18BEF8i* vt = *(sVE_18BEF8i**)(obj + 4);
    if (vt[2].fn(obj + vt[2].delta, audioMode) == 1) {
        float sfxVolume = func_00287670(cAudioInterface_bxAudio());
        float speechVolume = cSSXAudio_GetSFXVolume(cAudioInterface_bxAudio());
        float musicVolume = func_002876D0(cAudioInterface_bxAudio());
        int djSpeech = *(int*)((char*)cAudioInterface_bxAudio() + 0x62B8);
        int arcadeSfx = *(int*)((char*)cAudioInterface_bxAudio() + 0x62B4);
        int saved534 = *(int*)((char*)cAudioInterface_bxAudio() + 0x534);
        cSSXAudio_FrontEndPurge(cAudioInterface_bxAudio());
        SSXAUDIO_DeInit();
        SSXAUDIO_Init(audioMode);
        func_00287410(cAudioInterface_bxAudio(), (int)sfxVolume);
        cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), (int)speechVolume);
        func_00287520(cAudioInterface_bxAudio(), (int)musicVolume);
        cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), djSpeech);
        cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), arcadeSfx);
        cSSXAudio_FrontEndLoad(cAudioInterface_bxAudio());
        char* snd = (char*)cAudioInterface_bxAudio();
        sVE_18BEF8v* vt2 = *(sVE_18BEF8v**)(snd + 0x5558);
        char* sub = snd + 0x118;
        vt2[4].fn(sub + vt2[4].delta, saved534);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C0E8);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateOptionsHUD_vtbl[];
extern int D_004A14D8;

/**
 * Constructs the HUD options state (tag "cFEStateOptionsHUD", 72 bytes), which cFECustom_getNextState creates for state
 * 30.
 *
 * Runs cUIState's constructor cUIState_cUIState (the engine pointer from the second argument register goes straight
 * through; it is not declared here), stores the vtable cFEStateOptionsHUD_vtbl in mVtbl (+0x8), sets mHashName (+0xC) to 0x1E and
 * clears the HUD changed flag D_004A14D8. cFEStateOptionsHUD_cFEStateOptionsHUD is the variant built with a null
 * engine.
 *
 * @return @p self
 */
extern "C" void* func_0018C0E8(void* self)
{
    cUIState_cUIState(self);
    *(void***)((char*)self + 8) = cFEStateOptionsHUD_vtbl;
    *(int*)((char*)self + 0xC) = 0x1E;
    D_004A14D8 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_21hud_opt[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateOptionsHUD_updateWidget(cFEStateOptionsHUD* self);

/**
 * Creates the HUD options screen ("21hud_opt"), plays its frame 0 and fills its menu with cFEStateOptionsHUD_updateWidget.
 *
 * The screen comes from cUIEngine_addScreenByHashName on the engine (+0x10) and is stored in mpScreen (+0x40); a
 * missing screen is not played, but cFEStateOptionsHUD_updateWidget runs either way.
 */
extern "C" void cFEStateOptionsHUD_onCreateScreen(cFEStateOptionsHUD* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_21hud_opt), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateOptionsHUD_updateWidget(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the HUD options state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateOptionsHUD_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_onPopupEvent);
#ifdef SKIP_ASM
struct sVE_18C1B8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
int GetHashValue32(char* str);
extern char str_111resetopts[];
extern int D_004A14D8;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBEOptionInterface_defaultHUD(void* self);
extern "C" void cFEStateOptionsHUD_updateWidget(cFEStateOptionsHUD* self);

/**
 * Handles the HUD options screen's reset popup: on acceptance it restores the default HUD options.
 *
 * Only event 22 from the popup hashed "111resetopts" does anything: when the popup's virtual slot 23 (getData) with 2
 * is nonzero it resets the HUD options (cBEOptionInterface_defaultHUD, the HUD step of cBEOptionInterface_revert, on cBE interface 4),
 * refreshes the menu (cFEStateOptionsHUD_updateWidget) and sets the HUD changed flag D_004A14D8.
 *
 * @param widget  the popup state (despite the name); its mHashName (+0xC) is checked
 * @param msg  popup event code (22 is handled)
 */
extern "C" void cFEStateOptionsHUD_onPopupEvent(cFEStateOptionsHUD* self, cUIState* widget, int msg)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (msg) {
    case 21:
        break;
    case 22: {
        int popupHash = widget->mHashName;
        if (popupHash == GetHashValue32(str_111resetopts)) {
            sVE_18C1B8* getDataEntry = &((sVE_18C1B8*)widget->mVtbl)[23];
            if (getDataEntry->fn((char*)widget + getDataEntry->delta, 2) != 0) {
                cBEOptionInterface_defaultHUD(iface);
                cFEStateOptionsHUD_updateWidget(self);
                D_004A14D8 = 1;
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters menu events on the HUD options screen: 0x100 for event 9, 0 for event 6, 0x101 for anything else.
 *
 * The menu index the caller passes as a fourth argument is not declared.
 *
 * @param menu  the menu (unused)
 * @param event  menu event code
 * @return 0x100, 0 or 0x101 (cUIMenu reply codes)
 */
extern "C" int cFEStateOptionsHUD_onWidgetMenu(cFEStateOptionsHUD* self, int menu, int event)
{
    switch (event) {
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern void* cFEPopupOptionReset_vtbl[];
extern int D_004A14D8;

struct sOptions_C2A0 {
    unsigned int pad0 : 30;
    unsigned int mode : 2;
    int data[(0x288 - 4) / 4];
};
extern sOptions_C2A0 D_00535610_C2A0 __asm__("cBEOptionInterface_mOption");

struct sVE_18C2A0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sVE_18C2A0n {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE_18C2A0p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

static inline int Is_0018C2A0(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Handles a widget event on the HUD options screen: the reset popup, leaving the screen and highlighting a HUD choice.
 *
 * A null widget is ignored. Event 7 pushes a cFEPopupOptionReset popup (vtable cFEPopupOptionReset_vtbl). Event 6 (back) calls
 * virtual slot 1 of the option interface (cBE interface 4) and pushes the state that virtual slot 5 of the engine's
 * custom interface (engine + 0x0) returns for the widget's mMiscData.
 *
 * Event 1 (item focused) shows the item (its virtual slot 9, setVisible, with 1), sets the HUD changed flag D_004A14D8
 * and stores the item's mMiscData (+0x18, read as a byte) into bits 30-31 of cBEOptionInterface_mOption (the view's
 * `mode`). Event 2 (focus lost) hides the item when it is "1", "2" or "3" (D_004A14F8, D_004A1500, D_004A1508).
 *
 * @param widget  the widget the event is for (null: nothing happens)
 * @param event  UI event code (1, 2, 6 and 7 are handled)
 */
extern "C" void cFEStateOptionsHUD_onWidgetEvent(cFEStateOptionsHUD* self, cUIThing* widget, unsigned int event)
{
    if (((char*)widget) == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 7: {
        char* resetPopup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        cUIState_cUIState1(resetPopup, self->mpEngine, ((char*)self));
        *(void***)(resetPopup + 8) = cFEPopupOptionReset_vtbl;
        *(char*)(resetPopup + 0x48) = 0;
        cUIStateStack_pushPopup(self->mpEngine->mStateStack, resetPopup);
        break;
    }
    case 6: {
        sVE_18C2A0n* vt = *(sVE_18C2A0n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        char* obj = *(char**)self->mpEngine;
        sVE_18C2A0p* vt2 = *(sVE_18C2A0p**)(obj + 4);
        void* nextState = vt2[5].fn(obj + vt2[5].delta, ((char*)self), widget->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 1: {
        sVE_18C2A0* vt = (sVE_18C2A0*)widget->mVtbl;
        vt[9].fn(((char*)widget) + vt[9].delta, 1);
        D_004A14D8 = 1;
        D_00535610_C2A0.mode = *(unsigned char*)&widget->mMiscData;
        break;
    }
    case 2:
        if (Is_0018C2A0(widget->mHashName, D_004A14F8) || Is_0018C2A0(widget->mHashName, D_004A1500) ||
            Is_0018C2A0(widget->mHashName, D_004A1508)) {
            sVE_18C2A0* vt = (sVE_18C2A0*)widget->mVtbl;
            vt[9].fn(((char*)widget) + vt[9].delta, 0);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsHUD_updateWidget);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern char str_Menu[];

struct sName_C478 {
    char s[2];
};
extern char D_004A14F8[];

struct sOptions_C478 {
    unsigned int pad0 : 30;
    unsigned int mode : 2;
    int data[(0x288 - 4) / 4];
};
extern sOptions_C478 D_00535610_C478 __asm__("cBEOptionInterface_mOption");

static inline int optMode_C478(sOptions_C478 o)
{
    return o.mode;
}

struct sVE_18C478h {
    short delta;
    short index;
    char* (*fn)(void*, int);
};

struct sVE_18C478v {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Fills the HUD options menu: tags items "1" to "3" with their index and shows and selects the one for the current HUD
 * setting.
 *
 * Finds "Menu" on mpScreen (+0x40) and looks up items "1", "2", "3" with the menu's virtual slot 14
 * (getObjectByHashName). Each item found gets its index (0-2) in mMiscData (+0x18); the item whose index equals bits
 * 30-31 of cBEOptionInterface_mOption (the view's `mode`) is shown (virtual slot 9, setVisible) and selected
 * (cUIMenu_setSelectedByIndex), the others are hidden. Finally it clears bit 0 of the menu's mAttr (+0x14; mOrientation
 * in On Tour's tAttrThing). The option interface (cBE interface 4) is fetched but not used.
 *
 * @param vself  the HUD options state
 */
extern "C" void cFEStateOptionsHUD_updateWidget(cFEStateOptionsHUD* vself)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sName_C478 name = *(sName_C478*)D_004A14F8;
    char* menu = (char*)cUIScreen_getObjectByHashName(vself->mpScreen, GetHashValue32(str_Menu));
    if (menu != 0) {
        int i;
        for (i = 0; i < 3; i++) {
            name.s[0] = '1' + i;
            sVE_18C478h* vt = *(sVE_18C478h**)(menu + 8);
            char* item = vt[14].fn(menu + vt[14].delta, GetHashValue32(name.s));
            if (item != 0) {
                *(int*)(item + 0x18) = i;
                if (i == optMode_C478(D_00535610_C478)) {
                    sVE_18C478v* vt2 = *(sVE_18C478v**)(item + 8);
                    vt2[9].fn(item + vt2[9].delta, 1);
                    cUIMenu_setSelectedByIndex(menu, i);
                } else {
                    sVE_18C478v* vt2 = *(sVE_18C478v**)(item + 8);
                    vt2[9].fn(item + vt2[9].delta, 0);
                }
            }
        }
        *(int*)(menu + 0x14) &= ~1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C658);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateOptionsController_vtbl[];
extern int D_004A14DC;

/**
 * Constructs the controller options state (tag "cFEStateOptionsController", 84 bytes), which cFECustom_getNextState
 * creates for state 31.
 *
 * Runs cUIState's constructor cUIState_cUIState (the engine pointer from the second argument register goes straight
 * through; it is not declared here), stores cFEStateOptionsController_vtbl in mVtbl (+0x8), sets mHashName (+0xC) to
 * 0x1F, clears the byte at +0x50 (the pads whose test vibration is running) and clears the controller changed flag
 * D_004A14DC.
 *
 * @return @p self
 */
extern "C" void* func_0018C658(cFEStateOptionsController* self)
{
    cUIState_cUIState(self);
    self->mVtbl = cFEStateOptionsController_vtbl;
    self->mHashName = 0x1F;
    self->mVibrateOn = 0;
    D_004A14DC = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cUIListBox_addEntryByStringID(void* box, int id, int idx);
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y);
extern "C" void cFEStateOptionsController_updateWidget(cFEStateOptionsController* self);
extern char str_22control[];
extern char str_1lb[];
extern char str_2lb[];
extern char str_3lb[];
extern char str_4lb[];
extern char str_kT_CMNOff[];
extern char str_kT_CMNOn[];
extern char str_kT_20PresetDefault[];
extern char str_kT_OVRCMNPro[];

/**
 * Creates the controller options screen ("22control") and fills its lists.
 *
 * Clears +0x48 and +0x4C, gives "1lb" and "2lb" off/on entries and "3lb"/"4lb" default/pro preset entries,
 * calls cFEStateOptionsController_vibratePad(self, pad, 0, 0) for pads 0 and 1 (probably vibratePad, stopping vibration) and then
 * cFEStateOptionsController_updateWidget (probably updateWidget).
 */
extern "C" void cFEStateOptionsController_onCreateScreen(cFEStateOptionsController* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_22control), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mTimer[1] = 0;
    self->mTimer[0] = 0;
    void* listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOn), 1);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_2lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOn), 1);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_3lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_20PresetDefault), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_OVRCMNPro), 1);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_4lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_20PresetDefault), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_OVRCMNPro), 1);
    }
    float zero = 0.0f;
    cFEStateOptionsController_vibratePad(self, 0, zero, zero);
    cFEStateOptionsController_vibratePad(self, 1, zero, zero);
    cFEStateOptionsController_updateWidget(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y);
void* cUIState_onDestroyScreen(void* self);

/**
 * Sets both controllers' output channels to zero, then destroys the controller-options state's screens.
 *
 * Calls cFEStateOptionsController_vibratePad(self, pad, 0, 0) for pads 0 and 1; for a pad whose device reports state 2 that sets both
 * of its output channels to 0, which probably stops any vibration. cUIState_onDestroyScreen() then destroys the
 * state's screen list.
 *
 * @return no meaningful value (cUIState_onDestroyScreen() returns nothing)
 */
extern "C" void* cFEStateOptionsController_onDestroyScreen(cFEStateOptionsController* self)
{
    cFEStateOptionsController_vibratePad(self, 0, 0.0f, 0.0f);
    cFEStateOptionsController_vibratePad(self, 1, 0.0f, 0.0f);
    return cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_defaultCtrl(void* self);
extern "C" void cFEStateOptionsController_updateWidget(cFEStateOptionsController* self);
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y);
extern char str_111resetopts[];
extern int D_004A14DC;

static inline int Is_0018C910(int id, char* s) { return id == GetHashValue32(s); }

class cUIObj_18C910 {
public:
    int pad[2];
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
    virtual int getData(int a);
};

/**
 * Handles the controller options screen's popups: stops both pads' vibration when one opens and restores the default
 * controller settings on an accepted reset.
 *
 * Event 21 calls cFEStateOptionsController_vibratePad(self, pad, 0, 0) for pads 0 and 1, setting both of their motor levels to 0. Event 22
 * from the popup hashed "111resetopts", when its virtual slot 23 (getData) with 2 is nonzero, resets the controller
 * settings (cBENewPlayerInterface_defaultCtrl on the player interface, cBE interface 1), refreshes the widgets
 * (cFEStateOptionsController_updateWidget) and sets the controller changed flag D_004A14DC.
 *
 * @param obj  the popup state; its mHashName (+0xC) is checked
 * @param msg  popup event code (21 and 22 are handled)
 */
extern "C" void cFEStateOptionsController_onPopupEvent(cFEStateOptionsController* self, cUIObj_18C910* obj, int msg)
{
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    switch (msg) {
    case 21: {
        float zero = 0.0f;
        cFEStateOptionsController_vibratePad(self, 0, zero, zero);
        cFEStateOptionsController_vibratePad(self, 1, zero, zero);
        break;
    }
    case 22:
        if (Is_0018C910(*(int*)((char*)obj + 0xC), str_111resetopts)) {
            if (obj->getData(2) != 0) {
                cBENewPlayerInterface_defaultCtrl(pi);
                cFEStateOptionsController_updateWidget(self);
                D_004A14DC = 1;
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onWidgetEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cBENewPlayerInterface_setUseDualShock(void* self, int index, int value);
extern "C" void cBENewPlayerInterface_setControllerMapping(void* self, int index, int value);
extern "C" int cBENewPlayerInterface_getUseDualShock(void* iface, int a1);
extern "C" void cFEStateOptionsController_displayPadConfig(cFEStateOptionsController* self, int on);
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y);
extern char str_cFEPopupOptionReset[];
extern void* cFEPopupOptionReset_vtbl[];
extern char str_1lb[];
extern char str_2lb[];
extern char str_3lb[];
extern char str_4lb[];
extern int D_004A14DC;

struct sVE_18CA10n {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE_18CA10p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

static inline int Is_0018CA10(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Handles a widget event on the controller options screen: the reset popup, leaving the screen and setting changes.
 *
 * A null widget is ignored. Event 7 pushes a cFEPopupOptionReset popup (vtable cFEPopupOptionReset_vtbl). Event 6 (back) calls
 * virtual slot 1 of the player interface (cBE interface 1) and pushes the state that virtual slot 5 of the engine's
 * custom interface (engine + 0x0) returns for the widget's mMiscData.
 *
 * Event 9 (a value changed): "1lb" and "2lb" set pad 0's or pad 1's vibration flag (cBENewPlayerInterface_setUseDualShock) to the chosen entry;
 * when the flag reads back set (cBENewPlayerInterface_getUseDualShock) the pad buzzes (cFEStateOptionsController_vibratePad with 10, 10), its timer (+0x48 for pad 0,
 * +0x4C for pad 1) is set to 60 and its bit (1 or 2) is set in +0x50 so cFEStateOptionsController_onUpdate stops it later; otherwise its
 * motors are set to 0. "3lb" and "4lb" store pad 0's or pad 1's button layout (cBENewPlayerInterface_setControllerMapping) and show the default or
 * pro layout picture (cFEStateOptionsController_displayPadConfig with entry data == 0). Every event 9 sets the controller changed flag D_004A14DC.
 *
 * @param widget  the widget the event is for (null: nothing happens)
 * @param event  UI event code (6, 7 and 9 are handled)
 */
extern "C" void cFEStateOptionsController_onWidgetEvent(cFEStateOptionsController* self, cUIThing* widget, unsigned int event)
{
    if (((char*)widget) == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    switch (event) {
    case 7: {
        char* resetPopup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        cUIState_cUIState1(resetPopup, self->mpEngine, ((char*)self));
        *(void***)(resetPopup + 8) = cFEPopupOptionReset_vtbl;
        *(char*)(resetPopup + 0x48) = 0;
        cUIStateStack_pushPopup((char*)self->mpEngine + 0x18, resetPopup);
        break;
    }
    case 6: {
        sVE_18CA10n* vt = *(sVE_18CA10n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        char* obj = *(char**)self->mpEngine;
        sVE_18CA10p* vt2 = *(sVE_18CA10p**)(obj + 4);
        void* nextState = vt2[5].fn(obj + vt2[5].delta, ((char*)self), widget->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        }
        break;
    }
    case 9:
        if (Is_0018CA10(widget->mHashName, str_1lb)) {
            cBENewPlayerInterface_setUseDualShock(iface, 0, cUIListBox_getCurrentEntryData(((char*)widget)) != 0);
            if (cBENewPlayerInterface_getUseDualShock(iface, 0)) {
                cFEStateOptionsController_vibratePad(self, 0, 10.0f, 10.0f);
                self->mTimer[0] = 0x3C;
                *(unsigned char*)&self->mVibrateOn |= 1;
            } else {
                cFEStateOptionsController_vibratePad(self, 0, 0.0f, 0.0f);
            }
        } else if (Is_0018CA10(widget->mHashName, str_2lb)) {
            cBENewPlayerInterface_setUseDualShock(iface, 1, cUIListBox_getCurrentEntryData(((char*)widget)) != 0);
            if (cBENewPlayerInterface_getUseDualShock(iface, 1)) {
                cFEStateOptionsController_vibratePad(self, 1, 10.0f, 10.0f);
                self->mTimer[1] = 0x3C;
                *(unsigned char*)&self->mVibrateOn |= 2;
            } else {
                cFEStateOptionsController_vibratePad(self, 1, 0.0f, 0.0f);
            }
        } else if (Is_0018CA10(widget->mHashName, str_3lb)) {
            cBENewPlayerInterface_setControllerMapping(iface, 0, cUIListBox_getCurrentEntryData(((char*)widget)));
            cFEStateOptionsController_displayPadConfig(self, cUIListBox_getCurrentEntryData(((char*)widget)) == 0);
        } else if (Is_0018CA10(widget->mHashName, str_4lb)) {
            cBENewPlayerInterface_setControllerMapping(iface, 1, cUIListBox_getCurrentEntryData(((char*)widget)));
            cFEStateOptionsController_displayPadConfig(self, cUIListBox_getCurrentEntryData(((char*)widget)) == 0);
        }
        D_004A14DC = 1;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onWidgetMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getControllerMapping(void* player, int which);
// cFEStateOptionsController_displayPadConfig returns void; declaring it int ($v0 set by the call) reproduces the original argument scheduling.
extern "C" int cFEStateOptionsController_displayPadConfig_CCE0(void* self, int on) __asm__("cFEStateOptionsController_displayPadConfig");

/**
 * Filters menu keys on the controller options screen; accepting a button-layout row shows that pad's layout picture.
 *
 * Key 9 gives 0x100 and key 6 gives 0. Key 5 on menu item 2 or 3 (the menu's mScrollItemCurrent, +0x95) calls
 * cFEStateOptionsController_displayPadConfig with whether pad 0 or pad 1 uses layout 0 (cBENewPlayerInterface_getControllerMapping) and gives 0x101;
 * it changes no setting. Every other key, and key 5 on other items, gives 0x101.
 *
 * @param menu  the menu; its mScrollItemCurrent (+0x95) is the highlighted item
 * @param key  menu event code
 * @return 0x100, 0 or 0x101 (cUIMenu reply codes)
 */
extern "C" int cFEStateOptionsController_onWidgetMenu(cFEStateOptionsController* self, char* menu, unsigned int key)
{
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    switch (key) {
    case 9:
        return 0x100;
    case 5: {
        int which;
        switch (*(unsigned char*)(menu + 0x95)) {
        case 2:
            which = 0;
            break;
        case 3:
            which = 1;
            break;
        default:
            return 257;
        }
        cFEStateOptionsController_displayPadConfig_CCE0(self, cBENewPlayerInterface_getControllerMapping(player, which) == 0);
        return 257;
    }
    case 6:
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onWidgetPair);
#ifdef SKIP_ASM
/**
 * Returns 0x100 for any nonzero pair event on the controller options screen, and 0 for event 0.
 *
 * @param pair  the pair widget (unused)
 * @param event  pair event code
 * @return 0x100 when @p event is nonzero, else 0
 */
extern "C" int cFEStateOptionsController_onWidgetPair(cFEStateOptionsController* self, int pair, int event)
{
    return event ? 0x100 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_updateWidget);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getUseDualShock(void* iface, int a1);
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int a1);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern "C" void cFEStateOptionsController_displayPadConfig(cFEStateOptionsController* self, int on);
extern char str_1lb[];
extern char str_2lb[];
extern char str_3lb[];
extern char str_4lb[];

/**
 * Sets the controller options lists from the current player settings.
 *
 * On mpScreen (+0x40), "1lb" and "2lb" get pad 0's and pad 1's vibration flag (cBENewPlayerInterface_getUseDualShock), "3lb" and "4lb" their
 * button layout (cBENewPlayerInterface_getControllerMapping on cBE interface 1); each list selects the entry whose data
 * matches (cUIListBox_getEntryByData, cUIListBox_setCurrentEntryByIndex). With "3lb" present it also shows pad 0's
 * layout picture (cFEStateOptionsController_displayPadConfig with layout == 0). Missing lists are skipped.
 *
 * Likely bug: the result of cUIListBox_getEntryByData is not checked, so a value no entry matches selects an
 * uninitialised index.
 */
extern "C" void cFEStateOptionsController_updateWidget(cFEStateOptionsController* self)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    unsigned char vals[4];
    void* listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getUseDualShock(np, 0), &vals[0]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[0]);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_2lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getUseDualShock(np, 1), &vals[1]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[1]);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_3lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getControllerMapping(np, 0), &vals[2]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[2]);
        cFEStateOptionsController_displayPadConfig(self, cBENewPlayerInterface_getControllerMapping(np, 0) == 0);
    }
    listBox = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_4lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getControllerMapping(np, 1), &vals[3]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[3]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_displayPadConfig);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char* D_004A15BC;
extern char* D_004A15C0;
extern char str_default_text[];
extern char str_pro_text[];

class cUIObj_18CF50 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Shows the default or the pro button-layout picture and caption on the controller options screen.
 *
 * With @p on set it shows "CtrlPS2_default" (D_004A15BC) and "default_text" and hides "CtrlPS2_pro" (D_004A15C0) and
 * "pro_text"; with @p on clear it does the reverse. Each object is found on mpScreen (+0x40) and changed through its
 * virtual slot 9 (setVisible); missing ones are skipped.
 *
 * @param on  nonzero for the default layout, zero for the pro layout
 */
extern "C" void cFEStateOptionsController_displayPadConfig(cFEStateOptionsController* self, int on)
{
    cUIObj_18CF50* defaultPicture = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A15BC));
    if (defaultPicture != 0) {
        defaultPicture->setVisible(on);
    }
    cUIObj_18CF50* proPicture = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(D_004A15C0));
    if (proPicture != 0) {
        proPicture->setVisible(on ^ 1);
    }
    cUIObj_18CF50* defaultText = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_default_text));
    if (defaultText != 0) {
        defaultText->setVisible(on);
    }
    cUIObj_18CF50* proText = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_pro_text));
    if (proText != 0) {
        proText->setVisible(on ^ 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cUIState_onUpdate(void* self);
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y);

/**
 * Runs the base cUIState_onUpdate() and counts down each pad's test vibration, stopping it when its timer runs out.
 *
 * For pad 0 (bit 0 of +0x50, timer +0x48) and pad 1 (bit 1, timer +0x4C): while the bit is set the timer drops by one
 * per update; once it reaches 0 the bit is cleared and cFEStateOptionsController_vibratePad(self, pad, 0, 0) sets the pad's motor levels to 0.
 * cFEStateOptionsController_onWidgetEvent starts the timers at 60.
 */
extern "C" void cFEStateOptionsController_onUpdate(cFEStateOptionsController* self)
{
    cUIState_onUpdate(self);
    if (*(unsigned char*)&self->mVibrateOn & 1) {
        if (self->mTimer[0] > 0) {
            self->mTimer[0] -= 1;
        } else {
            self->mTimer[0] = 0;
            *(unsigned char*)&self->mVibrateOn &= ~1;
            cFEStateOptionsController_vibratePad(self, 0, 0.0f, 0.0f);
        }
    }
    if (*(unsigned char*)&self->mVibrateOn & 2) {
        if (self->mTimer[1] > 0) {
            self->mTimer[1] -= 1;
        } else {
            self->mTimer[1] = 0;
            *(unsigned char*)&self->mVibrateOn &= ~2;
            cFEStateOptionsController_vibratePad(self, 1, 0.0f, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_vibratePad);
#ifdef SKIP_ASM
extern char* PadCache;

// The pad device (vtable 0x48E488: dtor, sampleButtons, numActuators, setActuator; On Tour cPadDevice).
class cPadDevice {
public:
    virtual ~cPadDevice();
    virtual int sampleButtons();
    virtual int numActuators();
    virtual void setActuator(int act, float frequency);
};

/**
 * Rumbles a controller pad with two motor strengths derived from @p x and @p y.
 *
 * Scales @p x to x' = max(0, x * 0.91333 - 18.5185) and @p y by 0.99167, then looks up pad @p idx (as a signed char) in
 * PadCache's device table (+0x2EEC). When the device has 2 actuators (numActuators), sets motor 0 to the largest of 0,
 * (y - 0.5) * 0.01 and (x' - 2) / 972.22, and motor 1 to the largest of 0, (y - 70) * 0.00625 and (x' - 100) / 5000
 * (setActuator). Callers: cFEStateOptionsController_onCreateScreen and _onDestroyScreen,
 * cFEStateOptionsController_onPopupEvent, cFEStateOptionsController_onWidgetEvent, cFEStateOptionsController_onUpdate;
 * fepopupmisc's func_001E3AC8 is the same body without the state argument. Likely bug: with no PadCache the device
 * stays 0 and numActuators() is still called through it.
 *
 * @param idx  the pad index
 * @param x  first strength input (feeds both motors' second terms)
 * @param y  second strength input (feeds both motors' first terms)
 */
// PORT: uses g++'s >? (max) operator.
extern "C" void cFEStateOptionsController_vibratePad(cFEStateOptionsController* self, int idx, float x, float y)
{
    signed char i = idx;
    float scaledX = x * 0.9133333563804626f - 18.518518447875977f;
    float clampedX = 0.0f;
    if (scaledX >= 0.0f) {
        clampedX = scaledX;
    }
    y = y * 0.9916666746139526f;
    cPadDevice* rumbleDevice = 0;
    if (PadCache != 0) {
        rumbleDevice = *(cPadDevice**)(PadCache + (i << 2) + 0x2EEC);
    }
    if (rumbleDevice->numActuators() == 2) {
        rumbleDevice->setActuator(0, ((y - 0.5f) * 0.009999999776482582f >? (clampedX - 2.0f) / 972.2222290039062f) >? 0.0f);
        rumbleDevice->setActuator(1, ((y - 70.0f) * 0.0062500000931322575f >? (clampedX - 100.0f) / 5000.0f) >? 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateOptionsSaveLoad_vtbl[];

/**
 * Constructs the save/load options state on the engine the caller passes.
 *
 * Runs the cUIState constructor (cUIState_cUIState; the engine argument stays in place in the second argument register),
 * sets the state id (+0xC) to 0x20, installs cFEStateOptionsSaveLoad_vtbl and clears the byte at +0x48. The front end's
 * next-state choosers (cFECustom_getNextState, cFECustom_getPrevState) allocate it as "cFEStateOptionsSaveLoad" (76 bytes).
 *
 * @return @p self
 */
extern "C" void* cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(void* self)
{
    cUIState_cUIState(self);
    *(int*)((char*)self + 0xC) = 0x20;
    *(void***)((char*)self + 0x8) = cFEStateOptionsSaveLoad_vtbl;
    *(char*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_25saveload[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the save/load menu screen ("25saveload") and shows its first frame.
 */
extern "C" void cFEStateOptionsSaveLoad_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_25saveload), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_1saveoptions[];
extern char str_2loadoptions[];
extern char str_3loadreplay[];
extern char str_4new_game[];
extern char D_004A1508[];
extern char D_004A1500[];

static inline int IsHash_D2E8(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Tags each save/load menu entry with the action it starts.
 *
 * Misc data (+0x18): "1saveoptions" 0, "2loadoptions" 2, "3loadreplay" 1, "4new game" 3, "3" 0x300, "2"
 * 0x200.
 *
 * @param widget  the widget being created; matched by its hash name (+0x38)
 */
extern "C" void cFEStateOptionsSaveLoad_onWidgetCreate(void* self, cUIThing* widget)
{
    if (IsHash_D2E8(widget->mHashName, str_1saveoptions)) {
        widget->mMiscData = 0;
    } else if (IsHash_D2E8(widget->mHashName, str_2loadoptions)) {
        widget->mMiscData = 2;
    } else if (IsHash_D2E8(widget->mHashName, str_3loadreplay)) {
        widget->mMiscData = 1;
    } else if (IsHash_D2E8(widget->mHashName, str_4new_game)) {
        widget->mMiscData = 3;
    } else if (IsHash_D2E8(widget->mHashName, D_004A1508)) {
        widget->mMiscData = 0x300;
    } else if (IsHash_D2E8(widget->mHashName, D_004A1500)) {
        widget->mMiscData = 0x200;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onPreRender2D);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern int D_004A19D8;
extern char str_3loadreplay[];
extern char str_4new_game[];

/**
 * Applies DisableTextWidget to the load-replay and new-game entries while D_004A19D8 is set.
 *
 * DisableTextWidget and D_004A19D8 are not identified (probably greys the two entries out).
 */
extern "C" void cFEStateOptionsSaveLoad_onPreRender2D(void* self)
{
    if (D_004A19D8 != 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_3loadreplay));
        if (obj != 0) {
            DisableTextWidget(obj);
        }
        obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_4new_game));
        if (obj != 0) {
            DisableTextWidget(obj);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Filters menu keys on the save/load menu.
 *
 * Keys 8 and 9 give 0x100; every other key gives 0x101.
 *
 * @param menu  the menu (unused)
 * @param key  menu key/event code
 * @return 0x100 for keys 8 and 9, else 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateOptionsSaveLoad_onWidgetMenu(void* self, int menu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onPopupEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIStateStack_pop(void* list, int a1);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void cBENewPlayerInterface_setPlayerName(void* self, int a1, const char* name);
extern "C" void cBE_newProfile(void* self, int c);
extern "C" void* cFERiderManager_getHuman1(void* self, int a1);
extern "C" void cFERider_purge(void* self);
extern "C" int cFERider_init(void* self, int a1, int a2, int a3);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern char D_004A1408[];
extern char str_TransitionOut[];

struct sVE_18D460n {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE_18D460p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Handles the new-game confirmation popup on the save/load menu.
 *
 * Events 0xF and 0x10 store the answer (1 or 0) in the popup's misc data (+0x18) and pop the state stack.
 * Event 0x16 with a yes answer resets the player for the user (+0x44, at least 0) through back-end interface
 * 1 (cBENewPlayerInterface_setPlayerName with an empty name, then slot 1) and cBE_newProfile, re-inits both front-end riders
 * (cFERider_init), pushes the state from the custom interface's slot 4 for (1 << +0x48) | 0x100 and plays
 * "TransitionOut".
 *
 * @param widget  the popup state (its misc data at +0x18 holds the answer)
 * @param event  popup event code (0xF, 0x10, 0x15 or 0x16)
 */
extern "C" void cFEStateOptionsSaveLoad_onPopupEvent(char* self, cUIState* widget, unsigned int event)
{
    switch (event) {
    case 0xF:
        widget->miMiscData = 1;
        cUIStateStack_pop(*(char**)(self + 0x10) + 0x18, 1);
        break;
    case 0x10:
        widget->miMiscData = 0;
        cUIStateStack_pop(*(char**)(self + 0x10) + 0x18, 1);
        break;
    case 21:
        break;
    case 22: {
        if (widget->miMiscData == 0) {
            break;
        }
        int sel = *(signed char*)(self + 0x44);
        if (sel < 0) {
            sel = 0;
        }
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewPlayerInterface_setPlayerName(iface, sel, D_004A1408);
        sVE_18D460n* vt = *(sVE_18D460n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        cBE_newProfile(cBE_getBE(), sel);
        void* feRider = cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, 0);
        cFERider_purge(feRider);
        cFERider_init(feRider, 0, 4, 0);
        feRider = cFERiderManager_getHuman1((char*)SSXApp->RealFE + 0xB0, 1);
        cFERider_purge(feRider);
        cFERider_init(feRider, 1, 0, 0);
        char* obj = **(char***)(self + 0x10);
        sVE_18D460p* vt2 = *(sVE_18D460p**)(obj + 4);
        void* x = vt2[4].fn(obj + vt2[4].delta, self, (1 << *(signed char*)(self + 0x48)) | 0x100);
        if (x != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, x);
        }
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)(self + 0x40), GetHashValue32(str_TransitionOut));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)(self + 0x40), frame, 1);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsSaveLoad_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void* cFEPopupConfirm_cFEPopupConfirm(void* self, void* engine, void* owner, unsigned short* text, int a4, int a5, int a6, int a7, int a8);
extern char str_kT_HELPCreateNewFilePrompt[];
extern char str_cFEPopupConfirm[];

struct sVE_18D648s {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sVE_18D648p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sVE_18D648v {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Handles a select or back on the save/load menu.
 *
 * Event 5: reads the entry's action from its misc data (+0x18) and then always stores 3 there. Action 3 (new
 * game) remembers the current controller (+0x14 into +0x48) and pushes a cFEPopupConfirm with
 * kT_HELPCreateNewFilePrompt; other actions (above 3 with the controller bit added) push the state that the
 * engine's custom interface (engine + 0x0) virtual slot 4, probably getNextState returns. Event 6 pushes the
 * state from the engine's custom interface (engine + 0x0) virtual slot 5, probably getPrevState. A null
 * widget is ignored.
 *
 * @param widget  the selected widget
 * @param event  UI event code (5 or 6 are handled)
 */
extern "C" void cFEStateOptionsSaveLoad_onWidgetEvent(char* self, cUIThing* widget, int event)
{
    if (((char*)widget) == 0) {
        return;
    }
    switch (event) {
    case 5: {
        unsigned int widgetId = (unsigned int)widget->mMiscData;
        widget->mMiscData = 3;
        if (widgetId == 3) {
            *(signed char*)(self + 0x48) = *(signed char*)(self + 0x14);
            char* mgr = (char*)SSXApp->FELocale;
            sVE_18D648s* vt = *(sVE_18D648s**)(mgr + 4);
            unsigned short* text = vt[4].fn(mgr + vt[4].delta, GetHashValue32(str_kT_HELPCreateNewFilePrompt));
            char* mem = (char*)operator new(3068, str_cFEPopupConfirm, 0x100, 0);
            char* confirmPopup = (char*)cFEPopupConfirm_cFEPopupConfirm(mem, *(void**)(self + 0x10), self, text, 0, 0, 0, 0, 0);
            sVE_18D648v* vt2 = *(sVE_18D648v**)(confirmPopup + 8);
            vt2[25].fn(confirmPopup + vt2[25].delta, 1);
            *(int*)(confirmPopup + 0x18) = 0;
            cUIStateStack_pushPopup(*(char**)(self + 0x10) + 0x18, confirmPopup);
        } else {
            if (widgetId > 3) {
                widgetId |= 1 << *(signed char*)(self + 0x14);
            }
            char* obj = **(char***)(self + 0x10);
            sVE_18D648p* vt = *(sVE_18D648p**)(obj + 4);
            void* nextState = vt[4].fn(obj + vt[4].delta, self, widgetId);
            if (nextState != 0) {
                cUIStateStack_push(*(char**)(self + 0x10) + 0x18, nextState);
            }
        }
        break;
    }
    case 6: {
        char* obj = **(char***)(self + 0x10);
        sVE_18D648p* vt = *(sVE_18D648p**)(obj + 4);
        void* nextState = vt[5].fn(obj + vt[5].delta, self, widget->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern "C" void* cSSXApp_getMCManager(void* app);
extern void* cFEStateOptionsDeviceSelect_vtbl[];

/**
 * Constructs the memory-card device select state (state id 33) for a load or a save.
 *
 * Runs cFEMemCard_cFEMemCard2 with @p engine and @p cardMode, installs cFEStateOptionsDeviceSelect_vtbl, stores @p
 * isSave at +0x22C and @p isOnline at +0x230, and clears +0x234 (the hidden-buttons flag) and +0x238. A load screen (@p
 * isSave 0) also clears the card manager's mPollForFullCard. cFECustom_getNextState creates it with (2, 1, 0), (1, 0,
 * 0) or (2, 0, 0), and the quick-messages popup (func_001C2C50) with (2, 1, 1).
 *
 * @param engine  the UI engine
 * @param cardMode  the memory-card screen type passed to cFEMemCard_cFEMemCard2 (kept at +0x1BC)
 * @param isSave  nonzero for a save screen
 * @param isOnline  nonzero when opened from the online front end (On Tour bOnline); cFEStateOptionsDeviceSelect_updateDone then picks next-state
 *            choice 2
 * @return @p self
 */
extern "C" void* cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(cFEStateOptionsDeviceSelect* self, int engine, int cardMode, int isSave, int isOnline)
{
    cFEMemCard_cFEMemCard2(self, engine, cardMode);
    self->mbOnline = isOnline;
    self->mVtbl = cFEStateOptionsDeviceSelect_vtbl;
    self->mHashName = 33;
    self->mIsSaveScreen = isSave;
    self->mIsHidden = 0;
    self->mbChoseToSave = 0;
    if (isSave == 0) {
        *(int*)((char*)cSSXApp_getMCManager(SSXApp) + 0x11C) = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_120MCselect[];
extern char str_btext3[];
extern char str_btext3a[];
extern char str_kT_TITLELoadReplay[];
extern char str_helprelay_ps2[];
extern char str_ps2sq[];
extern char str_btext5[];
extern char str_kT_TITLESaveOptions[];
extern char str_helpsave_ps2[];
extern char str_kT_TITLELoadOptions[];
extern char str_helpload_ps2[];
extern char str_title_Save_Load[];
extern char str_1percentage[];
extern char str_Memory_card_text[];
extern char str_kT_MEMEmpty[];
extern char str_pct_dmemory_card[];
extern char str_pct_d_b[];
extern char D_004A1408[];

struct sColor_DD60 {
    float r, g, b, a;
};
extern sColor_DD60 D_004C66C8;

class cUIObj_DD60 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
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
    virtual void setUseStaticColour(int v);
};

struct sVE_DD60 {
    short delta;
    short index;
    char* (*fn)(void*, int);
};

static inline cUIObj_DD60* getObj_DD60(void* self, char* name)
{
    return (cUIObj_DD60*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

// Looks up a localized string by the hash of its name.
static inline char* getString_DD60(char* name)
{
    char* loc = (char*)SSXApp->FELocale;
    sVE_DD60* vt = *(sVE_DD60**)(loc + 4);
    char* thisp = loc + vt[4].delta;
    return vt[4].fn(thisp, GetHashValue32(name));
}

/**
 * Creates the memory card selection screen ("120MCselect") and sets it up for its mode.
 *
 * The mode comes from +0x1BC (1: load replay) and +0x22C (nonzero: save options; else load options): it picks
 * the title (kT_TITLELoadReplay/SaveOptions/LoadOptions) and which button and help texts show. The four
 * "%dmemory card" slots get kT_MEMEmpty and their index; outside replay mode only slot 1 stays visible.
 */
extern "C" void cFEStateOptionsDeviceSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_120MCselect), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen == 0) {
        return;
    }
    cUIScreen_playFrame(screen, 0, 0);
    cUIObj_DD60* uiObj;
    uiObj = getObj_DD60(self, str_btext3);
    uiObj->setVisible(0);
    uiObj = getObj_DD60(self, str_btext3a);
    uiObj->setVisible(0);
    char* title;
    if (*(int*)((char*)self + 0x1BC) == 1) {
        title = getString_DD60(str_kT_TITLELoadReplay);
        uiObj = getObj_DD60(self, str_btext3a);
        uiObj->setVisible(1);
        uiObj = getObj_DD60(self, str_helprelay_ps2);
        getObj_DD60(self, str_ps2sq)->setVisible(0);
        uiObj->setVisible(1);
        uiObj = getObj_DD60(self, str_btext5);
        uiObj->setVisible(0);
    } else if (*(int*)((char*)self + 0x22C) != 0) {
        title = getString_DD60(str_kT_TITLESaveOptions);
        uiObj = getObj_DD60(self, str_btext3);
        uiObj->setVisible(1);
        uiObj = getObj_DD60(self, str_helpsave_ps2);
        uiObj->setVisible(1);
    } else {
        title = getString_DD60(str_kT_TITLELoadOptions);
        uiObj = getObj_DD60(self, str_btext3a);
        uiObj->setVisible(1);
        uiObj = getObj_DD60(self, str_helpload_ps2);
        getObj_DD60(self, str_ps2sq)->setVisible(0);
        uiObj->setVisible(1);
        uiObj = getObj_DD60(self, str_btext5);
        uiObj->setVisible(0);
    }
    cUIText_setUnicodeString(getObj_DD60(self, str_title_Save_Load), title);
    cUIText_setAsciiString((cUIText*)getObj_DD60(self, str_1percentage), D_004A1408);
    cUIText_setAsciiString((cUIText*)getObj_DD60(self, str_Memory_card_text), D_004A1408);
    char* slot = getString_DD60(str_kT_MEMEmpty);
    char buf[32];
    for (int i = 1; i < 5; i++) {
        sprintf(buf, str_pct_dmemory_card, i);
        uiObj = getObj_DD60(self, buf);
        *(int*)((char*)uiObj + 0x18) = i - 1;
        cUIText_setUnicodeString(uiObj, slot);
        *(sColor_DD60*)((char*)uiObj + 0x88) = D_004C66C8;
        uiObj->setUseStaticColour(1);
        if (*(int*)((char*)self + 0x1BC) != 1 && i >= 2) {
            uiObj->setStatic(1);
            uiObj->setVisible(0);
            sprintf(buf, str_pct_d_b, i);
            uiObj = getObj_DD60(self, buf);
            uiObj->setVisible(0);
            cUIText_setAsciiString((cUIText*)uiObj, buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_onWidgetMenu);
#ifdef SKIP_ASM
struct cList0018DDD0 {
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
    virtual int v54(int a);
};
extern "C" void* cSSXApp_getMCManager(void* app);

/**
 * Answers the device select menu's events: refuses accepting on an unusable memory card.
 *
 * Events 6 and 8 give 0x100 when the selected device (the card manager's mSelectedDevice) fails the memory card's
 * slot-54 check (mpMemCard vtable, func_002D3840 for a cPS2MemCard); event 9 always gives 0x100; everything else gives
 * 0x101. The extra event-8 test on a save screen (+0x22C) returns 0x101 early for a usable card, the same result as
 * falling through. Fills slot +0xA4 (onWidgetMenu) of cFEStateOptionsDeviceSelect_vtbl.
 *
 * @param widget  the menu that sent the event (unused)
 * @param msg  the menu event
 * @return 0x100 to refuse the event, 0x101 to allow it
 */
extern "C" int cFEStateOptionsDeviceSelect_onWidgetMenu(cFEStateOptionsDeviceSelect* self, void* widget, unsigned int msg)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (msg) {
    case 8:
        if (self->mIsSaveScreen != 0 && (*(cList0018DDD0**)(app + 0x434))->v54(*(int*)(app + 0x428)) != 0) {
            return 257;
        }
    case 6:
        if ((*(cList0018DDD0**)(app + 0x434))->v54(*(int*)(app + 0x428)) == 0) {
            return 0x100;
        }
        break;
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_writeSetFileType(void* app, int a1, int a2);
extern "C" void cMCOverlayManager_readSetFileType(void* app, int a1);
extern "C" void cMCOverlayManager_SetAction(void* app, int a1);

struct sVE_18DEA0n {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVE_18DEA0i {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sVE_18DEA0v {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDevSel_18DEA0 {
    char pad0[8];
    sVE_18DEA0n* vt;        // 0x8
    char padC[0x1A8 - 0xC];
    int busy;               // 0x1A8
    char pad1AC[0x1BC - 0x1AC];
    int mode;               // 0x1BC
    int state;              // 0x1C0
    int slot;               // 0x1C4
    char pad1C8[0x1DC - 0x1C8];
    int f1DC;               // 0x1DC
    int pad1E0;
    int f1E4;               // 0x1E4
    char pad1E8[0x214 - 0x1E8];
    int f214;               // 0x214
    char pad218[0x224 - 0x218];
    int f224;               // 0x224
    int pad228;
    int f22C;               // 0x22C
    char pad230[0x238 - 0x230];
    int f238;               // 0x238
};

class cDev_18DEA0 {
public:
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
    virtual int v54(int a);
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
    virtual int HasFileName(int a);
};

/**
 * Handles the device select screen's widget events: accepting a save slot, backing out and the event-7 action.
 *
 * Event 5 (a slot widget): does nothing without a widget or while busy (+0x1A8); sets +0x238, then acts only when the
 * display state f214 is 3 and the main button is active (+0x224). It takes the slot from the widget (+0x18) into slot
 * and the card manager's mFileCursor. In mode 2 on a save screen (+0x22C) it starts a save of file type 2
 * (cMCOverlayManager_writeSetFileType with profile -1), sets +0x1DC, busy and state 3 and calls this state's vtable slot 47 (cFEMemCard_displayStatusPopup,
 * the status popup with the manager's message). Otherwise a slot holding a file (cPS2MemCard_HasFileName through
 * mpMemCard) starts a load (cMCOverlayManager_readSetFileType, file type 0 in mode 1, else 2) and calls slot 44 (cFEMemCard_confirmLoad); an empty
 * slot calls slot 47.
 *
 * Event 6 (back): clears +0x238; unless busy is 1 or state is already 6, sets +0x1E4, resets the manager
 * (cMCOverlayManager_SetState through its mVtbl, state 0) when state is 0, 1 or 2, and sets state 6.
 *
 * Event 7: on a save screen, not busy and not in state 6, with the selected device passing the card's slot-54 check
 * (func_002D3840), takes the widget's slot as for event 5; in state 1 with a file in that slot it sets busy and state
 * 4, cMCOverlayManager_SetAction(0) and the manager state 0x18.
 *
 * @param widget  the widget that sent the event (its +0x18 word is the slot index)
 * @param event  the widget event
 */
extern "C" void cFEStateOptionsDeviceSelect_onWidgetEvent(sDevSel_18DEA0* self, cUIThing* widget, unsigned int event)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (event) {
    case 5: {
        if (((char*)widget) == 0)
            return;
        if (self->busy != 0)
            return;
        self->f238 = 1;
        int st = self->f214;
        if (st != 3)
            return;
        if (self->f224 == 0)
            return;
        int pickedSlot = widget->mMiscData;
        self->slot = pickedSlot;
        *(int*)(app + 0xF8) = pickedSlot;
        if (self->mode == 2 && self->f22C != 0) {
            cMCOverlayManager_writeSetFileType(app, 2, -1);
            self->f1DC = 1;
            self->busy = 1;
            self->state = st;
            self->vt[47].fn((char*)self + self->vt[47].delta);
        } else if (self->mode == 1) {
            if ((*(cDev_18DEA0**)(app + 0x434))->HasFileName(self->slot)) {
                cMCOverlayManager_readSetFileType(app, 0);
                self->vt[44].fn((char*)self + self->vt[44].delta);
            } else {
                self->vt[47].fn((char*)self + self->vt[47].delta);
            }
        } else {
            if ((*(cDev_18DEA0**)(app + 0x434))->HasFileName(self->slot)) {
                cMCOverlayManager_readSetFileType(app, 2);
                self->vt[44].fn((char*)self + self->vt[44].delta);
            } else {
                self->vt[47].fn((char*)self + self->vt[47].delta);
            }
        }
        break;
    }
    case 6: {
        self->f238 = 0;
        char* mcManager = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->busy == 1)
            return;
        int currentState = self->state;
        if (currentState == 6)
            return;
        self->f1E4 = 1;
        if (currentState == 0 || currentState == 2 || currentState == 1) {
            sVE_18DEA0v* vt = *(sVE_18DEA0v**)(mcManager + 0x748);
            vt[1].fn(mcManager + vt[1].delta, 0);
        }
        self->state = 6;
        break;
    }
    case 7: {
        if (self->busy == 1)
            return;
        if (self->state == 6)
            return;
        if (self->f22C == 0)
            return;
        char* mcManager = (char*)cSSXApp_getMCManager(SSXApp);
        if ((*(cDev_18DEA0**)(mcManager + 0x434))->v54(*(int*)(mcManager + 0x428)) == 0)
            return;
        int pickedSlot = widget->mMiscData;
        self->slot = pickedSlot;
        *(int*)(mcManager + 0xF8) = pickedSlot;
        int currentState = self->state;
        if (currentState != 1)
            return;
        if ((*(cDev_18DEA0**)(mcManager + 0x434))->HasFileName(self->slot)) {
            self->busy = currentState;
            self->state = 4;
            cMCOverlayManager_SetAction(mcManager, 0);
            sVE_18DEA0v* vt = *(sVE_18DEA0v**)(mcManager + 0x748);
            vt[1].fn(mcManager + vt[1].delta, 0x18);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_hideActionButtons);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x[];
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_ps2sq[];

/**
 * Shows or hides the device select screen's action buttons.
 *
 * Does nothing when +0x234 already equals @p on. Otherwise sets "ps2x" visible when @p on is 0 and hidden when 1, and
 * likewise "btext3", "btext5" and "ps2sq" on a save screen (+0x22C) or "btext3a" otherwise (cUIThing_setVisible), then
 * stores @p on in +0x234. Fills slot +0x12C of cFEStateOptionsDeviceSelect_vtbl, the slot of
 * cFEMemCard_hideActionButtons in the memory-card base vtable.
 *
 * @param on  1 to hide the buttons, 0 to show them
 */
extern "C" void cFEStateOptionsDeviceSelect_hideActionButtons(cFEStateOptionsDeviceSelect* self, int on)
{
    if (self->mIsHidden == on)
        return;
    int off = on ^ 1;
    ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x)))->setVisible(off);
    if (self->mIsSaveScreen != 0) {
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(off);
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq)))->setVisible(off);
    } else {
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a)))->setVisible(off);
    }
    self->mIsHidden = on;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_hideDeleteButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext5[];
extern char str_ps2sq[];
struct cUIObj_E2C0 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

/**
 * Shows or hides the device select screen's delete button.
 *
 * On a save screen (+0x22C) sets "btext5" and "ps2sq" visible when @p on is false (cUIThing_setVisible); always stores !@p on in +0x220 (the delete-button-active flag that event 8 of the profile menu
 * tests). Fills slot +0x134 of cFEStateOptionsDeviceSelect_vtbl (cFEMemCard_hideDeleteButton's slot).
 *
 * @param on  true to hide the button
 */
extern "C" void cFEStateOptionsDeviceSelect_hideDeleteButton(cFEStateOptionsDeviceSelect* self, bool on)
{
    if (self->mIsSaveScreen != 0) {
        int off = !on;
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq)))->setVisible(off);
    }
    self->mDeleteButtonActive = !on;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_hideMainButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext3[];
extern char str_btext3a[];
extern char str_ps2x[];

struct cUIObj_E368 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int on);
};

/**
 * Shows or hides the device select screen's main (accept) button.
 *
 * On a save screen (+0x22C) shows "btext3" and "ps2x" only when @p saveBlocked is false; otherwise shows "btext3a" and
 * "ps2x" only when @p loadBlocked is false. The shown state is also stored in +0x224, which event 5 of cFEStateOptionsDeviceSelect_onWidgetEvent
 * requires. Fills slot +0x13C of cFEStateOptionsDeviceSelect_vtbl (cFEMemCard_hideMainButton's slot).
 *
 * @param loadBlocked  true to hide the button on a load screen
 * @param saveBlocked  true to hide the button on a save screen
 */
extern "C" void cFEStateOptionsDeviceSelect_hideMainButton(cFEStateOptionsDeviceSelect* self, bool loadBlocked, bool saveBlocked)
{
    cUIObj_E368* xButton = (cUIObj_E368*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x));
    if (self->mIsSaveScreen != 0) {
        int showButton = !saveBlocked;
        ((cUIObj_E368*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(showButton);
        self->mMainButtonActive = showButton;
        xButton->setVisible(showButton);
    } else {
        int showButton = !loadBlocked;
        ((cUIObj_E368*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a)))->setVisible(showButton);
        self->mMainButtonActive = showButton;
        xButton->setVisible(showButton);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_updateDone);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_pushCheck(void* list, void* item);

struct sVEntry0018E478 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Leaves the device select screen once its card work is done, pushing the state chosen for the outcome.
 *
 * Runs once: unless +0x1AC is already set it computes a choice, 1 when the mode (+0x1BC) is 1, forced to 0 when +0x1E4
 * (set by backing out) is set or +0x1B4 is clear, and 2 when the screen was opened online (+0x230). It asks the engine
 * owner (the first word of the engine at +0x10) for the next state through its vtable slot 5 (cFECustom_getPrevState in
 * cFECustom_vtbl) with this state and the choice, and pushes the result on the engine's state stack (+0x18) with
 * cUIStateStack_pushCheck. Sets +0x1AC afterwards. Fills slot +0x10C (cFEMemCard_updateDone's slot).
 */
extern "C" void cFEStateOptionsDeviceSelect_updateDone(cFEStateOptionsDeviceSelect* self)
{
    if (self->mIsDone == 0) {
        int mode = self->mType == 1;
        if (self->mIsCancelPressed == 1) {
            mode = 0;
        }
        if (self->mIsLoadDoneOK == 0) {
            mode = 0;
        }
        if (self->mbOnline != 0) {
            mode = 2;
        }
        void* obj = *(void**)self->mpEngine;
        sVEntry0018E478* vt = *(sVEntry0018E478**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, mode);
        if (nextState != 0) {
            cUIStateStack_pushCheck((char*)self->mpEngine + 0x18, nextState);
        }
    }
    self->mIsDone = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsDeviceSelect_displayStatus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mgr, int dev, unsigned short* buf, int n);
extern "C" void cUIText_setUnicodeStringWrap(void* text, unsigned short* str);
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern "C" void cFEMemCard_callbackDisplayNoCards();
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* app, int dev, int a3);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_1percentage[];
extern char str_Memory_card_text[];
extern char str_pct_dmemory_card[];
extern char str_arrow_left[];
extern char str_arrow_right[];

// UI widget: data before the vptr (vtable at +8).
class cUIObj_18E510 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int on);
    virtual void setVisible(int on);
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
    virtual void setUseStaticColour(int on);
};

// Device-list vtable entries (g++ 2.95 layout: delta, index, function).
struct sDevVtI_18E510 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sDevVt0_18E510 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sDevVtS_18E510 {
    short delta;
    short index;
    void (*fn)(void*, int, char*);
};

// Virtual calls on the memory-card device list (cMCOverlayManager + 0x434, vptr at +0), slot n.
#define DEVS_18E510(mc, n, a, b) ((*(sDevVtS_18E510**)*(char**)((mc) + 0x434))[n].fn(*(char**)((mc) + 0x434) + (*(sDevVtS_18E510**)*(char**)((mc) + 0x434))[n].delta, a, b))
#define DEV0_18E510(mc, n) ((*(sDevVt0_18E510**)*(char**)((mc) + 0x434))[n].fn(*(char**)((mc) + 0x434) + (*(sDevVt0_18E510**)*(char**)((mc) + 0x434))[n].delta))
#define DEVI_18E510(mc, n, a) ((*(sDevVtI_18E510**)*(char**)((mc) + 0x434))[n].fn(*(char**)((mc) + 0x434) + (*(sDevVtI_18E510**)*(char**)((mc) + 0x434))[n].delta, a))


class cStateBase_18E510 {
public:
    char pad[0x8];
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
    virtual void triggerDisplayState(int s);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int displayFileOK();
    virtual void setMenuHighlight();
};

struct cState_18E510 : cStateBase_18E510 {
    char padC[0x40 - 0xC];
    void* screen;           // 0x40
    char pad44[0x1BC - 0x44];
    int mode;               // 0x1BC
    char pad1C0[0x214 - 0x1C0];
    int f214;               // 0x214
    int pad218;
    int dev;                // 0x21C
    char pad220[0x22C - 0x220];
    int f22C;               // 0x22C
};

/**
 * Refreshes the device select screen's memory-card panel: the device label, the slot entries and the device arrows.
 *
 * When this state's vtable slot 42 (cFEMemCard_displayFileOK) passes: with no card found (mpMemCard slot 53, cPS2MemCard_IsAvailable1) it
 * shows the no-cards message (cFEMemCard_callbackDisplayNoCards), otherwise it switches the display state to 3 (slot 35,
 * cFEMemCard_triggerDisplayState) unless f214 is 3 already. If slot 42 still passes it highlights a menu entry (slot
 * 43, cFEMemCard_setMenuHighlight), puts the label of the card manager's mSelectedDevice
 * (cMCOverlayManager_GetDeviceDisplayString) in "Memory card text", stores that device in dev and hides "1percentage".
 *
 * Then for entries 1 to 4 (only entry 1 unless mode is 1) it sets the "%dmemory card" text to the card's title string
 * for file slot i - 1 (mpMemCard slot 14, cPS2MemCard_GetTitleString) and makes the entry selectable (setStatic(0) and
 * slot 25, cUIText_setUseStaticColour(0)) or static (both 1): an empty, enabled slot on a usable card (slots 67 and 66
 * false, slot 55 true) is static in mode 1, and on a save screen (f22C) only the first one is selectable; any other
 * slot is selectable when the selected device passes slot 54 and the slot is not disabled. The "arrow left" and "arrow
 * right" widgets are shown when cFEMemCard_checkNextAvailableDevice finds a device that way. Whenever f214 is not 3,
 * the shown entries get setUseStaticColour(0) and setStatic(1). Fills slot +0xD4 of cFEStateOptionsDeviceSelect_vtbl
 * (cFEMemCard_displayStatus's slot).
 * On Tour: cOVState_SAVEOPTIONS::displayStatus (ovstatereplay.cpp).
 */
extern "C" void cFEStateOptionsDeviceSelect_displayStatus(cFEStateOptionsDeviceSelect* self)
{
    char* pMC = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short ustrDisplay[800];
    if (((cState_18E510*)self)->displayFileOK()) {
        if (DEV0_18E510(pMC, 53) == 0) {
            cFEMemCard_callbackDisplayNoCards();
        } else if (self->mScreenStatus != 3) {
            ((cState_18E510*)self)->triggerDisplayState(3);
        }
    }
    if (((cState_18E510*)self)->displayFileOK()) {
        char text[80];
        cUIObj_18E510* pText;
        int bIsNewSet;
        ((cState_18E510*)self)->setMenuHighlight();
        cMCOverlayManager_GetDeviceDisplayString(pMC, *(int*)(pMC + 0x428), ustrDisplay, 800);
        cUIText_setUnicodeStringWrap(cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Memory_card_text)), ustrDisplay);
        self->mLastSelectedDevice = *(int*)(pMC + 0x428);
        pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_1percentage));
        pText->setVisible(0);
        bIsNewSet = 0;
        for (int i = 1; i < 5 && (self->mType == 1 || i < 2); i++) {
            char strObj[60];
            sprintf(strObj, str_pct_dmemory_card, i);
            pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(strObj));
            DEVS_18E510(pMC, 14, i - 1, text);
            cUIText_setUnicodeString(pText, text);
            if (DEVI_18E510(pMC, 67, i - 1) == 0 && DEVI_18E510(pMC, 66, i - 1) == 0
                && DEV0_18E510(pMC, 55) != 0) {
                if (self->mType == 1) {
                    pText->setUseStaticColour(1);
                    pText->setStatic(1);
                } else if (bIsNewSet == 0 && self->mIsSaveScreen != 0) {
                    bIsNewSet = 1;
                    pText->setStatic(0);
                    pText->setUseStaticColour(0);
                } else {
                    pText->setUseStaticColour(1);
                    pText->setStatic(1);
                }
            } else if (DEVI_18E510(pMC, 54, *(int*)(pMC + 1064)) != 0) {
                if (DEVI_18E510(pMC, 66, i - 1) != 0) {
                    pText->setStatic(1);
                    pText->setUseStaticColour(1);
                } else {
                    pText->setStatic(0);
                    pText->setUseStaticColour(0);
                }
            } else {
                pText->setUseStaticColour(1);
                pText->setStatic(1);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(((cState_18E510*)self), pMC, *(int*)(pMC + 0x428), 1) == -1) {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrow_left)))->setVisible(0);
        } else {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrow_left)))->setVisible(1);
        }
        if (cFEMemCard_checkNextAvailableDevice(((cState_18E510*)self), pMC, *(int*)(pMC + 0x428), 0) == -1) {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrow_right)))->setVisible(0);
        } else {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrow_right)))->setVisible(1);
        }
    }
    if (self->mScreenStatus != 3) {
        for (int i = 1; i < 5 && (self->mType == 1 || i < 2); ) {
            char strObj[80];
            sprintf(strObj, str_pct_dmemory_card, i);
            i++;
            cUIObj_18E510* pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(strObj));
            pText->setUseStaticColour(0);
            pText->setStatic(1);
        }
    }
}
#endif


//100%
INCLUDE_ASM("fe/festateoptions", cFEStateProfileSelect_cFEStateProfileSelect1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateProfileSelect_vtbl[];

/**
 * Constructs the profile select state on the engine the caller passes.
 *
 * Runs the cUIState constructor (cUIState_cUIState; the engine argument stays in place in the second argument register),
 * installs cFEStateProfileSelect_vtbl and sets the state id (+0xC) to 8. No caller in the source references it.
 *
 * @return @p self
 */
extern "C" void* cFEStateProfileSelect_cFEStateProfileSelect1(void* self)
{
    cUIState_cUIState(self);
    *(void***)((char*)self + 0x8) = cFEStateProfileSelect_vtbl;
    *(int*)((char*)self + 0xC) = 8;
    return self;
}
#endif
