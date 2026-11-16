#include "common.h"
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
extern "C" void cFEStateOptions_onWidgetCreate(void* self, void* item)
{
    if (Is_00188870(*(int*)((char*)item + 0x38), D_004A14F8)) {
        *(int*)((char*)item + 0x18) = 0x1C;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1500)) {
        *(int*)((char*)item + 0x18) = 29;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1508)) {
        *(int*)((char*)item + 0x18) = 0x1F;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1510)) {
        *(int*)((char*)item + 0x18) = 0x1E;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1518)) {
        *(int*)((char*)item + 0x18) = 0x20;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1520)) {
        *(int*)((char*)item + 0x18) = 36;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1528)) {
        *(int*)((char*)item + 0x18) = 36;
    } else if (Is_00188870(*(int*)((char*)item + 0x38), D_004A1530)) {
        *(int*)((char*)item + 0x18) = 34;
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
extern "C" void func_001CCF00(void* self, char* s);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int a1, int a2, int a3);
extern "C" int func_00187D38(void* self);
extern "C" void func_00188E58(void* self);
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
    virtual void v02(int msg);
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
 * (func_00188E58, probably saveOptionPrompt) when an options-changed flag (D_004A14D0/D4/D8/DC) is set and
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
extern "C" void cFEStateOptions_onWidgetEvent(sOpt_188980* self, char* widget, unsigned int event)
{
    switch (event) {
    case 5:
        if (Is_00188980(*(int*)(widget + 0x38), D_004A1520)) {
            void* mem = cMemMan_alloc(1092, str_cKeyboardPopup, 0x100, 0);
            void* pop = cKeyboardPopup_cKeyboardPopup(mem, self->engine, self, 0, self->port);
            self->popup = pop;
            cKeyboardPopup_setInputText(pop, D_004A1408);
            func_001CCF00(self->popup, D_004A1408);
            cKeyboardPopup_setIncludeKey(self->popup, 75, 1, 2);
            cUIStateStack_pushPopup(self->engine + 0x18, self->popup);
            break;
        } else if (Is_00188980(*(int*)(widget + 0x38), D_004A1530)) {
            if (D_004A14E0 == 0 && (D_004A14D0 != 0 || D_004A14D4 != 0 || D_004A14DC != 0 || D_004A14D8 != 0)) {
                func_00188E58(self);
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
            void* r = vt[4].fn(obj + vt[4].delta, self, *(int*)(widget + 0x18));
            if (r != 0)
                cUIStateStack_push(self->engine + 0x18, r);
        }
        break;
    case 6: {
        if (D_004A14E0 == 0 && (D_004A14D0 != 0 || D_004A14D4 != 0 || D_004A14DC != 0 || D_004A14D8 != 0)) {
            func_00188E58(self);
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
                ((cListener_188980*)obj)->v02(6);
            } else {
                ((cListener_188980*)obj)->v02(4);
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
extern "C" int cFEStateOptions_onInputBegin(void* self, void* pInput)
{
    D_004A3484 = *(unsigned char*)((char*)pInput + 4);
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
extern "C" int cFEStateOptions_onWidgetMenu(void* self, void* menu, unsigned int key, int index)
{
    if (key < 10) {
        if (key >= 8) {
            return 0x100;
        }
    }
    if (key == 6) {
        int itemHash = *(int*)((char*)cList_getByIndex((char*)menu + 0x74, index) + 0x38);
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
extern "C" void cFEStateOptions_onPopupEvent(void* self, char* popup, int event)
{
    switch (event) {
    case 21: {
        void* scr = *(void**)(popup + 0x40);
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(scr, GetHashValue32(str_reset_text));
        if (text != 0) {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_FESaveOptionsQuesPS2));
        }
        break;
    }
    case 22:
        if (IsK188D08(*(int*)(popup + 0xC), str_111resetopts)) {
            void* uiObj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1530));
            sVEK188D08a* vt = *(sVEK188D08a**)(popup + 8);
            if (vt[23].fn(popup + vt[23].delta, 0)) {
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
INCLUDE_ASM("fe/festateoptions", func_00188E58);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern void* D_0046B3D0[];
extern int D_004A14D0;
extern int D_004A14D4;
extern int D_004A14D8;
extern int D_004A14DC;
extern int D_004A14E0;

extern "C" void func_00188E58(void* self)
{
    char* p = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
    func_0039E318(p, *(void**)((char*)self + 0x10), self);
    *(void***)(p + 8) = D_0046B3D0;
    *(char*)(p + 0x48) = 0;
    cUIStateStack_pushPopup(*(char**)((char*)self + 0x10) + 0x18, p);
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
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateOptionsGame_vtbl[];

extern "C" void* func_00188EE8(void* self)
{
    func_0039E2A0(self);
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

struct sGameCfg_188F28 {
    unsigned int pad0 : 19;
    unsigned int showHud : 1;       // bit 19
    unsigned int f20 : 2;           // bits 20-21
    unsigned int mode : 3;          // bits 22-24
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
    virtual void setEnabled(int v);
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
 * skipped. Row 1 gets MPH/KPH and selects the speed-unit setting (the view's field is named showHud); row 2
 * gets Off/16:9/anamorphic and selects the widescreen setting (f20). Each row also gets its title and help
 * text, the entry "1" is hidden but enabled, and the "Menu" selection starts at entry 1.
 */
extern "C" void cFEStateOptionsGame_onCreateScreen(sOptGame_188F28* self)
{
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_19game_opt), 0);
    self->screen = (char*)screen;
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    sOptionIface_188F28* iface = (sOptionIface_188F28*)cBE_getInterface_Fv(cBE_getBE(), 4);
    self->item[0] = -1;
    self->item[1] = 1;
    self->item[2] = 2;
    cUIObj_188F28* uiObj = (cUIObj_188F28*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A14F8));
    cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A1500));
    uiObj->setVisible(0);
    uiObj->setEnabled(1);
    void* menu = cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Menu));
    if (menu != 0)
        cUIMenu_setSelectedByIndex(menu, 1);
    if (self->item[0] >= 0) {
        void* w = find_188F28(self, D_00440FD8, 0);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGAmerican), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGFrench), 1);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGGerman), 2);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_LANGSpanish), 3);
            unsigned char sel;
            if (cUIListBox_getEntryByData(w, iface->getOptions().mode, &sel))
                cUIListBox_setCurrentEntryByIndex(w, sel);
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FB8, 0);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[0]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FF8, 0);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FE8[0]));
        }
    }
    if (self->item[1] >= 0) {
        void* w = find_188F28(self, D_00440FD8, 1);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNMPH), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNKPH), 0);
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().showHud);
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FB8, 1);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[1]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FF8, 1);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FE8[1]));
        }
    }
    if (self->item[2] >= 0) {
        void* w = find_188F28(self, D_00440FD8, 2);
        if (w != 0) {
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_19Widescreen169), 0);
            cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_19WidescreenAnimorphic), 0);
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().f20);
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FB8, 2);
            if (label != 0)
                cUIText_setUnicodeStringByID(label, GetHashValue32(D_00440FC8[2]));
        }
        {
            cUIText* label = (cUIText*)find_188F28(self, D_00440FF8, 2);
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
INCLUDE_ASM("fe/festateoptions", func_00189610);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0014F4F8(void* iface);
extern "C" void func_00189D60(void* self);
extern "C" void* func_0028B180();
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

extern "C" void func_00189610(void* self, char* popup, int event)
{
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 21:
        break;
    case 22:
        if (Is_189610(*(int*)(popup + 0xC), str_111resetopts)) {
            if (GetOpt_189610(popup, 2)) {
                func_0014F4F8(iface);
                D_004A14D0 = 1;
                func_00189D60(self);
                func_0029ED90(func_0028B180(), 1);
            }
        } else if (Is_189610(*(int*)(popup + 0xC), str_24screen_position)) {
            if (GetOpt_189610(popup, 2)) {
                int a = GetOpt_189610(popup, 0);
                int b = GetOpt_189610(popup, 1);
                sGameCfg_189610* c = &D_00535610_189610;
                D_004A14D0 = 1;
                c->f6 = a;
                c->f5 = b;
            } else {
                cPS2GraphicsMan* ps2GraphicsMan = GrMan;
                sVE_189610b* vt = (sVE_189610b*)ps2GraphicsMan->mVtbl;
                char* thisp = (char*)ps2GraphicsMan + vt[12].delta;
                void (**pf)(void*, int, int) = &vt[12].fn;
                (*pf)(thisp, Cfg_189610().f6, Cfg_189610().f5);
            }
        } else if (*(int*)(popup + 0xC) == 27) {
            if (GetOpt_189610(popup, 2)) {
                func_0014F4F8(iface);
                D_004A14D0 = 1;
                func_00189D60(self);
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
INCLUDE_ASM("fe/festateoptions", func_00189958);
#ifdef SKIP_ASM
extern "C" int func_00189958(void* self, int menu, int event)
{
    return event != 9 ? 257 : 0x100;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00189970);
#ifdef SKIP_ASM
extern "C" int func_00189970(void* self, int pair, int event)
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
extern "C" void* func_0028B180();
extern "C" void func_0029ED90(void* p, int a);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern char str_cFEPopupScreenPos[];
extern char str_cFEPopupVideoCalibration[];
extern void* cFEPopupScreenPos_vtbl[];
extern void* cFEPopupVideoCalibration_vtbl[];
extern void* D_0046B3D0[];
extern char D_004A1520[];
extern char D_004A1528[];
extern char* D_00440FD8[];
extern int D_004A14D0;

struct sGameCfg_189980 {
    unsigned int pad0 : 19;
    bool showHud : 1;           // bit 19
    unsigned int f20 : 2;       // bits 20-21
    unsigned int mode : 3;      // bits 22-24
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
extern "C" void cFEStateOptionsGame_onWidgetEvent(sOptGame_189980* self, char* widget, unsigned int event)
{
    if (widget == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 7: {
        char* popup = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        func_0039E318(popup, self->engine, self);
        *(void***)(popup + 8) = D_0046B3D0;
        popup[0x48] = 0;
        cUIStateStack_pushPopup(self->engine + 0x18, popup);
        break;
    }
    case 5:
        if (Is_189980(*(int*)(widget + 0x38), D_004A1520)) {
            char* popup = (char*)operator new(84, str_cFEPopupScreenPos, 0x100, 0);
            func_0039E318(popup, self->engine, self);
            *(void***)(popup + 8) = cFEPopupScreenPos_vtbl;
            *(int*)(popup + 0x4C) = 0;
            *(int*)(popup + 0x48) = 0;
            popup[0x50] = 0;
            cUIStateStack_pushPopup(self->engine + 0x18, popup);
        } else if (Is_189980(*(int*)(widget + 0x38), D_004A1528)) {
            char* popup = (char*)operator new(76, str_cFEPopupVideoCalibration, 0x100, 0);
            func_0039E318(popup, self->engine, self);
            *(void***)(popup + 8) = cFEPopupVideoCalibration_vtbl;
            *(int*)(popup + 0xC) = 27;
            cUIStateStack_pushPopup(self->engine + 0x18, popup);
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
            vt[3].fn(locale + vt[3].delta, D_00535610_189980.mode);
        }
        Refresh_189980(self);
        char* eng = *(char**)self->engine;
        sVE_189980c* vt = *(sVE_189980c**)(eng + 4);
        void* nextState = vt[5].fn(eng + vt[5].delta, self, *(int*)(widget + 0x18));
        if (nextState != 0) {
            cUIStateStack_push(self->engine + 0x18, nextState);
        }
        break;
    }
    case 9:
        if (self->item[0] >= 0 && Is_189980(*(int*)(widget + 0x38), D_00440FD8[self->item[0]])) {
            D_00535610_189980.mode = cUIListBox_getCurrentEntryData(widget);
            char* locale = (char*)SSXApp->FELocale;
            sVE_189980b* vt = *(sVE_189980b**)(locale + 4);
            char* thisp = locale + vt[3].delta;
            vt[3].fn(thisp, cUIListBox_getCurrentEntryData(widget));
            Refresh_189980(self);
            func_0029ED90(func_0028B180(), 1);
        }
        if (self->item[1] >= 0 && Is_189980(*(int*)(widget + 0x38), D_00440FD8[self->item[1]])) {
            bool on = *(unsigned char*)(widget + 0x319);
            D_00535610_189980.showHud = on;
        }
        if (self->item[2] >= 0 && Is_189980(*(int*)(widget + 0x38), D_00440FD8[self->item[2]])) {
            D_00535610_189980.f20 = *(unsigned char*)(widget + 0x319);
        }
        cSSXApp_activateBEOptions(SSXApp);
        D_004A14D0 = 1;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_00189D60);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cSSXApp_activateBEOptions(void* p);
extern char* D_00440FD8[];

struct sGameCfg_189D60 {
    unsigned int pad0 : 19;
    unsigned int showHud : 1;       // bit 19
    unsigned int f20 : 2;           // bits 20-21
    unsigned int mode : 3;          // bits 22-24
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

// Pushes the current game options into the option widgets of the screen.
extern "C" void func_00189D60(void* self_)
{
    sOptGame_189D60* self = (sOptGame_189D60*)self_;
    sOptionIface_189D60* iface = (sOptionIface_189D60*)cBE_getInterface_Fv(cBE_getBE(), 4);
    if (self->item[0] >= 0) {
        void* w = find_189D60(self, 0);
        if (w != 0) {
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().mode);
            char* o = (char*)SSXApp->FELocale;
            sVE_189D60b* vt = *(sVE_189D60b**)(o + 4);
            vt[3].fn(o + vt[3].delta, cUIListBox_getCurrentEntryData(w));
            char* scr = self->screen;
            sVE_189D60a* vt2 = *(sVE_189D60a**)(scr + 0x48);
            char* sub = scr + 0x40;
            vt2[16].fn(sub + vt2[16].delta);
        }
    }
    if (self->item[1] >= 0) {
        void* w = find_189D60(self, 1);
        if (w != 0)
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().showHud);
    }
    if (self->item[2] >= 0) {
        void* w = find_189D60(self, 2);
        if (w != 0)
            cUIListBox_setCurrentEntryByIndex(w, iface->getOptions().f20);
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
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateOptionsSound_vtbl[];
extern int D_004A14D4;

extern "C" void* func_0018A258(void* self)
{
    func_0039E2A0(self);
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
extern "C" void cFEStateOptionsSound_updateWidget(void* self);
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
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

/**
 * Creates the sound options screen ("141advsettings") and fills its widgets.
 *
 * Calls cFEStateOptionsSound_updateWidget, then shows "ongroup" and entry "7" (disabled) when D_004A1A70 is
 * 1, and "fegroup" otherwise (D_004A1A70's meaning is unknown).
 */
extern "C" void cFEStateOptionsSound_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_141advsettings), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateOptionsSound_updateWidget(self);
    int on = D_004A1A70 == 1;
    cUIObj_18A298* onObj = (cUIObj_18A298*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ongroup));
    if (onObj != 0) {
        onObj->setVisible(on);
    }
    cUIObj_18A298* offObj = (cUIObj_18A298*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_fegroup));
    if (offObj != 0) {
        offObj->setVisible(on ^ 1);
    }
    cUIObj_18A298* item7 = (cUIObj_18A298*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1528));
    if (item7 != 0) {
        item7->setVisible(on);
        item7->setEnabled(on ^ 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A3D8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0014F458(void* iface);
extern "C" void func_0018BEF8(void* self, unsigned int mode);
extern "C" void* func_0028B180();
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
extern "C" void cFEStateOptionsSound_updateWidget(void* self);
extern char str_111resetopts[];
extern int D_004A14D4;

struct sSndCfg_18A3D8 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int f16 : 1;                    // bit 16
    int f17 : 1;                    // bit 17
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

extern "C" void func_0018A3D8(void* self, char* popup, int event)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 21:
        break;
    case 22:
        if (Is_18A3D8(*(int*)(popup + 0xC), str_111resetopts)) {
            if (GetOpt_18A3D8(popup, 2)) {
                func_0014F458(iface);
                func_0018BEF8(self, D_00535610_18A3D8.output);
                func_00287410(func_0028B180(), Cfg_18A3D8().sfxVol);
                func_00287488(func_0028B180(), Cfg_18A3D8().speechVol);
                func_00287520(func_0028B180(), Cfg_18A3D8().musicVol);
                cSSXAudio_SetArcadeSFX(func_0028B180(), Cfg_18A3D8().f17 != 0);
                func_002875D0(func_0028B180(), Cfg_18A3D8().f16 != 0);
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
INCLUDE_ASM("fe/festateoptions", func_0018A890);
#ifdef SKIP_ASM
extern "C" int func_0018A890(void* self, int menu, int event)
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
INCLUDE_ASM("fe/festateoptions", func_0018A8C0);
#ifdef SKIP_ASM
extern "C" int func_0018A8C0(void* self, int pair, int event)
{
    return event ? 0x100 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018A8D0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void func_0018BEF8(void* self, unsigned int mode);
extern "C" void* func_0028B180();
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
extern char str_cFEPopupOptionReset[];
extern void* D_0046B3D0[];
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
    int f16 : 1;                    // bit 16
    int f17 : 1;                    // bit 17
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

struct sOptSound_18A8D0 {
    char pad0[0x10];
    char* engine;                   // 0x10
};

// Sound options screen: widget event handler (popup, apply on exit, value changes).
extern "C" void func_0018A8D0(sOptSound_18A8D0* self, char* widget, unsigned int event)
{
    if (widget == 0)
        return;
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 5: // empty case; needed for the original compare-tree shape
        break;
    case 7: {
        char* p = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        func_0039E318(p, self->engine, self);
        *(void***)(p + 8) = D_0046B3D0;
        p[0x48] = 0;
        cUIStateStack_pushPopup(self->engine + 0x18, p);
        break;
    }
    case 6: {
        {
            sVE_18A8D0a* vt = *(sVE_18A8D0a**)(iface + 0xC);
            vt[1].fn(iface + vt[1].delta);
        }
        func_00287410(func_0028B180(), Cfg_18A8D0().sfxVol);
        func_00287488(func_0028B180(), Cfg_18A8D0().speechVol);
        func_00287520(func_0028B180(), Cfg_18A8D0().musicVol);
        cSSXAudio_SetArcadeSFX(func_0028B180(), Cfg_18A8D0().f17 != 0);
        func_002875D0(func_0028B180(), Cfg_18A8D0().f16 != 0);
        char* eng = *(char**)self->engine;
        sVE_18A8D0c* vt = *(sVE_18A8D0c**)(eng + 4);
        void* r = vt[5].fn(eng + vt[5].delta, self, *(int*)(widget + 0x18));
        if (r != 0)
            cUIStateStack_push(self->engine + 0x18, r);
        break;
    }
    case 9:
        if (Is_18A8D0(*(int*)(widget + 0x38), str_1lb)) {
            D_00535610_18A8D0.output = cUIListBox_getCurrentEntryData(widget);
            func_0018BEF8(self, cUIListBox_getCurrentEntryData(widget));
        }
        else if (Is_18A8D0(*(int*)(widget + 0x38), str_2sl)) {
            unsigned char vol = *(unsigned char*)(widget + 0x74);
            D_00535610_18A8D0.sfxVol = vol;
            func_00287410(func_0028B180(), Cfg_18A8D0().sfxVol);
        }
        else if (Is_18A8D0(*(int*)(widget + 0x38), str_3sl)) {
            unsigned char vol = *(unsigned char*)(widget + 0x74);
            D_00535610_18A8D0.speechVol = vol;
            func_00287488(func_0028B180(), Cfg_18A8D0().speechVol);
        }
        else if (Is_18A8D0(*(int*)(widget + 0x38), str_4sl)) {
            unsigned char vol = *(unsigned char*)(widget + 0x74);
            D_00535610_18A8D0.musicVol = vol;
            func_00287520(func_0028B180(), Cfg_18A8D0().musicVol);
        }
        else if (Is_18A8D0(*(int*)(widget + 0x38), str_5lb)) {
            D_00535610_18A8D0.f17 = cUIListBox_getCurrentEntryData(widget) != 0;
            cSSXAudio_SetArcadeSFX(func_0028B180(), Cfg_18A8D0().f17 != 0);
        }
        else if (Is_18A8D0(*(int*)(widget + 0x38), str_6lb)) {
            D_00535610_18A8D0.f16 = cUIListBox_getCurrentEntryData(widget) != 0;
            func_002875D0(func_0028B180(), Cfg_18A8D0().f16 != 0);
        }
        bool on = D_004A1A70 == 1;
        if (on && Is_18A8D0(*(int*)(widget + 0x38), str_7lb))
            D_00535610_18A8D0.f18 = cUIListBox_getCurrentEntryData(widget) != 0;
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
extern "C" void func_0039A670(void* box);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
extern "C" void* func_0028B180();
int func_00289DC0(void* snd);
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
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
    int f16 : 1;                    // bit 16
    int f17 : 1;                    // bit 17
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
    unsigned int f16 : 1;           // bit 16
    unsigned int f17 : 1;           // bit 17
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
 * "1lb" lists stereo/surround/mono/DTS and selects the sound manager's current mode (func_00289DC0); the
 * sliders "2sl", "3sl", "4sl" (range 12) show the effects, speech and music volumes and push them to the
 * sound manager (func_0028B180); "5lb" and "6lb" are off/on switches applied the same way; "7lb" is filled
 * only when D_004A1A70 is 1.
 *
 * @param vself  the cFEStateOptionsSound state
 */
// Sound options screen: fills the option widgets from the current sound settings.
extern "C" void cFEStateOptionsSound_updateWidget(void* vself)
{
    sOptSound_18B380* self = (sOptSound_18B380*)vself;
    sOptionIface_18B380* iface = (sOptionIface_18B380*)cBE_getInterface_Fv(cBE_getBE(), 4);
    {
    char* w = (char*)Find_18B380(self, str_1lb);
    if (w != 0) {
        func_0039A670(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerStereo), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerSurround), 1);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerMono), 2);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_20SpeakerDTS), 3);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, func_00289DC0(func_0028B180()), &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_2sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().sfxVol;
        func_00287410(func_0028B180(), Cfg_18B380().sfxVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_3sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().speechVol;
        func_00287488(func_0028B180(), Cfg_18B380().speechVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_4sl);
    if (w != 0) {
        *(int*)(w + 0x78) = 12;
        *(int*)(w + 0x74) = iface->getOptions().musicVol;
        func_00287520(func_0028B180(), Cfg_18B380().musicVol);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_5lb);
    if (w != 0) {
        func_0039A670(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOn), 1);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, iface->getOptions().f17, &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
        cSSXAudio_SetArcadeSFX(func_0028B180(), Cfg_18B380().f17 != 0);
    }
    }
    {
    char* w = (char*)Find_18B380(self, str_6lb);
    if (w != 0) {
        func_0039A670(w);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID_i(w, GetHashValue32(str_kT_CMNOn), 1);
        unsigned char sel;
        if (cUIListBox_getEntryByData(w, iface->getOptions().f16, &sel))
            cUIListBox_setCurrentEntryByIndex(w, sel);
        func_002875D0(func_0028B180(), Cfg_18B380().f16 != 0);
    }
    }
    bool on = D_004A1A70 == 1;
    if (on) {
        char* w = (char*)Find_18B380(self, str_7lb);
        if (w != 0) {
            func_0039A670(w);
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
INCLUDE_ASM("fe/festateoptions", func_0018BEF8);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" int func_00287670(void* snd);
extern "C" int func_002876A0(void* snd);
extern "C" int func_002876D0(void* snd);
extern "C" void cSSXAudio_FrontEndPurge(void* snd);
extern "C" void SSXAUDIO_DeInit();
extern "C" void SSXAUDIO_Init(int mode);
extern "C" void func_00287410(void* snd, int v);
extern "C" void func_00287488(void* snd, int v);
extern "C" void func_00287520(void* snd, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* snd, int v);
extern "C" void func_002875D0(void* snd, int v);
extern "C" void func_00285FB0(void* snd);

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

extern "C" void func_0018BEF8(void* self, unsigned int mode)
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
    char* obj = *(char**)(**(char***)((char*)func_0028B180() + 0x118) + 0x1D8);
    sVE_18BEF8i* vt = *(sVE_18BEF8i**)(obj + 4);
    if (vt[2].fn(obj + vt[2].delta, audioMode) == 1) {
        float sfxVolume = func_00287670(func_0028B180());
        float speechVolume = func_002876A0(func_0028B180());
        float musicVolume = func_002876D0(func_0028B180());
        int s0 = *(int*)((char*)func_0028B180() + 0x62B8);
        int arcadeSfx = *(int*)((char*)func_0028B180() + 0x62B4);
        int s2 = *(int*)((char*)func_0028B180() + 0x534);
        cSSXAudio_FrontEndPurge(func_0028B180());
        SSXAUDIO_DeInit();
        SSXAUDIO_Init(audioMode);
        func_00287410(func_0028B180(), (int)sfxVolume);
        func_00287488(func_0028B180(), (int)speechVolume);
        func_00287520(func_0028B180(), (int)musicVolume);
        cSSXAudio_SetArcadeSFX(func_0028B180(), s0);
        func_002875D0(func_0028B180(), arcadeSfx);
        func_00285FB0(func_0028B180());
        char* snd = (char*)func_0028B180();
        sVE_18BEF8v* vt2 = *(sVE_18BEF8v**)(snd + 0x5558);
        char* sub = snd + 0x118;
        vt2[4].fn(sub + vt2[4].delta, s2);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C0E8);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046C550[];
extern int D_004A14D8;

extern "C" void* func_0018C0E8(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 8) = D_0046C550;
    *(int*)((char*)self + 0xC) = 0x1E;
    D_004A14D8 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C128);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_21hud_opt[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0018C478(void* self);

extern "C" void func_0018C128(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_21hud_opt), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0018C478(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C198__FPv);
#ifdef SKIP_ASM
void* func_0018C198(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C1B8);
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
extern "C" void func_0014F600(void* self);
extern "C" void func_0018C478(void* self);

extern "C" void func_0018C1B8(void* self, void* widget, int msg)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (msg) {
    case 21:
        break;
    case 22: {
        int h = *(int*)((char*)widget + 0xC);
        if (h == GetHashValue32(str_111resetopts)) {
            sVE_18C1B8* e = &(*(sVE_18C1B8**)((char*)widget + 8))[23];
            if (e->fn((char*)widget + e->delta, 2) != 0) {
                func_0014F600(iface);
                func_0018C478(self);
                D_004A14D8 = 1;
            }
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C270);
#ifdef SKIP_ASM
extern "C" int func_0018C270(void* self, int menu, int event)
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
INCLUDE_ASM("fe/festateoptions", func_0018C2A0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEPopupOptionReset[];
extern void* D_0046B3D0[];
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

extern "C" void func_0018C2A0(char* self, char* widget, unsigned int event)
{
    if (widget == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (event) {
    case 7: {
        char* p = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        func_0039E318(p, *(void**)(self + 0x10), self);
        *(void***)(p + 8) = D_0046B3D0;
        *(char*)(p + 0x48) = 0;
        cUIStateStack_pushPopup(*(char**)(self + 0x10) + 0x18, p);
        break;
    }
    case 6: {
        sVE_18C2A0n* vt = *(sVE_18C2A0n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        char* obj = **(char***)(self + 0x10);
        sVE_18C2A0p* vt2 = *(sVE_18C2A0p**)(obj + 4);
        void* r = vt2[5].fn(obj + vt2[5].delta, self, *(int*)(widget + 0x18));
        if (r != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, r);
        }
        break;
    }
    case 1: {
        sVE_18C2A0* vt = *(sVE_18C2A0**)(widget + 8);
        vt[9].fn(widget + vt[9].delta, 1);
        D_004A14D8 = 1;
        D_00535610_C2A0.mode = *(unsigned char*)(widget + 0x18);
        break;
    }
    case 2:
        if (Is_0018C2A0(*(int*)(widget + 0x38), D_004A14F8) || Is_0018C2A0(*(int*)(widget + 0x38), D_004A1500) ||
            Is_0018C2A0(*(int*)(widget + 0x38), D_004A1508)) {
            sVE_18C2A0* vt = *(sVE_18C2A0**)(widget + 8);
            vt[9].fn(widget + vt[9].delta, 0);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C478);
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

extern "C" void func_0018C478(void* vself)
{
    char* self = (char*)vself;
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sName_C478 name = *(sName_C478*)D_004A14F8;
    char* menu = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_Menu));
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
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateOptionsController_vtbl[];
extern int D_004A14DC;

extern "C" void* func_0018C658(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 8) = cFEStateOptionsController_vtbl;
    *(int*)((char*)self + 0xC) = 0x1F;
    *(char*)((char*)self + 0x50) = 0;
    D_004A14DC = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onCreateScreen);
#ifdef SKIP_ASM
extern "C" void cUIListBox_addEntryByStringID(void* box, int id, int idx);
extern "C" void func_0018D108(void* self, int idx, float x, float y);
extern "C" void func_0018CDC0(void* self);
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
 * calls func_0018D108(self, pad, 0, 0) for pads 0 and 1 (probably vibratePad, stopping vibration) and then
 * func_0018CDC0 (probably updateWidget).
 */
extern "C" void cFEStateOptionsController_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_22control), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    void* listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_1lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOn), 1);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_2lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOff), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_CMNOn), 1);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_3lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_20PresetDefault), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_OVRCMNPro), 1);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_4lb));
    if (listBox != 0) {
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_20PresetDefault), 0);
        cUIListBox_addEntryByStringID(listBox, GetHashValue32(str_kT_OVRCMNPro), 1);
    }
    float zero = 0.0f;
    func_0018D108(self, 0, zero, zero);
    func_0018D108(self, 1, zero, zero);
    func_0018CDC0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", cFEStateOptionsController_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void func_0018D108(void* self, int idx, float x, float y);
