#include "common.h"
#include "fe/keyboardpopup.h"
#include "fe/festatecharselect.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festatelodgeriderdetail.h"
#include "fe/festatelodge.h"
#include "fe/festatebuyattrib.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_28lodge[];
extern int D_00441B14[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getPlayerCheatCharID(void* iface, int player);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_0028F140(void* self, int a1);

/**
 * Opens the lodge screen ("28lodge"), reads the user's cheat character and disables lodge option 7.
 *
 * Keeps the screen at 0x40, stores the user's cheat character id (cBENewPlayerInterface_getPlayerCheatCharID; On Tour mCheatCharID) at
 * 0x48, clears entry 7 of the option-enabled table gLodgeOptionUnlocked (written as D_00441B14[0]), switches the
 * front-end sound-bank mode to 1, clears bToNextState (0x4C) and mbIsYesOrNo (0x54) and sets
 * mPathToSaveProfile (0x50) to 0x12.
 */
extern "C" void cFEStateLodge_onCreateScreen(cFEStateLodge* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_28lodge), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mCheatCharID = cBENewPlayerInterface_getPlayerCheatCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
    D_00441B14[0] = 0;
    func_0028F140(cAudioInterface_bxAudio(), 1);
    self->bToNextState = 0;
    self->mPathToSaveProfile = 18;
    self->mbIsYesOrNo = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getLastHub(void* iface, int a1);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_option_0[];
extern char str_option_1[];
extern char str_option_2[];
extern char str_option_3[];
extern char str_option_4[];
extern char str_option_5[];
extern char str_option_5b[];
extern char str_option_6[];
extern char str_option_7[];
extern char str_title_lodge[];
extern char str_kT_TITLELodgePeak1[];
extern char str_kT_TITLELodgePeak2[];
extern char str_kT_TITLELodgePeak3[];

struct sWVt_3840 { short delta; short index; void (*fn)(void*, int); };

/**
 * Numbers the lodge's option widgets and titles the screen with the current peak.
 *
 * "option_0" to "option_6" get misc data 0-6, except "option_5b" which gets 8; "option_7" gets 7 and is made
 * static and hidden (vtable slots 8 and 9). "title_lodge" shows kT_TITLELodgePeak1 for user 0's last hub
 * 17-18, kT_TITLELodgePeak2 for 19-20 and kT_TITLELodgePeak3 otherwise.
 *
 * @param w  the widget just created (its hash name is at +0x38)
 */
extern "C" void cFEStateLodge_onWidgetCreate(cFEStateLodge* self, cUIThing* w)
{
    int hashName = w->mHashName;
    if (hashName == GetHashValue32(str_option_0)) {
        w->mMiscData = 0;
    } else if (hashName == GetHashValue32(str_option_1)) {
        w->mMiscData = 1;
    } else if (hashName == GetHashValue32(str_option_2)) {
        w->mMiscData = 2;
    } else if (hashName == GetHashValue32(str_option_3)) {
        w->mMiscData = 3;
    } else if (hashName == GetHashValue32(str_option_4)) {
        w->mMiscData = 4;
    } else if (hashName == GetHashValue32(str_option_5)) {
        w->mMiscData = 5;
    } else if (hashName == GetHashValue32(str_option_5b)) {
        w->mMiscData = 8;
    } else if (hashName == GetHashValue32(str_option_6)) {
        w->mMiscData = 6;
    } else if (hashName == GetHashValue32(str_option_7)) {
        w->mMiscData = 7;
        sWVt_3840* vt = (sWVt_3840*)w->mVtbl;
        vt[8].fn((char*)w + vt[8].delta, 1);
        vt = (sWVt_3840*)w->mVtbl;
        vt[9].fn((char*)w + vt[9].delta, 0);
    } else if (hashName == GetHashValue32(str_title_lodge)) {
        int lvl = cBENewPlayerInterface_getLastHub(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
        if (lvl >= 17) {
            if (lvl < 19) {
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLELodgePeak1));
                return;
            }
            if (lvl < 21) {
                cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLELodgePeak2));
                return;
            }
        }
        cUIText_setUnicodeStringByID((cUIText*)w, GetHashValue32(str_kT_TITLELodgePeak3));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEPopupConfirm_cFEPopupConfirm(void* self, void* engine, void* owner, unsigned short* text, int a4, int a5, int a6, int a7, int a8);
