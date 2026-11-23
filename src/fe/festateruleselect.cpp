#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "ui/uipair.h"
#include "ui/uitext.h"
#include "ui/uithing.h"
#include "ui/uilistbox.h"
#include "ui/uiscreen.h"
#include "ui/uimenu.h"
#include "be/beintnewrace.h"
#include "ui/uiengine.h"
#include "fe/festateruleselect.h"

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_setupMenu);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIScreen;
struct cUIText;
int GetHashValue32(char* str);
int cUIScreen_getFrameByLabel(cUIScreen* self, int hash);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUIListBox_deleteAllEntries(void* self);
extern "C" unsigned char cUIListBox_addEntryByStringID(void* box, int id, int idx);
// The entry data is unsigned here: the negative time steps are passed as 0xFFFFFFFD etc.
extern "C" unsigned char cUIListBox_addEntryByAsciiString(void* box, const char* str, unsigned int data);
extern "C" int cUIListBox_getEntryByData(void* self, int key, unsigned char* out);
extern "C" void cUIListBox_setCurrentEntryByIndex(void* self, int v);
extern "C" int cFEStateRuleSelect_getBEData(cFEStateRuleSelect* self, int rule);
extern "C" void cFEStateRuleSelect_updateMenuColor(cFEStateRuleSelect* self);
extern char str_0b[];
extern char str_0t[];
extern char str_Menu[];
extern char str_arrw_left[];
extern char str_arrw_right[];
extern char str_kT_CMNOff[];
extern char str_kT_CMNOn[];
extern char str_kT_CMNONo[];
extern char str_kT_CMNYes[];
extern char str_kT_CMNSkillLevel1[];
extern char str_kT_CMNSkillLevel2[];
extern char str_kT_CMNSkillLevel3[];
extern char str_90pct[];
extern char str_80pct[];
extern char str_70pct[];
extern char str_60pct[];
extern char str_10s[];
extern char str_5s[];
extern char str_3s[];
extern char str_3s_b[];
extern char str_5s_b[];
extern char str_10s_b[];
extern char D_004A13A0[];
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];
extern char D_004A1510[];
extern char D_004A1518[];
extern signed char D_00441008[];
extern char* D_00441018[];
// PORT: the frame labels are copied with the builtin block move (the original likely used
// strcpy on string literals); the two strings live at these symbols.
extern char str_greyedout[] __attribute__((aligned(8)));
extern char str_normal[] __attribute__((aligned(8)));

struct sStr3_1388 { char c[3]; };

struct sVE_1388 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void SetVisible_1388(char* o, int on)
{
    sVE_1388* vt = *(sVE_1388**)(o + 0x8);
    vt[9].fn(vthis(o, vt[9]), on);
}


static inline void* find_1388(cFEStateRuleSelect* self, char* name)
{
    return cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
}

/**
 * Fills the 11 visible rule rows (name text and option list box) from the rule list, starting at a scroll
 * offset.
 *
 * Each rule's kind picks its options: off/on, no/yes, off or 90-60%, +10s to -10s, 0-5, or skill levels 1-3;
 * the
 * list box selects the rule's current setting (cFEStateRuleSelect_getBEData) and rule 0 has none. Plays the "normal" or
 * "greyedout" frame for the selected row, then updates the row colours.
 *
 * @param self_  the rule select state
 * @param first  first rule shown (On Tour MenuOffset)
 */
