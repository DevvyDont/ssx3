#include "common.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadHintState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern signed char D_004A2C38;
extern char str_101QPMPHints[];
extern char str_kT_FEHINTTitlepct_d[];
extern char str_kT_FEHINTDESpct_d[];
extern char str_hint_title[];
extern char str_hint_body[];
extern char str_Bitmap0000_b[];

/**
 * Creates the loading-hint screen ("101QPMPHints") and fills it with the next hint and the loading image.
 *
 * Hints cycle through kT_FEHINTTitle%d / kT_FEHINTDES%d numbers 1-15, skipping 12 (counter D_004A2C38). The
 * image comes from the load info at +0x48 (its word at +0x10) and goes on the "Bitmap0000" widget.
 */
extern "C" void cFELoadHintState_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_101QPMPHints), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    int n = D_004A2C38 + 1;
    if (n == 12) {
        D_004A2C38++;
        n = D_004A2C38 + 1;
    }
    char title[0x40];
    char body[0x40];
    sprintf(title, str_kT_FEHINTTitlepct_d, n);
    sprintf(body, str_kT_FEHINTDESpct_d, n);
    D_004A2C38 = (D_004A2C38 + 1) % 15;
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hint_title));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(title));
    }
    text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hint_body));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(body));
    }
    char* icon = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Bitmap0000_b));
    if (icon != 0) {
        char* loadInfo = *(char**)((char*)self + 0x48);
        if (loadInfo != 0) {
            int textureId = *(int*)(loadInfo + 0x10);
            if (textureId != 0) {
                *(int*)(icon + 0x78) = textureId;
                *(int*)(icon + 0x7C) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadHintState_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A2BC8[];
extern char str_pct_3dpct_pct[];
extern void* cAppMan_TheAppMan;
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf2(char* buf, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, char* text);

/**
 * Shows the loading percentage on the hint screen's "%" text widget.
 *
 * The value is the float at cAppMan_TheAppMan + 0x38, printed as "%3d%%"; it is probably the load progress.
 *
 * @return always 0
 */
extern "C" int cFELoadHintState_onGainTransition(void* self)
{
    char buf[200];
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2BC8));
    if (obj != 0) {
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)((char*)cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadHintState_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the load-hint state's onWidgetCreate has an empty body.
 */
void cFELoadHintState_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState__cFELoadState);
#ifdef SKIP_ASM
extern void* cFELoadState_vtbl[];
extern "C" void* func_0039E390(void* self);

/**
 * Destroys the front-end load state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFELoadState__cFELoadState(void* self)
{
    *(void**)((char*)self + 0x8) = cFELoadState_vtbl;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFELoadState_InitwidgetMultiP(void* self, int idx);
extern signed char D_00535C11[];
extern char str_103MPStat[];
extern char str_100QPStat[];

/**
 * Creates the loading screen with rider stats: "103MPStat" for both players online, else "100QPStat".
 *
 * The online check is the new-race game path (D_00535C11 == 2); the two-player screen is filled with
 * InitwidgetMultiP() for players 0 and 1.
 */
extern "C" void cFELoadState_onCreateScreen(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        void* engine = *(void**)((char*)self + 0x10);
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_103MPStat), 0);
        *(void**)((char*)self + 0x40) = screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
            cFELoadState_InitwidgetMultiP(self, 0);
            cFELoadState_InitwidgetMultiP(self, 1);
        }
    } else {
        void* engine = *(void**)((char*)self + 0x10);
        void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_100QPStat), 0);
        *(void**)((char*)self + 0x40) = screen;
        if (screen != 0) {
            cUIScreen_playFrame(screen, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A2BC8[];
extern char str_pct_3dpct_pct[];
extern void* cAppMan_TheAppMan;
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf2(char* buf, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, char* text);

/**
 * Shows the loading percentage on the stats loading screen's "%" text widget.
 *
 * Same as cFELoadHintState_onGainTransition(): the float at cAppMan_TheAppMan + 0x38 printed as "%3d%%".
 *
 * @return always 0
 */
extern "C" int cFELoadState_onGainTransition(void* self)
{
    char buf[200];
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2BC8));
    if (obj != 0) {
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)((char*)cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cFELoadState_widgetCreateQP(void* self, void* widget);
extern signed char D_00535C11[];

/**
 * Creates the load screen's quick-play widget, except on the online game path.
 *
 * Fetches back-end interface 0 (the result is unused), then calls cFELoadState_widgetCreateQP() unless the
 * race's game path (cGamePath, D_00535C11) is 2, online.
 *
 * @param widget  the widget being created
 */
extern "C" void cFELoadState_onWidgetCreate(void* self, void* widget)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] != 2) {
        cFELoadState_widgetCreateQP(self, widget);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge__cFELoadStateInLodge);
#ifdef SKIP_ASM
extern void* cFELoadStateInLodge_vtbl[];
extern "C" void* func_0039E390(void* self);

