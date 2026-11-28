#include "vtable.h"
#include "common.h"
#include "fe/festateonlineagreement.h"
#include "ui/uithing.h"
#include "ui/uimenu.h"
#include "ui/uigroup.h"
#include "ui/uistate.h"
#include "main/gamemode.h"
#include "util/locale.h"
#include "be/beintreward.h"
#include "be/berewarddb.h"
#include "be/beintscore.h"
#include "fe/ovstate.h"
#include "sound/ssxaudio.h"
#include "bx/appman.h"
#include "fe/ovstateeventresults.h"
#include "fe/fesound.h"
#include "fe/festateselectmp.h"
#include "fe/festatestats.h"
#include "fe/festateaudiooptions.h"
#include "fe/festatetrophyroom.h"
#include "fe/festatenethelppopup.h"
#include "fe/festatevideoplayer.h"
#include "fe/festatebigradio.h"
#include "fe/festatecharequip.h"
#include "ui/uiinput.h"
#include "ui/uitextscroll.h"
#include "ui/uiengine.h"
#include "ui/uiscreen.h"
#include "fe/festaterewardsroom.h"
#include "fe/festatepeakroom.h"
#include "fe/festatempcircuit.h"
#include "fe/fepopupvideocalibration.h"
#include "fe/fepopupselectmultiplayermode.h"
#include "fe/fepopupscreenpos.h"
#include "fe/festateruleselect.h"
#include "fe/festateriderbio.h"
#include "fe/festatepreviewreward.h"
#include "fe/festatemountainroom.h"
#include "fe/festaterewards.h"
#include "fe/festatenet.h"
#include "fe/festatelogin.h"
#include "fe/festatenetcreateroompopup.h"
#include "fe/festateauthdnas.h"
#include "fe/festatenetquickmessages.h"
#include "fe/festatenetquickplaypopup.h"
#include "fe/festateonlinecharacterselect.h"
#include "fe/festatecreateduplicate.h"
#include "fe/festatenetchallenge.h"
#include "fe/festatenetaccount.h"
#include "fe/festatenetlobby.h"
#include "fe/festatenetcreate.h"
#include "fe/festatenetconfig.h"
#include "fe/messagecenter.h"
#include "fe/festatenetmessenger.h"
#include "fe/festatechatroom.h"
#include "fe/festatereadmessagepopup.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_156thx[];
extern char* D_004A1540;
extern char str_Text0000[];
extern char str_kT_THXBrightness[];

/**
 * Opens the THX video-calibration popup ("156thx") on its brightness page.
 *
 * Shows the "brightps2" image and sets Text0000 to kT_THXBrightness.
 */
void cFEPopupVideoCalibration::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_156thx), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cUIThing* obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(D_004A1540));
    if (obj != 0) {
        obj->setVisible(1);
    }
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text0000));
    if (text != 0) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_THXBrightness));
    }
    mImage = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupVideoCalibration_onInputBegin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// D_004A1540 is an 8-byte string table; this reads its second entry.
extern char* D_004A1540_DEC68[2] __asm__("D_004A1540");
extern char str_kT_THXBars[];
extern char str_Text0001[];

struct sVEi_EC68 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVEm_EC68 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDefObj_EC68 {
    sVEm_EC68* vt;
};
extern sDefObj_EC68 gUISoundNull_EC68 __asm__("gUISoundNull");

struct sPopup_EC68 {
    char pad0[0x10];
    char* owner;                // 0x10
    char pad14[0x8];            // 0x14
    int flags;                  // 0x1C
    char pad20[0x20];           // 0x20
    void* screen;               // 0x40
    char pad44[0x4];            // 0x44
    signed char shown;          // 0x48
};

/**
 * Steps the video-calibration popup on input slot 5 (probably accept): first to the colour-bars page, then
 * marks it done.
 *
 * The bars page shows "barsps2" in place of "brightps2", sets Text0000 to kT_THXBars and hides Text0001. Each
 * press plays UI sound 6.
 *
 * @param in  the input (On Tour cUIInput* pInput)
 * @return 1 (the popup takes every input)
 */
bool cFEPopupVideoCalibration::onInputBegin(cUIInput* in)
{
    if (in->isNext() != 0) {
        if (mImage == 0) {
            cUIThing* obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(D_004A1540));
            if (obj != 0) {
                obj->setVisible(0);
            }
            obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(D_004A1540_DEC68[1]));
            if (obj != 0) {
                obj->setVisible(1);
            }
            cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text0000));
            if (text != 0) {
                cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_THXBars));
            }
            obj = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Text0001));
            if (obj != 0) {
                obj->setVisible(0);
            }
            mImage = 1;
        } else {
            mAttrState = ((mAttrState | 0x80) & ~0x3F00) | 0x700;
        }
        sDefObj_EC68* uiSound = (sDefObj_EC68*)mpEngine->mpSound;
        if (uiSound == 0)
            uiSound = &gUISoundNull_EC68;
        if (uiSound != 0)
            uiSound->vt[2].fn(vthis(uiSound, uiSound->vt[2]), 6);
    }
    return true;
}

extern void* cFEPopupScreenPos_vtbl[];
extern "C" void* cUIState__cUIState(void*);
#endif

extern void* cFEPopupScreenPos_vtbl[];
extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos__cFEPopupScreenPos__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the screen-position popup through the UI-state destructor cUIState__cUIState().
 *
 * Restores cFEPopupScreenPos_vtbl first. Only @p self is declared: the destructor flags the caller passed in the
 * second argument register reach cUIState__cUIState() unchanged.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
void* cFEPopupScreenPos__cFEPopupScreenPos(void* vself)
{
    cFEPopupScreenPos* self = (cFEPopupScreenPos*)vself;
    VPTR_OF(cListNode, self) = cFEPopupScreenPos_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_24screen_position[];

struct sOptions_EE10 {
    unsigned int flags;
    int pad_4 : 8;
    int b5 : 8;
    int b6 : 8;
    int pad_7 : 8;
    int data[(0x288 - 8) / 4];
};
extern sOptions_EE10 cBEOptionInterface_mOption_EE10 __asm__("cBEOptionInterface_mOption");

static inline int optB6_EE10(sOptions_EE10 o)
{
    return o.b6;
}

static inline int optB5_EE10(sOptions_EE10 o)
{
    return o.b5;
}

/**
 * Opens the screen-position popup ("24screen_position") and loads the current screen offset from the options.
 */
void cFEPopupScreenPos::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_24screen_position), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
        mHashName = GetHashValue32(str_24screen_position);
    }
    void* be = cBE_getBE();
    cBE_getInterface_Fv(be, 4);
    int* offsetX = (int*)&mX;
    *offsetX = optB6_EE10(cBEOptionInterface_mOption_EE10);
    int* p4C = (int*)&mY;
    *p4C = optB5_EE10(cBEOptionInterface_mOption_EE10);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_getData);
#ifdef SKIP_ASM
/**
 * Returns one of the screen-position popup's results.
 *
 * @param i  0 for the horizontal offset, 1 for the vertical offset (both clamped to -20..20 by the popup's
 *           update), anything else for the accepted flag
 * @return the x offset, the y offset, or 1 when the popup was accepted and 0 when it was cancelled
 */
extern "C" int cFEPopupScreenPos_getData(cFEPopupScreenPos* self, int i)
{
    switch (i) {
    case 0:
        return self->mX;
    case 1:
        return self->mY;
    }
    return self->mAccept;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onUpdate);
#ifdef SKIP_ASM
struct cUIScreen;
// PORT: the unit declares cUIScreen_getFrameByLabel as returning int; the frame id is an unsigned short.
unsigned short cUIScreen_getFrameByLabel_us(cUIScreen* screen, int hash) __asm__("cUIScreen_getFrameByLabel__FP9cUIScreeni");
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIStateStack_pop(void* self, int a1);
extern char str_hl_left[];
extern char str_hl_up[];
extern char str_hl_down[];
extern char str_hl_right[];

extern cUISoundInterface gUISoundNull;

static inline void sound_F050(cFEPopupScreenPos* s, int msg)
{
    cUISoundInterface* p = s->mpEngine->mpSound;
    if (p == 0)
        p = &gUISoundNull;
    if (p != 0)
        p->playSound(msg);
}

static inline void playFrame_F050(cFEPopupScreenPos* s, char* name)
{
    unsigned short frame = cUIScreen_getFrameByLabel_us((cUIScreen*)((void*)s->mpScreen), GetHashValue32(name));
    if (frame != cUIScreen::kNoFrame)
        cUIScreen_playFrame(((void*)s->mpScreen), frame, 1);
}

static inline int clamp_F050(int v, int lo, int hi)
{
    if (v >= lo) {
        if (v > hi) {
            v = hi;
        }
    } else {
        v = lo;
    }
    return v;
}

/**
 * Moves the screen offset with the first pad that has input, or closes the popup on accept (keep) or back
 * (cancel).
 *
 * The offset stays within -20..20 on each axis (UI sound 4 when pushed past it) and is applied at once
 * through
 * the renderer (GrMan slot 12). Accept plays sound 6 and back sound 3.
 */
