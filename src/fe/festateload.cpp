#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_Map[];
extern char str_femap_template[];
extern unsigned int cUITemplate_MAP_mMapStyle;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUITemplate_MAP_onCreateScreen(void* tmpl, void* screen, void* owner);

/**
 * Creates the event-select screens "Map" and "femap_template" and sets up the map template.
 *
 * Only "Map" is kept in the state's screen pointer. Clears cUITemplate_MAP_mMapStyle, then runs
 * cUITemplate_MAP_onCreateScreen() on the template at +0x48.
 */
extern "C" void cFEStateEventSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_Map), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = *(void**)((char*)self + 0x10);
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_femap_template), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* main = *(void**)((char*)self + 0x40);
    cUITemplate_MAP_mMapStyle = 0;
    cUITemplate_MAP_onCreateScreen((char*)self + 0x48, main, self);
}
#endif

extern "C" void* cUITemplate_MAP_onWidgetCreate(void*);

//100%
INCLUDE_ASM("fe/festateload", func_00186708__FPv);
#ifdef SKIP_ASM
/**
 * Forwards widget creation to the embedded map template at +0x48 (cUITemplate_MAP_onWidgetCreate()).
 *
 * Sits in the onWidgetCreate slot of cFEStateEventSelect_vtbl; the widget passes through untouched in the second
 * argument register, and cUITemplate_MAP_onWidgetCreate() selects the peak in the "SP_PeakList" menu.
 *
 * @return cUITemplate_MAP_onWidgetCreate()'s return value (it returns nothing meaningful)
 */
void* func_00186708(void* self)
{
    return cUITemplate_MAP_onWidgetCreate((char*)self + 0x48);
}
#endif

extern "C" void* cUITemplate_MAP_onGainTransition(void*);

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Forwards the gain-transition event to the event select state's map template (+0x48).
 *
 * @return cUITemplate_MAP_onGainTransition()'s result
 */