/**
 * Destroys the in-lodge load state: stores its vtable at +0x8, then runs the cUIState destructor.
 *
 * The cUIState destructor (func_0039E390) gets the g++ delete flags from the second argument register,
 * untouched.
 *
 * @return no meaningful value (func_0039E390 returns nothing)
 */
extern "C" void* cFELoadStateInLodge__cFELoadStateInLodge(void* self)
{
    *(void**)((char*)self + 0x8) = cFELoadStateInLodge_vtbl;
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_117loadinlodge[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the in-lodge loading screen ("117loadinlodge") and plays its first frame.
 */
extern "C" void cFELoadStateInLodge_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_117loadinlodge), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the in-lodge loading state's onWidgetCreate has an empty body.
 */
void cFELoadStateInLodge_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadStateInLodge_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A2BC8[];
extern char str_pct_3dpct_pct[];
extern void* cAppMan_TheAppMan;
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf2(char* buf, const char* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, char* text);

/**
 * Shows the loading percentage on the in-lodge loading screen's "%" text widget.
 *
 * Same as cFELoadHintState_onGainTransition(): the float at cAppMan_TheAppMan + 0x38 printed as "%3d%%".
 *
 * @return always 0
 */
extern "C" int cFELoadStateInLodge_onGainTransition(void* self)
{
    char buf[200];
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2BC8));
    if (obj != 0) {
        sprintf2(buf, str_pct_3dpct_pct, (int)*(float*)((char*)cAppMan_TheAppMan + 0x38));
        cUIText_setUnicodeString(obj, buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245E30);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];

extern "C" int func_00245E30(int* a, int* b)
{
    unsigned int va = D_00536640[*a];
    unsigned int vb = D_00536640[*b];
    if (vb < va) {
        return -1;
    }
    return va != vb;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245E78);
#ifdef SKIP_ASM
extern unsigned int D_00536640[];

extern "C" int func_00245E78(int* a, int* b)
{
    unsigned int va = D_00536640[*a];
    unsigned int vb = D_00536640[*b];
    if (va < vb) {
        return -1;
    }
    return va != vb;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245EC0);
#ifdef SKIP_ASM
extern int D_00536708[];

extern "C" int func_00245EC0(int* a, int* b)
{
    int va = D_00536708[*a];
    int vb = D_00536708[*b];
    if (vb < va) {
        return 1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245F00);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char D_0047DB28[];

extern "C" void func_00245F00(void* self, int flags)
{
    *(void**)self = D_0047DB28;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_0039E390(void* self);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245F30__FPv);
#ifdef SKIP_ASM
void* func_00245F30(void* self)
{
    return func_0039E390(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00245F50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_supopup[];
extern char str_Title_b[];
extern char str_yes_c[];
extern char str_no_c[];
extern char str_The_display_mode_has_been_switched_to_Pr[];
extern char str_The_display_mode_has_been_set_to_interla[];
extern char str_kT_CMNContinue_c[];

struct sVEK245F50 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00245F50(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_supopup), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_b));
    if (text != 0) {
        if (*(signed char*)((char*)self + 0x48) == 0)
            cUIText_setAsciiString(text, str_The_display_mode_has_been_switched_to_Pr);
        else
            cUIText_setAsciiString(text, str_The_display_mode_has_been_set_to_interla);
    }
    cUIText* text2 = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_yes_c));
    if (text2 != 0) {
        cUIText_setUnicodeStringByID(text2, GetHashValue32(str_kT_CMNContinue_c));
    }
    char* noButton = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_no_c));
    if (noButton != 0) {
        sVEK245F50* vt = *(sVEK245F50**)(noButton + 8);
        vt[9].fn(noButton + vt[9].delta, 0);
        sVEK245F50* vt2 = *(sVEK245F50**)(noButton + 8);
        vt2[8].fn(noButton + vt2[8].delta, 1);
    }
    *(int*)((char*)self + 0x4C) = 300;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00246098);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* func_0039E510(void* self);
extern char str_Title_b[];
extern char str_yes_c[];
extern char str_no_c[];
extern char str_The_display_mode_has_been_switched_to_Pr[];
extern char str_The_display_mode_has_been_set_to_interla[];
extern char str_kT_CMNContinue_c[];