void cFEPopupScreenPos::onUpdate()
{
    cUIInput* in = (cUIInput*)((char*)mpEngine->mpInput);
    for (int i = 0; i < 2; i++) {
        sVEntry<void(void*, unsigned char)>* vt = (sVEntry<void (void*, unsigned char)>*)VPTR_OF(cUIInput, in);
        vt[44].fn(vthis(in, vt[44]), i);
        if (in->keyPressed() == 0) {
            continue;
        }
        if (in->isNext()) {
            sound_F050((this), 6);
            mAccept = 1;
            mAttrState |= 0x80;
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
        } else if (in->isPrevious()) {
            sound_F050((this), 3);
            mAccept = 0;
            mAttrState |= 0x80;
            cUIStateStack_pop(&mpEngine->mStateStack, 1);
        } else if (in->isLeft()) {
            mX--;
            playFrame_F050((this), str_hl_left);
        } else if (in->isRight()) {
            mX++;
            playFrame_F050((this), str_hl_right);
        } else if (in->isUp()) {
            mY++;
            playFrame_F050((this), str_hl_up);
        } else if (in->isDown()) {
            mY--;
            playFrame_F050((this), str_hl_down);
        }
        if (mX < -20 || mX > 20 || mY < -20 || mY > 20) {
            sound_F050((this), 4);
        }
        mX = clamp_F050(mX, -20, 20);
        mY = clamp_F050(mY, -20, 20);
        GrMan->SetScreenOffset(mX, mY);
        return;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupScreenPos_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the screen-position popup's lose transition at once.
 *
 * The state stack update cUIStateStack_updateLoop() ends a state's lose transition once this returns nonzero.
 *
 * @return always true (transition finished)
 */
bool cFEPopupScreenPos::onLoseTransition()
{
    return true;
}
#endif

extern void* cFEState_vtbl[];
extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRiderBio_cFEStateRiderBio1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRiderBio_vtbl[];

/**
 * Constructs a rider-bio state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateRiderBio_vtbl; nothing else is set (the full
 * constructor cFEStateRiderBio_cFEStateRiderBio also stores the user number and controller bits).
 *
 * @return @p self
 */
void* cFEStateRiderBio::cFEStateRiderBio1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRiderBio_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRiderBio__cFEStateRiderBio__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rider-bio front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRiderBio__cFEStateRiderBio(void* vself)
{
    cFEStateRiderBio* self = (cFEStateRiderBio*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRuleSelect_cFEStateRuleSelect1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRuleSelect_vtbl[];

/**
 * Constructs a rule-select state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateRuleSelect_vtbl; nothing else is set (the
 * full constructor is cFEStateRuleSelect_cFEStateRuleSelect).
 *
 * @return @p self
 */
void* cFEStateRuleSelect::cFEStateRuleSelect1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRuleSelect_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRuleSelect__cFEStateRuleSelect__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rule-select front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRuleSelect__cFEStateRuleSelect(void* vself)
{
    cFEStateRuleSelect* self = (cFEStateRuleSelect*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cBraggingRightsMenu__cBraggingRightsMenu);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cBraggingRightsMenu_vtbl[];

/**
 * Destroys a rule-select element (the 0x34-byte objects whose vptr is at +0x30), freeing it when asked.
 *
 * Stores the element's vtable cBraggingRightsMenu_vtbl at +0x30 and calls operator_delete when bit 0 of @p flags is set. The elements
 * are built by cBraggingRightsMenu_cBraggingRightsMenu; cFEStateBraggingRights holds two of them at +0x6D0.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cBraggingRightsMenu__cBraggingRightsMenu(cBraggingRightsMenu* self, int flags)
{
    self->mVtbl = cBraggingRightsMenu_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateBraggingRights_cFEStateBraggingRights1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new[](unsigned int, void* p) { return p; }
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern "C" void cBraggingRightsMenu_cBraggingRightsMenu(void* self);
extern void* cFEStateBraggingRights_vtbl[];

// cBraggingRightsMenu with its constructor (cBraggingRightsMenu_cBraggingRightsMenu), for the mMenuCtrl[2] array.
struct cBraggingRightsMenuArr : cBraggingRightsMenu {
    cBraggingRightsMenuArr() { cBraggingRightsMenu_cBraggingRightsMenu(this); }
};

/**
 * Constructs a bragging-rights state with no engine (the default-constructor copy).
 *
 * Runs the cFEStateNet constructor cFEStateNet_cFEStateNet(self, 0, 0), stores cFEStateBraggingRights_vtbl and builds
 * the two rule-select columns mMenuCtrl[2] (0x34 bytes each) with cBraggingRightsMenu_cBraggingRightsMenu. The full
 * constructor cFEStateBraggingRights_cFEStateBraggingRights also sets mHashName and clears mpGameEventMapping.
 *
 * @return @p self
 */
void* cFEStateBraggingRights::cFEStateBraggingRights1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateBraggingRights_vtbl;
    new (mMenuCtrl) cBraggingRightsMenuArr[2];
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateBraggingRights__cFEStateBraggingRights);
#ifdef SKIP_ASM
extern void* cFEStateBraggingRights_vtbl[];
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");


/**
 * Destroys the bragging-rights front-end state: its two rule-select columns (mMenuCtrl[2], each through its virtual
 * destructor), then the base state.
 *
 * The base destructor cFEStateNet__cFEStateNet frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateBraggingRights__cFEStateBraggingRights(cFEStateBraggingRights* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateBraggingRights_vtbl;
    // g++'s member-array destruction: each mMenuCtrl element's virtual destructor (entry 1, flags 0), last first
    if (self->mMenuCtrl != 0) {
        cBraggingRightsMenu* menu = self->mMenuCtrl + 2;
        while (self->mMenuCtrl != menu) {
            menu--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)menu->mVtbl;
            vt[1].fn(vthis(menu, vt[1]), 0);
        }
    }
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateBraggingRights_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a menu of this state gains the focus (an empty onMenuGainFocus override).
 *
 * On Tour's version also takes the root group (cUIGroup*), which this declaration leaves out.
 */
void cFEStateBraggingRights::onMenuGainFocus(cUIGroup*)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChatRoom_cFEStateChatRoom1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateChatRoom_vtbl[];

/**
 * Constructs a chat-room state with no engine (the default-constructor copy).
 *
 * Runs cFEStateNet_cFEStateNet(self, 0, 0), clears +0x6F8 and stores cFEStateChatRoom_vtbl.
 *
 * @return @p self
 */
void* cFEStateChatRoom::cFEStateChatRoom1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    mbExiting = 0;
    VPTR_OF(cListNode, this) = cFEStateChatRoom_vtbl;
    return this;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChatRoom_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a menu of this state gains the focus (an empty onMenuGainFocus override).
 *
 * On Tour's version also takes the root group (cUIGroup*), which this declaration leaves out.
 */
void cFEStateChatRoom::onMenuGainFocus(cUIGroup*)
{
}
#endif

extern void* cFEStateOnlineMainMenu_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineMainMenu_cFEStateOnlineMainMenu1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the online main menu without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateOnlineMainMenu_vtbl.
 *
 * @return @p self
 */
extern "C" void *cFEStateOnlineMainMenu_cFEStateOnlineMainMenu1(cFEStateNet* self) {
    cFEStateNet_cFEStateNet(self, 0, 0);
    VPTR_OF(cListNode, self) = cFEStateOnlineMainMenu_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineMainMenu__cFEStateOnlineMainMenu__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the online main-menu state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateOnlineMainMenu__cFEStateOnlineMainMenu(void* vself)
{
    cFEStateNet* self = (cFEStateNet*)vself;
    VPTR_OF(cListNode, self) = cFEStateOnlineMainMenu_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineAgreement_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the terms-of-service state: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateOnlineAgreement::onLoseTransition()
{
    return true;
}
#endif

extern void* cFEStateNetQuickPlayPopup_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the quick-play popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then clears
 * mbSearching and installs cFEStateNetQuickPlayPopup_vtbl.
 *
 * @return @p self
 */
void * cFEStateNetQuickPlayPopup::cFEStateNetQuickPlayPopup1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    mbSearching = 0;
    VPTR_OF(cListNode, this) = cFEStateNetQuickPlayPopup_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the quick-match popup state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup(void* vself)
{
    cFEStateNetQuickPlayPopup* self = (cFEStateNetQuickPlayPopup*)vself;
    VPTR_OF(cListNode, self) = cFEStateNetQuickPlayPopup_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickPlayPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the quick-match popup: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateNetQuickPlayPopup::onLoseTransition()
{
    return true;
}
#endif

extern void* cFEStateNetSSXWorld_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetSSXWorld_cFEStateNetSSXWorld1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the SSX World state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateNetSSXWorld_vtbl.
 *
 * @return @p self
 */
extern "C" void *cFEStateNetSSXWorld_cFEStateNetSSXWorld1(cFEStateNet* self) {
    cFEStateNet_cFEStateNet(self, 0, 0);
    VPTR_OF(cListNode, self) = cFEStateNetSSXWorld_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetSSXWorld__cFEStateNetSSXWorld__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the SSX World hub state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateNetSSXWorld__cFEStateNetSSXWorld(void* vself)
{
    cFEStateNet* self = (cFEStateNet*)vself;
    VPTR_OF(cListNode, self) = cFEStateNetSSXWorld_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

extern void* cFEStateNetQuickMessages_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickMessages_cFEStateNetQuickMessages1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the quick-messages state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateNetQuickMessages_vtbl.
 *
 * @return @p self
 */
void * cFEStateNetQuickMessages::cFEStateNetQuickMessages1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateNetQuickMessages_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickMessages__cFEStateNetQuickMessages__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the quick-messages editor state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateNetQuickMessages__cFEStateNetQuickMessages(void* vself)
{
    cFEStateNetQuickMessages* self = (cFEStateNetQuickMessages*)vself;
    VPTR_OF(cListNode, self) = cFEStateNetQuickMessages_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetQuickMessages_isChanged);
#ifdef SKIP_ASM
/**
 * Returns the int at +0x6D0 of an online state.
 *
 * An out-of-line inline accessor with no caller. It sits right after cFEStateNetQuickMessages' destructor; in that
 * class +0x6D0 is mbChanged.
 *
 * @return the int at +0x6D0
 */
int cFEStateNetQuickMessages::isChanged()
{
    return (mbChanged);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLobbyMenu_cFEStateLobbyMenu1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateLobbyMenu_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs the lobby menu without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateLobbyMenu_vtbl and sets mszChannelName and mszRoomName to cBXString_EmptyString.
 *
 * @return @p self
 */
void* cFEStateLobbyMenu::cFEStateLobbyMenu1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyStr = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateLobbyMenu_vtbl;
    mszChannelName = emptyStr;
    mszRoomName = emptyStr;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLobbyMenu__cFEStateLobbyMenu);
#ifdef SKIP_ASM
extern void* cFEStateLobbyMenu_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");

/**
 * Destroys the lobby screen state: frees its room and channel name strings, then runs the cFEStateNet destructor.
 *
 * Restores cFEStateLobbyMenu_vtbl, destroys the cBXStrings mszRoomName (+0xB80) and mszChannelName (+0x76C) and
 * calls cFEStateNet__cFEStateNet with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateLobbyMenu__cFEStateLobbyMenu(cFEStateLobbyMenu* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateLobbyMenu_vtbl;
    cBXString__cBXString((char*)&self->mszRoomName, 2);
    cBXString__cBXString((char*)&self->mszChannelName, 2);
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateNetCreateRoomPopup_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs the create-room popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateNetCreateRoomPopup_vtbl and sets mszRoomName, mszChannelName and mszPassword to cBXString_EmptyString.
 *
 * @return @p self
 */
void* cFEStateNetCreateRoomPopup::cFEStateNetCreateRoomPopup1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyStr = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateNetCreateRoomPopup_vtbl;
    mszRoomName = emptyStr;
    mszChannelName = emptyStr;
    mszPassword = emptyStr;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup);
#ifdef SKIP_ASM
extern void* cFEStateNetCreateRoomPopup_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");

/**
 * Destroys the create-room popup: frees its three strings (mszPassword, mszChannelName, mszRoomName), then runs the
 * cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup(cFEStateNetCreateRoomPopup* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateNetCreateRoomPopup_vtbl;
    cBXString__cBXString((char*)&self->mszPassword, 2);
    cBXString__cBXString((char*)&self->mszChannelName, 2);
    cBXString__cBXString((char*)&self->mszRoomName, 2);
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the create-room popup: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateNetCreateRoomPopup::onLoseTransition()
{
    return true;
}
#endif

extern void* cFEStateNetHelpPopup_vtbl[];
extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getRoomName);
#ifdef SKIP_ASM
/**
 * Returns the room name entered in the popup, mszRoomName (+0x6DC).
 *
 * @return the room name string
 */
char* cFEStateNetCreateRoomPopup::getRoomName()
{
    return mszRoomName;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getChannelName);
#ifdef SKIP_ASM
/**
 * Returns the popup's channel name, mszChannelName (+0x6E0).
 *
 * @return the channel name string
 */
char* cFEStateNetCreateRoomPopup::getChannelName()
{
    return mszChannelName;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetCreateRoomPopup_getPassword);
#ifdef SKIP_ASM
/**
 * Returns the password entered in the popup, mszPassword (+0x6E4).
 *
 * @return the password string
 */
char* cFEStateNetCreateRoomPopup::getPassword()
{
    return mszPassword;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_cFEStateNetHelpPopup1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateNetHelpPopup_vtbl[];

// Element type of an inline-constructed array; its empty constructor leaves only the loop counters.
struct sSlot_F900 {
    int v;
    sSlot_F900() {}
};

/**
 * Constructs a network help popup with no engine or parent (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0), stores cFEStateNetHelpPopup_vtbl and runs the empty constructor
 * of a 7-element array at +0x20 (only the loop is left). The full constructor is cFEStateNetHelpPopup_cFEStateNetHelpPopup.
 *
 * @return @p self
 */
void* cFEStateNetHelpPopup::cFEStateNetHelpPopup1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateNetHelpPopup_vtbl;
    new ((char*)&mpParentState) sSlot_F900[7];
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup__cFEStateNetHelpPopup__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the network help popup through the UI-state destructor cUIState__cUIState().
 *
 * Restores cFEStateNetHelpPopup_vtbl first. Only @p self is declared: the destructor flags the caller passed in
 * the second argument register reach cUIState__cUIState() unchanged.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
void* cFEStateNetHelpPopup__cFEStateNetHelpPopup(void* vself)
{
    cFEStateNetHelpPopup* self = (cFEStateNetHelpPopup*)vself;
    VPTR_OF(cListNode, self) = cFEStateNetHelpPopup_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the network help popup's lose transition at once.
 *
 * The state stack update cUIStateStack_updateLoop() ends a state's lose transition once this returns nonzero.
 *
 * @return always true (transition finished)
 */
bool cFEStateNetHelpPopup::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_setScreenName);
#ifdef SKIP_ASM
/**
 * Stores @p value at +0x54 of a network help popup: its title text id.
 *
 * cFEStateNetHelpPopup_onGainTransition shows the title from this id (an empty title when 0). An out-of-line inline
 * setter with no caller.
 *
 * @param value  the title's text id
 */
void cFEStateNetHelpPopup::setScreenName(int value)
{
    ((int)muiScreenName) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_setBottomText);
#ifdef SKIP_ASM
/**
 * Stores @p value at +0x64 of a network help popup: the text id of its bottom note.
 *
 * cFEStateNetHelpPopup_onGainTransition shows it in the third text widget (+0xB0). An out-of-line inline setter with no
 * caller.
 *
 * @param value  the bottom note's text id
 */
void cFEStateNetHelpPopup::setBottomText(int value)
{
    ((int)muiBottomText) = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_setReferEventsToParent);
#ifdef SKIP_ASM
/**
 * Stores @p value at +0x50 of a network help popup: whether it passes events to its parent state.
 *
 * When it is nonzero, cFEStateNetHelpPopup_handleEvent forwards events to mpParentState. An out-of-line inline setter
 * with no caller.
 *
 * @param value  nonzero to forward events to the parent
 */
void cFEStateNetHelpPopup::setReferEventsToParent(int value)
{
    mbReferEventsToParent = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetHelpPopup_getReferEventsToParent);
#ifdef SKIP_ASM
/**
 * Returns +0x50 of a network help popup: whether it passes events to its parent state.
 *
 * An out-of-line inline getter with no caller (see cFEStateNetHelpPopup_setReferEventsToParent).
 *
 * @return the forwarding flag
 */
int cFEStateNetHelpPopup::getReferEventsToParent()
{
    return (mbReferEventsToParent);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCharEquip_isInStore);
#ifdef SKIP_ASM
/**
 * Reports whether the equip-gear state is in store (buying) mode: its mode at +0xC8 is kEquipModeStore.
 *
 * An out-of-line inline accessor with no caller; cFEStateCharEquip_cFEStateCharEquip stores kEquipModeStore there in
 * the store.
 *
 * @return true in store mode, else false
 */
bool cFEStateCharEquip::isInStore()
{
    return (mEquipMode) == kEquipModeStore;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCharEquip_isIconsLoaded);
#ifdef SKIP_ASM
/**
 * Returns the equip-gear state's icons-loaded flag (+0xA60).
 *
 * cFEStateCharEquip_onUpdate sets it to 1 once the character's icon file has arrived in the FE file cache. An
 * out-of-line inline getter with no caller.
 *
 * @return nonzero once the icons are loaded
 */
int cFEStateCharEquip::isIconsLoaded()
{
    return (mIconsLoaded);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCharEquip_setCanSwitchEquipMode);
#ifdef SKIP_ASM
/**
 * Stores whether the equip-gear state may switch between store and inventory mode (+0xAE4).
 *
 * The constructor cFEStateCharEquip_cFEStateCharEquip stores its canSwitchMode argument there. An out-of-line inline
 * setter with no caller.
 *
 * @param value  nonzero to allow switching
 */
void cFEStateCharEquip::setCanSwitchEquipMode(int value)
{
    mCanSwitchMode = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCharEquip_canSwitchEquipMode);
#ifdef SKIP_ASM
/**
 * Returns whether the equip-gear state may switch between store and inventory mode (+0xAE4).
 *
 * An out-of-line inline getter with no caller.
 *
 * @return the stored flag
 */
int cFEStateCharEquip::canSwitchEquipMode()
{
    return (mCanSwitchMode);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_cFEStateRewardGalleryBase1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryBase_vtbl[];

/**
 * Constructs a reward-gallery base state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the reward-gallery base vtable cFEStateRewardGalleryBase_vtbl.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryBase::cFEStateRewardGalleryBase1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryBase_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase__cFEStateRewardGalleryBase);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys a reward-gallery state through the base classes.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryBase__cFEStateRewardGalleryBase(cFEStateRewardGalleryBase* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    self->cFEStateRewardGalleryBase::purgeRewards();
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getStoreNum);
#ifdef SKIP_ASM
/**
 * Returns a reward gallery's store number (+0x8F0); negative when the gallery is not a store.
 *
 * An out-of-line inline getter with no caller.
 *
 * @return the store number, or a negative value outside a store
 */
int cFEStateRewardGalleryBase::getStoreNum()
{
    return (mStoreNum);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_isInStore);
#ifdef SKIP_ASM
/**
 * Reports whether a reward gallery is a store: its store number (+0x8F0) is not negative.
 *
 * An out-of-line inline accessor with no caller.
 *
 * @return 1 when the store number is 0 or more, else 0
 */
unsigned int cFEStateRewardGalleryBase::isInStore()
{
    return (unsigned int) ~(mStoreNum) >> 0x1F;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getIconsFileName__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base has no icon file name (vtable entry 25, +0xCC).
 *
 * The gallery classes override it with their icon file's name (posters: cFEStateRewardGalleryPosters_getIconsFileName__FPv); the cheat-character gallery
 * keeps this one.
 *
 * @return 0
 */
int cFEStateRewardGalleryBase_getIconsFileName(void* vself)
{
    cFEStateRewardGalleryBase* self = (cFEStateRewardGalleryBase*)vself;
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_isIconsLoaded);
#ifdef SKIP_ASM
/**
 * Reports whether a reward gallery's icon file has finished loading (bit 3 of the flags at +0x48).
 *
 * cFEStateRewardGalleryBase_updateIconsLoad sets the bit when the icon file has arrived; cFEStateRewardGalleryBase_loadIcons sets it right away when there is no icon
 * file. An out-of-line inline accessor with no caller.
 *
 * @return 1 when the icons are loaded, else 0
 */
int cFEStateRewardGalleryBase::isIconsLoaded()
{
    return ((int) (mLoadingState) >> 3) & 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getIconID1__FPv);
#ifdef SKIP_ASM
/**
 * Returns -1: the reward-gallery base finds no icon for a reward (vtable entry 26, +0xD4).
 *
 * The poster-style galleries override it with a lookup by the reward's icon name (cFEStateRewardGalleryPosters_getIconID).
 *
 * @return -1
 */
int cFEStateRewardGalleryBase::getIconID1()
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getTextureElement__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base has no texture element for a reward (vtable entry 27, +0xDC).
 *
 * The cheat-character gallery overrides it (cFEStateRewardGalleryCheatChars_getTextureElement looks the reward's texture up in the engine's texture bank).
 *
 * @return 0 (no texture element)
 */
const cTextureElement* cFEStateRewardGalleryBase::getTextureElement(int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_isRewardsLoaded);
#ifdef SKIP_ASM
/**
 * Reports whether a reward gallery's reward list has been built (bit 1 of the flags at +0x48).
 *
 * The gallery code sets bit 1 (clearing bit 0, loading) once the rewards are allocated. An out-of-line inline accessor
 * with no caller.
 *
 * @return 1 when the rewards are loaded, else 0
 */
int cFEStateRewardGalleryBase::isRewardsLoaded()
{
    return ((int) (mLoadingState) >> 1) & 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_allocRewards__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the reward-gallery base builds no reward list (vtable entry 28, +0xE4).
 *
 * Each gallery class overrides it with the code that fills its reward array at +0xA28 (posters: cFEStateRewardGalleryPosters_allocRewards).
 */
void cFEStateRewardGalleryBase::allocRewards()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_purgeRewards__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the reward-gallery base has no reward list to free (vtable entry 29, +0xEC).
 *
 * The gallery classes override it (posters: cFEStateRewardGalleryPosters_purgeRewards clears the list); the destructors call this base version
 * directly.
 */
void cFEStateRewardGalleryBase::purgeRewards()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getReward__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base has no reward to return (vtable entry 34, +0x114).
 *
 * @return 0
 */
const void* cFEStateRewardGalleryBase::getReward(int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getRewardCost__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base gives no reward a price (vtable entry 35, +0x11C).
 *
 * @return 0
 */
int cFEStateRewardGalleryBase::getRewardCost(const void*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getRewardStoreNum__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base puts every reward in store 0 (vtable entry 36, +0x124).
 *
 * @return 0
 */
int cFEStateRewardGalleryBase::getRewardStoreNum(const void*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryBase_getRewardName__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the reward-gallery base has no reward names (vtable entry 37, +0x12C).
 *
 * @return 0 (no name)
 */
const char* cFEStateRewardGalleryBase::getRewardName(const void*)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryPosters_vtbl[];

/**
 * Constructs the posters gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the posters gallery's vtable cFEStateRewardGalleryPosters_vtbl; the full
 * constructor (cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters, reward kind 3) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryPosters::cFEStateRewardGalleryPosters1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryPosters_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters__cFEStateRewardGalleryPosters);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the posters gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryPosters__cFEStateRewardGalleryPosters(cFEStateRewardGalleryPosters* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the posters gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryPosters_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryPosters::getReward(int i)
{
    return mpPosters[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a posters gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryPosters_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryPosters_getRewardCost(cFEStateRewardGalleryPosters* self, const tPosterInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a posters gallery reward: its tPosterInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryPosters_vtbl.
 *
 * @param other  the reward record (a tPosterInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryPosters::getRewardStoreNum(const void* other)
{
    tPosterInfo* poster = (tPosterInfo*)other;
    return poster->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryPosters_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a posters gallery reward: its tPosterInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryPosters_vtbl.
 *
 * @param other  the reward record (a tPosterInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryPosters::getRewardName(const void* other)
{
    tPosterInfo* poster = (tPosterInfo*)other;
    return poster->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryTradingCards_vtbl[];

/**
 * Constructs the trading-card gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the trading-card gallery's vtable cFEStateRewardGalleryTradingCards_vtbl; the
 * full constructor (cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards, reward kind 4) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryTradingCards::cFEStateRewardGalleryTradingCards1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryTradingCards_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards__cFEStateRewardGalleryTradingCards);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the trading-card gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryTradingCards__cFEStateRewardGalleryTradingCards(cFEStateRewardGalleryTradingCards* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the trading-card gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryTradingCards_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryTradingCards::getReward(int i)
{
    return mpTradingCards[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a trading-card gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryTradingCards_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryTradingCards_getRewardCost(cFEStateRewardGalleryTradingCards* self,
        const tTradingCardInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a trading-card gallery reward: its tTradingCardInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryTradingCards_vtbl.
 *
 * @param other  the reward record (a tTradingCardInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryTradingCards::getRewardStoreNum(const void* other)
{
    tTradingCardInfo* card = (tTradingCardInfo*)other;
    return card->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryTradingCards_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a trading-card gallery reward: its tTradingCardInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryTradingCards_vtbl.
 *
 * @param other  the reward record (a tTradingCardInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryTradingCards::getRewardName(const void* other)
{
    tTradingCardInfo* card = (tTradingCardInfo*)other;
    return card->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryConceptArts_vtbl[];

/**
 * Constructs the concept-art gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the concept-art gallery's vtable cFEStateRewardGalleryConceptArts_vtbl; the full
 * constructor (cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts, reward kind 5) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryConceptArts::cFEStateRewardGalleryConceptArts1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryConceptArts_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts__cFEStateRewardGalleryConceptArts);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the concept-art gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryConceptArts__cFEStateRewardGalleryConceptArts(cFEStateRewardGalleryConceptArts* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the concept-art gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryConceptArts_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryConceptArts::getReward(int i)
{
    return mpConceptArts[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a concept-art gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryConceptArts_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryConceptArts_getRewardCost(cFEStateRewardGalleryConceptArts* self,
        const tConceptArtInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a concept-art gallery reward: its tConceptArtInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryConceptArts_vtbl.
 *
 * @param other  the reward record (a tConceptArtInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryConceptArts::getRewardStoreNum(const void* other)
{
    tConceptArtInfo* art = (tConceptArtInfo*)other;
    return art->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryConceptArts_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a concept-art gallery reward: its tConceptArtInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryConceptArts_vtbl.
 *
 * @param other  the reward record (a tConceptArtInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryConceptArts::getRewardName(const void* other)
{
    tConceptArtInfo* art = (tConceptArtInfo*)other;
    return art->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryVideos_vtbl[];

/**
 * Constructs the video gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the video gallery's vtable cFEStateRewardGalleryVideos_vtbl; the full
 * constructor (cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos, reward kind 6) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryVideos::cFEStateRewardGalleryVideos1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryVideos_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos__cFEStateRewardGalleryVideos);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the video gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryVideos__cFEStateRewardGalleryVideos(cFEStateRewardGalleryVideos* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the video gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryVideos_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryVideos::getReward(int i)
{
    return mpVideos[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a video gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryVideos_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryVideos_getRewardCost(cFEStateRewardGalleryVideos* self, const tVideoInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a video gallery reward: its tVideoInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryVideos_vtbl.
 *
 * @param other  the reward record (a tVideoInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryVideos::getRewardStoreNum(const void* other)
{
    tVideoInfo* video = (tVideoInfo*)other;
    return video->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryVideos_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a video gallery reward: its tVideoInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryVideos_vtbl.
 *
 * @param other  the reward record (a tVideoInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryVideos::getRewardName(const void* other)
{
    tVideoInfo* video = (tVideoInfo*)other;
    return video->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys_cFEStateRewardGalleryToys1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryToys_vtbl[];

/**
 * Constructs the toy gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the toy gallery's vtable cFEStateRewardGalleryToys_vtbl; the full
 * constructor (cFEStateRewardGalleryToys_cFEStateRewardGalleryToys, reward kind 7) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryToys::cFEStateRewardGalleryToys1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryToys_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys__cFEStateRewardGalleryToys);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the toy gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryToys__cFEStateRewardGalleryToys(cFEStateRewardGalleryToys* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the toy gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryToys_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryToys::getReward(int i)
{
    return mpToys[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a toy gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryToys_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryToys_getRewardCost(cFEStateRewardGalleryToys* self, const tToyInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a toy gallery reward: its tToyInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryToys_vtbl.
 *
 * @param other  the reward record (a tToyInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryToys::getRewardStoreNum(const void* other)
{
    tToyInfo* toy = (tToyInfo*)other;
    return toy->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryToys_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a toy gallery reward: its tToyInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryToys_vtbl.
 *
 * @param other  the reward record (a tToyInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryToys::getRewardName(const void* other)
{
    tToyInfo* toy = (tToyInfo*)other;
    return toy->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardGalleryCheatChars_vtbl[];

/**
 * Constructs the cheat-character gallery state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores the cheat-character gallery's vtable cFEStateRewardGalleryCheatChars_vtbl; the
 * full constructor (cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars, reward kind 8) also builds the gallery base and clears the reward list.
 *
 * @return @p self
 */
void* cFEStateRewardGalleryCheatChars::cFEStateRewardGalleryCheatChars1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardGalleryCheatChars_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars__cFEStateRewardGalleryCheatChars);
#ifdef SKIP_ASM
extern void* cFEStateRewardGalleryBase_vtbl[];
extern void* cFEState_vtbl[];
void cFEStateRewardGalleryBase_purgeRewards(void* self);
extern "C" void cFEStateRewardGalleryBase_purgeIcons(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the cheat-character gallery state through the gallery base and its bases.
 *
 * Stores the gallery base vtable cFEStateRewardGalleryBase_vtbl, runs the base's purgeRewards (cFEStateRewardGalleryBase_purgeRewards, empty) and releases the icon
 * textures (cFEStateRewardGalleryBase_purgeIcons), then stores the cFEState vtable cFEState_vtbl and calls the cUIState destructor cUIState__cUIState
 * with @p flags. The same code as cFEStateRewardGalleryBase__cFEStateRewardGalleryBase (the base destructor inlined).
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFEStateRewardGalleryCheatChars__cFEStateRewardGalleryCheatChars(cFEStateRewardGalleryCheatChars* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateRewardGalleryBase_vtbl;
    cFEStateRewardGalleryBase_purgeRewards(self);
    cFEStateRewardGalleryBase_purgeIcons(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars_getReward__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the cheat-character gallery's reward @p i: entry @p i of its reward array at +0xA28.
 *
 * Vtable entry 34 (+0x114) of cFEStateRewardGalleryCheatChars_vtbl; no bounds check.
 *
 * @param i  the reward index
 * @return the reward record
 */
const void* cFEStateRewardGalleryCheatChars::getReward(int i)
{
    return mpCheatChars[i];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars_getRewardCost);
#ifdef SKIP_ASM
/**
 * Returns the price of a cheat-character gallery reward: its mCost, times 10 when positive.
 *
 * Vtable entry 35 (+0x11C) of cFEStateRewardGalleryCheatChars_vtbl; zero and negative values are returned unchanged.
 *
 * @param reward  the reward record
 * @return the price
 */
extern "C" int cFEStateRewardGalleryCheatChars_getRewardCost(cFEStateRewardGalleryCheatChars* self,
        const tCheatCharInfo* reward)
{
    int value = reward->mCost;
    if (value > 0) value = value * 10;
    return value;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars_getRewardStoreNum__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the store number of a cheat-character gallery reward: its tCheatCharInfo's mStoreNum (a signed byte).
 *
 * Vtable entry 36 (+0x124) of cFEStateRewardGalleryCheatChars_vtbl.
 *
 * @param other  the reward record (a tCheatCharInfo)
 * @return the store the reward is sold in
 */
int cFEStateRewardGalleryCheatChars::getRewardStoreNum(const void* other)
{
    tCheatCharInfo* cheatChar = (tCheatCharInfo*)other;
    return cheatChar->mStoreNum;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardGalleryCheatChars_getRewardName__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the name of a cheat-character gallery reward: its tCheatCharInfo's mNameStringID.
 *
 * Vtable entry 37 (+0x12C) of cFEStateRewardGalleryCheatChars_vtbl.
 *
 * @param other  the reward record (a tCheatCharInfo)
 * @return the reward's name
 */
const char* cFEStateRewardGalleryCheatChars::getRewardName(const void* other)
{
    tCheatCharInfo* cheatChar = (tCheatCharInfo*)other;
    return cheatChar->mNameStringID;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePreviewReward_cFEStatePreviewReward1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStatePreviewReward_vtbl[];

/**
 * Constructs a reward-preview state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStatePreviewReward_vtbl. The full constructor is
 * cFEStatePreviewReward_cFEStatePreviewReward.
 *
 * @return @p self
 */
void* cFEStatePreviewReward::cFEStatePreviewReward1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStatePreviewReward_vtbl;
    return ((void*)this);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePreviewReward_isRewardHorizontal);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * An out-of-line copy with no caller and no table entry; nothing in it shows which class it belongs to.
 *
 * @return 0
 */
extern "C" int cFEStatePreviewReward_isRewardHorizontal(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePreviewReward_isRewardVertical);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * An out-of-line copy with no caller and no table entry; nothing in it shows which class it belongs to.
 *
 * @return 0
 */
extern "C" int cFEStatePreviewReward_isRewardVertical(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePreviewReward_isRewardSquare);
#ifdef SKIP_ASM
/**
 * Returns 1.
 *
 * An out-of-line copy with no caller and no table entry; nothing in it shows which class it belongs to.
 *
 * @return 1
 */
extern "C" int cFEStatePreviewReward_isRewardSquare(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Lets the video-player state's gaining transition finish at once (vtable entry 7, onGainTransition).
 *
 * Entry 7 of cFEStateVideoPlayer_vtbl, the vtable cFEStateVideoPlayer_cFEStateVideoPlayer stores for the video-player state (video and audio file names at
 * +0x48 and +0x148, movie player at +0x248).
 *
 * @return true (transition done)
 */
bool cFEStateVideoPlayer::onGainTransition(int)
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Lets the video-player state's leaving transition finish at once (vtable entry 10, onLoseTransition).
 *
 * Entry 10 of cFEStateVideoPlayer_vtbl, the video-player state's vtable.
 *
 * @return true (transition done)
 */
bool cFEStateVideoPlayer::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_setInterruptible);
#ifdef SKIP_ASM
/**
 * Stores whether the video-player state's movie may be interrupted (+0x270).
 *
 * The constructor cFEStateVideoPlayer_cFEStateVideoPlayer sets it to 1; while it is set, the input handler cFEStateVideoPlayer_onInputBegin stops the movie when the
 * input's entry 5 or 6 answers nonzero. An out-of-line inline setter with no caller.
 *
 * @param value  nonzero to let input stop the movie
 */
void cFEStateVideoPlayer::setInterruptible(int value)
{
    mInterruptible = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_setFullScreen);
#ifdef SKIP_ASM
/**
 * Stores the full-screen option of the video-player state's movie (+0x258, in the play options at +0x24C).
 *
 * An out-of-line inline setter with no caller.
 *
 * @param value  nonzero for full screen
 */
void cFEStateVideoPlayer::setFullScreen(int value)
{
    mOptions.bFullScreen = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_setSound);
#ifdef SKIP_ASM
/**
 * Stores the sound option of the video-player state's movie (+0x254, in the play options at +0x24C).
 *
 * An out-of-line inline setter with no caller. The start routine cFEStateVideoPlayer_Start also sets the word to 1.
 *
 * @param value  nonzero to play the movie's sound
 */
void cFEStateVideoPlayer::setSound(int value)
{
    mOptions.bSoundOn = value;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateVideoPlayer_setOptions);
#ifdef SKIP_ASM
/**
 * Copies a whole set of movie play options (0x24 bytes) into the video-player state at +0x24C.
 *
 * cFEStateVideoPlayer_Start hands that block to the movie player when the video starts. An out-of-line inline setter with no
 * caller.
 *
 * @param src  the options to copy (nine words)
 */
void cFEStateVideoPlayer::setOptions(const tMoviePlayOptions* src)
{
    mOptions = *src;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMountainRoom_cFEStateMountainRoom1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateMountainRoom_vtbl[];

/**
 * Constructs a mountain-room state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateMountainRoom_vtbl; the full constructor is
 * cFEStateMountainRoom_cFEStateMountainRoom.
 *
 * @return @p self
 */
void* cFEStateMountainRoom::cFEStateMountainRoom1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateMountainRoom_vtbl;
    return ((void*)this);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMountainRoom_isPeakPassLoaded);
#ifdef SKIP_ASM
/**
 * Reports whether the mountain room has no peak-pass load outstanding: its load slot (+0xBC) is -1.
 *
 * The room starts the pass image load in its constructor and resets the slot to -1 once the image is taken
 * (cFEStateMountainRoom_updateTextureLoad). An out-of-line inline accessor with no caller.
 *
 * @return true when the slot is -1, else false
 */
bool cFEStateMountainRoom::isPeakPassLoaded()
{
    return ~(mTextureFileIndex) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMountainRoom_hidePass);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Hides the mountain room's peak-pass group (the widget at +0x48).
 *
 * Calls its setVisible with 0, as cFEStateMountainRoom_onInputBegin does to close the pass view. No null check.
 */
void cFEStateMountainRoom::hidePass()
{
    ((cUIThing*)mpPassGroup)->setVisible(0);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMountainRoom_isPassVisible);
#ifdef SKIP_ASM
/**
 * Reports whether the mountain room's peak-pass group (mpPassGroup) is visible.
 *
 * Reads the mVisible bit (bit 6) of the widget's mAttr; 0 when there is no widget. An out-of-line inline accessor with
 * no caller.
 *
 * @return 1 when the pass is shown, else 0
 */
int cFEStateMountainRoom::isPassVisible()
{
    int var_2;
    cUIGroup* temp_3;

    temp_3 = mpPassGroup;
    var_2 = 0;
    if (temp_3 != 0) {
        var_2 = (temp_3->mAttr >> 6) & 1;
    }
    return var_2;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMountainRoom_isPassFrontVisible);
#ifdef SKIP_ASM
/**
 * Reports whether both the mountain room's peak-pass group (mpPassGroup) and its pass picture (mpPassFrontGroup) are
 * visible.
 *
 * Each check reads the widget's mVisible bit (bit 6 of mAttr) and is 0 for a missing widget.
 * cFEStateMountainRoom_updateTextureLoad makes the same test before showing the pass. An out-of-line inline accessor
 * with no caller.
 *
 * @return 1 when both are shown, else 0
 */
int cFEStateMountainRoom::isPassFrontVisible()
{
    int frontVisible;
    int groupVisible;
    cUIGroup* group;
    cUIGroup* front;

    // each group's isVisible(): tAttrThing's mVisible, bit 6 of the mAttr word
    group = mpPassGroup;
    frontVisible = 0;
    groupVisible = 0;
    if (group != 0) {
        groupVisible = (group->mAttr >> 6) & 1;
    }
    if (groupVisible != 0) {
        front = mpPassFrontGroup;
        if (front != 0) {
            frontVisible = (front->mAttr >> 6) & 1;
        }
    }
    return frontVisible;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePeakRoom_cFEStatePeakRoom1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStatePeakRoom_vtbl[];

/**
 * Constructs a peak-room state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStatePeakRoom_vtbl.
 *
 * @return @p self
 */
void* cFEStatePeakRoom::cFEStatePeakRoom1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStatePeakRoom_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStatePeakRoom__cFEStatePeakRoom__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the peak-room front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStatePeakRoom__cFEStatePeakRoom(void* vself)
{
    cFEStatePeakRoom* self = (cFEStatePeakRoom*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateTrophyRoom_cFEStateTrophyRoom1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateTrophyRoom_vtbl[];

/**
 * Constructs a trophy-room state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateTrophyRoom_vtbl.
 *
 * @return @p self
 */
void* cFEStateTrophyRoom::cFEStateTrophyRoom1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateTrophyRoom_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateTrophyRoom__cFEStateTrophyRoom);
#ifdef SKIP_ASM
extern void* cFEStateTrophyRoom_vtbl[];
extern void* cFEState_vtbl[];
extern "C" void cFEStateTrophyRoom_purgeTextures(void* self);
// PORT: unit declares cUIState__cUIState with one arg; the body takes (self, flags)
extern "C" void cUIState__cUIState_dtor(void* self, int flags) __asm__("cUIState__cUIState");

/**
 * Destroys the trophy-room front-end state: releases its five loaded images (cFEStateTrophyRoom_purgeTextures), then the base
 * states.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFEStateTrophyRoom__cFEStateTrophyRoom(cFEStateTrophyRoom* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateTrophyRoom_vtbl;
    cFEStateTrophyRoom_purgeTextures(self);
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    cUIState__cUIState_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardsRoom_cFEStateRewardsRoom1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRewardsRoom_vtbl[];

/**
 * Constructs a rewards-room state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateRewardsRoom_vtbl. The full constructor is
 * cFEStateRewardsRoom_cFEStateRewardsRoom.
 *
 * @return @p self
 */
void* cFEStateRewardsRoom::cFEStateRewardsRoom1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRewardsRoom_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardsRoom__cFEStateRewardsRoom__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the rewards-room front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRewardsRoom__cFEStateRewardsRoom(void* vself)
{
    cFEStateRewardsRoom* self = (cFEStateRewardsRoom*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

extern void* cFEPopupSelectMultiplayerMode_vtbl[];
extern "C" void* cUIState__cUIState(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRewardsRoom_isInStore);
#ifdef SKIP_ASM
/**
 * Reports whether the rewards room was opened as a store: its flag at +0x4C.
 *
 * The constructor cFEStateRewardsRoom_cFEStateRewardsRoom stores its inStore argument there. An out-of-line inline accessor with no caller.
 *
 * @return the in-store flag
 */
int cFEStateRewardsRoom::isInStore()
{
    return (mInStore);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode__cFEPopupSelectMultiplayerMode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the multiplayer-mode selection popup through the UI-state destructor cUIState__cUIState().
 *
 * Restores cFEPopupSelectMultiplayerMode_vtbl first. Only @p self is declared: the destructor flags the caller
 * passed in the second argument register reach cUIState__cUIState() unchanged.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
void* cFEPopupSelectMultiplayerMode__cFEPopupSelectMultiplayerMode(void* vself)
{
    cFEPopupSelectMultiplayerMode* self = (cFEPopupSelectMultiplayerMode*)vself;
    VPTR_OF(cListNode, self) = cFEPopupSelectMultiplayerMode_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_86multiplayermode[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Opens the multiplayer-mode popup ("86multiplayermode").
 */
void cFEPopupSelectMultiplayerMode::onCreateScreen()
{
    int hash = GetHashValue32(str_86multiplayermode);
    mHashName = hash;
    void* screen = cUIEngine_addScreenByHashName(mpEngine, this, hash, 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void cUIMenu_setScrollableMenu(void* self, int a1);
extern char str_menu0000[];
extern char D_004A13A0[];
extern char D_004A14F8[];

/**
 * Sets up the multiplayer-mode popup's widgets: menu0000 becomes a wrapping two-item menu (mAttr bits 0 and 7),
 * items "0" and "1" get mMiscData 0 and 1.
 *
 * @param item  the widget being created (On Tour cUIThing* pWidget)
 */
void cFEPopupSelectMultiplayerMode::onWidgetCreate(cUIThing* item)
{
    int itemHash = item->mHashName;
    if (itemHash == GetHashValue32(str_menu0000)) {
        item->mAttr |= 1;
        cUIMenu_setScrollableMenu(item, 2);
        item->mAttr |= 0x80;
        return;
    }
    int itemHash2 = item->mHashName;
    if (itemHash2 == GetHashValue32(D_004A13A0)) {
        item->mMiscData = 0;
        return;
    }
    int itemHash3 = item->mHashName;
    if (itemHash3 == GetHashValue32(D_004A14F8)) {
        item->mMiscData = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetEvent);
#ifdef SKIP_ASM
extern int D_004A19D4;
extern "C" void cUIStateStack_pop(void* self, int a1);

/**
 * Closes the multiplayer-mode popup: select (kONNEXT) records the chosen mode, back (kONPREV) cancels.
 *
 * Select stores whether item 1 was chosen in D_004A19D4, which the front end checks when it starts the next
 * multiplayer state.
 *
 * @param pWidget  the widget that sent the event
 * @param Event  the widget event
 */
void cFEPopupSelectMultiplayerMode::onWidgetEvent(cUIThing* pWidget, unsigned int Event)
{
    switch (Event) {
    case cUIState::kONNEXT: {
        cUIEngine* engine = mpEngine;
        mMultiMode = 1;
        D_004A19D4 = pWidget->mMiscData != 0;
        cUIStateStack_pop(&engine->mStateStack, 1);
        break;
    }
    case cUIState::kONPREV: {
        cUIEngine* engine = mpEngine;
        mMultiMode = 0;
        cUIStateStack_pop(&engine->mStateStack, 1);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

/**
 * Handles the multiplayer-mode popup's menu events by moving the selection on scroll events.
 *
 * cUIMenu_onInputChangeItem() sends kSCROLLUP when the menu scrolls up and kSCROLLDOWN when it scrolls down.
 * kSCROLLDOWN selects item @p idx + 1; kSCROLLUP selects @p idx - 1 and then, falling through into kSCROLLDOWN's
 * case, @p idx + 1.
 *
 * @param menu  the menu that sent the event
 * @param key   menu event number (On Tour: Event)
 * @param idx   the event's index argument (the menu's scroll offset for kSCROLLUP and kSCROLLDOWN)
 * @return always 0x101, the base handler's result
 */
int cFEPopupSelectMultiplayerMode::onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int idx)
{
    switch (key) {
    default:
        break;
    case cUIMenu::kSCROLLUPDATE:
    case cUIMenu::kSCROLLCHANGE:
    case cUIMenu::kONNEXT:
    case cUIMenu::kONPREVIOUS:
        if (key < cUIMenu::kONNEXT) {
            return 257;
        }
        break;
    case cUIMenu::kSCROLLUP:
        cUIMenu_setSelectedByIndex(menu, idx - 1);
    case cUIMenu::kSCROLLDOWN:
        cUIMenu_setSelectedByIndex(menu, idx + 1);
        break;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_getData__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether a multiplayer mode was chosen in the popup.
 *
 * @return 1 after an entry was chosen (widget event 5), 0 after widget event 6 (probably cancel)
 */
signed char cFEPopupSelectMultiplayerMode_getData(void* vself)
{
    cFEPopupSelectMultiplayerMode* self = (cFEPopupSelectMultiplayerMode*)vself;
    return self->mMultiMode;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the multiplayer-mode selection popup's gain transition at once.
 *
 * The state stack update cUIStateStack_updateLoop() ends a state's gain transition once this returns nonzero.
 *
 * @return always true (transition finished)
 */
bool cFEPopupSelectMultiplayerMode::onGainTransition(int)
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEPopupSelectMultiplayerMode_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the multiplayer-mode selection popup's lose transition at once.
 *
 * The state stack update cUIStateStack_updateLoop() ends a state's lose transition once this returns nonzero.
 *
 * @return always true (transition finished)
 */
bool cFEPopupSelectMultiplayerMode::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAudioOptions_cFEStateAudioOptions1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateAudioOptions_vtbl[];

/**
 * Constructs an audio-options state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateAudioOptions_vtbl; the full constructor is
 * cFEStateAudioOptions_cFEStateAudioOptions.
 *
 * @return @p self
 */
void* cFEStateAudioOptions::cFEStateAudioOptions1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateAudioOptions_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAudioOptions__cFEStateAudioOptions__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the audio-options front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateAudioOptions__cFEStateAudioOptions(void* vself)
{
    cFEStateAudioOptions* self = (cFEStateAudioOptions*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAudioOptions_isInStore);
#ifdef SKIP_ASM
/**
 * Reports whether the audio options were opened from a store: the flag at +0x74.
 *
 * An out-of-line inline accessor with no caller.
 *
 * @return the in-store flag
 */
int cFEStateAudioOptions::isInStore()
{
    return (mInStore);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_cFEStateRequestLine1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateRequestLine_vtbl[];

/**
 * Constructs a request-line (radio playlist) state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateRequestLine_vtbl.
 *
 * @return @p self
 */
void* cFEStateRequestLine::cFEStateRequestLine1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateRequestLine_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine__cFEStateRequestLine__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the request-line front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateRequestLine__cFEStateRequestLine(void* vself)
{
    cFEStateRequestLine* self = (cFEStateRequestLine*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_isInStore);
#ifdef SKIP_ASM
/**
 * Reports whether the request line was opened as a store: the flag at +0x16C.
 *
 * An out-of-line inline accessor with no caller.
 *
 * @return the in-store flag
 */
int cFEStateRequestLine::isInStore()
{
    return (mInStore);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_countBits);
#ifdef SKIP_ASM
/**
 * Counts the set bits of a 64-bit mask.
 *
 * @param requestLine  the request-line state (not used)
 * @param bits    the 64-bit mask
 * @return the number of bits set (0..64)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int cFEStateRequestLine::countBits(unsigned long bits)
{
    int count;
    int i;
    int bit;

    count = 0;
    i = 0;
    do {
        bit = (bits >> i) & 1;
        i += 1;
        count += bit;
    } while (i < 0x40);
    return count;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_selectSong);
#ifdef SKIP_ASM
/**
 * Adds song @p bit to the request line's playlist: sets that bit of the 64-bit mask at +0x158.
 *
 * @param bit  the song index (0..63)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void cFEStateRequestLine::selectSong(int bit)
{
    mPlaylist |= 1L << bit;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_unselectSong);
#ifdef SKIP_ASM
/**
 * Removes song @p bit from the request line's playlist: clears that bit of the 64-bit mask at +0x158.
 *
 * @param bit  the song index (0..63)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void cFEStateRequestLine::unselectSong(int bit)
{
    mPlaylist &= ~(1L << bit);
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_isEmptyPlaylist);
#ifdef SKIP_ASM
/**
 * Reports whether the request line's playlist (the 64-bit mask at +0x158) is empty.
 *
 * @param requestLine  the request-line state
 * @return true when no song is selected, else false
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
bool cFEStateRequestLine::isEmptyPlaylist()
{
    return ((long)mPlaylist) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_isOnlySongInPlaylist);
#ifdef SKIP_ASM
/**
 * Reports whether song @p bit is the only song the playlist could hold: no other bit of the mask at +0x158 is set.
 *
 * Also true for an empty playlist, since the song's own bit is ignored.
 *
 * @param bit  the song index (0..63)
 * @return true when no other song is selected, else false
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
bool cFEStateRequestLine::isOnlySongInPlaylist(int bit)
{
    return ((long)mPlaylist & ~(1L << bit)) == 0;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_isSongSelected);
#ifdef SKIP_ASM
/**
 * Reports whether song @p bit is in the request line's playlist (bit @p bit of the mask at +0x158).
 *
 * @param bit  the song index (0..63)
 * @return 1 when selected, else 0
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int cFEStateRequestLine::isSongSelected(int bit)
{
    return (mPlaylist >> bit) & 1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_getNumSongsInPlaylist);
#ifdef SKIP_ASM
/**
 * Counts the songs in the request line's playlist (the set bits of the 64-bit mask at +0x158).
 *
 * @param requestLine  the request-line state
 * @return the number of selected songs
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int cFEStateRequestLine::getNumSongsInPlaylist()
{
    int temp_2;
    int var_4;
    int var_5;

    var_5 = 0;
    var_4 = 0;
    do {
        temp_2 = ((unsigned long) (mPlaylist) >> var_4) & 1;
        var_4 += 1;
        var_5 += temp_2;
    } while (var_4 < 0x40);
    return var_5;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_isSongOwned);
#ifdef SKIP_ASM
/**
 * Reports whether song @p bit is owned (bit @p bit of the 64-bit mask at +0x160).
 *
 * @param bit  the song index (0..63)
 * @return 1 when owned, else 0
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int cFEStateRequestLine::isSongOwned(int bit)
{
    return (mOwnedSong >> bit) & 1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_ownSong);
#ifdef SKIP_ASM
/**
 * Marks song @p bit as owned: sets that bit of the 64-bit mask at +0x160.
 *
 * @param bit  the song index (0..63)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void cFEStateRequestLine::ownSong(int bit)
{
    mOwnedSong |= 1L << bit;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRequestLine_getNumOwnedSongs);
#ifdef SKIP_ASM
/**
 * Counts the owned songs (the set bits of the 64-bit mask at +0x160).
 *
 * @param requestLine  the request-line state
 * @return the number of owned songs
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
int cFEStateRequestLine::getNumOwnedSongs()
{
    int temp_2;
    int var_4;
    int var_5;

    var_5 = 0;
    var_4 = 0;
    do {
        temp_2 = (mOwnedSong >> var_4) & 1;
        var_4 += 1;
        var_5 += temp_2;
    } while (var_4 < 0x40);
    return var_5;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFESound__cFESound);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUISoundInterface_vtbl[];

/**
 * Destroys the front end's sound interface (vtable cFESound_vtbl), freeing it when asked.
 *
 * Puts back the base sound-interface vtable cUISoundInterface_vtbl and calls operator_delete when bit 0 of @p flags is set. The
 * object is built by cFESound_cFESound (fe/fereal); its play routine is cFESound_playSound.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFESound__cFESound(cFESound* self, int flags)
{
    *(void***)self = cUISoundInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFESound_soundInitialize__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the front end's sound interface needs no set-up (entry 3 of cFESound_vtbl).
 */
void cFESound::soundInitialize()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFESound_soundShutdown__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the front end's sound interface has nothing to shut down (entry 4 of cFESound_vtbl).
 */
void cFESound::soundShutdown()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound__cOVSound);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUISoundInterface_vtbl[];

/**
 * Destroys the overlay sound interface, freeing it when asked.
 *
 * Puts back cUISoundInterface_vtbl, the base sound-interface vtable (On Tour cUISoundInterface; its own destructor cUISoundInterface__cUISoundInterface
 * stores the same table), and calls operator_delete when bit 0 of flags is set. cOVSound owns nothing to release
 * (On Tour: only mDummyVar at +0x4).
 *
 * @param flags  g++ 2.95 destructor flags; bit 0 = free the object
 */
extern "C" void cOVSound__cOVSound(cOVSound* self, int flags)
{
    *(void***)self = cUISoundInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound_soundInitialize__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the overlay sound interface needs no set-up.
 *
 * Empty, like On Tour's cOVSound::soundInitialize. Vtable slot 3 of cOVSound_vtbl, after cOVSound_playSound (slot 2, which
 * plays the menu sounds) and before cOVSound_soundShutdown.
 */
void cOVSound::soundInitialize()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVSound_soundShutdown__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the overlay sound interface has nothing to shut down.
 *
 * Empty, like On Tour's cOVSound::soundShutdown. Vtable slot 4 of cOVSound_vtbl, the last one.
 */
void cOVSound::soundShutdown()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateSelectMultiplayerMode_vtbl[];

/**
 * Constructs a select-multiplayer-mode state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateSelectMultiplayerMode_vtbl.
 *
 * @return @p self
 */
void* cFEStateSelectMultiplayerMode::cFEStateSelectMultiplayerMode1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateSelectMultiplayerMode_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectMultiplayerMode__cFEStateSelectMultiplayerMode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the select-multiplayer-mode front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateSelectMultiplayerMode__cFEStateSelectMultiplayerMode(void* vself)
{
    cFEStateSelectMultiplayerMode* self = (cFEStateSelectMultiplayerMode*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMPCircuit_cFEStateMPCircuit1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern void* cFEStateMPCircuit_vtbl[];

/**
 * Constructs a multiplayer-circuit state with no engine (the default-constructor copy).
 *
 * Runs the cUIState constructor cUIState_cUIState(self, 0) and stores cFEStateMPCircuit_vtbl.
 *
 * @return @p self
 */
void* cFEStateMPCircuit::cFEStateMPCircuit1()
{
    cUIState_cUIState(this, 0);
    VPTR_OF(cListNode, this) = cFEStateMPCircuit_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateMPCircuit__cFEStateMPCircuit__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the multiplayer-circuit front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(); the g++
 * delete
 * flags reach it untouched in the second argument register.
 *
 * @return meaningless (the void base destructor's leftover return register)
 */
void* cFEStateMPCircuit__cFEStateMPCircuit(void* vself)
{
    cFEStateMPCircuit* self = (cFEStateMPCircuit*)vself;
    VPTR_OF(cListNode, self) = cFEState_vtbl;
    return cUIState__cUIState(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEUseExistingAccount_cFEUseExistingAccount1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEUseExistingAccount_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs the use-existing-account state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then
 * installs cFEUseExistingAccount_vtbl, sets mszEmail to cBXString_EmptyString and clears unk_6D0 and
 * mpTextForgotPassword.
 *
 * @return @p self
 */
void* cFEUseExistingAccount::cFEUseExistingAccount1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyString = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEUseExistingAccount_vtbl;
    mszEmail = emptyString;
    unk_6D0 = 0;
    mpTextForgotPassword = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEUseExistingAccount_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the use-existing-account state publishes the player's buddy presence as offline.
 *
 * Entry 29 (+0xEC) of cFEUseExistingAccount_vtbl; cFEStateNet's cFEStateNet_publishPresence asks this entry (func_001DE870, 1, by
 * default) whether to show the player online.
 *
 * @return 0
 */
int cFEUseExistingAccount::isOnlineState()
{
    return 0;
}
#endif

extern void* cFEStateSelectPersona_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona_cFEStateSelectPersona1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the persona selection state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateSelectPersona_vtbl.
 *
 * @return @p self
 */
void * cFEStateSelectPersona::cFEStateSelectPersona1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateSelectPersona_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona__cFEStateSelectPersona__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the persona-selection state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateSelectPersona__cFEStateSelectPersona(void* vself)
{
    cFEStateSelectPersona* self = (cFEStateSelectPersona*)vself;
    VPTR_OF(cListNode, self) = cFEStateSelectPersona_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the select-persona state publishes the player's buddy presence as offline.
 *
 * Entry 29 (+0xEC) of cFEStateSelectPersona_vtbl; cFEStateNet's cFEStateNet_publishPresence asks this entry (func_001DE870, 1, by
 * default) whether to show the player online.
 *
 * @return 0
 */
int cFEStateSelectPersona::isOnlineState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateSelectPersona_publishPresence__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the select-persona state does not publish the player's buddy presence when it gains focus.
 *
 * Entry 32 (+0x104) of cFEStateSelectPersona_vtbl, replacing cFEStateNet's cFEStateNet_publishPresence, which cFEStateNet_onGainFocus
 * calls.
 */
void cFEStateSelectPersona_publishPresence(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup_cFEStateChallengePopup1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateChallengePopup_vtbl[];
extern int D_00441268[];

/**
 * Constructs the challenge popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateChallengePopup_vtbl, points mpEventOptions at the default table D_00441268 and clears mbChallengeSent.
 *
 * @return @p self
 */
void* cFEStateChallengePopup::cFEStateChallengePopup1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateChallengePopup_vtbl;
    mpEventOptions = D_00441268;
    mbChallengeSent = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup__cFEStateChallengePopup);
#ifdef SKIP_ASM

extern void* cFEStateChallengePopup_vtbl[];
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");

/**
 * Destroys the challenge popup: calls its parent state's slot 32, then runs the cFEStateNet destructor.
 *
 * Slot 32 is the same hook the popup's own onGainTransition calls on itself, so the parent refreshes as the popup
 * goes away.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateChallengePopup__cFEStateChallengePopup(cFEStateChallengePopup* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateChallengePopup_vtbl;
    cUIState* obj = self->mpParentState;
    sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)VPTR_OF(cListNode, obj);
    vt[32].fn(vthis(obj, vt[32]));
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengePopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the challenge popup: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateChallengePopup::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateChallengeReceivedPopup_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs the challenge-received popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then
 * installs cFEStateChallengeReceivedPopup_vtbl and sets mszChallenger to cBXString_EmptyString.
 *
 * @return @p self
 */
void* cFEStateChallengeReceivedPopup::cFEStateChallengeReceivedPopup1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyStr = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateChallengeReceivedPopup_vtbl;
    mszChallenger = emptyStr;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup);
#ifdef SKIP_ASM
extern void* cFEStateChallengeReceivedPopup_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");

/**
 * Destroys the challenge-received popup: calls its parent state's slot 32, frees the string at +0x6D0, then runs the
 * cFEStateNet destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup(cFEStateChallengeReceivedPopup* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateChallengeReceivedPopup_vtbl;
    cUIState* obj = self->mpParentState;
    sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)VPTR_OF(cListNode, obj);
    vt[32].fn(vthis(obj, vt[32]));
    cBXString__cBXString((char*)&self->mszChallenger, 2);
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateChallengeReceivedPopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the challenge-received popup: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateChallengeReceivedPopup::onLoseTransition()
{
    return true;
}
#endif

extern void* cFEStateOnlineCharacterSelect_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the online character select without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then clears
 * mbConfirmed and installs cFEStateOnlineCharacterSelect_vtbl.
 *
 * @return @p self
 */
void * cFEStateOnlineCharacterSelect::cFEStateOnlineCharacterSelect1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    mbConfirmed = 0;
    VPTR_OF(cListNode, this) = cFEStateOnlineCharacterSelect_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the online character-select state: restores its vtable and runs the cFEStateNet destructor
 * (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect(void* vself)
{
    cFEStateOnlineCharacterSelect* self = (cFEStateOnlineCharacterSelect*)vself;
    VPTR_OF(cListNode, self) = cFEStateOnlineCharacterSelect_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetConfig_cFEStateNetConfig1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateNetConfig_vtbl[];

/**
 * Constructs the network configuration state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then
 * installs cFEStateNetConfig_vtbl and clears its fields: miSelectedSetting, miNumSettings, mbCancelled, mbPushLogin,
 * mpLBAdapters, mpLBSettings, unk_6F0, mpMenuConfig, mpConfigRectangle, mpNoConfiguration, mbAuthFailed,
 * mpustrPopupText and the three mpustrAuthError entries.
 *
 * @return @p self
 */
void* cFEStateNetConfig::cFEStateNetConfig1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    miSelectedSetting = 0;
    VPTR_OF(cListNode, this) = cFEStateNetConfig_vtbl;
    miNumSettings = 0;
    mbCancelled = 0;
    mbPushLogin = 0;
    mpLBAdapters = 0;
    mpLBSettings = 0;
    unk_6F0 = 0;
    mpMenuConfig = 0;
    mpConfigRectangle = 0;
    mpNoConfiguration = 0;
    mbAuthFailed = 0;
    mpustrPopupText = 0;
    mpustrAuthError[0] = 0;
    mpustrAuthError[1] = 0;
    mpustrAuthError[2] = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetConfig_publishPresence__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the network-configuration state does not publish the player's buddy presence when it gains focus.
 *
 * Entry 32 (+0x104) of cFEStateNetConfig_vtbl, replacing cFEStateNet's cFEStateNet_publishPresence, which cFEStateNet_onGainFocus
 * calls.
 */
void cFEStateNetConfig_publishPresence(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetConfig_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the network-configuration state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateNetConfig_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateNetConfig::isOnlineState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_cFEStateAuthDNAS1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateAuthDNAS_vtbl[];

/**
 * Constructs the DNAS authentication state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then
 * installs cFEStateAuthDNAS_vtbl, clears mpAuth, mpTextAuth, miState and mpustrText, and sets unk_6F4 to 2 and unk_6F8
 * to 30.
 *
 * @return @p self
 */
void* cFEStateAuthDNAS::cFEStateAuthDNAS1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateAuthDNAS_vtbl;
    mpAuth = 0;
    mpTextAuth = 0;
    miState = 0;
    unk_6F4 = 2;
    unk_6F8 = 0x1E;
    mpustrText = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Opening transition of the DNAS authentication state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateAuthDNAS_onGainTransition(void* vself)
{
    cFEStateAuthDNAS* self = (cFEStateAuthDNAS*)vself;
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the DNAS authentication state: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateAuthDNAS::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_publishPresence__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the DNAS authentication state does not publish the player's buddy presence when it gains focus.
 *
 * Entry 32 (+0x104) of cFEStateAuthDNAS_vtbl, replacing cFEStateNet's cFEStateNet_publishPresence, which cFEStateNet_onGainFocus
 * calls.
 */
void cFEStateAuthDNAS_publishPresence(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNAS_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the DNAS authentication state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateAuthDNAS_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateAuthDNAS::isOnlineState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_cFEStateAuthDNASError1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateAuthDNASError_vtbl[];

/**
 * Constructs the DNAS authentication error popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then
 * installs cFEStateAuthDNASError_vtbl and clears mpustrTitle, mpustrBody and mpustrFooter.
 *
 * @return @p self
 */
void* cFEStateAuthDNASError::cFEStateAuthDNASError1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    mpustrBody = 0;
    VPTR_OF(cListNode, this) = cFEStateAuthDNASError_vtbl;
    mpustrFooter = 0;
    mpustrTitle = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the DNAS error state: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateAuthDNASError::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_publishPresence__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the DNAS error state does not publish the player's buddy presence when it gains focus.
 *
 * Entry 32 (+0x104) of cFEStateAuthDNASError_vtbl, replacing cFEStateNet's cFEStateNet_publishPresence, which cFEStateNet_onGainFocus
 * calls.
 */
void cFEStateAuthDNASError_publishPresence(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateAuthDNASError_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the DNAS error state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateAuthDNASError_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateAuthDNASError::isOnlineState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetAccountMem_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the account memory-card popup's gain-focus transition at once.
 *
 * @return always 1 (the transition is done)
 */
int cFEStateNetAccountMem_onGainTransition(void* vself)
{
    cFEStateNetAccountMem* self = (cFEStateNetAccountMem*)vself;
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetAccountMem_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Ends the account memory-card popup's lose-focus transition at once.
 *
 * @return always true (the transition is done)
 */
bool cFEStateNetAccountMem::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep1_cFEStateCreateStep11);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateCreateStep1_vtbl[];
extern char* cBXString_EmptyString;

/**
 * Constructs an account-creation step-1 state with no engine (the default-constructor copy).
 *
 * Runs cFEStateNet_cFEStateNet(self, 0, 0), stores cFEStateCreateStep1_vtbl, points mszSavedPassword, mszSavedConfirm
 * and mszSavedMail at the empty cBXString rep, sets mbEditAccount to 1 and clears mpStaticContinue.
 *
 * @return @p self
 */
void* cFEStateCreateStep1::cFEStateCreateStep11()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyStr = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateCreateStep1_vtbl;
    mszSavedPassword = emptyStr;
    mszSavedConfirm = emptyStr;
    mszSavedMail = emptyStr;
    mbEditAccount = 1;
    mpStaticContinue = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep1__cFEStateCreateStep1);
#ifdef SKIP_ASM
extern void* cFEStateCreateStep1_vtbl[];
extern "C" void cBXString__cBXString(void* self, int flags);
// PORT: unit declares cFEStateNet__cFEStateNet with one arg; the body takes (self, flags)
extern "C" void cFEStateNet__cFEStateNet_dtor(void* self, int flags) __asm__("cFEStateNet__cFEStateNet");

/**
 * Destroys account creation step 1: frees its saved mail, confirmation and password strings, then the cFEStateNet
 * part.
 *
 * Restores cFEStateCreateStep1_vtbl, destroys the cBXStrings mszSavedMail (+0x6DC), mszSavedConfirm (+0x6D8) and
 * mszSavedPassword (+0x6D4) and calls cFEStateNet__cFEStateNet with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory (handled by the base destructor)
 */
extern "C" void cFEStateCreateStep1__cFEStateCreateStep1(cFEStateCreateStep1* self, int flags)
{
    VPTR_OF(cListNode, self) = cFEStateCreateStep1_vtbl;
    cBXString__cBXString((char*)&self->mszSavedMail, 2);
    cBXString__cBXString((char*)&self->mszSavedConfirm, 2);
    cBXString__cBXString((char*)&self->mszSavedPassword, 2);
    cFEStateNet__cFEStateNet_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep1_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the account-creation step-1 state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateCreateStep1_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateCreateStep1::isOnlineState()
{
    return 0;
}
#endif

extern void* cFEStateCreateStep2_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep2_cFEStateCreateStep21);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs an account-creation step-2 state with no engine (the default-constructor copy).
 *
 * Runs cFEStateNet_cFEStateNet(self, 0, 0) and stores cFEStateCreateStep2_vtbl.
 *
 * @return @p self
 */
void * cFEStateCreateStep2::cFEStateCreateStep21()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateCreateStep2_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep2__cFEStateCreateStep2__FPv);
#ifdef SKIP_ASM
/**
 * Destroys account creation step 2: restores cFEStateCreateStep2_vtbl and runs the cFEStateNet destructor.
 *
 * The C passes only @p self to cFEStateNet__cFEStateNet; the destructor flags reach it untouched in $a1 (a port must pass
 * them).
 *
 * @return cFEStateNet__cFEStateNet()'s result (not meaningful; declared void* by the unit)
 */
void* cFEStateCreateStep2__cFEStateCreateStep2(void* vself)
{
    cFEStateCreateStep2* self = (cFEStateCreateStep2*)vself;
    VPTR_OF(cListNode, self) = cFEStateCreateStep2_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateStep2_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the account-creation step-2 state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateCreateStep2_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateCreateStep2::isOnlineState()
{
    return 0;
}
#endif

extern void* cFEStateCreateDuplicate_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateDuplicate_cFEStateCreateDuplicate1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);

/**
 * Constructs the duplicate-account popup without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs
 * cFEStateCreateDuplicate_vtbl.
 *
 * @return @p self
 */
void * cFEStateCreateDuplicate::cFEStateCreateDuplicate1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    VPTR_OF(cListNode, this) = cFEStateCreateDuplicate_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateDuplicate__cFEStateCreateDuplicate__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the duplicate-account state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateCreateDuplicate__cFEStateCreateDuplicate(void* vself)
{
    cFEStateCreateDuplicate* self = (cFEStateCreateDuplicate*)vself;
    VPTR_OF(cListNode, self) = cFEStateCreateDuplicate_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateCreateDuplicate_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the duplicate-account state counts the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateCreateDuplicate_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateCreateDuplicate::isOnlineState()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLogin_cFEStateLogin1);
#ifdef SKIP_ASM
extern "C" void* cFEStateNet_cFEStateNet(void*, int, int);
extern void* cFEStateLogin_vtbl[];

/**
 * Constructs the login state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then clears
 * mpustrDisconnect and installs cFEStateLogin_vtbl.
 *
 * @return @p self
 */
void* cFEStateLogin::cFEStateLogin1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    mpustrDisconnect = 0;
    VPTR_OF(cListNode, this) = cFEStateLogin_vtbl;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLogin_publishPresence__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the login and re-login states do not publish the player's buddy presence when they gain focus.
 *
 * Entry 32 (+0x104) of cFEStateLogin_vtbl and cFEStateRelogin_vtbl, replacing cFEStateNet's cFEStateNet_publishPresence, which
 * cFEStateNet_onGainFocus calls.
 */
void cFEStateLogin_publishPresence(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateLogin_isOnlineState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0, so the login and re-login states count the player as offline for buddy presence.
 *
 * Entry 29 (+0xEC) of cFEStateLogin_vtbl and cFEStateRelogin_vtbl, replacing cFEStateNet's func_001DE870 (returns 1).
 *
 * @return 0
 */
int cFEStateLogin::isOnlineState()
{
    return 0;
}
#endif

extern void* cFEStateStats_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateStats__cFEStateStats__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the stats screen state: restores cFEStateStats_vtbl and runs the cFEStateNet destructor.
 *
 * The C passes only @p self to cFEStateNet__cFEStateNet; the destructor flags reach it untouched in $a1 (a port must pass
 * them).
 *
 * @return cFEStateNet__cFEStateNet()'s result (not meaningful; declared void* by the unit)
 */
void* cFEStateStats__cFEStateStats(void* vself)
{
    cFEStateStats* self = (cFEStateStats*)vself;
    VPTR_OF(cListNode, self) = cFEStateStats_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateStats_onMenuGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when a menu of this state gains the focus (an empty onMenuGainFocus override).
 *
 * On Tour's version also takes the root group (cUIGroup*), which this declaration leaves out.
 */
void cFEStateStats::onMenuGainFocus(cUIGroup*)
{
}
#endif

extern void* cFEStateNetNews_vtbl[];
extern "C" void* cFEStateNet__cFEStateNet(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetNews__cFEStateNetNews__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the server-news state: restores its vtable and runs the cFEStateNet destructor (cFEStateNet__cFEStateNet).
 *
 * The g++ destructor flags are not declared here: they stay in $a1 and reach cFEStateNet__cFEStateNet unchanged, which frees the
 * object when bit 0 is set.
 *
 * @return cFEStateNet__cFEStateNet()'s result (declared void* here)
 */
void* cFEStateNetNews__cFEStateNetNews(void* vself)
{
    cFEStateNet* self = (cFEStateNet*)vself;
    VPTR_OF(cListNode, self) = cFEStateNetNews_vtbl;
    return cFEStateNet__cFEStateNet(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetNews_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the server-news state: nothing to animate, so it reports the transition done at once.
 *
 * @return 1 (done)
 */
int cFEStateNetNews_onLoseTransition(void* self)
{
    return 0x1;
}
#endif

extern void* cFEStateRelogin_vtbl[];
extern "C" void* cFEStateLogin__cFEStateLogin(void*);

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin_cFEStateRelogin1);
#ifdef SKIP_ASM
extern "C" void* cFEStateLogin_cFEStateLogin(void* self, int);
extern void* cFEStateRelogin_vtbl[];

/**
 * Constructs the relogin state without an engine: cFEStateLogin_cFEStateLogin(self, 0), then installs
 * cFEStateRelogin_vtbl (+0x8).
 *
 * @return @p self
 */
extern "C" void* cFEStateRelogin_cFEStateRelogin1(cFEStateLogin* self)
{
    cFEStateLogin_cFEStateLogin(self, 0);
    VPTR_OF(cListNode, self) = cFEStateRelogin_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin__cFEStateRelogin__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the relogin state: installs cFEStateRelogin_vtbl (+0x8) and runs the login state's destructor
 * (cFEStateLogin__cFEStateLogin), which receives the caller's destructor flags in its second register.
 *
 * @return cFEStateLogin__cFEStateLogin's result
 */
void* cFEStateRelogin__cFEStateRelogin(void* vself)
{
    cFEStateLogin* self = (cFEStateLogin*)vself;
    VPTR_OF(cListNode, self) = cFEStateRelogin_vtbl;
    return cFEStateLogin__cFEStateLogin(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateRelogin_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Lets the transition away from the relogin state proceed at once.
 *
 * @return 1
 */
int cFEStateRelogin_onLoseTransition(void* vself)
{
    cFEStateLogin* self = (cFEStateLogin*)vself;
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_cFEStateNetMessenger1);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* cFEStateNet_cFEStateNet(void* self, int a1, int a2);
extern void* cFEStateNetMessenger_vtbl[];
extern char* cBXString_EmptyString;

struct sAlloc_E0F60 {
    sAlloc_E0F60() {}
    sAlloc_E0F60(const sAlloc_E0F60&) {}
};

// The zeroed triple at 0x6E8 looks like an STL vector; its constructor takes an
// allocator temporary, which is what leaves the unused stack slot.
static inline void InitVec_E0F60(void* vvself, const sAlloc_E0F60& a = sAlloc_E0F60())
{
    cFEStateNetMessenger* vself = (cFEStateNetMessenger*)vvself;
    cFEStateNetMessenger* self = (cFEStateNetMessenger*)vself;
    self->mNames.mpStart = 0;
    self->mNames.mpFinish = 0;
    self->mNames.mpEndOfStorage = 0;
}

/**
 * Constructs the messenger state without an engine or owner: cFEStateNet_cFEStateNet(self, 0, 0), then installs its
 * vtable cFEStateNetMessenger_vtbl, sets mszFoundName, mszSearchName and mszShownName to cBXString_EmptyString and
 * clears the three words at +0x6E8 (a vector's begin, end and capacity).
 *
 * @return @p self
 */
void* cFEStateNetMessenger::cFEStateNetMessenger1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyString = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateNetMessenger_vtbl;
    mszFoundName = emptyString;
    mszSearchName = emptyString;
    mszShownName = emptyString;
    InitVec_E0F60(this);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Lets the transition away from the messenger proceed at once.
 *
 * @return true
 */
bool cFEStateNetMessenger::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateNetMessenger_onGainTransition);
#ifdef SKIP_ASM
/**
 * Publishes the messenger presence through its own vtable slot 32 (cFEStateNetMessenger_publishPresence, which reports the player as "in the
 * messenger") and lets the transition proceed.
 *
 * @return 1
 */
extern "C" int cFEStateNetMessenger_onGainTransition(cFEStateNetMessenger* self)
{
    // cFEStateNetMessenger_vtbl entry 32: cFEStateNetMessenger_publishPresence (publish the presence)
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cListNode, self);
    vt[32].fn(vthis(self, vt[32]));
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateReadMessagePopup_cFEStateReadMessagePopup1);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* cFEStateNet_cFEStateNet(void* self, int a1, int a2);
extern void* cFEStateReadMessagePopup_vtbl[];
extern char* cBXString_EmptyString;

struct sAlloc_E0FF0 {
    sAlloc_E0FF0() {}
    sAlloc_E0FF0(const sAlloc_E0FF0&) {}
};

// The zeroed triple at 0x6DC is the STL vector mMessages; its constructor takes an
// allocator temporary, which is what leaves the unused stack slot.
static inline void InitVec_E0FF0(cFEStateReadMessagePopup* self, const sAlloc_E0FF0& a = sAlloc_E0FF0())
{
    self->mMessages.mpStart = 0;
    self->mMessages.mpFinish = 0;
    self->mMessages.mpEndOfStorage = 0;
}

/**
 * Constructs a read-message popup with no engine (the default-constructor copy).
 *
 * Runs cFEStateNet_cFEStateNet(self, 0, 0), stores cFEStateReadMessagePopup_vtbl, points mszSender at the empty
 * cBXString rep and zeroes the message vector mMessages (mpStart, mpFinish, mpEndOfStorage).
 *
 * @return @p self
 */
void* cFEStateReadMessagePopup::cFEStateReadMessagePopup1()
{
    cFEStateNet_cFEStateNet(this, 0, 0);
    char* emptyStr = cBXString_EmptyString;
    VPTR_OF(cListNode, this) = cFEStateReadMessagePopup_vtbl;
    mszSender = emptyStr;
    InitVec_E0FF0(this);
    return this;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cFEStateReadMessagePopup_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Leaving transition of the read-message popup: nothing to animate, so it reports the transition done at once.
 *
 * @return true (done)
 */
bool cFEStateReadMessagePopup::onLoseTransition()
{
    return true;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUISoundInterface__cUISoundInterface);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUISoundInterface_vtbl[];

/**
 * Destroys a base UI sound interface, freeing it when asked.
 *
 * Stores the base sound-interface vtable cUISoundInterface_vtbl at +0x0 and calls operator_delete when bit 0 of @p flags is set.
 * cUISoundInterface_vtbl is also the vtable of the null sound object gUISoundNull.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUISoundInterface__cUISoundInterface(cUISoundInterface* self, int flags)
{
    *(void***)self = cUISoundInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUISoundInterface_playSound__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI sound interface plays no sound (entry 2 of cUISoundInterface_vtbl).
 */
void cUISoundInterface::playSound(int)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUISoundInterface_soundInitialize__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI sound interface needs no set-up (entry 3 of cUISoundInterface_vtbl).
 */
void cUISoundInterface::soundInitialize()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUISoundInterface_soundShutdown__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI sound interface has nothing to shut down (entry 4 of cUISoundInterface_vtbl).
 */
void cUISoundInterface::soundShutdown()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUILocaleInterface_setTextNumCharFlag);
#ifdef SKIP_ASM
/**
 * Sets or clears a locale interface's text-num-char flag (mAttrLocale.mTextNumChar: bit 3 of the attribute byte,
 * stored through the whole word at 0x0 as bit 27).
 *
 * cUILocaleInterface_setDisplayMode calls it with (mode == 2).
 *
 * @param enabled  nonzero to set the flag
 */
void cUILocaleInterface::setTextNumCharFlag(int enabled)
{
    mAttrLocale.mTextNumChar = (enabled != 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUICustomInterface__cUICustomInterface);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUICustomInterface_vtbl[];

/**
 * Destroys a base UI custom interface, freeing it when asked.
 *
 * Stores the base vtable cUICustomInterface_vtbl at +0x4 and calls operator_delete when bit 0 of @p flags is set;
 * cFECustom__cFECustom puts back the same table.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUICustomInterface__cUICustomInterface(cUICustomInterface* self, int flags)
{
    VPTR_OF(cUICustomInterface, self) = cUICustomInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUICustomInterface_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI custom interface has no per-frame work (entry 2 of cUICustomInterface_vtbl).
 */
void cUICustomInterface::onUpdate()
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUICustomInterface_onScreenEvent__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the base UI custom interface handles no screen event (entry 3 of cUICustomInterface_vtbl).
 *
 * @return 0 (not handled)
 */
bool cUICustomInterface::onScreenEvent(cUIScreen*, unsigned int)
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUICustomInterface_getNextState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the base UI custom interface has no next state (entry 4 of cUICustomInterface_vtbl).
 *
 * @return 0 (no state)
 */
cUIState* cUICustomInterface::getNextState(cUIState*, int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUICustomInterface_getPrevState__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0: the base UI custom interface has no previous state (entry 5 of cUICustomInterface_vtbl).
 *
 * @return 0 (no state)
 */
cUIState* cUICustomInterface::getPrevState(cUIState*, int)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput__cUIInput);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cUIInput_vtbl[];

/**
 * Destroys a UI input object: restores cUIInput's vtable, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIInput__cUIInput(cUIInput* self, int flags)
{
    VPTR_OF(cUIInput, self) = cUIInput_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getState__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's getState: always 0 (On Tour getState(char state); retail's stub reads no state).
 *
 * @return always 0 (not active)
 */
bool cUIInput::getState(char) const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isStart__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's start test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isStart() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isSelect__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's select test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isSelect() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNext__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isNext() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPrevious__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isPrevious() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOption__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isOption() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMisc__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isMisc() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNextHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isNextHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPreviousHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isPreviousHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOptionHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isOptionHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMiscHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isMiscHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isNextReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's next-released test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isNextReleased() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isPreviousReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's previous-released test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isPreviousReleased() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isOptionReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's option-released test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isOptionReleased() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isMiscReleased__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's misc-released test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isMiscReleased() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isUp__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's up test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isUp() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isDown__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's down test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isDown() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLeft__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's left test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLeft() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRight__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's right test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRight() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isUpHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's up-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isUpHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isDownHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's down-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isDownHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLeftHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's left-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLeftHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRightHeld__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's right-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRightHeld() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift1__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R1 test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRShift1() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift2__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R2 test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRShift2() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift1__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L1 test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLShift1() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift2__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L2 test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLShift2() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R1-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRShift1Held() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isRShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's R2-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isRShift2Held() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift1Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L1-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLShift1Held() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isLShift2Held__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's L2-held test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isLShift2Held() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-zoom test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isCamZoom() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-pan test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isCamPan() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-rotate test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isCamRotate() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamZoom__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-zoom amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput::getCamZoom() const
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamPan__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-pan amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput::getCamPan() const
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getCamRotate__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's camera-rotate amount: always 0.0; device classes override it.
 *
 * @return always 0.0
 */
float cUIInput::getCamRotate() const
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_keyPressed__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's key-pressed test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::keyPressed() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getKeyReady__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's key-ready test (a typed key waiting): always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::getKeyReady() const
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getVKey__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's virtual key code: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput::getVKey() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_getAsciiKey__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's typed ASCII key: always 0; device classes override it.
 *
 * @return always 0
 */
int cUIInput::getAsciiKey() const
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_setPadNum__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the controller pad number of a UI input object.
 *
 * @param val  pad number, stored as a byte at offset 4
 */
void cUIInput::setPadNum(int val)
{
    mPadNum = (char)val;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_isActive__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's active test: always 0; device classes override it.
 *
 * @return always 0
 */
bool cUIInput::isActive()
{
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cUIInput_update__FPv);
#ifdef SKIP_ASM
/**
 * Base UI input's per-frame update: an empty stub; device classes override it.
 */
void cUIInput::update()
{
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12B0__FPv);
#ifdef SKIP_ASM
/**
 * Runs all_in_one_main's static constructors: calls func_001DBC98(1, 0xFFFF).
 *
 * Entry +0x20 of the global constructor table __CTOR_LIST__ that __do_global_ctors (__do_global_ctors) walks; func_001E12D0 is
 * its destructor twin. The declared parameter is not used.
 *
 * @return func_001DBC98's result
 */
void* func_001E12B0(void* self)
{
    return func_001DBC98(1, 0xffff);
}
#endif

extern "C" void* func_001DBC98(int, int);

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E12D0__FPv);
#ifdef SKIP_ASM
/**
 * Runs all_in_one_main's static destructors: calls func_001DBC98(0, 0xFFFF).
 *
 * Entry +0x8 of the global destructor table D_0043CEE4; func_001E12B0 is its constructor twin. The declared parameter
 * is not used.
 *
 * @return func_001DBC98's result
 */
void* func_001E12D0(void* self)
{
    return func_001DBC98(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", initMessageGroups);
#ifdef SKIP_ASM
#include "fakematch.h"
#include "ui/uitext.h"
// sPopupEntry14 (0x14 bytes: +0x4 start, +0x8 count) is defined later in the unit.
struct sPopupEntry14;
extern sPopupEntry14 D_00441630[];

struct sCount_12F0 {
    int f0;
    int start;
    int count;
    int fC;
    int f10;
};

struct sRun_12F0 {
    int start;
    int count;
    int f8;
    int fC;
    int f10;
};

/**
 * Fills in the message-group table D_00441630: how many messages each of its 61 groups holds and where each group
 * starts.
 *
 * Each 0x14-byte group gets its message count (+0x8): 0 for group 0, 6 for groups 1-6, 11, 17, 7, 6 and 8 for groups
 * 7-11, 3 for groups 12-21, 4 for groups 22-27, 16 for groups 28-33, 2 for groups 34-39, 0 for groups 40-51 and 1 for
 * groups 52-60 (256 messages in all). Group 0 starts at message 0 and every later group's first message (+0x4) is the
 * previous group's start plus its count. updateTimedMessages runs it once.
 */
// FAKEMATCH: statement order (each run of equal counts is written with its first entry last, and the
// zero and one entries of the tail interleaved, so sched1 reproduces retail's store order) plus five
// FM_OPAQUE(n) after n = 1 (delays the ones by five cycles so d[52] issues after d[46]); plain form 99.82%
// (natural index order: 99.02%).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void initMessageGroups(void)
{
    sCount_12F0* groupTable = (sCount_12F0*)D_00441630;
    int i;
    int n;

    n = 6;
    groupTable[2].count = n;
    groupTable[3].count = n;
    groupTable[4].count = n;
    groupTable[5].count = n;
    groupTable[6].count = n;
    groupTable[1].count = n;
    groupTable[0].count = 0;
    n = 11;
    groupTable[7].count = n;
    n = 17;
    groupTable[8].count = n;
    n = 7;
    groupTable[9].count = n;
    n = 6;
    groupTable[10].count = n;
    n = 8;
    groupTable[11].count = n;
    n = 3;
    groupTable[13].count = n;
    groupTable[14].count = n;
    groupTable[15].count = n;
    groupTable[16].count = n;
    groupTable[17].count = n;
    groupTable[18].count = n;
    groupTable[19].count = n;
    groupTable[20].count = n;
    groupTable[21].count = n;
    groupTable[12].count = n;
    n = 4;
    groupTable[23].count = n;
    groupTable[24].count = n;
    groupTable[25].count = n;
    groupTable[26].count = n;
    groupTable[27].count = n;
    groupTable[22].count = n;
    n = 16;
    groupTable[28].count = n;
    groupTable[29].count = n;
    groupTable[30].count = n;
    groupTable[31].count = n;
    groupTable[32].count = n;
    groupTable[33].count = n;
    groupTable[40].count = 0;
    n = 2;
    groupTable[35].count = n;
    groupTable[36].count = n;
    groupTable[37].count = n;
    groupTable[38].count = n;
    groupTable[39].count = n;
    groupTable[34].count = n;
    groupTable[41].count = 0;
    groupTable[42].count = 0;
    groupTable[43].count = 0;
    groupTable[44].count = 0;
    groupTable[45].count = 0;
    groupTable[46].count = 0;
    groupTable[47].count = 0;
    n = 1;
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    FM_OPAQUE(n);
    groupTable[53].count = n;
    groupTable[48].count = 0;
    groupTable[54].count = n;
    groupTable[49].count = 0;
    groupTable[55].count = n;
    groupTable[50].count = 0;
    groupTable[56].count = n;
    groupTable[51].count = 0;
    groupTable[57].count = n;
    groupTable[0].start = 0;
    groupTable[58].count = n;
    groupTable[59].count = n;
    groupTable[60].count = n;
    groupTable[52].count = n;
    for (i = 1; i <= 60; i++) {
        sRun_12F0* groupRuns = (sRun_12F0*)&groupTable->start;
        groupRuns[i].start = groupRuns[i - 1].start + groupRuns[i - 1].count;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1450);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty function that returns at once. Nothing in the build references it.
 */
extern "C" void func_001E1450(void) {

}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", updateTimedMessages);
#ifdef SKIP_ASM
extern int D_004A20C0;
extern int D_004A20C8;
extern int D_004A20BC;
extern "C" void initMessageGroups(void);
extern "C" void runTimedMessageHooks(int type);

/**
 * Runs the message system's periodic check (called every frame by cGame_update).
 *
 * Skipped while the message center is open (D_004A20C0 set). The first call fills in the message-group table
 * (initMessageGroups, guarded by D_004A20C8). Whenever the app manager's counter at +0x1C has moved more than 600 past
 * D_004A20BC, calls runTimedMessageHooks(1) and stores the counter in D_004A20BC.
 */
extern "C" void updateTimedMessages(void)
{
    if (D_004A20C0 == 0) {
        if (D_004A20C8 == 0) {
            D_004A20C8 = 1;
            initMessageGroups();
        }
        int appTicks = cAppMan_TheAppMan->Tick;
        if (appTicks - D_004A20BC > 600) {
            runTimedMessageHooks(1);
            D_004A20BC = appTicks;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", runTimedMessageHooks);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool isMessageFlagSet(int group);
void func_001E1548(int group) __asm__("func_001E1548__FPv");

struct sEntry14_14C0 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_14C0 D_00441630_14C0[] __asm__("D_00441630");

/**
 * Runs the timed-delivery hook for every message group of a given type that has not been flagged yet.
 *
 * Calls func_001E1548 with the group index for each of the 61 groups whose message flag is clear (isMessageFlagSet) and
 * whose type (+0x0 of D_00441630) is @p type or 5. func_001E1548 is empty, so this has no effect (and no group has type
 * 5).
 *
 * @param type  the group type to deliver (updateTimedMessages passes 1)
 */
extern "C" void runTimedMessageHooks(int type)
{
    int i;
    for (i = 0; i < 61; i++) {
        if (isMessageFlagSet(i) == 0) {
            int groupType = D_00441630_14C0[i].type;
            if (groupType == type || groupType == 5) {
                func_001E1548(i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E1548__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the per-group hook runTimedMessageHooks calls.
 *
 * @param group  the message group index (the symbol's mangling says void*)
 */
void func_001E1548(int group)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueRivalChallengeMessages);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" bool isMessageFlagSet(int group);
extern "C" void queueMessage(int a, unsigned char b);
extern "C" void setMessageFlag(int a, int b);
extern "C" int cBENewPlayerInterface_isRivalEventLocked(void* iface, int a, int ch, int b, int c);
extern "C" int cBENewPlayerInterface_hasPlayedEvent(void* iface, int a, int b, int c);
unsigned int BXrand();

/**
 * Flags and queues the first message of the back-country groups 1-6 after a race, when the player has not yet earned
 * them.
 *
 * Skipped during replay playback (the replay's first word 1-9). For the first of groups 1, 2, 3 whose message flag is
 * clear (peak k = 0, 1, 2; later groups are not tried in the same call): when cBENewPlayerInterface_isRivalEventLocked(player interface, 0,
 * character, k, 1) and cBENewPlayerInterface_hasPlayedEvent(player interface, 0, 14 + k, 4) both return 0, sets the group's flag
 * (setMessageFlag) and queues a random one of the group's first three messages (queueMessage), or of its last three
 * when the human rider's character is 3, 7 or 8 respectively. Groups 4, 5, 6 are handled the same way, independently,
 * with cBENewPlayerInterface_isRivalEventLocked(..., k, 0) and event 5. cBEScoreInterface_processRiderRanking (race result) calls it except in modes 4 and 5.
 */
extern "C" void queueRivalChallengeMessages(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        if (isMessageFlagSet(1) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 0, 1) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0xE, 4) == 0) {
                setMessageFlag(1, 1);
                if (ch != 3) {
                    queueMessage(BXrand() % 3, 0);
                } else {
                    queueMessage(BXrand() % 3 + 3, 0);
                }
            }
        } else if (isMessageFlagSet(2) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 1, 1) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0xF, 4) == 0) {
                setMessageFlag(2, 1);
                if (ch != 7) {
                    queueMessage(BXrand() % 3 + 6, 0);
                } else {
                    queueMessage(BXrand() % 3 + 9, 0);
                }
            }
        } else if (isMessageFlagSet(3) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 2, 1) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0x10, 4) == 0) {
                setMessageFlag(3, 1);
                if (ch != 8) {
                    queueMessage(BXrand() % 3 + 0xC, 0);
                } else {
                    queueMessage(BXrand() % 3 + 0xF, 0);
                }
            }
        }
        if (isMessageFlagSet(4) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 0, 0) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0xE, 5) == 0) {
                setMessageFlag(4, 1);
                if (ch != 3) {
                    queueMessage(BXrand() % 3 + 18, 0);
                } else {
                    queueMessage(BXrand() % 3 + 21, 0);
                }
            }
        } else if (isMessageFlagSet(5) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 1, 0) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0xF, 5) == 0) {
                setMessageFlag(5, 1);
                if (ch != 7) {
                    queueMessage(BXrand() % 3 + 0x18, 0);
                } else {
                    queueMessage(BXrand() % 3 + 27, 0);
                }
            }
        } else if (isMessageFlagSet(6) == 0) {
            if (cBENewPlayerInterface_isRivalEventLocked(iface, 0, ch, 2, 0) == 0 && cBENewPlayerInterface_hasPlayedEvent(iface, 0, 0x10, 5) == 0) {
                setMessageFlag(6, 1);
                if (ch != 8) {
                    queueMessage(BXrand() % 3 + 0x1E, 0);
                } else {
                    queueMessage(BXrand() % 3 + 33, 0);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", isSameCharacterId);
#ifdef SKIP_ASM
/**
 * Reports whether @p x is a character id (0-9) equal to @p y.
 *
 * @param x  the value to test (only 0-9 can match)
 * @param y  the character id to compare with
 * @return true on a match, else false
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" bool isSameCharacterId(int x, int y)
{
    switch (x) {
    case 0: return y == 0;
    case 1: return y == 1;
    case 2: return y == 2;
    case 3: return y == 3;
    case 4: return y == 4;
    case 5: return y == 5;
    case 6: return y == 6;
    case 7: return y == 7;
    case 8: return y == 8;
    case 9: return y == 9;
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", pickForeshadowMessage);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
unsigned int BXrand();
extern "C" bool isSameCharacterId(int kind, int arg);
extern "C" int findMessage(int id);
extern char str_AddForeshadowMessage[];

struct sPopupEntry18_1AD0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};
extern sPopupEntry18_1AD0 D_004C6C08_1AD0[] __asm__("D_004C6C08");

struct sRangeK1E1AD0 {
    int f0;
    int start;   // 0x4
    int count;   // 0x8
};

/**
 * Picks a random message from a message group that is not about a given character and not already queued.
 *
 * Allocates a list (operator new, tag "AddForeshadowMessage"), collects the group's message indices i (from its first
 * message for its count) whose character (+0xC of D_004C6C08[i]) does not match @p skipCharId (isSameCharacterId) and that are not
 * in the message list (findMessage returns -1), returns one of them at random (BXrand) and frees the list. Likely
 * bug: when nothing qualifies it returns 0x100 without freeing the list.
 *
 * @param messageGroup    the message group (a D_00441630 entry: +0x4 first message, +0x8 count)
 * @param skipCharId  the character whose messages are skipped
 * @return a message index, or 0x100 when none qualifies
 */
extern "C" int pickForeshadowMessage(sRangeK1E1AD0* messageGroup, int skipCharId)
{
    int* list = (int*)operator_new_tag(messageGroup->count * 4, str_AddForeshadowMessage, 0x100, 0);
    int n = 0;
    int i;
    for (i = messageGroup->start; i < messageGroup->count + messageGroup->start; i++) {
        if (isSameCharacterId(D_004C6C08_1AD0[i].fC, skipCharId) == 0 && findMessage(i) == -1) {
            list[n++] = i;
        }
    }
    if (n == 0)
        return 0x100;
    int id = list[BXrand() % n];
    if (list != 0)
        cMemMan_free(list);
    return id;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueForeshadowMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" bool isMessageFlagSet(int group);
extern "C" void queueMessage(int a, unsigned char b);

struct sEntry14_1C10 {
    int type;
    int f4;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_1C10 D_00441630_1C10[] __asm__("D_00441630");



/**
 * Queues a foreshadowing message for a peak.
 *
 * Skipped during replay playback and when either of the peak's two foreshadow groups (28/29, 30/31, 32/33) has its
 * message flag set. Uses the pair's second group when the human rider's character is 3, 7 or 8 (for peaks 0, 1, 2),
 * else the first, picks a message from it that is not about the human rider's character (pickForeshadowMessage) and queues it
 * (queueMessage) unless none was found. Sets no flag itself (flagForeshadowGroups does). Called by the reward interface after
 * a peak event.
 *
 * @param which  the peak (0-2)
 */
extern "C" void queueForeshadowMessage(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        int id;
        switch (which) {
        case 0: {
            if (isMessageFlagSet(28) != 0) {
                break;
            }
            if (isMessageFlagSet(29) != 0) {
                break;
            }
            sEntry14_1C10* foreshadowGroup;
            if (ch != 3) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[0x1C];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[29];
            }
            id = pickForeshadowMessage((sRangeK1E1AD0*)foreshadowGroup, ch);
            if (id != 0x100) {
                queueMessage(id, 0);
            }
            break;
        }
        case 1: {
            if (isMessageFlagSet(30) != 0) {
                break;
            }
            if (isMessageFlagSet(31) != 0) {
                break;
            }
            sEntry14_1C10* foreshadowGroup;
            if (ch != 7) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[0x1E];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[0x1F];
            }
            id = pickForeshadowMessage((sRangeK1E1AD0*)foreshadowGroup, ch);
            if (id != 0x100) {
                queueMessage(id, 0);
            }
            break;
        }
        case 2: {
            if (isMessageFlagSet(32) != 0) {
                break;
            }
            if (isMessageFlagSet(33) != 0) {
                break;
            }
            sEntry14_1C10* foreshadowGroup;
            if (ch != 8) {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[0x20];
            } else {
                sEntry14_1C10* tbl = D_00441630_1C10;
                foreshadowGroup = &tbl[33];
            }
            id = pickForeshadowMessage((sRangeK1E1AD0*)foreshadowGroup, ch);
            if (id != 0x100) {
                queueMessage(id, 0);
            }
            break;
        }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", setChallengeMessagePending);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int id, int a1);
static inline int IsBusy001E1DD0()
{
    int n = *(int*)SSXApp->Game->Replay;
    return n != 0 && n < 10;
}
/**
 * Flags the follow-up message for an event mode 6-11 unless the player already has it.
 *
 * Skipped during replay playback. Mode 6, 7, 8, 9, 10, 11 maps to group 34, 36, 38, 35, 37, 39; when that group's
 * message flag is clear, sets pending flag 40, 44, 48, 42, 46 or 50 (one higher when @p alt is set) with setMessageFlag.
 * sendPendingChallengeMessages later turns the pending flags into messages. The first argument is not used (the reward interface
 * passes the peak).
 *
 * @param kind  the event mode (6-11; others do nothing)
 * @param alt   nonzero to set the pending flag one above (the reward interface passes 1 for medal 2, 0 for medal 3)
 */
extern "C" void setChallengeMessagePending(void* self, int kind, int alt)
{
    if (IsBusy001E1DD0()) return;
    int id = 0;
    switch (kind) {
    case 6: id = isMessageFlagSet(34) == 0 ? 40 : 0; break;
    case 7: id = isMessageFlagSet(36) == 0 ? 44 : 0; break;
    case 8: id = isMessageFlagSet(38) == 0 ? 0x30 : 0; break;
    case 9: id = isMessageFlagSet(35) == 0 ? 42 : 0; break;
    case 10: id = isMessageFlagSet(37) == 0 ? 46 : 0; break;
    case 11: id = isMessageFlagSet(39) == 0 ? 50 : 0; break;
    }
    if (id != 0) {
        if (alt != 0) id++;
        setMessageFlag(id, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", sendPendingChallengeMessages);
#ifdef SKIP_ASM
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queuePointsChallengeMessage(int which, int flag);
extern "C" void queueTimeChallengeMessage(int which, int flag);

/**
 * Queues the follow-up messages flagged by setChallengeMessagePending and clears their pending flags.
 *
 * Skipped during replay playback. For peaks 0-2, pending flag 40/44/48 calls queueTimeChallengeMessage(peak, 0) and 41/45/49 calls
 * queueTimeChallengeMessage(peak, 1); pending flag 42/46/50 calls queuePointsChallengeMessage(peak, 0) and 43/47/51 queuePointsChallengeMessage(peak, 1). Each
 * handled flag is cleared (setMessageFlag(flag, 0)); the second of a pair is only checked when the first is clear.
 * cBEScoreInterface_processRiderRanking (race result) calls it.
 */
extern "C" void sendPendingChallengeMessages(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        if (isMessageFlagSet(40) != 0) {
            queueTimeChallengeMessage(0, 0);
            setMessageFlag(40, 0);
        } else if (isMessageFlagSet(41) != 0) {
            queueTimeChallengeMessage(0, 1);
            setMessageFlag(41, 0);
        }
        if (isMessageFlagSet(44) != 0) {
            queueTimeChallengeMessage(1, 0);
            setMessageFlag(44, 0);
        } else if (isMessageFlagSet(45) != 0) {
            queueTimeChallengeMessage(1, 1);
            setMessageFlag(45, 0);
        }
        if (isMessageFlagSet(48) != 0) {
            queueTimeChallengeMessage(2, 0);
            setMessageFlag(0x30, 0);
        } else if (isMessageFlagSet(49) != 0) {
            queueTimeChallengeMessage(2, 1);
            setMessageFlag(49, 0);
        }
        if (isMessageFlagSet(42) != 0) {
            queuePointsChallengeMessage(0, 0);
            setMessageFlag(42, 0);
        } else if (isMessageFlagSet(43) != 0) {
            queuePointsChallengeMessage(0, 1);
            setMessageFlag(43, 0);
        }
        if (isMessageFlagSet(46) != 0) {
            queuePointsChallengeMessage(1, 0);
            setMessageFlag(46, 0);
        } else if (isMessageFlagSet(47) != 0) {
            queuePointsChallengeMessage(1, 1);
            setMessageFlag(47, 0);
        }
        if (isMessageFlagSet(50) != 0) {
            queuePointsChallengeMessage(2, 0);
            setMessageFlag(50, 0);
        } else if (isMessageFlagSet(51) != 0) {
            queuePointsChallengeMessage(2, 1);
            setMessageFlag(51, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queuePointsChallengeMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

struct sEntry14_20D0 {   // a D_00441630 message group (names ours)
    int type;
    int start;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_20D0 D_00441630_20D0[] __asm__("D_00441630");

/**
 * Queues the message of group 35, 37 or 39 for peak @p which unless that group's flag is set.
 *
 * Skipped during replay playback. Queues the group's first message (+0x4 of D_00441630), or, when @p flag is set, sets
 * the group's flag and queues its second message.
 *
 * @param which  the peak (0-2; others do nothing)
 * @param flag   nonzero to send the second message and flag the group
 */
extern "C" void queuePointsChallengeMessage(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (isMessageFlagSet(35) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    setMessageFlag(35, 1);
                    queueMessage(tbl[35].start + 1, 0);
                } else {
                    queueMessage(tbl[35].start, 0);
                }
            }
            break;
        case 1:
            if (isMessageFlagSet(37) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    setMessageFlag(37, 1);
                    queueMessage(tbl[37].start + 1, 0);
                } else {
                    queueMessage(tbl[37].start, 0);
                }
            }
            break;
        case 2:
            if (isMessageFlagSet(39) == 0) {
                sEntry14_20D0* tbl = D_00441630_20D0;
                if (flag) {
                    setMessageFlag(39, 1);
                    queueMessage(tbl[39].start + 1, 0);
                } else {
                    queueMessage(tbl[39].start, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueTimeChallengeMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

struct sEntry14_2220 {   // a D_00441630 message group (names ours)
    int type;
    int start;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_2220 D_00441630_2220[] __asm__("D_00441630");

/**
 * Queues the message of group 34, 36 or 38 for peak @p which unless that group's flag is set.
 *
 * Skipped during replay playback. Queues the group's first message (+0x4 of D_00441630), or, when @p flag is set, sets
 * the group's flag and queues its second message.
 *
 * @param which  the peak (0-2; others do nothing)
 * @param flag   nonzero to send the second message and flag the group
 */
extern "C" void queueTimeChallengeMessage(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (isMessageFlagSet(34) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    setMessageFlag(34, 1);
                    queueMessage(tbl[34].start + 1, 0);
                } else {
                    queueMessage(tbl[34].start, 0);
                }
            }
            break;
        case 1:
            if (isMessageFlagSet(36) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    setMessageFlag(36, 1);
                    queueMessage(tbl[36].start + 1, 0);
                } else {
                    queueMessage(tbl[36].start, 0);
                }
            }
            break;
        case 2:
            if (isMessageFlagSet(38) == 0) {
                sEntry14_2220* tbl = D_00441630_2220;
                if (flag) {
                    setMessageFlag(38, 1);
                    queueMessage(tbl[38].start + 1, 0);
                } else {
                    queueMessage(tbl[38].start, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", flagForeshadowGroups);
#ifdef SKIP_ASM
extern "C" void setMessageFlag(int a, int b);

/**
 * Sets the message flags of both foreshadow groups of a peak (28/29, 30/31 or 32/33), so queueForeshadowMessage sends no more.
 *
 * Skipped during replay playback. The reward interface calls it once a peak's events are all won.
 *
 * @param which  the peak (0-2; others do nothing)
 */
extern "C" void flagForeshadowGroups(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            setMessageFlag(0x1C, 1);
            setMessageFlag(29, 1);
            break;
        case 1:
            setMessageFlag(0x1E, 1);
            setMessageFlag(0x1F, 1);
            break;
        case 2:
            setMessageFlag(0x20, 1);
            setMessageFlag(33, 1);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queuePeakCompletedMessage);
#ifdef SKIP_ASM
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

/**
 * Queues the peak-award message for peak @p which once: message 252, 253 or 254, guarded by flag 57, 58 or 59.
 *
 * Skipped during replay playback. cBERewardInterface_awardUser calls it for award categories 2, 3 and 4. Likely bug:
 * for peak 1 it tests flag 58 but sets flag 57, so flag 58 is never set.
 *
 * @param which  the peak (0-2; others do nothing)
 */
extern "C" void queuePeakCompletedMessage(int which)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (which) {
        case 0:
            if (isMessageFlagSet(57) == 0) {
                setMessageFlag(57, 1);
                queueMessage(0xFC, 0);
            }
            break;
        case 1:
            if (isMessageFlagSet(58) == 0) {
                setMessageFlag(57, 1);
                queueMessage(253, 0);
            }
            break;
        case 2:
            if (isMessageFlagSet(59) == 0) {
                setMessageFlag(59, 1);
                queueMessage(0xFE, 0);
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueMountainCompletedMessage);
#ifdef SKIP_ASM
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

/**
 * Queues message 255 once (guarded by message flag 60).
 *
 * Skipped during replay playback. cBERewardInterface_awardUser calls it for award category 1.
 */
extern "C" void queueMountainCompletedMessage(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0 && isMessageFlagSet(60) == 0) {
        setMessageFlag(0x3C, 1);
        queueMessage(0xFF, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueMountainConqueredMessage);
#ifdef SKIP_ASM
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

/**
 * Queues message 251 once (guarded by message flag 56).
 *
 * Skipped during replay playback. cBERewardInterface_awardUser calls it for award category 0.
 */
extern "C" void queueMountainConqueredMessage(void)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0 && isMessageFlagSet(56) == 0) {
        setMessageFlag(0x38, 1);
        queueMessage(251, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueRivalResultMessage);
#ifdef SKIP_ASM
extern "C" void queueRivalTimeResultMessage(void* a, int b);
extern "C" void queueRivalPointsResultMessage(void* a, int b);

/**
 * Passes a back-country race result to the message system: mode 4 to queueRivalTimeResultMessage, mode 5 to queueRivalPointsResultMessage.
 *
 * Skipped during replay playback; other modes do nothing. cBEScoreInterface_processRiderRanking (race result) calls it with the game mode, the
 * event's location id and the result.
 *
 * @param key  the game mode
 * @param locationId   the event's location id (declared void*)
 * @param raceResult   the race result (0 = won)
 */
extern "C" void queueRivalResultMessage(int key, void* locationId, int raceResult)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        switch (key) {
        case 4:
            queueRivalTimeResultMessage(locationId, raceResult);
            break;
        case 5:
            queueRivalPointsResultMessage(locationId, raceResult);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueRivalTimeResultMessage);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);
extern "C" void removeGroupMessages(int type);

// PORT: the unit declares queueRivalTimeResultMessage(void*, int); the first arg is really an int id.
extern "C" void queueRivalTimeResultMessage_impl(int which, int flag) __asm__("queueRivalTimeResultMessage");
/**
 * Queues the result message of a mode-4 back-country event at location 14, 15 or 16 (peak 0, 1, 2).
 *
 * Skipped during replay playback and when the location's group (22, 23, 24) is already flagged. First removes the
 * queued messages of groups 1/2/3 and 22/23/24 (removeGroupMessages). On a win (@p flag 0) sets the group's flag and queues
 * message 116, 120 or 124 (118, 122, 126 when the human rider's character is 3, 7 or 8 respectively); otherwise queues
 * 115, 119 or 123 (117, 121, 125) and leaves the flag clear.
 *
 * @param which  the location id (14-16; others do nothing)
 * @param flag   the race result (0 = won)
 */
extern "C" void queueRivalTimeResultMessage_impl(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        switch (which) {
        case 0xE:
            if (isMessageFlagSet(22) != 0) {
                break;
            }
            removeGroupMessages(1);
            removeGroupMessages(22);
            if (flag == 0) {
                setMessageFlag(22, 1);
                if (ch != 3) {
                    queueMessage(116, 0);
                } else {
                    queueMessage(118, 0);
                }
            } else {
                if (ch != 3) {
                    queueMessage(115, 0);
                } else {
                    queueMessage(117, 0);
                }
            }
            break;
        case 0xF:
            if (isMessageFlagSet(23) != 0) {
                break;
            }
            removeGroupMessages(2);
            removeGroupMessages(23);
            if (flag == 0) {
                setMessageFlag(23, 1);
                if (ch != 7) {
                    queueMessage(0x78, 0);
                } else {
                    queueMessage(122, 0);
                }
            } else {
                if (ch != 7) {
                    queueMessage(119, 0);
                } else {
                    queueMessage(121, 0);
                }
            }
            break;
        case 0x10:
            if (isMessageFlagSet(24) != 0) {
                break;
            }
            removeGroupMessages(3);
            removeGroupMessages(0x18);
            if (flag == 0) {
                setMessageFlag(0x18, 1);
                if (ch != 8) {
                    queueMessage(0x7C, 0);
                } else {
                    queueMessage(0x7E, 0);
                }
            } else {
                if (ch != 8) {
                    queueMessage(123, 0);
                } else {
                    queueMessage(125, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueRivalPointsResultMessage);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);
extern "C" void removeGroupMessages(int type);

// PORT: the unit declares queueRivalPointsResultMessage(void*, int); the first arg is really an int id.
extern "C" void queueRivalPointsResultMessage_impl(int which, int flag) __asm__("queueRivalPointsResultMessage");
/**
 * Queues the result message of a mode-5 back-country event at location 14, 15 or 16 (peak 0, 1, 2).
 *
 * Skipped during replay playback and when the location's group (25, 26, 27) is already flagged. First removes the
 * queued messages of groups 4/5/6 and 25/26/27 (removeGroupMessages). On a win (@p flag 0) sets the group's flag and queues
 * message 128, 132 or 136 (130, 134, 138 when the human rider's character is 3, 7 or 8 respectively); otherwise queues
 * 127, 131 or 135 (129, 133, 137) and leaves the flag clear.
 *
 * @param which  the location id (14-16; others do nothing)
 * @param flag   the race result (0 = won)
 */
extern "C" void queueRivalPointsResultMessage_impl(int which, int flag)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy == 0) {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        int ch = cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0));
        switch (which) {
        case 0xE:
            if (isMessageFlagSet(25) != 0) {
                break;
            }
            removeGroupMessages(4);
            removeGroupMessages(25);
            if (flag == 0) {
                setMessageFlag(25, 1);
                if (ch != 3) {
                    queueMessage(0x80, 0);
                } else {
                    queueMessage(130, 0);
                }
            } else {
                if (ch != 3) {
                    queueMessage(0x7F, 0);
                } else {
                    queueMessage(129, 0);
                }
            }
            break;
        case 0xF:
            if (isMessageFlagSet(26) != 0) {
                break;
            }
            removeGroupMessages(5);
            removeGroupMessages(26);
            if (flag == 0) {
                setMessageFlag(26, 1);
                if (ch != 7) {
                    queueMessage(132, 0);
                } else {
                    queueMessage(134, 0);
                }
            } else {
                if (ch != 7) {
                    queueMessage(131, 0);
                } else {
                    queueMessage(133, 0);
                }
            }
            break;
        case 0x10:
            if (isMessageFlagSet(27) != 0) {
                break;
            }
            removeGroupMessages(6);
            removeGroupMessages(27);
            if (flag == 0) {
                setMessageFlag(27, 1);
                if (ch != 8) {
                    queueMessage(136, 0);
                } else {
                    queueMessage(138, 0);
                }
            } else {
                if (ch != 8) {
                    queueMessage(135, 0);
                } else {
                    queueMessage(137, 0);
                }
            }
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueAggressionMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int getAggressionGroup(int charID);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);
unsigned int BXrand();

struct sEntry14_2A08 {
    int type;
    int start;   // 0x4
    int count;   // 0x8
    char pad_0xC[0x14 - 0xC];
};
extern sEntry14_2A08 D_00441630_2A08[] __asm__("D_00441630");

/**
 * Queues an aggression message about another character when the human rider's grudge against it runs high.
 *
 * Skipped during replay playback and when @p charID is the human rider's own character. The character's group is
 * getAggressionGroup(charID) (12-21). When the group is flagged, clears the flag once @p level falls below 5; otherwise,
 * when @p level exceeds 15 plus a random 0-7, queues a random message of the group and sets the flag.
 * cBEAggressionInterface_applyAggression calls it for rider 0.
 *
 * @param charID  the other rider's character id
 * @param level   the aggression ticks toward that rider
 */
extern "C" void queueAggressionMessage(int charID, int level)
{
    int n = *(int*)SSXApp->Game->Replay;
    int busy = n != 0 && n < 10;
    if (busy)
        return;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    if (cBENewPlayerInterface_getRiderCharID(iface, cBENewPlayerInterface_getHumanRider(iface, 0)) == charID)
        return;
    int id = getAggressionGroup(charID);
    sEntry14_2A08* aggressionGroup = &D_00441630_2A08[id];
    if (isMessageFlagSet(id)) {
        if (level < 5)
            setMessageFlag(id, 0);
    } else if ((int)((BXrand() & 7) + 0xF) < level) {
        queueMessage(aggressionGroup->start + BXrand() % aggressionGroup->count, 0);
        setMessageFlag(id, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getAggressionGroup);
#ifdef SKIP_ASM
/**
 * Returns the aggression message group of a character: 12 + @p x for characters 0-9, else 61.
 *
 * @param x  the character id
 * @return the message group index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int getAggressionGroup(int x)
{
    switch (x) {
    case 0: return 0xC;
    case 1: return 13;
    case 2: return 0xE;
    case 3: return 0xF;
    case 4: return 0x10;
    case 5: return 17;
    case 6: return 18;
    case 7: return 19;
    case 8: return 20;
    case 9: return 21;
    case 10:
    default:
        return 61;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", countMessageRows);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool isMessageFlagSet(int group);

struct sEntry14_2BB8 {   // a D_00441630 message group (names ours)
    int type;
    int start;
    int count;
    int category;
    int isFolder;
};
extern sEntry14_2BB8 D_00441630_2BB8[] __asm__("D_00441630");

/**
 * Counts the message-center rows taken by all message groups of one type.
 *
 * A group of that type counts all its messages when @p all is set or when it is a folder (+0x10 set) whose flag is set
 * (expanded), else one row.
 *
 * @param type  the group type (+0x0 of D_00441630; the message center uses 4, the FAQ groups 7-11)
 * @param all   nonzero to count every message
 * @return the number of rows
 */
extern "C" int countMessageRows(int type, int all)
{
    int total = 0;
    int i;
    for (i = 0; i < 61; i++) {
        sEntry14_2BB8* groupEntry = &D_00441630_2BB8[i];
        if (groupEntry->type == type) {
            if (all != 0 || (groupEntry->isFolder != 0 && isMessageFlagSet(i) != 0)) {
                total += groupEntry->count;
            } else {
                total += 1;
            }
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageGroupCount);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sEntry14_2C60 {
    int type;
    int start;   // 0x4
    int count;   // 0x8
    int fC;
    int f10;
};
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

/**
 * Returns the number of messages in message group @p i (+0x8 of D_00441630).
 *
 * @param i  the group index
 * @return its message count
 */
extern "C" int getMessageGroupCount(int i)
{
    return D_00441630_2C60[i].count;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageForRow);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool isMessageFlagSet(int group);

struct sEntry14_2C80 {   // a D_00441630 message group (names ours)
    int type;
    int first;
    int count;
    int category;
    int isFolder;
};
extern sEntry14_2C80 D_00441630_2C80[] __asm__("D_00441630");

/**
 * Returns the message shown in row @p idx of the rows built from the message groups of one type.
 *
 * Walks the groups of that type; each takes all its messages when @p mode is 0, when it is not a folder (+0x10 clear)
 * or when its flag is set, else one row (its first message). Note that countMessageRows counts a non-folder group as one
 * row in its non-full mode; the two agree for type 4, whose groups (7-11) are all folders.
 *
 * @param type  the group type (+0x0 of D_00441630)
 * @param idx   the row
 * @param mode  0 to list every message
 * @return the message index, or -1 past the last row
 */
extern "C" int getMessageForRow(int type, int idx, int mode)
{
    int base = 0;
    int result = -1;
    int i;
    for (i = 0; i < 61; i++) {
        sEntry14_2C80* groupEntry = &D_00441630_2C80[i];
        if (groupEntry->type == type) {
            int n;
            if (mode == 0 || groupEntry->isFolder == 0 || isMessageFlagSet(i) != 0) {
                n = groupEntry->count;
            } else {
                n = 1;
            }
            n += base;
            if (idx < n) {
                result = groupEntry->first + (idx - base);
                goto done;
            }
            base = n;
        }
    }
done:
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", isMessageInCategory);
#ifdef SKIP_ASM
struct sPopupEntry18 {
    int field_0x0;
    int groupIndex;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18 D_004C6C08[];

struct sPopupEntry14 {
    int type;
    int start;   // 0x4: the group's first message
    int count;   // 0x8: its message count
    int category;   // 0xC: the category isMessageInCategory tests
    int isFolder;   // 0x10
};
extern sPopupEntry14 D_00441630[];

/**
 * Reports whether message @p i belongs to a group of category @p category (+0xC of its D_00441630 group).
 *
 * @param category  the category (3 back-country result, 4 foreshadow, 5 aggression, 8 peak awards, as the subject strings name
 *           them)
 * @param i  the message index
 * @return true when it does, else false
 */
extern "C" bool isMessageInCategory(int category, int i)
{
    sPopupEntry18* messageEntry = &D_004C6C08[i];
    sPopupEntry14* groupEntry = &D_00441630[messageEntry->groupIndex];
    return groupEntry->category == category;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", isMessageGroupInCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

/**
 * Reports whether message group @p i has category @p category (+0xC of D_00441630).
 *
 * @param category  the category
 * @param i  the group index
 * @return true when it does, else false
 */
extern "C" bool isMessageGroupInCategory(int category, int i)
{
    int* groupWords = (int*)&D_00441630_2C60[0] + i * 5;
    return groupWords[3] == category;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessagePositionInCategory);
#ifdef SKIP_ASM
/**
 * Returns message @p idx's 0-based position among all messages of the same category.
 *
 * Walks the messages from 0 a group at a time, adding the counts of earlier groups with that category and, for the
 * group holding @p idx, its offset within the group.
 *
 * @param idx  the message index
 * @return the position (the variant number cOVState_ViewPopup_getSubjectVariant and cOVState_MESSAGECENTER_getFAQSubjectVariant derive the subject from)
 */
extern "C" int getMessagePositionInCategory(int idx)
{
    sPopupEntry18* ta = &D_004C6C08[idx];
    sPopupEntry14* target = &D_00441630[ta->groupIndex];
    int position = 0;
    int i = 0;
    while (i <= idx) {
        sPopupEntry18* messageEntry = &D_004C6C08[i];
        sPopupEntry14* groupEntry = &D_00441630[messageEntry->groupIndex];
        int start = groupEntry->start;
        int count = groupEntry->count;
        if (start + count - 1 < idx) {
            if (groupEntry->category == target->category) position += count;
            i += count;
        } else {
            if (groupEntry->category == target->category) position += idx - start + 1;
            break;
        }
    }
    return position - 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getGroupMessageIndex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern sEntry14_2C60 D_00441630_2C60[] __asm__("D_00441630");

/**
 * Returns the index of message @p n of message group @p i (the group's first message, +0x4, plus @p n).
 *
 * @param i  the group index
 * @param n  the offset within the group
 * @return the message index
 */
extern "C" int getGroupMessageIndex(int i, int n)
{
    return D_00441630_2C60[i].start + n;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageSenderName);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern char str_kT_CHARMoby[];
extern char str_kT_CHARKaori[];
extern char str_kT_CHARAllegra[];
extern char str_kT_CHARMac[];
extern char str_kT_CHARZoe[];
extern char str_kT_CHARGriff[];
extern char str_kT_CHARElise[];
extern char str_kT_CHARNate[];
extern char str_kT_CHARPsymon[];
extern char str_kT_CHARViggo[];
extern char str_kT_CMNGameTitle[];
extern char str_kT_CMNDJAtomica[];
extern char str_kT_FAQFolder[];

/**
 * Returns the string hash of a message sender's name, shown in the message center's From column.
 *
 * Kinds 0-9 are the characters (kT_CHARMoby, Kaori, Allegra, Mac, Zoe, Griff, Elise, Nate, Psymon, Viggo), 10 the game
 * title (kT_CMNGameTitle), 11 DJ Atomica (kT_CMNDJAtomica) and 12 the FAQ folder (kT_FAQFolder).
 *
 * @param kind  the sender (+0xC of the D_004C6C08 message)
 * @return GetHashValue32 of the name's string id, or 0 for other kinds
 */
extern "C" int getMessageSenderName(int kind)
{
    switch ((unsigned int)kind) {
    case 0:
        return GetHashValue32(str_kT_CHARMoby);
    case 1:
        return GetHashValue32(str_kT_CHARKaori);
    case 2:
        return GetHashValue32(str_kT_CHARAllegra);
    case 3:
        return GetHashValue32(str_kT_CHARMac);
    case 4:
        return GetHashValue32(str_kT_CHARZoe);
    case 5:
        return GetHashValue32(str_kT_CHARGriff);
    case 6:
        return GetHashValue32(str_kT_CHARElise);
    case 7:
        return GetHashValue32(str_kT_CHARNate);
    case 8:
        return GetHashValue32(str_kT_CHARPsymon);
    case 9:
        return GetHashValue32(str_kT_CHARViggo);
    case 10:
        return GetHashValue32(str_kT_CMNGameTitle);
    case 11:
        return GetHashValue32(str_kT_CMNDJAtomica);
    case 12:
        return GetHashValue32(str_kT_FAQFolder);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueMessage);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_hud_b[];
extern "C" void removeMessageAt(int idx);
extern "C" unsigned char pickMessageVariant(int i);
extern "C" void* cUIStateStack_findState(void* list, int hash);

struct sPopupListEntry_2FE0 {
    int id;
    unsigned char flag;
    char pad_0x5[3];
};
struct sPopupList_2FE0 {
    sPopupListEntry_2FE0 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList_2FE0* getMessageList_list2FE0() __asm__("getMessageList");

struct sPopupEntry18_2FE0 {   // a D_004C6C08 message (names ours)
    int field_0x0;
    int groupIndex;
    int kind;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_2FE0 D_004C6C08_2FE0[] __asm__("D_004C6C08");


/**
 * Adds a message to the human rider's message list as unread and notifies the HUD.
 *
 * The list (getMessageList) holds up to 25 {message, variant byte} entries, a read mask at +0xC8 and a count byte at
 * +0xCC. When it is full the oldest entry is dropped first (removeMessageAt(0)). A message whose D_004C6C08 kind (+0x8)
 * is 1 gets a random variant from pickMessageVariant instead of @p flag. The new entry's read bit is cleared; then, when the
 * game has a UI, the "hud_b" state found on its state stack gets handleEvent (vtable entry 24) with event 8.
 *
 * @param id    the message index (D_004C6C08)
 * @param flag  the variant byte stored with it
 */
extern "C" void queueMessage(int id, unsigned char flag)
{
    sPopupList_2FE0* messageList = getMessageList_list2FE0();
    if (messageList->count == 25)
        removeMessageAt(0);
    if (D_004C6C08_2FE0[id].kind == 1)
        flag = pickMessageVariant(id);
    messageList->entries[messageList->count].id = id;
    messageList->entries[messageList->count].flag = flag;
    messageList->mask &= ~(1 << messageList->count);
    cUIEngine* gameUi = SSXApp->Game->UI;
    if (((char*)gameUi) != 0) {
        cUIState* obj = (cUIState*)((char*)cUIStateStack_findState(&gameUi->mStateStack, GetHashValue32(str_hud_b)));
        if (((char*)obj) != 0) {
            obj->handleEvent(8, 0);
        }
    }
    messageList->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", pickMessageVariant);
#ifdef SKIP_ASM
extern "C" bool isMessageInCategory(int v, int i);
unsigned int BXrand();

/**
 * Picks a random subject variant for message @p i by its group's category.
 *
 * Category 3 gives 0-9, 5 gives 0-10, 8 gives 0-3 and 4 gives 0-9 (BXrand); any other category gives 0xFF.
 *
 * @param i  the message index
 * @return the variant, or 0xFF
 */
extern "C" unsigned char pickMessageVariant(int i)
{
    unsigned char pick = 0xFF;
    if (isMessageInCategory(3, i)) {
        pick = BXrand() % 10;
    } else if (isMessageInCategory(5, i)) {
        pick = BXrand() % 11;
    } else if (isMessageInCategory(8, i)) {
        pick = BXrand() % 4;
    } else if (isMessageInCategory(4, i)) {
        pick = BXrand() % 10;
    }
    return pick;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", removeMessageAt);
#ifdef SKIP_ASM
struct sPopupListEntry_31B8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_31B8 {
    sPopupListEntry_31B8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList_31B8* getMessageList_list31B8() __asm__("getMessageList");

/**
 * Removes entry @p idx from the message list, moving later entries and their read bits down one place.
 *
 * The read bit of the old last slot is left as it was.
 *
 * @param idx  the list position to remove
 */
extern "C" void removeMessageAt(int idx)
{
    sPopupList_31B8* msgList = getMessageList_list31B8();
    int i;
    for (i = idx; i < msgList->count - 1; i++) {
        msgList->entries[i] = msgList->entries[i + 1];
        int bit = 1 << (i + 1);
        if (msgList->mask & bit) {
            msgList->mask |= 1 << i;
        } else {
            msgList->mask &= ~(1 << i);
        }
    }
    msgList->count--;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", removeMessage);
#ifdef SKIP_ASM
struct sPopupListEntry {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList {
    sPopupListEntry entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList* getMessageList_list() __asm__("getMessageList");
extern "C" void removeMessageAt(int idx);

/**
 * Removes the first message-list entry holding message @p id, if any.
 *
 * @param id  the message index
 */
extern "C" void removeMessage(int id)
{
    sPopupList* msgList = getMessageList_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        if (id == msgList->entries[i].id) {
            removeMessageAt(i);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", removeGroupMessages);
#ifdef SKIP_ASM
struct sPopupEntry18_32C8 {
    int field_0x0;
    int groupIndex;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_32C8 D_004C6C08_32C8[] __asm__("D_004C6C08");
struct sPopupListEntry_32C8 {
    int id;
    unsigned char field_0x4;
    char pad_0x5[3];
};
struct sPopupList_32C8 {
    sPopupListEntry_32C8 entries[25];
    int mask;
    unsigned char count;
};
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList_32C8* getMessageList_list32C8() __asm__("getMessageList");
extern "C" void removeMessageAt(int idx);

/**
 * Removes every queued message that belongs to message group @p type.
 *
 * Likely bug: the loop compares an iteration count with the shrinking list count, so after each removal one entry at
 * the end of the list is never examined.
 *
 * @param type  the message group (D_00441630 index) whose messages are removed
 */
extern "C" void removeGroupMessages(int type)
{
    int n = 0;
    int j = 0;
    sPopupList_32C8* messageList = getMessageList_list32C8();
    for (; n < messageList->count; n++) {
        sPopupEntry18_32C8* messageEntry = &D_004C6C08_32C8[messageList->entries[j].id];
        if (type == messageEntry->groupIndex) {
            removeMessageAt(j);
        } else {
            j++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", findMessage);
#ifdef SKIP_ASM
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList* getMessageList_list() __asm__("getMessageList");

/**
 * Returns the message-list position of message @p id.
 *
 * @param id  the message index
 * @return its position, or -1 when it is not queued
 */
extern "C" int findMessage(int id)
{
    sPopupList* msgList = getMessageList_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        if (id == msgList->entries[i].id) {
            return i;
        }
    }
    return -1;
}
#endif

extern "C" void* getMessageList(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageCount__FPv);
#ifdef SKIP_ASM
/**
 * Returns how many messages the human rider's message list holds (its count byte at +0xCC).
 *
 * @return the message count
 */
unsigned char getMessageCount(void* self)
{
    return getMessageList_list()->count;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", countUnreadMessages);
#ifdef SKIP_ASM
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList* getMessageList_list() __asm__("getMessageList");

/**
 * Counts the unread messages in the human rider's message list (entries whose read bit at +0xC8 is clear).
 *
 * The pause menu pulses its "NewMail" icon while this is nonzero; the message center prints it as the unread count.
 *
 * @return the number of unread messages
 */
extern "C" int countUnreadMessages()
{
    sPopupList* msgList = getMessageList_list();
    int n = 0;
    int i;
    for (i = 0; i < msgList->count; i++) {
        int bit = 1 << i;
        if (!(msgList->mask & bit)) {
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", findUnreadMessage);
#ifdef SKIP_ASM
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupList* getMessageList_list() __asm__("getMessageList");
extern "C" int getListMessageId(int idx);

/**
 * Finds the first unread message in the human rider's message list.
 *
 * @param out  receives the list position when one is found
 * @return its message index (getListMessageId), or -1 when every message is read
 */
extern "C" int findUnreadMessage(int* out)
{
    sPopupList* msgList = getMessageList_list();
    int i;
    for (i = 0; i < msgList->count; i++) {
        int bit = 1 << i;
        if (!(msgList->mask & bit)) {
            *out = i;
            return getListMessageId(i);
        }
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("fe/fepopupmisc", getNextMessageInGroup);
#ifdef SKIP_ASM
/**
 * Returns the next message of the same group as message @p i: @p i + 1 when that message is in the same group.
 *
 * @param i  the message index
 * @return i + 1, or -1 when message i is the last of its group
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int getNextMessageInGroup(int i)
{
    sPopupEntry18* currentEntry = &D_004C6C08[i];
    int n = i + 1;
    sPopupEntry18* followingEntry = &D_004C6C08[n];
    if (followingEntry->groupIndex == currentEntry->groupIndex)
        return n;
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", requestMessageCenter);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setHelpDone(void* iface, int a1, int a2);
extern int D_004A20CC;

/**
 * Asks for the message center to open, from a world script (MessageCenterFunc, when not in a network game).
 *
 * When the new race's cGamePath (newRace.cGamePath) is 0, sets D_004A20CC to 0, so the game update triggers overlay 0x22 (the
 * message center) on its next pass, and sets player 0's flag through cBENewPlayerInterface_setHelpDone(player interface, 0, 1); otherwise
 * does nothing.
 */
extern "C" void requestMessageCenter(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        D_004A20CC = 0;
        cBENewPlayerInterface_setHelpDone(cBE_getInterface_Fv(cBE_getBE(), 1), 0, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", canOpenMessageCenter);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHelpDone(void* iface, int a1);

/**
 * Reports whether a world script may open the message center (ShowMessageIconFunc returns it to the script).
 *
 * Returns false when the new race's cGamePath is not kGAMEPATH_WORLDCIRCUIT or its cGameEvent is
 * kGAMEEVENT_TIME_PEAK1 or kGAMEEVENT_POINTS_PEAK1; otherwise true while player 0's flag read
 * by cBENewPlayerInterface_getHelpDone is still clear (requestMessageCenter sets it).
 *
 * @return true when the message center may be opened, else false
 */
extern "C" bool canOpenMessageCenter(void)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath != kGAMEPATH_WORLDCIRCUIT) {
        return false;
    }
    if (cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_TIME_PEAK1
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_POINTS_PEAK1) {
        return false;
    }
    return cBENewPlayerInterface_getHelpDone(cBE_getInterface_Fv(cBE_getBE(), 1), 0) == 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageListEntry);
#ifdef SKIP_ASM
extern "C" void* getMessageList(void* self);

/**
 * Returns the address of message-list entry @p index (the list plus 8 bytes per entry).
 *
 * @param index  the entry's index
 * @return the entry's address
 */
extern "C" sPopupListEntry* getMessageListEntry(int index)
{
    return &getMessageList_list()->entries[index];
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getListMessageId);
#ifdef SKIP_ASM
struct sPopupEntry8 {
    int messageId;
    unsigned char variant;
    char pad_0x5[3];
};
struct sPopupTable8 {
    sPopupEntry8 entries[1];
};
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupTable8* getMessageList_get() __asm__("getMessageList");

/**
 * Returns the message index stored in message-list entry @p idx.
 *
 * @param idx  the list position
 * @return the message index (D_004C6C08)
 */
extern "C" int getListMessageId(int idx)
{
    return getMessageList_get()->entries[idx].messageId;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getListMessageVariant);
#ifdef SKIP_ASM
// PORT: unit declares getMessageList(void*), but its body takes no arguments
extern "C" sPopupTable8* getMessageList_get() __asm__("getMessageList");

/**
 * Returns the variant byte stored in message-list entry @p idx.
 *
 * @param idx  the list position
 * @return the subject variant (0xFF for none)
 */
extern "C" unsigned char getListMessageVariant(int idx)
{
    return getMessageList_get()->entries[idx].variant;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageRow);
#ifdef SKIP_ASM
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern int D_004A20C4;

/**
 * Returns the message-center row that shows message @p id.
 *
 * The rows list the queued messages newest first, then the FAQ rows (type 4, built by getMessageForRow with collapsed
 * folders as one row). Returns @p id unchanged when D_004A20C4 is set.
 *
 * @param id  the message index
 * @return the row, or 0xFF when the message is not shown
 */
extern "C" unsigned char getMessageRow(int id)
{
    if (D_004A20C4 != 0) {
        return id;
    }
    int faqRowCount = countMessageRows(4, 0);
    int queuedCount = getMessageCount_noarg();
    int i;
    for (i = 0; i < faqRowCount + queuedCount; i++) {
        int k = i + 1;
        int idx = queuedCount - k;
        if (idx >= 0) {
            if (getListMessageId(idx) == id) {
                return i;
            }
        } else if (idx + faqRowCount >= 0) {
            if (getMessageForRow(4, ~idx, 1) == id) {
                return i;
            }
        }
    }
    return 0xFF;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueEarningsMessages);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern "C" int cBEEconInterface_getTotalMoneyEarned1(void* self, int rider);
int cBERewardInterface_getEarningsTarget(void* self, int a1);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

/**
 * Queues the earnings messages when an amount of career money would reach a peak's earnings target.
 *
 * Only for rider 0. When peak 1 is locked (cBENewPlayerInterface_isPeakLocked1) and the money earned so far plus @p amt
 * reaches cBERewardInterface_getEarningsTarget(.., 0), sets flag 52 and queues message 247 once; the same for peak 2
 * with target 1, flag 53 and message 248. cRiderMetrix_evAddCash calls it before crediting the money.
 *
 * @param rider  the rider's AI index (only 0 does anything)
 * @param amt    the money about to be credited
 */
extern "C" void queueEarningsMessages(int rider, int amt)
{
    if (rider == 0) {
        void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
        void* rew = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
        if (cBENewPlayerInterface_isPeakLocked1(pl, 0, 1)) {
            if ((unsigned)(cBEEconInterface_getTotalMoneyEarned1(econ,
                    0) + amt) >= (unsigned)cBERewardInterface_getEarningsTarget(rew, 0) && isMessageFlagSet(52) == 0) {
                setMessageFlag(52, 1);
                queueMessage(247, 0);
            }
        }
        if (cBENewPlayerInterface_isPeakLocked1(pl, rider, 2)) {
            if ((unsigned)(cBEEconInterface_getTotalMoneyEarned1(econ,
                    rider) + amt) >= (unsigned)cBERewardInterface_getEarningsTarget(rew, 1) && isMessageFlagSet(53) == 0) {
                setMessageFlag(53, 1);
                queueMessage(0xF8, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", queueTrophyMessages);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind); bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
int cBENewPlayerInterface_getRiderCharID(void* self, int player);
extern "C" int cBENewPlayerInterface_isPeakLocked1(void* self, int rider, int peak);
extern "C" int cBERewardInterface_isTrophyOwned(void* self, int rider, int a, int b, int c);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
extern "C" void queueMessage(int a, unsigned char b);

/**
 * Queues the trophy messages for the locked peaks once the human rider owns the needed trophy.
 *
 * Only for player 0. When peak 1 is locked and cBERewardInterface_isTrophyOwned(.., character, 0, 2) is true, sets flag
 * 54 and queues message 249 once; peak 2 with trophy (1, 2), flag 55 and message 250 likewise.
 * cBERewardInterface_awardUser calls it for award categories 11 and 12.
 *
 * @param player  the profile (only 0 does anything)
 */
extern "C" void queueTrophyMessages(int player)
{
    if (player == 0) {
        void* stats = cBE_getInterface_Fv(cBE_getBE(), 13);
        void* np = cBE_getInterface_Fv(cBE_getBE(), 1);
        int charID = cBENewPlayerInterface_getRiderCharID(np, cBENewPlayerInterface_getHumanRider(np, 0));
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 1)) {
            if (cBERewardInterface_isTrophyOwned(stats, player, charID, 0, 2) && isMessageFlagSet(54) == 0) {
                setMessageFlag(54, 1);
                queueMessage(249, 0);
            }
        }
        if (cBENewPlayerInterface_isPeakLocked1(np, player, 2)) {
            if (cBERewardInterface_isTrophyOwned(stats, player, charID, 1, 2) && isMessageFlagSet(55) == 0) {
                setMessageFlag(55, 1);
                queueMessage(250, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", getMessageList);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewPlayerInterface_getRiderMessages(void* iface, int a1);

/**
 * Returns the human rider's message list: cBENewPlayerInterface_getRiderMessages(player interface, 0), the character record at +0xE38.
 *
 * The list holds 25 {message, variant byte} entries, a read mask at +0xC8 and a count byte at +0xCC. The declared
 * parameter is not used.
 *
 * @return the message list
 */
extern "C" void* getMessageList(void* self)
{
    return cBENewPlayerInterface_getRiderMessages(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", isMessageFlagSet);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getMessageGroupFlag(void* iface, int a1, int flag);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

/**
 * Reports whether the human rider's message flag number @p group is set (cBENewPlayerInterface_getMessageGroupFlag on the player interface).
 *
 * The flags are the bits of the character record's words at +0xF08.
 *
 * @param group  the message group (flag) index
 * @return true when the flag is set, else false
 */
extern "C" bool isMessageFlagSet(int group)
{
    return cBENewPlayerInterface_getMessageGroupFlag(cBE_getInterface_Fv(cBE_getBE(), 1), 0, group) != 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", setMessageFlag);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBENewPlayerInterface_setMessageGroupFlag(void* iface, int a1, int a2, int a3);

/**
 * Sets or clears the human rider's message flag number @p flagIndex (cBENewPlayerInterface_setMessageGroupFlag on the player interface).
 *
 * @param flagIndex  the flag index
 * @param enabled  nonzero to set the flag, 0 to clear it
 */
extern "C" void setMessageFlag(int flagIndex, int enabled)
{
    cBENewPlayerInterface_setMessageGroupFlag(cBE_getInterface_Fv(cBE_getBE(), 1), 0, flagIndex, enabled);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", vibratePad);
#ifdef SKIP_ASM
extern char* PadCache;

// The pad device (vtable 0x48E488: dtor, sampleButtons, numActuators, setActuator; On Tour cPadDevice).
class cPadDevice {
public:
    virtual ~cPadDevice();
    virtual int sampleButtons();
    virtual int numActuators();
    virtual void setActuator(int act, float frequency);
};

/**
 * Sets the two vibration levels of pad @p idx from two values.
 *
 * Uses the pad device at PadCache + 0x2EEC + 4 x idx. With x' = max(0, 0.91333 x - 18.5185) and y scaled by 0.99167,
 * when the device has 2 actuators (numActuators) sets actuator 0 to max((y - 0.5) / 100, (x' - 2) / 972.22, 0) and
 * actuator 1 to max((y - 70) / 160, (x' - 100) / 5000, 0). Same body as cFEStateOptionsController_vibratePad in
 * fe/festateoptions without its unused state argument. Likely bug: with PadCache null the device stays null and is
 * still called.
 *
 * @param idx  the pad (0 or 1)
 * @param x    the first level input
 * @param y    the second level input
 */
// PORT: uses g++'s >? (max) operator.
extern "C" void vibratePad(int idx, float x, float y)
{
    signed char i = idx;
    float adjusted = x * 0.9133333563804626f - 18.518518447875977f;
    float clamped = 0.0f;
    if (adjusted >= 0.0f) {
        clamped = adjusted;
    }
    y = y * 0.9916666746139526f;
    cPadDevice* gamepad = 0;
    if (PadCache != 0) {
        gamepad = *(cPadDevice**)(PadCache + (i << 2) + 0x2EEC);
    }
    if (gamepad->numActuators() == 2) {
        gamepad->setActuator(0, ((y - 0.5f) * 0.009999999776482582f >? (clamped - 2.0f) / 972.2222290039062f) >? 0.0f);
        gamepad->setActuator(1, ((y - 70.0f) * 0.0062500000931322575f >? (clamped - 100.0f) / 5000.0f) >? 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_cOVState_MESSAGECENTER);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self, void* engine);
extern "C" int cUITextureBank_getElementByHashname(void* self, int hash);
extern "C" bool isMessageFlagSet(int group);
extern "C" void setMessageFlag(int a, int b);
int GetHashValue32(char* str);
extern void* cOVState_vtbl[];
extern void* cOVState_MESSAGECENTER_vtbl[];
extern unsigned short str_sp_b[];
extern char str_folder[];
extern char str_minus_folder[];
extern char str_questmark_b[];
extern int D_004A20CC;

struct sObj_3C00 {
    char pad_0x0[0x9C];
    int mpUnreadMail[8];    // 0x9C (cOVState_MESSAGECENTER's names)
    int mpFAQIcon[8];       // 0xBC
    int mpFrom[8];          // 0xDC
    int mpSubject[8];       // 0xFC
    int mpNumber[8];        // 0x11C
    char pad_0x13C[0x164 - 0x13C];
    int miFolderTex;        // 0x164
    int miMinusFolderTex;   // 0x168
    int miQuestionMarkTex;  // 0x16C
};

/**
 * Constructs the in-game message center overlay: clears its widget tables and looks up its folder icons.
 *
 * Runs the cUIState constructor (cUIState_cUIState), then the inlined cOVState part (mOVID = @p id, vtable cOVState_vtbl,
 * mDoneTransition, mAllowPauseMenu and mRemovePDA 0, chat buffer " "), stores cOVState_MESSAGECENTER_vtbl, zeroes the eight-row
 * widget tables (unread mail, FAQ icon, from, subject, number) and the labels, menu, buttons and arrows (not
 * mpNoMessage), and looks up the "folder", "minus folder" and "questmark" textures in the engine's texture bank
 * (+0x58, cUITextureBank_getElementByHashname). miMiscData becomes the opening mode: 2 when D_004A20CC is -1, else 0 or 1 by whether
 * new-player hint 7 is set (isMessageFlagSet), which it then sets (setMessageFlag(7, 1)).
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param id  the overlay's id (On Tour cOVState ctor tOVStateEnum id; the overlay factory passes 0x22)
 * @return @p self
 */
extern "C" void* cOVState_MESSAGECENTER_cOVState_MESSAGECENTER(cOVState_MESSAGECENTER* self, cUIEngine* engine, int id)
{
    sObj_3C00* stateView = (sObj_3C00*)self;
    cUIState_cUIState(self, engine);
    self->mOVID = id;
    VPTR_OF(cListNode, self) = cOVState_vtbl;
    self->mDoneTransition = 0;
    self->mAllowPauseMenu = 0;
    self->mRemovePDA = 0;
    *(unsigned short*)((char*)self + 0x58) = str_sp_b[0];
    VPTR_OF(cListNode, self) = cOVState_MESSAGECENTER_vtbl;
    int i;
    for (i = 0; i < 8; i++) {
        stateView->mpUnreadMail[i] = 0;
        stateView->mpFAQIcon[i] = 0;
        stateView->mpFrom[i] = 0;
        stateView->mpSubject[i] = 0;
        stateView->mpNumber[i] = 0;
    }
    self->mpTotalMessagesLabel = 0;
    self->mpUnreadMessagesLabel = 0;
    self->mpMenu = 0;
    self->mpButtonSquare = 0;
    self->mpButtonText3 = 0;
    self->mpButtonCross = 0;
    self->mpButtonText1 = 0;
    self->mpArrowUp = 0;
    self->mpArrowDown = 0;
    {
        cUIEngine* eng = self->mpEngine;
        int nameHash = GetHashValue32(str_folder);
        stateView->miFolderTex = cUITextureBank_getElementByHashname(&eng->mTextureBank, nameHash);
    }
    {
        cUIEngine* eng = self->mpEngine;
        int nameHash = GetHashValue32(str_minus_folder);
        stateView->miMinusFolderTex = cUITextureBank_getElementByHashname(&eng->mTextureBank, nameHash);
    }
    {
        cUIEngine* eng = self->mpEngine;
        int nameHash = GetHashValue32(str_questmark_b);
        stateView->miQuestionMarkTex = cUITextureBank_getElementByHashname(&eng->mTextureBank, nameHash);
    }
    if (~D_004A20CC != 0) {
        if (isMessageFlagSet(7) != 0)
            self->miMiscData = 0;
        else
            self->miMiscData = 1;
        setMessageFlag(7, 1);
    } else {
        self->miMiscData = 2;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onCreateScreen);
#ifdef SKIP_ASM
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cOVState_MESSAGECENTER_fillRows(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_showMessageCounts(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_updateButtons(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_updateScrollArrows(cOVState_MESSAGECENTER*);
extern char str_112messagecenter[];

/**
 * Creates the message center's screen and fills it (vtable entry 4, onCreateScreen).
 *
 * Adds the "112messagecenter" screen (cUIEngine_addScreenByHashName), keeps it in mpMessageScreen and plays its first
 * frame, runs cOVState_onCreateScreen, adds the PDA template in opening modes 0 and 1 (miMiscData), zeroes
 * mScrollOffset, sizes the menu to the queued messages plus every FAQ row (cUIMenu_setScrollableMenu), refreshes the
 * rows, labels, buttons and arrows (cOVState_MESSAGECENTER_fillRows, cOVState_MESSAGECENTER_showMessageCounts, cOVState_MESSAGECENTER_updateButtons, cOVState_MESSAGECENTER_updateScrollArrows) and sets D_004A20C0
 * (message center open).
 */
void cOVState_MESSAGECENTER::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_112messagecenter), 0);
    mpMessageScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    int mode = miMiscData;
    if (mode == 0) {
        goto add;
    }
    if (mode == 1) {
    add:
        cOVStateManager_addPDATemplate();
    }
    mScrollOffset = 0;
    int n = getMessageCount_noarg();
    n += countMessageRows(4, 1);
    cUIMenu_setScrollableMenu(mpMenu, (unsigned char)n);
    this->fillRows();
    this->showMessageCounts();
    this->updateButtons();
    this->updateScrollArrows();
    D_004A20C0 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void setMessageFlag(int a, int b);
extern "C" void cOVStateManager_removePDATemplate(void);
extern "C" void cOVState_onDestroyScreen(void* self);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];

/**
 * Closes the message center's screen (vtable entry 5, onDestroyScreen).
 *
 * In opening modes 0 and 1 (miMiscData) sets message flag 7 (mode 0) or clears it (mode 1), removes the PDA template
 * and restores cPauseState_Flags from the saved pause-state stack (cPauseState_Stack, cPauseState_StackIdx, cPauseState_FlagsTable); then runs
 * cOVState_onDestroyScreen.
 */
void cOVState_MESSAGECENTER::onDestroyScreen()
{
    int openMode = miMiscData;
    if (openMode == 1) {
        setMessageFlag(7, 0);
        cOVStateManager_removePDATemplate();
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
    } else if (openMode == 0) {
        setMessageFlag(7, 1);
        cOVStateManager_removePDATemplate();
        cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
    }
    cOVState_onDestroyScreen(this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onWidgetCreate);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* D_004A20A8;
extern char* D_004A2098;
extern char str_btext3_b[];
extern char str_btext1[];
extern char str_TotalMessagesLabel[];
extern char str_UnreadMessagesLabel[];
extern char str_MessageView[];
extern char str_MenuFroms[];
extern char str_arr_down_b[];
extern char str_arr_up_b[];
extern char str_NoMessage[];
extern char str_UnreadMailpct_d[];
extern char str_FAQIconpct_d[];
extern char str_Frompct_d[];
extern char str_Subjectpct_d[];
extern char str_Numberpct_d[];


/**
 * Binds the message center's widgets by their hashed names as the screen creates them (vtable entry 18,
 * onWidgetCreate).
 *
 * Stores "ps2sq", "btext3", "ps2x", "btext1", "TotalMessagesLabel", "UnreadMessagesLabel", "MenuFroms", "arr_down",
 * "arr_up" and "NoMessage" (hidden) in their fields, hides "MessageView", and fills the eight-row tables from
 * "UnreadMail%d", "FAQIcon%d", "From%d", "Subject%d" (also setEllipses(1), vtable entry 27) and "Number%d".
 *
 * @param w  the widget being created (matched by its mHashName)
 */
void cOVState_MESSAGECENTER::onWidgetCreate(cUIThing* w)
{
    char buf[32];
    int widgetHash = w->mHashName;
    if (widgetHash == GetHashValue32(D_004A20A8))
        mpButtonSquare = w;
    else if (widgetHash == GetHashValue32(str_btext3_b))
        mpButtonText3 = w;
    if (widgetHash == GetHashValue32(D_004A2098)) {
        mpButtonCross = w;
        return;
    }
    if (widgetHash == GetHashValue32(str_btext1)) {
        mpButtonText1 = (cUIText*)w;
        return;
    }
    if (widgetHash == GetHashValue32(str_TotalMessagesLabel)) {
        mpTotalMessagesLabel = (cUIText*)w;
        return;
    }
    if (widgetHash == GetHashValue32(str_UnreadMessagesLabel)) {
        mpUnreadMessagesLabel = (cUIText*)w;
        return;
    }
    if (widgetHash == GetHashValue32(str_MessageView)) {
        w->setVisible(0);
        return;
    }
    if (widgetHash == GetHashValue32(str_MenuFroms)) {
        mpMenu = (cUIMenu*)w;
        return;
    }
    if (widgetHash == GetHashValue32(str_arr_down_b)) {
        mpArrowDown = w;
        return;
    }
    if (widgetHash == GetHashValue32(str_arr_up_b)) {
        mpArrowUp = w;
        return;
    }
    if (widgetHash == GetHashValue32(str_NoMessage)) {
        mpNoMessage = w;
        w->setVisible(0);
        return;
    }
    for (int i = 0; i < 8; i++) {
        sprintf(buf, str_UnreadMailpct_d, i);
        if (widgetHash == GetHashValue32(buf)) {
            mpUnreadMail[i] = w;
            break;
        }
        sprintf(buf, str_FAQIconpct_d, i);
        if (widgetHash == GetHashValue32(buf)) {
            mpFAQIcon[i] = w;
            return;
        }
        sprintf(buf, str_Frompct_d, i);
        if (widgetHash == GetHashValue32(buf)) {
            mpFrom[i] = (cUIText*)w;
            return;
        }
        sprintf(buf, str_Subjectpct_d, i);
        if (widgetHash == GetHashValue32(buf)) {
            mpSubject[i] = (cUIText*)w;
            ((cUIText*)w)->setEllipses(1);
            return;
        }
        sprintf(buf, str_Numberpct_d, i);
        if (widgetHash == GetHashValue32(buf)) {
            mpNumber[i] = (cUIText*)w;
            return;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" unsigned char getMessageRow(int id);
extern "C" void cOVState_MESSAGECENTER_openViewPopup(cOVState_MESSAGECENTER* self, int a1, int a2);


/**
 * Focuses the message center's menu and, when it opened by itself, shows message 37 (vtable entry 6, onGainFocus).
 *
 * In opening modes 0 and 1 (miMiscData) selects the row of message 37 (getMessageRow), focuses the menu (setFocus(1),
 * vtable entry 7) and opens the view popup for message 37 with number -1 (cOVState_MESSAGECENTER_openViewPopup); otherwise only focuses the
 * menu.
 */
void cOVState_MESSAGECENTER::onGainFocus(cUIState*)
{
    int state = miMiscData;
    if (state == 0) {
        goto reset;
    }
    if (state == 1) {
    reset:
        cUIMenu_setSelectedByIndex(mpMenu, getMessageRow(37));
        cUIMenu* menu = mpMenu;
        menu->setFocus(1);
        this->openViewPopup(37, -1);
    } else {
        cUIMenu* menu = mpMenu;
        menu->setFocus(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onWidgetEvent);
#ifdef SKIP_ASM
struct sPopupEntry18_4200 {
    int field_0x0;
    int id;                         // 0x4
    char pad_0x8[0x10];
};
extern sPopupEntry18_4200 D_004C6C08_4200[] __asm__("D_004C6C08");

struct sPopupEntry14_4200 {
    int type;
    int start;                      // 0x4
    int count;                      // 0x8
    int field_0xC;
    int enabled;                    // 0x10
};
extern sPopupEntry14_4200 D_00441630_4200[] __asm__("D_00441630");

struct sVEntry001E4200 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct cUIScreen;
int cUIScreen_getFrameByLabel(cUIScreen* screen, int hash);
extern char str_TransitionOut_b[];
extern int D_004A20C0;

// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
// PORT: isMessageFlagSet is declared int; its result is used as a bool here
extern "C" bool isMessageFlagSet_b(int id) __asm__("isMessageFlagSet");
extern "C" int getListMessageId(int idx);
extern "C" void setMessageFlag(int a, int b);
extern "C" void cOVState_MESSAGECENTER_fillRows(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_updateButtons(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_updateScrollArrows(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_openViewPopup(cOVState_MESSAGECENTER* self, int a1, int a2);
extern "C" void cOVState_MESSAGECENTER_deleteMessage(cOVState_MESSAGECENTER* self, int idx);

/**
 * Handles the message center's widget events (vtable entry 19, onWidgetEvent).
 *
 * kONNEXT (select): on a queued-message row marks it read and opens the view popup with its row number (cOVState_MESSAGECENTER_openViewPopup);
 * on a FAQ row either toggles the folder (when the row is a folder's first message: flips the group's message flag and
 * refreshes rows, arrows and buttons) or opens the view popup. kONOPTION: clears mbMessageDeleted and, on a queued row,
 * deletes that message (cOVState_MESSAGECENTER_deleteMessage) and sets mbMessageDeleted, then refreshes the buttons. kONPREV: plays the
 * screen's "TransitionOut" frame, calls cUIStateStack_pop(mpEngine + 0x18, 1) and clears D_004A20C0. kONGAINFOCUS:
 * calls vtable entry 5 with 0 on the eight subject texts. Ignores a null @p widget.
 *
 * @param widget  the widget (a row's From text holds its message index in mMiscData)
 * @param event   the widget event
 */
void cOVState_MESSAGECENTER::onWidgetEvent(cUIThing* widget, unsigned int event)
{
    if (widget == 0) {
        return;
    }
    sPopupList* list = getMessageList_list();
    int faqRowCount = countMessageRows(4, 1);
    int queuedCount = getMessageCount_noarg();
    switch (event) {
    case cUIState::kONNEXT:
        if (faqRowCount + queuedCount > 0) {
            int k = mpMenu->mScrollItemCurrent;
            int idx = queuedCount - k - 1;
            if (idx >= 0) {
                int id = getListMessageId(idx);
                list->mask |= 1 << idx;
                this->openViewPopup(id, k + 1);
            } else if (idx + faqRowCount >= 0) {
                int w = widget->mMiscData;
                sPopupEntry18_4200* messageEntry = &D_004C6C08_4200[w];
                sPopupEntry14_4200* groupEntry = &D_00441630_4200[messageEntry->id];
                if (groupEntry->enabled != 0 && w == groupEntry->start) {
                    setMessageFlag(messageEntry->id, !isMessageFlagSet_b(messageEntry->id));
                    this->fillRows();
                    this->updateScrollArrows();
                    this->updateButtons();
                } else {
                    this->openViewPopup(w, k + 1);
                }
            }
        }
        break;
    case cUIState::kONOPTION:
        mbMessageDeleted = 0;
        if (queuedCount > 0) {
            int idx = queuedCount - mpMenu->mScrollItemCurrent - 1;
            if (idx >= 0) {
                this->deleteMessage(idx);
                mbMessageDeleted = 1;
            }
            this->updateButtons();
        }
        break;
    case cUIState::kONPREV: {
        int outFrame = cUIScreen_getFrameByLabel(mpMessageScreen, GetHashValue32(str_TransitionOut_b));
        if (outFrame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(mpMessageScreen, outFrame, 1);
        }
        cUIStateStack_pop(&mpEngine->mStateStack, 1);
        D_004A20C0 = 0;
        break;
    }
    case cUIState::kONGAINFOCUS: {
        int i;
        for (i = 0; i < 8; i++) {
            void* subjectText = ((void**)&mpSubject)[i];
            sVEntry001E4200* vt = *(sVEntry001E4200**)((char*)subjectText + 8);
            vt[5].fn(vthis(subjectText, vt[5]), 0);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_updateButtons);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern "C" int getMessageForRow(int type, int idx, int mode);
extern "C" bool isMessageFlagSet(int group);
extern "C" void cOVState_MESSAGECENTER_hideWidget(cOVState_MESSAGECENTER* self, cUIThing* obj);
extern "C" void cOVState_MESSAGECENTER_showWidget(cOVState_MESSAGECENTER* self, cUIThing* obj);
extern char str_kT_BTNView[];
extern char str_kT_BTNCollapse[];
extern char str_kT_BTNExpand[];

struct sEntry18_4458 {   // a D_004C6C08 message (names ours)
    int field_0x0;
    int groupIndex;
    char pad_0x8[0x18 - 0x8];
};
extern sEntry18_4458 D_004C6C08_4458[] __asm__("D_004C6C08");
struct sEntry14_4458 {   // a D_00441630 message group (names ours)
    int type;
    int start;
    char pad_0x8[0x14 - 0x8];
};
extern sEntry14_4458 D_00441630_4458[] __asm__("D_00441630");

/**
 * Updates the message center's buttons for the selected row.
 *
 * On a queued-message row shows the square button and "btext3" and labels "btext1" kT_BTNView. On a FAQ row hides them
 * and labels "btext1" kT_BTNCollapse or kT_BTNExpand on a folder's first message (by the folder's message flag), else
 * kT_BTNView.
 */
void cOVState_MESSAGECENTER::updateButtons()
{
    int base = mpMenu->mScrollItemCurrent;
    int diff = getMessageCount_noarg() - base;
    if (diff - 1 >= 0) {
        showWidget(mpButtonSquare);
        showWidget(mpButtonText3);
        if (mpButtonText1 != 0)
            cUIText_setUnicodeStringByID(mpButtonText1, GetHashValue32(str_kT_BTNView));
    } else {
        hideWidget(mpButtonSquare);
        hideWidget(mpButtonText3);
        if (mpButtonText1 != 0) {
            int id = getMessageForRow(4, -diff, 1);
            sEntry18_4458* messageEntry = &D_004C6C08_4458[id];
            int groupIndex = messageEntry->groupIndex;
            sEntry14_4458* groupEntry = &D_00441630_4458[groupIndex];
            if (id == groupEntry->start) {
                if (isMessageFlagSet(groupIndex))
                    cUIText_setUnicodeStringByID(mpButtonText1, GetHashValue32(str_kT_BTNCollapse));
                else
                    cUIText_setUnicodeStringByID(mpButtonText1, GetHashValue32(str_kT_BTNExpand));
            } else {
                cUIText_setUnicodeStringByID(mpButtonText1, GetHashValue32(str_kT_BTNView));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onWidgetMenu);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cOVState_MESSAGECENTER_updateScrollArrows(cOVState_MESSAGECENTER* self);
extern "C" void cOVState_MESSAGECENTER_fillRows(cOVState_MESSAGECENTER* self);
extern "C" void cOVState_MESSAGECENTER_updateButtons(cOVState_MESSAGECENTER* self);

/**
 * Answers the message center menu's events (vtable entry 20, onWidgetMenu).
 *
 * kSCROLLUPDATE stores @p value in mScrollOffset and refreshes arrows and rows; kSCROLLCHANGE refreshes the buttons;
 * kONOPTION reports whether mbMessageDeleted is set; kONNEXT and kONPREVIOUS return 1 and kONMISC returns 0.
 *
 * @param widget  the menu
 * @param event   the menu event
 * @param value   the event's value (the new scroll offset for kSCROLLUPDATE)
 * @return 257 for kSCROLLUPDATE, kSCROLLCHANGE and others, the answers above otherwise
 */
int cOVState_MESSAGECENTER::onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int value)
{
    switch (event) {
    case cUIMenu::kSCROLLUPDATE:
        mScrollOffset = value;
        this->updateScrollArrows();
        this->fillRows();
        break;
    case cUIMenu::kSCROLLCHANGE:
        this->updateButtons();
        return 257;
    case cUIMenu::kONMISC:
        return 0;
    case cUIMenu::kONOPTION:
        return mbMessageDeleted != 0;
    case cUIMenu::kONNEXT:
    case cUIMenu::kONPREVIOUS:
        return 1;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void cOVState_MESSAGECENTER_fillRows(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_showMessageCounts(cOVState_MESSAGECENTER*);
extern "C" void cUIStateStack_pop(void* self, int a1);

/**
 * Refreshes the message center when its view popup closes (vtable entry 17, onPopupEvent).
 *
 * On kONSCREENPOPUPPREV (sent by the view popup) refreshes the rows and the count labels (cOVState_MESSAGECENTER_fillRows, cOVState_MESSAGECENTER_showMessageCounts)
 * and calls cUIStateStack_pop(mpEngine + 0x18, 1).
 *
 * @param popup  the popup
 * @param event  the popup event
 */
void cOVState_MESSAGECENTER::onPopupEvent(cUIState* popup, unsigned int event)
{
    if (event == cUIState::kONSCREENPOPUPPREV) {
        this->fillRows();
        this->showMessageCounts();
        cUIStateStack_pop(&mpEngine->mStateStack, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_updateScrollArrows);
#ifdef SKIP_ASM
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern "C" int countMessageRows(int type, int all);
extern int D_004A20C4;


/**
 * Shows or hides the message center's scroll arrows.
 *
 * mpArrowUp is shown while mScrollOffset is not 0; mpArrowDown while mScrollOffset + 8 is below the row count (the
 * queued messages plus the FAQ rows, every FAQ row when D_004A20C4 is set).
 */
void cOVState_MESSAGECENTER::updateScrollArrows()
{
    if (mScrollOffset == 0) {
        cUIThing* arrowWidget = mpArrowUp;
        arrowWidget->setVisible(0);
    } else {
        cUIThing* arrowWidget = mpArrowUp;
        arrowWidget->setVisible(1);
    }
    int n;
    if (D_004A20C4 != 0) {
        n = getMessageCount_noarg();
        n += countMessageRows(4, 1);
    } else {
        n = getMessageCount_noarg();
        n += countMessageRows(4, 0);
    }
    if (mScrollOffset + 8 >= n) {
        cUIThing* arrowWidget = mpArrowDown;
        arrowWidget->setVisible(0);
    } else {
        cUIThing* arrowWidget = mpArrowDown;
        arrowWidget->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_showMessageCounts);
#ifdef SKIP_ASM
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNTotalMsg[];
extern char str_kT_OVRCMNUnread[];


/**
 * Prints the message center's total and unread counts.
 *
 * Formats kT_OVRCMNTotalMsg with the queued message count and kT_OVRCMNUnread with the unread count (countUnreadMessages),
 * each looked up through the front-end locale (vtable entry 4) and written with sprintf1 into mpTotalMessagesLabel and
 * mpUnreadMessagesLabel when present.
 */
void cOVState_MESSAGECENTER::showMessageCounts()
{
    unsigned short buf[0x38];
    if (mpTotalMessagesLabel != 0) {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, locale);
        unsigned short* fmt = vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_OVRCMNTotalMsg));
        sprintf1(buf, fmt, getMessageCount_noarg());
        cUIText_setUnicodeString(mpTotalMessagesLabel, buf);
    }
    if (mpUnreadMessagesLabel != 0) {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, locale);
        unsigned short* fmt = vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_OVRCMNUnread));
        sprintf1(buf, fmt, countUnreadMessages());
        cUIText_setUnicodeString(mpUnreadMessagesLabel, buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_hideWidget);
#ifdef SKIP_ASM

/**
 * Hides a widget (vtable entry 9, setVisible, with 0); does nothing for null.
 *
 * @param obj  the widget
 */
void cOVState_MESSAGECENTER::hideWidget(cUIThing* obj)
{
    if (obj != 0) {
        obj->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_showWidget);
#ifdef SKIP_ASM

/**
 * Shows a widget (vtable entry 9, setVisible, with 1); does nothing for null.
 *
 * @param obj  the widget
 */
void cOVState_MESSAGECENTER::showWidget(cUIThing* obj)
{
    if (obj != 0) {
        obj->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_fillRows);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_showObjSafe_48B0(void* self, char* name) __asm__("cUIState_showObjSafe");
extern "C" void cUIState_hideObjSafe_48B0(void* self, char* name) __asm__("cUIState_hideObjSafe");
extern "C" int countMessageRows_48B0(int kind, int online) __asm__("countMessageRows");
extern "C" int getMessageCount_48B0() __asm__("getMessageCount__FPv");
extern "C" void cOVState_MESSAGECENTER_hideWidget_48B0(void* self, int obj) __asm__("cOVState_MESSAGECENTER_hideWidget");
extern "C" void cOVState_MESSAGECENTER_showWidget_48B0(void* self, int obj) __asm__("cOVState_MESSAGECENTER_showWidget");
extern "C" int getListMessageId_48B0(int idx) __asm__("getListMessageId");
extern "C" int getListMessageVariant_48B0(int idx) __asm__("getListMessageVariant");
extern "C" void cOVState_MESSAGECENTER_fillRow_48B0(void* self, int idx, int row, int a3, int a4) __asm__("cOVState_MESSAGECENTER_fillRow");
extern "C" int getMessageForRow_48B0(int kind, int idx, int local) __asm__("getMessageForRow");
extern "C" int cOVState_MESSAGECENTER_getFAQSubjectVariant_48B0(void* self, int entry) __asm__("cOVState_MESSAGECENTER_getFAQSubjectVariant");
extern "C" void cUIMenu_setScrollableMenu_48B0(void* menu, unsigned char n) __asm__("cUIMenu_setScrollableMenu");
extern int D_004A20C4;
extern char str_MessageNumbers[];
extern char str_MessageFroms[];
extern char str_MessageSubjects[];
extern char str_UnreadMailBitmaps[];
extern char str_MessageHighlights[];
extern char str_NoMessages[];

struct sList_48B0 {
    char pad0[0x9C];
    int mpUnreadMail[8];    // 0x9C (cOVState_MESSAGECENTER's names)
    int mpFAQIcon[8];       // 0xBC
    int mpFrom[8];          // 0xDC
    int mpSubject[8];       // 0xFC
    int mpNumber[8];        // 0x11C
    char pad13C[0x144 - 0x13C];
    char* mpMenu;           // 0x144
    char pad148[0x14C - 0x148];
    int mpButtonSquare;     // 0x14C
    int mpButtonText3;      // 0x150
    int mpButtonCross;      // 0x154
    int mpButtonText1;      // 0x158
    char pad15C[0x174 - 0x15C];
    int mScrollOffset;      // 0x174
};

/**
 * Rebuilds the message center's eight visible rows.
 *
 * With no rows hides the "MessageNumbers", "MessageFroms", "MessageSubjects", "UnreadMailBitmaps" and
 * "MessageHighlights" groups, shows "NoMessages" and hides the buttons. Otherwise shows them and fills the rows from
 * mScrollOffset: queued messages newest first (cOVState_MESSAGECENTER_fillRow with the list position and the entry's variant), then FAQ
 * rows (cOVState_MESSAGECENTER_fillRow with -1 and cOVState_MESSAGECENTER_getFAQSubjectVariant's variant), hiding a row's widgets past the end. Finally resets the
 * menu's item count (cUIMenu_setScrollableMenu) while keeping its selected row (+0x95) and stores the scroll offset's
 * low byte in the menu's +0x98.
 *
 */
void cOVState_MESSAGECENTER::fillRows()
{
    sList_48B0* self = (sList_48B0*)this;
    int online;
    if (D_004A20C4) {
        online = countMessageRows_48B0(4, 1);
    } else {
        online = countMessageRows_48B0(4, 0);
    }
    int local = getMessageCount_48B0();
    int total = local + online;
    int sel;
    if (total == 0) {
        sel = 0;
        cUIState_hideObjSafe_48B0(self, str_MessageNumbers);
        cUIState_hideObjSafe_48B0(self, str_MessageFroms);
        cUIState_hideObjSafe_48B0(self, str_MessageSubjects);
        cUIState_hideObjSafe_48B0(self, str_UnreadMailBitmaps);
        cUIState_hideObjSafe_48B0(self, str_MessageHighlights);
        cUIState_showObjSafe_48B0(self, str_NoMessages);
        cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpButtonCross);
        cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpButtonText1);
        cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpButtonSquare);
        cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpButtonText3);
    } else {
        cUIState_showObjSafe_48B0(self, str_MessageNumbers);
        sel = total;
        cUIState_showObjSafe_48B0(self, str_MessageFroms);
        cUIState_showObjSafe_48B0(self, str_MessageSubjects);
        cUIState_showObjSafe_48B0(self, str_UnreadMailBitmaps);
        cUIState_hideObjSafe_48B0(self, str_NoMessages);
        cOVState_MESSAGECENTER_showWidget_48B0(self, self->mpButtonCross);
        cOVState_MESSAGECENTER_showWidget_48B0(self, self->mpButtonText1);
        for (int i = 0; i < 8; i++) {
            int n = i + 1;
            int k = local - self->mScrollOffset - n;
            if (k >= 0) {
                int messageIndex = getListMessageId_48B0(k);
                cOVState_MESSAGECENTER_fillRow_48B0(self, k, i, messageIndex, getListMessageVariant_48B0(k));
            } else if (k + online >= 0) {
                int nk = ~k;
                int faqMessage;
                if (D_004A20C4) {
                    faqMessage = getMessageForRow_48B0(4, nk, 0);
                } else {
                    faqMessage = getMessageForRow_48B0(4, nk, 1);
                }
                cOVState_MESSAGECENTER_fillRow_48B0(self, -1, i, faqMessage, (unsigned char)cOVState_MESSAGECENTER_getFAQSubjectVariant_48B0(self, faqMessage));
            } else {
                cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpUnreadMail[i]);
                cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpFAQIcon[i]);
                cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpFrom[i]);
                cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpNumber[i]);
                cOVState_MESSAGECENTER_hideWidget_48B0(self, self->mpSubject[i]);
            }
        }
    }
    unsigned char keep = self->mpMenu[0x95];
    cUIMenu_setScrollableMenu_48B0(self->mpMenu, sel);
    self->mpMenu[0x95] = keep;
    unsigned char scrollByte = self->mScrollOffset;
    self->mpMenu[0x98] = scrollByte;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_getFAQSubjectVariant);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A20C4;
extern "C" int getMessagePositionInCategory(int idx);

struct sPopupEntry18_4B80 {   // a D_004C6C08 message (names ours)
    int field_0x0;
    int groupIndex;
    int kind;
    char pad_0xC[0x18 - 0xC];
};
extern sPopupEntry18_4B80 D_004C6C08_4B80[] __asm__("D_004C6C08");

/**
 * Returns the subject variant of a FAQ-row message when D_004A20C4 is set.
 *
 * For a message of kind 1 (+0x8) takes its position in its category (getMessagePositionInCategory) and reduces it by the group's
 * category: 3 gives (position / 2) mod 10, 5 mod 11, 4 mod 10, 8 mod 4, others 0. Same rule as cOVState_ViewPopup_getSubjectVariant.
 *
 * @param idx  the message index
 * @return the variant, or 0xFF when D_004A20C4 is clear or the message is not of kind 1
 */
unsigned char cOVState_MESSAGECENTER::getFAQSubjectVariant(int idx)
{
    unsigned char variant = 0xFF;
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    sPopupEntry18_4B80* messageEntry = &D_004C6C08_4B80[idx];
    if (messageEntry->kind == 1) {
        int position = getMessagePositionInCategory(idx);
        int div = 1;
        sPopupEntry14* groupEntry = &D_00441630[messageEntry->groupIndex];
        switch (groupEntry->category) {
        case 3:
            div = 10;
            position = position / 2;
            break;
        case 5:
            div = 11;
            break;
        case 4:
            div = 10;
            break;
        case 8:
            div = 4;
            break;
        }
        variant = position % div;
    }
    return variant;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_fillRow);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int getMessageSenderName(int id);
extern "C" void cOVState_MESSAGECENTER_setRowSubject(cOVState_MESSAGECENTER* self, int slot, int id, unsigned char sub);
extern char str_plus[];
extern char str_dash[];
extern char str_sp_gt[];
extern char str_pct_d_d[];

// a D_004C6C08 message (names ours, from use: its group, kind, sender and subject string)
struct sEnt18_4C78 {
    int f0;
    int groupIndex;
    int kind;
    int sender;
    int subject;
    int f14;
};
extern sEnt18_4C78 D_004C6C08_4C78[] __asm__("D_004C6C08");

// a D_00441630 message group (names ours)
struct sEnt14_4C78 {
    int type;
    int start;
    int count;
    int category;
    int isFolder;
};
extern sEnt14_4C78 D_00441630_4C78[] __asm__("D_00441630");

struct sVEntry_4C78 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj_4C78 {
    int f0;
    int f4;
    sVEntry_4C78* vt;   // 0x08
    char padC[0xC];
    int mMiscData;      // 0x18 (cUIThing's)
    char pad1C[0x5C];
    int mTextureID;     // 0x78 (cUIIcon's)
    int mpTextureElem;  // 0x7C
};

struct sPopup_4C78 {
    char pad0[0x9C];
    sObj_4C78* mpUnreadMail[8];     // 0x9C (cOVState_MESSAGECENTER's names)
    sObj_4C78* mpFAQIcon[8];        // 0xBC
    sObj_4C78* mpFrom[8];           // 0xDC
    sObj_4C78* mpSubject[8];        // 0xFC
    sObj_4C78* mpNumber[8];         // 0x11C
    char pad13C[0x28];
    int miFolderTex;                // 0x164
    int miMinusFolderTex;           // 0x168
    int miQuestionMarkTex;          // 0x16C
    char pad170[4];
    int mScrollOffset;              // 0x174
};

static inline void setTexture_4C78(sObj_4C78* o, int elem, int textureId)
{
    o->mTextureID = textureId;
    o->mpTextureElem = elem;
}

static inline void show_4C78(sObj_4C78* o)
{
    o->vt[9].fn(vthis(o, o->vt[9]), 1);
}

/**
 * Fills one row of the message center with a message.
 *
 * For a FAQ row (@p idx -1) hides the unread icon and shows the FAQ icon, textured (+0x7C) as a closed folder
 * (miFolderTex), an open folder (miMinusFolderTex, folder flag set) or a question mark (miQuestionMarkTex, not a
 * folder's first message); for a queued message shows the unread icon while its read bit (+0xC8) is clear and hides the
 * FAQ icon. The number reads "+", "-" or " >" on folder rows, else "%d." with mScrollOffset + @p slot + 1. From gets
 * the sender's name (getMessageSenderName) and keeps @p id at +0x18. The subject is cOVState_MESSAGECENTER_setRowSubject's variant text for messages
 * of kind 1, else the message's subject id (+0x10).
 *
 * @param idx    the message-list position, or -1 for a FAQ row
 * @param slot   the row (0-7)
 * @param id     the message index (D_004C6C08)
 * @param sub    the subject variant
 */
void cOVState_MESSAGECENTER::fillRow(int idx, int slot, int id, unsigned char sub)
{
    sPopup_4C78* self = (sPopup_4C78*)this;
    char buf[16];
    sEnt18_4C78* messageEntry = &D_004C6C08_4C78[id];
    sEnt14_4C78* groupEntry = &D_00441630_4C78[messageEntry->groupIndex];
    char* messageList = (char*)getMessageList(self);
    if (idx == -1) {
        if (groupEntry->isFolder != 0) {
            if (id == groupEntry->start) {
                if (isMessageFlagSet(messageEntry->groupIndex) == 0)
                    setTexture_4C78(self->mpFAQIcon[slot], self->miFolderTex, idx);
                else
                    setTexture_4C78(self->mpFAQIcon[slot], self->miMinusFolderTex, idx);
            } else {
                setTexture_4C78(self->mpFAQIcon[slot], self->miQuestionMarkTex, idx);
            }
        }
        ((cOVState_MESSAGECENTER*)self)->hideWidget((cUIThing*)self->mpUnreadMail[slot]);
        ((cOVState_MESSAGECENTER*)self)->showWidget((cUIThing*)self->mpFAQIcon[slot]);
    } else {
        int mask = 1 << idx;
        if (*(int*)(messageList + 0xC8) & mask) {
            ((cOVState_MESSAGECENTER*)self)->hideWidget((cUIThing*)self->mpUnreadMail[slot]);
            ((cOVState_MESSAGECENTER*)self)->hideWidget((cUIThing*)self->mpFAQIcon[slot]);
        } else {
            ((cOVState_MESSAGECENTER*)self)->showWidget((cUIThing*)self->mpUnreadMail[slot]);
            ((cOVState_MESSAGECENTER*)self)->hideWidget((cUIThing*)self->mpFAQIcon[slot]);
        }
    }
    if (self->mpNumber[slot] != 0) {
        if (groupEntry->isFolder != 0) {
            if (id == groupEntry->start) {
                if (isMessageFlagSet(messageEntry->groupIndex) == 0)
                    sprintf(buf, str_plus);
                else
                    sprintf(buf, str_dash);
            } else {
                sprintf(buf, str_sp_gt);
            }
        } else {
            sprintf(buf, str_pct_d_d, self->mScrollOffset + slot + 1);
        }
        cUIText_setAsciiString((cUIText*)self->mpNumber[slot], buf);
        show_4C78(self->mpNumber[slot]);
    }
    if (self->mpFrom[slot] != 0) {
        cUIText_setUnicodeStringByID((cUIText*)self->mpFrom[slot], getMessageSenderName(messageEntry->sender));
        show_4C78(self->mpFrom[slot]);
        self->mpFrom[slot]->mMiscData = id;
    }
    if (messageEntry->kind == 1) {
        ((cOVState_MESSAGECENTER*)self)->setRowSubject(slot, id, sub);
    } else if (self->mpSubject[slot] != 0) {
        cUIText_setUnicodeStringByID((cUIText*)self->mpSubject[slot], messageEntry->subject);
        show_4C78(self->mpSubject[slot]);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_setRowSubject);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" unsigned char cOVState_ViewPopup_getSubjectVariant(cOVState_ViewPopup* self, int idx);
extern "C" int findMessage(int id);
extern "C" unsigned char getListMessageVariant(int idx);
extern "C" bool isMessageInCategory(int v, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A20C4;
extern char str_kT_MSGSubjectBCFinishLosspct_d[];
extern char str_kT_MSGSubjectBCFinishWinpct_d[];
extern char str_kT_MSGSubjectAggressionpct_d[];
extern char str_kT_MSGSubjectForeshadowpct_d[];
extern char str_kT_MSGSubjectBeatThePeakpct_d[];



/**
 * Sets a message-center row's subject to the text for a message's variant.
 *
 * Builds the string id from the message's category: kT_MSGSubjectBCFinishLoss%d / Win%d (odd / even messages 115-138)
 * for category 3, Aggression%d (5), Foreshadow%d (4), BeatThePeak%d (8), formatted with @p lvl, and sets it on
 * mpSubject[@p slot] (GetHashValue32) and shows it. Likely bug: for any other message the buffer is hashed without
 * being written.
 *
 * @param slot  the row (0-7)
 * @param id    the message index
 * @param lvl   the variant number
 */
void cOVState_MESSAGECENTER::setRowSubject(int slot, int id, unsigned char lvl)
{
    char buf[48];
    if (isMessageInCategory(3, id)) {
        switch (id) {
        case 115: case 117: case 119: case 121: case 123: case 125: case 0x7F: case 129: case 131: case 133: case 135: case 137:
            sprintf(buf, str_kT_MSGSubjectBCFinishLosspct_d, lvl);
            break;
        case 116: case 118: case 0x78: case 122: case 0x7C: case 0x7E: case 0x80: case 130: case 132: case 134: case 136: case 138:
            sprintf(buf, str_kT_MSGSubjectBCFinishWinpct_d, lvl);
            break;
        }
    } else if (isMessageInCategory(5, id)) {
        sprintf(buf, str_kT_MSGSubjectAggressionpct_d, lvl);
    } else if (isMessageInCategory(4, id)) {
        sprintf(buf, str_kT_MSGSubjectForeshadowpct_d, lvl);
    } else if (isMessageInCategory(8, id)) {
        sprintf(buf, str_kT_MSGSubjectBeatThePeakpct_d, lvl);
    }
    cOVState_MESSAGECENTER* view = this;
    if (view->mpSubject[slot] != 0) {
        cUIText_setUnicodeStringByID(view->mpSubject[slot], GetHashValue32(buf));
        cUIText* textObj = view->mpSubject[slot];
        textObj->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_openViewPopup);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cOVState_ViewPopup_cOVState_ViewPopup(cOVState_ViewPopup* self, cUIEngine* a1, cOVState_MESSAGECENTER* owner, int a3, int a4);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_cOVState_ViewPopup[];

/**
 * Opens the view popup for a message.
 *
 * Allocates a cOVState_ViewPopup (0x70 bytes, cMemMan_alloc), constructs it (cOVState_ViewPopup_cOVState_ViewPopup with
 * mpEngine, this, @p messageIndex, @p messageNumber) and pushes it (cUIStateStack_pushPopup) on the stack at +0x18 of the object reached
 * through mpMessageScreen's word +0xD0 and that object's +0x10.
 *
 * @param messageIndex  the message index (the popup's miMessage)
 * @param messageNumber  the number to show (miNumber; -1 from onGainFocus)
 */
void cOVState_MESSAGECENTER::openViewPopup(int messageIndex, int messageNumber)
{
    void* item = cOVState_ViewPopup_cOVState_ViewPopup((cOVState_ViewPopup*)cMemMan_alloc(0x70, str_cOVState_ViewPopup, 0, 0), mpEngine, this, messageIndex, messageNumber);
    void* screen = mpMessageScreen;
    void* screenOwner = *(void**)((char*)screen + 0xD0);
    void* ownerEngine = *(void**)((char*)screenOwner + 0x10);
    cUIStateStack_pushPopup((char*)ownerEngine + 0x18, item);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_MESSAGECENTER_deleteMessage);
#ifdef SKIP_ASM
extern "C" void removeMessageAt(int idx);
extern "C" int countMessageRows(int a0, int a1);
// PORT: getMessageCount is declared (void*) but never reads its argument; called here with none
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");
extern "C" void cOVState_MESSAGECENTER_fillRows(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_showMessageCounts(cOVState_MESSAGECENTER*);
extern "C" void cOVState_MESSAGECENTER_updateScrollArrows(cOVState_MESSAGECENTER*);

/**
 * Deletes a queued message from the message center and refreshes it.
 *
 * Removes list entry @p idx (removeMessageAt), moves the menu's selected row (mScrollItemCurrent) back by one when it is
 * now past the end, then refreshes the rows, labels and arrows.
 *
 * @param idx  the message-list position
 */
void cOVState_MESSAGECENTER::deleteMessage(int idx)
{
    removeMessageAt(idx);
    int cur = mpMenu->mScrollItemCurrent;
    int n = countMessageRows(4, 1);
    n += getMessageCount_noarg();
    if (cur >= n) {
        cur--;
    }
    mpMenu->mScrollItemCurrent = cur;
    fillRows();
    showMessageCounts();
    updateScrollArrows();
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_cOVState_ViewPopup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern void* cOVState_ViewPopup_vtbl[];

/**
 * Constructs the popup that shows one saved message (screen "113ViewMessage").
 *
 * Runs the cUIState constructor (cUIState_cUIState1), stores the message index and number, cOVState_ViewPopup_vtbl, and
 * zeroes every widget pointer.
 *
 * @param engine  the UI engine (the message center's mpEngine)
 * @param owner  the parent state (the cOVState_MESSAGECENTER)
 * @param message  the message's index in the saved list D_004C6C08 (miMessage)
 * @param number  the number shown for it (miNumber)
 * @return @p self
 */
extern "C" void* cOVState_ViewPopup_cOVState_ViewPopup(cOVState_ViewPopup* self, cUIEngine* engine, cOVState_MESSAGECENTER* owner, int message, int number)
{
    cUIState_cUIState1(self, engine, owner);
    self->miMessage = message;
    self->miNumber = number;
    VPTR_OF(cListNode, self) = cOVState_ViewPopup_vtbl;
    self->mpNumberLabel = 0;
    self->mpNumber = 0;
    self->mpFrom = 0;
    self->mpSubject = 0;
    self->mpButtonSquare = 0;
    self->mpDeleteMessage = 0;
    self->mpKeepMessage = 0;
    self->mpMessageLine0 = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_113ViewMessage[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Opens the message view popup's "113ViewMessage" screen into mpScreen and plays its frame 0.
 */
void cOVState_ViewPopup::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_113ViewMessage), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

extern "C" void* cUIState_onUpdate(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Runs one frame of the message view popup: only the cUIState update (cUIState_onUpdate).
 *
 * @return cUIState_onUpdate()'s result (not meaningful; declared void* by the unit)
 */
void* cOVState::ViewPopup_onUpdate()
{
    return cUIState_onUpdate(this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cOVState_ViewPopup_fillMessage(cOVState_ViewPopup* self);

/**
 * Fills the message view the first time it gains focus (cOVState_ViewPopup_fillMessage: number, sender, subject, body and buttons).
 *
 * @param on  nonzero on the first transition (On Tour bFirstTime)
 * @return always 0
 */
bool cOVState_ViewPopup::onGainTransition(int on)
{
    if (on != 0) {
        this->fillMessage();
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_From[];
extern char str_Subject[];
extern char str_NumberLabel[];
extern char str_Number[];
extern char str_MV_KeepMessage[];
extern char* D_004A20A8;
extern char str_MV_DeleteMessage[];
extern char str_MessageLine0[];
extern char str_MenuDummy[];


/**
 * Shows each of the message view's widgets as it is created and binds the ones it fills.
 *
 * Every widget is shown (setVisible, slot 9). "From", "Subject", "NumberLabel", "Number", "MV_KeepMessage", the square
 * button (D_004A20A8), "MV_DeleteMessage" and "MessageLine0" are stored in their members; the menu "MenuDummy" gets
 * mAttrMenu.mDisableDirSound (bit 3 of the word at 0x90).
 *
 * @param widget  the widget being created (On Tour pWidget)
 */
void cOVState_ViewPopup::onWidgetCreate(cUIThing* widget)
{
    int nameHash = widget->mHashName;
    widget->setVisible(1);
    if (nameHash == GetHashValue32(str_From)) {
        mpFrom = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_Subject)) {
        mpSubject = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_NumberLabel)) {
        mpNumberLabel = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_Number)) {
        mpNumber = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_MV_KeepMessage)) {
        mpKeepMessage = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(D_004A20A8)) {
        mpButtonSquare = widget;
    } else if (nameHash == GetHashValue32(str_MV_DeleteMessage)) {
        mpDeleteMessage = widget;
    } else if (nameHash == GetHashValue32(str_MessageLine0)) {
        mpMessageLine0 = (cUIText*)widget;
    } else if (nameHash == GetHashValue32(str_MenuDummy)) {
        ((cUIMenu*)widget)->mAttrMenu.mDisableDirSound = 1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_fillMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPopupEntry18_5428 {   // a D_004C6C08 message (names ours)
    int field_0x0;
    int groupIndex;
    char pad_0x8[0x4];
    int sender;
    char pad_0x10[0x8];
};
extern sPopupEntry18_5428 D_004C6C08_5428[] __asm__("D_004C6C08");

struct sPopupEntry14_5428 {
    int type;
    char pad_0x4[0x10];
};
extern sPopupEntry14_5428 D_00441630_5428[] __asm__("D_00441630");


struct sVEntryS1E5428 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

extern char str_kT_BTNKeepMsg[];
extern char str_kT_MSGFAQNumber[];
extern char D_004A2138[];

void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int getMessageSenderName(int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cOVState_ViewPopup_setSubject(cOVState_ViewPopup* self, int idx);
extern "C" void cOVState_ViewPopup_setBody(cOVState_ViewPopup* self, int idx);

/**
 * Fills the view popup with its message (miMessage).
 *
 * For a FAQ message (group type 4) hides the square button and mpDeleteMessage and sets mpKeepMessage to kT_BTNKeepMsg.
 * mpNumber shows kT_MSGFAQNumber with the message's position in its category (getMessagePositionInCategory) for FAQ messages, else
 * miNumber ("%d"). mpFrom gets the sender's name (getMessageSenderName); then the subject (cOVState_ViewPopup_setSubject) and the body
 * (cOVState_ViewPopup_setBody) are set.
 */
void cOVState_ViewPopup::fillMessage()
{
    sPopupEntry18_5428* messageEntry = &D_004C6C08_5428[miMessage];
    sPopupEntry14_5428* groupEntry = &D_00441630_5428[messageEntry->groupIndex];
    if (groupEntry->type == kMessageGroupFAQ) {
        cUIThing* buttonWidget = mpButtonSquare;
        if (buttonWidget != 0) {
            buttonWidget->setVisible(0);
        }
        buttonWidget = mpDeleteMessage;
        if (buttonWidget != 0) {
            buttonWidget->setVisible(0);
        }
        if (mpKeepMessage != 0) {
            cUIText_setUnicodeStringByID(mpKeepMessage, GetHashValue32(str_kT_BTNKeepMsg));
        }
    }
    if (mpNumber != 0) {
        if (groupEntry->type == kMessageGroupFAQ) {
            unsigned short buf[0x18];
            cFELocale* mgr = SSXApp->FELocale;
            sVEntryS1E5428* vt = (sVEntryS1E5428*)VPTR_OF(cUILocaleInterface, mgr);
            unsigned short* fmt = vt[4].fn(vthis(mgr, vt[4]), GetHashValue32(str_kT_MSGFAQNumber));
            sprintf1(buf, fmt, getMessagePositionInCategory(miMessage));
            cUIText_setUnicodeString(mpNumber, buf);
        } else {
            char buf[0x30];
            sprintf(buf, D_004A2138, miNumber);
            cUIText_setAsciiString(mpNumber, buf);
        }
    }
    if (mpFrom != 0) {
        cUIText_setUnicodeStringByID(mpFrom, getMessageSenderName(messageEntry->sender));
    }
    setSubject(miMessage);
    setBody(miMessage);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_setSubject);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cOVState_ViewPopup_setVariantSubject(cOVState_ViewPopup* self, int idx);

/**
 * Sets the view popup's subject: the variant text (cOVState_ViewPopup_setVariantSubject) for messages of kind 1, else the subject id (+0x10).
 *
 * @param idx  the message index
 */
void cOVState_ViewPopup::setSubject(int idx)
{
    char* messageEntry = (char*)&D_004C6C08[idx];
    if (*(int*)(messageEntry + 0x8) == 1) {
        setVariantSubject(idx);
    } else {
        cUIText* uiText = mpSubject;
        if (uiText != 0) {
            cUIText_setUnicodeStringByID(uiText, *(int*)(messageEntry + 0x10));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_setVariantSubject);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" unsigned char cOVState_ViewPopup_getSubjectVariant(cOVState_ViewPopup* self, int idx);
extern "C" int findMessage(int id);
extern "C" unsigned char getListMessageVariant(int idx);
extern "C" bool isMessageInCategory(int v, int i);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_004A20C4;
extern char str_kT_MSGSubjectBCFinishLosspct_d[];
extern char str_kT_MSGSubjectBCFinishWinpct_d[];
extern char str_kT_MSGSubjectAggressionpct_d[];
extern char str_kT_MSGSubjectForeshadowpct_d[];
extern char str_kT_MSGSubjectBeatThePeakpct_d[];

/**
 * Sets the view popup's subject to the text for a message's variant.
 *
 * The variant is cOVState_ViewPopup_getSubjectVariant's when D_004A20C4 is set, else the one stored with the queued message (getListMessageVariant of
 * its list position). The string id is built as in cOVState_MESSAGECENTER_setRowSubject (BCFinishLoss / Win, Aggression, Foreshadow,
 * BeatThePeak) and set on mpSubject.
 *
 * @param id  the message index
 */
void cOVState_ViewPopup::setVariantSubject(int id)
{
    char buf[32];
    unsigned char lvl;
    if (D_004A20C4 != 0) {
        lvl = getSubjectVariant(id);
    } else {
        lvl = getListMessageVariant(findMessage(id));
    }
    if (isMessageInCategory(3, id)) {
        switch (id) {
        case 115: case 117: case 119: case 121: case 123: case 125: case 0x7F: case 129: case 131: case 133: case 135: case 137:
            sprintf(buf, str_kT_MSGSubjectBCFinishLosspct_d, lvl);
            break;
        case 116: case 118: case 0x78: case 122: case 0x7C: case 0x7E: case 0x80: case 130: case 132: case 134: case 136: case 138:
            sprintf(buf, str_kT_MSGSubjectBCFinishWinpct_d, lvl);
            break;
        }
    } else if (isMessageInCategory(5, id)) {
        sprintf(buf, str_kT_MSGSubjectAggressionpct_d, lvl);
    } else if (isMessageInCategory(4, id)) {
        sprintf(buf, str_kT_MSGSubjectForeshadowpct_d, lvl);
    } else if (isMessageInCategory(8, id)) {
        sprintf(buf, str_kT_MSGSubjectBeatThePeakpct_d, lvl);
    }
    if (mpSubject != 0) {
        cUIText_setUnicodeStringByID(mpSubject, GetHashValue32(buf));
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_setBody);
#ifdef SKIP_ASM

/**
 * Sets and shows the view popup's body text for messages of kind 0 or 1.
 *
 * Sets 0x80 in mpMessageLine0's attribute word (+0x14), gives it the message's body id (+0x14) and shows it; messages
 * of other kinds leave it unchanged.
 *
 * @param idx  the message index
 */
void cOVState_ViewPopup::setBody(int idx)
{
    char* messageEntry = (char*)&D_004C6C08[idx];
    int kind = *(int*)(messageEntry + 0x8);
    if (kind < 2) {
        if (kind >= 0) {
            char* lineWidget = (char*)mpMessageLine0;
            *(int*)(lineWidget + 0x14) |= 0x80;
            cUIText_setUnicodeStringByID(mpMessageLine0, *(int*)(messageEntry + 0x14));
            cUIText* obj = mpMessageLine0;
            obj->setVisible(1);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPopupEntry18_5800 {
    int field_0x0;
    int groupIndex;
    char pad_0x8[0x18 - 0x8];
};
extern sPopupEntry18_5800 D_004C6C08_5800[] __asm__("D_004C6C08");
struct sEntry14_5800 {
    int type;
    char pad_0x4[0x14 - 0x4];
};
extern sEntry14_5800 D_00441630_5800[] __asm__("D_00441630");
extern "C" void removeMessage(int id);

/**
 * Handles the message view's widget events: delete the message, or close the popup.
 *
 * kONOPTION deletes the shown message from the saved list (removeMessage) unless its type (D_00441630[entry +0x4])
 * is kMessageGroupFAQ, in which case nothing happens; it then falls through to close. kONNEXT and kONPREV (and a
 * delete) tell the parent state's onPopupEvent (slot 17) kONSCREENPOPUPPREV (back) from this popup.
 *
 * @param pWidget  the widget (On Tour pWidget); not used
 * @param msg  the widget event (On Tour Event)
 */
void cOVState_ViewPopup::onWidgetEvent(cUIThing* pWidget, unsigned int msg)
{
    switch (msg) {
    case cUIState::kONOPTION: {
        int id = miMessage;
        sPopupEntry18_5800* popupEntry = &D_004C6C08_5800[id];
        sEntry14_5800* typeEntry = &D_00441630_5800[popupEntry->groupIndex];
        if (typeEntry->type == kMessageGroupFAQ) {
            return;
        }
        removeMessage(id);
    }
    case cUIState::kONNEXT: {
        cUIState* parentState = mpParentState;
        parentState->onPopupEvent((cUIState*)this, cUIState::kONSCREENPOPUPPREV);
        break;
    }
    case cUIState::kONPREV: {
        cUIState* parentState = mpParentState;
        parentState->onPopupEvent((cUIState*)this, cUIState::kONSCREENPOPUPPREV);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_showNextMessage);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" int getNextMessageInGroup(int i);
extern "C" int countUnreadMessages();
extern "C" int findUnreadMessage(int* out);
extern "C" void cOVState_ViewPopup_fillMessage(cOVState_ViewPopup* self);
// PORT: getMessageCount is declared (void*) but called with no argument here
unsigned char getMessageCount_noarg() __asm__("getMessageCount__FPv");

/**
 * Steps the view popup to the next message, or tells its parent to close it.
 *
 * For a FAQ message moves to the next message of the same folder (getNextMessageInGroup), adds 1 to miNumber and refills the
 * popup. Otherwise, while unread messages remain, shows the first unread one (findUnreadMessage), marks it read and sets
 * miNumber to its row number (count minus position). When there is nothing more it sends mpParentState onPopupEvent
 * (vtable entry 17) with this popup and kONSCREENPOPUPPREV.
 */
void cOVState_ViewPopup::showNextMessage()
{
    int id = miMessage;
    sPopupEntry18_5800* messageEntry = &D_004C6C08_5800[id];
    sEntry14_5800* groupEntry = &D_00441630_5800[messageEntry->groupIndex];
    if (groupEntry->type == kMessageGroupFAQ) {
        int n = getNextMessageInGroup(id);
        if (n == -1) {
            cUIState* parentState = mpParentState;
            parentState->onPopupEvent((cUIState*)this, cUIState::kONSCREENPOPUPPREV);
            return;
        }
        miMessage = n;
        miNumber = miNumber + 1;
        fillMessage();
    } else if (countUnreadMessages() > 0) {
        sPopupList* messageList = getMessageList_list();
        int idx;
        miMessage = findUnreadMessage(&idx);
        messageList->mask |= 1 << idx;
        miNumber = getMessageCount_noarg() - idx;
        fillMessage();
    } else {
        cUIState* parentState = mpParentState;
        parentState->onPopupEvent((cUIState*)this, cUIState::kONSCREENPOPUPPREV);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the message view's menu events; kONOPTION asks whether the shown message can be deleted.
 *
 * @param pMenu  the menu (On Tour pMenu); not used
 * @param Event  the menu event (On Tour Event)
 * @return 1 for kONNEXT and kONPREVIOUS, 0 for kONMISC, for kONOPTION whether the message's type is not
 *         kMessageGroupFAQ, otherwise 0x101
 */
int cOVState_ViewPopup::onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
{
    switch (Event) {
    case cUIMenu::kONNEXT:
    case cUIMenu::kONPREVIOUS:
        return 1;
    case cUIMenu::kONMISC:
        return 0;
    case cUIMenu::kONOPTION: {
        sPopupEntry18* popupEntry = &D_004C6C08[miMessage];
        sPopupEntry14* typeEntry = &D_00441630[popupEntry->groupIndex];
        return typeEntry->type != kMessageGroupFAQ;
    }
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_ViewPopup_getSubjectVariant);
#ifdef SKIP_ASM
extern int D_004A20C4;

/**
 * Returns a message's subject variant from its position in its category, when D_004A20C4 is set.
 *
 * Category 3 gives (position / 2) mod 10, 5 mod 11, 4 mod 10, 8 mod 4, others 0 (position from getMessagePositionInCategory).
 *
 * @param idx  the message index
 * @return the variant, or 0xFF when D_004A20C4 is clear
 */
unsigned char cOVState_ViewPopup::getSubjectVariant(int idx)
{
    if (D_004A20C4 == 0) {
        return 0xFF;
    }
    int position = getMessagePositionInCategory(idx);
    int div = 1;
    sPopupEntry18* messageEntry = &D_004C6C08[idx];
    sPopupEntry14* groupEntry = &D_00441630[messageEntry->groupIndex];
    switch (groupEntry->category) {
    case 3:
        div = 10;
        position = position / 2;
        break;
    case 5:
        div = 11;
        break;
    case 4:
        div = 10;
        break;
    case 8:
        div = 4;
        break;
    }
    return position % div;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onCreateScreen);
#ifdef SKIP_ASM

extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern void* cBENewRaceInterface_getLocationName1_5B80(void* iface) __asm__("cBENewRaceInterface_getLocationName1");
extern void strcpy3_5B80(unsigned short* dst, const char* src) __asm__("strcpy3");
extern void sprintf2_5B80(unsigned short* dst, const unsigned short* fmt, ...) __asm__("sprintf2");
extern "C" char* cBECharacterInterface_getShortNameByCharacter(void* iface, int a1, int a2);
extern "C" int cGameModeMan_getHeatScore(cGameModeMan* info, int idx, int which);
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_bGiveUp[];
extern int cGameModeMan_iCharRank[];
extern char* D_00441D10[];
extern unsigned short D_004A2140_5B80[] __asm__("D_004A2140");
extern char str_42freestyle_standings[];
extern char str_OV_darkblue[];
extern char str_track_event[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char str_title_b[];
extern char str_kT_OVRCMNQFHeat1Stand[];
extern char str_total[];
extern char str_kT_OVRCMNQFHeat2Stand[];
extern char str_heat1[];
extern char str_kT_OVRCMNScoreColon[];
extern char str_rider_pct_d[];
extern char str_run_1_pct_d[];
extern char str_DNF[];
extern char str_run_2_pct_d[];
extern char str_total_pct_d[];
extern char str_dash_sp_dash_sp_dash[];
extern char str_help_current0pct_d[];
extern char str_kT_CMNHELPEarnedEnoughPoints[];
extern char str_help_advance[];
extern char str_help_sorry[];
extern char str_overlayOptpct_d[];
extern char str_kT_OVRCMNFinalRound[];


struct sIdx5_5B80 {
    int v[5];
};
extern sIdx5_5B80 D_0046E488;


/**
 * Builds the freestyle heat standings overlay (overlay 10, vtable cOVState_HEAT_STANDINGS_vtbl, entry 4 onCreateScreen).
 *
 * Adds the "42freestyle_standings" screen (kept at +0x40) over "OV_darkblue", runs cOVState_onCreateScreen and fills
 * it: "track_event" reads "<location> - <Slope|Big Air|Pipe>" by the new race's cGameMode; the title is
 * kT_OVRCMNQFHeat1Stand (hiding "total") or kT_OVRCMNQFHeat2Stand by the game mode manager's heat; "heat1" reads
 * kT_OVRCMNScoreColon off the career path. Each standing row (cGameModeMan_iCharRank order) gets the rider's name (cBECharacterInterface_getShortNameByCharacter),
 * run 1 and run 2 scores and the total (cGameModeMan_getHeatScore) or DNF for a human rider who gave up
 * (cGameModeMan_bGiveUp); the player's help_current0N marker shows in heat 1 (kT_CMNHELPEarnedEnoughPoints in a final
 * qualifying round), help_advance or help_sorry in heat 2, and the five overlayOpt buttons take their option ids from
 * D_0046E488 (names D_00441D10, kT_OVRCMNFinalRound for a final round), stored at +0x18.
 */
void cOVState_HEAT_STANDINGS::onCreateScreen()
{
    unsigned short title[0x38];
    unsigned short line[104];
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_42freestyle_standings), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* iface2 = cBE_getInterface_Fv(cBE_getBE(), 2);
    signed char event = cBENewRaceInterface_newRace.cGameMode;
    cGameModeMan* info = SSXApp->GameModeMan;
    int kind = info->iHeatNum;
    cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_track_event));
    strcpy3_5B80(title, (char*)cBENewRaceInterface_getLocationName1_5B80(iface0));
    unsigned short* fmt = 0;
    switch (event) {
    case 1: {
        cFELocale* object = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, object);
        fmt = vt[4].fn(vthis(object, vt[4]), GetHashValue32(str_kT_cmnSlopeCaps));
        break;
    }
    case 2: {
        cFELocale* object = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, object);
        fmt = vt[4].fn(vthis(object, vt[4]), GetHashValue32(str_kT_cmnBigAirCaps));
        break;
    }
    case 3: {
        cFELocale* object = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, object);
        fmt = vt[4].fn(vthis(object, vt[4]), GetHashValue32(str_kT_cmnPipeCaps));
        break;
    }
    }
    sprintf2_5B80(line, D_004A2140_5B80, title, fmt);
    cUIText_setUnicodeString(uiText, line);
    uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title_b));
    switch (kind) {
    case 1:
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQFHeat1Stand));
        cUIState_hideObjSafe(this, str_total);
        break;
    case 2:
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQFHeat2Stand));
        break;
    }
    uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_heat1));
    if (cBENewRaceInterface_newRace.cGamePath != kGAMEPATH_WORLDCIRCUIT) {
        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNScoreColon));
    }
    int flag = 0;
    if (kind == 1 && info->iNextHeat == 3) {
        flag = 1;
    }
    int pl0 = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int pl1;
    int any = 0;
    pl1 = cBENewPlayerInterface_getHumanRider(iface1, 1);
    int last;
    {
        char name[32];
        char num[16];
        char num2[32];
        int i;
        for (i = 1; i < info->iNumberAI + 2; i++) {
            int idx = cGameModeMan_iCharRank[i - 1];
            sprintf(name, str_rider_pct_d, i);
            int hl = 0;
            uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
            if (i - 1 == cGameModeMan_iRanking[pl0]) {
                if (cGameModeMan_bGiveUp[pl0]) hl = 1; else hl = 0;
            } else if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER
                && i - 1 == cGameModeMan_iRanking[pl1]) {
                if (cGameModeMan_bGiveUp[pl1]) hl = 1; else hl = 0;
            }
            cUIText_setAsciiString(uiText, cBECharacterInterface_getShortNameByCharacter(iface2, info->iCharacters[idx], info->iCheatCharacters[idx]));
            sprintf(name, str_run_1_pct_d, i);
            uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
            if (hl && kind == 1) {
                cUIText_setAsciiString(uiText, str_DNF);
                any = 1;
            } else {
                sprintf(num, D_004A2138, cGameModeMan_getHeatScore(info, idx, 0));
                cUIText_setAsciiString(uiText, num);
            }
            sprintf(name, str_run_2_pct_d, i);
            uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
            sprintf(name, str_total_pct_d, i);
            cUIThing* totalText = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
            if (kind == 2 || flag) {
                if (hl) {
                    cUIText_setAsciiString(uiText, str_DNF);
                    any = 1;
                    totalText->setVisible(0);
                } else {
                    sprintf(num, "%d", cGameModeMan_getHeatScore(info, idx, 1));
                    cUIText_setAsciiString(uiText, num);
                    int total = cGameModeMan_getHeatScore(info, idx, 0) + cGameModeMan_getHeatScore(info, idx, 1);
                    sprintf(num2, "%d", total);
                    cUIText_setAsciiString((cUIText*)totalText, num2);
                    if (i - 1 == cGameModeMan_iRanking[pl0] && flag) {
                        cUIText_setAsciiString(uiText, str_dash_sp_dash_sp_dash);
                    }
                }
            } else {
                cUIText_setAsciiString(uiText, str_dash_sp_dash_sp_dash);
                totalText->setVisible(0);
            }
        }
        last = -1;
        int k;
        for (k = 1; k < 7; k++) {
            sprintf(name, str_help_current0pct_d, k);
            int playerPlace = cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(iface1, 0)];
            int placeMarker = playerPlace < 6 ? playerPlace : 5;
            if (kind == 1 && placeMarker + 1 == k && !any) {
                last = k;
                cUIState_showObjSafe(this, name);
            } else {
                cUIState_hideObjSafe(this, name);
            }
        }
        if (flag) {
            sprintf(name, str_help_current0pct_d, last);
            cFELocale* object = (cFELocale*)((cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name)));
            cUIText_setUnicodeStringByID(((cUIText*)object), GetHashValue32(str_kT_CMNHELPEarnedEnoughPoints));
        }
    }
    {
        sIdx5_5B80 ids = D_0046E488;
        char name[32];
        int i;
        cUIState_hideObjSafe(this, str_help_advance);
        cUIState_hideObjSafe(this, str_help_sorry);
        if (any) {
            ids.v[0] = 2;
        } else if (kind == 2) {
            if (cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(iface1, 0)] < 3) {
                cUIState_showObjSafe(this, str_help_advance);
            } else {
                cUIState_showObjSafe(this, str_help_sorry);
                ids.v[0] = kind;
            }
        }
        for (i = 0; i < 5; i++) {
            sprintf(name, str_overlayOptpct_d, i);
            cFELocale* object = (cFELocale*)((cUIThing*)cUIScreen_getObjectByHashName(mpScreen,
                    GetHashValue32(name)));
            if (((cUIThing*)object) != 0) {
                if (i < 5) {
                    cUIText_setUnicodeStringByID((cUIText*)object, GetHashValue32(D_00441D10[ids.v[i]]));
                    if (i == 0 && ids.v[0] == 1 && info->iNextHeat == 3) {
                        cUIText_setUnicodeStringByID((cUIText*)object, GetHashValue32(str_kT_OVRCMNFinalRound));
                    }
                    object->mpLocaleFile[4] = (cLocaleFile*)ids.v[i];
                } else {
                    ((cUIThing*)object)->setVisible(0);
                    ((cUIThing*)object)->setStatic(1);
                }
            }
        }
    }
}
#endif

extern "C" void* cOVState_onUpdate(void* self);

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the freestyle standings overlay: runs cOVState_onUpdate (vtable entry 12).
 *
 * @return cOVState_onUpdate's result
 */
void* cOVState_HEAT_STANDINGS_onUpdate(void* vself)
{
    cOVState_HEAT_STANDINGS* self = (cOVState_HEAT_STANDINGS*)vself;
    return cOVState_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onGainFocus);
#ifdef SKIP_ASM
// cUIState_onGainFocus returns nothing (ends in a void vcall); the unit's later void* declaration is a guess.
void cUIState_onGainFocus_v(void* self) __asm__("cUIState_onGainFocus");
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int cGameModeMan_iRanking[];
extern char str_rider_pct_d[];
extern char str_at_TextPulseWhiteOrange[];
extern char str_run_pct_d_pct_d[];

/**
 * Pulses the player's standings row when the freestyle standings overlay gains focus (vtable entry 6, onGainFocus).
 *
 * Runs cUIState_onGainFocus, fades the human rider's "rider_%d" row and the "run_%d_%d" entry (heat from the game mode
 * manager's first word) with "@TextPulseWhiteOrange" (cOVState_triggerTextFade; place capped at 6) and plays menu sound
 * 0xE.
 */
void cOVState_HEAT_STANDINGS::onGainFocus(cUIState*)
{
    char buf[32];
    cUIState_onGainFocus_v(this);
    int* heatData = (int*)SSXApp->GameModeMan;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    int x = *heatData;
    int n = cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(iface, 0)] + 1;
    if (n >= 7) {
        n = 6;
    }
    sprintf(buf, str_rider_pct_d, n);
    cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
    sprintf(buf, str_run_pct_d_pct_d, x, n);
    cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onInputBegin);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_overlayMenu[];
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cReplay_setReplayPadNum(void* self, int val);

/**
 * Notes the input's pad in the replay while the standings menu has its third button highlighted (vtable entry 8).
 *
 * When the screen's "overlayMenu" has row 2 selected (+0x95; Replay in the default option list) passes the input's byte
 * +0x4 to the replay (cReplay_setReplayPadNum).
 *
 * @param input  the input
 * @return 0
 */
bool cOVState_HEAT_STANDINGS::onInputBegin(cUIInput* input)
{
    cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_overlayMenu));
    if (obj != 0) {
        if (obj->mScrollItemCurrent == 2) {
            cReplay_setReplayPadNum(SSXApp->Game->Replay, input->mPadNum);
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the freestyle standings menu's events (vtable entry 20, onWidgetMenu): 257 for kONNEXT, else 0.
 *
 * @param widget  the menu
 * @param event   the menu event
 * @return 257 or 0
 */
int cOVState_HEAT_STANDINGS::onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_HEAT_STANDINGS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

/**
 * Passes the chosen option of the freestyle standings overlay to the overlay manager (vtable entry 19, onWidgetEvent).
 *
 * On kONNEXT sets cOVStateManager_mStatus to 1 and cOVStateManager_mMiscData to the widget's option id (mMiscData),
 * which cOVStateManager_gotoNextOverlay then acts on.
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState_HEAT_STANDINGS::onWidgetEvent(cUIThing* popup, unsigned int event)
{
    if (popup != 0 && event == cUIState::kONNEXT) {
        int popupId = popup->mMiscData;
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_43final_standings[];
extern char str_OV_darkblue[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onCreateScreen(void* self);

/**
 * Creates the final standings overlay's screens (overlay 11, vtable cOVState_FINAL_STANDINGS_vtbl, entry 4 onCreateScreen).
 *
 * Adds "43final_standings" (kept at +0x40) over "OV_darkblue", runs cOVState_onCreateScreen and sets +0x9C so the next
 * onGainTransition fills the screen.
 */
void cOVState_FINAL_STANDINGS::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_43final_standings), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    doInit = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onGainTransition);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).

// Declarations shared by the functions of this unit.
struct cUIText;

int GetHashValue32(char* str);

extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");


extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);


extern "C" int sprintf(char* buf, const char* fmt, ...);

extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);

extern "C" void cUIText_setUnicodeString(void* text, void* p);

void cUIText_setAsciiString(cUIText* text, const char* str);

extern char D_004A2138[];

extern int cGameModeMan_iRanking[];


extern char str_helptext_b[];

extern "C" int fptoui(float f);

extern "C" char* formatMinutesSeconds(int v);

extern unsigned int D_00536640[];

void cUIText_setUnicodeStringByID(cUIText* t, int id);

extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" unsigned short* strcpy3(void* dst, const char* s);
extern "C" void sprintf2(void* dst, const char* fmt, ...);
extern "C" const char* cBENewRaceInterface_getLocationName1(void* iface);
extern "C" char* cBENewPlayerInterface_getRiderName(void* iface, int idx);
extern "C" char* cBECharacterInterface_getShortNameByCharacter(void* iface, int id, int a2);
extern "C" void cUIText_setUnicodeStringWrap(void* text, unsigned short* str);
extern "C" void cUIIcon_setTextureByHashValue(void* obj, int hash);
extern int cGameModeMan_iCharRank[];
extern int D_005366A8[];
extern char* D_00441D10[];
extern int D_004A213C;
struct sIdx5_6718 {
    int v[5];
};
extern sIdx5_6718 D_0046E648;
extern char str_screenData[];
extern char str_track_event[];
extern char str_kT_cmnRaceCaps_b[];
extern char str_kT_cmnSlopeCaps[];
extern char str_kT_cmnBigAirCaps[];
extern char str_kT_cmnPipeCaps[];
extern char D_0046E558[];
extern char D_004A2140[];
extern char str_title_b[];
extern char str_kT_OVRCMNMultiPlayResults[];
extern char str_kT_OVRCMNQuickPlayResults[];
extern char str_kT_OVRQualifierResults[];
extern char str_kT_OVRSemiFinalResults[];
extern char str_kT_OVRFinalResults[];
extern char str_kT_TITLERivalChallenge[];
extern char str_finalScoreTime[];
extern char str_kT_OVRCMNTime_b[];
extern char str_kT_OVRCMNScore[];
extern char str_riderpct_d[];
extern char str_timePointspct_d[];
extern char str_DNF[];
extern char str_rankpct_d[];
extern char str_kT_HELPCongratWonChal[];
extern char str_kT_OVRHELPSorryLose[];
extern char str_kT_OVRCongratGoToSemi[];
extern char str_kT_OVRRaceObjectives7[];
extern char str_kT_OVRCMNSorryGetThirdSemi[];
extern char str_kT_OVRCMNSorryGetThird[];
extern char str_kT_OVRCongratulationsPlatinum[];
extern char str_kT_OVRCongratulationsGold[];
extern char str_kT_OVRCongratulationsSilver[];
extern char str_kT_OVRCongratulationsBronze[];
extern char str_kT_OVRCMNCongratP1[];
extern char str_gold[];
extern char str_silver[];
extern char str_bronze[];
extern char str_platinum[];
extern char str_overlayOptpct_d[];
extern char str_kT_OVRCMNFinalRound[];

// EA's 3-float vector (prototype name, as in fe/fepopup).
static inline unsigned short* Loc1E6718(char* name)
{
    cFELocale* obj = SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, obj);
    return vt[4].fn(vthis(obj, vt[4]), GetHashValue32(name));
}

/**
 * Fills the final standings overlay on its first transition (vtable entry 7, onGainTransition).
 *
 * Runs only while +0x9C is set, which it clears. Sets "track_event" to "<location> - <Race|Slope|Big Air|Pipe>" ("%s -
 * Jam" for game mode 6), the title by game path and mode (multiplayer, quick play, qualifier / semi-final / final
 * results, rival challenge, final results), the Time or Score column, then up to six rider rows (two for rival
 * challenges and two-player games, the AI's rider count for races) with names and times, scores or DNF; the help text
 * (won / lost challenge, go to the semi-final, sorry, platinum / gold / silver / bronze, or the two-player winner line
 * kT_OVRCMNCongratP1); the medal icons (platinum instead of gold when D_004A213C is set) and the option buttons from
 * D_0046E648, or only option 8 in a network game. Raises "screenData" (y = 200) in two-player and challenge games.
 *
 * @return 0 (the transition is not finished)
 */
bool cOVState_FINAL_STANDINGS::onGainTransition(int)
{
    if (doInit != 0) {
        doInit = 0;
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
        void* net = cBE_getInterface_Fv(cBE_getBE(), 2);
        cGameModeMan* raceSetup = SSXApp->GameModeMan;
        int mode = raceSetup->iHeatNum;
        int track = cBENewRaceInterface_newRace.cGameMode;
        int players = cBENewRaceInterface_newRace.cGamePath;
        if ((track != 0 && players == 2) || (track == 5 || track == 6)) {
            cUIThing* dataPanel = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_screenData));
            dataPanel->setVisible(1);
            t3Vector pos = dataPanel->mPosition;
            t3Vector raisedPos;
            raisedPos.x = pos.x;
            raisedPos.y = 200.0f;
            raisedPos.z = pos.z;
            dataPanel->mPosition = raisedPos;
        }
        cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_track_event));
        unsigned short name[0x38];
        strcpy3(name, cBENewRaceInterface_getLocationName1(iface0));
        unsigned short* str = 0;
        switch (cBENewRaceInterface_newRace.cGameMode) {
        case 0:
        case 5:
            str = Loc1E6718(str_kT_cmnRaceCaps_b);
            break;
        case 1:
            str = Loc1E6718(str_kT_cmnSlopeCaps);
            break;
        case 2:
            str = Loc1E6718(str_kT_cmnBigAirCaps);
            break;
        case 3:
            str = Loc1E6718(str_kT_cmnPipeCaps);
            break;
        }
        unsigned short title[104];
        // titleFmt ("%s - %s") is a variable: the target loads its %hi before the track test, which a literal
        // argument (loaded in the else arm only) does not.
        char* titleFmt = D_004A2140;
        if (track == 6) {
            sprintf2(title, D_0046E558, name);
        } else {
            sprintf2(title, titleFmt, name, str);
        }
        cUIText_setUnicodeString(uiText, title);
        uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title_b));
        if (players == 2) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNMultiPlayResults));
        } else if (players == 1) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNQuickPlayResults));
        } else if (track == 0) {
            switch (mode) {
            case 1:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRQualifierResults));
                break;
            case 2:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRSemiFinalResults));
                break;
            case 3:
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRFinalResults));
                break;
            }
        } else if (track == 5 || track == 6) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_TITLERivalChallenge));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRFinalResults));
        }
        uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_finalScoreTime));
        if (track == 0 || track == 5) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNTime_b));
        } else {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNScore));
        }
        int count = 6;
        if (track == 5 || track == 6) {
            count = 2;
        } else if (track == 0) {
            count = SSXApp->Game->AI->NumberOfRidersAllocated;
        } else if (players == 2) {
            count = 2;
        }
        int pl0 = cBENewPlayerInterface_getHumanRider(player, 0);
        int pl1;
        int done = 0;
        pl1 = cBENewPlayerInterface_getHumanRider(player, 1);
        char buf[48];
        char text[0x70];
        for (int i = 0; i < 6; i++) {
            int idx = cGameModeMan_iCharRank[i];
            if (i < count) {
                sprintf(buf, str_riderpct_d, i + 1);
                int finished = 0;
                uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
                if (i == cGameModeMan_iRanking[pl0]) {
                    if (D_005366A8[pl0] != 0) {
                        finished = 1;
                    }
                } else if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER
                    && i == cGameModeMan_iRanking[pl1]) {
                    if (D_005366A8[pl1] != 0) {
                        finished = 1;
                    }
                }
                if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER && i == cGameModeMan_iRanking[pl0]) {
                    cUIText_setAsciiString(uiText, cBENewPlayerInterface_getRiderName(player, pl0));
                } else if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER
                    && i == cGameModeMan_iRanking[pl1]) {
                    cUIText_setAsciiString(uiText, cBENewPlayerInterface_getRiderName(player, pl1));
                } else {
                    int rider = raceSetup->iCharacters[idx];
                    int outfit = raceSetup->iCheatCharacters[idx];
                    cUIText_setAsciiString(uiText, cBECharacterInterface_getShortNameByCharacter(net, rider, outfit));
                }
                sprintf(buf, str_timePointspct_d, i + 1);
                uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
                if (finished) {
                    cUIText_setAsciiString(uiText, str_DNF);
                    done = 1;
                } else if (track == 0 || track == 5) {
                    cUIText_setAsciiString(uiText, formatMinutesSeconds(fptoui(D_00536640[idx] * 0.01666666753590107f)));
                } else {
                    unsigned int time = D_00536640[idx];
                    sprintf(text, D_004A2138, time);
                    cUIText_setAsciiString(uiText, text);
                }
            } else {
                sprintf(buf, str_riderpct_d, i + 1);
                cUIState_hideObjSafe(this, buf);
                sprintf(buf, str_timePointspct_d, i + 1);
                cUIState_hideObjSafe(this, buf);
                sprintf(buf, str_rankpct_d, i + 1);
                cUIState_hideObjSafe(this, buf);
            }
        }
        sIdx5_6718 ids = D_0046E648;
        int same = 0;
        if (players == 2 && track != 0 && track != 5) {
            same = D_00536640[pl0] == D_00536640[pl1];
        }
        uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_b));
        if (players != 2) {
            if (track == 5 || track == 6) {
                if (cGameModeMan_iRanking[pl0] == 0) {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_HELPCongratWonChal));
                } else {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRHELPSorryLose));
                }
            } else if (track == 0 && mode < 3) {
                if (cGameModeMan_iRanking[pl0] < 3) {
                    ids.v[0] = 1;
                    if (mode == 1) {
                        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratGoToSemi));
                    } else {
                        cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRRaceObjectives7));
                    }
                } else if (mode == 1) {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNSorryGetThirdSemi));
                } else {
                    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCMNSorryGetThird));
                }
            } else if (D_004A213C != 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsPlatinum));
            } else if (cGameModeMan_iRanking[pl0] == 0) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsGold));
            } else if (cGameModeMan_iRanking[pl0] == 1) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsSilver));
            } else if (cGameModeMan_iRanking[pl0] == 2) {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRCongratulationsBronze));
            } else {
                cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_OVRHELPSorryLose));
            }
            if (players == 1) {
                ids.v[0] = 3;
            }
        } else {
            ids.v[0] = 3;
            if (SSXApp->Game->AI->NumberOfRidersAllocated != players || same) {
                ((cUIThing*)uiText)->setVisible(0);
            } else {
                if (cGameModeMan_iRanking[pl0] < cGameModeMan_iRanking[pl1]) {
                    strcpy3(text, cBENewPlayerInterface_getRiderName(player, pl0));
                } else {
                    strcpy3(text, cBENewPlayerInterface_getRiderName(player, pl1));
                }
                unsigned short msg[200];
                unsigned short* msgFmt = Loc1E6718(str_kT_OVRCMNCongratP1);
                sprintf1(msg, msgFmt, (unsigned short*)text);
                cUIText_setUnicodeStringWrap(uiText, msg);
            }
        }
        if (done && players != 2 && players != 1) {
            ids.v[0] = 2;
        }
        int show2 = 0;
        cUIState_hideObjSafe(this, str_gold);
        cUIState_hideObjSafe(this, str_silver);
        cUIState_hideObjSafe(this, str_bronze);
        int show1 = 1;
        if (cBENewRaceInterface_newRace.cGamePath != kGAMEPATH_MULTIPLAYER && (track == 5 || track == 6)
            && cGameModeMan_iRanking[pl0] != 0) {
            show1 = 0;
        }
        if (pl1 != -1 && cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
            show2 = 1;
            if ((track >= 1 && track <= 3) || track == 5 || track == 6) {
                if (cGameModeMan_iRanking[pl0] < cGameModeMan_iRanking[pl1]) {
                    show2 = 0;
                } else {
                    show1 = 0;
                }
            }
        }
        if (same) {
            show1 = 0;
            show2 = 0;
        }
        if (mode == 3 || track == 5 || track == 6) {
            if (show1) {
                if (cGameModeMan_iRanking[pl0] == 0) {
                    cUIState_showObjSafe(this, str_gold);
                } else if (cGameModeMan_iRanking[pl0] == 1) {
                    cUIState_showObjSafe(this, str_silver);
                } else if (cGameModeMan_iRanking[pl0] == 2) {
                    cUIState_showObjSafe(this, str_bronze);
                }
                if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_WORLDCIRCUIT && D_004A213C != 0) {
                    void* goldIcon = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_gold));
                    cUIIcon_setTextureByHashValue(goldIcon, GetHashValue32(str_platinum));
                }
            }
            if (show2) {
                if (cGameModeMan_iRanking[pl1] == 0) {
                    cUIState_showObjSafe(this, str_gold);
                } else if (cGameModeMan_iRanking[pl1] == 1) {
                    cUIState_showObjSafe(this, str_silver);
                } else if (cGameModeMan_iRanking[pl1] == 2) {
                    cUIState_showObjSafe(this, str_bronze);
                }
            }
        }
        int netIds[1] = { 8 };
        for (int i = 0; i < 5; i++) {
            sprintf(text, str_overlayOptpct_d, i);
            cUIThing* button = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(text));
            if (button != 0) {
                cBE_getInterface_Fv(SSXApp->BE, 7);
                if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
                    if (i < 1) {
                        cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(D_00441D10[netIds[i]]));
                        button->mMiscData = netIds[i];
                    } else {
                        button->setVisible(0);
                        button->setStatic(1);
                    }
                } else {
                    if (i < 5) {
                        cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(D_00441D10[ids.v[i]]));
                        if (i == 0 && ids.v[0] == 1 && raceSetup->iNextHeat == 3) {
                            cUIText_setUnicodeStringByID((cUIText*)button, GetHashValue32(str_kT_OVRCMNFinalRound));
                        }
                        button->mMiscData = ids.v[i];
                    } else {
                        button->setVisible(0);
                        button->setStatic(1);
                    }
                }
            }
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_overlayMenu[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

/**
 * Preselects the second button of the final standings menu on the quick-play path (vtable entry 18, onWidgetCreate).
 *
 * When the created widget is "overlayMenu" and the new race's cGamePath is kGAMEPATH_SINGLEEVENT, selects its
 * row 1.
 *
 * @param menu  the widget being created
 */
void cOVState_FINAL_STANDINGS::onWidgetCreate(cUIThing* menu)
{
    int hash = menu->mHashName;
    if (hash == GetHashValue32(str_overlayMenu)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_SINGLEEVENT) {
            cUIMenu_setSelectedByIndex(menu, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onGainFocus);
#ifdef SKIP_ASM
// cUIState_onGainFocus returns nothing (ends in a void vcall); the unit's later void* declaration is a guess.
void cUIState_onGainFocus_v(void* self) __asm__("cUIState_onGainFocus");
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cOVState_triggerTextFade(void* self, char* text, char* name);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern int cGameModeMan_iRanking[];
extern char str_riderpct_d[];
extern char str_at_TextPulseWhiteOrange[];
extern char str_timePointspct_d[];
extern char str_ChatGroup[];
extern char str_chatText[];
extern char str_kT_POPUP235Chat_b[];
extern char str_helptexts[];
extern char str_textScrollChat_b[];

extern tARGB D_004C8408;

/**
 * Highlights the players' rows and sets up online chat when the final standings overlay gains focus (vtable entry 6).
 *
 * Runs cUIState_onGainFocus, fades the human riders' "rider%d" and "timePoints%d" entries with "@TextPulseWhiteOrange"
 * (player 1, and player 2 online; neither online in freestyle or challenge modes). In a connected network game shows
 * "ChatGroup", sets "chatText" to kT_POPUP235Chat, shows "helptexts" and prepares "textScrollChat" (chat window mode,
 * selected colour D_004C8408, input map 1, hidden). Plays menu sound 0xE.
 */
void cOVState_FINAL_STANDINGS::onGainFocus(cUIState*)
{
    char buf[32];
    int second = 0;
    cUIState_onGainFocus_v(this);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGamePath;
    int track = cBENewRaceInterface_newRace.cGameMode;
    int first = 1;
    if (mode == 2) {
        second = 1;
        if ((track >= 1 && track <= 3) || track == 5 || track == 6) {
            first = 0;
            second = 0;
        }
    }
    if (first) {
        sprintf(buf, str_riderpct_d, cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(player, 0)] + 1);
        cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
        sprintf(buf, str_timePointspct_d, cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(player, 0)] + 1);
        cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
    }
    if (second) {
        sprintf(buf, str_riderpct_d, cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(player, 1)] + 1);
        cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
        sprintf(buf, str_timePointspct_d, cGameModeMan_iRanking[cBENewPlayerInterface_getHumanRider(player, 1)] + 1);
        cOVState_triggerTextFade(this, buf, str_at_TextPulseWhiteOrange);
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cGameComm_mGameComm != 0 && cGameComm_mGameComm->mConnected != 0) {
        cUIThing* groupWidget = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ChatGroup));
        if (groupWidget != 0) {
            groupWidget->setVisible(1);
        }
        cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_chatText));
        if (uiText != 0) {
            cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_POPUP235Chat_b));
        }
        groupWidget = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptexts));
        if (groupWidget != 0) {
            groupWidget->setVisible(1);
        }
        cUITextScroll* chatScroll =
            (cUITextScroll*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_textScrollChat_b));
        chatScroll->mAttrTextScroll.mMode = 1;
        chatScroll->mSelectedColour = D_004C8408;
        cUITextScroll_setInputMap(chatScroll, 1);
        chatScroll->setVisible(0);
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when the final standings overlay loses focus (vtable entry 11, onLoseFocus).
 */
void cOVState_FINAL_STANDINGS::onLoseFocus(cUIState*)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onUpdate);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;
extern "C" int cOVState_onUpdate_HandleIncomingChat(void* self);
extern char str_helptext_b[];
extern char str_helptexts[];
extern char str_ChatGroup[];
extern char str_chatText[];
extern char str_kT_POPUP235Chat_b[];

/**
 * Updates the final standings overlay and its online chat (vtable entry 12, onUpdate).
 *
 * Runs cOVState_onUpdate. With a game connection: when incoming chat arrives (cOVState_onUpdate_HandleIncomingChat)
 * hides "helptext" and "helptexts"; every 30 frames while disconnected hides "ChatGroup" and resets "chatText"; when
 * cGameComm's mOverlayTimeout reaches 0 sets it to -1 and chooses option 8 (cOVStateManager_mMiscData 8, mStatus 1).
 */
void cOVState_FINAL_STANDINGS::onUpdate()
{
    cOVState_onUpdate(this);
    if (cGameComm_mGameComm != 0) {
        if (cGameComm_mGameComm->mConnected != 0) {
            if (cOVState_onUpdate_HandleIncomingChat(this) != 0) {
                cUIThing* panelWidget = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_b));
                if (panelWidget != 0) {
                    panelWidget->setVisible(0);
                }
                panelWidget = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptexts));
                if (panelWidget != 0) {
                    panelWidget->setVisible(0);
                }
            }
        }
        if (cAppMan_TheAppMan->TimerTick % 30 == 0 && cGameComm_mGameComm->mConnected == 0) {
            cUIThing* panelWidget = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ChatGroup));
            if (panelWidget != 0) {
                panelWidget->setVisible(0);
            }
            cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_chatText));
            if (text != 0) {
                cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_POPUP235Chat_b));
            }
        }
        if (cGameComm_mGameComm->mOverlayTimeout == 0) {
            cGameComm_mGameComm->mOverlayTimeout = -1;
            cOVStateManager_mMiscData = 8;
            cOVStateManager_mStatus = 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onInputBegin);
#ifdef SKIP_ASM
extern "C" void cOVStateManager_triggerOverlay(int state);


/**
 * Handles input on the final standings overlay (vtable entry 8, onInputBegin).
 *
 * Passes the input's pad byte (+0x4) to the replay (cReplay_setReplayPadNum) while "overlayMenu" has row 2 selected; in a
 * connected network game triggers overlay 40 when the input's vtable entry 27 answers nonzero.
 *
 * @param input  the input
 * @return 0
 */
bool cOVState_FINAL_STANDINGS::onInputBegin(cUIInput* input)
{
    cUIMenu* obj = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_overlayMenu));
    if (obj != 0) {
        if (obj->mScrollItemCurrent == 2) {
            cReplay_setReplayPadNum(SSXApp->Game->Replay, input->mPadNum);
        }
    }
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0 && cGameComm_mGameComm != 0 && cGameComm_mGameComm->mConnected != 0) {
        if (input->isLShift1() != 0) {
            cOVStateManager_triggerOverlay(40);
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the final standings menu's events (vtable entry 20, onWidgetMenu): 257 for kONNEXT, else 0.
 *
 * @param widget  the menu
 * @param event   the menu event
 * @return 257 or 0
 */
int cOVState_FINAL_STANDINGS::onWidgetMenu(cUIMenu* widget, unsigned int event, unsigned int)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_FINAL_STANDINGS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

/**
 * Passes the chosen option of the final standings overlay to the overlay manager (vtable entry 19, onWidgetEvent).
 *
 * On kONNEXT sets cOVStateManager_mStatus to 1 and cOVStateManager_mMiscData to the widget's option id (mMiscData).
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState_FINAL_STANDINGS::onWidgetEvent(cUIThing* popup, unsigned int event)
{
    if (popup != 0 && event == cUIState::kONNEXT) {
        int popupId = popup->mMiscData;
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_RACE_FINISHOV_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int fptoui(float f);
extern "C" char* formatMinutesSeconds(int v);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern int D_004A213C;
extern int cGameModeMan_iRanking[];
extern unsigned int D_00536640[];
extern char str_finishov[];
extern char str_Menu0000_b[];
extern char str_firstrun[];
extern char str_secondrun[];
extern char str_thirdrun[];
extern char str_finalrun[];
extern char str_pointstotal[];
extern char str_new_record[];
extern char str_screendata[];
extern char str_platinum[];
extern char str_gold[];
extern char str_silver[];
extern char str_bronze[];
extern char str_placepct_d[];
extern char str_3D_Ov[];

class cUIObj_7BB0 {
public:
    int pad[2];
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

/**
 * Builds the race finish overlay (overlay 12, vtable cOVState_RACE_FINISHOV_vtbl, entry 4 onCreateScreen).
 *
 * Adds the "finishov" screen (kept at +0x40), runs cOVState_onCreateScreen, zeroes the timer (mElapsedTime), sets flag
 * 8 in "Menu0000"'s +0x90, hides the run labels, shows the human rider's time in "pointstotal" (frames / 60) and only
 * their "place%d". Shows the platinum, gold, silver or bronze icon by the reward interface's mLastEventMedal
 * (kMEDAL_PLATINUM .. kMEDAL_BRONZE) in a final (heat 3) or kGAMEEVENT_RIVALTIME, setting D_004A213C for platinum, and
 * "new_record" when the score interface has a new record (+0x18 or +0x1C not negative). Online, when the human rider's
 * word +0x480 is set, or for the time challenges (kGAMEEVENT_TIME_PEAK1..3) hides "screendata" and "3D_Ov"; otherwise
 * plays menu sound 0xE.
 */
void cOVState_RACE_FINISHOV::onCreateScreen()
{
    char buf[32];
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_finishov), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    mElapsedTime = 0.0f;
    char* obj = (char*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Menu0000_b));
    if (obj != 0) {
        *(int*)(obj + 0x90) |= 8;
    }
    int* modeData = (int*)SSXApp->GameModeMan;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBEScoreInterface* iface8 = (cBEScoreInterface*)cBE_getInterface_Fv(cBE_getBE(), 8);
    cBERewardInterface* ifaceD = (cBERewardInterface*)cBE_getInterface_Fv(cBE_getBE(), 13);
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int mode = *modeData;
    int sel = cGameModeMan_iRanking[pl];
    cUIState_hideObjSafe(this, str_firstrun);
    cUIState_hideObjSafe(this, str_secondrun);
    cUIState_hideObjSafe(this, str_thirdrun);
    cUIState_hideObjSafe(this, str_finalrun);
    cUIObj_7BB0* screenWidget = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_pointstotal));
    cUIText_setAsciiString((cUIText*)screenWidget, formatMinutesSeconds(fptoui(D_00536640[pl] * 0.01666666753590107f)));
    int off = pl * 4;
    int i;
    for (i = 1; i < 7; i++) {
        sprintf(buf, str_placepct_d, i);
        screenWidget = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        screenWidget->setVisible(0);
        if (i == sel + 1) {
            screenWidget->setVisible(1);
        }
    }
    if (ifaceD->mLastEventMedal == kMEDAL_PLATINUM
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME)) {
        D_004A213C = 1;
        cUIState_showObjSafe(this, str_platinum);
    } else {
        D_004A213C = 0;
        cUIState_hideObjSafe(this, str_platinum);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_GOLD
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME)) {
        cUIState_showObjSafe(this, str_gold);
    } else {
        cUIState_hideObjSafe(this, str_gold);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_SILVER
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME)) {
        cUIState_showObjSafe(this, str_silver);
    } else {
        cUIState_hideObjSafe(this, str_silver);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_BRONZE
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME)) {
        cUIState_showObjSafe(this, str_bronze);
    } else {
        cUIState_hideObjSafe(this, str_bronze);
    }
    screenWidget = (cUIObj_7BB0*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_new_record));
    int vis = 0;
    if (iface8->mNewTrackHighScoreIndex[0] >= 0 || iface8->mNewTrackHighScoreIndex[1] >= 0) {
        vis = 1;
    }
    if (vis) {
        screenWidget->setVisible(1);
    } else {
        screenWidget->setVisible(0);
    }
    char* humanRider = *(char**)((char*)SSXApp->Game->AI + off + 0x28);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER
        || *(int*)(humanRider + 0x480) != 0
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_TIME_PEAK1
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_TIME_PEAK2
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_TIME_PEAK3) {
        cUIState_hideObjSafe(this, str_screendata);
        cUIState_hideObjSafe(this, str_3D_Ov);
    } else {
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_RACE_FINISHOV_onUpdate);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cGame_doNextState(void* self);

/**
 * Moves the race finish overlay on after a delay (vtable entry 12, onUpdate).
 *
 * Runs cOVState_onUpdate and adds 1/60 to the timer (mElapsedTime); when mCanContinue is set and the timer reaches 3
 * seconds (1 second when the human rider's mDidNotFinish is set) purges the normal states and calls
 * cGame_doNextState.
 */
void cOVState_RACE_FINISHOV::onUpdate()
{
    float limit = 3.0f;
    int idx = cBENewPlayerInterface_getHumanRider(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    cRider* rider = SSXApp->Game->AI->Riders[idx];
    if (rider->mDidNotFinish != 0) {
        limit = 1.0f;
    }
    cOVState_onUpdate(this);
    mElapsedTime += 0.01666666753590107f;
    if (mCanContinue != 0 && mElapsedTime >= limit) {
        cUIStateStack_purgeNormalStates(&SSXApp->Game->UI->mStateStack);
        cGame_doNextState(SSXApp->Game);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_RACE_FINISHOV_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cGame_doNextState(void* self);

/**
 * Moves the race finish overlay on when its button is pressed while mCanContinue is set (vtable entry 19,
 * onWidgetEvent).
 *
 * On kONNEXT purges the normal UI states and calls cGame_doNextState.
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState_RACE_FINISHOV::onWidgetEvent(cUIThing* popup, unsigned int event)
{
    if (popup != 0) {
        if (event == cUIState::kONNEXT && mCanContinue != 0) {
            cGame* game = SSXApp->Game;
            cUIStateStack_purgeNormalStates(&game->UI->mStateStack);
            cGame_doNextState(game);
        }
    }
}
#endif

//88.13% - logic verified correct; GCC allocates v0 to the comparison
// constant and v1 to the result, the target does the reverse. No source
// shape tried (temp local, early return, nested ternary) flips it.
//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_RACE_FINISHOV_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the race finish menu's events (vtable entry 20): 257 for kONNEXT while mCanContinue is set, else 0.
 *
 * @param widget  the menu
 * @param key     the menu event
 * @return 257 or 0
 */
int cOVState_RACE_FINISHOV::onWidgetMenu(cUIMenu* widget, unsigned int key, unsigned int)
{
    int result = 0;
    if (key == cUIMenu::kONNEXT) {
        if (mCanContinue == 0) {
            result = 0;
        } else {
            return 257;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_RACE_FINISHOV_handleEvent);
#ifdef SKIP_ASM
/**
 * Sets the race finish overlay's may-continue flag (mCanContinue) from a message (vtable entry 24, handleEvent).
 *
 * Message 0 clears it and 1 sets it (the overlay factory starts it at 1); others are ignored.
 *
 * @param message  the message
 * @return 0
 */
int cOVState_RACE_FINISHOV::handleEvent(int message, void*)
{
    switch (message) {
    case 0:
        mCanContinue = 0;
        break;
    case 1:
        mCanContinue = 1;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen);
#ifdef SKIP_ASM
// Wide-string copy; returns dst.
extern "C" unsigned short* strcpy1(unsigned short* dst, const unsigned short* src);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern char str_kT_OVRCMNPointstotal[];
extern char str_place6[];

/**
 * Builds the event finish overlay for scored runs (overlay 13, vtable cOVState_BASSSP_QUAL_FINISHOV_vtbl, entry 4 onCreateScreen).
 *
 * Like the race finish overlay: "finishov" screen, timer reset, "Menu0000" flag 8. Shows firstrun / secondrun /
 * finalrun by the game mode manager's heat (not for kGAMEMODE_PNTSCHALLENGE), the medal icon by the reward interface's
 * mLastEventMedal in a final or kGAMEEVENT_RIVALTIME / kGAMEEVENT_RIVALPOINTS (platinum also sets D_004A213C), the
 * human rider's "place%d" (or "place6" past sixth), "pointstotal" as kT_OVRCMNPointstotal with the rider's points and
 * "new_record". Hides "screendata" and "3D_Ov" online, when the human rider's word +0x480 is set, for the points
 * challenges (kGAMEEVENT_POINTS_PEAK1..3) or in a final qualifying round; plays menu sound 0xE.
 */
void cOVState_BASSSP_QUAL_FINISHOV::onCreateScreen()
{
    char buf[32];
    unsigned short fmt[104];
    unsigned short text[104];
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_finishov), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    mElapsedTime = 0.0f;
    char* obj = (char*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Menu0000_b));
    if (obj != 0) {
        *(int*)(obj + 0x90) |= 8;
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBEScoreInterface* iface8 = (cBEScoreInterface*)cBE_getInterface_Fv(cBE_getBE(), 8);
    cBERewardInterface* ifaceD = (cBERewardInterface*)cBE_getInterface_Fv(cBE_getBE(), 13);
    int* modeData = (int*)SSXApp->GameModeMan;
    signed char event = cBENewRaceInterface_newRace.cGameMode;
    cUIState_hideObjSafe(this, str_firstrun);
    cUIState_hideObjSafe(this, str_secondrun);
    cUIState_hideObjSafe(this, str_thirdrun);
    cUIState_hideObjSafe(this, str_finalrun);
    int mode = *modeData;
    if (event != kGAMEMODE_PNTSCHALLENGE) {
        switch (mode) {
        case 0:
        case 1:
            cUIState_showObjSafe(this, str_firstrun);
            break;
        case 2:
            cUIState_showObjSafe(this, str_secondrun);
            break;
        case 3:
            cUIState_showObjSafe(this, str_finalrun);
            break;
        }
    }
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    int sel = cGameModeMan_iRanking[pl];
    if (ifaceD->mLastEventMedal == kMEDAL_PLATINUM
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALTIME
            || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALPOINTS)) {
        D_004A213C = 1;
        cUIState_showObjSafe(this, str_platinum);
    } else {
        D_004A213C = 0;
        cUIState_hideObjSafe(this, str_platinum);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_GOLD
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALPOINTS)) {
        cUIState_showObjSafe(this, str_gold);
    } else {
        cUIState_hideObjSafe(this, str_gold);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_SILVER
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALPOINTS)) {
        cUIState_showObjSafe(this, str_silver);
    } else {
        cUIState_hideObjSafe(this, str_silver);
    }
    if (ifaceD->mLastEventMedal == kMEDAL_BRONZE
        && (mode == 3 || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_RIVALPOINTS)) {
        cUIState_showObjSafe(this, str_bronze);
    } else {
        cUIState_hideObjSafe(this, str_bronze);
    }
    int off = pl * 4;
    int i;
    cUIThing* screenWidget;
    for (i = 1; i < 7; i++) {
        sprintf(buf, str_placepct_d, i);
        screenWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        screenWidget->setVisible(0);
        if (i == sel + 1) {
            screenWidget->setVisible(1);
        }
    }
    if ((unsigned int)sel >= 6) {
        cUIState_showObjSafe(this, str_place6);
    }
    screenWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_pointstotal));
    cFELocale* loc = SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, loc);
    strcpy1(fmt, vt[4].fn(vthis(loc, vt[4]), GetHashValue32(str_kT_OVRCMNPointstotal)));
    sprintf1(text, fmt, *(unsigned int*)((char*)D_00536640 + off));
    cUIText_setUnicodeString(screenWidget, text);
    screenWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_new_record));
    int vis = 0;
    if (iface8->mNewTrackHighScoreIndex[0] >= 0 || iface8->mNewTrackHighScoreIndex[1] >= 0) {
        vis = 1;
    }
    if (vis) {
        screenWidget->setVisible(1);
    } else {
        screenWidget->setVisible(0);
    }
    char* humanRider = *(char**)((char*)SSXApp->Game->AI + off + 0x28);
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER
        || *(int*)(humanRider + 0x480) != 0
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_POINTS_PEAK1
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_POINTS_PEAK2
        || cBENewRaceInterface_newRace.cGameEvent == kGAMEEVENT_POINTS_PEAK3
        || (mode == 1 && modeData[0x70 / 4] == 3)) {
        cUIState_hideObjSafe(this, str_screendata);
        cUIState_hideObjSafe(this, str_3D_Ov);
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_BASSSP_QUAL_FINISHOV_onUpdate);
#ifdef SKIP_ASM
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cGame_doNextState(void* self);

/**
 * Moves the event finish overlay on after a delay (vtable entry 12, onUpdate).
 *
 * Runs cOVState_onUpdate and adds 1/60 to the timer (mElapsedTime); when mCanContinue is set and the timer reaches 3
 * seconds (1 second when the human rider's mDidNotFinish is set) purges the normal states and calls
 * cGame_doNextState.
 */
void cOVState_BASSSP_QUAL_FINISHOV::onUpdate()
{
    float limit = 3.0f;
    int idx = cBENewPlayerInterface_getHumanRider(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    cRider* rider = SSXApp->Game->AI->Riders[idx];
    if (rider->mDidNotFinish != 0) {
        limit = 1.0f;
    }
    cOVState_onUpdate(this);
    mElapsedTime += 0.01666666753590107f;
    if (mCanContinue != 0 && mElapsedTime >= limit) {
        cUIStateStack_purgeNormalStates(&SSXApp->Game->UI->mStateStack);
        cGame_doNextState(SSXApp->Game);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_BASSSP_QUAL_FINISHOV_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_purgeNormalStates(void* list);
extern "C" void cGame_doNextState(void* self);

/**
 * Moves the event finish overlay on when its button is pressed while mCanContinue is set (vtable entry 19,
 * onWidgetEvent).
 *
 * On kONNEXT purges the normal UI states and calls cGame_doNextState.
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState_BASSSP_QUAL_FINISHOV::onWidgetEvent(cUIThing* popup, unsigned int event)
{
    if (popup != 0) {
        if (event == cUIState::kONNEXT && mCanContinue != 0) {
            cGame* game = SSXApp->Game;
            cUIStateStack_purgeNormalStates(&game->UI->mStateStack);
            cGame_doNextState(game);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_BASSSP_QUAL_FINISHOV_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the event finish menu's events (vtable entry 20): 257 for kONNEXT while mCanContinue is set, else 0.
 *
 * @param widget  the menu
 * @param key     the menu event
 * @return 257 or 0
 */
int cOVState_BASSSP_QUAL_FINISHOV::onWidgetMenu(cUIMenu* widget, unsigned int key, unsigned int)
{
    int result = 0;
    if (key == cUIMenu::kONNEXT) {
        if (mCanContinue == 0) {
            result = 0;
        } else {
            return 257;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_BASSSP_QUAL_FINISHOV_handleEvent);
#ifdef SKIP_ASM
/**
 * Sets the event finish overlay's may-continue flag (mCanContinue) from a message (vtable entry 24, handleEvent).
 *
 * Message 0 clears it and 1 sets it (the overlay factory starts it at 1); others are ignored.
 *
 * @param message  the message
 * @return 0
 */
int cOVState_BASSSP_QUAL_FINISHOV::handleEvent(int message, void*)
{
    switch (message) {
    case 0:
        mCanContinue = 0;
        break;
    case 1:
        mCanContinue = 1;
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int a1);
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" int fptoui(float f);
extern "C" char* formatMinutesSeconds(int v);
extern "C" void* getPeakChallengeEventName(int v);
extern "C" void cUIText_setUnicodeString(void* text, void* p);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_005366A8[];
extern int cGameModeMan_iRanking[];
extern int cGameModeMan_bGiveUp[];
extern unsigned int D_00536640[];
extern char* D_00441D10[];
struct sIdx3_8920 {
    int v[3];
};
extern sIdx3_8920 D_0046E9B0;
extern char str_70peakchal_results[];
extern char str_OV_darkblue[];
extern char str_title1[];
extern char str_title2[];
extern char str_kT_TITLERivalChallenge[];
extern char str_label_target_timescore[];
extern char str_target_timescore[];
extern char str_label_timescore[];
extern char str_timescore[];
extern char str_kT_OVRCMNTimeToBeatColon[];
extern char str_kT_OVRCMNYourTimeColon[];
extern char str_kT_OVRCMNScoreToBeatColon[];
extern char str_kT_CMNYourScore[];
extern char D_004A2138[];
extern char str_DNF[];
extern char str_helptext_b[];
extern char str_kT_HELPCongratWonChal[];
extern char str_kT_HELPSorryBeatTime[];
extern char str_kT_HELPSorryBeatScore[];
extern char str_overlayOptpct_d[];

/**
 * Builds the peak challenge results overlay (overlay 22, vtable D_00473ED8, entry 4 onCreateScreen).
 *
 * Adds "70peakchal_results" (kept at +0x40) over "OV_darkblue" and runs cOVState_onCreateScreen. "title1" reads
 * kT_TITLERivalChallenge and "title2" getPeakChallengeEventName's name for game events 6-11. For events 6-8 the labels read
 * kT_OVRCMNTimeToBeatColon / kT_OVRCMNYourTimeColon with the target time (mode data +0x78) and the rider's time or DNF;
 * otherwise kT_OVRCMNScoreToBeatColon / kT_CMNYourScore with the target score (+0x7C). The help text is
 * kT_HELPCongratWonChal for places 0-3, else kT_HELPSorryBeatTime or kT_HELPSorryBeatScore, hidden when the player gave
 * up; the first three overlayOpt buttons take option ids from D_0046E9B0 and the rest are hidden.
 */
void cOVState::PEAKCHAL_RESULTS_onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_70peakchal_results), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(this);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface1 = cBE_getInterface_Fv(cBE_getBE(), 1);
    cGameModeMan* modeData = SSXApp->GameModeMan;
    int trk = cBENewRaceInterface_newRace.cGameEvent;
    int pl = cBENewPlayerInterface_getHumanRider(iface1, 0);
    cUIText* uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title1));
    cUIText_setUnicodeStringByID(uiText, GetHashValue32(str_kT_TITLERivalChallenge));
    uiText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_title2));
    if (trk < 12) {
        if (trk >= 6) {
            cUIText_setUnicodeString(uiText, getPeakChallengeEventName(trk));
        }
    }
    cUIText* targetLabel = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_label_target_timescore));
    cUIText* targetValue = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_target_timescore));
    cUIText* resultLabel = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_label_timescore));
    cUIText* resultValue = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_timescore));
    if (trk >= 6 && trk <= 8) {
        cUIText_setUnicodeStringByID(targetLabel, GetHashValue32(str_kT_OVRCMNTimeToBeatColon));
        cUIText_setUnicodeStringByID(resultLabel, GetHashValue32(str_kT_OVRCMNYourTimeColon));
        cUIText_setAsciiString(targetValue, formatMinutesSeconds(fptoui(modeData->uiTimeLimit * 0.01666666753590107f)));
        if (D_005366A8[0] != 0) {
            cUIText_setAsciiString(resultValue, str_DNF);
        } else {
            cUIText_setAsciiString(resultValue, formatMinutesSeconds(fptoui(D_00536640[0] * 0.01666666753590107f)));
        }
    } else {
        char buf[32];
        cUIText_setUnicodeStringByID(targetLabel, GetHashValue32(str_kT_OVRCMNScoreToBeatColon));
        cUIText_setUnicodeStringByID(resultLabel, GetHashValue32(str_kT_CMNYourScore));
        sprintf(buf, D_004A2138, (int)modeData->uiPointTarget);
        cUIText_setAsciiString(targetValue, buf);
        if (D_005366A8[0] != 0) {
            cUIText_setAsciiString(resultValue, str_DNF);
        } else {
            sprintf(buf, D_004A2138, D_00536640[0]);
            cUIText_setAsciiString(resultValue, buf);
        }
    }
    cUIThing* screenWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_b));
    int playerPlace = cGameModeMan_iRanking[pl];
    if (playerPlace == 0) goto yes;
    if (playerPlace == 1) goto yes;
    if (playerPlace == 2) goto yes;
    if (playerPlace != 3) goto no;
yes:
    cUIText_setUnicodeStringByID((cUIText*)screenWidget, GetHashValue32(str_kT_HELPCongratWonChal));
    goto done;
no:
    if (trk >= 6 && trk <= 8) {
        cUIText_setUnicodeStringByID((cUIText*)screenWidget, GetHashValue32(str_kT_HELPSorryBeatTime));
    } else {
        cUIText_setUnicodeStringByID((cUIText*)screenWidget, GetHashValue32(str_kT_HELPSorryBeatScore));
    }
done:
    if (cGameModeMan_bGiveUp[pl] != 0) {
        screenWidget->setVisible(0);
    }
    {
        sIdx3_8920 ids = D_0046E9B0;
        char name[32];
        int i;
        for (i = 0; i < 5; i++) {
            sprintf(name, str_overlayOptpct_d, i);
            screenWidget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
            if (screenWidget != 0) {
                if (i < 3) {
                    cUIText_setUnicodeStringByID((cUIText*)screenWidget, GetHashValue32(D_00441D10[ids.v[i]]));
                    screenWidget->mMiscData = ids.v[i];
                } else {
                    screenWidget->setVisible(0);
                    screenWidget->setStatic(1);
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Updates the peak challenge results overlay: runs cOVState_onUpdate (vtable entry 12).
 *
 * @return cOVState_onUpdate's result
 */
void* cOVState::PEAKCHAL_RESULTS_onUpdate()
{
    return cOVState_onUpdate(this);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Ignores input on the peak challenge results overlay (vtable entry 8, onInputBegin).
 *
 * @return 0
 */
int cOVState::PEAKCHAL_RESULTS_onInputBegin()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the peak challenge results menu's events (vtable entry 20, onWidgetMenu): 257 for kONNEXT, else 0.
 *
 * @param widget  the menu
 * @param event   the menu event
 * @return 257 or 0
 */
int cOVState::PEAKCHAL_RESULTS_onWidgetMenu(int widget, int event)
{
    return event == cUIMenu::kONNEXT ? 257 : 0;
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

/**
 * Passes the chosen option of the peak challenge results overlay to the overlay manager (vtable entry 19).
 *
 * On kONNEXT sets cOVStateManager_mStatus to 1 and cOVStateManager_mMiscData to the widget's option id (mMiscData).
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState::PEAKCHAL_RESULTS_onWidgetEvent(cUIThing* popup, int event)
{
    if (popup != 0 && event == cUIState::kONNEXT) {
        int popupId = popup->mMiscData;
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_PEAKCHAL_RESULTS_onGainFocus);
#ifdef SKIP_ASM
extern "C" void* cUIState_onGainFocus(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);

/**
 * Plays menu sound 0xE when the peak challenge results overlay gains focus, after cUIState_onGainFocus (vtable entry
 * 6).
 */
void cOVState::PEAKCHAL_RESULTS_onGainFocus()
{
    cUIState_onGainFocus(this);
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_TOURNAMENT_STANDINGS_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_109MultiplayerStandings[];
extern char str_OV_darkblue[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cOVState_onCreateScreen(void* self);
void func_001E8FC0(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);

/**
 * Creates the multiplayer standings overlay's screens (overlay 35, vtable cOVState_TOURNAMENT_STANDINGS_vtbl, entry 4 onCreateScreen).
 *
 * Adds "109MultiplayerStandings" (kept at +0x40) over "OV_darkblue", calls the empty func_001E8FC0, runs
 * cOVState_onCreateScreen and plays menu sound 0xE.
 */
void cOVState_TOURNAMENT_STANDINGS::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_109MultiplayerStandings), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    engine = (void*)mpEngine;
    screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_OV_darkblue), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    func_001E8FC0(this);
    cOVState_onCreateScreen(this);
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", func_001E8FC0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the multiplayer standings overlay's fill step, called from its onCreateScreen).
 */
void func_001E8FC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fepopupmisc", cOVState_TOURNAMENT_STANDINGS_onWidgetEvent);
#ifdef SKIP_ASM
extern int cOVStateManager_mStatus;
extern int cOVStateManager_mMiscData;

/**
 * Passes the chosen option of the multiplayer standings overlay to the overlay manager (vtable entry 19).
 *
 * On kONNEXT sets cOVStateManager_mStatus to 1 and cOVStateManager_mMiscData to the widget's option id (mMiscData).
 *
 * @param popup  the widget
 * @param event  the widget event
 */
void cOVState_TOURNAMENT_STANDINGS::onWidgetEvent(cUIThing* popup, unsigned int event)
{
    if (popup != 0 && event == cUIState::kONNEXT) {
        int popupId = popup->mMiscData;
        cOVStateManager_mStatus = 1;
        cOVStateManager_mMiscData = popupId;
    }
}
#endif