// Fills the 11 rule rows (name text plus the option list box) starting at the given scroll row.
extern "C" void cFEStateRuleSelect_setupMenu(cFEStateRuleSelect* self_, signed char first)
{
    cFEStateRuleSelect* self = self_;
    signed char i = 0;
    sStr3_1388 name;
    sStr3_1388 label;
    char* lp = label.c;
    name = *(sStr3_1388*)str_0b;
    label = *(sStr3_1388*)str_0t;
    for (; i < 11; i++) {
        char* lb = (char*)find_1388(self, name.c);
        cUIText* txt = (cUIText*)find_1388(self, lp);
        if (lb != 0 && txt != 0) {
            signed char row = i + first;
            if (row >= *(signed char*)&self->mcArraySize)
                break;
            int rule = ((signed char*)self->mpGameEventMapping)[row];
            int kind = D_00441008[rule];
            cUIText_setUnicodeStringByID(txt, GetHashValue32(D_00441018[rule]));
            cUIListBox_deleteAllEntries(lb);
            *(int*)(lb + 0x18) = rule;
            switch (kind) {
            case 0:
                break;
            case 1:
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNOff), 0);
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNOn), 1);
                break;
            case 2:
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNONo), 0);
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNYes), 1);
                break;
            case 3:
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNOff), 0);
                cUIListBox_addEntryByAsciiString(lb, str_90pct, 90);
                cUIListBox_addEntryByAsciiString(lb, str_80pct, 80);
                cUIListBox_addEntryByAsciiString(lb, str_70pct, 70);
                cUIListBox_addEntryByAsciiString(lb, str_60pct, 60);
                break;
            case 4:
                cUIListBox_addEntryByAsciiString(lb, str_10s, 10);
                cUIListBox_addEntryByAsciiString(lb, str_5s, 5);
                cUIListBox_addEntryByAsciiString(lb, str_3s, 3);
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNOff), 0);
                cUIListBox_addEntryByAsciiString(lb, str_3s_b, -3);
                cUIListBox_addEntryByAsciiString(lb, str_5s_b, -5);
                cUIListBox_addEntryByAsciiString(lb, str_10s_b, -10);
                break;
            case 5:
                cUIListBox_addEntryByAsciiString(lb, D_004A13A0, 0);
                cUIListBox_addEntryByAsciiString(lb, D_004A14F8, 0);
                cUIListBox_addEntryByAsciiString(lb, D_004A1500, 0);
                cUIListBox_addEntryByAsciiString(lb, D_004A1508, 0);
                cUIListBox_addEntryByAsciiString(lb, D_004A1510, 0);
                cUIListBox_addEntryByAsciiString(lb, D_004A1518, 0);
                break;
            case 6:
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNSkillLevel1), 0);
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNSkillLevel2), 0);
                cUIListBox_addEntryByStringID(lb, GetHashValue32(str_kT_CMNSkillLevel3), 0);
                break;
            }
            if (rule != 0) {
                unsigned char out;
                cUIListBox_getEntryByData(lb, cFEStateRuleSelect_getBEData((cFEStateRuleSelect*)self, rule), &out);
                cUIListBox_setCurrentEntryByIndex(lb, out);
            } else {
                SetVisible_1388(lb, 0);
            }
        }
        if (i < 9) {
            name.c[0]++;
            label.c[0]++;
        } else {
            label.c[0] = name.c[0] = i + 88;
        }
    }
    unsigned int sel = 0xFF;
    char* menu = (char*)find_1388(self, str_Menu);
    if (menu != 0)
        sel = *(unsigned char*)(menu + 0x95);
    char frame[32];
    if (((signed char*)self->mpGameEventMapping)[sel] == 0) {
        char* arrowLeft = (char*)find_1388(self, str_arrw_left);
        char* arrowRight = (char*)find_1388(self, str_arrw_right);
        if (arrowLeft) SetVisible_1388(arrowLeft, 0);
        if (arrowRight) SetVisible_1388(arrowRight, 0);
        __builtin_memcpy(frame, str_normal, 7);
    } else {
        __builtin_memcpy(frame, str_greyedout, 10);
    }
    int frameIndex = cUIScreen_getFrameByLabel((cUIScreen*)((void*)self->mpScreen), GetHashValue32(frame));
    if (frameIndex != 0xFFFF)
        cUIScreen_playFrame(((void*)self->mpScreen), frameIndex, 1);
    cFEStateRuleSelect_updateMenuColor((cFEStateRuleSelect*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onCreateScreen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cFEStateRuleSelect_setupMenu(cFEStateRuleSelect* self, signed char a1);
extern "C" void cUIMenu_setScrollableMenu(void* menu, int count);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char str_11my_rules[];
extern char str_rule_help[];
extern char str_Menu[];
extern char cBERules_mRules[];
extern signed char D_00441098[11];
extern signed char D_004410A8[10];
extern signed char D_004410B8[11];
extern signed char D_004A1690[8];
extern signed char D_004A1698[8];
extern signed char D_004A16A0[8];
extern char* D_00441058[];
struct sStr3_19188 { char c[3]; };
struct sStr2_19188 { char c[2]; };
// PORT: the local name templates "0b" and "0" are splat-named sdata literals; copied from the symbols.
extern char str_0b[];
extern char D_004A13A0[];


struct sRuleSel_19188 {
    char pad0[0x10];
    void* engine;                   // 0x10
    char pad14[0x2C];
    void* screen;                   // 0x40
    char pad44[0x4];
    signed char* rules;             // 0x48
    char pad4C[0x4];
    signed char count;              // 0x50
    signed char sel[15];            // 0x51
};

static inline void* find_19188(sRuleSel_19188* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
}

/**
 * Builds the custom rules screen ("11my_rules"): picks the rule list for the game event and fills the rows.
 *
 * The list and its length (8-11 rules) depend on the new-race game event (newRace.cGameEvent). Clears the
 * per-rule disable counters and the rule settings (cBERules_mRules), makes the menu scrollable, shows rule 0's
 * help
 * and hides the rows past the list.
 */
// Builds the rule select screen: picks the rule list for the current mode and fills the 11 rule rows.
extern "C" void cFEStateRuleSelect_onCreateScreen(sRuleSel_19188* self)
{
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_11my_rules), 0);
    self->screen = screen;
    if (screen != 0)
        cUIScreen_playFrame(screen, 0, 0);
    self->rules = 0;
    int count = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (cBENewRaceInterface_newRace.cGameEvent) {
    case 0:
        self->rules = D_00441098;
        count = 11;
        break;
    case 1:
        self->rules = D_004410A8;
        count = 10;
        break;
    case 3:
        self->rules = D_004A1690;
        count = 8;
        break;
    case 2:
        self->rules = D_004A1698;
        count = 8;
        break;
    case 4:
        self->rules = D_004410B8;
        count = 11;
        break;
    case 5:
        self->rules = D_004A16A0;
        count = 8;
        break;
    }
    sStr3_19188 name;
    sStr2_19188 label;
    char* lp = label.c;
    int n = 14;
    signed char* disableCounter = &self->sel[14];
    for (; n >= 0; n--)
        *disableCounter-- = 0;
    self->count = count;
    MEM_fill(cBERules_mRules, 0, 0x20);
    cFEStateRuleSelect_setupMenu((cFEStateRuleSelect*)self, 0);
    cUIThing* menu = (cUIThing*)find_19188(self, str_Menu);
    if (menu != 0) {
        menu->mAttr |= 0x80;
        cUIMenu_setScrollableMenu(menu, count);
        cUIMenu_setSelectedByIndex(menu, 0);
        cUIText* help = (cUIText*)find_19188(self, str_rule_help);
        int rule = self->rules[0];
        if (help != 0)
            cUIText_setUnicodeStringByID(help, GetHashValue32(D_00441058[rule]));
    }
    name = *(sStr3_19188*)str_0b;
    label = *(sStr2_19188*)D_004A13A0;
    for (signed char i = 0; i < 11; i++) {
        cUIThing* rowListBox = (cUIThing*)find_19188(self, name.c);
        rowListBox->mAttr &= ~1;
        cUIThing* rowItem = (cUIThing*)find_19188(self, lp);
        if (i >= count) {
            rowItem->setVisible(0);
            rowItem->setStatic(1);
        }
        if (i < 9) {
            label.c[0]++;
            name.c[0]++;
        } else {
            name.c[0] = label.c[0] = i + 88;
        }
    }
}
#endif