struct sVEK246098 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00246098(void* self)
{
    if (*(int*)((char*)self + 0x4C) != 0) {
        if (--*(int*)((char*)self + 0x4C) == 0) {
            if (*(signed char*)((char*)self + 0x48) == 0) {
                *(signed char*)((char*)self + 0x48) = 1;
                cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Title_b));
                if (text != 0) {
                    if (*(signed char*)((char*)self + 0x48) == 0)
                        cUIText_setAsciiString(text, str_The_display_mode_has_been_switched_to_Pr);
                    else
                        cUIText_setAsciiString(text, str_The_display_mode_has_been_set_to_interla);
                }
                cUIText* text2 = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_yes_c));
                if (text2 != 0) {
                    cUIText_setUnicodeStringByID(text2, GetHashValue32(str_kT_CMNContinue_c));
                }
                char* noButton = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_no_c));
                if (noButton != 0) {
                    sVEK246098* vt = *(sVEK246098**)(noButton + 8);
                    vt[9].fn(noButton + vt[9].delta, 0);
                    sVEK246098* vt2 = *(sVEK246098**)(noButton + 8);
                    vt2[8].fn(noButton + vt2[8].delta, 1);
                }
                cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
                sVEK246098* vt3 = (sVEK246098*)ps2GraphicsMan->mVtbl;
                vt3[8].fn((char*)ps2GraphicsMan + vt3[8].delta, 0);
            }
        }
    }
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_002461E0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00186A08(void* self, void* engine);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_yes_c[];
extern char str_cFEStateMemCard[];

extern "C" void func_002461E0(void* self, void* item, int msg)
{
    if (item != 0) {
        if (msg == 5) {
            int hash = *(int*)((char*)item + 0x38);
            if (hash == GetHashValue32(str_yes_c)) {
                void* state = func_00186A08(cMemMan_alloc(0x248, str_cFEStateMemCard, 0, 0), *(void**)((char*)self + 0x10));
                cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, state);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00246268);
#ifdef SKIP_ASM
extern "C" int func_00246268(void* self, int menu, int event)
{
    return event == 6 ? 0x101 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00246278);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int func_00144C98(void* iface);
extern "C" int func_00144BF0(void* iface);
extern signed char D_00535C10[];
extern char str_pct_s_pct_S_b[];
extern char str_Location_Text[];
extern char str_kT_Peak1name_b[];
extern char str_kT_Peak2name_b[];
extern char str_kT_Peak3name_c[];
extern char str_Title_Multi_Race[];
extern char str_Title_Multi_SS[];
extern char str_Title_Multi_BA[];
extern char str_Title_Multi_SP[];
extern char str_Title_Multi_BC[];

struct sVEntry_246278 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_246278(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_246278* vt = *(sVEntry_246278**)(obj + 4);
    return vt[4].fn(obj + vt[4].delta, GetHashValue32(key));
}

static inline void Fmt_246278(char* buf, char* fmt, char* key, void* iface)
{
    int str = Loc_246278(key);
    sprintf2(buf, fmt, str, func_00144BF0(iface));
}

struct sVEntryV_246278 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void Activate_246278(char* obj)
{
    sVEntryV_246278* vt = *(sVEntryV_246278**)(obj + 8);
    vt[9].fn(obj + vt[9].delta, 1);
}

static inline bool IsId_246278(char* obj, char* key)
{
    int id = *(int*)(obj + 0x38);
    return id == GetHashValue32(key);
}

extern "C" void func_00246278(void* self, char* obj)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (IsId_246278(obj, str_Location_Text)) {
        char buf[200];
        func_00416210(buf, 0, 200);
        char* fmt = str_pct_s_pct_S_b;
        switch (func_00144C98(iface)) {
        case 0:
            Fmt_246278(buf, fmt, str_kT_Peak1name_b, iface);
            break;
        case 1:
            Fmt_246278(buf, fmt, str_kT_Peak2name_b, iface);
            break;
        case 2:
            Fmt_246278(buf, fmt, str_kT_Peak3name_c, iface);
            break;
        }
        cUIText_setUnicodeString(obj, buf);
        return;
    }
    switch (D_00535C10[0]) {
    case 0:
        if (IsId_246278(obj, str_Title_Multi_Race)) Activate_246278(obj);
        break;
    case 1:
        if (IsId_246278(obj, str_Title_Multi_SS)) Activate_246278(obj);
        break;
    case 2:
        if (IsId_246278(obj, str_Title_Multi_BA)) Activate_246278(obj);
        break;
    case 3:
        if (IsId_246278(obj, str_Title_Multi_SP)) Activate_246278(obj);
        break;
    case 4:
        if (IsId_246278(obj, str_Title_Multi_Race)) Activate_246278(obj);
        break;
    case 5:
        if (IsId_246278(obj, str_Title_Multi_BC)) Activate_246278(obj);
        break;
    case 6:
        if (IsId_246278(obj, str_Title_Multi_BC)) Activate_246278(obj);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_002464E8);
#ifdef SKIP_ASM
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
extern "C" int func_00144D18(void* iface, int idx);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" const char* cBECharacterInterface_getShortName(void* iface, int a1);
extern "C" int func_00148950(void* iface, int id);
extern "C" void* strcpy1(void* dst, void* src);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Location[];
extern char str_Title_Quick_Race[];
extern char str_Title_Quick_SS[];
extern char str_Title_Quick_BA[];
extern char str_Title_Quick_SP[];
extern char str_Title_Quick_BC[];
extern char str_et_race[];
extern char str_et_SS[];
extern char str_et_BA[];
extern char str_et_SP[];
extern char str_et_TChal[];
extern char str_et_PChal[];
extern char str_ec_beg[];
extern char str_ec_inter[];
extern char str_ec_expert[];
extern char str_vert_drop_percent[];
extern char str_kT_OVRCMNNumberOfFeet[];
extern char str_cou_leng_percent[];
extern char str_rider_name_c[];
extern char str_player_rank_num[];
extern char str_kT_OVRCMNLevelNum[];
extern char str_map_box[];

// 17 per-event values (two rodata tables copied to the stack)
struct sTable17_2464E8 {
    int v[17];
};
extern sTable17_2464E8 D_0047BAE0;
extern sTable17_2464E8 D_0047BB28;

struct sOptTriple_2464E8 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    int b;
    int c;
};
extern sOptTriple_2464E8 D_005308B8[];

struct sVEnt_2464E8 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};