extern "C" void* cFEStateProfileLoad_cFEStateProfileLoad(void* self, void* engine, int a2, unsigned char mask);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern int gLodgeOptionUnlocked[];
extern char str_kT_CMNSaveProgress[];
extern char str_cFEPopupConfirm_b[];
extern char str_kT_OVRCMNQuitTitlePrompt[];
extern char str_cFEStateProfileLoad_b[];
extern char str_TransitionOut_b[];

struct sVE_1F3A38s {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sVE_1F3A38p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Acts on an accepted lodge option: asks to save or quit, opens the profile state, or moves on.
 *
 * Only event 5, and only for options enabled in gLodgeOptionUnlocked. Until bToNextState (0x4C) is set, option 0 and
 * option 6 push a cFEPopupConfirm asking kT_CMNSaveProgress or kT_OVRCMNQuitTitlePrompt (path 0x10 or 6 at
 * 0x50). Option 8 sets the custom interface's +0xC to 0x27 and pushes a cFEStateProfileLoad (cFEStateProfileLoad_cFEStateProfileLoad,
 * mode 3); any other plays "TransitionOut" and pushes the custom interface's getNextState for the option. BE
 * interface 4 is fetched first and its result ignored.
 *
 * @param w  the option widget that sent the event; nothing happens when null
 * @param event  the widget event (only 5, accept, is handled)
 */