extern "C" void* cUIState_onGainFocus(void* self);

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the rule-select state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateRuleSelect_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void* func_001887A0(void* mem, void* owner, int a2);
extern "C" void cFEStateRuleSelect_updateBE(cFEStateRuleSelect* self, int idx, int a2);
struct sRuleSel_2380;
extern "C" void cFEStateRuleSelect_checkConflictForMenu(sRuleSel_2380* self, cUIListBox* item);
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern "C" void MEM_fill(void* dst, int c, int n);
extern char str_FEOptions[];
extern char cBERules_mRules[];


struct sMsg_1C68 {
    char pad_0x0[0x18];
    int id;             // 0x18
    char pad_0x1C[0x60];
    int* info;          // 0x7C
};

/**
 * Handles the rule screen's widget events: open states through the factory, apply an option change, open
 * options.
 *
 * Event 5 (select, when the entry allows it) and event 6 push the engine factory's state for the entry (slot
 * 4 or
 * 5); event 6 also resets the rule settings. Event 9 stores the list box's new value for the rule and updates
 * the
 * rules it disables; event 7 pushes a "FEOptions" state.
 *
 * @param msg  the widget (its rule at +0x18), or 0
 * @param type  widget event number (On Tour Event)
 */
extern "C" void cFEStateRuleSelect_onWidgetEvent(cFEStateRuleSelect* self, sMsg_1C68* msg, unsigned int type)
{
    if (msg == 0) {
        return;
    }
    switch (type) {
    case 5:
        if (msg->info[6] == 0) {
            cUICustomInterface* custom = self->mpEngine->mpCustom;
            void* st = custom->getNextState((cUIState*)self, msg->id);
            if (st) {
                cUIStateStack_push(&self->mpEngine->mStateStack, st);
            }
        }
        break;
    case 6: {
        cUICustomInterface* custom = self->mpEngine->mpCustom;
        void* st = custom->getPrevState((cUIState*)self, msg->id);
        if (st) {
            cUIStateStack_push(&self->mpEngine->mStateStack, st);
        }
        MEM_fill(cBERules_mRules, 0, 0x20);
        break;
    }
    case 9: {
        int id = msg->id;
        cFEStateRuleSelect_updateBE(self, id, cUIListBox_getCurrentEntryData(msg));
        cFEStateRuleSelect_checkConflictForMenu((sRuleSel_2380*)self, (cUIListBox*)msg);
        break;
    }
    case 7: {
        char* st = (char*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), self->mpEngine, 1);
        *(int*)(st + 0x1C) = (*(int*)(st + 0x1C) & ~0x3F00) | 0x100;
        cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, st, 0, 0);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onWidgetPair);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A13A0[];

