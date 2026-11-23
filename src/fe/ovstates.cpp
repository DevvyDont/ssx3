#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "fe/fepopupselectmultiplayermode.h"
#include "be/beintoption.h"
#include "input/inputpad.h"
#include "sound/ssxaudio.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festatetitle.h"
#include "fe/festatemainmenu.h"
#include "fe/festatebonusmaterial.h"
#include "be/beintnetwork.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern char str_06title[];

/**
 * Builds the title screen ("06title"), plays its first frame, clears its idle timer and sets the front-end
 * music state to the title (func_0028F140(audio, 0)).
 */
extern "C" void cFEStateTitle_onCreateScreen(cFEStateTitle* self)
{
    void* engine;
    void* screen;
    self->mNumTicksIdle = 0;
    engine = (void*)self->mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_06title), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_0028F140(cAudioInterface_bxAudio(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onGainTransition);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Menu0000[];
extern char str_tReal[];
extern int gTitleScreenDelay;

class cUIObj_1947F8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int v);
    virtual void setFocus(int v);
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Holds the title's first appearance for 300 calls, then focuses its menu and shows "tReal".
 *
 * The countdown gTitleScreenDelay is global and stays 0 afterwards, so later visits finish at once. "Menu0000" gets
 * setFocus(1) (slot 7) and "tReal" (the Press START text) becomes visible.
 *
 * @return 1 when the transition is done, 0 while waiting
 */
extern "C" int cFEStateTitle_onGainTransition(cFEStateTitle* self)
{
    if (gTitleScreenDelay == 0 || --gTitleScreenDelay == 0) {
        cUIObj_1947F8* uiObj = (cUIObj_1947F8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu0000));
        if (uiObj != 0) {
            uiObj->setFocus(1);
        }
        uiObj = (cUIObj_1947F8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_tReal));
        if (uiObj != 0) {
            uiObj->setVisible(1);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onUpdate);
#ifdef SKIP_ASM
extern "C" void cUIState_onUpdate(void* self);
extern int cRealFE_mMovieList;


struct sApp_1948A8 {
    char pad[0xB0];
    cInputPad** lists[2];
};

static inline int isActive_1948A8(cInputPad* l, int j)
{
    if (j < l->NumButtons) {
        return ((&l->Buttons[0]))[j].IsHeld;
    }
    return 0;
}

/**
 * Counts idle frames on the title screen and requests the attract movie after 1800 of them.
 *
 * After the UI update (cUIState_onUpdate), an active entry in either controller's input list resets the timer
 * (+0x48). Past 1800 frames (30 s at 60 Hz) the timer restarts and bit 0x4 of cRealFE_mMovieList is set, which
 * cRealFE_update plays as a skippable movie.
 */
