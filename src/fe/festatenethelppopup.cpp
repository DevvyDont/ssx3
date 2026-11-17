#include "common.h"
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
extern "C" void cFEStateNetHelpPopup_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_help_popup), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_handleEvent);
#ifdef SKIP_ASM
struct sVEntry001D9E20 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

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
extern "C" int cFEStateNetHelpPopup_handleEvent(void* self, int msg, int msgParam)
{
    if (*(int*)((char*)self + 0x50) != 0) {
        void* obj = *(void**)((char*)self + 0x20);
        if (obj != 0) {
            sVEntry001D9E20* vt = *(sVEntry001D9E20**)((char*)obj + 8);
            return vt[24].fn((char*)obj + vt[24].delta, msg, msgParam);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_onUpdate);
#ifdef SKIP_ASM
extern "C" float cInput_getValue(void* self, int id);
extern "C" void func_0039F190(void*, int);

/**
 * Closes the help popup once neither controller holds input 0x99, probably the help button that opened it.
 */
extern "C" void cFEStateNetHelpPopup_onUpdate(void* self)
{
    int i;
    int found = 0;
    for (i = 0; i < 2; i++) {
        int** input = *(int***)((char*)SSXApp + (i << 2) + 0xB0);
        int ok = *input != 0 && **input != 0;
        if (ok && cInput_getValue(input, 153) != 0.0f) {
            found = 1;
        }
    }
    if (found == 0) {
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
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

struct sWidget_1D9F38 {
    char pad00[0x14];
    unsigned int flags;
    char pad18[0x20];
    int hash;
};

struct sNetHelp_1D9F38 {
    char pad00[0xA8];
    sWidget_1D9F38* wA8;
    sWidget_1D9F38* wAC;
    sWidget_1D9F38* wB0;
    sWidget_1D9F38* rows[7];
    sWidget_1D9F38* wD0;
    sWidget_1D9F38* wD4;
    sWidget_1D9F38* wD8;
    sWidget_1D9F38* wDC;
    sWidget_1D9F38* wE0;
    sWidget_1D9F38* wE4;
    sWidget_1D9F38* wE8;
    sWidget_1D9F38* wEC;
    sWidget_1D9F38* wF0;
};

static inline int IsHash_1D9F38(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Binds the help popup's widgets by name: title, body and bottom note texts, the button icons and the seven
 * option rows.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateNetHelpPopup_onWidgetCreate(sNetHelp_1D9F38* self, sWidget_1D9F38* w)
{
    int hashName = w->hash;
    if (hashName == GetHashValue32(str_OnlineAccSetup)) {
        self->wA8 = w;
        w->flags |= 0x80;
    } else if (hashName == GetHashValue32(str_TopText)) {
        self->wAC = w;
        w->flags |= 0x80;
    } else if (hashName == GetHashValue32(str_BottomNote)) {
        self->wB0 = w;
        w->flags |= 0x80;
    } else if (IsHash_1D9F38(w->hash, str_xselect)) {
        self->wD0 = w;
    } else if (IsHash_1D9F38(w->hash, str_ps2triangle)) {
        self->wD4 = w;
    } else if (IsHash_1D9F38(w->hash, str_ps2circle)) {
        self->wD8 = w;
    } else if (IsHash_1D9F38(w->hash, str_updownarrows)) {
        self->wDC = w;
    } else if (IsHash_1D9F38(w->hash, str_leftrightarrows)) {
        self->wE0 = w;
    } else if (IsHash_1D9F38(w->hash, str_ps2l1)) {
        self->wE8 = w;
    } else if (IsHash_1D9F38(w->hash, str_ps2r1)) {
        self->wE4 = w;
    } else if (IsHash_1D9F38(w->hash, str_actrl)) {
        self->wF0 = w;
    } else if (IsHash_1D9F38(w->hash, str_ps2sel)) {
        self->wEC = w;
    } else {
        char buf[16];
        for (int i = 0; i < 7; i++) {
            sprintf(buf, str_TextOptionpct_d, i);
            if (hashName == GetHashValue32(buf)) {
                self->rows[i] = w;
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

struct sVE_A110 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sStrings_A110 {
    int pad0;
    sVE_A110* vt;
};

/**
 * Builds the help popup's description: concatenates its (up to three) localised description strings and shows them in
 * the popup's text.
 *
 * Looks up each non-zero description id at +0x58 (stopping at the first 0) through SSXApp's FELocale (slot 4),
 * allocates one buffer of the total length (tag "cFEStateNetHelpPopup"), appends the strings with func_002C2688(),
 * hands it to cUIText_setUnicodeStringWrap() on the text at +0xAC and frees it again. With no description the text gets
 * a null string. cFEStateNetHelpPopup_onGainTransition() calls it.
 *
 * @param helpPopup  the help popup state
 */
extern "C" void cFEStateNetHelpPopup_createAndSetDescriptionString(void* helpPopup)
{
    char* self = (char*)helpPopup;
    unsigned short* strs[3];
    unsigned short* buf = 0;
    int total = 0;
    int count = 0;
    int* ids = (int*)(self + 0x58);
    int i = 0;
    do {
        if (ids[i] == 0) {
            break;
        }
        sStrings_A110* locale = (sStrings_A110*)SSXApp->FELocale;
        strs[i] = locale->vt[4].fn((char*)locale + locale->vt[4].delta, ids[i]);
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
    cUIText_setUnicodeStringWrap(*(void**)(self + 0xAC), buf);
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
extern "C" void cFEStateNetHelpPopup_createAndSetDescriptionString(void* self);
extern char D_004A1408[];

struct sVEnt_1DA238 { short delta; short index; void (*fn)(void*, int); };
struct sVec3_1DA238 { float x, y, z; };

struct sWidget_1DA238 {
    char pad00[8];
    sVEnt_1DA238* vt;
    char pad0C[0x38];
    sVec3_1DA238 pos;
};

struct sNetHelp_1DA238 {
    char pad00[0x48];
    int state;                  // 0x48
    char pad4C[8];
    int title;                  // 0x54
    int c[3];                   // 0x58
    int f64;                    // 0x64
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
    w->vt[9].fn((char*)w + w->vt[9].delta, on);
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
            cUIText_setAsciiString((cUIText*)self->wA8, D_004A1408);
        }
        if (self->c[0]) {
            cFEStateNetHelpPopup_createAndSetDescriptionString(self);
        } else {
            cUIText_setAsciiString((cUIText*)self->wAC, D_004A1408);
        }
        if (self->f64) {
            cUIText_setUnicodeStringByID((cUIText*)self->wB0, self->f64);
        } else {
            cUIText_setAsciiString((cUIText*)self->wB0, D_004A1408);
        }
        for (int j = 0; j < 9; j++) {
            show_1DA238(self->icons[j], 0);
        }
        for (int i = 0; i < 7; i++) {
            if (i < self->state) {
                show_1DA238(self->rows[i], 1);
                show_1DA238(self->icons[self->b[i]], 1);
                sVec3_1DA238 objectPos = self->icons[self->b[i]]->pos;
                objectPos.y += (float)i * 22.0f;
                self->icons[self->b[i]]->pos = objectPos;
                if (self->a[i]) {
                    cUIText_setUnicodeStringByID((cUIText*)self->rows[i], self->a[i]);
                } else {
                    cUIText_setAsciiString((cUIText*)self->rows[i], D_004A1408);
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
extern "C" void func_0039F190(void*, int);

/**
 * Closes the help popup on widget event 6.
 *
 * @param pWidget  the widget, unused
 * @param msg  widget event number
 */
extern "C" void cFEStateNetHelpPopup_onWidgetEvent(void* self, int pWidget, int msg)
{
    if (msg == 6) {
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_clear);
#ifdef SKIP_ASM
struct sNetHelpPopup_DA4A8 {
    char pad_0x0[0x48];
    int state;      // 0x48
    char pad_0x4C[0x8];
    int f54;        // 0x54
    int c[3];       // 0x58
    int f64;        // 0x64
    int a[7];       // 0x68
    int b[7];       // 0x84
};

/**
 * Clears the help popup's contents: no options, no descriptions, every option's text 0 and icon -1.
 *
 * Sets the option count (+0x48, see cFEStateNetHelpPopup_setNumHelpOptions()), +0x54, the three description ids (+0x58)
 * and +0x64 to 0, and the seven option text ids (+0x68) to 0 and icons (+0x84) to -1 (see
 * cFEStateNetHelpPopup_setOption()). The help popup's constructor func_001D9D48() calls it.
 */
extern "C" void cFEStateNetHelpPopup_clear(void* self)
{
    sNetHelpPopup_DA4A8* helpPopup = (sNetHelpPopup_DA4A8*)self;
    int i;
    helpPopup->state = 0;
    helpPopup->f54 = 0;
    for (i = 2; i >= 0; i--) {
        helpPopup->c[i] = 0;
    }
    helpPopup->f64 = 0;
    for (i = 0; i < 7; i++) {
        helpPopup->a[i] = 0;
        helpPopup->b[i] = -1;
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
void cFEStateNetHelpPopup_setNumHelpOptions(void* self, int numOptions)
{
    *(int*)((char*)self + 0x48) = numOptions;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", cFEStateNetHelpPopup_setDescription__FPvii);
#ifdef SKIP_ASM
void cFEStateNetHelpPopup_setDescription(void* self, int descriptionNum, int description)
{
    *(int*)((char*)self + (descriptionNum << 2) + 0x58) = description;
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
 * @return 1 when the first range sorts before the second, else 0
 */
extern "C" int func_001DA580(const char* first1, const char* last1, const char* first2, const char* last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return 1;
        if (*first2 < *first1) return 0;
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
INCLUDE_ASM("fe/festatenethelppopup", func_001DA648);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void* func_002CCDF0(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern "C" char* func_00416958(char* dst, const char* src, int n);
extern void* D_0046D690[];
extern char str_Create_Account[];
extern char str_Login_Name[];
extern char str_Password[];
extern char str_Cancel[];
extern char D_004A1408[];

static inline void copyName_1DA648(char* dst, const char* src)
{
    dst[0] = 0;
    func_00416958(dst, src, 0xF);
}

/**
 * Constructs the debug "Create Account" menu: a title, a spacer, "Login Name" and "Password" entries pre-filled from
 * SSXApp, an action item and "Cancel".
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D690 (onCommand func_001DBC38, which creates the account) and the owner from
 * @p vinfo at +0x130, and builds the items in place: the "Create Account" title (cNullMenuItem), a spacer of 10, two
 * text entries (func_002CE418()) labelled "Login Name" (+0x168) and "Password" (+0x194), an action item labelled @p actionLabel
 * (func_002CCF98(), +0x1C0) and a "Cancel" item (func_002CCDF0(), +0x1E4), adding all six in that order. The entries'
 * text buffers (+0x180, +0x1AC) get up to 15 characters of SSXApp's UserName and Password. func_001DA7E8 is the same
 * menu with the Login vtable; func_001DA988 embeds both.
 *
 * @param vinfo  a holder whose first word (the debug front end) is stored at +0x130
 * @param actionLabel  the action item's label
 * @return @p self
 */
extern "C" void* func_001DA648(void* self, void* vinfo, void* actionLabel)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D690;
    *(int*)((char*)self + 0x130) = *(int*)vinfo;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Create_Account);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)10);
    func_002CE418((char*)self + 0x168, str_Login_Name, D_004A1408);
    func_002CE418((char*)self + 0x194, str_Password, D_004A1408);
    func_002CCF98((char*)self + 0x1C0, 0, actionLabel, 0);
    func_002CCDF0((char*)self + 0x1E4, str_Cancel);
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    cMenu_addItem(self, (char*)self + 0x168, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C0, -1);
    cMenu_addItem(self, (char*)self + 0x1E4, -1);
    copyName_1DA648((char*)self + 0x180, SSXApp->UserName);
    copyName_1DA648((char*)self + 0x1AC, SSXApp->Password);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DA7E8);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void* func_002CCDF0(void* self, void* text);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern "C" char* func_00416958(char* dst, const char* src, int n);
extern void* D_0046D6E8[];
extern char str_Create_Account[];
extern char str_Login_Name[];
extern char str_Password[];
extern char str_Cancel[];
extern char D_004A1408[];

static inline void copyName_1DA7E8(char* dst, const char* src)
{
    dst[0] = 0;
    func_00416958(dst, src, 0xF);
}

/**
 * Constructs the debug "Login" menu: the same items as func_001DA648 ("Create Account" title, "Login Name", "Password",
 * an action item, "Cancel") with the Login vtable.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D6E8 (onCommand func_001DBC68, which logs in with the typed name and
 * password) and the owner from @p vinfo at +0x130, builds and adds the six items, and pre-fills the name and password
 * entries (+0x180, +0x1AC) with up to 15 characters of SSXApp's UserName and Password. The title is "Create Account"
 * here too (likely a copy-paste slip; func_001DA988 labels the sub-menu "Login").
 *
 * @param vinfo  a holder whose first word (the debug front end) is stored at +0x130
 * @param actionLabel  the action item's label
 * @return @p self
 */
extern "C" void* func_001DA7E8(void* self, void* vinfo, void* actionLabel)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D6E8;
    *(int*)((char*)self + 0x130) = *(int*)vinfo;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Create_Account);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)10);
    func_002CE418((char*)self + 0x168, str_Login_Name, D_004A1408);
    func_002CE418((char*)self + 0x194, str_Password, D_004A1408);
    func_002CCF98((char*)self + 0x1C0, 0, actionLabel, 0);
    func_002CCDF0((char*)self + 0x1E4, str_Cancel);
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    cMenu_addItem(self, (char*)self + 0x168, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    cMenu_addItem(self, (char*)self + 0x1C0, -1);
    cMenu_addItem(self, (char*)self + 0x1E4, -1);
    copyName_1DA7E8((char*)self + 0x180, SSXApp->UserName);
    copyName_1DA7E8((char*)self + 0x1AC, SSXApp->Password);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festatenethelppopup", func_001DA988);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
extern "C" void* func_001DA648(void* self, void* a1, void* a2);
extern "C" void* func_001DA7E8(void* self, void* a1, void* a2);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D638[];
extern char str_Create_Account[];
extern char str_Login[];
extern char str_Login_Method[];

/**
 * Constructs the debug "Login Method" menu with "Create Account" and "Login" sub-menus.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D638, builds the Create Account menu (func_001DA648()) at +0x130 and the
 * Login menu (func_001DA7E8()) at +0x32C, labelling their action items "Create Account" and "Login", then adds the
 * "Login Method" title, a spacer of 10 and two cSubMenuItems opening those menus. cDebugLobbyScreen_cDebugLobbyScreen() builds it at +0x70 of
 * the debug front end.
 *
 * @param createOwner  owner holder passed to the Create Account menu
 * @param loginOwner  owner holder passed to the Login menu
 * @return @p self
 */
extern "C" void* func_001DA988(void* self, void* createOwner, void* loginOwner)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D638;
    func_001DA648((char*)self + 0x130, createOwner, str_Create_Account);
    func_001DA7E8((char*)self + 0x32C, loginOwner, str_Login);
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x528), str_Login_Method);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x544), (void*)10);
    cSubMenuItem_cSubMenuItem((char*)self + 0x55C, (char*)self + 0x130, str_Create_Account);
    cSubMenuItem_cSubMenuItem((char*)self + 0x578, (char*)self + 0x32C, str_Login);
    cMenu_addItem(self, (char*)self + 0x528, -1);
    cMenu_addItem(self, (char*)self + 0x544, -1);
    cMenu_addItem(self, (char*)self + 0x55C, -1);
    cMenu_addItem(self, (char*)self + 0x578, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAAC8);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D588[];
extern char str_Create_Persona[];
extern char str_Persona_Name[];
extern char D_004A1408[];
extern char str_Create[];

/**
 * Constructs the debug "Create Persona" menu: a title, a spacer, a "Persona Name" entry and a "Create" item.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D588 (onCommand func_001DBBE8, which creates the persona) and the owner from
 * @p info at +0x130, builds the "Create Persona" title, a spacer of 14, a text entry labelled "Persona Name"
 * (func_002CE418(), +0x168) and a "Create" action item (func_002CCF98(), +0x194), and adds them in that order.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* func_001DAAC8(void* self, int* info)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D588;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Create_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    func_002CE418((char*)self + 0x168, str_Persona_Name, D_004A1408);
    func_002CCF98((char*)self + 0x194, 0, str_Create, 0);
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    cMenu_addItem(self, (char*)self + 0x168, -1);
    cMenu_addItem(self, (char*)self + 0x194, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DABD0);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D5E0[];
extern char str_Delete_Persona[];

/**
 * Constructs the debug "Delete Persona" menu: a title, a spacer and four blank persona lines.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D5E0 (onCommand func_001DBC10, which deletes the chosen persona) and the
 * owner from @p info at +0x130, builds the "Delete Persona" title and a spacer of 14, constructs four empty command
 * items (func_002CCF08()) at +0x168 (0x24 bytes apart) and adds all six. func_001DB330 later fills in the persona
 * lines.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* func_001DABD0(void* self, int* info)
{
    char* personaItem = (char*)self + 0x168;
    int i = 3;
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D5E0;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Delete_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(personaItem);
        personaItem += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 4; j++) {
        cMenu_addItem(self, (char*)self + 0x168 + j * 36, -1);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DACB8);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* cSubMenuItem_cSubMenuItem(void* self, void* a1, void* a2);
// PORT: the member ctors return self, but this caller treats them as void
void func_001DAAC8_v(void* self, int* info) __asm__("func_001DAAC8");
void func_001DABD0_v(void* self, int* info) __asm__("func_001DABD0");
extern "C" void* func_002CCF08(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D530[];
extern char str_Select_Persona[];
extern char str_Create_Persona[];
extern char str_Delete_Persona[];


/**
 * Constructs the debug "Select Persona" menu with four persona lines and "Create Persona" / "Delete Persona" sub-menus.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D530 (onCommand func_001DBBC0, which selects the chosen persona), builds the
 * Create Persona menu (func_001DAAC8()) at +0x130 and the Delete Persona menu (func_001DABD0()) at +0x2E8, stores the
 * owner from @p info at +0x4E0, then builds the "Select Persona" title, a spacer of 14, four empty command items
 * (func_002CCF08()) at +0x518 and cSubMenuItems for the two sub-menus, adding them in that order. func_001DB418 fills
 * in the persona lines; cDebugLobbyScreen_cDebugLobbyScreen() builds it at +0x74C of the debug front end.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x4E0
 * @param createOwner  owner holder passed to the Create Persona menu
 * @param deleteOwner  owner holder passed to the Delete Persona menu
 * @return @p self
 */
extern "C" void* func_001DACB8(void* self, int* info, int* createOwner, int* deleteOwner)
{
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D530;
    func_001DAAC8_v((char*)self + 0x130, createOwner);
    func_001DABD0_v((char*)self + 0x2E8, deleteOwner);
    *(int*)((char*)self + 0x4E0) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x4E4), str_Select_Persona);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x500), (void*)0xE);
    char* personaItem = (char*)self + 0x518;
    for (int i = 3; i != -1; i--) {
        func_002CCF08(personaItem);
        personaItem += 0x24;
    }
    cSubMenuItem_cSubMenuItem((char*)self + 0x5A8, (char*)self + 0x130, str_Create_Persona);
    cSubMenuItem_cSubMenuItem((char*)self + 0x5C4, (char*)self + 0x2E8, str_Delete_Persona);
    cMenu_addItem(self, (char*)self + 0x4E4, -1);
    cMenu_addItem(self, (char*)self + 0x500, -1);
    int j;
    for (j = 0; j < 4; j++) {
        cMenu_addItem(self, (char*)self + 0x518 + j * 36, -1);
    }
    cMenu_addItem(self, (char*)self + 0x5A8, -1);
    cMenu_addItem(self, (char*)self + 0x5C4, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAE20);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D4D8[];
extern char str_Select_Room[];

/**
 * Constructs the debug "Select Room" menu: a title, a spacer and twelve blank room lines.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D4D8 (onCommand func_001DBB90, which joins the chosen room) and the owner
 * from @p info at +0x130, builds the "Select Room" title and a spacer of 14, constructs twelve empty command items
 * (func_002CCF08()) at +0x168 (0x24 bytes apart) and adds all fourteen. func_001DB528 fills in the room names.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* func_001DAE20(void* self, int* info)
{
    char* roomItem = (char*)self + 0x168;
    int i = 11;
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D4D8;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Select_Room);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(roomItem);
        roomItem += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 12; j++) {
        cMenu_addItem(self, (char*)self + 0x168 + j * 36, -1);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DAF08);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void* func_002CCF98(void* self, int a1, void* a2, int a3);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D480[];
extern char str_Room_Occupants[];
extern char str_Chat[];

/**
 * Constructs the debug "Room Occupants" menu: a title, a spacer, 24 blank occupant lines and a "Chat" item.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D480 (onCommand func_001DBB48) and the owners from @p occupantOwner (+0x130, used for an
 * occupant) and @p chatOwner (+0x134, used by "Chat"), builds the "Room Occupants" title and a spacer of 14, 24 empty command
 * items (func_002CCF08()) at +0x16C (0x24 bytes apart) and a "Chat" action item with id -2 (func_002CCF98(), +0x4CC),
 * and adds them in that order. func_001DB620 fills in the occupants.
 *
 * @param occupantOwner  holder whose first word is stored at +0x130 (owner for an occupant command)
 * @param chatOwner  holder whose first word is stored at +0x134 (owner for the "Chat" command)
 * @return @p self
 */
extern "C" void* func_001DAF08(void* self, int* occupantOwner, int* chatOwner)
{
    char* occupantItem = (char*)self + 0x16C;
    int i = 23;
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D480;
    *(int*)((char*)self + 0x130) = *occupantOwner;
    *(int*)((char*)self + 0x134) = *chatOwner;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x138), str_Room_Occupants);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x154), (void*)0xE);
    for (; i != -1; i--) {
        func_002CCF08(occupantItem);
        occupantItem += 0x24;
    }
    func_002CCF98((char*)self + 0x4CC, -2, str_Chat, 0);
    cMenu_addItem(self, (char*)self + 0x138, -1);
    cMenu_addItem(self, (char*)self + 0x154, -1);
    int j;
    for (j = 0; j < 0x18; j++) {
        cMenu_addItem(self, (char*)self + 0x16C + j * 36, -1);
    }
    cMenu_addItem(self, (char*)self + 0x4CC, -1);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatenethelppopup", func_001DB028);
#ifdef SKIP_ASM
struct cNullMenuItem;
struct cSpaceMenuItem;
void* cMenu_cMenu(void* self);
cNullMenuItem* cNullMenuItem_cNullMenuItem(cNullMenuItem* self, void* text);
cSpaceMenuItem* cSpaceMenuItem_cSpaceMenuItem(cSpaceMenuItem* self, void* text);
extern "C" void* func_002CCF08(void* self);
extern "C" void* func_002CE418(void* self, void* a1, const char* name);
extern "C" void cMenu_addItem(void* menu, void* item, int index);
extern void* D_0046D428[];
extern char str_Chat_Menu[];
extern char str_Enter_Chat[];
extern char D_004A1408[];
extern int D_00467D70[];

/**
 * Constructs the debug "Chat Menu": a title, a spacer, an "Enter Chat" entry and fourteen canned chat lines.
 *
 * Runs cMenu_cMenu(), stores vtable D_0046D428 (onCommand func_001DBB20, which sends the chosen line as a lobby chat
 * message) and the owner from @p info at +0x130, builds the "Chat Menu" title, a spacer of 14 and a text entry labelled
 * "Enter Chat" (func_002CE418(), +0x168), then fourteen command items (func_002CCF08()) at +0x194, each with id j
 * (+0x8) and the label D_00467D70[j] (canned lines such as "Assassins!"). Adds the title, the spacer and the fourteen
 * lines; likely bug: the "Enter Chat" entry is built but never added to the menu.
 *
 * @param info  a holder whose first word (the debug front end) is stored at +0x130
 * @return @p self
 */
extern "C" void* func_001DB028(void* self, int* info)
{
    char* chatItem = (char*)self + 0x194;
    int i = 13;
    cMenu_cMenu(self);
    *(void***)((char*)self + 0x12C) = D_0046D428;
    *(int*)((char*)self + 0x130) = *info;
    cNullMenuItem_cNullMenuItem((cNullMenuItem*)((char*)self + 0x134), str_Chat_Menu);
    cSpaceMenuItem_cSpaceMenuItem((cSpaceMenuItem*)((char*)self + 0x150), (void*)0xE);
    func_002CE418((char*)self + 0x168, str_Enter_Chat, D_004A1408);
    for (; i != -1; i--) {
        func_002CCF08(chatItem);
        chatItem += 0x24;
    }
    cMenu_addItem(self, (char*)self + 0x134, -1);
    cMenu_addItem(self, (char*)self + 0x150, -1);
    int j;
    for (j = 0; j < 0xE; j++) {
        char* item = (char*)self + 0x194 + j * 36;
        *(int*)(item + 0x8) = j;
        *(int*)(item + 0x14) = D_00467D70[j];
        cMenu_addItem(self, item, -1);
    }
    return self;
}
#endif

