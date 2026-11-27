#ifndef FE_FESTATESTATS_H
#define FE_FESTATESTATS_H

// cFEStateStats: the online high-score / stats screen ("249Stats": overall, race and freestyle rankings per event
// and course, ten result rows). A cFEStateNet (fe/festatenet.h, 0x6D0). EA's name from the preview's linkonce
// sections (_$_13cFEStateStats, onMenuGainFocus__13cFEStateStatsP8cUIGroup). EA's source file is not known: no
// preview assert falls in its methods (link order puts it after cFEStateOnlineMainMenu, whose cFEStateOnlineMainMenu_onWidgetCreate asserts
// in festatenetmainmenu.cpp), so the header is named after the class.
//
// Size 0x6F0: cMemMan_alloc(0x6F0, "cFEStateStats") + cFEStateStats_cFEStateStats(mem, pEngine) (cFEStateOnlineMainMenu_onWidgetEvent); the
// constructor stores up to 0x6EC. Constructors cFEStateStats_cFEStateStats and cFEStateStats_cFEStateStats1 (the default one), destructor
// cFEStateStats__cFEStateStats (fe/fepopupmisc; the preview's _$_13cFEStateStats). Retail vtable cFEStateStats_vtbl (32 slots: 1 dtor, 4
// onCreateScreen cFEStateStats_onCreateScreen, 7 onGainTransition cFEStateStats_onGainTransition, 9 onInputAlways cFEStateStats_onInputAlways, 12 onUpdate
// cFEStateStats_onUpdate, 13 cFEStateStats_onMenuGainFocus, 18 onWidgetCreate cFEStateStats_onWidgetCreate, 19 onWidgetEvent cFEStateStats_onWidgetEvent, 20
// onWidgetMenu cFEStateStats_onWidgetMenu, 25 cFEStateStats_fillHelpPopup (help), 31 cFEStateStats_onNetEvent (net event 0xE1), 32 cFEStateStats_publishPresence).
// Methods: fe/feasyncfile 0x1C2D18-0x1C4F58. tools/layout_xlate.py: preview 0xC4 = retail 0x6E4 (+0x620).
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. No On Tour DWARF and no
// preview accessors exist for this class, so the own fields are named from their retail use. Evidence tags:
//   [R] retail's constructors cFEStateStats_cFEStateStats / cFEStateStats_cFEStateStats1 and destructor cFEStateStats__cFEStateStats (and cFEStateNet's);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cUIMenu;

struct cFEStateStats : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateStats_vtbl (32 slots), stored by cFEStateStats_cFEStateStats / cFEStateStats_cFEStateStats1 after
    //       the cFEStateNet ctor and by the destructor cFEStateStats__cFEStateStats
    // cFEStateStats's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateStats_cFEStateStats stores 0x42
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateStats
    cUIMenu* mpMenus[4];            // 0x6D0 [R] 0 (both ctors loop over the four); [U] onWidgetCreate cFEStateStats_onWidgetCreate: [0] "HeadingsMenu",
                                    //       [1] "EventsMenu", [2] "CoursesMenu", [3] "ResultsMenu" (scrollable, input map 1); cFEStateStats_focusNextMenu /
                                    //       cFEStateStats_focusPrevMenu move the focus with mpMenus[miFocusMenu]; cFEStateStats_onNetEvent selects row 0 of [3]
    int miMode;                     // 0x6E0 [R] 0; [U] cFEStateStats_setMode(self, mode) stores it (0 overall, 1 race, 2 freestyle: the "Txt_Overal" /
                                    //       "Txt_Race" / "Txt_FreeStyle" highlights); cFEStateStats_onWidgetEvent / cFEStateStats_fillEventList / cFEStateStats_fillCourseList switch on it
    unsigned int miFocusMenu;       // 0x6E4 [R] 0; [U] the focused mpMenus[] index: cFEStateStats_focusNextMenu / cFEStateStats_focusPrevMenu step it (0..3,
                                    //       wrapping); onInputAlways cFEStateStats_onInputAlways moves only while it is < 3
    int miFirstResult;              // 0x6E8 [R] 0; [U] the results list's scroll offset: onWidgetMenu cFEStateStats_onWidgetMenu stores it (event 4),
                                    //       cFEStateStats_fillResultRows shows rows miFirstResult..+9, cFEStateStats_requestResults resets it
    int mbShowTime;                 // 0x6EC [R] 1 (cFEStateStats_cFEStateStats); [U] cFEStateStats_setMode sets it for mode 1, clears it for 0 and 2;
                                    //       cFEStateStats_fillResultRows prints the score as a time (cFEStateStats_formatTime) when set, else as points

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateStats1() __asm__("cFEStateStats_cFEStateStats1");
    // retail's destructor cFEStateStats__cFEStateStats stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateStats();
    virtual void onCreateScreen() __asm__("cFEStateStats_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* obj) __asm__("cFEStateStats_onWidgetCreate");
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateStats_onGainTransition");
    virtual void onUpdate() __asm__("cFEStateStats_onUpdate__FPv");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int index)
        __asm__("cFEStateStats_onWidgetMenu");
    virtual void onMenuGainFocus(cUIGroup*) __asm__("cFEStateStats_onMenuGainFocus__FPv");
};

CHECK_OFFSET(cFEStateStats, mpMenus, 0x6D0);
CHECK_OFFSET(cFEStateStats, miMode, 0x6E0);
CHECK_OFFSET(cFEStateStats, miFocusMenu, 0x6E4);
CHECK_OFFSET(cFEStateStats, miFirstResult, 0x6E8);
CHECK_OFFSET(cFEStateStats, mbShowTime, 0x6EC);
CHECK_SIZE(cFEStateStats, 0x6F0);

#endif /* FE_FESTATESTATS_H */
