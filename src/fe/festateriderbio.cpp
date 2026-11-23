#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beintplayer.h"
#include "fe/festateoptions.h"
#include "ui/uiengine.h"
#include "fe/ovstatehud.h"
#include "sound/ssxaudio.h"
#include "fe/fereal.h"
#include "fe/keyboardpopup.h"
#include "fe/festatecharselect.h"
#include "fe/festateriderdetail.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "fe/festateubertrick.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateCharSetup_updateWidget(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void cFERiderManager_setHumanVisible(void* self, int a1, int a2);
extern "C" void cFERiderManager_setHumanCharacter(void* self, int idx, int a2, int a3);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);
extern "C" int cBERewardInterface_getNumOwnedCheatChars(void* self, int a1, int a2);
extern char str_154rider_details[];


/**
 * Creates the rider-details screen "154rider_details" and shows the player's character.
 *
 * Runs cFEStateCharSetup_updateWidget, reloads the player table (the interface's slot 2, discard), sets up the user's
 * front-end rider with the player's character, calls func_0028F140 with 1, and sets +0x50 when the character
 * has any cheat characters unlocked (cBERewardInterface_getNumOwnedCheatChars).
 */
extern "C" void cFEStateRiderDetail_onCreateScreen(cFEStateRiderDetail* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_154rider_details), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateCharSetup_updateWidget(self);
    cBENewPlayerInterface* pi = (cBENewPlayerInterface*)cBE_getInterface_Fv(cBE_getBE(), 1);
    int charID = cBENewPlayerInterface_getPlayerCharID(pi, self->mUserNum);
    pi->discard();
    cFERiderManager_setHumanVisible(&SSXApp->RealFE->mRiderManager, self->mUserNum, 0);
    cFERiderManager_setHumanCharacter(&SSXApp->RealFE->mRiderManager, self->mUserNum, charID, 0);
    func_0028F140(cAudioInterface_bxAudio(), 1);
    self->mbCheatCharEnable = 1;
    if (cBERewardInterface_getNumOwnedCheatChars(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum, charID) == 0) {
        self->mbCheatCharEnable = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onGainFocus);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEState_onGainFocus(void* self, int a1);
extern char str_Option_1[];
extern char str_Option_2[];
extern char str_Option_3[];
extern char str_Option_4[];
extern char str_Option_5[];

