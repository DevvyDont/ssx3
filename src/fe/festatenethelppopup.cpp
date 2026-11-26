#include "vtable.h"
#include "common.h"
#include "util/menu.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "util/locale.h"
#include "ui/uithing.h"
#include "fe/debuglobbymenus.h"
#include "fe/festatenethelppopup.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_help_popup[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the online help popup's screen ("help_popup") and plays its first frame.
 */
void cFEStateNetHelpPopup::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_help_popup), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_handleEvent);
#ifdef SKIP_ASM

/**
 * Forwards an event to the state that opened the help popup.
 *
 * When +0x50 is non-zero and the parent state (mpParentState, +0x20) exists, returns the parent's virtual slot 24
 * (handleEvent) with @p msg and @p msgParam; otherwise returns 0.
 *
 * @param msg       the event id (On Tour: int msg)
 * @param msgParam  the event data (On Tour: void *pParam)
 * @return the parent's result, or 0
 */
extern "C" int cFEStateNetHelpPopup_handleEvent(cFEStateNetHelpPopup* self, int msg, int msgParam)
{
    if (self->mbReferEventsToParent != 0) {
        cUIState* obj = self->mpParentState;
        if (((void*)obj) != 0) {
            return obj->handleEvent(msg, (void*)msgParam);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onUpdate);
#ifdef SKIP_ASM
extern "C" float cInput_getValue(void* self, int id);
extern "C" void cUIStateStack_pop(void*, int);

/**
 * Closes the help popup once neither controller holds input 0x99, probably the help button that opened it.
 */
void cFEStateNetHelpPopup::onUpdate()
{
    int i;
    int found = 0;
    for (i = 0; i < 2; i++) {
        int** input = (int**)SSXApp->Inputs[i];
        int ok = *input != 0 && **input != 0;
        if (ok && cInput_getValue(input, 153) != 0.0f) {
            found = 1;
        }
    }
    if (found == 0) {
        cUIStateStack_pop(&mpEngine->mStateStack, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Consumes all controller input for the network help popup before its screens see it.
 *
 * The base UI update cUIState_onUpdate() skips the state's screens when this returns nonzero; onInputAlways still
 * runs.
 *
 * @return always 1 (input consumed)
 */
int cFEStateNetHelpPopup_onInputBegin(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_OnlineAccSetup[];
extern char str_TopText[];
extern char str_BottomNote[];
extern char str_xselect[];
extern char str_ps2triangle[];
extern char str_ps2circle[];
extern char str_updownarrows[];
extern char str_leftrightarrows[];
extern char str_ps2l1[];
extern char str_ps2r1[];
extern char str_actrl[];
extern char str_ps2sel[];
extern char str_TextOptionpct_d[];


struct sNetHelp_1D9F38 {
    char pad00[0xA8];
    cUIThing* wA8;
    cUIThing* wAC;
    cUIThing* wB0;
    cUIThing* rows[7];
    cUIThing* wD0;
    cUIThing* wD4;
    cUIThing* wD8;
    cUIThing* wDC;
    cUIThing* wE0;
    cUIThing* wE4;
    cUIThing* wE8;
    cUIThing* wEC;
    cUIThing* wF0;
};

static inline int IsHash_1D9F38(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Binds the help popup's widgets by name: title, body and bottom note texts, the button icons and the seven
 * option rows.
 *
 * @param w  the widget being created
 */
void cFEStateNetHelpPopup::onWidgetCreate(cUIThing* w)
{
    int hashName = w->mHashName;
    if (hashName == GetHashValue32(str_OnlineAccSetup)) {
        mpScreenName = (cUIText*)w;
        (*(unsigned int*)&w->mAttr) |= 0x80;
    } else if (hashName == GetHashValue32(str_TopText)) {
        mpDescription = (cUIText*)w;
        (*(unsigned int*)&w->mAttr) |= 0x80;
    } else if (hashName == GetHashValue32(str_BottomNote)) {
        mpBottom = (cUIText*)w;
        (*(unsigned int*)&w->mAttr) |= 0x80;
    } else if (IsHash_1D9F38(w->mHashName, str_xselect)) {
        mpObject[0] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_ps2triangle)) {
        mpObject[1] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_ps2circle)) {
        mpObject[2] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_updownarrows)) {
        mpObject[3] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_leftrightarrows)) {
        mpObject[4] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_ps2l1)) {
        mpObject[6] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_ps2r1)) {
        mpObject[5] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_actrl)) {
        mpObject[8] = (cUIIcon*)w;
    } else if (IsHash_1D9F38(w->mHashName, str_ps2sel)) {
        mpObject[7] = (cUIIcon*)w;
    } else {
        char buf[16];
        for (int i = 0; i < 7; i++) {
            sprintf(buf, str_TextOptionpct_d, i);
            if (hashName == GetHashValue32(buf)) {
                mpOptions[i] = (cUIText*)w;
                break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_createAndSetDescriptionString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cMemMan_free__FPv(void* p);
extern "C" int USTR_length(unsigned short* s);
extern "C" void func_002C2688(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_setUnicodeStringWrap(void* text, unsigned short* str);
extern char str_cFEStateNetHelpPopup[];



/**
 * Builds the help popup's description: concatenates its (up to three) localised description strings and shows them in
 * the popup's text.
 *
 * Looks up each non-zero description id at +0x58 (stopping at the first 0) through SSXApp's FELocale (slot 4),
 * allocates one buffer of the total length (tag "cFEStateNetHelpPopup"), appends the strings with func_002C2688(),
 * hands it to cUIText_setUnicodeStringWrap() on the text at +0xAC and frees it again. With no description the text gets
 * a null string. cFEStateNetHelpPopup_onGainTransition() calls it.
 *
 */
void cFEStateNetHelpPopup::createAndSetDescriptionString()
{
    unsigned short* strs[3];
    unsigned short* buf = 0;
    int total = 0;
    int count = 0;
    int* ids = (int*)(&muiDescription[0]);
    int i = 0;
    do {
        if (ids[i] == 0) {
            break;
        }
        cFELocale* locale = SSXApp->FELocale;
        strs[i] = locale->getStringByHash(ids[i]);
        count++;
        total += USTR_length(strs[i]) + 1;
        i++;
    } while (i < 3);
    if (total > 0) {
        buf = new (str_cFEStateNetHelpPopup, 0x100, 0) unsigned short[total];
        buf[0] = 0;
        for (int i = 0; i < count; i++) {
            func_002C2688(buf, strs[i]);
        }
    }
    cUIText_setUnicodeStringWrap(mpDescription, buf);
    if (buf != 0) {
        cMemMan_free__FPv(buf);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onGainTransition);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cFEStateNetHelpPopup_createAndSetDescriptionString(cFEStateNetHelpPopup* self);
extern char str_empty_d[];

struct sVEnt_1DA238 { short delta; short index; void (*fn)(void*, int); };

struct sWidget_1DA238 {
    char pad00[8];
    sVEnt_1DA238* vt;
    char pad0C[0x38];
    t3Vector pos;
};

struct sNetHelp_1DA238 {
    char pad00[0x48];
    int state;                  // 0x48
    char pad4C[8];
    int title;                  // 0x54
    int c[3];                   // 0x58
    int muiBottomText;                    // 0x64
    int a[7];                   // 0x68
    int b[7];                   // 0x84
    char padA0[8];
    sWidget_1DA238* wA8;        // 0xA8
    sWidget_1DA238* wAC;        // 0xAC
    sWidget_1DA238* wB0;        // 0xB0
    sWidget_1DA238* rows[7];    // 0xB4
    sWidget_1DA238* icons[9];   // 0xD0
};

static inline void show_1DA238(sWidget_1DA238* w, int on)
{
    w->vt[9].fn(vthis(w, w->vt[9]), on);
}

/**
 * Fills the help popup on its first transition: title, body, bottom note and one row per option with its
 * button icon.
 *
 * Rows past the option count are hidden; each shown row's icon is moved down 22 units per row. A missing
 * string
 * ID leaves the text empty; the body is built by cFEStateNetHelpPopup_createAndSetDescriptionString().
 *
 * @param bFirstTime  nonzero on the first transition
 * @return 1
 */
extern "C" int cFEStateNetHelpPopup_onGainTransition(sNetHelp_1DA238* self, int bFirstTime)
{
    if (bFirstTime) {
        if (self->title) {
            cUIText_setUnicodeStringByID((cUIText*)self->wA8, self->title);
        } else {
            cUIText_setAsciiString((cUIText*)self->wA8, str_empty_d);
        }
        if (self->c[0]) {
            ((cFEStateNetHelpPopup*)self)->createAndSetDescriptionString();
        } else {
            cUIText_setAsciiString((cUIText*)self->wAC, str_empty_d);
        }
        if (self->muiBottomText) {
            cUIText_setUnicodeStringByID((cUIText*)self->wB0, self->muiBottomText);
        } else {
            cUIText_setAsciiString((cUIText*)self->wB0, str_empty_d);
        }
        for (int j = 0; j < 9; j++) {
            show_1DA238(self->icons[j], 0);
        }
        for (int i = 0; i < 7; i++) {
            if (i < self->state) {
                show_1DA238(self->rows[i], 1);
                show_1DA238(self->icons[self->b[i]], 1);
                t3Vector objectPos = self->icons[self->b[i]]->pos;
                objectPos.y += (float)i * 22.0f;
                self->icons[self->b[i]]->pos = objectPos;
                if (self->a[i]) {
                    cUIText_setUnicodeStringByID((cUIText*)self->rows[i], self->a[i]);
                } else {
                    cUIText_setAsciiString((cUIText*)self->rows[i], str_empty_d);
                }
            } else {
                show_1DA238(self->rows[i], 0);
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_pop(void*, int);

/**
 * Closes the help popup on widget event kONPREV.
 *
 * @param pWidget  the widget, unused
 * @param msg  widget event number
 */
extern "C" void cFEStateNetHelpPopup_onWidgetEvent(cFEStateNetHelpPopup* self, int pWidget, int msg)
{
    if (msg == cUIState::kONPREV) {
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_clear);
#ifdef SKIP_ASM

/**
 * Clears the help popup's contents: no options, no descriptions, every option's text 0 and icon -1.
 *
 * Sets the option count (+0x48, see cFEStateNetHelpPopup_setNumHelpOptions()), +0x54, the three description ids (+0x58)
 * and +0x64 to 0, and the seven option text ids (+0x68) to 0 and icons (+0x84) to -1 (see
 * cFEStateNetHelpPopup_setOption()). The help popup's constructor cFEStateNetHelpPopup_cFEStateNetHelpPopup() calls it.
 */
void cFEStateNetHelpPopup::clear()
{
    cFEStateNetHelpPopup* helpPopup = this;
    int i;
    helpPopup->miNumOptions = 0;
    helpPopup->muiScreenName = 0;
    for (i = 2; i >= 0; i--) {
        helpPopup->muiDescription[i] = 0;
    }
    helpPopup->muiBottomText = 0;
    for (i = 0; i < 7; i++) {
        helpPopup->muiOptions[i] = 0;
        helpPopup->meObject[i] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_setOption__FPviii);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA510 {
    char pad[0x68];
    int a[7];
    int b[7];
};

/**
 * Sets option row @p optionNum of the help popup: its text id (table at +0x68) and its button icon (table at +0x84).
 *
 * @param optionNum  the row, 0-6
 * @param textId  the text id (hash of a "kT_HELP..." name)
 * @param iconIndex  the button icon to show next to it
 */
void cFEStateNetHelpPopup_setOption(void* self, int optionNum, int textId, int iconIndex)
{
    sNetHelpPopup_DA510* popup = (sNetHelpPopup_DA510*)self;
    popup->a[optionNum] = textId;
    popup->b[optionNum] = iconIndex;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_setNumHelpOptions__FPvi);
#ifdef SKIP_ASM
/**
 * Sets how many option rows the help popup shows (+0x48).
 *
 * @param numOptions  the number of rows set with cFEStateNetHelpPopup_setOption (up to 7)
 */
void cFEStateNetHelpPopup::setNumHelpOptions(int numOptions)
{
    miNumOptions = numOptions;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_setDescription__FPvii);
#ifdef SKIP_ASM
/**
 * Sets description line @p descriptionNum of the help popup (the text-id table at +0x58, three entries) to
 * @p description.
 *
 * Callers pass GetHashValue32 of a "kT_HELP..." text name; the popup stops at the first 0 entry when it builds its
 * text.
 *
 * @param descriptionNum  the line, 0-2
 * @param description  the text id (hash)
 */
void cFEStateNetHelpPopup::setDescription(int descriptionNum, int description)
{
    *(int*)((char*)this + (descriptionNum << 2) + 0x58) = description;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA540);
#ifdef SKIP_ASM
/**
 * Returns its first argument unchanged.
 *
 * Declared variadic: the body only spills the remaining argument registers (integer and float) to its stack frame
 * before returning. Nothing in the source calls it.
 *
 * @return @p self
 */
extern "C" void* func_001DA540(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA580);
#ifdef SKIP_ASM
/**
 * Compares two character ranges lexicographically and reports whether the first is less than the second.
 *
 * Characters compare as signed chars; a range that is a proper prefix of the other is less. This is a char
 * instantiation of the STL lexicographical_compare; nothing in the source calls it.
 *
 * @param first1  start of the first range
 * @param last1  end of the first range
 * @param first2  start of the second range
 * @param last2  end of the second range
 * @return true when the first range sorts before the second
 */
extern "C" bool func_001DA580(const char* first1, const char* last1, const char* first2, const char* last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    return first1 == last1 && first2 != last2;
}
#endif
//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA5E8);
#ifdef SKIP_ASM
/**
 * Compares two character ranges lexicographically, three-way.
 *
 * Characters compare as signed chars; a proper prefix sorts first. Nothing in the source calls it.
 *
 * @param first1  start of the first range
 * @param last1  end of the first range
 * @param first2  start of the second range
 * @param last2  end of the second range
 * @return -1 when the first range sorts first, 1 when it sorts after, 0 when the ranges are equal
 */
extern "C" int func_001DA5E8(const char* first1, const char* last1, const char* first2, const char* last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return -1;
        if (*first2 < *first1) return 1;
    }
    if (first2 == last2) return first1 != last1;
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", AccountDetailsMenu_AccountDetailsMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cTextMenuItem_cTextMenuItem(void* self, void* a1, const char* name);
extern "C" void* cCommandMenuItem_cCommandMenuItem2(void* self, int a1, void* a2, int a3);
extern "C" void* cPopMenuItem_cPopMenuItem(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern "C" char* strncat(char* dst, const char* src, int n);
extern void* AccountDetailsMenu_vtbl[];
extern char str_Create_Account[];
extern char str_Login_Name[];
extern char str_Password[];
extern char str_Cancel[];
extern char str_empty_d[];

static inline void copyName_1DA648(char* dst, const char* src)
{
    dst[0] = 0;
    strncat(dst, src, 0xF);
}

/**
 * Constructs the debug "Create Account" menu: a title, a spacer, "Login Name" and "Password" entries pre-filled from
 * SSXApp, an action item and "Cancel".
 *
 * Runs cMenu_cMenu(), stores vtable AccountDetailsMenu_vtbl (onCommand AccountDetailsMenu_onCommand, which creates the account) and the owner from
 * @p vinfo at +0x130, and builds the items in place: the "Create Account" title (cNullMenuItem), a spacer of 10, two
 * text entries (cTextMenuItem_cTextMenuItem()) labelled "Login Name" (+0x168) and "Password" (+0x194), an action item labelled @p actionLabel
 * (cCommandMenuItem_cCommandMenuItem2(), +0x1C0) and a "Cancel" item (cPopMenuItem_cPopMenuItem(), +0x1E4), adding all six in that order. The entries'
 * text buffers (+0x180, +0x1AC) get up to 15 characters of SSXApp's UserName and Password. AccountDetailsMenu_AccountDetailsMenu1 is the same
 * menu with the Login vtable; AccountMenu_AccountMenu embeds both.
 *
 * @param vinfo  a holder whose first word (the debug front end) is stored at +0x130
 * @param actionLabel  the action item's label
 * @return @p self
 */
extern "C" void* AccountDetailsMenu_AccountDetailsMenu(AccountDetailsMenu* self, void* vinfo, void* actionLabel)
{
    cMenu_cMenu(self);
    self->mVtbl = AccountDetailsMenu_vtbl;
    self->mScreen = (cDebugLobbyScreen*)(*(int*)vinfo);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Create_Account);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)10);
    cTextMenuItem_cTextMenuItem(&self->mLoginNameItem, str_Login_Name, str_empty_d);
    cTextMenuItem_cTextMenuItem(&self->mPasswordItem, str_Password, str_empty_d);
    cCommandMenuItem_cCommandMenuItem2(&self->mActionItem, 0, actionLabel, 0);
    cPopMenuItem_cPopMenuItem(&self->mCancelItem, str_Cancel);
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    cMenu_addItem(self, &self->mLoginNameItem, -1);
    cMenu_addItem(self, &self->mPasswordItem, -1);
    cMenu_addItem(self, &self->mActionItem, -1);
    cMenu_addItem(self, &self->mCancelItem, -1);
    copyName_1DA648((char*)&self->mLoginNameItem.Text[0], SSXApp->UserName);
    copyName_1DA648((char*)&self->mPasswordItem.Text[0], SSXApp->Password);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", AccountDetailsMenu_AccountDetailsMenu1);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cTextMenuItem_cTextMenuItem(void* self, void* a1, const char* name);
extern "C" void* cCommandMenuItem_cCommandMenuItem2(void* self, int a1, void* a2, int a3);
extern "C" void* cPopMenuItem_cPopMenuItem(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern "C" char* strncat(char* dst, const char* src, int n);
extern void* AccountDetailsMenu1_vtbl[];
extern char str_Create_Account[];
extern char str_Login_Name[];
extern char str_Password[];
extern char str_Cancel[];
extern char str_empty_d[];

static inline void copyName_1DA7E8(char* dst, const char* src)
{
    dst[0] = 0;
    strncat(dst, src, 0xF);
}

/**
 * Constructs the debug "Login" menu: the same items as AccountDetailsMenu_AccountDetailsMenu ("Create Account" title, "Login Name", "Password",
 * an action item, "Cancel") with the Login vtable.
 *
 * Runs cMenu_cMenu(), stores vtable AccountDetailsMenu1_vtbl (onCommand AccountDetailsMenu_onCommand1, which logs in with the typed name and
 * password) and the owner from @p vinfo at +0x130, builds and adds the six items, and pre-fills the name and password
 * entries (+0x180, +0x1AC) with up to 15 characters of SSXApp's UserName and Password. The title is "Create Account"
 * here too (likely a copy-paste slip; AccountMenu_AccountMenu labels the sub-menu "Login").
 *
 * @param vinfo  a holder whose first word (the debug front end) is stored at +0x130
 * @param actionLabel  the action item's label
 * @return @p self
 */
void* AccountDetailsMenu::AccountDetailsMenu1(void* vinfo, void* actionLabel)
{
    cMenu_cMenu(this);
    mVtbl = AccountDetailsMenu1_vtbl;
    mScreen = (cDebugLobbyScreen*)(*(int*)vinfo);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&mTitleItem), str_Create_Account);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&mSpaceItem), (void*)10);
    cTextMenuItem_cTextMenuItem(&mLoginNameItem, str_Login_Name, str_empty_d);
    cTextMenuItem_cTextMenuItem(&mPasswordItem, str_Password, str_empty_d);
    cCommandMenuItem_cCommandMenuItem2(&mActionItem, 0, actionLabel, 0);
    cPopMenuItem_cPopMenuItem(&mCancelItem, str_Cancel);
    cMenu_addItem(this, &mTitleItem, -1);
    cMenu_addItem(this, &mSpaceItem, -1);
    cMenu_addItem(this, &mLoginNameItem, -1);
    cMenu_addItem(this, &mPasswordItem, -1);
    cMenu_addItem(this, &mActionItem, -1);
    cMenu_addItem(this, &mCancelItem, -1);
    copyName_1DA7E8((char*)&mLoginNameItem.Text[0], SSXApp->UserName);
    copyName_1DA7E8((char*)&mPasswordItem.Text[0], SSXApp->Password);
    return ((void*)this);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatenethelppopup", AccountMenu_AccountMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* AccountDetailsMenu_AccountDetailsMenu(AccountDetailsMenu* self, void* a1, void* a2);
extern "C" void* AccountDetailsMenu_AccountDetailsMenu1(AccountDetailsMenu* self, void* a1, void* a2);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* AccountMenu_vtbl[];
extern char str_Create_Account[];
extern char str_Login[];
extern char str_Login_Method[];

/**
 * Constructs the debug "Login Method" menu with "Create Account" and "Login" sub-menus.
 *
 * Runs cMenu_cMenu(), stores vtable AccountMenu_vtbl, builds the Create Account menu (AccountDetailsMenu_AccountDetailsMenu()) at +0x130 and the
 * Login menu (AccountDetailsMenu_AccountDetailsMenu1()) at +0x32C, labelling their action items "Create Account" and "Login", then adds the
 * "Login Method" title, a spacer of 10 and two cSubMenuItems opening those menus. cDebugLobbyScreen_cDebugLobbyScreen() builds it at +0x70 of
 * the debug front end.
 *
 * @param createOwner  owner holder passed to the Create Account menu
 * @param loginOwner  owner holder passed to the Login menu
 * @return @p self
 */
extern "C" void* AccountMenu_AccountMenu(AccountMenu* self, void* createOwner, void* loginOwner)
{
    cMenu_cMenu(self);
    self->mVtbl = AccountMenu_vtbl;
    AccountDetailsMenu_AccountDetailsMenu((AccountDetailsMenu*)(&self->mCreateAccountMenu), createOwner, str_Create_Account);
    self->mLoginMenu.AccountDetailsMenu1(loginOwner, str_Login);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Login_Method);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)10);
    cSubMenuItem_cSubMenuItem(&self->mCreateAccountItem, &self->mCreateAccountMenu, str_Create_Account);
    cSubMenuItem_cSubMenuItem(&self->mLoginItem, &self->mLoginMenu, str_Login);
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    cMenu_addItem(self, &self->mCreateAccountItem, -1);
    cMenu_addItem(self, &self->mLoginItem, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", CreatePersonaMenu_CreatePersonaMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cTextMenuItem_cTextMenuItem(void* self, void* a1, const char* name);
extern "C" void* cCommandMenuItem_cCommandMenuItem2(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* CreatePersonaMenu_vtbl[];
extern char str_Create_Persona[];
extern char str_Persona_Name[];
extern char str_empty_d[];
extern char str_Create[];

/**
 * Constructs the debug "Create Persona" menu: a title, a spacer, a "Persona Name" entry and a "Create" item.
 *
 * Runs cMenu_cMenu(), stores vtable CreatePersonaMenu_vtbl (onCommand CreatePersonaMenu_onCommand, which creates the persona) and the owner from
 * @p info at +0x130, builds the "Create Persona" title, a spacer of 14, a text entry labelled "Persona Name"
 * (cTextMenuItem_cTextMenuItem(), +0x168) and a "Create" action item (cCommandMenuItem_cCommandMenuItem2(), +0x194), and adds them in that order.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* CreatePersonaMenu_CreatePersonaMenu(CreatePersonaMenu* self, int* info)
{
    cMenu_cMenu(self);
    self->mVtbl = CreatePersonaMenu_vtbl;
    self->mScreen = (cDebugLobbyScreen*)(*info);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Create_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    cTextMenuItem_cTextMenuItem(&self->mPersonaNameItem, str_Persona_Name, str_empty_d);
    cCommandMenuItem_cCommandMenuItem2(&self->mCreateItem, 0, str_Create, 0);
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    cMenu_addItem(self, &self->mPersonaNameItem, -1);
    cMenu_addItem(self, &self->mCreateItem, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", DeletePersonaMenu_DeletePersonaMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cCommandMenuItem_cCommandMenuItem(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* DeletePersonaMenu_vtbl[];
extern char str_Delete_Persona[];

/**
 * Constructs the debug "Delete Persona" menu: a title, a spacer and four blank persona lines.
 *
 * Runs cMenu_cMenu(), stores vtable DeletePersonaMenu_vtbl (onCommand DeletePersonaMenu_onCommand, which deletes the chosen persona) and the
 * owner from @p info at +0x130, builds the "Delete Persona" title and a spacer of 14, constructs four empty command
 * items (cCommandMenuItem_cCommandMenuItem()) at +0x168 (0x24 bytes apart) and adds all six. DeletePersonaMenu_setpersonas later fills in the persona
 * lines.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* DeletePersonaMenu_DeletePersonaMenu(DeletePersonaMenu* self, int* info)
{
    char* personaItem = (char*)&self->mPersonaItems[0];
    int i = 3;
    cMenu_cMenu(self);
    self->mVtbl = DeletePersonaMenu_vtbl;
    self->mScreen = (cDebugLobbyScreen*)(*info);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Delete_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    for (; i != -1; i--) {
        cCommandMenuItem_cCommandMenuItem(personaItem);
        personaItem += 0x24;
    }
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    int j;
    for (j = 0; j < 4; j++) {
        cMenu_addItem(self, &self->mPersonaItems[j], -1);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", PersonaMenu_PersonaMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
// PORT: the member ctors return self, but this caller treats them as void
void CreatePersonaMenu_CreatePersonaMenu_v(void* self, int* info) __asm__("CreatePersonaMenu_CreatePersonaMenu");
void DeletePersonaMenu_DeletePersonaMenu_v(void* self, int* info) __asm__("DeletePersonaMenu_DeletePersonaMenu");
extern "C" void* cCommandMenuItem_cCommandMenuItem(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* PersonaMenu_vtbl[];
extern char str_Select_Persona[];
extern char str_Create_Persona[];
extern char str_Delete_Persona[];


/**
 * Constructs the debug "Select Persona" menu with four persona lines and "Create Persona" / "Delete Persona" sub-menus.
 *
 * Runs cMenu_cMenu(), stores vtable PersonaMenu_vtbl (onCommand PersonaMenu_onCommand, which selects the chosen persona), builds the
 * Create Persona menu (CreatePersonaMenu_CreatePersonaMenu()) at +0x130 and the Delete Persona menu (DeletePersonaMenu_DeletePersonaMenu()) at +0x2E8, stores the
 * owner from @p info at +0x4E0, then builds the "Select Persona" title, a spacer of 14, four empty command items
 * (cCommandMenuItem_cCommandMenuItem()) at +0x518 and cSubMenuItems for the two sub-menus, adding them in that order. PersonaMenu_setpersonas fills
 * in the persona lines; cDebugLobbyScreen_cDebugLobbyScreen() builds it at +0x74C of the debug front end.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x4E0
 * @param createOwner  owner holder passed to the Create Persona menu
 * @param deleteOwner  owner holder passed to the Delete Persona menu
 * @return @p self
 */
extern "C" void* PersonaMenu_PersonaMenu(PersonaMenu* self, int* info, int* createOwner, int* deleteOwner)
{
    cMenu_cMenu(self);
    self->mVtbl = PersonaMenu_vtbl;
    CreatePersonaMenu_CreatePersonaMenu_v(&self->mCreatePersonaMenu, createOwner);
    DeletePersonaMenu_DeletePersonaMenu_v(&self->mDeletePersonaMenu, deleteOwner);
    self->mScreen = (cDebugLobbyScreen*)(*info);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Select_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    char* personaItem = (char*)&self->mPersonaItems[0];
    for (int i = 3; i != -1; i--) {
        cCommandMenuItem_cCommandMenuItem(personaItem);
        personaItem += 0x24;
    }
    cSubMenuItem_cSubMenuItem(&self->mCreatePersonaItem, &self->mCreatePersonaMenu, str_Create_Persona);
    cSubMenuItem_cSubMenuItem(&self->mDeletePersonaItem, &self->mDeletePersonaMenu, str_Delete_Persona);
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    int j;
    for (j = 0; j < 4; j++) {
        cMenu_addItem(self, &self->mPersonaItems[j], -1);
    }
    cMenu_addItem(self, &self->mCreatePersonaItem, -1);
    cMenu_addItem(self, &self->mDeletePersonaItem, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", LobbyMenu_LobbyMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cCommandMenuItem_cCommandMenuItem(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* LobbyMenu_vtbl[];
extern char str_Select_Room[];

/**
 * Constructs the debug "Select Room" menu: a title, a spacer and twelve blank room lines.
 *
 * Runs cMenu_cMenu(), stores vtable LobbyMenu_vtbl (onCommand LobbyMenu_onCommand, which joins the chosen room) and the owner
 * from @p info at +0x130, builds the "Select Room" title and a spacer of 14, constructs twelve empty command items
 * (cCommandMenuItem_cCommandMenuItem()) at +0x168 (0x24 bytes apart) and adds all fourteen. LobbyMenu_updaterooms fills in the room names.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* LobbyMenu_LobbyMenu(LobbyMenu* self, int* info)
{
    char* roomItem = (char*)&self->mRoomItems[0];
    int i = 11;
    cMenu_cMenu(self);
    self->mVtbl = LobbyMenu_vtbl;
    self->mScreen = (cDebugLobbyScreen*)(*info);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Select_Room);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    for (; i != -1; i--) {
        cCommandMenuItem_cCommandMenuItem(roomItem);
        roomItem += 0x24;
    }
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    int j;
    for (j = 0; j < 12; j++) {
        cMenu_addItem(self, &self->mRoomItems[j], -1);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", RoomMenu_RoomMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cCommandMenuItem_cCommandMenuItem(void* self);
extern "C" void* cCommandMenuItem_cCommandMenuItem2(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* RoomMenu_vtbl[];
extern char str_Room_Occupants[];
extern char str_Chat[];

/**
 * Constructs the debug "Room Occupants" menu: a title, a spacer, 24 blank occupant lines and a "Chat" item.
 *
 * Runs cMenu_cMenu(), stores vtable RoomMenu_vtbl (onCommand RoomMenu_onCommand) and the owners from @p occupantOwner (+0x130, used for an
 * occupant) and @p chatOwner (+0x134, used by "Chat"), builds the "Room Occupants" title and a spacer of 14, 24 empty command
 * items (cCommandMenuItem_cCommandMenuItem()) at +0x16C (0x24 bytes apart) and a "Chat" action item with id -2 (cCommandMenuItem_cCommandMenuItem2(), +0x4CC),
 * and adds them in that order. RoomMenu_updateusers fills in the occupants.
 *
 * @param occupantOwner  holder whose first word is stored at +0x130 (owner for an occupant command)
 * @param chatOwner  holder whose first word is stored at +0x134 (owner for the "Chat" command)
 * @return @p self
 */
extern "C" void* RoomMenu_RoomMenu(RoomMenu* self, int* occupantOwner, int* chatOwner)
{
    char* occupantItem = (char*)&self->mUserItems[0];
    int i = 23;
    cMenu_cMenu(self);
    self->mVtbl = RoomMenu_vtbl;
    self->mSelectUserScreen = (cDebugLobbyScreen*)(*occupantOwner);
    self->mChatScreen = (cDebugLobbyScreen*)(*chatOwner);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Room_Occupants);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    for (; i != -1; i--) {
        cCommandMenuItem_cCommandMenuItem(occupantItem);
        occupantItem += 0x24;
    }
    cCommandMenuItem_cCommandMenuItem2(&self->mChatItem, -2, str_Chat, 0);
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    int j;
    for (j = 0; j < 0x18; j++) {
        cMenu_addItem(self, &self->mUserItems[j], -1);
    }
    cMenu_addItem(self, &self->mChatItem, -1);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", ChatMenu_ChatMenu);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cCommandMenuItem_cCommandMenuItem(void* self);
extern "C" void* cTextMenuItem_cTextMenuItem(void* self, void* a1, const char* name);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* ChatMenu_vtbl[];
extern char str_Chat_Menu[];
extern char str_Enter_Chat[];
extern char str_empty_d[];
extern int D_00467D70[];

/**
 * Constructs the debug "Chat Menu": a title, a spacer, an "Enter Chat" entry and fourteen canned chat lines.
 *
 * Runs cMenu_cMenu(), stores vtable ChatMenu_vtbl (onCommand ChatMenu_onCommand, which sends the chosen line as a lobby chat
 * message) and the owner from @p info at +0x130, builds the "Chat Menu" title, a spacer of 14 and a text entry labelled
 * "Enter Chat" (cTextMenuItem_cTextMenuItem(), +0x168), then fourteen command items (cCommandMenuItem_cCommandMenuItem()) at +0x194, each with id j
 * (+0x8) and the label D_00467D70[j] (canned lines such as "Assassins!"). Adds the title, the spacer and the fourteen
 * lines; likely bug: the "Enter Chat" entry is built but never added to the menu.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* ChatMenu_ChatMenu(ChatMenu* self, int* info)
{
    char* chatItem = (char*)&self->mMessageItems[0];
    int i = 13;
    cMenu_cMenu(self);
    self->mVtbl = ChatMenu_vtbl;
    self->mScreen = (cDebugLobbyScreen*)(*info);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)(&self->mTitleItem), str_Chat_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)(&self->mSpaceItem), (void*)0xE);
    cTextMenuItem_cTextMenuItem(&self->mEnterChatItem, str_Enter_Chat, str_empty_d);
    for (; i != -1; i--) {
        cCommandMenuItem_cCommandMenuItem(chatItem);
        chatItem += 0x24;
    }
    cMenu_addItem(self, &self->mTitleItem, -1);
    cMenu_addItem(self, &self->mSpaceItem, -1);
    int j;
    for (j = 0; j < 0xE; j++) {
        cCommandMenuItem* item = &self->mMessageItems[j];
        item->ID = j;
        item->Caption = (const char*)D_00467D70[j];
        cMenu_addItem(self, item, -1);
    }
    return ((void*)self);
}
#endif