/**
 * Decides a widget pair event on the rule screen by the pair's name.
 *
 * @param msg  the pair (On Tour cUIPair* pPair)
 * @return 0x101 for the pair named "0", otherwise 0x100
 */
extern "C" int cFEStateRuleSelect_onWidgetPair(cFEStateRuleSelect* self, cUIPair* msg)
{
    int id = msg->mHashName;
    return (id == GetHashValue32(D_004A13A0)) ? 257 : 0x100;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_onWidgetMenu);
#ifdef SKIP_ASM
struct cUIScreen;
struct cUIText;
int GetHashValue32(char* str);
extern "C" void cFEStateRuleSelect_setupMenu(cFEStateRuleSelect* self, signed char idx);
int cUIScreen_getFrameByLabel(cUIScreen* self, int hash);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_arrw_left[];
extern char str_arrw_right[];
extern char str_rule_help[];
extern char* D_00441058[];

// PORT: the frame labels are copied with the builtin block move (the original likely used
// strcpy on string literals); the two strings live at these symbols.
extern char str_greyedout[] __attribute__((aligned(8)));
extern char str_normal[] __attribute__((aligned(8)));

struct sVE_1E48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void SetVisible_1E48(char* o, int on)
{
    sVE_1E48* vt = *(sVE_1E48**)(o + 0x8);
    vt[9].fn(vthis(o, vt[9]), on);
}

/**
 * Handles the rule menu's events: highlight shows the rule's help and arrows, scrolling refills the rows.
 *
 * A rule whose disable counter (+0x51 + rule) is above 0 cannot be highlighted. Event 9 is swallowed.
 *
 * @param pMenu  the menu
 * @param type  menu event number (On Tour Event)
 * @param idx  the item index for event 1, the new scroll offset for event 4
 * @return for event 1, 1 when the rule is enabled and 0 when it is disabled; 0x100 for event 9; otherwise
 *         0x101
 */
extern "C" int cFEStateRuleSelect_onWidgetMenu(cFEStateRuleSelect* self, cUIMenu* pMenu, unsigned int type, int idx)
{
    switch (type) {
    case 9:
        return 0x100;
    case 1: {
        int sel = ((signed char*)self->mpGameEventMapping)[idx];
        int play = 0;
        char* arrowLeft = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrw_left));
        char* arrowRight = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_arrw_right));
        int locked = *(signed char*)(((char*)self) + sel + 0x51) < 1;
        cUIText* textHelp = (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_rule_help));
        if (locked && textHelp) {
            cUIText_setUnicodeStringByID(textHelp, GetHashValue32(D_00441058[sel]));
        }
        char label[32];
        if (sel) {
            if (locked) {
                if (arrowLeft) SetVisible_1E48(arrowLeft, 1);
                if (arrowRight) SetVisible_1E48(arrowRight, 1);
                play = 1;
                __builtin_memcpy(label, str_greyedout, 10);
            }
        } else {
            if (arrowLeft) SetVisible_1E48(arrowLeft, 0);
            if (arrowRight) SetVisible_1E48(arrowRight, 0);
            play = 1;
            __builtin_memcpy(label, str_normal, 7);
        }
        if (play) {
            int frameIndex = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(label));
            if (frameIndex != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, frameIndex, 1);
            }
        }
        return locked;
    }
    case 4:
        cFEStateRuleSelect_setupMenu(self, idx);
        return 257;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_updateBE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int cBERules_mRules_2088[] __asm__("cBERules_mRules");
extern int D_005308D4_2088[] __asm__("D_005308D4");

/**
 * Stores a custom race rule's new setting in the rule settings cBERules_mRules.
 *
 * Rule 2 stores @p ruleValue whole in D_005308D4. The others store bit 0 of @p ruleValue as one flag: rules 3, 7 and 14 in word 0 of
 * cBERules_mRules (bits 3, 4 and 5); rules 4, 8, 1, 5, 6 and 9-13 in word 6 (D_005308D0; bits 0, 1, 2, 3, 4 and 5-9).
 * Unknown rules change nothing and @p self is not used. cFEStateRuleSelect_getBEData reads the same bits back.
 *
 * @param idx  the rule id (1-14)
 * @param ruleValue  the new value: the list box's entry data
 */