void* cUIState_onDestroyScreen(void* self);

/**
 * Sets both controllers' output channels to zero, then destroys the controller-options state's screens.
 *
 * Calls func_0018D108(self, pad, 0, 0) for pads 0 and 1; for a pad whose device reports state 2 that sets both
 * of its output channels to 0, which probably stops any vibration. cUIState_onDestroyScreen() then destroys the
 * state's screen list.
 *
 * @return no meaningful value (cUIState_onDestroyScreen() returns nothing)
 */
extern "C" void* cFEStateOptionsController_onDestroyScreen(void* self)
{
    func_0018D108(self, 0, 0.0f, 0.0f);
    func_0018D108(self, 1, 0.0f, 0.0f);
    return cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018C910);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_defaultCtrl(void* self);
extern "C" void func_0018CDC0(void* self);
extern "C" void func_0018D108(void* self, int idx, float x, float y);
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
    virtual int v23(int a);
};

extern "C" void func_0018C910(void* self, cUIObj_18C910* obj, int msg)
{
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    switch (msg) {
    case 21: {
        float zero = 0.0f;
        func_0018D108(self, 0, zero, zero);
        func_0018D108(self, 1, zero, zero);
        break;
    }
    case 22:
        if (Is_0018C910(*(int*)((char*)obj + 0xC), str_111resetopts)) {
            if (obj->v23(2) != 0) {
                cBENewPlayerInterface_defaultCtrl(pi);
                func_0018CDC0(self);
                D_004A14DC = 1;
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CA10);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cMemMan_alloc bound as operator new so gcc treats it as malloc-like.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_0039E318(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void func_00147528(void* self, int index, int value);
extern "C" void func_00147658(void* self, int index, int value);
extern "C" int func_001474E8(void* iface, int a1);
extern "C" void func_0018CF50(void* self, int on);
extern "C" void func_0018D108(void* self, int idx, float x, float y);
extern char str_cFEPopupOptionReset[];
extern void* D_0046B3D0[];
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

extern "C" void func_0018CA10(char* self, char* widget, unsigned int event)
{
    if (widget == 0) {
        return;
    }
    char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
    switch (event) {
    case 7: {
        char* p = (char*)operator new(76, str_cFEPopupOptionReset, 0x100, 0);
        func_0039E318(p, *(void**)(self + 0x10), self);
        *(void***)(p + 8) = D_0046B3D0;
        *(char*)(p + 0x48) = 0;
        cUIStateStack_pushPopup(*(char**)(self + 0x10) + 0x18, p);
        break;
    }
    case 6: {
        sVE_18CA10n* vt = *(sVE_18CA10n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        char* obj = **(char***)(self + 0x10);
        sVE_18CA10p* vt2 = *(sVE_18CA10p**)(obj + 4);
        void* r = vt2[5].fn(obj + vt2[5].delta, self, *(int*)(widget + 0x18));
        if (r != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, r);
        }
        break;
    }
    case 9:
        if (Is_0018CA10(*(int*)(widget + 0x38), str_1lb)) {
            func_00147528(iface, 0, cUIListBox_getCurrentEntryData(widget) != 0);
            if (func_001474E8(iface, 0)) {
                func_0018D108(self, 0, 10.0f, 10.0f);
                *(int*)(self + 0x48) = 0x3C;
                *(unsigned char*)(self + 0x50) |= 1;
            } else {
                func_0018D108(self, 0, 0.0f, 0.0f);
            }
        } else if (Is_0018CA10(*(int*)(widget + 0x38), str_2lb)) {
            func_00147528(iface, 1, cUIListBox_getCurrentEntryData(widget) != 0);
            if (func_001474E8(iface, 1)) {
                func_0018D108(self, 1, 10.0f, 10.0f);
                *(int*)(self + 0x4C) = 0x3C;
                *(unsigned char*)(self + 0x50) |= 2;
            } else {
                func_0018D108(self, 1, 0.0f, 0.0f);
            }
        } else if (Is_0018CA10(*(int*)(widget + 0x38), str_3lb)) {
            func_00147658(iface, 0, cUIListBox_getCurrentEntryData(widget));
            func_0018CF50(self, cUIListBox_getCurrentEntryData(widget) == 0);
        } else if (Is_0018CA10(*(int*)(widget + 0x38), str_4lb)) {
            func_00147658(iface, 1, cUIListBox_getCurrentEntryData(widget));
            func_0018CF50(self, cUIListBox_getCurrentEntryData(widget) == 0);
        }
        D_004A14DC = 1;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CCE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getControllerMapping(void* player, int which);
// func_0018CF50 returns void; declaring it int ($v0 set by the call) reproduces the original argument scheduling.
extern "C" int func_0018CF50_CCE0(void* self, int on) __asm__("func_0018CF50");

// Options toggle row: accept flips the setting chosen by the cursor (items 2 and 3), back is consumed.
extern "C" int func_0018CCE0(void* self, char* menu, unsigned int key)
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
        func_0018CF50_CCE0(self, cBENewPlayerInterface_getControllerMapping(player, which) == 0);
        return 257;
    }
    case 6:
        return 0;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CDB0);
#ifdef SKIP_ASM
extern "C" int func_0018CDB0(void* self, int pair, int event)
{
    return event ? 0x100 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CDC0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int func_001474E8(void* iface, int a1);
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int a1);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int a1);
extern "C" void func_0018CF50(void* self, int on);
extern char str_1lb[];
extern char str_2lb[];
extern char str_3lb[];
extern char str_4lb[];

extern "C" void func_0018CDC0(void* self)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    unsigned char vals[4];
    void* listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_1lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, func_001474E8(np, 0), &vals[0]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[0]);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_2lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, func_001474E8(np, 1), &vals[1]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[1]);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_3lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getControllerMapping(np, 0), &vals[2]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[2]);
        func_0018CF50(self, cBENewPlayerInterface_getControllerMapping(np, 0) == 0);
    }
    listBox = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_4lb));
    if (listBox != 0) {
        cUIListBox_getEntryByData(listBox, cBENewPlayerInterface_getControllerMapping(np, 1), &vals[3]);
        cUIListBox_setCurrentEntryByIndex(listBox, vals[3]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018CF50);
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
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void func_0018CF50(void* self, int on)
{
    cUIObj_18CF50* a = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15BC));
    if (a != 0) {
        a->setVisible(on);
    }
    cUIObj_18CF50* b = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A15C0));
    if (b != 0) {
        b->setVisible(on ^ 1);
    }
    cUIObj_18CF50* c = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_default_text));
    if (c != 0) {
        c->setVisible(on);
    }
    cUIObj_18CF50* d = (cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_pro_text));
    if (d != 0) {
        d->setVisible(on ^ 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D058);
#ifdef SKIP_ASM
extern "C" void* cUIState_onUpdate(void* self);
extern "C" void func_0018D108(void* self, int idx, float x, float y);

extern "C" void func_0018D058(void* self)
{
    cUIState_onUpdate(self);
    if (*(unsigned char*)((char*)self + 0x50) & 1) {
        if (*(int*)((char*)self + 0x48) > 0) {
            *(int*)((char*)self + 0x48) -= 1;
        } else {
            *(int*)((char*)self + 0x48) = 0;
            *(unsigned char*)((char*)self + 0x50) &= ~1;
            func_0018D108(self, 0, 0.0f, 0.0f);
        }
    }
    if (*(unsigned char*)((char*)self + 0x50) & 2) {
        if (*(int*)((char*)self + 0x4C) > 0) {
            *(int*)((char*)self + 0x4C) -= 1;
        } else {
            *(int*)((char*)self + 0x4C) = 0;
            *(unsigned char*)((char*)self + 0x50) &= ~2;
            func_0018D108(self, 1, 0.0f, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D108);
#ifdef SKIP_ASM
extern char* PadCache;

class cSnd_18D108 {
public:
    virtual void v01();
    virtual void v02();
    virtual int getState();
    virtual void setVolume(int ch, float v);
};

// PORT: uses g++'s >? (max) operator.
extern "C" void func_0018D108(void* self, int idx, float x, float y)
{
    signed char i = idx;
    float scaledX = x * 0.9133333563804626f - 18.518518447875977f;
    float clampedX = 0.0f;
    if (scaledX >= 0.0f) {
        clampedX = scaledX;
    }
    y = y * 0.9916666746139526f;
    cSnd_18D108* rumbleDevice = 0;
    if (PadCache != 0) {
        rumbleDevice = *(cSnd_18D108**)(PadCache + (i << 2) + 0x2EEC);
    }
    if (rumbleDevice->getState() == 2) {
        rumbleDevice->setVolume(0, ((y - 0.5f) * 0.009999999776482582f >? (clampedX - 2.0f) / 972.2222290039062f) >? 0.0f);
        rumbleDevice->setVolume(1, ((y - 70.0f) * 0.0062500000931322575f >? (clampedX - 100.0f) / 5000.0f) >? 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D240);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateOptionsSaveLoad_vtbl[];

extern "C" void* func_0018D240(void* self)
{
    func_0039E2A0(self);
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
extern "C" void cFEStateOptionsSaveLoad_onWidgetCreate(void* self, void* widget)
{
    if (IsHash_D2E8(*(int*)((char*)widget + 0x38), str_1saveoptions)) {
        *(int*)((char*)widget + 0x18) = 0;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), str_2loadoptions)) {
        *(int*)((char*)widget + 0x18) = 2;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), str_3loadreplay)) {
        *(int*)((char*)widget + 0x18) = 1;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), str_4new_game)) {
        *(int*)((char*)widget + 0x18) = 3;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_004A1508)) {
        *(int*)((char*)widget + 0x18) = 0x300;
    } else if (IsHash_D2E8(*(int*)((char*)widget + 0x38), D_004A1500)) {
        *(int*)((char*)widget + 0x18) = 0x200;
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
extern "C" void func_0039F190(void* list, int a1);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void func_00147138(void* self, int a1, const char* name);
extern "C" void func_0014DE28(void* self, int c);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019EBA0(void* self);
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
 * 1 (func_00147138 with an empty name, then slot 1) and func_0014DE28, re-inits both front-end riders
 * (cFERider_init), pushes the state from the custom interface's slot 4 for (1 << +0x48) | 0x100 and plays
 * "TransitionOut".
 *
 * @param widget  the popup state (its misc data at +0x18 holds the answer)
 * @param event  popup event code (0xF, 0x10, 0x15 or 0x16)
 */
extern "C" void cFEStateOptionsSaveLoad_onPopupEvent(char* self, char* widget, unsigned int event)
{
    switch (event) {
    case 0xF:
        *(int*)(widget + 0x18) = 1;
        func_0039F190(*(char**)(self + 0x10) + 0x18, 1);
        break;
    case 0x10:
        *(int*)(widget + 0x18) = 0;
        func_0039F190(*(char**)(self + 0x10) + 0x18, 1);
        break;
    case 21:
        break;
    case 22: {
        if (*(int*)(widget + 0x18) == 0) {
            break;
        }
        int sel = *(signed char*)(self + 0x44);
        if (sel < 0) {
            sel = 0;
        }
        char* iface = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_00147138(iface, sel, D_004A1408);
        sVE_18D460n* vt = *(sVE_18D460n**)(iface + 0xC);
        vt[1].fn(iface + vt[1].delta);
        func_0014DE28(cBE_getBE(), sel);
        void* feRider = func_001A0548((char*)SSXApp->RealFE + 0xB0, 0);
        func_0019EBA0(feRider);
        cFERider_init(feRider, 0, 4, 0);
        feRider = func_001A0548((char*)SSXApp->RealFE + 0xB0, 1);
        func_0019EBA0(feRider);
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
extern "C" void cFEStateOptionsSaveLoad_onWidgetEvent(char* self, char* widget, int event)
{
    if (widget == 0) {
        return;
    }
    switch (event) {
    case 5: {
        unsigned int widgetId = *(unsigned int*)(widget + 0x18);
        *(int*)(widget + 0x18) = 3;
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
        void* nextState = vt[5].fn(obj + vt[5].delta, self, *(int*)(widget + 0x18));
        if (nextState != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018D7F0);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard(void* self, int a1, int a2);
extern "C" void* cSSXApp_getMCManager(void* app);
extern void* cFEStateOptionsDeviceSelect_vtbl[];

extern "C" void* func_0018D7F0(void* self, int engine, int cardMode, int isSave, int a4)
{
    cFEMemCard_cFEMemCard(self, engine, cardMode);
    *(int*)((char*)self + 0x230) = a4;
    *(void***)((char*)self + 8) = cFEStateOptionsDeviceSelect_vtbl;
    *(int*)((char*)self + 0xC) = 33;
    *(int*)((char*)self + 0x22C) = isSave;
    *(int*)((char*)self + 0x234) = 0;
    *(int*)((char*)self + 0x238) = 0;
    if (isSave == 0) {
        *(int*)((char*)cSSXApp_getMCManager(SSXApp) + 0x11C) = 0;
    }
    return self;
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
    virtual void setEnabled(int v);
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
    virtual void setHighlight(int v);
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
        uiObj->setHighlight(1);
        if (*(int*)((char*)self + 0x1BC) != 1 && i >= 2) {
            uiObj->setEnabled(1);
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
INCLUDE_ASM("fe/festateoptions", func_0018DDD0);
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

extern "C" int func_0018DDD0(void* self, void* widget, unsigned int msg)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (msg) {
    case 8:
        if (*(int*)((char*)self + 0x22C) != 0 && (*(cList0018DDD0**)(app + 0x434))->v54(*(int*)(app + 0x428)) != 0) {
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
INCLUDE_ASM("fe/festateoptions", func_0018DEA0);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void func_0023CAE8(void* app, int a1, int a2);
extern "C" void func_0023C8F0(void* app, int a1);
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
    virtual int v67(int a);
};

extern "C" void func_0018DEA0(sDevSel_18DEA0* self, char* widget, unsigned int event)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (event) {
    case 5: {
        if (widget == 0)
            return;
        if (self->busy != 0)
            return;
        self->f238 = 1;
        int st = self->f214;
        if (st != 3)
            return;
        if (self->f224 == 0)
            return;
        int v = *(int*)(widget + 0x18);
        self->slot = v;
        *(int*)(app + 0xF8) = v;
        if (self->mode == 2 && self->f22C != 0) {
            func_0023CAE8(app, 2, -1);
            self->f1DC = 1;
            self->busy = 1;
            self->state = st;
            self->vt[47].fn((char*)self + self->vt[47].delta);
        } else if (self->mode == 1) {
            if ((*(cDev_18DEA0**)(app + 0x434))->v67(self->slot)) {
                func_0023C8F0(app, 0);
                self->vt[44].fn((char*)self + self->vt[44].delta);
            } else {
                self->vt[47].fn((char*)self + self->vt[47].delta);
            }
        } else {
            if ((*(cDev_18DEA0**)(app + 0x434))->v67(self->slot)) {
                func_0023C8F0(app, 2);
                self->vt[44].fn((char*)self + self->vt[44].delta);
            } else {
                self->vt[47].fn((char*)self + self->vt[47].delta);
            }
        }
        break;
    }
    case 6: {
        self->f238 = 0;
        char* a = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->busy == 1)
            return;
        int s = self->state;
        if (s == 6)
            return;
        self->f1E4 = 1;
        if (s == 0 || s == 2 || s == 1) {
            sVE_18DEA0v* vt = *(sVE_18DEA0v**)(a + 0x748);
            vt[1].fn(a + vt[1].delta, 0);
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
        char* a = (char*)cSSXApp_getMCManager(SSXApp);
        if ((*(cDev_18DEA0**)(a + 0x434))->v54(*(int*)(a + 0x428)) == 0)
            return;
        int v = *(int*)(widget + 0x18);
        self->slot = v;
        *(int*)(a + 0xF8) = v;
        int s = self->state;
        if (s != 1)
            return;
        if ((*(cDev_18DEA0**)(a + 0x434))->v67(self->slot)) {
            self->busy = s;
            self->state = 4;
            cMCOverlayManager_SetAction(a, 0);
            sVE_18DEA0v* vt = *(sVE_18DEA0v**)(a + 0x748);
            vt[1].fn(a + vt[1].delta, 0x18);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E178);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x[];
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_ps2sq[];

extern "C" void func_0018E178(void* self, int on)
{
    if (*(int*)((char*)self + 0x234) == on)
        return;
    int off = on ^ 1;
    ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x)))->setVisible(off);
    if (*(int*)((char*)self + 0x22C) != 0) {
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setVisible(off);
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq)))->setVisible(off);
    } else {
        ((cUIObj_18CF50*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3a)))->setVisible(off);
    }
    *(int*)((char*)self + 0x234) = on;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E2C0);
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
    virtual void setActive(int on);
};

extern "C" void func_0018E2C0(void* self, bool on)
{
    if (*(int*)((char*)self + 0x22C) != 0) {
        int off = !on;
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext5)))->setActive(off);
        ((cUIObj_E2C0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2sq)))->setActive(off);
    }
    *(int*)((char*)self + 0x220) = !on;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E368);
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

extern "C" void func_0018E368(void* self, bool loadBlocked, bool saveBlocked)
{
    cUIObj_E368* xButton = (cUIObj_E368*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ps2x));
    if (*(int*)((char*)self + 0x22C) != 0) {
        int showButton = !saveBlocked;
        ((cUIObj_E368*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3)))->setVisible(showButton);
        *(int*)((char*)self + 0x224) = showButton;
        xButton->setVisible(showButton);
    } else {
        int showButton = !loadBlocked;
        ((cUIObj_E368*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_btext3a)))->setVisible(showButton);
        *(int*)((char*)self + 0x224) = showButton;
        xButton->setVisible(showButton);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E478);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_pushCheck(void* list, void* item);

