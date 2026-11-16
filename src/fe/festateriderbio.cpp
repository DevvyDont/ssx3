#include "common.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_00182DB8(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void func_001A0570(void* self, int a1, int a2);
extern "C" void func_001A0508(void* self, int idx, int a2, int a3);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);
extern "C" int func_001577A0(void* self, int a1, int a2);
extern char str_154rider_details[];

struct sVE_32A8 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Creates the rider-details screen "154rider_details" and shows the player's character.
 *
 * Runs func_00182DB8, reloads the player table (the interface's slot 2, discard), sets up the user's
 * front-end rider with the player's character, calls func_0028F140 with 1, and sets +0x50 when the character
 * has any cheat characters unlocked (func_001577A0).
 */
extern "C" void cFEStateRiderDetail_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_154rider_details), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_00182DB8(self);
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    int charID = cBENewPlayerInterface_getPlayerCharID(pi, *(signed char*)((char*)self + 0x44));
    sVE_32A8* vt = *(sVE_32A8**)((char*)pi + 0xC);
    vt[2].fn((char*)pi + vt[2].delta);
    func_001A0570((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44), 0);
    func_001A0508((char*)SSXApp->RealFE + 0xB0, *(signed char*)((char*)self + 0x44), charID, 0);
    func_0028F140(func_0028B180(), 1);
    *(int*)((char*)self + 0x50) = 1;
    if (func_001577A0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)((char*)self + 0x44), charID) == 0) {
        *(int*)((char*)self + 0x50) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEState_onGainFocus(void* self, int a1);
extern char str_Option_1[];
extern char str_Option_2[];
extern char str_Option_3[];
extern char str_Option_4[];
extern char str_Option_5[];

struct sVtEnt_001833D0 { short delta; short index; void (*fn)(void*, int); };

static inline void showObj_001833D0(void* self, char* name, int id)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (obj != 0) {
        sVtEnt_001833D0* vt = *(sVtEnt_001833D0**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
        *(int*)((char*)obj + 0x18) = id;
    }
}

/**
 * Gives focus to the rider detail screen: shows its five option objects and tags them, then runs the base
 * handler.
 *
 * Each of "Option_1" to "Option_5" found on the screen (+0x40) is made visible (setVisible, slot 9) and gets 5
 * to 9 in mMiscData (+0x18). Then calls cFEState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before (On Tour pPrevState), passed on to cFEState_onGainFocus()
 */
extern "C" void cFEStateRiderDetail_onGainFocus(void* self, int pPrevState)
{
    showObj_001833D0(self, str_Option_1, 5);
    showObj_001833D0(self, str_Option_2, 6);
    showObj_001833D0(self, str_Option_3, 7);
    showObj_001833D0(self, str_Option_4, 8);
    showObj_001833D0(self, str_Option_5, 9);
    cFEState_onGainFocus(self, pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onPreRender2D);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void func_00194498(void* obj);
extern char str_Option_2[];

/**
 * Greys out "Option_2" (the cheat characters) when the character has none unlocked (+0x50 is 0).
 *
 * func_00194498 disables the widget and caps its alpha at 0.5.
 */
extern "C" void cFEStateRiderDetail_onPreRender2D(void* self)
{
    if (*(int*)((char*)self + 0x50) == 0) {
        void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_Option_2));
        if (obj != 0) {
            func_00194498(obj);
        }
    }
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the rider-detail state's 2D layer with the shared UI-state render routine func_0039E6B8().
 *
 * @return no meaningful value (func_0039E6B8() returns nothing)
 */
void* cFEStateRiderDetail_onRender2D(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onUpdate);
#ifdef SKIP_ASM
extern "C" void func_0039E510(void*);
extern "C" void func_00182EC0(void*);

/**
 * Updates the rider-detail state: runs the base UI-state update, then poses the front-end rider model.
 *
 * func_0039E510() is the base UI-state update, which dispatches controller input to the state and its screens.
 * func_00182EC0() (shared with the character-setup state) places the loaded rider model and, when the state asks
 * for it, restarts its animation.
 */
extern "C" void cFEStateRiderDetail_onUpdate(void* self)
{
    func_0039E510(self);
    func_00182EC0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00147138(void* self, int a1, const char* name);
extern "C" void func_001474A8(void* self, int a1, int a2);
extern "C" void func_001831D0(void* self, int a1);
extern "C" unsigned int strlen(const char* s);
extern char str_131cheat_char[];

struct sVE_35F8 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline int Is_001835F8(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Takes the result of the rider-details popups when one closes (event 0x16): a cheat character or a name.
 *
 * From "131cheat_char" a confirmed choice (+0x74) is stored in the player table (func_001474A8) and
 * committed; from the keyboard (hash name 0) an accepted, non-empty text becomes the player's name
 * (func_00147138). Then func_001831D0 with 1.
 *
 * @param item  the popup that sent the event
 * @param msg  the popup event; only 0x16 is handled
 */
extern "C" void cFEStateRiderDetail_onPopupEvent(void* self, void* item, int msg)
{
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (msg == 0x16) {
        if (Is_001835F8(*(int*)((char*)item + 0xC), str_131cheat_char)) {
            int cheatCharId = *(int*)((char*)item + 0x70);
            if (*(int*)((char*)item + 0x74) != 0) {
                void* pi2 = cBE_getInterface_Fv(cBE_getBE(), 1);
                func_001474A8(pi2, *(signed char*)((char*)self + 0x44), cheatCharId);
                sVE_35F8* vt = *(sVE_35F8**)((char*)pi2 + 0xC);
                vt[1].fn((char*)pi2 + vt[1].delta);
            }
        } else if (*(int*)((char*)item + 0xC) == 0) {
            char* name = (char*)item + 0x74;
            if (*(int*)((char*)item + 0x5C) != 0 && strlen(name) != 0) {
                func_00147138(pi, *(signed char*)((char*)self + 0x44), name);
                sVE_35F8* vt = *(sVE_35F8**)((char*)pi + 0xC);
                vt[1].fn((char*)pi + vt[1].delta);
            }
        }
        func_001831D0(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" char* cBENewPlayerInterface_getPlayerName(void* iface, int rider);
extern "C" int func_001577A0(void* self, int a1, int a2);
extern "C" void* func_00182220(void* mem, void* engine, void* owner, int rider, int cid);
extern "C" void func_001831D0(void* self, int a1);
extern "C" void* func_001887A0(void* mem, void* engine, int a2);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* self, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cKeyboardPopup_setInputText(void* self, const char* str);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int v);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void func_001CE408(void* self, int v);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern const char str_cKeyboardPopup[];
extern const char str_cFEStateCheatCharSelect[];
extern const char str_FEOptions[];

struct sVE_3710 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
struct sBits_3710 {
    unsigned lo : 8;
    unsigned mode : 6;
    unsigned hi : 18;
};

/**
 * Handles the rider-details options: edit the name, pick a cheat character, other screens, back and options.
 *
 * Event 5 on option 8 opens an 8-character keyboard (several symbol keys excluded) holding the player's name;
 * on option 6 it opens "cFEStateCheatCharSelect" when the character has cheat characters; other options push
 * the custom interface's next state. Event 6 pushes the previous state; 7 opens "FEOptions" (mode 1).
 *
 * @param item  the widget (ignored when 0); its miscellaneous data (+0x18) is the option
 * @param msg  the widget event
 */
extern "C" void cFEStateRiderDetail_onWidgetEvent(char* self, char* item, unsigned int msg)
{
    if (item == 0) return;
    switch (msg) {
    case 5: {
        int sub = *(int*)(item + 0x18);
        if (sub == 8) {
            char* w = (char*)cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(0x444, str_cKeyboardPopup, 0x100, 0), *(void**)(self + 0x10), self, 0, 0xF);
            *(int*)(w + 0x140) = 0x37;
            cKeyboardPopup_setMaxCharacters(w, 8);
            cKeyboardPopup_setIncludeKey(w, 0x4B, 1, 2);
            *(int*)(w + 0x58) = 1;
            func_001CE408(w, 1);
            cKeyboardPopup_setIncludeKey(w, 0xE, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x27, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x28, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x34, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x1A, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x19, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x35, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x40, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x41, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x42, 0, 2);
            cUIStateStack_pushPopup(*(char**)(self + 0x10) + 0x18, w);
            cKeyboardPopup_setInputText(w, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)(self + 0x44)));
            func_001831D0(self, 0);
        } else if (sub == 6) {
            int cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), *(signed char*)(self + 0x44));
            if (func_001577A0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(signed char*)(self + 0x44), cid) > 0) {
                char* w = (char*)func_00182220(cMemMan_alloc(0x1E4, str_cFEStateCheatCharSelect, 0, 0), *(void**)(self + 0x10), self, *(signed char*)(self + 0x44), cid);
                *(unsigned char*)(w + 0x15) = func_001A1CD0(**(void***)(self + 0x10), *(signed char*)(self + 0x44));
                cUIStateStack_pushPopup(*(char**)(self + 0x10) + 0x18, w);
                func_001831D0(self, 0);
            }
        } else {
            char* obj = **(char***)(self + 0x10);
            sVE_3710* vt = *(sVE_3710**)(obj + 4);
            void* nextState = vt[4].fn(obj + vt[4].delta, self, sub);
            if (nextState != 0) {
                cUIStateStack_push(*(char**)(self + 0x10) + 0x18, nextState);
            }
        }
        func_001831D0(self, 0);
        break;
    }
    case 6: {
        char* obj = **(char***)(self + 0x10);
        sVE_3710* vt = *(sVE_3710**)(obj + 4);
        void* nextState = vt[5].fn(obj + vt[5].delta, self, *(int*)(item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push(*(char**)(self + 0x10) + 0x18, nextState);
            func_001831D0(self, 0);
        }
        break;
    }
    case 7: {
        char* w = (char*)func_001887A0(cMemMan_alloc(0x54, str_FEOptions, 0x100, 0), *(void**)(self + 0x10), 1);
        ((sBits_3710*)(w + 0x1C))->mode = 1;
        cUIStateStack_pushSpecial(*(char**)(self + 0x10) + 0x18, w, 0, 0);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183A98);
#ifdef SKIP_ASM
extern void* D_0046CCC0[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_00183A98(void* self, int a1, int a2)
{
    signed char idx = a2;
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046CCC0;
    *(int*)((char*)self + 0xC) = 0x12;
    *(signed char*)((char*)self + 0x44) = idx;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183B08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_hilite_right[];
extern char str_hilite_left[];
extern char str_helptext_choose[];
extern char str_helptext_custom[];

class cUIObj_183B08 {
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

extern "C" void func_00183B08(void* self)
{
    cUIObj_183B08* hiliteRight = (cUIObj_183B08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hilite_right));
    if (hiliteRight != 0) {
        hiliteRight->setVisible(0);
    }
    cUIObj_183B08* hiliteLeft = (cUIObj_183B08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hilite_left));
    if (hiliteLeft != 0) {
        hiliteLeft->setVisible(1);
    }
    cUIObj_183B08* helpChoose = (cUIObj_183B08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_choose));
    if (helpChoose != 0) {
        helpChoose->setVisible(1);
    }
    cUIObj_183B08* helpCustom = (cUIObj_183B08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_custom));
    if (helpCustom != 0) {
        helpCustom->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183C08);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_hilite_right[];
extern char str_hilite_left[];
extern char str_helptext_choose[];
extern char str_helptext_custom[];

class cUIObj_183C08 {
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

extern "C" void func_00183C08(void* self)
{
    cUIObj_183C08* hiliteRight = (cUIObj_183C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hilite_right));
    if (hiliteRight != 0) {
        hiliteRight->setVisible(1);
    }
    cUIObj_183C08* hiliteLeft = (cUIObj_183C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_hilite_left));
    if (hiliteLeft != 0) {
        hiliteLeft->setVisible(0);
    }
    cUIObj_183C08* helpChoose = (cUIObj_183C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_choose));
    if (helpChoose != 0) {
        helpChoose->setVisible(0);
    }
    cUIObj_183C08* helpCustom = (cUIObj_183C08*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_helptext_custom));
    if (helpCustom != 0) {
        helpCustom->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183D08);
#ifdef SKIP_ASM
struct sVec2_00183D08 { float x, y; };

class cWidget_00183D08 {
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
    virtual void v09(int a);
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
    virtual void v20(sVec2_00183D08* p);
    virtual void v21(sVec2_00183D08* p);
};

extern "C" void func_00183D08(cWidget_00183D08* self, void* rect, float scale)
{
    if (rect) {
        sVec2_00183D08 pos;
        self->v20(&pos);
        float k = scale * 256.0f;
        pos.x = (*(float*)((char*)rect + 0x14) - *(float*)((char*)rect + 0x10)) * k;
        pos.y = (*(float*)((char*)rect + 0x18) - *(float*)((char*)rect + 0xC)) * k;
        self->v21(&pos);
        self->v09(1);
    } else {
        self->v09(0);
    }
}
#endif

