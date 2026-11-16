#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onCreateScreen);
#ifdef SKIP_ASM
struct sVec4_181148 { float x, y, z, w; } __attribute__((aligned(16)));
struct sCam_181148 { char pad[0x10]; float fov; };
int GetHashValue32(char* str);
extern char str_08sel_char[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0015E050(void* cam, sVec4_181148* a, sVec4_181148* b);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);

/**
 * Creates the character-select screen "08sel_char" and sets up the front-end camera and rider view.
 *
 * The camera (cFE::mCamera) gets (0, 200, 0) and the origin, probably position and target (func_0015E050),
 * and a field of view of 0.436 (25 degrees). Calls func_0019E538 with 0 on the user's front-end rider, sets
 * +0x80 to 1, +0x5C to -1 and +0x58 and +0x60 to 0.
 */
extern "C" void cFEStateCharSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_08sel_char), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    sCam_181148* cam = (sCam_181148*)SSXApp->RealFE;
    sVec4_181148 pos;
    pos.x = 0.0f;
    pos.y = 200.0f;
    pos.z = 0.0f;
    pos.w = 1.0f;
    sVec4_181148 lookAt;
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 0.0f;
    lookAt.w = 1.0f;
    func_0015E050((char*)cam + 0x10, &pos, &lookAt);
    cam->fov = 0.4363323450088501f;
    func_0019E538(func_001A0548((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    *(int*)((char*)self + 0x80) = 1;
    *(float*)((char*)self + 0x5C) = -1.0f;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x60) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onDestroyScreen);
#ifdef SKIP_ASM
extern signed char D_00535C11[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);
void* cUIState_onDestroyScreen(void* self);

/**
 * Destroys the character-select screen; off game path 0 it first resets the user's front-end rider.
 *
 * The rider (cFE::mRiderManager slot for mUserNum) gets func_0019E538 with 0, then cUIState_onDestroyScreen()
 * runs.
 */
extern "C" void cFEStateCharSelect_onDestroyScreen(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    char* mgr = (char*)SSXApp->RealFE;
    if (mgr != 0 && D_00535C11[0] != 0) {
        func_0019E538(func_001A0548(mgr + 0xB0, *(signed char*)((char*)self + 0x44)), 0);
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onGainTransition);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_00181BD0(void* self, int index, signed char charID);

/**
 * Shows the stats of the player's current character when the character-select gain transition starts.
 *
 * When @p on is nonzero, reads the character id for mUserNum from the new-player interface and passes both to
 * func_00181BD0(), which fills the seven stat texts and bars and the overall average.
 *
 * @param on  the state's first-time flag as passed by the state stack; nonzero on the transition's first call
 * @return always 0
 */
extern "C" int cFEStateCharSelect_onGainTransition(void* self, int on)
{
    if (on != 0) {
        int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
        func_00181BD0(self, *(signed char*)((char*)self + 0x44), id);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onGainFocus);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void func_001A0508(void* self, int idx, int a2, int a3);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_00181EF0(void* self);
extern "C" void cUIState_onGainFocus(void* self, int a1);
extern signed char D_00440F68[];
extern char str_Menu[];

/**
 * Gives the character-select screen the focus: selects the player's character in "Menu" and shows the rider.
 *
 * Finds the player's character among the 10 selectable ones (D_00440F68) and selects its entry. The front-end
 * rider is then set to D_00440F68[0] (not the player's character), its +0xCCC cleared, and func_00181EF0
 * places it before cUIState_onGainFocus() runs.
 *
 * @param pPrevState  the state that had focus before, passed on
 */
extern "C" void cFEStateCharSelect_onGainFocus(void* self, int pPrevState)
{
    int i = 0;
    int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)((char*)self + 0x44));
    for (; i < 10; i++) {
        if (D_00440F68[i] == id) {
            void* menu = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Menu));
            if (menu != 0)
                cUIMenu_setSelectedByIndex(menu, i);
            break;
        }
    }
    func_001A0508((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44), D_00440F68[0], 0);
    char* feRider = (char*)func_001A0548((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44));
    *(int*)(feRider + 0xCCC) = 0;
    func_00181EF0(self);
    cUIState_onGainFocus(self, pPrevState);
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the character-select state's 2D layer with the shared UI-state render routine func_0039E6B8().
 *
 * @return no meaningful value (func_0039E6B8() returns nothing)
 */