struct sVEntV_2464E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void* Loc_2464E8(char* key)
{
    char* o = (char*)SSXApp->FELocale;
    sVEnt_2464E8* vt = *(sVEnt_2464E8**)(o + 4);
    return vt[4].fn(o + vt[4].delta, GetHashValue32(key));
}

static inline bool IsId_2464E8(char* obj, char* key)
{
    int id = *(int*)(obj + 0x38);
    return id == GetHashValue32(key);
}

static inline void Activate_2464E8(char* obj)
{
    sVEntV_2464E8* vt = *(sVEntV_2464E8**)(obj + 8);
    vt[9].fn(obj + vt[9].delta, 1);
}

static inline void Fmt_2464E8(char* buf, char* fmt, char* key, void* iface)
{
    void* str = Loc_2464E8(key);
    sprintf2(buf, fmt, str, func_00144BF0(iface));
}

extern "C" void func_002464E8(void* self, char* obj)
{
    sTable17_2464E8 tA = D_0047BAE0;
    sTable17_2464E8 tB = D_0047BB28;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    char buf[200];
    unsigned short fmtA[100];
    unsigned short fmtB[100];
    if (IsId_2464E8(obj, str_Location)) {
        func_00416210(buf, 0, 200);
        char* fmt = str_pct_s_pct_S_b;
        switch (func_00144C98(iface)) {
        case 0:
            Fmt_2464E8(buf, fmt, str_kT_Peak1name_b, iface);
            break;
        case 1:
            Fmt_2464E8(buf, fmt, str_kT_Peak2name_b, iface);
            break;
        case 2:
            Fmt_2464E8(buf, fmt, str_kT_Peak3name_c, iface);
            break;
        }
        cUIText_setUnicodeString(obj, buf);
        return;
    }
    switch (cBENewRaceInterface_newRace.cGameMode) {
    case 0:
        if (IsId_2464E8(obj, str_Title_Quick_Race)) { Activate_2464E8(obj); return; }
        break;
    case 1:
        if (IsId_2464E8(obj, str_Title_Quick_SS)) { Activate_2464E8(obj); return; }
        break;
    case 2:
        if (IsId_2464E8(obj, str_Title_Quick_BA)) { Activate_2464E8(obj); return; }
        break;
    case 3:
        if (IsId_2464E8(obj, str_Title_Quick_SP)) { Activate_2464E8(obj); return; }
        break;
    case 4:
        if (IsId_2464E8(obj, str_Title_Quick_Race)) { Activate_2464E8(obj); return; }
        break;
    case 5:
        if (IsId_2464E8(obj, str_Title_Quick_BC)) { Activate_2464E8(obj); return; }
        break;
    case 6:
        if (IsId_2464E8(obj, str_Title_Quick_BC)) { Activate_2464E8(obj); return; }
        break;
    }
    switch (cBENewRaceInterface_newRace.cGameMode) {
    case 0:
        if (IsId_2464E8(obj, str_et_race)) { Activate_2464E8(obj); return; }
        break;
    case 1:
        if (IsId_2464E8(obj, str_et_SS)) { Activate_2464E8(obj); return; }
        break;
    case 2:
        if (IsId_2464E8(obj, str_et_BA)) { Activate_2464E8(obj); return; }
        break;
    case 3:
        if (IsId_2464E8(obj, str_et_SP)) { Activate_2464E8(obj); return; }
        break;
    case 5:
        if (IsId_2464E8(obj, str_et_TChal)) { Activate_2464E8(obj); return; }
        break;
    case 6:
        if (IsId_2464E8(obj, str_et_PChal)) { Activate_2464E8(obj); return; }
        break;
    case 7:
        break;
    }
    switch (func_00144D18(iface, cBENewRaceInterface_getLocationID(iface))) {
    case 0:
        if (IsId_2464E8(obj, str_ec_beg)) { Activate_2464E8(obj); return; }
        break;
    case 1:
        if (IsId_2464E8(obj, str_ec_inter)) { Activate_2464E8(obj); return; }
        break;
    case 2:
        if (IsId_2464E8(obj, str_ec_expert)) { Activate_2464E8(obj); return; }
        break;
    }
    if (IsId_2464E8(obj, str_vert_drop_percent)) {
        strcpy1(fmtA, Loc_2464E8(str_kT_OVRCMNNumberOfFeet));
        sprintf1((unsigned short*)buf, fmtA, tB.v[cBENewRaceInterface_getLocationID(iface)]);
        cUIText_setUnicodeString(obj, buf);
    } else if (IsId_2464E8(obj, str_cou_leng_percent)) {
        strcpy1(fmtB, Loc_2464E8(str_kT_OVRCMNNumberOfFeet));
        sprintf1((unsigned short*)buf, fmtB, tA.v[cBENewRaceInterface_getLocationID(iface)]);
        cUIText_setUnicodeString(obj, buf);
    } else if (IsId_2464E8(obj, str_rider_name_c)) {
        void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
        cUIText_setAsciiString((cUIText*)obj, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
    } else if (IsId_2464E8(obj, str_player_rank_num)) {
        void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* iface3 = cBE_getInterface_Fv(cBE_getBE(), 3);
        strcpy1(fmtA, Loc_2464E8(str_kT_OVRCMNLevelNum));
        int v = func_00148950(iface3, cBENewPlayerInterface_getHumanRider(iface1, 0));
        int idx = cBENewPlayerInterface_getHumanRider(iface1, 0);
        sprintf1((unsigned short*)buf, fmtA, D_005308B8[idx].b1 ? 1 : v);
        cUIText_setUnicodeString(obj, buf);
    } else if (IsId_2464E8(obj, str_map_box)) {
        char* p = *(char**)((char*)self + 0x48);
        if (p != 0) {
            int v = *(int*)(p + 0x10);
            if (v != 0) {
                *(int*)(obj + 0x78) = v;
                *(int*)(obj + 0x7C) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", func_00246CF8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry_6CF8<void> is a raw slot). Virtual calls in this unit go through these by hand.
template <class F> struct sVEntry_6CF8 {
    short delta;
    short index;
    F* fn;
};

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern "C" void cUIText_setUnicodeString(void* obj, char* text);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern signed char D_00535C11[];

void cUIText_setAsciiString(cUIText* text, const char* str);


extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

extern "C" int func_00148950(void* iface, int id);

extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);

extern sOptTriple_2464E8 D_005308B8[];

extern "C" signed char func_00148410(void* self, int rider);
extern "C" signed char func_001484E0(void* self, int rider);
extern "C" signed char func_001485B0(void* self, int rider);
extern "C" signed char func_00148680(void* self, int rider);
extern "C" signed char func_00148750(void* self, int rider);
extern "C" signed char func_00148820(void* self, int rider);
extern "C" signed char func_001488F0(void* self, int rider);
extern "C" void cUIProgress_setValue(void* self, int pos);
extern char str_100[];
extern char D_004A2C18[];
extern char str_00pb[];
extern char D_004A2C28_6CF8[] __asm__("str_pct_d_g");
extern char str_ranking00[];
extern char str_ranking01[];
extern char str_kT_CMNLvNum[];
// The network game state (dirtysock/tagsunk sNetState); the unit declares cBENetworkInterface_networkOpt as int[] below.
struct sNetState_6CF8 {
    int active;                     // 0x000
    int f4;                         // 0x004
    char pad_0x008[0x4A8 - 0x8];
    unsigned char stats[7];         // 0x4A8 (D_00534FD8)
};
extern unsigned char D_00534FD8[];
// Inline copy of the BE stat-block getter (func_0015C600).
static inline unsigned char* GetStats_6CF8() { return D_00534FD8; }
struct sLoadInfo_6CF8 {
    char pad0[0x10];
    int image[2];                   // 0x10: per-player image
};
// PORT: the widget name templates are splat-named literals; copied from the symbols as char-array structs.
struct sName4_6CF8 { char c[4]; };
struct sName2_6CF8 { char c[2]; };
struct sName5_6CF8 { char c[5]; };

extern "C" void func_00246CF8(void* self, char player)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 2) {
        sName4_6CF8 statName = *(sName4_6CF8*)str_100;
        sName2_6CF8 picName = *(sName2_6CF8*)D_004A2C18;
        sName5_6CF8 barName = *(sName5_6CF8*)str_00pb;
        int stats[12];                  // 7 used; the frame has room for 12 (buf sits at sp+0x60)
        char buf[0x20];
        unsigned short text[100];
        picName.c[0] = player + '0';
        statName.c[0] = player + '1';
        void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
        int idx = cBENewPlayerInterface_getHumanRider(iface1, player != 0);
        void* iface3 = cBE_getInterface_Fv(cBE_getBE(), 3);
        int rank = func_00148950(iface3, cBENewPlayerInterface_getHumanRider(iface1, player != 0));
        if (D_005308B8[cBENewPlayerInterface_getHumanRider(iface1, player != 0)].b1) {
            rank = 1;
        }
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cCommSystem_mCommSystem != 0 && (player == 1 || cBENetworkInterface_networkOpt.bIsRankedGame != 0)) {
            if (cBENetworkInterface_networkOpt.bIsRankedGame != 0) {
                rank = 8;
                for (int i = 0; i < 7; i++) {
                    stats[i] = 40;
                }
            } else if (D_005308B8[idx].b1) {
                for (int i = 0; i < 7; i++) {
                    stats[i] = 5;
                }
            } else {
                int sum = 0;
                for (int i = 0; i < 7; i++) {
                    sum += cBENetworkInterface_networkOpt.oppCharacterAttributes[i];
                }
                rank = sum / 35;
                unsigned char* s = GetStats_6CF8();
                for (int i = 0; i < 7; i++) {
                    stats[i] = s[i];
                }
            }
        } else {
            if (D_005308B8[idx].b1) {
                for (int i = 0; i < 7; i++) {
                    stats[i] = 5;
                }
            } else {
                stats[0] = func_001484E0(iface3, idx);
                stats[1] = func_00148680(iface3, idx);
                stats[2] = func_00148410(iface3, idx);
                stats[3] = func_00148750(iface3, idx);
                stats[4] = func_001488F0(iface3, idx);
                stats[5] = func_00148820(iface3, idx);
                stats[6] = func_001485B0(iface3, idx);
            }
        }
        // fmt stays in $18 across both GetHashValue32/getObjectByHashName calls: the target computes
        // the full address before the player test (a literal is rematerialised at the sprintf).
        char* fmt = D_004A2C28_6CF8;
        void* obj;
        if (player == 0) {
            obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ranking00));
        } else {
            obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_ranking01));
        }
        sprintf(buf, fmt, rank);
        if (obj != 0) {
            cUIText_setAsciiString((cUIText*)obj, buf);
        }
        for (int i = 0; i < 7; i++) {
            statName.c[2] = i + '0';
            char* label = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(statName.c));
            barName.c[0] = player + '0';
            barName.c[1] = i + '0';
            char* bar = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(barName.c));
            if (label != 0 && bar != 0) {
                int v = stats[i];
                int level = v / 5;
                char* o = (char*)SSXApp->FELocale;
                sVEntry_6CF8<void*(void*, int)>* vt = *(sVEntry_6CF8<void*(void*, int)>**)(o + 4);
                sprintf1(text, (unsigned short*)vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_CMNLvNum)), level);
                sVEntry_6CF8<void(void*, int)>* vt2 = *(sVEntry_6CF8<void(void*, int)>**)(label + 8);
                vt2[9].fn(label + vt2[9].delta, 1);
                cUIText_setUnicodeString(label, (char*)text);
                *(int*)(bar + 0x80) = 0x37;
                cUIProgress_setValue(bar, v);
            }
        }
        char* pic = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(picName.c));
        sLoadInfo_6CF8* p = *(sLoadInfo_6CF8**)((char*)self + 0x48);
        if (p != 0 && pic != 0) {
            int q = p->image[player];
            if (q != 0) {
                *(int*)(pic + 0x78) = q;
                *(int*)(pic + 0x7C) = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_InitwidgetMultiP);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

// Declarations shared by the functions of this unit.
struct cUIText;

int GetHashValue32(char* str);

extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" int sprintf(char* buf, const char* fmt, ...);

extern "C" void cUIText_setUnicodeString(void* obj, char* text);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern unsigned int D_00536640[];

void cUIText_setAsciiString(cUIText* text, const char* str);


extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);