extern "C" void cFEStateRuleSelect_updateBE(cFEStateRuleSelect* self, int idx, int ruleValue)
{
    switch (idx) {
    case 4:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -2) | ((ruleValue & 1) << 0);
        break;
    case 3:
        cBERules_mRules_2088[0] = (cBERules_mRules_2088[0] & -9) | ((ruleValue & 1) << 3);
        break;
    case 5:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -9) | ((ruleValue & 1) << 3);
        break;
    case 14:
        cBERules_mRules_2088[0] = (cBERules_mRules_2088[0] & -33) | ((ruleValue & 1) << 5);
        break;
    case 7:
        cBERules_mRules_2088[0] = (cBERules_mRules_2088[0] & -17) | ((ruleValue & 1) << 4);
        break;
    case 8:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -3) | ((ruleValue & 1) << 1);
        break;
    case 1:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -5) | ((ruleValue & 1) << 2);
        break;
    case 2:
        D_005308D4_2088[0] = ruleValue;
        break;
    case 6:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -17) | ((ruleValue & 1) << 4);
        break;
    case 9:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -33) | ((ruleValue & 1) << 5);
        break;
    case 10:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -65) | ((ruleValue & 1) << 6);
        break;
    case 11:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -129) | ((ruleValue & 1) << 7);
        break;
    case 12:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -257) | ((ruleValue & 1) << 8);
        break;
    case 13:
        cBERules_mRules_2088[6] = (cBERules_mRules_2088[6] & -513) | ((ruleValue & 1) << 9);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_getBEData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned int cBERules_mRules_192240[] __asm__("cBERules_mRules");
extern int D_005308D0[];
extern int D_005308D4[];

/**
 * Returns the current setting of a custom race rule from the rule settings cBERules_mRules.
 *
 * Rule 2 returns D_005308D4 whole. Rules 3, 7 and 14 return bits 3, 4 and 5 of cBERules_mRules's word 0; rules 4, 8, 1, 5, 6
 * and 9-13 return bits 0, 1, 2, 3, 4 and 5-9 of D_005308D0 (word 6): the bits cFEStateRuleSelect_updateBE writes. @p self is not
 * used.
 *
 * @param rule  the rule id (1-14)
 * @return the rule's flag (0 or 1) or rule 2's value, or -1 for an unknown rule
 */
