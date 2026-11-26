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
#include "input/input.h"
#include "ui/uiinput.h"
#include "ui/uimenu.h"
#include "be/beintnewrace.h"

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
void cFEStateTitle::onCreateScreen()
{
    void* engine;
    void* screen;
    mNumTicksIdle = 0;
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_06title), 0);
    mpScreen = (cUIScreen*)screen;
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

/**
 * Holds the title's first appearance for 300 calls, then focuses its menu and shows "tReal".
 *
 * The countdown gTitleScreenDelay is global and stays 0 afterwards, so later visits finish at once. "Menu0000" gets
 * setFocus(1) (slot 7) and "tReal" (the Press START text) becomes visible.
 *
 * @return true when the transition is done, false while waiting
 */
extern "C" bool cFEStateTitle_onGainTransition(cFEStateTitle* self)
{
    if (gTitleScreenDelay == 0 || --gTitleScreenDelay == 0) {
        cUIThing* uiObj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Menu0000));
        if (uiObj != 0) {
            uiObj->setFocus(1);
        }
        uiObj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_tReal));
        if (uiObj != 0) {
            uiObj->setVisible(1);
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/ovstates", cFEStateTitle_onUpdate);
#ifdef SKIP_ASM
extern "C" void cUIState_onUpdate(void* self);
extern int cRealFE_mMovieList;


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
 * After the UI update (cUIState_onUpdate), a held button on either controller's pad (SSXApp->Inputs[i]->Pad) resets
 * the timer (mNumTicksIdle). Past 1800 frames (30 s at 60 Hz) the timer restarts and bit 0x4 of cRealFE_mMovieList
 * is set, which cRealFE_update plays as a skippable movie.
 */
void cFEStateTitle::onUpdate()
{
    cUIState_onUpdate((this));
    mNumTicksIdle += 1;
    if (mNumTicksIdle > 1800) {
        mNumTicksIdle = 0;
        cRealFE_mMovieList |= 4;
        return;
    }
    int found = 0;
    int i;
    for (i = 0; i < 2; i++) {
        cInputPad* list = SSXApp->Inputs[i]->Pad;
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
                mNumTicksIdle = 0;
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
void cFEStateTitle_onPopupEvent(void* vself)
{
    cFEStateTitle* self = (cFEStateTitle*)vself;
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
void cFEStateTitle::onWidgetCreate(cUIThing* item)
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
 * @param type  the widget event; only kONNEXT acts
 */
extern "C" void cFEStateTitle_onWidgetEvent(cFEStateTitle* self, cUIThing* msg, int type)
{
    if (type == cUIState::kONNEXT) {
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
 * @return 0x101 (animation and sound) for kONNEXT, else 0x100 (animation only)
 */
extern "C" int cFEStateTitle_onWidgetMenu(cFEStateTitle* self, int pMenu, int Event)
{
    return Event != cUIMenu::kONNEXT ? 0x100 : 257;
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
void* cFEStateMainMenu::cFEStateMainMenu1()
{
    cUIState_cUIState(this);
    mHashName = 2;
    VPTR_OF(cListNode, this) = cFEStateMainMenu_vtbl;
    mGoToNextState = 0;
    mpSelectMultiModeWidget = 0;
    return ((void*)this);
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
 * Clears the option block cBERules_mRules, the choice flag (mGoToNextState) and the Circle counter (mHackCheatTotal);
 * destroys the voice object, BuddyMan, LobbyMan, cGameComm and cCommSystem (func_00266CD8, func_00261008,
 * func_0025B688, cGameComm_destroy, cCommSystem_destroy); clears bIsNetworkGame and calls the network interface's slot
 * 1; sets the FE music state 1; stores the number of active pads (the UI input's setPadNum / isActive, slots 44 / 45)
 * in mNumOfCtrl.
 */
void cFEStateMainMenu::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_07main_men), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    MEM_fill(((char*)&cBERules_mRules), 0, 0x20);
    mGoToNextState = 0;
    mHackCheatTotal = 0;
    func_00266CD8();
    func_00261008();
    func_0025B688();
    cGameComm_destroy();
    cCommSystem_destroy();
    cBEInterface* iface = cBE_getInterface_Fv(SSXApp->BE, 7);
    cBENetworkInterface_networkOpt.bIsNetworkGame = 0;
    iface->submit();
    func_0028F140(cAudioInterface_bxAudio(), 1);
    cUIInput* input = mpEngine->mpInput;
    mNumOfCtrl = 0;
    for (int i = 0; i < 2; i++) {
        input->setPadNum((unsigned char)i); // the original passes the pad number as a byte
        if (input->isActive()) {
            (*(unsigned char*)&mNumOfCtrl)++;
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
void cFEStateMainMenu::onPreRender2D()
{
    cUIObj_194BF0* obj = (cUIObj_194BF0*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_3multi_play));
    if (obj != 0) {
        if (mNumOfCtrl < 2) {
            DisableTextWidget(obj);
        } else if ((*(unsigned char*)((char*)this + 0x1D) & 0x3F) == 5) {
            obj->setUseStaticColour(0);
            obj->setStatic(0);
            tARGB color = *(tARGB*)((char*)obj + 0x1C);
            color.A = 1.0f;
            obj->setColor(color);
        }
    }
    if (EnableOnline == 0) {
        void* uiObj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_5online));
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
void cFEStateMainMenu::onWidgetCreate(cUIThing* item)
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

/**
 * Recounts the connected controllers and moves the selection off Multi Play when fewer than two remain.
 *
 * After the UI update, the engine's UI input (mpInput: setPadNum selects pad i, isActive, slots 44 / 45) counts
 * active pads into mNumOfCtrl; if "Menu" has row 2 selected (mScrollItemCurrent) and fewer than 2 are active, row 1
 * is selected.
 */
void cFEStateMainMenu::onUpdate()
{
    int i = 0;
    cUIState_onUpdate(this);
    cUIInput* input = mpEngine->mpInput;
    mNumOfCtrl = 0;
    for (; i < 2; i++) {
        input->setPadNum((unsigned char)i); // the original passes the pad number as a byte
        if (input->isActive()) {
            (mNumOfCtrl)++;
        }
    }
    cUIMenu* menu = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Menu));
    if (menu != 0) {
        if (menu->mScrollItemCurrent == 2) {
            if (mNumOfCtrl < 2) {
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
 * Cross (kONNEXT): func_001A86E8(0), then by the item's miscData: 11 Conquer kGAMEPATH_WORLDCIRCUIT, 9 Quick Play
 * kGAMEPATH_SINGLEEVENT, 22 Multi Play kGAMEPATH_MULTIPLAYER, 49 Online kGAMEPATH_MULTIPLAYER plus the online
 * singletons (cCommSystem_construct ...), 5 Bonus Material; each sets the choice flag (+0x48). After the race
 * interface's slot 1 it pushes the custom interface's getNextState(miscData). The Select Multiplayer Mode popup branch
 * needs +0x48 clear for item 22, which case 22 has just set, so it never runs. Triangle (kONPREV) pushes
 * getPrevState(); Square (kONOPTION) pushes a new "FEOptions" state (func_001887A0); each Circle (kONMISC) counts in
 * +0x54 and from the 7th on clears the static flag of "2quick play", "3multi play" and "4online".
 *
 * @param item_  the item (On Tour pWidget); ignored when null
 * @param type  the widget event: kONNEXT Cross, kONPREV Triangle, kONOPTION Square, kONMISC Circle
 */
void cFEStateMainMenu::onWidgetEvent(cUIThing* item_, unsigned int type)
{
    if (((char*)item_) == 0)
        return;
    switch (type) {
    case cUIState::kONNEXT: {
        func_001A86E8(0);
        cBEInterface* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (item_->mMiscData) {
        case 11:
            cBENewRaceInterface_setGamePath(iface, kGAMEPATH_WORLDCIRCUIT);
            mGoToNextState = 1;
            break;
        case 9:
            cBENewRaceInterface_setGamePath(iface, kGAMEPATH_SINGLEEVENT);
            mGoToNextState = 1;
            // both arms set the flag; the leftover compare on the cheat counter is in the target too
            if (mHackCheatTotal >= 7)
                mGoToNextState = 1;
            break;
        case 22:
            cBENewRaceInterface_setGamePath(iface, kGAMEPATH_MULTIPLAYER);
            mGoToNextState = 1;
            break;
        case 49:
            cBENewRaceInterface_setGamePath(iface, kGAMEPATH_MULTIPLAYER);
            cCommSystem_construct();
            func_0025B650();
            func_00260F80();
            func_00266CA0();
        case 5:
            mGoToNextState = 1;
            break;
        }
        iface->submit();
        if (item_->mMiscData == 22 && mGoToNextState == 0) {
            mSaveCurrentCtrl = mCurrentCtrl;
            cFEPopupSelectMultiplayerMode* popup = (cFEPopupSelectMultiplayerMode*)((char*)operator new(76, str_cFEPopupSelectMultiplayerMode, 0, 0));
            cUIState_cUIState1(popup, mpEngine, ((char*)this));
            VPTR_OF(cListNode, popup) = cFEPopupSelectMultiplayerMode_vtbl;
            int bit = 1;
            bit <<= mSaveCurrentCtrl;
            popup->mCtrlBits = bit;
            popup->mMultiMode = 0;
            cUIStateStack_pushPopup(&mpEngine->mStateStack, popup);
            mpSelectMultiModeWidget = (cUIThing*)item_;
            setWidgetHidden_194EB0(((char*)this), str_hl);
            setWidgetHidden_194EB0(((char*)this), str_3_help_multi);
            setWidgetHidden_194EB0(((char*)this), str_btext5);
            setWidgetHidden_194EB0(((char*)this), str_ps2sq);
            return;
        }
        cUICustomInterface* obj = *(cUICustomInterface**)mpEngine;
        void* nextState = obj->getNextState((cUIState*)this, item_->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        break;
    }
    case cUIState::kONPREV: {
        cUICustomInterface* obj = *(cUICustomInterface**)mpEngine;
        void* nextState = obj->getPrevState((cUIState*)this, item_->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        break;
    }
    case cUIState::kONOPTION: {
        cUIState* optionsState =
            (cUIState*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), mpEngine, 1);
        optionsState->mAttrState = (optionsState->mAttrState & ~0x3F00) | 0x100; // tAttrState::mMode = 1
        cUIStateStack_pushSpecial(&mpEngine->mStateStack, optionsState, 0, 0);
        break;
    }
    case cUIState::kONMISC:
        if (++mHackCheatTotal >= 7) {
            setWidgetVisible_194EB0(((char*)this), str_2quick_play);
            setWidgetVisible_194EB0(((char*)this), str_3multi_play);
            setWidgetVisible_194EB0(((char*)this), str_4online);
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
 * @return for kONNEXT, 1 (sound only) while the choice flag +0x48 is 0, else 0x101; 0x100 for kONMISC; 0x101
 *         otherwise
 */
extern "C" int cFEStateMainMenu_onWidgetMenu(cFEStateMainMenu* self, int pMenu, int Event)
{
    switch (Event) {
    case cUIMenu::kONNEXT:
        return self->mGoToNextState == 0 ? 1 : 257;
    case cUIMenu::kONMISC:
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
 * Only kONPOPUPDESTROY acts. If the popup's getData(0) (slot 23) is nonzero it sets the choice flag (+0x48),
 * restores mCurrentCtrl from +0x50, calls cUIThing_playKeyframerEvent(item, 2) and its own onWidgetEvent (slot 19) with the
 * item (+0x4C) and 5. Then "hl", "3 help multi", "btext5" and "ps2sq" are shown again.
 *
 * @param obj  the popup state (On Tour pPopup)
 * @param msg  the popup event (0x15, 0x16)
 */
// On confirm (msg 0x16) commits the selected entry, then re-shows the four main-menu buttons.
extern "C" void cFEStateMainMenu_onPopupEvent(cFEStateMainMenu* self, cUIState* obj, int msg)
{
    if (msg == cUIState::kONPOPUPCREATE) {
        return;
    }
    if (msg != cUIState::kONPOPUPDESTROY) {
        return;
    }
    if ((int)obj->getData(0) != 0) {
        self->mGoToNextState = 1;
        int savedCtrl = *(signed char*)&self->mSaveCurrentCtrl;
        self->mCurrentCtrl = savedCtrl;
        cUIThing_playKeyframerEvent(self->mpSelectMultiModeWidget, 2);
        self->onWidgetEvent(self->mpSelectMultiModeWidget, 5);
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
void* cFEStateBonusMaterial::cFEStateBonusMaterial1()
{
    cUIState_cUIState(this);
    VPTR_OF(cListNode, this) = (void**)cFEStateBonusMaterial_vtbl;
    mHashName = 5;
    return ((void*)this);
}
#endif