extern "C" void cFEStateLodge_onWidgetEvent(cFEStateLodge* self, cUIThing* w, int event)
{
    if (((char*)w) == 0) {
        return;
    }
    if (event != 5) {
        return;
    }
    if (gLodgeOptionUnlocked[w->mMiscData] == 0) {
        return;
    }
    cBE_getInterface_Fv(cBE_getBE(), 4);
    if (w->mMiscData == 0 && self->bToNextState == 0) {
        char* mgr = (char*)SSXApp->FELocale;
        sVE_1F3A38s* vt = *(sVE_1F3A38s**)(mgr + 4);
        unsigned short* text = vt[4].fn(mgr + vt[4].delta, GetHashValue32(str_kT_CMNSaveProgress));
        void* mem = cMemMan_alloc(3068, str_cFEPopupConfirm_b, 0x100, 0);
        void* newState = cFEPopupConfirm_cFEPopupConfirm(mem, self->mpEngine, ((char*)self), text, 0, 0, 0, 0, 0);
        self->mPathToSaveProfile = 0x10;
        cUIStateStack_pushPopup(&self->mpEngine->mStateStack, newState);
        return;
    }
    int k = w->mMiscData;
    if (k == 6 && self->bToNextState == 0) {
        char* mgr = (char*)SSXApp->FELocale;
        sVE_1F3A38s* vt = *(sVE_1F3A38s**)(mgr + 4);
        unsigned short* text = vt[4].fn(mgr + vt[4].delta, GetHashValue32(str_kT_OVRCMNQuitTitlePrompt));
        void* mem = cMemMan_alloc(3068, str_cFEPopupConfirm_b, 0x100, 0);
        void* newState = cFEPopupConfirm_cFEPopupConfirm(mem, self->mpEngine, ((char*)self), text, 0, 0, 0, 0, 0);
        self->mPathToSaveProfile = k;
        cUIStateStack_pushPopup(&self->mpEngine->mStateStack, newState);
        return;
    }
    if (k == 8) {
        *(int*)(*(char**)self->mpEngine + 0xC) = 39;
        void* mem = cMemMan_alloc(640, str_cFEStateProfileLoad_b, 0, 0);
        void* newState = cFEStateProfileLoad_cFEStateProfileLoad(mem, self->mpEngine, 3, 1 << self->mCurrentCtrl);
        cUIStateStack_push(&self->mpEngine->mStateStack, newState);
        return;
    }
    cUIScreen_playFrame(self->mpScreen, cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut_b)), 1);
    char* obj = *(char**)self->mpEngine;
    sVE_1F3A38p* vt = *(sVE_1F3A38p**)(obj + 4);
    void* nextState = vt[4].fn(obj + vt[4].delta, ((char*)self), w->mMiscData);
    if (nextState != 0) {
        cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the lodge menu's events: 0x100 for previous, option and misc (7, 8, 9), 0x101 for the rest.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param key  the menu event
 * @return 0x100 for events 7, 8 and 9, otherwise 0x101
 */
// Key filter: keys 7, 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateLodge_onWidgetMenu(cFEStateLodge* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 7) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodge_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEPopupConfirm_cFEPopupConfirm(void* self, void* engine, void* owner, unsigned short* text, int a4, int a5, int a6, int a7, int a8);
extern "C" void* cFEStateProfileLoad_cFEStateProfileLoad(void* self, void* engine, int a2, unsigned char mask);
extern "C" void cUIStateStack_pop(void* list, int a1);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_pushCheck(void* list, void* item);
extern "C" void cUIStateStack_pushSpecial(void* list, void* item, int a2, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern char str_option_0[];
extern char str_TransitionOut_b[];
extern char str_cFEPopupConfirm_b[];
extern char str_cFEStateProfileLoad_b[];
extern char str_kT_CMNSaveBeforeQuit[];

struct sVE_1F3CF0s {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

struct sVE_1F3CF0p {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sVE_1F3CF0o {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Runs the lodge's save and quit prompts: records yes or no, then saves, quits or carries on.
 *
 * Event 0xF (yes) sets mbIsYesOrNo (0x54) and 0x10 (no) clears it, both popping the prompt. No to the save
 * prompt (path 0x10) sets bToNextState and re-sends accept for "option_0"; no to save-before-quit (0x11)
 * plays "TransitionOut" and pushes getNextState(6). When a prompt (state id 0) closes (0x16) after a yes,
 * path 0x10 or 0x11 pushes a cFEStateProfileLoad with +0x27C = 1 or 2 and mode 1, and path 6 (quit) asks
 * kT_CMNSaveBeforeQuit (path 0x11).
 *
 * @param msg  the popup that sent the event (despite the name)
 * @param id  the popup event (0xF yes, 0x10 no, 0x15 opened, 0x16 closed)
 */
extern "C" void cFEStateLodge_onPopupEvent(cFEStateLodge* self, cUIState* msg, unsigned int id)
{
    switch (id) {
    case 0xF:
        self->mbIsYesOrNo = 1;
        if (self->mPathToSaveProfile == 6 || self->mPathToSaveProfile == 0x10 || self->mPathToSaveProfile == 17) {
            cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
        }
        break;
    case 0x10:
        self->mbIsYesOrNo = 0;
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
        switch (self->mPathToSaveProfile) {
        case 0x10: {
            self->bToNextState = 1;
            sVE_1F3CF0o* vt = (sVE_1F3CF0o*)self->mVtbl;
            vt[19].fn(((char*)self) + vt[19].delta, cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_option_0)), 5);
            break;
        }
        case 17: {
            self->bToNextState = 1;
            cUIScreen_playFrame(self->mpScreen, cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut_b)), 1);
            char* obj = *(char**)self->mpEngine;
            sVE_1F3CF0p* vt = *(sVE_1F3CF0p**)(obj + 4);
            void* nextState = vt[4].fn(obj + vt[4].delta, ((char*)self), 6);
            if (nextState != 0) {
                cUIStateStack_pushCheck(&self->mpEngine->mStateStack, nextState);
            }
            break;
        }
        }
        break;
    case 21:
        break;
    case 22:
        if (msg->mHashName != 0) {
            break;
        }
        if (self->mbIsYesOrNo == 0) {
            break;
        }
        switch (self->mPathToSaveProfile) {
        case 0x10: {
            void* mem = cMemMan_alloc(640, str_cFEStateProfileLoad_b, 0x100, 0);
            char* newState = (char*)cFEStateProfileLoad_cFEStateProfileLoad(mem, self->mpEngine, 3, self->mCtrlBits);
            *(int*)(newState + 0x27C) = 1;
            *(int*)(newState + 0x1C) = (*(int*)(newState + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, newState, 0, 1);
            break;
        }
        case 17: {
            void* mem = cMemMan_alloc(640, str_cFEStateProfileLoad_b, 0x100, 0);
            char* newState = (char*)cFEStateProfileLoad_cFEStateProfileLoad(mem, self->mpEngine, 3, self->mCtrlBits);
            *(int*)(newState + 0x27C) = 2;
            *(int*)(newState + 0x1C) = (*(int*)(newState + 0x1C) & ~0x3F00) | 0x100;
            cUIStateStack_pushSpecial(&self->mpEngine->mStateStack, newState, 0, 1);
            break;
        }
        case 6: {
            char* mgr = (char*)SSXApp->FELocale;
            sVE_1F3CF0s* vt = *(sVE_1F3CF0s**)(mgr + 4);
            unsigned short* text = vt[4].fn(mgr + vt[4].delta, GetHashValue32(str_kT_CMNSaveBeforeQuit));
            void* mem = cMemMan_alloc(3068, str_cFEPopupConfirm_b, 0x100, 0);
            void* newState = cFEPopupConfirm_cFEPopupConfirm(mem, self->mpEngine, ((char*)self), text, 0, 0, 0, 0, 0);
            self->mPathToSaveProfile = 17;
            cUIStateStack_pushPopup(&self->mpEngine->mStateStack, newState);
            break;
        }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail);
#ifdef SKIP_ASM
extern void* cFEStateLodgeRiderDetail_vtbl[];
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);

/**
 * Constructs the lodge rider-detail state: base cUIState, cFEStateLodgeRiderDetail_vtbl, state id 40, user 0.
 *
 * Runs the cUIState constructor cUIState_cUIState() (the engine passes through in the second argument register), stores the
 * vtable and mHashName 40, sets mUserNum (+0x44) to 0 and mCtrlBits (+0x15) to rider slot 0's controller mask
 * (cFECustom_getPlayerCtrlMask() on the engine's FE custom object). cFECustom_getNextState() and cFECustom_getPrevState() create it.
 *
 * @return @p self
 */
extern "C" void* cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cFEStateLodgeRiderDetail* self)
{
    cUIState_cUIState(self);
    self->mVtbl = cFEStateLodgeRiderDetail_vtbl;
    self->mHashName = 40;
    *(char*)&self->mUserNum = 0;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, 0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_155rider_details_conquer[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
extern "C" int cBERewardInterface_getNumOwnedCheatChars(void* self, int a1, int a2);

/**
 * Opens the rider-details screen ("155rider_details_conquer") and checks for unlocked cheat characters.
 *
 * Keeps the screen at 0x40 and sets 0x48 (On Tour mbCheatCharEnable) to 1, or to 0 when cBERewardInterface_getNumOwnedCheatChars() on
 * the reward interface (13) counts no unlocked cheat characters (ids 10-29) for the user and their character.
 */
extern "C" void cFEStateLodgeRiderDetail_onCreateScreen(cFEStateLodgeRiderDetail* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_155rider_details_conquer), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mbCheatCharEnable = 1;
    int charID = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
    if (cBERewardInterface_getNumOwnedCheatChars(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum, charID) == 0) {
        self->mbCheatCharEnable = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onPopupEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setPlayerName(void* self, int a1, const char* name);
extern "C" void cBENewPlayerInterface_setPlayerCheatCharID(void* self, int a1, int a2);
extern "C" int strlen(const char* s);
extern char str_131cheat_char_b[];

struct sVEntryK1F4108 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline int IsK1F4108(int id, char* s)
{
    return id == GetHashValue32(s);
}

static inline void commitK1F4108(void* np)
{
    sVEntryK1F4108* vt = *(sVEntryK1F4108**)((char*)np + 0xC);
    vt[1].fn((char*)np + vt[1].delta);
}

/**
 * Applies the rider-details popups when they close: the chosen cheat character or a new player name.
 *
 * Only event 0x16. From the popup named "131cheat_char" with its confirm word (+0x74) set, stores its choice
 * (+0x70) as the user's cheat character (cBENewPlayerInterface_setPlayerCheatCharID); from a popup with state id 0 (the keyboard) that was
 * accepted (+0x5C) with a non-empty text (+0x74), sets the user's player name (cBENewPlayerInterface_setPlayerName). The player
 * interface's vtable slot 1 is then called, probably to commit the change.
 *
 * @param popup  the popup state that sent the event
 * @param event  the popup event (0x16 closed)
 */
extern "C" void cFEStateLodgeRiderDetail_onPopupEvent(cFEStateLodgeRiderDetail* self, cUIState* popup, int event)
{
    void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (event != 22)
        return;
    if (IsK1F4108(popup->mHashName, str_131cheat_char_b)) {
        int cheatCharId = ((cFEStateCheatCharSelect*)popup)->mCheatCharID;
        if (((cFEStateCheatCharSelect*)popup)->mDone != 0) {
            void* np2 = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBENewPlayerInterface_setPlayerCheatCharID(np2, self->mUserNum, cheatCharId);
            commitK1F4108(np2);
        }
    } else if (popup->mHashName == 0) {
        if (((cKeyboardPopup*)popup)->mbAcceptText != 0) {
            char* name = ((cKeyboardPopup*)popup)->mszEditString;
            if (strlen(name) != 0) {
                cBENewPlayerInterface_setPlayerName(np, self->mUserNum, name);
                commitK1F4108(np);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void* cBECharacterInterface_getShortNameByCharacter(void* self, int player, int index);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern char str_Option_1_b[];
extern char str_Option_2_b[];
extern char str_Option_3_b[];
extern char str_Option_4_b[];
extern char str_Option_5_b[];
extern char str_Option_6[];
extern char str_Option_7[];
extern char str_rider_name_b[];

/**
 * Numbers the rider-details options and shows the rider's name.
 *
 * "Option_1" to "Option_7" get misc data 9 to 15; "rider name" shows, as ASCII, cBECharacterInterface_getShortNameByCharacter()'s record (BE
 * interface 2) for the character of human rider 0.
 *
 * @param w  the widget just created (its hash name is at +0x38)
 */
extern "C" void cFEStateLodgeRiderDetail_onWidgetCreate(cFEStateLodgeRiderDetail* self, cUIThing* w)
{
    int hashName = w->mHashName;
    if (hashName == GetHashValue32(str_Option_1_b)) {
        w->mMiscData = 9;
    }     else if (hashName == GetHashValue32(str_Option_2_b)) {
        w->mMiscData = 10;
    }     else if (hashName == GetHashValue32(str_Option_3_b)) {
        w->mMiscData = 11;
    }     else if (hashName == GetHashValue32(str_Option_4_b)) {
        w->mMiscData = 12;
    }     else if (hashName == GetHashValue32(str_Option_5_b)) {
        w->mMiscData = 13;
    }     else if (hashName == GetHashValue32(str_Option_6)) {
        w->mMiscData = 14;
    }     else if (hashName == GetHashValue32(str_Option_7)) {
        w->mMiscData = 15;
    } else if (hashName == GetHashValue32(str_rider_name_b)) {
        void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
        signed char cid = cBENewPlayerInterface_getRiderCharID(np, cBENewPlayerInterface_getHumanRider(np, 0));
        cUIText_setAsciiString((cUIText*)w, (const char*)cBECharacterInterface_getShortNameByCharacter(cBE_getInterface_Fv(cBE_getBE(), 2), cid, 0));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onPreRender2D);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void DisableTextWidget(void* obj);
extern char str_Option_3_b[];

/**
 * Greys out "Option_3" (the cheat-character option) before each draw when no cheat character is unlocked.
 *
 * When 0x48 (On Tour mbCheatCharEnable) is 0, DisableTextWidget() makes the widget static and caps its alpha at
 * 0.5.
 */
extern "C" void cFEStateLodgeRiderDetail_onPreRender2D(cFEStateLodgeRiderDetail* self)
{
    if (self->mbCheatCharEnable == 0) {
        void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Option_3_b));
        if (obj != 0) {
            DisableTextWidget(obj);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the rider-details menu's events: 0x100 for the option and misc buttons (8, 9), else 0x101.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param key  the menu event
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
extern "C" int cFEStateLodgeRiderDetail_onWidgetMenu(cFEStateLodgeRiderDetail* self, int pMenu, unsigned int key)
{
    int result = 257;
    if (key < 10) {
        if (key < 8) {
            result = 257;
        } else {
            return 0x100;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateLodgeRiderDetail_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* self, void* engine, void* owner, int a3, unsigned char a4);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int a1);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int key, int a2, int a3);
extern "C" void cKeyboardPopup_setDisableShiftNumbers(void* self, int a1);
extern "C" void cKeyboardPopup_setInputText(void* self, const char* str);
extern "C" const char* cBENewPlayerInterface_getPlayerName(void* iface, int player);
extern "C" void* cFEStateCheatCharSelect_cFEStateCheatCharSelect(void* self, void* engine, void* owner, int player, int charID);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" int cBERewardInterface_getNumOwnedCheatChars(void* self, int a1, int a2);
int cBENewPlayerInterface_getPlayerCharID(void* self, int player);
struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern int gLodgeOptionUnlocked[];
extern char str_cKeyboardPopup_b[];
extern char str_cFEStateCheatCharSelect_b[];
extern char str_TransitionOut_b[];

struct sVE_1F4400 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Acts on the rider-details options: renames the rider, picks a cheat character, opens a screen, or leaves.
 *
 * Event 5 (accept), for options enabled in gLodgeOptionUnlocked (read at 9-15, past its 7 entries): option 14
 * ("Option_6") opens a "cKeyboardPopup" (cKeyboardPopup_cKeyboardPopup) limited to 8 characters, with some keys left out and
 * the player's name as its text; option 11 ("Option_3") opens a "cFEStateCheatCharSelect" popup
 * (cFEStateCheatCharSelect_cFEStateCheatCharSelect) when a cheat character is unlocked; any other plays "TransitionOut" and pushes
 * getNextState's state for the option. Event 6 (back) pushes getPrevState's state.
 *
 * @param w  the option widget that sent the event; nothing happens when null
 * @param event  the widget event (5 accept, 6 back)
 */
extern "C" void cFEStateLodgeRiderDetail_onWidgetEvent(cFEStateLodgeRiderDetail* self, cUIThing* w, int event)
{
    if (((char*)w) == 0) {
        return;
    }
    switch (event) {
    case 5: {
        int k = w->mMiscData;
        if (gLodgeOptionUnlocked[k] == 0) {
            return;
        }
        if (k == 14) {
            void* mem = cMemMan_alloc(1092, str_cKeyboardPopup_b, 0x100, 0);
            char* popup = (char*)cKeyboardPopup_cKeyboardPopup(mem, self->mpEngine, ((char*)self), 0, self->mCtrlBits);
            *(char*)(popup + 0x44) = 0;
            *(int*)(popup + 0x140) = 55;
            cKeyboardPopup_setMaxCharacters(popup, 8);
            cKeyboardPopup_setIncludeKey(popup, 75, 1, 2);
            *(int*)(popup + 0x58) = 1;
            cKeyboardPopup_setDisableShiftNumbers(popup, 1);
            cKeyboardPopup_setIncludeKey(popup, 0xE, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 39, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 40, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 52, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 26, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 25, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 53, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 0x40, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 65, 0, 2);
            cKeyboardPopup_setIncludeKey(popup, 66, 0, 2);
            cUIStateStack_pushPopup(&self->mpEngine->mStateStack, popup);
            cKeyboardPopup_setInputText(popup, cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum));
            return;
        }
        if (k == 11) {
            int id = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->mUserNum);
            if (cBERewardInterface_getNumOwnedCheatChars(cBE_getInterface_Fv(cBE_getBE(), 13), self->mUserNum, id) > 0) {
                void* mem = cMemMan_alloc(484, str_cFEStateCheatCharSelect_b, 0, 0);
                void* popup = cFEStateCheatCharSelect_cFEStateCheatCharSelect(mem, self->mpEngine, ((char*)self), self->mUserNum, id);
                cUIStateStack_pushPopup(&self->mpEngine->mStateStack, popup);
            }
            return;
        }
        cUIScreen_playFrame(self->mpScreen, cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut_b)), 1);
        char* obj = *(char**)self->mpEngine;
        sVE_1F4400* vt = *(sVE_1F4400**)(obj + 4);
        void* nextState = vt[4].fn(obj + vt[4].delta, ((char*)self), w->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    case 6: {
        char* obj = *(char**)self->mpEngine;
        sVE_1F4400* vt = *(sVE_1F4400**)(obj + 4);
        void* nextState = vt[5].fn(obj + vt[5].delta, ((char*)self), 0);
        if (nextState != 0) {
            cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelodge", cFEStateBuyAttrib_cFEStateBuyAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState_cUIState_4728(void* self, int a1) __asm__("cUIState_cUIState");
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" unsigned char cFECustom_getPlayerCtrlMask_4728(void* p, int a1) __asm__("cFECustom_getPlayerCtrlMask");
extern "C" int cBEStatInterface_getStatFromProfile_4728(void* iface, int a1, int stat) __asm__("cBEStatInterface_getStatFromProfile");
extern "C" void MEM_fill(void* dst, int c, int n);
extern void* cFEStateBuyAttrib_vtbl[];
extern const int D_004780B0[];


/**
 * Constructs the lodge attribute-buying state: base cUIState, cFEStateBuyAttrib_vtbl, state id 41, and a copy of the
 * rider's seven attribute values.
 *
 * Runs the cUIState constructor cUIState_cUIState() on @p engine, stores mHashName 41 and the vtable, sets mUserNum (+0x44) to
 * 0 and mCtrlBits (+0x15) to rider slot 0's controller mask (cFECustom_getPlayerCtrlMask()), and clears the 0x1C bytes at +0x84. For
 * each of the seven attributes listed in D_004780B0 it stores cBEStatInterface_getStatFromProfile() (player interface, rider 0) at +0x4C[i]
 * and that value / 5 (the level) at +0x68[i]. cFECustom_getNextState() creates it.
 *
 * @param stateMemory  the memory for the state
 * @param engine  the UI engine
 * @return @p stateMemory
 */
extern "C" void* cFEStateBuyAttrib_cFEStateBuyAttrib(cFEStateBuyAttrib* stateMemory, int engine)
{
    cFEStateBuyAttrib* self = stateMemory;
    cUIState_cUIState_4728(self, engine);
    stateMemory->mHashName = 41;
    stateMemory->mVtbl = cFEStateBuyAttrib_vtbl;
    stateMemory->mUserNum = 0;
    stateMemory->mCtrlBits = cFECustom_getPlayerCtrlMask_4728(*(void**)stateMemory->mpEngine, 0);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    MEM_fill(&stateMemory->mPotentialExperience[0], 0, 0x1C);
    for (int i = 0; i < 7; i++) {
        self->mExperience[i] = cBEStatInterface_getStatFromProfile_4728(iface, 0, D_004780B0[i]);
        self->mExperienceLevel[i] = (signed char)(cBEStatInterface_getStatFromProfile_4728(iface, 0, D_004780B0[i]) / 5);
    }
    return ((void*)stateMemory);
}
#endif