void* cFEStateCharSelect_onRender2D(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onUpdate);
#ifdef SKIP_ASM
extern "C" void func_00181EF0(void* self);
extern "C" void* func_0039E510(void* self);

/**
 * Updates the character-select state: poses the front-end rider model, then runs the base UI-state update.
 *
 * func_00181EF0() places and turns the rider model once it is loaded and restarts its animation when the state
 * asks for it. func_0039E510() is the base UI-state update, which dispatches controller input to the state and
 * its screens.
 */
extern "C" void cFEStateCharSelect_onUpdate(void* self)
{
    func_00181EF0(self);
    func_0039E510(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onInputBegin);
#ifdef SKIP_ASM
struct sPadVtI_1450 { short delta; short index; int (*fn)(void*); };
struct sPadVtF_1450 { short delta; short index; float (*fn)(void*); };

static inline int padBtn_1450(char* pad, int slot)
{
    sPadVtI_1450* vt = *(sPadVtI_1450**)(pad + 8);
    return vt[slot].fn(pad + vt[slot].delta);
}

static inline float padAxis_1450(char* pad, int slot)
{
    sPadVtF_1450* vt = *(sPadVtF_1450**)(pad + 8);
    return vt[slot].fn(pad + vt[slot].delta);
}

static inline float wrapHi_1450(float a)
{
    while (a >= 360.0f)
        a -= 360.0f;
    return a;
}

static inline float wrapLo_1450(float a)
{
    while (a < 0.0f)
        a += 360.0f;
    return a;
}

// PORT: g++ `<?` (min) operator.
static inline float clamp_1450(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

/**
 * Lets the player turn and adjust the character model on the select screen with the analogue inputs.
 *
 * While input 35 is held, axis 38 times 3 turns the model (+0x60, degrees, wrapped to 0-360). Inputs 33 and
 * 34 change +0x58 (clamped to 0-1) and +0x5C (clamped to -1-1) by their axes (36, 37) times -0.1.
 *
 * @param pInput  the input object
 * @return 1 when any of the values changed, else 0
 */
extern "C" int cFEStateCharSelect_onInputBegin(char* self, char* pInput)
{
    int changed = 0;
    if (padBtn_1450(pInput, 35)) {
        *(float*)(self + 0x60) += padAxis_1450(pInput, 38) * 3.0f;
        if (*(float*)(self + 0x60) >= 360.0f) {
            float angle = *(float*)(self + 0x60);
            do
                angle -= 360.0f;
            while (angle >= 360.0f);
            *(float*)(self + 0x60) = angle;
        }
        float angle = *(float*)(self + 0x60);
        float minAngle = 0.0f;
        if (angle < minAngle) {
            float wrapped;
            do {
                wrapped = angle + 360.0f;
                angle = wrapped;
            } while (wrapped < minAngle);
            *(float*)(self + 0x60) = wrapped;
        }
        changed = 1;
    }
    if (padBtn_1450(pInput, 33)) {
        *(float*)(self + 0x58) = clamp_1450(*(float*)(self + 0x58) + padAxis_1450(pInput, 36) * -0.10000000149011612f, 0.0f, 1.0f);
        changed = 1;
    }
    if (padBtn_1450(pInput, 34)) {
        *(float*)(self + 0x5C) = clamp_1450(*(float*)(self + 0x5C) + padAxis_1450(pInput, 37) * -0.10000000149011612f, -1.0f, 1.0f);
        changed = 1;
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern signed char D_00535C11[];
extern signed char D_00440F68[];
extern char str_names_menu[];
extern char str_titlesel_character[];
extern char str_titlesel_character_multi[];
extern char str_kT_TITLEPlayer1SelectCharacter[];
extern char str_kT_TITLEPlayer2SelectCharacter[];
struct sCharKey_1620 { char c[2]; };
extern sCharKey_1620 D_004A13A0[];

struct sWVt_1620 { short delta; short index; void (*fn)(void*, int); };
struct sWidget_1620 {
    int f0;
    int f4;
    sWVt_1620* vt;              // 0x8
    int fC;
    int f10;
    unsigned b0 : 1;            // 0x14
    unsigned b1 : 6;
    unsigned b7 : 1;
    unsigned b8 : 24;
    int id18;                   // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;                   // 0x38
};

static inline void refresh_1620(sWidget_1620* w)
{
    w->vt[9].fn((char*)w + w->vt[9].delta, 0);
}

/**
 * Sets up the character-select widgets: the name menu, the titles and the ten character names.
 *
 * "names menu" gets flag bit 0 cleared and bit 7 set. On game path 2 "titlesel_character" is hidden and
 * "titlesel_character_multi" shows the player 1 or 2 title (by mUserNum); on other paths that one is hidden.
 * Texts "0" to "9" get the selectable characters' names, their index as id, and start hidden.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateCharSelect_onWidgetCreate(char* self, sWidget_1620* w)
{
    int hashName = w->hash;
    if (hashName == GetHashValue32(str_names_menu)) {
        w->b0 = 0;
        w->b7 = 1;
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (hashName == GetHashValue32(str_titlesel_character) && D_00535C11[0] == 2) {
        refresh_1620(w);
        return;
    }
    if (hashName == GetHashValue32(str_titlesel_character_multi)) {
        if (D_00535C11[0] == 2) {
            switch (*(signed char*)(self + 0x44)) {
            case 0:
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLEPlayer1SelectCharacter));
                break;
            case 1:
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLEPlayer2SelectCharacter));
                break;
            }
            return;
        }
        refresh_1620(w);
        return;
    }
    sCharKey_1620 key = D_004A13A0[0];
    for (int i = 0; i < 10; i++) {
        key.c[0] = '0' + i;
        if (hashName == GetHashValue32(key.c)) {
            cUIText_setAsciiString((cUIText*)w, (const char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), D_00440F68[i], 0));
            w->id18 = i;
            refresh_1620(w);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" void* func_001887A0(void* mem, void* owner, int a2);
extern "C" void* func_0018ECA0(void* mem, void* owner, int a2, unsigned char a3);
extern "C" void* func_001A0548(void* self, int a1);
extern "C" void func_0019E538(void* self, int a1);
extern "C" void func_001A1CB8(void* self, int rider, unsigned char mask);
extern "C" void func_00181BD0(void* self, int index, signed char charID);
extern "C" void func_001472C8(void* iface, int rider, int idx);
extern "C" void func_00147448(void* iface, int rider, int id);
extern "C" void func_001474A8(void* iface, int rider, int id);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern signed char D_00440F68[];
extern char str_FEOptions[];
extern char str_cFEStateProfileLoad[];

class cWidgetBase_17E8 {
public:
    int pad0[2];
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

class cWidget_17E8 : public cWidgetBase_17E8 {
public:
    char padC[0xC];
    int id;                         // 0x18
};
typedef cWidget_17E8 sObj_17E8;

class cIface_17E8 {
public:
    int pad0[3];
    virtual void commit();
};
typedef cIface_17E8 sIface_17E8;

class cStateFactory_17E8 {
public:
    int pad0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* onSelect(void* owner, int id);
    virtual void* onBack(void* owner, int id);
};

struct sCharSel_17E8 {
    char pad0[0x10];
    char* engine;                   // 0x10
    signed char player;             // 0x14
    char pad15[0x2F];
    signed char rider;              // 0x44
    char pad45[0x7];
    int board;                      // 0x4C
    char pad50[0x30];
    int dirty;                      // 0x80
};

static inline char* riderSlot_17E8(sCharSel_17E8* self)
{
    return (char*)func_001A0548((char*)SSXApp->RealFE + 0xB0, self->rider);
}

static inline void refresh_17E8(sIface_17E8* iface)
{
    iface->commit();
}

/**
 * Handles character-select events: picking or highlighting a character, back, options and profile load.
 *
 * Event 5 sets +0xC20 (clearing +0xC1C) on the user's front-end rider, stores the controller, the character
 * and +0x4C in the player table, commits it, records the controller mask (func_001A1CB8) and pushes the
 * custom interface's next state. Event 1 shows the highlighted name and its stats (func_00181BD0) and stores
 * the choice; 2 hides the name. 6 pushes the previous state; 7 opens "FEOptions" (state bits 0x100); 8 opens
 * "cFEStateProfileLoad" for this controller.
 *
 * @param widget  the widget (ignored when 0); its id (+0x18) is the character index
 * @param event  the widget event
 */
// Character select: widget event handler.
extern "C" void cFEStateCharSelect_onWidgetEvent(sCharSel_17E8* self, sObj_17E8* widget, int event)
{
    if (widget == 0)
        return;
    switch (event) {
    case 5: {
        int on = 1;
        char* feRider = riderSlot_17E8(self);
        *(int*)(feRider + 0xC20) = on;
        *(int*)(feRider + 0xC1C) = 0;
        int rider = self->rider;
        int player = self->player;
        int ch = D_00440F68[widget->id];
        int board = self->board;
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001472C8(iface, rider, player);
        func_00147448(iface, rider, ch);
        func_001474A8(iface, rider, board);
        refresh_17E8(iface);
        unsigned char mask = on << self->player;
        func_001A1CB8(*(void**)self->engine, self->rider, mask);
        void* st = (*(cStateFactory_17E8**)self->engine)->onSelect(self, widget->id);
        if (st != 0)
            cUIStateStack_push(self->engine + 0x18, st);
        func_0019E538(riderSlot_17E8(self), 0);
        self->dirty = 0;
        break;
    }
    case 6: {
        void* st = (*(cStateFactory_17E8**)self->engine)->onBack(self, widget->id);
        if (st != 0) {
            cUIStateStack_push(self->engine + 0x18, st);
            func_0019E538(riderSlot_17E8(self), 0);
            self->dirty = 0;
        }
        break;
    }
    case 1: {
        widget->setVisible(1);
        func_00181BD0(self, self->rider, D_00440F68[widget->id]);
        int rider = self->rider;
        int player = self->player;
        int ch = D_00440F68[widget->id];
        int board = self->board;
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001472C8(iface, rider, player);
        func_00147448(iface, rider, ch);
        func_001474A8(iface, rider, board);
        refresh_17E8(iface);
        break;
    }
    case 2:
        widget->setVisible(0);
        break;
    case 3:
    case 4:
        break;
    case 7: {
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001472C8(iface, self->rider, self->player);
        refresh_17E8(iface);
        char* st = (char*)func_001887A0(cMemMan_alloc(0x54, str_FEOptions, 0x100, 0), self->engine, 1);
        *(int*)(st + 0x1C) = (*(int*)(st + 0x1C) & ~0x3F00) | 0x100;
        cUIStateStack_pushSpecial(self->engine + 0x18, st, 0, 0);
        func_0019E538(riderSlot_17E8(self), 0);
        self->dirty = 0;
        break;
    }
    case 8: {
        sIface_17E8* iface = (sIface_17E8*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001472C8(iface, self->rider, self->player);
        refresh_17E8(iface);
        void* st = func_0018ECA0(cMemMan_alloc(0x280, str_cFEStateProfileLoad, 0x100, 0), self->engine, 2, 1 << self->player);
        cUIStateStack_push(self->engine + 0x18, st);
        char* top = *(char**)self->engine;
        if (top != 0)
            *(int*)(top + 0xC) = 9;
        func_0019E538(riderSlot_17E8(self), 0);
        self->dirty = 0;
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181BD0);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int func_00148B78(void* iface, int a, int b);
extern "C" int func_00148BC8(void* iface, int a, int b);
extern "C" int func_00148C18(void* iface, int a, int b);
extern "C" int func_00148C68(void* iface, int a, int b);
extern "C" int func_00148CB8(void* iface, int a, int b);
extern "C" int func_00148D08(void* iface, int a, int b);
extern "C" int func_00148D58(void* iface, int a, int b);
extern "C" void func_001A0508(void* self, int idx, int a2, int a3);
extern "C" void cUIProgress_setValue(void* bar, int v);
extern "C" double fptodp(float f);
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_1f;
extern char str_NumAttrOver[];
extern char str_overall[];
struct sStr9_181BD0 { char c[9]; };
struct sStr4_181BD0 { char c[4]; };
extern sStr9_181BD0 D_0045D6E0;
extern char str_0pb[];

class cFEVObj_181BD0 {
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
    virtual void v09(int on);
};

// PORT: the unit declares func_00181BD0 with an int index; the body sign-extends both id arguments.
extern "C" void func_00181BD0_impl(void* self, signed char rider, signed char charID) __asm__("func_00181BD0");

extern "C" void func_00181BD0_impl(void* self, signed char rider, signed char charID)
{
    sStr9_181BD0 name = D_0045D6E0;
    char bar[8];
    *(sStr4_181BD0*)bar = *(sStr4_181BD0*)str_0pb;
    func_00416210(bar + 4, 0, 4);
    char buf[100];
    int stats[7];
    float k = 0.2f;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    stats[0] = func_00148BC8(iface, rider, charID);
    stats[1] = func_00148C68(iface, rider, charID);
    stats[2] = func_00148B78(iface, rider, charID);
    stats[3] = func_00148CB8(iface, rider, charID);
    stats[4] = func_00148D58(iface, rider, charID);
    stats[5] = func_00148D08(iface, rider, charID);
    stats[6] = func_00148C18(iface, rider, charID);
    int total = 0;
    for (int i = 0; i < 7; i++) {
        name.c[7] = '0' + i;
        cFEVObj_181BD0* t = (cFEVObj_181BD0*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name.c));
        bar[0] = '0' + i;
        char* b = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(bar));
        if (t != 0 && b != 0) {
            int v = stats[i];
            // PORT: g++ minimum/maximum operator
            int c = v >? 0;
            total += v;
            sprintf(buf, &str_pct_1f, fptodp((float)v * k));
            t->v09(1);
            cUIText_setAsciiString((cUIText*)t, buf);
            *(int*)(b + 0x80) = 0x37;
            cUIProgress_setValue(b, c);
        }
    }
    total -= total % 5;
    char* ovr = str_overall;
    cUIText* avg = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_NumAttrOver));
    sprintf(buf, &str_pct_1f, fptodp((float)total * 0.02857142873108387f));
    cUIText_setAsciiString(avg, buf);
    char* ob = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(ovr));
    *(int*)(ob + 0x80) = 0x37;
    cUIProgress_setValue(ob, total / 7);
    func_001A0508((char*)SSXApp->RealFE + 0xB0, rider, charID, 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181EF0);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_reset(void* anim);
extern "C" void cRiderAnimBase_play(void* anim, int id, int a2, float t);

struct sQuat_181EF0
{
    float x, y, z, w;
    sQuat_181EF0() {}
    // Assigns w first, like the w-first quaternion ctors elsewhere (CHEATSHEET "Quaternions").
    sQuat_181EF0(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct sXform_181EF0
{
    sQuat_181EF0 pos;
    sQuat_181EF0 rot;
};

extern sQuat_181EF0 D_004FF140;
extern sQuat_181EF0 t4vect_YAxis;
extern sQuat_181EF0 D_004FF160;
extern "C" void func_0019F3E8(void* rider, sXform_181EF0* xf);
extern "C" void func_0019F548(void* rider, sXform_181EF0* xf);

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_181EF0 QuatMul_181EF0(const sQuat_181EF0& a, const sQuat_181EF0& b)
{
    sQuat_181EF0 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sQuat_181EF0 AxisAngle_181EF0(const sQuat_181EF0& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
    return sQuat_181EF0(s * axis.x, s * axis.y, s * axis.z, c);
}

extern "C" void func_00181EF0(void* vself)
{
    char* self = (char*)vself;
    char* rider = (char*)func_001A0548((char*)SSXApp->RealFE + 0xB0, *(signed char*)(self + 0x44));
    int ok = 0;
    if (*(unsigned int*)rider < 10 && *(int*)(rider + 0xCB8) != 0) {
        ok = *(int*)(rider + 0xCB4) != 0;
    }
    if (ok && *(int*)(self + 0x80) != 0) {
        sQuat_181EF0 pos;
        sXform_181EF0 xf;
        float ang = *(float*)(self + 0x60);
        {
            sQuat_181EF0 p;
            p.x = 136.0f;
            p.y = -250.0f;
            p.z = -82.0f;
            p.w = 1.0f;
            pos = p;
            xf.pos = p;
            xf.rot.x = 0.0f;
            xf.rot.y = 0.0f;
            xf.rot.z = 0.0f;
            xf.rot.w = 1.0f;
            xf.rot = QuatMul_181EF0(xf.rot, AxisAngle_181EF0(D_004FF140, 0.0f));
        }
        xf.rot = QuatMul_181EF0(xf.rot, AxisAngle_181EF0(t4vect_YAxis, 0.0f));
        ang += 100.0f;
        ang *= 0.01745329424738884f;
        xf.rot = QuatMul_181EF0(xf.rot, AxisAngle_181EF0(D_004FF160, ang));
        func_0019F3E8(rider, &xf);
        if (*(int*)(self + 0x54) != 0) {
            void* anim = *(void**)(rider + 0xC);
            if (anim != 0) {
                cRiderAnimBase_reset(anim);
                cRiderAnimBase_play(anim, 0x1B2, 0, -1.0f);
            }
            *(int*)(rider + 0xCCC) = 0;
            *(int*)(self + 0x54) = 0;
        }
        {
            sQuat_181EF0 p;
            p.x = 0.0f;
            p.y = 10000.0f;
            p.z = 0.0f;
            p.w = 1.0f;
            xf.pos = p;
        }
        func_0019F548(rider, &xf);
        func_0019E538(rider, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182220);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* func_0014EEC8(void* self, int player, int index);
extern "C" int func_00157EE0(void* iface, int a1, int a2, int a3);
extern "C" void* func_0015A478();
extern "C" int func_00398380(void* self, int id);
extern "C" void* func_0039E318(void* self, void* engine, void* owner);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern void* cFEStateCheatCharSelect_vtbl[];
extern char* D_00440F78[];

struct sCharEnt_2220 { char* name; char* label; int f8; char pad[3]; signed char id; };
struct sCharList_2220 { char pad[0x28]; sCharEnt_2220* ents; char pad2C[0x50 - 0x2C]; int count; };
struct sCharSel_2220 {
    char pad0[0x8];
    void** vt;                  // 0x8
    int fC;                     // 0xC
    char* owner;                // 0x10
    char f14;
    unsigned char f15;          // 0x15
    char pad16[0x44 - 0x16];
    signed char player;         // 0x44
    char pad45[0x54 - 0x45];
    int sel[6];                 // 0x54
    int f6C;
    int f70;                    // 0x70
    int f74;                    // 0x74
    int count;                  // 0x78
    int ids[30];                // 0x7C
    int hashes[30];             // 0xF4
    char* names[30];            // 0x16C
};

extern "C" sCharSel_2220* func_00182220(sCharSel_2220* self, void* engine, void* owner, signed char player, int slot)
{
    func_0039E318(self, engine, owner);
    self->fC = 10;
    self->vt = cFEStateCheatCharSelect_vtbl;
    self->player = player;
    self->count = 1;
    self->f70 = 0;
    self->ids[0] = 0;
    char* o = self->owner;
    int h = GetHashValue32(D_00440F78[slot]);
    self->hashes[0] = func_00398380(o + 0x58, h);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 13);
    sCharList_2220* list = (sCharList_2220*)func_0015A478();
    int n = list->count;
    for (int i = 0; i < n; i++) {
        sCharEnt_2220* e = &list->ents[i];
        if (func_00157EE0(iface, self->player, slot, e->id)) {
            char* lbl = e->label;
            self->ids[self->count] = e->id;
            char* o2 = self->owner;
            int h2 = GetHashValue32(lbl);
            self->hashes[self->count] = func_00398380(o2 + 0x58, h2);
            self->names[self->count] = e->name;
            self->count++;
        }
    }
    self->names[0] = (char*)func_0014EEC8(cBE_getInterface_Fv(cBE_getBE(), 2), slot, 0);
    for (int j = 5; j >= 0; j--)
        self->sel[j] = 0;
    self->f74 = 0;
    self->f15 = func_001A1CD0(*(void**)self->owner, self->player);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_131cheat_char[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the cheat-character select screen "131cheat_char" and plays its first frame.
 */
extern "C" void cFEStateCheatCharSelect_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_131cheat_char), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetCreate);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char str_6pics[];
extern char str_arrowleft[];
extern char str_arrowright[];
extern char str_nametext[];
extern char D_004A1408[];
extern char* D_00440FA0[];
extern char D_004C66B8[];
extern "C" void cUIMenu_setScrollableMenu(void* w, unsigned char n);

struct sCWVt_2460 { short delta; short index; void (*fn)(void*, int); };
struct sCWVtP_2460 { short delta; short index; void (*fn)(void*, void*); };
struct sCW_2460 {
    int f0;
    int f4;
    sCWVt_2460* vt;             // 0x8
    int fC;
    int f10;
    unsigned b0 : 1;            // 0x14
    unsigned b1 : 31;
    int id18;                   // 0x18
    char pad1C[0x38 - 0x1C];
    int hash;                   // 0x38
    char pad3C[0x78 - 0x3C];
    int f78;                    // 0x78
    int f7C;                    // 0x7C
};
struct sCheat_2460 {
    char pad0[0x48];
    sCW_2460* f48;              // 0x48
    sCW_2460* f4C;              // 0x4C
    sCW_2460* f50;              // 0x50
    sCW_2460* slots[6];         // 0x54
    sCW_2460* f6C;              // 0x6C
    int f70, f74;
    int count;                  // 0x78
    char pad7C[0xF4 - 0x7C];
    int vals[6];                // 0xF4
};

static inline void vcall_2460(sCW_2460* w, int k, int v)
{
    w->vt[k].fn((char*)w + w->vt[k].delta, v);
}

/**
 * Binds the cheat-character select widgets: picture menu, arrows, name text and picture slots.
 *
 * "6pics" is scrollable when there are 6 or more entries; "arrowleft" and "arrowright" show from 7 entries;
 * "nametext" starts empty. The six picture slots ("pic1" ...) take the first entries' values; slots past the
 * entry count are hidden and disabled, and the first slot gets the highlight colour.
 *
 * @param w  the widget being created
 */
extern "C" void cFEStateCheatCharSelect_onWidgetCreate(sCheat_2460* self, sCW_2460* w)
{
    int hashName = w->hash;
    if (hashName == GetHashValue32(str_6pics)) {
        self->f6C = w;
        w->id18 = 0;
        self->f6C->b0 = 0;
        if (self->count >= 6) cUIMenu_setScrollableMenu(self->f6C, self->count);
        return;
    }
    if (hashName == GetHashValue32(str_arrowleft)) {
        self->f48 = w;
        vcall_2460(w, 9, self->count >= 7);
        return;
    }
    if (hashName == GetHashValue32(str_arrowright)) {
        self->f4C = w;
        vcall_2460(w, 9, self->count >= 7);
        return;
    }
    if (hashName == GetHashValue32(str_nametext)) {
        self->f50 = w;
        cUIText_setAsciiString((cUIText*)w, D_004A1408);
        return;
    }
    int i = 0;
    char** tb = D_00440FA0;
    sCW_2460** sp = self->slots;
    for (; i < 6; sp++, tb++, i++) {
        if (hashName == GetHashValue32(*tb)) {
            *sp = w;
            w->id18 = i;
            if (i == 0) {
                sCWVtP_2460* vtEntry = &((sCWVtP_2460*)self->slots[0]->vt)[21];
                vtEntry->fn((char*)self->slots[0] + vtEntry->delta, D_004C66B8);
            }
            if (i >= self->count) {
                vcall_2460(*sp, 9, 0);
                vcall_2460(*sp, 8, 1);
            } else {
                sCW_2460* sl = *sp;
                sl->f7C = (&self->vals[0])[i];
                sl->f78 = -1;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetEvent);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_001474A8(void* iface, int rider, int id);
extern "C" void func_0039F190(void*, int);
extern char D_004C66A8[];
extern char D_004C66B8[];

struct sVEK182690a {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct sVEK182690b {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Handles the cheat-character pictures: highlight, the shown name, picking one and backing out.
 *
 * Event 1 highlights the picture and shows its entry's name, 2 restores its colour. Event 5 stores the
 * entry's value (0 for the first entry) in +0x70 and in the player table (func_001474A8), commits it, sets
 * +0x74 and closes the popup; 6 closes it with +0x70 = 0.
 *
 * @param menu  the picture widget (ignored when 0); its id plus the menu's scroll offset is the entry
 * @param msg  the widget event
 */
extern "C" void cFEStateCheatCharSelect_onWidgetEvent(void* self, void* menu, unsigned int msg)
{
    if (menu == 0)
        return;
    switch (msg) {
    case 2: {
        sVEK182690a* vt = *(sVEK182690a**)((char*)menu + 8);
        vt[21].fn((char*)menu + vt[21].delta, D_004C66A8);
        break;
    }
    case 1: {
        int sel = *(int*)((char*)menu + 0x18) + *(unsigned char*)(*(char**)((char*)self + 0x6C) + 0x98);
        sVEK182690a* vt = *(sVEK182690a**)((char*)menu + 8);
        vt[21].fn((char*)menu + vt[21].delta, D_004C66B8);
        cUIText_setAsciiString(*(cUIText**)((char*)self + 0x50), *(const char**)((char*)self + (sel << 2) + 0x16C));
        break;
    }
    case 6:
        *(int*)((char*)self + 0x70) = 0;
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    case 5: {
        int sel = *(int*)((char*)menu + 0x18) + *(unsigned char*)(*(char**)((char*)self + 0x6C) + 0x98);
        if (sel == 0)
            *(int*)((char*)self + 0x70) = 0;
        else
            *(int*)((char*)self + 0x70) = *(int*)((char*)self + (sel << 2) + 0x7C);
        char* pl = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        func_001474A8(pl, *(signed char*)((char*)self + 0x44), *(int*)((char*)self + 0x70));
        sVEK182690b* vt = *(sVEK182690b**)(pl + 0xC);
        vt[1].fn(pl + vt[1].delta);
        *(int*)((char*)self + 0x74) = 1;
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self);

/**
 * Handles the cheat-character menu's events: entry checks, picture refresh and which events pass on.
 *
 * @param pMenu  the menu (unused)
 * @param msg  the menu event
 * @param value  the entry index for event 1
 * @return 0x100 for events 7-9, for event 1 whether @p value is below the entry count, else 0x101 (event 5
 *         refreshes the pictures first)
 */
extern "C" int cFEStateCheatCharSelect_onWidgetMenu(void* self, int pMenu, unsigned int msg, int value)
{
    switch (msg) {
    case 7:
    case 8:
    case 9:
        return 0x100;
    case 1:
        return value < *(int*)((char*)self + 0x78);
    case 5:
        func_00182870(self);
        break;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182870);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self)
{
    int i;
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x6c) + 0x98);
    for (i = 0; i < 6; i++) {
        void* it = ((void**)((char*)self + 0x54))[i];
        if (it != 0) {
            int v = ((int*)((char*)self + 0xf4))[idx + i];
            *(int*)((char*)it + 0x78) = -1;
            *(int*)((char*)it + 0x7c) = v;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharselect", func_001828C0);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern void* cFEStateCharSetup_vtbl[];

extern "C" void* func_001828C0(void* self, void* engine, signed char idx)
{
    func_0039E2A0(self, engine);
    *(int*)((char*)self + 0xC) = 0xC;
    *(void***)((char*)self + 0x8) = cFEStateCharSetup_vtbl;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

