#include "common.h"

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
extern "C" void cFEStateBonusMaterial_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_146Bonusmat), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_004A14F8[];
extern char D_004A1500[];
extern char D_004A1508[];

/**
 * Numbers the bonus material entries "1", "2" and "3" as 0-2.
 *
 * @param menu  the widget being created
 */
extern "C" void cFEStateBonusMaterial_onWidgetCreate(void* self, void* menu)
{
    int menuHash = *(int*)((char*)menu + 0x38);
    if (menuHash == GetHashValue32(D_004A14F8)) {
        *(int*)((char*)menu + 0x18) = 0;
    }
    menuHash = *(int*)((char*)menu + 0x38);
    if (menuHash == GetHashValue32(D_004A1500)) {
        *(int*)((char*)menu + 0x18) = 1;
    }
    menuHash = *(int*)((char*)menu + 0x38);
    if (menuHash == GetHashValue32(D_004A1508)) {
        *(int*)((char*)menu + 0x18) = 2;
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
extern "C" int cFEStateBonusMaterial_onWidgetMenu(void* self, int menu, unsigned int key)
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

extern "C" void* cUIState_onGainFocus(void* self);

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onGainFocus__FPv);
#ifdef SKIP_ASM
/**
 * Gives the bonus-material state focus using the base cUIState_onGainFocus().
 *
 * @return no meaningful value (cUIState_onGainFocus() returns nothing)
 */
void* cFEStateBonusMaterial_onGainFocus(void* self)
{
    return cUIState_onGainFocus(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatebonusmaterial", cFEStateBonusMaterial_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001D22F0(void* self, void* engine, void* owner, signed char idx);
extern "C" void func_001D2598(void* self, const char* a, const char* b);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cUIStateStack_push(void* list, void* item);
extern char str_cFEStateVideoPlayer[];
extern const char* D_00441128[];

struct sVEntryK195600 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Plays the selected bonus video in a cFEStateVideoPlayer popup, or on event 6 pushes the factory's state.
 *
 * @param item  the entry (its index at +0x18 picks the video from D_00441128), or 0
 * @param event  widget event number (On Tour Event)
 */
extern "C" void cFEStateBonusMaterial_onWidgetEvent(void* self, void* item, unsigned int event)
{
    if (item == 0)
        return;
    switch (event) {
    case 4:
        break;
    case 5: {
        void* popup = func_001D22F0(cMemMan_alloc(640, str_cFEStateVideoPlayer, 0, 0), *(void**)((char*)self + 0x10), self,
                                *(signed char*)((char*)self + 0x44));
        func_001D2598(popup, D_00441128[*(int*)((char*)item + 0x18)], 0);
        cUIStateStack_pushPopup(*(char**)((char*)self + 0x10) + 0x18, popup);
        break;
    }
    case 6: {
        char* factory = **(char***)((char*)self + 0x10);
        sVEntryK195600* vt = *(sVEntryK195600**)(factory + 4);
        void* nextState = vt[5].fn(factory + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0)
            cUIStateStack_push(*(char**)((char*)self + 0x10) + 0x18, nextState);
        break;
    }
    }
}
#endif