struct sVEntry0018E478 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

extern "C" void func_0018E478(void* self)
{
    if (*(int*)((char*)self + 0x1AC) == 0) {
        int mode = *(int*)((char*)self + 0x1BC) == 1;
        if (*(int*)((char*)self + 0x1E4) == 1) {
            mode = 0;
        }
        if (*(int*)((char*)self + 0x1B4) == 0) {
            mode = 0;
        }
        if (*(int*)((char*)self + 0x230) != 0) {
            mode = 2;
        }
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry0018E478* vt = *(sVEntry0018E478**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, mode);
        if (nextState != 0) {
            cUIStateStack_pushCheck(*(char**)((char*)self + 0x10) + 0x18, nextState);
        }
    }
    *(int*)((char*)self + 0x1AC) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateoptions", func_0018E510);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mgr, int dev, unsigned short* buf, int n);
extern "C" void cUIText_setUnicodeStringWrap(void* text, unsigned short* str);
extern "C" void cUIText_setUnicodeString(void* text, char* str);
extern "C" void func_001D8240();
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
    virtual void v08(int on);
    virtual void v09(int on);
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
    virtual void v25(int on);
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
    virtual void v35(int s);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int v42();
    virtual void v43();
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

// cFEStateOptions*::displayStatus (On Tour: cOVState_SAVEOPTIONS::displayStatus, ovstatereplay.cpp): the memory-card
// device text, the per-slot "%dmemory card" entries and the arrows.
extern "C" void func_0018E510(cState_18E510* self)
{
    char* pMC = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short ustrDisplay[800];
    if (self->v42()) {
        if (DEV0_18E510(pMC, 53) == 0) {
            func_001D8240();
        } else if (self->f214 != 3) {
            self->v35(3);
        }
    }
    if (self->v42()) {
        char text[80];
        cUIObj_18E510* pText;
        int bIsNewSet;
        self->v43();
        cMCOverlayManager_GetDeviceDisplayString(pMC, *(int*)(pMC + 0x428), ustrDisplay, 800);
        cUIText_setUnicodeStringWrap(cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_Memory_card_text)), ustrDisplay);
        self->dev = *(int*)(pMC + 0x428);
        pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_1percentage));
        pText->v09(0);
        bIsNewSet = 0;
        for (int i = 1; i < 5 && (self->mode == 1 || i < 2); i++) {
            char strObj[60];
            sprintf(strObj, str_pct_dmemory_card, i);
            pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(strObj));
            DEVS_18E510(pMC, 14, i - 1, text);
            cUIText_setUnicodeString(pText, text);
            if (DEVI_18E510(pMC, 67, i - 1) == 0 && DEVI_18E510(pMC, 66, i - 1) == 0
                && DEV0_18E510(pMC, 55) != 0) {
                if (self->mode == 1) {
                    pText->v25(1);
                    pText->v08(1);
                } else if (bIsNewSet == 0 && self->f22C != 0) {
                    bIsNewSet = 1;
                    pText->v08(0);
                    pText->v25(0);
                } else {
                    pText->v25(1);
                    pText->v08(1);
                }
            } else if (DEVI_18E510(pMC, 54, *(int*)(pMC + 1064)) != 0) {
                if (DEVI_18E510(pMC, 66, i - 1) != 0) {
                    pText->v08(1);
                    pText->v25(1);
                } else {
                    pText->v08(0);
                    pText->v25(0);
                }
            } else {
                pText->v25(1);
                pText->v08(1);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(self, pMC, *(int*)(pMC + 0x428), 1) == -1) {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_arrow_left)))->v09(0);
        } else {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_arrow_left)))->v09(1);
        }
        if (cFEMemCard_checkNextAvailableDevice(self, pMC, *(int*)(pMC + 0x428), 0) == -1) {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_arrow_right)))->v09(0);
        } else {
            ((cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_arrow_right)))->v09(1);
        }
    }
    if (self->f214 != 3) {
        for (int i = 1; i < 5 && (self->mode == 1 || i < 2); ) {
            char strObj[80];
            sprintf(strObj, str_pct_dmemory_card, i);
            i++;
            cUIObj_18E510* pText = (cUIObj_18E510*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(strObj));
            pText->v25(0);
            pText->v08(1);
        }
    }
}
#endif


//100%
INCLUDE_ASM("fe/festateoptions", func_0018EA98);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateProfileSelect_vtbl[];

extern "C" void* func_0018EA98(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateProfileSelect_vtbl;
    *(int*)((char*)self + 0xC) = 8;
    return self;
}
#endif