extern "C" int cFEStateRuleSelect_getBEData(cFEStateRuleSelect* self, int rule)
{
    switch (rule) {
    case 4:
        return D_005308D0[0] & 1;
    case 3:
        return (cBERules_mRules_192240[0] >> 3) & 1;
    case 5:
        return (D_005308D0[0] >> 3) & 1;
    case 14:
        return (cBERules_mRules_192240[0] >> 5) & 1;
    case 7:
        return (cBERules_mRules_192240[0] >> 4) & 1;
    case 8:
        return (D_005308D0[0] >> 1) & 1;
    case 1:
        return (D_005308D0[0] >> 2) & 1;
    case 2:
        return D_005308D4[0];
    case 6:
        return (D_005308D0[0] >> 4) & 1;
    case 9:
        return (D_005308D0[0] >> 5) & 1;
    case 10:
        return (D_005308D0[0] >> 6) & 1;
    case 11: {
        int shiftedRules = D_005308D0[0] >> 7;
        return shiftedRules & 1;
    }
    case 12:
        return (D_005308D0[0] >> 8) & 1;
    case 13:
        return (D_005308D0[0] >> 9) & 1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_checkConflictForMenu);
#ifdef SKIP_ASM
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" void cFEStateRuleSelect_updateBE(cFEStateRuleSelect* self, int idx, int a2);
extern "C" void cFEStateRuleSelect_updateMenuColor(cFEStateRuleSelect* self);
extern int D_004410C8[];

struct sRuleSel_2380 {
    char pad[0x51];
    unsigned char count[15];
};

/**
 * Updates the rule conflict counters after a rule's list box changed, and turns conflicting rules off.
 *
 * The rule is the list box's mMiscData (+0x18); D_004410C8[rule] << 1 has a bit for each rule (1-14) that may stay on
 * with it, so every rule whose bit is clear conflicts. When the list box's current entry data
 * (cUIListBox_getCurrentEntryData) is 0 each conflicting rule's counter in +0x51 (On Tour mcRuleConflictRefTable) is
 * decremented, not below 0; otherwise each is incremented and that rule's setting is cleared (cFEStateRuleSelect_updateBE with 0).
 * Then the menu is recoloured (cFEStateRuleSelect_updateMenuColor).
 *
 * @param item  the rule's list box that changed
 */
extern "C" void cFEStateRuleSelect_checkConflictForMenu(sRuleSel_2380* self, cUIListBox* item)
{
    int mask = D_004410C8[item->mMiscData] << 1;
    if (cUIListBox_getCurrentEntryData(item) == 0) {
        for (int i = 1; i < 15; i++) {
            int bit = 1 << i;
            if ((bit & mask) == 0) {
                int newCount = self->count[i] - 1;
                self->count[i] = newCount;
                if ((signed char)newCount <= 0) {
                    self->count[i] = 0;
                }
            }
        }
    } else {
        for (int i = 1; i < 15; i++) {
            int bit = 1 << i;
            if ((bit & mask) == 0) {
                self->count[i] += 1;
                cFEStateRuleSelect_updateBE((cFEStateRuleSelect*)self, i, 0);
            }
        }
    }
    cFEStateRuleSelect_updateMenuColor((cFEStateRuleSelect*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cFEStateRuleSelect_updateMenuColor);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

struct sVE_2488 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sQuad_2488 {
    int a, b, c, d;
};
extern sQuad_2488 gStaticColour;


// PORT: the names are built with the builtin block move (the original likely used
// strcpy on string literals); the two strings live at these symbols.
extern char str_0b[];
extern char str_0t[];

/**
 * Greys out the visible rule rows whose rule is disabled by another rule's setting and restores the others.
 *
 * @param selfp  the rule select state
 */
extern "C" void cFEStateRuleSelect_updateMenuColor(cFEStateRuleSelect* selfp)
{
    char name0[16];
    char name1[16];
    __builtin_memcpy(name0, str_0b, 3);
    __builtin_memcpy(name1, str_0t, 3);
    signed char n = *(signed char*)&selfp->mcArraySize < 12 ? *(signed char*)&selfp->mcArraySize : 11;
    for (signed char i = 0; i < n; i++) {
        cUIText* listBox = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpScreen, GetHashValue32(name0));
        cUIText* textBox = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpScreen, GetHashValue32(name1));
        if (listBox && textBox) {
            if (*(signed char*)(((char*)selfp) + listBox->mMiscData + 0x51) > 0) {
                ((sVE_2488*)textBox->mVtbl)[25].fn(vthis(textBox, ((sVE_2488*)textBox->mVtbl)[25]), 1);
                (*(sQuad_2488*)&textBox->mStaticColour) = gStaticColour;
                ((sVE_2488*)textBox->mVtbl)[8].fn(vthis(textBox, ((sVE_2488*)textBox->mVtbl)[8]), 1);
                ((sVE_2488*)listBox->mVtbl)[24].fn(vthis(listBox, ((sVE_2488*)listBox->mVtbl)[24]), 1);
                (*(sQuad_2488*)&listBox->mStaticColour) = gStaticColour;
                ((sVE_2488*)listBox->mVtbl)[8].fn(vthis(listBox, ((sVE_2488*)listBox->mVtbl)[8]), 1);
            } else {
                ((sVE_2488*)textBox->mVtbl)[25].fn(vthis(textBox, ((sVE_2488*)textBox->mVtbl)[25]), 0);
                ((sVE_2488*)textBox->mVtbl)[8].fn(vthis(textBox, ((sVE_2488*)textBox->mVtbl)[8]), 0);
                ((sVE_2488*)listBox->mVtbl)[24].fn(vthis(listBox, ((sVE_2488*)listBox->mVtbl)[24]), 0);
                ((sVE_2488*)listBox->mVtbl)[8].fn(vthis(listBox, ((sVE_2488*)listBox->mVtbl)[8]), 0);
            }
        }
        if (i < 9) {
            name0[0]++;
            name1[0]++;
        } else {
            name1[0] = name0[0] = i + 88;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_cBraggingRightsMenu);
#ifdef SKIP_ASM
extern void* cBraggingRightsMenu_vtbl[];

/**
 * Constructs one bragging-rights choice panel (0x34 bytes; the bragging-rights state holds two at +0x6D0).
 *
 * Stores the vtable cBraggingRightsMenu_vtbl at +0x30 and clears the eight list-box pointers (+0x8), the menu (+0x0), the group
 * (+0x4), the confirmed flag (+0x28) and the row enable mask (+0x2C) (On Tour mpListBox, mpPlayerMenu, mpGroup,
 * mbConfirmed, mcItemEnableFlag).
 *
 * @return @p self
 */
extern "C" void* cBraggingRightsMenu_cBraggingRightsMenu(cBraggingRightsMenu* self)
{
    int i;
    self->mVtbl = cBraggingRightsMenu_vtbl;
    for (i = 7; i >= 0; i--) {
        ((int*)(&self->mpListBox[0]))[i] = 0;
    }
    self->mpPlayerMenu = 0;
    self->mcItemEnableFlag = 0;
    self->mbConfirmed = 0;
    self->mpGroup = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_setConfirmed);
#ifdef SKIP_ASM


/**
 * Sets a bragging-rights panel's confirmed flag (+0x28) and locks or unlocks its list boxes with it.
 *
 * Each of the eight list boxes (+0x8) that exists gets setStatic(@p enabled) (vtable slot 8).
 * cFEStateBraggingRights_onWidgetEvent passes 1 when a row is accepted; cFEStateBraggingRights_onCreateScreen and
 * func_001941B0 pass 0.
 *
 * @param enabled  the new flag: 1 confirms (locks), 0 unlocks
 */
extern "C" void cBraggingRightsMenu_setConfirmed(cBraggingRightsMenu* self, int enabled)
{
    int i;
    self->mbConfirmed = enabled;
    for (i = 0; i < 8; i++) {
        cUIThing* item = ((cUIThing*)self->mpListBox[i]);
        if (item != 0) {
            item->setStatic(enabled);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_setMenuItemColor);
#ifdef SKIP_ASM
extern "C" int cUIListBox_getCurrentEntryData(void* self);
extern "C" int cBraggingRightsMenu_isItemEnable(cBraggingRightsMenu* self, int idx);

struct sColor_001927B0 {
    float r, g, b, a;
    sColor_001927B0(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};
struct sVEnt_001927B0 { short delta; short index; void (*fn)(void*, const sColor_001927B0&); };
struct sRuleSel_001927B0 { void* owner; int f4; cUIThing* items[8]; };

/**
 * Colours a bragging-rights panel's rows: the selected row white, the others by their current value.
 *
 * Does nothing without a menu (+0x0). Skips missing list boxes and rows not enabled (cBraggingRightsMenu_isItemEnable). The row equal to
 * the menu's mScrollItemCurrent (+0x95) gets (A, R, G, B) = (1, 1, 1, 1); a row whose current entry data
 * (cUIListBox_getCurrentEntryData) is non-zero gets (1, 0.592, 0.043, 0) and the rest (1, 0.145, 0.027, 0.020), through
 * setColour (vtable slot 11).
 */
extern "C" void cBraggingRightsMenu_setMenuItemColor(sRuleSel_001927B0* self)
{
    void* owner = self->owner;
    if (owner == 0) {
        return;
    }
    int sel = *(signed char*)((char*)owner + 0x95);
    for (int i = 0; i < 8; i++) {
        if (self->items[i] == 0) {
            continue;
        }
        if (cBraggingRightsMenu_isItemEnable((cBraggingRightsMenu*)self, i) == 0) {
            continue;
        }
        if (i == sel) {
            cUIThing* item = self->items[i];
            sVEnt_001927B0* colorEntry = &((sVEnt_001927B0*)VPTR_OF(cListNode, item))[11];
            colorEntry->fn(vthis(item, *colorEntry), sColor_001927B0(1.0f, 1.0f, 1.0f, 1.0f));
        } else if (cUIListBox_getCurrentEntryData(self->items[i])) {
            cUIThing* item = self->items[i];
            sVEnt_001927B0* colorEntry = &((sVEnt_001927B0*)VPTR_OF(cListNode, item))[11];
            colorEntry->fn(vthis(item, *colorEntry), sColor_001927B0(1.0f, 0.5921568870544434f, 0.04313725605607033f, 0.0f));
        } else {
            cUIThing* item = self->items[i];
            sVEnt_001927B0* colorEntry = &((sVEnt_001927B0*)VPTR_OF(cListNode, item))[11];
            colorEntry->fn(vthis(item, *colorEntry), sColor_001927B0(1.0f, 0.14509804546833038f, 0.027450982481241226f, 0.019607843831181526f));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_findListBoxP);
#ifdef SKIP_ASM
/**
 * Returns the row index (0-7) of a list box in a bragging-rights panel's eight list boxes (+0x8), or -1.
 *
 * @param val  the list box to look for
 * @return its index, or -1 when the panel does not hold it
 */
extern "C" int cBraggingRightsMenu_findListBoxP(cBraggingRightsMenu* self, int val)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (((int*)(&self->mpListBox[0]))[i] == val) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_isItemEnable);
#ifdef SKIP_ASM
/**
 * Tells whether a bragging-rights panel's row is enabled in its enable mask (+0x2C).
 *
 * Quirk: the mask is built as (0x1000000 << @p itemIndex) >> 24, a sign-extended byte, so for row 7 the test also
 * covers bits 8-31.
 *
 * @param itemIndex  the row (0-7)
 * @return 1 when the row's bit is set, else 0
 */
extern "C" int cBraggingRightsMenu_isItemEnable(cBraggingRightsMenu* self, int itemIndex)
{
    int mask = (int)(0x1000000 << itemIndex) >> 24;
    return (self->mcItemEnableFlag & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_setEnableFlag__FPvi);
#ifdef SKIP_ASM
/**
 * Stores a bragging-rights panel's row enable mask (+0x2C).
 *
 * cFEStateBraggingRights_onCreateScreen passes a bit for each rule row of the current event.
 *
 * @param val  the mask, bit i for row i
 */
void cBraggingRightsMenu_setEnableFlag(void* vself, int val)
{
    cBraggingRightsMenu* self = (cBraggingRightsMenu*)vself;
    self->mcItemEnableFlag = val;
}
#endif

//100%
INCLUDE_ASM("fe/festateruleselect", cBraggingRightsMenu_setEnableItem);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

struct sName11_00192970 { char c[11]; };
struct sName12_00192970 { char c[12]; };
struct sName13_00192970 { char c[13]; };
struct sName4_00192970 { char c[4]; };
struct sName2_00192970 { char c[2]; };
struct sName5_00192970 { char c[5]; };

extern char str_0arrw_left[];
extern char str_0arrw_right[];
extern char str_0arrw_left1[];
extern char str_0arrw_right1[];
extern char str_w11[];
extern char str_w21[];
extern char D_004A14F8[];
extern char str_box1[];

class cFEVObj_00192970 {
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
    virtual void setStatic(int on);
    virtual void setVisible(int on);
};

static inline void Hide_00192970(void* screen, char* name)
{
    cFEVObj_00192970* t = (cFEVObj_00192970*)cUIScreen_getObjectByHashName(screen, GetHashValue32(name));
    if (t != 0) {
        t->setVisible(0);
    }
}

struct sOwner_00192970 {
    char pad[0x8];
    cFEVObj_00192970* items[8];     // 0x8
    int f28;                        // 0x28
    int mask;                       // 0x2C
};

/**
 * Locks and hides the rows that a bragging-rights panel's enable mask leaves out, with their arrows and labels.
 *
 * For each row i (0-7) whose bit in +0x2C is clear and whose list box exists, the list box gets setStatic(1) and
 * setVisible(0) (vtable slots 8 and 9), and the objects named "<i>arrw_left", "<i>arrw_right", "<i>arrw_left1",
 * "<i>arrw_right1", the row label "<i+1>", "box<i+1>", "w1<i+1>" and "w2<i+1>" are hidden on @p screen (setVisible(0)).
 * The names are stack copies of the strings with the digit patched in.
 *
 * @param screen  the screen holding the panel's widgets
 */
extern "C" void cBraggingRightsMenu_setEnableItem(sOwner_00192970* self, cUIScreen* screen)
{
    sName11_00192970 leftArrowName = *(sName11_00192970*)str_0arrw_left;
    sName12_00192970 rightArrowName = *(sName12_00192970*)str_0arrw_right;
    sName12_00192970 leftArrow1Name = *(sName12_00192970*)str_0arrw_left1;
    sName13_00192970 rightArrow1Name = *(sName13_00192970*)str_0arrw_right1;
    sName4_00192970 w1Name = *(sName4_00192970*)str_w11;
    sName4_00192970 w2Name = *(sName4_00192970*)str_w21;
    sName2_00192970 rowLabelName = *(sName2_00192970*)D_004A14F8;
    sName5_00192970 boxName = *(sName5_00192970*)str_box1;
    for (int i = 0; i < 8; i++) {
        if (((self->mask >> i) & 1) == 0) {
            if (self->items[i] != 0) {
                self->items[i]->setStatic(1);
                self->items[i]->setVisible(0);
                leftArrowName.c[0] = i + '0';
                rightArrowName.c[0] = i + '0';
                leftArrow1Name.c[0] = i + '0';
                rightArrow1Name.c[0] = i + '0';
                rowLabelName.c[0] = i + '1';
                boxName.c[3] = i + '1';
                w1Name.c[2] = i + '1';
                w2Name.c[2] = i + '1';
                Hide_00192970(screen, leftArrowName.c);
                Hide_00192970(screen, rightArrowName.c);
                Hide_00192970(screen, leftArrow1Name.c);
                Hide_00192970(screen, rightArrow1Name.c);
                Hide_00192970(screen, rowLabelName.c);
                Hide_00192970(screen, boxName.c);
                Hide_00192970(screen, w1Name.c);
                Hide_00192970(screen, w2Name.c);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateruleselect", cFEStateBraggingRights_cFEStateBraggingRights);
#ifdef SKIP_ASM
extern void* cFEStateBraggingRights_vtbl[];
extern "C" void* cFEStateNet_cFEStateNet(void* self, int a1, int a2);

/**
 * Constructs the bragging-rights state (allocated as "cFEStateBraggingRights", 1852 bytes).
 *
 * Runs cFEStateNet_cFEStateNet with @p engine and 0, stores cFEStateBraggingRights_vtbl, builds the two choice panels
 * at +0x6D0 and +0x704 (cBraggingRightsMenu_cBraggingRightsMenu), sets mHashName 20 and clears +0x738.
 *
 * @param engine  the UI engine
 * @return @p self
 */
extern "C" void* cFEStateBraggingRights_cFEStateBraggingRights(cFEStateBraggingRights* self, int engine)
{
    char* ruleSel;
    int i;
    cFEStateNet_cFEStateNet(self, engine, 0);
    self->mVtbl = cFEStateBraggingRights_vtbl;
    ruleSel = (char*)&self->mMenuCtrl[0];
    for (i = 1; i != -1; i--, ruleSel += 0x34) {
        cBraggingRightsMenu_cBraggingRightsMenu((cBraggingRightsMenu*)ruleSel);
    }
    self->mHashName = 20;
    self->mpGameEventMapping = 0;
    return ((void*)self);
}
#endif