extern "C" int func_00148950(void* iface, int id);

extern "C" void* strcpy1(void* dst, void* src);

extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);

extern char str_kT_OVRCMNLevelNum[];

extern signed char D_00535C10[];

// PORT: the widget name templates are splat-named literals; copied from the symbols as char-array structs.
struct sName19_72C0 { char c[19]; };
struct sName12_72C0 { char c[12]; };
struct sName10_72C0 { char c[10]; };
struct sName8_72C0 { char c[8]; };
struct sName7_72C0 { char c[7]; };
struct sName2_72C0 { char c[2]; };
extern char str_1_Player_rank_numb[];
extern char str_1_rank[];
extern char str_1_time[];
extern char str_1knockdowns[];
extern char str_1wipeouts[];
extern char str_1tricks[];
extern char str_1handplants[];
extern char str_1ubertricks[];
extern char str_1ubergrinds[];
extern char D_004A2C18[];
// PORT: the literal "%d" (strcpy of a constant expands to an aligned 3-byte copy).
extern char str_pct_d_g[] __attribute__((aligned(8)));
extern char str_Time[];
extern char str_Score[];
extern int D_00536730[];
extern "C" void* cGameModeMan_getGM();
extern "C" char* func_00155270(void* iface, int a1);
extern "C" unsigned int func_004139F8(float f);
extern "C" char* func_0020A1A8(int v);
extern "C" char* strcpy(char* dst, const char* src);