void* cFEStateEventSelect_onGainTransition(void* self)
{
    return cUITemplate_MAP_onGainTransition((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onGainFocus);
#ifdef SKIP_ASM
extern "C" void* cUIState_onGainFocus(void* self);
extern "C" void cUITemplate_MAP_onGainFocus(void* tmpl);

/**
 * Gives focus to the event select state: runs cUIState_onGainFocus(), then the map template's (+0x48) handler.
 */
extern "C" void cFEStateEventSelect_onGainFocus(void* self)
{
    cUIState_onGainFocus(self);
    cUITemplate_MAP_onGainFocus((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onDestroyScreen);
#ifdef SKIP_ASM
void* cUIState_onDestroyScreen(void* self);
extern "C" void cUITemplate_MAP_onDestroyScreen(void* tmpl);

/**
 * Destroys the event select state's screens, then tears down its map template (+0x48).
 *
 * After cUIState_onDestroyScreen(), cUITemplate_MAP_onDestroyScreen() unloads the template's async file, removes the PDA template
 * in map states 1, 3 and 4, and calls func_0028F678() on the audio object with 0.
 */
extern "C" void cFEStateEventSelect_onDestroyScreen(void* self)
{
    cUIState_onDestroyScreen(self);
    cUITemplate_MAP_onDestroyScreen((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_001867A8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* self, int hash);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" int cUITemplate_MAP_onWidgetEvent(void* tmpl, void* obj, int msg);
extern "C" void func_0039F190(void* list, int a1);
extern "C" void cUIStateStack_push(void* list, void* item);
extern int D_004A1A70;

static inline int IsQuick1867A8() { return D_004A1A70 == 1; }
extern char str_TransitionOut[];

class cMgr1867A8 {
public:
    int f0;
    // vptr at 0x4
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* v04(void* owner, int id);
    virtual void* v05(void* owner, int id);
};

class cFEVObj_1867A8 {
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
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual int v24(int a, int b);
};

static inline void PlayIntro1867A8(void* self)
{
    int f = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(str_TransitionOut));
    if (f != 0xFFFF) {
        cUIScreen_playFrame(*(void**)((char*)self + 0x40), f, 1);
    }
}

static inline void Opened1867A8(void* self, void* item)
{
    if (item != 0) {
        cFEVObj_1867A8* c = *(cFEVObj_1867A8**)(**(char***)((char*)self + 0x10) + 0x10);
        if (c != 0) {
            c->v24(1, 0);
        }
        PlayIntro1867A8(self);
        cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, item);
    }
}

/**
 * Handles widget events for the event-select map screen, moving to the next or previous state on accept or back.
 *
 * Every event first goes to the embedded map template's handler, cUITemplate_MAP_onWidgetEvent() on +0x48. For event 5
 * (accept) or 6 (back) that the template answers with 1: online (D_004A1A70 == 1) it plays the screen's "TransitionOut"
 * frame and pops this state (func_0039F190() with count 1 on the engine's state stack, mpEngine +0x18); otherwise it
 * asks the engine's FE custom object for a state (cFECustom_getNextState(), slot 4, for 5; func_001A16C0(), slot 5, for
 * 6) with the widget's id (+0x18) and, when one comes back, calls handleEvent (slot 24) with (1, 0) on the state at the
 * custom object's +0x10, plays "TransitionOut" and pushes the new state.
 *
 * @param obj  the widget
 * @param msg  the widget event (5 accept, 6 back)
 */
extern "C" void func_001867A8(void* self, void* obj, int msg)
{
    if (msg == 5) {
        if (cUITemplate_MAP_onWidgetEvent((char*)self + 0x48, obj, 5) == 1) {
            if (IsQuick1867A8()) {
                PlayIntro1867A8(self);
                func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
            } else {
                cMgr1867A8* feCustom = **(cMgr1867A8***)((char*)self + 0x10);
                Opened1867A8(self, feCustom->v04(self, *(int*)((char*)obj + 0x18)));
            }
        }
    } else if (msg == 6) {
        if (cUITemplate_MAP_onWidgetEvent((char*)self + 0x48, obj, 6) == 1) {
            if (IsQuick1867A8()) {
                PlayIntro1867A8(self);
                func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
            } else {
                cMgr1867A8* feCustom = **(cMgr1867A8***)((char*)self + 0x10);
                Opened1867A8(self, feCustom->v05(self, *(int*)((char*)obj + 0x18)));
            }
        }
    } else {
        cUITemplate_MAP_onWidgetEvent((char*)self + 0x48, obj, msg);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_00186950);
#ifdef SKIP_ASM
extern "C" int cUITemplate_MAP_onWidgetMenu(void* self, int a1, int a2);

/**
 * Answers a menu event for the event-select screen with 257 when the map template accepts it, else 0.
 *
 * cUITemplate_MAP_onWidgetMenu() on the template at +0x48 rejects events 6, 8 and 9.
 *
 * @param menu  the menu
 * @param event  the menu event
 * @return 257 (0x101) when cUITemplate_MAP_onWidgetMenu() returns non-zero, else 0
 */
extern "C" int func_00186950(void* self, int menu, int event)
{
    if (cUITemplate_MAP_onWidgetMenu((char*)self + 0x48, menu, event) != 0) {
        return 257;
    }
    return 0;
}
#endif

void cUITemplate_MAP_onInputBegin(void*);

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onInputBegin);
#ifdef SKIP_ASM
/**
 * Passes the start of input handling to the map template (+0x48) through cUITemplate_MAP_onInputBegin(), which returns 0.
 */
extern "C" void cFEStateEventSelect_onInputBegin(void* self)
{
    cUITemplate_MAP_onInputBegin((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onInputAlways);
#ifdef SKIP_ASM
extern "C" void cUITemplate_MAP_onInputAlways(void* self, int a1);
// PORT: cUIState_onInputAlways__FPv is called with (self, a1) here; bind the 2-arg form to that symbol.
void cUIState_onInputAlways_2(void* self, int a1) __asm__("cUIState_onInputAlways__FPv");

/**
 * Passes the always-on input to the map template (+0x48), then to cUIState's handler.
 *
 * @param pInput  the input object
 */
extern "C" void cFEStateEventSelect_onInputAlways(void* self, int pInput)
{
    cUITemplate_MAP_onInputAlways((char*)self + 0x48, pInput);
    cUIState_onInputAlways_2(self, pInput);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cUIState_onUpdate(void* self);
extern "C" void cUITemplate_MAP_onUpdate(void* tmpl);

/**
 * Per-frame update of the event select screen: the base UI state update, then the map template's.
 *
 * Calls cUIState_onUpdate() and then cUITemplate_MAP_onUpdate() on the template embedded at +0x48.
 */
extern "C" void cFEStateEventSelect_onUpdate(void* self)
{
    cUIState_onUpdate(self);
    cUITemplate_MAP_onUpdate((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_00186A08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard_cFEMemCard(void* self, void* a1, int a2);
extern "C" void* cSSXApp_getMCManager(void* p);
extern void* cFEStateMemCard_vtbl[];
// D_004A14B8 holds the active load state (the unit declares it as int elsewhere).
extern void* D_004A14B8_K186A08 __asm__("D_004A14B8");
extern int D_004A14B4;
extern int D_004A14BC;
extern "C" void cFEStateMemCard_callbackLoadReady();
extern "C" void cFEStateMemCard_callbackFileNotFound();
extern "C" void cFEStateMemCard_callbackFileChecked();
extern "C" void cFEStateMemCard_callbackSysRecheckFile();
extern "C" void cFEStateMemCard_callbackCardsChecked();
extern "C" void func_00186C98();
extern "C" void func_00186D60();
extern "C" void cFEStateMemCard_callbackAcknowledgeCardStatusChange();
extern "C" void cFEStateMemCard_callbackConfirmNoFreeBlocks();

struct sFEStateLoadK186A08 {
    char pad_0x0[0x8];
    void** vtbl;        // 0x8
    int fC;             // 0xC
    char pad_0x10[0x19C - 0x10];
    int f19C;           // 0x19C
    char pad_0x1A0[0x204 - 0x1A0];
    int f204;           // 0x204
    char pad_0x208[0x22C - 0x208];
    int f22C;           // 0x22C
    int f230;           // 0x230
    int f234;           // 0x234
    int f238;           // 0x238
    int f23C;           // 0x23C
    int f240;           // 0x240
    int f244;           // 0x244
};

struct sMemCardK186A08 {
    char pad_0x0[0x38];
    void* cb38;         // 0x38
    void* cb3C;         // 0x3C
    char pad_0x40[0x8];
    void* cb48;         // 0x48
    char pad_0x4C[0x10];
    void* cb5C;         // 0x5C
    char pad_0x60[0x30];
    void* cb90;         // 0x90
    void* cb94;         // 0x94
    char pad_0x98[0xC];
    void* cbA4;         // 0xA4
    char pad_0xA8[0x4];
    void* cbAC;         // 0xAC
    void* cbB0;         // 0xB0
    char pad_0xB4[0x11C - 0xB4];
    int f11C;           // 0x11C
    int f120;           // 0x120
    int f124;           // 0x124
    int f128;           // 0x128
    int f12C;           // 0x12C
};

/**
 * Constructs the memory-card load state and points the memory-card manager's callbacks at the load handlers.
 *
 * Runs cFEMemCard_cFEMemCard() with type 2, stores cFEStateMemCard_vtbl, makes itself the active load state
 * (D_004A14B8) and clears D_004A14B4 and D_004A14BC. Sets mHashName (+0xC), +0x204 and +0x23C to 1, +0x230, +0x234,
 * +0x240 and +0x244 to 0, +0x19C to 26 and +0x22C and +0x238 to -1. On cSSXApp_getMCManager()'s manager it sets +0x120,
 * +0x124 and +0x12C to 1, +0x11C and +0x128 to 0, and installs nine callbacks from fememcard (cFEStateMemCard_callbackLoadReady,
 * cFEStateMemCard_callbackFileNotFound, cFEStateMemCard_callbackFileChecked, cFEStateMemCard_callbackSysRecheckFile, cFEStateMemCard_callbackCardsChecked, func_00186C98, func_00186D60, cFEStateMemCard_callbackAcknowledgeCardStatusChange,
 * cFEStateMemCard_callbackConfirmNoFreeBlocks). Callers allocate 584 bytes ("cFEStateMemCard").
 *
 * @param engine  the UI engine
 * @return @p self
 */
extern "C" sFEStateLoadK186A08* func_00186A08(sFEStateLoadK186A08* self, void* engine)
{
    cFEMemCard_cFEMemCard(self, engine, 2);
    int one = 1;
    self->vtbl = cFEStateMemCard_vtbl;
    D_004A14B8_K186A08 = self;
    D_004A14B4 = 0;
    D_004A14BC = 0;
    self->f204 = one;
    self->fC = one;
    self->f23C = one;
    self->f230 = 0;
    self->f234 = 0;
    self->f240 = 0;
    self->f244 = 0;
    self->f19C = 26;
    self->f22C = -1;
    self->f238 = -1;
    sMemCardK186A08* mc = (sMemCardK186A08*)cSSXApp_getMCManager(SSXApp);
    mc->f12C = one;
    mc->f124 = one;
    mc->f120 = one;
    mc->cbA4 = (void*)cFEStateMemCard_callbackLoadReady;
    mc->cb38 = (void*)cFEStateMemCard_callbackFileNotFound;
    mc->cb94 = (void*)cFEStateMemCard_callbackFileChecked;
    mc->cbAC = (void*)cFEStateMemCard_callbackSysRecheckFile;
    mc->cb90 = (void*)cFEStateMemCard_callbackCardsChecked;
    mc->cb48 = (void*)func_00186C98;
    mc->cb3C = (void*)func_00186D60;
    mc->cbB0 = (void*)cFEStateMemCard_callbackAcknowledgeCardStatusChange;
    mc->cb5C = (void*)cFEStateMemCard_callbackConfirmNoFreeBlocks;
    mc->f128 = 0;
    mc->f11C = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateload", cFEStateMemCard__cFEStateMemCard);
#ifdef SKIP_ASM
extern void* cFEStateMemCard_vtbl[];
extern int D_004A14B8;
extern "C" void cFEMemCard__cFEMemCard(void* self, int flags);

/**
 * Destroys a memory-card state: clears D_004A14B8 and runs the base destructor cFEMemCard__cFEMemCard.
 *
 * @param flags  destructor flags, passed on
 */
extern "C" void cFEStateMemCard__cFEStateMemCard(void* self, int flags)
{
    *(void***)((char*)self + 8) = cFEStateMemCard_vtbl;
    D_004A14B8 = 0;
    cFEMemCard__cFEMemCard(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_00186B48__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0x100 for every menu event (the onWidgetMenu slot of cFEStateMemCard_vtbl).
 *
 * @return 0x100
 */
int func_00186B48(void* self)
{
    return 0x100;
}
#endif

