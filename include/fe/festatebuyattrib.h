#ifndef FE_FESTATEBUYATTRIB_H
#define FE_FESTATEBUYATTRIB_H
#pragma interface

// cFEStateBuyAttrib: the lodge's attribute store screen (a cFEState); cFEReal creates it with cMemMan_alloc(176, "cFEStateBuyAttrib").
// Size 0xB0 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateBuyAttrib : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateBuyAttrib
    cUIScreen* mpAttribScreen;      // 0x048 [O] mpScreen (On Tour's own member hides cUIState::mpScreen)
    int mExperience[7];             // 0x04C [O]
    int mExperienceLevel[7];        // 0x068 [O]
    int mPotentialExperience[7];    // 0x084 [O]
    int mTotalCost;                 // 0x0A0 [O]
    int mBank;                      // 0x0A4 [O]
    int mBuyPrompt;                 // 0x0A8 [O] bool
    cUIMenu* pAttribMenu;           // 0x0AC [O]

    // Methods bound to their retail symbols by asm labels.
    void updateLevels() __asm__("cFEStateBuyAttrib_updateLevels");
    void updateCostPerLevel() __asm__("cFEStateBuyAttrib_updateCostPerLevel");
    void updateExperienceDisplay() __asm__("cFEStateBuyAttrib_updateExperienceDisplay");
    void updateTotalCost() __asm__("cFEStateBuyAttrib_updateTotalCost");
    void updateBank() __asm__("cFEStateBuyAttrib_updateBank");
    void buyExperience() __asm__("cFEStateBuyAttrib_buyExperience");
    virtual ~cFEStateBuyAttrib();   // g++'s own symbol _$_17cFEStateBuyAttrib (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateBuyAttrib_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* widget) __asm__("cFEStateBuyAttrib_onWidgetCreate");
    virtual bool onGainTransition(int on) __asm__("cFEStateBuyAttrib_onGainTransition");
    virtual void onInputAlways(cUIInput* w) __asm__("cFEStateBuyAttrib_onInputAlways");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateBuyAttrib_onGainFocus");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int) __asm__("cFEStateBuyAttrib_onWidgetMenu");
    virtual void onPopupEvent(cUIState* sender, unsigned int msg) __asm__("cFEStateBuyAttrib_onPopupEvent");
    virtual void onWidgetEvent(cUIThing* obj, unsigned int msg) __asm__("cFEStateBuyAttrib_onWidgetEvent");
};

CHECK_OFFSET(cFEStateBuyAttrib, mpAttribScreen, 0x048);
CHECK_OFFSET(cFEStateBuyAttrib, mExperience, 0x04C);
CHECK_OFFSET(cFEStateBuyAttrib, mExperienceLevel, 0x068);
CHECK_OFFSET(cFEStateBuyAttrib, mPotentialExperience, 0x084);
CHECK_OFFSET(cFEStateBuyAttrib, mTotalCost, 0x0A0);
CHECK_OFFSET(cFEStateBuyAttrib, mBank, 0x0A4);
CHECK_OFFSET(cFEStateBuyAttrib, mBuyPrompt, 0x0A8);
CHECK_OFFSET(cFEStateBuyAttrib, pAttribMenu, 0x0AC);
CHECK_SIZE(cFEStateBuyAttrib, 0xB0);
__asm__("_vt$17cFEStateBuyAttrib = cFEStateBuyAttrib_vtbl");   // g++'s name for the retail vtable

#endif