static inline void showObj_001833D0(void* self, char* name, int id)
{
    cUIThing* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (obj != 0) {
        obj->setVisible(1);
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
extern "C" void cFEStateRiderDetail_onGainFocus(cFEStateRiderDetail* self, int pPrevState)
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
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern char str_Option_2[];

/**
 * Greys out "Option_2" (the cheat characters) when the character has none unlocked (+0x50 is 0).
 *
 * DisableTextWidget disables the widget and caps its alpha at 0.5.
 */
extern "C" void cFEStateRiderDetail_onPreRender2D(cFEStateRiderDetail* self)
{
    if (self->mbCheatCharEnable == 0) {
        void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Option_2));
        if (obj != 0) {
            DisableTextWidget(obj);
        }
    }
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Renders the rider-detail state's 2D layer with the shared UI-state render routine cUIState_onRender2D().
 *
 * @return no meaningful value (cUIState_onRender2D() returns nothing)
 */
void* cFEStateRiderDetail_onRender2D(void* self)
{
    return cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onUpdate);
#ifdef SKIP_ASM
extern "C" void cUIState_onUpdate(void*);
extern "C" void cFEStateCharSetup_updateCharacter3DPos(void*);

/**
 * Updates the rider-detail state: runs the base UI-state update, then poses the front-end rider model.
 *
 * cUIState_onUpdate() is the base UI-state update, which dispatches controller input to the state and its screens.
 * cFEStateCharSetup_updateCharacter3DPos() (shared with the character-setup state) places the loaded rider model and, when the state asks
 * for it, restarts its animation.
 */
extern "C" void cFEStateRiderDetail_onUpdate(cFEStateRiderDetail* self)
{
    cUIState_onUpdate(self);
    cFEStateCharSetup_updateCharacter3DPos(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setPlayerName(void* self, int a1, const char* name);
extern "C" void cBENewPlayerInterface_setPlayerCheatCharID(void* self, int a1, int a2);
extern "C" void cFEStateCharSetup_show3DCharacter(void* self, int a1);
extern "C" unsigned int strlen(const char* s);
extern char str_131cheat_char[];


static inline int Is_001835F8(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Takes the result of the rider-details popups when one closes (event 0x16): a cheat character or a name.
 *
 * From "131cheat_char" a confirmed choice (+0x74) is stored in the player table (cBENewPlayerInterface_setPlayerCheatCharID) and
 * committed; from the keyboard (hash name 0) an accepted, non-empty text becomes the player's name
 * (cBENewPlayerInterface_setPlayerName). Then cFEStateCharSetup_show3DCharacter with 1.
 *
 * @param item  the popup that sent the event
 * @param msg  the popup event; only 0x16 is handled
 */
extern "C" void cFEStateRiderDetail_onPopupEvent(cFEStateRiderDetail* self, cUIState* item, int msg)
{
    cBEInterface* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (msg == 22) {
        if (Is_001835F8(item->mHashName, str_131cheat_char)) {
            int cheatCharId = ((cFEStateCheatCharSelect*)item)->mCheatCharID;
            if (((cFEStateCheatCharSelect*)item)->mDone != 0) {
                cBEInterface* pi2 = cBE_getInterface_Fv(cBE_getBE(), 1);
                cBENewPlayerInterface_setPlayerCheatCharID(pi2, self->mUserNum, cheatCharId);
                pi2->submit();
            }
        } else if (item->mHashName == 0) {
            char* name = ((cKeyboardPopup*)item)->mszEditString;
            if (((cKeyboardPopup*)item)->mbAcceptText != 0 && strlen(name) != 0) {
                cBENewPlayerInterface_setPlayerName(pi, self->mUserNum, name);
                pi->submit();
            }
        }
        cFEStateCharSetup_show3DCharacter(self, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateRiderDetail_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cUIStateStack_pushSpecial(void* stack, void* state, int a2, int a3);
extern "C" char* cBENewPlayerInterface_getPlayerName(void* iface, int rider);
extern "C" int cBERewardInterface_getNumOwnedCheatChars(void* self, int a1, int a2);
extern "C" void* cFEStateCheatCharSelect_cFEStateCheatCharSelect(void* mem, void* engine, void* owner, int rider, int cid);
extern "C" void cFEStateCharSetup_show3DCharacter(void* self, int a1);
extern "C" void* func_001887A0(void* mem, void* engine, int a2);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* self, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cKeyboardPopup_setInputText(void* self, const char* str);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int v);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_setDisableShiftNumbers(void* self, int v);
extern "C" void cUIStateStack_pushPopup(void* stack, void* state);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern const char str_cKeyboardPopup[];
extern const char str_cFEStateCheatCharSelect[];
extern const char str_FEOptions[];

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
extern "C" void cFEStateRiderDetail_onWidgetEvent(cFEStateRiderDetail* self, cUIThing* item, unsigned int msg)
{
    if (((char*)item) == 0) return;
    switch (msg) {
    case 5: {
        int sub = item->mMiscData;
        if (sub == 8) {
            char* w = (char*)cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(1092, str_cKeyboardPopup, 0x100, 0), self->mpEngine, ((char*)self), 0, 0xF);
            *(int*)(w + 0x140) = 55;
            cKeyboardPopup_setMaxCharacters(w, 8);
            cKeyboardPopup_setIncludeKey(w, 75, 1, 2);
            *(int*)(w + 0x58) = 1;
            cKeyboardPopup_setDisableShiftNumbers(w, 1);
            cKeyboardPopup_setIncludeKey(w, 0xE, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 39, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 40, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 52, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 26, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 25, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 53, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 0x40, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 65, 0, 2);
            cKeyboardPopup_setIncludeKey(w, 66, 0, 2);
            cUIStateStack_pushPopup(&self->mpEngine->mStateStack, w);
            cKeyboardPopup_setInputText(w, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum));
            cFEStateCharSetup_show3DCharacter(((char*)self), 0);
        } else if (sub == 6) {
            int cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
            if (cBERewardInterface_getNumOwnedCheatChars(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum, cid) > 0) {
                char* w = (char*)cFEStateCheatCharSelect_cFEStateCheatCharSelect(cMemMan_alloc(484, str_cFEStateCheatCharSelect, 0, 0), self->mpEngine, ((char*)self), self->mUserNum, cid);
                *(unsigned char*)(w + 0x15) = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, self->mUserNum);
                cUIStateStack_pushPopup(&self->mpEngine->mStateStack, w);
                cFEStateCharSetup_show3DCharacter(((char*)self), 0);
            }
        } else {
            cUICustomInterface* obj = self->mpEngine->mpCustom;
            void* nextState = obj->getNextState((cUIState*)self, sub);
            if (nextState != 0) {
                cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
            }
        }
        cFEStateCharSetup_show3DCharacter(((char*)self), 0);
        break;
    }
    case 6: {
        cUICustomInterface* obj = self->mpEngine->mpCustom;
        void* nextState = obj->getPrevState((cUIState*)self, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
            cFEStateCharSetup_show3DCharacter(((char*)self), 0);
        }
        break;
    }
    case 7: {
        char* w = (char*)func_001887A0(cMemMan_alloc(84, str_FEOptions, 0x100, 0), self->mpEngine, 1);
        ((sBits_3710*)(w + 0x1C))->mode = 1;
        cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, w, 0, 0);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateUberTrick_cFEStateUberTrick);
#ifdef SKIP_ASM
extern void* cFEStateUberTrick_vtbl[];
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);

/**
 * Constructs the uber-trick store state for a user (allocated as "cFEStateUberTrick", 0x60 bytes).
 *
 * Runs cUIState's constructor (cUIState_cUIState; @p engine reaches it untouched in the second argument register as the
 * engine), stores the vtable cFEStateUberTrick_vtbl and mHashName 18, sets mUserNum to @p userNum and mCtrlBits to that user's
 * controller bits (cFECustom_getPlayerCtrlMask on the engine's front end), and clears the two menu pointers +0x4C and +0x50 (On Tour
 * mpMainMenu, mpTrickMenu) and the rider-shown flag +0x48 (On Tour mbModelVisible).
 *
 * @param engine  the UI engine (only passed on to cUIState's constructor)
 * @param userNum  the user number
 * @return @p self
 */
extern "C" void* cFEStateUberTrick_cFEStateUberTrick(cFEStateUberTrick* self, int engine, int userNum)
{
    signed char idx = userNum;
    cUIState_cUIState(self);
    VPTR_OF(cListNode, self) = cFEStateUberTrick_vtbl;
    self->mHashName = 18;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, idx);
    self->mpMainMenu = 0;
    self->mpTrickMenu = 0;
    self->mbModelVisible = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateUberTrick_activateMainMenu);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
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
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Switches the uber-trick screen's highlight and help text to the category menu.
 *
 * On mpScreen (+0x40) it hides "hilite_right" and "helptext_custom" and shows "hilite_left" and "helptext_choose"
 * (setVisible, vtable slot 9); objects that are missing are skipped. cFEStateUberTrick_activateTrickMenu does the opposite. Called when the
 * state gains focus (cFEStateUberTrick_onGainFocus) and when the trick menu is left (event 6 in cFEStateUberTrick_onWidgetEvent).
 */
extern "C" void cFEStateUberTrick_activateMainMenu(cFEStateUberTrick* self)
{
    cUIObj_183B08* hiliteRight = (cUIObj_183B08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_hilite_right));
    if (hiliteRight != 0) {
        hiliteRight->setVisible(0);
    }
    cUIObj_183B08* hiliteLeft = (cUIObj_183B08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_hilite_left));
    if (hiliteLeft != 0) {
        hiliteLeft->setVisible(1);
    }
    cUIObj_183B08* helpChoose = (cUIObj_183B08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
    if (helpChoose != 0) {
        helpChoose->setVisible(1);
    }
    cUIObj_183B08* helpCustom = (cUIObj_183B08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_custom));
    if (helpCustom != 0) {
        helpCustom->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", cFEStateUberTrick_activateTrickMenu);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
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
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

/**
 * Switches the uber-trick screen's highlight and help text to the trick menu.
 *
 * On mpScreen (+0x40) it shows "hilite_right" and "helptext_custom" and hides "hilite_left" and "helptext_choose"
 * (setVisible, vtable slot 9); objects that are missing are skipped. cFEStateUberTrick_activateMainMenu does the opposite. Called when a
 * category is accepted (event 5 in cFEStateUberTrick_onWidgetEvent).
 */
extern "C" void cFEStateUberTrick_activateTrickMenu(cFEStateUberTrick* self)
{
    cUIObj_183C08* hiliteRight = (cUIObj_183C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_hilite_right));
    if (hiliteRight != 0) {
        hiliteRight->setVisible(1);
    }
    cUIObj_183C08* hiliteLeft = (cUIObj_183C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_hilite_left));
    if (hiliteLeft != 0) {
        hiliteLeft->setVisible(0);
    }
    cUIObj_183C08* helpChoose = (cUIObj_183C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_choose));
    if (helpChoose != 0) {
        helpChoose->setVisible(0);
    }
    cUIObj_183C08* helpCustom = (cUIObj_183C08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_custom));
    if (helpCustom != 0) {
        helpCustom->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateriderbio", func_00183D08);
#ifdef SKIP_ASM

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
    virtual void setVisible(int a);
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
    virtual void getSize(t3Vector* p);
    virtual void setSize(t3Vector* p);
};

/**
 * Sizes a UI object to a texture element and shows it, or hides the object when there is no element.
 *
 * With @p rect it reads the object's size (getSize, vtable slot 20), sets x to (mRight - mLeft) * 256 * @p scale and y
 * to (mBottom - mTop) * 256 * @p scale from the cTextureElement (+0x14, +0x10, +0x18, +0xC), writes the size back
 * (setSize, slot 21) and shows the object (setVisible(1), slot 9). A null @p rect only hides it (setVisible(0)).
 * cFEStateUberTrick_updateTrickMenu uses it for the uber-trick screen's "InstBtn" button icons with scale 0.8.
 *
 * @param rect  the texture element (cTextureElement), or 0
 * @param scale  size factor on top of 256 per texture-coordinate unit
 */
extern "C" void func_00183D08(cWidget_00183D08* self, cTextureElement* rect, float scale)
{
    if (((void*)rect)) {
        t3Vector pos;
        self->getSize(&pos);
        float k = scale * 256.0f;
        pos.x = (rect->mRight - rect->mLeft) * k;
        pos.y = (rect->mBottom - rect->mTop) * k;
        self->setSize(&pos);
        self->setVisible(1);
    } else {
        self->setVisible(0);
    }
}
#endif

