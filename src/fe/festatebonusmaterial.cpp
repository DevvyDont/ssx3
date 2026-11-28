#include "common.h"
#include "vtable.h"
#include "ui/uithing.h"
#include "ui/uiengine.h"
#include "fe/festatebonusmaterial.h"

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_146Bonusmat[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the bonus material screen ("146Bonusmat") and plays its first frame.
 */
void cFEStateBonusMaterial::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_146Bonusmat), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_1_c[];
extern char str_2[];
extern char str_3[];

/**
 * Numbers the bonus material entries "1", "2" and "3" as 0-2.
 *
 * @param menu  the widget being created
 */
void cFEStateBonusMaterial::onWidgetCreate(cUIThing* menu)
{
    int menuHash = menu->mHashName;
    if (menuHash == GetHashValue32(str_1_c)) {
        menu->mMiscData = 0;
    }
    menuHash = menu->mHashName;
    if (menuHash == GetHashValue32(str_2)) {
        menu->mMiscData = 1;
    }
    menuHash = menu->mHashName;
    if (menuHash == GetHashValue32(str_3)) {
        menu->mMiscData = 2;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Swallows menu events 8 and 9 on the bonus material screen.
 *
 * @param menu  the menu (On Tour cUIMenu* pMenu), unused
 * @param key  menu event number (On Tour Event)
 * @return 0x100 for events 8 and 9, otherwise 0x101
 */
// Key filter: keys 8 and 9 return 0x100, every other key 0x101.
int cFEStateBonusMaterial::onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int)
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
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the bonus-material state focus using the base cUIState_onGainFocus().
 *
 * @param pPrevState  the state that had focus before, passed on
 */
void cFEStateBonusMaterial::onGainFocus(cUIState* pPrevState)
{
    cUIState::onGainFocus(pPrevState);
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateVideoPlayer_cFEStateVideoPlayer(void* self, void* engine, void* owner, signed char idx);
extern "C" void cFEStateVideoPlayer_setVideoSourceFiles(void* self, const char* a, const char* b);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEStateVideoPlayer[];
extern const char* cFEStateBonusMaterial_videoFiles[];


/**
 * Plays the selected bonus video in a cFEStateVideoPlayer popup, or on kONPREV pushes the factory's state.
 *
 * @param item  the entry (its index at +0x18 picks the video from cFEStateBonusMaterial_videoFiles), or 0
 * @param event  widget event number (On Tour Event)
 */
void cFEStateBonusMaterial::onWidgetEvent(cUIThing* item, unsigned int event)
{
    if (((void*)item) == 0)
        return;
    switch (event) {
    case cUIState::kONSELECT:
        break;
    case cUIState::kONNEXT: {
        void* popup = cFEStateVideoPlayer_cFEStateVideoPlayer(cMemMan_alloc(640, str_cFEStateVideoPlayer, 0, 0),
                mpEngine, this,
                                mUserNum);
        cFEStateVideoPlayer_setVideoSourceFiles(popup, cFEStateBonusMaterial_videoFiles[item->mMiscData], 0);
        cUIStateStack_pushPopup(&mpEngine->mStateStack, popup);
        break;
    }
    case cUIState::kONPREV: {
        cUICustomInterface* factory = (cUICustomInterface*)((char*)mpEngine->mpCustom);
        void* nextState = factory->getPrevState((cUIState*)this, item->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        break;
    }
    }
}
#endif

