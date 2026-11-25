#ifndef FE_FESTATEBUYATTRIB_H
#define FE_FESTATEBUYATTRIB_H

// cFEStateBuyAttrib: the lodge's attribute store screen (a cUIState); cFEReal creates it with cMemMan_alloc(176, "cFEStateBuyAttrib").
// Size 0xB0 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateBuyAttrib : public cUIState {
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

#endif