extern "C" void cFEStateTitle_onUpdate(cFEStateTitle* self)
{
    cUIState_onUpdate((self));
    self->mNumTicksIdle += 1;
    if (self->mNumTicksIdle > 1800) {
        self->mNumTicksIdle = 0;
        cRealFE_mMovieList |= 4;
        return;
    }
    int found = 0;
    int i;
    for (i = 0; i < 2; i++) {
        cInputPad* list = *((sApp_1948A8*)SSXApp)->lists[i];
        int n = list->NumButtons;
        if (n != 0) {
            int j;
            for (j = 0; j < n; j++) {
                if (isActive_1948A8(list, j)) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                self->mNumTicksIdle = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onPopupEvent__FPv);
#ifdef SKIP_ASM
/**
 * Ignores popup events: the title state's onPopupEvent has an empty body.
 */
void cFEStateTitle_onPopupEvent(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIThing_setSound(void* self, int a1, int a2);
extern char str_tReal[];

/**
 * Gives the title's "tReal" item choice id 0 and silences its error sound (mSound[0] = 0).
 *
 * @param item  the widget being created
 */
extern "C" void cFEStateTitle_onWidgetCreate(cFEStateTitle* self, cUIThing* item)
{
    int id = item->mHashName;
    if (id == GetHashValue32(str_tReal)) {
        item->mMiscData = 0;
        cUIThing_setSound(item, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * On Cross / Start, asks the front end for the next state and pushes it.
 *
 * The engine's custom interface (vtable slot 4, On Tour getNextState) gets the item's miscData; a non-null
 * result goes onto the UI state stack.
 *
 * @param msg  the widget (On Tour pWidget), despite the name
 * @param type  the widget event; only 5 acts
 */
extern "C" void cFEStateTitle_onWidgetEvent(cFEStateTitle* self, cUIThing* msg, int type)
{
    if (type == 5) {
        cUICustomInterface* obj = (cUICustomInterface*)((void*)self->mpEngine->mpCustom);
        void* nextState = obj->getNextState((cUIState*)self, msg->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Picks the title menu's button feedback: only Cross plays a sound.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (6 Cross)
 * @return 0x101 (animation and sound) for event 6, else 0x100 (animation only)
 */
extern "C" int cFEStateTitle_onWidgetMenu(cFEStateTitle* self, int pMenu, int Event)
{
    return Event != 6 ? 0x100 : 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_cFEStateMainMenu1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void*);
extern void* cFEStateMainMenu_vtbl[];

/**
 * Constructs the front end's main menu state.
 *
 * Runs the cUIState constructor (cUIState_cUIState; the engine reaches it in the second argument register, which this
 * definition does not declare), sets mHashName to 2 and the vtable cFEStateMainMenu_vtbl, and clears +0x48 and +0x4C.
 *
 * @return @p self
 */
extern "C" void* cFEStateMainMenu_cFEStateMainMenu1(cFEStateMainMenu* self)
{
    cUIState_cUIState(self);
    self->mHashName = 2;
    self->mVtbl = cFEStateMainMenu_vtbl;
    self->mGoToNextState = 0;
    self->mpSelectMultiModeWidget = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onCreateScreen);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void MEM_fill(void* p, int v, int n);
extern "C" void func_00266CD8();
extern "C" void func_00261008();
extern "C" void func_0025B688();
extern "C" void cGameComm_destroy();
extern "C" void cCommSystem_destroy();
extern char str_07main_men[];


/**
 * Builds the main menu ("07main_men"), shuts the online layer down and counts the connected controllers.
 *
 * Clears the option block cBERules_mRules, the choice flag (+0x48) and the Circle counter (+0x54); destroys the
 * voice object, BuddyMan, LobbyMan, cGameComm and cCommSystem (func_00266CD8, func_00261008, func_0025B688,
 * cGameComm_destroy, cCommSystem_destroy); clears bIsNetworkGame and calls the network interface's slot 1; sets the FE
 * music state 1; stores the number of active pads (UI input slots 44 / 45) in +0x55.
 */
extern "C" void cFEStateMainMenu_onCreateScreen(cFEStateMainMenu* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_07main_men), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    MEM_fill(((char*)&cBERules_mRules), 0, 0x20);
    self->mGoToNextState = 0;
    self->mHackCheatTotal = 0;
    func_00266CD8();
    func_00261008();
    func_0025B688();
    cGameComm_destroy();
    cCommSystem_destroy();
    cBEInterface* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.bIsNetworkGame = 0;
    iface->submit();
    func_0028F140(cAudioInterface_bxAudio(), 1);
    char* obj = *(char**)&self->mpEngine->mpInput;
    *(unsigned char*)&self->mNumOfCtrl = 0;
    for (int i = 0; i < 2; i++) {
        sVEntry<void(void*, unsigned char)>* vt1 = *(sVEntry<void(void*, unsigned char)>**)(obj + 8);
        vt1[44].fn(vthis(obj, vt1[44]), i);
        sVEntry<int(void*)>* vt2 = *(sVEntry<int(void*)>**)(obj + 8);
        if (vt2[45].fn(vthis(obj, vt2[45])) != 0) {
            (*(unsigned char*)&self->mNumOfCtrl)++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onPreRender2D);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern int EnableOnline;
extern char str_3multi_play[];
extern char str_5online[];


class cUIObj_194BF0 {
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
    virtual void setColor(const tARGB& c);
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

/**
 * Greys out Multi Play without two controllers and Online without online support, every frame.
 *
 * "3multi play" is greyed out (DisableTextWidget) with fewer than two active pads (+0x55); otherwise, while the
 * state's mode is 5, it is restored: slot 25 (cUIText_setUseStaticColour) and slot 8 (cUIThing_setStatic) cleared, alpha 1. "5online" is greyed out while EnableOnline (set by initOnline) is 0.
 */
extern "C" void cFEStateMainMenu_onPreRender2D(cFEStateMainMenu* self)
{
    cUIObj_194BF0* obj = (cUIObj_194BF0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_3multi_play));
    if (obj != 0) {
        if (*(signed char*)&self->mNumOfCtrl < 2) {
            DisableTextWidget(obj);
        } else if ((*(unsigned char*)((char*)self + 0x1D) & 0x3F) == 5) {
            obj->setUseStaticColour(0);
            obj->setStatic(0);
            tARGB color = *(tARGB*)((char*)obj + 0x1C);
            color.A = 1.0f;
            obj->setColor(color);
        }
    }
    if (EnableOnline == 0) {
        void* uiObj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_5online));
        if (uiObj != 0) {
            DisableTextWidget(uiObj);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_1conquer[];
extern char str_2quick_play[];
extern char str_3multi_play[];
extern char str_4bonus_material[];
extern char str_5online[];

/**
 * Tags the main-menu items with the choice ids that the event handler and the front end switch on.
 *
 * "1conquer" 11, "2quick play" 9, "3multi play" 22, "4bonus material" 5, "5online" 49 (miscData).
 *
 * @param item  the widget being created
 */
extern "C" void cFEStateMainMenu_onWidgetCreate(cFEStateMainMenu* self, cUIThing* item)
{
    int id;
    id = item->mHashName;
    if (id == GetHashValue32(str_1conquer)) {
        item->mMiscData = 11;
        return;
    }
    id = item->mHashName;
    if (id == GetHashValue32(str_2quick_play)) {
        item->mMiscData = 9;
        return;
    }
    id = item->mHashName;
    if (id == GetHashValue32(str_3multi_play)) {
        item->mMiscData = 22;
        return;
    }
    id = item->mHashName;
    if (id == GetHashValue32(str_4bonus_material)) {
        item->mMiscData = 5;
        return;
    }
    id = item->mHashName;
    if (id == GetHashValue32(str_5online)) {
        item->mMiscData = 49;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onUpdate);
#ifdef SKIP_ASM
extern "C" void cUIState_onUpdate(void* self);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, int index);
extern char str_Menu[];

class cProfileMan_194DD0 {
public:
    int pad[2];
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
    virtual void select(unsigned char i);
    virtual int isValid();
};

/**
 * Recounts the connected controllers and moves the selection off Multi Play when fewer than two remain.
 *
 * After the UI update, the UI input (engine +0xC, slot 44 selects pad i, slot 45 cFEInput_isActive) counts
 * active pads into +0x55; if "Menu" has row 2 selected and fewer than 2 are active, row 1 is selected. The
 * local class name cProfileMan_194DD0 is misleading.
 */
extern "C" void cFEStateMainMenu_onUpdate(cFEStateMainMenu* self)
{
    int i = 0;
    cUIState_onUpdate(self);
    cProfileMan_194DD0* pm = *(cProfileMan_194DD0**)&self->mpEngine->mpInput;
    *(signed char*)&self->mNumOfCtrl = 0;
    for (; i < 2; i++) {
        pm->select(i);
        if (pm->isValid()) {
            (*(signed char*)&self->mNumOfCtrl)++;
        }
    }
    void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu));
    if (menu != 0) {
        if (*(unsigned char*)((char*)menu + 0x95) == 2) {
            if (*(signed char*)&self->mNumOfCtrl < 2) {
                cUIMenu_setSelectedByIndex(menu, 1);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
extern "C" void func_001A86E8(int a);
extern "C" void cBENewRaceInterface_setGamePath(void* iface, int mode);
extern "C" void cCommSystem_construct();
extern "C" void func_0025B650();
extern "C" void func_00260F80();
extern "C" void func_00266CA0();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as malloc-like (no aliasing), as the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern "C" void* func_001887A0(void* mem, void* engine, int a2);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern void* cFEPopupSelectMultiplayerMode_vtbl[];
extern char str_cFEPopupSelectMultiplayerMode[];
extern char str_FEOptions[];
extern char str_4online[];
extern char str_3multi_play[];
extern char str_2quick_play[];
extern char str_3_help_multi[];
extern char str_btext5[];
extern char str_ps2sq[];
extern char str_hl[];



static inline void setWidgetHidden_194EB0(char* self, char* name)
{
    cUIThing* o = cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(name));
    if (o != 0) {
        o->setVisible(0);
    }
}

static inline void setWidgetVisible_194EB0(char* self, char* name)
{
    cUIThing* o = cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(name));
    if (o != 0) {
        o->setStatic(0);
    }
}

/**
 * Handles a main-menu choice: sets the game path for the chosen mode and pushes the next front-end state.
 *
 * Cross (5): func_001A86E8(0), then by the item's miscData: 11 Conquer path 0, 9 Quick Play path 1, 22 Multi
 * Play path 2, 49 Online path 2 plus the online singletons (cCommSystem_construct ...), 5 Bonus Material;
 * each sets the choice flag (+0x48). After the race interface's slot 1 it pushes the custom interface's
 * getNextState(miscData). The Select Multiplayer Mode popup branch needs +0x48 clear for item 22, which case
 * 22 has just set, so it never runs.
 * Triangle (6) pushes getPrevState(); Square (7) pushes a new "FEOptions" state (func_001887A0); each Circle
 * (8) counts in +0x54 and from the 7th on clears the static flag of "2quick play", "3multi play" and
 * "4online".
 *
 * @param self_  the main-menu state (this)
 * @param item_  the item (On Tour pWidget); ignored when null
 * @param type  the widget event: 5 Cross, 6 Triangle, 7 Square, 8 Circle
 */
extern "C" void cFEStateMainMenu_onWidgetEvent(cFEStateMainMenu* self_, cUIThing* item_, unsigned int type)
{
    if (((char*)item_) == 0)
        return;
    switch (type) {
    case 5: {
        func_001A86E8(0);
        cBEInterface* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (item_->mMiscData) {
        case 11:
            cBENewRaceInterface_setGamePath(iface, 0);
            self_->mGoToNextState = 1;
            break;
        case 9:
            cBENewRaceInterface_setGamePath(iface, 1);
            self_->mGoToNextState = 1;
            // both arms set the flag; the leftover compare on the cheat counter is in the target too
            if (*(signed char*)&self_->mHackCheatTotal >= 7)
                self_->mGoToNextState = 1;
            break;
        case 22:
            cBENewRaceInterface_setGamePath(iface, 2);
            self_->mGoToNextState = 1;
            break;
        case 49:
            cBENewRaceInterface_setGamePath(iface, 2);
            cCommSystem_construct();
            func_0025B650();
            func_00260F80();
            func_00266CA0();
        case 5:
            self_->mGoToNextState = 1;
            break;
        }
        iface->submit();
        if (item_->mMiscData == 22 && self_->mGoToNextState == 0) {
            self_->mSaveCurrentCtrl = self_->mCurrentCtrl;
            cFEPopupSelectMultiplayerMode* popup = (cFEPopupSelectMultiplayerMode*)((char*)operator new(76, str_cFEPopupSelectMultiplayerMode, 0, 0));
            cUIState_cUIState1(popup, self_->mpEngine, ((char*)self_));
            popup->mVtbl = cFEPopupSelectMultiplayerMode_vtbl;
            int bit = 1;
            bit <<= self_->mSaveCurrentCtrl;
            *(char*)&popup->mCtrlBits = bit;
            popup->mMultiMode = 0;
            cUIStateStack_pushPopup(&self_->mpEngine->mStateStack, popup);
            self_->mpSelectMultiModeWidget = (cUIThing*)item_;
            setWidgetHidden_194EB0(((char*)self_), str_hl);
            setWidgetHidden_194EB0(((char*)self_), str_3_help_multi);
            setWidgetHidden_194EB0(((char*)self_), str_btext5);
            setWidgetHidden_194EB0(((char*)self_), str_ps2sq);
            return;
        }
        cUICustomInterface* obj = *(cUICustomInterface**)self_->mpEngine;
        void* nextState = obj->getNextState((cUIState*)self_, item_->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&self_->mpEngine->mStateStack, nextState);
        break;
    }
    case 6: {
        cUICustomInterface* obj = *(cUICustomInterface**)self_->mpEngine;
        void* nextState = obj->getPrevState((cUIState*)self_, item_->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&self_->mpEngine->mStateStack, nextState);
        break;
    }
    case 7: {
        char* optionsState = (char*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), self_->mpEngine, 1);
        *(int*)(optionsState + 0x1C) = (*(int*)(optionsState + 0x1C) & ~0x3F00) | 0x100;
        cUIStateStack_pushSpecial(&self_->mpEngine->mStateStack, optionsState, 0, 0);
        break;
    }
    case 8:
        if (++*(signed char*)&self_->mHackCheatTotal >= 7) {
            setWidgetVisible_194EB0(((char*)self_), str_2quick_play);
            setWidgetVisible_194EB0(((char*)self_), str_3multi_play);
            setWidgetVisible_194EB0(((char*)self_), str_4online);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Picks the main menu's button feedback: Cross skips the animation until a choice is made, Circle animates
 * silently.
 *
 * @param pMenu  the menu (On Tour pMenu); unused
 * @param Event  the menu event (6 Cross, 9 Circle)
 * @return for event 6, 1 (sound only) while the choice flag +0x48 is 0, else 0x101; 0x100 for event 9; 0x101
 *         otherwise
 */
extern "C" int cFEStateMainMenu_onWidgetMenu(cFEStateMainMenu* self, int pMenu, int Event)
{
    switch (Event) {
    case 6:
        return self->mGoToNextState == 0 ? 1 : 257;
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int cUIThing_playKeyframerEvent(void* self, int x);
extern char str_hl[];
extern char str_3_help_multi[];
extern char str_btext5[];
extern char str_ps2sq[];



static inline void Show195328(void* self, char* name)
{
    cUIThing* o = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (o != 0) {
        o->setVisible(1);
    }
}

/**
 * Handles the Select Multiplayer Mode popup closing: on a result, replays Cross on the remembered Multi Play
 * item.
 *
 * Only event 0x16 acts. If the popup's getData(0) (slot 23) is nonzero it sets the choice flag (+0x48),
 * restores mCurrentCtrl from +0x50, calls cUIThing_playKeyframerEvent(item, 2) and its own onWidgetEvent (slot 19) with the
 * item (+0x4C) and 5. Then "hl", "3 help multi", "btext5" and "ps2sq" are shown again.
 *
 * @param obj  the popup state (On Tour pPopup)
 * @param msg  the popup event (0x15, 0x16)
 */
// On confirm (msg 0x16) commits the selected entry, then re-shows the four main-menu buttons.
extern "C" void cFEStateMainMenu_onPopupEvent(cFEStateMainMenu* self, cUIState* obj, int msg)
{
    if (msg == 21) {
        return;
    }
    if (msg != 22) {
        return;
    }
    sVEntry<int(void*, int)>* ovt = (sVEntry<int(void*, int)>*)obj->mVtbl;
    if (ovt[23].fn(vthis(obj, ovt[23]), 0) != 0) {
        self->mGoToNextState = 1;
        int savedCtrl = *(signed char*)&self->mSaveCurrentCtrl;
        *(char*)&self->mCurrentCtrl = savedCtrl;
        cUIThing_playKeyframerEvent(self->mpSelectMultiModeWidget, 2);
        sVEntry<void(void*, ...)>* svt = (sVEntry<void(void*, ...)>*)self->mVtbl;
        svt[19].fn(vthis(self, svt[19]), self->mpSelectMultiModeWidget, 5);
    }
    Show195328(self, str_hl);
    Show195328(self, str_3_help_multi);
    Show195328(self, str_btext5);
    Show195328(self, str_ps2sq);
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateBonusMaterial_cFEStateBonusMaterial1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void*);
extern char cFEStateBonusMaterial_vtbl[];

/**
 * Constructs the front end's bonus material state: the cUIState constructor (cUIState_cUIState, with the engine left in the
 * second argument register), the vtable cFEStateBonusMaterial_vtbl and mHashName 5.
 *
 * @return @p self
 */
extern "C" void* cFEStateBonusMaterial_cFEStateBonusMaterial1(cFEStateBonusMaterial* self)
{
    cUIState_cUIState(self);
    self->mVtbl = (void**)cFEStateBonusMaterial_vtbl;
    self->mHashName = 5;
    return ((void*)self);
}
#endif