struct sLoadInfo_72C0 {
    char pad0[0x10];
    int image[2];                   // 0x10: per-player image
};

static inline void Activate_72C0(char* obj)
{
    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(obj + 8);
    vt[9].fn(obj + vt[9].delta, 1);
}

// The player index is a char parameter (the caller in onCreateScreen declares it int); bound by asm label.
extern "C" void cFELoadState_InitwidgetMultiP_72C0(void* self, char player) __asm__("cFELoadState_InitwidgetMultiP");

/**
 * Fills one player's column of the two-player loading screen with that rider's stats and picture.
 *
 * Widget names start with the side number ("1 rank", "2 time", ...). Fills rank number, level
 * (kT_OVRCMNLevelNum;
 * fixed at 8 in a ranked online game), time (race modes 0 and 5) or score, knockdowns, wipeouts, tricks,
 * handplants, ubertricks and ubergrinds. In an online game the client swaps the player index, so each side
 * shows
 * the same rider on both machines.
 *
 * @param player  screen side, 0 or 1 (On Tour iHuman)
 */
extern "C" void cFELoadState_InitwidgetMultiP_72C0(void* self, char player)
{
    sName19_72C0 rankNumName = *(sName19_72C0*)str_1_Player_rank_numb;
    sName7_72C0 rankName = *(sName7_72C0*)str_1_rank;
    sName7_72C0 timeName = *(sName7_72C0*)str_1_time;
    sName12_72C0 knockdownsName = *(sName12_72C0*)str_1knockdowns;
    sName10_72C0 wipeoutsName = *(sName10_72C0*)str_1wipeouts;
    sName8_72C0 tricksName = *(sName8_72C0*)str_1tricks;
    sName12_72C0 handplantsName = *(sName12_72C0*)str_1handplants;
    sName12_72C0 ubertricksName = *(sName12_72C0*)str_1ubertricks;
    sName12_72C0 ubergrindsName = *(sName12_72C0*)str_1ubergrinds;
    char buf[0x20];
    char fmt[0x20];
    char side = player;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsHost == 0 && cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
        player = !player;
    }
    if (side == 1) {
        rankNumName.c[0] = '2';
        rankName.c[0] = '2';
        timeName.c[0] = '2';
        knockdownsName.c[0] = '2';
        wipeoutsName.c[0] = '2';
        tricksName.c[0] = '2';
        handplantsName.c[0] = '2';
        ubertricksName.c[0] = '2';
        ubergrindsName.c[0] = '2';
    }
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface3 = cBE_getInterface_Fv(cBE_getBE(), 3);
    cGameModeMan_getGM();
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    char* st = func_00155270(iface8, cBENewPlayerInterface_getHumanRider(iface1, player != 0));
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(rankNumName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, D_00536730[player] + 1);
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(rankName.c));
    if (obj != 0) {
        cBE_getInterface_Fv(SSXApp->BE, 7);
        int rank;
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cBENetworkInterface_networkOpt.bIsRankedGame != 0) {
            rank = 8;
        } else {
            rank = func_00148950(iface3, cBENewPlayerInterface_getHumanRider(iface1, player != 0));
        }
        unsigned short fmtw[50];
        unsigned short text[100];
        char* o = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = *(sVEntry<void*(void*, int)>**)(o + 4);
        strcpy1(fmtw, vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_OVRCMNLevelNum)));
        sprintf1(text, fmtw, rank);
        cUIText_setUnicodeString(obj, (char*)text);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(timeName.c));
    if (obj != 0) {
        if (D_00535C10[0] == 0 || D_00535C10[0] == 5) {
            int i = cBENewPlayerInterface_getHumanRider(iface1, player != 0);
            unsigned int seconds = func_004139F8((float)D_00536640[i] * (1.0f / 60.0f));
            char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Time));
            if (o != 0) {
                Activate_72C0(o);
            }
            strcpy(buf, func_0020A1A8(seconds));
        } else {
            int i = cBENewPlayerInterface_getHumanRider(iface1, player != 0);
            unsigned int score = D_00536640[i];
            char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Score));
            if (o != 0) {
                Activate_72C0(o);
            }
            sprintf(buf, str_pct_d_g, score);
        }
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(knockdownsName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0x2C));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(wipeoutsName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0x28));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(tricksName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0x14));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(handplantsName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0xC));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(ubertricksName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0x18));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(ubergrindsName.c));
    if (obj != 0) {
        __builtin_memcpy(fmt, str_pct_d_g, sizeof("%d"));
        sprintf(buf, fmt, *(int*)(st + 0x20));
        cUIText_setAsciiString((cUIText*)obj, buf);
    }
    sName2_72C0 picName = *(sName2_72C0*)D_004A2C18;
    picName.c[0] = side + '0';
    char* pic = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(picName.c));
    sLoadInfo_72C0* loadInfo = *(sLoadInfo_72C0**)((char*)self + 0x48);
    if (loadInfo != 0 && pic != 0) {
        int textureId = loadInfo->image[player];
        if (textureId != 0) {
            *(int*)(pic + 0x78) = textureId;
            *(int*)(pic + 0x7C) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateloadhint", cFELoadState_widgetCreateQP);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" char* func_00155270(void* iface, int a1);
extern "C" const char* cBECharacterInterface_getShortName(void* iface, int a1);
extern "C" int func_00148950(void* iface, int id);
extern "C" void func_002C2540(void* dst, const char* s);
extern "C" void* strcpy1(void* dst, void* src);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* obj, char* text);
extern "C" double fptodp(float f);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_d_g[];
extern char str_kT_OVRCMNLevelNum[];
extern char str_results_rider[];
extern char str_results_ranking[];
extern char str_results_knockdown[];
extern char str_results_wipeouts[];
extern char str_results_ubertricks[];
extern char str_results_ubergrinds[];
extern char str_results_handplants[];
extern char str_results_grabtime[];
extern char str_results_best_combo[];
// PORT: the literal "%02.0f:%02.0f" (strcpy of a constant expands to an aligned 14-byte copy);
// splat names it, so copy from the symbol with the literal's length and rodata alignment.
extern char D_0047BF78[] __attribute__((aligned(8)));

static inline int wHash_7AB0(char* w) { return *(int*)(w + 0x38); }

struct sVEnt_7AB0 { short delta; short index; void* (*fn)(void*, int); };

/**
 * Fills one widget of the single-player loading screen with the human rider's stats, chosen by widget name.
 *
 * Handles results_rider (short name), results_ranking (level, kT_OVRCMNLevelNum), results_knockdown,
 * results_wipeouts, results_ubertricks, results_ubergrinds, results_handplants, results_grabtime (mm:ss) and
 * results_best combo; other widgets are left alone.
 *
 * @param widget  the widget being created (On Tour cUIThing* pWidget)
 */
extern "C" void cFELoadState_widgetCreateQP(void* self, void* widget)
{
    char* w = (char*)widget;
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    void* iface3 = cBE_getInterface_Fv(cBE_getBE(), 3);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    char* st = func_00155270(iface8, cBENewPlayerInterface_getHumanRider(iface1, 0));
    unsigned short text[0x18];
    unsigned short fmtw[0x20];
    if (wHash_7AB0(w) == GetHashValue32(str_results_rider)) {
        func_002C2540(text, cBECharacterInterface_getShortName(iface2, cBENewPlayerInterface_getHumanRider(iface1, 0)));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_ranking)) {
        char* locale = (char*)SSXApp->FELocale;
        sVEnt_7AB0* vt = *(sVEnt_7AB0**)(locale + 4);
        strcpy1(fmtw, vt[4].fn(locale + vt[4].delta, GetHashValue32(str_kT_OVRCMNLevelNum)));
        sprintf1(text, fmtw, func_00148950(iface3, cBENewPlayerInterface_getHumanRider(iface1, 0)));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_knockdown)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0x2C));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_wipeouts)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0x28));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_ubertricks)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0x18));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_ubergrinds)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0x20));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_handplants)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0xC));
        cUIText_setUnicodeString(widget, (char*)text);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_grabtime)) {
        int totalSeconds = (int)*(float*)(st + 0x48);
        int min = totalSeconds / 60;
        int sec = totalSeconds % 60;
        char fmt[0x20];
        char buf[0x20];
        __builtin_memcpy(fmt, D_0047BF78, sizeof("%02.0f:%02.0f"));
        sprintf(buf, fmt, fptodp((float)min), fptodp((float)sec));
        cUIText_setAsciiString((cUIText*)widget, buf);
    } else if (wHash_7AB0(w) == GetHashValue32(str_results_best_combo)) {
        func_002C2540(fmtw, str_pct_d_g);
        sprintf1(text, fmtw, *(int*)(st + 0x88));
        cUIText_setUnicodeString(widget, (char*)text);
    }
}
#endif

extern "C" void* func_00242EB8(int, int);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00247E20__FPv);
#ifdef SKIP_ASM
void* func_00247E20(void* self)
{
    return func_00242EB8(1, 0xffff);
}
#endif

extern "C" void* func_00242EB8(int, int);

//100%
INCLUDE_ASM("fe/festateloadhint", func_00247E40__FPv);
#ifdef SKIP_ASM
void* func_00247E40(void* self)
{
    return func_00242EB8(0, 0xffff);
}
#endif

