#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "ui/uitext.h"
#include "be/beintecon.h"
#include "be/bedbprofile.h"
#include "sound/ssxaudio.h"
#include "fe/uistatebuypopup.h"
#include "ui/uimenu.h"
#include "ui/uiinput.h"
#include "ui/uistate.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festatebuyattrib.h"
#include "fe/festatecareerstats.h"
#include "ui/uiscreen.h"

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_33buyattribs[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int cBEEconInterface_getMoney(void* iface, int a1, int charID);
extern "C" void cFEStateBuyAttrib_updateTotalCost(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateLevels(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateCostPerLevel(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateBank(cFEStateBuyAttrib* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_SetFrontEndMusicMode(void* self, int a1);

/**
 * Opens the buy-attributes screen ("33buyattribs") and fills in its cost, level, bar and bank displays.
 *
 * Keeps the screen at 0x48 (On Tour mpScreen) and plays frame 0, sets the bank (0xA4, mBank) to the money of
 * user 0's character from the economy interface (11), clears the total cost (0xA0) and the prompt flag
 * (0xA8), runs the five update functions and switches the audio's front-end sound-bank mode to 3
 * (cSSXAudio_SetFrontEndMusicMode on gSSXAudio).
 */
void cFEStateBuyAttrib::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_33buyattribs), 0);
    mpAttribScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 11);
    mBank = cBEEconInterface_getMoney(iface, 0, cBELibrary_getCharacterID(0));
    mBuyPrompt = 0;
    mTotalCost = 0;
    updateTotalCost();
    updateLevels();
    updateCostPerLevel();
    updateExperienceDisplay();
    updateBank();
    cSSXAudio_SetFrontEndMusicMode(cAudioInterface_bxAudio(), 3);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_BuyAttributesPrompt[];
extern char str_kT_OVRCMNBuyAttributesColon_b[];
extern char str_AttribMenu[];
extern char str_Buy_yes[];
extern char str_Buy_no[];

static inline int IsHash_4910(int id, char* s) { return id == GetHashValue32(s); }

/**
 * Sets up the buy-attributes screen's widgets as they are created, matched by their hashed names.
 *
 * "BuyAttributesPrompt" gets the text kT_OVRCMNBuyAttributesColon, "AttribMenu" is kept at 0xAC (On Tour
 * pAttribMenu), and "Buy_yes" and "Buy_no" get misc data 0 and 1.
 *
 * @param widget  the widget just created (its hash name is at +0x38)
 */
void cFEStateBuyAttrib::onWidgetCreate(cUIThing* widget)
{
    if (IsHash_4910(widget->mHashName, str_BuyAttributesPrompt)) {
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(str_kT_OVRCMNBuyAttributesColon_b));
    } else if (IsHash_4910(widget->mHashName, str_AttribMenu)) {
        pAttribMenu = (cUIMenu*)widget;
    } else if (IsHash_4910(widget->mHashName, str_Buy_yes)) {
        widget->mMiscData = 0;
    } else if (IsHash_4910(widget->mHashName, str_Buy_no)) {
        widget->mMiscData = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(cFEStateBuyAttrib* self);

/**
 * Refreshes the experience display when the buy-attribute state's gain transition starts.
 *
 * Calls cFEStateBuyAttrib_updateExperienceDisplay() only when @p on is nonzero, and returns 0 like the base
 * cUIState_onGainTransition().
 *
 * @param on  the state's first-time flag as passed by the state stack; nonzero on the transition's first call
 * @return always 0
 */
bool cFEStateBuyAttrib::onGainTransition(int on)
{
    if (on != 0) {
        updateExperienceDisplay();
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onGainFocus);
#ifdef SKIP_ASM

extern "C" void cFEState_onGainFocus(void* self);

/**
 * Gives the buy-attribute state focus, then focuses its "AttribMenu" widget.
 *
 * Runs the base cFEState_onGainFocus() and calls setFocus(1) (vtable slot 7) on the widget that
 * cFEStateBuyAttrib_onWidgetCreate() stored at 0xAC, without a null check.
 */
void cFEStateBuyAttrib::onGainFocus(cUIState*)
{
    cFEState_onGainFocus(this);
    cUIMenu* obj = pAttribMenu;
    obj->setFocus(1);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the attribute menu's events: 0x100 for the option and misc buttons (8 and 9), 0x101 for the rest.
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
int cFEStateBuyAttrib::onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
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
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void cFEStateBuyAttrib_buyExperience(cFEStateBuyAttrib* self);

/**
 * Buys the chosen attribute points when the buy popup closes with a yes.
 *
 * On kONPOPUPDESTROY (a popup was popped) with the popup's result word (+0x6C) set, calls
 * cFEStateBuyAttrib_buyExperience() (On Tour buyExperience), which pays the total cost and records the points with the
 * economy interface.
 *
 * @param sender  the popup that closed (the cUIStateBuyPopup)
 * @param msg  the popup event (0x15 opened, 0x16 closed)
 */
void cFEStateBuyAttrib::onPopupEvent(cUIState* sender, unsigned int msg)
{
    if (msg == cUIState::kONPOPUPDESTROY) {
        if (((cUIStateBuyPopup*)sender)->mbAccept != 0) {
            buyExperience();
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetEvent);
#ifdef SKIP_ASM
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIStateBuyPopup_cUIStateBuyPopup(void* mem, void* owner, void* state, int a3);
extern "C" void cUIStateBuyPopup_initBuyAttributes(void* self, int a1, int a2, int a3);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_UITRICKBUY_b[];
extern char str_PopupGroup[];
extern char str_TransitionOut_b[];
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern cUISoundInterface gUISoundNull;

/**
 * Handles accept and back on the buy-attributes screen: opens the buy popup, closes the prompt, or leaves.
 *
 * kONNEXT (accept): when the bank (0xA4) covers a positive total cost (0xA0), pushes a 0x70-byte "UITRICKBUY"
 * cUIStateBuyPopup (cUIStateBuyPopup_cUIStateBuyPopup, given the cost and bank by cUIStateBuyPopup_initBuyAttributes) as a popup; otherwise plays UI
 * sound 4 through the engine's sound interface (+0x14).
 *
 * kONPREV (back): with the prompt flag (0xA8) set, clears it, hides "PopupGroup" and refocuses the attribute
 * menu; otherwise asks the engine's custom interface (getPrevState, vtable slot 5) for a state with the
 * widget's misc data and, when it gives one, plays "TransitionOut" and pushes it.
 *
 * @param obj  the widget that sent the event; nothing happens when null
 * @param msg  the widget event (5 accept, 6 back)
 */
void cFEStateBuyAttrib::onWidgetEvent(cUIThing* obj, unsigned int msg)
{
    if (obj == 0)
        return;
    switch (msg) {
    case cUIState::kONNEXT:
        if (mBank >= mTotalCost && mTotalCost > 0) {
            void* pPopup = cUIStateBuyPopup_cUIStateBuyPopup(cMemMan_alloc(0x70, str_UITRICKBUY_b, 0x100, 0), mpEngine, ((char*)this), mUserNum);
            cUIStateBuyPopup_initBuyAttributes(pPopup, 0, mTotalCost, mBank);
            cUIStateStack_pushPopup(&mpEngine->mStateStack, pPopup);
        } else {
            cUISoundInterface* sound = mpEngine->mpSound;
            if (sound == 0)
                sound = &gUISoundNull;
            if (sound != 0)
                sound->playSound(4);
        }
        break;
    case cUIState::kONPREV:
        if (mBuyPrompt != 0) {
            mBuyPrompt = 0;
            cUIState_hideObjSafe(((char*)this), str_PopupGroup);
            cUIMenu* attribMenu = pAttribMenu;
            attribMenu->setFocus(1);
        } else {
            cUICustomInterface* mgr = mpEngine->mpCustom;
            void* nextState = mgr->getPrevState((cUIState*)this, obj->mMiscData);
            if (nextState != 0) {
                int frame = cUIScreen_getFrameByLabel(mpAttribScreen, GetHashValue32(str_TransitionOut_b));
                if (frame != cUIScreen::kNoFrame)
                    cUIScreen_playFrame(mpAttribScreen, frame, 1);
                cUIStateStack_push(&mpEngine->mStateStack, nextState);
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onInputAlways);
#ifdef SKIP_ASM
// PORT: cUIState_onInputAlways__FPv is called with (self, widget) here; bind the 2-arg form to that symbol.
void cUIState_onInputAlways_2(void* self, void* w) __asm__("cUIState_onInputAlways__FPv");
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
int cBEEconInterface_getAttribCost(void* iface, int level);
extern "C" void cFEStateBuyAttrib_updateTotalCost(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(cFEStateBuyAttrib* self);
extern char str_AttribMenu[];
extern char str_hl_right_b[];
extern char str_hl_left_b[];



static inline void defaultMsg_4C30(cFEStateBuyAttrib* s, int msg)
{
    cUISoundInterface* p = s->mpEngine->mpSound;
    if (p == 0)
        p = &gUISoundNull;
    if (p != 0)
        p->playSound(msg);
}

static inline void playFrame_4C30(cFEStateBuyAttrib* s, char* name)
{
    unsigned short frame = cUIScreen_getFrameByLabel((cUIScreen*)((void*)s->mpAttribScreen), GetHashValue32(name));
    if (frame != cUIScreen::kNoFrame)
        cUIScreen_playFrame(((void*)s->mpAttribScreen), frame, 1);
    defaultMsg_4C30(s, 1);
}

/**
 * Adds or takes back a point of the selected attribute with right and left and updates the total cost.
 *
 * Works on the row selected in "AttribMenu". Left (input slot 19) takes back one of the points bought this
 * visit (0x84); right (slot 20) adds one while the level (0x68) is below 11, the bank covers the new total
 * and the points stay below (level + 1) * 5. A point costs cBEEconInterface_getAttribCost(level - 1). A change plays "hl left"
 * or "hl right" with UI sound 1; nothing to take back, level 11 or too little money plays sound 4, while a
 * point at the level boundary is refused silently. Either key refreshes the bars. Nothing happens while the
 * prompt flag (0xA8) is set.
 *
 * @param w  the controller input (cUIInput)
 */
void cFEStateBuyAttrib::onInputAlways(cUIInput* w)
{
    int changed = 0;
    cUIState_onInputAlways_2((this), ((char*)w));
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 11);
    unsigned char idx = ((cUIMenu*)cUIScreen_getObjectByHashName(mpAttribScreen,
            GetHashValue32(str_AttribMenu)))->mScrollItemCurrent;
    if (mBuyPrompt == 0) {
        if (w->isLeft() != 0) {
            int* bought = mPotentialExperience;
            int* cnt = &bought[idx];
            changed = 1;
            if (*cnt > 0) {
                (*cnt)--;
                mTotalCost -= cBEEconInterface_getAttribCost(iface, mExperienceLevel[idx] - 1);
                updateTotalCost();
                playFrame_4C30((this), str_hl_left_b);
            } else {
                defaultMsg_4C30((this), 4);
            }
        } else {
            if (w->isRight() != 0) {
                int* level = mExperienceLevel;
                int* lvl = &level[idx];
                changed = 1;
                int total = cBEEconInterface_getAttribCost(iface, *lvl - 1) + mTotalCost;
                if (*lvl < 11 && total <= mBank) {
                    int* bought = mPotentialExperience;
                    int* cnt = &bought[idx];
                    if (mExperience[idx] + *cnt < (*lvl + 1) * 5) {
                        (*cnt)++;
                        mTotalCost += cBEEconInterface_getAttribCost(iface, *lvl - 1);
                        updateTotalCost();
                        playFrame_4C30((this), str_hl_right_b);
                    }
                } else {
                    defaultMsg_4C30((this), 4);
                }
            }
        }
    }
    if (changed) {
        updateExperienceDisplay();
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateLevels);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_lvlpct_d[];
extern char str_pct_d_pct_d_c[];


/**
 * Shows each of the seven attributes' level as "level.fraction" in its "lvl%d" text.
 *
 * The level is the attribute's points (0x4C) / 5 and raises the stored level (0x68) when higher; the text is
 * "%d.%d" of the level and the leftover points times 2 (each point is 0.2 of a level).
 *
 */
void cFEStateBuyAttrib::updateLevels()
{
    cFEStateBuyAttrib* self = this;
    char name[0x20];
    char text[0x10];
    int i;
    for (i = 0; i < 7; i++) {
        int lvl = self->mExperience[i] / 5;
        if (self->mExperienceLevel[i] < lvl)
            self->mExperienceLevel[i] = lvl;
        int rem = self->mExperience[i] - lvl * 5;
        sprintf(name, str_lvlpct_d, i);
        cUIText* levelText = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpAttribScreen), GetHashValue32(name));
        if (levelText != 0) {
            sprintf(text, str_pct_d_pct_d_c, lvl, rem * 2);
            cUIText_setAsciiString(levelText, text);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateCostPerLevel);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* getDollarString(int value);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEEconInterface_getAttribCost(void* iface, int level);
extern char str_costpct_d[];
struct sVE4FC0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows the price of one more point for each of the seven attributes in its "cost%d" text.
 *
 * The price is cBEEconInterface_getAttribCost(level - 1) for the attribute's level (0x68), formatted by getDollarString(); at
 * level 11 or more the text is hidden instead (setVisible(0), vtable slot 9).
 */
void cFEStateBuyAttrib::updateCostPerLevel()
{
    char buf[32];
    int i;
    int* levels = (int*)&mExperienceLevel;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 11);
    for (i = 0; i < 7; i++, levels++) {
        sprintf(buf, str_costpct_d, i);
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpAttribScreen, GetHashValue32(buf));
        if (text != 0) {
            cUIText_setAsciiString(text, getDollarString(cBEEconInterface_getAttribCost(iface, *levels - 1)));
            if (*levels >= 11) {
                text->setVisible(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateExperienceDisplay);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_Vectorpct_d[];
extern char str_underpct_d[];

/**
 * Sizes the seven attributes' bars: "Vector%d" for the points owned and "under%d" for owned plus bought.
 *
 * Each bar is set (setSize, vtable slot 21) to 37/11 pixels per point and 12 high, so 55 points fill 185
 * pixels, then shown (setVisible(1)).
 *
 */
void cFEStateBuyAttrib::updateExperienceDisplay()
{
    cFEStateBuyAttrib* self = this;
    char name[0x20];
    int i;
    for (i = 0; i < 7; i++) {
        sprintf(name, str_Vectorpct_d, i);
        cUIThing* bar = (cUIThing*)((char*)cUIScreen_getObjectByHashName(((void*)self->mpAttribScreen), GetHashValue32(name)));
        if (((char*)bar) != 0) {
            t3Vector barSize;
            barSize.y = 12.0f;
            barSize.x = (float)self->mExperience[i] * 3.3636362552642822f;
            barSize.z = 0.0f;
            bar->setSize(&barSize);
            bar->setVisible(1);
        }
        sprintf(name, str_underpct_d, i);
        bar = (cUIThing*)((char*)cUIScreen_getObjectByHashName(((void*)self->mpAttribScreen), GetHashValue32(name)));
        if (((char*)bar) != 0) {
            t3Vector barSize;
            barSize.x = (float)(self->mPotentialExperience[i] + self->mExperience[i]) * 3.3636362552642822f;
            barSize.y = 12.0f;
            barSize.z = 0.0f;
            bar->setSize(&barSize);
            bar->setVisible(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateTotalCost);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* getDollarString(int value);
extern char str_AttributeCost[];

/**
 * Shows the total cost of the chosen points (0xA0) as dollars in the "AttributeCost" text, if it exists.
 */
void cFEStateBuyAttrib::updateTotalCost()
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpAttribScreen, GetHashValue32(str_AttributeCost));
    if (text != 0) {
        cUIText_setAsciiString(text, getDollarString(mTotalCost));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateBank);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* getDollarString(int value);
extern char str_CurrentMoney[];

/**
 * Shows the bank (0xA4) as dollars in the "CurrentMoney" text, if it exists.
 */
void cFEStateBuyAttrib::updateBank()
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpAttribScreen, GetHashValue32(str_CurrentMoney));
    if (text != 0) {
        cUIText_setAsciiString(text, getDollarString(mBank));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_buyExperience);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" void cFEStateBuyAttrib_updateTotalCost(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateLevels(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateCostPerLevel(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(cFEStateBuyAttrib* self);
extern "C" void cFEStateBuyAttrib_updateBank(cFEStateBuyAttrib* self);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" int cBEEconInterface_buyAttribute(void* iface, int a1, int charID, int attr);
extern char str_PopupGroup[];
extern const int cFEStateBuyAttrib_attributeOrder[];





/**
 * Buys the attribute points the player has added: pays for them, records them with the economy and resets the counters.
 *
 * Takes the total cost (+0xA0) from the bank (+0xA4) and zeroes it (cFEStateBuyAttrib_updateTotalCost), clears the
 * prompt flag (+0xA8), hides "PopupGroup" and focuses the attribute menu (+0xAC, vtable entry 7 with 1). For each of
 * the seven attributes it calls cBEEconInterface_buyAttribute(BE interface 11, 0, character, cFEStateBuyAttrib_attributeOrder[i]) once per point bought
 * (+0x84), then calls that interface's vtable entry 1; adds the bought points to the experience (+0x4C) and zeroes
 * them, and refreshes the levels, cost per level, experience display and bank. cFEStateBuyAttrib_onPopupEvent calls it
 * when the confirm popup closes with a yes.
 *
 */
void cFEStateBuyAttrib::buyExperience()
{
    cFEStateBuyAttrib* self = this;
    self->mBank -= self->mTotalCost;
    self->mTotalCost = 0;
    self->updateTotalCost();
    self->mBuyPrompt = 0;
    cUIState_hideObjSafe(self, str_PopupGroup);
    cUIMenu* attribMenu = (self->pAttribMenu);
    attribMenu->setFocus(1);
    cBEEconInterface* iface = (cBEEconInterface*)cBE_getInterface_Fv(cBE_getBE(), 11);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < self->mPotentialExperience[i]; j++) {
            cBEEconInterface_buyAttribute(iface, 0, cBELibrary_getCharacterID(0), cFEStateBuyAttrib_attributeOrder[i]);
        }
    }
    iface->submit();
    int* max = self->mExperience;
    int* added = self->mPotentialExperience;
    for (int i = 0; i < 7; i++) {
        max[i] += added[i];
        added[i] = 0;
    }
    self->updateLevels();
    self->updateCostPerLevel();
    self->updateExperienceDisplay();
    self->updateBank();
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F54A8);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty function that returns at once. Nothing in the build references it.
 */
extern "C" void func_001F54A8(void) {

}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_cFEStateCareerStats);
#ifdef SKIP_ASM
extern void* cFEStateCareerStats_vtbl[];
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);

/**
 * Constructs a career-stats state.
 *
 * Runs the cUIState constructor cUIState_cUIState (the engine reaches it in the second argument register), stores
 * cFEStateCareerStats_vtbl, sets the state id (+0xC) to 42 and the user (+0x44) to 0, and copies player 0's control
 * mask from the custom interface (cFECustom_getPlayerCtrlMask) into +0x15, without checking that the interface exists.
 *
 * @return @p self
 */
extern "C" void* cFEStateCareerStats_cFEStateCareerStats(cFEStateCareerStats* self)
{
    cUIState_cUIState(self);
    VPTR_OF(cListNode, self) = cFEStateCareerStats_vtbl;
    self->mHashName = 42;
    self->mUserNum = 0;
    self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, 0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_35car_stat[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateCareerStats_initTotalEventMedal(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupLayout(cFEStateCareerStats* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_SetFrontEndMusicMode(void* self, int a1);

/**
 * Opens the career-stats screen ("35car_stat") on its highlights page and counts the event medals.
 *
 * Keeps the screen at 0x40, clears the scroll positions, event totals and medal bit masks (0x4C to 0x74) and
 * the page (0x48), then runs cFEStateCareerStats_initTotalEventMedal() (probably On Tour initTotalEventMedal), cFEStateCareerStats_setupLayout() (shows
 * the page's group, probably setupLayout) and switches the front-end sound-bank mode to 8.
 */
void cFEStateCareerStats::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_35car_stat), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    mTopHighlightNumber = 0;
    mTopRaceEvent = 0;
    mTopFreestyleEvent = 0;
    mTotalRaceEvent = 0;
    mTotalOwnedRaceEvent = 0;
    mbitOpenedRace = (unsigned int)0;
    mbitOwnedRaceMedal = (unsigned int)0;
    mTotalFreestyleEvent = 0;
    mbitOwnedFSMedal = (unsigned int)0;
    mTotalOwnedFSEvent = 0;
    mbitOpenedFS = (unsigned int)0;
    mStatsState = 0;
    initTotalEventMedal();
    setupLayout();
    cSSXAudio_SetFrontEndMusicMode(cAudioInterface_bxAudio(), 8);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onGainTransition);
#ifdef SKIP_ASM
extern "C" void cFEStateCareerStats_setupHighlightsList(cFEStateCareerStats* self);

/**
 * Builds the highlights list when the career-stats state's gain transition starts.
 *
 * Calls cFEStateCareerStats_setupHighlightsList() only when @p on is nonzero, and returns 0 like the base
 * cUIState_onGainTransition().
 *
 * @param on  the state's first-time flag as passed by the state stack; nonzero on the transition's first call
 * @return always 0
 */
bool cFEStateCareerStats::onGainTransition(int on)
{
    if (on != 0) {
        setupHighlightsList();
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onGainFocus);
#ifdef SKIP_ASM
extern "C" void cFEState_onGainFocus(void* self);
extern "C" void cFEStateCareerStats_setupMenuFocus(cFEStateCareerStats* self, int focus);

/**
 * Gives the career-stats state focus and re-applies the focus of its current menu.
 *
 * Runs the base cFEState_onGainFocus(), then cFEStateCareerStats_setupMenuFocus() with the menu index kept at
 * 0x48.
 */
void cFEStateCareerStats::onGainFocus(cUIState*)
{
    cFEState_onGainFocus(this);
    setupMenuFocus(mStatsState);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Answers the career-stats menu's events: 0x100 for next, option and misc (6, 8, 9), 0x101 for the rest.
 *
 * cUIMenu_onInput() sends 6 for the next button, 7 for previous, 8 for option and 9 for misc. Bit 0x100 plays
 * the selected item's feedback frame and bit 0 also plays its mSound[1], so a 0x100 answer leaves the button
 * silent.
 *
 * @param pMenu  the menu that sent the event (unused; On Tour pMenu)
 * @param Event  the menu event
 * @return 0x100 for kONNEXT, kONOPTION and kONMISC, otherwise 0x101
 */
int cFEStateCareerStats::onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
{
    switch (Event) {
    case cUIMenu::kONNEXT:
    case cUIMenu::kONOPTION:
    case cUIMenu::kONMISC:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * Leaves the career-stats screen on back, pushing the state the engine's custom interface gives.
 *
 * Only kONPREV does anything: getPrevState (vtable slot 5 of the custom interface) is asked with the widget's
 * misc data, and a returned state is pushed on the state stack.
 *
 * @param item  the widget that sent the event; nothing happens when null
 * @param msg  the widget event (6 back; 5 accept is ignored)
 */
void cFEStateCareerStats::onWidgetEvent(cUIThing* item, unsigned int msg)
{
    if (((void*)item) == 0) {
        return;
    }
    switch (msg) {
    case cUIState::kONNEXT:
        break;
    case cUIState::kONPREV: {
        cUICustomInterface* obj = (cUICustomInterface*)((void*)mpEngine->mpCustom);
        void* nextState = obj->getPrevState((cUIState*)this, item->mMiscData);
        if (nextState != 0) {
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onInputBegin);
#ifdef SKIP_ASM
extern "C" void cFEStateCareerStats_setupHighlightsList(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupMenuFocus(cFEStateCareerStats* self, int focus);
extern "C" void cFEStateCareerStats_setupLayout(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupRacePlatinumMedalsList(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupFSPlatinumMedalsList(cFEStateCareerStats* self);
extern char str_hll[];
extern char str_hlr[];
extern char str_high_up[];
extern char str_race_up[];
extern char str_free_up[];
extern char str_high_down[];
extern char str_race_down[];
extern char str_free_down[];

struct sVEi_56C8 {
    short delta;
    short index;
    int (*fn)(void*);
};


static inline int wantCall_56C8(char* w, int slot)
{
    sVEi_56C8* vt = *(sVEi_56C8**)(w + 8);
    return vt[slot].fn(vthis(w, vt[slot]));
}

static inline void playFrame_56C8(cFEStateCareerStats* s, char* name)
{
    unsigned short frame = cUIScreen_getFrameByLabel((cUIScreen*)((void*)s->mpScreen), GetHashValue32(name));
    if (frame != cUIScreen::kNoFrame)
        cUIScreen_playFrame(((void*)s->mpScreen), frame, 1);
}

/**
 * Pages through the career stats with left and right and scrolls the current page's list with up and down.
 *
 * Left and right (input slots 19 and 20) step the page (0x48) through 0-3 with wrap, play "hll" or "hlr" and
 * refresh the focus and layout. Up and down (slots 17 and 18) move the highlights list (0x4C; 3 of 24 rows
 * shown) on page 0 and the race (0x50) and freestyle (0x54) platinum lists (5 rows shown of the totals at
 * 0x58 and 0x68) on pages 1 and 2, playing the list's "up" or "down" frame. The UI sound is always played at
 * the end: 1 for a page change, 2 for a scroll, 4 at a list end, 0 for nothing.
 *
 * @param w  the controller input (cUIInput)
 * @return 1 when a key changed the page or a list, otherwise 0
 */
bool cFEStateCareerStats::onInputBegin(cUIInput* w)
{
    int msg = 0;
    int handled = 0;
    if (wantCall_56C8(((char*)w), 19)) {
        playFrame_56C8((this), str_hll);
        msg = 1;
        handled = 1;
        if (--mStatsState < 0) {
            mStatsState = cFEStateCareerStats::kCareerStats_RidersBest;
        }
    } else if (wantCall_56C8(((char*)w), 20)) {
        playFrame_56C8((this), str_hlr);
        msg = 1;
        handled = 1;
        if (++mStatsState >= 4) {
            mStatsState = 0;
        }
    }
    if (handled) {
        setupMenuFocus(mStatsState);
        setupLayout();
    }
    if (mStatsState == cFEStateCareerStats::kCareerStats_Highlights) {
        if (wantCall_56C8(((char*)w), 17)) {
            if (mTopHighlightNumber > 0) {
                mTopHighlightNumber--;
                setupHighlightsList();
                handled = 1;
                playFrame_56C8((this), str_high_up);
                msg = 2;
            } else {
                msg = 4;
            }
        } else if (wantCall_56C8(((char*)w), 18)) {
            if (mTopHighlightNumber + 3 < 0x18) {
                mTopHighlightNumber++;
                setupHighlightsList();
                handled = 1;
                playFrame_56C8((this), str_high_down);
                msg = 2;
            } else {
                msg = 4;
            }
        }
    } else if (mStatsState == cFEStateCareerStats::kCareerStats_Race_PlatMedals) {
        if (wantCall_56C8(((char*)w), 17)) {
            if (mTopRaceEvent > 0) {
                mTopRaceEvent--;
                setupRacePlatinumMedalsList();
                handled = 1;
                playFrame_56C8((this), str_race_up);
                msg = 2;
            } else {
                msg = 4;
            }
        } else if (wantCall_56C8(((char*)w), 18)) {
            if (mTopRaceEvent + 5 < mTotalRaceEvent) {
                handled = 1;
                mTopRaceEvent++;
                setupRacePlatinumMedalsList();
                playFrame_56C8((this), str_race_down);
                msg = 2;
            } else {
                msg = 4;
            }
        }
    } else if (mStatsState == cFEStateCareerStats::kCareerStats_FS_PlatMedals) {
        if (wantCall_56C8(((char*)w), 17)) {
            if (mTopFreestyleEvent > 0) {
                mTopFreestyleEvent--;
                setupFSPlatinumMedalsList();
                handled = 1;
                playFrame_56C8((this), str_free_up);
                msg = 2;
            } else {
                msg = 4;
            }
        } else if (wantCall_56C8(((char*)w), 18)) {
            if (mTopFreestyleEvent + 5 < mTotalFreestyleEvent) {
                handled = 1;
                mTopFreestyleEvent++;
                setupFSPlatinumMedalsList();
                playFrame_56C8((this), str_free_down);
                msg = 2;
            } else {
                msg = 4;
            }
        }
    }
    cUISoundInterface* sound = mpEngine->mpSound;
    if (sound == 0)
        sound = &gUISoundNull;
    if (sound != 0)
        sound->playSound(msg);
    return handled;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupLayout);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern char str_highlights[];
extern char str_race_platinum[];
extern char str_freestyle_platinum[];
extern char str_ridersbest[];
extern char str_horizontal_dash_b[];
extern char str_vertical_dash_b[];


/**
 * Shows the career-stats page chosen by the page index (+0x48).
 *
 * Hides "highlights", "race_platinum", "freestyle_platinum" and "ridersbest", then shows page 0, 1, 2 or 3 of those.
 * The "horizontal_dash_b" and "vertical_dash_b" lines are shown (vtable entry 9) except on pages 0 and 3.
 */
void cFEStateCareerStats::setupLayout()
{
    cUIState_hideObjSafe(this, str_highlights);
    cUIState_hideObjSafe(this, str_race_platinum);
    cUIState_hideObjSafe(this, str_freestyle_platinum);
    cUIState_hideObjSafe(this, str_ridersbest);
    int show = 1;
    switch (mStatsState) {
    case kCareerStats_Highlights:
        cUIState_showObjSafe(this, str_highlights);
        show = 0;
        break;
    case kCareerStats_Race_PlatMedals:
        cUIState_showObjSafe(this, str_race_platinum);
        break;
    case kCareerStats_FS_PlatMedals:
        cUIState_showObjSafe(this, str_freestyle_platinum);
        break;
    case kCareerStats_RidersBest:
        cUIState_showObjSafe(this, str_ridersbest);
        show = 0;
        break;
    }
    cUIThing* dashObj = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_horizontal_dash_b));
    if (((char*)dashObj) != 0) {
        dashObj->setVisible(show);
    }
    dashObj = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_vertical_dash_b));
    if (((char*)dashObj) != 0) {
        dashObj->setVisible(show);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupMenuFocus);
#ifdef SKIP_ASM
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateCareerStats_setupHighlightsList(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupRidersBest(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupRacePlatinumMedalsList(cFEStateCareerStats* self);
extern "C" void cFEStateCareerStats_setupFSPlatinumMedalsList(cFEStateCareerStats* self);
extern char str_highlightsMenu[];
extern char str_race_platinumdummyMenu[];
extern char str_fs_platinumdummyMenu[];
extern char str_ridersbestDummyMenu[];

/**
 * Switches the career stats to a page: focuses that page's dummy menu and fills its list.
 *
 * Stores @p menu at 0x48 (On Tour mStatsState), unfocuses "highlightsMenu", "race_platinumdummyMenu",
 * "fs_platinumdummyMenu" and "ridersbestDummyMenu" (setFocus, vtable slot 7), then focuses the page's one
 * without a null check and runs setupHighlightsList(), cFEStateCareerStats_setupRacePlatinumMedalsList() or cFEStateCareerStats_setupFSPlatinumMedalsList() (probably the race
 * and freestyle platinum-medal lists) or setupRidersBest().
 *
 * @param menu  the page: kCareerStats_Highlights, _Race_PlatMedals, _FS_PlatMedals or _RidersBest
 */
void cFEStateCareerStats::setupMenuFocus(int menu)
{
    mStatsState = menu;
    cUIThing* menuTab0 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_highlightsMenu));
    if (menuTab0 != 0)
        menuTab0->setFocus(0);
    cUIThing* menuTab1 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_race_platinumdummyMenu));
    if (menuTab1 != 0)
        menuTab1->setFocus(0);
    cUIThing* menuTab2 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_fs_platinumdummyMenu));
    if (menuTab2 != 0)
        menuTab2->setFocus(0);
    cUIThing* menuTab3 = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ridersbestDummyMenu));
    if (menuTab3 != 0)
        menuTab3->setFocus(0);
    switch (mStatsState) {
    case kCareerStats_Highlights:
        menuTab0->setFocus(1);
        setupHighlightsList();
        break;
    case kCareerStats_Race_PlatMedals:
        menuTab1->setFocus(1);
        setupRacePlatinumMedalsList();
        break;
    case kCareerStats_FS_PlatMedals:
        menuTab2->setFocus(1);
        setupFSPlatinumMedalsList();
        break;
    case kCareerStats_RidersBest:
        menuTab3->setFocus(1);
        setupRidersBest();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupHighlightsList);
#ifdef SKIP_ASM
struct cUIText;
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
int cBEScoreInterface_getCurrentHighlightLevel(void* score, int idx);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void MEM_fill(void* dst, int c, int n);
struct sTrickId;
extern "C" void gGenTrickName(char* buf, sTrickId* id, int size);
extern "C" void gGenMonsterTrickReqName(char* buf, unsigned int trick, int size);
extern char str_HL_arrowup[];
extern char str_HL_arrowdown[];
extern char str_checkmarkpct_d[];
extern char str_checkboxpct_d[];
extern char str_pct_s_Monster_Trick[];
extern char str_hlsecpct_da[];
extern char str_kT_FEUnlockMonsterTrick[];
extern char str_hlpct_d[];
extern char str_pct_spct_d[];
extern char str_hlsecpct_d[];
extern char* D_00441BA0[];
extern unsigned int D_00441B40[];

// PORT: 64-bit bitfield container (long is 8 bytes here).
struct sTrickId5DA0 {
    unsigned long rest : 59;
    unsigned long type : 5;
};

union uTrickBuf5DA0 {
    char str[32];
    sTrickId5DA0 id;
};


/**
 * Fills the three visible rows of the career highlights list from the current scroll position.
 *
 * The list has 24 highlights, 3 tiers in each of 8 categories. Shows "HL_arrowup" when scrolled down and
 * "HL_arrowdown" while rows remain below. Each row gets the highlight's text, a check mark or empty check box
 * from the score interface's current highlight level, "<trick> Monster Trick", and the monster-trick requirement
 * once the tier is reached ("kT_FEUnlockMonsterTrick" before that).
 *
 */
void cFEStateCareerStats::setupHighlightsList()
{
    cFEStateCareerStats* self = this;
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    if (self->mTopHighlightNumber > 0) {
        cUIState_showObjSafe(self, str_HL_arrowup);
    } else {
        cUIState_hideObjSafe(self, str_HL_arrowup);
    }
    if (self->mTopHighlightNumber + 3 < 0x18) {
        cUIState_showObjSafe(self, str_HL_arrowdown);
    } else {
        cUIState_hideObjSafe(self, str_HL_arrowdown);
    }
    char name[32];
    uTrickBuf5DA0 trickBuf;
    char onName[32];
    char offName[32];
    char trick[112];
    char text[112];
    for (int i = 0; i < 3; i++) {
        int idx = i + self->mTopHighlightNumber;
        int level = idx / 3;
        int sub = idx % 3;
        sprintf(name, str_hlpct_d, i + 1);
        int done = 0;
        cUIText* textObj = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
        if (textObj != 0) {
            sprintf(trickBuf.str, str_pct_spct_d, D_00441BA0[level], sub + 1);
            cUIText_setUnicodeStringByID(textObj, GetHashValue32(trickBuf.str));
            sprintf(onName, str_checkmarkpct_d, i + 1);
            sprintf(offName, str_checkboxpct_d, i + 1);
            if (sub < cBEScoreInterface_getCurrentHighlightLevel(score, level)) {
                cUIState_showObjSafe(self, onName);
                done = 1;
                cUIState_hideObjSafe(self, offName);
            } else {
                cUIState_hideObjSafe(self, onName);
                cUIState_showObjSafe(self, offName);
            }
        }
        sprintf(name, str_hlsecpct_d, i + 1);
        textObj = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
        if (textObj != 0) {
            MEM_fill(&trickBuf, 0, 8);
            trickBuf.id.type = D_00441B40[idx];
            gGenTrickName(trick, (sTrickId*)&trickBuf.id, 100);
            sprintf(text, str_pct_s_Monster_Trick, trick);
            cUIText_setAsciiString(textObj, text);
        }
        sprintf(name, str_hlsecpct_da, i + 1);
        textObj = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(name));
        if (textObj != 0) {
            if (done) {
                gGenMonsterTrickReqName(trickBuf.str, D_00441B40[idx], 100);
                cUIText_setAsciiString(textObj, trickBuf.str);
            } else {
                cUIText_setUnicodeStringByID(textObj, GetHashValue32(str_kT_FEUnlockMonsterTrick));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupRacePlatinumMedalsList);
#ifdef SKIP_ASM
struct cUIText;
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* cBENewRaceInterface_getLocationName(void* race, int track);
extern "C" int cBENewRaceInterface_getPlatinumGoal(void* race, int track, int mode);
extern "C" char* formatMinutesSeconds(int time);
extern char str_race_arrowup[];
extern char str_race_arrowdown[];
extern char str_race_medals[];
extern char str_race_peakNumpct_d[];
extern char str_race_trackpct_d[];
extern char str_race_medalpct_d[];
extern char str_pct_d_j[];
extern char str_pct_d_pct_d_d[];
extern char str_pct_s_Race[];
extern char str_timepct_d_b[];
extern int D_004780D0[];
extern int D_004780F0[];


/**
 * Fills the career-stats race platinum medal list: five rows from the scroll position (+0x50).
 *
 * Shows "race_arrowup" while the scroll is above 0 and "race_arrowdown" while rows remain below; "race_medals" reads
 * "%d / %d" (owned +0x5C of the total +0x58). Each row gets its peak number (D_004780F0 + 1), the location name
 * (cBENewRaceInterface_getLocationName of D_004780D0; "%s Race" with the mode-4 time for every third event, else the
 * mode-0 time), the best time formatted by formatMinutesSeconds (cBENewRaceInterface_getPlatinumGoal) and the "race_medal%d" icon when bit idx of
 * +0x60 is set; rows past the total are hidden.
 *
 */
void cFEStateCareerStats::setupRacePlatinumMedalsList()
{
    cFEStateCareerStats* self = this;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (self->mTopRaceEvent > 0) {
        cUIState_showObjSafe(self, str_race_arrowup);
    } else {
        cUIState_hideObjSafe(self, str_race_arrowup);
    }
    if (self->mTopRaceEvent + 5 < self->mTotalRaceEvent) {
        cUIState_showObjSafe(self, str_race_arrowdown);
    } else {
        cUIState_hideObjSafe(self, str_race_arrowdown);
    }
    char buf[32];
    cUIText* textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_race_medals));
    sprintf(buf, str_pct_d_pct_d_d, self->mTotalOwnedRaceEvent, self->mTotalRaceEvent);
    cUIText_setAsciiString(textWidget, buf);
    for (int i = 0; i < 5; i++) {
        int idx = i + self->mTopRaceEvent;
        if (idx < self->mTotalRaceEvent) {
            char peak[32];
            char num[16];
            char track[32];
            char text[32];
            char medal[80];
            sprintf(peak, str_race_peakNumpct_d, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(peak));
            sprintf(num, str_pct_d_j, D_004780F0[idx] + 1);
            cUIText_setAsciiString(textWidget, num);
            sprintf(track, str_race_trackpct_d, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(track));
            int raceTime;
            if (idx % 3 == 0) {
                sprintf(text, str_pct_s_Race, cBENewRaceInterface_getLocationName(race, D_004780D0[idx]));
                raceTime = cBENewRaceInterface_getPlatinumGoal(race, D_004780D0[idx], 4);
                cUIText_setAsciiString(textWidget, text);
            } else {
                raceTime = cBENewRaceInterface_getPlatinumGoal(race, D_004780D0[idx], 0);
                cUIText_setAsciiString(textWidget, cBENewRaceInterface_getLocationName(race, D_004780D0[idx]));
            }
            sprintf(text, str_timepct_d_b, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(text));
            cUIText_setAsciiString(textWidget, formatMinutesSeconds(raceTime));
            sprintf(medal, str_race_medalpct_d, i);
            if (self->mbitOwnedRaceMedal & (1 << idx)) {
                cUIState_showObjSafe(self, medal);
            } else {
                cUIState_hideObjSafe(self, medal);
            }
        } else {
            char peak[32];
            char track[32];
            char time[32];
            char medal[32];
            sprintf(peak, str_race_peakNumpct_d, i);
            cUIState_hideObjSafe(self, peak);
            sprintf(track, str_race_trackpct_d, i);
            cUIState_hideObjSafe(self, track);
            sprintf(time, str_timepct_d_b, i);
            cUIState_hideObjSafe(self, time);
            sprintf(medal, str_race_medalpct_d, i);
            cUIState_hideObjSafe(self, medal);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupFSPlatinumMedalsList);
#ifdef SKIP_ASM
struct cUIText;
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* cBENewRaceInterface_getLocationName(void* race, int track);
extern "C" int cBENewRaceInterface_getPlatinumGoal(void* race, int track, int mode);
extern "C" int cBENewRaceInterface_getLocationDefaultGameEvent(void* game, int track);
extern char str_fs_arrowup[];
extern char str_fs_arrowdown[];
extern char str_fs_medals[];
extern char str_fs_peakNumpct_d[];
extern char str_fs_trackpct_d[];
extern char str_fs_pointspct_d[];
extern char str_fs_medalpct_d[];
extern char str_pct_d_j[];
extern char str_pct_d_pct_d_d[];
extern char str_pct_s_Jam[];
extern int D_00478110[];


/**
 * Fills the career-stats freestyle platinum medal list: five rows from the scroll position (+0x54).
 *
 * Like the race list with the "fs_" widgets: the peak number is idx / 4 + 1, the location name of D_00478110 ("%s Jam"
 * with the mode-5 score for every fourth event, else the score of the location's mode from cBENewRaceInterface_getLocationDefaultGameEvent), the points
 * as "%d" and the medal icon by bit idx of +0x70; the counter reads +0x6C of +0x68.
 *
 */
void cFEStateCareerStats::setupFSPlatinumMedalsList()
{
    cFEStateCareerStats* self = this;
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    if (self->mTopFreestyleEvent > 0) {
        cUIState_showObjSafe(self, str_fs_arrowup);
    } else {
        cUIState_hideObjSafe(self, str_fs_arrowup);
    }
    if (self->mTopFreestyleEvent + 5 < self->mTotalFreestyleEvent) {
        cUIState_showObjSafe(self, str_fs_arrowdown);
    } else {
        cUIState_hideObjSafe(self, str_fs_arrowdown);
    }
    char buf[32];
    cUIText* hdr = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_fs_medals));
    sprintf(buf, str_pct_d_pct_d_d, self->mTotalOwnedFSEvent, self->mTotalFreestyleEvent);
    cUIText_setAsciiString(hdr, buf);
    cUIText* textWidget;
    for (int i = 0; i < 5; i++) {
        int idx = i + self->mTopFreestyleEvent;
        if (idx < self->mTotalFreestyleEvent) {
            char peak[32];
            char num[16];
            char track[32];
            char text[16];
            char timeName[32];
            char medal[64];
            int level = idx / 4;
            sprintf(peak, str_fs_peakNumpct_d, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(peak));
            sprintf(num, str_pct_d_j, level + 1);
            cUIText_setAsciiString(textWidget, num);
            sprintf(track, str_fs_trackpct_d, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(track));
            int eventPoints;
            if (idx % 4 == 0) {
                sprintf(text, str_pct_s_Jam, cBENewRaceInterface_getLocationName(race, D_00478110[idx]));
                cUIText_setAsciiString(textWidget, text);
                eventPoints = cBENewRaceInterface_getPlatinumGoal(race, D_00478110[idx], 5);
            } else {
                cUIText_setAsciiString(textWidget, cBENewRaceInterface_getLocationName(race, D_00478110[idx]));
                eventPoints = cBENewRaceInterface_getPlatinumGoal(race, D_00478110[idx], cBENewRaceInterface_getLocationDefaultGameEvent(race, D_00478110[idx]));
            }
            sprintf(text, str_pct_d_j, eventPoints);
            sprintf(timeName, str_fs_pointspct_d, i);
            textWidget = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(timeName));
            cUIText_setAsciiString(textWidget, text);
            sprintf(medal, str_fs_medalpct_d, i);
            if (self->mbitOwnedFSMedal & (1 << idx)) {
                cUIState_showObjSafe(self, medal);
            } else {
                cUIState_hideObjSafe(self, medal);
            }
        } else {
            char peak[32];
            char track[32];
            char time[32];
            char medal[32];
            sprintf(peak, str_fs_peakNumpct_d, i);
            cUIState_hideObjSafe(self, peak);
            sprintf(track, str_fs_trackpct_d, i);
            cUIState_hideObjSafe(self, track);
            sprintf(time, str_fs_pointspct_d, i);
            cUIState_hideObjSafe(self, time);
            sprintf(medal, str_fs_medalpct_d, i);
            cUIState_hideObjSafe(self, medal);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setTextSafe);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);

/**
 * Sets the text of the screen object named @p name when it exists.
 *
 * @param name  the object's name
 * @param str   the ASCII text
 */
void cFEStateCareerStats::setTextSafe(char* name, const char* str)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(name));
    if (text != 0) {
        cUIText_setAsciiString(text, str);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupRidersBest);
#ifdef SKIP_ASM
struct cUIText;
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
int GetHashValue32(char* str);
int cBELibrary_getCharacterID(int);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* cBENewRaceInterface_getLocationName(void* race, int track);
extern "C" char* fptodp(float v);
extern char str_dash[] __attribute__((aligned(8)));
extern char str_pct_d_j[];
extern char str_dash_sp_dash_sp_dash[];
extern char str_ko[];
extern char D_004A23C0[];
extern char str_pct_d_sec[];
extern char str_ubers[];
extern char str_airtime[];
extern char str_ko_event[];
extern char str_handplants[];
extern char str_handplants_event[];
extern char str_ubers_event[];
extern char str_superubers[];
extern char str_superubers_event[];
extern char str_ubergrinds[];
extern char str_ubergrinds_event[];
extern char str_pct_0_1f_m[];
extern char str_longrail[];
extern char str_longrail_event[];
extern char str_airtime_event[];
extern char str_favoriteEvent_label[];
extern char str_kT_ovrcmnBestCombo[];
extern char str_favoriteEvent[];

// One "best" record per stat: the value and the track it was set on.
struct sBestEntry_6890 {
    int value;
    int track;
};


extern "C" tRiderBest* cBEScoreInterface_getPlayerBestStats(void* iface, int a, int charId);


/**
 * Fills the rider's-best page with the best records of user 0's character and the location of each.
 *
 * The record block comes from cBEScoreInterface_getPlayerBestStats() on the score interface (8), profile 0. Knockouts, ubers, super
 * ubers and uber grinds show as "%d", handplants and airtime as "%d sec.", the longest rail as "%0.1f m."
 * (value / 100), each with "[location]" from the race interface; a record of 0 or less shows "-" and "- - -".
 * "favoriteEvent" shows the best combo (record 4) under the label kT_ovrcmnBestCombo, whose text widget is
 * not null-checked. Texts are set through cFEStateCareerStats_setTextSafe() (On Tour setTextSafe).
 *
 */
void cFEStateCareerStats::setupRidersBest()
{
    cFEStateCareerStats* self = this;
    char buf[112];
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* iface8 = cBE_getInterface_Fv(cBE_getBE(), 8);
    tRiderBest best = *cBEScoreInterface_getPlayerBestStats(iface8, 0, cBELibrary_getCharacterID(0));
    int value;
    int track;

    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[0].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[0].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_j, value);
            self->setTextSafe(str_ko, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_ko_event, buf);
        } else {
            self->setTextSafe(str_ko, str_dash);
            self->setTextSafe(str_ko_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[5].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[5].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_sec, value);
            self->setTextSafe(str_handplants, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_handplants_event, buf);
        } else {
            self->setTextSafe(str_handplants, str_dash);
            self->setTextSafe(str_handplants_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[1].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[1].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_j, value);
            self->setTextSafe(str_ubers, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_ubers_event, buf);
        } else {
            self->setTextSafe(str_ubers, str_dash);
            self->setTextSafe(str_ubers_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[2].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[2].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_j, value);
            self->setTextSafe(str_superubers, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_superubers_event, buf);
        } else {
            self->setTextSafe(str_superubers, str_dash);
            self->setTextSafe(str_superubers_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[3].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[3].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_j, value);
            self->setTextSafe(str_ubergrinds, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_ubergrinds_event, buf);
        } else {
            self->setTextSafe(str_ubergrinds, str_dash);
            self->setTextSafe(str_ubergrinds_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[6].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[6].track;
        if (value > 0) {
            sprintf(buf, str_pct_0_1f_m, fptodp((float)value * 0.009999999776482582f));
            self->setTextSafe(str_longrail, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_longrail_event, buf);
        } else {
            self->setTextSafe(str_longrail, str_dash);
            self->setTextSafe(str_longrail_event, str_dash_sp_dash_sp_dash);
        }
    }
    {
        value = ((sBestEntry_6890*)&best.eventStat[0][0])[7].value;
        track = ((sBestEntry_6890*)&best.eventStat[0][0])[7].track;
        if (value > 0) {
            sprintf(buf, str_pct_d_sec, value);
            self->setTextSafe(str_airtime, buf);
            sprintf(buf, D_004A23C0, cBENewRaceInterface_getLocationName(race, track));
            self->setTextSafe(str_airtime_event, buf);
        } else {
            self->setTextSafe(str_airtime, str_dash);
            self->setTextSafe(str_airtime_event, str_dash_sp_dash_sp_dash);
        }
    }
    cUIText* title = (cUIText*)cUIScreen_getObjectByHashName(((void*)self->mpScreen), GetHashValue32(str_favoriteEvent_label));
    cUIText_setUnicodeStringByID(title, GetHashValue32(str_kT_ovrcmnBestCombo));
    if (((sBestEntry_6890*)&best.eventStat[0][0])[4].value > 0) {
        sprintf(buf, str_pct_d_j, ((sBestEntry_6890*)&best.eventStat[0][0])[4].value);
    } else {
        // strcpy(buf, "-"): gcc's inline strcpy of a literal is a 2-byte aligned copy
        __builtin_memcpy(buf, str_dash, 2);
    }
    self->setTextSafe(str_favoriteEvent, buf);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_getOwnedMedalByith);
#ifdef SKIP_ASM
/**
 * Returns the bit number of the @p n-th set bit (counting from 0) among the low @p count bits of @p mask.
 *
 * @param mask   the bit set
 * @param n      which set bit
 * @param count  how many bits to look at
 * @return the bit number, or -1 when there are not that many
 */
int cFEStateCareerStats::getOwnedMedalByith(unsigned int mask, int n, int count)
{
    int seen = -1;
    for (int i = 0; i < count; i++) {
        unsigned int bit = 1u << i;
        if (mask & bit) {
            seen++;
            if (seen == n) return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_initTotalEventMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int cBENewPlayerInterface_getHumanRider(void* player, int idx);
extern "C" int cBERewardInterface_isPeakLocked(void* reward, int id, int n);
extern "C" int cBENewRaceInterface_getLocationDefaultGameEvent(void* game, int track);
int cBENewPlayerInterface_getRiderCharID(void* self, int idx);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int mode, int track);
extern int D_004780D0[];
extern int D_00478110[];

/**
 * Counts the career-stats events and the platinum medals the human rider owns.
 *
 * For each peak not locked for the human rider (cBERewardInterface_isPeakLocked returns 0; peaks 1 and 2 can be) adds 3, 3 or 2 race
 * events (+0x58) and 4 freestyle events (+0x68). Then for the 8 race events (D_004780D0; mode 4 for every third, else
 * 0) and the 12 freestyle events (D_00478110; mode 5 for every fourth, else cBENewRaceInterface_getLocationDefaultGameEvent's mode) whose
 * cBERewardInterface_getTrackMedal is 0, sets bit i of +0x60 / +0x70 and counts +0x5C / +0x6C. All events are checked
 * whether or not their peak is counted.
 *
 */
void cFEStateCareerStats::initTotalEventMedal()
{
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* game = cBE_getInterface_Fv(cBE_getBE(), 0);
    char charID = cBENewPlayerInterface_getRiderCharID(player, cBENewPlayerInterface_getHumanRider(player, 0));
    if (cBERewardInterface_isPeakLocked(reward, cBENewPlayerInterface_getHumanRider(player, 0), 0) == 0) {
        mTotalRaceEvent += 3;
        mTotalFreestyleEvent += 4;
    }
    if (cBERewardInterface_isPeakLocked(reward, cBENewPlayerInterface_getHumanRider(player, 0), 1) == 0) {
        mTotalRaceEvent += 3;
        mTotalFreestyleEvent += 4;
    }
    if (cBERewardInterface_isPeakLocked(reward, cBENewPlayerInterface_getHumanRider(player, 0), 2) == 0) {
        mTotalRaceEvent += 2;
        mTotalFreestyleEvent += 4;
    }
    for (int i = 0; i < 8; i++) {
        int mode = 0;
        if (i % 3 == 0) mode = 4;
        char medal = cBERewardInterface_getTrackMedal(reward, 0, charID, mode, D_004780D0[i]);
        if (medal == 0) {
            mbitOwnedRaceMedal |= 1 << i;
            mTotalOwnedRaceEvent += 1;
        }
    }
    for (int i = 0; i < 12; i++) {
        int mode;
        if ((i & 3) == 0) {
            mode = 5;
        } else {
            mode = cBENewRaceInterface_getLocationDefaultGameEvent(game, D_00478110[i]);
        }
        char medal = cBERewardInterface_getTrackMedal(reward, 0, charID, mode, D_00478110[i]);
        if (medal == 0) {
            mbitOwnedFSMedal |= 1 << i;
            mTotalOwnedFSEvent += 1;
        }
    }
}
#endif

